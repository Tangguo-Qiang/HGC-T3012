/*
********************************************************************************
*文件名     : device_usart.c
*作用       : 串口设备
*原理       : 发送采用Buffer发送，接收采用App注册函数方式接收
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
*V0.2    Wangsw        2013/09/13       第一次更改
********************************************************************************
*/

#include "drive.h"
#include "system.h"

typedef struct 
{
    byte    * Start;            //指向队列开始 
    byte    * End;              //指向队列结束 
    byte    * In;               //插入一个消息  
    byte    * Out;              //取出一个消息  
    ushort    Entries;          //消息长度      
}QueueStruct;


#define Txd1BufferSum 100
#define Rxd1BufferSum 100
static QueueStruct Txd1Queue;
static QueueStruct Rxd1Queue;
static byte Txd1Buffer[Txd1BufferSum];
static byte Rxd1Buffer[Rxd1BufferSum];
static USART_InitTypeDef USART_InitStructure;
//static byte RecvStartFlag=0;
static byte RecvTimer=0;

static void InitQueue(void)
{
    Txd1Queue.Start = Txd1Buffer;
    Txd1Queue.End = Txd1Buffer + Txd1BufferSum - 1;
    Txd1Queue.In = Txd1Queue.Start;
    Txd1Queue.Out = Txd1Queue.Start;
    Txd1Queue.Entries = 0;

    Rxd1Queue.Start = Rxd1Buffer;
    Rxd1Queue.End = Rxd1Buffer + Rxd1BufferSum - 1;
    Rxd1Queue.In = Rxd1Queue.Start;
    Rxd1Queue.Out = Rxd1Queue.Start;
    Rxd1Queue.Entries = 0;
	
}

/*******************************************************************************
* 描述	    : 串口1数据缓冲区接收一个字节
* 输入参数  : data 字节类型，发送参数
* 返回参数  : bool 类型
*******************************************************************************/
static bool ReadByteFromUsart3(byte* data) 
{
    if (Rxd1Queue.Entries ==0) return (false);
    
    Rxd1Queue.Entries--;
    
    *data =*Rxd1Queue.Out++ ;	
    
    if (Rxd1Queue.Out > Rxd1Queue.End) 
        Rxd1Queue.Out = Rxd1Queue.Start;
    
    return (true);
}

/*******************************************************************************
* 描述	    : 串口1数据缓冲区发送一个字节
* 输入参数  : data 字节类型，发送参数
* 返回参数  : bool 类型
*******************************************************************************/
static bool WriteByteToUsart3(byte data) 
{
    if (Txd1Queue.Entries >= Txd1BufferSum) return (false);
    
    Txd1Queue.Entries++;
    
    *Txd1Queue.In++ = data;	
    
    if (Txd1Queue.In > Txd1Queue.End) 
        Txd1Queue.In = Txd1Queue.Start;
    
    return (true);
}

/*******************************************************************************
* 描述	    : 串口1发送一串数据
* 输入参数  : dataPointer 数据指针，sum 数据总数
*******************************************************************************/
static void WriteToUsart3(byte * dataPointer, int sum)
{
    if (sum)
    {
        while (sum--) 
            WriteByteToUsart3(*dataPointer++);
    }
    else
    {
        while (*dataPointer) 
        {
            if (*dataPointer == '\n')       //换行符
                WriteByteToUsart3('\r');    //增加回车符
            WriteByteToUsart3(*dataPointer++);
        }
    }
}

/*******************************************************************************
* 描述	    : Keil编译器支持printf函数，需要调用此函数，注意这个需要MicorLib支持
* 输入参数  : ch 实为byte类型，实际发送的数据
*           : f 设备，不需要处理
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //换行符
        WriteByteToUsart3('\r');            //增加回车符

    WriteByteToUsart3((byte)ch);

    return ch;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序发送缓冲区数据
*******************************************************************************/
void Usart3TransSystick1000Routine(void) 
{
	if (Txd1Queue.Entries == 0) return;

    if ((USART3->SR&0x40)==0) return;
    USART3->DR = *Txd1Queue.Out++;
    Txd1Queue.Entries--;
    
    if (Txd1Queue.Out > Txd1Queue.End) 
        Txd1Queue.Out = Txd1Queue.Start;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序接收缓冲区数据
*******************************************************************************/
void Usart3RecvSystick1000Routine(void) 
{
  if(RecvTimer)
	{
		RecvTimer--;
		if(!RecvTimer)
			PostMessage(MessageCommRecv, Rxd1Queue.Entries);
	}
		
}

/*******************************************************************************
* 描述	    : 串口中断处理函数
*******************************************************************************/
void USART3_IRQHandler(void) 
{
    
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
    {
			*Rxd1Queue.In++ = (byte)USART_ReceiveData(USART3);
			if (Rxd1Queue.In > Rxd1Queue.End) 
					Rxd1Queue.In = Rxd1Queue.Start;

			RecvTimer =5;
			if (Rxd1Queue.Entries >= Rxd1BufferSum)
			{
				PostMessage(MessageCommRecv, Rxd1Queue.Entries);;
				RecvTimer =0;
			}
			Rxd1Queue.Entries++;
			
    } 
} 

static void OpenUsart3(void)
{
	USART_Cmd(USART3, DISABLE);	
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);	
}

static void CloseUsart3(void)
{
	USART_Cmd(USART3, DISABLE);	
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);	
}



void InitUsart3(void) 
{
//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
//USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU ;//GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	
  
//USART 初始化设置
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启中断
   
    USART_Cmd(USART3, ENABLE);                              //使能串口 

    InitQueue();

    System.Device.Usart1.Open = OpenUsart3;
    System.Device.Usart1.Close = CloseUsart3;
    System.Device.Usart1.WriteByte = WriteByteToUsart3;
    System.Device.Usart1.ReadByte = ReadByteFromUsart3;
}



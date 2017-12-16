/*
********************************************************************************
*文件名     : device_usart.c
*作用       : 串口设备
*原理       : 发送采用Buffer发送，接收采用App注册函数方式接收

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
static __IO byte RecvTimer=0;

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
static bool ReadByteFromUsart1(byte* data) 
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
static bool WriteByteToUsart1(byte data) 
{
    if (Txd1Queue.Entries >= Txd1BufferSum) return (false);
    
    Txd1Queue.Entries++;
    
    *Txd1Queue.In++ = data;	
    
    if (Txd1Queue.In > Txd1Queue.End) 
        Txd1Queue.In = Txd1Queue.Start;
    
    return (true);
}



/*******************************************************************************
* 描述	    : Keil编译器支持printf函数，需要调用此函数，注意这个需要MicorLib支持
* 输入参数  : ch 实为byte类型，实际发送的数据
*           : f 设备，不需要处理
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //换行符
        WriteByteToUsart1('\r');            //增加回车符

    WriteByteToUsart1((byte)ch);

    return ch;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序发送缓冲区数据
*******************************************************************************/
void Usart1TransSystick1000Routine(void) 
{
	if (Txd1Queue.Entries == 0) return;

    if ((USART1->SR&0X40)==0) return;

    USART1->DR = *Txd1Queue.Out++;
//    USART1->DR = 0x46;
    Txd1Queue.Entries--;
    
    if (Txd1Queue.Out > Txd1Queue.End) 
        Txd1Queue.Out = Txd1Queue.Start;
	if (Txd1Queue.Entries == 0) return;
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序接收缓冲区数据
*******************************************************************************/
void Usart1RecvSystick1000Routine(void) 
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
void USART1_IRQHandler(void) 
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
			if(Rxd1Queue.Entries < Rxd1BufferSum)
			{
				*Rxd1Queue.In++ = (byte)USART_ReceiveData(USART1);
				if (Rxd1Queue.In > Rxd1Queue.End) 
						Rxd1Queue.In = Rxd1Queue.Start;

				RecvTimer =5;
				Rxd1Queue.Entries++;
				if (Rxd1Queue.Entries >= Rxd1BufferSum)
				{
					RecvTimer =1;
//					PostMessage(MessageCommRecv, Rxd1Queue.Entries);;
				}
			}
			else
			{
				USART_ReceiveData(USART1);
			}
			
    } 
} 

static void OpenUsart1(void)
{
	USART_Cmd(USART1, DISABLE);	
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);	
}

static void CloseUsart1(void)
{
	USART_Cmd(USART1, DISABLE);	
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);	
}



void InitUsart1(void) 
{
//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
//USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//;GPIO_Mode_AF_OD
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	
  

    InitQueue();
//USART 初始化设置
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启中断
   
    USART_Cmd(USART1, ENABLE);                              //使能串口 

    System.Device.Usart1.Open = OpenUsart1;
    System.Device.Usart1.Close = CloseUsart1;
    System.Device.Usart1.WriteByte = WriteByteToUsart1;
    System.Device.Usart1.ReadByte = ReadByteFromUsart1;
}



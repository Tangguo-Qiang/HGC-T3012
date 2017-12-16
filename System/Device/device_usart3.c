/*
********************************************************************************
*�ļ���     : device_usart.c
*����       : �����豸
*ԭ��       : ���Ͳ���Buffer���ͣ����ղ���Appע�ắ����ʽ����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
*V0.2    Wangsw        2013/09/13       ��һ�θ���
********************************************************************************
*/

#include "drive.h"
#include "system.h"

typedef struct 
{
    byte    * Start;            //ָ����п�ʼ 
    byte    * End;              //ָ����н��� 
    byte    * In;               //����һ����Ϣ  
    byte    * Out;              //ȡ��һ����Ϣ  
    ushort    Entries;          //��Ϣ����      
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
* ����	    : ����1���ݻ���������һ���ֽ�
* �������  : data �ֽ����ͣ����Ͳ���
* ���ز���  : bool ����
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
* ����	    : ����1���ݻ���������һ���ֽ�
* �������  : data �ֽ����ͣ����Ͳ���
* ���ز���  : bool ����
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
* ����	    : ����1����һ������
* �������  : dataPointer ����ָ�룬sum ��������
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
            if (*dataPointer == '\n')       //���з�
                WriteByteToUsart3('\r');    //���ӻس���
            WriteByteToUsart3(*dataPointer++);
        }
    }
}

/*******************************************************************************
* ����	    : Keil������֧��printf��������Ҫ���ô˺�����ע�������ҪMicorLib֧��
* �������  : ch ʵΪbyte���ͣ�ʵ�ʷ��͵�����
*           : f �豸������Ҫ����
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //���з�
        WriteByteToUsart3('\r');            //���ӻس���

    WriteByteToUsart3((byte)ch);

    return ch;
}

/*******************************************************************************
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ô˷�������ͻ���������
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
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ô˷��������ջ���������
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
* ����	    : �����жϴ�����
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
//GPIO�˿�����
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

//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	
  
//USART ��ʼ������
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //�����ж�
   
    USART_Cmd(USART3, ENABLE);                              //ʹ�ܴ��� 

    InitQueue();

    System.Device.Usart1.Open = OpenUsart3;
    System.Device.Usart1.Close = CloseUsart3;
    System.Device.Usart1.WriteByte = WriteByteToUsart3;
    System.Device.Usart1.ReadByte = ReadByteFromUsart3;
}



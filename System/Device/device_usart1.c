/*
********************************************************************************
*�ļ���     : device_usart.c
*����       : �����豸
*ԭ��       : ���Ͳ���Buffer���ͣ����ղ���Appע�ắ����ʽ����

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
* ����	    : ����1���ݻ���������һ���ֽ�
* �������  : data �ֽ����ͣ����Ͳ���
* ���ز���  : bool ����
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
* ����	    : ����1���ݻ���������һ���ֽ�
* �������  : data �ֽ����ͣ����Ͳ���
* ���ز���  : bool ����
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
* ����	    : Keil������֧��printf��������Ҫ���ô˺�����ע�������ҪMicorLib֧��
* �������  : ch ʵΪbyte���ͣ�ʵ�ʷ��͵�����
*           : f �豸������Ҫ����
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    if (ch == '\n')                         //���з�
        WriteByteToUsart1('\r');            //���ӻس���

    WriteByteToUsart1((byte)ch);

    return ch;
}

/*******************************************************************************
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ô˷�������ͻ���������
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
* ����	    : ϵͳ����1000/S,��1mSһ�ε��ô˷��������ջ���������
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
* ����	    : �����жϴ�����
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
//GPIO�˿�����
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

//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	
  

    InitQueue();
//USART ��ʼ������
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //�����ж�
   
    USART_Cmd(USART1, ENABLE);                              //ʹ�ܴ��� 

    System.Device.Usart1.Open = OpenUsart1;
    System.Device.Usart1.Close = CloseUsart1;
    System.Device.Usart1.WriteByte = WriteByteToUsart1;
    System.Device.Usart1.ReadByte = ReadByteFromUsart1;
}



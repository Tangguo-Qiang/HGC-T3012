/*
********************************************************************************
*文件名     : device_wifi.c
*作用       : 串口设备
*原理       : 发送采用Buffer发送，接收采用App注册函数方式接收
********************************************************************************
*/

#include "drive.h"
#include "system.h"


//通用帧头格式 
typedef struct
{
	unsigned char header;
	unsigned char length;
	unsigned char type;
	unsigned char number;
}GeneralHeader;


//Hekr各帧长度
typedef	enum
{
	ModuleQueryFrameLength = 0x07,
	ModuleProdKeyFrameLength = 0x16,
	ModuleResponseFrameLength = 0x0B,
	ErrorFrameLength = 0x07
}AllFrameLength;

//Hekr错误码取值
typedef	enum
{
	ErrorOperation = 0x01,
	ErrorSumCheck = 0x02,
	ErrorDataRange = 0x03,
	ErrorNoCMD = 0xFF
}AllErrorValue;

//Hekr各帧类型
typedef	enum
{
	DeviceUploadType = 0x01,
	ModuleDownloadType = 0x02,
	ModuleOperationType = 0xFE,
	ErrorFrameType = 0xFF
}AllFrameType;

//模块查询帧格式
typedef struct
{
	//通用帧头
	GeneralHeader header;
	//有效数据
	unsigned char CMD;
	unsigned char Reserved;
	//和校验
	unsigned char SUM;
}ModuleQueryFrame; 


//错误帧格式
typedef struct
{
	//通用帧头
	GeneralHeader header;
	//有效数据
	unsigned char ErrorCode;
	unsigned char Reserved;
	//和校验
	unsigned char SUM;
}ErrorFrame; 

#define HEKR_DATA_LEN 0x05u
#define HEKR_FRAME_HEADER 0x48u

#define WIFIBUFFER_LEN		(WIFIUSER_MAX_LEN+HEKR_DATA_LEN+1)

#define TxdWIFIBufferSum (WIFIBUFFER_LEN)
#define RxdWIFIBufferSum (WIFIBUFFER_LEN)

#ifdef __LANGJIE_CTRL
const byte ProdKey[16]={0x01,0xbf,0xb8,0x9b,0x7c,0xc2,0x23,0xa7,0x1d,0xcb,0x2a,0x96,0xba,0x66,0xcf,0x47};
#endif
#ifdef __MAILINSEN_CTRL
const byte ProdKey[16]={0x35,0x09,0x2a,0x9f,0x6c,0xc6,0x53,0xca,0x26,0x40,0xa0,0xdd,0x0b,0x4e,0x43,0xa5};
#endif

#ifdef __HAYDN_CTRL
const byte ProdKey[16]={0x01,0x96,0x35,0x4D,0x06,0x9D,0x20,0xC5,0x4B,0x87,0xA0,0x01,0xE1,0x18,0x76,0x29};
#endif


static byte TxdWifiBuffer[TxdWIFIBufferSum];
static byte RxdWifiBuffer[TxdWIFIBufferSum];
static byte* pTxValidDataBuffer;
static byte* pRxValidDataBuffer;
static byte* pModuleStatus;
static byte RecvTimer1ms = 0;
static byte TransTimer1ms = 0;
static byte Frame_No = 0;
static byte RxdDataLength = 0;
static byte RxdDataIndex = 0;

static unsigned char SumCalculate(unsigned char* data)
{
	unsigned char temp;
	unsigned char i;
	unsigned char len = data[1] - 1;
	temp = 0;
	for(i = 0;i < len; i++)
	{
			temp += data[i];
	}
	return temp;
}

static void HekrValidDataCopy(void)
{
	unsigned char len,i;
	len = RxdWifiBuffer[1]- HEKR_DATA_LEN;
	for(i = 0 ;i < len ; i++)
		*(pRxValidDataBuffer+i) = RxdWifiBuffer[i+4];
}

static unsigned char SumCheckIsErr(unsigned char* data)
{
	unsigned char temp = SumCalculate(data);
	unsigned char len = data[1] - 1;
	if(temp == data[len])
		return 0;
	return 1;
}

static void StartTransData(void)
{
	byte i,len =TxdWifiBuffer[1];
	TxdWifiBuffer[len-1] = SumCalculate(TxdWifiBuffer);
	TransTimer1ms=5;
	for(i=0;i<len;i++)
	{
		while((USART2->SR&0x40)==0)
		{
			if(!TransTimer1ms)
				break;
		}
    USART2->DR = (ushort)TxdWifiBuffer[i];//*pTxd++;
		TransTimer1ms=5;

	}

	
	
}

static void HekrSendFrame(byte *data)
{
	byte len = data[1];
	byte i=0;
	for(i=0;i<len;i++)
		TxdWifiBuffer[i]=*data++;
	for(i=0;i<len;i++)
	{
		while((USART2->SR&0x40)==0)
		{
			if(!TransTimer1ms)
				break;
		}
    USART2->DR = (ushort)TxdWifiBuffer[i];//*pTxd++;
		TransTimer1ms=5;

	}

}

static void ErrResponse(unsigned char data)
{
		TxdWifiBuffer[0] = HEKR_FRAME_HEADER;
		TxdWifiBuffer[1] = ErrorFrameLength;
		TxdWifiBuffer[2] = ErrorFrameType;
		TxdWifiBuffer[3] = Frame_No++;
		TxdWifiBuffer[4] = data;
		TxdWifiBuffer[5] = 0x00;
		StartTransData();
}

static void HekrModuleStateCopy(void)
{
	unsigned char len,i;
	len = RxdWifiBuffer[1]- HEKR_DATA_LEN;
	for(i = 0 ;i < len ; i++)
		*(pModuleStatus+i) = RxdWifiBuffer[i+4];
}


unsigned char HekrRecvDataHandle(void)
{
	//检查和校验
	if(SumCheckIsErr(RxdWifiBuffer))
	{
		ErrResponse(ErrorSumCheck);
		return RecvDataSumCheckErr;
	}
	//确认帧类型
	switch(RxdWifiBuffer[2])
	{
	case DeviceUploadType://MCU上传信息反馈 不需要处理 
	                        return MCU_UploadACK;
	case ModuleDownloadType://WIFI下传信息
	                        HekrSendFrame(RxdWifiBuffer);
	                        HekrValidDataCopy();
	                        return ValidDataUpdate;
	case ModuleOperationType://Hekr模块状态
													if(RxdWifiBuffer[1] != ModuleResponseFrameLength)
														return MCU_ControlModuleACK;
	                        HekrModuleStateCopy();
	                        return HekrModuleStateUpdate;
	case ErrorFrameType://上一帧发送错误	
	                        return LastFrameSendErr;
	default:ErrResponse(ErrorNoCMD);break;
	}
	return RecvDataUseless;
}

static void HekrValidDataUpload(byte len)
{
	unsigned char i;
	TxdWifiBuffer[0] = HEKR_FRAME_HEADER;
	TxdWifiBuffer[1] = len + 5;;
	TxdWifiBuffer[2] = DeviceUploadType;
	TxdWifiBuffer[3] = Frame_No++;
	for(i = 0; i < len ; i++)
		TxdWifiBuffer[i+4] = *(pTxValidDataBuffer+i);
	StartTransData();
}

static void HekrModuleControl(unsigned char data)
{
	TxdWifiBuffer[0] = HEKR_FRAME_HEADER;
	TxdWifiBuffer[1] = ModuleQueryFrameLength;
	TxdWifiBuffer[2] = ModuleOperationType;
	TxdWifiBuffer[3] = Frame_No++;
	TxdWifiBuffer[4] = data;
	TxdWifiBuffer[5] = 0x00;
	StartTransData();
}

static TestStatus HekrModuleStart(void)
{
	byte i;
	uint cnt=0;
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
	
	for(i=0;i<100;i++)
	{
		USART_Cmd(USART2, ENABLE);
		HekrModuleControl(ModuleQuery);	
		RxdDataIndex=0;
		StartTransData();	
		cnt = 0;
		while((RxdDataIndex<0x0B)&&(++cnt<72000))
		{
			if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET) 
				RxdWifiBuffer[RxdDataIndex++] = (byte)USART_ReceiveData(USART2);
		}
		if(cnt<72000)
		{
			if(RxdWifiBuffer[1] == ModuleResponseFrameLength)
			 i=200;
		}
		USART_Cmd(USART2, DISABLE);
	}
	
	if(i>=200)
	{
		if(RxdWifiBuffer[9]==0x01)
			System.Device.Led.LedModeSet(LED_GREEN,LED_TWINKLE);
		return PASSED;
	}
	else
		return FAILED;

}


static TestStatus HekrModuleProdKey(void)
{
	byte i;
	uint cnt=0;
	
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
	TxdWifiBuffer[0] = HEKR_FRAME_HEADER;
	TxdWifiBuffer[1] = ModuleProdKeyFrameLength;
	TxdWifiBuffer[2] = ModuleOperationType;
	TxdWifiBuffer[3] = Frame_No++;
	TxdWifiBuffer[4] = HekrProdKey;	
	for(i=0;i<16;i++)
	 TxdWifiBuffer[i+5] = ProdKey[i];
	
	for(i=0;i<0x17;i++)
	 RxdWifiBuffer[i] = 0;
	
	RxdDataIndex=0;
	StartTransData();	
	cnt = 0;
	while((RxdDataIndex<0x17)&&(++cnt<720000))
	{
		if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET) 
		{
			RxdWifiBuffer[RxdDataIndex] = (byte)USART_ReceiveData(USART2);
			while(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET);
			if(RxdDataIndex==0)
			{
				if(RxdWifiBuffer[0]==0x48)
					RxdDataIndex++;
			}
			else if(RxdWifiBuffer[0]==0x48)
				RxdDataIndex++;
		}
	}
	
	
	return Buffercmp((byte*)&RxdWifiBuffer[5],(byte*)&ProdKey[0],16);

}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序接收缓冲区数据
*******************************************************************************/
void Usart2RecvSystick1000Routine(void) 
{
  if(RecvTimer1ms)
	{
		RecvTimer1ms--;
		if(!RecvTimer1ms)
		{
			if((RxdDataIndex==RxdDataLength)&&RxdDataIndex)
			{
				PostMessage(MessageWifiRecv, HekrRecvDataHandle());
				RecvTimer1ms = 0;
			}
			RxdDataIndex =0;
			RxdDataLength =0;
		}
	}
		
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S,即1mS一次调用此服务程序发送缓冲区数据
*******************************************************************************/
void Usart2TransSystick1000Routine(void) 
{
//	if (TransDataLength == 0) return;

//    if ((USART2->SR&0x40)==0) return;
//    USART2->DR = *pTxd++;
//    TransDataLength--;
	if(TransTimer1ms)
		TransTimer1ms--;
    

}

/*******************************************************************************
* 描述	    : 串口中断处理函数
*******************************************************************************/
void USART2_IRQHandler(void) 
{
    
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
    {
			RxdWifiBuffer[RxdDataIndex] = (byte)USART_ReceiveData(USART2);
			if (RxdWifiBuffer[0]==HEKR_FRAME_HEADER) 
					RxdDataIndex++;

			if (RxdDataIndex==4)
			{
				RxdDataLength =RxdWifiBuffer[1];
				if(RxdDataLength>WIFIBUFFER_LEN)
					RxdDataIndex = 0;  // refuse to receive
			}

			RecvTimer1ms =100;
			if((RxdDataIndex==RxdDataLength)&&RxdDataIndex)
			{
				PostMessage(MessageWifiRecv, HekrRecvDataHandle());
				RxdDataIndex =0;
				RecvTimer1ms = 0;
				RxdDataLength =0;
			}
			
			if(RxdDataIndex>=WIFIBUFFER_LEN)
				RxdDataIndex = 0;  // refuse to receive
			
    } 
} 

static void InitWifi(byte* RxValidDateBuffer,byte* TxValidDateBuffer,byte* ModuleStatus)
{
	
	pRxValidDataBuffer=RxValidDateBuffer;
	pTxValidDataBuffer=TxValidDateBuffer;
	pModuleStatus = ModuleStatus;
  USART_Cmd(USART2, ENABLE);                              //使能串口 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启中断
	
}

void InitWifiUsart2(void) 
{
	USART_InitTypeDef USART_InitStructure;
//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
//USART2_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU ;//GPIO_Mode_AF_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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
  USART_Init(USART2, &USART_InitStructure);

   
  DelayMsNull(2000);
	
#ifdef __WIFI_TEST
  USART_Cmd(USART2, ENABLE);                              //使能串口 
//  if(HekrModuleStart()==PASSED)
//	{
		while(1)
		{
			System.Device.Led.LedModeSet(LED_GREEN,TURN_ON);
				if(HekrModuleProdKey() == PASSED)
					break;
			System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
				if(HekrModuleProdKey() == PASSED)
					break;
		}
//	}
//	else
//		System.Device.Led.LedModeSet(LED_RED,TURN_ON);
#endif
  USART_Cmd(USART2, DISABLE);                              //使能串口 
	System.Device.Led.LedModeSet(LED_RED,TURN_OFF);	
  System.Device.Wifi.InitWifi = InitWifi;
  System.Device.Wifi.HekrModuleControl = HekrModuleControl;
  System.Device.Wifi.HekrValidDataUpload = HekrValidDataUpload;
		
	
}


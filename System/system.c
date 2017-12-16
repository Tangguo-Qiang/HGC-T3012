/*
********************************************************************************
*文件名   : system.c
*作用     : 系统应用文件
*创建时间 : 2013.7.21
********************************************************************************
*/

#include "system.h"

uint Empty;
void Dummy(void) {};
	
byte GifTimer100ms=0;
//byte TimeOut1ms=0;

ushort CriticalNesting = 0;


static const byte HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

#define DelayUsTick (MainClock/9000000)

#define DelayMsTick (MainClock/9000)


void DelayNull(int times)
{
	while(times--);
}

void DelayUsNull(int times)
{
	while(times--)
		DelayNull(DelayUsTick);
}

void DelayMsNull(int times)
{
	while(times--)
		DelayNull(DelayMsTick);
}



byte HexToAscii(byte hex)
{
	Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}

/**
* @brief  Fills buffer.
* @param  pBuffer: pointer on the Buffer to fill
* @param  BufferLength: size of the buffer to fill
* @retval None
*/
void Fill_BufferZero(uint8_t *pBuffer, uint16_t BufferLength)
{
  
  /* Put in global buffer same values */
  while(BufferLength>0 )
  {
    *pBuffer++ = 0x00;
		BufferLength--;
  }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval PASSED: pBuffer1 identical to pBuffer2
*         FAILED: pBuffer1 differs from pBuffer2
*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength>0)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
		
		BufferLength--;
  }
  
  return PASSED;
}


/*******************************************************************************
* 函数名	: PostMessage
* 描述	    : 向LogicTask发送消息
* 输入参数  : messageType: 消息类型
*             data: 消息值
* 返回参数  : 无
*******************************************************************************/
void PostMessage(MessageEnum message, uint data)
{
    GetByte3FromUint(data) = message;
    System.OS.PostMessageQueue(data);	
}


extern void InitDevice(void);

static void Init(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x3000); 
	DisableIrq();
    
    
    InitGui();
	InitOs();
    InitDevice();
}

SystemStruct System = 
{
	Init,
};



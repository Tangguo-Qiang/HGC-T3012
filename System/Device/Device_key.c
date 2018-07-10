/*
********************************************************************************
*文件名     : device_key.c
*作用       : 按键设备
*原理       : 基于SystemTick 100/秒 10mS扫描获取按键值，按键值分为长按与短按两种
********************************************************************************

********************************************************************************
*/



#include "drive.h"
#include "system.h"


#define ScanKey1Pin15	PeIn(2)
#define ScanKey2Pin14	PeIn(3)
#define ScanKey3Pin13	PeIn(4)
#define ScanEnc3Pin00	PcIn(0)
#define ScanEnc2Pin02	PcIn(2)
#define ScanEnc1Pin01	PcIn(1)


#define ShortInterval       4		// 短按按键间隔，不得低于3  per10ms
#define LongInterval        100 //40		// 长按按键间隔
#define InvalidInterval     4       // 无效识别长度
#define DoubleHitInterval   50		// 防连续双击误动间隔
#define KeyBeepInterval     20      // 按键音长度

#define POSTKEY_INTERVALMIN_10MS			10

static byte KeyIntervalMin10ms=0;

static byte ScanData[3];

static byte ValidCounter = 0;
static byte InvalidCounter = 0;
static byte DoubleHitCounter = 0;

//static byte rollCWTimer1ms=0;
//static byte rollCCWTimer1ms=0;
//static byte rollCWFlag =0;
//static byte rollCCWFlag =0;



static byte RemapKey(byte scan) 
{
    switch(scan)
    {
        case 0xFE:  return(KeyPower);         
        case 0xFD:  return(KeyMode);       
        case 0xFB:  return(KeyRate);   
        case 0xF7:  return(KeyRollCenter);
        default:    return(invalid);  
    }
}

static byte RemapLongKey(byte scan) 
{
    switch(scan)
    {
        case 0xFE:  return(KeyLongPower);
        case 0xFD:  return(KeyLongMode);
        case 0xFC:  return(KeyLongPowerMode);
        case 0xFB:  return(KeyLongRate);
        case 0xF7:  return(KeyLongRollCenter);
        case 0xF3:  return(KeyLongRollCenterRate);
//        case 0xF6:  return(KeyLongRollCenterPower);
        default:    return(invalid);
    }	
}

static byte ScanPin(void)
{
    byte scan = invalid;   
    
    if(ScanEnc3Pin00 == 0)  scan &= 0xF7;
    if(ScanKey3Pin13 == 0)  scan &= 0xFB;
    if(ScanKey2Pin14 == 0)  scan &= 0xFD;
    if(ScanKey1Pin15 == 0)  scan &= 0xFE;
    

    return(scan);
}


/*******************************************************************************
* 描述	    : 系统节拍100/S，即10mS一次扫描获取按键值，在多次检测确认后，
*           : 发送按键的映射消息到LogicTask的消息队列中。
*******************************************************************************/
void KeySystick100Routine(void) 
{
    byte scan;
    byte key;

		if(KeyIntervalMin10ms)
			KeyIntervalMin10ms--;
		
    scan = ScanPin();
	
    if (scan != invalid) 
    {
        ValidCounter++;
        InvalidCounter = 0;
			
			
		
        if (ValidCounter == 1) 
            ScanData[0] = scan;
        else if(ValidCounter == 2)
				{
           ScanData[1] = scan;
					if (ScanData[0] != ScanData[1])
					{
						ScanData[0] = ScanData[1];
						ValidCounter =1;
					}
				}
        else if(ValidCounter == 3)
				{
            ScanData[2] = scan;
					if(ScanData[1] != ScanData[2])
					{
						ScanData[0] = ScanData[2];
						ValidCounter =1;
					}
				}
				else if(ScanData[0] != scan)
				{
					ScanData[0] = scan;
					ValidCounter =1;
				}
        else if (ValidCounter > LongInterval) 
            ValidCounter = LongInterval;
    }
    else
	  {
        InvalidCounter++;
        if (InvalidCounter >= InvalidInterval)
        {   
            InvalidCounter = InvalidInterval;
       
            if(DoubleHitCounter)
            {
                DoubleHitCounter--;
                ValidCounter = 0;
                return;
            }

            if (ValidCounter < ShortInterval) return;

//            if (ScanData[0] == ScanData[1])
//                key = ScanData[0];
//            else if (ScanData[0] == ScanData[2])
//                key = ScanData[0];
//            else if (ScanData[1] == ScanData[2])
//                key = ScanData[1];
            if (ScanData[0] == ScanData[1])
                key = ScanData[0];
            else if (ScanData[0] == ScanData[2])
                key = ScanData[0];
            else if (ScanData[1] == ScanData[2])
                key = ScanData[1];
            else
            {
                ValidCounter = 0;
                return;
            }

            if (ValidCounter == LongInterval) 
                key = RemapLongKey(key);
            else if (ValidCounter >= ShortInterval) 
                key = RemapKey(key);
            else
                key = invalid;
            
            if (key != invalid)
            {
              if(!KeyIntervalMin10ms)
							{
								PostMessage(MessageKey, key); 
								KeyIntervalMin10ms=POSTKEY_INTERVALMIN_10MS;
							}								
                DoubleHitCounter = DoubleHitInterval;
            }
            ValidCounter = 0;
    	} 
    }
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S，即1mS一次扫描获取按键值，在多次检测确认后，
*           : 发送按键的映射消息到LogicTask的消息队列中。

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if(rollCCWTimer1ms)
			rollCCWFlag=1;
		else
			rollCWTimer1ms = 10;
		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)	
	{
		if(rollCWTimer1ms)
			rollCWFlag=1;
		else
			rollCCWTimer1ms = 10;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
*******************************************************************************/
void KeyRollSystick1000Routine(void) 
{
	static byte encoder=0;
	
	encoder |= (0x03&(GPIO_ReadInputData(GPIOC)>>1));
	
	switch(encoder&0xf)
	{
		case 0x04:
			if(!KeyIntervalMin10ms)
			{
				PostMessage(MessageKey, KeyRollCW);
				KeyIntervalMin10ms=POSTKEY_INTERVALMIN_10MS;
			}
			break;
		case 0x0b:
			if(!KeyIntervalMin10ms)
			{
				PostMessage(MessageKey, KeyRollCW);
				KeyIntervalMin10ms=POSTKEY_INTERVALMIN_10MS;
			}
			break;
		case 0x07:
			if(!KeyIntervalMin10ms)
			{
				PostMessage(MessageKey, KeyRollCCW);
				KeyIntervalMin10ms=POSTKEY_INTERVALMIN_10MS;
			}
			break;
		case 0x80:
			if(!KeyIntervalMin10ms)
			{
				PostMessage(MessageKey, KeyRollCCW);
				KeyIntervalMin10ms=POSTKEY_INTERVALMIN_10MS;
			}
			break;
	}
	encoder <<=2;
	
	

	
}

void EXTI_RollENC_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

   
   /* Connect EXTI10 Line to PC1 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  
  /* Connect EXTI11 Line to PC2 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  
  /* Configure EXTI1 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;// EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure EXTI2 line */
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;// EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_Init(&EXTI_InitStructure);

  
}

void Exit_RollBall(FunctionalState newstate)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable and set EXTI1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = newstate;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable and set EXTI2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = newstate;
  NVIC_Init(&NVIC_InitStructure);
	
}

void InitKey(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC , ENABLE);

  /*Configure GPIO pin : PB13-15 BUTTON-KEY*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /*Configure GPIO pin : PC0-2 ENC*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;  
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	EXTI_RollENC_Config();
//	Exit_RollBall(ENABLE);

}


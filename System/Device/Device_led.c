/*
********************************************************************************
*文件名     : device_key.c
*作用       : 按键设备
*原理       : 基于SystemTick 100/秒 10mS扫描获取按键值，按键值分为长按与短按两种

********************************************************************************
*/



#include "drive.h"
#include "system.h"


#define LED_TWINKLE_GREEN			PbOut(1) ^= 0x01;
#define LED_OFF_GREEN					PbOut(1)=0;
#define LED_ON_GREEN					PbOut(1)=1;
#define LED_TWINKLE_RED				PbOut(0) ^= 0x01;
#define LED_OFF_RED						PbOut(0)=0;
#define LED_ON_RED						PbOut(0)=1;

static LedModeTypedef LedGreen=TURN_OFF;
static LedModeTypedef LedRed=TURN_OFF;
static LedModeTypedef LedSet=TURN_OFF;
//static LedModeTypedef LedSet=TURN_OFF;

static byte FreqLedPulse10us=0;
static byte LightLedFreezeTimes=0;
static byte LightLedWidth=0;
static byte LightOnLedFlag=0;



static void LedModeSet(LedTypedef led,LedModeTypedef mode) 
{
	if(led ==LED_GREEN)
	{
		switch(mode)
		{
			case TEMP_TWINKLE:
				if(LedGreen != TEMP_TWINKLE)
				{
					LedSet = LedGreen;
					LedGreen = TEMP_TWINKLE;
					TIM_Cmd(TIM6, DISABLE);  //light sleepled
					TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				}
				break;
			case LED_TWINKLE:
				LedGreen = LED_TWINKLE;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case TURN_ON:
				LedGreen = TURN_ON;
				LED_ON_GREEN;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case TURN_OFF:
				LedGreen = TURN_OFF;
				LED_OFF_GREEN;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case BREATH_ON:
				LedGreen = BREATH_ON;
				FreqLedPulse10us=99;
				LightLedFreezeTimes=0;
				LightLedWidth=100;
				LightOnLedFlag=0;
				TIM_Cmd(TIM6, ENABLE);  //breathing light 
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				break;
			default:
				LedGreen = TURN_OFF;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
		}
			
	}
	else if(led ==LED_RED)
	{
		switch(mode)
		{
			case TEMP_TWINKLE:
				if(LedRed != TEMP_TWINKLE)
				{
					LedSet = LedRed;
					LedRed = TEMP_TWINKLE;
					TIM_Cmd(TIM6, DISABLE);  //light sleepled
					TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				}
				break;
			case LED_TWINKLE:
				LedRed = LED_TWINKLE;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case TURN_ON:
				LedRed = TURN_ON;
				LED_ON_RED;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case TURN_OFF:
				LedRed = TURN_OFF;
				LED_OFF_RED;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
			case BREATH_ON:
//				LedRed = BREATH_ON;
//				TIM_Cmd(TIM6, ENABLE);  //breathing light 
//				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				break;
			default:
				LedRed = TURN_OFF;
				LED_OFF_RED;
				TIM_Cmd(TIM6, DISABLE);  //light sleepled
				TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
				break;
		}
			
	}
	else
	{
		if(mode == TURN_ON)
		{
			LED_ON_RED;
			LED_ON_GREEN;
		}
		else
		{
			LED_OFF_RED;
			LED_OFF_GREEN;
		}
			
	}
}





/*******************************************************************************
* 描述	    : 系统节拍100/S，即10mS一次扫描获取按键值，在多次检测确认后，
*           : 发送按键的映射消息到LogicTask的消息队列中。
*******************************************************************************/
void LedSystick10Routine(void) 
{
	static byte times=0;
	static byte flash100ms=0;
	
    if(LedGreen < TURN_ON) 
    {
			flash100ms++;
			if(flash100ms>2)
			{
				LED_TWINKLE_GREEN;
				flash100ms=0;
				if(LedGreen == TEMP_TWINKLE) 
				{
					times++;
					if(times>6)
					{
						LedModeSet(LED_GREEN,LedSet);
						times = 0;
					}
				}
			}
    }
    else if(LedRed < TURN_ON) 
    {
			flash100ms++;
			if(flash100ms>2)
			{
				LED_TWINKLE_RED;
				flash100ms=0;
				if(LedRed == TEMP_TWINKLE) 
				{
					times++;
					if(times>6)
					{
						LedModeSet(LED_RED,LedSet);
						times = 0;
					}
				}
			}
    }

}

void TIM6_IRQHandler(void) //10us
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //per 10us
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update );
		FreqLedPulse10us++;
		if(FreqLedPulse10us>100) //1ms
		{
			FreqLedPulse10us =0;
			LightLedFreezeTimes++;
			if(LightLedFreezeTimes>50) //60ms
			{
				LightLedFreezeTimes = 0;
				if(LightOnLedFlag) //lighter and lighter
				{
					LightLedWidth++;
					if(LightLedWidth>100)
						LightOnLedFlag = 0;
				}
				else
				{
					LightLedWidth--;
					if(LightLedWidth<2)
						LightOnLedFlag = 1;
				}
			}
		}
		if(FreqLedPulse10us==0)
		{
			LED_OFF_GREEN;
		}
		else if(FreqLedPulse10us== LightLedWidth)
		{
			LED_ON_GREEN;
		}
	}
}

/*******************************************************************************
* 描述	    : 系统节拍1000/S，即1mS一次扫描获取按键值，在多次检测确认后，
*           : 发送按键的映射消息到LogicTask的消息队列中。
*******************************************************************************/
static void TIM6_ModeConfig(void)  //休眠灯PWM发生器
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	uint16_t PrescalerValue;


  /* TIM9 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

/* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 10-1; //10us
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);


//  /* TIM4 enable counter */
//  TIM_Cmd(TIM6, ENABLE);
//  /* TIM IT enable */
//  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
  /* --------------------------NVIC Configuration -------------------------------*/
  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}



void InitLed(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

  /*Configure GPIO pin : PB0-1 LED*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;  
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	LED_OFF_GREEN;
	LED_OFF_RED;
	
	TIM6_ModeConfig();
	System.Device.Led.LedModeSet = LedModeSet;
}




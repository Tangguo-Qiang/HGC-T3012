/*
********************************************************************************
*�ļ���     : device_rtc.c
*����       : ����ʱ���豸�����32.768KHz���������ʱ�ӣ�һ�����ھ��Ȳ��߳���
*ԭ��       : ����ϵͳ���Ĳ�����ʱ�ӷ�Ƶ��õ�ʱ���֡��룬�˾��Ȳ��ߣ�����һ�㳡��

********************************************************************************
*/

#include "system.h"
#include "drive.h"

#define	RTC_Weekday_Monday             ((uint8_t)0x01)
#define	RTC_Weekday_Tuesday            ((uint8_t)0x02)
#define	RTC_Weekday_Wednesday          ((uint8_t)0x03)
#define	RTC_Weekday_Thursday           ((uint8_t)0x04)
#define	RTC_Weekday_Friday             ((uint8_t)0x05)
#define	RTC_Weekday_Saturday           ((uint8_t)0x06)
#define	RTC_Weekday_Sunday             ((uint8_t)0x07)

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


static RTC_TimeTypeDef* RtcPointer = (RTC_TimeTypeDef *)0;


/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * WARNING: this function will overflow on 2106-02-07 06:28:16 on
 * machines were long is 32-bit! (However, as time_t is signed, we
 * will already get problems at other places on 2038-01-19 03:14:08)

 *ADD by fire���������ڹ����е��������Ϊ����ʱ�䣬
 ������ת����ʱ���ʱ���Ҫ�ӱ���ʱ��ת��Ϊ��׼ʱ���ʱ���
 */
static uint32_t mktimev(RTC_TimeTypeDef *tm)
{

//	if ((int) (tm->Mon -= 2)<=0) //�����ʱʱ����2��ǰ
	if (tm->Mon <=2) //�����ʱʱ����2��ǰ
        {	/* 1..12 -> 11,12,1..10 */
		tm->Mon += 12;		/* Puts Feb last since it has leap day */
		tm->Mon -=2;
		tm->Year -= 1;
	}
	else
	{
		tm->Mon -=2;					
	}

	return (((
		(u32) (tm->Year/4 - tm->Year/100 + tm->Year/400 + 367*tm->Mon/12 + tm->Mday) +
			tm->Year*365 - 719499
	    )*24 + tm->Hour /* now have hours */
	  )*60 + tm->Min /* now have minutes */
	)*60 + tm->Sec-8*60*60; /* finally seconds */
	/*Add by fire: -8*60*60 ������ı���ʱ��ת��Ϊ��׼ʱ�䣬
	��д���ʱ���У�ȷ����ʱ��������Ϊ��׼��UNIXʱ���*/ 
	 
}
/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
 /*���㹫�� �����ڽ�����ʱ�䡾�롿ת����׼ʱ��ģʽ*/
static void GregorianDay(RTC_TimeTypeDef * tm)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear=tm->Year-1;

	/*����ӹ�ԪԪ�굽������ǰһ��֮��һ�������˶��ٸ�����*/
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

     /*������������һ��Ϊ���꣬�Ҽ������·���2��֮����������1�����򲻼�1*/
	if((tm->Year%4==0) &&
	   ((tm->Year%100!=0) || (tm->Year%400==0)) &&
	   (tm->Mon>2)) 
        {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} 
        else 
        {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->Mon-1] + tm->Mday; /*����ӹ�ԪԪ��Ԫ������������һ���ж�����*/

	tm->Wday=day%7;
}


/***********************************************
���洢���еļ���ֵ���룬��������ʱ�䣬UNIXʱ�����ת��Ϊ��׼ʱ���ʽ
ע�⣺��Ϊ��������ʱ��
******************************************************/
static void to_tm(uint32_t tim, RTC_TimeTypeDef * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	tm->Hour = hms / 3600;
	tm->Min = (hms % 3600) / 60;
	tm->Sec = (hms % 3600) % 60;

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->Year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(tm->Year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->Mon = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	tm->Mday = day + 1;

	/*
	 * Determine the day of week*/
	 
	GregorianDay(tm);
	
}

static void Time_Adjust_Ready(void)
{
		/* Enable PWR and BKP clocks��Դ�ӿ�ʱ�Ӻͱ��ݽӿ�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain PWR_CRȡ��������д����*/
	PWR_BackupAccessCmd(ENABLE);
}
/*
 * ��������Time_Adjust
 * ����  ��ʱ�����,��ʼ��ʱ��
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
//static void Time_Adjust(RTC_TimeTypeDef *tm)
static void Time_Adjust(void)
{
	Time_Adjust_Ready();
	/* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	
	  /* Get wday �ӹ�Ԫ0000�����𣬽������ڼ���
	  GregorianDay(tm);*/

	  /* �޸ĵ�ǰRTC�����Ĵ������� */
	  RTC_SetCounter(mktimev(RtcPointer));

	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
}

/*
 * ��������Time_Display
 * ����  ����ʾ��ǰʱ��ֵ
 * ����  ��-TimeVar RTC����ֵ����λΪ s
 * ���  ����
 * ����  ���ڲ�����
 */	
static void Time_To_BJ(uint32_t TimeVar,RTC_TimeTypeDef *tm)
{
	   uint32_t BJ_TimeVar;

	   /*  �ѱ�׼ʱ��ת��Ϊ����ʱ��*/
	   BJ_TimeVar =TimeVar + 8*60*60;

	   to_tm(BJ_TimeVar, tm);/*�Ѷ�ʱ����ֵת��Ϊ����ʱ��*/	

}


/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
static void RTC_Configuration(void)
{
	Time_Adjust_Ready();

	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}
static void RTC_SetFlag(byte reg)
{
   	  /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5*/
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	    Time_Adjust_Ready();
	switch(reg)
	{
		case 2:
			 BKP_WriteBackupRegister(BKP_DR2, 0xb5b5);
			break;
		case 3:
			 BKP_WriteBackupRegister(BKP_DR3, 0xc5c5);
			break;
		default:
			break;
	}

}

/*
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
//void RTC_CheckAndConfig(RTC_TimeTypeDef *tm)
void RTC_CheckAndConfig(void)
{
   	  /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
	  ������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
//	    printf("\r\n\r\n RTC not yet configured....");
		
	    /* RTC Configuration */
	    RTC_Configuration();
		
		/* Adjust time by users typed on the hyperterminal */
//	    Time_Adjust(tm);
	    Time_Adjust();
		
      BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
//	    /*��������������ʱ��*/
//		/*����Ƿ��������*/
//		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
//		{
//		    printf("\r\n\r\n Power On Reset occurred....");
//		}
//		/*����Ƿ�Reset��λ*/
//		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
//	    {
//	      printf("\r\n\r\n External Reset occurred....");
//	    }
//		
//		printf("\r\n No need to configure RTC....");
//		
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	
		
	  /* Clear reset flags */
	  RCC_ClearFlag();

}






/*
 * ��������Time_Show
 * ����  ���ڳ����ն�����ʾ��ǰʱ��ֵ
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */ 
static void Time_Update(RTC_TimeTypeDef *tm)
{

		/* update current time */
				
	  Time_To_BJ( RTC_GetCounter(),tm); 
	  
	    
	  
}


//static void Time_Check(RTC_TimeTypeDef *tm)
static void Time_Check(void)
{
	if(RtcPointer->Year>2059) RtcPointer->Year=2000;
	else if(RtcPointer->Year<2000) RtcPointer->Year = 2059;
	
	if(RtcPointer->Mon>12) RtcPointer->Mon=1;
	else if(RtcPointer->Mon<1) RtcPointer->Mon = 12;
	
	if(RtcPointer->Mday>(days_in_month(RtcPointer->Mon))) RtcPointer->Mday=1;
	else if(RtcPointer->Mday<1) RtcPointer->Mday = days_in_month(RtcPointer->Mon);
	
	if(RtcPointer->Wday>RTC_Weekday_Sunday) RtcPointer->Wday=RTC_Weekday_Monday;
	else if(RtcPointer->Wday<RTC_Weekday_Monday) RtcPointer->Wday = RTC_Weekday_Sunday;
	
	if(RtcPointer->Hour>23) RtcPointer->Hour=0;
	else if(RtcPointer->Hour<0) RtcPointer->Hour = 23;
	
	if(RtcPointer->Min>59) RtcPointer->Min=0;
	else if(RtcPointer->Min<0) RtcPointer->Min = 59;
	
	if(RtcPointer->Sec>59) RtcPointer->Sec=0;
	else if(RtcPointer->Sec<0) RtcPointer->Sec = 59;
}	

/**
  ��ʹ�����жϣ���������Ȼ����
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
*/  
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
/*     Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

/*     Enable time update */
//    TimeDisplayFlag =1;
	Time_Update(RtcPointer);



/*     Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
  }
}
/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
static void NVIC_RTC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
  * @brief  Enable or disable Interrupt per seconde.
  * @param  ENABLE or DISABLE 
  * @retval None
*/  
static void Time_Read(FunctionalState State)
{
		Time_Update(RtcPointer);
	
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		
		/*����RTC���ж�*/
		RTC_ITConfig(RTC_IT_SEC, State);
		
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	
}

/**
  * @brief  Configures EXTI Lines.
  * @param  None
  * @retval None
  */
static void EXTI17_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bits for Preemption Priority and 0 bits for Sub Priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure EXTI Line17(RTC Alarm) to generate an interrupt on rising edge */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = 	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

static void AlarmSet(uint TimeSec)
{
	if(TimeSec)
	{
		TimeSec += RTC_GetCounter();
//		/* Wait till RTC Second event occurs */
//		RTC_ClearFlag(RTC_FLAG_SEC);
//		while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);
		Time_Adjust_Ready();
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		RTC_SetAlarm(TimeSec);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Enable the RTC Alarm interrupt */
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
	}
	else
	{
		/* Disable the RTC Alarm interrupt */
		RTC_ITConfig(RTC_IT_ALR, DISABLE);
	}
	
}

/**
  * @brief  This function handles RTC Alarm interrupt request.
  * @param  None
  * @retval None
  */
void RTCAlarm_IRQHandler(void)
{
	
  if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {

    /* Clear EXTI line17 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line17);

//    /* Check if the Wake-Up flag is set */
//    if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//    {
//      /* Clear Wake Up flag */
//      PWR_ClearFlag(PWR_FLAG_WU);
//    }
		RTC_ITConfig(RTC_IT_SEC, DISABLE);
		while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();   
    /* Clear RTC Alarm interrupt pending bit */
    RTC_ClearITPendingBit(RTC_IT_ALR);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
		/* Disable the RTC Alarm interrupt */
		RTC_ITConfig(RTC_IT_ALR, DISABLE);
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		PostMessage(MessageAlarm, ALARM_SOUND);
		
		
   }

}

static void Register(RTC_TimeTypeDef *rtcPointer)
{
		/* Enable PWR and BKP clocks��Դ�ӿ�ʱ�Ӻͱ��ݽӿ�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  RtcPointer = rtcPointer;
	NVIC_RTC_Configuration();
  RTC_CheckAndConfig();  
	EXTI17_Configuration();
}


void InitRtc(void) 
{
	System.Device.Rtc.Register = Register;
	System.Device.Rtc.Time_Check = Time_Check;
	System.Device.Rtc.Time_Read = Time_Read;
	System.Device.Rtc.Time_Adjust = Time_Adjust;
	System.Device.Rtc.AlarmSet = AlarmSet;
	System.Device.Rtc.RTC_SetFlag = RTC_SetFlag;
}


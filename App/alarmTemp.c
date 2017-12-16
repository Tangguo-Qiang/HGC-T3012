#include "alarm.h"

#define DAY_SEC			(uint32_t)86400

uint waitsec=0;


void AlarmDeInit(void)
{
	App.AlarmSys.AirflowSet = AIRFLOW_STOP;
	App.AlarmSys.Alarmgoal = NOFLOW;
	App.AlarmSys.Nextnum=0;
}
/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void AlarmStart(void)
{
	DayTimersTypedef* pdaytimer;
	SysTimerTypedef* ptimer;
	byte *timermode;
	uint32_t nowsec=0,stopsec=0,startsec=0,setsec=0;
	uint8_t i,num;

	/* Enable the RTC Alarm  Interrupt */
	RTC_ITConfig(RTC_IT_ALR, DISABLE);
	
	if(App.Data.SysCtrlPara.WorkMode  == WEEKTIMER_ON)
	{
		System.Device.Rtc.Time_Read(ENABLE);
		nowsec = App.SysTime.Hour*3600+App.SysTime.Min*60+App.SysTime.Sec;
		timermode = (byte*)&App.Data.WeekTimerMode.MondayTimer;
				/*if today isn't Sunday*/
		if(App.SysTime.Wday>0)
			timermode += (App.SysTime.Wday-1);
		else
			timermode += 6;		
		switch(*timermode)
		{
			case EMPTYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
				break;
			case WORKDAYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.WorkDayTimer);
				break;
			case WEEKENDTYPE:
				pdaytimer = &(App.Data.DayTimerMode.WeekendTimer);
				break;
			case HOLIDAYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
				break;
			case OFFICETYPE:
				pdaytimer = &(App.Data.DayTimerMode.OfficeTimer);
				break;
			case ENERGYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.EnergyTimer);
				break;
		}
		ptimer = &(pdaytimer->Timer0);
//		AlarmDeInit();
		/* App.AlarmSys.Nextnum=0,find current airflow for now*/
		if(App.AlarmSys.Nextnum==0)
		{
			for(i=0; i<5;i++)
			{
				stopsec = ptimer->StopTime.Hour*3600;
				stopsec += ptimer->StopTime.Minute*60;
				if(stopsec >= nowsec)
					break;
				ptimer++;
			}
//			ptimer--;
			if(i<5) //available timers today exist 
			{
				/* startsec must be less 60seconds than DAY_SEC at least */
				startsec = ptimer->StartTime.Hour*3600;
				startsec += ptimer->StartTime.Minute*60;
				if(nowsec<startsec)
				{
					setsec = startsec-nowsec;
					App.AlarmSys.Alarmgoal = STARTFLOW;
					App.AlarmSys.AirflowSet = ptimer->AirFlowSet;
					App.AlarmSys.Nextnum = i+1;
					App.Data.SysCtrlPara.AirFlowAuto=AIRFLOW_STOP;
//					PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
				}
				/* nowsec is in the timer period running at some airflowset */
				else
				{
					App.Data.SysCtrlPara.AirFlowAuto =ptimer->AirFlowSet;
//					PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
					setsec = stopsec - nowsec;
					App.AlarmSys.Alarmgoal= STOPFLOW;
					App.AlarmSys.AirflowSet= AIRFLOW_STOP;
					i += 2;
					if(i>5)
						i=1;
					App.AlarmSys.Nextnum =(i<<4);
				}
			}
			else  //timers for this day has been runout, search for nextday timer
			{
				num = App.SysTime.Wday;
				i=0;
				do{
					num += 1;
					if(num==7)
						num =0;
					if(num>0)
					{
						timermode = (byte*)&App.Data.WeekTimerMode.MondayTimer;
						timermode += num-1;
					}
					else
						timermode = (byte*)&App.Data.WeekTimerMode.SundayTimer;
					if(*timermode != (byte)EMPTYTYPE)
						break;
					else
						i++;
				}while(i<7);
				if(i<7)
				{
					setsec = DAY_SEC*(i+1);
					switch(*timermode)
					{
						case EMPTYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
							break;
						case WORKDAYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.WorkDayTimer);
							break;
						case WEEKENDTYPE:
							pdaytimer = &(App.Data.DayTimerMode.WeekendTimer);
							break;
						case HOLIDAYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
							break;
						case OFFICETYPE:
							pdaytimer = &(App.Data.DayTimerMode.OfficeTimer);
							break;
						case ENERGYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.EnergyTimer);
							break;
					}
					ptimer = &(pdaytimer->Timer0);
					startsec = ptimer->StartTime.Hour*3600;
					startsec += ptimer->StartTime.Minute*60;
					setsec += startsec;
					setsec +=(DAY_SEC-nowsec);
					App.AlarmSys.AirflowSet = ptimer->AirFlowSet ;
					App.AlarmSys.Nextnum =0x01;
					App.AlarmSys.Alarmgoal = STARTFLOW;
					App.Data.SysCtrlPara.AirFlowAuto =AIRFLOW_STOP;
					//PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
				}
				else // no timer available in a week
				{
					App.Data.SysCtrlPara.AirFlowAuto =AIRFLOW_STOP;
					setsec=0;
					/* no timers available in a whole week
					motos power off*/
					//PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
				}
			}
		} //App.AlarmSys.Nextnum=0,find current airflow for now
		System.Device.Rtc.AlarmSet(setsec);
	}
	else
	{
		AlarmDeInit() ;
	}
	
}

static void AlarmContinue(void)
{
	DayTimersTypedef* pdaytimer;
	SysTimerTypedef* ptimer;
	byte *timermode;
	uint32_t nowsec=0,stopsec=0,startsec=0,setsec=0;
	uint8_t i,num,nextnum;

	/* Enable the RTC Alarm  Interrupt */
	RTC_ITConfig(RTC_IT_ALR, DISABLE);
	
	if(App.Data.SysCtrlPara.WorkMode  == WEEKTIMER_ON)
	{
		System.Device.Rtc.Time_Read(ENABLE);
		nowsec = App.SysTime.Hour*3600+App.SysTime.Min*60+App.SysTime.Sec;
		timermode = (byte*)&App.Data.WeekTimerMode.MondayTimer;
				/*if today isn't Sunday*/
		if(App.SysTime.Wday>0)
			timermode += (App.SysTime.Wday-1);
		else
			timermode += 6;		
		switch(*timermode)
		{
			case EMPTYTYPE:
//				pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
				break;
			case WORKDAYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.WorkDayTimer);
				break;
			case WEEKENDTYPE:
				pdaytimer = &(App.Data.DayTimerMode.WeekendTimer);
				break;
			case HOLIDAYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
				break;
			case OFFICETYPE:
				pdaytimer = &(App.Data.DayTimerMode.OfficeTimer);
				break;
			case ENERGYTYPE:
				pdaytimer = &(App.Data.DayTimerMode.EnergyTimer);
				break;
		}
		ptimer = &(pdaytimer->Timer0);
		/* App.AlarmSys.Nextnum=0,find current airflow for now*/
		if(App.AlarmSys.Nextnum !=0)
		{
			nextnum = App.AlarmSys.Nextnum;
			if(nextnum&0xF0)
			{
				nextnum >>=4;
				if(nextnum == 1)
				{
					num = App.SysTime.Wday;
					i=1;
					while(i<8)
					{
						num += 1;
						if(num==7)
							num =0;
						if(num>0)
						{
							timermode = (byte*)&App.Data.WeekTimerMode.MondayTimer;
							timermode += num-1;
						}
						else
							timermode = (byte*)&App.Data.WeekTimerMode.SundayTimer;
						if(*timermode != (byte)EMPTYTYPE)
							break;
						else
							i++;
					}
					setsec = DAY_SEC*(i-1);
					switch(*timermode)
					{
						case EMPTYTYPE:
//							pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
							break;
						case WORKDAYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.WorkDayTimer);
							break;
						case WEEKENDTYPE:
							pdaytimer = &(App.Data.DayTimerMode.WeekendTimer);
							break;
						case HOLIDAYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.HolidayTimer);
							break;
						case OFFICETYPE:
							pdaytimer = &(App.Data.DayTimerMode.OfficeTimer);
							break;
						case ENERGYTYPE:
							pdaytimer = &(App.Data.DayTimerMode.EnergyTimer);
							break;
					}
					ptimer = &(pdaytimer->Timer0);
					startsec = ptimer->StartTime.Hour*3600;
					startsec += ptimer->StartTime.Minute*60;
					setsec += startsec;
					setsec +=(DAY_SEC-nowsec);
					App.AlarmSys.AirflowSet = ptimer->AirFlowSet ;
					App.AlarmSys.Nextnum =0x01;
					App.AlarmSys.Alarmgoal = STARTFLOW;					
				}
				else
				{
					ptimer += nextnum-1;
					startsec = ptimer->StartTime.Hour*3600;
					startsec += ptimer->StartTime.Minute*60;		
					if(startsec>nowsec)
						setsec = startsec - nowsec;
					else
						setsec =0;
					App.AlarmSys.AirflowSet = ptimer->AirFlowSet ;
					App.AlarmSys.Nextnum = nextnum;
					App.AlarmSys.Alarmgoal = STARTFLOW;
				}
			}
			else
			{
				nextnum &= 0x0F;
				ptimer += nextnum-1;
				stopsec = ptimer->StopTime.Hour*3600;
				stopsec += ptimer->StopTime.Minute*60;
				if(stopsec>nowsec)
					setsec = stopsec- nowsec;
				else
					setsec =0;
				App.AlarmSys.AirflowSet = AIRFLOW_STOP ;
				App.AlarmSys.Alarmgoal = STOPFLOW;
				if(++nextnum>5)
					nextnum =1;				
				App.AlarmSys.Nextnum = nextnum<<4;
			}
			waitsec = setsec;
		if(setsec<10)
			PostMessage(MessageAlarm, ALARM_SOUND);
		else
			System.Device.Rtc.AlarmSet(setsec);
		}

	}					
	else
	{
		AlarmDeInit() ;
	}
	
}

/**
  * @brief  This function handles RTC Alarm interrupt request.
  * @param  None
  * @retval None
  */
void AlarmSound(void)
{
	if(App.AlarmSys.Alarmgoal == STARTFLOW)
	{
		App.Data.SysCtrlPara.AirFlowAuto = App.AlarmSys.AirflowSet;
	}
	else if(App.AlarmSys.Alarmgoal == STOPFLOW)
	{
		App.Data.SysCtrlPara.AirFlowAuto = AIRFLOW_STOP ;
	}
  AlarmContinue();
}

#include "app.h"


static uint* DispFlagPointer = &(App.Menu.MainForm.RefreshFlag) ;


void InitHmi(void)
{
	App.Menu.FocusFormPointer = &App.Menu.MainForm;
	DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
	*DispFlagPointer =MAINFORM_INIT;
	App.KeyBtn.Focus = FOCUS_ONFORM;
	App.KeyBtn.ParaTitle = FORM_FOCUS;
	App.KeyBtn.ParaSubTitle = FORM_FOCUS;
	App.KeyBtn.Position =FORM_FOCUS;
	App.KeyBtn.val = 0;
	
	PostMessage(MessageParaUpdate, PARA_WORKMODE);
	
}
/*******************************************************************************
* 描述	    : 按键消息处理函数
* 输入参数  : key，任意按键值
*******************************************************************************/
void KeyProcessMainForm(KeyActEnum key)
{
	if(App.Data.SysCtrlPara.Power == POWER_SLEEP)
	{
		System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
		CommTalk_Trans(COMM_BEEPONE);
		App.Data.SysCtrlPara.Power = POWER_ON;
		App.Data.SysCtrlPara.MuteSet = MUTEMODE_OFF;
		LCD_BL_ON;
		return;
	}

	if((App.Data.SysCtrlPara.Power == POWER_OFF)&&(key!=KeyPower))
		return;
	
  if(App.SysCtrlStatus.ChildLock==CHILD_LOCK)
	{
		System.Device.Led.LedModeSet(LED_GREEN,TEMP_TWINKLE);
		return;
	}
	
//	if((App.SysFault.FaultFlag)&&(!(App.SysFault.FaultFlag&FAULTICON_DISP)))
//	{
//		App.Menu.FocusFormPointer = &App.Menu.FaultInfoForm;
//		DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
//		*DispFlagPointer =FAUTFORM_INIT;
//		CommTalk_Trans(COMM_BEEPONE);
//		PostMessage(MessageProcess, FAULTINFO_DISP);
//		return;
//	}
	

		switch(key)
    {
			case KeyPower:
        CommTalk_Trans(COMM_BEEPLONG);
			break;
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRollCenter:
        CommTalk_Trans(COMM_BEEPTWO);
			  break;
			case KeyLongPower:
        CommTalk_Trans(COMM_BEEPLONG);
				break;
			case KeyLongMode:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRate:
        CommTalk_Trans(COMM_BEEPTHREE);
				break;
			case KeyLongRollCenterRate:
				break;
			case KeyRollCW:
			case KeyRollCCW:
//        CommTalk_Trans(COMM_BEEPONE);
				break;
		}
		switch(key)
    {
        case KeyPower:                
					if(App.Data.SysCtrlPara.Power == POWER_OFF)
						App.Data.SysCtrlPara.Power = POWER_ON;
					else
						App.Data.SysCtrlPara.Power = POWER_OFF;
					PostMessage(MessageParaUpdate, PARA_POWER_SET);
            break;
        case KeyMode:
					App.Data.SysCtrlPara.CircleModeSet++;
				if(App.Data.SysCtrlPara.CircleModeSet>CIRCLEMODE_IN)
					App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_AUTO;
					*DispFlagPointer |=MAINFORM_CICLEMODE;
				PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
            break;
        case KeyRate :
					App.Data.SysCtrlPara.VentilateRate++;
				if(App.Data.SysCtrlPara.VentilateRate>RATE10TO12)
					App.Data.SysCtrlPara.VentilateRate = RATE10TO06;
					*DispFlagPointer |=MAINFORM_VENTIRATE;
				PostMessage(MessageParaUpdate, PARA_VENTILATE);
            break;
        case KeyRollCenter:
						App.Menu.FocusFormPointer = &App.Menu.SysParaForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =SYSPARAFORM_INIT;
            break;
        case KeyLongPower:                
					if(App.Data.SysCtrlPara.Power == POWER_OFF)
					{
						App.Data.SysCtrlPara.Power = POWER_ON;
					}
					else
						App.Data.SysCtrlPara.Power = POWER_OFF;
					PostMessage(MessageParaUpdate, PARA_POWER_SET);
            break;
        case KeyLongMode:
					if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
					{
						App.Data.SysCtrlPara.WorkMode = WEEKTIMER_ON;
						App.SysCtrlStatus.AirFlowSet = App.Data.SysCtrlPara.AirFlowSet;
					}
					else
					{
						App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
						App.SysCtrlStatus.AirFlowSet = App.Data.SysCtrlPara.AirFlowAuto;
					}
					PostMessage(MessageParaUpdate, PARA_WORKMODE);
            break;
        case KeyLongRate :
					PostMessage(MessageWifiCtrl, ModuleRestart);
            break;
        case KeyLongRollCenter:
					PostMessage(MessageWifiCtrl, HekrConfig);
            break;
            
        case KeyLongRollCenterRate:
            break;
            
        case KeyLongPowerMode:
//					if(App.SysFault.PowerBaseFault)
//						PostMessage(MessageParaUpdate, PARA_SYSTEM_UPDATE);
            break;
        case KeyRollCW:
					if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
					{
            App.SysCtrlStatus.AirFlowSet++;
						if(App.SysCtrlStatus.AirFlowSet>AIRFLOW_STEP20)
							App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP20;
						App.Data.SysCtrlPara.AirFlowSet=App.SysCtrlStatus.AirFlowSet;
//						*DispFlagPointer |=MAINFORM_AIRFLOW;
						
						SeqOperDelay =SEQUENCEOPER_DELAY_100MS;
//						PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
					}
					else
						System.Device.Led.LedModeSet(LED_RED,TEMP_TWINKLE);
            break;
        case KeyRollCCW:
					if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND) 
					{						
            App.SysCtrlStatus.AirFlowSet--;
						if(App.SysCtrlStatus.AirFlowSet<AIRFLOW_STEP1)
							App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP1;
						App.Data.SysCtrlPara.AirFlowSet=App.SysCtrlStatus.AirFlowSet;
//						*DispFlagPointer |=MAINFORM_AIRFLOW;

						SeqOperDelay =SEQUENCEOPER_DELAY_100MS;
//						PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
					}
					else
						System.Device.Led.LedModeSet(LED_RED,TEMP_TWINKLE);
            break;
         
    }
}
void KeyProcessSysParaForm(KeyActEnum key)
{
		switch(key)
    {
			case KeyPower:
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRollCenter:
			case KeyLongPower:
			case KeyLongMode:
			case KeyLongRate:
        CommTalk_Trans(COMM_BEEPLONG);
				break;
			case KeyRollCW:
			case KeyRollCCW:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			default:
				break;
		}

	switch(key)
    {
        case KeyPower:                // 数字按键操作
            break;
        case KeyMode:
					App.KeyBtn.Focus--;
					if(App.KeyBtn.Focus<FOCUS_ONFORM)
						InitHmi();
					else
					{
						switch(App.KeyBtn.Focus)
						{
							case FOCUS_ONFORM:
								App.KeyBtn.Position = 0;
							break;
							case FOCUS_PARATITLE:
								if(App.KeyBtn.Position>14)
								{
									App.KeyBtn.Position= SYSPARA_SYSTEMTIME;
									
								PostMessage(MessageAlarm, ALARM_INIT);
								}
								else if(App.KeyBtn.Position>11)
								{
									App.KeyBtn.Position= SYSPARA_THERMALMODE;
								}
								else if(App.KeyBtn.Position>7)
								{
									App.KeyBtn.Position= SYSPARA_VERTIMODE;
								}
								else if(App.KeyBtn.Position>4)
								{
									App.KeyBtn.Position= SYSPARA_CIRCLEMODE;
								}	
								break;
							case FOCUS_PARASUBTITLE:
								*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;
							break;
							default:
								break;
						}
					}
					
            break;
        case KeyRate :
            break;
        case KeyRollCenter:
					App.KeyBtn.Focus++;
					if((App.KeyBtn.Focus==FOCUS_PARAVAL)&&(App.KeyBtn.ParaTitle<SYSPARA_SYSTEMTIME))
						App.KeyBtn.Focus=FOCUS_VERIFY;
					switch(App.KeyBtn.Focus)
					{
						case FOCUS_VERIFY:
							switch(App.KeyBtn.Position)
							{
								case SYSPARA_CIRCLEOUT:
									App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_OUT;
								PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
									break;
								case SYSPARA_CIRCLEIN:
									App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_IN;
								PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
									break;
								case SYSPARA_CIRCLEAUTO:
									App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_AUTO;
								PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
									break;
								case SYSPARA_RATE06:
									App.Data.SysCtrlPara.VentilateRate = RATE10TO06;
									PostMessage(MessageParaUpdate, PARA_VENTILATE);
									break;
								case SYSPARA_RATE08:
									App.Data.SysCtrlPara.VentilateRate = RATE10TO08;
									PostMessage(MessageParaUpdate, PARA_VENTILATE);
									break;
								case SYSPARA_RATE10:
									App.Data.SysCtrlPara.VentilateRate = RATE10TO10;
									PostMessage(MessageParaUpdate, PARA_VENTILATE);
									break;
								case SYSPARA_RATE12:
									App.Data.SysCtrlPara.VentilateRate = RATE10TO12;
									PostMessage(MessageParaUpdate, PARA_VENTILATE);
									break;
								case SYSPARA_THERMALOPEN:
									App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_ON;
								PostMessage(MessageParaUpdate, PARA_THERMALMODE);
									break;
								case SYSPARA_THERMALCLOSE:
									App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_OFF;
								PostMessage(MessageParaUpdate, PARA_THERMALMODE);
									break;
								case SYSPARA_THERMALAUTO:
									App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_AUTO;
								PostMessage(MessageParaUpdate, PARA_THERMALMODE);
									break;
								case SYSPARA_TIMEDATE:
									System.Device.Rtc.Time_Adjust();
								System.Device.Rtc.Time_Read(ENABLE);
									break;
								case SYSPARA_TIMEMIN:
									System.Device.Rtc.Time_Adjust();
								System.Device.Rtc.Time_Read(ENABLE);
									break;
								default:
									break;
							}
							App.KeyBtn.Focus = FOCUS_PARATITLE;
							if(App.KeyBtn.Position>14)
							{
								if(App.KeyBtn.ParaSubTitle>PARAVAL_DAY)
									App.KeyBtn.Position= SYSPARA_TIMEMIN;
								else
									App.KeyBtn.Position= SYSPARA_TIMEDATE;
								*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							App.KeyBtn.Focus= FOCUS_PARAVAL;//FOCUS_PARASUBTITLE;
							}
							else if(App.KeyBtn.Position>11)
							{
								App.KeyBtn.Position= SYSPARA_THERMALMODE;
								*DispFlagPointer |= SYSPARAFORM_THERMALMODE;
								PostMessage(MessageParaUpdate, PARA_THERMALMODE);
							}
							else if(App.KeyBtn.Position>7)
							{
								App.KeyBtn.Position= SYSPARA_VERTIMODE;
								*DispFlagPointer |= SYSPARAFORM_VERIRATE;
								PostMessage(MessageParaUpdate, PARA_VENTILATE);								
							}
							else if(App.KeyBtn.Position>4)
							{
								App.KeyBtn.Position= SYSPARA_CIRCLEMODE;
								*DispFlagPointer |= SYSPARAFORM_CIRCLEMODE; 
								PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
							}
						break;
						case FOCUS_ONFORM:
							break;
						case FOCUS_PARASUBTITLE:
							switch(App.KeyBtn.ParaTitle)
							{
								case SYSPARA_CIRCLEMODE:
									App.KeyBtn.Position = SYSPARA_CIRCLEOUT;
									break;
								case SYSPARA_VERTIMODE:
									App.KeyBtn.Position = SYSPARA_RATE06;
									break;
								case SYSPARA_THERMALMODE:
									App.KeyBtn.Position = SYSPARA_THERMALOPEN;
									break;
								case SYSPARA_SYSTEMTIME:
									App.KeyBtn.Position = SYSPARA_TIMEDATE;
									break;
							}
							break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle = SYSPARA_CIRCLEMODE;
						  App.KeyBtn.Position = SYSPARA_CIRCLEMODE;
							break;
						case FOCUS_PARAVAL:
							if(App.KeyBtn.Position==SYSPARA_TIMEDATE)
								App.KeyBtn.ParaSubTitle = PARAVAL_YEAR;
							else
								App.KeyBtn.ParaSubTitle = PARAVAL_HOUR;
						*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME; 
							break;
						case FOCUS_PARAVALSET:
							System.Device.Rtc.Time_Read(DISABLE);
							switch(App.KeyBtn.ParaSubTitle)
							{
								case PARAVAL_YEAR:
								App.KeyBtn.val = (void*)&(App.SysTime.Year) ;
								break;
								case PARAVAL_MONTH:
								App.KeyBtn.val = (void*)&(App.SysTime.Mon) ;
								break;
								case PARAVAL_DAY:
								App.KeyBtn.val = (void*)&(App.SysTime.Mday) ;
								break;
								case PARAVAL_HOUR:
								App.KeyBtn.val = (void*)&(App.SysTime.Hour) ;
								break;
								case PARAVAL_MIN:
								App.KeyBtn.val = (void*)&(App.SysTime.Min) ;
								break;
								default:
								App.KeyBtn.ParaSubTitle	=PARAVAL_MIN;
								App.KeyBtn.val = (void*)&(App.SysTime.Min) ;
								break;
							}
							*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME; 
							break;
					}
            break;
        case KeyLongPower:                
            break;
        case KeyLongMode:
            break;
        case KeyLongRate :
					PostMessage(MessageWifiCtrl, ModuleRecover);
            break;
        case KeyLongRollCenter:
            break;
            
        case KeyRollCW:					
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.WeekTimerForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =WEEKTIMERFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle++;
						  App.KeyBtn.Position++;
							if(App.KeyBtn.ParaTitle>SYSPARA_SYSTEMTIME)
							{
								App.KeyBtn.Position = SYSPARA_CIRCLEMODE;
								App.KeyBtn.ParaTitle=SYSPARA_CIRCLEMODE;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.Position++;
						  switch(App.KeyBtn.ParaTitle)
							{
								case SYSPARA_CIRCLEMODE:
									if(App.KeyBtn.Position>SYSPARA_CIRCLEAUTO)
										App.KeyBtn.Position = SYSPARA_CIRCLEOUT;
									break;
								case SYSPARA_VERTIMODE:
									if(App.KeyBtn.Position>SYSPARA_RATE12)
										App.KeyBtn.Position = SYSPARA_RATE06;
									break;
								case SYSPARA_THERMALMODE:
									if(App.KeyBtn.Position>SYSPARA_THERMALAUTO)
										App.KeyBtn.Position = SYSPARA_THERMALOPEN;
									break;
								case SYSPARA_SYSTEMTIME:
									if(App.KeyBtn.Position>SYSPARA_TIMEMIN)
										App.KeyBtn.Position = SYSPARA_TIMEDATE;
									break;
								default:
									break;
								
							}
							break;
						case FOCUS_PARAVAL:
							App.KeyBtn.ParaSubTitle++;
							*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							if(App.KeyBtn.Position==SYSPARA_TIMEDATE)
							{
								if(App.KeyBtn.ParaSubTitle>PARAVAL_DAY)
									App.KeyBtn.ParaSubTitle=PARAVAL_YEAR;
							}
							else
							{
								if(App.KeyBtn.ParaSubTitle>PARAVAL_MIN)
									App.KeyBtn.ParaSubTitle=PARAVAL_HOUR;
							}
							
							break;
						case FOCUS_PARAVALSET:
							switch(App.KeyBtn.ParaSubTitle)
							{
								case PARAVAL_YEAR:
									(*(ushort*)App.KeyBtn.val)++ ;
								break;
								case PARAVAL_MONTH:
									(*(ushort*)App.KeyBtn.val)++ ;
								break;
								case PARAVAL_DAY:
									(*(ushort*)App.KeyBtn.val)++ ;
								break;
								case PARAVAL_HOUR:
									(*(short*)App.KeyBtn.val)++ ;
								break;
								case PARAVAL_MIN:
									(*(short*)App.KeyBtn.val)++ ;
								break;
								default:
								break;
							}
							System.Device.Rtc.Time_Check();							
							*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							break;	
						default:
							break;
					}
						
            break;
        case KeyRollCCW:
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.MainForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =MAINFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle--;
						  App.KeyBtn.Position--;
							if(App.KeyBtn.ParaTitle<SYSPARA_CIRCLEMODE)
							{
								App.KeyBtn.Position = SYSPARA_SYSTEMTIME;
								App.KeyBtn.ParaTitle=SYSPARA_SYSTEMTIME;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.Position--;
						  switch(App.KeyBtn.ParaTitle)
							{
								case SYSPARA_CIRCLEMODE:
									if(App.KeyBtn.Position<SYSPARA_CIRCLEOUT)
										App.KeyBtn.Position =SYSPARA_CIRCLEAUTO ;
									break;
								case SYSPARA_VERTIMODE:
									if(App.KeyBtn.Position<SYSPARA_RATE06)
										App.KeyBtn.Position =SYSPARA_RATE12 ;
									break;
								case SYSPARA_THERMALMODE:
									if(App.KeyBtn.Position<SYSPARA_THERMALOPEN)
										App.KeyBtn.Position =SYSPARA_THERMALAUTO ;
									break;
								case SYSPARA_SYSTEMTIME:
									if(App.KeyBtn.Position<SYSPARA_TIMEDATE)
										App.KeyBtn.Position =SYSPARA_TIMEMIN ;
									break;
								default:
									break;
								
							}
							break;
						case FOCUS_PARAVAL:
							App.KeyBtn.ParaSubTitle--;
							if(App.KeyBtn.Position==SYSPARA_TIMEDATE)
							{
								if(App.KeyBtn.ParaSubTitle<PARAVAL_YEAR)
									App.KeyBtn.ParaSubTitle=PARAVAL_DAY;
							}
							else
							{
								if(App.KeyBtn.ParaSubTitle<PARAVAL_HOUR)
									App.KeyBtn.ParaSubTitle=PARAVAL_MIN;
							}
								
							*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							break;
						case FOCUS_PARAVALSET:
							switch(App.KeyBtn.ParaSubTitle)
							{
								case PARAVAL_YEAR:
									(*(ushort*)App.KeyBtn.val)-- ;
								break;
								case PARAVAL_MONTH:
									(*(ushort*)App.KeyBtn.val)-- ;
								break;
								case PARAVAL_DAY:
									(*(ushort*)App.KeyBtn.val)-- ;
								break;
								case PARAVAL_HOUR:
									(*(short*)App.KeyBtn.val)-- ;
								break;
								case PARAVAL_MIN:
									(*(short*)App.KeyBtn.val)-- ;
								break;
								default:
								break;
							}
							System.Device.Rtc.Time_Check();							
							*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							break;	
						default:
							break;
					}
						
					break;
		}
}

void KeyProcessWeekTimerForm(KeyActEnum key)
{
		switch(key)
    {
			case KeyPower:
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRollCenter:
			case KeyLongPower:
			case KeyLongMode:
			case KeyLongRate:
				break;
			case KeyRollCW:
			case KeyRollCCW:
        CommTalk_Trans(COMM_BEEPONE);
				break;
		}

		switch(key)
    {
        case KeyPower:                // 数字按键操作
            break;
        case KeyMode:
					App.KeyBtn.Focus--;
					if(App.KeyBtn.Focus==FOCUS_ONFORM)
					{
						App.KeyBtn.Position = 0;
					}
					else if(App.KeyBtn.Focus<FOCUS_ONFORM)
						InitHmi();
					else
					{
						App.KeyBtn.ParaSubTitle = 0;
							switch(App.KeyBtn.Position)
							{
								case WEEKTIMER_MONDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP1;         
									break;
								case WEEKTIMER_TUESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP2;         
									break;
								case WEEKTIMER_WEDNESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP3;         
									break;
								case WEEKTIMER_THURSDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP4;         
									break;
								case WEEKTIMER_FRIDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP5;         
									break;
								case WEEKTIMER_SATURDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP6;         
									break;
								case WEEKTIMER_SUNDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP7;         
									break;
						}	
					}							
            break;
        case KeyRate :
            break;
        case KeyRollCenter:
					App.KeyBtn.Focus++;
					if(App.KeyBtn.Focus==FOCUS_PARAVAL)
						App.KeyBtn.Focus=FOCUS_VERIFY;
					switch(App.KeyBtn.Focus)
					{
						case FOCUS_VERIFY:
							App.KeyBtn.Focus = FOCUS_PARATITLE;
//							*((byte*)App.KeyBtn.val+ App.KeyBtn.Position-1) = App.KeyBtn.ParaSubTitle;
							switch(App.KeyBtn.ParaTitle)
							{
								case WEEKTIMER_MONDAY:
									if(App.Data.WeekTimerMode.MondayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.MondayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.MondayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP1;
								break;
								case WEEKTIMER_TUESDAY:
									if(App.Data.WeekTimerMode.TuesdayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.TuesdayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.TuesdayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP2;
									break;
								case WEEKTIMER_WEDNESDAY:
									if(App.Data.WeekTimerMode.WednsdayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.WednsdayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.WednsdayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP3;
									break;
								case WEEKTIMER_THURSDAY:
									if(App.Data.WeekTimerMode.ThurdayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.ThurdayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.ThurdayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP4;
									break;
								case WEEKTIMER_FRIDAY:
									if(App.Data.WeekTimerMode.FridayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.FridayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.FridayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP5;
									break;
								case WEEKTIMER_SATURDAY:
									if(App.Data.WeekTimerMode.SaturdayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.SaturdayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.SaturdayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP6;
									break;
								case WEEKTIMER_SUNDAY:
									if(App.Data.WeekTimerMode.SundayTimer != (TimerTypesTypedef)App.KeyBtn.ParaSubTitle)
										App.Data.WeekTimerMode.SundayTimer = (TimerTypesTypedef)App.KeyBtn.ParaSubTitle;
									else
										App.Data.WeekTimerMode.SundayTimer = EMPTYTYPE;
									App.KeyBtn.ParaSubTitle = 0;
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP7;
									break;
								default:
									break;
							}
						PostMessage(MessageAlarm, ALARM_INIT);
						StorePost(STORE_WEEKTIMER);
							App.WifiState |= WIFI_UPLOAD_WEEKTIMER;							
						break;
						case FOCUS_ONFORM:
							break;
						case FOCUS_PARASUBTITLE:
								App.KeyBtn.ParaSubTitle = 1;
//								App.KeyBtn.val =(void*)&App.Data.WeekTimerMode.MondayTimer ;
							switch(App.KeyBtn.Position)
							{
								case WEEKTIMER_MONDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP1;         
									break;
								case WEEKTIMER_TUESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP2;         
									break;
								case WEEKTIMER_WEDNESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP3;         
									break;
								case WEEKTIMER_THURSDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP4;         
									break;
								case WEEKTIMER_FRIDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP5;         
									break;
								case WEEKTIMER_SATURDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP6;         
									break;
								case WEEKTIMER_SUNDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP7;         
									break;
						}	
							break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle = WEEKTIMER_MONDAY;
						  App.KeyBtn.Position = WEEKTIMER_MONDAY;
							break;
						case FOCUS_PARAVAL:
//							App.KeyBtn.ParaSubTitle = PARAVAL_YEAR;
//							App.KeyBtn.val = (void*)&(App.Data.SysTime.tm_year) ;
							break;
						case FOCUS_PARAVALSET:
							/*  flash effect */
							break;
					}
            break;
        case KeyLongPower:                // 数字按键操作
            break;
        case KeyLongMode:
            break;
        case KeyLongRate :
            break;
        case KeyLongRollCenter:
            break;
            
        case KeyRollCW:					
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.FilterForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =WEEKTIMERFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle++;
						  App.KeyBtn.Position++;
							if(App.KeyBtn.ParaTitle>WEEKTIMER_SUNDAY)
							{
								App.KeyBtn.Position = WEEKTIMER_MONDAY;
								App.KeyBtn.ParaTitle=WEEKTIMER_MONDAY;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.ParaSubTitle++;
						if(App.KeyBtn.ParaSubTitle>5)
							App.KeyBtn.ParaSubTitle=1;
							switch(App.KeyBtn.Position)
							{
								case WEEKTIMER_MONDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP1;         
									break;
								case WEEKTIMER_TUESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP2;         
									break;
								case WEEKTIMER_WEDNESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP3;         
									break;
								case WEEKTIMER_THURSDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP4;         
									break;
								case WEEKTIMER_FRIDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP5;         
									break;
								case WEEKTIMER_SATURDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP6;         
									break;
								case WEEKTIMER_SUNDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP7;         
									break;
						

							}
							break;
						case FOCUS_PARAVAL:
							
							break;
						case FOCUS_PARAVALSET:
							break;	
						default:
							break;
					}
						
            break;
        case KeyRollCCW:
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.SysParaForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =SYSPARAFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle--;
						  App.KeyBtn.Position--;
							if(App.KeyBtn.ParaTitle<WEEKTIMER_MONDAY)
							{
								App.KeyBtn.Position = WEEKTIMER_SUNDAY;
								App.KeyBtn.ParaTitle=WEEKTIMER_SUNDAY;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.ParaSubTitle--;
						if(App.KeyBtn.ParaSubTitle<1)
							App.KeyBtn.ParaSubTitle=5;

							switch(App.KeyBtn.Position)
							{
								case WEEKTIMER_MONDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP1;         
									break;
								case WEEKTIMER_TUESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP2;         
									break;
								case WEEKTIMER_WEDNESDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP3;         
									break;
								case WEEKTIMER_THURSDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP4;         
									break;
								case WEEKTIMER_FRIDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP5;         
									break;
								case WEEKTIMER_SATURDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP6;         
									break;
								case WEEKTIMER_SUNDAY:
									*DispFlagPointer |= WEEKTIMERFORM_CHECKGROUP7;         
									break;				

							}
							break;
						case FOCUS_PARAVAL:
							
							break;
						case FOCUS_PARAVALSET:
							break;	
						default:
							break;
					}
						
            break;

		}
}

void KeyProcessFilterForm(KeyActEnum key)
{
		switch(key)
    {
			case KeyPower:
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRollCenter:
			case KeyLongPower:
			case KeyLongMode:
			case KeyLongRate:
				break;
			case KeyRollCW:
			case KeyRollCCW:
        CommTalk_Trans(COMM_BEEPONE);
				break;
		}
	
	switch(key)
    {
        case KeyPower:                // 数字按键操作
            break;
        case KeyMode:
					App.KeyBtn.Focus--;
				if(App.KeyBtn.Focus<FOCUS_ONFORM)
						InitHmi();
					else
					{			
						if(App.KeyBtn.Focus==FOCUS_PARAVAL)
							App.KeyBtn.Focus=FOCUS_PARASUBTITLE;
					
						switch(App.KeyBtn.Focus)
						{
							case FOCUS_VERIFY:
							break;
							case FOCUS_ONFORM:
								App.KeyBtn.Position = 0;
								break;
							case FOCUS_PARASUBTITLE:
								switch(App.KeyBtn.Position)
								{
									case PARATITLE_FIRSTFILTER:
										*DispFlagPointer |= FILTERFORM_FIRST;
									break;
									case PARATITLE_MIDFILTER:
										*DispFlagPointer |= FILTERFORM_MID;
										break;
									case PARATITLE_ESPFILTER:
										*DispFlagPointer |= FILTERFORM_ESP;
										break;
									case PARATITLE_HPFILTER:
										*DispFlagPointer |= FILTERFORM_HP;
										break;
									default:
										break;
							}	
								break;
							case FOCUS_PARATITLE:
								switch(App.KeyBtn.Position)
								{
									case PARATITLE_FIRSTFILTER:
										*DispFlagPointer |= FILTERFORM_FIRST;
									break;
									case PARATITLE_MIDFILTER:
										*DispFlagPointer |= FILTERFORM_MID;
										break;
									case PARATITLE_ESPFILTER:
										*DispFlagPointer |= FILTERFORM_ESP;
										break;
									case PARATITLE_HPFILTER:
										*DispFlagPointer |= FILTERFORM_HP;
										break;
									default:
										break;
							}	
								break;
							case FOCUS_PARAVAL:
								break;
							case FOCUS_PARAVALSET:
		
								break;
						}						
						
					}							
            break;
        case KeyRate :
            break;
        case KeyRollCenter:
					App.KeyBtn.Focus++;
					if(App.KeyBtn.Focus==FOCUS_PARAVAL)
					{
						if(App.KeyBtn.ParaSubTitle>1)
							App.KeyBtn.Focus=FOCUS_PARAVALSET;
						else
							App.KeyBtn.Focus=FOCUS_PARASUBTITLE;
					}	
				
					switch(App.KeyBtn.Focus)
					{
						case FOCUS_VERIFY:
							App.KeyBtn.Focus= FOCUS_PARATITLE;
//							*((byte*)App.KeyBtn.val+ App.KeyBtn.Position-1) = App.KeyBtn.ParaSubTitle;
//							switch(App.KeyBtn.ParaTitle)
//							{
//								case PARATITLE_FIRSTFILTER:
//									App.Data.FilterHourLimit.FirstFilterHourLimit = *(ushort*)App.KeyBtn.val;
//									*DispFlagPointer |= FILTERFORM_FIRST;
//								break;
//								case PARATITLE_MIDFILTER:
//									App.Data.FilterHourLimit.MidFilterHourLimit = *(ushort*)App.KeyBtn.val;
//									*DispFlagPointer |= FILTERFORM_MID;
//									break;
//								case PARATITLE_ESPFILTER:
//									App.Data.FilterHourLimit.ESPFilterHourLimit = *(ushort*)App.KeyBtn.val;
//									*DispFlagPointer |= FILTERFORM_ESP;
//									break;
//								case PARATITLE_HPFILTER:
//									App.Data.FilterHourLimit.HPFilterHourLimit = *(ushort*)App.KeyBtn.val;
//									*DispFlagPointer |= FILTERFORM_HP;
//									break;
//								default:
//									break;
//							}
							App.KeyBtn.Focus = FOCUS_PARATITLE;
							App.KeyBtn.ParaSubTitle = SUBTITLE_EMPTY;
							App.WifiState |= WIFI_UPLOAD_FILTER;
							PostMessage(MessageProcess, FILTER_CHECK);
							StorePost(STORE_FILTERLIMIT);
						break;
						case FOCUS_ONFORM:
							break;
						case FOCUS_PARASUBTITLE:
								App.KeyBtn.ParaSubTitle = SUBTITLE_RESET;
//								App.KeyBtn.val =(void*)&App.Data.WeekTimerMode.MondayTimer ;
							switch(App.KeyBtn.Position)
							{
								case PARATITLE_FIRSTFILTER:
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
						}	
							break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle = PARATITLE_FIRSTFILTER;
						  App.KeyBtn.Position = PARATITLE_FIRSTFILTER;
							break;
						case FOCUS_PARAVAL:
							break;
						case FOCUS_PARAVALSET:
							/*  flash effect */
							switch(App.KeyBtn.Position)
							{
								case PARATITLE_FIRSTFILTER:
  								App.KeyBtn.val = (void*)&(App.Data.FilterHourLimit.FirstFilterHourLimit) ;
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
  								App.KeyBtn.val = (void*)&(App.Data.FilterHourLimit.MidFilterHourLimit) ;
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
  								App.KeyBtn.val = (void*)&(App.Data.FilterHourLimit.ESPFilterHourLimit) ;
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
  								App.KeyBtn.val = (void*)&(App.Data.FilterHourLimit.HPFilterHourLimit) ;
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
							}								
							break;
					}
            break;
        case KeyLongPower:                // 数字按键操作
            break;
        case KeyLongMode:
            break;
        case KeyLongRate :
					App.KeyBtn.Focus = FOCUS_PARATITLE;
					switch(App.KeyBtn.Position)
					{
						case PARATITLE_FIRSTFILTER:
  						App.Data.FilterTimerHour.FirstFilterTimerHour=0 ;
							*DispFlagPointer |= FILTERFORM_FIRST;
							CommTalk_Trans(COMM_BEEPTHREE);
	
						App.SysFault.FaultFlag &= ~FIRSTFILTER_CHECK;
//						PostMessage(MessageProcess, FILTER_CLEAR);
						break;
						case PARATITLE_MIDFILTER:
  						App.Data.FilterTimerHour.MidFilterTimerHour=0 ;
							*DispFlagPointer |= FILTERFORM_MID;
							CommTalk_Trans(COMM_BEEPTHREE);
						
							App.SysFault.FaultFlag &= ~MIDFILTER_CHECK;
//						PostMessage(MessageProcess, FILTER_CLEAR);
						break;
						case PARATITLE_ESPFILTER:
  						App.Data.FilterTimerHour.ESPFilterTimerHour=0 ;
							*DispFlagPointer |= FILTERFORM_ESP;
							CommTalk_Trans(COMM_BEEPTHREE);
						
							App.SysFault.FaultFlag &= ~ESPFILTER_CHECK;
//						PostMessage(MessageProcess, FILTER_CLEAR);
						break;
						case PARATITLE_HPFILTER:
  						App.Data.FilterTimerHour.HPFilterTimerHour=0 ;
							*DispFlagPointer |= FILTERFORM_HP;
							CommTalk_Trans(COMM_BEEPTHREE);
						
							App.SysFault.FaultFlag &= ~HPFILTER_CHECK;
//							PostMessage(MessageProcess, FILTER_CLEAR);
						break;
						default:
						break;
					}
					StorePost(STORE_FILTERTIMER);
					
						break;       
        case KeyLongRollCenter:
            break;
            
        case KeyRollCW:					
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.ModeCtrlForm;
							DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
							*DispFlagPointer =MODECTRLFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle++;
						  App.KeyBtn.Position++;
							if(App.KeyBtn.ParaTitle>PARATITLE_HPFILTER)
							{
								App.KeyBtn.Position = PARATITLE_FIRSTFILTER;
								App.KeyBtn.ParaTitle=PARATITLE_FIRSTFILTER;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.ParaSubTitle++;
						if(App.KeyBtn.ParaSubTitle>SUBTITLE_LIMITHOUR)
							App.KeyBtn.ParaSubTitle=SUBTITLE_RESET;
							switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_FIRSTFILTER:
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
							}

							break;
						case FOCUS_PARAVAL:
							
							break;
						case FOCUS_PARAVALSET:
							switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_FIRSTFILTER:
									 *(ushort*)App.KeyBtn.val += 50;
									if(*(ushort*)App.KeyBtn.val>5000)
										*(ushort*)App.KeyBtn.val = 2000;
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
									 *(ushort*)App.KeyBtn.val += 50;
									if(*(ushort*)App.KeyBtn.val>5000)
										*(ushort*)App.KeyBtn.val = 2000;
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
									 *(ushort*)App.KeyBtn.val += 50;
									if(*(ushort*)App.KeyBtn.val>2000)
										*(ushort*)App.KeyBtn.val = 300;
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
									 *(ushort*)App.KeyBtn.val += 50;
									if(*(ushort*)App.KeyBtn.val>5000)
										*(ushort*)App.KeyBtn.val = 3000;
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
							}
							break;	
						default:
							break;
					}
						
            break;
        case KeyRollCCW:
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.WeekTimerForm;
							DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
							*DispFlagPointer =WEEKTIMERFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle--;
						  App.KeyBtn.Position--;
							if(App.KeyBtn.ParaTitle<PARATITLE_FIRSTFILTER)
							{
								App.KeyBtn.Position = PARATITLE_HPFILTER;
								App.KeyBtn.ParaTitle=PARATITLE_HPFILTER;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.ParaSubTitle--;
						if(App.KeyBtn.ParaSubTitle<SUBTITLE_RESET)
							App.KeyBtn.ParaSubTitle=SUBTITLE_LIMITHOUR;
							switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_FIRSTFILTER:
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
							}

							break;
						case FOCUS_PARAVAL:
							
							break;
						case FOCUS_PARAVALSET:
							switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_FIRSTFILTER:
									if(*(ushort*)App.KeyBtn.val>=2050)
									 *(ushort*)App.KeyBtn.val -= 50;
									else
										*(ushort*)App.KeyBtn.val = 5000;
									*DispFlagPointer |= FILTERFORM_FIRST;
								break;
								case PARATITLE_MIDFILTER:
									if(*(ushort*)App.KeyBtn.val>=2050)
									 *(ushort*)App.KeyBtn.val -= 50;
									else
										*(ushort*)App.KeyBtn.val = 5000;
									*DispFlagPointer |= FILTERFORM_MID;
									break;
								case PARATITLE_ESPFILTER:
									if(*(ushort*)App.KeyBtn.val>=350)
									 *(ushort*)App.KeyBtn.val -= 50;
									else
										*(ushort*)App.KeyBtn.val = 2000;
									*DispFlagPointer |= FILTERFORM_ESP;
									break;
								case PARATITLE_HPFILTER:
									if(*(ushort*)App.KeyBtn.val>=3050)
									 *(ushort*)App.KeyBtn.val -= 50;
									else
										*(ushort*)App.KeyBtn.val = 5000;
									*DispFlagPointer |= FILTERFORM_HP;
									break;
								default:
									break;
							}
							break;	
						default:
							break;
					}
						
						
            break;

		}
	
}

void KeyProcessModeCtrlForm(KeyActEnum key)
{
		switch(key)
    {
			case KeyPower:
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				break;
			case KeyLongRollCenter:
			case KeyLongPower:
			case KeyLongMode:
			case KeyLongRate:
				break;
			case KeyRollCW:
			case KeyRollCCW:
        CommTalk_Trans(COMM_BEEPONE);
				break;
		}
	
	switch(key)
    {
        case KeyPower:                // 数字按键操作
            break;
        case KeyMode:
					App.KeyBtn.Focus--;
					if(App.KeyBtn.Focus==FOCUS_ONFORM)
					{
						App.KeyBtn.Position = 0;
					}
					else if(App.KeyBtn.Focus<FOCUS_ONFORM)
						InitHmi();
					else
					{
							if(App.KeyBtn.Position>MODECTRL_29OC)
							{
								App.KeyBtn.Position= MODECTRL_CO2CHAR;
					//			*DispFlagPointer |= SYSPARAFORM_SYSTEMTIME;         
							}
							else if(App.KeyBtn.Position>MODECTRL_250UG)
							{
								App.KeyBtn.Position= MODECTRL_TEMPCHAR;
					//			*DispFlagPointer |= SYSPARAFORM_VERIRATE;         
							}
							else if(App.KeyBtn.Position>MODECTRL_CO2CHAR)
							{
								App.KeyBtn.Position= MODECTRL_PMCHAR;
					//			*DispFlagPointer |= SYSPARAFORM_CIRCLEMODE;         
							}	
						}							
            break;
        case KeyRate :
            break;
        case KeyRollCenter:
					App.KeyBtn.Focus++;
				if(App.KeyBtn.Focus==FOCUS_PARAVAL)
					App.KeyBtn.Focus=FOCUS_VERIFY;
					switch(App.KeyBtn.Focus)
					{
						case FOCUS_VERIFY:
							App.KeyBtn.Focus= FOCUS_ONFORM;
							switch(App.KeyBtn.Position)
							{
								case MODECTRL_100UG:
									App.Data.SysCtrlLine.PMInsideLine = 100;
									break;
								case MODECTRL_150UG:
									App.Data.SysCtrlLine.PMInsideLine = 150;
									break;
								case MODECTRL_200UG:
									App.Data.SysCtrlLine.PMInsideLine = 200;
									break;
								case MODECTRL_250UG:
									App.Data.SysCtrlLine.PMInsideLine = 250;
									break;
								case MODECTRL_14OC:
									App.Data.SysCtrlLine.TempInsideWinterLine = 14;
									break;
								case MODECTRL_15OC:
									App.Data.SysCtrlLine.TempInsideWinterLine = 15;
									break;
								case MODECTRL_16OC:
									App.Data.SysCtrlLine.TempInsideWinterLine = 16;
									break;
								case MODECTRL_17OC:
									App.Data.SysCtrlLine.TempInsideWinterLine = 17;
									break;
								case MODECTRL_18OC:
									App.Data.SysCtrlLine.TempInsideWinterLine = 18;
									break;
								case MODECTRL_25OC:
									App.Data.SysCtrlLine.TempInsideSummerLine = 25;
									break;
								case MODECTRL_26OC:
									App.Data.SysCtrlLine.TempInsideSummerLine = 26;
									break;
								case MODECTRL_27OC:
									App.Data.SysCtrlLine.TempInsideSummerLine = 27;
									break;
								case MODECTRL_28OC:
									App.Data.SysCtrlLine.TempInsideSummerLine = 28;
									break;
								case MODECTRL_29OC:
									App.Data.SysCtrlLine.TempInsideSummerLine = 29;
									break;
								case MODECTRL_800PPM:
									App.Data.SysCtrlLine.CO2InsideLine = 800;
									break;
								case MODECTRL_1000PPM:
									App.Data.SysCtrlLine.CO2InsideLine = 1000;
									break;
								case MODECTRL_1200PPM:
									App.Data.SysCtrlLine.CO2InsideLine = 1200;
									break;
								default:
									break;
							}
							App.KeyBtn.Focus = FOCUS_PARATITLE;
							if(App.KeyBtn.Position>MODECTRL_29OC)
							{
								App.KeyBtn.Position= MODECTRL_CO2CHAR;
								*DispFlagPointer |= MODECTRLFORM_CO2;         
							}
							else if(App.KeyBtn.Position>MODECTRL_250UG)
							{
								App.KeyBtn.Position= MODECTRL_TEMPCHAR;
								*DispFlagPointer |= MODECTRLFORM_TEMP;         
							}
							else if(App.KeyBtn.Position>MODECTRL_CO2CHAR)
							{
								App.KeyBtn.Position= MODECTRL_PMCHAR;
								*DispFlagPointer |= MODECTRLFORM_PM;         
							}
							App.WifiState |= WIFI_UPLOAD_ENVPARA;
							StorePost(STORE_ENVPARA);
						break;
						case FOCUS_ONFORM:
							break;
						case FOCUS_PARASUBTITLE:
							switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_PMCTRLSET:
									App.KeyBtn.Position = MODECTRL_100UG;
									break;
								case PARATITLE_TEMPCTRLSET:
									App.KeyBtn.Position = MODECTRL_14OC;
									break;
								case PARATITLE_CO2CTRLSET:
									App.KeyBtn.Position = MODECTRL_800PPM;
									break;
								default:
									break;
							}
							break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle = PARATITLE_PMCTRLSET;
						  App.KeyBtn.Position = MODECTRL_PMCHAR;
							break;
						case FOCUS_PARAVAL:
							break;
						case FOCUS_PARAVALSET:
							/*  flash effect */
							break;
					}
            break;
        case KeyLongPower:                // 数字按键操作
            break;
        case KeyLongMode:
            break;
        case KeyLongRate :
            break;
        case KeyLongRollCenter:
            break;
            
        case KeyRollCW:					
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
#ifdef __FAULT_DISP							
							if(App.SysFault.FaultFlag&SYSFAULT_FLAG)
							{
								App.Menu.FocusFormPointer = &App.Menu.FaultInfoForm;
								DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
								*DispFlagPointer =FAUTFORM_INIT;
							}
							else
#endif
							{
								InitHmi();
							}
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle++;
						  App.KeyBtn.Position++;
							if(App.KeyBtn.ParaTitle>PARATITLE_CO2CTRLSET)
							{
								App.KeyBtn.ParaTitle=PARATITLE_PMCTRLSET;
								App.KeyBtn.Position =MODECTRL_PMCHAR ;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.Position++;
						  switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_PMCTRLSET:
									if(App.KeyBtn.Position>MODECTRL_250UG)
										App.KeyBtn.Position = MODECTRL_100UG;
									break;
								case PARATITLE_TEMPCTRLSET:
									if(App.KeyBtn.Position>MODECTRL_29OC)
										App.KeyBtn.Position = MODECTRL_14OC;
									break;
								case PARATITLE_CO2CTRLSET:
									if(App.KeyBtn.Position>MODECTRL_1200PPM)
										App.KeyBtn.Position = MODECTRL_800PPM;
									break;
								default:
									break;
								
							}
							break;
						case FOCUS_PARAVAL:
			
							break;
						case FOCUS_PARAVALSET:
       
							break;	
						default:
							break;
					}
						
            break;
        case KeyRollCCW:
          switch(App.KeyBtn.Focus)
					{
						case FOCUS_ONFORM:
							App.Menu.FocusFormPointer = &App.Menu.FilterForm;
						DispFlagPointer = &(App.Menu.FocusFormPointer->RefreshFlag);
						*DispFlagPointer =FILTERFORM_INIT;
						break;
						case FOCUS_PARATITLE:
							App.KeyBtn.ParaTitle--;
						  App.KeyBtn.Position--;
							if(App.KeyBtn.ParaTitle<PARATITLE_PMCTRLSET)
							{
								App.KeyBtn.ParaTitle=PARATITLE_CO2CTRLSET;
								App.KeyBtn.Position = MODECTRL_CO2CHAR;
							}
							break;
						case FOCUS_PARASUBTITLE:
						  App.KeyBtn.Position--;
						  switch(App.KeyBtn.ParaTitle)
							{
								case PARATITLE_PMCTRLSET:
									if(App.KeyBtn.Position<MODECTRL_100UG)
										App.KeyBtn.Position =MODECTRL_250UG ;
									break;
								case PARATITLE_TEMPCTRLSET:
									if(App.KeyBtn.Position<MODECTRL_14OC)
										App.KeyBtn.Position =MODECTRL_29OC ;
									break;
								case PARATITLE_CO2CTRLSET:
									if(App.KeyBtn.Position<MODECTRL_800PPM)
										App.KeyBtn.Position =MODECTRL_1200PPM ;
									break;
								default:
									break;
								
							}
							break;
						case FOCUS_PARAVAL:

							break;
						case FOCUS_PARAVALSET:

							break;	
						default:
							break;
					}
						
					break;
		}
	
}

void KeyProcessFaultInfoForm(KeyActEnum key)
{
		switch(key)
    {
			case KeyPower:
			case KeyMode:
			case KeyRate:
			case KeyRollCenter:
        CommTalk_Trans(COMM_BEEPONE);
				InitHmi();
				break;
			case KeyLongRollCenter:
			case KeyLongPower:
			case KeyLongMode:
			case KeyLongRate:
				break;
			case KeyRollCW:
			case KeyRollCCW:
				break;
		}

}

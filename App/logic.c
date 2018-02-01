/*
********************************************************************************
*文件名     : logic.c
*作用       : 业务逻辑处理文件
*原理       : 处理各类消息，如按键、传感器等，标准化一些按键的处理模式
********************************************************************************

********************************************************************************
*/


#include "app.h"
#include "StorePara.h"
#include "alarm.h"

#define SCREEN_WAITLIGHT_1S		120

typedef struct{
	uint16_t		Moto1PwmPace;
	uint16_t  	Moto1RpmPace;
	uint16_t  	Moto2PwmPace;
	uint16_t  	Moto2RpmPace;
}SysIndex_TypeDef;
//PWM_DUTYOCCUPY_MOTO1ACT
SysIndex_TypeDef SysIndex={PWM_DUTYOCCUPY_OUT_MOTO1STEP,RPM_MOTO1_OUT_STEP,0,0};
uint16_t MotoPFStopPwm = PWM_DUTYOCCUPY_MOTO2STOP;

byte SeqOperDelay= 0; 
														
static ushort UseTimer10s=0;
static byte OperTimer1s=SCREEN_WAITLIGHT_1S;
//static byte LogicDelayMs=10;
static byte StoreDelay=0;

void SetVentiMoto2Act(void)
{
///	App.SysCtrlStatus.VentilateRate = App.Data.SysCtrlPara.VentilateRate ;
	switch(App.Data.SysCtrlPara.VentilateRate )
	{
		case RATE10TO06:
			SysIndex.Moto2PwmPace = (uint16_t)((PWM_DUTYOCCUPY_MOTO2_RATE10TO06)/AIRFLOW_TOTAL_STEPS);
			SysIndex.Moto2RpmPace = (uint16_t)((RPM_MOTO2_MAX06-RPM_MOTO2_MIN)/AIRFLOW_TOTAL_STEPS);
			break;
		case RATE10TO08:
			SysIndex.Moto2PwmPace = (uint16_t)((PWM_DUTYOCCUPY_MOTO2_RATE10TO08)/AIRFLOW_TOTAL_STEPS);
			SysIndex.Moto2RpmPace = (uint16_t)((RPM_MOTO2_MAX08-RPM_MOTO2_MIN)/AIRFLOW_TOTAL_STEPS);
			break;
		case RATE10TO10:
			SysIndex.Moto2PwmPace = (uint16_t)((PWM_DUTYOCCUPY_MOTO2_RATE10TO10)/AIRFLOW_TOTAL_STEPS);
			SysIndex.Moto2RpmPace = (uint16_t)((RPM_MOTO2_MAX10-RPM_MOTO2_MIN)/AIRFLOW_TOTAL_STEPS);
			break;
		case RATE10TO12:
			SysIndex.Moto2PwmPace = (uint16_t)((PWM_DUTYOCCUPY_MOTO2_RATE10TO12)/AIRFLOW_TOTAL_STEPS);
			SysIndex.Moto2RpmPace = (uint16_t)((RPM_MOTO2_MAX12-RPM_MOTO2_MIN)/AIRFLOW_TOTAL_STEPS);
			break;
		default:
			App.Data.SysCtrlPara.VentilateRate = RATE10TO08;
			SysIndex.Moto2PwmPace = (uint16_t)((PWM_DUTYOCCUPY_MOTO2_RATE10TO08)/AIRFLOW_TOTAL_STEPS);
			SysIndex.Moto2RpmPace = (uint16_t)((RPM_MOTO2_MAX08-RPM_MOTO2_MIN)/AIRFLOW_TOTAL_STEPS);
			break;
	}	
}

void GetMotoXFOcupy(uint8_t airflow)
{
	uint32_t temp;
	
	if(airflow>AIRFLOW_STEP20)
		airflow=AIRFLOW_STEP20;
	if(airflow )
	{
		temp = (SysIndex.Moto1RpmPace*airflow);
		temp += RPM_MOTO1_MIN;
		App.SysCtrlStatus.XFmotoRPM= (uint16_t)temp;
		
				
		temp = SysIndex.Moto1PwmPace*airflow;
		temp += (PWM_DUTYOCCUPY_MOTO1MIN);
		App.SysCtrlStatus.XFmotoPWM= (uint16_t)temp; 
	}
	else
	{
		App.SysCtrlStatus.XFmotoPWM= 0;
		App.SysCtrlStatus.XFmotoRPM =0;
	}
}
		
void GetMotoPFOcupy(uint8_t airflow)
{
	uint32_t temp;
	
	if(airflow>AIRFLOW_STEP20)
		airflow=AIRFLOW_STEP20;
	if((App.SysCtrlStatus.BypassMode != BYPASS_CIRCLEIN)&&(App.SysCtrlStatus.ThermalMode != HEATMODE_ON))
	{
		if(airflow )
		{
			temp = (SysIndex.Moto2RpmPace*airflow);
			temp += RPM_MOTO2_MIN;
			App.SysCtrlStatus.PFmotoRPM= (uint16_t)temp;
			
					
			temp = SysIndex.Moto2PwmPace*airflow;
			temp += (PWM_DUTYOCCUPY_MOTO2MIN);
			App.SysCtrlStatus.PFmotoPWM= (uint16_t)temp; 
			
		}
		else
		{
			App.SysCtrlStatus.PFmotoPWM= 0;
			App.SysCtrlStatus.PFmotoRPM =0;
		}
	}
  else
	{
			App.SysCtrlStatus.PFmotoPWM=  MotoPFStopPwm;
		App.SysCtrlStatus.PFmotoRPM = RPM_MOTO2_STOP;//350;
	}		
	
}

#ifdef __SELF_ADJUSTMOTO

#endif

/*******************************************************************************
* 描述	    : 根据传感器数据判定运行模式
*******************************************************************************/
void ParseEchoData(byte data)
{
	switch(data)
	{
		case COMM_TEMPER_RH:
			if(App.Data.SysCtrlPara.ThermalModeSet == HEATMODE_AUTO)
			{
				if(App.Data.SensorData.TempOutside>22)
				{
					if((App.Data.SensorData.TempInside>App.Data.SysCtrlLine.TempInsideSummerLine)&&(App.Data.SensorData.TempOutside<=(App.Data.SysCtrlLine.TempInsideSummerLine-2)))
						 App.SysCtrlStatus.ThermalMode = HEATMODE_ON;
					else
						App.SysCtrlStatus.ThermalMode =  HEATMODE_OFF;
				}
				else if(App.Data.SensorData.TempOutside<21)
				{
					if((App.Data.SensorData.TempInside<App.Data.SysCtrlLine.TempInsideWinterLine)&&(App.Data.SensorData.TempOutside>=(App.Data.SysCtrlLine.TempInsideWinterLine+2)))
						 App.SysCtrlStatus.ThermalMode = HEATMODE_ON;
					else
						App.SysCtrlStatus.ThermalMode =  HEATMODE_OFF;
				}
				else
					App.SysCtrlStatus.ThermalMode =  HEATMODE_OFF;			
				GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
			}
			App.Menu.MainForm.RefreshFlag|=MAINFORM_RHSENSOR;
			break;
		case COMM_CO2_READ:
			if(App.Data.SysCtrlPara.CircleModeSet==CIRCLEMODE_AUTO)
			{
				App.SysFault.FaultFlag &= ~CO2INSIDEBEYOND;
				if(App.Data.SensorData.CO2Inside>App.Data.SysCtrlLine.CO2InsideLine)
				{
					App.SysFault.FaultFlag |= CO2INSIDEBEYOND;
					if(App.SysCtrlStatus.BypassMode != BYPASS_CIRCLEOUT)
					{
						App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
					PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
					}
//					System.Device.Timer.Start(CO2_WARNING_TIMER,TimerMessage,CO2_WARNING_MS,SwitchCircleMode);
				}
				else if(App.Data.SensorData.CO2Inside<(App.Data.SysCtrlLine.CO2InsideLine-150))
				{
					if(App.SysCtrlStatus.BypassMode != BYPASS_CIRCLEIN)
					{
						App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEIN;
					PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
					}
				}
				App.Menu.MainForm.RefreshFlag |= MAINFORM_CICLEMODE;
			}	
			App.Menu.MainForm.RefreshFlag |= MAINFORM_AIRSENSOR;
			break;
		case COMM_IAQ_READ:
			if(App.Data.SysCtrlPara.CircleModeSet==CIRCLEMODE_AUTO)
			{					
				App.SysFault.FaultFlag &= ~PMINSIDEBEYONG;
				if(App.Data.SensorData.PMInside>App.Data.SysCtrlLine.PMInsideLine)
				{
					App.SysFault.FaultFlag |= PMINSIDEBEYONG;
					App.SysCtrlStatus.AirFlowSet ++;
					if(App.SysCtrlStatus.AirFlowSet>AIRFLOW_STEP20)
							App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP20;
					PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
				}
				else if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
				{
					if(App.SysCtrlStatus.AirFlowSet != App.Data.SysCtrlPara.AirFlowSet )
					{
						if(App.SysCtrlStatus.AirFlowSet>App.Data.SysCtrlPara.AirFlowSet )
							App.SysCtrlStatus.AirFlowSet --;
						else
							App.SysCtrlStatus.AirFlowSet = App.Data.SysCtrlPara.AirFlowSet;		
						PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
					}						
				}
				else if(App.SysCtrlStatus.AirFlowSet != App.Data.SysCtrlPara.AirFlowAuto)
				{
					App.SysFault.FaultFlag &= ~PMINSIDEBEYONG;
					if(App.SysCtrlStatus.AirFlowSet>App.Data.SysCtrlPara.AirFlowAuto )
						App.SysCtrlStatus.AirFlowSet --;
					else
						App.SysCtrlStatus.AirFlowSet = App.Data.SysCtrlPara.AirFlowAuto;

					PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
				}
				App.Menu.MainForm.RefreshFlag|=MAINFORM_AIRFLOW;
//				PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			}	
			App.Menu.MainForm.RefreshFlag|=MAINFORM_AIRSENSOR;
			break;
		case COMM_XFMOTODUTY:
#ifdef __SELF_ADJUSTMOTO
		  if(App.SysVersion.PowerMainVersion>1)
			{
				if(App.SysVersion.PowerSubVersion>23)
				{
					if(App.SysFault.MotoXF>20)  //plus 10%
					{
						App.SysFault.FaultFlag |= XFMOTO_FAULT;
						App.SysFault.MotoXF=20;
					}
					else
						App.SysFault.FaultFlag &= ~XFMOTO_FAULT;
				}
				else
				{
					if(App.SysRunStatus.XFmotoRPM<RPM_MOTO1_BOTTOM)
					{
						App.SysFault.MotoXF++;
					}
					else if(App.SysRunStatus.XFmotoRPM>(RPM_MOTO1_TOP))
					{
						App.SysFault.MotoXF++;
					}
					else
						App.SysFault.MotoXF = 0;
					
					if(App.SysFault.MotoXF>20)  //plus 10%
					{
						App.SysFault.FaultFlag |= XFMOTO_FAULT;
						App.SysFault.MotoXF=20;
					}
					else
						App.SysFault.FaultFlag &= ~XFMOTO_FAULT;
				}
			}				
#endif
			break;
		case COMM_PFMOTODUTY:
#ifdef __SELF_ADJUSTMOTO
		  if(App.SysVersion.PowerMainVersion>1)
			{
				if(App.SysVersion.PowerSubVersion>23)
				{
					if(App.SysFault.MotoPF>20)  //plus 10%
					{
						App.SysFault.FaultFlag |= PFMOTO_FAULT;
						App.SysFault.MotoPF=20;
					}
					else
						App.SysFault.FaultFlag &= ~PFMOTO_FAULT;
				}
				else
				{
					if(App.SysRunStatus.PFmotoRPM<RPM_MOTO2_BOTTOM)
					{
						App.SysFault.MotoPF++;
					}
					else if(App.SysRunStatus.PFmotoRPM>(RPM_MOTO2_TOP))
					{
						App.SysFault.MotoPF++;
					}
					else
						App.SysFault.MotoPF = 0;
					
					if(App.SysFault.MotoPF>20)  //plus 10%
					{
						App.SysFault.FaultFlag |= PFMOTO_FAULT;
						App.SysFault.MotoPF=20;
					}
					else
						App.SysFault.FaultFlag &= ~PFMOTO_FAULT;

				}
			}	
#endif
			break;
		case COMM_POWER_SET:
			if(App.SysCtrlStatus.Power == POWER_ON)
			{
				if(App.SysRunStatus.Power ==POWER_ON)
				{
					App.SysFault.PowerBaseFault=0;
					App.SysFault.FaultFlag  &= ~POWERBASE_FAULT;	
				}
				else
				{
//					PostMessage(MessageCommTrans, COMM_POWER_SET);
					App.SysFault.PowerBaseFault++;
					if(App.SysFault.PowerBaseFault>20)
						App.SysFault.FaultFlag  |= POWERBASE_FAULT;
				}
			}
			else //if(App.Data.SysCtrlPara.Power == POWER_OFF)
			{
				App.SysCtrlStatus.Power = POWER_OFF;
				if(App.SysRunStatus.Power ==POWER_OFF)
				{
					App.SysFault.PowerBaseFault=0;
					App.SysFault.FaultFlag  &= ~POWERBASE_FAULT;
				}
				else
				{
//					PostMessage(MessageCommTrans, COMM_POWER_SET);
					App.SysFault.PowerBaseFault++;
					if(App.SysFault.PowerBaseFault>20)
						App.SysFault.FaultFlag  |= POWERBASE_FAULT;
				}
			}
			break;
		default:
			break;
	}

	
}



void Function_Run(void)
{
//	uint16_t temp=0;
	static byte seconds=0;

//#ifdef __DEBUG
//			PostMessage(MessageCommTrans, COMM_XFMOTODUTY);
//			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
//#endif
	seconds++;
	
	
		
	switch(seconds) //10s
	{
		case 1:
		  if(App.SysCtrlStatus.Power != App.SysRunStatus.Power )
			{
				seconds=0;
				PostMessage(MessageCommTrans, COMM_POWER_SET);
			}
			else
			{
				if(App.SysCtrlStatus.BypassMode != App.SysRunStatus.BypassMode )
					PostMessage(MessageCommTrans, COMM_CIRCLEMODE);
//				if(App.SysCtrlStatus.PFmotoPWM != App.SysRunStatus.PFmotoPWM )
//					PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
//				if(App.SysCtrlStatus.XFmotoPWM != App.SysRunStatus.XFmotoPWM )
//					PostMessage(MessageCommTrans, COMM_XFMOTODUTY);
			}
		break;
		case 2:
			PostMessage(MessageCommTrans, COMM_XFMOTODUTY);
		break;
		case 3:
			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
		break;
		case 4:
			PostMessage(MessageCommTrans, COMM_CO2_READ);
		break;
		case 5:
			PostMessage(MessageCommTrans, COMM_IAQ_READ);
		break;
		case 6:
			PostMessage(MessageCommTrans, COMM_TEMPER_RH);
		break;
		case 7:
			PostMessage(MessageCommTrans, COMM_XFMOTODUTY);
		break;
		case 8:
			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
		break;
		case 9:
			PostMessage(MessageCommTrans, COMM_CO2_READ);
		break;
		case 10:
			seconds=0;
			UseTimer10s++;
#ifdef __TESTFILTER			
			PostMessage(MessageProcess, FILTER_ONEHOUR);
#endif
			if(UseTimer10s>360)
			{
				PostMessage(MessageProcess, FILTER_ONEHOUR);
				UseTimer10s=0;

			}
			break;
		default:
			break;
	}


}
		
static void AppSystick10(void)
{
	static byte second;
	
	if(++second==10)
	{
		second =0;
		if(App.SysCtrlStatus.Power==POWER_OFF)
		{
			if(App.SysRunStatus.Power != POWER_OFF)
				PostMessage(MessageCommTrans, COMM_POWER_SET);
			return;
		}
		
		Function_Run();
		if(OperTimer1s)
		{
			OperTimer1s--;
			if(OperTimer1s==(SCREEN_WAITLIGHT_1S-20))
			{
				if(App.Menu.FocusFormPointer != &App.Menu.MainForm)
				{
					InitHmi();
				}
			}
			else 
			{
				if(!OperTimer1s)
				{
					LCD_BL_OFF;
					System.Device.Led.LedModeSet(LED_GREEN,BREATH_ON);
					App.Data.SysCtrlPara.Power = POWER_SLEEP;
//					StorePost(STORE_SYSPARA);
				}
			}
				
		}
	}
	
	if(SeqOperDelay)
	{
		SeqOperDelay--;
		App.Menu.MainForm.RefreshFlag|=MAINFORM_AIRFLOW;
		if(!SeqOperDelay)
		{
			PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			CommTalk_Trans(COMM_BEEPONE);
		}
	}
	
}

static void SysPowerOff(void)
{
	App.Data.SysCtrlPara.Power = POWER_OFF;
	App.SysCtrlStatus.Power = POWER_OFF;	
	App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP1;
	AlarmDeInit();
	OperTimer1s=0;
	LCD_BL_OFF;
//	LogicDelayMs=0;
	System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
	System.Device.Lcd.LCD_Reset();
	System.Device.Led.LedModeSet(LED_RED,TURN_ON);
	App.Menu.FocusFormPointer = &App.Menu.MainForm;
}

static void SysPowerOn(void)
{
	App.SysCtrlStatus.AirFlowSet= App.Data.SysCtrlPara.AirFlowSet;
//	App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
//	App.SysCtrlStatus.ThermalMode = App.Data.SysCtrlPara.ThermalModeSet;
	SetVentiMoto2Act();
	
	App.SysRunStatus.XFmotoPWM = 0;
	App.SysRunStatus.PFmotoPWM = 0;
	App.SysRunStatus.Power = POWER_EMPTY;
	App.SysRunStatus.BypassMode = BYPASS_UNKNOW;
	PostMessage(MessageParaUpdate, PARA_CIRCLEMODE); 
	PostMessage(MessageParaUpdate, PARA_THERMALMODE);
	
//	PostMessage(MessageParaUpdate, PARA_WORKMODE);  // send in InitHmi
	App.Data.SysCtrlPara.Power = POWER_WAKEUP;
	App.SysCtrlStatus.Power = POWER_ON;
	App.Menu.FocusFormPointer = &App.Menu.MainForm;
//	App.Menu.FocusFormPointer->RefreshFlag &=	MAINFORM_NEW;
	System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
	System.Device.Led.LedModeSet(LED_RED,TURN_OFF);
//	LogicDelayMs=10;
	WifiCtrlCode(ModuleQuery);
	OperTimer1s=SCREEN_WAITLIGHT_1S;
	InitHmi();
//	LCD_BL_ON;
}

void CtrlParaUpdate(ParaOperTypedef data)
{
	switch(data)
	{
		case PARA_XFMOTODUTY:
//			if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
//			{
//				App.Data.SysCtrlPara.AirFlowSet = App.SysCtrlStatus.AirFlowSet;
//				StorePost(STORE_SYSPARA);
//				
//			}
//			else
//				App.SysCtrlStatus.AirFlowSet =App.Data.SysCtrlPara.AirFlowAuto;
			GetMotoXFOcupy(App.SysCtrlStatus.AirFlowSet);
			GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
		/* COMM_XFMOTODUTY/COMM_PFMOTODUTY send per5s*/
			PostMessage(MessageCommTrans, COMM_XFMOTODUTY);
//		  PostMessage(MessageParaUpdate, PARA_PFMOTODUTY);
			App.Menu.MainForm.RefreshFlag|=MAINFORM_AIRFLOW;
			StorePost(STORE_SYSPARA);
			break;
		case PARA_PFMOTODUTY:
			GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
		
			break;
		case PARA_CIRCLEMODE:
			switch(App.Data.SysCtrlPara.CircleModeSet)
			{
				case CIRCLEMODE_OUT:
					App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
					App.SysFault.FaultFlag &= ~(DATABEYOND_FLAG);
				 SysIndex.Moto1PwmPace = PWM_DUTYOCCUPY_OUT_MOTO1STEP;
				 SysIndex.Moto1RpmPace = RPM_MOTO1_OUT_STEP;
					PostMessage(MessageCommTrans, COMM_CIRCLEMODE);
				break;
				case CIRCLEMODE_IN:
					App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEIN;
					App.SysFault.FaultFlag &= ~(DATABEYOND_FLAG);
				 SysIndex.Moto1PwmPace = PWM_DUTYOCCUPY_IN_MOTO1STEP;
				 SysIndex.Moto1RpmPace = RPM_MOTO1_IN_STEP;
//					PostMessage(MessageParaUpdate, PARA_PFMOTODUTY);
					PostMessage(MessageCommTrans, COMM_CIRCLEMODE);
				break;
				case CIRCLEMODE_AUTO:
					if(App.SysCtrlStatus.BypassMode == BYPASS_CIRCLEIN)
					{
						 SysIndex.Moto1PwmPace = PWM_DUTYOCCUPY_IN_MOTO1STEP;
						 SysIndex.Moto1RpmPace = RPM_MOTO1_IN_STEP;
					}
					else
					{
						App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
					  SysIndex.Moto1PwmPace = PWM_DUTYOCCUPY_OUT_MOTO1STEP;
					  SysIndex.Moto1RpmPace = RPM_MOTO1_OUT_STEP;
					}
				break;
				default:
					App.SysFault.FaultFlag &= ~(DATABEYOND_FLAG);
					App.Data.SysCtrlPara.CircleModeSet =CIRCLEMODE_OUT;
					App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
				 SysIndex.Moto1PwmPace = PWM_DUTYOCCUPY_OUT_MOTO1STEP;
				 SysIndex.Moto1RpmPace = RPM_MOTO1_OUT_STEP;
					PostMessage(MessageCommTrans, COMM_CIRCLEMODE);
					break;
			}	
			GetMotoXFOcupy(App.SysCtrlStatus.AirFlowSet);
			GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
			App.Menu.MainForm.RefreshFlag |=MAINFORM_CICLEMODE;
			StorePost(STORE_SYSPARA);
			break;
		case PARA_THERMALMODE:
			switch(App.Data.SysCtrlPara.ThermalModeSet)
			{
				case HEATMODE_AUTO:
					break;
				case HEATMODE_ON:
					App.SysCtrlStatus.ThermalMode = HEATMODE_ON;
					break;
				case HEATMODE_OFF:
					App.SysCtrlStatus.ThermalMode = HEATMODE_OFF;
					break;
				default:
					App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_OFF;
					App.SysCtrlStatus.ThermalMode = HEATMODE_OFF;
					break;
			}
			GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
			App.Menu.MainForm.RefreshFlag |=MAINFORM_THERMALMODE;
			StorePost(STORE_SYSPARA);
			break;
		case PARA_VENTILATE:
			SetVentiMoto2Act();
  		GetMotoPFOcupy(App.SysCtrlStatus.AirFlowSet);
			App.Menu.MainForm.RefreshFlag |=MAINFORM_VENTIRATE;
			PostMessage(MessageCommTrans, COMM_PFMOTODUTY);
			StorePost(STORE_SYSPARA);
			break;
		case PARA_MUTEMODE:
			if(App.Data.SysCtrlPara.MuteSet == MUTEMODE_ACT)
			{
				if(App.Menu.FocusFormPointer == &App.Menu.MainForm)
				{
					OperTimer1s=0;
					LCD_BL_OFF;
					System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
					App.Data.SysCtrlPara.Power = POWER_SLEEP;
				}
			}
			else
			{
				App.Data.SysCtrlPara.MuteSet = MUTEMODE_OFF;
				OperTimer1s=SCREEN_WAITLIGHT_1S;
				LCD_BL_ON;
				System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
				App.Data.SysCtrlPara.Power = POWER_ON;
			}
			break;
		case PARA_CHILDLOCK:
			App.Menu.MainForm.RefreshFlag |=MAINFORM_CHILDLOCK;
			break;
		case PARA_WORKMODE:
			if(App.Data.SysCtrlPara.WorkMode == WEEKTIMER_ON)
			{
				PostMessage(MessageAlarm, ALARM_START);
			}
			else
			{
				App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
				App.SysCtrlStatus.AirFlowSet= App.Data.SysCtrlPara.AirFlowSet ;
				PostMessage(MessageAlarm, ALARM_INIT);
			}
			
			App.Menu.MainForm.RefreshFlag |=MAINFORM_RUNMODE;
			PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			StorePost(STORE_SYSPARA);
			break;
		case PARA_POWER_SET:
			if(App.Data.SysCtrlPara.Power == POWER_ON)
				SysPowerOn();
			else
				SysPowerOff();
			PostMessage(MessageCommTrans, COMM_POWER_SET);
			StorePost(STORE_SYSPARA);

			break;
		case PARA_SYSTEM_UPDATE:
			System.Device.Rtc.RTC_SetFlag(2);
		  LCD_BL_OFF;
     NVIC_SystemReset();
			break;
		default:
			break;
	}
}

void AlarmOper(AlarmTypedef AlarmOper)
{
	switch(AlarmOper)
	{
		case ALARM_SOUND:
			AlarmSound();
		  if(App.Data.SysCtrlPara.WorkMode == WEEKTIMER_ON)
			{
				App.SysCtrlStatus.AirFlowSet =App.Data.SysCtrlPara.AirFlowAuto;
				PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			}
			break;
		case ALARM_START:
			if(PASSED ==AlarmStart())
			{
				App.SysCtrlStatus.AirFlowSet =App.Data.SysCtrlPara.AirFlowAuto;
				PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			}
			else
			{
				App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
				App.SysFault.FaultFlag |= STORE_FAULT;
			}
			break;
		case ALARM_INIT:
			AlarmDeInit();
			break;
		default:
			break;
	}
}

void ProcessParse(ProcessTypedef ProcessMessage)
{
	switch(ProcessMessage)
	{
		case FILTER_ONEHOUR:
			App.Data.UsedHours++;
			if(App.Data.UsedHours>99999)
				App.Data.UsedHours = 99999;
			StorePost(STORE_USEDTIMER);
			if(App.SysCtrlStatus.AirFlowSet>0)
			{
				App.Data.FilterTimerHour.FirstFilterTimerHour++;
				if(App.Data.FilterTimerHour.FirstFilterTimerHour > 9999)
					App.Data.FilterTimerHour.FirstFilterTimerHour = 9999;
				
				App.Data.FilterTimerHour.MidFilterTimerHour++;
				if(App.Data.FilterTimerHour.MidFilterTimerHour > 9999)
					App.Data.FilterTimerHour.MidFilterTimerHour = 9999;
				
				App.Data.FilterTimerHour.ESPFilterTimerHour++;
				if(App.Data.FilterTimerHour.ESPFilterTimerHour > 9999)
					App.Data.FilterTimerHour.ESPFilterTimerHour = 9999;
				
				App.Data.FilterTimerHour.HPFilterTimerHour++;
				if(App.Data.FilterTimerHour.HPFilterTimerHour > 9999)
					App.Data.FilterTimerHour.HPFilterTimerHour = 9999;
				
				StorePost(STORE_FILTERTIMER);
				PostMessage(MessageProcess, FILTER_CHECK);
			}
			break;
		case FILTER_CLEAR:
//			if(App.SysFault.FaultFlag&FILTER_CHECKFLAG)
//				System.Device.Led.LedModeSet(LED_RED, TURN_ON);
//			else
//				System.Device.Led.LedModeSet(LED_RED, TURN_OFF);
//			StorePost(STORE_FILTERTIMER);
			break;
		case FILTER_CHECK:
			if(App.Data.FilterTimerHour.FirstFilterTimerHour> App.Data.FilterHourLimit.FirstFilterHourLimit)
			{
//				System.Device.Led.LedModeSet(LED_RED, TURN_ON);
				App.SysFault.FaultFlag |= FIRSTFILTER_CHECK;
			}
			else
				App.SysFault.FaultFlag &= ~FIRSTFILTER_CHECK;
				
			if(App.Data.FilterTimerHour.MidFilterTimerHour> App.Data.FilterHourLimit.MidFilterHourLimit)
			{
//				System.Device.Led.LedModeSet(LED_RED, TURN_ON);
				App.SysFault.FaultFlag |= MIDFILTER_CHECK;
			}
			else
				App.SysFault.FaultFlag &= ~MIDFILTER_CHECK;
			
			if(App.Data.FilterTimerHour.ESPFilterTimerHour> App.Data.FilterHourLimit.ESPFilterHourLimit)
			{
//				System.Device.Led.LedModeSet(LED_RED, TURN_ON);
				App.SysFault.FaultFlag |= ESPFILTER_CHECK;
			}
			else
				App.SysFault.FaultFlag &= ~ESPFILTER_CHECK;
			
			if(App.Data.FilterTimerHour.HPFilterTimerHour> App.Data.FilterHourLimit.HPFilterHourLimit)
			{
//				System.Device.Led.LedModeSet(LED_RED, TURN_ON);
				App.SysFault.FaultFlag |= HPFILTER_CHECK;
			}
			else
				App.SysFault.FaultFlag &= ~HPFILTER_CHECK;
			break;
		case FAULTINFO_DISP:
			OperTimer1s = 0;
			break;
		
	}
}

static void InitLogic(void)
{
  System.Device.Systick.Register(Systick10, AppSystick10);
	AppWifiInit();
}

void LogicTask(void)
{
    uint message;
    uint data; 
    InitLogic();

//    App.Menu.FocusFormPointer = &App.Menu.LogoForm;     //页面焦点

//    System.OS.DelayMs(2000);
//    
  App.Menu.FocusFormPointer = &App.Menu.MainForm;
  System.Device.Iwdog.IwdogReload();
  System.OS.DelayMs(50);
//	SysPowerOn();
	
	PostMessage(MessageCommTrans, COMM_VERSION);
	PostMessage(MessageParaUpdate, PARA_POWER_SET);	
    
    //逻辑业务任务获取消息，分配处理
    while(true)
    {     
        message = System.OS.PendMessageQueue();
        
        data = GetMessageData(message);
        switch(GetMessageType(message))
        {
            case MessageKey:                    //按键消息
                App.Menu.FocusFormPointer->KeyProcess((KeyActEnum)data);
							OperTimer1s=SCREEN_WAITLIGHT_1S;
                break;
            case MessageCommRecv:
                if(true==CommTalk_Echo(data))
								{
									App.SysFault.PowerBaseFault=0;
									App.SysFault.FaultFlag &= ~POWERBASE_FAULT;
									if(!(App.SysFault.FaultFlag))
										App.SysFault.FaultFlag &= ~FAULTICON_DISP;
								}
                break;
            case MessageCommTrans:
                CommTalk_Trans(data);
						    App.SysFault.PowerBaseFault++;
						if(App.SysFault.PowerBaseFault>100)
						{
							App.SysFault.FaultFlag |= POWERBASE_FAULT;
							App.SysFault.FaultFlag &= ~FAULTICON_DISP;
						}
                break;
            case MessageIR:
							OperTimer1s=SCREEN_WAITLIGHT_1S;
                ParseRemoteIR(data);
                break;
            case MessageEchoData:
                ParseEchoData(data);
                break;
            case MessageAlarm:
							if(App.Data.SysCtrlPara.Power != POWER_OFF)
								AlarmOper((AlarmTypedef)data);
                break;
						case MessageProcess:
							ProcessParse((ProcessTypedef)data);
							break;
            case MessageParaUpdate:
                CtrlParaUpdate((ParaOperTypedef)data);
								App.WifiState |= WIFI_UPLOAD_SYSPARA;
                break;
            case MessageWifiRecv:
                WifiRecvParse(data);
                break;
            case MessageWifiCtrl:
                WifiCtrlCode(data);
                break;
            default:
							if(App.Data.SysCtrlPara.Power>POWER_ON)
							{
								StoreDelay++;
								if(App.Data.SysCtrlPara.Power== POWER_WAKEUP)
								{
									App.Data.SysCtrlPara.Power = POWER_ON;
#ifndef __DEBUG
									StorePara();
#endif
									StoreDelay=0;
									LCD_BL_ON;
								}
									
								if(StoreDelay>50)
								{
									StoreDelay=0;
#ifndef __DEBUG
									StorePara();
#endif
								}
							}
							else
								StoreDelay=0;
                break;
        }
				
//				if(App.Data.SysCtrlPara.Power == POWER_OFF)
//					System.Device.Iwdog.IwdogReload();
//				System.OS.DelayMs(LogicDelayMs);
				
				System.OS.DelayMs(20);
    }
}

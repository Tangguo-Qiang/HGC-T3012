#include "app.h"
#include "remoteIR.h"



void ParseRemoteIR(byte order)
{
	if(App.Data.SysCtrlPara.Power == POWER_SLEEP)
	{
		System.Device.Led.LedModeSet(LED_GREEN,TURN_OFF);
		App.Data.SysCtrlPara.Power = POWER_ON;
		App.Data.SysCtrlPara.MuteSet = MUTEMODE_OFF;
		LCD_BL_ON;
		CommTalk_Trans(COMM_BEEPONE);
		return;
	}
	if((App.Data.SysCtrlPara.Power == POWER_OFF)&&(order!=IR_REMOTE_POWER))
		return;
  if((App.SysCtrlStatus.ChildLock==CHILD_LOCK)&&(order!=IR_REMOTE_LOCK)&&(order!=IR_REMOTE_MUTE))
	{
		System.Device.Led.LedModeSet(LED_GREEN,TEMP_TWINKLE);
		return;
	}
  
	App.WifiState |= WIFI_UPLOAD_SYSPARA;
	switch(order)
	{
		case IR_REMOTE_AIRFLOW_PLUS:
			if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
			{
        App.SysCtrlStatus.AirFlowSet++;
				if(App.SysCtrlStatus.AirFlowSet>AIRFLOW_STEP20)
					App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP20;
				App.Data.SysCtrlPara.AirFlowSet=App.SysCtrlStatus.AirFlowSet;

						SeqOperDelay =SEQUENCEOPER_DELAY_100MS;
//				PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			}
			else
			{
				System.Device.Led.LedModeSet(LED_RED,TEMP_TWINKLE);
				CommTalk_Trans(COMM_BEEPONE);
			}
			break;
		case IR_REMOTE_AIRFLOW_MINUS:
			if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND) 
			{						
        App.SysCtrlStatus.AirFlowSet--;
	  		if(App.SysCtrlStatus.AirFlowSet<AIRFLOW_STEP1)
					App.SysCtrlStatus.AirFlowSet = AIRFLOW_STEP1;
				App.Data.SysCtrlPara.AirFlowSet=App.SysCtrlStatus.AirFlowSet;

						SeqOperDelay =SEQUENCEOPER_DELAY_100MS;
//				PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
			}
			else
			{
				System.Device.Led.LedModeSet(LED_RED,TEMP_TWINKLE);
				CommTalk_Trans(COMM_BEEPONE);
			}
			break;
		case IR_REMOTE_LOCK:
			if(App.SysCtrlStatus.ChildLock == CHILD_UNLOCK)
				App.SysCtrlStatus.ChildLock = CHILD_LOCK;
			else
				App.SysCtrlStatus.ChildLock = CHILD_UNLOCK;
			PostMessage(MessageParaUpdate, PARA_CHILDLOCK);
			CommTalk_Trans(COMM_BEEPONE);
			break;
		case IR_REMOTE_VENTIRATE_PLUS:
			App.Data.SysCtrlPara.VentilateRate++;
			if(App.Data.SysCtrlPara.VentilateRate>RATE10TO12)
				App.Data.SysCtrlPara.VentilateRate = RATE10TO12;
			PostMessage(MessageParaUpdate, PARA_VENTILATE);
			CommTalk_Trans(COMM_BEEPONE);
			
			break;
		case IR_REMOTE_VENTIRATE_MINUS:
			App.Data.SysCtrlPara.VentilateRate--;
			if(App.Data.SysCtrlPara.VentilateRate<RATE10TO06)
				App.Data.SysCtrlPara.VentilateRate = RATE10TO06;
			PostMessage(MessageParaUpdate, PARA_VENTILATE);
			CommTalk_Trans(COMM_BEEPONE);
			
			break;
		case IR_REMOTE_POWER:
			if(App.Data.SysCtrlPara.Power == POWER_OFF)
				App.Data.SysCtrlPara.Power = POWER_ON;
			else
				App.Data.SysCtrlPara.Power = POWER_OFF;
			PostMessage(MessageParaUpdate, PARA_POWER_SET);
			CommTalk_Trans(COMM_BEEPLONG);
			
			break;
		case IR_REMOTE_MUTE:
			if(App.Data.SysCtrlPara.MuteSet == MUTEMODE_OFF)
				App.Data.SysCtrlPara.MuteSet = MUTEMODE_ACT;
			else
				App.Data.SysCtrlPara.MuteSet = MUTEMODE_OFF;
			PostMessage(MessageParaUpdate, PARA_MUTEMODE);
			CommTalk_Trans(COMM_BEEPONE);
			
			break;
		case IR_REMOTE_CIRCLEOUT:
			App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_OUT;
			PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
			CommTalk_Trans(COMM_BEEPONE);
			break;
		case IR_REMOTE_CIRCLEIN:
			App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_IN;
			PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
			
			CommTalk_Trans(COMM_BEEPONE);
			break;
		case IR_REMOTE_CIRCLEAUTO:
			App.Data.SysCtrlPara.CircleModeSet = CIRCLEMODE_AUTO;
			PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
			
			CommTalk_Trans(COMM_BEEPONE);
			break;
		case IR_REMOTE_THERMALMODE:
			App.Data.SysCtrlPara.ThermalModeSet++;
			if(App.Data.SysCtrlPara.ThermalModeSet>HEATMODE_OFF)
				App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_AUTO;						
			PostMessage(MessageParaUpdate, PARA_THERMALMODE);
			CommTalk_Trans(COMM_BEEPONE);
			break;
		case IR_REMOTE_TIMERMODE:
			if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
			{
				App.Data.SysCtrlPara.WorkMode = WEEKTIMER_ON;
			}
			else
			{
				App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
			}
			PostMessage(MessageParaUpdate, PARA_WORKMODE);
			
			CommTalk_Trans(COMM_BEEPONE);
			break;
		default:
			break;
	}
}

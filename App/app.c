/*
********************************************************************************
*文件名     : app.c
*作用       : 应用程序入口
*原理       : 应用层入口，也是系统入口，主要初始化各类驱动、数据
*           : 建立LogicTask和MenuTask,处理LogicTask内的消息          
********************************************************************************

********************************************************************************
*/


#include "system.h"
#include "app.h"

AppStruct App;

/*******************************************************************************
* 描述	    : 初始化数据库
*******************************************************************************/
static void InitData(void)
{
  App.Data.SensorData.CO2Inside=600;
	App.Data.SensorData.PMInside = 50;
	App.Data.SensorData.RHInside = 50;
	App.Data.SensorData.TempInside = 25;
	App.Data.SensorData.TempOutside = 31;
	
	App.SysFault.MotoXF=0;
	App.SysFault.MotoPF = 0;
	App.SysFault.CO2Sensor =0;
	App.SysFault.PMSensor =0;
	App.SysFault.RHSensor =0;
	App.SysFault.TempSensors =0;
	App.SysFault.FaultFlag =0x00;
	
	App.Data.SysCtrlPara.AirFlowAuto =AIRFLOW_STEP1;
	App.Data.SysCtrlPara.AirFlowSet =AIRFLOW_STEP1;
	App.Data.SysCtrlPara.CircleModeSet= CIRCLEMODE_OUT;
	App.Data.SysCtrlPara.ThermalModeSet = HEATMODE_OFF;
	App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
	App.Data.SysCtrlPara.VentilateRate = RATE10TO08;
	App.Data.SysCtrlPara.Power = POWER_ON;
	
	App.Data.FilterTimerHour.FirstFilterTimerHour =0;
	App.Data.FilterTimerHour.MidFilterTimerHour =0;
	App.Data.FilterTimerHour.ESPFilterTimerHour =0;
	App.Data.FilterTimerHour.HPFilterTimerHour  =0;
	
	App.Data.FilterHourLimit.FirstFilterHourLimit = 2000;
	App.Data.FilterHourLimit.MidFilterHourLimit = 2000;
	App.Data.FilterHourLimit.ESPFilterHourLimit = 1000;
	App.Data.FilterHourLimit.HPFilterHourLimit = 3000;
	
	App.Data.SysCtrlLine.CO2InsideLine = 1000;
	App.Data.SysCtrlLine.PMInsideLine = 150;
	App.Data.SysCtrlLine.TempInsideSummerLine = 25;
	App.Data.SysCtrlLine.TempInsideWinterLine = 18;
	
	App.Data.DayTimerMode.WorkDayTimer.Timer0.StartTime.Hour =  0;
	App.Data.DayTimerMode.WorkDayTimer.Timer0.StartTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer0.StopTime.Hour = 8;
	App.Data.DayTimerMode.WorkDayTimer.Timer0.StopTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer0.AirFlowSet = AIRFLOW_STEP10;
	App.Data.DayTimerMode.WorkDayTimer.Timer1.StartTime.Hour = 8;
	App.Data.DayTimerMode.WorkDayTimer.Timer1.StartTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer1.StopTime.Hour = 12;
	App.Data.DayTimerMode.WorkDayTimer.Timer1.StopTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer1.AirFlowSet = AIRFLOW_STEP5;
	App.Data.DayTimerMode.WorkDayTimer.Timer2.StartTime.Hour = 12;
	App.Data.DayTimerMode.WorkDayTimer.Timer2.StartTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer2.StopTime.Hour = 14;
	App.Data.DayTimerMode.WorkDayTimer.Timer2.StopTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer2.AirFlowSet = AIRFLOW_STEP16;
	App.Data.DayTimerMode.WorkDayTimer.Timer3.StartTime.Hour = 14;
	App.Data.DayTimerMode.WorkDayTimer.Timer3.StartTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer3.StopTime.Hour = 18;
	App.Data.DayTimerMode.WorkDayTimer.Timer3.StopTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer3.AirFlowSet = AIRFLOW_STEP5;
	App.Data.DayTimerMode.WorkDayTimer.Timer4.StartTime.Hour = 18;
	App.Data.DayTimerMode.WorkDayTimer.Timer4.StartTime.Minute = 0;
	App.Data.DayTimerMode.WorkDayTimer.Timer4.StopTime.Hour = 23;
	App.Data.DayTimerMode.WorkDayTimer.Timer4.StopTime.Minute = 59;
	App.Data.DayTimerMode.WorkDayTimer.Timer4.AirFlowSet = AIRFLOW_STEP16;
	
	App.Data.DayTimerMode.WeekendTimer.Timer0.StartTime.Hour = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer0.StartTime.Minute = 00;
	App.Data.DayTimerMode.WeekendTimer.Timer0.StopTime.Hour = 8;
	App.Data.DayTimerMode.WeekendTimer.Timer0.StopTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer0.AirFlowSet = AIRFLOW_STEP10;
	App.Data.DayTimerMode.WeekendTimer.Timer1.StartTime.Hour = 8;
	App.Data.DayTimerMode.WeekendTimer.Timer1.StartTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer1.StopTime.Hour = 12;
	App.Data.DayTimerMode.WeekendTimer.Timer1.StopTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer1.AirFlowSet = AIRFLOW_STEP16;
	App.Data.DayTimerMode.WeekendTimer.Timer2.StartTime.Hour = 12;
	App.Data.DayTimerMode.WeekendTimer.Timer2.StartTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer2.StopTime.Hour = 14;
	App.Data.DayTimerMode.WeekendTimer.Timer2.StopTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer2.AirFlowSet = AIRFLOW_STEP16;
	App.Data.DayTimerMode.WeekendTimer.Timer3.StartTime.Hour = 14;
	App.Data.DayTimerMode.WeekendTimer.Timer3.StartTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer3.StopTime.Hour = 18;
	App.Data.DayTimerMode.WeekendTimer.Timer3.StopTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer3.AirFlowSet = AIRFLOW_STEP16;
	App.Data.DayTimerMode.WeekendTimer.Timer4.StartTime.Hour = 18;
	App.Data.DayTimerMode.WeekendTimer.Timer4.StartTime.Minute = 0;
	App.Data.DayTimerMode.WeekendTimer.Timer4.StopTime.Hour = 23;
	App.Data.DayTimerMode.WeekendTimer.Timer4.StopTime.Minute = 59;
	App.Data.DayTimerMode.WeekendTimer.Timer4.AirFlowSet = AIRFLOW_STEP16;
	
	App.Data.DayTimerMode.HolidayTimer.Timer0.StartTime.Hour = 0;
	App.Data.DayTimerMode.HolidayTimer.Timer0.StartTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer0.StopTime.Hour = 8;
	App.Data.DayTimerMode.HolidayTimer.Timer0.StopTime.Minute = 0;
	App.Data.DayTimerMode.HolidayTimer.Timer0.AirFlowSet = AIRFLOW_STEP3;
	App.Data.DayTimerMode.HolidayTimer.Timer1.StartTime.Hour = 8;
	App.Data.DayTimerMode.HolidayTimer.Timer1.StartTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer1.StopTime.Hour = 12;
	App.Data.DayTimerMode.HolidayTimer.Timer1.StopTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer1.AirFlowSet = AIRFLOW_STEP3;
	App.Data.DayTimerMode.HolidayTimer.Timer2.StartTime.Hour = 12;
	App.Data.DayTimerMode.HolidayTimer.Timer2.StartTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer2.StopTime.Hour = 14;
	App.Data.DayTimerMode.HolidayTimer.Timer2.StopTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer2.AirFlowSet = AIRFLOW_STEP3;
	App.Data.DayTimerMode.HolidayTimer.Timer3.StartTime.Hour = 14;
	App.Data.DayTimerMode.HolidayTimer.Timer3.StartTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer3.StopTime.Hour = 18;
	App.Data.DayTimerMode.HolidayTimer.Timer3.StopTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer3.AirFlowSet = AIRFLOW_STEP3;
	App.Data.DayTimerMode.HolidayTimer.Timer4.StartTime.Hour = 18;
	App.Data.DayTimerMode.HolidayTimer.Timer4.StartTime.Minute = 00;
	App.Data.DayTimerMode.HolidayTimer.Timer4.StopTime.Hour = 23;
	App.Data.DayTimerMode.HolidayTimer.Timer4.StopTime.Minute = 59;
	App.Data.DayTimerMode.HolidayTimer.Timer4.AirFlowSet = AIRFLOW_STEP3;
	
	App.Data.DayTimerMode.OfficeTimer.Timer0.StartTime.Hour = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer0.StartTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer0.StopTime.Hour = 8;
	App.Data.DayTimerMode.OfficeTimer.Timer0.StopTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer0.AirFlowSet = AIRFLOW_STEP5;
	App.Data.DayTimerMode.OfficeTimer.Timer1.StartTime.Hour = 8;
	App.Data.DayTimerMode.OfficeTimer.Timer1.StartTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer1.StopTime.Hour = 12;
	App.Data.DayTimerMode.OfficeTimer.Timer1.StopTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer1.AirFlowSet = AIRFLOW_STEP20;
	App.Data.DayTimerMode.OfficeTimer.Timer2.StartTime.Hour = 12;
	App.Data.DayTimerMode.OfficeTimer.Timer2.StartTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer2.StopTime.Hour = 14;
	App.Data.DayTimerMode.OfficeTimer.Timer2.StopTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer2.AirFlowSet = AIRFLOW_STEP20;
	App.Data.DayTimerMode.OfficeTimer.Timer3.StartTime.Hour = 14;
	App.Data.DayTimerMode.OfficeTimer.Timer3.StartTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer3.StopTime.Hour = 18;
	App.Data.DayTimerMode.OfficeTimer.Timer3.StopTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer3.AirFlowSet = AIRFLOW_STEP20;
	App.Data.DayTimerMode.OfficeTimer.Timer4.StartTime.Hour = 18;
	App.Data.DayTimerMode.OfficeTimer.Timer4.StartTime.Minute = 0;
	App.Data.DayTimerMode.OfficeTimer.Timer4.StopTime.Hour = 23;
	App.Data.DayTimerMode.OfficeTimer.Timer4.StopTime.Minute = 59;
	App.Data.DayTimerMode.OfficeTimer.Timer4.AirFlowSet = AIRFLOW_STEP5;
	
	App.Data.DayTimerMode.EnergyTimer.Timer0.StartTime.Hour = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer0.StartTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer0.StopTime.Hour = 8;
	App.Data.DayTimerMode.EnergyTimer.Timer0.StopTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer0.AirFlowSet = AIRFLOW_STEP7;
	App.Data.DayTimerMode.EnergyTimer.Timer1.StartTime.Hour = 8;
	App.Data.DayTimerMode.EnergyTimer.Timer1.StartTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer1.StopTime.Hour = 12;
	App.Data.DayTimerMode.EnergyTimer.Timer1.StopTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer1.AirFlowSet = AIRFLOW_STEP13;
	App.Data.DayTimerMode.EnergyTimer.Timer2.StartTime.Hour = 12;
	App.Data.DayTimerMode.EnergyTimer.Timer2.StartTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer2.StopTime.Hour = 14;
	App.Data.DayTimerMode.EnergyTimer.Timer2.StopTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer2.AirFlowSet = AIRFLOW_STEP7;
	App.Data.DayTimerMode.EnergyTimer.Timer3.StartTime.Hour = 14;
	App.Data.DayTimerMode.EnergyTimer.Timer3.StartTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer3.StopTime.Hour = 18;
	App.Data.DayTimerMode.EnergyTimer.Timer3.StopTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer3.AirFlowSet = AIRFLOW_STEP13;
	App.Data.DayTimerMode.EnergyTimer.Timer4.StartTime.Hour = 18;
	App.Data.DayTimerMode.EnergyTimer.Timer4.StartTime.Minute = 0;
	App.Data.DayTimerMode.EnergyTimer.Timer4.StopTime.Hour = 23;
	App.Data.DayTimerMode.EnergyTimer.Timer4.StopTime.Minute = 59;
	App.Data.DayTimerMode.EnergyTimer.Timer4.AirFlowSet = AIRFLOW_STEP10;
	
	App.Data.WeekTimerMode.MondayTimer = WORKDAYTYPE;
	App.Data.WeekTimerMode.TuesdayTimer = WORKDAYTYPE;
	App.Data.WeekTimerMode.WednsdayTimer = WORKDAYTYPE;
	App.Data.WeekTimerMode.ThurdayTimer = WORKDAYTYPE;
	App.Data.WeekTimerMode.FridayTimer = WORKDAYTYPE;
	App.Data.WeekTimerMode.SaturdayTimer =WEEKENDTYPE;
	App.Data.WeekTimerMode.SundayTimer = WEEKENDTYPE;
	
	App.SysTime.Year =2017;
	App.SysTime.Mon =4;
	App.SysTime.Mday =7;
	App.SysTime.Wday =5;
	App.SysTime.Hour =10;
	App.SysTime.Min=15;
	App.SysTime.Sec =0;
	
	App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
	App.SysCtrlStatus.ChildLock = CHILD_UNLOCK;
	
	App.SysCtrlStatus.AirFlowSet= App.Data.SysCtrlPara.AirFlowSet;
	App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;
	App.SysCtrlStatus.ThermalMode = App.Data.SysCtrlPara.ThermalModeSet;
	App.SysCtrlStatus.XFmotoRPM = 0;
	App.SysCtrlStatus.PFmotoRPM = 0;
	
	App.SysRunStatus.BypassMode = BYPASS_UNKNOW;
	App.SysRunStatus.XFmotoPWM = 0;
	App.SysRunStatus.PFmotoPWM = 0;
	App.SysRunStatus.Power = POWER_EMPTY;

  App.Menu.FocusFormPointer = (Form *)&App.Menu.MainForm;
	App.Menu.MainForm.RefreshFlag =MAINFORM_INIT;
	App.WifiState =0;
	App.RFState = 0;
	
	App.SysVersion.CtrlMainVersion=MAIN_VERSION;
	App.SysVersion.CtrlSubVersion=SUB_VERSION;
	App.SysVersion.PowerMainVersion=1;
	App.SysVersion.PowerSubVersion=10;
	
	App.KeyBtn.Focus = FOCUS_ONFORM;
	App.KeyBtn.ParaTitle = FORM_FOCUS;
	App.KeyBtn.ParaSubTitle = FORM_FOCUS;
	
	App.AlarmSys.AirflowSet = AIRFLOW_STOP;
	App.AlarmSys.Alarmgoal = NOFLOW;
	App.AlarmSys.Nextnum=0;
	App.KeyBtn.Position =FORM_FOCUS;
	App.KeyBtn.val = 0;
	
	System.Device.Rtc.Register(&App.SysTime);
	InitPara();
	
}


/*******************************************************************************
* 描述	    : C语言入口Main函数
*******************************************************************************/
int main(void) 
{      
    System.Init();                          //初始化系统层
    InitData();
	

    System.OS.CreateLogicTask(LogicTask);   //创建业务逻辑任务
    System.OS.CreateMenuTask(MenuTask);     //创建菜单界面任务
    
    System.OS.Start();
}






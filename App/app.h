/*

********************************************************************************
*文件名     : app.h
********************************************************************************

********************************************************************************
*/


#ifndef __APP_H
#define __APP_H

#include "system.h"

#include "logicctrl.h"

#include "logic.h"

#include "hmi.h"
#include "remoteIR.h"
#include "remoteApp.h"

#include "menu.h"

#include "comm.h"
#include "StorePara.h"

typedef enum{
	POWER_EMPTY =(byte)0x00,
	POWER_ON =(byte)0x01,
	POWER_OFF= (byte)0x02,
	POWER_SLEEP = (byte)0x03,
	POWER_WAKEUP = (byte)0x04
}PowerSetTypedef;

typedef enum{
	EMPTYTYPE = (byte)0x00,
	WORKDAYTYPE =(byte)0x01,
	WEEKENDTYPE= (byte)0x02,
	HOLIDAYTYPE = (byte)0x03,
	OFFICETYPE = (byte)0x04,
	ENERGYTYPE = (byte)0x05
}TimerTypesTypedef;

typedef enum{
	SENSOR_ON =(byte)0x01,
	SENSOR_READ = (byte)0x02,
}SensorActTypedef;


typedef enum{
	RATEEMPTY  = (byte)0x00,
	RATE10TO06 = (byte)0x01,
	RATE10TO08 = (byte)0x02,
	RATE10TO10 = (byte)0x03,
	RATE10TO12 = (byte)0x04
}VentilateRateTypedef;

typedef enum{
	CIRCLEMODE_EMPTY = (byte)0x00,
	CIRCLEMODE_AUTO = (byte)0x01,
	CIRCLEMODE_OUT = (byte)0x02,
	CIRCLEMODE_IN = (byte)0x03
}CircleModeSetTypedef;

#ifdef __BYPASS_INVERSE
	typedef enum{
		BYPASS_UNKNOW = (byte)CIRCLEMODE_EMPTY,
		BYPASS_CIRCLEOUT = (byte)0x02,//(CIRCLEMODE_IN-1),
		BYPASS_CIRCLEIN = (byte)0x01//(CIRCLEMODE_OUT-1)
	}BypassModeTypedef;
#else
	typedef enum{
		BYPASS_UNKNOW = (byte)CIRCLEMODE_EMPTY,
		BYPASS_CIRCLEOUT = (byte)0x01,//(CIRCLEMODE_OUT-1),
		BYPASS_CIRCLEIN = (byte)0x02//(CIRCLEMODE_IN-1)
	}BypassModeTypedef;
#endif

typedef enum{
	HEATMODE_EMPTY = (byte)0x00,
	HEATMODE_AUTO = (byte)0x01,
	HEATMODE_ON = (byte)0x02,
	HEATMODE_OFF = (byte)0x03
}HeatModeTypedef;

typedef enum{
	TEMPTIMER_ON = 0x00,
	WEEKTIMER_ON = 0x01,
	ALLBYHAND = 0x02
}RunModeTypedef;



typedef struct {
	byte Hour;
	byte Minute;
}TimeTypedef;

typedef struct {
	TimeTypedef StartTime;
	TimeTypedef StopTime;
	AirFlowSetEnum AirFlowSet;
}SysTimerTypedef;

typedef struct {
	SysTimerTypedef Timer0;
	SysTimerTypedef Timer1;
	SysTimerTypedef Timer2;
	SysTimerTypedef Timer3;
	SysTimerTypedef Timer4;
}DayTimersTypedef;
	


typedef enum{
	CHILD_UNLOCK = (byte)0x00,
	CHILD_LOCK = (byte)0x01
}ChildLockTypedef;


typedef enum{
	MUTEMODE_OFF = (byte)0x00,
	MUTEMODE_ACT = (byte)0x01,
}MutSetTypedef;

typedef enum{
	FIRSTFILTER_CHECK = (uint)0x01,
	MIDFILTER_CHECK = (uint)0x02,
	ESPFILTER_CHECK = (uint)0x04,
	HPFILTER_CHECK = (uint)0x08,
	POWERBASE_FAULT = (uint)0x10,
	SENSORTEMP_FAULT = (uint)0x20,
	SENSORRH_FAULT = (uint)0x40,
	SENSORCO2_FAULT = (uint)0x80,
	SENSORPM_FAULT = (uint)0x100,
	XFMOTO_FAULT = (uint)0x200,
	PFMOTO_FAULT = (uint)0x400,
	CO2INSIDEBEYOND=(uint)0x800,
	PMINSIDEBEYONG=(uint)0x1000,
	STORE_FAULT=(uint)0x2000,
	
	
	FAULTICON_DISP = (uint)0x10000
}FaultTypedef;

#define FILTER_CHECKFLAG 	(FIRSTFILTER_CHECK|MIDFILTER_CHECK|ESPFILTER_CHECK|HPFILTER_CHECK)
#define COREFAULT_FLAG 		(POWERBASE_FAULT|XFMOTO_FAULT|PFMOTO_FAULT)
#define NORMALFAULT_FLAG 	(SENSORCO2_FAULT|SENSORPM_FAULT|SENSORRH_FAULT|SENSORTEMP_FAULT)
#define SYSFAULT_FLAG 		(COREFAULT_FLAG|NORMALFAULT_FLAG)
#define DATABEYOND_FLAG 		(CO2INSIDEBEYOND|PMINSIDEBEYONG)


/*******************************************************************************
* 描述	    : msOS中最重要的一个结构体，封装了App层的所有数据及窗口控件
*******************************************************************************/
typedef struct
{
	struct Data
	{
    struct SensorData
    {
        signed char TempInside;                 
        byte RHInside;
        signed char TempOutside;
        ushort CO2Inside;
        ushort PMInside;
		}SensorData;

		struct SysCtrlPara
		{			
			PowerSetTypedef Power;
			RunModeTypedef WorkMode;
			MutSetTypedef MuteSet;
			CircleModeSetTypedef CircleModeSet;
			HeatModeTypedef ThermalModeSet;
			VentilateRateTypedef VentilateRate;
			AirFlowSetEnum AirFlowAuto; 
			AirFlowSetEnum AirFlowSet; 
		}SysCtrlPara; 
			
		struct
		{
			ushort FirstFilterTimerHour;
			ushort MidFilterTimerHour;
			ushort ESPFilterTimerHour;
			ushort HPFilterTimerHour;
		}FilterTimerHour;
		
		struct
		{
			ushort FirstFilterHourLimit;
			ushort MidFilterHourLimit;
			ushort ESPFilterHourLimit;
			ushort HPFilterHourLimit;
		}FilterHourLimit;

		struct
		{
			char TempInsideSummerLine;
			char TempInsideWinterLine;
			ushort PMInsideLine;
			ushort CO2InsideLine;
		}SysCtrlLine;

		struct
		{
			TimerTypesTypedef MondayTimer;
			TimerTypesTypedef TuesdayTimer;
			TimerTypesTypedef WednsdayTimer;
			TimerTypesTypedef ThurdayTimer;
			TimerTypesTypedef FridayTimer;
			TimerTypesTypedef SaturdayTimer;
			TimerTypesTypedef SundayTimer;
		}WeekTimerMode;
		
		struct
		{
			DayTimersTypedef WorkDayTimer;
			DayTimersTypedef WeekendTimer;
			DayTimersTypedef HolidayTimer;
			DayTimersTypedef OfficeTimer;
			DayTimersTypedef EnergyTimer;
		}DayTimerMode;

		uint UsedHours;
    }Data;

		RTC_TimeTypeDef SysTime;
		AlarmSysTypeDef AlarmSys;
		
		struct 
		{			
			PowerSetTypedef Power;
			ChildLockTypedef ChildLock;
			BypassModeTypedef BypassMode;
//			VentilateRateTypedef VentilateRate;
			HeatModeTypedef ThermalMode;
			ushort XFmotoPWM; //persent of full speed
			ushort PFmotoPWM; //persent of full speed
			ushort XFmotoRPM; 
			ushort PFmotoRPM; 
			AirFlowSetEnum AirFlowSet; 
		}SysCtrlStatus; 
	
		struct 
		{			
			BypassModeTypedef BypassMode;
			ushort XFmotoPWM; 
			ushort PFmotoPWM; 
			ushort XFmotoRPM; 
			ushort PFmotoRPM; 
//			AirFlowSetEnum AirFlowSet; //persent of full speed
			PowerSetTypedef Power;
		}SysRunStatus; 

		
    struct SysFault
    {
        byte PowerBaseFault;                 
        byte TempSensors;                 
        byte RHSensor;
        byte CO2Sensor;
        byte PMSensor;
			  byte MotoXF;
			  byte MotoPF;
			  uint FaultFlag;
		}SysFault;
		
		struct SysVersion
		{
			byte CtrlMainVersion;
			byte CtrlSubVersion;
			byte PowerMainVersion;
			byte PowerSubVersion;
		}SysVersion;

    struct Menu
    {
        Form MainForm;
        Form SysParaForm;
        Form WeekTimerForm;
        Form FilterForm;
        Form ModeCtrlForm;
        Form FaultInfoForm;
        Form * FocusFormPointer;
    }Menu;
		
		BTN_ACTTypeDef KeyBtn;
		uint WifiState;
		uint StoreState;
		uint RFState;

}AppStruct;

extern AppStruct App;


#endif /*__APP_H*/







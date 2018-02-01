#include "remoteApp.h"

//*************************************************************************
//
//ModuleStatus 指针中各个有效位具体码值
//
//*************************************************************************

//Hekr模块状态码
typedef	enum
{
	STA_Mode = 0x01,
	HekrConfig_Mode = 0x02,
	AP_Mode = 0x03,
	STA_AP_Mode = 0x04,
	RF_OFF_Mode = 0x05
}HekrModuleWorkCode;

//Hekr WIFI状态码
typedef	enum
{
	RouterConnected = 0x01,
	RouterConnectedFail = 0x02,
	RouterConnecting = 0x03,
	PasswordErr = 0x04,
	NoRouter = 0x05,
	RouterTimeOver = 0x06
}HekrModuleWIFICode;

//Hekr Cloud状态码
typedef	enum
{
	CloudConnected = 0x01,
	DNS_Fail = 0x02,
	CloudTimeOver = 0x03
}HekrModuleCloudCode;


static byte RxValidDataBuffer[WIFIUSER_MAX_LEN]={0};
static byte TxValidDataBuffer[WIFIUSER_MAX_LEN]={0};
static byte ModuleStatus[8]={0};
static WifiTransDataTypeDef WifiTrans;
static uint SysFaultBuffer=0;




static void AppSysParaUpload(void)
{
	byte index=0;
//	byte* pdes=TxValidDataBuffer;
	
	TxValidDataBuffer[index++] =SysParaUpload; //4  0
	TxValidDataBuffer[index++] =MACHINE_CAPACITY;   //5  1
	if(App.Data.SysCtrlPara.Power != POWER_OFF)
		TxValidDataBuffer[index++] = POWER_ON;   	//6  2
	else
		TxValidDataBuffer[index++] = POWER_OFF;   	//6  2
	
	TxValidDataBuffer[index++] = App.Data.SysCtrlPara.WorkMode;   //7  3
	TxValidDataBuffer[index++] = App.Data.SysCtrlPara.MuteSet;   //8  4
	TxValidDataBuffer[index++] = App.Data.SysCtrlPara.CircleModeSet;   //9  5
	TxValidDataBuffer[index++] = App.Data.SysCtrlPara.ThermalModeSet;   //10  6
	TxValidDataBuffer[index++] = App.Data.SysCtrlPara.VentilateRate;   //11  7
	TxValidDataBuffer[index++] = App.SysCtrlStatus.AirFlowSet;   //12  8
	TxValidDataBuffer[index++] = App.SysCtrlStatus.ChildLock;   //13  9
	TxValidDataBuffer[index++] = App.SysCtrlStatus.BypassMode;   //14  10
	TxValidDataBuffer[index++] = App.SysCtrlStatus.ThermalMode;   //15  11
	TxValidDataBuffer[index++] = App.Data.SensorData.TempInside+40;   //16  12
	TxValidDataBuffer[index++] = App.Data.SensorData.RHInside;   //17  13
	TxValidDataBuffer[index++] = App.Data.SensorData.TempOutside+40;   //18  14
	TxValidDataBuffer[index++] = (App.Data.SensorData.CO2Inside>>8);   //19  15
	TxValidDataBuffer[index++] = (App.Data.SensorData.CO2Inside&0xFF);   //20  16
	TxValidDataBuffer[index++] = App.Data.SensorData.PMInside>>8;   //21  17
	TxValidDataBuffer[index++] = App.Data.SensorData.PMInside&0xFF;   //22  18
//	*pdes++ = App.SysFault.PowerBaseFault;   //23  19
//	*pdes++ = App.SysFault.TempSensors;   //24  20
//	*pdes++ = App.SysFault.RHSensor;   //25  21
//	*pdes++ = App.SysFault.CO2Sensor;   //26  22
//	*pdes++ = App.SysFault.PMSensor;   //27  23
//	*pdes++ = App.SysFault.MotoXF;   //28  24
//	*pdes = App.SysFault.MotoPF;   //29  25
   
	 WifiTrans = 	SysParaUpload;
   System.Device.Wifi.HekrValidDataUpload(index);	
//   System.Device.Wifi.HekrValidDataUpload(26);	
}

static void AppWeekTimerSet(void)
{
	byte* psrc=RxValidDataBuffer;
	byte* pdes;
	byte i;

	psrc++ ; //5
    pdes= (byte*)&(App.Data.WeekTimerMode);
   for(i=0;i<7;i++)   //6-12
     *pdes++ = *psrc++;
}

static void AppWeekTimerUpload(void)
{
	byte index=0;
	byte* psrc;
	byte i;

	TxValidDataBuffer[index++] =WeekTimerUpload; //5
   psrc = (byte*)&(App.Data.WeekTimerMode);
   for(i=0;i<7;i++)   //6-12
     TxValidDataBuffer[index++] = *psrc++;
   WifiTrans = 	WeekTimerUpload;
   System.Device.Wifi.HekrValidDataUpload(index);	
}

static void AppTimerModeSet(void)
{
	byte* psrc=RxValidDataBuffer;
	byte* pdes;
	byte i, type;
	
	psrc++ ; //5
	type = *psrc++; //6
	switch(type)
	{
		case WORKDAYTYPE:
			 pdes= (byte*)&(App.Data.DayTimerMode.WorkDayTimer);
		   StorePost(STORE_WORKDAYMODE);
			break;
		case WEEKENDTYPE:
			 pdes = (byte*)&(App.Data.DayTimerMode.WeekendTimer);
		   StorePost(STORE_WEEKENDMODE);
			break;
		case HOLIDAYTYPE:
			 pdes = (byte*)&(App.Data.DayTimerMode.HolidayTimer);
		   StorePost(STORE_HOLIDAYMODE);
			break;
		case OFFICETYPE:
			 pdes = (byte*)&(App.Data.DayTimerMode.OfficeTimer);
		   StorePost(STORE_PARTYMODE);
			break;
		case ENERGYTYPE:
			pdes = (byte*)&(App.Data.DayTimerMode.EnergyTimer);
		   StorePost(STORE_ENERGYMODE);
			break;
	}
	for(i=0;i<25;i++)   //7-31
	   *pdes++ = *psrc++;

}

static void AppTimerModeUpload(byte type)
{
	byte index=0;
	byte* psrc;
	byte i;
	
	TxValidDataBuffer[index++] =TimerModeUpload; //5
	switch(type)
	{
		case WORKDAYTYPE:
			TxValidDataBuffer[index++] = WORKDAYTYPE; //6
			 psrc = (byte*)&(App.Data.DayTimerMode.WorkDayTimer);
			break;
		case WEEKENDTYPE:
			TxValidDataBuffer[index++] = WEEKENDTYPE; //6
			 psrc = (byte*)&(App.Data.DayTimerMode.WeekendTimer);
			break;
		case HOLIDAYTYPE:
			TxValidDataBuffer[index++] = HOLIDAYTYPE; //6
			 psrc = (byte*)&(App.Data.DayTimerMode.HolidayTimer);
			break;
		case OFFICETYPE:
			TxValidDataBuffer[index++] = OFFICETYPE; //6
			 psrc = (byte*)&(App.Data.DayTimerMode.OfficeTimer);
			break;
		case ENERGYTYPE:
			TxValidDataBuffer[index++] = ENERGYTYPE; //6
		 psrc = (byte*)&(App.Data.DayTimerMode.EnergyTimer);
			break;
	}
	for(i=0;i<25;i++)   //7-31
	   TxValidDataBuffer[index++] = *psrc++;
  
  System.Device.Wifi.HekrValidDataUpload(index);	
	WifiTrans = 	TimerModeUpload;
}

static void AppEnvParaSet(void)
{
	byte* psrc=RxValidDataBuffer;
	byte val;

	psrc++ ; //5
	App.Data.SysCtrlLine.TempInsideSummerLine= *psrc++; //6
	App.Data.SysCtrlLine.TempInsideWinterLine= *psrc++;   	//7
	val = *psrc++;
	switch(val)
	{
		case 1:
			App.Data.SysCtrlLine.PMInsideLine= 100;   //8
			break;
		case 2:
			App.Data.SysCtrlLine.PMInsideLine= 150;   //8
			break;
		case 3:
			App.Data.SysCtrlLine.PMInsideLine= 200;   //8
			break;
		case 4:
			App.Data.SysCtrlLine.PMInsideLine= 250;   //8
			break;
		default:
			break;
	}
	val = *psrc++;
	switch(val)
	{
		case 1:
			 App.Data.SysCtrlLine.CO2InsideLine= 800;   //9
			break;
		case 2:
			 App.Data.SysCtrlLine.CO2InsideLine= 1000;   //9
			break;
		case 3:
			 App.Data.SysCtrlLine.CO2InsideLine= 1200;   //9
			break;
		default:
			break;
	}
}

static void AppEnvParaUpload(void)
{
	byte index=0;

	TxValidDataBuffer[index++] =EnvParaUpload; //5  0
	TxValidDataBuffer[index++] =App.Data.SysCtrlLine.TempInsideSummerLine; //6 1
	TxValidDataBuffer[index++] = App.Data.SysCtrlLine.TempInsideWinterLine;   	//7  2
	switch(App.Data.SysCtrlLine.PMInsideLine)
	{
		case 100:
			TxValidDataBuffer[index++] = 1;   //8  3
			break;
		case 150:
			TxValidDataBuffer[index++] = 2;   //8
			break;
		case 200:
			TxValidDataBuffer[index++] = 3;   //8
			break;
		case 250:
			TxValidDataBuffer[index++] = 4;   //8
			break;
		default:
			break;
	}
	switch(App.Data.SysCtrlLine.CO2InsideLine)
	{
		case 800:
			TxValidDataBuffer[index++] = 1;   //9  4
			break;
		case 1000:
			TxValidDataBuffer[index++] = 2;   //9
			break;
		case 1200:
			TxValidDataBuffer[index++] = 3;   //9
			break;
		default:
			break;
	}
  
  System.Device.Wifi.HekrValidDataUpload(index);	
	WifiTrans = 	EnvParaUpload;
}

static void AppFilterSet(void)
{
	byte* psrc=RxValidDataBuffer;
	ushort val;
	
	psrc++ ; //5
	val = *psrc++;
	val <<=8;
	val |= *psrc++;
	App.Data.FilterHourLimit.FirstFilterHourLimit = val; //6
	val = *psrc++;
	val <<=8;
	val |= *psrc++;
	App.Data.FilterHourLimit.MidFilterHourLimit = val;   	//7
	val = *psrc++;
	val <<=8;
	val |= *psrc++;
	App.Data.FilterHourLimit.ESPFilterHourLimit = val;   //8
	val = *psrc++;
	val <<=8;
	val |= *psrc;
	App.Data.FilterHourLimit.HPFilterHourLimit = val;   //9
  StorePost(STORE_FILTERLIMIT);
}

static void AppFilterUpload(void)
{
	byte index=0;
	
	TxValidDataBuffer[index++] =FilterUpload; //5
	TxValidDataBuffer[index++] =App.Data.FilterHourLimit.FirstFilterHourLimit>>8; //6
	TxValidDataBuffer[index++] =App.Data.FilterHourLimit.FirstFilterHourLimit&0xff; //7
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.MidFilterHourLimit>>8;   	//8
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.MidFilterHourLimit&0xff;   	//9
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.ESPFilterHourLimit>>8;   //10
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.ESPFilterHourLimit&0xff;   //11
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.HPFilterHourLimit>>8;   //12
	TxValidDataBuffer[index++] = App.Data.FilterHourLimit.HPFilterHourLimit&0xff;   //13
	
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.FirstFilterTimerHour>>8;   //14
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.FirstFilterTimerHour&0xff;   //15
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.MidFilterTimerHour>>8;   //16
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.MidFilterTimerHour&0xff;   //17
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.ESPFilterTimerHour>>8;   //18
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.ESPFilterTimerHour&0xff;   //19
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.HPFilterTimerHour>>8;   //20
	TxValidDataBuffer[index++] = App.Data.FilterTimerHour.HPFilterTimerHour&0xff;   //21
  
	WifiTrans = 	FilterUpload;
  System.Device.Wifi.HekrValidDataUpload(index);	
}

static void AppDevTimeSet(void)
{
	byte* psrc=RxValidDataBuffer;
	ushort val;
	
	psrc++ ; //5
	val = *psrc++ ;
	val +=2000;
	App.SysTime.Year = val;
	val = *psrc++ ;
	App.SysTime.Mon = val;
	val = *psrc++ ;
	App.SysTime.Mday = val;
	val = *psrc++ ;
	App.SysTime.Wday = val;
	val = *psrc++ ;
	App.SysTime.Hour = val;
	val = *psrc++ ;
	App.SysTime.Min = val;
	System.Device.Rtc.Time_Adjust();
  
}

static void AppSYSFaultReport(void)
{
	uint val=0;
	byte i=0;
	byte index=0;

	SysFaultBuffer = App.SysFault.FaultFlag;
	TxValidDataBuffer[index++] =SYSFaultReport; //5
	
	val=POWERBASE_FAULT;
	for(i=0;i<7;i++)
	{
		if(SysFaultBuffer&val)
			TxValidDataBuffer[index++] =1;
		else
			TxValidDataBuffer[index++] =0;
		val <<=1;
	}
	val = FIRSTFILTER_CHECK;
	for(i=0;i<4;i++)
	{
		if(SysFaultBuffer&val)
			TxValidDataBuffer[index++] =1;
		else
			TxValidDataBuffer[index++] =0;
		val <<=1;
	}
	val = CO2INSIDEBEYOND;
	if(SysFaultBuffer&val)
		TxValidDataBuffer[index++] =1;
	else
		TxValidDataBuffer[index++] =0;
	val = PMINSIDEBEYONG;
	if(SysFaultBuffer&val)
		TxValidDataBuffer[index++] =1;
	else
		TxValidDataBuffer[index++] =0;
	
	WifiTrans = 	SYSFaultReport;
  System.Device.Wifi.HekrValidDataUpload(index);	
}

static void AppHistoryGraghDataReport(void)
{
	byte index=0;

	TxValidDataBuffer[index++] =HistoryGraghDataReport; //5
	TxValidDataBuffer[index++] = (App.Data.SensorData.CO2Inside>>8);   //6
	TxValidDataBuffer[index++] = (App.Data.SensorData.CO2Inside&0xFF);   //7
	TxValidDataBuffer[index++] = App.Data.SensorData.PMInside>>8;   //8
	TxValidDataBuffer[index++] = App.Data.SensorData.PMInside&0xFF;   //9

	WifiTrans = 	HistoryGraghDataReport;
  System.Device.Wifi.HekrValidDataUpload(index);	
}

void WifiRecvParse(byte type)
{
	switch(type)
	{
		case RecvDataSumCheckErr:
			break;
		case LastFrameSendErr:
			break;
		case MCU_UploadACK:
			WifiTrans = TRANSFINISH;
			break;
		case ValidDataUpdate:
			switch(RxValidDataBuffer[0])
			{
				case SysParaQuery:
					AppSysParaUpload();
					break;
				case DevPowerSet:
					App.Data.SysCtrlPara.Power =(PowerSetTypedef)RxValidDataBuffer[1];   	//7
					PostMessage(MessageParaUpdate, PARA_POWER_SET);
					break;
				case DevRunModeSet:
					App.Data.SysCtrlPara.WorkMode =(RunModeTypedef)RxValidDataBuffer[1];   //8
//				  if(RxValidDataBuffer[2]>15)
//					{
//						if((RxValidDataBuffer[2] != App.SysTime.Year)||(RxValidDataBuffer[3] != App.SysTime.Mon)||(RxValidDataBuffer[4] != App.SysTime.Mday)
//							||(RxValidDataBuffer[5] != App.SysTime.Hour)||(RxValidDataBuffer[6] != App.SysTime.Min))
//						{
//							App.SysTime.Year = RxValidDataBuffer[2];
//							App.SysTime.Mon = RxValidDataBuffer[3];
//							App.SysTime.Mday =RxValidDataBuffer[4] ;
//							App.SysTime.Hour = RxValidDataBuffer[5];
//							App.SysTime.Min =RxValidDataBuffer[6];
//							System.Device.Rtc.Time_Adjust();
//						}
//					}
					PostMessage(MessageParaUpdate, PARA_WORKMODE);
					StorePost(STORE_SYSPARA);
					break;
				case DevCircleModeSet:
					App.Data.SysCtrlPara.CircleModeSet =(CircleModeSetTypedef)RxValidDataBuffer[1];   //10
					PostMessage(MessageParaUpdate, PARA_CIRCLEMODE);
					StorePost(STORE_SYSPARA);
					break;
				case DevMuteSet:
					App.Data.SysCtrlPara.MuteSet =(MutSetTypedef)RxValidDataBuffer[1];   //9
					PostMessage(MessageParaUpdate, PARA_MUTEMODE);
					break;
				case DevChildLockSet:
					App.SysCtrlStatus.ChildLock =(ChildLockTypedef)RxValidDataBuffer[1];   //14
					PostMessage(MessageParaUpdate, PARA_CHILDLOCK);
					break;
				case DevHeatModeSet:
					App.Data.SysCtrlPara.ThermalModeSet =(HeatModeTypedef)RxValidDataBuffer[1];   //11
					PostMessage(MessageParaUpdate, PARA_THERMALMODE);
					StorePost(STORE_SYSPARA);
					break;
				case DevVentilateRateSet:
					App.Data.SysCtrlPara.VentilateRate =(VentilateRateTypedef)RxValidDataBuffer[1];   //12
					PostMessage(MessageParaUpdate, PARA_VENTILATE);
					StorePost(STORE_SYSPARA);
					break;
				case DevAirflowSet:
					if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
					{
						App.SysCtrlStatus.AirFlowSet =(AirFlowSetEnum)RxValidDataBuffer[1];  
						App.Data.SysCtrlPara.AirFlowSet=App.SysCtrlStatus.AirFlowSet;
						PostMessage(MessageParaUpdate, PARA_XFMOTODUTY);
					}
					StorePost(STORE_SYSPARA);
					break;
				case DevTimeSet:
					App.Data.SysCtrlPara.WorkMode = ALLBYHAND;
					AppDevTimeSet();				
					break;
				case WeekTimerQuery:
					AppWeekTimerUpload();
					break;
				case WeekTimerSet:
					AppWeekTimerSet();
	StorePost(STORE_WEEKTIMER);
	PostMessage(MessageAlarm, ALARM_INIT);
					break;
				case TimerModeQuery:
					AppTimerModeUpload(RxValidDataBuffer[1]);
					break;
				case TimerModeSet:
					AppTimerModeSet();
	PostMessage(MessageAlarm, ALARM_INIT);
					break;
				case EnvParaQuery:
					AppEnvParaUpload();
					break;
				case EnvParaSet:
					AppEnvParaSet();
  StorePost(STORE_ENVPARA);
					break;
				case FilterQuery:
					AppFilterUpload();
					break;
				case FilterLimitSet:
					AppFilterSet();
				PostMessage(MessageProcess, FILTER_CHECK);
					break;
				default:
					break;
			}
			break;
		case RecvDataUseless:
			break;
		case HekrModuleStateUpdate:
			switch(ModuleStatus[1])
			{
				case STA_Mode:
					App.WifiState &= ~WIFI_STATE;
				if(ModuleStatus[3]==CloudConnected)
					App.WifiState |= WIFI_STATE_STA;
				else
					App.WifiState |= WIFI_STATE_STA_AP;
					break;
				case HekrConfig_Mode:
					App.WifiState &= ~WIFI_STATE;
					App.WifiState |= WIFI_STATE_CONFIG;
					break;
				case AP_Mode:
					break;
				case STA_AP_Mode:
					App.WifiState &= ~WIFI_STATE;
					App.WifiState |= WIFI_STATE_STA_AP;
					break;
				default:
					break;
			}
			/* HEKR_TEST_V4 Router Found, Module Signal Receive Normal*/
			
			App.Menu.MainForm.RefreshFlag |= MAINFORM_WIFISTATE;
			
			break;
		case MCU_ControlModuleACK:
			break;
		default:
			break;
	}
}

void WifiCtrlCode(byte code)
{
	switch(code)
	{
		case ModuleQuery:
			System.Device.Wifi.HekrModuleControl(ModuleQuery);
			break;
		case ModuleRestart:
			System.Device.Wifi.HekrModuleControl(ModuleRestart);
			break;
		case ModuleRecover:
			System.Device.Wifi.HekrModuleControl(ModuleRecover);
			App.WifiState = 0;
			break;
		case HekrConfig:
			System.Device.Wifi.HekrModuleControl(HekrConfig);
			App.WifiState &= ~WIFI_STATE;
			App.WifiState |= WIFI_STATE_CONFIG;
		App.Menu.MainForm.RefreshFlag |= MAINFORM_WIFISTATE;
			break;
		case HekrProdKey:
			break;
	}
}

static void WifiUpload(void)
{
	byte i;
	uint val=0x80000000;
	
	if(WifiTrans == TRANSFINISH)
	{
	
		for(i=0;i<16;i++)
		{
			if(App.WifiState&val)
				break;
			else
				val >>= 1;
		}		

		switch(i)
		{
			case 5:
				AppSysParaUpload();
				App.WifiState &= ~WIFI_UPLOAD_SYSPARA;
				WifiTrans = SysParaUpload;
				break;
			case 6:
				AppWeekTimerUpload();
				App.WifiState &= ~WIFI_UPLOAD_WEEKTIMER;
				WifiTrans = WeekTimerUpload;
				break;
			case 7:
				AppEnvParaUpload();
				App.WifiState &= ~WIFI_UPLOAD_ENVPARA;
				WifiTrans = EnvParaUpload;
				break;
			case 8:
				AppFilterUpload();
				App.WifiState &= ~WIFI_UPLOAD_FILTER;
				WifiTrans = FilterUpload;
				break;
			case 9:
				AppHistoryGraghDataReport();
				App.WifiState &= ~WIFI_UPLOAD_GRAPHDATA;
				WifiTrans = HistoryGraghDataReport;
				break;
			case 10:
				if(SysFaultBuffer != App.SysFault.FaultFlag)
				{
					AppSYSFaultReport();
					WifiTrans = SYSFaultReport;
				}
				App.WifiState &= ~WIFI_UPLOAD_FAULT;
				break;
			default :
				break;
		}
	}
	else
	{
		switch(WifiTrans)
		{
			case SysParaUpload:
				AppSysParaUpload();
				break;
			case WeekTimerUpload:
				AppWeekTimerUpload();
				break;
			case EnvParaUpload:
				AppEnvParaUpload();
				break;
			case FilterUpload:
				AppFilterUpload();
				break;
			case HistoryGraghDataReport:
				AppHistoryGraghDataReport();
				break;
			case SYSFaultReport:
				AppSYSFaultReport();
				break;
			default :
				WifiTrans = TRANSFINISH;
				break;
		}
	}
}

static void WifiSystick10(void)
{
	static byte second=0;
	static ushort graphtimer=0;
	
	if(++second>9)   //per 1s
	{
		second=0;
		if((App.WifiState&WIFI_STATE_STA)&&(App.WifiState&0x07E00000))
				WifiUpload();
	
				
	
		graphtimer++;
		if((graphtimer%30==0)&&(!(App.WifiState&WIFI_STATE_STA)))
					WifiCtrlCode(ModuleQuery);
		if(graphtimer>900)
		{
			graphtimer =1;
				if(App.SysFault.FaultFlag)
					App.WifiState |= WIFI_UPLOAD_FAULT;
			
			if(App.Data.SysCtrlPara.Power != POWER_OFF)
				App.WifiState |= WIFI_UPLOAD_GRAPHDATA;

		}
	}
	
}

void AppWifiInit(void)
{
	System.Device.Systick.Register(Systick10, WifiSystick10);
	System.Device.Wifi.InitWifi(RxValidDataBuffer,TxValidDataBuffer,ModuleStatus);
	WifiCtrlCode(ModuleQuery);
//	System.Device.Wifi.HekrModuleControl(HekrConfig);
}


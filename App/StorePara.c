#include "app.h"

#define SYSTEM_INITFLAG  (uint8_t)0x33
	
TestStatus StoreParaOpt(StorePara_TypDef paratype,StoreOpt_TypDef oper)
{
	uint8_t* pBuffer=0;
	uint16_t OperAddr=0;
	uint16_t NumByteToOper=0; //max=1023
	TestStatus flag=FAILED;
	
	switch(paratype)
	{
		case STORE_USEDTIMER:
			pBuffer = (uint8_t*)&(App.Data.UsedHours);
			OperAddr = (uint16_t)EEPROM_ADDRESS_USEDTIMER;
			NumByteToOper = SIZEOF_USEDTIMER;
			break;
		case STORE_SYSPARA:
			pBuffer = (uint8_t*)&(App.Data.SysCtrlPara);
			OperAddr = (uint16_t)EEPROM_ADDRESS_SYSPARA;
			NumByteToOper = SIZEOF_SYSPARA;
			break;
		case STORE_FILTERTIMER:
			pBuffer = (uint8_t*)&App.Data.FilterTimerHour;
			OperAddr = EEPROM_ADDRESS_FILTERTIMER;
			NumByteToOper = SIZEOF_FILTERTIMER;
			break;
		case STORE_FILTERLIMIT:
			pBuffer = (uint8_t*)&App.Data.FilterHourLimit;
			OperAddr = EEPROM_ADDRESS_FILTERLIMIT;
			NumByteToOper = SIZEOF_FILTERLIMIT;
			break;
		case STORE_ENVPARA:
			pBuffer = (uint8_t*)&App.Data.SysCtrlLine;
			OperAddr = EEPROM_ADDRESS_ENVPARA;
			NumByteToOper = SIZEOF_ENVPARA;
			break;
		case STORE_WEEKTIMER:
			pBuffer = (uint8_t*)&App.Data.WeekTimerMode;
			OperAddr = EEPROM_ADDRESS_WEEKTIMER;
			NumByteToOper = SIZEOF_WEEKTIMER;
			break;
		case STORE_WORKDAYMODE:
			pBuffer = (uint8_t*)&App.Data.DayTimerMode.WorkDayTimer;
			OperAddr = EEPROM_ADDRESS_WORKDAYMODE;
			NumByteToOper = SIZEOF_WORKDAYMODE;
			break;
		case STORE_WEEKENDMODE:
			pBuffer = (uint8_t*)&App.Data.DayTimerMode.WeekendTimer;
			OperAddr = EEPROM_ADDRESS_WEEKENDMODE;
			NumByteToOper = SIZEOF_WEEKENDMODE;
			break;
		case STORE_HOLIDAYMODE:
			pBuffer = (uint8_t*)&App.Data.DayTimerMode.HolidayTimer;
			OperAddr = EEPROM_ADDRESS_HOLIDAYMODE;
			NumByteToOper = SIZEOF_HOLIDAYMODE;
			break;
		case STORE_PARTYMODE:
			pBuffer = (uint8_t*)&App.Data.DayTimerMode.OfficeTimer;
			OperAddr = EEPROM_ADDRESS_PARTYMODE;
			NumByteToOper = SIZEOF_PARTYMODE;
			break;
		case STORE_ENERGYMODE:
			pBuffer = (uint8_t*)&App.Data.DayTimerMode.EnergyTimer;
			OperAddr = EEPROM_ADDRESS_ENERGYMODE;
			NumByteToOper = SIZEOF_ENERGYMODE;
			break;

		default:
			break;
	}
	if(oper==READFROMSTORE)
		flag = System.Device.Eeprom.I2C_EE_BufferRead(pBuffer,  OperAddr,  NumByteToOper);
	else if(oper==WRITETOSTORE)
		flag = System.Device.Eeprom.I2C_EE_BufferWrite(pBuffer,  OperAddr,  NumByteToOper);
	
	return flag;
}


TestStatus IfSysInit(void)
{
	uint8_t flag=0;
	
	System.Device.Eeprom.I2C_EE_BufferRead(&flag,  EEPROM_ADDRESS_INITFLAG,  1);
	if(flag == SYSTEM_INITFLAG)
		return PASSED;
	else
	{
		flag= SYSTEM_INITFLAG;
		System.Device.Eeprom.I2C_EE_BufferWrite(&flag,  EEPROM_ADDRESS_INITFLAG,  1);
		return FAILED;
	}
	
}

void StorePost(StorePara_TypDef paratype)
{
	byte i;
	uint val = 1;
	
	i = (byte)paratype;
	App.StoreState |= val<<i;
}

void StorePara(void)
{
	byte i;
	uint val=0x00000001;
	TestStatus flag=FAILED;
	
	if(App.StoreState)
	{
		for(i=0;i<32;i++)
		{
			if(App.StoreState&val)
				break;
			else
				val <<= 1;
		}
		switch(i)
		{
			case STORE_USEDTIMER:
				flag = StoreParaOpt(STORE_USEDTIMER,WRITETOSTORE);
				break;
			case STORE_SYSPARA:
				flag = StoreParaOpt(STORE_SYSPARA,WRITETOSTORE);
				break;
			case STORE_FILTERTIMER:
				flag = StoreParaOpt(STORE_FILTERTIMER,WRITETOSTORE);
				break;
			case STORE_FILTERLIMIT:
				flag = StoreParaOpt(STORE_FILTERLIMIT,WRITETOSTORE);
				break;
			case STORE_ENVPARA:
				flag = StoreParaOpt(STORE_ENVPARA,WRITETOSTORE);
				break;
			case STORE_WEEKTIMER:
				flag = StoreParaOpt(STORE_WEEKTIMER,WRITETOSTORE);
				break;
			case STORE_WORKDAYMODE:
				flag = StoreParaOpt(STORE_WORKDAYMODE,WRITETOSTORE);
				break;
			case STORE_WEEKENDMODE:
				flag = StoreParaOpt(STORE_WEEKENDMODE,WRITETOSTORE);
				break;
			case STORE_HOLIDAYMODE:
				flag = StoreParaOpt(STORE_HOLIDAYMODE,WRITETOSTORE);
				break;
			case STORE_PARTYMODE:
				flag = StoreParaOpt(STORE_PARTYMODE,WRITETOSTORE);
				break;
			case STORE_ENERGYMODE:
				flag = StoreParaOpt(STORE_ENERGYMODE,WRITETOSTORE);
				break;
			default:
				flag = PASSED;
				break;
		}
		if(flag == FAILED)
			App.StoreState |= val;
		else
			App.StoreState &= ~val;
	}
}


static TestStatus CheckStore(void)
{
	byte i;
	byte* pbyte;
	ushort* pushort;
	
	pushort = (ushort*)&App.Data.FilterTimerHour.FirstFilterTimerHour;
	for(i=0;i<4;i++)
	{
		if(*pushort>9999)
		{
			return FAILED;
		}
		pushort++;
	}
	
	pushort = (ushort*)&App.Data.FilterHourLimit.FirstFilterHourLimit;
	for(i=0;i<4;i++)
	{
		if((*pushort>5000)||(*pushort<200))
		{
			App.Data.FilterHourLimit.FirstFilterHourLimit = 2000;
			App.Data.FilterHourLimit.MidFilterHourLimit = 2000;
			App.Data.FilterHourLimit.ESPFilterHourLimit = 1000;
			App.Data.FilterHourLimit.HPFilterHourLimit = 3000;
			StoreParaOpt(STORE_FILTERLIMIT,WRITETOSTORE);
			i=4;
//			return FAILED;
		}
		pushort++;
	}
	
	pbyte = (byte*)&App.Data.WeekTimerMode.MondayTimer;
	for(i=0;i<7;i++)
	{
		if(*pbyte>ENERGYTYPE)
			return FAILED;
		pbyte++;
	}
	

	return PASSED;
}

void InitPara(void)
{
	StorePara_TypDef paratyp;
	TestStatus flag=FAILED;
	byte i;
	
	
	if(IfSysInit()==FAILED)
	{
		i=3;
		while(--i)
		{
			for(paratyp=STORE_SYSPARA;paratyp<STORE_PARAINIT;paratyp++)
			{
				flag =StoreParaOpt(paratyp,WRITETOSTORE);		
				if(flag == FAILED)
					break;
			}
		}
		
		if(flag == FAILED)
		{
			System.Device.Led.LedModeSet(LED_DOUBLE,TURN_ON);
			while(1){};
		}
	}
	
	for(paratyp=STORE_SYSPARA;paratyp<STORE_PARAINIT;paratyp++)
	{
		flag =StoreParaOpt(paratyp,READFROMSTORE);	
		if(flag == FAILED)
			break;
	}
	if(flag == FAILED)
	{
		System.Device.Led.LedModeSet(LED_DOUBLE,TURN_ON);
		while(1){};
	}
	 
	i=5;
	while(--i)
	{
	  if(CheckStore()==FAILED)
		{
			for(paratyp=STORE_SYSPARA;paratyp<STORE_PARAINIT;paratyp++)
			{
				flag =StoreParaOpt(paratyp,READFROMSTORE);	
				if(flag == FAILED)
					break;
			}
		}
		else
			break;
	}	
	if(!i)
		App.SysFault.FaultFlag |= STORE_FAULT;
	
	if(App.Data.SysCtrlPara.Power != POWER_OFF)
		App.Data.SysCtrlPara.Power = POWER_ON;
	
	if(App.Data.SysCtrlPara.AirFlowSet>AIRFLOW_STEP20)
		App.Data.SysCtrlPara.AirFlowSet=AIRFLOW_STEP20;
	if(App.Data.SysCtrlPara.AirFlowSet<AIRFLOW_STEP1)
		App.Data.SysCtrlPara.AirFlowSet=AIRFLOW_STEP1;
	
	if((App.Data.SysCtrlLine.CO2InsideLine<800)||(App.Data.SysCtrlLine.CO2InsideLine>1200))
		App.Data.SysCtrlLine.CO2InsideLine=1000;
	if((App.Data.SysCtrlLine.PMInsideLine <100)||(App.Data.SysCtrlLine.PMInsideLine >250))
		App.Data.SysCtrlLine.PMInsideLine =150;
	if((App.Data.SysCtrlLine.TempInsideSummerLine<25)||(App.Data.SysCtrlLine.TempInsideSummerLine>29))
		App.Data.SysCtrlLine.TempInsideSummerLine=25;
	if((App.Data.SysCtrlLine.TempInsideWinterLine<14)||(App.Data.SysCtrlLine.TempInsideWinterLine>18))
		App.Data.SysCtrlLine.TempInsideWinterLine=18;
	
//	PostMessage(MessageParaUpdate, PARA_POWER_SET);
		

}

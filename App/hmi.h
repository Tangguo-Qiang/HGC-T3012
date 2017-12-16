#ifndef __HMI_H
#define __HMI_H



typedef enum
{
	FORM_FOCUS=(byte)0,
	SYSPARA_CIRCLEMODE=(byte)1,
	SYSPARA_VERTIMODE=(byte)2,
	SYSPARA_THERMALMODE=(byte)3,
	SYSPARA_SYSTEMTIME=(byte)4,
	
	WEEKTIMER_MONDAY=(byte)5,
	WEEKTIMER_TUESDAY=(byte)6,
	WEEKTIMER_WEDNESDAY=(byte)7,
	WEEKTIMER_THURSDAY=(byte)8,
	WEEKTIMER_FRIDAY=(byte)9,
	WEEKTIMER_SATURDAY=(byte)10,
	WEEKTIMER_SUNDAY=(byte)11,
	
	PARATITLE_FIRSTFILTER=(byte)12,
	PARATITLE_MIDFILTER=(byte)13,
	PARATITLE_ESPFILTER=(byte)14,
	PARATITLE_HPFILTER=(byte)15,
	
	PARATITLE_PMCTRLSET=(byte)16,
	PARATITLE_TEMPCTRLSET=(byte)17,
	PARATITLE_CO2CTRLSET=(byte)18,
	
	PARATITLE_COREFAULT=(byte)19,
	PARATITLE_NORMALFAULT=(byte)20,
	PARATITLE_FILTER=(byte)21,
}ParaTitle_Typedef;	

typedef enum
{
	SYSPARA_CIRCLEOUT=(byte)5,
	SYSPARA_CIRCLEIN=(byte)6,
	SYSPARA_CIRCLEAUTO=(byte)7,
	SYSPARA_RATE06=(byte)8,
	SYSPARA_RATE08=(byte)9,
	SYSPARA_RATE10=(byte)10,
	SYSPARA_RATE12=(byte)11,
	SYSPARA_THERMALOPEN=(byte)12,
	SYSPARA_THERMALCLOSE=(byte)13,
	SYSPARA_THERMALAUTO=(byte)14,
	SYSPARA_TIMEDATE=(byte)15,
	SYSPARA_TIMEMIN=(byte)16
}SysPara_DotLoction;

typedef enum
{
	MODECTRL_PMCHAR=(byte)1,
	MODECTRL_TEMPCHAR=(byte)2,
	MODECTRL_CO2CHAR=(byte)3,
	MODECTRL_100UG=(byte)4,
	MODECTRL_150UG=(byte)5,
	MODECTRL_200UG=(byte)6,
	MODECTRL_250UG=(byte)7,
//	MODECTRL_SUMMER=(byte)8,
//	MODECTRL_WINTER=(byte)9,
	MODECTRL_14OC=(byte)8,
	MODECTRL_15OC=(byte)9,
	MODECTRL_16OC=(byte)10,
	MODECTRL_17OC=(byte)11,
	MODECTRL_18OC=(byte)12,
	MODECTRL_25OC=(byte)13,
	MODECTRL_26OC=(byte)14,
	MODECTRL_27OC=(byte)15,
	MODECTRL_28OC=(byte)16,
	MODECTRL_29OC=(byte)17,
	MODECTRL_800PPM=(byte)18,
	MODECTRL_1000PPM=(byte)19,
	MODECTRL_1200PPM=(byte)20
}ModeCtrl_DotLoction;

typedef enum
{
	SUBTITLE_EMPTY=(byte)0,
	SUBTITLE_RESET=(byte)1,
	SUBTITLE_LIMITHOUR=(byte)2,
}SubTitle_TyepDef;

typedef enum
{
	PARAVAL_YEAR=(byte)1,
	PARAVAL_MONTH=(byte)2,
	PARAVAL_DAY=(byte)3,
	PARAVAL_HOUR=(byte)4,
	PARAVAL_MIN=(byte)5
}ParaVal_TypeDef;

typedef enum
{
	FOCUS_ONFORM=(byte)1,
	FOCUS_PARATITLE=(byte)2,
	FOCUS_PARASUBTITLE=(byte)3,
	FOCUS_PARAVAL=(byte)4,
	FOCUS_PARAVALSET=(byte)5,
	FOCUS_VERIFY=(byte)6
}Focus_TypeDef;


typedef struct
{
	Focus_TypeDef Focus;
	ParaTitle_Typedef ParaTitle;
	byte ParaSubTitle;
	byte Position;
  void *val;	
}BTN_ACTTypeDef;


extern void InitHmi(void);
extern void KeyProcessMainForm(KeyActEnum key);
extern void KeyProcessSysParaForm(KeyActEnum key);
extern void KeyProcessWeekTimerForm(KeyActEnum key);
extern void KeyProcessFilterForm(KeyActEnum key);
extern void KeyProcessModeCtrlForm(KeyActEnum key);
extern void KeyProcessSleepForm(KeyActEnum key);
extern void KeyProcessFaultInfoForm(KeyActEnum key);

#endif

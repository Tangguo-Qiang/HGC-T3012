/*
********************************************************************************
*文件名     : device.h
*作用       : 定义设备层相关宏定义
*原理       : 
********************************************************************************
********************************************************************************
*/


#ifndef __DEVICE_H
#define __DEVICE_H

#include "drive.h"

#define LCD_BL_ON     		PdOut(12)=1
#define LCD_BL_OFF     		PdOut(12)=0 

typedef enum 
{
    KeyPower                  = 0x01,
    KeyLongPower              = 0x31,
    
    KeyMode                  = 0x02,
    KeyLongMode              = 0x32,

    KeyRate                = 0x03,
    KeyLongRate            = 0x33,

    KeyRollCenter              = 0x04,
    KeyLongRollCenter          = 0x34,
    
    KeyRollCW                 = 0x05,

    KeyRollCCW         = 0x06,
	
	  KeyRollCenterRate   = 0x07,
	  KeyLongRollCenterRate   = 0x37,
	
	  KeyPowerMode   = 0x08,
	  KeyLongPowerMode   = 0x38,
}KeyActEnum;

typedef enum 
{
    TimerSystick            = 0,
    TimerMessage            = 1	
}TimerModeEnum;	

typedef enum
{
    Systick10000,
    Systick1000,
    Systick100,
    Systick10
}SystickEnum;

typedef enum{
	TEMP_TWINKLE = (byte)0x01,
	LED_TWINKLE = (byte)0x02,
	TURN_ON = (byte)0x03,
	TURN_OFF = (byte)0x04,
	BREATH_ON = (byte)0x05
}LedModeTypedef;

typedef enum{
	LED_RED = (byte)0x01,
	LED_GREEN = (byte)0x02,
	LED_DOUBLE = (byte)0x03
}LedTypedef;


typedef enum{
        AIRFLOW_STOP=(byte)0x00,                 /* 0 */
        AIRFLOW_STEP1=(byte)0x01,                 /* 0 */
        AIRFLOW_STEP2=(byte)0x02,                /* 1 */
        AIRFLOW_STEP3=(byte)0x03,                /* 2 */
        AIRFLOW_STEP4=(byte)0x04,                /* 3 */
        AIRFLOW_STEP5=(byte)0x05,                /* 4 */
        AIRFLOW_STEP6=(byte)0x06,                /* 5 */
        AIRFLOW_STEP7=(byte)0x07,                /* 6 */
        AIRFLOW_STEP8=(byte)0x08,                /* 7 */
        AIRFLOW_STEP9=(byte)0x09,                /* 8 */
        AIRFLOW_STEP10=(byte)0x0A,                /* 9 */
        AIRFLOW_STEP11=(byte)0x0B,                /* 10 */
        AIRFLOW_STEP12=(byte)0x0C,                /* 11 */
        AIRFLOW_STEP13=(byte)0x0D,                /* 12 */
        AIRFLOW_STEP14=(byte)0x0E,                /* 13 */
        AIRFLOW_STEP15=(byte)0x0F,                /* 14 */
        AIRFLOW_STEP16=(byte)0x10,                /* 15 */
        AIRFLOW_STEP17=(byte)0x11,                /* 16 */
        AIRFLOW_STEP18=(byte)0x12,                /* 17 */
        AIRFLOW_STEP19=(byte)0x13,                /* 18 */
        AIRFLOW_STEP20=(byte)0x14,               /* 19 */
}AirFlowSetEnum;

typedef struct{
	ushort Wday;
	ushort Year;
	ushort Mon;
	ushort Mday;
	short Hour;
	short Min;
	short Sec;
//	short flag;
}RTC_TimeTypeDef;



typedef	enum
{
	RecvDataSumCheckErr = (byte)0x01,
	LastFrameSendErr = (byte)0x02,
	MCU_UploadACK = (byte)0x03,
	ValidDataUpdate = (byte)0x04,
	RecvDataUseless = (byte)0x05,
	HekrModuleStateUpdate = (byte)0x06,
	MCU_ControlModuleACK = (byte)0x07,
	MCU_ProdKeyModuleACK = (byte)0x08
}RecvDataHandleCode;

//Hekr模块控制码
typedef	enum
{
	ModuleQuery = 0x01,
	ModuleRestart = 0x02,
	ModuleRecover = 0x03,
	HekrConfig = 0x04,
	HekrProdKey = 0x21
}HekrModuleControlCode;

#define WIFIUSER_MAX_LEN 50u


/******常用颜色*****/
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F  
#define BRED    0xF81F
#define GRED    0xFFE0
#define GBLUE   0x07FF
#define BLACK   0x0000
#define WHITE   0xFFFF
#define	DBROWN	0x6163
#define GREY	0xC618
#define DBLUE	0x006F
#define BKCOLOR  GBLUE
#define	DGREYB	0x0989 	//深灰蓝
#define	LGREYB	0x5BF2 	//浅灰蓝
#define LGBLUE	0xd6fd  //极浅蓝绿
#define ORANGE	0xFC40  //橘红色
#define	YELLOW	GRED

typedef struct
{
  ushort col;                /*!< 显示起始点列号 ，x*/
  ushort row;                /*!< 显示起始点行号 ,y*/
  ushort wide;          /*!< 字符宽度，单位：像素 */
  ushort heigh;                /*!< 字符高度 ，单位：像素*/
  ushort color;                /*!< 前景色*/
  ushort bkcolor;                /*!< 背景色*/
  const byte * pFig;
}LCD_CharTypeDef;

typedef struct
{
  ushort col;                /*!< 显示起始点列号 ，x*/
  ushort row;                /*!< 显示起始点行号 ,y*/
  ushort wide;          /*!< 字符宽度，单位：像素 */
  ushort heigh;                /*!< 字符高度 ，单位：像素*/
  ushort figloc;
}LCD_FigFlashTypeDef;

typedef struct
{
  ushort col;                /*!< 显示起始点列号 ，x*/
  ushort row;                /*!< 显示起始点行号 ,y*/
  ushort length;          /*!< 线条长度*/
  ushort bold;                /*!<线条宽度 ，单位：像素*/
  ushort fontcolor;
}LCD_LineTypeDef;

typedef struct
{
  ushort col;                /*!< 显示起始点列号 ，x*/
  ushort row;                /*!< 显示起始点行号 ,y*/
  ushort width;          /*!< 线条长度*/
  ushort Height;                /*!<线条宽度 ，单位：像素*/
  ushort color;
}LCD_RectangleTypeDef;

typedef enum{
	NOFLOW=(byte)0,
	STARTFLOW=(byte)0x01,
	STOPFLOW=(byte)0x02
}AlarmGoalTypeDef;

typedef struct{
	byte Nextnum;
	AlarmGoalTypeDef Alarmgoal;
	AirFlowSetEnum AirflowSet;
}AlarmSysTypeDef;

extern void InitDevice(void);

#endif



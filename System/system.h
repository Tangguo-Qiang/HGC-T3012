/*
********************************************************************************
*文件名   : system.h
*作用     : 系统库全局头文件
*创建时间 : 2013.7.21
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


//#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <stdarg.h>



/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit
//typedef char                sbyte;	    /*char Keil4默认定义为byte，若要带符号，需要更改keil设置*/
typedef unsigned char       byte;       
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;

//字符串
typedef char *              string;

typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)   ((function)(address))()


typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;






#define true                        1
#define false                       0
#define null                        ((void *) 0)                    // 空指针
#define invalid                     0xFF                            // 无效值
#define MainClock                   72000000                        // 系统时钟频率
#define IdentifyNumberAddress       0x1FFFF7E8                      // 芯片ID号

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // 置位
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // 复位
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // 获取位

#define GetByte0FromUint(data)      ((byte *)(&(data)))[0]
#define GetByte1FromUint(data)      ((byte *)(&(data)))[1]
#define GetByte2FromUint(data)      ((byte *)(&(data)))[2]
#define GetByte3FromUint(data)      ((byte *)(&(data)))[3]

#define GetUshort0FromUint(data)    ((ushort *)(&(data)))[0]
#define GetUshort1FromUint(data)    ((ushort *)(&(data)))[1]

#define GetMessageType(data)        GetByte3FromUint(data)
#define GetMessageData(data)        (data & 0x00FFFFFF)

#define GetByteFrom(address)        *((byte *)(address)) 
#define GetUshortFrom(address)      *((ushort *)(address)) 
#define GetUintFrom(address)        *((uint *)(address)) 

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0


typedef enum
{
    MessageKey       		 = 0x01,
    MessageCommRecv      = 0x02,
    MessageCommTrans     = 0x03,
    MessageIR    				 = 0x04,
    MessageAlarm       = 0x05,
    MessageParaUpdate    = 0x06,
    MessageEchoData    	 = 0x07,
    MessageWifiRecv      = 0x08,
    MessageWifiCtrl      = 0x09,
    MessageWifiUpload      = 0x0A,
    MessageProcess      = 0x0B,
    MessageTimer   
    /*请填充类型*/
}MessageEnum;

typedef enum{
	COMM_TEMPER_RH =(byte)1,
	COMM_XFMOTODUTY =(byte)2,
	COMM_PFMOTODUTY =(byte)3,
	COMM_CIRCLEMODE =(byte)4,
	COMM_THERMALMODE =(byte)5,
	COMM_VENTILATE =(byte)6,
	COMM_IAQ_READ =(byte)7,
	COMM_CO2_READ =(byte)8,
	COMM_POWER_SET =(byte)9,
	COMM_BEEPONE =(byte)0x0A,
	COMM_BEEPTWO =(byte)0x0B,
	COMM_BEEPTHREE =(byte)0x0C,
	COMM_BEEPLONG =(byte)0x0D,
	COMM_VERSION =(byte)0xEE
}CommOrderTypedef;


typedef enum{
	PARA_POWER_SET = (uint)0x10,
	PARA_XFMOTODUTY = (uint)0x20,
	PARA_PFMOTODUTY = (uint)0x30,
	PARA_CIRCLEMODE = (uint)0x40,
	PARA_THERMALMODE = (uint)0x50,
	PARA_VENTILATE = (uint)0x60,
	PARA_MUTEMODE = (uint)0x70,
	PARA_WORKMODE = (uint)0x80,
	PARA_CHILDLOCK = (uint)0x90,
	PARA_SYSTEM_UPDATE = (uint)0xF0
}ParaOperTypedef;

typedef enum{
	ALARM_SOUND = (uint)0x100,
	ALARM_START = (uint)0x200,
	ALARM_INIT = (uint)0x300
}AlarmTypedef;

typedef enum{
	FILTER_ONEHOUR = (uint)0x1000,
	FILTER_CLEAR = (uint)0x2000,
	FILTER_CHECK = (uint)0x3000,
	FAULTINFO_DISP = (uint)0x4000
}ProcessTypedef;

#include "sysdefs.h"
#include "device.h"  
#include "os.h"
#include "gui.h"

//extern byte TimeOut1ms;
extern byte GifTimer100ms;
extern uint Empty;
extern void Dummy(void);
extern ushort CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

#define EnableIrq()         __enable_irq();
#define DisableIrq()        __disable_irq();



extern void DelayNull(int times);

extern void DelayUsNull(int times);

extern void DelayMsNull(int times);

extern byte HexToAscii(byte hex);

extern void Fill_BufferZero(uint8_t *pBuffer, uint16_t BufferLength);

extern void PostMessage(MessageEnum message, uint data);

/*******************************************************************************
* 结构体名	: SystemStruct
* 描述	    : msOS中最重要的一个结构体，封装了System层的所有接口
*******************************************************************************/
typedef struct
{
    void (*Init)(void);
    
    struct Device
    {     
      
        struct Rtc
        {
            void (*Register)(RTC_TimeTypeDef * rtcPointer);
            void (*Time_Check)(void);
            void (*Time_Adjust)(void);
            void (*Time_Read)(FunctionalState State);
					  void (*AlarmSet)(uint TimeSec);
					   void (*RTC_SetFlag)(byte reg);
        }Rtc;

        
        struct Lcd
        {
            void (* LCD_Reset)(void);
            void (* LCD_ClearScreen)(byte menu);
            void (* LCD_Num)(LCD_CharTypeDef* LCDnum,byte num);
            void (* LCD_ColLine)(LCD_LineTypeDef* line);
            void (* LCD_RowLine)(LCD_LineTypeDef* line);
					  void (*LCD_ColorRectangle)(LCD_RectangleTypeDef* rectangle);
            void (* LCD_FigureInFlash)(LCD_FigFlashTypeDef* figinfo);
            void (* LCD_FigNuLL)(LCD_FigFlashTypeDef* figinfo);
            void (* LCD_NumFigInFlash)(LCD_FigFlashTypeDef* figinfo,byte num);
        }Lcd;
        
        struct Usart1
        {
            void (*Open)(void);
            void (*Close)(void);
            bool (*WriteByte)(byte data);
            bool (*ReadByte)(byte* data); 

        }Usart1;				
        
        struct Wifi
        {
					void (*InitWifi)(byte* RxValidDateBuffer,byte* TxValidDateBuffer,byte* ModuleStatus);
					void (*HekrModuleControl)(byte data);
					void (*HekrValidDataUpload)(byte len);
        }Wifi;
				
        struct Spi1
        {
            uint (*SPI_FLASH_ReadDeviceID)(void);
            uint (*SPI_FLASH_ReadID)(void);
        }Spi1;
				
        struct Eeprom
        {
            TestStatus (*I2C_EE_BufferRead)(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
					  TestStatus (*I2C_EE_BufferWrite)(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
        }Eeprom;
				
        struct Led
        {
            void(*LedModeSet)(LedTypedef led,LedModeTypedef mode);
        }Led;
				
        struct RemoteIR
        {
            void(*RemoteIRCtrl)(FunctionalState state);
        }RemoteIR;
        
        struct Timer
        {
            void (*Start)(int id, TimerModeEnum mode, int times, function registerFunction);
            void (*Stop)(int id); 
        }Timer;

        struct Systick
        {
            bool (*Register)(SystickEnum type, function registerFunction);
        }Systick;

        struct Iwdog
        {
            void (*IwdogStart)(uint delay1ms);
            void (*IwdogReload)(void);
        }Iwdog;
        
    }Device;

    struct OS
    {
        void (* CreateLogicTask)(function taskPointer);
        void (* CreateMenuTask)(function taskPointer);
        void (* Start)(void);
        bool (* PostMessageQueue)(uint message);
        uint (* PendMessageQueue)(void);
        void (* DelayMs)(int times); 
    }OS;

    struct Gui
    {
        void (*Parse)(Form * formPointer);
        void (*LoadForm)(Form * formPointer);

        struct
        {
            void (* AddGifLabel)(Form * formPointer, GifLabel *GifPointer);
            void (* AddFigLabel)(Form * formPointer, FigLabel *labelPointer);
            void (* AddTextBox)(Form * formPointer, TextBox *textBoxPointer);
            void (* AddRectangle)(Form * formPointer, Rectangle *rectanglePointer);
            void (*AddCheckBoxGroup)(Form * formPointer, CheckBoxGroup *CheckBoxGroupPointer);
            void (* Init)(Form *formPointer);
//            void (*ModifyTextBoxData)(KeyActEnum keyAct);
        }Form;
    }Gui;
}SystemStruct;



extern SystemStruct System;

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#endif 

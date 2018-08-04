
/*

********************************************************************************
*文件名     : app.h
********************************************************************************
*/


#ifndef __SYSDEFS_H
#define __SYSDEFS_H


#define MAIN_VERSION  2
#define SUB_VERSION		34

#define __WIFI_TEST
#define __FAULT_DISP
#define __SELF_ADJUSTMOTO

//#define __HAYDN_CTRL
#define __LANGJIE_CTRL
//#define __MAILINSEN_CTRL

//#define __ONLINE_DEBUG
//#define __RPMCTRL_OFF
//#define __DEBUG

#ifdef __ONLINE_DEBUG
  #define __DEBUG
#endif


//#define HD_GJ_350B
//#define HD_GJ_250A
//#define HD_GJ_250B
//#define HD_GJ_350C
//#define HD_GJ_250C
//#define HD_GJ_250T
//#define HD_GJ_150C
//#define HD_GJ_350TEST
#define HD_GJ_800C
//#define HD_GJ_500B
//#define HD_GJ_350A
//#define HD_GJ_500C
//#define HD_GJ_500A
//#define HD_GJ_600Z
//#define HD_GJ_360C
//#define HD_BG_80


typedef enum{
	Capital_A =0,
	Capital_B =1,
	Capital_C =2,
	Capital_D,                     
	Capital_E,
	Capital_F, 
	Capital_G,
	Capital_H,
	Capital_I,
	Capital_J,
	Capital_K,
	Capital_L,
	Capital_M,
	Capital_N,
	Capital_O,
	Capital_P,
	Capital_Q,
	Capital_R,
	Capital_S,
	Capital_T,
	Capital_U,
	Capital_V,
	Capital_W,
	Capital_X,
	Capital_Y,
	Capital_Z
}MachineTypedef;

/*APP机型参数，不能随意修改   V2.26-27 erase HB_80*/
typedef	enum
{
	HB_80 = 0x01,
	HG_150 = 0x02,
	HG_250 = 0x03,
	HG_350 = 0x04,
	HG_500 = 0x05,
	HG_800 = 0x06
}MachineType;

/* __TESTFILTER defined,10s as 1hour,grapdata upload per 30s*/
//#define __TESTFILTER

#endif

/*
********************************************************************************
*�ļ���       : device.c
*����         : �豸�ӿ�
*ԭ��         : ��
********************************************************************************

********************************************************************************
*/



#include "system.h"

extern void InitUsart1(void);
//extern void InitUsart3(void);


extern void InitRtc(void);

extern void InitKey(void);

extern void InitIRReceiver(void);

extern void InitLcd(void);

extern void InitTimer(void);

extern void InitSystick(void);

extern void InitSPI1(void);

extern void InitLed(void);

extern void InitWifiUsart2(void);

extern void InitE2prom(void);

extern void IwdogInit(void);

/*******************************************************************************
* ����	    : ��ʼ���豸
*******************************************************************************/
void InitDevice(void)
{    
    InitUsart1();	

    InitSPI1();

    InitRtc();

    InitKey();
	
		InitIRReceiver();	
	
		InitLed();

    InitTimer();
	
	InitE2prom();

    InitLcd();
	
	InitWifiUsart2();
	
	IwdogInit();

    InitSystick();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     // ��ռ���ȼ�|����ռ���ȼ�,0Ϊ��
                                                        // bit3 bit2 | bit1 bit0 
                                                        //  0 ~ 3    |    0 ~ 3 
	NVIC_SetPriority(SysTick_IRQn, 0x0E);               //    3      |      2
 	NVIC_SetPriority(PendSV_IRQn, 0x0F);                //    3      |      3
}



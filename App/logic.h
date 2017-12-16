/*
********************************************************************************
*文件名     : logic.h
*作用       : 
*原理       : 
********************************************************************************

********************************************************************************
*/



#ifndef __LOGIC_H
#define __LOGIC_H



#define CO2_WARNING_TIMER	1
#define CO2_WARNING_MS	300000 //5MIN

#define 	SEQUENCEOPER_DELAY_100MS		5

extern uint8_t SeqOperDelay;

extern void LogicTask(void);

#endif 


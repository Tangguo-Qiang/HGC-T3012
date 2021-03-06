/*
********************************************************************************
*文件名     : device_systick.c
*作用       : 系统节拍
*原理       : 
********************************************************************************
*/



#include "drive.h"
#include "system.h"

extern void KeySystick100Routine(void);

extern void KeyRollSystick1000Routine(void);

extern void IRSystick1000Routine(void);

extern void TimerSystick1000Routine(void);

extern void Usart1TransSystick1000Routine(void);
extern void Usart1RecvSystick1000Routine(void); 

extern void LedSystick10Routine(void); 

extern void Usart2TransSystick1000Routine(void); 
extern void Usart2RecvSystick1000Routine(void); 

extern void SysTick_Handler(void);

//#define Systicks 10000     // 每秒种产生中断的次数，也就是节拍数
#define Systicks 1000     // 每秒种产生中断的次数，也就是节拍数

#define Systick10000Sum 2
#define Systick1000Sum  2
#define Systick100Sum   5
#define Systick10Sum   5

//static function Systick10000RegisterPointerBlock[Systick10000Sum] =
//{
//    Dummy, Dummy
//};

static function Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, 
};

static function Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, 
};

static function Systick10RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, 
};

/*******************************************************************************
* 描述	    : 系统节拍注册函数，用于注册回调函数到不同速度的系统节拍数组中。
* 输入参数  : systemTick 系统节拍速度类型 registerFunction 注册的函数指针
* 返回参数  : bool布尔变量，返回是否成功
*******************************************************************************/
static bool RegisterSystick(SystickEnum type, function registerFunction)
{
    static byte Systick100Counter = 0;
    static byte Systick1000Counter = 0;
//    static byte Systick10000Counter = 0;
    static byte Systick10Counter = 0;

    switch(type)
    {
        case Systick10:
            if (Systick10Counter == Systick10Sum) return(false);
            Systick10RegisterPointerBlock[Systick10Counter++] = registerFunction;  
            return(true);
        case Systick100:
            if (Systick100Counter == Systick100Sum) return(false);
            Systick100RegisterPointerBlock[Systick100Counter++] = registerFunction;  
            return(true);
        case Systick1000:
            if (Systick1000Counter == Systick1000Sum) return(false);
            Systick1000RegisterPointerBlock[Systick1000Counter++] = registerFunction;   
            return(true);
//        case Systick10000:
//            if (Systick10000Counter == Systick10000Sum) return(false);
//            Systick10000RegisterPointerBlock[Systick10000Counter++] = registerFunction;   
//            return(true);
        default:
            return(false);
    }
}

/*******************************************************************************
* 描述	    : 系统节拍中断入口，非常重要，每秒10000次，即0.1mS一次
*           : 系统节拍通过switch细分为每秒1000次，每秒100次的例行节拍
*           : 同层直接调用，高层采用注册调用
*******************************************************************************/
void SysTick_Handler(void)
{
    byte div;
    static byte Counter = 0;
    if(++Counter == 100) Counter = 0;
//    Systick10000RegisterPointerBlock[0]();
//    Systick10000RegisterPointerBlock[1]();
	KeyRollSystick1000Routine();
	IRSystick1000Routine();
	Systick1000RegisterPointerBlock[0]();
	Systick1000RegisterPointerBlock[1]();
	Usart1TransSystick1000Routine();
	Usart1RecvSystick1000Routine();
	Usart2TransSystick1000Routine();
	Usart2RecvSystick1000Routine();
	TimerSystick1000Routine();
   div = Counter / 10;
    switch(Counter % 10)
    {
        case 0: Systick100RegisterPointerBlock[0](); break;
        case 1: Systick100RegisterPointerBlock[1](); break;
        case 2: Systick100RegisterPointerBlock[2](); break;
        case 3: Systick100RegisterPointerBlock[3](); break;
        case 4: Systick100RegisterPointerBlock[4]();      break;
        case 5: KeySystick100Routine();        break;
        case 6:           break;
        case 7:         break;
        case 8: 								break;
        case 9:
            switch(div)
            {
                case 0:  Systick10RegisterPointerBlock[0]();      break;
                case 1:  Systick10RegisterPointerBlock[1]();      break;
                case 2:  Systick10RegisterPointerBlock[2]();      break;
                case 3:  Systick10RegisterPointerBlock[3]();      break;
                case 4:  Systick10RegisterPointerBlock[4]();      break;
                case 5:  GifTimer100ms = 1;      break;
                case 6:  LedSystick10Routine();      break;
                default:                              break;
            }
            break;
    }
//		if(TimeOut1ms)
//			TimeOut1ms--;
}

void InitSystick(void)
{
    SysTick_Config(SystemCoreClock / Systicks);             // 设置节拍速度
    System.Device.Systick.Register = RegisterSystick;       // 关联接口函数指针
}






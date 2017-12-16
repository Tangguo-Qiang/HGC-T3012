/*
********************************************************************************
*�ļ���     : device_systick.c
*����       : ϵͳ����
*ԭ��       : 
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

//#define Systicks 10000     // ÿ���ֲ����жϵĴ�����Ҳ���ǽ�����
#define Systicks 1000     // ÿ���ֲ����жϵĴ�����Ҳ���ǽ�����

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
* ����	    : ϵͳ����ע�ắ��������ע��ص���������ͬ�ٶȵ�ϵͳ���������С�
* �������  : systemTick ϵͳ�����ٶ����� registerFunction ע��ĺ���ָ��
* ���ز���  : bool���������������Ƿ�ɹ�
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
* ����	    : ϵͳ�����ж���ڣ��ǳ���Ҫ��ÿ��10000�Σ���0.1mSһ��
*           : ϵͳ����ͨ��switchϸ��Ϊÿ��1000�Σ�ÿ��100�ε����н���
*           : ͬ��ֱ�ӵ��ã��߲����ע�����
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
    SysTick_Config(SystemCoreClock / Systicks);             // ���ý����ٶ�
    System.Device.Systick.Register = RegisterSystick;       // �����ӿں���ָ��
}






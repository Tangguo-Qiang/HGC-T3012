/*
********************************************************************************
*文件名     : gui.c
*作用       : 图形库文件
*原理       : 面向对象设计，把一个Menu分为多个Form,一个Form包含多个控件，如Checkbox、
*           : Label、TextBox等，建立一个最低优先级任务MenuTask去解析执行这些控件信息
********************************************************************************
*/

#include "system.h"
//#include "Zimo.h"

const FigInfo FigInSys[NUM_Full]={{32,16},	
{66,79},{57,67},{21,21},{55,65}, {66,79},
/*NUM_CIRCLEicon32x16 = 0,	
NUM_CIRCLEmode02_66x79 = 1,
NUM_CIRCLEmode57x67 = 2,	
NUM_CLOCKicon21X21 = 3,
NUM_CO2icon55X65 = 4,	
NUM_HEATmode02_66X79 = 5,	
*/
{57,67},{165,67},{31,67},{55,67},{83,67},{110,67},
/*NUM_HEATmode57X67 = 6,	
NUM_LEAF00_165X67 = 7,
NUM_LEAF01_31X67 = 8,
NUM_LEAF02_55X67 = 9,
NUM_LEAF03_83X67 = 10,
NUM_LEAF04_110X67 = 11,	
*/
{140,67},{165,67},{68,29},{55,65},{38,16},
/*
NUM_LEAF05_140X67 = 12,
NUM_LEAF06_165X67 = 13,
NUM_M3Hicon68X29 = 14,
NUM_PM25icon55X65 = 15,
NUM_ppm38X16 = 16,	
*/
{66,79},{57,67},{25,17},{29,41},{12,12},
/*
NUM_RATEmode02_66x79 = 17,
NUM_RATEmode57X67 = 18,
NUM_RFicon25x17 = 19,
NUM_RHicon29X41 = 20,
NUM_RHunit12X12 = 21,
*/
{143,9},{21,44},{15,13},{53,20},{38,37},	
/*NUM_SHADOWbar143X9 = 22,
NUM_Ticon21X44 = 23,
NUM_Tunit15X13 = 24,
NUM_ugm3_53X20 = 25,
NUM_WHEELicon38X37 = 26,
*/
{26,21},{12,17},{12,17},{10,17},{10,17},
/*NUM_WIFIicon26X21 = 27,
NUM_FigNumBW12x17 = 28,
NUM_FigNumWB12x17 =29,
NUM_Colon10x17=30,
NUM_Slash10x17 =31,
*/
{125,30},{28,44},{28,44},{47,77},{47,77},
/*
NUM_FrameBW125x30 =32,
NUM_FigTimeNumBW28x44 = 33,
NUM_ColonBW28x44 = 34,
NUM_FigVolNum47x77_0 = 35,
NUM_FigVolNum47x77_1 = 36,
*/
{46,22},{46,22},{66,79},{57,67},{57,67},
/*
NUM_FigWeekDayBW46x22 = 37,
NUM_FigWeekDayWB46x22 = 38,
NUM_TimeSet02_66X79 =39,
NUM_HEATmodeNot57X67=40,
NUM_RATEmodeNot57X67=41,
*/
{43,14},{43,14},{43,14},{43,14},{43,14},
/*
NUM_CircleOutChar43x14 =42,
NUM_CircleInChar43x14 =43,
NUM_AutoChar43x14 = 44,
NUM_CloseChar43x14 = 45,
NUM_OpenChar43x14 = 46,
*/
{43,14},{43,14},{43,14},{43,14},{67,30},
/*
NUM_Rate1vs06_43x14=47,
NUM_Rate1vs08_43x14=48,
NUM_Rate1vs10_43x14=49,
NUM_Rate1vs12_43x14=50,
NUM_CircleOutFrame67x30 =51,
*/
{67,30},{67,30},{67,30},{67,30},{67,30},
/*
NUM_CircleOutFrameW67x30 =52,
NUM_CircleInFrame67x30 =53,
NUM_CircleInFrameW67x30 =54,
NUM_AutoFrame67x30 =55,
NUM_AutoFrameW67x30 =56,
*/
{67,30},{67,30},{67,30},{67,30},{67,30},
/*
NUM_OpenFrame67x30 =57,
NUM_OpenFrameW67x30 = 58,
NUM_Rate1vs06Frame67x30=59,
NUM_Rate1vs06FrameW67x30 =60,
NUM_Rate1vs08Frame67x30 =61,
*/
{67,30},{67,30},{67,30},{67,30},{67,30},
/*
NUM_Rate1vs08FrameW67x30 =62,
NUM_Rate1vs10Frame67x30 =63,
NUM_Rate1vs10FrameW67x30 =64,
NUM_Rate1vs12Frame67x30 =65,
NUM_Rate1vs12FrameW67x30 =66,
*/
{67,30},{67,30},{480,34},{480,34},{480,34},
/*
NUM_CloseFrame67x30=67,
NUM_CloseFrameW67x30=68,
NUM_TitleRunSet480x34=69,
NUM_TitleTimerSet480x34=70,
NUM_TitleFilterSet480x34=71,
*/
{480,34},{480,34},{28,28},{28,28},{28,28},{345,23},
/*
NUM_TitleParaSet480x34=72,
NUM_TitleFault480x34=72,
NUM_CheckOn28x28=73,
NUM_CheckBox28x28=74,
NUM_CheckBoxFocus28x28=75,
NUM_TimerTitle345x23=76,
*/
{14,13},{14,19},{73,18},{73,18},{73,18},
/*
NUM_Dot14x13=77,
NUM_HChar14x19= 78,
NUM_MidFilterChar73x18= 79,
NUM_FirstFilterChar73x18= 80,
NUM_EPSChar73x18= 81,
*/
{73,18},{43,24},{43,24},{55,19},{151,24},
/*
NUM_HPChar73x18= 82,
NUM_ResetCharBW43x24= 83,
NUM_ResetCharWB43x24= 84,
NUM_UsedChar55x19= 85,
NUM_ExamCleanChar151x24= 86,
*/
{151,24},{229,15},{229,15},{229,15},{229,15},
/*
NUM_ExamChangeChar151x24= 87,
NUM_Progress0Bar229x15= 88,
NUM_Progress1Bar229x15= 89,
NUM_Progress2Bar229x15= 90,
NUM_Progress3Bar229x15= 91,
*/
{229,15},{229,15},{229,15},{229,15},{229,15},
/*
NUM_Progress4Bar229x15= 92,
NUM_Progress5Bar229x15= 93,
NUM_Progress6Bar229x15= 94,
NUM_Progress7Bar229x15= 95,
NUM_Progress8Bar229x15= 96,
*/
{229,15},{229,15},{105,28},{105,28},{105,28},
/*NUM_Progress9Bar229x15= 97,
NUM_Progress10Bar229x15= 98,
NUM_Co2SetChar105x28= 99,
NUM_PMSetChar105x28= 100,
NUM_TempSetChar105x28= 101,
*/
{53,27},{53,27},{53,27},{53,27},{53,27},{53,27},
/*NUM_Ppm800Frame53x27= 102,
NUM_Ppm800FrameW53x27= 103,
NUM_Ppm1000Frame53x27= 104,
NUM_Ppm1000FrameW53x27= 105,
NUM_Ppm1200Frame53x27= 106,
NUM_Ppm1200FrameW53x27= 107,
*/
{44,27},{44,27},{44,27},{44,27},{44,27},{44,27},{44,27},{44,27},
/*
NUM_PM100Frame44x27= 108,
NUM_PM100FrameW44x27= 109,
NUM_PM150Frame44x27= 110,
NUM_PM150FrameW44x27= 111,
NUM_PM200Frame44x27= 112,
NUM_PM200FrameW44x27= 113,
NUM_PM250Frame44x27= 114,
NUM_PM250FrameW44x27= 115,
*/
{53,27},{53,27},{53,27},{53,27},{53,27},
{53,27},{53,27},{53,27},{53,27},{53,27},
{53,27},{53,27},{53,27},{53,27},{53,27},
{53,27},{53,27},{53,27},{53,27},{53,27},
/*
NUM_Temp14Frame53x27= 116,
NUM_Temp14FrameW53x27= 117,
NUM_Temp15Frame53x27= 118,
NUM_Temp15FrameW53x27= 119,
NUM_Temp16Frame53x27= 120,
NUM_Temp16FrameW53x27= 121,
NUM_Temp17Frame53x27= 122,
NUM_Temp17FrameW53x27= 123,
NUM_Temp18Frame53x27= 124,
NUM_Temp18FrameW53x27= 125,
NUM_Temp25Frame53x27= 126,
NUM_Temp25FrameW53x27= 127,
NUM_Temp26Frame53x27= 128,
NUM_Temp26FrameW53x27= 129,
NUM_Temp27Frame53x27= 130,
NUM_Temp27FrameW53x27= 131,
NUM_Temp28Frame53x27= 132,
NUM_Temp28FrameW53x27= 133,
NUM_Temp29Frame53x27= 134,
NUM_Temp29FrameW53x27= 135,
*/
{46,28},{46,28},{16,23},{16,23},{16,23},
/*NUM_WinterChar46x28= 136,
NUM_SummerChar46x28= 137,
NUM_FigNumBRed16x23=138,
NUM_FigNumBOrange16x23=139,
NUM_FigNumBGreen16x23=140,
*/
{30,28},{15,21},{33,29},{73,58},{60,57},
/*NUM_CleanIcon30x28=141,
NUM_LockIcon15x21=142,
NUM_FaultIcon33x29=143,
NUM_AlertIcon73x58=144,
NUM_FilterAlertIcon60x57=145,
*/
{444,71},{430,47},{444,69},{17,17},{10,17},
/*NUM_CoreAlertText444x71=146,
NUM_FilterAlertText430x47=147,
NUM_NormalAlerText444x69=148,
NUM_Capital12x17=150,
NUM_Point10x17=151,

*/
};

//const CharInfo CharInSys[17] ={{Num8x16,8,16},{Num12x24,12,24},{Num16x32,16,32},
//                               {Num24x48,24,48},{setpara16x16,16,16},{systimeTitle16x16,16,16},
//                               {WeekTitle24x24,24,24},{WorktimeTitle24x24,24,24},{FilterTitle24x24,24,24},
//                               {TimerTitle16x16,16,16},{PageTitle32x32,32,32},{RateParaTitle8x16,8,16},
//															 {heattype16x16,16,16},{bracket8x16,8,16},{NumL56x85,56,85},{formtitle24x24,24,24},{formtitle24x24,32,32}};

static Form * FocusFormPointer;

static byte TextFlash=0;
static byte FigFlash=0;
static byte GifFlash100ms=0;



static void RectangleToGuiLcd(Rectangle * RectanglePointer)
{
	LCD_LineTypeDef line;
	LCD_RectangleTypeDef rectangle;
    
	if(RectanglePointer->Disp != Invisible)
	{
		if(RectanglePointer->Solid == false)
		{
			line.col = RectanglePointer->Coord.X;
			line.row = RectanglePointer->Coord.Y;
			line.bold = RectanglePointer->Bold;
			line.fontcolor = RectanglePointer->FontColor;
			
			if(RectanglePointer->Width>0)
			{
				line.length = RectanglePointer->Width;
				System.Device.Lcd.LCD_RowLine(&line);
				if(RectanglePointer->Height>0)
				{
					line.row = RectanglePointer->Coord.Y + RectanglePointer->Height;
					System.Device.Lcd.LCD_RowLine(&line);
				}
			}

			if(RectanglePointer->Height>0)
			{
				line.length = RectanglePointer->Height;
				line.row = RectanglePointer->Coord.Y;
				System.Device.Lcd.LCD_ColLine(&line);
				if(RectanglePointer->Width>0)
				{
					line.col = RectanglePointer->Coord.X + RectanglePointer->Width;
					System.Device.Lcd.LCD_ColLine(&line);
				}
			}
		}
		else
		{
			rectangle.col = RectanglePointer->Coord.X;
			rectangle.row = RectanglePointer->Coord.Y;
			rectangle.width = RectanglePointer->Width;
			rectangle.Height = RectanglePointer->Height;
			rectangle.color = RectanglePointer->FontColor;
			System.Device.Lcd.LCD_ColorRectangle(&rectangle);
		}
	}
    
}

static void CheckBoxGroupToGuiLcd(CheckBoxGroup * CheckBoxGroupPointer)
{
    LCD_FigFlashTypeDef figlabel;
	byte i=0;
    
	if(CheckBoxGroupPointer->Group != 0)
	{
		figlabel.col = CheckBoxGroupPointer->Coord.X;
		figlabel.row  = CheckBoxGroupPointer->Coord.Y;
		figlabel.wide = FigInSys[NUM_CheckBox28x28].Wide ;
		figlabel.heigh =FigInSys[NUM_CheckBox28x28].Height;
		for(i=1;i<(CheckBoxGroupPointer->Group+1);i++)
		{
			if(i==CheckBoxGroupPointer->FocuseOn)
				figlabel.figloc =NUM_CheckBoxFocus28x28;
			else if(i==(*CheckBoxGroupPointer->CheckOn))
				figlabel.figloc =NUM_CheckOn28x28;
			else
				figlabel.figloc =NUM_CheckBox28x28;
			System.Device.Lcd.LCD_FigureInFlash(&figlabel);
			if(CheckBoxGroupPointer->PitchType == HoriGroup)
			{
				figlabel.col += figlabel.wide ;
				figlabel.col += CheckBoxGroupPointer->Pitch ;
			}
			else
			{
				figlabel.row += figlabel.heigh ;
				figlabel.row += CheckBoxGroupPointer->Pitch ;
			}
		}
	}
    
}

static void FigLabelToGuiLcd(FigLabel * labelPointer)
{
    LCD_FigFlashTypeDef figlabel;
	
		if(FigFlash>0x08)
			FigFlash =0;

		switch(labelPointer->Disp)
		{
			case NormalDisp:
				figlabel.figloc = labelPointer->FigNum;
				labelPointer->UpdataFlag = false ;    
			break;
			case InverseDisp:
				figlabel.figloc = labelPointer->FigNum+1;
				labelPointer->UpdataFlag = false ;    
			break;
			case FlashDisp:
				labelPointer->UpdataFlag = true ;	
				if(FigFlash==0x04)
					figlabel.figloc = labelPointer->FigNum;
				else if(!FigFlash)
					figlabel.figloc = labelPointer->FigNum+1;
			break;
			case InvisibleFlashDisp:
				labelPointer->UpdataFlag = true ;	
				figlabel.figloc = labelPointer->FigNum;
			break;
			default:
				labelPointer->UpdataFlag = false ;  
				break;
		}
		
	figlabel.col = labelPointer->Coord.X;
	figlabel.row  = labelPointer->Coord.Y;
	figlabel.wide = FigInSys[labelPointer->FigNum].Wide ;
	figlabel.heigh =FigInSys[labelPointer->FigNum].Height;
	if(labelPointer->Disp < InvisibleFlashDisp)
	{
		System.Device.Lcd.LCD_FigureInFlash(&figlabel);
	}
 	else
	{
		if((labelPointer->Disp==Invisible)||(!FigFlash))
		{
			System.Device.Lcd.LCD_FigNuLL(&figlabel);
		}
		else  if(FigFlash==0x08)
				System.Device.Lcd.LCD_FigureInFlash(&figlabel);
	}
   
}

static void GifLabelToGuiLcd(GifLabel * GifPointer)
{
    LCD_FigFlashTypeDef figlabel;
	static byte i=0;
		
	if(GifFlash100ms==GifPointer->Speed)
	{
		GifFlash100ms =0;
		figlabel.col = GifPointer->Coord.X;
		figlabel.row  = GifPointer->Coord.Y;
		figlabel.figloc = GifPointer->FigNum+i;
		figlabel.wide = FigInSys[GifPointer->FigNum+i].Wide ;
		figlabel.heigh =FigInSys[GifPointer->FigNum+i].Height;
		System.Device.Lcd.LCD_FigureInFlash(&figlabel);
		if(++i==GifPointer->Frame)
			i=0;
	}

    
}

static void TextBoxToGuiLcd(TextBox * TextBoxPointer)
{
//  LCD_CharTypeDef charNum;
	LCD_FigFlashTypeDef charNum;
	byte i,zero=0;
	signed char val;
	uint uintval;
	
	if(TextFlash>0x0a)
		TextFlash =0;
	if(TextBoxPointer->Disp != Invisible)
	{
		switch(TextBoxPointer->Disp)
		{
			case NormalDisp:
				charNum.figloc = TextBoxPointer->FigNum;
  TextBoxPointer->UpdataFlag = false;  
			break;
			case InverseDisp:
				charNum.figloc = TextBoxPointer->FigNum+1;
  TextBoxPointer->UpdataFlag = false;  
			break;
			case FlashDisp:
				if(TextFlash>0x05)
					charNum.figloc = TextBoxPointer->FigNum;
				else 
					charNum.figloc = TextBoxPointer->FigNum+1;
				TextBoxPointer->UpdataFlag = true;
			break;
			default:
				break;
		}			
			
		charNum.col = TextBoxPointer->Coord.X;
		charNum.row  = TextBoxPointer->Coord.Y;
		charNum.wide = FigInSys[TextBoxPointer->FigNum].Wide;
		charNum.heigh = FigInSys[TextBoxPointer->FigNum].Height;
		
		switch(TextBoxPointer->Type)
		{
			case GuiDataTypeString:		
				val = (*(byte*)TextBoxPointer->DataPointer);
//				val %= 10;
//			if(val>10)
//				val = 10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataType10sString:		
				val = (*(byte*)TextBoxPointer->DataPointer);
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				charNum.col += charNum.wide;
				val = (*((byte*)TextBoxPointer->DataPointer+1));
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeUnSignedNDec:
				val = (*(char*)TextBoxPointer->DataPointer);
				val %= 10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeSigned10sDec:
				val = (*(char*)TextBoxPointer->DataPointer);
			  if(val<0)
				{
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,10);
					charNum.col += charNum.wide;
					val *= -1;
				}
				else
				{
					System.Device.Lcd.LCD_FigNuLL(&charNum);
					charNum.col += charNum.wide*2;
					System.Device.Lcd.LCD_FigNuLL(&charNum);
					charNum.col -= charNum.wide*2;
					charNum.col += charNum.wide/2;
				}
					
				i= val/10;
				val %= 10;
				if(i)
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,i);
				else
					System.Device.Lcd.LCD_FigNuLL(&charNum);
				charNum.col += charNum.wide;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeUnSigned10sDec:
				val = (*(char*)TextBoxPointer->DataPointer);
				i= val/10;
				val %= 10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,i);
				charNum.col += charNum.wide;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeUnSigned100sDec:
				uintval = (*(ushort*)TextBoxPointer->DataPointer);
				val= uintval/100;
				uintval %= 100;
			  if(val)
				{
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					zero=1;
				}
				else
					System.Device.Lcd.LCD_FigNuLL(&charNum);
				charNum.col += charNum.wide;
				val= uintval/10;
				if(val||zero)
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				else
					System.Device.Lcd.LCD_FigNuLL(&charNum);
				charNum.col += charNum.wide;
				val= uintval%10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeUnSigned1000sDec:
				uintval = (*(ushort*)TextBoxPointer->DataPointer);
				val= uintval/1000;
				uintval %= 1000;
			  if(TextBoxPointer->Initial)
				{
					if(val)
					{
						System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
						zero =1;
					}
					else
						System.Device.Lcd.LCD_FigNuLL(&charNum);
					charNum.col += charNum.wide;
					val= uintval/100;
					uintval %= 100;
					if(val||zero)
					{
						zero =1;
						System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					}
					else
						System.Device.Lcd.LCD_FigNuLL(&charNum);
					charNum.col += charNum.wide;
					val= uintval/10;
					if(val||zero)
						System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					else
						System.Device.Lcd.LCD_FigNuLL(&charNum);
					charNum.col += charNum.wide;
					val= uintval%10;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				}
				else
				{
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					charNum.col += charNum.wide;
					val= uintval/100;
					uintval %= 100;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					charNum.col += charNum.wide;
					val= uintval/10;
						System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					charNum.col += charNum.wide;
					val= uintval%10;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				}
				break;
			case GuiDataTypeUnSigned10000sDec:
				uintval = (*(uint*)TextBoxPointer->DataPointer);
				val= uintval/10000;
				uintval %= 10000;
			  if(val)
				{
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					charNum.col += charNum.wide;
				}
				else
					System.Device.Lcd.LCD_FigNuLL(&charNum);
				val= uintval/1000;
				uintval %= 1000;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				charNum.col += charNum.wide;
				val= uintval/100;
				uintval %= 100;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				charNum.col += charNum.wide;
				val= uintval/10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				charNum.col += charNum.wide;
				val= uintval%10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
			case GuiDataTypeUnSigned100000sDec:
				uintval = (*(uint*)TextBoxPointer->DataPointer);
				val= uintval/100000;
				uintval %= 100000;
			  if(val)
				{
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
					zero =1;
				}
				val= uintval/10000;
				uintval %= 10000;
				if(val||zero)
				{
					zero =1;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				}
				charNum.col += charNum.wide;
				val= uintval/1000;
				uintval %= 1000;
				if(val||zero)
				{
					zero =1;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				}
				charNum.col += charNum.wide;
				val= uintval/100;
				uintval %= 100;
				if(val||zero)
				{
					zero =1;
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				}
				charNum.col += charNum.wide;
				val= uintval/10;
				if(val||zero)
					System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				charNum.col += charNum.wide;
				val= uintval%10;
				System.Device.Lcd.LCD_NumFigInFlash(&charNum,val);
				break;
		
		
			
		}
	}
    
}

/*******************************************************************************
* 描述	    : Rectange控件扫描刷新
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseRectangle(void)
{
	Rectangle * RectanglePointer= null;
    
    RectanglePointer = FocusFormPointer->RectanglePointer;

    while(RectanglePointer != null)
    {
      if(RectanglePointer->UpdataFlag == true)  
			{
				RectangleToGuiLcd(RectanglePointer);
				RectanglePointer->UpdataFlag = false ;
			}
      RectanglePointer = RectanglePointer->NextRectanglePointer;
    }
}

/*******************************************************************************
* 描述	    : CheckBoxGroup控件扫描刷新
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseCheckBoxGroup(void)
{
	CheckBoxGroup * CheckBoxGroupPointer= null;
    
    CheckBoxGroupPointer = FocusFormPointer->CheckBoxGroupPointer;

    while(CheckBoxGroupPointer != null)
    {
      if(CheckBoxGroupPointer->UpdataFlag == true)  
			{
				CheckBoxGroupToGuiLcd(CheckBoxGroupPointer);
				CheckBoxGroupPointer->UpdataFlag = false ;
			}
      CheckBoxGroupPointer = CheckBoxGroupPointer->NextCheckBoxGroupPointer;
    }
}

/*******************************************************************************
* 描述	    : Label控件扫描刷新，Label一般用于文本显示，但不能按键输入操作
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseFigLabel(void)
{
	FigLabel * figLabelPointer= null;
    
    figLabelPointer = FocusFormPointer->FigLabelPointer;

    while(figLabelPointer != null)
    {
      if(figLabelPointer->UpdataFlag == true)  
			{
				FigLabelToGuiLcd(figLabelPointer);
//				figLabelPointer->UpdataFlag = false ;
			}
      figLabelPointer = figLabelPointer->NextLabelPointer;
    }
}

/*******************************************************************************
* 描述	    : GifLabel控件扫描刷新，Gif一般用于动画显示
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseGifLabel(void)
{
	GifLabel * GifPointer= null;
    
    GifPointer = FocusFormPointer->GifPointer;

    while(GifPointer != null)
    {
      if(GifPointer->ActiveFlag == true)  
			{
				GifLabelToGuiLcd(GifPointer);
			}
      GifPointer = GifPointer->NextGifPointer;
    }
}

/*******************************************************************************
* 描述	    : TextBox控件扫描刷新，TextBox不仅用于文本显示，还能按键输入修改参数
* 输入参数  : formPointer 当前窗体指针
*******************************************************************************/
static void ParseTextBox(void)
{
    
    TextBox * textBoxPointer= null;

    textBoxPointer = FocusFormPointer->TextBoxPointer;

    while(textBoxPointer != null)
    {
        
      if(textBoxPointer->UpdataFlag == true)
			{
				TextBoxToGuiLcd((TextBox *)textBoxPointer);
//				textBoxPointer->UpdataFlag = false;
			}
        
      textBoxPointer = textBoxPointer->NextTextBoxPointer;
    }
}

/*******************************************************************************
* 描述	    : 解析页面
*******************************************************************************/
static void LoadForm(Form * formPointer)
{  
	Rectangle * RectanglePointer= null;
	FigLabel * figLabelPointer= null;
	GifLabel * GifPointer= null;
  TextBox * textBoxPointer= null;
  CheckBoxGroup * CheckBoxGroupPointer= null;
    
  FocusFormPointer = formPointer;
  RectanglePointer = FocusFormPointer->RectanglePointer;
    while(RectanglePointer != null)
    {
      RectanglePointer->UpdataFlag =true;
      RectanglePointer = RectanglePointer->NextRectanglePointer;
    }	
    
    figLabelPointer = FocusFormPointer->FigLabelPointer;
    while(figLabelPointer != null)
    {
      figLabelPointer->UpdataFlag = true; 
      figLabelPointer = figLabelPointer->NextLabelPointer;
    }

    textBoxPointer = FocusFormPointer->TextBoxPointer;
    while(textBoxPointer != null)
    {        
      textBoxPointer->UpdataFlag = true;
      textBoxPointer = textBoxPointer->NextTextBoxPointer;
    }

    CheckBoxGroupPointer = FocusFormPointer->CheckBoxGroupPointer;
    while(CheckBoxGroupPointer != null)
    {        
      CheckBoxGroupPointer->UpdataFlag = true;
			CheckBoxGroupPointer->FocuseOn=0;
      CheckBoxGroupPointer = CheckBoxGroupPointer->NextCheckBoxGroupPointer;
    }

    GifPointer = FocusFormPointer->GifPointer;
    while(GifPointer != null)
    {        
      GifPointer->ActiveFlag = true;
      GifPointer = GifPointer->NextGifPointer;
    }
		TextFlash=0;
		FigFlash=0;
		GifFlash100ms=0;

//		System.Device.Lcd.LCD_ClearScreen();

}

/*******************************************************************************
* 描述	    : 解析页面
*******************************************************************************/
static void Parse(Form * formPointer)
{  
  FocusFormPointer = formPointer;
    
	if(GifTimer100ms)
	{
		TextFlash++;
		FigFlash++;
		GifFlash100ms++;
		
		GifTimer100ms=0;
	}
  ParseRectangle();             //矩形框扫描解析
  ParseFigLabel();               //标签控件扫描解析
  ParseTextBox();             //文本控件扫描解析
	ParseGifLabel();
	ParseCheckBoxGroup();
	


}

/*******************************************************************************
* 描述	    : 初始化页面
* 输入参数  : formPointer :窗体Form指针
*******************************************************************************/
static void InitForm(Form *formPointer)
{
    formPointer->RefreshFlag = 0;
    formPointer->FigLabelPointer = null;
    formPointer->RectanglePointer = null;
    formPointer->TextBoxPointer = null;
    formPointer->CheckBoxGroupPointer = null;
    formPointer->GifPointer = null;
}

/*******************************************************************************
* 描述	    : 窗体Form加载矩形，加载时清空，加载后，需要对Rectangle初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : rectanglePointer: 控件Rectangle指针
*******************************************************************************/
static void AddRectangle(Form * formPointer, Rectangle *rectanglePointer)
{
   Rectangle *pointer;
	
	rectanglePointer->Width = 0;
    rectanglePointer->Height = 0;
    rectanglePointer->Bold = 0;
        
    rectanglePointer->UpdataFlag = true;
	rectanglePointer->Disp = NormalDisp ;
	
//    rectanglePointer->NextRectanglePointer = formPointer->RectanglePointer;
//    formPointer->RectanglePointer = rectanglePointer;
	
    if (formPointer->RectanglePointer == null)
    {
         formPointer->RectanglePointer = rectanglePointer;
         return;
    }
    
    pointer = formPointer->RectanglePointer;
        
    while (pointer->NextRectanglePointer != null)
        pointer = pointer->NextRectanglePointer;

    pointer->NextRectanglePointer = rectanglePointer;	
}


/*******************************************************************************
* 描述	    : 窗体Form加载Label，加载时清空Label内容，加载后，需要对Label初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : labelPointer: 控件Label指针
*******************************************************************************/
static void AddFigLabel(Form * formPointer, FigLabel *labelPointer)
{
   FigLabel *pointer;
	
	labelPointer->FigNum = NUM_Full;
        
    labelPointer->Disp = NormalDisp;
	labelPointer->UpdataFlag = true;
//    labelPointer->NextLabelPointer = formPointer->FigLabelPointer;
//    formPointer->FigLabelPointer = labelPointer;
    if (formPointer->FigLabelPointer == null)
    {
         formPointer->FigLabelPointer = labelPointer;
         return;
    }
    
    pointer = formPointer->FigLabelPointer;
        
    while (pointer->NextLabelPointer != null)
        pointer = pointer->NextLabelPointer;

    pointer->NextLabelPointer = labelPointer;	
	
}


/*******************************************************************************
* 描述	    : 窗体Form加载Gif
* 输入参数  : formPointer :窗体Form指针
*           : GifPointer: 控件Label指针
*******************************************************************************/
static void AddGifLabel(Form * formPointer, GifLabel *GifPointer)
{
   GifLabel *pointer;
	
	GifPointer->FigNum = NUM_Full;
        
    GifPointer->ActiveFlag = false;
	GifPointer->Speed = 0;
	GifPointer->Frame = 0;
//    labelPointer->NextLabelPointer = formPointer->FigLabelPointer;
//    formPointer->FigLabelPointer = labelPointer;
    if (formPointer->GifPointer == null)
    {
         formPointer->GifPointer = GifPointer;
         return;
    }
    
    pointer = formPointer->GifPointer;
        
    while (pointer->NextGifPointer != null)
        pointer = pointer->NextGifPointer;

    pointer->NextGifPointer = GifPointer;	
	
}

/*******************************************************************************
* 描述	    : 窗体Form加载CheckBoxGroup，加载时清空CheckBoxGroup内容，加载后，需要对CheckBoxGroup初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : CheckBoxGroup: 控件CheckBoxGroup指针
*******************************************************************************/
static void AddCheckBoxGroup(Form * formPointer, CheckBoxGroup *checkBoxGroupPointer)
{
	CheckBoxGroup *pointer;
	
    checkBoxGroupPointer->CheckOn = 0;
	checkBoxGroupPointer->Group=0;
	checkBoxGroupPointer->FocuseOn=0;
	checkBoxGroupPointer->UpdataFlag=true;
//    CheckBoxGroupPointer->NextCheckBoxGroupPointer = formPointer->CheckBoxGroupPointer;
//    formPointer-> CheckBoxGroupPointer= CheckBoxGroupPointer;
    if (formPointer->CheckBoxGroupPointer == null)
    {
         formPointer->CheckBoxGroupPointer = checkBoxGroupPointer;
         return;
    }
    
    pointer = formPointer->CheckBoxGroupPointer;
        
    while (pointer->NextCheckBoxGroupPointer != null)
        pointer = pointer->NextCheckBoxGroupPointer;

    pointer->NextCheckBoxGroupPointer = checkBoxGroupPointer;	

}


/*******************************************************************************
* 描述	    : 窗体Form加载TextBox，加载时清空TextBox内容，加载后，需要对TextBox初始化内容
* 输入参数  : formPointer :窗体Form指针
*           : textBoxPointer: 控件TextBox指针
*******************************************************************************/
static void AddTextBox(Form * formPointer, TextBox *textBoxPointer)
{    
    TextBox * pointer;
    
    textBoxPointer->DataPointer = null;
    
//    textBoxPointer->Digits= 0;
    
    textBoxPointer->FigNum = NUM_Full;
    textBoxPointer->NextTextBoxPointer = null;
	textBoxPointer->UpdataFlag = true;
	textBoxPointer->Disp = NormalDisp;
	textBoxPointer->Initial= InitialNull;

    if (formPointer->TextBoxPointer == null)
    {
         formPointer->TextBoxPointer = textBoxPointer;
         return;
    }
    
    pointer = formPointer->TextBoxPointer;
        
    while (pointer->NextTextBoxPointer != null)
        pointer = pointer->NextTextBoxPointer;

    pointer->NextTextBoxPointer = textBoxPointer;
}


/*******************************************************************************
* 描述	    : 文本控件数值修改处理修改程序
* 输入参数  : key: AddKey、AddLongKey、SubKey、SubLongKey
*           :      AssistUpKey、AssistUpLongKey、AssistDownKey、AssistDownLongKey

static void ModifyTextBoxData(KeyActEnum keyAct)
{
    TextBox * textBoxPointer;
    void * dataPointer;
    
    
    int intData;
    int intDataMax;
    int intDataMin;
    int intDataStep;
    
    DataType dataType;

    textBoxPointer = FocusFormPointer->FocusTextBoxPointer;
    if (textBoxPointer == null) return;
    
    dataPointer = textBoxPointer->DataPointer;

    dataType = textBoxPointer->Type;
    
    switch(dataType)
    {
        case GuiDataTypeSigned10sDec:
            break;
        case GuiDataTypeUnSigned10sDec:
					intData =(int)( * (byte *)(textBoxPointer->DataPointer));
          intDataMax = (int)(textBoxPointer->DataMax);
          intDataMin = (int)(textBoxPointer->DataMin);
          intDataStep = (int)(textBoxPointer->DataStep);        
           switch(keyAct)
            {
                case KeyRollCW:
                    if(intData + intDataStep > intDataMax)
                        intData = intDataMax;
                    else
                        intData += intDataStep;
                    break;
                   case KeyRollCCW:
                    if(intData - intDataStep < intDataMin)
                        intData = intDataMin;
                    else
                        intData -= intDataStep;
                    break;
            }
            *(byte *)dataPointer = (byte)intData;        break;
        case GuiDataTypeUnSigned100sDec:
            break;
        case GuiDataTypeUnSigned1000sDec:
					intData =(int)( * (ushort *)(textBoxPointer->DataPointer));
          intDataMax = (int)(textBoxPointer->DataMax);
          intDataMin = (int)(textBoxPointer->DataMin);
          intDataStep = (int)(textBoxPointer->DataStep);        
           switch(keyAct)
            {
                case KeyRollCW:
                    if(intData + intDataStep > intDataMax)
                        intData = intDataMax;
                    else
                        intData += intDataStep;
                    break;
                   case KeyRollCCW:
                    if(intData - intDataStep < intDataMin)
                        intData = intDataMin;
                    else
                        intData -= intDataStep;
                    break;
            }
            *(uint *)dataPointer = (uint)intData;  
            break;
        case GuiDataTypeUnSigned10000sDec:
        case GuiDataTypeUnSigned100000sDec:
        case GuiDataTypeString:

            break;

        default:
            break;
    }

}
*******************************************************************************/
void InitGui(void)
{
    
    System.Gui.Parse = Parse;
    System.Gui.LoadForm = LoadForm;
    
    System.Gui.Form.AddRectangle = AddRectangle;
    System.Gui.Form.AddFigLabel = AddFigLabel;

    System.Gui.Form.AddTextBox = AddTextBox;
    System.Gui.Form.AddGifLabel = AddGifLabel;
	

    System.Gui.Form.Init = InitForm;    

//    System.Gui.Form.SwitchTextBoxFocus = SwitchTextBoxFocus;

	System.Gui.Form.AddCheckBoxGroup = AddCheckBoxGroup;
}




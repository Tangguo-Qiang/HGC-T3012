/*
********************************************************************************
*文件名     : gui.h
*作用       : 
*原理       : 

********************************************************************************
*/



#ifndef __GUI_H
#define __GUI_H


typedef enum{
NUM_CIRCLEicon32x16 = 0,	
NUM_CIRCLEmode02_66x79 = 1,
NUM_CIRCLEmode57x67 = 2,	
NUM_CLOCKicon21X21 = 3,
NUM_CO2icon55X65 = 4,	
NUM_HEATmode02_66X79 = 5,	
NUM_HEATmode57X67 = 6,	
NUM_LEAF00_165X67 = 7,
NUM_LEAF01_31X67 = 8,
NUM_LEAF02_55X67 = 9,
NUM_LEAF03_83X67 = 10,
NUM_LEAF04_110X67 = 11,	
NUM_LEAF05_140X67 = 12,
NUM_LEAF06_165X67 = 13,
NUM_M3Hicon68X29 = 14,
NUM_PM25icon55X65 = 15,
NUM_ppm38X16 = 16,	
NUM_RATEmode02_66x79 = 17,
NUM_RATEmode57X67 = 18,
NUM_RFicon25x17 = 19,
NUM_RHicon29X41 = 20,
NUM_RHunit12X12 = 21,
NUM_SHADOWbar143X9 = 22,
NUM_Ticon21X44 = 23,
NUM_Tunit15X13 = 24,
NUM_ugm3_53X20 = 25,
NUM_WHEELicon38X37 = 26,
NUM_WIFIicon26X21 = 27,

NUM_FigNumBW12x17 = 28,
NUM_FigNumWB12x17 =29,
NUM_Colon10x17=30,
NUM_Slash10x17 =31,
NUM_FrameBW125x30 =32,
NUM_FigTimeNumBW28x44 = 33,
NUM_ColonBW28x44 = 34,

NUM_FigVolNum47x77_0 = 35,
NUM_FigVolNum47x77_1 = 36,
NUM_FigWeekDayBW46x22 = 37,
NUM_FigWeekDayWB46x22 = 38,
NUM_TimeSet02_66X79 =39,
NUM_HEATmodeNot57X67=40,
NUM_RATEmodeNot57X67=41,
NUM_CircleOutChar43x14 =42,
NUM_CircleInChar43x14 =43,
NUM_AutoChar43x14 = 44,
NUM_CloseChar43x14 = 45,
NUM_OpenChar43x14 = 46,
NUM_Rate1vs06_43x14=47,
NUM_Rate1vs08_43x14=48,
NUM_Rate1vs10_43x14=49,
NUM_Rate1vs12_43x14=50,
NUM_CircleOutFrame67x30 =51,
NUM_CircleOutFrameW67x30 =52,
NUM_CircleInFrame67x30 =53,
NUM_CircleInFrameW67x30 =54,
NUM_AutoFrame67x30 =55,
NUM_AutoFrameW67x30 =56,
NUM_OpenFrame67x30 =57,
NUM_OpenFrameW67x30 = 58,
NUM_Rate1vs06Frame67x30=59,
NUM_Rate1vs06FrameW67x30 =60,
NUM_Rate1vs08Frame67x30 =61,
NUM_Rate1vs08FrameW67x30 =62,
NUM_Rate1vs10Frame67x30 =63,
NUM_Rate1vs10FrameW67x30 =64,
NUM_Rate1vs12Frame67x30 =65,
NUM_Rate1vs12FrameW67x30 =66,

NUM_CloseFrame67x30=67,
NUM_CloseFrameW67x30=68,
NUM_TitleRunSet480x34=69,
NUM_TitleTimerSet480x34=70,
NUM_TitleFilterSet480x34=71, 
NUM_TitleParaSet480x34=72,
NUM_TitleFault480x34=73,
NUM_CheckOn28x28=74,
NUM_CheckBox28x28=75,
NUM_CheckBoxFocus28x28=76,
NUM_TimerTitle345x23=77,
NUM_Dot14x13=78,
NUM_HChar14x19= 79,
NUM_MidFilterChar73x18= 80,
NUM_FirstFilterChar73x18= 81,
NUM_EPSChar73x18= 82,
NUM_HPChar73x18= 83,
NUM_ResetCharBW43x24= 84,
NUM_ResetCharWB43x24= 85,
NUM_UsedChar55x19= 86,
NUM_ExamCleanChar151x24= 87,
NUM_ExamChangeChar151x24= 88,
NUM_Progress0Bar229x15= 89,
NUM_Progress1Bar229x15= 90,
NUM_Progress2Bar229x15= 91,
NUM_Progress3Bar229x15= 92,
NUM_Progress4Bar229x15= 93,
NUM_Progress5Bar229x15= 94,
NUM_Progress6Bar229x15= 95,
NUM_Progress7Bar229x15= 96,
NUM_Progress8Bar229x15= 97,
NUM_Progress9Bar229x15= 98,
NUM_Progress10Bar229x15= 99,

NUM_Co2SetChar105x28= 100,
NUM_PMSetChar105x28= 101,
NUM_TempSetChar105x28= 102,
NUM_Ppm800Frame53x27= 103,
NUM_Ppm800FrameW53x27= 104,
NUM_Ppm1000Frame53x27= 105,
NUM_Ppm1000FrameW53x27= 106,
NUM_Ppm1200Frame53x27= 107,
NUM_Ppm1200FrameW53x27= 108,
NUM_PM100Frame44x27= 109,
NUM_PM100FrameW44x27= 110,
NUM_PM150Frame44x27= 111,
NUM_PM150FrameW44x27= 112,
NUM_PM200Frame44x27= 113,
NUM_PM200FrameW44x27= 114,
NUM_PM250Frame44x27= 115,
NUM_PM250FrameW44x27= 116,
NUM_Temp14Frame53x27= 117,
NUM_Temp14FrameW53x27= 118,
NUM_Temp15Frame53x27= 119,
NUM_Temp15FrameW53x27= 120,
NUM_Temp16Frame53x27= 121,
NUM_Temp16FrameW53x27= 122,
NUM_Temp17Frame53x27= 123,
NUM_Temp17FrameW53x27= 124,
NUM_Temp18Frame53x27= 125,
NUM_Temp18FrameW53x27= 126,
NUM_Temp25Frame53x27= 127,
NUM_Temp25FrameW53x27= 128,
NUM_Temp26Frame53x27= 129,
NUM_Temp26FrameW53x27= 130,
NUM_Temp27Frame53x27= 131,
NUM_Temp27FrameW53x27= 132,
NUM_Temp28Frame53x27= 133,
NUM_Temp28FrameW53x27= 134,
NUM_Temp29Frame53x27= 135,
NUM_Temp29FrameW53x27= 136,
NUM_WinterChar46x28= 137,
NUM_SummerChar46x28= 138,

NUM_FigNumBRed16x23=139,
NUM_FigNumBOrange16x23=140,
NUM_FigNumBGreen16x23=141,

NUM_CleanIcon30x28=142,
NUM_LockIcon15x21=143,
NUM_FaultIcon33x29=144,
NUM_AlertIcon73x58=145,
NUM_FilterAlertIcon60x57=146,
NUM_CoreAlertText444x71=147,
NUM_FilterAlertText430x47=148,
NUM_NormalAlerText444x69=149,

NUM_Capital17x17=150,
NUM_Point10x17=151,
NUM_Full=152

}FigList;

//typedef enum{
//	NUM_Num8x16 = 0,
//	NUM_Num12x24 = 1,
//	NUM_Num16x32 = 2,
//	NUM_Num24x48 = 3,
//	NUM_setpara16x16 = 4,
//	NUM_systimeTitle16x16 = 5,
//	NUM_WeekTitle24x24 = 6,
//	NUM_WorktimeTitle24x24 = 7,
//	NUM_FilterTitle24x24 = 8,
//	NUM_TimerTitle16x16 = 9,
//	NUM_PageTitle32x32 = 10,
//	NUM_RateParaTitle8x16 = 11,
//	NUM_heattype16x16 = 12,
//	NUM_bracket8x16 = 13,
//	NUM_NumL56x85 = 14,
//	NUM_formtitle24x24 = 15,
//	NUM_formtitle32x32 = 16,
//	NUM_Null = 17

//}CharList;

typedef enum 
{
    GuiDataTypeUnSignedNDec,
    GuiDataTypeSigned10sDec,
    GuiDataTypeUnSigned10sDec,
    GuiDataTypeUnSigned100sDec,
    GuiDataTypeUnSigned1000sDec,
    GuiDataTypeUnSigned10000sDec,
    GuiDataTypeUnSigned100000sDec,
//    GuiDataTypeUintHex,
    GuiDataTypeString,
    GuiDataType10sString,
//    GuiDataTypeFigure
}DataType;

typedef enum 
{
  HoriGroup,
	VertiGroup
}CheckBoxPitchType;

typedef enum 
{
   NormalDisp=(byte)0,
   InverseDisp=(byte)1,
	 FlashDisp=(byte)2,
	 InvisibleFlashDisp=(byte)3,
   Invisible=(byte)4
}DispType;

typedef enum
{
	GuiDataAlignLeft,
	GuiDataAlignRight
}DataAlign;

typedef enum
{
	InitialZero=(byte)0,
	InitialNull=(byte)1
}InitialType;

typedef struct
{
    ushort Wide;
    ushort Height;
}FigInfo;

typedef struct
{
    ushort X;
    ushort Y;
}Location;

typedef struct
{
	const byte* Addr;
    ushort Wide;
    ushort Height;
}CharInfo;

typedef struct
{
    char Character;
    byte Column[16];
}Chart;

typedef struct RectangleSelf
{
  bool UpdataFlag;  
	DispType Disp;
	bool Solid;
	Location Coord;
	byte Bold;
	ushort Width;
  ushort Height;
	ushort FontColor;
  struct RectangleSelf * NextRectanglePointer;
}Rectangle;

typedef struct FigLabelSelf
{
  bool UpdataFlag;  
	Location Coord;
	DispType Disp;
	FigList FigNum;
  struct FigLabelSelf * NextLabelPointer;
}FigLabel;

typedef struct GifLabelSelf
{
  bool ActiveFlag;  
	Location Coord;
	byte Frame;
	byte Speed;
	FigList FigNum;
  struct GifLabelSelf * NextGifPointer;
}GifLabel;


typedef struct CheckBoxGroupSelf
{
  bool UpdataFlag;  
	Location Coord;
	byte Group;
	byte FocuseOn;
	byte* CheckOn;
	byte Pitch;
	CheckBoxPitchType PitchType;
  struct CheckBoxGroupSelf * NextCheckBoxGroupPointer;
}CheckBoxGroup;


typedef struct TextBoxSelf
{
  bool UpdataFlag;  
	Location Coord;
//	ushort BkColor;
//	ushort FontColor;
	DispType Disp;
	InitialType Initial;
//	byte Pitch;
//  ushort Digits;
//	CharList CharNum;
	FigList FigNum;
  DataType Type;
	
//  int DataMax;
//  int DataMin;
//  int DataStep;
    
  void * DataPointer;
  struct TextBoxSelf * NextTextBoxPointer;

}TextBox;

typedef struct
{
    uint RefreshFlag;
    FigLabel  * FigLabelPointer;
    Rectangle  * RectanglePointer;
    TextBox   * TextBoxPointer;
    CheckBoxGroup   * CheckBoxGroupPointer;
	GifLabel* GifPointer;
	  void (*RefreshDisp)(void);
	  void (*KeyProcess)(KeyActEnum);
}Form;


extern void InitGui(void);
    
#endif 





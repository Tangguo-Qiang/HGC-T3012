/*

********************************************************************************
*文件名     : menu.c
*作用       : 界面设计
*原理       : 例子程序中包含五个窗体Form，每一个窗体可以加载BackText背景文字、
*           : Chart表、Label标签、TextBox文本四类控件，采用Add方法加载之后，
*           : 需要初始化值。
********************************************************************************

********************************************************************************
*/

#include "system.h"
#include "app.h"
#include "menu.h"
#include "hmi.h"
#include "menudef.h"

#ifdef __LANGJIE_CTRL
	#define MANUFACTOR_CAPITALPLUS1	Capital_A
	#define MANUFACTOR_CAPITAL1	Capital_L
	#define MANUFACTOR_CAPITAL2 Capital_J
#endif
#ifdef __MAILINSEN_CTRL
	#define MANUFACTOR_CAPITALPLUS1	Capital_M
	#define MANUFACTOR_CAPITAL1	Capital_L
	#define MANUFACTOR_CAPITAL2 Capital_S
#endif
#ifdef __HAYDN_CTRL
	#define MANUFACTOR_CAPITALPLUS1	Capital_A
	#define MANUFACTOR_CAPITAL1	Capital_H
	#define MANUFACTOR_CAPITAL2 Capital_D
#endif


const byte VersionInfo[5]={Capital_V,MACHINE_TYPE,MANUFACTOR_CAPITAL1,MANUFACTOR_CAPITAL2,MANUFACTOR_CAPITALPLUS1};
	
const Location DotLocArraySysPara[]=
{{SYSPARAFORM_TITLECOL1-14,SYSPARAFORM_TITLEROW1},{SYSPARAFORM_TITLECOL2-14,SYSPARAFORM_TITLEROW1},{SYSPARAFORM_TITLECOL3-14,SYSPARAFORM_TITLEROW1},{SYSPARAFORM_TITLECOL4-14,SYSPARAFORM_TITLEROW1},
{SYSPARAFORM_TITLECOL1-14,SYSPARAFORM_PARAROW1},{SYSPARAFORM_TITLECOL1-14,SYSPARAFORM_PARAROW2},{SYSPARAFORM_TITLECOL1-14,SYSPARAFORM_PARAROW3},
{SYSPARAFORM_TITLECOL2-14,SYSPARAFORM_PARAROW1},{SYSPARAFORM_TITLECOL2-14,SYSPARAFORM_PARAROW2},{SYSPARAFORM_TITLECOL2-14,SYSPARAFORM_PARAROW3},{SYSPARAFORM_TITLECOL2-14,SYSPARAFORM_PARAROW4},
{SYSPARAFORM_TITLECOL3-14,SYSPARAFORM_PARAROW1},{SYSPARAFORM_TITLECOL3-14,SYSPARAFORM_PARAROW2},{SYSPARAFORM_TITLECOL3-14,SYSPARAFORM_PARAROW3},
{SYSPARAFORM_TITLECOL4-14,SYSPARAFORM_PARAROW1},{SYSPARAFORM_TITLECOL4-14,SYSPARAFORM_PARAROW2},
};

const Location DotLocArrayWeekTimer[]=
{{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW1},{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW2},
{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW3},{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW4},
{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW5},{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW6},
{WEEKTIMERFORM_PARACOL1-14,WEEKTIMERFORM_CHECKGROUPROW7},};

const Location DotLocArrayFilter[]=
{{FILTERFORM_TITLECOL1-14,FILTERFORM_TITLEROW1},{FILTERFORM_TITLECOL1-14,FILTERFORM_TITLEROW2},
{FILTERFORM_TITLECOL1-14,FILTERFORM_TITLEROW3},{FILTERFORM_TITLECOL1-14,FILTERFORM_TITLEROW4},
};

const Location DotLocArrayModeCtrl[]=
{
{MODEFORM_TITLECOL-14,MODEFORM_TITLE1ROW},
{MODEFORM_TITLECOL-14,MODEFORM_TITLE2ROW},
{MODEFORM_TITLECOL-14,MODEFORM_TITLE3ROW},

{MODEFORM_TITLE1_PARACOL0+14,MODEFORM_TITLE1_PARAROW-13},
{MODEFORM_TITLE1_PARACOL1+14,MODEFORM_TITLE1_PARAROW-13},
{MODEFORM_TITLE1_PARACOL2+14,MODEFORM_TITLE1_PARAROW-13},
{MODEFORM_TITLE1_PARACOL3+14,MODEFORM_TITLE1_PARAROW-13},

{MODEFORM_TITLE2_PARACOL0+14,MODEFORM_TITLE2_PARA0ROW-13},
{MODEFORM_TITLE2_PARACOL1+14,MODEFORM_TITLE2_PARA0ROW-13},
{MODEFORM_TITLE2_PARACOL2+14,MODEFORM_TITLE2_PARA0ROW-13},
{MODEFORM_TITLE2_PARACOL3+14,MODEFORM_TITLE2_PARA0ROW-13},
{MODEFORM_TITLE2_PARACOL4+14,MODEFORM_TITLE2_PARA0ROW-13},

{MODEFORM_TITLE2_PARACOL0+14,MODEFORM_TITLE2_PARA1ROW+29},
{MODEFORM_TITLE2_PARACOL1+14,MODEFORM_TITLE2_PARA1ROW+29},
{MODEFORM_TITLE2_PARACOL2+14,MODEFORM_TITLE2_PARA1ROW+29},
{MODEFORM_TITLE2_PARACOL3+14,MODEFORM_TITLE2_PARA1ROW+29},
{MODEFORM_TITLE2_PARACOL4+14,MODEFORM_TITLE2_PARA1ROW+29},

{MODEFORM_TITLE3_PARACOL0+14,MODEFORM_TITLE3_PARA0ROW+29},
{MODEFORM_TITLE3_PARACOL1+14,MODEFORM_TITLE3_PARA0ROW+29},
{MODEFORM_TITLE3_PARACOL2+14,MODEFORM_TITLE3_PARA0ROW+29},
};


static	byte Position=0;
static	byte MinuteBuf=0;

/* '/','周',':' */
byte TextFig[11] ={0,1,2,3,4,5,6,7,8,9,10};
// 自检
/*  MainForm */
static Rectangle RowLine;
static Rectangle ColLine;

static FigLabel TempInFigLabel;
static FigLabel TempUnitFigLabel;
static FigLabel RHInFigLabel;
static FigLabel RHUnitFigLabel;
static FigLabel CO2FigLabel;
static FigLabel CO2BarFigLabel;
static FigLabel CO2ppmFigLabel;
static FigLabel PMFigLabel;
static FigLabel PMBarFigLabel;
static FigLabel PMugmFigLabel;
static FigLabel LockIconFigLabel;
static FigLabel RFIconFigLabel;
static FigLabel WIFIIconFigLabel;
static FigLabel TimerModeIconFigLabel;
static FigLabel CircleIconFigLabel;
static FigLabel Unitm3hFigLabel;
static FigLabel WheelIconFigLabel;
static FigLabel CircleModeBmpFigLabel;
static FigLabel ThermalModeBmpFigLabel;
static FigLabel VentilateRateModeBmpFigLabel;
static FigLabel CircleModeFigLabel;
static FigLabel ThermalModeFigLabel;
static FigLabel VentilateRateFigLabel;
static FigLabel RTCYearSlashFigLabel;
static FigLabel RTCMonthSlashFigLabel;
static FigLabel RTCTimeColonFigLabe;

static FigLabel CleanIconFigLabel;
static FigLabel FaultIconFigLabel;


static GifLabel GreenLeafGif;

static TextBox RTCYearTextBox;
static TextBox RTCMonthTextBox;
static TextBox RTCDayTextBox;
static TextBox RTCWeekDayTextBox;
static TextBox RTCHourTextBox;
static TextBox RTCMinuteTextBox;
static TextBox TempInsideTextBox;
static TextBox RHInsideTextBox;
static TextBox CO2InsideTextBox;
static TextBox PMInsideTextBox;
static TextBox FlowVolumeTextBox;
#ifdef __DEBUG
static TextBox XFRpmTextBox;
static TextBox PFRpmTextBox;
#endif

/* SysParaForm */
	
static FigLabel TitleRunSetFigLabel;
static Rectangle DotEreaseSysPara;
static FigLabel DotSysPara; //NUM_Dot14x13

static FigLabel CircleSetFigLabel;
static FigLabel CircleOutSetFigLabel;
static FigLabel CircleInSetFigLabel;
static FigLabel CircleAutoSetFigLabel;

static FigLabel ThermalSetFigLabel;
static FigLabel ThermalOpenSetFigLabel;
static FigLabel ThermalCloseSetFigLabel;
static FigLabel ThermalAutoSetFigLabel;

static FigLabel VentilateSetFigLabel;
static FigLabel VentiRate06SetFigLabel;
static FigLabel VentiRate08SetFigLabel;
static FigLabel VentiRate10SetFigLabel;
static FigLabel VentiRate12SetFigLabel;
static FigLabel TimeYearSlashFigLabel;
static FigLabel TimeMonthSlashFigLabel;
static FigLabel TimeColonFigLabel;
static Rectangle DotEreaseSysPara;
static FigLabel DotSysPara; //NUM_Dot14x13

static FigLabel TimeSetFigLabel;
static FigLabel TimeDateFrameFigLabel;
static FigLabel TimeMinFrameFigLabel;
static TextBox TimeYearTextBox;	
static TextBox TimeMonthTextBox; 
static TextBox TimeDayTextBox ;
static TextBox TimeHourTextBox; 
static TextBox TimeMinuteTextBox;

/* WeekTimerForm */
static FigLabel TitleWeekTimerFigLabel;
static Rectangle DotEreaseWeekTimer;
static FigLabel DotWeekTimer; //NUM_Dot14x13
static FigLabel WeekTimerTableFigLabel;
static TextBox MondayTextBox;
static TextBox TuesdayTextBox;
static TextBox WednesdayTextBox;
static TextBox ThursdayTextBox;
static TextBox FridayTextBox;
static TextBox SaturdayTextBox;
static TextBox SundayTextBox;
static CheckBoxGroup MondayCheckBoxGroup;
static CheckBoxGroup TuesdayCheckBoxGroup;
static CheckBoxGroup WednesdayCheckBoxGroup;
static CheckBoxGroup ThursdayCheckBoxGroup;
static CheckBoxGroup FridayCheckBoxGroup;
static CheckBoxGroup SaturdayCheckBoxGroup;
static CheckBoxGroup SundayCheckBoxGroup;

/*  FiterForm  */
static FigLabel TitleFilterFigLabel;
static Rectangle DotEreaseFilter;
static FigLabel DotFilter; //NUM_Dot14x13

static FigLabel FirstFilterCharFigLabel;
static FigLabel FirstFilterUsedFigLabel;
static FigLabel FirstFilterHFigLabel;
static FigLabel FirstFilterResetCharFigLabel;
static FigLabel FirstFilterCheckCharFigLabel;
static TextBox  FirstFilterBarTextBox;
static TextBox  FirstFilterHourTextBox;
static TextBox  FirstFilterHourLimitTextBox;

static FigLabel MidFilterCharFigLabel;
static FigLabel MidFilterUsedFigLabel;
static FigLabel MidFilterHFigLabel;
static FigLabel MidFilterResetCharFigLabel;
static FigLabel MidFilterCheckCharFigLabel;
static TextBox  MidFilterBarTextBox;
static TextBox  MidFilterHourTextBox;
static TextBox  MidFilterHourLimitTextBox;

static FigLabel ESPFilterCharFigLabel;
static FigLabel ESPFilterUsedFigLabel;
static FigLabel ESPFilterHFigLabel;
static FigLabel ESPFilterResetCharFigLabel;
static FigLabel ESPFilterCheckCharFigLabel;
static TextBox  ESPFilterBarTextBox;
static TextBox  ESPFilterHourTextBox;
static TextBox  ESPFilterHourLimitTextBox;

static FigLabel HPFilterCharFigLabel;
static FigLabel HPFilterUsedFigLabel;
static FigLabel HPFilterHFigLabel;
static FigLabel HPFilterResetCharFigLabel;
static FigLabel HPFilterCheckCharFigLabel;
static TextBox  HPFilterBarTextBox;
static TextBox  HPFilterHourTextBox;
static TextBox  HPFilterHourLimitTextBox;

/*ModeCtrlForm*/
static FigLabel TitleModeCtrlFigLabel;
static Rectangle DotEreaseModeCtrl;
static FigLabel DotModeCtrl; //NUM_Dot14x13

static FigLabel PMSetCharFigLabel;
static FigLabel PMUnitFigLabel;
static FigLabel PMCtrlPara0FigLabel;	
static FigLabel PMCtrlPara1FigLabel;	
static FigLabel PMCtrlPara2FigLabel;	
static FigLabel PMCtrlPara3FigLabel;	

static FigLabel TempSetCharFigLabel;
static FigLabel WinterCharFigLabel;
static FigLabel WinterTemp0FigLabel;		
static FigLabel WinterTemp1FigLabel;		
static FigLabel WinterTemp2FigLabel;		
static FigLabel WinterTemp3FigLabel;		
static FigLabel WinterTemp4FigLabel;		
static FigLabel SummerCharFigLabel;
static FigLabel SummerTemp0FigLabel;
static FigLabel SummerTemp1FigLabel;
static FigLabel SummerTemp2FigLabel;
static FigLabel SummerTemp3FigLabel;
static FigLabel SummerTemp4FigLabel;

static FigLabel Co2SetCharFigLabel;
static FigLabel PpmUnitFigLabel;
static FigLabel Co2Para0FigLabel;
static FigLabel Co2Para1FigLabel;
static FigLabel Co2Para2FigLabel;

#ifdef __MAILINSEN_CTRL
static TextBox ManuPlusCharTextBox;
#endif
static TextBox ManuCharTextBox;
static TextBox VerCharTextBox;
static TextBox CtrlMainVersionTextBox;
static TextBox CtrlSubVersionTextBox;
static TextBox CtrlMachineTextBox;
static FigLabel CtrlPointFigLabel;

static TextBox PowerMainVersionTextBox;
static TextBox PowerSubVersionTextBox;
static FigLabel PowerPointFigLabel;

/*FaultForm*/
static FigLabel TitleFaultFigLabel;
static FigLabel AlertIconFigLabel;
static FigLabel FilterAlertIconFigLabel;
static FigLabel CoreAlertTextLabel;
static FigLabel FilterAlertTextLabel;
static FigLabel NormalAlertTextLabel;
static TextBox  FaultCodeTextBox;

void RefreshMenuMainForm(void);
void RefreshMenuSysParaForm(void);
void RefreshMenuWeekTimerForm(void);
void RefreshMenuFilterForm(void);
void RefreshMenuCtrlForm(void);
void RefreshFaultInfoForm(void);

/*******************************************************************************
* 函数名	: InitializeMmi
* 描述	    : 初始化MMI,加载各类控件到各个窗体中
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitMenu(void)
{

// Main Form
    System.Gui.Form.Init(&App.Menu.MainForm);
	App.Menu.MainForm.RefreshDisp = RefreshMenuMainForm;
	App.Menu.MainForm.KeyProcess = KeyProcessMainForm;
	App.Menu.MainForm.RefreshFlag = MAINFORM_INIT;//&(~FORMSCREEN_NEW);
// SysPara Form
    System.Gui.Form.Init(&App.Menu.SysParaForm);
	App.Menu.SysParaForm.RefreshDisp =RefreshMenuSysParaForm;
	App.Menu.SysParaForm.KeyProcess =KeyProcessSysParaForm;
	
// WeekTimer Form
    System.Gui.Form.Init(&App.Menu.WeekTimerForm);
	App.Menu.WeekTimerForm.RefreshDisp =RefreshMenuWeekTimerForm;
	App.Menu.WeekTimerForm.KeyProcess =KeyProcessWeekTimerForm;
// Filter Form
    System.Gui.Form.Init(&App.Menu.FilterForm);
	App.Menu.FilterForm.RefreshDisp =RefreshMenuFilterForm;
	App.Menu.FilterForm.KeyProcess =KeyProcessFilterForm;
// ModeCtrlLine Form
    System.Gui.Form.Init(&App.Menu.ModeCtrlForm);
	App.Menu.ModeCtrlForm.RefreshDisp =RefreshMenuCtrlForm;
	App.Menu.ModeCtrlForm.KeyProcess =KeyProcessModeCtrlForm;
// FaultInfo Form
    System.Gui.Form.Init(&App.Menu.FaultInfoForm);
	App.Menu.FaultInfoForm.RefreshDisp =RefreshFaultInfoForm;
	App.Menu.FaultInfoForm.KeyProcess =KeyProcessFaultInfoForm;

    System.Gui.Form.AddRectangle(&App.Menu.MainForm, &RowLine);
	RowLine.Coord.X = 20;
	RowLine.Coord.Y = 155;
	RowLine.Bold = 1;
	RowLine.Solid = false;
	RowLine.FontColor = WHITE;
	RowLine.Height = 0;
	RowLine.Width = 201;
	
    System.Gui.Form.AddRectangle(&App.Menu.MainForm, &ColLine);
	ColLine.Coord.X = 223;
	ColLine.Coord.Y = 1;
	ColLine.Bold = 1;
	ColLine.Solid = false;
	ColLine.FontColor = WHITE;
	ColLine.Height = 318;
	ColLine.Width = 0;
	
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &TempInFigLabel);
	TempInFigLabel.Coord.X = 32;
	TempInFigLabel.Coord.Y = 106;
	TempInFigLabel.FigNum = NUM_Ticon21X44;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RHInFigLabel);
	RHInFigLabel.Coord.X = 138;
	RHInFigLabel.Coord.Y = 109;
	RHInFigLabel.FigNum = NUM_RHicon29X41;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &TempUnitFigLabel);
	TempUnitFigLabel.Coord.X = 89;
	TempUnitFigLabel.Coord.Y = 137;
	TempUnitFigLabel.FigNum = NUM_Tunit15X13;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RHUnitFigLabel);
	RHUnitFigLabel.Coord.X = 195;
	RHUnitFigLabel.Coord.Y = 138;
	RHUnitFigLabel.FigNum = NUM_RHunit12X12;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CO2FigLabel);
	CO2FigLabel.Coord.X = 21;
	CO2FigLabel.Coord.Y = 175;
	CO2FigLabel.FigNum = NUM_CO2icon55X65;
	CO2FigLabel.Disp = NormalDisp;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CO2BarFigLabel);
	CO2BarFigLabel.Coord.X = 76;
	CO2BarFigLabel.Coord.Y = 210;
	CO2BarFigLabel.FigNum = NUM_SHADOWbar143X9;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CO2ppmFigLabel);
	CO2ppmFigLabel.Coord.X = 176;
	CO2ppmFigLabel.Coord.Y = 190;
	CO2ppmFigLabel.FigNum = NUM_ppm38X16;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &PMFigLabel);
	PMFigLabel.Coord.X = 21;
	PMFigLabel.Coord.Y = 247;
	PMFigLabel.FigNum = NUM_PM25icon55X65;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &PMBarFigLabel);
	PMBarFigLabel.Coord.X = 76;
	PMBarFigLabel.Coord.Y = 283;
	PMBarFigLabel.FigNum = NUM_SHADOWbar143X9;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &PMugmFigLabel);
	PMugmFigLabel.Coord.X = 166;
	PMugmFigLabel.Coord.Y = 258;
	PMugmFigLabel.FigNum = NUM_ugm3_53X20;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &LockIconFigLabel);
	LockIconFigLabel.Coord.X = 323;
	LockIconFigLabel.Coord.Y = 15;
	LockIconFigLabel.FigNum = NUM_LockIcon15x21;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &FaultIconFigLabel);
	FaultIconFigLabel.Coord.X = 228;
	FaultIconFigLabel.Coord.Y = 11;
	FaultIconFigLabel.FigNum = NUM_FaultIcon33x29;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CleanIconFigLabel);
	CleanIconFigLabel.Coord.X = 265;
	CleanIconFigLabel.Coord.Y = 11;
	CleanIconFigLabel.FigNum = NUM_CleanIcon30x28;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RFIconFigLabel);
	RFIconFigLabel.Coord.X = 353;
	RFIconFigLabel.Coord.Y = 17;
	RFIconFigLabel.FigNum = NUM_RFicon25x17;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &WIFIIconFigLabel);
	WIFIIconFigLabel.Coord.X = 383;
	WIFIIconFigLabel.Coord.Y = 15;
	WIFIIconFigLabel.FigNum = NUM_WIFIicon26X21;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &TimerModeIconFigLabel);
	TimerModeIconFigLabel.Coord.X = 414;
	TimerModeIconFigLabel.Coord.Y = 15;
	TimerModeIconFigLabel.FigNum = NUM_CLOCKicon21X21;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CircleIconFigLabel);
	CircleIconFigLabel.Coord.X = 440;
	CircleIconFigLabel.Coord.Y = 17;
	CircleIconFigLabel.FigNum = NUM_CIRCLEicon32x16;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &Unitm3hFigLabel);
	Unitm3hFigLabel.Coord.X = 410;
	Unitm3hFigLabel.Coord.Y = 104;
	Unitm3hFigLabel.FigNum = NUM_M3Hicon68X29;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &WheelIconFigLabel);
	WheelIconFigLabel.Coord.X = 245;
	WheelIconFigLabel.Coord.Y = 156;
	WheelIconFigLabel.FigNum = NUM_WHEELicon38X37;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CircleModeBmpFigLabel);
	CircleModeBmpFigLabel.Coord.X = 245;
	CircleModeBmpFigLabel.Coord.Y = 219;
	CircleModeBmpFigLabel.FigNum = NUM_CIRCLEmode57x67;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &ThermalModeBmpFigLabel);
	ThermalModeBmpFigLabel.Coord.X = 328;
	ThermalModeBmpFigLabel.Coord.Y = 219;
	ThermalModeBmpFigLabel.FigNum = NUM_HEATmode57X67;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &VentilateRateModeBmpFigLabel);
	VentilateRateModeBmpFigLabel.Coord.X = 410;
	VentilateRateModeBmpFigLabel.Coord.Y = 219;
	VentilateRateModeBmpFigLabel.FigNum = NUM_RATEmode57X67;
	VentilateRateModeBmpFigLabel.Disp = NormalDisp;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &FlowVolumeTextBox);
    FlowVolumeTextBox.DataPointer = (void *)(&AirVolume[App.Data.SysCtrlPara.AirFlowSet]);
    FlowVolumeTextBox.Type = GuiDataTypeUnSigned100sDec;
		FlowVolumeTextBox.FigNum = NUM_FigVolNum47x77_0;
    FlowVolumeTextBox.Coord.X = 269;
    FlowVolumeTextBox.Coord.Y = 54;
	
#ifdef __DEBUG
    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &XFRpmTextBox);
	XFRpmTextBox.Coord.X = 415;
	XFRpmTextBox.Coord.Y = 54;
	XFRpmTextBox.FigNum = NUM_FigNumBW12x17;
	XFRpmTextBox.Type = GuiDataTypeUnSigned1000sDec;
	XFRpmTextBox.Initial = InitialZero;
	XFRpmTextBox.DataPointer = (void*)&App.SysRunStatus.XFmotoRPM;
	
    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &PFRpmTextBox);
	PFRpmTextBox.Coord.X = 415;
	PFRpmTextBox.Coord.Y = 81;
	PFRpmTextBox.FigNum = NUM_FigNumBW12x17;
	PFRpmTextBox.Type = GuiDataTypeUnSigned1000sDec;
	PFRpmTextBox.Initial = InitialZero;
	PFRpmTextBox.DataPointer = (void*)&App.SysRunStatus.PFmotoRPM;
#endif

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCYearTextBox);
    RTCYearTextBox.DataPointer = (void *)(&App.SysTime.Year);
    RTCYearTextBox.Type = GuiDataTypeUnSigned1000sDec;
		RTCYearTextBox.FigNum = NUM_FigNumBW12x17;
    RTCYearTextBox.Coord.X = 31;
    RTCYearTextBox.Coord.Y = 20;
		
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RTCYearSlashFigLabel);
		RTCYearSlashFigLabel.FigNum = NUM_Slash10x17;
    RTCYearSlashFigLabel.Coord.X = 79;
    RTCYearSlashFigLabel.Coord.Y = 20;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCMonthTextBox);
    RTCMonthTextBox.DataPointer = (void *)(&App.SysTime.Mon);
    RTCMonthTextBox.Type = GuiDataTypeUnSigned10sDec;
		RTCMonthTextBox.FigNum = NUM_FigNumBW12x17;
    RTCMonthTextBox.Coord.X = 91;
    RTCMonthTextBox.Coord.Y = 20;
	
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RTCMonthSlashFigLabel);
		RTCMonthSlashFigLabel.FigNum = NUM_Slash10x17;
    RTCMonthSlashFigLabel.Coord.X = 115;
    RTCMonthSlashFigLabel.Coord.Y = 20;
	
    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCDayTextBox);
    RTCDayTextBox.DataPointer = (void *)(&App.SysTime.Mday);
    RTCDayTextBox.Type = GuiDataTypeUnSigned10sDec;
		RTCDayTextBox.FigNum = NUM_FigNumBW12x17;
    RTCDayTextBox.Coord.X = 127;
    RTCDayTextBox.Coord.Y = 20;
	
    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCWeekDayTextBox);
    RTCWeekDayTextBox.DataPointer = (void *)(&App.SysTime.Wday);
    RTCWeekDayTextBox.Type = GuiDataTypeString;
		RTCWeekDayTextBox.FigNum = NUM_FigWeekDayBW46x22;
    RTCWeekDayTextBox.Coord.X = 166;
    RTCWeekDayTextBox.Coord.Y = 20;
		
    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCHourTextBox);
    RTCHourTextBox.DataPointer = (void *)(&App.SysTime.Hour);
    RTCHourTextBox.Type = GuiDataTypeUnSigned10sDec;
		RTCHourTextBox.FigNum = NUM_FigTimeNumBW28x44;
    RTCHourTextBox.Coord.X = 56;
    RTCHourTextBox.Coord.Y = 52;
		
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &RTCTimeColonFigLabe);
		RTCTimeColonFigLabe.FigNum = NUM_ColonBW28x44;
    RTCTimeColonFigLabe.Coord.X = 104;
    RTCTimeColonFigLabe.Coord.Y = 52;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RTCMinuteTextBox);
    RTCMinuteTextBox.DataPointer = (void *)(&App.SysTime.Min);
    RTCMinuteTextBox.Type = GuiDataTypeUnSigned10sDec;
		RTCMinuteTextBox.FigNum = NUM_FigTimeNumBW28x44;
    RTCMinuteTextBox.Coord.X = 128;
    RTCMinuteTextBox.Coord.Y = 52;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &TempInsideTextBox);
    TempInsideTextBox.DataPointer = (void *)(&App.Data.SensorData.TempInside);
    TempInsideTextBox.Type = GuiDataTypeSigned10sDec;
		TempInsideTextBox.FigNum = NUM_FigNumBW12x17;
    TempInsideTextBox.Coord.X = 53;
    TempInsideTextBox.Coord.Y = 133;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &RHInsideTextBox);
    RHInsideTextBox.DataPointer = (void *)(&App.Data.SensorData.RHInside);
    RHInsideTextBox.Type = GuiDataTypeUnSigned10sDec;
		RHInsideTextBox.FigNum = NUM_FigNumBW12x17;
    RHInsideTextBox.Coord.X = 167;
    RHInsideTextBox.Coord.Y = 133;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &CO2InsideTextBox);
    CO2InsideTextBox.DataPointer = (void *)(&App.Data.SensorData.CO2Inside);
    CO2InsideTextBox.Type = GuiDataTypeUnSigned1000sDec;
		CO2InsideTextBox.FigNum = NUM_FigNumBGreen16x23;
    CO2InsideTextBox.Coord.X = 110;
    CO2InsideTextBox.Coord.Y = 178;

    System.Gui.Form.AddTextBox(&App.Menu.MainForm, &PMInsideTextBox);
    PMInsideTextBox.DataPointer = (void *)(&App.Data.SensorData.PMInside);
    PMInsideTextBox.Type = GuiDataTypeUnSigned100sDec;
		PMInsideTextBox.FigNum = NUM_FigNumBGreen16x23;
    PMInsideTextBox.Coord.X = 116;
    PMInsideTextBox.Coord.Y = 254;

    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &CircleModeFigLabel);
	CircleModeFigLabel.Coord.X = 251;
	CircleModeFigLabel.Coord.Y = 287;
	CircleModeFigLabel.FigNum = NUM_CircleOutChar43x14;

		
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &ThermalModeFigLabel);
		ThermalModeFigLabel.FigNum = NUM_CloseChar43x14;
    ThermalModeFigLabel.Coord.X = 332;
    ThermalModeFigLabel.Coord.Y = 287;
		
    System.Gui.Form.AddFigLabel(&App.Menu.MainForm, &VentilateRateFigLabel);
		VentilateRateFigLabel.FigNum = NUM_Rate1vs08_43x14;
    VentilateRateFigLabel.Coord.X = 416;
    VentilateRateFigLabel.Coord.Y = 287;

    System.Gui.Form.AddGifLabel(&App.Menu.MainForm, &GreenLeafGif);
	GreenLeafGif.ActiveFlag = true;
	GreenLeafGif.Coord.X = 294;
	GreenLeafGif.Coord.Y = 142;
	GreenLeafGif.Frame =7;
	GreenLeafGif.Speed =6;
	GreenLeafGif.FigNum = NUM_LEAF00_165X67;

/*  SysParaForm  */
    System.Gui.Form.AddRectangle(&App.Menu.SysParaForm, &DotEreaseSysPara);
	DotEreaseSysPara.Coord.X = 15;
	DotEreaseSysPara.Coord.Y = 155;
	DotEreaseSysPara.Bold = 1;
	DotEreaseSysPara.Solid = true;
	DotEreaseSysPara.FontColor = BLACK;
	DotEreaseSysPara.Width = 14;
	DotEreaseSysPara.Height = 13;
	DotEreaseSysPara.Disp = Invisible;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &DotSysPara);
	DotSysPara.Coord.X = 31-14;
	DotSysPara.Coord.Y = 71;
//	DotSysPara.Disp = Invisible;
	DotSysPara.FigNum = NUM_Dot14x13;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TitleRunSetFigLabel);
	TitleRunSetFigLabel.Coord.X = 0;
	TitleRunSetFigLabel.Coord.Y = 0;
	TitleRunSetFigLabel.FigNum = NUM_TitleRunSet480x34;
		
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &CircleSetFigLabel);
	CircleSetFigLabel.Coord.X = SYSPARAFORM_TITLECOL1;
	CircleSetFigLabel.Coord.Y = SYSPARAFORM_TITLEROW1;
	CircleSetFigLabel.FigNum = NUM_CIRCLEmode02_66x79;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &VentilateSetFigLabel);
	VentilateSetFigLabel.Coord.X = SYSPARAFORM_TITLECOL2;
	VentilateSetFigLabel.Coord.Y = SYSPARAFORM_TITLEROW1;
	VentilateSetFigLabel.FigNum = NUM_RATEmode02_66x79;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &ThermalSetFigLabel);
	ThermalSetFigLabel.Coord.X = SYSPARAFORM_TITLECOL3;
	ThermalSetFigLabel.Coord.Y = SYSPARAFORM_TITLEROW1;
	ThermalSetFigLabel.FigNum = NUM_HEATmode02_66X79;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeSetFigLabel);
	TimeSetFigLabel.Coord.X = SYSPARAFORM_TITLECOL4;
	TimeSetFigLabel.Coord.Y = SYSPARAFORM_TITLEROW1;
	TimeSetFigLabel.FigNum = NUM_TimeSet02_66X79;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &CircleOutSetFigLabel);
  		CircleOutSetFigLabel.FigNum = NUM_CircleOutFrame67x30;
    CircleOutSetFigLabel.Coord.X = SYSPARAFORM_PARACOL1;
    CircleOutSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW1;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &CircleInSetFigLabel);
		CircleInSetFigLabel.FigNum = NUM_CircleInFrame67x30;
    CircleInSetFigLabel.Coord.X = SYSPARAFORM_PARACOL1;
    CircleInSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW2;

	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &CircleAutoSetFigLabel);
		CircleAutoSetFigLabel.FigNum = NUM_AutoFrame67x30;
    CircleAutoSetFigLabel.Coord.X = SYSPARAFORM_PARACOL1;
    CircleAutoSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW3;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &VentiRate06SetFigLabel);
		VentiRate06SetFigLabel.FigNum = NUM_Rate1vs06Frame67x30;
    VentiRate06SetFigLabel.Coord.X = SYSPARAFORM_PARACOL2;
    VentiRate06SetFigLabel.Coord.Y = SYSPARAFORM_PARAROW1;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &VentiRate08SetFigLabel);
 		VentiRate08SetFigLabel.FigNum = NUM_Rate1vs08Frame67x30;
    VentiRate08SetFigLabel.Coord.X = SYSPARAFORM_PARACOL2;
    VentiRate08SetFigLabel.Coord.Y = SYSPARAFORM_PARAROW2;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &VentiRate10SetFigLabel);
		VentiRate10SetFigLabel.FigNum = NUM_Rate1vs10Frame67x30;
    VentiRate10SetFigLabel.Coord.X = SYSPARAFORM_PARACOL2;
    VentiRate10SetFigLabel.Coord.Y = SYSPARAFORM_PARAROW3;
	
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &VentiRate12SetFigLabel);
		VentiRate12SetFigLabel.FigNum = NUM_Rate1vs12Frame67x30;
    VentiRate12SetFigLabel.Coord.X = SYSPARAFORM_PARACOL2;
    VentiRate12SetFigLabel.Coord.Y = SYSPARAFORM_PARAROW4;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &ThermalOpenSetFigLabel);
		ThermalOpenSetFigLabel.FigNum = NUM_OpenFrame67x30;
    ThermalOpenSetFigLabel.Coord.X = SYSPARAFORM_PARACOL3;
    ThermalOpenSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW1;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &ThermalCloseSetFigLabel);
 		ThermalCloseSetFigLabel.FigNum = NUM_CloseFrame67x30;
    ThermalCloseSetFigLabel.Coord.X = SYSPARAFORM_PARACOL3;
    ThermalCloseSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW2;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &ThermalAutoSetFigLabel);
		ThermalAutoSetFigLabel.FigNum = NUM_AutoFrame67x30;
    ThermalAutoSetFigLabel.Coord.X = SYSPARAFORM_PARACOL3;
    ThermalAutoSetFigLabel.Coord.Y = SYSPARAFORM_PARAROW3;
	

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeDateFrameFigLabel);
		TimeDateFrameFigLabel.FigNum = NUM_FrameBW125x30;
    TimeDateFrameFigLabel.Coord.X = SYSPARAFORM_PARACOL4;
    TimeDateFrameFigLabel.Coord.Y = SYSPARAFORM_PARAROW1;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeMinFrameFigLabel);
 		TimeMinFrameFigLabel.FigNum = NUM_FrameBW125x30;
    TimeMinFrameFigLabel.Coord.X = SYSPARAFORM_PARACOL4;
    TimeMinFrameFigLabel.Coord.Y = SYSPARAFORM_PARAROW2;
		
    System.Gui.Form.AddTextBox(&App.Menu.SysParaForm, &TimeYearTextBox);
    TimeYearTextBox.DataPointer = (void *)(&App.SysTime.Year);
    TimeYearTextBox.Type = GuiDataTypeUnSigned1000sDec;
		TimeYearTextBox.FigNum = NUM_FigNumBW12x17;
    TimeYearTextBox.Coord.X = 353;
    TimeYearTextBox.Coord.Y = 172;

    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeYearSlashFigLabel);
 		TimeYearSlashFigLabel.FigNum = NUM_Slash10x17;
    TimeYearSlashFigLabel.Coord.X = 401;
    TimeYearSlashFigLabel.Coord.Y = 172;
		
    System.Gui.Form.AddTextBox(&App.Menu.SysParaForm, &TimeMonthTextBox);
    TimeMonthTextBox.DataPointer = (void *)(&App.SysTime.Mon);
    TimeMonthTextBox.Type = GuiDataTypeUnSigned10sDec;
		TimeMonthTextBox.FigNum = NUM_FigNumBW12x17;
    TimeMonthTextBox.Coord.X = 411;
    TimeMonthTextBox.Coord.Y = 172;
		
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeMonthSlashFigLabel);
 		TimeMonthSlashFigLabel.FigNum = NUM_Slash10x17;
    TimeMonthSlashFigLabel.Coord.X = 435;
    TimeMonthSlashFigLabel.Coord.Y = 172;
	
    System.Gui.Form.AddTextBox(&App.Menu.SysParaForm, &TimeDayTextBox);
    TimeDayTextBox.DataPointer = (void *)(&App.SysTime.Mday);
    TimeDayTextBox.Type = GuiDataTypeUnSigned10sDec;
		TimeDayTextBox.FigNum = NUM_FigNumBW12x17;
    TimeDayTextBox.Coord.X = 445;
    TimeDayTextBox.Coord.Y = 172;
		
    System.Gui.Form.AddTextBox(&App.Menu.SysParaForm, &TimeHourTextBox);
    TimeHourTextBox.DataPointer = (void *)(&App.SysTime.Hour);
    TimeHourTextBox.Type = GuiDataTypeUnSigned10sDec;
		TimeHourTextBox.FigNum = NUM_FigNumBW12x17;
    TimeHourTextBox.Coord.X = 353;
    TimeHourTextBox.Coord.Y = 207;
		
    System.Gui.Form.AddFigLabel(&App.Menu.SysParaForm, &TimeColonFigLabel);
 		TimeColonFigLabel.FigNum = NUM_Colon10x17;
    TimeColonFigLabel.Coord.X = 377;
    TimeColonFigLabel.Coord.Y = 207;

    System.Gui.Form.AddTextBox(&App.Menu.SysParaForm, &TimeMinuteTextBox);
    TimeMinuteTextBox.DataPointer = (void *)(&App.SysTime.Min);
    TimeMinuteTextBox.Type = GuiDataTypeUnSigned10sDec;
		TimeMinuteTextBox.FigNum = NUM_FigNumBW12x17;
    TimeMinuteTextBox.Coord.X = 387;
    TimeMinuteTextBox.Coord.Y = 207;	

/*  WeekTimerForm  */
    System.Gui.Form.AddRectangle(&App.Menu.WeekTimerForm, &DotEreaseWeekTimer);
	DotEreaseWeekTimer.Coord.X = 15;
	DotEreaseWeekTimer.Coord.Y = 155;
	DotEreaseWeekTimer.Bold = 1;
	DotEreaseWeekTimer.Solid = true;
	DotEreaseWeekTimer.FontColor = BLACK;
	DotEreaseWeekTimer.Width = 14;
	DotEreaseWeekTimer.Height = 13;
	DotEreaseWeekTimer.Disp = Invisible;

    System.Gui.Form.AddFigLabel(&App.Menu.WeekTimerForm, &DotWeekTimer);
	DotWeekTimer.Coord.X = 31-14;
	DotWeekTimer.Coord.Y = 71;
//	DotSysPara.Disp = Invisible;
	DotWeekTimer.FigNum = NUM_Dot14x13;

    System.Gui.Form.AddFigLabel(&App.Menu.WeekTimerForm, &TitleWeekTimerFigLabel);
	TitleWeekTimerFigLabel.Coord.X = 0;
	TitleWeekTimerFigLabel.Coord.Y = 0;
	TitleWeekTimerFigLabel.FigNum = NUM_TitleTimerSet480x34;
		
    System.Gui.Form.AddFigLabel(&App.Menu.WeekTimerForm, &WeekTimerTableFigLabel);
	WeekTimerTableFigLabel.Coord.X = WEEKTIMERFORM_PARACOL1;
	WeekTimerTableFigLabel.Coord.Y = WEEKTIMERFORM_TITLEROW1;
	WeekTimerTableFigLabel.FigNum = NUM_TimerTitle345x23;
		
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &MondayTextBox);
	MondayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	MondayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW1;
	MondayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	MondayTextBox.Type = GuiDataTypeString;
	MondayTextBox.DataPointer = (void*)&TextFig[1];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &TuesdayTextBox);
	TuesdayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	TuesdayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW2;
	TuesdayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	TuesdayTextBox.Type = GuiDataTypeString;
	TuesdayTextBox.DataPointer = (void*)&TextFig[2];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &WednesdayTextBox);
	WednesdayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	WednesdayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW3;
	WednesdayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	WednesdayTextBox.Type = GuiDataTypeString;
	WednesdayTextBox.DataPointer = (void*)&TextFig[3];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &ThursdayTextBox);
	ThursdayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	ThursdayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW4;
	ThursdayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	ThursdayTextBox.Type = GuiDataTypeString;
	ThursdayTextBox.DataPointer = (void*)&TextFig[4];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &FridayTextBox);
	FridayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	FridayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW5;
	FridayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	FridayTextBox.Type = GuiDataTypeString;
	FridayTextBox.DataPointer = (void*)&TextFig[5];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &SaturdayTextBox);
	SaturdayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	SaturdayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW6;
	SaturdayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	SaturdayTextBox.Type = GuiDataTypeString;
	SaturdayTextBox.DataPointer = (void*)&TextFig[6];
	
	System.Gui.Form.AddTextBox(&App.Menu.WeekTimerForm, &SundayTextBox);
	SundayTextBox.Coord.X = WEEKTIMERFORM_PARACOL1;
	SundayTextBox.Coord.Y = WEEKTIMERFORM_PARAROW7;
	SundayTextBox.FigNum = NUM_FigWeekDayBW46x22;
	SundayTextBox.Type = GuiDataTypeString;
	SundayTextBox.DataPointer = (void*)&TextFig[7];
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &MondayCheckBoxGroup);
	MondayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	MondayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW1;
	MondayCheckBoxGroup.Pitch = 35;
	MondayCheckBoxGroup.Group = 5;
	MondayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.MondayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &TuesdayCheckBoxGroup);
	TuesdayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	TuesdayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW2;
	TuesdayCheckBoxGroup.Pitch = 35;
	TuesdayCheckBoxGroup.Group = 5;
	TuesdayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.TuesdayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &WednesdayCheckBoxGroup);
	WednesdayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	WednesdayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW3;
	WednesdayCheckBoxGroup.Pitch = 35;
	WednesdayCheckBoxGroup.Group = 5;
	WednesdayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.WednsdayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &ThursdayCheckBoxGroup);
	ThursdayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	ThursdayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW4;
	ThursdayCheckBoxGroup.Pitch = 35;
	ThursdayCheckBoxGroup.Group = 5;
	ThursdayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.ThurdayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &FridayCheckBoxGroup);
	FridayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	FridayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW5;
	FridayCheckBoxGroup.Pitch = 35;
	FridayCheckBoxGroup.Group = 5;
	FridayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.FridayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &SaturdayCheckBoxGroup);
	SaturdayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	SaturdayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW6;
	SaturdayCheckBoxGroup.Pitch = 35;
	SaturdayCheckBoxGroup.Group = 5;
	SaturdayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.SaturdayTimer;
	
	System.Gui.Form.AddCheckBoxGroup(&App.Menu.WeekTimerForm, &SundayCheckBoxGroup);
	SundayCheckBoxGroup.Coord.X = WEEKTIMERFORM_CHECKGROUPCOL1;
	SundayCheckBoxGroup.Coord.Y = WEEKTIMERFORM_CHECKGROUPROW7;
	SundayCheckBoxGroup.Pitch = 35;
	SundayCheckBoxGroup.Group = 5;
	SundayCheckBoxGroup.CheckOn = (byte*)&App.Data.WeekTimerMode.SundayTimer;
	
/*  FiterForm  */
    System.Gui.Form.AddRectangle(&App.Menu.FilterForm, &DotEreaseFilter);
	DotEreaseFilter.Coord.X = 15;
	DotEreaseFilter.Coord.Y = 155;
	DotEreaseFilter.Bold = 1;
	DotEreaseFilter.Solid = true;
	DotEreaseFilter.FontColor = BLACK;
	DotEreaseFilter.Width = 14;
	DotEreaseFilter.Height = 13;
	DotEreaseFilter.Disp = Invisible;

    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &DotFilter);
	DotFilter.Coord.X = 31-14;
	DotFilter.Coord.Y = 71;
//	DotSysPara.Disp = Invisible;
	DotFilter.FigNum = NUM_Dot14x13;

   System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &TitleFilterFigLabel);
	TitleFilterFigLabel.Coord.X = 0;
	TitleFilterFigLabel.Coord.Y = 0;
	TitleFilterFigLabel.FigNum = NUM_TitleFilterSet480x34;
		
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &FirstFilterCharFigLabel);
	FirstFilterCharFigLabel.Coord.X = 30;
	FirstFilterCharFigLabel.Coord.Y = 77;
	FirstFilterCharFigLabel.FigNum = NUM_FirstFilterChar73x18;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &FirstFilterBarTextBox);
	FirstFilterBarTextBox.Coord.X = 110;
	FirstFilterBarTextBox.Coord.Y = 79;
	FirstFilterBarTextBox.FigNum = NUM_Progress0Bar229x15;
	FirstFilterBarTextBox.Type = GuiDataTypeString;
	FirstFilterBarTextBox.DataPointer = (void*)&TextFig[0];
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &FirstFilterUsedFigLabel);
	FirstFilterUsedFigLabel.Coord.X = 345;
	FirstFilterUsedFigLabel.Coord.Y = 77;
	FirstFilterUsedFigLabel.FigNum = NUM_UsedChar55x19;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &FirstFilterHourTextBox);
	FirstFilterHourTextBox.Coord.X = 400;
	FirstFilterHourTextBox.Coord.Y = 78;
	FirstFilterHourTextBox.FigNum = NUM_FigNumBW12x17;
	FirstFilterHourTextBox.Type = GuiDataTypeUnSigned1000sDec;
	FirstFilterHourTextBox.Initial = InitialZero;
	FirstFilterHourTextBox.DataPointer = (void*)&App.Data.FilterTimerHour.FirstFilterTimerHour;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &FirstFilterHFigLabel);
	FirstFilterHFigLabel.Coord.X = 448;
	FirstFilterHFigLabel.Coord.Y = 77;
	FirstFilterHFigLabel.FigNum = NUM_HChar14x19;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &FirstFilterResetCharFigLabel);
	FirstFilterResetCharFigLabel.Coord.X = 137;
	FirstFilterResetCharFigLabel.Coord.Y = 100;
	FirstFilterResetCharFigLabel.FigNum = NUM_ResetCharBW43x24;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &FirstFilterCheckCharFigLabel);
	FirstFilterCheckCharFigLabel.Coord.X = 188;
	FirstFilterCheckCharFigLabel.Coord.Y = 100;
	FirstFilterCheckCharFigLabel.FigNum = NUM_ExamCleanChar151x24;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &FirstFilterHourLimitTextBox);
	FirstFilterHourLimitTextBox.Coord.X = 270;
	FirstFilterHourLimitTextBox.Coord.Y = 104;
	FirstFilterHourLimitTextBox.FigNum = NUM_FigNumBW12x17;
	FirstFilterHourLimitTextBox.Type = GuiDataTypeUnSigned1000sDec;
	FirstFilterHourLimitTextBox.Initial = InitialZero;
	FirstFilterHourLimitTextBox.DataPointer = (void*)&App.Data.FilterHourLimit.FirstFilterHourLimit;
		
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &MidFilterCharFigLabel);
	MidFilterCharFigLabel.Coord.X = 30;
	MidFilterCharFigLabel.Coord.Y = 137;
	MidFilterCharFigLabel.FigNum = NUM_MidFilterChar73x18;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &MidFilterBarTextBox);
	MidFilterBarTextBox.Coord.X = 110;
	MidFilterBarTextBox.Coord.Y = 139;
	MidFilterBarTextBox.FigNum = NUM_Progress0Bar229x15;
	MidFilterBarTextBox.Type = GuiDataTypeString;
	MidFilterBarTextBox.DataPointer = (void*)&TextFig[0];
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &MidFilterUsedFigLabel);
	MidFilterUsedFigLabel.Coord.X = 345;
	MidFilterUsedFigLabel.Coord.Y = 137;
	MidFilterUsedFigLabel.FigNum = NUM_UsedChar55x19;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &MidFilterHourTextBox);
	MidFilterHourTextBox.Coord.X = 400;
	MidFilterHourTextBox.Coord.Y = 138;
	MidFilterHourTextBox.FigNum = NUM_FigNumBW12x17;
	MidFilterHourTextBox.Type = GuiDataTypeUnSigned1000sDec;
	MidFilterHourTextBox.Initial = InitialZero;
	MidFilterHourTextBox.DataPointer = (void*)&App.Data.FilterTimerHour.MidFilterTimerHour;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &MidFilterHFigLabel);
	MidFilterHFigLabel.Coord.X = 448;
	MidFilterHFigLabel.Coord.Y = 137;
	MidFilterHFigLabel.FigNum = NUM_HChar14x19;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &MidFilterResetCharFigLabel);
	MidFilterResetCharFigLabel.Coord.X = 137;
	MidFilterResetCharFigLabel.Coord.Y = 160;
	MidFilterResetCharFigLabel.FigNum = NUM_ResetCharBW43x24;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &MidFilterCheckCharFigLabel);
	MidFilterCheckCharFigLabel.Coord.X = 188;
	MidFilterCheckCharFigLabel.Coord.Y = 160;
	MidFilterCheckCharFigLabel.FigNum = NUM_ExamChangeChar151x24;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &MidFilterHourLimitTextBox);
	MidFilterHourLimitTextBox.Coord.X = 270;
	MidFilterHourLimitTextBox.Coord.Y = 164;
	MidFilterHourLimitTextBox.FigNum = NUM_FigNumBW12x17;
	MidFilterHourLimitTextBox.Type = GuiDataTypeUnSigned1000sDec;
	MidFilterHourLimitTextBox.Initial = InitialZero;
	MidFilterHourLimitTextBox.DataPointer = (void*)&App.Data.FilterHourLimit.MidFilterHourLimit;
		
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &ESPFilterCharFigLabel);
	ESPFilterCharFigLabel.Coord.X = 30;
	ESPFilterCharFigLabel.Coord.Y = 197;
	ESPFilterCharFigLabel.FigNum = NUM_EPSChar73x18;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &ESPFilterBarTextBox);
	ESPFilterBarTextBox.Coord.X = 110;
	ESPFilterBarTextBox.Coord.Y = 199;
	ESPFilterBarTextBox.FigNum = NUM_Progress0Bar229x15;
	ESPFilterBarTextBox.Type = GuiDataTypeString;
	ESPFilterBarTextBox.DataPointer = (void*)&TextFig[0];
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &ESPFilterUsedFigLabel);
	ESPFilterUsedFigLabel.Coord.X = 345;
	ESPFilterUsedFigLabel.Coord.Y = 197;
	ESPFilterUsedFigLabel.FigNum = NUM_UsedChar55x19;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &ESPFilterHourTextBox);
	ESPFilterHourTextBox.Coord.X = 400;
	ESPFilterHourTextBox.Coord.Y = 198;
	ESPFilterHourTextBox.FigNum = NUM_FigNumBW12x17;
	ESPFilterHourTextBox.Type = GuiDataTypeUnSigned1000sDec;
	ESPFilterHourTextBox.Initial = InitialZero;
	ESPFilterHourTextBox.DataPointer = (void*)&App.Data.FilterTimerHour.ESPFilterTimerHour;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &ESPFilterHFigLabel);
	ESPFilterHFigLabel.Coord.X = 448;
	ESPFilterHFigLabel.Coord.Y = 197;
	ESPFilterHFigLabel.FigNum = NUM_HChar14x19;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &ESPFilterResetCharFigLabel);
	ESPFilterResetCharFigLabel.Coord.X = 137;
	ESPFilterResetCharFigLabel.Coord.Y = 220;
	ESPFilterResetCharFigLabel.FigNum = NUM_ResetCharBW43x24;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &ESPFilterCheckCharFigLabel);
	ESPFilterCheckCharFigLabel.Coord.X = 188;
	ESPFilterCheckCharFigLabel.Coord.Y = 220;
	ESPFilterCheckCharFigLabel.FigNum = NUM_ExamChangeChar151x24;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &ESPFilterHourLimitTextBox);
	ESPFilterHourLimitTextBox.Coord.X = 270;
	ESPFilterHourLimitTextBox.Coord.Y = 224;
	ESPFilterHourLimitTextBox.FigNum = NUM_FigNumBW12x17;
	ESPFilterHourLimitTextBox.Type = GuiDataTypeUnSigned1000sDec;
	ESPFilterHourLimitTextBox.Initial = InitialZero;
	ESPFilterHourLimitTextBox.DataPointer = (void*)&App.Data.FilterHourLimit.ESPFilterHourLimit;
		
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &HPFilterCharFigLabel);
	HPFilterCharFigLabel.Coord.X = 30;
	HPFilterCharFigLabel.Coord.Y = 257;
	HPFilterCharFigLabel.FigNum = NUM_HPChar73x18;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &HPFilterBarTextBox);
	HPFilterBarTextBox.Coord.X = 110;
	HPFilterBarTextBox.Coord.Y = 259;
	HPFilterBarTextBox.FigNum = NUM_Progress0Bar229x15;
	HPFilterBarTextBox.Type = GuiDataTypeString;
	HPFilterBarTextBox.DataPointer = (void*)&TextFig[0];
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &HPFilterUsedFigLabel);
	HPFilterUsedFigLabel.Coord.X = 345;
	HPFilterUsedFigLabel.Coord.Y = 257;
	HPFilterUsedFigLabel.FigNum = NUM_UsedChar55x19;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &HPFilterHourTextBox);
	HPFilterHourTextBox.Coord.X = 400;
	HPFilterHourTextBox.Coord.Y = 258;
	HPFilterHourTextBox.FigNum = NUM_FigNumBW12x17;
	HPFilterHourTextBox.Type = GuiDataTypeUnSigned1000sDec;
	HPFilterHourTextBox.Initial = InitialZero;
	HPFilterHourTextBox.DataPointer = (void*)&App.Data.FilterTimerHour.HPFilterTimerHour;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &HPFilterHFigLabel);
	HPFilterHFigLabel.Coord.X = 448;
	HPFilterHFigLabel.Coord.Y = 257;
	HPFilterHFigLabel.FigNum = NUM_HChar14x19;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &HPFilterResetCharFigLabel);
	HPFilterResetCharFigLabel.Coord.X = 137;
	HPFilterResetCharFigLabel.Coord.Y = 280;
	HPFilterResetCharFigLabel.FigNum = NUM_ResetCharBW43x24;
	
    System.Gui.Form.AddFigLabel(&App.Menu.FilterForm, &HPFilterCheckCharFigLabel);
	HPFilterCheckCharFigLabel.Coord.X = 188;
	HPFilterCheckCharFigLabel.Coord.Y = 280;
	HPFilterCheckCharFigLabel.FigNum = NUM_ExamChangeChar151x24;
		
	System.Gui.Form.AddTextBox(&App.Menu.FilterForm, &HPFilterHourLimitTextBox);
	HPFilterHourLimitTextBox.Coord.X = 270;
	HPFilterHourLimitTextBox.Coord.Y = 284;
	HPFilterHourLimitTextBox.FigNum = NUM_FigNumBW12x17;
	HPFilterHourLimitTextBox.Type = GuiDataTypeUnSigned1000sDec;
	HPFilterHourLimitTextBox.Initial = InitialZero;
	HPFilterHourLimitTextBox.DataPointer = (void*)&App.Data.FilterHourLimit.HPFilterHourLimit;
		
/*  ModeCtrlForm  */
    System.Gui.Form.AddRectangle(&App.Menu.ModeCtrlForm, &DotEreaseModeCtrl);
	DotEreaseModeCtrl.Coord.X = 15;
	DotEreaseModeCtrl.Coord.Y = 155;
	DotEreaseModeCtrl.Bold = 1;
	DotEreaseModeCtrl.Solid = true;
	DotEreaseModeCtrl.FontColor = BLACK;
	DotEreaseModeCtrl.Width = 14;
	DotEreaseModeCtrl.Height = 13;
	DotEreaseModeCtrl.Disp = Invisible;

    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &DotModeCtrl);
	DotModeCtrl.Coord.X = 31-14;
	DotModeCtrl.Coord.Y = 71;
//	DotSysPara.Disp = Invisible;
	DotModeCtrl.FigNum = NUM_Dot14x13;

   System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &TitleModeCtrlFigLabel);
	TitleModeCtrlFigLabel.Coord.X = 0;
	TitleModeCtrlFigLabel.Coord.Y = 0;
	TitleModeCtrlFigLabel.FigNum = NUM_TitleParaSet480x34;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMSetCharFigLabel);
	PMSetCharFigLabel.Coord.X = 30;
	PMSetCharFigLabel.Coord.Y = 70;
	PMSetCharFigLabel.FigNum = NUM_PMSetChar105x28;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMCtrlPara0FigLabel);
	PMCtrlPara0FigLabel.Coord.X = 140;
	PMCtrlPara0FigLabel.Coord.Y = 71;
	PMCtrlPara0FigLabel.FigNum = NUM_PM100Frame44x27;
	
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMCtrlPara1FigLabel);
	PMCtrlPara1FigLabel.Coord.X = 186;
	PMCtrlPara1FigLabel.Coord.Y = 71;
	PMCtrlPara1FigLabel.FigNum = NUM_PM150Frame44x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMCtrlPara2FigLabel);
	PMCtrlPara2FigLabel.Coord.X = 232;
	PMCtrlPara2FigLabel.Coord.Y = 71;
	PMCtrlPara2FigLabel.FigNum = NUM_PM200Frame44x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMCtrlPara3FigLabel);
	PMCtrlPara3FigLabel.Coord.X = 278;
	PMCtrlPara3FigLabel.Coord.Y = 71;
	PMCtrlPara3FigLabel.FigNum = NUM_PM250Frame44x27;
	
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PMUnitFigLabel);
	PMUnitFigLabel.Coord.X = 324;
	PMUnitFigLabel.Coord.Y = 78;
	PMUnitFigLabel.FigNum = NUM_ugm3_53X20;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &TempSetCharFigLabel);
	TempSetCharFigLabel.Coord.X = 30;
	TempSetCharFigLabel.Coord.Y = 140;
	TempSetCharFigLabel.FigNum = NUM_TempSetChar105x28;
	
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterCharFigLabel);
	WinterCharFigLabel.Coord.X = 140;
	WinterCharFigLabel.Coord.Y = 140;
	WinterCharFigLabel.FigNum = NUM_WinterChar46x28;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterTemp0FigLabel);
	WinterTemp0FigLabel.Coord.X = 190;
	WinterTemp0FigLabel.Coord.Y = 141;
	WinterTemp0FigLabel.FigNum = NUM_Temp14Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterTemp1FigLabel);
	WinterTemp1FigLabel.Coord.X = 245;
	WinterTemp1FigLabel.Coord.Y = 141;
	WinterTemp1FigLabel.FigNum = NUM_Temp15Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterTemp2FigLabel);
	WinterTemp2FigLabel.Coord.X = 300;
	WinterTemp2FigLabel.Coord.Y = 141;
	WinterTemp2FigLabel.FigNum = NUM_Temp16Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterTemp3FigLabel);
	WinterTemp3FigLabel.Coord.X = 355;
	WinterTemp3FigLabel.Coord.Y = 141;
	WinterTemp3FigLabel.FigNum = NUM_Temp17Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &WinterTemp4FigLabel);
	WinterTemp4FigLabel.Coord.X = 410;
	WinterTemp4FigLabel.Coord.Y = 141;
	WinterTemp4FigLabel.FigNum = NUM_Temp18Frame53x27;
				
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerCharFigLabel);
	SummerCharFigLabel.Coord.X = 140;
	SummerCharFigLabel.Coord.Y = 175;
	SummerCharFigLabel.FigNum = NUM_SummerChar46x28;

    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerTemp0FigLabel);
	SummerTemp0FigLabel.Coord.X = 190;
	SummerTemp0FigLabel.Coord.Y = 176;
	SummerTemp0FigLabel.FigNum = NUM_Temp25Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerTemp1FigLabel);
	SummerTemp1FigLabel.Coord.X = 245;
	SummerTemp1FigLabel.Coord.Y = 176;
	SummerTemp1FigLabel.FigNum = NUM_Temp26Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerTemp2FigLabel);
	SummerTemp2FigLabel.Coord.X = 300;
	SummerTemp2FigLabel.Coord.Y = 176;
	SummerTemp2FigLabel.FigNum = NUM_Temp27Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerTemp3FigLabel);
	SummerTemp3FigLabel.Coord.X = 355;
	SummerTemp3FigLabel.Coord.Y = 176;
	SummerTemp3FigLabel.FigNum = NUM_Temp28Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &SummerTemp4FigLabel);
	SummerTemp4FigLabel.Coord.X = 410;
	SummerTemp4FigLabel.Coord.Y = 176;
	SummerTemp4FigLabel.FigNum = NUM_Temp29Frame53x27;
	
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &Co2SetCharFigLabel);
	Co2SetCharFigLabel.Coord.X = 30;
	Co2SetCharFigLabel.Coord.Y = 245;
	Co2SetCharFigLabel.FigNum = NUM_Co2SetChar105x28;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &Co2Para0FigLabel);
	Co2Para0FigLabel.Coord.X = 140;
	Co2Para0FigLabel.Coord.Y = 246;
	Co2Para0FigLabel.FigNum = NUM_Ppm800Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &Co2Para1FigLabel);
	Co2Para1FigLabel.Coord.X = 195;
	Co2Para1FigLabel.Coord.Y = 246;
	Co2Para1FigLabel.FigNum = NUM_Ppm1000Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &Co2Para2FigLabel);
	Co2Para2FigLabel.Coord.X = 250;
	Co2Para2FigLabel.Coord.Y = 246;
	Co2Para2FigLabel.FigNum = NUM_Ppm1200Frame53x27;
		
    System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PpmUnitFigLabel);
	PpmUnitFigLabel.Coord.X = 305;
	PpmUnitFigLabel.Coord.Y = 257;
	PpmUnitFigLabel.FigNum = NUM_ppm38X16;

  /* Manu information */
#ifdef __MAILINSEN_CTRL
	System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &ManuPlusCharTextBox);
	ManuPlusCharTextBox.Coord.X = 336;
	ManuPlusCharTextBox.Coord.Y = 283; 
	ManuPlusCharTextBox.FigNum = NUM_Capital17x17;
	ManuPlusCharTextBox.Type = GuiDataTypeString;
	ManuPlusCharTextBox.Initial = InitialZero;
	ManuPlusCharTextBox.DataPointer = (void*)&VersionInfo[4];
#endif

	System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &ManuCharTextBox);
	ManuCharTextBox.Coord.X = 353;
	ManuCharTextBox.Coord.Y = 283; //290
	ManuCharTextBox.FigNum = NUM_Capital17x17;
	ManuCharTextBox.Type = GuiDataType10sString;
	ManuCharTextBox.Initial = InitialZero;
	ManuCharTextBox.DataPointer = (void*)&VersionInfo[2];
	
  /* Version information */
	System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &VerCharTextBox);
	VerCharTextBox.Coord.X = 393;
	VerCharTextBox.Coord.Y = 283; //290
	VerCharTextBox.FigNum = NUM_Capital17x17;
	VerCharTextBox.Type = GuiDataTypeString;
	VerCharTextBox.Initial = InitialZero;
	VerCharTextBox.DataPointer = (void*)&VersionInfo[0];
	
  System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &CtrlMainVersionTextBox);
	CtrlMainVersionTextBox.Coord.X = 412;
	CtrlMainVersionTextBox.Coord.Y = 275;
	CtrlMainVersionTextBox.FigNum = NUM_FigNumBW12x17;
	CtrlMainVersionTextBox.Type = GuiDataTypeUnSignedNDec;
	CtrlMainVersionTextBox.Initial = InitialZero;
	CtrlMainVersionTextBox.DataPointer = (void*)&App.SysVersion.CtrlMainVersion;
		
  System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &CtrlPointFigLabel);
	CtrlPointFigLabel.Coord.X = 424;
	CtrlPointFigLabel.Coord.Y = 275;
	CtrlPointFigLabel.FigNum = NUM_Point10x17;
	
  System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &CtrlSubVersionTextBox);
	CtrlSubVersionTextBox.Coord.X = 434;
	CtrlSubVersionTextBox.Coord.Y = 275;
	CtrlSubVersionTextBox.FigNum = NUM_FigNumBW12x17;
	CtrlSubVersionTextBox.Type = GuiDataTypeUnSigned10sDec;
	CtrlSubVersionTextBox.Initial = InitialZero;
	CtrlSubVersionTextBox.DataPointer = (void*)&App.SysVersion.CtrlSubVersion;
	
  System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &CtrlMachineTextBox);
	CtrlMachineTextBox.Coord.X = 458;
	CtrlMachineTextBox.Coord.Y = 283;
	CtrlMachineTextBox.FigNum = NUM_Capital17x17;
	CtrlMachineTextBox.Type = GuiDataTypeString;
	CtrlMachineTextBox.Initial = InitialZero;
	CtrlMachineTextBox.DataPointer = (void*)&VersionInfo[1];
	
  System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &PowerMainVersionTextBox);
	PowerMainVersionTextBox.Coord.X = 412;
	PowerMainVersionTextBox.Coord.Y = 292;
	PowerMainVersionTextBox.FigNum = NUM_FigNumBW12x17;
	PowerMainVersionTextBox.Type = GuiDataTypeUnSignedNDec;
	PowerMainVersionTextBox.Initial = InitialZero;
	PowerMainVersionTextBox.DataPointer = (void*)&App.SysVersion.PowerMainVersion;
		
  System.Gui.Form.AddFigLabel(&App.Menu.ModeCtrlForm, &PowerPointFigLabel);
	PowerPointFigLabel.Coord.X = 424;
	PowerPointFigLabel.Coord.Y = 292;
	PowerPointFigLabel.FigNum = NUM_Point10x17;
	
  System.Gui.Form.AddTextBox(&App.Menu.ModeCtrlForm, &PowerSubVersionTextBox);
	PowerSubVersionTextBox.Coord.X = 434;
	PowerSubVersionTextBox.Coord.Y = 292;
	PowerSubVersionTextBox.FigNum = NUM_FigNumBW12x17;
	PowerSubVersionTextBox.Type = GuiDataTypeUnSigned10sDec;
	PowerSubVersionTextBox.Initial = InitialZero;
	PowerSubVersionTextBox.DataPointer = (void*)&App.SysVersion.PowerSubVersion;
	
	
	
	

/*FaultForm*/
    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &TitleFaultFigLabel);
	TitleFaultFigLabel.Coord.X = 0;
	TitleFaultFigLabel.Coord.Y = 0;
	TitleFaultFigLabel.FigNum = NUM_TitleFault480x34;

    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &AlertIconFigLabel);
	AlertIconFigLabel.Coord.X = 30;
	AlertIconFigLabel.Coord.Y = 50;
	AlertIconFigLabel.FigNum = NUM_AlertIcon73x58;

    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &CoreAlertTextLabel);
	CoreAlertTextLabel.Coord.X = 30;
	CoreAlertTextLabel.Coord.Y = 120;
	CoreAlertTextLabel.FigNum = NUM_CoreAlertText444x71;

    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &NormalAlertTextLabel);
	NormalAlertTextLabel.Coord.X = 30;
	NormalAlertTextLabel.Coord.Y = 120;
	NormalAlertTextLabel.FigNum = NUM_NormalAlerText444x69;

    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &FilterAlertIconFigLabel);
	FilterAlertIconFigLabel.Coord.X = 30;
	FilterAlertIconFigLabel.Coord.Y = 50;
	FilterAlertIconFigLabel.FigNum = NUM_FilterAlertIcon60x57;

    System.Gui.Form.AddFigLabel(&App.Menu.FaultInfoForm, &FilterAlertTextLabel);
	FilterAlertTextLabel.Coord.X = 30;
	FilterAlertTextLabel.Coord.Y = 120;
	FilterAlertTextLabel.FigNum = NUM_FilterAlertText430x47;
	
	System.Gui.Form.AddTextBox(&App.Menu.FaultInfoForm, &FaultCodeTextBox);
	FaultCodeTextBox.Coord.X = 100;
	FaultCodeTextBox.Coord.Y = 70;
	FaultCodeTextBox.FigNum = NUM_FigNumBW12x17;
	FaultCodeTextBox.Type = GuiDataTypeUnSigned1000sDec;
	FaultCodeTextBox.DataPointer = (void*)&App.SysFault.FaultFlag;
	
}

void RefreshMenuMainForm(void)
{	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_RHSENSOR)
	{
//		TempInsideTextBox.DataPointer = (void *)(&App.Data.SensorData.TempInside);
		TempInsideTextBox.UpdataFlag = true;
//		TempInsideTextBox.DataPointer = (void *)(&App.Data.SensorData.RHInside);
		RHInsideTextBox.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_RHSENSOR;
	}
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_AIRSENSOR)
	{
		if(App.Data.SensorData.CO2Inside>1200)
			CO2InsideTextBox.FigNum=NUM_FigNumBRed16x23;
		else if(App.Data.SensorData.CO2Inside>1000)
			CO2InsideTextBox.FigNum=NUM_FigNumBOrange16x23;
		else
			CO2InsideTextBox.FigNum=NUM_FigNumBGreen16x23;
		
		if(App.Data.SensorData.PMInside>100)
			PMInsideTextBox.FigNum=NUM_FigNumBRed16x23;
		else if(App.Data.SensorData.PMInside>65)
			PMInsideTextBox.FigNum=NUM_FigNumBOrange16x23;
		else
			PMInsideTextBox.FigNum=NUM_FigNumBGreen16x23;
		
		CO2InsideTextBox.UpdataFlag = true;
		PMInsideTextBox.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag |=MAINFORM_CICLEMODE;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_AIRSENSOR;
	}
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_CICLEMODE)
	{
		switch(App.Data.SysCtrlPara.CircleModeSet)
		{
			case CIRCLEMODE_OUT:
				CircleModeFigLabel.FigNum = NUM_CircleOutChar43x14;
//				App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEOUT;  //move to logic
			break;
			case CIRCLEMODE_IN:
				CircleModeFigLabel.FigNum = NUM_CircleInChar43x14;
//				App.SysCtrlStatus.BypassMode = BYPASS_CIRCLEIN;
			break;
			case CIRCLEMODE_AUTO:
				CircleModeFigLabel.FigNum = NUM_AutoChar43x14;
			break;
			default:
				break;
		}	
		CircleModeFigLabel.UpdataFlag = true;
		if(App.SysCtrlStatus.BypassMode==BYPASS_CIRCLEOUT)
		{
			CircleIconFigLabel.Disp = NormalDisp;
			ThermalModeBmpFigLabel.FigNum = NUM_HEATmode57X67;
			VentilateRateModeBmpFigLabel.FigNum = NUM_RATEmode57X67;
		}
		else
		{
			CircleIconFigLabel.Disp = Invisible;
			ThermalModeBmpFigLabel.FigNum = NUM_HEATmodeNot57X67;
			VentilateRateModeBmpFigLabel.FigNum = NUM_RATEmodeNot57X67;
		}
		CircleIconFigLabel.UpdataFlag = true;
		ThermalModeBmpFigLabel.UpdataFlag = true;
		VentilateRateModeBmpFigLabel.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_CICLEMODE;

//		PostMessage(MessageParaUpdate, CIRCLEMODE);
	}
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_THERMALMODE)
	{
		switch(App.Data.SysCtrlPara.ThermalModeSet)
		{
			case HEATMODE_AUTO:
				ThermalModeFigLabel.FigNum =NUM_AutoChar43x14;
				break;
			case HEATMODE_ON:
				ThermalModeFigLabel.FigNum =NUM_OpenChar43x14;
				break;
			case HEATMODE_OFF:
				ThermalModeFigLabel.FigNum =NUM_CloseChar43x14;
				break;
			default:
				break;
		}
		ThermalModeFigLabel.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_THERMALMODE;
		
//		PostMessage(MessageParaUpdate, HEATTEMPER);
	}
	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_VENTIRATE)
	{
		switch(App.Data.SysCtrlPara.VentilateRate)
		{
			case RATE10TO06:
				VentilateRateFigLabel.FigNum = NUM_Rate1vs06_43x14;
			break;
			case RATE10TO08:
				VentilateRateFigLabel.FigNum = NUM_Rate1vs08_43x14;
			break;
			case RATE10TO10:
				VentilateRateFigLabel.FigNum = NUM_Rate1vs10_43x14;
			break;
			case RATE10TO12:
				VentilateRateFigLabel.FigNum = NUM_Rate1vs12_43x14;
			break;
			default:
				break;
		}
		VentilateRateFigLabel.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_VENTIRATE;

		
	}
	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_CHILDLOCK)
	{
		if(App.SysCtrlStatus.ChildLock == CHILD_LOCK)
			LockIconFigLabel.Disp = NormalDisp;
		else
			LockIconFigLabel.Disp = Invisible;
		LockIconFigLabel.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_CHILDLOCK;
	}
	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_RUNMODE)
	{
		if(App.Data.SysCtrlPara.WorkMode == ALLBYHAND)
			TimerModeIconFigLabel.Disp = Invisible;
		else
			TimerModeIconFigLabel.Disp = NormalDisp;
		TimerModeIconFigLabel.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_RUNMODE;
	}
	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_AIRFLOW)
	{
		FlowVolumeTextBox.DataPointer = (void *)(&AirVolume[App.SysCtrlStatus.AirFlowSet]);
		FlowVolumeTextBox.UpdataFlag = true;
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_AIRFLOW;
	}
	
	if(App.Menu.MainForm.RefreshFlag&MAINFORM_WIFISTATE)
	{
		WIFIIconFigLabel.Disp = Invisible;
		WIFIIconFigLabel.UpdataFlag = true;
		if(App.WifiState&WIFI_STATE_CONFIG)
		{
			WIFIIconFigLabel.Disp = InvisibleFlashDisp;
		}
		else if(App.WifiState&WIFI_STATE_STA)
		{
			WIFIIconFigLabel.Disp = NormalDisp;
		}
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_WIFISTATE;
	}

	if(App.Menu.MainForm.RefreshFlag&MAINFORM_RFSTATE)
	{
		RFIconFigLabel.Disp = Invisible;
		RFIconFigLabel.UpdataFlag = true;
		if(App.RFState&0x02)
		{
			RFIconFigLabel.Disp = FlashDisp;
			RFIconFigLabel.UpdataFlag = true;
		}
		else if(App.RFState&0x01)
		{
			RFIconFigLabel.Disp = NormalDisp;
			RFIconFigLabel.UpdataFlag = true;
		}
		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_RFSTATE;
	}
	if(MinuteBuf !=App.SysTime.Min)
	{
		RTCYearTextBox.UpdataFlag=true;
		RTCMonthTextBox.UpdataFlag=true;
		RTCDayTextBox.UpdataFlag=true;
		RTCWeekDayTextBox.UpdataFlag=true;
		RTCHourTextBox.UpdataFlag=true;
		RTCMinuteTextBox.UpdataFlag=true;
		MinuteBuf =App.SysTime.Min;
	}

#ifdef __FAULT_DISP	
//	if(App.Menu.MainForm.RefreshFlag&MAINFORM_FAULT)
//	{
		if((App.SysFault.FaultFlag&FILTER_CHECKFLAG)&&(CleanIconFigLabel.Disp != NormalDisp))
		{
			CleanIconFigLabel.Disp = NormalDisp;
			CleanIconFigLabel.UpdataFlag = true;
		}
		else if((!(App.SysFault.FaultFlag&FILTER_CHECKFLAG))&&(CleanIconFigLabel.Disp != Invisible))
		{
			CleanIconFigLabel.Disp = Invisible;
			CleanIconFigLabel.UpdataFlag = true;
		}
	
		if((App.SysFault.FaultFlag&SYSFAULT_FLAG)&&(FaultIconFigLabel.Disp != NormalDisp))
		{
			FaultIconFigLabel.Disp = NormalDisp;
			FaultIconFigLabel.UpdataFlag = true;
		}
		else if((!(App.SysFault.FaultFlag&SYSFAULT_FLAG))&&(FaultIconFigLabel.Disp != Invisible))
		{
			FaultIconFigLabel.Disp = Invisible;
			FaultIconFigLabel.UpdataFlag = true;
		}
		else if(App.SysFault.FaultFlag&STORE_FAULT)
		{
			FaultIconFigLabel.Disp = InvisibleFlashDisp;
			FaultIconFigLabel.UpdataFlag = true;
		}
//		App.Menu.MainForm.RefreshFlag &= ~MAINFORM_FAULT;		
//	}
#endif
		
	if(App.Menu.MainForm.RefreshFlag&FORMSCREEN_NEW)
	{
		System.Device.RemoteIR.RemoteIRCtrl(ENABLE);
		System.Gui.LoadForm(&App.Menu.MainForm);
		System.Device.Lcd.LCD_ClearScreen(0);
		CleanIconFigLabel.Disp = Invisible;
		FaultIconFigLabel.Disp = Invisible;
		App.Menu.MainForm.RefreshFlag &= ~FORMSCREEN_NEW;
		MinuteBuf =App.SysTime.Min;
//		App.Menu.MainForm.RefreshFlag |= MAINFORM_TIME;
	}

	

	
	
	
#ifdef __DEBUG
	XFRpmTextBox.UpdataFlag = true;
	PFRpmTextBox.UpdataFlag = true;
#endif


	


	
}

void RefreshMenuSysParaForm(void)
{
	
		
	if(App.Menu.SysParaForm.RefreshFlag&SYSPARAFORM_CIRCLEMODE)
	{
		switch(App.Data.SysCtrlPara.CircleModeSet)
		{
			case CIRCLEMODE_OUT:
				CircleOutSetFigLabel.Disp = InverseDisp;
				CircleInSetFigLabel.Disp = NormalDisp;
				CircleAutoSetFigLabel.Disp = NormalDisp;
				break;
			case CIRCLEMODE_IN:
				CircleInSetFigLabel.Disp = InverseDisp;
				CircleOutSetFigLabel.Disp = NormalDisp;
				CircleAutoSetFigLabel.Disp = NormalDisp;
				break;
			case CIRCLEMODE_AUTO:
				CircleAutoSetFigLabel.Disp = InverseDisp;
				CircleInSetFigLabel.Disp = NormalDisp;
				CircleOutSetFigLabel.Disp = NormalDisp;
				break;
		}
				CircleOutSetFigLabel.UpdataFlag = true;
				CircleInSetFigLabel.UpdataFlag = true;
				CircleAutoSetFigLabel.UpdataFlag = true;
		App.Menu.SysParaForm.RefreshFlag &= ~SYSPARAFORM_CIRCLEMODE;
	}
		
	if(App.Menu.SysParaForm.RefreshFlag&SYSPARAFORM_VERIRATE)
	{
		switch(App.Data.SysCtrlPara.VentilateRate)
		{
			case RATE10TO06:
				VentiRate06SetFigLabel.Disp = InverseDisp;
				VentiRate08SetFigLabel.Disp = NormalDisp;
				VentiRate10SetFigLabel.Disp = NormalDisp;
				VentiRate12SetFigLabel.Disp = NormalDisp;
				break;
			case RATE10TO08:
				VentiRate08SetFigLabel.Disp = InverseDisp;
				VentiRate06SetFigLabel.Disp = NormalDisp;
				VentiRate10SetFigLabel.Disp = NormalDisp;
				VentiRate12SetFigLabel.Disp = NormalDisp;
				break;
			case RATE10TO10:
				VentiRate10SetFigLabel.Disp = InverseDisp;
				VentiRate06SetFigLabel.Disp = NormalDisp;
				VentiRate08SetFigLabel.Disp = NormalDisp;
				VentiRate12SetFigLabel.Disp = NormalDisp;
				break;
			case RATE10TO12:
				VentiRate12SetFigLabel.Disp = InverseDisp;
				VentiRate06SetFigLabel.Disp = NormalDisp;
				VentiRate08SetFigLabel.Disp = NormalDisp;
				VentiRate10SetFigLabel.Disp = NormalDisp;
				break;
		}
		VentiRate12SetFigLabel.UpdataFlag = true;
		VentiRate06SetFigLabel.UpdataFlag = true;
		VentiRate08SetFigLabel.UpdataFlag = true;
		VentiRate10SetFigLabel.UpdataFlag = true;
		App.Menu.SysParaForm.RefreshFlag &= ~SYSPARAFORM_VERIRATE;
	}
		
	if(App.Menu.SysParaForm.RefreshFlag&SYSPARAFORM_THERMALMODE)
	{
		switch(App.Data.SysCtrlPara.ThermalModeSet)
		{
			case HEATMODE_AUTO:
				ThermalAutoSetFigLabel.Disp = InverseDisp;
				ThermalOpenSetFigLabel.Disp = NormalDisp;
				ThermalCloseSetFigLabel.Disp = NormalDisp;
				break;
			case HEATMODE_ON:
				ThermalOpenSetFigLabel.Disp = InverseDisp;
				ThermalCloseSetFigLabel.Disp = NormalDisp;
				ThermalAutoSetFigLabel.Disp = NormalDisp;
				break;
			case HEATMODE_OFF:
				ThermalOpenSetFigLabel.Disp = NormalDisp;
				ThermalCloseSetFigLabel.Disp = InverseDisp;
				ThermalAutoSetFigLabel.Disp = NormalDisp;
				break;
		}
		ThermalOpenSetFigLabel.UpdataFlag = true;
		ThermalCloseSetFigLabel.UpdataFlag = true;
		ThermalAutoSetFigLabel.UpdataFlag = true;
		App.Menu.SysParaForm.RefreshFlag &= ~SYSPARAFORM_THERMALMODE;
	}
	if(App.Menu.SysParaForm.RefreshFlag&SYSPARAFORM_SYSTEMTIME)
	{
		if(App.KeyBtn.Focus == FOCUS_PARAVAL)
		{
			switch(App.KeyBtn.ParaSubTitle)
			{
				case PARAVAL_YEAR:
					TimeYearTextBox.Disp = InverseDisp;			
					TimeMonthTextBox.Disp = NormalDisp;
					TimeDayTextBox.Disp = NormalDisp;
					TimeHourTextBox.Disp = NormalDisp;
					TimeMinuteTextBox.Disp = NormalDisp;
					break;
				case PARAVAL_MONTH:
					TimeMonthTextBox.Disp = InverseDisp;
					TimeYearTextBox.Disp = NormalDisp;			
					TimeDayTextBox.Disp = NormalDisp;
					TimeHourTextBox.Disp = NormalDisp;
					TimeMinuteTextBox.Disp = NormalDisp;
					break;
				case PARAVAL_DAY:
					TimeDayTextBox.Disp = InverseDisp;
					TimeMonthTextBox.Disp = NormalDisp;
					TimeYearTextBox.Disp = NormalDisp;			
					TimeHourTextBox.Disp = NormalDisp;
					TimeMinuteTextBox.Disp = NormalDisp;
					break;
				case PARAVAL_HOUR:
					TimeHourTextBox.Disp = InverseDisp;
					TimeMinuteTextBox.Disp = NormalDisp;
					TimeDayTextBox.Disp = NormalDisp;
					TimeMonthTextBox.Disp = NormalDisp;
					TimeYearTextBox.Disp = NormalDisp;			
					break;
				case PARAVAL_MIN:
					TimeMinuteTextBox.Disp = InverseDisp;
					TimeHourTextBox.Disp = NormalDisp;
					TimeDayTextBox.Disp = NormalDisp;
					TimeMonthTextBox.Disp = NormalDisp;
					TimeYearTextBox.Disp = NormalDisp;			
					break;
				default:
					TimeMinuteTextBox.Disp = NormalDisp;
					TimeHourTextBox.Disp = NormalDisp;
					TimeDayTextBox.Disp = NormalDisp;
					TimeMonthTextBox.Disp = NormalDisp;
					TimeYearTextBox.Disp = NormalDisp;			
					break;
			}
			App.Menu.SysParaForm.RefreshFlag &= ~SYSPARAFORM_SYSTEMTIME;
		}
		else if(App.KeyBtn.Focus == FOCUS_PARAVALSET)
		{
			switch(App.KeyBtn.ParaSubTitle)
			{
				case PARAVAL_YEAR:
					TimeYearTextBox.Disp = FlashDisp;			
					break;
				case PARAVAL_MONTH:
					TimeMonthTextBox.Disp = FlashDisp;
					break;
				case PARAVAL_DAY:
					TimeDayTextBox.Disp = FlashDisp;
					break;
				case PARAVAL_HOUR:
					TimeHourTextBox.Disp = FlashDisp;
					break;
				case PARAVAL_MIN:
					TimeMinuteTextBox.Disp = FlashDisp;
					break;
			}
		}
		else if(App.KeyBtn.Focus == FOCUS_PARASUBTITLE)
		{
			TimeMinuteTextBox.Disp = NormalDisp;
			TimeHourTextBox.Disp = NormalDisp;
			TimeDayTextBox.Disp = NormalDisp;
			TimeMonthTextBox.Disp = NormalDisp;
			TimeYearTextBox.Disp = NormalDisp;			

		}
		TimeYearTextBox.UpdataFlag = true;
		TimeMonthTextBox.UpdataFlag = true;
		TimeDayTextBox.UpdataFlag = true;
		TimeHourTextBox.UpdataFlag = true;
		TimeMinuteTextBox.UpdataFlag = true;
	}
	if((App.KeyBtn.Focus)&&(Position != App.KeyBtn.Position))
	{
		if(App.KeyBtn.Position>0)
		{
			DotSysPara.Disp = NormalDisp;
			DotEreaseSysPara.Disp = NormalDisp;
			DotEreaseSysPara.Coord = DotSysPara.Coord;
			DotSysPara.Coord = DotLocArraySysPara[App.KeyBtn.Position-1];
			DotSysPara.UpdataFlag =true;
			if(Position)
				DotEreaseSysPara.UpdataFlag =true;
		}
		else
		{
			DotEreaseSysPara.Coord = DotSysPara.Coord;
			DotSysPara.Coord = DotLocArraySysPara[0];
			DotEreaseSysPara.UpdataFlag =true;
		}
		Position = App.KeyBtn.Position;
	}
	if(App.Menu.SysParaForm.RefreshFlag&FORMSCREEN_NEW)
	{
		App.Menu.SysParaForm.RefreshFlag =0;
		
		DotSysPara.Coord = DotLocArraySysPara[0];
		DotSysPara.Disp = Invisible;
		DotEreaseSysPara.Coord = DotSysPara.Coord;
		DotEreaseSysPara.Disp = Invisible;
		
		App.KeyBtn.Focus = FOCUS_ONFORM;
		App.KeyBtn.ParaTitle = FORM_FOCUS;
		App.KeyBtn.Position = FORM_FOCUS;
		Position =0;
		System.Gui.LoadForm(&App.Menu.SysParaForm);
		System.Device.RemoteIR.RemoteIRCtrl(DISABLE);
		System.Device.Lcd.LCD_ClearScreen(1);
		App.Menu.SysParaForm.RefreshFlag &=~FORMSCREEN_NEW;
	}

			
	
	
}
void RefreshMenuWeekTimerForm(void)
{	
		
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP1)
	{
		MondayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		MondayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP1;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP2)
	{
		TuesdayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		TuesdayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP2;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP3)
	{
		WednesdayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		WednesdayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP3;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP4)
	{
		ThursdayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		ThursdayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP4;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP5)
	{
		FridayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		FridayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP5;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP6)
	{
		SaturdayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		SaturdayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP6;
	}
	if(App.Menu.WeekTimerForm.RefreshFlag&WEEKTIMERFORM_CHECKGROUP7)
	{
		SundayCheckBoxGroup.FocuseOn = App.KeyBtn.ParaSubTitle;
		SundayCheckBoxGroup.UpdataFlag = true;
		App.Menu.WeekTimerForm.RefreshFlag &= ~WEEKTIMERFORM_CHECKGROUP7;
	}
		
	if((App.KeyBtn.Focus)&&(Position != App.KeyBtn.Position))
	{
		if(App.KeyBtn.Position>0)
		{
			DotWeekTimer.Disp = NormalDisp;
			DotEreaseWeekTimer.Disp = NormalDisp;
			DotEreaseWeekTimer.Coord = DotWeekTimer.Coord;
			DotWeekTimer.Coord = DotLocArrayWeekTimer[App.KeyBtn.Position-5];
			DotWeekTimer.UpdataFlag =true;
			if(Position)
				DotEreaseWeekTimer.UpdataFlag =true;
		}
		else
		{
			DotEreaseWeekTimer.Coord = DotWeekTimer.Coord;
			DotWeekTimer.Coord = DotLocArrayWeekTimer[0];
			DotEreaseWeekTimer.UpdataFlag =true;
		}
		Position = App.KeyBtn.Position;
	}

	if(App.Menu.WeekTimerForm.RefreshFlag&FORMSCREEN_NEW)
	{ 
		App.Menu.WeekTimerForm.RefreshFlag =0;
		
		DotWeekTimer.Coord = DotLocArraySysPara[0];
		DotWeekTimer.Disp = Invisible;
		DotEreaseWeekTimer.Coord = DotSysPara.Coord;
		DotEreaseWeekTimer.Disp = Invisible;
		
		App.KeyBtn.Focus = FOCUS_ONFORM;
		App.KeyBtn.ParaTitle = FORM_FOCUS;
		App.KeyBtn.Position = FORM_FOCUS;
		Position =0;
		System.Gui.LoadForm(&App.Menu.WeekTimerForm);
		System.Device.Lcd.LCD_ClearScreen(1);
		App.Menu.WeekTimerForm.RefreshFlag &= ~FORMSCREEN_NEW;
	}
}

void RefreshMenuFilterForm(void)
{
	uint valtemp=0;
	
	if(App.Menu.FilterForm.RefreshFlag&FILTERFORM_FIRST)
	{
		switch(App.KeyBtn.Focus)
		{
			case FOCUS_PARASUBTITLE:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					FirstFilterResetCharFigLabel.Disp = InverseDisp;
					FirstFilterHourLimitTextBox.Disp = NormalDisp;
				}
				else
				{
					FirstFilterHourLimitTextBox.Disp = InverseDisp;
					FirstFilterResetCharFigLabel.Disp = NormalDisp;
				}
				FirstFilterResetCharFigLabel.UpdataFlag = true;
				FirstFilterHourLimitTextBox.UpdataFlag = true;
				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_FIRST;
				break;
			case FOCUS_PARATITLE:
				FirstFilterResetCharFigLabel.Disp = NormalDisp;
				FirstFilterHourLimitTextBox.Disp = NormalDisp;
				FirstFilterResetCharFigLabel.UpdataFlag = true;
				FirstFilterHourLimitTextBox.UpdataFlag = true;
				FirstFilterHourTextBox.UpdataFlag = true;
			
				valtemp = App.Data.FilterTimerHour.FirstFilterTimerHour * 10;
				valtemp /= App.Data.FilterHourLimit.FirstFilterHourLimit;
				if(valtemp>9) valtemp =10;
				FirstFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
				FirstFilterBarTextBox.UpdataFlag=true;

				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_FIRST;
				break;
			case FOCUS_PARAVALSET:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					FirstFilterResetCharFigLabel.Disp = FlashDisp;
//					FirstFilterHourLimitTextBox.Disp = NormalDisp;
					FirstFilterResetCharFigLabel.UpdataFlag = true;
				}
				else
				{
					FirstFilterHourLimitTextBox.Disp = FlashDisp;
//					FirstFilterResetCharFigLabel.Disp = NormalDisp;
					FirstFilterHourLimitTextBox.UpdataFlag = true;
				}
			break;
			default:
				break;
		}
				
	}
		
	if(App.Menu.FilterForm.RefreshFlag&FILTERFORM_MID)
	{
		switch(App.KeyBtn.Focus)
		{
			case FOCUS_PARASUBTITLE:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					MidFilterResetCharFigLabel.Disp = InverseDisp;
					MidFilterHourLimitTextBox.Disp = NormalDisp;
				}
				else
				{
					MidFilterHourLimitTextBox.Disp = InverseDisp;
					MidFilterResetCharFigLabel.Disp = NormalDisp;
				}
				MidFilterResetCharFigLabel.UpdataFlag = true;
				MidFilterHourLimitTextBox.UpdataFlag = true;
				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_MID;		
				break;
			case FOCUS_PARATITLE:
					MidFilterResetCharFigLabel.Disp = NormalDisp;
					MidFilterHourLimitTextBox.Disp = NormalDisp;
				MidFilterResetCharFigLabel.UpdataFlag = true;
				MidFilterHourLimitTextBox.UpdataFlag = true;
				MidFilterHourTextBox.UpdataFlag = true;
			
				valtemp = App.Data.FilterTimerHour.MidFilterTimerHour * 10;
				valtemp /= App.Data.FilterHourLimit.MidFilterHourLimit;
				if(valtemp>9) valtemp =10;
				MidFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
				MidFilterBarTextBox.UpdataFlag=true;
			
				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_MID;		
				break;
			case FOCUS_PARAVALSET:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					MidFilterResetCharFigLabel.Disp = FlashDisp;
//					MidFilterHourLimitTextBox.Disp = NormalDisp;
					MidFilterResetCharFigLabel.UpdataFlag = true;
				}
				else
				{
					MidFilterHourLimitTextBox.Disp = FlashDisp;
//					MidFilterResetCharFigLabel.Disp = NormalDisp;
					MidFilterHourLimitTextBox.UpdataFlag = true;
				}
				break;
			default:
				break;
		}
	}

	if(App.Menu.FilterForm.RefreshFlag&FILTERFORM_ESP)
	{
		switch(App.KeyBtn.Focus)
		{
			case FOCUS_PARASUBTITLE:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					ESPFilterResetCharFigLabel.Disp = InverseDisp;
					ESPFilterHourLimitTextBox.Disp = NormalDisp;
				}
				else
				{
					ESPFilterHourLimitTextBox.Disp = InverseDisp;
					ESPFilterResetCharFigLabel.Disp = NormalDisp;
				}
				ESPFilterResetCharFigLabel.UpdataFlag = true;
				ESPFilterHourLimitTextBox.UpdataFlag = true;
				App.Menu.FilterForm.RefreshFlag&= ~FILTERFORM_ESP;
				break;
			case FOCUS_PARATITLE:
					ESPFilterResetCharFigLabel.Disp = NormalDisp;
					ESPFilterHourLimitTextBox.Disp = NormalDisp;
				ESPFilterResetCharFigLabel.UpdataFlag = true;
				ESPFilterHourLimitTextBox.UpdataFlag = true;
				ESPFilterHourTextBox.UpdataFlag = true;
			
				valtemp = App.Data.FilterTimerHour.ESPFilterTimerHour * 10;
				valtemp /= App.Data.FilterHourLimit.ESPFilterHourLimit;
				if(valtemp>9) valtemp =10;
				ESPFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
				ESPFilterBarTextBox.UpdataFlag=true;
				App.Menu.FilterForm.RefreshFlag&= ~FILTERFORM_ESP;
				break;
			case FOCUS_PARAVALSET:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					ESPFilterResetCharFigLabel.Disp = FlashDisp;
//					ESPFilterHourLimitTextBox.Disp = NormalDisp;
					ESPFilterResetCharFigLabel.UpdataFlag = true;
				}
				else
				{
					ESPFilterHourLimitTextBox.Disp = FlashDisp;
//					ESPFilterResetCharFigLabel.Disp = NormalDisp;
					ESPFilterHourLimitTextBox.UpdataFlag = true;
				}
				break;
			default:
				break;
		}
				
	}

	if(App.Menu.FilterForm.RefreshFlag&FILTERFORM_HP)
	{
		switch(App.KeyBtn.Focus)
		{
			case FOCUS_PARASUBTITLE:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					HPFilterResetCharFigLabel.Disp = InverseDisp;
					HPFilterHourLimitTextBox.Disp = NormalDisp;
				}
				else
				{
					HPFilterHourLimitTextBox.Disp = InverseDisp;
					HPFilterResetCharFigLabel.Disp = NormalDisp;
				}
				HPFilterResetCharFigLabel.UpdataFlag = true;
				HPFilterHourLimitTextBox.UpdataFlag = true;
				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_HP;
				break;
			case FOCUS_PARATITLE:
					HPFilterResetCharFigLabel.Disp = NormalDisp;
					HPFilterHourLimitTextBox.Disp = NormalDisp;
				HPFilterResetCharFigLabel.UpdataFlag = true;
				HPFilterHourLimitTextBox.UpdataFlag = true;
				HPFilterHourTextBox.UpdataFlag = true;
			
				valtemp = App.Data.FilterTimerHour.HPFilterTimerHour * 10;
				valtemp /= App.Data.FilterHourLimit.HPFilterHourLimit;
				if(valtemp>9) valtemp =10;
				HPFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
				HPFilterBarTextBox.UpdataFlag=true;
				App.Menu.FilterForm.RefreshFlag &= ~FILTERFORM_HP;
				break;
			case FOCUS_PARAVALSET:
				if(App.KeyBtn.ParaSubTitle==1)
				{
					HPFilterResetCharFigLabel.Disp = FlashDisp;
//					HPFilterHourLimitTextBox.Disp = NormalDisp;
					HPFilterResetCharFigLabel.UpdataFlag = true;
				}
				else
				{
					HPFilterHourLimitTextBox.Disp = FlashDisp;
//					HPFilterResetCharFigLabel.Disp = NormalDisp;
					HPFilterHourLimitTextBox.UpdataFlag = true;
				}
				break;
			default:
				break;
		}
				
	}	
	
	if((App.KeyBtn.Focus)&&(Position != App.KeyBtn.Position))
	{
		if(App.KeyBtn.Position>0)
		{
			DotFilter.Disp = NormalDisp;
			DotEreaseFilter.Disp = NormalDisp;
			DotEreaseFilter.Coord = DotFilter.Coord;
			DotFilter.Coord = DotLocArrayFilter[App.KeyBtn.Position-12];
			DotFilter.UpdataFlag =true;
			if(Position)
				DotEreaseFilter.UpdataFlag =true;
		}
		else
		{
			DotEreaseFilter.Coord = DotFilter.Coord;
			DotFilter.Coord = DotLocArrayFilter[0];
			DotEreaseFilter.UpdataFlag =true;
		}
		Position = App.KeyBtn.Position;
	}
	if(App.Menu.FilterForm.RefreshFlag&FORMSCREEN_NEW)
	{
		valtemp = App.Data.FilterTimerHour.FirstFilterTimerHour * 10;
		valtemp /= App.Data.FilterHourLimit.FirstFilterHourLimit;
		if(valtemp>9) valtemp =10;
		FirstFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
		valtemp = App.Data.FilterTimerHour.MidFilterTimerHour * 10;
		valtemp /= App.Data.FilterHourLimit.MidFilterHourLimit;
		if(valtemp>9) valtemp =10;
		MidFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
		valtemp = App.Data.FilterTimerHour.ESPFilterTimerHour * 10;
		valtemp /= App.Data.FilterHourLimit.ESPFilterHourLimit;
		if(valtemp>9) valtemp =10;
		ESPFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
		valtemp = App.Data.FilterTimerHour.HPFilterTimerHour * 10;
		valtemp /= App.Data.FilterHourLimit.HPFilterHourLimit;
		if(valtemp>9) valtemp =10;
		HPFilterBarTextBox.DataPointer = (void*)&TextFig[valtemp];
		
		App.Menu.FilterForm.RefreshFlag =0;
		
		DotFilter.Coord = DotLocArrayFilter[0];
		DotFilter.Disp = Invisible;
		DotEreaseFilter.Coord = DotFilter.Coord;
		DotEreaseFilter.Disp = Invisible;
		
		App.KeyBtn.Focus = FOCUS_ONFORM;
		App.KeyBtn.ParaTitle = FORM_FOCUS;
		App.KeyBtn.Position = FORM_FOCUS;
		Position =0;
		App.Menu.FilterForm.RefreshFlag &= ~FORMSCREEN_NEW;
		System.Gui.LoadForm(&App.Menu.FilterForm);
		System.Device.Lcd.LCD_ClearScreen(1);
	}

}
void RefreshMenuCtrlForm(void)
{
	if(App.Menu.ModeCtrlForm.RefreshFlag&MODECTRLFORM_PM)
	{
		switch(App.Data.SysCtrlLine.PMInsideLine)
		{
			case 100:
				PMCtrlPara0FigLabel.Disp = InverseDisp;
				PMCtrlPara1FigLabel.Disp = NormalDisp;
				PMCtrlPara2FigLabel.Disp = NormalDisp;
				PMCtrlPara3FigLabel.Disp = NormalDisp;
				break;
			case 150:
				PMCtrlPara1FigLabel.Disp = InverseDisp;
				PMCtrlPara0FigLabel.Disp = NormalDisp;
				PMCtrlPara2FigLabel.Disp = NormalDisp;
				PMCtrlPara3FigLabel.Disp = NormalDisp;
				break;
			case 200:
				PMCtrlPara2FigLabel.Disp = InverseDisp;
				PMCtrlPara0FigLabel.Disp = NormalDisp;
				PMCtrlPara1FigLabel.Disp = NormalDisp;
				PMCtrlPara3FigLabel.Disp = NormalDisp;
				break;
			case 250:
				PMCtrlPara3FigLabel.Disp = InverseDisp;
				PMCtrlPara0FigLabel.Disp = NormalDisp;
				PMCtrlPara1FigLabel.Disp = NormalDisp;
				PMCtrlPara2FigLabel.Disp = NormalDisp;
				break;
		}
		PMCtrlPara1FigLabel.UpdataFlag = true;
		PMCtrlPara2FigLabel.UpdataFlag = true;
		PMCtrlPara3FigLabel.UpdataFlag = true;
		PMCtrlPara0FigLabel.UpdataFlag = true;
		App.Menu.ModeCtrlForm.RefreshFlag &= ~MODECTRLFORM_PM;
	}
	
	if(App.Menu.ModeCtrlForm.RefreshFlag&MODECTRLFORM_SUMMER)
	{
		switch(App.Data.SysCtrlLine.TempInsideSummerLine)
		{
			case 25:
				SummerTemp0FigLabel.Disp = InverseDisp;
				SummerTemp4FigLabel.Disp = NormalDisp;
				SummerTemp3FigLabel.Disp = NormalDisp;
				SummerTemp2FigLabel.Disp = NormalDisp;
				SummerTemp1FigLabel.Disp = NormalDisp;
				break;
			case 26:
				SummerTemp1FigLabel.Disp = InverseDisp;
				SummerTemp4FigLabel.Disp = NormalDisp;
				SummerTemp3FigLabel.Disp = NormalDisp;
				SummerTemp2FigLabel.Disp = NormalDisp;
				SummerTemp0FigLabel.Disp = NormalDisp;
				break;
			case 27:
				SummerTemp2FigLabel.Disp = InverseDisp;
				SummerTemp4FigLabel.Disp = NormalDisp;
				SummerTemp3FigLabel.Disp = NormalDisp;
				SummerTemp1FigLabel.Disp = NormalDisp;
				SummerTemp0FigLabel.Disp = NormalDisp;
				break;
			case 28:
				SummerTemp3FigLabel.Disp = InverseDisp;
				SummerTemp4FigLabel.Disp = NormalDisp;
				SummerTemp2FigLabel.Disp = NormalDisp;
				SummerTemp1FigLabel.Disp = NormalDisp;
				SummerTemp0FigLabel.Disp = NormalDisp;
				break;
			case 29:
				SummerTemp4FigLabel.Disp = InverseDisp;
				SummerTemp3FigLabel.Disp = NormalDisp;
				SummerTemp2FigLabel.Disp = NormalDisp;
				SummerTemp1FigLabel.Disp = NormalDisp;
				SummerTemp0FigLabel.Disp = NormalDisp;
				break;
		}
		SummerTemp0FigLabel.UpdataFlag = true;
		SummerTemp1FigLabel.UpdataFlag = true;
		SummerTemp2FigLabel.UpdataFlag = true;
		SummerTemp3FigLabel.UpdataFlag = true;
		SummerTemp4FigLabel.UpdataFlag = true;
		App.Menu.ModeCtrlForm.RefreshFlag &= ~MODECTRLFORM_SUMMER;
	}
	
	if(App.Menu.ModeCtrlForm.RefreshFlag&MODECTRLFORM_WINTER)
	{
		switch(App.Data.SysCtrlLine.TempInsideWinterLine)
		{
			case 14:
				WinterTemp0FigLabel.Disp = InverseDisp;
				WinterTemp4FigLabel.Disp = NormalDisp;
				WinterTemp1FigLabel.Disp = NormalDisp;
				WinterTemp2FigLabel.Disp = NormalDisp;
				WinterTemp3FigLabel.Disp = NormalDisp;
				break;
			case 15:
				WinterTemp1FigLabel.Disp = InverseDisp;
				WinterTemp4FigLabel.Disp = NormalDisp;
				WinterTemp2FigLabel.Disp = NormalDisp;
				WinterTemp3FigLabel.Disp = NormalDisp;
				WinterTemp0FigLabel.Disp = NormalDisp;
				break;
			case 16:
				WinterTemp2FigLabel.Disp = InverseDisp;
				WinterTemp4FigLabel.Disp = NormalDisp;
				WinterTemp1FigLabel.Disp = NormalDisp;
				WinterTemp3FigLabel.Disp = NormalDisp;
				WinterTemp0FigLabel.Disp = NormalDisp;
				break;
			case 17:
				WinterTemp3FigLabel.Disp = InverseDisp;
				WinterTemp4FigLabel.Disp = NormalDisp;
				WinterTemp1FigLabel.Disp = NormalDisp;
				WinterTemp2FigLabel.Disp = NormalDisp;
				WinterTemp0FigLabel.Disp = NormalDisp;
				break;
			case 18:
				WinterTemp4FigLabel.Disp = InverseDisp;
				WinterTemp1FigLabel.Disp = NormalDisp;
				WinterTemp2FigLabel.Disp = NormalDisp;
				WinterTemp3FigLabel.Disp = NormalDisp;
				WinterTemp0FigLabel.Disp = NormalDisp;
				break;
		}
		WinterTemp0FigLabel.UpdataFlag = true;
		WinterTemp1FigLabel.UpdataFlag = true;
		WinterTemp2FigLabel.UpdataFlag = true;
		WinterTemp3FigLabel.UpdataFlag = true;
		WinterTemp4FigLabel.UpdataFlag = true;
		App.Menu.ModeCtrlForm.RefreshFlag &= ~MODECTRLFORM_WINTER;
	}
	
	if(App.Menu.ModeCtrlForm.RefreshFlag&MODECTRLFORM_CO2)
	{
		switch(App.Data.SysCtrlLine.CO2InsideLine)
		{
			case 800:
				Co2Para0FigLabel.Disp= InverseDisp;
				Co2Para1FigLabel.Disp= NormalDisp;
				Co2Para2FigLabel.Disp= NormalDisp;
				break;
			case 1000:
				Co2Para1FigLabel.Disp= InverseDisp;
				Co2Para0FigLabel.Disp= NormalDisp;
				Co2Para2FigLabel.Disp= NormalDisp;
				break;
			case 1200:
				Co2Para2FigLabel.Disp= InverseDisp;
				Co2Para0FigLabel.Disp= NormalDisp;
				Co2Para1FigLabel.Disp= NormalDisp;
				break;
		}
		Co2Para0FigLabel.UpdataFlag=true;
		Co2Para1FigLabel.UpdataFlag=true;
		Co2Para2FigLabel.UpdataFlag=true;
		App.Menu.ModeCtrlForm.RefreshFlag &= ~MODECTRLFORM_CO2;
	}


	if((App.KeyBtn.Focus)&&(Position != App.KeyBtn.Position))
	{
		if(App.KeyBtn.Position>0)
		{
			DotModeCtrl.Disp = NormalDisp;
			DotEreaseModeCtrl.Disp = NormalDisp;
			DotEreaseModeCtrl.Coord = DotModeCtrl.Coord;
			DotModeCtrl.Coord = DotLocArrayModeCtrl[App.KeyBtn.Position-1];
			DotModeCtrl.UpdataFlag =true;
			if(Position)
				DotEreaseModeCtrl.UpdataFlag =true;
		}
		else
		{
			DotEreaseModeCtrl.Coord = DotModeCtrl.Coord;
			DotModeCtrl.Coord = DotLocArrayModeCtrl[0];
			DotEreaseModeCtrl.UpdataFlag =true;
		}
		Position = App.KeyBtn.Position;
	}
	if(App.Menu.ModeCtrlForm.RefreshFlag&FORMSCREEN_NEW)
	{
		App.Menu.ModeCtrlForm.RefreshFlag =0;
		
		DotModeCtrl.Coord = DotLocArrayModeCtrl[0];
		DotModeCtrl.Disp = Invisible;
		DotEreaseModeCtrl.Coord = DotModeCtrl.Coord;
		DotEreaseModeCtrl.Disp = Invisible;
		
		App.KeyBtn.Focus = FOCUS_ONFORM;
		App.KeyBtn.ParaTitle = FORM_FOCUS;
		App.KeyBtn.Position = FORM_FOCUS;
		Position =0;
		
//#ifdef __RPMCTRL_OFF
//		ManuCharTextBox.Disp
		App.Menu.ModeCtrlForm.RefreshFlag &= ~FORMSCREEN_NEW;
		System.Gui.LoadForm(&App.Menu.ModeCtrlForm);
		System.Device.Lcd.LCD_ClearScreen(1);

	}
}

void RefreshFaultInfoForm(void)
{
	if(App.Menu.FaultInfoForm.RefreshFlag&FORMSCREEN_NEW)
	{
				
		App.KeyBtn.Focus = FOCUS_ONFORM;
		App.KeyBtn.Position = FORM_FOCUS;
		App.Menu.FaultInfoForm.RefreshFlag &= ~FORMSCREEN_NEW;
		System.Gui.LoadForm(&App.Menu.FaultInfoForm);
		AlertIconFigLabel.UpdataFlag = false;
		NormalAlertTextLabel.UpdataFlag = false;
		FilterAlertIconFigLabel.UpdataFlag = false;
		FilterAlertTextLabel.UpdataFlag = false;
		CoreAlertTextLabel.UpdataFlag = false;
		System.Device.Lcd.LCD_ClearScreen(1);
		if(App.SysFault.FaultFlag&COREFAULT_FLAG)
		{
//			App.Menu.FaultInfoForm.RefreshFlag |=FAULTFORM_COREALERTTEXT;
			App.KeyBtn.ParaTitle = PARATITLE_COREFAULT;
			
			AlertIconFigLabel.Disp = NormalDisp;
			AlertIconFigLabel.UpdataFlag = true;
#ifdef __HAYDN_CTRL
			CoreAlertTextLabel.Disp = NormalDisp;
			CoreAlertTextLabel.UpdataFlag = true;
//			NormalAlertTextLabel.Disp = Invisible;
//			FilterAlertIconFigLabel.Disp = Invisible;
//			FilterAlertTextLabel.Disp = Invisible;
#endif
		}
		else if(App.SysFault.FaultFlag&NORMALFAULT_FLAG)
		{
//			App.Menu.FaultInfoForm.RefreshFlag |=FAULTFORM_NOREMALALERTTEXT;
			App.KeyBtn.ParaTitle = PARATITLE_NORMALFAULT;
			AlertIconFigLabel.Disp = NormalDisp;
			AlertIconFigLabel.UpdataFlag = true;
#ifdef __HAYDN_CTRL
			NormalAlertTextLabel.Disp = NormalDisp;
			NormalAlertTextLabel.UpdataFlag = true;
//			CoreAlertTextLabel.Disp = Invisible;
//			FilterAlertIconFigLabel.Disp = Invisible;
//			FilterAlertTextLabel.Disp = Invisible;
#endif
		}
//		else if(App.SysFault.FaultFlag&FILTER_CHECKFLAG)
//		{
////			App.Menu.FaultInfoForm.RefreshFlag |=FAULTFORM_FILTERALERTTEXT;
//			App.KeyBtn.ParaTitle = PARATITLE_FILTER;
//			FilterAlertIconFigLabel.Disp = NormalDisp;
//			FilterAlertIconFigLabel.UpdataFlag = true;
//#ifndef LANGJIE_CTRL
//			FilterAlertTextLabel.Disp = NormalDisp;
//			FilterAlertTextLabel.UpdataFlag = true;
////			NormalAlertTextLabel.Disp = Invisiblep;
////			CoreAlertTextLabel.Disp = Invisible;
////			AlertIconFigLabel.Disp = Invisible;
//#endif
//		}
	}

}

/*******************************************************************************
* 函数名	: MenuTask
* 描述	    : MenuTask任务，必须为最低优先级任务，解析窗体Form内的各个控件用于显示
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************

*******************************************************************************/
void MenuTask (void)
{
    InitMenu();
		App.Menu.FocusFormPointer->RefreshDisp(); 
    while(true)             //大循环
    {
			
			System.Device.Iwdog.IwdogReload();
				App.Menu.FocusFormPointer->RefreshDisp(); 
//			if(App.Data.SysCtrlPara.Power != POWER_OFF)
//			if(App.SysCtrlStatus.Power != POWER_OFF)
			{
				System.Device.Iwdog.IwdogReload();
				System.Gui.Parse(App.Menu.FocusFormPointer);
			}
			
//			DelayMsNull(2);       //延时100mS，降低显示速度
    }
}



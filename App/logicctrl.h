#ifndef __LOGICCTRL_H
	#define __LOGICCTRL_H

#ifdef HD_GJ_250T
	#define MACHINE_TYPE     								Capital_T
	#define MACHINE_CAPACITY     							HG_250
	#define BYPASS_ANGLE												30
	#define __BYPASS_INVERSE
	
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								200
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											9400  //100%
	#define PWM_DUTYOCCUPY_MOTO1ACT											5000    //100-60
	#define PWM_DUTYOCCUPY_MOTO1MIN											5000    //70%

	#define PWM_DUTYOCCUPY_MOTO2MIN											7800    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7400    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							850  //82-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							950  //86-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1050  //90-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1150  //94-70
	
//	#define RPM_MOTO1_TOP							(uint16_t)1235
//	#define RPM_MOTO2_TOP							(uint16_t)1000
//	#define DISP_AIRFLOW_MIN					DISP_AIRFLOW_MIN_250
//	#define DISP_AIRFLOW_VARY					DISP_AIRFLOW_VARY_250
#ifdef __RPMCTRL_OFF
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1600	
	
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
	
#else
	
	#define RPM_MOTO1_MAX							(uint16_t)1150		//1500
	#define RPM_MOTO1_MIN							(uint16_t)450
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)650		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)550		//1350
	#define RPM_MOTO2_MIN							(uint16_t)350
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+200)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+200)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-150)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-150)
#endif

#endif

#ifdef HD_GJ_350TEST
	#define MACHINE_TYPE     								Capital_T
	#define MACHINE_CAPACITY     							HG_350
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												150
	
	#define PWM_DUTYOCCUPY_MOTO1ACT											2000    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											8000    //70%
	#define PWM_DUTYOCCUPY_MOTO1MAX				(PWM_DUTYOCCUPY_MOTO1MIN+PWM_DUTYOCCUPY_MOTO1ACT)

	#define PWM_DUTYOCCUPY_MOTO2MIN						8000    
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06 	1000							
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08 	1200							
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10		1400					
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12		2000						
	#define PWM_DUTYOCCUPY_MOTO2STOP			7500							
	
	#define RPM_MOTO1_TOP							(uint16_t)1700
	#define RPM_MOTO2_TOP							(uint16_t)1700	
	#define RPM_MOTO1_BOTTOM							(uint16_t)(150)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(150)
	
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
	

#endif

#ifdef HD_GJ_500B		/*2016.03.11, BypassInverse,500A-former*/
	#define MACHINE_TYPE     								Capital_B
	#define MACHINE_CAPACITY     							HG_500
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												40
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_MOTO1ACT											1500    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7600    //70%

	#define PWM_DUTYOCCUPY_MOTO2MIN											7600    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP							      7500    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							600  //83-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							900  //85-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1000  //87-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1200  //89-78
	
 //	#define RPM_MOTO1_TOP							(uint16_t)1100
//	#define RPM_MOTO2_TOP							(uint16_t)950
	
	#define RPM_MOTO1_MAX							(uint16_t)1150		//1500
	#define RPM_MOTO1_MIN							(uint16_t)450
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1150		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)900		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)650		//1350
	#define RPM_MOTO2_MIN						  	(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+150)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+150)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-150)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-150)
	
#endif

#ifdef HD_GJ_500C		/*2016.03.11, BypassInverse,500b-former*/
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_500
	#define BYPASS_ANGLE												150
	#define DISP_AIRFLOW_MIN								100
	#define DISP_AIRFLOW_VARY								400
	#define AIRFLOW_TOTAL_STEPS									(20)
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_OUT_MOTO1ACT											(1500)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1ACT											(1120)    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7800    //70%
	#define PWM_DUTYOCCUPY_OUT_MOTO1STEP											(PWM_DUTYOCCUPY_OUT_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1STEP											(PWM_DUTYOCCUPY_IN_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											7600    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP							      7500    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							600  //83-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							900  //85-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1000  //87-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1200  //89-78
	
//	#define RPM_MOTO1_TOP							(uint16_t)1100
//	#define RPM_MOTO2_TOP							(uint16_t)900
	
	#define RPM_MOTO1_OUT_MAX							(uint16_t)1150		//1130
	#define RPM_MOTO1_IN_MAX							(uint16_t)790		//1130
	#define RPM_MOTO1_MIN							(uint16_t)450
	#define RPM_MOTO1_OUT_ACT							(RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_IN_ACT							(RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_OUT_STEP						(uint16_t)((RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	#define RPM_MOTO1_IN_STEP						(uint16_t)((RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	
	#define RPM_MOTO2_MAX12							(uint16_t)900		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)740		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)620		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)520		//1350
	#define RPM_MOTO2_MIN								(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350

	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_OUT_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM					(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM					(uint16_t)(RPM_MOTO2_STOP-50)   
/*****500disp @440act  *******************/
//	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
//	#define PWM_DUTYOCCUPY_MOTO1ACT											1615    //100-70
//	#define PWM_DUTYOCCUPY_MOTO1MIN											8100    //70%

//	#define PWM_DUTYOCCUPY_MOTO2MIN											7700    //70%
//	#define PWM_DUTYOCCUPY_MOTO2STOP							      7500    //70%
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							510  //83-82
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							765  //85-82
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							850  //87-82
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1020  //89-78
//	
//	#define RPM_MOTO1_TOP							(uint16_t)960
//	#define RPM_MOTO2_TOP							(uint16_t)760
//	
//	#define RPM_MOTO1_MAX							(uint16_t)920		//1500
//	#define RPM_MOTO1_MIN							(uint16_t)450
//	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
//	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
//	
//	#define RPM_MOTO2_MAX12							(uint16_t)750		//1350
//	#define RPM_MOTO2_MAX10							(uint16_t)630		//1350
//	#define RPM_MOTO2_MAX08							(uint16_t)530		//1350
//	#define RPM_MOTO2_MAX06							(uint16_t)460		//1350
//	#define RPM_MOTO2_MIN							(uint16_t)380

#endif

#ifdef HD_GJ_500A		/*2016.03.11, BypassInverse,500b-former*/
	#define MACHINE_TYPE     								Capital_A
	#define MACHINE_CAPACITY     							HG_500
	#define BYPASS_ANGLE		150
	#define DISP_AIRFLOW_MIN								100
	#define DISP_AIRFLOW_VARY								400
	#define AIRFLOW_TOTAL_STEPS									(20)

	#define PWM_DUTYOCCUPY_MOTO1MAX											9100  //100%
	#define PWM_DUTYOCCUPY_OUT_MOTO1ACT									(1500)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1ACT									(1500)    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7600    //70%
	#define PWM_DUTYOCCUPY_OUT_MOTO1STEP								(PWM_DUTYOCCUPY_OUT_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1STEP									(PWM_DUTYOCCUPY_IN_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70

	#define PWM_DUTYOCCUPY_MOTO2MIN											7600    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7400    //70%	
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							600  //82-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							900  //86-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1000  //90-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1150  //94-70

//	#define RPM_MOTO1_TOP							(uint16_t)1100
//	#define RPM_MOTO2_TOP							(uint16_t)950
	
	
	#define RPM_MOTO1_OUT_MAX							(uint16_t)1150		//1130
	#define RPM_MOTO1_IN_MAX							(uint16_t)950		//1130
	#define RPM_MOTO1_MIN									(uint16_t)450
	#define RPM_MOTO1_OUT_ACT							(RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_IN_ACT							(RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_OUT_STEP						(uint16_t)((RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	#define RPM_MOTO1_IN_STEP						(uint16_t)((RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)

	
	#define RPM_MOTO2_MAX12							(uint16_t)1050		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)900		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)650		//1350
	#define RPM_MOTO2_MIN							(uint16_t)350
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_OUT_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM					(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM					(uint16_t)(RPM_MOTO2_STOP-50)
	
#endif

#ifdef HD_GJ_350B
	#define MACHINE_TYPE     								Capital_B
	#define MACHINE_CAPACITY     							HG_350
	#define BYPASS_ANGLE												40
	
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								300
	#define PWM_DUTYOCCUPY_MOTO1ACT											2000    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											8000    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											8000    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7800    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 700  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1050  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1350  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1650  //96.75-80


  #define RPM_MOTO1_MAX							(uint16_t)1285		//1500
	#define RPM_MOTO1_MIN							(uint16_t)600
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1200		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)1050		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)850		//1350
	#define RPM_MOTO2_MIN							(uint16_t)450
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM					(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM					(uint16_t)(RPM_MOTO2_STOP-50)
#endif

#ifdef HD_GJ_350C
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_350
	#define BYPASS_ANGLE												30
	
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								300
	
/* for new 49moto*/	
//	#define PWM_DUTYOCCUPY_MOTO1ACT											2200    //100-70
//	#define PWM_DUTYOCCUPY_MOTO1MIN											7200    //70%
//	
//	#define PWM_DUTYOCCUPY_MOTO2MIN											7200    //70%
//	#define PWM_DUTYOCCUPY_MOTO2STOP										7000    //70%
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 900  //87-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1100  //90.25-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1300  //93.5-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1500  //96.75-80

	#define AIRFLOW_TOTAL_STEPS									(20)
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_OUT_MOTO1ACT											(2000)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1ACT											(1500)    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											8000    //70%
	#define PWM_DUTYOCCUPY_OUT_MOTO1STEP											(PWM_DUTYOCCUPY_OUT_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1STEP											(PWM_DUTYOCCUPY_IN_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70

	#define PWM_DUTYOCCUPY_MOTO2MIN											8000    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7800    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 700  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							950  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1150  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1350  //96.75-80

#ifdef __RPMCTRL_OFF
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1600	
	
	
#else
	#define RPM_MOTO1_OUT_MAX							(uint16_t)1420		//1130
	#define RPM_MOTO1_IN_MAX							(uint16_t)1050		//1130
	#define RPM_MOTO1_MIN									(uint16_t)500
	#define RPM_MOTO1_OUT_ACT							(RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_IN_ACT							(RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_OUT_STEP						(uint16_t)((RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	#define RPM_MOTO1_IN_STEP						(uint16_t)((RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)

	
	#define RPM_MOTO2_MAX12							(uint16_t)1050		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)850		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)750		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_OUT_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)   //FOR V2.20   & 150-200 for V2.18-19
#endif

#endif

#ifdef HD_GJ_250C
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_250
	#define BYPASS_ANGLE												30
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								200
	#define AIRFLOW_TOTAL_STEPS									(20)
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											9400  //100%
	#define PWM_DUTYOCCUPY_OUT_MOTO1ACT											(2000)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1ACT											(1500)    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7400    //70%
	#define PWM_DUTYOCCUPY_OUT_MOTO1STEP											(PWM_DUTYOCCUPY_OUT_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1STEP											(PWM_DUTYOCCUPY_IN_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70

	#define PWM_DUTYOCCUPY_MOTO2MIN											7400    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7400    //70%	
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							850  //82-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							950  //86-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1050  //90-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1150  //94-70
	
//	#define RPM_MOTO1_TOP							(uint16_t)1235
//	#define RPM_MOTO2_TOP							(uint16_t)1000
//	#define DISP_AIRFLOW_MIN					DISP_AIRFLOW_MIN_250
//	#define DISP_AIRFLOW_VARY					DISP_AIRFLOW_VARY_250
#ifdef __RPMCTRL_OFF
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1600	
	
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
	
#else
	
	#define RPM_MOTO1_OUT_MAX							(uint16_t)1150		//1130
	#define RPM_MOTO1_IN_MAX							(uint16_t)950		//1130
	#define RPM_MOTO1_MIN									(uint16_t)450
	#define RPM_MOTO1_OUT_ACT							(RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_IN_ACT							(RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_OUT_STEP						(uint16_t)((RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	#define RPM_MOTO1_IN_STEP						(uint16_t)((RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)

	
	#define RPM_MOTO2_MAX12							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)650		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)550		//1350
	#define RPM_MOTO2_MIN							(uint16_t)350
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_OUT_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)
#endif

#endif

#ifdef HD_GJ_250B
	#define MACHINE_TYPE     								Capital_B
	#define MACHINE_CAPACITY     							HG_250
	#define BYPASS_ANGLE												30
	#define MACHINE_CAPACITY_250     						250
	#define DISP_AIRFLOW_MIN_250								50
	#define DISP_AIRFLOW_VARY_250								200
	
	#define PWM_DUTYOCCUPY_MOTO1MAX						9200  //92% 3.94V
	#define PWM_DUTYOCCUPY_MOTO1ACT						2200    //100-60
	#define PWM_DUTYOCCUPY_MOTO1MIN						7000    //70%	
	
	#define PWM_DUTYOCCUPY_MOTO2MAX							7800  //100%
	#define PWM_DUTYOCCUPY_MOTO2MIN							7000    //70%  Normal
	#define PWM_DUTYOCCUPY_MOTO2STOP							6000    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							1200  //83-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1600  //85-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							2000  //87-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							2200  //89-65
	
//	#define RPM_MOTO1_TOP							(uint16_t)1280
//	#define RPM_MOTO2_TOP							(uint16_t)1200
	
	#define RPM_MOTO1_MAX							(uint16_t)1280		//1500
	#define RPM_MOTO1_MIN							(uint16_t)550
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1200		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)900		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)800		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)700		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)   //FOR V2.20   & 150-200 for V2.18-19
#endif


#ifdef HD_GJ_350A
	#define MACHINE_TYPE     								Capital_A
	#define MACHINE_CAPACITY     							HG_350
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												30
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								300
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_MOTO1ACT											2500    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7500    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											7500    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7500    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							1200  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1450  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1750  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							2050  //96.75-80

/* Liuxiaofeng V2.22  
	#define PWM_DUTYOCCUPY_MOTO1ACT											1800    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7700    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											7700    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7700    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 800  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1050  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1350  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1550  //96.75-80
***************************************/
 #ifdef __RPMCTRL_OFF
 
	#define RPM_MOTO1_TOP							(uint16_t)1500
	#define RPM_MOTO2_TOP							(uint16_t)1500	
	
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
	
 #else
 

  #define RPM_MOTO1_MAX							(uint16_t)1300		//1500
	#define RPM_MOTO1_MIN							(uint16_t)600
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1250		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)1100		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)850		//1350
	#define RPM_MOTO2_MIN							  (uint16_t)350
	#define RPM_MOTO2_STOP							(uint16_t)350
	
/* Liuxiaofeng V2.22  ************************

  #define RPM_MOTO1_MAX							(uint16_t)1150		//1500
	#define RPM_MOTO1_MIN							(uint16_t)450
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1050		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)850		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)750		//1350
	#define RPM_MOTO2_MIN							  (uint16_t)350
	#define RPM_MOTO2_STOP							(uint16_t)350
*****************************************/

	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM					(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM					(uint16_t)(RPM_MOTO2_STOP-50)  
 #endif
#endif


#ifdef HD_GJ_250A
	#define MACHINE_TYPE     								Capital_A
	#define MACHINE_CAPACITY     							HG_250
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												30
	#define MACHINE_CAPACITY_250     						250
	#define DISP_AIRFLOW_MIN_250								50
	#define DISP_AIRFLOW_VARY_250								200
	
	#define PWM_DUTYOCCUPY_MOTO1MAX						9200  //92% 3.94V
	#define PWM_DUTYOCCUPY_MOTO1ACT						2200    //100-60
	#define PWM_DUTYOCCUPY_MOTO1MIN						7000    //70%	
	
	#define PWM_DUTYOCCUPY_MOTO2MAX							7800  //100%
	#define PWM_DUTYOCCUPY_MOTO2MIN							6800    //70%  Normal
	#define PWM_DUTYOCCUPY_MOTO2STOP							6750    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							1200  //83-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1600  //85-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							2000  //87-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							2200  //89-65
	
 #ifdef __RPMCTRL_OFF
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
 
	#define RPM_MOTO1_TOP							(uint16_t)1500
	#define RPM_MOTO2_TOP							(uint16_t)1500	
	#define RPM_MOTO1_BOTTOM							(uint16_t)350
	#define RPM_MOTO2_BOTTOM							(uint16_t)350
	
 #else
//	
//	#define RPM_MOTO1_TOP							(uint16_t)1230		//1500
//	#define RPM_MOTO2_TOP							(uint16_t)1150		//1350
	
	#define RPM_MOTO1_MAX							(uint16_t)1230		//1500
	#define RPM_MOTO1_MIN							(uint16_t)550
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1150		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)850		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)650		//1350
	#define RPM_MOTO2_MIN							  (uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)  //FOR V2.20   & 150-200 for V2.18-19
	
 #endif
#endif


#ifdef HD_GJ_150C
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_150
	#define BYPASS_ANGLE											30
//	#define MACHINE_CAPACITY_150     					150
//	#define DISP_AIRFLOW_MIN_150							50
//	#define DISP_AIRFLOW_VARY_150							100
	
/* Moto2.0 1800rpm,37W   */
	#define PWM_DUTYOCCUPY_MOTO1MAX						9000  //92% 3.94V
//	#define PWM_DUTYOCCUPY_MOTO1ACT_150						1400    //100-60  Normal
	#define PWM_DUTYOCCUPY_MOTO1ACT						1800    //100-60
	#define PWM_DUTYOCCUPY_MOTO1MIN						7000    //70%	
	
	#define PWM_DUTYOCCUPY_MOTO2MAX							7800  //100%
	#define PWM_DUTYOCCUPY_MOTO2MIN							7000    //70%  Normal
	#define PWM_DUTYOCCUPY_MOTO2STOP							6750    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							600  //83-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							800  //85-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							900  //87-65
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1100  //89-65

 #ifdef __RPMCTRL_OFF
	
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0	
	
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1600	
//	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+200)
//	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+200)
	#define RPM_MOTO1_BOTTOM							(uint16_t)350  //(RPM_MOTO1_MIN-150)
	#define RPM_MOTO2_BOTTOM							(uint16_t)350  //(RPM_MOTO2_STOP-150)
 #else
	
	#define RPM_MOTO1_MAX							(uint16_t)1550		//1500
	#define RPM_MOTO1_MIN							(uint16_t)550
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1000		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)850		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)650		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)  
	
#endif	
//	#define DISP_AIRFLOW_MIN									DISP_AIRFLOW_MIN_150
//	#define DISP_AIRFLOW_VARY									DISP_AIRFLOW_VARY_150
#endif

#ifdef HD_GJ_800C		/*2016.03.11, BypassInverse,500b-former*/
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_800
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												150
	
	#define DISP_AIRFLOW_MIN								200
	#define DISP_AIRFLOW_VARY								600
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_MOTO1ACT											1900    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											8100    //70%

	#define PWM_DUTYOCCUPY_MOTO2MIN											7700    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP							      7500    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							600  //83-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							900  //85-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1000  //87-82
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1200  //89-78
	
	#define RPM_MOTO1_MAX							(uint16_t)1600		//1500
	#define RPM_MOTO1_MIN							(uint16_t)450
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)740		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)620		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)520		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)  

#endif

#ifdef HD_GJ_360C
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HG_350
	#define BYPASS_ANGLE												30
	
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								300
	
/* for new 49moto*/	
	#define PWM_DUTYOCCUPY_MOTO1ACT											2200    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7200    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											7200    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7000    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 900  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1100  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1300  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1500  //96.75-80

//	#define PWM_DUTYOCCUPY_MOTO1ACT											2000    //100-70
//	#define PWM_DUTYOCCUPY_MOTO1MIN											8000    //70%
//	
//	#define PWM_DUTYOCCUPY_MOTO2MIN											8000    //70%
//	#define PWM_DUTYOCCUPY_MOTO2STOP										7800    //70%
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							 700  //87-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							950  //90.25-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							1150  //93.5-80
//	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							1350  //96.75-80
//	#define RPM_MOTO1_TOP							(uint16_t)1385		//1500
//	#define RPM_MOTO2_TOP							(uint16_t)1100		//1350

#ifdef __RPMCTRL_OFF
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1600	
	
	
#else

	#define RPM_MOTO1_MAX							(uint16_t)1350		//1500
	#define RPM_MOTO1_MIN							(uint16_t)500
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1500		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)950		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)850		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)750		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)   //FOR V2.20   & 150-200 for V2.18-19
#endif

#endif

#ifdef HD_GJ_600Z
	#define MACHINE_TYPE     								Capital_Z
	#define MACHINE_CAPACITY     							HG_350

#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												110
	
	#define DISP_AIRFLOW_MIN								50
	#define DISP_AIRFLOW_VARY								300
	
/* for new 49moto*/	
	#define PWM_DUTYOCCUPY_MOTO1ACT											2000    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											7200    //70%
	
	#define PWM_DUTYOCCUPY_MOTO2MIN											7200    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										7000    //70%
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							1600  //87-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							1800  //90.25-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							2000  //93.5-80
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							2200  //96.75-80


#ifdef __RPMCTRL_OFF
	
	#define RPM_MOTO1_MAX							(uint16_t)0		//1500
	#define RPM_MOTO1_MIN							(uint16_t)0
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)0		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)0		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)0
 
	#define RPM_MOTO1_TOP							(uint16_t)1800
	#define RPM_MOTO2_TOP							(uint16_t)1800	
	
	
#else

	#define RPM_MOTO1_MAX							(uint16_t)1200		//1500
	#define RPM_MOTO1_MIN							(uint16_t)400
	#define RPM_MOTO1_ACT							(RPM_MOTO1_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_STEP						(uint16_t)((RPM_MOTO1_MAX-RPM_MOTO1_MIN)/20)
	
	#define RPM_MOTO2_MAX12							(uint16_t)1300		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)1200		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)1000		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)900		//1350
	#define RPM_MOTO2_MIN							(uint16_t)400
	#define RPM_MOTO2_STOP							(uint16_t)350
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)   //FOR V2.20   & 150-200 for V2.18-19
#endif
#endif

#ifdef HD_BG_80
	#define MACHINE_TYPE     								Capital_C
	#define MACHINE_CAPACITY     							HB_80
	#define __BYPASS_INVERSE
	#define BYPASS_ANGLE												40
	#define DISP_AIRFLOW_MIN								0
	#define DISP_AIRFLOW_VARY								100
	#define AIRFLOW_TOTAL_STEPS									(20)
	
	#define PWM_DUTYOCCUPY_MOTO1MAX											10000  //100%
	#define PWM_DUTYOCCUPY_OUT_MOTO1ACT											(10000)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1ACT											(10000)    //100-70
	#define PWM_DUTYOCCUPY_MOTO1MIN											0    //70%
	#define PWM_DUTYOCCUPY_OUT_MOTO1STEP											(PWM_DUTYOCCUPY_OUT_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70
	#define PWM_DUTYOCCUPY_IN_MOTO1STEP											(PWM_DUTYOCCUPY_IN_MOTO1ACT/AIRFLOW_TOTAL_STEPS)    //100-70

	#define PWM_DUTYOCCUPY_MOTO2MIN											0    //70%
	#define PWM_DUTYOCCUPY_MOTO2STOP										3000    //70%	
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO06							7000  //82-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO08							8000  //86-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO10							9000  //90-70
	#define PWM_DUTYOCCUPY_MOTO2_RATE10TO12							10000  //94-70
	
//	#define RPM_MOTO1_TOP							(uint16_t)1235
//	#define RPM_MOTO2_TOP							(uint16_t)1000
//	#define DISP_AIRFLOW_MIN					DISP_AIRFLOW_MIN_250
//	#define DISP_AIRFLOW_VARY					DISP_AIRFLOW_VARY_250
	
	#define RPM_MOTO1_OUT_MAX							(uint16_t)1400		//1130
	#define RPM_MOTO1_IN_MAX							(uint16_t)1100		//1130
	#define RPM_MOTO1_MIN									(uint16_t)0
	#define RPM_MOTO1_OUT_ACT							(RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_IN_ACT							(RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)
	#define RPM_MOTO1_OUT_STEP						(uint16_t)((RPM_MOTO1_OUT_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)
	#define RPM_MOTO1_IN_STEP						(uint16_t)((RPM_MOTO1_IN_MAX-RPM_MOTO1_MIN)/AIRFLOW_TOTAL_STEPS)

	
	#define RPM_MOTO2_MAX12							(uint16_t)1050		//1350
	#define RPM_MOTO2_MAX10							(uint16_t)900		//1350
	#define RPM_MOTO2_MAX08							(uint16_t)750		//1350
	#define RPM_MOTO2_MAX06							(uint16_t)600		//1350
	#define RPM_MOTO2_MIN							(uint16_t)0
	#define RPM_MOTO2_STOP							(uint16_t)510
	
	#define RPM_MOTO1_TOP							(uint16_t)(RPM_MOTO1_OUT_MAX+50)
	#define RPM_MOTO2_TOP							(uint16_t)(RPM_MOTO2_MAX12+50)
	#define RPM_MOTO1_BOTTOM							(uint16_t)(RPM_MOTO1_MIN-50)
	#define RPM_MOTO2_BOTTOM							(uint16_t)(RPM_MOTO2_STOP-50)
#endif

#define 	AIRFLOWCTRL_CTRLSTEP_PERSENT 		5
#endif

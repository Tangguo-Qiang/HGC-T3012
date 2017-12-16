/*
********************************************************************************
*文件名     : device_lcd.c
*作用       : 480*320 彩屏
*原理       : 128*64字库黑白屏，分为四行，字库汉字部分支持GB2312编码，
*           : Keil编译器直接支持GB2312，汉字每个16*16点阵，一行可以显示8个汉字
*           : 字母支持ASCII标准，每个8*16，一行可以显示16个字母
********************************************************************************

********************************************************************************
*/



#include "system.h"
#include "drive.h"

#define LCD_BKLIGHT_PIN   GPIO_Pin_12
#define LCD_BKLIGHT_PORT  GPIOD         
//#define LCD_BL_ON     		GPIO_SetBits(LCD_BKLIGHT_PORT,LCD_BKLIGHT_PIN); 
//#define LCD_BL_OFF     		0GPIO_ResetBits(LCD_BKLIGHT_PORT,LCD_BKLIGHT_PIN); 

#define LCD_RST_PIN   		GPIO_Pin_4
#define LCD_RST_PORT  		GPIOC         




/* 选择BANK1-BORSRAM1 连接 TFT，地址范围为0X60000000~0X63FFFFFF
 * FSMC_A16 接LCD的DC(寄存器/数据选择)脚
 * 16 bit => FSMC[24:0]对应HADDR[25:1](AHB[25:1])
 * 寄存器基地址 = 0X60000000
 * RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * 当选择不同的地址线时，地址要重新计算。
 */
#define Bank1_LCD_D    ((uint)0x60020000)       //Disp Data ADDR
#define Bank1_LCD_C    ((uint)0x60000000)	   //Disp Reg ADDR

/*色彩数据存储格式 0xRRRRRGGGGGGGBBBBB */

/*选定LCD指定寄存器*/
#define LCD_WR_REG(index)    ((*(__IO uint16_t *) (Bank1_LCD_C)) = ((uint16_t)index))
/*往LCD GRAM写入数据*/
#define LCD_WR_Color(val)       {(*(__IO uint16_t *) (Bank1_LCD_D)) = ((uint16_t)(val));}
/*将LCD中的index寄存器设置为val值, LCD_HX8347I_Reg(index,val)*/
#define LCD_WR_CMD(index,val)  {(*(__IO uint16_t *) (Bank1_LCD_C)) = ((uint16_t)index);	(*(__IO uint16_t *) (Bank1_LCD_D)) = ((uint16_t)(val));}while(0)
#define	LCD_WR_Data(val)	LCD_WR_Color(val)


#define SPI_FLASH                   SPI1
#define SPI_FLASH_CLK               RCC_APB2Periph_SPI1
#define SPI_FLASH_GPIO              GPIOA
#define SPI_FLASH_GPIO_CLK          RCC_APB2Periph_GPIOA  
#define SPI_FLASH_PIN_SCK           GPIO_Pin_5
#define SPI_FLASH_PIN_MISO          GPIO_Pin_6
#define SPI_FLASH_PIN_MOSI          GPIO_Pin_7
#define SPI_FLASH_DMA               DMA1
#define SPI_FLASH_DMA_CLK           RCC_AHBPeriph_DMA1  
#define SPI_FLASH_Rx_DMA_Channel    DMA1_Channel2
#define SPI_FLASH_Rx_DMA_FLAG       DMA1_FLAG_TC2
#define SPI_FLASH_Tx_DMA_Channel    DMA1_Channel3
#define SPI_FLASH_Tx_DMA_FLAG       DMA1_FLAG_TC3  
#define SPI_FLASH_DR_Base           0x4001300C

#define FIG_FLASH_DR_Base           (uint32_t)00

static DMA_InitTypeDef    DMA_InitStruct_SpiRx,DMA_InitStruct_SpiTx;
uint8_t SPI_FLASH_Buffer_Tx[2] = {0xff,0xff};
uint16_t BackColor = 0x0000;

const uint16_t figbytes[NUM_Full]={1024,  
	10428, 7638, 882,  7150,	10428, /*5*/
	 7638,22110, 4154,  7370, 11122,	/*10*/
	14740,18760,22110,  3944,  7150,	/*15*/
	 1216,10428, 7638,  850,  2378,	 /*20*/ 
    288, 2574, 1848,   390,  2120,	/*25*/
   2812, 1092, 4488,   4488,   340, /*30*/ 
	  340,   7500,24640, 2464, 36190,/*35*/
	36190,  16192,16192,10428,  7638,  /*40*/  
	 7638,	 1204, 1204, 1204,  1204,/*45*/   
	 1204, 	 1204, 1204, 1204,  1204,/*50*/  
	 4020, 	 4020, 4020, 4020,  4020, /*55*/  
	 4020, 	 4020, 4020, 4020,  4020, /*60*/
	 4020,   4020, 4020, 4020,  4020,  /*65*/
	 4020,   4020, 4020,32640, 32640,  /*70*/
	32640,	32640, 32640,1568, 1568,  1568,  /*75*/
	15870,	  364,  532, 2628,  2628,  /*80*/
	 2628,	 2628, 2064, 2064,  2090,   /*85*/
   7248,	 7248, 6870, 6870,  6870, /*90*/
	 6870, 	 6870, 6870, 6870,  6870, /*95*/
   6870, 	 6870, 6870, 5880,  5880,/*100*/
	 5880,   2862, 2862, 2862,  2862,/*105*/ 
	 2862,   2862, 2376, 2376,  2376, /*110*/
	 2376,  2376,	 2376, 2376,  2376, /*115*/
	 2862,  2862,  2862, 2862,  2862,/*120*/
	 2862,  2862,	 2862, 2862,  2862, /*125*/
	 2862,  2862,  2862, 2862,  2862, /*130*/
	 2862,  2862,  2862, 2862,  2862, /*135*/
	 2576,  2576,  7360, 7360,  7360,  /*140*/
	 1680,  630,  1914, 8468,  6840, /*145*/
	63048, 40420, 61272,15028,340,
	 
};
	
extern void	SPI_FLASH_ReadFinish(void);
extern void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
	
#define DelayMsTick MainClock / 900
//#define DelayMsTick MainClock / 9000

static void Delay1ms(int times)
{
	while(times--)
		DelayNull(DelayMsTick);
}
	/*******************************************
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*********************************************/  
static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能FSMC时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* 使能FSMC对应相应管脚时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* 配置LCD背光控制管脚 PD12*/
    GPIO_InitStructure.GPIO_Pin = LCD_BKLIGHT_PIN;		
    GPIO_Init(LCD_BKLIGHT_PORT, &GPIO_InitStructure);
    LCD_BL_OFF;
	
    /* 配置LCD复位控制管脚 PC4  */
    GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN ; 	 
    GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);  		   
    
    /* 配置FSMC相对应的数据线,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* 配置FSMC相对应的控制线
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
   
//	GPIO_SetBits(GPIOA, GPIO_Pin_12);	//复位	 

	 
}
/*******************************************
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*********************************************/ 
static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
		FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;						   //配置读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 							   //配置写时序
		FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);

    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间，50MHz，40ns
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;   //总线恢复时间
    p.FSMC_CLKDivision = 0x00;			  //时钟分频
    p.FSMC_DataLatency = 0x00;			  //数据保持时间
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 //在地址数\据线不复用的情况下，ABCD模式的区别不大
											 //本成员配置只有使用扩展模式才有效
											
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;			 //NOR FLASH的BANK1
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	  //数据线与地址线不复用
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;				  //存储器类型NOR FLASH
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		  //数据宽度为16位
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	  //使用异步写模式，禁止突发模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //本成员的配置只在突发模式下有效，等待信号极性为低
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;				 //禁止非对齐突发模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;   //本成员配置仅在突发模式下有效。NWAIT信号在什么时期产生
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;			  //本成员的配置只在突发模式下有效，禁用NWAIT信号
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;			  //禁止突发写操作
   	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		   //写使能
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;		  //禁止扩展模式，扩展模式可以使用独立的读、写模式
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;						   //配置读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 							   //配置写时序
   
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* 使能 FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

/**********************************
 * 函数名：LCD_Rst
 * 描述  ：LCD 软件复位
 * 输入  : 无 
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无 
*************************************/

static void LCD_Rst(void)
{			
    GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);	 //低电平复位
    Delay1ms(30); 					   
    GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN );		 	 
    Delay1ms(30); 	
}

/**********************************
 * 函数名：LCD_RD_data
 * 描述  ：读 HX8347I RAM 数据
 * 输入  : 无 
 * 输出  ：读取的数据,16bit *
 * 举例  ：无
 * 注意  ：无
*************************************/
static uint16_t LCD_RD_data(void)
{
	ushort R,G,B,RGB=0 ;

//	LCD_WR_REG(0x22);
	RGB =(*(__IO uint16_t *) (Bank1_LCD_D));  /*FIRST READ OUT DUMMY DATA*/
	R =(*(__IO uint16_t *) (Bank1_LCD_D));  /*FIRST READ OUT DUMMY DATA*/
	B =(*(__IO uint16_t *) (Bank1_LCD_D));  /*FIRST READ OUT DUMMY DATA*/
	G = (R & 0x00FC)<<3;
	R = R&0xF800;
	B = (B & 0xF800)>>11;
	
	RGB = R|G;
	RGB|= B;


	return RGB ;

}

static void Lcd_data_start(void)
{
     LCD_WR_REG(0x2c);//开始写数据到GRAM
}


/**********************************
 * 函数名：Set_direction
 * 描述  ：设置HX8347IGRAM指针扫描方向
 * 输入  : 0： 横向扫描
 		   1： 纵向扫描
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
static void Set_direction(uint8_t option)
{

    switch(option)
    {
    case 0:
		    /*横屏*/
      break;
    case 1:
    		/*竖屏*/
      break;
        
    }

}

/**********************************
 * 函数名：LCD_open_windows
 * 描述  ：开窗(以x,y为坐标起点，长为len,高为wid)
 * 输入  : -x    窗户起点
           -y	   窗户起点
           -len  窗户长 
           -wid 窗户宽
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
static void LCD_open_windows(uint16_t x,uint16_t y,uint16_t len,uint16_t wid)
{                    

  LCD_WR_REG(0x2A);
	LCD_WR_Data(x>>8);	//列start 起始位置的高8位
  LCD_WR_Data(x&0x00ff);//起始位置的低8位
  LCD_WR_Data((x+len-1)>>8);	//end 结束位置的高8位
  LCD_WR_Data((x+len-1)&0x00ff);  //结束位置的低8位
		
  LCD_WR_REG(0x2B);
  LCD_WR_Data(y>>8);	//start row起始位置的高8位
  LCD_WR_Data(y&0x00ff);//起始位置的低8位
  LCD_WR_Data((y+wid-1)>>8);	//end 结束位置的高8位
  LCD_WR_Data((y+wid-1)&0x00ff);  //结束位置的低8位


      Lcd_data_start();     
}
/**********************************
 * 函数名：Lcd_init_conf
 * 描述  ：HX8347I LCD寄存器初始配置
 * 输入  : 无
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
static void Lcd_init_conf(void)
{
	Delay1ms(50);
	LCD_WR_REG(0x11);
	Delay1ms(50);
	LCD_WR_REG(0xF0);
	LCD_WR_Data(0XC3);
	LCD_WR_REG(0xF0);
	LCD_WR_Data(0X96);
	//Memory data access control
	//MY MX MV ML RGB - - -   
	LCD_WR_REG(0x36);
	LCD_WR_Data(0X28);
	LCD_WR_REG(0xB4);
	LCD_WR_Data(0X01);
	LCD_WR_REG(0xE8);
	LCD_WR_Data(0X40);
	LCD_WR_Data(0X8A);
	LCD_WR_Data(0X00);
	LCD_WR_Data(0X00);
	LCD_WR_Data(0X29);
	LCD_WR_Data(0X19);
	LCD_WR_Data(0Xa5);
	LCD_WR_Data(0X33);
	LCD_WR_REG(0xC1);//EQ
	LCD_WR_Data(0X06);
	LCD_WR_REG(0xC2);
	LCD_WR_Data(0XA7);
	LCD_WR_REG(0xC5);
	LCD_WR_Data(0X22);
	LCD_WR_REG(0xE0);
	LCD_WR_Data(0Xf0);
	LCD_WR_Data(0X09);
	LCD_WR_Data(0X0b);
	LCD_WR_Data(0X06);
	LCD_WR_Data(0X04);
	LCD_WR_Data(0X15);
	LCD_WR_Data(0X2f);
	LCD_WR_Data(0X54);
	LCD_WR_Data(0X42);
	LCD_WR_Data(0X3c);
	LCD_WR_Data(0X17);
	LCD_WR_Data(0X14);
	LCD_WR_Data(0X18);
	LCD_WR_Data(0X1b);
	LCD_WR_REG(0xE1);
	LCD_WR_Data(0Xf0);
	LCD_WR_Data(0X09);
	LCD_WR_Data(0X0b);
	LCD_WR_Data(0X06);
	LCD_WR_Data(0X04);
	LCD_WR_Data(0X03);
	LCD_WR_Data(0X2d);
	LCD_WR_Data(0X43);
	LCD_WR_Data(0X42);
	LCD_WR_Data(0X3b);
	LCD_WR_Data(0X16);
	LCD_WR_Data(0X14);
	LCD_WR_Data(0X17);
	LCD_WR_Data(0X1b);

	LCD_WR_REG(0xF0);
	LCD_WR_Data(0X3C);
	LCD_WR_REG(0xF0);
	LCD_WR_Data(0X69);
	Delay1ms(50);

	LCD_WR_REG(0x3A);//16bit RGB
	LCD_WR_Data(0X55);




  LCD_WR_REG(0x29);              //Start GRAM write 	

}

/*************************************API START*************************************************************/
static void spi_DMAInit(void)
{
  /* Enable DMA1 or/and DMA2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* SPI_FLASH_Rx_DMA_Channel configuration ---------------------------------*/
  DMA_InitStruct_SpiRx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiRx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiRx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;	
  DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiRx.DMA_MemoryBaseAddr = (uint32_t)Bank1_LCD_D;
  DMA_InitStruct_SpiRx.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct_SpiRx.DMA_Priority = DMA_Priority_High;
	
  /* SPI_FLASH_Tx_DMA_Channel configuration ---------------------------------*/
  DMA_InitStruct_SpiTx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiTx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiTx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiTx.DMA_M2M = DMA_M2M_Disable;

  DMA_InitStruct_SpiTx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiTx.DMA_MemoryBaseAddr = (uint32_t)SPI_FLASH_Buffer_Tx;
  DMA_InitStruct_SpiTx.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct_SpiTx.DMA_Priority = DMA_Priority_High;//DMA_Priority_Low;


}
/******************************************************
 * 函数名：LCD_Rectangle
 * 描述  ：在指定坐标处显示指定长宽的矩形
 * 输入  : -x1      横向矩形起点 0~319
           -y1      纵向矩形起点 0~239
		   -len     横向长度 0~319
		   -wid     纵向宽度 0~480
		   -rgb565  填充颜色
 * 输出  ：无
 * 举例  ：	LCD_Rectangle(10,10,120,160,0);
		    LCD_Rectangle(200,200,120,40,0);
 * 注意  ：	已测试
*********************************************************/    
static void LCD_ColorRectangle(LCD_RectangleTypeDef* rectangle)
{                    
   uint n, temp;
//	 Set_direction(0);	 						/*设定方向为横向*/
   LCD_open_windows(rectangle->col,rectangle->row,rectangle->width,rectangle->Height);   
   temp = (uint32_t)rectangle->Height;
	temp *= (uint32_t)rectangle->width;    
   for(n=0; n<temp; n++)
	 {
        LCD_WR_Data( rectangle->color );
	 }
	  
}

static void LCD_ClearScreen(byte menu)
{
	uint bytes,i;

//  bytes = 320*480;    
//    LCD_open_windows(0,0,480,320); 
//    
//    for (i=0;i<bytes;i++)            //pos表示1字模中第pos个字节,本程序中nbytes字节每字符
//    {
//					LCD_WR_Data(BLACK);       
//    }

	if(menu)
	{
		bytes = 480*(320-34);
		LCD_open_windows(0,32,480,(320-34)); 
	}		
  else
	{
		bytes = 320*480;
		LCD_open_windows(0,0,480,320); 
	}
//	SPI_FLASH_StartReadSequence(0);

  DMA_InitStruct_SpiRx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiRx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiRx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;	
  DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiRx.DMA_MemoryBaseAddr = (uint32_t)Bank1_LCD_D;
  DMA_InitStruct_SpiRx.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct_SpiRx.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)&BackColor;//SPI_FLASH_Buffer_Rx;
  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Enable;
	
	if(bytes>0x7fff)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
		bytes -= 0x7fff;
	}
	else if(bytes > 0)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = bytes;
		bytes = 0;
	}
	else
	{
		DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
		DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
		return;
	}
  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);	
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
	
	if(bytes>0x7fff)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
		bytes -= 0x7fff;
	}
	else if(bytes > 0)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = bytes;
		bytes = 0;
	}
	else
	{
		DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
		DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
		return;
	}
  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
//  DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
	
	if(bytes>0x7fff)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
		bytes -= 0x7fff;
	}
	else if(bytes > 0)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = bytes;
		bytes = 0;
	}
	else
	{
		DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
		DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
		return;
	}
  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
//  DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
	
	if(bytes>0x7fff)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
		bytes -= 0x7fff;
	}
	else if(bytes > 0)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = bytes;
		bytes = 0;
	}
	else
	{
		DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
		DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
		return;
	}
  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
//  DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
	
	if(bytes>0x7fff)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = 0x7fff;
		bytes -= 0x7fff;
	}
	else if(bytes > 0)
	{
		DMA_InitStruct_SpiRx.DMA_BufferSize = bytes;
		bytes = 0;
	}
	else
	{
		DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
		DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
		return;
	}
  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
//  DMA_InitStruct_SpiRx.DMA_BufferSize = 0x5802;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
	
//	SPI_FLASH_ReadFinish();  
	
	DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr =(uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;
}

static void LCD_FigureInFlash(LCD_FigFlashTypeDef* figinfo)
{                    
  SPI_InitTypeDef  SPI_InitStructure;
   uint32_t sum;
	uint32_t addrread=FIG_FLASH_DR_Base;
	uint32_t i;
	
	for(i=0;i<(figinfo->figloc);i++)
	{
		addrread += figbytes[i];
	}
	sum = (uint32_t)(figinfo->heigh)*(figinfo->wide);    
//	 Set_direction(0);        /*设定方向为横向*/

	LCD_open_windows(figinfo->col,figinfo->row,figinfo->wide,figinfo->heigh);   
	SPI_FLASH_StartReadSequence(addrread);
	
  SPI_Cmd(SPI1, DISABLE);	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
  DMA_InitStruct_SpiRx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiRx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiRx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;	
  DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiRx.DMA_MemoryBaseAddr = (uint32_t)Bank1_LCD_D;
  DMA_InitStruct_SpiRx.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct_SpiRx.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct_SpiRx.DMA_BufferSize = sum;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);

  /* SPI_FLASH_Tx_DMA_Channel configuration ---------------------------------*/
  DMA_DeInit(SPI_FLASH_Tx_DMA_Channel);  
  DMA_InitStruct_SpiTx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiTx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiTx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiTx.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStruct_SpiTx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiTx.DMA_MemoryBaseAddr = (uint32_t)SPI_FLASH_Buffer_Tx;
  DMA_InitStruct_SpiTx.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct_SpiTx.DMA_Priority = DMA_Priority_High;//DMA_Priority_Low;
  DMA_InitStruct_SpiTx.DMA_BufferSize = sum;
  DMA_Init(SPI_FLASH_Tx_DMA_Channel, &DMA_InitStruct_SpiTx);		
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);	
  /* Enable SPI_MASTER DMA Rx request */
  SPI_I2S_DMACmd(SPI_FLASH, SPI_I2S_DMAReq_Rx, ENABLE);
  /* Enable SPI_MASTER DMA Tx request */
  SPI_I2S_DMACmd(SPI_FLASH, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);
  DMA_Cmd(SPI_FLASH_Tx_DMA_Channel, ENABLE);
   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
//  while(!DMA_GetFlagStatus(SPI_FLASH_Tx_DMA_FLAG));
	
  SPI_Cmd(SPI1, DISABLE);	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);	
	
	SPI_FLASH_ReadFinish();  
}

static void LCD_NumFigInFlash(LCD_FigFlashTypeDef* figinfo,uint8_t num)
{                    
  SPI_InitTypeDef  SPI_InitStructure;
   uint32_t sum;
	uint32_t addrread=FIG_FLASH_DR_Base;
	uint32_t i;
	
	for(i=0;i<(figinfo->figloc);i++)
	{
		addrread += figbytes[i];
	}
	sum = (uint32_t)(figinfo->heigh)*(figinfo->wide);    
//	 Set_direction(0);        /*设定方向为横向*/
	addrread += sum*num*2;

	LCD_open_windows(figinfo->col,figinfo->row,figinfo->wide,figinfo->heigh);   
	SPI_FLASH_StartReadSequence(addrread);
	
  SPI_Cmd(SPI1, DISABLE);	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  DMA_DeInit(SPI_FLASH_Rx_DMA_Channel);
  DMA_InitStruct_SpiRx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiRx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiRx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiRx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiRx.DMA_M2M = DMA_M2M_Disable;	
  DMA_InitStruct_SpiRx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiRx.DMA_MemoryBaseAddr = (uint32_t)Bank1_LCD_D;
  DMA_InitStruct_SpiRx.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStruct_SpiRx.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct_SpiRx.DMA_BufferSize = sum;
  DMA_Init(SPI_FLASH_Rx_DMA_Channel, &DMA_InitStruct_SpiRx);

  /* SPI_FLASH_Tx_DMA_Channel configuration ---------------------------------*/
  DMA_DeInit(SPI_FLASH_Tx_DMA_Channel);  
  DMA_InitStruct_SpiTx.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct_SpiTx.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStruct_SpiTx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct_SpiTx.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct_SpiTx.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStruct_SpiTx.DMA_PeripheralBaseAddr = (uint32_t)SPI_FLASH_DR_Base;
  DMA_InitStruct_SpiTx.DMA_MemoryBaseAddr = (uint32_t)SPI_FLASH_Buffer_Tx;
  DMA_InitStruct_SpiTx.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct_SpiTx.DMA_Priority = DMA_Priority_High;//DMA_Priority_Low;
  DMA_InitStruct_SpiTx.DMA_BufferSize = sum;
  DMA_Init(SPI_FLASH_Tx_DMA_Channel, &DMA_InitStruct_SpiTx);		
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);	
  /* Enable SPI_MASTER DMA Rx request */
  SPI_I2S_DMACmd(SPI_FLASH, SPI_I2S_DMAReq_Rx, ENABLE);
  /* Enable SPI_MASTER DMA Tx request */
  SPI_I2S_DMACmd(SPI_FLASH, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Enable DMA channels */
  DMA_Cmd(SPI_FLASH_Rx_DMA_Channel, ENABLE);
  DMA_Cmd(SPI_FLASH_Tx_DMA_Channel, ENABLE);
   
  /* Transfer complete */
	i=0;
  while(!DMA_GetFlagStatus(SPI_FLASH_Rx_DMA_FLAG))
	{
		if(++i==0)
			break;
	}
//  while(!DMA_GetFlagStatus(SPI_FLASH_Tx_DMA_FLAG));
	
  SPI_Cmd(SPI1, DISABLE);	
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);	
	
	SPI_FLASH_ReadFinish();  
}

static void LCD_FigNuLL(LCD_FigFlashTypeDef* figinfo)
{       
//  SPI_InitTypeDef  SPI_InitStructure;
//	uint32_t addrread=0;
  uint16_t nbytes,i;

//	LCD_open_windows(figinfo->col,figinfo->row,figinfo->wide,figinfo->heigh);   

  nbytes = figinfo->wide*figinfo->heigh;
    
    LCD_open_windows(figinfo->col,figinfo->row,figinfo->wide,figinfo->heigh); 
//		Lcd_data_start();
    
    for (i=0;i<nbytes;i++)            //pos表示1字模中第pos个字节,本程序中nbytes字节每字符
    {
					LCD_WR_Data(BLACK); 
        
    }

}

/*  画横线，横屏,单像素线条  */
static void LCD_RowLine(LCD_LineTypeDef* line)
{
	uint32_t i,val;
	
	val = line->length*line->bold;
	LCD_open_windows(line->col,line->row,line->length,line->bold);
	
	for (i=0;i<val;i++)
	{
		LCD_WR_Data(line->fontcolor);
	}	
}

/*  画竖线，横屏,单像素线条  */
static void LCD_ColLine(LCD_LineTypeDef* line)
{
	uint32_t i,val;
	
	val = line->length*line->bold;
	LCD_open_windows(line->col,line->row,line->bold,line->length);
	
	for (i=0;i<val;i++)
	{
		LCD_WR_Data(line->fontcolor);
	}
	
}

/************************************************************
 * 函数名：LCD_Num
 * 描述  ：在液晶坐标(x,y)中,显示一个指定大小的数字
 * 输入  : 	结构体LCDnum
 * 输出  ：无
 * 举例  ：无
 * 注意  ：字符串显示方向为横 
******************************************************************/    
static void LCD_Num(LCD_CharTypeDef* LCDnum,uint8_t num)
{       
    
  uint8_t temp,bwid, t;
  uint16_t start,nbytes, pos;
  
    
    bwid = LCDnum->wide/8;
    if(LCDnum->wide%8>0)
      bwid += 1;
    nbytes = bwid*LCDnum->heigh;
    
    start = num*nbytes;  //取出字模地址号
    LCD_open_windows(LCDnum->col,LCDnum->row,bwid*8,LCDnum->heigh); 
//		Lcd_data_start();
    
    for (pos=0;pos<nbytes;pos++)            //pos表示1字模中第pos个字节,本程序中nbytes字节每字符
    {
        
        temp= *(LCDnum->pFig+start+pos);  //写第一行的第一个字节
        
        for(t=0; t<8; t++)
        {
            
            if(temp & 0x80)
            {
              LCD_WR_Data(LCDnum->color);
            }
            else 	
            {
                LCD_WR_Data(LCDnum->bkcolor);  
            }            
            temp <<= 1;	            
        }    
        
    }

}
static void LCD_Reset(void)
{
  LCD_Rst();				//复位LCD液晶屏
	Lcd_init_conf();	    //写入命令参数，对液晶屏进行基本的初始化配置

	spi_DMAInit();
}

void InitLcd(void)
{
  LCD_GPIO_Config();
	LCD_FSMC_Config();
  Delay1ms(50);
	LCD_Reset();
	LCD_ClearScreen(0);

  System.Device.Lcd.LCD_Reset = LCD_Reset;
  System.Device.Lcd.LCD_ClearScreen = LCD_ClearScreen;
  System.Device.Lcd.LCD_Num = LCD_Num;
  System.Device.Lcd.LCD_ColLine = LCD_ColLine;
  System.Device.Lcd.LCD_RowLine = LCD_RowLine;
  System.Device.Lcd.LCD_ColorRectangle = LCD_ColorRectangle;
  System.Device.Lcd.LCD_FigureInFlash = LCD_FigureInFlash;
  System.Device.Lcd.LCD_FigNuLL = LCD_FigNuLL;
  System.Device.Lcd.LCD_NumFigInFlash = LCD_NumFigInFlash;
}






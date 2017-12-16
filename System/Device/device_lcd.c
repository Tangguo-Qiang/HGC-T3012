/*
********************************************************************************
*�ļ���     : device_lcd.c
*����       : 480*320 ����
*ԭ��       : 128*64�ֿ�ڰ�������Ϊ���У��ֿ⺺�ֲ���֧��GB2312���룬
*           : Keil������ֱ��֧��GB2312������ÿ��16*16����һ�п�����ʾ8������
*           : ��ĸ֧��ASCII��׼��ÿ��8*16��һ�п�����ʾ16����ĸ
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




/* ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X60000000~0X63FFFFFF
 * FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
 * 16 bit => FSMC[24:0]��ӦHADDR[25:1](AHB[25:1])
 * �Ĵ�������ַ = 0X60000000
 * RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * ��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼��㡣
 */
#define Bank1_LCD_D    ((uint)0x60020000)       //Disp Data ADDR
#define Bank1_LCD_C    ((uint)0x60000000)	   //Disp Reg ADDR

/*ɫ�����ݴ洢��ʽ 0xRRRRRGGGGGGGBBBBB */

/*ѡ��LCDָ���Ĵ���*/
#define LCD_WR_REG(index)    ((*(__IO uint16_t *) (Bank1_LCD_C)) = ((uint16_t)index))
/*��LCD GRAMд������*/
#define LCD_WR_Color(val)       {(*(__IO uint16_t *) (Bank1_LCD_D)) = ((uint16_t)(val));}
/*��LCD�е�index�Ĵ�������Ϊvalֵ, LCD_HX8347I_Reg(index,val)*/
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
 * ��������LCD_GPIO_Config
 * ����  ������FSMC����LCD��I/O
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/  
static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ����LCD������ƹܽ� PD12*/
    GPIO_InitStructure.GPIO_Pin = LCD_BKLIGHT_PIN;		
    GPIO_Init(LCD_BKLIGHT_PORT, &GPIO_InitStructure);
    LCD_BL_OFF;
	
    /* ����LCD��λ���ƹܽ� PC4  */
    GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN ; 	 
    GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);  		   
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* ����FSMC���Ӧ�Ŀ�����
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
    
   
//	GPIO_SetBits(GPIOA, GPIO_Pin_12);	//��λ	 

	 
}
/*******************************************
 * ��������LCD_FSMC_Config
 * ����  ��LCD  FSMC ģʽ����
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ����
*********************************************/ 
static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
		FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;						   //���ö�дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 							   //����дʱ��
		FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);

    p.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ�䣬50MHz��40ns
    p.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    p.FSMC_DataSetupTime = 0x05;		 //���ݽ���ʱ��
    p.FSMC_BusTurnAroundDuration = 0x00;   //���߻ָ�ʱ��
    p.FSMC_CLKDivision = 0x00;			  //ʱ�ӷ�Ƶ
    p.FSMC_DataLatency = 0x00;			  //���ݱ���ʱ��
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 //�ڵ�ַ��\���߲����õ�����£�ABCDģʽ�����𲻴�
											 //����Ա����ֻ��ʹ����չģʽ����Ч
											
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;			 //NOR FLASH��BANK1
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	  //���������ַ�߲�����
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;				  //�洢������NOR FLASH
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		  //���ݿ��Ϊ16λ
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	  //ʹ���첽дģʽ����ֹͻ��ģʽ
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //����Ա������ֻ��ͻ��ģʽ����Ч���ȴ��źż���Ϊ��
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;				 //��ֹ�Ƕ���ͻ��ģʽ
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;   //����Ա���ý���ͻ��ģʽ����Ч��NWAIT�ź���ʲôʱ�ڲ���
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;			  //����Ա������ֻ��ͻ��ģʽ����Ч������NWAIT�ź�
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;			  //��ֹͻ��д����
   	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		   //дʹ��
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;		  //��ֹ��չģʽ����չģʽ����ʹ�ö����Ķ���дģʽ
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;						   //���ö�дʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 							   //����дʱ��
   
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

/**********************************
 * ��������LCD_Rst
 * ����  ��LCD �����λ
 * ����  : �� 
 * ���  ����
 * ����  ����
 * ע��  ���� 
*************************************/

static void LCD_Rst(void)
{			
    GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);	 //�͵�ƽ��λ
    Delay1ms(30); 					   
    GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN );		 	 
    Delay1ms(30); 	
}

/**********************************
 * ��������LCD_RD_data
 * ����  ���� HX8347I RAM ����
 * ����  : �� 
 * ���  ����ȡ������,16bit *
 * ����  ����
 * ע��  ����
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
     LCD_WR_REG(0x2c);//��ʼд���ݵ�GRAM
}


/**********************************
 * ��������Set_direction
 * ����  ������HX8347IGRAMָ��ɨ�跽��
 * ����  : 0�� ����ɨ��
 		   1�� ����ɨ��
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
static void Set_direction(uint8_t option)
{

    switch(option)
    {
    case 0:
		    /*����*/
      break;
    case 1:
    		/*����*/
      break;
        
    }

}

/**********************************
 * ��������LCD_open_windows
 * ����  ������(��x,yΪ������㣬��Ϊlen,��Ϊwid)
 * ����  : -x    �������
           -y	   �������
           -len  ������ 
           -wid ������
 * ���  ����
 * ����  ����
 * ע��  ����
*************************************/
static void LCD_open_windows(uint16_t x,uint16_t y,uint16_t len,uint16_t wid)
{                    

  LCD_WR_REG(0x2A);
	LCD_WR_Data(x>>8);	//��start ��ʼλ�õĸ�8λ
  LCD_WR_Data(x&0x00ff);//��ʼλ�õĵ�8λ
  LCD_WR_Data((x+len-1)>>8);	//end ����λ�õĸ�8λ
  LCD_WR_Data((x+len-1)&0x00ff);  //����λ�õĵ�8λ
		
  LCD_WR_REG(0x2B);
  LCD_WR_Data(y>>8);	//start row��ʼλ�õĸ�8λ
  LCD_WR_Data(y&0x00ff);//��ʼλ�õĵ�8λ
  LCD_WR_Data((y+wid-1)>>8);	//end ����λ�õĸ�8λ
  LCD_WR_Data((y+wid-1)&0x00ff);  //����λ�õĵ�8λ


      Lcd_data_start();     
}
/**********************************
 * ��������Lcd_init_conf
 * ����  ��HX8347I LCD�Ĵ�����ʼ����
 * ����  : ��
 * ���  ����
 * ����  ����
 * ע��  ����
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
 * ��������LCD_Rectangle
 * ����  ����ָ�����괦��ʾָ������ľ���
 * ����  : -x1      ���������� 0~319
           -y1      ���������� 0~239
		   -len     ���򳤶� 0~319
		   -wid     ������ 0~480
		   -rgb565  �����ɫ
 * ���  ����
 * ����  ��	LCD_Rectangle(10,10,120,160,0);
		    LCD_Rectangle(200,200,120,40,0);
 * ע��  ��	�Ѳ���
*********************************************************/    
static void LCD_ColorRectangle(LCD_RectangleTypeDef* rectangle)
{                    
   uint n, temp;
//	 Set_direction(0);	 						/*�趨����Ϊ����*/
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
//    for (i=0;i<bytes;i++)            //pos��ʾ1��ģ�е�pos���ֽ�,��������nbytes�ֽ�ÿ�ַ�
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
//	 Set_direction(0);        /*�趨����Ϊ����*/

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
//	 Set_direction(0);        /*�趨����Ϊ����*/
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
    
    for (i=0;i<nbytes;i++)            //pos��ʾ1��ģ�е�pos���ֽ�,��������nbytes�ֽ�ÿ�ַ�
    {
					LCD_WR_Data(BLACK); 
        
    }

}

/*  �����ߣ�����,����������  */
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

/*  �����ߣ�����,����������  */
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
 * ��������LCD_Num
 * ����  ����Һ������(x,y)��,��ʾһ��ָ����С������
 * ����  : 	�ṹ��LCDnum
 * ���  ����
 * ����  ����
 * ע��  ���ַ�����ʾ����Ϊ�� 
******************************************************************/    
static void LCD_Num(LCD_CharTypeDef* LCDnum,uint8_t num)
{       
    
  uint8_t temp,bwid, t;
  uint16_t start,nbytes, pos;
  
    
    bwid = LCDnum->wide/8;
    if(LCDnum->wide%8>0)
      bwid += 1;
    nbytes = bwid*LCDnum->heigh;
    
    start = num*nbytes;  //ȡ����ģ��ַ��
    LCD_open_windows(LCDnum->col,LCDnum->row,bwid*8,LCDnum->heigh); 
//		Lcd_data_start();
    
    for (pos=0;pos<nbytes;pos++)            //pos��ʾ1��ģ�е�pos���ֽ�,��������nbytes�ֽ�ÿ�ַ�
    {
        
        temp= *(LCDnum->pFig+start+pos);  //д��һ�еĵ�һ���ֽ�
        
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
  LCD_Rst();				//��λLCDҺ����
	Lcd_init_conf();	    //д�������������Һ�������л����ĳ�ʼ������

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






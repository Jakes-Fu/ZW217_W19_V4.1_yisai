/******************************************************************************
 ** File Name:     FSTN_NT7538.c                                              *
 ** Description:															  *
 **    This file contains driver for B&W LCD.(FSTN)						      *
 ** 目前支持的型号有: 精锐通WOM-12864P11                                      *                                   *
 ** Author:         Jianping Ma                                               *
 ** DATE:           2010-03-05                                                *
 ** Copyright:      2010 Stelcom, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2010-03-05     Jianping Ma	    Create.
 ******************************************************************************/

#include "os_api.h"

#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "gpio_drv.h"
#include "mem_prod.h"


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

    
    #define    GPIO_lCD_DATA           32//46      
    #define    GPIO_lCD_CLK            33//47    
    #define    GPIO_lCD_RS            30// 48  
    
    #define    GPIO_lCD_CS             29//49    
    #define    GPIO_LCD_RESET         27// 139     
//10,0000,0000
//*(volatile uint32 *)(0x8A000100) |= 0x00008000
    
    #define    NT7538_RST_ENABLE        GPIO_SetValue(GPIO_LCD_RESET, 0)
    #define    NT7538_RST_DISABLE       GPIO_SetValue(GPIO_LCD_RESET, 1)
    //gpio 29
    //#define    NT7538_CS_ENABLE         *(volatile uint32 *)(0x8A000180) &= 0xFFFFFFFD
    #define    NT7538_CS_ENABLE         *(volatile uint32 *)(0x8A000080) &= 0xFFFFDFFF
    
    //#define    NT7538_CS_DISABLE        *(volatile uint32 *)(0x8A000180) |= 0x00000002
    #define    NT7538_CS_DISABLE        *(volatile uint32 *)(0x8A000080) |= 0x00002000
    
    
    //gpio 30
    //#define    NT7538_RS_COMMAND     *(volatile uint32 *)(0x8A000100) &= 0xFFFFFDFF
    #define    NT7538_RS_COMMAND     *(volatile uint32 *)(0x8A000080) &= 0xFFFFBFFF
    
    //#define    NT7538_RS_DATA           *(volatile uint32 *)(0x8A000100) |= 0x00000200//*(volatile uint32 *)(0x8A000180) |= 0x00000001
    #define    NT7538_RS_DATA           *(volatile uint32 *)(0x8A000080) |= 0x00004000//*(volatile uint32 *)(0x8A000180) |= 0x00000001

    
    //gpio 33
    //#define    NT7538_CLK_LOW           *(volatile uint32 *)(0x8A000100) &= 0xFFFF7FFF
    #define    NT7538_CLK_LOW           *(volatile uint32 *)(0x8A000100) &= 0xFFFFFFFD

    
    //#define    NT7538_CLK_HIGH          *(volatile uint32 *)(0x8A000100) |= 0x00008000
    #define    NT7538_CLK_HIGH          *(volatile uint32 *)(0x8A000100) |= 0x00000002

    

	//gpio32
    #define    NT7538_DATA_LOW           *(volatile uint32 *)(0x8A000100) &= 0xFFFFFFFE

    #define    NT7538_DATA_HIGH          *(volatile uint32 *)(0x8A000100) |= 0x00000001

#define lcd_delay(ms) \
        OS_TickDelay(ms);

//1 //1 1.lcd display
#define LCD_START_LINE			0x20
#define LCD_START_COLUMN		0x01

//1 2.lcd contrast
#define LCD_CONTRAST_STEP		(5)//步长
#define LCD_CONTRAST_MIN		(28)//最小对比度
#define LCD_CONTRAST_MAX		(63)//最大对比度


    
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
static uint8  lcd_image[ 8 ][ 132 + 4];
static uint32 lcd_current_contrast = 0x30;//0x20;//by brezen @ 10.05.14
static uint8  s_start_colum = LCD_START_COLUMN;
static uint8  s_start_row   = LCD_START_LINE;
static uint8  s_max_contrast = LCD_CONTRAST_MAX;

//有的屏幕为132 有的为128 为了统一，初始化的时候全部按照132的屏幕，进行清屏 by brezen
#ifndef atwLcdExtInit
#define atwLcdExtInit() \
do{\
    uint page,col;                                          \
                                                             \
    for(col = 0; col < 132; col++)  \
    {                                                        \
        for(page = 0; page < 8; page++)                      \
        {                                                    \
            lcd_set_column(col);                             \
            lcd_set_page(page);                              \
            lcd_send_data(0);                                \
        }                                                    \
    }                                                        \
}while(0)
#endif
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
void lcd_send_command(unsigned char command)
{       
#if 0	
    unsigned char bit;
 
    NT7538_CS_ENABLE;  
    
    NT7538_RS_COMMAND;
	
    for (bit = 0; bit < 8; bit++) 
    {
		if (command & 0x80)
		{
            NT7538_DATA_HIGH;  
        }
        else
        {
            NT7538_DATA_LOW;  
		}
        command = command<< 1;
		
        NT7538_CLK_LOW;    

        NT7538_CLK_HIGH;  

    }
    
    NT7538_CS_DISABLE;
#else
	unsigned char bit;
	/// add here to selcet SPI slaver 
 
    GPIO_SetValue(GPIO_lCD_CS, SCI_FALSE);  //cs

	GPIO_SetValue    (GPIO_lCD_RS  , 0);


	
    for (bit = 0; bit < 8; bit++) 
    {

		if (command & 0x80)
		{
            //*(volatile uint32 *)(0x8A000100) |= 0x00004000;//SSPI_SETMOSI;  /* set MOSI */
            NT7538_DATA_HIGH;
        }
        else
        {
            //*(volatile uint32 *)(0x8A000100) &= 0xFFFFBFFF;//SSPI_CLRMOSI;  /* clear MOSI */
            NT7538_DATA_LOW; 
		}
        command = command<< 1;
		
        //*(volatile uint32 *)(0x8A000100) &= 0xFFFF7FFF;//SSPI_CLRCLK;    
        ////SSPI_DELAY(SSPI_SPEED/2);////
        //*(volatile uint32 *)(0x8A000100) |= 0x00008000;//SSPI_SETCLK;  
        ////SSPI_DELAY(SSPI_SPEED/2);////
        NT7538_CLK_LOW;   
        
        NT7538_CLK_HIGH;  

    }
    GPIO_SetValue(GPIO_lCD_CS, SCI_TRUE);  //cs
#endif
}


void lcd_send_data(unsigned char command)
{       
    unsigned char bit;
 
    NT7538_CS_ENABLE;  
    
    NT7538_RS_DATA;
	
    for (bit = 0; bit < 8; bit++) 
    {
		if (command & 0x80)
		{
            NT7538_DATA_HIGH;  
        }
        else
        {
            NT7538_DATA_LOW;  
		}
        command = command<< 1;
		
        NT7538_CLK_LOW;    

        NT7538_CLK_HIGH;  

    }
    
    NT7538_CS_DISABLE;
}

static __inline void lcd_set_page( uint page )
{
    uint cmd;

    cmd = 0xB0 + (page & 0xF);
    lcd_send_command( cmd );
}

static __inline void lcd_set_column( uint col )
{
    uint lsb;

    lsb = (col & 0xF);
    col = (col >> 4) & 0xF;

#if defined(LCD_COL_MSB_FIRST)//cooper on 2010.7.22
    lcd_send_command( col | 0x10 ); // send  msb
    lcd_send_command( lsb );        // send  lsb
#else
    lcd_send_command( lsb );        // send  lsb
    lcd_send_command( col | 0x10 ); // send  msb
#endif    
}

static __inline void lcd_refresh_image( uint left, uint top, uint right, uint bottom)
{
    uint    tmp;
    uint8 * ptr;
	
    uint32    row    =0;      //=(行值/8)
	uint32    column =0;      //=(列值)    
	uint32   temp;     
	
    uint16 *pLcdbuf = (uint16 *)MEM_GetMainLCDBuffer();

    if( bottom >= LCD_HEIGHT )  
    {   
        bottom = LCD_HEIGHT - 1;  
    }
    if( right >= LCD_WIDTH )    
    {   
        right  = LCD_WIDTH - 1;  
    }

       
    for(row =0; row<=7; row++)
    {    
        for(column =0; column<=127; column++)
        {
           temp=column+row*1024; // temp=column+row<<10;

           lcd_image[row][column] =  ( ( pLcdbuf[temp    ]? 0 : 1) << 0) \
                                    |( ( pLcdbuf[temp+128]? 0 : 1) << 1) \
                                    |( ( pLcdbuf[temp+256]? 0 : 1) << 2) \
                                    |( ( pLcdbuf[temp+384]? 0 : 1) << 3) \
                                    |( ( pLcdbuf[temp+512]? 0 : 1) << 4) \
                                    |( ( pLcdbuf[temp+640]? 0 : 1) << 5) \
                                    |( ( pLcdbuf[temp+768]? 0 : 1) << 6) \
                                    |( ( pLcdbuf[temp+896]? 0 : 1) << 7) ;

        }   
    }    

    top    = top    >> 3; // get start page
    bottom = bottom >> 3; // get end page

    while( top <= bottom )
    {
        // set display start line
        lcd_send_command( 0x40 + s_start_row);//cooper on 2010.6.18
        // set page
        lcd_set_page( top );

        // set start col
		lcd_set_column( left + s_start_colum);//cooper on 2010.6.18
        ptr = &lcd_image[ top ][ left ];
        for( tmp = left; tmp <= right; tmp++ )
        {
            lcd_send_data( (uint)(*ptr++) );
        }

        top++;
    }
}


static uint32 lcd_set_contrast( uint32 contrast )
{
    uint32   ret = lcd_current_contrast;

    if( contrast > s_max_contrast )//cooper modifed for common on 2010.7.21
    {
		contrast = LCD_CONTRAST_MAX;
        // return the default value;
        ret      = contrast;
    }

    lcd_current_contrast      = contrast;

    lcd_send_command( 0x81 );
    lcd_send_command( lcd_current_contrast );

    return ret;
}


static void lcd_clr(uint bClearIcon)
{
    uint   col, row;

    lcd_refresh_image(0, 0, 128, 64);
}

static const unsigned char initCommand[] =
{
  0xe2,
  0xae,
  0xa0,
  0xc8, //固话LCD翻转
  0xa6,
  0xa2,
  0x2c,
  0x2e,
  0x2f,
  0x24,
  0x81,
  0x30,
  0xaf
};

#define INIT_CMD_SIZE (sizeof(initCommand)/sizeof(unsigned char)) 

static void transInitCommand(void)
{
	unsigned char i = 0;
	for(i = 0; i < INIT_CMD_SIZE; i++)
	{
		lcd_send_command(initCommand[i]);
	}
}

#if 0
static void lcd_delay( uint milliseconds )
{
    uint   loop;

    while( milliseconds-- )
    {
        // delay one millisecond
        for( loop = 0; loop < 0x1FFF; loop++ )
        {
            ;  // nothing to do.
        }
    }
}
#endif

static void lcd_init(void)
{
	
	SCI_TRACE_LOW("lcd_init zbyu");
	GPIO_Enable      (GPIO_LCD_RESET);
	//GPIO_SetDirection(GPIO_LCD_RESET,1);
	GPIO_SetValue(GPIO_LCD_RESET, 1); 
    
	GPIO_SetValue(GPIO_LCD_RESET, 0); 

	lcd_delay(20);
	 // (if lcd_delay( 10 ), about wait 1us )   
	GPIO_SetValue(GPIO_LCD_RESET, 1); 
	lcd_delay(20 );
	
	transInitCommand();

#if defined(atwLcdExtInit)//cooper on 2010.7.22
	atwLcdExtInit();
#endif

	
}

static void lcd_close(void)
{
    #if 0
    // display off
    lcd_send_command( 0xAE );
    lcd_send_command( 0xE2 );
    #endif

}

/**
 * Reset the LCD, be sure LCD has been power on, in other words MV8602 has been power on
 */
LOCAL void MONO12864_HardwareReset(void)
{
    /* hardware reset */
    NT7538_RST_DISABLE;
    LCD_Delayms(5);
    NT7538_RST_ENABLE;
    LCD_Delayms(5);
    NT7538_RST_DISABLE;
    LCD_Delayms(20);
}


/**
 * LCD initialize sequence.
 */
LOCAL ERR_LCD_E MONO12864_Init(void)
{
	uint32 bkground = 0xFF00;

    /*reset hardware & power off*/
    MONO12864_HardwareReset();    
    lcd_init();
    
    return ERR_LCD_NONE;
}


/**
 * Enter sleep or resume
 */
LOCAL ERR_LCD_E MONO12864_EnterSleep(  BOOLEAN is_sleep )
{
	//need todo!
    return ERR_LCD_NONE;

}


/**
 * Close operation
 */
LOCAL void MONO12864_Close(void)
{
    MONO12864_EnterSleep( SCI_TRUE );	
    lcd_close();
}



/**
 * Invalidate a rectanglular on LCD
 */
PUBLIC ERR_LCD_E MONO12864_InvalidateRect( uint16 left, uint16 top, uint16 right, uint16 bottom )
{
    lcd_refresh_image( left, top, right, bottom );

    return ERR_LCD_NONE; 
}

/**
 * LCD invalidate
 */
LOCAL ERR_LCD_E MONO12864_Invalidate(void)
{
    
    return MONO12864_InvalidateRect(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
}




LOCAL ERR_LCD_E MONO12864_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    //need todo!
	return ERR_LCD_NONE;
}


/**
 *  dumming function, set lcd contrast
 */
LOCAL ERR_LCD_E MONO12864_SetContrast(
                                     uint16  contrast   //contrast value to set
                                     )
{     
    (uint32)lcd_set_contrast( contrast );
    return ERR_LCD_NONE;
}


/**
 * dumming function, set lcd brightness
 */
LOCAL ERR_LCD_E MONO12864_SetBrightness(
                                       uint16 brightness    //birghtness to set
                                       )
{
    //need todo!
    return ERR_LCD_FUNC_NOT_SUPPORT;
}


/**
 * dumming function, set display window
 */
LOCAL ERR_LCD_E MONO12864_SetDisplayWindow(
                                          uint16 left,  //left of the window
                                          uint16 top,   //top of the window 
                                          uint16 right, //right of the window
                                          uint16 bottom //bottom of the window
                                          )
{
    //need todo!
    return ERR_LCD_FUNC_NOT_SUPPORT;
}



LOCAL ERR_LCD_E MONO12864_SetDirection( LCD_DIRECT_E direct_type)
{
    //need todo!
	return ERR_LCD_FUNC_NOT_SUPPORT;
}


LCD_OPERATIONS_T MONO12864_operations = 
{
    MONO12864_Init,
    MONO12864_EnterSleep,
    MONO12864_SetContrast,
    MONO12864_SetBrightness,
    MONO12864_SetDisplayWindow,
    MONO12864_InvalidateRect,
    MONO12864_Invalidate,
    MONO12864_Close,
    MONO12864_RotationInvalidateRect,
    MONO12864_SetDirection,
    NULL,
    NULL
};


PUBLIC LCD_OPERATIONS_T* MONO12864_GetOperations()
{
    return (LCD_OPERATIONS_T*)&MONO12864_operations;
}

const LCD_SPEC_T g_lcd_NT7538[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_SPI,
	WIDTH_8,
	NULL,
	(LCD_OPERATIONS_T*)&MONO12864_operations,
	0,
	0
	}
};
#define alexLcdExtInit() \
do{\
    uint page,col;                                          \
    uint extCol[] = {131,0,129,130};                          \
                                                             \
    for(col = 0; col < sizeof(extCol)/sizeof(uint); col++)  \
    {                                                        \
        for(page = 0; page < 8; page++)                      \
        {                                                    \
            lcd_set_column(extCol[col]);                     \
            lcd_set_page(page);                              \
            lcd_send_data(0);                                \
        }                                                    \
    }                                                        \
}while(0)
PUBLIC void alexLcdExtPainFull()
{
	#ifdef alexLcdExtInit
	alexLcdExtInit();
	#endif
	return;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 




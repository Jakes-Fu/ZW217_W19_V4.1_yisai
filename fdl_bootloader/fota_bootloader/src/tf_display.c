
#include "tf_load.h"
#include "analog_drvapi.h"

extern  TF_LCD_INFO s_lcd_info;

#define   DM_WHITE_COLOR		        	0xffff
#define   DM_BLACK_COLOR		        	0x001f //0x0000

#define   MAINSCREEN_WIDTH                  s_lcd_info.lcd_info.lcd_width
#define   MAINSCREEN_HEIGHT             	s_lcd_info.lcd_info.lcd_height
#define   DM_PRG_TEXT_LEFT                 	(1*MAINSCREEN_WIDTH/4)
#define   DM_PRG_TEXT_TOP                  	(0)
#define   DM_PRG_TEXT_RIGHT              	(3*MAINSCREEN_WIDTH/4)
#define   DM_PRG_TEXT_BOTTOM           	    (1*MAINSCREEN_HEIGHT/8)

#define   DM_PRG_RECT_LEFT                		(1*MAINSCREEN_WIDTH/5)
#define   DM_PRG_RECT_TOP                		(1*MAINSCREEN_HEIGHT/4)
#define   DM_PRG_RECT_RIGHT             		(4*MAINSCREEN_WIDTH/5)
#define   DM_PRG_RECT_BOTTOM          		    (1*MAINSCREEN_HEIGHT/2)

#define TF_UartTrace              		rprintf


PUBLIC ERR_LCD_E TF_LcdInit (void)
{
#if defined(PLATFORM_UWS6121E)
    return LCD_Init();
#else
    return LcdInit();
#endif
}

/*****************************************************************************/
//  Discription: Draw a line from (left, top) --> (right, bottom)
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void TF_DrawLine(LCD_ID_E lcd_id, uint16 left, uint16 top,uint16  right,uint16 bottom, uint8 color)
{
    uint16  dx = right - left, dy = bottom - top;
    uint16  wc_x = 0, wc_y = 0;
    uint16  inc_x, inc_y, sum;
    uint16  i;

    uint16 main_lcd_width = s_lcd_info.lcd_info.lcd_width;
    uint16 main_lcd_height = s_lcd_info.lcd_info.lcd_height;
    // check input
    left    = (left >= main_lcd_width)? main_lcd_width   :   left;
    right  = (right >= main_lcd_width)? main_lcd_width   :   right;
    top     = (top >= main_lcd_height)?    main_lcd_height  :   top;
    bottom  = (bottom >= main_lcd_height)    ?    main_lcd_height  : bottom;

    if( dx & 0x8000 )
    {
        //dx < 0;
        dx      = 0x0 - dx; // dx = -dx;
        inc_x   = (uint16)(-1);
    }
    else
    {
        //inc_x = (dx) ? 1 : 0;
        inc_x = !(dx);
        inc_x++;
    }

    if( dy & 0x8000 )
    {
        //dy < 0;
        dy      = 0 - dy;
        inc_y   = (uint16)(-1);
    }
    else
    {
        //inc_y = (dy) ? 1 : 0;
        inc_y = !(dy);
        inc_y++;
    }

    sum = (dx > dy) ?  dx  : dy;

    for( i = 0; i <= sum; i++)
    {
        LCD_Set8Pixels(left, top, color);

        wc_x += dx;
        wc_y += dy;
        if( wc_x >= sum )
        {
            wc_x -= sum;
            left += inc_x;
        }
        if( wc_y >= sum )
        {
            wc_y -=sum;
            top += inc_y;
        }
    }
}	

/*****************************************************************************/
//  Description:  display the text(assert information) when assert.(just english) 
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC  void TF_DisplayText(char *text, uint16 len)
{
    LCD_DisplayText(DM_PRG_TEXT_LEFT,DM_PRG_TEXT_TOP, text, len);
}

/*****************************************************************************/
//  Discription:show  err  info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_Show_Error(uint8 errno)
{
    char buff[100] = {0};
    //sprintf(buff, "%s",TF_Errstring_Gen(errno));
    TF_DisplayText(buff, strlen(buff));	
}

/*****************************************************************************/
// Description:    tf lcm init
// Global resource dependence: 
// Author: younger
// Note:
/******************************************************************************/
PUBLIC void TF_LcmInit(TF_UPDATE_INFO *tf_update_info_ptr )
{
    s_lcd_info.bus_width = tf_update_info_ptr->tf_update_lcd.bus_width;
    s_lcd_info.dir_type   = tf_update_info_ptr->tf_update_lcd.dir_type;
    s_lcd_info.rotate_mode = tf_update_info_ptr->tf_update_lcd.rotate_mode;
    s_lcd_info.lcd_info.lcd_height = tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_height;
    s_lcd_info.lcd_info.lcd_width  = tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_width;
    s_lcd_info.data_addr		= tf_update_info_ptr->tf_update_lcd.data_addr;
    s_lcd_info.block_buf_ptr	= tf_update_info_ptr->tf_update_lcd.block_buf_ptr;
    TF_UartTrace("TF_LcmInit: lcd_height=0x%x, lcd_width=0x%x", s_lcd_info.lcd_info.lcd_height, s_lcd_info.lcd_info.lcd_width);
}

PUBLIC void TF_LCD_Fill(LCD_ID_E lcd_id, uint16 left, uint16 top, uint16  right, uint16 bottom, uint16 color)
{
     uint16 i, j;
     uint16 *_ptr = (uint16 *)s_lcd_info.lcd_info.lcdbuff_ptr;

     for (i = top; i < bottom; i++)
     {
          for (j = left; j < right; j++)
          {
              _ptr[i * s_lcd_info.lcd_info.lcd_width + j] = color;
          }
     	}
}

/*****************************************************************************/
//  Discription:Lcd Draw progress
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  void TF_LCD_DisplayProgressBar(uint16 left, uint16 top, uint16 right, uint16 bottom, uint16 progress)
{
    TF_LCD_Fill(MAIN_LCD_ID, left, top, right, bottom, DM_WHITE_COLOR);
    TF_LCD_Fill(MAIN_LCD_ID, left, top, progress, bottom, DM_BLACK_COLOR);

    TF_DrawLine(MAIN_LCD_ID,left,top,left,bottom,1);
    TF_DrawLine(MAIN_LCD_ID,left,top,right,top, 1);
    TF_DrawLine(MAIN_LCD_ID,right,top,right,bottom, 1);
    TF_DrawLine(MAIN_LCD_ID,left,bottom,right,bottom, 1);

    LCD_Display(MAIN_LCD_ID, 0, 0, s_lcd_info.lcd_info.lcd_width -1, s_lcd_info.lcd_info.lcd_height-1);
}

/*****************************************************************************/
//  Discription:Lcd Draw progress
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_ShowProgress(uint32 current, uint32 total)
{  

    rprintf("TF_ShowProgress: %d-%d\n", current, total);
    TF_LCD_DisplayProgressBar ( DM_PRG_RECT_LEFT, DM_PRG_RECT_TOP, 
            DM_PRG_RECT_RIGHT, DM_PRG_RECT_BOTTOM, 
            DM_PRG_RECT_LEFT + ((DM_PRG_RECT_RIGHT - DM_PRG_RECT_LEFT) * current) / total);
    //	if(current == total)
    //	{
    //		LCD_Clear(MAIN_LCD_ID, 0xff); 
    //		TF_LCD_Display(MAIN_LCD_ID, 0, 0, s_lcd_info.lcd_info.lcd_width-1, s_lcd_info.lcd_info.lcd_height-1);
    //	}
}

PUBLIC  void* TF_GetMainLCDBuffer(void)
{
    return (void*)LCD_GetMainLCDBuffer();
}

PUBLIC  void* TF_GetSubLCDBuffer(void)
{
    return (void*)LCD_GetSubLCDBuffer();
}

/*******************************************************************************/
// Description: get lcd bus width
// Global resource dependence:
// Author: 
// Note:
/********************************************************************************/	
PUBLIC uint32 TF_GetLcdBusWidth(  LCD_ID_E lcd_id)
{
    return LCD_GetLcdBusWidth(lcd_id);
}

/*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//                  value = britness
//  Author:     Zhemin.Lin
//  Note:        LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void  TF_SetLcdBackLight(BOOLEAN b_on, uint16 value)
{
#if defined(PLATFORM_SC6800H)
    if(FALSE == b_on)
    {
        //close lcm backlight
        *(volatile uint32*)GR_ANA_CTL &= ~(BIT_0 | BIT_1);
        *(volatile uint32*)GR_ANA_CTL |= (BIT_0);
    }
    else
    {   
        //open lcm backlight
        *(volatile uint32*)GR_ANA_CTL &= ~(BIT_0 | BIT_1);
        *(volatile uint32*)GR_ANA_CTL |= (BIT_1);     
    }

    *(volatile uint32*)GR_ANA_CTL &= ~(0x1F<<2);
    *(volatile uint32*)GR_ANA_CTL |= ((value&0x1F)<<2);
#else
    ADI_init();
    if(b_on)
    {
        _ANA_SetLCMBackLight(ANA_DEV_OPEN);
    }
    else
    {
        _ANA_SetLCMBackLight(ANA_DEV_CLOSE);

    }
    _ANA_SetLCMBrightness(value);

    return;
#endif
}


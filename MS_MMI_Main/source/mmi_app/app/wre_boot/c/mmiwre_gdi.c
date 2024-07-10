#ifdef WRE_SUPPORT
/******************************************************************************
 ** File Name:      osadapt_gdi.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           04/10/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps gdi-layer for mocor os				       *
 **																              *
 ******************************************************************************/
#include "mmiwre_gdi.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "guilcd.h"
#include "guiblock.h"
#include "spml_api.h"
#include "guistring.h"
#include "mmi_theme.h"
#include "mmitheme_update.h"
#include "mmk_type.h"


 //LOCAL unsigned short *pSubLcdBuffer=0;		// Sub LCD
 //LOCAL unsigned short *pMainLcdBuffer=0;		// Main LCD

////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : GetLcdWHMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
void GetLcdWHMocor(unsigned char lcd_type, unsigned short *width, unsigned short *height)
{
#if 0
	LCD_INFO_T lcd_info = {0};

	LCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);

	*width=lcd_info.lcd_width;
	*height=lcd_info.lcd_height;
#else
	int w,h;

	if(width  == NULL || height == NULL)
		return;

	WRE_GetScreenLogicSize(&w,&h);
	*width=w;
	*height=h;


#endif
}
/*****************************************************************************/
//  Description : LCD_Refresh
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
void LCD_Refresh(int lcd,int x1,int y1,int x2,int y2)
{
    GUI_RECT_T     rect = {0};
#ifdef WIN32
	unsigned short h,w;
	GetLcdWHMocor(0,&w,&h);
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = w-1;
    rect.bottom = h-1;
#else
    rect.left   = x1;
    rect.top    = y1;
    rect.right  = x2;
    rect.bottom = y2;
#endif
    MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(), rect);
}


/*****************************************************************************/
//  Description : OpenLcd
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
void OpenLcd(unsigned char bLcdID)
{
}

/*****************************************************************************/
//  Description : CloseLcd
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
void CloseLcd(unsigned char bLcdID)
{
}

//由应用设置需要显示的BUFFER
/*
void * SetDrawBuffer(void *pBuffer)
{
    void *pOldBuffer = 0;

    pOldBuffer = pMainLcdBuffer;
    pMainLcdBuffer = pBuffer;

 
    return pOldBuffer;
}
*/
/*
void * SetSubScreenBuffer(void)
{
#if 0

    void*      lcdbuf_ptr = PNULL;
    LCD_INFO_T lcd_info = {0};

    if ( ERR_LCD_NONE != LCD_GetInfo(SUB_LCD_ID, &lcd_info))
    {
        SCI_ASSERT ( 0 );
    }
    else    // called successful
    {
        pSubLcdBuffer= (void *)(lcd_info.lcdbuff_ptr);
    }
#endif

    return NULL;//pSubLcdBuffer;
}
*/
/* internal version of ExtTextOut, passed flags for text data type*/

/*****************************************************************************/
//  Description : MyExtTextOutMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
unsigned int MyExtTextOutMocor(int x, int y,TXTPARAM *pTxtParam,unsigned short* lpszString, unsigned short cbCount)
{
	SPML_TEXT_PARAM_T  param = {0};
	SPML_TEXT_METRICS_T  metrics_data = {0};

	if(pTxtParam==NULL)
		return 0;
	
	///
	param.size = pTxtParam->size;
	param.font_color= pTxtParam->font_color;
	param.bg_color = pTxtParam->bg_color;
	param.edge_color = pTxtParam->edge_color;
	param.line_color = pTxtParam->line_color;
	param.buffer_height = pTxtParam->buffer_height;
	param.buffer_width = pTxtParam->buffer_width;
	param.buffer_type=SPML_BUF_RGB565;
	param.display_rect.left = x;//0
	param.display_rect.top = y;//0
    param.display_rect.right = pTxtParam->buffer_width;
    param.display_rect.bottom = pTxtParam->buffer_height;
    /*created font have format*/
    param.format = pTxtParam->format;
    
    param.clip_rect = param.display_rect;
    param.buffer_ptr = pTxtParam->buffer_ptr;
    if(param.buffer_ptr == NULL) return FALSE;
    SPMLAPI_DrawText( &param, lpszString, cbCount, &metrics_data );
    
    return metrics_data.width;
}

/*****************************************************************************/
//  Description : WRE_GetScreenLogicSize
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
void WRE_GetScreenLogicSize(int* width,int* height)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;

	if(width == NULL || height == NULL)
		return;
        
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    //heng.xiao add begin 20120310
    //note:if WRE support after,must del codes as follow 
    //heng.xiao add 20120310 for error when invert screen because WRE do not support invertint screen
    if(lcd_width > lcd_height)
    {//swap
        int tmp;
        tmp = lcd_width;
        lcd_width = lcd_height;
        lcd_height = tmp;
    }
    //heng.xiao add end 20120310
    
    *width=lcd_width;
    *height=lcd_height;
}

#endif

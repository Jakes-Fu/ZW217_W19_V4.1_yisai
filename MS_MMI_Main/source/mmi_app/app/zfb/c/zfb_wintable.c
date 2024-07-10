/****************************************************************************
** File Name:      mmiphsapp_wintab.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef _MMIZFB_WINTAB_C_
#define _MMIZFB_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmk_app.h"
#include "mmicc_export.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "guiedit.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmi_common.h"

#include "zfb_app.h"
#include "mmiidle_export.h"
#include "mmi_position.h"
#include "ldo_drvapi.h"
#include "img_dec_interface.h"
#include "guiownerdraw.h"
#include "graphics_draw.h"
#include "mmiparse_export.h"
#include "mmicl_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef ENG_SUPPORT
#include "mmieng_export.h"
#endif
#if defined(MMI_INDIAN_SOS_SUPPORT) && !defined(MMI_PDA_SUPPORT)
#include "mmiset_export.h"
#endif
#include "qr_encode.h"

#define ZFB_TP_OFFSET_VALUE 20

PUBLIC void MMIZFB_OpenMenuWin(void);
PUBLIC void MMIZFB_OpenPayWin(void);
PUBLIC void MMIZFB_OpenObtainWin(void);

uint8 g_zfb_pay_display_timer_id = 0;
uint8 g_zfb_bind_display_timer_id = 0;
uint8 g_zfb_obtain_display_timer_id = 0;

void alipay_qr_fillrect(int x1, int y1, int x2, int y2,  GUI_COLOR_T color)
{
    const GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};
    rect.left = x1;
    rect.top = y1;
    rect.right = x2;
    rect.bottom = y2;

    GUI_FillRect( &lcd_dev_info, rect, color);
}

void alipay_showQR(uint8_t *data, int size, int pos_x, int pos_y, int zoom_val)
{
    int i=0,j=0,a=0;  
    int x1, y1, x2, y2;
    uint8_t cur_data = 0;
    uint8_t cur_bit = 0;
    uint8_t cur_val = 0;
    for(i=0;i<size;i++)  
    {  
        for(j=0;j<size;j++)
        {  
            a = i * size + j;
            cur_data = data[a / 8];
            cur_bit = a%8;
            cur_bit = 7 - cur_bit;
            cur_val = cur_data & (0x01 << cur_bit);
        	x1 = pos_x + j * zoom_val;
        	y1 = pos_y + i * zoom_val;
        	x2 = x1 + zoom_val-1;
        	y2 = y1 + zoom_val-1;
            if(cur_val)
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);    
            }  
            else  
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
            }  
        }  
    }  
}

uint32_t alipay_showBarCode(uint16_t *data, int size, int pos_x, int pos_y, int zoom_val)
{
    int i=0,j=0;  
    int x1, y1, x2, y2;
    uint16_t cur_data = 0;
    uint16_t cur_bit = 0;
    uint16_t cur_val = 0;
	uint32_t diplay_len = 0;

    if(size < 3)
    {
        return;
    }
    y1 = pos_y;
    y2 = y1 + 72*zoom_val - 1;
	x1 = pos_x;
	x2 = pos_x+zoom_val-1;
    for(i=0;i<size-1;i++)  
    {  
        cur_data =data[i];
        for(j=0;j<11;j++)
        {  
            cur_bit = 10-j;
            cur_val = cur_data & (0x0001 << cur_bit);
			if(x2 < MMI_MAINSCREEN_WIDTH)
			{
				if(cur_val)
				{  
					alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);
				}  
				else  
				{  
					alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
				}  
			}
			x1 += zoom_val;
        	x2 = x1 + zoom_val-1;
        }  
    }  

	cur_data =data[size-1];
	for(j=0;j<12;j++)
	{  
		cur_bit = 11-j;
		cur_val = cur_data & (0x0001 << cur_bit);
		if(x2 < MMI_MAINSCREEN_WIDTH)
		{
			if(cur_val)
			{  
				alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);
			}  
			else  
			{  
				alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
			}  

		}
		x1 += zoom_val;
		x2 = x1 + zoom_val-1;
    }  

	diplay_len = x2 - pos_x +1;
	return diplay_len;
}

static const float _sin_0_90_table[91] = 
{
    0,
    0.017, 0.034, 0.052, 0.069, 0.087, 0.104, 0.121, 0.139, 0.156, 0.173,
    0.190, 0.207, 0.224, 0.241, 0.258, 0.275, 0.292, 0.309, 0.325, 0.342,
    0.358, 0.374, 0.390, 0.406, 0.422, 0.438, 0.453, 0.469, 0.484, 0.499,
    0.515, 0.529, 0.544, 0.559, 0.573, 0.587, 0.601, 0.615, 0.629, 0.642,
    0.656, 0.669, 0.681, 0.694, 0.707, 0.719, 0.731, 0.743, 0.754, 0.766,
    0.777, 0.788, 0.798, 0.809, 0.819, 0.829, 0.838, 0.848, 0.857, 0.866,
    0.874, 0.882, 0.891, 0.898, 0.906, 0.913, 0.920, 0.927, 0.933, 0.939,
    0.945, 0.951, 0.956, 0.961, 0.965, 0.970, 0.974, 0.978, 0.981, 0.984,
    0.987, 0.990, 0.992, 0.994, 0.996, 0.997, 0.998, 0.999, 0.999, 1.000,
};

static float SIN_S(int16 angle)
{
    
    if(angle < 0)
    {
        return -SIN_S(-angle);
    }
    angle %= 360;

    if(angle <= 90)
    {
        return _sin_0_90_table[angle];
    }
    else if (angle <= 180)
    {
        return _sin_0_90_table[180 - angle];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[angle - 180];
    }
    else //if (angle <= 360)
    {
        return -_sin_0_90_table[360 - angle];
    }
}

/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
static float COS_S(int16 angle)
{
    if(angle<0)
    {
        angle = -angle; //Cos(-x)=Cos(x);
    }
    angle %= 360;

    if (angle <= 90)
    {
        return _sin_0_90_table[90 - angle];
    }
    else if (angle <= 180)
    {
        return -_sin_0_90_table[angle - 90];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[270 - angle];
    }
    else //if (angle <= 360)
    {
        return _sin_0_90_table[angle - 270];
    }  
}


uint32_t alipay_showBarCodeLine(uint16_t *data, int size, int pos_x, int pos_y)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    int i=0,j=0;  
    int x,y,x1, y1, x2, y2;
    uint16_t cur_data = 0;
    uint16_t cur_bit = 0;
    uint16_t cur_val = 0;
    uint16_t pre_val = 0;
    uint32_t diplay_len = 0;
    float  s,c;

    if(size < 3)
    {
        return;
    }

    diplay_len = size*11+1;
    
    s = SIN_S(45);
    s = s * diplay_len;
    x = s;
    
    
    if(x >= MMI_MAINSCREEN_WIDTH)
    {
        y = 0;
    }
    else
    {
        y = MMI_MAINSCREEN_WIDTH-x;
    }
    x1 = 0;
    y1 = y;
    x2 = y;
    y2 = 0;
    for(i=0;i<size-1;i++)  
    {  
        cur_data =data[i];
        for(j=0;j<11;j++)
        {  
            cur_bit = 10-j;
            cur_val = cur_data & (0x0001 << cur_bit);
            if(x2 < MMI_MAINSCREEN_WIDTH)
            {
                if(cur_val)
                {  
                	LCD_DrawLine(&lcd_dev_info,x1,y1,x2,y2,MMI_BLACK_COLOR);
                   LCD_DrawLine(&lcd_dev_info,x1+1,y1,x2,y2+1,MMI_BLACK_COLOR);
                }  
                else  
                {  
                	LCD_DrawLine(&lcd_dev_info,x1,y1,x2,y2,MMI_WHITE_COLOR);    
                }  
            }
            pre_val = cur_val;
            x1++;
            x2++;
            y1++;
            y2++;
        }  
    }  

    cur_data =data[size-1];
    for(j=0;j<12;j++)
    {  
        cur_bit = 11-j;
        cur_val = cur_data & (0x0001 << cur_bit);
        if(x2 < MMI_MAINSCREEN_WIDTH)
        {
            if(cur_val)
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);
            }  
            else  
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
            }  
        }
        x1++;
        x2++;
        y1++;
        y2++;
    }  

	return diplay_len;
}

void alipay_show_mont(uint8 bitmap_amount[32][14], int pos_x, int pos_y, int zoom_val)
{
    int i=0,j=0,a=0;  
    int x1, y1, x2, y2;
    uint8_t cur_data = 0;
    uint8_t cur_bit = 0;
    uint8_t cur_val = 0;
    for(i=0;i<32;i++)  
    {  
        for(j=0;j<112;j++)
        {  
            a = j/8;
            cur_data = bitmap_amount[i][a];
            cur_bit = j%8;
            cur_bit = 7 - cur_bit;
            cur_val = cur_data & (0x01 << cur_bit);
        	x1 = pos_x + j * zoom_val;
        	y1 = pos_y + i * zoom_val;
        	x2 = x1 + zoom_val-1;
        	y2 = y1 + zoom_val-1;
            if(cur_val)
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);    
            }  
            else  
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
            }  
        }  
    }  
}
void alipay_showMount(uint8_t *data, int size, int pos_x, int pos_y, int zoom_val)
{
    int i=0,j=0,a=0;  
    int x1, y1, x2, y2;
    uint8_t cur_data = 0;
    uint8_t cur_bit = 0;
    uint8_t cur_val = 0;
    for(i=0;i<size;i++)  
    {  
        for(j=0;j<size;j++)
        {  
            a = i * size + j;
            cur_data = data[a / 8];
            cur_bit = a%8;
            cur_bit = 7 - cur_bit;
            cur_val = cur_data & (0x01 << cur_bit);
        	x1 = pos_x + j * zoom_val;
        	y1 = pos_y + i * zoom_val;
        	x2 = x1 + zoom_val-1;
        	y2 = y1 + zoom_val-1;
            if(cur_val)
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_BLACK_COLOR);    
            }  
            else  
            {  
                alipay_qr_fillrect(x1,y1,x2,y2,MMI_WHITE_COLOR);    
            }  
        }  
    }  
}
static void MMIZFB_BackLight(BOOLEAN is_alway_on)
{
      if(is_alway_on)
      {
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		MMIDEFAULT_TurnOnBackLight();
      }
      else
      {
		MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		MMIDEFAULT_SetAlwaysHalfOnBackLight(FALSE);
      }
}

static void MMIZFB_OpenAlertWin(MMI_TEXT_ID_T text1_id,MMI_TEXT_ID_T text2_id)
{
    uint32 ms = 3000;
    MMI_WIN_ID_T    alert_win_id = MMIZFB_ALERT_WIN_ID;
    
    MMIPUB_OpenAlertWinByTextId(&ms, text1_id,
                                            text2_id, IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
}

static void ZFB_OBTAIN_Display_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(MMK_IsOpenWin(MMIZFB_OBTAIN_WIN_ID))
    {
        MMK_CloseWin(MMIZFB_OBTAIN_WIN_ID);
    }
}

void ZFB_OBTAIN_Display_TimeStart(uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 30;
    }

    if(0 != g_zfb_obtain_display_timer_id)
    {
        MMK_StopTimer(g_zfb_obtain_display_timer_id);
        g_zfb_obtain_display_timer_id = 0;
    }
    
    g_zfb_obtain_display_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        ZFB_OBTAIN_Display_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void ZFB_OBTAIN_Display_TimeStop(void)
{
    if(0 != g_zfb_obtain_display_timer_id)
    {
        MMK_StopTimer(g_zfb_obtain_display_timer_id);
        g_zfb_obtain_display_timer_id = 0;
    }
}

static void ZFB_PAY_Display_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(MMK_IsOpenWin(MMIZFB_PAY_WIN_ID))
    {
        MMK_CloseWin(MMIZFB_PAY_WIN_ID);
    }
}

void ZFB_PAY_Display_TimeStart(uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 30;
    }

    if(0 != g_zfb_pay_display_timer_id)
    {
        MMK_StopTimer(g_zfb_pay_display_timer_id);
        g_zfb_pay_display_timer_id = 0;
    }
    
    g_zfb_pay_display_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        ZFB_PAY_Display_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void ZFB_PAY_Display_TimeStop(void)
{
    if(0 != g_zfb_pay_display_timer_id)
    {
        MMK_StopTimer(g_zfb_pay_display_timer_id);
        g_zfb_pay_display_timer_id = 0;
    }
}

static void ZFB_BIND_Display_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(MMK_IsOpenWin(MMIZFB_BIND_WIN_ID))
    {
        MMK_CloseWin(MMIZFB_BIND_WIN_ID);
    }
}

void ZFB_BIND_Display_TimeStart(uint16 time_s)
{
    if(time_s == 0)
    {
        time_s = 30;
    }

    if(0 != g_zfb_bind_display_timer_id)
    {
        MMK_StopTimer(g_zfb_bind_display_timer_id);
        g_zfb_bind_display_timer_id = 0;
    }
    
    g_zfb_bind_display_timer_id = MMK_CreateTimerCallback(time_s*1000, 
                                                                        ZFB_BIND_Display_TimeOut, 
                                                                        0, 
                                                                        FALSE);
}

void ZFB_BIND_Display_TimeStop(void)
{
    if(0 != g_zfb_bind_display_timer_id)
    {
        MMK_StopTimer(g_zfb_bind_display_timer_id);
        g_zfb_bind_display_timer_id = 0;
    }
}

static BOOLEAN ZFB_DisplayBmpBuf(uint16 ox, uint16 oy,double scale,uint8* input, uint32 input_len)
{
    BOOLEAN result = FALSE;
    int32 decodeBufSize = 0;
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T  rect = {0};
    GUI_POINT_T point = {0};
    GUIIMG_BITMAP_T bitmap_ptr = {0};
    GUIIMG_SRC_T		img_src = {0};     
    GUIIMG_DISPLAY_T	img_display = {0};  
    GUIIMG_DEC_OUT_T	img_output = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    IMG_DEC_SRC_T       dec_src = {0};
    int w,h;
    double dw,dh;
    dec_src.src_ptr       = (unsigned char*)input;
    dec_src.src_file_size = input_len;
    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
    {	
        return FALSE;
    }
    w=dec_info.img_detail_info.bmp_info.img_width;
    h=dec_info.img_detail_info.bmp_info.img_height;

    decodeBufSize = h*w*6;
    bitmap_ptr.bit_ptr = (void*)SCI_ALLOCA(decodeBufSize);	

    if(bitmap_ptr.bit_ptr == NULL)
    {
        return FALSE;
    }
    img_src.is_file = FALSE;
    img_src.src_buf_ptr = input;
    img_src.src_data_size = input_len;

    SCI_MEMSET(bitmap_ptr.bit_ptr, 0, decodeBufSize);
    dw = w*scale;
    dh = h*scale;
    img_display.dest_width = dw;
    img_display.dest_height = dw;
    img_output.decode_data_ptr = bitmap_ptr.bit_ptr;
    img_output.decode_data_size = decodeBufSize;
    img_output.actual_height= h;
    img_output.actual_width = w;
    point.x = ox;
    point.y = oy;
    rect.left = ox;
    rect.top  = oy;
    rect.right = ox + w - 1;
    rect.bottom= oy + h - 1;

    if(GUIIMG_Decode(&img_src, &img_display, &img_output))
    {
        bitmap_ptr.img_height = img_output.actual_height;
        bitmap_ptr.img_width = img_output.actual_width;
        if(GUIIMG_DisplayBmp(FALSE, &rect, &point, (void*)&bitmap_ptr, &dev_info))
        {
            result = TRUE;
        }
    }
    SCI_FREE(bitmap_ptr.bit_ptr);

    return result;
}

#if 1
//21 的倍数
#if MMI_MAINSCREEN_WIDTH == 240
#define ZFB_OBTAIN_DISP_X      33
#define ZFB_OBTAIN_DISP_Y	    33
#define ZFB_OBTAIN_DISP_WIDTH  	174
#define ZFB_OBTAIN_DISP_HEIGHT	174
#else
#define ZFB_OBTAIN_DISP_X      22
#define ZFB_OBTAIN_DISP_Y	    22
#define ZFB_OBTAIN_DISP_WIDTH	      84
#define ZFB_OBTAIN_DISP_HEIGHT	84
#endif

uint16 g_cur_zfb_obtain_display_status= 0;
uint16 g_cur_zfb_obtain_display_over_status= 0;

static BOOLEAN ZFB_Obtain_2VM_BmpShow(uint8 * qData,uint32 DataLen)
{
    BOOLEAN                 result = FALSE;
    uint8 * p_img_buf = NULL;
    uint32 img_len = 0;
    
    ZFB_2VM_ConvertTo_Bmp(qData,DataLen,4,&p_img_buf,&img_len);

    if(p_img_buf != NULL && img_len > 0)
    {
        ZFB_DisplayBmpBuf(ZFB_OBTAIN_DISP_X,ZFB_OBTAIN_DISP_Y,1,p_img_buf,img_len);
    }

    if(p_img_buf != NULL)
    {
        SCI_FREE(p_img_buf);
        p_img_buf = NULL;
    }
}

LOCAL BOOLEAN  ZFB_Obtain_Show_2VM_QR(void)
{
    int x,y;
    int zoom_val;
    char ewm_str[ZFB_ALIPAY_OBTAIN_CODE_MAX_LEN+1] = {0};
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    
    if(g_zfb_alipay_obtain_code_len > 0)
    {
        SCI_MEMCPY(ewm_str,g_zfb_alipay_obtain_code,g_zfb_alipay_obtain_code_len);
    }
    side = qr_encode(QR_LEVEL_Q, 0, ewm_str, g_zfb_alipay_obtain_code_len, bitdata);
    #if 0
        ZFB_Obtain_2VM_BmpShow(bitdata,side);
    #else
    if(side > ZFB_OBTAIN_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
            y = (ZFB_OBTAIN_DISP_WIDTH - (zoom_val*side))/2;
        }
        else
        {
            //显示异常
            x = 0;
            y = ZFB_OBTAIN_DISP_Y;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = ZFB_OBTAIN_DISP_WIDTH/side;
        x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
        y = (ZFB_OBTAIN_DISP_WIDTH - (zoom_val*side))/2+ZFB_OBTAIN_DISP_Y;
    }
    alipay_showQR(bitdata, side, x, y, zoom_val);
    //GT_Qcode_show(ewm_str, ZFB_OBTAIN_DISP_X, ZFB_OBTAIN_DISP_Y, ZFB_OBTAIN_DISP_WIDTH, ZFB_OBTAIN_DISP_HEIGHT);
    #endif
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show_BG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show_2VM(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_OBTAIN,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_20;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=ZFB_OBTAIN_DISP_Y-4;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    text_style.font = SONG_FONT_20;
    text_display_rect.left = 0;
    text_display_rect.top = ZFB_OBTAIN_DISP_Y+ZFB_OBTAIN_DISP_HEIGHT+4;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=MMI_MAINSCREEN_HEIGHT-1;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_UPDATE,&sn_string);
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    ZFB_Obtain_Show_2VM_QR();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show_Net(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;

    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_INITING,&sn_string);
        
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_24;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 80;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=160;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show_Status0(void)
{
    ZFB_Obtain_Show_BG();
    ZFB_Obtain_Show_2VM();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show_Status1(void)
{
    ZFB_Obtain_Show_BG();
    ZFB_Obtain_Show_Net();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Obtain_Show(void)
{
    if(g_cur_zfb_obtain_display_status == 0)
    {
        ZFB_Obtain_Show_Status0();
    }
    else if(g_cur_zfb_obtain_display_status == 1)
    {
        ZFB_Obtain_Show_Status1();
    }
    else
    {
        ZFB_Obtain_Show_Status0();
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZFB_ObtainWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_WIN_ID_T    alert_win_id = MMIZFB_ALERT_WIN_ID;
    static GUI_POINT_T g_obtain_tp_disp_old_pt = {0};
    
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
                MMIZFB_BackLight(TRUE);
                if(g_zfb_alipay_obtain_code_len > 0)
                {
                    g_cur_zfb_obtain_display_status = 0;
                    g_cur_zfb_obtain_display_over_status = 0;
                }
                else
                {

                    g_cur_zfb_obtain_display_status = 1;
                    g_cur_zfb_obtain_display_over_status = 0;
                }
                ZFBPay_GetObtainCode(TRUE);
                ZFB_OBTAIN_Display_TimeStart(60);
                g_obtain_tp_disp_old_pt.x = 0;
                g_obtain_tp_disp_old_pt.y = 0;
            break;
			
	case MSG_FULL_PAINT:
                 ZFB_Obtain_Show();
		break;
        
        case MSG_ZDB_STATUS_OBTAIN:
                g_cur_zfb_obtain_display_status = 0;
                ZFB_Obtain_Show_Status0();
            break;

        case MSG_ZDB_STATUS_ERR_INIT:
                MMIZFB_OpenAlertWin(TXT_ZFB_INIT_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
        case MSG_ZDB_STATUS_ERR_BIND:
                MMIZFB_OpenAlertWin(TXT_ZFB_BIND_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
        case MSG_ZDB_STATUS_ERR_OBTAIN:
                MMIZFB_OpenAlertWin(TXT_ZFB_OBTAIN_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            g_obtain_tp_disp_old_pt = point;
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            ZFBPay_GetObtainCode(FALSE);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
                MMIZFB_BackLight(FALSE);
                ZFB_OBTAIN_Display_TimeStop();
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_OBTAIN_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZFB_ObtainWinMsg ),
    WIN_ID(MMIZFB_OBTAIN_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenObtainWin(void)
{
    MMK_CreateWin((uint32 *)MMIZFB_OBTAIN_WIN_TAB,PNULL);
}
#endif

#if 1
//21 的倍数
#if MMI_MAINSCREEN_WIDTH == 240
#define ZFB_PAY_DISP_X      45
#define ZFB_PAY_DISP_Y	    45
#define ZFB_PAY_DISP_WIDTH  	147
#define ZFB_PAY_DISP_HEIGHT	147
#else
#define ZFB_PAY_DISP_X      22
#define ZFB_PAY_DISP_Y	    22
#define ZFB_PAY_DISP_WIDTH	      84
#define ZFB_PAY_DISP_HEIGHT	84
#endif

uint16 g_cur_zfb_pay_display_status= 0;
uint16 g_cur_zfb_pay_display_over_status= 0;

static BOOLEAN ZFB_Pay_2VM_BmpShow(uint8 * qData,uint32 DataLen)
{
    BOOLEAN                 result = FALSE;
    uint8 * p_img_buf = NULL;
    uint32 img_len = 0;
    
    ZFB_2VM_ConvertTo_Bmp(qData,DataLen,4,&p_img_buf,&img_len);

    if(p_img_buf != NULL && img_len > 0)
    {
        ZFB_DisplayBmpBuf(ZFB_PAY_DISP_X,ZFB_PAY_DISP_Y,1,p_img_buf,img_len);
    }

    if(p_img_buf != NULL)
    {
        SCI_FREE(p_img_buf);
        p_img_buf = NULL;
    }
}

LOCAL BOOLEAN  ZFB_Pay_Show_2VM_QR(void)
{
    int x,y;
    int zoom_val;
    char ewm_str[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1] = {0};
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    
    if(g_zfb_alipay_pay_code_len > 0)
    {
        SCI_MEMCPY(ewm_str,g_zfb_alipay_pay_code,g_zfb_alipay_pay_code_len);
    }
    side = qr_encode(QR_LEVEL_Q, 0, ewm_str, g_zfb_alipay_pay_code_len, bitdata);
    #if 0
        ZFB_Pay_2VM_BmpShow(bitdata,side);
    #else
    if(side > ZFB_PAY_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
            y = (ZFB_PAY_DISP_WIDTH - (zoom_val*side))/2;
        }
        else
        {
            //显示异常
            x = 0;
            y = ZFB_PAY_DISP_Y;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = ZFB_PAY_DISP_WIDTH/side;
        x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
        y = (ZFB_PAY_DISP_WIDTH - (zoom_val*side))/2+ZFB_PAY_DISP_Y;
    }
    alipay_showQR(bitdata, side, x, y, zoom_val);
    //GT_Qcode_show(ewm_str, ZFB_PAY_DISP_X, ZFB_PAY_DISP_Y, ZFB_PAY_DISP_WIDTH, ZFB_PAY_DISP_HEIGHT);
    #endif
    return TRUE;
}

static BOOLEAN ZFB_Pay_BarCode_BmpShow(uint16 * qData,uint32 DataLen,uint16 zoom_lv,double scale)
{
    BOOLEAN                 result = FALSE;
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    uint8 * p_img_buf = NULL;
    uint16 x = 0;
    uint16 y = 0;
    uint32 img_len = 0;
    uint16 disp_w = 0;
    uint16 disp_h = 0;
    double disp_pix = DataLen*11+1;
    disp_pix = disp_pix *zoom_lv*scale;
    disp_w = disp_pix;
    disp_h = disp_w/2;
    if(disp_w > MMI_MAINSCREEN_WIDTH)
    {
        x = 4;
    }
    else
    {
        x = (MMI_MAINSCREEN_WIDTH-disp_w)/2;
    }
    y = ZFB_PAY_DISP_Y+4;
    ZFB_BarCode_ConvertTo_Bmp(qData,DataLen,zoom_lv,&p_img_buf,&img_len);

    if(p_img_buf != NULL && img_len > 0)
    {
        ZFB_DisplayBmpBuf(x,y,scale,p_img_buf,img_len);
    }

    if(p_img_buf != NULL)
    {
        SCI_FREE(p_img_buf);
        p_img_buf = NULL;
    }
}

LOCAL BOOLEAN  ZFB_Pay_Show_BarCode_QR(void)
{
    int x,y;
    int zoom_val;
    char ewm_str[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1] = {0};
    uint16_t bitdata[CODE128_MEX_LEN+1] = {0};
    int side = 0;
    uint32 disp_pix = 0;
    uint16 room_lv = 0;
    double scale = 1;
    double temp = 0;
    if(g_zfb_alipay_pay_code_len > (CODE128_MEX_LEN-3))
    {
        return FALSE;
    }
    if(g_zfb_alipay_pay_code_len > 0)
    {
        SCI_MEMCPY(ewm_str,g_zfb_alipay_pay_code,g_zfb_alipay_pay_code_len);
    }
    if((g_zfb_alipay_pay_code_len%2) == 0)
    {
        room_lv = 2;
        side = code128C_encode(ewm_str, g_zfb_alipay_pay_code_len, bitdata);
    }
    else
    {
        room_lv = 1;
        side = code128B_encode(ewm_str, g_zfb_alipay_pay_code_len, bitdata);
    }
    disp_pix = side*11+1;
#if 0
    disp_pix = disp_pix*room_lv;
    if(disp_pix >= MMI_MAINSCREEN_WIDTH)
    {
        if(disp_pix > MMI_MAINSCREEN_WIDTH-20)
        {
            temp = disp_pix;
            temp = (MMI_MAINSCREEN_WIDTH-20)/temp;
            scale = temp;
        }
        else
        {
            scale = 1;
        }
        ZFB_Pay_BarCode_BmpShow(bitdata,side,room_lv,scale);
    }
    else
#endif
    {
        if(disp_pix > ZFB_PAY_DISP_WIDTH)
        {
            if(disp_pix < MMI_MAINSCREEN_WIDTH)
            {
                zoom_val = MMI_MAINSCREEN_WIDTH/disp_pix;
                x = (MMI_MAINSCREEN_WIDTH - (zoom_val*disp_pix))/2;
                y = ZFB_PAY_DISP_Y+20;
            }
            else
            {
                //显示异常
                x = 0;
                y = ZFB_PAY_DISP_Y;
                zoom_val = 1;
            }
        }
        else
        {
            zoom_val = ZFB_PAY_DISP_WIDTH/disp_pix;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*disp_pix))/2;
            y = ZFB_PAY_DISP_Y+15;
        }
        alipay_showBarCode(bitdata, side, x, y, zoom_val);
        //alipay_showBarCodeLine(bitdata, side, x, y);
    }
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Mount(void)
{
    int x,y;
    x = 8;
    y = 42;
    alipay_show_mont(g_zfb_cur_pay_amount, x, y, 2);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Fail_Result(void)
{
    uint32 err_code = 0;
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN;
    uint16 txt_user_unbind[10] = {0x7528, 0x6237, 0x5DF2, 0x89E3, 0x90A6, 0x0}; //用户已解邦
    uint16 txt_user_forbind[10] = {0x652F, 0x4ED8, 0x529F, 0x80FD, 0x88AB, 0x7981, 0x7528, 0x0}; //支付功能被禁用
    uint16 txt_user_net[10] = {0x7F51, 0x7EDC, 0x9519, 0x8BEF, 0x0}; //网络错误
    uint16 txt_user_unknow[10] = {0x672A, 0x77E5, 0x9519, 0x8BEF, 0x0}; //未知错误
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_PAYING_FAIL,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_30;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=40;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    if((g_zfb_cur_pay_result & 0xFF000000) > 0)
    {
        err_code = g_zfb_cur_pay_result & 0x00FFFFFF;
        if(ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER == err_code)
        {
            sn_string.wstr_ptr = txt_user_unbind;
            sn_string.wstr_len = MMIAPICOM_Wstrlen(txt_user_unbind);
        }
        else if(ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER == err_code)
        {
            sn_string.wstr_ptr = txt_user_forbind;
            sn_string.wstr_len = MMIAPICOM_Wstrlen(txt_user_forbind);
        }
        else
        {
            sn_string.wstr_ptr = txt_user_unknow;
            sn_string.wstr_len = MMIAPICOM_Wstrlen(txt_user_unknow);
        }
    }
    else
    {
        err_code = g_zfb_cur_pay_result;
        sn_string.wstr_ptr = txt_user_unknow;
        sn_string.wstr_len = MMIAPICOM_Wstrlen(txt_user_unknow);
    }

    
    text_style.font_color = MMI_DARK_RED_COLOR;
    text_style.font = SONG_FONT_24;
    text_display_rect.left = 0;
    text_display_rect.top = 80;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=160;

    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}
LOCAL BOOLEAN  ZFB_Pay_Show_BG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_2VM(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_2VM,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_20;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=ZFB_PAY_DISP_Y-4;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    text_style.font = SONG_FONT_16;
    text_display_rect.left = 0;
    text_display_rect.top = ZFB_PAY_DISP_Y+ZFB_PAY_DISP_HEIGHT+6;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=text_display_rect.top+20;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_UPDATE,&sn_string);
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    text_style.font_color = MMI_DARK_BLUE_COLOR;
    text_style.font = SONG_FONT_16;
    text_display_rect.top = text_display_rect.bottom+2;
    text_display_rect.bottom=text_display_rect.top+20;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_SWITCH_PAY,&sn_string);
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    ZFB_Pay_Show_2VM_QR();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_BarCode(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    uint16 bar_wstr[ZFB_ALIPAY_PAY_CODE_MAX_LEN+1] = {0};
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_PACT,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_20;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=ZFB_PAY_DISP_Y-4;	
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    text_style.font = SONG_FONT_16;
    text_display_rect.left = 0;
    text_display_rect.top = ZFB_PAY_DISP_Y+ZFB_PAY_DISP_HEIGHT+4;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=text_display_rect.top+20;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_UPDATE,&sn_string);
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    text_style.font_color = MMI_DARK_BLUE_COLOR;
    text_style.font = SONG_FONT_16;
    text_display_rect.top = text_display_rect.bottom+2;
    text_display_rect.bottom=text_display_rect.top+20;
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_SWITCH_OBTAIN,&sn_string);
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );


    ZFB_Pay_Show_BarCode_QR();
    if(g_zfb_alipay_pay_code_len > 0)
    {
        MMIAPICOM_StrToWstr(g_zfb_alipay_pay_code, bar_wstr);
        sn_string.wstr_ptr = bar_wstr;
        sn_string.wstr_len = MMIAPICOM_Wstrlen(bar_wstr);
        #if 0
        if(sn_string.wstr_len > 10)
        {
            bar_wstr[11] = '.';
            bar_wstr[12] = '.';
            bar_wstr[13] = '.';
            bar_wstr[14] = 0;
            sn_string.wstr_len = 14;
        }
        #endif
    text_style.font_color = MMI_BLACK_COLOR;
        text_style.font = SONG_FONT_16;
        text_display_rect.left = 0;
        text_display_rect.top = ZFB_PAY_DISP_Y+ZFB_PAY_DISP_HEIGHT-30;
        text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
        text_display_rect.bottom=ZFB_PAY_DISP_Y+ZFB_PAY_DISP_HEIGHT;
        GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
        GUISTR_DrawTextToLCDInRect( 
                                                MMITHEME_GetDefaultLcdDev(),
                                                (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                                (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                                (const MMI_STRING_T    *)&sn_string,
                                                &text_style,
                                                str_state,
                                                GUISTR_TEXT_DIR_AUTO
                                                );
    }
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Success(uint16 status)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_POINT_T                 dis_point = {80,130};
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    
    GUIRES_DisplayImg(&dis_point,
                                    &bg_rect,
                                    PNULL,
                                    MMIZFB_PAY_WIN_ID,
                                    IMAGE_ZFB_PAY_OK_IMG,
                                    &lcd_dev_info);
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_PAYING_OK,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_DARK_GREEN_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_30;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=40;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    ZFB_Pay_Show_Mount();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Net(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;

    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_INITING,&sn_string);
        
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_24;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 80;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=160;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Status0(void)
{
    ZFB_Pay_Show_BG();
    ZFB_Pay_Show_2VM();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Status1(void)
{
    ZFB_Pay_Show_BG();
    ZFB_Pay_Show_BarCode();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Status2(uint16 status)
{
    ZFB_Pay_Show_BG();
    if(status == 1)
    {
        ZFB_Pay_Show_Fail_Result();
    }
    else
    {
        ZFB_Pay_Show_Success(status);
    }
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Pay_Show_Status3(void)
{
    ZFB_Pay_Show_BG();
    ZFB_Pay_Show_Net();
    return TRUE;
}
LOCAL BOOLEAN  ZFB_Pay_Show(void)
{
    if(g_cur_zfb_pay_display_status == 0)
    {
        ZFB_Pay_Show_Status0();
    }
    else if(g_cur_zfb_pay_display_status == 1)
    {
        ZFB_Pay_Show_Status1();
    }
    else if(g_cur_zfb_pay_display_status == 2)
    {
        ZFB_Pay_Show_Status2(g_cur_zfb_pay_display_over_status);
    }
    else if(g_cur_zfb_pay_display_status == 3)
    {
        ZFB_Pay_Show_Status3();
    }
    else
    {
        ZFB_Pay_Show_Status0();
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZFB_PayWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_WIN_ID_T    alert_win_id = MMIZFB_ALERT_WIN_ID;
    static GUI_POINT_T g_pay_tp_disp_old_pt = {0};
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
                MMIZFB_BackLight(TRUE);
                if(g_zfb_alipay_pay_code_len > 0)
                {
                    g_cur_zfb_pay_display_status = 0;
                    g_cur_zfb_pay_display_over_status = 0;
                }
                else
                {

                    g_cur_zfb_pay_display_status = 3;
                    g_cur_zfb_pay_display_over_status = 0;
                }
                ZFBPay_GetPayCode(TRUE);
                if(ZFB_Net_IsAllow())
                {
                    ZFBPay_QueryPayResult();
                }
                ZFB_PAY_Display_TimeStart(60);
                g_pay_tp_disp_old_pt.x = 0;
                g_pay_tp_disp_old_pt.y = 0;
            break;
			
	case MSG_FULL_PAINT:
                 ZFB_Pay_Show();
		break;
        
        case MSG_ZDB_STATUS_PAY:
                ZFB_Pay_Show();
            break;
            
         case MSG_ZDB_PAYING_OK:
                g_cur_zfb_pay_display_status = 2;
                ZFBPay_QueryPayStop();
                ZFB_Pay_Show_Status2(0);
                ZFB_PAY_Display_TimeStart(4);
            break;
            
         case MSG_ZDB_PAYING_ERROR:
                ZFB_Pay_Show_Status2(1);
                ZFB_PAY_Display_TimeStart(4);
                //MMIZFB_OpenAlertWin(TXT_ZFB_PAYING_FAIL,TXT_NULL);
                //MMK_CloseWin(win_id);
                //MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;

        case MSG_ZDB_STATUS_ERR_INIT:
                MMIZFB_OpenAlertWin(TXT_ZFB_INIT_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
        case MSG_ZDB_STATUS_ERR_BIND:
                MMIZFB_OpenAlertWin(TXT_ZFB_BIND_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
        case MSG_ZDB_STATUS_ERR_PAY:
                MMIZFB_OpenAlertWin(TXT_ZFB_PAY_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIZFB_MENU_WIN_ID);
            break;
            
    case MSG_ZDB_STATUS_ERR:
            MMIZFB_OpenAlertWin(TXT_ZFB_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            g_pay_tp_disp_old_pt = point;

        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            int offset_y =  0;
            int offset_x =  0;
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            offset_y =  point.y - g_pay_tp_disp_old_pt.y;
            offset_x =  point.x - g_pay_tp_disp_old_pt.x;
            	if(offset_x <= -(ZFB_TP_OFFSET_VALUE))	
        	{
                //Left
                if(g_cur_zfb_pay_display_status  == 0)
                {
                    g_cur_zfb_pay_display_status = 1;
                    ZFB_Pay_Show_Status1();
                }
                else if(g_cur_zfb_pay_display_status  == 1)
                {
                    g_cur_zfb_pay_display_status = 0;
                    ZFB_Pay_Show_Status0();
                }
        	}	
        	else if(offset_x >= (ZFB_TP_OFFSET_VALUE))
        	{
                //Right
                if(g_cur_zfb_pay_display_status  == 0)
                {
                    g_cur_zfb_pay_display_status = 1;
                    ZFB_Pay_Show_Status1();
                }
                else if(g_cur_zfb_pay_display_status  == 1)
                {
                    g_cur_zfb_pay_display_status = 0;
                    ZFB_Pay_Show_Status0();
                }
        	}
            else
            {
                ZFBPay_GetPayCode(FALSE);
            }
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
                ZFBPay_QueryPayStop();
                MMIZFB_BackLight(FALSE);
                ZFB_PAY_Display_TimeStop();
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_PAY_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZFB_PayWinMsg ),
    WIN_ID(MMIZFB_PAY_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenPayWin(void)
{
    MMK_CreateWin((uint32 *)MMIZFB_PAY_WIN_TAB,PNULL);
}
#endif


#if 1
//33的倍 数
#if MMI_MAINSCREEN_HEIGHT == 284
#define ZFB_BIND_DISP_X	37
#define ZFB_BIND_DISP_Y	(37+23)
#define ZFB_BIND_DISP_WIDTH	165
#define ZFB_BIND_DISP_HEIGHT    165
#elif MMI_MAINSCREEN_HEIGHT == 240
#define ZFB_BIND_DISP_X	37
#define ZFB_BIND_DISP_Y	37
#define ZFB_BIND_DISP_WIDTH	165
#define ZFB_BIND_DISP_HEIGHT	165
#else
#define ZFB_BIND_DISP_X  31
#define ZFB_BIND_DISP_Y	  31
#define ZFB_BIND_DISP_WIDTH	66
#define ZFB_BIND_DISP_HEIGHT    66
#endif
uint16 g_cur_zfb_bind_display_status= 0;
uint16 g_cur_zfb_bind_display_over_status= 0;

LOCAL BOOLEAN  ZFB_Bind_Show_2VM_QR(void)
{
    int x,y;
    int zoom_val;
    char ewm_str[ZFB_ALIPAY_BIND_CODE_MAX_LEN+1] = {0};
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    if(g_zfb_alipay_bind_code_len > 0)
    {
        SCI_MEMCPY(ewm_str,g_zfb_alipay_bind_code,g_zfb_alipay_bind_code_len);
    }
    side = qr_encode(QR_LEVEL_L, 0, ewm_str, g_zfb_alipay_bind_code_len, bitdata);
    if(side > ZFB_BIND_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
            y = (ZFB_BIND_DISP_HEIGHT - (zoom_val*side))/2;
        }
        else
        {
            //显示异常
            x = ZFB_BIND_DISP_X;
            y = ZFB_BIND_DISP_Y;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = ZFB_BIND_DISP_WIDTH/side;
        x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
        y = (ZFB_BIND_DISP_HEIGHT - (zoom_val*side))/2+ZFB_BIND_DISP_Y;
    }
    alipay_showQR(bitdata, side, x, y, zoom_val);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_BG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_2VM(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    ZFB_Bind_Show_2VM_QR();
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_BIND,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_30;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=ZFB_BIND_DISP_Y-4;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_Start(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_BINDING,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_DARK_BLUE_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_32;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 80;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=160;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_Over(uint16 status)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    if(status == 0)
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_BINDING_OK,&sn_string);
    }
    else
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_BINDING_FAIL,&sn_string);
    }
    
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_24;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 80;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=160;	
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    return TRUE;
}
LOCAL BOOLEAN  ZFB_Bind_Show_Status0(void)
{
    ZFB_Bind_Show_BG();
    ZFB_Bind_Show_2VM();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_Status1(void)
{
    ZFB_Bind_Show_BG();
    ZFB_Bind_Show_Start();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show_Status2(uint16 status)
{
    ZFB_Bind_Show_BG();
    ZFB_Bind_Show_Over(status);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Bind_Show(void)
{
    if(g_cur_zfb_bind_display_status == 0)
    {
        ZFB_Bind_Show_Status0();
    }
    else if(g_cur_zfb_bind_display_status == 1)
    {
        ZFB_Bind_Show_Status1();
    }
    else if(g_cur_zfb_bind_display_status == 2)
    {
        ZFB_Bind_Show_Status2(g_cur_zfb_bind_display_over_status);
    }
    else
    {
        ZFB_Bind_Show_Status0();
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZFB_BindWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_WIN_ID_T    alert_win_id = MMIZFB_ALERT_WIN_ID;

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
                MMIZFB_BackLight(TRUE);
                g_cur_zfb_bind_display_status = 0;
                g_cur_zfb_bind_display_over_status = 0;
                ZFBPay_QueryBindResult();
            break;
			
	case MSG_FULL_PAINT:
                 ZFB_Bind_Show();
		break;
        
         case MSG_ZDB_STATUS_PAY:
                MMIZFB_OpenPayWin();
                MMIZFB_OpenAlertWin(TXT_ZFB_BINDING_OK,TXT_NULL);
                MMK_CloseWin(win_id);
            break;
            
        case MSG_ZDB_STATUS_BIND:
                g_cur_zfb_bind_display_status = 0;
                ZFB_Bind_Show_Status0();
            break;
            
         case MSG_ZDB_BINDING_START:
                g_cur_zfb_bind_display_status = 1;
                ZFB_Bind_Show_Status1();
            break;
            
         case MSG_ZDB_BINDING_OK:
                MMIZFB_OpenMenuWin();
                MMIZFB_OpenAlertWin(TXT_ZFB_BINDING_OK,TXT_NULL);
                MMK_CloseWin(win_id);
            break;
            
         case MSG_ZDB_BINDING_ERROR:
                MMIZFB_OpenAlertWin(TXT_ZFB_BINDING_FAIL,TXT_NULL);
                MMK_CloseWin(win_id);
            break;

    case MSG_ZDB_STATUS_ERR_INIT:
            MMIZFB_OpenAlertWin(TXT_ZFB_INIT_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR_BIND:
            MMIZFB_OpenAlertWin(TXT_ZFB_BIND_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR:
            MMIZFB_OpenAlertWin(TXT_ZFB_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            ZFBPay_GetBindStatus(FALSE);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
                    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(g_cur_zfb_bind_display_status != 1)
            {
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CLOSE_WINDOW:
                ZFBPay_QueryBindStop();
                ZFB_BIND_Display_TimeStop();
                MMIZFB_BackLight(FALSE);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_BIND_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZFB_BindWinMsg ),
    WIN_ID(MMIZFB_BIND_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenBindWin(void)
{
    MMK_CreateWin((uint32 *)MMIZFB_BIND_WIN_TAB,PNULL);
}

#endif

#if 1
//21 的倍数
#if MMI_MAINSCREEN_WIDTH == 240
#define ZFB_SETTING_DISP_X      33
#define ZFB_SETTING_DISP_Y	    33
#define ZFB_SETTING_DISP_WIDTH  	174
#define ZFB_SETTING_DISP_HEIGHT	174
#else
#define ZFB_SETTING_DISP_X      22
#define ZFB_SETTING_DISP_Y	    22
#define ZFB_SETTING_DISP_WIDTH	      84
#define ZFB_SETTING_DISP_HEIGHT	84
#endif

uint16 g_cur_zfb_setting_display_status= 0;
uint16 g_cur_zfb_setting_display_over_status= 0;

static BOOLEAN ZFB_Setting_2VM_BmpShow(uint8 * qData,uint32 DataLen)
{
    BOOLEAN                 result = FALSE;
    uint8 * p_img_buf = NULL;
    uint32 img_len = 0;
    
    ZFB_2VM_ConvertTo_Bmp(qData,DataLen,4,&p_img_buf,&img_len);

    if(p_img_buf != NULL && img_len > 0)
    {
        ZFB_DisplayBmpBuf(ZFB_SETTING_DISP_X,ZFB_SETTING_DISP_Y,1,p_img_buf,img_len);
    }

    if(p_img_buf != NULL)
    {
        SCI_FREE(p_img_buf);
        p_img_buf = NULL;
    }
}

LOCAL BOOLEAN  ZFB_Setting_Show_2VM_QR(void)
{
    int x,y;
    int zoom_val;
    const char * ewm_str = "alipays://platformapi/startapp?appId=2017092008831174&page=%2Fpages%2Findex%2Findex";
    uint8_t bitdata[QR_MAX_BITDATA] = {0};
    int side = 0;
    uint16 ewm_len = strlen(ewm_str);
    
    side = qr_encode(QR_LEVEL_Q, 0, ewm_str, ewm_len, bitdata);
    #if 0
        ZFB_Setting_2VM_BmpShow(bitdata,side);
    #else
    if(side > ZFB_SETTING_DISP_WIDTH)
    {
        if(side < MMI_MAINSCREEN_WIDTH)
        {
            zoom_val = MMI_MAINSCREEN_WIDTH/side;
            x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
            y = (ZFB_SETTING_DISP_WIDTH - (zoom_val*side))/2;
        }
        else
        {
            //显示异常
            x = 0;
            y = ZFB_SETTING_DISP_Y;
            zoom_val = 1;
        }
    }
    else
    {
        zoom_val = ZFB_SETTING_DISP_WIDTH/side;
        x = (MMI_MAINSCREEN_WIDTH - (zoom_val*side))/2;
        y = (ZFB_SETTING_DISP_WIDTH - (zoom_val*side))/2+ZFB_SETTING_DISP_Y;
    }
    alipay_showQR(bitdata, side, x, y, zoom_val);
    //GT_Qcode_show(ewm_str, ZFB_SETTING_DISP_X, ZFB_SETTING_DISP_Y, ZFB_SETTING_DISP_WIDTH, ZFB_SETTING_DISP_HEIGHT);
    #endif
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show_BG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    GUI_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show_2VM(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_SETTING,&sn_string);
    
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font_color = MMI_BLACK_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_26;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 0;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=ZFB_SETTING_DISP_Y-4;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );

    ZFB_Setting_Show_2VM_QR();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show_Net(void)
{
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    uint16 logon_id_wstr[ZFB_ALIPAY_LOGON_ID_MAX_LEN+1] = {0};
    uint16 nick_name_wstr[ZFB_ALIPAY_NICK_NAME_MAX_LEN+1] = {0};
    if(g_zfb_alipay_nick_name_len > 0)
    {
        GUI_UTF8ToWstr(nick_name_wstr, ZFB_ALIPAY_LOGON_ID_MAX_LEN,(uint8 *)g_zfb_alipay_nick_name, g_zfb_alipay_nick_name_len);
        sn_string.wstr_ptr = nick_name_wstr;
        sn_string.wstr_len = MMIAPICOM_Wstrlen(nick_name_wstr);
    }    
    else
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ZFB_SETTING,&sn_string);
    }
        
    text_style.align = ALIGN_HVMIDDLE; // 这里设置为Left,因为Label的位置自有自己调整区域
    text_style.font_color = MMI_DARK_RED_COLOR;
    text_style.angle = ANGLE_0;
    text_style.char_space = 0;
    text_style.effect = FONT_EFFECT_CUSTOM;
    text_style.edge_color = 0;

    text_style.font = SONG_FONT_20;
    text_style.line_space = 0;
    text_style.region_num = 0;
    text_style.region_ptr = PNULL;		
    text_display_rect.left = 0;
    text_display_rect.top = 128;
    text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
    text_display_rect.bottom=148;
	
    GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
    GUISTR_DrawTextToLCDInRect( 
                                            MMITHEME_GetDefaultLcdDev(),
                                            (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                            (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                            (const MMI_STRING_T    *)&sn_string,
                                            &text_style,
                                            str_state,
                                            GUISTR_TEXT_DIR_AUTO
                                            );
    if(g_zfb_alipay_logon_id_len > 0)
    {
        GUI_UTF8ToWstr(logon_id_wstr, ZFB_ALIPAY_LOGON_ID_MAX_LEN,(uint8 *)g_zfb_alipay_logon_id, g_zfb_alipay_logon_id_len);
        sn_string.wstr_ptr = logon_id_wstr;
        sn_string.wstr_len = MMIAPICOM_Wstrlen(logon_id_wstr);
        text_style.font_color = MMI_DARK_RED_COLOR;
        text_style.font = SONG_FONT_20;
        text_display_rect.left = 0;
        text_display_rect.top = 149;
        text_display_rect.right=MMI_MAINSCREEN_WIDTH-1;
        text_display_rect.bottom=169;
        GUISTR_GetStringInfo(&text_style, &sn_string, str_state, &text_info);
        GUISTR_DrawTextToLCDInRect( 
                                                MMITHEME_GetDefaultLcdDev(),
                                                (const GUI_RECT_T      *)&text_display_rect,       //the fixed display area
                                                (const GUI_RECT_T      *)&text_display_rect,       //用户要剪切的实际区域
                                                (const MMI_STRING_T    *)&sn_string,
                                                &text_style,
                                                str_state,
                                                GUISTR_TEXT_DIR_AUTO
                                                );
    }
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show_Status0(void)
{
    ZFB_Setting_Show_BG();
    ZFB_Setting_Show_2VM();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show_Status1(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_POINT_T                 dis_point = {0,0};
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    
    GUIRES_DisplayImg(&dis_point,
                                    &bg_rect,
                                    PNULL,
                                    MMIZFB_SETTING_WIN_ID,
                                    IMAGE_ZFB_SETTING_IMG,
                                    &lcd_dev_info);
    ZFB_Setting_Show_Net();
    return TRUE;
}

LOCAL BOOLEAN  ZFB_Setting_Show(void)
{
    if(g_cur_zfb_setting_display_status == 0)
    {
        ZFB_Setting_Show_Status0();
    }
    else if(g_cur_zfb_setting_display_status == 1)
    {
        ZFB_Setting_Show_Status1();
    }
    else
    {
        ZFB_Setting_Show_Status1();
    }
    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZFB_SettingWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_RECT_T  text_display_rect={0};
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T sn_string = {0};
    GUISTR_INFO_T       text_info = {0};
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUISTR_STATE_T      str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_WIN_ID_T    alert_win_id = MMIZFB_ALERT_WIN_ID;
    static GUI_POINT_T g_setting_tp_disp_old_pt = {0};

    switch(msg_id) {
        case MSG_OPEN_WINDOW:
                MMIZFB_BackLight(TRUE);
                if(ZFB_Net_IsAllow())
                {
                    ZFBPay_GetLogonID();
                    ZFBPay_GetNickName();
                }
                g_cur_zfb_setting_display_status = 1;
                g_setting_tp_disp_old_pt.x = 0;
                g_setting_tp_disp_old_pt.y = 0;
            break;
			
	case MSG_FULL_PAINT:
                 ZFB_Setting_Show();
		break;
        
        case MSG_ZDB_NICK_NAME_OK:
                ZFB_Setting_Show();
            break;
            
        case MSG_ZDB_LOGON_ID_OK:
                ZFB_Setting_Show();
            break;
                        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            g_setting_tp_disp_old_pt = point;

        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            int offset_y =  0;
            int offset_x =  0;
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            offset_y =  point.y - g_setting_tp_disp_old_pt.y;
            offset_x =  point.x - g_setting_tp_disp_old_pt.x;
            	if(offset_x <= -(ZFB_TP_OFFSET_VALUE))	
        	{
                //Left
                if(g_cur_zfb_setting_display_status  == 0)
                {
                    g_cur_zfb_setting_display_status = 1;
                    ZFB_Setting_Show_Status1();
                }
                else if(g_cur_zfb_setting_display_status  == 1)
                {
                    g_cur_zfb_setting_display_status = 0;
                    ZFB_Setting_Show_Status0();
                }
        	}	
        	else if(offset_x >= (ZFB_TP_OFFSET_VALUE))
        	{
                //Right
                if(g_cur_zfb_setting_display_status  == 0)
                {
                    g_cur_zfb_setting_display_status = 1;
                    ZFB_Setting_Show_Status1();
                }
                else if(g_cur_zfb_setting_display_status  == 1)
                {
                    g_cur_zfb_setting_display_status = 0;
                    ZFB_Setting_Show_Status0();
                }
        	}
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
                MMIZFB_BackLight(FALSE);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_SETTING_WIN_TAB ) = {
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleZFB_SettingWinMsg ),
    WIN_ID(MMIZFB_SETTING_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenSettingWin(void)
{
    if(FALSE == ZFB_Net_IsAllow() && (g_zfb_alipay_logon_id_len == 0 || g_zfb_alipay_nick_name_len == 0))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ZFB_NET_ERR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        MMK_CreateWin((uint32 *)MMIZFB_SETTING_WIN_TAB,PNULL);
    }
}
#endif

#if 1
LOCAL BOOLEAN  ZFB_Menu_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_POINT_T                 dis_point = {0,0};
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    
    GUIRES_DisplayImg(&dis_point,
                                    &bg_rect,
                                    PNULL,
                                    win_id,
                                    IMAGE_ZFB_MENU_IMG,
                                    &lcd_dev_info);

    return TRUE;
}

LOCAL MMI_RESULT_E  HandleZFB_MenuWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
            if(g_zfb_alipay_pay_code_len == 0)
            {
                ZFBPay_GetPayCode(TRUE);
            }
            if(g_zfb_alipay_obtain_code_len == 0)
            {
                ZFBPay_GetObtainCode(TRUE);
            }
            if(ZFB_Net_IsAllow())
            {
                if(g_zfb_alipay_logon_id_len == 0)
                {
                    ZFBPay_GetLogonID();
                }
                if(g_zfb_alipay_nick_name_len == 0)
                {
                    ZFBPay_GetNickName();
                }
            }
            break;
			
	case MSG_FULL_PAINT:
                ZFB_Menu_Show(win_id);
		break;
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            if(point.x > 20 && point.x < 220 && point.y > 10 && point.y <= 80)
            {
                MMIZFB_OpenPayWin();
            }
            else if(point.x > 20 && point.x < 220 && point.y > 80 && point.y <= 160)
            {
                MMIZFB_OpenObtainWin();
            }
            else if(point.x > 20 && point.x < 220 && point.y > 160 && point.y <= 239)
            {
                MMIZFB_OpenSettingWin();
            }
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_MENU_WIN_TAB ) = {
    WIN_FUNC( (uint32)HandleZFB_MenuWinMsg ),
    WIN_ID(MMIZFB_MENU_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenMenuWin(void)
{
    SCI_TRACE_LOW("MMIZFB_OpenMainWin alipay_status=%d ",g_zfb_app_status);
    MMK_CreateWin((uint32 *)MMIZFB_MENU_WIN_TAB,PNULL);
}
#endif

#if 1
LOCAL BOOLEAN  ZFB_Main_Show(MMI_WIN_ID_T    win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_POINT_T                 dis_point = {0,0};
    GUI_RECT_T      bg_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    
    GUIRES_DisplayImg(&dis_point,
                                    &bg_rect,
                                    PNULL,
                                    win_id,
                                    IMAGE_ZFB_START_IMG,
                                    &lcd_dev_info);

    return TRUE;
}
LOCAL MMI_RESULT_E  HandleZFB_MainWinMsg(
    MMI_WIN_ID_T    win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
) 
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    GUI_RECT_T      bg_rect = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    
    switch(msg_id) {
        case MSG_OPEN_WINDOW:
                MMIZFB_BackLight(TRUE);
                if(ZFBPay_IsInit())
                {
                    if(ZFB_Net_IsAllow())
                    {
                        ZFBPay_GetBindStatus(TRUE);
                    }
                    else
                    {
                        if(ZFB_DB_INIT_OK_Get())
                        {
                            MMK_PostMsg(win_id, MSG_ZDB_STATUS_PAY, PNULL,0);
                        }
                        else
                        {
                            MMK_PostMsg(win_id, MSG_ZDB_STATUS_ERR, PNULL,0);
                        }
                    }
                }
                else
                {
                   MMIZFB_SendSigTo_APP(ZFB_APP_SIGNAL_ALIPAY_INIT_START,PNULL,0);
                }
            break;
			
	case MSG_FULL_PAINT:
                ZFB_Main_Show(win_id);
		break;
        
    case MSG_ZDB_STATUS_INIT:
            if(ZFB_Net_IsAllow())
            {
                ZFBPay_GetBindStatus(TRUE);
            }
            else
            {
                if(ZFB_DB_INIT_OK_Get())
                {
                    MMK_PostMsg(win_id, MSG_ZDB_STATUS_PAY, PNULL,0);
                }
                else
                {
                    MMK_PostMsg(win_id, MSG_ZDB_STATUS_ERR, PNULL,0);
                }
            }
        break;
        
    case MSG_ZDB_STATUS_BIND:
            MMIZFB_OpenBindWin();
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_PAY:
            MMIZFB_OpenMenuWin();
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR_INIT:
            MMIZFB_OpenAlertWin(TXT_ZFB_INIT_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR_BIND:
            MMIZFB_OpenAlertWin(TXT_ZFB_BIND_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR_PAY:
            MMIZFB_OpenAlertWin(TXT_ZFB_PAY_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
    case MSG_ZDB_STATUS_ERR_OBTAIN:
            MMIZFB_OpenAlertWin(TXT_ZFB_OBTAIN_FAIL,TXT_NULL);
            MMK_CloseWin(win_id);
        break;

    case MSG_ZDB_STATUS_ERR:
            MMIZFB_OpenAlertWin(TXT_ZFB_NET_ERR,TXT_NULL);
            MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
                MMIZFB_BackLight(FALSE);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;

}

WINDOW_TABLE( MMIZFB_MAIN_WIN_TAB ) = {
    WIN_FUNC( (uint32)HandleZFB_MainWinMsg ),
    WIN_ID(MMIZFB_MAIN_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIZFB_OpenMainWin(void)
{
    SCI_TRACE_LOW("MMIZFB_OpenMainWin alipay_status=%d ",g_zfb_app_status);
    if(FALSE == ZFB_DB_INIT_OK_Get())
    {
        if(ZFB_SIM_Exsit() == FALSE)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ZFB_NET_ERR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            return;
        }
        if(FALSE == ZFB_Net_IsAllow())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ZFB_NET_ERR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            return;
        }
    }
    MMK_CreateWin((uint32 *)MMIZFB_MAIN_WIN_TAB,PNULL);
}
#endif

PUBLIC BOOLEAN MMIZFB_IsOpenWin(void)
{
    if(MMK_IsOpenWin(MMIZFB_MAIN_WIN_ID))
    {
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_BIND_WIN_ID))
    {
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_PAY_WIN_ID))
    {
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_OBTAIN_WIN_ID))
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN MMIZFB_SendCallBackMsg(uint16 msg_id)
{
    SCI_TRACE_LOW("MMIZFB_SendCallBackMsg msg_id=0x%x ",msg_id);
    if(MMK_IsOpenWin(MMIZFB_MAIN_WIN_ID))
    {
        MMK_PostMsg(MMIZFB_MAIN_WIN_ID, msg_id, PNULL,0);
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_BIND_WIN_ID))
    {
        MMK_PostMsg(MMIZFB_BIND_WIN_ID, msg_id, PNULL,0);
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_PAY_WIN_ID))
    {
        MMK_PostMsg(MMIZFB_PAY_WIN_ID, msg_id, PNULL,0);
        return TRUE;
    }
    if(MMK_IsOpenWin(MMIZFB_OBTAIN_WIN_ID))
    {
        MMK_PostMsg(MMIZFB_OBTAIN_WIN_ID, msg_id, PNULL,0);
        return TRUE;
    }
    return FALSE;
}


#endif


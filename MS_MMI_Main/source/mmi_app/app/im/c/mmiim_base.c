/*******************************************************************************
** File Name: mmiim_public.c
** Author: Lianxiang.Zhou
** Date: 2009/08/08
** Copyright: 2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description: 应用常用的公共函数
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2009/08/08   Lianxiang.Zhou  Create.
*******************************************************************************/

//#include "std_header.h"
#include "mmi_app_im_trc.h"
#include "mmk_timer.h"

#include "mmiim_base.h"
#include "mmitheme_pos.h"

#if defined IM_ENGINE_SOGOU
#include "mmiim_image.h"
#include "mmiim_sogou.h"
#include "guires.h"

#define MMIIM_SPECIALCHAR_SPACE 0x2423  // unicode of space char
#define MMIIM_SPECIALCHAR_ENTER 0x21B5  // unicode of enter char

/******************************************************************************/
// Description: 是否是不可显示字符
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL BOOLEAN SGIsSpecialChar(wchar ch);

/******************************************************************************/
// Description: 获取不可显示字符图片宽度
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL uint16 SGGetSpecialCharWidth(wchar ch);

/******************************************************************************/
// Description: 获取字符串宽度
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL uint16 SGGetStringWidth(GUI_FONT_T font_type, const wchar* str_ptr, uint16 str_len);

/******************************************************************************/
// Description: 绘制不可显示字符
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL void SGDrawSpecialChar(wchar ch, GUI_RECT_T *display_rect_ptr, MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *dev_info_ptr);
#endif


/////////////common function////////////////////////////////////////////////////

/******************************************************************************/
// Description: 启动默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StartTimer(BOOLEAN *is_active, CAF_HANDLE_T ctrl_handle)
{
    MMIIM_StartTimerByTick(is_active, ctrl_handle, MMIIM_EXPL_TIMER_OUT_DEFAULT);

    return;
}


/******************************************************************************/
// Description: 启动默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StartTimerByTick(BOOLEAN *is_active, CAF_HANDLE_T ctrl_handle, uint32 ticks)
{
    if(*is_active)
    {
        MMK_StopTimer(MMIIM_EXPL_TIMER_ID);
    }
    
    MMK_StartWinTimer(ctrl_handle, MMIIM_EXPL_TIMER_ID, ticks, FALSE);
    *is_active = TRUE;

    return;
}



/******************************************************************************/
// Description: 关闭默认定时器
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_StopTimer(BOOLEAN *is_active)
{
    if(*is_active)
    {
        MMK_StopTimer(MMIIM_EXPL_TIMER_ID);
        *is_active = FALSE;
    }
    
    return;
}

/*****************************************************************************/
//  Description : 根据窗口的大小来设置popmenu的大小
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意物理键盘是从最上角往下弹出
/*****************************************************************************/
PUBLIC void MMIIM_SetPopMenuRect(MMI_HANDLE_T win_handle, GUI_BOTH_RECT_T *rect_ptr)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    //SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/
    if(PNULL == rect_ptr)
    {
        return;
    }
    client_rect = MMITHEME_GetWinClientBothRect(win_handle);

    logic_lcd_width = client_rect.h_rect.right - client_rect.h_rect.left;
    logic_lcd_height = client_rect.h_rect.bottom - client_rect.h_rect.top;
    rect_ptr->h_rect.left = client_rect.h_rect.left;
    rect_ptr->h_rect.top = client_rect.h_rect.top;
    rect_ptr->h_rect.right = rect_ptr->h_rect.left + logic_lcd_width * 1 / 2;
    rect_ptr->h_rect.bottom = rect_ptr->h_rect.top + logic_lcd_height * 4 / 5;

    logic_lcd_width = client_rect.v_rect.right - client_rect.v_rect.left;
    logic_lcd_height = client_rect.v_rect.bottom - client_rect.v_rect.top;
    rect_ptr->v_rect.left = client_rect.v_rect.left;
    rect_ptr->v_rect.top = client_rect.v_rect.top;
    rect_ptr->v_rect.right = rect_ptr->v_rect.left + logic_lcd_width * 1 / 2;
    rect_ptr->v_rect.bottom = rect_ptr->v_rect.top + logic_lcd_height * 4 / 5;
}

/*****************************************************************************/
//  Description : 根据窗口的大小来设置popmenu的大小
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 注意虚拟键盘是从左下角往上弹出，目前暂定方案，以后应该弄成传入参数
/*****************************************************************************/
PUBLIC void MMIIM_SetTpPopMenuRect(MMI_HANDLE_T win_handle, GUI_BOTH_RECT_T *rect_ptr)
{
    uint16 logic_lcd_width = 0;
    uint16 logic_lcd_height = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    //SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/
    if(PNULL == rect_ptr)
    {
        return;
    }
    
    client_rect = MMITHEME_GetWinClientBothRect(win_handle);

    logic_lcd_width = client_rect.h_rect.right - client_rect.h_rect.left;
    logic_lcd_height = client_rect.h_rect.bottom - client_rect.h_rect.top;
    rect_ptr->h_rect.left = client_rect.h_rect.left;
    rect_ptr->h_rect.bottom = client_rect.h_rect.bottom;
    rect_ptr->h_rect.right = rect_ptr->h_rect.left + logic_lcd_width * 1 / 2;
    rect_ptr->h_rect.top = rect_ptr->h_rect.bottom - logic_lcd_height * 4 / 5;

    logic_lcd_width = client_rect.v_rect.right - client_rect.v_rect.left;
    logic_lcd_height = client_rect.v_rect.bottom - client_rect.v_rect.top;
    rect_ptr->v_rect.left = client_rect.v_rect.left;
    rect_ptr->v_rect.bottom = client_rect.v_rect.bottom;
    rect_ptr->v_rect.right = rect_ptr->v_rect.left + logic_lcd_width * 1 / 2;
    rect_ptr->v_rect.top = rect_ptr->v_rect.bottom - logic_lcd_height * 4 / 5;
}

/*****************************************************************************/
//  Description : MMIIM_GetDisplayDirection
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_DIRECTION_SET_E MMIIM_GetDisplayDirection(GUIIM_METHOD_T method)
{
    MMI_DIRECTION_SET_E dis_direct = MMI_DIRECTION_LEFT;
    switch(method)
    {
        case GUIIM_M_URDU:
        case GUIIM_M_ARABIC:
        case GUIIM_M_PERSIAN:
        case GUIIM_M_HEBREW:
        case GUIIM_M_URDU_ABC:
        case GUIIM_M_ARABIC_ABC:
        case GUIIM_M_PERSIAN_ABC:
        case GUIIM_M_HEBREW_ABC:
        case GUIIM_M_Q_ARABIC:
        case GUIIM_M_Q_PERSIAN:
        case GUIIM_M_Q_URDU:
        case GUIIM_M_Q_HEBREW:
        case GUIIM_M_Q_ARABIC_ABC:
        case GUIIM_M_Q_PERSIAN_ABC:
        case GUIIM_M_Q_URDU_ABC:
        case GUIIM_M_Q_HEBREW_ABC:
            
        case GUIIM_M_TP_URDU:
        case GUIIM_M_TP_ARABIC:
        case GUIIM_M_TP_PERSIAN:
        case GUIIM_M_TP_HEBREW:
        case GUIIM_M_TP_URDU_ABC:
        case GUIIM_M_TP_ARABIC_ABC:
        case GUIIM_M_TP_PERSIAN_ABC:
        case GUIIM_M_TP_HEBREW_ABC:
        case GUIIM_M_TPQ_ARABIC:
        case GUIIM_M_TPQ_PERSIAN:
        case GUIIM_M_TPQ_URDU:
        case GUIIM_M_TPQ_HEBREW:
        case GUIIM_M_TPQ_ARABIC_ABC:
        case GUIIM_M_TPQ_PERSIAN_ABC:
        case GUIIM_M_TPQ_URDU_ABC:
        case GUIIM_M_TPQ_HEBREW_ABC:
            dis_direct = MMI_DIRECTION_RIGHT;
            break;

        default:
            dis_direct = MMI_DIRECTION_LEFT;
            break;
    }

    return dis_direct;
}



//#if defined(MMIIM_CORE_USE_CSTAR)
#if defined(MMIIM_CORE_USE_CSTAR) || defined(MMIIM_CORE_USE_SOGOU)

#include "mmidisplay_data.h"

#if defined(MMIIM_USE_ASSO_CSTAR)
#include "csk2api.h"
#include "mmiim_internal.h"
#endif

#include "guistring.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/
/******************************************************************************/
// Description: 左箭头
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawLeftIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c);

/******************************************************************************/
// Description: 右箭头
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawRightIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c);

/******************************************************************************/
// Description: 光标
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawCursor(GUI_LCD_DEV_INFO const *lcd_info_ptr, uint16 x, uint16 top, uint16 bottom, GUI_COLOR_T color);

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
MMIIM_UI_CONFIG_T const g_mmiim_ui_config =
{
#if (defined MMI_IM_PDA_SUPPORT)
    //font
    SONG_FONT_20, //font_default
    SONG_FONT_20, //font_pre_edit
    SONG_FONT_20, //font_candidate
    SONG_FONT_20, //font_pre_choice
    SONG_FONT_16, //font_lable
    //font color
    0x0000, //color_font_default
    0x0000, //color_font_pre_edit
    0x0000, //color_font_candidate
    0x0000, //color_font_pre_choice
    0x7bef, //color_font_lable
    //bg
    0xe71c, //color_bg_default
    0xe71c, //color_bg_pre_edit
    0xe71c, //color_bg_candidate
    0xe71c, //color_bg_pre_choice
    0xe71c, //color_bg_lable
    //hot color
    0x85a6, //color_hot_default
    0xef9d, //color_hot_pre_edit
    0x85a6, //color_hot_candidate
    0x34df, //color_hot_pre_choice
    0x34df, //color_hot_lable
    //用于装饰的边框
    0x8410, //color_border
#else
#ifdef  MAINLCD_SIZE_128X64      
    //font
    SONG_FONT_14, //font_default
    SONG_FONT_14, //font_pre_edit
    SONG_FONT_14, //font_candidate
    SONG_FONT_14, //font_pre_choice
    SONG_FONT_10, //font_lable
    //font color
    0x0000, //color_font_default
    0x0000, //color_font_pre_edit
    0x0000, //color_font_candidate
    0x0000, //color_font_pre_choice
    0x0000, //color_font_lable
    //bg
    0xFFFF, //color_bg_default
    0xFFFF, //color_bg_pre_edit
    0xFFFF, //color_bg_candidate
    0xFFFF, //color_bg_pre_choice
    0xFFFF, //color_bg_lable
    //hot color
    0x0000, //color_hot_default
    0x0000, //color_hot_pre_edit
    0x0000, //color_hot_candidate
    0x0000, //color_hot_pre_choice
    0xFFFF, //color_hot_lable
    //用于装饰的边框
    0xFFFF, //color_border
#else
    //font
    SONG_FONT_20, //font_default
    SONG_FONT_20, //font_pre_edit
    SONG_FONT_20, //font_candidate
    SONG_FONT_20, //font_pre_choice
    SONG_FONT_16, //font_lable
    //font color
    0x0000, //color_font_default
    0x0000, //color_font_pre_edit
    0x0000, //color_font_candidate
    0x0000, //color_font_pre_choice
    0x7bef, //color_font_lable
    //bg
    0xe71c, //color_bg_default
    0xe71c, //color_bg_pre_edit
    0xe71c, //color_bg_candidate
    0xe71c, //color_bg_pre_choice
    0xe71c, //color_bg_lable
    //hot color
    0x85a6, //color_hot_default
    0xef9d, //color_hot_pre_edit
    0x85a6, //color_hot_candidate
    0x34df, //color_hot_pre_choice
    0x34df, //color_hot_lable
    //用于装饰的边框
    0x8410, //color_border
#endif
#endif
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/******************************************************************************/
// Description: 左箭头
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawLeftIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c)
{
    uint16 x = 0, y = 0;
    int32 i;

#ifdef IM_ENGINE_SOGOU
	if (r->left == r->right && r->top == r->bottom)
	{
		return;
	}
#endif
    x = r->left + (r->right - r->left - MMIIM_IDC_WIDTH)/2;
    y = r->top + (r->bottom - r->top)/2;

    for(i=0; i<MMIIM_IDC_WIDTH; i++)
    {
        LCD_DrawVLine(lcd_info_ptr, x+i, y-i, y+i, c);
    }

    return;
}


/******************************************************************************/
// Description: 右箭头
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawRightIDC(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *r, GUI_COLOR_T c)
{
    uint16 x = 0, y = 0;
    int32 i;
    uint16 delta_y;

#ifdef IM_ENGINE_SOGOU
	if (r->left == r->right && r->top == r->bottom)
	{
		return;
	}
#endif
    x = r->left + (r->right - r->left - MMIIM_IDC_WIDTH)/2;
    y = r->top + (r->bottom - r->top)/2;

    for(i=0; i<MMIIM_IDC_WIDTH; i++)
    {
        delta_y = MMIIM_IDC_WIDTH - 1 - i;
        LCD_DrawVLine(lcd_info_ptr, x+i, y-delta_y, y+delta_y, c);
    }

    return;
}


/******************************************************************************/
// Description: 光标
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
LOCAL void DrawCursor(GUI_LCD_DEV_INFO const *lcd_info_ptr, uint16 x, uint16 top, uint16 bottom, GUI_COLOR_T color)
{
    LCD_DrawVLine(lcd_info_ptr, x, top, bottom, color);
    LCD_DrawVLine(lcd_info_ptr, x+1, top, bottom, color);
    
    return;
}


/******************************************************************************/
// Description: 空格字符
// Global resource dependence: NONE
// Author: yaoguang.chen
// Note:
/******************************************************************************/
PUBLIC int32 MMIIM_DrawSpace(GUI_LCD_DEV_INFO const *lcd_info_ptr, GUI_RECT_T const *prect_draw, GUI_COLOR_T color)
{
    GUI_RECT_T rect_line = {0};
    int line_height = 1, width = 0;
    
    if(lcd_info_ptr == PNULL || prect_draw == NULL)
    {
        //SCI_TRACE_LOW:"MMIIM_DrawSpace"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_BASE_371_112_2_18_2_24_50_18,(uint8*)"");
        return -1;
    }

    width = prect_draw->right - prect_draw->left > 0 ? prect_draw->right - prect_draw->left : prect_draw->left - prect_draw->right;
    line_height = line_height < width / 6? line_height : width / 6;  //线条宽度
    
    //水平线
    rect_line.left = prect_draw->left + width / 6;
    rect_line.right = rect_line.left + width * 2 / 3 + line_height;
    rect_line.top = (prect_draw->bottom + prect_draw->top) / 2;
    rect_line.bottom = rect_line.top + line_height;
    LCD_FillRect(lcd_info_ptr, rect_line, color);
    
    //左竖线
    rect_line.bottom = rect_line.top;
    rect_line.top -= 2 * line_height;
    rect_line.right = rect_line.left + line_height;
    LCD_FillRect(lcd_info_ptr, rect_line, color);

    //右竖线
    rect_line.left += width * 2 / 3;
    rect_line.right = rect_line.left + line_height;
    rect_line.bottom += line_height;
    LCD_FillRect(lcd_info_ptr, rect_line, color);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

/******************************************************************************/
// Description: 初始化候选字窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetCandWin(MMIIM_CAND_WIN_T *cand_win_ptr)
{
    MMIIM_CAND_WIN_T cand_win = {0};

    //SCI_ASSERT(PNULL != cand_win_ptr); /*assert verified*/
    if(PNULL == cand_win_ptr)
    {
        return;
    }
    cand_win = *cand_win_ptr;

    SCI_MEMSET(cand_win_ptr, 0, sizeof(MMIIM_CAND_WIN_T));

    cand_win_ptr->rect_cand = cand_win.rect_cand;
    cand_win_ptr->rect_idc_left = cand_win.rect_idc_left;
    cand_win_ptr->rect_idc_right = cand_win.rect_idc_right;
    cand_win_ptr->lable_width = cand_win.lable_width;
    cand_win_ptr->state = cand_win.state;
    cand_win_ptr->is_not_asso = cand_win.is_not_asso;
    cand_win_ptr->draw_direction = cand_win.draw_direction;

    return;
}


/******************************************************************************/
// Description: 初始化预编译窗口中的变量
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr)
{
    MMIIM_PRE_EDIT_WIN_T edit_win = {0};

    //SCI_ASSERT(PNULL != edit_win_ptr); /*assert verified*/
    if(PNULL == edit_win_ptr)
    {
        return;
    }
	edit_win = *edit_win_ptr;
    
    SCI_MEMSET(edit_win_ptr, 0, sizeof(MMIIM_PRE_EDIT_WIN_T));

    edit_win_ptr->rect = edit_win.rect;
    edit_win_ptr->state = edit_win.state;
    edit_win_ptr->draw_direction = edit_win.draw_direction;

    return;
}


#ifndef IM_ENGINE_SOGOU
/******************************************************************************/
// Description: 绘制候选字窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawCandWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr
                              )
{
    GUI_COLOR_T hot_color = 0;
    int32 index = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T draw_rect = {0}; 
    GUI_RECT_T hot_rect = {0};
    wchar *word_ptr = PNULL;
    uint16 word_len = 0;
    uint16 word_width = 0;
    wchar index_str[2] = {0};
    int16 right_margin = 0;
    int16 left_margin = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
//added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT 
    GUI_COLOR_T      font_color; // color of font   
#endif

    //清空
    disp_rect = cand_win_ptr->rect_cand;
    disp_rect.left = cand_win_ptr->rect_idc_left.left;
    disp_rect.right = cand_win_ptr->rect_idc_right.right;
	//begin 20101202
    #ifdef MAINLCD_SIZE_128X64
	if (cand_win_ptr->count > 0)
	{
    #endif    
    LCD_FillRect(dev_info_ptr, disp_rect, g_mmiim_ui_config.color_bg_candidate);

    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  disp_rect.left,
                  disp_rect.top - 1,
                  disp_rect.right,
                  g_mmiim_ui_config.color_border);
    #ifdef MAINLCD_SIZE_128X64
	}
    #endif
	//end 20101202
//    if(cand_win_ptr->count == 0
//        || cand_win_ptr->state == MMIIM_WIN_STATE_NONE)
//    {
//        return;
//     }

    /*
    1 编辑状态，候选词标签不用显示
    2 选择状态，显示候选词标签
    */
    if(cand_win_ptr->has_lable && MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
    {
        hot_color = g_mmiim_ui_config.color_hot_candidate;
    }
    else
    {
        hot_color = g_mmiim_ui_config.color_hot_default;
    }

    /*
    buffer 候选词窗口的字符串，使用'\0'分割多个候选词
    count 当前候选词的个数
    hot_index 被选中的候选词id，从0开始计算，最少总有一个被选中
    */
    disp_rect = cand_win_ptr->rect_cand;
    word_ptr = cand_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
		if(*word_ptr == ' ')
		{
			word_width = 8;
		}
		else
		{
			//函数GUI_GetStringWidth计算的点阵字体宽度比实际宽度要小，导致字符显示在hot背景色外面， +1修改之
			word_width = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
		}

            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
                style.font_color = g_mmiim_ui_config.color_font_lable;
                style.align = ALIGN_RVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &disp_rect,
                    &disp_rect,
                    &string,
                    &style,
                    0x0,
                    GUISTR_TEXT_DIR_AUTO);
            }

            //无论如何留出lable位置            
            disp_rect.right -= cand_win_ptr->lable_width;

            draw_rect = disp_rect;
            
            //right已经减过了，left不可以再减了
            left_margin = disp_rect.right - word_width;
            
            if(left_margin < cand_win_ptr->rect_cand.left)
            {
                cand_win_ptr->is_self_idc = TRUE;
                cand_win_ptr->disp_per_page = index - cand_win_ptr->disp_first_index;
                cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
                
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.right += (cand_win_ptr->rect_cand.left - left_margin);
            }
            
            //画选中背景标识
            if(cand_win_ptr->has_hot && cand_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(left_margin < cand_win_ptr->rect_cand.left)
                {
                    hot_rect.left = cand_win_ptr->rect_cand.left + 2;
                }
                else
                {
                    hot_rect.left = hot_rect.right - word_width;
                }
                
                //hot_rect.right = hot_rect.left + word_width - 1;
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT  
                style.font_color = 0xFFFF;  //与hot_color 反色              
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }
            
            //写字
            if(*word_ptr == ' ')    //空格字符
            {
                GUI_RECT_T rect_space = disp_rect;

                rect_space = disp_rect;
                if(left_margin < cand_win_ptr->rect_cand.left)
                {
                    rect_space.left = cand_win_ptr->rect_cand.left + 2;
                }
                else
                {
                    rect_space.left = rect_space.right - word_width;
                }

#ifndef MONOCHROME_LCD_SUPPORT
                style.font_color = g_mmiim_ui_config.color_font_candidate;
#endif
                
                MMIIM_DrawSpace(dev_info_ptr, &rect_space, style.font_color);
            }
            else
            {
                string.wstr_ptr = word_ptr;
                string.wstr_len = word_len;
                style.font = g_mmiim_ui_config.font_candidate;
                style.align = ALIGN_RVMIDDLE;
                
                //added by lxf for dphone
#ifndef MONOCHROME_LCD_SUPPORT
                style.font_color = g_mmiim_ui_config.color_font_candidate;
#endif
                
#ifdef SCRIPTEASY_SUPPORT
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
#else
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    0x0,
                    GUISTR_TEXT_DIR_AUTO);
#endif
            }
            
            disp_rect.right -= word_width;
            disp_rect.right -= MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        if(cand_win_ptr->disp_first_index > 0)
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
        }
    }
    else
    {
        //从左往右画
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }

            if(*word_ptr == ' ')
            {
                word_width = 8;
            }
            else
            {
                word_width = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
            }
            
            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
                style.font_color = g_mmiim_ui_config.color_font_lable;
                style.align = ALIGN_LVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &disp_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }

            //无论如何留出lable位置
            disp_rect.left += cand_win_ptr->lable_width;

            draw_rect = disp_rect;

            //left已经加过了，不可以再加lable
            right_margin = disp_rect.left + word_width;
            
            if(right_margin > cand_win_ptr->rect_cand.right)
            {
                cand_win_ptr->is_self_idc = TRUE;
                cand_win_ptr->disp_per_page = index - cand_win_ptr->disp_first_index;
                cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
                
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.left -= (right_margin - cand_win_ptr->rect_cand.right);
            }
                        
            
            //画选中背景标识
            if(cand_win_ptr->has_hot && cand_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(right_margin > cand_win_ptr->rect_cand.right)
                {
                    hot_rect.right = cand_win_ptr->rect_cand.right - 1;
                }
                else
                {
                    hot_rect.right = hot_rect.left + word_width - 1;
                }
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT 
                style.font_color = 0xFFFF;  //与hot_color 反色
                
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }

            //写字
            if(*word_ptr == ' ')    //空格字符
            {
                GUI_RECT_T rect_space = disp_rect;

                rect_space = disp_rect;
                if(right_margin > cand_win_ptr->rect_cand.right)
                {
                    rect_space.right = cand_win_ptr->rect_cand.right - 1;
                }
                else
                {
                    rect_space.right = rect_space.left + word_width - 1;
                }

				//added by lxf for dphone
#ifndef MONOCHROME_LCD_SUPPORT 
                style.font_color = g_mmiim_ui_config.color_font_candidate;
#endif
      
                MMIIM_DrawSpace(dev_info_ptr, &rect_space, style.font_color);
            }
            else
            {
                //写字
                string.wstr_ptr = word_ptr;
                string.wstr_len = word_len;
                style.font = g_mmiim_ui_config.font_candidate;
                style.align = ALIGN_VMIDDLE;
                
                //added by lxf for dphone
#ifndef MONOCHROME_LCD_SUPPORT 
                style.font_color = g_mmiim_ui_config.color_font_candidate;
#endif
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }

            disp_rect.left += word_width;
            disp_rect.left += MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        if(cand_win_ptr->disp_first_index > 0)
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
        }
    }
    
    /*
    flagPageCand 候选词窗口是否需要显示上下翻页图标：
        0不显示，1显示上翻页，2显示下翻页，3显示上下翻页
    */
    if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_LEFT)
    {
        DrawLeftIDC(dev_info_ptr, &(cand_win_ptr->rect_idc_left), hot_color);
    }

    if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_RIGHT)
    {
        DrawRightIDC(dev_info_ptr, &(cand_win_ptr->rect_idc_right), hot_color);
    }

    return;
}


/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawPreEditWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_PRE_EDIT_WIN_T *edit_win_ptr
                                 )
{
    GUI_COLOR_T color = 0;
    wchar buffer[MMIIM_PRE_BUFFER_LEN] = {0};
    wchar space[] = L" "; //空格
    wchar const *item_ptr = PNULL;
    uint16 item_len = 0;
    int32 index = 0;
    uint16 cursor_x = 0;
    GUI_RECT_T draw_rect = {0};
    int16 delta_x = 0;
    size_t cursor_len = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    wchar tip_string[MMIIM_WIN_BUFFER_LEN] = {0};
    int16 tip_len = 0;
    
    //清空
    LCD_FillRect(dev_info_ptr, edit_win_ptr->rect, g_mmiim_ui_config.color_bg_pre_edit);
    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  edit_win_ptr->rect.left,
                  edit_win_ptr->rect.top - 1,
                  edit_win_ptr->rect.right,
                  g_mmiim_ui_config.color_border);

    if(edit_win_ptr->state == MMIIM_WIN_STATE_NONE)
    {
        return;
    }

    if(MMIIM_WIN_STATE_USER == edit_win_ptr->state)
    {        
        MMIAPICOM_Wstrcat(tip_string, edit_win_ptr->buffer);
        tip_len = (int16)MMIAPICOM_Wstrlen(tip_string);
        if (0 == tip_len)
        {
            wchar new_word_str[MMIIM_WIN_BUFFER_LEN] = L"Add new word:";
            MMIAPICOM_Wstrcat(tip_string, new_word_str);
            tip_len = MMIAPICOM_Wstrlen(tip_string);
        }
        string.wstr_ptr = tip_string;
        string.wstr_len = tip_len;
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = g_mmiim_ui_config.color_font_default;
        if (MMIIM_DRAW_RIGHT2LEFT == edit_win_ptr->draw_direction)
        {
            style.align = ALIGN_RVMIDDLE;

            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                                       &edit_win_ptr->rect,
                                       &edit_win_ptr->rect,
                                       &string,
                                       &style,
                                       0x0,
                                       GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
            style.align = ALIGN_VMIDDLE;

            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                                       &edit_win_ptr->rect,
                                       &edit_win_ptr->rect,
                                       &string,
                                       &style,
                                       GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                                       GUISTR_TEXT_DIR_AUTO);
        }

        return;
    }
    
    if(edit_win_ptr->count == 0)
    {
        return;
    }

    if(MMIIM_WIN_STATE_CAND == edit_win_ptr->state)
    {
        color = g_mmiim_ui_config.color_font_default;
    }
    else
    {
        color = g_mmiim_ui_config.color_font_pre_edit;
    }

    /*
    buffer 需要在Pre-Edit窗口显示的字符串，使用'\0'分割多个项
    count Pre-Edit共有多少项
    cursor_index 需要显示光标的项id，从0开始计算
    */
    item_ptr = edit_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == edit_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同        
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(edit_win_ptr->has_cursor && edit_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = edit_win_ptr->rect;
        
        //往前面移动一段距离
        cursor_x = draw_rect.right - cursor_x - 2;
        if (cursor_x < draw_rect.left)
        {
            delta_x = draw_rect.left - cursor_x;

            draw_rect.right += delta_x;
        }

        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_RVMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &edit_win_ptr->rect,
            &string,
            &style,
            0x0,
            GUISTR_TEXT_DIR_AUTO);
        
        if(edit_win_ptr->has_cursor && edit_win_ptr->count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x,
                edit_win_ptr->rect.top,
                edit_win_ptr->rect.bottom,
                color);
        }
    }
    else
    {
        //从左到右      
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(edit_win_ptr->has_cursor && edit_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = edit_win_ptr->rect;
        
        //往前面移动一段距离
        while(cursor_x > draw_rect.right - 2) // -2 给光标留个地方
        {
            delta_x = cursor_x - (draw_rect.right - 2);
            
            cursor_x = draw_rect.right - 2;
            draw_rect.left -= delta_x;
        }
        
        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_VMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &edit_win_ptr->rect,
            &string,
            &style,
            GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
            GUISTR_TEXT_DIR_AUTO);
        
        if(edit_win_ptr->has_cursor && edit_win_ptr->count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x + edit_win_ptr->rect.left,
                edit_win_ptr->rect.top,
                edit_win_ptr->rect.bottom,
                color);
        }
    }

    return;
}
#endif

/******************************************************************************/
// Description: 绘制用户自造词窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawUserWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *cand_win_ptr
                              )
{
    GUI_RECT_T draw_rect = {0};
    uint16 cursor_x = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    int16 delta_x = 0;

    //清空
    draw_rect = cand_win_ptr->rect_cand;
    draw_rect.left = cand_win_ptr->rect_idc_left.left;
    draw_rect.right = cand_win_ptr->rect_idc_right.right;
    LCD_FillRect(dev_info_ptr, draw_rect, g_mmiim_ui_config.color_bg_candidate);

    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  draw_rect.left,
                  draw_rect.top - 1,
                  draw_rect.right,
                  g_mmiim_ui_config.color_border);

    //SCI_ASSERT(cand_win_ptr->state == MMIIM_WIN_STATE_USER);
    if (cand_win_ptr->state != MMIIM_WIN_STATE_USER)
    {
        return;
    }

    draw_rect = cand_win_ptr->rect_cand;
    draw_rect.left += cand_win_ptr->lable_width;
    
    cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate,
                                  cand_win_ptr->buffer,
                                  cand_win_ptr->hot_index);

    if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
    {
        //从右往左显示
        cursor_x = draw_rect.right - cursor_x - 2;
        if (cursor_x < draw_rect.left)
        {
            delta_x = draw_rect.left - cursor_x;

            draw_rect.right += delta_x;
        }
        
        //写字
        string.wstr_ptr = cand_win_ptr->buffer;
        string.wstr_len = cand_win_ptr->count;
        style.font = g_mmiim_ui_config.font_candidate;
#if defined(IM_ENGINE_SOGOU)
		if (MMIAPIIM_SGIsDrawSkin())
		{
			style.font_color = 0x0000;
		}
		else
#endif
        style.font_color = g_mmiim_ui_config.color_font_candidate;
        style.align = ALIGN_RVMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &(cand_win_ptr->rect_cand),
            &string,
            &style,
            0x0,
            GUISTR_TEXT_DIR_AUTO);
        
        
        DrawCursor(dev_info_ptr,
            cursor_x,
            cand_win_ptr->rect_cand.top,
            cand_win_ptr->rect_cand.bottom,
            g_mmiim_ui_config.color_font_candidate);
    }
    else
    {
        
        cursor_x += draw_rect.left;
        
        if (cursor_x > draw_rect.right)
        {
            delta_x = cursor_x - draw_rect.right;
            
            cursor_x = draw_rect.right;
            draw_rect.left -= delta_x;
        }
        
        //写字
        string.wstr_ptr = cand_win_ptr->buffer;
        string.wstr_len = cand_win_ptr->count;
        style.font = g_mmiim_ui_config.font_candidate;
#if defined(IM_ENGINE_SOGOU)
		if (MMIAPIIM_SGIsDrawSkin())
		{
			style.font_color = 0x0000;
		}
		else
#endif
		style.font_color = g_mmiim_ui_config.color_font_candidate;
        style.align = ALIGN_VMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &(cand_win_ptr->rect_cand),
            &string,
            &style,
            GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
            GUISTR_TEXT_DIR_AUTO);
        
        
        DrawCursor(dev_info_ptr,
            cursor_x,
            cand_win_ptr->rect_cand.top,
            cand_win_ptr->rect_cand.bottom,
            g_mmiim_ui_config.color_font_candidate);
    }

    return;
}

/******************************************************************************/
// Description: 点击选择窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCand(
    MMIIM_CAND_WIN_T *cand_win_ptr,
    GUI_POINT_T point,
    MMI_HANDLE_T im_handle
)
{
    int32 index = 0;
    wchar const *word_ptr = PNULL;
    uint16 word_len = 0;
    uint16 word_width = 0;
    GUI_RECT_T word_rect = {0};
    int32 i = 0;
    BOOLEAN is_gotten_word = FALSE;
    GUIIM_NOTIFY_PACK_T notify_pack = {0};


    if(GUI_PointIsInRect(point, cand_win_ptr->rect_idc_left))
    {
        if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_LEFT)
        {
            cand_win_ptr->disp_first_index -= cand_win_ptr->disp_per_page;
            cand_win_ptr->idc_flag &= ~MMIIM_IDC_FLAG_LEFT;
        }
        
        return TRUE;
    }
    else if(GUI_PointIsInRect(point, cand_win_ptr->rect_idc_right))
    {
        if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_RIGHT)
        {
            cand_win_ptr->disp_first_index += cand_win_ptr->disp_per_page;
            //SCI_ASSERT(cand_win_ptr->disp_first_index < cand_win_ptr->count);
            if (cand_win_ptr->disp_first_index >= cand_win_ptr->count)
            {
                return FALSE;
            }
            cand_win_ptr->idc_flag &= ~MMIIM_IDC_FLAG_RIGHT;
        }

        return TRUE;
    }
    else if(!GUI_PointIsInRect(point, cand_win_ptr->rect_cand))
    {
        return FALSE;
    }

    word_ptr = cand_win_ptr->buffer;
    word_rect = cand_win_ptr->rect_cand;
    
    for(index = 0; index < cand_win_ptr->count; index++)
    {
        word_len = MMIAPICOM_Wstrlen(word_ptr);

        word_len = MIN( word_len, GUIIM_COMMIT_BUFFER_MAX );
        
        if(index < cand_win_ptr->disp_first_index)
        {
            if(word_ptr > cand_win_ptr->buffer + MMIIM_WIN_BUFFER_LEN - word_len - 1)
            {
                break;
            }
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
            continue;
        }
        
#ifdef IM_ENGINE_SOGOU
	word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
#else
        //函数GUI_GetStringWidth计算的点阵字体宽度比实际宽度要小，导致字符显示在hot背景色外面， +1修改之
        word_width = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len) + 1;
#endif
        
        word_rect.right = word_rect.left;
        word_rect.right += cand_win_ptr->lable_width;
        word_rect.right += word_width;
        word_rect.right += MMIIM_CAND_WORDS_SPACE;
        word_rect.right -= 1;
        
        //判定是否击中
        if(GUI_PointIsInRect(point, word_rect))
        {
            for(i=0; i < word_len; i++)
            {
                notify_pack.data.commit_data.commit[i] = *(word_ptr + i);
            }

            notify_pack.data.commit_data.commit_len = i;

            notify_pack.notify = GUIIM_NOTIFY_COMMIT;
            GUIIM_SetNotify(im_handle, &notify_pack);

            is_gotten_word = TRUE;

            if(cand_win_ptr->is_not_asso)
            {
                MMIIM_ResetCandWin(cand_win_ptr);
                cand_win_ptr->state = MMIIM_WIN_STATE_NONE;
            }
            else
            {
                //获得下一个联想字
                MMIIM_AssoCand(cand_win_ptr, notify_pack.data.commit_data.commit, word_len, word_len);
            }
                        
            break;
        }

        word_rect.left = word_rect.right + 1;

        //更新显示词，1为分隔字符'\0'
        if(word_ptr > cand_win_ptr->buffer + MMIIM_WIN_BUFFER_LEN - word_len - 1)
        {
            break;
        }
        word_ptr += word_len + 1;
    }

    return is_gotten_word;
}


/******************************************************************************/
// Description: 根据上下文设置候选窗口的候选字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 如果选中，则同时处理返回值
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_AssoCand(
                              MMIIM_CAND_WIN_T *cand_win_ptr,
                              wchar const * context_ptr,
                              uint16 context_len,
                              uint16 context_pos
                              )
{
    int32 i = 0;
    
    //获得下一个联想字
    MMIIM_ResetCandWin(cand_win_ptr);
    cand_win_ptr->state = MMIIM_WIN_STATE_CAND;
    cand_win_ptr->count = MMIIM_ASSO_WORD_MAX_LEN;
    MMIIM_QueryAsso(context_ptr, context_len, context_pos, cand_win_ptr->buffer, &cand_win_ptr->count);
    //SCI_ASSERT(cand_win_ptr->count <= MMIIM_ASSO_WORD_MAX_LEN);
    if (cand_win_ptr->count > MMIIM_ASSO_WORD_MAX_LEN)
    {
        return FALSE;
    }

    //返回的序列是连续的单字
    for(i = cand_win_ptr->count - 1; i >= 0; i--)
    {
        cand_win_ptr->buffer[2*i+1] = '\0';
        cand_win_ptr->buffer[2*i] = cand_win_ptr->buffer[i];
    }

    cand_win_ptr->has_hot = TRUE;
    cand_win_ptr->hot_index = 0;
    
    return TRUE;
}


#ifndef IM_ENGINE_SOGOU
/******************************************************************************/
// Description: 点击选择窗口，当按键处理，不处理结果
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_TpDownCandAsKey(
                                     MMIIM_CAND_WIN_T *cand_win_ptr,
                                     GUI_POINT_T point,
                                     uint16 *index_ptr
                                     )
{
    int32 index = 0;
    wchar const *word_ptr = PNULL;
    uint16 word_len = 0;
    uint16 word_width = 0;
    GUI_RECT_T word_rect = {0};
    BOOLEAN is_gotten_word = FALSE;

    if(!GUI_PointIsInRect(point, cand_win_ptr->rect_cand))
    {
        return FALSE;
    }

    word_ptr = cand_win_ptr->buffer;
    word_rect = cand_win_ptr->rect_cand;
    
    if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
    {
        //字符从右往左显示
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);

            word_rect.left = word_rect.right;
            word_rect.left -= cand_win_ptr->lable_width;
            word_rect.left -= word_width;
            word_rect.left -= MMIIM_CAND_WORDS_SPACE;
            word_rect.left += 1;
            
            //判定是否击中
            if(GUI_PointIsInRect(point, word_rect))
            {
                *index_ptr = index;
                is_gotten_word = TRUE;
                
                break;
            }
            
            word_rect.right = word_rect.left - 1;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
    }
    else
    {
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = GUI_GetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
            
            word_rect.right = word_rect.left;
            word_rect.right += cand_win_ptr->lable_width;
            word_rect.right += word_width;
            word_rect.right += MMIIM_CAND_WORDS_SPACE;
            word_rect.right -= 1;
            
            //判定是否击中
            if(GUI_PointIsInRect(point, word_rect))
            {
                *index_ptr = index;
                is_gotten_word = TRUE;
                
                break;
            }
            
            word_rect.left = word_rect.right + 1;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
    }    

    return is_gotten_word;
}
#endif

#if 0
/* 获取联想汉字 */
CS_RESULT   HanziAsso_QueryAsso(CS_BOOL  isPinyin,
                                CS_WCHAR wContext,  /* 上下文内容unicode字符串，如“开始” */
                                CS_PWSTR pwzAsso,       /* 返回的联想候选序列*/
                                CS_INT  *pnAsso);       /* 输入为所要的联想最大个数，返回的是实际候选个数*/

#endif

/******************************************************************************/
// Description: 获取联想汉字
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC int32 MMIIM_QueryAsso(
                             wchar const * context_ptr,
                             uint16 context_len,
                             uint16 context_pos,
                             wchar *asso_ptr,
                             int32 *asso_len_ptr
                             )
{
#if defined(MMIIM_USE_ASSO_CSTAR)
    //SCI_ASSERT(PNULL != context_ptr);
    //SCI_ASSERT(PNULL != asso_ptr);
    //SCI_ASSERT(PNULL != asso_len_ptr);
    if (PNULL == context_ptr)
    {
        //SCI_TRACE_LOW:"MMIIM_QueryAsso: context_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_BASE_1479_112_2_18_2_24_52_19,(uint8*)"");
        return 0;
    }
    if (PNULL == asso_ptr)
    {
        //SCI_TRACE_LOW:"MMIIM_QueryAsso: asso_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_BASE_1484_112_2_18_2_24_52_20,(uint8*)"");
        return 0;
    }    
    if (PNULL == asso_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIIM_QueryAsso: asso_len_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_BASE_1489_112_2_18_2_24_52_21,(uint8*)"");
        return 0;
    }
    
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
    (void)HanziAsso_QueryAsso(MMIIM_ASSO_CHS_TYPE, *context_ptr, asso_ptr, asso_len_ptr);/*lint !e64*/
#endif

#else
    *asso_len_ptr = 0;
#endif

    return *asso_len_ptr;
}

/******************************************************************************/
// Description: 初始化候选字框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitCandWin(MMIIM_CAND_WIN_T *cand_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height)
{
    //SCI_ASSERT(PNULL != cand_win_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_rect_ptr); /*assert verified*/
    if(PNULL == cand_win_ptr || PNULL == win_rect_ptr)
    {
        return;
    }
    cand_win_ptr->rect_idc_left.bottom = win_rect_ptr->bottom;
    cand_win_ptr->rect_idc_left.top = cand_win_ptr->rect_idc_left.bottom - height + 1;
    cand_win_ptr->rect_idc_left.left = win_rect_ptr->left;
    cand_win_ptr->rect_idc_left.right = cand_win_ptr->rect_idc_left.left + MMIIM_IDC_FULL_WIDTH - 1;

    cand_win_ptr->rect_idc_right.bottom = win_rect_ptr->bottom;
    cand_win_ptr->rect_idc_right.top = cand_win_ptr->rect_idc_right.bottom - height;
    cand_win_ptr->rect_idc_right.right = win_rect_ptr->right;
    cand_win_ptr->rect_idc_right.left = cand_win_ptr->rect_idc_right.right - MMIIM_IDC_FULL_WIDTH + 1;

    cand_win_ptr->rect_cand.bottom = win_rect_ptr->bottom;
    cand_win_ptr->rect_cand.top = cand_win_ptr->rect_cand.bottom - height + 1;
    cand_win_ptr->rect_cand.left = cand_win_ptr->rect_idc_left.right;
    cand_win_ptr->rect_cand.right = cand_win_ptr->rect_idc_right.left;

    return;
}


/******************************************************************************/
// Description: 初始化预编辑框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitPreEditWin(MMIIM_PRE_EDIT_WIN_T *edit_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height)
{
    //SCI_ASSERT(PNULL != edit_win_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_rect_ptr); /*assert verified*/
    if(PNULL == edit_win_ptr || PNULL == win_rect_ptr)
    {
        return;
    }
    
    edit_win_ptr->rect.bottom = win_rect_ptr->bottom;
    edit_win_ptr->rect.top = edit_win_ptr->rect.bottom - height + 1;
    edit_win_ptr->rect.left = win_rect_ptr->left;
    edit_win_ptr->rect.right = win_rect_ptr->right;

    return;
}
/******************************************************************************/
// Description: 初始化混淆区域
// Global resource dependence: NONE
// Author: juan.wu
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitChoiceWin(MMIIM_CHOICE_WIN_T *choice_win_ptr, GUI_RECT_T *win_rect_ptr,uint16 height)
{
    //SCI_ASSERT(PNULL != choice_win_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != win_rect_ptr); /*assert verified*/
    if(PNULL == choice_win_ptr || PNULL == win_rect_ptr)
    {
        return;
    }
    
    choice_win_ptr->rect.bottom = win_rect_ptr->bottom;
    choice_win_ptr->rect.top = choice_win_ptr->rect.bottom - height + 1;
    choice_win_ptr->rect.left = win_rect_ptr->left;
    choice_win_ptr->rect.right = win_rect_ptr->right;

    return;

}
/******************************************************************************/
// Description: 初始化choice窗口中的变量
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ResetChoiceWin(MMIIM_CHOICE_WIN_T *choice_win_ptr)
{
    MMIIM_CHOICE_WIN_T choice_win = {0};
    
    //SCI_ASSERT(PNULL != choice_win_ptr); /*assert verified*/
    if(PNULL == choice_win_ptr )
    {
        return;
    }
    choice_win = * choice_win_ptr;

    SCI_MEMSET(choice_win_ptr, 0, sizeof(MMIIM_CHOICE_WIN_T));

    choice_win_ptr->rect = choice_win.rect;
    choice_win_ptr->state = choice_win.state;
    choice_win_ptr->draw_direction = choice_win.draw_direction;

}

/******************************************************************************/
// Description: 混淆窗口(给单字库用的)
// Global resource dependence: NONE
// Author: juan.wu
// Note:
/******************************************************************************/
#ifndef IM_ENGINE_SOGOU
PUBLIC void MMIIM_DrawChoiceWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_CHOICE_WIN_T *choice_win_ptr
                                 )
{
    GUI_COLOR_T color = 0;
    wchar buffer[MMIIM_PRE_BUFFER_LEN] = {0};
    wchar space[] = L" "; //空格
    wchar const *item_ptr = PNULL;
    uint16 item_len = 0;
    int32 index = 0;
    uint16 cursor_x = 0;
    GUI_RECT_T draw_rect = {0};
    int16 delta_x = 0;
    size_t cursor_len = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    wchar tip_string[MMIIM_WIN_BUFFER_LEN] = {0};
    int16 tip_len = 0;
    
    //清空
    LCD_FillRect(dev_info_ptr, choice_win_ptr->rect, g_mmiim_ui_config.color_bg_pre_choice);

    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  choice_win_ptr->rect.left,
                  choice_win_ptr->rect.top - 1,
                  choice_win_ptr->rect.right,
                  g_mmiim_ui_config.color_border);


    if(choice_win_ptr->state == MMIIM_WIN_STATE_NONE)
    {
        return;
    }

    if(MMIIM_WIN_STATE_USER == choice_win_ptr->state)
    {        
        MMIAPICOM_Wstrcat(tip_string, choice_win_ptr->buffer);
        tip_len = (int16)MMIAPICOM_Wstrlen(tip_string);
        if (0 == tip_len)
        {
            wchar new_word_str[MMIIM_WIN_BUFFER_LEN] = L"Add new word:";
            MMIAPICOM_Wstrcat(tip_string, new_word_str);
            tip_len = MMIAPICOM_Wstrlen(tip_string);
        }
        
        string.wstr_ptr = tip_string;
        string.wstr_len = tip_len;
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = g_mmiim_ui_config.color_font_default;
        if (MMIIM_DRAW_RIGHT2LEFT == choice_win_ptr->draw_direction)
        {
            style.align = ALIGN_RVMIDDLE;

            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                                       &choice_win_ptr->rect,
                                       &choice_win_ptr->rect,
                                       &string,
                                       &style,
                                       0x0,
                                       GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
            style.align = ALIGN_VMIDDLE;

            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                                       &choice_win_ptr->rect,
                                       &choice_win_ptr->rect,
                                       &string,
                                       &style,
                                       GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                                       GUISTR_TEXT_DIR_AUTO);
        }

        return;
    }
    
    if(choice_win_ptr->choice_count == 0)
    {
        return;
    }

    if(MMIIM_WIN_STATE_CAND == choice_win_ptr->state)
    {
        color = g_mmiim_ui_config.color_font_default;
    }
    else
    {
        color = g_mmiim_ui_config.color_font_pre_choice;
    }
    
    item_ptr = choice_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == choice_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同        
        for(index = 0; index < choice_win_ptr->choice_count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(choice_win_ptr->has_cursor && choice_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = choice_win_ptr->rect;
        
        //往前面移动一段距离
        cursor_x = draw_rect.right - cursor_x - 2;
        if (cursor_x < draw_rect.left)
        {
            delta_x = draw_rect.left - cursor_x;

            draw_rect.right += delta_x;
        }

        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_RVMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &choice_win_ptr->rect,
            &string,
            &style,
            0x0,
            GUISTR_TEXT_DIR_AUTO);
        
        if(choice_win_ptr->has_cursor && choice_win_ptr->choice_count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x,
                choice_win_ptr->rect.top,
                choice_win_ptr->rect.bottom,
                color);
        }
    }
    else
    {
        //从左到右      
        for(index = 0; index < choice_win_ptr->choice_count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(choice_win_ptr->has_cursor && choice_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = choice_win_ptr->rect;
        
        //往前面移动一段距离
        while(cursor_x > draw_rect.right - 2) // -2 给光标留个地方
        {
            delta_x = cursor_x - (draw_rect.right - 2);
            
            cursor_x = draw_rect.right - 2;
            draw_rect.left -= delta_x;
        }
        
        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_VMIDDLE;
        
        GUISTR_DrawTextToLCDInRect(dev_info_ptr,
            &draw_rect,
            &choice_win_ptr->rect,
            &string,
            &style,
            GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
            GUISTR_TEXT_DIR_AUTO);
        
        if(choice_win_ptr->has_cursor && choice_win_ptr->choice_count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x + choice_win_ptr->rect.left,
                choice_win_ptr->rect.top,
                choice_win_ptr->rect.bottom,
                color);
        }
    }

    return;
}
#endif

#ifdef IM_ENGINE_SOGOU
/******************************************************************************/
// Description: 初始化预编辑框的区域
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void MMIIM_InitLetterWin(MMIIM_CAND_WIN_T *edit_win_ptr, GUI_RECT_T *win_rect_ptr, uint16 height)
{
    //SCI_ASSERT(PNULL != edit_win_ptr);  /*assert verified*/
    //SCI_ASSERT(PNULL != win_rect_ptr);  /*assert verified*/
    if(PNULL == edit_win_ptr || PNULL == win_rect_ptr)
    {
        return;
    }
    edit_win_ptr->rect_cand.bottom = win_rect_ptr->bottom;
    edit_win_ptr->rect_cand.top = edit_win_ptr->rect_cand.bottom - height + 1;
    edit_win_ptr->rect_cand.left = win_rect_ptr->left;
    edit_win_ptr->rect_cand.right = win_rect_ptr->right;

    return;
}

/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawLetterWin(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              MMIIM_CAND_WIN_T *edit_win_ptr,
                              uint8 *lastdisp,
                              void *imeResult_in,
							  MMI_WIN_ID_T   win_id
							  )

{
    GUI_COLOR_T hot_color = 0;
    int32 index = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T draw_rect = {0}; 
    GUI_RECT_T hot_rect = {0};
    wchar *word_ptr;
    uint16 word_len = 0;
    uint16 word_width = 0;
    wchar index_str[2] = {0};
    int16 right_margin = 0;
    int16 left_margin = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
//added by lxf for dphone
#ifdef  MAINLCD_SIZE_128X64      
    GUI_COLOR_T      font_color; // color of font   
#endif
    WD_imeResult * imeResult = (WD_imeResult*)(imeResult_in);

    LCD_DrawHLine(dev_info_ptr, edit_win_ptr->rect_cand.left, edit_win_ptr->rect_cand.top - 1, edit_win_ptr->rect_cand.right,
					  g_mmiim_ui_config.color_border);
    /*
    1 编辑状态，候选词标签不用显示
    2 选择状态，显示候选词标签
    */
    if(edit_win_ptr->has_lable && MMIIM_WIN_STATE_CAND == edit_win_ptr->state)
    {
        hot_color = g_mmiim_ui_config.color_hot_candidate;
    }
    else
    {
        hot_color = g_mmiim_ui_config.color_hot_default;
    }

    /*
    buffer 候选词窗口的字符串，使用'\0'分割多个候选词
    count 当前候选词的个数
    hot_index 被选中的候选词id，从0开始计算，最少总有一个被选中
    */
    disp_rect = edit_win_ptr->rect_cand;
    word_ptr = edit_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == edit_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < edit_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);

            //写索引
            if(edit_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == edit_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
				if (MMIAPIIM_SGIsDrawSkin())
				{
					style.font_color = 0x0000;
				}
				else
                {
                    style.font_color = g_mmiim_ui_config.color_font_lable;
                }
                style.align = ALIGN_RVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &disp_rect,
                    &disp_rect,
                    &string,
                    &style,
                    0x0,
                    GUISTR_TEXT_DIR_AUTO);
            }

            //无论如何留出lable位置            
            disp_rect.right -= edit_win_ptr->lable_width;

            draw_rect = disp_rect;
            
            //right已经减过了，left不可以再减了
            left_margin = disp_rect.right - word_width;
            
            if(left_margin < edit_win_ptr->rect_cand.left)
            {
                edit_win_ptr->is_self_idc = TRUE;
                edit_win_ptr->disp_per_page = index - edit_win_ptr->disp_first_index;
                edit_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
                
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.right += (edit_win_ptr->rect_cand.left - left_margin);
            }
            
            //画选中背景标识
            if(edit_win_ptr->has_hot && edit_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(left_margin < edit_win_ptr->rect_cand.left)
                {
                    hot_rect.left = edit_win_ptr->rect_cand.left + 2;
                }
                else
                {
                    hot_rect.left = hot_rect.right - word_width;
                }
                
                //hot_rect.right = hot_rect.left + word_width - 1;
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef  MAINLCD_SIZE_128X64  
                style.font_color = 0xFFFF;  //与hot_color 反色              
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }
            
            //写字
            string.wstr_ptr = word_ptr;
            string.wstr_len = word_len;
            style.font = g_mmiim_ui_config.font_candidate;
            style.align = ALIGN_RVMIDDLE;
            
            //added by lxf for dphone
#ifndef  MAINLCD_SIZE_128X64 
            if (MMIAPIIM_SGIsDrawSkin())
            {
                style.font_color = 0x0000;
            }
            else
            {
                style.font_color = g_mmiim_ui_config.color_font_candidate;
            }
#endif
            
			if (word_len == 1 && SGIsSpecialChar(*word_ptr))
			{
				SGDrawSpecialChar(*word_ptr, &draw_rect, win_id, dev_info_ptr);
			}
			else
            {
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    0x0,
                    GUISTR_TEXT_DIR_AUTO);
            }
            
            disp_rect.right -= word_width;
            
            disp_rect.right -= MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        if(edit_win_ptr->disp_first_index > 0)
        {
            edit_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
        }
    }
    else
    {
        //从左往右画
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < edit_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
            
            //写索引
            if(edit_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == edit_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
				if (MMIAPIIM_SGIsDrawSkin())
				{
					style.font_color = 0x0000;
				}
				else
                {
                    style.font_color = g_mmiim_ui_config.color_font_lable;
                }
                style.align = ALIGN_LVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &disp_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }

            //无论如何留出lable位置
            disp_rect.left += edit_win_ptr->lable_width;

            draw_rect = disp_rect;

            //left已经加过了，不可以再加lable
            right_margin = disp_rect.left + word_width;
            
            if(right_margin > edit_win_ptr->rect_cand.right)
            {
                edit_win_ptr->is_self_idc = TRUE;
                edit_win_ptr->disp_per_page = index - edit_win_ptr->disp_first_index;
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.left -= (right_margin - edit_win_ptr->rect_cand.right);
                if (index > 1)
                {
                    break;
                }
            }
                        
            //画选中背景标识
            if(edit_win_ptr->has_hot && edit_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(right_margin > edit_win_ptr->rect_cand.right)
                {
                    hot_rect.right = edit_win_ptr->rect_cand.right - 1;
                }
                else
                {
                    hot_rect.right = hot_rect.left + word_width - 1;
                }
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef  MAINLCD_SIZE_128X64  
                style.font_color = 0xFFFF;  //与hot_color 反色
                
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }
            
            //写字
            string.wstr_ptr = word_ptr;
            string.wstr_len = word_len;
            style.font = g_mmiim_ui_config.font_candidate;
            style.align = ALIGN_BOTTOM;
            
            //added by lxf for dphone
#ifndef  MAINLCD_SIZE_128X64 
            if (MMIAPIIM_SGIsDrawSkin())
            {
                style.font_color = 0x0000;
            }
            else
            {
			    style.font_color = g_mmiim_ui_config.color_font_candidate;
            }
#endif

            if (word_len == 1 && SGIsSpecialChar(*word_ptr))
            {
                SGDrawSpecialChar(*word_ptr, &draw_rect, win_id, dev_info_ptr);
            }
            else
            {
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }
            
            disp_rect.left += word_width;
            
            disp_rect.left += MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        *lastdisp = index;

        if(edit_win_ptr->disp_first_index > 0)
        {
            edit_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
        }
    }
    if((imeResult->selecting[*lastdisp][0] 
        || (imeResult->imeType < WDI_FOREIGN_BEGIN && imeResult->curPage == MAX_PAGE_FOR_SOGOU - 1)
        || (imeResult->imeType > WDI_FOREIGN_BEGIN && imeResult->curPage == 19 )
        || (imeResult->imeType < WDI_FOREIGN_BEGIN && *lastdisp >= imeResult->selectingCount ))
        && (*lastdisp - 1 != edit_win_ptr->hot_index)
        )
    {
        if (MMIIM_DRAW_RIGHT2LEFT == edit_win_ptr->draw_direction)
        {
            edit_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
        }
        else
        {
            edit_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
        }
    }
    /*
    flagPageCand 候选词窗口是否需要显示上下翻页图标：
        0不显示，1显示上翻页，2显示下翻页，3显示上下翻页
    */
    if(edit_win_ptr->idc_flag & MMIIM_IDC_FLAG_LEFT)
    {
        DrawLeftIDC(dev_info_ptr, &(edit_win_ptr->rect_idc_left), hot_color);
    }

    if(edit_win_ptr->idc_flag & MMIIM_IDC_FLAG_RIGHT)
    {
        DrawRightIDC(dev_info_ptr, &(edit_win_ptr->rect_idc_right), hot_color);
    }

    return;

}

/******************************************************************************/
// Description: 是否是不可显示字符
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL BOOLEAN SGIsSpecialChar(wchar ch)
{
    if (ch == MMIIM_SPECIALCHAR_SPACE || ch == MMIIM_SPECIALCHAR_ENTER)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
// Description: 获取不可显示字符图片宽度
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL uint16 SGGetSpecialCharWidth(wchar ch)
{
    uint16 width = 0;
    // uint16 height = 0;

    if (ch == MMIIM_SPECIALCHAR_SPACE)
    {
        // GUIRES_GetImgWidthHeight(&width, &height, IMG_IMSP_SYMBOL_SPACE, win_id);
        width = 20;
    }
    else if (ch == MMIIM_SPECIALCHAR_ENTER)
    {
        // GUIRES_GetImgWidthHeight(&width, &height, IMG_IMSP_SYMBOL_ENTER, win_id);
        width = 20;
    }

    return width;
}

/******************************************************************************/
// Description: 获取字符串宽度
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL uint16 SGGetStringWidth(GUI_FONT_T font_type, const wchar* str_ptr, uint16 str_len)
{
    if (str_len == 1 && SGIsSpecialChar(*str_ptr))
    {
        return SGGetSpecialCharWidth(*str_ptr);
    }
    else
    {
        return GUI_GetStringWidth(font_type, str_ptr, str_len);
    }
}

/******************************************************************************/
// Description: 绘制不可显示字符
// Global resource dependence: NONE
// Author: 
// Note:
/******************************************************************************/
LOCAL void SGDrawSpecialChar(wchar ch, GUI_RECT_T *display_rect_ptr, MMI_WIN_ID_T win_id, const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    GUI_POINT_T point;

    point.x = display_rect_ptr->left;
    point.y = display_rect_ptr->top;

    if (ch == MMIIM_SPECIALCHAR_SPACE)
    {
        GUIRES_DisplayImg(&point, NULL, NULL, win_id, IMG_IMSP_SYMBOL_SPACE, dev_info_ptr);
    }
    else if (ch == MMIIM_SPECIALCHAR_ENTER)
    {
        GUIRES_DisplayImg(&point, NULL, NULL, win_id, IMG_IMSP_SYMBOL_ENTER, dev_info_ptr);
    }
}

PUBLIC void MMIIM_DrawCandWin(
							  GUI_LCD_DEV_INFO const *dev_info_ptr,
							  MMIIM_CAND_WIN_T *cand_win_ptr,
							  uint8 *lastdisp,
							  MMI_WIN_ID_T win_id,
                              void *imeResult_in
							  )
{
    GUI_COLOR_T hot_color = 0;
    int32 index = 0;
    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T draw_rect = {0}; 
    GUI_RECT_T hot_rect = {0};
    wchar *word_ptr = PNULL;
    uint16 word_len = 0;
    uint16 word_width = 0;
    wchar index_str[2] = {0};
    int16 right_margin = 0;
    int16 left_margin = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    WD_imeResult * imeResult = (WD_imeResult*)(imeResult_in);
	GUI_RECT_T label_rect = { 0 };
//added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT 
    GUI_COLOR_T      font_color; // color of font   
#endif

    //清空
    disp_rect = cand_win_ptr->rect_cand;
    disp_rect.left = cand_win_ptr->rect_idc_left.left;
    disp_rect.right = cand_win_ptr->rect_idc_right.right;
	//begin 20101202

    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  disp_rect.left,
                  disp_rect.top - 1,
                  disp_rect.right,
                  g_mmiim_ui_config.color_border);
	//end 20101202
//    if(cand_win_ptr->count == 0
//        || cand_win_ptr->state == MMIIM_WIN_STATE_NONE)
//    {
//        return;
//     }

    /*
    1 编辑状态，候选词标签不用显示
    2 选择状态，显示候选词标签
    */
    if(cand_win_ptr->has_lable && MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
    {
        hot_color = g_mmiim_ui_config.color_hot_candidate;
    }
    else
    {
        hot_color = g_mmiim_ui_config.color_hot_default;
    }

    /*
    buffer 候选词窗口的字符串，使用'\0'分割多个候选词
    count 当前候选词的个数
    hot_index 被选中的候选词id，从0开始计算，最少总有一个被选中
    */
    disp_rect = cand_win_ptr->rect_cand;
    word_ptr = cand_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);

            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
                if (MMIAPIIM_SGIsDrawSkin())
                {
                	style.font_color = 0x0000;
                }
                else
                {
                    style.font_color = g_mmiim_ui_config.color_font_lable;
                }
                style.align = ALIGN_RVMIDDLE;
                
				SCI_MEMCPY(&label_rect, &disp_rect, sizeof(GUI_RECT_T));
            }

            //无论如何留出lable位置            
            disp_rect.right -= cand_win_ptr->lable_width;

            draw_rect = disp_rect;
            
            //right已经减过了，left不可以再减了
            left_margin = disp_rect.right - word_width;
            
            if(left_margin < cand_win_ptr->rect_cand.left)
            {
                cand_win_ptr->is_self_idc = TRUE;
                cand_win_ptr->disp_per_page = index - cand_win_ptr->disp_first_index;
                
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.right += (cand_win_ptr->rect_cand.left - left_margin);
                if (index > 0)
                {
                    break;
                }
            }
            
            //画选中背景标识
            if(cand_win_ptr->has_hot && cand_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(left_margin < cand_win_ptr->rect_cand.left)
                {
                    hot_rect.left = cand_win_ptr->rect_cand.left + 2;
                }
                else
                {
                    hot_rect.left = hot_rect.right - word_width;
                }
                
                hot_rect.bottom = hot_rect.bottom - 1;
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT  
                style.font_color = 0xFFFF;  //与hot_color 反色              
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }
            
            //写字
            string.wstr_ptr = word_ptr;
            string.wstr_len = word_len;
            style.font = g_mmiim_ui_config.font_candidate;
            style.align = ALIGN_RVMIDDLE;
            
            //added by lxf for dphone
#ifndef MONOCHROME_LCD_SUPPORT
            if (MMIAPIIM_SGIsDrawSkin())
            {
            	style.font_color = 0x0000;
            }
            else
            {
    			style.font_color = g_mmiim_ui_config.color_font_candidate;
            }
#endif

            if (word_len == 1 && SGIsSpecialChar(*word_ptr))
            {
                GUI_RECT_T ch_rect = draw_rect;
                ch_rect.left = ch_rect.right - SGGetSpecialCharWidth(*word_ptr);
				SGDrawSpecialChar(*word_ptr, &ch_rect, win_id, dev_info_ptr);
            }
            else
            {
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    0x0,
                    GUISTR_TEXT_DIR_AUTO);
            }
            
            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
                if (MMIAPIIM_SGIsDrawSkin())
                {
                	style.font_color = 0x0000;
                }
                else
                {
                	style.font_color = g_mmiim_ui_config.color_font_lable;
                }
                style.align = ALIGN_RVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &label_rect,
                    &label_rect,
                    &string,
                    &style,
                    0,
                    GUISTR_TEXT_DIR_AUTO);
            }
			disp_rect.right -= word_width;
            
            disp_rect.right -= MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        *lastdisp = index;

        if(cand_win_ptr->disp_first_index > 0)
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
        }
    }
    else
    {
        //从左往右画
        for(index = 0; index < cand_win_ptr->count; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
            
            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
				if (MMIAPIIM_SGIsDrawSkin())
				{
					style.font_color = 0x0000;
				}
				else
                {
                    style.font_color = g_mmiim_ui_config.color_font_lable;
                }
                style.align = ALIGN_LVMIDDLE;
                
				SCI_MEMCPY(&label_rect, &disp_rect, sizeof(GUI_RECT_T));
            }

            //无论如何留出lable位置
            disp_rect.left += cand_win_ptr->lable_width;

            draw_rect = disp_rect;

            //left已经加过了，不可以再加lable
            right_margin = disp_rect.left + word_width;
            
            if(right_margin > cand_win_ptr->rect_cand.right)
            {
                cand_win_ptr->is_self_idc = TRUE;
                cand_win_ptr->disp_per_page = index - cand_win_ptr->disp_first_index;
                
                //修改文字显示的区域，不是从第一个字开始显示
                draw_rect.left -= (right_margin - cand_win_ptr->rect_cand.right);
                if (index > 0)
                {
                    break;
                }
            }
                        
            
            //画选中背景标识
            if(cand_win_ptr->has_hot && cand_win_ptr->hot_index == index)
            {
                hot_rect = disp_rect;
                if(right_margin > cand_win_ptr->rect_cand.right)
                {
                    hot_rect.right = cand_win_ptr->rect_cand.right - 1;
                }
                else
                {
                    hot_rect.right = hot_rect.left + word_width - 1;
                }
                hot_rect.bottom = hot_rect.bottom - 1;
                LCD_FillRect(dev_info_ptr, hot_rect, hot_color);
                //added by lxf for dphone
#ifdef MONOCHROME_LCD_SUPPORT 
                style.font_color = 0xFFFF;  //与hot_color 反色
                
            }
            else
            {
                style.font_color = hot_color;  
#endif
            }
            
            //写字
            string.wstr_ptr = word_ptr;
            string.wstr_len = word_len;
            style.font = g_mmiim_ui_config.font_candidate;
            style.align = ALIGN_VMIDDLE;
            
            //added by lxf for dphone
#ifndef MONOCHROME_LCD_SUPPORT 
            if (MMIAPIIM_SGIsDrawSkin())
            {
                style.font_color = 0x0000;
            }
            else
            style.font_color = g_mmiim_ui_config.color_font_candidate;
#endif

            if (word_len == 1 && SGIsSpecialChar(*word_ptr))
            {
                SGDrawSpecialChar(*word_ptr, &draw_rect, win_id, dev_info_ptr);
            }
            else
            {
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &draw_rect,
                    &disp_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }

            //写索引
            if(cand_win_ptr->has_lable)
            {
                if(MMIIM_WIN_STATE_CAND == cand_win_ptr->state)
                {
                    index_str[0] = '1' + index; //从1(one)开始
                }
                else
                {
                    index_str[0] = ' '; //空格
                }
                
                string.wstr_ptr = index_str;
                string.wstr_len = 1;
                style.font = g_mmiim_ui_config.font_lable;
				if (MMIAPIIM_SGIsDrawSkin())
				{
					style.font_color = 0x0000;
				}
				else
				{
					style.font_color = g_mmiim_ui_config.color_font_lable;
				}
                style.align = ALIGN_LVMIDDLE;
                
                GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                    &label_rect,
                    &label_rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                    GUISTR_TEXT_DIR_AUTO);
            }
            disp_rect.left += word_width;
            
            disp_rect.left += MMIIM_CAND_WORDS_SPACE;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
        
        *lastdisp = index;
        if(cand_win_ptr->disp_first_index > 0)
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
        }
    }
    
    if(!(
            (        
                imeResult->selecting[*lastdisp][0] == 0 
                || (imeResult->imeType < WDI_FOREIGN_BEGIN && imeResult->curPage == MAX_PAGE_FOR_SOGOU - 1)
                || (imeResult->imeType > WDI_FOREIGN_BEGIN && imeResult->curPage == 19 )
                || (imeResult->imeType < WDI_FOREIGN_BEGIN && *lastdisp >= imeResult->selectingCount ) 
            )
            && (*lastdisp - 1 == cand_win_ptr->hot_index)
         )
         && imeResult->selecting[0][0]
        )
    {
        if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_LEFT;
        }
        else
        {
            cand_win_ptr->idc_flag |= MMIIM_IDC_FLAG_RIGHT;
        }
    }
    /*
    flagPageCand 候选词窗口是否需要显示上下翻页图标：
        0不显示，1显示上翻页，2显示下翻页，3显示上下翻页
    */
    if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_LEFT)
    {
        DrawLeftIDC(dev_info_ptr, &(cand_win_ptr->rect_idc_left), hot_color);
    }

    if(cand_win_ptr->idc_flag & MMIIM_IDC_FLAG_RIGHT)
    {
        DrawRightIDC(dev_info_ptr, &(cand_win_ptr->rect_idc_right), hot_color);
    }

    return;
}


/******************************************************************************/
// Description: 预编辑窗口
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawPreEditWin(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 MMIIM_PRE_EDIT_WIN_T *edit_win_ptr,
								 MMI_WIN_ID_T   win_id
							  )
{
    GUI_COLOR_T color = 0;
    wchar buffer[MMIIM_PRE_BUFFER_LEN] = {0};
    wchar space[] = L" "; //空格
    wchar const *item_ptr = PNULL;
    uint16 item_len = 0;
    int32 index = 0;
    uint16 cursor_x = 0;
    GUI_RECT_T draw_rect = {0};
    int16 delta_x = 0;
    size_t cursor_len = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    wchar tip_string[MMIIM_WIN_BUFFER_LEN] = {0};
    int16 tip_len = 0;
    
    //画装饰部分
    LCD_DrawHLine(dev_info_ptr,
                  edit_win_ptr->rect.left,
                  edit_win_ptr->rect.top - 1,
                  edit_win_ptr->rect.right,
                  g_mmiim_ui_config.color_border);

    if(edit_win_ptr->state == MMIIM_WIN_STATE_NONE)
    {
        return;
    }
    
    if(edit_win_ptr->count == 0)
    {
        return;
    }

    if(MMIIM_WIN_STATE_CAND == edit_win_ptr->state)
    {
		if (MMIAPIIM_SGIsDrawSkin())
		{
			color = 0x0000;
		}
		else
        {
            color = g_mmiim_ui_config.color_font_default;
        }
    }
    else
    {
		if (MMIAPIIM_SGIsDrawSkin())
		{
			color = 0x0000;
		}
		else
        {
            color = g_mmiim_ui_config.color_font_pre_edit;
        }
    }

    /*
    buffer 需要在Pre-Edit窗口显示的字符串，使用'\0'分割多个项
    count Pre-Edit共有多少项
    cursor_index 需要显示光标的项id，从0开始计算
    */
    item_ptr = edit_win_ptr->buffer;

    if (MMIIM_DRAW_RIGHT2LEFT == edit_win_ptr->draw_direction)
    {
        //从右往左画，注意显示的内容是一样的，只是显示的区域有点不同        
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(edit_win_ptr->has_cursor && edit_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = edit_win_ptr->rect;
        
        //往前面移动一段距离
        cursor_x = draw_rect.right - cursor_x - 2;
        if (cursor_x < draw_rect.left)
        {
            delta_x = draw_rect.left - cursor_x;

            draw_rect.right += delta_x;
        }

        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_RVMIDDLE;
        
		if (string.wstr_len == 2 && SGIsSpecialChar(*(string.wstr_ptr)))
		{
			SGDrawSpecialChar(*(string.wstr_ptr), &draw_rect, win_id, dev_info_ptr);
		}
		else
        {
            uint16 str_width = GUI_GetStringWidth(style.font, string.wstr_ptr, string.wstr_len);
            if (str_width > (draw_rect.right - draw_rect.left))
            {
                draw_rect.left = draw_rect.right - str_width;
            }
            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                &draw_rect,
                &edit_win_ptr->rect,
                &string,
                &style,
                0x0,
                GUISTR_TEXT_DIR_AUTO);
        }
        
        if(edit_win_ptr->has_cursor && edit_win_ptr->count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x,
                edit_win_ptr->rect.top,
                edit_win_ptr->rect.bottom,
                color);
        }
    }
    else
    {
        //从左到右      
        for(index = 0; index < edit_win_ptr->count; index++)
        {
            MMIAPICOM_Wstrcat(buffer, item_ptr);
            
            if(edit_win_ptr->has_cursor && edit_win_ptr->cursor_index == index)
            {
                cursor_len = MMIAPICOM_Wstrlen(buffer);
                cursor_x = GUI_GetStringWidth(g_mmiim_ui_config.font_pre_edit, buffer, cursor_len);
            }
            
            MMIAPICOM_Wstrcat(buffer, space);
            
            item_len = MMIAPICOM_Wstrlen(item_ptr);
            item_ptr += item_len + 1;
        }
        
        draw_rect = edit_win_ptr->rect;
        
        //往前面移动一段距离
        while(cursor_x > draw_rect.right - 2) // -2 给光标留个地方
        {
            delta_x = cursor_x - (draw_rect.right - 2);
            
            cursor_x = draw_rect.right - 2;
            draw_rect.left -= delta_x;
        }
        
        string.wstr_ptr = buffer;
        string.wstr_len = (int16)MMIAPICOM_Wstrlen(buffer);
        style.font = g_mmiim_ui_config.font_pre_edit;
        style.font_color = color;
        style.align = ALIGN_VMIDDLE;
        
		if (string.wstr_len == 2 && SGIsSpecialChar(*(string.wstr_ptr)))
		{
			SGDrawSpecialChar(*(string.wstr_ptr), &draw_rect, win_id, dev_info_ptr);
		}
		else
        {
            uint16 str_width = GUI_GetStringWidth(style.font, string.wstr_ptr, string.wstr_len);
            if (str_width > (draw_rect.right - draw_rect.left))
            {
                uint8 i = 0;
                while (i < string.wstr_len && string.wstr_ptr[i] > 0x4DFF)
                {
                    i++;
                }
                if (!(i > 0 && GUI_GetStringWidth(style.font, string.wstr_ptr, i) < (draw_rect.right - draw_rect.left)))
                {
                    draw_rect.left = draw_rect.right - str_width;
                }
            }
            GUISTR_DrawTextToLCDInRect(dev_info_ptr,
                &draw_rect,
                &edit_win_ptr->rect,
                &string,
                &style,
                GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE,
                GUISTR_TEXT_DIR_AUTO);
        }
        
        if(edit_win_ptr->has_cursor && edit_win_ptr->count > 1)
        {
            DrawCursor(dev_info_ptr,
                cursor_x + edit_win_ptr->rect.left,
                edit_win_ptr->rect.top,
                edit_win_ptr->rect.bottom,
                color);
        }
    }

    return;
}

PUBLIC BOOLEAN MMIIM_TpDownCandAsKey(
                                     MMIIM_CAND_WIN_T *cand_win_ptr,
                                     GUI_POINT_T point,
                                     uint16 *index_ptr,
                                     uint8 lastdisplay
                                     )
{
    int32 index = 0;
    wchar const *word_ptr = PNULL;
    uint16 word_len = 0;
    uint16 word_width = 0;
    GUI_RECT_T word_rect = {0};
    BOOLEAN is_gotten_word = FALSE;

    if(!GUI_PointIsInRect(point, cand_win_ptr->rect_cand))
    {
        return FALSE;
    }

    word_ptr = cand_win_ptr->buffer;
    word_rect = cand_win_ptr->rect_cand;
    
    if (MMIIM_DRAW_RIGHT2LEFT == cand_win_ptr->draw_direction)
    {
        //字符从右往左显示
        for(index = 0; index < lastdisplay; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);

            word_rect.left = word_rect.right;
            word_rect.left -= cand_win_ptr->lable_width;
            word_rect.left -= word_width;
            word_rect.left -= MMIIM_CAND_WORDS_SPACE;
            word_rect.left += 1;
            
            //判定是否击中
            if(GUI_PointIsInRect(point, word_rect))
            {
                *index_ptr = index;
                is_gotten_word = TRUE;
                
                break;
            }
            
            word_rect.right = word_rect.left - 1;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
    }
    else
    {
        for(index = 0; index < lastdisplay; index++)
        {
            word_len = MMIAPICOM_Wstrlen(word_ptr);
            
            if(index < cand_win_ptr->disp_first_index)
            {
                //更新显示词，1为分隔字符'\0'
                word_ptr += word_len + 1;
                continue;
            }
            
            word_width = SGGetStringWidth(g_mmiim_ui_config.font_candidate, word_ptr, word_len);
            
            word_rect.right = word_rect.left;
            word_rect.right += cand_win_ptr->lable_width;
            word_rect.right += word_width;
            word_rect.right += MMIIM_CAND_WORDS_SPACE;
            word_rect.right -= 1;
            
            //判定是否击中
            if(GUI_PointIsInRect(point, word_rect))
            {
                *index_ptr = index;
                is_gotten_word = TRUE;
                
                break;
            }
            
            word_rect.left = word_rect.right + 1;
            
            //更新显示词，1为分隔字符'\0'
            word_ptr += word_len + 1;
        }
    }    

    return is_gotten_word;
}

#endif

#endif


#if defined(MMIIM_CORE_USE_T9)

#include "mmidisplay_data.h"
#include "guistring.h"

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                         LOCAL FUNCTION DECLARE                            */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
MMIIM_UI_CONFIG_T const g_mmiim_ui_config =
{
    //font
    SONG_FONT_14, //font_default
    SONG_FONT_14, //font_pre_edit
    SONG_FONT_14, //font_candidate
    SONG_FONT_14, //font_pre_choice
    SONG_FONT_6, //font_lable
    //font color
    0xef9d, //color_font_default
    0xef9d, //color_font_pre_edit
    0xef9d, //color_font_candidate
    0xef9d, //color_font_pre_choice
    0xef9d, //color_font_lable
    //bg, also meaning highlight color in samsung style.
    0x0040, //color_bg_default
    0x0040, //color_bg_pre_edit
    0x0040, //color_bg_candidate
    0x0040, //color_bg_pre_choice
    0x0040, //color_bg_lable
    //hot color
    0x001f, //color_hot_default
    0x001f, //color_hot_pre_edit
    0x001f, //color_hot_candidate
    0x001f, //color_hot_pre_choice
    0x001f, //color_hot_lable
    //用于装饰的边框
    0x8410, //color_border
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/ 




#endif



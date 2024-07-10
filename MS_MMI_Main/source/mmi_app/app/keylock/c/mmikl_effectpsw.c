/*****************************************************************************
** File Name:      mmikl_effectpsw.c                                         *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe input psw function          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/

#ifndef _MMIKL_EFFECTPSW_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_effectpsw.h"
#include "mmikl_effectmain.h"
#include "mmikl_export.h"
#include "window_parse.h"
#include "mmi_keylock_position.h"
#include "mmicc_id.h"
#include "mmiset_text.h"
#include "guistatusbar.h"
#include "guires.h"
#include "mmidisplay_data.h"
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

#define KEY_LOCK_PSW_ENTRY_ANIM_FRAME_NUM  32

#define KEY_LOCK_PSW_KEY_NUM 12

#define KEY_NUM_ONE_LINE    3
#define KEY_LINE_NUM            4
#define GET_KEY_POS_PSW(x, y, key)  \
{   \
    x = (s_key_lock_psw_info.key_width + 1) * (key % KEY_NUM_ONE_LINE);   \
    y = (s_key_lock_psw_info.key_height + 1) * (key / KEY_NUM_ONE_LINE) + 1;   \
}

typedef enum
{
    PSW_KEY_1,
    PSW_KEY_2,
    PSW_KEY_3,
    PSW_KEY_4,
    PSW_KEY_5,
    PSW_KEY_6,
    PSW_KEY_7,
    PSW_KEY_8,
    PSW_KEY_9,
    PSW_KEY_EMERGENCY,
    PSW_KEY_0,
    PSW_KEY_BACK, 
    PSW_KEY_MAX
}MMI_KEY_LOCK_PSW_KEY_E;

typedef enum
{
    KL_PSW_INPUT_1,
    KL_PSW_INPUT_2,
    KL_PSW_INPUT_2_AGAIN,
    KL_PSW_INPUT_CONFIRM1,
    KL_PSW_INPUT_CONFIRM2,
    KL_PSW_INPUT_MAX
}MMI_KEY_LOCK_PSW_INPUT_STATE_E;

typedef struct
{
    uint16 max;
    uint16 total;
    uint8 psw1[MMI_KL_PSW_LEN];
    uint8 psw2[MMI_KL_PSW_LEN];
    BOOLEAN is_re_check;
    BOOLEAN is_matched;
    MMI_KEY_LOCK_PSW_INPUT_STATE_E input_state;
}MMI_KEY_LOCK_PSW_INPUT_T;

typedef struct
{
    GUI_LCD_DEV_INFO keypad_layer;
    GUI_LCD_DEV_INFO input_layer;
    GUI_LCD_DEV_INFO key_layer;
    
    uint16 keypad_pos_y;
    uint16 keypad_area_height;
    
    uint16 input_area_pos_y;
    uint16 input_area_height;
    
    uint16 key_width;
    uint16 key_height;
    MMI_KEY_LOCK_PSW_INPUT_T input;

    MMI_TEXT_ID_T title_label1;
    MMI_TEXT_ID_T title_label2;
    
}MMI_KEY_LOCK_PSW_INFO_T;

LOCAL MMI_KEY_LOCK_PSW_INFO_T s_key_lock_psw_info = {0};

LOCAL BOOLEAN s_is_need_psw = FALSE;
LOCAL BOOLEAN s_is_psw_for_app = FALSE;



/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
  
/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  Get input layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetInputLayerPsw(void);

/******************************************************************************/
//  Description : handle psw key
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePswKey(MMI_KEY_LOCK_PSW_KEY_E key,MMI_WIN_ID_T win_id);

/******************************************************************************/
//  Description : covert presskey to psw key
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_KEY_LOCK_PSW_KEY_E CovertPresskeyToPswKey(uint16 msg_id);

/******************************************************************************/
//  Description : handle kl display window msg of psw type.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLWinPswMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        );

WINDOW_TABLE(MMIKC_DISP_PSW_WINTAB) =
{
    WIN_FUNC((uint32)HandleKLWinPswMsg),
//    WIN_HIDE_STATUS,
    WIN_ID(MMIKL_PSW_WIN_ID),
    WIN_STYLE( WS_DISABLE_RETURN_WIN|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG ),
//    CREATE_ANIM_CTRL(MMIKL_MAIN_ANIM_BG_ID,MMIKL_CLOCK_DISP_WIN_ID),
//    WIN_SOFTKEY(TXT_IDLE_UNLOCK,TXT_NULL,TXT_NULL),
    END_WIN
};
/******************************************************************************/
//  Description : Open Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKL_OpenPswWin(BOOLEAN is_form_app, MMI_PSW_FUNC call_back)
{
    if(MMK_IsOpenWin(MMIKL_PSW_WIN_ID))
    {
        MMK_CloseWin(MMIKL_PSW_WIN_ID);
    }
    if (PNULL != call_back)
    {
        s_is_psw_for_app = is_form_app;
    }
    else
    {
        s_is_psw_for_app = FALSE;
    }
    MMK_CreateWin((uint32*)MMIKC_DISP_PSW_WINTAB, (ADD_DATA)call_back); /*lint !e611*/
}

/******************************************************************************/
//  Description : Open Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsPswWinOpen(void)
{
    return MMK_IsOpenWin(MMIKL_PSW_WIN_ID);
}

/******************************************************************************/
//  Description : Close Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIKL_ClosePswWin(void)
{
    if(MMK_IsOpenWin(MMIKL_PSW_WIN_ID))
    {
        MMK_CloseWin(MMIKL_PSW_WIN_ID);
    }

    s_is_psw_for_app = FALSE;
}

/******************************************************************************/
//  Description : Close Psw window.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_IsNeedPsw(void)
{
    if(MMIAPISET_IsKLPswOn())
    {
        return s_is_need_psw;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :  Set psw flag.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_SetPswFlagPsw(BOOLEAN is_need)
{
    s_is_need_psw =is_need;
}

/******************************************************************************/
//  Description : If can open psw win.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_IsCanOpenPsw(void)
{
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description :  Get which win is opend.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMI_WIN_ID_T GetWinIdPsw(void)
{
    if(MMK_IsOpenWin(MMIKL_PSW_WIN_ID))
    {
        return MMIKL_PSW_WIN_ID;
    }
    else
    {
        return MMIKL_PSW_INPUT_WIN_ID;
    }
}

/*****************************************************************************/
//  Description :  Make a judgement that a appointed key is enable.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsKeyEnablePsw(MMI_KEY_LOCK_PSW_KEY_E key)
{
    BOOLEAN is_enable = TRUE;

    if(GetWinIdPsw() != MMIKL_PSW_WIN_ID)
    {
        if(PSW_KEY_EMERGENCY == key)
        {
            is_enable = FALSE;
        }
    }
    return is_enable;
}

/*****************************************************************************/
//  Description :  Get key layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetKeyLayerPsw(void)
{
    GUI_LCD_DEV_INFO layer = {0};

    if(s_key_lock_psw_info.key_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_psw_info.key_layer))
        {
            layer = s_key_lock_psw_info.key_layer;
        }
    }
    return layer;
}

/*****************************************************************************/
//  Description :  Get input layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetInputLayerPsw(void)
{
    GUI_LCD_DEV_INFO layer = {0};

    if(s_key_lock_psw_info.input_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_psw_info.input_layer))
        {
            layer = s_key_lock_psw_info.input_layer;
        }
    }
    return layer;
}

/*****************************************************************************/
//  Description :  Get keypad layer of psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetKeypadLayerPsw(void)
{
    GUI_LCD_DEV_INFO layer = {0};

    if(s_key_lock_psw_info.keypad_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_psw_info.keypad_layer))
        {
            layer = s_key_lock_psw_info.keypad_layer;
        }
    }
    return layer;    
}

/*****************************************************************************/
//  Description :  Append key layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendKeyLayerPsw(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetKeyLayerPsw();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  Append key layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendInputLayerPsw(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetInputLayerPsw();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  Append keypad layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/

LOCAL void AppendKeypadLayerPsw(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetKeypadLayerPsw();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }    
}

/*****************************************************************************/
//  Description :  create key layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateKeyLayerPsw(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_PSW_HILIGHT_KEY1, win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    MMI_KL_CREATE_LAYER(create_info, win_id, lcd_width, image_height, layer, result);
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_psw_info.key_layer = layer;
        AppendKeyLayerPsw();
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_psw_info.key_layer.block_id = 0;
    }
}

/*****************************************************************************/
//  Description :  create input layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateInputLayerPsw(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_PSW_INPUT_AREA_BG, win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    MMI_KL_CREATE_LAYER(create_info, win_id, lcd_width, image_height, layer, result);
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_psw_info.input_layer = layer;
        AppendInputLayerPsw();
        UILAYER_SetLayerPosition(&layer, 0, s_key_lock_psw_info.input_area_pos_y);
        
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_psw_info.input_layer.block_id = 0;
    }
}

 /*****************************************************************************/
//  Description :  create keypad layer of effect psw.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateKeypadLayerPsw(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_PSW_KEYPAD_BG, win_id);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    MMI_KL_CREATE_LAYER(create_info, win_id, lcd_width, image_height, layer, result);
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_psw_info.keypad_layer = layer;
        AppendKeypadLayerPsw();
        UILAYER_SetLayerPosition(&layer, 0, s_key_lock_psw_info.keypad_pos_y);
        
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_psw_info.keypad_layer.block_id = 0;
    }
}

LOCAL void SetTitlePsw(MMI_TEXT_ID_T label1, MMI_TEXT_ID_T label2)
{
    s_key_lock_psw_info.title_label1 = label1;
    s_key_lock_psw_info.title_label2 = label2;
}

/******************************************************************************/
//  Description : Display key pad background of psw type unlocking screen.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayKeypadBgPsw(void)
{
    GUI_LCD_DEV_INFO layer = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_POINT_T pos  = {0};
    GUI_RECT_T rect = {0};
    uint16 img_width = 0, img_height = 0;
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};
    MMI_WIN_ID_T win_id = GetWinIdPsw() ;
    
    layer = GetKeypadLayerPsw();
    
    pos.y = s_key_lock_psw_info.keypad_pos_y;
    UILAYER_Clear(&layer);
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_KEYPAD_BG, &layer);

    style.font = MMI_DEFAULT_SMALL_FONT;
    style.align = ALIGN_HVMIDDLE;
    style.font_color = MMI_WHITE_COLOR;
    
    if(IsKeyEnablePsw(PSW_KEY_EMERGENCY))
    {
        GET_KEY_POS_PSW(pos.x, pos.y, PSW_KEY_EMERGENCY);
        pos.y += s_key_lock_psw_info.keypad_pos_y;
        //GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_HILIGHT_KEY_EMERGENCY, &lcd_dev_info);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_HILIGHT_KEY_EMERGENCY, win_id);/* !!!*/
        rect.left = pos.x;
        rect.top = pos.y;
        rect.right = pos.x + img_width - 1;
        rect.bottom = pos.y + img_height - 1;
    
        MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &string);
        GUISTR_DrawTextToLCDInRect(&layer,
                &rect,
                &rect,
                &string,
                &style,
                GUISTR_STATE_ALIGN,
                GUISTR_TEXT_DIR_AUTO
                );
        
    }

    GET_KEY_POS_PSW(pos.x, pos.y, PSW_KEY_BACK);
    pos.y += s_key_lock_psw_info.keypad_pos_y;
    
//    if(win_id == MMIKL_PSW_WIN_ID)
    {
        if(s_key_lock_psw_info.input.total > 0)
        {
            GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_KEY_BACK, &layer);
        }
        else
        {
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_KEY_BACK, win_id);
            rect.left = pos.x;
            rect.top = pos.y;
            rect.right = pos.x + img_width - 1;
            rect.bottom = pos.y + img_height - 1;
        
            MMI_GetLabelTextByLang(STXT_CANCEL, &string);
            GUISTR_DrawTextToLCDInRect(&layer,
                    &rect,
                    &rect,
                    &string,
                    &style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    );
        }
    }
//    else
//    {
//        GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_KEY_BACK, &layer);
//    }
}

/******************************************************************************/
//  Description : Display title of psw effect.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayTitleBGPsw()
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    GUI_POINT_T pos  = {0};
    MMI_WIN_ID_T win_id = GetWinIdPsw();

    pos.y = MMITHEME_GetStatusBarHeight();
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_TITLE_BG, &lcd_dev_info);
}
/******************************************************************************/
//  Description : Display title of psw effect.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayTitlePsw(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_STRING_T string = {0};
    uint16 img_width = 0, img_height = 0;
    GUISTR_STYLE_T style = {0};
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    GUI_RECT_T rect = {0};
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    MMI_TEXT_ID_T label1 = 0, label2 = 0;
    
    label1 = s_key_lock_psw_info.title_label1;
    label2 = s_key_lock_psw_info.title_label2;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_TITLE_BG, win_id);

    style.font = MMI_DEFAULT_BIG_FONT;
    style.align = ALIGN_HVMIDDLE;
    style.font_color = MMI_WHITE_COLOR;
    
    MMI_GetLabelTextByLang(label1, &string);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    rect.left = 0;
    rect.right = lcd_width - 1;
    rect.top = MMITHEME_GetStatusBarHeight();

    if(0 == label2)
    {
        rect.bottom = rect.top + img_height;
    }
    else
    {
        uint16 *buff = PNULL;
        buff = (uint16 *)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
        
        if (PNULL == buff)
        {
            return;
        }
        /* 包括状态栏，都做一层蒙板效果*/
        rect.bottom = rect.top + img_height;        
        rect.top = 0;
        LCD_FillDestBuffArgbRect( buff, lcd_width, lcd_width, lcd_height, rect, MMI_RED_COLOR, 80);

        /* 恢复文本区域大小*/
        rect.top = MMITHEME_GetStatusBarHeight();        
        rect.bottom = rect.top + img_height / 2;
    }
    GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
            &rect,
            &rect,
            &string,
            &style,
            GUISTR_STATE_ALIGN,
            GUISTR_TEXT_DIR_AUTO
            );

    if(label2 != 0)
    {
        style.font = MMI_DEFAULT_NORMAL_FONT;
        rect.top = rect.bottom + 1;
        rect.bottom = rect.top + img_height / 2;
        MMI_GetLabelTextByLang(label2, &string);
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info,
                &rect,
                &rect,
                &string,
                &style,
                GUISTR_STATE_ALIGN,
                GUISTR_TEXT_DIR_AUTO
                );
    }
}

/******************************************************************************/
//  Description : Display title of psw effect.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayInputBgPsw(void)
{
    GUI_LCD_DEV_INFO layer = {0};
    GUI_POINT_T pos  = {0};
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    pos.y = s_key_lock_psw_info.input_area_pos_y;
    layer = GetInputLayerPsw();
    UILAYER_Clear(&layer);
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_INPUT_AREA_BG, &layer);
}

 /*****************************************************************************/
//  Description :  set wall paper params.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayThemeBgPsw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T rect  = {0 };   
    uint16 width = 0, height = 0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);

    rect.right = width - 1;
    rect.bottom = height - 1;

    MMITHEME_DisplayBg(&rect, win_id, &lcd_dev);
    GUIWIN_ReDrawStbCtrl(win_id, FALSE);
}


/******************************************************************************/
//  Description : Display all the  background of psw type win.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayBgPsw(void)
{
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    DisplayThemeBgPsw(win_id);
    DisplayTitleBGPsw();
    DisplayInputBgPsw();
    DisplayKeypadBgPsw();
}

/******************************************************************************/
//  Description : Display high light key.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayHilightKey(MMI_KEY_LOCK_PSW_KEY_E key)
{
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO     layer = {0};
    GUI_POINT_T pos  = {0};
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    uint16 img_width = 0, img_height = 0 ;
    GUI_RECT_T rect = {0};
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};

    
    if(key >= KEY_LOCK_PSW_KEY_NUM)
    {
        return;
    }
    if(!IsKeyEnablePsw(key))
    {
        return;
    }
    img_id = IMAGE_KL_PSW_HILIGHT_KEY1 + key;
    layer = GetKeyLayerPsw();
    UILAYER_SetLayerPosition(&layer, 0, 0);

    if(PSW_KEY_BACK != key)
    {
        GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, img_id, &layer);
    }

    style.font = MMI_DEFAULT_SMALL_FONT;
    style.align = ALIGN_HVMIDDLE;
    style.font_color = MMI_WHITE_COLOR;

    if(PSW_KEY_EMERGENCY == key)
    {
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_HILIGHT_KEY_EMERGENCY, win_id);/* !!!*/
        rect.right = img_width - 1;
        rect.bottom = img_height - 1;
    
        MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &string);
        GUISTR_DrawTextToLCDInRect(&layer,
                &rect,
                &rect,
                &string,
                &style,
                GUISTR_STATE_ALIGN,
                GUISTR_TEXT_DIR_AUTO
                );
    }

    else if(PSW_KEY_BACK == key)
    {        
        //if(win_id == MMIKL_PSW_WIN_ID)
        {
            if(s_key_lock_psw_info.input.total > 0)
            {
                GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_HILIGHT_KEY_BACK, &layer);
            }
            else
            {
                GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_KEY_BACK, win_id);
                rect.right = img_width - 1;
                rect.bottom = img_height - 1;

                /* we should display an empty pic, IMAGE_KL_PSW_HILIGHT_KEY_EMERGENCY is we need */
                GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_HILIGHT_KEY_EMERGENCY, &layer);

                MMI_GetLabelTextByLang(STXT_CANCEL, &string);
                GUISTR_DrawTextToLCDInRect(&layer,
                        &rect,
                        &rect,
                        &string,
                        &style,
                        GUISTR_STATE_ALIGN,
                        GUISTR_TEXT_DIR_AUTO
                        );
            }
        }
        //else
        //{
        //    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, IMAGE_KL_PSW_HILIGHT_KEY_BACK, &layer);
        //}
    }
    
    GET_KEY_POS_PSW(pos.x, pos.y, key);
    UILAYER_SetLayerPosition(&layer, pos.x, pos.y + s_key_lock_psw_info.keypad_pos_y);
}

/*****************************************************************************/
//  Description :  Clear the high light key
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void ClearHilightKey(MMI_KEY_LOCK_PSW_KEY_E key)
{
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO     layer = {0};

    if(key >= KEY_LOCK_PSW_KEY_NUM)
    {
        return;
    }

    img_id = IMAGE_KL_PSW_HILIGHT_KEY1 + key;
    layer = GetKeyLayerPsw();
    UILAYER_Clear(&layer);
}

/******************************************************************************/
//  Description : Display high light key.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL  MMI_KEY_LOCK_PSW_KEY_E GetPressedKeyPsw(GUI_POINT_T pos)
{
    uint32 i = 0;
    uint16 h = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    MMI_KEY_LOCK_PSW_KEY_E key = PSW_KEY_MAX;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if(pos.y >= s_key_lock_psw_info.keypad_pos_y && pos.y < lcd_height && pos.x < lcd_width)
    {
        h = s_key_lock_psw_info.keypad_pos_y;
        for(i = 0; i < KEY_LINE_NUM; i++)
        {
            h += s_key_lock_psw_info.key_height;
            if(pos.y >= h)
            {
                continue;
            }
            if(pos.x < s_key_lock_psw_info.key_width)
            {
                key = PSW_KEY_1;
            }
            else if(pos.x < s_key_lock_psw_info.key_width * 2)
            {
                key = PSW_KEY_2;
            }
            else
            {
                key = PSW_KEY_3;
            }
            break;
        }
        
        key += (i * KEY_NUM_ONE_LINE); 
    }
    
    return key;
}

/*****************************************************************************/
//  Description :  Get the appointed input code's rect.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetCodeRectPsw(uint16 index)
{
    GUI_RECT_T rect = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 useful_width = 0;
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    useful_width = lcd_width - KEY_LOCK_PSW_CODE_HMARGIN * 2;
    rect.top = s_key_lock_psw_info.input_area_pos_y;
    rect.left = KEY_LOCK_PSW_CODE_HMARGIN +  useful_width * index / MMI_KL_PSW_LEN;
    rect.right = KEY_LOCK_PSW_CODE_HMARGIN + useful_width * (index + 1) / MMI_KL_PSW_LEN - 1;
    rect.bottom = rect.top + s_key_lock_psw_info.input_area_height;
    return rect;
}

/*****************************************************************************/
//  Description :  Display one input code.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayOneCodePsw(uint16 index, BOOLEAN is_space)
{
    MMI_IMAGE_ID_T img_id = 0;
    GUI_POINT_T pos ={0};
    GUI_LCD_DEV_INFO     layer = {0};
    GUI_RECT_T rect = {0};
    uint16 img_width = 0, img_height = 0;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    layer = GetInputLayerPsw();
    
    rect = GetCodeRectPsw(index);
    
    if(is_space)
    { 
        img_id = IMAGE_KL_PSW_SPACE;
    }
    else
    {
        img_id = IMAGE_KL_PSW_POINT;
    }
    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);

    pos.x = rect.left + (rect.right - rect.left + 1 - img_width)/2;
    pos.y = rect.top + (rect.bottom - rect.top + 1 - img_height)/2;
    GUIRES_DisplayImg(&pos, PNULL, PNULL, win_id, img_id, &layer);

}

/*****************************************************************************/
//  Description :  Display the all inputed code.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayInputPsw(void)
{
    uint i = 0;
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    
    for(i = 0; i <MMI_KL_PSW_LEN;  i++)
    {
        DisplayOneCodePsw(i, TRUE);
    }
    
    for(i = 0; i < psw->total; i++)
    {
        DisplayOneCodePsw(i, FALSE);
    }
}

/*****************************************************************************/
//  Description :  Add the input key to buff, and display it.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void  AddKeyAndDisplayPsw(MMI_KEY_LOCK_PSW_KEY_E key)
{
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    if(psw->total < psw->max && key < PSW_KEY_MAX)
    {
        psw->psw1[psw->total] = key;
        if(0 == psw->total)
        {
            psw->total++;
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            DisplayOneCodePsw(psw->total, FALSE);
            psw->total++;
        }
    }
}

/*****************************************************************************/
//  Description :  Delete the last key from the input buff and update display.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DelKeyAndDisplayPsw(void)
{
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;

    if(psw->total > 0)
    {
        psw->total--;
        psw->psw1[psw->total] = 0;
        if(0 == psw->total)
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }       
        else
        {
            DisplayOneCodePsw(psw->total, TRUE);
        }
    }
}

/*****************************************************************************/
//  Description :  If input is full.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInputFullPsw(void)
{
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    return (psw->total >= MMI_KL_PSW_LEN);
}

/*****************************************************************************/
//  Description :  If input buff is empty.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInputEmptyPsw(void)
{
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    return (0 == psw->total);
}

/*****************************************************************************/
//  Description :  If two times input code is same.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTwoInputSamePsw(void)
{
    uint32 i = 0;
    BOOLEAN is_same = TRUE;
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    
    for(i = 0; i < MMI_KL_PSW_LEN; i++)
    {
        if(psw->psw1[i] != psw->psw2[i])
        {
            is_same = FALSE;
            break;
        }
    }
    return is_same;
}

/*****************************************************************************/
//  Description :  If the input code is same with setted password.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN IsSameWithSettedPsw(void)
{
    uint8 psw[MMI_KL_PSW_LEN + 1];
    uint32 i = 0;
    BOOLEAN is_same = TRUE;
    
    MMIAPISET_GetKLPsw(psw);
    for(i = 0; i < MMI_KL_PSW_LEN; i++)
    {
        if(psw[i] != s_key_lock_psw_info.input.psw1[i])
        {
            is_same = FALSE;
            break;
        }
    }
    return is_same;
}

/*****************************************************************************/
//  Description :  Get which win is opend.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DoPswCheckFailedPsw(void)
{
    MMI_KEY_LOCK_PSW_INPUT_T *psw = &s_key_lock_psw_info.input;
    MMI_WIN_ID_T win_id = GetWinIdPsw();

    if(!psw->is_re_check)
    {
        psw->is_re_check = TRUE;
    }
    else
    {
        //start a timer to blend the reminding.
    }
    psw->total = 0;
    SCI_MEMSET(&psw->psw1, 0, sizeof(psw->psw1));
    SetTitlePsw(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);

    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

/*****************************************************************************/
//  Description :  Do entry animation.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DoEntryAnimPsw()
{
    uint32 index = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    GUI_RECT_T rect = {0};
    int16 x = 0, y = 0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    rect.right = lcd_width - 1;
    rect.bottom = lcd_height - 1;

    for(index = 1; index < KEY_LOCK_PSW_ENTRY_ANIM_FRAME_NUM; index++)
    {
#if 0    
        y = lcd_height - s_key_lock_psw_info.keypad_area_height * index/8;
        UILAYER_SetLayerPosition(&s_key_lock_psw_info.keypad_layer, 0, y);
        UILAYER_SetLayerPosition(&s_key_lock_psw_info.input_layer, (index - 8) * lcd_width/8, y - s_key_lock_psw_info.input_area_height - 1);
        //GUILCD_InvalidateRect(GUI_MAIN_LCD_ID, rect, 0);     
        MMITHEME_UpdateRect();
        SCI_SLEEP(5 * (8 - index));        
#else
        y = lcd_height - MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, s_key_lock_psw_info.keypad_area_height, KEY_LOCK_PSW_ENTRY_ANIM_FRAME_NUM, index);
        UILAYER_SetLayerPosition(&s_key_lock_psw_info.keypad_layer, 0, y);
        x = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, lcd_width, KEY_LOCK_PSW_ENTRY_ANIM_FRAME_NUM, index) - lcd_width;
        UILAYER_SetLayerPosition(&s_key_lock_psw_info.input_layer, x, y - s_key_lock_psw_info.input_area_height);
        MMITHEME_UpdateRect();
        //SCI_SLEEP(20);                
#endif
    }
    UILAYER_SetLayerPosition(&s_key_lock_psw_info.keypad_layer, 0, s_key_lock_psw_info.keypad_pos_y);
    UILAYER_SetLayerPosition(&s_key_lock_psw_info.input_layer, 0, s_key_lock_psw_info.input_area_pos_y);
    
}

/*****************************************************************************/
//  Description :  init parameters.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitParamPsw(void)
{
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 img_width = 0, img_height = 0;
    MMI_WIN_ID_T win_id = GetWinIdPsw();
    
    SCI_MEMSET(&s_key_lock_psw_info, 0, sizeof(s_key_lock_psw_info));
    GUIRES_GetImgWidthHeight(&s_key_lock_psw_info.key_width, &s_key_lock_psw_info.key_height, IMAGE_KL_PSW_HILIGHT_KEY1, win_id);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_KEYPAD_BG, win_id);

    s_key_lock_psw_info.keypad_pos_y = lcd_height - img_height;
    s_key_lock_psw_info.keypad_area_height = img_height;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_PSW_INPUT_AREA_BG, win_id);
    s_key_lock_psw_info.input_area_pos_y  = s_key_lock_psw_info.keypad_pos_y - img_height;
    s_key_lock_psw_info.input_area_height = img_height;

    s_key_lock_psw_info.input.max = MMI_KL_PSW_LEN;

    CreateKeypadLayerPsw();
    CreateInputLayerPsw();
    CreateKeyLayerPsw();
}
/******************************************************************************/
//  Description : handle psw key
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandlePswKey(MMI_KEY_LOCK_PSW_KEY_E key,MMI_WIN_ID_T win_id)
{
    if(key < PSW_KEY_MAX)
    {
        ClearHilightKey(key);
        if(PSW_KEY_BACK == key)
        {
            if(IsInputEmptyPsw())
            {
                if (!s_is_psw_for_app)
                {
                    MMIAPIKL_LockPhone();
                    /* In this operation, we do not open program, so we clear the flag */
                    MMIKL_InitProcFlag();
                }
                MMIAPIKL_ClosePswWin();
            }
            else
            {
                DelKeyAndDisplayPsw();
            }
        }
        else if(PSW_KEY_EMERGENCY == key)
        { 
            if (!MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
                &&!MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))//来电去电状态，不允许进行紧急呼叫
            {
                s_key_lock_psw_info.input.total = 0;
                MMK_CreatePubEditWin((uint32*)MMICC_EMERGENCY_WIN_TAB, PNULL); 
            }
        }
        else
        {
            AddKeyAndDisplayPsw(key);
            if(IsInputFullPsw())
            {
                MMITHEME_UpdateRect();
                SCI_SLEEP(5);

                if(IsSameWithSettedPsw())
                {
                    MMIKL_SetPswFlagPsw(FALSE);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    DoPswCheckFailedPsw();
                }
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/******************************************************************************/
//  Description : handle kl display window msg of psw type.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleKLWinPswMsg(
                                MMI_WIN_ID_T    win_id, 
                                uint16          msg_id, 
                                DPARAM          param
                                )
{
    MMI_KEY_LOCK_PSW_KEY_E key = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    static GUI_POINT_T tp_down_point = {0};
    static BOOLEAN is_has_painted = FALSE;
    
    //SCI_TRACE_LOW:"HandleKLWinPswMsg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_EFFECTPSW_1224_112_2_18_2_30_56_2,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE); 
	    GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);       
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState( win_id, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, TRUE);        
        is_has_painted = FALSE;
        InitParamPsw();
        SetTitlePsw(TXT_KL_PSW_INPUT_PSW, 0);
        tp_down_point.x = 0;
        tp_down_point.y = 0;

        if (MMK_IsOpenWin(MMICC_EMERGENCY_WIN_ID))
        {
            MMK_CloseWin(MMICC_EMERGENCY_WIN_ID);
        }
        break;
    case MSG_FULL_PAINT:
        DisplayBgPsw();
        DisplayInputPsw();
        DisplayTitlePsw();
        
        if(!is_has_painted)
        {
            MMITHEME_SetUpdateDelayCount(0);
        
            is_has_painted = TRUE;
            DoEntryAnimPsw();
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_down_point.x = MMK_GET_TP_X(param);
        tp_down_point.y = MMK_GET_TP_Y(param);
        key = GetPressedKeyPsw(tp_down_point);
        
        if(key < PSW_KEY_MAX)
        {
            DisplayHilightKey(key);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        key = GetPressedKeyPsw(tp_down_point);
        HandlePswKey(key,win_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_GET_FOCUS:
        AppendKeypadLayerPsw();
        AppendInputLayerPsw();
        AppendKeyLayerPsw();
        break;
    case MSG_CLOSE_WINDOW:
        {
            MMI_PSW_FUNC call_back_func = (MMI_PSW_FUNC)MMK_GetWinAddDataPtr(win_id); /*lint !e611*/
            if (s_is_psw_for_app)
            {
                if (PNULL!= call_back_func && !IsInputEmptyPsw() && IsSameWithSettedPsw())
                {
                    call_back_func();
                }
            }
            else
            {
                MMIKL_OpenEffectProc(win_id);
            }
        }
        break;
    default:
        key = CovertPresskeyToPswKey(msg_id);
        if(!HandlePswKey(key,win_id))
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }

    return recode;
}


/******************************************************************************/
//  Description : check the input password widh the saved one.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DoSettingInputFullPsw(MMI_WIN_ID_T win_id)
{
#define RESET_INPUT \
    {   \
        s_key_lock_psw_info.input.total = 0;    \
    }
#define SET_INPUT_STATE(x)  \
    {   \
        s_key_lock_psw_info.input.input_state = x;  \
    }
#define SET_TITLE(label1,label2)    \
    {   \
        SetTitlePsw(label1, label2);    \
        is_need_update = TRUE;  \
    }
    ADD_DATA open_type = MMK_GetWinAddDataPtr(win_id);
    BOOLEAN is_need_update = FALSE;
    switch((uint32 )open_type)
    {
    case MMI_KL_OPEN_PSW_INPUT_ENTER:
        if(MMIAPISET_IsKLPswOn())
        {
            if(IsSameWithSettedPsw())
            {
                MMIAPISET_OpenKLPswSettingWin();
                MMK_CloseWin(MMIKL_PSW_INPUT_WIN_ID);
            }
            else
            {
                RESET_INPUT;
                SET_TITLE(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
            }
        }
        else
        {
            switch(s_key_lock_psw_info.input.input_state)
            {
            case KL_PSW_INPUT_1:
                SCI_MEMCPY(s_key_lock_psw_info.input.psw2, s_key_lock_psw_info.input.psw1, sizeof(s_key_lock_psw_info.input.psw2));
                RESET_INPUT;
                SET_TITLE(TXT_KL_PSW_INPUT_PSW_AGAIN, 0);
                SET_INPUT_STATE(KL_PSW_INPUT_2);                
                break;
            case KL_PSW_INPUT_2:
                if(IsTwoInputSamePsw())
                {
                    MMIAPISET_SetKLPsw(s_key_lock_psw_info.input.psw1);
                    MMIAPISET_SetKLPswOnOff(TRUE);
                    MMIAPISET_OpenKLPswSettingWin();
                    MMK_CloseWin(MMIKL_PSW_INPUT_WIN_ID);
                }
                else
                {
                    RESET_INPUT;
                    SET_INPUT_STATE(KL_PSW_INPUT_1);
                    SET_TITLE(TXT_KL_PSW_NOT_SAME, TXT_KL_PSW_INPUT_PSW_AGAIN);
                }
                break;
            default:
                break;
            }
        }        
        break;
    case MMI_KL_OPEN_PSW_INPUT_SWITCHOFF:
        if(IsSameWithSettedPsw())
        {
            MMIAPISET_SetKLPswOnOff(FALSE);
            MMK_CloseWin(MMIKL_PSW_INPUT_WIN_ID);
            MMK_CloseWin(MMIKL_PSW_SETTING_WIN_ID);
        }
        else
        {
            RESET_INPUT;
            switch(s_key_lock_psw_info.input.input_state)
            {
            case KL_PSW_INPUT_1:
                SET_INPUT_STATE(KL_PSW_INPUT_CONFIRM1);
                SET_TITLE(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
                break;
            case KL_PSW_INPUT_CONFIRM1:
                SET_INPUT_STATE(KL_PSW_INPUT_CONFIRM2);
                SET_TITLE(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
                break;
            case KL_PSW_INPUT_CONFIRM2:
                SET_TITLE(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
                break;
            default:
                break;
            }
        }
        break;
    case MMI_KL_OPEN_PSW_INPUT_CHANGE:
        switch(s_key_lock_psw_info.input.input_state)
        {
        case KL_PSW_INPUT_1:
            if(IsSameWithSettedPsw())
            {
                RESET_INPUT;
                SET_INPUT_STATE( KL_PSW_INPUT_2);
                SET_TITLE(TXT_SET_KL_PSW_INPUT, 0);
            }
            else
            {            
                RESET_INPUT;
                SET_TITLE(TXT_KL_PSW_WRONG_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
            }
            break;
        case KL_PSW_INPUT_2:
            if(IsSameWithSettedPsw())
            {
                SET_TITLE(TXT_KL_PSW_USE_DIFFERENT_PSW, TXT_KL_PSW_INPUT_PSW_AGAIN);
                RESET_INPUT;
            }
            else
            {
                SCI_MEMCPY(&s_key_lock_psw_info.input.psw2, &s_key_lock_psw_info.input.psw1, sizeof(s_key_lock_psw_info.input.psw2));
                RESET_INPUT;
                SET_INPUT_STATE(KL_PSW_INPUT_2_AGAIN);
                SET_TITLE(TXT_KL_PSW_INPUT_PSW_AGAIN, 0);                
            }
            break;
        case KL_PSW_INPUT_2_AGAIN:
            if(IsTwoInputSamePsw())
            {
                MMIAPISET_SetKLPsw(s_key_lock_psw_info.input.psw1);
                MMK_CloseWin(win_id);
            }
            else
            {
                RESET_INPUT;
                SET_INPUT_STATE(KL_PSW_INPUT_2);
                SET_TITLE(TXT_KL_PSW_NOT_SAME, TXT_KL_PSW_INPUT_PSW_AGAIN);
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    
    if(is_need_update)
    {
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
}

/******************************************************************************/
//  Description : covert presskey to psw key
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_KEY_LOCK_PSW_KEY_E CovertPresskeyToPswKey(uint16 msg_id)
{
    MMI_KEY_LOCK_PSW_KEY_E key = PSW_KEY_MAX;
    
    switch(msg_id) 
    {
        case MSG_KEYUP_Q:
        case MSG_KEYUP_1:
            key = PSW_KEY_1;
            break;
        case MSG_KEYUP_T:
        case MSG_KEYUP_2:
            key = PSW_KEY_2;
            break;
        case MSG_KEYUP_Y:
        case MSG_KEYUP_3:
            key = PSW_KEY_3;
            break;
        case MSG_KEYUP_F:
        case MSG_KEYUP_4:
            key = PSW_KEY_4;
            break;
        case MSG_KEYUP_G:
        case MSG_KEYUP_5:
            key = PSW_KEY_5;
            break;
        case MSG_KEYUP_H:
        case MSG_KEYUP_6:
            key = PSW_KEY_6;
            break;
        case MSG_KEYUP_V:
        case MSG_KEYUP_7:
            key = PSW_KEY_7;
            break;
        case MSG_KEYUP_B:
        case MSG_KEYUP_8:
            key = PSW_KEY_8;
            break;
        case MSG_KEYUP_N:
        case MSG_KEYUP_9:
            key = PSW_KEY_9;
            break;
        case MSG_KEYUP_M:
        case MSG_KEYUP_0:
            key = PSW_KEY_0;
            break;
        case MSG_KEYUP_DEL:
        case MSG_KEYUP_HASH:
            key = PSW_KEY_BACK;
            break;
        case MSG_KEYUP_STAR:
            key = PSW_KEY_EMERGENCY;
            break;
        default:
            key = PSW_KEY_MAX;
            break;
    }
    return key;

}
/******************************************************************************/
//  Description : handle kl password setting.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIKL_HandleKLPswInputWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        )
{
    MMI_KEY_LOCK_PSW_KEY_E key = 0;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    static GUI_POINT_T tp_down_point = {0};
    
    //SCI_TRACE_LOW:"HandleKLPswInputWinMsg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_EFFECTPSW_1537_112_2_18_2_30_56_3,(uint8*)"d",msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);        
        //GUIWIN_SetStbBgIsDesplay(win_id, FALSE);需求状态栏加半透黑色背景，mmikl_effectmain.c统一处理了
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState( win_id, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, TRUE);                
        InitParamPsw();
        SetTitlePsw(TXT_KL_PSW_INPUT_PSW, 0);
        break;
    case MSG_FULL_PAINT:
        DisplayBgPsw();
        DisplayInputPsw();
        DisplayTitlePsw();
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        tp_down_point.x = MMK_GET_TP_X(param);
        tp_down_point.y = MMK_GET_TP_Y(param);
        key = GetPressedKeyPsw(tp_down_point);
        
        if(key < PSW_KEY_MAX)
        {
            DisplayHilightKey(key);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        key = GetPressedKeyPsw(tp_down_point);
        if(key < PSW_KEY_MAX)
        {
            ClearHilightKey(key);
            if(PSW_KEY_BACK == key)
            {
                if(IsInputEmptyPsw())
                {
                    MMK_CloseWin(win_id);
                }
                else
                {
                    DelKeyAndDisplayPsw();
                }
            }
            else if(PSW_KEY_EMERGENCY == key)
            {
                /* Nothing to do */
            }
            else
            {
                AddKeyAndDisplayPsw(key);
                if(IsInputFullPsw())
                {
                    MMITHEME_UpdateRect();
                    SCI_SLEEP(5);
                    DoSettingInputFullPsw(win_id);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_GET_FOCUS:
        InitParamPsw();
        SetTitlePsw(TXT_KL_PSW_INPUT_PSW, 0);   
        break;
    case MSG_LOSE_FOCUS:
        UILAYER_ReleaseAllLayerOfHandle(win_id);        
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        key= CovertPresskeyToPswKey(msg_id);
        if (PSW_KEY_MAX !=key)
        {
            if(PSW_KEY_BACK == key)
            {
                if(IsInputEmptyPsw())
                {
                    MMK_CloseWin(win_id);
                }
                else
                {
                    DelKeyAndDisplayPsw();
                }
            }
            else
            {
                AddKeyAndDisplayPsw(key);
                if(IsInputFullPsw())
                {
                    MMITHEME_UpdateRect();
                    SCI_SLEEP(5);
                    DoSettingInputFullPsw(win_id);
                }
            }
        }
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
    }

    return recode;
}
#endif //defined MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
#endif




/*Edit by script, ignore 6 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527

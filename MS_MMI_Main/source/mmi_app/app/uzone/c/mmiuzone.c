/*****************************************************************************
** File Name:      mmiuzone.c                                                *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica              Creat
******************************************************************************/
#include "mmi_app_uzone_trc.h"
#ifdef MMIUZONE_SUPPORT

#define _MMIUZONE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "guires.h"
#include "mmitheme_pos.h"
#include "mmidisplay_data.h"
#include "mmi_modu_main.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_app.h"

#ifdef MMI_AUDIO_PLAYER_SUPPORT 
#include "mmimp3_export.h"
#endif
#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif
#include "mmipicview_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#ifdef  CAMERA_SUPPORT
#include "mmidc_export.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#ifdef MMI_KING_MOVIE_SUPPORT	
#include "mmikm_export.h"
#endif
#ifdef EBOOK_SUPPORT
//#include "mmiebook.h"
#include "mmiebook_export.h"
#endif
#include "mmiuzone_position.h"
#include "mmiuzone_internal.h"
#include "mmiuzone_image.h"
#include "mmiuzone_text.h"
#include "mmiuzone_display.h"
#include "mmiuzone_nv.h"
#include "mmiuzone.h"
#include "mmiuzone_position.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : the callback function of MP3 module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneMp3Cb(void);


 /*****************************************************************************/
//  Description : the callback function of FM module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneFMCb(void);


 /*****************************************************************************/
//  Description : the callback function of ImageViewer module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneImageViewerCb(void);


 /*****************************************************************************/
//  Description : the callback function of VideoPlayer module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneVPCb(void);
 

 /*****************************************************************************/
//  Description : the callback function of ATV module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneATVCb(void);


 /*****************************************************************************/
//  Description : the callback function of Camera module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneCameraCb(void);


 /*****************************************************************************/
//  Description : the callback function of Record module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneRecordCb(void);


 /*****************************************************************************/
//  Description : the callback function of KingMovie module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneKMCb(void);


 /*****************************************************************************/
//  Description : the callback function of Ebook module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneEbookCb(void);

/*****************************************************************************/
// Description : init uzone items
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
 LOCAL void InitUzoneItems(MMI_WIN_ID_T win_id);


/*****************************************************************************/
// Description : init uzone bg
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void InitUzoneBG(void);


/*****************************************************************************/
// Description : init uzone each item
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void InitUzoneEachItem(MMIUZONE_ITEM_T* item_ptr, MMI_WIN_ID_T win_id);



/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIUZONE_RUNNING_INFO* s_uzone_running_info_ptr = PNULL;

LOCAL MMIUZONE_ITEM_RES_T s_uzone_item_res[] = 
{
#ifdef MMIUZONE_F_MP3    
    {STXT_UZONE_MP3, IMG_UZONE_ICON_MP3, IMG_UZONE_GLASS, UzoneMp3Cb}, //mp3播放器
#endif
#ifdef MMIUZONE_F_FM
    {STXT_UZONE_FM, IMG_UZONE_ICON_FM, IMG_UZONE_GLASS, UzoneFMCb}, //收音机
#endif
#ifdef MMIUZONE_F_IMAGEVIEWER
    {TXT_COMMON_PIC, IMG_UZONE_ICON_IMAGEVIEWER, IMG_UZONE_GLASS, UzoneImageViewerCb}, //图片浏览器
#endif
#ifdef MMIUZONE_F_VIDEOPLAYER
    {TXT_COMMON_VIDEOPLAYER, IMG_UZONE_ICON_VIDEOPLAYER, IMG_UZONE_GLASS, UzoneVPCb}, //视频播放器
#endif
#ifdef MMIUZONE_F_ATV
    {STXT_UZONE_ATV, IMG_UZONE_ICON_ATV, IMG_UZONE_GLASS, UzoneATVCb}, //ATV电视
#endif
#ifdef MMIUZONE_F_CAMERA
    {TXT_ENTERTAIMENT_CAMERA, IMG_UZONE_ICON_CAMERA, IMG_UZONE_GLASS, UzoneCameraCb}, //相机
#endif
#ifdef MMIUZONE_F_RECORD
    {STXT_UZONE_RECORD, IMG_UZONE_ICON_RECORD, IMG_UZONE_GLASS, UzoneRecordCb}, //录音机
#endif
#ifdef MMIUZONE_F_KINGMOVIE
    {STXT_UZONE_KINGMOVIE, IMG_UZONE_ICON_KINGMOVIE, IMG_UZONE_GLASS, UzoneKMCb}, //电影王
#endif
#ifdef MMIUZONE_F_EBOOK
    {TXT_COMM_EBOOK, IMG_UZONE_ICON_EBOOK, IMG_UZONE_GLASS, UzoneEbookCb}, //电子书
#endif    
};


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : the callback function of MP3 module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneMp3Cb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneMp3Cb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_209_112_2_18_3_4_20_0,(uint8*)"");
#ifdef MMI_AUDIO_PLAYER_SUPPORT 
    MMIAPIMP3_OpenMp3Player();
#endif
}


 /*****************************************************************************/
//  Description : the callback function of FM module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneFMCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneFMCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_223_112_2_18_3_4_20_1,(uint8*)"");
#ifdef FM_SUPPORT
    MMIAPIFM_OpenMainWin();
#endif
}


 /*****************************************************************************/
//  Description : the callback function of ImageViewer module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneImageViewerCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneImageViewerCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_237_112_2_18_3_4_20_2,(uint8*)"");
    MMIAPIPICVIEW_OpenPicViewer();
}


 /*****************************************************************************/
//  Description : the callback function of VideoPlayer module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneVPCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneVPCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_249_112_2_18_3_4_20_3,(uint8*)"");
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
    MMIAPIVP_MainEntry();
#endif
#endif
}
 

 /*****************************************************************************/
//  Description : the callback function of ATV module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneATVCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneATVCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_263_112_2_18_3_4_20_4,(uint8*)"");
#ifdef ATV_SUPPORT
    MMIAPIATV_OpenATV();
#endif
}


 /*****************************************************************************/
//  Description : the callback function of Camera module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneCameraCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneCameraCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_277_112_2_18_3_4_20_5,(uint8*)"");
#ifdef  CAMERA_SUPPORT
    MMIAPIDC_OpenPhotoWin(); 
#endif
}


 /*****************************************************************************/
//  Description : the callback function of Record module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneRecordCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneRecordCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_291_112_2_18_3_4_20_6,(uint8*)"");
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_OpenMainWin(PNULL);
#endif
}


 /*****************************************************************************/
//  Description : the callback function of KingMovie module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneKMCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneKMCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_303_112_2_18_3_4_20_7,(uint8*)"");
#ifdef MMI_KING_MOVIE_SUPPORT	
    MMIAPIKM_MainEntry();
#endif
}


 /*****************************************************************************/
//  Description : the callback function of Ebook module
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UzoneEbookCb(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] UzoneEbookCb"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_317_112_2_18_3_4_20_8,(uint8*)"");
#ifdef EBOOK_SUPPORT
    if ( MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_EBOOK) )
    {
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_EBOOK,MMIAPIEBOOK_ValidatePrivacyAppEntry);
    }
    else
    {
        MMIAPIEBOOK_ManagerWinMsg(TRUE);
    }
#endif
}

/*****************************************************************************/
// Description : init uzone each item
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void InitUzoneEachItem(MMIUZONE_ITEM_T* item_ptr, MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect = {0};
    uint16 width = 0,height = 0;
    
    
    if(NULL == s_uzone_running_info_ptr || NULL == s_uzone_running_info_ptr->item_ptr 
        || NULL == item_ptr)
    {
        return;
    }


    //fill bg_rect.h_rect
    rect.top = 0;
    rect.left = 0;
    rect.right = rect.left + s_uzone_running_info_ptr->item_size.width - 1;
    rect.bottom = rect.top + s_uzone_running_info_ptr->item_size.height - 1;
    item_ptr->item_info.bg_rect = rect;

    //fill icon_rect
    rect.top = rect.top + UZONE_ITEM_ICON_POS_Y;
    rect.left = rect.left + UZONE_ITEM_ICON_POS_X;
    //GUIRES_GetImgWidthHeight(&width, &height, item_ptr->item_info.item_res.icon_id, win_id);
    width = UZONE_ITEM_ICON_WIDTH;
    height = UZONE_ITEM_ICON_HEIGHT;
    
    rect.right = rect.left+ width - 1;
    rect.bottom = rect.top + height - 1;
    item_ptr->item_info.icon_rect = rect;

    //fill str_rect
    //MMI_GetLabelTextByLang(item_ptr->item_info.item_res.text_id, &text_str);
    //width = GUISTR_GetStringWidth(&str_style, &text_str, str_state);
    //height = GUI_GetStringHeight(MMIUZONE_TEXT_FONT,text_str.wstr_ptr,text_str.wstr_len);
    width = UZONE_ITEM_STR_WIDTH;
    height = UZONE_ITEM_STR_HEIGHT;
    
    rect = item_ptr->item_info.bg_rect;
    rect.top += UZONE_ITEM_STR_POS_Y;
    rect.bottom = rect.top + height - 1;
    rect.left += UZONE_ITEM_STR_POS_X;
    rect.right = rect.left + width - 1;
    item_ptr->item_info.str_rect = rect;

}


/*****************************************************************************/
// Description : init uzone items
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void InitUzoneItems(MMI_WIN_ID_T win_id)
{
    MMIUZONE_ITEM_T* item_ptr = PNULL;

    if(NULL == s_uzone_running_info_ptr || NULL == s_uzone_running_info_ptr->item_ptr)
    {
        return;
    }

    SCI_MEMSET(s_uzone_running_info_ptr->item_ptr, 0, sizeof(MMIUZONE_ITEM_T)*MMIUZONE_ITEM_ID_MAX);

    s_uzone_running_info_ptr->item_num = 0;
    
    while(s_uzone_running_info_ptr->item_num < MMIUZONE_ITEM_ID_MAX)
    {
        item_ptr = s_uzone_running_info_ptr->item_ptr + s_uzone_running_info_ptr->item_num;
        item_ptr->item_info.item_id = (MMIUZONE_ITEM_ID_E)s_uzone_running_info_ptr->item_num;
        item_ptr->item_info.item_res = s_uzone_item_res[s_uzone_running_info_ptr->item_num];

        InitUzoneEachItem(item_ptr, win_id);
        
        s_uzone_running_info_ptr->item_num++;
    }       
}

/*****************************************************************************/
// Description : init uzone bg
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void InitUzoneBG()
{
    if(NULL == s_uzone_running_info_ptr)
    {
        return;
    }

    s_uzone_running_info_ptr->bg_info.bg_id = IMG_UZONE_GLASS_BG;

    s_uzone_running_info_ptr->bg_info.bg_both_rect = MMITHEME_GetFullScreenBothRect();
}


/*****************************************************************************/
// Description : get the string state
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC GUISTR_STATE_T MMIUZONE_GetStringState(void)
{
    GUISTR_STATE_T   state = GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_SINGLE_LINE;

    return state;
}

/*****************************************************************************/
// Description : get the string style
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC GUISTR_STYLE_T MMIUZONE_GetStringStyle(void)
{
    GUISTR_STYLE_T   style = {0};

    style.align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    style.char_space = 0;
    style.line_space = 0;
    style.effect = FONT_EFFECT_NONE;
    style.font = MMIUZONE_TEXT_FONT;
    style.font_color = MMIUZONE_TEXT_COLOR;
    style.edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
    style.bg_color = MMI_BLACK_COLOR;
    style.region_num = 0;
    style.region_ptr = PNULL;

    return style;
}


 /*****************************************************************************/
//  Description : release uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ReleaseLayers()
{
    //SCI_TRACE_LOW:"[MMIUZONE]MMIUZONE_ReleaseLayers"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_485_112_2_18_3_4_20_9,(uint8*)"");

    if(NULL == s_uzone_running_info_ptr)
    {
        return;
    }

    //UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->bglayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->fglayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->icon_srclayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->str_srclayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->page_srclayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->str_shadow_dev_info); /*lint !e506 !e774*/
    

    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->bigstarlayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->smallstarlayer_dev_info); /*lint !e506 !e774*/
    UILAYER_RELEASELAYER(&s_uzone_running_info_ptr->focusitemlayer_dev_info); /*lint !e506 !e774*/

}


 /*****************************************************************************/
//  Description : create uzone layers
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
 PUBLIC BOOLEAN MMIUZONE_CreateLayers(void)
{
    UILAYER_CREATE_T create_info = {0};
    GUI_RECT_T rect = {0};
    UILAYER_RESULT_E result =  UILAYER_RESULT_ERROR ;
    int16 height = 0;

    //SCI_TRACE_LOW:"[MMIUZONE]MMIUZONE_CreateLayers"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_510_112_2_18_3_4_20_10,(uint8*)"");

    if(NULL == s_uzone_running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE]MMIUZONE_CreateLayers - Invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_514_112_2_18_3_4_20_11,(uint8*)"");
        return FALSE;
    }

    MMIUZONE_ReleaseLayers();

    s_uzone_running_info_ptr->bglayer_dev_info.lcd_id = 0;
    s_uzone_running_info_ptr->bglayer_dev_info.block_id = 0;
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = s_uzone_running_info_ptr->main_win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    if(s_uzone_running_info_ptr->is_landscape)
    {
        rect = s_uzone_running_info_ptr->bg_info.bg_both_rect.h_rect;
    }
    else
    {
        rect = s_uzone_running_info_ptr->bg_info.bg_both_rect.v_rect;
    }
    create_info.width = rect.right - rect.left + 1;
    create_info.height = rect.bottom - rect.top + 1;


    //create fg layer
    create_info.is_bg_layer = FALSE;    
    create_info.is_static_layer = FALSE;


    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->fglayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        //SCI_TRACE_LOW:"[MMIUZONE]MMIUZONE_CreateLayers - create fg layer fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_551_112_2_18_3_4_20_13,(uint8*)"");
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }
    
    UILAYER_Clear(&s_uzone_running_info_ptr->fglayer_dev_info);


    //create src layer
    //创建icon src layer
    create_info.width= UZONE_ITEM_ICON_WIDTH;
    create_info.height = MMIUZONE_ITEM_ID_MAX *UZONE_ITEM_ICON_HEIGHT;
    
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->icon_srclayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }

    UILAYER_Clear(&s_uzone_running_info_ptr->icon_srclayer_dev_info);

    //创建str src layer
    create_info.width= UZONE_ITEM_STR_WIDTH;
    create_info.height = MMIUZONE_ITEM_ID_MAX * UZONE_ITEM_STR_HEIGHT;
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->str_srclayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }

    UILAYER_Clear(&s_uzone_running_info_ptr->str_srclayer_dev_info); 
    
    //创建str shadow layer
    create_info.width= UZONE_ITEM_STR_WIDTH;
    create_info.height = MMIUZONE_ITEM_ID_MAX * UZONE_ITEM_STR_HEIGHT;
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->str_shadow_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }

    UILAYER_Clear(&s_uzone_running_info_ptr->str_shadow_dev_info); 

    //创建page src layer
    height = s_uzone_running_info_ptr->item_size.height + s_uzone_running_info_ptr->item_size.shadow_height;
    create_info.width= s_uzone_running_info_ptr->item_size.width;
    create_info.height = height;

    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->page_srclayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }

    UILAYER_Clear(&s_uzone_running_info_ptr->page_srclayer_dev_info); 


    create_info.width= s_uzone_running_info_ptr->item_size.width;
    create_info.height = s_uzone_running_info_ptr->item_size.height;
    
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->focusitemlayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        SCI_TRACE_LOW("[MMIUZONE]MMIUZONE_CreateLayers - create focus item layer fail");
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }
    
    UILAYER_Clear(&s_uzone_running_info_ptr->focusitemlayer_dev_info);

    create_info.width = s_uzone_running_info_ptr->big_star_size.width;
    create_info.height = s_uzone_running_info_ptr->big_star_size.height * UZONE_STAR_NUMBER;
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->bigstarlayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        SCI_TRACE_LOW("[MMIUZONE]MMIUZONE_CreateLayers - create big star layer fail");
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }
    
    UILAYER_Clear(&s_uzone_running_info_ptr->bigstarlayer_dev_info);

    create_info.width = s_uzone_running_info_ptr->small_star_size.width;
    create_info.height = s_uzone_running_info_ptr->small_star_size.height * UZONE_STAR_NUMBER;
    result = UILAYER_CreateLayer(&create_info, &s_uzone_running_info_ptr->smallstarlayer_dev_info);
    if(UILAYER_RESULT_SUCCESS != result)
    {
        SCI_TRACE_LOW("[MMIUZONE]MMIUZONE_CreateLayers - create small star layer fail");
        MMIUZONE_ReleaseLayers();
        return FALSE;
    }
    
    UILAYER_Clear(&s_uzone_running_info_ptr->smallstarlayer_dev_info);



    return TRUE;
}


 
 /*****************************************************************************/
//  Description : init uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Init(MMI_WIN_ID_T win_id)
{

    //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Init enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_596_112_2_18_3_4_20_15,(uint8*)"");

    if(NULL == s_uzone_running_info_ptr || NULL == s_uzone_running_info_ptr->item_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Init return FALSE for invalid buffer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_600_112_2_18_3_4_20_16,(uint8*)"");
        return FALSE;
    }
    
    s_uzone_running_info_ptr->main_win_id = win_id;

    s_uzone_running_info_ptr->item_size.width = UZONE_ITEM_BG_WIDTH;
    s_uzone_running_info_ptr->item_size.height = UZONE_ITEM_BG_HEIGHT;
    s_uzone_running_info_ptr->item_size.shadow_height = UZONE_ITEM_SHADOW_HEIGHT_H;
    
    s_uzone_running_info_ptr->big_star_size.height = UZONE_BIG_STAR_HEIGHT;
    s_uzone_running_info_ptr->big_star_size.width = UZONE_BIG_STAR_WIDTH;
    
    s_uzone_running_info_ptr->small_star_size.height = UZONE_SMALL_STAR_HEIGHT;
    s_uzone_running_info_ptr->small_star_size.width = UZONE_SMALL_STAR_WIDTH;
  
    InitUzoneBG();
    
#if defined(MMI_PDA_SUPPORT)
    s_uzone_running_info_ptr->is_landscape = TRUE;
#else
	 s_uzone_running_info_ptr->is_landscape = FALSE;
#endif

    if(!MMIUZONE_CreateLayers())
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Init - create layer fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_620_112_2_18_3_4_20_17,(uint8*)"");
        return FALSE;
    }

    InitUzoneItems(win_id);
    SCI_TRACE_LOW("[MMIUZONE] MMIUZONE_Init - init %d items", s_uzone_running_info_ptr->item_num);

    return TRUE;
}

 /*****************************************************************************/
//  Description : destroy uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_Destroy(void)
{
    //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Destroy enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_636_112_2_18_3_4_20_18,(uint8*)"");
    if(NULL != s_uzone_running_info_ptr)
    {
        if(NULL != s_uzone_running_info_ptr->item_ptr)
        {
            SCI_FREE(s_uzone_running_info_ptr->item_ptr);
        }
        MMIUZONE_ReleaseLayers();
        SCI_FREE(s_uzone_running_info_ptr);
        s_uzone_running_info_ptr = NULL;
    }
}

 /*****************************************************************************/
//  Description : create uzone system
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Create(MMI_HANDLE_T applet_handle)
{
    //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_Create enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_656_112_2_18_3_4_21_19,(uint8*)"");
    
    SCI_ASSERT(5 <= MMIUZONE_ITEM_ID_MAX); /*assert verified*/
    
    if(NULL != s_uzone_running_info_ptr)
    {
        MMIUZONE_Destroy();
    }
    
    s_uzone_running_info_ptr = (MMIUZONE_RUNNING_INFO*)SCI_ALLOCA(sizeof(MMIUZONE_RUNNING_INFO));
    if(NULL == s_uzone_running_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_668_112_2_18_3_4_21_20,(uint8*)"d",sizeof(MMIUZONE_RUNNING_INFO));
        return FALSE;
    }
    SCI_MEMSET(s_uzone_running_info_ptr, 0, sizeof(MMIUZONE_RUNNING_INFO));


    s_uzone_running_info_ptr->item_ptr = SCI_ALLOCA(sizeof(MMIUZONE_ITEM_T)*MMIUZONE_ITEM_ID_MAX);
    if(NULL == s_uzone_running_info_ptr->item_ptr)
    {
        //SCI_TRACE_LOW:"[MMIUZONE] create uzone fail! Can't malloc memory (size: %d)!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_677_112_2_18_3_4_21_21,(uint8*)"d", sizeof(MMIUZONE_ITEM_T)*MMIUZONE_ITEM_ID_MAX);
        SCI_FREE(s_uzone_running_info_ptr);
        return FALSE;
    }
    SCI_MEMSET(s_uzone_running_info_ptr->item_ptr, 0, sizeof(MMIUZONE_ITEM_T)*MMIUZONE_ITEM_ID_MAX);

    s_uzone_running_info_ptr->applet_handle = applet_handle;

    return TRUE;    
}


/*****************************************************************************/
//  Description : callback function of item click
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_ItemAction(uint16 item_index)
{
    MMIUZONE_ITEM_T* item_ptr = PNULL;

    //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_ItemAction (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_697_112_2_18_3_4_21_22,(uint8*)"d",item_index);

    if((PNULL == s_uzone_running_info_ptr) ||(PNULL == s_uzone_running_info_ptr->item_ptr) 
        || (item_index >= s_uzone_running_info_ptr->item_num))
    {
        //SCI_TRACE_LOW:"[MMIUZONE] MMIUZONE_ItemAction - Invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_702_112_2_18_3_4_21_23,(uint8*)"");
        return ;
    }

    item_ptr = s_uzone_running_info_ptr->item_ptr + item_index;
    item_ptr->item_info.item_res.callback();
}



/*****************************************************************************/
//  Description : get pointer to uzone running info
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIUZONE_RUNNING_INFO* MMIUZONE_GetRunningInfo(void)
{
    return s_uzone_running_info_ptr;
}

#endif //MMIUZONE_SUPPORT


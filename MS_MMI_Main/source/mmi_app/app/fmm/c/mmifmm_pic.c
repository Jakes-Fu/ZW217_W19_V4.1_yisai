/*****************************************************************************
** File Name:      mmifmm_pic.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIFMM_PIC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "mmifmm_id.h"
#include "guictrl_api.h"
#include "guilistbox.h"
#include "guisetlist.h"
#include "guires.h"
#include "mmifmm_pic.h"
#include "mmipicview_image.h"
#include "mmifmm_image.h"
#include "mmitheme_pos.h"
#include "mmidisplay_data.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

//#include "mmipicview_id.h"
#include "guibutton.h"
#include "mmipub.h"
#include "mmimultim_image.h"
#include "mmifmm_internal.h"
#include "mmifmm_menutable.h"
#include "mmifmm_mainwin.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIFMM_TITLE_STR_MAX_LEN        15  //title string max len
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
#define MMIFMM_PIC_GLIDE_PAGE_NUM       3
#define MMIFMM_PIC_GLIDE_RATIO          4   //移动距离超过lcd的1/4,则切换
#define MMIFMM_PIC_GLIDE_AUTO_FRAME     4   //松开后自动的桢数,每次移动剩余的1/2
#define MMIFMM_PIC_GLIDE_KEY_FRAME      6   //方向键切换移动的桢数
#endif
#ifdef MMI_PDA_SUPPORT
#define MMIPICVIEW_BUTTON_MARGIN_WIDTH      10
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN                   s_mmifmm_pic_is_tp_softkey = FALSE;
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
LOCAL GUI_POINT_T               s_mmifmm_pic_glide_tp_start_point = {0};
LOCAL GUI_POINT_T               s_mmifmm_pic_glide_tp_prev_point = {0};
LOCAL MMI_CTRL_ID_T             s_mmifmm_pic_glide_ctrl[MMIFMM_PIC_GLIDE_PAGE_NUM] = {0};
#endif
LOCAL MMIFMM_PIC_ADD_DATA_T     s_mmifmm_pic_add_data = {0};

#ifdef DRM_SUPPORT
LOCAL BOOLEAN		s_mmifmm_pic_is_consume_rights = FALSE;
LOCAL uint32		s_mmifmm_pic_consume_rights_index = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmPicPreviewWinMsg (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewOsd (
    uint32         cur_index,  //in:from 1 start
    uint32         total_num,  //in:
    MMI_CTRL_ID_T  ctrl_id,
    MMI_WIN_ID_T   win_id
);

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewTitleOsd (
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id,
    MMI_WIN_ID_T      win_id
);

/*****************************************************************************/
//  Description : display preview osd softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSoftkey (
    MMI_WIN_ID_T   win_id  //in:
);

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : set pre button param
//  Global resource dependence :
//  Author: Jassmine
//  Note:for cr:NEWMS00127659
/*****************************************************************************/
LOCAL void SetPreviewButtonParam (
    MMI_WIN_ID_T   win_id  //in:
);
#endif

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetAnimParam (
    BOOLEAN          is_update,  //是否立即刷新
    uint32           item_index, //list item index
    MMI_CTRL_ID_T    ctrl_id
);

/*****************************************************************************/
//  Description : anim owner draw call back function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AnimOwnerDrawCallback (
    GUIANIM_OWNER_DRAW_T   *owner_draw_ptr
);

/*****************************************************************************/
//  Description : switch the previous picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PreviousPic (
    MMI_CTRL_ID_T             ctrl_id,        //in
    MMI_CTRL_ID_T             setlist_ctrl_id,//in
    MMIFMM_PIC_ADD_DATA_T     *add_data_ptr   //in
);

/*****************************************************************************/
//  Description : get previous list index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPrevListIndex (
    uint16                   *index_ptr,     //in/out:may PNULL
    uint16                   *cur_pos_ptr,   //in/out:may PNULL
    MMIFMM_PIC_ADD_DATA_T    *add_data_ptr   //in
);

/*****************************************************************************/
//  Description : get next list index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextListIndex (
    uint16                   *index_ptr,     //in/out:may PNULL
    uint16                   *cur_pos_ptr,   //in/out:may PNULL
    MMIFMM_PIC_ADD_DATA_T    *add_data_ptr   //in
);

/*****************************************************************************/
//  Description : modify preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPreviewTitle (
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id
);

/*****************************************************************************/
//  Description : next picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NextPic (
    MMI_CTRL_ID_T             ctrl_id,        //in
    MMI_CTRL_ID_T             setlist_ctrl_id,//in
    MMIFMM_PIC_ADD_DATA_T     *add_data_ptr   //in
);
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
/*****************************************************************************/
//  Description : handle preview glide win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmPicGlideWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description : init anim param for glide,need three page
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideAnimParam (
    uint32                item_index,     //from 0
    MMI_WIN_ID_T          win_id,         //win id
    MMIFMM_PIC_ADD_DATA_T *add_data_ptr
);

/*****************************************************************************/
//  Description : init glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideCtrlId (void);

/*****************************************************************************/
//  Description : modify glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyGlideCtrlId (
    BOOLEAN    is_prev,    //in:
    BOOLEAN    is_next     //in:
);

/*****************************************************************************/
//  Description : get left glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetLeftAnimCtrlId (void);

/*****************************************************************************/
//  Description : get Middle glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetMiddleAnimCtrlId (void);

/*****************************************************************************/
//  Description : get right glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetRightAnimCtrlId (void);

/*****************************************************************************/
//  Description : set left glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimRect (
    BOOLEAN  is_update
);

/*****************************************************************************/
//  Description : set middle glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimRect (
    BOOLEAN   is_update
);


/*****************************************************************************/
//  Description : set right glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimRect (
    BOOLEAN     is_update
);
#endif
#if (defined PIC_GLIDE_SUPPORT)
/*****************************************************************************/
//  Description : set left anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimParam (
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr
);

/*****************************************************************************/
//  Description : set middle anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimParam (
    uint32           item_index, //in:from 0,middle item index
    MMI_WIN_ID_T     win_id      //in:
);

/*****************************************************************************/
//  Description : set right anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimParam (
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr
);
#endif
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
/*****************************************************************************/
//  Description : switch picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideSwitchKey (
    BOOLEAN                 is_prev,
    MMI_WIN_ID_T            win_id,         //in:
    MMI_CTRL_ID_T           setlist_ctrl_id,//in:
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr   //in:
);

/*****************************************************************************/
//  Description : sest control rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRectDisplayRect (
    int16             move_x, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
);
#endif
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpDown (
    DPARAM     param   //in:
);
#endif
//fixed by liyan.zhu for bugzilla 2760
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpOsdSoftkey (
    GUI_POINT_T    *point_ptr
);
#endif
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture glide tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpMove (
    DPARAM     param   //in:
);

/*****************************************************************************/
//  Description : handle picture glide tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpUp (
    MMI_WIN_ID_T             win_id,         //in:
    MMI_CTRL_ID_T            setlist_ctrl_id,//in:
    DPARAM                   param,          //in:
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr    //in:
);
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//picture full screen preview
WINDOW_TABLE (MMIFMM_PIC_PREVIEW_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleFmmPicPreviewWinMsg),
    WIN_ID (MMIFMM_PICTURE_PREVIEW_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_STYLE (WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL (MMIFMM_PIC_PREVIEW_CTRL_ID, MMIFMM_PICTURE_PREVIEW_WIN_ID),
    CREATE_SETLIST_CTRL (MMIFMM_PREVIEW_SETLIST_CTRL_ID),
    WIN_SOFTKEY (TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

//picture full screen preview
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
WINDOW_TABLE (MMIFMM_PIC_GLIDE_WIN_TAB) =
{
    WIN_FUNC ( (uint32) HandleFmmPicGlideWinMsg),
    WIN_ID (MMIFMM_PICTURE_PREVIEW_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_STYLE (WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL (MMIFMM_PIC_GLIDE1_CTRL_ID, MMIFMM_PICTURE_PREVIEW_WIN_ID),
    CREATE_ANIM_CTRL (MMIFMM_PIC_GLIDE2_CTRL_ID, MMIFMM_PICTURE_PREVIEW_WIN_ID),
    CREATE_ANIM_CTRL (MMIFMM_PIC_GLIDE3_CTRL_ID, MMIFMM_PICTURE_PREVIEW_WIN_ID),
    CREATE_SETLIST_CTRL (MMIFMM_PREVIEW_SETLIST_CTRL_ID),
    WIN_SOFTKEY (TXT_NULL, TXT_NULL, TXT_NULL),
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    CREATE_BUTTON_CTRL (ISTYLE_IMAGE_FMM_PIC_PREVIEW_RETURN, MMIFMM_RETURN_BUTTON_CTRL_ID),//NEWMS00189732
    CREATE_BUTTON_CTRL (ISTYLE_IMAGE_FMM_PIC_PREVIEW_OPTION, MMIFMM_OPTION_BUTTON_CTRL_ID),////NEWMS00189732
#endif
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : return true or false to sign whther the pic is supported with Pic index data
//  Global resource dependence :
//  Author: Liyan.zhu
//  Note:CR NEWMS00210875,CR NEWMS00217600
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsPicSupportFromPath (wchar   *full_path_name, uint16 len)
{
    wchar	suffix_name[MMIFMM_TITLE_STR_MAX_LEN]	= {0};
    uint16       suffix_len = MMIFMM_TITLE_STR_MAX_LEN;
    BOOL isSupport = FALSE;

    if (MMIAPIFMM_SplitFileName (full_path_name, len,
                                 PNULL, PNULL,
                                 suffix_name, &suffix_len))
    {

        switch (MMIAPIFMM_GetFileType (suffix_name, suffix_len))
        {
        case MMIFMM_PICTURE_JPG:
#ifdef JPG_DEC_SUPPORT
            isSupport = TRUE;
#endif
            break;
        case MMIFMM_PICTURE_GIF:
#ifdef GIF_DEC_SUPPORT
            isSupport = TRUE;
#endif
            break;
        case MMIFMM_PICTURE_BMP:
        case MMIFMM_PICTURE_WBMP:
#ifdef BMP_DEC_SUPPORT
            isSupport = TRUE;
#endif
            break;
        case MMIFMM_PICTURE_PNG:
#ifdef PNG_DEC_SUPPORT
            isSupport = TRUE;
#endif
            break;

#ifdef DRM_SUPPORT
        case MMIFMM_DM_FILE:
        case MMIFMM_DCF_FILE:
            isSupport = TRUE;
            break;
#endif

        default:
            break;
        }
    }

    return isSupport;
}
/*****************************************************************************/
//  Description : preview image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_PicPreview (
    MMI_CTRL_ID_T                 list_ctrl_id,
    MMIFMM_PICTURE_INDEX_DATA_T   *pic_data_ptr
)
{
    //set add data
    s_mmifmm_pic_add_data.list_ctrl_id = list_ctrl_id;
    s_mmifmm_pic_add_data.pic_data_ptr = pic_data_ptr;

#ifdef PIC_GLIDE_SUPPORT
        MMK_CreateWin ( (uint32 *) MMIFMM_PIC_GLIDE_WIN_TAB, PNULL);
#else
        MMK_CreateWin ( (uint32 *) MMIFMM_PIC_PREVIEW_WIN_TAB, PNULL);
#endif

}

/*****************************************************************************/
//  Description : handle preview win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmPicPreviewWinMsg (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    BOOLEAN                 is_next = FALSE;
    uint16                  item_index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIFMM_PIC_PREVIEW_CTRL_ID;
    MMI_CTRL_ID_T           setlist_ctrl_id = MMIFMM_PREVIEW_SETLIST_CTRL_ID;
    MMI_CTRL_ID_T           list_ctrl_id = s_mmifmm_pic_add_data.list_ctrl_id;
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetFullScreenBothRect();
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr = &s_mmifmm_pic_add_data;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef DRM_SUPPORT
        s_mmifmm_pic_is_consume_rights = FALSE;
        s_mmifmm_pic_consume_rights_index = 0;
#endif
        both_rect.h_rect.bottom = both_rect.h_rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
		both_rect.v_rect.bottom = both_rect.v_rect.bottom - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;
     
        GUIAPICTRL_SetBothRect (ctrl_id, &both_rect);

        //get item index
        item_index = GUILIST_GetCurItemIndex (list_ctrl_id);

        //set title and softkey
        SetPreviewOsd ( (add_data_ptr->pic_data_ptr->current_pos + 1),
                        add_data_ptr->pic_data_ptr->total_num,
                        setlist_ctrl_id,
                        win_id);

        //set anim param
        SetAnimParam (FALSE, item_index, ctrl_id);

        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_APP_MENU:
        //MMK_CreateWin((uint32 *)MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_TAB,(ADD_DATA)list_ctrl_id);
        MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_PICTURE_PREVIEW_OPT, MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID, MMIFMM_PIC_PREVIEW_OPT_MENU_CTRL_ID);

        break;

    case MSG_APP_UP:
    case MSG_APP_LEFT:
        //previous picture
        PreviousPic (ctrl_id, setlist_ctrl_id, add_data_ptr);
        break;

    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        //next picture
        NextPic (ctrl_id, setlist_ctrl_id, add_data_ptr);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;

    case MSG_CTL_SETLIST_SWITCH:
        is_next = * (BOOLEAN*) param;

        if (is_next)
        {
            //next picture
            NextPic (ctrl_id, setlist_ctrl_id, add_data_ptr);
        }
        else
        {
            //previous picture
            PreviousPic (ctrl_id, setlist_ctrl_id, add_data_ptr);
        }

        break;

    case MSG_NOTIFY_UPDATE:
        GUIANIM_UpdateRect (* (GUI_RECT_T *) param, ctrl_id);
        break;

    case MSG_CLOSE_WINDOW:
        {
            MMIFMM_PICTURE_INDEX_DATA_T  **  pic_data_ptr = MMIFMM_GetPicIndexData();

            if (PNULL != *pic_data_ptr)
            {
                SCI_FREE (*pic_data_ptr);
                *pic_data_ptr = PNULL;
            }
        }
        break;


    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewOsd (
    uint32         cur_index,  //in:from 1 start
    uint32         total_num,  //in:
    MMI_CTRL_ID_T  ctrl_id,
    MMI_WIN_ID_T   win_id
)
{
    //set title
    SetPreviewTitleOsd (cur_index, total_num, ctrl_id, win_id);

    //set softkey
    SetPreviewSoftkey (win_id);

#ifdef MMI_PDA_SUPPORT
#ifdef MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        SetPreviewButtonParam (win_id);
    }
#endif
#endif
}

/*****************************************************************************/
//  Description : set anim preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewTitleOsd (
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id,
    MMI_WIN_ID_T      win_id
)
{
    BOOLEAN                 is_need_arrow = FALSE;
#ifndef CMCC_UI_STYLE
    uint8                   str[MMIFMM_TITLE_STR_MAX_LEN] = {0};
    wchar                   wstr[MMIFMM_TITLE_STR_MAX_LEN] = {0};
    uint16                  str_len = 0;
#endif
    uint16                  title_height = 0;
    uint16                  lcd_width = 0;
    uint16                  lcd_height = 0;
    GUI_BG_T                bg = {0};
    GUI_BORDER_T            border = {0};
    GUI_FONT_ALL_T          font = {0};
    GUI_BOTH_RECT_T         both_rect = {0};
    GUISETLIST_ITEM_INFO_T  item_info = {0};

    //get title height
    GUIRES_GetImgWidthHeight (PNULL, &title_height,
                              IMAGE_LIST_TITLE_ARROW_LEFT_DOWN,
                              win_id);

    title_height = (uint16) MAX (title_height, GUIFONT_GetHeight (SONG_FONT_18));/*lint !e666*/
    //get lcd width and height
    GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //screen wide mode
    if (lcd_width > lcd_height)
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)

        if (MMITHEME_IsIstyle())
        {
            both_rect.h_rect.left   = (int16) (lcd_width >> 2);
            both_rect.h_rect.right  = (int16) (lcd_width - both_rect.h_rect.left  - 1);
        }
        else
#endif
        {
            both_rect.h_rect.left   = (int16) (lcd_width >> 1);
            both_rect.h_rect.right  = (int16) (lcd_width - 1);
        }

        both_rect.h_rect.bottom = (int16) (both_rect.h_rect.top + title_height - 1);

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)

        if (MMITHEME_IsIstyle())
        {
            both_rect.v_rect.left   = (int16) (lcd_height >> 2);
            both_rect.v_rect.right  = (int16) (lcd_height - both_rect.v_rect.left - 1);
        }
        else
#endif
        {
            both_rect.v_rect.left   = (int16) (lcd_height >> 1);
            both_rect.v_rect.right  = (int16) (lcd_height - 1);
        }

        both_rect.v_rect.bottom = (int16) (both_rect.v_rect.top + title_height - 1);
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)

        if (MMITHEME_IsIstyle())
        {
            both_rect.v_rect.left   = (int16) (lcd_width >> 2);
            both_rect.v_rect.right  = (int16) (lcd_width - both_rect.v_rect.left - 1);
        }
        else
#endif
        {
#if defined MAINLCD_SIZE_128X160
            both_rect.v_rect.left   = (int16) (lcd_width >> 2);
#else
            both_rect.v_rect.left   = (int16) (lcd_width >> 1);
#endif
            both_rect.v_rect.right  = (int16) (lcd_width - 1);
        }

        both_rect.v_rect.bottom = (int16) (both_rect.v_rect.top + title_height - 1);

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)

        if (MMITHEME_IsIstyle())
        {
            both_rect.h_rect.left   = (int16) (lcd_height >> 2);
            both_rect.h_rect.right  = (int16) (lcd_height - both_rect.h_rect.left -  1);
        }
        else
#endif
        {
#if defined MAINLCD_SIZE_128X160
            both_rect.h_rect.left   = (int16) (lcd_height >> 2);
#else
            both_rect.h_rect.left   = (int16) (lcd_height >> 1);
#endif
            both_rect.h_rect.right  = (int16) (lcd_height - 1);
        }

        both_rect.h_rect.bottom = (int16) (both_rect.h_rect.top + title_height - 1);
    }

    //set setting list rect
    GUIAPICTRL_SetBothRect (ctrl_id, &both_rect);

    //set align
    GUISETLIST_SetFontAlign (ctrl_id, ALIGN_HVMIDDLE);

    //set arrow
#if (!defined TOUCHPANEL_TYPE_NONE) &&(!defined PIC_GLIDE_SUPPORT)
    GUISETLIST_SetArrow (ctrl_id,
                         IMAGE_TAB_LEFT_PRESSED,
                         IMAGE_TAB_LEFT_RELEASED,
                         IMAGE_TAB_RIGHT_PRESSED,
                         IMAGE_TAB_RIGHT_RELEASED);
#endif

    GUISETLIST_SetStrEffect (ctrl_id, FONT_EFFECT_REVERSE_EDGE);

#ifndef CMCC_UI_STYLE
    //set string
    sprintf ( (char *) str, "%ld/%ld", cur_index, total_num); /*lint !e64*/
    str_len = (uint16) strlen ( (char *) str);
    MMI_STRNTOWSTR (wstr, MMIFMM_TITLE_STR_MAX_LEN,
                    str, MMIFMM_TITLE_STR_MAX_LEN,
                    str_len);

    //set title
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = wstr;
    item_info.str_info.wstr_len = str_len;
    GUISETLIST_AddItem (ctrl_id, &item_info);
#endif
    if (1 < total_num)
    {
        is_need_arrow = TRUE;
        GUISETLIST_AddItem (ctrl_id, &item_info);
    }


#if (!defined TOUCHPANEL_TYPE_NONE) &&(!defined PIC_GLIDE_SUPPORT)
    //need arrow
    GUISETLIST_SetNeedArrow (ctrl_id, is_need_arrow);
#endif

    //set font
    font.color = MMI_DEFAULT_TEXT_COLOR;
    font.font  = SONG_FONT_18;
    GUISETLIST_SetFont (ctrl_id, &font);

    //no bg
    bg.bg_type = GUI_BG_NONE;
    GUISETLIST_SetBackGround (ctrl_id, &bg);

    //no border
    border.type = GUI_BORDER_NONE;
    GUISETLIST_SetBorder (ctrl_id, &border);

    //circular handle left/right
    GUISETLIST_SetCircular (ctrl_id, TRUE);

#ifdef PIC_GLIDE_SUPPORT
    //not active,not tp
    GUIAPICTRL_SetState (ctrl_id, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
#else
    //not active
    GUIAPICTRL_SetState (ctrl_id, GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
#endif
}

/*****************************************************************************/
//  Description : display preview osd softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetPreviewSoftkey (
    MMI_WIN_ID_T   win_id  //in:
)
{
    GUI_BG_T    common_bg = {0};

    //set common bg type
    common_bg.bg_type = GUI_BG_NONE;
    GUIWIN_SetSoftkeyCommonBg (win_id, &common_bg);
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    //set left image
    GUIWIN_SeSoftkeytButtonIconId (win_id,
                                   ISTYLE_IMAGE_FMM_PIC_PREVIEW_OPTION,
                                   0,
                                   FALSE);

    //set right softkey
    GUIWIN_SeSoftkeytButtonIconId (win_id,
                                   ISTYLE_IMAGE_FMM_PIC_PREVIEW_RETURN,
                                   2,
                                   FALSE);
#else
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
	GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
	GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);    
#endif
#endif
}

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
/*****************************************************************************/
//  Description : set pre button param
//  Global resource dependence :
//  Author: Jassmine
//  Note:for cr:NEWMS00127659
/*****************************************************************************/
LOCAL void SetPreviewButtonParam (
    MMI_WIN_ID_T   win_id  //in:
)
{
    GUI_RECT_T option_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T return_rect = MMITHEME_GetFullScreenRect();
    uint16 image_height = 0;
    uint16 image_width = 0;

    GUIRES_GetImgWidthHeight (&image_width, &image_height, ISTYLE_IMAGE_FMM_PIC_PREVIEW_RETURN, win_id);//NEWMS00189732

    return_rect.left += MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    return_rect.right = return_rect.left + image_width;
    //return_rect.top = return_rect.top + MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    return_rect.bottom = return_rect.top + image_height;

    GUIBUTTON_SetRect (MMIFMM_RETURN_BUTTON_CTRL_ID, &return_rect);

    GUIRES_GetImgWidthHeight (&image_width, &image_height, ISTYLE_IMAGE_FMM_PIC_PREVIEW_OPTION, win_id);//NEWMS00189732

    option_rect.right = option_rect.right - MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    option_rect.left = option_rect.right - image_width;
    //option_rect.top = option_rect.top + MMIPICVIEW_BUTTON_MARGIN_WIDTH;
    option_rect.bottom = option_rect.top + image_height;

    GUIBUTTON_SetRect (MMIFMM_OPTION_BUTTON_CTRL_ID, &option_rect);
}
#endif

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetAnimParam (
    BOOLEAN          is_update,  //是否立即刷新
    uint32           item_index, //list item index
    MMI_CTRL_ID_T    ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

#ifdef DRM_SUPPORT

    if ( (s_mmifmm_pic_is_consume_rights) &&
            (s_mmifmm_pic_consume_rights_index != item_index))
    {
#ifdef PIC_GLIDE_SUPPORT

        if (ctrl_id == GetMiddleAnimCtrlId())
#endif
        {
            s_mmifmm_pic_is_consume_rights = FALSE;
        }
    }

#endif

#ifdef CMCC_UI_STYLE

    if (!MMITHEME_IsSupportGsensor())
    {
        //set anim auto adapt display
        GUIANIM_SetAutoAdaptDisplay (TRUE, ctrl_id);
    }

#endif

    //set anim min period
    GUIANIM_SetPeriod (MMITHEME_ANIM_MIN_PERIOD, ctrl_id);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id    = ctrl_id;

    //get name
    anim_info.full_path_wstr_ptr = MMIFMM_GetPicName ( (uint16) item_index, &anim_info.full_path_wstr_len);

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = is_update;
    display_info.bg.bg_type  = GUI_BG_COLOR;
    display_info.bg.color    = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam (&control_info, PNULL, &anim_info, &display_info);

    //set call back
    call_back.param         = item_index;
    call_back.OwnerDrawFunc = AnimOwnerDrawCallback;
    GUIANIM_SetOwnerDrawEx (ctrl_id, &call_back);

    if ( (GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : anim owner draw call back function
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AnimOwnerDrawCallback (
    GUIANIM_OWNER_DRAW_T   *owner_draw_ptr
)
{
    MMI_CTRL_ID_T   setlist_ctrl_id = MMIFMM_PREVIEW_SETLIST_CTRL_ID;

#ifdef DRM_SUPPORT
    wchar			*name_ptr = PNULL;
    uint16			name_len = 0;
    uint32			item_index = 0;

    if ( (!GUI_IsRectEmpty (owner_draw_ptr->display_rect)) &&
            (!s_mmifmm_pic_is_consume_rights) &&
#ifdef PIC_GLIDE_SUPPORT
            (owner_draw_ptr->ctrl_handle == MMK_ConvertIdToHandle (GetMiddleAnimCtrlId())) &&
#endif
            (GUIANIM_IsSupport (owner_draw_ptr->ctrl_handle)))
    {
        //get item index
        item_index = (uint32) owner_draw_ptr->param;

        //get name
        name_ptr = MMIFMM_GetPicName ( (uint16) item_index, &name_len);

        if (PNULL != name_ptr)
        {
            MMIAPIDRM_ConsumeRights (SFS_INVALID_HANDLE, name_ptr);

            s_mmifmm_pic_is_consume_rights = TRUE;
            s_mmifmm_pic_consume_rights_index = item_index;
        }
    }

#endif

    //update title
    GUISETLIST_SetVisible (setlist_ctrl_id, TRUE, TRUE);

    //update softkey
    GUIWIN_UpdateSoftkey (owner_draw_ptr->win_handle);


#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)

    if (MMITHEME_IsIstyle())
    {
        GUIBUTTON_SetVisible (MMIFMM_OPTION_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible (MMIFMM_RETURN_BUTTON_CTRL_ID, TRUE, TRUE);
    }

#endif

}

/*****************************************************************************/
//  Description : switch the previous picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PreviousPic (
    MMI_CTRL_ID_T             ctrl_id,        //in
    MMI_CTRL_ID_T             setlist_ctrl_id,//in
    MMIFMM_PIC_ADD_DATA_T     *add_data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    uint16      cur_index = 0;
    uint16      cur_pos = 0;

    if (GetPrevListIndex (&cur_index, &cur_pos, add_data_ptr))
    {
        //set current pos
        add_data_ptr->pic_data_ptr->current_pos = cur_pos;

        //set list index
        GUILIST_SetCurItemIndex (add_data_ptr->list_ctrl_id, cur_index);

        //modify title
        ModifyPreviewTitle ( (add_data_ptr->pic_data_ptr->current_pos + 1),
                             add_data_ptr->pic_data_ptr->total_num,
                             setlist_ctrl_id);

        //add pic
        SetAnimParam (TRUE, cur_index, ctrl_id);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get previous list index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPrevListIndex (
    uint16                   *index_ptr,     //in/out:may PNULL
    uint16                   *cur_pos_ptr,   //in/out:may PNULL
    MMIFMM_PIC_ADD_DATA_T    *add_data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    uint16      cur_pos = 0;
    uint16      item_index = 0;

    if (1 < add_data_ptr->pic_data_ptr->total_num)
    {
        //get current position
        cur_pos = add_data_ptr->pic_data_ptr->current_pos;

        if (0 < cur_pos)
        {
            cur_pos--;
        }
        else
        {
            cur_pos = (uint16) (add_data_ptr->pic_data_ptr->total_num - 1);
        }

        //set list index
        item_index = add_data_ptr->pic_data_ptr->list_index[cur_pos];

        if (PNULL != index_ptr)
        {
            *index_ptr = item_index;
        }

        if (PNULL != cur_pos_ptr)
        {
            *cur_pos_ptr = cur_pos;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get next list index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextListIndex (
    uint16                   *index_ptr,     //in/out:may PNULL
    uint16                   *cur_pos_ptr,   //in/out:may PNULL
    MMIFMM_PIC_ADD_DATA_T    *add_data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    uint16      cur_pos = 0;
    uint16      item_index = 0;

    if (1 < add_data_ptr->pic_data_ptr->total_num)
    {
        //get current position
        cur_pos = add_data_ptr->pic_data_ptr->current_pos;

        if (cur_pos < (add_data_ptr->pic_data_ptr->total_num - 1))
        {
            cur_pos++;
        }
        else
        {
            cur_pos = 0;
        }

        //set list index
        item_index = add_data_ptr->pic_data_ptr->list_index[cur_pos];

        if (PNULL != index_ptr)
        {
            *index_ptr = item_index;
        }

        if (PNULL != cur_pos_ptr)
        {
            *cur_pos_ptr = cur_pos;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : modify preview title
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPreviewTitle (
    uint32            cur_index,  //in:from 1 start
    uint32            total_num,  //in:
    MMI_CTRL_ID_T     ctrl_id
)
{
#ifndef CMCC_UI_STYLE
    BOOLEAN                 is_need_arrow = FALSE;
    uint8                   str[MMIFMM_TITLE_STR_MAX_LEN] = {0};
    wchar                   wstr[MMIFMM_TITLE_STR_MAX_LEN] = {0};
    uint16                  str_len = 0;
    GUISETLIST_ITEM_INFO_T  item_info = {0};

    //set string

    sprintf ( (char *) str, "%ld/%ld", cur_index, total_num); /*lint !e64*/

    
    str_len = (uint16) strlen ( (char *) str);
    MMI_STRNTOWSTR (wstr, MMIFMM_TITLE_STR_MAX_LEN,
                    str, MMIFMM_TITLE_STR_MAX_LEN,
                    str_len);

    //set title
    item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
    item_info.str_info.wstr_ptr = wstr;
    item_info.str_info.wstr_len = str_len;

    GUISETLIST_ModifyItem (ctrl_id, &item_info, 0);

    if (1 < total_num)
    {
        is_need_arrow = TRUE;
        GUISETLIST_ModifyItem (ctrl_id, &item_info, 1);
    }

#if (!defined TOUCHPANEL_TYPE_NONE) &&(!defined PIC_GLIDE_SUPPORT)
    //need arrow
    GUISETLIST_SetNeedArrow (ctrl_id, is_need_arrow);
#endif
#endif
}

/*****************************************************************************/
//  Description : next picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NextPic (
    MMI_CTRL_ID_T             ctrl_id,        //in
    MMI_CTRL_ID_T             setlist_ctrl_id,//in
    MMIFMM_PIC_ADD_DATA_T     *add_data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    uint16      cur_index = 0;
    uint16      cur_pos = 0;

    if (GetNextListIndex (&cur_index, &cur_pos, add_data_ptr))
    {
        //set current pos
        add_data_ptr->pic_data_ptr->current_pos = cur_pos;

        //set list index
        GUILIST_SetCurItemIndex (add_data_ptr->list_ctrl_id, cur_index);

        //modify title
        ModifyPreviewTitle ( (add_data_ptr->pic_data_ptr->current_pos + 1),
                             add_data_ptr->pic_data_ptr->total_num,
                             setlist_ctrl_id);

        //add pic
        SetAnimParam (TRUE, cur_index, ctrl_id);

        result = TRUE;
    }

    return (result);
}
//fixed by liyan.zhu for bugzilla 2760
#ifdef PIC_GLIDE_SUPPORT
/*****************************************************************************/
//  Description : handle preview glide win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFmmPicGlideWinMsg (
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    uint16                  item_index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           setlist_ctrl_id = MMIFMM_PREVIEW_SETLIST_CTRL_ID;
    MMI_CTRL_ID_T           list_ctrl_id = s_mmifmm_pic_add_data.list_ctrl_id;
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr = &s_mmifmm_pic_add_data;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef DRM_SUPPORT
        s_mmifmm_pic_is_consume_rights = FALSE;
        s_mmifmm_pic_consume_rights_index = 0;
#endif

        //get item index
        item_index = GUILIST_GetCurItemIndex (list_ctrl_id);

        //set title and softkey
        SetPreviewOsd ( (add_data_ptr->pic_data_ptr->current_pos + 1),
                        add_data_ptr->pic_data_ptr->total_num,
                        setlist_ctrl_id,
                        win_id);

        //init glide anim param
        InitGlideAnimParam (item_index, win_id, add_data_ptr);
#ifdef MMI_PDA_SUPPORT

        if (!MMITHEME_IsIstyle())
        {
            GUIBUTTON_SetVisible (MMIFMM_OPTION_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible (MMIFMM_RETURN_BUTTON_CTRL_ID, FALSE, FALSE);
        }

#endif
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_LCD_SWITCH:
#ifdef MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            SetPreviewButtonParam (win_id);
        }
#endif
        break;
#if defined TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:

        if (PNULL != param)
        {
            if (MMIFMM_OPTION_BUTTON_CTRL_ID == ( (MMI_NOTIFY_T*) param)->src_id)
            {
                GUIBUTTON_SetVisible (MMIFMM_OPTION_BUTTON_CTRL_ID, FALSE, TRUE);
                GUIBUTTON_SetVisible (MMIFMM_RETURN_BUTTON_CTRL_ID, FALSE, TRUE);

                MMK_PostMsg (win_id, MSG_APP_MENU, param, PNULL);
            }
            else if (MMIFMM_RETURN_BUTTON_CTRL_ID == ( (MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg (win_id, MSG_APP_CANCEL, param, PNULL);
            }
        }

        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_LOSE_FOCUS:
        //reset control rect and display rect
        SetLeftAnimRect (FALSE);
        SetMidAnimRect (FALSE);
        SetRightAnimRect (FALSE);
        break;

    case MSG_APP_OK:
    case MSG_APP_MENU:
        //MMK_CreateWin((uint32 *)MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_TAB,(ADD_DATA)list_ctrl_id);
        MMIFMM_OpenMeunPubWin (win_id, MENU_FMM_PICTURE_PREVIEW_OPT, MMIFMM_PIC_PREVIEW_OPT_MENU_WIN_ID, MMIFMM_PIC_PREVIEW_OPT_MENU_CTRL_ID);
        break;

    case MSG_APP_UP:
    case MSG_APP_LEFT:
        //previous picture
        HandleGlideSwitchKey (TRUE,
                              win_id,
                              setlist_ctrl_id,
                              add_data_ptr);
        break;

    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
        //next picture
        HandleGlideSwitchKey (FALSE,
                              win_id,
                              setlist_ctrl_id,
                              add_data_ptr);
        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin (win_id);
        break;
#if defined TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        HandleGlideTpDown (param);
        break;

    case MSG_TP_PRESS_MOVE:
        HandleGlideTpMove (param);
        break;

    case MSG_TP_PRESS_UP:
        HandleGlideTpUp (win_id,
                         setlist_ctrl_id,
                         param,
                         add_data_ptr);
        break;
#endif

    case MSG_CLOSE_WINDOW:
        {
            MMIFMM_PICTURE_INDEX_DATA_T  **  pic_data_ptr = MMIFMM_GetPicIndexData();

			if (PNULL != pic_data_ptr)
			{//cr132687
				if (PNULL != *pic_data_ptr)
				{
					SCI_FREE (*pic_data_ptr);
					*pic_data_ptr = PNULL;
				}
			}
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : init anim param for glide,need three page
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideAnimParam (
    uint32                item_index,     //from 0
    MMI_WIN_ID_T          win_id,         //win id
    MMIFMM_PIC_ADD_DATA_T *add_data_ptr
)
{
    //set glide control id
    InitGlideCtrlId();

    //set middle anim
    SetMidAnimRect (FALSE);
    SetMidAnimParam (item_index, win_id);

    //set left anim
    SetLeftAnimRect (FALSE);
    SetLeftAnimParam (add_data_ptr);

    //set right anim
    SetRightAnimRect (FALSE);
    SetRightAnimParam (add_data_ptr);

}

/*****************************************************************************/
//  Description : init glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitGlideCtrlId (void)
{
    s_mmifmm_pic_glide_ctrl[0] = MMIFMM_PIC_GLIDE1_CTRL_ID;
    s_mmifmm_pic_glide_ctrl[1] = MMIFMM_PIC_GLIDE2_CTRL_ID;
    s_mmifmm_pic_glide_ctrl[2] = MMIFMM_PIC_GLIDE3_CTRL_ID;
}

/*****************************************************************************/
//  Description : modify glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyGlideCtrlId (
    BOOLEAN    is_prev,    //in:
    BOOLEAN    is_next     //in:
)
{
    uint16          i = 0;
    uint16          max_index = MMIFMM_PIC_GLIDE_PAGE_NUM - 1;
    MMI_CTRL_ID_T   ctrl_id = 0;
    MMI_CTRL_ID_T	mid_ctrl_id = 0;

    //get middle ctrl id
    mid_ctrl_id = GetMiddleAnimCtrlId();

    if (is_prev)
    {
        ctrl_id = s_mmifmm_pic_glide_ctrl[max_index];

        for (i = max_index; i > 0; i--)
        {
            s_mmifmm_pic_glide_ctrl[i] = s_mmifmm_pic_glide_ctrl[i-1];
        }

        s_mmifmm_pic_glide_ctrl[i] = ctrl_id;
    }
    else if (is_next)
    {
        ctrl_id = s_mmifmm_pic_glide_ctrl[0];

        for (i = 0; i < max_index; i++)
        {
            s_mmifmm_pic_glide_ctrl[i] = s_mmifmm_pic_glide_ctrl[i+1];
        }

        s_mmifmm_pic_glide_ctrl[i] = ctrl_id;
    }

#ifdef DRM_SUPPORT

    //get middle ctrl id
    if ( (mid_ctrl_id != GetMiddleAnimCtrlId()) &&
            (s_mmifmm_pic_is_consume_rights))
    {
        s_mmifmm_pic_is_consume_rights = FALSE;
    }

#endif
}


/*****************************************************************************/
//  Description : get left glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetLeftAnimCtrlId (void)
{
    return (s_mmifmm_pic_glide_ctrl[0]);
}

/*****************************************************************************/
//  Description : get Middle glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetMiddleAnimCtrlId (void)
{
    return (s_mmifmm_pic_glide_ctrl[1]);
}

/*****************************************************************************/
//  Description : get right glide anim control id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_CTRL_ID_T GetRightAnimCtrlId (void)
{
    return (s_mmifmm_pic_glide_ctrl[2]);
}

/*****************************************************************************/
//  Description : set left glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimRect (
    BOOLEAN  is_update
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      display_rect = {0};
    MMI_CTRL_ID_T   ctrl_id = GetLeftAnimCtrlId();
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(MMIFMM_PICTURE_PREVIEW_WIN_ID);//{0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //screen wide mode
    if (lcd_width > lcd_height)
    {
        both_rect.h_rect.right  = -1;
        both_rect.h_rect.left   = (int16) (both_rect.h_rect.right - lcd_width + 1);
//        both_rect.h_rect.bottom = (int16) (lcd_height - 1);

        both_rect.v_rect.right  = -1;
        both_rect.v_rect.left   = (int16) (both_rect.v_rect.right - lcd_height + 1);
//        both_rect.v_rect.bottom = (int16) (lcd_width - 1);
    }
    else
    {
        both_rect.v_rect.right  = -1;
        both_rect.v_rect.left   = (int16) (both_rect.v_rect.right - lcd_width + 1);
//        both_rect.v_rect.bottom = (int16) (lcd_height - 1);

        both_rect.h_rect.right  = -1;
        both_rect.h_rect.left   = (int16) (both_rect.h_rect.right - lcd_height + 1);
//        both_rect.h_rect.bottom = (int16) (lcd_width - 1);
    }

    GUIAPICTRL_SetBothRect (ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect (ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set middle glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimRect (
    BOOLEAN   is_update
)
{
    MMI_CTRL_ID_T       ctrl_id = GetMiddleAnimCtrlId();
    GUI_RECT_T          display_rect = MMITHEME_GetClientRectEx(MMIFMM_PICTURE_PREVIEW_WIN_ID);//MMITHEME_GetFullScreenRect();
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetWinClientBothRect(MMIFMM_PICTURE_PREVIEW_WIN_ID);//MMITHEME_GetFullScreenBothRect();

    GUIAPICTRL_SetBothRect (ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect (ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set right glide anim both rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimRect (
    BOOLEAN     is_update
)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    GUI_RECT_T      display_rect = {0};
    MMI_CTRL_ID_T   ctrl_id = GetRightAnimCtrlId();
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(MMIFMM_PICTURE_PREVIEW_WIN_ID);//{0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    //screen wide mode
    if (lcd_width > lcd_height)
    {
        both_rect.h_rect.left   = lcd_width;
        both_rect.h_rect.right  = (int16) (both_rect.h_rect.left + lcd_width - 1);
//        both_rect.h_rect.bottom = (int16) (lcd_height - 1);

        both_rect.v_rect.left   = lcd_height;
        both_rect.v_rect.right  = (int16) (both_rect.v_rect.left + lcd_height - 1);
//        both_rect.v_rect.bottom = (int16) (lcd_width - 1);
    }
    else
    {
        both_rect.v_rect.left   = lcd_width;
        both_rect.v_rect.right  = (int16) (both_rect.v_rect.left + lcd_width - 1);
//        both_rect.v_rect.bottom = (int16) (lcd_height - 1);

        both_rect.h_rect.left   =  lcd_height;
        both_rect.h_rect.right  = (int16) (both_rect.h_rect.left + lcd_height - 1);
//        both_rect.h_rect.bottom = (int16) (lcd_width - 1);
    }

    GUIAPICTRL_SetBothRect (ctrl_id, &both_rect);
    GUIANIM_SetDisplayRect (ctrl_id, &display_rect, is_update);
}

/*****************************************************************************/
//  Description : set left anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetLeftAnimParam (
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr
)
{
    uint16          item_index = 0;
    GUI_BG_T        error_bg = {0};
    MMI_CTRL_ID_T   ctrl_id = GetLeftAnimCtrlId();

    if (GetPrevListIndex (&item_index, PNULL, add_data_ptr))
    {
        //set left anim param
        SetAnimParam (FALSE, item_index, ctrl_id);
    }
    else
    {
        //no error bg
        error_bg.bg_type = GUI_BG_NONE;
        GUIANIM_SetErrorBg (ctrl_id, &error_bg);
    }
}

/*****************************************************************************/
//  Description : set middle anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetMidAnimParam (
    uint32           item_index, //in:from 0,middle item index
    MMI_WIN_ID_T     win_id      //in:
)
{
    MMI_CTRL_ID_T   ctrl_id = GetMiddleAnimCtrlId();

    //set middle anim param
    SetAnimParam (FALSE, item_index, ctrl_id);

    //set active
    MMK_SetAtvCtrl (win_id, ctrl_id);
}

/*****************************************************************************/
//  Description : set right anim param
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetRightAnimParam (
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr
)
{
    uint16          item_index = 0;
    GUI_BG_T        error_bg = {0};
    MMI_CTRL_ID_T   ctrl_id = GetRightAnimCtrlId();

    if (GetNextListIndex (&item_index, PNULL, add_data_ptr))
    {
        //set right anim param
        SetAnimParam (FALSE, item_index, ctrl_id);
    }
    else
    {
        //no error bg
        error_bg.bg_type = GUI_BG_NONE;
        GUIANIM_SetErrorBg (ctrl_id, &error_bg);
    }
}

/*****************************************************************************/
//  Description : switch picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideSwitchKey (
    BOOLEAN                 is_prev,
    MMI_WIN_ID_T            win_id,         //in:
    MMI_CTRL_ID_T           setlist_ctrl_id,//in:
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr   //in:
)
{
    BOOLEAN         result = FALSE;
    int16           move_space = 0;
    uint16          lcd_width = 0;
    uint16          item_index = 0;
    uint16          cur_pos = 0;
    uint16          i = 0;
    MMI_CTRL_ID_T   left_ctrl_id = GetLeftAnimCtrlId();
    MMI_CTRL_ID_T   mid_ctrl_id = GetMiddleAnimCtrlId();
    MMI_CTRL_ID_T   right_ctrl_id = GetRightAnimCtrlId();

    //get lcd width
    GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, PNULL);

    if (is_prev)
    {
        //previous
        move_space = (int16) (lcd_width);

        result = GetPrevListIndex (&item_index, &cur_pos, add_data_ptr);
    }
    else
    {
        //next
        move_space = (int16) (-lcd_width);

        result = GetNextListIndex (&item_index, &cur_pos, add_data_ptr);
    }

    //is current picture
    if (result)
    {
        //auto move
        for (i = 1; i < MMIFMM_PIC_GLIDE_KEY_FRAME; i++)
        {
            //set move space
            move_space = (int16) (move_space / 2);

            if (0 == move_space)
            {
                break;
            }
            else
            {
                //set control rect and display rect
                SetCtrlRectDisplayRect (move_space, left_ctrl_id);
                SetCtrlRectDisplayRect (move_space, right_ctrl_id);
                SetCtrlRectDisplayRect (move_space, mid_ctrl_id);

                //update
                MMITHEME_UpdateRect();
            }
        }

        //modify control id and load
        ModifyGlideCtrlId (is_prev, (BOOLEAN) (!is_prev));

        //modify title
        ModifyPreviewTitle ( (cur_pos + 1),
                             add_data_ptr->pic_data_ptr->total_num,
                             setlist_ctrl_id);

        //set current pos
        add_data_ptr->pic_data_ptr->current_pos = cur_pos;

        //set list index
        GUILIST_SetCurItemIndex (add_data_ptr->list_ctrl_id, item_index);

        //set control rect and display rect
        SetLeftAnimRect (TRUE);
        SetRightAnimRect (TRUE);
        SetMidAnimRect (TRUE);

        //update
        MMITHEME_UpdateRect();

        if (is_prev)
        {
            SetLeftAnimParam (add_data_ptr);
        }
        else
        {
            SetRightAnimParam (add_data_ptr);
        }

// 		//set left anim
// 		SetLeftAnimParam(add_data_ptr);
//
//         //set right anim
//         SetRightAnimParam(add_data_ptr);

        //set active
        MMK_SetAtvCtrl (win_id, GetMiddleAnimCtrlId());
    }
}

/*****************************************************************************/
//  Description : sest control rect and display rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetCtrlRectDisplayRect (
    int16             move_x, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
)
{
    GUI_RECT_T      ctrl_rect = {0};
    GUI_RECT_T      display_rect = {0};

    //get control rect
    GUIAPICTRL_GetRect (ctrl_id, &ctrl_rect);

    ctrl_rect.left  = (int16) (ctrl_rect.left + move_x);
    ctrl_rect.right = (int16) (ctrl_rect.right + move_x);

    //set control rect
    GUIANIM_SetCtrlRect (ctrl_id, &ctrl_rect);

    //get display rect
    display_rect = MMITHEME_GetFullScreenRect();

    if (!GUI_IntersectRect (&display_rect, ctrl_rect, display_rect))
    {
        display_rect.left   = 0;
        display_rect.top    = 0;
        display_rect.right  = 0;
        display_rect.bottom = 0;
    }

    //set display rect
    GUIANIM_SetDisplayRect (ctrl_id, &display_rect, TRUE);
}
#endif
#if defined TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpDown (
    DPARAM     param   //in:
)
{
    GUI_POINT_T     tp_point = {0};

    //get point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    //is tp softkey
    s_mmifmm_pic_is_tp_softkey = IsTpOsdSoftkey (&tp_point);

    if (!s_mmifmm_pic_is_tp_softkey)
    {
#ifdef PIC_GLIDE_SUPPORT
        //get point
        s_mmifmm_pic_glide_tp_start_point = tp_point;

        s_mmifmm_pic_glide_tp_prev_point  = s_mmifmm_pic_glide_tp_start_point;
#endif

    }
}

/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpOsdSoftkey (
    GUI_POINT_T    *point_ptr
)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  softkey_rect = {0};

    //get softkey rect
    softkey_rect = MMITHEME_GetSoftkeyRect (0);

    if ( (point_ptr->y >= softkey_rect.top) &&
            (point_ptr->y <= softkey_rect.bottom))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle picture glide tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpMove (
    DPARAM     param   //in:
)
{
#ifdef PIC_GLIDE_SUPPORT
    int16           move_x = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_CTRL_ID_T   left_ctrl_id = GetLeftAnimCtrlId();
    MMI_CTRL_ID_T   mid_ctrl_id = GetMiddleAnimCtrlId();
    MMI_CTRL_ID_T   right_ctrl_id = GetRightAnimCtrlId();

    if (!s_mmifmm_pic_is_tp_softkey)
    {
        //get point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        //set move x
        move_x = (int16) (tp_point.x - s_mmifmm_pic_glide_tp_prev_point.x);

        if (0 != move_x)
        {
            //set previous tp point
            s_mmifmm_pic_glide_tp_prev_point = tp_point;

            //sest control rect and display rect
            SetCtrlRectDisplayRect (move_x, left_ctrl_id);
            SetCtrlRectDisplayRect (move_x, right_ctrl_id);
            SetCtrlRectDisplayRect (move_x, mid_ctrl_id);
        }
    }

#endif
}

/*****************************************************************************/
//  Description : handle picture glide tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleGlideTpUp (
    MMI_WIN_ID_T             win_id,         //in:
    MMI_CTRL_ID_T            setlist_ctrl_id,//in:
    DPARAM                   param,          //in:
    MMIFMM_PIC_ADD_DATA_T   *add_data_ptr    //in:
)
{
#ifdef PIC_GLIDE_SUPPORT
    BOOLEAN         is_prev = FALSE;
    BOOLEAN         is_next = FALSE;
    int16           move_x = 0;
    int16           move_space = 0;
    uint16          lcd_width = 0;
    uint16          item_index = 0;
    uint16          cur_pos = 0;
    uint16          i = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_CTRL_ID_T   left_ctrl_id = GetLeftAnimCtrlId();
    MMI_CTRL_ID_T   mid_ctrl_id = GetMiddleAnimCtrlId();
    MMI_CTRL_ID_T   right_ctrl_id = GetRightAnimCtrlId();

    if (!s_mmifmm_pic_is_tp_softkey)
    {
        //get point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        //set move x
        move_x = (int16) (tp_point.x - s_mmifmm_pic_glide_tp_start_point.x);

        //get lcd width
        GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, PNULL);

        //is current picture
        if ( (1 == add_data_ptr->pic_data_ptr->total_num) ||
                (abs (move_x) < (lcd_width / MMIFMM_PIC_GLIDE_RATIO)))
        {
            //reset
            move_space = (int16) (s_mmifmm_pic_glide_tp_start_point.x - s_mmifmm_pic_glide_tp_prev_point.x);
        }
        else//previous or next picture
        {
            //set move space
            if (0 < move_x)
            {
                //previous
                is_prev = TRUE;
                move_space = (int16) (lcd_width + s_mmifmm_pic_glide_tp_start_point.x - s_mmifmm_pic_glide_tp_prev_point.x);

                //get liset index
                GetPrevListIndex (&item_index, &cur_pos, add_data_ptr);
            }
            else
            {
                //next
                is_next = TRUE;
                move_space = (int16) (s_mmifmm_pic_glide_tp_start_point.x - s_mmifmm_pic_glide_tp_prev_point.x - lcd_width);

                //get liset index
                GetNextListIndex (&item_index, &cur_pos, add_data_ptr);
            }
        }

        //auto move
        for (i = 1; i < MMIFMM_PIC_GLIDE_AUTO_FRAME; i++)
        {
            //set move space
            move_space = (int16) (move_space / 2);

            if (0 == move_space)
            {
                break;
            }
            else
            {
                //set control rect and display rect
                SetCtrlRectDisplayRect (move_space, left_ctrl_id);
                SetCtrlRectDisplayRect (move_space, right_ctrl_id);
                SetCtrlRectDisplayRect (move_space, mid_ctrl_id);

                //update
                MMITHEME_UpdateRect();
            }
        }

        if ( (is_prev) || (is_next))
        {
            //modify control id and load
            ModifyGlideCtrlId (is_prev, is_next);

            //modify title
            ModifyPreviewTitle ( (cur_pos + 1),
                                 add_data_ptr->pic_data_ptr->total_num,
                                 setlist_ctrl_id);

            //set current pos
            add_data_ptr->pic_data_ptr->current_pos = cur_pos;

            //set list index
            GUILIST_SetCurItemIndex (add_data_ptr->list_ctrl_id, item_index);
        }

        //set control rect and display rect
        SetLeftAnimRect (TRUE);
        SetRightAnimRect (TRUE);
        SetMidAnimRect (TRUE);

        //update
        MMITHEME_UpdateRect();

        if (is_prev)
        {
            SetLeftAnimParam (add_data_ptr);
        }
        else if (is_next)
        {
            SetRightAnimParam (add_data_ptr);
        }

//         //set left anim
//         SetLeftAnimParam(add_data_ptr);
//
//         //set right anim
//         SetRightAnimParam(add_data_ptr);
    }

    //set active
    MMK_SetAtvCtrl (win_id, GetMiddleAnimCtrlId());
#endif
}
#endif

/*Edit by script, ignore 3 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527

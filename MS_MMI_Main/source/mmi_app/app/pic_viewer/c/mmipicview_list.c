/*****************************************************************************
** File Name:      mmipicview_list.c                                         *
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

#define _MMIPICVIEW_LIST_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "window_parse.h"
#include "guilistbox.h"
#include "guiiconlist.h"
#include "guiiconfolder.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmipicview_export.h"
#include "mmipicview_internal.h"
#include "mmipicview_id.h"
#include "mmipicview_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmidc_setting.h"
#include "mmidc_save.h"
#include "mmipicview_image.h"
#include "mmipicview_position.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_image.h"
#endif
//#ifdef PIC_PREVIEW_U200_SUPPORT
#include "guires.h"
#include "mmidisplay_data.h"
#include "guiiconlist.h"
#include "guistatusbar.h"
#include "mmifilearray_export.h"

//#endif

#if defined (BAIDU_DRIVE_SUPPORT)
#include "mmibaidu_export.h"
#include "watch_commonwin_export.h"
#include "ctrlrichtext_export.h"
#include "ctrliconlist_export.h"
#include "mmiset_image.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_TITLE_COLOR      0xFEA3
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


LOCAL BOOLEAN                   s_is_reload_file = FALSE;   //是否重新加载文件
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL BOOLEAN               s_is_need_to_reinit_picarray_from_piclib   = FALSE;  //是否重新初始化pic array
#else
LOCAL BOOLEAN               s_is_need_to_reinit_picarray_from_piclib   = TRUE;  //128X160无pic_folder窗口，需要重新初始化pic array
#endif
LOCAL uint32                s_cur_folder_index_for_reinit_picarray = 0;      //当s_is_need_to_reinit_picarray_from_piclib == TRUE时有效

#ifdef PIC_PREVIEW_U200_SUPPORT
LOCAL  IMG_RES_SRC_T        res_src     = {0};
LOCAL  IMG_RES_SRC_T        targ_src    = {0};
LOCAL  IMGREF_DATA_FORMAT_E target_format   = IMGREF_FORMAT_RGB565;
#endif

LOCAL BOOLEAN  s_is_clear_pic_folder_List  = FALSE;  //是否已经释放anim_ctrl_add


LOCAL BOOLEAN                   s_is_list_empty = FALSE;   //list界面是否为空,yaye.jiang

LOCAL MMIPICVIEW_FROM_TYPE_E s_enter_from_type = MMIPICVIEW_FROM_NONE;

#ifdef BAIDU_DRIVE_SUPPORT
#define MMIPICVIEW_NUM_THUMBNAIL_PER_PAGE 4 
#define MMIPICVIEW_NUM_THUMBNAIL_PER_ROW 2
LOCAL SCI_MUTEX_PTR         s_picview_mutex_ptr    = SCI_NULL;
LOCAL uint16 s_mmipicview_save_items_marked_num = 0;
LOCAL BOOLEAN   s_mmipicview_multi_uploading_done = FALSE; 
LOCAL BOOLEAN   s_mmipicview_multi_uploading_highlight = FALSE; 
LOCAL BOOLEAN is_need_download_thumbnails = FALSE;
LOCAL uint8 s_thumbnail_down_total_count=0;
static uint16 s_download_thumbnail_index = 0;
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
LOCAL const uint8   s_mmipicview_all_suffix[MMIPICVIEW_PIC_TYPE_MAX][MMIPICVIEW_SUFFIX_MAX_LEN] =
{
#ifdef BMP_DEC_SUPPORT
    "*.bmp ",
    "*.wbmp ",
#endif
#ifdef GIF_DEC_SUPPORT
    "*.gif ",
#endif
#ifdef PNG_DEC_SUPPORT
    "*.png ",
#endif
#ifdef DRM_SUPPORT
    "*.dm ",
    "*.dcf ",
#endif
    "*.jpg ",
    "*.jpeg"
};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
//extern MMIPICVIEW_DEFAULT_PATH_T g_mmipicview_dc_path;

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void EntryPicviewListClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);//NEWMS00145771

/*****************************************************************************/
//  Description : handle pictures privacy password
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PICVIEW_ValidatePrivacyPicList(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : handle picture list win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle picture list open win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenPicListWin(
    MMI_WIN_ID_T      win_id, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
);

/*****************************************************************************/
//  Description : create pictures list control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CreatePicListCtrl(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
);

/*****************************************************************************/
//  Description : load all picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);

/*****************************************************************************/
//  Description : start load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPic(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : get all picture files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetAllPicSuffix(
    MMIFMM_FILTER_T  *filter_ptr
);

/*****************************************************************************/
//  Description : finish load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPic(
    MMI_WIN_ID_T   win_id
);

/*****************************************************************************/
//  Description : clear pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearPicList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);

/*****************************************************************************/
//  Description : update pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
);

/*****************************************************************************/
//  Description : append picture list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicListItem(
    BOOLEAN         is_text,        //in:是否添加text data
    uint16          item_index,     //in:item索引
    uint16          content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T   ctrl_id         //in:control id
);

/*****************************************************************************/
//  Description : modify pictures list view
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPicListView(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
);

/*****************************************************************************/
//  Description : modify list item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ModifyListItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
);
/*****************************************************************************/
//  Description : append picture icon list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL BOOLEAN AppendPicIconListItem(
//     uint16          index,
//     MMI_CTRL_ID_T   ctrl_id
// );

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
//  Description : handle pictures list web key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListWeb(
    MMI_CTRL_ID_T   ctrl_id
);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
// 	Description : handle pic folder win msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);
#endif
/*****************************************************************************/
// 	Description : handle pic folder win open msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenPicFolderWin(MMI_WIN_ID_T        win_id);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : camera button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CamerButtonCallBack(void);
#endif
/*****************************************************************************/
//  Description : clear pictures folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
);
/*****************************************************************************/
//  Description : load all picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicFolderWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
);
/*****************************************************************************/
//  Description : start load picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPicWithFolders(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
);

/*****************************************************************************/
//  Description : finish load picture Folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPicWithFolders(
    MMI_WIN_ID_T   win_id
);
/*****************************************************************************/
//  Description : update picture folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicFolderList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
);

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIPICVIEW_GetClientWinRect(MMI_WIN_ID_T  win_id);
#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//  Description : set title for pic list 
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPicListTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T return_id);
#endif
/*****************************************************************************/
// 	Description : set iconlist param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_folder_list);

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set spftkey param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetButtonFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_IMAGE_ID_T* img_id, uint16 button_num);

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void InitMarkMenuButtons(MMI_WIN_ID_T win_id);

#endif//PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set the iconlist groups orded in time
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetlistOrderInTime(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Discription: display when list win is empty
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
LOCAL void PicviewListEmptyShow(void);
#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
// 	Description : display the total pic number
//	Global resource dependence :
//  Author: langhua.yang
//	Note: just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DispTotalPicNum(void);
#endif

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicListWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param
                                           );

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void LoadPicList(MMI_CTRL_ID_T ctrl_id);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Discription: get default picture path
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void GetDefaultPicPath(MMIPICVIEW_DEFAULT_PATH_T* path_ptr);
#endif

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifdef  PIC_PREVIEW_U200_SUPPORT
//picture viewer list
WINDOW_TABLE(MMIPICVIEW_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicListWinMsg),
    WIN_ID(MMIPICVIEW_LIST_WIN_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_LIST_TITLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_RETURN, MMIPICVIEW_LIST_RETURN_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_LIST_SLIDE_PREVIEW),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_LIST_MARKED),
    END_WIN
};
// the window for  pic folder list window
WINDOW_TABLE(MMIPREVIEW_PICFOLDER_WIN_TAB) =
{
    WIN_ID(MMIPICVIEW_PICFOLDER_WIN_ID),
    WIN_FUNC((uint32)HandlePicFolderWinMsg),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_FLODER_TITLE_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_RETURN, MMIPICVIEW_FLODER_RETURN_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_CAMERA_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(PNULL, MMIPICVIEW_SET_BUTTON_CTRL_ID),
    CREATE_ICONLIST_CTRL(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID),
    END_WIN
};
#else
//picture viewer list
WINDOW_TABLE(MMIPICVIEW_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandlePicListWinMsg),
    WIN_ID(MMIPICVIEW_LIST_WIN_ID),
#ifdef BAIDU_DRIVE_SUPPORT
WIN_HIDE_STATUS,
    CREATE_BUTTON_CTRL(IMAGE_PICVIEW_CHANGE_FOLDER , MMIPICVIEW_SET_BUTTON_CTRL_ID),
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#else
    WIN_TITLE(TXT_PICVIEW_TITLE),
#endif
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif

#ifndef MMI_GUI_STYLE_MINISCREEN
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_LIST_TITLE_CTRL_ID),
#endif    
#ifdef MMI_GUI_STYLE_MINISCREEN
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID),
#endif
    END_WIN
};
#ifndef MMI_PICVIEW_MINI_SUPPORT
WINDOW_TABLE(MMIPREVIEW_PICFOLDER_WIN_TAB) =
{
    WIN_ID(MMIPICVIEW_PICFOLDER_WIN_ID),
    WIN_FUNC((uint32)HandlePicFolderWinMsg),
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,  
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_COMMON_OK, TXT_COMMON_OK),
#else
    WIN_TITLE(TXT_PICVIEW_TITLE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIPICVIEW_FLODER_TITLE_CTRL_ID),
#endif
    CREATE_ICONLIST_CTRL(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID),
    END_WIN
};
#endif
#endif //PIC_PREVIEW_U200_SUPPORT


LOCAL void EntryPicviewListClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)//NEWMS00145771
{
	MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;
    //get view type
	view_type = MMIPICVIEW_GetViewType();
	s_is_clear_pic_folder_List=TRUE;
	
    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_428_112_2_18_2_46_21_7,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        //GUILIST_SetCurItemIndex(ctrl_id, cur_folder_index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
       // GUIICONLIST_SetCurIconIndex(cur_folder_index,  ctrl_id);
        break;
    }

}

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
// 	Description : handle pic folder win msg
//	Global resource dependence :
//   Author: kevin.lou
//	Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E        result      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     = MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID;
    GUIANIM_DATA_INFO_T data_info   = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        OpenPicFolderWin(win_id);
        break;
        
    case MSG_FULL_PAINT:
        DisplayBackgroudImg(win_id);
        break;

    case MSG_PICVIEWER_RELOAD_FILE:
        {
            uint32                      time_out = 0;
            MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;

            MMIPICVIEW_DestroyPicLibArray();
            MMIPICVIEW_CreatePicLibArray();

            //clear pic list
            ClearPicFolderList(win_id, ctrl_id);

            MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_NONE, HandleLoadPicFolderWinMsg);
        }
        break;
    case MSG_GET_FOCUS:
        //reload file
// #ifndef MMI_PICREVIEW_LIST_LOW_MEMORY////NEWMS00145771
//         if (s_is_reload_file)
//         {
//             MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
//             s_is_reload_file = FALSE;
//         }
// #else
        if (s_is_reload_file)
        {
            MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
            s_is_reload_file = FALSE;
        }
        else
        {
            if(s_is_clear_pic_folder_List)
            {
                uint16 pic_num = MMIPICVIEW_GetAllPicNum();
                MMK_SendMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &pic_num);
            }
        }
// #endif
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        break;
        
    case MSG_PICVIEWER_UPDATE_LIST:
        UpdatePicFolderList(win_id, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, *(uint16*)param);
        break;
    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            uint16 icon_num                     = 0;
            FILEARRAY   picfilelib_array        = MMIPICVIEW_GetPicLibArray();
            GUIICONLIST_DISP_T display_ptr      = {0};
            FILEARRAY_DATA_T   file_info        = {0};

            wchar  file_name[MMIFMM_FILENAME_LEN + 1]   = {0};
            uint16 file_name_len = 0;

            char      str_file_count[30 + 1]            = {0};
            uint16    str_file_count_len                = 0;

            //append icon item text
            uint16 folder_index = *((uint16 *)param);

#ifdef FONT_TYPE_SUPPORT_NONE   //bug117110
			GUI_FONT_ALL_T text_info = {0};
			text_info.font      = MMI_DEFAULT_TEXT_FONT/*SONG_FONT_20*/;
			text_info.color     = MMI_DEFAULT_TEXT_COLOR;
#endif
            display_ptr.softkey_id[0] = STXT_OPTION;
            display_ptr.softkey_id[2] = STXT_RETURN;
#ifndef PIC_PREVIEW_U200_SUPPORT
            if (folder_index == 0)
            {
                //MMI_GetLabelTextByLang(TXT_COMMON_CAPTURE, &display_ptr.name_str);
                wchar str_tmp[] = L"Camera";
                display_ptr.name_str.wstr_ptr = str_tmp;
                display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);
                MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
                GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
				GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif
            }
            else
            {
                if(folder_index == 1)
                {
#if defined(CMCC_UI_STYLE)
					if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
					{
						MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index - 1, &file_info);
						MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
					}
					else
#endif
					{
 //                   MMI_GetLabelTextByLang(TXT_DC_MY_PICTURE, &display_ptr.name_str);
                    wchar str_tmp[] = L"Camera";
                    display_ptr.name_str.wstr_ptr = str_tmp;
                    display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);

                    MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                    file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
					}
                }
                else
                {
                    MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index - 1, &file_info);
                    MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
                }

                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index - 1);
                }
#else
            if (folder_index == 0)
            {
//                MMI_GetLabelTextByLang(TXT_DC_MY_PICTURE, &display_ptr.name_str);
                wchar str_tmp[] = L"Camera";
                display_ptr.name_str.wstr_ptr = str_tmp;
                display_ptr.name_str.wstr_len = MMIAPICOM_Wstrlen(str_tmp);
                
                MMI_WSTRNCPY(file_name, MMIFMM_FILENAME_LEN, (const wchar *)display_ptr.name_str.wstr_ptr, display_ptr.name_str.wstr_len, display_ptr.name_str.wstr_len);
                file_name_len = MIN(display_ptr.name_str.wstr_len,MMIFMM_FILENAME_LEN);
            }
            else
            {
                MMIAPIFILEARRAY_ReadFolder(picfilelib_array, folder_index, &file_info);
                MMIAPICOM_GetFileName(file_info.filename, file_info.name_len, file_name, &file_name_len);
            }
            
            if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index))
            {
                icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index);
            }
#endif
//             sprintf(str_file_count, " (%d)", icon_num);
//             str_file_count_len = strlen(str_file_count);
//             if ((file_name_len + str_file_count_len) <= MMIFMM_FILENAME_LEN)
//             {
//                 MMI_STRNTOWSTR(file_name + file_name_len, MMIFMM_FILENAME_LEN - file_name_len, (const uint8*)str_file_count, str_file_count_len, str_file_count_len);
//                 file_name_len += str_file_count_len;
//             }
// 
//             display_ptr.name_str.wstr_ptr = file_name;
//             display_ptr.name_str.wstr_len = file_name_len;
//             GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
            if((0 == file_name_len) && (0 != file_info.name_len))
            {
               // file_name_len = file_info.name_len;
                file_name_len = MIN(file_info.name_len,MMIFMM_FILENAME_LEN);
                SCI_MEMCPY(file_name, file_info.filename, file_name_len * sizeof(wchar));
            }

            sprintf(str_file_count, " (%d)", icon_num);
            str_file_count_len = strlen(str_file_count);
            if (file_name_len + str_file_count_len > MMIFMM_FILENAME_LEN)
            {
				file_name_len = MMIFMM_FILENAME_LEN - str_file_count_len;
				SCI_MEMSET(file_name + file_name_len , 0, (MMIFMM_FILENAME_LEN - file_name_len)*sizeof(wchar));
            }
            display_ptr.name_str.wstr_ptr = file_name;
            display_ptr.name_str.wstr_len = file_name_len;
            GUIICONLIST_AppendText(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
			GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif
			
			SCI_MEMSET(file_name, 0, MMIFMM_FILENAME_LEN * sizeof(wchar));
            MMI_STRNTOWSTR(file_name, MMIFMM_FILENAME_LEN, (const uint8*)str_file_count, str_file_count_len, str_file_count_len);
            display_ptr.name_str.wstr_ptr = file_name;
            display_ptr.name_str.wstr_len = str_file_count_len;
            GUIICONLIST_AppendTextAddtion(folder_index, ctrl_id, &display_ptr);
#ifdef FONT_TYPE_SUPPORT_NONE
			GUIICONLIST_SetIconListTextInfo(ctrl_id, text_info);
#endif

#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }
        break;

    case MSG_CTL_ICONLIST_ICON_NUM:
        {
            uint16 icon_num = 0;
            FILEARRAY   picfilelib_array    = MMIPICVIEW_GetPicLibArray();

            uint16 folder_index = *((uint16 *)param);
#ifndef PIC_PREVIEW_U200_SUPPORT
            if (0 == folder_index)
            {
                GUIICONLIST_SetIconNum(folder_index, 1, ctrl_id);
            }
            else
            {
                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index - 1) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index - 1);
                }

#else
                if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index))
                {
                    icon_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, folder_index);
                }
#endif
                else
                {
                    //if (0 == folder_index)
                    {
                        icon_num = 1;
                    }
                }
                GUIICONLIST_SetIconNum(folder_index, icon_num, ctrl_id);
#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*)param)->src_id)
                {
#ifdef PIC_PREVIEW_U200_SUPPORT
                case MMIPICVIEW_FLODER_RETURN_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                    break;

                case MMIPICVIEW_SET_BUTTON_CTRL_ID:
                    MMIAPIPICVIEW_OpenSetWin();
                    break;
#endif
                case MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID:
                    MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                    break;
                    
                default:                
                    result = MMI_RESULT_FALSE;
                    break;
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_ICONFOLDER_APPEND_ICON:
        {
            //append icon
            FILEARRAY_DATA_T            file_info = {0};
            GUIANIM_FILE_INFO_T         icon_file = {0};
            GUIICONFOLDER_NOTIFY_T  *notify_ptr = (GUIICONFOLDER_NOTIFY_T *)param;

            FILEARRAY   picfilelib_array        = MMIPICVIEW_GetPicLibArray();
            
#ifndef PIC_PREVIEW_U200_SUPPORT
            if(0 == notify_ptr->folder_index)
            {
                data_info.img_id = IMAGE_PICVIEW_CAMERA_FOLDER;
                GUIICONFOLDER_AppendIcon(0, notify_ptr->notify.src_handle, &data_info, PNULL);
            }
            else
            {
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index - 1))

#else
            //if ((0 == notify_ptr->folder_index)&&(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index)))
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(picfilelib_array, notify_ptr->folder_index))
#endif
            {
                //除了第一个文件夹，其它所有文件夹都不应该出现空文件夹的情况
                data_info.img_id = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
                GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle,&data_info, PNULL);
            }
            else
            {
#ifndef PIC_PREVIEW_U200_SUPPORT
                MMIAPIFILEARRAY_Read(picfilelib_array, notify_ptr->icon_index + MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, notify_ptr->folder_index - 1), &file_info);
#else
                MMIAPIFILEARRAY_Read(picfilelib_array, notify_ptr->icon_index + MMIAPIFILEARRAY_GetFirstFilePos(picfilelib_array, notify_ptr->folder_index), &file_info);
#endif
#ifdef DRM_SUPPORT  
                if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
                {
                    //is it has right
                    if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
                    {
                        data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
                    }
                    else
                    {
                        data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
                    }
                    //append icon
                    GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle,&data_info, PNULL);
                }
                else
#endif
                {
                    //set file name
                    icon_file.full_path_wstr_ptr = file_info.filename;
                    icon_file.full_path_wstr_len = file_info.name_len;
                    //append icon
                    GUIICONFOLDER_AppendIcon(notify_ptr->icon_index, notify_ptr->notify.src_handle, PNULL, &icon_file);
                }
            }
#ifndef PIC_PREVIEW_U200_SUPPORT
            }
#endif
        }

        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if (MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID == MMK_GetActiveCtrlId(win_id))
        {
            uint32 folder_index = 0;
            folder_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
#ifndef PIC_PREVIEW_U200_SUPPORT
            if(0 == folder_index)
            {
                CamerButtonCallBack();
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index - 1);
            }
            else if(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index - 1))
#else
            //if ((0 == folder_index)&&(MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index)))
            if (MMIFMM_INVALID_INDEX == MMIAPIFILEARRAY_GetLastFilePos(MMIPICVIEW_GetPicLibArray(),folder_index))
#endif
            {
                result = MMI_RESULT_FALSE;
            }
            else
            {
                EntryPicviewListClearPicFolderList(win_id,ctrl_id);
#ifndef PIC_PREVIEW_U200_SUPPORT
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index - 1);
#else
                MMIPICVIEW_InitPicArrayFromPicLib(folder_index);
#endif
                MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB, PNULL);
            }
        }
        break;
        
#ifndef PIC_PREVIEW_U200_SUPPORT
    case MSG_APP_MENU:
    case MSG_CTL_OK:
        MMK_CreateWin((uint32 *)MMIPICVIEW_FOLDER_LIST_OPT_WIN_TAB, PNULL);
        break;
#endif

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
    //destroy lib array
    MMIPICVIEW_DestroyPicLibArray();
    break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : handle pic folder win open msg
//	Global resource dependence :
//   Author: yaye.jiang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenPicFolderWin(MMI_WIN_ID_T        win_id)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     = MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID;
#ifdef PIC_PREVIEW_U200_SUPPORT
    MMI_CTRL_ID_T       button_id[] ={MMIPICVIEW_CAMERA_BUTTON_CTRL_ID, MMIPICVIEW_SET_BUTTON_CTRL_ID};
    MMI_IMAGE_ID_T      img_id[]    = {IMAGE_PICVIEW_CAMERA, IMAGE_PICVIEW_SET};
#endif
//    GUI_BG_T            bg_info     = {0};
    GUI_BORDER_T        boder_type  = {0};
    
    DisplayBackgroudImg(win_id);

//     bg_info.bg_type = GUI_BG_IMG;
// 	bg_info.img_id	= PICVIEW_DEFAULT_BG_IMG;
//     GUISTATUSBAR_SetBg(win_id, bg_info);
#ifndef MMI_GUI_STYLE_MINISCREEN
    GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#endif
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
#ifndef MMI_GUI_STYLE_MINISCREEN
	GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
    SetPicListTitle(win_id, MMIPICVIEW_FLODER_TITLE_CTRL_ID, MMIPICVIEW_FLODER_RETURN_CTRL_ID);
    SetButtonFormParam(win_id, button_id, img_id, 2);
    GUIBUTTON_SetCallBackFunc(MMIPICVIEW_CAMERA_BUTTON_CTRL_ID, CamerButtonCallBack);
    boder_type.type     = GUI_BORDER_NONE;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
    SetPicListTitle(win_id, MMIPICVIEW_FLODER_TITLE_CTRL_ID, PNULL);
#else
    /*MINI UI 需求:不显示list_title*/
    CTRLICONLIST_SetIsDispTitle(ctrl_id, FALSE);
#endif
    //GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);

//    GUIWIN_SetSoftkeyCommonBg(win_id, &bg_info);
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
    boder_type.type     = GUI_BORDER_SOLID;
    boder_type.width    = 0;
    boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);    
    boder_type.width    = 2;
    boder_type.color    = MMIPICVIEW_TITLE_COLOR;MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#endif

    SetIconListParam(win_id, ctrl_id, TRUE);
    GUIICONLIST_SetDefaultIconByPtr(ctrl_id, IMAGE_PICVIEW_FOLDER_DEFAULT_ICON, IMAGE_PICVIEW_PIC_ERROR_ICON);
    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_FOLDER_UIDT);
    
    //同步打开等待窗口, 避免闪烁
    MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
    return result;
}

/*****************************************************************************/
//  Description : camera button callback
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CamerButtonCallBack(void)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
	MMIVIRTUALARRAY_SetBufferSize(MMIPICVIEW_GetPicLibArray(), 2);
#ifndef MMI_GUI_STYLE_MINISCREEN
	EntryPicviewListClearPicFolderList(MMIPICVIEW_PICFOLDER_WIN_ID, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID);
#endif
    MMIAPIDC_OpenPhotoWin();

    return result;
}

/*****************************************************************************/
//  Description : clear pictures folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void ClearPicFolderList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)
{
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicFolderList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_853_112_2_18_2_46_22_8,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, 0);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
        GUIICONLIST_SetCurIconIndex(0,  ctrl_id);
        break;
    }

    //set title
    //GUIWIN_SetTitleTextId(win_id,TXT_PICVIEW,FALSE);

    //set softkey
    //GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_NULL,STXT_RETURN,FALSE);
}
/*****************************************************************************/
//  Description : load all picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicFolderWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = StartLoadPicWithFolders(win_id, msg_id, param);
SCI_TRACE_LOW("[mmipicview_list]HandleLoadPicFolderWinMsg:start to StartLoadPicWithFolders, and result is %d", result);
        break;

    case MSG_PICVIEWER_LOAD_FINISH:
SCI_TRACE_LOW("[mmipicview_list]HandleLoadPicFolderWinMsg:MSG_PICVIEWER_LOAD_FINISH");
        FinishLoadPicWithFolders(win_id);
        break;

    case MSG_APP_CANCEL:
SCI_TRACE_LOW("[mmipicview_list]HandleLoadPicFolderWinMsg:MSG_APP_CANCEL");
        MMIAPIFMM_SearchFileStop();
        MMK_PostMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, (DPARAM)PNULL, (uint32)0);
        //MMK_PostMsg(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, MSG_CTL_CANCEL, (DPARAM)PNULL, (uint32)0);
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : start load picture folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPicWithFolders(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    BOOLEAN                     is_recurse = FALSE;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter = {0};
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    MMIPICVIEW_DEFAULT_PATH_T   photos_path = {0};


    //get all picture file suffix
    GetAllPicSuffix(&file_filter);
    MMIPICVIEW_SetViewType(MMIPICVIEW_VIEW_ICONLIST);

#if defined(CMCC_UI_STYLE)
    if (MMIPICVIEW_DOWNLOAD_PIC == MMIPICVIEW_GetPathType())
    {
        default_path.path_len = MMIFMM_PATHNAME_LEN;
        
#if defined(DL_SUPPORT)
        MMIAPIDL_GetDownloadPath(MMIDL_DOWNLOAD_PICTURE_TYPE, default_path.path, &default_path.path_len);
#else
        GetDefaultPicPath(&default_path);
#endif
    }
    else
#endif
    {
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);
        MMIAPIPICVIEW_GetMyPhotosPath(&photos_path);
		if(!MMIAPICOM_Wstrncmp(photos_path.path, default_path.path, photos_path.path_len))
		{
			default_path = photos_path;//如果default_path是photos_path的子目录，则只搜一次
		}
    }

    is_recurse = TRUE;

#if 1
    MMIAPIFMM_SearchFileInPathEx(default_path.path, default_path.path_len,
                              (const wchar *)photos_path.path,
                              &file_filter,
                              is_recurse,
                              FUNC_FIND_FOLDERS_WITH_SUBFILES,
                              FILEARRAY_SORT_TIME_CREATE_DESCEND,
                              MMIPICVIEW_GetPicLibArray(),
                              win_id,
                              MSG_PICVIEWER_LOAD_FINISH
                             );
#else
    //search all the picture file aysn
    MMIAPIFMM_SearchFileInPath(path_ptr,
                            path_len,
                            &file_filter,
                            is_recurse,
                            FUNC_FIND_FOLDERS_WITH_SUBFILES,
                            MMIPICVIEW_GetPicLibArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH);
#endif

    //set softkey
    //MMIPUB_SetWinSoftkey(win_id,TXT_NULL,STXT_CANCEL,FALSE);

    result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

    return (result);
}
/*****************************************************************************/
//  Description : finish load picture Folders
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPicWithFolders(
    MMI_WIN_ID_T   win_id
)
{
    uint16      cur_index = 0;
    uint32      pic_num = 0;

    pic_num = MMIPICVIEW_GetAllPicNum();
    if (0 < pic_num)
    {
        if (MMIAPIDC_IsOpened())
        {
            cur_index = (uint16)MMIAPIDC_GetSavePhotoIndex(pic_num, MMIPICVIEW_GetPicLibArray());
        }
    }

    //need update list

    MMK_PostMsg(MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_UPDATE_LIST, &cur_index, sizeof(uint16));

    MMK_CloseWin(win_id);
}
/*****************************************************************************/
//  Description : update picture folder list
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicFolderList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
)
{
    uint16                  i = 0;
    GUILIST_ITEM_T          item = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;
    FILEARRAY   			picfilelib_array    = MMIPICVIEW_GetPicLibArray();
    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicFolderList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1034_112_2_18_2_46_23_9,(uint8*)"d", view_type);

	if(s_is_clear_pic_folder_List)
	{
		MMIVIRTUALARRAY_SetBufferSize(picfilelib_array, 50);
	}

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        {
            uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNumInLib();
            //set max item
            GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);

            //st title
            GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);

            if (0 < total_num)
            {
                //set title index
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

                //set item
                item.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
                for (i = 0; i < total_num; i++)
                {
                    GUILIST_AppendItem(ctrl_id, &item);
                }

                //set current index
                GUILIST_SetCurItemIndex(ctrl_id, cur_index);
            }
        }
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        {
            uint32          total_folder_num  = MMIAPIFILEARRAY_GetTotalFolderCount(picfilelib_array);
#if 0
            //set title
            if (0 == total_folder_num)
            {
                GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);
            }
            else
#endif                
            {
#ifdef PIC_PREVIEW_U200_SUPPORT
                GUIICONLIST_SetTotalIcon(ctrl_id, total_folder_num);
#else
                GUIICONLIST_SetTotalIcon(ctrl_id, total_folder_num + 1);
#endif

        		if(s_is_clear_pic_folder_List)
        		{
        	        s_is_clear_pic_folder_List=FALSE;
//#ifdef PIC_PREVIEW_U200_SUPPORT
                    GUIICONLIST_SetCurIconIndex(MMIPICVIEW_GetCurFolderIndex(), ctrl_id);
//#else
//                    GUIICONLIST_SetCurIconIndex(MMIPICVIEW_GetCurFolderIndex() + 1, ctrl_id);
//#endif
        		}
        		else
        		{
                    GUIICONLIST_SetCurIconIndex(0, ctrl_id);
                }

            }
        }

        break;
    }

}
#endif
/*****************************************************************************/
// 	Description : show pic list in my photos path
//	Global resource dependence : none
//    Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterMyPhotos(void)
{
    s_is_need_to_reinit_picarray_from_piclib = TRUE;

    if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID))
    {
        MMK_WinGrabFocus(MMIPICVIEW_LIST_WIN_ID);
    }
    else
    {
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB, PNULL);
    }
}

PUBLIC void MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_TYPE_E type)
{
    s_enter_from_type = type;
}

PUBLIC MMIPICVIEW_FROM_TYPE_E MMIPICVIEW_GetEnterType(void)
{
    return s_enter_from_type;
}
/*****************************************************************************/
//  Description : handle pictures privacy password
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void PICVIEW_ValidatePrivacyPicList(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_WIN_ID_T win_id = 0;

    if (PNULL != param_ptr)
    {
        win_id = *(MMI_WIN_ID_T*)param_ptr;
    }
    
    if ( is_permitted )
    {
        //同步打开等待窗口, 避免闪烁
        MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
    }
    else
    {
        MMK_CloseWin(win_id);
    }
}

#ifdef BAIDU_DRIVE_SUPPORT	

LOCAL void PICVIEW_HandleBaiduDrv_msg(MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param)
{
    uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;
    GUILIST_NEED_ITEM_DATA_T    *item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    MMI_STRING_T                title_str = {0};
    MMI_WIN_ID_T                      query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    uint16                      pic_num =0;
    uint32                      cur_index = 0;
    uint16                      mark_num = 0;
    wchar                       mark_wstr[60] ={0};
    char                        mark_num_str[30] ={0}; 
    wchar                       mark_num_wstr[30]={0};
    PIC_SOURCE_TYPE path = PICVIEW_SOURCE_NONE;
    MMIPICVIEW_FROM_TYPE_E      enter_form = MMIPICVIEW_FROM_NONE;

	if(PNULL == s_picview_mutex_ptr)
		s_picview_mutex_ptr = SCI_CreateMutex("Mutex for pic view protection", SCI_INHERIT);

    switch (msg_id)
    {
    case MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY :      
        SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_CAMERA_ROLL_UPLOAD_PHOTO_NOTIFY ");
        break;
    case MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF:
        SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF ");        
		break;        
    case MSG_BAIDU_DRV_INIT_CNF:
        {
            BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
            BAIDU_DRV_TYPE_E fromtype = BAIDU_DRV_TYPE_NONE;
			
		if(param!=PNULL){
		    fromtype = data->baidu_drv_msg_data.init_type; 
		}
		
		SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_BAIDU_DRV_INIT_CNF is_need_download_thumbnails=%d,s_mmipicview_multi_uploading_done=%d,fromtype=%d",is_need_download_thumbnails,s_mmipicview_multi_uploading_done,fromtype);

		if(s_mmipicview_multi_uploading_done){
		    MMISRV_BAIDUDRV_Upload_multiple_photos();
		}else if(BAIDU_DRV_TYPE_DOWNLOAD_THUMBNAIL == fromtype && is_need_download_thumbnails){
		    //  s_thumbnail_down_total_count = 0;
		    MMISRV_BAIDUDRV_Download_Thumbnail();
		}
        }
        break;
    case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY  :
        {
			BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
			GUIANIM_DATA_INFO_T data_info   = {0};
			GUIANIM_FILE_INFO_T     icon_file = {0};		
			GUIANIM_DISPLAY_INFO_T  display_info 	= {0};			
			uint8 i = 0;
			uint16 curr_exist_thumb_num =  0 , total_thumb_num = 0; //MMIPICVIEW_GetAllPicNum();
//			FILEARRAY existed_file_array = MMIAPISRV_BAIDUSRV_GET_Exist_Thumb_File_array();//MMIAPIFILEARRAY_Create();
			FILEARRAY all_file_array = MMIAPISRV_BAIDUSRV_GET_All_Thumb_File_array();//MMIAPIFILEARRAY_Create();
			FILEARRAY pic_file_array = PNULL ; 
			FILEARRAY_DATA_T    file_info = {0};
			
            if(    MMIAPIPICVIEW_GetShowFolderPath() != PICVIEW_SOURCE_NONE
				&& MMIAPIPICVIEW_GetShowFolderPath() != PICVIEW_SOURCE_CLOUD_THUMBNAIL){
                break;
            }               
			cur_index=MMIPICVIEW_GetCurIndex(ctrl_id);

			data_info.img_id = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
			display_info.bg.bg_type = GUI_BG_NONE;
			display_info.bg.color   = MMI_GRAY_WHITE_COLOR;
			display_info.bg.shape   = GUI_SHAPE_RECT;
			display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
			display_info.is_crop_rect      = TRUE;
			display_info.is_disp_one_frame = TRUE;
			display_info.is_free_buf_lose_focus = TRUE;			
			//get thumbnail total count 
			total_thumb_num = MMISRV_BAIDUDRV_Get_Total_file_num();
			//get existed thumbnail count
			//curr_exist_thumb_num = MMIAPIFILEARRAY_GetArraySize(existed_file_array);
			
			MMIPICVIEW_DestroyAllPicArray();
			MMIPICVIEW_CreateAllPicArray();

			pic_file_array = MMIAPIPICVIEW_GetAllPicArray();
			MMIAPIFILEARRAY_Combine(pic_file_array , all_file_array);
			//MMIAPIFILEARRAY_Combine(pic_file_array , existed_file_array);

			s_thumbnail_down_total_count = data->baidu_drv_msg_data.curr_download_num;
			
			SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY total_count=%d",s_thumbnail_down_total_count);
			
			//if( 0 == curr_exist_thumb_num && 0 == s_thumbnail_down_total_count)
			if( 0 == total_thumb_num && 0 == s_thumbnail_down_total_count)
			{
				MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
				MMIPICVIEW_SetListEmpty(TRUE);
				PicviewListAllEmptyShow(MMIPICVIEW_LIST_WIN_ID);
				if( MMK_IsFocusWin(MMIPICVIEW_WAIT_WIN_ID))
				{
					SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY SEND MSG_PICVIEWER_LOAD_FINISH !!!");					
					MMK_CloseWin(MMIPICVIEW_WAIT_WIN_ID);
				}
				
				MMK_CreateWin((uint32 *)WATCH_GALLERY_MAIN_WIN_TAB,PNULL);
				MMK_CloseWin(MMIPICVIEW_LIST_WIN_ID);
			}
			else
			{
//				MMIPICVIEW_SetCurIndex(ctrl_id,cur_index);
//				GUIICONLIST_SetTotalIcon(ctrl_id, curr_exist_thumb_num+s_thumbnail_down_total_count);
				if(total_thumb_num != GUIICONLIST_GetTotalIcon(ctrl_id))
				{
					GUIICONLIST_SetTotalIcon(ctrl_id, total_thumb_num);
				}
				SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY GUIICONLIST_SetTotalIcon 1 num= %d " ,  total_thumb_num);
//				SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY GUIICONLIST_SetTotalIcon 1 num= %d " ,  curr_exist_thumb_num+s_thumbnail_down_total_count);
				
				for(i=0;i<total_thumb_num;i++)
				{
					SCI_MEMSET((void*)&file_info , 0x00 , sizeof(FILEARRAY_DATA_T));		
					MMIPICVIEW_GetPicFileInfo(i,&file_info);

					if(MMIAPIFMM_IsFileExist(file_info.filename , file_info.name_len))
					{
						icon_file.full_path_wstr_ptr = &file_info.filename[0] ;
						icon_file.full_path_wstr_len = file_info.name_len;
						CTRLICONLIST_AppendIconEx(i, ctrl_id,  PNULL, &icon_file , &display_info);
						SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY append icon idx = %d , icon_file" , i);
					}
					else
					{
						CTRLICONLIST_AppendIconEx(i, ctrl_id,  &data_info, PNULL, &display_info);
						SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY append icon idx = %d , data_info" , i);
					}					
				}

				//if( 0 < curr_exist_thumb_num && MMK_IsFocusWin(MMIPICVIEW_WAIT_WIN_ID))
				if( 0 < total_thumb_num && MMK_IsFocusWin(MMIPICVIEW_WAIT_WIN_ID))
				{
					SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY SEND MSG_PICVIEWER_LOAD_FINISH !!!");
					//MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
					MMK_CloseWin(MMIPICVIEW_WAIT_WIN_ID);
				}				
			}


            s_download_thumbnail_index = 0;
            if(MMK_IsFocusWin(win_id)){
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;	
    case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF:
        {
			BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
			GUIANIM_FILE_INFO_T     icon_file = {0};
			uint8 success_count = 0 ,  i = 0 , total_iconlist_num=0;
			uint16 updated_pic_array_num = 0;
			//uint16 ori_pic_array_num = MMIPICVIEW_GetAllPicNum();
			char    temp_code[100]={0};
			GUIANIM_DATA_INFO_T data_info   = {0};
			FILEARRAY append_file_array = PNULL ; 		
			FILEARRAY pic_file_array = PNULL ; 			
			FILEARRAY_DATA_T    file_info = {0};
			GUIANIM_DISPLAY_INFO_T  display_info 	= {0};
			
			success_count = data->baidu_drv_msg_data.success_num; 
			MMIAPICOM_WstrToStr(data->baidu_drv_msg_data.filename, (uint8 *)temp_code);
			SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF count %d,total=%d,filename=%s,done=%d" , success_count,
				data->baidu_drv_msg_data.curr_download_num,temp_code,data->baidu_drv_msg_data.bDone);
					
			if(!data->baidu_drv_msg_data.bDone)
			{
				SCI_TRACE_LOW("[MMIPIC]: 1-!data->baidu_drv_msg_data.bDone !!!");
				
				break;
			}

			if(MMK_IsFocusWin(MMIPICVIEW_WAIT_WIN_ID))
			{
				SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF SEND MSG_PICVIEWER_LOAD_FINISH !!!");
				//MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
				MMK_CloseWin(MMIPICVIEW_WAIT_WIN_ID);
			}


			icon_file.full_path_wstr_ptr = data->baidu_drv_msg_data.filename;
			icon_file.full_path_wstr_len = MMIAPICOM_Wstrlen(data->baidu_drv_msg_data.filename);
			data_info.img_id = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;

			display_info.bg.bg_type = GUI_BG_NONE;
			display_info.bg.color   = MMI_GRAY_WHITE_COLOR;
			display_info.bg.shape   = GUI_SHAPE_RECT;
			display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
			display_info.is_crop_rect      = TRUE;
			display_info.is_disp_one_frame = TRUE;
			display_info.is_free_buf_lose_focus = TRUE;

			
			total_iconlist_num	= GUIICONLIST_GetTotalIcon(ctrl_id) ; 
			SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF GUIICONLIST_GetTotalIcon  = %d " , total_iconlist_num);
			

			for(i=0;i<total_iconlist_num;i++)
			{
				SCI_MEMSET((void*)&file_info , 0x00 , sizeof(FILEARRAY_DATA_T));		
				MMIPICVIEW_GetPicFileInfo(i,&file_info);

				//if(MMIAPIFMM_IsFileExist(file_info.filename , file_info.name_len)

				if (0 == MMIAPICOM_Wstrncmp (icon_file.full_path_wstr_ptr, file_info.filename , file_info.name_len))
				{
					icon_file.full_path_wstr_ptr = &file_info.filename[0] ;
					icon_file.full_path_wstr_len = file_info.name_len;
					CTRLICONLIST_AppendIconEx(i, ctrl_id,  PNULL, &icon_file , &display_info);
					SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF idx found !! idx = %d , icon_file" , i);
					break;
				}
			}

				if(s_thumbnail_down_total_count == ( data->baidu_drv_msg_data.success_num +  data->baidu_drv_msg_data.fail_num))
				{
					SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF SEND MSG_PICVIEWER_LOAD_FINISH !!!");
					//MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID,MSG_PICVIEWER_LOAD_FINISH,PNULL,PNULL);
				//MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
			}
			
			
			if(MMK_IsFocusWin(win_id)){
				MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			}
            s_download_thumbnail_index ++;
			
			if(s_thumbnail_down_total_count <= MMIPICVIEW_NUM_THUMBNAIL_PER_PAGE
				&&s_download_thumbnail_index >= s_thumbnail_down_total_count){
				is_need_download_thumbnails = FALSE;
				s_thumbnail_down_total_count = 0;//finish download thumbnails
			}
			else{
				is_need_download_thumbnails = TRUE;
			}
			SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF  index %d,is_need_download_thumbnails=%d",
				s_download_thumbnail_index,is_need_download_thumbnails);                       
	}
    break;
    case MSG_BAIDU_DRV_DELETE_PHOTO_NOTIFY:
        //DeleteLocalThumbnailsByServer(ctrl_id);
        SCI_TRACE_LOW("[MMIPIC]:delete photo.  ###############");
        break;
    case MSG_BAIDU_DRV_ERROR_NOTIFY:
        {
            BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
            BAIDU_DRV_ERROR_E err_trype = data->baidu_drv_msg_data.error_type;
			MMI_STRING_T txt_tip = {0};
			WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};

            SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg receive BAIDU_DRV_ERROR_NOTIFY err_trype=%d",err_trype);

            if(BAIDU_DRV_AUTO_SYNC== MMISRV_BAIDUDRV_Get_Sync_State()
                && (err_trype == BAIDU_DRV_ERROR_NO_NETWORK||err_trype == BAIDU_DRV_ERROR_HTTP_ERROR)
                && MMISRV_BAIDUDRV_Need_snapshot_auto_upload_photo()
				&&MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE)
			{
						MMI_GetLabelTextByLang(TXT_ERROR, &txt_tip);
						WatchCOM_NoteWin_1Line_Enter(win_id, &txt_tip,0,softkey,PNULL);
					}
            
        if(MMIPICVIEW_GetEnterType()!=MMIPICVIEW_FROM_NONE){
                break;
            }
            if(MMIAPIPICVIEW_GetShowFolderPath()!=PICVIEW_SOURCE_NONE
            &&MMIAPIPICVIEW_GetShowFolderPath()!=PICVIEW_SOURCE_CLOUD_THUMBNAIL){
                break;
            }
            //delete default thumbnail icons when err
            if(s_download_thumbnail_index < s_thumbnail_down_total_count
				&& (err_trype == BAIDU_DRV_ERROR_NO_NETWORK||err_trype == BAIDU_DRV_ERROR_HTTP_ERROR))
			{
                uint16 icon_num = GUIICONLIST_GetTotalIcon(ctrl_id);
                uint16 file_num = MMIPICVIEW_GetAllPicNum();
                SCI_TRACE_LOW("[MMIPIC]:HandlePicListWinMsg thumbnais  icon_num=%d,file_num=%d",icon_num,file_num);
				
                //when end icon with file,but the icon middle without file,such as download thumbnail timeout, then insert others normal icon,need reload
				if((GUIICONLIST_GetTotalIcon(ctrl_id)-file_num > 0)&&(s_thumbnail_down_total_count - s_download_thumbnail_index <= 0) && MMIPICVIEW_GetAllPicNum() > 0)
				{
//					MMIPICVIEW_PostUpdateMsg(MMIPICVIEW_GetCurIndex(ctrl_id));
					if(MMK_IsFocusWin(win_id))
					{
						MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
					}
				}
				else if(MMIPICVIEW_GetAllPicNum() == 0
					&&!GUIICONLIST_GetTotalIcon(ctrl_id) ==0)
				{
                    FinishLoadPic(win_id);
				}
				else if(GUIICONLIST_GetTotalIcon(ctrl_id)==file_num)
				{
					if(MMK_IsFocusWin(win_id))
					{
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                }
                s_download_thumbnail_index = 0;
                s_thumbnail_down_total_count = 0;
            }
            
			if(MMK_IsFocusWin(MMIPICVIEW_WAIT_WIN_ID))
			{
				SCI_TRACE_LOW("[MMIPIC]: MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY SEND MSG_PICVIEWER_LOAD_FINISH !!!");
				MMK_CloseWin(MMIPICVIEW_WAIT_WIN_ID);
			}	

        }
        break;
    	}
}
#endif

#if defined(BAIDU_DRIVE_SUPPORT)
LOCAL MMI_RESULT_E HandlePicListTPKeyUpMsg(MMI_WIN_ID_T win_id,
                                                                                      MMI_MESSAGE_ID_E msg_id,
                                                                                      DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMI_CTRL_ID_T               ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
	
    switch (msg_id)
    {
	 case MSG_KEYUP_DOWN:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527		 	
        case MSG_TP_PRESS_UP:
#endif			
        {
		uint16 cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);
		uint16 curr_download_count = 0 ;
		
			if(!MMIPICVIEW_GetListMarkable(ctrl_id)           
				&& (MMIAPIPICVIEW_GetShowFolderPath()== PICVIEW_SOURCE_NONE
				||MMIAPIPICVIEW_GetShowFolderPath() == PICVIEW_SOURCE_CLOUD_THUMBNAIL)
				&&MMIPICVIEW_GetEnterType()==MMIPICVIEW_FROM_NONE)
			{           
				//uint32 total_exist_thumbnail_num =MMIPICVIEW_GetAllPicNum();	
				//uint16 total_cloud_thumbnail_num = MMISRV_BAIDUDRV_Get_Total_file_num();
				SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_KEYUP_DOWN is_need_download_thumbnails=%d",is_need_download_thumbnails);
				//SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_KEYUP_DOWN exist num =%d , cloud num = %d",total_exist_thumbnail_num,total_cloud_thumbnail_num);
				//check whether all the thumbnails are downloaded 
				if( (FALSE == MMISRV_BAIDUDRV_Is_Downloading_Thumbanils()) 
					/*&& (total_cloud_thumbnail_num > total_exist_thumbnail_num)*/)
				{
					//check whether is the last page , if yes then prepare to trigger download process
					//if(cur_index + MMIPICVIEW_NUM_THUMBNAIL_PER_PAGE > total_exist_thumbnail_num)
					{
						SCI_TRACE_LOW("[HandlePicListWinMsg]:MSG_KEYUP_DOWN MMIAPISRV_BAIDUSRV_Update_Thumb !! cur_index = %d" , cur_index);
						//MMIPIC_CreateOneDriveSyncProgressDialogWin();
						curr_download_count = MMIAPISRV_BAIDUSRV_Update_Thumb();
						if(0 < curr_download_count)
							is_need_download_thumbnails =  TRUE;
					}			
				}							
			}
		}

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return MMI_RESULT_FALSE;
}

LOCAL BOOLEAN SetButtonParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_IMAGE_ID_T img_id)
{
    BOOLEAN             result      = TRUE;
    GUI_BG_T            bg_info     = {0};
    GUI_BG_T            fg_info     = {0};
    GUI_BOTH_RECT_T     form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T     button_rect = {0};
    uint16              img_width=0 , img_height=0, button_width_v=0, button_width_h=0;
    int                 i = 0;

	GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);
   
	form_rect.v_rect.top = form_rect.v_rect.bottom - img_height + 1;
	form_rect.h_rect.top = form_rect.h_rect.bottom - img_height + 1;
	bg_info.bg_type = GUI_BG_NONE;
	bg_info.img_id  = res_gallery_pic_button_bg;

	button_rect       = form_rect;
	fg_info.bg_type = GUI_BG_IMG;

	button_rect.v_rect.left    = (form_rect.v_rect.right - form_rect.v_rect.left + 1) / 2 - img_width/2 ;
	button_rect.v_rect.right   = button_rect.v_rect.left + img_width ;
	button_rect.h_rect.left    = (form_rect.h_rect.right - form_rect.h_rect.left + 1) / 2 - img_width/2 ;
	button_rect.h_rect.right   = button_rect.h_rect.left + img_width ;
	GUIAPICTRL_SetBothRect(ctrl_id, &button_rect);

	fg_info.img_id = img_id;
	//GUIBUTTON_SetBg(ctrl_id, &bg_info);
	GUIBUTTON_SetFg(ctrl_id, &fg_info);
	GUIBUTTON_PermitBorder(ctrl_id, FALSE);

	return FALSE;
}
#endif

/*****************************************************************************/
//  Description : handle picture list win msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;
    GUILIST_NEED_ITEM_DATA_T    *item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    
#ifdef  PIC_PREVIEW_U200_SUPPORT
    static  BOOLEAN             is_marked_all = FALSE;
    MMI_WIN_ID_T                query_win_id = MMIPICVIEW_QUERY_WIN_ID;
#endif

    //SCI_TRACE_LOW("MMIPICVIEW HandlePicListWinMsg msg_id = %d", msg_id);
    switch (msg_id)
    {

    case MSG_OPEN_WINDOW:
        MMIPICVIEW_SetListEmpty(FALSE);
        OpenPicListWin(win_id, ctrl_id);
		CTRLICONLIST_SetEmptyStrId(ctrl_id , TXT_NULL);
#if defined(BAIDU_DRIVE_SUPPORT)		
	 MMK_SetWinHookFunc( win_id, HandlePicListTPKeyUpMsg);		
	SetButtonParam(win_id, MMIPICVIEW_SET_BUTTON_CTRL_ID, IMAGE_PICVIEW_CHANGE_FOLDER);
#endif
        break;
        
    case MSG_FULL_PAINT:
    {
        DisplayBackgroudImg(win_id);
        if(s_is_list_empty)
          {
            PicviewListEmptyShow();
          }
        /* add by langhua yang to display the total pic number*/
#ifdef MMI_GUI_STYLE_MINISCREEN
        DispTotalPicNum();
#endif
    }
        break;

    case MSG_PICVIEWER_RELOAD_FILE:
        //load file
        if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        }
        MMIPICVIEW_DestroyAllPicArray();
        MMIPICVIEW_CreateAllPicArray();

        //clear pic list
        ClearPicList(win_id, ctrl_id);

        MMIPUB_OpenAlertWinByTextId(&time_out, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleLoadPicWinMsg);
        break;

    case MSG_PICVIEWER_UPDATE_LIST:
        //update file
        UpdatePicList(win_id, ctrl_id, *(uint16*)param);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //add item text etc.
        item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
        if (PNULL == item_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_data_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1209_112_2_18_2_46_23_10,(uint8*)"");
            break;
        }

        AppendPicListItem(TRUE, item_data_ptr->item_index, 0, ctrl_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //add item anim
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        if (PNULL == item_content_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_content_ptr is PNULL !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1221_112_2_18_2_46_23_11,(uint8*)"");
            break;
        }

        AppendPicListItem(FALSE, item_content_ptr->item_index, item_content_ptr->item_content_index, ctrl_id);
        break;

    case MSG_CTL_LIST_MARK_ITEM:
        //get mark num
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
        }
        break;

    case MSG_PICVIEWER_MODIFY_VIEW:
        ModifyPicListView(win_id, ctrl_id);
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            GUIICONLIST_DISP_T display_ptr      = {0};

            display_ptr.softkey_id[0] = STXT_OPTION;
            display_ptr.softkey_id[2] = STXT_RETURN;
            GUIICONLIST_AppendText(*(uint16 *)param, ctrl_id, &display_ptr);
        }
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        AppendPicIconListIcon(*(uint16 *)param, ctrl_id);
        break;

#ifdef  PIC_PREVIEW_U200_SUPPORT
    case MSG_CTL_ICONLIST_MARK_ITEM:
        {
            uint16 mark_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);

            if((mark_num != GUIICONLIST_GetTotalIcon(ctrl_id)) && is_marked_all)
            {
                is_marked_all = FALSE;
                GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            else if ((mark_num == GUIICONLIST_GetTotalIcon(ctrl_id)) && !is_marked_all)
            {
                is_marked_all = TRUE;
                GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_CANCEL_MARK);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
        }
        break;
#endif

    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg MSG_GET_FOCUS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1250_112_2_18_2_46_23_12,(uint8*)"");
#ifdef DRM_SUPPORT
		if(MMIPICVIEW_GetIsDownloadRightsFile())
		{
	        MMIPICVIEW_ModifyItemIcon(ctrl_id, MMIPICVIEW_GetCurIndex(ctrl_id));
			MMIPICVIEW_SetIsDownloadRightsFile(FALSE);
		}
#endif
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);        
// #ifndef PIC_PREVIEW_U200_SUPPORT
//         //crNEWMS00122330:???°áD±íμ??üD????ú??í????ˉàà?÷μ???ò?é2??
//         //reload file
//         if (s_is_reload_file)
//         {
//             MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
//             s_is_reload_file = FALSE;
//         }
//         else if (0 == MMIPICVIEW_GetAllPicNum())
//         {
//             //clear pic list
//             ClearPicList(win_id, ctrl_id);
//         }
// #endif
	    break;
#ifndef PIC_PREVIEW_U200_SUPPORT
    case MSG_APP_MENU:
    case MSG_CTL_OK:
        MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_OPT_WIN_TAB, PNULL);
        break;
        
#else
    case MSG_CTL_LIST_LONGOK:
    case MSG_NOTIFY_ICONLIST_TPLONG:
        //长按List弹出context menu菜单
        if(!MMIPICVIEW_GetListMarkable(ctrl_id))
    	{
	        MMIPICVIEW_OpenPicListLongokOptWin();
    	}
        break;
#endif
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined (ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#ifdef BAIDU_DRIVE_SUPPORT
	case MSG_NOTIFY_ICONLIST_TPLONG:
	{
		MMI_STRING_T            queryTipStr = { 0 };
		WATCH_SOFTKEY_TEXT_ID_T softkey = { STXT_RETURN, PNULL, STXT_OK };


		MMI_GetLabelTextByLang( TXT_DELETE, &queryTipStr );
		WatchCOM_QueryWin_2Btn_Enter(
			MMIPICVIEW_QUERY_WIN_ID,
			&queryTipStr,
			PNULL,
			PNULL,
			IMAGE_SET_COM_BTN_FIXED,
			IMAGE_SET_COM_BTN_CANCEL,
			softkey,
			HandlePicPreviewDelQueryWin
			);
	
	}
		break;
#endif
#endif
    case MSG_CTL_PENOK:
        if(PNULL != param)
        {   
            uint16  marked_num = 0;
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
#if defined(BAIDU_DRIVE_SUPPORT)
			case MMIPICVIEW_SET_BUTTON_CTRL_ID:
			{
				MMK_CreateWin((uint32 *)WATCH_GALLERY_MAIN_WIN_TAB,PNULL);
				MMK_CloseWin(win_id);
			}
			break;
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
            case MMIPICVIEW_LIST_RETURN_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;

            case MMIPICVIEW_LIST_SLIDE_PREVIEW:
    			MMIPICVIEW_SliePlay(ctrl_id);
                break;
                
            case MMIPICVIEW_LIST_MARKED:
                MMIPICVIEW_SetButtonsVisible(FALSE);
                InitMarkMenuButtons(win_id);
                HandlePicListMarked(ctrl_id);
                is_marked_all = FALSE;
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case MMIPICVIEW_MARK_ALLMARK_CTRL_ID:
                //全部标记的状态下
                if(is_marked_all)
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                    HandlePicListUnmarkedAll(ctrl_id);
                    is_marked_all   = FALSE;
                }
                else
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_CANCEL_MARK);
                    HandlePicListMarkedAll(ctrl_id);
                    is_marked_all   = TRUE;
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
                
            case MMIPICVIEW_MARK_DELETE_CTRL_ID:
                marked_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0);
#ifdef DRM_SUPPORT
                if (MMIPICVIEW_IsDeleteDrm(ctrl_id))
                {
                    if (1 < marked_num)
                    {
                        //prompt query window
                        MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                    }
                    else
                    {
                        //prompt query window
                        MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                    }
                }
                else
#endif
    			{
    			    if(1 <= marked_num)
    			    {
        				//prompt query window
        				MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
        		    }
    			}
                if(0 == MMIPICVIEW_GetSelectItemIndex(ctrl_id,PNULL,0))
                {
                    GUIBUTTON_SetTextId(MMIPICVIEW_MARK_ALLMARK_CTRL_ID, TXT_MARK_ALL);
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                 break;
                
            case MMIPICVIEW_MARK_CANCEL_CTRL_ID:
                MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                MMIPICVIEW_SetButtonsVisible(TRUE);
                MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                break;
#endif
            case MMIPICVIEW_LIST_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);
                break;

            default:              
                result = MMI_RESULT_FALSE;
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        HandlePicListWeb(ctrl_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
		if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
		{
	        GUIICONLIST_PrepareSpecial(ctrl_id, MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID);
		}
        MMK_CloseWin(win_id);
        break;
   #ifdef BAIDU_DRIVE_SUPPORT   				
    case MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY :      
    case MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF:
    case MSG_BAIDU_DRV_INIT_CNF:
    case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY  :
    case MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF:
    case MSG_BAIDU_DRV_DELETE_PHOTO_NOTIFY:
    case MSG_BAIDU_DRV_ERROR_NOTIFY:
		PICVIEW_HandleBaiduDrv_msg(win_id , msg_id , param);
        break;
     #endif
    case MSG_CLOSE_WINDOW:

		SCI_TRACE_LOW("MSG_CLOSE_WINDOW");
#ifdef BAIDU_DRIVE_SUPPORT
	  /*delete cload photo*/
	 MMISRV_BAIDUDRV_Delete_CloadPhoto_note();
#endif
// #ifndef PIC_PREVIEW_U200_SUPPORT
//         s_is_reload_file = FALSE;
// #endif
        //destroy pic array
        MMIPICVIEW_DestroyAllPicArray();
        if (s_is_need_to_reinit_picarray_from_piclib)
        {
#ifndef MMI_PICVIEW_MINI_SUPPORT
#ifndef PIC_PREVIEW_U200_SUPPORT
            MMIPICVIEW_InitPicArrayFromPicLib(s_cur_folder_index_for_reinit_picarray - 1);
#else
            MMIPICVIEW_InitPicArrayFromPicLib(s_cur_folder_index_for_reinit_picarray);

#endif
#endif

#ifndef MMI_PICVIEW_MINI_SUPPORT
            s_is_need_to_reinit_picarray_from_piclib = FALSE;
#endif
        }
        break;

    default:
        result = MMIAPIPICVIEW_HandleDelQueryWin(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle picture list open win msg
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void OpenPicListWin(
    MMI_WIN_ID_T      win_id, //in:
    MMI_CTRL_ID_T     ctrl_id //in:
)
{
#ifdef  PIC_PREVIEW_U200_SUPPORT
    MMI_CTRL_ID_T   ctrl_array[]= {MMIPICVIEW_LIST_SLIDE_PREVIEW, MMIPICVIEW_LIST_MARKED};
    MMI_IMAGE_ID_T  img_array[] = {IMAGE_PICVIEW_PLAY_DEFAULT, IMAGE_PICVIEW_MARK_DEFAULT};  
#endif
//    GUI_BG_T        bg_info     = {0};
    
    DisplayBackgroudImg(win_id);
    
//     bg_info.bg_type = GUI_BG_IMG;
// 	bg_info.img_id	= PICVIEW_DEFAULT_BG_IMG;
//     GUISTATUSBAR_SetBg(win_id, bg_info);
    GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
	GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#ifdef  PIC_PREVIEW_U200_SUPPORT
    SetPicListTitle(win_id, MMIPICVIEW_LIST_TITLE_CTRL_ID, MMIPICVIEW_LIST_RETURN_CTRL_ID);
    CreatePicListCtrl(win_id, ctrl_id);
    SetButtonFormParam(win_id, ctrl_array, img_array, 2);
#else
#ifndef MMI_GUI_STYLE_MINISCREEN
    //MINI UI设计中无ListTitle
    SetPicListTitle(win_id, MMIPICVIEW_LIST_TITLE_CTRL_ID, PNULL);
#else
    if(MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
    	GUITITLE_SetTextAlignType(MMIPICVIEW_LIST_WIN_ID, MMITHEME_GetTitleCtrlId(), ALIGN_HMIDDLE);
    }
#endif
    CreatePicListCtrl(win_id, ctrl_id);
   /* MINI UI 需求:add by langhua.yang display the total num */   
#ifdef MMI_GUI_STYLE_MINISCREEN
    {
        GUI_BOTH_RECT_T         both_rect={0};
        both_rect.v_rect.top    = MMIPICVIEW_TOTAL_NUM_TOP;
        both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPICVIEW_TOTAL_NUM_HEIGHT;
        both_rect.v_rect.left   = MMIPICVIEW_TOTAL_NUM_V_LEFT;
        both_rect.v_rect.right  = MMIPICVIEW_TOTAL_NUM_V_RIGHT;
        
        both_rect.h_rect.top    = MMIPICVIEW_TOTAL_NUM_TOP;
        both_rect.h_rect.bottom = both_rect.v_rect.top + MMIPICVIEW_TOTAL_NUM_HEIGHT;
        both_rect.h_rect.left   = MMIPICVIEW_TOTAL_NUM_H_LEFT;
        both_rect.h_rect.right  = MMIPICVIEW_TOTAL_NUM_H_RIGHT;
        GUIAPICTRL_SetBothRect(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID,&both_rect);
        if (MMK_GetCtrlPtr(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID))
		{
 	      IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);
		}
    }
#endif
    
    //GUIWIN_SetSoftkeyCommonBg(win_id, &bg_info);
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#endif

    if (s_is_need_to_reinit_picarray_from_piclib
#ifdef BAIDU_DRIVE_SUPPORT
		|| ( PICVIEW_SOURCE_CLOUD_THUMBNAIL== MMIAPIPICVIEW_GetShowFolderPath())
#endif				
		)
    { 
#ifndef MMI_PICVIEW_MINI_SUPPORT
        s_cur_folder_index_for_reinit_picarray = MMIPICVIEW_ReleasePicArrayFromPicLib();
#endif
        if (MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_MYDOC))
        {
            //validate privacy password
            MMIAPISET_ValidatePrivacy(MMISET_PROTECT_MYDOC, (MMISET_PRIVACY_CALLBACK_PFUNC)PICVIEW_ValidatePrivacyPicList, (void*)&win_id, sizeof(win_id));
        }
        else
        {
            //同步打开等待窗口, 避免闪烁
            MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
        }
    }
    else if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
    {
        GUIICONLIST_PrepareSpecial(MMIPICVIEW_FOLDER_ICONLIST_CTRL_ID, ctrl_id);
        GUIICONLIST_SetTotalIcon(ctrl_id, MMIPICVIEW_GetAllPicNum());
        GUIICONLIST_SetCurIconIndex(0, ctrl_id);
		LoadPicList(ctrl_id);
		//SetlistOrderInTime(ctrl_id);
    }
}

/*****************************************************************************/
//  Description : create pictures list control
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL void CreatePicListCtrl(
                             MMI_WIN_ID_T   win_id, //in:
                             MMI_CTRL_ID_T  ctrl_id //in:
                             )
{
    GUILIST_INIT_DATA_T     list_init_data = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIICONLIST_INIT_DATA_T icon_init_data = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_GetViewType();
#ifndef BAIDU_DRIVE_SUPPORT
    GUI_BOTH_RECT_T         both_rect = MMIPICVIEW_GetClientWinRect(win_id);
#else
	GUI_BOTH_RECT_T         both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
    GUI_BORDER_T    		boder_type  = {0};
	
    //SCI_TRACE_LOW:"MMIPICVIEW CreatePicListCtrl view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1488_112_2_18_2_46_24_13,(uint8*)"d", view_type);
    
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set init data
        list_init_data.both_rect = both_rect;
        list_init_data.type      = GUILIST_TEXTLIST_E;

        create.guid          = SPRD_GUI_LIST_ID;
        create.init_data_ptr = &list_init_data;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set init data
        icon_init_data.both_rect = both_rect;

        create.guid          = SPRD_GUI_ICONLIST_ID;
        create.init_data_ptr = &icon_init_data;
        break;
    }

    //creat control
    create.ctrl_id           = ctrl_id;
    create.parent_win_handle = win_id;

    MMK_CreateControl(&create);

    //set active
    MMK_SetAtvCtrl(win_id, ctrl_id);
    if(view_type == MMIPICVIEW_VIEW_ICONLIST)//NEWMS00172348
    {
            GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);

            //GUIICONLIST_SetDispCustomerSoftkey(ctrl_id, TRUE);



		    boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
		    boder_type.width    = 0;
		    boder_type.type     = GUI_BORDER_SOLID;
		    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);    
		    boder_type.width    = 2;
		    boder_type.color    = MMIPICVIEW_TITLE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
		    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#ifdef MMI_GUI_STYLE_MINISCREEN
            /*MINI UI 需求:不显示list_title*/
            CTRLICONLIST_SetIsDispTitle(ctrl_id, FALSE);
#endif
		    SetIconListParam(win_id, ctrl_id, FALSE);
		    GUIICONLIST_SetDefaultIconByPtr(ctrl_id, IMAGE_PICVIEW_FOLDER_DEFAULT_ICON, IMAGE_PICVIEW_PIC_ERROR_ICON);
		    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON);
		    GUIICONLIST_SetLoadType(ctrl_id, GUIICONLIST_LOAD_ONE_BY_ONE);
//		    GUIICONLIST_SetIconCacheNum(ctrl_id,1);
    }
    else
    {
            GUILIST_SetListState(ctrl_id, GUILIST_STATE_NEED_SOFTKEY, TRUE);
    }
}

/*****************************************************************************/
//  Description : load all picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicWinMsg(
    MMI_WIN_ID_T     win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM           param
)
{
    MMI_RESULT_E    		result = MMI_RESULT_TRUE;
	MMIFMM_FILTER_T 		file_filter = {0};
    static MMIFILE_DEVICE_E s_searching_device = MMI_DEVICE_UDISK;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_searching_device = MMI_DEVICE_UDISK;
        result = StartLoadPic(win_id, msg_id, param);
        break;

    case MSG_PICVIEWER_LOAD_FINISH:
        FinishLoadPic(win_id);
        break;

    case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        break;

    case MSG_APP_RED:
    case MSG_APP_FLIP:
        MMIAPIFMM_SearchFileStop();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

	case MSG_PICVIEWER_LOAD_CONTINUE:
		GetAllPicSuffix(&file_filter);
		if(!MMIPICVIEW_SearchAllPhotos(win_id, &file_filter, &s_searching_device))
		{
			if(MMIAPIFMM_GetAllAvarialeDiskNum() > 1)
			{
				MMIAPIFILEARRAY_Sort(MMIAPIPICVIEW_GetAllPicArray(), FILEARRAY_SORT_TIME_CREATE_DESCEND);
			}
			MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_FINISH, PNULL);
		}
		break;
		
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : start load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StartLoadPic(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    BOOLEAN                     is_recurse = FALSE;
#endif
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter = {0};
//    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
    FILEARRAY_DATA_T file_info_ptr={0}; 
    wchar                   abs_default_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1]={0};
    uint16                  abs_default_path_length = MMIFMM_FILE_FULL_PATH_MAX_LEN;
	PIC_SOURCE_TYPE type =PICVIEW_SOURCE_NONE;
#if defined(CMCC_UI_STYLE)
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};
#endif
        
    //get all picture file suffix
    GetAllPicSuffix(&file_filter);

#ifndef BAIDU_DRIVE_SUPPORT
    if (MMIAPIDC_IsOpened())
    {
        MMK_SendMsg(win_id, MSG_PICVIEWER_LOAD_CONTINUE, PNULL);
    }
#endif

#ifndef MMI_PICVIEW_MINI_SUPPORT
#ifndef BAIDU_DRIVE_SUPPORT
    else
#endif
    {
#ifdef BAIDU_DRIVE_SUPPORT
		if(PICVIEW_SOURCE_CLOUD_THUMBNAIL== MMIAPIPICVIEW_GetShowFolderPath())
		{
			is_need_download_thumbnails = TRUE;
			MMISRV_BAIDUDRV_CreateTask(MMI_MODULE_PIC_VIEWER , BAIDU_DRV_TYPE_DOWNLOAD_THUMBNAIL);
		}
		else
#endif			
		{
        MMIAPIPICVIEW_GetCurPicFileInfo(TRUE,&file_info_ptr);
        //获取当前文件夹
        MMIAPIFMM_SplitFullPathExt(file_info_ptr.filename,MMIAPICOM_Wstrlen(file_info_ptr.filename),abs_default_path, &abs_default_path_length, PNULL, PNULL, PNULL, PNULL);
        //分离出的路径最后一个\.字符  
        if(MMIFILE_SLASH==abs_default_path[abs_default_path_length-1])
        {
            abs_default_path[abs_default_path_length-1]=0;
            abs_default_path_length--;
		}
        path_ptr =abs_default_path;
        path_len =abs_default_path_length;
        
        MMIAPIFMM_SearchFileInPathAndSort(path_ptr,
                                path_len,
                                &file_filter,
                                is_recurse,
                                FUNC_FIND_FILE,
                                MMIAPIPICVIEW_GetAllPicArray(),
                                win_id,
                                MSG_PICVIEWER_LOAD_FINISH,
                                FILEARRAY_SORT_TIME_CREATE_DESCEND);
     }
     }
#else
     MMIAPIFMM_SearchFileInPathAndSort(PNULL,
                            path_len,
                            &file_filter,
                            TRUE,
                            FUNC_FIND_FILE,
                            MMIAPIPICVIEW_GetAllPicArray(),
                            win_id,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_TIME_ASCEND);
#endif
    //set softkey
    MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);

    result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

    return (result);
}

PUBLIC void PicviewListAllEmptyShow( MMI_WIN_ID_T   win_id)
{

    MMI_CONTROL_CREATE_T    create = {0};
    //GUILABEL_INIT_DATA_T    init_data = {0};
   //GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
  //  GUITEXT_INIT_DATA_T init_data = {0};
    GUILIST_INIT_DATA_T     list_init_data = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T string = {0}; 
    uint16    index= 0;
    MMI_CTRL_ID_T ctrl_id = MMIPICVIEW_LIST_EMPTY_CTRL_ID;
    GUI_BG_T        pic_bg = {0};
    GUI_COLOR_T     font_color = MMI_BLUE_COLOR;
    GUI_FONT_T      font=MMIPICPREVIEW_TITLE_FONT;
    GUILIST_EMPTY_INFO_T empty_info = {0};


    GUI_LCD_DEV_INFO    dev_info    = {0};
    MMI_STRING_T        str_data    = {0};
    GUISTR_STYLE_T      style_info  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_RECT_T          disp_rect   = {0};

#ifndef BAIDU_DRIVE_SUPPORT
    list_init_data.both_rect = MMIPICVIEW_GetClientWinRect(win_id);
#else
	list_init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
#endif
    
    list_init_data.type      = GUILIST_TEXTLIST_E;
    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_LIST_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &list_init_data;
    MMK_CreateControl(&create); 
    MMK_SetAtvCtrl(win_id, ctrl_id);
    SCI_TRACE_LOW("MMIPICVIEW  PicviewListAllEmptyShow");

    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_COMMON_COMMON_BACK, RIGHT_BUTTON, FALSE);
    if(MMIPICVIEW_GetEnterType() == MMIPICVIEW_FROM_NONE){
        GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OPTION, LEFT_BUTTON, FALSE);
    }else{
        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);

    }
    if(MMIAPIPICVIEW_GetShowFolderPath() != PICVIEW_SOURCE_NONE
        ||MMIAPIDC_IsOpened()){
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, MIDDLE_BUTTON, FALSE);
    }else{
        if((MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_MMS) ||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_VIDEOS_FROM_MMS) ||
            (MMIPICVIEW_GetEnterType() == MMIPICVIEW_PHOTOS_FROM_CAMERA)){
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
        }else{
                GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_MAIN_CAMERA, MIDDLE_BUTTON, FALSE);
        }
    }
    empty_info.text_id = TXT_COMMON_COMMON_EMPTY_LIST;
    GUILIST_SetEmptyInfo(ctrl_id, &empty_info);

}


/*****************************************************************************/
//  Description : MMIPICVIEW_IsVideoSuffix
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsVideoSuffix( FILEARRAY_DATA_T *fileinfo)
{
    uint16 suffix_len = MMIFMM_FILENAME_LEN;
    uint32 type =0;
    wchar list_suffix_name[MMIFMM_FILENAME_LEN+1] = {0};

    if (MMIAPIFMM_SplitFileName (fileinfo->filename, fileinfo->name_len, PNULL, PNULL, list_suffix_name, &suffix_len))
    {
        type = MMIAPIFMM_GetFileType (list_suffix_name, suffix_len);
        if((type!=MMIFMM_MOVIE_MP4)
            && (type!=MMIFMM_MOVIE_3GP)
            && (type!=MMIFMM_MOVIE_RMVB)
            && (type!=MMIFMM_MOVIE_AVI)
            && (type!=MMIFMM_MOVIE_FLV)){
            return FALSE;
        }
        return TRUE;

    } else{
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : get all picture files suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetAllPicSuffix(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetAllPicSuffix filter_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1695_112_2_18_2_46_24_14,(uint8*)"");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIPICVIEW_PIC_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_mmipicview_all_suffix[i], sizeof(s_mmipicview_all_suffix[i]),
                   sizeof(s_mmipicview_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_mmipicview_all_suffix[i]); /*lint !e64*/
    }
}

/*****************************************************************************/
//  Description : finish load picture files
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void FinishLoadPic(
    MMI_WIN_ID_T   win_id
)
{
    uint16      cur_index = 0;
    uint32      pic_num = 0;

    pic_num = MMIPICVIEW_GetAllPicNum();
    if (0 < pic_num)
    {
        if (MMIAPIDC_IsOpened())
        {
            cur_index = (uint16)MMIAPIDC_GetSavePhotoIndex(pic_num, MMIAPIPICVIEW_GetAllPicArray());
        }
    }
	else
	{
#ifdef PIC_PREVIEW_U200_SUPPORT
        GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_SLIDE_PREVIEW, TRUE, PNULL, TRUE);
        GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_MARKED, TRUE, PNULL, TRUE);
#else
		GUIWIN_SetSoftkeyBtnGray(MMIPICVIEW_LIST_WIN_ID, LEFT_BUTTON, TRUE);	
        GUIWIN_SetSoftkeyTextId(MMIPICVIEW_LIST_WIN_ID, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
        MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
        MMIPICVIEW_SetListEmpty(TRUE);
	}

    //need update list
    MMIPICVIEW_PostUpdateMsg(cur_index);

    MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description : post update list msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_PostUpdateMsg(
    uint16 index
)
{
    MMI_WIN_ID_T    win_id = MMIPICVIEW_LIST_WIN_ID;

    MMK_PostMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &index, sizeof(uint16));
}

/*****************************************************************************/
//  Description : clear pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearPicList(
    MMI_WIN_ID_T   win_id,  //in:
    MMI_CTRL_ID_T  ctrl_id  //in
)
{
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ClearPicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1765_112_2_18_2_46_24_15,(uint8*)"d", view_type);

    //set list enable mark
    MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, 0, TRUE);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, 0);
        break;
    }

    //set title
    //GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW, FALSE);

    //set softkey
    //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
}

/*****************************************************************************/
//  Description : update pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void UpdatePicList(
    MMI_WIN_ID_T   win_id,     //in:
    MMI_CTRL_ID_T  ctrl_id,    //in
    uint16         cur_index   //in
)
{
    uint16                  i = 0;
    uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNum();
    GUILIST_ITEM_T          item = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1809_112_2_18_2_46_24_16,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //set max item
        GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);

        //st title
        GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);

        if (0 < total_num)
        {
            //set title index
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);

            //set item
            item.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
            for (i = 0; i < total_num; i++)
            {
                GUILIST_AppendItem(ctrl_id, &item);
            }

            //set current index
            GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        }
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        //MMIAPIPICVIEW_DeleteOrderInTime(ctrl_id);

        //set max icon
        GUIICONLIST_SetTotalIcon(ctrl_id, total_num);
        //set title
        if (0 == total_num)
        {
#ifndef MMI_GUI_STYLE_MINISCREEN
            GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);
#else
            /*MINI UI 需求*/
            GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW_TITLE, FALSE);
#endif
        }

        //set current index
        GUIICONLIST_SetCurIconIndex(cur_index, ctrl_id);
        LoadPicList(ctrl_id);
       //SetlistOrderInTime(ctrl_id);
        break;
    }

//     //set softkey
//     if (0 == total_num)
//     {
//         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
//     }
//     else
//     {
//         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_PREVIEW, STXT_RETURN, FALSE);
//     }
}

/*****************************************************************************/
//  Description : append picture list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicListItem(
    BOOLEAN         is_text,        //in:是否添加text data
    uint16          item_index,     //in:item索引
    uint16          content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T   ctrl_id         //in:control id
)
{
    BOOLEAN             result = FALSE;
    wchar               temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar               *file_name_ptr = PNULL;
    uint16              file_name_len = 0;
    FILEARRAY_DATA_T    file_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index, &file_info))
    {
        //set softkey
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = STXT_PREVIEW;
        item_data.softkey_id[2] = STXT_RETURN;

        //icon
#ifdef DRM_SUPPORT
        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            MMI_IMAGE_ID_T  icon_id = IMAGE_NULL;

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                icon_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                icon_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = icon_id;
        }
        else
#endif
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
            anim_path.full_path_wstr_ptr = file_info.filename;
            anim_path.full_path_wstr_len = file_info.name_len;
        }

        //alloc file name mem
        file_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof(wchar));
        SCI_MEMSET(file_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));

        //split file name
        MMIAPIFMM_SplitFullPath(file_info.filename,
                              file_info.name_len,
                              PNULL,
                              PNULL,
                              PNULL,
                              PNULL,
                              file_name_ptr,
                              &file_name_len);

        //text1,file name
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name_ptr;

        //text2,file size
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        MMIAPIFMM_GetFileSizeString(file_info.size, temp_wstr, MMIFMM_SIZE_STR_LEN, TRUE);
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(item_data.item_content[2].item_data.text_buffer.wstr_ptr);

        if (is_text)
        {
            //add item text etc.
            result = GUILIST_SetItemData(ctrl_id,
                                         &item_data,
                                         item_index);
        }
        else
        {
            //add item anim
            result = GUILIST_SetItemContent(ctrl_id,
                                            &item_data.item_content[content_index],
                                            item_index,
                                            content_index);
        }

        //free file name mem
        SCI_FREE(file_name_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : modify pictures list view
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPicListView(
    MMI_WIN_ID_T   win_id, //in:
    MMI_CTRL_ID_T  ctrl_id //in:
)
{
    uint16                  cur_index = 0;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get current index
    cur_index = MMIPICVIEW_GetCurIndex(ctrl_id);

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW ModifyPicListView view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_1990_112_2_18_2_46_25_17,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        view_type = MMIPICVIEW_VIEW_ICONLIST;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        view_type = MMIPICVIEW_VIEW_LIST;
        break;
    }

    //modify view type
    MMIPICVIEW_SetViewType(view_type);

    //destroy control
    MMK_DestroyControl(ctrl_id);

    //creat control
    CreatePicListCtrl(win_id, ctrl_id);

    //need update list
    MMIPICVIEW_PostUpdateMsg(cur_index);
}

/*****************************************************************************/
//  Description : append picture icon list one item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL BOOLEAN AppendPicIconListItem(
//     uint16          index,
//     MMI_CTRL_ID_T   ctrl_id
// )
// {
//     BOOLEAN                 result = FALSE;
//     uint16                  total_num = (uint16)MMIPICVIEW_GetAllPicNum();
//     wchar                   *file_name_ptr = PNULL;
//     uint16                  file_name_len = 0;
//     FILEARRAY_DATA_T        file_info = {0};
//     GUIICONLIST_DISP_T      item = {0};
// 
//     if (0 < total_num)
//     {
//         //get picture info
//         if (MMIPICVIEW_GetPicFileInfo(index, &file_info))
//         {
//             //alloc file name mem
//             file_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof(wchar));
//             SCI_MEMSET(file_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
// 
//             //split file name
//             MMIAPIFMM_SplitFullPath(file_info.filename,
//                                   file_info.name_len,
//                                   PNULL,
//                                   PNULL,
//                                   PNULL,
//                                   PNULL,
//                                   file_name_ptr,
//                                   &file_name_len);
// 
//             //set title
//             item.name_str.wstr_ptr = file_name_ptr;
//             item.name_str.wstr_len = file_name_len;
// 
//             item.softkey_id[0] = STXT_OPTION;
//             item.softkey_id[1] = STXT_PREVIEW;
//             item.softkey_id[2] = STXT_RETURN;
// 
//             //append item
//             GUIICONLIST_AppendText(index, ctrl_id, &item);
// 
//             //free file name mem
//             SCI_FREE(file_name_ptr);
// 
//             result = TRUE;
//         }
//     }
// 
//     return (result);
// }

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(index, &file_info))
    {
#ifdef DRM_SUPPORT
        GUIANIM_DATA_INFO_T data_info = {0};

        if (MMIPICVIEW_IsDrmFile(file_info.filename, file_info.name_len))
        {
            //is it has right
            if (MMIPICVIEW_IsDrmRight(file_info.filename, file_info.name_len))
            {
                data_info.img_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_DRM_PIC_LOCK_ICON;
            }

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, &data_info, PNULL);
        }
        else
#endif
        {
            //set file name
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle pictures list web key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListWeb(
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN     is_enable_mark = FALSE;
    uint16      mark_num = 0;
//#ifdef DRM_SUPPORT
    uint16	item_index = 0;
//#endif

    if (0 < MMIPICVIEW_GetAllPicNum())
    {
        //get mark status
        is_enable_mark = MMIPICVIEW_GetListMarkable(ctrl_id);
        if (is_enable_mark)
        {
            //get mark num
            mark_num = MMIPICVIEW_GetSelectItemIndex(ctrl_id, PNULL, 0);
            if (0 == mark_num)
            {
                //set list not enable mark
                MMIPICVIEW_SetListMarkable(ctrl_id, FALSE);
            }
        }


        item_index = MMIPICVIEW_GetCurIndex(ctrl_id);
#ifdef DRM_SUPPORT
        if (MMIPICVIEW_IsPreview(TRUE, item_index))
#endif
        {
            //win_id = MMK_GetFocusWinId();
#ifdef BAIDU_DRIVE_SUPPORT
			FILEARRAY_DATA_T    file_info = {0};
    
			if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
			{
				if(!MMISRV_BAIDUDRV_is_in_BaiduFolder(PICVIEW_SOURCE_CLOUD_THUMBNAIL , file_info.filename , file_info.name_len))
				{
					if (MMIPICVIEW_isFileError(ctrl_id, item_index, MMIPICVIEW_PICOPE_POINT))
					{
						return;
					}				
				}
			}
#else
            if (MMIPICVIEW_isFileError(ctrl_id, item_index, MMIPICVIEW_PICOPE_POINT))
            {
                return;
            }
#endif			
            //GUIICONLIST_PrepareSpecial(ctrl_id, 0);

#ifdef PIC_GLIDE_SUPPORT
            MMK_CreateWin((uint32 *)MMIPICVIEW_GLIDE_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
#endif
        }
    }
}

/*****************************************************************************/
//  Description : get current index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetCurIndex(
    MMI_CTRL_ID_T  ctrl_id
)
{
    uint16                      cur_index = 0;
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_GetCurIndex view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2190_112_2_18_2_46_25_18,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
        break;
    }

    return (cur_index);
}

/*****************************************************************************/
//  Description : set current index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetCurIndex(
    MMI_CTRL_ID_T    ctrl_id,
    uint16           cur_index
)
{
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetCurIndex view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2221_112_2_18_2_46_25_19,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        GUILIST_SetCurItemIndex(ctrl_id, cur_index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        GUIICONLIST_SetCurIconIndex(cur_index, ctrl_id);
        break;
    }
}

/*****************************************************************************/
//  Description : set max selected number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetMaxSelected(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         selected_max_num
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2251_112_2_18_2_46_25_20,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetMaxSelectedItem(ctrl_id, selected_max_num);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetMaxSelectedItem(ctrl_id, selected_max_num);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get list markable
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetListMarkable(
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2282_112_2_18_2_46_25_21,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_GetListStateById(ctrl_id, GUILIST_STATE_ENABLE_MARK);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_GetMarkable(ctrl_id);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set list markable
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetListMarkable(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_markable
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetListMarkable view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2314_112_2_18_2_46_25_22,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetTextListMarkable(ctrl_id, is_markable);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetMarkable(ctrl_id, is_markable);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : item is select
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSelectItem(
    MMI_CTRL_ID_T    ctrl_id,
    uint16           index
)
{
    BOOLEAN                 result = FALSE;
    const GUILIST_ITEM_T    *item_ptr = PNULL;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2347_112_2_18_2_46_25_23,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //get item pointer
        item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);

        //current item is mark
        result = GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_IsSelectedItem(ctrl_id, index);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get selected item index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetSelectItemIndex(//selected number
    MMI_CTRL_ID_T   ctrl_id,    //in:
    uint16          *index_ptr, //in/out:may PNULL
    uint16		    array_len   //in:
)
{
    uint16                  select_num = 0;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2384_112_2_18_2_46_25_24,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        select_num = GUILIST_GetSelectedItemIndex(ctrl_id, index_ptr, array_len);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        select_num = GUIICONLIST_GetSelectedItemIndex(ctrl_id, index_ptr, array_len);
        break;
    }

    return (select_num);
}

/*****************************************************************************/
//  Description : set selected item
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetSelectedItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index,
    BOOLEAN       is_selected
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2417_112_2_18_2_46_25_25,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_SetSelectedItem(ctrl_id, index, is_selected);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetSelectedItem(ctrl_id, index, is_selected);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetAllSelected(
    MMI_CTRL_ID_T  ctrl_id,
    BOOLEAN        is_selected
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetAllSelected view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2449_112_2_18_2_46_25_26,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        GUILIST_SetAllSelected(ctrl_id, is_selected);
        result = TRUE;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_SetAllSelected(ctrl_id, is_selected);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_RemoveItem(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW UpdatePicList view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2482_112_2_18_2_46_25_27,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        result = GUILIST_RemoveItem(ctrl_id, index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        result = GUIICONLIST_RemoveItem(ctrl_id, index);
        break;
    }

    return (result);
}
#if defined(DRM_SUPPORT)//norflashcut_pic
/*****************************************************************************/
//  Description : modify item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_ModifyItemIcon view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2514_112_2_18_2_46_26_28,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        ModifyListItemIcon(ctrl_id, index);
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
        result = AppendPicIconListIcon(index, ctrl_id);
        break;

    default:
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
//  Description : modify list item icon
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ModifyListItemIcon(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
#ifdef DRM_SUPPORT
    const GUILIST_ITEM_T    *item_ptr = PNULL;
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};

    //get item
    item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
    if (PNULL != item_ptr)
    {
        //copy item
        MMI_MEMCPY(&item_t, sizeof(GUILIST_ITEM_T),
                   item_ptr, sizeof(GUILIST_ITEM_T),
                   sizeof(GUILIST_ITEM_T));

        MMI_MEMCPY(&item_data, sizeof(GUILIST_ITEM_DATA_T),
                   item_ptr->item_data_ptr, sizeof(GUILIST_ITEM_DATA_T),
                   sizeof(GUILIST_ITEM_DATA_T));

        //set item
        item_t.item_data_ptr = &item_data;
        if (IMAGE_DRM_PIC_LOCK_ICON != item_data.item_content[0].item_data.image_id)
        {
            item_data.item_content[0].item_data.image_id = IMAGE_DRM_PIC_LOCK_ICON;

            //set item
            GUILIST_ReplaceItem(ctrl_id, &item_t, index);

            result = TRUE;
        }
    }
#endif

    return (result);
}

/*****************************************************************************/
//  Description : modify item text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemText(
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_ModifyItemText view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_LIST_2594_112_2_18_2_46_26_29,(uint8*)"d", view_type);

    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        //need update list
        MMIPICVIEW_PostUpdateMsg(index);
        result = TRUE;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
//         result = AppendPicIconListItem(index, ctrl_id);
        break;

    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : reload picture
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ReloadFile(void)
{
    //reload file
    s_is_reload_file = TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicFolderWinOpen(void)
{
    return MMK_IsOpenWin(MMIPICVIEW_PICFOLDER_WIN_ID);
}

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T MMIPICVIEW_GetClientWinRect(MMI_WIN_ID_T  win_id)
{
    GUI_BOTH_RECT_T     client_rect = MMITHEME_GetWinClientBothRect(win_id);
    client_rect.v_rect.top     += MMIPICPREVIEW_TITLE_HEIGHT;
#ifdef  PIC_PREVIEW_U200_SUPPORT
    client_rect.v_rect.bottom  -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    client_rect.v_rect.bottom = MAX(client_rect.v_rect.bottom, client_rect.v_rect.top);
#endif    
    client_rect.h_rect.top     += MMIPICPREVIEW_TITLE_HEIGHT;
#ifdef  PIC_PREVIEW_U200_SUPPORT
    client_rect.h_rect.bottom  -= MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    client_rect.h_rect.bottom = MAX(client_rect.h_rect.bottom, client_rect.h_rect.top);
#endif
    return client_rect;
}
#ifndef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
//  Description : set title for pic list 
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPicListTitle(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T return_id)
{
    GUI_BOTH_RECT_T     title_rect   = MMITHEME_GetWinClientBothRect(win_id);    
    GUI_BOTH_RECT_T     button_rect  = title_rect;
    uint16              title_height = MMIPICPREVIEW_TITLE_HEIGHT;
	GUI_BG_T            bg          = {0};
    MMI_STRING_T        title_txt   = {0};
    GUISTR_STYLE_T      txt_style   = {0};
    GUISTR_STATE_T      txt_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_BOLD;
    uint16              img_width   = 0;
    uint16              img_height  = 0;
    uint16              button_wid  = 0;

#ifdef  PIC_PREVIEW_U200_SUPPORT
    //label rect,txt,font....
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICVIEW_RETURN, win_id);
#endif
//     button_wid = 2 * MMIPICPREVIEW_TITLE_MARGIN + img_width;

    title_rect.v_rect.bottom = title_rect.v_rect.top + title_height - 1;
    title_rect.h_rect.bottom = title_rect.h_rect.top + title_height - 1;
    title_rect.v_rect.left   = MMIPICPREVIEW_TITLE_MARGIN;
    title_rect.h_rect.left   = MMIPICPREVIEW_TITLE_MARGIN;

    if(PNULL != return_id)
    {
        button_wid = 2 * MMIPICPREVIEW_TITLE_MARGIN + img_width;
        //return button
        button_rect.v_rect.top      = title_rect.v_rect.top;
        button_rect.h_rect.top      = title_rect.h_rect.top;
        button_rect.v_rect.bottom   = title_rect.v_rect.bottom;
        button_rect.h_rect.bottom   = title_rect.h_rect.bottom;
        button_rect.v_rect.right    -= MMIPICPREVIEW_TITLE_MARGIN;
        button_rect.h_rect.right    -= MMIPICPREVIEW_TITLE_MARGIN;
        button_rect.v_rect.left     = button_rect.v_rect.right - button_wid + 1;
        button_rect.h_rect.left     = button_rect.h_rect.right - button_wid + 1;
        GUIAPICTRL_SetBothRect(return_id, &button_rect);
    }
    if(PNULL != ctrl_id)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_PIC, &title_txt);

        bg.bg_type  = GUI_BG_NONE;
        
        txt_style.font      = MMIPICPREVIEW_TITLE_FONT;
        txt_style.font_color= MMI_WHITE_COLOR;
        txt_style.align     = ALIGN_LEFT;//水平剧居中,垂直方向居中

		GUIAPICTRL_SetState(ctrl_id,GUICTRL_STATE_DISABLE_ACTIVE,TRUE);

        GUILABEL_SetBg(ctrl_id, &bg);
        GUILABEL_SetFont(ctrl_id, MMIPICPREVIEW_TITLE_FONT, MMI_WHITE_COLOR);
        GUILABEL_SetText(ctrl_id, &title_txt, FALSE);

        button_wid = GUISTR_GetStringWidth(&txt_style, &title_txt, txt_state) + MMIPICPREVIEW_TITLE_MARGIN;
        title_rect.v_rect.right  = title_rect.v_rect.left + button_wid - 1;
        title_rect.h_rect.right  = title_rect.h_rect.left + button_wid - 1;
        if (PNULL != return_id)
        {
            title_rect.v_rect.right  = MIN(button_rect.v_rect.left, title_rect.v_rect.right);
            title_rect.h_rect.right  = MIN(button_rect.h_rect.left, title_rect.h_rect.right);
        }
        else
        {
            title_rect.v_rect.right  = MIN(button_rect.v_rect.right, title_rect.v_rect.right);
            title_rect.h_rect.right  = MIN(button_rect.h_rect.right, title_rect.h_rect.right);
        }
        GUIAPICTRL_SetBothRect(ctrl_id, &title_rect);
    }
}
#endif
/*****************************************************************************/
// 	Description : set iconlist param for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_folder_list)
{
    GUI_BG_T            ctrl_bg     = {0};
    
    uint16              icon_width  = MMIPICPREVIEW_LIST_ICON_SIZE;
    GUIICONLIST_MARGINSPACE_INFO_T  icon_space = {0};
#ifndef BAIDU_DRIVE_SUPPORT
    GUI_BOTH_RECT_T     icon_rect   = MMIPICVIEW_GetClientWinRect(win_id);
#else
	//GUI_BOTH_RECT_T         icon_rect = MMITHEME_GetWinClientBothRect(win_id);
	GUI_BOTH_RECT_T         icon_rect = MMITHEME_GetFullScreenBothRect();
	icon_rect.h_rect.top = icon_rect.v_rect.top = (icon_rect.v_rect.bottom - icon_rect.v_rect.top+1 - MMI_PIC_PREVIEW_HEIGHT) / 2; 
	icon_rect.h_rect.bottom = icon_rect.v_rect.bottom = icon_rect.v_rect.top + MMI_PIC_PREVIEW_HEIGHT-1;
	icon_rect.h_rect.left = icon_rect.v_rect.left = (icon_rect.v_rect.right - icon_rect.v_rect.left - MMI_PIC_PREVIEW_WIDTH) / 2 ;
	icon_rect.h_rect.right = icon_rect.v_rect.right = icon_rect.v_rect.left +MMI_PIC_PREVIEW_WIDTH -1 ;  
#endif


    //设置iconlist的范围
    GUIAPICTRL_SetBothRect(ctrl_id, &icon_rect);
        
    //设置背景
    ctrl_bg.bg_type         = GUI_BG_COLOR;
    ctrl_bg.is_screen_img   = TRUE;
    ctrl_bg.color       = MMI_BLACK_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &ctrl_bg);

    if(!is_folder_list)
    {
        //设置每个icon的高、宽距
#ifdef BAIDU_DRIVE_SUPPORT
		icon_space.iconlist_hmargin = 0;
#else
        icon_space.iconlist_hmargin = MMIPICPREVIEW_LIST_ICON_MARGIN;
#endif
        icon_space.iconlist_vmargin = MMIPICPREVIEW_LIST_ICON_MARGIN;
        icon_space.icon_item_hspace = MMIPICPREVIEW_LIST_ICON_MARGIN;
        icon_space.icon_item_vspace = MMIPICPREVIEW_LIST_ICON_MARGIN;
        GUIICONLIST_SetIconItemSpace(ctrl_id, icon_space);
        GUIICONLIST_SetIconWidthHeight(ctrl_id, icon_width, icon_width);
    }
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

#ifdef  PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set spftkey param for pic list
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetButtonFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_IMAGE_ID_T* img_id, uint16 button_num)
{
    BOOLEAN             result      = TRUE;
    GUI_BG_T            bg_info     = {0};
    GUI_BG_T            fg_info     = {0};
    GUI_BOTH_RECT_T     form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T     button_rect = {0};
    uint16              button_height   = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    uint16              button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16              button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
    int                 i = 0;
    
    if(button_num > 0)
    {
        form_rect.v_rect.top = form_rect.v_rect.bottom - button_height + 1;
        form_rect.h_rect.top = form_rect.h_rect.bottom - button_height + 1;
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = IMAGE_PICVIEW_BUTTON;
		
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
        button_rect       = form_rect;

        fg_info.bg_type = GUI_BG_IMG;
        for(i = 0; i < button_num; i++)
        {
            button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
            button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
            button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
            button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;
            GUIAPICTRL_SetBothRect(ctrl_id[i], &button_rect);

            fg_info.img_id = img_id[i];
            GUIBUTTON_SetBg(ctrl_id[i], &bg_info);
            GUIBUTTON_SetFg(ctrl_id[i], &fg_info);
            GUIBUTTON_PermitBorder(ctrl_id[i], FALSE);
        }
    }
    else
    {
        result = FALSE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetButtonsVisible(BOOLEAN is_visible)
{
    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_SLIDE_PREVIEW, is_visible, FALSE);
    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_MARKED, is_visible, FALSE);    
}

/*****************************************************************************/
// 	Description : handle after point the mark menu
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void InitMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create_ptr  = {0};
    GUIBUTTON_INIT_DATA_T   init_data   = {0};
    GUI_BOTH_RECT_T         form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T         button_rect = form_rect;
    uint16                  button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16                  button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
//    GUI_BG_T                button_bg      = {0};
    GUI_FONT_ALL_T          button_font_all = {0};//button的字体与颜色
    MMI_CTRL_ID_T           button_id[] = {MMIPICVIEW_MARK_ALLMARK_CTRL_ID, MMIPICVIEW_MARK_DELETE_CTRL_ID, MMIPICVIEW_MARK_CANCEL_CTRL_ID};//
    MMI_TEXT_ID_T           txt_id[]    = {TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL};// 
    uint8                   button_num  = 3;
    uint8                   i;

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;
        
        button_rect.v_rect.top = button_rect.v_rect.bottom - MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
        button_rect.h_rect.top = button_rect.h_rect.bottom - MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
                 
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
                
        init_data.bg.bg_type    = GUI_BG_IMG;
        init_data.bg.img_id     = IMAGE_PICVIEW_BUTTON;
        button_font_all.font    = MMI_DEFAULT_NORMAL_FONT;
        button_font_all.color   = MMI_WHITE_COLOR;
        for(i = 0; i < button_num; i++)
        {
            button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
            button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
            button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
            button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;

            if(i == (button_num - 1))
            {
                button_rect.v_rect.right = MAX(button_rect.v_rect.right, form_rect.v_rect.right);
                button_rect.h_rect.right = MAX(button_rect.h_rect.right, form_rect.h_rect.right);
            }
            init_data.both_rect = button_rect;
            create_ptr.ctrl_id = button_id[i];
            MMK_CreateControl(&create_ptr);
            
            GUIBUTTON_SetFont(button_id[i], &button_font_all);
            GUIBUTTON_SetTextId(button_id[i], txt_id[i]);
            GUIBUTTON_PermitBorder(button_id[i], FALSE);
        }
    }
}

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    if(PNULL != MMK_GetCtrlPtrByWin(win_id, MMIPICVIEW_MARK_ALLMARK_CTRL_ID))
    {
        MMK_DestroyControl(MMIPICVIEW_MARK_ALLMARK_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_MARK_DELETE_CTRL_ID);
        MMK_DestroyControl(MMIPICVIEW_MARK_CANCEL_CTRL_ID);
    }
}
#endif

/*****************************************************************************/
// 	Description : set the iconlist groups orded in time
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void SetlistOrderInTime(MMI_CTRL_ID_T ctrl_id)
{
    MMISET_DATE_DISPLAY_TYPE_E      date_type   = MMISET_DATE_YMD;
    GUIICONLIST_DELIMITER_INFO_T*   group_info_ptr  = {0};
    FILEARRAY_DATA_T                file_info   = {0};
    uint32                          pic_num     = MMIPICVIEW_GetAllPicNum();
    SCI_DATE_T                      tmp_date    = {0};   //DT??è??ú
    char                            tmp_char[9] = {0};
    uint8                           str_len     = 0;
    GUI_BG_T                        bg_ptr      = {0};
    GUI_FONT_ALL_T                  txt_style   = {0};
    uint16                          txt_left_space = 0;
    uint32                          i,j;
    wchar 							tmp_str[9]  = {0};
	DELIMITER_BG_SPACE_T    		bg_space	= {0};
    uint32                          start_index = 0;
    FILEARRAY                       file_array  = MMIAPIPICVIEW_GetCurPicArray();

    SCI_TRACE_LOW("[mmipicview] SetlistOrderInTime all_pic_num = %d", pic_num);
    if (pic_num > 0)
    {
        date_type = MMIAPISET_GetDateDisplayType();

        MMIVIRTUALARRAY_SetBufferSize(file_array,500);
#ifndef MMI_PICVIEW_MINI_SUPPORT
        start_index = MMIAPIPICVIEW_GetFirstPos(file_array);
#endif
        SCI_TRACE_LOW("[mmipicview] SetlistOrderInTime start_index = %d", start_index);

        group_info_ptr = (GUIICONLIST_DELIMITER_INFO_T *)SCI_ALLOC_APP(sizeof(GUIICONLIST_DELIMITER_INFO_T) );
        SCI_MEMSET(group_info_ptr, 0, sizeof(GUIICONLIST_DELIMITER_INFO_T) );
        
        //get all the time info of all pics
        for(i = 0, j = 0; (i < pic_num) && (j < pic_num); i++)
        {
            if(!MMIAPIFILEARRAY_Read(file_array, (start_index + i), &file_info))
            {
                break;
            }
            if((file_info.create_date.year !=  tmp_date.year)
                || (file_info.create_date.mon != tmp_date.mon))
            {
                
                tmp_date.year = file_info.create_date.year;
                tmp_date.mon  = file_info.create_date.mon;
                
                if(MMISET_DATE_YMD == date_type)
                {
                    sprintf(tmp_char, "%04d.%02d:", tmp_date.year, tmp_date.mon);
                }
                else
                {
                    sprintf(tmp_char, "%02d.%04d:", tmp_date.mon, tmp_date.year);
                }
                str_len = strlen(tmp_char);
                MMI_STRNTOWSTR(tmp_str, 8, (uint8 *)tmp_char, 8, str_len);
                group_info_ptr->delimiter_str_info.wstr_ptr = tmp_str;
                group_info_ptr->delimiter_str_info.wstr_len = str_len;
                group_info_ptr->start_item_index = i;
                GUIICONLIST_CreateGroup(ctrl_id, group_info_ptr);
                j++;
            }
        }
        		
        txt_style.color = MMIPICVIEW_TITLE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_SELECT);
        txt_style.font  = MMI_DEFAULT_NORMAL_FONT;
        txt_left_space  = 10;
        GUIICONLIST_SetDelimiterStrInfo(ctrl_id, &txt_style, txt_left_space);
#ifndef MMI_GUI_STYLE_MINISCREEN
        bg_ptr.bg_type  = GUI_BG_IMG;//GUI_BG_NONE;
        bg_ptr.img_id	= IMAGE_PICVIEW_OSD_BG; 
#else
        //设置背景
        bg_ptr.bg_type = GUI_BG_IMG; 
        bg_ptr.img_id  = IMAGE_PICVIEW_TIME_BG;
#endif
        GUIICONLIST_SetDelimiterBg(ctrl_id, &bg_ptr);
#ifndef MMI_GUI_STYLE_MINISCREEN
  {
    	bg_space.height = GUI_GetStringHeight(txt_style.font, tmp_str, 8)+ 5;
		bg_space.width	= GUI_GetStringWidth(txt_style.font, tmp_str, 8) + 8;
		bg_space.left	= txt_left_space;
		bg_space.top	= 5;
  }
#else
  { 
        //IGUICTRL_T  *iconlist_ctrl_ptr = MMK_GetCtrlPtr(ctrl_id);
        bg_space.top = 0;
        bg_space.height = MMIPICPREVIEW_TIME_HEIGHT;
        bg_space.left = 0;
        //if(PNULL != iconlist_ctrl_ptr)
        {  
            /*判断是否需要显示滚动条*/
			if(GUIICONLIST_GetRelativeHeight( ctrl_id) > GUIICONLIST_GetDispHeight( ctrl_id))
            {
                bg_space.width = MMITHEME_GetClientRectWidth() - MMITHEME_GetScrollBarWidth() - MMIPICVIEW_DELIMETER_MARGIN;
            }
            else
            {
                bg_space.width = MMITHEME_GetClientRectWidth();
            }
        }
         //128X160 DelimiterHeight 与背景图片同高
        GUIICONLIST_SetDelimiterHeight(ctrl_id, MMIPICPREVIEW_TIME_HEIGHT); 
        /*设置背景图片的显示区域*/
  }
#endif
        GUIICONLIST_SetDelimiterBgSpace(ctrl_id, &bg_space);
        
        SCI_FREE(group_info_ptr);
        MMIVIRTUALARRAY_SetBufferSize(file_array,50);
    }
    MMK_PostMsg(MMIPICVIEW_WAIT_WIN_ID, MSG_PICLIST_LOAD_END, (DPARAM)PNULL, (uint32)0);
}

/*****************************************************************************/
// 	Description : refresh the iconlist groups orded in time
//	Global resource dependence : 
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeleteOrderInTime(MMI_CTRL_ID_T ctrl_id)
{
    uint16 group_num = GUIICONLIST_GetDelimiterGroupNum(ctrl_id);
//    uint16 i = 0;
    if(group_num > 0)
    {
//         for(i = group_num; i > 0; i--)
//         {
//             GUIICONLIST_RemoveGroup(ctrl_id, i - 1);
//         }
		GUIICONLIST_RemoveAllDelimiter(ctrl_id);
    }
}

/*****************************************************************************/
// 	Description : display the default img for picview
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
LOCAL void DisplayBackgroudImg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          disp_rect   = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    dev_info    = {0};

    //GUIRES_DisplayImg(PNULL, &disp_rect, &disp_rect, win_id, PICVIEW_DEFAULT_BG_IMG, &dev_info);
	GUI_FillRect(&dev_info, disp_rect, MMI_BLACK_COLOR);
}

/*****************************************************************************/
//  Discription: display when list win is empty
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
LOCAL void PicviewListEmptyShow(void)
{
    MMI_WIN_ID_T        win_id      = MMIPICVIEW_LIST_WIN_ID;
    GUI_RECT_T          client_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T          disp_rect   = {0};
    MMI_IMAGE_ID_T      img_id      = IMAGE_PICVIEW_FOLDER_DEFAULT_ICON;
    uint16              img_width   = 0;
    uint16              img_height  = 0;
    GUI_LCD_DEV_INFO    dev_info    = {0};
    MMI_STRING_T        str_data    = {0};
    GUISTR_STYLE_T      style_info  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);
    disp_rect.top    = client_rect.top + ((client_rect.bottom - client_rect.top + 1) / 2 - img_height / 2); 
    disp_rect.bottom = disp_rect.top + img_height - 1;
    disp_rect.left   = client_rect.left + ((client_rect.right - client_rect.left + 1) / 2 - img_width / 2);
    disp_rect.right  = disp_rect.left + img_width - 1;
    GUIRES_DisplayImg(PNULL, &disp_rect, PNULL, win_id, img_id, &dev_info);
    
    style_info.align    = ALIGN_HMIDDLE;
    style_info.font     = SONG_FONT_14;
    style_info.font_color= MMI_GRAY_WHITE_COLOR;
    disp_rect.top       = disp_rect.bottom + 8;
    disp_rect.bottom    = disp_rect.top + 18;
    disp_rect.left      = client_rect.left;
    disp_rect.right     = client_rect.right;
    MMI_GetLabelTextByLang(TXT_PICVIEW_NOPHOTO, &str_data);
    GUISTR_DrawTextToLCDInRect(&dev_info, &disp_rect, &disp_rect, &str_data, &style_info,  state, GUISTR_TEXT_DIR_AUTO);
}

/*****************************************************************************/
// 	Description : set if the list win is empty
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetListEmpty(BOOLEAN is_empty)
{
    s_is_list_empty = is_empty;
}

/*****************************************************************************/
// 	Description : get if the list win is open form DCamera
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetifListFromDc(void)
{
	return s_is_need_to_reinit_picarray_from_piclib;
}

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoadPicListWinMsg(
                                           MMI_WIN_ID_T     win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    static BOOLEAN s_piclist_cancel_load = FALSE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMI_CTRL_ID_T     ctrl_id=0;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    ctrl_id = pubwin_info_ptr->user_data;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_PostMsg(win_id, MSG_PICLIST_LOAD_START, PNULL, 0);
        break;
        
    case MSG_PICLIST_LOAD_START:
         if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        }
        SetlistOrderInTime(ctrl_id);
        break;
        
    case MSG_PICLIST_LOAD_END:
        MMK_CloseWin(win_id);
        break;
                
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : load all picture 
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void LoadPicList(MMI_CTRL_ID_T ctrl_id)
{
    MMI_WIN_ID_T                wait_win_id = MMIPICVIEW_WAIT_WIN_ID;
    
    MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleLoadPicListWinMsg, ctrl_id);
}
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Discription: get default picture path
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void GetDefaultPicPath(MMIPICVIEW_DEFAULT_PATH_T* path_ptr)
{
    MMIFILE_DEVICE_E fmm_dev =  0;
    const wchar* dev_ptr = PNULL;
    uint16       dev_len = 0;
    
    if (PNULL == path_ptr)
    {
        SCI_TRACE_LOW("MMIPICVIEW GetDefaultPicPath path_ptr is PNULL !!!");
        return ;
    }
    
    fmm_dev = MMIAPIFMM_GetDefaultDisk();
    
    dev_ptr = MMIAPIFMM_GetDevicePath(fmm_dev);
    dev_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);
    path_ptr->path_len = MMIFMM_PATHNAME_LEN;
    
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
        MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
        PNULL, 0,
        path_ptr->path, &path_ptr->path_len);
}
#endif

// /*****************************************************************************/
// // 	Description : prepare special photo data as bg
// //	Global resource dependence :
// //  Author: yaye.jiang
// //	Note: //目前只实现用固有资源实现
// /*****************************************************************************/
// LOCAL void PrepareSpecialPhotoBgData(MMI_WIN_ID_T win_id)
// {
//     BOOLEAN             result          = FALSE;
//     GUIIMG_SRC_T        src_info        = {0};
//     GUIIMG_DISPLAY_T    img_display     = {0};
//     GUIIMG_DEC_OUT_T    img_output      = {0};
//     MMI_IMAGE_ID_T      img_id          = PICVIEW_DEFAULT_BG_IMG;
//     uint16              src_size        = 0;
//     uint16              input_width     = 0;
//     uint16              input_height    = 0;
//     uint16              target_size     = 0;
//     uint16              target_width    = 0;
//     uint16              target_height   = 0;
//     IMG_INPUT_PARAM_T   input_param     = {0};
//     GUI_LCD_DEV_INFO    lcd_info        = {0};
// 
//     GUILCD_GetLogicWidthHeight(lcd_info, &target_width, &target_height);
//     
//     target_format   = IMGREF_FORMAT_RGB565;
//     input_param.smooth_type         = IMGREF_SMOOTH_MEAN_EX;//目前只支持这一种
//     input_param.target_data_format  = target_format;
//     
//     src_size = input_height * input_width * 4;
//     res_src->imgstream_ptr = (uint8 *)SCI_ALLOC_APP(sizeof(src_size));
//     if (PNULL == res_src->imgstream_ptr)
//     {
//         SCI_TRACE_LOW("[Mmipicview_list]PrepareSpecialPhotoBgData: No Memory!");
//         return FALSE;
//     }
// 
//     SCI_MEMSET(res_src->imgstream_ptr, 0, res_src->imgstream_ptr);
//     
//     //get valid file or resource img
//     if()    //data
//     {
//         src_info.is_file        = FALSE;
//         src_info.src_buf_ptr    = (uint8 *)MMI_GetLabelImage(img_id, win_id, &(src_info.src_data_size));
//         GUIRES_GetImgWidthHeight(&input_width, &input_height, img_id, win_id);
//     }
//     else    //file
//     {
//         src_info.is_file        = TRUE;
//         src_info.full_path_ptr  = GetValidFile();
//         //如何获取file的size信息????????????????????????????????????????????????????????????
//     }
//     
//     //decode to get the input data
//     input_width     = min(input_width, target_width);
//     input_height    = min(input_height, target_height);
//     img_display.dest_width  = input_width;
//     img_display.dest_height = input_height;
//     img_output.actual_width = input_height;
//     img_output.actual_height= input_height;
//     img_output.decode_data_ptr  = res_src->imgstream_ptr;
//     img_output.decode_data_size = src_size;
//     GUIIMG_Decode(&src_info, &img_display, &img_output);
// 
//     //set the input buffer param
//     if(GUIIMG_DATE_TYPE_RGB565 == img_output.data_type)
//     {
//         input_param.src_data_format     = IMGREF_FORMAT_RGB565;
//         src_size   = input_height * input_width * 2;
//         res_src->imgstreamsize = src_size;
//     }
//     else
//     {
//         input_param.src_data_format     = IMGREF_FORMAT_ARGB888;
//         res_src->imgstreamsize = src_size;
//     }
//     input_param.src_chn.ch0.ptr     = res_src.imgstream_ptr;
//     input_param.src_size.h          = img_output.actual_height; 
//     input_param.src_size.w          = img_output.actual_width;
//     input_param.src_chn.chn0.size   = res_src.imgstreamsize;
//     
//     //set the target buffer param
//     if(IMGREF_FORMAT_ARGB888 == input_param.target_data_format )
//     {
//         target_size = target_height * target_width * 4;
//     }
//     else if(IMGREF_FORMAT_RGB565 == input_param.target_data_format)
//     {
//         target_size = target_height * target_width * 2;
//     }       
//     targ_src->imgstream_ptr = (uint8 *)SCI_ALLOC_APP(sizeof(target_size));
//     targ_src->imgstreamsize = target_size;
//     input_param.target_chn.chn0.ptr = targ_src.imgstream_ptr;
//     input_param.target_size.h       = target_height;
//     input_param.target_size.w       = target_width;
//     input_param.target_chn.chn0.size= targ_src.imgstreamsize;
//     
//     input_param.trim_rect.x = 300;
//     input_param.trim_rect.y = 300;
//     input_param.trim_rect.h = 50;
//     input_param.trim_rect.w = 100;
//     
//     ScaleAndDiffusionImage_Ex(&input_param);
// }
// 
// /*****************************************************************************/
// // 	Description : display the special photo data
// //	Global resource dependence :
// //  Author: yaye.jiang
// //	Note: 
// /*****************************************************************************/
// LOCAL BOOLEAN ShowSpecialPhotoBg(MMI_WIN_ID_T win_id)
// {
//     BOOLEAN             result          = FALSE;
//     GUIIMG_DISPLAYBMP_PARAM_T display_info  = {0};
//     GUI_LCD_DEV_INFO    lcd_info        = {0};
//     GUI_RECT_T          display_rect    = MMITHEME_GetClientRect();
//     uint16              target_width    = 0;
//     uint16              target_height   = 0;
//     
//     GUILCD_GetLogicWidthHeight(lcd_info, &target_width, &target_height);
//     
//     if(PNULL != targ_src->imgstream_ptr)
//     {
//         display_info.bitmap_ptr.bit_ptr     = targ_src->imgstream_ptr;
//         display_info.bitmap_ptr.img_height  = target_height;
//         display_info.bitmap_ptr.img_width   = target_width;
//         display_info.bitmap_ptr.date_type   = target_format;
// 
//         display_info.lcd_dev_ptr            = &lcd_info;
//         display_info.rect_ptr               = &display_rect;
//         
//         if(GUIIMG_DisplayBmpEx(display_info))
//         {
//             result = TRUE;
//         }
//     }
//     return result;
// }
// 
#ifdef MMI_GUI_STYLE_MINISCREEN
/*****************************************************************************/
// 	Description : display the total pic number
//	Global resource dependence :
//  Author: langhua.yang
//	Note: just for MAINLCD_SIZE_128X160 and MAINLCD_SIZE_176X220
/*****************************************************************************/
LOCAL void DispTotalPicNum(void)
{    
    uint8                   str[8] = {0};
    wchar                   wstr[8] = {0};
    uint16                  freq_len = 0;
    uint16                  str_len = 0;
    MMI_STRING_T            string = {0};/*lint !e64*/
    uint32                  total_num = MMIPICVIEW_GetAllPicNum();
    sprintf((char *)str, "%ld", total_num); /*lint !e64*/
    str_len = (uint16)strlen((char *)str);
    MMI_STRNTOWSTR(wstr, 8,
                str, 8,
                str_len);
    freq_len = MMIAPICOM_Wstrlen(wstr);
    if(freq_len > 0)
    {           
      string.wstr_len = freq_len;
      string.wstr_ptr = wstr;        
      GUILABEL_SetFont(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID, SONG_FONT_10, MMI_GRAY_WHITE_COLOR);
      GUILABEL_SetText(MMIPICVIEW_TOTAL_NUM_LABEL_CTRL_ID,&string,FALSE);
    }  
}  
#endif
#endif //#ifdef PIC_VIEWER_SUPPORT

/*Edit by script, ignore 2 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527

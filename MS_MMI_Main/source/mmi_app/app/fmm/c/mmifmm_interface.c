/*****************************************************************************
** File Name:      mmifmm_interface.c                                        *
** Author:                                                                   *
** Date:             12/19/2007                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe fmm interface module        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2007       liqing.peng          Create                                 *
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifmm_interface.h"
#include "mmifmm_comfunc.h"
#include "mmifmm_mainwin.h"
#include "mmifmm_id.h"
#include "mmifmm_image.h"
#include "mmifmm_text.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "guitab.h"
#ifdef MSDC_CARD_SUPPORT
#include "mmisd_export.h"
#endif
#include "mmi_filetask.h"
#include "mmidc_export.h"

#include "mmifmmsrv_api.h"
#include "mmifmm_mainwin.h"
#include "mmifmm_app.h"
#ifdef DRM_SUPPORT
#include "mmidrm_image.h"
#include "mmidrm_export.h"
#endif
#include "mmifmm.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmifmm.h"
#include "mmifmm_app.h"
#include "gui_ucs2b_converter.h"
#include "mmidisplay_data.h"
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
#include "guires.h"
#endif
#ifdef MSDC_CARD_SUPPORT
#include "mmifmm_sd.h"
#endif
#include "mmisrvfmm_export.h"
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
#define FMM_SLASH       0x005C
#define FMM_FILE_MARK_MAXNUM        10000


#define MMIFMM_TITLE_MAX_LEN         16/*lint !e767*/
#define MMIFMM_DEFAULT_PATH_LEN       3


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    MMIFMM_OPEN_LAYER_WIN_INFO_T    *win_info;
    MMI_TEXT_ID_T                   s_leftsoft_id;
    MMI_TEXT_ID_T                   s_middlesoft_id;
    MMI_TEXT_ID_T                   s_rightsoft_id;
    MMIFMM_FILTER_T                 s_filter;
    wchar                           s_cur_path [FMM_SEARCH_FILENAME_MAXLEN+1];
    uint16                          s_cur_path_len;
    wchar                           s_filter_str [FMM_FILTER_STRING_MAXLEN];
    BOOLEAN                         s_is_multi_sel;
    PROCESSMSG_FUNC                 s_handle_func;
    FILEARRAY                       s_ret_array;   //set by user
    FILEARRAY                       s_file_array;  //use for internal list
    MMIFMM_WINDISP_TYPE_E           s_disp_type;
    MMIFMM_OpenFileCallBack         s_callback;
    MMIFMM_SelectFileCallBack       s_select_cbk;
    BOOLEAN                         s_is_recurse;
    BOOLEAN                         s_is_sel_folder;
    MMIFMM_FUNC_TYPE_E              s_func_type;      //类型
    BOOLEAN                         s_is_ret_succ;
#ifdef DRM_SUPPORT
    uint32                          s_filter_type;
#endif
} MMIFMM_OPEN_LAYER_WIN_DATA_T;

typedef struct
{
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle;
    BOOLEAN                         s_need_list;
    BOOLEAN                         s_need_stop_search;
    FILEARRAY                       s_file_array;
    MMIFMM_SRV_HANDLE               srv_hande;
} MMIFMM_LAYER_WAIT_DATA_T;

typedef struct
{
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle;
    MMIFILE_DEVICE_E                s_layerwin_device_tab [MMI_DEVICE_NUM];
} MMIFMM_FILE_WIN_DATA_T;

typedef struct
{
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle;
    wchar                           *s_dev_path_ptr[MMI_DEVICE_NUM - 1];
    BOOLEAN                         s_need_show_error_msg;
    uint16                          *s_sel_file_name_ptr;
    uint16                          s_sel_file_name_len;
    BOOLEAN                         is_window_open;
    uint32                          pre_tab_index;
} MMIFMM_SUBFILEWIN_DATA_T;
/**--------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **--------------------------------------------------------------------------*/
#ifdef MMI_PDA_SUPPORT
LOCAL BOOLEAN       s_is_have_preview_icon = FALSE;
#endif

/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 获得上一级目录
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetPrevPath (
    wchar  *cur_path,
    uint16 cur_path_len,
    wchar  *ret_path,
    uint16 ret_path_max_len
);

/*****************************************************************************/
//  Description : handle open file by layer waiting window message
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenFileByLayerWaitWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
// Description : 把文件load到list中
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void LoadFileToList (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T   ctrl_id,
    FILEARRAY       file_array
);
/*****************************************************************************/
// Description : 把item load到list中
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void LoadItemToList (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T   ctrl_id,
    uint16          item_index,
    FILEARRAY       file_array,
    GUILIST_ITEM_DATA_T   *item_data_ptr,
    uint16          item_content_index,
    BOOLEAN         is_item_data
);

/*****************************************************************************/
//  Description : 打开搜索文件等待窗口
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void OpenSearchFileWaitWin (MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle);

/*****************************************************************************/
//  Description : 打开文件列表主窗口
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void OpenFileListMainWin (MMIFMM_OPEN_LAYER_WIN_DATA_T* w_handle);


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_CreateNewSoftKey (MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id);
#endif
/*****************************************************************************/
//  Description : handle open file by layer list window message
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIFMM_HandleOpenFileWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
/*****************************************************************************/
//  Description : MMIFMM_SetDeviceTable
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetLayerWinDeviceTable (uint32 tab_index, MMIFILE_DEVICE_E fmm_dev);

/*****************************************************************************/
//  Description : MMIFMM_SetDeviceTable
//  Global resource dependence :
//  Author: juan.wu
//  Note:tab_index的范围总是在0~(MMI_DEVICE_NUM - MMI_DEVICE_UDISK)之间
//       单T卡tab_index = 0 或1；双T卡tab_index= 0，1，2
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E  GetLayerWinDeviceType (uint32 tab_index);

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetTileTxt (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E  BackButtonCallBack (void);

#endif

/*****************************************************************************/
//  Description : SetLayerwinSoftKey
//  Return:
//  Global resource dependence :
//  Author:
//  Note:设置layerwin窗口的softkey
/*****************************************************************************/
LOCAL void SetLayerwinSoftKey (MMI_WIN_ID_T win_id, uint16 index, FILEARRAY file_array);

/*****************************************************************************/
//  Description : FMMLayerWin_ValidatePrivacyCBK
//  Return:
//  Global resource dependence :
//  Author:
//  Note:开启隐私保护的处理
/*****************************************************************************/
LOCAL void FMMLayerWin_ValidatePrivacyCBK (BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description : set param before search
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void InitSearchParam (
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle,
    const wchar                     *path_ptr,          //in, 搜索目录名，如果是NULL，则搜索所有存储器
    uint16                          path_len,
    MMIFMM_FILTER_T                 *filter_ptr    //筛选参数
);
/*****************************************************************************/
// Description : 从源字符串中解析搜索关键字符串
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseFilterStr (
    MMIFMM_FILTER_T    *filter_ptr,
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle
);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LayerWaitWinOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle
);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LayerWaitWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FileWinOpenCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FileWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EnterOpenFileByLayerWin (
    MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle,
    uint32 current_index
);
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SubOpenFileWinOpenCtrlFunc (MMI_WIN_ID_T win_id);


/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SubOpenFileWinCloseCtrlFunc (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle open file by layer list window message
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenFileByLayerWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);
/*****************************************************************************/
//  Description : search file call back
//  Return: 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void WaitWinSearchFileCallBack (
                               MMIFMM_SRV_HANDLE search_handle,
                               FILEARRAY folder_array
                               );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE (MMIFMM_OPENFILE_WIN_TAB) =
{
    WIN_FUNC ( (uint32) MMIFMM_HandleOpenFileWin),
    WIN_ID (MMIFMM_OPEN_FILE_WIN_ID),
//fixed by liyan.zhu for bugzilla 2781
#if (defined (MMI_GUI_STYLE_TYPICAL)||defined (MMI_GUI_STYLE_MINISCREEN))
    CREATE_TAB_CTRL (GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL, MMIFMM_LAYER_TAB_CTRL_ID),
#else
    CREATE_TAB_CTRL (GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL | GUITAB_STATE_SINGLE_LINE, MMIFMM_LAYER_TAB_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE (MMIFMM_OPENFILE_BYLAYER_WIN_TAB) =
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),

    WIN_FUNC ( (uint32) HandleOpenFileByLayerWin),
    WIN_ID (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE (WS_HAS_BUTTON_SOFTKEY | WS_DISABLE_SOFTKEY_TO_TITLE),
#endif
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
    CREATE_FORM_CTRL (GUIFORM_LAYOUT_ORDER, MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID),
    CHILD_FORM_CTRL (FALSE, GUIFORM_LAYOUT_SBS, MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID),
    CHILD_LABEL_CTRL (GUILABEL_ALIGN_MIDDLE, FALSE, MMIFMM_OPEN_FILE_BYLAYER_TITLE_LABEL_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL (TRUE, IMAGE_COMMON_BACK, MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID),
    CHILD_LIST_CTRL (TRUE, GUILIST_TEXTLIST_E, MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID),
#else
    CREATE_LISTBOX_CTRL (GUILIST_TEXTLIST_E, MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID),
#endif
    WIN_SOFTKEY (STXT_SELECT, TXT_NULL, STXT_RETURN),
    //WIN_TITLE( TXT_FMM_FILE_LIST ),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : handle open file by layer waiting window message
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenFileByLayerWaitWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint32          total_num = 0;
    uint32          i  = 0;
    FILEARRAY_DATA_T f_array_data = {0};
    MMIFMM_LAYER_WAIT_DATA_T *lwait_win_data = (MMIFMM_LAYER_WAIT_DATA_T*) MMK_GetWinUserData (win_id);
	if (PNULL == lwait_win_data)//coverity 12976
	{
		SCI_TRACE_LOW("[mmifmm_interface.c] [HandleOpenFileByLayerWaitWin] PNULL != lwait_win_data");
		return MMI_RESULT_FALSE;
	}

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMI_RESULT_FALSE;
        break;
    case MSG_FMM_OPENFILEWIN_STARTSEARCH:
        if (PNULL!=lwait_win_data->w_handle)
        {
            if (NULL != lwait_win_data->w_handle->s_file_array)
            {
                //进入下一级目录之前,先把上次的文件数组释放
                MMIAPIFILEARRAY_Destroy (&lwait_win_data->w_handle->s_file_array);
                lwait_win_data->w_handle->s_file_array = NULL;
            }

            lwait_win_data->w_handle->s_file_array = MMIAPIFILEARRAY_Create();

            //SCI_ASSERT(NULL != lwait_win_data->s_file_array);
            if (PNULL == lwait_win_data->w_handle->s_file_array)
            {
                //SCI_TRACE_LOW:"MMIFMM: HandleOpenFileByLayerWaitWin PNULL == lwait_win_data->w_handle->s_file_array"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1476_112_2_18_2_20_22_294, (uint8*) "");
                return MMI_RESULT_FALSE;
            }

            if (lwait_win_data->s_need_list)
            {
                MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
                search_file_info.path_ptr = lwait_win_data->w_handle->s_cur_path;
                search_file_info.path_len = lwait_win_data->w_handle->s_cur_path_len;
                search_file_info.file_array = lwait_win_data->w_handle->s_file_array;
                search_file_info.filter_ptr = &lwait_win_data->w_handle->s_filter;
                search_file_info.is_recurse = FALSE;
                search_file_info.sort_type  = FILEARRAY_SORT_NAME_ASCEND;//FILEARRAY_SORT_TYPE_ASCEND;
                search_file_info.win_id     = win_id;
                search_file_info.msg_id     = MSG_FMM_OPENFILEWIN_SEARCH_END;
                search_file_info.func_type = lwait_win_data->w_handle->s_func_type;
                search_file_info.search_type = SearchFileInPath;
                search_file_info.search_cbk = WaitWinSearchFileCallBack;
                MMIFMM_SearchFileSrv (lwait_win_data->srv_hande, &search_file_info);
            }
            else
            {
                MMIFMM_SEARCH_FILE_INFO_T search_file_info = {0};
                search_file_info.path_ptr = lwait_win_data->w_handle->s_cur_path;
                search_file_info.path_len = lwait_win_data->w_handle->s_cur_path_len;
                search_file_info.file_array = lwait_win_data->w_handle->s_file_array;
                search_file_info.filter_ptr = &lwait_win_data->w_handle->s_filter;
                search_file_info.is_recurse = FALSE;
                search_file_info.sort_type  = FILEARRAY_SORT_NULL;
                search_file_info.win_id     = win_id;
                search_file_info.msg_id     = MSG_FMM_OPENFILEWIN_SEARCH_END;
                search_file_info.func_type = lwait_win_data->w_handle->s_func_type;
                search_file_info.search_type = SearchFileInPathNoSort;
                search_file_info.search_cbk = WaitWinSearchFileCallBack;
                MMIFMM_SearchFileSrv (lwait_win_data->srv_hande, &search_file_info);
            }
        }
        break;
    case MSG_FMM_SET_SEARCH_SEL_DIRECTLY:
        lwait_win_data->s_need_list = FALSE;
        break;
    case MSG_FMM_OPENFILEWIN_SEARCH_END:

        if (lwait_win_data->s_need_list)
        {
            //打开文件列表主窗口
            if (!lwait_win_data->s_need_stop_search)
            {
                break;
            }

            OpenFileListMainWin (lwait_win_data->w_handle);
            MMK_SendMsg (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_FMM_OPENFILEWIN_RELOAD, NULL);
        }
        else
        {
            if (PNULL!=lwait_win_data->w_handle)//coverity 12976
            {
                //only for selected
                total_num = (uint16) MMIAPIFILEARRAY_GetArraySize (lwait_win_data->w_handle->s_file_array);

                for (i = 0; i < total_num; i++)
                {
                    SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));

                    if (MMIAPIFILEARRAY_Read (lwait_win_data->w_handle->s_file_array, i, &f_array_data))
                    {
                        if (FILEARRAY_TYPE_FILE == f_array_data.type)
                        {
                            MMIAPIFILEARRAY_Add (lwait_win_data->w_handle->s_ret_array, &f_array_data);
                        }
                    }
                }

                lwait_win_data->w_handle->s_callback (lwait_win_data->s_need_stop_search, lwait_win_data->w_handle->s_ret_array);
                lwait_win_data->w_handle->s_ret_array = PNULL;
                //lwait_win_data->w_handle->s_callback  = PNULL;

                MMIAPIFILEARRAY_Destroy (&lwait_win_data->w_handle->s_file_array);
                lwait_win_data->w_handle->s_file_array = PNULL;
            }

            if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
            {
                MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
            }
        }

        MMK_CloseWin (win_id);
        break;

    case MSG_APP_CANCEL:
	//	break; //CR178665 search时按右键无反应；实际此时应该取消search，但search并未真正取消，只是将search的等待窗口即时关闭。
               //和red做同样处理，返回false。cancel的默认处理什么都不做；red返回idle；故实际上两个消息的效果并不同。
    case MSG_APP_RED:
        lwait_win_data->s_need_stop_search = FALSE;
        MMIAPIFMM_SearchFileStop();
        MMIAPIFMM_CloseSelectFileWin();
        //MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;
    case MSG_CLOSE_WINDOW:

        if (lwait_win_data->s_need_stop_search)
        {
            //窗口异常被关闭，search end的消息将不被处理
            //如果是正常的关闭窗口，则先处理了search end消息，这里的FALSE就没作用
            lwait_win_data->s_need_stop_search = FALSE;
            MMIAPIFMM_SearchFileStop();
        }

        //防止内存泄漏,只有在需要列表窗口时，才需要在这里释放，
        //否则在MSG_FMM_OPENFILEWIN_SEARCH_END中释放
        if (!MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID) && lwait_win_data->s_need_list)
        {
			if (PNULL!=lwait_win_data->w_handle)
			{
            if (NULL != lwait_win_data->w_handle->s_file_array)
            {
                MMIAPIFILEARRAY_Destroy (&lwait_win_data->w_handle->s_file_array);
                lwait_win_data->w_handle->s_file_array = NULL;
            }

            if (PNULL != lwait_win_data->w_handle->s_callback && PNULL != lwait_win_data->w_handle->s_ret_array)
            {
                lwait_win_data->w_handle->s_callback (FALSE, lwait_win_data->w_handle->s_ret_array);
            }

            SCI_FREE (lwait_win_data->w_handle);
			}
        }

        LayerWaitWinCloseCtrlFunc (win_id);
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    default:
        result = MMIPUB_HandleWaitWinMsg (win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : search file call back
//  Return: 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void WaitWinSearchFileCallBack (
                               MMIFMM_SRV_HANDLE search_handle,
                               FILEARRAY folder_array
                               )
{
    MMIAPIFMM_DestroySrv (search_handle);
}
/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LayerWaitWinOpenCtrlFunc (
    MMI_WIN_ID_T win_id,
    MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle
)
{
    MMIFMM_LAYER_WAIT_DATA_T *lwait_win_data = PNULL;

    if (PNULL == w_handle)
    {
        return MMI_RESULT_FALSE;
    }

    lwait_win_data = (MMIFMM_LAYER_WAIT_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_LAYER_WAIT_DATA_T));
    lwait_win_data->w_handle = w_handle;
    lwait_win_data->s_need_list = TRUE;
    lwait_win_data->s_need_stop_search = TRUE;
    lwait_win_data->srv_hande = MMIAPIFMM_CreateSrv();
    MMK_SetWinUserData (win_id, (void *) lwait_win_data);

    //MMICL_RegisterDateChangeEvent(win_id, MMICL_CALLTIMELIST_WIN_FLAG);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LayerWaitWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_LAYER_WAIT_DATA_T *win_data_p = (MMIFMM_LAYER_WAIT_DATA_T*) MMK_GetWinUserData (win_id);

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : MMIFMM_CreateNewSoftKey
//  Global resource dependence :
//  Author: mary.xiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIFMM_CreateNewSoftKey (MMI_HANDLE_T win_handle, MMI_TEXT_ID_T leftkey_id, MMI_TEXT_ID_T midkey_id, MMI_TEXT_ID_T rightkey_id)
{
    GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
    MMI_CONTROL_CREATE_T softkey_create = {0};
    MMI_CTRL_ID_T   softkey_ctrl_id = 0;

    MMK_DestroyControl (MMK_GetCtrlHandleByWin (win_handle , MMITHEME_GetSoftkeyCtrlId()));

    softkey_data.leftsoft_id = leftkey_id;
    softkey_data.midsoft_id = midkey_id;
    softkey_data.rightsoft_id = rightkey_id;


    softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
    softkey_create.ctrl_id = softkey_ctrl_id;
    softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
    softkey_create.parent_win_handle = win_handle;
    softkey_create.init_data_ptr = &softkey_data;

    MMK_CreateControl (&softkey_create);

    MMK_SetWinSoftkeyCtrlId (win_handle, softkey_ctrl_id);

    return TRUE;
}
#endif

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SubOpenFileWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);
    uint32 i = 0;

    if (PNULL == win_data_p)
    {
        return MMI_RESULT_FALSE;
    }

    win_data_p->s_sel_file_name_ptr = (wchar *) SCI_ALLOC_APPZ ( (FMM_SEARCH_FILENAME_MAXLEN + 1) * sizeof (wchar));

    for (i = 0; i < MMI_DEVICE_NUM - 1; i++)
    {
        if (PNULL == win_data_p->s_dev_path_ptr[i])
        {
            win_data_p->s_dev_path_ptr[i] = SCI_ALLOC_APPZ (sizeof (wchar) * (FMM_SEARCH_FILENAME_MAXLEN + 1));

            if (PNULL == win_data_p->s_dev_path_ptr[i])
            {
                return MMI_RESULT_FALSE;
            }
        }
    }

    MMK_SetWinUserData (win_id, (void *) win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SubOpenFileWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);

    uint8 i = 0;

    if (PNULL != win_data_p)
    {
        for (i = 0; i < MMI_DEVICE_NUM - 1; i++)
        {
            if (PNULL != win_data_p->s_dev_path_ptr[i])
            {
                SCI_FREE (win_data_p->s_dev_path_ptr[i]);
                win_data_p->s_dev_path_ptr[i] = PNULL;
            }
        }

        if (NULL != win_data_p->s_sel_file_name_ptr)
        {
            SCI_FREE (win_data_p->s_sel_file_name_ptr);
        }

        //win_data_p->is_window_open = FALSE;
        //win_data_p->pre_tab_index = 0;
    }

    SCI_FREE (win_data_p);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle open file by layer list window message
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOpenFileByLayerWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID;
    uint16          item_index = 0;
    uint16          item_content_index = 0;
    uint16          cur_sel = 0;
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    wchar           default_path[MMIFMM_DEFAULT_PATH_LEN] = {0};
    uint16          default_path_len = 0;
    uint16                  i = 0;
    uint16                  total_num = 0;
    uint16                  file_name_len = 0;
    FILEARRAY_DATA_T        f_array_data = {0};
    BOOLEAN          is_preview = FALSE;
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    GUIFORM_CHILD_WIDTH_T  child_width = {0};
    GUI_BG_T               bg_info  = {0};
    uint16                 hor_space = 0;
    uint16                 ver_space = 0;
    uint16                 back_button_w = 0;
    uint16                 back_button_h = 0;
#endif /* MMI_PDA_SUPPORT */
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_TRACE_LOW:"MMIFMM_HandleOpenFileByLayerWin, msg_id = %x"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1624_112_2_18_2_20_22_295, (uint8*) "d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        if (MMI_RESULT_FALSE == SubOpenFileWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);
        if (PNULL == win_data_p)
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_data_p->is_window_open = TRUE;

        if (MMK_IsOpenWin (MMIFMM_MOVE_FILE_WIN_ID)) //移动时，不要提示
        {
            win_data_p->s_need_show_error_msg = FALSE;
        }
        else
        {
            win_data_p->s_need_show_error_msg = TRUE;
        }

        GUILIST_SetListState (ctrl_id, GUILIST_STATE_NEED_SOFTKEY, FALSE);
        SCI_TRACE_LOW ("FMM: win_data_p->s_need_show_error_msg = %d", win_data_p->s_need_show_error_msg);
        win_data_p->w_handle->s_is_ret_succ = FALSE;

        SCI_MEMSET (win_data_p->s_sel_file_name_ptr, 0, ( (FMM_SEARCH_FILENAME_MAXLEN + 1) * sizeof (wchar)));
        win_data_p->s_sel_file_name_len = 0;

        //init selected file
        file_name_len = FMM_SEARCH_FILENAME_MAXLEN;

        if (MMIFMM_GetOpenFileWinTickedFile ((wchar*)win_data_p->s_sel_file_name_ptr, &file_name_len))
        {
            win_data_p->s_sel_file_name_len = file_name_len;
        }

        if (win_data_p->w_handle->s_is_multi_sel)
        {
            GUILIST_SetTextListMarkable (ctrl_id, TRUE);
        }

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
        SetTileTxt (win_id);
#else
        MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, MMIAPICOM_Wstrlen (win_data_p->w_handle->s_cur_path));
#endif

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT

        {

            child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
            GUIRES_GetImgWidthHeight (&back_button_w, &back_button_h, IMAGE_COMMON_BACK, win_id);
            GUIFORM_IsSlide (MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID, FALSE);
            GUIFORM_SetChildHeight (MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID, &child_height);
            GUIFORM_PermitChildBg (MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBorder (MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID, FALSE);
            GUIFORM_SetSpace (MMIFMM_OPEN_FILE_BYLAYER_FORM_CTRL_ID, &hor_space, &ver_space);
#ifdef MMI_PDA_SUPPORT

            if (MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
#endif
            {
                child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
                child_width.add_data = back_button_w;
                GUIFORM_SetChildWidth (MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, &child_width);
                GUIBUTTON_SetCallBackFunc (MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, BackButtonCallBack);
                bg_info.bg_type = GUI_BG_IMG;
                bg_info.img_id = IMAGE_COMMON_BACK_HIGHLIGHT;

                GUIBUTTON_SetPressedBg (MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, &bg_info);
                GUIBUTTON_SetRunSheen (MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, FALSE);
            }
        }
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT

        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            child_width.add_data = 0;
            GUIFORM_SetChildWidth (MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, &child_width);
            GUIBUTTON_SetVisible (MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, FALSE, FALSE);

        }

#endif
#endif
        MMK_SetAtvCtrl (win_id, ctrl_id);
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_LIST_CHECK_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
            item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *) param;

            if (item_content_ptr == PNULL)
            {
                break;
            }

            cur_sel = item_content_ptr->item_index;

            if (!MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_sel, &f_array_data))
            {
                break;
            }

            if ( (item_content_ptr->item_content_index == 2) && (f_array_data.type == FILEARRAY_TYPE_FOLDER))
            {
                MMK_SendMsg (win_id, MSG_APP_OK, PNULL);
            }

            if (PNULL != win_data_p->w_handle->s_handle_func)
            {
                win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
            }

        }
        break;
#endif
    case MSG_FMM_OPENFILEWIN_RELOAD:

        if (win_data_p->s_need_show_error_msg)
        {
            if (SFS_ERROR_NO_SPACE == MMIAPIFILEARRAY_GetFileErrorType (win_data_p->w_handle->s_file_array)
                    && FILEARRAY_MAX_BUFFER_NUMBER >= MMIAPIFILEARRAY_GetArraySize (win_data_p->w_handle->s_file_array))
            {
                win_data_p->s_need_show_error_msg = FALSE;
            }
            else
            {
                MMIAPIFMM_PromptFileErrorType (PNULL,
                                               MMIAPIFILEARRAY_GetFileErrorType (win_data_p->w_handle->s_file_array),
                                               MMIAPIFILEARRAY_GetFileDev (win_data_p->w_handle->s_file_array)
                                              );
            }

            win_data_p->s_need_show_error_msg = FALSE;
        }

        GUIFORM_SetChildDisplay (MMIFMM_OPEN_FILE_BYLAYER_TITLE_FORM_CTRL_ID, MMIFMM_OPEN_FILE_BYLAYER_TITLE_BUTTON_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);

        LoadFileToList (win_id, ctrl_id, win_data_p->w_handle->s_file_array);

//        pre_preview_index = 0xffff;
        if (win_data_p->is_window_open)
        {
            MMIFMM_SetOpenFileWinCurFile (win_data_p->s_sel_file_name_ptr, win_data_p->s_sel_file_name_len);
        }

        //set softkey
#ifndef MMI_PDA_SUPPORT//NEWMS00172268  

        if (win_data_p->w_handle->s_is_sel_folder || FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type) //只是选择文件夹
        {
            cur_sel = GUILIST_GetCurItemIndex (ctrl_id);
            SetLayerwinSoftKey (win_id, cur_sel, win_data_p->w_handle->s_file_array);
        }
        else if (0 == MMIAPIFILEARRAY_GetArraySize (win_data_p->w_handle->s_file_array) && !win_data_p->w_handle->s_is_sel_folder) //NEWMS00194102
        {
            //SCI_TRACE_LOW:"MMIFMM_HandleOpenFileByLayerWin softkey STXT_RETURN11"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1722_112_2_18_2_20_22_296, (uint8*) "");
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);  
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);     
#endif
        }
        else
        {
            cur_sel = GUILIST_GetCurItemIndex (ctrl_id);
            SetLayerwinSoftKey (win_id, cur_sel, win_data_p->w_handle->s_file_array);
        }

#endif
#ifdef MMI_PDA_SUPPORT

        if ( (FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type || win_data_p->w_handle->s_is_sel_folder) && (0 < win_data_p->w_handle->s_cur_path_len))
        {
            MMIFMM_CreateNewSoftKey (win_id, TXT_FMM_SELECT_THIS_FOLDER, TXT_NULL, STXT_CANCEL);
        }
        else if (win_data_p->w_handle->s_is_multi_sel)
        {
            MMIFMM_CreateNewSoftKey (win_id, TXT_COMMON_OK, TXT_NULL, STXT_CANCEL);

            if (0 == GUILIST_GetSelectedItemNum (ctrl_id))
            {
                GUIWIN_SetSoftkeyBtnGray (win_id, 0, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyBtnGray (win_id, 0, FALSE);
            }
        }
        else
        {
            MMIFMM_CreateNewSoftKey (win_id, TXT_NULL, STXT_CANCEL, TXT_NULL);
        }

#endif

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

        MMK_SetAtvCtrlEx (win_id, ctrl_id, FALSE);
        break;

    case MSG_CTL_TAB_SWITCH:
        {
            uint32 cur_tab_index = GUITAB_GetCurSel (MMIFMM_LAYER_TAB_CTRL_ID);
            MMIFILE_DEVICE_E cur_dev_type = GetLayerWinDeviceType (cur_tab_index);

            //SCI_TRACE_LOW:"MMIFMM_HandleOpenFileByLayerWin cur_dev_type = %d,cur_tab_index = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1805_112_2_18_2_20_23_299, (uint8*) "dd", cur_dev_type, cur_tab_index);

            if (PNULL != win_data_p->w_handle->s_file_array)
            {
                MMIAPIFILEARRAY_Destroy (&win_data_p->w_handle->s_file_array);
            }

            MMIFMM_RemoveListData (ctrl_id);
            //记录前一次的路径
            SCI_MEMSET (win_data_p->s_dev_path_ptr[win_data_p->pre_tab_index], 0, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1));//coverity 12986
            MMIAPICOM_Wstrncpy (win_data_p->s_dev_path_ptr[win_data_p->pre_tab_index], win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);

            if(MMI_DEVICE_NUM - 1 <= cur_tab_index || 0 > cur_tab_index)
            {
                break;
            }
            //如果当前路径没有初值，是第一次进入该tab
            if (0 == MMIAPICOM_Wstrlen (win_data_p->s_dev_path_ptr[cur_tab_index]))
            {
                MMIAPICOM_Wstrcpy (default_path, MMIAPIFMM_GetDevicePath (cur_dev_type));
                MMIAPICOM_Wstrcat (default_path, L":");
                default_path_len = (uint16) MMIAPICOM_Wstrlen (default_path);
                MMIAPICOM_Wstrncpy (win_data_p->s_dev_path_ptr[cur_tab_index], default_path, default_path_len);
            }

            InitSearchParam (win_data_p->w_handle,
                             win_data_p->s_dev_path_ptr[cur_tab_index],
                             MMIAPICOM_Wstrlen (win_data_p->s_dev_path_ptr[cur_tab_index]),
                             &win_data_p->w_handle->s_filter);
            win_data_p->w_handle->s_is_recurse = FALSE;
            //win_data_p->w_handle->s_func_type = win_data_p->w_handle->s_func_type; //coverity 12940

            if (MMIFMM_IsPreviewMusic()) //如果在预览铃声，则停掉audio
            {
                MMIAPISET_StopAppRing();
            }

            OpenSearchFileWaitWin (win_data_p->w_handle);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
            SetTileTxt (win_id);
#else
            MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
#endif
            win_data_p->pre_tab_index = cur_tab_index;
#ifdef MSDC_CARD_SUPPORT
            if (MMIAPISD_IsSDNeedFormat (cur_tab_index+1))
            {
                //格式错误，提示是否格式化
                MMIFMM_FmmMemoryFormat ((cur_tab_index+1), TXT_IS_FORMAT_ERROR_SDCARD);
                break;
            }
#endif

        }
        break;

    case MSG_TIMER:

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*) param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );
            if (PNULL == need_item_data_ptr)
            {
                //SCI_TRACE_LOW:"MSG_CTL_LIST_NEED_ITEM_DATA,msg_id = %x, param error :PNULL == need_item_data_ptr"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1854_112_2_18_2_20_23_300, (uint8*) "d");
                break;
            }

            item_index = need_item_data_ptr->item_index;

            LoadItemToList (win_id, ctrl_id, item_index, win_data_p->w_handle->s_file_array, &item_data, 0, TRUE);
            MMIFMM_DrawTickedOnFile (item_index);
            //GUILIST_SetItemData(ctrl_id, &item_data, item_index );
        }
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*) param;

            //SCI_ASSERT( PNULL != need_item_content_ptr );
            if (PNULL == need_item_content_ptr)
            {
                //SCI_TRACE_LOW:"MSG_CTL_LIST_NEED_ITEM_CONTENT,msg_id = %x, param error :PNULL == need_item_content_ptr"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_1871_112_2_18_2_20_23_301, (uint8*) "d");
                break;
            }

            item_index = need_item_content_ptr->item_index;
            item_content_index = need_item_content_ptr->item_content_index;

            LoadItemToList (win_id, ctrl_id, item_index, win_data_p->w_handle->s_file_array, &item_data, item_content_index, FALSE);
            //GUILIST_SetItemContent(ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index );
        }
        break;

    case MSG_NOTIFY_LIST_MARK_ITEM:
        {
            uint16 mark_num = GUILIST_GetSelectedItemNum (ctrl_id);
            uint16 cur_index = GUILIST_GetCurItemIndex (ctrl_id);

            if (PNULL != win_data_p->w_handle->s_select_cbk)
            {
                BOOLEAN cbk_result = TRUE;

                MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_index, &f_array_data);
                cbk_result = win_data_p->w_handle->s_select_cbk (mark_num, &f_array_data);

                if (!cbk_result)
                {
                    GUILIST_SetSelectedItem (ctrl_id, cur_index, FALSE);
                    mark_num = GUILIST_GetSelectedItemNum (ctrl_id);
                    MMK_SendMsg (ctrl_id, MSG_CTL_PAINT, PNULL);
                }
            }

            SetLayerwinSoftKey (win_id, cur_index, win_data_p->w_handle->s_file_array);
#ifdef MMI_PDA_SUPPORT

            if (win_data_p->w_handle->s_is_multi_sel)
            {
                if (0 < mark_num)
                {
                    GUIWIN_SetSoftkeyBtnGray (win_id, 0, FALSE);
                }
                else
                {
                    GUIWIN_SetSoftkeyBtnGray (win_id, 0, TRUE);
                }
            }

#endif
        }
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:

        //set softkey
        if ((0 == MMIAPIFILEARRAY_GetArraySize (win_data_p->w_handle->s_file_array))
            && FUNC_FIND_FOLDER != win_data_p->w_handle->s_func_type)
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE); 
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
			GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);     
#endif
        }
        else
        {
            cur_sel = GUILIST_GetCurItemIndex (ctrl_id);

            SetLayerwinSoftKey (win_id, cur_sel, win_data_p->w_handle->s_file_array);
        }

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

        break;
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
        // case MSG_APP_MENU:
        cur_sel = GUILIST_GetCurItemIndex (ctrl_id);

        if (!MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_sel, &f_array_data))
        {
#ifndef MMI_PDA_SUPPORT

            if (win_data_p->w_handle->s_is_sel_folder || FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type)
            {
                SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));
                f_array_data.type = FILEARRAY_TYPE_FOLDER;
                MMI_WSTRNCPY (f_array_data.filename, FMM_SEARCH_FILENAME_MAXLEN,
                              win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len, win_data_p->w_handle->s_cur_path_len);
                f_array_data.name_len = win_data_p->w_handle->s_cur_path_len;

                //select current folder
                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);

                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                }

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
#endif
            break;
        }

        {
#if 1//def MMI_PDA_SUPPORT

            if ( (!win_data_p->w_handle->s_is_sel_folder) && (0 < GUILIST_GetSelectedItemNum (ctrl_id)))
#else
            if (0 < GUILIST_GetSelectedItemNum (ctrl_id))

#endif
            {
                //add masked files to array
                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    total_num = (uint16) MMIAPIFILEARRAY_GetArraySize (win_data_p->w_handle->s_file_array);

                    for (i = 0; i < total_num; i++)
                    {
                        SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));

                        if ( (GUILIST_GetItemState (GUILIST_GetItemPtrByIndex (ctrl_id, i), GUIITEM_STATE_SELECTED)) && MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, i, &f_array_data))
                        {
                            MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);
                        }
                    }

                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                }

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
            else if (FILEARRAY_TYPE_FILE == f_array_data.type && !win_data_p->w_handle->s_is_sel_folder)
            {
                //add current file to array
                if (PNULL != win_data_p->w_handle->s_select_cbk)
                {
                    BOOLEAN cbk_result = TRUE;

                    cbk_result = win_data_p->w_handle->s_select_cbk (1, &f_array_data);

                    if (!cbk_result)
                    {
                        break;
                    }
                }

                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);
                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                }

                if (PNULL != win_data_p->w_handle->s_handle_func)
                {
                    win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
                }

                is_preview = MMK_IsOpenWin (MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID) || MMK_IsOpenWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID);

                if (!is_preview) //墙纸设置修改了流程，点击某图片是预览，不是选中，所以不需要关闭该窗口 cr153753
                {
                    if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                    {
                        MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                    }
                }
            }            
            else if (
#ifdef MMI_PDA_SUPPORT
                FILEARRAY_TYPE_FOLDER == f_array_data.type && 
#endif
                     (FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type || win_data_p->w_handle->s_is_sel_folder))
                //else if(FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type || win_data_p->w_handle->s_is_sel_folder)
            {
#ifndef MMI_PDA_SUPPORT//只有非PDA版本需要设置当前路径为选择路径
                SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));
                f_array_data.type = FILEARRAY_TYPE_FOLDER;
                MMI_WSTRNCPY (f_array_data.filename, FMM_SEARCH_FILENAME_MAXLEN,
                              win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len, win_data_p->w_handle->s_cur_path_len);
                f_array_data.name_len = win_data_p->w_handle->s_cur_path_len;
#endif

                //select current folder
                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);

                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                }

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
            else if (FILEARRAY_TYPE_FOLDER == f_array_data.type
                     && (FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type))
            {
                //enter folder or select folder
                if (win_data_p->w_handle->s_is_multi_sel)
                {
                    //select this folder
                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    SCI_MEMSET (win_data_p->w_handle->s_cur_path, 0, sizeof (win_data_p->w_handle->s_cur_path));
                    MMI_WSTRNCPY (win_data_p->w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                                  f_array_data.filename, f_array_data.name_len, f_array_data.name_len);
                    win_data_p->w_handle->s_cur_path_len = f_array_data.name_len;

                    if (MMIFMM_IsPreviewMusic()) //如果在预览铃声，则停掉audio
                    {
                        MMIAPISET_StopAppRing();
                    }

                    OpenSearchFileWaitWin (win_data_p->w_handle);
                    MMK_SendMsg (MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID, MSG_FMM_SET_SEARCH_SEL_DIRECTLY, PNULL);
                }
                else
                {
                    //enter this folder
                    SCI_MEMSET (win_data_p->w_handle->s_cur_path, 0, sizeof (win_data_p->w_handle->s_cur_path));
                    MMI_WSTRNCPY (win_data_p->w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                                  f_array_data.filename, f_array_data.name_len, f_array_data.name_len);
                    win_data_p->w_handle->s_cur_path_len = f_array_data.name_len;

                    if (MMIFMM_IsPreviewMusic()) //如果在预览铃声，则停掉audio
                    {
                        MMIAPISET_StopAppRing();
                    }

                    //打开搜索文件等待窗口
                    OpenSearchFileWaitWin (win_data_p->w_handle);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
                    SetTileTxt (win_id);
#else
                    MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
#endif
                }
            }
        }

        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            uint16   path_depth = 0;

            path_depth = MMIFMM_GetPathDepth (win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);

            //SCI_TRACE_LOW:"[MMIFMM]:MSG_APP_CANCEL path_depth = %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2085_112_2_18_2_20_23_302, (uint8*) "d", path_depth);

            if (1 < path_depth)
            {
                if (PNULL != win_data_p->s_sel_file_name_ptr)
                {
                    SCI_MEMSET (win_data_p->s_sel_file_name_ptr, 0, ( (FMM_SEARCH_FILENAME_MAXLEN) * sizeof (wchar)));
                    MMI_WSTRNCPY (win_data_p->s_sel_file_name_ptr, FMM_SEARCH_FILENAME_MAXLEN,
                                  win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len, win_data_p->w_handle->s_cur_path_len);
                }

                win_data_p->s_sel_file_name_len = win_data_p->w_handle->s_cur_path_len;

                //获得上一级目录
                win_data_p->w_handle->s_cur_path_len = GetPrevPath (win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len, win_data_p->w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN);

                if (MMIFMM_IsPreviewMusic())
                {
                    MMIAPISET_StopAppRing();
                }

                //打开搜索文件等待窗口
                OpenSearchFileWaitWin (win_data_p->w_handle);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
                SetTileTxt (win_id);
#else
                MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
#endif
            }
            else
            {
                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }

                if (MMK_IsOpenWin (MMIFMM_COPY_SELECT_FILE_WIN_ID))
                {
                    MMK_CloseWin (MMIFMM_COPY_SELECT_FILE_WIN_ID);
                }
            }
        }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_KEYLONG_CANCEL:

        if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
        {
            MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
        }

        break;
#endif
#if defined TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT //coverity 12864
//#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:

        if (PNULL != param)
        {
            uint32 src_id = ( (MMI_NOTIFY_T*) param)->src_id;

            if (src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {

                if (FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type || win_data_p->w_handle->s_is_sel_folder)
                {
                    SCI_MEMSET (&f_array_data, 0, sizeof (f_array_data));
                    f_array_data.type = FILEARRAY_TYPE_FOLDER;
                    MMI_WSTRNCPY (f_array_data.filename, FMM_SEARCH_FILENAME_MAXLEN,
                                  win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len, win_data_p->w_handle->s_cur_path_len);
                    f_array_data.name_len = win_data_p->w_handle->s_cur_path_len;

                    if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                    {
                        MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);

                        win_data_p->w_handle->s_is_ret_succ = TRUE;
                        win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                    }

                    if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                    {
                        MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                    }
                }
                else
                {
                    MMK_SendMsg (win_id, MSG_APP_OK, PNULL);
                }

                break;

            }
            else if (src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }

                break;
            }
            else if (src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {

                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }

                break;
            }

        }

        cur_sel = GUILIST_GetCurItemIndex (ctrl_id);

        if (!MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_sel, &f_array_data))
        {
            break;
        }

        ///if (0 == s_mark_num)
        {
            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
            {
                //获得下一级目录
                SCI_MEMSET (win_data_p->w_handle->s_cur_path, 0, sizeof (win_data_p->w_handle->s_cur_path));
                MMI_WSTRNCPY (win_data_p->w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                              f_array_data.filename, f_array_data.name_len, f_array_data.name_len);
                win_data_p->w_handle->s_cur_path_len = f_array_data.name_len;

                if (MMIFMM_IsPreviewMusic()) //如果在预览铃声，则停掉audio
                {
                    MMIAPISET_StopAppRing();
                }

                //打开搜索文件等待窗口
                OpenSearchFileWaitWin (win_data_p->w_handle);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
                SetTileTxt (win_id);
#else
                MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
#endif
                if (PNULL != win_data_p->w_handle->s_handle_func)
                {
                    win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
                }
            }
            else if (FILEARRAY_TYPE_FILE == f_array_data.type
                     && FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type
                     && !win_data_p->w_handle->s_is_multi_sel)
            {
                BOOLEAN  need_close_parent_win = FALSE;
                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);

                    win_data_p->w_handle->s_is_ret_succ = TRUE;
                    win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
                    need_close_parent_win = TRUE;
                }
                
                if (PNULL != win_data_p->w_handle->s_handle_func)
                {
                    win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
                }
                
                if (need_close_parent_win)
                {
                    if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                    {
                        MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                    }
                }
            }
        }
        break;
//#endif //TOUCH_PANEL_SUPPORT //IGNORE9527                      
#else
//#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527//coverity 12864
    case MSG_CTL_PENOK:
//#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#if defined TOUCH_PANEL_SUPPORT
        if (MSG_CTL_PENOK == msg_id)
        {
            uint32 src_id = ( (MMI_NOTIFY_T*) param)->src_id;

            if (MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID != src_id)
            {
                break;
            }
        }

#endif
        cur_sel = GUILIST_GetCurItemIndex (ctrl_id);

        if (!MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_sel, &f_array_data))
        {
            break;
        }

        if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
        {
            //获得下一级目录
            SCI_MEMSET (win_data_p->w_handle->s_cur_path, 0, sizeof (win_data_p->w_handle->s_cur_path));
            MMI_WSTRNCPY (win_data_p->w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                          f_array_data.filename, f_array_data.name_len, f_array_data.name_len);
            win_data_p->w_handle->s_cur_path_len = f_array_data.name_len;

            if (MMIFMM_IsPreviewMusic()) //如果在预览铃声，则停掉audio
            {
                MMIAPISET_StopAppRing();
            }

            //打开搜索文件等待窗口
            OpenSearchFileWaitWin (win_data_p->w_handle);
#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
            SetTileTxt (win_id);
#else
            MMIFMM_SetWinTitle (TRUE, win_id, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
#endif
            if (PNULL != win_data_p->w_handle->s_handle_func)
            {
                win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
                is_preview = MMK_IsOpenWin (MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID) || MMK_IsOpenWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID);

                if (!is_preview && FILEARRAY_TYPE_FILE == f_array_data.type && MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
        }
        else if (FILEARRAY_TYPE_FILE == f_array_data.type
                 && FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type
                 && !win_data_p->w_handle->s_is_multi_sel)
        {
            if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
            {
                MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_ret_array, &f_array_data);

                win_data_p->w_handle->s_is_ret_succ = TRUE;
                win_data_p->w_handle->s_callback (TRUE, win_data_p->w_handle->s_ret_array);
            }
            if (PNULL != win_data_p->w_handle->s_handle_func)
            {
                win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
                is_preview = MMK_IsOpenWin (MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID) || MMK_IsOpenWin (MMIFMM_SEL_VIDEO_PREVIEW_WIN_ID)
					||MMK_IsOpenWin (MMIFMM_ALERT_VIDEO_PREVIEW_WIN_ID);//CR120028

                if (!is_preview && FILEARRAY_TYPE_FILE == f_array_data.type && MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
            else
            {
                if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
                {
                    MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
                }
            }
        }
        break;

    case MSG_FMM_CLOSE_LAYER_WIN_BY_CLIENT://用户需要关掉该窗口，直接关窗口，给第三方用

        if (MMK_IsOpenWin (MMIFMM_OPEN_FILE_WIN_ID))
        {
            MMK_CloseParentWin (MMIFMM_OPEN_FILE_WIN_ID);
        }

        MMK_CloseWin (MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID);
        break;

    case MSG_CLOSE_WINDOW:
        APPFILE_StopFindFile();

        while (APPFILE_IsFindingFile())
        {
            SCI_Sleep (100);
        }

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

        SubOpenFileWinCloseCtrlFunc (win_id);
        break;
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:

        if (PNULL != win_data_p->w_handle->s_handle_func)
        {
            result = win_data_p->w_handle->s_handle_func (win_id, msg_id, param);
        }

        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}



/*****************************************************************************/
// Description : 获得上一级目录
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL uint16 GetPrevPath (
    wchar  *cur_path,
    uint16 cur_path_len,
    wchar  *ret_path,
    uint16 ret_path_max_len
)
{
    uint16      len = 0;

    //SCI_ASSERT(NULL != cur_path);
    //SCI_ASSERT(NULL != ret_path);
    if ( (PNULL == cur_path) || (PNULL == ret_path))
    {
        //SCI_TRACE_LOW:"MMIFMM:GetPrevPath param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_2941_112_2_18_2_20_25_312, (uint8*) "");
        return 0;
    }

    if (0 == cur_path_len)
    {
        return 0;
    }

    len = cur_path_len;

    while (0 < len && MMIFILE_SLASH != cur_path[len-1]) //找到"\"的位置
    {
		ret_path[len] = 0;
        len--;
    }

    if (0 < len && MMIFILE_SLASH == cur_path[len-1])
    {
        len--;
		ret_path[len] = 0;
        MMI_WSTRNCPY (ret_path, ret_path_max_len,
                      cur_path, len, len);
    }
    else
    {
        len = 0;
    }

    return len;
}


/*****************************************************************************/
// Description : 把文件load到list中
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void LoadFileToList (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T   ctrl_id,
    FILEARRAY       file_array
)
{
    uint16         i = 0;
    uint16         total_num = 0;
    GUILIST_ITEM_T item_t = {0};
    MMI_HANDLE_T   list_handle = MMK_ConvertIdToHandle (ctrl_id);
	
#ifndef MMI_PDA_SUPPORT
//	GUIITEM_STYLE_E                 style_id = 0;
    FILEARRAY_DATA_T     f_array_data_append = {0};
#endif
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_ASSERT(NULL != file_array);
    if (PNULL == file_array)
    {
        //SCI_TRACE_LOW:"LoadFileToList param error:PNULL == file_array"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3014_112_2_18_2_20_25_313, (uint8*) "");
        return;
    }

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

    total_num = (uint16) MMIAPIFILEARRAY_GetArraySize (file_array);
	//CR85604 liyan.zhu
	//MMIAPIFILEARRAY_Sort(file_array,FILEARRAY_SORT_TYPE_ASCEND);// xfind，不需要用filearray来sort
#ifdef  CAMERA_SUPPORT
    //add by @dream.chen for udisk remove Udisk item when in dv
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)

    if (MMIAPIDC_GetDVSavePathState())
    {
        FILEARRAY_DATA_T     f_array_data = {0};
        SCI_MEMSET (&f_array_data, 0x00, sizeof (FILEARRAY_DATA_T));
        MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, 0, &f_array_data);

        //if (2 == f_array_data.name_len
        //&& FMM_U_DISK == f_array_data.filename[0]
        //&& MMIFILE_COLON == f_array_data.filename[1])

        if (MMI_DEVICE_UDISK == MMIAPIFMM_GetDeviceTypeByPath (f_array_data.filename, f_array_data.name_len))
        {
            total_num = ( (total_num > 0) ? (total_num - 1) : (total_num));

            MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, 1, &f_array_data);
            MMIAPIFILEARRAY_Destroy (&win_data_p->w_handle->s_file_array);
            win_data_p->w_handle->s_file_array = NULL;
            win_data_p->w_handle->s_file_array = MMIAPIFILEARRAY_Create();

            //SCI_ASSERT(NULL != win_data_p->w_handle->s_file_array);
            if (PNULL == win_data_p->w_handle->s_file_array)
            {
                //SCI_TRACE_LOW:"MMIFMM:LoadFileToList error ! PNULL == win_data_p->w_handle->s_file_array"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3045_112_2_18_2_20_25_314, (uint8*) "");
                return;
            }

            MMIAPIFILEARRAY_Add (win_data_p->w_handle->s_file_array, &f_array_data);
        }
    }

#endif
#endif  //#ifdef  CAMERA_SUPPORT

    if (GUILIST_GetTotalItemNum (list_handle))
    {
        GUILIST_RemoveAllItems (list_handle);
    }

    //显示空列表
    if (0 == total_num)
    {
        GUILIST_SetTitleIndexType (list_handle, GUILIST_TITLE_INDEX_NONE);

        GUILIST_SetMaxItem (list_handle, 1, FALSE);
        GUILIST_SetMaxSelectedItem (ctrl_id, 0);
        MMIAPISET_AppendListItemByTextId (TXT_BLANK, TXT_NULL, list_handle, GUIITEM_STYLE_ONE_LINE_TEXT);

        if (win_data_p->w_handle->s_is_multi_sel)
        {
            GUILIST_SetItemStateById(list_handle, 0, GUIITEM_STATE_DISABLE_MARK, TRUE);
        }
    }
    else
    {

        GUILIST_SetTitleIndexType (list_handle, GUILIST_TITLE_INDEX_DEFAULT);

        GUILIST_SetMaxItem (list_handle, total_num, TRUE);
        GUILIST_SetMaxSelectedItem (ctrl_id, total_num);

#ifdef MMI_PDA_SUPPORT
        if (WIN_DISP_1LINE == win_data_p->w_handle->s_disp_type 
            || win_data_p->w_handle->s_is_sel_folder)
        {
            //  if((win_data_p->w_handle->s_func_type != FUNC_FIND_FOLDER) && (!win_data_p->w_handle->s_is_multi_sel))
            {
                item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
            }

            // else
            if ( (win_data_p->w_handle->s_func_type == FUNC_FIND_FOLDER) || (win_data_p->w_handle->s_is_multi_sel) || (win_data_p->w_handle->s_is_sel_folder))
            {
                //    item_t.item_style =GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT;
                item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
            }
        }
        else
        {
            if (win_data_p->w_handle->s_func_type != FUNC_FIND_FOLDER && (!win_data_p->w_handle->s_is_multi_sel))
            {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
            }
            else
            {
                item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON;
                //item_t.item_state |= GUIITEM_STATE_CONTENT_CHECK;
            }
        }

        for (i = 0; i < total_num; i++)
        {
            GUILIST_AppendItem (list_handle, &item_t);
        }
		//CR223997 liyan.zhu
		//根据不同的item赋不同的style(单行icon_text(文件夹类型)/单行icon_two_text(非图片文件类型)/双行(图片类型))
#else
        for(i=0; i<total_num; i++)
        {
			SCI_MEMSET(&f_array_data_append,0x00,sizeof(FILEARRAY_DATA_T));
			if(!MMIAPIFILEARRAY_Read(file_array, i, &f_array_data_append))
			{
				SCI_TRACE_LOW("[FMM]LoadFileToList:read file array failed!");
                item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
                GUILIST_AppendItem(list_handle, &item_t);
				continue;
			}
			
			if (FILEARRAY_TYPE_FOLDER == f_array_data_append.type)
			{

                item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
			}
			else
			{
#ifdef DRM_SUPPORT
				if(WIN_DISP_2LINE == win_data_p->w_handle->s_disp_type 
					&&(FILEARRAY_TYPE_FILE == f_array_data_append.type)
					&& MMIAPIFMM_IsDrmFileBySuffix(f_array_data_append.filename, f_array_data_append.name_len)) 
				{
                    item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
				}
				else
				{
#endif
					if(WIN_DISP_2LINE == win_data_p->w_handle->s_disp_type
						&&(FILEARRAY_TYPE_FILE == f_array_data_append.type)
						&& MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_GetFileTypeByFileName(f_array_data_append.filename, f_array_data_append.name_len))
					{

                        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
					}
					else
					{
                        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;
					}
#ifdef DRM_SUPPORT
				}
#endif
			}
			GUILIST_AppendItem(list_handle, &item_t);
		}
#endif 
    }
}

/*****************************************************************************/
// Description : 把item load到list中
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL void LoadItemToList (
    MMI_WIN_ID_T win_id,
    MMI_CTRL_ID_T   ctrl_id,
    uint16          item_index,
    FILEARRAY       file_array,
    GUILIST_ITEM_DATA_T   *item_data_ptr,
    uint16          item_content_index,
    BOOLEAN         is_item_data
)
{
    //static FILEARRAY_DATA_T     f_array_data = {0};
    FILEARRAY_DATA_T     f_array_data = {0};
    wchar           file_name[FILEARRAY_FILENAME_MAX_LEN+1] = {0};
    uint16          file_name_len = 0;
    MMI_STRING_T    str_text = {0};
    MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
    GUIANIM_FILE_INFO_T anim_path = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    //GUILIST_ITEM_T item_t = {0};/*lint !e64*/
    uint8            file_size_info_index = 2;
#ifdef MMI_PDA_SUPPORT
    BOOLEAN          is_show_preview_icon = FALSE;
#endif
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);

    //SCI_ASSERT(NULL != file_array);
    //SCI_ASSERT(NULL != item_data_ptr);
    if ( (PNULL == file_array) || (PNULL == item_data_ptr))
    {
        //SCI_TRACE_LOW:"LoadItemToList param error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3149_112_2_18_2_20_25_315, (uint8*) "");
        return;
    }

    item_data_ptr->softkey_id[0] = win_data_p->w_handle->s_leftsoft_id;
    item_data_ptr->softkey_id[1] = win_data_p->w_handle->s_middlesoft_id;
    item_data_ptr->softkey_id[2] = win_data_p->w_handle->s_rightsoft_id;

    SCI_MEMSET (&f_array_data, 0x00, sizeof (FILEARRAY_DATA_T));

    if (!MMIAPIFILEARRAY_Read (file_array, item_index, &f_array_data))
    {
        //SCI_TRACE_LOW:"[FMM] LoadItemToList read file array failed !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3161_112_2_18_2_20_25_316, (uint8*) "");
        return;
    }

    if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
    {
        GUILIST_SetItemStateById(ctrl_id, item_index, GUIITEM_STATE_DISABLE_MARK, TRUE);

#ifdef MMI_PDA_SUPPORT

        if (PNULL != win_data_p->w_handle->s_select_cbk)
        {
            //如果s_select_cbk != PNULL，则不可以选择整个文件夹，如视频墙纸，需要判断
            //某视频是否可以设为墙纸。
            GUILIST_SetItemStyle (ctrl_id, GUIITEM_STYLE_ONE_LINE_ICON_TEXT, item_index);
        }
        else
        {
            if ( (win_data_p->w_handle->s_func_type == FUNC_FIND_FOLDER) || (win_data_p->w_handle->s_is_multi_sel) || win_data_p->w_handle->s_is_sel_folder)
            {

                GUILIST_SetItemStyle (ctrl_id, GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT, item_index);
            }
            else
            {
                GUILIST_SetItemStyle (ctrl_id, GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT, item_index);
            }
        }

#endif
        //GUILIST_SetItemStateById(ctrl_id, item_index, GUIITEM_STATE_DISABLE_MARK, TRUE);
        //GUILIST_SetItemState ( (GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, item_index), GUIITEM_STATE_DISABLE_MARK, TRUE);/*lint -e605*/

    }

#ifdef MMI_PDA_SUPPORT

    if (s_is_have_preview_icon)
    {
        is_show_preview_icon = TRUE;

        if (WIN_DISP_1LINE == win_data_p->w_handle->s_disp_type)
        {
            if (FILEARRAY_TYPE_FOLDER != f_array_data.type) //maryxiao crNEWMS00126823
            {

                GUILIST_SetItemStyle (ctrl_id, GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON, item_index);
            }

        }
        else
        {
            if (FILEARRAY_TYPE_FOLDER != f_array_data.type)
            {
                GUILIST_SetItemStyle (ctrl_id, GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON, item_index);
            }
        }

        GUILIST_SetItemState ( (GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (ctrl_id, item_index), GUIITEM_STATE_CONTENT_CHECK, TRUE);

    }

#endif

    if (WIN_DISP_2LINE == win_data_p->w_handle->s_disp_type
            && (FILEARRAY_TYPE_FILE == f_array_data.type)
            && MMIFMM_FILE_TYPE_PICTURE == MMIAPIFMM_GetFileTypeByFileName (f_array_data.filename, f_array_data.name_len))
    {
        //双行显示，并且是图片文件，则显示缩略图
        item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_data_ptr->item_content[0].item_data.anim_path_ptr = &anim_path;
        anim_path.full_path_wstr_ptr = f_array_data.filename;
        anim_path.full_path_wstr_len = f_array_data.name_len;
#ifdef MMI_PDA_SUPPORT
        item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data_ptr->item_content[2].item_data.image_id = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;

        if (MMIAPISET_IsWallpaperCall()
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
        ||MMIAPISET_IsPoweronCall()||MMIAPISET_IsPoweroffCall()
#endif
#ifdef SCREENSAVER_SUPPORT
        ||MMIAPISET_IsScreensaverCall()
#endif
	)
        {
            file_size_info_index = 2;
        }
        else
        {
            file_size_info_index = 3;
        }

#endif

    }

#ifdef DRM_SUPPORT
    else if (WIN_DISP_2LINE == win_data_p->w_handle->s_disp_type
             && (FILEARRAY_TYPE_FILE == f_array_data.type)
             && MMIAPIFMM_IsDrmFileBySuffix (f_array_data.filename, f_array_data.name_len))
    {
        MMI_IMAGE_ID_T  icon_id = IMAGE_NULL;

        //is drm picture
        if (MMIFMM_FILE_TYPE_PICTURE ==
                MMIAPIDRM_GetMediaFileType (SFS_INVALID_HANDLE, f_array_data.filename))
        {
            //is it has right
            if (MMIAPIDRM_IsRightsValid (SFS_INVALID_HANDLE, f_array_data.filename, TRUE, DRM_PERMISSION_DISPLAY))
            {
                icon_id = IMAGE_DRM_PIC_UNLOCK_ICON;
            }
            else
            {
                icon_id = IMAGE_DRM_PIC_LOCK_ICON;
            }
        }
        else
        {
            icon_id = IMAGE_DRM_PIC_UNKNOW_ICON;
        }

        item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data_ptr->item_content[0].item_data.image_id = icon_id;
    }

#endif
    else
    {
        //显示icon
        item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;

        if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
        {
            if (FUNC_FIND_FILE == win_data_p->w_handle->s_func_type
                    || ( (FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type)
                         && (!win_data_p->w_handle->s_is_multi_sel))
               )

            {
                //only select file
                item_data_ptr->softkey_id[0] = TXT_COMM_OPEN;
                item_data_ptr->softkey_id[1] = TXT_NULL;
            }
            else
            {
                item_data_ptr->softkey_id[0] = STXT_SELECT;
                item_data_ptr->softkey_id[1] = TXT_COMM_OPEN;

            }

            {
                item_data_ptr->item_content[0].item_data.image_id = IMAGE_FMM_FOLDER_ICON;
#ifdef MMI_PDA_SUPPORT

                if (win_data_p->w_handle->s_func_type == FUNC_FIND_FOLDER || win_data_p->w_handle->s_is_sel_folder)
                {
                    item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
                    item_data_ptr->item_content[2].item_data.image_id = IMG_FMM_NEW_FOLDER_MENU_ICON;
                }
                else if (win_data_p->w_handle->s_is_multi_sel)
                {
                    if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
                    {
                        item_data_ptr->item_content[2].item_data_type = GUIITEM_DATA_IMAGE_ID;
                        item_data_ptr->item_content[2].item_data.image_id = IMG_FMM_NEW_FOLDER_MENU_ICON;
                    }
                }

#endif
            }
        }
        else
        {
            item_data_ptr->item_content[0].item_data.image_id = MMIFMM_GetIconIdByFileName (f_array_data.filename, f_array_data.name_len);

            //最后的 preview icon
#ifdef MMI_PDA_SUPPORT

            if (is_show_preview_icon)
            {
                item_data_ptr->item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data_ptr->item_content[3].item_data.image_id = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
            }

#endif
        }
    }

    //get file name
    device = MMIAPIFMM_GetDeviceTypeByPath (f_array_data.filename, MMIFILE_DEVICE_LEN);

    if (2 == f_array_data.name_len
            && (device < MMI_DEVICE_NUM)
            && MMIFILE_COLON == f_array_data.filename[1])
    {
        //U disk root
        MMI_GetLabelTextByLang (MMIAPIFMM_GetDeviceName (device), &str_text);

        file_name_len = str_text.wstr_len;
        MMI_WSTRNCPY (file_name, FILEARRAY_FILENAME_MAX_LEN, str_text.wstr_ptr, str_text.wstr_len, str_text.wstr_len);
    }
    else if (!MMIAPIFMM_SplitFullPath (f_array_data.filename, f_array_data.name_len,
                                       NULL, NULL, NULL, NULL,
                                       file_name, &file_name_len)
            )
    {
        MMI_WSTRNCPY (file_name, FILEARRAY_FILENAME_MAX_LEN, f_array_data.filename, f_array_data.name_len, f_array_data.name_len);

        file_name_len = f_array_data.name_len;
    }

    //text
    item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data_ptr->item_content[1].item_data.text_buffer.wstr_len = file_name_len;

    item_data_ptr->item_content[1].item_data.text_buffer.wstr_ptr = file_name;

    /*Start of   2009.9.2 for 修改CR MS00150754 */
    if (FILEARRAY_TYPE_FILE == f_array_data.type)
    {

        item_data_ptr->item_content[file_size_info_index].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

        MMIAPIFMM_GetFileSizeString (f_array_data.size,
                                     temp_wstr,
                                     MMIFMM_SIZE_STR_LEN,
                                     TRUE);
        item_data_ptr->item_content[file_size_info_index].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data_ptr->item_content[file_size_info_index].item_data.text_buffer.wstr_len = (uint16) MMIAPICOM_Wstrlen (temp_wstr);
    }

    /*End   of   2009.9.2 for 修改CR MS00150754 */

    if (is_item_data)
    {
        GUILIST_SetItemData (ctrl_id, item_data_ptr, item_index);
    }
    else
    {
        GUILIST_SetItemContent (ctrl_id, &item_data_ptr->item_content[item_content_index], item_index, item_content_index);
    }
}

/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_GetFileTypeByFileName (
    const wchar  *file_name,
    uint16       file_name_len
)
{
    uint16      suffix_name[FMM_FILE_SUFFIX_MAXLEN] = {0};
    uint16      suffix_len = 0;
    MMIFMM_FILE_TYPE_E  file_type = MMIFMM_FILE_TYPE_NORMAL;

    //SCI_ASSERT(NULL != file_name);
    if (PNULL == file_name)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_GetFileTypeByFileName param error:PNULL == file_name"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3567_112_2_18_2_20_26_320, (uint8*) "");
        return MMIFMM_FILE_TYPE_NUM;
    }

    //获取后缀名
    suffix_len = FMM_FILE_SUFFIX_MAXLEN - 1;

    if (MMIAPIFMM_SplitFileName (file_name, file_name_len,
                                 PNULL, PNULL,
                                 suffix_name, &suffix_len))
    {
        file_type = MMIAPIFMM_ConvertFileType (suffix_name, suffix_len);

    }

    return file_type;
}



/*****************************************************************************/
//  Description : 打开搜索文件等待窗口
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void OpenSearchFileWaitWin (MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle)
{
    MMI_STRING_T    wait_text = {0};
    MMI_WIN_ID_T    win_id = MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID;

    //进入打开文件等待界面
    if (!MMK_IsOpenWin (win_id))
    {
        MMI_GetLabelTextByLang (TXT_COMMON_WAITING, &wait_text);
        MMIPUB_OpenWaitWin (1, &wait_text, PNULL, PNULL, win_id,
                            COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
                            MMIPUB_SOFTKEY_CUSTOMER, HandleOpenFileByLayerWaitWin);
        MMIPUB_SetWaitWinIsupdatedelay(win_id, FALSE); //yanyan.an add
        MMIPUB_SetWinSoftkey (win_id, TXT_NULL, STXT_CANCEL, FALSE);

        LayerWaitWinOpenCtrlFunc (win_id, w_handle);

        MMK_PostMsg (win_id, MSG_FMM_OPENFILEWIN_STARTSEARCH, NULL, 0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIFMM]OpenSearchFileWaitWin waitwin already opened !"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3788_112_2_18_2_20_27_323, (uint8*) "");
    }
}
/*****************************************************************************/
//  Description : 打开文件列表主窗口
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void OpenFileListMainWin (MMIFMM_OPEN_LAYER_WIN_DATA_T* w_handle)
{
    MMK_CreateWin ( (uint32 *) MMIFMM_OPENFILE_WIN_TAB, (ADD_DATA) w_handle);
}

/*****************************************************************************/
//  Description :
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetOpenFileWinCurFile (
    FILEARRAY_DATA_T *file_data_ptr
)
{
    uint16      cur_sel = 0;
    BOOLEAN     result = FALSE;

    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);

    if (PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIFMM_GetOpenFileWinCurFile param error:PNULL == file_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3955_112_2_18_2_20_27_324, (uint8*) "");
        return FALSE;
    }

    cur_sel = GUILIST_GetCurItemIndex (MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID);

    if (PNULL!=win_data_p && PNULL != win_data_p->w_handle)
    {
        result = MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, cur_sel, file_data_ptr);
    }
    return result;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :
//  Return:
//  Global resource dependence :
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetIsHavePreviewIcon (BOOLEAN    is_show)
{
    s_is_have_preview_icon = is_show;
}
#endif

/*****************************************************************************/
//  Description :
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetOpenFileWinCurFile (
    uint16   *file_name_ptr,
    uint16   file_name_len
)
{
    MMI_CTRL_ID_T   ctrl_id = MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID;
    uint32          file_num = 0;
    uint32           i = 0;
    FILEARRAY_DATA_T    file_data = {0};

    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);

    if (NULL == file_name_ptr || 0 == file_name_len)
    {
        GUILIST_SetCurItemIndex (ctrl_id, 0);
        return;
    }

    file_num = MMIAPIFILEARRAY_GetArraySize (win_data_p->w_handle->s_file_array);

    for (i = 0; i < file_num; i++)
    {
        SCI_MEMSET (&file_data, 0, sizeof (file_data));

        if (MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, i, &file_data))
        {
            if (0 == MMIAPICOM_CompareTwoWstr (file_name_ptr, file_name_len, file_data.filename, file_data.name_len))
            {
                break;
            }
        }
    }

    if (i < file_num)
    {
        //设置第i项为当前项
        GUILIST_SetCurItemIndex (ctrl_id, (uint16) i);
    }
    else
    {
        GUILIST_SetCurItemIndex (ctrl_id, 0);
    }
}

/*****************************************************************************/
//  Description :
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_GetOpenFileWinFile (
    uint16        index,
    FILEARRAY_DATA_T  *file_data_ptr
)
{
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);

    if (PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM_GetOpenFileWinFile param error:PNULL == file_data_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4047_112_2_18_2_20_27_326, (uint8*) "");
        return FALSE;
    }

    return MMIAPIFILEARRAY_Read (win_data_p->w_handle->s_file_array, index, file_data_ptr);
}

/*****************************************************************************/
//  Description :
//  Return:
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetOpenFileWinSoftkey (
    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
    MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id 传过来的不一定是TEXT_ID有可能是_IMAGE_ID
    MMI_TEXT_ID_T   rightsoft_id   // the right softkey id
)
{
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);

    if (PNULL != win_data_p && PNULL != win_data_p->w_handle)
    {
        win_data_p->w_handle->s_leftsoft_id = leftsoft_id;
        win_data_p->w_handle->s_middlesoft_id = middlesoft_id;
        win_data_p->w_handle->s_rightsoft_id = rightsoft_id;
    }
}

/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIFMM_OpenFileWinByLayerEx (MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info)
{
    MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E valid_dev = MMI_DEVICE_UDISK;
    wchar  *path_name_ptr = PNULL;
    uint16 path_name_len = 0;
    uint16 default_path_len = 0;
    wchar default_path[MMIFMM_DEFAULT_PATH_LEN] = {0};
    MMIFMM_OPEN_LAYER_WIN_DATA_T * w_handle = PNULL;

    //SCI_ASSERT(NULL != filter_ptr);
    if (!MMIAPIUDISK_IsDeviceReady())
    {
        //手机无存储卡
        return FMM_OPENWIN_FILE_NOT_EXIST;
    }

    if (PNULL == open_layer_info->filter_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM_OpenFileWinByLayerInternal param error:PNULL == filter_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4178_112_2_18_2_20_27_329, (uint8*) "");
        return FMM_OPENWIN_FILE_NOT_EXIST;
    }

    path_name_ptr = (wchar *) SCI_ALLOC_APPZ ( (MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof (wchar));

    if (path_name_ptr == PNULL)
    {
        return FMM_OPENWIN_FILE_NOT_EXIST;
    }
    MMK_CloseWin (MMIFMM_OPEN_FILE_WIN_ID);
    MMK_CloseWin (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);
    MMK_CloseWin (MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID);


    if (!open_layer_info->is_internal_call)
    {
        MMIAPIFMM_InitFolders();
    }

    if (PNULL != open_layer_info->path_ptr)
    {
        MMIAPICOM_Wstrncpy (path_name_ptr, open_layer_info->path_ptr, MIN (MMIFILE_FULL_PATH_MAX_LEN, open_layer_info->path_len));
        path_name_len = open_layer_info->path_len;

        if (!MMIAPIFMM_IsFolderExist (path_name_ptr, path_name_len))
        {
            //目录不存在,退回到根目录
            path_name_len = 2;
            device = MMIAPIFMM_GetDeviceTypeByPath (path_name_ptr, MMIFILE_DEVICE_LEN);

            if (device < MMI_DEVICE_NUM)
            {
                if (!MMIAPIFMM_GetDeviceStatus ( (uint16 *) MMIAPIFMM_GetDevicePath (device), MMIAPIFMM_GetDevicePathLen (device)))
                {
                    device = MMIAPIFMM_GetDefaultDisk();
#ifdef  CAMERA_SUPPORT
                    //add by @dream.chen for udisk remove Udisk item when in dv
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)

                    if ( (device == MMI_DEVICE_UDISK) && MMIAPIDC_GetDVSavePathState())
                    {
                        MMIAPIDC_SetDVSavePathState (FALSE);
                        //prompt info
                        SCI_FREE (path_name_ptr);
                        return FMM_OPENWIN_FILE_NOT_EXIST;
                    }

#endif
#endif  //#ifdef  CAMERA_SUPPORT 

                    if (device < MMI_DEVICE_NUM)
                    {
                        path_name_ptr[0] = *MMIAPIFMM_GetDevicePath (device);
                        path_name_ptr[1] = MMIFILE_COLON;
                        path_name_len = 2;
                    }
                }
            }
        }
    }

    w_handle = (MMIFMM_OPEN_LAYER_WIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_OPEN_LAYER_WIN_DATA_T));

    InitSearchParam (w_handle, path_name_ptr, path_name_len, open_layer_info->filter_ptr);

    w_handle->s_is_recurse = FALSE;
    w_handle->s_is_multi_sel = open_layer_info->is_multi_sel;
    w_handle->s_handle_func = open_layer_info->handle_func;
    w_handle->s_func_type = open_layer_info->func_type;
    w_handle->s_ret_array = open_layer_info->ret_array;
    w_handle->s_callback = open_layer_info->callback;
    w_handle->s_select_cbk = open_layer_info->select_cbk;
    w_handle->s_is_sel_folder = open_layer_info->is_sel_folder;
    w_handle->s_disp_type = open_layer_info->disp_type;

    if (0 == MMIAPICOM_Wstrlen (w_handle->s_cur_path)) //没有初值
    {
        valid_dev = MMIAPIFMM_GetFirstValidDevice();
        MMIAPICOM_Wstrcpy (default_path, MMIAPIFMM_GetDevicePath (valid_dev));
        MMIAPICOM_Wstrcat (default_path, L":");
        default_path_len = (uint16) MMIAPICOM_Wstrlen (default_path);
        MMIAPICOM_Wstrncpy (w_handle->s_cur_path, default_path, default_path_len);
        w_handle->s_cur_path_len = default_path_len;
    }

    w_handle->s_leftsoft_id = STXT_SELECT;
#ifdef MMI_PDA_SUPPORT

    if (open_layer_info->is_multi_sel)
    {
        w_handle->s_leftsoft_id = TXT_COMMON_OK;
    }

#else

    if (open_layer_info->is_multi_sel)
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		w_handle->s_middlesoft_id = IMAGE_COMMON_SOFTKEY_MARKED_ICON;
#else
        w_handle->s_middlesoft_id = STXT_MARK;
#endif
    }
    else
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		w_handle->s_middlesoft_id = IMAGE_NULL;
#else
        w_handle->s_middlesoft_id = TXT_NULL;
#endif
    }

#endif
    w_handle->s_rightsoft_id = STXT_RETURN;

    if ( (!open_layer_info->is_internal_call)  &&  MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_MYDOC))
    {
        //validate privacy password
        //MMIAPISET_ValidatePrivacyAppEntry( MMISET_PROTECT_MYDOC, OpenSearchFileWaitWin);
        MMIAPISET_ValidatePrivacy (
            MMISET_PROTECT_MYDOC,
            FMMLayerWin_ValidatePrivacyCBK,
            (ADD_DATA) w_handle,
            sizeof (MMIFMM_OPEN_LAYER_WIN_DATA_T)
        );
        SCI_FREE(w_handle);
    }
    else
    {
        //search file begin
        OpenSearchFileWaitWin (w_handle);
    }

    SCI_FREE (path_name_ptr);
    //打开搜索文件等待窗口
    //OpenSearchFileWaitWin();

    return FMM_OPENWIN_SUCCESS;
}

/*****************************************************************************/
// Description : open ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FileWinOpenCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle = (MMIFMM_OPEN_LAYER_WIN_DATA_T*) MMK_GetWinAddDataPtr (win_id);
    MMIFMM_FILE_WIN_DATA_T *file_win_data = PNULL;

    if (PNULL == w_handle)
    {
        return MMI_RESULT_FALSE;
    }

    file_win_data = (MMIFMM_FILE_WIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_FILE_WIN_DATA_T));
    file_win_data->w_handle = w_handle;

    MMK_SetWinUserData (win_id, (void *) file_win_data);

    //MMICL_RegisterDateChangeEvent(win_id, MMICL_CALLTIMELIST_WIN_FLAG);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E FileWinCloseCtrlFunc (MMI_WIN_ID_T win_id)
{
    MMIFMM_FILE_WIN_DATA_T *win_data_p = (MMIFMM_FILE_WIN_DATA_T*) MMK_GetWinUserData (win_id);

    if (PNULL != win_data_p)
    {
        if (PNULL != win_data_p->w_handle)
        {
            if (!win_data_p->w_handle->s_is_ret_succ)
            {
                if (NULL != win_data_p->w_handle->s_callback && NULL != win_data_p->w_handle->s_ret_array)
                {
                    win_data_p->w_handle->s_callback (FALSE, win_data_p->w_handle->s_ret_array);
                }
            }

            if (NULL != win_data_p->w_handle->s_file_array)
            {
                MMIAPIFILEARRAY_Destroy (&win_data_p->w_handle->s_file_array);
                win_data_p->w_handle->s_file_array = NULL;
            }
            if (!MMK_IsOpenWin(MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID))
            {
                SCI_FREE (win_data_p->w_handle);
            }
        }

        SCI_FREE (win_data_p);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : enter rename win
// Global resource dependence :
// Author:  jian.ma
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E EnterOpenFileByLayerWin (
    MMIFMM_OPEN_LAYER_WIN_DATA_T *w_handle,
    uint32 current_index
)
{
    MMIFMM_SUBFILEWIN_DATA_T*  subwin_param = PNULL;
    subwin_param = SCI_ALLOC_APPZ (sizeof (MMIFMM_SUBFILEWIN_DATA_T));
    subwin_param->pre_tab_index = current_index;
    subwin_param->w_handle = w_handle;

    if (MMK_CreateChildWin (MMIFMM_OPEN_FILE_WIN_ID, (uint32*) MMIFMM_OPENFILE_BYLAYER_WIN_TAB, (ADD_DATA) subwin_param))
    {
        MMK_SendMsg (MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MSG_FMM_OPENFILEWIN_RELOAD, NULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : MMIFMM_HandleOpenFileWin
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIFMM_HandleOpenFileWin (
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIFILE_DEVICE_E    fmm_dev = MMI_DEVICE_UDISK;
    uint32              base_index = 0;
    MMI_CTRL_ID_T       ctrl_id  = MMIFMM_LAYER_TAB_CTRL_ID;
    uint32              cur_tab_index = 0;
    MMIFILE_DEVICE_E    cur_device  = MMI_DEVICE_UDISK;
    MMI_STRING_T        str_text = {0};
    char                get_label_name[MMIFILE_NEW_DEVICE_NAME_MAX_LEN*3+1] = {0};
    wchar               buffer_name[MMIFMM_FULL_FILENAME_LEN+1] = {0};
    BOOLEAN             get_label_res = FALSE;
    MMIFMM_FILE_WIN_DATA_T *win_data_p = PNULL;//coverity 13029

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

        if (MMI_RESULT_FALSE == FileWinOpenCtrlFunc (win_id))
        {
            MMK_CloseWin (win_id);
            break;
        }

        win_data_p = (MMIFMM_FILE_WIN_DATA_T*) MMK_GetWinUserData (win_id);
        MMK_SetAtvCtrl (win_id, ctrl_id);
        GUITAB_SetMaxItemNum (ctrl_id, MMI_DEVICE_NUM);
        base_index = fmm_dev;

        for (; fmm_dev < MMI_DEVICE_NUM; fmm_dev++)
        {
            if (MMIAPIFMM_GetDeviceTypeStatus (fmm_dev)
#ifdef MSDC_CARD_SUPPORT
                    || MMIAPISD_IsSDNeedFormat (fmm_dev)
#endif
               )
            {
                MMIFMM_AppendTabCtrl (ctrl_id, base_index,  fmm_dev);
                GUITAB_AddChildWindow (
                    ctrl_id,
                    MMIFMM_OPEN_FILE_BYLAYER_WIN_ID,
                    cur_tab_index);

                SetLayerWinDeviceTable (cur_tab_index, fmm_dev);

                if(MMI_DEVICE_UDISK < fmm_dev && fmm_dev < MMI_DEVICE_NUM)
                {
                    get_label_res = MMIAPIFMM_GetDeviceLabel(MMIAPIFMM_GetDevicePath(fmm_dev),get_label_name);
                }
                if(get_label_res)
                {
                    uint16 temp_wstr_len = 0;//pclint bug 2099193
                    SCI_MEMSET(buffer_name,0x00,(sizeof(wchar)*(MMIFMM_FULL_FILENAME_LEN+1)));
                    GUI_OtherCodeToWstr(buffer_name,MMIFMM_FULL_FILENAME_LEN,GUI_GetCodeType(get_label_name,strlen (get_label_name)),(uint8 *)get_label_name,strlen(get_label_name));

                    //  GUI_OtherCodeToWstr(buffer_name,MMIFILE_NEW_DEVICE_NAME_MAX_LEN,GUI_CHAR_CODE_ANSI,(uint8 *)get_label_name,strlen(get_label_name));
                    str_text.wstr_ptr = buffer_name;
                    temp_wstr_len = MMIAPICOM_Wstrlen (buffer_name);
                    str_text.wstr_len = MIN(temp_wstr_len,MMIFILE_NEW_DEVICE_NAME_MAX_LEN);
                }
                else
                {
                    MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(fmm_dev), &str_text);
                }

                GUITAB_SetSimpleItem (MMIFMM_LAYER_TAB_CTRL_ID,
                                      &str_text,
                                      IMAGE_FMM_UDISK_FOCUSED + fmm_dev - base_index,
                                      IMAGE_FMM_UDISK_RELEASED + fmm_dev - base_index,
                                      cur_tab_index);
                cur_tab_index++;

            }

        }
        if(PNULL != win_data_p&& PNULL!=win_data_p->w_handle)
        {
            cur_device = MMIAPIFMM_GetDeviceTypeByPath (win_data_p->w_handle->s_cur_path, 1);
            if(FALSE == GUITAB_SetCurSel(ctrl_id, (cur_device - MMI_DEVICE_UDISK)))/*lint !e656*/
            {
                GUITAB_SetCurSel(ctrl_id, (cur_tab_index - MMI_DEVICE_UDISK));
            }
#ifdef MSDC_CARD_SUPPORT        
        if(MMIAPISD_IsSDNeedFormat(cur_device))
        {
            MMIFMM_FmmMemoryFormat(cur_device, TXT_IS_FORMAT_ERROR_SDCARD);
        }
#endif
            EnterOpenFileByLayerWin (win_data_p->w_handle, GUITAB_GetCurSel (MMIFMM_LAYER_TAB_CTRL_ID));
        }

        break;

    case MSG_CLOSE_WINDOW:
        FileWinCloseCtrlFunc (win_id);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;

}
/*****************************************************************************/
//  Description : ComputTitleShow
//  Global resource dependence :
//  Author: liyan.zhu
//  Note:2012.11.20-for cr 91840:使title显示更自适应
/*****************************************************************************/
PUBLIC void MMIFMM_ComputTitleShow(
                                    MMI_CTRL_ID_T        tab_ctrl_id,
                                    MMI_STRING_T        *title_string,
                                    wchar                *current_pathname_ptr,
                                    wchar               *title1_ptr
                                    )
{
    uint16              title_len = 0;
    GUISTR_STYLE_T      font_style = {0};
    GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN|GUISTR_STATE_ANGLE;
    uint16              logic_width = 0;
    uint16              logic_height = 0;
    uint16              str_width = 0;
    uint16              label_width = 0;
#if(defined (MMI_FMM_TITLE_BUTTON_SUPPORT))&&(!defined(MMI_GUI_STYLE_MINISCREEN))
    uint16              back_button_w = 0;
    uint16              back_button_h = 0;
#endif

    if (PNULL == title_string)
    {
        SCI_TRACE_LOW("[mmifmm_interface.c][MMIFMM_ComputTitleShow]PNULL == title_string");
        return;
    }

    if (PNULL == current_pathname_ptr)
    {
        SCI_TRACE_LOW("[mmifmm_interface.c][MMIFMM_ComputTitleShow]PNULL == current_pathname_ptr");
        return;
    }

    if (PNULL == title1_ptr)
    {
        SCI_TRACE_LOW("[mmifmm_interface.c][MMIFMM_ComputTitleShow]PNULL == title1_ptr");
        return;
    }

    font_style.font = MMI_DEFAULT_TEXT_FONT;
    title_string->wstr_ptr = current_pathname_ptr;
    title_string->wstr_len = MMIAPICOM_Wstrlen(current_pathname_ptr);
    str_width = GUISTR_GetStringWidth(&font_style, title_string, state);
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &logic_width, &logic_height);

#if(defined (MMI_FMM_TITLE_BUTTON_SUPPORT))&&(!defined(MMI_GUI_STYLE_MINISCREEN))
    GUIRES_GetImgWidthHeight(&back_button_w, &back_button_h, IMAGE_COMMON_BACK, MMIFMM_UDISK_CHILD_WIN_ID);
    label_width = logic_width - back_button_w - MMIFMM_TITLE_LABEL_MARGIN;
#else
    label_width = logic_width  - 2*MMIFMM_TITLE_LABEL_MARGIN;
#endif

#if !((defined (MMI_FMM_TITLE_BUTTON_SUPPORT))&&(!defined(MMI_GUI_STYLE_MINISCREEN)))
    if (tab_ctrl_id == 0)
    {
        return;
    }
    if(GUITAB_GetItemState(tab_ctrl_id,0,GUITAB_ITEMSTATE_SHOW_TEXT))
    {//tab上显示
        uint16 tab_count = 0;//bug 2114186 pclint告警
#ifdef DUAL_TCARD_SUPPORT//双卡
#if defined UMEM_SUPPORT || defined NANDBOOT_SUPPORT//三个tab
        ++ tab_count;
#endif
        if (MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), 1))
        {
            ++tab_count;
        }
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1),1))
        {
            ++tab_count;
        }
#else//单卡
#if defined UMEM_SUPPORT || defined NANDBOOT_SUPPORT//两个tab
        ++ tab_count;
#endif
        if (MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), 1))
        {
            ++tab_count;
        }
#endif
        label_width = logic_width/tab_count - MMIFMM_TITLE_LABEL_MARGIN;
    }
#endif

    if(str_width > label_width)
    {
        int suit_len = MMIFMM_WIN_TITLE_MAX_LEN;
        title_string->wstr_len = suit_len + 2;
        str_width = GUISTR_GetStringWidth(&font_style, title_string, state);
        while(str_width < label_width)
        {
            title_string->wstr_len  = title_string->wstr_len + 2;
            str_width = GUISTR_GetStringWidth(&font_style, title_string, state);
        }
        title_string->wstr_len = title_string->wstr_len - 2;
        title_len = title_string->wstr_len - MMIFMM_ELLIPSIS_LEN;
        MMIAPICOM_Wstrncpy(title1_ptr, current_pathname_ptr,  title_len);
        MMIAPICOM_Wstrncpy(title1_ptr + title_len, MMIFMM_ELLIPSIS, MMIFMM_ELLIPSIS_LEN);
        title_string->wstr_ptr = title1_ptr;
        title_string->wstr_len = (uint16)(title_len+MMIFMM_ELLIPSIS_LEN);
    }
}
/*****************************************************************************/
//  Description : MMIFMM_IsListMultiSel
//  Global resource dependence :  TURE 说明该list是要多选的
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsListMultiSel (void)
{
    MMIFMM_FILE_WIN_DATA_T *win_data_p = (MMIFMM_FILE_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_WIN_ID);

    if (PNULL != win_data_p && PNULL != win_data_p->w_handle)
    {
        return win_data_p->w_handle->s_is_multi_sel;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIFMM_SetDeviceTable
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetLayerWinDeviceTable (uint32 tab_index, MMIFILE_DEVICE_E fmm_dev)
{
    MMIFMM_FILE_WIN_DATA_T *win_data_p = (MMIFMM_FILE_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_WIN_ID);

    if (PNULL != win_data_p && fmm_dev < MMI_DEVICE_NUM)
    {
        win_data_p->s_layerwin_device_tab[tab_index] = fmm_dev;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIFMM_SetDeviceTable tab_index = %d,fmm_dev= %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4423_112_2_18_2_20_28_330, (uint8*) "dd", tab_index, fmm_dev);
    }
}
/*****************************************************************************/
//  Description : MMIFMM_SetDeviceTable
//  Global resource dependence :
//  Author: juan.wu
//  Note:tab_index的范围总是在0~(MMI_DEVICE_NUM - MMI_DEVICE_UDISK)之间
//       单T卡tab_index = 0 或1；双T卡tab_index= 0，1，2
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E  GetLayerWinDeviceType (uint32 tab_index)
{
    MMIFMM_FILE_WIN_DATA_T *win_data_p = (MMIFMM_FILE_WIN_DATA_T*) MMK_GetWinUserData (MMIFMM_OPEN_FILE_WIN_ID);

    if (tab_index < (MMI_DEVICE_NUM - MMI_DEVICE_UDISK)) /*lint !e656*/
    {
        return win_data_p->s_layerwin_device_tab[tab_index];
    }
    else
    {
        //SCI_TRACE_LOW:"MMIFMM_GetDeviceType tab_index = %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_4439_112_2_18_2_20_28_331, (uint8*) "d", tab_index);
        return MMI_DEVICE_UDISK;
    }
}


//fixed by liyan.zhu for bugzilla 2760

#ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL void SetTileTxt (MMI_WIN_ID_T win_id)
{
    MMI_CTRL_ID_T  label_ctrl = MMIFMM_OPEN_FILE_BYLAYER_TITLE_LABEL_CTRL_ID;
    MMI_STRING_T   title_info = {0};
    //wchar          title1[MMIFMM_WIN_TITLE_MAX_LEN + 1] = {0};
	wchar               *title1_ptr = PNULL;
    uint16         path_depth = 0;

    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);

	if(PNULL == win_data_p->w_handle)
	{
		SCI_TRACE_LOW("[mmifmm_wintab.c][SetTitleLabelParam]:PNULL == win_data_p->w_handle");
		return;
	}
	
	if(0 == win_data_p->w_handle->s_cur_path_len)
	{
		SCI_TRACE_LOW("[mmifmm_wintab.c][SetTitleLabelParam]:s_cur_path_len=0");
		return;
	}
	
    path_depth = MMIFMM_GetPathDepth (win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);

	title1_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN + 1));	
    if (PNULL == title1_ptr)	
    {			
        SCI_TRACE_LOW("[mmifmm_wintab.c][SetTitleLabelParam]:PNULL == title1_ptr");
		return;	
    }  

    if (1 >= path_depth)
    {
        MMIAPICOM_Wstrncpy (title1_ptr, win_data_p->w_handle->s_cur_path, win_data_p->w_handle->s_cur_path_len);
        MMIAPICOM_Wstrcat (title1_ptr, L"\\");
        title_info.wstr_ptr = title1_ptr;
        title_info.wstr_len = win_data_p->w_handle->s_cur_path_len + 1;
    }
    else
    {
		MMIFMM_ComputTitleShow(0,&title_info,win_data_p->w_handle->s_cur_path,title1_ptr);
#if 0
        uint16              i = 0;
        uint16              title_len = 0;
        GUISTR_STYLE_T      font_style = {0};
        GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN | GUISTR_STATE_ANGLE;
        uint16              str_width = 0;
        uint16              label_width = 0;
        uint16              logic_width = 0;
        uint16              logic_height = 0;
        uint16              back_button_w = 0;
        uint16              back_button_h = 0;

        font_style.font = MMI_DEFAULT_TEXT_FONT;

        title_info.wstr_len = win_data_p->w_handle->s_cur_path_len;
        title_info.wstr_ptr = win_data_p->w_handle->s_cur_path;


        GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &logic_width, &logic_height);
        str_width = GUISTR_GetStringWidth (&font_style, &title_info, state);
        GUIRES_GetImgWidthHeight (&back_button_w, &back_button_h, IMAGE_COMMON_BACK, MMIFMM_OPEN_FILE_BYLAYER_WIN_ID);
        label_width = logic_width - back_button_w - MMIFMM_TITLE_LABEL_MARGIN;

        if (str_width > label_width)
        {
            MMIAPICOM_Wstrncpy (title1, MMIFMM_ELLIPSIS, MMIFMM_ELLIPSIS_LEN);
            i = win_data_p->w_handle->s_cur_path_len + MMIFMM_ELLIPSIS_LEN - MMIFMM_WIN_TITLE_MAX_LEN;
            title_len = MMIFMM_WIN_TITLE_MAX_LEN - MMIFMM_ELLIPSIS_LEN;

            MMIAPICOM_Wstrncpy (&title1[MMIFMM_ELLIPSIS_LEN], &win_data_p->w_handle->s_cur_path[i],  title_len);

            title_info.wstr_ptr = title1;
            title_info.wstr_len = (uint16) (title_len + MMIFMM_ELLIPSIS_LEN);
        }
#endif
    }

    GUILABEL_SetText (label_ctrl, &title_info, FALSE);

    if (PNULL != title1_ptr)	/*lint !e774*/
	{		
	    SCI_Free(title1_ptr);
	    title1_ptr = PNULL;	
    }
    MMIFMM_SetWinTitle (TRUE, MMIFMM_OPEN_FILE_BYLAYER_WIN_ID, MMIFMM_LAYER_TAB_CTRL_ID, win_data_p->w_handle->s_cur_path, MMIFMM_DEVICE_NAME_LEN);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL  MMI_RESULT_E  BackButtonCallBack (void)
{
    MMI_WIN_ID_T  win_id = MMIFMM_OPEN_FILE_BYLAYER_WIN_ID;

    MMK_SendMsg (win_id, MSG_APP_CANCEL, PNULL);
    return MMI_RESULT_TRUE;
}


#endif

/*****************************************************************************/
//  Description : SetLayerwinSoftKey
//  Return:
//  Global resource dependence :
//  Author:
//  Note:设置layerwin窗口的softkey
/*****************************************************************************/
LOCAL void SetLayerwinSoftKey (MMI_WIN_ID_T win_id, uint16 index, FILEARRAY file_array)
{
    FILEARRAY_DATA_T    f_array_data = {0};
#ifndef MMI_PDA_SUPPORT
    MMI_CTRL_ID_T       ctrl_id = MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID;
#endif
    MMIFMM_SUBFILEWIN_DATA_T *win_data_p = (MMIFMM_SUBFILEWIN_DATA_T*) MMK_GetWinUserData (win_id);
    SCI_TRACE_LOW ("SetLayerwinSoftKey enter");

    if (MMIAPIFILEARRAY_Read (file_array, index, &f_array_data))
    {

        if (FILEARRAY_TYPE_FILE == f_array_data.type &&
                FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type &&
                win_data_p->w_handle->s_is_multi_sel)
        {
#ifndef MMI_PDA_SUPPORT//只有非PDA版本才要中间的"取消"和"标记"

            if (GUILIST_GetItemState (GUILIST_GetItemPtrByIndex (ctrl_id, index), GUIITEM_STATE_SELECTED))
            {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					TXT_NULL,
					win_data_p->w_handle->s_rightsoft_id,
					TRUE);
				GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_CANCEL_ICON, 1, TRUE);
#else 
                GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					STXT_CANCEL,
					win_data_p->w_handle->s_rightsoft_id,
					TRUE);
#endif
            }
            else
            {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					TXT_NULL,
					win_data_p->w_handle->s_rightsoft_id,
					TRUE);
				GUIWIN_SeSoftkeytButtonIconId(win_id,IMAGE_COMMON_SOFTKEY_MARKED_ICON, 1, TRUE);
#else 
                GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					STXT_MARK,
					win_data_p->w_handle->s_rightsoft_id,
					TRUE);
#endif
            }

#endif
        }
        else
        {
            if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
            {

                if (FUNC_FIND_FILE == win_data_p->w_handle->s_func_type
                        || ( (FUNC_FIND_FILE_AND_ALLFOLER == win_data_p->w_handle->s_func_type)
                             && (!win_data_p->w_handle->s_is_multi_sel))
                   )

                {
                    //only select file
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_COMM_OPEN,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE);
					GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_COMM_OPEN,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE); 
#endif 
                }
                else
                {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE); 
					GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_OK_ICON, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_COMM_OPEN,win_data_p->w_handle->s_rightsoft_id,TRUE); 
#endif   
                }
            }
            else if (win_data_p->w_handle->s_is_sel_folder || FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type)
            {
                if (FILEARRAY_TYPE_FILE == f_array_data.type)
                {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE);
					GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE);
#endif      
                }
                else if (FILEARRAY_TYPE_FOLDER == f_array_data.type)
                {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE); 
					GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_OK_ICON, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_COMM_OPEN,win_data_p->w_handle->s_rightsoft_id,TRUE); 
#endif     
                }
            }
            else
            {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					TXT_NULL,
					win_data_p->w_handle->s_rightsoft_id,TRUE);
				GUIWIN_SeSoftkeytButtonIconId(win_id, win_data_p->w_handle->s_middlesoft_id, 1, TRUE);
#else
				GUIWIN_SetSoftkeyTextId(win_id,
					win_data_p->w_handle->s_leftsoft_id,
					win_data_p->w_handle->s_middlesoft_id,
					win_data_p->w_handle->s_rightsoft_id,TRUE);
#endif
            }
        }
    }
    else if (win_data_p->w_handle->s_is_sel_folder || FUNC_FIND_FOLDER == win_data_p->w_handle->s_func_type)
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE);  
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
        GUIWIN_SetSoftkeyTextId(win_id,STXT_SELECT,TXT_NULL,win_data_p->w_handle->s_rightsoft_id,TRUE);     
#endif
    }
    else
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, TRUE);
#else
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
#endif
    }
}
/*****************************************************************************/
//  Description : FMMLayerWin_ValidatePrivacyCBK
//  Return:
//  Global resource dependence :
//  Author:
//  Note:开启隐私保护的处理
/*****************************************************************************/
LOCAL void FMMLayerWin_ValidatePrivacyCBK (BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMIFMM_OPEN_LAYER_WIN_DATA_T* w_handle = PNULL;// (MMIFMM_OPEN_LAYER_WIN_DATA_T*) param_ptr;

    if(PNULL == param_ptr)
    {
        SCI_TRACE_LOW("FMMLayerWin_ValidatePrivacyCBK PNULL == param_ptr");
        return;
    }
    w_handle = (MMIFMM_OPEN_LAYER_WIN_DATA_T *) SCI_ALLOC_APPZ (sizeof (MMIFMM_OPEN_LAYER_WIN_DATA_T));
    if(PNULL == w_handle)
    {
        SCI_TRACE_LOW("FMMLayerWin_ValidatePrivacyCBK PNULL == w_handle");
        return;
    }
    SCI_MEMCPY(w_handle, param_ptr, sizeof (MMIFMM_OPEN_LAYER_WIN_DATA_T));
    if (is_permitted)
    {
        OpenSearchFileWaitWin (w_handle);
    }
    else
    {
        if (PNULL != w_handle->s_callback && PNULL != w_handle->s_ret_array)
        {
            w_handle->s_callback (FALSE, w_handle->s_ret_array);
            w_handle->s_ret_array = PNULL;
            w_handle->s_callback  = PNULL;
            SCI_FREE (w_handle);
        }
    }

}


/*****************************************************************************/
//  Description : set param before search
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void InitSearchParam (
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle,
    const wchar                     *path_ptr,          //in, 搜索目录名，如果是NULL，则搜索所有存储器
    uint16                          path_len,
    MMIFMM_FILTER_T                 *filter_ptr    //筛选参数

)
{

    //SCI_ASSERT(NULL != filter_ptr);
    if (PNULL == filter_ptr)
    {
        //SCI_TRACE_LOW:"InitSearchParam param error:PNULL == filter_ptr"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, MMIFMM_INTERFACE_3634_112_2_18_2_20_26_321, (uint8*) "");
        return;
    }

    //设置搜索条件
    if (filter_ptr != &w_handle->s_filter)
    {
        SCI_MEMSET (&w_handle->s_filter, 0, sizeof (w_handle->s_filter));
        MMI_MEMCPY (w_handle->s_filter.filter_str, FMM_FILTER_STRING_MAXLEN,
                    filter_ptr->filter_str, strlen ( (char*) filter_ptr->filter_str), strlen ( (char*) filter_ptr->filter_str));/*lint !e666*/
    }

    if (!ParseFilterStr (filter_ptr, w_handle))
    {
        return;
    }

    //记录下搜索目录
    if (NULL != path_ptr)
    {
        if (path_ptr != w_handle->s_cur_path)
        {
            SCI_MEMSET (w_handle->s_cur_path, 0, sizeof (w_handle->s_cur_path));/*lint !e666*/
            MMI_WSTRNCPY (w_handle->s_cur_path, FMM_SEARCH_FILENAME_MAXLEN,
                          path_ptr, path_len, path_len);
            w_handle->s_cur_path_len = path_len;
        }
    }
    else
    {
        SCI_MEMSET (w_handle->s_cur_path, 0, sizeof (w_handle->s_cur_path));
        w_handle->s_cur_path_len = 0;
    }
}

/*****************************************************************************/
// Description : 从源字符串中解析搜索关键字符串
// Global resource dependence :
// Author: liqing.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseFilterStr (
    MMIFMM_FILTER_T    *filter_ptr,
    MMIFMM_OPEN_LAYER_WIN_DATA_T    *w_handle
)
{

    uint32 i = 0;

    if (PNULL == w_handle)
    {
        return FALSE;
    }

    SCI_MEMSET (w_handle->s_filter_str, 0x00, sizeof (w_handle->s_filter_str));
    MMIAPICOM_StrToWstr (filter_ptr->filter_str, w_handle->s_filter_str);

    for (i = 0; i < sizeof (w_handle->s_filter_str) / sizeof (wchar); i++)
    {
        if (' ' == w_handle->s_filter_str[i])
        {
            w_handle->s_filter_str[i] = '|';
        }
    }

#ifdef DRM_SUPPORT
    //通过后缀字符串获得后缀的类型值，用于查找结果的过滤
    MMIFMM_GetSuffixTypeInfo ( (char*) filter_ptr->filter_str, &w_handle->s_filter_type);
#endif

    return TRUE;
}



/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  参数说明:
//      ret_array： 一定要的，如果为NULL，则不返回文件
//      callback：  一定要的，如果为NULL，则不返回文件
//
//      path_ptr/path_len：打开目录，如果是NULL，则从根目录打开
//      filter_ptr：  搜索关键字符串，例如"*.mp3"
//
//      func_type:  一般填 FUNC_FIND_FILE_AND_ALLFOLER，在选择文件的同时会把所有文件夹显示出来
//      is_multi_sel：是否返回多个文件，对于选择单个文件，填FALSE
//
//      handle_func：   基本用不到，填NULL
//
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayer (
    wchar                   *path_ptr,      //默认打开目录，如果是NULL，则从根目录打开
    uint16                  path_len,
    MMIFMM_FILTER_T         *filter_ptr,    //筛选参数
    MMIFMM_FUNC_TYPE_E      func_type,      //类型
    BOOLEAN                 is_multi_sel,   //是否多选
    PROCESSMSG_FUNC         handle_func,    //自定义处理函数
    FILEARRAY               ret_array,      //输出数组
    MMIFMM_OpenFileCallBack callback,        //回调函数
    BOOLEAN                 is_sel_folder //是否选择文件夹，传给s_is_sel_folder,
    //目前，is_sel_folder参数仅在复制，移动，图片浏览器三个应用为TRUE,其余默认为FALSE,cr164694
)
{
    MMIFMM_OPEN_LAYER_WIN_INFO_T open_layer_info = {0};
    open_layer_info.path_ptr = path_ptr;
    open_layer_info.path_len = path_len;
    open_layer_info.filter_ptr = filter_ptr;
    open_layer_info.func_type = func_type;
    open_layer_info.is_multi_sel = is_multi_sel;
    open_layer_info.handle_func = handle_func;
    open_layer_info.ret_array = ret_array;
    open_layer_info.callback = callback;
    open_layer_info.is_internal_call = FALSE;
    open_layer_info.is_sel_folder = is_sel_folder;
    return MMIFMM_OpenFileWinByLayerEx (&open_layer_info);
}




/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayerEx (MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info)
{
    MMIFMM_OPENWIN_RET_E result = FMM_OPENWIN_SUCCESS;

    if (PNULL == open_layer_info)
    {
        return FMM_OPENWIN_FAIL;
    }

    //s_select_cbk = open_layer_info->select_cbk;
    MMIFMM_OpenFileWinByLayerEx (open_layer_info);
    return result;
}

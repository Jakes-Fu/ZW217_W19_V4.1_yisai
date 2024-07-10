/*****************************************************************************
** File Name:      mmidrm.c                                                  *
** Author:                                                                   *
** Date:           02/23/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to deal with DRM-related operations     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/03/2011     haiwu.chen       Create                                    *
******************************************************************************/

#include "mmi_app_drm_trc.h"
#if defined(DRM_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmi_drmfilemgr.h"
#include "mmidisplay_data.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"
#include "mmidrm.h"
#include "mmipub.h"
#include "mmidrm_id.h"
#include "mmidrm_text.h"
#include "mmidrm_image.h"
#include "in_message.h"
#include "mmi_image.h"
#include "mmidrm_nv.h"
#ifdef EBOOK_SUPPORT
//#include "mmiebook.h"
#include "mmiebook_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#include "mmk_timer.h"
#endif
#include "mmisms_app.h"


#include "mmidrm_export.h"
#include "mmi_event_api.h"
#include "mmi_nv.h"

#include "mmimms_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DEFINITION                        *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create right file saving window
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateRoSaveWin(
    char* rights_buffer_ptr,            //DRM rights download buffer pointer
    uint32 rights_buffer_len,           //DRM rights download buffer len
    BOOLEAN is_wbxml_rights             //DRM rights encode type
);

/*****************************************************************************/
//  Description : handle right file msg callback
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRoFileMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : 释放释放资源
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnWinDataReleased(MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr);
/*****************************************************************************/
//  Description : Install drm rights file request
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InstallDrmRoFileReq(MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : open querying window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryingWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : open alerting window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void OpenAlertingWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : get default path
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void GetDefaultPath(
    uint16** path_pptr,
    uint16 *path_len_ptr,
    MIME_TYPE_E mime_type
);

/*****************************************************************************/
//  Description : to handle the message of waiting for install DRM window    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmInstallWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : handle cancel install drm file   
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 取消安装
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCancelInstallQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : install drm file failed
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void InstallDrmFileFail(
    MMI_WIN_ID_T win_id,
    MMIDRM_INSTALL_DATA_T *install_data_ptr
);

/*****************************************************************************/
//  Description : delete the file which install fail   
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteInstallFailFile(
    wchar *full_path_ptr
);

#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of waiting for right object window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmRoWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
            
/*****************************************************************************/
//  Description : to handle the message of waiting for right object window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmReceivedRoAlertWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
#endif            

/*****************************************************************************/
//  Description : handle DRM_SIG_DH_INSTALL_FILE_CNF in install waiting window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDrmInstallCnf(
    MMIDRM_INSTALL_DATA_T **install_data_pptr,  //[IN]/[OUT],*install_data_pptr may be set to PNULL
    DRM_ERROR_E error_code                      //[IN]
);

/*****************************************************************************/
//  Description : handle the drm file with invalid rights    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 安装成功，但是版权无效，需要分类型对待
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmSaveWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
                    
/*****************************************************************************/
//  Description : free MMIDRM_INSTALL_DATA_T
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void FreeDrmInstallData(MMIDRM_INSTALL_DATA_T **install_data_pptr);

/*****************************************************************************/
//  Description : start to install the DRM file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartInstallDrm(
    MMIDRM_INSTALL_DATA_T *install_data_ptr, 
    MIME_TYPE_E drm_mo_mime_type,
    wchar *new_full_path_ptr
);

/*****************************************************************************/
//  Description : get install name
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetInstalledName(
    MMIDRM_INSTALL_DATA_T *install_data_ptr,
    MIME_TYPE_E drm_mo_mime_type,
    wchar *new_full_path_ptr,
    uint16 *new_full_path_len_ptr,
    BOOLEAN is_check_rename
);

/*****************************************************************************/
//  Description : handle querying rename win
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmRenameQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateRenameWin(
    wchar *src_full_path_ptr
);

/*****************************************************************************/
// Description : handle rename drm win
// Global resource dependence : 
// Author: haiwu.chen
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmRenameWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : 设置DRM文件重命名前的名称
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetInitFileName(
    MMI_CTRL_ID_T ctrl_id,
    wchar *full_path_ptr,
    uint16 path_len
);

/*****************************************************************************/
//  Description : 重命名DRM安装文件名
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenameInstallFile(
    MMI_CTRL_ID_T ctrl_id,
    MMIDRM_RENAME_WIN_PARAM_T *param_rename_win_ptr
);

/*****************************************************************************/
//  Description : get push then play music and so on
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void DrmPushNotify(void);

/*****************************************************************************/
//  Description : handle querying rename win
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCleanSpaceQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Description : to handle the message of cleaning sapce    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCleanSpaceWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);

/*****************************************************************************/
//  Discription: Update current file list
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note: copy this fun from mmibt_wintab.c
/*****************************************************************************/
LOCAL void UpdateCurrentFileList(void);     

/*****************************************************************************/
//  Discription: Update current list item
//  Global resource dependence: none 
//  Author: liyan.zhu 
//  Note: copy this fun from mmidrm.c UpdateCurrentFileList
/*****************************************************************************/
LOCAL void UpdateCurrentListItem(void);

/*****************************************************************************/
//  Description : start to install the DRM ro file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 与安装Mo区分
/*****************************************************************************/
LOCAL BOOLEAN StartInstallDrmRo(
    MMIDRM_INSTALL_DATA_T *install_data_ptr
);

/**--------------------------------------------------------------------------*
 **                         EXTERNAL FUNCTION DECLARE                        *
 **--------------------------------------------------------------------------*/


//rename win
WINDOW_TABLE(MMIDRM_RENAME_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleDrmRenameWinMsg ),    
    WIN_ID(MMIDRM_RENAME_WIN_ID),
    WIN_TITLE(TXT_RENAME_DRM),
#ifdef MMI_PDA_SUPPORT
     CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIDRM_RENAME_FORM_CTRL_ID),
     CHILD_EDIT_TEXT_CTRL(TRUE,0,MMIDRM_RENAME_EDITBOX_CTRL_ID,MMIDRM_RENAME_FORM_CTRL_ID),
     CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMIDRM_RENAME_SOFTKEY_CTRL_ID,MMIDRM_RENAME_FORM_CTRL_ID),
#else    
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(0, MMIDRM_RENAME_EDITBOX_CTRL_ID),
#endif    
    END_WIN
};


/*****************************************************************************/
//  Description : drm init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_Init(void)
{
    MMIDRM_NITZ_TYPE_T nitz_time = {0};
    
    //向浏览器注册DRM服务
    MMIDRM_RegBrowserInfo();
	
    //初始化NV
    MMIAPIDRM_InitNetworkTime();
	
    //读取NV，给全局变量
    MMIAPIDRM_GetNetworkTime(&nitz_time);
    MMIAPIDRM_SetDrmNitzTime(nitz_time);
	
    //向DRM协议注册回调函数,并设置RO文件夹隐藏属性
    MMIDRM_InitAgent();
	
	MMIFW_SetTimeUpdateListerner(MMIAPIDRM_AlterNetworkTime, TRUE);
}

/*****************************************************************************/
//  Description : reg browser info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_RegBrowserInfo(void)
{
#ifdef BROWSER_SUPPORT  
    MMIBROWSER_MIME_DOWNLOAD_PARAM_T drm_callback = {0};

    drm_callback.call_back = MMIDRM_HandleDLDrmCB;    
    drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DM;
    MMIAPIBROWSER_RegMimeFunc(&drm_callback);
    
    drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DCF;
    MMIAPIBROWSER_RegMimeFunc(&drm_callback);
    
    drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DR;
    MMIAPIBROWSER_RegMimeFunc(&drm_callback);
    
    drm_callback.mime_type = MMIBROWSER_MIME_TYPE_DRC;
    MMIAPIBROWSER_RegMimeFunc(&drm_callback);
#endif
}

/*****************************************************************************/
//  Description : install sd type ro file from push message
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_InstallRoFile(
    char* rights_buffer_ptr,            //DRM rights download buffer pointer
    uint32 rights_buffer_len,           //DRM rights download buffer len
    BOOLEAN is_wbxml_rights,            //DRM rights encode type
    BOOLEAN is_trust_rights             //whether trust rights
)
{
    BOOLEAN is_result = FALSE;
//    DRM_ERROR_E result = DRM_DH_FAILED;
//    DRM_INSTALL_FILE_T  *file_handle_ptr = PNULL;
    
    if (PNULL == rights_buffer_ptr
        || 0 == rights_buffer_len)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_InstallRoFile: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_398_112_2_18_2_14_39_47,(uint8*)"");
        return is_result;
    }

    //创建窗口之前先来段music
    DrmPushNotify();

    //暂时都设定是信任版本商发过来的
    is_trust_rights = TRUE;

    if (is_trust_rights)   /*lint !e774*/
    {
        CreateRoSaveWin(rights_buffer_ptr, rights_buffer_len, is_wbxml_rights);
    }    

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIDRM_HandleDrmControlMsg
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 处理drm反馈消息
/*****************************************************************************/
PUBLIC BOOLEAN MMIDRM_HandleDrmControlMsg(MMI_MESSAGE_ID_E msg_id, void *param)
{
    DRM_ERROR_E error = DRM_DH_SUCCESS;
    void *original_param = PNULL;
    
    //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_HandleDrmControlMsg msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_425_112_2_18_2_14_39_48,(uint8*)"d", msg_id);
    
    if (PNULL == param)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_HandleDrmControlMsg: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_429_112_2_18_2_14_39_49,(uint8*)"");
        return FALSE;
    }    
    original_param = (void*)((uint32)param);
    
    switch (msg_id)
    {
        case DRM_SIG_DH_INSTALL_FILE_CNF:
            {
                //response the ro file of push message
                DRM_INSTALL_FILE_CNF_SIG_T *install_cnf_sig_ptr = PNULL;
                install_cnf_sig_ptr = (DRM_INSTALL_FILE_CNF_SIG_T *)original_param;

                //SCI_TRACE_LOW:"[MMIDRM] MMIDRM_HandleDrmControlMsg error_code=%d, install_file_type=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_443_112_2_18_2_14_39_50,(uint8*)"dd", install_cnf_sig_ptr->error_code, install_cnf_sig_ptr->install_file_type);
                    
                error = install_cnf_sig_ptr->error_code;

                MMK_SendMsg(MMIDRM_ACCEPT_RO_FILE_WIN_ID, msg_id, (ADD_DATA)error);
                if (DRM_CONTENTT_TYPE == install_cnf_sig_ptr->install_file_type)
                {
                    MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, msg_id, (DPARAM)error);
                }
                if (DRM_RIGHTS_TYPE == install_cnf_sig_ptr->install_file_type)
                {
                    //通过web get 方式安装
                    if (MMK_IsOpenWin(MMIDRM_INSTALL_WAITING_WIN_ID))
                    {
                        MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, msg_id, (DPARAM)error);
                    }
                    
                    //通过web push的方式安装
                    if (MMK_IsOpenWin(MMIDRM_RO_WAITING_WIN_ID))
                    {
                        MMK_SendMsg(MMIDRM_RO_WAITING_WIN_ID, msg_id, (DPARAM)error);
                    }
                }
                if (MMK_IsOpenWin(MMIDRM_CANCEL_INSTALLFILL_WIN_ID))
                {
                    MMK_CloseWin(MMIDRM_CANCEL_INSTALLFILL_WIN_ID);
                }
            }
            break;

        case DRM_SIG_GET_FILE_MIME_CNF:
            {
                DRM_FILE_CONTEXT_TYPE_CNF_SIG *cnf_ptr = PNULL;
                MIME_TYPE_E mime_type = MIME_TYPE_UNKNOWN; //DRM context type
                
                cnf_ptr = (DRM_FILE_CONTEXT_TYPE_CNF_SIG *)original_param;
                mime_type = cnf_ptr->mime_type;

                MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, msg_id, (DPARAM)mime_type);
            }
            break;

        //这个消息要换成底层新增加的删除完成的回执消息
        case DRM_SIG_DH_DELETE_RIGHTS_FILE_CNF:
            {
                DRM_DELETEL_RIGHTS_FILE_CNF_SIG *cnf_ptr = PNULL;
                cnf_ptr = (DRM_DELETEL_RIGHTS_FILE_CNF_SIG *)original_param;
                error = cnf_ptr->error_code;
                MMK_SendMsg(MMIDRM_CLEANSPACE_WAITING_WIN_ID, msg_id, (ADD_DATA)error);
            }
            break;

        //删除过期文件后的消息回执
        case DRM_SIG_CANCEL_INSTALL_FILE_CNF:
            {
                DRM_CANCEL_INSTALL_FILE_CNF_SIG *cnf_ptr = PNULL;
                cnf_ptr = (DRM_CANCEL_INSTALL_FILE_CNF_SIG *)original_param;
                error = cnf_ptr->error_code;
                
                MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, msg_id, (ADD_DATA)error);
            }
            break;

        default:
            break;
    }

    return TRUE;    
}


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create right file saving window
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateRoSaveWin(
    char* rights_buffer_ptr,            //DRM rights download buffer pointer
    uint32 rights_buffer_len,           //DRM rights download buffer len
    BOOLEAN is_wbxml_rights             //DRM rights encode type
)
{
    MMI_WIN_ID_T win_id = MMIDRM_ACCEPT_RO_FILE_WIN_ID;
    MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr = PNULL;
    MMI_STRING_T text1_str = {0};

    if (PNULL == rights_buffer_ptr
        || 0 == rights_buffer_len)
    {
        //SCI_TRACE_LOW:"[MMIDRM] CreateRoSaveWin: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_533_112_2_18_2_14_39_51,(uint8*)"");
        return;
    }
    
    win_data_ptr = (MMIDRM_RO_SAVE_WIN_DATA_T*)SCI_ALLOCA(sizeof(MMIDRM_RO_SAVE_WIN_DATA_T));
    if (PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr error.!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_540_112_2_18_2_14_39_52,(uint8*)"");
        return;
    }
    SCI_MEMSET(win_data_ptr, 0, sizeof(MMIDRM_RO_SAVE_WIN_DATA_T));

    win_data_ptr->rights_buffer_ptr = (char*)SCI_ALLOCA(rights_buffer_len+1);
    if (PNULL == win_data_ptr->rights_buffer_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] CreateRoSaveWin: SCI_ALLOCA win_data_ptr->rights_buffer_ptr error.!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_548_112_2_18_2_14_39_53,(uint8*)"");
        return;
    }
    SCI_MEMSET(win_data_ptr->rights_buffer_ptr, 0, rights_buffer_len+1);

    SCI_MEMCPY(win_data_ptr->rights_buffer_ptr, rights_buffer_ptr, rights_buffer_len);
    
    *(win_data_ptr->rights_buffer_ptr+rights_buffer_len) = '\0';
    win_data_ptr->rights_buffer_len = rights_buffer_len;
    win_data_ptr->is_wbxml_rights = is_wbxml_rights;
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &text1_str);
    MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(),1,&text1_str,PNULL,PNULL,
            win_id,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,
            HandleRoFileMsg, (uint32)win_data_ptr); 
}

/*****************************************************************************/
//  Description : handle right file msg callback
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRoFileMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{    
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr = PNULL;    
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL == pubwin_info_ptr
        || 0 == pubwin_info_ptr->user_data)
    {
        //SCI_TRACE_LOW:"[MMIDRM] HandleRoFileMsg: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_584_112_2_18_2_14_39_54,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    win_data_ptr = (MMIDRM_RO_SAVE_WIN_DATA_T *)pubwin_info_ptr->user_data;
    if(PNULL == win_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] HandleRoFileMsg: win_data_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_591_112_2_18_2_14_39_55,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            // install 
            //SCI_TRACE_LOW:"[MMIDRM] HandleRoFileMsg: Install request!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_600_112_2_18_2_14_39_56,(uint8*)"");
            InstallDrmRoFileReq(win_data_ptr);
        }
        break;
         
    case DRM_SIG_DH_INSTALL_FILE_CNF:
        {
            DRM_ERROR_E error = DRM_DH_SUCCESS;
            
            error = (uint32)param;

            switch (error)
            {
                //不成功，便成仁
                case DRM_DH_SUCCESS:
                    OpenAlertingWin(TXT_DRM_RECEIVE_RO_FILE);
                    MMK_PostBCMessage(MSG_DRM_GET_PUSH_INFO, PNULL, 0);
                    MMK_CloseWin(win_id);
                    break;

                //空间不够
                case DRM_DH_SYSTEM_DISK_NO_SPACE:
#if 0
                    OpenAlertingWin(TXT_DRM_NOSPACE_DROP_RO_FILE);
                    MMK_CloseWin(win_id);
#endif                    
                    MMIPUB_OpenQueryWinByTextIdEx(
                        MMK_GetFirstAppletHandle(),
                        TXT_DRM_NO_SPACE_TOSAVE_RO_FILE,
                        IMAGE_PUBWIN_QUERY,
                        PNULL,
                        HandleCleanSpaceQueryMsg,
                        PNULL
                        );
                    break;

                //版权的格式无效
                case DRM_DH_INVALID_FORMAT:
					MMIPUB_OpenAlertFailWin(TXT_DRM_FILE_INSTALL_FAIL);
                    MMK_CloseWin(win_id);
                    break;

                //安装成功，但是版权无效，需要分类型对待
                case DRM_DH_DRM_RIGHTS_INVALID:
					MMIPUB_OpenAlertFailWin(TXT_INVALID_RIGHTS_GIVE_UP);
                    MMK_CloseWin(win_id);
                    break;
                    
                default:
                    OpenAlertingWin(TXT_DRM_DROP_RO_FILE);
                    MMK_CloseWin(win_id);
                    break;
            }
        }
        break;

    //空间不够，但是用户也不清除空间
    case MSG_DRM_NOT_CLEAN_SPACE:
        OpenAlertingWin(TXT_DRM_DROP_RO_FILE);
        MMK_CloseWin(win_id);
        break;

    case MSG_DRM_CLEAN_SPACE_SUC:
        InstallDrmRoFileReq(win_data_ptr); 
        break;
    case MSG_CLOSE_WINDOW:
        OnWinDataReleased(win_data_ptr);        
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : release window data
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void OnWinDataReleased(MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr)
{
    if(PNULL != win_data_ptr)
    {
        //free data
        SCI_FREE(win_data_ptr->rights_buffer_ptr);
        win_data_ptr->rights_buffer_len= 0;
    }
}

/*****************************************************************************/
//  Description : Install drm rights file request
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void InstallDrmRoFileReq(MMIDRM_RO_SAVE_WIN_DATA_T *win_data_ptr)
{    
    DRM_INSTALL_FILE_T install_file = {0};

    if (PNULL == win_data_ptr)
    {
        return;
    }
    
    SCI_MEMSET(&install_file, 0, sizeof(DRM_INSTALL_FILE_T));
    
    install_file.rights_buffer_ptr = win_data_ptr->rights_buffer_ptr;
    install_file.rights_buf_len = win_data_ptr->rights_buffer_len;
    install_file.is_wbxml_rights = win_data_ptr->is_wbxml_rights;
    install_file.install_file_type = DRM_RIGHTS_TYPE;
    DRM_InstallFileReq(&install_file);
}

/*****************************************************************************/
//  Description : open querying window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryingWin(MMI_TEXT_ID_T text_id)
{
    MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_DRM_PROMPT_ICON, PNULL, PNULL);
}

/*****************************************************************************/
//  Description : open alerting window
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void OpenAlertingWin(MMI_TEXT_ID_T text_id)
{
    #if 1
    MMIPUB_OpenAlertWinByTextId(PNULL, text_id, TXT_NULL, IMAGE_DRM_PROMPT_ICON, PNULL, PNULL,
        MMIPUB_SOFTKEY_ONE, PNULL);
    #endif
}

/*****************************************************************************/
//  Description : get default path
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void GetDefaultPath(
    uint16** path_pptr,
    uint16 *path_len_ptr,
    MIME_TYPE_E mime_type
)
{
    if ((PNULL == path_pptr) || (PNULL == path_len_ptr))
    {
        return;
    }
    
    switch(mime_type)
    {
    case MIME_TYPE_IMAGE_ANY:
    case MIME_TYPE_IMAGE_GIF:
    case MIME_TYPE_IMAGE_JPEG:
    case MIME_TYPE_IMAGE_TIFF:
    case MIME_TYPE_IMAGE_PNG:
    case MIME_TYPE_IMAGE_VND_WAP_WBMP:
    case MIME_TYPE_IMAGE_SVG:
    case MIME_TYPE_IMAGE_BMP:
    case MIME_TYPE_IMAGE_X_UP_WPNG:
        {
            *path_pptr=(uint16*)MMIMULTIM_DIR_PICTURE;
            *path_len_ptr=MMIMULTIM_DIR_PIC_LEN;
        }
        break;
        
    case MIME_TYPE_AUDIO_ANY:       
    case MIME_TYPE_AUDIO_MID:
    case MIME_TYPE_AUDIO_MPEG:
    case MIME_TYPE_AUDIO_X_WAV:
    case MIME_TYPE_AUDIO_AMR:
    case MIME_TYPE_AUDIO_X_AAC:
    case MIME_TYPE_AUDIO_MIDI:
    case MIME_TYPE_AUDIO_X_MIDI:
    case MIME_TYPE_AUDIO_SP_MIDI:
    case MIME_TYPE_AUDIO_IMELODY:
        {
            *path_pptr=(uint16*)MMIMULTIM_DIR_MUSIC;
            *path_len_ptr=MMIMULTIM_DIR_MOVIE_LEN;
        }
        break;
        
    case MIME_TYPE_VIDEO_MPEG:
    case MIME_TYPE_VIDEO_3GPP:
    case MIME_TYPE_VIDEO_AVI:
    case MIME_TYPE_VIDEO_MP4:
        {
            *path_pptr=(uint16*)MMIMULTIM_DIR_MOVIE;
            *path_len_ptr=MMIMULTIM_DIR_MUSIC_LEN;
        }
        break;
        
#ifdef EBOOK_SUPPORT
    case MIME_TYPE_TEXT_ANY:         // "text/*"
    case MIME_TYPE_TEXT_HTML:        // "text/html"
    case MIME_TYPE_TEXT_PLAIN:       // "text/plain"
    case MIME_TYPE_TEXT_XML:         // "text/xml"
    //case MIME_TYPE_TEXT_X_VCALENDAR: // "text/x-vcalendar
    //case MIME_TYPE_TEXT_X_VCARD:     // "text/x-vcard"//vcard not support
        {
            *path_pptr = (uint16*)MMIMULTIM_DIR_EBOOK;
            *path_len_ptr = MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK);		
                                                
        }
        break;
#endif

    default:
        {
            *path_pptr=(uint16*)MMIMULTIM_OTHER_DEFAULT_DIR;
            *path_len_ptr=MMIAPICOM_Wstrlen(MMIMULTIM_OTHER_DEFAULT_DIR);
        }
        break;

    }
    
}

/*****************************************************************************/
//  Description : start to install the DRM file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN StartInstallDrm(
    MMIDRM_INSTALL_DATA_T *install_data_ptr,
    MIME_TYPE_E drm_mo_mime_type,
    wchar *new_full_path_ptr
)
{
    DRM_INSTALL_FILE_T install_file = {0};
    uint32  new_file_name_len = 0;

    if (PNULL == install_data_ptr
        || PNULL == new_full_path_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] StartInstallDrm ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_833_112_2_18_2_14_40_57,(uint8*)"");
        return FALSE;
    }
    
    install_data_ptr->dl_data.drm_mo_type = drm_mo_mime_type;

    //install the DRM begin
    install_file.header_type_ptr = (char*)uint_to_mime(install_data_ptr->mime_type);
    install_file.install_file_type = DRM_CONTENTT_TYPE;
    install_file.download_file_name_ptr = install_data_ptr->download_file_name_ptr;        
    install_file.new_file_name_ptr = new_full_path_ptr;
    install_file.is_hava_saved = install_data_ptr->is_hava_saved;
    
    if (DRM_DH_SUCCESS != DRM_InstallFileReq(&install_file))
    {
        return FALSE;
    }
    //install the DRM end

    new_file_name_len = MMIAPICOM_Wstrlen(install_file.new_file_name_ptr);
    if (new_file_name_len > 0)
    {
        new_file_name_len++;
        install_data_ptr->new_file_name_ptr = SCI_ALLOCA(new_file_name_len*sizeof(uint16));
        if (install_data_ptr->new_file_name_ptr != PNULL)
        {
            SCI_MEMSET(install_data_ptr->new_file_name_ptr, 0x00, (new_file_name_len*sizeof(uint16)));
            SCI_MEMCPY(install_data_ptr->new_file_name_ptr, install_file.new_file_name_ptr, (new_file_name_len*sizeof(uint16)));
        }
        else
        {
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : get install name
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetInstalledName(
    MMIDRM_INSTALL_DATA_T *install_data_ptr,
    MIME_TYPE_E drm_mo_mime_type,
    wchar *new_full_path_ptr,
    uint16 *new_full_path_len_ptr,
    BOOLEAN is_check_rename
)
{
    BOOLEAN is_result = FALSE;
    wchar   device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    uint16  device_name_len = 0;
    wchar   dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    uint16  dir_name_len = 0;
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16  file_name_len = 0;
    wchar   *new_path_name_ptr = PNULL;
    uint16  new_path_name_len = 0;

    if (PNULL == install_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] StartInstallDrm ERROR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_893_112_2_18_2_14_40_58,(uint8*)"");
        return is_result;
    }

    //get the new full file name by the type
    MMIAPIFMM_SplitFullPath(install_data_ptr->download_file_name_ptr, 
        MMIAPICOM_Wstrlen(install_data_ptr->download_file_name_ptr), 
        device_name, &device_name_len, 
        dir_name, &dir_name_len, 
        file_name, &file_name_len
        );
    GetDefaultPath(&new_path_name_ptr,&new_path_name_len, drm_mo_mime_type);

    MMIAPIFMM_CombineFullPath(device_name, device_name_len, 
        new_path_name_ptr, new_path_name_len, 
        file_name, file_name_len,
        new_full_path_ptr, new_full_path_len_ptr
        );

    if (is_check_rename && MMIAPIFMM_IsFileExist(new_full_path_ptr, *new_full_path_len_ptr))
    {
        wchar *rename_path_ptr = PNULL;
        
        //alloc memory
        rename_path_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
        SCI_MEMSET(rename_path_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));

        //set send param
        MMIAPICOM_Wstrcpy(rename_path_ptr, new_full_path_ptr);

        MMIPUB_OpenQueryWinByTextIdEx(
            MMK_GetFirstAppletHandle(),
            TXT_IS_RENAME_DRM_FILE,
            IMAGE_PUBWIN_QUERY,
            PNULL,
            HandleDrmRenameQueryMsg,
            (uint32)rename_path_ptr
            );
        
        is_result = FALSE;
    }
    else
    {
        //往下处理，不必重命名
        is_result = TRUE;
    }    

    return is_result;
}

/*****************************************************************************/
//  Description : handle DRM_SIG_DH_INSTALL_FILE_CNF in install waiting window
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDrmInstallCnf(
    MMIDRM_INSTALL_DATA_T **install_data_pptr,      //[IN]/[OUT],*install_data_pptr may be set to PNULL
    DRM_ERROR_E error_code                          //[IN]
)
{
    MMIDRM_INSTALL_DATA_T *install_data_ptr = PNULL;
#if defined(JAVA_SUPPORT)                    
    MMI_STRING_T text1_str = {0};
#endif    

    if ((PNULL == install_data_pptr) || (PNULL == *install_data_pptr))
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIDRM] HandleDrmInstallCnf: error_code =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_962_112_2_18_2_14_40_59,(uint8*)"d", error_code);
    install_data_ptr = *install_data_pptr;

    switch (error_code)
    {
        case DRM_DH_SUCCESS:
        {
            switch (install_data_ptr->mime_type)
            {
            case MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE://FL and CD
                MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
                MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, TRUE);

#if defined(JAVA_SUPPORT)                    
                if (MIME_TYPE_APPLICATION_JAVA_JAR == install_data_ptr->dl_data.drm_mo_type)
                {
                    //SCI_TRACE_LOW:"[MMIDRM] HandleDrmInstallCnf BEGIN TO install java"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_977_112_2_18_2_14_40_60,(uint8*)"");
                    MMIAPIJAVA_OTAInstallDrm(install_data_ptr->new_file_name_ptr, install_data_ptr->url_ptr); /*lint !e718 !e746*/
                }
#endif                    
                break;
             
            case MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT://SD
                {
                    
#if defined(JAVA_SUPPORT)                    
                    if ((MIME_TYPE_APPLICATION_JAVA_JAR == install_data_ptr->dl_data.drm_mo_type) 
                        && (install_data_ptr->dl_data.timer_out > 0))
                    {
                    	BOOLEAN is_rights_expired = FALSE;                   
                    
                        //只针对JAR类型文件进行版权等待和后续JAR的自动安装
                        is_rights_expired = MMIDRM_IsRightsExpiredByPath(
                            install_data_ptr->new_file_name_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 
                            0, 0, DRM_PERMISSION_EXECUTE);
                        
                        //SCI_TRACE_LOW:"[MMIDRM] is_rights_expired = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_997_112_2_18_2_14_40_61,(uint8*)"d", is_rights_expired);

                        if (is_rights_expired)           
                        {
                            //begin to waiting the rights
                            MMI_GetLabelTextByLang(TXT_DRM_COPYRIGHTS_WAITNG, &text1_str);
                            MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(),1,&text1_str,PNULL,PNULL,
                                        MMIDRM_RO_WAITING_WIN_ID,IMAGE_NULL,
                                        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,
                                        HandleDrmRoWaitWinMsg, (uint32)install_data_ptr);

                            //install_data_ptr is used in next pub window            
                            *install_data_pptr = PNULL;//pubwin_info_ptr->user_data = PNULL;
                        }
                        else//protect:rights has received, begin to install JAR
                        {
                             //SCI_TRACE_LOW:"[MMIDRM] BEGIN TO install java."
                             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1013_112_2_18_2_14_40_62,(uint8*)"");
                             MMIAPIJAVA_OTAInstallDrm(install_data_ptr->new_file_name_ptr, install_data_ptr->url_ptr);/*lint !e718 !e746*/
                        }
                    }
                    else//save only
#endif
                    {
                          MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
                          MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);                       
                    }
                }
                break;   
                
             //安装版权成功
            case MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML:
            case MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML:
                MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
                MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
                break;               
                
            default:
                MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
                MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
                break;
            }
            
            //下面这个处理完全是为了得到一个文件以后，需要更新FMM的资源列表
            //UpdateCurrentFileList();
			UpdateCurrentListItem();
        }
        break;

    //安装失败删除原文件
    case DRM_DH_INVALID_FORMAT:
        DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
        MMIPUB_OpenAlertFailWin(TXT_DRM_FILE_INSTALL_FAIL);
        MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
        break;

    case DRM_DH_SYSTEM_DISK_NO_SPACE:
        DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
        MMIPUB_OpenAlertFailWin(TXT_DRM_NO_UDISK_SPACE);
        MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
        break;

    //安装成功，但是版权无效，需要分类型对待
    case DRM_DH_DRM_RIGHTS_INVALID:
        {
            wchar *drmfile_name_ptr = PNULL;
            drmfile_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(drmfile_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            MMIAPICOM_Wstrcpy(drmfile_name_ptr, install_data_ptr->new_file_name_ptr);

            DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
            
            MMIPUB_OpenQueryWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                TXT_DRM_COPYRIGHTS_INVLIAD_QUERY_SAVE,
                IMAGE_PUBWIN_QUERY,
                PNULL,
                HandleDrmSaveWaitWinMsg,
                (uint32)drmfile_name_ptr);
            MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
        }
        break;

    case DRM_DH_NOT_SUPPORTTED_TYPE:
        DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
        MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_SUPPORT);
        break;   
    default:
        DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
        MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
        MMIAPIMMSDRM_SetSaved(install_data_ptr->download_file_name_ptr, FALSE);
        break;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : handle the drm file with invalid rights    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 安装成功，但是版权无效，需要分类型对待
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmSaveWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E is_ret = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    wchar *drmfile_path_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
        drmfile_path_ptr = (wchar *)pubwin_info_ptr->user_data;
    }    
    if (PNULL == drmfile_path_ptr)
    {
        return is_ret;
    }

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //安装成功，但是CD FL，版权无效，协议会返回版权无效的返回值
        DeleteInstallFailFile(drmfile_path_ptr);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        SCI_FREE(drmfile_path_ptr);
        is_ret = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        break;
    }

    return (is_ret);

    
}

/*****************************************************************************/
//  Description : free MMIDRM_INSTALL_DATA_T
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void FreeDrmInstallData(MMIDRM_INSTALL_DATA_T **install_data_pptr)
{
    MMIDRM_INSTALL_DATA_T *install_data_ptr = PNULL;

    if ((PNULL != install_data_pptr) && (PNULL != *install_data_pptr))
    {
        install_data_ptr = *install_data_pptr;

        if (install_data_ptr->download_file_name_ptr != PNULL)
        {
            SCI_FREE(install_data_ptr->download_file_name_ptr);
            install_data_ptr->download_file_name_ptr= PNULL;
        }
        
        if (install_data_ptr->new_file_name_ptr != PNULL)
        {
            SCI_FREE(install_data_ptr->new_file_name_ptr);
            install_data_ptr->new_file_name_ptr= PNULL;
        }
        
        if (install_data_ptr->url_ptr != PNULL)
        {
            SCI_FREE(install_data_ptr->url_ptr);
            install_data_ptr->url_ptr = PNULL;
        }

        SCI_FREE(install_data_ptr);
        *install_data_pptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : start to install the original DRM file,then open a waiting win or a fail alert win
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_StartToInstallDrm(
    wchar *file_name_ptr,               //[IN]
    MIME_TYPE_E mime_type,              //[IN]
    char *url_ptr,                      //[IN]
    MMIDRM_DL_DATA_T *dl_data_ptr,       //[IN]
    uint16  is_hava_saved
)
{
    MMI_STRING_T text1_str = {0};
    MMIDRM_INSTALL_DATA_T *install_data_ptr = PNULL;
    uint32 old_file_name_len = 0;
    BOOLEAN result = FALSE;
    DRM_FILE_CONTEXT_TYPE_T content = {0};
    uint32 url_len = 0;
    
    do 
    {
        if (PNULL == file_name_ptr) 
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name_ptr ERROR"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1195_112_2_18_2_14_40_63,(uint8*)"");
            break;
        }
        
        old_file_name_len = MMIAPICOM_Wstrlen(file_name_ptr);
        if (0 == old_file_name_len)
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm file_name ERROR"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1202_112_2_18_2_14_41_64,(uint8*)"");
            break;
        }

        install_data_ptr = SCI_ALLOCA(sizeof(MMIDRM_INSTALL_DATA_T));
        if (PNULL == install_data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm ALLOC ERROR!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1209_112_2_18_2_14_41_65,(uint8*)"");
            break;
        }
        SCI_MEMSET(install_data_ptr, 0x00, sizeof(MMIDRM_INSTALL_DATA_T));

        install_data_ptr->mime_type = mime_type;
        install_data_ptr->is_hava_saved = is_hava_saved;

        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm mime_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1216_112_2_18_2_14_41_66,(uint8*)"d", install_data_ptr->mime_type);
        
        //get the MO mime type form the DRM file
        
        content.download_file_name_ptr = file_name_ptr;
        old_file_name_len++;//end flag
        install_data_ptr->download_file_name_ptr = SCI_ALLOCA(old_file_name_len*sizeof(uint16));
        SCI_MEMSET(install_data_ptr->download_file_name_ptr, 0x00, old_file_name_len*sizeof(uint16));
        if (install_data_ptr->download_file_name_ptr != PNULL)
        {
            SCI_MEMCPY(install_data_ptr->download_file_name_ptr, file_name_ptr, (old_file_name_len*sizeof(uint16)));
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm download_file_name_ptr PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1230_112_2_18_2_14_41_67,(uint8*)"");
            break;
        }

        //get the MO mime type form the DRM file
        if(MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE == install_data_ptr->mime_type
            || MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT == install_data_ptr->mime_type)
        {
            content.header_type_ptr = uint_to_mime(install_data_ptr->mime_type);  /*lint !e605*/
            if (DRM_DH_SUCCESS != DRM_GetContentMimeTypeReq(&content))
            {
                break;
            }
            
            if (dl_data_ptr != PNULL)
            {
                SCI_MEMCPY(&install_data_ptr->dl_data, dl_data_ptr, sizeof(MMIDRM_DL_DATA_T));
            }
            else
            {
                install_data_ptr->dl_data.drm_mo_type = MIME_TYPE_UNKNOWN;
            }
            
            //the url_ptr is used in JAR now
            if ((MIME_TYPE_UNKNOWN == install_data_ptr->dl_data.drm_mo_type) //type is unknown,so it may be JAR
                || (MIME_TYPE_APPLICATION_JAVA_JAR == install_data_ptr->dl_data.drm_mo_type))
            {
                if ((url_ptr != PNULL) && ((url_len = SCI_STRLEN(url_ptr)) > 0))
                {
                    install_data_ptr->url_ptr = SCI_ALLOCA(url_len + 1);
                    if (install_data_ptr->url_ptr != PNULL)
                    {
                        SCI_MEMCPY(install_data_ptr->url_ptr, url_ptr, (url_len + 1));
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIDRM_StartToInstallDrm url_ptr ALLOR Error, url_len =%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1266_112_2_18_2_14_41_68,(uint8*)"d", url_len);
                    }
                }
            }
        }
        
        result = TRUE;
    }while(0);

    if (result)
    {
        MMI_GetLabelTextByLang(TXT_DRM_PROCESSING, &text1_str);
        MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(), 1, &text1_str,PNULL,PNULL,
                MMIDRM_INSTALL_WAITING_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,
                HandleDrmInstallWaitWinMsg, (uint32)install_data_ptr);
    }
    else//error
    {
        FreeDrmInstallData(&install_data_ptr);
		MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
}

/*****************************************************************************/
//  Description : to handle the message of waiting for install DRM window    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmInstallWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MIME_TYPE_E mime_type = MIME_TYPE_UNKNOWN;
    MMIDRM_INSTALL_DATA_T   *install_data_ptr = PNULL;
    MMIDRM_RENAME_WIN_PARAM_T *rename_win_data_ptr = PNULL;    
    wchar new_full_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16 new_full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
        install_data_ptr = (MMIDRM_INSTALL_DATA_T *)pubwin_info_ptr->user_data;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == install_data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1319_112_2_18_2_14_41_69,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        //if ro, just install it
        if(MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML == install_data_ptr->mime_type
            || MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML == install_data_ptr->mime_type)
        {
            StartInstallDrmRo(install_data_ptr);
        }
        
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
        
    case DRM_SIG_GET_FILE_MIME_CNF:
        if (PNULL == install_data_ptr)
        {
            //SCI_TRACE_LOW:"[MMIDRM] HandleDrmInstallWaitWinMsg: install_data_ptr is null!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1336_112_2_18_2_14_41_70,(uint8*)"");
            MMK_CloseWin(win_id);
            break;
        }
        
        //begin to install the DRM,after get the type in DRM file
        mime_type = (uint32)param; //DRM context type
        if ((mime_type != MIME_TYPE_APPLICATION_JAVA_JAR) && (install_data_ptr->url_ptr != PNULL))
        {
            SCI_FREE(install_data_ptr->url_ptr);
            install_data_ptr->url_ptr = PNULL;
        }
        //set mime type
        install_data_ptr->dl_data.drm_mo_type = mime_type;

        if (GetInstalledName(install_data_ptr, mime_type, new_full_name, &new_full_name_len, TRUE))
        {
            //不需要重命名，直接得到安装后的名称
            if (!StartInstallDrm(install_data_ptr, mime_type, new_full_name))
            {
                InstallDrmFileFail(win_id, install_data_ptr);      
            }
        }
        break;

    case MSG_DRM_RENAME_CANCEL_CNF:
        {
            //不需要重命名，直接得到安装后的名称
            wchar *src_path_ptr = (wchar *)param;
            if (PNULL != src_path_ptr
                && PNULL != install_data_ptr)
            {
                //不需要重命名，直接得到安装后的名称
                if (!StartInstallDrm(install_data_ptr, install_data_ptr->dl_data.drm_mo_type, 
                    src_path_ptr))
                {
                    InstallDrmFileFail(win_id, install_data_ptr); 
                }
            }
            else
            {
             	if(PNULL != install_data_ptr)
             	{
               		DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
             	}
                MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
                MMK_CloseWin(win_id);
            }
        }
        break;

    //发生重命名，则在这里取得新的文件名
    case MSG_DRM_RENAME_END_CNF:
        {
            rename_win_data_ptr = (MMIDRM_RENAME_WIN_PARAM_T *)param;
            if (PNULL != rename_win_data_ptr
                && PNULL != install_data_ptr)
            {
                //不需要重命名，直接得到安装后的名称
                if (!StartInstallDrm(install_data_ptr, install_data_ptr->dl_data.drm_mo_type, 
                    rename_win_data_ptr->full_path_ptr))
                {
                    InstallDrmFileFail(win_id, install_data_ptr); 
                }
            }
        }
        break;
        
    case DRM_SIG_DH_INSTALL_FILE_CNF:
        {
            DRM_ERROR_E error_code = (uint32)param; //DRM file install  error code
            
			if (PNULL != pubwin_info_ptr)
			{
				HandleDrmInstallCnf((MMIDRM_INSTALL_DATA_T**)&(pubwin_info_ptr->user_data), error_code);
			}
            MMK_CloseWin(win_id);
        }
        break;
                 
    case MSG_APP_FLIP:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        {
            //打开一个询问窗口，是否确认取消
            wchar *down_name_ptr = PNULL;
            wchar *install_name_ptr = PNULL;
            DRM_CANCEL_INSTALL_FILE *cancel_install_ptr = PNULL;
            MMI_WIN_ID_T win_cancel_id = MMIDRM_CANCEL_INSTALLFILL_WIN_ID;
            if (PNULL != install_data_ptr)
            {
                if (MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML == install_data_ptr->mime_type
                || MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML == install_data_ptr->mime_type)
                {
                    result = MMI_RESULT_FALSE;
                    MMK_CloseWin(win_id);
                }    
            }
            
            GetInstalledName(install_data_ptr, mime_type, 
                new_full_name, &new_full_name_len, FALSE);
            
            //alloc memory
            down_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(down_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
			if (PNULL != install_data_ptr)
			{
				MMIAPICOM_Wstrcpy(down_name_ptr, install_data_ptr->download_file_name_ptr);
			}
            
            install_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            SCI_MEMSET(install_name_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN+1) * sizeof(wchar));
            MMIAPICOM_Wstrcpy(install_name_ptr, new_full_name);

            cancel_install_ptr = SCI_ALLOC_APPZ(sizeof(DRM_CANCEL_INSTALL_FILE));
            SCI_MEMSET(cancel_install_ptr, 0x00, sizeof(DRM_CANCEL_INSTALL_FILE));
            cancel_install_ptr->download_file_name_ptr = down_name_ptr;
            cancel_install_ptr->new_file_name_ptr= install_name_ptr;            

            MMIPUB_OpenQueryWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                TXT_DRM_CANCEL_PROCESSING,
                IMAGE_PUBWIN_QUERY,
                &win_cancel_id,
                HandleCancelInstallQueryMsg,
                (uint32)cancel_install_ptr
                );
        }
        break;
		
    case DRM_SIG_CANCEL_INSTALL_FILE_CNF:
        {
            DRM_ERROR_E error_code = (uint32)param;
            if (DRM_DH_SUCCESS == error_code)
            {
    			MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            }
            else
            {
    			MMIPUB_OpenAlertSuccessWin(TXT_COMM_INSTALL_FAILED);
            }
            MMK_CloseWin(win_id);
        }
        break;
		
    case MSG_CLOSE_WINDOW:
        if ((PNULL != pubwin_info_ptr) && (pubwin_info_ptr->user_data != 0))
        {
            FreeDrmInstallData((MMIDRM_INSTALL_DATA_T**)&(pubwin_info_ptr->user_data));
        }

        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle cancel install drm file   
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 取消安装
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCancelInstallQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E is_ret = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    DRM_CANCEL_INSTALL_FILE *cancel_install_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
        cancel_install_ptr = (DRM_CANCEL_INSTALL_FILE *)pubwin_info_ptr->user_data;
    }    
    if (PNULL == cancel_install_ptr)
    {
        return is_ret;
    }

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        DRM_CancelInstallFileReq(cancel_install_ptr);
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:       
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        SCI_FREE(cancel_install_ptr->download_file_name_ptr);
        SCI_FREE(cancel_install_ptr->new_file_name_ptr);
        SCI_FREE(cancel_install_ptr);        
        is_ret = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id, msg_id,param);
        break;
    }

    return (is_ret);
}


/*****************************************************************************/
//  Description : install drm file failed
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void InstallDrmFileFail(
    MMI_WIN_ID_T win_id,
    MMIDRM_INSTALL_DATA_T *install_data_ptr
)
{
    if (PNULL != install_data_ptr
        && PNULL != install_data_ptr->download_file_name_ptr)
    {    
        //需要扩展,安装失败应提示用户是否删除原文件
		MMIPUB_OpenAlertFailWin(TXT_DRM_FILE_INSTALL_FAIL);
        //安装不成功，则删除原垃圾文件
        DeleteInstallFailFile(install_data_ptr->download_file_name_ptr);
        
        MMK_CloseWin(win_id); 
    }
}

/*****************************************************************************/
//  Description : delete the file which install fail   
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteInstallFailFile(
    wchar *full_path_ptr
)
{
    BOOLEAN is_ret = FALSE;
    uint16 full_path_len = 0;
    
    if (PNULL == full_path_ptr)
    {
        return is_ret;
    }

    full_path_len = MMIAPICOM_Wstrlen(full_path_ptr);
    if (MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
    {
        if (SFS_NO_ERROR == MMIAPIFMM_DeleteFile(full_path_ptr, PNULL))
        {
            is_ret = TRUE;
        }
        else
        {
            is_ret = FALSE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDRM] DeleteInstallFailFile: delete file is not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1608_112_2_18_2_14_41_71,(uint8*)"");
        is_ret = TRUE;
    }

    return is_ret;
}

#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of waiting for right object window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmRoWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMIDRM_INSTALL_DATA_T   *install_data_ptr = PNULL;
    LOCAL uint8  s_waiting_timer_id = 0;
    MMI_WIN_ID_T    query_win_id = MMIDRM_RO_WAITNG_CANCEL_WIN_ID;

    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
        install_data_ptr = (MMIDRM_INSTALL_DATA_T *)pubwin_info_ptr->user_data;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL != install_data_ptr)
        {
            s_waiting_timer_id = MMK_CreateWinTimer(win_id, install_data_ptr->dl_data.timer_out, FALSE);
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        }
        else//error
        {
            //SCI_TRACE_LOW:"[MMIDRM] HandleDrmRoWaitWinMsg MSG_OPEN_WINDOW: install_data_ptr is null!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1648_112_2_18_2_14_41_72,(uint8*)"");
            MMK_CloseWin(win_id);
        }
        break;
        
    case DRM_SIG_DH_INSTALL_FILE_CNF://RO is ok
    {
        if (s_waiting_timer_id != 0)
        {
            MMIPUB_CloseQuerytWin(&query_win_id);
            MMK_StopTimer(s_waiting_timer_id);
            s_waiting_timer_id = 0;
        }

        if (PNULL != install_data_ptr)
        {
            uint32  time_period = 0;
            MMI_WIN_ID_T    alert_win_id = MMIDRM_RECEIVED_RO_ALERT_WIN_ID;

            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(), &time_period, TXT_DRM_RECEIVE_RO_FILE,TXT_NULL,
                    IMAGE_PUBWIN_SUCCESS,&alert_win_id,
                    PNULL,MMIPUB_SOFTKEY_CUSTOMER,
                    HandleDrmReceivedRoAlertWinMsg,(uint32)install_data_ptr);
            MMIPUB_SetWinSoftkey(alert_win_id, TXT_COMMON_OK, TXT_NULL, FALSE);   
			if (PNULL != pubwin_info_ptr)
			{
				pubwin_info_ptr->user_data = PNULL;        
			}
            MMK_CloseWin(win_id);
        }
        else//error
        {
            //SCI_TRACE_LOW:"[MMIDRM] HandleDrmRoWaitWinMsg DRM_SIG_DH_INSTALL_FILE_CNF: install_data_ptr is null!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1680_112_2_18_2_14_41_73,(uint8*)"");
            MMK_CloseWin(win_id);
        }
    }
        break;

    case MSG_TIMER:
        if (s_waiting_timer_id == *((uint8 *)param))
        {
            MMIPUB_CloseQuerytWin(&query_win_id);
			MMIPUB_OpenAlertFailWin(TXT_DRM_COPYRIGHTS_WAITNG_EXPIRATION);
            MMK_StopTimer(s_waiting_timer_id);
            s_waiting_timer_id = 0;
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //如果取消版权下载,将无法自动安装,取消等待版权?   
        MMIPUB_OpenQueryWinByTextId(TXT_DRM_COPYRIGHTS_WAITNG_CANCEL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
        break;

    case MSG_PROMPTWIN_OK:
        //cancle to waiting for the RO file and installing the JAR then
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMK_CloseWin(win_id);
         break;
    
    case MSG_PROMPTWIN_CANCEL:
        //close the query,continue to waiting the RO file
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
         
    case MSG_CLOSE_WINDOW:
        if (s_waiting_timer_id != 0)
        {
            MMK_StopTimer(s_waiting_timer_id);
            s_waiting_timer_id = 0;
        }
        MMIPUB_CloseQuerytWin(&query_win_id);

        if ((PNULL != pubwin_info_ptr) && (pubwin_info_ptr->user_data != 0))
        {
            FreeDrmInstallData((MMIDRM_INSTALL_DATA_T**)&(pubwin_info_ptr->user_data));
        }

        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}
/*****************************************************************************/
//  Description : to handle the message of waiting for right object window    
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDrmReceivedRoAlertWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMIDRM_INSTALL_DATA_T   *install_data_ptr = PNULL;
    
    switch (msg_id)
    {
    case MSG_APP_OK:
    case MSG_APP_WEB:
        pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if ((PNULL != pubwin_info_ptr) && (pubwin_info_ptr->user_data != 0))
        {
            install_data_ptr = (MMIDRM_INSTALL_DATA_T *)pubwin_info_ptr->user_data;

            //SCI_TRACE_LOW:"[MMIDRM] BEGIN TO install java"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1761_112_2_18_2_14_42_74,(uint8*)"");
            MMIAPIJAVA_OTAInstallDrm(install_data_ptr->new_file_name_ptr,  install_data_ptr->url_ptr);/*lint !e718 !e746*/
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIDRM] HandleDrmReceivedRoAlertWinMsg:NULL pubwin_info_ptr = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_1766_112_2_18_2_14_42_75,(uint8*)"d", pubwin_info_ptr);
        }
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

        if ((PNULL != pubwin_info_ptr) && (pubwin_info_ptr->user_data != 0))
        {
            FreeDrmInstallData((MMIDRM_INSTALL_DATA_T**)&(pubwin_info_ptr->user_data));
        }

        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (result);

}
#endif/*JAVA_SUPPORT*/

/*****************************************************************************/
//  Description : handle querying rename win
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmRenameQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E        is_ret = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pub_win_ptr = PNULL;
    wchar               *drmfile_path_ptr = PNULL;

    //get pubwin data
    pub_win_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL == pub_win_ptr)
    {
        return is_ret;
    }
    
    //get file info
    drmfile_path_ptr = (wchar *)pub_win_ptr->user_data;
    if (PNULL == drmfile_path_ptr)
    {
        return is_ret;
    }

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        CreateRenameWin(drmfile_path_ptr);
        MMK_CloseWin(win_id);
        break;
		
    case MSG_APP_RED:
        //back to idle
        MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, MSG_DRM_RENAME_CANCEL_CNF, PNULL);
        MMK_CloseWin(win_id);
        is_ret = MMI_RESULT_FALSE;
        break;

	case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, MSG_DRM_RENAME_CANCEL_CNF, drmfile_path_ptr);        
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        SCI_FREE(drmfile_path_ptr);
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (is_ret);
}

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL void CreateRenameWin(
    wchar *src_full_path_ptr
)
{
    uint16 path_len = 0;
//    wchar *full_path_ptr = PNULL;
    MMIDRM_RENAME_WIN_PARAM_T *param_rename_win_ptr = PNULL;

    path_len = MMIAPICOM_Wstrlen(src_full_path_ptr);

    param_rename_win_ptr = SCI_ALLOC_APPZ(sizeof(MMIDRM_RENAME_WIN_PARAM_T));
    SCI_MEMSET(param_rename_win_ptr, 0x00, sizeof(wchar));
    
    param_rename_win_ptr->full_path_ptr = SCI_ALLOC_APPZ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));
    SCI_MEMSET(param_rename_win_ptr->full_path_ptr, 0x00, (MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar)); 
    MMIAPICOM_Wstrcpy(param_rename_win_ptr->full_path_ptr, src_full_path_ptr);

    param_rename_win_ptr->win_id = MMIDRM_INSTALL_WAITING_WIN_ID;
    param_rename_win_ptr->msg_id = MSG_DRM_RENAME_END_CNF;
        
    MMK_CreateWin((uint32 *)MMIDRM_RENAME_WIN_TAB, (ADD_DATA)param_rename_win_ptr);
}


/*****************************************************************************/
// Description : handle rename drm win
// Global resource dependence : 
// Author: haiwu.chen
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDrmRenameWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMIDRM_RENAME_WIN_PARAM_T *param_rename_win_ptr = PNULL;               
    wchar                   *full_path_ptr = PNULL;
    uint16                  path_len = 0;
    MMI_CTRL_ID_T           ctrl_id = MMIDRM_RENAME_EDITBOX_CTRL_ID;
    
    param_rename_win_ptr = (MMIDRM_RENAME_WIN_PARAM_T*)MMK_GetWinAddDataPtr(win_id);
    if (PNULL == param_rename_win_ptr
        || PNULL == param_rename_win_ptr->full_path_ptr)
    {
        return result;
    }
    full_path_ptr = param_rename_win_ptr->full_path_ptr;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        path_len = MMIAPICOM_Wstrlen(full_path_ptr);
        SetInitFileName(ctrl_id, full_path_ptr, path_len);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef MMI_PDA_SUPPORT        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:

        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                {
                    if (RenameInstallFile(ctrl_id, param_rename_win_ptr))
                    {        
                        MMK_SendMsg(param_rename_win_ptr->win_id, param_rename_win_ptr->msg_id, param_rename_win_ptr);
                        
                        MMK_CloseWin(win_id);
                    }
                }                 
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, MSG_DRM_RENAME_CANCEL_CNF, PNULL);  
                MMK_CloseWin(win_id);                
                break;
            default:
                break;
            }
          }
          break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527                      
#else                  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            if (RenameInstallFile(ctrl_id, param_rename_win_ptr))
            {        
                MMK_SendMsg(param_rename_win_ptr->win_id, param_rename_win_ptr->msg_id, param_rename_win_ptr);
                
                MMK_CloseWin(win_id);
            }
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_SendMsg(MMIDRM_INSTALL_WAITING_WIN_ID, MSG_DRM_RENAME_CANCEL_CNF, PNULL);  
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        SCI_FREE(param_rename_win_ptr->full_path_ptr);
        SCI_FREE(param_rename_win_ptr);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 设置DRM文件重命名前的名称
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void SetInitFileName(
    MMI_CTRL_ID_T ctrl_id,
    wchar *full_path_ptr,
    uint16 full_path_len
)
{
    uint16 path_len = 0;
    uint16 name_len = 0;
    uint16 char_max_num = 0;

    if (PNULL == full_path_ptr
        || 0 == full_path_len)
    {
        return;
    }
    
    //split file name
    MMIAPIFMM_SplitFullPathExt(full_path_ptr, full_path_len,
        PNULL,&path_len,
        PNULL,&name_len,
        PNULL,PNULL);

    char_max_num = MMIFILE_FILE_NAME_MAX_LEN - full_path_len + name_len;

    //set edit box max len
	GUIEDIT_SetMaxLen( ctrl_id,  char_max_num);
    //set text string
	GUIEDIT_SetString( ctrl_id, full_path_ptr + path_len, name_len);
}

/*****************************************************************************/
//  Description : 重命名DRM安装文件名
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenameInstallFile(
    MMI_CTRL_ID_T  ctrl_id,
    MMIDRM_RENAME_WIN_PARAM_T *param_rename_win_ptr
)
{
    BOOLEAN             result = FALSE;
    wchar               *old_full_path_ptr = PNULL;
    uint16              old_path_len = 0;
//    uint16              cur_index = 0;
    uint16              path_len = MMIFILE_FILE_NAME_MAX_LEN;
    uint16              suffix_len = MMIFILE_SUFFIX_MAX_LEN;
    uint16              new_name_len = 0;
    int32               i = 0;
    wchar               *new_name_ptr = PNULL;
    wchar               suffix[MMIFILE_SUFFIX_MAX_LEN+1] = {0};
    MMI_STRING_T        name_str = {0};

    if (PNULL == param_rename_win_ptr
        || PNULL == param_rename_win_ptr->full_path_ptr)
    {
        return result;
    }

    //get edit box text info
    GUIEDIT_GetString( ctrl_id, &name_str);

    if (0 < name_str.wstr_len)
    {
        //judge file name is valid
        if(!MMIAPICOM_IsIncludeInvalidChar(name_str.wstr_ptr,name_str.wstr_len))
        {
            //delete space the name head
            for (i=0; i<name_str.wstr_len; i++)
            {
                if (' ' != name_str.wstr_ptr[i])
                {
                    break;
                }
            }

            if (i == name_str.wstr_len)
            {
                //prompt valid name
				MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);
            }
            else
            {
                old_full_path_ptr = param_rename_win_ptr->full_path_ptr;
                old_path_len = MMIAPICOM_Wstrlen(old_full_path_ptr);
                
                //new name alloc memory
                new_name_ptr = SCI_ALLOC_APPZ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));

                //get new full name, split old full name,get the path and suffix
                MMIAPIFMM_SplitFullPathExt(old_full_path_ptr,old_path_len,
                    new_name_ptr,&path_len,
                    PNULL,PNULL,
                    suffix,&suffix_len);
                new_name_len = path_len;
            
                //copy name
                MMI_WSTRNCPY(new_name_ptr+new_name_len,(MMIFILE_FILE_NAME_MAX_LEN-new_name_len),
                    name_str.wstr_ptr+i,(name_str.wstr_len-i),
                    (name_str.wstr_len-i));
                new_name_len += name_str.wstr_len-i;

                //copy suffix
                MMI_WSTRNCPY(new_name_ptr+new_name_len,(MMIFILE_FILE_NAME_MAX_LEN-new_name_len),
                    suffix,suffix_len,
                    suffix_len);
                new_name_len += suffix_len;

                //new name and old name is same
                if (0 == MMIAPICOM_CompareTwoWstr(new_name_ptr,new_name_len,old_full_path_ptr,old_path_len))
                {
                    //prompt complete
					MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                    result = TRUE;
                }
                else
                {
                    //new name is exist
                    if (MMIAPIFMM_IsFileExist(new_name_ptr,new_name_len))
                    {
                        //prompt name repeat
						MMIPUB_OpenAlertFailWin(TXT_DC_NAMEREPEAT);
                    }
                    else
                    {
                        //发个消息出去
                        SCI_MEMSET(param_rename_win_ptr->full_path_ptr, 0, (MMIFMM_FULL_FILENAME_LEN * sizeof(wchar)) );
                        MMI_WSTRNCPY( param_rename_win_ptr->full_path_ptr, MMIFMM_FULL_FILENAME_LEN, new_name_ptr, new_name_len, new_name_len );
                        result = TRUE;
                    }
                }

                //free new name
                SCI_FREE(new_name_ptr);
            }
        }
        else
        {
            //prompt valid name
			MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get push then play music and so on
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL void DrmPushNotify(void)
{
    MN_DUAL_SYS_E dual_sys = MMIAPISET_GetActiveSim();
        
    //SCI_TRACE_LOW:"[MMIDRM] DrmPushNotify dual_sys =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_2138_112_2_18_2_14_42_76,(uint8*)"d", dual_sys);
    
    MMIAPISMS_PlayNewSMSRing(dual_sys);
    MMIDEFAULT_SetBackLight(TRUE);
}

/*****************************************************************************/
//  Description : handle querying rename win
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCleanSpaceQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_STRING_T        text1_str = {0};
    MMI_RESULT_E        is_ret = MMI_RESULT_TRUE;
//    wchar               *drmfile_path_ptr = PNULL;

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
        //开始请求清理DRM版权空间的
			DRM_DELETE_RIGHTS_FILE_T del_rights_req = {0};
            uint16 sd_ro_path[DRM_RIGHTS_DIR_LEN+2] = {0};
            MMIDRM_GetDrmRoDir(sd_ro_path);
            del_rights_req.delete_type = DRM_DELETE_EXPIRED_RIGHTS_FILE;
            del_rights_req.delete_rights_path_ptr = sd_ro_path;            
            DRM_DeleteRightsHandleReq(&del_rights_req);
        
        //创建等待协议删除的回执消息
            MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &text1_str);
        MMIPUB_OpenWaitWinEx(MMK_GetFirstAppletHandle(), 1,&text1_str,PNULL,PNULL,
                MMIDRM_CLEANSPACE_WAITING_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,
                HandleCleanSpaceWaitWinMsg, PNULL);
        MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_RED:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //不乐意清除空间
        MMK_SendMsg(MMIDRM_ACCEPT_RO_FILE_WIN_ID, MSG_DRM_NOT_CLEAN_SPACE, PNULL);        
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (is_ret);
}

/*****************************************************************************/
//  Description : to handle the message of cleaning sapce    
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCleanSpaceWaitWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL != pubwin_info_ptr)
    {
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    case DRM_SIG_DH_DELETE_RIGHTS_FILE_CNF:
        {
            DRM_ERROR_E error = DRM_DH_SUCCESS;            
            error = (uint32)param;
            
            switch (error)
            {
                //不成功，便成仁
                case DRM_DH_SUCCESS:                    
                    MMK_SendMsg(MMIDRM_ACCEPT_RO_FILE_WIN_ID, MSG_DRM_CLEAN_SPACE_SUC, PNULL);
                    break;
                    
                default:
                    MMK_SendMsg(MMIDRM_ACCEPT_RO_FILE_WIN_ID, MSG_DRM_NOT_CLEAN_SPACE, PNULL);
                    break;
            }
            
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //如果取消版权下载,将无法自动安装,取消等待版权?   
        MMK_SendMsg(MMIDRM_ACCEPT_RO_FILE_WIN_ID, MSG_DRM_NOT_CLEAN_SPACE, PNULL);
        MMK_CloseWin(win_id);
        break;
         
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Discription: Update current file list
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note: copy this fun from mmibt_wintab.c
/*****************************************************************************/
LOCAL void UpdateCurrentFileList(void)
{
    //图片、音乐、视频文件列表选择窗口
    MMIAPIFMM_CloseSelectFileWin();
    MMIAPIFMM_UpdateCurrentFileLIst();
    
    //停止录音
    //MMIAPIRECORD_Exit();
}

/*****************************************************************************/
//  Discription: Update current list item
//  Global resource dependence: none 
//  Author: liyan.zhu 
//  Note: copy this fun from mmidrm.c UpdateCurrentFileList
/*****************************************************************************/
LOCAL void UpdateCurrentListItem(void)
{
    //图片、音乐、视频文件列表选择窗口
    MMIAPIFMM_CloseSelectFileWin();
    MMIAPIFMM_UpdateCurrentLIstItem();
}

#ifdef BROWSER_SUPPORT    
/*****************************************************************************/
//  Description : 处理通过浏览下载到的DRM文件
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDRM_HandleDLDrmCB(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr)
{
    MIME_TYPE_E mime_type = MIME_TYPE_UNKNOWN;

    if (PNULL == param_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIDRM MMIDRM_HandleDLDrmCB mime type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_2296_112_2_18_2_14_43_77,(uint8*)"d", param_ptr->mime_type);
    
    switch (param_ptr->mime_type)
    {
    case MMIBROWSER_MIME_TYPE_DM:
        mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE;
        break;
        
    case MMIBROWSER_MIME_TYPE_DCF:
        mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT;
        break;
        
    case  MMIBROWSER_MIME_TYPE_DR :
        mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML;
        break;
        
    case MMIBROWSER_MIME_TYPE_DRC:
        mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML;
        break;
    default:
        return;
    }    

    MMIAPIDRM_StartToInstallDrm(param_ptr->file_name, 
                mime_type, 
                param_ptr->url_ptr, 
                param_ptr->user_data,
                0);

}
#endif

/*****************************************************************************/
//  Description : start to install the DRM ro file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 与安装Mo区分
/*****************************************************************************/
LOCAL BOOLEAN StartInstallDrmRo(
    MMIDRM_INSTALL_DATA_T *install_data_ptr
)
{
    DRM_INSTALL_FILE_T install_file = {0};

    if (PNULL == install_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] StartInstallDrmRo param error."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDRM_2339_112_2_18_2_14_43_78,(uint8*)"");
        return FALSE;
    }

    //install the DRM begin
    install_file.header_type_ptr = uint_to_mime(install_data_ptr->mime_type);/*lint !e605*/
    install_file.install_file_type = DRM_CONTENTT_TYPE;
    install_file.download_file_name_ptr = install_data_ptr->download_file_name_ptr;  
    install_file.is_hava_saved = install_data_ptr->is_hava_saved;
    
    if (DRM_DH_SUCCESS != DRM_InstallFileReq(&install_file))
    {
        return FALSE;
    }
    //install the DRM end    

    return TRUE;
}

/*****************************************************************************/
//  Description : handle download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIDRM_HandleDrmDLRoQueryMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
)
{
    MMI_RESULT_E        is_ret = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *pub_win_ptr = PNULL;
    wchar               *drmfile_path_ptr = PNULL;

    //get pubwin data
    pub_win_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    if (PNULL == pub_win_ptr)
    {
        return is_ret;
    }
    
    //get file info
    drmfile_path_ptr = (wchar *)pub_win_ptr->user_data;
    if (PNULL == drmfile_path_ptr)
    {
        return is_ret;
    }

    switch (msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //down copyrights
        MMIDRM_DownloadRightsFileByPath(drmfile_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        is_ret = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        //free memory
        SCI_FREE(drmfile_path_ptr);
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;

    default:
        is_ret = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (is_ret);
}

/*****************************************************************************/
//  Description : init nv MMINV_SET_NETWORK_TIME_NUM
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitNetworkTime(void)
{
    MMIDRM_NITZ_TYPE_T nitz_value = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;

    //init is syn net time and the num of local time minus network time
    MMINV_READ(MMINV_DRM_NITZ_TYPE, &nitz_value, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_DRM_NITZ_TYPE, &nitz_value);
    }

    return;
}

/*****************************************************************************/
//  Description : get the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetNetworkTime(MMIDRM_NITZ_TYPE_T *drm_nitz_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //get the num of local time minus network time
    MMINV_READ(MMINV_DRM_NITZ_TYPE, drm_nitz_ptr, return_value);

    return;
}

/*****************************************************************************/
//  Description : set the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetNetworkTime(MMIDRM_NITZ_TYPE_T drm_nitz)
{    
    //write the num of local time minus network time
    MMINV_WRITE(MMINV_DRM_NITZ_TYPE, &drm_nitz);

    return;
}



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_ENABLE)



/*Edit by script, ignore 7 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

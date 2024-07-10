/****************************************************************************
** File Name:      mmimms_export.c                                         *
** Author:         minghu.mao                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        minghu.mao          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmimms_export.h"
#include "mmi_appmsg.h"
#ifdef MMS_SUPPORT_DORADO
#include "mmk_type.h"
//#ifdef MMS_SUPPORT
#include "mmimms_setting.h"
#include "mmimms_edit.h"
#include "mmimms_control_main.h"
#include "mmimms_main.h"
#include "mmimms_push.h"
#include "mmimms_net.h"
#include "mmimms_menutable.h"
//#include "mmimms_id.h"
#include "mmimms_internal.h"
#include "mmimms_edit.h"
#include "mmimms_main.h"
#include "mmimms_file.h"
#include "mmipub.h"
//#include "mmi_id.h"
//#include "mmiota_export.h"
#include "mmimms_text.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmifmm_export.h"
#include "mmidrm_export.h"
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmisms_app.h"
#endif
#include "mmisms_id.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
const wchar g_mmimms_dir_wstr[] = {'M', 'M', 'S', 0 };
MMIFILE_FILE_INFO_T   *g_file_info_ptr = PNULL;
MMIFMM_FILE_TYPE_E   g_file_type = MMIFMM_FILE_TYPE_NORMAL;

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
typedef struct 
{
   MN_DUAL_SYS_E    dual_sys;
   MMI_STRING_T     subject_str;      
   MMI_STRING_T   string_to_str;   
} MMIMMS_WRITE_MSG_INFO_T;
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify MMS
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyMMSFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif
/*****************************************************************************/
//  Description :mms edit insert picture query handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPicInsertQueryWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );


/*****************************************************************************/
//  Description : answer mms ,add privacy check
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleMMSOpenWriteWin(BOOLEAN is_permitted, DPARAM param_ptr);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify MMS
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyMMSFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMIMMS]: HandleSIMPlugNotifyMMSFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_94_112_2_18_2_38_15_128,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:        
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        MMK_CloseMiddleWin(MMIMMS_WIN_ID_START, MMIMMS_MAX_WIN_ID);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMISMS_COM_SELECT_SIM_WIN_ID);
#endif
        break;

    default:
        break;
    }
}
#endif
/*****************************************************************************/
//  Description : init mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIAPIMMS_AppInit(void)
{
    MMS_INIT_PARAM      init_param ={0};
#ifdef SIM_PLUG_IN_SUPPORT    
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif
    //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIMMS_AppInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_119_112_2_18_2_38_15_129,(uint8*)"");
    MMIMMS_RegMenuGroup();
    MMIMMS_RegWinIdNameArr(); 
    MMIMMS_InitSetting();
    MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_MAX);
    MMIMMS_ControlInit(MMIMMS_HandleControlMsg);

    init_param.alloc_cb_ptr = MMIMMS_AllocInBlock;
    init_param.free_cb_ptr = MMIMMS_FreeInBlock;
    MMS_Init(&init_param); //Init mms lib
    MMS_SetSlideTimes(MMIAPIMMS_GetSlideTimes());

    //MMIOTA_InitOTAPUSH();
#ifdef SIM_PLUG_IN_SUPPORT    
    sim_plug_notify_info.module_id = MMI_MODULE_MMS;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyMMSFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif    
}

/*****************************************************************************/
//  Description : open mms setting win
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_OpenMMSSettingWin(void)
{
    MMIMMS_CreateMMSSettingWin();
    return;
}

/*****************************************************************************/
//  Description : init mms list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIAPIMMS_ListInit(void)
{
//  FILE_DEV_E_T dev_type = FS_INVALID;
    //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIMMS_ListInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_155_112_2_18_2_38_15_130,(uint8*)"");

    //MMIMMS_PushInit();
    MMIMMS_FreeMMSList();
    if (MMIMMS_CreateMMSDir())
    {
        MMIMMS_ReadMMSListFile();
//        MMIMMS_InsertMMSListToOrderList();
    }
    else
    {
        //no u disk, no sd card
        //create MMS dir fail
    }
    MMIAPISMS_MMSOrderDone();
    
    //后台收发功能此处需要填充自动收发链表,触发自动下载
    MMIMMS_InitAutoList();
    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
}

/*****************************************************************************/
//  Description : Reset the mms setting to factory setting 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ResetMMSFactorySetting(void)
{
    MMIMMS_ResetMMSFactorySetting();
}

/*****************************************************************************/
//  Description : MMS is sending or receiving 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsSendingOrRecving(void)
{
    return MMIMMS_IsSendingOrRecving();
}


/*****************************************************************************/
//  Description : delete user info/ delete all mms file
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DelUserInfo(void)
{
    MMIMMS_DeleteAllMMSFiles();
    //清除短彩合一中的所有彩信条目,释放list内存
    MMIMMS_FreeMMSList(); 
}

/*****************************************************************************/
//  Description : set mms lock flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetLockFlag(uint32 record_id,
                                  BOOLEAN is_locked)
{
    MMISMS_ORDER_INFO_T *item_ptr = PNULL;

    item_ptr = MMIMMS_GetListItemByID(record_id);
    
    if(item_ptr)
    {
        item_ptr->is_lock = is_locked;
    }
}

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//  Description : set mms lock flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityMMS(uint32 record_id,
                                      BOOLEAN is_to_security)
{
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;

     mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);
     if (PNULL == mms_list_info_ptr)
     {
         return FALSE;
     }
     if (is_to_security)//form box to security box
     {
         if (MMISMS_FOLDER_NORMAL == mms_list_info_ptr->folder_type)
         {
             MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS, record_id);
             mms_list_info_ptr->folder_type = MMISMS_FOLDER_SECURITY;
             MMIAPISMS_InsertOneMsgToOrder(mms_list_info_ptr);
             MMIAPIMMS_SaveListInfo();
             //如果移动彩信通知到安全信箱，关闭新彩信提示窗口
             if (  (MMISMS_MT_NOT_DOWNLOAD == mms_list_info_ptr->mo_mt_type 
                   || MMISMS_MT_NOT_NOTIFY == mms_list_info_ptr->mo_mt_type
                   || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_list_info_ptr->mo_mt_type
                   )
                   && mms_list_info_ptr->record_id == (int32)MMIMMS_GetNewMMSId()
                )
             {
                 if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                 {
                     MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                 }
             }
         }
     }
     else//from security box to box
     {
         if (MMISMS_FOLDER_SECURITY == mms_list_info_ptr->folder_type)
         {
             MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS, record_id);
             mms_list_info_ptr->folder_type = MMISMS_FOLDER_NORMAL;
             MMIAPISMS_InsertOneMsgToOrder(mms_list_info_ptr);
             MMIAPIMMS_SaveListInfo();
         }
     }
     return TRUE;
}
#endif

/*****************************************************************************/
//  Description : save mms list
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SaveListInfo(void)
{
    return MMIMMS_SaveMMSListFile();
}


/*****************************************************************************/
//  Description : mms is full or not
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsMMSFull(void)
{
    if(MMIMMS_IsMaxNum() || !MMIMMS_IsEnoughSpace(MMIMMS_MIN_RESERVE_SPACE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Exit mms module 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ExitMMS(void)
{
}

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : mms is playing ring  
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsAudioInUse(void)
{
    return GetMMSPreviewIsPlayMusic();
}

/*****************************************************************************/
//  Description : mms is playing video  
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsVideoInUse(void)
{
    return GetMMSPreviewIsPlayVideo();
}

#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : edit new mms 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditNewMMS(void)
{
    BOOLEAN result = FALSE;
    MMIMMS_ERROR_E edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr = PNULL;

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif      
    edit_err = MMIMMS_EnterEditCheck();   
    if (MMIMMS_NO_ERROR != edit_err)
    {
        //todo 提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(edit_err);
        result = FALSE;
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMK_SendMsg(MMIMMS_EDIT_MENU_WIN_ID,MSG_APP_RED,PNULL);

            MMK_CloseWin(MMIMMS_EDIT_TO_LIST_WIN_ID);
            MMK_CloseWin(MMIMMS_EDIT_SUBJECT_WIN_ID);
            MMK_CloseWin(MMIMMS_EDIT_TEXT_WIN_ID);
            MMK_CloseWin(MMIMMS_EDIT_SET_SLIDE_TIME_WIN_ID);
        }
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if (editdoc_ptr)
        {
            MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_WRITE_NEW);
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
            #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
            #endif
            MMIMMS_CreateEditWin(); 
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIMMS_EditNewMMS : edit document alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_390_112_2_18_2_38_16_131,(uint8*)"");
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            result = FALSE;
        }    
    } 
     return result;
}
//保存当前条发送内容的信息，供隐私保护回调使用
/*****************************************************************************/
//  Description : ansmer mms ,add privacy check
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleMMSOpenWriteWin(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMIMMS_EDIT_DOCUMENT_T      *editdoc_ptr = PNULL;
    const wchar     re_wstr[] = { 'R', 'E', ':', 0 };       //to be 修改为资源
    uint16          re_wstr_len = MMIAPICOM_Wstrlen( re_wstr );
    MMIMMS_WRITE_MSG_INFO_T *write_info = (MMIMMS_WRITE_MSG_INFO_T*)param_ptr;
    if(PNULL == write_info )
    {
        return; 
    }
    if(is_permitted)
    {
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if (editdoc_ptr)
        {
            if(write_info->string_to_str.wstr_ptr && 0 != write_info->string_to_str.wstr_len)
            {
                MMIMMS_SetMMSTo(&write_info->string_to_str,editdoc_ptr);
                MMIMMS_SetMMSNameListByTo(editdoc_ptr);
            }                
            //设置主题
            if (   MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER == MMIMMS_GetMMSEditEnterReason()
                || MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER == MMIMMS_GetMMSEditEnterReason())
            {
                MMIMMS_SetSubject(&write_info->subject_str,editdoc_ptr);
                MMIMMS_AddStringBeforeSubject(&editdoc_ptr->edit_subject,re_wstr,re_wstr_len);
                MMIMS_SetAnswerSim(write_info->dual_sys);
            }  
            if (MMIMMS_EDIT_FROM_CL_SEND == MMIMMS_GetMMSEditEnterReason())
            {
                MMIMS_SetAnswerSim(write_info->dual_sys);
            }
            
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TEXT);
            #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
            #endif
            MMIMMS_CreateEditWin(); 
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIMMS_AnswerMMS : edit document alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_470_112_2_18_2_38_16_133,(uint8*)"");
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        }
    } 
    
    SCI_FREE(write_info->subject_str.wstr_ptr);
    SCI_FREE(write_info->string_to_str.wstr_ptr);
}

/*****************************************************************************/
//  Description : ansmer mms 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_AnswerMMS(
                                   MN_DUAL_SYS_E        dual_sys,
                                   const MMI_STRING_T   *subject_ptr,
                                   const MMI_STRING_T   *string_to_ptr
                                   )
{
    BOOLEAN                     result = FALSE;
//  MMI_STRING_T                 subject = {0};
    MMIMMS_ERROR_E              edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T      *editdoc_ptr = PNULL;
    const wchar     re_wstr[] = { 'R', 'E', ':', 0 };       //to be 修改为资源
    uint16          re_wstr_len = MMIAPICOM_Wstrlen( re_wstr );

    //SCI_TRACE_LOW:"MMIAPIMMS_AnswerMMS : answer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_415_112_2_18_2_38_16_132,(uint8*)"");
    //if(string_to_ptr && string_to_ptr->wstr_ptr && 0 != string_to_ptr->wstr_len)
    {
#ifdef VT_SUPPORT
        if (MMIAPIVT_IsVtCalling())
        {
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
            return FALSE;
        }
#endif  
        //进入编辑主界面
        edit_err = MMIMMS_EnterEditCheck();
        if (MMIMMS_NO_ERROR != edit_err)
        {
            //提示彩信初始化失败
            MMIMMS_EditMMSAlertWin(edit_err);
            result = FALSE;
        }
        else
        {
            if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return FALSE;
            }

            if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
            {
                MMIMMS_WRITE_MSG_INFO_T mmimms_write_info = {0};                                
                if(subject_ptr != PNULL && PNULL  != subject_ptr->wstr_ptr)
                {
                    mmimms_write_info.subject_str.wstr_ptr = SCI_ALLOCAZ((subject_ptr->wstr_len + 1) * sizeof(wchar));
                    if(PNULL == mmimms_write_info.subject_str.wstr_ptr )
                    {
                        return FALSE;
                    }
                    SCI_MEMCPY((void*)mmimms_write_info.subject_str.wstr_ptr,(void*)subject_ptr->wstr_ptr,(subject_ptr->wstr_len + 1) * sizeof(wchar));
                    mmimms_write_info.subject_str.wstr_len = subject_ptr->wstr_len;
                    
                }

                if(string_to_ptr != PNULL)
                {
                    mmimms_write_info.string_to_str.wstr_ptr = SCI_ALLOCAZ((string_to_ptr->wstr_len + 1) * sizeof(wchar));
                    if(PNULL == mmimms_write_info.string_to_str.wstr_ptr )
                    {
                        if(mmimms_write_info.subject_str.wstr_ptr != PNULL)
                        {
                            SCI_FREE(mmimms_write_info.subject_str.wstr_ptr);
                        }
                        return FALSE;
                    }
                    SCI_MEMCPY((void*)mmimms_write_info.string_to_str.wstr_ptr,(void*)string_to_ptr->wstr_ptr,(string_to_ptr->wstr_len + 1) * sizeof(wchar));                   
					mmimms_write_info.string_to_str.wstr_len = string_to_ptr->wstr_len;
                }
                
                mmimms_write_info.dual_sys = dual_sys;
                 
                MMIAPISET_ValidatePrivacy(MMISET_PROTECT_SMS, HandleMMSOpenWriteWin,(DPARAM)&mmimms_write_info,sizeof(MMIMMS_WRITE_MSG_INFO_T));
            }
            else
            {            
                editdoc_ptr = MMIMMS_CreateEditDocument();
                if (editdoc_ptr)
                {
                    if(string_to_ptr && string_to_ptr->wstr_ptr && 0 != string_to_ptr->wstr_len)
                    {
                        MMIMMS_SetMMSTo(string_to_ptr,editdoc_ptr);
                        MMIMMS_SetMMSNameListByTo(editdoc_ptr);
                    }                
                    //设置主题
                    if (   MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER == MMIMMS_GetMMSEditEnterReason()
                        || MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER == MMIMMS_GetMMSEditEnterReason())
                    {
                        MMIMMS_SetSubject(subject_ptr,editdoc_ptr);
                        MMIMMS_AddStringBeforeSubject(&editdoc_ptr->edit_subject,re_wstr,re_wstr_len);
                        MMIMS_SetAnswerSim(dual_sys);
                    }  
                    if (MMIMMS_EDIT_FROM_CL_SEND == MMIMMS_GetMMSEditEnterReason())
                    {
                        MMIMS_SetAnswerSim(dual_sys);
                    }
                    
                    MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TEXT);
                    #ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIMMS_SetIsMMSMode(TRUE);
                    #endif
                    MMIMMS_CreateEditWin(); 
                    result = TRUE;
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIAPIMMS_AnswerMMS : edit document alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_470_112_2_18_2_38_16_133,(uint8*)"");
                    MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
                    result = FALSE;
                } 
            }
        }
    }
    return result;
}


/*****************************************************************************/
//  Description : brwoser send url by mms 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_BrwSendUrlByMMS(const MMI_STRING_T   *string_url_ptr)
{
    BOOLEAN result = FALSE;
    MMIMMS_ERROR_E              edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    uint16 slide_num       = 0;

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif
    edit_err = MMIMMS_EnterEditCheck();
    if (MMIMMS_NO_ERROR != edit_err)
    {
        //提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(edit_err);
        result = FALSE;
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if (editdoc_ptr)
        {
            MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_WRITE_NEW);
            slide_num = MMIMMS_GetSlideCount(editdoc_ptr);
            if (0 == slide_num)
            {
                edit_err = MMIMMS_AddSlide(editdoc_ptr,1);
                if (MMIMMS_NO_ERROR != edit_err)
                {
                    MMIMMS_EditMMSAlertWin(edit_err);
                    return FALSE;
                }
            }
            edit_err = MMIMMS_AddText(editdoc_ptr, string_url_ptr);
            if (MMIMMS_NO_ERROR != edit_err)
            {
                MMIMMS_EditMMSAlertWin(edit_err);
                result = FALSE;
            }
            else
            {
                MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIMMS_SetIsMMSMode(TRUE);
                #endif
                MMIMMS_CreateEditWin(); 
                result = TRUE;
            }            
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIMMS_BrwSendUrlByMMS : edit document alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_544_112_2_18_2_38_16_134,(uint8*)"");
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            result = FALSE;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : draft box edit mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_DraftEditMMS(uint32 record_id)
{
    BOOLEAN result = FALSE;
//  MMI_STRING_T                 subject = {0};
    MMIMMS_ERROR_E              edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif  
    //进入编辑主界面
    edit_err = MMIMMS_EnterEditCheck();
    if (MMIMMS_NO_ERROR != edit_err)
    {
        //提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(edit_err);
        result = FALSE;
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
        editdoc_ptr = MMIMMS_ReadMMS(record_id);
        MMIMMS_SetNowReadMMSId(record_id);
        if (editdoc_ptr)
        {
            MMIMMS_AttachmentTextEncodeType(editdoc_ptr);
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TEXT);
            #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
            #endif
            MMIMMS_CreateEditWin(); 
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIMMS_DraftEditMMS : edit document alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_599_112_2_18_2_38_16_135,(uint8*)"");
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            result = FALSE;
        } 
    }
    return result;
}
/*****************************************************************************/
//  Description : farward mms 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_FarwardMMS(int32      record_id)
{
    BOOLEAN result = FALSE;
//  MMI_STRING_T                 subject = {0};
    MMIMMS_ERROR_E              edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    const wchar     fw_wstr[] = { 'F', 'W', ':', 0 };       //to be 修改为资源？
    uint16          fw_wstr_len = MMIAPICOM_Wstrlen( fw_wstr );

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif  
    //进入编辑主界面
    edit_err = MMIMMS_EnterEditCheck();
    if (MMIMMS_NO_ERROR != edit_err)
    {
        //提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(edit_err);
        result = FALSE;
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#ifdef DRM_SUPPORT
        if (MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason())
        {
            editdoc_ptr = GetMMSReadEditDoc();
            if (MMIMMS_IsMMSForbidFarward(editdoc_ptr))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);                   
                return FALSE;
            }
        }
#endif    
        if (MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD == MMIMMS_GetMMSEditEnterReason())
        {
            editdoc_ptr = MMIMMS_ReadMMS(record_id);
        }
        else if (MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason()
              || MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD == MMIMMS_GetMMSEditEnterReason())
        {
            editdoc_ptr = GetMMSReadEditDoc();
        }
        //当文字的大小超过200k，认为文字过大，提示不让转发
        if (PNULL != editdoc_ptr && MMIMMS_MMS_LIMIT_SIZE < (uint32)editdoc_ptr->editbody_ptr->totalsize)
        {
            MMSMMI_OpenMMSAlertWin(TXT_FILESIZE_TOOBIG, TXT_NULL, IMAGE_PUBWIN_WARNING);
            if (MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD == MMIMMS_GetMMSEditEnterReason())
            {
                MMIMMS_DestroyEditDocument(editdoc_ptr);
            }
            return FALSE;
        }
        
        if (editdoc_ptr)
        {
            MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr = PNULL;
            MMIMMS_EDIT_NUMBER_LIST_T *free_numberlist_ptr = PNULL;

            //清除收件人
            if(editdoc_ptr->edit_to.wstr_ptr)
            {
                MMIMMS_FREE(editdoc_ptr->edit_to.wstr_ptr);
                editdoc_ptr->edit_to.wstr_len = 0;
            }
            numberlist_ptr = editdoc_ptr->numberlist_header_ptr;
            while(PNULL != numberlist_ptr)
            {
                if(PNULL != numberlist_ptr->number_name_ptr)
                {
                    SCI_FREE(numberlist_ptr->number_name_ptr);
                    numberlist_ptr->number_name_ptr = PNULL;
                }
                if(PNULL != numberlist_ptr->number_ptr)
                {
                    SCI_FREE(numberlist_ptr->number_ptr);
                    numberlist_ptr->number_ptr = PNULL;
                }
                free_numberlist_ptr = numberlist_ptr;
                numberlist_ptr = numberlist_ptr->next;
                SCI_FREE(free_numberlist_ptr);
                free_numberlist_ptr = PNULL;                    
            }
            editdoc_ptr->numberlist_header_ptr = PNULL;
            editdoc_ptr->record_id = 0;
            //设置主题
            MMIMMS_AddStringBeforeSubject(&editdoc_ptr->edit_subject,fw_wstr,fw_wstr_len);
            MMIMMS_HeadSlide(editdoc_ptr);
            MMIMMS_AttachmentTextEncodeType(editdoc_ptr);
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
            #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
            #endif
            MMIMMS_CreateEditWin(); 
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIMMS_FarwardMMS : edit document alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_733_112_2_18_2_38_16_136,(uint8*)"");
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            result = FALSE;
        } 
    }
    return result;
}

/*****************************************************************************/
//  Description : inbox, sendfailbox, sendsuccbox read mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_ReadMMS(int32      record_id)
{
    MMISMS_ORDER_INFO_T *mms_order_info_ptr = PNULL;
//  MMIMMS_ERROR_E                 edit_err = MMIMMS_NO_ERROR;
    MMIMMS_EDIT_DOCUMENT_T *    editdoc_ptr = PNULL;
//  uint32  unsettle_num                    = 0;

    MMIMMS_SetNowReadMMSId(record_id);
    mms_order_info_ptr = MMIMMS_GetListItemByID(record_id);
    if ( PNULL != mms_order_info_ptr && MMISMS_TYPE_MMS == mms_order_info_ptr->msg_type)
    {        
        if (   MMISMS_MT_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type 
            || MMISMS_MT_NOT_NOTIFY == mms_order_info_ptr->mo_mt_type
            || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type)
        {
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
            if ((uint32)record_id == MMIMMS_GetNewMMSId())
            {
                if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                }
            }
            if (MMIAPIMMS_IsProcessMMSById(record_id))
            {
                if (!MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
                {
                    MMSMMI_CreateDownloadWaitingWin(mms_order_info_ptr->dual_sys, record_id);
                }                
            }
            else
            {//彩信通知
                MMIMMS_CreateMMSDetailShowWin(record_id);
            }            
        }
        else 
        {
#ifdef VT_SUPPORT
            if (MMIAPIVT_IsVtCalling())
            {
                MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
                return FALSE;
            }
#endif  
            //彩信
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
            if (MMISMS_MO_SEND_FAIL == mms_order_info_ptr->mo_mt_type && MMIAPIMMS_IsProcessMMSById(record_id))
            {
                if (!MMK_IsOpenWin(MMIMMS_DOWNLOADING_WIN_ID))
                {
                    if(MMIAPISET_GetFlyMode())
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_CC_CLOSING_CONNECTION, TXT_NULL, IMAGE_PUBWIN_WARNING);

                    }
                    else
                    {
                        MMSMMI_CreateSendingWaitingWin(mms_order_info_ptr->dual_sys, record_id);
                    }
                }
                //MMSMMI_OpenMMSAlertWin(TXT_SENDING, TXT_NULL, IMAGE_PUBWIN_WARNING);            
                return TRUE;
            }
            if (MMISMS_MO_SEND_FAIL == mms_order_info_ptr->mo_mt_type && MMIAPIMMS_IsMMSInDownloadQueue(record_id))
            {
                MMSMMI_OpenMMSAlertWin(TXT_SENDING, TXT_NULL, IMAGE_PUBWIN_WARNING);
                return TRUE;
            }
            editdoc_ptr = MMIMMS_ReadMMS(record_id);
            if (PNULL == editdoc_ptr)
            {
                //SCI_TRACE_LOW:"MMIAPIMMS_ReadMMS : edit document alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_814_112_2_18_2_38_16_137,(uint8*)"");
                MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
                return FALSE;
            }            
            if(MMISMS_MT_TO_BE_READ == mms_order_info_ptr->mo_mt_type)
            { 
                if(editdoc_ptr->read_report)
                {
                    //需要提示是否发送阅读报告
                    MMIMMS_SetIsSendRPT(TRUE);
                    MMIMMS_CreateMMSRPTSendWaitingWin(editdoc_ptr);
                }
                else
                {
                    MMIMMS_CreateMMSPreviewWin(editdoc_ptr, MMS_PLAY);
                }
                if ((uint32)record_id == MMIMMS_GetNewMMSId())
                {
                    if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                    }
                }                
                MMIMMS_ChangeRecordType(mms_order_info_ptr->record_id, MMISMS_MT_HAVE_READ);
                MMIMMS_SaveMMSListFile();
            }
            else
            {
                MMIMMS_CreateMMSPreviewWin(editdoc_ptr, MMS_PLAY);
            }
        }
        
    }
    else 
    {
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : out box send mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_OutboxSendMMS(int32      record_id)
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif
    edit_doc_ptr = MMIMMS_ReadMMS(record_id);
    if (PNULL == edit_doc_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIMMS_OutboxSendMMS : edit document read failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_871_112_2_18_2_38_17_138,(uint8*)"");
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        return FALSE;
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT	
    MMIMMS_SetIsMMSMode(TRUE);
#endif	
    MMIMMS_OutboxSendMMS(record_id, edit_doc_ptr);
    MMIMMS_DestroyEditDocument(edit_doc_ptr);
    return TRUE;
}

/*****************************************************************************/
//  Description : set mms memory info to richtext item
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
#ifndef MMI_GUI_STYLE_TYPICAL
PUBLIC void  MMIAPIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUILIST_ITEM_DATA_T *item_data,
                                      uint16 *index
                                      )
{
    MMIMMS_SetMMSMemState(ctrl_id, item_data, index);
    return;
}

#else
PUBLIC void  MMIAPIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUIRICHTEXT_ITEM_T *item_data,
                                      uint16 *index
                                      )
{
    MMIMMS_SetMMSMemState(ctrl_id, item_data, index);
    return;
}
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsBlockConflict(BLOCK_MEM_ID_E id)
{
    //MMS在使用而且BLOCK冲突则返回TRUE
    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : edit mms from content
//  Global resource dependence : none
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_EditMMSFromContent(
                                         const MMI_STRING_T     *subject_ptr,       //主题
                                         const MMI_STRING_T     *content_info_ptr,      //文字内容
                                         MMIFILE_FILE_INFO_T    *file_info_ptr,     //媒体文件
                                         MMIFMM_FILE_TYPE_E     file_type
                                         )
{
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return ;
    }
#endif

    if (PNULL != file_info_ptr && MMIMMS_MAX_CONTENT_SIZE < file_info_ptr->file_size)
    {
        MMSMMI_OpenMMSAlertWin(TXT_FILESIZE_TOOBIG, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return;
        }
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if(editdoc_ptr)
        {
            MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
            if(PNULL != subject_ptr)
            {
                MMIMMS_SetSubject(subject_ptr,editdoc_ptr);
            }
            if(PNULL != content_info_ptr)
            {
                MMIMMS_AddText(editdoc_ptr,content_info_ptr);
            }
            if(PNULL != file_info_ptr)
            {
                MMIMMS_AddFile(editdoc_ptr,file_info_ptr,file_type,FALSE,0);
            }
            
            MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);                      
            #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
            #endif            
            MMIMMS_CreateEditWin(); 
        }
        else
        {
            //提示彩信初始化失败
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        }
    }
}

/*****************************************************************************/
//  Description :mms edit insert picture query handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPicInsertQueryWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E  err_code               = MMI_RESULT_TRUE;
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSPicInsertQueryWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_1003_112_2_18_2_38_17_139,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    //显示彩信下载进度条

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:  
        if (PNULL != g_file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == g_file_type)
        {
            //editdoc_ptr = MMIMMS_CreateEditDocument();
            if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return err_code;
            }
            editdoc_ptr = MMIMMS_CreateEditDocument();
            if(editdoc_ptr)
            {
                MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
                if(MMIMMS_AddFile(editdoc_ptr,g_file_info_ptr,g_file_type,FALSE,0))
                {
                    MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIMMS_SetIsMMSMode(TRUE);
                #endif
                if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
                {
                    MMIAPISET_ValidatePrivacy(
                                         MMISET_PROTECT_SMS,
                                         (MMISET_PRIVACY_CALLBACK_PFUNC)MMIMMS_CreateEditWinExt,
                                         PNULL,
                                         0); 
                }
                else
                {
                    MMIMMS_CreateEditWin(); 
                }
            }
            else
            {
                    MMIMMS_DestroyEditDocument(editdoc_ptr);
                    MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
                    
                }
            }
            else
            {
                //提示彩信初始化失败
                MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            }

            SCI_FREE(g_file_info_ptr);
            g_file_info_ptr = PNULL;
            g_file_type = MMIFMM_FILE_TYPE_NORMAL;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if (PNULL != g_file_info_ptr)
        {
            SCI_FREE(g_file_info_ptr);
            g_file_info_ptr = PNULL;
        }
        g_file_type = MMIFMM_FILE_TYPE_NORMAL;
        break;
    default :
        err_code = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param);;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description : edit mms from file
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_EditMMSFromFile(
                                      MMIFILE_FILE_INFO_T   *file_info_ptr,
                                      MMIFMM_FILE_TYPE_E   file_type
                                      )
{
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    GUIANIM_TYPE_E              pic_type = GUIANIM_TYPE_NONE;
    MMIMMS_WINDOW_ID_E win_id            = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return ;
    }
#endif

    if(PNULL == file_info_ptr || 0 == file_info_ptr->file_size)
    {
        MMSMMI_OpenMMSAlertWin(TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    }
    else if(MMIFMM_FILE_TYPE_EBOOK == MMIMMS_GetFileType(file_info_ptr->file_name, file_info_ptr->file_name_len))
    {
        MMSMMI_OpenMMSAlertWin(TXT_COMMON_UNSUPPORT_FILE_TYPE, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
    }
    else if (MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT < file_info_ptr->file_size)
    {
        pic_type = MMIAPICOM_GetImgaeType(file_info_ptr->file_name, file_info_ptr->file_name_len);
        if (MMIFMM_FILE_TYPE_PICTURE == file_type && GUIANIM_TYPE_IMAGE_JPG == pic_type)
        {
            MMK_CloseWin(win_id);

#ifdef MMIMMS_AUTO_COMPRESS_IMAGE
            editdoc_ptr = MMIMMS_CreateEditDocument();
            if(editdoc_ptr)
            {
                MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
                if(MMIMMS_AddFile(editdoc_ptr,file_info_ptr,file_type,FALSE,0))
                {
                    MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);

                    if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
                    {
                        MMIAPISET_ValidatePrivacy(
                            MMISET_PROTECT_SMS,
                            (MMISET_PRIVACY_CALLBACK_PFUNC)MMIMMS_CreateEditWinExt,
                            PNULL,
                            0); 
                    }
                    else
                    {
                        MMIMMS_CreateEditWin(); 
                    }



                }
                else
                {
                    MMIMMS_DestroyEditDocument(editdoc_ptr);
                    MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
                }
            }
#else
            g_file_type = file_type;
            if (PNULL != g_file_info_ptr)
            {
                SCI_FREE(g_file_info_ptr);
                g_file_info_ptr = PNULL;
            }
            g_file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOC_APP(sizeof(MMIFILE_FILE_INFO_T));
            if (PNULL == g_file_info_ptr)
            {
                g_file_type = MMIFMM_FILE_TYPE_NORMAL;
                return;
            }
            SCI_MEMSET((char *)g_file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
            SCI_MEMCPY((char *)g_file_info_ptr, (char *)file_info_ptr, sizeof(MMIFILE_FILE_INFO_T));
            MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC,IMAGE_PUBWIN_QUERY, &win_id,HandleMMSPicInsertQueryWinMsg);
#endif
        }
        else
        {
            MMSMMI_OpenMMSAlertWin(TXT_FILESIZE_TOOBIG, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        }        
    }
    else
    {
        MMIMMS_CloseEditWin();
#ifdef DRM_SUPPORT
        {
            uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};

            MMI_WSTRNTOSTR(file_name, MMIMMS_FILE_NAME_LEN, file_info_ptr->file_name, MMIMMS_FILE_NAME_LEN, MIN(MMIMMS_FILE_NAME_LEN, file_info_ptr->file_name_len));
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);                   
                return;
            }
        }
#endif
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if(editdoc_ptr)
        {
            MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
            if(MMIMMS_AddFile(editdoc_ptr,file_info_ptr,file_type,FALSE,0))
            {
                MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIMMS_SetIsMMSMode(TRUE);
                #endif
                
                if(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_SMS))
                {
                    MMIAPISET_ValidatePrivacy(
                        MMISET_PROTECT_SMS,
                        (MMISET_PRIVACY_CALLBACK_PFUNC)MMIMMS_CreateEditWinExt,
                        PNULL,
                        0); 
                }
                else
                {
                    MMIMMS_CreateEditWin(); 
                }

            }
            else
            {
                MMIMMS_DestroyEditDocument(editdoc_ptr);
                MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
                
            }
        }
        else
        {
            //提示彩信初始化失败
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        }
    }
}

/*****************************************************************************/
//  Description :to make sure mms not in 1 can insert to edittext
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SetEditTextContent(wchar* str_phiz, uint16 str_len)
{
    return MMIMMS_SetEditTextContent(str_phiz, str_len);
}
#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : start auto download
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_CheckAutoDownload(void)
{
    //if (MMK_IsFocusWin(MAIN_IDLE_WIN_ID) || MMIMMS_IsFocusInNewMsgWin())
    {
        MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_HandleTimer(DPARAM param)
{
    return MMIMMS_HandleTimer(param);
}

#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : delete one mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_DeleteMMS(uint32 record_id,
                             MMI_WIN_ID_T win_id
                             )
{
    MMISMS_ORDER_INFO_T* mms_order_ptr = PNULL;
    
    mms_order_ptr =  MMIMMS_GetListItemByID(record_id);
    if (PNULL == mms_order_ptr)
    {
        return FALSE;
    }
    if (record_id == MMIMMS_GetNowReadMMSId())
    {
        if (   MMISMS_MT_HAVE_READ == mms_order_ptr->mo_mt_type
            || MMISMS_MT_SR_HAVE_READ == mms_order_ptr->mo_mt_type
            || MMISMS_MO_SEND_SUCC == mms_order_ptr->mo_mt_type
            || MMISMS_MO_SEND_FAIL == mms_order_ptr->mo_mt_type)
        {
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
        }
        else if (   MMISMS_MT_NOT_DOWNLOAD == mms_order_ptr->mo_mt_type 
                 || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_order_ptr->mo_mt_type)
        {
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
        }
        else if (MMISMS_MO_DRAFT == mms_order_ptr->mo_mt_type)
        {
            if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
            {
                MMIMMS_CloseEditWin();
            }
        }
    } 
    if (!MMIAPIMMS_IsProcessMMSById(record_id))
    {
        MMIMMS_DeleteMMS(record_id); 
    }
    else
    {
        SCI_TRACE_LOW("MMISMS:MMIAPIMMS_DeleteMMS Failed!!! record_id:%d", record_id);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : Get mms subject by record id
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetSubjectById(uint32 record_id, MMI_STRING_T *subject_str)
{
    MMIMMS_EDIT_DOCUMENT_T *list_doc_ptr = PNULL;
    uint16 subject_len                    = 0;
    uint16 temp_len                       = 0;
    MMISMS_ORDER_INFO_T *mms_order_ptr   = PNULL;

    if (subject_str == PNULL || !MMIMMS_IsRecordIDExist(record_id))
    {
        return FALSE;
    }
    mms_order_ptr =  MMIMMS_GetListItemByID(record_id);
    if (PNULL == mms_order_ptr)
    {
        return FALSE;
    }
    if (   MMISMS_MT_TO_BE_READ == mms_order_ptr->mo_mt_type
        || MMISMS_MT_HAVE_READ == mms_order_ptr->mo_mt_type)
    {
        if (MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER == MMIMMS_GetMMSEditEnterReason())
        {
            list_doc_ptr = MMIMMS_ReadMMS(record_id);
        }
        else if (MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER == MMIMMS_GetMMSEditEnterReason())
        {
            list_doc_ptr = GetMMSReadEditDoc();
        }
        
        if (PNULL != list_doc_ptr && PNULL != list_doc_ptr->edit_subject.wstr_ptr && 0 != list_doc_ptr->edit_subject.wstr_len)
        {
            subject_len = list_doc_ptr->edit_subject.wstr_len;
            temp_len = (subject_len + 1) * sizeof(wchar);
            subject_str->wstr_ptr = (wchar * )MMIMMS_ALLOC(temp_len);
            SCI_MEMSET(subject_str->wstr_ptr, 0, temp_len);
            subject_str->wstr_len = subject_len;
            MMI_WSTRNCPY(subject_str->wstr_ptr, MMIMMS_MAX_SUBJECT_LEN, list_doc_ptr->edit_subject.wstr_ptr, subject_len, subject_len);
        }
        
        if (PNULL != list_doc_ptr && MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER == MMIMMS_GetMMSEditEnterReason())
        {
            MMIMMS_DestroyEditDocument(list_doc_ptr);
        }  
    } 
    return TRUE;
}

/*****************************************************************************/
//  Description :mms  detail sender
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DetailSenderString(MMI_STRING_T *str_dst, uint8 *sender_ptr,  MMI_TEXT_ID_T text_id)
{
    MMSDetailSenderString(str_dst, sender_ptr,text_id);
}

/*****************************************************************************/
//  Description :mms  detail time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DetailTimeString(MMI_STRING_T *str_dst, uint32 time)
{
    MMSDetailTimeString(str_dst, time, FALSE);
}

/*****************************************************************************/
//  Description : set mms enter reason  [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_REASON_TYPE_E edit_type)
{
    MMIMMS_SetMMSEditEnterReason(edit_type);
}

/*****************************************************************************/
//  Description : get mms enter reason   [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_REASON_TYPE_E MMIAPIMMS_GetMMSEditEnterReason(void)
{
    return MMIMMS_GetMMSEditEnterReason();
}

/*****************************************************************************/
//  Description : play mms sound
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_PlayMMSSound(MN_DUAL_SYS_E dual_sys)
{
    MMIMMS_PlayMMSSound(dual_sys);
}

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_HandleNetTimer(uint8 timer_id, uint32 param)
{
    MMIMMS_HandleNetTimer(timer_id, param);
}

/*****************************************************************************/
//  Description : if mms in edit
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsEditMMS(void)
{
    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID)
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    #ifdef MMI_SMS_CHAT_SUPPORT
     || MMIAPISMS_IsChatEditMMS()
    #endif
    #endif
     )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsDownloadingMMS(void)
{
    BOOLEAN result = FALSE;

    result = MMIMMS_IsDownloadingMMS();
    return result;
}

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsSendingMMS(void)
{
    BOOLEAN result = FALSE;

    result = MMIMMS_IsSendingMMS();
    return result;
}

/*****************************************************************************/
//  Description : if record id is downloading 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsProcessMMSById(uint32 record_id)
{
    return MMIMMS_IsInProcess(record_id);
}

/*****************************************************************************/
//  Description : if record id is in download queue and wait for downloading
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSInDownloadQueue(uint32 record_id)
{
    return MMIMMS_IsInQueue(record_id);
}

/*****************************************************************************/
//  Description : 如果没有彩信通知了就关闭新彩信提示窗口
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CloseNewMMSPromptWin(void)
{
    uint32 unsettle_num = 0;
    
    unsettle_num = MMIMMS_GetUnsettleNum();
	if(0 == unsettle_num)
	{
		MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
	}
}

/*****************************************************************************/
//  Description : 重新启动自动下载链表下载彩信，在飞行模式关闭后调用
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_RestartAutodownMMS(void)
{
    //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIMMS_RestartAutodownMMS!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_1429_112_2_18_2_38_18_140,(uint8*)"");
    MMIMMS_InitAutoList();
    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_MMS_ACTIVE_RECV_SEND,PNULL,0);
    MMIMMS_SetIsStopDownloadForFlyMode(FALSE);
}

/*****************************************************************************/
//  Description : 获得用户设定的单页彩信播放时间
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetSlideTimes(void)
{
    return MMIMMS_DEFAULT_SLIDE_TIMES;
}

/*****************************************************************************/
//  Description : virtual win active recieve and send mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ActiveRecvSendModule(void)
{
    MMIMMS_ActiveRecvSendModule();
}

/*****************************************************************************/
//  Description : 下载链表是否还有彩信在等待下载
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSWaitingDownload(void)
{
    return !MMIMMS_IsSendRecvListEmpty();
}

/*****************************************************************************/
//  Description : 正在下载彩信的folder type
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_FOLDER_TYPE_E MMIAPIMMS_GetDownloadingMMSFolderType(void)
{
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
    uint32 record_id = MMIMMS_GetNowDownloadMMSId();
    MMISMS_ORDER_INFO_T *mms_order_ptr = PNULL;

    if (record_id != 0)
    {
        mms_order_ptr = MMIMMS_GetListItemByID(record_id);
        if (PNULL != mms_order_ptr)
        {
            folder_type = mms_order_ptr->folder_type;
        }        
    }
    return folder_type;
}

/*****************************************************************************/
//  Description :清空自动下载链表下载彩信，在飞行模式开始后调用，包括停止正在下载的彩信
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_StopAutodownMMS(void)
{
    MMIMMS_ClearRecvSendModule();
    MMIMMS_SetIsStopDownloadForFlyMode(TRUE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ClearRecvSendModule(void)
{
    MMIMMS_ClearRecvSendModule();
}

/*****************************************************************************/
//  Description : 获取彩信总条数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetMMSTotalNumber(void)
{
    return MMIMMS_GetTotalNum();
}

/*****************************************************************************/
//  Description : 获取彩信总大小
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetTotalSizeAndAllSize(uint32 *total_size_ptr, uint32 *all_space_ptr)
{
    return MMIMMS_GetTotalSizeAndAllSize(total_size_ptr, all_space_ptr);
}

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_Setting_SetNetSettingIndex(
                                                 MN_DUAL_SYS_E dual_sys,
                                                 uint8         index
                                                 )
{
    MMIMMS_Setting_SetNetSettingIndex(dual_sys, index);
}

/*****************************************************************************/
//  Description : is pdp permit using,G ner and in phone can not use pdp
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys)
{
    return MMIMMS_IsPdpPermitUsing(dual_sys);
}

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : api for sms module
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_INFO_T *MMIAPIMMS_GetListItemByID(int32 record_id)
{
    return MMIMMS_GetListItemByID(record_id);
}
#endif

/*****************************************************************************/
//  Description : is in mms moudle window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsInMMSWin(void)
{
    MMI_WIN_ID_T win_id = 0;
    BOOLEAN is_in_mms_win = FALSE;

    for (win_id = MMIMMS_WIN_ID_START; win_id < MMIMMS_MAX_WIN_ID; win_id++)
    {
        if (MMK_IsOpenWin(win_id) && (win_id != MMIMMS_NEWMSG_WIN_ID && win_id != MMIMMS_ARRIVE_RPT_WIN_ID))
        {
            is_in_mms_win = TRUE;
            break;
        }
    }

    return is_in_mms_win;
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : create mms setting child win
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CreateMMSSettingsChildWin(MMI_WIN_ID_T	parent_win_id)
{
    MMIMMS_CreateMMSSettingsChildWin(parent_win_id);
}
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT

/*****************************************************************************/
//  Description : create mms win
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CreateMMSEditWin(void)
{
    BOOLEAN ret = FALSE;
    ret = MMIMMS_InitData();
    if(ret)
    {
        MMIMMS_CreateEditWin();
    }
    else
    {
        //SCI_TRACE_LOW:"MMS edit init failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_1591_112_2_18_2_38_18_141,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : edit add mms subject
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSubject(
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr)
{
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    BOOLEAN ret = TRUE;
    MMIMMS_ERROR_E add_slide_err = MMIMMS_ERROR_MAX;
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        return FALSE;
    }

    editdoc_ptr = MMIMMS_CreateEditDocument();
    if(editdoc_ptr)
    {
        MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
       
        MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
        #ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMIMMS_SetIsMMSMode(TRUE);
        #endif
        MMIMMS_CreateEditWin(); 
        MMIMMS_AddText(editdoc_ptr,mms_content_string);

        if(string_to_ptr && string_to_ptr->wstr_ptr && 0 != string_to_ptr->wstr_len)
        {
            MMIMMS_SetMMSTo(string_to_ptr,editdoc_ptr);
            MMIMMS_SetMMSNameListByTo(editdoc_ptr);
        }  
    }
    else
    {            
        //提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);        
    }
    
    return ret;	
}

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetIsMMSMode( BOOLEAN is_mms_mode)
{
    MMIMMS_SetIsMMSMode(is_mms_mode);
}

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetIsMMSMode( void)
{
    return MMIMMS_GetIsMMSMode();
}

/*****************************************************************************/
//  Description :关闭编辑窗口
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CloseEditWin( void)
{
    MMIMMS_CloseEditWin();
}
/*****************************************************************************/
//  Description : edit mms add mms slide
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSlide(
                                      uint32 slide_direct,                                      
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr
                                      )
{
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    BOOLEAN ret = FALSE;
    MMIMMS_ERROR_E add_slide_err = MMIMMS_ERROR_MAX;
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif

    if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
    {
        MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        return FALSE;
    }

    editdoc_ptr = MMIMMS_CreateEditDocument();
    if(editdoc_ptr)
    {
        MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
       
        MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
        #ifdef MMIMMS_SMS_IN_1_SUPPORT
        MMIMMS_SetIsMMSMode(TRUE);
        #endif
        MMIMMS_CreateEditWin(); 
         
        MMIMMS_AddText(editdoc_ptr,mms_content_string);

        if(string_to_ptr && string_to_ptr->wstr_ptr && 0 != string_to_ptr->wstr_len)
        {
            MMIMMS_SetMMSTo(string_to_ptr,editdoc_ptr);
            MMIMMS_SetMMSNameListByTo(editdoc_ptr);
        }  

        add_slide_err = MMIMMS_AddSlide(editdoc_ptr,slide_direct);  
        if (MMIMMS_NO_ERROR != add_slide_err)
        {
            MMIMMS_EditMMSAlertWin(add_slide_err);            
        }                     
        else
        {
            ret = TRUE;
        }
    }
    else
    {
        //提示彩信初始化失败
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
    }

    return ret;
}
/*****************************************************************************/
//  Description : edit mms add mms attach
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSAttach(
                                      MMIFILE_FILE_INFO_T   *file_info_ptr,
                                      MMIFMM_FILE_TYPE_E   file_type,
                                      uint8   *file_data_ptr,
                                      uint32 file_data_len,
                                      MMI_STRING_T *vacard_str,    
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr,
                                      BOOLEAN is_multi_file  //多媒体文件,music，video，pic
                                      )
{
    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
    BOOLEAN ret = FALSE;
    
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_IN_VT_CALL);
        return FALSE;
    }
#endif

    if(
#if defined MMI_VCARD_SUPPORT
        file_type != MMIFMM_FILE_TYPE_VCARD && 
#endif
        #ifdef MMI_VCALENDAR_SUPPORT
        file_type != MMIFMM_FILE_TYPE_VCALENDAR && 
        #endif 
        (PNULL == file_info_ptr || 0 == file_info_ptr->file_size)
        )
    {
        MMSMMI_OpenMMSAlertWin(TXT_EMPTY_FILE, TXT_NULL, IMAGE_PUBWIN_WARNING);        
    }
    else
    {
        if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#ifdef DRM_SUPPORT
        if(PNULL != file_info_ptr && 0 != file_info_ptr->file_size)
        {
            uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};

            MMI_WSTRNTOSTR(file_name, MMIMMS_FILE_NAME_LEN, file_info_ptr->file_name, MMIMMS_FILE_NAME_LEN, MIN(MMIMMS_FILE_NAME_LEN, file_info_ptr->file_name_len));
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name))
            {
                MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);                   
                return FALSE;
            }
        }
#endif
        editdoc_ptr = MMIMMS_CreateEditDocument();
        if(editdoc_ptr)
        {
            MMS_CreateNewSlide(editdoc_ptr->editbody_ptr);
            if(is_multi_file)
            {
                if(MMIMMS_AddFile(editdoc_ptr,file_info_ptr,file_type,FALSE,0))
                {
                    ret = TRUE;
                }
            }
            else
            {
                if(MMIMMS_AddAndReplaceAttachment( editdoc_ptr, file_info_ptr, PNULL, file_data_ptr, file_data_len, file_type, vacard_str))
                {
                    ret = TRUE;
                }
            }
            
            if(ret)
            {
                MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TO);
                #ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIMMS_SetIsMMSMode(TRUE);
                #endif
                MMIMMS_CreateEditWin(); 

                MMIMMS_AddText(editdoc_ptr,mms_content_string);

                if(string_to_ptr && string_to_ptr->wstr_ptr && 0 != string_to_ptr->wstr_len)
                {
                    MMIMMS_SetMMSTo(string_to_ptr,editdoc_ptr);
                    MMIMMS_SetMMSNameListByTo(editdoc_ptr);
                }   
            }
            else
            {
                MMIMMS_DestroyEditDocument(editdoc_ptr);
                MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
            }            
        }
        else
        {
            //提示彩信初始化失败
            MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_FAIL);
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : is emali addr exist
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetIsEmailAddrExist(BOOLEAN is_have_email_addr)
{
    MMIMMS_SetMMSEditIsEmailAddrExist(is_have_email_addr);
}

#endif
/*****************************************************************************/
//  Description :change mms file size to size string
//  Global resource dependence : 
//  Author:jixin.xu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetMMSFileSizeStr(
                                uint32 total_size, //in
                                wchar *display_wstr_size //out
                                )
{
    char display_str_size[MMIMMS_SIZE_STRING_MAX_LEN+1] ={0};
    uint8 display_size_len = 0;

    if (0 != total_size)
    {
        //SCI_TRACE_LOW:"FNH total_size = %ld"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_EXPORT_1807_112_2_18_2_38_18_142,(uint8*)"d",total_size);
        _snprintf(display_str_size, MMIMMS_SIZE_STRING_MAX_LEN+1, "%.1fK", (float)total_size/ONE_K_LEN); 
        if (((float)total_size/ONE_K_LEN) < MMIMMS_SIZE_FLOAT_A && ((float)total_size/ONE_K_LEN) > MMIMMS_SIZE_FLOAT_B)//sunxia 07.01.05 for detailSize is 0
        {
            _snprintf(display_str_size, MMIMMS_SIZE_STRING_MAX_LEN+1, "%.1fk", 0.1);
        }
    }
    display_size_len = strlen(display_str_size);
    MMI_STRNTOWSTR( display_wstr_size, MMIMMS_SIZE_STRING_MAX_LEN+1, (uint8 *)display_str_size, MMIMMS_SLIDE_TIME_STR_LEN, display_size_len);

    return display_size_len;
}

/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    return MMIMMS_Setting_GetNetSettingIndex(dual_sys);
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetMMSLimitSize(uint32 mms_limit)
{
    MMIMMS_SetMMSLimitSize(mms_limit);
}
#endif

/*****************************************************************************/
//  Description : Get MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetMMSLimitSize(void)
{
    return MMIMMS_GetMMSLimitSize();
}

/*****************************************************************************/
//  Description : 检测是否可以进入彩信编辑模块
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIAPIMMS_EnterEditCheck(void)
{
    return MMIMMS_EnterEditCheck();
}

/*****************************************************************************/
//  Description : alert window when edit new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditMMSAlertWin(MMIMMS_ERROR_E mms_error_e)
{
    return MMIMMS_EditMMSAlertWin(mms_error_e);
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_Interupt(void)
{
    return MMIMMS_Interupt();
}

/*****************************************************************************/
//  Description : for the case if close ps or switch on flight mode while sending win open
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DestroyOperateWaitingWin(void)
{
    MMSMMI_DestroyOperateWaitingWin();
}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set if the content saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMSDRM_SetSaved(wchar* w_saved_fname, BOOLEAN isSaved)
{
    MMIAPIDRM_SetSaved(w_saved_fname, isSaved);
}
#endif

#else   //MMS_SUPPORT_DORADO mms dummy
const wchar g_mmimms_dir_wstr[] = {'M', 'M', 'S', 0 };

PUBLIC void	MMIAPIMMS_AppInit(void){}
PUBLIC void MMIAPIMMS_OpenMMSSettingWin(void){}
PUBLIC void	MMIAPIMMS_ListInit(void)
{
    MMIAPISMS_MMSOrderDone();
}
PUBLIC void MMIAPIMMS_ResetMMSFactorySetting(void){}

PUBLIC BOOLEAN MMIAPIMMS_IsSendingOrRecving(void){ return FALSE;}
PUBLIC void MMIAPIMMS_DelUserInfo(void){}
PUBLIC void MMIAPIMMS_SetLockFlag(uint32 record_id,BOOLEAN is_locked){}
PUBLIC BOOLEAN MMIAPIMMS_SaveListInfo(void){return TRUE;}
PUBLIC BOOLEAN MMIAPIMMS_IsMMSFull(void){return FALSE;}
PUBLIC void MMIAPIMMS_ExitMMS(void){}
#if 0 /* Reduce Code size */
PUBLIC BOOLEAN MMIAPIMMS_IsAudioInUse(void){ return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_IsVideoInUse(void){ return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_HandleTimer(DPARAM param){return FALSE;}
#endif /* Reduce Code size */
PUBLIC BOOLEAN MMIAPIMMS_EditNewMMS(void){ return FALSE;}

PUBLIC BOOLEAN MMIAPIMMS_AnswerMMS(
                                   MN_DUAL_SYS_E        dual_sys,
                                   const MMI_STRING_T   *subject_ptr,
                                   const MMI_STRING_T   *string_to_ptr
                                   ){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_BrwSendUrlByMMS(const MMI_STRING_T   *string_url_ptr){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_DraftEditMMS(uint32 record_id){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_FarwardMMS(int32 record_id){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_ReadMMS(int32 record_id){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_OutboxSendMMS(int32 record_id){return FALSE;}
#ifndef MMI_GUI_STYLE_TYPICAL
PUBLIC void  MMIAPIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUILIST_ITEM_DATA_T *item_data,
                                      uint16 *index
                                      ){}
#else
PUBLIC void  MMIAPIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUIRICHTEXT_ITEM_T *item_data,
                                      uint16 *index
                                      ){}
#endif
PUBLIC BOOLEAN MMIAPIMMS_IsBlockConflict(BLOCK_MEM_ID_E id){return FALSE;}
PUBLIC void MMIAPIMMS_EditMMSFromFile(MMIFILE_FILE_INFO_T *file_info_ptr,MMIFMM_FILE_TYPE_E file_type){}
PUBLIC BOOLEAN MMIAPIMMS_SetEditTextContent(wchar* str_phiz, uint16 str_len);
#if 0 /* Reduce Code size */
PUBLIC BOOLEAN MMIAPIMMS_CheckAutoDownload(void){return FALSE;}
PUBLIC MMIMMS_EDIT_REASON_TYPE_E MMIAPIMMS_GetMMSEditEnterReason(void){return 0;}  //[minghumao记得整理]
#endif /* Reduce Code size */
PUBLIC BOOLEAN MMIAPIMMS_DeleteMMS(uint32 record_id,MMI_WIN_ID_T win_id){return FALSE;}
PUBLIC BOOLEAN MMIAPIMMS_GetSubjectById(uint32 record_id, MMI_STRING_T *subject_str){return FALSE;}
PUBLIC void MMIAPIMMS_DetailSenderString(MMI_STRING_T *str_dst, uint8 *sender_ptr,  MMI_TEXT_ID_T text_id){}
PUBLIC void MMIAPIMMS_DetailTimeString(MMI_STRING_T *str_dst, uint32 time){}

PUBLIC void MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_REASON_TYPE_E edit_type){} //[minghumao记得整理]
PUBLIC void MMIAPIMMS_PlayMMSSound(MN_DUAL_SYS_E dual_sys){}
PUBLIC void MMIAPIMMS_HandleNetTimer(uint8 timer_id, uint32 param){}
PUBLIC void MMIAPIMMS_CloseNewMMSPromptWin(void){}
PUBLIC BOOLEAN MMIAPIMMS_IsEditMMS(void){return FALSE;}
PUBLIC BOOLEAN  MMIAPIMMS_IsDownloadingMMS(void){return FALSE;}
PUBLIC BOOLEAN  MMIAPIMMS_IsSendingMMS(void){return FALSE;}
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSWaitingDownload(void){return FALSE;}
PUBLIC BOOLEAN  MMIAPIMMS_IsProcessMMSById(uint32 record_id){return FALSE;}
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSInDownloadQueue(uint32 record_id) {return FALSE;}
PUBLIC uint32 MMIAPIMMS_GetSlideTimes(void){return 0;}
PUBLIC void MMIAPIMMS_RestartAutodownMMS(void){}
PUBLIC void MMIAPIMMS_StopAutodownMMS(void){}
PUBLIC MMISMS_FOLDER_TYPE_E MMIAPIMMS_GetDownloadingMMSFolderType(void){return MMISMS_FOLDER_NORMAL;}
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityMMS(uint32 record_id,
                                      BOOLEAN is_to_security){return FALSE;}
#endif
PUBLIC void MMIAPIMMS_ActiveRecvSendModule(void){}
PUBLIC void MMIAPIMMS_Setting_SetNetSettingIndex(
                                               MN_DUAL_SYS_E dual_sys,
                                               uint8         index
                                               )
{}
PUBLIC BOOLEAN MMIAPIMMS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys){return FALSE;}
PUBLIC uint32 MMIAPIMMS_GetMMSTotalNumber(void)
{return 0;}
PUBLIC BOOLEAN MMIAPIMMS_GetTotalSizeAndAllSize(uint32 *total_size_ptr, uint32 *all_space_ptr)
{return FALSE;}
PUBLIC void MMIAPIMMS_EditMMSFromContent(
                                         const MMI_STRING_T     *subject_ptr,       //主题
                                         const MMI_STRING_T     *content_info_ptr,      //文字内容
                                         MMIFILE_FILE_INFO_T    *file_info_ptr,     //媒体文件
                                         MMIFMM_FILE_TYPE_E     file_type
                                         )
{
}
PUBLIC BOOLEAN MMIAPIMMS_IsInMMSWin(void){return FALSE;}
BOOLEAN MMIAPIMMS_InsertMessage(wchar *wstr_ptr,uint16 wstr_len){return FALSE;}

#ifndef MMI_GUI_STYLE_TYPICAL
PUBLIC void MMIAPIMMS_CreateMMSSettingsChildWin(MMI_WIN_ID_T	parent_win_id)
{
}
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT

/*****************************************************************************/
//  Description : edit add mms subject
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSubject( 
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetIsMMSMode( BOOLEAN is_mms_mode)
{
    
}

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetIsMMSMode( void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :关闭编辑窗口
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CloseEditWin( void)
{

}


/*****************************************************************************/
//  Description : edit mms add mms attach
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSAttach(
                                      MMIFILE_FILE_INFO_T   *file_info_ptr,
                                      MMIFMM_FILE_TYPE_E   file_type,
                                      uint8   *file_data_ptr,
                                      uint32 file_data_len,
                                      MMI_STRING_T *vacard_str,    
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr,
                                      BOOLEAN is_multi_file  //多媒体文件,music，video，pic
                                      )
{
    return FALSE;
}

PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSlide(
                                      uint32 slide_direct,                                      
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr
                                      )
{
    return FALSE;
}


PUBLIC void MMIAPIMMS_SetIsEmailAddrExist(BOOLEAN is_have_email_addr){}

#endif
/*****************************************************************************/
//  Description :change mms file size to size string
//  Global resource dependence : 
//  Author:jixin.xu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetMMSFileSizeStr(
                                uint32 total_size, //in
                                wchar *display_wstr_size //out
                                )
{return 0;}

/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    return 0;
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetMMSLimitSize(uint32 mms_limit)
{
    
}
#endif

/*****************************************************************************/
//  Description : Get MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetMMSLimitSize(void)
{
    return MMIMMS_MAX_ONE_MMS_SIZE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_Interupt(void)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : for the case if close ps or switch on flight mode while sending win open
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DestroyOperateWaitingWin(void){}

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set if the content saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMSDRM_SetSaved(wchar* w_saved_fname, BOOLEAN isSaved){}
#endif

#endif

#ifdef MMISEARCH_SUPPORT_ONLYMMS
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SearchMMSContent(uint32 record_id, wchar *search_str, uint16 search_len)
{
    MMIMMS_EDIT_DOCUMENT_T  *editdoc_ptr = PNULL;
    MMISMS_ORDER_INFO_T     *mms_order_info_ptr = PNULL;
    MMI_STRING_T            mms_text_string = {0};
    uint16                  slide_num = 0;
    
    MMIMMS_SetNowReadMMSId(record_id);

    mms_order_info_ptr = MMIMMS_GetListItemByID(record_id);

    if (PNULL == mms_order_info_ptr || MMISMS_TYPE_MMS != mms_order_info_ptr->msg_type)
    {
        return FALSE;
    }

    if (MMISMS_MT_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type 
        || MMISMS_MT_NOT_NOTIFY == mms_order_info_ptr->mo_mt_type
        || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type)
    {
        return FALSE;
    }

    editdoc_ptr = MMIMMS_ReadMMS(record_id);

    if (PNULL == editdoc_ptr)
    {
        return FALSE;
    }

    slide_num = MMIMMS_GetSlideCount(editdoc_ptr);

    if(0 == slide_num)
    {
        return FALSE;
    }

    do
    {
        if (1 == editdoc_ptr->editbody_ptr->slide_cur->text_flag)
        {
            if (MMIMMS_GetCurrentSlideText(editdoc_ptr, &mms_text_string))
            {
                if(MMIAPICOM_WstrnstrExt(mms_text_string.wstr_ptr, mms_text_string.wstr_len, search_str, search_len, FALSE))
                {
                    return TRUE;
                }
            }
        }
    } while(MMIMMS_NO_ERROR == MMIMMS_NextSlide(editdoc_ptr));

    return FALSE;
}
#endif


#ifdef MMI_MMS_LIST_DISPLAY_MULTI_PEOPLE
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIMMS_GetSendNumberArray(uint32 record_id, wchar (*sender_array)[MMISMS_NUMBER_MAX_LEN + 1], uint16 array_num)
{
    uint16                      cnt = 0;
    MMIMMS_EDIT_DOCUMENT_T      * editdoc_ptr = PNULL;
    MMIMMS_EDIT_NUMBER_LIST_T   *numberlist_header_ptr = PNULL;
    MMISMS_ORDER_INFO_T         *mms_order_info_ptr = PNULL;

    if (PNULL == sender_array || sender_array == 0)
    {
        return 0;
    }

    mms_order_info_ptr = MMIMMS_GetListItemByID(record_id);

    if (PNULL == mms_order_info_ptr || MMISMS_TYPE_MMS != mms_order_info_ptr->msg_type)
    {
        return 0;
    }

    if (MMISMS_MT_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type 
        || MMISMS_MT_TO_BE_READ == mms_order_info_ptr->mo_mt_type
        || MMISMS_MT_HAVE_READ == mms_order_info_ptr->mo_mt_type
        || MMISMS_MT_SR_TO_BE_READ == mms_order_info_ptr->mo_mt_type 
        || MMISMS_MT_SR_HAVE_READ == mms_order_info_ptr->mo_mt_type
        || MMISMS_MT_NOT_NOTIFY == mms_order_info_ptr->mo_mt_type
        || MMISMS_MT_NEED_NOT_DOWNLOAD == mms_order_info_ptr->mo_mt_type
         )
    {
        return 0;
    }

    editdoc_ptr = MMIMMS_ReadMMS(record_id);

    if (PNULL == editdoc_ptr)
    {
        return 0;
    }

    numberlist_header_ptr = editdoc_ptr->numberlist_header_ptr;

    while(PNULL != numberlist_header_ptr)
    {
        SCI_MEMSET((char*)sender_array[cnt], 0 , MMISMS_NUMBER_MAX_LEN * sizeof(wchar));
        SCI_MEMCPY(sender_array[cnt], numberlist_header_ptr->number_ptr, MIN(numberlist_header_ptr->number_len, MMISMS_NUMBER_MAX_LEN) * sizeof(wchar))

        cnt++;

        if (cnt >= array_num)
        {
            break;
        }

        numberlist_header_ptr = numberlist_header_ptr->next;

    }

    SCI_FREE(editdoc_ptr);

    return cnt;
}
#endif
/*Edit by script, ignore 1 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527

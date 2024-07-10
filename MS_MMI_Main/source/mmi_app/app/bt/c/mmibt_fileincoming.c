/****************************************************************************
** File Name:     mmibt_fileincoming.c                                       *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_FILEINCOMING_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmibt_a2dp.h"
#include "mmibt_nv.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_fileincoming.h"
#include "mmi_appmsg.h"
#ifdef WRE_SUPPORT
#include "Mmiwre_export.h"
#endif
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIBT_FTS_DEL_QUERY_MAX_LEN         300

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct mmibt_file_incoming_param_tag
{
    MMIBT_TRANSFER_FILE_INFO_T    transfer_file_info; 
}MMIBT_FILE_INCOMING_PARAM_T;
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth incoming file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingFileWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
/*****************************************************************************/
//  Description : get text id and ctr id by tran_type
//  Global resource dependence :
//  Author: 
///*****************************************************************************/
LOCAL void GetIncomingFileInfo(MMI_WIN_ID_T win_id, MMIBT_TRANSFER_FILE_INFO_T file_info, MMI_STRING_T *info_str);

/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFileIncomingWin(MMIBT_FILE_INCOMING_PARAM_T *param_ptr);
/*------------------------------------------------------------------------------*/
//incoming file window for file type can be identified
WINDOW_TABLE(MMIBT_INCOMING_FILE_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleIncomingFileWinMsg ),    
    WIN_ID(MMIBT_INCOMING_FILE_WIN_ID),
    WIN_TITLE(TXT_NEW_FILE),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	//WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_TEXT_CTRL(MMIBT_TEXT_BOX_NEW_FILE_DETAIL_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
    END_WIN
};
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth incoming file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingFileWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E                  result = MMI_RESULT_TRUE;
    MMI_STRING_T content_str = {0};
	static BOOLEAN                s_is_cancelled = FALSE;
    MMIBT_FILE_INCOMING_PARAM_T *win_param_ptr = (MMIBT_FILE_INCOMING_PARAM_T *)MMK_GetWinUserData(win_id);
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //display incoming file info
		s_is_cancelled = FALSE;

        //set text to content_str
        GetIncomingFileInfo(win_id, win_param_ptr->transfer_file_info,&content_str);
        //display text
        GUITEXT_SetString(MMIBT_TEXT_BOX_NEW_FILE_DETAIL_CTRL_ID,
            content_str.wstr_ptr,
            content_str.wstr_len,
            FALSE);
        if(NULL != content_str.wstr_ptr)
        {
            SCI_FREE(content_str.wstr_ptr);
            content_str.wstr_ptr = NULL;
        }
#ifdef JAVA_SUPPORT
        MMIAPIJAVA_PauseBackgroundJVM(); 
#endif
#ifdef WRE_SUPPORT
        MMIWRE_PauseMM(); 
#endif

        MMK_SetAtvCtrl(win_id, MMIBT_TEXT_BOX_NEW_FILE_DETAIL_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
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
		        s_is_cancelled = TRUE;
		        if(AssignReceiveFileCfg(win_param_ptr->transfer_file_info.server_type, FALSE))
		        {
			        //enter receiving file win
			        MMIBT_EnterFileTransferWin(win_param_ptr->transfer_file_info.transfer_file_name,
                               win_param_ptr->transfer_file_info.server_type,
                               win_param_ptr->transfer_file_info.transfered_size,
                               win_param_ptr->transfer_file_info.total_file_size);
		        }
		        
                MMK_CloseWin(win_id);
#ifdef WRE_SUPPORT //added by leichi 20120418 fix the bug:the wre media can't be resume after paused by bt
        MMIWRE_ResumeMM(); 
#endif
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                //call bt api to cancel the receiving file process
		        s_is_cancelled = TRUE;
                //StopVibratorTimer();
                MMIBT_CancelFileTransfer(TRUE);
#ifdef JAVA_SUPPORT
                MMIAPIJAVA_ResumeBackgroundJVM();
#endif
#ifdef WRE_SUPPORT //added by leichi 20120414 fix the bug:the wre media can't be resume after paused by bt
        MMIWRE_ResumeMM(); 
#endif
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
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:   
		s_is_cancelled = TRUE;
		if(AssignReceiveFileCfg(win_param_ptr->transfer_file_info.server_type, FALSE))
		{
			//enter receiving file win
			MMIBT_EnterFileTransferWin(win_param_ptr->transfer_file_info.transfer_file_name,
                               win_param_ptr->transfer_file_info.server_type,
                               win_param_ptr->transfer_file_info.transfered_size,
                               win_param_ptr->transfer_file_info.total_file_size);
		}
		
        MMK_CloseWin(win_id);
	#ifdef WRE_SUPPORT
        MMIWRE_ResumeMM(); //leichi added 20120418
	#endif

        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        //call bt api to cancel the receiving file process
		s_is_cancelled = TRUE;
        MMIBT_CancelFileTransfer(TRUE);
#ifdef JAVA_SUPPORT
        MMIAPIJAVA_ResumeBackgroundJVM();
#endif
#ifdef WRE_SUPPORT
        MMIWRE_ResumeMM(); 
#endif
        break;
	case MSG_BT_AUTO_CLOSE_WIN:
		s_is_cancelled = TRUE;
		break;        
    case MSG_CLOSE_WINDOW:
		if(!s_is_cancelled)
		{
			if (MMIBT_FILE_SERVER_OPP_SEND == win_param_ptr->transfer_file_info.server_type)
			{
//				SCI_MEMSET(&g_current_bt, 0x00, sizeof(g_current_bt));
				BT_CancelOppSendFile();
			}
			else if (MMIBT_FILE_SERVER_OPP_RECIEVE == win_param_ptr->transfer_file_info.server_type)
			{
				BT_CancelOppReceiveFile();
			}
			else if (MMIBT_FILE_SERVER_FTP_RECIEVE== win_param_ptr->transfer_file_info.server_type)
			{
				BT_CancelFtpTransfer();
			}
			else if (MMIBT_FILE_SERVER_FTP_SEND == win_param_ptr->transfer_file_info.server_type)
			{
				BT_CancelFtpTransfer();
			}
#ifdef JAVA_SUPPORT
            MMIAPIJAVA_ResumeBackgroundJVM();
#endif
#ifdef WRE_SUPPORT
            MMIWRE_ResumeMM(); 
#endif
		}
        SCI_FREEIF(win_param_ptr);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;     
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterFileIncomingWin(MMIBT_TRANSFER_FILE_INFO_T *param_ptr)
{
    MMIBT_FILE_INCOMING_PARAM_T incoming_file = {0};
    
    if(PNULL != param_ptr)
    {
        SCI_MEMCPY(&incoming_file.transfer_file_info, param_ptr, sizeof(MMIBT_TRANSFER_FILE_INFO_T));
        OpenFileIncomingWin(&incoming_file);
    }
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenFileIncomingWin(MMIBT_FILE_INCOMING_PARAM_T *param_ptr)
{
    MMIBT_FILE_INCOMING_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;  
    
    if(PNULL == param_ptr)
        return res;
    
    if(MMK_IsOpenWin(MMIBT_INCOMING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
    }

    win_param_ptr = (MMIBT_FILE_INCOMING_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_FILE_INCOMING_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMSET(&win_param_ptr->transfer_file_info,0,sizeof(MMIBT_TRANSFER_FILE_INFO_T));
        SCI_MEMCPY(&win_param_ptr->transfer_file_info, &param_ptr->transfer_file_info, 
                        sizeof(param_ptr->transfer_file_info));

        win_handle = MMK_CreateWin((uint32 *)MMIBT_INCOMING_FILE_WIN_TAB, (ADD_DATA)win_param_ptr);
        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREEIF(win_param_ptr);
        }
        else
        {
            MMK_SetWinUserData(MMIBT_INCOMING_FILE_WIN_ID,(void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
        }
    }
    else
    {
        SCI_TRACE_LOW("OpenFileIncomingWin alloc fail");
    }

    return res;
}
/*****************************************************************************/
//  Description : get text id and ctr id by tran_type
//  Global resource dependence :
//  Author: 
//  Note:   if info_str not NULL, it will set file information to info_str,
//          if info_str == NULL, it will set file information to Text Label
///*****************************************************************************/
LOCAL void GetIncomingFileInfo(MMI_WIN_ID_T win_id, MMIBT_TRANSFER_FILE_INFO_T file_info, MMI_STRING_T *info_str)
{
    MMI_STRING_T  file_name = {0};
    MMI_STRING_T  file_length_info = {0};
    wchar         length_info_buf[MMIBT_FILE_LENGTH_INFO +1] = {0};
    wchar         file_name_buf[MMIBT_FULL_PATH_LEN +1] ={0};
    uint8         utf8_length_buf[MMIBT_FILE_LENGTH_INFO +1] = {0};
    uint32        lengh = 0;//MMIBT_FTP_REQ_FILE_MAX_LEN << 1 + MMIBT_FILE_LENGTH_INFO << 1+ file_str.length + file_name.length << 1 ;
    uint32        free_space_high= 0;
    uint32        free_space_low= 0;
    uint32        total_free_space= 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_STRING_T  text_str = {0};
    uint16  u16lf[] = {0x0A,0x0D,'\0'};
    
    //get file name
    file_name.wstr_ptr = file_name_buf;
    Get_Transfer_FileName(&file_name);
    //display file length info  0/total size
	MMIAPIFMM_GetFileSizeString(file_info.total_file_size, length_info_buf, MMIBT_FILE_LENGTH_INFO, FALSE);
  
    file_length_info.wstr_ptr = length_info_buf;
    file_length_info.wstr_len = MMIAPICOM_Wstrlen(length_info_buf);//MIN(strlen((char*)utf8_length_buf), MMIBT_FILE_LENGTH_INFO);    
    if(PNULL != info_str)
    {
        lengh = (MMIBT_FTS_DEL_QUERY_MAX_LEN + file_length_info.wstr_len + file_name.wstr_len) +1;
        //only set file info to info_str
        info_str->wstr_ptr = (wchar *)SCI_ALLOC_APP(lengh*sizeof(wchar));
        if(PNULL == info_str->wstr_ptr)
        {
            //SCI_TRACE_LOW:"GetFileInfo alloc fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9631_112_2_18_2_8_20_321,(uint8*)"");
            return;
        }
        SCI_MEMSET(info_str->wstr_ptr, 0x00, (lengh*sizeof(wchar)));
        //new file: file name
        MMI_GetLabelTextByLang(TXT_INPUT_FILE_NAME, &text_str);
        info_str->wstr_len = MIN(text_str.wstr_len, lengh);
        MMIAPICOM_Wstrncpy(info_str->wstr_ptr, text_str.wstr_ptr, info_str->wstr_len);
		if(lengh  <= info_str->wstr_len + file_name.wstr_len +4)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
		
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, file_name.wstr_ptr, file_name.wstr_len);
        info_str->wstr_len += file_name.wstr_len;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
        //file size :xxxK
        MMI_GetLabelTextByLang(TXT_FILE_DETAIL_SIZE, &text_str);
		if(lengh  <= info_str->wstr_len + text_str.wstr_len)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
        info_str->wstr_len += text_str.wstr_len;
		if(lengh <= info_str->wstr_len + file_length_info.wstr_len + 4)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, file_length_info.wstr_ptr, file_length_info.wstr_len);
        info_str->wstr_len += file_length_info.wstr_len;

        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
        //phone not used space:xxMB
        MMIAPIFMM_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(file_info.filedev), 
                                  MMIAPIFMM_GetDevicePathLen(file_info.filedev),
                                  &free_space_high, 
                                  &free_space_low);
        text_id = MMIAPIFMM_GetDeviceFreeSpaceText(file_info.filedev);
        
        total_free_space = ((free_space_high << 22)|(free_space_low >> 10)) ;
        
        MMI_GetLabelTextByLang(text_id, &text_str);
		if(lengh <= info_str->wstr_len + text_str.wstr_len)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
        info_str->wstr_len += text_str.wstr_len;
        if(total_free_space >>10)
        {
            sprintf((char *)utf8_length_buf, "%ld.%03ldM", (total_free_space >>10), ((total_free_space %1024)*1000/1024));
        }
        else
        {
            sprintf((char *)utf8_length_buf, "%ldK", (total_free_space %1024));
        }
        
		if(lengh <= info_str->wstr_len + strlen((char*)utf8_length_buf) + 4)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
        MMI_STRNTOWSTR(&(info_str->wstr_ptr[info_str->wstr_len]), MMIBT_FILE_LENGTH_INFO, (char*)utf8_length_buf, MMIBT_FILE_LENGTH_INFO, strlen(utf8_length_buf));/*lint !e64*/
        info_str->wstr_len += strlen((char*)utf8_length_buf);
        info_str->wstr_ptr[info_str->wstr_len] = 0;
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;
        //aceept or not 
        MMI_GetLabelTextByLang(TXT_BT_FILE_ACCEPT, &text_str);
		if(lengh <= info_str->wstr_len + text_str.wstr_len + 2)
		{
			return;
		}
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, text_str.wstr_ptr, text_str.wstr_len);
        info_str->wstr_len += text_str.wstr_len;//cr149280
        MMIAPICOM_Wstrncat(info_str->wstr_ptr, u16lf, 2);
        info_str->wstr_len += 2;        
    }
}
#endif

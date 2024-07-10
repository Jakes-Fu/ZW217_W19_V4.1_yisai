/****************************************************************************
** File Name:     mmibt_filetransfer.c                                        *
** Author:         yuantao.xu                                            *
** Date:           8/3/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/3/2012    yuantao.xu     Create                                  *
**                                                                         *
****************************************************************************/
#define _MMIBT_FILETRANSFER_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_filetransfer.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "mmidisplay_data.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "guictrl_api.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmisrvaud_api.h"
#include "mmimultim_image.h"
#include "mmidisplay_data.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIBT_SCROLL_BAR_COLOR         MMI_DARK_BLUE_COLOR 

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
 typedef enum
{
    MMIBT_LABEL_ID,
    MMIBT_LABEL_DIME 
}MMIBT_LABEL_ARRAY_INDEX/*lint !e751*/;

typedef struct mmibt_file_transfer_param_tag
{
    wchar						transfer_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1];	//the name of transfer(send or recieve) file
    MMIBT_FILE_SERVER_TYPE_E    transfer_type;    
    uint32     file_size;
    uint32     total_file_size;
}MMIBT_FILE_TRANSFER_PARAM_T;
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/
const uint32 s_labels_array[][MMIBT_LABEL_DIME] = 
{
    {MMIBT_BT_SEND_FILE_LAB1_CTRL_ID},/*lint !e64*/
    {MMIBT_BT_SEND_FILE_LAB2_CTRL_ID},/*lint !e64*/
    {MMIBT_BT_SEND_FILE_LAB3_CTRL_ID},/*lint !e64*/
    {MMIBT_BT_SEND_FILE_TEXT_CTRL_ID}/*lint !e64*/
};
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8 s_bt_transfer_end_timer = 0; //一通电话挂断时，播放嘟嘟音的定时?
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle send file waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFileTransferWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
/*****************************************************************************/
//  Description : create text label
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void Create_TextLabel(MMI_WIN_ID_T win_id, MMIBT_FILE_SERVER_TYPE_E tran_type);
/*****************************************************************************/
//  Description : display transfer file information
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void Display_FileInfo(MMI_WIN_ID_T win_id, MMIBT_FILE_SERVER_TYPE_E tran_type);
/*****************************************************************************/
//  Description : display progress icon of receive or send file
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void DisplayProgressIcon(
                               uint32        pos,
							   BOOLEAN       is_finished,
                               MMI_WIN_ID_T win_id
                               );
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:maryxiao
//	Note:
/*****************************************************************************/
LOCAL void transferEnd(uint8 timer_id, uint32 param);
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Discription: Install Received Drm File
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void MMIBT_InstallReceiveDrmFile(wchar *transfer_file_name);
#endif
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E  OpenFileTransfersWin(MMIBT_FILE_TRANSFER_PARAM_T *param_ptr);
/*----------------------------------------------------------------------------*/
//window for sending  file waiting 
WINDOW_TABLE(MMIBT_SENDING_FILE_WAIT_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleFileTransferWindow ),    
    WIN_ID(MMIBT_SENDING_FILE_WAIT_WIN_ID),
    WIN_TITLE(TXT_BT_SEND_FILE), 
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN    
};

//window for receiving new file waiting 
WINDOW_TABLE(MMIBT_RECEIVING_FILE_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleFileTransferWindow ),    
    WIN_ID(MMIBT_RECEIVING_FILE_WIN_ID),
    WIN_TITLE(TXT_BT_RECEIVE_FILE),    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL), 
    END_WIN    
};

/*****************************************************************************/
//  Discription: Handle send file waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFileTransferWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16              image_width = 0;
	uint16              image_height = 0;
	GUI_BOTH_RECT_T     both_rect = {0};

    MMIBT_FILE_TRANSFER_PARAM_T *win_param = (MMIBT_FILE_TRANSFER_PARAM_T*)MMK_GetWinUserData(win_id);/*lint !e64*/
    win_param->file_size = MMIBT_GetTransferedSize();
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	    both_rect = MMITHEME_GetFullScreenBothRect();
        MMK_SetBothRect(win_id, &both_rect);
		GUIRES_GetImgWidthHeight(&image_width, &image_height,IMAGE_COMMON_PROCESS_GROOVE_1, win_id);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        //create text label
        Create_TextLabel(win_id, win_param->transfer_type);
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_HIDE, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_CANCEL, FALSE);
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_BT_TRANSFERFILE);
#endif
        break;
        
    case MSG_FULL_PAINT:
        MMIBT_ClrClientRect(win_id);
		
		SCI_TRACE_LOW("[HandleFileTransferWindow] [MSG_FULL_PAINT] tranfered size = %d", win_param->file_size);
		DisplayFtpFileSizeInfo(win_param->file_size, MMIBT_BT_SEND_FILE_TEXT_CTRL_ID, win_id);
		{
		    Display_FileInfo(win_id, win_param->transfer_type);
			DisplayProgressIcon(win_param->file_size, FALSE, win_id);   
		}
        break;        
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:    
        //User refuse to accept the file
        MMIBT_StopBTTransferFile();
        break;
        
   case MSG_APP_RED:
   case MSG_APP_WEB:
   case MSG_LOSE_FOCUS:
   case MSG_CTL_MIDSK:
   case MSG_CTL_OK:
        //SCI_TRACE_LOW:"[&&&]HandleFileTransferWindow Status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4396_112_2_18_2_8_9_249,(uint8*)"d",MMIBT_GetTransferStatus());
        if(MMIBT_SENDING_FILE_FOREGROUND == MMIBT_GetTransferStatus())
        {
            MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_BACKGROUND);
        }
        else if(MMIBT_RECEIVEING_FILE_FOREGROUND == MMIBT_GetTransferStatus())
        {
            MMIBT_SetTransferStatus(MMIBT_REVEIVEING_FILE_BACKGROUND);
        }        
        MMK_CloseWin( win_id );
        break;
	case MSG_BT_AUTO_CLOSE_WIN:
		break;		
    case MSG_CLOSE_WINDOW:
        SCI_FREEIF(win_param);
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Discription: Open the blue tooth device details  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterFileTransferWin(wchar	*file_name_ptr,
                                                MMIBT_FILE_SERVER_TYPE_E  transfer_type,
                                                uint32     file_size,
                                                uint32     total_file_size)
{
    MMIBT_FILE_TRANSFER_PARAM_T param_info = {0};

    if(PNULL != file_name_ptr)
    {        
        MMIAPICOM_Wstrncpy(&param_info.transfer_file_name, file_name_ptr, MMIAPICOM_Wstrlen(file_name_ptr));
    }
    param_info.file_size = file_size;
    param_info.total_file_size = total_file_size;
    param_info.transfer_type = transfer_type;
    OpenFileTransfersWin(&param_info);
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E  OpenFileTransfersWin(MMIBT_FILE_TRANSFER_PARAM_T *param_ptr)
{
    BOOLEAN res = MMI_RESULT_FALSE;
    MMIBT_FILE_TRANSFER_PARAM_T *win_param_ptr = PNULL;    
    MMI_HANDLE_T    win_handle = 0;
    MMI_HANDLE_T    win_id = MMIBT_SENDING_FILE_WAIT_WIN_ID;
    uint32 *     win_table_ptr = MMIBT_SENDING_FILE_WAIT_WIN_TAB;
    
    if(PNULL == param_ptr)
        return res;      

    if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }

    if(MMIBT_FILE_SERVER_OPP_RECIEVE == param_ptr->transfer_type
        || MMIBT_FILE_SERVER_FTP_RECIEVE == param_ptr->transfer_type)
    {
        win_table_ptr = MMIBT_RECEIVING_FILE_WIN_TAB;
        win_id = MMIBT_RECEIVING_FILE_WIN_ID;
    }
    
    win_param_ptr = (MMIBT_FILE_TRANSFER_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_FILE_TRANSFER_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMSET(win_param_ptr->transfer_file_name, 0x00, sizeof(win_param_ptr->transfer_file_name));
        MMIAPICOM_Wstrncpy(win_param_ptr->transfer_file_name,param_ptr->transfer_file_name,
                                                        MMIAPICOM_Wstrlen(param_ptr->transfer_file_name));
        
        win_param_ptr->transfer_type= param_ptr->transfer_type; 
        win_param_ptr->file_size= param_ptr->file_size;
        win_param_ptr->total_file_size = param_ptr->total_file_size;
        
        win_handle = MMK_CreateWin(win_table_ptr, (ADD_DATA)win_param_ptr);
        if(!win_handle || MMI_INVALID_ID == win_handle)
        {
            SCI_TRACE_LOW("OpenFileTransfersWin open win fail!");
            SCI_FREEIF(win_param_ptr);            
        }
        else
        {
            MMK_SetWinUserData(win_id,(void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
        }
    }
    SCI_TRACE_LOW("OpenFileTransfersWin malloc memery fail,res %d",res);

    return res;
}
/*****************************************************************************/
//  Description : create text label
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void Create_TextLabel(MMI_WIN_ID_T win_id, MMIBT_FILE_SERVER_TYPE_E tran_type)
{
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetWinClientBothRect(win_id);
    GUILABEL_INIT_DATA_T    init_data = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    int32 i = 0;
    int32 loop_count = 0;
    uint16 font_height = GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT)+5;
    uint16 line_space_h = 0;
	uint16 line_space_v = 0;
	uint16 image_width = 0;
	uint16 image_height = 0;
    if(!MMK_IsOpenWin(win_id))
    {
        //SCI_TRACE_LOW:"Create_TextLabel window already opened !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10227_112_2_18_2_8_21_337,(uint8*)"");
        return;
    }

	GUIRES_GetImgWidthHeight(&image_width, &image_height,IMAGE_COMMON_PROCESS_GROOVE_1, win_id);
    loop_count = ARR_SIZE(s_labels_array);
    if(MMIBT_FILE_SERVER_SEND_CONNECT == tran_type)
    {
        //in connect windows, only need first 2 labels to display
        loop_count = MIN(2, loop_count);
		image_height = 0;
    }
    if(font_height *loop_count + image_height < (both_rect.h_rect.bottom - both_rect.h_rect.top))
	{
		line_space_h = (both_rect.h_rect.bottom - both_rect.h_rect.top - image_height - font_height *loop_count)/(loop_count + 2);
	}
    if(font_height *loop_count + image_width < (both_rect.v_rect.bottom - both_rect.v_rect.top))
	{
		line_space_v = (both_rect.v_rect.bottom - both_rect.v_rect.top - image_height -font_height *loop_count)/(loop_count + 2);
	}

    //create text label
    init_data.both_rect.h_rect.left = both_rect.h_rect.left;
    init_data.both_rect.v_rect.left = both_rect.v_rect.left;
    init_data.both_rect.h_rect.right = both_rect.h_rect.right;
    init_data.both_rect.v_rect.right = both_rect.v_rect.right;
    init_data.both_rect.h_rect.top = init_data.both_rect.h_rect.bottom = both_rect.h_rect.top;
    init_data.both_rect.v_rect.top = init_data.both_rect.v_rect.bottom = both_rect.v_rect.top;
    for(i = 0; i < loop_count; i++)
    {
        init_data.align = GUILABEL_ALIGN_MIDDLE;
		init_data.both_rect.h_rect.top = init_data.both_rect.h_rect.bottom;
		init_data.both_rect.h_rect.bottom = init_data.both_rect.h_rect.bottom + line_space_h + font_height;
		init_data.both_rect.v_rect.top = init_data.both_rect.v_rect.bottom;
		init_data.both_rect.v_rect.bottom = init_data.both_rect.v_rect.bottom + line_space_v + font_height;
        create.ctrl_id = s_labels_array[i][MMIBT_LABEL_ID];
        create.guid    = SPRD_GUI_LABEL_ID;
        create.parent_win_handle = win_id;
        create.init_data_ptr = &init_data;
        MMK_CreateControl(&create); 
    }
}
/*****************************************************************************/
//  Description : display transfer file information
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void Display_FileInfo(MMI_WIN_ID_T win_id, MMIBT_FILE_SERVER_TYPE_E tran_type)
{
    wchar  device_name_buf[BT_DEVICE_NAME_SIZE + 1] = {0};
    wchar  file_name_buf[MMIBT_FULL_PATH_LEN +1] ={0};
    MMI_STRING_T device_name = {0};
    MMI_STRING_T file_name = {0};
    uint16 label_index = 0;
    uint16 label_max_id = ARR_SIZE(s_labels_array);

    device_name.wstr_ptr = device_name_buf;
    Get_Transfer_DeviceName(&device_name, BT_DEVICE_NAME_SIZE);
    file_name.wstr_ptr = file_name_buf;
    Get_Transfer_FileName(&file_name);
    if(MMIBT_FILE_SERVER_SEND_CONNECT == tran_type)
    {
        //正在连接
        GUILABEL_SetTextById(s_labels_array[label_index][MMIBT_LABEL_ID],TXT_CONNECTING,FALSE);
        label_index ++;
        label_index = MIN(label_index, label_max_id -1);
        //device name
        GUILABEL_SetText(s_labels_array[label_index][MMIBT_LABEL_ID],&device_name,FALSE);
    }
	else//arvin zhang added MMK_SetAtvCtrl() to solve NEWMS00210635 and modified the following code structure
	{
		if((MMIBT_FILE_SERVER_OPP_SEND == tran_type) || (MMIBT_FILE_SERVER_FTP_SEND == tran_type))//set send info 
    	{        	
        	GUILABEL_SetTextById(s_labels_array[label_index][MMIBT_LABEL_ID],TXT_BT_SEND_FILE_TO,FALSE);
		}
		else//set receive info 
    	{        	
        	GUILABEL_SetTextById(s_labels_array[label_index][MMIBT_LABEL_ID],TXT_COMMON_RECEIVING,FALSE);
		}
		//device name
        label_index ++;
        label_index = MIN(label_index, label_max_id -1);
        GUILABEL_SetText(s_labels_array[label_index][MMIBT_LABEL_ID],&device_name,FALSE);
        //file name
        label_index ++;
        label_index = MIN(label_index, label_max_id -1);
        GUILABEL_SetText(s_labels_array[label_index][MMIBT_LABEL_ID],&file_name,FALSE);
		MMK_SetAtvCtrl(win_id,s_labels_array[label_index][MMIBT_LABEL_ID]);
    }	
}
/*****************************************************************************/
//  Description : display progress icon of receive or send file
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
LOCAL void DisplayProgressIcon(
                               uint32        pos,
							   BOOLEAN       is_finished,
                               MMI_WIN_ID_T win_id
                               )
{
    GUI_RECT_T  rect = MMITHEME_GetClientRect();
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    uint16              image_width = 0;
	uint16              image_height = 0;
	uint16              font_height = GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT);
	uint16              line_space = 0;  
	uint16              loop_count = ARR_SIZE(s_labels_array);
    MMIBT_FILE_TRANSFER_PARAM_T *win_param = (MMIBT_FILE_TRANSFER_PARAM_T*)MMK_GetWinUserData(win_id);

    if(font_height *loop_count + image_height < (rect.bottom - rect.top))
	{
		line_space = (rect.bottom - rect.top - image_height - font_height *loop_count)/(loop_count + 2);
	}
	
	GUIRES_GetImgWidthHeight(&image_width, &image_height,IMAGE_COMMON_PROCESS_GROOVE_1, win_id);
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

	dis_point.x = rect.left + (rect.right - rect.left - image_width)/2;
	dis_point.y = rect.bottom - image_height - line_space;    
    
	//display progress bar	
	GUIRES_DisplayImg(&dis_point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_COMMON_PROCESS_GROOVE_1,
		&lcd_dev_info);

    if (pos > 0 || is_finished) /*lint !e685 !e568*/
    {
        //display scroll bar
        rect.left = dis_point.x + 1;
        rect.top = dis_point.y + image_height/4;
        if(pos > 0)
        {
            pos = pos + 1;
        }
        //rect.right = rect.left + MMIBT_SCROLL_BAR_DEAFULT_LEN * pos;
		if(win_param->total_file_size > 0)
		{
			rect.right = rect.left + (uint64)(image_width -3)*(uint64)pos/(uint64)win_param->total_file_size;/*lint !e737*/
		}
		else
		{
			rect.right = rect.left + image_width -2;
		}
        rect.bottom = rect.top + image_height/3;
        
        //防止滚动条长度超过进度条
        if ((rect.left + image_width -3) < rect.right)
        {
            rect.right = rect.left + image_width -3;
        }
        LCD_FillRect(&lcd_dev_info, rect, MMIBT_SCROLL_BAR_COLOR);          
    }
}
/*****************************************************************************/
//  Discription: get packet send indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPacketSentInd(BT_MSG_T* msg_body_ptr)
{
    BT_FILE_TRANSFER_T           transfer_info = *(( BT_FILE_TRANSFER_T *) msg_body_ptr->body_ptr);
	MMIBT_TRANSFER_STATUS_TYPE_E transfer_status = MMIBT_TRANSFER_END;
		
    //SCI_TRACE_LOW:"MMIBT_GetPacketReceivedInd: the received data is = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6786_112_2_18_2_8_14_280,(uint8*)"d",transfer_info.transmitted_count);
    MMIBT_SetTransferedSize(transfer_info.transmitted_count);
    transfer_status = MMIBT_GetTransferStatus();
	if((MMIBT_TRANSFER_END == transfer_status) || (MMIBT_TRANSFER_FREE == transfer_status))
    {   
        if(MMK_IsFocusWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
        {
            MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
			MMIBT_UpdateMainMenu(TRUE);
        }
        else
        {
            MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_BACKGROUND);
			MMIBT_UpdateMainMenu(TRUE);
        }        
    }
    if (MMK_IsFocusWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
    {
        DisplayFtpFileSizeInfo(transfer_info.transmitted_count,MMIBT_BT_SEND_FILE_TEXT_CTRL_ID,MMIBT_SENDING_FILE_WAIT_WIN_ID);
        DisplayProgressIcon(transfer_info.transmitted_count, FALSE,MMIBT_SENDING_FILE_WAIT_WIN_ID);   
    }
}

/*****************************************************************************/
//  Discription: get packet received indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPacketReceivedInd(BT_MSG_T* msg_body_ptr)
{
    BT_FILE_TRANSFER_T      transfer_info = *((BT_FILE_TRANSFER_T *)msg_body_ptr->body_ptr);
	
    //SCI_TRACE_LOW:"MMIBT_GetPacketReceivedInd: the received data is = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6824_112_2_18_2_8_14_282,(uint8*)"d",transfer_info.transmitted_count);
    MMIBT_SetTransferedSize(transfer_info.transmitted_count);
    if(MMK_IsFocusWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
        DisplayFtpFileSizeInfo(transfer_info.transmitted_count,MMIBT_BT_SEND_FILE_TEXT_CTRL_ID,MMIBT_RECEIVING_FILE_WIN_ID);
        DisplayProgressIcon(transfer_info.transmitted_count, FALSE, MMIBT_RECEIVING_FILE_WIN_ID);
    }   
}
/*****************************************************************************/
//  Discription: get send file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendFileResInd(BT_MSG_T* msg_body_ptr)
{
    BT_FILE_TRANSFER_T      transfer_info = {0};
    
    //MMIBT_SetTransferStatus(MMIBT_TRANSFER_FREE);
	//MMIBT_UpdateMainMenu(TRUE);
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_SendFileResInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6903_112_2_18_2_8_14_283,(uint8*)"");
        return;
    }

	transfer_info = *((BT_FILE_TRANSFER_T *)msg_body_ptr->body_ptr);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
		MMIDEFAULT_SetBackLight(TRUE);
        //send file success 
        //redraw send 100% progress bar agin
        if (MMK_IsFocusWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
        {
            DisplayFtpFileSizeInfo(transfer_info.transmitted_count,MMIBT_BT_SEND_FILE_TEXT_CTRL_ID, MMIBT_SENDING_FILE_WAIT_WIN_ID);
            DisplayProgressIcon(transfer_info.transmitted_count, TRUE, MMIBT_SENDING_FILE_WAIT_WIN_ID);
        }
        //reset blue tooth status
        MMIBT_SetTransferStatus(MMIBT_TRANSFER_FREE);
    }
    else
    {       
        MMIBT_SendFileFailedInd();
        MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
    }
	
}
/*****************************************************************************/
//  Discription: get receive file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ReceiveFileResInd(BT_MSG_T* msg_body_ptr, BOOLEAN is_fts)
{
    BT_FILE_INFO            file_info = {0};
    uint32                  file_size = 0;
    BT_FILE_TRANSFER_T      transfer_info = {0};	
    MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
    MMIBT_UpdateMainMenu(TRUE);
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_ReceiveFileResInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7007_112_2_18_2_8_14_285,(uint8*)"");
        return;
    }
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);

    if(is_fts)
    {
        file_info = *((BT_FILE_INFO *)msg_body_ptr->body_ptr);
        file_size  = file_info.size;    
    }
    else
    {
	transfer_info = *((BT_FILE_TRANSFER_T *)msg_body_ptr->body_ptr);
	file_size  = transfer_info.transmitted_count;
    }
	
    SCI_TRACE_LOW("[MMIBT] MMIBT_ReceiveFileResInd msg_body_ptr->status = %d, file_size = %d", msg_body_ptr->status, file_size);
    
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        MMIDEFAULT_SetBackLight(TRUE);	
        MMIBT_UpdateCurrentFileList();
        UpdateCurrentPictureViewList();
            
        //redraw received 100% progress bar
        if(MMK_IsFocusWin(MMIBT_RECEIVING_FILE_WIN_ID))
        {
            DisplayFtpFileSizeInfo(file_size, MMIBT_BT_SEND_FILE_TEXT_CTRL_ID, MMIBT_RECEIVING_FILE_WIN_ID);
            //display progress icon
            DisplayProgressIcon(file_size, TRUE, MMIBT_RECEIVING_FILE_WIN_ID);
        }
        else
        {
            SCI_TRACE_LOW("[MMIBT] MMIBT_ReceiveFileResInd not focus win MMIBT_RECEIVING_FILE_WIN_ID");
        }
        s_bt_transfer_end_timer = MMK_CreateTimerCallback(500, transferEnd,MMIBT_RECEIVING_FILE_WIN_ID, FALSE);
    }
    else
    {
        //receive file fail
        MMIBT_ReceiveFileFail();
        MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    }    
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:maryxiao
//	Note:
/*****************************************************************************/
LOCAL void transferEnd(uint8 timer_id, uint32 param)
{
#ifdef DRM_SUPPORT
    MMIBT_FILE_TRANSFER_PARAM_T *win_param = (MMIBT_FILE_TRANSFER_PARAM_T*)MMK_GetWinUserData(param);/*lint !e64*/
#endif
    uint16                 received_file_suffix[MMIFILE_FULL_PATH_MAX_LEN] ={0};    // new received file suffix
    
    SCI_TRACE_LOW("MMIBT transferEnd s_bt_transfer_end_timer %d", s_bt_transfer_end_timer);
    if (0 != s_bt_transfer_end_timer)
    {                
        MMK_StopTimer(s_bt_transfer_end_timer);
        s_bt_transfer_end_timer = 0;
        
        if (param == MMIBT_SENDING_FILE_WAIT_WIN_ID)
        { 
		    MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);	
			ClosePrmAndOpenDoneWin(
				MMIBT_SENDING_FILE_WAIT_WIN_ID,
				TXT_COMM_SEND_SUCCEED, 
				IMAGE_PUBWIN_SUCCESS,
				MMI_2SECONDS
				); 
            MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
        }
        else if (param == MMIBT_RECEIVING_FILE_WIN_ID)
        { 
#ifdef DRM_SUPPORT
            MMIAPICOM_GetLongFileNameAndSuffix(win_param->transfer_file_name,received_file_suffix,PNULL);
            if(MMIBT_CheckNewFileType(received_file_suffix)
			    &&(MMIBT_FILE_SERVER_OPP_RECIEVE == win_param->transfer_type))
            {
                MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);			
                MMIBT_InstallReceiveDrmFile(win_param->transfer_file_name);
            }
#endif
        //receive file success
            MMIBT_OpenNewReceivedFileWin(PNULL);
            MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);          
        }
    }
}
/*****************************************************************************/
//  Discription: get receive file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetSendFileEndTimer(void)
{
    //if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
    if(MMIBT_TRANSFER_END != MMIBT_GetTransferStatus())
    {
        if(0 != s_bt_transfer_end_timer)
        {
            MMK_StopTimer(s_bt_transfer_end_timer);
            s_bt_transfer_end_timer = 0;
        }
        s_bt_transfer_end_timer = MMK_CreateTimerCallback(500, transferEnd,MMIBT_SENDING_FILE_WAIT_WIN_ID, FALSE); 
    }
    MMIBT_SetCurBtInfo(PNULL);
}
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Discription: Install Received Drm File
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void MMIBT_InstallReceiveDrmFile(wchar *transfer_file_name)
{   
    MMIFILE_DEVICE_E            filedev = MMI_DEVICE_UDISK;
    BOOLEAN                     is_exist = FALSE;
    wchar                       full_path_arr[MMIBT_FULL_PATH_LEN +1] = {0};	
    // Only thie type drm file could be transfer by bt
    MIME_TYPE_E                 mime_type = MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT;
    
    filedev = MMIAPIFMM_GetDeviceTypeByPath(transfer_file_name, MMIFILE_DEVICE_LEN);
    
    MMIAPICOM_CombineDefaultFullPathByFilename(filedev,
            transfer_file_name, MMIAPICOM_Wstrlen(transfer_file_name),
            (wchar *)full_path_arr, MMIBT_FULL_PATH_LEN);
    
	is_exist = MMIAPIFMM_IsFileExist((wchar *)full_path_arr,MMIBT_FULL_PATH_LEN);

    if (is_exist)
    {
        //SCI_TRACE_LOW:"mmibt_wintab.c: start to install drm!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11237_112_2_18_2_8_23_359,(uint8*)"");
        //不是通过蓝牙下载，后面2个参数均为空
        MMIAPIDRM_StartToInstallDrm(full_path_arr, mime_type, PNULL, PNULL,0);
    }
    else
    {
        //SCI_TRACE_LOW:"mmibt_wintab.c: file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11243_112_2_18_2_8_23_360,(uint8*)"");
    }
}
#endif
#endif


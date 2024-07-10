/****************************************************************************
** File Name:      mmiautodemo_main.c                                  *
** Author:         chunjie.liu                                                            *
** Date:           3/17/2012                                             *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2/2012         chujie liu         Create
****************************************************************************/
#define _MMIAUTODEMO_MAIN_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
// #ifdef __cplusplus
// extern   "C"
// {
// #endif
    
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
#include "mmk_type.h"
#include "mmiautodemo_id.h"
#include "mmiautodemo_export.h"
#include "Window_parse.h"
#include "Guilistbox.h"
#include "mmiautodemo_internal.h"
#include "mmifmm_internal.h"
#include "mmifmm_export.h"
#include "mmiautodemo_text.h"
#include "mmiautodemo_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
//LOCAL wchar				s_filename_ptr[MMIFMM_FULL_FILENAME_LEN +1]     = {0}; //文件全名的指针

/*will modify later, window will add menu to select udisk or sd card*/
LOCAL  wchar			s_dir[MMIFMM_PATHNAME_LEN]					    = L"D:\\AutoDemo\\Script";

LOCAL MMIFILE_HANDLE	s_file_handle								= PNULL; //读取文件handle
//LOCAL int32				s_file_cur_pos								    = 0; //读取文件到cmd时不足一行时回退的大小
//LOCAL uint8				s_cmd_buf_ptr[MMI_AUTO_DEMO_CMD_TOREAD_SIZE]    ={0}; //读取文件到cmd时的buffer
//LOCAL uint16			s_cmd_buf_len									= MMI_AUTO_DEMO_CMD_TOREAD_SIZE; //读取文件到cmd的大小
LOCAL BOOLEAN			s_is_readfile_over							= FALSE; //文件是否读完
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : to handle auto demo window message
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/                                           
LOCAL MMI_RESULT_E  HandleAutoDemoWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to handle auto demo disk opt window message
//  Global resource dependence : s_dir
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutodemoDiskOptWinMsg(
												MMI_WIN_ID_T    win_id, 
												MMI_MESSAGE_ID_E   msg_id, 
												DPARAM             param
                                             );
/*****************************************************************************/
//  Description : append list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint16                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           mid_softkey_id
                          );
/********************************************************************************
//  Description : append list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
********************************************************************************/
LOCAL void AppendDiskListItem(
                            MMI_CTRL_ID_T	ctrl_id,
                            MMI_TEXT_ID_T	text_id                          
                            );
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                               *
**--------------------------------------------------------------------------*/


WINDOW_TABLE( MMIAD_AUTO_DEMO_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAutoDemoWindow ),    
    WIN_ID( MMIAD_AUTO_DEMO_WIN_ID ),
    WIN_TITLE(TXT_AUTODEMO_TITLE),    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAD_AUTO_DEMO_LISTBOX_CTRL_ID), 
	WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIAD_DISK_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleAutodemoDiskOptWinMsg),
	WIN_ID(MMIAD_DISK_OPT_MENU_WIN_ID),
    WIN_TITLE(TXT_DISKOPT_TITLE),
	//WIN_STYLE( WS_HAS_TRANSPARENT),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIAD_DISK_OPT_LISTBOX_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
#endif
	END_WIN
};
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle auto demo window message
//  Global resource dependence : s_dir, s_filename_ptr,s_cmd_buf_ptr, s_cmd_buf_len, s_is_readfile_over
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoDemoWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E         recode			= MMI_RESULT_TRUE;
    MMI_CTRL_ID_T        ctrl_id		= MMIAD_AUTO_DEMO_LISTBOX_CTRL_ID;
    uint16               index			= 0;
    MMIFILE_FIND_DATA_T    find_result   =  {0};
    uint16			     cur_index		= 0;
    const GUILIST_ITEM_T *cur_list_ptr	= {0};
    MMI_STRING_T		 name_str		= {0};
    wchar				 my_suffix[]	= L".txt";//will modify later by config menu
    wchar				 my_suffix2[]	= L"txt";
    wchar				 my_suffix3[]	= L"\\*.*";
    wchar	name_ptr[MMIFMM_FILE_FILE_NAME_MAX_LEN]	= {0};      //out, one of name_ptr and suffix_ptr can be NULL
    uint16	name_len								= MMIFMM_FILE_FILE_NAME_MAX_LEN;      //out, 双字节为单位
    wchar	suffix_ptr[MMI_AUTO_DEMO_SUFFIX_LEN]	= {0};        //out
    uint16	suffix_len								= MMI_AUTO_DEMO_SUFFIX_LEN;     //in/out, 双字节为单位, in表示后缀名的最大长度	
    wchar				filepath_ptr[MMIFMM_FULL_FILENAME_LEN +1]     = {0}; //文件全名的指针
    uint32  dir_len = 0;
    MMIFILE_HANDLE	file_handle								= PNULL;


    switch(msg_id)  
    {
    case MSG_OPEN_WINDOW:
	{			
           MMK_SetAtvCtrl(win_id, ctrl_id);
    	    GUILIST_SetMaxItem(ctrl_id, MMI_AUTO_DEMO_LIST_NUM, FALSE);
            // 形成路径 如D:\AutoDemo\Script\*.* 用于查找
            SCI_MEMSET(filepath_ptr, 0, sizeof(filepath_ptr));
            dir_len = MMIAPICOM_Wstrlen(s_dir);
            MMI_WSTRNCPY(
    			filepath_ptr,
    			MMIFMM_FULL_FILENAME_LEN,
    			s_dir,
    			MMIFMM_FULL_FILENAME_LEN,
    			MIN(dir_len, MMIFMM_FULL_FILENAME_LEN)
    			);
            MMI_WSTRNCPY(
    			filepath_ptr + dir_len,
    			MMIFMM_FULL_FILENAME_LEN,
    			my_suffix3,
    			MMIFMM_FULL_FILENAME_LEN,
    			MIN(MMIAPICOM_Wstrlen(my_suffix3), MMIFMM_FULL_FILENAME_LEN)/*lint !e666*/
    			);
            
            file_handle = MMIAPIFMM_FindFirstFile(filepath_ptr, MMIAPICOM_Wstrlen(filepath_ptr), &find_result);
         
            if(SFS_INVALID_HANDLE != file_handle)
            {          
                do
                {
                     MMIAPIFMM_SplitFileName(
    				find_result.name,     //in
    				MMIAPICOM_Wstrlen(find_result.name),      //in, 双字节为单位
    				name_ptr,          //out, one of name_ptr and suffix_ptr can be NULL
    				&name_len,      //out, 双字节为单位
    				suffix_ptr,        //out
    				&suffix_len     //in/out, 双字节为单位, in表示后缀名的最大长度
                    );
                    if (!MMIAPICOM_Wstrncmp(suffix_ptr,my_suffix2,suffix_len))
                    {
                        AppendListItem(name_ptr,name_len, ctrl_id, STXT_SELECT);
                    }
                    //查找下一个文件
                    
                }while(SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(file_handle, &find_result));
                
                    SFS_FindClose(file_handle);
            }
    	    s_file_handle = PNULL;
    	    s_is_readfile_over = FALSE;
           break;
    	}
	
    case MSG_GET_FOCUS:
        GUILIST_RemoveAllItems(ctrl_id);
        // 形成路径 如D:\AutoDemo\Script\*.* 用于查找
        SCI_MEMSET(filepath_ptr, 0, sizeof(filepath_ptr));
        dir_len = MMIAPICOM_Wstrlen(s_dir);
        MMI_WSTRNCPY(
			filepath_ptr,
			MMIFMM_FULL_FILENAME_LEN,
			s_dir,
			MMIFMM_FULL_FILENAME_LEN,
			MIN(dir_len, MMIFMM_FULL_FILENAME_LEN)
			);
        MMI_WSTRNCPY(
			filepath_ptr + dir_len,
			MMIFMM_FULL_FILENAME_LEN,
			my_suffix3,
			MMIFMM_FULL_FILENAME_LEN,
			MIN(MMIAPICOM_Wstrlen(my_suffix3), MMIFMM_FULL_FILENAME_LEN)/*lint !e666*/
			);
        
        file_handle = MMIAPIFMM_FindFirstFile(filepath_ptr, MMIAPICOM_Wstrlen(filepath_ptr), &find_result);
     
        if(SFS_INVALID_HANDLE != file_handle)
        {         
            do
            {
                MMIAPIFMM_SplitFileName(
				find_result.name,     //in
				MMIAPICOM_Wstrlen(find_result.name),      //in, 双字节为单位
				name_ptr,          //out, one of name_ptr and suffix_ptr can be NULL
				&name_len,      //out, 双字节为单位
				suffix_ptr,        //out
				&suffix_len     //in/out, 双字节为单位, in表示后缀名的最大长度
                );
                if (!MMIAPICOM_Wstrncmp(suffix_ptr,my_suffix2,suffix_len))
                {
                    AppendListItem(name_ptr,name_len, ctrl_id, STXT_SELECT);
                }
                //查找下一个文件
                
            }while(SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(file_handle, &find_result));
            
            SFS_FindClose(file_handle);
        }
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
	case MSG_APP_MENU:
    case MSG_APP_OK:
        MMIAPIAUTODEMO_OpenDiskOptWindow();
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:                   
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		cur_index = GUILIST_GetCurItemIndex(ctrl_id);
        cur_list_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, cur_index);
        name_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr;
        name_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
        dir_len = MMIAPICOM_Wstrlen(s_dir);
        SCI_MEMSET(filepath_ptr, 0, sizeof(filepath_ptr));
		MMI_WSTRNCPY(
			filepath_ptr,
			MMIFMM_FULL_FILENAME_LEN,
			s_dir,
			MMIFMM_FULL_FILENAME_LEN,
			MIN(dir_len, MMIFMM_FULL_FILENAME_LEN)
			);

		MMI_WSTRNCPY(
			filepath_ptr + MMIAPICOM_Wstrlen(s_dir),
			MMIFMM_FULL_FILENAME_LEN,
			L"\\",
			MMIFMM_FULL_FILENAME_LEN,
			1
			);
		MMI_WSTRNCPY(
					filepath_ptr + MMIAPICOM_Wstrlen(s_dir) + 1,
					MMIFMM_FULL_FILENAME_LEN,
					name_str.wstr_ptr,
					MMIFMM_FULL_FILENAME_LEN,
					MIN(name_str.wstr_len, MMIFMM_FULL_FILENAME_LEN)
					);

		MMI_WSTRNCPY(
			filepath_ptr + name_str.wstr_len + MMIAPICOM_Wstrlen(s_dir) +1,
			MMIFMM_FULL_FILENAME_LEN,
			my_suffix,
			MMIFMM_FULL_FILENAME_LEN,
			4
			);
        //filepath_ptr[name_str.wstr_len+MMIAPICOM_Wstrlen(s_dir) + 5] = 0x0000;
		if(PNULL == s_file_handle)
		{
			s_file_handle = MMIAPIFMM_CreateFile(filepath_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
			if (SFS_INVALID_HANDLE == s_file_handle) 
			{
				SCI_TRACE_LOW("HandleAutoDemoWindow Create Autodemo Script File Failed\n");
			}

		}
		MMIAPIAD_Start();
        break;  
		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

     case MSG_CLOSE_WINDOW:
        break;  
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Description : to handle auto demo disk opt window message
//  Global resource dependence : s_dir
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutodemoDiskOptWinMsg(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E        recode			                = MMI_RESULT_TRUE;
    MMIFILE_DEVICE_E    device_type	                    = MMI_DEVICE_UDISK;
    MMI_TEXT_ID_T		text_id		                    = TXT_NULL;   
	wchar			    device_name_element[10]         = {'0'};
	wchar* 			    device_name_ptr	                = device_name_element;
    uint16              device_name_len                 = 0;
    static wchar		device_name_arry[MMI_DEVICE_NUM] = {0};
    MMI_CTRL_ID_T       ctrl_id		                    = MMIAD_DISK_OPT_LISTBOX_CTRL_ID;
	uint16			    i				                = 0;
    static uint16		s_cur_selected_index            = 0;

    switch(msg_id)  
    {
    case MSG_OPEN_WINDOW:
	{			
        MMK_SetAtvCtrl(win_id, ctrl_id);
	    GUILIST_SetMaxItem(ctrl_id, MMI_AUTO_DEMO_LIST_NUM, FALSE);
        while(MMI_DEVICE_SYSTEM < device_type && device_type < MMI_DEVICE_NUM) 
        { 			
  			text_id = MMIAPIFMM_GetDeviceName(device_type);
			device_name_ptr = MMIAPIFMM_GetDevicePath(device_type);
            if (PNULL == device_name_ptr)
            {
                SCI_TRACE_LOW("Autodemo HandleAutodemoDiskOptWin: Get Device Path Failed\n");
                recode = MMI_RESULT_FALSE;

            }
            else
            {
                device_name_arry[i++] = *device_name_ptr;
			    AppendDiskListItem(ctrl_id, text_id);
            }
            device_type++;
        }
		GUILIST_SetSelectedItem(ctrl_id, s_cur_selected_index, TRUE);
        break;
	}
	
    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		s_cur_selected_index = GUILIST_GetCurItemIndex(ctrl_id);
	    s_dir[0] = device_name_arry[s_cur_selected_index];
		MMK_CloseWin(win_id);
        break;  
		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

     case MSG_CLOSE_WINDOW:
		//MMIAPIFMM_CloseFile(s_file_handle);
        break;  
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : apend list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint16                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           mid_softkey_id
                          )
{
    GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
    
    //item_data.softkey_id[0] = TXT_OPTION;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = STXT_RETURN;    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    GUILIST_AppendItem(ctrl_id, &item_t);      
}

/********************************************************************************
//  Description : apend list item
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
********************************************************************************/
LOCAL void AppendDiskListItem(
                            MMI_CTRL_ID_T	ctrl_id,
                            MMI_TEXT_ID_T	text_id                          
                            )
{
    GUILIST_ITEM_T      item_t    = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id= text_id;
    GUILIST_AppendItem(ctrl_id, &item_t); 
    
    /*
    item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
    item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
    item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;    */
    
}
/*****************************************************************************/
//  Description : read file to cmd
//  Global resource dependence : s_cmd_buf_ptr, s_cmd_buf_len, s_is_readfile_over,s_file_cur_pos
//  Author: chunjie.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUTODEMO_LoadCmd(
                                    uint8* cmd_buf,
									uint32 *cmd_buf_len_ptr
                                 )
{
    uint32			bytes_to_read								= MMI_AUTO_DEMO_CMD_TOREAD_SIZE; //将要从脚本文件中读取到cmd的数据大小
    MMIFILE_ERROR_E file_read_error								= SFS_NO_ERROR;  //脚本文件是否读取完成
    BOOLEAN         is_load										= FALSE;  //本次读取是否成功
    uint32			data_str_index								= 0;   //读入数据有行数据时往前查找换行的下标
    uint32			bytes_read									= 0;   //本次读取到的数据大小
    uint32			cmd_buf_len								    = MMI_AUTO_DEMO_CMD_TOREAD_SIZE;  //从脚本文件中读入数据的大小
    LOCAL uint32	s_file_cur_pos								= 0; //读取文件到cmd时不足一行时回退的数据大小
    LOCAL uint8		s_cmd_buf_ptr[MMI_AUTO_DEMO_CMD_TOREAD_SIZE]    ={0}; //读取文件到cmd时的buffer
    //把上次读取数据时不足整行的数据拷贝到本次读取数据的buffer中    
    //SCI_PASSERT(PNULL != cmd_buf && PNULL != cmd_buf_len_ptr, ("MMIAPIAUTODEMO_LoadCmd, PNULL != cmd_buf && PNULL != cmd_buf_len_ptr"));
    if(PNULL == cmd_buf)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("MMIAPIAUTODEMO_LoadCmd, cmd_buf = %x, cmd_buf_len_ptr = %d", cmd_buf, *cmd_buf_len_ptr);

    if (0 != s_file_cur_pos)
    {
        MMIAPICOM_CopyString(
                                                    (uint8*)&s_file_cur_pos,       //OUT:
            cmd_buf,     //OUT:
            s_file_cur_pos,             //IN:
            s_file_cur_pos + 1,            //IN:
            s_cmd_buf_ptr       //IN:
            );
    }
    
    bytes_to_read -= s_file_cur_pos;
    //读取脚本数据到buffer
    file_read_error = MMIAPIFMM_ReadFile(
                                        s_file_handle,
                                        cmd_buf + s_file_cur_pos,
                                        bytes_to_read,
                                        &bytes_read,
                                        PNULL
                                        );
    
    if (SFS_NO_ERROR == file_read_error)
    {
		//if分支是对读完脚本文件的处理
        if (bytes_read < bytes_to_read)
        {
            s_is_readfile_over = TRUE;
            SCI_MEMSET(cmd_buf+ s_file_cur_pos+bytes_read, 0, (bytes_to_read-bytes_read))
            cmd_buf_len = bytes_read + s_file_cur_pos;

                
        }
        //此分支是对未读完脚本文件的处理
        else
        {
            data_str_index = bytes_to_read + s_file_cur_pos - 1;
            //此循环追踪不足整行数据的最近的换行符
            while (cmd_buf[data_str_index] != '\n')
            {
                data_str_index--;
            }
            //去掉不足整行数据后的相关变量的设置
            bytes_read =  1 + data_str_index;
            cmd_buf_len = 1 + data_str_index;		
            if (bytes_read != bytes_to_read + s_file_cur_pos)
            {
                s_file_cur_pos += (bytes_to_read - bytes_read);
            }
			//把不足整行的数据拷贝到内存中，保留下次读取时使用
            MMIAPICOM_CopyString(
                                                            (uint8*)&s_file_cur_pos,       //OUT:
                                s_cmd_buf_ptr,     //OUT:
                                s_file_cur_pos,             //IN:
                                s_file_cur_pos + 1,            //IN:
                                cmd_buf + cmd_buf_len
                                );
        }

        is_load = TRUE;
        *cmd_buf_len_ptr = cmd_buf_len;
        if(s_is_readfile_over)
        {
            s_file_cur_pos = 0;
        }
        SCI_TRACE_LOW("MMIAPIAUTODEMO_LoadCmd, SFS_NO_ERROR == file_read_error cmd_buf = %x, cmd_buf_len_ptr = %d", cmd_buf, *cmd_buf_len_ptr);
    }
    
    SCI_TRACE_LOW("MMIAPIAUTODEMO_LoadCmd, file_read_error = %d, data_str_index = %d", file_read_error, data_str_index);

    return is_load;
}

/*****************************************************************************/
//  Description : to open autodemo main menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUTODEMO_OpenAutodemoWindow(void)
{
     return MMK_CreateWin((uint32*)MMIAD_AUTO_DEMO_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : to open DiskOpt menu window
//  Global resource dependence : none
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAUTODEMO_OpenDiskOptWindow(void)
{
	return MMK_CreatePubListWin((uint32*)MMIAD_DISK_OPT_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : is read  the file over
//  Global resource dependence : s_is_readfile_over
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/     
PUBLIC BOOLEAN MMIAPIAUTODEMO_IsFileOver(void)
{
    return s_is_readfile_over;
}    

/*****************************************************************************/
//  Description : get file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC MMIFILE_HANDLE MMIAPIAUTODEMO_GetFileHandle(void)
{
    return s_file_handle;
}    

/*****************************************************************************/
//  Description : set file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC void MMIAPIAUTODEMO_SetFileHandle(MMIFILE_HANDLE file_handle)
{
    s_file_handle = file_handle;
}       
// #ifdef   __cplusplus
//     }
// #endif
    


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527

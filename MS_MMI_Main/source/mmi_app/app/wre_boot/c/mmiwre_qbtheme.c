#include "mmipub.h"
#include "mmi_link.h"
#include "theme_gzip_export.h"

#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_internal.h"
#include "mmiqbtheme_text.h"
#endif
#include "mmiwre_qbtheme.h"
#include "mmi_utility.h"
#include "mmk_timer.h"

#include "mmk_app.h"

#include "mmiwre_text.h"

#include "mmiwre_other.h"

#include "mmi_resource.h"

#ifdef QBTHEME_SUPPORT
//hain
//#define QBTHEME_NAME_EN_MAX_LEN	31	
#define WREQBTHEME_TIMEOUT 5000


//请维护以下结构与mmiqbtheme_app.c中
//的QBTHEME_PACKAGE_HEADER_T结构一致
typedef struct
{
	uint32 tag;
	uint32 version;
	uint32 hdr_size;
	uint16 lcd_width;
	uint16 lcd_height;
	uint8  zip;
	uint8  is_bigendian;
	uint8  reserved[14];
	wchar  name[QBTHEME_NAME_EN_MAX_LEN + 1];
}WREQBTHEME_PACKAGE_HEADER_T;

PUBLIC MMI_APPLICATION_T g_wre_qbtheme_app={0};

LOCAL wchar s_wre_qbtheme_file_name[QBTHEME_NAME_EN_MAX_LEN + 1] = {0};							  
LOCAL wchar s_wre_qbtheme_point_out_info[QBTHEME_NAME_EN_MAX_LEN+1]={0};


extern WINDOW_TABLE( QBTHEME_PREVIEW_GLIDE_WIN_TAB );
extern BOOLEAN IsVersionMatch(uint32 ver);

MMI_RESULT_E HandleWREQbQueryOKWinMsg(
									   MMI_WIN_ID_T        win_id,    //IN:
									   MMI_MESSAGE_ID_E    msg_id,    //IN:
									   DPARAM              param    //IN:
									   )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T       *win_info_ptr   = PNULL;
    
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            MMIAPIWRE_ExitWre();            
            MMIAPIMENU_SetQbThemeIdleStyle();
            MMIPUB_CloseQuerytWin(&win_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&win_id);
        break;
        
    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);	
}
BOOLEAN MMIAPIWRE_QBTHEME_QueryIdleStyle()
{
	BOOLEAN result = TRUE;
	MMISET_IDLE_STYLE_E set_idle_style = MMIAPISET_GetIdleStyle();

	if(set_idle_style != MMISET_IDLE_QBTHEME)
	{
		MMI_STRING_T tmptxt = {0};
		MMI_WIN_PRIORITY_E tmp_priority = WIN_ONE_LEVEL;
		result = FALSE;

		SCI_MEMSET(s_wre_qbtheme_point_out_info,0,QBTHEME_NAME_EN_MAX_LEN*2);
		MMIRES_GetText(TXT_OPEN_COMN,PNULL,&tmptxt);
		MMIAPICOM_Wstrncpy(s_wre_qbtheme_point_out_info,tmptxt.wstr_ptr,tmptxt.wstr_len);
		MMIRES_GetText(TXT_QB_QBTHEME_IDLE,PNULL,&tmptxt);
		MMIAPICOM_Wstrncat(s_wre_qbtheme_point_out_info,tmptxt.wstr_ptr,tmptxt.wstr_len);
		tmptxt.wstr_ptr = s_wre_qbtheme_point_out_info;
		tmptxt.wstr_len = MMIAPICOM_Wstrlen(tmptxt.wstr_ptr);
		
	    	MMIPUB_OpenQueryWinByTextPtrEx(VIRTUAL_WIN_ID, &tmptxt, PNULL,\
	        	IMAGE_PUBWIN_QUERY, NULL,HandleWREQbQueryOKWinMsg,0);
	}
	return result;
}
//TRUE 主题安装成功或者已经存在;FALSE 程序出错
BOOLEAN MMIAPIWRE_QBTHEME_Install(wchar* pkg_path_ptr)
{
	BOOLEAN bret = FALSE;
	SFS_ERROR_E sfs_ret = SFS_ERROR_NOT_EXIST;
	MMI_TEXT_ID_T err_text = TXT_QB_INSTALL_FAIL;
	SFS_HANDLE pkg_handle = PNULL;
	WREQBTHEME_PACKAGE_HEADER_T* pkg_hdr_ptr = PNULL;
	if(!MMIAPIWRE_QBTHEME_QueryIdleStyle())
	{
		MMIPUB_CloseAlertWin();
		return bret;
	}
	if (PNULL == pkg_path_ptr)
	{
		return bret;
	}
	
	pkg_handle = SFS_CreateFile(pkg_path_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);
	if( 0 == pkg_handle)
	{
		err_text = TXT_QB_PACKAGE_ERROR;
		sfs_ret = SFS_ERROR_SYSTEM;
		goto WREQB_INSTALL_ERROR;
	}
	//if (0 != pkg_handle)
	{
		uint32	read_cnt = 0;
		pkg_hdr_ptr = (WREQBTHEME_PACKAGE_HEADER_T*)SCI_ALLOC_APPZ(sizeof(WREQBTHEME_PACKAGE_HEADER_T));
		if (PNULL == pkg_hdr_ptr)
		{
			sfs_ret = SFS_ERROR_SYSTEM;
			goto WREQB_INSTALL_ERROR;
		}
		
		SCI_MEMSET(pkg_hdr_ptr, 0, sizeof(WREQBTHEME_PACKAGE_HEADER_T));
		sfs_ret = SFS_ReadFile(pkg_handle, pkg_hdr_ptr, sizeof(WREQBTHEME_PACKAGE_HEADER_T), &read_cnt, PNULL);
		
		if (SFS_ERROR_NONE == sfs_ret)
		{			
			if (QBTHEME_PACKAGE_TAG == B2L_U32_F(&pkg_hdr_ptr->tag)
				&& sizeof(WREQBTHEME_PACKAGE_HEADER_T) == read_cnt
				&& MMI_IsBigEndian() == pkg_hdr_ptr->is_bigendian
                		&& IsVersionMatch(B2L_U32_F(&pkg_hdr_ptr->version)))
			{
				uint16 lcd_width = 0;
				uint16 lcd_height = 0;
				wchar file_name[QBTHEME_NAME_EN_MAX_LEN + 1] = {0};
				wchar root_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
				wchar*str_ptr = NULL;
				// resolution
				MMI_GetMainLCDRealSpecs(&lcd_width, &lcd_height);
				if (GUILCD_IsLandscape(GUI_MAIN_LCD_ID))
				{
					uint16 temp = 0;
					temp = lcd_width;
					lcd_width = lcd_height;
					lcd_height = temp;
				}
				
				if (lcd_width != B2L_U16(&pkg_hdr_ptr->lcd_width)
					|| lcd_height != B2L_U16(&pkg_hdr_ptr->lcd_height))
				{
					err_text = TXT_QB_RESOLUTION_ERROR;
					sfs_ret = SFS_ERROR_INVALID_FORMAT;
					goto WREQB_INSTALL_ERROR;
				}
				
				// name
				MMIAPICOM_Wstrncpy(file_name, pkg_hdr_ptr->name, QBTHEME_NAME_EN_MAX_LEN);
				str_ptr = file_name;
				//if( PNULL != str_ptr )
				{		
					do
					{
						*str_ptr = B2L_U16(str_ptr);
					}while (*str_ptr++);
				}
				//B2L_Wstr(file_name);
				memset(s_wre_qbtheme_file_name,0,QBTHEME_NAME_EN_MAX_LEN*2);
				MMIAPICOM_Wstrncpy(s_wre_qbtheme_file_name, file_name, QBTHEME_NAME_EN_MAX_LEN);
				if((uint16)-1 != MMIQBTHEME_GetIndexByName(file_name))
				{//已经安装过
					SCI_FREE(pkg_hdr_ptr);
					SFS_CloseFile(pkg_handle);	
					bret = TRUE;					
					return bret;
				}
				//s_wre_qbtheme_wait_win = MMK_GetFirstAppletHandle();	
				MMIQBTHEME_GetLocalRootFolder(root_path);	
				MMIAPIWRE_ExitWre();
				if (0 == theme_unzip(pkg_path_ptr, root_path, file_name))
				{
					err_text = TXT_QB_INSTALL_FAIL;
					sfs_ret = SFS_ERROR_SYSTEM;
					goto WREQB_INSTALL_ERROR;	
				}
			}
			else
			{
				err_text = TXT_QB_PACKAGE_ERROR;
				sfs_ret = SFS_ERROR_INVALID_FORMAT;
				goto WREQB_INSTALL_ERROR;
			}
		}
		
		SCI_FREE(pkg_hdr_ptr);
	}
	SFS_CloseFile(pkg_handle);	
	bret = TRUE;
	return bret;
WREQB_INSTALL_ERROR:
	{
		SCI_FREE(pkg_hdr_ptr);
		SFS_CloseFile(pkg_handle);
		
		MMIPUB_OpenAlertFailWin(err_text);		
	}	
	return bret;
}
void WREQBTHEME_HandleActiveMsg(uint32 param)
{
	uint16 cur_idx = MMIQBTHEME_GetIndexByName(s_wre_qbtheme_file_name) + 1; // add system

	MMK_CreateWin((uint32 *)QBTHEME_PREVIEW_GLIDE_WIN_TAB, (ADD_DATA)cur_idx);
}     
void MMIAPIWRE_QBTHEME_Hook_ProcEx(wchar* path)
{
	if(MMIAPIWRE_QBTHEME_Install(path))
	{
		//MMIQBTHEME_GetIndexByName()没有则返回-1
		uint16 cur_idx = MMIQBTHEME_GetIndexByName(s_wre_qbtheme_file_name) + 1;
		if(cur_idx != 0)
		{
			MMIAPIWRE_ExitWre();
			MMIPUB_CloseAlertWin();
			MMK_CreateTimerCallback(500,WREQBTHEME_HandleActiveMsg,PNULL,FALSE);
		}
	}
}
LOCAL MMI_RESULT_E HandleWRE_Qbtheme_AlertWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E        bResult       = MMI_RESULT_TRUE;
	switch(msg_id)
	{
	case MSG_QBTHEME_UNZIP:
			MMIPUB_CloseAlertWin();
			MMIAPIWRE_ExitWre();
			MMK_CreateTimerCallback(10,WREQBTHEME_HandleActiveMsg,PNULL,FALSE);	
		break;
	default:
		bResult = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	}
	return bResult;
}
#endif
BOOLEAN MMIAPIWRE_QBTHEME_Hook_Proc(wchar* path)
{
	#ifdef QBTHEME_SUPPORT
	uint32       waiting_time_out =  WREQBTHEME_TIMEOUT;
	MMI_WIN_PRIORITY_E tmp_priority = WIN_ONE_LEVEL;	
	MMIPUB_OpenAlertWinByTextId( &waiting_time_out, TXT_COMMON_WAITING, NULL, 
								IMAGE_PUBWIN_WAIT, PNULL,
								&tmp_priority ,MMIPUB_SOFTKEY_NONE,HandleWRE_Qbtheme_AlertWinMsg);		
	MMIAPIWRE_QBTHEME_Hook_ProcEx(path);
	return TRUE;
	#else
	return FALSE;
	#endif
}


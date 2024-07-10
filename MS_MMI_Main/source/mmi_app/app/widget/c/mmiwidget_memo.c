/*****************************************************************************
** File Name:      mmiwidget_mp3.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_MEMO_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_MEMO 
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "guibutton.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmimp3_export.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_text.h"
#include "mmiwidget_position.h"
#include "guiform.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifndef MMI_GRID_IDLE_SUPPORT
#define MMIWIDGET_MEMO_EDIT_MAX_LEN        30
#else
#if defined (MAINLCD_SIZE_240X400)
#define MMIWIDGET_MEMO_EDIT_MAX_LEN        64
#define MMIWIDGET_MEMO_EDIT_ADJUST_GAP     6
#define MMIWIDGET_MEMO_TEXT_LEFT            45
#define MMIWIDGET_MEMO_TEXT_TOP             32
#define MMIWIDGET_MEMO_TEXT_RIGHT           35

//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_X     26
//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_Y     25
//#define MMIWIDGET_MEMO_EDIT_ICON_WIDTH     76
#elif defined(MAINLCD_SIZE_240X320)
#define MMIWIDGET_MEMO_EDIT_MAX_LEN        64
#define MMIWIDGET_MEMO_EDIT_ADJUST_GAP     6
#define MMIWIDGET_MEMO_TEXT_LEFT            45
#define MMIWIDGET_MEMO_TEXT_TOP             32
#define MMIWIDGET_MEMO_TEXT_RIGHT           35

//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_X     26
//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_Y     25
//#define MMIWIDGET_MEMO_EDIT_ICON_WIDTH     76
#elif defined(MAINLCD_SIZE_320X480)
#define MMIWIDGET_MEMO_EDIT_MAX_LEN        64
#define MMIWIDGET_MEMO_EDIT_ADJUST_GAP     6
#define MMIWIDGET_MEMO_TEXT_LEFT            24
#define MMIWIDGET_MEMO_TEXT_TOP             36
#define MMIWIDGET_MEMO_TEXT_RIGHT           14

//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_X     33
//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_Y     36
//#define MMIWIDGET_MEMO_EDIT_ICON_WIDTH     96
#else
#define MMIWIDGET_MEMO_EDIT_MAX_LEN        64
#define MMIWIDGET_MEMO_EDIT_ADJUST_GAP     6
#define MMIWIDGET_MEMO_TEXT_LEFT            45
#define MMIWIDGET_MEMO_TEXT_TOP             32
#define MMIWIDGET_MEMO_TEXT_RIGHT           0

//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_X     33
//#define MMIWIDGET_MEMO_EDIT_ICON_ADJUST_Y     36
//#define MMIWIDGET_MEMO_EDIT_ICON_WIDTH     96
#endif
#endif

//according to g_edit_str,g_edit_str1,g_edit_str2
//#if defined (MAINLCD_SIZE_320X480)
//#define   MMIWIDGET_MEMO_TEXT_FONT                   SONG_FONT_16
//#else
#define   MMIWIDGET_MEMO_TEXT_FONT                   MMI_DEFAULT_NORMAL_FONT
//#endif 
#define   MMIWIDGET_MEMO_TEXT_COLOR                   MMI_BLACK_COLOR

#define  MMIWIDGET_MEMO_FULL_PATH_LEN              255

#define FS_FULL_PATH_MEMO        {'E', ':', '\\', 'm', 'e', 'm', 'o', '\\',  'f', 's', '.', 's', 'y', 's',  0}

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
/*! @enum DISPLAY_RECT_E
@brief 显示区域的枚举
*/
#ifdef MMI_GRID_IDLE_SUPPORT
typedef enum
{
    RECT_DISPLAY1,       /*!<显示区域1*/
    RECT_DISPLAY2,       /*!<显示区域2*/
    RECT_DISPLAY3,    /*!<符号“+”区域*/
    RECT_MAX     /*!<保留位*/
} DISPLAY_RECT_E;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


#ifdef MMI_GRID_IDLE_SUPPORT
LOCAL   MMI_STRING_T                g_memo_edit_str[MMIGRID_MEMO_WIDGET_NUM] = {0};
LOCAL   BOOLEAN                     s_memo_status_full[MMIGRID_MEMO_WIDGET_NUM] = {0};
LOCAL   uint32                      g_edit_index = 0;
#else
LOCAL   MMI_STRING_T                g_edit_str = {0};
#endif

LOCAL   const wchar                 fs_full_path[]= FS_FULL_PATH_MEMO;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E  HandleWidgetMemoWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );
#if 1//ndef MMI_GRID_IDLE_SUPPORT
LOCAL void MMIWIDGET_Memo_DisplayMemoPanel(
										   MMI_WIN_ID_T     win_id,
										   MMI_STRING_T *str_ptr);
#else
LOCAL void MMIWIDGET_Memo_DisplayMemoPanel(
										   MMI_WIN_ID_T     win_id,
										   MMI_STRING_T *str_ptr,MMI_STRING_T *str_ptr1,MMI_STRING_T *str_ptr2);
#endif

LOCAL void MMIWIDGET_Memo_GetMemoTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   );
LOCAL MMI_RESULT_E HandleMemoEditWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										);
LOCAL  BOOLEAN MMIWIDGET_Memo_ReadMemoFile(
                                           void
                                           );
LOCAL  BOOLEAN MMIWIDGET_Memo_WriteMemoFile(
                                            void
                                            );
#ifdef MMI_GRID_IDLE_SUPPORT
LOCAL void	Get_Rect(MMI_WIN_ID_T win_id,GUI_RECT_T *rect,DISPLAY_RECT_E rect_type);
LOCAL BOOLEAN  Get_RectIndex(MMI_WIN_ID_T win_id,GUI_POINT_T point, uint32 *index_ptr);
LOCAL void Set_EditRectString(uint32  cur_index);
LOCAL void Get_EditRectString(uint32  cur_index);
LOCAL void Set_GlobalString(MMI_STRING_T str_t,MMI_STRING_T *g_str_t);
LOCAL void Set_GlobalNull(uint32 edit_index);
#endif
/*****************************************************************************/
// Description :GetCurMemoIdx
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
LOCAL uint32 GetCurMemoIdx(MMI_WIN_ID_T win_id);

/**--------------------------------------------------------------------------*/
#ifndef MMI_GRID_IDLE_SUPPORT
PUBLIC const MMIWIDGET_ITEM_INFO_T g_memo_widget =
{
        MMIWIDGET_MEMO_ID, 
		HandleWidgetMemoWinMsg, 
		PNULL,
		PNULL,
		PNULL,
		WIDGET_MEMO_WIN_ID, 
		IMAGE_WIDGET_MEMO_SHORTCUT, 
		IMAGE_WIDGET_MEMO_BG,
		TXT_WIDGET_MEMO,
		50,//    75, 
		50,//    50
		FALSE,
}

#else
PUBLIC const MMIWIDGET_ITEM_INFO_T g_memo_add_widget =
{
        MMIWIDGET_MEMO_ID, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,//    75, 
    	50,//    50
    	FALSE,
};

PUBLIC const MMIWIDGET_ITEM_INFO_T g_memo_widget[] =
{
    {
        MMIWIDGET_MEMO_ID0, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID0, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,/*lint !e651*/
    	50, 
    	FALSE,
    },
    {
        MMIWIDGET_MEMO_ID0 + 1, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID1, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,//    75, 
    	50,//    50
    	FALSE,
    },
    {
        MMIWIDGET_MEMO_ID0 + 2, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID2, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,//    75, 
    	50,//    50
    	FALSE,
    },
    {
        MMIWIDGET_MEMO_ID0 + 3, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID3, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,//    75, 
    	50,//    50
    	FALSE,
    },
    {
        MMIWIDGET_MEMO_ID0 + 4, 
    	HandleWidgetMemoWinMsg, 
    	PNULL,
    	PNULL,
		PNULL,    	
    	WIDGET_MEMO_WIN_ID4, 
    	IMAGE_WIDGET_MEMO_SHORTCUT, 
    	IMAGE_WIDGET_NOTE_BG,
    	TXT_WIDGET_MEMO,
    	50,//    75, 
    	50,//    50
    	FALSE,
    }

};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
// 编辑窗口
LOCAL WINDOW_TABLE(MMIWIDGET_MEMO_EDIT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleMemoEditWinMsg),    
		WIN_ID( WIDGET_MEMO_EDIT_WIN_ID),
		WIN_TITLE(TXT_WIDGET_MEMO),
#ifdef MMI_PDA_SUPPORT
		CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIWIDGET_MEMO_FORM_CTRL_ID),
		CHILD_EDIT_TEXT_CTRL(TRUE, 0, MMIWIDGET_MEMO_EDITBOX_CTRL_ID, MMIWIDGET_MEMO_FORM_CTRL_ID),
		CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN, MMIWIDGET_MEMO_SOFTKEY_CTRL_ID, MMIWIDGET_MEMO_FORM_CTRL_ID),
#else
		CREATE_EDIT_TEXT_CTRL(MMIWIDGET_MEMO_EDIT_MAX_LEN,MMIWIDGET_MEMO_EDITBOX_CTRL_ID),
		WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
		END_WIN
};

/*****************************************************************************/
//  Description : 读memo文件
//  Global resource dependence : none
//  Author:
//  Note: 
//Lenth|Str|  
//  4  | n |
/*****************************************************************************/
LOCAL  BOOLEAN MMIWIDGET_Memo_ReadMemoFile(void)
{
    MMIFILE_HANDLE      file_handle = 0; 
#ifndef MMI_GRID_IDLE_SUPPORT	
    uint32              read_size  = 0;
	uint32              read  = 0;
    wchar *buffer = g_edit_str.wstr_ptr;
    uint32 bytes_to_read = MMIWIDGET_MEMO_EDIT_MAX_LEN*sizeof(wchar);
#else
    uint8               i = 0;
    uint32              read_size[MMIGRID_MEMO_WIDGET_NUM]  = {0};
    uint32              read[MMIGRID_MEMO_WIDGET_NUM]  = {0};
#endif

#ifndef MMI_GRID_IDLE_SUPPORT	
    if (PNULL == g_edit_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Memo_ReadMemoFile PNULL == g_edit_str.wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MEMO_324_112_2_18_3_8_13_201,(uint8*)"");
        return FALSE;
    }
#else
    if (PNULL == g_memo_edit_str[0].wstr_ptr || PNULL == g_memo_edit_str[1].wstr_ptr
    || PNULL == g_memo_edit_str[2].wstr_ptr || PNULL == g_memo_edit_str[3].wstr_ptr || PNULL == g_memo_edit_str[4].wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Memo_ReadMemoFile PNULL == g_memo_edit_str[i].wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MEMO_331_112_2_18_3_8_13_202,(uint8*)"");
        return FALSE;
    }
#endif
	
	if(!MMIAPIFMM_IsFileExist(fs_full_path, (uint16)MMIAPICOM_Wstrlen(fs_full_path)))
	{
       file_handle = MMIAPIFMM_CreateFile(fs_full_path, (SFS_MODE_READ|SFS_MODE_CREATE_NEW), NULL, NULL);  /*lint !e655*/
	}
	else
	{
		file_handle = MMIAPIFMM_CreateFile(fs_full_path, (SFS_MODE_READ|SFS_MODE_OPEN_EXISTING), NULL, NULL);  /*lint !e655*/
	}
	if(SFS_INVALID_HANDLE == file_handle)
	{
		return FALSE;
	}
	
#ifndef MMI_GRID_IDLE_SUPPORT
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, buffer,bytes_to_read, &read_size, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //包含空文件read_size为0也需要
    g_edit_str.wstr_len = read_size/sizeof(wchar);
#else
    for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &read_size[i],sizeof(uint32), &read[i], NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }

        if (read_size[i] > 0 && read_size[i] <= MMIWIDGET_MEMO_EDIT_MAX_LEN*sizeof(wchar))
        {
            if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, g_memo_edit_str[i].wstr_ptr,read_size[i], &read[i], NULL))
            {
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            }
        }
        g_memo_edit_str[i].wstr_len = read_size[i]/sizeof(wchar);
    }
#endif
    MMIAPIFMM_CloseFile(file_handle);	
    return TRUE;
}
/*****************************************************************************/
//  Description : 写memo文件
//  Global resource dependence : none
//  Author:
//  Note: 
//Lenth|Str|  
//  4  | n |
/*****************************************************************************/
LOCAL  BOOLEAN MMIWIDGET_Memo_WriteMemoFile(void)
{
    MMIFILE_HANDLE      file_handle = 0;  
    uint32              write_len = 0;
#ifdef MMI_GRID_IDLE_SUPPORT
    uint8  i = 0;
    uint32 bytes_to_write  = 0;
#else
    wchar *buffer = g_edit_str.wstr_ptr;
    uint32 bytes_to_write = g_edit_str.wstr_len*sizeof(wchar);
#endif

    SCI_TRACE_LOW("MMIWIDGET_MEMO WriteMemoFile create handle start = %d", SCI_GetTickCount());
    file_handle = MMIAPIFMM_CreateFile(fs_full_path, (SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS), NULL, NULL);   /*lint !e655*/
    SCI_TRACE_LOW("MMIWIDGET_MEMO WriteMemoFile create handle end = %d", SCI_GetTickCount());
    
    if(SFS_INVALID_HANDLE == file_handle)
    {
        return FALSE;
    }	
#ifndef MMI_GRID_IDLE_SUPPORT
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, buffer, bytes_to_write,&write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
#else
    
    SCI_TRACE_LOW("MMIWIDGET_MEMO WriteMemoFile write file start = %d", SCI_GetTickCount());
    for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
    {
        bytes_to_write = g_memo_edit_str[i].wstr_len*sizeof(wchar);
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &bytes_to_write, sizeof(uint32),&write_len, PNULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, g_memo_edit_str[i].wstr_ptr, bytes_to_write,&write_len, PNULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
    }
    SCI_TRACE_LOW("MMIWIDGET_MEMO WriteMemoFile write file end = %d", SCI_GetTickCount());
#endif
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;

}
/*****************************************************************************/
//  Description : 编辑窗口处理函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMemoEditWinMsg(
										MMI_WIN_ID_T        win_id, 
										MMI_MESSAGE_ID_E    msg_id, 
										DPARAM              param
										)
{
    MMI_RESULT_E              result   = MMI_RESULT_TRUE;
#ifndef MMI_GRID_IDLE_SUPPORT
    MMI_STRING_T              str_info = {0};
    MMI_STRING_T              str_t    = {0};
    MMI_STRING_T              edit_str_t    = {0};	
    edit_str_t.wstr_ptr = g_edit_str.wstr_ptr;
    edit_str_t.wstr_len = g_edit_str.wstr_len;
#else
    uint32                    cur_idx = (uint32)MMK_GetWinAddDataPtr(win_id);
	GUIFORM_CHILD_HEIGHT_T child_height = {0};
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GRID_IDLE_SUPPORT
        if((PNULL !=edit_str_t.wstr_ptr) && (edit_str_t.wstr_len != 0))	
        {
            GUIEDIT_SetString(MMIWIDGET_MEMO_EDITBOX_CTRL_ID, edit_str_t.wstr_ptr, edit_str_t.wstr_len);
        }
#else
		child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
        GUIFORM_SetChildHeight(MMIWIDGET_MEMO_FORM_CTRL_ID,MMIWIDGET_MEMO_EDITBOX_CTRL_ID,&child_height);
		GUIFORM_PermitChildBg(MMIWIDGET_MEMO_FORM_CTRL_ID,FALSE);
		GUIEDIT_SetStyle(MMIWIDGET_MEMO_EDITBOX_CTRL_ID,GUIEDIT_STYLE_MULTI);		
		GUIEDIT_PermitBorder(MMIWIDGET_MEMO_EDITBOX_CTRL_ID, FALSE);
		GUIEDIT_SetDividingLine(MMIWIDGET_MEMO_EDITBOX_CTRL_ID,1,MMI_DARK_GRAY_COLOR);
		GUIEDIT_SetTextMaxLen(MMIWIDGET_MEMO_EDITBOX_CTRL_ID,MMIWIDGET_MEMO_EDIT_MAX_LEN,MMIWIDGET_MEMO_EDIT_MAX_LEN);
        Set_EditRectString(cur_idx);
#endif
    
        MMK_SetAtvCtrl(win_id, MMIWIDGET_MEMO_EDITBOX_CTRL_ID);
        break;
		
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:    
#ifndef MMI_GRID_IDLE_SUPPORT 
        {    
            GUIEDIT_GetString(MMIWIDGET_MEMO_EDITBOX_CTRL_ID, &str_info);   
            if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
            {
                str_t.wstr_len = str_info.wstr_len; 
                str_t.wstr_ptr = str_info.wstr_ptr;
                SCI_MEMSET(g_edit_str.wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)));
                g_edit_str.wstr_len = MIN((str_t.wstr_len),MMIWIDGET_MEMO_EDIT_MAX_LEN) ;   
                SCI_MEMCPY(g_edit_str.wstr_ptr,str_t.wstr_ptr,(g_edit_str.wstr_len)*sizeof(wchar));
    
            }
            //删除所有的字符时，清空
            else
            {   
                SCI_MEMSET(g_edit_str.wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)));
                g_edit_str.wstr_len = 0;
            }
            MMIWIDGET_Memo_WriteMemoFile();            
            MMK_CloseWin(win_id);   
        }
#else
         Get_EditRectString(cur_idx);
         MMIWIDGET_Memo_WriteMemoFile();           
         MMK_CloseWin(win_id);  
#endif
        break;
        

#ifdef MMI_GRID_IDLE_SUPPORT     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
    {
        int32 src_id = ((MMI_NOTIFY_T*)param)->src_id;               
        switch(src_id)
        {
        case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
            {   
                Get_EditRectString(cur_idx);
                MMIWIDGET_Memo_WriteMemoFile();	                
                MMK_CloseWin(win_id);	
            }
            break;

        case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
    }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
	case MSG_CLOSE_WINDOW:
		break;
	
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Description : memo的窗口回调函数
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetMemoWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
	//GUI_POINT_T point   = {0};
#ifdef MMI_GRID_IDLE_SUPPORT
    uint8       cur_idx = GetCurMemoIdx(win_id);
    uint8       i  = 0;
    if(cur_idx >= MMIGRID_MEMO_WIDGET_NUM)
    {
        return MMI_RESULT_FALSE;
    }
#endif

    switch (msg_id)
    {     
	case MSG_OPEN_WINDOW:
#ifdef MMI_GRID_IDLE_SUPPORT
        for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
        {
    		if(PNULL ==  g_memo_edit_str[i].wstr_ptr)
    		{
    			g_memo_edit_str[i].wstr_ptr = (wchar *)SCI_ALLOC_APP((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar) + 2);
    			SCI_MEMSET(g_memo_edit_str[i].wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)) + 2);
                g_memo_edit_str[i].wstr_len = 0;
    		}
        }
#else
		if(PNULL ==  g_edit_str.wstr_ptr)
		{
			g_edit_str.wstr_ptr = (wchar *)SCI_ALLOC_APP((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar) + 2);
			SCI_MEMSET(g_edit_str.wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)) + 2);
            g_edit_str.wstr_len = 0;
		}
#endif
		MMIWIDGET_Memo_ReadMemoFile();
        //SCI_TRACE_LOW:"[MMIWIDGET_Memo]: HandleMemoEditWinMsg = before MMIWIDGET_Memo_ReadMemoFile! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MEMO_579_112_2_18_3_8_14_203,(uint8*)"");
		break;

	case MSG_WIDGET_TP_PRESS_UP:
#ifndef MMI_GRID_IDLE_SUPPORT 
        MMK_CreateWin((uint32*)MMIWIDGET_MEMO_EDIT_WIN_TAB, PNULL);
#else
        MMK_CreateWin((uint32*)MMIWIDGET_MEMO_EDIT_WIN_TAB, (ADD_DATA)cur_idx);
#endif
		break;		
		
	case MSG_FULL_PAINT:
#ifndef MMI_GRID_IDLE_SUPPORT         
        MMIWIDGET_Memo_DisplayMemoPanel(win_id, &g_edit_str);
#else
        MMIWIDGET_Memo_DisplayMemoPanel(win_id, g_memo_edit_str);
#endif
		break;

#ifdef MMI_GRID_IDLE_SUPPORT  
    case MSG_GET_FOCUS:
        break;
        
    case MSG_GRID_ADD_WIDGET:
        s_memo_status_full[cur_idx] = TRUE;
        break;
        
	case MSG_GRID_REMOVE_WIDGET:
        s_memo_status_full[cur_idx] = FALSE;
        Set_GlobalNull(cur_idx);
        //MMIWIDGET_Memo_WriteMemoFile();	
	    break;
#endif

	case MSG_CLOSE_WINDOW:
#ifdef MMI_GRID_IDLE_SUPPORT
        for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
        {
    		if(PNULL !=  g_memo_edit_str[i].wstr_ptr)
    		{
    			SCI_Free( g_memo_edit_str[i].wstr_ptr);
    			g_memo_edit_str[i].wstr_ptr = PNULL;
            }
            g_memo_edit_str[i].wstr_len = 0;
        }
#else
		if(PNULL !=  g_edit_str.wstr_ptr)
		{
			SCI_Free( g_edit_str.wstr_ptr);
			g_edit_str.wstr_ptr = PNULL;
		}
        g_edit_str.wstr_len = 0;
#endif
    	break;

	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : 获得memo字体
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Memo_GetMemoTextStyle(
										   GUISTR_STYLE_T *style_ptr,
										   GUI_FONT_T     font,
										   GUI_COLOR_T    font_color
										   )
{
    //SCI_ASSERT(PNULL != style_ptr);

    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Memo_GetMemoTextStyle style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MEMO_653_112_2_18_3_8_14_204,(uint8*)"");
        return;
    }
  	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
    style_ptr->line_space = MMIWIDGET_MEMO_LINE_SPACE;    
}
#ifdef MMI_GRID_IDLE_SUPPORT
/*****************************************************************************/
//  Description : 显示memo
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Memo_DisplayMemoPanel(
										   MMI_WIN_ID_T win_id,
										   MMI_STRING_T *str_ptr
										   )/*,
										   MMI_STRING_T *str1_ptr,
										   MMI_STRING_T *str2_ptr
										   )*/
{
    GUI_RECT_T win_rect = {0};
	GUI_RECT_T display_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    // UILAYER_HANDLE_T layer_handle = 0;
    GUI_POINT_T      point={0};
    GUISTR_STYLE_T text_style = {0};
    uint16     add_h = 0;
	uint16     add_w = 0;
	//uint8      i     = 0;
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;	
    uint32     cur_idx = GetCurMemoIdx(win_id);
    
    GUIRES_GetImgWidthHeight(&add_w , &add_h,IMAGE_WIDGET_MEMO_EDIT_ICON, win_id);
    MMIWIDGET_Memo_GetMemoTextStyle(&text_style, MMIWIDGET_MEMO_TEXT_FONT, MMIWIDGET_MEMO_TEXT_COLOR);	
    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
		
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
		
		GUIRES_DisplayImg(
		&point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_WIDGET_NOTE_BG,
		&lcd_dev_info
		);

		if(0 == str_ptr[cur_idx].wstr_len)
		{
			point.x = (win_rect.left + win_rect.right - add_w)/2 ;
			point.y = (win_rect.top + win_rect.bottom- add_h)/2 ;
			GUIRES_DisplayImg(
				&point,
				PNULL,
				PNULL,
				win_id,
				IMAGE_WIDGET_MEMO_EDIT_ICON,
				&lcd_dev_info
				);
		}
		else
		{
			display_rect = win_rect;
			display_rect.left += MMIWIDGET_MEMO_TEXT_LEFT ;
			display_rect.top += MMIWIDGET_MEMO_TEXT_TOP; 
            display_rect.right -=MMIWIDGET_MEMO_TEXT_RIGHT;
			
			GUISTR_DrawTextToLCDInRect(
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				&display_rect,
				&display_rect,
				&str_ptr[cur_idx],	   
				&text_style,
				text_state,
				GUISTR_TEXT_DIR_AUTO
				);
		}

    }
}
#else
/*****************************************************************************/
//  Description : 显示memo
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Memo_DisplayMemoPanel(
										   MMI_WIN_ID_T win_id,
										   MMI_STRING_T *str_ptr
										   )
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    // UILAYER_HANDLE_T layer_handle = 0;
    GUI_POINT_T      point={0};
    GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T     tempStr = {0};
	
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;	
    MMIWIDGET_Memo_GetMemoTextStyle(&text_style, MMIWIDGET_MEMO_TEXT_FONT, MMIWIDGET_MEMO_TEXT_COLOR);	
    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
		
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
		
		GUIRES_DisplayImg(
		&point,
		PNULL,
		PNULL,
		win_id,
		IMAGE_WIDGET_NOTE_BG,
		&lcd_dev_info
		);
        win_rect.left  += MMIWIDGET_MEMO_LEFT_GAP;
        win_rect.right -= MMIWIDGET_MEMO_RIGHT_GAP;
        win_rect.top  += MMIWIDGET_MEMO_TOP_GAP;
        win_rect.bottom -= MMIWIDGET_MEMO_BOTTOM_GAP;
        if(0 == str_ptr->wstr_len)
        {
            MMI_GetLabelTextByLang(TXT_WIDGET_EDIT_MEMO, &tempStr);
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                &win_rect,
                &win_rect,
                &tempStr,	   
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO
                );
        }
        else
        {
	           //display string
	           GUISTR_DrawTextToLCDInRect(
                   (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                   &win_rect,
                   &win_rect,
                   str_ptr,	   
                   &text_style,
                   text_state,
                   GUISTR_TEXT_DIR_AUTO
                   );
        }
    }
}
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
/*****************************************************************************/
//  Description : 获得编辑框区域
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void	Get_Rect(MMI_WIN_ID_T win_id,GUI_RECT_T *rect,DISPLAY_RECT_E rect_type)
{
	GUI_RECT_T win_rect = {0};
    MMK_GetWinRect(win_id, &win_rect);
	
	switch(rect_type)
	{
	case RECT_DISPLAY1:
		rect->left   = win_rect.left + MMIWIDGET_MEMO_LEFT_GAP - MMIWIDGET_MEMO_EDIT_ADJUST_GAP/2 - MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		rect->right  = win_rect.right - MMIWIDGET_MEMO_RIGHT_GAP - (((win_rect.right - win_rect.left)/3)*2) + MMIWIDGET_MEMO_EDIT_ADJUST_GAP/2; 
		rect->top    = win_rect.top + MMIWIDGET_MEMO_TOP_GAP - MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		rect->bottom = win_rect.bottom - MMIWIDGET_MEMO_BOTTOM_GAP + MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		break;
		
	case RECT_DISPLAY2:
		rect->left   = win_rect.left + MMIWIDGET_MEMO_LEFT_GAP + (win_rect.right - win_rect.left)/3 - MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		rect->right  = win_rect.right - MMIWIDGET_MEMO_RIGHT_GAP - ((win_rect.right - win_rect.left)/3) + MMIWIDGET_MEMO_EDIT_ADJUST_GAP/2; 
		rect->top    = win_rect.top + MMIWIDGET_MEMO_TOP_GAP - MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		rect->bottom = win_rect.bottom - MMIWIDGET_MEMO_BOTTOM_GAP + MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		break;

	case RECT_DISPLAY3:
		rect->left   = win_rect.left + MMIWIDGET_MEMO_LEFT_GAP + ((win_rect.right - win_rect.left)/3)*2 - MMIWIDGET_MEMO_EDIT_ADJUST_GAP*2;
		rect->right  = win_rect.right - MMIWIDGET_MEMO_EDIT_ADJUST_GAP*2; 
		rect->top    = win_rect.top + MMIWIDGET_MEMO_TOP_GAP - MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		rect->bottom = win_rect.bottom - MMIWIDGET_MEMO_BOTTOM_GAP + MMIWIDGET_MEMO_EDIT_ADJUST_GAP;
		break;

	default:
		break;
	}
}
/*****************************************************************************/
//  Description : 获得编辑框序号
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Get_RectIndex(MMI_WIN_ID_T  win_id,GUI_POINT_T point, uint32 *index_ptr)
{
	uint16 i = 0;
	GUI_RECT_T   rect = {0};
	for (; i <= 2; i++)
	{
		Get_Rect(win_id,&rect,(DISPLAY_RECT_E)i);
        if (GUI_PointIsInRect(point, rect))
        {
			*index_ptr = i;
            return TRUE;
        }
	}
	return FALSE;
}
/*****************************************************************************/
//  Description : 设置编辑框内容
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Set_EditRectString(uint32  cur_index)
{
	MMI_STRING_T              edit_str_t  = {0};
	//detail_win_param += 1;
	edit_str_t.wstr_ptr = g_memo_edit_str[cur_index].wstr_ptr;
	edit_str_t.wstr_len = g_memo_edit_str[cur_index].wstr_len;
	GUIEDIT_SetString(MMIWIDGET_MEMO_EDITBOX_CTRL_ID, edit_str_t.wstr_ptr, edit_str_t.wstr_len);
}
/*****************************************************************************/
//  Description : 获得编辑框内容
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Get_EditRectString(uint32  cur_index)
{
	MMI_STRING_T      str_info         = {0};
    MMI_STRING_T      str_t            = {0};
	
	GUIEDIT_GetString(MMIWIDGET_MEMO_EDITBOX_CTRL_ID, &str_info);

	if (str_info.wstr_len > 0 && PNULL != str_info.wstr_ptr)
	{
		str_t.wstr_len = str_info.wstr_len; 
		str_t.wstr_ptr = str_info.wstr_ptr;
		Set_GlobalString(str_t,&g_memo_edit_str[cur_index]);
	}
	else
	{   
		Set_GlobalNull(cur_index);
	}
}
/*****************************************************************************/
//  Description : 用编辑框内容设置全局变量
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Set_GlobalString(MMI_STRING_T str_t,MMI_STRING_T *g_str_t)
{
    if (PNULL != g_str_t->wstr_ptr)
    {
	    SCI_MEMSET(g_str_t->wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)));
	    g_str_t->wstr_len = MIN((str_t.wstr_len),MMIWIDGET_MEMO_EDIT_MAX_LEN) ;	
	    SCI_MEMCPY(g_str_t->wstr_ptr,str_t.wstr_ptr,(g_str_t->wstr_len)*sizeof(wchar));
    }
}
/*****************************************************************************/
//  Description : 编辑框为空时，清空全局变量
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void Set_GlobalNull(uint32 edit_index)
{
	SCI_MEMSET(g_memo_edit_str[edit_index].wstr_ptr, 0x00, ((MMIWIDGET_MEMO_EDIT_MAX_LEN )*sizeof(wchar)));
	g_memo_edit_str[edit_index].wstr_len = 0;
}
#endif
/*****************************************************************************/
//  Description : 是不是有可以添加的记事本
//  Global resource dependence : none
//  Author:
//  Note: TRUE表示该节点已经被添加过了，FALSE表示可以被添加
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_IsMemoStatusFull(uint32 index)
{
    return s_memo_status_full[index];
}

/*****************************************************************************/
// Description :GetCurMemoIdx
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
LOCAL uint32 GetCurMemoIdx(MMI_WIN_ID_T win_id)
{
    uint32 idx = 0;
    MMIWIDGET_ITEM_T *item_ptr = PNULL;

    item_ptr = (MMIWIDGET_ITEM_T*)MMK_GetWinAddDataPtr(win_id);
    if(PNULL == item_ptr)
    {
        //SCI_TRACE_LOW:"GetCurMemoIdx PNULL == item_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MEMO_947_112_2_18_3_8_15_205,(uint8*)"");
        return 0;
    }
    
    idx = item_ptr->item_info.widget_id - MMIWIDGET_MEMO_ID0;

    return idx;
}
/*****************************************************************************/
// Description :MMIGRID_MEMO_IsNotesFull
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIGRID_MEMO_IsNotesFull(void)

{
    uint8 i = 0;
    uint8 num = 0;
    BOOLEAN result = FALSE;
    
    for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
    {
        if(s_memo_status_full[i])
        {
            num++;
        }
    }
    if(MMIGRID_MEMO_WIDGET_NUM == num)
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
// Description :reset memo settings to initial
// Global resource dependence :
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_MEMO_ResetFactorySettings(void)
{
#ifdef MMI_GRID_IDLE_SUPPORT
    int32 i = 0;
    SCI_MEMSET(s_memo_status_full, 0 ,  sizeof(s_memo_status_full));
    for(i = 0; i < MMIGRID_MEMO_WIDGET_NUM; i++)
    {
        if(PNULL !=  g_memo_edit_str[i].wstr_ptr)
        {
            SCI_Free( g_memo_edit_str[i].wstr_ptr);
            g_memo_edit_str[i].wstr_ptr = PNULL;
        }
        g_memo_edit_str[i].wstr_len = 0;
    }
#endif
}

#endif
#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527

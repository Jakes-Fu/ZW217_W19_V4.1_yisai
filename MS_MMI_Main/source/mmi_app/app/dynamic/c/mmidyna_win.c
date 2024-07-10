/****************************************************************************
** File Name:      mmidynamic_win.c                            
** Author:                                                                  
** Date:            2008/08/20
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:    This file is used to create windows of game
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 20/08/2006         peng         Create
**  
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#include "std_header.h"

#include "mmi_app_dynamic_trc.h"
#ifdef DYNAMIC_MODULE_SUPPORT

#include "window_parse.h"
#include "guimenu.h"
#include "mmk_app.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmk_modinfo.h"
#include "cafctrllist.h"
#include "mmidyna_id.h"
#include "mmidyna_text.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "mmidyna_export.h.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilabel.h"
#include "mmi_link.h"
#include "mmi_appmsg.h"
#include "guires.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"
#include "guilistbox.h"
#include "mmk_shell.h"
#include "guitext.h"
#include "mmi_mainmenu_export.h"
#include "app_tcp_if.h"
#include "mmi_position.h"
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/       
#define DYNAMIC_APP_LIST_MAX 50

#define MMI_SZIP_FILE_SIZE_MAX						     (10 * 1024 * 1024)

/*location of detail label*/
#define MMIDYNA_APP_DETAIL_LEFT                          105

#define MMIDYNA_APP_DETAIL_NAME_TOP                      (MMI_TITLE_HEIGHT + 6)
#define MMIDYNA_APP_DETAIL_NAME_BOTTOM                   (MMI_DEFAULT_FONT_HEIGHT+2+MMIDYNA_APP_DETAIL_NAME_TOP)

#define MMIDYNA_APP_DETAIL_VERSION_TOP                   (5 + MMIDYNA_APP_DETAIL_NAME_BOTTOM)
#define MMIDYNA_APP_DETAIL_VERSION_BOTTOM                (MMI_DEFAULT_FONT_HEIGHT+2+MMIDYNA_APP_DETAIL_VERSION_TOP)

#define MMIDYNA_APP_DETAIL_DATE_TOP                      (5+MMIDYNA_APP_DETAIL_VERSION_BOTTOM)
#define MMIDYNA_APP_DETAIL_DATE_BOTTOM                   (MMI_DEFAULT_FONT_HEIGHT+2+MMIDYNA_APP_DETAIL_DATE_TOP)

#define MMIDYNA_APP_DETAIL_MEMORY_TOP                    (5+MMIDYNA_APP_DETAIL_DATE_BOTTOM)
#define MMIDYNA_APP_DETAIL_MEMORY_BOTTOM                 (MMI_DEFAULT_FONT_HEIGHT+2+MMIDYNA_APP_DETAIL_MEMORY_TOP)

#define MMIDYNA_APP_DETAIL_LOCATION_TOP                  (5+MMIDYNA_APP_DETAIL_MEMORY_BOTTOM)
#define MMIDYNA_APP_DETAIL_LOCATION_BOTTOM               (MMI_DEFAULT_FONT_HEIGHT+2+MMIDYNA_APP_DETAIL_LOCATION_TOP)


typedef enum
{
    MMIDYNA_LIST_APPEND_ITEM,
    MMIDYNA_LIST_SET_ITEM_DATA,
    MMIDYNA_LIST_SET_ITEM_CONTECT
}MMIDYNA_LIST_APPEND_TYPE;

typedef enum
{
     MMIFMM_SZIP_NO_ERROR = 0,                
	 MMIFMM_SZIP_HAS_EXIT, 
	 MMIFMM_SZIP_ERROR, 

	 MMIFMM_SZIP_MAX
} MMIFMM_SZIP_TYPE_E;

#define  CAF_VERSION_MAX_LEN 25

// 文件详情数据
typedef struct mmidyna_detail_data_tag
{
    uint32			create_date;	/*创建日期*/
    uint32			file_size;	// 文件大小             
    wchar	 		version[CAF_VERSION_MAX_LEN + 1];	// 版本    
    wchar			filename[CAF_FILENAME_MAX_LEN+1];// 文件名
	CAF_MOD_STORE_TYPE_E store;	//存贮位置
} MMIDYNA_DETAIL_DATA_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const uint16* s_full_file_name = PNULL;
LOCAL uint16 *s_new_full_path = NULL;
//*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 小程序选项菜单处理程序
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaMyAppListOpt(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

//*****************************************************************************/
// 	Description : 游戏主菜单
//	Global resource dependence : 
//  Author:ycd 01
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaMainListWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
/*****************************************************************************/
// 	Description : 详情窗口处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaAppDetailWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );

/*****************************************************************************/
// 	Description : set detail
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL void  SetCAFDetail(
                         MMIDYNA_DETAIL_DATA_T *caf_detail
                         );

/*****************************************************************************/
// 	Description : query handle message
//	Global resource dependence : 
//  Author:peng.chen
//	Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E QueryDelAppHandleMsg(
                                               CAF_HANDLE_T win_handle, 
                                               CAF_MESSAGE_ID_E msg_id, 
                                               void* param
                                               );

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ApendListItem(
                                  MMI_HANDLE_T win_handle,
                                  MMI_CTRL_ID_T ctrl_id
                                  );


/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  LoadListItemInfo(
                                     MMI_HANDLE_T win_handle, 
                                     MMI_CTRL_ID_T ctrl_id,
                                     uint32 index,
                                     uint32 index_icon,
                                     uint32 guid,
                                     MMIDYNA_LIST_APPEND_TYPE type
                                     );

/*****************************************************************************/
// 	Description : handle unpack query win
//	Global resource dependence : none
//  Author: james.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDynaUnpackSzipFileQueryWin(
                                                   MMI_WIN_ID_T 	win_id, 
                                                   MMI_MESSAGE_ID_E	msg_id, 
                                                   DPARAM           param
                                                   );



/*****************************************************************************/
// 	Description : unpack file
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMIFMM_SZIP_TYPE_E UnpackSzipDir(	  
                                       MMIFILE_DEVICE_E store_device, 
                                       const uint16* full_path_name,
                                       const uint16 *file_name,      //without suffix
                                       uint16        file_name_len,   //without suffix
                                       uint16		*new_full_path, //out
                                       uint16		new_path_len
                                       );

/*****************************************************************************/
//  Description : unpack szip
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
LOCAL BOOLEAN InstallSzipFile(
                              MMIFILE_DEVICE_E store_device
                              );

/*****************************************************************************/
// 	Description : handle overwrite query win
//	Global resource dependence : none
//  Author: james.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDynaOverwriteQueryWin(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E	msg_id, 
                                                   DPARAM           param
                                                   );
/*****************************************************************************/
// 	Description : handle open start app waiter window
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInstallWaitWin(
										 MMI_WIN_ID_T       win_id, 	
										 MMI_MESSAGE_ID_E   msg_id, 
										 DPARAM             param
										 );
/*****************************************************************************/
// 	Description : handle open start app waiter window
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UnpackSzipFile( 
                             const uint16* full_path_name,				 
                             const uint16* new_full_path, 
                             uint16 new_path_buf_len//wchar len
                             );

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsValidInstallApp(
                                      const uint16 *file_name
                                      );

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleDynaStoreType(
                                        MMI_WIN_ID_T         win_id, 	
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM               param
                                        );

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL void   SetRadioStoreTypeItem(
                                   MMIFILE_DEVICE_E      store_type_e
                                   );

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL void  AppendListByTxtPtr(
                               MMI_CTRL_ID_T        ctrl_id,
                               MMI_STRING_T			*str_ptr, 
                               MMI_TEXT_ID_T        left_softkey_id,
                               MMI_TEXT_ID_T        middle_softkey_id,
                               MMI_TEXT_ID_T        right_softkey_id,
                               GUIITEM_STYLE_E      item_style
                               );

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
/*动态小程序的列表窗口*/
WINDOW_TABLE( MMIDYNA_MAIN_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDynaMainListWindow ),    
    WIN_ID( MMIDYNA_MAIN_LIST_WIN_ID ),    
    //WIN_TITLE(TXT_MY_APPLICATION),
    WIN_TITLE(TXT_MAINMENU_FAVORITE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID ),
    END_WIN
};
/*Detail window*/
WINDOW_TABLE(MMIDYNA_APP_DETAIL_WIN_TAB) =
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDynaAppDetailWinMsg ), 
    WIN_ID( MMIDYNA_APP_DETAIL_WIN_ID ), 
    WIN_TITLE(STXT_DETAIL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_NAME_TITLE_LABEL_CTRL_ID), //name title
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_NAME_LABEL_CTRL_ID),       //name

	CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_VERSION_TITLE_LABEL_CTRL_ID), //version title
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_VERSION_LABEL_CTRL_ID),       //version	

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_DATE_TITLE_LABEL_CTRL_ID), //data title
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_DATE_LABEL_CTRL_ID),       //data             
		
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_MEMORY_TITLE_LABEL_CTRL_ID), //memory title
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_MEMORY_LABEL_CTRL_ID),       //memory

    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_LOCATION_TITLE_LABEL_CTRL_ID), //location title
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID),       //location

    END_WIN
};

/*Radio window for install */
WINDOW_TABLE( MMIDYNA_INSTALL_STORE_TYPE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleDynaStoreType ),    
    WIN_ID( MMIDYNA_INSTALL_STORE_TYPE_WIN_ID ),
    WIN_TITLE(TXT_FMM_STORE_TYPE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIDYNA_INSTALL_STORE_LIST_CTRL_ID),
	END_WIN
};

WINDOW_TABLE( MMIDYNA_MYAPP_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDynaMyAppListOpt),
    WIN_ID(MMIDYNA_MYAPP_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_DYNA_MYAPP_OPT,MMIDYNA_MYAPP_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

/*****************************************************************************/
// 	Description : initialize DYNA module
//	Global resource dependence : none
//  Author: peng.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_InitModule(void)
{
    //register menu
    MMIDYNA_RegMenuGroup();
    MMIDYNA_RegWinIdNameArr();
}

/*****************************************************************************/
// 	Description :  进入小程序列表窗口
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_OpenMainListWindow(void)
{
    MMK_CreateWin((uint32 *)MMIDYNA_MAIN_LIST_WIN_TAB, PNULL);
}

/*****************************************************************************/
// 	Description :  安装应用
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDYNA_InstallApp(
                               const uint16* full_file_name
                               )
{
    uint32 title_id = 0;

    //SCI_ASSERT( PNULL != full_file_name ); /*assert verified*/
    if(PNULL == full_file_name)
    {
       return;
    }

    s_full_file_name = full_file_name;

    /*验证程序是否已经签名*/
    if(CAF_RESULT_FAILED == IsValidInstallApp( full_file_name ) )
    {
        /*安装程序未通过验证，提示用户是否安装*/
        title_id = TXT_FMM_APP_NOT_VALID;
    }
    else
    {
        title_id = TXT_COM_LOCALINSTALL;
    }
    
    MMIPUB_OpenQueryWinByTextId( title_id,IMAGE_PUBWIN_QUERY, PNULL,(MMIPUB_HANDLE_FUNC) HandleDynaUnpackSzipFileQueryWin );
    
}
/*****************************************************************************/
// 	Description : to append the info to list_box
//	Global resource dependence : 
//  Author:peng.chen
//	Note: 
/*****************************************************************************/
LOCAL void AppendForMod(
                        MMI_CTRL_ID_T       ctrl_id,	
                        MMI_OBJECT_T        *text_id,
                        MMI_OBJECT_T	 	*icon_id,
                        uint32				user_data,
                        uint32              item_index,
                        uint16              item_content_index,
                        MMIDYNA_LIST_APPEND_TYPE              type
                        )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
	uint16	icon_len = 0;
    GUIANIM_DATA_INFO_T anim_data = {0};
	
	item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;
	item_t.item_data_ptr = &item_data;
	item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

	/*icon item*/
	if(NULL != icon_id)
	{
		icon_len = icon_id->content.wstr_len * sizeof(uint16);

        item_data.item_content[0].item_data.anim_data_ptr = &anim_data;

        anim_data.data_size = icon_len;//icon_id->content.wstr_len;
		anim_data.is_bitmap = FALSE;
		anim_data.data_ptr = (uint8*)icon_id->content.wstr_ptr;
	}
	
	if (NULL != text_id)
	{
		item_data.item_content[1].item_data.text_buffer.wstr_len = text_id->content.wstr_len;
		//item_data.item_content[1].item_data.text_buffer.is_ucs2 = text_id->content.is_ucs2;
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_id->content.wstr_ptr;

//         MMIAPICOM_Wstrncpy(item_data.item_content[1].item_data.text_buffer.wstr, 
//             text_id->content.wstr_ptr, 
//             item_data.item_content[1].item_data.text_buffer.wstr_len );
	}
	
	item_t.user_data = user_data;	

    if(type == MMIDYNA_LIST_APPEND_ITEM)
    {
	    GUILIST_AppendItem(ctrl_id, &item_t );    
    }
    else if (type == MMIDYNA_LIST_SET_ITEM_DATA)
    {
        GUILIST_SetItemData( ctrl_id, &item_data, item_index );
    }
    else if (type == MMIDYNA_LIST_SET_ITEM_CONTECT)
    {
        GUILIST_SetItemContent( ctrl_id, &item_data.item_content[item_content_index], item_index, item_content_index );
    }
}

/*****************************************************************************/
// 	Description : 小程序选项菜单处理程序
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaMyAppListOpt(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
	MMI_RESULT_E	result = MMI_RESULT_TRUE;
	MMI_MENU_GROUP_ID_T     group_id = 0;
    MMI_MENU_ID_T		    menu_id = 0;
    MMI_CTRL_ID_T           ctrl_id = MMIDYNA_MYAPP_LIST_OPT_CTRL_ID;

	//SCI_TRACE_LOW:"MMIFDYNA: HandleDynaMyAppListOpt, win_id =%d, msg_id = %d."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDYNA_WIN_473_112_2_18_2_15_3_0,(uint8*)"dd", win_id, msg_id);
	switch(msg_id)
    {
		case MSG_OPEN_WINDOW:
			MMK_SetAtvCtrl(win_id, MMIDYNA_MYAPP_LIST_OPT_CTRL_ID);						
			break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
			GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
			switch (menu_id)
			{
        case ID_DYNA_OPT_DETAIL:
            {
                MMI_OBJECT_T *obj_ptr = NULL;
                MMI_MODINFO_NODE_T * modinfo_ptr = NULL;
                MMIDYNA_DETAIL_DATA_T *detail = NULL;
                uint32 guid_item = 0;
//                CAF_RESULT_E isValid = CAF_RESULT_SUCCEEDED;
                uint16 item_cur = GUILIST_GetCurItemIndex(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID);
                
                if(FALSE == GUILIST_GetItemData(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, item_cur, &guid_item))
                {
                    return MMI_RESULT_FALSE;
                }
                modinfo_ptr = MMK_SearchModInfo(guid_item, MMI_QUERY_BY_GUID);
                if (NULL != modinfo_ptr)
                {
                    
                    detail = (MMIDYNA_DETAIL_DATA_T *)SCI_ALLOCA(sizeof(MMIDYNA_DETAIL_DATA_T));

					if(PNULL == detail)
                    {
                       return MMI_RESULT_FALSE;
                    }
                    
                    SCI_MEMSET(detail, 0, sizeof(MMIDYNA_DETAIL_DATA_T));
                    detail->create_date = modinfo_ptr->module_info.create_time;
                    detail->file_size = modinfo_ptr->module_info.size;
                    
                    /*get cif str data*/
                    obj_ptr = MMK_SearchObject(modinfo_ptr->module_info.cif_str_data.text_list_ptr, 
                        ((CAF_APP_INFO_T*)(modinfo_ptr->module_info.class_info_ptr->data))->text_id);

                    MMI_WSTRNCPY( detail->filename, 
                        CAF_FILENAME_MAX_LEN,
                        obj_ptr->content.wstr_ptr, 
                        obj_ptr->content.wstr_len,
                        obj_ptr->content.wstr_len );

//                     SCI_MEMCPY(detail->filename, 
//                         obj_ptr->content.str_ptr, 
//                         MIN(CAF_FILENAME_MAX_LEN, obj_ptr->content.length));

                    detail->store = modinfo_ptr->module_info.store;
                    if (0 < modinfo_ptr->module_info.version_data.wstr_len)
                    {
                        MMI_WSTRNCPY( detail->version,
                            CAF_VERSION_MAX_LEN,
                            modinfo_ptr->module_info.version_data.wstr_ptr, 
                            modinfo_ptr->module_info.version_data.wstr_len,
                            modinfo_ptr->module_info.version_data.wstr_len
                            );

//                         SCI_MEMCPY(	detail->version,
//                             modinfo_ptr->module_info.version_data.str_ptr, 
//                             modinfo_ptr->module_info.version_data.length);
                        
                    }						
                    MMK_CreateWin( (uint32*)MMIDYNA_APP_DETAIL_WIN_TAB, (ADD_DATA)detail);
                }
                
                break;
            }
            
        case ID_DYNA_OPT_DELAPP:
				{
					MMIPUB_OpenQueryWinByTextId(TXT_DYNA_DEL_APP, 
												IMAGE_PUBWIN_QUERY, 
												PNULL, QueryDelAppHandleMsg);
					break;
				}
			}
			break;

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin( win_id);
			break;		

		default:
			result = MMI_RESULT_FALSE;
			break;
	}

    return (result);
}

/*****************************************************************************/
// 	Description : query handle message
//	Global resource dependence : 
//  Author:peng.chen
//	Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E QueryDelAppHandleMsg(
                                               CAF_HANDLE_T win_handle, 
                                               CAF_MESSAGE_ID_E msg_id, 
                                               void* param
                                               )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
		{	
			uint32 guid_item = 0;					
			uint16 item_cur = GUILIST_GetCurItemIndex(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID);
			MMI_MODINFO_NODE_T * modinfo_ptr = NULL;
			wchar *file_path = NULL;
			uint16 path_len = 0;
			
			MMIPUB_CloseQuerytWin(PNULL);				

            if(FALSE == GUILIST_GetItemData(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, item_cur, &guid_item))
			{
				return MMI_RESULT_FALSE;
			}
			modinfo_ptr = MMK_SearchModInfo(guid_item, MMI_QUERY_BY_GUID);

			file_path = SCI_ALLOC_APP( ( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar) );
			SCI_MEMSET( file_path, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );

            SCI_ASSERT(PNULL != modinfo_ptr); /*assert verified*/
			if (FALSE == MMIAPICOM_GetFilePath(modinfo_ptr->module_info.dll_file_name, 
   											   MMIAPICOM_Wstrlen(modinfo_ptr->module_info.dll_file_name),
											   (uint16 *)file_path,
											   &path_len))
			{
				SCI_FREE(file_path);
				break;
			}
			/*delete all*/
			MMIAPICOM_DelDirAll((uint16 *)file_path, CAF_FILENAME_MAX_LEN);

			SCI_FREE(file_path);
			/*update mod info*/
			MMK_LoadDynamicModInfo();
			GUILIST_RemoveItem(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, item_cur);
			MMIPUB_OpenAlertSuccessWin(TXT_DELETED);//ID TXT_DC_NAMEINVALID

		}
		break;

	case MSG_APP_CANCEL:
		MMIPUB_CloseQuerytWin(PNULL);
		break;
		
	default:
        result = MMIPUB_HandleQueryWinMsg(win_handle,msg_id,param);
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  ApendListItem(
                                  MMI_HANDLE_T win_handle,
                                  MMI_CTRL_ID_T ctrl_id
                                  )
{
	MMI_MODINFO_NODE_T *p_mod = NULL;
//	MMI_OBJECT_T* obj_ptr_txt = PNULL;
//	MMI_OBJECT_T* obj_ptr_ico = PNULL;
    uint16 number = 0;
	MMI_MODINFO_LIST_T *modinfo_ptr = MMK_GetDynModInfoList();

	//SCI_PASSERT(modinfo_ptr, ("modinfo_ptr == NULL")); /*assert verified*/
    if(PNULL == modinfo_ptr)
    {
        return FALSE;
    }
	
	p_mod = modinfo_ptr->modinfo_list_ptr;
    
    // 查询动态模块中游戏的个数
    while (NULL != p_mod)
    {
        MMI_LINK_NODE_T *node_ptr = p_mod->module_info.class_info_ptr;
        
        if (node_ptr != PNULL)
        {
            do {
                if(CAF_APPLET_APPLICATION == ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->type)
                {
                    /*It's application, should be added in the list*/
                    number++;
                }
                node_ptr = node_ptr->next_ptr;
                
            } while(node_ptr != p_mod->module_info.class_info_ptr);
        }
        p_mod = p_mod->next_ptr;
    }

    if (number == 0)
    {
        return MMI_RESULT_FALSE;
    }

    p_mod = modinfo_ptr->modinfo_list_ptr;

	GUILIST_SetMaxItem( ctrl_id, number, TRUE );
    
	while (NULL != p_mod)
	{
        int32 i = 0;
        MMI_LINK_NODE_T *node_ptr = p_mod->module_info.class_info_ptr;

        if (node_ptr != PNULL)
        {
            do {
                if(CAF_APPLET_APPLICATION == ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->type)
				{
					/*It's application, should be added in the list*/
                    //obj_ptr_txt = MMK_SearchObject(p_mod->module_info.cif_str_data.text_list_ptr, ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->text_id);
                    AppendForMod(ctrl_id, PNULL, PNULL, 
                        ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->guid, i, 0, MMIDYNA_LIST_APPEND_ITEM);
                    i++;
				}
                node_ptr = node_ptr->next_ptr;
                
            } while(node_ptr != p_mod->module_info.class_info_ptr);
        }

		p_mod = p_mod->next_ptr;
	
	}

    return TRUE;
}

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  LoadListItemInfo(
                                     MMI_HANDLE_T win_handle,
                                     MMI_CTRL_ID_T       ctrl_id,
                                     uint32 index,
                                     uint32 index_icon,
                                     uint32 guid,
                                     MMIDYNA_LIST_APPEND_TYPE type
                                     )
{
	MMI_MODINFO_NODE_T *p_mod = NULL;
	MMI_OBJECT_T* obj_ptr_txt = PNULL;
	MMI_OBJECT_T* obj_ptr_ico = PNULL;
    uint16 number = 0;
	MMI_MODINFO_LIST_T *modinfo_ptr = MMK_GetDynModInfoList();

	//SCI_PASSERT(modinfo_ptr, ("modinfo_ptr == NULL"));	/*assert verified*/
    if(PNULL == modinfo_ptr)
    {
        return FALSE;
    }
	
	p_mod = modinfo_ptr->modinfo_list_ptr;
    
	while (NULL != p_mod)
	{
        MMI_LINK_NODE_T *node_ptr = p_mod->module_info.class_info_ptr;

        if (node_ptr != PNULL)
        {
            do {
                if(CAF_APPLET_APPLICATION == ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->type)
				{
					/*It's application, should be added in the list*/
                    if (guid == ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->guid)
                    {
                        obj_ptr_txt = MMK_SearchObject(p_mod->module_info.cif_str_data.text_list_ptr, ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->text_id);

                        obj_ptr_ico = MMK_SearchObject(p_mod->module_info.cif_obj_data.text_list_ptr, ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->icon_id);
                        AppendForMod(ctrl_id, obj_ptr_txt, obj_ptr_ico, 
                            ((CAF_APP_INFO_T*)(p_mod->module_info.class_info_ptr->data))->guid, index, index_icon, type);
                        
                        //return, break不能退出两个循环
                        return TRUE;
                    }
				}
                node_ptr = node_ptr->next_ptr;
                
            } while(node_ptr != p_mod->module_info.class_info_ptr);
        }

		p_mod = p_mod->next_ptr;
	
	}

    return TRUE;
}

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaMainListWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
//	uint32          group_id = 0; 
//	uint32          menu_id  = 0;
	MMI_RESULT_E    recode   = MMI_RESULT_TRUE;
	MMI_HANDLE_T	win_handle = MMK_ConvertIdToHandle(win_id);
    MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin(win_handle,MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID);
    const GUILIST_ITEM_T* item_ptr = PNULL;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

        ApendListItem(win_handle, ctrl_handle);
        
        ISHELL_SetActiveCtrl( ctrl_handle );
        //ISHELL_SetWinSoftkeyId( win_handle, STXT_OPTION, 0, STXT_RETURN, FALSE);  
        break;

    case MSG_FULL_PAINT:
        ISHELL_SetWinSoftkeyId( win_handle , STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE );
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            //SCI_ASSERT( PNULL != need_item_data_ptr ); /*assert verified*/
			if(PNULL == need_item_data_ptr)
			{
			   return MMI_RESULT_FALSE;
			}

            item_ptr = GUILIST_GetItemPtrByIndex(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, need_item_data_ptr->item_index );
            //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
			if(PNULL == item_ptr)
			{
			   return MMI_RESULT_FALSE;
			}

            LoadListItemInfo(win_handle, ctrl_handle,
                need_item_data_ptr->item_index, 0, item_ptr->user_data, MMIDYNA_LIST_SET_ITEM_DATA);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
            GUILIST_NEED_ITEM_CONTENT_T *need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;

            //SCI_ASSERT( PNULL != need_item_content_ptr ); /*assert verified*/
			if(PNULL == need_item_content_ptr)
			{
			   return MMI_RESULT_FALSE;
			}
            item_ptr = GUILIST_GetItemPtrByIndex(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, need_item_content_ptr->item_index);
            //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
			if(PNULL == item_ptr)
			{
			   return MMI_RESULT_FALSE;
			}
            LoadListItemInfo(win_handle, ctrl_handle,
                need_item_content_ptr->item_index, need_item_content_ptr->item_content_index, item_ptr->user_data, MMIDYNA_LIST_SET_ITEM_CONTECT);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:	
    case MSG_APP_WEB:
        {	/*start app*/
            uint32 guid_item = 0;
//            CAF_RESULT_E isValid;
            
            CAF_APPLET_START_T start ={0};
            uint16 item_cur = GUILIST_GetCurItemIndex(MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID);
            
            if(FALSE == GUILIST_GetItemData(ctrl_handle, item_cur, &guid_item))
            {
                return MMI_RESULT_FALSE;
            }
            /*start app */			
            start.guid = guid_item;
            ISHELL_StartApplet(&start);
        }
        
        break;   
        
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIDYNA_MYAPP_LIST_OPT_WIN_TAB,PNULL);
        break;		
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_handle);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}


/*****************************************************************************/
// 	Description : set detail
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL void  SetCAFDetail(
                         MMIDYNA_DETAIL_DATA_T *caf_detail
                         )
{
    uint8        temp_str[30] = {0};
	MMI_STRING_T str = {0};
	MMI_TM_T	time = {0};

	/*set title text*/
	GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_NAME_TITLE_LABEL_CTRL_ID,		TXT_COMMON_NAME,	FALSE);
    GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_VERSION_TITLE_LABEL_CTRL_ID,	TXT_COMM_VERSION_1,	FALSE);
    GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_DATE_TITLE_LABEL_CTRL_ID,		TXT_DYNA_DETAIL_CREATE_DATE,	FALSE);
    GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_MEMORY_TITLE_LABEL_CTRL_ID,		TXT_FILE_DETAIL_SIZE,	FALSE);
	GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_LOCATION_TITLE_LABEL_CTRL_ID,	TXT_DYNA_DETAIL_LOCATION,FALSE);

	str.wstr_ptr = SCI_ALLOC_APP( ( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar) );
	SCI_MEMSET( str.wstr_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar) ));

	str.wstr_len = MMIAPICOM_Wstrlen( caf_detail->filename );

    MMIAPICOM_Wstrncpy( str.wstr_ptr, caf_detail->filename, str.wstr_len );

	//str.is_ucs2 = TRUE;
	GUILABEL_SetText(MMIDYNA_APP_DETAIL_NAME_LABEL_CTRL_ID, &str, FALSE);

	/*set version*/	
	SCI_MEMSET( str.wstr_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );

	str.wstr_len = MMIAPICOM_Wstrlen( caf_detail->version );

	if( str.wstr_len > 0 )
	{
        MMIAPICOM_Wstrncpy( str.wstr_ptr, caf_detail->version, str.wstr_len );

		GUILABEL_SetText(MMIDYNA_APP_DETAIL_VERSION_LABEL_CTRL_ID, &str, FALSE);
	}	

	/*set file size*/
	SCI_MEMSET( str.wstr_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );
	sprintf( temp_str, "%ld.%03ldK", (caf_detail->file_size /1024), ((caf_detail->file_size %1024)*1000/1024));  
    str.wstr_len = strlen( temp_str );
    MMI_STRNTOWSTR( str.wstr_ptr, CAF_FILENAME_MAX_LEN, temp_str, str.wstr_len, str.wstr_len );
    //str.is_ucs2 = FALSE;
    GUILABEL_SetText(MMIDYNA_APP_DETAIL_MEMORY_LABEL_CTRL_ID, &str, FALSE);

	/*set create date*/
	time = MMIAPICOM_Second2Tm(caf_detail->create_date);
	SCI_MEMSET( str.wstr_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );
    MMIAPISET_FormatDateStrByDateStyle(time.tm_year, time.tm_mon, time.tm_mday,'.',temp_str,30);
	//sprintf(temp_str, "%04d.%02d.%02d", time.tm_year, time.tm_mon, time.tm_mday);
	str.wstr_len = strlen( temp_str );
    MMI_STRNTOWSTR( str.wstr_ptr, CAF_FILENAME_MAX_LEN, temp_str, str.wstr_len, str.wstr_len );
    //str.is_ucs2 = FALSE;
    GUILABEL_SetText(MMIDYNA_APP_DETAIL_DATE_LABEL_CTRL_ID, &str, FALSE);

	/*set location*/
	if (CAF_MOD_STORE_PHONE == caf_detail->store)
	{
		GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID, TXT_PHONE, FALSE);
	}
	if (CAF_MOD_STORE_SD == caf_detail->store)
	{
 #ifndef DUAL_TCARD_SUPPORT
		GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID, TXT_COMMON_SD_CARD, FALSE);
 #else
        GUILABEL_SetTextById(MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID, TXT_COMMON_SD_CARD0, FALSE);
 #endif
	}

	SCI_FREE(str.wstr_ptr);
}

/*****************************************************************************/
// 	Description : 详情窗口处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDynaAppDetailWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
	MMI_RESULT_E				result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO            lcd_dev_info = {0};
    GUI_POINT_T         dis_point = {0};
    MMIDYNA_DETAIL_DATA_T        *detail_data = PNULL;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    detail_data = (MMIDYNA_DETAIL_DATA_T*)MMK_GetWinAddDataPtr(win_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_RECT_T lab_rect = {0};//,MMIDYNA_APP_DETAIL_NAME_TOP,MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_NAME_BOTTOM};
            lab_rect.left = 0;
            lab_rect.top = MMIDYNA_APP_DETAIL_NAME_TOP;
            lab_rect.right = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_NAME_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_NAME_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_NAME_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMIDYNA_APP_DETAIL_NAME_BOTTOM};
            lab_rect.left = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.top = MMIDYNA_APP_DETAIL_NAME_TOP;
            lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_NAME_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_NAME_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{0,MMIDYNA_APP_DETAIL_VERSION_TOP,MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_VERSION_BOTTOM};
            lab_rect.left = 0;
            lab_rect.top = MMIDYNA_APP_DETAIL_VERSION_TOP;
            lab_rect.right = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_VERSION_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_VERSION_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_VERSION_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMIDYNA_APP_DETAIL_VERSION_BOTTOM};
            lab_rect.left = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.top = MMIDYNA_APP_DETAIL_VERSION_TOP;
            lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_VERSION_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_VERSION_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{0,MMIDYNA_APP_DETAIL_DATE_TOP,MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_DATE_BOTTOM};
            lab_rect.left = 0;
            lab_rect.top = MMIDYNA_APP_DETAIL_DATE_TOP;
            lab_rect.right = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_DATE_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_DATE_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_DATE_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMIDYNA_APP_DETAIL_DATE_BOTTOM};
            lab_rect.left = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.top = MMIDYNA_APP_DETAIL_DATE_TOP;
            lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_DATE_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_DATE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }		
        {
            GUI_RECT_T lab_rect = {0};//{0,MMIDYNA_APP_DETAIL_MEMORY_TOP,MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_MEMORY_BOTTOM};
            lab_rect.left = 0;
            lab_rect.top = MMIDYNA_APP_DETAIL_MEMORY_TOP;
            lab_rect.right = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_MEMORY_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_MEMORY_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }	
        {
            GUI_RECT_T lab_rect = {0};//{MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_MEMORY_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMIDYNA_APP_DETAIL_MEMORY_BOTTOM};
            lab_rect.left = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.top = MMIDYNA_APP_DETAIL_MEMORY_TOP;
            lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_MEMORY_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_MEMORY_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{0,MMIDYNA_APP_DETAIL_LOCATION_TOP,MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_LOCATION_BOTTOM};
            lab_rect.left = 0;
            lab_rect.top = MMIDYNA_APP_DETAIL_LOCATION_TOP;
            lab_rect.right = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_LOCATION_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_LOCATION_TITLE_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
        {
            GUI_RECT_T lab_rect = {0};//{MMIDYNA_APP_DETAIL_LEFT,MMIDYNA_APP_DETAIL_LOCATION_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMIDYNA_APP_DETAIL_LOCATION_BOTTOM};
            lab_rect.left = MMIDYNA_APP_DETAIL_LEFT;
            lab_rect.top = MMIDYNA_APP_DETAIL_LOCATION_TOP;
            lab_rect.right = MMI_MAINSCREEN_RIGHT_MAX_PIXEL;
            lab_rect.bottom = MMIDYNA_APP_DETAIL_LOCATION_BOTTOM;
			GUILABEL_SetRect(MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID, &lab_rect, FALSE);
        }
    
        SetCAFDetail(detail_data);
        
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        break;

    case MSG_FULL_PAINT:
        dis_point.x = 0;
        dis_point.y = MMI_TITLE_HEIGHT;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                IMAGE_COMMON_BG,
                &lcd_dev_info);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != detail_data)
        {
           SCI_FREE(detail_data);
           detail_data = PNULL;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : 小程序列表主菜单处理函数
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
LOCAL CAF_RESULT_E  IsValidInstallApp(
                                      const uint16 *file_name
                                      )
{
	uint32				size = 0;
	MMK_SZIP_HEAD_INFO info  = {0};
	SFS_HANDLE  sfs_handle = 0;
	
	//SCI_PASSERT(file_name, ("MMIFMM: file_name is null")); /*assert verified*/
	if(PNULL == file_name)
	{
	   return CAF_RESULT_FAILED;
	}
	sfs_handle = SFS_CreateFile( file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );
	SCI_PASSERT(sfs_handle,("MMIFMM: sfs_createfile is error!")); /*assert verified*/
	
	//读文件头
    SFS_ReadFile( sfs_handle, (void*)&info, sizeof(MMK_SZIP_HEAD_INFO), &size, PNULL);
	SCI_PASSERT(size == sizeof(MMK_SZIP_HEAD_INFO), ("MMIFMM: ReadFile is error")); /*assert verified*/
	SFS_CloseFile( sfs_handle );
	/*计算CRC*/
	{
        uint32* crc_table_ptr = PNULL;
		uint32 crc_data = 0;
		uint32 read_size = 0;
		uint8 *pCrcBuff = NULL;
		uint32 file_crc = 0;
		SFS_HANDLE f_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);

        SCI_ASSERT(0 != f_handle); /*assert verified*/
		SFS_GetFileSize(f_handle, &size);
		if(size > MMI_SZIP_FILE_SIZE_MAX || size < DYNAMIC_FILE_CRC_NUM)
        {
            //SCI_TRACE_LOW:"[MOD MNG]: error! module code size is 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDYNA_WIN_1145_112_2_18_2_15_5_1,(uint8*)"d", size);
            return CAF_RESULT_MODU_SIZE_ERROR;
        }
		pCrcBuff = SCI_ALLOC_APP(DYNAMIC_FILE_CRC_NUM);
		SCI_ASSERT(NULL != pCrcBuff); /*assert verified*/
		/*将指针指向要计算CRC的地方*/
		SFS_SetFilePointer(f_handle, sizeof( MMK_SZIP_HEAD_INFO ) + sizeof(MMK_SZIP_FILE_INFO), 0);
		SFS_ReadFile(f_handle, pCrcBuff, DYNAMIC_FILE_CRC_NUM, &read_size, PNULL);
		SCI_ASSERT(read_size == DYNAMIC_FILE_CRC_NUM); /*assert verified*/
		SFS_CloseFile(f_handle);
        
		crc_table_ptr = MMK_CreateCRCTable();
		file_crc = MMK_CalculateBlockCRC32( crc_table_ptr, DYNAMIC_FILE_CRC_NUM, CRC_MASK, pCrcBuff );
		SCI_FREE(pCrcBuff);
		MMK_FreeCRCTable( crc_table_ptr );

		crc_data = ntohl(info.crc_data);
		if(file_crc == crc_data)
		{
			/*验证通过*/
			return CAF_RESULT_SUCCEEDED;
		}
	}
	return CAF_RESULT_FAILED;	
}
/*****************************************************************************/
// 	Description : handle open start app waiter window
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleInstallWaitWin(
										 MMI_WIN_ID_T       win_id, 	
										 MMI_MESSAGE_ID_E   msg_id, 
										 DPARAM             param
										 )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
		
    switch (msg_id)
    {
		case MSG_PUB_START_APP_WAIT:
		{
			BOOLEAN install = TRUE;
			SCI_ASSERT(s_new_full_path); /*assert verified*/
			install = UnpackSzipFile(s_full_file_name, 
									s_new_full_path, 
									MMIFMM_FULL_FILENAME_LEN );
			if(FALSE == install)
			{
				/*安装失败，删除所有文件，并弹出提示框*/
				MMIAPICOM_DelDirAll(s_new_full_path, 
									MMIFMM_FULL_FILENAME_LEN );
				MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);				
			}
			else
			{
				MMIPUB_OpenAlertSuccessWin(TXT_FMM_INSTALL_SUCC);
				/*update mod info*/
				MMK_LoadDynamicModInfo();
			}
			MMK_CloseWin(win_id);

			if (s_new_full_path)
            {
                SCI_FREE(s_new_full_path);
            }
			
			break;
		}
			
		default:
			result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
		break;		
	}
	return result;
}

/*****************************************************************************/
// 	Description : handle overwrite query win
//	Global resource dependence : none
//  Author: james.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDynaOverwriteQueryWin(
                                              MMI_WIN_ID_T     win_id, 
                                              MMI_MESSAGE_ID_E	msg_id, 
                                              DPARAM           param
                                              )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
//	BOOLEAN      result = FALSE;

	switch(msg_id)
	{
	case MSG_APP_OK:
	{
		MMI_STRING_T wait_text = {0};

		MMIPUB_CloseQuerytWin(PNULL);
		MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
		MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, MMIPUB_WAIT_WIN_ID,
						   COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
						   MMIPUB_SOFTKEY_NONE, HandleInstallWaitWin);
		MMK_PostMsg(MMIPUB_WAIT_WIN_ID, MSG_PUB_START_APP_WAIT, NULL, 0);
		
        break;		
	}	
	
	case MSG_APP_CANCEL:
		MMIPUB_CloseQuerytWin(PNULL);
		break;
	default:
		 recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
	}

	return recode;

}
/*****************************************************************************/
// 	Description : handle unpack query win
//	Global resource dependence : none
//  Author: james.zhang
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDynaUnpackSzipFileQueryWin(
                                                   MMI_WIN_ID_T     win_id, 
                                                   MMI_MESSAGE_ID_E	msg_id, 
                                                   DPARAM           param
                                                   )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
//	BOOLEAN      result = FALSE;

	switch(msg_id)
	{
	case MSG_APP_OK:
		MMIPUB_CloseQuerytWin(PNULL);

		MMK_CreateWin( (uint32*)MMIDYNA_INSTALL_STORE_TYPE_WIN_TAB , PNULL);		

        break;		
	
	case MSG_APP_CANCEL:
		MMIPUB_CloseQuerytWin(PNULL);
		break;
	default:
		 recode = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
		break;
	}

	return recode;

}



/*****************************************************************************/
//  Description : unpack szip
//  Global resource dependence :
//  Author: james.zhang
//  Note: 2007-6-12
/*****************************************************************************/
LOCAL BOOLEAN InstallSzipFile(
                              MMIFILE_DEVICE_E store_device
                              )
{
    BOOLEAN     result = FALSE;
    MMIFMM_SZIP_TYPE_E dir_result = MMIFMM_SZIP_NO_ERROR;
	uint32 space_high = 0;
	uint32 space_low = 0;
	uint size = 0;
	SFS_HANDLE			sfs_handle = 0;
	MMK_SZIP_HEAD_INFO	info = {0};
	uint16      name[MMIFMM_FILENAME_LEN+1] = {0};
	uint16      name_len = 0;
	MMI_STRING_T wait_text = {0};

	sfs_handle = SFS_CreateFile( s_full_file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );    
	if( 0 == sfs_handle )
	{		
		SFS_CloseFile( sfs_handle );
		return FALSE;
	}

	/*Add the file size item*/
	SFS_ReadFile( sfs_handle, (void*)&info, sizeof(MMK_SZIP_HEAD_INFO), &size, PNULL);
	SFS_CloseFile( sfs_handle );

	MMIAPIFMM_GetDeviceFreeSpace(
	                            MMIAPIFMM_GetDevicePath(store_device), 
								MMIAPIFMM_GetDevicePathLen(store_device),
								&space_high, 
								&space_low
								);
								
	if (0 == space_high && size > space_low)
	{
		/*空间不足*/
		MMIPUB_OpenAlertWarningWin(TXT_FMM_SPACE_IS_NOT_ENOUGH);
		return FALSE;
	}
	/*安装程序*/
	s_new_full_path = (wchar*)SCI_ALLOC_APP( ( MMIFMM_FULL_FILENAME_LEN + 1 ) * sizeof(wchar) );
	SCI_MEMSET( s_new_full_path, 0, (( MMIFMM_FULL_FILENAME_LEN + 1 ) * sizeof(wchar)) );				

	if ( !MMIAPIFMM_SplitFullPath( (uint16*)s_full_file_name, 
								MMIAPICOM_Wstrlen( s_full_file_name ),
								PNULL, PNULL, PNULL, PNULL, name, &name_len ) )
	{
		/*安装失败*/
		MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);
		if (s_new_full_path) SCI_FREE(s_new_full_path);
		return FALSE;
	}
	dir_result = UnpackSzipDir(store_device,
							   s_full_file_name, 
							   name,
							   name_len - 5,	//len of ".szip" is 5
							   s_new_full_path, 
							   MMIFMM_FULL_FILENAME_LEN);

	if(MMIFMM_SZIP_ERROR == dir_result)
	{
		/*安装失败*/
		MMIPUB_OpenAlertFailWin(TXT_COMM_INSTALL_FAILED);		
		if (s_new_full_path) SCI_FREE(s_new_full_path);
		return FALSE;
	}
	if(MMIFMM_SZIP_HAS_EXIT == dir_result)
	{
		/*已经存在，是否覆盖*/
		MMIPUB_OpenQueryWinByTextId( TXT_FMM_INSTALL_OVERWRITE,
									 IMAGE_PUBWIN_QUERY, PNULL,
									 (MMIPUB_HANDLE_FUNC) HandleDynaOverwriteQueryWin );
		//if (s_new_full_path) SCI_FREE(s_new_full_path);
		return TRUE;
		
	}	
	/*安装SZIP文件，弹出等待框*/
	MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
	MMIPUB_OpenWaitWin(1, &wait_text, PNULL, PNULL, MMIPUB_WAIT_WIN_ID,
					   COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL,
					   MMIPUB_SOFTKEY_NONE, HandleInstallWaitWin);
	MMK_PostMsg(MMIPUB_WAIT_WIN_ID, MSG_PUB_START_APP_WAIT, NULL, 0);
	
	return result;
}
/*****************************************************************************/
// 	Description : unpack file
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN UnpackSzipFile( 
                             const uint16		*full_path_name,					 
                             const uint16		*new_full_path, 
                             uint16			new_path_buf_len//wchar len
                             )
{
	uint16				file_full_len = 0;
	uint16				*file_name_ptr = NULL;
	MMK_SZIP_HEAD_INFO	info     = {0};
    uint32				size     = 0;
    SFS_HANDLE			sfs_handle = 0;
	uint16				full_len = 0;
	uint32				offset = 0;
	MMI_RESULT_E		result = MMI_RESULT_TRUE;
	uint32				i = 0;
	
	//SCI_ASSERT( PNULL != full_path_name ); /*assert verified*/
	if(PNULL == full_path_name)
	{
	    return FALSE;
	}

	sfs_handle = SFS_CreateFile( full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );
	if( 0 == sfs_handle )
    {
        return FALSE;
    }
	SFS_ReadFile( sfs_handle, (void*)&info, sizeof(MMK_SZIP_HEAD_INFO), &size, PNULL);
	info.file_num = ntohl(info.file_num);
	full_len = MMIAPICOM_Wstrlen(new_full_path);
	SCI_ASSERT(new_path_buf_len > full_len); /*assert verified*/
	
                
	offset = sizeof(MMK_SZIP_HEAD_INFO);
	/*分别解析SZIP里的文件*/
    for (i = 0; i < info.file_num; i++ )
    {
        SFS_HANDLE         new_file_handle = 0;
        MMK_SZIP_FILE_INFO file_info       = {0};
        uint8*             temp_name       = NULL;
        uint16*            new_file_name   = PNULL;
        uint16             new_file_name_len = 0;
        
        //读文件信息
        SFS_SetFilePointer( sfs_handle, offset, SFS_SEEK_BEGIN );
        SFS_ReadFile( sfs_handle, (void*)&file_info, sizeof(MMK_SZIP_FILE_INFO), &size, PNULL );

		/*change the sequence*/
		file_info.file_name_len = ntohl(file_info.file_name_len);
		file_info.file_size = ntohl(file_info.file_size);
		file_info.zip_size	= ntohl(file_info.zip_size);

        offset += sizeof(MMK_SZIP_FILE_INFO);

        //SCI_TRACE_LOW:"[Szip]:file_name_len = %d, file_size = %d, zip_size = %d, reserved = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDYNA_WIN_1445_112_2_18_2_15_5_2,(uint8*)"dddd",file_info.file_name_len, file_info.file_size, file_info.zip_size, file_info.reserved );
        
        temp_name = (uint8*)SCI_ALLOC_APP( file_info.file_name_len + 1 );
        SCI_MEMSET( temp_name, 0, (file_info.file_name_len + 1) );
        new_file_name = (uint16*)SCI_ALLOC_APP( (file_info.file_name_len + 1) * sizeof(wchar) );
        SCI_MEMSET( new_file_name, 0, ((file_info.file_name_len + 1) * sizeof(wchar)) );
        
        //读文件名
        SFS_SetFilePointer( sfs_handle, offset, SFS_SEEK_BEGIN );
        SFS_ReadFile( sfs_handle, (void*)temp_name, file_info.file_name_len * sizeof(uint8), &size, PNULL );
        offset += file_info.file_name_len;
        
        new_file_name_len = GUI_GBToWstr( new_file_name, temp_name, (uint16)file_info.file_name_len );
        
		if (full_len + new_file_name_len > new_path_buf_len)
		{
			SFS_CloseFile(sfs_handle);
			SCI_FREE(temp_name);
			SCI_FREE(new_file_name);
			return FALSE;
		}
		/*
        SCI_MEMCPY( new_full_path + full_len, new_file_name, new_file_name_len );
        new_full_path[full_len + new_file_name_len] = 0;
        */
		file_name_ptr = (uint16 *)SCI_ALLOC_APP( (MMIFMM_FULL_FILENAME_LEN +1 ) * sizeof(wchar) );
		SCI_MEMSET(file_name_ptr, 0, ((MMIFMM_FULL_FILENAME_LEN +1 ) * sizeof(wchar)) );
		MMIAPICOM_CombinePath(new_full_path, full_len, new_file_name, new_file_name_len, FALSE, file_name_ptr, &file_full_len);
        //根据文件名创建新文件
        new_file_handle = SFS_CreateFile( file_name_ptr, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL );
        SCI_FREE(file_name_ptr);
        if( 0 != new_file_handle )
        {
            uint8* file_buf   = (uint8*)SCI_ALLOC_APP( MMK_UNPACK_BUFFER_SIZE );
            uint32 file_size  = file_info.file_size;
            
            //读文件内容并写入新文件
            SFS_SetFilePointer( sfs_handle, offset, SFS_SEEK_BEGIN );

            while ( 1 )
            {
                SFS_SetFilePointer( sfs_handle, offset, SFS_SEEK_BEGIN );

                if( file_size > MMK_UNPACK_BUFFER_SIZE )
                {
                    SFS_ReadFile( sfs_handle, (void*)file_buf, MMK_UNPACK_BUFFER_SIZE, &size, PNULL );
                    SFS_WriteFile( new_file_handle, (const void *)file_buf, MMK_UNPACK_BUFFER_SIZE, &size, NULL );

                    file_size = file_size - MMK_UNPACK_BUFFER_SIZE;

                    offset += MMK_UNPACK_BUFFER_SIZE;
                }
                else
                {
                    SFS_ReadFile( sfs_handle, (void*)file_buf, file_size, &size, PNULL );
                    SFS_WriteFile( new_file_handle, (const void *)file_buf, file_size, &size, NULL );

                    offset += file_size;

                    break;
                }
            }
            
            SCI_FREE( file_buf );
            
            SFS_CloseFile( new_file_handle );

            //offset += file_info.file_size;
        }
        else
        {
            result = FALSE;
        }
        
        SCI_FREE( temp_name );
        SCI_FREE( new_file_name );
        
        if( !result )
        {
            break;
        }
    }
        

	SFS_CloseFile( sfs_handle );	
	return result;
}
/*****************************************************************************/
// 	Description : unpack file
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMIFMM_SZIP_TYPE_E UnpackSzipDir(	  
                                       MMIFILE_DEVICE_E store_device, 
                                       const uint16* full_path_name,
                                       const uint16 *file_name,      //without suffix
                                       uint16        file_name_len,   //without suffix
                                       uint16		*new_full_path, //out
                                       uint16		new_path_len
                                       )
{
	char		*str_szip = "SZIP";
	MMIFMM_SZIP_TYPE_E result = MMIFMM_SZIP_NO_ERROR;
    SFS_HANDLE  sfs_handle = 0;
    SFS_ERROR_E sfs_error  = 0;
    
	//SCI_ASSERT( PNULL != new_full_path ); /*assert verified*/
    //SCI_ASSERT( PNULL != full_path_name ); /*assert verified*/
    //SCI_ASSERT( PNULL != file_name ); /*assert verified*/
    if(
	   (PNULL == new_full_path)||
	   (PNULL == full_path_name)||
	   (PNULL == file_name)
	   )
    {
        return result;
    }
    
    sfs_handle = SFS_CreateFile( full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );
    
    if( 0 == sfs_handle )
    {
        result = MMIFMM_SZIP_ERROR;
    }
    else
    {
        
        uint32             size     = 0;
        uint32             offset   = 0;
		uint32			   full_len = 0;
		MMK_SZIP_HEAD_INFO info     = {0};
		
        //读文件头
        SFS_ReadFile( sfs_handle, (void*)&info, sizeof(MMK_SZIP_HEAD_INFO), &size, PNULL);

        offset += sizeof(MMK_SZIP_HEAD_INFO);
		if(0xFF == info.ident[0] && 
		   0 == MMIAPICOM_CompareTwoUint8String(info.ident + 1, 4, str_szip, 4))
		{
            wchar slash_char = '\\';
            //uint16 cif_dev_name_d_wstr_len = MMIAPICOM_Wstrlen( MOD_CIF_DEVICE_NAME_D );
            //uint16 cif_dev_name_e_wstr_len = MMIAPICOM_Wstrlen( MOD_CIF_DEVICE_NAME_E );
            //uint16 cif_folder_wstr_len     = MMIAPICOM_Wstrlen( MOD_CIF_FOLDER );

			/*标识为szip，识别创建目录*/
			if(MMI_DEVICE_NUM > store_device)
			{
				MMIAPICOM_Wstrncpy( new_full_path, MMIAPIFMM_GetDevicePath(store_device), MMIAPIFMM_GetDevicePathLen(store_device) );
				new_full_path[1] = MMIFILE_COLON;
				full_len = MMIAPICOM_Wstrlen(new_full_path);
			}
			
            MMIAPICOM_Wstrncpy( new_full_path + full_len, &slash_char, 1 );
            //SCI_MEMCPY( new_full_path + full_len, &slash_char, 1 );
            full_len += 1;


            MMIAPICOM_Wstrncpy( new_full_path + full_len, MOD_CIF_FOLDER, MOD_CIF_FOLDER_LEN );
            //SCI_MEMCPY( new_full_path + full_len, MOD_CIF_FOLDER, 3 );
            full_len += MOD_CIF_FOLDER_LEN;
			
			/*创建COM目录*/
			sfs_error = SFS_CreateDirectory( new_full_path );

            MMIAPICOM_Wstrncpy(  new_full_path + full_len, &slash_char, 1 );
			//SCI_MEMCPY( new_full_path + full_len, MOD_CIF_SLASH, 1 );
            full_len += 1;

            MMIAPICOM_Wstrncpy( new_full_path + full_len, file_name, file_name_len );
            //SCI_MEMCPY( new_full_path + full_len, file_name, file_name_len );
            full_len += file_name_len;
            
			SCI_ASSERT(full_len < new_path_len); /*assert verified*/
            //创建安装目录
            sfs_error = SFS_CreateDirectory( new_full_path );
            if (SFS_ERROR_HAS_EXIST == sfs_error)
			{
				result = MMIFMM_SZIP_HAS_EXIT;
			}
			else if (SFS_ERROR_NONE == sfs_error)
			{
				result = MMIFMM_SZIP_NO_ERROR;
			}
			else
			{
				result = MMIFMM_SZIP_ERROR;
			}
		}
		else
		{
			result = MMIFMM_SZIP_ERROR;
		}
	}
	SFS_CloseFile(sfs_handle);
	return result;        
}

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleDynaStoreType(
						  			    MMI_WIN_ID_T         win_id, 	
									    MMI_MESSAGE_ID_E     msg_id, 
									    DPARAM               param
			     					   )
{
	MMI_RESULT_E                result = MMI_RESULT_TRUE;


    switch (msg_id)
    {
		case MSG_OPEN_WINDOW:
		{
			uint8				str_ary[CAF_FILENAME_MAX_LEN + 1] = {0};
			uint32				size = 0;
			MMK_SZIP_HEAD_INFO	info = {0};
			SFS_HANDLE			sfs_handle = 0;
			uint16				uint16_str[50] = {0};
			uint16				uint16_len = 0;
			MMI_STRING_T		str_buf = {0};
			MMIFILE_DEVICE_E    dev = MMI_DEVICE_UDISK;

			GUILIST_SetMaxItem(MMIDYNA_INSTALL_STORE_LIST_CTRL_ID, 3, FALSE);

			GUILIST_SetOwnSofterKey( MMIDYNA_INSTALL_STORE_LIST_CTRL_ID, TRUE );
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);

			/*计算文件大小及剩余空间*/

			/*Get info about szip file*/
				
			sfs_handle = SFS_CreateFile( s_full_file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL );    
			if( 0 == sfs_handle )
			{
				result = FALSE;
				SFS_CloseFile( sfs_handle );
				break;
			}

			/*Add the file size item*/
			SFS_ReadFile( sfs_handle, (void*)&info, sizeof(MMK_SZIP_HEAD_INFO), &size, PNULL);
			SFS_CloseFile( sfs_handle );
			info.total_size = ntohl(info.total_size);
			sprintf((char *)str_ary, "%ld.%03ldK", (info.total_size / 1024), ((info.total_size%1024)*1000/1024));

			//str_buf.wstr_ptr = wstr_ary;
			//str_buf.wstr_len = strlen(str_ary);
			//str_buf.is_ucs2 = FALSE;

			MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_len, TXT_FMM_NEED_SPACE);
			MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_len, str_ary, strlen(str_ary) );

			str_buf.wstr_ptr = uint16_str;
			str_buf.wstr_len = uint16_len;
			//str_buf.is_ucs2 = TRUE;
			
			AppendListByTxtPtr(MMIDYNA_INSTALL_STORE_LIST_CTRL_ID, 
								&str_buf, TXT_NULL, TXT_NULL, STXT_RETURN, GUIITEM_STYLE_ONE_LINE_TEXT);

			/*add radio item*/
			do
			{
			    SetRadioStoreTypeItem(dev);
            }while(++dev < MMI_DEVICE_NUM);
            
			MMK_SetAtvCtrl( win_id, MMIDYNA_INSTALL_STORE_LIST_CTRL_ID );
			break;			
		}

        case MSG_CTL_MIDSK:
		case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_OK:
		case MSG_APP_WEB:
		{			
			uint16 cur_index = 0;

			cur_index = GUILIST_GetCurItemIndex( MMIDYNA_INSTALL_STORE_LIST_CTRL_ID );
			//1TODO:Confirm the modi!
			if(0 != cur_index)
			{
				InstallSzipFile((MMIFILE_DEVICE_E)(MMI_DEVICE_UDISK + cur_index - 1));	
		    }
		    MMK_CloseWin(win_id);
			break;
		}

		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:			
			MMK_CloseWin(win_id);
			break;

		case MSG_CLOSE_WINDOW:
			if (s_new_full_path) SCI_FREE(s_new_full_path);
			break;

		default:
	        result = MMI_RESULT_FALSE;
		    break;
	}
	return result;
}

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL void   SetRadioStoreTypeItem(
                                   MMIFILE_DEVICE_E      store_type_e
                                   )
{
	wchar				unicode_tab = 0x09;
	uint32				space_high = 0;
	uint32				space_low = 0;
	MMI_STRING_T		tab_s = {0};
	MMI_STRING_T		str_buf = {0};
	uint16				uint16_str[CAF_FILENAME_MAX_LEN + 1] = {0};
	uint16				uint16_len = 0;
	uint8				str_ary[CAF_FILENAME_MAX_LEN + 1] = {0};
	
	/*Add the phone item*/
	MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(store_type_e),
	                            MMIAPIFMM_GetDevicePathLen(store_type_e),
								&space_high,
								&space_low
								);
	
	sprintf((char *)str_ary, "%ld.%03ldK", 
			space_high*4*1024*1024 + MMICOM_GET_KBYTE(space_low), 
			MMICOM_GET_PERCENTKBYTE(space_low));

	tab_s.wstr_ptr = &unicode_tab;
	tab_s.wstr_len = 1;
	
	MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_len, MMIAPIFMM_GetDeviceName(store_type_e));
	
	if (MMI_DEVICE_UDISK == store_type_e)
	{
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_len, &tab_s);
		MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_len, &tab_s);
	}	
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_len, &tab_s);
	MMIAPICOM_StrcatFromSTRINGToUCS2(uint16_str, &uint16_len, &tab_s);
	MMIAPICOM_StrcatFromTextIDToUCS2(uint16_str, &uint16_len, TXT_FMM_FREE_SPACE);
	MMIAPICOM_StrcatFromStrToUCS2(uint16_str, &uint16_len, str_ary, strlen(str_ary) );

	str_buf.wstr_ptr = uint16_str;
	str_buf.wstr_len = uint16_len;
//	str_buf.is_ucs2 = TRUE;
 
	AppendListByTxtPtr(MMIDYNA_INSTALL_STORE_LIST_CTRL_ID, 
								&str_buf, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, GUIITEM_STYLE_ONE_LINE_RADIO);
}

/*****************************************************************************/
//  Description : handle message for radio window
//  Global resource dependence :
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL void  AppendListByTxtPtr(
                               MMI_CTRL_ID_T        ctrl_id,
                               MMI_STRING_T			*str_ptr, 
                               MMI_TEXT_ID_T        left_softkey_id,
                               MMI_TEXT_ID_T        middle_softkey_id,
                               MMI_TEXT_ID_T        right_softkey_id,
                               GUIITEM_STYLE_E      item_style
                               )
{
//	MMI_RESULT_E		result = MMI_RESULT_FALSE;
	GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = item_style;
    item_t.item_data_ptr = &item_data;
    
	//SCI_ASSERT( PNULL != str_ptr ); /*assert verified*/
	if(PNULL == str_ptr)
	{
	   return;
	}

    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;

    item_data.item_content[0].item_data.text_buffer.wstr_len = str_ptr->wstr_len;
    //item_data.item_content[0].item_data.text_buffer.is_ucs2 = str_ptr->is_ucs2;

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = str_ptr->wstr_ptr;

//     MMIAPICOM_Wstrncpy(item_data.item_content[0].item_data.text_buffer.wstr, 
//         str_ptr->wstr_ptr, 
//         item_data.item_content[0].item_data.text_buffer.wstr_len
//         );

//     SCI_MEMCPY(item_data.item_content[0].item_data.text_buffer.str, 
//         str_ptr->str_ptr, 
//         item_data.item_content[0].item_data.text_buffer.str_len);

    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = middle_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );  
}

#endif


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

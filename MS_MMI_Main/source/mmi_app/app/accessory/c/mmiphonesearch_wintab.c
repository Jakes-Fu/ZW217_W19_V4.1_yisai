/*****************************************************************************
** File Name:		mmiphonesearch_wintab.c                                                
** Author:			zhenhao.huang
** Date:            2007/11
** Copyright:		2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:		This file is used to describe mmi stopwatch common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE        	NAME             	DESCRIPTION                              *
** 2007/11/16	zhenhao.huang		Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h" 
#endif
//#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"

#if defined(HOME_LOCATION_INQUIRE_SUPPORT) && (defined IM_SIMP_CHINESE_SUPPORT|| defined IM_TRAD_CHINESE_SUPPORT)//宏定义开头
#include "mmiacc_id.h"
#include "mmiacc_text.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmiphonesearch_export.h"
#include "mmimp_location.h"
#include "mmipub.h"
#include "mmiacc_image.h"
#include "mmi_modu_main.h"
#include "mmi_data.h"
#include "guiedit.h"
#ifdef _ATEST_
#include "atest_location.h"
#endif
#include "gui_ucs2b_converter.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmifmm_export.h"
#include "mmiacc_menutable.h"
#include "mmisd_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 //for cr125359
//#define MMIPHONESEARCH_MAIN_LIST_MAX_NUM	50
#define MMIPHONESEARCH_LIST_MAX_NUM			800
#define RECORDSIZE						(MMIFINDMP_CITYNAME_LENGTH+4)
#define RECORDSIZE_CITYIDQUHAO			2
#define RECORDSIZE_AREACODE				5	//area code string length
#define RECORDSIZE_PROVINCE				10  //province name string length
#define RECORDSIZE_AREACODECITY			(MMIFINDMP_CITYNAME_LENGTH+RECORDSIZE_PROVINCE+RECORDSIZE_AREACODE)	
										//record size of AREACODE2CITY
#define SEEK_INVALID_INDEX              0xFFFF
#define MMIPHONESEARCH_LOCAL_PHONE_MAX_LENGTH			8
#define MMIPHONESEARCH_XG_OM_TW_OFFSET                  30000      //香港/澳门/台湾区号以00开始与内陆城市按照数字会相同，所以统一数据多100000

#define LOCATION_FILE_TCARD_PATH L"E:\\location\\"
#define LOCATION_FILE_TCARD1_PATH L"F:\\location\\"
#define LOCATION_FILE_UDISK_PATH L"C:\\location\\"
#define LOCATION_FILE_DOWNLOAD_URL   L"http://51wre.com/pnl/"

typedef enum
{
	MMI_PHONESEARCH_NONE,							/*!<default  */
	MMI_PHONESEARCH_MAIN,							/*!<main window  */
	MMI_PHONESEARCH_RESULT,							/*!<window with result  */
	MMI_PHONESEARCH_INPUT,							/*!<window with input  */
	MMI_PHONESEARCH_MAX								/*!<  */
}MMI_PHONESEARCH_UI_E;

typedef enum
{
    SEEK_COUNTRY_CODE =                  1,
    SEEK_PROV_CODE    =                  2,
    SEEK_ZONE_CODE    =                  3,
    SEEK_CODE_MAX
}SEEK_CODE_GRADE;

typedef struct list_grade_tag
{
    SEEK_CODE_GRADE     code_grade;
    uint16              index;
}ZONECODE_LIST_GRADE_T;

LOCAL ZONECODE_LIST_GRADE_T s_list_grade = {0}; /*lint !e64*/

LOCAL MMI_PHONESEARCH_UI_E s_phonesearch_cur_ui = MMI_PHONESEARCH_NONE;

LOCAL uint8	**s_phonesearch_data_pptr =PNULL;
LOCAL MMI_PHONESEARCH_UI_E s_current_status = MMI_PHONESEARCH_NONE;

/*****************************************************************************/
//  Description : main window message handling
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : input window message handling
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : left softkey handling of input wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputLeftSK(void);

/*****************************************************************************/
//  Description : right softkey handling of input wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputRightSK(void);

/*****************************************************************************/
// 	Description: initialize the input wnd
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: 
/*****************************************************************************/
LOCAL void InitInputWnd(void);

/*****************************************************************************/
//  Description : left softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainLeftSK(void);

/*****************************************************************************/
//  Description : right softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainRightSK(void);

/*****************************************************************************/
//  Description : mid softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainMidSK(void);

/*****************************************************************************/
// 	Description: initialize the list
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: 
/*****************************************************************************/
LOCAL void InitMainWnd(void);

/*****************************************************************************/
// 	Description: update the list with search result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in UCS2/GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateList(wchar* text, uint16 length);

/*****************************************************************************/
// 	Description: update the list with search city name's results
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithCity(wchar* text,uint16 length);

/*****************************************************************************/
// 	Description: update the list with search province or 直辖市 name's results
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithProvince(wchar* text,uint16 text_len);

/*****************************************************************************/
// 	Description: update the list with search Mobile Phone's result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: the mobile phone number in text( >= "1300000" <= "13999999999" )
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithMP( uint8* number, uint32 length );

/*****************************************************************************/
// 	Description: update the list with search Area Code's result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: the area code in text( e.g: "21", "517" )
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithAreaCode( const uint8* code, uint32 length );

/*****************************************************************************/
// 	Description: append the list item with text of GB into MMIPHONESEARCH_MAIN_LIST_ID
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of GUILIST_STRING_MAX_NUM
//		  wnd status, the wnd status of the list, since in main wnd and result wnd, the
//		  softkey status are different
/*****************************************************************************/
LOCAL void AppendListWithGBText(uint8* text, MMI_PHONESEARCH_UI_E wnd_status);

/*****************************************************************************/
// 	Description: check if the input code is of a zhi xia shi
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: return TRUE if it is a zhi xia shi's code
/*****************************************************************************/
LOCAL BOOLEAN is_code_zhixiashi( uint16 code );

/*****************************************************************************/
// 	Description: return the length in bytes of the name of the province or 
//               zhi xia shi specified the input code
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note:
/*****************************************************************************/
LOCAL uint32 length_of_province_name( uint16 code );

/*****************************************************************************/
// 	Description: append the list item with text 
//	Global resource dependence: 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void AppendPhoneSearchListTextData(uint8* text,uint32 index);

/*****************************************************************************/
// 	Description: alloc and add src to s_phonesearch_data_pptr[i]
//	Global resource dependence: 
//  Author: zhaohui
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL void AppendDynamicItem(int index, char* src, uint16 src_max_length);

/*****************************************************************************/
//  Description : HandlePhoneSearchQueryWinMsg
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchQueryWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          );
#if defined(BROWSER_SUPPORT) && defined(HOME_LOCATION_INQUIRE_SUPPORT_TCARD)
/*******************************************************************/
//  Interface:		HandlePHONESEARCHDownloadCB
//  Description : 	HandlePHONESEARCHDownloadCB
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
LOCAL void HandlePHONESEARCHDownloadCB(
								   MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr
								   );
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
/*******************************************************************/
//  Interface:		MoveFileToLocation
//  Description : 	MoveFileToLocation
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
LOCAL void MoveFileToLocation(const wchar *file_path);
/*****************************************************************************/
//  Description : get location file dir
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidLocationFileDir(uint16* device_dir,const wchar *file_name_ptr,uint32 file_size);
#endif
#if defined(BROWSER_SUPPORT) && defined(HOME_LOCATION_INQUIRE_SUPPORT_TCARD) 
/*******************************************************************/
//  Interface:		OpenDownLoadUrl
//  Description : 	OpenDownLoadUrl
//  Global resource dependence : 
//  Author:
//  Note:
/*******************************************************************/
LOCAL void OpenDownLoadUrl(void);
#endif
/*****************************************************************************/

WINDOW_TABLE( MMIPHONESEARCH_MAIN_WIN_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePhoneSearchMainWinMsg),
    WIN_ID(MMIPHONESEARCH_MAIN_WIN_ID),
    WIN_TITLE(TXT_SEARCHPHONE_SELECT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIPHONESEARCH_MAIN_LIST_ID),
    END_WIN
};

WINDOW_TABLE( MMIPHONESEARCH_INPUT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePhoneSearchInputWinMsg),
    WIN_ID(MMIPHONESEARCH_INPUT_WIN_ID),
    WIN_TITLE(TXT_SEARCHPHONE_INPUT),
	CREATE_EDIT_TEXT_CTRL(MMIFINDMP_CITYNAME_LENGTH,MMIPHONESEARCH_INPUT_EDIT_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIPHONESEARCH_POP_QUERY_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePhoneSearchQueryWinMsg),
    WIN_ID(MMIPHONESEARCH_POP_QUERY_WIN_ID),
    CREATE_POPMENU_CTRL(MENU_PHONESEARCH_OPT,MMIPHONESEARCH_MENU_CTRL_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    END_WIN
};

/*****************************************************************************/
// 	Description: check if the input code is of a zhi xia shi
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: return TRUE if it is a zhi xia shi's code
/*****************************************************************************/
LOCAL BOOLEAN is_code_zhixiashi( uint16 code )
{
	if( ( 10 == code ) || (( code >= 21 ) && ( code <= 23 )) )
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
// 	Description: return the length in bytes of the name of the province or 
//               zhi xia shi specified the input code
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note:
/*****************************************************************************/
LOCAL uint32 length_of_province_name( uint16 code )
{
	// 451, 黑龙江哈尔滨市, 483	内蒙古阿拉善盟
	if( ( code >= 451) && ( code <= 483 ) )
		return 6;
	else
		return 4;
}

/*****************************************************************************/
// 	Description: reset the content of list according to the specified wnd_status
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: 
/*****************************************************************************/
//LOCAL void ResetList(MMI_PHONESEARCH_UI_E wnd_status)
//{
//	MMI_STRING_T		str1 = {0};
//	MMI_STRING_T		str2 = {0};   
//	
//	//setmaxitem will clear and reset the list items
//    GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, MMIPHONESEARCH_LIST_MAX_NUM, FALSE ); //500
	
	// temporary comment because of list's incapability of text justification
	//if( MMI_PHONESEARCH_RESULT == wnd_status )
/*	if( FALSE )
	{
        GUILIST_ITEM_T          item_t    = {0};
        GUILIST_ITEM_DATA_T     item_data = {0};

        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;

        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;

        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        
		// set the text and softkeys
		MMI_GetLabelTextByLang( TXT_SEARCHPHONE_CITY, &str1 );
        MMI_GetLabelTextByLang( TXT_SEARCHPHONE_QUHAO, &str2 );

		SCI_MEMCPY( item_data.item_content[0].item_data.text_buffer.str, str1.str_ptr, str1.length);
		item_data.item_content[0].item_data.text_buffer.str_len += str1.length;
		
		SCI_MEMCPY( item_data.item_content[0].item_data.text_buffer.str+str1.length, str2.str_ptr, str2.length );
		item_data.item_content[0].item_data.text_buffer.str_len += str2.length;
		item_data.item_content[0].item_data.text_buffer.is_ucs2 = str1.is_ucs2;

		GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
	}
*/
// }

/*****************************************************************************/
// 	Description: append the list item with text of GB into MMIPHONESEARCH_MAIN_LIST_ID
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of GUILIST_STRING_MAX_NUM
//		  wnd status, the wnd status of the list, since in main wnd and result wnd, the
//		  softkey status are different
/*****************************************************************************/
LOCAL void AppendListWithGBText(uint8* text, MMI_PHONESEARCH_UI_E wnd_status)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER; 
    
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen((char*)text);
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;

    GUI_GBToWstr( temp_wstr,text,strlen((char*)text));   	
//    MMI_STRNTOWSTR(item_data.item_content[0].item_data.text_buffer.wstr,
//        item_data.item_content[0].item_data.text_buffer.wstr_len,
//        text,
//        strlen((char*)text),
//        strlen((char*)text));
   // GUI_GB2UCS((uint16*)item_data.item_content[0].item_data.text_buffer.str, text, strlen((char*)text));
   // item_data.item_content[0].item_data.text_buffer.is_ucs2 = TRUE;

    // set the softkeys
    if( MMI_PHONESEARCH_MAIN == wnd_status )
    {
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_INQUIRE;
        item_data.softkey_id[2] = STXT_RETURN;
    }
    else if( MMI_PHONESEARCH_RESULT == wnd_status )
    {
        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
    }

	GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
}
/*****************************************************************************/
// 	Description: append the list item with text 
//	Global resource dependence: 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
LOCAL void AppendPhoneSearchListTextData(uint8* text,uint32 index)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER; 

    item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;

    GUI_GBToWstr( temp_wstr ,text,strlen((char*)text));
    item_data.item_content[0].item_data.text_buffer.wstr_len = strlen((char*)text);
   
    //    GUI_GB2UCS((uint16*)item_data.item_content[0].item_data.text_buffer.str, text, strlen((char*)text));
   // item_data.item_content[0].item_data.text_buffer.is_ucs2 = TRUE;

	if( MMI_PHONESEARCH_MAIN == s_current_status)
    {
        item_data.softkey_id[0] = STXT_OPTION;
        item_data.softkey_id[1] = TXT_INQUIRE;
        item_data.softkey_id[2] = STXT_RETURN;
    }
    else if( MMI_PHONESEARCH_RESULT == s_current_status )
    {
        item_data.softkey_id[0] = TXT_NULL;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
    }
	
	GUILIST_SetItemData( MMIPHONESEARCH_MAIN_LIST_ID, &item_data, index); 
}


/*****************************************************************************/
// 	Description: initialize the main wnd
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: 
/*****************************************************************************/
LOCAL void InitMainWnd(void)
{
	uint16					sel_item_index = 0;
	MMIAUD_RING_DATA_INFO_T file_info = {0}; /*lint !e64*/
	uint8*					ptr = PNULL;
	uint8					city_name[MMIFINDMP_CITYNAME_LENGTH]={0};
	uint16					city_type = 0;
	uint32					index = 0;
	uint32					i = 0;
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8   src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
	// set the mid SK button
	GUIWIN_SetTitleTextId(MMIPHONESEARCH_MAIN_WIN_ID,TXT_SEARCHPHONE_SELECT, FALSE);
	
	//reset list
	sel_item_index = GUILIST_GetCurItemIndex(MMIPHONESEARCH_MAIN_LIST_ID);
	MMK_SetAtvCtrl(MMIPHONESEARCH_MAIN_WIN_ID , MMIPHONESEARCH_MAIN_LIST_ID);
	//ResetList(MMI_PHONESEARCH_MAIN);

#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_CITY_TYPE_QUHAO_TCARD,strlen((char*)R_ACC_CITY_TYPE_QUHAO_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else     	
	MMI_GetDataInfo(R_ACC_CITY_TYPE_QUHAO, &file_info);
#endif
	ptr = file_info.data_ptr;
	//SCI_ASSERT( ptr != PNULL );/*assert verified*/
	if( ptr == PNULL )
	{
        return;
    }
	// set the softkeys
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
	if(PNULL == s_phonesearch_data_pptr)
	{
		s_phonesearch_data_pptr = SCI_ALLOCA(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*));
		SCI_MEMSET(s_phonesearch_data_pptr,0x00,(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*)));
	}
	else
	{
		while(PNULL != s_phonesearch_data_pptr[i])
		{
			SCI_FREE(s_phonesearch_data_pptr[i]);
			i++;
		}
	}	
	// list all the 省和直辖市
	while( ((uint32)(ptr - file_info.data_ptr)) < file_info.data_len )
	{
		// if the city type is not a normal city
		city_type = ptr[MMIFINDMP_CITYNAME_LENGTH]+ptr[MMIFINDMP_CITYNAME_LENGTH+1]*256;
		if(city_type)
		{
			SCI_MEMSET(city_name,0,sizeof(city_name));
			strncpy((char*)city_name,(const char*)ptr,sizeof(city_name));
			
			// set text
            AppendDynamicItem(index,(char*)city_name,MMIFINDMP_CITYNAME_LENGTH);
			index++;
			// skip the cities owned by this province
			if( city_type > 1 )
				ptr+=city_type*RECORDSIZE;
		}
		// move to next record
		ptr+=RECORDSIZE;
	}
	s_current_status = MMI_PHONESEARCH_MAIN;
	GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, index, TRUE );
	for (i = 0; i<index;i++)
	{
		GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
	}
    if (s_list_grade.index != SEEK_INVALID_INDEX)
    {
        sel_item_index = s_list_grade.index;
    }
    GUILIST_SetCurItemIndex(MMIPHONESEARCH_MAIN_LIST_ID, sel_item_index);
    s_list_grade.index = SEEK_INVALID_INDEX;
    s_list_grade.code_grade = SEEK_CODE_MAX;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif
    // GUILIST_UpdateListBox(list_ctrl_id);
}

/*****************************************************************************/
// 	Description: update the list with search city name's results
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithCity( wchar* text,uint16 length )
{
	uint16					city_type                                   = 0;
	uint16					area_code                                   = 0;
	uint8*					ptr                                         = PNULL;
	char*					sub_str                                     = PNULL;
	BOOLEAN					found                                       = FALSE;
	BOOLEAN					province_show                               = FALSE;
	uint8					concat_text[GUILIST_STRING_MAX_NUM]         = {0};
	uint8					city_name[MMIFINDMP_CITYNAME_LENGTH]        = {0};
	uint8					province_name[MMIFINDMP_CITYNAME_LENGTH]    = {0};
	uint32					index                                       = 0;
	MMIAUD_RING_DATA_INFO_T file_info                                   = {0}; /*lint !e64*/
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    uint8 *                  tmp_text = PNULL;
    uint32 i = 0;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8   src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
    
    tmp_text = (uint8 *)SCI_ALLOC_APPZ(length*2+1);
    //SCI_ASSERT(PNULL != tmp_text);/*assert verified*/
    SCI_MEMSET(tmp_text,0,(length*2+1));
	GUI_WstrToGB( tmp_text, text, length );
    //MMI_WSTRNTOSTR(tmp_text,length*2,text,length,length);

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_CITY_TYPE_QUHAO_TCARD,strlen((char*)R_ACC_CITY_TYPE_QUHAO_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else 	
	MMI_GetDataInfo(R_ACC_CITY_TYPE_QUHAO, &file_info);
#endif
	ptr = file_info.data_ptr;
	//SCI_ASSERT( ptr != PNULL );/*assert verified*/
    if(ptr == PNULL)
    {
        SCI_FREE(tmp_text);
        return found;
    }
	// find the city or its cities if it is a province
	while( ((uint32)(ptr - file_info.data_ptr)) < file_info.data_len )
	{
		city_type = ptr[MMIFINDMP_CITYNAME_LENGTH]+ptr[MMIFINDMP_CITYNAME_LENGTH+1]*256;
		if( city_type > 1)
		{
			// store the current province name
			SCI_MEMSET(province_name,0,sizeof(province_name));
			strncpy((char*)province_name, (const char*)ptr,strlen((const char*)ptr));
			province_show = FALSE;
		}
			
		// if found the input string in city name
		sub_str = strstr((const char*)ptr,(const char*)tmp_text);/*lint !e605 */
		while( sub_str )
		{
			uint32 offset = (uint32)sub_str-(uint32)ptr;
			if( (offset%2) == 0 )
			{
				if( !found )
				{
					// find 1st record
					found = TRUE;					
					//reset log list
					if(PNULL == s_phonesearch_data_pptr)
					{
						s_phonesearch_data_pptr = SCI_ALLOCA(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*));
						SCI_MEMSET(s_phonesearch_data_pptr,0x00,(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*)));
					}
					else
					{
						while(PNULL != s_phonesearch_data_pptr[index])
						{
							SCI_FREE(s_phonesearch_data_pptr[index]);
							index++;
						}
						index = 0;
					}

				}
					
				SCI_MEMSET(city_name,0,sizeof(city_name));
				strncpy((char*)city_name,(const char*)ptr,sizeof(city_name));
				
				if( city_type == 0 )
				{
					// just a normal city, print its province name and its name
					area_code = ptr[MMIFINDMP_CITYNAME_LENGTH+2]+ptr[MMIFINDMP_CITYNAME_LENGTH+3]*256;

					if( !province_show )
					{
						// set text of its province
                        AppendDynamicItem(index,(char*)province_name,MMIFINDMP_CITYNAME_LENGTH);
                        index++;
						province_show = TRUE;
					}

					// set text
					SCI_MEMSET(concat_text,0,sizeof(concat_text));
					sprintf((char*)concat_text,"    %s  0%d", city_name,area_code);
                    AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
                    index++;
				}
				else if( city_type == 1)
				{
					// zhixiashi, set text
					area_code = ptr[MMIFINDMP_CITYNAME_LENGTH+2]+ptr[MMIFINDMP_CITYNAME_LENGTH+3]*256;
					SCI_MEMSET(concat_text,0,sizeof(concat_text));
                    if (area_code> MMIPHONESEARCH_XG_OM_TW_OFFSET) //香港/澳门/台湾
                    {
                        area_code -=MMIPHONESEARCH_XG_OM_TW_OFFSET;
				        sprintf((char*)concat_text,"%s  00%d", city_name,area_code);
                    }
                    else
                    {
				        sprintf((char*)concat_text,"%s  0%d", city_name,area_code);
                    }
                    AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
                    index ++;
				}
				else
				{
					// if it is a province, list its name

					//SCI_MEMSET(s_phonesearch_data,0x00,sizeof(s_phonesearch_data));
                    AppendDynamicItem(index,(char*)city_name,MMIFINDMP_CITYNAME_LENGTH);
                    index++;	
				    // and list its owned cities
					for( i = 0; i < city_type; i++)
					{
						ptr += RECORDSIZE;
						area_code = ptr[MMIFINDMP_CITYNAME_LENGTH+2]+ptr[MMIFINDMP_CITYNAME_LENGTH+3]*256;
						SCI_MEMSET(concat_text,0,sizeof(concat_text));
						sprintf((char*)concat_text,"    %s  0%d", ptr,area_code);
                        AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
              			index++;
					}
				}
				break;
			}
			else
			{
				// continue search within the str
				sub_str = strstr((const char*)sub_str + 1,(const char*)tmp_text);/*lint !e605 */
			}
		}
		ptr += RECORDSIZE;
	}
			
    if (0 < index)//有查询结果才重新设置list
    {	
        GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, index, TRUE );
		s_current_status = MMI_PHONESEARCH_RESULT;	
        for (i = 0; i<index;i++)
        {
            GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
        }
    }	
	
    SCI_FREE(tmp_text);
    tmp_text = PNULL;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif
	return found;
}

/*****************************************************************************/
// 	Description: update the list with search province or 直辖市 name's results
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithProvince(wchar* text,uint16 text_len)
{
	MMIAUD_RING_DATA_INFO_T file_info = {0}; /*lint !e64*/
	uint8*					ptr = PNULL;
	uint8					city_name[MMIFINDMP_CITYNAME_LENGTH]={0};
	uint16					city_type = 0;
	uint8					concat_text[GUILIST_STRING_MAX_NUM]={0};
	uint32					index = 0;
	uint16					area_code = 0;
	BOOLEAN					found = FALSE;
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    uint8 *                  tmp_text = PNULL;
	uint32                  i = 0;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8   src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
    
    tmp_text = SCI_ALLOC_APPZ(text_len*2+1);
	GUI_WstrToGB(tmp_text,text,text_len);
//    MMI_WSTRNTOSTR(tmp_text,GUILIST_STRING_MAX_NUM,text,GUILIST_STRING_MAX_NUM,GUILIST_STRING_MAX_NUM);
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_CITY_TYPE_QUHAO_TCARD,strlen((char*)R_ACC_CITY_TYPE_QUHAO_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else 	
	MMI_GetDataInfo(R_ACC_CITY_TYPE_QUHAO, &file_info);
#endif
	ptr = file_info.data_ptr;
	//SCI_ASSERT( ptr != PNULL );/*assert verified*/
    if(ptr == PNULL)
    {
        SCI_FREE(tmp_text);
        return found;
    }
    
	if(PNULL == s_phonesearch_data_pptr)
	{
		s_phonesearch_data_pptr = SCI_ALLOCA(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*));
		SCI_MEMSET(s_phonesearch_data_pptr,0x00,(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*)));
	}
	else
	{
		while(PNULL != s_phonesearch_data_pptr[index])
		{
			SCI_FREE(s_phonesearch_data_pptr[index]);
			index++;
		}
		index = 0;
	}
	// find the city or its cities if it is a province
	while( ((uint32)(ptr - file_info.data_ptr)) < file_info.data_len )
	{
		city_type = ptr[MMIFINDMP_CITYNAME_LENGTH]+ptr[MMIFINDMP_CITYNAME_LENGTH+1]*256;
        
		
		if( city_type == 0 )
		{
			// not a province or 直辖市, go to the next record
			ptr += RECORDSIZE;
		}
		else if( strncmp((const char*)ptr,(const char*)tmp_text, strlen((const char*)tmp_text)) )
		{
			// not match, go to the next record
			ptr += RECORDSIZE;
		}
		else
		{
			// if found the input string in city name
			//reset log list
			//ResetList(MMI_PHONESEARCH_RESULT);

			// copy its name
			SCI_MEMSET(city_name,0,sizeof(city_name));
			strncpy((char*)city_name,(const char*)ptr,sizeof(city_name));

			if( city_type == 1)
			{
				// 直辖市, set text
				area_code = ptr[MMIFINDMP_CITYNAME_LENGTH+2]+ptr[MMIFINDMP_CITYNAME_LENGTH+3]*256;
				SCI_MEMSET(concat_text,0,sizeof(concat_text));
                if (area_code> MMIPHONESEARCH_XG_OM_TW_OFFSET) //香港/澳门/台湾
                {
                    area_code -=MMIPHONESEARCH_XG_OM_TW_OFFSET;
				    sprintf((char*)concat_text,"%s  00%d", city_name,area_code);
                }
                else
                {
				    sprintf((char*)concat_text,"%s  0%d", city_name,area_code);
                }
                AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
				index++;				
			}
			else
			{
				// if it is a province, list its name

				//SCI_MEMSET(s_phonesearch_data,0x00,sizeof(s_phonesearch_data));
                AppendDynamicItem(index,(char*)city_name,MMIFINDMP_CITYNAME_LENGTH);
				index ++;

				// and list its owned cities
				for( i = 0; i < city_type; i++)
				{
					ptr += RECORDSIZE;
					area_code = ptr[MMIFINDMP_CITYNAME_LENGTH+2]+ptr[MMIFINDMP_CITYNAME_LENGTH+3]*256;
					SCI_MEMSET(concat_text,0,sizeof(concat_text));
//xiyuan edit for CR00108669	
                    sprintf((char*)concat_text,"%s  0%d", ptr,area_code);
                    AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
					index ++;
				}
			}
			// set the flag and exit the loop
			found =  TRUE;
			break;
		}
	}
	
    if (0<index)//有查询结果,才重置guilist
    {
	    GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, index, TRUE );
	    s_current_status = MMI_PHONESEARCH_RESULT;
//xiyuan edit for CR00108669
	    for (i = 0; i<index;i++)
	    {
			if (0 == i)
			{           
				item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
		        GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
			}
			else
			{
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT;
		        GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
			}
	    }

    }	
    
    //if (PNULL!=tmp_text)
    {
        SCI_FREE(tmp_text);
        tmp_text = PNULL;
    }
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif
	return found;
}

/*****************************************************************************/
// 	Description: update the list with search Mobile Phone's result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: the mobile phone number in text( >= "1300000" <= "13999999999" )
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithMP( uint8* number, uint32 length )
{
	// temp text storage
	uint8  temp_str[MMIFINDMP_CITYNAME_LENGTH+1] = {0};
	// the finally found city ID, used to retrieve the city strings
	uint16 city = 0;
	uint8  concat_text[GUILIST_STRING_MAX_NUM]={0};
	MMIAUD_RING_DATA_INFO_T file_info = {0};/*lint !e64*/
	uint8* ptr = PNULL;
	uint16 area_code = 0;
	uint32 offset = 0;
	uint16 index  = 0;
    BOOLEAN  res = FALSE;
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8 src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif

	strncpy((char*)temp_str, (const char*)number, length);
	// padding zeros upto MPNB_TOTAL_LENGTH, MMIAPI_FindMPLocationID need a real mobile phone number
	for( index = length; index < MPNB_TOTAL_LENGTH; index++ )
		temp_str[index] = '0';

	city = MMIAPI_FindMPLocationID( temp_str, strlen((char*)temp_str) );	
	// found a valid city ID
	if( city )
	{
		// get the city text
		SCI_MEMSET( temp_str, 0, sizeof(temp_str) );
		MMIAPI_FindMPLocationText(temp_str, city);
        if(0 == temp_str[1])//查出的城市名称有误的情况
        {
            return FALSE;
        }
        
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
        strncpy( src_file_path,R_ACC_CITYID_QUHAO_TCARD,strlen((char*)R_ACC_CITYID_QUHAO_TCARD));/*lint !e64*/
    	GetAreaCodeData(src_file_path,&file_info);
#else        
		MMI_GetDataInfo(R_ACC_CITYID_QUHAO, &file_info);
#endif
		ptr = file_info.data_ptr;
		//SCI_ASSERT( ptr != PNULL );/*assert verified*/
        if(ptr == PNULL)
        {
            return res;
        }
		offset = (city-1)*RECORDSIZE_CITYIDQUHAO;
		area_code = ptr[offset] + ptr[offset+1]*256;
		
		if( area_code )
		{
			//reset log list
			//ResetList(MMI_PHONESEARCH_RESULT);
			GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, 2, FALSE );//2
			if( is_code_zhixiashi(area_code) )
			{
				sprintf((char*)concat_text,"%s  0%d",temp_str,area_code);
				
				// append the found city & its area code
				AppendListWithGBText(concat_text, MMI_PHONESEARCH_RESULT);
			}
			else
			{
				// extract the province information
				strncpy((char*)concat_text, (const char*)temp_str, length_of_province_name(area_code));
				AppendListWithGBText(concat_text, MMI_PHONESEARCH_RESULT);
				SCI_MEMSET(concat_text, 0, sizeof(concat_text));

				sprintf((char*)concat_text,"    %s  0%d",temp_str+length_of_province_name(area_code),area_code);
				
				// append the found city & its area code
				AppendListWithGBText(concat_text, MMI_PHONESEARCH_RESULT);
			}

			res = TRUE;
		}
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
        if(PNULL != file_info.data_ptr)
        {
            SCI_FREE(file_info.data_ptr);
            file_info.data_ptr = PNULL;
        }
#endif
	}
	return res;
}

/*****************************************************************************/
// 	Description: update the list with search Area Code's result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: the area code in text( e.g: "21", "517" )
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateListWithAreaCode( const uint8* code, uint32 length )
{
	// variables to read data file
	MMIAUD_RING_DATA_INFO_T file_info = {0};/*lint !e64*/
	uint8*					ptr = PNULL;

	// temp text storage
	uint8					concat_text[GUILIST_STRING_MAX_NUM]={0};
	BOOLEAN					found = FALSE;

//	BOOLEAN					province_show = FALSE;
	uint32					index = 0;
	uint32					i = 0;
	GUILIST_ITEM_T          item_t    = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T     item_data = {0};/*lint !e64*/
	uint8	province_name[MMIFINDMP_CITYNAME_LENGTH + 1] = {0};
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8 src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
        
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_QUHAOCITY_TCARD,strlen((char*)R_ACC_QUHAOCITY_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else
	MMI_GetDataInfo(R_ACC_QUHAOCITY, &file_info);
#endif
	ptr = file_info.data_ptr;
	//SCI_ASSERT( ptr != PNULL );/*assert verified*/
    if(ptr == PNULL )
    {
        return found;
    }
    
	while( ((uint32)(ptr - file_info.data_ptr)) < file_info.data_len )
	{
		if( PNULL!=strstr((const char*)ptr,(const char*)code)
            ||!strncmp((const char*)code, (const char*)ptr,MIN(strlen((char*)ptr),length))/*lint !e666*/
            )
		{
			// match record
			if( !found )
			{
				// find 1st record
				found = TRUE;
				//SCI_MEMSET(s_phonesearch_data,0x00,sizeof(s_phonesearch_data));
				if(PNULL == s_phonesearch_data_pptr)
				{
					s_phonesearch_data_pptr = SCI_ALLOCA(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*));
					SCI_MEMSET(s_phonesearch_data_pptr,0x00,(MMIPHONESEARCH_LIST_MAX_NUM*sizeof(uint8*)));
				}
				else				
				{
					while(PNULL != s_phonesearch_data_pptr[index])
					{
						SCI_FREE(s_phonesearch_data_pptr[index]);
						index++;
					}
					index = 0;
				}
				//reset log list
				//ResetList(MMI_PHONESEARCH_RESULT);
			}

			if( strlen((char*)(ptr+RECORDSIZE_PROVINCE+RECORDSIZE_AREACODE)) )
			{
				// it's not a zhixiashi
				if( strncmp((const char*)province_name, (const char*)(ptr+RECORDSIZE_AREACODE), strlen((const char*)(ptr+RECORDSIZE_AREACODE))))
				{
					// a new province found
					SCI_MEMSET(province_name,0,sizeof(province_name));
					strncpy((char*)province_name, (const char*)ptr+RECORDSIZE_AREACODE, strlen((const char*)(ptr+RECORDSIZE_AREACODE)));
                    AppendDynamicItem(index,(char*)province_name,MMIFINDMP_CITYNAME_LENGTH);
					index++;
				}
				
				// format the output text
				SCI_MEMSET(concat_text,0,sizeof(concat_text));
				sprintf((char*)concat_text,"    %s  0%s", ptr+RECORDSIZE_PROVINCE+RECORDSIZE_AREACODE,ptr);
                AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
				index++;
			}
			else
			{
	            uint8	string[GUILIST_STRING_MAX_NUM]={0};
                int32 str_len = 0;
                
				// it's a zhixiashi
				// format the output text
				str_len = strlen((char*)ptr);
                str_len = MIN(str_len, 5);
				SCI_MEMSET(string,0,sizeof(string));
                SCI_MEMCPY(string,ptr,str_len);
                if (!strncmp((char*)string, "30886", 5)
                    ||!strncmp((char*)string, "30852", 5))
                {
                    if (strncmp((char*)code, "308", 3)
                        &&strncmp((char*)code, "30", 2)
                        &&strncmp((char*)code, "3", 1))
                    {
                        string[0]='0';
				        SCI_MEMSET(concat_text,0,sizeof(concat_text));
				        sprintf((char*)concat_text,"%s  %s", ptr+RECORDSIZE_AREACODE,string);
                    }
                    else if (!strncmp((char*)code, "30886", 5)
                        ||!strncmp((char*)code, "30852", 5)
                        )
                    {
                        string[0]='0';
				        SCI_MEMSET(concat_text,0,sizeof(concat_text));
				        sprintf((char*)concat_text,"%s  %s", ptr+RECORDSIZE_AREACODE,string);
                    }
                    else
                    {
		                // go to the next record
		                ptr += RECORDSIZE_AREACODECITY;
                        if (0 == index)
                        {
                            found = 0;
                        }
                        continue;
                    }
                }
                else if (!strncmp((char*)string, "30853", 5))
                {
                    if (strncmp((char*)code, "30", 2))
                    {
                        string[0]='0';
				        SCI_MEMSET(concat_text,0,sizeof(concat_text));
				        sprintf((char*)concat_text,"%s  %s", ptr+RECORDSIZE_AREACODE,string);
                    }
                    else if (!strncmp((char*)code, "30853", 5))
                    {
                        string[0]='0';
				        SCI_MEMSET(concat_text,0,sizeof(concat_text));
				        sprintf((char*)concat_text,"%s  %s", ptr+RECORDSIZE_AREACODE,string);
                    }
                    else
                    {
		                // go to the next record
		                ptr += RECORDSIZE_AREACODECITY;
                        if (0 == index)
                        {
                            found = 0;
                        }
                        continue;
                    }
                }
                else
                {
				    SCI_MEMSET(concat_text,0,sizeof(concat_text));
				    sprintf((char*)concat_text,"%s  0%s", ptr+RECORDSIZE_AREACODE,ptr);
                }
                AppendDynamicItem(index,(char*)concat_text,GUILIST_STRING_MAX_NUM);
				index++;
			}
		}

		// go to the next record
		ptr += RECORDSIZE_AREACODECITY;
	}

    if ( 0<index )//有查询结果才重置guilist
    {
        found = TRUE;
        GUILIST_SetMaxItem(MMIPHONESEARCH_MAIN_LIST_ID, index, TRUE );
        s_current_status = MMI_PHONESEARCH_RESULT;
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
        item_t.item_data_ptr = &item_data;
        for (i = 0; i<index; i++)
        {
    	    GUILIST_AppendItem(MMIPHONESEARCH_MAIN_LIST_ID, &item_t);
        }
    }
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif
	return found;
}

/*****************************************************************************/
// 	Description: update the list with search result
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: text_ucs2, the char in UCS2/GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL BOOLEAN UpdateList(wchar* text, uint16 length)
{
    //uint8 text_gb[MMIFINDMP_CITYNAME_LENGTH]={0};
    uint8   index = 0;
    uint8   offset = 0;
    BOOLEAN is_digits = TRUE;
    BOOLEAN recode = FALSE;
    uint8 *                  char_text = PNULL;

    // check if the text is all digits
    for( index = 0; index < length; index++ )
    {
        if( ( text[index] < '0') || ( text[index] > '9') )
        {
            is_digits = FALSE;
            break;
        }
    }

    if( is_digits )
    {
        char_text = (uint8 *)SCI_ALLOC_APPZ(length+1);
        //SCI_ASSERT(PNULL != char_text);/*assert verified*/
        SCI_MEMSET(char_text,0,(length+1));
        MMI_WSTRNTOSTR(char_text, length, text, length, length);
    
        offset =0;
        if (!strncmp((char*)char_text, "00886", 5)
            ||!strncmp((char*)char_text, "00853", 5)
            ||!strncmp((char*)char_text, "00852", 5)
            )
        {
            char_text[0] = '3'; //香港30852/澳门30853/台湾30886
        }
        else
        {
            // "13" is the beginning of a mobile phone number

            for (index = 0; index < length; index++)
            {
               if (char_text[index] == '0')
               {
                   length--;
                   offset++;
               }
               else
               {
                   break;
               }
            }
        }
    
       if(strncmp((char*)char_text, "13", 2) && 
          strncmp((char*)char_text, "14", 2) &&
          strncmp((char*)char_text, "15", 2) &&
          strncmp((char*)char_text, "18", 2))
        {
            // it is not a mobile phone number, check if it is a QUHAO
           recode = UpdateListWithAreaCode(&char_text[offset], length);
        }
        else
        {
            // it is a mobile phone number
            if( ( strlen((char*)char_text) >= MPNB_USE_LENGTH ) && ( strlen((char*)char_text) <= MPNB_TOTAL_LENGTH ) )
           {
               recode = UpdateListWithMP(&char_text[offset], length);
           }
        }
        SCI_FREE(char_text);
        char_text = PNULL; 
    }
    else
    {
            recode = UpdateListWithCity(text,length);
    }

 
    return (recode);
}

/*****************************************************************************/
// 	Description : to append the info to list_box
//	Global resource dependence : 
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
LOCAL void GetListCurrentItemContent(
                                     MMI_CTRL_ID_T ctrl_id,
                                    // BOOLEAN *is_ucs2_ptr,
                                     uint16 *str_len_ptr,
                                     wchar *str_ptr
                                     )
{
    uint16                     current_index = GUILIST_GetCurItemIndex( ctrl_id );
    const GUILIST_ITEM_T*  item_ptr      = GUILIST_GetItemPtrByIndex( ctrl_id, current_index );
    
    //SCI_ASSERT( PNULL != str_len_ptr );/*assert verified*/
    //SCI_ASSERT( PNULL != str_ptr );/*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr);/*assert verified*/
    if(PNULL == str_len_ptr ||PNULL == str_ptr || PNULL == item_ptr)
    {
        return;
    }
    
    if (s_list_grade.index == SEEK_INVALID_INDEX)
    {
        s_list_grade.index = current_index;
        s_list_grade.code_grade = SEEK_PROV_CODE;
    }
    if( GUIITEM_STYLE_ONE_LINE_TEXT == item_ptr->item_style 
        && PNULL != item_ptr->item_data_ptr )
    {
        if( GUIITEM_DATA_TEXT_BUFFER == item_ptr->item_data_ptr->item_content[0].item_data_type )
        {
          //  *is_ucs2_ptr = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.is_ucs2;
            *str_len_ptr = item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len;
            MMI_WSTRNCPY( str_ptr, *str_len_ptr,
                item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_ptr,
                item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len,
                item_ptr->item_data_ptr->item_content[0].item_data.text_buffer.wstr_len);
        }
    }   
}

/*****************************************************************************/
//  Description : left softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainLeftSK(void)
{
//	BOOLEAN	is_ucs2		= FALSE;
	uint16	length		= 0;
	wchar	string[GUILIST_STRING_MAX_NUM]={0};
//	uint8	text_gb[MMIFINDMP_CITYNAME_LENGTH]={0};
	//MMI_WINDOW_T*   window_ptr = NULL;
	
    if (MMI_PHONESEARCH_RESULT != s_phonesearch_cur_ui)
    {
	    GetListCurrentItemContent(MMIPHONESEARCH_MAIN_LIST_ID,
					    &length, string );

	    if( length > 0)
	    {
	    
		    if(UpdateListWithProvince(string,length))
		    {
			    // found some match cities or province
			    s_phonesearch_cur_ui = MMI_PHONESEARCH_RESULT;

			    //window_ptr = MMK_GetWinPtrBaseWinId(MMIPHONESEARCH_MAIN_WIN_ID);
			    //if (NULL != window_ptr)
                if( MMK_IsOpenWin( MMIPHONESEARCH_MAIN_WIN_ID ) )
			    {
				    // update the wnd title
				    GUIWIN_SetTitleTextId(MMIPHONESEARCH_MAIN_WIN_ID,TXT_SEARCHPHONE_RESULT, FALSE);
				    
				    // disable the mid button
				    //MMK_SetWinSKBButton(MMIPHONESEARCH_MAIN_WIN_ID, MMI_SOFTKEYBAR_SECOND_ICON, IMAGE_PHONESEARCH_INPUT, 
				    //			IMAGE_PHONESEARCH_INPUT_PRESSED, NULL, FALSE);

				    // just redraw the window
				    MMK_PostMsg(MMIPHONESEARCH_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
			    }
		    }
	    }
    }

	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : right softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainRightSK(void)
{
	if( MMI_PHONESEARCH_RESULT == s_phonesearch_cur_ui )
	{
		// back from result wnd to main wnd
		InitMainWnd();
		s_phonesearch_cur_ui = MMI_PHONESEARCH_MAIN;
		
		// redraw the main wnd
		MMK_PostMsg(MMIPHONESEARCH_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
	}
	else if( MMI_PHONESEARCH_MAIN == s_phonesearch_cur_ui )
	{
		// exit
		MMK_CloseWin(MMIPHONESEARCH_MAIN_WIN_ID);
	}
	
	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : mid softkey handling of main wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainMidSK(void)
{
	// enter the input wnd
	MMK_CreateWin((uint32 *)MMIPHONESEARCH_INPUT_WIN_TAB,PNULL);
	
	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : main window message handling
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchMainWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;   
	int32 i = 0;
	
    switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
            s_list_grade.code_grade = SEEK_CODE_MAX;
            s_list_grade.index = SEEK_INVALID_INDEX;
			InitMainWnd();
			s_phonesearch_cur_ui = MMI_PHONESEARCH_MAIN;
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
			break;
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
			uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            //SCI_ASSERT( PNULL != need_item_data_ptr );/*assert verified*/
            if(PNULL == need_item_data_ptr)
            {
                break;
            }
            index = need_item_data_ptr->item_index;
            if ((PNULL != s_phonesearch_data_pptr)
                && (PNULL != s_phonesearch_data_pptr[index]))
            {
			    AppendPhoneSearchListTextData(s_phonesearch_data_pptr[index],index);
            }
		}
		break;
		case MSG_FULL_PAINT:
            if(s_phonesearch_cur_ui == MMI_PHONESEARCH_RESULT)
            {
#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(MMIPHONESEARCH_MAIN_WIN_ID, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
            else
            {
#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            }
			break;

		case MSG_APP_GREEN:
			break;

		case MSG_CTL_OK:
        case MSG_APP_OK:
#if defined MMI_PDA_SUPPORT
        case MSG_APP_MENU:
#endif
            if (MMI_PHONESEARCH_RESULT != s_phonesearch_cur_ui)
            {
                MMK_CreateWin((uint32 *)MMIPHONESEARCH_POP_QUERY_WIN_TAB,PNULL);
            }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
			HandlePhoneSearchMainLeftSK();
			break;
            
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD          
        case MSG_GET_FOCUS://网络下载结束时加载数据
            if(MMI_PHONESEARCH_MAIN == s_phonesearch_cur_ui)
            {
    			InitMainWnd();
            }
			break;
#endif

		case MSG_APP_WEB:
		case MSG_CTL_MIDSK:
    		{
                uint16          current_index = 0;
    		     
                if (MMI_PHONESEARCH_RESULT != s_phonesearch_cur_ui)
                {
                    current_index = GUILIST_GetCurItemIndex( MMIPHONESEARCH_MAIN_LIST_ID );

                    if (s_list_grade.index == SEEK_INVALID_INDEX)
                    {
                        s_list_grade.index = current_index;
                        s_list_grade.code_grade = SEEK_PROV_CODE;
                    }
    			    HandlePhoneSearchMainMidSK();
                }
            }
			break;


        case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
			HandlePhoneSearchMainRightSK();
			break;
       
		case MSG_CLOSE_WINDOW:
			s_phonesearch_cur_ui = MMI_PHONESEARCH_NONE;
			i = 0;
            if (PNULL != s_phonesearch_data_pptr)
            {
                while(PNULL != s_phonesearch_data_pptr[i])
			    {
				    SCI_FREE(s_phonesearch_data_pptr[i]);
				    i++;
			    }
				SCI_FREE(s_phonesearch_data_pptr);
				s_phonesearch_data_pptr = PNULL;
            }
			break;

		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}

/*****************************************************************************/
//  Description : left softkey handling of input wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputLeftSK(void)
{
	//MMI_WINDOW_T*   window_ptr = NULL;
	MMI_STRING_T str = {0};
	
	GUIEDIT_GetString(MMIPHONESEARCH_INPUT_EDIT_ID, &str);

	if( str.wstr_len == 0 )	// invalid input
	{
		MMIPUB_OpenAlertWarningWin(TXT_SEARCHPHONE_INPUT);
	}
	else 
	{
		if(UpdateList(str.wstr_ptr,str.wstr_len)) // found some match records
		{
			MMK_CloseWin(MMIPHONESEARCH_INPUT_WIN_ID);
				
			s_phonesearch_cur_ui = MMI_PHONESEARCH_RESULT;

			// update the window title and redraw it
			//window_ptr = MMK_GetWinPtrBaseWinId(MMIPHONESEARCH_MAIN_WIN_ID);
			//if (NULL != window_ptr)
            if( MMK_IsOpenWin( MMIPHONESEARCH_MAIN_WIN_ID ) )
			{
				// update the wnd title
				GUIWIN_SetTitleTextId(MMIPHONESEARCH_MAIN_WIN_ID,TXT_SEARCHPHONE_RESULT, FALSE);
                // just redraw the window
				MMK_PostMsg(MMIPHONESEARCH_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
			}
		}
		else	// nothing found
		{
		 //for cr116166
		 //	MMIPUB_OpenAlertFailWin(TXT_ERROR);
		 MMIPUB_OpenAlertWarningWin(TXT_WITHOUT_RESULT);
		}
	}

	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : right softkey handling of input wnd
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputRightSK(void)
{
	MMK_CloseWin(MMIPHONESEARCH_INPUT_WIN_ID);
	s_phonesearch_cur_ui = MMI_PHONESEARCH_MAIN;

	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// 	Description: initialize the input wnd
//	Global resource dependence: 
//  Author: zhenhao.huang
//	Note: 
/*****************************************************************************/
LOCAL void InitInputWnd()
{
	// initialize the text edit
	MMK_SetAtvCtrl( MMIPHONESEARCH_INPUT_WIN_ID,  MMIPHONESEARCH_INPUT_EDIT_ID);

	s_phonesearch_cur_ui = MMI_PHONESEARCH_INPUT;
}

/*****************************************************************************/
//  Description : input window message handling
//  Global resource dependence :
//  Author: zhenhao.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchInputWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
	MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
            GUIEDIT_SetIm(MMIPHONESEARCH_INPUT_EDIT_ID,GUIIM_TYPE_SET_ALL,GUIIM_TYPE_SMART);/*lint !e655*/
			InitInputWnd();
			break;
		
		case MSG_FULL_PAINT:
			break;

		case MSG_APP_GREEN:
			break;

#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_CTL_MIDSK:
			HandlePhoneSearchInputLeftSK();
			break;    
		
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            if (PNULL != param)
            {
                uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
                
                switch (src_id)
                {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                    HandlePhoneSearchInputLeftSK();
                    break;                

                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    HandlePhoneSearchInputRightSK();
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
			HandlePhoneSearchInputRightSK();
			break;

		case MSG_CLOSE_WINDOW:
#ifdef MMI_PDA_SUPPORT
            MMK_CloseWin(MMIPHONESEARCH_POP_QUERY_WIN_ID);
#endif
			break;
			
		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	return recode;
}

/*****************************************************************************/
// 	Description : open the wnd of phone search
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONESEARCH_OpenWnd(void)
{
	MMK_CreateWin((uint32 *)MMIPHONESEARCH_MAIN_WIN_TAB,PNULL);
}

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//  Mender: Mingxuan.Zhu
//	Note: return length of location_str, if not found return 0
//        this funciton is invoked by atest, which is added a output param named phone_location.
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindPhoneLocation(	
                                       uint8*   tel_num_str,	// input phone number string in ASCII
                                       int32    tel_num_length, // string length
                                       uint16*  location_str,	// output string, length is MMIFINDMP_CITYNAME_LENGTH
                                       char*    phone_location  // get the name of the phone location.
                                      )
{
	uint8	area_code_length = 0;
	uint16	return_length = 0;	
	// variables to read data file
	MMIAUD_RING_DATA_INFO_T file_info = {0};/*lint !e64*/
	uint8*					ptr = PNULL;
	// temp text storage
	uint8					concat_text[GUILIST_STRING_MAX_NUM]={0};
	uint16					concat_text_len = 0;//@cr113008,对内存覆盖隐患加上保护
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD	
	uint8 src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
	// judge the area code length according to the first digits
	if( ( tel_num_str[0] == '1' ) || ( tel_num_str[0] == '2' ) )
	{
		// area code of 2 digits
		area_code_length = 2;
	}
	else if( strncmp( (const char*)tel_num_str, "80", 2) == 0 )
	{
		// area code of 4 digits
		area_code_length = 4;
	}
    else if( strncmp( (const char*)tel_num_str, "3085", 4) == 0 
             ||strncmp( (const char*)tel_num_str, "3088", 4) == 0)
    {
        area_code_length = 5;
    }
	else
	{
		// area code of 3 digits
		area_code_length = 3;
	}
        
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_QUHAOCITY_TCARD,strlen((char*)R_ACC_QUHAOCITY_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else
	MMI_GetDataInfo(R_ACC_QUHAOCITY, &file_info);
#endif
	ptr = file_info.data_ptr;

    if (PNULL == ptr)
    {
        //MMI_ASSERT_WARNING("ptr != PNULL");/*assert verified*/
        return return_length;
    }
    
    while( ((uint32)(ptr - file_info.data_ptr)) < file_info.data_len )
    {
        if( !strncmp((const char*)tel_num_str, (const char*)ptr, area_code_length) )
        {
            // match record
            if( strlen((const char*)(ptr+RECORDSIZE_PROVINCE+RECORDSIZE_AREACODE)) )
            {
                // it's not a zhixiashi, we need to get the province and city name
                // format the output text
                sprintf((char*)concat_text,"%s%s", ptr+RECORDSIZE_AREACODE,ptr+RECORDSIZE_PROVINCE+RECORDSIZE_AREACODE);
            }
            else
            {
                // it's a zhixiashi
                // format the output text
                sprintf((char*)concat_text,"%s", ptr+RECORDSIZE_AREACODE);
            }
            
            //@cr113008,对内存覆盖隐患加上保护
            concat_text_len = strlen((char*)concat_text);
            concat_text_len = (uint16)MIN(MMIFINDMP_CITYNAME_LENGTH, concat_text_len);
            return_length += GUI_GBToWstr((uint16*)location_str, concat_text, concat_text_len);
            //@cr113008 end
#ifdef _ATEST_
#ifdef WIN32
            SCI_STRCPY(phone_location,(char*)concat_text);  // get the name of the phone location. - added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_ 
            break;
        }
        else
        {        
            // go to the next record
            ptr += RECORDSIZE_AREACODECITY;
        }
    }
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif    
    return return_length;
}

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note: return length of location_str, if not found return 0
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONESEARCH_FindLocation(	
								uint8* tel_num_str,		// input phone number string in ASCII
								int32  tel_num_length,  // string length
								uint16* location_str	// output string, length is MMIFINDMP_CITYNAME_LENGTH
								)
{
	uint32 country_code_length = 0;
    int32  valid_num = 0;
#ifdef _ATEST_
#ifdef WIN32
    char tel_location[PHONE_LOCATION_TOTAL_NAME_LENGTH] = {0}; // the temp variable for ATEST
#else
    char tel_location[1] = {0}; // the temp variable for NO ATEST
#endif//WIN32
#else
    char tel_location[1] = {0}; // the temp variable for NO ATEST
#endif//_ATEST_

	if( tel_num_str && tel_num_length && (location_str != PNULL) )
	{
		// check if it begins with country code
		if( strncmp((const char*)tel_num_str, "86", 2) == 0 &&tel_num_length>8)
		{
			country_code_length = 2;
		}
		else if( strncmp((const char*)tel_num_str, "+86", 3) == 0 )
		{
			country_code_length = 3;
		}
		else if( strncmp((const char*)tel_num_str, "0086", 4) == 0 )
		{
			country_code_length = 4;
		}
        //International call dailed with "00" or "+",code "86" is China international code.
        else if(((strncmp((const char*)tel_num_str, "00", 2) == 0)&&(strncmp((const char*)tel_num_str, "0086", 4) != 0))
                ||((strncmp((const char*)tel_num_str, "+", 1) == 0)&&(strncmp((const char*)tel_num_str, "+86", 3) != 0)))
        {
            if ((strncmp((const char*)tel_num_str, "0085", 4) == 0)||
                (strncmp((const char*)tel_num_str, "0088", 4) == 0))
            {
                tel_num_str[0]='3';
                return MMIAPI_FindPhoneLocation(tel_num_str, tel_num_length, location_str, tel_location);
            }
            else
            {
                return 0;
            }
        }

		// skip the country code
		tel_num_str += country_code_length;
		tel_num_length -= (int32)country_code_length;
		if((strncmp((const char*)tel_num_str, "13", 2) == 0) ||
			(strncmp((const char*)tel_num_str, "14", 2) == 0)|| 
            (strncmp((const char*)tel_num_str, "15", 2) == 0)|| 
            (strncmp((const char*)tel_num_str, "18", 2) == 0))
		{
			// it is a mobile phone number
            return MMIAPI_FindMPLocation(tel_num_str, tel_num_length, location_str,tel_location/*LocationTest - ATEST*/);
		}
		if((strncmp((const char*)tel_num_str, "013", 3) == 0) ||
			( strncmp((const char*)tel_num_str, "014", 3) == 0)|| 
            ( strncmp((const char*)tel_num_str, "015", 3) == 0)||
            ( strncmp((const char*)tel_num_str, "018", 3) == 0))
		{
			// it is a DDD mobile phone number
			tel_num_str++;
			tel_num_length--;
			
            return MMIAPI_FindMPLocation(tel_num_str, tel_num_length, location_str,tel_location/*LocationTest - ATEST*/);
		}
		else
		{
			// if the area code begin with "0", skip it
            if((strcmp((const char*)tel_num_str, "08") != 0) &&( country_code_length == 0 ) && ( tel_num_str[0] == '0' ) )
			{
				// a DDD number
				tel_num_str++;
				tel_num_length--;
                if(tel_num_length > SEEK_ZONE_CODE + MMIPHONESEARCH_LOCAL_PHONE_MAX_LENGTH)//number is a long-distance telephone and number length is longer than long-distance telephone number
                {
                    return 0;
                }
                else
                {
                    return MMIAPI_FindPhoneLocation(tel_num_str, tel_num_length, location_str,tel_location/*LocationTest - ATEST*/);
                }				
			}
			else if( country_code_length )
			{
                if (tel_num_str[0] == '0')
                {
				    // a DDD number
				    tel_num_str++;
				    tel_num_length--;
                }
				// a IDD number
                return MMIAPI_FindPhoneLocation(tel_num_str, tel_num_length, location_str,tel_location/*LocationTest - ATEST*/);
			}
            else if(tel_num_length > MMIPHONESEARCH_LOCAL_PHONE_MAX_LENGTH)//number is not a long-distance telephone and number length is longer than local phone number 
            {
                for (valid_num=0; valid_num<tel_num_length; valid_num++)
                {
                    if(tel_num_str[valid_num]<'0'||tel_num_str[valid_num]>'9')
                    {
                        break;
                    }
                }
                if (valid_num <= MMIPHONESEARCH_LOCAL_PHONE_MAX_LENGTH)
                {
				    // a local number
				    return MMIPHONESEARCH_LOCALNUMBER;
                }
                else
                {
                    return 0;
                }
            }
            else if(tel_num_str[0]<'0'||tel_num_str[0]>'9')
            {
                return 0;
            }
            else
            {
                    // a local number
                return MMIPHONESEARCH_LOCALNUMBER;
            }
		}
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************/
// 	Description: alloc and add src to s_phonesearch_data_pptr[i]
//	Global resource dependence: 
//  Author: zhaohui
//	Note: text, the char in GB with buffer size of MMIFINDMP_CITYNAME_LENGTH
//  return TRUE if found 
/*****************************************************************************/
LOCAL void AppendDynamicItem(int index, char* src, uint16 src_max_length)
{
    uint16 str_len = 0;

    if (index >= 0 && src != PNULL)
    {
        if (index<MMIPHONESEARCH_LIST_MAX_NUM)
        {
                if (PNULL ==s_phonesearch_data_pptr[index])
            {
                s_phonesearch_data_pptr[index] = SCI_ALLOCA(sizeof(uint8)*(GUILIST_STRING_MAX_NUM+1));
            }
    
            str_len = MIN(src_max_length, GUILIST_STRING_MAX_NUM);
            SCI_MEMSET(s_phonesearch_data_pptr[index],0x00,(GUILIST_STRING_MAX_NUM+1));
            MMI_MEMCPY(s_phonesearch_data_pptr[index],str_len,src,str_len,str_len);
        }
    }
}
/*****************************************************************************/
//  Description : HandlePhoneSearchQueryWinMsg
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePhoneSearchQueryWinMsg(
                                          MMI_WIN_ID_T      win_id,     
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    uint16           current_index = 0;
    uint32           menu_id       = 0;
    MMI_MENU_GROUP_ID_T group_id   = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMIPHONESEARCH_MENU_CTRL_ID);
            break;
        
        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            //menu_id = GUIMENU_GetCurNodeId(MMIPHONESEARCH_POP_MENU_ID); 
            GUIMENU_GetId(MMIPHONESEARCH_MENU_CTRL_ID,&group_id,&menu_id);
            
            switch(menu_id)
            {
                case MMI_PHONESEARCH_POP_QUERY_MENU_ID:
                    if (MMI_PHONESEARCH_RESULT != s_phonesearch_cur_ui)
                    {                
                        current_index = GUILIST_GetCurItemIndex( MMIPHONESEARCH_MAIN_LIST_ID );

                        if (s_list_grade.index == SEEK_INVALID_INDEX)
                        {
                            s_list_grade.index = current_index;
                            s_list_grade.code_grade = SEEK_PROV_CODE;
                        }
        			    HandlePhoneSearchMainMidSK();
                    }
                    break;
#if defined(BROWSER_SUPPORT) && defined(HOME_LOCATION_INQUIRE_SUPPORT_TCARD)
                  case MMI_PHONESEARCH_POP_DOWNLOAD_MENU_ID:
                    {
                        if(!MMISD_IsSDExistEx())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_INSERT_MEMORYCARD);
                        }
                        else
                        {
                            OpenDownLoadUrl();
                        }                        
                    }
                    break;
#endif                    
                  default:
                    break;
            }
            MMK_CloseWin(win_id);
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        
        default:
            result = MMI_RESULT_FALSE;
            break;
            
    }

    return result;
}
#if defined(BROWSER_SUPPORT) && defined(HOME_LOCATION_INQUIRE_SUPPORT_TCARD)
/*******************************************************************/
//  Interface:		OpenDownLoadUrl
//  Description : 	OpenDownLoadUrl
//  Global resource dependence : 
//  Author:
//  Note:
/*******************************************************************/
LOCAL void OpenDownLoadUrl(void)
{
	uint8 *url_ptr = PNULL;
	MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
	uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(LOCATION_FILE_DOWNLOAD_URL) * 3 + 1);
	
	url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));	
	if (PNULL != url_ptr)
	{
		SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));		
		GUI_WstrToUTF8(url_ptr, str_len, LOCATION_FILE_DOWNLOAD_URL, MMIAPICOM_Wstrlen(LOCATION_FILE_DOWNLOAD_URL));
		
		entry_param.type = MMIBROWSER_ACCESS_URL;
		entry_param.dual_sys = MN_DUAL_SYS_MAX;
		entry_param.url_ptr = (char *)url_ptr;
		entry_param.user_agent_ptr = PNULL;
		
		MMIAPIBROWSER_Entry(&entry_param);
        
        SCI_FREE(url_ptr);
        url_ptr = PNULL;
	}
}
/*******************************************************************/
//  Interface:		HandlePHONESEARCHDownloadCB
//  Description : 	HandlePHONESEARCHDownloadCB
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
LOCAL void HandlePHONESEARCHDownloadCB(
								   MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr
								   )
{    
    if (PNULL != param_ptr)
    {
    	SCI_TRACE_LOW("HandlePHONESEARCHDownloadCB enter param_ptr->mime_type %d",param_ptr->mime_type);
    }

    if (PNULL != param_ptr
		&& MMIBROWSER_MIME_TYPE_PNL == param_ptr->mime_type)
    {
        //SCI_TRACE_LOW:"HandlePHONESEARCHDownloadCB enter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPHONESEARCH_WINTAB_1972_112_2_18_2_1_55_83,(uint8*)"");
        MoveFileToLocation(param_ptr->file_name);
    }
}
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
/*******************************************************************/
//  Description : 	MMIAPIPHONESEARCH_RegBrowserCB
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIPHONESEARCH_RegBrowserCB(void)
{
#ifdef BROWSER_SUPPORT    
	MMIBROWSER_MIME_DOWNLOAD_PARAM_T pnl_callback = {0};

	pnl_callback.call_back = HandlePHONESEARCHDownloadCB;	
	pnl_callback.mime_type = MMIBROWSER_MIME_TYPE_PNL;
	MMIAPIBROWSER_RegMimeFunc(&pnl_callback);
#endif
}
/*******************************************************************/
//  Interface:		MoveFileToLocation
//  Description : 	网络下载的数据优先存储在T卡,相同文件名可以覆盖。
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
LOCAL void MoveFileToLocation(const wchar *file_path)
{
    wchar *dest_file_path_ptr = PNULL;
    //wchar *dest_device_path_ptr = PNULL;
    wchar   file_name[MMIFILE_FILE_NAME_MAX_LEN+1]   = {0};
    uint16  file_name_len = 0;
    uint16  file_path_len = 0;
    uint32  file_size = 0;
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    
    if(PNULL == file_path)
        return;

    dest_file_path_ptr = (wchar *)SCI_ALLOCAZ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));
    if(PNULL == dest_file_path_ptr)
        return;
    //SCI_MEMSET(dest_file_path_ptr,0x00,(MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));
    
    file_path_len = MMIAPICOM_Wstrlen(file_path);
    MMIAPIFMM_SplitFullPath(file_path, file_path_len,PNULL, PNULL, PNULL, PNULL, 
                                    file_name, &file_name_len); 
    MMIAPIFMM_GetFileInfo(dest_file_path_ptr,file_path_len,&file_size,PNULL,PNULL);
    
    if(GetValidLocationFileDir(dest_file_path_ptr,file_name,file_size))
    {
        if(MMIAPICOM_CompareTwoWstrExt(file_path,file_path_len,dest_file_path_ptr,MMIAPICOM_Wstrlen(dest_file_path_ptr),FALSE))
	    {//只有res和des的path不相同，才需要copy
	        sfs_error = MMIAPIFMM_CopyFile(file_path, dest_file_path_ptr);
	        if (SFS_NO_ERROR == sfs_error)
	        {
	            sfs_error = MMIAPIFMM_DeleteFile(file_path, NULL);
	        }
	        else if (SFS_ERROR_NO_SPACE == sfs_error)
	        {
	           MMIPUB_OpenAlertWarningWin( TXT_NO_SPACE);
	        }
	        else
	        {
	           MMIPUB_OpenAlertWarningWin( TXT_FILE_SAVEFAILED);
	        }
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
    }
    //if(PNULL != dest_file_path_ptr)
    SCI_FREE(dest_file_path_ptr);
    
}
/*****************************************************************************/
//  Description : get location file dir
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidLocationFileDir(uint16* device_dir,const wchar *file_name_ptr,uint32 file_size)
{
    MMIFILE_DEVICE_E file_dev_type = MMI_DEVICE_SYSTEM;
    BOOLEAN          result = TRUE;

    file_dev_type = MMIAPIFMM_GetDefaultDisk();
    if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED, file_dev_type, file_size, PNULL))
    {
        switch (file_dev_type)
        {
            case MMI_DEVICE_UDISK:
                MMIAPICOM_Wstrncpy(device_dir,LOCATION_FILE_UDISK_PATH,MMIAPICOM_Wstrlen(LOCATION_FILE_UDISK_PATH));
                break;
            case MMI_DEVICE_SDCARD:
                MMIAPICOM_Wstrncpy(device_dir,LOCATION_FILE_TCARD_PATH,MMIAPICOM_Wstrlen(LOCATION_FILE_TCARD_PATH));
                break;
            case MMI_DEVICE_SDCARD_1:
                MMIAPICOM_Wstrncpy(device_dir,LOCATION_FILE_TCARD1_PATH,MMIAPICOM_Wstrlen(LOCATION_FILE_TCARD1_PATH));
                break;
            default:
                MMIAPICOM_Wstrncpy(device_dir,LOCATION_FILE_UDISK_PATH,MMIAPICOM_Wstrlen(LOCATION_FILE_UDISK_PATH));
                break;
        }
        MMIAPICOM_Wstrcat(device_dir, file_name_ptr);
    }
    else
    {
        result = FALSE;
    }
    return result;
}
#endif
#else//!HOME_LOCATION_INQUIRE_SUPPORT
/*****************************************************************************/
// 	Description : open the wnd of phone search
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONESEARCH_OpenWnd(void)
{

}

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhaohui
//	Note: return 0
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONESEARCH_FindLocation(	
								uint8* tel_num_str,		// input phone number string in ASCII
								int32  tel_num_length,  // string length
								uint16* location_str	// output string, length is MMIFINDMP_CITYNAME_LENGTH
								)
{
    return 0;
}
/*******************************************************************/
//  Description : 	MMIAPIPHONESEARCH_RegBrowserCB
//  Global resource dependence : 
//  Author: 
//  Note:
/*******************************************************************/
PUBLIC void MMIAPIPHONESEARCH_RegBrowserCB(void)
{

}

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//  Mender: Mingxuan.Zhu
//	Note: return length of location_str, if not found return 0
//        this funciton is invoked by atest, which is added a output param named phone_location.
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindPhoneLocation(	
                                       uint8*   tel_num_str,	// input phone number string in ASCII
                                       int32    tel_num_length, // string length
                                       uint16*  location_str,	// output string, length is MMIFINDMP_CITYNAME_LENGTH
                                       char*    phone_location  // get the name of the phone location.
                                      )
{
    return 0;
}
#endif


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527

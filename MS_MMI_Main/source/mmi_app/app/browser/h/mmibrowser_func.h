/*****************************************************************************
** File Name:      mmibrowser_func.h                                     *
** Author:          fen.xie                                                  *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#ifndef BROWSER_FUNC_H
#define BROWSER_FUNC_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "brw_control_api.h"
#include "mmipub.h"
#include "mmi_signal_ext.h"
#include "mmibrowser_bookmark.h"
#include "browser_manager_text.h"

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
#define MMIBRW_DIR  ""
#define MMIBRW_DIR_LEN  0
#define MMIBROWSER_FILENAME_EXT_MAX_LEN  (5)
#define MMIBROWSER_TMPFILE_EXT   ".tmp"
#define MMIBROWSER_HTTP_STRING                  "http://"

#ifdef MMIDORADO_FIX_NAV_SUPPORT
//Fix Navigation--can't be modified by user:热门精品Being
#define MMIBRW_FIX_NAV_MAX_ITEM    8
typedef struct _MMIBRW_FIX_NAV_ITEM_T
{
    MMI_TEXT_ID_T   text_id;//the title of the item
    MMI_IMAGE_ID_T  img_id;//the image of the item
    char    *url_ptr;//the url content of the item
}MMIBRW_FIX_NAV_ITEM_T;

typedef struct 
{
	uint32  item_count;
	MMIBRW_FIX_NAV_ITEM_T	item_info[MMIBRW_FIX_NAV_MAX_ITEM];
}MMIBRW_FIX_NAV_CONTEXT_T;
//Fix Navigation:End
#else
#define MMIBRW_FIX_NAV_MAX_ITEM    0
#endif

#ifdef MMIDORADO_MY_NAV_SUPPORT
//my navigation:我的导航begin
#define MMIBRW_MY_NAV_MAX_ITEM			8
//#define MMIBRW_MY_NAV_DEFAULT_MAX_ITEM			2//< MMIBRW_MY_NAV_MAX_ITEM
#define MMIBRW_MY_NAV_MAX_URL_LENGTH   256//MMIBRW_BOOKMARK_MAX_URL_LENGTH
//#define MMIBRW_MY_NAV_MAX_FILE_NAME_LEN   30//only the file name(without path and suffix)
#define MMIBRW_MY_NAV_DEFAULT_IMAGE_ID IMAGE_BROWSER_DEFAULT_FAVORITE_ICON

typedef enum
{
    MMIBRW_MY_NAV_ITEM_TYPE_NONE,//invalid
    MMIBRW_MY_NAV_ITEM_TYPE_FIXED,//default
    MMIBRW_MY_NAV_ITEM_TYPE_CUSTOMIZED,//customized(add by user)
    MMIBRW_MY_NAV_ITEM_TYPE_MAX
}MMIBRW_MY_NAV_ITEM_TYPE_E;

typedef struct _MMIBRW_MY_NAV_ITEM_T
{
    MMIBRW_MY_NAV_ITEM_TYPE_E   item_type;
    union
    {
        wchar  title_warr[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH+1];
        MMI_TEXT_ID_T  title_text_id;
    }title_info_u;
    MMI_IMAGE_ID_T  img_id;
#if 0    
    union
    {
        wchar  img_file_name[MMIBRW_MY_NAV_MAX_FILE_NAME_LEN+1];//only the file name(withou path and suffix)
        MMI_IMAGE_ID_T  img_id;
    }img_info_u;
#endif
    char    url_arr[MMIBRW_MY_NAV_MAX_URL_LENGTH+1];
}MMIBRW_MY_NAV_ITEM_T;

typedef struct 
{
	//uint32  item_count;
	MMIBRW_MY_NAV_ITEM_T	item_info[MMIBRW_MY_NAV_MAX_ITEM];
}MMIBRW_MY_NAV_CONTEXT_T;

typedef struct _MMIBRW_MY_NAV_DEFAULT_ITEM_T
{
    MMI_TEXT_ID_T  title_text_id;
    char    *url_ptr;
    MMI_IMAGE_ID_T  img_id;
}MMIBRW_MY_NAV_DEFAULT_ITEM_T;

//my navigation:我的导航end
#else
#define MMIBRW_MY_NAV_MAX_ITEM			0
#endif

typedef enum
{
    MMIBROWSER_FIX_DISPLAY_TYPE_SNAPSHOT,
    MMIBROWSER_FIX_DISPLAY_TYPE_PAGE,
    MMIBROWSER_FIX_DISPLAY_TYPE_MAX
}MMIBROWSER_FIX_DISPLAY_TYPE_E;

typedef struct
{
    BOOLEAN is_content_valid;//内容是否有效
    BOOLEAN is_need_display;//TRUE:BRW_DisplayPage;FALSE:BRW_RepaintPage
    MMIBROWSER_FIX_DISPLAY_TYPE_E  display_type;
    uint32  scroll_handle;
}MMIBROWSER_FIX_DISPLAY_T;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef void (*MMIBRW_OpenFileCallBack)(BOOLEAN is_success, FILEARRAY file_array);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : enter browser :main menu, homepage, url........
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_Enter(void);
 
/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_Init(void);

/*****************************************************************************/
//  Description : the exit of main browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_Exit(void);

/*****************************************************************************/
//  Description : exit the web (window)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ExitWeb(void);

/*****************************************************************************/
//  Description : init the reference infomation of brower position
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_StartWeb(
                MMIBROWSER_WEB_ENTRY_TYPE_E entry_type,
                uint8 *url_ptr,
                wchar *snapshot_file_name_ptr
                );

/*****************************************************************************/
//  Description : the entry of browser is permit?
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsPermitEntry(void);

/*****************************************************************************/
//  Description : enter to add a bookmark 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_EnterAddBookmark(const char *url_ptr, const char *url_title_ptr);

/*****************************************************************************/
//  Description : select network type(sim or wifi)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SelectNetworkType(MN_DUAL_SYS_E dual_sys, BOOLEAN is_use_wifi);

#ifdef BROWSER_SEARCH_SUPPORT
/*****************************************************************************/
//  Description : Generate search web;cppy form MMIWWW_GenerateSearchWeb
//  Global resource dependence : 
//  Author: fen.xie
//  Note:RETURN need to FREE outside
/*****************************************************************************/
PUBLIC uint8 * MMIBROWSER_GenerateSearchWeb(
                        MMIBROWSER_SEARCH_ENGINE_MODE_E search_engine, 
                        const MMI_STRING_T *search_str
                        );

/*****************************************************************************/
//  Description : search the word in browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_Search(void);
/*****************************************************************************/
// 	Description : write key name to nv list
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_WriteKeyItem( MMI_STRING_T *in_url_ptr );
#endif
/*****************************************************************************/
//  Description : get the url of default homepage for browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetDefaultHomepageUrl(void);

/*****************************************************************************/
//  Description : get the url of homepage for browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetHomepageUrl(void);

/*****************************************************************************/
//  Description :set homepage and write to NV
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageUrl(const MMI_STRING_T *string_ptr);

/*****************************************************************************/
//  Description : convert the display text to fmm path(for example,"phone:"-> "D:")
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_DisplayToFmmPath(
                                      const MMI_STRING_T *display_path_str_ptr,//[IN]
                                      MMI_STRING_T *path_str_ptr//[IN/OUT]
                                      );
                                   
/*****************************************************************************/
//  Description : convert the fmm path to display format(for example,"D:"->"phone:")
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_FmmPathToDisplay(
                                      const MMI_STRING_T *path_str_ptr,//[IN]
                                      MMI_STRING_T *display_path_str_ptr//[IN/OUT]
                                      );
                                      
/*****************************************************************************/
//  Description : split pre pathname to device, dir
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SplitPrePath(      //支持多级目录
                              const wchar      *pre_path_ptr,     //in
                              uint16           pre_path_len,      //in, 双字节为单位
                              wchar            *device_ptr,        //out, one of device_ptr, dir_ptr and file_name_ptr should be NOT NULL
                              uint16           *device_len_ptr,    //in, 双字节为单位
                              wchar            *dir_ptr,           //out
                              uint16           *dir_len_ptr       //in, 双字节为单位
                              );

/*****************************************************************************/
//  Description : Appoint export save path text
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppointSaveMediaFilePath(wchar *path_ptr, uint32 path_len, MMIBRW_OpenFileCallBack callback);

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_CombineDownloadFile(
                                             MMI_STRING_T *path_ptr,
                                             MMI_STRING_T *name_ptr,
                                             wchar      *full_path_ptr,     //out
                                             uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                             BOOLEAN is_combine_tmp_file
                                             );

/*****************************************************************************/
//  Description : geth default media path for download(for example"D:/audio")
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetDefaultMediaFilePath(
                    wchar      *full_path_ptr,     //out
                    uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                    BRW_MIME_TYPE_E mime_type
                    );

/*****************************************************************************/
//  Description : open one snapshot file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshot(wchar *filename_ptr, BRW_INSTANCE brw_instance, BOOLEAN is_fixed);

/*****************************************************************************/
//  Description : merge the text id and num state("bookmark(m/n)".etc)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_MergeTextAndNum(
                MMI_STRING_T *string_ptr,//OUT:string_ptr->wstr_ptr need free outside
                MMI_TEXT_ID_T label_id,//[IN]
                uint16 total_num,//[IN]
                uint16 max_num//[IN]
                );
                
/*****************************************************************************/
//  Description : get default media dir for the file to be downloaded
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void BrowserDlGetDefaultMediaDir(
                uint16** path_ptr,
                uint16 *path_len_ptr,
                BRW_MIME_TYPE_E mime_type);

/*****************************************************************************/
//  Description : get the first valid device name for download
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E BrowserDlGetDeviceName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr, 
                BRW_MIME_TYPE_E mime_type);

/*****************************************************************************/
//  Description : when save one media file, whether only choose the storage for user by mime type?
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsOnlyChooseStorage(BRW_MIME_TYPE_E mime_type);

/*****************************************************************************/
//  Description : handle zoom msg
//  Global resource dependence : 
//  Author:fen.xie
//  Note:  retrun->is_zoom
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_HandleZoom(MMI_GESTURE_TYPE_E gesture_type);

/*****************************************************************************/
//  Description : get sys dev name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetBrwSysDevName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr);
/*****************************************************************************/
//  Description : get cookie dev name
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetBrwCookieDevName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr);

/*****************************************************************************/
//  Description : get cur page's url info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 与MMIBROWSER_FreeCurPageUrlInfo配对使用
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetCurPageUrlInfo(
                uint8 **url_pptr, //[IN/OUT]:need to free *url_pptr outside
                wchar **wurl_pptr//[IN/OUT]:need to free *wurl_pptr outside
                );
                
/*****************************************************************************/
//  Description : free memory for get cur page's url info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeCurPageUrlInfo(
                uint8 **url_pptr, //[IN/OUT]
                wchar **wurl_pptr//[IN/OUT]
                );
                
/*****************************************************************************/
//  Description : get cur address label info
//  Global resource dependence : 
//  Author: fen.xie
//  Note:  与MMIBROWSER_FreeCurAddrLabelInfo配对使用
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetCurAddrLabelInfo(wchar **label_info_pptr);//[IN/OUT]:need to free *label_info_pptr outside

/*****************************************************************************/
//  Description : free cur address label info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeCurAddrLabelInfo(wchar **label_info_pptr);//[IN/OUT]

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *string_1, MMI_STRING_T *string_2, MMI_IMAGE_ID_T image_id, uint32 user_data);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreateDynamicPopupRadioMenuByString(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIMENU_BUTTON_STYLE_E style, MMI_TEXT_ID_T title_id, uint16 node_num, MMI_STRING_T *string_array, uint32 *node_array, uint32 select_index);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index);

//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendOneTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *text_1, MMI_IMAGE_ID_T image_id, uint32 user_data);

/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author:Jiaoyou.wu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendSplitListItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id);

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreatePopupAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *title_str, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array);

#ifdef MMIDORADO_MY_NAV_SUPPORT
/*****************************************************************************/
//  Description : get my navigation info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetMyNavInfo(MMIBRW_MY_NAV_CONTEXT_T *nv_info_ptr);

/*****************************************************************************/
//  Description : set my navigation info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetMyNavInfo(MMIBRW_MY_NAV_CONTEXT_T *nv_info_ptr);

/*****************************************************************************/
//  Description : reset the my navigation information
//  Global resource dependence : MMINV_BROWSER_MY_NAV_INFO
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetMyNavInfo(void);

/*****************************************************************************/
//  Description : get the full path file name of the ico by filename
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIcoFullFileName(
                wchar  *name_ptr,      //[in]may PNULL,don't include suffix
                uint16 name_len,  //[in]may PNULL,name_ptr不为空时,此处必须填max len
                wchar  *full_path_ptr, //[in/out]
                uint16 *full_path_len_ptr  //[in/out]the len of full_path_ptr
                );

/*****************************************************************************/
//  Description : delete the ico directory(with the files of ico)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_DeleteIcoDir(void);

/*****************************************************************************/
//  Description : get the full path file name of the ico by index
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIcoFullFileNameByIdex(
                uint16  index,
                wchar  *full_path_ptr, //[in/out]
                uint16 *full_path_len_ptr  //[in/out]the len of full_path_ptr
                );

/*****************************************************************************/
//  Description : delete the ico file by the index
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_DeleteIco(uint16  index);

/*****************************************************************************/
//  Description : get s_is_ico_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsIcoUpdate(void);

/*****************************************************************************/
//  Description : set s_is_ico_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsIcoUpdate(BOOLEAN is_need_update);
#endif

/*****************************************************************************/
//  Description : send the url and title info by sms
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SendUrlContentBySms(
            const wchar   *title_ptr,
            uint16  title_len,
            const wchar   *url_ptr,
            uint16  url_len
            );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

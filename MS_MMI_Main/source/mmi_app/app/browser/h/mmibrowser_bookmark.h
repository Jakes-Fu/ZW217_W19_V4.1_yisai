/*****************************************************************************
** File Name:      mmibrowser_bookmark.h                                     *
** Author:         juan.zhang                                                *
** Date:           11/02/2009                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/02/2009     juan.zhang          Create                                 *
******************************************************************************/
#ifndef _MMIBROWSER_BOOKMARKL_H_
#define _MMIBROWSER_BOOKMARKL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmibrowser_export.h"
#include "gui_ucs2b_converter.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"

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
#define MMIBRW_BOOKMARK_MAX_TITLE_LENGTH	MMIBROWSER_BOOKMARK_MAX_TITLE_LENGTH
#define MMIBRW_BOOKMARK_MAX_URL_LENGTH		MMIBROWSER_BOOKMARK_MAX_URL_LENGTH
#ifdef MMIDORADO_BOOKMARK_BY_NV
#if defined(LOW_MEMORY_SUPPORT)  || !defined(NANDBOOT_SUPPORT)
#ifdef LOW_MEMORY_SUPPORT_32X16
#define MMIBRW_BOOKMARK_MAX_ITEM			5
#else
#define MMIBRW_BOOKMARK_MAX_ITEM			20
#endif
#else
#define MMIBRW_BOOKMARK_MAX_ITEM			50
#endif

#else/*MMIDORADO_BOOKMARK_BY_NV*/

#if defined(LOW_MEMORY_SUPPORT)
#ifdef MMI_BROWSER_MINI_SUPPORT
#define MMIBRW_BOOKMARK_MAX_ITEM			5
#else
#define MMIBRW_BOOKMARK_MAX_ITEM			20
#endif/*MMI_BROWSER_MINI_SUPPORT*/
#else
#define MMIBRW_BOOKMARK_MAX_ITEM			50
#endif

#endif/*MMIDORADO_BOOKMARK_BY_NV*/

#ifndef MMIDORADO_BOOKMARK_BY_NV
#define MMIBRW_BOOKMARK_FILE_NAME			"Bookmark.dat"
#define MMIBRW_BOOKMARK_FILE_DIR			"Bookmark"
#define MMIBRW_BOOKMARK_DIR_MAX_NUM       (5)
#define MMIBRW_ONE_BK_MIN_SPACE       (1)// 1K(1024)
#define MMIBRW_BOOKMARK_MAX_FOLDER_NAME_LENGTH	30//NEWMS00197620

#endif/*MMIDORADO_BOOKMARK_BY_NV*/

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
#ifdef MMIDORADO_BOOKMARK_BY_NV
typedef struct _MMIBRW_BOOKMARK_ITEM_T
{
	uint16	title_arr[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH];
	uint32	title_len;
	char	url_ptr[MMIBRW_BOOKMARK_MAX_URL_LENGTH+1];
}MMIBRW_BOOKMARK_ITEM_T;

typedef struct 
{
	uint32					item_count;
	MMIBRW_BOOKMARK_ITEM_T	item_info[MMIBRW_BOOKMARK_MAX_ITEM];
}MMIBRW_BOOKMARK_T;

#else/*MMIDORADO_BOOKMARK_BY_NV*/
typedef struct MMIBRW_BOOKMARK_T_
{
    uint32 total;//total
    uint32 cur_index_num;//current bookmark list 's total items num
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] ;//current fullpath
    uint16 full_path_len;//current fullpath len
    FILEARRAY file_array;
    uint32 file_array_step;
    uint32 cur_folder_num;//current bookmark list 's total folder num
}MMIBRW_BOOKMARK_T;

typedef enum MMIBRW_BOOKMARK_STEP_TYPE_E_
{
    BOOKMARK_STEP_TYPE_ROOT,
    BOOKMARK_STEP_TYPE_NEXT,
    BOOKMARK_STEP_TYPE_PRE,
    BOOKMARK_STEP_TYPE_MAX
}MMIBRW_BOOKMARK_STEP_TYPE_E;

typedef struct _MMIBROWSER_BK_STEP_INFO_
{
    MMIBRW_BOOKMARK_STEP_TYPE_E step_type;
    uint16 cur_array_index;//load 根目录时无效
    MMIFMM_FUNC_TYPE_E   fmm_func_type;
}MMIBROWSER_BK_STEP_INFO;
#endif

typedef enum
{
    MMIBRW_BM_CHANGE_ADD_NULL,      /*Title and Url is null*/
    MMIBRW_BM_CHANGE_ADD_CUR_PAGE,  /*Title and Url is from current web*/
    MMIBRW_BM_CHANGE_ADD_URL,       /*Url is from other app*/
    MMIBRW_BM_CHANGE_EDIT,          /*Title and Url is from bookmark*/
    
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BM_CHANGE_ADD_DIR,      /*folder*/
    MMIBRW_BM_CHANGE_EDIT_DIR,     /*Title  is from bookmark folder*/
#endif

    MMIBRW_BM_CHANGE_MAX
}MMIBRW_BM_CHANGE_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
#ifdef MMI_BROWSER_MINI_SUPPORT
/****************************************************************************/
//Description : free mem, add for low memory project
//Global resource dependence : 
//Author: bown.zhang
//Note:	
/****************************************************************************/
PUBLIC void MMIBRW_FreeBookmark(void);
#endif

/****************************************************************************/
//Description : bookmark initiation
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC void MMIBRW_BookmarkInit(void);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
//  Description : BookMark Initialize after the phone is start up(for protect)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_BookmarkInitAfterStartup(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : BookMark EXIT
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_BookmarkExit(void);
#endif /* Reduce Code size */
#endif

/****************************************************************************/
//Description : Get total count of the bookmark
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC uint32 MMIBRW_BookmarkGetCount(void);

#if 0
/*****************************************************************************/
//  Description : MMIBRW_Bookmark Split Path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkSplitPath(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, 双字节为单位
                            wchar           *pre_path_ptr, //IN/OUT
                            uint16          *pre_path_len_ptr,   //IN/OUT, 双字节为单位
                            wchar            *file_name_ptr,     //IN/OUT
                            uint16           *file_name_len_ptr,  //IN/OUT, 双字节为单位
                            BOOLEAN     is_with_suffix  //IN
                            );
#endif

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkCombinePath(
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          path_len,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          name_len,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     const wchar    *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          suffix_len, //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     wchar     *full_path_ptr, //out
                                     uint16          *full_path_len_ptr  //out
                                     );

#if 0
/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwCombineFullPath(
                                         char  *in_fname,
                                         wchar      *full_path_ptr,     //out
                                         uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                         );

/*****************************************************************************/
//  Description : BookMark finalize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_Bookmarkfinalize(void);
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : update the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIBRW_UpdateBookmarkTotalNum(void);

/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY_TYPE_E MMIBRW_GetBookmarkTitle(
                                            uint16 in_index, //in
                                            MMI_STRING_T *out_title//in/out:out_title->wstr_ptr need free outside
                                            );
#else
/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_GetBookmarkTitle(
                                uint16 in_index, //in
                                MMI_STRING_T *out_title//in/out
                                );
#endif

/*****************************************************************************/
// Description : Gets Url of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_GetBookmarkUrl(
                            uint16 in_index, //in
                            MMI_STRING_T *out_url//in/out:out_url->wstr_ptr need free outside
                            );
							
#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : load new file array(load 根目录或cur_array_index对应的文件夹的file)
// Global resource dependence :
// Author: xie.fen
// Note: fmm_func_type :1 FUNC_FIND_NULL表示使用默认的搜索方式
//          root:FUNC_FIND_FILE_AND_FOLDER  other:FUNC_FIND_FILE
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_LoadBookmarkFileArray(
                        MMIBRW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type,//in
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id
                        );
/*****************************************************************************/
// Description : reload file array
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ReloadBookmarkFileArray(
                        const wchar *path_ptr,
                        uint16 path_len,
                        MMIFMM_FUNC_TYPE_E fmm_func_type
                        );
#endif

/*****************************************************************************/
// Description : get the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBRW_BookmarkGetCount(void);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIBRW_GetBookmarkNumber(void);

/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: dave.ruan
// Note: 
/*****************************************************************************/
PUBLIC MMIBRW_BOOKMARK_T *MMIBRW_GetBookmark(void);
#endif

/*****************************************************************************/
// Description : Access network by bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIBRW_BookmarkGotoURL(uint16 in_index);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_AddBookmark(MMI_STRING_T in_url_str, //IN
                                            MMI_STRING_T in_title_str, //IN
                                            BOOLEAN in_protect, //IN
                                            uint16 *out_index_ptr,//OUT:new index
                                            wchar *full_path_ptr,//IN:(PNULL: add in current catelog)
                                            BOOLEAN is_need_update_array
                                            );

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_AddBookmarkFolder (MMI_STRING_T in_title_str, BOOLEAN in_protect, uint16 *out_index_ptr);

/*****************************************************************************/
// Description : Changes the title of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkTitle(uint16 in_index, MMI_STRING_T in_title_str, uint16 *out_index_ptr);

/*****************************************************************************/
// Description : Changes the url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkUrl(uint16 in_index, MMI_STRING_T in_url_str, uint16 *out_index_ptr);
#endif

/*****************************************************************************/
// Description : Deletes bookmark entry specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveBookmark(uint16 in_index , BOOLEAN is_for_delete_all);

/*****************************************************************************/
// Description : Deletes all bookmark include file and folder,but only in root
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveAllBookmark(void);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Deletes all bookmark in folder,but do Not delete folder
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_RemoveAllBookmarkOfSubFolder(void);

/*****************************************************************************/
//  Description : Clean bookmark Data Info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 清空书签
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_CleanBookmarkData(BOOLEAN in_force);
#endif

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_SetDefaultBookmark(void);

#ifndef MMIDORADO_BOOKMARK_BY_NV
/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIBRW_FindDuplicatedBookmarkIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 path_step, 
                    wchar *new_name_ptr);

/*****************************************************************************/
// Description : Find duplicated Bookmark dir
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIBRW_FindDuplicatedBookmarkDirIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 cur_path_num, 
                    wchar *new_name_ptr);

/*****************************************************************************/
//  Description : Bookmark Is In Root
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkIsInRoot(void);

/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIBRW_BookmarkGetFolderNum(BOOLEAN is_by_filearray);

/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolderName(FILEARRAY cur_file_array,
                uint16 fodler_index,  //folder index of all folder
                MMI_STRING_T *out_folder_name//in/out:out_folder_name->wstr_ptr need free outside
                );
                                                                            
/*****************************************************************************/
//  Description : Bookmark Get Folder Num
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolderNameArray(FILEARRAY cur_file_array,
                    MMI_STRING_T **out_folder_name_pptr,//[in/out]:*out_folder_name_pptr and out_folder_name_pptr[i]->wstr_ptr need free outside
                    uint16  *folder_total_num_ptr//[in/out]
                    );
                    
/*****************************************************************************/
//  Description : Bookmark Get Folder
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkGetFolder(
                                                                            uint16 fodler_index,  //folder index of all folder
                                                                            wchar *full_path_ptr,
                                                                            uint16 *full_path_len_ptr,
                                                                            FILEARRAY cur_file_array
                                                                            );

/*****************************************************************************/
//  Description : Add or Edit bookmark
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIBRW_ChangeBookmarkWin(MMIBRW_BM_CHANGE_TYPE_E change_type);

/*****************************************************************************/
//  Description : check if the default bookmark is load,and record in NV 
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsBookmarkLoaded(void);

/*****************************************************************************/
//  Description : check if the default bookmark is load,and record in NV 
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBookmarkLoaded(BOOLEAN is_load);

/*****************************************************************************/
//  Description : get bookmark default root path
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwGetDefaultRootPath(
                wchar      *full_path_ptr,     //out
                uint16     *full_path_len_ptr  //in/out, 双字节为单位
                );

/*****************************************************************************/
// Description : after load the file array for bookmark, update the bookmark info
// Global resource dependence :s_load_fmm_func_type
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_LoadFinish(void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsBKFolderExist(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    wchar *new_name_ptr,
                    uint16 new_name_len
                    );
#endif /* Reduce Code size */
                    
/*****************************************************************************/
//  Description : handle plug out sd card for browser bookmark
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BKHandleSDCardPlugOut(void);

/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_BKHandleStorageFormat(void);

/*****************************************************************************/
// Description : Changes the title and url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBRW_ChangeBookmarkTitleAndUrl(uint16 in_index, 
                MMI_STRING_T in_title_str,
                MMI_STRING_T in_url_str, 
                uint16 *out_index_ptr);

/*****************************************************************************/
// Description : get the file suffix name of bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC const wchar* MMIBRW_GetBkSuffixName(void);
#else
/****************************************************************************/
//Description : Get the indicated index node info 
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC MMIBRW_BOOKMARK_ITEM_T* MMIBRW_BookmarkGetItem(uint32 item_index);

/****************************************************************************/
//Description : To add a bookmark item
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkAddItem(
                MMIBRW_BOOKMARK_ITEM_T	*item_info_ptr
                );

/****************************************************************************/
//Description : To update the bookmark info for the indecated node
//Global resource dependence : 
//Author: juan.zhang
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN MMIBRW_BookmarkUpdateItem(
                uint32					item_index,
                MMIBRW_BOOKMARK_ITEM_T *item_info_ptr
                );
#endif

/*****************************************************************************/
//  Description : open the bookmark edit window of browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_TYPE_E change_type);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif 


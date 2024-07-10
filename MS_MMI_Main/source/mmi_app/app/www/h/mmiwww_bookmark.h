/***************************************************************************
** File Name:      mmiwww_bookmark.h                                            *
** Author:                                                                 *
** Date:             06/09/2010                                             *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser netfront define                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2010        fen.xie        Create
****************************************************************************/
#ifndef  _MMIWWW_BOOKMARK_H_    
#define  _MMIWWW_BOOKMARK_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiwww_internal.h"
#include "std_header.h"
#include "mmk_app.h"
#include "mmi_theme.h"
#include "mmiwww_export.h"
#include "mmiwww_netfront.h"
#include "mmi_position.h"
#include "gui_ucs2b_converter.h"
#include "mmi_nv.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiwww_position.h"
#include "mmiwww_func.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifmm_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/                        
/*****************************************************************************/
// Description : update the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_UpdateBookmarkTotalNum(void);

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_Bookmark_Initialize(MMIWWW_INSTANCE_T *self);

/*****************************************************************************/
//  Description : BookMark finalize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Bookmark_finalize(void);

/*****************************************************************************/
// Description : Gets title of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC FILEARRAY_TYPE_E MMIWWW_GetBookmarkTitle(
                                            uint16 in_index, //in
                                            MMI_STRING_T *out_title//in/out:out_title->wstr_ptr need free outside
                                            );

/*****************************************************************************/
// Description : Gets Url of specified title entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_GetBookmarkUrl(
                            uint16 in_index, //in
                            MMI_STRING_T *out_url//in/out:out_url->wstr_ptr need free outside
                            );

/*****************************************************************************/
// Description : load new file array(load 根目录或cur_array_index对应的文件夹的file)
// Global resource dependence :
// Author: xie.fen
// Note: fmm_func_type :1 FUNC_FIND_NULL表示使用默认的搜索方式
//          root:FUNC_FIND_FILE_AND_FOLDER  other:FUNC_FIND_FILE
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_LoadBookmarkFileArray(
                        WWW_BOOKMARK_STEP_TYPE_E step_type,//in
                        uint16 cur_array_index,//in:load 根目录时无效
                        MMIFMM_FUNC_TYPE_E   fmm_func_type//in
                        );

/*****************************************************************************/
// Description : reload file array
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_ReloadBookmarkFileArray(
                        const wchar *path_ptr,
                        uint16 path_len,
                        MMIFMM_FUNC_TYPE_E   fmm_func_type
                        );

/*****************************************************************************/
// Description : get the total bookmark number
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWWW_GetBookmarkTotalNumber(void);

/*****************************************************************************/
// Description : get index num of current bookmark path(include the "..." index)
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_GetBookmarkNumber(void);

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_AddBookmark (MMI_STRING_T in_url_str, //IN
                                            MMI_STRING_T in_title_str, //IN
                                            BOOLEAN in_protect, //IN
                                            uint16 *out_index//OUT:new index
                                            );

/*****************************************************************************/
// Description : Adds a new bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_AddBookmarkFolder (MMI_STRING_T in_title_str, slim_bool in_protect, uint16 *out_index);


/*****************************************************************************/
// Description : Changes the title of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ChangeBookmarkTitle(uint16 in_index, MMI_STRING_T in_title_str, uint16 *out_index);

/*****************************************************************************/
// Description : Changes the url of specified bookmark entry
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIWWW_ChangeBookmarkUrl(uint16 in_index, MMI_STRING_T in_url_str);

/*****************************************************************************/
// Description : Deletes bookmark entry specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveBookmark(uint16 in_index , BOOLEAN is_for_delete_all);

/*****************************************************************************/
// Description : Deletes all bookmark include file and folder,but only in root
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveAllBookmark(BOOLEAN in_force);

/*****************************************************************************/
// Description : Deletes all bookmark in folder,but do Not delete folder
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_RemoveAllBookmarkOfSubFolder(void);

/*****************************************************************************/
// Description : Access network by bookmark
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/ 
PUBLIC void MMIWWW_Bookmark_GotoURL(uint16 in_index);

/*****************************************************************************/
// Description : Swap Bookmark
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: bookmark will be de destroyed if FALSE return
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_SwapBookmark(int32 index_1, int32 index_2);

/*****************************************************************************/
// Description : Find duplicated Bookmark
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIWWW_FindDuplicatedBookmarkIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 path_step, 
                    wchar *new_name_ptr);

/*****************************************************************************/
// Description : Find duplicated Bookmark dir
// Global resource dependence :
// Author: fen.xie
// Note: negative number  means there is no duplicated bookmark
/*****************************************************************************/
PUBLIC int32 MMIWWW_FindDuplicatedBookmarkDirIndex(wchar *cur_path_ptr, 
                    uint16 cur_path_len, 
                    uint16 cur_path_num, 
                    wchar *new_name_ptr);

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_BookmarkIsInRoot(void);

/*****************************************************************************/
//  Description : BookMark Initialize
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWWW_BookmarkGetFolderNum(void);

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_BookmarkCombinePath(
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          path_len,  //in/out:may PNULL,path_ptr不为空时,此处必须填max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          name_len,  //in/out:may PNULL,name_ptr不为空时,此处必须填max len
                                     wchar           *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          suffix_len, //in/out:may PNULL,suffix_ptr不为空时,此处必须填max len
                                     wchar     *full_path_ptr, //out
                                     uint16          *full_path_len_ptr  //out
                                     );

/*****************************************************************************/
//  Description : Clean bookmark Data Info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 清空书签
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_CleanBookmarkData(BOOLEAN in_force);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


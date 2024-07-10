/*****************************************************************************
** File Name:               mmiapwin_multilist.h                             *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAPWIN_MULTILIST_H_
#define _MMIAPWIN_MULTILIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    uint32                      select_num;
    uint16                      *select_index_ptr;
    uint16                      dst_list_index;
    BOOLEAN                     src_is_music_list;
    uint16                      src_list_index;
    BOOLEAN                     is_new_list;
    MMI_HANDLE_T                win_handle;
} MMIMP3_LIST_ADD_TO_INFO_T;


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIAPMULTILIST_AddCurFileToList
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPMULTILIST_AddCurFileToList(BOOLEAN is_create_new_list);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMULTILIST_EnterMultiListWin(void);

/*****************************************************************************/
//  Description : open current play list
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPLISTWIN_OpenCurPlayList(void);

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetListIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPMULTILIST_GetListSelection(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetFileIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPMULTILIST_GetFileIndex(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : MMIAPMULTILIST_GetFileIndex
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPMULTILIST_IsMusicList(MMI_WIN_ID_T win_id);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif



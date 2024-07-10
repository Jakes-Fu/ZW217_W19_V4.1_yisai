/*****************************************************************************
** File Name:      ctrlscroll_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLSCROLL_EXPORT_H_
#define _CTRLSCROLL_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<ºáÊúÆÁÇøÓò*/
} GUISCROLL_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set control total size
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetTotalSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 total_size //in
);



/*****************************************************************************/
// 	Description : set control page size
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetPageSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 page_size //in
);



/*****************************************************************************/
// 	Description : set control line size
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetLineSize(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 line_size //in
);



/*****************************************************************************/
// 	Description : set control top of current page
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLL_SetCurrentPostion(
    MMI_HANDLE_T ctrl_handle, //in
    uint32 top_of_current_page//in
);


/*****************************************************************************/
// 	Description : control update
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLL_Update(
    MMI_HANDLE_T ctrl_handle
);


/*****************************************************************************/
// 	Description : control update
//	Global resource dependence :
//  Author: lianxiang.zhou & xiyuan.ma
//	Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLL_DisableAutoThin(
    MMI_HANDLE_T ctrl_handle
);



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/

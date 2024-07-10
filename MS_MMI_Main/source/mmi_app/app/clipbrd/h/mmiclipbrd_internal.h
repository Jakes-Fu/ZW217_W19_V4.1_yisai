/***************************************************************************
** File Name:      mmiclipbrd_internal.h                                   *
** Author:         hua.fang                                                *
** Date:           08/05/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMICLIPBRD_INTERNAL_H_
#define _MMICLIPBRD_INTERNAL_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmitheme_clipbrd.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : show clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T _MMICLIPBRD_ShowMenu(                                    // [ret]  return clipboard window handle
                                         const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                         );

/*****************************************************************************/
//  Description : close clipboard menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN _MMICLIPBRD_CloseMenu(                       // [ret]
                                     MMI_HANDLE_T handle    // [in] clipboard window handle
                                     );

/*****************************************************************************/
//  Description : show clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T _MMICLIPBRD_ShowToolbar(                                    // [ret]  return clipboard window handle
                                            const CLIPBRDMENU_DATA_T *data_ptr  // [in] data
                                            );

/*****************************************************************************/
//  Description : close clipboard toolbar
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN _MMICLIPBRD_CloseToolbar(                       // [ret]
                                        MMI_HANDLE_T handle    // [in] clipboard window handle
                                        );

/*****************************************************************************/
//  Description : 创建放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StartMagnify(
                                     int16   x,
                                     int16   y,
                                     MMI_HANDLE_T ctrl_handle
                                     );

/*****************************************************************************/
//  Description : 移动放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_MoveMagnify(
                                    int16   x,
                                    int16   y,
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//  Description : 结束放大镜
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void _MMICLIPBRD_StopMagnify();

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMICLIPBRD_INTERNAL_H_

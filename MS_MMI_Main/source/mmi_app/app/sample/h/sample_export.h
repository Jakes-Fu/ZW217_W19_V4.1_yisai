/***************************************************************************
** File Name:      sample_export.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         Xiaoqing.Lu      Create                                  *
**																		   *
****************************************************************************/
#ifndef _SPAMPLE_EXPORT_H_
#define _SPAMPLE_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"

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
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISAMPLE_CreateHelloSprdWin(void);

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISAMPLE_HELLO_ModuleInit(void);

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMI_CreateHelloSprdWin              MMIAPISAMPLE_CreateHelloSprdWin
#define MMIHELLO_ModuleInit                 MMIAPISAMPLE_HELLO_ModuleInit

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_HELLO_SPRD_H_
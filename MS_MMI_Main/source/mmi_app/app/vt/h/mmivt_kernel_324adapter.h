/****************************************************************************
** File Name:      mmivt_kernel_refAdapter.c                               *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
#ifndef _MMIVT_KERNEL_324ADAPTER_H_
#define _MMIVT_KERNEL_324ADAPTER_H_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
//#include "mmivt_export.h"
#ifdef VT_SUPPORT

#include "sci_types.h"
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
// 	Description : open the 324 stack
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_Open(int32 b_mo);

/*****************************************************************************/
// 	Description : close the 324 stack
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_324_Close(void);


/*****************************************************************************/
// 	Description :  make call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_MakeCall(void);


/*****************************************************************************/
// 	Description :  listen for call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_ConnectReq(void);


/*****************************************************************************/
// 	Description :  hangup
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVT_324_HangUp(void);
    
/*****************************************************************************/
// 	Description :  SendUserInput
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_324_SendUserInput(uint8 send_char);

/*****************************************************************************/
// 	Description :  MMIVT_324_EnableCamera
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_324_EnableCamera(BOOLEAN is_enable);
    
#endif //VT_SUPPORT
    
#ifdef   __cplusplus
}
#endif

#endif //_MMIVT_KERNEL_324ADAPTER_H_


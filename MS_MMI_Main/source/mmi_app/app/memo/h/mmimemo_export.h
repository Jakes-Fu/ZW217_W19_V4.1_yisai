/*****************************************************************************
** File Name:      mmimemo_export.h                                          *
** Author:          fen.xie                                                  *
** Date:            05/08/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the memo  export interface  *
*****************************************************************************/
#ifndef _MEMO_EXPORT_H_
#define _MEMO_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmipub.h"

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

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                             *
 **--------------------------------------------------------------------------*/    

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open memo main win msg
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_OpenMain(void);

/*****************************************************************************/
//  Description : exit the memo module
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMEMO_Exit(void);

/*****************************************************************************/
//  Description : init memo module
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMEMO_InitModule(void);

/*****************************************************************************/
//  Description : MMIMEMO_DeleteUserDataInPhone
//  Global resource dependence : 
//  Author: leon.wang
//  Note:   delete all memo file stored in phone space  
/*****************************************************************************/
PUBLIC void MMIMEMO_DeleteUserDataInPhone(void);

/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIMEMO_InitModule          MMIAPIMEMO_InitModule

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



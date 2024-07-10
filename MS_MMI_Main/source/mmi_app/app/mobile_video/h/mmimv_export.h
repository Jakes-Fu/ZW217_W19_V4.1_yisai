/*****************************************************************************
** File Name:      mmimv_export.h                                           	*
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18                 Create
******************************************************************************/

#ifndef _MMIMV_EXPORT_H_
#define _MMIMV_EXPORT_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIMV_InitModule        MMIAPIMV_InitModule
#define MMIMV_Entry             MMIAPIMV_Entry
#define MMIMV_Exit              MMIAPIMV_Exit
#define MMIMV_Pause             MMIAPIMV_Pause
#define MMIMV_IsLiving          MMIAPIMV_IsLiving
#define MMIMV_IsOpenMobileVideo MMIAPIMV_IsOpenMobileVideo

#endif //MMI_FUNC_COMPATIBLE_SUPPORT        

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : initialize mobile video module
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_InitModule(void);

/*****************************************************************************/
//  Description :init mv app 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_Init(void);

/*****************************************************************************/
//  Description : mobile video entry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_Entry(void);

/*****************************************************************************/
//  Description : mobile video exit
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_Exit(void);

/*****************************************************************************/
//  Description : pause mobile video 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/

PUBLIC void MMIAPIMV_Pause(void);

/*****************************************************************************/
//  Description : is living
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_IsLiving(void);

/*****************************************************************************/
//  Description : is open mobile video
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_IsOpenMobileVideo(void);

/*****************************************************************************/
//  Description : cancel contact dialog
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMV_CancelContactDialog(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif




#endif


/***************************************************************************
** File Name:      mmidcd_wintab.h                                         *
** Author:                                                                 *
** Date:           20/09/2007                                              *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/2007        violent.wei      Create
****************************************************************************/
#ifndef  _MMICS_WINTAB_H_    
#define  _MMICS_WINTAB_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef CMCC_UI_STYLE
#include "guilcd.h"
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
typedef enum
{
    MMICS_CM_SELECT_NULL=0,
    MMICS_CM_SELECT_NAME,
    MMICS_CM_SELECT_NUM,
    MMICS_CM_HOTLINE_NUM,
    MMICS_CM_SELECT_MAX,

} MMICS_CM_SELECT_TYPE_E;



/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMICS_OpenMainWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif //MMI_ENABLE_DCD
#endif


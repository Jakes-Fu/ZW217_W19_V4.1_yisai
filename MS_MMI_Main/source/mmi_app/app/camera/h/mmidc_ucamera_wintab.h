/*****************************************************************************
** File Name:      mmidc_ucamera_wintab.h                                                *
** Author:         Jessica                                               *
** Date:           9/2/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to descirbe ucamera                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2012       Jessica             Create
******************************************************************************/

#ifndef _MMIDC_UCAMERA_WINTAB_H_
#define _MMIDC_UCAMERA_WINTAB_H_ 

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
//Description : open ucamera win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenUCameraWin(void);

/*****************************************************************************/
//Description : open u camera alert win
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenUCameraAlertWin(MMI_TEXT_ID_T txt_id);

/*****************************************************************************/
//Description : get first entry ucamera flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_GetFirstEntryUcameraFlag(void);

/*****************************************************************************/
//Description : set first entry ucamera flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetFirstEntryUcameraFlag(BOOLEAN flag);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDC_UCAMERA_WINTAB_H_



/*****************************************************************************
** File Name:      mmiap_lastinfo_ctrl.h                                     *
** Author:                                                                   *
** Date:           5/29/2012                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mp3 history list                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/5/29      robert.wang       Create                                   *
******************************************************************************/

#ifdef MMIAP_F_HISTORY_PLAYLIST
#ifndef _MMIAP_LASTINFO_CTRL_H_
#define _MMIAP_LASTINFO_CTRL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiap_lastinfo_manager.h"

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
                                                      
/*****************************************************************************/
//  Description : set history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAP_SetHistoryInitFlag(BOOLEAN is_init);

/*****************************************************************************/
//  Description : get history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsHistoryInit(void);

/*****************************************************************************/
//  Description : save mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SavePlayListInfo(void);

/*****************************************************************************/
//  Description : get history mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetHistoryPlayListInfo(void);


/*****************************************************************************/
//  Description : save mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetHistoryPanelInfo(MMIAP_HISTORY_PANEL_INFO_T *info_ptr //out
                                                 );
                                                 
/*****************************************************************************/
//  Description : get history review button valid
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetReviewButtonStatusFromLastInfo(void);

/*****************************************************************************/
//  Description : reset last info nv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResetLastInfoNV(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

#endif //#ifdef MMIAP_F_HISTORY_PLAYLIST



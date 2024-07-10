/***************************************************************************
** File Name:      mmiautodemo_task.h                                             *
** Author:                                                                 *
** Date:           3/16/2012                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                  NAME                  DESCRIPTION                             *
** 3/2012         yongsheng.wang            Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMIAUTODEMO_TSAK_H_
#define  _MMIAUTODEMO_TSAK_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_osbridge.h"
#include "mmiautodemo_api.h"
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
//  Description : send kpd signal to mmi task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_Send_KPD_SignalToMMITask( uint32 keycode , MMIAD_MSG_TYPE_E msg_type, uint16 pause_time);

/*****************************************************************************/
//  Description : send tp signal to mmi task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_Send_TP_SignalToMMITask( MMIAD_MSG_TYPE_E msg_type,  
										  GUI_POINT_T start_point , 
										  GUI_POINT_T end_point,
										  uint16 pause_time);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_MMIAUTODEMO_TSAK_H_
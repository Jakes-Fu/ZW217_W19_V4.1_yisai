/****************************************************************************
** File Name:      mmk_opsensor.h                                           *
** Author:                                                                 *
** Date:           06/2012                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2012       juan.wu           Create
** 
****************************************************************************/
#ifdef MOTION_SENSOR_TYPE_OPTICAL

#ifndef  _MMK_OPSENSOR_H_    
#define _MMK_OPSENSOR_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_msg.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMK_OPSENSOR_STATUS_NONE,
    MMK_OPSENSOR_STATUS_NO_DETECTED,
    MMK_OPSENSOR_STATUS_STH_DETECTED,
    MMK_OPSENSOR_STATUS_MAX
}MMK_OPSENSOR_STATUS_E;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : check gsensor signal
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckOPsensorSignal( void );

/*****************************************************************************/
// 	Description : open gsensor
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenOPsensor(
                              MMI_HANDLE_T handle
                              );


/*****************************************************************************/
// 	Description : close gsensor
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseOPsensor(
                                MMI_HANDLE_T handle
                                );

/*****************************************************************************/
// 	Description : dispatch optical sensor msg
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchOPsensorMsg(
                                   MmiSignalS* signal_ptr
                                   );

#endif
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_GSENSOR_H_  */
/*! @} */



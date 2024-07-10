/****************************************************************************
** File Name:      mmk_gsensor.h                                           *
** Author:                                                                 *
** Date:           07/2010                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010       James.Zhang         Create
** 
****************************************************************************/

#ifndef  _MMK_GSENSOR_H_    
#define _MMK_GSENSOR_H_   

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
    MMK_GSENSOR_STATUS_SHAKE = 0x00, 	// NO direction, Only detect shake
        
    //活动状态    
    MMK_GSENSOR_STATUS_LEFT,
    MMK_GSENSOR_STATUS_RIGHT,
    MMK_GSENSOR_STATUS_UP,
    MMK_GSENSOR_STATUS_DOWN,
    MMK_GSENSOR_STATUS_FORWARD,
    MMK_GSENSOR_STATUS_BACKWARD,
    
    //静止状态
    MMK_GSENSOR_STATUS_LEFT_SIDE,
    MMK_GSENSOR_STATUS_RIGHT_SIDE,	
    MMK_GSENSOR_STATUS_UP_SIDE,
    MMK_GSENSOR_STATUS_DOWN_SIDE,
    MMK_GSENSOR_STATUS_FORWRAD_SIDE,
    MMK_GSENSOR_STATUS_BACKWARD_SIDE,		
        
    MMK_GSENSOR_STATUS_NONE			// No motion
        
}MMK_GSENSOR_STATUS_E;

typedef struct
{
    int32 x;					//X轴加速度(1G加速度值=50量化值)
    int32 y;					//Y轴加速度(1G加速度值=50量化值)
    int32 z;					//Z轴加速度(1G加速度值=50量化值)
}MMK_GSENSOR_VECTOR_T;

/*--------------------------------------------------------------------------*/
/*                         FUNCTIONS                                        */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : check gsensor signal
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckGsensorSignal( void );

/*****************************************************************************/
// 	Description : open gsensor
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenGsensor(
                              MMI_HANDLE_T handle
                              );


/*****************************************************************************/
// 	Description : close gsensor
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseGsensor(
                                MMI_HANDLE_T handle
                                );

/*****************************************************************************/
// 	Description : get gsensor status
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMK_GSENSOR_STATUS_E MMK_GetGsensorStatus(void);

/*****************************************************************************/
// 	Description : get gsensor vector
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetGsensorVector(
                                    MMK_GSENSOR_VECTOR_T* vector_ptr
                                    );

/*****************************************************************************/
// 	Description : dispatch sensor msg
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchGsensorMsg(
                                   MmiSignalS* signal_ptr
                                   );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif

#endif /* END _MMK_GSENSOR_H_  */
/*! @} */


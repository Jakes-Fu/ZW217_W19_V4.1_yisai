/****************************************************************************
** File Name:      mmk_gsensor.c                                           *
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
#define MMK_GSENSOR_C

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmk_gsensor.h"
#include "mmk_app.h"
#include "msensor_drv.h"
#ifdef MOTION_SENSOR_TYPE_OPTICAL
#include "mmk_opsensor.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_MAX_GSENSOR_MSG          5
#define MMK_MAX_GSENSOR_REG_NUM   	 16  //最多可注册的gsensor个数
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMK_GSENSOR_STATUS_E status;
    uint32               msg_num;
    MMI_HANDLE_T         handle_array[MMK_MAX_GSENSOR_REG_NUM];
} MMK_GSENSOR_INFO_T;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMK_GSENSOR_INFO_T s_gsensor_info = { MMK_GSENSOR_STATUS_NONE, 0, 0 };
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         External FUNCTION                                *
 **--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : convert msensor type to gsensor status
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMK_GSENSOR_STATUS_E ConvertGsensorStatus(
                                                MSENSOR_MOTION_TYPE_E type
                                                );

/*****************************************************************************/
// 	Description : reduce gsensor msg num
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ReduceGsensorMsgNum(void);

/*****************************************************************************/
// 	Description : add gsensor handle
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AddGsensorHandle(
                               MMI_HANDLE_T handle
                               );

/*****************************************************************************/
// 	Description : remove gsensor handle
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveGsensorHandle(
                                  MMI_HANDLE_T  handle
                                  );

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : convert msensor type to gsensor status
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL MMK_GSENSOR_STATUS_E ConvertGsensorStatus(
                                                MSENSOR_MOTION_TYPE_E type
                                                )
{
    MMK_GSENSOR_STATUS_E status = MMK_GSENSOR_STATUS_NONE;
    
    switch( type )
    {
    case MSENSOR_MOTION_TYPE_SHAKE:
        status = MMK_GSENSOR_STATUS_SHAKE;
        break;
        
    case MSENSOR_MOTION_TYPE_LEFT:
        status = MMK_GSENSOR_STATUS_LEFT;
        break;
        
    case MSENSOR_MOTION_TYPE_RIGHT:
        status = MMK_GSENSOR_STATUS_RIGHT;
        break;
        
    case MSENSOR_MOTION_TYPE_UP:
        status = MMK_GSENSOR_STATUS_UP;
        break;
        
    case MSENSOR_MOTION_TYPE_DOWN:
        status = MMK_GSENSOR_STATUS_DOWN;
        break;
        
    case MSENSOR_MOTION_TYPE_FORWARD:
        status = MMK_GSENSOR_STATUS_FORWARD;
        break;
        
    case MSENSOR_MOTION_TYPE_BACKWARD:
        status = MMK_GSENSOR_STATUS_BACKWARD;
        break;
        
    case MSENSOR_MOTION_TYPE_LEFT_SIDE:
        status = MMK_GSENSOR_STATUS_LEFT_SIDE;
        break;
        
    case MSENSOR_MOTION_TYPE_RIGHT_SIDE:
        status = MMK_GSENSOR_STATUS_RIGHT_SIDE;
        break;
        
    case MSENSOR_MOTION_TYPE_UP_SIDE:
        status = MMK_GSENSOR_STATUS_UP_SIDE;
        break;
        
    case MSENSOR_MOTION_TYPE_DOWN_SIDE:
        status = MMK_GSENSOR_STATUS_DOWN_SIDE;
        break;
        
    case MSENSOR_MOTION_TYPE_FORWRAD_SIDE:
        status = MMK_GSENSOR_STATUS_FORWRAD_SIDE;
        break;
        
    case MSENSOR_MOTION_TYPE_BACKWARD_SIDE:
        status = MMK_GSENSOR_STATUS_BACKWARD_SIDE;
        break;
        
    default:
        break;
    }
    
    return status;
}

/*****************************************************************************/
// 	Description : reduce gsensor msg num
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void ReduceGsensorMsgNum(void)
{
    if ( s_gsensor_info.msg_num > 0)
    {
        s_gsensor_info.msg_num--;
    }
}

/*****************************************************************************/
// 	Description : add gsensor handle
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AddGsensorHandle(
                               MMI_HANDLE_T handle
                               )
{
    BOOLEAN result = FALSE;
    int32 i = 0;

    handle = MMK_ConvertIdToHandle( handle );

    if ( 0 == handle )
    {
        return result;
    }

    for ( i = 0; i < MMK_MAX_GSENSOR_REG_NUM; i++ )
    {
        if ( handle == s_gsensor_info.handle_array[i] )
        {
            //相同handle返回FALSE
            return FALSE;
        }
    }
    
    for ( i = 0; i < MMK_MAX_GSENSOR_REG_NUM; i++ )
    {
        if ( 0 == s_gsensor_info.handle_array[i] )
        {
            s_gsensor_info.handle_array[i] = handle;
            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : remove gsensor handle
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveGsensorHandle(
                                  MMI_HANDLE_T  handle
                                  )
{
    BOOLEAN result = FALSE;
    int32 i = 0;

    handle = MMK_ConvertIdToHandle( handle );

    if ( 0 == handle )
    {
        return result;
    }

    for ( i = 0; i < MMK_MAX_GSENSOR_REG_NUM; i++ )
    {
        if ( handle == s_gsensor_info.handle_array[i] )
        {
            s_gsensor_info.handle_array[i] = 0;
            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
// 	Description : check gsensor
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckGsensorSignal( void )
{
    BOOLEAN result = FALSE;

    if ( s_gsensor_info.msg_num < MMI_MAX_GSENSOR_MSG )
    {
        s_gsensor_info.msg_num++;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// 	Description : open gsensor
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenGsensor(
                               MMI_HANDLE_T handle
                               )
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
    if ( AddGsensorHandle( handle ) )
    {
#ifndef WIN32
        if ( MSENSOR_SUCCESS == MSensor_Open(MSENSOR_TYPE_ACCELEROMETER) )
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
#else
        result = TRUE;
#endif
    }
#endif

    return result;
}

/*****************************************************************************/
// 	Description : close gsensor
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseGsensor(
                                MMI_HANDLE_T handle
                                )
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
    if ( RemoveGsensorHandle( handle ) )
    {
#ifndef WIN32
        if ( MSENSOR_SUCCESS == MSensor_Close(MSENSOR_TYPE_ACCELEROMETER) )
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
#else
        result = TRUE;
#endif
    }
#endif

    return result;
}

/*****************************************************************************/
// 	Description : get gsensor status
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMK_GSENSOR_STATUS_E MMK_GetGsensorStatus(void)
{
    return s_gsensor_info.status;
}

/*****************************************************************************/
// 	Description : get gsensor vector
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetGsensorVector(
                                    MMK_GSENSOR_VECTOR_T* vector_ptr
                                    )
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
#ifndef WIN32
    MSENSOR_MOTION_DATA_T msensor_data = {0};
    
    if ( PNULL == vector_ptr )
    {
        return FALSE;
    }
    //SCI_ASSERT( PNULL != vector_ptr );

    if ( MSENSOR_SUCCESS == MSensor_ioctl( MSENSOR_TYPE_ACCELEROMETER, 
                                           MSENSOR_IOCTL_GET_DATA,
                                           (void *)&msensor_data ) )
    {
        vector_ptr->x = msensor_data.u.vector.x;
        vector_ptr->y = msensor_data.u.vector.y;
        vector_ptr->z = msensor_data.u.vector.z;

        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#endif
#endif
    
    return result;
}

/*****************************************************************************/
// 	Description : dispatch sensor msg
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchGsensorMsg(
                                   MmiSignalS* signal_ptr
                                   )
{
    MmiKeyPress*    gsensor_ptr = PNULL;
    int32           i = 0;
    MSENSOR_TYPE_E  sensor_type = MSENSOR_TYPE_ACCELEROMETER;
    
    if ( PNULL == signal_ptr )
    {
        return;
    }

    gsensor_ptr = (MmiKeyPress*)signal_ptr;
    SCI_TRACE_LOW("MMK_DispatchGsensorMsg enter gsensor_ptr->keyCode = %d",gsensor_ptr->keyCode);

    sensor_type = gsensor_ptr->keyCode >> MSG_MISC_SENSOR_BIT_LEN;
    
#ifdef MOTION_SENSOR_TYPE_OPTICAL
    if(MSENSOR_TYPE_OPTICAL == sensor_type)
    {
        MMK_DispatchOPsensorMsg(signal_ptr);
        SCI_TRACE_LOW("MMK_DispatchOPsensorMsg enter");
    }
    else
#endif
    {
    	s_gsensor_info.status = ConvertGsensorStatus( gsensor_ptr->keyCode );

        for ( i = 0; i < MMK_MAX_GSENSOR_REG_NUM; i++ )
        {
            if ( 0 != s_gsensor_info.handle_array[i] )
            {
                MMK_SendMsg( s_gsensor_info.handle_array[i], MSG_GSENSOR, (DPARAM)s_gsensor_info.status );
            }
        }
        
        ReduceGsensorMsgNum();
    }
}


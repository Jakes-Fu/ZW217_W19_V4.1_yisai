/****************************************************************************
** File Name:      mmk_opsensor.c                                           *
** Author:                                                                 *
** Date:           06/2012                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2012       Juan.Wu           Create
** 
****************************************************************************/
#ifdef MOTION_SENSOR_TYPE_OPTICAL
#define MMK_OPSENSOR_C

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmk_opsensor.h"
#include "mmk_app.h"
#include "msensor_drv.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_MAX_OPSENSOR_MSG          5
#define MMK_MAX_OPSENSOR_REG_NUM   	 16  //最多可注册的Optical sensor个数
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    MMK_OPSENSOR_STATUS_E    status;
    uint32                  msg_num;
    MMI_HANDLE_T            handle_array[MMK_MAX_OPSENSOR_REG_NUM];
} MMK_OPSENSOR_INFO_T;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMK_OPSENSOR_INFO_T s_opsensor_info = { MMK_OPSENSOR_STATUS_NONE, 0, 0 };
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
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL MMK_OPSENSOR_STATUS_E ConvertOPsensorStatus(
                                                MSENSOR_MOTION_TYPE_E type
                                                );

/*****************************************************************************/
// 	Description : reduce gsensor msg num
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void ReduceOPsensorMsgNum(void);

/*****************************************************************************/
// 	Description : add gsensor handle
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void AddOPsensorHandle(
                               MMI_HANDLE_T handle
                               );

/*****************************************************************************/
// 	Description : remove gsensor handle
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void RemoveOPsensorHandle(
                                  MMI_HANDLE_T  handle
                                  );

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : convert msensor type to gsensor status
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL MMK_OPSENSOR_STATUS_E ConvertOPsensorStatus(
                                                MSENSOR_MOTION_TYPE_E type
                                                )
{
    MMK_OPSENSOR_STATUS_E status = MMK_OPSENSOR_STATUS_NONE;
    
    switch( type )
    {
    case MSENSOR_OPTICAL_NO_DETECTED:
        status = MMK_OPSENSOR_STATUS_NO_DETECTED;
        break;
        
    case MSENSOR_OPTICAL_STH_DETECTED:
        status = MMK_OPSENSOR_STATUS_STH_DETECTED;
        break;
        
    default:
        break;
    }
    
    return status;
}

/*****************************************************************************/
// 	Description : reduce gsensor msg num
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void ReduceOPsensorMsgNum(void)
{
    if ( s_opsensor_info.msg_num > 0)
    {
        s_opsensor_info.msg_num--;
    }
}

/*****************************************************************************/
// 	Description : add gsensor handle
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void AddOPsensorHandle(
                               MMI_HANDLE_T handle
                               )
{
    int32 i = 0;

    handle = MMK_ConvertIdToHandle( handle );

    if ( 0 == handle )
    {
        return;
    }

    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        if ( handle == s_opsensor_info.handle_array[i] )
        {
            //相同handle不需要添加handle，直接return
            return;
        }
    }
    
    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        if ( 0 == s_opsensor_info.handle_array[i] )
        {
            s_opsensor_info.handle_array[i] = handle;
            SCI_TRACE_LOW("AddOPsensorHandle SUCCESS!");
            break;
        }
    }
    return;
}

/*****************************************************************************/
// 	Description : get total handle num
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL uint16 GetOPsensorHandleNum(void)
{
    uint8 i = 0;
    uint16 total_num = 0;
    
    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        if(0 != s_opsensor_info.handle_array[i])
        {
            total_num++;
        }
    }
    return total_num;
}

/*****************************************************************************/
// 	Description : remove gsensor handle
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void RemoveOPsensorHandle(
                                  MMI_HANDLE_T  handle
                                  )
{
    int32 i = 0;

    handle = MMK_ConvertIdToHandle( handle );

    if ( 0 == handle )
    {
        return;
    }

    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        if ( handle == s_opsensor_info.handle_array[i] )
        {
            s_opsensor_info.handle_array[i] = 0;
            SCI_TRACE_LOW("RemoveOPsensorHandle SUCCESS!");
            break;
        }
    }
    return;
}
/*****************************************************************************/
// 	Description : add sensor msg num
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
LOCAL void AddOPsensorMsgNum(void)
{
    s_opsensor_info.msg_num++;
}
/*****************************************************************************/
// 	Description : check gsensor
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckOPsensorSignal( void )
{
    BOOLEAN result = FALSE;

    if ( s_opsensor_info.msg_num < MMI_MAX_OPSENSOR_MSG )
    {
        result = TRUE;
    }

    SCI_TRACE_LOW("MMK_CheckOPsensorSignal result = %d",result);
    return result;
}
/*****************************************************************************/
// 	Description : check if the handle all zero
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:if find a handle != 0 return FALSE
/*****************************************************************************/
LOCAL BOOLEAN CheckOPSensorHandle(void)
{
    uint8    i = 0;
    BOOLEAN result = TRUE;
    
    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        //return FALSE when find a handle != 0
        if(0 != s_opsensor_info.handle_array[i])
        {
            result = FALSE;
            break;
        }
    }
    SCI_TRACE_LOW("CheckOPSensorHandle result = %d",result);
    return result;
}

/*****************************************************************************/
// 	Description : open gsensor
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenOPsensor(
                               MMI_HANDLE_T handle
                               )
{
    BOOLEAN     result = TRUE;
    uint16      total_handle_num = 0;

    SCI_TRACE_LOW("MMK_OpenOPsensor enter");
#ifdef MOTION_SENSOR_TYPE_OPTICAL
    total_handle_num = GetOPsensorHandleNum();
    //不论是否已经add的handle，都不要再加了。
    if(total_handle_num >= MMK_MAX_OPSENSOR_REG_NUM) 
    {
        SCI_TRACE_LOW("MMK_OpenOPsensor total_handle_num = %d",total_handle_num);
        return FALSE;
    }
    //only open the sensor when the hanle array equal to 0
    if (0 == total_handle_num)
    {
#ifndef WIN32
        if ( MSENSOR_SUCCESS == MSensor_Open(MSENSOR_TYPE_OPTICAL) )
        {
            result = TRUE;            
            SCI_TRACE_LOW("MMK_OpenOPsensor SUCCESS!");
        }
        else
        {
            SCI_TRACE_LOW("MMK_OpenOPsensor FALSE!");
            result = FALSE;
        }
#else
        result = TRUE;
#endif
    }
    //只有当open sensor 失败时，才不添加handle
    if(result)
    {
        AddOPsensorHandle( handle );
    }
#endif
    SCI_TRACE_LOW("MMK_OpenOPsensor result = %d",result);
    return result;
}

/*****************************************************************************/
// 	Description : close gsensor
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseOPsensor(MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE_OPTICAL
    RemoveOPsensorHandle( handle );
    //close the sensor when handle array == 0
    if (CheckOPSensorHandle())
    {
#ifndef WIN32
        if ( MSENSOR_SUCCESS == MSensor_Close(MSENSOR_TYPE_OPTICAL) )
        {
            result = TRUE;
        }
        else
        {
            result = FALSE;
        }
        SCI_TRACE_LOW("MMK_CloseOPsensor handle array is empty!");
#else
        result = TRUE;
#endif
    }
#endif
    SCI_TRACE_LOW("MMK_CloseOPsensor handle = %d,result = %d",handle,result);
    return result;
}
/*****************************************************************************/
// 	Description : dispatch optical sensor msg
//	Global resource dependence : 
//  Author: Juan.Wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchOPsensorMsg(
                                   MmiSignalS* signal_ptr
                                   )
{
    MmiKeyPress*    opsensor_ptr = PNULL;
    int32           i = 0;
    
    if ( PNULL == signal_ptr )
    {
        SCI_TRACE_LOW("MMK_DispatchOPsensorMsg PNULL == signal_ptr");
        return;
    }

    AddOPsensorMsgNum();
    opsensor_ptr = (MmiKeyPress*)signal_ptr;

	s_opsensor_info.status = ConvertOPsensorStatus( opsensor_ptr->keyCode );
    SCI_TRACE_LOW("MMK_DispatchOPsensorMsg s_opsensor_info.status = %d",
                    s_opsensor_info.status);

    for ( i = 0; i < MMK_MAX_OPSENSOR_REG_NUM; i++ )
    {
        if ( 0 != s_opsensor_info.handle_array[i] )
        {
            MMK_SendMsg(s_opsensor_info.handle_array[i], 
                                   MSG_GSENSOR, 
                                    (DPARAM)s_opsensor_info.status );  
        }   
    }
    ReduceOPsensorMsgNum();
}
#endif


/******************************************************************************
 ** File Name:	dc_atv.c                                               	  
 ** Author:		Tim.Zhu		                                          
 ** DATE:		06/10/2010                                               
 ** Copyright:	2010 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION                            
 ******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_api.h"
#include "os_api.h"
#include "dc_app_6x00.h"
#include "sensor_drv.h"
#include "dc_framemgr.h"
#include "jpeg_interface.h"
#include "dc_bufmgr.h"
#include "isp_service.h"

#include "dc_preview.h"
#include "dc_snapshot.h"
#include "dc_review.h"
#include "dc_misc.h"
#include "dc_param_ctrl.h"
#include "deep_sleep.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define ATV_SCAN_CHN_TIMER 500
#define ATV_GET_RSSI_TIMER 3000

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Data                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Data                                   *
 **---------------------------------------------------------------------------*/
DECLARE_GLOBAL_CONTEXT;


extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/****************************************************************************************/
// Description: dc analog tv callback
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      atv_param: atv cmd
//                      cmd : atv callback cmd
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************************/
LOCAL uint32 _DC_AtvCallback(uint32 atv_param, DCAMERA_CALLBACK_CMD_E cmd)
{
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();

    switch(atv_param)
    {
        case DC_ATV_CMD_SCAN_CHN_START:
        case DC_ATV_CMD_SCAN_CHN:
        {
            if(PNULL!=pContext->atv_search_callback)
            {
                pContext->atv_call_param.cmd=cmd;
                pContext->atv_search_callback(&(pContext->atv_call_param));
            }

            break;
        }
        case DC_ATV_CMD_SCAN_CHN_STOP:
        {
            if(PNULL!=pContext->atv_stop_callback)
            {
                pContext->atv_call_param.cmd=cmd;
                pContext->atv_stop_callback(&(pContext->atv_call_param));
            }
            else
            {
                DC_SetEvent(DC_ATV_STOP_EVENT);
            }
            
            break;
        }
        case DC_ATV_CMD_RSSI:
        {
            if(PNULL!=pContext->atv_callback)
            {
                pContext->atv_call_param.cmd=cmd;
                pContext->atv_callback(&(pContext->atv_call_param));
            }
            break;
        }
        case DC_ATV_CMD_DECT_REGION:
        {
            if(PNULL!=pContext->param_callback)
            {
                pContext->param_callback(NULL, SCI_SUCCESS);
            }
            else
            {
                DC_SetEvent(DC_PARAM_CTRL_EVENT);
            }
            break;
        }
        default:
            break;
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: dc analog tv auto dection region
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 _DC_AtvAutoDectRegion(void)
{
    uint32 cmd=0x00;
    uint32 param=0x00;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();

    cmd=ATV_CMD_DECT_REGION;
    param=(cmd<<0x10)&0xffff0000;
    pContext->atv_call_param.rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
    _DC_AtvCallback(DC_ATV_CMD_DECT_REGION, DCAMERA_CALLBACK_ATV_DECT_REGION);

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: dc analog tv scan channel handler
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 DC_AtvScanChnHandler(void)
{
    DC_SendMsg(DCAM_MSG_ATV, DC_ATV_CMD_SCAN_CHN);
	
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: dc analog tv Rssi handler
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 DC_AtvRssiHandler(void)
{
    DC_SendMsg(DCAM_MSG_ATV, DC_ATV_CMD_RSSI);
	
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: dc analog tv handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      atv_param: atv cmd
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 DC_AtvHandle(uint32 atv_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=0x00;
    uint32 param=0x00;
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    PREVIEW_OPS_HANDLER_T* preview_handlers=DC_Preview_GetOpsHandler();

    switch(atv_param)
    {
        case DC_ATV_CMD_SCAN_CHN_START:
        {
            if(DC_IN_STATE(DC_STATE_ATV_SCAN_CHN))
            {
                DC_TimerDactive();
                //scan chn init
                cmd=ATV_CMD_SCAN_INIT;
                param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
                rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
                
                if(SCI_SUCCESS!=rtn)
                {
                    _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN_START, DCAMERA_CALLBACK_ATV_ERR);
                    return rtn;
                }
                DC_AtvScanChnHandler();
            }
            break;
        }
        case DC_ATV_CMD_SCAN_CHN:
        {
            if(DC_IN_STATE(DC_STATE_ATV_SCAN_CHN))
            {
                preview_handlers->stop();
                //scan chn start
                cmd=ATV_CMD_SCAN_CHN;
                param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
                rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
                //parse return param
                pContext->atv_call_param.atv_chn_is_valid=((rtn>>0x08)&0x01);
                pContext->atv_call_param.atv_chn_id=((rtn>>0x10)&0xffff);

                if(SCI_TRUE!=(rtn&0x01))
                {
                    SCI_Sleep(100);
                    rtn=preview_handlers->start();
                    if(SCI_SUCCESS!=rtn)
                    {
                        pContext->atv_call_param.rtn=SCI_ERROR;
                        _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_ERR);
                        DC_TimerDactive();
                    }
                    else
                    {
                        pContext->atv_call_param.rtn=SCI_SUCCESS;
                        SET_DCAM_STATE(DC_STATE_ATV_SCAN_CHN);
                        _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN);
                        DC_TimerActive(ATV_SCAN_CHN_TIMER, DC_AtvScanChnHandler);
                    }
                }
                else
                {
                    _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN);
                    _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN_END);
                    DC_TimerDactive();
                }
            }
            break;
        }
        case DC_ATV_CMD_SCAN_CHN_STOP:
        {
            DC_TimerDactive();

            DC_RETURN_VAL_IF_FAIL(preview_handlers->stop(), DCAMERA_OP_PREVIEW_ERR);

            cmd=ATV_CMD_SCAN_CHN_STOP;
            param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
            rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

            _DC_AtvCallback(DC_ATV_CMD_SCAN_CHN_STOP, DCAMERA_CALLBACK_ATV_SCAN_CHN_STOP);
            break;
        }
        case DC_ATV_CMD_RSSI:
        {
            if(DC_IN_STATE(DC_STATE_PREVIEWING))
            {
                cmd=ATV_CMD_GET_RSSI;
                param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
                rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

                if(pContext->atv_call_param.atv_rssi!=rtn)
                {
                    pContext->atv_call_param.atv_rssi=rtn;
                    _DC_AtvCallback(DC_ATV_CMD_RSSI, DCAMERA_CALLBACK_ATV_RSSI);
                }

                DC_TimerActive(ATV_GET_RSSI_TIMER, DC_AtvRssiHandler);
            }
            else
            {
                DC_TimerDactive();
            }
            break;
        }
        case DC_ATV_CMD_DECT_REGION:
        {
            rtn=_DC_AtvAutoDectRegion();
            break;
        }
        default :
            break;
    }

    return rtn;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif// End


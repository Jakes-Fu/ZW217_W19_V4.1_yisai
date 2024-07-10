/******************************************************************************
 ** File Name:  dcamera_atv.c                                                     
 ** Author:     Eddy.Wei                                                  
 ** DATE:       01/18/2012                                              
 ** Copyright:  2012 Spreatrum, Incoporated. All Rights Reserved.         
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
#include "sensor_drv.h"
#include "deep_sleep.h"
#include "dal_dcamera.h"
#include "dcamera_common.h"
#include "dcamera_misc.h"
#include "dcamera_atv.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif


/*----------------------------------------------------------------------------*
**                         External Function Prototype                           *
**---------------------------------------------------------------------------*/
extern PUBLIC DCAMERA_INFO_T* GetDCAMInfo(void);
extern PUBLIC uint32 DCAMERA_SendSignal(DCAMERA_TASK_CMD_E  cmd_type,
                                                    uint32  param_val,
                                                    void*   param_ptr1,
                                                    void*   param_ptr2);
extern PUBLIC void  DCAMERA_ATV_BeforeSetting(void);
extern PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ATV_AfterSetting(BOOLEAN is_isp_start);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
PUBLIC uint32 ATVErrHandler(void)
{
    ATV_INFO_T atv_info={0x00};
    ATV_INFO_T_PTR atv_info_ptr=NULL;
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=0x00;
    uint32 param=0x00;

    cmd=ATV_CMD_GET_INFO;
    param=(cmd<<0x10)&0xffff0000;
    atv_info_ptr=(ATV_INFO_T_PTR)Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
    SCI_MEMCPY((void*)&atv_info, (void*)atv_info_ptr, sizeof(ATV_INFO_T));

    Sensor_Close();
    Sensor_Open();

    cmd=ATV_CMD_SET_REGION;
    param=((cmd<<0x10)&0xffff0000)|(atv_info.atv_cur_region&0xffff);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    atv_info_ptr->cur_volume_level=atv_info.cur_volume_level;

    cmd=ATV_CMD_SET_CHN;
    param=((cmd<<0x10)&0xffff0000)|(atv_info.atv_cur_chn&0xffff);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    return 0;
}

/*****************************************************************************/
// Description: dc analog tv callback
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      atv_param: atv cmd
//                      cmd : atv callback cmd
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************/
LOCAL uint32 _DCAMERA_AtvCallback(uint32 atv_param, DCAMERA_CALLBACK_CMD_E cmd)
{
    DCAMERA_INFO_T *pDCamInfo=GetDCAMInfo();

    switch(atv_param)
    {
        case DCAMERA_ATV_CMD_SCAN_CHN_START:
        case DCAMERA_ATV_CMD_SCAN_CHN:
        {
            if(PNULL!=pDCamInfo->atv_search_callback)
            {
                pDCamInfo->atv_call_param.cmd=cmd;
                pDCamInfo->atv_search_callback(&(pDCamInfo->atv_call_param));
            }

            break;
        }
        case DCAMERA_ATV_CMD_SCAN_CHN_STOP:
        {
            if(PNULL!=pDCamInfo->atv_stop_callback)
            {
                pDCamInfo->atv_call_param.cmd=cmd;
                pDCamInfo->atv_stop_callback(&(pDCamInfo->atv_call_param));
            }
            else
            {
                DCAMERA_SetEvent(DCAMERA_ATV_STOP_EVENT);
            }
            
            break;
        }
        case DCAMERA_ATV_CMD_RSSI:
        {
            if(PNULL!=pDCamInfo->atv_callback)
            {
                pDCamInfo->atv_call_param.cmd=cmd;
                pDCamInfo->atv_callback(&(pDCamInfo->atv_call_param));
            }
            break;
        }
        default:
            break;
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
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
/*****************************************************************************/
LOCAL uint32 DCAMERA_AtvScanChnHandler(void)
{
    DCAMERA_SendSignal(DCAMERA_TASK_CMD_ATV,DCAMERA_ATV_CMD_SCAN_CHN, 0 ,0);
        
    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description: dc analog tv Rssi handler
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************/
PUBLIC uint32 DCAMERA_AtvRssiHandler(void)
{
    DCAMERA_SendSignal(DCAMERA_TASK_CMD_ATV,DCAMERA_ATV_CMD_RSSI, 0, 0);
    
    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description: dc analog tv handle
// Global resource dependence: 
// Author: Eddy.wei
// input parameter:
//                      atv_param: atv cmd
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success 
//          other: fail
// Note:
/*****************************************************************************/
PUBLIC uint32 DCAMERA_AtvHandle(uint32 atv_param)
{
    uint32 rtn=SCI_SUCCESS;
    uint32 cmd=0x00;
    uint32 param=0x00;
    DCAMERA_INFO_T *pDCamInfo=GetDCAMInfo();

    switch(atv_param)
    {
        case DCAMERA_ATV_CMD_SCAN_CHN_START:
        {
            DCAMERA_TimerDactive();
            //scan chn init
            cmd=ATV_CMD_SCAN_INIT;
            param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
            rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
            
            if(SCI_SUCCESS!=rtn)
            {
                _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_SCAN_CHN_START, DCAMERA_CALLBACK_ATV_ERR);
                return rtn;
            }
            
            DCAMERA_AtvScanChnHandler();
            break;
        }
        case DCAMERA_ATV_CMD_SCAN_CHN:
        {
            DCAMERA_ATV_BeforeSetting();
            //scan chn start
            cmd=ATV_CMD_SCAN_CHN;
            param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
            rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
            //parse return param
            pDCamInfo->atv_call_param.atv_chn_is_valid=((rtn>>0x08)&0x01);
            pDCamInfo->atv_call_param.atv_chn_id=((rtn>>0x10)&0xffff);

            if(SCI_TRUE!=(rtn&0x01))
            {
                SCI_Sleep(100);
                rtn = DCAMERA_ATV_AfterSetting(SCI_TRUE);

                if(SCI_SUCCESS!=rtn)
                {
                    pDCamInfo->atv_call_param.atv_chn_is_valid=0x00;
                    ATVErrHandler();
                }

                pDCamInfo->atv_call_param.rtn=SCI_SUCCESS;
                _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN);
                DCAMERA_TimerActive(ATV_SCAN_CHN_TIMER, DCAMERA_AtvScanChnHandler);
            }
            else
            {
                DCAMERA_ATV_AfterSetting(SCI_FALSE);
                _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN);
                _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_SCAN_CHN, DCAMERA_CALLBACK_ATV_SCAN_CHN_END);
                DCAMERA_TimerDactive();
            }
            break;
        }
        case DCAMERA_ATV_CMD_SCAN_CHN_STOP:
        {
            DCAMERA_TimerDactive();

            DCAMERA_StopPreview();

            cmd=ATV_CMD_SCAN_CHN_STOP;
            param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
            rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

            _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_SCAN_CHN_STOP, DCAMERA_CALLBACK_ATV_SCAN_CHN_STOP);
            break;
        }
        case DCAMERA_ATV_CMD_RSSI:
        {
            cmd=ATV_CMD_GET_RSSI;
            param=((cmd<<0x10)&0xffff0000)|(param&0xffff);
            rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

            if(pDCamInfo->atv_call_param.atv_rssi!=rtn)
            {
                pDCamInfo->atv_call_param.atv_rssi=rtn;
                _DCAMERA_AtvCallback(DCAMERA_ATV_CMD_RSSI, DCAMERA_CALLBACK_ATV_RSSI);
            }

            DCAMERA_TimerActive(ATV_GET_RSSI_TIMER, DCAMERA_AtvRssiHandler);
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


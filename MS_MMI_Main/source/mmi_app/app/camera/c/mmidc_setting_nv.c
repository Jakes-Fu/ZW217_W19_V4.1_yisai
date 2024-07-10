/*************************************************************************
** File Name:      mmidc_nv.c                                           *
** Author:         ryan.xu                                              *
** Date:           2007/02/28                                           *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:     This file defines the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2007/02/28     ryan.xu          Create.                              *
*************************************************************************/




/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef  CAMERA_SUPPORT
#include "dal_dcamera.h"
//#include "sci_types.h"
#include "mmidc_setting_nv.h"
#include "mmi_modu_main.h"
#include "mmidc_setting.h"

/*the length of dc nv*/
const uint16 dc_nv_len[] =
{
    sizeof(MMIDC_SETTING_NV_INFO_T),
    sizeof(DCAMERA_CAM_NV_INFO)
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_RegSettingNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CAMERA, dc_nv_len, sizeof(dc_nv_len) / sizeof(uint16));
}

/*****************************************************************************/
//  Description : read sensor param nv
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSensorParamNV(uint32 nv_id,
                                        DCAMERA_CAM_NV_INFO *nv_val_ptr /*out*/
                                        )
{
    BOOLEAN ret = FALSE;
    
    MN_RETURN_RESULT_E nv_ret = MN_RETURN_FAILURE;


    if (PNULL != nv_val_ptr)
    {
        MMINV_READ(nv_id, nv_val_ptr, nv_ret);
        if (MN_RETURN_SUCCESS == nv_ret)
        {
            ret = TRUE;
        }
    }
    
    SCI_TRACE_LOW("[MMIDC] ReadSensorParamNV nv_id=%d",\
                  nv_id);
    SCI_TRACE_LOW("[MMIDC] ReadSensorParamNV ret=%d, nv_ret=%d",\
                  ret, nv_ret);

    return ret;                  
}

/*****************************************************************************/
//  Description : write sensor param nv
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void WriteSensorParamNV(uint32 nv_id, 
                                        DCAMERA_CAM_NV_INFO *nv_val_ptr /*in*/
                                        )
{
    SCI_TRACE_LOW("[MMIDC] WriteSensorParamNV nv_id=%d",\
                  nv_id);
                  
    if (PNULL != nv_val_ptr)
    {
        MMINV_WRITE(nv_id, nv_val_ptr);
    }
}

/*****************************************************************************/
//  Description : init sensor
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_InitSensor(BOOLEAN is_force_init,
                                         DCAMERA_CAM_PARAM_T *sensor_param_ptr /*out*/
                                         )
{
    BOOLEAN ret = FALSE;
    
    DCAMERA_RETURN_VALUE_E dcam_ret = DCAMERA_OP_SUCCESS;
    DCAMERA_CAM_NV_INFO nv_param = {0};
    BOOLEAN is_init_sensor = FALSE;
    BOOLEAN read_nv_ret = FALSE;
    
    MMIDC_NV_ITEM_E nv_id = MMINV_SENSOR_INFO;



    if (PNULL == sensor_param_ptr)
    {
        return ret;
    }


    read_nv_ret = ReadSensorParamNV(nv_id, &nv_param);
    if (!read_nv_ret || is_force_init)
    {
        is_init_sensor = TRUE;
    }

    sensor_param_ptr->cam_val_nv = nv_param;

    dcam_ret = DCAMERA_Init(is_init_sensor, sensor_param_ptr); //temp for idle 


    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_InitSensor is_init_sensor=%d", is_init_sensor);
    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_InitSensor dcam_ret=%d", dcam_ret);    
    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_InitSensor is_need_save=%d",\
                 sensor_param_ptr->is_need_save);

    if (DCAMERA_OP_SUCCESS == dcam_ret)
    {
        ret = TRUE;
    }


    if (ret    
       && sensor_param_ptr->is_need_save
       )
    {
        nv_param = sensor_param_ptr->cam_val_nv;
        WriteSensorParamNV(nv_id, &nv_param);
    }
    

    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_InitSensor ret=%d", ret);    
    return ret;
}

#endif  //#ifdef  CAMERA_SUPPORT


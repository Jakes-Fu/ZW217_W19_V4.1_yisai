/*******************************************************************************
 ** FileName:      dc_param_ctrl.c                                             *
 ** Author:        jing.li                                                     *
 ** Date:          2008/12/09                                                  *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                                 *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           NAME                   DESCRIPTION                                   *
 ** 2008/12/09     jing.li                 Created                                      *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "dal_dcamera.h"
#include "sensor_drv.h"
#include "isp_service.h"
#include "dc_app_6x00.h"

#include "dc_preview.h"
#include "dc_misc.h"
#include "dc_param_ctrl.h"
#include "dc_cfg.h" 
#include "gpio_prod_api.h"

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
#define DC_FLASH_WAIT_FRAME 10

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewEnviroment(DCAMERA_PARAM_ENVIRONMENT_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewBrightness(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewContrast(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewSharpness(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewSaturation(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewEffect(DCAMERA_PARAM_EFFECT_E type, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewWBMode(DCAMERA_PARAM_WB_MODE_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewVideoMode(DCAMERA_PARAM_VIDEO_MODE_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _AntiBandingFlicker(DCAMERA_PARAM_FLICKER_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetZoomLevel(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetTargetSize(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetExposureCompensation(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetATVChn(uint32 chn_id, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetATVRegion(uint32 region, DCAMERA_PARAM_CTRL_CALL_BACK callback);
PUBLIC DCAMERA_RETURN_VALUE_E _SetATVVolume(uint32 volume_level, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SwitchImgSenosr(DCAMERA_IMG_SENSOR_MODE mode, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetBackGroundColor(uint32 bg_color_param_ptr, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _FlashCtrl(BOOLEAN on, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL  DCAMERA_RETURN_VALUE_E _FocusCtrl(uint32 param, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL  DCAMERA_RETURN_VALUE_E _ExposureCtrl(uint32 param, DCAMERA_PARAM_CTRL_CALL_BACK callback);
LOCAL DCAMERA_RETURN_VALUE_E _SetSensorRot(
                                    uint32 arg, 
                                    DCAMERA_PARAM_CTRL_CALL_BACK callback);

/*--------------------------- Local Data ------------------------------------*/

LOCAL DCAMERA_PARAM_CTRL_CALL_BACK s_param_ctrl_callback = PNULL;

LOCAL const  pf_dcamera_param_ctrl_process s_param_cmd_array[] = {
    PNULL,                      	            //DCAMERA_PARAM_CTRL_NONE = 0x00,	                                         
    _SetPreviewBrightness,	            //DCAMERA_PARAM_CTRL_BRIGHTNESS,					// 亮度设置              
    _SetPreviewContrast,  	            //DCAMERA_PARAM_CTRL_CONTRAST,					// 对比度设置            
    _SetPreviewSharpness,               //DCAMERA_PARAM_CTRL_SHARPNESS,					// 锐利度设置            
    _SetPreviewSaturation,	            //DCAMERA_PARAM_CTRL_SATURATION,					// 饱和度设置            
    _SetZoomLevel,        	            //DCAMERA_PARAM_CTRL_ZOOM,						// 数码变焦设置          
    _SetPreviewEffect,     	            //DCAMERA_PARAM_CTRL_EFFECT,						// 特效设置              
    _SetPreviewEnviroment,              //DCAMERA_PARAM_CTRL_PREVIEW_MODE,				// 情景模式设置          
    _SetTargetSize,       	            //DCAMERA_PARAM_CTRL_SET_TARGET_SIZE,				// 目标图像大小设置      
    PNULL,              	            //DCAMERA_PARAM_CTRL_CHECK_TARGET_SIZE,			// 目标图片大小检查      
    _SetPreviewWBMode,                  //DCAMERA_PARAM_CTRL_WB_MODE,						// 白平衡模式设置        
    PNULL,                         	    //DCAMERA_PARAM_CTRL_AWB_CAL_ENABLE,				// 自动白平衡校准使能设置
    PNULL,                        	    //DCAMERA_PARAM_CTRL_ISO,							// 感光度                
    _SetExposureCompensation,           //DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION,		// 曝光补偿 
    _AntiBandingFlicker,                // Banding set 
    _SetPreviewVideoMode,               // video mode set
    _SetATVChn,                         // analog tv channel set
    _SetATVRegion,                      // analog tv region set
    _SetATVVolume,                      // analog tv volume set
    _SwitchImgSenosr,                  // switch the image sensor
    _SetBackGroundColor,
    _FlashCtrl,
    _FocusCtrl,
    _ExposureCtrl,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    _SetSensorRot
};

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
DECLARE_GLOBAL_CONTEXT;

/*--------------------------- External Function ------------------------------*/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: set sensor param err handle
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _DC_SetSensorParamErr(DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    if(PNULL!=callback) 
    { 
        uint32 param=DCAMERA_OP_SUCCESS; 
        callback(&param, 0x00); 
    }
    
    return DCAMERA_OP_PARAM_ERR; 
}

/******************************************************************************/
// Description: notify the param ctrl caller that param_ctrl is done
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC int DC_ParamCtrlCallback(uint32 is_success)
{
    DCAMERA_PARAM_CTRL_CALL_BACK callback=s_param_ctrl_callback;
    uint32 param=is_success;

    if(callback)
    {
        callback(&param, 0x00);/*lint !e64*/
        s_param_ctrl_callback=PNULL;
    }

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: get callback fun of param ctrl 
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_PARAM_CTRL_CALL_BACK DC_GetParamCtrlCallBack(void)
{
    return s_param_ctrl_callback;
}

//arg: high 16bit mode; low 16bit rot; 


LOCAL DCAMERA_RETURN_VALUE_E _SetSensorRot(
                                    uint32 arg, 
                                    DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_VMIRROR_ENABLE, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/
    uint32 rot_mode = arg;

    //SCI_TRACE_LOW:"_SetSensorRot, Angle %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_159_112_2_18_1_23_37_178,(uint8*)"d",arg);

    if(DCAMERA_ROTATION_FLIP == rot_mode)
    {

         st_param.arg = 1;
         ISP_ServiceSetSensorParam(&st_param);
	  st_param.cmd = SENSOR_IOCTL_HMIRROR_ENABLE;
	  st_param.arg = 0;

		 
    }
    else if(DCAMERA_ROTATION_180 == rot_mode)
    {
         st_param.arg = 1;
         ISP_ServiceSetSensorParam(&st_param);
	  st_param.cmd = SENSOR_IOCTL_HMIRROR_ENABLE;
	  st_param.arg = 1;                   
    }
    else if(DCAMERA_ROTATION_MIRROR == rot_mode)
    {
         st_param.arg = 0;
         ISP_ServiceSetSensorParam(&st_param);
	  st_param.cmd = SENSOR_IOCTL_HMIRROR_ENABLE;
	  st_param.arg = 1;                     
    }
    else
    {
         st_param.arg = 0;
         ISP_ServiceSetSensorParam(&st_param);
	  st_param.cmd = SENSOR_IOCTL_HMIRROR_ENABLE;
	  st_param.arg = 0;  
    }



    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);

}


/******************************************************************************/
// Description: Set preview Enviroment
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewEnviroment(
                                    DCAMERA_PARAM_ENVIRONMENT_E   mode, 
                                    DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_PREVIEWMODE, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewEnviroment %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_215_112_2_18_1_23_38_179,(uint8*)"d", mode);

    if(DCAMERA_ENVIRONMENT_MAX<=mode)
    {
        //DC_LOG_TRACE:"DCAM: _SetPreviewEnviroment %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_219_112_2_18_1_23_38_180,(uint8*)"d", mode);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=ENVIROMENT_MODE=mode;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview brightness
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewBrightness(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_BRIGHTNESS, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewBrightness %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_241_112_2_18_1_23_38_181,(uint8*)"d", level);

    if(0x07<=level)
    {
        //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewBrightness %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_245_112_2_18_1_23_38_182,(uint8*)"d", level);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=PREVIEW_BRIGHTNESS=level;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview contrast
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewContrast(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_CONTRAST, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewContrast %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_267_112_2_18_1_23_38_183,(uint8*)"d", level);

    if(0x07<=level)
    {
        //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewContrast %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_271_112_2_18_1_23_38_184,(uint8*)"d", level);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=PREVIEW_CONTRAST=level;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview sharpness
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewSharpness(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_SHARPNESS, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewSharpness %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_293_112_2_18_1_23_38_185,(uint8*)"d", level);

    if(0x07<=level)
    {
        //DC_LOG_TRACE:"DCAM: _SetPreviewSharpness %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_297_112_2_18_1_23_38_186,(uint8*)"d", level);
        return _DC_SetSensorParamErr(callback);
    }
    
    st_param.arg=PREVIEW_SHARPNESS=level;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview saturation
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewSaturation(uint32 level, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_SATURATION, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewSaturation %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_319_112_2_18_1_23_38_187,(uint8*)"d", level);

    if(0x07<=level)
    {
        //DC_LOG_TRACE:"DCAM: _SetPreviewSaturation %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_323_112_2_18_1_23_38_188,(uint8*)"d", level);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=PREVIEW_SATURATION=level;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview effect
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewEffect(DCAMERA_PARAM_EFFECT_E type, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_IMAGE_EFFECT, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewEffect %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_345_112_2_18_1_23_38_189,(uint8*)"d", type);

    if(DCAMERA_EFFECT_MAX<=type)
    {
        //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewEffect %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_349_112_2_18_1_23_38_190,(uint8*)"d", type);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=PREVIEW_EFFECT=type;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set preview while balance mode
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewWBMode(DCAMERA_PARAM_WB_MODE_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_SET_WB_MODE, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewWBMode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_371_112_2_18_1_23_38_191,(uint8*)"d", mode);

    if(DCAMERA_WB_MODE_MAX<=mode)
    {
        //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewWBMode %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_375_112_2_18_1_23_38_192,(uint8*)"d", mode);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=PREVIEW_WB_MODE=mode;

    if(PNULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set dc/dv mode
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetPreviewVideoMode(DCAMERA_PARAM_VIDEO_MODE_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_VIDEO_MODE, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetPreviewVideoMode %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_397_112_2_18_1_23_38_193,(uint8*)"d", mode);

    if(DCAMERA_MODE_MAX<=mode)
    {
        //DC_LOG_TRACE:"DCAM: _SetPreviewVideoMode %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_401_112_2_18_1_23_38_194,(uint8*)"d", mode);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=VIDEO_MODE=mode;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    ISP_ServiceSetVideFlag();

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: Set banding flicker mode
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _AntiBandingFlicker(DCAMERA_PARAM_FLICKER_E mode, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_ANTI_BANDING_FLICKER, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_AntiBandingFlicker %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_425_112_2_18_1_23_38_195,(uint8*)"d", mode);

    if(DCAMERA_FLICKER_MAX<=mode)
    {
        //DC_LOG_TRACE:"DCAM: _DCAMERA_AntiBandingFlicker %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_429_112_2_18_1_23_38_196,(uint8*)"d", mode);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=FLICKER_MODE=mode;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }

    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: set img sensor exposure compensation
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetExposureCompensation(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    ISP_SENSOR_PARAM_T st_param = {SENSOR_IOCTL_EXPOSURE_COMPENSATION, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/

    //DC_LOG_TRACE:"DCAM: _DCAMERA_SetExposureCompensation %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_451_112_2_18_1_23_38_197,(uint8*)"d", arg);

    if(DCAMERA_EV_MAX<=arg)
    {
        //DC_LOG_TRACE:"DCAM: _SetExposureCompensation %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_455_112_2_18_1_23_38_198,(uint8*)"d", arg);
        return _DC_SetSensorParamErr(callback);
    }

    st_param.arg=EV_MODE=arg;

    if(NULL!=callback)
    {
        st_param.set_param_success=DC_ParamCtrlCallback;
        s_param_ctrl_callback=callback;
    }
    
    return ISP_ServiceSetSensorParam(&st_param);
}

/******************************************************************************/
// Description: zoom callback
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL int  _zoom_callback(uint32 zoom_ok)
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;

    //DC_LOG_TRACE:"DCAM: _zoom_callback %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_478_112_2_18_1_23_38_199,(uint8*)"d", zoom_ok);

    if(1==zoom_ok)
    {
        pContext->is_zoom_busy = FALSE;
    }

    if(PNULL!=pContext->zoom_callback)
    {
        if(0==zoom_ok)
        {
            //DC_LOG_TRACE:"DCAM: _zoom_callback error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_489_112_2_18_1_23_38_200,(uint8*)"");
            eRet=DCAMERA_OP_ERROR;
        }

        (*pContext->zoom_callback)(eRet, PNULL, 0);
    }

    return eRet;
}

/******************************************************************************/
// Description: Set zoom level
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetZoomLevel(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    int eRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DC_LOG_TRACE:"DCAM: _SetZoomLevel %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_507_112_2_18_1_23_38_201,(uint8*)"d", arg);

    if(DC_IN_STATE(DC_STATE_PREVIEWING))
    {
        if(pContext->is_zoom_busy)
        {
            pContext->zoom_level = arg;
            _zoom_callback(0);
        }
        else
        {
            if(PNULL==callback)
            {
                //DC_LOG_TRACE:"DCAM: _SetZoomLevel callback is pnull error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_520_112_2_18_1_23_38_202,(uint8*)"");
                return DCAMERA_OP_ERROR;
            }
            pContext->zoom_callback = callback;/*lint !e64*/		
            pContext->is_zoom_busy = TRUE;
            //send zoom request to ISP
            ISP_ServiceZoom(arg,_zoom_callback);
            pContext->zoom_level=arg;
        }
    }
    else
    {
        pContext->zoom_level = arg;
        pContext->zoom_callback = callback;/*lint !e64*/
        _zoom_callback(1);
    }

    return eRet;
}

/******************************************************************************/
// Description: Set Analog TV channel
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL int32 _DC_SetATVChnHandler(uint32 is_success)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    DCAMERA_PARAM_CTRL_CALL_BACK callback = s_param_ctrl_callback;

    if(DCAMERA_OP_SUCCESS==is_success)
    {
        DC_SendMsg(DCAM_MSG_ATV, DC_ATV_CMD_DECT_REGION);

        if(NULL==callback)
        {
            if(DCAMERA_OP_SUCCESS!=DC_GetEvent(DC_PARAM_CTRL_EVENT, DC_PARAM_CTRL_TIMEOUT))
            {
                rtn=DCAMERA_OP_IOCTL_ERR;
            }
        }
        else
        {
            pContext->param_callback=callback;
        }
    }
    else
    {
        if(NULL!=callback)
        {
            callback(NULL, DCAMERA_OP_SUCCESS);
        }
    }

    return rtn;
}

/******************************************************************************/
// Description: Set Analog TV channel
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetATVChn(uint32 chn_id, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    ISP_SENSOR_PARAM_T st_param={SENSOR_IOCTL_ATV, 0, 0, Sensor_Ioctl, NULL};/*lint !e64*/
    uint32 op_mode=(chn_id&0xffff0000)>>0x10;

    //DC_LOG_TRACE:"DCAM: _SetATVChn op_mode 0x%x, id 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_585_112_2_18_1_23_38_203,(uint8*)"dd", op_mode, (chn_id&0xffff));
    
    st_param.arg=(ATV_CMD_SET_CHN<<0x10)&0xffff0000;
    st_param.arg|=(chn_id&0xffff);

    if(DCAMERA_ATV_CHN_NORMAL_OP==op_mode)
    {
        st_param.set_param_success=_DC_SetATVChnHandler;
    }
    else if(DCAMERA_ATV_CHN_NOLY_SET==op_mode)
    {
        st_param.set_param_success=callback;
    }

    s_param_ctrl_callback=callback;

    if(DCAMERA_ATV_CHN_NOLY_DECT!=op_mode)
    {
        rtn=ISP_ServiceSetSensorParam(&st_param);
    }
    else
    {
        rtn=_DC_SetATVChnHandler(DCAMERA_OP_SUCCESS);
    }

    return rtn;
}

/******************************************************************************/
// Description: Set Analog TV Region
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetATVRegion(uint32 region, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 param=0x00;
    uint32 rtn=SCI_SUCCESS;

    param=(ATV_CMD_SET_REGION<<0x10)&0xffff0000;
    param|=(region&0xffff);
    //DC_LOG_TRACE:"DCAM: _SetATVRegion 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_623_112_2_18_1_23_38_204,(uint8*)"d", param);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    if(NULL!=callback)
    {
        callback(NULL, NULL);  
    }

    return rtn;
}

/******************************************************************************/
// Description: Set Analog TV voulme
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC  DCAMERA_RETURN_VALUE_E _SetATVVolume(uint32 volume_level, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 param=0x00;
    uint32 rtn=SCI_SUCCESS;

    param=(ATV_CMD_SET_VOLUME<<0x10)&0xffff0000;
    param|=(volume_level&0xffff);
    //DC_LOG_TRACE:"DCAM: _SetATVVolume 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_644_112_2_18_1_23_38_205,(uint8*)"d", param);
    rtn=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);

    if(NULL!=callback)
    {
        callback(NULL, NULL);  
    }

    return rtn;
}

/******************************************************************************/
// Description: sticth image sensor
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _SwitchImgSenosr(DCAMERA_IMG_SENSOR_MODE mode, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 rtn=SCI_SUCCESS;

    if(mode!=Sensor_GetCurId())
    {
        rtn = Sensor_SetCurId(mode);/*lint !e64*/
        if(rtn)  
        {
            rtn = DCAMERA_OP_ERROR;
        }
    }

    if(NULL!=callback)
    {
        callback(NULL, NULL);  
    }

    return rtn;
}

/******************************************************************************/
// Description: set background color
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _SetBackGroundColor(uint32 bg_color_param_ptr, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    DCAMERA_DISP_BG_COLOR_T_PTR bg_color_ptr=(DCAMERA_DISP_BG_COLOR_T_PTR)bg_color_param_ptr;
    uint32 rtn=SCI_SUCCESS;

    if(NULL!=bg_color_ptr)
    {
        pContext->bg_color.r=bg_color_ptr->r;
        pContext->bg_color.g=bg_color_ptr->g;
        pContext->bg_color.b=bg_color_ptr->b;
    }

    if(NULL!=callback)
    {
        callback(NULL, NULL);
    }

    return rtn;
}

/******************************************************************************/
// Description: Dc flash ctrol
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _FlashCtrl(BOOLEAN on, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 rtn=SCI_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();

    //DC_LOG_TRACE:"DCAM: _FlashCtrl 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_712_112_2_18_1_23_39_206,(uint8*)"d", on);

    if(on!=pContext->flash_eb)
    {
        if(DC_PRODUCT_FLASH_TYPE_LED==pDCProductCfg->flash_type)
        {
            GPIO_SetDCFlash(on);

            if(SCI_TRUE==on)
            {
                ISP_SENSOR_FRAME_COUNTER_T param={0x00};

                param.num=DC_FLASH_WAIT_FRAME;
                if(NULL!=callback)
                {
                    s_param_ctrl_callback=callback;
                    param.callback=DC_ParamCtrlCallback;
                }
                else
                {
                    param.callback=PNULL;
                }
                ISP_ServiceGetSensorFrameCounter(&param);
            }
        }
        else if(DC_PRODUCT_FLASH_TYPE_XENON==pDCProductCfg->flash_type)
        {

        }
    }

    pContext->flash_eb=on;

    return rtn;
}
/******************************************************************************/
// Description: Dc focus ctrol
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _FocusCtrl(uint32 param, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 rtn=SCI_SUCCESS;
    DCAMERA_PARAM_CTRL_T_PTR ctrl_ptr=(DCAMERA_PARAM_CTRL_T_PTR)param;
    SENSOR_EXT_FUN_T ext_fun_param={0x00};

    //DC_LOG_TRACE:"DCAM: _FocusCtrl : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_756_112_2_18_1_23_39_207,(uint8*)"d", ctrl_ptr->param);

    if(NULL==ctrl_ptr)
    {
        //DC_LOG_TRACE:"DCAM: _FocusCtrl param is null error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_760_112_2_18_1_23_39_208,(uint8*)"");
        if(PNULL!=callback)
        {
            callback(SCI_ERROR, 0x00);
        }
        return SCI_ERROR;
    }

    ext_fun_param.cmd=SENSOR_EXT_FOCUS_START;
    ext_fun_param.param=ctrl_ptr->param;

    //calc point
    if(DCAMERA_FOCUS_ZONE==ctrl_ptr->param)
    {
        rtn=DC_LcdPointMatchToImg(&ctrl_ptr->lcd_point, &ext_fun_param.zone);
    }

    if(SCI_SUCCESS==rtn)
    {
        rtn=Sensor_Ioctl(SENSOR_IOCTL_EXT_FUNC, (uint32)&ext_fun_param);
    }
    else
    {
        rtn=DCAMERA_OP_ERROR;
    }

    if(PNULL!=callback)
    {
        callback(rtn, 0x00);
    }

    return rtn;
}

/******************************************************************************/
// Description: Dc exposure ctrol
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL  DCAMERA_RETURN_VALUE_E _ExposureCtrl(uint32 param, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    uint32 rtn=SCI_SUCCESS;
    DCAMERA_PARAM_CTRL_T_PTR ctrl_ptr=(DCAMERA_PARAM_CTRL_T_PTR)param;
    SENSOR_EXT_FUN_T ext_fun_param={0x00};

    //DC_LOG_TRACE:"DCAM: _ExposureCtrl : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_803_112_2_18_1_23_39_209,(uint8*)"d", ctrl_ptr->param);

    if(NULL==ctrl_ptr)
    {
        //DC_LOG_TRACE:"DCAM: _ExposureCtrl param is null error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_807_112_2_18_1_23_39_210,(uint8*)"");
        if(PNULL!=callback)
        {
            callback(SCI_ERROR, 0x00);
        }
        return SCI_ERROR;
    }

    ext_fun_param.cmd=SENSOR_EXT_EXPOSURE_START;
    ext_fun_param.param=ctrl_ptr->param;

    //calc point
    if(DCAMERA_EXPOSURE_ZONE==ctrl_ptr->param)
    {
        rtn=DC_LcdPointMatchToImg(&ctrl_ptr->lcd_point, &ext_fun_param.zone);
    }

    if(SCI_SUCCESS==rtn)
    {
        rtn=Sensor_Ioctl(SENSOR_IOCTL_EXT_FUNC, (uint32)&ext_fun_param);
    }
    else
    {
        rtn=DCAMERA_OP_ERROR;
    }

    if(PNULL!=callback)
    {
        callback(rtn, 0x00);
    }

    return rtn;
}

/******************************************************************************/
// Description: change target size
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
LOCAL DCAMERA_RETURN_VALUE_E _SetTargetSize(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_ERROR;
    DCAMERA_SIZE_T* st_sensor_size_ptr = DC_GetSensorSizeStPtr(arg);

    //DC_LOG_TRACE:"DCAM: _SetTargetSize %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_849_112_2_18_1_23_39_211,(uint8*)"d", arg);

    if(DCAMERA_PHOTO_SIZE_MAX<=arg)
    {
        //DC_LOG_TRACE:"DCAM: _SetTargetSize arg:%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_853_112_2_18_1_23_39_212,(uint8*)"d", arg);
        return DCAMERA_OP_ERROR;
    }
    
    if(!DC_IN_STATE(DC_STATE_PREVIEWING))
    {
        //DC_LOG_TRACE:"DCAM: _SetTargetSize - just record for not preview"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_859_112_2_18_1_23_39_213,(uint8*)"");
        SNAPSHOT_TARGET_WIDTH = st_sensor_size_ptr->w;
        SNAPSHOT_TARGET_HEIGHT = st_sensor_size_ptr->h;
        DC_SetCaptureImgSensorMode();
        ret = DCAMERA_OP_SUCCESS;
    }
    else
    {
        /* if it is previewing, check whether the target size changes. */
        if((SNAPSHOT_TARGET_WIDTH== st_sensor_size_ptr->w) &&
            (SNAPSHOT_TARGET_HEIGHT== st_sensor_size_ptr->h))
        {
            //DC_LOG_TRACE:"DCAM: _SetTargetSize - do nothing in preview"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_871_112_2_18_1_23_39_214,(uint8*)"");
            /* no change, then return */
            ret = DCAMERA_OP_SUCCESS;
        }
        else
        {
            /*
                the target size changes
                record the original config
            */
            DCAMERA_PREVIEW_PARAM_T st_preview_param = PREVIEW_PARAM_ST;
            uint32 original_preview_mode = PREVIEW_MODE;
            uint32 original_zoom_level = ZOOM_LEVEL;
            PREVIEW_OPS_HANDLER_T* preview_handlers = DC_Preview_GetOpsHandler();

            //DC_LOG_TRACE:"DCAM: _SetTargetSize - need to change in preview"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_886_112_2_18_1_23_39_215,(uint8*)"");

            SNAPSHOT_TARGET_WIDTH=st_sensor_size_ptr->w;
            SNAPSHOT_TARGET_HEIGHT=st_sensor_size_ptr->h;

            /* set zoom level to zero when target size changes */
            ZOOM_LEVEL = 0;
            
            /* calculate the parameter, same as in DCAMERA_StartPreview */
            st_preview_param.disp_rect = PREVIEW_DISP_RECT;

            ret=preview_handlers->set_param(&st_preview_param);
            DC_RETURN_VAL_IF_FAIL(ret, DCAMERA_OP_PARAM_ERR);

            if((original_zoom_level != ZOOM_LEVEL) ||
              (original_preview_mode != PREVIEW_MODE))
            {
                /* the zoom level changes, it must restart preview*/
                preview_handlers->stop();

                /*check if the sensor mode need change */
                if(original_preview_mode != PREVIEW_MODE) 
                {
                    DC_RETURN_VAL_IF_FAIL_WARNING(Sensor_SetMode(PREVIEW_MODE),\
                                                        DCAMERA_OP_SENSOR_NOT_WORK_ERR,\
                                                        ("DCAM, DCAMERA_StartPreview - set sensor mode error"));
                }

                /* restart with new config */
                preview_handlers->start();
            }
            ret = DCAMERA_OP_SUCCESS;
        }
    }

    if(callback)
         callback(0, 0);

    return ret;
}

/******************************************************************************/
// Description: set sensor param all
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC uint32 DC_SetSensorParamAll(void)
{
    _SetPreviewBrightness(PREVIEW_BRIGHTNESS, PNULL);
    _SetPreviewContrast(PREVIEW_CONTRAST, PNULL);
    //_SetPreviewSharpness(PREVIEW_SHARPNESS, PNULL);
    _SetPreviewSaturation(PREVIEW_SATURATION, PNULL);
    _SetPreviewEffect(PREVIEW_EFFECT, PNULL);
    _SetPreviewEnviroment(ENVIROMENT_MODE, PNULL);
    _SetPreviewWBMode(PREVIEW_WB_MODE, PNULL);
    _SetExposureCompensation(EV_MODE, PNULL);
    _AntiBandingFlicker(FLICKER_MODE, PNULL);
    _SetPreviewVideoMode(VIDEO_MODE, PNULL);
    
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: Get the parameter control process function according to the input command
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC pf_dcamera_param_ctrl_process DC_GetParamCtrlFunc(uint32 cmd)
{
    if(DCAMERA_PARAM_CTRL_MAX<=cmd)
    {
        //DC_LOG_TRACE:"DCAM: DC_GetParamCtrlFunc cmd %d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PARAM_CTRL_953_112_2_18_1_23_39_216,(uint8*)"d", cmd);
        return PNULL;
    }

    return s_param_cmd_array[cmd];/*lint !e661*//*lint !e662*/
}


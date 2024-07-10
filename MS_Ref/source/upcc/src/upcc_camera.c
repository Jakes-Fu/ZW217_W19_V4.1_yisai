/******************************************************************************
 ** File Name:    UPCC_camera.c                                               *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/23/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file contain the dc interface for PC Camera.           *
 ******************************************************************************/
/*****************************************************************************
**                   Edit    History                                         *
**---------------------------------------------------------------------------*
** DATE          NAME            DESCRIPTION                                 *
** 3/23/2006     yuhua.shi       Create.                                     *
** 1/03/2008     Liangwen.Zhen   Modify                                      *
******************************************************************************/
#ifndef _UPCC_CAMERA_C_
#define _UPCC_CAMERA_C_

/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */
#include "ms_ref_upcc_trc.h"
#include "upcc_camera.h"
#include "upcc_api.h"
#include "upcc_drv.h"
#include "dal_jpeg.h"
#include "dal_dcamera.h"
#include "sensor_drv.h"
#include "dal_jpeg.h"
//#include "UDC_api.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         DATA STRUCTURE                                    *
 **---------------------------------------------------------------------------*/
typedef enum
{
    UPCC_DATA_TYPE_JPEG = 0x00,
    UPCC_DATA_TYPE_YUV,

    UPCC_DATA_TYPE_MAX
} UPCC_DATA_TYPE_E;

typedef struct upcc_camera_cfg_info_tag
{
    UPCC_DATA_TYPE_E data_type_from_record;
} UPCC_CAMERA_CFG_INFO_T;

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/
LOCAL UPCC_SETTING_INFO_T   s_UPCC_setting_info;
LOCAL uint32                s_upcc_frame_count = 0;

uint8 *g_UPCC_jpeg_malloc_buf = NULL;
uint8 *g_UPCC_jpeg_buf = NULL;
LOCAL uint32 g_UPCC_jpeg_len = NULL;
SCI_SEMAPHORE_PTR g_UPCC_jpeg_buf_semaphore;

LOCAL BOOLEAN s_UPCC_is_video_start = FALSE;
LOCAL BOOLEAN s_UPCC_is_video_start_by_host = FALSE;
//LOCAL BOOLEAN s_UPCC_is_start_by_self = FALSE;

// need configure according to the special platform
LOCAL const  UPCC_CAMERA_CFG_INFO_T s_UPCC_cfg_info =
{
    UPCC_DATA_TYPE_JPEG
};

/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/
extern UPCC_INIT_CONTROL_INFO_T g_init_control_info;
extern uint8 g_UPCC_ConfigDescr[];
extern BLOCK_ID upcc_main_id;//this task used to collect garbage generated in write operation.
extern BOOLEAN g_UPCC_is_video_streaming_allowed_transmit;
/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
BOOLEAN g_UPCC_is_camera_opened=FALSE;//if camera is closed,it is not allowed to set brightness
extern uint32 g_upcc_setcur_flag;
/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : set video streaming start flag
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL void _UPCC_SetVideoStartFlag (BOOLEAN state);

/*****************************************************************************/
//  Description : get video streaming start flag
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _UPCC_GetVideoStartFlag (void);

/**---------------------------------------------------------------------------*
 **                       LOCAL FUNCTION DEFINITION                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set upcc frame count
//  Global resource dependence :
//  Author: Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL void _UPCC_SetFrameCount (BOOLEAN is_increase)
{
    if (is_increase)
    {
        s_upcc_frame_count++;
    }
    else
    {
        if (s_upcc_frame_count > 0)
        {
            s_upcc_frame_count--;
        }
    }
}

/*****************************************************************************/
//  Description : get upcc frame count
//  Global resource dependence :
//  Author: Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL uint32 _UPCC_GetFrameCount (void)
{
    return s_upcc_frame_count;
}


/*****************************************************************************/
//  Description : reset upcc frame count
//  Global resource dependence :
//  Author: Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL void _UPCC_ResetFrameCount (void)
{
    s_upcc_frame_count = 0;
}

/*****************************************************************************/
//  Description : set video streaming start flag
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL void _UPCC_SetVideoStartFlag (BOOLEAN state)
{
    s_UPCC_is_video_start = state;
    //UPCC_TRACE_LOW:"UPCC_camera.c:SetVideoStartFlag:set %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_145_112_2_18_1_55_16_0,(uint8*)"d", state);
}

/*****************************************************************************/
//  Description : get video streaming start flag
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _UPCC_GetVideoStartFlag (void)
{
    return s_UPCC_is_video_start;
}

/*****************************************************************************/
//  Description : when inttrupt occur,dc send message to upcc
//  Global resource dependence :
//  Author: feifei.gu
//  Note:
/*****************************************************************************/
LOCAL uint32 _UPCC_Notice2Upcc (uint32 ret_value, void *addr, uint32 data_len)
{
    xSignalHeader   signal;

    if (SCI_TRUE!=g_UPCC_is_video_streaming_allowed_transmit)
    {
        return SCI_SUCCESS;
    }

    if (SCI_SUCCESS!=ret_value)
    {
        return SCI_ERROR;
    }

    if (NULL == g_UPCC_jpeg_buf_semaphore)
    {
        return SCI_ERROR;
    }

    if (SCI_GetSemaphore (g_UPCC_jpeg_buf_semaphore,SCI_NO_WAIT) == SCI_SUCCESS)
    {
        SCI_MEMCPY (g_UPCC_jpeg_buf, addr, data_len);/*lint !e718 !e746 -e718 -e746*/
        g_UPCC_jpeg_len = data_len;
        //      UPM_SendCommand (UPM_VIDEO_PC_CAMERA,UPCC_CMD_EXCEPTION,(uint32)data_len,g_UPCC_jpeg_buf);

        signal = SCI_ALLOC (sizeof (*signal));
        signal->SignalCode = UPCC_CMD_EXCEPTION;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, upcc_main_id);
    }
    else
    {
        //SCI_TRACE_LOW:"UPCC_NoticeUpcc: Drop one frame !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_194_112_2_18_1_55_16_1,(uint8*)"");
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description : Set default value of pc camera setting
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL void _UPCC_SetDefaultValue (UPCC_SETTING_INFO_T  *setting_info_ptr, \
                                  UPCC_INIT_SET_INFO_T *init_set_prt)
{
    uint32 features = 0;
        
    UPCC_ASSERT (PNULL != setting_info_ptr);   /*assert verified*/
    UPCC_ASSERT (PNULL != init_set_prt);             /*assert verified*/

    SCI_MEMSET (setting_info_ptr, 0, sizeof (UPCC_SETTING_INFO_T));

    setting_info_ptr->st_contrast.def_value = init_set_prt->contrast;       //UPCC_CONTRAST_DEF;
    setting_info_ptr->st_contrast.cur_value = init_set_prt->contrast;
    setting_info_ptr->st_contrast.min_value = UPCC_CONTRAST_MIN;
    setting_info_ptr->st_contrast.max_value = UPCC_CONTRAST_MAX;

    setting_info_ptr->st_brightness.def_value = init_set_prt->brightness;   //UPCC_BRIGHTNESS_DEF;
    setting_info_ptr->st_brightness.cur_value = init_set_prt->brightness;
    setting_info_ptr->st_brightness.min_value = UPCC_BRIGHTNESS_MIN;
    setting_info_ptr->st_brightness.max_value = UPCC_BRIGHTNESS_MAX;

    DCAMERA_GetInfo (DCAMERA_INFO_UPCC_SIZE,&features);
    setting_info_ptr->e_video_size = (UPCC_VIDEO_SIZE_E)features;           //UPCC_FRAME_SIZE_DEF;
    DCAMERA_GetInfo (DCAMERA_INFO_UPCC_ROTATION,&features);
    setting_info_ptr->e_rotation = (UPCC_VIDEO_ROTATION_E)features;         //UPCC_ROTATION;
    setting_info_ptr->e_frame_rate = init_set_prt->frame_rate;              //UPCC_FRAME_RATE_DEF;
    setting_info_ptr->e_effect     = init_set_prt->hue;                     //UPCC_HUE_DEF;
    setting_info_ptr->e_quality    = UPCC_VIDEO_QUALITY_HIGH;

}

/*****************************************************************************/
//  Description : Set all sensor effect include brightness, contrast and sepia
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _UPCC_SetALLSensorEffect (UPCC_SETTING_INFO_T *setting_ptr)
{
    UPCC_ASSERT (setting_ptr->st_contrast.def_value <= UPCC_CONTRAST_MAX);  /*assert verified*/
    UPCC_ASSERT (setting_ptr->st_brightness.def_value <= UPCC_BRIGHTNESS_MAX); /*assert verified*/
    UPCC_ASSERT (setting_ptr->e_effect <= UPCC_HUE_MAX);   /*assert verified*/

    //set contrast
    if (!UPCC_SetContrastValue (setting_ptr->st_contrast.cur_value))
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetALLSensorEffect:set contrast fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_242_112_2_18_1_55_16_2,(uint8*)"");
        return FALSE;
    }

    //set brightness
    if (!UPCC_SetBrightnessValue (setting_ptr->st_brightness.cur_value))
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetALLSensorEffect:set brightness fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_249_112_2_18_1_55_16_3,(uint8*)"");
        return FALSE;
    }

    //set effect
    if (!UPCC_SetHueValue (setting_ptr->e_effect))
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetALLSensorEffect:set effect fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_256_112_2_18_1_55_16_4,(uint8*)"");
        return FALSE;
    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetALLSensorEffect:success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_260_112_2_18_1_55_16_5,(uint8*)"");


    return TRUE;
}


/**---------------------------------------------------------------------------*
 **                       PUBLIC FUNCTION DEFINITION                          *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get pc camera setting info
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC UPCC_SETTING_INFO_T *UPCC_GetSettingInfo (void)
{
    return &s_UPCC_setting_info;
}

/*****************************************************************************/
//  Description : init dc setting
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_SettingInit (void)
{
    uint32 features = 0;
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    //  uint8  byte;

    UPCC_INIT_SET_INFO_T    init_setting = {  UPCC_BRIGHTNESS_DEF,
                            UPCC_CONTRAST_DEF,
                            UPCC_HUE_DEF,
                            UPCC_FRAME_RATE_DEF,
                            UPCC_FRAME_SIZE_DEF
                                           };

    //set the default setting value
    UPCC_InitPCCamera (&init_setting);

    //set dc control flag

    _UPCC_SetVideoStartFlag (FALSE);
    UPCC_SetVideoStartFlagByHost (FALSE);
    //s_UPCC_is_start_by_self = FALSE;

    // first disable all endpoint dma

    eRet = DCAMERA_GetInfo (DCAMERA_INFO_BRIGHTNESS,&features);

    if ((features != 0) && (DCAMERA_OP_SUCCESS == eRet))
    {
        g_UPCC_ConfigDescr[0x49] |= 0x01;
    }

    eRet = DCAMERA_GetInfo (DCAMERA_INFO_CONTRAST,&features);

    if ((features != 0) && (DCAMERA_OP_SUCCESS == eRet))
    {
        g_UPCC_ConfigDescr[0x49] |= 0x02;
    }

    eRet = DCAMERA_GetInfo (DCAMERA_INFO_SATURATION,&features);

    if ((features != 0) && (DCAMERA_OP_SUCCESS == eRet))
    {
        g_UPCC_ConfigDescr[0x49] |= 0x08;
    }

    eRet = DCAMERA_GetInfo (DCAMERA_INFO_SHARPNESS,&features);

    if ((features != 0) && (DCAMERA_OP_SUCCESS == eRet))
    {
        g_UPCC_ConfigDescr[0x49] |= 0x10;
    }

    eRet = DCAMERA_GetInfo (DCAMERA_INFO_WB_MODE,&features);

    if ((features != 0) && (DCAMERA_OP_SUCCESS == eRet))
    {
        g_UPCC_ConfigDescr[0x49] |= 0x40;
    }

    if ( (features&0x01) != 0)
    {
        g_UPCC_ConfigDescr[0x4A] |= 0x10;
    }
}

/*****************************************************************************/
//  Description : set the start flag of video started by host
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_SetVideoStartFlagByHost (BOOLEAN flag)
{
    s_UPCC_is_video_start_by_host = flag;
    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetVideoStartFlagByHost:set %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_357_112_2_18_1_55_16_6,(uint8*)"d", flag);
}

/*****************************************************************************/
//  Description : get the start flag of video started by host
//  Global resource dependence :
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_IsVideoStartByHost (void)
{
    return s_UPCC_is_video_start_by_host;
}

/*****************************************************************************/
//  Description: set the init control value for pc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_SetInitControl (UPCC_INIT_SET_INFO_T *init_set_prt)
{
    UPCC_ASSERT (init_set_prt != PNULL);   /*assert verified*/

    g_init_control_info.brightness.def_value      = init_set_prt->brightness;
    g_init_control_info.contrast.def_value        = init_set_prt->contrast;
    g_init_control_info.hue.def_value             = init_set_prt->hue;
    g_init_control_info.frame_rate.def_value      = init_set_prt->frame_rate;
    g_init_control_info.frame_size.def_value      = init_set_prt->frame_size;
}

/*****************************************************************************/
//  Description: init the pc camera
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_InitPCCamera (UPCC_INIT_SET_INFO_T *init_set_prt)
{
    UPCC_SETTING_INFO_T     *setting_ptr = PNULL;

    UPCC_ASSERT (init_set_prt != PNULL); /*assert verified*/

    setting_ptr = UPCC_GetSettingInfo();
    UPCC_ASSERT (setting_ptr != PNULL);   /*assert verified*/

    //UPCC_TRACE_LOW:"upcc_camera:UPCC_InitPCCamera"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_397_112_2_18_1_55_16_7,(uint8*)"");

    //set the default setting value for dc
    _UPCC_SetDefaultValue (setting_ptr, init_set_prt);

    //set the init control value for pc
    UPCC_SetInitControl (init_set_prt);

}

/*****************************************************************************/
//  Description: start the pc camera
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_OpenPCCamera (void)
{
    BOOLEAN                     ret_val = SCI_TRUE;

    if (DCAMERA_OP_SUCCESS != DCAMERA_Open (DCAMERA_OP_UPCC))
    {
        //SCI_TRACE_LOW:"UPCC_StartVideoStreaming: Open DC fail !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_417_112_2_18_1_55_16_8,(uint8*)"");
        return SCI_FALSE;
    }

    g_UPCC_is_camera_opened=TRUE;
    return ret_val;
}

/*****************************************************************************/
//  Description: stop the pc camera
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ClosePCCamera (void)
{
    /*stop by self*/
    //s_UPCC_is_start_by_self = FALSE;

    //Stop Video Streaming
    if (!UPCC_StopVideoStreaming())
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_StopVideoStreaming fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_437_112_2_18_1_55_16_9,(uint8*)"");
        return FALSE;
    }

    //close DCAMERA
    if (DCAMERA_OP_SUCCESS != DCAMERA_Close())
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:DC_Close fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_444_112_2_18_1_55_16_10,(uint8*)"");
        return FALSE;
    }

    g_UPCC_is_camera_opened=FALSE;
    g_upcc_setcur_flag = FALSE;
    return TRUE;
}

/*****************************************************************************/
//  Description: start the video streaming of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_StartVideoStreaming (void)
{
    UPCC_SETTING_INFO_T         *setting_ptr = PNULL;
    DCAMERA_RECORD_PARAM_T      rec_param = {0};
    DCAMERA_RETURN_VALUE_E      ret_value = DCAMERA_OP_SUCCESS;
    SENSOR_EXP_INFO_T           *sensor_info_ptr = PNULL;

    /*only permit to start vidoe preview after setting by application*/
#if 0

    if (!s_UPCC_is_start_by_self)
    {
        return FALSE;
    }

#endif

    if (_UPCC_GetVideoStartFlag())
    {
        //SCI_TRACE_LOW:"-- UPCC_StartVideoStreaming(): Already run. ---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_476_112_2_18_1_55_16_11,(uint8*)"");

        return TRUE;

    }

    /*
        if(DCAMERA_OP_SUCCESS != DCAMERA_Open(DCAMERA_OP_DV))
        {
            //SCI_TRACE_LOW:"UPCC_StartVideoStreaming: Open DC fail !!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_485_112_2_18_1_55_16_12,(uint8*)"");
            return SCI_FALSE;
        }
    */

    setting_ptr = UPCC_GetSettingInfo();
    {
        UPCC_ASSERT (setting_ptr != PNULL);  /*assert verified*/
    }

    //UPCC_TRACE_LOW:"---------UPCC_StartVideoStreaming()--------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_495_112_2_18_1_55_16_13,(uint8*)"");


    _UPCC_ResetFrameCount();

    rec_param.data_buf_ptr      = PNULL;
    rec_param.data_buf_len      = NULL;
    rec_param.enc_quality_level = setting_ptr->e_quality;

    if (UPCC_DATA_TYPE_YUV == s_UPCC_cfg_info.data_type_from_record)
    {
        rec_param.out_put_data_type = DCAMERA_DATA_TYPE_YUV422;
        //SCI_TRACE_LOW:"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_YUV"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_507_112_2_18_1_55_16_14,(uint8*)"");
    }
    else if (UPCC_DATA_TYPE_JPEG == s_UPCC_cfg_info.data_type_from_record)
    {
        rec_param.out_put_data_type = DCAMERA_DATA_TYPE_JPEG;
        //SCI_TRACE_LOW:"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_512_112_2_18_1_55_16_15,(uint8*)"");
    }
    else
    {
        rec_param.out_put_data_type = DCAMERA_DATA_TYPE_JPEG;
        //SCI_TRACE_LOW:"UPCC_StartVideoStreaming:  DCAMERA_DATA_TYPE_JPEG"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_517_112_2_18_1_55_16_16,(uint8*)"");
    }

    rec_param.target_size.x     = 0;
    rec_param.target_size.y     = 0;

    if (UPCC_VIDEO_SIZE_160X128 == setting_ptr->e_video_size)
    {
        rec_param.target_size.w = 160;
        rec_param.target_size.h = 128;
    }
    else if (UPCC_VIDEO_SIZE_320X240 == setting_ptr->e_video_size)
    {
        rec_param.target_size.w = 320;
        rec_param.target_size.h = 240;

        g_UPCC_ConfigDescr[0x86] = 0x40;
        g_UPCC_ConfigDescr[0x87] = 0x01;
        g_UPCC_ConfigDescr[0x88] = 0xF0;
        g_UPCC_ConfigDescr[0x89] = 0;
        g_UPCC_ConfigDescr[0xAC] = 0x40;
        g_UPCC_ConfigDescr[0xAD] = 0x01;
        g_UPCC_ConfigDescr[0xAE] = 0xF0;
        g_UPCC_ConfigDescr[0xAF] = 0;
        g_UPCC_ConfigDescr[0xD2] = 0x40;
        g_UPCC_ConfigDescr[0xD3] = 0x01;
        g_UPCC_ConfigDescr[0xD4] = 0xF0;
        g_UPCC_ConfigDescr[0xD5] = 0;
        g_UPCC_ConfigDescr[0xD6] = 0x40;
        g_UPCC_ConfigDescr[0xD7] = 0x01;
        g_UPCC_ConfigDescr[0xD8] = 0xF0;
        g_UPCC_ConfigDescr[0xD9] = 0;
        g_UPCC_ConfigDescr[0xDA] = 0x40;
        g_UPCC_ConfigDescr[0xDB] = 0x01;
        g_UPCC_ConfigDescr[0xDC] = 0xF0;
        g_UPCC_ConfigDescr[0xDD] = 0;
    }
    else
    {
        rec_param.target_size.w = 240;
        rec_param.target_size.h = 320;

        g_UPCC_ConfigDescr[0x86] = 0xF0;
        g_UPCC_ConfigDescr[0x87] = 0;
        g_UPCC_ConfigDescr[0x88] = 0x40;
        g_UPCC_ConfigDescr[0x89] = 0x01;
        g_UPCC_ConfigDescr[0xAC] = 0xF0;
        g_UPCC_ConfigDescr[0xAD] = 0;
        g_UPCC_ConfigDescr[0xAE] = 0x40;
        g_UPCC_ConfigDescr[0xAF] = 0x01;
        g_UPCC_ConfigDescr[0xD2] = 0xF0;
        g_UPCC_ConfigDescr[0xD3] = 0;
        g_UPCC_ConfigDescr[0xD4] = 0x40;
        g_UPCC_ConfigDescr[0xD5] = 0x01;
        g_UPCC_ConfigDescr[0xD6] = 0xF0;
        g_UPCC_ConfigDescr[0xD7] = 0;
        g_UPCC_ConfigDescr[0xD8] = 0x40;
        g_UPCC_ConfigDescr[0xD9] = 0x01;
        g_UPCC_ConfigDescr[0xDA] = 0xF0;
        g_UPCC_ConfigDescr[0xDB] = 0;
        g_UPCC_ConfigDescr[0xDC] = 0x40;
        g_UPCC_ConfigDescr[0xDD] = 0x01;
    }
    
    rec_param.rotation_mode = (DCAMERA_ROTATION_MODE_E)setting_ptr->e_rotation;

    rec_param.record_callback = (DCAMERA_CALL_BACK) _UPCC_Notice2Upcc;
    ret_value = DCAMERA_StartRecord (&rec_param);

    if (DCAMERA_OP_SUCCESS != ret_value)
    {
        return FALSE;
    }

    _UPCC_SetVideoStartFlag (TRUE);

    sensor_info_ptr = Sensor_GetInfo();

    if (sensor_info_ptr->image_format == SENSOR_IMAGE_FORMAT_RAW)
    {
        if (!_UPCC_SetALLSensorEffect (setting_ptr))
        {
            //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetALLSensorEffect wrong."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_550_112_2_18_1_55_16_17,(uint8*)"");
            return FALSE;
        }
    };

    //reset cache
    UPCC_ResetCache();

    return TRUE;
}

/*****************************************************************************/
//  Description: stop the video streaming of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_StopVideoStreaming (void)
{
    if (!_UPCC_GetVideoStartFlag())
    {
        //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_StopVideoStreaming:video is not start."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_569_112_2_18_1_55_16_18,(uint8*)"");
        return TRUE;
    }

    _UPCC_ResetFrameCount();

    _UPCC_SetVideoStartFlag (FALSE);

    DCAMERA_StopRecord();

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_StopVideoStreaming success."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_579_112_2_18_1_55_16_19,(uint8*)"");

    return TRUE;
}

/*****************************************************************************/
//  Description: set the brightness value to dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetBrightnessValue (uint16 value)
{

    uint32 e_brightness = 0;

    if (value > UPCC_BRIGHTNESS_MAX)
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetBrightnessValue:invalid value."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_595_112_2_18_1_55_16_20,(uint8*)"");
        return FALSE;
    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetBrightnessValue:value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_599_112_2_18_1_55_17_21,(uint8*)"d", value);


    if (_UPCC_GetVideoStartFlag())
    {
        e_brightness = value - UPCC_BRIGHTNESS_MIN;

        if (SCI_SUCCESS != DCAMERA_ParamCtrl (DCAMERA_PARAM_CTRL_BRIGHTNESS, e_brightness, PNULL))
        {
            //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetBrightnessValue fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_608_112_2_18_1_55_17_22,(uint8*)"");
            return FALSE;
        }

        //change the value
        s_UPCC_setting_info.st_brightness.cur_value = value;
    }
    else
    {
        /*it is needed to save the setting value which is obtained from pc
        when the dc is closed because of a coming call or other event.     */
        s_UPCC_setting_info.st_brightness.cur_value = value;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description: get the brightness value from dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetBrightnessValue (uint16 *value, uint8 bRequest)
{
    switch (bRequest)
    {
        case USB_VIDEO_RC_GET_DEF:
            *value = s_UPCC_setting_info.st_brightness.def_value;
            break;
        case USB_VIDEO_RC_GET_CUR:
            *value = s_UPCC_setting_info.st_brightness.cur_value;
            break;
        case USB_VIDEO_RC_GET_MIN:
            *value = s_UPCC_setting_info.st_brightness.min_value;
            break;
        case USB_VIDEO_RC_GET_MAX:
            *value = s_UPCC_setting_info.st_brightness.max_value;
            break;
        default:
            //SCI_TRACE_LOW:"UPCC_camer.c:UPCC_GetBrightnessValue:invalid request"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_646_112_2_18_1_55_17_23,(uint8*)"");
            //UPCC_ASSERT(FALSE);   /*assert to do*/
            break;
    }
}

/*****************************************************************************/
//  Description: set the contrast value to dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetContrastValue (uint16 value)
{
    //@fly disable 2006-09-21

    uint32 e_contrast = 0;

    if (value > UPCC_CONTRAST_MAX)
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetContrastValue:invalid value."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_664_112_2_18_1_55_17_24,(uint8*)"");
        return FALSE;
    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetContrastValue:value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_668_112_2_18_1_55_17_25,(uint8*)"d", value);

    if (_UPCC_GetVideoStartFlag())
    {
        e_contrast = value - UPCC_CONTRAST_MIN;

        if (SCI_SUCCESS != DCAMERA_ParamCtrl (DCAMERA_PARAM_CTRL_CONTRAST, e_contrast, PNULL))
        {
            //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetContrastValue fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_676_112_2_18_1_55_17_26,(uint8*)"");
            return FALSE;
        }

        //change the value
        s_UPCC_setting_info.st_contrast.cur_value = value;
    }
    else
    {
        /*it is needed to save the setting value which is obtained from pc
        when the dc is closed because of a coming call or other event.     */
        s_UPCC_setting_info.st_contrast.cur_value = value;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description: get the contrast value from dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetContrastValue (uint16 *value, uint8 bRequest)
{
    switch (bRequest)
    {
        case USB_VIDEO_RC_GET_DEF:
            *value = s_UPCC_setting_info.st_contrast.def_value;
            break;
        case USB_VIDEO_RC_GET_CUR:
            *value = s_UPCC_setting_info.st_contrast.cur_value;
            break;
        case USB_VIDEO_RC_GET_MIN:
            *value = s_UPCC_setting_info.st_contrast.min_value;
            break;
        case USB_VIDEO_RC_GET_MAX:
            *value = s_UPCC_setting_info.st_contrast.max_value;
            break;
        default:
            //SCI_TRACE_LOW:"UPCC_camer.c:UPCC_GetContrastValue:invalid request"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_714_112_2_18_1_55_17_27,(uint8*)"");
            //UPCC_ASSERT(FALSE);  /*assert to do*/
            break;
    }
}

/*****************************************************************************/
//  Description: set the hue value to dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SetHueValue (uint16 value)
{

    uint32 e_effect = 0;

    if (value > UPCC_HUE_MAX)
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetHueValue:invalid value."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_731_112_2_18_1_55_17_28,(uint8*)"");
        return FALSE;
    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetHueValue:value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_735_112_2_18_1_55_17_29,(uint8*)"d", value);

    if (_UPCC_GetVideoStartFlag())
    {
        e_effect = value ;

        if (SCI_SUCCESS != DCAMERA_ParamCtrl (DCAMERA_PARAM_CTRL_EFFECT, e_effect, PNULL))
        {
            //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetHueValue fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_743_112_2_18_1_55_17_30,(uint8*)"");
            return FALSE;
        }

        //change the value
        s_UPCC_setting_info.e_effect = (UPCC_PHOTO_EFFECT_E) value;
    }
    else
    {
        /*it is needed to save the setting value which is obtained from pc
        when the dc is closed because of a coming call or other event.     */
        s_UPCC_setting_info.e_effect = (UPCC_PHOTO_EFFECT_E) value;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description: get the hue value from dc
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetHueValue (uint16 *value)
{
    *value = (uint16) s_UPCC_setting_info.e_effect;
}

/*****************************************************************************/
//  Description: set the frame size pattern of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_SetFrameSizePattern (UPCC_VIDEO_SIZE_E type)
{

    if (type >= UPCC_VIDEO_SIZE_MAX)
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetFrameSizePattern:invalid value."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_778_112_2_18_1_55_17_31,(uint8*)"");
        return;
    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetFrameSizePattern:value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_782_112_2_18_1_55_17_32,(uint8*)"d", type);

    s_UPCC_setting_info.e_video_size = (UPCC_VIDEO_SIZE_E) type;
    return ;
}

/*****************************************************************************/
//  Description: get the frame size pattern of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetFrameSizePattern (UPCC_VIDEO_SIZE_E *type)
{
    *type = s_UPCC_setting_info.e_video_size;
}

/*****************************************************************************/
//  Description: set the frame rate of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_SetFrameRate (uint8 type)
{

    UPCC_VIDEO_FRAME_RATE_E value;

    if (type > UPCC_VIDEO_FRAME_RATE_MAX)
    {
        //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetFrameRate:invalid value."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_808_112_2_18_1_55_17_33,(uint8*)"");

    }

    //UPCC_TRACE_LOW:"UPCC_camera.c:UPCC_SetFrameRate:value = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_812_112_2_18_1_55_17_34,(uint8*)"d", type);

    value = (UPCC_VIDEO_FRAME_RATE_E) type ;

    if (_UPCC_GetVideoStartFlag())
    {
        if (SCI_SUCCESS != DCAMERA_ParamCtrl (DCAMERA_PARAM_CTRL_PREVIEW_MODE, value, PNULL))
        {
            //SCI_TRACE_LOW:"UPCC_camera.c:UPCC_SetFrameRate fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_CAMERA_820_112_2_18_1_55_17_35,(uint8*)"");
        }

        //change the value
        s_UPCC_setting_info.e_frame_rate= (UPCC_VIDEO_FRAME_RATE_E) value;

    }
    else
    {
        /*it is needed to save the setting value which is obtained from pc
        when the dc is closed because of a coming call or other event.     */
        s_UPCC_setting_info.e_frame_rate = (UPCC_VIDEO_FRAME_RATE_E) value;
    }


}

/*****************************************************************************/
//  Description: get the frame rate of M-JPEG
//  Global resource dependence:
//  Author: yuhua.shi
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetFrameRate (uint8 *type)
{
    *type = (uint8) s_UPCC_setting_info.e_frame_rate;
}

/*****************************************************************************/
//  Description : Get and send a frame.
//  Global resource dependence :
//  Author: feifei.gu
//  Note:
/*****************************************************************************/
PUBLIC void UPCC_GetAndSendFrame()
{
    //DCAMERA_YUV_CHN_ADDR_T*frame_addr_ptr     =NULL;
    UPCC_JPG_FRAME_T      frame             = {0};

    if (UPCC_JPEG_BUF_MAX_SIZE <= g_UPCC_jpeg_len)
    {
        SCI_ASSERT (0); /*assert verified*/ //this situation had happen one time
    }

    if (0 == g_UPCC_jpeg_len)
    {
        return; //avoid DMA tansfer length equal zero
    }

    frame.p_head = (uint8 *) g_UPCC_jpeg_buf;
    frame.len = g_UPCC_jpeg_len;

    UPCC_SendAFrame (&frame);
}


/**---------------------------------------------------------------------------*/
#endif //_UPCC_CAMERA_C_

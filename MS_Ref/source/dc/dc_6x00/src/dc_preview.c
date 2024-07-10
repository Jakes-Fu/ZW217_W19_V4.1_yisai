/*******************************************************************************
 ** FileName:      dc_preview.c                                                *
 ** Author:        jing.li                                                     *
 ** Date:          2008/12/01                                                  *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                                 *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           NAME                   DESCRIPTION                                   *
 ** 2008/12/01     jing.li                 Created                                      *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "dc_app_6x00.h"
#include "dc_preview.h"
#include "dc_misc.h"

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

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/******************************************************************************
// Description: 	check dc state before preview
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_status(void);

/******************************************************************************
// Description: 	check parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  -1          no need to start again
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_parameters(DCAMERA_PREVIEW_PARAM_T * param_ptr);

/******************************************************************************
// Description: 	set parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		set the parameters to the global context
******************************************************************************/
LOCAL int32 _set_parameters(DCAMERA_PREVIEW_PARAM_T * param_ptr);

/******************************************************************************
// Description: 	start preview actually
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _start_preview(void);

/******************************************************************************
// Description: 	stop preview
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _stop_preview(void);

/******************************************************************************
// Description:     get preview & snapshot mode & info struct for sensor 
// Author:          jing.li
// Input:           param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          none
// Note:            search the appropriate sensor mode for both preview & snapshot
//                  according to preview param(input) & sensor_info(sensor_drv)
******************************************************************************/
LOCAL uint32 _get_preview_mode(DCAMERA_PREVIEW_PARAM_T * param_ptr);

/******************************************************************************
// Description:     get information for isp preview parameter
// Author:          jing.li
// Input:           isp_preview_ptr: preview parameters struct pointer for isp
// Output:		    none
// Return:          none
// Note:            calculate the parameters for isp according to the dc preview parameters
******************************************************************************/
LOCAL uint32 _get_isp_param(ISP_PREVIEW_PARAM_T* isp_preview_ptr);

/*--------------------------- Local Data ------------------------------------*/
LOCAL const  PREVIEW_OPS_HANDLER_T s_preview_ops_handlers=
{
    _check_status,
    _check_parameters,
    _set_parameters,
    _start_preview,
    _stop_preview
};

LOCAL ISP_PREVIEW_PARAM_T s_st_isp_preview_param = {0};

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
DECLARE_GLOBAL_CONTEXT;

/*--------------------------- External Function ------------------------------*/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/******************************************************************************
// Description: 	get preview operation handlers
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          preview operation handler struct pointer
// Note:       		
******************************************************************************/
PUBLIC PREVIEW_OPS_HANDLER_T* DC_Preview_GetOpsHandler(void)
{
    return (PREVIEW_OPS_HANDLER_T*)(&s_preview_ops_handlers);
}

/******************************************************************************
// Description: 	check dc state before preview
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_status(void)
{
	/* check if DC is open */
    DC_RETURN_VAL_IF_FALSE_WARNING(DCAMERA_IsOpen(), DCAMERA_OP_NOT_OPEN,\
                                    ("DCAM: DCAMERA Preview, DC is not open, err"));

    return 0;
}

/******************************************************************************
// Description: 	check parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  -1          no need to start again
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_parameters(DCAMERA_PREVIEW_PARAM_T * param_ptr)
{
    uint32 ret = DCAMERA_OP_SUCCESS;

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_CheckDispRect(&param_ptr->disp_rect, &param_ptr->target_rect, param_ptr->rotation_mode), 
                                                        DCAMERA_OP_ERROR, ("DCAM: preview display param check err"));

    if(DC_IN_STATE(DC_STATE_PREVIEWING)
        ||DC_IN_STATE(DC_STATE_ATV_SCAN_CHN))
    {
        if(memcmp(param_ptr, &PREVIEW_PARAM_ST, sizeof(DCAMERA_PREVIEW_PARAM_T)))
        {
            ret = DCAMERA_OP_PARAM_ERR;
            return ret;
        }
        else
        {
            ret = DCAMERA_StopPreview();
        }
    }

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_IDLE), DCAMERA_OP_ERROR, 
                                        ("DCAM:  DC state is not idle, i_ui_status=0x%x, err", DCAM_STATE));

    return ret;
}

/******************************************************************************
// Description: 	set parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		set the parameters to the global context
******************************************************************************/
LOCAL int32 _set_parameters(DCAMERA_PREVIEW_PARAM_T * param_ptr)
{
    
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    uint8 i=0x00;

    PREVIEW_PARAM_ST= *param_ptr;

    PREVIEW_PARAM_ST.capture_mode=DCAMERA_CAPTURE_NORMAL;
    //PREVIEW_PARAM_ST.capture_mode=DCAMERA_CAPTURE_PANORAMA;
    //PREVIEW_PARAM_ST.capture_mode=DCAMERA_CAPTURE_FOURINONE;
    
    if(DCAMERA_CAPTURE_PANORAMA==pContext->preview_param.capture_mode)
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(DC_Malloc(DC_STATE_PANORAMA), DCAMERA_OP_NO_ENOUGH_MEMORY, ("DCAM: Alloc panorama buffer err!"));
    }
    else if(DCAMERA_CAPTURE_FOURINONE==pContext->preview_param.capture_mode)
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(DC_Malloc(DC_STATE_FOURINONE), DCAMERA_OP_NO_ENOUGH_MEMORY, ("DCAM: Alloc FourInOne buffer err!"));
    }
    else
    {// normal preview
        DC_RETURN_VAL_IF_FAIL_WARNING(DC_Malloc(DC_STATE_PREVIEWING), DCAMERA_OP_NO_ENOUGH_MEMORY, ("DCAM: Alloc normal preview buffer err!"));
        
        pContext->combine_num=0x00;
        for(i=0x00; i<MAX_COMBINE_NUM; i++)
        {
            pContext->combine_buf[i].y_addr=NULL;
            pContext->combine_buf[i].uv_addr=NULL;  
        }
    }

    DC_RETURN_VAL_IF_FAIL_WARNING(_get_preview_mode(param_ptr), DCAMERA_OP_PARAM_ERR, ("DCAM: set preview param err"));

    _get_isp_param(&s_st_isp_preview_param);

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description: 	start preview actually
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _start_preview(void)
{
    DCAMERA_RETURN_VALUE_E	eRet = DCAMERA_OP_SUCCESS;

    SET_DCAM_STATE(DC_STATE_PREVIEWING);

    //SENSOR, set mode
    DC_RETURN_VAL_IF_FAIL_WARNING(Sensor_SetMode(PREVIEW_MODE), DCAMERA_OP_SENSOR_NOT_WORK_ERR, ("DCAM: set preview mode sensor err"));

    //ISP,open 
    ISP_ServiceOpen( );

    //ISP,set previewing parameters 
    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetPreviewParam(&s_st_isp_preview_param),DCAMERA_OP_PARAM_ERR, ("DCAM: set preview param err"));

    //ISP,start previewing 
    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartPreview(),DCAMERA_OP_REVIEW_ERR, ("DCAM: start preview err"));

    return eRet;
}

/******************************************************************************
// Description: 	stop preview
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _stop_preview(void)
{
    uint32 eRet = DCAMERA_OP_SUCCESS;

    //DC_LOG_TRACE:"DCAM: _stop_preview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PREVIEW_277_112_2_18_1_23_40_217,(uint8*)"");

    if(DC_IN_STATE(DC_STATE_PREVIEWING)
        ||DC_IN_STATE(DC_STATE_ATV_SCAN_CHN))
    {
        ISP_ServiceFeatureStop();

        SET_DCAM_STATE(DC_STATE_IDLE);
        DC_Free(DC_STATE_PREVIEWING);
        eRet = DCAMERA_OP_SUCCESS;
    }
    else
    {
        //DC_LOG_TRACE:"DCAM: stop preview state err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PREVIEW_290_112_2_18_1_23_40_218,(uint8*)"");
        eRet = DCAMERA_OP_PREVIEW_ERR;
    }

    return eRet;
}

/******************************************************************************
// Description:     get preview & snapshot mode & info struct for sensor 
// Author:          jing.li
// Input:           param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          none
// Note:            search the appropriate sensor mode for both preview & snapshot
//                  according to preview param(input) & sensor_info(sensor_drv)
******************************************************************************/
LOCAL uint32 _get_preview_mode(DCAMERA_PREVIEW_PARAM_T * param_ptr)
{
    uint32 nRtn=DCAMERA_OP_SUCCESS;
    SENSOR_EXP_INFO_T* sensor_info_ptr = Sensor_GetInfo();

    /* check preview mode, only support YUV422 in preview mode*/
    if(SENSOR_IMAGE_FORMAT_YUV422 != sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].image_format)
    {
        //DC_LOG_TRACE:"DCAM: preview data format %x err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PREVIEW_313_112_2_18_1_23_40_219,(uint8*)"d", sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].image_format);
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }; 

    PREVIEW_MODE = SENSOR_MODE_PREVIEW_ONE;
    SNAPSHOT_MODE = SENSOR_MODE_PREVIEW_ONE;
    
    DC_SetCaptureImgSensorMode();

    return nRtn;
}

/******************************************************************************
// Description:     get information for isp preview parameter
// Author:          jing.li
// Input:           isp_preview_ptr: preview parameters struct pointer for isp
// Output:		    none
// Return:          none
// Note:            calculate the parameters for isp according to the dc preview parameters
******************************************************************************/
LOCAL uint32 _get_isp_param(ISP_PREVIEW_PARAM_T* isp_preview_ptr)
{
    DCAM_CONTEXT_T*             pContext = _GetDCAMContext();
    DCAMERA_PREVIEW_PARAM_T*    dc_preview_ptr = &pContext->preview_param;
    DCAMERA_RECT_T*             src_rect_ptr = &pContext->trim_rect;
    DCAMERA_RECT_T*             disp_rect_ptr = &pContext->disp_rect;
    DCAMERA_RECT_T*             target_rect_ptr = &pContext->target_rect;
    ISP_SENSOR_SIGNAL_INFO_T isp_sensor_singnal={0};

    if((NULL==pContext->disp_buf_ptr)
        ||(NULL==pContext->disp_buf_len))
    {
        //DC_LOG_TRACE:"DCAM: preview _get_isp_param display buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PREVIEW_344_112_2_18_1_23_40_220,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    
    *disp_rect_ptr=dc_preview_ptr->disp_rect;
    *target_rect_ptr=dc_preview_ptr->target_rect;

    //set lcd id and lcd block id
    isp_preview_ptr->disp_lcd_id    = dc_preview_ptr->lcd_id;
    isp_preview_ptr->disp_block_id  = dc_preview_ptr->lcd_block_id;
    
    
    isp_preview_ptr->disp_addr = (uint32)pContext->disp_buf_ptr;
    isp_preview_ptr->disp_buf_size = pContext->disp_buf_len;

    isp_preview_ptr->rot_buf_addr = (uint32)pContext->rot_buf_ptr;
    isp_preview_ptr->rot_buf_size = pContext->rot_buf_len;
    
    isp_preview_ptr->disp_rect = *(ISP_RECT_T*)disp_rect_ptr;

    isp_preview_ptr->target_rect = *(ISP_RECT_T*)target_rect_ptr;

    isp_preview_ptr->input_size.w = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].width;
    isp_preview_ptr->input_size.h = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].height;

    isp_preview_ptr->input_rect.x = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_x;
    isp_preview_ptr->input_rect.y = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_y;
    isp_preview_ptr->input_rect.w = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_width;
    isp_preview_ptr->input_rect.h = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_height;
    
    // size of source image
    src_rect_ptr->x = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_x;
    src_rect_ptr->y = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_y;
    src_rect_ptr->w = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_width;
    src_rect_ptr->h = pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_height;

    if(((DCAMERA_CAPTURE_PANORAMA==dc_preview_ptr->capture_mode)||(DCAMERA_CAPTURE_FOURINONE==dc_preview_ptr->capture_mode))
        &&(DCAMERA_DISP_FULLVIEW!=pContext->preview_param.disp_mode))
    {
        //DC_LOG_TRACE:"DCAM: Panorama FourInOne preview distortion %d err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_PREVIEW_383_112_2_18_1_23_40_221,(uint8*)"d", pContext->preview_param.disp_mode);
        return DCAMERA_OP_PARAM_ERR;
    }

    DC_AdjRect_OnDistortion(src_rect_ptr, disp_rect_ptr, target_rect_ptr, dc_preview_ptr->disp_mode, dc_preview_ptr->rotation_mode);

    // Set ISP input rect
    isp_preview_ptr->target_rect=*(ISP_RECT_T*)target_rect_ptr;
    
    isp_preview_ptr->disp_rotation = CONVERT_ROTAION_MODE(dc_preview_ptr->rotation_mode);

    isp_preview_ptr->init_zoom_level = pContext->zoom_level;

    isp_preview_ptr->pclk= pContext->sensor_info_ptr->sensor_mode_info[pContext->preview_mode].pclk;

    DC_GetImgSensorSignalInfo(PREVIEW_MODE,&isp_sensor_singnal);

    ISP_ServiceSetSensorSignalInfo(&isp_sensor_singnal);

    return DCAMERA_OP_SUCCESS;
    
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End

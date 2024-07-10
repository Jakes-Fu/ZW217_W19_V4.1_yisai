/*******************************************************************************
 ** FileName:      dc_preview.c                                                *
 ** Author:        jing.li                                                     *
 ** Date:          2008/12/02                                                  *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2008/12/02     jing.li                 Created                             *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "dc_app_6x00.h"
#include "dc_review.h"
#include "dc_misc.h"
#include "dc_codec.h"

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

LOCAL int32 _check_status(void);
LOCAL int32 _check_parameters(DCAMERA_REVIEW_PARAM_T * param_ptr);
LOCAL int32 _set_parameters(DCAMERA_REVIEW_PARAM_T * param_ptr);
LOCAL int32 _start_review(void);
LOCAL int _get_isp_review_param(ISP_REVIEW_PARAM_T* isp_review_ptr);

/*--------------------------- Local Data ------------------------------------*/
LOCAL const  REVIEW_OPS_HANDLER_T s_review_ops_handlers = {
    _check_status,
    _check_parameters,
    _set_parameters,
    _start_review
};

//LOCAL ISP_REVIEW_PARAM_T s_st_isp_review_param = {0};

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
DECLARE_GLOBAL_CONTEXT;

/*--------------------------- External Function ------------------------------*/

extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/******************************************************************************
// Description: 	get review operation handlers
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          review operation handler struct pointer
// Note:       		
******************************************************************************/
PUBLIC REVIEW_OPS_HANDLER_T* DC_Review_GetOpsHandler(void)
{
    return (REVIEW_OPS_HANDLER_T *)(&s_review_ops_handlers);
}

/******************************************************************************
// Description: 	check dc state before review
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
                                    ("DCAM: %s, DC is not open", FILENAME_STR));

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_IDLE), DCAMERA_OP_ERROR, 
                                        ("DCAM: %s, DC state is not idle, state=0x%x", FILENAME_STR, DCAM_STATE));

    return 0;
}

/******************************************************************************
// Description: 	check parameters before review
// Author:     		jing.li
// Input:      		param_ptr: preview arameters struct pointer
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_parameters(DCAMERA_REVIEW_PARAM_T * param_ptr)
{
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description: 	set parameters before review
// Author:     		jing.li
// Input:      		param_ptr: review parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		set the parameters to the global isp parameter
******************************************************************************/
LOCAL int32 _set_parameters(DCAMERA_REVIEW_PARAM_T * param_ptr)
{
    REVIEW_PARAM_ST= *param_ptr;
    
//    DC_Malloc(DC_STATE_REVIEWING);
//DC_RETURN_IF_FAIL(DC_Malloc(DC_STATE_REVIEWING), ("DCAM: _set_parameters alloc memory fail err"));

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description: 	start review actually
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _start_review(void)
{
    DCAMERA_RETURN_VALUE_E	eRet = DCAMERA_OP_SUCCESS;
    ISP_REVIEW_PARAM_T st_isp_review_param = {0};

    SET_DCAM_STATE(DC_STATE_REVIEWING);

    DC_RETURN_VAL_IF_FAIL_WARNING(_DecodeJpegToYYUV(), DCAMERA_OP_DECODE_ERR, ("DCAM: _start_review jpg decoder err"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_get_isp_review_param(&st_isp_review_param), DCAMERA_OP_PARAM_ERR, ("DCAM: _start_review get review param err"));

    ISP_ServiceOpen();

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetReviewParam(&st_isp_review_param), DCAMERA_OP_PARAM_ERR, ("DCAM: _start_review set review param err"));
    
    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartReview(), DCAMERA_OP_REVIEW_ERR, ("DCAM: _start_review review err"));

    SET_DCAM_STATE(DC_STATE_IDLE);

    return eRet;
}


/******************************************************************************
// Description: 	set isp review parametr base on global context
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int _get_isp_review_param(ISP_REVIEW_PARAM_T* isp_review_ptr)
{
    int	nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T*	pContext	 = _GetDCAMContext();

    isp_review_ptr->input_size.w        = pContext->target_width;
    isp_review_ptr->input_size.h        = pContext->target_height;
    isp_review_ptr->input_rect.x        = 0;
    isp_review_ptr->input_rect.y        = 0;
    isp_review_ptr->input_rect.w        = pContext->target_width;
    isp_review_ptr->input_rect.h        = pContext->target_height;
    isp_review_ptr->input_format        = ISP_DATA_YUV422;
    isp_review_ptr->src_addr.y_addr     = pContext->yyuv_buf.y_addr;
    isp_review_ptr->src_addr.uv_addr    = pContext->yyuv_buf.uv_addr;
    isp_review_ptr->src_len.y_len       = pContext->target_width*pContext->target_height;
    isp_review_ptr->src_len.uv_len      = pContext->target_width*pContext->target_height;
    isp_review_ptr->disp_format         = ISP_DATA_RGB565;
    isp_review_ptr->disp_addr           = (uint32)pContext->disp_buf_ptr;
    isp_review_ptr->disp_buf_size       = pContext->disp_buf_len;
    isp_review_ptr->disp_lcd_id         = pContext->review_param.lcd_id;
    isp_review_ptr->disp_block_id       = pContext->review_param.lcd_block_id;
    //display rect
    isp_review_ptr->disp_rect.x         = pContext->review_param.disp_rect.x;
    isp_review_ptr->disp_rect.y         = pContext->review_param.disp_rect.y;
    isp_review_ptr->disp_rect.w         = pContext->review_param.disp_rect.w;
    isp_review_ptr->disp_rect.h         = pContext->review_param.disp_rect.h;

    isp_review_ptr->target_rect.x = pContext->review_param.target_rect.x;
    isp_review_ptr->target_rect.y = pContext->review_param.target_rect.y;
    isp_review_ptr->target_rect.w = pContext->review_param.target_rect.w;
    isp_review_ptr->target_rect.h = pContext->review_param.target_rect.h;

    isp_review_ptr->rot_buf_len.y_len=pContext->rot_buf_len;
    isp_review_ptr->rot_buf_len.uv_len=DC_Algin((pContext->review_param.disp_rect.w*pContext->review_param.disp_rect.h),0xff, 0x08);
    isp_review_ptr->rot_addr.y_addr=(uint32)pContext->rot_buf_ptr;
    isp_review_ptr->rot_addr.uv_addr=(uint32)(pContext->rot_buf_ptr+isp_review_ptr->rot_buf_len.uv_len);

    isp_review_ptr->disp_rotation = CONVERT_ROTAION_MODE(pContext->review_param.rotation_mode);
    //isp_review_ptr->callback	= _display_callback;

    //DC_LOG_TRACE:"DCAM: pContext->review_param.disp_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_REVIEW_203_112_2_18_1_23_41_222,(uint8*)"d",pContext->review_param.disp_mode);

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&isp_review_ptr->input_rect, 
                            (DCAMERA_RECT_T*)&isp_review_ptr->disp_rect, 
                            (DCAMERA_RECT_T*)&isp_review_ptr->target_rect, 
                            pContext->review_param.disp_mode,
                            pContext->review_param.rotation_mode);

#if 1
    //DC_LOG_TRACE:"DCAM: _adj_disp_mode - rotation mode: %d, disp size: %d, %d, %d, %d;  trim size: %d, %d, %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_REVIEW_215_112_2_18_1_23_41_223,(uint8*)"ddddddddd", isp_review_ptr->disp_rotation,isp_review_ptr->disp_rect.x, isp_review_ptr->disp_rect.w, isp_review_ptr->disp_rect.y, isp_review_ptr->disp_rect.h,isp_review_ptr->input_rect.x, isp_review_ptr->input_rect.w, isp_review_ptr->input_rect.y, isp_review_ptr->input_rect.h);
#endif

    return nRet;
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End

/*****************************************************************************
** File Name:      guianim_ref.c                                             *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2009      Jassmine              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "img_dec_interface.h"
#include "graphics_draw.h"
#include "mmk_msg.h"
#include "ctrlanim.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : convert decode/encode result to mmi result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ConvertResult(
                                     IMG_DEC_RET_E  dec_result
                                     );

/*****************************************************************************/
//  Description : asynchronous deoce/encode image callback function
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void AsynGetFrameCallbackFunc(
                                    uint32                      img_handle,
                                    IMG_DEC_RET_E               dec_ret,
                                    IMG_DEC_FRAME_OUT_PARAM_T   *output_ptr,
                                    uint32                      param
                                    );

/*****************************************************************************/
//  Description : delete get cnf msg from queue
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteGetDataCnf(
                               void             *in_param_ptr,  //in
                               MMI_MESSAGE_T    *msg_ptr        //in
                               );
/*****************************************************************************/
//  Description : get ico frame index
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetIcoFrameIndex(
                            GUI_RECT_T          *rect_ptr,       //in:ico文件需要匹配rect;                                       
                            IMG_DEC_ICO_INFO_T  *img_info_ptr   //in/out:
                            );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : creat deoce/encode image handle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_CreatHandle(
                                            uint16              *full_path_ptr, //in:文件名,may PNULL
                                            uint32              *img_handle_ptr,//in/out:
                                            GUIANIM_IMG_DATA_T  *src_data_ptr   //in:传入的图片数据,may PNULL
                                            )
{
    IMG_DEC_RET_E       creat_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != img_handle_ptr); /*assert verified*/
    //SCI_ASSERT((PNULL != full_path_ptr) || (PNULL != src_data_ptr)); /*assert verified*/
    if ((PNULL == img_handle_ptr)||((PNULL == full_path_ptr)&&(PNULL == src_data_ptr)))
    {
        return GUIANIM_RESULT_WRONG_PARAM;
    }    

    //set decode src info
    if (PNULL == full_path_ptr)
    {
        dec_src.src_ptr       = src_data_ptr->src_buf_ptr;
        dec_src.src_file_size = src_data_ptr->src_data_size;
    }
    else
    {
        dec_src.file_name_ptr = full_path_ptr;
    }

    //creat image decode/encode handle
    creat_result = IMG_DEC_Create(&dec_src,img_handle_ptr);
    //SCI_TRACE_LOW:"GUIANIM_CreatHandle:creat handle=0x%x result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_113_112_2_18_3_15_1_32,(uint8*)"dd",*img_handle_ptr,creat_result);
    
    //get result
    result = ConvertResult(creat_result);

    return (result);
}

/*****************************************************************************/
//  Description : convert decode/encode result to mmi result
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E ConvertResult(
                                     IMG_DEC_RET_E  dec_result
                                     )
{
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;

    switch (dec_result)
    {
    case IMG_DEC_RET_SUCCESS:
        result = GUIANIM_RESULT_SUCC;
        break;

    case IMG_DEC_RET_UNKNOWN_FILE_TYPE:
    case IMG_DEC_RET_FILE_SIZE_NOTENOUGH:
        result = GUIANIM_RESULT_NO_SUPPORT;
        break;

    case IMG_DEC_RET_MEMORY_NOT_ENOUGH:
        result = GUIANIM_RESULT_LACK_MEMORY;
        break;

    case IMG_DEC_RET_BAD_DATA:
        result = GUIANIM_RESULT_BAD_DATA;
        break;
        
    case IMG_DEC_RET_BEYOND_MAX_RESOLUTION:
        result = GUIANIM_RESULT_BIG_RESOLUTION;
        break;

    case IMG_DEC_RET_BEYOND_MAX_FILESIZE:
        result = GUIANIM_RESULT_BIG;
        break;

    case IMG_DEC_RET_SFS_ERROR:
        result = GUIANIM_RESULT_SFS_FAIL;
        break;

    case IMG_DEC_RET_BUSYING:
        result = GUIANIM_RESULT_BUSY;
        break;

    case IMG_DEC_RET_INVALID_HANDLE:
    case IMG_DEC_RET_SCALING_ERROR:
        result = GUIANIM_RESULT_FAIL;
        break;

    default:
        result = GUIANIM_RESULT_FAIL;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get deoce/encode image frame
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_GetFrame(
                                         BOOLEAN                is_syn_decode,  //in:
                                         uint32                 img_handle,     //in:
                                         MMI_HANDLE_T           ctrl_handle,    //in:
                                         GUIANIM_TARGET_T       *target_ptr,    //in:
                                         GUIANIM_DEC_RESULT_T   *dec_result_ptr //in/out:
                                         )
{
    IMG_DEC_RET_E               get_result = IMG_DEC_RET_SUCCESS;
    GUIANIM_RESULT_E            result = GUIANIM_RESULT_FAIL;
    GUIANIM_DEC_CALLBACK_T      carry_param = {0};
    IMG_DEC_FRAME_IN_PARAM_T    dec_in = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   dec_out = {0};
    CTRLANIM_OBJ_T              *anim_ctrl_ptr = PNULL;

    if ((0 < img_handle) && 
        (PNULL != target_ptr) && 
        (PNULL != target_ptr->target_buf_ptr) && 
        (PNULL != dec_result_ptr))
    {
        //set decode handle
        dec_in.handle = img_handle;

        //set decode target
        dec_in.is_dec_thumbnail     = target_ptr->is_decode_thumb;
        dec_in.is_exist_background  = target_ptr->is_exist_bg;
        dec_in.target_ptr           = target_ptr->target_buf_ptr;
        dec_in.target_buf_size      = target_ptr->target_buf_size;
        dec_in.target_width         = target_ptr->target_width;
        dec_in.target_height        = target_ptr->target_height;
        dec_in.alpha_buf_ptr        = target_ptr->alpha_buf_ptr;
        dec_in.alpha_buf_size       = target_ptr->alpha_buf_size;
        dec_in.img_rect.left        = target_ptr->crop_rect.left;
        dec_in.img_rect.top         = target_ptr->crop_rect.top;
        dec_in.img_rect.right       = target_ptr->crop_rect.right;
        dec_in.img_rect.bottom      = target_ptr->crop_rect.bottom;
        dec_in.frame_index          = (uint8)target_ptr->ico_frame_index;
      
        dec_in.target_rect.left     = 0;
        dec_in.target_rect.right    = (uint16)(target_ptr->target_width -1);
        dec_in.target_rect.top      = 0;
        dec_in.target_rect.bottom   = (uint16)(target_ptr->target_height -1);
        
        switch (target_ptr->target_type)
        {
        case GUIIMG_DATE_TYPE_RGB565:
            dec_in.data_format = IMG_DEC_RGB565;
            break;

        case GUIIMG_DATE_TYPE_ARGB888:
            dec_in.data_format = IMG_DEC_ARGB888;
            break;

        case GUIIMG_DATE_TYPE_JPG:
            dec_in.data_format = IMG_DEC_JPEG;
            break;

        default:
            SCI_PASSERT(FALSE,("GUIANIM_GetFrame:handle=0x%x target_type %d is error!",img_handle,target_ptr->target_type)); /*assert verified*/
            break;
        }

        anim_ctrl_ptr = GUIANIM_GetPtr(ctrl_handle);
        if (PNULL != anim_ctrl_ptr)
        {
            if (GUIANIM_TYPE_IMAGE_ICO == anim_ctrl_ptr->type)
            {                
                dec_in.frame_index = (uint8)anim_ctrl_ptr->ico_frame_index;
            }
        }
        
        if (!is_syn_decode)
        {
            //set app carry param
            carry_param.img_handle  = img_handle;
            carry_param.ctrl_handle = ctrl_handle;
            carry_param.data_type   = target_ptr->target_type;

            dec_in.app_param_ptr  = (uint32)(&carry_param);
            dec_in.app_param_size = (uint32)(sizeof(GUIANIM_DEC_CALLBACK_T));
            dec_in.callback = AsynGetFrameCallbackFunc;
        }

        dec_in.quality = JINF_QUALITY_HIGH;

        //get image frame
        get_result = IMG_DEC_GetFrame(&dec_in,&dec_out);
        //SCI_TRACE_LOW:"GUIANIM_GetFrame:get frame result is %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_269_112_2_18_3_15_1_33,(uint8*)"d",get_result);
    
        //get result
        result = ConvertResult(get_result);

        //set return param
        dec_result_ptr->result = result;
        dec_result_ptr->data_type = target_ptr->target_type;
        if (is_syn_decode)
        {
            //set other
            dec_result_ptr->img_width         = (uint16)(dec_out.real_width);
            dec_result_ptr->img_height        = (uint16)(dec_out.real_height);
            dec_result_ptr->img_size          = dec_out.img_size;
            dec_result_ptr->delay_time        = dec_out.delay_time;
            dec_result_ptr->is_decode_gif_end = dec_out.is_decode_finished;
            dec_result_ptr->is_handle_alpha   = dec_out.is_process_alpha;
            dec_result_ptr->cur_frame_index   = (uint16)(dec_out.current_frame);
            dec_result_ptr->img_handle        = img_handle;
        }
        else
        {
            if (GUIANIM_RESULT_SUCC == result)
            {
                result = GUIANIM_RESULT_WAIT;
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : asynchronous deoce/encode image callback function
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void AsynGetFrameCallbackFunc(
                                    uint32                      img_handle,
                                    IMG_DEC_RET_E               dec_ret,
                                    IMG_DEC_FRAME_OUT_PARAM_T   *output_ptr,
                                    uint32                      param
                                    )
{
    GUIANIM_DEC_RESULT_T    dec_result = {0};
    GUIANIM_DEC_CALLBACK_T  *carry_param_ptr = (GUIANIM_DEC_CALLBACK_T *)param;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != output_ptr); /*assert verified*/
    if (PNULL == output_ptr)
    {
        return;
    }

    //convert result
    dec_result.result = ConvertResult(dec_ret);

    //set other
    dec_result.img_width         = (uint16)(output_ptr->real_width);
    dec_result.img_height        = (uint16)(output_ptr->real_height);
    dec_result.img_size          = output_ptr->img_size;
    dec_result.delay_time        = output_ptr->delay_time;
    dec_result.is_decode_gif_end = output_ptr->is_decode_finished;
    dec_result.is_handle_alpha   = output_ptr->is_process_alpha;
    dec_result.cur_frame_index   = (uint16)(output_ptr->current_frame);
    dec_result.img_handle        = carry_param_ptr->img_handle;
    dec_result.data_type         = carry_param_ptr->data_type;
    

    SCI_TRACE_LOW("GUIANIM:AsynGetFrameCallbackFunc:post get data cnf msg to MMI,img_handle=0x%x!",dec_result.img_handle);

    //post msg to anim control
    MMK_PostMsg(carry_param_ptr->ctrl_handle,MSG_CTL_ANIM_GET_DATA_CNF,&dec_result,sizeof(dec_result));
    
    MMITHEME_TriggerMMITask();

    GUI_INVALID_PARAM(img_handle);/*lint !e522*/
}

/*****************************************************************************/
//  Description : destroy deoce/encode image handle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_DestroyHandle(
                                              BOOLEAN       is_del_cnf, //in:is delete get data cnf msg
                                              uint32        img_handle, //in:
                                              MMI_HANDLE_T  handle      //in:
                                              )
{
    IMG_DEC_RET_E       destroy_result = IMG_DEC_RET_SUCCESS;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    GUIANIM_DEL_MSG_T   del_msg = {0};

    if (0 < img_handle)
    {
        if (is_del_cnf)
        {
            //set delete msg param
            del_msg.handle     = handle;
            del_msg.msg_id     = MSG_CTL_ANIM_GET_DATA_CNF;
            del_msg.img_handle = img_handle;

            //delete get cnf msg from queue
            MMK_DeleteMsgByCallback(&del_msg,DeleteGetDataCnf);
        }
    
        destroy_result = IMG_DEC_Destroy(img_handle);
        //SCI_TRACE_LOW:"GUIANIM_DestroyHandle:destroy handle=0x%x result is %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_375_112_2_18_3_15_2_35,(uint8*)"dd",img_handle,destroy_result);
    
        //get result
        result = ConvertResult(destroy_result);
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete get cnf msg from queue
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteGetDataCnf(
                               void             *in_param_ptr,  //in
                               MMI_MESSAGE_T    *msg_ptr        //in
                               )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_DEL_MSG_T       *del_msg_ptr = PNULL;
    GUIANIM_DEC_RESULT_T    *dec_result_ptr = PNULL;

    del_msg_ptr = (GUIANIM_DEL_MSG_T *)in_param_ptr;

    if ((PNULL != del_msg_ptr) &&
        (del_msg_ptr->handle == msg_ptr->handle) &&
        (del_msg_ptr->msg_id == msg_ptr->msg_id) &&
        (PNULL != msg_ptr->param_ptr))
    {
        dec_result_ptr = (GUIANIM_DEC_RESULT_T *)(msg_ptr->param_ptr);

        if (del_msg_ptr->img_handle == dec_result_ptr->img_handle)
        {
            SCI_TRACE_LOW("GUIANIM:DeleteGetDataCnf:handle=0x%x,img_handle=0x%x delete cnf msg!",del_msg_ptr->handle,del_msg_ptr->img_handle);
            result = TRUE;
        }
    }

    return (result);
}
/*****************************************************************************/
//  Description : create handle and get imginfo
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_CreateImgHandlAndGetInfo(                                                                   
                                                        wchar               *full_path_ptr, //in:文件名,may PNULL
                                                        GUIANIM_IMG_DATA_T  *src_data_ptr,  //in:传入的图片数据,may PNULL
                                                        GUIANIM_IMG_INFO_T  *img_info_ptr,  //in/out:
                                                        GUI_RECT_T          *rect_ptr,      //ico文件匹配rect;
                                                        uint32              *img_handle_ptr //int/out 
                                                        )
{
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    uint32              img_handle = 0;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;    

    if (PNULL == img_info_ptr)
    {
        return GUIANIM_RESULT_WRONG_PARAM;
    }    

    //set decode src info
    if (PNULL == full_path_ptr)
    {
        SCI_ASSERT(PNULL != src_data_ptr); /*assert verified*/
        dec_src.src_ptr       = src_data_ptr->src_buf_ptr;  /*lint !e413*/
        dec_src.src_file_size = src_data_ptr->src_data_size; /*lint !e413*/
    }
    else
    {
        dec_src.file_name_ptr = full_path_ptr;
    }
    
    dec_src.info_param_ptr = &dec_info;

    get_result = IMG_DEC_CreateEx(&dec_src,&img_handle);    
    SCI_TRACE_LOW("GUIANIM_CreateImgHandlAndGetInfo,result=%d,img_handle=%d",get_result,img_handle);    
    
    if (IMG_DEC_RET_SUCCESS == get_result)
    {       
        //set image info         
        img_info_ptr->is_support = TRUE;
        *img_handle_ptr = img_handle;
        switch (dec_info.img_type)
        {
        case IMG_DEC_TYPE_BMP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_BMP;
            img_info_ptr->img_width  = dec_info.img_detail_info.bmp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.bmp_info.img_height;
            break;

        case IMG_DEC_TYPE_WBMP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_WBMP;
            img_info_ptr->img_width = dec_info.img_detail_info.wbmp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.wbmp_info.img_height;
            break;

        case IMG_DEC_TYPE_JPEG:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_JPG;
            img_info_ptr->is_support = dec_info.img_detail_info.jpeg_info.is_dec_supported;
            img_info_ptr->img_width  = dec_info.img_detail_info.jpeg_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.jpeg_info.img_heigth;

            //set thumbnail info
            img_info_ptr->jpg_thumbnail.is_existed   = dec_info.img_detail_info.jpeg_info.thumbnail_info.is_thumb_existed;
            img_info_ptr->jpg_thumbnail.thumb_width  = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_width;
            img_info_ptr->jpg_thumbnail.thumb_height = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_height;
            img_info_ptr->jpg_thumbnail.thumb_offset = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_offset;
            img_info_ptr->jpg_thumbnail.thumb_size   = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_size;
            break;

        case IMG_DEC_TYPE_PNG:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_PNG;
            img_info_ptr->img_width  = dec_info.img_detail_info.png_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.png_info.img_height;
            break;

        case IMG_DEC_TYPE_GIF:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_GIF;
            img_info_ptr->img_width  = dec_info.img_detail_info.gif_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.gif_info.img_height;
            break;
         
        case IMG_DEC_TYPE_ICO:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_ICO;
            img_info_ptr->ico_frame_index = GetIcoFrameIndex(rect_ptr,&dec_info.img_detail_info.ico_info);
            img_info_ptr->img_width  = dec_info.img_detail_info.ico_info.img_width[img_info_ptr->ico_frame_index];
            img_info_ptr->img_height = dec_info.img_detail_info.ico_info.img_height[img_info_ptr->ico_frame_index];
            break;
#ifdef WEBP_DEC_SUPPORT_WEBP_SW
        case IMG_DEC_TYPE_WEBP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_WEBP;
            img_info_ptr->img_width  = dec_info.img_detail_info.webp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.webp_info.img_height;
            break;
#endif
        case IMG_DEC_TYPE_UNKNOWN:
            img_info_ptr->type = GUIANIM_TYPE_UNKNOWN;
            break;       
        
        default:
            SCI_PASSERT(FALSE,("GUIANIM_CreateImgHandlAndGetInfo:img_type %d is error!",dec_info.img_type)); /*assert verified*/
            break;
        }
        
        SCI_TRACE_LOW("GUIANIM_CreateImgHandlAndGetInfo,type=%d,img_width=%d,img_height=%d",
                    img_info_ptr->type,
                    img_info_ptr->img_width,
                    img_info_ptr->img_height
                    ); 
    }

    //get result
    result = ConvertResult(get_result);

    return (result);
}
/*****************************************************************************/
//  Description : get image info
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_GetInfo(
                                        GUI_RECT_T          *rect_ptr,      //in:ico文件需要匹配rect;                                     
                                        wchar               *full_path_ptr, //in:文件名,may PNULL
                                        GUIANIM_IMG_DATA_T  *src_data_ptr,  //in:传入的图片数据,may PNULL
                                        GUIANIM_IMG_INFO_T  *img_info_ptr   //in/out:
                                        )
{
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != img_info_ptr); /*assert verified*/
    if (PNULL == img_info_ptr)
    {
        return GUIANIM_RESULT_WRONG_PARAM;
    }    

    //set decode src info
    if (PNULL == full_path_ptr)
    {
        SCI_ASSERT(PNULL != src_data_ptr); /*assert verified*/
        dec_src.src_ptr       = src_data_ptr->src_buf_ptr;  /*lint !e413*/
        dec_src.src_file_size = src_data_ptr->src_data_size; /*lint !e413*/
    }
    else
    {
        dec_src.file_name_ptr = full_path_ptr;
    }

    //get image info
    get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
    SCI_TRACE_LOW("GUIANIM_GetInfo:IMG_DEC_GetInfo result is %d!",get_result);    
    if (IMG_DEC_RET_SUCCESS == get_result)
    {
        //set image info
        img_info_ptr->is_support = TRUE;
        switch (dec_info.img_type)
        {
        case IMG_DEC_TYPE_BMP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_BMP;
            img_info_ptr->img_width  = dec_info.img_detail_info.bmp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.bmp_info.img_height;
            break;

        case IMG_DEC_TYPE_WBMP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_WBMP;
            img_info_ptr->img_width = dec_info.img_detail_info.wbmp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.wbmp_info.img_height;
            break;

        case IMG_DEC_TYPE_JPEG:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_JPG;
            img_info_ptr->is_support = dec_info.img_detail_info.jpeg_info.is_dec_supported;
            img_info_ptr->img_width  = dec_info.img_detail_info.jpeg_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.jpeg_info.img_heigth;

            //set thumbnail info
            img_info_ptr->jpg_thumbnail.is_existed   = dec_info.img_detail_info.jpeg_info.thumbnail_info.is_thumb_existed;
            img_info_ptr->jpg_thumbnail.thumb_width  = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_width;
            img_info_ptr->jpg_thumbnail.thumb_height = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_height;
            img_info_ptr->jpg_thumbnail.thumb_offset = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_offset;
            img_info_ptr->jpg_thumbnail.thumb_size   = dec_info.img_detail_info.jpeg_info.thumbnail_info.thumb_size;
            break;

        case IMG_DEC_TYPE_PNG:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_PNG;
            img_info_ptr->img_width  = dec_info.img_detail_info.png_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.png_info.img_height;
            break;

        case IMG_DEC_TYPE_GIF:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_GIF;
            img_info_ptr->img_width  = dec_info.img_detail_info.gif_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.gif_info.img_height;
            break;
         
        case IMG_DEC_TYPE_ICO:                      
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_ICO;
            img_info_ptr->ico_frame_index = GetIcoFrameIndex(rect_ptr,&dec_info.img_detail_info.ico_info);
            img_info_ptr->img_width  = dec_info.img_detail_info.ico_info.img_width[img_info_ptr->ico_frame_index];
            img_info_ptr->img_height = dec_info.img_detail_info.ico_info.img_height[img_info_ptr->ico_frame_index];            
            break;
#ifdef WEBP_DEC_SUPPORT_WEBP_SW
        case IMG_DEC_TYPE_WEBP:
            img_info_ptr->type = GUIANIM_TYPE_IMAGE_WEBP;
            img_info_ptr->img_width  = dec_info.img_detail_info.webp_info.img_width;
            img_info_ptr->img_height = dec_info.img_detail_info.webp_info.img_height;
            break;
#endif
        case IMG_DEC_TYPE_UNKNOWN:
            img_info_ptr->type = GUIANIM_TYPE_UNKNOWN;
            break;       
        
        default:
            SCI_PASSERT(FALSE,("GUIANIM_GetInfo:img_type %d is error!",dec_info.img_type)); /*assert verified*/
            break;
        }
    }

    //get result
    result = ConvertResult(get_result);

    return (result);
}
/*****************************************************************************/
//  Description : get ico frame index
//  Global resource dependence : 
//  Author:kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetIcoFrameIndex(
                            GUI_RECT_T         *rect_ptr,       //in:ico文件需要匹配rect;                                    
                            IMG_DEC_ICO_INFO_T *img_info_ptr    //in:
                            )
{
    uint16  i,index     = 0;    
    if (PNULL != rect_ptr)
    {
        for (i=0; i<img_info_ptr->count; i++)
        {
            if ((img_info_ptr->img_width[i]*img_info_ptr->img_height[i])
                    <= ((rect_ptr->right + 1 - rect_ptr->left)*(rect_ptr->bottom + 1 - rect_ptr->top))
               )
            {
                if ((img_info_ptr->img_width[i]*img_info_ptr->img_height[i])
                    >=(img_info_ptr->img_width[index]*img_info_ptr->img_height[index]))
                {
                    index = i;
                }
            }
        }
    }     
     
    return index;
}
/*****************************************************************************/
//  Description : zoom in or out image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_ZoomImage(
                                          GUIANIM_ZOOM_INPUT_T  *zoom_in_ptr,   //in:
                                          GUIANIM_ZOOM_OUTPUT_T *zoom_out_ptr   //in/out:
                                          )
{
    uint32              scale_result = 0;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    SCALE_IMAGE_IN_T    scale_in = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != zoom_in_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != zoom_out_ptr); /*assert verified*/
    if ((PNULL == zoom_in_ptr)||(PNULL == zoom_out_ptr))
    {
        return GUIANIM_RESULT_WRONG_PARAM;
    }

    //set src width and height
    scale_in.src_size.w = zoom_in_ptr->src_img_width;
    scale_in.src_size.h = zoom_in_ptr->src_img_height;

    //set target width and height
    scale_in.target_size.w = zoom_in_ptr->dest_img_width;
    scale_in.target_size.h = zoom_in_ptr->dest_img_height;

    //w 4对齐，h 2对齐
    scale_in.target_size.w = (uint16)GUIANIM_ALIGN(scale_in.target_size.w,4);
    scale_in.target_size.h = (uint16)GUIANIM_ALIGN(scale_in.target_size.h,2);
    
    //set trim rect
    scale_in.src_trim_rect.x = zoom_in_ptr->clip_rect.left;
    scale_in.src_trim_rect.y = zoom_in_ptr->clip_rect.top;
    scale_in.src_trim_rect.w = (uint16)MIN((zoom_in_ptr->clip_rect.right - zoom_in_ptr->clip_rect.left + 1),(zoom_in_ptr->src_img_width - zoom_in_ptr->clip_rect.left));
    scale_in.src_trim_rect.h = (uint16)MIN((zoom_in_ptr->clip_rect.bottom - zoom_in_ptr->clip_rect.top + 1),(zoom_in_ptr->src_img_height - zoom_in_ptr->clip_rect.top));

    //check scaling up trim rect
	if ((scale_in.target_size.w > scale_in.src_trim_rect.w) && 
        (scale_in.src_trim_rect.w << 2) < scale_in.target_size.w)
	{
        scale_in.src_trim_rect.w = (uint16)(scale_in.target_size.w >> 2);
	}
    if ((scale_in.target_size.h > scale_in.src_trim_rect.h) && 
        (scale_in.src_trim_rect.h << 2) < scale_in.target_size.h)
	{
        scale_in.src_trim_rect.h = (uint16)(scale_in.target_size.h >> 2);
	}

    //set src format
    switch (zoom_in_ptr->src_data_type)
    {
    case GUIIMG_DATE_TYPE_RGB565:
        scale_in.src_format = IMGREF_FORMAT_RGB565;
        break;

    case GUIIMG_DATE_TYPE_ARGB888:
        scale_in.src_format = IMGREF_FORMAT_ARGB888;
        break;

    default:
        SCI_PASSERT(FALSE,("GUIANIM_ZoomImage:src_data_type %d is error!",zoom_in_ptr->src_data_type)); /*assert verified*/
        break;
    }

    //set src buffer
    scale_in.src_chn.chn0.ptr  = zoom_in_ptr->src_buf_ptr;
    scale_in.src_chn.chn0.size = zoom_in_ptr->src_buf_size;

    //set target format
    scale_in.target_format = scale_in.src_format;

    //set target buffer
    scale_in.target_buf.ptr  = zoom_in_ptr->dest_buf_ptr;
    scale_in.target_buf.size = zoom_in_ptr->dest_buf_size;

    //scale
    scale_result = GRAPH_ScaleImage(&scale_in,&scale_out);
    //SCI_TRACE_LOW:"GUIANIM_ZoomImage:zoom result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_638_112_2_18_3_15_2_38,(uint8*)"d",scale_result);
    switch (scale_result)
    {
    case SCI_SUCCESS:
        //set image width and height
        zoom_out_ptr->img_width  = scale_out.output_size.w;
        zoom_out_ptr->img_height = scale_out.output_size.h;

        //set target buffer offset
        zoom_out_ptr->dest_buf_offset = (uint32)(scale_out.output_chn.chn0.ptr) - (uint32)(scale_in.target_buf.ptr);

        //set target data type
        zoom_out_ptr->data_type = zoom_in_ptr->src_data_type;

        result = GUIANIM_RESULT_SUCC;
        break;
    
    case GRAPH_ERR_SIZE_UNSUPPORT:
        result = GUIANIM_RESULT_NO_SUPPORT;
        break;

    default:
        result = GUIANIM_RESULT_FAIL;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get zoom range
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_ZoomRange(
                                 uint16     *min_ptr,   //in/out:zoom out,1/min
                                 uint16     *max_ptr    //in/out:zoom in,max
                                 )
{
    BOOLEAN             result = FALSE;
    uint32              scale_result = 0;
    SCALE_IMAGE_CAP_T   img_cap = {0};

    //get scale range
    scale_result = GRAPH_GetScaleCapability(&img_cap);
    //SCI_TRACE_LOW:"GUIANIM_ZoomRange:zoom result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_682_112_2_18_3_15_2_39,(uint8*)"d",scale_result);
    if (SCI_SUCCESS == scale_result)
    {
        //set min and max
        if (PNULL != min_ptr)
        {
            *min_ptr = (uint16)(img_cap.scale_down_range.max);
        }
        if (PNULL != max_ptr)
        {
            *max_ptr = (uint16)(img_cap.scale_up_range.max);
        }

        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : rotate image angle 90,180,270 or mirror
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_RotateImage(
                                            GUIANIM_ROTATE_INPUT_T  *rotate_in_ptr, //in:
                                            GUIANIM_ROTATE_OUTPUT_T *rotate_out_ptr //in/out:
                                            )
{
    uint32              rotate_result = 0;
    GUIANIM_RESULT_E    result = GUIANIM_RESULT_FAIL;
    ROTATE_IMAGE_IN_T   rotate_in = {0};
    ROTATE_IMAGE_OUT_T  rotate_out = {0};

    //set src width and height
    rotate_in.src_size.w = rotate_in_ptr->src_img_width;
    rotate_in.src_size.h = rotate_in_ptr->src_img_height;

    //set src format
    switch (rotate_in_ptr->src_data_type)
    {
    case GUIIMG_DATE_TYPE_RGB565:
        rotate_in.src_format = IMGREF_FORMAT_RGB565;
        break;

    case GUIIMG_DATE_TYPE_ARGB888:
        rotate_in.src_format = IMGREF_FORMAT_ARGB888;
        break;

    default:
        SCI_PASSERT(FALSE,("GUIANIM_RotateImage:src_data_type %d is error!",rotate_in_ptr->src_data_type)); /*assert verified*/
        break;
    }

    //set src buffer
    rotate_in.src_chn.chn0.ptr  = rotate_in_ptr->src_buf_ptr;
    rotate_in.src_chn.chn0.size = rotate_in_ptr->src_buf_size;

    //set rotate mode
    rotate_in.rotate_mode = rotate_in_ptr->rotate_mode;

    //set target buffer
    rotate_in.target_buf.ptr  = rotate_in_ptr->dest_buf_ptr;
    rotate_in.target_buf.size = rotate_in_ptr->dest_buf_size;

    //rotate
    rotate_result = GRAPH_RotateImage(&rotate_in,&rotate_out);
    //SCI_TRACE_LOW:"GUIANIM_RotateImage:rotate result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_748_112_2_18_3_15_2_40,(uint8*)"d",rotate_result);
    switch (rotate_result)
    {
    case SCI_SUCCESS:
        //set image width and height
        rotate_out_ptr->img_width  = rotate_out.output_size.w;
        rotate_out_ptr->img_height = rotate_out.output_size.h;

        //set target buffer offset
        rotate_out_ptr->dest_buf_offset = (uint32)(rotate_out.output_chn.chn0.ptr) - (uint32)(rotate_in.target_buf.ptr);

        //set target data type
        rotate_out_ptr->data_type = rotate_in_ptr->src_data_type;
        
        result = GUIANIM_RESULT_SUCC;
        break;
    
    case GRAPH_ERR_SIZE_UNSUPPORT:
    case GRAPH_ERR_FORMAT_UNSUPPORT:
        result = GUIANIM_RESULT_NO_SUPPORT;
        break;

    default:
        result = GUIANIM_RESULT_FAIL;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_RotateAngle(
                                            GUIANIM_ROTATE_ANGLE_INPUT_T    *rotate_in_ptr, //in:
                                            GUIANIM_ROTATE_ANGLE_OUTPUT_T   *rotate_out_ptr //in/out:
                                            )
{
    uint32                  rotate_result = 0;
    GUIANIM_RESULT_E        result = GUIANIM_RESULT_FAIL;
    ROTATE_ARBITRARY_IN_T   rotate_in = {0};
    ROTATE_ARBITRARY_OUT_T  rotate_out = {0};

    //set src width and height
    rotate_in.src_size.w = rotate_in_ptr->src_img_width;
    rotate_in.src_size.h = rotate_in_ptr->src_img_height;

    //set src format
    switch (rotate_in_ptr->src_data_type)
    {
    case GUIIMG_DATE_TYPE_RGB565:
        rotate_in.src_format = IMGREF_FORMAT_RGB565;
        break;

    case GUIIMG_DATE_TYPE_ARGB888:
        rotate_in.src_format = IMGREF_FORMAT_ARGB888;
        break;

    default:
        SCI_PASSERT(FALSE,("GUIANIM_RotateAngle:src_data_type %d is error!",rotate_in_ptr->src_data_type)); /*assert verified*/
        break;
    }

    //set src buffer
    rotate_in.src_chn.chn0.ptr  = rotate_in_ptr->src_buf_ptr;
    rotate_in.src_chn.chn0.size = rotate_in_ptr->src_buf_size;

    //set src center point
    rotate_in.src_center.x = (int16)(rotate_in_ptr->src_img_width >> 1);
    rotate_in.src_center.y = (int16)(rotate_in_ptr->src_img_height >> 1);

    //set target width and height
    rotate_in.target_size.w = rotate_in_ptr->dest_img_width;
    rotate_in.target_size.h = rotate_in_ptr->dest_img_height;

    //set target format
    rotate_in.target_format = IMGREF_FORMAT_ARGB888;

    //set target buffer
    rotate_in.target_chn.chn0.ptr  = rotate_in_ptr->dest_buf_ptr;
    rotate_in.target_chn.chn0.size = rotate_in_ptr->dest_buf_size;

    //set target center point
    rotate_in.target_center.x = (int16)(rotate_in_ptr->dest_img_width >> 1);
    rotate_in.target_center.y = (int16)(rotate_in_ptr->dest_img_height >> 1);

    //set angle
    rotate_in.angle = rotate_in_ptr->angle;

    //set mode
    rotate_in.mode = ROTATE_ARBITRARY_NO_CROP;

    //rotate
    rotate_result = GRAPH_RotateArbitrary(&rotate_in,&rotate_out);
    //SCI_TRACE_LOW:"GUIANIM_RotateArbitrary:rotate result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_843_112_2_18_3_15_3_41,(uint8*)"d",rotate_result);
    switch (rotate_result)
    {
    case SCI_SUCCESS:
        //set image width and height
        rotate_out_ptr->img_width  = rotate_out.output_size.w;
        rotate_out_ptr->img_height = rotate_out.output_size.h;

        //set target data type
        rotate_out_ptr->data_type = GUIIMG_DATE_TYPE_ARGB888;

        result = GUIANIM_RESULT_SUCC;
        break;
    
    case GRAPH_ERR_SIZE_UNSUPPORT:
    case GRAPH_ERR_FORMAT_UNSUPPORT:
        result = GUIANIM_RESULT_NO_SUPPORT;
        break;

    default:
        result = GUIANIM_RESULT_FAIL;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get image width and height after rotate
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetRotateSize(
                                     uint16     *width_ptr,     //in/out:
                                     uint16     *height_ptr,    //in/out:
                                     uint16     rotate_angle    //in:
                                     )
{
    BOOLEAN         result = FALSE;
    uint32          rotate_result = 0;
    IMGREF_SIZE_T   src_size = {0};
    IMGREF_SIZE_T   dest_size = {0};

    if ((PNULL != width_ptr) &&
        (PNULL != height_ptr))
    {
        //set src size
        src_size.w = *width_ptr;
        src_size.h = *height_ptr;

        rotate_result = GRAPH_GetRotatedSize(&src_size,rotate_angle,&dest_size);
        //SCI_TRACE_LOW:"GUIANIM_GetRotateSize:rotate result is %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIANIM_REF_893_112_2_18_3_15_3_42,(uint8*)"d",rotate_result);
        switch (rotate_result)
        {
        case SCI_SUCCESS:
            //set image width and height
            *width_ptr  = dest_size.w;
            *height_ptr = dest_size.h;

            result = TRUE;
            break;
    
        default:
            break;
        }
    }

    return (result);
}




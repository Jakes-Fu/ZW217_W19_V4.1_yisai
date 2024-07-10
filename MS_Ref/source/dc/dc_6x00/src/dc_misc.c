/*******************************************************************************
 ** FileName:      dc_misc.c                                                *
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
#include "sci_api.h"
#include "dc_app_6x00.h"
#include "dc_framemgr.h"
#include "jpeg_interface.h"
#include "dc_bufmgr.h"
#include "block_mem_def.h"
#include "isp_drv.h"

#include "dc_misc.h"
#include "dc_param_ctrl.h"
#include "dc_snapshot.h"
#include "dc_review.h"
#include "display.h"
#include "dc_cfg.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------*
 **                         Local Function Prototype                           *
 **---------------------------------------------------------------------------*/
 
 /*--------------------------- Local Data ------------------------------------*/
 /*task-related resources*/
static uint32 s_dcam_task_id=SCI_INVALID_BLOCK_ID;
static DCAM_SIGNAL_T* s_dcam_sig_ptr=PNULL;
LOCAL SCI_TIMER_PTR s_dc_timer=PNULL;
LOCAL SCI_MUTEX_PTR	s_dc_mutex_ptr=PNULL;
LOCAL SCI_EVENT_GROUP_PTR s_dc_event=PNULL;


typedef struct jpeg_interp_size_t
{

	uint16 dst_width;
	uint16 dst_height;

	uint32 jpeg_size;

}JPEG_INTERP_SIZE_T;

typedef struct jpeg_interp_table_t
{

	uint16 src_width;
	uint16 src_height;

	uint32 size_addr;

}JPEG_INTERP_TABLE_T;




LOCAL const JPEG_INTERP_SIZE_T vga_inter_size[] =
{
	{1280, 960, 300*1024},
	{1600, 1200, 500*1024},
	{2048, 1536, 800*1024},
	{0xffff, 0xffff, 0xffff}
	
};

LOCAL const JPEG_INTERP_TABLE_T jpeg_size_tbl[] = 
{
	{640, 480, (uint32)vga_inter_size}
};

LOCAL const uint32 jpeg_size_tbl_size = (sizeof(jpeg_size_tbl)/sizeof(JPEG_INTERP_TABLE_T));

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
DECLARE_GLOBAL_CONTEXT;

/*--------------------------- External Function ------------------------------*/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);
extern PUBLIC uint32 DC_AtvHandle(uint32 atv_param);
//extern PUBLIC SENSOR_TYPE_E _Sensor_GetSensorType(void); 
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description: Create Mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void _DC_CreateMutex(void)
{	
	s_dc_mutex_ptr = SCI_CreateMutex("DC_SYNC_MUTEX", SCI_INHERIT);
	SCI_PASSERT((s_dc_mutex_ptr!=PNULL),("DC Great MUTEX fail!"));/*assert verified*/
	
}

/*****************************************************************************/
// Description: Get Mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_GetMutex(void)
{
    uint32 ret=SCI_SUCCESS;
    if(PNULL==s_dc_mutex_ptr)
    {
        _DC_CreateMutex();
    }

    ret = SCI_GetMutex(s_dc_mutex_ptr, SCI_WAIT_FOREVER);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}
/*****************************************************************************/
// Description: Put mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_PutMutex(void)
{
    uint32 ret=SCI_SUCCESS;    
    if(NULL==s_dc_mutex_ptr)
    {
        return;
    }
    
    ret=SCI_PutMutex(s_dc_mutex_ptr);	
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}

/*****************************************************************************/
// Description: delete mutex
// Global resource dependence:
// Author: Tim.zhu
// input:
//          onne
// output:
//          none
// return:
//          none
/*****************************************************************************/
PUBLIC void DC_DelMutex(void)
{
    uint32 ret=SCI_SUCCESS;

    if(NULL==s_dc_mutex_ptr)
    {
        return;
    }
    
    ret=SCI_DeleteMutex(s_dc_mutex_ptr);

    if(ret==SCI_SUCCESS)
    {
        s_dc_mutex_ptr=NULL;
    }
    
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}

/****************************************************************************************/
// Description: dc set event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 set_flags : event put flags
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_SetEvent(uint32 set_flags)
{
    if(PNULL!=s_dc_event)
    {   
        SCI_SetEvent(s_dc_event, set_flags, SCI_OR);
    }
}

/****************************************************************************************/
// Description: Isp get event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 requested_flags : event request flags
//                      uint32 wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
PUBLIC uint32 DC_GetEvent(uint32 requested_flags, uint32 wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if(NULL==s_dc_event)
    {
        s_dc_event=SCI_CreateEvent("DC_EVENT"); 
    }

    if(PNULL!=s_dc_event)
    {  
        status = SCI_GetEvent(s_dc_event, 
                                requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                0);
        status = SCI_GetEvent(s_dc_event, 
                                requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                wait_option);
    }

    return status;
}

/****************************************************************************************/
// Description: isp timer active
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      timer_expire: timer timeout expire
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _DC_CreateTimer(void)
{
    if(NULL==s_dc_timer)
    {
        s_dc_timer=SCI_CreateTimer("DC Timer", NULL, 0, DC_WAIT_TIME, SCI_NO_ACTIVATE);
    }
}

/****************************************************************************************/
// Description: isp timer active
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      timer_expire: timer timeout expire
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _DC_DeleteTimer(void)
{
    if(NULL!=s_dc_timer)
    {
        if(SCI_IsTimerActive(s_dc_timer))
        {
            SCI_DeactiveTimer(s_dc_timer);
        }

        SCI_PASSERT((SCI_SUCCESS==SCI_DeleteTimer(s_dc_timer)), ("DCAM: del timer fail error !"));/*assert verified*/

        s_dc_timer=NULL;
    }
}

/****************************************************************************************/
// Description: isp timer active
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      timer_expire: timer timeout expire
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_TimerActive(uint32 timer_expire, uint32(*fun)())
{
    if (NULL!= s_dc_timer)
    {
        SCI_ChangeTimer(s_dc_timer, fun, timer_expire);/*lint !e64*/
        SCI_ActiveTimer(s_dc_timer);
    }
}

/****************************************************************************************/
// Description: isp timer dactive
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void DC_TimerDactive(void)
{    
    if(NULL!=s_dc_timer)
    {
        if(SCI_IsTimerActive(s_dc_timer))
        {
            SCI_DeactiveTimer(s_dc_timer);
        }
    }
}

/******************************************************************************
// Description:	dc update cache
// Author: Tim.zhu
// Input:
// Return:
// Note:			
******************************************************************************/ 
PUBLIC uint32 DC_UpdateCache(uint32 buffer_addr, uint32 size, DC_OP_MODE_E op_mode)
{
    if(SCI_FALSE==ISP_ServiceIsSupportedUpdateCache())
    {
        return SCI_SUCCESS;
    }

    if((NULL!=buffer_addr)&&(0x00<size))
    {
        DMABUFFER_DIRECTION_E direction=DMABUFFER_BIDIRECTIONAL;

        switch(op_mode)
        {
            case DC_OP_READ:
                direction=DMABUFFER_TO_DEVICE;
                break;

            case DC_OP_WRITE:
                direction=DMABUFFER_FROM_DEVICE;
                break;

            case DC_OP_READ_WRITE:
                direction=DMABUFFER_BIDIRECTIONAL;
                break;

            default:
                break;			
        }

        MMU_DmaCacheSync(buffer_addr, size, direction);
    }

    return SCI_SUCCESS;

}

/******************************************************************************
// Description:	dc align
// Author:		Tim.zhu
// Input:			source length
// Return:		align length
// Note:			
******************************************************************************/ 
PUBLIC uint32 DC_Algin(uint32 pLen , uint16 algin_blk, uint16 algin_bits)
{
    uint32 algin_len=0x00;
    
    algin_len=pLen;
    algin_len=(((algin_len+algin_blk)>>algin_bits)<<algin_bits);

    return algin_len;
}

/******************************************************************************
// Description:     map dc rotaion mode to isp rotation mode
// Author:          jing.li
// Input:           src_mode: dc rotation mode
// Output:		    none
// Return:          isp rotation mode
// Note:            
******************************************************************************/
PUBLIC uint32 DC_Convert_Rotation_Mode_to_Isp(uint32 src_mode)
{
    uint32 dst_mode = 0;
    
	switch(src_mode)
	{
		case DCAMERA_ROTATION_0:
		{
			dst_mode = ISP_ROTATION_0;
		}
		break;

		case DCAMERA_ROTATION_90:
		{
			dst_mode = ISP_ROTATION_90;
		}
		break;

		case DCAMERA_ROTATION_180:
		{
			dst_mode = ISP_ROTATION_180;
		}
		break;

		case DCAMERA_ROTATION_270:
		{
			dst_mode = ISP_ROTATION_270;
		}
		break;

		default:
		    break;
	}

    return dst_mode;
}

/******************************************************************************
// Description:     adjust trim & display size according to distortion mode
// Author:          jing.li
// Input:           st_trim_rect_ptr: trim rect for sensor
//                  st_disp_rect_ptr: display rect on LCD
//                  disp_mode:
// Output:		    st_trim_rect_ptr(it may be changed according to the distortion mode)
//                  st_disp_rect_ptr(it may be changed according to the distortion mode)
// Return:          none
// Note:            
******************************************************************************/
PUBLIC void DC_AdjRect_OnDistortion(DCAMERA_RECT_T* src_rect_ptr,
                                            DCAMERA_RECT_T* disp_rect_ptr, 
                                            DCAMERA_RECT_T* target_rect_ptr, 
                                            DCAMERA_DISP_MODE_E disp_mode, 
                                            DCAMERA_ROTATION_MODE_E rot_mode)
{
    DCAMERA_RECT_T target_rect={0x00};
    uint16 swap_reg=0x00;

    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: disp_mode:%d, rot_mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_449_112_2_18_1_23_32_123,(uint8*)"dd", disp_mode,rot_mode);
    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: src rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_450_112_2_18_1_23_32_124,(uint8*)"dddd", src_rect_ptr->x,src_rect_ptr->y,src_rect_ptr->w,src_rect_ptr->h);
    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: disp rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_451_112_2_18_1_23_32_125,(uint8*)"dddd", disp_rect_ptr->x,disp_rect_ptr->y,disp_rect_ptr->w,disp_rect_ptr->h);
    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: target rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_452_112_2_18_1_23_32_126,(uint8*)"dddd", target_rect_ptr->x,target_rect_ptr->y,target_rect_ptr->w,target_rect_ptr->h);
    
    switch(rot_mode)
    {
        case DCAMERA_ROTATION_0:
        case DCAMERA_ROTATION_180:
        {
            target_rect.w=target_rect_ptr->w;
            target_rect.h=target_rect_ptr->h;
            break;
        }
        case DCAMERA_ROTATION_90:
        case DCAMERA_ROTATION_270:
        {
            target_rect.w=target_rect_ptr->h;
            target_rect.h=target_rect_ptr->w;

            target_rect_ptr->w=target_rect.w;
            target_rect_ptr->h=target_rect.h;           
            break;
        }
        default:
            break;
    }

    switch(disp_mode)
    {
        case DCAMERA_DISP_FULLVIEW:
        {
            if((src_rect_ptr->w*target_rect.h)>(src_rect_ptr->h*target_rect.w))
            { // base on display width
                target_rect_ptr->h=DC_LEFT_TRUNK2((src_rect_ptr->h*target_rect.w)/src_rect_ptr->w);

                switch(rot_mode)
                {// adjust target rect
                    case DCAMERA_ROTATION_0:
                    case DCAMERA_ROTATION_180:
                    {
                        target_rect_ptr->y+=(target_rect.h-target_rect_ptr->h)/2;
                        target_rect_ptr->y=DC_Algin(target_rect_ptr->y, 0x00, 0x02);
                        break;
                    }
                    case DCAMERA_ROTATION_90:
                    case DCAMERA_ROTATION_270:
                    {   
                        target_rect_ptr->x+=(target_rect.h-target_rect_ptr->h)/2;
                        target_rect_ptr->x=DC_Algin(target_rect_ptr->x, 0x00, 0x02);
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            { // base on display height
                target_rect_ptr->w=DC_Algin(((src_rect_ptr->w* target_rect.h)/src_rect_ptr->h), 0x00, 0x02);

                switch(rot_mode)
                {// adjust target rect
                    case DCAMERA_ROTATION_0:
                    case DCAMERA_ROTATION_180:
                    {
                        target_rect_ptr->x+=(target_rect.w-target_rect_ptr->w)/2;
                        target_rect_ptr->x=DC_Algin(target_rect_ptr->x, 0x00, 0x02);
                        break;
                    }
                    case DCAMERA_ROTATION_90:
                    case DCAMERA_ROTATION_270:
                    {   
                        target_rect_ptr->y+=(target_rect.w-target_rect_ptr->w)/2;
                        target_rect_ptr->y=DC_Algin(target_rect_ptr->y, 0x00, 0x02);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case DCAMERA_DISP_FULLSCREEN:
        {
            if((src_rect_ptr->w*target_rect.h)>(src_rect_ptr->h*target_rect.w))
            {// base on image height
                swap_reg = src_rect_ptr->w;
                src_rect_ptr->w=DC_RIGHT_TRUNK4((src_rect_ptr->h*target_rect.w)/target_rect.h);
                src_rect_ptr->x+=(swap_reg-src_rect_ptr->w)/2;
                src_rect_ptr->x=DC_RIGHT_TRUNK4(src_rect_ptr->x);
            }
            else if((src_rect_ptr->w*target_rect.h)<(src_rect_ptr->h*target_rect.w))
            {// base on image height
                swap_reg=src_rect_ptr->h;
                src_rect_ptr->h=DC_RIGHT_TRUNK4((src_rect_ptr->w*target_rect.h)/target_rect.w);
                src_rect_ptr->y+=(swap_reg-src_rect_ptr->h)/2;
            }
            break;
        }
        case DCAMERA_DISP_CUSTOMIZE:
        {
            break;
        }
        default:
            break;
    }

    if(target_rect_ptr->x<disp_rect_ptr->x)
    {
        target_rect_ptr->x=disp_rect_ptr->x;
    }

    if(target_rect_ptr->y<disp_rect_ptr->y)
    {
        target_rect_ptr->y=disp_rect_ptr->y;
    }

    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: modify src rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_566_112_2_18_1_23_32_127,(uint8*)"dddd", src_rect_ptr->x,src_rect_ptr->y,src_rect_ptr->w,src_rect_ptr->h);
    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: modify disp rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_567_112_2_18_1_23_32_128,(uint8*)"dddd", disp_rect_ptr->x,disp_rect_ptr->y,disp_rect_ptr->w,disp_rect_ptr->h);
    //SCI_TRACE_LOW:"DCAM:DC_AdjRect_OnDistortion: modify target rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_568_112_2_18_1_23_32_129,(uint8*)"dddd", target_rect_ptr->x,target_rect_ptr->y,target_rect_ptr->w,target_rect_ptr->h);

	return;
}

/******************************************************************************
// Description:     match the lcd point to image point
// Author:          Tim.zhu
// Input:           lcd_point_ptr: the point of lcd touch
// Output:		    target_point_ptr: the point of preview size 
// Return:          none
// Note:            
******************************************************************************/
PUBLIC uint32 DC_LcdPointMatchToImg(DCAMERA_RECT_T_PTR lcd_point_ptr, DCAMERA_RECT_T_PTR target_point_ptr)
{
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    DCAMERA_RECT_T target_rect={0x00};
    DCAMERA_RECT_T statistic_point={0x00};
    uint16 swap_reg=0x00;

    //SCI_TRACE_LOW:"DCAM: DC_LcdPointMatchToImg: lcd rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_587_112_2_18_1_23_32_130,(uint8*)"dddd", lcd_point_ptr->x,lcd_point_ptr->y,lcd_point_ptr->w,lcd_point_ptr->h);

    target_rect.x=pContext->preview_param.target_rect.x;
    target_rect.y=pContext->preview_param.target_rect.y;

    switch(pContext->preview_param.rotation_mode)
    {
        case DCAMERA_ROTATION_0:
        case DCAMERA_ROTATION_180:
        {
            target_rect.w=pContext->preview_param.target_rect.w;
            target_rect.h=pContext->preview_param.target_rect.h;
            break;
        }
        case DCAMERA_ROTATION_90:
        case DCAMERA_ROTATION_270:
        {
            target_rect.w=pContext->preview_param.target_rect.h;
            target_rect.h=pContext->preview_param.target_rect.w;        
            break;
        }
        default:
            break;
    }

    if((lcd_point_ptr->x<=target_rect.x)
        ||(lcd_point_ptr->y<=target_rect.y)
        ||(lcd_point_ptr->x>=target_rect.x+target_rect.w)
        ||(lcd_point_ptr->y>=target_rect.y+target_rect.h))
    {
        return SCI_ERROR;
    }

    statistic_point.x=lcd_point_ptr->x-pContext->preview_param.target_rect.x;
    statistic_point.y=lcd_point_ptr->y-pContext->preview_param.target_rect.y;
    switch(pContext->preview_param.rotation_mode)
    {
        case DCAMERA_ROTATION_0:
        {
            break;
        }
        case DCAMERA_ROTATION_90:
        {
            swap_reg=statistic_point.x;
            statistic_point.x=statistic_point.y;
            statistic_point.y=pContext->preview_param.target_rect.h-swap_reg;
            break;
        }
        case DCAMERA_ROTATION_180:
        {
            statistic_point.x=pContext->preview_param.target_rect.w-statistic_point.x;
            statistic_point.y=pContext->preview_param.target_rect.h-statistic_point.y;
            break;
        }
        case DCAMERA_ROTATION_270:
        {
            swap_reg=statistic_point.x;
            statistic_point.x=pContext->preview_param.target_rect.w-statistic_point.h;
            statistic_point.y=statistic_point.x;
            break;
        }
        default:
            break;
    }

    target_point_ptr->x=pContext->trim_rect.w*statistic_point.x/target_rect.w;
    target_point_ptr->x+=pContext->trim_rect.x;

    target_point_ptr->y=pContext->trim_rect.h*statistic_point.y/target_rect.h;
    target_point_ptr->y+=pContext->trim_rect.y;

    //SCI_TRACE_LOW:"DCAM: DC_LcdPointMatchToImg: modify target rect x:%d, y:%d,w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_658_112_2_18_1_23_32_131,(uint8*)"dddd", target_point_ptr->x,target_point_ptr->y,target_point_ptr->w,target_point_ptr->h);

	return SCI_SUCCESS;
}

/******************************************************************************
// Description:     get image sensor signal info
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          void* : the pointer of sensor signal
// Note:            none
******************************************************************************/
PUBLIC void DC_GetImgSensorSignalInfo(SENSOR_MODE_E sensor_mode,ISP_SENSOR_SIGNAL_INFO_T_PTR isp_sensor_singnal_ptr)
{
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();

    isp_sensor_singnal_ptr->img_format=CONVERT_SENSOR_FORMAT(pContext->sensor_info_ptr->sensor_mode_info[sensor_mode].image_format);
    isp_sensor_singnal_ptr->img_pattern=pContext->sensor_info_ptr->image_pattern;
    isp_sensor_singnal_ptr->vsync_polarity=(uint32)pContext->sensor_info_ptr->vsync_polarity;
    isp_sensor_singnal_ptr->hsync_polarity=(uint32)pContext->sensor_info_ptr->hsync_polarity;
    isp_sensor_singnal_ptr->preview_skip_num=pContext->sensor_info_ptr->preview_skip_num;
    isp_sensor_singnal_ptr->capture_skip_num=pContext->sensor_info_ptr->capture_skip_num;
    isp_sensor_singnal_ptr->preview_deci_num=pContext->sensor_info_ptr->preview_deci_num;
    isp_sensor_singnal_ptr->video_preview_deci_num=pContext->sensor_info_ptr->video_preview_deci_num;
    isp_sensor_singnal_ptr->threshold_eb=pContext->sensor_info_ptr->threshold_eb;
    isp_sensor_singnal_ptr->threshold_mode=pContext->sensor_info_ptr->threshold_mode;
    isp_sensor_singnal_ptr->threshold_start=pContext->sensor_info_ptr->threshold_start;
    isp_sensor_singnal_ptr->threshold_end=pContext->sensor_info_ptr->threshold_end;

}

/******************************************************************************
// Description:     convert format
// Author:          jing.li
// Input:           sensor format: dcamera value
// Output:		    none
// Return:          isp format value
// Note:            
******************************************************************************/
PUBLIC uint32 DC_Convert_Sensor_Format_to_Isp(uint32 sensor_format)
{
    uint32 isp_format = 0;
    
	switch(sensor_format)
	{
		case SENSOR_IMAGE_FORMAT_YUV422:
		{
			isp_format	= ISP_DATA_YUV422;
		}
		break;

		case SENSOR_IMAGE_FORMAT_YUV420:
		{
			isp_format	= ISP_DATA_YUV420;
		}
		break;

		case SENSOR_IMAGE_FORMAT_RGB565:
		{
			isp_format	= ISP_DATA_RGB565;
		}
		break;

		case SENSOR_IMAGE_FORMAT_RGB666:
		{
			isp_format 	= ISP_DATA_RGB666;
		}
		break;

		case SENSOR_IMAGE_FORMAT_RGB888:
		{
			isp_format 	= ISP_DATA_RGB888;
		}
		break;

		case SENSOR_IMAGE_FORMAT_CCIR656:
		{
			isp_format 	= ISP_DATA_CCIR656;
		}
		break;

		case SENSOR_IMAGE_FORMAT_JPEG:
		{
			isp_format = ISP_DATA_JPEG;
		}
		break;

		case SENSOR_IMAGE_FORMAT_RAW:
		default:
		break;
	}

    return isp_format;
}

/******************************************************************************
// Description:     set lcdc buffer buffer
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          uint32 : low 16bit rgb565
// Note:            none
******************************************************************************/
PUBLIC uint32 DC_GetBackGroundColor(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint32 bg_color=0x00;

    bg_color=(pContext->bg_color.b>>0x03)&0x001f;
    bg_color|=(pContext->bg_color.g<<0x03)&0x07e0;
    bg_color|=(pContext->bg_color.b<<0x08)&0xf800;

    return bg_color;
}

/******************************************************************************
// Description:     checker the display rect
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          uint32 : low 16bit rgb565
// Note:            none
******************************************************************************/
PUBLIC uint32 DC_CheckDispRect(DCAMERA_RECT_T* disp_rect, DCAMERA_RECT_T* target_rect, DCAMERA_ROTATION_MODE_E rotation_mode)
{
    uint32 rtn=DCAMERA_OP_SUCCESS;

    if((disp_rect->x<target_rect->x)
        ||(disp_rect->y<target_rect->y)
        ||((disp_rect->x+disp_rect->w)<(target_rect->x+target_rect->w))
        ||((disp_rect->y+disp_rect->h)<(target_rect->y+target_rect->h)))
    {
        rtn=DCAMERA_OP_ERROR;
    }

    return rtn;
}

/******************************************************************************
// Description:     free the app buffer
// Author:          jing.li
// Input:           app - dc application
// Output:          none
// Return:          none
// Note:            none
******************************************************************************/
PUBLIC void DC_Free(uint32 app)
{
    /* dummy */
}


/******************************************************************************
// Description:     assign the DC buffer according to app input
// Author:          jing.li
// Input:           app - dc application
// Output:          none
// Return:          none
// Note:            none
******************************************************************************/
/*lint -save -e64 */	
PUBLIC uint32 DC_Malloc(uint32 app)
{
    uint32 Rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    pContext->yyuv_buf.y_addr=NULL;
    pContext->yyuv_buf.uv_addr=NULL;
    pContext->yyuv_buf_len=0x00;

    pContext->quick_view_buf.y_addr=NULL;
    pContext->quick_view_buf.uv_addr=NULL;
    pContext->quick_view_buf_len=0x00;

    pContext->thumbnail_buf_addr=0x00;
    pContext->thumbnail_buf_len=0x00;

    pContext->exif_buf_addr=0x00;
    pContext->exif_buf_len=0x00;

    pContext->jpeg_buf_ptr=NULL;
    pContext->jpeg_buf_len=0x00;

    pContext->isp_capture_ptr=NULL;
    pContext->isp_capture_len=0x00;

    pContext->disp_buf_ptr=NULL;
    pContext->disp_buf_len=0x00;

    pContext->rot_buf_ptr=NULL;
    pContext->rot_buf_len=0x00;

    pContext->jpg_yuv_buf_addr=NULL;
    pContext->jpg_yuv_buf_len=0x00;

    pContext->src_jpg_addr=0x00;
    pContext->src_jpg_len=0x00;

    pContext->minature_swap_buf_addr=0x00;
    pContext->minature_swap_buf_len=0x00;

    pContext->exif_swap_buf_addr=0x00;
    pContext->exif_swap_buf_len=0x00;

    pContext->jpeg_fw_ptr=NULL;
    pContext->jpeg_fw_len=0x00;

    pContext->cx_buf_ptr=NULL;
    pContext->cx_buf_len=0x00;

    pContext->frames_buf_ptr=NULL;
    pContext->frames_buf_len=0x00;

    pContext->interpolation_buf_ptr=NULL;
    pContext->interpolation_buf_len=0x00;

    pContext->dc_buf_addr=0x00;
    pContext->dc_buf_len=0x00;
    pContext->remain_buf_ptr=NULL;
    pContext->remain_buf_len=0x00;  

    DC_RETURN_VAL_IF_FAIL_WARNING(MM_Init(app), DCAMERA_OP_NO_ENOUGH_MEMORY, ("DCAM: Alloc the buffer err!"));

    switch(app)
    {
        case DC_STATE_PREVIEWING:
        {
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_PREVIEW_DISP);
            pContext->disp_buf_len=MM_GetSize(MM_DC_PREVIEW_DISP);

            pContext->rot_buf_ptr=MM_Malloc(MM_DC_PREVIEW_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DC_PREVIEW_ROT);

            break;
        }

        case DC_STATE_CAPTURING:
        {
            //malloc capture exif buffer
            pContext->exif_buf_addr=MM_Malloc(MM_DC_CAPTURE_EXIF);
            pContext->exif_buf_len=MM_GetSize(MM_DC_CAPTURE_EXIF);

            //malloc capture thumbnail buffer
            pContext->thumbnail_buf_addr=MM_Malloc(MM_DC_CAPTURE_THUMBNAIL);
            pContext->thumbnail_buf_len=MM_GetSize(MM_DC_CAPTURE_THUMBNAIL);

            //malloc jpeg data buffer            
            pContext->jpeg_buf_ptr=MM_Malloc(MM_DC_CAPTURE_JPEG);
            pContext->jpeg_buf_len=MM_GetSize(MM_DC_CAPTURE_JPEG);

            //malloc yuv encoded data buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DC_CAPTURE_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DC_CAPTURE_YUV);

            //malloc minature src jpg data buffer
            pContext->src_jpg_addr=MM_Malloc(MM_DC_CAPTURE_MINATURE_SRC);
            pContext->src_jpg_len=MM_GetSize(MM_DC_CAPTURE_MINATURE_SRC);

            //malloc minature temp data buffer
            pContext->minature_swap_buf_addr=MM_Malloc(MM_DC_CAPTURE_MINATURE_SWAP);
            pContext->minature_swap_buf_len=MM_GetSize(MM_DC_CAPTURE_MINATURE_SWAP);

            //malloc minature temp data buffer
            //share to minature swap buf
            pContext->jpg_yuv_buf_addr=MM_Malloc(MM_DC_CAPTURE_MINATURE_SWAP);
            pContext->jpg_yuv_buf_len=MM_GetSize(MM_DC_CAPTURE_MINATURE_SWAP);

            //malloc quick view buffer
            pContext->quick_view_buf.y_addr=MM_Malloc(MM_DC_CAPTURE_QUICK_VIEW);
            pContext->quick_view_buf_len=MM_GetSize(MM_DC_CAPTURE_QUICK_VIEW);

            //malloc interpolation swap buffer
            pContext->cx_buf_ptr=MM_Malloc(MM_DC_CAPTURE_SWAP);
            pContext->cx_buf_len=MM_GetSize(MM_DC_CAPTURE_SWAP); 

            //malloc jpeg firmware buffer
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DC_CAPTURE_JPEG_FW);
            pContext->jpeg_fw_len=MM_GetSize(MM_DC_CAPTURE_JPEG_FW);

            //malloc capture disp buffer
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_CAPTURE_DISP);
            pContext->disp_buf_len=MM_GetSize(MM_DC_CAPTURE_DISP);

            //malloc capture rot buffer
            pContext->rot_buf_ptr=MM_Malloc(MM_DC_CAPTURE_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DC_CAPTURE_ROT);

            //malloc capture exif swap buffer
            pContext->exif_swap_buf_addr=MM_Malloc(MM_DC_CAPTURE_EXIF_SWAP);
            pContext->exif_swap_buf_len=MM_GetSize(MM_DC_CAPTURE_EXIF_SWAP);

            //recorder the dc start buffer
            pContext->dc_buf_addr=pContext->thumbnail_buf_addr;
            break;
        }
        
        case DC_STATE_PANORAMA:
        {
            //malloc jpeg data buffer
            pContext->jpeg_buf_ptr=MM_Malloc(MM_DC_PANORAMA_JPEG);
            pContext->jpeg_buf_len=MM_GetSize(MM_DC_PANORAMA_JPEG);

            //malloc panorama stat buffer
            pContext->cx_buf_ptr=MM_Malloc(MM_DC_PANORAMA_STAT);
            pContext->cx_buf_len=MM_GetSize(MM_DC_PANORAMA_STAT);                        

            //malloc yuv 0 1 2 3 capture data buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DC_PANORAMA_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DC_PANORAMA_YUV);

            // malloc panorama preview buf
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_PANORAMA_PREVIEW);
            pContext->disp_buf_len=MM_GetSize(MM_DC_PANORAMA_PREVIEW);   

            //malloc quick view buffer
            pContext->quick_view_buf.y_addr=MM_Malloc(MM_DC_PANORAMA_QUICK_VIEW);
            pContext->quick_view_buf_len=MM_GetSize(MM_DC_PANORAMA_QUICK_VIEW);   

            //malloc jpeg firmware buffer(no interpolation for quick view display)
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DC_PANORAMA_JPEG_ENC);
            pContext->jpeg_fw_len=MM_GetSize(MM_DC_PANORAMA_JPEG_ENC);

            break;
        }

        case DC_STATE_FOURINONE:
        {
            //malloc jpeg data buffer
            pContext->jpeg_buf_ptr=MM_Malloc(MM_DC_FOURinONE_JPEG);
            pContext->jpeg_buf_len=MM_GetSize(MM_DC_FOURinONE_JPEG);             

            //malloc yuv 0 1 2 3 capture data buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DC_FOURinONE_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DC_FOURinONE_YUV);

            // malloc panorama preview buf
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_FOURinONE_PREVIEW);
            pContext->disp_buf_len=MM_GetSize(MM_DC_FOURinONE_PREVIEW);   

            //malloc quick view buffer
            pContext->quick_view_buf.y_addr=MM_Malloc(MM_DC_FOURinONE_QUICK_VIEW);
            pContext->quick_view_buf_len=MM_GetSize(MM_DC_FOURinONE_QUICK_VIEW);   

            //malloc interpolation swap buffer
            pContext->cx_buf_ptr=MM_Malloc(MM_DC_FOURinONE_SWAP);
            pContext->cx_buf_len=MM_GetSize(MM_DC_FOURinONE_SWAP);                  

            //malloc jpeg firmware buffer(no interpolation for quick view display)
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DC_FOURinONE_JPEG_ENC);
            pContext->jpeg_fw_len=MM_GetSize(MM_DC_FOURinONE_JPEG_ENC);
            break;
        }
        
        case DC_STATE_BURST_CAPTURING:
        {
            //malloc capture exif buffer
            pContext->exif_buf_addr=MM_Malloc(MM_DC_CONTINUESHOT_EXIF);
            pContext->exif_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_EXIF);

            //malloc continueshot jpeg data buffer
            pContext->jpeg_buf_ptr=MM_Malloc(MM_DC_CONTINUESHOT_JPEG);
            pContext->jpeg_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_JPEG);

            //malloc continueshot yuv encoded data buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DC_CONTINUESHOT_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_YUV); 

            //malloc jpeg quick view decoder yuv buffer
            pContext->minature_swap_buf_addr=MM_Malloc(MM_DC_CONTINUESHOT_YUV);
            pContext->minature_swap_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_YUV);

            //malloc interpolation swap buffer
            pContext->cx_buf_ptr=MM_Malloc(MM_DC_CONTINUESHOT_SWAP);
            pContext->cx_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_SWAP); 

            //malloc continueshot jpeg firmware buffer
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DC_CONTINUESHOT_JPEG_FW);
            pContext->jpeg_fw_len=MM_GetSize(MM_DC_CONTINUESHOT_JPEG_FW);

            //malloc continueshot quick view display buffer
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_CONTINUESHOT_DISP);
            pContext->disp_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_DISP);

            //malloc capture rot buffer
            pContext->rot_buf_ptr=MM_Malloc(MM_DC_CONTINUESHOT_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DC_CONTINUESHOT_ROT); 

            //malloc recorder src jpg data buffer
            pContext->src_jpg_addr=pContext->jpeg_buf_ptr;
            pContext->src_jpg_len=pContext->jpeg_buf_len;

            //malloc capture exif swap buffer
            pContext->exif_swap_buf_addr=pContext->jpeg_fw_ptr;
            pContext->exif_swap_buf_len=pContext->jpeg_fw_len;

            //malloc recorder the dc start buffer
            pContext->dc_buf_addr=pContext->exif_buf_addr;
            break;
        }
        
        case DC_STATE_REVIEWING: 
        {
            //malloc review rot data buffer
            pContext->rot_buf_ptr=MM_Malloc(MM_DC_REVIEW_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DC_REVIEW_ROT);

            //malloc review yuv buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DC_REVIEW_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DC_REVIEW_YUV);

            //malloc review jpg fw buffer
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DC_REVIEW_FW);
            pContext->jpeg_fw_len=MM_GetSize(MM_DC_REVIEW_FW);

            //malloc review display buffer
            pContext->disp_buf_ptr=MM_Malloc(MM_DC_REVIEW_DISP);
            pContext->disp_buf_len=MM_GetSize(MM_DC_REVIEW_DISP);
            break;
        }
        
        case DC_STATE_UPCC_REC:
        case DC_STATE_REC:
        {
            //malloc capture yuv data buffer(use the osd buf for mjpg capture yuv)
            pContext->isp_capture_ptr=MM_Malloc(MM_DV_SHOT_CAP);
            pContext->isp_capture_len=MM_GetSize(MM_DV_SHOT_CAP);

            //malloc display buffer
            pContext->disp_buf_ptr=MM_Malloc(MM_DV_SHOT_DISPLAY);
            pContext->disp_buf_len=MM_GetSize(MM_DV_SHOT_DISPLAY);

            //malloc yuv encoded data buffer
            pContext->yyuv_buf.y_addr=MM_Malloc(MM_DV_SHOT_YUV);
            pContext->yyuv_buf_len=MM_GetSize(MM_DV_SHOT_YUV);

            //malloc rotation data buffer
            pContext->rot_buf_ptr=MM_Malloc(MM_DV_SHOT_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DV_SHOT_ROT);

            //malloc mjpeg frame buffer
            pContext->frames_buf_ptr=MM_Malloc(MM_DV_SHOT_FRAME_POOL);
            pContext->frames_buf_len=MM_GetSize(MM_DV_SHOT_FRAME_POOL);

            //malloc jpeg firmware buffer
            pContext->jpeg_fw_ptr=MM_Malloc(MM_DV_SHOT_JPEG_ENC);
            pContext->jpeg_fw_len=MM_GetSize(MM_DV_SHOT_JPEG_ENC);

            //Init frame buffer manager
            DC_FrameMgr_Init(pContext->frames_buf_ptr, pContext->frames_buf_len);
            
            break;
        }

        case DC_STATE_VIDEO_REVIEW:
        {
            //malloc display buffer
            pContext->disp_buf_ptr=MM_Malloc(MM_DV_PLAY_DISP);
            pContext->disp_buf_len=MM_GetSize(MM_DV_PLAY_DISP);

            //malloc rotation data buffer
            pContext->rot_buf_ptr=MM_Malloc(MM_DV_PLAY_ROT);
            pContext->rot_buf_len=MM_GetSize(MM_DV_PLAY_ROT);

            break;
        }

        default:
        {
            break;
        }        
    }
    
    if(DC_STATE_PREVIEWING==app)
    {
        pContext->remain_buf_ptr=(uint8*)((uint32)pContext->disp_buf_ptr+pContext->disp_buf_len);
        pContext->remain_buf_len=pContext->total_buf_len-pContext->disp_buf_len;
    }
    else
    {
        pContext->remain_buf_ptr=NULL;
        pContext->remain_buf_len=0x00;
    }
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->dc_buf_addr 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1134_112_2_18_1_23_33_132,(uint8*)"d",pContext->dc_buf_addr);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->thumbnail_buf_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1135_112_2_18_1_23_33_133,(uint8*)"dd",pContext->thumbnail_buf_addr, pContext->thumbnail_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->exif_buf_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1136_112_2_18_1_23_33_134,(uint8*)"dd",pContext->exif_buf_addr, pContext->exif_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->jpeg_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1137_112_2_18_1_23_33_135,(uint8*)"dd",pContext->jpeg_buf_ptr, pContext->jpeg_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->yyuv_buf.y_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1138_112_2_18_1_23_33_136,(uint8*)"dd",pContext->yyuv_buf.y_addr, pContext->yyuv_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->quick_view_buf.y_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1139_112_2_18_1_23_33_137,(uint8*)"dd",pContext->quick_view_buf.y_addr, pContext->quick_view_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->cx_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1140_112_2_18_1_23_33_138,(uint8*)"dd",pContext->cx_buf_ptr, pContext->cx_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->jpeg_fw_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1141_112_2_18_1_23_33_139,(uint8*)"dd",pContext->jpeg_fw_ptr, pContext->jpeg_fw_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->disp_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1142_112_2_18_1_23_33_140,(uint8*)"dd",pContext->disp_buf_ptr, pContext->disp_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->rot_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1143_112_2_18_1_23_33_141,(uint8*)"dd",pContext->rot_buf_ptr, pContext->rot_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->exif_swap_buf_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1144_112_2_18_1_23_33_142,(uint8*)"dd",pContext->exif_swap_buf_addr, pContext->exif_swap_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->jpg_yuv_buf_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1145_112_2_18_1_23_33_143,(uint8*)"dd",pContext->jpg_yuv_buf_addr, pContext->jpg_yuv_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->src_jpg_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1146_112_2_18_1_23_33_144,(uint8*)"dd",pContext->src_jpg_addr, pContext->src_jpg_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->minature_swap_buf_addr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1147_112_2_18_1_23_33_145,(uint8*)"dd",pContext->minature_swap_buf_addr, pContext->minature_swap_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->total_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1148_112_2_18_1_23_33_146,(uint8*)"dd",pContext->total_buf_ptr, pContext->total_buf_len/1024);
    //DCAM_PRINT:"DCAM: DC_Malloc, pContext->remain_buf_ptr 0x%x, %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1149_112_2_18_1_23_33_147,(uint8*)"dd",pContext->remain_buf_ptr, pContext->remain_buf_len/1024);

    return Rtn;
}
/*lint -restore*/

/******************************************************************************
// Description:     
// Author:          jing.li
// Input:           none
// Output:          none
// Return:          0       successful
//                  others  failed
// Note:            none
******************************************************************************/
PUBLIC void DC_SendMsg(DCAM_MSG_E sig_code, uint32 param)
{
 	DCAM_SIGNAL_T*	sig_ptr 	= NULL;
	xSignalHeaderRec*	sig		=PNULL;

	//DCAM_PRINT:"dc_task,DC_SendMessage sig_code = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1167_112_2_18_1_23_33_148,(uint8*)"d",sig_code);

	if(SCI_INVALID_BLOCK_ID == s_dcam_task_id)
		return;

	sig_ptr = (DCAM_SIGNAL_T *)SCI_ALLOCA(sizeof(DCAM_SIGNAL_T));
	SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/

	sig				= &sig_ptr->sig;
	sig->SignalSize	= sizeof(DCAM_SIGNAL_T);
	sig->Sender		= SCI_IdentifyThread();
	sig->SignalCode	= (int)sig_code;
	sig_ptr->param	= param ;

	SCI_SEND_SIGNAL( (xSignalHeaderRec*)sig_ptr, s_dcam_task_id);    
}


LOCAL void DC_TaskEntry(uint32 argc, void *argv)
{
    DCAMERA_RETURN_VALUE_E eRet;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    uint32 sig_param;

    while(1)/*lint !e716*/
    {
        s_dcam_sig_ptr = (DCAM_SIGNAL_T *)SCI_GetSignal(SCI_IdentifyThread());

        if(NULL==s_dcam_sig_ptr)
        {
            //DCAM_PRINT:"DCAM: DC_TaskEntry sig is null error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1197_112_2_18_1_23_34_149,(uint8*)"");
            continue ;
        }

        if(DCAM_MSG_MAX<=s_dcam_sig_ptr->sig.SignalCode)
        {
            //DCAM_PRINT:"DCAM: DC_TaskEntry cmd % error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1203_112_2_18_1_23_34_150,(uint8*)"", s_dcam_sig_ptr->sig.SignalCode);
            SCI_FREE(s_dcam_sig_ptr);
            s_dcam_sig_ptr = PNULL;
            continue ;
        }

        sig_param = s_dcam_sig_ptr->param;
        eRet = DCAMERA_OP_SUCCESS;

    	if(pContext->is_open)
    	{
            //IVSP_TestPoint((0x80901000+s_dcam_sig_ptr->sig.SignalCode), 0);
            switch(s_dcam_sig_ptr->sig.SignalCode)
            {
                case DCAM_MSG_PREVIEW:
                {
                    //DCAM_PRINT:"dc_task, DC_TASK_MSG_PREVIEW"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1219_112_2_18_1_23_34_151,(uint8*)"");
                }
                break;

                case DCAM_MSG_SNAPSHOT:
                {	
                    SNAPSHOT_OPS_HANDLER_T*     snapshot_handlers = DC_Snapshot_GetOpsHandler();
                    
                    //DCAM_PRINT:"DCAM: dc_task, DC_TASK_MSG_SNAPSHOT"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1227_112_2_18_1_23_34_152,(uint8*)"");

                    eRet = snapshot_handlers->start();

                    if(PNULL!=pContext->snapshot_param.snapshot_callback)
                    {
                        DC_GetMutex();
                        (pContext->snapshot_param.snapshot_callback)(eRet, pContext->jpeg_buf_ptr, pContext->jpeg_size);
                        DC_PutMutex();
                    }
                }
                break;

                case DCAM_MSG_DISPLAY:
                {
                    REVIEW_OPS_HANDLER_T* review_handlers = DC_Review_GetOpsHandler();

                    //DCAM_PRINT:"dc_task, DC_TASK_MSG_DISPLAY"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1244_112_2_18_1_23_34_153,(uint8*)"");

                    eRet = review_handlers->start();

                    if(pContext->review_param.review_callback)
                    {
                        DC_GetMutex();
                        (*pContext->review_param.review_callback)(eRet, PNULL, 0);
                        DC_PutMutex();
                    }
                }
                break;

                case DC_PARAM_CTRL_CALLBACK_PROC:
                {
                    DC_ParamCtrlCallback(sig_param);
                }
                break;

                case DCAM_MSG_BURST_SNAPSHOT:
                {
                    SNAPSHOT_OPS_HANDLER_T*     snapshot_handlers = DC_Snapshot_GetOpsHandler();

                    //DCAM_PRINT:"DCAM: dc_task, DCAM_MSG_BURST_SNAPSHOT"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1267_112_2_18_1_23_34_154,(uint8*)"");

                    eRet = snapshot_handlers->start_burst();
                }
                break;

                case DCAM_MSG_DECODE:
                {
                    //DCAM_PRINT:"dc_task, DC_TASK_MSG_DECODE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1275_112_2_18_1_23_34_155,(uint8*)"");
                }
                break;

                case DCAM_MSG_MJPEG_CAPTURE:
                {
                    //DCAM_PRINT:"dc_task, DC_TASK_MSG_MJPEG_CAPTURE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1281_112_2_18_1_23_34_156,(uint8*)"");
                }
                break;

                case DCAM_MSG_DISPLAY_DONE:
                {
                    //DCAM_PRINT:"dc_task, DCAM_MSG_DISPLAY_DONE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1287_112_2_18_1_23_34_157,(uint8*)"");
                    pContext->status = DCAM_STATUS_DISPLAY_DONE;
                }
                break;

                case DCAM_MSG_OVERTIME:
                {
                    //DCAM_PRINT:"dc_task, DC_TASK_MSG_OVERTIME"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1294_112_2_18_1_23_34_158,(uint8*)"");
                }
                break;

                case DCAM_MSG_ZOOM_DONE:
                {
                    //DCAM_PRINT:"dc_task, DCAM_MSG_ZOOM_DONE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1300_112_2_18_1_23_34_159,(uint8*)"");
                    if(PNULL !=pContext->zoom_callback)
                       (*pContext->zoom_callback)(DCAMERA_OP_SUCCESS, PNULL, 0);
                }
                break;

                case DCAM_MSG_ATV:
                {
                    DC_AtvHandle(sig_param);
                    break;
                }

                default:
                {
                    //DCAM_PRINT:"dc_task, invalid sig code to dc_task_Entry,line %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1314_112_2_18_1_23_34_160,(uint8*)"d",__LINE__);
                }
                break;
            }
    	}
	
        if(s_dcam_sig_ptr != PNULL)
        {
            IVSP_TestPoint((0x08091000+s_dcam_sig_ptr->sig.SignalCode), 1);
            SCI_FREE(s_dcam_sig_ptr);
            s_dcam_sig_ptr = PNULL;
        }
    }
	
}


PUBLIC uint32 DC_TaskOpen(uint32 priority)
{
    int nRet = SCI_SUCCESS;

    if(SCI_INVALID_BLOCK_ID!=s_dcam_task_id)
    {
    	return SCI_ERROR;
    }

    s_dcam_task_id = SCI_CreateAppThread("T_DCAM",
                                    "Q_DCAM",
                                    DC_TaskEntry,
                                    0,
                                    0,
                                    DCAM_STACK_SIZE,
                                    DCAM_QUEUE_NUM,
                                    priority,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);

    if(SCI_INVALID_BLOCK_ID == s_dcam_task_id)
    {
    	return SCI_ERROR;
    }

    _DC_CreateTimer();

    //DCAM_PRINT:"DCAM_TaskOpen, task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1358_112_2_18_1_23_34_161,(uint8*)"d",s_dcam_task_id);

	return nRet;
}


PUBLIC uint32 DC_TaskClose(void)
{
    //DCAM_PRINT:"DCAM_TaskClose,task_id = %lu"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1366_112_2_18_1_23_34_162,(uint8*)"u",s_dcam_task_id);

    if(SCI_INVALID_BLOCK_ID==s_dcam_task_id)
    {
    	return SCI_ERROR;
    }

    _DC_DeleteTimer();

  
    DC_GetMutex();              //high pirioty mutex get first 
    ISP_ServiceGetMutex();          

    ImgSensor_GetMutex(); 

        
    SCI_SuspendThread(s_dcam_task_id);

    if(s_dcam_sig_ptr != PNULL)      // Free the processing signal
    {
        SCI_FREE(s_dcam_sig_ptr);
        s_dcam_sig_ptr = NULL;
    }
    // Empty the signal queue
    while (1)/*lint !e716*/
    {
        s_dcam_sig_ptr = (void*)SCI_PeekSignal(s_dcam_task_id);
        if (s_dcam_sig_ptr != PNULL)
        {
            SCI_FREE(s_dcam_sig_ptr);
	    s_dcam_sig_ptr = NULL;
    	}
    	else
    	{
    		break;
    	}
    }   

    SCI_TerminateThread(s_dcam_task_id);
    SCI_DeleteThread(s_dcam_task_id); 

    ImgSensor_PutMutex();

    ISP_ServicePutMutex();
    DC_PutMutex();
    //DC_DelMutex();   
   
   
    IMGJPEG_FreeRes();	                //optimize the mutex handle

    s_dcam_task_id = SCI_INVALID_BLOCK_ID;

    return SCI_SUCCESS;
}


/******************************************************************************/
// Description: 	ISP send msg to DCAM for finishing zoom
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		
/******************************************************************************/
PUBLIC void DC_ISPZoomDone(void)
{
    DC_SendMsg(DCAM_MSG_ZOOM_DONE, 0);
}

/******************************************************************************
// Description: get img sensor max output yuv width
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the width of yuv
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorYuvWidth(void)
{
    DCAM_CONTEXT_T* pDcContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint8 i=0x00;
    
    for(i=SENSOR_MODE_PREVIEW_ONE; SENSOR_MODE_MAX>i; i++)
    {
        if(SENSOR_IMAGE_FORMAT_YUV422==pDcContext->sensor_info_ptr->sensor_mode_info[i].image_format)
        {
            if(sensor_width<pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width)
            {
                sensor_width=pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width;
            }
        }
    }

    return sensor_width;
}

/******************************************************************************
// Description: get img sensor max output yuv height
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of yuv
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorYuvHeight(void)
{
    DCAM_CONTEXT_T* pDcContext=_GetDCAMContext();
    uint16 sensor_height=0x00;
    uint8 i=0x00;
    
    for(i=SENSOR_MODE_PREVIEW_ONE; SENSOR_MODE_MAX>i; i++)
    {
        if(SENSOR_IMAGE_FORMAT_YUV422==pDcContext->sensor_info_ptr->sensor_mode_info[i].image_format)
        {
            if(sensor_height<pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_height)
            {
                sensor_height=pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_height;
            }
        }
    }

    return sensor_height;
}

/******************************************************************************
// Description: get img sensor max output jpg width
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the width of jpg
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorJpgWidth(void)
{
    DCAM_CONTEXT_T* pDcContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint8 i=0x00;
    
    for(i=SENSOR_MODE_PREVIEW_ONE; SENSOR_MODE_MAX>i; i++)
    {
        if(SENSOR_IMAGE_FORMAT_JPEG==pDcContext->sensor_info_ptr->sensor_mode_info[i].image_format)
        {
            if(sensor_width<pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width)
            {
                sensor_width=pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width;
            }
        }
    }

    return sensor_width;
}

/******************************************************************************
// Description: get img sensor max output jpg height
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorJpgHeight(void)
{
    DCAM_CONTEXT_T* pDcContext=_GetDCAMContext();
    uint16 sensor_height=0x00;
    uint8 i=0x00;
    
    for(i=SENSOR_MODE_PREVIEW_ONE; SENSOR_MODE_MAX>i; i++)
    {
        if(SENSOR_IMAGE_FORMAT_JPEG==pDcContext->sensor_info_ptr->sensor_mode_info[i].image_format)
        {
            if(sensor_height<pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_height)
            {
                sensor_height=pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_height;
            }
        }
    }

    return sensor_height;
}

/******************************************************************************
// Description: get img sensor max size(width or height)
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint16 DC_GetMaxImgSensorSize(void)
{
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 sensor_jpg_width=0x00;
    uint16 sensor_jpg_height=0x00;

    sensor_width=DC_GetMaxImgSensorYuvWidth();
    sensor_height=DC_GetMaxImgSensorYuvHeight();
    sensor_jpg_width=DC_GetMaxImgSensorJpgWidth();
    sensor_jpg_height=DC_GetMaxImgSensorJpgHeight();

    if(sensor_width<sensor_jpg_width)
    {
        return sensor_jpg_width;
    }

    return sensor_width;
}

/******************************************************************************
// Description: the first and end line process
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  SCI_SUCCESS:success
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_ImgTerminalLineCopy(DCAMERA_DATA_ADDR_T_PTR data_addr, DCAMERA_SIZE_T_PTR size, DCAM_DATA_TYPE_E data_format)
{
    DCAMERA_DATA_ADDR_T src_addr={0x00};
    DCAMERA_DATA_ADDR_T dst_addr={0x00};

    switch(data_format)
    {
        case DCAM_DATA_TYPE_RGB565:
        {
            src_addr.y_addr=data_addr->y_addr+(size->w<<0x01);
            dst_addr.y_addr=data_addr->y_addr;
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,(size->w<<0x01));
            src_addr.y_addr=data_addr->y_addr+(size->w<<0x01)*(size->h-0x02);
            dst_addr.y_addr=data_addr->y_addr+(size->w<<0x01)*(size->h-0x01);
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,(size->w<<0x01));
            break;
        }
        case DCAM_DATA_TYPE_YUV422:
        {
            src_addr.y_addr=data_addr->y_addr+size->w;
            dst_addr.y_addr=data_addr->y_addr;
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,size->w);
            src_addr.uv_addr=data_addr->uv_addr+size->w;
            dst_addr.uv_addr=data_addr->uv_addr;
            SCI_MEMCPY((void*)dst_addr.uv_addr,(void*)src_addr.uv_addr,size->w);

            src_addr.y_addr=data_addr->y_addr+size->w*(size->h-0x02);
            dst_addr.y_addr=data_addr->y_addr+size->w*(size->h-0x01);
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,size->w);
            src_addr.uv_addr=data_addr->uv_addr+size->w*(size->h-0x02);
            dst_addr.uv_addr=data_addr->uv_addr+size->w*(size->h-0x01);
            SCI_MEMCPY((void*)dst_addr.uv_addr,(void*)src_addr.uv_addr,size->w);
            break;
        }
        default:
            break;

    }

    return SCI_SUCCESS;

}

/******************************************************************************
// Description: get img sensor max size(width or height)
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetZoomTrimSize(uint32* infor_ptr, DCAMERA_SIZE_T_PTR trim_size_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    LCD_INFO_T lcd_info={0x00};
    uint32 lcd_id=((*infor_ptr)>>0x18)&0xff;    
    uint16 preview_width=0x00;
    uint16 preview_height=0x00;
    uint16 capture_width=0x00;
    uint16 capture_height=0x00;

    LCD_GetInfo(lcd_id, &lcd_info);/*lint !e64*/

    preview_width=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_width;
    preview_height=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_height;

    capture_width=pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
    capture_height=pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;

    if(lcd_info.lcd_width*preview_height<preview_width*lcd_info.lcd_height)
    {
        trim_size_ptr->h=(lcd_info.lcd_height/ISP_GetMaxPreviewScaleUpCoeff())*capture_height/preview_height;
        trim_size_ptr->w=trim_size_ptr->h*preview_width/preview_height;
    }
    else
    {
        trim_size_ptr->w=(lcd_info.lcd_width/ISP_GetMaxPreviewScaleUpCoeff())*capture_width/preview_width;
        trim_size_ptr->h=trim_size_ptr->w*preview_height/preview_width;
    }

    trim_size_ptr->w=DC_Algin(trim_size_ptr->w,0x03,0x02);
    trim_size_ptr->h=DC_Algin(trim_size_ptr->h,0x02,0x01);
    
    return SCI_SUCCESS;
}

/******************************************************************************
// Description: get img sensor max size(width or height)
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureMatchSize(DCAMERA_SIZE_T_PTR photo_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint32 sensor_output_type=SENSOR_MODE_MAX;

    for(sensor_output_type=SENSOR_MODE_PREVIEW_ONE; sensor_output_type<SENSOR_MODE_MAX; sensor_output_type++)
    {
        if((photo_ptr->w<=pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].width)
            &&(photo_ptr->h<=pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].height))
        {
            break;
        }
    }
 

    //SCI_TRACE_LOW:"DCAM: DC_GetCaptureMatchSize :%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1682_112_2_18_1_23_34_163,(uint8*)"d",sensor_output_type);

    return sensor_output_type;
}

/******************************************************************************
// Description: get img sensor match size(width or height)
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the height of jpg
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetMaxSensorSize(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    int32 size_type=SENSOR_MODE_MAX;

    for(size_type=(DCAMERA_PHOTO_SIZE_MAX-0x01); size_type>=DCAMERA_PHOTO_SIZE_176X220; size_type--)
    {
        if((DC_GetSensorSizeStPtr(size_type)->w<=pContext->sensor_info_ptr->source_width_max)
            &&(DC_GetSensorSizeStPtr(size_type)->h<=pContext->sensor_info_ptr->source_height_max))
        {
            break;
        }
    } 

    return size_type;
}

/******************************************************************************
// Description: set dc snapshot img sensor mode
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  None
// Note:    none
******************************************************************************/
PUBLIC void DC_SetCaptureImgSensorMode(void)
{
    DCAM_CONTEXT_T* pDcContext=_GetDCAMContext();
    uint8 i=0x00;

    
    
    for(i=SENSOR_MODE_PREVIEW_ONE; SENSOR_MODE_MAX>i; i++)
    {
        if(0x00!=pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width)
        {
            if(pDcContext->sensor_info_ptr->sensor_mode_info[i-0x01].trim_width<pDcContext->sensor_info_ptr->sensor_mode_info[i].trim_width)
            {
                pDcContext->snapshot_mode=i;/*lint !e64*/
            }
            
            if(pDcContext->target_width<=pDcContext->sensor_info_ptr->sensor_mode_info[i].width)
            {
                break ;
            }                
        }
    }
    //SCI_TRACE_LOW:"--DC_SetCaptureImgSensorMode:target_w =%d,pDcContext->snapshot_mode=%d,%d--"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_1740_112_2_18_1_23_35_164,(uint8*)"ddd",pDcContext->target_width,pDcContext->snapshot_mode,pDcContext->sensor_info_ptr->sensor_mode_info[2].trim_width);
    return ;
}

/******************************************************************************
// Description: get max hardware interpolation type
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      max hardware interpolation type
******************************************************************************/ 
LOCAL uint16 _Get_MaxHardWareInterpolationType(void)
{
    uint16 sensor_width=0x00;
    uint16 max_photo_width=0x00;
    uint16 photo_width=0x00;
    uint16 hardware_type=0x00;

    if(DC_GetMaxImgSensorYuvWidth()<DC_GetMaxImgSensorJpgWidth())
    {
        sensor_width=DC_GetMaxImgSensorJpgWidth();
    }
    else
    {
        sensor_width=DC_GetMaxImgSensorYuvWidth();
    }

    if(ISP_ServiceGetMaxSliceScaleWidth()<=sensor_width)
    {
        max_photo_width=sensor_width;
    }
    else
    {
        max_photo_width=sensor_width*ISP_ServiceGetMaxScaleUpCoeff();
    }
    
    for(hardware_type=DCAMERA_PHOTO_SIZE_MAX-1; hardware_type>DCAMERA_PHOTO_SIZE_176X220; hardware_type--)
    {
        photo_width=DC_GetSensorSizeStPtr(hardware_type)->w;

        if(photo_width<=max_photo_width)
        {
            break;
        } 
    }

    return hardware_type;

}

/******************************************************************************
// Description: get max software interpolation type
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      max software interpolation type
******************************************************************************/ 
LOCAL uint16 _Get_MaxSoftWareInterpolationType(void)
{
    uint16 sensor_width=0x00;
    uint16 software_type=0x00;

    sensor_width=DC_GetMaxImgSensorYuvWidth();

    if(VGA_WIDTH==sensor_width)
    {
        software_type=DCAMERA_PHOTO_SIZE_1600X1200;
    }

    return software_type;    
}

/******************************************************************************
// Description: get interpolation type
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      0x01: HW
//              0x02: SW
******************************************************************************/ 
PUBLIC uint32 DC_GetInterpolationType(void)
{
    uint16 hardware_type=0x00;
    uint16 software_type=0x00;
    uint32 interpolation_type=0x00;

    hardware_type=_Get_MaxHardWareInterpolationType();
    software_type=_Get_MaxSoftWareInterpolationType();  

    if(software_type<=hardware_type)
    {
        interpolation_type=DC_INTERPOLATION_TYPE_HW;
    }
    else
    {
        interpolation_type=DC_INTERPOLATION_TYPE_SW;
    }
 
    return interpolation_type;

}

/******************************************************************************
// Description: get interpolation type
// Author:      Tim.zhu
// Input:       src_height : soruce height
//              dst_height : dst height
//              interpolation_type : interpolation type
// Output:      none
// Return:      uint16: slice scale line
******************************************************************************/ 
PUBLIC uint16 DC_GetInterpolationSliceLine(uint16 src_height, uint16 dst_height, uint32 interpolation_type)
{
    uint16 slice_lines=0x00;
    uint16 max_gcd=0x00;


    if(DC_INTERPOLATION_TYPE_HW==interpolation_type)
    {
        slice_lines=HW_INTERPOLATION_SLICE_LINE;
    }
    else
    {
        max_gcd=DC_GetGCD(dst_height, src_height);

        slice_lines=(dst_height/max_gcd)<<0x03;
    }
 
    return slice_lines;

}

/******************************************************************************
// Description: get capture jpg encoder buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture jpg buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureJpgBufSize(uint16 photo_width, uint16 photo_height)
{
    uint32 buf_size=0x00;
	//uint32 jpeg_size_table_size = 
	
    buf_size=DC_Algin(((photo_width*photo_height*0x02)/6),0xff,0x08);

    return buf_size;
}


PUBLIC uint32 DC_GetCaptureJpgBufSize_Ext(uint16 src_width, uint16 src_height, uint16 dst_width, uint16 dst_height)
{
	uint32 i, k;
    uint32 buf_size=0x00;
	uint32 jpeg_size_table_size = jpeg_size_tbl_size;

	for(i=0; i<jpeg_size_table_size; i++)
	{
		if(src_width == jpeg_size_tbl[i].src_width && src_height == jpeg_size_tbl[i].src_height)
		{
			JPEG_INTERP_SIZE_T *table_ptr = (JPEG_INTERP_SIZE_T *)(jpeg_size_tbl[i].size_addr);

			if(PNULL != table_ptr)
			{
				k = 0;
				while(table_ptr[k].dst_width != 0xffff || table_ptr[k].dst_height != 0xffff)
				{
					if(table_ptr[k].dst_width == dst_width || table_ptr[k].dst_height == dst_height)
					{
						buf_size = table_ptr[k].jpeg_size;
						break;
					}
					k++;
									
				}
				
			}
					
		}
		
	}
	if(0 == buf_size)
	{
    	buf_size=DC_Algin(((dst_width*dst_height*0x02)/6),0xff,0x08);
	}

    return buf_size;
}


/******************************************************************************
// Description: get capture yuv buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureYuvBufSize(uint16 sensor_width, uint16 sensor_height, uint16 photo_width, uint16 photo_height)
{
    uint32 buf_size=0x00;
    uint16 min_width=0x00;
    uint16 min_height=0x00;


	if(photo_width <= ISP_Service_GetDirectMaxScaleWidth()) 
	{
		min_width=photo_width;
		min_height=photo_height;
	}
	else
	{
		min_height=sensor_height;
		min_width=sensor_width;
	}
#if 0
    if(sensor_width<photo_width)
    {
        min_width=sensor_width;
    }
    else
    {
        min_width=photo_width;
    }

    if(sensor_height<photo_height)
    {
        min_height=sensor_height;
    }
    else
    {
        min_height=photo_height;
    }

#endif

    buf_size=DC_Algin((min_width*min_height), 0xff, 0x08)<<0x01;

    return buf_size;
}

/******************************************************************************
// Description: get capture jpg encoder fw buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
// Output:  None 
// Return:  uint32:the length of capture encoder fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureJpgFwBufSize(uint16 sensor_width, uint16 photo_width)
{
    uint32 buf_size=0x00;
    uint16 slice_lines=0x00;
    uint32 interpolation_type=0x00;
	DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    if(sensor_width<=photo_width ||
	  0 != pContext->zoom_level)
    {
        interpolation_type=DC_GetInterpolationType();
        slice_lines=DC_GetInterpolationSliceLine(sensor_width, photo_width, interpolation_type);
        // slice scale pingpong buffer
        buf_size=(DC_Algin((photo_width*slice_lines), 0xff, 0x08)*0x06);
        // slice scale out buffer
        buf_size+=(DC_Algin((photo_width*(slice_lines+0x04)), 0xff, 0x08)*0x02);
        // slice stream buffer
        buf_size+=(DC_Algin((16*1024), 0xff, 0x08)<<0x01);
    }
    else
    {
        buf_size=80*1024;
    }

    DC_Algin(buf_size, 0xff, 0x08);

    return buf_size;
}

/******************************************************************************
// Description: get capture swap buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            photo_width: photo width
// Output:  None 
// Return:  uint32:the length of capture swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureSwapBufSize(uint16 sensor_width, uint16 photo_width)
{
    uint16 slice_lines=0x00;
    uint32 buf_size=0x00;
    uint32 interpolation_type=0x00;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    SENSOR_MODE_INFO_T sensor_mode_info = {0};

    sensor_mode_info = pContext->sensor_info_ptr->sensor_mode_info[pContext->snapshot_mode];

    #if (defined(PLATFORM_SC6600L))
    buf_size=(photo_width<<0x04);
    #elif (defined(PLATFORM_SC6800H))
    //if(sensor_width<=photo_width || (sensor_width>photo_width)
	if((sensor_width !=photo_width && ISP_Service_GetDirectMaxScaleWidth() < photo_width  ) \
       ||(sensor_mode_info.trim_width!=sensor_mode_info.width) \
       || (0 !=pContext->zoom_level))
    {
        interpolation_type=DC_GetInterpolationType();

        if(DC_INTERPOLATION_TYPE_HW==interpolation_type)
        {
            slice_lines=DC_GetInterpolationSliceLine(sensor_width, photo_width, interpolation_type);
            // scale src buf size
            buf_size=(DC_Algin((sensor_width*slice_lines), 0xff, 0x08)<<0x01);
            // temp buf size
            buf_size+=(DC_Algin((photo_width*slice_lines), 0xff, 0x08)<<0x01);
            // line buf size
            buf_size+=DC_Algin((ISP_ServiceGetSliceScaleLineWidth(photo_width)<<0x03), 0xff, 0x08);
        }
        else if(DC_INTERPOLATION_TYPE_SW==interpolation_type)
        {
            buf_size=(photo_width<<0x04);
        }
    }
    #endif

    DC_Algin(buf_size, 0xff, 0x08);

    return buf_size;
}

/******************************************************************************
// Description: get capture minature jpg temp buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
            photo_width: photo wodth
            photo_height: photo height
// Output:  None 
// Return:  uint32:the length of capture minature jpg temp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetCaptureMinAtureJpgTempBufSize(uint16 sensor_width, uint16 sensor_height,uint16 photo_width, uint16 photo_height)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
//    LCD_INFO_T lcd_info={0x00};
    uint16 quickview_width=0x00;
    uint16 quickview_height=0x00;
    uint32 quickview_yuv_buf_size=0x00;
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6800H))
    if(0x00!=sensor_width)
    {    
        //minature jpg temp buf
        buf_size=DC_Algin((sensor_width*192), 0xff, 0x08);
        buf_size+=DC_Algin((sensor_width*8), 0xff, 0x08);
        buf_size+=DC_Algin((photo_width*96), 0xff, 0x08);
        buf_size+=DC_Algin((1024*90), 0xff, 0x08);

        #if 0
            
        //quick view jpg to yuv buf
        LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

        if(lcd_info.lcd_width<lcd_info.lcd_height)
        {
            quickview_height=lcd_info.lcd_height;
        }
        else
        {
            quickview_height=lcd_info.lcd_width;
        }

        quickview_width=photo_width*quickview_height/photo_height;
        quickview_width=DC_Algin(quickview_width, 0x03, 0x02);
        quickview_yuv_buf_size=(DC_Algin((quickview_width*quickview_height), 0xff, 0x08)<<0x01);
        #else
        quickview_width=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;
        quickview_width=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;

        quickview_yuv_buf_size=(DC_Algin((quickview_width*quickview_height), 0xff, 0x08)<<0x01);
        #endif
        
    }
    
    if(buf_size<quickview_yuv_buf_size)
    {
        buf_size=quickview_yuv_buf_size;
    }
    #else
    buf_size=0x00;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture yuv encoder buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
            photo_mode: photo mode
// Output:  None 
// Return:  uint32:the length of capture yuv encoder buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetYuvEncoderBufSize(uint16 sensor_width, uint16 sensor_height,uint16 photo_width, uint16 photo_height)
{
    uint32 buf_size=0x00;
    uint32 jpg_size=0x00;
    uint32 yuv_size=0x00;
    uint32 fw_size=0x00;
    uint32 disp_size=0x00;

    //pg_size=DC_GetCaptureJpgBufSize(photo_width, photo_height);

	jpg_size = DC_GetCaptureJpgBufSize_Ext(sensor_width, sensor_height,photo_width, photo_height);
    yuv_size=DC_GetCaptureYuvBufSize(sensor_width, sensor_height, photo_width, photo_height);
    fw_size=DC_GetCaptureJpgFwBufSize(sensor_width, photo_width);
    disp_size=MM_GetCaptureDispBufSize();

#if 1 
    if((yuv_size>>0x01)<jpg_size)
    {
        jpg_size=jpg_size-(yuv_size>>0x01)+10*1024;
    }
    else
    {
        jpg_size=10*1024;
    }
#else
     jpg_size=jpg_size-(yuv_size>>0x01)+20*1024;
#endif

    if(fw_size<disp_size)
    {
        fw_size=disp_size;
    }

    buf_size=MM_GetCaptureExifBufSize()
            +MM_GetCaptureThumbnailBufSize()
            +jpg_size
            +yuv_size
            +MM_GetCaptureQuickViewBufSize()
            +DC_GetCaptureSwapBufSize(sensor_width,photo_width)
            +fw_size;

	if(photo_width == 1280)
	{
		//SCI_TRACE_LOW:"Hansen swap:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2182_112_2_18_1_23_35_165,(uint8*)"d", DC_GetCaptureSwapBufSize(sensor_width,photo_width));
	}

    //DC_LOG_TRACE:"DCAM: GetYuvEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2185_112_2_18_1_23_35_166,(uint8*)"ddd", sensor_width, photo_width, buf_size/1024);

    return buf_size;

}

/******************************************************************************
// Description: get capture jpg encoder buffer size
// Author:  Tim.zhu
// Input:   sensor_width: img sensor width
            sensor_height: img sensor height
            photo_mode: photo mode
// Output:  None 
// Return:  uint32:the length of capture jpg encoder buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 DC_GetJpgEncoderBufSize(uint16 sensor_width, uint16 sensor_height,uint16 photo_width, uint16 photo_height)
{
    uint32 src_jpg_size=0x00;
    uint32 dst_jpg_size=0x00;
    uint32 fw_size=0x00;
    uint32 disp_size=0x00;
    uint32 buf_size=0x00;
    
    dst_jpg_size=DC_GetCaptureJpgBufSize(photo_width, photo_height);
    src_jpg_size=DC_GetCaptureJpgBufSize(sensor_width, sensor_height);
    fw_size=DC_GetCaptureJpgFwBufSize(sensor_width,photo_width);
    disp_size=MM_GetCaptureDispBufSize();

    dst_jpg_size=dst_jpg_size-(src_jpg_size>>0x01);

    if(fw_size<disp_size)
    {
        fw_size=disp_size;
    }
    
    buf_size=MM_GetCaptureExifBufSize()
            +MM_GetCaptureThumbnailBufSize()
            +dst_jpg_size
            +src_jpg_size
            +DC_GetCaptureMinAtureJpgTempBufSize(sensor_width, sensor_height, photo_width, photo_height)
            +MM_GetCaptureQuickViewBufSize()
            +DC_GetCaptureSwapBufSize(sensor_width, photo_width)
            +fw_size;

    //DC_LOG_TRACE:"DCAM: GetJpgEncoderBufSize :sensor_w=%d, photo=%d, mem=%dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2229_112_2_18_1_23_36_167,(uint8*)"ddd", sensor_width, photo_width, buf_size/1024);

    return buf_size;

}

/******************************************************************************
// Description: get support max photo size
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      max photo size
******************************************************************************/ 
PUBLIC uint32 DC_GetSensorSize(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext(); 
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    uint16 photo_width=0x00;
    uint16 photo_height=0x00;
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 sensor_jpg_width=0x00;
    uint16 sensor_jpg_height=0x00;
    uint16 sensor_jpg_type=0x00;
    uint16 hardware_type=0x00;
    uint16 software_type=0x00;
    uint32 yuv_buf_size=0;
    uint32 jpg_buf_size=0;
    uint32 buf_size=0;
    uint32 sensor_type=0x00;

    if((PNULL!=pContext->feature_context.mem_ptr)
        &&(0x00!=pContext->feature_context.ex_mem_size))
    {
        buf_size=pContext->feature_context.ex_mem_size;
    }
    else
    {
        buf_size=pDCProductCfg->dc_mode_mem;
    }

    //DC_LOG_TRACE:"DCAM: DC_GetSensorSize mem total: %dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2269_112_2_18_1_23_36_168,(uint8*)"d", buf_size/1024);

    sensor_width=DC_GetMaxImgSensorYuvWidth();
    sensor_height=DC_GetMaxImgSensorYuvHeight();
    sensor_jpg_width=DC_GetMaxImgSensorJpgWidth();
    sensor_jpg_height=DC_GetMaxImgSensorJpgHeight();

    if(DCAMERA_IMG_SENSOR_TYPE_4P3!=DC_GetImgSensorType(NULL))
    {
        for(sensor_type=0; sensor_type<DCAMERA_PHOTO_SIZE_MAX; sensor_type++)
        {
            if(((sensor_width*2)==DC_GetSensorSizeStPtr(sensor_type)->w) &&
                ((sensor_height*2)==DC_GetSensorSizeStPtr(sensor_type)->h))
            {
                break;
            }
        }

        sensor_type=(0xffffffff>>(0x1f-sensor_type));

        return sensor_type;
    }

    hardware_type=_Get_MaxHardWareInterpolationType();
    software_type=_Get_MaxSoftWareInterpolationType();  

    if(software_type<=hardware_type)
    {
        sensor_type=hardware_type;
    }
    else
    {
        sensor_type=software_type;
    }
    //nsor_jpg_type=sensor_type;

    /***************yuv sensor*******************/ 
    if((0x00!=sensor_width)&&(0x00!=sensor_height))
    {
        for( ; DCAMERA_PHOTO_SIZE_176X220<sensor_type; sensor_type--)
        {
            photo_width=DC_GetSensorSizeStPtr(sensor_type)->w;
            photo_height=DC_GetSensorSizeStPtr(sensor_type)->h;
            yuv_buf_size=DC_GetYuvEncoderBufSize(sensor_width, sensor_height, photo_width, photo_height);

			if(photo_width == 1280 )
			{
				//SCI_TRACE_LOW:"Hansen: %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2316_112_2_18_1_23_36_169,(uint8*)"d", yuv_buf_size);
			}

            if(buf_size>=yuv_buf_size)
            {// calc the slice encoder bufer
                break;
            }
        }
    }

    /***************jpg sensor*******************/
    if((0x00!=sensor_jpg_width)&&(0x00!=sensor_jpg_height))
    {
    	sensor_jpg_type=sensor_type;
        /* find the sensor type depend on the width */
        for( ; DCAMERA_PHOTO_SIZE_176X220<sensor_jpg_type; sensor_jpg_type--)
        {
            photo_width=DC_GetSensorSizeStPtr(sensor_type)->w;
            photo_height=DC_GetSensorSizeStPtr(sensor_type)->h;
            jpg_buf_size=DC_GetJpgEncoderBufSize(sensor_width, sensor_height, photo_width, photo_height);

            if(buf_size>=yuv_buf_size)
            {// calc the slice encoder bufer
                break;
            }
        }
    }
    else
    {
        sensor_jpg_type=sensor_type;
    }

    if(sensor_type<sensor_jpg_type)
    {
        sensor_type=sensor_jpg_type;
    }

    pContext->max_photo_width=DC_GetSensorSizeStPtr(sensor_type)->w;
    pContext->max_photo_height=DC_GetSensorSizeStPtr(sensor_type)->h;

    sensor_type=(0xffffffff>>(0x1f-sensor_type));

    return sensor_type;

}


/******************************************************************************
// Description: get img sensor cur format
// Author:      Tim.zhu
// Input:       sensor mode
// Output:      none
// Return:      img sensor cur format
******************************************************************************/ 
PUBLIC SENSOR_IMAGE_FORMAT DC_GetSensorCurFormat(SENSOR_MODE_E sensor_mode)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    if(SENSOR_MODE_MAX>sensor_mode)
    {
        return pContext->sensor_info_ptr->sensor_mode_info[sensor_mode].image_format;
    }
    else
    {
        return SENSOR_IMAGE_FORMAT_MAX;
    }
}

/******************************************************************************
// Description:	judge if zoom supported
// Author:		Tim.zhu
// Input:			none
// Output:		none
// Return:		true or false
******************************************************************************/ 
LOCAL BOOLEAN _DC_IsZoomSupported(uint32* infor_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
//    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    DCAMERA_SIZE_T sensor_size={0x00};
    DCAMERA_SIZE_T photo_size={0x00};
    uint32 size_type=(*infor_ptr)&0xff;    
    uint32 buf_size=0x00;
    uint32 sensor_output_type=0x00;
    BOOLEAN is_support=SCI_FALSE;

    if(DCAMERA_IMG_SENSOR_TYPE_4P3!=DC_GetImgSensorType(NULL))
    {
        //SCI_TRACE_LOW:"DCAM: _DC_IsZoomSupported is not support !4P3"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2402_112_2_18_1_23_36_170,(uint8*)"");
        return SCI_FALSE;
    }
    photo_size.w=DC_GetSensorSizeStPtr(size_type)->w;
    photo_size.h=DC_GetSensorSizeStPtr(size_type)->h;
    
    /* if it need interpolation, zoom is not supported */
    if(DC_GetMaxImgSensorSize()<photo_size.w)
    {
        //SCI_TRACE_LOW:"DCAM: _DC_IsZoomSupported is not support sensor:%d photo:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2411_112_2_18_1_23_36_171,(uint8*)"dd", DC_GetMaxImgSensorSize(),photo_size.w);
        return SCI_FALSE;
    }

    //DC_GetZoomTrimSize(infor_ptr, &sensor_size);

    sensor_output_type=DC_GetCaptureMatchSize(&photo_size);

    sensor_size.w=pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].trim_width;
    sensor_size.h=pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].trim_height;

    if(SENSOR_IMAGE_FORMAT_JPEG==DC_GetSensorCurFormat(sensor_output_type))
    {
        buf_size=DC_GetJpgEncoderBufSize(sensor_size.w, sensor_size.h, photo_size.w, photo_size.h);
    }
    else
    {
        buf_size=DC_GetYuvEncoderBufSize(sensor_size.w, sensor_size.h, photo_size.w, photo_size.h);
    }

    if(NULL!=pContext->feature_context.mem_ptr)
    {
        if(buf_size<pContext->feature_context.ex_mem_size)
        {
            is_support=SCI_TRUE;
        }
    }
    else
    {
        if(buf_size<DC_GetMemSize(DC_STATE_CAPTURING))
        {
            is_support=SCI_TRUE;
        }
    }
    
    return is_support;

}

/******************************************************************************
// Description:	calc max zoom level
// Author:		Tim.zhu
// Input:			none
// Output:		none
// Return:		max zoom level
******************************************************************************/ 
LOCAL uint32 _DC_CalcMaxZoomLevel(uint32* infor_ptr)
{
    SENSOR_EXP_INFO_T* sensor_info_ptr=Sensor_GetInfo(); 
    uint32 size_type=(*infor_ptr)&0xff;
    uint16 disp_mode=((*infor_ptr)>>0x08)&0xff;
    uint16 rotation_mode=((*infor_ptr)>>0x10)&0xff;
    uint16 lcd_id=((*infor_ptr)>>0x18)&0xff;
    uint16 in_width=0x00;
    uint16 in_height=0x00;
    uint16 out_width=0x00;
    uint16 out_height=0x00;
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 min_capture_width=0x00;
    uint16 min_capture_height=0x00;
    uint16 lcd_size_swap=0x00;
    uint16 zoom_coeff=0x00;
    uint16 capture_zoom_coeff=0x00;
    uint32 zoom_level=0x00; 
    LCD_INFO_T lcd_info={0x00};
    DCAMERA_SIZE_T photo={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);/*lint !e64*/

    sensor_width=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_width;
    sensor_height=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_height;

    if((DCAMERA_ROTATION_90==rotation_mode)
        ||(DCAMERA_ROTATION_270==rotation_mode))
    {
        lcd_size_swap=lcd_info.lcd_width;
        lcd_info.lcd_width=lcd_info.lcd_height;
        lcd_info.lcd_height=lcd_size_swap;
    }

    if(DCAMERA_DISP_FULLVIEW==disp_mode)
    {
        if(lcd_info.lcd_width*sensor_height<sensor_width*lcd_info.lcd_height)
        {// calc display height
            lcd_info.lcd_height=(lcd_info.lcd_width*sensor_height)/sensor_width;
        }
        else
        {// calc display width
            lcd_info.lcd_width=(lcd_info.lcd_height*sensor_width)/sensor_height;
        }
    }
    else
    {
        if(lcd_info.lcd_width*sensor_height<sensor_width*lcd_info.lcd_height)
        {// calc img width
            sensor_width=(lcd_info.lcd_width*sensor_height)/lcd_info.lcd_height;
        }
        else
        {// calc img height
            sensor_height=(lcd_info.lcd_height*sensor_width)/lcd_info.lcd_width;
        }
    }

    in_width=sensor_width*ISP_ServiceGetMaxPreviewScaleUpCoeff();
    in_height=sensor_height*ISP_ServiceGetMaxPreviewScaleUpCoeff();
    out_width=lcd_info.lcd_width;
    out_height=lcd_info.lcd_height;
    
    if(out_width*in_height<in_width*out_height)
    {// depend on out height
        zoom_coeff=in_height*100/out_height;
    }
    else
    {// depend on out width
        zoom_coeff=in_width*100/out_width;
    }

    photo.w=DC_GetSensorSizeStPtr(size_type)->w;
    photo.h=DC_GetSensorSizeStPtr(size_type)->h;

    size_type=DC_GetCaptureMatchSize(&photo);
    min_capture_width=sensor_info_ptr->sensor_mode_info[size_type].trim_width;
    min_capture_height=sensor_info_ptr->sensor_mode_info[size_type].trim_height;

    if((photo.w<min_capture_width)
        &&(photo.h<min_capture_height))
    {
        min_capture_width *=ISP_ServiceGetMaxPreviewScaleUpCoeff();     //actual is multi with capture scalup coeff
        min_capture_height *=ISP_ServiceGetMaxPreviewScaleUpCoeff();
    
        if(photo.w*min_capture_height<photo.h*min_capture_width)
        {// depend on out height
            capture_zoom_coeff=min_capture_height*100/photo.h;
        }
        else
        {// depend on out width
            capture_zoom_coeff=min_capture_width*100/photo.w;
        }
    }
    else
    {
        min_capture_width /=ISP_ServiceGetMaxScaleUpCoeff();
        min_capture_height /=ISP_ServiceGetMaxScaleUpCoeff();

        if(photo.w*min_capture_height<photo.h*min_capture_width)
        {// depend on out height
            capture_zoom_coeff=photo.h*100/min_capture_height;
        }
        else
        {// depend on out width
            capture_zoom_coeff=photo.w*100/min_capture_width;
        }
    }

    if(capture_zoom_coeff<zoom_coeff)
    {
        zoom_coeff=capture_zoom_coeff;
    }

    if(125>zoom_coeff)
    {
        zoom_level=ISP_SERVICE_ZOOM_100X;
    }
    else if(150>zoom_coeff)
    {
        zoom_level=ISP_SERVICE_ZOOM_125X;
    }
    else if(175>zoom_coeff)
    {
        zoom_level=ISP_SERVICE_ZOOM_150X;
    }
    else if(200>zoom_coeff)
    {
        zoom_level=ISP_SERVICE_ZOOM_175X;
    }
    else
    {
        zoom_level=ISP_SERVICE_ZOOM_200X;
    }

    ISP_ServiceSetMaxZoomLevel(zoom_level);

    zoom_level+=0x01;
    
    return zoom_level;
    
}

/******************************************************************************
// Description: get max zoom level
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      zoom level,0~4
******************************************************************************/ 
PUBLIC uint8 DC_GetZoomMaxLevel(uint32* infor_ptr)
{
    uint8 zoomlevel = 0;
    
    //SCI_TRACE_LOW:"DCAM: DC_GetZoomMaxLevel start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2609_112_2_18_1_23_36_172,(uint8*)"");
    if(_DC_IsZoomSupported(infor_ptr))
    {
        zoomlevel=_DC_CalcMaxZoomLevel(infor_ptr);
    }
    else
    {
        zoomlevel=0x00;
    }
    //SCI_TRACE_LOW:"DCAM: DC_GetZoomMaxLevel :%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2618_112_2_18_1_23_36_173,(uint8*)"d", zoomlevel);
    return zoomlevel;
}

/******************************************************************************
// Description: get continue shoot max num
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      conuintue shoot num  9~0
******************************************************************************/ 
PUBLIC uint8 DC_GetContinueShootMaxNum(uint32* size)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    DCAMERA_SIZE_T* photo_size_ptr=DC_GetSensorSizeStPtr(*size);
    uint32 buf_size=0;    
    uint32 src_buf_size=0x00;
    uint32 encoder_buf_size=0x00;
    uint32 sensor_output_type=SENSOR_MODE_PREVIEW_ONE;
    uint16 preview_width=0x00;
    uint16 preview_height=0x00;
    uint8 continue_shoot_num=0x00;

    if(DCAMERA_IMG_SENSOR_TYPE_4P3!=DC_GetImgSensorType(NULL))
    {
        return continue_shoot_num;
    }
    //SCI_TRACE_LOW:"---DC_GetContinueShootMaxNum:%d,%d---"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2644_112_2_18_1_23_36_174,(uint8*)"dd",pContext->sensor_info_ptr->source_width_max,photo_size_ptr->w);
    if(pContext->sensor_info_ptr->source_width_max<photo_size_ptr->w)
    {
        return DC_CONTINUE_SHOOT_DISABLE;
    }

    if(pContext->feature_context.mem_ptr!=PNULL)
    {
        buf_size=pContext->feature_context.ex_mem_size;
    }
    else
    {
        buf_size=DC_GetMemSize(DC_STATE_BURST_CAPTURING);
    }

    src_buf_size=(DC_Algin((photo_size_ptr->w*photo_size_ptr->h),0xff, 0x08))<<0x01;
    encoder_buf_size=src_buf_size/6;

    encoder_buf_size+=MM_GetCaptureExifBufSize();

    sensor_output_type=DC_GetCaptureMatchSize(photo_size_ptr);

    if(SENSOR_IMAGE_FORMAT_YUV422==pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].image_format)
    {//src buffer can save the last encoder data
        //src_buf_size-=encoder_buf_size; 
    }
    else if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].image_format)
    {//jpg sensor not need src buf, the buf for quickview decoder
        preview_width=pContext->sensor_info_ptr->sensor_mode_info[PREVIEW_MODE].trim_width;
        preview_height=pContext->sensor_info_ptr->sensor_mode_info[PREVIEW_MODE].trim_height;
        src_buf_size=(DC_Algin((preview_width*preview_height),0xff, 0x08))<<0x01; 
    }

    if(MM_GetContinueShotJpgFwBufSize()<MM_GetContinueShotDispBufSize())
    {
        src_buf_size+=MM_GetContinueShotDispBufSize();
    }
    else
    {
        src_buf_size+=MM_GetContinueShotJpgFwBufSize();
    }

    src_buf_size+=MM_GetContinueShotRotBufSize();

    continue_shoot_num=(buf_size-src_buf_size)/encoder_buf_size;

    if(DCAMERA_BURST_SNAPSHOT_IMAGE_MAX<continue_shoot_num)
    {
        continue_shoot_num=DCAMERA_BURST_SNAPSHOT_IMAGE_MAX;
    }

    if(0x09<=continue_shoot_num)
    {
        continue_shoot_num=DC_CONTINUE_SHOOT_9;
    }
    else if(0x06<=continue_shoot_num)
    {
        continue_shoot_num=DC_CONTINUE_SHOOT_6;
    }
    else if(0x03<=continue_shoot_num)
    {
        continue_shoot_num=DC_CONTINUE_SHOOT_3;
    }
    else
    {
        continue_shoot_num=DC_CONTINUE_SHOOT_DISABLE;
    }

    //SCI_TRACE_LOW:"DCAM: DC_GetContinueShootMaxNum :%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2712_112_2_18_1_23_36_175,(uint8*)"d", continue_shoot_num);

    return continue_shoot_num;

}

/******************************************************************************
// Description: get img sensor type
// Author:      Tim.zhu
// Input:       none
// Output:      none
// Return:      sensor type
******************************************************************************/ 
PUBLIC uint32 DC_GetImgSensorType(uint32* param)
{
    SENSOR_EXP_INFO_T* sensor_info_ptr = Sensor_GetInfo();
    uint32 img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_MAX;

    if(sensor_info_ptr->source_height_max<sensor_info_ptr->source_width_max)
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_4P3;
    }
    else if(sensor_info_ptr->source_height_max>sensor_info_ptr->source_width_max)
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_3P4;
    }
    else
    {
        img_sensor_type=DCAMERA_IMG_SENSOR_TYPE_EQ;
    }

    return img_sensor_type;

}

/******************************************************************************
// Description: 	configure the memory block tables
// Author:     		Tim.zhu
// Input:      		case_id--indicate the case which needs to be configured
//				pContext --context pointer to MM
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/

PUBLIC uint32 DC_ExGetFeatureSupportInfo(uint32 ex_mem_size)
{
    MM_FEATURE_SUPPORT_T *p_feature_context=PNULL;
    DCAM_CONTEXT_T* pDCContext=PNULL;
    uint32 nRet = SCI_SUCCESS;
    uint32 index;
    uint32 temp_size=0;
    uint32 rotation_buf_size=0;
    uint32 disp_buf_size=0;
    uint32 jpg_fw_buf_size=0;
    uint8 num=0;
    uint8 capture_type=0;

    if(ex_mem_size==0)
    {
        return SCI_ERROR;
    }
	
    pDCContext=_GetDCAMContext();

    p_feature_context=(MM_FEATURE_SUPPORT_T*)&(pDCContext->feature_context);

    p_feature_context->ex_mem_size=ex_mem_size;
    p_feature_context->feature_support_bit=0;

    temp_size+=MM_GetPreviewCapBufSize();
    temp_size+=MM_GetPreviewRotBufSize();

    if(p_feature_context->ex_mem_size>=temp_size)
    {
        p_feature_context->feature_support_bit|=PREVIEW_SUPPORT;
    }
    capture_type=DC_GetSensorSize();
    p_feature_context->max_sup_capture_size=capture_type; 
    rotation_buf_size=MM_GetCaptureRotBufSize();
    disp_buf_size=MM_GetCaptureDispBufSize();
    jpg_fw_buf_size=MM_GetCaptureJpgFwBufSize();

    if(jpg_fw_buf_size<disp_buf_size)
    {
        jpg_fw_buf_size=disp_buf_size;
    }

    if(jpg_fw_buf_size<rotation_buf_size)
    {
        jpg_fw_buf_size=rotation_buf_size;
    }
    temp_size=jpg_fw_buf_size;
    temp_size+=MM_GetCaptureSwapBufSize();
    temp_size+=MM_GetCaptureQuickViewBufSize();
    temp_size+=MM_GetCaptureYuvBufSize();

    if(p_feature_context->ex_mem_size>=temp_size)
    {
        p_feature_context->feature_support_bit|=CAPTURE_SUPPORT;
    }
    for(index=0;index<DCAMERA_PHOTO_SIZE_MAX;index++)
    {
        num=DC_GetContinueShootMaxNum(&index);
        if(DC_GetSensorSizeStPtr(index)->w<QVGA_WIDTH)
        {
            p_feature_context->max_burst_shot_num[index]=DC_CONTINUE_SHOOT_6;
        }
        else
        {
            p_feature_context->max_burst_shot_num[index]=num;
        }
    }
    temp_size= MM_GetDvRecorderDispBufSize();
    temp_size+=MM_GetDvRecorderScaleBufSize();
    temp_size+=MM_GetDvRecorderCapBufSize();
    temp_size+=MM_GetDvRecorderRotBufSize();
    temp_size+=MM_GetDvRecorderFramePoolSize();

    if(p_feature_context->ex_mem_size>=temp_size)
    {
        p_feature_context->feature_support_bit|=DV_SUPPORT;
    }

    return nRet;
}

/******************************************************************************
// Description: match the memory size
// Author:     	Tim.zhu
// Input:      	cur_size: currect size
//				max_size: max size
// Output:		none
// Return:		the size of match
// Note:       	none
******************************************************************************/
LOCAL uint32 _DC_MatchMemSize(uint32 cur_size, uint32 max_size)
{
    uint32 mem_size=cur_size;

    if(mem_size<max_size)
    {
        mem_size+=0xff;
    }
    else if(mem_size>max_size)
    {
        mem_size=0x00;
    }

    return mem_size;
}

/******************************************************************************
// Description: 	get the memory size of dc feature
// Author:     		Tim.zhu
// Input:      		dc_state: cur dc state
// Output:		none
// Return:		none
// Note:        the size of memory
******************************************************************************/
PUBLIC uint32 DC_GetMemSize(DC_STATE_E dc_state)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    uint32 max_buf_size=pDCProductCfg->dc_mode_mem;
    uint32 buf_size=0x00;
	uint32 tmp_size = 0;

    //SCI_TRACE_LOW:"DCAM: DC_GetMemSize mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2876_112_2_18_1_23_37_176,(uint8*)"d",dc_state);


    switch(dc_state)
    {
        case DC_STATE_PREVIEWING:
        {
            buf_size=MM_GetPreviewCapBufSize()
                    +MM_GetPreviewRotBufSize();
            
            break;
        }
        case DC_STATE_CAPTURING:
        {
 
            uint16 sensor_width=0x00;
            uint16 sensor_height=0x00;
            uint16 sensor_jpg_width=0x00;
            uint16 sensor_jpg_height=0x00;
            uint32 yuv_buf_size=0;
            uint32 jpg_buf_size=0;

            sensor_width=DC_GetMaxImgSensorYuvWidth();
            sensor_height=DC_GetMaxImgSensorYuvHeight();
            sensor_jpg_width=DC_GetMaxImgSensorJpgWidth();
            sensor_jpg_height=DC_GetMaxImgSensorJpgHeight();

            DC_GetSensorSize();

            /***************yuv sensor*******************/ 
            if((0x00!=sensor_width)&&(0x00!=sensor_height))
            {
                yuv_buf_size=DC_GetYuvEncoderBufSize(sensor_width, sensor_height, pContext->max_photo_width, pContext->max_photo_height);
            }

            /***************jpg sensor*******************/
            if((0x00!=sensor_jpg_width)&&(0x00!=sensor_jpg_height))
            {
                jpg_buf_size=DC_GetJpgEncoderBufSize(sensor_width, sensor_height, pContext->max_photo_width, pContext->max_photo_height);
            }

            if(jpg_buf_size<yuv_buf_size)
            {
                buf_size=yuv_buf_size;
            }
            else
            {
                buf_size=jpg_buf_size;
            }

            break;
        }
        case DC_STATE_BURST_CAPTURING:
        {
            buf_size=pDCProductCfg->dc_mode_mem;
            break;
        }
        case DC_STATE_REC:
        case DC_STATE_UPCC_REC:
        {
            max_buf_size=pDCProductCfg->dv_mode_mem;

#if 0
            buf_size=MM_GetDvRecorderDispBufSize()
                     +MM_GetDvRecorderScaleBufSize()
                     +MM_GetDvRecorderRotBufSize()
                     +MM_GetDvRecorderCapBufSize()
                     +MM_GetDvRecorderFramePoolSize()
                     +MM_GetDvRecorderJpgFwBufSize();
#else
                     //cap and scaling share the same buffer
			tmp_size = MM_GetDvRecorderCapBufSize();
			if(tmp_size < MM_GetDvRecorderScaleBufSize() )
			{
				tmp_size = MM_GetDvRecorderScaleBufSize();
			}
                     buf_size=MM_GetDvRecorderDispBufSize()
				     + tmp_size
                     //+MM_GetDvRecorderScaleBufSize()
                     +MM_GetDvRecorderRotBufSize()
                     //+MM_GetDvRecorderCapBufSize()
                     +MM_GetDvRecorderFramePoolSize()
                     +MM_GetDvRecorderJpgFwBufSize();


#endif
            break;
        }
        case DC_STATE_VIDEO_REVIEW:
        {
            buf_size=MM_GetDvPlayDispBufSize()
                    +MM_GetDvPlayRotBufSize();
            break;
        }
        default :
        {
            break;
        }

    }

    buf_size=_DC_MatchMemSize(buf_size, max_buf_size);

    //SCI_TRACE_LOW:"DCAM: DC_GetMemSize buf_size:0x%x, 0x%xk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_MISC_2979_112_2_18_1_23_37_177,(uint8*)"dd", buf_size, buf_size/1024);
    
    return buf_size;
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End

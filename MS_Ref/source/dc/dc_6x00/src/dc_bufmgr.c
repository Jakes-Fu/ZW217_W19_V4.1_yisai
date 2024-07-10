/******************************************************************************
 ** File Name:	mm_bufmgr.c                                               	  
 ** Author:		Frank.Yang		                                          
 ** DATE:		8/4/2008                                               
 ** Copyright:		2008 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION                            
 ******************************************************************************/
#include "ms_ref_dc_trc.h"
#include "dal_lcd.h"
#include "dc_bufmgr.h"
#include "block_mem_def.h"
#include "dc_app_6x00.h"
#include "sensor_drv.h"
#include "dc_misc.h"
#include "chip.h"
#include "dc_cfg.h"
/*lint -save -e415 */
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define MM_BUF_ALGIN                0xff
#define MM_BUF_ALGIN_BITS           0x08

#define MM_BUF_START_ADDR           (0x00)
#define MM_ISP_DISP_LOOP_BUF_NUM    4

#define MM_EXIF_SIZE (20*1024)
#define MM_THUM_SIZE (100*1024)

#define MM_SUB(x, y)\
do\
{\
            if(x> y)\
			{\
				x = x - y;\
			}\
			else\
			{\
				Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;\
				break;\
			}\
}while(0);
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL BLOCK_MEM_ID_E s_mem_id=BLOCK_MEM_POOL_REF;

//--------------------------------relative to preview buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_preview_mem_cfg[] =
{
    {MM_DC_PREVIEW,          0, 0, MM_NOT_USED, TRUE},
    {MM_DC_PREVIEW_DISP,     0, 0, MM_NOT_USED, TRUE},
    {MM_DC_PREVIEW_ROT,      0, 0, MM_NOT_USED, TRUE},
};

//--------------------------------relative to snapshot buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_capture_mem_cfg[] =
{
    {MM_DC_CAPTURE,               0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_EXIF,          0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_THUMBNAIL,     0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_JPEG,          0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_YUV,           0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_MINATURE_SRC,  0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_MINATURE_SWAP, 0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_QUICK_VIEW,    0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_SWAP,          0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_JPEG_FW,       0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_DISP,          0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_ROT,           0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CAPTURE_EXIF_SWAP,     0, 0, MM_NOT_USED, TRUE},
};

//--------------------------------relative to panorama buffer assign---------------------------------------------//
static MM_BLOCK_MEM_T mm_panorama_mem_cfg[] =
{
    {MM_DC_PANORAMA,            0, 0, MM_NOT_USED, TRUE},
    {MM_DC_PANORAMA_JPEG,       0, 0, MM_NOT_USED, FALSE},
    {MM_DC_PANORAMA_STAT,       0, 0, MM_NOT_USED, FALSE},
    {MM_DC_PANORAMA_YUV,        0, 0, MM_NOT_USED, FALSE},
    {MM_DC_PANORAMA_PREVIEW,    0, 0, MM_NOT_USED, TRUE},
    {MM_DC_PANORAMA_QUICK_VIEW, 0, 0, MM_NOT_USED, FALSE},
    {MM_DC_PANORAMA_JPEG_ENC,   0, 0, MM_NOT_USED, FALSE},
};

//--------------------------------relative to fourINone buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_fourinone_mem_cfg[] =
{
    {MM_DC_FOURinONE,            0, 0, MM_NOT_USED, TRUE},
    {MM_DC_FOURinONE_JPEG,       0, 0, MM_NOT_USED, FALSE},
    {MM_DC_FOURinONE_OSD,        0, 0, MM_NOT_USED, FALSE},
    {MM_DC_FOURinONE_YUV,        0, 0, MM_NOT_USED, FALSE},
    {MM_DC_FOURinONE_PREVIEW,    0, 0, MM_NOT_USED, FALSE},     
    {MM_DC_FOURinONE_QUICK_VIEW, 0, 0, MM_NOT_USED, FALSE}, 
    {MM_DC_FOURinONE_SWAP,       0, 0, MM_NOT_USED, FALSE},      
    {MM_DC_FOURinONE_JPEG_ENC,   0, 0, MM_NOT_USED, FALSE},
};

//--------------------------------relative to review buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_review_mem_cfg[] =
{
    {MM_DC_REVIEW,      0, 0, MM_NOT_USED, TRUE},
    {MM_DC_REVIEW_YUV,  0, 0, MM_NOT_USED, TRUE},
    {MM_DC_REVIEW_FW,   0, 0, MM_NOT_USED, TRUE},
    {MM_DC_REVIEW_DISP, 0, 0, MM_NOT_USED, TRUE},
    {MM_DC_REVIEW_ROT,  0, 0, MM_NOT_USED, TRUE},
};

//--------------------------------relative to burstshot buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_continueshot_mem_cfg[] =
{
    {MM_DC_CONTINUESHOT,         0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_EXIF,    0, 0, MM_NOT_USED, TRUE},        
    {MM_DC_CONTINUESHOT_JPEG,    0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_YUV,     0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_DISP,    0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_SWAP,    0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_JPEG_FW, 0, 0, MM_NOT_USED, TRUE},
    {MM_DC_CONTINUESHOT_ROT,     0, 0, MM_NOT_USED, TRUE},
};

//--------------------------------relative to DV shot buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_dv_shot_mem_cfg[] =
{
    {MM_DV_SHOT,            0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_DISPLAY,    0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_YUV,        0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_ROT,        0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_CAP,        0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_FRAME_POOL, 0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_JPEG_ENC,   0, 0, MM_NOT_USED, TRUE},
    {MM_DV_SHOT_MUX,        0, 0, MM_NOT_USED, TRUE},
};

//--------------------------------relative to DV review buffer assign------------------------------------------------//
static MM_BLOCK_MEM_T mm_dv_play_mem_cfg[] =
{
    {MM_DV_PLAY,            0, 0, MM_NOT_USED, TRUE},
    {MM_DV_PLAY_DISP,       0, 0, MM_NOT_USED, TRUE},
    {MM_DV_PLAY_ROT,        0, 0, MM_NOT_USED, TRUE},
};

LOCAL const MM_BLOCK_MEM_TBL_T mm_mem_cfg_tbl[] =
{
    {mm_preview_mem_cfg,        sizeof(mm_preview_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_capture_mem_cfg,        sizeof(mm_capture_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_panorama_mem_cfg,       sizeof(mm_panorama_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_fourinone_mem_cfg,      sizeof(mm_fourinone_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_review_mem_cfg,         sizeof(mm_review_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_continueshot_mem_cfg,   sizeof(mm_continueshot_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_dv_shot_mem_cfg,        sizeof(mm_dv_shot_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
    {mm_dv_play_mem_cfg,        sizeof(mm_dv_play_mem_cfg)/sizeof(MM_BLOCK_MEM_T)},
};

static MM_CONTEXT_T	g_mm_context={0}; 

/*--------------------------- External Function ------------------------------*/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/******************************************************************************
// Description:	free MM raw buffer
// Author:		frank.yang
// Input:			state
// Output:		plen-pointer to buffer alloced
// Return:		buffer addr
// Note:			64 words align
******************************************************************************/ 
LOCAL void MM_FreeRawBuffer(void)
{
    DCAM_CONTEXT_T* pContext = _GetDCAMContext();

    if(NULL!=pContext->dc_raw_buf_ptr)
    {
        SCI_FREE(pContext->dc_raw_buf_ptr);
        pContext->dc_raw_buf_ptr=NULL;
        pContext->dc_raw_buf_len=0x00;
        pContext->total_buf_ptr=NULL;
        pContext->total_buf_len=0x00;
    }

	g_mm_context.buf_ptr = PNULL;
	g_mm_context.buf_len = 0;
}

/******************************************************************************
// Description:	get MM raw buffer
// Author:		frank.yang
// Input:			state
// Output:		plen-pointer to buffer alloced
// Return:		buffer addr
// Note:			64 words align
******************************************************************************/ 
LOCAL void* MM_AllocRawBuffer(DC_STATE_E dc_state, uint32* pLen)
{
    uint32 temp=0x00;
    uint32 buf_size=0x00;
    uint8* buf_ptr=NULL;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    pContext->total_buf_ptr=NULL;
    pContext->total_buf_len=0x00;

    buf_size=DC_GetMemSize(dc_state);

    if(NULL!=pContext->dc_raw_buf_ptr)
    {
        if((0x00!=buf_size)
            &&(buf_size<=pContext->dc_raw_buf_len))
        {
            buf_ptr=pContext->dc_raw_buf_ptr;
            temp=(uint32)buf_ptr;
            buf_ptr=(uint8*)DC_Algin((uint32)buf_ptr, 0xff, 0x08);
            buf_size=buf_size-((uint32)buf_ptr-temp);
            buf_size=DC_Algin(buf_size, 0x00, 0x08);
            pContext->total_buf_ptr=buf_ptr;
            pContext->total_buf_len=buf_size;
        }
        else
        {
            MM_FreeRawBuffer();
        }
    }
    
    if(NULL==pContext->dc_raw_buf_ptr)
    {
        if(0x00!=buf_size)
        {
            buf_ptr=(uint8*)SCI_ALLOCA(buf_size);

            if(NULL==buf_ptr)
            {
                pContext->dc_raw_buf_ptr=NULL;
                pContext->dc_raw_buf_len=0x00;
                //SCI_TRACE_LOW:"DCAM:MM_AllocRawBuffer: Buffer allaoc fail err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_236_112_2_18_1_23_24_65,(uint8*)"");
            }
            else
            {
                DC_UpdateCache((uint32)buf_ptr, (buf_size+0x04), DC_OP_READ_WRITE);
                pContext->dc_raw_buf_ptr=buf_ptr;
                pContext->dc_raw_buf_len=buf_size;
                temp=(uint32)buf_ptr;
                buf_ptr=(uint8*)DC_Algin((uint32)buf_ptr, 0xff, 0x08);
                buf_size=buf_size-((uint32)buf_ptr-temp);
                buf_size=DC_Algin(buf_size, 0x00, 0x08);
                pContext->total_buf_ptr=buf_ptr;
                pContext->total_buf_len=buf_size;
            }
        }

    }
    else
    {
        //SCI_TRACE_LOW:"DCAM: MM_AllocRawBuffer: Buffer is allaoced"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_255_112_2_18_1_23_24_66,(uint8*)"");
    }

    *pLen=pContext->total_buf_len;

    return (void*)pContext->total_buf_ptr;
}

/******************************************************************************
// Description: get lcdc osd-rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of lcdc osd-rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetLcdcBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;

    return buf_size;
}

/******************************************************************************
// Description: get preview cap buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview cap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetPreviewCapBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;
	uint32 atv_size = 0x00;
    uint32 param=0x00;
 
	
    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    buf_size=DC_Algin((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x02;

    if(SENSOR_TYPE_ATV == _Sensor_GetSensorType() )
	{
    	param=(ATV_CMD_GET_EMC<<0x10)&0xffff0000;
    	param=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
		//sync with the isp_service code : _ISP_SetATVPreviewParam
		atv_size = (param&0xfff)*((param>>0x10)&0xfff);
		atv_size = (atv_size+1024) * 2;

		atv_size = atv_size + (buf_size+1)/2;
					
		if(atv_size > buf_size )
		{
			buf_size = atv_size;
		}
    }

    return buf_size;
	
}

/******************************************************************************
// Description: get preview rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetPreviewRotBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    #if (defined(PLATFORM_SC6600L))
    buf_size=0x00;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x02;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureRotBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    buf_size=0x00;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((pDCContext->preview_param.disp_rect.w*pDCContext->preview_param.disp_rect.h),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture exif swap buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture exif swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureExifSwapBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    buf_size=0x00;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((pDCContext->preview_param.disp_rect.w*pDCContext->preview_param.disp_rect.h),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture jpg encoder fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture encoder fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureJpgFwBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;
    uint16 sensor_width=0x00;
    uint16 photo_width=0x00;

    photo_width=pDCContext->target_width;    
    sensor_width=DC_GetMaxImgSensorYuvWidth();

    buf_size=DC_GetCaptureJpgFwBufSize(sensor_width, photo_width);

    return buf_size;
}

/******************************************************************************
// Description: get capture display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture interpolate swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureDispBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    
    #if (defined(PLATFORM_SC6600L))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif
    
    return buf_size;
}

/******************************************************************************
// Description: get capture interpolate swap (slice temp and line)buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture interpolate swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureSwapBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint16 photo_width=0x00; 
    uint32 buf_size=0x00;

    photo_width=pDCContext->target_width; 
    sensor_width=DC_GetMaxImgSensorYuvWidth();

    buf_size=DC_GetCaptureSwapBufSize(sensor_width, photo_width);

    return buf_size;
}

/******************************************************************************
// Description: get capture rotation  buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureRotationBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;
    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    #if (defined(PLATFORM_SC6600L))

    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif
    
    return buf_size;
}

/******************************************************************************
// Description: get capture quick view buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture quick view buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureQuickViewBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    
    #if (defined(PLATFORM_SC6600L))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif
    
    return buf_size;
}

/******************************************************************************
// Description: get capture yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview osd buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureYuvBufSize(void)
{
    DCAM_CONTEXT_T_PTR pDCContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 photo_width=0x00;
    uint16 photo_height=0x00;
    uint32 buf_size=0x00;

    photo_width=pDCContext->target_width;
    photo_height=pDCContext->target_height;
    sensor_width=DC_GetMaxImgSensorYuvWidth();
    sensor_height=DC_GetMaxImgSensorYuvHeight();

    buf_size=DC_GetCaptureYuvBufSize(sensor_width, sensor_height, photo_width, photo_height);

    return buf_size;
}

/******************************************************************************
// Description: get capture minature jpg temp buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of minature jpg temp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureMinAtureJpgTempBufSize(void)
{
    DCAM_CONTEXT_T_PTR pDCContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 photo_width=0x00;
    uint16 photo_height=0x00;
    uint32 buf_size=0x00;
    
    photo_width=pDCContext->target_width;
    photo_height=pDCContext->target_height;
    sensor_width=DC_GetMaxImgSensorJpgWidth();
    sensor_height=DC_GetMaxImgSensorJpgHeight();
    
    buf_size=DC_GetCaptureMinAtureJpgTempBufSize(sensor_width, sensor_height, photo_width, photo_height);

    return buf_size;
}

/******************************************************************************
// Description: get capture minature jpg src buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of minature jpg src buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureMinAtureJpgSrcBufSize(void)
{
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6800H))
    sensor_width=DC_GetMaxImgSensorJpgWidth();
    sensor_height=DC_GetMaxImgSensorJpgHeight();

    buf_size=DC_GetCaptureJpgBufSize(sensor_width, sensor_height);

    #else
    buf_size=0x00;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture minature jpg dst buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of minature jpg dst buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureMinAtureJpgDstBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    uint16 photo_width=0x00;
    uint16 photo_height=0x00;
    uint32 src_buf_size=0x00;
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6800H))
    sensor_width=DC_GetMaxImgSensorJpgWidth();
    sensor_height=DC_GetMaxImgSensorJpgHeight();
    photo_width=pDCContext->target_width;
    photo_height=pDCContext->target_height;

    src_buf_size=DC_GetCaptureJpgBufSize(sensor_width, sensor_height);
    buf_size=DC_GetCaptureJpgBufSize(photo_width, photo_height);

    buf_size=buf_size-(src_buf_size>>0x01);
    #else
    buf_size=0x00;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get capture exif  buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture exif buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureExifBufSize(void)
{
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();   
    uint32 buf_size=0x00;

    if((NULL!=pDCProductCfg)
        &&(SCI_TRUE==pDCProductCfg->exif_eb))
    {
        buf_size=DC_Algin(MM_EXIF_SIZE, MM_BUF_ALGIN, MM_BUF_ALGIN_BITS);
    }

    return buf_size;
}

/******************************************************************************
// Description: get capture thumbnail buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture thumbnail buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureThumbnailBufSize(void)
{
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();   
    uint32 buf_size=0x00;

    if((NULL!=pDCProductCfg)
        &&(SCI_TRUE==pDCProductCfg->exif_eb)
        &&(SCI_TRUE==pDCProductCfg->thumbnail_eb))
    {
        buf_size=DC_Algin(MM_THUM_SIZE, MM_BUF_ALGIN,MM_BUF_ALGIN_BITS);
    }
    
    return buf_size;
}

/******************************************************************************
// Description: get continue shot jpg fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot jpg fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotJpgFwBufSize(void)
{
    uint32 buf_size=0x00;

    buf_size=DC_Algin((1024*80),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS);

    return buf_size;
}

/******************************************************************************
// Description: get continue shot rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotRotBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    #if (defined(PLATFORM_SC6600L))
    buf_size=0x00;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get continue shot display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue display buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotDispBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    #if (defined(PLATFORM_SC6600L))
    buf_size=0x00;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get continue shot yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotYuvBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 sensor_output_type=SENSOR_MODE_MAX;
    uint32 buf_size=0x00;
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    DCAMERA_SIZE_T photo_size={0x00};

    photo_size.w=pDCContext->target_width;
    photo_size.h=pDCContext->target_height;

    sensor_output_type=DC_GetCaptureMatchSize(&photo_size);
    
    sensor_width=pDCContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].trim_width;
    sensor_height=pDCContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].trim_height;

    if(SENSOR_IMAGE_FORMAT_JPEG==pDCContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].image_format)
    {//jpg sensor not need src buf
        sensor_width=pDCContext->sensor_info_ptr->sensor_mode_info[pDCContext->preview_mode].trim_width;
        sensor_height=pDCContext->sensor_info_ptr->sensor_mode_info[pDCContext->preview_mode].trim_height;
    }

    buf_size=DC_Algin((sensor_width*sensor_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01; 

    return buf_size;
}

/******************************************************************************
// Description: get image jpg size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of image jpg
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetImgJpgSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;
    uint16 photo_width=0x00;
    uint16 photo_height=0x00;
    
    photo_width=pDCContext->target_width;
    photo_height=pDCContext->target_height;

    buf_size=DC_Algin(((photo_width*photo_height)*2/0x06),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS);

    return buf_size;
}

/******************************************************************************
// Description: get review yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewYuvBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 sensor_output_type=SENSOR_MODE_MAX;
    uint16 sensor_width=0x00;
    uint16 sensor_height=0x00;
    DCAMERA_SIZE_T photo_size={0x00};
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    buf_size=QVGA_WIDTH*QVGA_HEIGHT*2;
    #elif (defined(PLATFORM_SC6800H))

    photo_size.w=pDCContext->target_width;
    photo_size.h=pDCContext->target_height;
    sensor_output_type=DC_GetCaptureMatchSize(&photo_size);
    
    if(SENSOR_IMAGE_FORMAT_JPEG==pDCContext->sensor_info_ptr->sensor_mode_info[sensor_output_type].image_format)
    {//jpg sensor not need src buf

        sensor_width=pDCContext->sensor_info_ptr->sensor_mode_info[pDCContext->preview_mode].trim_width;
        sensor_height=pDCContext->sensor_info_ptr->sensor_mode_info[pDCContext->preview_mode].trim_height;
        buf_size=DC_Algin((sensor_width*sensor_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01; 
    }
    else
    {
        photo_size.w=pDCContext->review_param.disp_rect.w;
        photo_size.h=pDCContext->review_param.disp_rect.h;
        buf_size=DC_Algin((photo_size.w*photo_size.h),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;

        if((QVGA_WIDTH*QVGA_HEIGHT*2)>buf_size)
        {
            buf_size=QVGA_WIDTH*QVGA_HEIGHT*2;
        }
    }
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get review rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewRotBufSize(void)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((pDCContext->review_param.disp_rect.w*pDCContext->review_param.disp_rect.h),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get review display buf buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review display buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewDispBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    #if (defined(PLATFORM_SC6600L))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((pDCContext->review_param.disp_rect.w*pDCContext->review_param.disp_rect.h),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get review jpg fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review jpg fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewJpgFwBufSize(void)
{
    uint32 buf_size=0x00;

    buf_size=DC_Algin((1024*80),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS);

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder disp buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder disp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderDispBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    #if (defined(PLATFORM_SC6600L))
    buf_size=DC_Algin((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #elif (defined(PLATFORM_SC6800H))
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder scale buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder yuv scale buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderScaleBufSize(void)
{
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    buf_size=QVGA_SIZE;
    #elif (defined(PLATFORM_SC6800H))
    if(NULL!=pDCProductCfg)
    {
        if(DC_MAX_VIDEO_MODE_CIF==pDCProductCfg->max_video_mode)
        {
            buf_size=DC_Algin((CIF_WIDTH*CIF_HEIGHT), MM_BUF_ALGIN, MM_BUF_ALGIN_BITS)<<0x01;
        }
        else if(DC_MAX_VIDEO_MODE_VGA==pDCProductCfg->max_video_mode)
        {
            buf_size=DC_Algin((VGA_WIDTH*VGA_HEIGHT), MM_BUF_ALGIN, MM_BUF_ALGIN_BITS)<<0x01;
        }
        else
        {
            buf_size=DC_Algin((QVGA_WIDTH*QVGA_HEIGHT), MM_BUF_ALGIN, MM_BUF_ALGIN_BITS)<<0x01;
        }
    }
    else
    {
        buf_size=DC_Algin(QVGA_SIZE, MM_BUF_ALGIN, MM_BUF_ALGIN_BITS);
    }
    #endif

    buf_size = buf_size + 1024;

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderRotBufSize(void)
{
    LCD_INFO_T lcd_info={0x00};
    uint32 buf_size=0x00;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder capture yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder capture yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderCapBufSize(void)
{
    uint32 buf_size=0x00;

    buf_size=QVGA_SIZE+1024;

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder frame pool size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder frame poool
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderFramePoolSize(void)
{
    uint32 buf_size=0x00;

    buf_size=MM_MJPEG_FRAME_BUF_LEN;

    return buf_size;
}

/******************************************************************************
// Description: get DV recorder jpg fw buf
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder jpg fw buf
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderJpgFwBufSize(void)
{
    uint32 buf_size=0x00;
	DCAM_CONTEXT_T* pContext=_GetDCAMContext();

	if(pContext->record_type != DCAM_RECORD_YUV)
	{
    	buf_size=DC_Algin((40*1024),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS);
	}
	else
	{
		buf_size = 0;
	}

    return buf_size;
}

/******************************************************************************
// Description: get DV play display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvPlayDispBufSize(void)
{
    uint32 buf_size=0x00;
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;

    return buf_size;
}

/******************************************************************************
// Description: get DV play rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvPlayRotBufSize(void)
{
    uint32 buf_size=0x00;

    #if (defined(PLATFORM_SC6600L))
    buf_size=QVGA_SIZE;
    #elif (defined(PLATFORM_SC6800H))
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    buf_size=DC_Algin((lcd_info.lcd_width*lcd_info.lcd_height),MM_BUF_ALGIN,MM_BUF_ALGIN_BITS)<<0x01;
    #endif

    return buf_size;
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
/*lint -save -e737 -e574*/
LOCAL uint32 MM_CfgMemBlock(uint32 case_id, MM_CONTEXT_T *pContext)
{
    uint32 Rnt=DCAMERA_OP_SUCCESS;
    uint32 block_id=0x01;// start from the second item
    int addr_offset=MM_BUF_START_ADDR;
    int minature_addr_offset=MM_BUF_START_ADDR;
    uint32 buffer_bottom_ptr=pContext->buf_len;
    uint32 disp_buf_size=0x00;
    uint32 jpg_fw_buf_size=0x00;
    uint32 rotation_buf_size=0x00;
    uint32 exif_swap_buf_size=0x00;

    switch(case_id)
    {
        case MM_DC_PREVIEW:
        {
            //MM_DC_PREVIEW_DISP
            mm_preview_mem_cfg[block_id].block_offset=addr_offset;
            mm_preview_mem_cfg[block_id].block_size=MM_GetPreviewCapBufSize();
            mm_preview_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_preview_mem_cfg[block_id].block_size;
            block_id++;

            //MM_DC_PREVIEW_ROT
            mm_preview_mem_cfg[block_id].block_offset=addr_offset;
            mm_preview_mem_cfg[block_id].block_size=MM_GetPreviewRotBufSize();
            mm_preview_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_preview_mem_cfg[block_id].block_size;

            if((pContext->buf_len<addr_offset)/*lint !e574*/
                ||(mm_mem_cfg_tbl[case_id>>4].tbl_num<block_id))
            {
                //MM_PRINT:"DCAM: MM_Init, preview buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1098_112_2_18_1_23_25_67,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }
            break;
        }

        case MM_DC_CAPTURE:
        {            
            block_id=(uint32)MM_DC_CAPTURE_EXIF_SWAP-(uint32)MM_DC_CAPTURE;
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,MM_BUF_ALGIN_BITS);

            exif_swap_buf_size=MM_GetCaptureExifSwapBufSize();
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

            if(jpg_fw_buf_size<exif_swap_buf_size)
            {
                jpg_fw_buf_size=exif_swap_buf_size;
            }

            disp_buf_size=jpg_fw_buf_size;
            rotation_buf_size=jpg_fw_buf_size;
            exif_swap_buf_size=jpg_fw_buf_size;

            //MM_DC_CAPTURE_EXIF_SWAP
            mm_capture_mem_cfg[block_id].block_size=exif_swap_buf_size;
            mm_capture_mem_cfg[block_id].block_offset=addr_offset-mm_capture_mem_cfg[block_id].block_size;
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            MM_SUB(addr_offset, mm_capture_mem_cfg[block_id].block_size)
            block_id--;

            //MM_DC_CAPTURE_ROT
            mm_capture_mem_cfg[block_id].block_size=disp_buf_size;
            mm_capture_mem_cfg[block_id].block_offset=addr_offset;
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--;

            //MM_DC_CAPTURE_DISP
            mm_capture_mem_cfg[block_id].block_size=disp_buf_size;
            mm_capture_mem_cfg[block_id].block_offset=addr_offset;
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--;

            //MM_DC_CAPTURE_JPEG_FW
            mm_capture_mem_cfg[block_id].block_size=jpg_fw_buf_size;
            mm_capture_mem_cfg[block_id].block_offset=addr_offset;
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--;

			
            //MM_DC_CAPTURE_SWAP
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureSwapBufSize();
            mm_capture_mem_cfg[block_id].block_offset=addr_offset -mm_capture_mem_cfg[block_id].block_size;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            MM_SUB(addr_offset, (int32)mm_capture_mem_cfg[block_id].block_size)
            block_id--;

            //MM_DC_CAPTURE_QUICK_VIEW
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureQuickViewBufSize();
            mm_capture_mem_cfg[block_id].block_offset=addr_offset-mm_capture_mem_cfg[block_id].block_size;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            MM_SUB(addr_offset, (int32)mm_capture_mem_cfg[block_id].block_size)
            block_id--;

            minature_addr_offset=addr_offset;
            //MM_DC_CAPTURE_MINATURE_TEMP
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureMinAtureJpgTempBufSize();
            mm_capture_mem_cfg[block_id].block_offset=minature_addr_offset-mm_capture_mem_cfg[block_id].block_size;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //minature_addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            MM_SUB(minature_addr_offset, (int32)mm_capture_mem_cfg[block_id].block_size)
            block_id--;

            //MM_DC_CAPTURE_MINATURE_SRC
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureMinAtureJpgSrcBufSize();
            mm_capture_mem_cfg[block_id].block_offset=minature_addr_offset-mm_capture_mem_cfg[block_id].block_size;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //minature_addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            MM_SUB(minature_addr_offset, (int32)mm_capture_mem_cfg[block_id].block_size)
            block_id--; 

            //MM_DC_CAPTURE_YUV
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureYuvBufSize(); 

			MM_SUB(addr_offset, mm_capture_mem_cfg[block_id].block_size)

            mm_capture_mem_cfg[block_id].block_offset= addr_offset;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            block_id--; 

            //MM_DC_CAPTURE_JPEG
            //Attention,please! part of jpeg buffe will be shared to used by YUV
            
#if 1			
            if(MM_GetCaptureYuvBufSize()<(MM_GetCaptureMinAtureJpgSrcBufSize()+MM_GetCaptureMinAtureJpgTempBufSize()))
            {
                mm_capture_mem_cfg[block_id].block_size=minature_addr_offset-(MM_GetCaptureThumbnailBufSize()+MM_GetCaptureExifBufSize())+(MM_GetCaptureMinAtureJpgSrcBufSize()>>0x01);
            }
            else
            {
                mm_capture_mem_cfg[block_id].block_size=addr_offset-(MM_GetCaptureThumbnailBufSize()+MM_GetCaptureExifBufSize())+(MM_GetCaptureYuvBufSize()>>0x01);
            }
//			if(MM_GetImgJpgSize())
#else
			
			mm_capture_mem_cfg[block_id].block_size = MM_GetImgJpgSize();
#endif
            mm_capture_mem_cfg[block_id].block_offset=MM_GetCaptureThumbnailBufSize()+MM_GetCaptureExifBufSize();
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset=mm_capture_mem_cfg[block_id].block_offset;
            block_id--;

            //MM_DC_CAPTURE_THUMBNAIL
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureThumbnailBufSize(); 
            mm_capture_mem_cfg[block_id].block_offset=MM_GetCaptureExifBufSize();//addr_offset-mm_capture_mem_cfg[block_id].block_size;            
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;
            //addr_offset-=mm_capture_mem_cfg[block_id].block_size;
            block_id--; 

            //MM_DC_CAPTURE_EXIF
            //Attention,please! part of jpeg buffe will be shared to used by YUV
            mm_capture_mem_cfg[block_id].block_size=MM_GetCaptureExifBufSize();
            mm_capture_mem_cfg[block_id].block_offset=NULL;
            mm_capture_mem_cfg[block_id].status=MM_NOT_USED;

            if(0x00>addr_offset)
            {
                //MM_PRINT:"DCAM: MM_Init, capture buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1240_112_2_18_1_23_26_68,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }
            break;
        }
        
        case MM_DC_PANORAMA:
        {
            block_id=MM_DC_PANORAMA_JPEG_ENC-0x20;
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   

            //MM_DC_PANORAMA_JPEG_ENC (not interpolation for quick view display buf and direct view buf)
            mm_panorama_mem_cfg[block_id].block_size=DC_Algin(pContext->quick_view_len,0xff,0x08);			
            mm_panorama_mem_cfg[block_id].block_offset=addr_offset-mm_panorama_mem_cfg[block_id].block_size;
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_panorama_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_PANORAMA_QUICK_VIEW ( quick view display buf save y uv data lcd size)        	
            mm_panorama_mem_cfg[block_id].block_size=DC_Algin(pContext->quick_view_len,0xff,0x08);
            mm_panorama_mem_cfg[block_id].block_offset=addr_offset-mm_panorama_mem_cfg[block_id].block_size;            
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset -=mm_panorama_mem_cfg[block_id].block_size;
            block_id--;                

            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   
            //MM_DC_PANORAMA_PREVIEW                  
            mm_panorama_mem_cfg[block_id].block_size= DC_Algin(((pContext->lcd_width*pContext->lcd_height)<<0x01)*MM_ISP_DISP_LOOP_BUF_NUM,0xff,0x08);
            mm_panorama_mem_cfg[block_id].block_offset=addr_offset-mm_panorama_mem_cfg[block_id].block_size;            
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_panorama_mem_cfg[block_id].block_size;
            block_id--;   

            //MM_DC_PANORAMA_YUV
            mm_panorama_mem_cfg[block_id].block_size=DC_Algin(MM_PANORAMA_YUV_BUF_LEN,0xff,0x08); 
            mm_panorama_mem_cfg[block_id].block_offset=addr_offset-MM_PANORAMA_YUV_BUF_LEN;            
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_panorama_mem_cfg[block_id].block_size;
            block_id--; 

            //MM_DC_PANORAMA_STAT
            mm_panorama_mem_cfg[block_id].block_size=DC_Algin(MM_PANORAMA_STAT_BUF_LEN,0xff,0x08); 
            mm_panorama_mem_cfg[block_id].block_offset=NULL;            
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_panorama_mem_cfg[block_id].block_size;
            block_id--;  

            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   
            //MM_DC_PANORAMA_JPEG
            //Attention,please! part of jpeg buffe will be shared to used by YUV
            mm_panorama_mem_cfg[block_id].block_offset=NULL;
            mm_panorama_mem_cfg[block_id].block_size=addr_offset-(pContext->quick_view_len<<0x01)-(pContext->yuv_max_buf_len>>0x01);
            mm_panorama_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--; 

            break;
        }

        case MM_DC_FOURinONE:
        {
            block_id=MM_DC_FOURinONE_JPEG_ENC-0x30;
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   

            //MM_DC_FOURinONE_JPEG_ENC (not interpolation for quick view display)
            mm_fourinone_mem_cfg[block_id].block_size=DC_Algin(MM_JPEG_VGA_UXGA_LEN,0xff,0x08);
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset-mm_fourinone_mem_cfg[block_id].block_size;
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_CAPTURE_SWAP        	
            mm_fourinone_mem_cfg[block_id].block_size=DC_Algin(MM_JPEG_INTERPLATION_SWAP_LEN,0xff,0x08);
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset-mm_fourinone_mem_cfg[block_id].block_size;            
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--;               

            //MM_DC_FOURinONE_QUICK_VIEW (quick view buffer save yuv data)
            mm_fourinone_mem_cfg[block_id].block_size=pContext->quick_view_len;			
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset-mm_fourinone_mem_cfg[block_id].block_size;
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--;            

            //MM_DC_FOURinONE_PREVIEW
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   
            mm_fourinone_mem_cfg[block_id].block_size=DC_Algin(((pContext->lcd_width*pContext->lcd_height)<<0x01),0xff,0x08)*MM_ISP_DISP_LOOP_BUF_NUM;
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset-mm_fourinone_mem_cfg[block_id].block_size;
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--;            

            //MM_DC_FOURinONE_YUV
            mm_fourinone_mem_cfg[block_id].block_size=DC_Algin(MM_FOURinONE_YUV_BUF_LEN,0xff,0x08); 
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset-MM_FOURinONE_YUV_BUF_LEN;            
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--; 

            //MM_DC_FOURinONE_OSD
            mm_fourinone_mem_cfg[block_id].block_size=0x00; 
            mm_fourinone_mem_cfg[block_id].block_offset=addr_offset;            
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--; 

            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,0x08);   
            //MM_DC_FOURinONE_JPEG
            mm_fourinone_mem_cfg[block_id].block_size=MM_FOURinONE_JPG_BUF_LEN;
            mm_fourinone_mem_cfg[block_id].block_offset=NULL;            
            mm_fourinone_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_fourinone_mem_cfg[block_id].block_size;
            block_id--;

            break;
        }

        case MM_DC_REVIEW:
        {
            block_id=(uint32)MM_DC_REVIEW_ROT-(uint32)MM_DC_REVIEW;
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,MM_BUF_ALGIN_BITS);

            //MM_DC_REVIEW_ROT
            mm_review_mem_cfg[block_id].block_size=MM_GetReviewRotBufSize();
            mm_review_mem_cfg[block_id].block_offset=addr_offset-mm_review_mem_cfg[block_id].block_size;
            mm_review_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_review_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_REVIEW_DISP
            mm_review_mem_cfg[block_id].block_size=MM_GetReviewDispBufSize();
            mm_review_mem_cfg[block_id].block_offset=addr_offset-mm_review_mem_cfg[block_id].block_size;
            mm_review_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_review_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_REVIEW_FW
            mm_review_mem_cfg[block_id].block_size=MM_GetReviewJpgFwBufSize();
            mm_review_mem_cfg[block_id].block_offset=addr_offset;
            mm_review_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--;

            //MM_DC_REVIEW_YUV
            mm_review_mem_cfg[block_id].block_size=MM_GetReviewYuvBufSize();
            mm_review_mem_cfg[block_id].block_offset=addr_offset-mm_review_mem_cfg[block_id].block_size;
            mm_review_mem_cfg[block_id].status=MM_NOT_USED;

            if(0x00>addr_offset)
                //||(0x01>block_id))
            {
                //MM_PRINT:"DCAM: MM_Init, review buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1390_112_2_18_1_23_26_69,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }
            break;
        }
        
        case MM_DC_CONTINUESHOT:
        {
            block_id=(uint32)MM_DC_CONTINUESHOT_ROT-(uint32)MM_DC_CONTINUESHOT;
            addr_offset=DC_Algin(buffer_bottom_ptr,0x00,MM_BUF_ALGIN_BITS);

            rotation_buf_size=MM_GetContinueShotDispBufSize();
            jpg_fw_buf_size=MM_GetContinueShotJpgFwBufSize();
            
            if(jpg_fw_buf_size<rotation_buf_size)
            {
                jpg_fw_buf_size=rotation_buf_size;
            }
            else
            {
                rotation_buf_size=jpg_fw_buf_size;
            }

            //MM_DC_CONTINUESHOT_ROT
            mm_continueshot_mem_cfg[block_id].block_size=rotation_buf_size;
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset-mm_continueshot_mem_cfg[block_id].block_size;
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_CONTINUESHOT_JPEG_FW
            mm_continueshot_mem_cfg[block_id].block_size=jpg_fw_buf_size;
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset;
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            block_id--;

            //MM_DC_CAPTURE_SWAP
            mm_continueshot_mem_cfg[block_id].block_size=MM_GetCaptureSwapBufSize();
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset -mm_continueshot_mem_cfg[block_id].block_size;            
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_CONTINUESHOT_DISP
            mm_continueshot_mem_cfg[block_id].block_size=MM_GetContinueShotDispBufSize();
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset -mm_continueshot_mem_cfg[block_id].block_size;            
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;
            block_id--;
            
            //MM_DC_CONTINUESHOT_YUV
            mm_continueshot_mem_cfg[block_id].block_size=MM_GetContinueShotYuvBufSize();
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset-mm_continueshot_mem_cfg[block_id].block_size;
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;
            block_id--;

            //MM_DC_CONTINUESHOT_JPEG
            mm_continueshot_mem_cfg[block_id].block_size=addr_offset-MM_GetCaptureExifBufSize();
            mm_continueshot_mem_cfg[block_id].block_offset=addr_offset-mm_continueshot_mem_cfg[block_id].block_size;
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;
            block_id--;

            mm_continueshot_mem_cfg[block_id].block_size=MM_GetCaptureExifBufSize();
            mm_continueshot_mem_cfg[block_id].block_offset=NULL;
            mm_continueshot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset-=mm_continueshot_mem_cfg[block_id].block_size;

            if(0x00>addr_offset)
                //||(0x01>block_id))
            {
                //MM_PRINT:"DCAM: MM_Init, continue shot buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1462_112_2_18_1_23_26_70,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }
            break;
        }

        case MM_DV_SHOT:
        {
		uint32 share_buf_size = MM_GetDvRecorderCapBufSize();

		if(share_buf_size < MM_GetDvRecorderScaleBufSize())
		{
			share_buf_size = MM_GetDvRecorderScaleBufSize();
		}
			
            //MM_DV_SHOT_DISPLAY
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=MM_GetDvRecorderDispBufSize();
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;

		            //MM_DV_SHOT_ROT
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=MM_GetDvRecorderRotBufSize();//MM_GetDvRecorderScaleBufSize();  //rotation share the same buffer with scale
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;


	     //YUV share the same buffer with cap buffer
            //MM_DV_SHOT_YUV
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=share_buf_size;//MM_GetDvRecorderScaleBufSize();
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            block_id++;


            //MM_DV_SHOT_CAP
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=share_buf_size;//MM_GetDvRecorderCapBufSize();
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;

            //MM_DV_SHOT_FRAME_POOL
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=MM_GetDvRecorderFramePoolSize();
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;

            //MM_DV_SHOT_JPEG_ENC
            mm_dv_shot_mem_cfg[block_id].block_offset=DC_RIGHT_TRUNK(addr_offset,256);
            mm_dv_shot_mem_cfg[block_id].block_size=MM_GetDvRecorderJpgFwBufSize();
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;

            //MM_DV_SHOT_MUX
            //The below code is not useful
            //DV MUX Buffer allocaton have been move to DV Module
            mm_dv_shot_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_shot_mem_cfg[block_id].block_size=0;
            mm_dv_shot_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_shot_mem_cfg[block_id].block_size;
            block_id++;

            if((pContext->buf_len<addr_offset)/*lint !e574*/
                ||(mm_mem_cfg_tbl[case_id>>4].tbl_num<block_id))
            {
                //MM_PRINT:"DCAM: MM_Init, dv review buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1533_112_2_18_1_23_26_71,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }			

            break;
        }

        case MM_DV_PLAY:
        {
            //MM_DV_PLAY_DISPLAY
            mm_dv_play_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_play_mem_cfg[block_id].block_size=MM_GetDvPlayDispBufSize();
            mm_dv_play_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_play_mem_cfg[block_id].block_size;
            block_id++;

            //MM_DV_PLAY_ROT
            mm_dv_play_mem_cfg[block_id].block_offset=addr_offset;
            mm_dv_play_mem_cfg[block_id].block_size=MM_GetDvPlayRotBufSize();
            mm_dv_play_mem_cfg[block_id].status=MM_NOT_USED;
            addr_offset+=mm_dv_play_mem_cfg[block_id].block_size;

            if((pContext->buf_len<addr_offset)/*lint !e574*/
                ||(mm_mem_cfg_tbl[case_id>>4].tbl_num<block_id))
            {
                //MM_PRINT:"DCAM: MM_Init, dv review buf is not enough err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1558_112_2_18_1_23_27_72,(uint8*)"");
                Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            }			

            break;
        }
        
        default:
        {
            //MM_PRINT:"DCAM: Sorry!MM buffer MGR,case id does not exist error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1567_112_2_18_1_23_27_73,(uint8*)"");
            Rnt=DCAMERA_OP_NO_ENOUGH_MEMORY;
            break;
        }        
    }
    return Rnt;
}
/*lint -restore*/

/******************************************************************************
// Description: 	multimedia buffer init
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_Init(uint32 dc_state)
{
    uint32 nRet=SCI_SUCCESS;
    uint32 length=0x00;
    MM_BLOCK_ID case_id=MM_BLOCK_MAX;
    MM_CONTEXT_T *pMM = &g_mm_context;

    switch(dc_state)
    {
        case DC_STATE_PREVIEWING:
            case_id=MM_DC_PREVIEW;
            break;
             
        case DC_STATE_CAPTURING:
            case_id=MM_DC_CAPTURE;
            break;

        case DC_STATE_REVIEWING:
            case_id=MM_DC_REVIEW;
            break;

        case DC_STATE_PANORAMA:
            case_id=MM_DC_PANORAMA;
            break;

        case DC_STATE_FOURINONE:
            case_id=MM_DC_FOURinONE;
            break;                     

        case DC_STATE_REC:
            case_id=MM_DV_SHOT;
            break; 

        case DC_STATE_VIDEO_REVIEW:
            case_id=MM_DV_PLAY;
            break; 

        case DC_STATE_BURST_CAPTURING:
            case_id=MM_DC_CONTINUESHOT;
            break;                
            
        default :
            break;

    }

    pMM->buf_ptr=(uint8*)MM_AllocRawBuffer(dc_state, &length);
    pMM->buf_len=length;

    //MM_PRINT:"DCAM: MM_Init, all buf start addr=0x%x,buf_len=%dk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1631_112_2_18_1_23_27_74,(uint8*)"dd",pMM->buf_ptr,pMM->buf_len/1024);

    if((NULL==pMM->buf_ptr)
        ||(NULL==pMM->buf_len))
    {
        //MM_PRINT:"DCAM: MM_Init alloc dc buf fail err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1636_112_2_18_1_23_27_75,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }

    nRet=MM_CfgMemBlock(case_id, pMM);        

    return nRet;
    
}

/******************************************************************************
// Description: 	multimedia buffer exit
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_Exit(void)
{
    uint32 i,j;
    uint32 items_num;
    MM_BLOCK_MEM_T* tbl_ptr=PNULL;

    //MM module is in use
    for(i=0; i<sizeof(mm_mem_cfg_tbl)/sizeof(MM_BLOCK_MEM_TBL_T); i++)
    {
        tbl_ptr = (MM_BLOCK_MEM_T *)(mm_mem_cfg_tbl[i].tbl_ptr);
        items_num = mm_mem_cfg_tbl[i].tbl_num;
        for(j=0; j<items_num; j++)
        {
            tbl_ptr[j].block_offset = 0;
            tbl_ptr[j].block_size = 0;
            tbl_ptr[j].status = MM_NOT_USED;
        }
    }

    MM_FreeRawBuffer();

    SCI_MEMSET(&g_mm_context, 0x0, sizeof(MM_CONTEXT_T));

    return SCI_SUCCESS;
}


/******************************************************************************
// Description: 	malloc a block buffer in Multimedia unit
// Author:     		Frank.yang
// Input:      		id--the id of block to be wanted
// Output:		none
// Return:		the block buffer address
// Note:       		none
******************************************************************************/
PUBLIC void* MM_Malloc(MM_BLOCK_ID mem_id)
{
    uint32 gid, bid;
    MM_BLOCK_MEM_T* tbl_ptr=PNULL;
    MM_CONTEXT_T *pMM = &g_mm_context;

    if((NULL==pMM->buf_ptr)
        ||(0x00==(mem_id&0xf)))
    {
        //DCAM_PRINT:"DCAM: MM_Malloc, pMM->buf_ptr is null %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1696_112_2_18_1_23_27_76,(uint8*)"d",__LINE__);
        return NULL;
    }
	
    gid = mem_id>>4;
    bid = mem_id - (gid<<4);
    tbl_ptr = (MM_BLOCK_MEM_T *)(mm_mem_cfg_tbl[gid].tbl_ptr);

    if(NULL==tbl_ptr)
    {
        //DCAM_PRINT:"DCAM: MM_Malloc, tbl_ptr is null %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1706_112_2_18_1_23_27_77,(uint8*)"d",__LINE__);
        return NULL;
    }

    //MM_PRINT:"DCAM: MM_Malloc, (group,block)=(%d,%d), addr range(0x%lx ~ 0x%lx) "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1711_112_2_18_1_23_27_78,(uint8*)"dddd", gid, bid, tbl_ptr[bid].block_offset, tbl_ptr[bid].block_offset+tbl_ptr[bid].block_size);

    return (void*)(pMM->buf_ptr+ tbl_ptr[bid].block_offset);
}

/******************************************************************************
// Description: 	get the size of the block  in Multimedia unit
// Author:     		Frank.yang
// Input:      		id--the id of block to be wanted
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_GetSize(MM_BLOCK_ID mem_id)
{
    uint32 gid, bid;
    MM_BLOCK_MEM_T* tbl_ptr=PNULL;
    MM_CONTEXT_T *pMM = &g_mm_context;

    if((NULL==pMM->buf_ptr)
        ||(0x00==(mem_id&0xf)))
    {
        //DCAM_PRINT:"DCAM: MM_GetSize, pMM->buf_ptr is null %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1732_112_2_18_1_23_27_79,(uint8*)"d",__LINE__);
        return 0x00;
    }
	
    gid = mem_id>>4;
    bid = mem_id - (gid<<4);

    tbl_ptr = (MM_BLOCK_MEM_T *)(mm_mem_cfg_tbl[gid].tbl_ptr);

    if(NULL==tbl_ptr)
    {
        //DCAM_PRINT:"DCAM: MM_GetSize, tbl_ptr is null %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_BUFMGR_1743_112_2_18_1_23_27_80,(uint8*)"d",__LINE__);
        return 0x00;
    }

    return tbl_ptr[bid].block_size;
}
/*lint -restore */ 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif 


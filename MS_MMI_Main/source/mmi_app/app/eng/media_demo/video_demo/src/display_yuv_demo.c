/****************************************************************************
** File Name:      display_yuv_demo.c                                             *
** Author:         victor.xu                                                           *
** Date:           2011.02.14                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    6800H yuv422/yuv420 display demo                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011.02.14     victor.xu          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_eng_trc.h"
#include "sci_api.h"
#include "os_api.h"
#include "sensor_drv.h"
#include "jpeg_interface.h"
#include  "dc_common_6600l.h"
#include "dal_audio.h"
#include "dal_lcd.h"
#include "dc_app_6x00.h"
/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA  DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef WIN32
const uint8 g_test_yuv422_y_data[]=
{
		1, 2,
};


const uint8 g_test_yuv422_uv_data[]=
{
		1, 2,
};



const uint8 g_test_yuv420_2P_y_data[]=
{
  	1, 2,
};


const uint8 g_test_yuv420_2P_uv_data[]=
{
		1, 2,
};

const uint8 g_test_yuv420_3P_y_data[]=
{
	1, 2,
};

const uint8 g_test_yuv420_3P_u_data[]=
{
	1, 2,
};

const uint8 g_test_yuv420_3P_v_data[]=
{
	1, 2,
};

LOCAL int DC_DisplayAFrame(DISPLAY_DATA_FRAME_T* p_frame)
{
    return 1;
}

LOCAL int DC_VideoSetDisplayParam(const DC_DPLAYER_DISPLAY_PARAM_T *pst_param)	
{
    return 1;
}

#else

__align(256) 
const uint8 g_test_yuv422_y_data[]=
{
    #include ".\fruits_qvga_y_422.inc"
};

__align(256) 
const uint8 g_test_yuv422_uv_data[]=
{
    #include ".\fruits_qvga_uv_422.inc"
};


__align(256) 
const uint8 g_test_yuv420_2P_y_data[]=
{
    #include ".\ad_qvga_y_420.inc"
};

__align(256) 
const uint8 g_test_yuv420_2P_uv_data[]=
{
    #include ".\ad_qvga_uv_420.inc"
};


__align(256) 
const uint8 g_test_yuv420_3P_y_data[]=
{
    #include ".\dog_qvga_y_420.inc"
};

__align(256) 
const uint8 g_test_yuv420_3P_u_data[]=
{
    #include ".\dog_qvga_u_420.inc"
};

__align(256) 
const uint8 g_test_yuv420_3P_v_data[]=
{
    #include ".\dog_qvga_v_420.inc"
};
#endif

#define DISPLAY_PARAM_SRC_RECT_X 0
#define DISPLAY_PARAM_SRC_RECT_Y 0
#define DISPLAY_PARAM_SRC_RECT_WIDTH 320
#define DISPLAY_PARAM_SRC_RECT_HEIGHT 240

#define DISPLAY_PARAM_CROP_RECT_X 0
#define DISPLAY_PARAM_CROP_RECT_Y 0
#define DISPLAY_PARAM_CROP_RECT_WIDTH 320
#define DISPLAY_PARAM_CROP_RECT_HEIGHT 240

#define DISPLAY_PARAM_DEST_RECT_X 40
#define DISPLAY_PARAM_DEST_RECT_Y 140
#define DISPLAY_PARAM_DEST_RECT_WIDTH 160
#define DISPLAY_PARAM_DEST_RECT_HEIGHT 120


#define DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))


extern  const uint8 g_test_yuv422_y_data[];
extern  const uint8 g_test_yuv422_uv_data[];
extern  const uint8 g_test_yuv420_2P_y_data[];
extern  const uint8 g_test_yuv420_2P_uv_data[];
extern  const uint8 g_test_yuv420_3P_y_data[];
extern  const uint8 g_test_yuv420_3P_u_data[];
extern  const uint8 g_test_yuv420_3P_v_data[];


DISPLAY_DATA_FRAME_T tDemoDisplayFrame;
DC_DPLAYER_DISPLAY_PARAM_T tDemoDisplayParam;

uint32 y_buf_addr = NULL;
uint32 u_buf_addr = NULL;
uint32 v_buf_addr = NULL;

PUBLIC void Demo_Set_Parameters(void)
{
    tDemoDisplayParam.video_buf_addr = NULL;
    tDemoDisplayParam.video_buf_len = 0;
    tDemoDisplayParam.i_lcd = 0;
    tDemoDisplayParam.st_disp_rect.x = DISPLAY_PARAM_DEST_RECT_X;
    tDemoDisplayParam.st_disp_rect.y = DISPLAY_PARAM_DEST_RECT_Y;
    tDemoDisplayParam.st_disp_rect.w = DISPLAY_PARAM_DEST_RECT_WIDTH;
    tDemoDisplayParam.st_disp_rect.h = DISPLAY_PARAM_DEST_RECT_HEIGHT;
    tDemoDisplayParam.target_rect.x = DISPLAY_PARAM_DEST_RECT_X;
    tDemoDisplayParam.target_rect.y = DISPLAY_PARAM_DEST_RECT_Y;
    tDemoDisplayParam.target_rect.w = DISPLAY_PARAM_DEST_RECT_WIDTH;
    tDemoDisplayParam.target_rect.h = DISPLAY_PARAM_DEST_RECT_HEIGHT;
    tDemoDisplayParam.st_trim_rect.x = DISPLAY_PARAM_CROP_RECT_X;
    tDemoDisplayParam.st_trim_rect.y = DISPLAY_PARAM_CROP_RECT_Y;
    tDemoDisplayParam.st_trim_rect.w = DISPLAY_PARAM_CROP_RECT_WIDTH;
    tDemoDisplayParam.st_trim_rect.h = DISPLAY_PARAM_CROP_RECT_HEIGHT;    
    tDemoDisplayParam.src_width = DISPLAY_PARAM_SRC_RECT_WIDTH;
    tDemoDisplayParam.src_height = DISPLAY_PARAM_SRC_RECT_HEIGHT;
    tDemoDisplayParam.disp_mode = DCAMERA_DISP_CUSTOMIZE;
    tDemoDisplayParam.rotation = DCAMERA_ROTATION_0;
    tDemoDisplayParam.review_callback = NULL;
}

PUBLIC void Demo_Set_DisplayFrame_yuv422_2p(void)
{
    tDemoDisplayFrame.yuv_type = JINF_FORMAT_YUV422;
    tDemoDisplayFrame.yuv_plane = YUV_PLANE_2;
    tDemoDisplayFrame.y_addr = (uint8*)y_buf_addr;
    tDemoDisplayFrame.y_data_len = (DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT);
    tDemoDisplayFrame.uv_addr = (uint8*)u_buf_addr;
    tDemoDisplayFrame.uv_data_len = DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT;
    tDemoDisplayFrame.uv1_addr = NULL;
    tDemoDisplayFrame.uv1_data_len = 0;
}

PUBLIC void Demo_Set_DisplayFrame_yuv420_2p(void)
{
    tDemoDisplayFrame.yuv_type = JINF_FORMAT_YUV420;
    tDemoDisplayFrame.yuv_plane = YUV_PLANE_2;
    tDemoDisplayFrame.y_addr = (uint8*)y_buf_addr;
    tDemoDisplayFrame.y_data_len = (DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT);
    tDemoDisplayFrame.uv_addr = (uint8*)u_buf_addr;
    tDemoDisplayFrame.uv_data_len = DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/2;
    tDemoDisplayFrame.uv1_addr = NULL;
    tDemoDisplayFrame.uv1_data_len = 0;
}

PUBLIC void Demo_Set_DisplayFrame_yuv420_3p(void)
{
    tDemoDisplayFrame.yuv_type = JINF_FORMAT_YUV420;
    tDemoDisplayFrame.yuv_plane = YUV_PLANE_3;
    tDemoDisplayFrame.y_addr = (uint8*)y_buf_addr;
    tDemoDisplayFrame.y_data_len = (DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT);
    tDemoDisplayFrame.uv_addr = (uint8*)u_buf_addr;
    tDemoDisplayFrame.uv_data_len = DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/4;
    tDemoDisplayFrame.uv1_addr = (uint8*)v_buf_addr;
    tDemoDisplayFrame.uv1_data_len = DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/4;
}

extern PUBLIC ERR_LCD_E LCD_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num);
extern PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num);


/*****************************************************************************/
//  Description : API of  video start demo 
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/

PUBLIC void Video_Demo_yuv_Start(uint8 demo_type)
{  
    uint32 demo_tick_cnt=0;

    uint32 y_alloc_addr = NULL;
    uint32 u_alloc_addr = NULL;
    uint32 v_alloc_addr = NULL;
    int ret_temp=0;
    
    //SCI_TRACE_LOW:"demo: Video_Demo_yuv_Start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_233_112_2_18_2_18_27_341,(uint8*)"");

    Demo_Set_Parameters();
    
    y_alloc_addr = (uint32)SCI_ALLOCA(DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT*2+1024);
    if(NULL==y_alloc_addr)
    {
        //SCI_TRACE_LOW:"demo: alloc err y_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_240_112_2_18_2_18_27_342,(uint8*)"d",y_alloc_addr);
    }
    else
    {
        //SCI_TRACE_LOW:"demo: alloc ok y_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_244_112_2_18_2_18_27_343,(uint8*)"d",y_alloc_addr);
    }

    u_alloc_addr = (uint32)SCI_ALLOCA(DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT*2+1024);
    if(NULL==u_alloc_addr)
    {
        //SCI_TRACE_LOW:"demo: alloc err u_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_250_112_2_18_2_18_27_344,(uint8*)"d",u_alloc_addr);
    }
    else
    {
        //SCI_TRACE_LOW:"demo: alloc ok u_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_254_112_2_18_2_18_27_345,(uint8*)"d",u_alloc_addr);
    }

    v_alloc_addr = (uint32)SCI_ALLOCA(DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT*2+1024);
    if(NULL==v_buf_addr)
    {
        //SCI_TRACE_LOW:"demo: alloc err v_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_260_112_2_18_2_18_27_346,(uint8*)"d",v_alloc_addr);
    }
    else
    {
        //SCI_TRACE_LOW:"demo: alloc ok v_buf_addr[0x%x]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_264_112_2_18_2_18_27_347,(uint8*)"d",v_alloc_addr);
    }
    
    y_buf_addr = DC_RIGHT_TRUNK(y_alloc_addr, 256);
    u_buf_addr = DC_RIGHT_TRUNK(u_alloc_addr, 256);
    v_buf_addr = DC_RIGHT_TRUNK(v_alloc_addr, 256);

    if(0==demo_type)
    {
        //demo yuv422_2p
        SCI_MEMCPY((uint8*)y_buf_addr,&g_test_yuv422_y_data[0],(DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT));
        SCI_MEMCPY((uint8*)u_buf_addr,&g_test_yuv422_uv_data[0],(DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT));
        Demo_Set_DisplayFrame_yuv422_2p();    
    
    }
    else if(1==demo_type)
    {
        //demo yuv420_2p
        SCI_MEMCPY((uint8*)y_buf_addr,&g_test_yuv420_2P_y_data[0],(DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT));
        SCI_MEMCPY((uint8*)u_buf_addr,&g_test_yuv420_2P_uv_data[0],DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/2);
        Demo_Set_DisplayFrame_yuv420_2p();
    }
    else if(2==demo_type)
    {
        //demo yuv420_3p
        SCI_MEMCPY((uint8*)y_buf_addr,&g_test_yuv420_3P_y_data[0],(DISPLAY_PARAM_SRC_RECT_WIDTH)*(DISPLAY_PARAM_SRC_RECT_HEIGHT));
        SCI_MEMCPY((uint8*)u_buf_addr,&g_test_yuv420_3P_u_data[0],DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/4);
        SCI_MEMCPY((uint8*)v_buf_addr,&g_test_yuv420_3P_v_data[0],DISPLAY_PARAM_SRC_RECT_WIDTH*DISPLAY_PARAM_SRC_RECT_HEIGHT/4);
        Demo_Set_DisplayFrame_yuv420_3p();
    }
      
    demo_tick_cnt = SCI_GetTickCount();


    Demo_Set_Parameters();

    if (SCI_SUCCESS != DC_VideoSetDisplayParam(&tDemoDisplayParam))
    {
        //SCI_TRACE_LOW:"demo:update_display_param,DC_VideoSetDisplayParam fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_302_112_2_18_2_18_28_348,(uint8*)"");
        return ;
    }
    ret_temp=LCD_DisableBlock(MAIN_LCD_ID, 0x02);
    
    if (SCI_SUCCESS != DC_DisplayAFrame(&tDemoDisplayFrame))
    {
        //SCI_TRACE_LOW:"demo:, DC_DisplayAFrame fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_309_112_2_18_2_18_28_349,(uint8*)"");
        return ;
    }
    ret_temp=LCD_EnableBlock(MAIN_LCD_ID, 0x02);

    //SCI_TRACE_LOW:"demo:cost time [%d]ms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DISPLAY_YUV_DEMO_314_112_2_18_2_18_28_350,(uint8*)"d",SCI_GetTickCount()-demo_tick_cnt);

    SCI_Sleep(10);

    if(NULL!=y_alloc_addr)
    {
        SCI_Free((void*)y_alloc_addr);
        y_alloc_addr = NULL;
    }
    if(NULL!=u_alloc_addr)
    {
        SCI_Free((void*)u_alloc_addr);
        u_alloc_addr = NULL;
    }
    if(NULL!=v_alloc_addr)
    {
        SCI_Free((void*)v_alloc_addr);
        v_alloc_addr = NULL;
    }
}



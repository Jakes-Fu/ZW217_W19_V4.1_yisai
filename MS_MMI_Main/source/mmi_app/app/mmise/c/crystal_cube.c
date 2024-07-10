/****************************************************************************
** File Name:      crystal_cube.c                                                	                    *
** Author:           jila                                                      		            *
** Date:           2011.03.15                                             	                            *
** Description:    This file is used to describe crystal cube effect.
****************************************************************************
**                         Important Edit History                         	 	*
** ------------------------------------------------------------------------*
** DATE           	        NAME             DESCRIPTION                       	*
** 2011.03.15        	jila	
** 
****************************************************************************/
#include "mmi_app_mmise_trc.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "graphics_3d_rotate.h"
#include "mmk_app.h"
#include "crystal_cube.h"
#include "mmise.h"
#include "mmidisplay_data.h "//jinx


/**---------------------------------------------------------------------------*
 **                         	Compiler Flag                                     	*
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         	Macro	Definition                                  *
 **---------------------------------------------------------------------------*/
//#define _DEBUG_QUEUE_NUM



#define CUBEPAGEMENU_ENTERANIM_START_GEOMETRY_Z           224
#define CUBEPAGEMENU_ENTERANIM_END_GEOMETRY_Z         3200
#define CUBEPAGEMENU_ENTERANIM_START_ANGLE                        220
#define CUBEPAGEMENU_ENTERANIM_GEOMETRY_TOTAL_ANGLE   (CUBEPAGEMENU_ENTERANIM_START_ANGLE + 900*MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM)//jinx_cry_stal
#define CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME                  30
#ifdef WIN32
#define CUBEPAGEMENU_TP_UP_ACCELERATION                     20 //估算值
#else
#define CUBEPAGEMENU_TP_UP_ACCELERATION                     10 //估算值
#endif
#define CUBEPAGEMENU_TP_UP_BACK_TOTAL_FRAME        15

#ifdef WIN32
#define CUBEPAGE_FLING_MAX_VELOCITY 800
#else
#define CUBEPAGE_FLING_MAX_VELOCITY 400
#endif
#define CUBEPAGE_FLING_MIN_VELOCITY  20

#ifdef WIN32
#define CUBEPAGE_VELOCITY_RATIO 400
#else
#define CUBEPAGE_VELOCITY_RATIO 200
#endif

#define CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM (5)
#if defined MAINLCD_SIZE_240X320//jinx
#define CUBE_AXIS_X	(MMI_CRYSTALCUBE_REGION_WIDTH * 12 / 16)
#define CUBE_AXIS_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 16 / 64)
#define CUBE_AXIS_Z	(MMI_CRYSTALCUBE_REGION_HEIGHT)

#define CUBE_EYE_X	(MMI_CRYSTALCUBE_REGION_WIDTH /2)
#define CUBE_EYE_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 12 / 16)
#define CUBE_EYE_Z	(-MMI_CRYSTALCUBE_REGION_HEIGHT*4)
#elif defined MAINLCD_SIZE_240X400
#define CUBE_AXIS_X	(MMI_CRYSTALCUBE_REGION_WIDTH * 12/ 16)
#define CUBE_AXIS_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 16 / 64)
#define CUBE_AXIS_Z	(MMI_CRYSTALCUBE_REGION_HEIGHT)

#define CUBE_EYE_X	(MMI_CRYSTALCUBE_REGION_WIDTH /2)
#define CUBE_EYE_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 12/ 16)
#define CUBE_EYE_Z	(-MMI_CRYSTALCUBE_REGION_HEIGHT*4)
#elif defined MAINLCD_SIZE_320X480
#define CUBE_AXIS_X	(MMI_CRYSTALCUBE_REGION_WIDTH * 44/ 64)
#define CUBE_AXIS_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 16 / 64)
#define CUBE_AXIS_Z	(MMI_CRYSTALCUBE_REGION_HEIGHT)

#define CUBE_EYE_X	(MMI_CRYSTALCUBE_REGION_WIDTH /2)
#define CUBE_EYE_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 12 / 16)
#define CUBE_EYE_Z	(-MMI_CRYSTALCUBE_REGION_HEIGHT*4)

#elif defined MAINLCD_SIZE_176X220
#define CUBE_AXIS_X	(MMI_CRYSTALCUBE_REGION_WIDTH * 44/ 64)
#define CUBE_AXIS_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 16 / 64)
#define CUBE_AXIS_Z	(MMI_CRYSTALCUBE_REGION_HEIGHT)

#define CUBE_EYE_X	(MMI_CRYSTALCUBE_REGION_WIDTH /2)
#define CUBE_EYE_Y	(MMI_CRYSTALCUBE_REGION_HEIGHT * 12 / 16)
#define CUBE_EYE_Z	(-MMI_CRYSTALCUBE_REGION_HEIGHT*4)
#endif
#define CUBE_WITH_SHADOW_HEIGHT_PARA	(5)				//加倒影总长度占原图大小比例 = CUBE_WITH_SHADOW_HEIGHT_PARA : CUBE_BASE_SRC_PARA
#define CUBE_NO_SHADOW_AREA_PARA	(3)					//原图中不参与倒影区域占原图大小比例 = CUBE_NO_SHADOW_AREA_PARA : CUBE_BASE_SRC_PARA
#define CUBE_BASE_SRC_PARA	(4)							

#define CUBE_MIN_ANGLE	(60)

#define SendNotify(id)  MMISE_SendMsgToMMI(id,PNULL,0)
#define SendNotifyWithParam(id,param,size)  MMISE_SendMsgToMMI(id,param,size)


//#define S3D_DATA_ARGB888 0

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
 

typedef struct
{
    BOOLEAN is_init;
    BOOLEAN is_forever;
    uint16 width;
    uint16 height;
    int16 rotate_angle;
    void *reflection_buffer_ptr;
    //uint8 *out_buffer_raw_ptr;
    uint8 *out_buffer_ptr;
    S3D_ROTATION_IMG_T s3d_rect[MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM];//jinx_crystal
    S3D_ROTATION_CUBE_SRC_INFO_T s3d_cube;
    S3D_ROTATION_INOUT_IMG_INFO_T s3d_window;
    S3D_ROTATION_GEOMETRY_T s3d_geometry;
    DUALBUFF_INFO_T* dualbuff_info_ptr;
}MMICRYSTALCUBE_PARAM_T;

typedef struct
{
    uint8 *out_buffer_raw_ptr;
    uint8 *out_buffer_ptr;
}MMICRYSTALCUBE_BUFFER_PARAM_T;

typedef struct
{
    BOOLEAN is_enter;
    int32 total_frame;
    int32 cur_frame;
    int16 start_angle;
    int16 end_angle;
    int32 start_z;
    int32 end_z;
}MMICRYSTALCUBE_ANIM_PARAM_T;

typedef struct
{
    BOOLEAN is_rotate;
    BOOLEAN is_tp_down;
    int16 start_xpos;
    int16 start_ypos;
    int16 xpos;
    int16 ypos;
    int16 pre_xpos;
    int16 pre_ypos;
    int16 start_angle;
    int16 cur_angle;
    int16 total_frame;
    int16 cur_frame;
    float fling_velocity;
}MMICRYSTALCUBE_TP_PARAM_T;

typedef struct  
{
    int32  x;
    int32  y;
    uint32 time;

}CRYSTALCUBE_VELOCITY_TRACKER_ITEM_T;

typedef struct  
{
    CRYSTALCUBE_VELOCITY_TRACKER_ITEM_T  item[CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM];

    uint8 item_num;

}CRYSTALCUBE_VELOCITY_TRACKER_T;

/**---------------------------------------------------------------------------*
 **                         		Constant Variables                                	*
 **---------------------------------------------------------------------------*/
 //一页中ICON的坐标
 #if defined MAINLCD_SIZE_176X220
LOCAL CONST GUI_POINT_T g_mmicrystalcube_iconitem_point_array[9] =
{
    {38, 93}, {83, 91}, {124, 90}, 
    {38, 142}, {83, 141}, {124, 140}, 
    {38, 190}, {83, 189}, {124, 188}, 
};
#elif defined MAINLCD_SIZE_240X320
LOCAL CONST GUI_POINT_T g_mmicrystalcube_iconitem_point_array[16] =
{
    {38, 93}, {83, 91}, {124, 90}, {169, 89},
    {38, 142}, {83, 141}, {124, 140}, {169, 139},
    {38, 190}, {83, 189}, {124, 188}, {169, 187},
    {38, 234}, {83, 235}, {124, 236}, {169, 237}
};
#elif defined MAINLCD_SIZE_240X400
LOCAL CONST GUI_POINT_T g_mmicrystalcube_iconitem_point_array[20] =
{
    {37, 135}, {78, 134}, {120, 133}, {160, 132},
    {37, 178}, {78, 177}, {120, 176}, {160, 175},
    {37, 213}, {78, 212}, {120, 211}, {160, 210},
    {37, 252}, {78, 253}, {120, 254}, {160, 255},
    {37, 290}, {78, 291}, {120, 292}, {160, 293}
};
#elif defined MAINLCD_SIZE_320X480 //5X5
LOCAL CONST GUI_POINT_T g_mmicrystalcube_iconitem_point_array[25] =
{
    {45, 152}, {90, 151}, {137, 150}, {184, 149},{228,148},
    {45, 203}, {90, 202}, {137, 201}, {184, 200},{228,199},
    {45, 250}, {90, 249}, {137, 248}, {184, 247},{228,246},
    {45, 301}, {90, 300}, {137, 299}, {184, 298},{228,297},
    {45, 350}, {90, 351}, {137, 352}, {184, 353},{228,354}
};
#endif
/**---------------------------------------------------------------------------*
**                 			Extern Functions                                      	*
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                    		Local Functions Declare                                         	*
**---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E CCubeHandleProcessMsg(
        PWND app_ptr,
        uint16 msg_id,
        DPARAM param
        );

LOCAL BOOLEAN CCUBE_ComputeCurrentVelocity(
                                          float* vx_ptr, //x方向速度, in/out
                                          float* vy_ptr  //y方向速度, in/out
                                          );
        
LOCAL void oper_get_next_anim_frame(void);
LOCAL void oper_get_next_tpscrollanim_frame(void);
LOCAL void do_operation(uint32 oper_type,void* param_ptr,uint32 param_size);
LOCAL void DualBuff_Exit(void);
LOCAL void SetFourCalcPage(MMICRYSTALCUBE_PARAM_T *param);
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BLOCK_ID g_cubepage_task_id = SCI_INVALID_BLOCK_ID;                    //电子书服务器任务ID
LOCAL MMICRYSTALCUBE_PARAM_T g_crystalcube_param = {0};
LOCAL MMICRYSTALCUBE_ANIM_PARAM_T g_crystalcube_anim_param = {0};
LOCAL MMICRYSTALCUBE_TP_PARAM_T g_crystalcube_tp_param = {0};
MMI_APPLICATION_T g_mmicrystalcube_app = {CCubeHandleProcessMsg, CT_CONTROL, PNULL};

LOCAL CRYSTALCUBE_VELOCITY_TRACKER_T s_crystalcube_velocity_tracker = {0};

LOCAL BOOLEAN s_release_is_done = FALSE;

LOCAL MMI_RESULT_E CCubeHandleProcessMsg(
        PWND app_ptr,
        uint16 msg_id,
        DPARAM param
        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MAINMENUWIN_MENU_CTRL_ID;
    MmiOtherTaskMsgS* sig_ptr = (MmiOtherTaskMsgS*)param;
    
    if (msg_id > MMISE_SIG_BEGIN && msg_id < MMISE_SIG_END)
    {
        //SCI_TRACE_LOW("CCubeHandleProcessMsg msg_id = %x", msg_id);

        MMK_PostMsg(ctrl_id, sig_ptr->msg_id, sig_ptr->param_ptr, sig_ptr->size_of_param);

        if( sig_ptr->size_of_param )
        {
            SCI_FREE( sig_ptr->param_ptr );
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 立方体初始化
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Init(MMICRYSTALCUBE_INIT_PARAM_T* init_param_ptr)
{

    MMISE_CalculateTaskCreate();
    do_operation(MMICRYSTALCUBE_MSG_PARAMINIT_REQ,(void*)init_param_ptr,sizeof(MMICRYSTALCUBE_INIT_PARAM_T));
}


/*****************************************************************************/
//  Description : 立方体参数reset
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_ResetParam(MMICRYSTALCUBE_INIT_PARAM_T* init_param_ptr)
{
    MMISE_CalculateTaskCreate();
    do_operation(MMICRYSTALCUBE_MSG_PARAM_RESET_REQ,(void*)init_param_ptr,sizeof(MMICRYSTALCUBE_INIT_PARAM_T));
}


/*****************************************************************************/
//  Description : 销毁立方体
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Destroy()
{
    do_operation(MMICRYSTALCUBE_MSG_EXIT_REQ,PNULL,0); 
    DualBuff_Exit();
    MMISE_CalculateTaskDestroy();
}


/*****************************************************************************/
//  Description : 启动进入动画
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_StartAnim(BOOLEAN is_enter)
{  
    do_operation(MMICRYSTALCUBE_MSG_ANIM_START_REQ,(void*)&is_enter,sizeof(BOOLEAN));

    
}


/*****************************************************************************/
//  Description : 请求动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:  下一帧准备好 后通过MMICRYSTALCUBE_MSG_ANIM_FRAME_IND 消息通知
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextAnimFrame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr*/ )
{

    do_operation(MMICRYSTALCUBE_MSG_ANIM_FRAME_CNF,PNULL,0);
}


/*****************************************************************************/
//  Description : 更新倒影
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_UpdateReflection(uint16 page_index)
{ 
    do_operation(MMICRYSTALCUBE_MSG_REFLECTION_UPDATE_REQ,(void*)&page_index,sizeof(uint16));
}


/*****************************************************************************/
//  Description : 请求显示当前的静态图像
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Display()
{

    do_operation(MMICRYSTALCUBE_MSG_DISPLAY_REQ,PNULL,0);
}



/*****************************************************************************/
//  Description :旋转
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Rotate(int16 angle)
{
    do_operation(MMICRYSTALCUBE_MSG_ROTATE_REQ,(void*)&angle,sizeof(int16));
}


/*****************************************************************************/
//  Description :请求旋转动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:  下一帧准备好 后通过MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND 消息通知
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextRotateFrame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr*/ )
{

    do_operation(MMICRYSTALCUBE_MSG_ROTATE_FRAME_CNF,PNULL,0);    
}


/*****************************************************************************/
//  Description : 触摸屏移动
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_TpScroll(MMICRYSTALCUBE_TP_REQ_T* tp_param_ptr)
{

    
    do_operation(MMICRYSTALCUBE_MSG_TP_SCROOL_REQ,(void*)tp_param_ptr,sizeof(MMICRYSTALCUBE_TP_REQ_T));
}


/*****************************************************************************/
//  Description : 请求触摸屏滑动动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextTpScrollAnimFrame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr */)
{

    do_operation(MMICRYSTALCUBE_MSG_SRCOLLANIM_CNF,PNULL,0);   
}



/*****************************************************************************/
//  Description : 触摸屏点触
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_TpCmd(int16 xpos,int16 ypos)
{
    MMICRYSTALCUBE_TP_REQ_T tp_param;

    SCI_MEMSET(&tp_param,0,sizeof(MMICRYSTALCUBE_TP_REQ_T));

    tp_param.xpos = xpos;
    tp_param.ypos = ypos;


    do_operation(MMICRYSTALCUBE_MSG_TP_CMD_REQ,(void*)&tp_param,sizeof(MMICRYSTALCUBE_TP_REQ_T));    
}


/*****************************************************************************/
//  Description : 释放参数
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_ReleaseParam()
{   
    DualBuff_Exit();
    do_operation(MMICRYSTALCUBE_MSG_PARAMRELEASE_REQ,PNULL,0); 
    
    
    /*要等待calc task处理完PARAMRELEASE_REQ之后才返回*/
    s_release_is_done = FALSE;
    while (!s_release_is_done)
    {
        SCI_Sleep(10);
    }
    s_release_is_done = TRUE;    
    MMISE_CalculateTaskDestroy();
}
/*****************************************************************************/
//  Description : RESET OUTPUT BUF
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void reset_output_buffer(void)
{
    if(g_crystalcube_param.is_init)
    {
        SCI_MEMSET(g_crystalcube_param.out_buffer_ptr, 0x00,
            g_crystalcube_param.s3d_window.size.w * g_crystalcube_param.s3d_window.size.h * sizeof(int32));
    }
}

/*****************************************************************************/
//  Description : 初始化参数
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void oper_init_param(MMICRYSTALCUBE_INIT_PARAM_T* input_param_ptr,BOOLEAN is_reset)
{
    uint16 page_index = 0;
    uint16 x = 0;
    uint16 y = 0;
    uint32 color = 0;

    g_crystalcube_param.dualbuff_info_ptr = input_param_ptr->dbuff_info_ptr;

//jinx_crystal   
#if 0
    g_crystalcube_param.s3d_rect[0].src_ptr = input_param_ptr->src_buffer_ptr[0];
    g_crystalcube_param.s3d_rect[0].src_type = S3D_DATA_ARGB888;

    g_crystalcube_param.s3d_rect[1].src_ptr = input_param_ptr->src_buffer_ptr[1];
    g_crystalcube_param.s3d_rect[1].src_type = S3D_DATA_ARGB888;

    g_crystalcube_param.s3d_rect[2].src_ptr = input_param_ptr->src_buffer_ptr[2];
    g_crystalcube_param.s3d_rect[2].src_type = S3D_DATA_ARGB888;

    g_crystalcube_param.s3d_rect[3].src_ptr = input_param_ptr->src_buffer_ptr[3];
    g_crystalcube_param.s3d_rect[3].src_type = S3D_DATA_ARGB888;
#endif
//读取没页的buf
    for (page_index = 0; page_index < MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM; page_index ++)
    {
        g_crystalcube_param.s3d_rect[page_index].src_ptr = input_param_ptr->src_buffer_ptr[page_index];
        g_crystalcube_param.s3d_rect[page_index].src_type = S3D_DATA_ARGB888;
    }

    //if(signal_rec_ptr->SignalCode == MMICRYSTALCUBE_MSG_PARAMINIT_REQ)
    if (!is_reset)
    {
        //初始化参数
        g_crystalcube_param.s3d_cube.alpha_set = S3D_ALPHA_6_2;
        //g_crystalcube_param.s3d_cube.s3d_img_ptr = &g_crystalcube_param.s3d_rect;

        g_crystalcube_param.s3d_geometry.is_axis_use_default = SCI_FALSE;
        g_crystalcube_param.s3d_geometry.is_eye_use_default = SCI_FALSE;

        g_crystalcube_param.width = input_param_ptr->layer_width;
        g_crystalcube_param.height = input_param_ptr->layer_height;
        //设定旋转轴为Y
        g_crystalcube_param.s3d_geometry.axis.axis = 1;


        //设定轴坐标
        g_crystalcube_param.s3d_geometry.axis.x = CUBE_AXIS_X;
        g_crystalcube_param.s3d_geometry.axis.y = CUBE_AXIS_Y;
        g_crystalcube_param.s3d_geometry.axis.z = CUBE_AXIS_Z;
        //设定眼的位置
        g_crystalcube_param.s3d_geometry.eye.x = CUBE_EYE_X;
        g_crystalcube_param.s3d_geometry.eye.y = CUBE_EYE_Y;
        g_crystalcube_param.s3d_geometry.eye.z = CUBE_EYE_Z;


        g_crystalcube_param.s3d_geometry.angle = CUBEPAGEMENU_ENTERANIM_START_ANGLE;

        g_crystalcube_param.s3d_geometry.img_size.x = 0;
        g_crystalcube_param.s3d_geometry.img_size.y = 0;
        g_crystalcube_param.s3d_geometry.img_size.w = input_param_ptr->layer_width;
        g_crystalcube_param.s3d_geometry.img_size.h = input_param_ptr->layer_height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA;

        g_crystalcube_param.s3d_window.size.w = input_param_ptr->window_width;
        g_crystalcube_param.s3d_window.size.h = input_param_ptr->window_height;

        SetFourCalcPage(&g_crystalcube_param);
    }
    //初始化特效buf
    if (g_crystalcube_param.reflection_buffer_ptr == PNULL)
    {
        g_crystalcube_param.reflection_buffer_ptr = SCI_ALLOC_APP(g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));
    }
    SCI_MEMSET(g_crystalcube_param.reflection_buffer_ptr, 0x00, g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));

    /*
    if (g_crystalcube_param.out_buffer_raw_ptr)
    {
        SCI_FREE(g_crystalcube_param.out_buffer_raw_ptr);

        g_crystalcube_param.out_buffer_raw_ptr = PNULL;
        g_crystalcube_param.out_buffer_ptr = PNULL;
    }
    
    g_crystalcube_param.out_buffer_raw_ptr = input_param_ptr->out_buffer_raw_ptr;
    g_crystalcube_param.out_buffer_ptr = input_param_ptr->out_buffer_ptr;
    

    SCI_MEMSET(g_crystalcube_param.out_buffer_ptr, 0x00,input_param_ptr->window_width * input_param_ptr->window_height * 4);
    */
    g_crystalcube_param.out_buffer_ptr = PNULL;

    g_crystalcube_param.s3d_window.color_ptr = g_crystalcube_param.out_buffer_ptr;
    
    g_crystalcube_param.is_forever = FALSE;

    g_crystalcube_param.is_init = TRUE;
//jinx_crystal
//计算page ,以及倒影区
    for (page_index = 0; page_index < MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM; page_index ++)
    {
        //SCI_MEMSET(g_crystalcube_param.reflection_buffer_ptr, 0x00, g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));
        for(y = 0; y < g_crystalcube_param.height * CUBE_NO_SHADOW_AREA_PARA / CUBE_BASE_SRC_PARA; y ++)
        {
            for(x = 0; x < g_crystalcube_param.width; x ++)
            {
                ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + y] = ((uint32 *)g_crystalcube_param.s3d_rect[page_index].src_ptr)[y * g_crystalcube_param.width + x];
            }
        }

        for(; y < g_crystalcube_param.height; y ++)
        {
            for(x = 0; x < g_crystalcube_param.width; x ++)
            {
                color = ((uint32 *)g_crystalcube_param.s3d_rect[page_index].src_ptr)[y * g_crystalcube_param.width + x];
                
                ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + y] = color;
                ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + g_crystalcube_param.height * 2 - y -1] =
                    (color & 0x00FFFFFF) | ((((color & 0xFF000000) >> 6) * (y - g_crystalcube_param.height * CUBE_NO_SHADOW_AREA_PARA / CUBE_BASE_SRC_PARA)) & 0xFF000000);;
            }
        }
        //更新page
        SCI_MEMCPY(g_crystalcube_param.s3d_rect[page_index].src_ptr, g_crystalcube_param.reflection_buffer_ptr, g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));
    }

    //如果在静止，且不是开场动画，且是reset 则开始display
    if ((is_reset)
        &&(g_crystalcube_anim_param.cur_frame == CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME)
        )
    {
        SendNotifyWithParam(MMICRYSTALCUBE_MSG_PARAM_RESET_CNF,(void*)&g_crystalcube_tp_param.is_rotate,sizeof(BOOLEAN));
    }
    //如果不是reset，或者是reset且正在进行开场动画则继续开场动画
    else
    {
        SendNotify(MMICRYSTALCUBE_MSG_PARAMINIT_CNF);
    }    
            
}
/*****************************************************************************/
//  Description : 设置输出BUF
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN oper_set_output_buffer(void)
{
    if (PNULL == (g_crystalcube_param.out_buffer_ptr = DualBuff_GetWriteBuf(g_crystalcube_param.dualbuff_info_ptr)))
        return FALSE;

    g_crystalcube_param.s3d_window.color_ptr = g_crystalcube_param.out_buffer_ptr;    

    return TRUE;
}
/*****************************************************************************/
//  Description : oper_start_anim
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void oper_start_anim(BOOLEAN is_enter)
{
    if (!g_crystalcube_param.is_init) return;
    if(g_crystalcube_anim_param.cur_frame != 0)
    {
        g_crystalcube_anim_param.cur_frame -= 1;
    }
    else
    {
    g_crystalcube_anim_param.is_enter = is_enter;


        g_crystalcube_anim_param.cur_frame = 0;
        g_crystalcube_anim_param.total_frame = CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME;
        g_crystalcube_anim_param.start_angle = CUBEPAGEMENU_ENTERANIM_START_ANGLE;
        g_crystalcube_anim_param.end_angle = CUBEPAGEMENU_ENTERANIM_GEOMETRY_TOTAL_ANGLE; //g_crystalcube_param.rotate_angle;// 保留
        //START_GEOMETRY_Z 是开始从远处运动的初始值
        if (g_crystalcube_anim_param.is_enter)
        {
            g_crystalcube_anim_param.start_z = CUBEPAGEMENU_ENTERANIM_START_GEOMETRY_Z;
            g_crystalcube_anim_param.end_z = CUBEPAGEMENU_ENTERANIM_END_GEOMETRY_Z;
        }
        else
        {
            g_crystalcube_anim_param.start_z = CUBEPAGEMENU_ENTERANIM_END_GEOMETRY_Z;
            g_crystalcube_anim_param.end_z = CUBEPAGEMENU_ENTERANIM_START_GEOMETRY_Z;
        }

    }
    oper_get_next_anim_frame();
           
}
/*****************************************************************************/
//  Description :获取下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void oper_get_next_anim_frame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr*/)
{
    int32 damp_z_offset = 0;
    int32 damp_angle_offset = 0;

    if (!g_crystalcube_param.is_init) 
    {
        return;
    }

    if(g_crystalcube_anim_param.cur_frame == g_crystalcube_anim_param.total_frame)
    { 
        SendNotify(MMICRYSTALCUBE_MSG_ANIM_END_IND);
        return;
    }

   
    if (!oper_set_output_buffer(/*out_buffer_raw_ptr, out_buffer_ptr*/))
        return;

    //SCI_ASSERT(g_crystalcube_param.is_init); /*assert verified*/

    reset_output_buffer();

    if (g_crystalcube_anim_param.cur_frame <= CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME / 2)
    {
        damp_z_offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,
            g_crystalcube_anim_param.end_z -g_crystalcube_anim_param.start_z ,
            CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME/2, 
            g_crystalcube_anim_param.cur_frame);
        damp_angle_offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, 
            g_crystalcube_anim_param.end_angle - g_crystalcube_anim_param.start_angle,
            CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME,
            g_crystalcube_anim_param.cur_frame);
//jinx_crystal        
        g_crystalcube_param.s3d_geometry.axis.z = g_crystalcube_anim_param.end_z - damp_z_offset;
        g_crystalcube_param.s3d_geometry.angle = (CUBEPAGEMENU_ENTERANIM_START_ANGLE + damp_angle_offset) % CRYSTALCUBE_CIRCLE_ANGLE;
    }
    else
    {
        damp_angle_offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_2, 
            g_crystalcube_anim_param.end_angle - g_crystalcube_anim_param.start_angle, 
            CUBEPAGEMENU_ENTERANIM_TOTAL_FRAME,
            g_crystalcube_anim_param.cur_frame);
         g_crystalcube_param.s3d_geometry.angle = (CUBEPAGEMENU_ENTERANIM_START_ANGLE + damp_angle_offset) % CRYSTALCUBE_CIRCLE_ANGLE;
    }
    SetFourCalcPage(&g_crystalcube_param);
    S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);

    g_crystalcube_anim_param.cur_frame ++;

    if(g_crystalcube_anim_param.cur_frame <= g_crystalcube_anim_param.total_frame)
    { 
        SendNotify(MMICRYSTALCUBE_MSG_ANIM_FRAME_IND);
    }
    else
    { 
        SendNotify(MMICRYSTALCUBE_MSG_ANIM_END_IND);
    } 
}
/*****************************************************************************/
//  Description : oper_update_reflection
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void oper_update_reflection(uint16 page_index)
{
    uint16 x = 0;
    uint16 y = 0;
    uint32 color = 0;
    
    //SCI_ASSERT(g_crystalcube_param.is_init); /*assert verified*/
    //SCI_ASSERT(g_crystalcube_param.reflection_buffer_ptr); /*assert verified*/
    if (
        (!g_crystalcube_param.is_init)||
        (PNULL == g_crystalcube_param.reflection_buffer_ptr)
        )
    { 
        return;
    }

    SCI_MEMSET(g_crystalcube_param.reflection_buffer_ptr, 0x00, g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));
    //更新page  buf
    for(y = 0; y < g_crystalcube_param.height * CUBE_NO_SHADOW_AREA_PARA / CUBE_BASE_SRC_PARA; y ++)
    {
        for(x = 0; x < g_crystalcube_param.width; x ++)
        {
            ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + y] = ((uint32 *)g_crystalcube_param.s3d_rect[page_index].src_ptr)[y * g_crystalcube_param.width + x];
        }
    }

    for(; y < g_crystalcube_param.height; y ++)
    {
        for(x = 0; x < g_crystalcube_param.width; x ++)
        {
            color = ((uint32 *)g_crystalcube_param.s3d_rect[page_index].src_ptr)[y * g_crystalcube_param.width + x];

            ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + y] = color;
            ((uint32 *)g_crystalcube_param.reflection_buffer_ptr)[x * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA + g_crystalcube_param.height * 2 - y -1] =
            (color & 0x00FFFFFF) | ((((color & 0xFF000000) >> 6) * (y - g_crystalcube_param.height * CUBE_NO_SHADOW_AREA_PARA / CUBE_BASE_SRC_PARA)) & 0xFF000000);;
        }
    }

    SCI_MEMCPY(g_crystalcube_param.s3d_rect[page_index].src_ptr, g_crystalcube_param.reflection_buffer_ptr, g_crystalcube_param.width * g_crystalcube_param.height * CUBE_WITH_SHADOW_HEIGHT_PARA / CUBE_BASE_SRC_PARA * sizeof(int32));
 
}
/*****************************************************************************/
//  Description : oper_display
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
LOCAL void oper_display()
{

     if (!g_crystalcube_param.is_init) return;
     
    if (!oper_set_output_buffer()) return;
    
    reset_output_buffer();
   SetFourCalcPage(&g_crystalcube_param);
    S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);

    SendNotify(MMICRYSTALCUBE_MSG_DISPLAY_IND);

}

LOCAL void oper_rotate(int16 angle)
{
    if (!g_crystalcube_param.is_init) return;
    

    g_crystalcube_param.rotate_angle = angle;
    //rotate_angle 的正负来判断旋转的方向，每次旋转的角度为6度
    if(g_crystalcube_param.rotate_angle > 0)
    {
        g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_param.s3d_geometry.angle >= CRYSTALCUBE_CIRCLE_ANGLE - CUBE_MIN_ANGLE) ? 0 : (g_crystalcube_param.s3d_geometry.angle + CUBE_MIN_ANGLE);
        g_crystalcube_param.rotate_angle = (g_crystalcube_param.rotate_angle - CUBE_MIN_ANGLE >= 0) ? (g_crystalcube_param.rotate_angle - CUBE_MIN_ANGLE) : 0;
        
    }
    else
    {
        g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_param.s3d_geometry.angle >= CUBE_MIN_ANGLE) ? (g_crystalcube_param.s3d_geometry.angle - CUBE_MIN_ANGLE) : (CRYSTALCUBE_CIRCLE_ANGLE - CUBE_MIN_ANGLE);
        g_crystalcube_param.rotate_angle = (g_crystalcube_param.rotate_angle + CUBE_MIN_ANGLE <= 0) ? (g_crystalcube_param.rotate_angle + CUBE_MIN_ANGLE) : 0;
    }


    if (!oper_set_output_buffer()) return;
    SetFourCalcPage(&g_crystalcube_param);
    S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);


    SendNotify(MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND);
    
}


LOCAL void oper_get_next_rotate_frame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr*/)
{
    //int32 damp_z_offset = 0;
    //int32 damp_angle_offset = 0;


     if (!g_crystalcube_param.is_init) return;
    
    
    if(g_crystalcube_param.rotate_angle > 0 || g_crystalcube_param.is_forever == TRUE)
    {
    //jinx_crystal
        g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_param.s3d_geometry.angle >= CRYSTALCUBE_CIRCLE_ANGLE - CUBE_MIN_ANGLE) ? 0 : (g_crystalcube_param.s3d_geometry.angle + CUBE_MIN_ANGLE);
        //SCI_TRACE_LOW:"****** %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_815_112_2_18_2_37_23_0,(uint8*)"d",g_crystalcube_param.s3d_geometry.angle);

        if (g_crystalcube_param.is_forever == FALSE)
        {
            g_crystalcube_param.rotate_angle = (g_crystalcube_param.rotate_angle - CUBE_MIN_ANGLE >= 0) ? (g_crystalcube_param.rotate_angle - CUBE_MIN_ANGLE) : 0;
        }
        //SCI_TRACE_LOW:"56789 time is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_821_112_2_18_2_37_23_1,(uint8*)"d",SCI_GetTickCount());

        if (!oper_set_output_buffer()) return;
        SetFourCalcPage(&g_crystalcube_param);
        S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);
 
        SendNotify(MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND);
    }
    else if(g_crystalcube_param.rotate_angle < 0 || g_crystalcube_param.is_forever == TRUE)
    {
        g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_param.s3d_geometry.angle >= CUBE_MIN_ANGLE) ? (g_crystalcube_param.s3d_geometry.angle - CUBE_MIN_ANGLE) : (CRYSTALCUBE_CIRCLE_ANGLE - CUBE_MIN_ANGLE);
        //SCI_TRACE_LOW:"===== %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_832_112_2_18_2_37_23_2,(uint8*)"d",g_crystalcube_param.s3d_geometry.angle);

        if (g_crystalcube_param.is_forever == FALSE)
        {
            g_crystalcube_param.rotate_angle = (g_crystalcube_param.rotate_angle + CUBE_MIN_ANGLE <= 0) ? (g_crystalcube_param.rotate_angle + CUBE_MIN_ANGLE) : 0;
        }
        //SCI_TRACE_LOW:"1234 time is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_838_112_2_18_2_37_23_3,(uint8*)"d",SCI_GetTickCount());

        if (!oper_set_output_buffer()) return;
        SetFourCalcPage(&g_crystalcube_param);
        S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);
 
        SendNotify(MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND);
    }
    else
    {
        SendNotify(MMICRYSTALCUBE_MSG_ROTATE_END_IND);
    }
        
}


LOCAL void on_tpscroll_up(int16 xpos,int16 ypos)
{

    //SCI_TRACE_LOW:"CCUBE: on_tpscroll_up,x=%d,y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_857_112_2_18_2_37_23_4,(uint8*)"dd",xpos,ypos);
    
    MMIAPICCUBE_AddVelocityItem(xpos, ypos);
    
    CCUBE_ComputeCurrentVelocity(&g_crystalcube_tp_param.fling_velocity, PNULL);
    
    g_crystalcube_tp_param.xpos = xpos;
    g_crystalcube_tp_param.ypos = ypos;

    g_crystalcube_tp_param.cur_angle = (g_crystalcube_tp_param.start_xpos - g_crystalcube_tp_param.xpos) * (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM)
        / g_crystalcube_param.s3d_window.size.w;
        
    g_crystalcube_tp_param.cur_frame = 0;
    g_crystalcube_tp_param.total_frame = CUBEPAGEMENU_TP_UP_BACK_TOTAL_FRAME;
//jinx_crystal
        if (g_crystalcube_tp_param.cur_angle < 0)
        {
            g_crystalcube_tp_param.cur_angle += CRYSTALCUBE_CIRCLE_ANGLE;
        }
        else if (g_crystalcube_tp_param.cur_angle >= CRYSTALCUBE_CIRCLE_ANGLE)
        {
            g_crystalcube_tp_param.cur_angle -= CRYSTALCUBE_CIRCLE_ANGLE;
        }
 

    g_crystalcube_tp_param.start_angle = g_crystalcube_param.s3d_geometry.angle;

    g_crystalcube_tp_param.is_rotate = TRUE;
    g_crystalcube_tp_param.is_tp_down = FALSE;

    MMIAPICCUBE_GetNextTpScrollAnimFrame( );

}

LOCAL void on_tpscroll_down(int16 xpos,int16 ypos)
{
    //SCI_TRACE_LOW:"CCUBE: on_tpscroll_down,x=%d,y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_893_112_2_18_2_37_23_5,(uint8*)"dd",xpos,ypos);
    
    SCI_MEMSET(&g_crystalcube_tp_param, 0x00, sizeof(g_crystalcube_tp_param));

    CCUBE_ResetVelocityItem();

    MMIAPICCUBE_AddVelocityItem(xpos, ypos);
    
	/*tp down的时候，不再是rotate状态*/
    //g_crystalcube_tp_param.is_rotate = TRUE;
    g_crystalcube_tp_param.fling_velocity = 0;

    g_crystalcube_tp_param.total_frame = CUBEPAGEMENU_TP_UP_BACK_TOTAL_FRAME;
    
//jinx_crystal
    if (g_crystalcube_param.s3d_geometry.angle < 0)
    {
        g_crystalcube_param.s3d_geometry.angle += CRYSTALCUBE_CIRCLE_ANGLE;
    }
    else if (g_crystalcube_param.s3d_geometry.angle >= CRYSTALCUBE_CIRCLE_ANGLE)
    {
        g_crystalcube_param.s3d_geometry.angle -= CRYSTALCUBE_CIRCLE_ANGLE;
    }

    g_crystalcube_tp_param.is_tp_down = TRUE;
    g_crystalcube_tp_param.start_angle = g_crystalcube_param.s3d_geometry.angle;
    g_crystalcube_tp_param.cur_angle = g_crystalcube_param.s3d_geometry.angle;
    g_crystalcube_tp_param.start_xpos = xpos;
    g_crystalcube_tp_param.start_ypos = ypos;
    g_crystalcube_tp_param.pre_xpos = g_crystalcube_tp_param.start_xpos;
    g_crystalcube_tp_param.pre_ypos = g_crystalcube_tp_param.start_ypos;
}

LOCAL void on_tpscroll_move(int16 xpos,int16 ypos)
{

    //SCI_TRACE_LOW:"CCUBE: on_tpscroll_move,x=%d,y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_929_112_2_18_2_37_23_6,(uint8*)"dd",xpos,ypos);
    
    MMIAPICCUBE_AddVelocityItem(xpos, ypos);
    
    g_crystalcube_tp_param.pre_xpos = g_crystalcube_tp_param.xpos;
    g_crystalcube_tp_param.pre_ypos = g_crystalcube_tp_param.ypos;

    g_crystalcube_tp_param.xpos = xpos;
    g_crystalcube_tp_param.ypos = ypos;

    g_crystalcube_tp_param.cur_angle = (g_crystalcube_tp_param.start_xpos - g_crystalcube_tp_param.xpos) * (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM)
        / g_crystalcube_param.s3d_window.size.w;
    //jinx_crystal
    if (g_crystalcube_tp_param.cur_angle < 0)
    {
        g_crystalcube_tp_param.cur_angle += CRYSTALCUBE_CIRCLE_ANGLE;
    }
    else if (g_crystalcube_tp_param.cur_angle >= CRYSTALCUBE_CIRCLE_ANGLE)
    {
        g_crystalcube_tp_param.cur_angle -= CRYSTALCUBE_CIRCLE_ANGLE;
    }

    g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_tp_param.start_angle + g_crystalcube_tp_param.cur_angle)%CRYSTALCUBE_CIRCLE_ANGLE;
    g_crystalcube_tp_param.cur_frame = 0;

    if (!oper_set_output_buffer()) return;
    SetFourCalcPage(&g_crystalcube_param);
    S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);
 

    SendNotify(MMICRYSTALCUBE_MSG_TP_SCROOL_FRAME_IND);

 
}


LOCAL void oper_tp_scroll(MMICRYSTALCUBE_TP_REQ_T* tp_param_ptr)
{
    FIX16_POINT_T tp_point = {0};
    
    tp_point.x = tp_param_ptr->xpos;
    tp_point.y = tp_param_ptr->ypos;

    switch(tp_param_ptr->tp_state)
    {
        case MMICRYSTALCUBE_TP_UP:
            {
                on_tpscroll_up(tp_param_ptr->xpos,tp_param_ptr->ypos);
                break;
            }
        case MMICRYSTALCUBE_TP_MOVE:
            {
                on_tpscroll_move(tp_param_ptr->xpos,tp_param_ptr->ypos);
                break;
            }
        case MMICRYSTALCUBE_TP_DOWN:
            {
                on_tpscroll_down(tp_param_ptr->xpos,tp_param_ptr->ypos);
                break;
            }
        default:
            {
                SCI_ASSERT(0); /*assert verified*/
                break;
            }
    }
    
}


LOCAL void oper_get_next_tpscrollanim_frame(/*uint8* out_buffer_raw_ptr,uint8* out_buffer_ptr*/)
{
    //int32 damp_z_offset = 0;
    int32 damp_angle_offset = 0;
    uint16 page_index = 0;
    int16 ilen = 0;

     if (!g_crystalcube_param.is_init) return;

    if (!g_crystalcube_tp_param.is_rotate) return;
    
    
    if (abs(g_crystalcube_tp_param.xpos - g_crystalcube_tp_param.pre_xpos) > CRYSTALCUBE_TP_MOVE_DIS)
    {
        if ((g_crystalcube_tp_param.fling_velocity) != 0)
        {
            if (g_crystalcube_tp_param.fling_velocity > 0)
            {
                g_crystalcube_tp_param.fling_velocity -= CUBEPAGEMENU_TP_UP_ACCELERATION;
                if (g_crystalcube_tp_param.fling_velocity < CUBEPAGE_FLING_MIN_VELOCITY)
                {
                    //SCI_TRACE_LOW("***** %f",g_crystalcube_tp_param.fling_velocity);                                
                    g_crystalcube_tp_param.fling_velocity = 0;
                }
            }
            else if (g_crystalcube_tp_param.fling_velocity <0)
            {
                g_crystalcube_tp_param.fling_velocity += CUBEPAGEMENU_TP_UP_ACCELERATION;
                if (g_crystalcube_tp_param.fling_velocity > -CUBEPAGE_FLING_MIN_VELOCITY)
                {
                    //SCI_TRACE_LOW("##### %f",g_crystalcube_tp_param.fling_velocity);                                                            
                    g_crystalcube_tp_param.fling_velocity = 0;
                }
            }

            //SCI_TRACE_LOW:"CCUBE:  fling_velocity=%f"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1034_112_2_18_2_37_23_7,(uint8*)"f",g_crystalcube_tp_param.fling_velocity);
            
            g_crystalcube_param.s3d_geometry.angle -= (int16)g_crystalcube_tp_param.fling_velocity;
//jinx_crystal
            if (g_crystalcube_param.s3d_geometry.angle < 0)
            {
                g_crystalcube_param.s3d_geometry.angle += CRYSTALCUBE_CIRCLE_ANGLE;
            }
            else if (g_crystalcube_param.s3d_geometry.angle >= CRYSTALCUBE_CIRCLE_ANGLE)
            {
                g_crystalcube_param.s3d_geometry.angle -= CRYSTALCUBE_CIRCLE_ANGLE;
            }

            //SCI_TRACE_LOW:"CCUBE:  angle is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1047_112_2_18_2_37_23_8,(uint8*)"d",g_crystalcube_param.s3d_geometry.angle);

            if (!oper_set_output_buffer()) return;
            SetFourCalcPage(&g_crystalcube_param);
            S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);

            g_crystalcube_tp_param.start_angle = g_crystalcube_param.s3d_geometry.angle;  //阻尼起始角度                        
 
            SendNotify(MMICRYSTALCUBE_MSG_SRCOLLANIM_IND);
            
        }
        else
        {
            //SCI_TRACE_LOW:"CCUBE: scroll start_angle is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1060_112_2_18_2_37_24_9,(uint8*)"d",g_crystalcube_tp_param.start_angle);
            
            if (g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) == CUBEPAGEMENU_ENTERANIM_START_ANGLE) //刚好转到22°时，此时直接静止，不回退或者前进
            {
                //SCI_TRACE_LOW:"CCUBE: scroll start_angle is equal 30 degree"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1064_112_2_18_2_37_24_10,(uint8*)"");
                g_crystalcube_tp_param.is_rotate = FALSE;

                g_crystalcube_param.s3d_geometry.angle = g_crystalcube_tp_param.start_angle;
//jinx_crystal                
                page_index = g_crystalcube_param.s3d_geometry.angle / (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);

                //调试代码
                if (page_index > MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM-1 || g_crystalcube_param.s3d_geometry.angle % (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) != CUBEPAGEMENU_ENTERANIM_START_ANGLE)
                {
                    SCI_PASSERT(0, ("angle %d", g_crystalcube_param.s3d_geometry.angle));/*assert verified*/
                }
 
                SendNotifyWithParam(MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND,(void*)&page_index,sizeof(uint16));
                
            }
            else
            {
                if (g_crystalcube_tp_param.is_tp_down)  /*tp被按下，应立刻停止*/
                {

                    //SCI_TRACE_LOW:"CCUBE: scroll tp is down , stop immediately"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1085_112_2_18_2_37_24_11,(uint8*)"");
                    
                    g_crystalcube_tp_param.is_rotate = FALSE;

                    g_crystalcube_param.s3d_geometry.angle = g_crystalcube_tp_param.start_angle;


                    if (!oper_set_output_buffer()) return;
                    SetFourCalcPage(&g_crystalcube_param);
                    S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);
 
                    SendNotify(MMICRYSTALCUBE_MSG_TP_SCROOL_FRAME_IND);
                    
                    g_crystalcube_tp_param.is_tp_down = FALSE;
                }
                else
                {
                    //SCI_TRACE_LOW:"CCUBE: scroll angle =%d, start anim"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1102_112_2_18_2_37_24_12,(uint8*)"d",g_crystalcube_tp_param.start_angle);
                    if (g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) >= (CRYSTALCUBE_CIRCLE_ANGLE/(2*MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM)))
                    {
                        ilen = CUBEPAGEMENU_ENTERANIM_START_ANGLE - g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) + (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);
                    }
                    else
                    {
                        ilen = CUBEPAGEMENU_ENTERANIM_START_ANGLE - g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);
                    }

                    damp_angle_offset = MMK_ClcltDampPlayOffset(
                        DAMP_PLAY_TYPE_1,
                        ilen,
                        g_crystalcube_tp_param.total_frame,
                        g_crystalcube_tp_param.cur_frame);
//jinx_crystal
                    g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_tp_param.start_angle + damp_angle_offset)%CRYSTALCUBE_CIRCLE_ANGLE;

                    g_crystalcube_tp_param.cur_frame ++;
                    //SCI_TRACE_LOW:"CCUBE: scroll anim frame=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1121_112_2_18_2_37_24_13,(uint8*)"d",g_crystalcube_tp_param.cur_frame);

                    if (g_crystalcube_tp_param.cur_frame <= g_crystalcube_tp_param.total_frame)
                    {

                        if (!oper_set_output_buffer()) return;
                        SetFourCalcPage(&g_crystalcube_param);
                        S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);
            
                        SendNotify(MMICRYSTALCUBE_MSG_SRCOLLANIM_IND);
                 
                    }
                    else
                    {
                        g_crystalcube_tp_param.cur_frame = 0;
                        g_crystalcube_tp_param.is_rotate = FALSE; 
//jinx_crystal
                        page_index = g_crystalcube_param.s3d_geometry.angle / (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);

                        //调试代码
                        if (page_index > MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM-1 || g_crystalcube_param.s3d_geometry.angle % (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) != CUBEPAGEMENU_ENTERANIM_START_ANGLE)
                        {
                            SCI_PASSERT(0, ("angle %d", g_crystalcube_param.s3d_geometry.angle));/*assert verified*/
                        }
                        
                        SendNotifyWithParam(MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND, (void*)&page_index, sizeof(uint16));
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"CCUBE : not move angle=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1154_112_2_18_2_37_24_14,(uint8*)"d",g_crystalcube_tp_param.start_angle);
        
        if (g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) == CUBEPAGEMENU_ENTERANIM_START_ANGLE)
        {
            g_crystalcube_tp_param.cur_frame = 0;
            g_crystalcube_tp_param.is_rotate = FALSE;
            g_crystalcube_param.s3d_geometry.angle = g_crystalcube_tp_param.start_angle; 
//jinx_crystal
            page_index = g_crystalcube_param.s3d_geometry.angle / (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);

            //调试代码
            if (page_index > MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM-1 || g_crystalcube_param.s3d_geometry.angle % (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) != CUBEPAGEMENU_ENTERANIM_START_ANGLE)
            {
                SCI_PASSERT(0, ("angle %d", g_crystalcube_param.s3d_geometry.angle));/*assert verified*/
            } 

            //SCI_TRACE_LOW:"CCUBE : angle is on position"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1170_112_2_18_2_37_24_15,(uint8*)"",g_crystalcube_tp_param.start_angle);
            
            SendNotifyWithParam(MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND, (void*)&page_index, sizeof(uint16));
            return;
            
        }
        else if (g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) >= CRYSTALCUBE_CIRCLE_ANGLE/(2*MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM))
        {
            ilen = CUBEPAGEMENU_ENTERANIM_START_ANGLE - g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) + (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);
        }
        else
        {
            ilen = CUBEPAGEMENU_ENTERANIM_START_ANGLE - g_crystalcube_tp_param.start_angle%(CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);
        }
        
        damp_angle_offset = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,
            ilen,
            g_crystalcube_tp_param.total_frame, 
            g_crystalcube_tp_param.cur_frame);
//jinx_crystal
        g_crystalcube_param.s3d_geometry.angle = (g_crystalcube_tp_param.start_angle + damp_angle_offset)%CRYSTALCUBE_CIRCLE_ANGLE;

        g_crystalcube_tp_param.cur_frame++;

        //SCI_TRACE_LOW:"CCUBE: 2 scroll anim frame=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1194_112_2_18_2_37_24_16,(uint8*)"d",g_crystalcube_tp_param.cur_frame);

        if (g_crystalcube_tp_param.cur_frame <= g_crystalcube_tp_param.total_frame)
        { 


            if (!oper_set_output_buffer()) return;
            SetFourCalcPage(&g_crystalcube_param);
            S3D_CubeAxisRotation_Crystal(&g_crystalcube_param.s3d_geometry, &g_crystalcube_param.s3d_cube, &g_crystalcube_param.s3d_window);

        
            SendNotify(MMICRYSTALCUBE_MSG_SRCOLLANIM_IND);
        }
        else
        {
           //SCI_TRACE_LOW:"CCUBE: scroll end: t_frm=%d,c_frm=%d,damp_angle_offset=%d,start_angle=%d,geometry_angle=%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1214_112_2_18_2_37_24_17,(uint8*)"ddddd",g_crystalcube_tp_param.total_frame,g_crystalcube_tp_param.cur_frame,damp_angle_offset,g_crystalcube_tp_param.start_angle,g_crystalcube_param.s3d_geometry.angle);
		   
            g_crystalcube_tp_param.cur_frame = 0;
            g_crystalcube_tp_param.is_rotate = FALSE;
            g_crystalcube_tp_param.start_angle = g_crystalcube_param.s3d_geometry.angle;
//jinx_crystal            
            page_index = g_crystalcube_param.s3d_geometry.angle / (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM);

            //调试代码
            if (page_index > MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM-1 || g_crystalcube_param.s3d_geometry.angle % (CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM) != CUBEPAGEMENU_ENTERANIM_START_ANGLE)
            {
                SCI_PASSERT(0, ("angle %d", g_crystalcube_param.s3d_geometry.angle));/*assert verified*/
            } 

            //SCI_TRACE_LOW:"CCUBE: sendnotify MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1228_112_2_18_2_37_24_18,(uint8*)"");
            SendNotifyWithParam(MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND, (void*)&page_index, sizeof(uint16));
        }
    }
        
}


LOCAL void oper_tp_cmd(int16 xpos,int16 ypos)
{
    FIX16_POINT_T tp_point = {0};
    FIX16_POINT_T sVertex[4] = {0};
    uint16 x = 0;
    uint16 y = 0;
    uint16 index = 0;
    BOOLEAN ret = FALSE;
    MMICRYSTALCUBE_TP_CMD_T tp_cmd;
   
    tp_point.x = xpos;
    tp_point.y = ypos;

    for (index = 0; index < CRYSTALCUBE_MENUMAIN_ICON_ITEM_MAX_NUM; index ++)
    {
        x = index % CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM;
        y = index / CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM;
        sVertex[0].x = g_mmicrystalcube_iconitem_point_array[y * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x].x;
        sVertex[0].y = g_mmicrystalcube_iconitem_point_array[y * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x].y;

        sVertex[1].x = g_mmicrystalcube_iconitem_point_array[y * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x + 1].x;
        sVertex[1].y = g_mmicrystalcube_iconitem_point_array[y * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x + 1].y;

        sVertex[2].x = g_mmicrystalcube_iconitem_point_array[(y + 1) * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x + 1].x;
        sVertex[2].y = g_mmicrystalcube_iconitem_point_array[(y + 1) * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x + 1].y;

        sVertex[3].x = g_mmicrystalcube_iconitem_point_array[(y + 1) * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x].x;
        sVertex[3].y = g_mmicrystalcube_iconitem_point_array[(y + 1) * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM + 1) + x].y;

        ret = S3D_QuadrilateralCheckPoint_Crystal( &sVertex[0], &tp_point);

        if (TRUE == ret)
        {
            break;
        }
    }

    tp_cmd.is_response = ret;
    if (tp_cmd.is_response)
    {
        tp_cmd.index = y * (CRYSTALCUBE_MENUMAIN_ICON_PAGE_LINE_NUM) + x;
    }

    SendNotifyWithParam(MMICRYSTALCUBE_MSG_TP_CMD_CNF, (void*)&tp_cmd, sizeof(MMICRYSTALCUBE_TP_CMD_T));
}


LOCAL void oper_release_param()
{
    g_crystalcube_param.is_init = FALSE;

    if(g_crystalcube_param.reflection_buffer_ptr)
    {
        SCI_FREE(g_crystalcube_param.reflection_buffer_ptr);
        g_crystalcube_param.reflection_buffer_ptr = PNULL;
    }
	/*
    if(g_crystalcube_param.out_buffer_raw_ptr)
    {
        SCI_FREE(g_crystalcube_param.out_buffer_raw_ptr);
        g_crystalcube_param.out_buffer_raw_ptr = PNULL;
        g_crystalcube_param.out_buffer_ptr = PNULL;
    }
	*/

    g_crystalcube_param.s3d_window.color_ptr = PNULL;
 
    //SendNotify(MMICRYSTALCUBE_MSG_PARAMRELEASE_CNF);    
}

LOCAL void oper_exit()
{
    //SCI_TRACE_LOW("CCUBE: MMICRYSTALCUBE_MSG_EXIT_REQ");
    //is_task_exit = TRUE;

    g_crystalcube_param.is_init = FALSE;

    if(g_crystalcube_param.reflection_buffer_ptr)
    {
        SCI_FREE(g_crystalcube_param.reflection_buffer_ptr);
        g_crystalcube_param.reflection_buffer_ptr = PNULL;
    }

    /*
    if(g_crystalcube_param.out_buffer_raw_ptr)
    {
        SCI_FREE(g_crystalcube_param.out_buffer_raw_ptr);
        g_crystalcube_param.out_buffer_raw_ptr = PNULL;
        g_crystalcube_param.out_buffer_ptr = PNULL;
    }
    */

//     g_crystalcube_param.out_buffer_raw_ptr = PNULL;
    g_crystalcube_param.out_buffer_ptr = PNULL;
    
    g_crystalcube_param.s3d_window.color_ptr = PNULL;

   g_crystalcube_anim_param.cur_frame = 0; 
}


LOCAL void crystalcube_calc_func(
                           uint32 calc_type,
                           void* param_ptr, 
                           uint32 param_size
                          )
{
    //SCI_TRACE_LOW:"CCUBE: crystalcube_calc_func type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1343_112_2_18_2_37_24_19,(uint8*)"d",calc_type);
    switch (calc_type)
    {
    case MMICRYSTALCUBE_MSG_PARAMINIT_REQ:
        {
            oper_init_param((MMICRYSTALCUBE_INIT_PARAM_T*)param_ptr,FALSE);
            break;
        }
    case MMICRYSTALCUBE_MSG_PARAM_RESET_REQ:
        {
            oper_init_param((MMICRYSTALCUBE_INIT_PARAM_T*)param_ptr,TRUE);
            break;
        }
        /*
    case MMICRYSTALCUBE_MSG_PARAM_BUF_SET_REQ:
        {
            MMICUBEPAGE_BUFFER_PARAM_T* buf_param_ptr = (MMICUBEPAGE_BUFFER_PARAM_T*)param_ptr;  
            oper_set_output_buffer(buf_param_ptr->out_buffer_raw_ptr, buf_param_ptr->out_buffer_ptr);
            break;
        }
        */
        
    case MMICRYSTALCUBE_MSG_ROTATE_REQ:
        {
            int16 angle = *((int16*)param_ptr);
            oper_rotate(angle);
            break;
        }

    case MMICRYSTALCUBE_MSG_ROTATE_FRAME_CNF:
        {
            //MMICUBEPAGE_BUFFER_PARAM_T* buf_param = (MMICUBEPAGE_BUFFER_PARAM_T*)param_ptr;
            //oper_get_next_rotate_frame(buf_param->out_buffer_raw_ptr, buf_param->out_buffer_ptr);     
            oper_get_next_rotate_frame();
            break;
        }

    case MMICRYSTALCUBE_MSG_TP_SCROOL_REQ:
        {
            MMICRYSTALCUBE_TP_REQ_T* tp_param_ptr = (MMICRYSTALCUBE_TP_REQ_T*)param_ptr;
            oper_tp_scroll(tp_param_ptr);
            break;
        }

    case MMICRYSTALCUBE_MSG_SRCOLLANIM_CNF:
        {
            //MMICUBEPAGE_BUFFER_PARAM_T* buf_param = (MMICUBEPAGE_BUFFER_PARAM_T*)param_ptr;
            //oper_get_next_tpscrollanim_frame(buf_param->out_buffer_raw_ptr, buf_param->out_buffer_ptr);           
            oper_get_next_tpscrollanim_frame();
            break;
        }

    case MMICRYSTALCUBE_MSG_TP_CMD_REQ:
        {
            MMICRYSTALCUBE_TP_REQ_T* tp_req_param_ptr = (MMICRYSTALCUBE_TP_REQ_T*)param_ptr;
            oper_tp_cmd(tp_req_param_ptr->xpos,tp_req_param_ptr->ypos);
            break;
        }

    case MMICRYSTALCUBE_MSG_DISPLAY_REQ:
        {
            oper_display();
            break;
        } 

    case MMICRYSTALCUBE_MSG_REFLECTION_UPDATE_REQ:
        {
            uint16 page_index= *((uint16*)param_ptr);
            oper_update_reflection(page_index);
            break;
        }
        
    case MMICRYSTALCUBE_MSG_PARAMRELEASE_REQ:
        {
            oper_release_param();
            s_release_is_done = TRUE;
            break;
        }

    /*
    case MMICRYSTALCUBE_MSG_ROTATEFOREVER_REQ:
        if (g_crystalcube_param.is_init)
        {
            g_crystalcube_param.is_forever = (g_crystalcube_param.is_forever == TRUE) ? FALSE : TRUE;
        }
        break;
    */

    case MMICRYSTALCUBE_MSG_ANIM_START_REQ:
        {
            BOOLEAN is_enter = *((uint8*)param_ptr);
            oper_start_anim(is_enter);
            break;
        }

    case MMICRYSTALCUBE_MSG_ANIM_FRAME_CNF:
        {
            //MMICUBEPAGE_BUFFER_PARAM_T* buf_param = (MMICUBEPAGE_BUFFER_PARAM_T*)param_ptr;
            //oper_get_next_anim_frame(buf_param->out_buffer_raw_ptr, buf_param->out_buffer_ptr);
            oper_get_next_anim_frame();
            break;
        }

    /*
    case MMICRYSTALCUBE_MSG_SRCOLLANIM_START_REQ:
        {
            MMICUBEPAGE_BUFFER_PARAM_T* buf_param = (MMICUBEPAGE_BUFFER_PARAM_T*)param_ptr;
            SendNotify(MMICRYSTALCUBE_MSG_SRCOLLANIM_START_CNF);
            oper_get_next_tpscrollanim_frame(NULL, buf_param->out_buffer_ptr);
	     break;
        }
    */
    case MMICRYSTALCUBE_MSG_EXIT_REQ:
        {
            oper_exit();
            break;
        }

    default:
        {
            //SCI_TRACE_LOW:"UNKNOWN calc type is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1463_112_2_18_2_37_24_20,(uint8*)"d", calc_type);
            SCI_ASSERT(0);/*assert verified*/
            break;
        }
    }

}




LOCAL void do_operation(uint32 oper_type,void* param_ptr,uint32 param_size)
{
    MMISE_CALC_PARAM_T calc_param;

    calc_param.calc_type = oper_type;
    calc_param.data_ptr = param_ptr;
    calc_param.data_size = param_size;
    calc_param.calc_func = crystalcube_calc_func;
    
    MMISE_StartCalculate(&calc_param);
}



/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_AddVelocityItem(int32 x,int32 y)
{
    
    if ( s_crystalcube_velocity_tracker.item_num < CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM )
    {
        s_crystalcube_velocity_tracker.item[s_crystalcube_velocity_tracker.item_num].x = x;
        s_crystalcube_velocity_tracker.item[s_crystalcube_velocity_tracker.item_num].y = y;
        s_crystalcube_velocity_tracker.item[s_crystalcube_velocity_tracker.item_num].time = SCI_GetTickCount();
        s_crystalcube_velocity_tracker.item_num++;
    }
    else
    {
        memmove( s_crystalcube_velocity_tracker.item, s_crystalcube_velocity_tracker.item + 1, sizeof(CRYSTALCUBE_VELOCITY_TRACKER_ITEM_T) * (CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM - 1) );

        s_crystalcube_velocity_tracker.item[CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM-1].x = x;
        s_crystalcube_velocity_tracker.item[CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM-1].y = y;
        s_crystalcube_velocity_tracker.item[CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM-1].time = SCI_GetTickCount();

        s_crystalcube_velocity_tracker.item_num = CUBEPAGE_VELOCITY_TRACKER_ITEM_NUM;
    }
}

/*****************************************************************************/
// 	Description : 重置速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void CCUBE_ResetVelocityItem( void )
{
    s_crystalcube_velocity_tracker.item_num = 0;
}



/*****************************************************************************/
// 	Description : 计算当前速度
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CCUBE_ComputeCurrentVelocity(
                                          float* vx_ptr, //x方向速度, in/out
                                          float* vy_ptr  //y方向速度, in/out
                                          )
{
    float  x_velocity = 0;
    float  y_velocity = 0;
    uint32 i = 0;
    uint8  item_num = s_crystalcube_velocity_tracker.item_num;
    BOOLEAN result = FALSE;

    for(i=0;i<item_num;i++)
        //SCI_TRACE_LOW:"CCUBE: ComputeCurrentVelocity %d:  (%d,%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1541_112_2_18_2_37_25_21,(uint8*)"ddd",i,s_crystalcube_velocity_tracker.item[i].x,s_crystalcube_velocity_tracker.item[i].time);

    if ( item_num > 0)
    {
        int32 first_x = s_crystalcube_velocity_tracker.item[0].x;
        int32 first_y = s_crystalcube_velocity_tracker.item[0].y;
        uint32 first_time = s_crystalcube_velocity_tracker.item[0].time;
        
        for ( i = 1; i < item_num; i++ ) 
        {
            int32 dur = s_crystalcube_velocity_tracker.item[i].time - first_time;

            if ( 0 != dur ) 
            {
                float dist = s_crystalcube_velocity_tracker.item[i].x - first_x;
                float vel = dist/dur * CUBEPAGE_VELOCITY_RATIO;
                if ( 0 == x_velocity )
                {
                    x_velocity = vel;
                }
                else 
                {
                    x_velocity = (x_velocity + vel) * 0.5f;
                }
                
                dist = s_crystalcube_velocity_tracker.item[i].y - first_y;
                vel = dist/dur * CUBEPAGE_VELOCITY_RATIO;
                
                if ( 0 == y_velocity ) 
                {
                    y_velocity = vel;
                }
                else
                {
                    y_velocity = (y_velocity + vel) * 0.5f;
                }
            }

        }
    }

    if ( PNULL != vx_ptr )
    {
        if ( x_velocity > CUBEPAGE_FLING_MIN_VELOCITY )
        {
            if ( *vx_ptr > 0 )
            {
                x_velocity += *vx_ptr;
            }

            *vx_ptr = MIN( x_velocity, CUBEPAGE_FLING_MAX_VELOCITY );
        }
        else if ( x_velocity < -CUBEPAGE_FLING_MIN_VELOCITY )
        {
            if ( *vx_ptr < 0 )
            {
                x_velocity += *vx_ptr;
            }

            *vx_ptr = MAX( x_velocity, -CUBEPAGE_FLING_MAX_VELOCITY );
        }
        else
        {
            *vx_ptr = 0;
        }

        result = TRUE;

        //SCI_TRACE_LOW:"CCUBE: ComputeCurrentVelocity result=%f"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CRYSTAL_CUBE_1609_112_2_18_2_37_25_22,(uint8*)"f",*vx_ptr);
    }

    if ( PNULL != vy_ptr )
    {
        if ( y_velocity > CUBEPAGE_FLING_MIN_VELOCITY )
        {
            if ( *vy_ptr > 0 )
            {
                y_velocity += *vy_ptr;
            }

            *vy_ptr = MIN( y_velocity, CUBEPAGE_FLING_MAX_VELOCITY );
        }
        else if ( y_velocity < -CUBEPAGE_FLING_MIN_VELOCITY )
        {
            if ( *vy_ptr < 0 )
            {
                y_velocity += *vy_ptr;
            }

            *vy_ptr = MAX( y_velocity, -CUBEPAGE_FLING_MAX_VELOCITY );
        }
        else
        {
            *vy_ptr = 0;
        }

        result = TRUE;
    }

    s_crystalcube_velocity_tracker.item_num = 0;

    return result;
}


LOCAL BOOLEAN s_dualbuff_exit = FALSE;
/*****************************************************************************/
// 	Description : MMIAPICCUBE_DualBuff_Init
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC DUALBUFF_INFO_T*  MMIAPICCUBE_DualBuff_Init(uint8* buffer1,uint8* buffer2)
{
    DUALBUFF_INFO_T* dbuff_info_ptr;

    dbuff_info_ptr = SCI_ALLOC_APP(sizeof(DUALBUFF_INFO_T));

    SCI_MEMSET(dbuff_info_ptr,0,sizeof(DUALBUFF_INFO_T));

    dbuff_info_ptr->read_idx = 1;
    dbuff_info_ptr->write_idx = 1;

    dbuff_info_ptr->buffer[0] = buffer1;
    dbuff_info_ptr->buffer[1] = buffer2;

    s_dualbuff_exit = FALSE;

    return dbuff_info_ptr;
}
/*****************************************************************************/
// 	Description : MMIAPICCUBE_DualBuff_Reset
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPICCUBE_DualBuff_Reset(DUALBUFF_INFO_T* dbuff_info_ptr,uint8* buffer1,uint8* buffer2)
{

    dbuff_info_ptr->read_idx = 1;
    dbuff_info_ptr->write_idx = 1;

    dbuff_info_ptr->buffer[0] = buffer1;
    dbuff_info_ptr->buffer[1] = buffer2;

    s_dualbuff_exit = FALSE;
    
}
/*****************************************************************************/
// 	Description : MMIAPICCUBE_DualBuff_GetReadBuf
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPICCUBE_DualBuff_GetReadBuf(DUALBUFF_INFO_T* dbuff_info_ptr)
{
    uint8* ret_buff;
    
    while ((dbuff_info_ptr->read_idx == dbuff_info_ptr->write_idx) && (!s_dualbuff_exit))  { SCI_Sleep(1);};

    if (s_dualbuff_exit) return PNULL;

    ret_buff = dbuff_info_ptr->buffer[dbuff_info_ptr->read_idx];
    
    dbuff_info_ptr->read_idx = (dbuff_info_ptr->read_idx)? 0:1;

    return ret_buff;
}
/*****************************************************************************/
// 	Description : DualBuff_GetWriteBuf
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint8* DualBuff_GetWriteBuf(DUALBUFF_INFO_T* dbuff_info_ptr)
{
    uint8* ret_buff;
    uint8 write_idx;

    write_idx = (dbuff_info_ptr->write_idx)? 0:1;
    
    while ((write_idx == dbuff_info_ptr->read_idx) && (!s_dualbuff_exit))  {SCI_Sleep(1);};

    if (s_dualbuff_exit) return PNULL;

    ret_buff = dbuff_info_ptr->buffer[dbuff_info_ptr->write_idx];
    
    dbuff_info_ptr->write_idx = write_idx;

    return ret_buff;    
}
/*****************************************************************************/
// 	Description : MMIAPICCUBE_DualBuff_Destroy
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_DualBuff_Destroy(DUALBUFF_INFO_T* dbuff_info_ptr)
{
    SCI_FREE(dbuff_info_ptr);
    s_dualbuff_exit = TRUE;
}
/*****************************************************************************/
// 	Description : DualBuff_Exit
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void DualBuff_Exit(void)
{
    s_dualbuff_exit = TRUE;
    SCI_Sleep(10);
}
//设置需要计算的四个面  当前为N，其他三个面为N-1 N+1 N+2
LOCAL void SetFourCalcPage(MMICRYSTALCUBE_PARAM_T* param)
{
#if 1
    uint16 n0 = 0;
    uint16 n1 = 0;
    uint16 n2 = 0;
    uint16 n3 = 0;
    int16 temp_angle = CRYSTALCUBE_CIRCLE_ANGLE/MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM;
    int16 angle = (*param).s3d_geometry.angle;
    static S3D_ROTATION_IMG_T 	s3d_img[4];
    
    //角度限制在0--7200
    if (angle < 0)
    {
        angle  += CRYSTALCUBE_CIRCLE_ANGLE;
    }
    else if (angle  >= CRYSTALCUBE_CIRCLE_ANGLE)
    {
        angle  -= CRYSTALCUBE_CIRCLE_ANGLE;
    } 

    n0 = (angle/temp_angle)%MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM;
    n1 = (n0+1)%MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM;
    n2 = (n1+1)%MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM;
    n3 = (n0-1+MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM)%MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM;
    
    memcpy(&s3d_img[0],&(*param).s3d_rect[n0],sizeof(S3D_ROTATION_IMG_T));
    memcpy(&s3d_img[1],&(*param).s3d_rect[n1],sizeof(S3D_ROTATION_IMG_T));
    memcpy(&s3d_img[2],&(*param).s3d_rect[n2],sizeof(S3D_ROTATION_IMG_T));
    memcpy(&s3d_img[3],&(*param).s3d_rect[n3],sizeof(S3D_ROTATION_IMG_T));

    (*param).s3d_cube.s3d_img_ptr = (S3D_ROTATION_IMG_T (*)[4])s3d_img;
#endif
}

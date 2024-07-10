/*****************************************************************************
** File Name:      mmi_livewallpaper.c                                    *
** Author:          arvin.wu                                                *
** Date:           01/08/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  08/2011      Arvin.wu            Create                                  *
******************************************************************************/

#ifndef _MMI_LIVEWALLPAPER_C_
#define _MMI_LIVEWALLPAPER_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_livewallpaper_trc.h"
#include "mmilivewallpaper_export.h"
#include "mmi_utility.h"
#include "mmi_event_api.h"
#include "mmiset_export.h"
#include "mmi_resource.h"
//#include "mmise.h"  //not use asyn task to calculate frame,use timer;
#include "mmiset_export.h"
#include "mmi_wallpaper_export.h"
#if 0//def SLIDESHOW_WALLPAPER_SUPPORT
#include "mmi_livewallpaper_slideshow.h"
#endif
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 #ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif
#define LIVEWALLPAPER_POOL_NAME               "livewallpaper pool"
#define INVALID_MEM_HANDLE  0
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
                
/*typedef a enum to control the livewallpaper state*/
typedef  enum
{
    E_LIVE_WALLPAPER_STOP,
    E_LIVE_WALLPAPER_PLAY
}E_LIVE_WALLPAPER_STATE;         

typedef struct _LW_TYPE_INFO_
{
    uint32  type;
    const char* type_name_ptr;
    uint8   res_type; // 0, T flash;1, rom
}LW_TYPE_INFO;

#undef LW_DEF
#define LW_DEF(lw_type, lw_type_name, lw_res_type)  {lw_type,lw_type_name,lw_res_type},
const LW_TYPE_INFO   c_s_type_info_array[] = 
{
#include "mmi_livewallpaper.def"
};
#undef LW_DEF

typedef struct
{
    MMI_WIN_ID_T     win_id;    
    GUI_LCD_DEV_INFO layer_dev_info_S;  //Temp wallpaper layer S, for static image bg.
  
    GUI_COLOR_T *scr_buf_ptr_S;        //Point to layer buffer of "layer_dev_info_S"
    
    BOOLEAN is_initialized;
    /*存储当前实例的指针*/
    MMI_LIVE_WALLPAPER_ITEM_INFO_T *current_instance_ptr;

    E_LIVE_WALLPAPER_STATE live_wallpaper_current_state;//add a tag to control the play or stop.It 's to avoid the instance free error.
    

    uint32 callback_timer_id;//通过Timer方式控制Callback

    LW_CALLBACK callback;//层的callback函数
    uint32   param;//参数
}LIVE_WALLPAPER_DATA_T;
UILAYER_MEM_FUNC_T uilayer_mem_func = 
{
	MMIAPILIVEWALLPAPER_AllocInBlock,
	MMIAPILIVEWALLPAPER_FreeInBlock,
};
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*这个数组从实例配置文件引用过来的*/
extern const MMI_LIVE_WALLPAPER_ITEM_INFO_T *g_livewallpaper_item_table[MMI_LIVE_WALLPAPER_TYPE_MAX];

LOCAL LIVE_WALLPAPER_DATA_T   s_wallpaper_data    = {0}; 
LOCAL uint8 s_wallpaper_drop_frame = 0;
MPOOL_HANDLE s_livewallpaper_mem_handle = INVALID_MEM_HANDLE;
LOCAL void*  s_livewallpaperblock_buffer_ptr=PNULL;
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
LOCAL BOOLEAN s_is_release_resource_at_losefocus = TRUE;
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*
    设置播放状态，这里做了一个判断，正在播放就不重复播放，

    避免实例中的不规范操作
*/
#define LIVEWALLPAPER_ENTER_PLAY \
            do{\
                    if(E_LIVE_WALLPAPER_PLAY != s_wallpaper_data.live_wallpaper_current_state)\
                    {\
                        s_wallpaper_data.live_wallpaper_current_state = E_LIVE_WALLPAPER_PLAY;\
                    }\
                    else\
                    {\
                        return FALSE;\
                    }\
            }while(0)

/*
    设置停止状态，这里做了一个判断，不播放就不停止，

    避免实例中的不规范操作
*/
#define LIVEWALLPAPER_ENTER_STOP \
            do{\
                    if(E_LIVE_WALLPAPER_PLAY == s_wallpaper_data.live_wallpaper_current_state)\
                    {\
                        s_wallpaper_data.live_wallpaper_current_state = E_LIVE_WALLPAPER_STOP;\
                    }\
                    else\
                    {\
                        return;\
                    }\
            }while(0)

#ifdef WIN32
#define LIVEWALLPAPER_TIMEOUT  (1)
#else
#define LIVEWALLPAPER_TIMEOUT  (1)
#endif
#define LIVEWALLPAPER_DROP_FRAME_COUNT (3)
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
                    
/*****************************************************************************/
//  Description : 获取当前动态墙纸的节点
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceNodeInit(void);
/*****************************************************************************/
//  Description : 框架初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperFrameWorkInit(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 获得实例的宽高
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:只要有一个入参不为空，就会计算出来
/*****************************************************************************/
LOCAL void LivewallpaperInstanceGetBgWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);
/*****************************************************************************/
//  Description : 框架销毁函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:将框架数据的处理跟实例数据剥离
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperFrameWorkDestroy(void);
/*****************************************************************************/
//  Description : create temp  layer
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateTempLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer_dev_info_ptr,uint16 width,uint16 height,BOOLEAN is_static);
/*****************************************************************************/
//  Description : 实例初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceInit(void);

/*****************************************************************************/
//  Description : 实例播放初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceDynamicConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 实例销毁
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperInstanceDestroy(void);
/*****************************************************************************/
//  Description : 交互数据处理
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得当前动态壁纸设置的类型，从NV中读取
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LivewallpaperGetCurrentInstanteType(void);

/*****************************************************************************/
//  Description : 获得当前动态壁纸实例的函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:请使用实例时都调用这个函数，不直接调用局部变量，便于管理
/*****************************************************************************/
LOCAL MMI_LIVE_WALLPAPER_ITEM_INFO_T *LivewallpaperGetCurrentInstante(void);
/*****************************************************************************/
//  Description : 初始化原始数据
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN LivewallpaperSourceDataInit(MMI_WIN_ID_T win_id);

 /*****************************************************************************/
//  Description : 开始算法Task
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN LivewallpaperStartCalculate(void);

/*****************************************************************************/
//  Description : 停止定时器函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperStopTimer(void);
/*****************************************************************************/
//  Description : 创建定时器函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperCreateTimer(void);
/*****************************************************************************/
//  Description : 重建定时器
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperReCreateTimer(void);
/*****************************************************************************/
//  Description : 在开始的时候首先计算第一帧
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperCalculateOneFrame(BOOLEAN is_lose_focus);
/*****************************************************************************/
//  Description : 算法CallBack函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:由这个函数路由到具体实例的算法
/*****************************************************************************/
LOCAL void LivewallpaperCalculateCallBack( uint8  timer_id,uint32 param);

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
/*****************************************************************************/
//  Description : LivewallpaperInitOutsideRes
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInitOutsideRes(void);
/*****************************************************************************/
//  Description : LivewallpaperUnInitOutsideRes
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperUnInitOutsideRes(void);
#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*******************************************************************/
//  Interface:		LivewallpaperInitHandle
//  Description : 	LivewallpaperInitHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN LivewallpaperInitHandle(uint32 type)
{
    BOOLEAN bRet = TRUE;
    
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
    if(type < MMI_LIVE_WALLPAPER_WRE_TYPE_BASE)
    {
        uint32 i= 0;
        
        for(i = 0; i < ARRAY_SIZE(c_s_type_info_array); i++)
        {
            if(type == c_s_type_info_array[i].type && 0 == c_s_type_info_array[i].res_type)
            {
                if (!MMI_IsLivewallpaperResExist(c_s_type_info_array[i].type_name_ptr))
                {
                    bRet = FALSE;
                }
                else
                {
                    MMI_LivewallpaperInitHandle(c_s_type_info_array[i].type_name_ptr);
                }               
                break;
            }
        }
    }
#endif
    
    return bRet;
}
/*****************************************************************************/
//  Description : 获取当前动态墙纸的节点
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceNodeInit(void)
{
    uint32 type = 0;
    uint32 i = 0;
    uint32 node_count = ARRAY_SIZE(g_livewallpaper_item_table);
    //LIVE_WALLPAPER_INFO wallpaper_info_ptr = {0};
    
    /*获取当前动态壁纸的类型*/
    type = LivewallpaperGetCurrentInstanteType();
    if (!LivewallpaperInitHandle(type))
    {
        return FALSE;
    }
    
    /*清空当前动态壁纸的指针*/
    s_wallpaper_data.current_instance_ptr = PNULL;
    
    /*初始化获得当前类型的动态壁纸(默认动态壁纸)，保存指针地址*/
    if(type < MMI_LIVE_WALLPAPER_WRE_TYPE_BASE)
    {
        for(i = 0;i < node_count;i++)
        {
            if(g_livewallpaper_item_table[i]->GetType  && type == g_livewallpaper_item_table[i]->GetType())
            {

                /*找到就退出来*/
                    s_wallpaper_data.current_instance_ptr = (MMI_LIVE_WALLPAPER_ITEM_INFO_T*)g_livewallpaper_item_table[i];
                    return TRUE;
            }
        }
    }
    else
    {
        /*提供给WRE动态壁纸扩展，目前不使用*/
        
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : 获得实例的宽高
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:只要有一个入参不为空，就会计算出来
/*****************************************************************************/
LOCAL void LivewallpaperInstanceGetBgWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    uint16 bg_width = 0;
    uint16 bg_height = 0;

    if(PNULL == width_ptr && PNULL == height_ptr)
    {
        return;
    }
    
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&bg_width,&bg_height);
    if(s_wallpaper_data.current_instance_ptr)
    {
        if(s_wallpaper_data.current_instance_ptr->GetWidthAndHeight)
        {
            s_wallpaper_data.current_instance_ptr->GetWidthAndHeight(&bg_width,&bg_height);
        }

        if(0 == bg_width || 0 == bg_height)
        {
            //SCI_TRACE_LOW:"LivewallpaperInstanceGetBgWidthAndHeight bg_width:%d,bg_height:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_327_112_2_18_2_31_48_0,(uint8*)"dd",bg_width,bg_height);
            GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&bg_width,&bg_height);
        }
    }
    
    if(PNULL != width_ptr)
    {
        *width_ptr = bg_width;
    }
    
    if(PNULL != height_ptr)
    {
        *height_ptr = bg_height;
    }
}
/*****************************************************************************/
//  Description : 框架初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:主要是准备BUFFER
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperFrameWorkInit(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    

    if(!s_wallpaper_data.is_initialized)
    {  
        /*根据当前的实例创建源数据buf*/
        LivewallpaperInstanceGetBgWidthAndHeight(&bg_width,&bg_height);
        if(!CreateTempLayer(win_id, &s_wallpaper_data.layer_dev_info_S, bg_width, bg_height,TRUE))
        {
            UILAYER_ReleaseLayer(&s_wallpaper_data.layer_dev_info_S);
            s_wallpaper_data.layer_dev_info_S.block_id = UILAYER_NULL_HANDLE;
            return FALSE;
        }
        s_wallpaper_data.scr_buf_ptr_S = (GUI_COLOR_T *)UILAYER_GetLayerBufferPtr(&s_wallpaper_data.layer_dev_info_S);
        if(PNULL == s_wallpaper_data.scr_buf_ptr_S)
        {
            UILAYER_ReleaseLayer(&s_wallpaper_data.layer_dev_info_S);
            s_wallpaper_data.layer_dev_info_S.block_id = UILAYER_NULL_HANDLE;
            return FALSE;
        }
        
        s_wallpaper_data.is_initialized = TRUE;
        b_result = TRUE;
    }
    s_wallpaper_data.win_id = win_id;

    return b_result;
}

/*****************************************************************************/
//  Description : get s_wallpaper_data.layer_dev_info_S
//  Global resource dependence : 
//  Author:xiaohua.Wu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO* MMILIVEWALLPAPER_GetSourceDevPtr()
{
    return &s_wallpaper_data.layer_dev_info_S;
}
/*****************************************************************************/
//  Description : 框架销毁函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:将框架数据的处理跟实例数据剥离
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperFrameWorkDestroy(void)
{
    if(s_wallpaper_data.is_initialized)
    {
        UILAYER_RELEASELAYER(&s_wallpaper_data.layer_dev_info_S);   /*lint !e506 !e774*/
        s_wallpaper_data.scr_buf_ptr_S = PNULL; 
        s_wallpaper_data.is_initialized = FALSE;
        return TRUE;
    }	
    return FALSE;
}
/*****************************************************************************/
//  Description : create temp  layer
//  Global resource dependence : 
//  Author:arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  CreateTempLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO *layer_dev_info_ptr,uint16 width,uint16 height,BOOLEAN is_static)
{
    GUI_LCD_DEV_INFO widget_layer_handle = {0};
    UILAYER_CREATE_T create_info = {0};   
    BOOLEAN b_result = FALSE;
    
    if (PNULL == layer_dev_info_ptr)
    {
        return b_result;
    }
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = TRUE;
    create_info.is_static_layer = is_static;   
    //create_info.func_ptr = &uilayer_mem_func; //To reduce size of LIVEWALLLPAPER POOL, allocate Background Picture layer's mem from system heap, not from LIVEWALLLPAPER POOL;

    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_info, &widget_layer_handle))
    {
        return b_result;
    }

    UILAYER_SetLayerColorKey(&widget_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    *layer_dev_info_ptr = widget_layer_handle;
    if (UILAYER_IsLayerActive(&widget_layer_handle))
    {
        UILAYER_Clear(&widget_layer_handle);
    }
    b_result = TRUE;
    
    return b_result;
}
/*****************************************************************************/
//  Description : 实例初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/ 
LOCAL BOOLEAN LivewallpaperInstanceInit(void)
{
    BOOLEAN b_result = TRUE;
    
    /*执行动态壁纸实例的初始化程序*/
    if(PNULL != s_wallpaper_data.current_instance_ptr)
    {
        if(s_wallpaper_data.current_instance_ptr->Init)
        {
            b_result = s_wallpaper_data.current_instance_ptr->Init();
        }
        else
        {
            //SCI_TRACE_LOW:"LiveWallPaperFrame:There's no live wallpaper init funtion!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_448_112_2_18_2_31_48_1,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LiveWallPaperFrame:Can't find the valid live wallpaper!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_453_112_2_18_2_31_48_2,(uint8*)"");
    }
    return b_result;
}
/*****************************************************************************/
//  Description : 实例播放初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceDynamicConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    MMI_LIVE_WALLPAPER_ITEM_INFO_T *current_wallpaper_instance_ptr = PNULL;
    
    current_wallpaper_instance_ptr = LivewallpaperGetCurrentInstante();
    
    if(PNULL != current_wallpaper_instance_ptr)
    {
        if(current_wallpaper_instance_ptr->DynamicConstruct)
        {
            b_result = current_wallpaper_instance_ptr->DynamicConstruct(win_id);
        }
        else
        {
            //SCI_TRACE_LOW:"LiveWallPaperFrame:There's no live wallpaper DynamicConstruct funtion!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_474_112_2_18_2_31_49_3,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LiveWallPaperFrame:Play Init need instance ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_479_112_2_18_2_31_49_4,(uint8*)"");
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 实例销毁
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperInstanceDestroy(void)
{

    MMI_LIVE_WALLPAPER_ITEM_INFO_T *current_wallpaper_instance_ptr = PNULL;
    current_wallpaper_instance_ptr = LivewallpaperGetCurrentInstante();
    
    if(PNULL != current_wallpaper_instance_ptr)
    {
        if(current_wallpaper_instance_ptr->DynamicDestruct)
        {
            current_wallpaper_instance_ptr->DynamicDestruct();  
        }
        else
        {
            //SCI_TRACE_LOW:"LiveWallPaperFrame:There's no live wallpaper Destroy funtion!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_501_112_2_18_2_31_49_5,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LiveWallPaperFrame:Destroy need instance ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_506_112_2_18_2_31_49_6,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : 交互数据处理
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInstanceHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_LIVE_WALLPAPER_ITEM_INFO_T *current_wallpaper_instance_ptr = PNULL;
    BOOLEAN b_result = FALSE;
    
    current_wallpaper_instance_ptr = LivewallpaperGetCurrentInstante();
    
    if(PNULL != current_wallpaper_instance_ptr)
    {
        if(current_wallpaper_instance_ptr->HandleEvent)
        {
            b_result = current_wallpaper_instance_ptr->HandleEvent(win_id,msg_id,param);
        }
        else
        {
            //SCI_TRACE_LOW:"LiveWallPaperFrame:There's no live wallpaper HandleEvent funtion!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_528_112_2_18_2_31_49_7,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LiveWallPaperFrame:HandleEvent need instance ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_533_112_2_18_2_31_49_8,(uint8*)"");
    }
    
    return b_result;
}
/*****************************************************************************/
//  Description : 获得当前动态壁纸设置的类型，从NV中读取
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LivewallpaperGetCurrentInstanteType(void)
{
#if 0    
    uint32 livewallpaper_type = MMI_LIVE_WALLPAPER_TYPE_BASE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMILIVEWALLPAPER_TYPE_SAVER, &livewallpaper_type, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMILIVEWALLPAPER_TYPE_SAVER, &livewallpaper_type);        
    }

    return livewallpaper_type;
#else
    MMISET_WALLPAPER_SET_T wallpaper = {0};
    uint32 livewallpaper_type = 0;
    MMIAPISET_GetWallpaperSetInfo(&wallpaper);
    if(MMISET_WALLPAPER_LIVE == wallpaper.wallpaper_type)
    {
        livewallpaper_type = (uint32)wallpaper.wallpaper_id;
    }
    return livewallpaper_type;
#endif
}
/*****************************************************************************/
//  Description : 获得当前动态壁纸设置的类型，从NV中读取
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_LIVE_WALLPAPER_ITEM_INFO_T *LivewallpaperGetCurrentInstante(void)
{
    return s_wallpaper_data.current_instance_ptr;
}
/*****************************************************************************/
//  Description : 初始化原始数据
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN LivewallpaperSourceDataInit(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    if(s_wallpaper_data.is_initialized)
    {
        if(s_wallpaper_data.current_instance_ptr && UILAYER_IsLayerActive(&s_wallpaper_data.layer_dev_info_S))
        {
            /*如果需要画BG，则画*/
            if(s_wallpaper_data.current_instance_ptr->DrawBg)
            {
                b_result = s_wallpaper_data.current_instance_ptr->DrawBg(win_id,&s_wallpaper_data.layer_dev_info_S);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"LivewallpaperSourceDataInit Faild:current_instance_ptr:%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_593_112_2_18_2_31_49_9,(uint8*)"d",s_wallpaper_data.current_instance_ptr);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LivewallpaperSourceDataInit Faild:Frame is not init!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_598_112_2_18_2_31_49_10,(uint8*)"");
    }
    return b_result;
}
/*****************************************************************************/
//  Description : 开始算法Task
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN LivewallpaperStartCalculate(void)
{
    LW_FG_INFO lw_fg_info = {0};
    
    if(s_wallpaper_data.is_initialized && s_wallpaper_data.win_id == MMK_GetFocusWinId())
    {
        // -3- 合成
        MMIAPILIVEWALLPAPER_GetDisplayInfo(s_wallpaper_data.win_id,&lw_fg_info);
        if(UILAYER_IsLayerActive(&lw_fg_info.disp_dev_info))
        {
            LivewallpaperCalculateOneFrame(FALSE);
            LivewallpaperCreateTimer();
            return TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LivewallpaperStartCalculate Faild:is_initialized:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_622_112_2_18_2_31_49_11,(uint8*)"d",s_wallpaper_data.is_initialized);
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : 停止定时器函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperStopTimer(void)
{
    MMK_StopTimer(s_wallpaper_data.callback_timer_id);
    s_wallpaper_data.callback_timer_id = 0;
}

/*****************************************************************************/
//  Description : 创建定时器函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperCreateTimer(void)
{
#ifdef MMI_PDA_SUPPORT
    if(MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()&&(!MMITHEME_IsIstyle()))
    {
    
    }
    else
#endif   
    {
        uint32 time_out = LIVEWALLPAPER_TIMEOUT;
        
       
        if(PNULL!=s_wallpaper_data.current_instance_ptr->GetTimerTimeOut)
        {
            time_out = s_wallpaper_data.current_instance_ptr->GetTimerTimeOut();
        }
        
        if(0 != s_wallpaper_data.callback_timer_id)
        {
            LivewallpaperStopTimer();
        }
        s_wallpaper_data.callback_timer_id = MMK_CreateTimerCallback(
                                        time_out,
                                        (MMI_TIMER_FUNC)LivewallpaperCalculateCallBack,
                                        (uint32)0,
                                        FALSE);

    }
}

/*****************************************************************************/
//  Description : 重建定时器
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperReCreateTimer(void)
{
#ifdef MMI_PDA_SUPPORT
    if(MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()&&(!MMITHEME_IsIstyle()))
    {
        return FALSE;
    }
    else
#endif   
    {
        if(E_LIVE_WALLPAPER_PLAY != s_wallpaper_data.live_wallpaper_current_state)
        {
            return FALSE;
        }

        LivewallpaperCreateTimer();

        return TRUE;
   }

}
//测试当前坐标信息是否出错
//#define LW_NEW_TEST
#ifdef LW_NEW_TEST
#define LW_TEST_STRING_MAX  (50)
#define LW_TEST_STRING_POS_TOP (200)
#define LW_TEST_FONT_SIZE  (10)
#define LW_TEST_FONT_COLOR (0xFFFF)
/*****************************************************************************/
//  Description : 打印当前的坐标信息
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperPrintPos(LW_CAL_PARAM *lw_cal_param)
{
    uint8 temp_str[LW_TEST_STRING_MAX + 1] = {0}; 
    wchar temp_wstr[LW_TEST_STRING_MAX + 1] = {0};
    uint16 str_len = 0;
    GUI_POINT_T point = {0};
    GUI_RECT_T  rect = MMITHEME_GetFullScreenRect();
    uint32 i = 0;
    MMI_STRING_T temp_string = {0};
    GUISTR_STATE_T temp_state = GUISTR_STATE_INVERSE | GUISTR_STATE_SINGLE_LINE;
    GUISTR_STYLE_T temp_style = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};

    rect.top = LW_TEST_STRING_POS_TOP;
    temp_string.wstr_ptr = temp_wstr;
    temp_style.font = LW_TEST_FONT_SIZE;
    temp_style.font_color = LW_TEST_FONT_COLOR;
    _snprintf( (char*)temp_str, LW_TEST_STRING_MAX, "fg_pos_info:w:%d h:%d x:%d,y:%d \n",lw_cal_param->fg_pos_info.rc.right,lw_cal_param->fg_pos_info.rc.bottom,lw_cal_param->fg_pos_info.x,lw_cal_param->fg_pos_info.y);
    str_len = strlen( (char*)temp_str);
    MMI_STRNTOWSTR( temp_wstr, LW_TEST_STRING_MAX, temp_str, str_len, str_len );
    temp_string.wstr_len = str_len;
    GUISTR_DrawTextToLCDInRect(&lcd_dev, &rect, &rect, &temp_string, &temp_style, temp_state, -1 );

}
#endif

/*****************************************************************************/
//  Description:  注册给层的callback函数
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void  LivewallpaperLayerCallBack(uint8* buffer, 
                                       UILAYER_INFO_T *layer_info_ptr,uint32 param)
{
    LW_BUF_INFO buf_info = {0};
    
    if(PNULL == buffer || PNULL == layer_info_ptr || PNULL == s_wallpaper_data.callback)
    {
        return;
    }

    buf_info.buf_ptr = (void*)buffer;
    buf_info.w = layer_info_ptr->layer_width;
    buf_info.h = layer_info_ptr->layer_height;
    buf_info.data_type = layer_info_ptr->data_type;
    
    s_wallpaper_data.callback(buf_info,param);
}

/*****************************************************************************/
//  Description:  将壁纸应用注册的回调转化为UILAYER的回调
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void RegesterLayerCallBack(GUI_LCD_DEV_INFO const *dev_info_ptr,
                                                              LW_CALLBACK cb,
                                                              uint32 param)
{
    s_wallpaper_data.callback = PNULL;
    s_wallpaper_data.param = 0;
    if(PNULL != cb && PNULL != dev_info_ptr)
    {
        s_wallpaper_data.callback = cb;
        s_wallpaper_data.param = param;
        UILAYER_SetBlendCallback((GUI_LCD_DEV_INFO const *) dev_info_ptr, (UILAYER_BLEND_CALLBACK)LivewallpaperLayerCallBack,param);
    }
}
        
/*****************************************************************************/
//  Description : 在开始的时候首先计算第一帧
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperCalculateOneFrame(BOOLEAN is_lose_focus)
{
    uint16 width = 0;
    uint16 height = 0;
    GUI_LCD_DEV_INFO main_lcd_dev_info = {0};
    GUI_COLOR_T *dest_buf_ptr = PNULL;
    LW_FG_INFO lw_fg_info = {0};
    LW_CAL_PARAM lw_cal_param = {0};
    UILAYER_INFO_T dest_layer_info = {0};
    GUI_RECT_T s_wallpaper_rect ={0};
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID,&lcd_width,&lcd_height);
    s_wallpaper_rect.right= lcd_width-1;
    s_wallpaper_rect.bottom= lcd_height-1;
    
    if(s_wallpaper_data.current_instance_ptr && s_wallpaper_data.current_instance_ptr->Calculate)
    {
        MMIAPILIVEWALLPAPER_GetDisplayInfo(s_wallpaper_data.win_id,&lw_fg_info);


        /*如果失去焦点，上层窗体是非全屏窗体或者透明窗体，直接画主层*/
        if(is_lose_focus && PNULL != s_wallpaper_data.scr_buf_ptr_S)
        {
            dest_buf_ptr = (GUI_COLOR_T *)UILAYER_GetLayerBufferPtr(&main_lcd_dev_info);
            lw_fg_info.disp_dev_info = main_lcd_dev_info;
            
            UILAYER_GetLayerInfo(&lw_fg_info.disp_dev_info, &dest_layer_info);
            /*包装参数*/
            lw_cal_param.dest_buf_info.buf_ptr = dest_buf_ptr;//目标buffer
            lw_cal_param.dest_buf_info.data_type = UILAYER_GetLayerColorType(&lw_fg_info.disp_dev_info);//数据类型
            lw_cal_param.dest_buf_info.w = dest_layer_info.layer_width;//宽
            lw_cal_param.dest_buf_info.h = dest_layer_info.layer_height;//高
            lw_cal_param.fg_pos_info = lw_fg_info.lw_fg_pos_info;//前景的参数，前景的区域，以及当前的位置
            lw_cal_param.src_buf_info.buf_ptr = s_wallpaper_data.scr_buf_ptr_S;//背景源buffer
            LivewallpaperInstanceGetBgWidthAndHeight(&lw_cal_param.src_buf_info.w,&lw_cal_param.src_buf_info.h);//背景宽高
            lw_cal_param.reserve1 = (uint32)is_lose_focus;
            if(PNULL != lw_cal_param.dest_buf_info.buf_ptr)
            {
                /*发起计算*/
                s_wallpaper_data.current_instance_ptr->Calculate(&lw_cal_param);

                RegesterLayerCallBack(&lw_fg_info.disp_dev_info,lw_cal_param.out_param.callback,lw_cal_param.out_param.param);
                /*值脏*/
                MMITHEME_StoreUpdateRect(&lw_fg_info.disp_dev_info, s_wallpaper_rect);
             
            }
            else
            {
                //SCI_TRACE_LOW:"LivewallpaperCalculate Faild:dest buffer:%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_800_112_2_18_2_31_49_12,(uint8*)"d",lw_cal_param.dest_buf_info.buf_ptr);
            }
#ifdef LW_NEW_TEST
            LivewallpaperPrintPos(&lw_cal_param);
#endif
        }
        else 
        {
            if(UILAYER_IsLayerActive(&lw_fg_info.disp_dev_info) && PNULL != s_wallpaper_data.scr_buf_ptr_S)
            {
                LivewallpaperInstanceGetBgWidthAndHeight(&width,&height);
                UILAYER_GetLayerInfo(&lw_fg_info.disp_dev_info, &dest_layer_info);
                dest_buf_ptr = (GUI_COLOR_T *)UILAYER_GetLayerBufferPtr(&lw_fg_info.disp_dev_info);
                
                /*包装参数*/
                lw_cal_param.dest_buf_info.buf_ptr = dest_buf_ptr;
                lw_cal_param.dest_buf_info.data_type = UILAYER_GetLayerColorType(&lw_fg_info.disp_dev_info);
                lw_cal_param.dest_buf_info.w = dest_layer_info.layer_width;
                lw_cal_param.dest_buf_info.h = dest_layer_info.layer_height;
                lw_cal_param.fg_pos_info = lw_fg_info.lw_fg_pos_info;
                lw_cal_param.src_buf_info.buf_ptr = s_wallpaper_data.scr_buf_ptr_S;
                LivewallpaperInstanceGetBgWidthAndHeight(&lw_cal_param.src_buf_info.w,&lw_cal_param.src_buf_info.h);
                if(PNULL != lw_cal_param.dest_buf_info.buf_ptr)
                {
                    s_wallpaper_data.current_instance_ptr->Calculate(&lw_cal_param);
                    
                    RegesterLayerCallBack(&lw_fg_info.disp_dev_info,lw_cal_param.out_param.callback,lw_cal_param.out_param.param);
                    MMITHEME_StoreUpdateRect(&lw_fg_info.disp_dev_info, s_wallpaper_rect);

                }
#ifdef LW_NEW_TEST
                LivewallpaperPrintPos(&lw_cal_param);
#endif

                return TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"LivewallpaperCalculate Faild:src buffer:%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_837_112_2_18_2_31_49_13,(uint8*)"d",s_wallpaper_data.scr_buf_ptr_S);
            }
        }
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : 算法CallBack函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:由这个函数路由到具体实例的算法
/*****************************************************************************/
LOCAL void LivewallpaperCalculateCallBack( uint8  timer_id,uint32 param)
{
    static uint32 s_livewallpaper_ticks = 0;
    
    if(timer_id == s_wallpaper_data.callback_timer_id)
    {
        if(LivewallpaperCalculateOneFrame(FALSE))
        {
            LivewallpaperReCreateTimer();
            //SCI_TRACE_LOW:"LivewallpaperCalculate Time:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_856_112_2_18_2_31_49_14,(uint8*)"d",SCI_GetTickCount() - s_livewallpaper_ticks);
            s_livewallpaper_ticks = SCI_GetTickCount();
        }
    }
    else
    {
        //SCI_TRACE_LOW:"LivewallpaperCalculate Faild:the buffer is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_862_112_2_18_2_31_49_15,(uint8*)"");
    }
}

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
/*****************************************************************************/
//  Description : LivewallpaperInitOutsideRes
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LivewallpaperInitOutsideRes(void)
{   
    if(s_is_release_resource_at_losefocus)
    {
        if(!MMIRES_NorFileResInit())
        {
            return FALSE;
        }
        
        s_is_release_resource_at_losefocus =FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : LivewallpaperUnInitOutsideRes
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
LOCAL void LivewallpaperUnInitOutsideRes(void)
{  
    if(!s_is_release_resource_at_losefocus) //当前是否全屏
    {
        MMIRES_NorFileResUnInit();
        s_is_release_resource_at_losefocus =TRUE;
    }
}
#endif

//start mem pool mechanism;
/*****************************************************************************/
//  Description : init livewallpaper block memory 
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_InitBlock(uint32 block_size)
{
    BOOLEAN return_val = TRUE;
    
    uint32 block_buff_size = 0;
    APP_MEM_CFG_T    mem_cfg = {0};
    
    SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIAPILIVEWALLPAPER_InitBlock");
    
    if(INVALID_MEM_HANDLE != s_livewallpaper_mem_handle)
    {
       MMIAPILIVEWALLPAPER_DestroyBlock(); 
    }    
    
    s_livewallpaperblock_buffer_ptr = SCI_ALLOCA(block_size);
    if(PNULL == s_livewallpaperblock_buffer_ptr)
    {
        return FALSE;
    }
    block_buff_size = (uint32)block_size;  //return memory size.
    
    mem_cfg.pool_mode = MEM_POOL_RELEASE_MODE;
    
    s_livewallpaper_mem_handle = SCI_CreateMemPool(LIVEWALLPAPER_POOL_NAME, s_livewallpaperblock_buffer_ptr, block_buff_size, &mem_cfg);

    if(INVALID_MEM_HANDLE == s_livewallpaper_mem_handle)
    {
        SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIMMS_InitBlock call MMIAPIMEM_Config fail!");
        SCI_FREE(s_livewallpaperblock_buffer_ptr);
        return_val = FALSE;
    } 
    else
    {
        SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIAPILIVEWALLPAPER_InitBlock INVALID_MEM_HANDLE != s_livewallpaper_mem_handle, create successfully!");
    }

    return return_val;
}

/*****************************************************************************/
//  Description : livewallpaper alloc in block
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void *MMIAPILIVEWALLPAPER_AllocInBlock(uint32 size)
{
    return MMIAPILIVEWALLPAPER_AllocInBlockEx(size, __FILE__, __LINE__);
}
PUBLIC void *MMIAPILIVEWALLPAPER_AllocInBlockEx(
                                 uint32         size,
                                 const char     *file_ptr, 
                                 uint32         line
                                 )
{ 
    uint8 *new_ptr = PNULL; 
    uint32  real_size = size;
	
    if(size == 0)
    {
        SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIAPILIVEWALLPAPER_AllocInBlock error:size == 0");
        return PNULL;
    }
    
    SCI_TRACE_LOW("[MMILIVEWALLPAPER]: MMIAPILIVEWALLPAPER_AllocInBlock -malloc size= %d", real_size);
	
    if(s_livewallpaper_mem_handle != INVALID_MEM_HANDLE)
    {
		new_ptr = SCI_MPALLOC(real_size, s_livewallpaper_mem_handle);
    }
	
    if(new_ptr != PNULL)
    {
        SCI_MEMSET(new_ptr,0,size);
        return (void*)new_ptr;
    }
    else
    {
        SCI_TRACE_LOW("[BRW]error:malloc failed,size = %d",size);
        return PNULL;
    }	
}

/*****************************************************************************/
//  Description : destroy livewallpaper block memory 
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_DestroyBlock(void)
{
    
    if (PNULL != s_livewallpaper_mem_handle)
    {
        SCI_DeleteMemPool(s_livewallpaper_mem_handle,TRUE);
        SCI_FREE(s_livewallpaperblock_buffer_ptr);
        s_livewallpaper_mem_handle = PNULL;
    }
    else
    {
        SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIAPILIVEWALLPAPER_DestroyBlock, handle is null!");
    }
    
}

/*****************************************************************************/
//  Description : livewallpaper free in block
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPILIVEWALLPAPER_FreeInBlock(
                                void 		*ptr
                                )
{
    //uint32 flag = 0;
    void* real_ptr=PNULL;
	
    SCI_TRACE_LOW("[MMILIVEWALLPAPER]:MMIAPILIVEWALLPAPER_FreeInBlock ERR:Memory FREE In Block!");
	
    real_ptr = (void*)ptr;
	
    if(PNULL != real_ptr)
    {
		SCI_MPFREE(real_ptr);
    }   
}

//end mem pool mechanism;
/*****************************************************************************/
//  Description : 动态墙纸消息处理函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:            
            if(s_wallpaper_data.current_instance_ptr&&s_wallpaper_data.current_instance_ptr->NeglectInCondition)
            {
                if(s_wallpaper_data.current_instance_ptr->NeglectInCondition(msg_id))
                {
                    return;
                }
            }
            MMIAPILIVEWALLPAPER_Play(win_id);
            break;
        case MSG_BACKLIGHT_TURN_ON:
        case MSG_GET_FOCUS:
            if(s_wallpaper_data.current_instance_ptr&&s_wallpaper_data.current_instance_ptr->NeglectInCondition)
            {
                if(s_wallpaper_data.current_instance_ptr->NeglectInCondition(msg_id))
                {
                    return;
                }
            }
            if(E_LIVE_WALLPAPER_PLAY == s_wallpaper_data.live_wallpaper_current_state)
            {
                LivewallpaperStartCalculate();
            }
            else
            {
                MMIAPILIVEWALLPAPER_Stop();
                MMIAPILIVEWALLPAPER_Play(win_id);
            }
            break;
		case MSG_LCD_SWITCH:
            MMIAPILIVEWALLPAPER_Stop();
            MMIAPILIVEWALLPAPER_Destroy(FALSE);
            MMIAPILIVEWALLPAPER_Init();
            MMIAPILIVEWALLPAPER_Play(win_id);
            break;
        case MSG_BACKLIGHT_TURN_OFF:
        case MSG_LOSE_FOCUS:
            if(s_wallpaper_data.current_instance_ptr&&s_wallpaper_data.current_instance_ptr->NeglectInCondition)
            {
                if(s_wallpaper_data.current_instance_ptr->NeglectInCondition(msg_id))
                {
                    return;
                }
            }
            MMIAPILIVEWALLPAPER_Stop();
            break;
        case MSG_CLOSE_WINDOW:
            MMIAPILIVEWALLPAPER_Stop();
            break;    
        case MSG_PRE_FULL_PAINT:
            if(s_wallpaper_data.current_instance_ptr&&s_wallpaper_data.current_instance_ptr->NeglectInCondition)
            {
                if(s_wallpaper_data.current_instance_ptr->NeglectInCondition(msg_id))
                {
                    return;
                }
            }
            if(!MMK_IsFocusWin(win_id))
            {   
                MMIAPILIVEWALLPAPER_Play(win_id);
                LivewallpaperCalculateOneFrame(TRUE);
                MMIAPILIVEWALLPAPER_Stop();
            }
            break;
        case MSG_TIMER:
            if(s_wallpaper_data.callback_timer_id == *( GUI_TIMER_ID_T*)param
                &&(MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
                &&(!MMITHEME_IsIstyle())
                )
            {
            	SCI_TRACE_LOW("[NEST]  START CALC    TIME = %d",SCI_GetTickCount());
                LivewallpaperCalculateOneFrame(FALSE);
            }
            break;
        default:
            /*具体事例handle*/
            LivewallpaperInstanceHandleEvent(win_id,msg_id,param);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : mount callback
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void LwUdiskMountCallback(
                                MMIEVENT_UDISK_DEV_E device,
                                BOOLEAN mount
                                )
{
    if (DEV_UDISK == device)
    {
        //SCI_TRACE_LOW:"@@Livewallpaper UdiskMount:%d \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_923_112_2_18_2_31_49_16,(uint8*)"d", mount);
        
        if (mount)
        {
            MMIAPILIVEWALLPAPER_Stop();
            MMIAPILIVEWALLPAPER_Destroy(FALSE);
        }
        else
        {
            MMIAPILIVEWALLPAPER_Init();
        }

        MMIAPIIDLE_SetWallpaper(TRUE);
    }
}

/*****************************************************************************/
//  Description : LivewallpaperReleaseAtInitExceptionStageRestore
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:Release works when exception takes place at initial  stage, later,may restore livewalllpaper.
/*****************************************************************************/
LOCAL void LivewallpaperReleaseAtInitExceptionStageRestore(void)
{
    GUI_RECT_T bg_rect=MMITHEME_GetFullScreenRect();
    MMI_IMAGE_ID_T bg_img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());

    SCI_TRACE_LOW("LivewallpaperReleaseAtInitExceptionStageRestore: Initial Stage Fail, then release mem pool and set current_instance_ptr pnull.,may restore livewalllpaper!");

    MMIAPILIVEWALLPAPER_Stop();
    MMIAPILIVEWALLPAPER_Destroy(FALSE);

    GUIRES_DisplayImg(PNULL,
                                        &bg_rect,
                                        &bg_rect,
                                        MAIN_IDLE_WIN_ID,
                                        bg_img_id,
                                        MMITHEME_GetDefaultLcdDev());
}

/*****************************************************************************/
//  Description : 动态墙纸初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_Init(void)
{
    BOOLEAN b_result = TRUE;
    SCI_TRACE_LOW("MMIAPILIVEWALLPAPER_Init: Init entry!");   
    
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        if(!LivewallpaperInitOutsideRes())
        {
            return FALSE;
        }
        b_result = MMI_LivewallpaperResListInit();
#endif
        
        if(b_result)
        { 
        /*初始化动态墙纸当前的节点*/
        b_result = LivewallpaperInstanceNodeInit();
        }

        if(b_result)
        {          
            /*动态墙纸实例的本地变量的初始化*/
            b_result = LivewallpaperInstanceInit();
        }

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
            LivewallpaperUnInitOutsideRes();//before livewallpaper play,release 240K resource pool, save memory for DC 2 Million.--NEWMS00211604
#endif

        if(!b_result)
        {
            LivewallpaperReleaseAtInitExceptionStageRestore();
            return FALSE;
        }

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        MMIFW_SetUdiskMountListener(LwUdiskMountCallback, TRUE);
#endif
    }
    return b_result;
}

/*****************************************************************************/
//  Description :  动态墙纸播放
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_Play(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    LIVE_WALLPAPER_INFO wallpaper_info_ptr = {0};
    LIVEWALLPAPER_ENTER_PLAY;

    SCI_TRACE_LOW("MMIAPILIVEWALLPAPER_Play: Play entry!");

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        if(!LivewallpaperInitOutsideRes())
        {
            return FALSE;
        }
#endif

    UILAYER_SetMainLayerType(DATA_TYPE_RGB565);
    /*初始化动态墙纸当前的节点*/
    if(PNULL == s_wallpaper_data.current_instance_ptr)
    {
        b_result = LivewallpaperInstanceNodeInit();
        //LivewallpaperInstanceInit(win_id);
    }
	
    if(b_result&&(PNULL != s_wallpaper_data.current_instance_ptr))
    {
        if(PNULL != s_wallpaper_data.current_instance_ptr->GetThumbNailInfo)
        {
            s_wallpaper_data.current_instance_ptr->GetThumbNailInfo(&wallpaper_info_ptr.thumbnail_info);
        }
    }
    if(b_result&&wallpaper_info_ptr.thumbnail_info.instance_mem_pool_size>0)
    {
        BLKMEM_GarbageClean();
        b_result = MMIAPILIVEWALLPAPER_InitBlock(wallpaper_info_ptr.thumbnail_info.instance_mem_pool_size);
    }
    
    /*动态壁纸框架初始化*/
    if(b_result)
    {
        b_result = LivewallpaperFrameWorkInit(win_id);
    }      
        
    /*初始化播放数据*/
    if(b_result)
    {
        b_result = LivewallpaperInstanceDynamicConstruct(win_id);
    }
    
    /*动态墙纸实例数据源初始化*/
    if(b_result)
    {
        b_result = LivewallpaperSourceDataInit(win_id);
    }
    
    /*启动播放任务*/
    if(b_result)
    {
        b_result = LivewallpaperStartCalculate();
    }
    else  /*异常处理 , 释放, 设置静态墙纸*/
    {
        LivewallpaperReleaseAtInitExceptionStageRestore();  //must put it before LivewallpaperStartCalculate();due to state transfer;
        return FALSE;
    }
        
    return b_result;
}

/*****************************************************************************/
//  Description :  主动调用播放帧函数，通常使用主转场的过程中
//                     应用主动调用刷屏函数，导致背景不更新
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_PlayFrame(MMI_WIN_ID_T win_id)
{
    if(win_id == s_wallpaper_data.win_id)
    {
        if(E_LIVE_WALLPAPER_PLAY == s_wallpaper_data.live_wallpaper_current_state)
        {
            if(0 == s_wallpaper_drop_frame)
            {
                LivewallpaperCalculateOneFrame(TRUE);
            }
            s_wallpaper_drop_frame = ((s_wallpaper_drop_frame+1)%LIVEWALLPAPER_DROP_FRAME_COUNT);
        }
    }
}
/*****************************************************************************/
//  Description :  动态墙纸停止
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:next_win_id 传入为0，不处理
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_Stop(void)
{
    LIVEWALLPAPER_ENTER_STOP;//如果未播放，就返回，请不要在此句之前做函数调用
    SCI_TRACE_LOW("MMIAPILIVEWALLPAPER_Stop: Stop entry!");   
    
    UILAYER_SetMainLayerType(MMITHEME_GetMainLayerDefaultType());
    /*销毁当前动态墙纸数据*/
    LivewallpaperInstanceDestroy();
    LivewallpaperStopTimer();
    
    /*销毁框架数据*/
    LivewallpaperFrameWorkDestroy();

    /*销毁为每个实例申请的内存池*/
    if(s_livewallpaper_mem_handle)
    {
        MMIAPILIVEWALLPAPER_DestroyBlock(); //we need to release mem pool at stop function, according to all places at stop in handevent;
	s_livewallpaper_mem_handle = INVALID_MEM_HANDLE;
    }

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
            LivewallpaperUnInitOutsideRes();
#endif   
}

/*****************************************************************************/
//  Description :  动态墙纸销毁
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_Destroy(BOOLEAN is_clean)
{
    SCI_TRACE_LOW("MMIAPILIVEWALLPAPER_Destroy: Destroy entry!");  
    /*
        增加对应的销毁初始化对应的销毁函数，适应平台的特效架构
        保持连续性质    
    */
    if(s_wallpaper_data.current_instance_ptr && s_wallpaper_data.current_instance_ptr->DeInit)
    {
        s_wallpaper_data.current_instance_ptr->DeInit();
    }
	
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
    if (PNULL != s_wallpaper_data.current_instance_ptr)
    {      
        MMI_LivewallpaperCleanUp();
        MMI_DestroyLivewallpaperResList();
        if (is_clean)
        {
            MMIFW_SetUdiskMountListener(LwUdiskMountCallback, FALSE);
        }
        LivewallpaperUnInitOutsideRes();
    }
#endif

    SCI_MEMSET(&s_wallpaper_data, 0, sizeof(s_wallpaper_data));  
}
/*****************************************************************************/
//  Description :  初始化动态壁纸模块
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_InitModule(void)
{
    MMLIVEWALLPAPER_RegNv();
}

/*****************************************************************************/
//  Description :   设置动态墙纸的类新
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_SetLiveWallPaperType(uint16 livewallpaper_index)
{
    uint32 type = 0;
    uint32 node_count = 0;

    node_count = ARRAY_SIZE(g_livewallpaper_item_table);

    if(livewallpaper_index < node_count && g_livewallpaper_item_table[livewallpaper_index]->GetType)
    {
        type = g_livewallpaper_item_table[livewallpaper_index]->GetType();
    }
    else
    {
        /*如果是WRE在此处理*/
        if(g_livewallpaper_item_table[0]->GetType)
        {
            type = g_livewallpaper_item_table[0]->GetType();
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPILIVEWALLPAPER_SetLiveWallPaperType error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_1096_112_2_18_2_31_50_17,(uint8*)"");
            return FALSE;
        }
    }
    MMINV_WRITE(MMILIVEWALLPAPER_TYPE_SAVER, &type);        
    return TRUE;
}
/*****************************************************************************/
//  Description :   获得当前墙纸的索引值
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPILIVEWALLPAPER_GetCurrentLiveWallPaperIndex(void)
{
    uint32 type = 0;
    uint32 i = 0;
    uint32 node_count = ARRAY_SIZE(g_livewallpaper_item_table);

    /*获取当前动态壁纸的类型*/
    type = LivewallpaperGetCurrentInstanteType();
    
    /*初始化获得当前类型的动态壁纸(默认动态壁纸)，保存指针地址*/
    if(type < MMI_LIVE_WALLPAPER_WRE_TYPE_BASE)
    {
        for(i = 0;i < node_count;i++)
        {
            if(g_livewallpaper_item_table[i]->GetType && type == g_livewallpaper_item_table[i]->GetType())
            {
                /*找到就退出来*/
                return i;
            }
        }
    }

    return MMI_LIVEWALLPAPER_INVALID_INDEX;
}
/*****************************************************************************/
//  Description :  获得动态壁纸信息
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_GetAllWallpapersInfo(LIVE_WALLPAPER_INFO *wallpaper_info_ptr,uint16 wallpaper_index)
{
    BOOLEAN bRet = FALSE;
    uint32 node_count = ARRAY_SIZE(g_livewallpaper_item_table);
    
    if(wallpaper_index < node_count)
    {
        if(PNULL != wallpaper_info_ptr)
        {
            if(g_livewallpaper_item_table[wallpaper_index]->GetThumbNailInfo)
            {
                g_livewallpaper_item_table[wallpaper_index]->GetThumbNailInfo(&wallpaper_info_ptr->thumbnail_info);
            }
            if(g_livewallpaper_item_table[wallpaper_index]->GetType)
            {
                wallpaper_info_ptr->type = g_livewallpaper_item_table[wallpaper_index]->GetType();
            }
            wallpaper_info_ptr->type_name_ptr = c_s_type_info_array[wallpaper_index].type_name_ptr;
            //SCI_MEMCPY(wallpaper_info_ptr.thumbnail_info,&g_livewallpaper_item_table[wallpaper_index]->,sizeof(LIVE_WALLPAPER_INFO));
            wallpaper_info_ptr->res_type = c_s_type_info_array[wallpaper_index].res_type;

            if (0 == wallpaper_info_ptr->res_type)
            {
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
                if (MMI_IsLivewallpaperResExist(wallpaper_info_ptr->type_name_ptr))
                {
                    bRet = TRUE;
                }
#endif
            }
            else
            {
                bRet = TRUE;
            }
        }
    }
    
    return bRet;
}

/*****************************************************************************/
//  Description : 创建设置窗体
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_CreateSettingWindow(void)
{
    uint16 i = 0;
    OPTIONLIST *option_list = PNULL;
    
    i = MMIAPILIVEWALLPAPER_GetCurrentLiveWallPaperIndex(); 

    /*如果当前壁纸有设置选项则创建设置选项列表*/
    if(MMI_LIVEWALLPAPER_INVALID_INDEX != i && PNULL != g_livewallpaper_item_table[i]->GetOptionList)
    {
        option_list = g_livewallpaper_item_table[i]->GetOptionList();
        if(option_list)
        {
            MMI_CreateOptionWin(option_list);
        }
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description :  壁纸是否支持选项设置列表
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_IsNeedOptionMenu(void)
{
    uint16 i = 0;
    
    i = MMIAPILIVEWALLPAPER_GetCurrentLiveWallPaperIndex();

    if(MMI_LIVEWALLPAPER_INVALID_INDEX != i && PNULL != g_livewallpaper_item_table[i]->GetOptionList)
    {
        return (PNULL != g_livewallpaper_item_table[i]->GetOptionList());
    }
    return FALSE;
}
/*****************************************************************************/
//  Description :  层中使用内存池分配 释放的回调接口 ；
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
PUBLIC UILAYER_MEM_FUNC_T * MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(void)
{
    return &uilayer_mem_func;
}
/*****************************************************************************/
//  Description : 获取idle timer id
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_SaveTimerId(uint8 timer_id)
{
    s_wallpaper_data.callback_timer_id = timer_id;
}
#endif /*_MMI_LIVEWALLPAPER_C_*/

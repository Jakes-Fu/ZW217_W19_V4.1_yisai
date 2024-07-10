/*****************************************************************************
** File Name:      mmi_livewallpaper.h                                    *
** Author:         kyle.jin                                                *
** Date:           01/08/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  08/2011      Arvin.wu            Create                                  *
******************************************************************************/

#ifndef _MMI_LIVEWALLPAPER_H_
#define _MMI_LIVEWALLPAPER_H_
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mmi_wallpaper_export.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
//#include "mmiset_export.h"
#include "ui_layer.h"
#include "mmitheme_update.h"
#include "special_effect.h" 
#include "graphics_2d.h"
#include "mmi_livewallpaper_nv.h"
#include "guires.h"
#include "mmi_livewallpaper_option.h"
//#include "mmise.h"  //not use asyn task to calculate frame,use timer;
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                                                       */
/*---------------------------------------------------------------------------*/
#define MMI_LIVEWALLPAPER_INVALID_INDEX   (0xFFFF)

#define PIC_OFFS_WIDTH 30//bg picture offset width
#define LIVEWALLPAPER_Z_MAX (10*MMI_MAINSCREEN_WIDTH)
#define Z_ZOOMIN_MAX   (-1000)
#define Z_DELTA_MAX  (PIC_OFFS_WIDTH*LIVEWALLPAPER_Z_MAX/MMI_MAINSCREEN_WIDTH)
#define USE_GRAPH_SCALE
#define MMILIVEWALLPAPER_BLOCK_WATERMARK      (20*1024)
#define MMILIVEWALLPAPER_ALLOC(size)   MMIAPILIVEWALLPAPER_AllocInBlock(size)  //MMIAPILIVEWALLPAPER_AllocInBlock(size, __FILE__, __LINE__)
#define MMILIVEWALLPAPER_FREE(ptr)     (MMIAPILIVEWALLPAPER_FreeInBlock(ptr), ptr=PNULL)
/*---------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                                                      */
/*---------------------------------------------------------------------------*/
typedef struct
{
	 int32 a;
	 int32 r1;
	 int32 r2;
	 int32 x0;
	 int32 y0;
	 int32 p;
	 int32 a0;	 
}POP_INFO_T;  //for bee pop;

typedef struct
{
	int32 direction;// 1 C.W. -1 anti-C.W.
	int32 radius;// 4 bits prec
	COLOR32 color;
	IMGREF_POINT_3D_T rot_point;
	int32 rot_x;
	int32 rot_y;
	int32 angle;
	int32 vel_angle;
     int32 top_radius;
     int32 st_radius;
}MAGICPOP_INFO_T;

#define LW_DEF(lw_type, lw_type_name, lw_res_type)  lw_type,

typedef enum 
{
    MMI_LIVE_WALLPAPER_TYPE_BASE = -1,

#include "mmi_livewallpaper.def"

    MMI_LIVE_WALLPAPER_TYPE_MAX
}MMI_LIVE_WALLPAPER_TYPE;

#undef LW_DEF

//#define MMISET_WALLPAPER_SPECIAL_ANIM_NUM   (MMI_LIVE_WALLPAPER_TYPE_MAX)
#define MMI_LIVE_WALLPAPER_WRE_TYPE_BASE  MMI_LIVE_WALLPAPER_TYPE_MAX

/*动态壁纸的基本信息，类型，小图标，名字*/
#define MMI_LIVEWALLPAPER_NAME_MAX  (20)
#define MMI_LIVEWALLPAPER_FILE_FULL_PATH_MAX_LEN  MMIFMM_FILE_FULL_PATH_MAX_LEN
typedef struct _LW_THUMBNAIL_INFO_
{

    BOOLEAN is_image_file;
    uint32 image_id;
    wchar img_path[MMI_LIVEWALLPAPER_FILE_FULL_PATH_MAX_LEN + 1];

    BOOLEAN is_not_string;
    uint32 text_id;
    wchar name[MMI_LIVEWALLPAPER_NAME_MAX + 1];
    uint32 instance_mem_pool_size;    
}LW_THUMBNAIL_INFO;

typedef struct _LIVE_WALLPAPER_INFO_
{
    uint32 type;   //动态壁纸类型
    LW_THUMBNAIL_INFO thumbnail_info;
    const char *type_name_ptr;
    uint8   res_type;
}LIVE_WALLPAPER_INFO;

/*增加buf信息的结构体by arvin begin*/
typedef struct _LW_BUF_INFO_
{
    void *buf_ptr;
    uint16 w;
    uint16 h;
    DISPLAY_DATA_TYPE_E data_type;
}LW_BUF_INFO;
typedef struct _LW_FG_POS_INFO_
{
    GUI_RECT_T rc;//前景的区域信息， 相对坐标
    int16 x;//当前屏幕在fg中的x坐标，相对坐标
    int16 y;//当前屏幕在fg中的y坐标，相对坐标
}LW_FG_POS_INFO;

typedef void (*LW_CALLBACK)( LW_BUF_INFO buf_info,uint32 param);
typedef struct _LW_CAL_OUT_PARAM_
{
    LW_CALLBACK  callback;//call back函数，函数内部要使用硬件操作，使用一次自动失效，若要连续使用，需要连续注册
    uint32 param;//CallBack函数的用户参数，用户的私有数据   
}LW_CAL_OUT_PARAM;
/*增加动态壁纸计算参数by arvin begin*/
typedef struct _LW_CAL_PARAM_
{
    LW_BUF_INFO dest_buf_info;//目标buffer信息
    LW_BUF_INFO src_buf_info;//源buffer信息

    LW_FG_POS_INFO fg_pos_info;//前景层的坐标信息

    LW_CAL_OUT_PARAM out_param;
    uint32 reserve1;//保留
    uint32 reserve2;//保留
    uint32 reserve3;//保留
}LW_CAL_PARAM;

typedef struct _LW_FG_INFO_
{
    LW_FG_POS_INFO lw_fg_pos_info;
    GUI_LCD_DEV_INFO disp_dev_info;
}LW_FG_INFO;
/*动态壁纸的抽象类*/
typedef struct _MMI_LIVE_WALLPAPER_ITEM_INFO_T_
{
    BOOLEAN (*Init)(void);//私有静态数据初始化，只初始化一次//修改后支持动态数据初始化
    
    BOOLEAN (*DynamicConstruct)(MMI_WIN_ID_T win_id);//私有数据创建

    BOOLEAN (*DrawBg)(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);//绘制或者准备原始的背景数据
        
    void (*Calculate)(LW_CAL_PARAM *lw_cal_param_ptr);//算法函数，实例在这个算法里操作内存
    
    void (*DynamicDestruct)(void);//私有数据销毁
    
    BOOLEAN (*HandleEvent)(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//处理交互数据

    void (*DeInit)(void);    //增加对应的销毁初始化对应的销毁函数，适应平台的特效架构

    uint32 (*GetType)(void);//返回当前动态壁纸的类型
    
    void (*GetThumbNailInfo)(LW_THUMBNAIL_INFO *thumbnail_info_ptr);//返回ThumbNail的信息
    
    void (*GetWidthAndHeight)(uint16 *width_ptr,uint16 *height_ptr);//返回Src的宽高,若为PNULL则为默认宽高
    
    uint32 (*GetTimerTimeOut)(void);
    BOOLEAN (*NeglectInCondition)(MMI_MESSAGE_ID_E msg_id/*, MMI_WIN_ID_T win_id, DPARAM param*/);
    
    OPTIONLIST *(*GetOptionList)(void);//返回选项的指针若为PNULL则为默认
    
}MMI_LIVE_WALLPAPER_ITEM_INFO_T;

typedef struct _MMI_LIVEWALLPAPER_WIN_CFG_
{
    uint32 win_id;
    //获得当前前景层的绝对区域lw_fg_info_ptr->lw_fg_pos_info.rc，
    //获得当前屏幕在前景层中的偏移lw_fg_info_ptr->lw_fg_pos_info.x,lw_fg_info_ptr->lw_fg_pos_info.y
    //获得显示的层信息
    void (*GetFgInfo)(LW_FG_INFO *lw_fg_info_ptr);
}MMI_LIVEWALLPAPER_WIN_CFG;

//layer的信息
typedef struct
{
    uint16 layer_width;
    uint16 layer_height;
    uint16 mem_width;
    uint8  *layer_buf_ptr;
    DISPLAY_DATA_TYPE_E data_type;
}LIVEWALLPAPER_UILAYER_INFO_T;


/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMILIVEWALLPAPER_Init           MMIAPILIVEWALLPAPER_Init
#define MMILIVEWALLPAPER_Destroy        MMIAPILIVEWALLPAPER_Destroy
#define MMILIVEWALLPAPER_Play           MMIAPILIVEWALLPAPER_Play
#define MMILIVEWALLPAPER_HandleEvent    MMIAPILIVEWALLPAPER_HandleEvent
#define MMILIVEWALLPAPER_InitModule     MMIAPILIVEWALLPAPER_InitModule
#define MMILIVEWALLPAPER_IsSupportWin   MMIAPILIVEWALLPAPER_IsSupportWin
#define MMILIVEWALLPAPER_GetDisplayInfo MMIAPILIVEWALLPAPER_GetDisplayInfo
#define MMILIVEWALLPAPER_SetLiveWallPaperType       MMIAPILIVEWALLPAPER_SetLiveWallPaperType
#define MMILIVEWALLPAPER_GetAllWallpapersInfo       MMIAPILIVEWALLPAPER_GetAllWallpapersInfo
#define MMILIVEWALLPAPER_CreatePreviewWin           MMIAPILIVEWALLPAPER_CreatePreviewWin
#define MMILIVEWALLPAPER_CreateSettingWindow        MMIAPILIVEWALLPAPER_CreateSettingWindow
#define MMILIVEWALLPAPER_IsNeedOptionMenu           MMIAPILIVEWALLPAPER_IsNeedOptionMenu
#define MMILIVEWALLPAPER_CreateSetWallPaperButton   MMIAPILIVEWALLPAPER_CreateSetWallPaperButton
#define MMILIVEWALLPAPER_AppendSetWallPaperButton   MMIAPILIVEWALLPAPER_AppendSetWallPaperButton
#define MMILIVEWALLPAPER_PlayFrame                  MMIAPILIVEWALLPAPER_PlayFrame

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/

/*以下几个函数提供给增加窗体的动态壁纸效果调用 begin*/
/*****************************************************************************/
//  Description : 动态墙纸初始化函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:开机的时候就做初始化工作
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_Init(void);


/*****************************************************************************/
//  Description :  动态墙纸销毁
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_Destroy(BOOLEAN is_clean);

/*****************************************************************************/
//  Description :  动态墙纸播放
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_Play(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description :  动态墙纸停止
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_Stop(void);

/*****************************************************************************/
//  Description : 动态墙纸消息处理函数
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*以下几个函数提供给增加窗体的动态壁纸效果调用 end*/
/*****************************************************************************/
//  Description :  初始化动态壁纸模块
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_InitModule(void);

/*****************************************************************************/
//  Description : 判断是否支持的窗体
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_IsSupportWin(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : MMILIVEWALLPAPER_GetDisplayLayer
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_GetDisplayInfo(MMI_WIN_ID_T win_id, LW_FG_INFO *lw_fg_info_ptr);

/*****************************************************************************/
//  Description : 动态壁纸合并和计算
//  Global resource dependence : 
//  Author: Gaily.Wang
//  Note:
/*****************************************************************************/
//PUBLIC void MMILIVEWALLPAPER_CalcCnf(void);

/*****************************************************************************/
//  Description :   设置动态墙纸的类新
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_SetLiveWallPaperType(uint16 livewallpaper_index);

/*****************************************************************************/
//  Description :  获得动态壁纸信息
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_GetAllWallpapersInfo(LIVE_WALLPAPER_INFO *wallpaper_info_ptr,uint16 wallpaper_index);

/*****************************************************************************/
//  Description :   获得当前墙纸的索引值
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPILIVEWALLPAPER_GetCurrentLiveWallPaperIndex(void);

/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_CreatePreviewWin
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_CreatePreviewWin(void);

/*****************************************************************************/
//  Description : 创建设置窗体
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_CreateSettingWindow(void);

/*****************************************************************************/
//  Description :  壁纸是否支持选项设置列表
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_IsNeedOptionMenu(void);

/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_CreateSetWallPaperButton
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_CreateSetWallPaperButton(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : MMIAPILIVEWALLPAPER_AppendSetWallPaperButton(供外部使用) 
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_AppendSetWallPaperButton(void);

/*****************************************************************************/
//  Description :  主动调用播放帧函数，通常使用主转场的过程中
//                     应用主动调用刷屏函数，导致背景不更新
//  Global resource dependence : 
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_PlayFrame(MMI_WIN_ID_T win_id);

//start mem pool mechanism;
/*****************************************************************************/
//  Description : init livewallpaper block memory 
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPILIVEWALLPAPER_InitBlock(uint32 block_size);

/*****************************************************************************/
//  Description : livewallpaper alloc in block
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void *MMIAPILIVEWALLPAPER_AllocInBlock(uint32 size);

/*****************************************************************************/
//  Description : livewallpaper alloc in block
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void *MMIAPILIVEWALLPAPER_AllocInBlockEx(uint32 size, const char *file_ptr, uint32 line);

/*****************************************************************************/
//  Description : destroy livewallpaper block memory 
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_DestroyBlock(void);


/*****************************************************************************/
//  Description : livewallpaper free in block
//  Global resource dependence : none
//  Author: glen.li
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPILIVEWALLPAPER_FreeInBlock(void *ptr);

/*****************************************************************************/
//  Description :  层中使用内存池分配 释放的回调接口 ；
//  Global resource dependence : 
//  Author: Glen.Li
//  Note:
/*****************************************************************************/
PUBLIC UILAYER_MEM_FUNC_T * MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(void);

/*****************************************************************************/
//  Description : 获取idle timer id
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPILIVEWALLPAPER_SaveTimerId(uint8 timer_id);
//end mem pool mechanism;
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                                                              */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif /*_MMI_LIVE_WALLPAPER_C_*/

/******************************************************************************
** File Name:      ui_layer.h                                                  *
** Author:                                                                    *
** Date:            		                                                  *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.          *
** Description:      								                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
**                                       									  *
******************************************************************************/


/*!
 *  \addtogroup mmk_mmiutil_group_label
 *  @{
 */


/*! 
 *  \file 	ui_layer.h
 *  \author	
 *  \date	
 *  \brief 	
 */
 
#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "mmk_type.h"
#include "dal_lcd.h"
//#include "guilcd.h"
#include "dal_display.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/

#define UILAYER_TOTAL_LAYER_COUNT       GUI_UILAYER_TOTAL_LAYER_COUNT//16      //逻辑层块的个数
#define UILAYER_BLEND_MAX_NUM           4       //底层硬件最多合成图层数（超过的话，分多次合成）
#define UILAYER_TOTAL_BLT_COUNT         16//8       //刷新层（叠加队列）最多个数
#define UILAYER_TRANSPARENT_COLOR       0x8080  //透明色
//#define UILAYER_MAIN_HANDLE             (GUI_BLOCK_MAIN)    //main_handle
#define UILAYER_NULL_HANDLE             (0xffff)    //null_handle

#define MMI_MEMMONITER_START    MMI_MemMoniterStart
#define MMI_MEMMONITER_END      MMI_MemMoniterEnd

#define UILAYER_RELEASELAYER UILAYER_ReleaseLayerExt
// #define UILAYER_RELEASELAYER(dev_info_ptr)     \
//     do  {   \
//         if (PNULL != (dev_info_ptr))    \
//         {   \
//             UILAYER_ReleaseLayer(dev_info_ptr); \
//             (dev_info_ptr)->lcd_id = 0;   \
//             (dev_info_ptr)->block_id = UILAYER_NULL_HANDLE;   \
//         }   \
//     }   \
//     while(0);

typedef uint32 UILAYER_HANDLE_T;    //layer句柄

//层buffer的类型
typedef uint8 UILAYER_MEM_FORMAT_T;

#define UILAYER_MEM_FORMAT_IMMUTABLE  0x01
#define UILAYER_MEM_FORMAT_DOUBLE     0x02
//#define UILAYER_MEM_FORMAT_PINGPONG   0x04

#define UILAYER_MEM_NONE		    0
#define UILAYER_MEM_IMMUTABLE       UILAYER_MEM_FORMAT_IMMUTABLE                            //层数据不变
#define UILAYER_MEM_DOUBLE_COPY     (UILAYER_MEM_FORMAT_DOUBLE)  //层数据为双缓冲拷贝模型，速度要比乒乓慢，多了拷贝时间
#define UILAYER_MEM_DOUBLE_PINGPONG (UILAYER_MEM_FORMAT_IMMUTABLE|UILAYER_MEM_FORMAT_DOUBLE)  //层数据为双缓冲乒乓模型

//内存不足时，双buffer改为单buffer
#ifdef MMI_FPS_ACCELERATE_SUPPORT
#define UILAYER_MEM_FPS_ACCELERATE UILAYER_MEM_DOUBLE_COPY
#else
#define UILAYER_MEM_FPS_ACCELERATE UILAYER_MEM_IMMUTABLE
#endif

//layer函数执行结果
typedef enum
{
    UILAYER_RESULT_SUCCESS,         //操作成功
    UILAYER_RESULT_USEDMAXNUM,      //层的使用已经达到最大数目
    UILAYER_RESULT_MULTILAYER_DISABLED,      //多层禁止使用
    UILAYER_RESULT_ERROR      //错误
}UILAYER_RESULT_E;

//layer的信息
typedef struct
{
    uint16 layer_width;
    uint16 layer_height;
    uint16 mem_width;
//    uint8  *layer_buf_ptr;
    DISPLAY_DATA_TYPE_E data_type;
}UILAYER_INFO_T;


typedef struct  
{
    void* (*Malloc)( uint32 size );
    void  (*Free)( void *memory_ptr );
}UILAYER_MEM_FUNC_T;

//创建layer的信息
typedef struct
{
    LCD_ID_E lcd_id;
    MMI_HANDLE_T owner_handle;
    int16 offset_x;
    int16 offset_y;
    int16 width;
    int16 height;
    BOOLEAN is_bg_layer;
    BOOLEAN is_static_layer;
    UILAYER_MEM_FORMAT_T format;
    UILAYER_MEM_FUNC_T* func_ptr;
}UILAYER_CREATE_T;

//blt图层的级别
typedef enum
{
    UILAYER_LEVEL_LOW,
    UILAYER_LEVEL_NORMAL,
    UILAYER_LEVEL_HIGH,
    UILAYER_LEVEL_DEBUG,

    UILAYER_LEVEL_MAX
}UILAYER_LEVEL_E;

//添加blt图层的信息
typedef struct
{
    GUI_LCD_DEV_INFO lcd_dev_info;
    UILAYER_LEVEL_E layer_level;
}UILAYER_APPEND_BLT_T;

//layer屏幕抓图结构
typedef struct
{
    DISPLAY_BLEND_LAYER_T lcd_layer_blend;
    DISPLAY_BLEND_LAYER_T sublcd_layer_blend;
    const DISPLAY_BLEND_LAYER_T layer_stuct_arr[UILAYER_TOTAL_BLT_COUNT];
    uint32 arr_size;
}UILAYER_PRINTSCREEN_INFO_T;

//封装blend信息
typedef struct
{
    DISPLAY_BLEND_LAYER_T dst_layer;
    DISPLAY_BLEND_LAYER_T src_layer[UILAYER_TOTAL_BLT_COUNT];
    uint32 layer_num;
    DISPLAY_RECT_T rect;
    DISPLAY_CALLBACK callback;
    uint32 param;
}UILAYER_BLEND_T;

//多图层handle的type
typedef enum
{
    UILAYER_HANDLE_INVALID,     //无效handle
    UILAYER_HANDLE_MAIN,        //主层handle
    UILAYER_HANDLE_BLOCK,       //block handle
    UILAYER_HANDLE_MULTI        //multi handle
}UILAYER_HANDLE_TYPE_E;

//copy buffer info
typedef struct 
{
    DISPLAY_DATA_TYPE_E data_type;
    uint8 *buf_ptr;
    uint16 width;
    uint16 height;
}UILAYER_COPYBUF_T;

//copy buffer param
typedef struct 
{
    GUI_RECT_T  rect;
    GUI_RECT_T* clip_rect_ptr;
}UILAYER_COPYBUF_PARAM_T;

typedef void (*UILAYER_BLEND_CALLBACK)( 
                                       uint8* buffer, 
                                       UILAYER_INFO_T *layer_info_ptr,
                                       uint32 param
                                       );

/*----------------------------------------------------------------------------*/
/*                         FUNCTIONS                                          */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : 设置layer透明度
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //透明度
                                  );

/*****************************************************************************/
//     Description : 设置layer透明色
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerColorKey(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr,
                                     BOOLEAN is_color_key_enable,
                                     GUI_COLOR_T color_key              //透明色
                                     );

/*****************************************************************************/
//     Description : 创建layer，返回layer句柄
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC UILAYER_RESULT_E UILAYER_CreateLayerEx(
                                            const UILAYER_CREATE_T *create_ptr,
                                            GUI_LCD_DEV_INFO *dev_info_ptr,     //out
                                            const char*       file_name_ptr,
                                            uint32            file_line
                                            );

#define UILAYER_CreateLayer(create_ptr, dev_info_ptr)   \
    UILAYER_CreateLayerEx(create_ptr, dev_info_ptr, _D_FILE_NAME, _D_FILE_LINE)

/*****************************************************************************/
//     Description : 在s_layer_blt中去掉layer_handle
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveBltLayer(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   );

/*****************************************************************************/
//     Description : 增加一个层
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_AppendBltLayer(
                                   UILAYER_APPEND_BLT_T *append_layer_ptr
                                   );

/*****************************************************************************/
//     Description : 合并层刷新到lcd上去。某一时刻，调用该函数的结果就是将s_layer_blt[0], s_layer_blt[1], 
//s_layer_blt[2], s_layer_blt[3]依次合并，如果is_refesh_to_lcd=TRUE，则刷新到LCD上。否则刷到lcd_buf上
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_InvalidateRect(//Return the operate status
	LCD_ID_E lcd_id,				//id of lcd to operate
    const GUI_RECT_T *rect_ptr
	);

/*****************************************************************************/
//     Description : 将目标层底下层的数据刷到到目标层上去。
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  const GUI_RECT_T *rect_ptr
                                  );

/*****************************************************************************/
//     Description : 初始化layer系统
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_InitSystem(
//                               GUI_COLOR_T *mainlcd_buf_ptr,
//                               uint32 mainlcd_buf_size,
//                               DISPLAY_DATA_TYPE_E data_type
//                               );
/*****************************************************************************/
//     Description : 初始化layer系统
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_InitSystem(void);

/*****************************************************************************/
//     Description : 获得layer的buffer指针
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint8 *UILAYER_GetLayerBufferPtr(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                   );

/*****************************************************************************/
//     Description : 获得layer的相对于LCD左上角的坐标
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_GetLayerPosition(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 *offset_x_ptr, //output
                                int16 *offset_y_ptr //output
                                );

/*****************************************************************************/
//     Description : 获得layer的信息
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_GetLayerInfo(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                 UILAYER_INFO_T *layer_info_ptr     //output
                                 );

/*****************************************************************************/
//     Description : 设置layer的位置（相对于lcd左上角的坐标）
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerPosition(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                int16 offset_x,
                                int16 offset_y
                                );

/*****************************************************************************/
//     Description : 多层功能是否开启
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:  FALSE 一层（主层） TRUE 多层
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsMultiLayerEnable(void);

/*****************************************************************************/
//     Description : 将图层清成透明色
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_Clear(
                          GUI_LCD_DEV_INFO const *dev_info_ptr
                          );

/*****************************************************************************/
//     Description : 将图层填充颜色
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FillColor(
                              GUI_LCD_DEV_INFO const *dev_info_ptr,
                              uint32 color
                              );

/*****************************************************************************/
//     Description : 激活Handle关联的所有layer，Handle get focus的时候调用
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveAllLayerOfHandle(
                                           MMI_HANDLE_T mmi_handle
                                           );

/*****************************************************************************/
//     Description : 去激活Handle关联的所有layer，Handle lose focus的时候调用
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveAllLayerOfHandle(
                                             MMI_HANDLE_T mmi_handle
                                             );

/*****************************************************************************/
//     Description : 释放Handle关联的所有layer，Handle close的时候调用
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseAllLayerOfHandle(
                                            MMI_HANDLE_T mmi_handle
                                            );

/*****************************************************************************/
//     Description : 释放layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayer(
                                 GUI_LCD_DEV_INFO const *dev_info_ptr
                                 );

//#if  defined(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)
/*****************************************************************************/
//     Description : 释放layerEX
//    Global resource dependence : 
//  Author:   Arvin.wu
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ReleaseLayerExt(
                                 GUI_LCD_DEV_INFO  *dev_info_ptr
                                 );
//#endif
/*****************************************************************************/
//     Description : 判断layer是否激活状态
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsLayerActive(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     );

/*****************************************************************************/
//     Description : 检查rect区域内是否透明，如果透明则将底下层的数据刷到到目标层上去
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLowerLayerWhenTransparent(
                                                 GUI_LCD_DEV_INFO const *dev_info_ptr,
                                                 const GUI_RECT_T *rect_ptr
                                                 );

/*****************************************************************************/
// Description : modify block width and rect info for rotation
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
PUBLIC void UILAYER_Rotation(
                             GUI_LCD_DEV_INFO   *lcd_dev_ptr
                             );
             
/*****************************************************************************/
//     Description : 获得内存检测地址
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 *UILAYER_GetMemCheckAddr(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr
                                       );

/*****************************************************************************/
//     Description : 内存检测：检测一段时间内某地址数据是否改变
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterStart(
                                uint32 *mem_ptr
                                );
                           
/*****************************************************************************/
//     Description : 内存检测：检测一段时间内某地址数据是否改变
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMI_MemMoniterEnd(void);

/*****************************************************************************/
//     Description : 将lcd数据备份到mainlayer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyLcdToMainLayer(void);

/*****************************************************************************/
//     Description : 判断layer是否激活状态
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E UILAYER_GetLayerColorType(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr
                                     );

/*****************************************************************************/
//     Description : 将图层清成某种颜色
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ClearRect(
                          GUI_LCD_DEV_INFO const *dev_info_ptr,
                          GUI_RECT_T rect
                          );

/*****************************************************************************/
//     Description : 减弱layer透明度
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_WeakLayerAlpha(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr,
                                  uint8 alpha_value            //透明度
                                  );

/*****************************************************************************/
//     Description : 将某层数据刷到另一层上去
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_BltLayerToLayer(
                                GUI_LCD_DEV_INFO const *dst_dev_ptr,
                                GUI_LCD_DEV_INFO const *src_dev_ptr,
                                const GUI_RECT_T *rect_ptr,
                                BOOLEAN is_compound     //TRUE, 混合，FALSE，直接覆盖
                                );

/*****************************************************************************/
//     Description : 判断layer是否blt layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsBltLayer(
                                  GUI_LCD_DEV_INFO const *dev_info_ptr
                                  );

/*****************************************************************************/
//     Description : 在s_layer_blt中去掉main layer handle
//    Global resource dependence : 
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RemoveMainLayer(void);

/*****************************************************************************/
//     Description : restore main layer.
//    Global resource dependence : 
//  Author:   
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_RestoreMainLayer(void);

/*****************************************************************************/
//     Description : （为屏幕抓图功能）获得lcd_buffer以及图层数据
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetBltInfoForPrintScreen(
                                                UILAYER_PRINTSCREEN_INFO_T *print_info_ptr
                                                );

/*****************************************************************************/
//     Description : 获得blend info
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetBlendInfo(
                                    LCD_ID_E lcd_id,    //in
                                    const GUI_RECT_T *rect_ptr,   //in
                                    UILAYER_BLEND_T *blend_ptr  //out
                                    );

/*****************************************************************************/
//     Description : get layer info of input blt layer array.
//     Global resource dependence : 
//     Author:  fulu.song
//     Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetLayersBlendInfo(
                                    const UILAYER_APPEND_BLT_T *blt_layers,   //in
                                    const uint32 num_layers,//in
                                    const GUI_LCD_DEV_INFO *dst_layer,
                                    const GUI_RECT_T *rect_ptr,   //in
                                    UILAYER_BLEND_T *blend_ptr  //out
                                    );

/*****************************************************************************/
//     Description : 获得handle类型
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC UILAYER_HANDLE_TYPE_E UILAYER_GetHandleType(
                                                   GUI_LCD_DEV_INFO const *dev_info_ptr
                                                   );
/*****************************************************************************/
//     Description : 判断是否是handle为空
//    Global resource dependence : 
//  Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsEmptyLayerHandle(
                                        GUI_LCD_DEV_INFO const *dev_info_ptr
                                        );
/*****************************************************************************/
//     Description : 获得invalidate param
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 为双缓冲main lcd buffer优化
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_GetInvalidateParam(
                                          LCD_ID_E lcd_id,                     //in
                                          const GUI_RECT_T *rect_ptr,
                                          DISPLAY_INVALIDATE_PARM_T* param_ptr //in out
                                          );

/*****************************************************************************/
//     Description : 设置主层mem format
//    Global resource dependence : 
//  Author:  bin.ji
//    Note: 
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerMemFormat(
                                    UILAYER_MEM_FORMAT_T type
                                    );

/*****************************************************************************/
//     Description : 设置layer数据不变
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 性能优化专用，若层数据不变，底层异步刷屏可节省拷贝层数据的时间
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerMemFormat(
                                      GUI_LCD_DEV_INFO const *dev_info_ptr,
                                      UILAYER_MEM_FORMAT_T format
                                      );

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 
/*****************************************************************************/
PUBLIC UILAYER_MEM_FORMAT_T UILAYER_GetLayerMemFormat(
                                                      GUI_LCD_DEV_INFO const *dev_info_ptr
                                                      );


/*****************************************************************************/
//     Description : 双buffer回写
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 只有双buffer的图层才能调用，且在双buffer切换后调用
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_DoubleMemWriteBack(
                                          GUI_LCD_DEV_INFO const *dev_info_ptr,
                                          const GUI_RECT_T       *dst_rect_ptr,
                                          const GUI_RECT_T       *clip_rect_ptr,
                                          const GUI_RECT_T       *src_rect_ptr
                                          );

/*****************************************************************************/
//     Description : 切换double mem
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 性能优化专用，切换double mem，不再拷贝，推荐在滑动时刷新整屏数据时使用
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_SwitchDoubleMem(
                                       GUI_LCD_DEV_INFO const *dev_info_ptr,
                                       BOOLEAN is_switch
                                       );

/*****************************************************************************/
//     Description : 设置主层数据固定
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 性能优化专用，假定主层数据固定不再进行双缓冲拷贝，推荐在滑动时时使用
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerImmutable(
                                          BOOLEAN is_immutable
                                          );

/*****************************************************************************/
//     Description : direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 性能优化专用，利用硬件特性，合层结果直接刷到lcd硬件上
//          但由于合层结果不会存lcd buffer，可能引起界面显示不正确，谨慎使用
/*****************************************************************************/
PUBLIC void UILAYER_SetDirectDraw(
                                  BOOLEAN is_direct
                                  );
                                  
/*****************************************************************************/
//     Description : is pre direct draw
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note: 性能优化专用，利用硬件特性，合层结果直接刷到lcd硬件上
//          判断上一帧是否为dricet draw
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsPerDirectDraw( void );

/*****************************************************************************/
//     Description : 获得参与合成的图层数目
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltNum(
                                LCD_ID_E lcd_id,    //in
                                const GUI_RECT_T *rect_ptr   //in
                                );
                                       
/*****************************************************************************/
//     Description : 设置剪切区域（rect_ptr是相对与图层位置）
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_SetClipRect(
//                                GUI_LCD_DEV_INFO const *dev_info_ptr,
//                                BOOLEAN is_clip_enable,
//                                const GUI_RECT_T *rect_ptr   //in
//                                );

/*****************************************************************************/
//     Description : 得到layer的全部区域
//    Global resource dependence : 
//  Author:   hongbo.lan
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T UILAYER_GetLayerRect(GUI_LCD_DEV_INFO *dev_info_ptr);                           
/*****************************************************************************/
//     Description : copy layer to buffer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyLayerToBuffer(
                               GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                               UILAYER_COPYBUF_PARAM_T *param_ptr,  //in
                               UILAYER_COPYBUF_T *copybuf_ptr   //in,out
                               );

/*****************************************************************************/
//     Description : store layer update rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_StoreLayerUpdateRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr
                                );

/*****************************************************************************/
//     Description : copy buffer to layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_CopyBufferToLayer(
                               GUI_LCD_DEV_INFO *dev_info_ptr,  //in
                               UILAYER_COPYBUF_PARAM_T *param_ptr,  //in
                               UILAYER_COPYBUF_T *copybuf_ptr   //in,out
                               );                                                          

/*****************************************************************************/
//     Description : 用来代替lcd_buffer的osd layer是否开启
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_IsOsdLayerEnable(void);

/*****************************************************************************/
//     Description : 开启/关闭用来代替lcd_buffer的osd layer
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_EnableOsdLayer(
                                   BOOLEAN is_enable
                                   );

#ifdef WRE_SUPPORT
/*****************************************************************************/
//　　Description : 将某层移动到指定的后面
//　　Global resource dependence : 
//　　Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_MoveLayer(
                            GUI_LCD_DEV_INFO const *dst_dev_ptr,
                            GUI_LCD_DEV_INFO const *after_dev_ptr);
#endif
#if defined(MENU_CRYSTALCUBE_SUPPORT) || defined (MENU_CYCLONE_SUPPORT)  || defined (MET_MEX_SUPPORT)
/*****************************************************************************/
//  Description : change buffer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void UILAYER_SetLayerBufferPtr(
                                   GUI_LCD_DEV_INFO const *dev_info_ptr,
                                   uint8 *new_buf_addr,
                                   uint8 *new_raw_buf_addr,
                                   uint8 **old_buf_addr,
                                   uint8 **old_raw_buf_addr
                                   );
/*****************************************************************************/
//  Description : get raw layer buffer ptr
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8* UILAYER_GetLayerRawBufferPtr(GUI_LCD_DEV_INFO const *dev_info_ptr);
#endif

/*****************************************************************************/
//     Description : set mainlayer type
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetMainLayerType(
                                     DISPLAY_DATA_TYPE_E data_type
                                     );

/*****************************************************************************/
//     Description : 去激活win_handle或者win_handle上控件关联的所有layer，为进入
// camera等大内存应用时，提前释放内存调用
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
//PUBLIC void UILAYER_DeactiveAllLayerOfWinHandle(
//												MMI_HANDLE_T mmi_handle
//												);

/*****************************************************************************/
// Description : 设置显示时，是否将565数据转为888数据的状态
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC void UILAYER_SetDisplayRGB565TOARGB888State(BOOLEAN is_true);

/*****************************************************************************/
// Description : 获得显示时，是否将565数据转为888数据的状态
// Global resource dependence : 
// Author: Xiaoqing.Lu
// Note:
/*****************************************************************************/
//PUBLIC BOOLEAN UILAYER_GetDisplayRGB565TOARGB888State(void);

/*****************************************************************************/
//  Description : 超过4层分多次合成
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_MultiBlendLayer(
                                       LCD_ID_E    lcd_id,
                                       DISPLAY_BLENDLAYER_PARM_T*  blend_param_ptr
                                       );

/*****************************************************************************/
//  Description : blend src layers to dst layers.
//  Global resource dependence : 
//  Author: fulu.song
//  Note: compare to UILAYER_MultiBlendLayer, this function also works when less
//  than 4 src layers 
/*****************************************************************************/
PUBLIC BOOLEAN UILAYER_MultiBlendLayerEx(
                                       LCD_ID_E    lcd_id,
                                       DISPLAY_BLENDLAYER_PARM_T* blend_param_ptr
                                       );


/*****************************************************************************/
//     Description : get all blt layers of handle 
//    Global resource dependence : 
//  Author:   fulu.song
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltLayersOfHandle(
                                  MMI_HANDLE_T mmi_handle,
                                  UILAYER_APPEND_BLT_T *layer_handle_arr,    //in/out
                                  uint32 arr_num    //in
                                  );

/*****************************************************************************/
//     Description : 设置哪几个handle预备刷到lcd上，参数的顺序决定了各层叠加的顺序
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetBltLayer(
                                UILAYER_APPEND_BLT_T *layer_handle_arr,
                                uint32 arr_size
                                );

/*****************************************************************************/
//     Description : 得到当前的blt layer设置，返回blt数目
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 UILAYER_GetBltLayer(
                                  UILAYER_APPEND_BLT_T *layer_handle_arr,    //in/out
                                  uint32 arr_num    //in
                                  );

/*****************************************************************************/
//     Description : 设置ARGB888的图层设为预乘
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetPreMultiplyAlpha(
                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
                                        BOOLEAN                 is_pre_multiply_alpha,//图层是否预乘alpha的格式
                                        BOOLEAN                 is_need_convert       //是否将图层数据进行转换
                                        );

/*****************************************************************************/
//     Description : 设置图层的回调函数,在刷屏后被调用
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetBlendCallback(
                                     GUI_LCD_DEV_INFO const *dev_info_ptr,
                                     UILAYER_BLEND_CALLBACK callback,
                                     uint32                 param
                                     );

/*****************************************************************************/
//     Description : set clip rect
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_SetClipRect(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,
                                GUI_RECT_T clip_rect
                                );

/*****************************************************************************/
//     Description : 清除所有图层的双缓冲
//    Global resource dependence : 
//  Author:  James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_FreeAllLayerDoubleBuffer( void );

/*****************************************************************************/
//  Description : deactive layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_DeactiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr);

/*****************************************************************************/
//  Description : active layer handle.
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note:
/*****************************************************************************/
PUBLIC void UILAYER_ActiveLayer(GUI_LCD_DEV_INFO const *dev_info_ptr);

/*****************************************************************************/
// 	Description : print info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void UILAYER_PrintInfo(void);


/*****************************************************************************/
//     Description : switch mainlayer（仅供camera使用，可以节省一个buffer）
//    Global resource dependence : 
//  Author:   bin.ji
//    Note: is_setto_mainlcd_buffer=TRUE，设置为lcd_buffer；FALSE，从lcd_buffer设置回来（动态分配）
/*****************************************************************************/
PUBLIC void UILAYER_SwitchMainLayer(
                                    BOOLEAN is_setto_mainlcd_buffer
                                    );

/*****************************************************************************/
//     Description : 设置RGB565 的图层设为ARGB555
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerRGB565ToARGB555(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert ,
									                                        BOOLEAN 			Alpha_Bit
									                                        );
/*****************************************************************************/
//     Description : 设置RGB565 的图层设为ARGB5515
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerRGB565ToARGB5515(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert ,
									                                        BOOLEAN       		Alpha_Bit
									                                        );
/*****************************************************************************/
//     Description : 设置ARGB555 的图层设为RGB565
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void UILAYER_ChangeLayerARGB555ToRGB565(
									                                        GUI_LCD_DEV_INFO const *dev_info_ptr,
									                                        BOOLEAN                 is_need_convert 
									                                        );
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif


#endif  /* FILENAME_H */
/*! @} */

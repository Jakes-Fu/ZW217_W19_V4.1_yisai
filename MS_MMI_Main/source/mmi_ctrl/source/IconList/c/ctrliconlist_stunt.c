#ifdef GUIF_ICONLIST
/*****************************************************************************
** File Name:      guiiconlist_anim.c                                        *
** Author:         hua.fang                                                  *
** Date:           07/20/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/

#include "ctrliconlist.h" /*lint -e766*/

#ifdef GUIF_ICONLIST_STUNT

#include "sci_types.h"
#include "chng_freq.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_tp.h"
#include "ui_layer.h"
#include "ctrliconlist.h"
#include "mmi_theme.h"
#include "mmitheme_iconlist.h"
#include "guistring.h"
#include "guires.h"
#include "guiwin.h"
#include "cafctrliconlist.h"
#include "mmitheme_frame_engine.h"
#include "graphics_draw.h"
#ifdef GUIF_ICONFOLDER
#include "ctrliconfolder.h"
#endif
#include "ctrlanim.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

#define GUIICONLIST_PIC_MARGIN                      80              // 做图像到文件动画时，图像之间的间距
#define GUIICONLIST_FRAME_COUNT                     10              // 动画帧数
#define GUIICONLIST_IMG2FILE_FILL_COUNT             8               // 图像到文件动画时，背景用单色填充的帧数
#define GUIICONLIST_IMG2FILE_BORDER_COUNT           10              // 图像到文件时，画边框的帧数
#define GUIICONLIST_FILE2FOLDER_COUNT               5               // 文件到目录时，非当前目录的运动帧数
#define GUIICONLIST_FILE2FOLDER_SCALE_COUNT         2               // 文件到目录时，非当前目录图像的放大帧数
#define GUIICONLIST_BG_COLOR                        0               // 单色时的背景色
#define GUIICONLIST_BORDER_COLOR                    0xffff          // 图像到文件时，边框的颜色

#define GUIICONLIST_ALIGN_EX(x,align)   ((x/align+1) * align)       // 对齐

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// 动画添加帧的回调函数类型
typedef void (*GUIICONLIST_ADD_FRAME_FUNC)(CTRLICONLIST_OBJ_T* iconlist_ctrl_ptr, uint16 screen_width, uint16 screen_height);

// 放大的用户数据
typedef struct
{
    CTRLICONLIST_OBJ_T      *iconlist_ctrl_ptr;             // iconlist对象
    uint16                  do_count;                       // 已经做了多少帧
    GUIICONLIST_IMG_INFO_T  img_info;                       // 图像数据
    uint16                  max_scale_width;                // 最大放大的宽
    uint16                  max_scale_height;               // 最大放大的高
} GUIICONLIST_SCALE_PARAM_T;

// 旋转的用户数据
typedef struct
{
    CTRLICONLIST_OBJ_T          *iconlist_ctrl_ptr;         // iconlist对象
    GUIICONLIST_ROTATE_INFO_T   rotate_info;                // 旋转的信息
} GUIICONLIST_ROTATE_PARAM_T;

//rotate arbitrary angle in param
typedef struct
{
    uint8               *src_buf_ptr;       //src buffer
    uint32              src_buf_size;       //src buffer size
    uint16              src_img_width;      //src image width
    uint16              src_img_height;     //src image height

    uint8               *dest_buf_ptr;      //dest buffer
    uint32              dest_buf_size;      //dest buffer size
    uint16              dest_img_width;     //dest image width
    uint16              dest_img_height;    //dest image height

    uint16              angle;              //angle
    GUIIMG_DATE_TYPE_E  src_data_type;  //src data type
} GUIICONLIST_ROTATE_ANGLE_INPUT_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateIconListLayer(
                                  int16                  width,              // in
                                  int16                  height,             // in
                                  UILAYER_MEM_FORMAT_T   mem_format,         // in
                                  GUI_LCD_DEV_INFO       *lcd_dev_info_ptr   // out
                                  );

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseIconListLayer(
                                GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                                );

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendIconListLayer(
                               GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                               );

/*****************************************************************************/
//  Description : size to rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_RECT_T Size2Rect(
                                    GUI_SIZE_T *size_ptr
                                    );

/*****************************************************************************/
//  Description : rect to size
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_SIZE_T Rect2Size(
                                    GUI_RECT_T *rect_ptr
                                    );

/*****************************************************************************/
//  Description : make rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_RECT_T MakeRect(
                                   int16        center_x,
                                   int16        center_y,
                                   uint16       w,
                                   uint16       h
                                   );

/*****************************************************************************/
//  Description : do file to image view animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFile2Img(
                               ANIM_HANDLE_T        anim_handle,
                               uint16               frame_index,
                               MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                               );

/*****************************************************************************/
//  Description : do image to file background animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationImg2FileBg(
                                 ANIM_HANDLE_T        anim_handle,
                                 uint16               frame_index,
                                 MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                                 );

/*****************************************************************************/
//  Description : do image to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationImg2File(
                               ANIM_HANDLE_T        anim_handle,
                               uint16               frame_index,
                               MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                               );

/*****************************************************************************/
//  Description : do image move and rotate background animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFolderFileBg(
                                   ANIM_HANDLE_T        anim_handle,
                                   uint16               frame_index,
                                   MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                                   );

/*****************************************************************************/
//  Description : do scale image animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationScaleImage(
                                 ANIM_HANDLE_T            anim_handle,
                                 uint16                   frame_index,
                                 MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                                 );

/*****************************************************************************/
//  Description : do image move and rotate animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFolderFile(
                                 ANIM_HANDLE_T        anim_handle,
                                 uint16               frame_index,
                                 MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                                 );

/*****************************************************************************/
//  Description : 动画结束时的回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationEnd(
                          ANIM_HANDLE_T anim_handle,
                          uint32        user_data
                          );

/*****************************************************************************/
//  Description : 画缩放后的图片到目标层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawScaleImage(
                          GUIICONLIST_SCALE_PARAM_T *user_data_ptr,
                          GUI_SIZE_T                *cur_size_ptr
                          );

/*****************************************************************************/
//  Description : 绘制bitmap的图像
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawImageBitmap(
                           CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                           GUI_RECT_T          *rect_ptr,
                           GUIIMG_BITMAP_T     *bitmap_ptr
                           );

/*****************************************************************************/
//  Description : 绘制默认的图片
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawDefaultImage(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                            GUI_RECT_T          *rect_ptr,
                            MMI_IMAGE_ID_T      image_id
                            );

/*****************************************************************************/
//  Description : 获取动画控件里图像的原始大小
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetImageWidthHeight(
                               MMI_HANDLE_T     anim_ctrl_handle,
                               uint16           *img_width_ptr,
                               uint16           *img_height_ptr
                               );

/*****************************************************************************/
//  Description : 获取最大的放大尺寸
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetScaleMaxSize(
                           CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                           CTRLBASE_OBJ_T        *anim_ctrl_ptr,
                           uint16                screen_width,
                           uint16                screen_height,
                           uint16                *max_width_ptr,
                           uint16                *max_height_ptr
                           );

/*****************************************************************************/
//  Description : 获取在文件iconlist里的位置信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetSizeInFile(
                         CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                         uint16                item_index,
                         GUI_SIZE_T            *src_size_ptr
                         );

/*****************************************************************************/
//  Description : 获取在图像界面里的位置信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetImgDstSize(
                         CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,  //in
                         uint16                 screen_width,
                         uint16                 screen_height,
                         GUI_SIZE_T             *size_ptr
                         );

/*****************************************************************************/
//  Description : 获取指定图像的信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
//  只能在ICON模式下使用
/*****************************************************************************/
LOCAL void GetImageInfo(
                        CTRLICONLIST_OBJ_T           *iconlist_ctrl_ptr,  //in
                        uint16                       item_index,
                        GUIICONLIST_IMG_INFO_T       *img_info_ptr
                        );

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL uint32 RotateAngle(
                         GUIICONLIST_ROTATE_ANGLE_INPUT_T   *rotate_in_ptr, //in:
                         GUIIMG_BITMAP_T                    *rotate_out_ptr //in/out:
                         );

/*****************************************************************************/
//  Description : 运行动画
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  这个函数是动画的框架性函数，具体添加的帧由AddFrameFunc函数来实现
/*****************************************************************************/
LOCAL void RunAnimation(
                        CTRLICONLIST_OBJ_T          *iconlist_ctrl_ptr,
                        GUIICONLIST_ADD_FRAME_FUNC  AddFrameFunc
                        );

/*****************************************************************************/
//  Description : 添加file到image的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFile2ImgFrame(
                            CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                            uint16                    screen_width,
                            uint16                    screen_height
                            );

/*****************************************************************************/
//  Description : 添加image到file的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddImg2FileFrame(
                            CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                            uint16                    screen_width,
                            uint16                    screen_height
                            );

/*****************************************************************************/
//  Description : 添加folder到file的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFolder2FileFrame(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height
                               );

/*****************************************************************************/
//  Description : 添加file到folder的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFile2FolderFrame(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height
                               );

/*****************************************************************************/
//  Description : 配置file到image的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFile2ImgParam(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height,
                               MMIANIM_PARAM_T           *size_anim_param_ptr
                               );

/*****************************************************************************/
//  Description : 配置image到file的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigImg2FileParam(
                               CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                               uint16                item_index,
                               uint16                screen_width,
                               uint16                screen_height,
                               MMIANIM_PARAM_T       *size_anim_param_ptr
                               );

/*****************************************************************************/
//  Description : 配置folder到file的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFolder2FileParam(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                                  uint16      item_index,
                                  MMIANIM_PARAM_T       *size_anim_param_ptr
                                  );

/*****************************************************************************/
//  Description : 配置file到folder的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFile2FolderParam(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                                  uint16                folder_index,
                                  uint16                item_index,
                                  uint16                screen_width,
                                  uint16                screen_height,
                                  MMIANIM_PARAM_T       *size_anim_param_ptr
                                  );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO    s_traget_dev_info = {0};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : run file to image animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFile2Img(
                                    CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                    )
{
    RunAnimation(iconlist_ctrl_ptr, AddFile2ImgFrame);
}

/*****************************************************************************/
//  Description : run image to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunImg2File(
                                    CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                    )
{
    RunAnimation(iconlist_ctrl_ptr, AddImg2FileFrame);
}

/*****************************************************************************/
//  Description : run folder to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFolder2File(
                                       CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                       )
{
    RunAnimation(iconlist_ctrl_ptr, AddFolder2FileFrame);
}

/*****************************************************************************/
//  Description : run file to folder animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ICONLIST_RunFile2Folder(
                                       CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr  //in
                                       )
{
    RunAnimation(iconlist_ctrl_ptr, AddFile2FolderFrame);
}

/*****************************************************************************/
//  Description : 运行动画
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  这个函数是动画的框架性函数，具体添加的帧由AddFrameFunc函数来实现
/*****************************************************************************/
LOCAL void RunAnimation(
                        CTRLICONLIST_OBJ_T          *iconlist_ctrl_ptr,
                        GUIICONLIST_ADD_FRAME_FUNC  AddFrameFunc
                        )
{
    BOOLEAN                 result          = FALSE;
    uint16                  screen_width    = 0;
    uint16                  screen_height   = 0;
    GUI_RECT_T              screen_rect     = {0};
    MMIANIM_START_PARAM_T   start_param     = {0};

    if (PNULL != iconlist_ctrl_ptr
        &&iconlist_ctrl_ptr->total_num > 0
        && PNULL != iconlist_ctrl_ptr->item_ptr)
    {
        // 获取屏幕大小
        screen_rect = MMITHEME_GetFullScreenRect();
        screen_width = screen_rect.right - screen_rect.left + 1;
        screen_height = screen_rect.bottom - screen_rect.top + 1;

        // 创建层
        result = CreateIconListLayer(
                screen_width,
                screen_height,
                UILAYER_MEM_DOUBLE_COPY,
                &s_traget_dev_info);

        if (result)
        {
            AppendIconListLayer(&s_traget_dev_info);

            // 重置动画引擎
            MMIFRAME_Reset();

            // 执行各个动画函数，用于添加帧
            AddFrameFunc(iconlist_ctrl_ptr, screen_width, screen_height);

            // 开始动画
            start_param.is_async = FALSE;
            start_param.frame_count = GUIICONLIST_FRAME_COUNT;
            start_param.period = 10;

            MMIFRAME_Start(&start_param);

            // 最后再重置下
            MMIFRAME_Reset();

            // 释放层
            ReleaseIconListLayer(&s_traget_dev_info);
        }
    }
}

/*****************************************************************************/
//  Description : 添加file到image的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFile2ImgFrame(
                            CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                            uint16                    screen_width,
                            uint16                    screen_height
                            )
{
    MMIANIM_PARAM_T     size_anim_param     = {0};

    ConfigFile2ImgParam(iconlist_ctrl_ptr, screen_width, screen_height, &size_anim_param);

    MMIFRAME_AddAnim(&size_anim_param);
}

/*****************************************************************************/
//  Description : 添加image到file的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddImg2FileFrame(
                            CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                            uint16                    screen_width,
                            uint16                    screen_height
                            )
{
    int16               j                   = 0;
    uint16              item_count          = 0;
    MMIANIM_PARAM_T     size_anim_param     = {0};
    MMIANIM_PARAM_T     blank_anim_param    = {0};

    // 添加背景
    blank_anim_param.anim_type = MMIANIM_TYPE_BLANK;
    blank_anim_param.anim_func.DoAnimFunc = DoAnimationImg2FileBg;
    MMIFRAME_AddAnim(&blank_anim_param);

    item_count = iconlist_ctrl_ptr->bottom_index - iconlist_ctrl_ptr->top_index + 1;

    // 添加图标的动画
    for (j = (int16)(item_count - 1); j >= 0; j--)
    {
        ConfigImg2FileParam(
                iconlist_ctrl_ptr, iconlist_ctrl_ptr->top_index + j,
                screen_width, screen_height,
                &size_anim_param);

        MMIFRAME_AddAnim(&size_anim_param);
    }
}

/*****************************************************************************/
//  Description : 添加folder到file的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFolder2FileFrame(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height
                               )
{
    int16                       j                   = 0;
    uint16                      item_count          = 0;
    MMIANIM_PARAM_T             size_anim_param     = {0};
    MMIANIM_PARAM_T             blank_anim_param    = {0};

    if (PNULL == iconlist_ctrl_ptr->rotate_info_ptr)
    {
        return;
    }

    // 添加背景
    blank_anim_param.anim_type = MMIANIM_TYPE_BLANK;
    blank_anim_param.anim_func.DoAnimFunc = DoAnimationFolderFileBg;
    MMIFRAME_AddAnim(&blank_anim_param);

    // 添加图标的动画
    item_count = iconlist_ctrl_ptr->bottom_index - iconlist_ctrl_ptr->top_index + 1;

    for (j = (int16)(item_count - 1); j >= 0; j--)
    {
        ConfigFolder2FileParam(
                iconlist_ctrl_ptr,
                iconlist_ctrl_ptr->top_index + j,
                &size_anim_param);

        MMIFRAME_AddAnim(&size_anim_param);
    }
}

/*****************************************************************************/
//  Description : 添加file到folder的帧
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AddFile2FolderFrame(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height
                               )
{
    int16                       	i               = 0;
    int16                       	j               = 0;
    uint16                      	item_count      = 0;
    uint16                      	max_icon_num    = 0;
    uint16                      	icon_num        = 0;
    uint16                      	start_frame_index  = 0;
    uint16                      	folder_start_frame_index = 2;
    CTRLICONFOLDER_OBJ_T*  			iconfolder_ctrl_ptr = PNULL;
    ANIM_HANDLE_T               	anim_handle = 0;
    MMIANIM_PARAM_T             	blank_anim_param = {0};
    MMIANIM_PARAM_T             	size_anim_param  = {0};
    MMIANIM_INFO_T              	anim_info        = {0};

    if (PNULL == iconlist_ctrl_ptr->rotate_info_ptr)
    {
        return;
    }

    // 添加背景
    blank_anim_param.anim_type = MMIANIM_TYPE_BLANK;
    blank_anim_param.anim_func.DoAnimFunc = DoAnimationFolderFileBg;
    MMIFRAME_AddAnim(&blank_anim_param);

    // 获取一个文件中最多的icon数
    iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*)iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].icon.ctrl_add;
    max_icon_num = ICONFOLDER_GetMaxIconNum(iconfolder_ctrl_ptr);

    item_count = iconlist_ctrl_ptr->bottom_index - iconlist_ctrl_ptr->top_index + 1;

    // 添加图标的动画
    for (i = 0; i < item_count; i++)
    {
        start_frame_index = folder_start_frame_index;

        // 获取这个文件夹里的文件数
        iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*)iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->top_index + i].icon.ctrl_add;
        icon_num = ICONFOLDER_GetTotalNum(iconfolder_ctrl_ptr);

        if (icon_num > max_icon_num)
        {
            icon_num = max_icon_num;
        }

        // 文件夹控件里的各个icon
        for (j = icon_num - 1; j >= 0; j--)
        {
            ConfigFile2FolderParam(
                    iconlist_ctrl_ptr,
                    iconlist_ctrl_ptr->top_index + i, j,
                    screen_width, screen_height,
                    &size_anim_param);

            // 如果非当前文件夹，则延迟动画
            if (iconlist_ctrl_ptr->top_index + i != iconlist_ctrl_ptr->cur_index)
            {
                anim_handle = MMIFRAME_AddAnim(&size_anim_param);

                anim_info.start_frame_index = start_frame_index;
                anim_info.end_frame_index = start_frame_index + GUIICONLIST_FILE2FOLDER_COUNT;

                MMIANIM_SetInfo(anim_handle, &anim_info);

                start_frame_index ++;
            }
            else
            {
                MMIFRAME_AddAnim(&size_anim_param);
            }
        }

        folder_start_frame_index += 2;
    }
}

/*****************************************************************************/
//  Description : 配置file到image的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFile2ImgParam(
                               CTRLICONLIST_OBJ_T        *iconlist_ctrl_ptr,
                               uint16                    screen_width,
                               uint16                    screen_height,
                               MMIANIM_PARAM_T           *size_anim_param_ptr
                               )
{
    GUIICONLIST_SCALE_PARAM_T *anim_param_ptr   = PNULL;

    // 此内存在动画结束时，由引擎发出的回调里释放
    anim_param_ptr = SCI_ALLOC_APP(sizeof(MMIANIM_PARAM_T));
    SCI_MEMSET(anim_param_ptr, 0, sizeof(MMIANIM_PARAM_T));

    // 回调时的用户数据
    anim_param_ptr->iconlist_ctrl_ptr = iconlist_ctrl_ptr;

    // 获取图片信息
    GetImageInfo(
            iconlist_ctrl_ptr,
            iconlist_ctrl_ptr->cur_index,
            &anim_param_ptr->img_info);

    // 获取最大的缩放尺寸
    GetScaleMaxSize(
            iconlist_ctrl_ptr,
            (CTRLBASE_OBJ_T*)iconlist_ctrl_ptr->item_ptr[iconlist_ctrl_ptr->cur_index].icon.ctrl_add,
            screen_width, screen_height,
            &anim_param_ptr->max_scale_width,
            &anim_param_ptr->max_scale_height);

    // 设置动画参数
    size_anim_param_ptr->anim_type = MMIANIM_TYPE_SIZE;
    size_anim_param_ptr->anim_func.DoAnimFunc = DoAnimationFile2Img;
    size_anim_param_ptr->anim_func.AnimDoneFunc = DoAnimationEnd;
    size_anim_param_ptr->user_data = (uint32)anim_param_ptr;
    size_anim_param_ptr->add_param.size_anim.play_type = DAMP_PLAY_TYPE_1;

    // 起始位置
    GetSizeInFile(
            iconlist_ctrl_ptr, iconlist_ctrl_ptr->cur_index,
            &size_anim_param_ptr->add_param.size_anim.start_size);

    // 结束位置
    GetImgDstSize(
            iconlist_ctrl_ptr, screen_width, screen_height,
            &size_anim_param_ptr->add_param.size_anim.end_size);
}

/*****************************************************************************/
//  Description : 配置image到file的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigImg2FileParam(
                               CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                               uint16                item_index,
                               uint16                screen_width,
                               uint16                screen_height,
                               MMIANIM_PARAM_T       *size_anim_param_ptr
                               )
{
    GUI_SIZE_T                  start_size = {0};
    GUI_RECT_T                  start_rect = {0};
    GUIICONLIST_SCALE_PARAM_T   *anim_param_ptr   = PNULL;

    // 此内存在动画结束时，由引擎发出的回调里释放
    anim_param_ptr = SCI_ALLOC_APP(sizeof(MMIANIM_PARAM_T));
    SCI_MEMSET(anim_param_ptr, 0, sizeof(MMIANIM_PARAM_T));

    // 配置动画的用户数据
    anim_param_ptr->iconlist_ctrl_ptr = iconlist_ctrl_ptr;

    // 获取图片信息
    GetImageInfo(
            iconlist_ctrl_ptr,
            item_index,
            &anim_param_ptr->img_info);

    // 获取最大的缩放尺寸
    GetScaleMaxSize(
            iconlist_ctrl_ptr, 
            (CTRLBASE_OBJ_T*)iconlist_ctrl_ptr->item_ptr[item_index].icon.ctrl_add,
            screen_width, screen_height,
            &anim_param_ptr->max_scale_width,
            &anim_param_ptr->max_scale_height);

    // 计算开始的位置
    GetImgDstSize(iconlist_ctrl_ptr, screen_width, screen_height, &start_size);

    start_rect = Size2Rect(&start_size);
    start_rect.top = start_rect.top + (item_index - iconlist_ctrl_ptr->cur_index) * (GUIICONLIST_PIC_MARGIN + start_size.h);
    start_rect.bottom = start_rect.top + start_size.h - 1;

    // 配置动画参数
    size_anim_param_ptr->anim_type = MMIANIM_TYPE_SIZE;
    size_anim_param_ptr->anim_func.DoAnimFunc = DoAnimationImg2File;
    size_anim_param_ptr->anim_func.AnimDoneFunc = DoAnimationEnd;
    size_anim_param_ptr->user_data = (uint32)anim_param_ptr;

    size_anim_param_ptr->add_param.size_anim.play_type = DAMP_PLAY_TYPE_1;

    size_anim_param_ptr->add_param.size_anim.start_size = Rect2Size(&start_rect);

    GetSizeInFile(
            iconlist_ctrl_ptr, item_index,
            &size_anim_param_ptr->add_param.size_anim.end_size);
}

/*****************************************************************************/
//  Description : 配置folder到file的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFolder2FileParam(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                                  uint16                item_index,
                                  MMIANIM_PARAM_T       *size_anim_param_ptr
                                  )
{
    GUI_RECT_T                  temp_rect = {0};
    GUIICONLIST_ROTATE_PARAM_T  *anim_param_ptr     = PNULL;

    // 此内存在动画结束时，由引擎发出的回调里释放
    anim_param_ptr = SCI_ALLOC_APP(sizeof(GUIICONLIST_ROTATE_PARAM_T));
    SCI_MEMSET(anim_param_ptr, 0, sizeof(GUIICONLIST_ROTATE_PARAM_T));

    // 配置用户数据
    anim_param_ptr->iconlist_ctrl_ptr = iconlist_ctrl_ptr;

    // 如果索引号小于保存的数据，则用保存的数据
    if (item_index - iconlist_ctrl_ptr->top_index < iconlist_ctrl_ptr->rotate_info_count)
    {
        anim_param_ptr->rotate_info = iconlist_ctrl_ptr->rotate_info_ptr[item_index - iconlist_ctrl_ptr->top_index];
    }
    else
    {
        // 获取图片信息
        GetImageInfo(
                iconlist_ctrl_ptr,
                item_index,
                &anim_param_ptr->rotate_info.img_info);

        anim_param_ptr->rotate_info.dis_rect = iconlist_ctrl_ptr->rotate_info_ptr[0].dis_rect;
        anim_param_ptr->rotate_info.rotate_angle = 0;
    }

    // 配置动画参数
    size_anim_param_ptr->anim_type = MMIANIM_TYPE_SIZE;
    size_anim_param_ptr->anim_func.DoAnimFunc = DoAnimationFolderFile;
    size_anim_param_ptr->anim_func.AnimDoneFunc = DoAnimationEnd;
    size_anim_param_ptr->user_data = (uint32)anim_param_ptr;
    size_anim_param_ptr->add_param.size_anim.play_type = DAMP_PLAY_TYPE_1;

    // important
    // 这里虽然用了size的动画，但只用到其坐标信息，大小并没有用，其中w代表了角度

    // 开始位置
    size_anim_param_ptr->add_param.size_anim.start_size = Rect2Size(&anim_param_ptr->rotate_info.dis_rect);

    // 用宽来代替角度
    size_anim_param_ptr->add_param.size_anim.start_size.w = anim_param_ptr->rotate_info.rotate_angle;

    // 结束位置
    temp_rect = ICONLIST_GetIconRect(item_index, iconlist_ctrl_ptr);
    size_anim_param_ptr->add_param.size_anim.end_size = Rect2Size(&temp_rect);

    // 用宽来代替角度
    // 如果大于180度的，则转到360，否则转到0
    if (anim_param_ptr->rotate_info.rotate_angle > 180)
    {
        size_anim_param_ptr->add_param.size_anim.end_size.w = 360;
    }
    else
    {
        size_anim_param_ptr->add_param.size_anim.end_size.w = 0;
    }
}

/*****************************************************************************/
//  Description : 配置file到folder的动画参数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ConfigFile2FolderParam(
                                  CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                                  uint16                folder_index,
                                  uint16                item_index,
                                  uint16                screen_width,
                                  uint16                screen_height,
                                  MMIANIM_PARAM_T       *size_anim_param_ptr
                                  )
{
    int16                       offset_x = 0;
    int16                       offset_y = 0;
    uint16                      pos_x = 0;
    uint16                      pos_y = 0;
    GUI_RECT_T                  icon_rect = {0};
    GUI_SIZE_T                  end_size = {0};
    GUIICONLIST_IMG_INFO_T      img_info = {0};
    GUIICONLIST_ROTATE_PARAM_T  *anim_param_ptr = PNULL;
    GUIICONLIST_SCALE_PARAM_T   *scale_param_ptr = PNULL;
    CTRLICONFOLDER_OBJ_T		*iconfolder_ctrl_ptr  = PNULL;
    CTRLBASE_OBJ_T              *anim_ctrl_ptr = PNULL;

    // 获取文件夹控件，及其里面的动画控件
    iconfolder_ctrl_ptr = (CTRLICONFOLDER_OBJ_T*)iconlist_ctrl_ptr->item_ptr[folder_index].icon.ctrl_add;
    anim_ctrl_ptr = ICONFOLDER_GetAnimCtrl(iconfolder_ctrl_ptr, item_index);

    // 结束位置
    icon_rect = ICONFOLDER_GetIconRect(item_index, iconfolder_ctrl_ptr);
    end_size = Rect2Size(&icon_rect);

    // 如果是当前文件夹，则做旋转的动画
    if (folder_index == iconlist_ctrl_ptr->cur_index)
    {
        // 拿原始的图片数据
        if (PNULL != anim_ctrl_ptr)
        {
            // 获取动画控件里的图片信息
            GUIANIM_GetImgBuf(
                    GUICTRL_GetCtrlHandle((IGUICTRL_T*)anim_ctrl_ptr),
                    &img_info.img_bitmap,
                    &img_info.img_id);
        }
        else
        {
            img_info.img_id = iconlist_ctrl_ptr->theme.default_icon_id;
        }

        // 配置用户数据
        // 此内存在动画结束时，由引擎发出的回调里释放
        anim_param_ptr = SCI_ALLOC_APP(sizeof(GUIICONLIST_ROTATE_PARAM_T));
        SCI_MEMSET(anim_param_ptr, 0, sizeof(GUIICONLIST_ROTATE_PARAM_T));

        anim_param_ptr->iconlist_ctrl_ptr = iconlist_ctrl_ptr;
        anim_param_ptr->rotate_info.img_info = img_info;

        // 配置动画参数
        size_anim_param_ptr->anim_type = MMIANIM_TYPE_SIZE;
        size_anim_param_ptr->user_data = (uint32)anim_param_ptr;
        size_anim_param_ptr->anim_func.DoAnimFunc = DoAnimationFolderFile;
        size_anim_param_ptr->anim_func.AnimDoneFunc = DoAnimationEnd;
        size_anim_param_ptr->add_param.size_anim.play_type = DAMP_PLAY_TYPE_1;

        size_anim_param_ptr->add_param.size_anim.end_size = end_size;
        size_anim_param_ptr->add_param.size_anim.end_size.w = ICONFOLDER_GetIconAngle(item_index, iconfolder_ctrl_ptr);

        // 当前文件夹的位置信息已经保存
        size_anim_param_ptr->add_param.size_anim.start_size = Rect2Size(&iconlist_ctrl_ptr->rotate_info_ptr[item_index].dis_rect);

        // important
        // 这里虽然用了size的动画，但只用到其坐标信息，大小并没有用，其中w代表了角度
        if (size_anim_param_ptr->add_param.size_anim.end_size.w > 180)
        {
            size_anim_param_ptr->add_param.size_anim.start_size.w = 360;
        }
        else
        {
            size_anim_param_ptr->add_param.size_anim.start_size.w = 0;
        }
    }
    else
    {
        // 拿动画控件里旋转后的数据
        if (PNULL != anim_ctrl_ptr)
        {
            GUIANIM_GetRotateImage(
                    GUICTRL_GetCtrlHandle((IGUICTRL_T*)anim_ctrl_ptr),
                    &img_info.img_bitmap,
                    &img_info.img_id);
        }
        else
        {
            img_info.img_id = iconlist_ctrl_ptr->theme.default_icon_id;
        }

        // 配置用户数据
        scale_param_ptr = SCI_ALLOC_APP(sizeof(GUIICONLIST_SCALE_PARAM_T));
        SCI_MEMSET(scale_param_ptr, 0, sizeof(GUIICONLIST_SCALE_PARAM_T));

        scale_param_ptr->iconlist_ctrl_ptr = iconlist_ctrl_ptr;
        scale_param_ptr->img_info = img_info;
        GetScaleMaxSize(
                iconlist_ctrl_ptr, anim_ctrl_ptr,
                screen_width, screen_height,
                &scale_param_ptr->max_scale_width,
                &scale_param_ptr->max_scale_height);

        // 配置动画参数
        size_anim_param_ptr->anim_type = MMIANIM_TYPE_SIZE;
        size_anim_param_ptr->user_data = (uint32)scale_param_ptr;
        size_anim_param_ptr->anim_func.DoAnimFunc = DoAnimationScaleImage;      // 一开始做缩放动画，后再做平移
        size_anim_param_ptr->anim_func.AnimDoneFunc = DoAnimationEnd;
        size_anim_param_ptr->add_param.size_anim.play_type = DAMP_PLAY_TYPE_1;

        // 加上一定的偏移
        pos_x = folder_index % iconlist_ctrl_ptr->col_num_page;
        pos_y = folder_index / iconlist_ctrl_ptr->row_num_page;

        if (0 == pos_x)
        {
            offset_x = -iconlist_ctrl_ptr->theme.icon_item.width;
        }
        else if (iconlist_ctrl_ptr->col_num_page - 1 == pos_x)
        {
            offset_x = iconlist_ctrl_ptr->theme.icon_item.width;
        }

        if (0 == pos_y)
        {
            offset_y = -iconlist_ctrl_ptr->theme.icon_item.height;
        }
        else if (iconlist_ctrl_ptr->row_num_page - 1 == pos_y)
        {
            offset_y = iconlist_ctrl_ptr->theme.icon_item.height;
        }

        size_anim_param_ptr->add_param.size_anim.start_size = end_size;
        size_anim_param_ptr->add_param.size_anim.start_size.x += offset_x;
        size_anim_param_ptr->add_param.size_anim.start_size.y += offset_y;
        size_anim_param_ptr->add_param.size_anim.start_size.w = end_size.w * 3 / 2;
        size_anim_param_ptr->add_param.size_anim.start_size.h = end_size.h * 3 / 2;

        size_anim_param_ptr->add_param.size_anim.end_size = end_size;
    }
}

/*****************************************************************************/
//  Description : do file to image view animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFile2Img(
                               ANIM_HANDLE_T            anim_handle,
                               uint16                   frame_index,
                               MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                               )
{
    GUI_SIZE_T                  *cur_size_ptr = &callback_param_ptr->size_anim.cur_size;
    GUIICONLIST_SCALE_PARAM_T   *user_data_ptr = (GUIICONLIST_SCALE_PARAM_T*)callback_param_ptr->size_anim.user_data;

    LCD_FillArgbRect(
        (const GUI_LCD_DEV_INFO*)&s_traget_dev_info,
        MMITHEME_GetFullScreenRect(),
        GUIICONLIST_BG_COLOR,
        255 * frame_index / GUIICONLIST_FRAME_COUNT);

    DrawScaleImage(user_data_ptr, cur_size_ptr);
}

/*****************************************************************************/
//  Description : do image to file background animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationImg2FileBg(
                                 ANIM_HANDLE_T              anim_handle,
                                 uint16                     frame_index,
                                 MMIANIM_CALLBACK_PARAM_U   *callback_param_ptr
                                 )
{
    // 之前填单色，之后再逐渐变亮
    if (frame_index < GUIICONLIST_IMG2FILE_FILL_COUNT)
    {
        LCD_FillRect(&s_traget_dev_info, MMITHEME_GetFullScreenRect(), GUIICONLIST_BG_COLOR);
    }
    else
    {
        LCD_FillArgbRect(
            (const GUI_LCD_DEV_INFO*)&s_traget_dev_info,
            MMITHEME_GetFullScreenRect(),
            GUIICONLIST_BG_COLOR,
            255 * (GUIICONLIST_FRAME_COUNT - frame_index) / (GUIICONLIST_FRAME_COUNT - GUIICONLIST_IMG2FILE_FILL_COUNT));
    }
}

/*****************************************************************************/
//  Description : do image to file animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationImg2File(
                               ANIM_HANDLE_T            anim_handle,
                               uint16                   frame_index,
                               MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                               )
{
    GUI_SIZE_T                  *cur_size_ptr = &callback_param_ptr->size_anim.cur_size;
    GUIICONLIST_SCALE_PARAM_T   *user_data_ptr = (GUIICONLIST_SCALE_PARAM_T*)callback_param_ptr->size_anim.user_data;

    if (frame_index < GUIICONLIST_IMG2FILE_BORDER_COUNT)
    {
        LCD_DrawRect((const GUI_LCD_DEV_INFO*)&s_traget_dev_info, Size2Rect(cur_size_ptr), GUIICONLIST_BORDER_COLOR);
    }
    else
    {
        DrawScaleImage(user_data_ptr, cur_size_ptr);
    }
}

/*****************************************************************************/
//  Description : do image move and rotate background animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFolderFileBg(
                                   ANIM_HANDLE_T             anim_handle,
                                   uint16                    frame_index,
                                   MMIANIM_CALLBACK_PARAM_U  *callback_param_ptr
                                   )
{
    UILAYER_Clear(&s_traget_dev_info);
}

/*****************************************************************************/
//  Description : do scale image animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationScaleImage(
                                 ANIM_HANDLE_T            anim_handle,
                                 uint16                   frame_index,
                                 MMIANIM_CALLBACK_PARAM_U *callback_param_ptr
                                 )
{
    MMIANIM_PARAM_T             anim_param = {0};
    GUI_SIZE_T                  *cur_size_ptr = &callback_param_ptr->size_anim.cur_size;
    GUIICONLIST_SCALE_PARAM_T   *user_data_ptr = (GUIICONLIST_SCALE_PARAM_T*)callback_param_ptr->size_anim.user_data;
    GUIICONLIST_ROTATE_PARAM_T  *rotate_user_data_ptr = PNULL;

    DrawScaleImage(user_data_ptr, cur_size_ptr);

    // 如果缩放进行了2帧，则做平移动画
    user_data_ptr->do_count++;

    if (GUIICONLIST_FILE2FOLDER_SCALE_COUNT == user_data_ptr->do_count)
    {
        // 配置平移动画的用户数据
        rotate_user_data_ptr = SCI_ALLOC_APP(sizeof(GUIICONLIST_ROTATE_PARAM_T));
        SCI_MEMSET(rotate_user_data_ptr, 0, sizeof(GUIICONLIST_ROTATE_PARAM_T));

        rotate_user_data_ptr->iconlist_ctrl_ptr = user_data_ptr->iconlist_ctrl_ptr;
        rotate_user_data_ptr->rotate_info.img_info = user_data_ptr->img_info;

        // 释放掉之前分配的内存
        DoAnimationEnd(anim_handle, (uint32)user_data_ptr);

        // 获取当前的动画参数，并重新设置
        anim_param = MMIFRAME_GetAnimParam(anim_handle);

        anim_param.user_data = (uint32)rotate_user_data_ptr;
        anim_param.anim_func.DoAnimFunc = DoAnimationFolderFile;
        anim_param.add_param.size_anim.start_size.w = 0;
        anim_param.add_param.size_anim.end_size.w = 0;

        MMIFRAME_SetAnimParam(anim_handle, &anim_param);
    }
}

/*****************************************************************************/
//  Description : do image move and rotate animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationFolderFile(
                                 ANIM_HANDLE_T             anim_handle,
                                 uint16                    frame_index,
                                 MMIANIM_CALLBACK_PARAM_U  *callback_param_ptr
                                 )
{
    GUI_SIZE_T                  *cur_size_ptr = &callback_param_ptr->size_anim.cur_size;
    GUIICONLIST_ROTATE_PARAM_T  *user_data_ptr = (GUIICONLIST_ROTATE_PARAM_T*)callback_param_ptr->size_anim.user_data;
    CTRLICONLIST_OBJ_T          *iconlist_ctrl_ptr  = user_data_ptr->iconlist_ctrl_ptr;
    GUI_RECT_T                  cur_rect   = {0};
    GUIIMG_BITMAP_T             bitmap_info = {0};
    GUIICONLIST_ROTATE_ANGLE_INPUT_T    rotate_input = {0};

    // 有数据的情况
    if (PNULL != user_data_ptr->rotate_info.img_info.img_bitmap.bit_ptr
        && user_data_ptr->rotate_info.img_info.img_bitmap.img_width > 0
        && user_data_ptr->rotate_info.img_info.img_bitmap.img_height > 0)
    {
        // 0度和360度，直接画
        if (0 == cur_size_ptr->w || 360 == cur_size_ptr->w)
        {
            cur_rect = MakeRect(
                    cur_size_ptr->x,
                    cur_size_ptr->y,
                    user_data_ptr->rotate_info.img_info.img_bitmap.img_width,
                    user_data_ptr->rotate_info.img_info.img_bitmap.img_height);

            DrawImageBitmap(iconlist_ctrl_ptr, &cur_rect, &user_data_ptr->rotate_info.img_info.img_bitmap);
        }
        else
        {
            //set src
            rotate_input.src_buf_ptr    = user_data_ptr->rotate_info.img_info.img_bitmap.bit_ptr;
            rotate_input.src_img_width  = user_data_ptr->rotate_info.img_info.img_bitmap.img_width;
            rotate_input.src_img_height = user_data_ptr->rotate_info.img_info.img_bitmap.img_height;
            if (GUIIMG_DATE_TYPE_ARGB888 == user_data_ptr->rotate_info.img_info.img_bitmap.date_type)
            {
                rotate_input.src_buf_size   = rotate_input.src_img_width*rotate_input.src_img_height*4;
            }
            else
            {
                rotate_input.src_buf_size   = rotate_input.src_img_width*rotate_input.src_img_height*2;
            }
            rotate_input.angle          = cur_size_ptr->w;
            rotate_input.src_data_type  = user_data_ptr->rotate_info.img_info.img_bitmap.date_type;

            //set dest width and height
            rotate_input.dest_img_width  = (uint16)(rotate_input.src_img_width + rotate_input.src_img_height);
            rotate_input.dest_img_height = (uint16)(rotate_input.src_img_width + rotate_input.src_img_height);

            //alloc rotate buffer,size 32bit对齐
            rotate_input.dest_buf_size = (uint32)(rotate_input.dest_img_width*rotate_input.dest_img_height*4 + 256);
            rotate_input.dest_buf_size = (uint32)GUIICONLIST_ALIGN_EX(rotate_input.dest_buf_size,32);
            rotate_input.dest_buf_ptr  = SCI_ALLOCA(rotate_input.dest_buf_size);

            if (PNULL != rotate_input.dest_buf_ptr)
            {
                //reset buffer
                SCI_MEMSET(rotate_input.dest_buf_ptr,0,rotate_input.dest_buf_size);

                //rotate image
                if (SCI_SUCCESS == RotateAngle(&rotate_input,&bitmap_info))
                {
                    cur_rect = MakeRect(
                            cur_size_ptr->x,
                            cur_size_ptr->y,
                            bitmap_info.img_width,
                            bitmap_info.img_height);

                    DrawImageBitmap(iconlist_ctrl_ptr, &cur_rect, &bitmap_info);
                }

                SCI_FREE(rotate_input.dest_buf_ptr);
            }
        }
    }
    else if (0 != user_data_ptr->rotate_info.img_info.img_id)   // 没有，就画默认图标
    {
        cur_rect = MakeRect(
                cur_size_ptr->x,
                cur_size_ptr->y,
                iconlist_ctrl_ptr->theme.icon_item.width,
                iconlist_ctrl_ptr->theme.icon_item.height);

        DrawDefaultImage(
                iconlist_ctrl_ptr,
                &cur_rect,
                user_data_ptr->rotate_info.img_info.img_id);
    }
}

/*****************************************************************************/
//  Description : 动画结束时的回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoAnimationEnd(
                          ANIM_HANDLE_T anim_handle,
                          uint32        user_data
                          )
{
    void *p = (void*)user_data;
    
    if (PNULL != p)
    {
        SCI_FREE(p);
    }
}
/*****************************************************************************/
//  Description : 画缩放后的图片到目标层
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawScaleImage(
                          GUIICONLIST_SCALE_PARAM_T *user_data_ptr,
                          GUI_SIZE_T                *cur_size_ptr
                          )
{
    uint16                      byte_per_pixel  = 0;
    IMGREF_DATA_FORMAT_E        buf_format      = 0;
    GUI_RECT_T                  cur_rect        = {0};
    SCALE_IMAGE_IN_T            scale_in        = {0};
    SCALE_IMAGE_OUT_T           scale_out       = {0};
    SCALE_IMAGE_IN_T            temp_scale_in   = {0};
    SCALE_IMAGE_OUT_T           temp_scale_out  = {0};
    GUIIMG_BITMAP_T             src_bitmap      = {0};
    GUIIMG_BITMAP_T             bitmap_info     = {0};

    // 有数据的情况
    if (PNULL != user_data_ptr->img_info.img_bitmap.bit_ptr
        && user_data_ptr->img_info.img_bitmap.img_width > 0
        && user_data_ptr->img_info.img_bitmap.img_height > 0)
    {
        // check下是否到了最大缩放值
        if (cur_size_ptr->w > user_data_ptr->max_scale_width
            || cur_size_ptr->h > user_data_ptr->max_scale_height)
        {
            cur_size_ptr->w = user_data_ptr->max_scale_width;
            cur_size_ptr->h = user_data_ptr->max_scale_height;
        }

        // 如果宽高一样的话，就直接画了，不用缩放
        if (cur_size_ptr->w == user_data_ptr->img_info.img_bitmap.img_width
            && cur_size_ptr->h == user_data_ptr->img_info.img_bitmap.img_height)
        {
            bitmap_info = user_data_ptr->img_info.img_bitmap;
        }
        else
        {
            switch (user_data_ptr->img_info.img_bitmap.date_type)
            {
            case GUIIMG_DATE_TYPE_RGB565:
                buf_format = IMGREF_FORMAT_RGB565;
                byte_per_pixel = 2;
                break;

            case GUIIMG_DATE_TYPE_ARGB888:
            default:
                buf_format = IMGREF_FORMAT_ARGB888;
                byte_per_pixel = 4;
                break;
            }

            // 由于底层的缩放函数最大支持4倍缩放，所以，如果大于4倍的话，还需临时
            // 放大一次。
            //  TODO: 可能放大一次也不够
            if (user_data_ptr->img_info.img_bitmap.img_width * 4 < cur_size_ptr->w
                || user_data_ptr->img_info.img_bitmap.img_height * 4 < cur_size_ptr->h)
            {
                temp_scale_in.src_format           = buf_format;
                temp_scale_in.src_chn.chn0.ptr     = user_data_ptr->img_info.img_bitmap.bit_ptr;
                temp_scale_in.src_chn.chn0.size    = user_data_ptr->img_info.img_bitmap.img_width * user_data_ptr->img_info.img_bitmap.img_height * byte_per_pixel;
                temp_scale_in.src_size.w           = user_data_ptr->img_info.img_bitmap.img_width;
                temp_scale_in.src_size.h           = user_data_ptr->img_info.img_bitmap.img_height;
                temp_scale_in.src_trim_rect.x      = 0;
                temp_scale_in.src_trim_rect.y      = 0;
                temp_scale_in.src_trim_rect.w      = user_data_ptr->img_info.img_bitmap.img_width;
                temp_scale_in.src_trim_rect.h      = user_data_ptr->img_info.img_bitmap.img_height;

                // 根据图片的宽高比与目标尺寸的宽高比，来决定缩放的尺寸
                if (user_data_ptr->img_info.img_bitmap.img_width * cur_size_ptr->h > cur_size_ptr->w * user_data_ptr->img_info.img_bitmap.img_height)
                {
                    temp_scale_in.target_size.h    = user_data_ptr->img_info.img_bitmap.img_height * 4;
                    temp_scale_in.target_size.w    = temp_scale_in.target_size.h * cur_size_ptr->w / cur_size_ptr->h;
                }
                else
                {
                    temp_scale_in.target_size.w    = user_data_ptr->img_info.img_bitmap.img_width * 4;
                    temp_scale_in.target_size.h    = temp_scale_in.target_size.w * cur_size_ptr->h / cur_size_ptr->w;
                }
                temp_scale_in.target_buf.size      = temp_scale_in.target_size.w * temp_scale_in.target_size.h * byte_per_pixel + 255;
                temp_scale_in.target_buf.ptr       = SCI_ALLOCA(temp_scale_in.target_buf.size);
                temp_scale_in.target_format        = buf_format;

                // 进行缩放
                if (PNULL != temp_scale_in.target_buf.ptr
                    && SCI_SUCCESS == GRAPH_ScaleImage(&temp_scale_in, &temp_scale_out))
                {
                    src_bitmap.bit_ptr = temp_scale_out.output_chn.chn0.ptr;
                    src_bitmap.img_width = temp_scale_out.output_size.w;
                    src_bitmap.img_height = temp_scale_out.output_size.h;
                }
            }
            else
            {
                src_bitmap = user_data_ptr->img_info.img_bitmap;
            }

            if (PNULL != src_bitmap.bit_ptr)
            {
                scale_in.src_format           = buf_format;
                scale_in.src_chn.chn0.ptr     = src_bitmap.bit_ptr;
                scale_in.src_chn.chn0.size    = src_bitmap.img_width * src_bitmap.img_height * byte_per_pixel;
                scale_in.src_size.w           = src_bitmap.img_width;
                scale_in.src_size.h           = src_bitmap.img_height;
                scale_in.src_trim_rect.x      = 0;
                scale_in.src_trim_rect.y      = 0;
                scale_in.src_trim_rect.w      = src_bitmap.img_width;
                scale_in.src_trim_rect.h      = src_bitmap.img_height;

                scale_in.target_format        = buf_format;
                scale_in.target_size.w        = cur_size_ptr->w;
                scale_in.target_size.h        = cur_size_ptr->h;
                scale_in.target_buf.size      = scale_in.target_size.w * scale_in.target_size.h * byte_per_pixel + 255;
                scale_in.target_buf.ptr       = SCI_ALLOCA(scale_in.target_buf.size);

                // 进行缩放
                if (PNULL != scale_in.target_buf.ptr
                    && SCI_SUCCESS == GRAPH_ScaleImage(&scale_in, &scale_out))
                {
                    bitmap_info.bit_ptr = scale_out.output_chn.chn0.ptr;
                    bitmap_info.img_width = scale_out.output_size.w;
                    bitmap_info.img_height = scale_out.output_size.h;
                    bitmap_info.date_type = user_data_ptr->img_info.img_bitmap.date_type;
                }
            }
        }

        if (PNULL != bitmap_info.bit_ptr)
        {
            cur_rect = Size2Rect(cur_size_ptr);

            DrawImageBitmap(
                    user_data_ptr->iconlist_ctrl_ptr,
                    &cur_rect,
                    &bitmap_info);
        }

        if (PNULL != scale_in.target_buf.ptr)
        {
            SCI_FREE(scale_in.target_buf.ptr);
        }

        if (PNULL != temp_scale_in.target_buf.ptr)
        {
            SCI_FREE(temp_scale_in.target_buf.ptr);
        }
    }
    else
    {
        cur_rect = Size2Rect(cur_size_ptr);

        DrawDefaultImage(
                user_data_ptr->iconlist_ctrl_ptr,
                &cur_rect,
                user_data_ptr->img_info.img_id);
    }
}

/*****************************************************************************/
//  Description : 绘制bitmap的图像
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawImageBitmap(
                           CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                           GUI_RECT_T          *rect_ptr,
                           GUIIMG_BITMAP_T     *bitmap_ptr
                           )
{
    GUI_POINT_T     pt = {0};
    GUI_RECT_T      cur_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = PNULL;

    base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    cur_rect = *rect_ptr;

    pt.x = rect_ptr->left;
    pt.y = rect_ptr->top;

    // 先判断是不是从文件到图像，这个特效可能会超出原控件的区域，所以不需要求交
    if (GUIICONLIST_SPECIAL_FILE_TO_IMAGE == iconlist_ctrl_ptr->specail_type
        || GUI_IntersectRect(&cur_rect, cur_rect, base_ctrl_ptr->rect))
    {
        // 画到目标层上
        GUIIMG_DisplayBmp(FALSE, &cur_rect, &pt, bitmap_ptr, &s_traget_dev_info);
    }
}

/*****************************************************************************/
//  Description : 绘制默认的图片
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DrawDefaultImage(
                            CTRLICONLIST_OBJ_T  *iconlist_ctrl_ptr,
                            GUI_RECT_T          *rect_ptr,
                            MMI_IMAGE_ID_T      image_id
                            )
{
    uint16          default_width = 0;
    uint16          default_height = 0;
    GUI_RECT_T      temp_rect = {0};
    GUI_RECT_T      cur_rect = {0};
    GUI_RECT_T      img_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) iconlist_ctrl_ptr;

    if (GUIRES_GetImgWidthHeight(
            &default_width,
            &default_height,
            image_id,
            iconlist_ctrl_ptr->win_id))
    {
        temp_rect = *rect_ptr;
        temp_rect = GUI_GetCenterRect(temp_rect, default_width, default_height);

        if (GUI_IntersectRect(&cur_rect, temp_rect, base_ctrl_ptr->rect))
        {
            img_rect.left = cur_rect.left - temp_rect.left;
            img_rect.top = cur_rect.top - temp_rect.top;
            img_rect.right = cur_rect.right - temp_rect.left;
            img_rect.bottom = cur_rect.bottom - temp_rect.top;

            //display default image icon
            IMG_EnableTransparentColor(TRUE);

            GUIRES_DisplayImg(
                PNULL,
                &cur_rect,
                &img_rect,
                iconlist_ctrl_ptr->win_id,
                image_id,
                &s_traget_dev_info);

            IMG_EnableTransparentColor(FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : 获取动画控件里图像的原始大小
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetImageWidthHeight(
                               MMI_HANDLE_T         anim_ctrl_handle,
                               uint16               *img_width_ptr,
                               uint16               *img_height_ptr
                               )
{
    GUIANIM_GetSrcImgWidthHeight(anim_ctrl_handle, img_width_ptr, img_height_ptr);
}

/*****************************************************************************/
//  Description : 获取指定图像的信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
//  只能在ICON模式下使用
/*****************************************************************************/
LOCAL void GetImageInfo(
                        CTRLICONLIST_OBJ_T           *iconlist_ctrl_ptr,  //in
                        uint16                       item_index,
                        GUIICONLIST_IMG_INFO_T       *img_info_ptr
                        )
{
    GUIICONLIST_ICON_T  *icon_ptr = &iconlist_ctrl_ptr->item_ptr[item_index].icon;

    SCI_ASSERT(GUIICONLIST_STYLE_ICON == iconlist_ctrl_ptr->style       /*assert verified*/
            || GUIICONLIST_STYLE_ICON_UIDT == iconlist_ctrl_ptr->style);

    if (0 != icon_ptr->ctrl_add)
    {
        // 获取动画控件里的图片信息
        GUIANIM_GetImgBuf(
                GUICTRL_GetCtrlHandle((IGUICTRL_T*)icon_ptr->ctrl_add),
                &img_info_ptr->img_bitmap,
                &img_info_ptr->img_id);
    }
    else
    {
        img_info_ptr->img_id = iconlist_ctrl_ptr->theme.default_icon_id;
    }
}

/*****************************************************************************/
//  Description : 获取在图像界面里的位置信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetImgDstSize(
                         CTRLICONLIST_OBJ_T     *iconlist_ctrl_ptr,  //in
                         uint16                 screen_width,
                         uint16                 screen_height,
                         GUI_SIZE_T             *size_ptr
                         )
{
    size_ptr->x = screen_width / 2;
    size_ptr->y = screen_height / 2;

    if (screen_width * iconlist_ctrl_ptr->theme.icon_item.height > screen_height * iconlist_ctrl_ptr->theme.icon_item.width)
    {
        size_ptr->h = screen_height;
        size_ptr->w = screen_height * iconlist_ctrl_ptr->theme.icon_item.width / iconlist_ctrl_ptr->theme.icon_item.height;
    }
    else
    {
        size_ptr->w = screen_width;
        size_ptr->h = screen_width * iconlist_ctrl_ptr->theme.icon_item.height / iconlist_ctrl_ptr->theme.icon_item.width;
    }
}

/*****************************************************************************/
//  Description : 获取在文件iconlist里的位置信息
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetSizeInFile(
                         CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                         uint16                item_index,
                         GUI_SIZE_T            *src_size_ptr
                         )
{
    GUI_RECT_T      temp_rect   = {0};

    temp_rect = ICONLIST_GetIconRect(item_index, iconlist_ctrl_ptr);
    *src_size_ptr = Rect2Size(&temp_rect);
}

/*****************************************************************************/
//  Description : 获取最大的放大尺寸
//  Global resource dependence :
//  Author:hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetScaleMaxSize(
                           CTRLICONLIST_OBJ_T    *iconlist_ctrl_ptr,  //in
                           CTRLBASE_OBJ_T        *anim_ctrl_ptr,
                           uint16                screen_width,
                           uint16                screen_height,
                           uint16                *max_width_ptr,
                           uint16                *max_height_ptr
                           )
{
    uint16                  img_width           = 0;
    uint16                  img_height          = 0;
    uint16                  max_width           = 0;
    uint16                  max_height          = 0;
    MMI_HANDLE_T            anim_ctrl_handle    = 0;
    GUIICONLIST_IMG_INFO_T  img_info            = {0};

    if (0 != anim_ctrl_ptr)
    {
        anim_ctrl_handle = GUICTRL_GetCtrlHandle((IGUICTRL_T*)anim_ctrl_ptr);

        GetImageWidthHeight(anim_ctrl_handle, &img_width, &img_height);

        //  拿到原始图片大小后，目标尺寸也需要根据它来自适应，如果图片大小容
        // 不下，则设置一个最大的尺寸，否则要按照图片的大小来
        if (img_width < iconlist_ctrl_ptr->theme.icon_item.width
            || img_height < iconlist_ctrl_ptr->theme.icon_item.height)
        {
            // 获取动画控件里的图片信息
            GUIANIM_GetImgBuf(
                    anim_ctrl_handle,
                    &img_info.img_bitmap,
                    PNULL);

            max_width = img_info.img_bitmap.img_width;
            max_height = img_info.img_bitmap.img_height;
        }
        else
        {
            // 先将图片放到将要显示的位置
            if (img_width >= screen_width && img_height >= screen_height)
            {
                if ((uint32)img_width * screen_height > (uint32)img_height * screen_width)
                {
                    max_width = screen_width;
                    max_height = screen_width * img_height / img_width;
                }
                else
                {
                    max_width = screen_height * img_width / img_height;
                    max_height = screen_height;
                }
            }
            else if (img_width > screen_width)
            {
                max_width = screen_width;
                max_height = screen_width * img_height / img_width;
            }
            else if (img_height > screen_height)
            {
                max_width = screen_height * img_width / img_height;
                max_height = screen_height;
            }
            else
            {
                max_width = img_width;
                max_height = img_height;
            }

            // 按照比例截取
            if ((uint32)iconlist_ctrl_ptr->theme.icon_item.height * max_width > (uint32)iconlist_ctrl_ptr->theme.icon_item.width * max_height)
            {
                max_width = max_height * iconlist_ctrl_ptr->theme.icon_item.width / iconlist_ctrl_ptr->theme.icon_item.height;
            }
            else
            {
                max_height = (uint32)max_width * iconlist_ctrl_ptr->theme.icon_item.height / iconlist_ctrl_ptr->theme.icon_item.width;
            }
        }
    }
    else
    {
        GUIRES_GetImgWidthHeight(
                &max_width,
                &max_height,
                iconlist_ctrl_ptr->theme.default_icon_id,
                iconlist_ctrl_ptr->win_id);
    }

    if (PNULL != max_width_ptr)
    {
        *max_width_ptr = max_width;
    }

    if (PNULL != max_height_ptr)
    {
        *max_height_ptr = max_height;
    }
}

/*****************************************************************************/
//  Description : create layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateIconListLayer(
                                  int16                  width,              // in
                                  int16                  height,             // in
                                  UILAYER_MEM_FORMAT_T   mem_format,         // in
                                  GUI_LCD_DEV_INFO       *lcd_dev_info_ptr   // out
                                  )
{
    BOOLEAN             result = FALSE;
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;
    UILAYER_CREATE_T    create_info = {0};

    if (PNULL != lcd_dev_info_ptr
        && UILAYER_IsMultiLayerEnable())
    {
        if (0 == lcd_dev_info_ptr->block_id || UILAYER_NULL_HANDLE == lcd_dev_info_ptr->block_id)
        {
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = 0;
            create_info.offset_x = 0;
            create_info.offset_y = 0;
            create_info.width = width;
            create_info.height = height;
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = TRUE;
            create_info.format = mem_format;

            layer_result = UILAYER_CreateLayer(&create_info, lcd_dev_info_ptr);

            if (UILAYER_RESULT_SUCCESS == layer_result)
            {
                UILAYER_SetLayerColorKey(lcd_dev_info_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
                UILAYER_Clear(lcd_dev_info_ptr);

                result = TRUE;
            }
        }
        else
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : release layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseIconListLayer(
                                GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                                )
{
    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RELEASELAYER(lcd_dev_info_ptr);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(lcd_dev_info_ptr);
//
//        lcd_dev_info_ptr->lcd_id = GUI_MAIN_LCD_ID;
//        lcd_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
    }
}

/*****************************************************************************/
//  Description : append layer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendIconListLayer(
                               GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                               )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        append_layer.lcd_dev_info = *lcd_dev_info_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;

        UILAYER_AppendBltLayer(&append_layer);
    }
}

/*****************************************************************************/
//  Description : size to rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_RECT_T Size2Rect(
                                    GUI_SIZE_T *size_ptr
                                    )
{
    GUI_RECT_T  rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != size_ptr);      /*assert verified*/
    if (PNULL != size_ptr)
    {
        rect.left = size_ptr->x - size_ptr->w / 2;
        rect.right = rect.left + size_ptr->w - 1;

        rect.top = size_ptr->y - size_ptr->h / 2;
        rect.bottom = rect.top + size_ptr->h - 1;
    }
    
    return rect;
}

/*****************************************************************************/
//  Description : rect to size
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_SIZE_T Rect2Size(
                                    GUI_RECT_T *rect_ptr
                                    )
{
    GUI_SIZE_T size = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != rect_ptr);      /*assert verified*/
    if (PNULL != rect_ptr)
    {
        size.x = (rect_ptr->left + rect_ptr->right) / 2;
        size.y = (rect_ptr->top + rect_ptr->bottom) / 2;
        size.w = (rect_ptr->right - rect_ptr->left + 1);
        size.h = (rect_ptr->bottom - rect_ptr->top + 1);
    }

    return size;
}

/*****************************************************************************/
//  Description : make rect
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_RECT_T MakeRect(
                                   int16        center_x,
                                   int16        center_y,
                                   uint16       w,
                                   uint16       h
                                   )
{
    GUI_RECT_T  rect = {0};

    rect.left = center_x - w / 2;
    rect.right = rect.left + w - 1;

    rect.top = center_y - h / 2;
    rect.bottom = rect.top + h - 1;

    return rect;
}

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL uint32 RotateAngle(
                         GUIICONLIST_ROTATE_ANGLE_INPUT_T   *rotate_in_ptr, //in:
                         GUIIMG_BITMAP_T                    *rotate_out_ptr //in/out:
                         )
{
    uint32                  rotate_result = 0;
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
        SCI_PASSERT(FALSE,("RotateAngle:src_data_type %d is error!",rotate_in_ptr->src_data_type)); /*assert verified*/
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
    if (SCI_SUCCESS == rotate_result)
    {
        //set image width and height
        rotate_out_ptr->img_width  = rotate_out.output_size.w;
        rotate_out_ptr->img_height = rotate_out.output_size.h;

        //set target data type
        rotate_out_ptr->date_type = GUIIMG_DATE_TYPE_ARGB888;
        rotate_out_ptr->bit_ptr = rotate_out.output_chn.chn0.ptr;
    }

    return (rotate_result);
}

#endif

#endif //end of GUIF_ICONLIST

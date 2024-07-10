/*****************************************************************************
** File Name:      ctrlanim_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLANIM_EXPORT_H_
#define _CTRLANIM_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "guictrl.h"
#include "mmitheme_anim.h"
#include "guiimg.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*! @def GUIANIM_MINIATURE_FULL_PATH_LEN
@brief 缩略图全文件名长度
*/
#define GUIANIM_MINIATURE_FULL_PATH_LEN     255

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum GUIANIM_ALIGN_STYLE_E
@brief 动画显示排列方式
*/
typedef enum
{
    GUIANIM_ALIGN_NONE,             /*!<保留位*/
    GUIANIM_ALIGN_LEFT_TOP,         /*!<左上*/
    GUIANIM_ALIGN_LEFT_VMIDDLE,     /*!<左中*/
    GUIANIM_ALIGN_LEFT_BOTTOM,      /*!<左下*/
    GUIANIM_ALIGN_HMIDDLE_TOP,      /*!<中上*/
    GUIANIM_ALIGN_HVMIDDLE,         /*!<中*/
    GUIANIM_ALIGN_HMIDDLE_BOTTOM,   /*!<中下*/
    GUIANIM_ALIGN_RIGHT_TOP,        /*!<右上*/
    GUIANIM_ALIGN_RIGHT_VMIDDLE,    /*!<右中*/
    GUIANIM_ALIGN_RIGHT_BOTTOM,     /*!<右下*/
    GUIANIM_ALIGN_MAX               /*!<保留位*/
} GUIANIM_ALIGN_STYLE_E;

/*! @enum GUIANIM_RESULT_E
@brief 返回值
*/
typedef enum
{
    GUIANIM_RESULT_SUCC,                    /*!<成功*/
    GUIANIM_RESULT_FAIL,                    /*!<失败*/
    GUIANIM_RESULT_WAIT,                    /*!<等待*/
    GUIANIM_RESULT_BUSY,                    /*!<繁忙*/
    GUIANIM_RESULT_BIG,                     /*!<文件太大*/
    GUIANIM_RESULT_EMPTY,                   /*!<数据为空*/
    GUIANIM_RESULT_SFS_FAIL,                /*!<文件系统错误*/
    GUIANIM_RESULT_BAD_DATA,                /*!<坏的数据*/
    GUIANIM_RESULT_INTERRUPT,               /*!<解压缩task被销毁*/
    GUIANIM_RESULT_NO_SUPPORT,              /*!<不支持*/
    GUIANIM_RESULT_LACK_MEMORY,             /*!<缺少内存*/
    GUIANIM_RESULT_MINIATURE_FAIL,          /*!<jpg创建缩略图失败*/
    GUIANIM_RESULT_BIG_RESOLUTION,          /*!<分辨率太大*/
    GUIANIM_RESULT_GIF_MULTI_LACK_MEMORY,   /*!<gif动画,缺少内存*/
    GUIANIM_RESULT_WRONG_PARAM,             /*!<参数错误*/
    GUIANIM_RESULT_MAX                      /*!<保留位*/
} GUIANIM_RESULT_E;

/*! @enum GUIANIM_ROTATE_E
@brief 动画旋转模式
*/
typedef enum
{
    GUIANIM_ROTATE_NONE,    /*!<保留位*/
    GUIANIM_ROTATE_90,      /*!<90度*/
    GUIANIM_ROTATE_180,     /*!<180度*/
    GUIANIM_ROTATE_270,     /*!<270度*/
    GUIANIM_ROTATE_MIRROR,  /*!<镜像*/
    GUIANIM_ROTATE_MAX      /*!<保留位*/
} GUIANIM_ROTATE_E;

/*! @struct GUIANIM_INIT_DATA_T
@brief 动画控件初始化数据
*/
typedef struct 
{
    BOOLEAN             is_form_bg; /*!<是否是form控件背景*/
    BOOLEAN             is_spinner;
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖区域*/
} GUIANIM_INIT_DATA_T;

/*! @struct GUIANIM_DATA_INFO_T
@brief 动画数据信息
*/
typedef struct 
{
    BOOLEAN             is_bitmap;      /*!<是否是bitmap信息*/
    BOOLEAN             is_free_bitmap; /*!<是否由Anim释放bitmap信息*/
    BOOLEAN             is_save_data;   /*!<是否保存data信息*/
    uint8               *data_ptr;      /*!<data信息*/
    uint32              data_size;      /*!<data数据长度*/
    GUIIMG_BITMAP_T     bitmap_data;    /*!<bitmap数据信息*/
    MMI_ANIM_ID_T       anim_id;        /*!<动画id*/
    MMI_IMAGE_ID_T      img_id;         /*!<图片id*/
} GUIANIM_DATA_INFO_T;

/*! @struct GUIANIM_FILE_INFO_T
@brief 文件信息
*/
typedef struct 
{
    wchar       *full_path_wstr_ptr; /*!<全路径名*/
    uint16      full_path_wstr_len;  /*!<全路径名长度*/
} GUIANIM_FILE_INFO_T;

typedef enum
{
    GUIANIM_MASK_TYPE_PATH,
    GUIANIM_MASK_TYPE_ID,
    GUIANIM_MASK_TYPE_MAX
}GUIANIM_MASK_TYPE_E;

/*! @struct GUIANIM_DISPLAY_INFO_T
@brief 显示信息
*/
typedef struct 
{
    BOOLEAN                 is_syn_decode;          /*!<是否同步解码*/
    BOOLEAN                 is_update;              /*!<是否立即刷新*/
    BOOLEAN                 is_zoom;                /*!<是否用于缩放*/
    BOOLEAN                 is_auto_zoom_in;        /*!<是否自动放大小图到控件大小,不拉伸,仅支持文件图片*/
    BOOLEAN                 is_crop_rect;           /*!<是否裁减图片*/

    BOOLEAN                 is_play_once;           /*!<如果是动画,是否只播放一遍*/
    BOOLEAN                 is_disp_one_frame;      /*!<是否只显示一帧图片,比如九宫格预览gif,选项卡主菜单,只显示单张图片,不显示动画*/
    BOOLEAN                 is_need_end_ind;        /*!<是否需要动画或者Gif动画播放一次完毕的消息*/
    BOOLEAN                 is_save_wallpaper;      /*!<是否保存墙纸bitmap数据,只针对墙纸*/
    
    BOOLEAN                 is_disp_frame;          /*!<是否显示图片框,只针对文件图片*/
    BOOLEAN                 is_handle_transparent;  /*!<是否处理透明,只针对gif,png,是则输出ARGB888,保存图片数据的buffer增加*/
    BOOLEAN                 is_no_miniatur;         /*!<是否不支持缩略图*/
    BOOLEAN                 is_free_buf_lose_focus; /*!<anim lose focus时,是否释放target buffer,默认否,是则get focus都需要重新解码，会慢*/
    
    BOOLEAN                 is_bg_buf;              /*!<是否复制背景数据,只针对Gif动画*/
    GUI_BG_T                bg;                     /*!<控件背景*/

    uint16                  specify_width;          /*!<指定解码后图片的最大宽度*/
    uint16                  specify_height;         /*!<指定解码后图片的最大高度*/
    uint16                  rotate_angle;           /*!<旋转角度0-360,软件旋转,速度慢*/
    GUIANIM_ROTATE_E        rotate_mode;            /*!<旋转模式,硬件旋转,速度快*/
    GUIANIM_ALIGN_STYLE_E   align_style;            /*!<动画显示排列方式*/
    BOOLEAN                 is_res_bg_color;        /*!<是否要替换资源的背景色*/
    uint32                  res_bg_color;           /*!<资源的背景色值*/
    BOOLEAN                 is_img_res_need_buffer; /*!<资源img是否需要buffer解码结果，只对img id有效*/


    BOOLEAN                         is_handle_mask;    /*是否使用蒙版*/
    GUIANIM_MASK_TYPE_E             mask_type;         /*蒙版类型*/
    wchar*                          p_mask_path;       /*蒙版的绝对路径（如果mask_type=GUIANIM_MASK_TYPE_PATH,此值不能为0）*/
    uint16                          mask_path_len;     /*绝对路径的长度（如果mask_type=GUIANIM_MASK_TYPE_PATH,此值不能为0）*/
    MMI_IMAGE_ID_T                  mask_id;           /*蒙版的资源id（如果mask_type=GUIANIM_MASK_TYPE_ID,此值不能为0）*/
} GUIANIM_DISPLAY_INFO_T;

/*! @struct GUIANIM_MINIATURE_FULL_PATH_T
@brief 缩略图文件名
*/
typedef struct 
{
    wchar       full_path_wstr[GUIANIM_MINIATURE_FULL_PATH_LEN + 1];/*!<全路径名*/
    uint16      full_path_wstr_len;                                 /*!<全路径名长度*/
} GUIANIM_MINIATURE_FULL_PATH_T;

/*! @struct GUIANIM_OWNER_DRAW_T
@brief 自绘信息
*/
typedef struct 
{
    uint32              param;          /*!<用户数据*/
    int16               display_x;      /*!<图片显示x坐标*/
    int16               display_y;      /*!<图片显示y坐标*/
    GUI_RECT_T          display_rect;   /*!<控件显示区域*/
    MMI_HANDLE_T        win_handle;     /*!<窗口handle*/
    MMI_HANDLE_T        ctrl_handle;    /*!<控件handle*/
    GUIIMG_BITMAP_T     bitmap_data;    /*!<bitmap数据*/
    GUI_LCD_DEV_INFO    lcd_dev;        /*!<lcd信息*/
} GUIANIM_OWNER_DRAW_T;

/*! @typedef GUIANIM_OWNER_DRAW_FUNC
@brief 动画自绘回调函数
*/
typedef void (*GUIANIM_OWNER_DRAW_FUNC)(
                                        GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                                        );

/*! @struct GUIANIM_CALL_BACK_T
@brief 动画自绘回调信息
*/
typedef struct
{
    uint32                  param;          /*!<携带参数*/
   GUIANIM_OWNER_DRAW_FUNC  OwnerDrawFunc;  /*!<自绘回调函数*/
} GUIANIM_CALL_BACK_T;

/*! @struct GUIANIM_CTRL_INFO_T
@brief 控件信息
*/
typedef struct 
{
    BOOLEAN             is_wallpaper_ctrl;  /*!<是否是墙纸控件*/
    BOOLEAN             is_ctrl_id;         /*!<是否传入控件id*/
    MMI_CTRL_ID_T       ctrl_id;            /*!<控件id*/
    void                *ctrl_ptr;          /*!<控件指针,可以为空*/
} GUIANIM_CTRL_INFO_T;

/*! @struct GUIANIM_DISPLAY_IND_T
@brief 动画显示消息参数
*/
typedef struct 
{
    MMI_HANDLE_T        ctrl_handle;/*!<控件handle*/
    GUIANIM_RESULT_E    result;     /*!<解码结果*/
} GUIANIM_DISPLAY_IND_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 设置动画参数
@author Jassmine
@param ctrl_info_ptr    [in] 控件信息
@param data_info_ptr    [in] 数据信息,可以为空
@param file_info_ptr    [in] 文件信息,可以为空
@param display_info_ptr [in] 显示信息
@return 操作结果
@note 
*******************************************************************************/
PUBLIC GUIANIM_RESULT_E CTRLANIM_SetParam(
                                         GUIANIM_CTRL_INFO_T    *ctrl_info_ptr,     //in:
                                         GUIANIM_DATA_INFO_T    *data_info_ptr,     //in:may PNULL
                                         GUIANIM_FILE_INFO_T    *file_info_ptr,     //in:may PNULL
                                         GUIANIM_DISPLAY_INFO_T *display_info_ptr   //in:
                                         );

/***************************************************************************//*!
@brief 根据排列方式,调整图片显示位置
@author Jassmine
@param display_x_ptr [in/out] 图片显示x坐标
@param display_y_ptr [in/out] 图片显示x坐标
@param pic_width     [in] 图片宽
@param pic_height    [in] 图片高
@param rect          [in] 显示区域
@param align_style   [in] 排列方式
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_AdjustDisplayPosition(
                                          int16                 *display_x_ptr, //in/out:
                                          int16                 *display_y_ptr, //in/out:
                                          uint16                pic_width,      //in:pic width
                                          uint16                pic_height,     //in:pic height
                                          GUI_RECT_T            rect,           //in
                                          GUIANIM_ALIGN_STYLE_E align_style     //in
                                          );

/***************************************************************************//*!
@brief 根据文件名获得图片的宽或者高
@author Jassmine
@param width_ptr     [in] 图片宽,可以为空
@param height_ptr    [in] 图片高,可以为空
@param file_path_ptr [in] 全文件名
@param file_len      [in] 全文件名长度
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeight(
                                         uint16      *width_ptr,     //in/out:may PNULL
                                         uint16      *height_ptr,    //in/out:may PNULL
                                         wchar       *file_path_ptr, //in:
                                         uint16      file_len        //in:
                                         );

/***************************************************************************//*!
@brief 根据控件handle获得图片的宽或者高
@author Jassmine
@param ctrl_handle [in] 控件handle
@param width_ptr   [in/out] 图片宽,可以为空
@param height_ptr  [in/out] 图片高,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeightEx(
                                           MMI_HANDLE_T     ctrl_handle,//in:
                                           uint16           *width_ptr, //in/out:may PNULL
                                           uint16           *height_ptr //in/out:may PNULL
                                           );

/***************************************************************************//*!
@brief 获得图片显示的起始点坐标
@author Jassmine
@param ctrl_handle   [in] 控件handle
@param display_x_ptr [in/out] 起始点x坐标
@param display_y_ptr [in/out] 起始点y坐标
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayPoint(
                                       MMI_HANDLE_T ctrl_handle,    //in:
                                       int16        *display_x_ptr, //in/out:
                                       int16        *display_y_ptr  //in/out:
                                       );

/***************************************************************************//*!
@brief 获得图片缩放的宽或高的最小,最大值
@author Jassmine
@param ctrl_handle    [in] 控件handle
@param width_min_ptr  [in/out] 图片宽最小值,可以为空
@param width_max_ptr  [in/out] 图片高最大值,可以为空
@param height_min_ptr [in/out] 图片高最小值,可以为空
@param height_max_ptr [in/out] 图片高最大值,可以为空
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetZoomWidthRange(
                                         MMI_HANDLE_T   ctrl_handle,    //in:
                                         uint16         *width_min_ptr, //in/out:may PNULL
                                         uint16         *width_max_ptr, //in/out:may PNULL
                                         uint16         *height_min_ptr,//in/out:may PNULL
                                         uint16         *height_max_ptr //in/out:may PNULL
                                         );

/***************************************************************************//*!
@brief 获得图片缩放的剪切区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in/out] 图片缩放的剪切区域
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetZoomClipRect(
                                       MMI_HANDLE_T     ctrl_handle,//in:
                                       GUI_RECT_T       *rect_ptr   //in/out
                                       );

/***************************************************************************//*!
@brief 缩放图片
@author Jassmine
@param ctrl_handle [in] 控件handle
@param is_zoom_in  [in] 是否放大
@param is_update   [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_Zoom(
                            MMI_HANDLE_T    ctrl_handle,
                            BOOLEAN         is_zoom_in,
                            BOOLEAN         is_update
                            );

/***************************************************************************//*!
@brief 移动缩放图片
@author Jassmine
@param ctrl_handle [in] 控件handle
@param move_x      [in] x<0 向左移动, x>0 向左移动
@param move_y      [in] y<0 向上移动, y>0 向下移动
@param is_update   [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomMove(
                                MMI_HANDLE_T    ctrl_handle,    //in:
                                int16           move_x,         //in:x<0 move left, x>0 move right
                                int16           move_y,         //in:y<0 move up, y>0 move bottom
                                BOOLEAN         is_update       //in:
                                );
/***************************************************************************//*!
@brief 缩放图片到极限
@author kevin.lou
@param ctrl_handle [in] 控件handle
@return 操作是否成功
@note 没放到最大时会放到最大，否则缩至最小
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomUtmost(
                                MMI_HANDLE_T    ctrl_handle    //in:
                                );

/***************************************************************************//*!
@brief 设置图片缩放比率
@author Jassmine
@param ctrl_handle          [in] 控件handle
@param zoom_in_ratio_ptr    [in] 放大比例,必须大于1
@param zoom_out_ratio_ptr   [in] 缩小比例,必须小于1
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetZoomRatio(
                                    MMI_HANDLE_T        ctrl_handle,        //in:
                                    MMITHEME_RATIO_T    *zoom_in_ratio_ptr, //in:may PNULL
                                    MMITHEME_RATIO_T    *zoom_out_ratio_ptr //in:may PNULL
                                    );

/***************************************************************************//*!
@brief 旋转图片90度,180度,270度或者镜像
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rotate_mode [in] 旋转模式
@param is_update   [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_Rotate(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIANIM_ROTATE_E  rotate_mode,
                              BOOLEAN           is_update
                              );

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLANIM_RotateArbitrary(
                                       MMI_HANDLE_T     ctrl_handle,
                                       uint16           angle,
                                       BOOLEAN          is_update
                                       );
                                       
/*****************************************************************************/
//  Description : get anim type
//  Global resource dependence : 
//  Author: chunyou.zhu
//  Note:
/*****************************************************************************/
PUBLIC GUIANIM_TYPE_E CTRLANIM_GetAnimType(
                                MMI_HANDLE_T    ctrl_handle
                                );
                                
/***************************************************************************//*!
@brief 播放动画
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnim(
                                MMI_HANDLE_T    ctrl_handle
                                );

/***************************************************************************//*!
@brief 通过控件指针播放动画
@author Jassmine
@param ctrl_ptr [in] 控件指针
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnimByPtr(
                                     void   *ctrl_ptr
                                     );

/***************************************************************************//*!
@brief 暂停动画
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_PauseAnim(
                              MMI_HANDLE_T ctrl_handle
                              );

/***************************************************************************//*!
@brief 继续动画
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_ResumeAnim(
                               MMI_HANDLE_T ctrl_handle
                               );

/***************************************************************************//*!
@brief 设置动画控件区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in] 区域
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRect(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_RECT_T      *rect_ptr
                                );

/***************************************************************************//*!
@brief 通过控件指针设置动画区域
@author Jassmine
@param ctrl_ptr     [in] 控件指针
@param rect_ptr     [in] 区域
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRectByPtr(
                                     void           *ctrl_ptr,
                                     GUI_RECT_T     *rect_ptr
                                     );

/***************************************************************************//*!
@brief 获得动画控件显示区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in/out] 区域
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayRect(
                                      MMI_HANDLE_T  ctrl_handle,    //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      );

/***************************************************************************//*!
@brief 设置动画控件显示区域
@author Jassmine
@param ctrl_handle [in] 控件handle
@param rect_ptr    [in] 区域
@param is_update   [in] 是否立即刷新
@note 可以显示半幅图片
*******************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRect(
                                   MMI_HANDLE_T     ctrl_handle,
                                   GUI_RECT_T       *rect_ptr,
                                   BOOLEAN          is_update
                                   );

/***************************************************************************//*!
@brief 通过控件指针设置动画控件显示区域
@author Jassmine
@param ctrl_ptr      [in] 控件指针
@param rect_ptr      [in] 区域
@param is_update     [in] 是否立即刷新
@note 可以显示半幅图片
*******************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRectByPtr(
                                        void            *ctrl_ptr,
                                        GUI_RECT_T      *rect_ptr,
                                        BOOLEAN         is_update
                                        );

/***************************************************************************//*!
@brief 设置动画控件背景图片
@author Jassmine
@param ctrl_handle [in] 控件handle
@param bg_img_id   [in] 图片id
@return 是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImg(
                                MMI_HANDLE_T    ctrl_handle,
                                MMI_IMAGE_ID_T  bg_img_id
                                );

/***************************************************************************//*!
@brief 通过控件指针设置动画控件背景图片
@author Jassmine
@param ctrl_ptr      [in] 控件指针
@param bg_img_id     [in] 图片id
@return 是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImgByPtr(
                                     void               *ctrl_ptr,
                                     MMI_IMAGE_ID_T     bg_img_id
                                     );

/***************************************************************************//*!
@brief 设置动画控件背景颜色
@author Jassmine
@param ctrl_handle [in] 控件handle
@param bg_color    [in] 颜色
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBgColor(
                               MMI_HANDLE_T     ctrl_handle,
                               GUI_COLOR_T      bg_color
                               );

/***************************************************************************//*!
@brief 通过控件指针设置动画控件背景颜色
@author Jassmine
@param ctrl_ptr      [in] 控件指针
@param bg_color      [in] 颜色
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBgColorByPtr(
                                    void            *ctrl_ptr,
                                    GUI_COLOR_T     bg_color
                                    );

/***************************************************************************//*!
@brief 设置动画控件边框
@author Jassmine
@param ctrl_handle [in] 控件handle
@param border_ptr  [in] 边框
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBorder(
                              MMI_HANDLE_T  ctrl_handle,
                              GUI_BORDER_T  *border_ptr
                              );

/***************************************************************************//*!
@brief 设置动画相框
@author Jassmine
@param ctrl_handle  [in] 控件handle
@param frame_ptr    [in] 相框
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetFrame(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_BORDER_T   *frame_ptr
                             );

/***************************************************************************//*!
@brief 设置动画控件默认图片
@author Jassmine
@param ctrl_handle [in] 控件handle
@param def_img_id  [in] 默认图片
@param err_img_id  [in] 默认错误图片
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIcon(
                                      MMI_HANDLE_T      ctrl_handle,
                                      MMI_IMAGE_ID_T    def_img_id, //default image id
                                      MMI_IMAGE_ID_T    err_img_id  //error image id
                                      );

/***************************************************************************//*!
@brief 通过控件指针设置动画控件默认图片
@author Jassmine
@param ctrl_ptr      [in] 控件指针
@param def_img_id    [in] 默认图片
@param err_img_id    [in] 默认错误图片
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIconByPtr(
                                           void             *ctrl_ptr,
                                           MMI_IMAGE_ID_T   def_img_id,
                                           MMI_IMAGE_ID_T   err_img_id
                                           );

/***************************************************************************//*!
@brief 设置动画控件图片解码错误背景
@author Jassmine
@param ctrl_handle  [in] 控件handle
@param error_bg_ptr [in] 背景
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetErrorBg(
                                  MMI_HANDLE_T  ctrl_handle,
                                  GUI_BG_T      *error_bg_ptr
                                  );

/***************************************************************************//*!
@brief 设置动画控件是否自适应横竖屏显示
@author Jassmine
@param is_adapt    [in] 是否自适应
@param ctrl_handle [in] 控件handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetAutoAdaptDisplay(
                                        BOOLEAN         is_adapt,   //in
                                        MMI_HANDLE_T    ctrl_handle //in
                                        );

/***************************************************************************//*!
@brief 设置动画最小时间间隔
@author Jassmine
@param min_period  [in] 最小时间间隔
@param ctrl_handle [in] 控件handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetPeriod(
                              uint32        min_period, //in
                              MMI_HANDLE_T  ctrl_handle //in
                              );

/***************************************************************************//*!
@brief 刷新动画
@author Jassmine
@param ctrl_handle [in] 控件handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_UpdateDisplay(
                                  MMI_HANDLE_T  ctrl_handle
                                  );

/***************************************************************************//*!
@brief 刷新动画制定区域
@author Jassmine
@param rect        [in] 区域
@param ctrl_handle [in] 控件handle
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRect(
                                  GUI_RECT_T        rect,
                                  MMI_HANDLE_T      ctrl_handle
                                  );

/***************************************************************************//*!
@brief 通过控件指针刷新动画制定区域
@author Jassmine
@param rect          [in] 区域
@param ctrl_ptr      [in] 控件指针
@return 是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRectByPtr(
                                       GUI_RECT_T   rect,
                                       void         *ctrl_ptr
                                       );

/***************************************************************************//*!
@brief 删除缩略图文件
@author Jassmine
@param full_path_ptr [in] 全文件名
@param full_path_len [in] 全文件名长度
@return 是否操作成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_DeleteMiniatureFile(
                                           const wchar  *full_path_ptr,     //in
                                           uint16       full_path_len       //in
                                           );

/***************************************************************************//*!
@brief 获得动画是否支持,即解码能否成功
@author Jassmine
@param ctrl_handle [in] 控件handle
@return 是否支持
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_IsSupport(
                                 MMI_HANDLE_T   ctrl_handle
                                 );

/***************************************************************************//*!
@brief 设置动画控件是否可见
@author Jassmine
@param ctrl_handle [in] 控件handle
@param is_visible  [in] 是否可见
@param is_update   [in] 是否立即刷新
@return 操作是否成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetVisible(
                                  MMI_HANDLE_T  ctrl_handle,
                                  BOOLEAN       is_visible,
                                  BOOLEAN       is_update
                                  );

/***************************************************************************//*!
@brief 设置动画控件自绘函数
@author Jassmine
@param ctrl_handle [in] 控件handle
@param func        [in] 自绘回调函数
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDraw(
                                 MMI_HANDLE_T               ctrl_handle,
                                 GUIANIM_OWNER_DRAW_FUNC    func
                                 );

/***************************************************************************//*!
@brief 设置动画控件自绘函数
@author Jassmine
@param ctrl_handle [in] 控件handle
@param call_back_ptr  [in] 自绘回调函数,可以携带参数
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDrawEx(
                                   MMI_HANDLE_T         ctrl_handle,
                                   GUIANIM_CALL_BACK_T  *call_back_ptr
                                   );

/***************************************************************************//*!
@brief 根据宽和高,获得图片旋转后的宽和高
@author Jassmine
@param width_ptr    [in/out] 图片宽
@param height_ptr   [in/out] 图片高
@param rotate_angle [in] 旋转角度(0-360)
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetRotateSize(
                                     uint16     *width_ptr,     //in/out:
                                     uint16     *height_ptr,    //in/out:
                                     uint16     rotate_angle    //in:
                                     );

/*****************************************************************************/
//  Description : get anim display bitmap
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLANIM_GetAnimDisplayBitmap(
                                          MMI_HANDLE_T  ctrl_handle,
                                          GUIIMG_BITMAP_T *bitmap_ptr
                                          );

/*****************************************************************************/
//  Description : anim is tp enable
//  Global resource dependence :
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC void CTRLANIM_IsTpEnable(
                                 MMI_HANDLE_T    ctrl_handle,
                                 BOOLEAN     is_tp_enable);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

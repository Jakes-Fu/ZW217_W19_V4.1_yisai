/*****************************************************************************
** File Name:      ctrlanim.h                                               *
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

#ifndef _CTRLANIM_H_
#define _CTRLANIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlanim_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// anim type info
#define CTRL_ANIM_TYPE     (ANIM_GetType())

#define GUIANIM_ALIGN(x,align)      (x/align * align)
#define GUIANIM_ALIGN_EX(x,align)   ((x/align+1) * align)

//缩略增加的后缀_320x240长度
#define GUIANIM_MINIATURE_SUFFIX_LEN        10
        
#define GUIANIM_BUF_ALIGN                   256

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUIANIM_SII_RESULT_E
@brief 返回值
*/
typedef enum
{
    GUIANIM_SII_SUCC,                   /*!<成功*/
    GUIANIM_SII_FAIL,                   /*!<失败*/
    GUIANIM_SII_JPG_ENCODE_SUCC,        /*!<jpg压缩成功*/
    GUIANIM_SII_JPG_ENCODE_FAIL,        /*!<jpg压缩失败*/
    GUIANIM_SII_DECODE_THUMB_FAIL,      /*!<解码jpg thumbnail失败*/
    GUIANIM_SII_DECODE_MINIATURE_FAIL,  /*!<解码jpg 缩略图失败*/
    GUIANIM_SII_MAX                     /*!<保留位*/
} GUIANIM_SII_RESULT_E;

/*! @struct GUIANIM_ZOOM_T
@brief 缩放信息
*/
typedef struct 
{
    uint8               *buf_ptr;       /*!<缩放目标buffer*/
    uint32              buf_size;       /*!<缩放目标buffer大小*/
    uint32              buf_offset;     /*!<缩放目标buffer偏移,要求256字节对齐*/
    uint16              buf_width;      /*!<目标buffer宽,即可见图片的宽*/
    uint16              buf_height;     /*!<目标buffer高,即可见图片的高*/
    uint16              cur_img_width;  /*!<缩放后当前图片宽,包括不可见部分*/
    uint16              cur_img_height; /*!<缩放后当前图片高,包括不可见部分*/
    GUI_RECT_T          cur_clip_rect;  /*!<相对于当前图片的剪切区域*/
    GUIIMG_DATE_TYPE_E  data_type;      /*!<缩放后数据类型*/
} GUIANIM_ZOOM_T;

//rotate info
typedef struct 
{
    uint8               *buf_ptr;       /*!<旋转目标buffer*/
    uint32              buf_size;       /*!<旋转目标buffer大小*/
    uint32              buf_offset;     /*!<旋转目标buffer偏移,要求256字节对齐*/
    uint16              img_width;      /*!<旋转后当前图片宽*/
    uint16              img_height;     /*!<旋转后当前图片高*/
    GUIIMG_DATE_TYPE_E  data_type;      /*!<旋转后数据类型*/
} GUIANIM_ROTATE_T;

// anim init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    BOOLEAN                     is_form_bg; /*!<是否是form控件背景*/
} CTRLANIM_INIT_PARAM_T;

//anim control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                         is_focus;           /*!<窗口是否处于焦点状态*/
    BOOLEAN                         is_pause;           /*!<动画是否暂时*/
    BOOLEAN                         is_visible;         /*!<动画是否可见*/
    BOOLEAN                         is_form_bg;         /*!<动画是否是form控件背景*/

    BOOLEAN                         is_pressed;         /*!<anim is pressed*/
    uint8                           timer_id;           /*!<动画定时器*/
    uint8                           gif_timer_id;       /*!<gif定时器*/

    BOOLEAN                         is_wallpaper_ctrl;  /*!<是否是墙纸控件*/
    BOOLEAN                         is_save_wallpaper;  /*!<是否保存墙纸bitmap数据*/
    BOOLEAN                         is_gif_wallpaper_saved; /*<gif墙纸保存第一帧后不再保存*/    
    BOOLEAN                         is_auto_adapt_disp; /*!<是否自适应显示,当图片宽大于长,横屏显示,否则竖屏显示*/
    BOOLEAN                         is_decoding;        /*!<是否正在解码*/

    uint8                           *bg_buf_ptr;        /*!<是否备份背景*/
    BOOLEAN                         is_decode_thumb;    /*!<是否在解码thumbnail*/
    BOOLEAN		                    is_encode_jpg;		/*!<是否在压缩jpg*/
    BOOLEAN                         is_exist_miniature; /*!<是否存在缩略图*/
    BOOLEAN                         is_rotate;          /*!<是否旋转*/

    BOOLEAN                         is_decoded;         /*!<是否已经解码完成*/
    BOOLEAN                         is_support;         /*!<图片格式是否支持*/
    BOOLEAN                         is_support_zoom;    /*!<图片格式是否支持缩放*/
    BOOLEAN                         is_adjust_disp_pos; /*!<显示位置是否已经调整,防止gif多次调整*/
    BOOLEAN                         is_send_disp_ind;   /*!<是否已经发过MSG_CTL_ANIM_DISPLAY_IND消息,防止gif多次发送*/
    BOOLEAN                         is_notify_update;   /*!<当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE*/
    
    BOOLEAN                         is_next_frame;      /*!<是否解码gif动画资源下一桢*/
    BOOLEAN                         is_dec_all_gif;     /*!<gif动画资源是否解码完成*/

    BOOLEAN                         is_target_alloc;    /*!<是否申请目标buffer*/
    uint8                           *target_buffer;     /*!<目标buffer*/
    uint8                           *target_backup_buf; /*!<目标备份buffer,只针对gif*/
    uint32                          target_buffer_size; /*!<目标buffer大小*/
    uint32                          target_buf_offset;  /*!<目标buffer偏移,要求256字节对齐*/
    uint32                          target_backup_offset;/*!<目标备份buffer偏移,要求256字节对齐*/
    GUIIMG_DATE_TYPE_E              target_data_type;   //target data type

    uint8                           *src_data_buffer;   /*!<源数据*/
    uint32                          src_data_size;      /*!<源数据大小*/

    uint16                          zoom_min_width;     /*!<缩放最小宽度*/
    uint16                          zoom_min_height;    /*!<缩放最小高度*/
    GUIANIM_ZOOM_T                  zoom_dest;          /*!<缩放目标信息*/

    GUIANIM_ROTATE_T                rotate_dest;        /*!<旋转目标信息*/

    uint32                          res_gif_handle;     /*!<gif资源handle*/
    uint32                          img_handle;         /*!<图片handle*/
    
    uint16                          src_width;          /*!<原图宽*/
    uint16                          src_height;         /*!<原图高*/
    uint16                          img_width;          /*!<图片宽*/
    uint16                          img_height;         /*!<图片高*/
    uint16                          thumb_width;        /*!<thumbnail宽*/
    uint16                          thumb_height;       /*!<thumbnail高*/
    uint16                          cur_frame;          /*!<当前桢索引*/
    uint16                          total_frame;        /*!<总桢数*/
    uint16                          ico_frame_index;    //ico frame index 

    uint32                          delay_time;         /*!<每桢动画时间间隔*/
    
    int16                           display_x;          /*!<图片显示x坐标,图片根据Align计算的显示位置*/
    int16                           display_y;          /*!<图片显示y坐标,图片根据Align计算的显示位置*/
    GUIANIM_TYPE_E                  type;               /*!<图片类型*/

    GUIANIM_DATA_INFO_T             data_info;          /*!<图片数据信息,包括image id,data或者bitmap等*/
    GUIANIM_FILE_INFO_T             file_info;          /*!<图片文件信息*/

    GUIANIM_DISPLAY_INFO_T          display_info;       /*!<图片xianshi信息*/

    uint16                          miniature_width;    /*!<缩略图宽*/
    uint16                          miniature_height;   /*!<缩略图高*/
    GUIANIM_MINIATURE_FULL_PATH_T   miniature_path;     /*!<缩略图文件名*/

    GUIANIM_CALL_BACK_T             CallBackFunc;       /*!<回调函数*/

    MMITHEME_ANIM_T                 theme;              /*!<动画风格*/

    MMI_HANDLE_T                    win_handle;         /*!<窗口handle*/
} CTRLANIM_OBJ_T;

// anim class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLANIM_VTBL_T;

//image data buffer info
typedef struct 
{
    uint8       *src_buf_ptr;   //src data buffer
    uint32      src_data_size;  //src data size
} GUIANIM_IMG_DATA_T;

//jpg thumbnail info
typedef struct
{
    BOOLEAN     is_existed;     //is exist thumbnail
    uint16      thumb_width;    //thumbnail width
    uint16      thumb_height;   //thumbnail height
    uint32      thumb_offset;   //thumbnail offset
    uint32      thumb_size;     //thumbnail size
} GUIANIM_JPG_THUMBNAIL_T;

//image info
typedef struct
{
    BOOLEAN                 is_support;     //is support,only for jpg
    uint16                  img_width;      //image width
    uint16                  img_height;     //image height
    uint16                  ico_frame_index;//ico frame index
    GUIANIM_TYPE_E          type;           //image type
    GUIANIM_JPG_THUMBNAIL_T jpg_thumbnail;  //jpg thumbnail info
} GUIANIM_IMG_INFO_T;

//decode,encode target info
typedef struct 
{
    BOOLEAN             is_decode_thumb;    //is decode thumbnail,only for jpg
    BOOLEAN			    is_exist_bg;		//is exist bg,only for gif
    uint8               *target_buf_ptr;    //target buffer
    uint32              target_buf_size;    //target buffer size
    uint16              target_width;       //target width
    uint16              target_height;      //target height
    uint8               *alpha_buf_ptr;     //alpha data buffer
    uint32              alpha_buf_size;     //alpha data size
    GUI_RECT_T          crop_rect;          //crop rect
    GUIIMG_DATE_TYPE_E  target_type;        //target type
    uint16              ico_frame_index;    //ico frame index
} GUIANIM_TARGET_T;

//decode,encode result info
typedef struct 
{
    BOOLEAN             is_decode_gif_end;  //for gif,GIF是否解码完成
    BOOLEAN             is_handle_alpha;    //is handle alpha for png
    uint16              cur_frame_index;    //for gif
    uint32              delay_time;         //for gif
    uint16              img_width;          //image width
    uint16              img_height;         //image height
    uint32		        img_size;		    //for encode jpg
    uint32              img_handle;         //image handle
    GUIANIM_RESULT_E    result;             //result
    GUIIMG_DATE_TYPE_E  data_type;          //data type
} GUIANIM_DEC_RESULT_T;

//decode,encode carry param
typedef struct 
{
    uint32              img_handle;     //image handle
    MMI_HANDLE_T        ctrl_handle;    //ctrl handle
    GUIIMG_DATE_TYPE_E  data_type;      //data type
} GUIANIM_DEC_CALLBACK_T;

//delete msg param
typedef struct 
{
    uint32              img_handle; //image handle
    MMI_HANDLE_T        handle;     //msg handle
    MMI_MESSAGE_ID_E    msg_id;     //msg id
} GUIANIM_DEL_MSG_T;

//zoom in param
typedef struct 
{
    uint8               *src_buf_ptr;       //src buffer
    uint32              src_buf_size;       //src buffer size
    uint8               *dest_buf_ptr;      //dest buffer
    uint32              dest_buf_size;      //dest buffer size

    uint16              src_img_width;      //src image width
    uint16              src_img_height;     //src image height
    uint16              dest_img_width;     //dest image width
    uint16              dest_img_height;    //dest image height
    GUI_RECT_T          clip_rect;          //clip rect

    GUIIMG_DATE_TYPE_E  src_data_type;      //src data type
} GUIANIM_ZOOM_INPUT_T;

//scale out param
typedef struct 
{
    uint16              img_width;      //image width
    uint16              img_height;     //image height
    uint32              dest_buf_offset;//dest buffer必须256对齐
    GUIIMG_DATE_TYPE_E  data_type;      //data type
} GUIANIM_ZOOM_OUTPUT_T;

//rotate in param
typedef struct 
{
    uint8                   *src_buf_ptr;       //src buffer
    uint32                  src_buf_size;       //src buffer size
    uint8                   *dest_buf_ptr;      //dest buffer
    uint32                  dest_buf_size;      //dest buffer size

    uint16                  src_img_width;      //src image width
    uint16                  src_img_height;     //src image height
    GUIIMG_DATE_TYPE_E      src_data_type;      //src data type
    IMGREF_ROTATION_MODE_E  rotate_mode;        //rotate mode
} GUIANIM_ROTATE_INPUT_T;

//rotate out param
typedef struct 
{
    uint16              img_width;      //image width
    uint16              img_height;     //image height
    uint32              dest_buf_offset;//dest buffer必须256对齐
    GUIIMG_DATE_TYPE_E  data_type;      //data type
} GUIANIM_ROTATE_OUTPUT_T;

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

    GUIIMG_DATE_TYPE_E  src_data_type;      //src data type
} GUIANIM_ROTATE_ANGLE_INPUT_T;

//rotate arbitrary angle out param
typedef struct 
{
    uint16              img_width;      //image width
    uint16              img_height;     //image height
    GUIIMG_DATE_TYPE_E  data_type;      //data type
} GUIANIM_ROTATE_ANGLE_OUTPUT_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get anim type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ANIM_GetType(void);

/*****************************************************************************/
//  Description : get anim pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLANIM_OBJ_T* GUIANIM_GetPtr(
                                      MMI_HANDLE_T  ctrl_handle
                                      );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_TypeOf(
                              CTRLBASE_OBJ_T     *ctrl_ptr
                              );

/*****************************************************************************/
//  Description : get anim rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_GetAnimRect(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr, //in:
                                   GUI_RECT_T       *rect_ptr       //in/out:
                                   );

/*****************************************************************************/
//  Description : get anim is display horizontal
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_IsHorizontal(
                                    CTRLANIM_OBJ_T      *anim_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : get image width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetImgWidthAndHeight(
                                         BOOLEAN        is_miniature,   //in:
                                         BOOLEAN        is_src,         //in:
                                         uint16         *img_width_ptr, //in/out:may PNULL
                                         uint16         *img_height_ptr,//in/out:may PNULL
                                         CTRLANIM_OBJ_T *anim_ctrl_ptr  //in:
                                         );

/*****************************************************************************/
//  Description : get decode target width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetTargetWidthAndHeight(
                                            BOOLEAN         is_for_zoom,        //in:is for zoom
                                            uint16          *target_width_ptr,  //in/out:
                                            uint16          *target_height_ptr, //in/out:
                                            CTRLANIM_OBJ_T  *anim_ctrl_ptr      //in
                                            );

/*****************************************************************************/
//  Description : get image show width and height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_GetShowWidthAndHeight(
                                          uint16          *show_width_ptr,  //in/out:
                                          uint16          *show_height_ptr, //in/out:
                                          CTRLANIM_OBJ_T  *anim_ctrl_ptr    //in
                                          );

/*****************************************************************************/
//  Description : post gif or anim play once end indication msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_PostPlayOnceEndIndMsg(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          );

/*****************************************************************************/
//  Description : display anim or image
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_Display(
                               BOOLEAN          is_full_paint,  //是否是full paint消息
                               BOOLEAN          is_only_update, //in:
                               CTRLANIM_OBJ_T   *anim_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : get display bitmap
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_GetDisplayBitmap(
                                     CTRLANIM_OBJ_T     *anim_ctrl_ptr, //in:
                                     GUIIMG_BITMAP_T    *bitmap_ptr     //in/out:
                                     );

/*****************************************************************************/
//  Description : start gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_StartGifTimer(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : stop gif timer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_StopGifTimer(
                                 uint8  *time_id_ptr
                                 );

/*****************************************************************************/
//  Description : start anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_StartAnimTimer(
                                   CTRLANIM_OBJ_T   *anim_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : stop anim timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_StopAnimTimer(
                                  uint8     *time_id_ptr
                                  );

/*****************************************************************************/
//  Description : display anim frame
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIANIM_DisplayFrame(
                                 CTRLANIM_OBJ_T     *anim_ctrl_ptr
                                 );

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
                                            );

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
                                         );

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
                                              );

/*****************************************************************************/
//  Description : get image info
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
//      kevin.lou modified for ico img file 2012/2/10;
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_GetInfo(
                                        GUI_RECT_T          *rect_ptr,      //in:ico文件需要匹配rect;
                                        wchar               *full_path_ptr, //in:文件名,may PNULL
                                        GUIANIM_IMG_DATA_T  *src_data_ptr,  //in:传入的图片数据,may PNULL
                                        GUIANIM_IMG_INFO_T  *img_info_ptr   //in/out:
                                        );

/*****************************************************************************/
//  Description : auto zoom image to display in anim rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_AutoZoom(
                                CTRLANIM_OBJ_T  *anim_ctrl_ptr  //in
                                );

/*****************************************************************************/
//  Description : zoom in or out image
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_ZoomImage(
                                          GUIANIM_ZOOM_INPUT_T  *zoom_in_ptr,   //in:
                                          GUIANIM_ZOOM_OUTPUT_T *zoom_out_ptr   //in/out:
                                          );

/*****************************************************************************/
//  Description : get zoom range
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_ZoomRange(
                                 uint16     *min_ptr,   //in/out:zoom out,1/min
                                 uint16     *max_ptr    //in/out:zoom in,max
                                 );

/*****************************************************************************/
//  Description : is rotate image to horizontal display
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_IsRotateHorizontal(
                                          CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                          );

/*****************************************************************************/
//  Description : auto rotate image to display in anim rect
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_AutoRotate(
                                  CTRLANIM_OBJ_T    *anim_ctrl_ptr  //in
                                  );

/*****************************************************************************/
//  Description : rotate image angle 90,180,270 or mirror
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_RotateImage(
                                            GUIANIM_ROTATE_INPUT_T  *rotate_in_ptr, //in:
                                            GUIANIM_ROTATE_OUTPUT_T *rotate_out_ptr //in/out:
                                            );

/*****************************************************************************/
//  Description : rotate image arbitrary angle
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_RESULT_E GUIANIM_RotateAngle(
                                            GUIANIM_ROTATE_ANGLE_INPUT_T    *rotate_in_ptr, //in:
                                            GUIANIM_ROTATE_ANGLE_OUTPUT_T   *rotate_out_ptr //in/out:
                                            );

/*****************************************************************************/
//  Description : 获取旋转后的图像数据
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC void GUIANIM_GetRotateImage(
                                  MMI_HANDLE_T          ctrl_handle,
                                  GUIIMG_BITMAP_T       *img_bitmap_ptr,
                                  MMI_IMAGE_ID_T        *img_id_ptr
                                  );

/*****************************************************************************/
//  Description : 获取图像数据
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC void GUIANIM_GetImgBuf(
                              MMI_HANDLE_T          ctrl_handle,
                              GUIIMG_BITMAP_T       *img_bitmap_ptr,
                              MMI_IMAGE_ID_T        *img_id_ptr
                              );

/*****************************************************************************/
//  Description : 根据控件handle获得图片的宽或者高
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI内部接口，对iconlist公开
/*****************************************************************************/
PUBLIC BOOLEAN GUIANIM_GetSrcImgWidthHeight(
                                            MMI_HANDLE_T     ctrl_handle,//in:
                                            uint16           *width_ptr, //in/out:may PNULL
                                            uint16           *height_ptr //in/out:may PNULL
                                            );
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
                                                        );

/*****************************************************************************/
//  Description : set anim border
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void GUIANIM_NotifyUpdateByPtr(
                                      CTRLANIM_OBJ_T    *anim_ctrl_ptr,
                                      BOOLEAN  is_notify_update// // 当背景为GUI_BG_NONE时，是否无条件给父亲发送MSG_NOTIFY_UPDATE
                                      );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

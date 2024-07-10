/*! @file guires.h
@brief  此文件用于定义跟图像资源相关的方法、类型
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 8/2007       Jassmine              Creat
*******************************************************************************/

/*! @page page_Res Res

(暂无)

*******************************************************************************/

/*! @addtogroup resGrp Res
@brief res模块
@details
该模块用于定义跟图像资源相关的方法、类型
@{
*******************************************************************************/

#ifndef _GUIRES_H_
#define _GUIRES_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guianim.h"

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

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @struct  GUIRES_IMG_SRC_T 
@brief  描述图片buffer的数据结构
*/
typedef struct
{
    uint8       *data_ptr;              /*!< 图片数据地址 */
    uint32      data_size;              /*!< 图片数据的大小 */
} GUIRES_IMG_SRC_T;


/*! @struct  GUIRES_IMG_INFO_T 
@brief  描述图片信息的数据结构
*/
typedef struct 
{
    BOOLEAN         is_argb;            /*!< 是否带透明信息 *///is argb
    uint16          width;              /*!< 图片宽度 *///image width
    uint16          height;             /*!< 图片高度 *///image height
    uint16          frame_num;          /*!< 图片帧数 *///frame number
    GUIANIM_TYPE_E  img_type;           /*!< 图片类型 *///image type
} GUIRES_IMG_INFO_T;


/*! @struct  GUIRES_DISPLAY_T 
@brief  图片的显示类型
*/
typedef struct 
{
    IMGREF_DATA_FORMAT_E    format;             /*!< 图片数据格式 */
    IMG_RES_BG_TYPE_E       bg_type;            /*!< 混合类型，显示时，直接覆盖，还是与背景混合。@note 仅在argb888下有效 *///only for argb888
    BOOLEAN                 is_res_bg_color;    /*!<是否支持资源背景颜色替换 */
    BOOLEAN                 is_exchange_color;  /*!<是否用颜色替换图片*/
    uint32                  res_bg_color;       /*!<资源背景替换颜色值 */
    uint32                  exchange_color;     /*!<图片替换颜色值 */
} GUIRES_DISPLAY_T;


/*! @struct  GUIRES_ANIM_DIS_T 
@brief  动画显示信息
*/
typedef struct 
{
    BOOLEAN         is_dis_ordinal;     /*!< 是否顺序显示图片,gif动画顺序显示 */
    BOOLEAN         is_next_frame;      /*!< 是否gif动画下一帧,否则显示当前帧 */
    uint16          frame_index;        /*!< 指定显示动画的某一帧,bmp,argb动画 */
    uint32          anim_handle;        /*!< 顺序显示图片,必须传入图片handle */
} GUIRES_ANIM_DIS_T;


/*! @struct  GUIRES_ANIM_INFO_T 
@brief  动画信息
*/
typedef struct 
{
    BOOLEAN         is_decode_all;      /*!< 是否全部解码完 *///is decode all frame
    uint16          cur_frame;          /*!< 当前帧索引 *///current frame index
    uint32          delay_time;         /*!< 动画延迟时间 *///delay time
} GUIRES_ANIM_INFO_T;

/*---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获取图片信息
@author Jassmine
@param src_ptr [in] 指向图片源数据的指针
@param info_ptr [in/out] 指向图片信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgInfo(
                                 GUIRES_IMG_SRC_T  *src_ptr,   //in
                                 GUIRES_IMG_INFO_T *info_ptr   //in/out
                                 );


/***************************************************************************//*!
@brief 获取指定的图片是否带透明信息
@author Jassmine
@param img_id [in] 图片ID
@param win_id [in] 窗口ID
@return 图片是否带有透明信息
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_IsArgb(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             );


/***************************************************************************//*!
@brief 通过图片ID获取宽度和高度
@author Jassmine
@param width_ptr [in/out] 指向图片宽度的指针
@param height_ptr [in/out] 指向图片高度的指针
@param img_id [in] 图片ID
@param win_id [in] 窗口ID
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeight(
                                        uint16          *width_ptr, //may PNULL
                                        uint16          *height_ptr,//may PNULL
                                        MMI_IMAGE_ID_T  img_id,     //in
                                        MMI_WIN_ID_T    win_id      //in
                                        );


/***************************************************************************//*!
@brief 通过图片buffer获取宽度和高度
@author Jassmine
@param data_ptr [in] 图片buffer
@param width_ptr [in] 指向图片宽度的指针
@param height_ptr [in] 指向图片高度的指针
@param data_size [in] 图片buffer的大小
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeightByPtr(
                                             uint8         *data_ptr,  //in:
                                             uint16        *width_ptr, //may PNULL
                                             uint16        *height_ptr,//may PNULL
                                             uint32        data_size   //in:
                                             );


/***************************************************************************//*!
@brief 将图片画在指定的LCD上
@author Jassmine
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImg(
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 );

/***************************************************************************//*!
@brief 将sabm格式的图片画在指定的LCD上, 若非sabm的图片, 则正常显示, 不会拉伸
@author James.Zhang
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。不能为空
@param full_rect_ptr  [in] 指定图片的原始区域。如果为空，则显示整张图片。
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplaySabmImg(
                                     GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                     GUI_RECT_T             *full_rect_ptr,     //in:may PNULL代表指定rect区域显示
                                     GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                     MMI_WIN_ID_T           win_id,             //in:win id
                                     MMI_IMAGE_ID_T         img_id,             //in:image id
                                     const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                     );


/***************************************************************************//*!
@brief 将图片画在指定的LCD上
@author Jassmine
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dev_info_ptr [in] 图层信息
@param display_ptr [in] 图片显示类型
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgEx(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr        //in:display info
                                   );

/***************************************************************************//*!
@brief 将图片画在指定的LCD上,并以指定颜色替换图片颜色
@author kevin.lou
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dev_info_ptr [in] 图层信息
@param is_exchange_color [in] 是否颜色替换
@param color [in] 替换目标颜色值
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
- 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgWitchColorExchange(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   BOOLEAN                  is_exchange_color,  //in:是否颜色替换
                                   uint32                   color               //in:替换目标颜色值
                                   );


/***************************************************************************//*!
@brief 通过图片buffer显示在指定的LCD上
@author Jassmine
@param data_ptr [in] 图片buffer
@param data_size [in] 图片buffer的大小
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtr(
                                      uint8                     *data_ptr,          //in:
                                      uint32                    data_size,          //in:
                                      GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                      GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                      GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                      const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                      );


/***************************************************************************//*!
@brief 通过图片buffer显示在指定的LCD上
@author Jassmine
@param data_ptr [in] 图片buffer
@param data_size [in] 图片buffer的大小
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param dev_info_ptr [in] 图层信息
@param display_ptr [in] 图片显示类型
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtrEx(
                                        uint8                   *data_ptr,          //in:
                                        uint32                  data_size,          //in:
                                        GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                        GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                        GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                        const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                        GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                        );


/***************************************************************************//*!
@brief 获取动画的宽度和高度
@author Jassmine
@param width_ptr [in] 指向动画宽度的指针，可以为空
@param height_ptr [in] 指向动画高度的指针，可以为空
@param frame_num_ptr [in] 指向动画帧数的指针，可以为空
@param anim_id [in] 动画ID
@param win_id [in] 窗口ID
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeight(
                                         uint16         *width_ptr,     //in/out:may PNULL
                                         uint16         *height_ptr,    //in/out:may PNULL
                                         uint16         *frame_num_ptr, //in/out:may PNULL
                                         MMI_ANIM_ID_T  anim_id,        //in
                                         MMI_WIN_ID_T   win_id          //in
                                         );


/***************************************************************************//*!
@brief 通过动画buffer获取宽度和高度
@author Jassmine
@param data_ptr [in] 指向动画buffer
@param width_ptr [in] 指向动画宽度的指针，可以为空
@param height_ptr [in] 指向动画高度的指针，可以为空
@param frame_num_ptr [in] 指向动画帧数的指针，可以为空
@param data_size [in] 动画buffer的大小
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeightByPtr(
                                              uint8          *data_ptr,      //in:may PNULL
                                              uint16         *width_ptr,     //in/out:may PNULL
                                              uint16         *height_ptr,    //in/out:may PNULL
                                              uint16         *frame_num_ptr, //in/out:may PNULL
                                              uint32         data_size       //in:
                                              );


/***************************************************************************//*!
@brief 将动画显示在指定的LCD上
@author Jassmine
@param frame_index [in] 指定第几帧，从0开始计数
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param win_id [in] 窗口ID
@param anim_id [in] 动画ID
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnim(
                                  uint16                    frame_index,        //in:from 0
                                  GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                  GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                  GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                  MMI_WIN_ID_T              win_id,             //in:win id
                                  MMI_ANIM_ID_T             anim_id,            //in:anim id
                                  const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                  );

/***************************************************************************//*!
@brief 将动画显示在指定的LCD上
@author Jassmine
@param frame_index [in] 指定第几帧，从0开始计数
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param win_id [in] 窗口ID
@param anim_id [in] 动画ID
@param dev_info_ptr [in] 图层信息
@param display_ptr [in] 动画显示类型
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimEx(
                                    uint16                  frame_index,        //in:from 0
                                    GUI_POINT_T             *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                    GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                    MMI_WIN_ID_T            win_id,             //in:win id
                                    MMI_ANIM_ID_T           anim_id,            //in:anim id
                                    const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                    GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                    );


/***************************************************************************//*!
@brief 通过动画buffer显示在指定的LCD上
@author Jassmine
@param data_ptr [in] 指向动画buffer
@param frame_index [in] 指定第几帧，从0开始计数
@param data_size [in] 动画buffer的大小
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtr(
                                       uint8                     *data_ptr,          //in:
                                       uint16                    frame_index,        //in:from 0
                                       uint32                    data_size,          //in:
                                       GUI_POINT_T               *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                       GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                       GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                       const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                       );


/***************************************************************************//*!
@brief 通过动画buffer显示在指定的LCD上
@author Jassmine
@param data_ptr [in] 指向动画buffer
@param frame_index [in] 指定第几帧，从0开始计数
@param data_size [in] 动画buffer的大小
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param dev_info_ptr [in] 图层信息
@param display_ptr [in] 动画显示类型
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtrEx(
                                         uint8                  *data_ptr,          //in:
                                         uint16                 frame_index,        //in:from 0
                                         uint32                 data_size,          //in:
                                         GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                         GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                         GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,      //in:
                                         GUIRES_DISPLAY_T       *display_ptr        //in:display info
                                         );


/***************************************************************************//*!
@brief 将图片拷贝到指定的内存
@author Jassmine
@param src_data_ptr [in] 指向源图片buffer
@param dest_width [in] 目标buffer的宽度
@param dest_height [in] 目标buffer的高度
@param dest_buf_size [in] 目标buffer的大小
@param src_data_size [in] 源图片buffer的大小
@param src_point_ptr [in] 指定源图片的起点位置，可以为空
@param src_rect_ptr [in] 指定源图片的区域，可以为空
@param dest_buf_ptr [in/out] 指向目标buffer
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBuf(
                                 const uint8   *src_data_ptr,  //in:src data buffer
                                 uint16        dest_width,     //in:
                                 uint16        dest_height,    //in:
                                 uint32        dest_buf_size,  //in:
                                 uint32        src_data_size,  //in:
                                 GUI_POINT_T   *src_point_ptr, //in:may PNULL代表指定rect区域
                                 GUI_RECT_T    *src_rect_ptr,  //in:may PNULL代表指定起始点
                                 GUI_COLOR_T   *dest_buf_ptr   //out:dest buffer
                                 );


/***************************************************************************//*!
@brief 将图片拷贝到指定的内存
@author Jassmine
@param src_data_ptr [in] 指向源图片buffer
@param dest_width [in] 目标buffer的宽度
@param dest_height [in] 目标buffer的高度
@param dest_buf_size [in] 目标buffer的大小
@param src_data_size [in] 源图片buffer的大小
@param src_point_ptr [in] 指定源图片的起点位置，可以为空
@param src_rect_ptr [in] 指定源图片的区域，可以为空
@param dest_buf_ptr [in/out] 指向目标buffer
@param display_ptr [in] 图片显示类型
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBufEx(
                                   const uint8          *src_data_ptr,  //in:src data buffer
                                   uint16               dest_width,     //in:
                                   uint16               dest_height,    //in:
                                   uint32               dest_buf_size,  //in:
                                   uint32               src_data_size,  //in:
                                   GUI_POINT_T          *src_point_ptr, //in:may PNULL代表指定rect区域
                                   GUI_RECT_T           *src_rect_ptr,  //in:may PNULL代表指定起始点
                                   void                 *dest_buf_ptr,  //out:dest buffer,888必须32bit
                                   GUIRES_DISPLAY_T     *display_ptr    //in:display info
                                   );


/***************************************************************************//*!
@brief 创建图片的句柄
@author Jassmine
@param handle_ptr [in/out] 图片的句柄
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dev_info_ptr [in] 图层信息(已废弃)
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CreatHandle(
                                  uint32            *handle_ptr,//in/out:
                                  MMI_WIN_ID_T      win_id,     //in:win id
                                  MMI_IMAGE_ID_T    img_id,      //in:image id
                                  GUI_LCD_DEV_INFO  *dev_info_ptr
                                  );


/***************************************************************************//*!
@brief  释放图片的句柄
@author Jassmine
@param handle [in] 图片的句柄
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_FreeHandle(
                                 uint32     handle//in/out:
                                 );


/***************************************************************************//*!
@brief  显示gif动画
@author Jassmine
@param is_next_frame [in] 是否gif动画下一帧,否则显示当前帧
@param handle [in] 图片的句柄
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param gif_info_ptr [in/out] 返回动画显示后的信息
@param dev_info_ptr [in] 图层信息
@return 是否调用成功
@note Gif动画只能逐帧显示,不能指定当前帧
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGif(
                                 BOOLEAN                is_next_frame,      //in:
                                 uint32                 handle,             //in:
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 GUIRES_ANIM_INFO_T     *gif_info_ptr,      //in/out:may PNULL
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 );


/***************************************************************************//*!
@brief 显示gif动画
@author Jassmine
@param is_next_frame [in] 是否gif动画下一帧,否则显示当前帧
@param handle [in] 图片的句柄
@param display_point_ptr [in] 指定动画在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定动画在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的动画区域。如果为空，则显示整张动画。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param gif_info_ptr [in/out] 返回动画显示后的信息
@param dev_info_ptr [in] 图层信息
@param display_ptr [in] 图片显示类型
@return 是否调用成功
@note Gif动画只能逐帧显示,不能指定当前帧
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGifEx(
                                   BOOLEAN                  is_next_frame,      //in:
                                   uint32                   handle,             //in:
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL代表指定rect区域显示
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   GUIRES_ANIM_INFO_T       *gif_info_ptr,      //in/out:may PNULL
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr    //in:display info
                                   );

/***************************************************************************//*!
@brief 获取指定的图片是否可拉伸
@author Jassmine
@param img_id [in] 图片ID
@param win_id [in] 窗口ID
@return 图片是否可拉伸
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_IsSabm(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             );

/***************************************************************************//*!
@brief 将图片画在指定的LCD上
@author lianxiang.zhou
@param display_point_ptr [in] 指定图片在LCD上的起点位置，宽度为整个LCD宽度。如果为空，将用display_rect_ptr的数据
@param display_rect_ptr [in] 指定图片在LCD上的显示区域。如果为空，将用display_point_ptr的数据
@param image_rect_ptr [in] 指定需要显示的图片区域。如果为空，则显示整张图片。
@param win_id [in] 窗口ID
@param img_id [in] 图片ID
@param dc_data_ptr [in] 图层信息
@return 是否调用成功
@note 
- display_point_ptr和display_rect_ptr不能同时为空，如果同时存在，则用display_point_ptr
- 图片显示的位置为display_point_ptr或display_rect_ptr，与image_rect_ptr的交集
- 直接将图片显示在层的相对位置上，免去中间位置转换
- 同时免去update rect和图层的alpha效果操作
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgDst(
    GUI_POINT_T            *display_point_ptr, //in:may PNULL代表指定rect区域显示
    GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL代表指定起始点显示
    GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL代表从图片(0,0)点显示
    MMI_WIN_ID_T           win_id,
    MMI_IMAGE_ID_T         img_id,
    GUILCD_DRAW_DC_T const *dc_data_ptr
);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

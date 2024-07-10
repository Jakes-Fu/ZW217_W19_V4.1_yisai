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

//�������ӵĺ�׺_320x240����
#define GUIANIM_MINIATURE_SUFFIX_LEN        10
        
#define GUIANIM_BUF_ALIGN                   256

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUIANIM_SII_RESULT_E
@brief ����ֵ
*/
typedef enum
{
    GUIANIM_SII_SUCC,                   /*!<�ɹ�*/
    GUIANIM_SII_FAIL,                   /*!<ʧ��*/
    GUIANIM_SII_JPG_ENCODE_SUCC,        /*!<jpgѹ���ɹ�*/
    GUIANIM_SII_JPG_ENCODE_FAIL,        /*!<jpgѹ��ʧ��*/
    GUIANIM_SII_DECODE_THUMB_FAIL,      /*!<����jpg thumbnailʧ��*/
    GUIANIM_SII_DECODE_MINIATURE_FAIL,  /*!<����jpg ����ͼʧ��*/
    GUIANIM_SII_MAX                     /*!<����λ*/
} GUIANIM_SII_RESULT_E;

/*! @struct GUIANIM_ZOOM_T
@brief ������Ϣ
*/
typedef struct 
{
    uint8               *buf_ptr;       /*!<����Ŀ��buffer*/
    uint32              buf_size;       /*!<����Ŀ��buffer��С*/
    uint32              buf_offset;     /*!<����Ŀ��bufferƫ��,Ҫ��256�ֽڶ���*/
    uint16              buf_width;      /*!<Ŀ��buffer��,���ɼ�ͼƬ�Ŀ�*/
    uint16              buf_height;     /*!<Ŀ��buffer��,���ɼ�ͼƬ�ĸ�*/
    uint16              cur_img_width;  /*!<���ź�ǰͼƬ��,�������ɼ�����*/
    uint16              cur_img_height; /*!<���ź�ǰͼƬ��,�������ɼ�����*/
    GUI_RECT_T          cur_clip_rect;  /*!<����ڵ�ǰͼƬ�ļ�������*/
    GUIIMG_DATE_TYPE_E  data_type;      /*!<���ź���������*/
} GUIANIM_ZOOM_T;

//rotate info
typedef struct 
{
    uint8               *buf_ptr;       /*!<��תĿ��buffer*/
    uint32              buf_size;       /*!<��תĿ��buffer��С*/
    uint32              buf_offset;     /*!<��תĿ��bufferƫ��,Ҫ��256�ֽڶ���*/
    uint16              img_width;      /*!<��ת��ǰͼƬ��*/
    uint16              img_height;     /*!<��ת��ǰͼƬ��*/
    GUIIMG_DATE_TYPE_E  data_type;      /*!<��ת����������*/
} GUIANIM_ROTATE_T;

// anim init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    BOOLEAN                     is_form_bg; /*!<�Ƿ���form�ؼ�����*/
} CTRLANIM_INIT_PARAM_T;

//anim control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                         is_focus;           /*!<�����Ƿ��ڽ���״̬*/
    BOOLEAN                         is_pause;           /*!<�����Ƿ���ʱ*/
    BOOLEAN                         is_visible;         /*!<�����Ƿ�ɼ�*/
    BOOLEAN                         is_form_bg;         /*!<�����Ƿ���form�ؼ�����*/

    BOOLEAN                         is_pressed;         /*!<anim is pressed*/
    uint8                           timer_id;           /*!<������ʱ��*/
    uint8                           gif_timer_id;       /*!<gif��ʱ��*/

    BOOLEAN                         is_wallpaper_ctrl;  /*!<�Ƿ���ǽֽ�ؼ�*/
    BOOLEAN                         is_save_wallpaper;  /*!<�Ƿ񱣴�ǽֽbitmap����*/
    BOOLEAN                         is_gif_wallpaper_saved; /*<gifǽֽ�����һ֡���ٱ���*/    
    BOOLEAN                         is_auto_adapt_disp; /*!<�Ƿ�����Ӧ��ʾ,��ͼƬ����ڳ�,������ʾ,����������ʾ*/
    BOOLEAN                         is_decoding;        /*!<�Ƿ����ڽ���*/

    uint8                           *bg_buf_ptr;        /*!<�Ƿ񱸷ݱ���*/
    BOOLEAN                         is_decode_thumb;    /*!<�Ƿ��ڽ���thumbnail*/
    BOOLEAN		                    is_encode_jpg;		/*!<�Ƿ���ѹ��jpg*/
    BOOLEAN                         is_exist_miniature; /*!<�Ƿ��������ͼ*/
    BOOLEAN                         is_rotate;          /*!<�Ƿ���ת*/

    BOOLEAN                         is_decoded;         /*!<�Ƿ��Ѿ��������*/
    BOOLEAN                         is_support;         /*!<ͼƬ��ʽ�Ƿ�֧��*/
    BOOLEAN                         is_support_zoom;    /*!<ͼƬ��ʽ�Ƿ�֧������*/
    BOOLEAN                         is_adjust_disp_pos; /*!<��ʾλ���Ƿ��Ѿ�����,��ֹgif��ε���*/
    BOOLEAN                         is_send_disp_ind;   /*!<�Ƿ��Ѿ�����MSG_CTL_ANIM_DISPLAY_IND��Ϣ,��ֹgif��η���*/
    BOOLEAN                         is_notify_update;   /*!<������ΪGUI_BG_NONEʱ���Ƿ������������׷���MSG_NOTIFY_UPDATE*/
    
    BOOLEAN                         is_next_frame;      /*!<�Ƿ����gif������Դ��һ��*/
    BOOLEAN                         is_dec_all_gif;     /*!<gif������Դ�Ƿ�������*/

    BOOLEAN                         is_target_alloc;    /*!<�Ƿ�����Ŀ��buffer*/
    uint8                           *target_buffer;     /*!<Ŀ��buffer*/
    uint8                           *target_backup_buf; /*!<Ŀ�걸��buffer,ֻ���gif*/
    uint32                          target_buffer_size; /*!<Ŀ��buffer��С*/
    uint32                          target_buf_offset;  /*!<Ŀ��bufferƫ��,Ҫ��256�ֽڶ���*/
    uint32                          target_backup_offset;/*!<Ŀ�걸��bufferƫ��,Ҫ��256�ֽڶ���*/
    GUIIMG_DATE_TYPE_E              target_data_type;   //target data type

    uint8                           *src_data_buffer;   /*!<Դ����*/
    uint32                          src_data_size;      /*!<Դ���ݴ�С*/

    uint16                          zoom_min_width;     /*!<������С���*/
    uint16                          zoom_min_height;    /*!<������С�߶�*/
    GUIANIM_ZOOM_T                  zoom_dest;          /*!<����Ŀ����Ϣ*/

    GUIANIM_ROTATE_T                rotate_dest;        /*!<��תĿ����Ϣ*/

    uint32                          res_gif_handle;     /*!<gif��Դhandle*/
    uint32                          img_handle;         /*!<ͼƬhandle*/
    
    uint16                          src_width;          /*!<ԭͼ��*/
    uint16                          src_height;         /*!<ԭͼ��*/
    uint16                          img_width;          /*!<ͼƬ��*/
    uint16                          img_height;         /*!<ͼƬ��*/
    uint16                          thumb_width;        /*!<thumbnail��*/
    uint16                          thumb_height;       /*!<thumbnail��*/
    uint16                          cur_frame;          /*!<��ǰ������*/
    uint16                          total_frame;        /*!<������*/
    uint16                          ico_frame_index;    //ico frame index 

    uint32                          delay_time;         /*!<ÿ�嶯��ʱ����*/
    
    int16                           display_x;          /*!<ͼƬ��ʾx����,ͼƬ����Align�������ʾλ��*/
    int16                           display_y;          /*!<ͼƬ��ʾy����,ͼƬ����Align�������ʾλ��*/
    GUIANIM_TYPE_E                  type;               /*!<ͼƬ����*/

    GUIANIM_DATA_INFO_T             data_info;          /*!<ͼƬ������Ϣ,����image id,data����bitmap��*/
    GUIANIM_FILE_INFO_T             file_info;          /*!<ͼƬ�ļ���Ϣ*/

    GUIANIM_DISPLAY_INFO_T          display_info;       /*!<ͼƬxianshi��Ϣ*/

    uint16                          miniature_width;    /*!<����ͼ��*/
    uint16                          miniature_height;   /*!<����ͼ��*/
    GUIANIM_MINIATURE_FULL_PATH_T   miniature_path;     /*!<����ͼ�ļ���*/

    GUIANIM_CALL_BACK_T             CallBackFunc;       /*!<�ص�����*/

    MMITHEME_ANIM_T                 theme;              /*!<�������*/

    MMI_HANDLE_T                    win_handle;         /*!<����handle*/
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
    BOOLEAN             is_decode_gif_end;  //for gif,GIF�Ƿ�������
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
    uint32              dest_buf_offset;//dest buffer����256����
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
    uint32              dest_buf_offset;//dest buffer����256����
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
                               BOOLEAN          is_full_paint,  //�Ƿ���full paint��Ϣ
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
                                            uint16              *full_path_ptr, //in:�ļ���,may PNULL
                                            uint32              *img_handle_ptr,//in/out:
                                            GUIANIM_IMG_DATA_T  *src_data_ptr   //in:�����ͼƬ����,may PNULL
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
                                        GUI_RECT_T          *rect_ptr,      //in:ico�ļ���Ҫƥ��rect;
                                        wchar               *full_path_ptr, //in:�ļ���,may PNULL
                                        GUIANIM_IMG_DATA_T  *src_data_ptr,  //in:�����ͼƬ����,may PNULL
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
//  Description : ��ȡ��ת���ͼ������
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI�ڲ��ӿڣ���iconlist����
/*****************************************************************************/
PUBLIC void GUIANIM_GetRotateImage(
                                  MMI_HANDLE_T          ctrl_handle,
                                  GUIIMG_BITMAP_T       *img_bitmap_ptr,
                                  MMI_IMAGE_ID_T        *img_id_ptr
                                  );

/*****************************************************************************/
//  Description : ��ȡͼ������
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI�ڲ��ӿڣ���iconlist����
/*****************************************************************************/
PUBLIC void GUIANIM_GetImgBuf(
                              MMI_HANDLE_T          ctrl_handle,
                              GUIIMG_BITMAP_T       *img_bitmap_ptr,
                              MMI_IMAGE_ID_T        *img_id_ptr
                              );

/*****************************************************************************/
//  Description : ���ݿؼ�handle���ͼƬ�Ŀ���߸�
//  Global resource dependence : 
//  Author:hua.fang
//  Note: 
//  GUI�ڲ��ӿڣ���iconlist����
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
                                                        wchar               *full_path_ptr, //in:�ļ���,may PNULL
                                                        GUIANIM_IMG_DATA_T  *src_data_ptr,  //in:�����ͼƬ����,may PNULL
                                                        GUIANIM_IMG_INFO_T  *img_info_ptr,  //in/out:
                                                        GUI_RECT_T          *rect_ptr,      //ico�ļ�ƥ��rect;
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
                                      BOOLEAN  is_notify_update// // ������ΪGUI_BG_NONEʱ���Ƿ������������׷���MSG_NOTIFY_UPDATE
                                      );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

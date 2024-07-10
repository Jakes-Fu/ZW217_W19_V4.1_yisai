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
@brief ����ͼȫ�ļ�������
*/
#define GUIANIM_MINIATURE_FULL_PATH_LEN     255

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum GUIANIM_ALIGN_STYLE_E
@brief ������ʾ���з�ʽ
*/
typedef enum
{
    GUIANIM_ALIGN_NONE,             /*!<����λ*/
    GUIANIM_ALIGN_LEFT_TOP,         /*!<����*/
    GUIANIM_ALIGN_LEFT_VMIDDLE,     /*!<����*/
    GUIANIM_ALIGN_LEFT_BOTTOM,      /*!<����*/
    GUIANIM_ALIGN_HMIDDLE_TOP,      /*!<����*/
    GUIANIM_ALIGN_HVMIDDLE,         /*!<��*/
    GUIANIM_ALIGN_HMIDDLE_BOTTOM,   /*!<����*/
    GUIANIM_ALIGN_RIGHT_TOP,        /*!<����*/
    GUIANIM_ALIGN_RIGHT_VMIDDLE,    /*!<����*/
    GUIANIM_ALIGN_RIGHT_BOTTOM,     /*!<����*/
    GUIANIM_ALIGN_MAX               /*!<����λ*/
} GUIANIM_ALIGN_STYLE_E;

/*! @enum GUIANIM_RESULT_E
@brief ����ֵ
*/
typedef enum
{
    GUIANIM_RESULT_SUCC,                    /*!<�ɹ�*/
    GUIANIM_RESULT_FAIL,                    /*!<ʧ��*/
    GUIANIM_RESULT_WAIT,                    /*!<�ȴ�*/
    GUIANIM_RESULT_BUSY,                    /*!<��æ*/
    GUIANIM_RESULT_BIG,                     /*!<�ļ�̫��*/
    GUIANIM_RESULT_EMPTY,                   /*!<����Ϊ��*/
    GUIANIM_RESULT_SFS_FAIL,                /*!<�ļ�ϵͳ����*/
    GUIANIM_RESULT_BAD_DATA,                /*!<��������*/
    GUIANIM_RESULT_INTERRUPT,               /*!<��ѹ��task������*/
    GUIANIM_RESULT_NO_SUPPORT,              /*!<��֧��*/
    GUIANIM_RESULT_LACK_MEMORY,             /*!<ȱ���ڴ�*/
    GUIANIM_RESULT_MINIATURE_FAIL,          /*!<jpg��������ͼʧ��*/
    GUIANIM_RESULT_BIG_RESOLUTION,          /*!<�ֱ���̫��*/
    GUIANIM_RESULT_GIF_MULTI_LACK_MEMORY,   /*!<gif����,ȱ���ڴ�*/
    GUIANIM_RESULT_WRONG_PARAM,             /*!<��������*/
    GUIANIM_RESULT_MAX                      /*!<����λ*/
} GUIANIM_RESULT_E;

/*! @enum GUIANIM_ROTATE_E
@brief ������תģʽ
*/
typedef enum
{
    GUIANIM_ROTATE_NONE,    /*!<����λ*/
    GUIANIM_ROTATE_90,      /*!<90��*/
    GUIANIM_ROTATE_180,     /*!<180��*/
    GUIANIM_ROTATE_270,     /*!<270��*/
    GUIANIM_ROTATE_MIRROR,  /*!<����*/
    GUIANIM_ROTATE_MAX      /*!<����λ*/
} GUIANIM_ROTATE_E;

/*! @struct GUIANIM_INIT_DATA_T
@brief �����ؼ���ʼ������
*/
typedef struct 
{
    BOOLEAN             is_form_bg; /*!<�Ƿ���form�ؼ�����*/
    BOOLEAN             is_spinner;
    GUI_BOTH_RECT_T     both_rect;  /*!<��������*/
} GUIANIM_INIT_DATA_T;

/*! @struct GUIANIM_DATA_INFO_T
@brief ����������Ϣ
*/
typedef struct 
{
    BOOLEAN             is_bitmap;      /*!<�Ƿ���bitmap��Ϣ*/
    BOOLEAN             is_free_bitmap; /*!<�Ƿ���Anim�ͷ�bitmap��Ϣ*/
    BOOLEAN             is_save_data;   /*!<�Ƿ񱣴�data��Ϣ*/
    uint8               *data_ptr;      /*!<data��Ϣ*/
    uint32              data_size;      /*!<data���ݳ���*/
    GUIIMG_BITMAP_T     bitmap_data;    /*!<bitmap������Ϣ*/
    MMI_ANIM_ID_T       anim_id;        /*!<����id*/
    MMI_IMAGE_ID_T      img_id;         /*!<ͼƬid*/
} GUIANIM_DATA_INFO_T;

/*! @struct GUIANIM_FILE_INFO_T
@brief �ļ���Ϣ
*/
typedef struct 
{
    wchar       *full_path_wstr_ptr; /*!<ȫ·����*/
    uint16      full_path_wstr_len;  /*!<ȫ·��������*/
} GUIANIM_FILE_INFO_T;

typedef enum
{
    GUIANIM_MASK_TYPE_PATH,
    GUIANIM_MASK_TYPE_ID,
    GUIANIM_MASK_TYPE_MAX
}GUIANIM_MASK_TYPE_E;

/*! @struct GUIANIM_DISPLAY_INFO_T
@brief ��ʾ��Ϣ
*/
typedef struct 
{
    BOOLEAN                 is_syn_decode;          /*!<�Ƿ�ͬ������*/
    BOOLEAN                 is_update;              /*!<�Ƿ�����ˢ��*/
    BOOLEAN                 is_zoom;                /*!<�Ƿ���������*/
    BOOLEAN                 is_auto_zoom_in;        /*!<�Ƿ��Զ��Ŵ�Сͼ���ؼ���С,������,��֧���ļ�ͼƬ*/
    BOOLEAN                 is_crop_rect;           /*!<�Ƿ�ü�ͼƬ*/

    BOOLEAN                 is_play_once;           /*!<����Ƕ���,�Ƿ�ֻ����һ��*/
    BOOLEAN                 is_disp_one_frame;      /*!<�Ƿ�ֻ��ʾһ֡ͼƬ,����Ź���Ԥ��gif,ѡ����˵�,ֻ��ʾ����ͼƬ,����ʾ����*/
    BOOLEAN                 is_need_end_ind;        /*!<�Ƿ���Ҫ��������Gif��������һ����ϵ���Ϣ*/
    BOOLEAN                 is_save_wallpaper;      /*!<�Ƿ񱣴�ǽֽbitmap����,ֻ���ǽֽ*/
    
    BOOLEAN                 is_disp_frame;          /*!<�Ƿ���ʾͼƬ��,ֻ����ļ�ͼƬ*/
    BOOLEAN                 is_handle_transparent;  /*!<�Ƿ���͸��,ֻ���gif,png,�������ARGB888,����ͼƬ���ݵ�buffer����*/
    BOOLEAN                 is_no_miniatur;         /*!<�Ƿ�֧������ͼ*/
    BOOLEAN                 is_free_buf_lose_focus; /*!<anim lose focusʱ,�Ƿ��ͷ�target buffer,Ĭ�Ϸ�,����get focus����Ҫ���½��룬����*/
    
    BOOLEAN                 is_bg_buf;              /*!<�Ƿ��Ʊ�������,ֻ���Gif����*/
    GUI_BG_T                bg;                     /*!<�ؼ�����*/

    uint16                  specify_width;          /*!<ָ�������ͼƬ�������*/
    uint16                  specify_height;         /*!<ָ�������ͼƬ�����߶�*/
    uint16                  rotate_angle;           /*!<��ת�Ƕ�0-360,�����ת,�ٶ���*/
    GUIANIM_ROTATE_E        rotate_mode;            /*!<��תģʽ,Ӳ����ת,�ٶȿ�*/
    GUIANIM_ALIGN_STYLE_E   align_style;            /*!<������ʾ���з�ʽ*/
    BOOLEAN                 is_res_bg_color;        /*!<�Ƿ�Ҫ�滻��Դ�ı���ɫ*/
    uint32                  res_bg_color;           /*!<��Դ�ı���ɫֵ*/
    BOOLEAN                 is_img_res_need_buffer; /*!<��Դimg�Ƿ���Ҫbuffer��������ֻ��img id��Ч*/


    BOOLEAN                         is_handle_mask;    /*�Ƿ�ʹ���ɰ�*/
    GUIANIM_MASK_TYPE_E             mask_type;         /*�ɰ�����*/
    wchar*                          p_mask_path;       /*�ɰ�ľ���·�������mask_type=GUIANIM_MASK_TYPE_PATH,��ֵ����Ϊ0��*/
    uint16                          mask_path_len;     /*����·���ĳ��ȣ����mask_type=GUIANIM_MASK_TYPE_PATH,��ֵ����Ϊ0��*/
    MMI_IMAGE_ID_T                  mask_id;           /*�ɰ����Դid�����mask_type=GUIANIM_MASK_TYPE_ID,��ֵ����Ϊ0��*/
} GUIANIM_DISPLAY_INFO_T;

/*! @struct GUIANIM_MINIATURE_FULL_PATH_T
@brief ����ͼ�ļ���
*/
typedef struct 
{
    wchar       full_path_wstr[GUIANIM_MINIATURE_FULL_PATH_LEN + 1];/*!<ȫ·����*/
    uint16      full_path_wstr_len;                                 /*!<ȫ·��������*/
} GUIANIM_MINIATURE_FULL_PATH_T;

/*! @struct GUIANIM_OWNER_DRAW_T
@brief �Ի���Ϣ
*/
typedef struct 
{
    uint32              param;          /*!<�û�����*/
    int16               display_x;      /*!<ͼƬ��ʾx����*/
    int16               display_y;      /*!<ͼƬ��ʾy����*/
    GUI_RECT_T          display_rect;   /*!<�ؼ���ʾ����*/
    MMI_HANDLE_T        win_handle;     /*!<����handle*/
    MMI_HANDLE_T        ctrl_handle;    /*!<�ؼ�handle*/
    GUIIMG_BITMAP_T     bitmap_data;    /*!<bitmap����*/
    GUI_LCD_DEV_INFO    lcd_dev;        /*!<lcd��Ϣ*/
} GUIANIM_OWNER_DRAW_T;

/*! @typedef GUIANIM_OWNER_DRAW_FUNC
@brief �����Ի�ص�����
*/
typedef void (*GUIANIM_OWNER_DRAW_FUNC)(
                                        GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                                        );

/*! @struct GUIANIM_CALL_BACK_T
@brief �����Ի�ص���Ϣ
*/
typedef struct
{
    uint32                  param;          /*!<Я������*/
   GUIANIM_OWNER_DRAW_FUNC  OwnerDrawFunc;  /*!<�Ի�ص�����*/
} GUIANIM_CALL_BACK_T;

/*! @struct GUIANIM_CTRL_INFO_T
@brief �ؼ���Ϣ
*/
typedef struct 
{
    BOOLEAN             is_wallpaper_ctrl;  /*!<�Ƿ���ǽֽ�ؼ�*/
    BOOLEAN             is_ctrl_id;         /*!<�Ƿ���ؼ�id*/
    MMI_CTRL_ID_T       ctrl_id;            /*!<�ؼ�id*/
    void                *ctrl_ptr;          /*!<�ؼ�ָ��,����Ϊ��*/
} GUIANIM_CTRL_INFO_T;

/*! @struct GUIANIM_DISPLAY_IND_T
@brief ������ʾ��Ϣ����
*/
typedef struct 
{
    MMI_HANDLE_T        ctrl_handle;/*!<�ؼ�handle*/
    GUIANIM_RESULT_E    result;     /*!<������*/
} GUIANIM_DISPLAY_IND_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ���ö�������
@author Jassmine
@param ctrl_info_ptr    [in] �ؼ���Ϣ
@param data_info_ptr    [in] ������Ϣ,����Ϊ��
@param file_info_ptr    [in] �ļ���Ϣ,����Ϊ��
@param display_info_ptr [in] ��ʾ��Ϣ
@return �������
@note 
*******************************************************************************/
PUBLIC GUIANIM_RESULT_E CTRLANIM_SetParam(
                                         GUIANIM_CTRL_INFO_T    *ctrl_info_ptr,     //in:
                                         GUIANIM_DATA_INFO_T    *data_info_ptr,     //in:may PNULL
                                         GUIANIM_FILE_INFO_T    *file_info_ptr,     //in:may PNULL
                                         GUIANIM_DISPLAY_INFO_T *display_info_ptr   //in:
                                         );

/***************************************************************************//*!
@brief �������з�ʽ,����ͼƬ��ʾλ��
@author Jassmine
@param display_x_ptr [in/out] ͼƬ��ʾx����
@param display_y_ptr [in/out] ͼƬ��ʾx����
@param pic_width     [in] ͼƬ��
@param pic_height    [in] ͼƬ��
@param rect          [in] ��ʾ����
@param align_style   [in] ���з�ʽ
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
@brief �����ļ������ͼƬ�Ŀ���߸�
@author Jassmine
@param width_ptr     [in] ͼƬ��,����Ϊ��
@param height_ptr    [in] ͼƬ��,����Ϊ��
@param file_path_ptr [in] ȫ�ļ���
@param file_len      [in] ȫ�ļ�������
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeight(
                                         uint16      *width_ptr,     //in/out:may PNULL
                                         uint16      *height_ptr,    //in/out:may PNULL
                                         wchar       *file_path_ptr, //in:
                                         uint16      file_len        //in:
                                         );

/***************************************************************************//*!
@brief ���ݿؼ�handle���ͼƬ�Ŀ���߸�
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param width_ptr   [in/out] ͼƬ��,����Ϊ��
@param height_ptr  [in/out] ͼƬ��,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetImgWidthHeightEx(
                                           MMI_HANDLE_T     ctrl_handle,//in:
                                           uint16           *width_ptr, //in/out:may PNULL
                                           uint16           *height_ptr //in/out:may PNULL
                                           );

/***************************************************************************//*!
@brief ���ͼƬ��ʾ����ʼ������
@author Jassmine
@param ctrl_handle   [in] �ؼ�handle
@param display_x_ptr [in/out] ��ʼ��x����
@param display_y_ptr [in/out] ��ʼ��y����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayPoint(
                                       MMI_HANDLE_T ctrl_handle,    //in:
                                       int16        *display_x_ptr, //in/out:
                                       int16        *display_y_ptr  //in/out:
                                       );

/***************************************************************************//*!
@brief ���ͼƬ���ŵĿ��ߵ���С,���ֵ
@author Jassmine
@param ctrl_handle    [in] �ؼ�handle
@param width_min_ptr  [in/out] ͼƬ����Сֵ,����Ϊ��
@param width_max_ptr  [in/out] ͼƬ�����ֵ,����Ϊ��
@param height_min_ptr [in/out] ͼƬ����Сֵ,����Ϊ��
@param height_max_ptr [in/out] ͼƬ�����ֵ,����Ϊ��
@return �����Ƿ�ɹ�
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
@brief ���ͼƬ���ŵļ�������
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in/out] ͼƬ���ŵļ�������
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetZoomClipRect(
                                       MMI_HANDLE_T     ctrl_handle,//in:
                                       GUI_RECT_T       *rect_ptr   //in/out
                                       );

/***************************************************************************//*!
@brief ����ͼƬ
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param is_zoom_in  [in] �Ƿ�Ŵ�
@param is_update   [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_Zoom(
                            MMI_HANDLE_T    ctrl_handle,
                            BOOLEAN         is_zoom_in,
                            BOOLEAN         is_update
                            );

/***************************************************************************//*!
@brief �ƶ�����ͼƬ
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param move_x      [in] x<0 �����ƶ�, x>0 �����ƶ�
@param move_y      [in] y<0 �����ƶ�, y>0 �����ƶ�
@param is_update   [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomMove(
                                MMI_HANDLE_T    ctrl_handle,    //in:
                                int16           move_x,         //in:x<0 move left, x>0 move right
                                int16           move_y,         //in:y<0 move up, y>0 move bottom
                                BOOLEAN         is_update       //in:
                                );
/***************************************************************************//*!
@brief ����ͼƬ������
@author kevin.lou
@param ctrl_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note û�ŵ����ʱ��ŵ���󣬷���������С
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_ZoomUtmost(
                                MMI_HANDLE_T    ctrl_handle    //in:
                                );

/***************************************************************************//*!
@brief ����ͼƬ���ű���
@author Jassmine
@param ctrl_handle          [in] �ؼ�handle
@param zoom_in_ratio_ptr    [in] �Ŵ����,�������1
@param zoom_out_ratio_ptr   [in] ��С����,����С��1
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetZoomRatio(
                                    MMI_HANDLE_T        ctrl_handle,        //in:
                                    MMITHEME_RATIO_T    *zoom_in_ratio_ptr, //in:may PNULL
                                    MMITHEME_RATIO_T    *zoom_out_ratio_ptr //in:may PNULL
                                    );

/***************************************************************************//*!
@brief ��תͼƬ90��,180��,270�Ȼ��߾���
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rotate_mode [in] ��תģʽ
@param is_update   [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
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
@brief ���Ŷ���
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnim(
                                MMI_HANDLE_T    ctrl_handle
                                );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�벥�Ŷ���
@author Jassmine
@param ctrl_ptr [in] �ؼ�ָ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_PlayAnimByPtr(
                                     void   *ctrl_ptr
                                     );

/***************************************************************************//*!
@brief ��ͣ����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_PauseAnim(
                              MMI_HANDLE_T ctrl_handle
                              );

/***************************************************************************//*!
@brief ��������
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_ResumeAnim(
                               MMI_HANDLE_T ctrl_handle
                               );

/***************************************************************************//*!
@brief ���ö����ؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in] ����
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRect(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_RECT_T      *rect_ptr
                                );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�����ö�������
@author Jassmine
@param ctrl_ptr     [in] �ؼ�ָ��
@param rect_ptr     [in] ����
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetCtrlRectByPtr(
                                     void           *ctrl_ptr,
                                     GUI_RECT_T     *rect_ptr
                                     );

/***************************************************************************//*!
@brief ��ö����ؼ���ʾ����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in/out] ����
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_GetDisplayRect(
                                      MMI_HANDLE_T  ctrl_handle,    //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      );

/***************************************************************************//*!
@brief ���ö����ؼ���ʾ����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in] ����
@param is_update   [in] �Ƿ�����ˢ��
@note ������ʾ���ͼƬ
*******************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRect(
                                   MMI_HANDLE_T     ctrl_handle,
                                   GUI_RECT_T       *rect_ptr,
                                   BOOLEAN          is_update
                                   );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�����ö����ؼ���ʾ����
@author Jassmine
@param ctrl_ptr      [in] �ؼ�ָ��
@param rect_ptr      [in] ����
@param is_update     [in] �Ƿ�����ˢ��
@note ������ʾ���ͼƬ
*******************************************************************************/
PUBLIC void CTRLANIM_SetDisplayRectByPtr(
                                        void            *ctrl_ptr,
                                        GUI_RECT_T      *rect_ptr,
                                        BOOLEAN         is_update
                                        );

/***************************************************************************//*!
@brief ���ö����ؼ�����ͼƬ
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param bg_img_id   [in] ͼƬid
@return �Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImg(
                                MMI_HANDLE_T    ctrl_handle,
                                MMI_IMAGE_ID_T  bg_img_id
                                );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�����ö����ؼ�����ͼƬ
@author Jassmine
@param ctrl_ptr      [in] �ؼ�ָ��
@param bg_img_id     [in] ͼƬid
@return �Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetBgImgByPtr(
                                     void               *ctrl_ptr,
                                     MMI_IMAGE_ID_T     bg_img_id
                                     );

/***************************************************************************//*!
@brief ���ö����ؼ�������ɫ
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param bg_color    [in] ��ɫ
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBgColor(
                               MMI_HANDLE_T     ctrl_handle,
                               GUI_COLOR_T      bg_color
                               );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�����ö����ؼ�������ɫ
@author Jassmine
@param ctrl_ptr      [in] �ؼ�ָ��
@param bg_color      [in] ��ɫ
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBgColorByPtr(
                                    void            *ctrl_ptr,
                                    GUI_COLOR_T     bg_color
                                    );

/***************************************************************************//*!
@brief ���ö����ؼ��߿�
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param border_ptr  [in] �߿�
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetBorder(
                              MMI_HANDLE_T  ctrl_handle,
                              GUI_BORDER_T  *border_ptr
                              );

/***************************************************************************//*!
@brief ���ö������
@author Jassmine
@param ctrl_handle  [in] �ؼ�handle
@param frame_ptr    [in] ���
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetFrame(
                             MMI_HANDLE_T   ctrl_handle,
                             GUI_BORDER_T   *frame_ptr
                             );

/***************************************************************************//*!
@brief ���ö����ؼ�Ĭ��ͼƬ
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param def_img_id  [in] Ĭ��ͼƬ
@param err_img_id  [in] Ĭ�ϴ���ͼƬ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIcon(
                                      MMI_HANDLE_T      ctrl_handle,
                                      MMI_IMAGE_ID_T    def_img_id, //default image id
                                      MMI_IMAGE_ID_T    err_img_id  //error image id
                                      );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ�����ö����ؼ�Ĭ��ͼƬ
@author Jassmine
@param ctrl_ptr      [in] �ؼ�ָ��
@param def_img_id    [in] Ĭ��ͼƬ
@param err_img_id    [in] Ĭ�ϴ���ͼƬ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetDefaultIconByPtr(
                                           void             *ctrl_ptr,
                                           MMI_IMAGE_ID_T   def_img_id,
                                           MMI_IMAGE_ID_T   err_img_id
                                           );

/***************************************************************************//*!
@brief ���ö����ؼ�ͼƬ������󱳾�
@author Jassmine
@param ctrl_handle  [in] �ؼ�handle
@param error_bg_ptr [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetErrorBg(
                                  MMI_HANDLE_T  ctrl_handle,
                                  GUI_BG_T      *error_bg_ptr
                                  );

/***************************************************************************//*!
@brief ���ö����ؼ��Ƿ�����Ӧ��������ʾ
@author Jassmine
@param is_adapt    [in] �Ƿ�����Ӧ
@param ctrl_handle [in] �ؼ�handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetAutoAdaptDisplay(
                                        BOOLEAN         is_adapt,   //in
                                        MMI_HANDLE_T    ctrl_handle //in
                                        );

/***************************************************************************//*!
@brief ���ö�����Сʱ����
@author Jassmine
@param min_period  [in] ��Сʱ����
@param ctrl_handle [in] �ؼ�handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetPeriod(
                              uint32        min_period, //in
                              MMI_HANDLE_T  ctrl_handle //in
                              );

/***************************************************************************//*!
@brief ˢ�¶���
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_UpdateDisplay(
                                  MMI_HANDLE_T  ctrl_handle
                                  );

/***************************************************************************//*!
@brief ˢ�¶����ƶ�����
@author Jassmine
@param rect        [in] ����
@param ctrl_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRect(
                                  GUI_RECT_T        rect,
                                  MMI_HANDLE_T      ctrl_handle
                                  );

/***************************************************************************//*!
@brief ͨ���ؼ�ָ��ˢ�¶����ƶ�����
@author Jassmine
@param rect          [in] ����
@param ctrl_ptr      [in] �ؼ�ָ��
@return �Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_UpdateRectByPtr(
                                       GUI_RECT_T   rect,
                                       void         *ctrl_ptr
                                       );

/***************************************************************************//*!
@brief ɾ������ͼ�ļ�
@author Jassmine
@param full_path_ptr [in] ȫ�ļ���
@param full_path_len [in] ȫ�ļ�������
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_DeleteMiniatureFile(
                                           const wchar  *full_path_ptr,     //in
                                           uint16       full_path_len       //in
                                           );

/***************************************************************************//*!
@brief ��ö����Ƿ�֧��,�������ܷ�ɹ�
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return �Ƿ�֧��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_IsSupport(
                                 MMI_HANDLE_T   ctrl_handle
                                 );

/***************************************************************************//*!
@brief ���ö����ؼ��Ƿ�ɼ�
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param is_visible  [in] �Ƿ�ɼ�
@param is_update   [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLANIM_SetVisible(
                                  MMI_HANDLE_T  ctrl_handle,
                                  BOOLEAN       is_visible,
                                  BOOLEAN       is_update
                                  );

/***************************************************************************//*!
@brief ���ö����ؼ��Ի溯��
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param func        [in] �Ի�ص�����
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDraw(
                                 MMI_HANDLE_T               ctrl_handle,
                                 GUIANIM_OWNER_DRAW_FUNC    func
                                 );

/***************************************************************************//*!
@brief ���ö����ؼ��Ի溯��
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param call_back_ptr  [in] �Ի�ص�����,����Я������
@note 
*******************************************************************************/
PUBLIC void CTRLANIM_SetOwnerDrawEx(
                                   MMI_HANDLE_T         ctrl_handle,
                                   GUIANIM_CALL_BACK_T  *call_back_ptr
                                   );

/***************************************************************************//*!
@brief ���ݿ�͸�,���ͼƬ��ת��Ŀ�͸�
@author Jassmine
@param width_ptr    [in/out] ͼƬ��
@param height_ptr   [in/out] ͼƬ��
@param rotate_angle [in] ��ת�Ƕ�(0-360)
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

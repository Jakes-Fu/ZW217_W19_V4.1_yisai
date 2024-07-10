/*! @file ctrlprogressbar.h
@brief ���ļ�������prgbox�ؼ��Ķ���ӿں���������
@author xiyuan.ma
@version 1.0
@date 01/10/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 01/2007      xiyuan.ma              Creat
*******************************************************************************/

/*! @page page_progressbar �������ͽ�����(PrgBox)

-# @ref sec_prgbox_function
-# @ref sec_prgbox_create
-# @ref sec_prgbox_app

@section sec_prgbox_function �ؼ�����

- Prgbox���������ͽ������ؼ�������prg��progress����д������ʾ�ı���ʱ�������ʾ����һҳ������Ļ������ʾ���ݣ���ͨ��ʹ�ù������ؼ������û�������ҳû����ʾ����������ʹ�����¼��л�����ҳ����һ�С�
- ���⣬�������ļ�ʱ������ʹ�ý�������ʾ�ļ�����Ľ��ȣ��������������ļ���web���صȡ�
- �������ͽ������ؼ���Ϊ������ʹ��ʱ��һ����Ƕ��List��Menu��Dropdownlist�ؼ��������ؼ��У��û�������Ĺ�������ʹ�á���Ȼ���û�Ҳ���Ե���ʹ�á�
- ��Ϊ�������ؼ���һ�㱻����Ӧ���ڴ����У������ʾ�����ļ��Ľ��ȡ�
- �������ȿ����Ǻ���ģ�Ҳ����������ģ����ܹ���Ӧ������Ϣ�����ʱ��Ϊ���������������¼�ͷ�����������Ϊ���Ҽ�ͷ�������磬�����ʵ�����¼�ͷ��ʱ�򣬿����л��б����һ������һ����¼�������ʵ���ڹ����������л������������ʱ�������л�����ҳ�������Դ����϶����顣

@section sec_prgbox_create �ؼ��Ĵ���

�������ͽ������ؼ������¼��Prg��һ�����������Ƕ�������ؼ��У�����Textbox��Editbox��menu��list�ȣ�����Ҫ�û��Լ�����������û���Ҫ����ʹ�øÿؼ�������ʱ��Ϊ��̬�����;�̬�������䴴����ʽ�������ؼ���ͬ��
����֮�󣬸�������������صĲ����Ϳ����ˣ�����ؼ��Ľӿ������˵���٣�Ҳ�Ƚϼ򵥡�

@section sec_prgbox_app �ؼ�Ӧ��
��ͼΪ�������ؼ���ʹ�ã�
@image html prgbox_display.jpg
ͼ 2 42 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup groupGrp PrgBox
@brief groupģ��
@details groupģ��
@{
*******************************************************************************/

#ifndef _CTRLPROGRESSBAR_EXPORT_H_
#define _CTRLPROGRESSBAR_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmitheme_prgbox.h"
#include "guictrl.h"

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
//ownerdraw handle
typedef struct _progressbar_obj_tag* CTRL_PROGRESSBAR_HANDLE_T;

/*!  @typedef GUIPRGBOX_STYLE_E
@brief ��������ʾ��ʽ
*/
typedef enum
{
    GUIPRGBOX_STYLE_VERTICAL_SCROLL,        /*!< ��������� */
    GUIPRGBOX_STYLE_HORIZONTAL_SCROLL,      /*!< ��������� */
    GUIPRGBOX_STYLE_VERTICAL_PROCESS,       /*!< ��������� */
    GUIPRGBOX_STYLE_HORIZONTAL_PROCESS,     /*!< ��������� */
    GUIPRGBOX_STYLE_VOLUME_BAR,             /*!< ���������� */
    GUIPRGBOX_STYLE_VERTICAL_WINDOWS,       /*!< ���� */
    GUIPRGBOX_STYLE_HORIZONTAL_WINDOWS,     /*!< ���� */
#ifdef GUIF_PRGBOX_LETTERSCROLL
    GUIPRGBOX_STYLE_LETTERS_SCROLL,     /*��ĸ������*/
#endif
    GUIPRGBOX_STYLE_MAX                     /*!< �� */
} GUIPRGBOX_STYLE_E;

/*!  @typedef CTRLPROGRESSBAR_RESULT_E
@brief ������handle result
*/
typedef enum
{
    GUIPRGBOX_RESULT_NONE,          /*!< �� */
    GUIPRGBOX_RESULT_HEAD,          /*!< ������ͷ�� */
    GUIPRGBOX_RESULT_TAIL,          /*!< ������β�� */
    GUIPRGBOX_RESULT_PRE_ONE,       /*!< ǰһ�� */
    GUIPRGBOX_RESULT_NEXT_ONE,      /*!< ��һ�� */
    GUIPRGBOX_RESULT_PRE_PAGE,      /*!< ǰһҳ */
    GUIPRGBOX_RESULT_NEXT_PAGE,     /*!< ��һҳ */
    GUIPRGBOX_RESULT_PRE_MOVE,      /*!< �ϸ��ƶ� */
    GUIPRGBOX_RESULT_NEXT_MOVE,     /*!< �¸��ƶ� */
    GUIPRGBOX_RESULT_CHANGE,          //for progress tp press&move
    GUIPRGBOX_RESULT_MAX            /*!< �� */
} GUIPRGBOX_RESULT_E;

/*! @struct CTRLPROGRESSBAR_INIT_DATA_T
@brief ��������ʼ�����ݽṹ
*/
typedef struct
{
    BOOLEAN                 is_forbid_paint;/*!< �Ƿ�ˢ�� */
    uint32                  lcd_id;         /*!< lcd_id */
    GUI_BOTH_RECT_T         both_rect;      /*!< ��������(�������ú����������) */
    GUIPRGBOX_STYLE_E style;          /*!< ������ģʽ */
    MMITHEME_PRGBOX_TYPE_E  theme_type;     /*!< ���������� */
} GUIPRGBOX_INIT_DATA_T;

typedef struct
{
    uint16          item_index;	/*!<��0��ʼ*/
    MMI_NOTIFY_T	notify;			/*!<���ýṹ*/
} CTRLPRGBOX_NOTIFY_T;

#define GUIPRGBOX_LETTER_FONT CAF_FONT_8
#define GUIPRGBOX_SCROLL_SPACE     8
#define VOLUME_BRIGHTNESS_BAR_HEIGHT 64

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ���ݿؼ�id����λ��
@author Jassmine
@param  is_update        [in]   �Ƿ�ˢ��
@param  cur_item_index   [in]   ��ǰitem���
@param  first_item_index [in]   ��һ��item�����
@param  ctrl_id          [in]   �ؼ�id
@return ��
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetPos (
    BOOLEAN          is_update,      //is update
    uint32           cur_item_index,
    uint32			first_item_index,
    MMI_CTRL_ID_T    ctrl_id         //control id
);

/***************************************************************************//*!
@brief ���ݿؼ�idˢ��
@author Jassmine
@param  ctrl_id        [in] �ؼ�id
@return ��
@note
*******************************************************************************/
void CTRLPROGRESSBAR_Update (
    MMI_CTRL_ID_T   ctrl_id //control id
);

/***************************************************************************//*!
@brief ���ݿؼ���Ϣ������������
@author allen
@param  prgbox_ptr          [in]    �������ؼ���������Ϣ
@param  new_rect            [in]    ������Ϣ
@return ���óɹ�����TRUE�����򷵻�FALSE
@note
*******************************************************************************/
BOOLEAN CTRLPROGRESSBAR_ResizeRect (
    MMI_HANDLE_T handle,
    GUI_RECT_T          new_rect  // the new total rectangle
);

/***************************************************************************//*!
@brief ���ݿؼ�handle��������
@author Jassmine
@param  ctrl_handle           [in]  �ؼ�handle
@param  rect_ptr              [in]  �µ�������Ϣ
@return ��
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetRect (
    MMI_HANDLE_T      ctrl_handle,
    GUI_RECT_T        *rect_ptr
);

/***************************************************************************//*!
@brief ���ݿؼ�id����item������ÿҳitem����
@author guanyong.zhang
@param  ctrl_id              [in]   �ؼ�id
@param  total_item_count     [in]   item������
@param  items_per_page       [in]   ÿҳitem����
@return ��
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetParam (
    MMI_CTRL_ID_T      ctrl_id,
    uint32             total_item_count,
    uint32             items_per_page
);

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description : set parameter: top limitation and bottom limitation 
//  Global resource dependence :
//  Author: fulu.song
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetConstraint(
    MMI_CTRL_ID_T  ctrl_id,
    uint32         max,
    uint32         min
);
#endif

/***************************************************************************//*!
@brief ���ݿؼ�������Ϣ�����Ƿ���ѭ��������
@author guanyong.zhang
@param      prgbox_ctrl_ptr     [in]    �ؼ�������Ϣ
@param      is_loop_scroll      [in]    �Ƿ�����ѭ��������
@return ��
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetLoopScroll (
    MMI_HANDLE_T        handle,
    BOOLEAN             is_loop_scroll
);

/***************************************************************************//*!
@brief ���ô�ֱ�������ı���
@author Jassmine
@param  ctrl_id      [in]   �ؼ�id
@param  bg_id        [in]   ����ͼƬid
@return ���ù���������
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetVerGrooveBgImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
);

/***************************************************************************//*!
@brief ����������Ϣ�����Ƿ���Ҫ����
@author xiaoqing.lu
@param      prgbox_ctrl_ptr     [in]    �ؼ�������Ϣ
@param      is_true             [in]    �Ƿ���Ҫ���û���
@return ��������ǰ������
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedSlideBlock (
    MMI_HANDLE_T handle,
    BOOLEAN              is_true
);

/***************************************************************************//*!
@brief ���ݿؼ�������Ϣ�ж��Ƿ���Ҫ���ñ���
@author xiaoqing.lu
@param      prgbox_ctrl_ptr [in]    �ؼ�������Ϣ
@param      is_true         [in]    �Ƿ���Ҫ���ñ���
@return ��������ǰ������
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetNeedBg (
    MMI_HANDLE_T handle,
    BOOLEAN              is_true
);

/***************************************************************************//*!
@brief ���ñ�����ɫ
@author guanyong.zhang
@param      ctrl_id     [in]    �ؼ�id
@param      bg_color    [in]    ������ɫ
@return ��
@note
*******************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBgColor (
    MMI_CTRL_ID_T        ctrl_id,
    GUI_COLOR_T         bg_color
);

/***************************************************************************//*!
@brief ���ù�����bar��ͼƬ
@author Jassmine
@param      ctrl_id [in]    �ؼ�id
@param      bg_id   [in]    ͼƬid
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESSBAR_SetScrollBarImg (
    MMI_CTRL_ID_T    ctrl_id,
    MMI_IMAGE_ID_T   bg_id
);

/***************************************************************************//*!
@brief ��ȡ��ǰ��������λ������Ӧ��ĸ��unicode
@author
@param      prgbox_ctrl_ptr����IN]�������Ŀؼ�ָ��
@param      item_pos���������ĵ�ǰλ��
@return ����item_pos����Ӧ����ĸ��unicode
@note
*******************************************************************************/
PUBLIC wchar  CTRLPROGRESSBAR_GetWcharByPos (
    MMI_HANDLE_T handle,
    uint16  item_pos
);
/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetBothRect (
    MMI_HANDLE_T handle,
    GUI_BOTH_RECT_T     *both_rect_ptr
);

/*****************************************************************************/
//  Description : �����Ƿ���Ҫ����
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:��������ǰ������
/*****************************************************************************/
PUBLIC void CTRLPROGRESSBAR_SetFobidThumb (
    MMI_CTRL_ID_T ctrl_id
);

PUBLIC uint32 CTRLPROGRESSBAR_GetTotalItemNum (MMI_HANDLE_T handle);
PUBLIC uint32 CTRLPROGRESSBAR_GetFirstItemIndex (MMI_HANDLE_T handle);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLPROGRESSBAR_EXPORT_H_



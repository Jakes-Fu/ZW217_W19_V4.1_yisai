/*****************************************************************************
** File Name:      ctrlbaseedit_export.h                                        *
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

#ifndef _CTRLBASEEDIT_EXPORT_H_
#define _CTRLBASEEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guiim.h"
#include "ctrlbase_export.h"

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

/*! @enum GUIEDIT_TYPE_E
@brief �༭������
*/
typedef enum
{
    GUIEDIT_TYPE_NULL,      /*!<����λ*/
    GUIEDIT_TYPE_TEXT,      /*!<�ı��༭��*/
    GUIEDIT_TYPE_PHONENUM,  /*!<�绰����༭��,[0-9,*,#]*/
    GUIEDIT_TYPE_DIGITAL,   /*!<���ֱ༭��,[0-9]*/
    GUIEDIT_TYPE_PASSWORD,  /*!<����༭��,[0-9,*,#]*/
    GUIEDIT_TYPE_LIST,      /*!<�б�༭��list[�绰�����б�,E-mail�б�]*/
    GUIEDIT_TYPE_DATE,      /*!<���ڱ༭��*/
    GUIEDIT_TYPE_TIME,      /*!<ʱ��༭��*/
    GUIEDIT_TYPE_IP,        /*!<IP�༭��*/
    GUIEDIT_TYPE_TOUCH_DATE,/*!<ȫ����������*/
    GUIEDIT_TYPE_TOUCH_TIME,/*!<ȫ������ʱ��*/
    GUIEDIT_TYPE_PICKER,    /*!<����ѡȡ*/
    GUIEDIT_TYPE_MAX        /*!<����λ*/
} GUIEDIT_TYPE_E;

/*! @enum GUIEDIT_STYLE_E
@brief �༭����
*/
typedef enum
{
    GUIEDIT_STYLE_NULL,     /*!<����λ*/
    GUIEDIT_STYLE_SINGLE,   /*!<����,û�й�����*/
    GUIEDIT_STYLE_MULTI,    /*!<����,�й�����*/
    GUIEDIT_STYLE_ADAPTIVE, /*!<����,û�й�����,��Ƕ����Ͽؼ���*/
    GUIEDIT_STYLE_MAX       /*!<����λ*/
} GUIEDIT_STYLE_E;

/*! @enum GUIEDIT_DISPLAY_DIR_E
@brief �༭����ʾ����
*/
typedef enum
{
    GUIEDIT_DISPLAY_DIR_LT, /*!<�����Ͽ�ʼ��ʾ*/
    GUIEDIT_DISPLAY_DIR_RB, /*!<�����¿�ʼ��ʾ*/
    GUIEDIT_DISPLAY_DIR_MAX /*!<����λ*/
} GUIEDIT_DISPLAY_DIR_E;

/*! @enum GUIEDIT_SEPARATOR_E
@brief �༭��ָ�������
*/
typedef enum
{
    GUIEDIT_SEPARATOR_NULL,         /*!<����λ*/
    GUIEDIT_SEPARATOR_DOT,          /*!<'.'*/
    GUIEDIT_SEPARATOR_LINE,         /*!<'-'*/
    GUIEDIT_SEPARATOR_COMMA,        /*!<','*/
    GUIEDIT_SEPARATOR_BLANK,        /*!<' '*/
    GUIEDIT_SEPARATOR_COLON,        /*!<':'*/
    GUIEDIT_SEPARATOR_SOLIDUS,      /*!<'/'*/
    GUIEDIT_SEPARATOR_SEMICOLON,    /*!<';'*/
    GUIEDIT_SEPARATOR_MAX           /*!<����λ*/
} GUIEDIT_SEPARATOR_E;

/*! @struct GUIEDIT_INIT_LIST_T
@brief �б�༭���ʼ������
*/
typedef struct
{
    uint16      item_num;       /*!<item����*/
    uint16      item_max_len;   /*!<ÿ��item������ַ�����*/
} GUIEDIT_INIT_LIST_T;

/*! @union GUIEDIT_INIT_ADD_U
@brief �༭���ʼ����������
*/
typedef union
{
    GUIEDIT_INIT_LIST_T     list;   /*!<�б��ʼ������*/
} GUIEDIT_INIT_ADD_U;

/*! @struct GUIEDIT_INIT_DATA_T
@brief �༭���ʼ������
*/
typedef struct
{
    BOOLEAN                 is_rect_adaptive;   /*!<�����Ƿ�����ַ����Զ�����*/
    uint16                  str_max_len;        /*!<���������ַ�����*/
    GUI_BOTH_RECT_T         both_rect;          /*!<��������*/
    GUIEDIT_TYPE_E          type;               /*!<�༭������*/
    GUIEDIT_INIT_ADD_U      add_data;           /*!<��ʼ����������*/
} GUIEDIT_INIT_DATA_T;

/*! @struct GUIEDIT_HIGHLIGHT_T
@brief �༭�������Ϣ
*/
typedef struct
{
    BOOLEAN     is_underline;   /*!<TRUE:im�ύ��ȷ���ַ�,FALSE:����/ճ����ȷ���ַ�*/
    uint16      start_pos;      /*!<������ʼλ��*/
    uint16      end_pos;        /*!<��������λ��*/
} GUIEDIT_HIGHLIGHT_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit type
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLBASEEDIT_GetType (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetDispImIcon (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_im_icon        //in:
);

/*****************************************************************************/
// 	Description : set edit is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_PermitBorder (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
);

/*****************************************************************************/
//  Description : set edit bg is transparent display image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetBgTransparent (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
);

/*****************************************************************************/
//  Description : set edit margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetMargin (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_T          *font_size_ptr,
    GUI_COLOR_T         *font_color_ptr
);

/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for single line
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
);

/*****************************************************************************/
//  Description : set edit rect is adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetRectAdaptive(
                                         MMI_CTRL_ID_T  ctrl_id,//in
                                         BOOLEAN        is_true //in:
                                         );

/*****************************************************************************/
//  Description : set edit im
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�������е����뷨,��ʼ���뷨����������
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetIm (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:��������뼯��
    GUIIM_TYPE_T      init_im     //in:��ʼ���뷨
);

///*****************************************************************************/
////  Description : set edit im capital mode
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImCapital (
//    MMI_CTRL_ID_T			ctrl_id,	//in
//    GUIIM_CAPITAL_MODE_T	capital_mode//in:��Сдģʽ
//);

///*****************************************************************************/
////  Description : set edit im input type
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImInput (
//    MMI_CTRL_ID_T		ctrl_id,		//in
//    GUIIM_INPUT_TYPE_T	init_input_type,//in:��ʼ���뷨����
//    GUIIM_INPUT_TYPE_T	allow_input_type//in:������������ͼ���
//);

/*****************************************************************************/
//  Description : set edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�Ƿ�����TP IM
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTp (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is enable hand writing
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:�Ƿ�����IM hand writing
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImHw (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is forbid symbo;
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImSymbol (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_forbid
);

/*****************************************************************************/
//  Description : set edit im tag
//  Global resource dependence :
//  Author: Jassmine
//  Note: ������email�����ʱ��һЩ���뷨�ᶨ��һЩ����
//      �����ڼ���edit�ؼ�֮ǰ���ã���Ϊ�����뷨����֮�����ã����ؽ����뷨
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTag (
    MMI_CTRL_ID_T  ctrl_id,
    GUIIM_TAG_T    tag
);

/*****************************************************************************/
//  Description : config edit im custom key
//  Global resource dependence :
//  Author: Jassmine
//  Note: must < 9 ,only config digital or multi-tap im mode
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_ConfigImCustomKey (
    MMI_CTRL_ID_T         ctrl_id,    //in:
    GUIIM_TYPE_T          im_type,    //in:only digital or multi-tap
    GUIIM_CUSTOM_KEY_T    custom_key, //in:
    wchar                 *value_ptr, //in:
    uint32                value_count //in:
);

/*****************************************************************************/
//  Description: check whether im is open
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImOpen (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description: judge whether application can set softkey or not
//  Author: arvin.zhang
//  Note:app cann't modify softkey if im is working on several state
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImNeedSoftkey (
	MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : when edit get active, is create im
//  Global resource dependence :
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_CreateImEditActive (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    BOOLEAN           is_create   //in:
);

/*****************************************************************************/
//  Description: judge whether the TP panel is visible
//  Author: wancan.you
//  Param
//       ctrl_id :control id
//  Return:
//       TRUE:visible;FALSE:invisible
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsTPPanelVisible (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : get im handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBASEEDIT_GetIMHandle(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

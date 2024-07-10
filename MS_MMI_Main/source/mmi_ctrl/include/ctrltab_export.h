/*****************************************************************************
** File Name:      ctrltab_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLTAB_EXPORT_H_
#define _CTRLTAB_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmk_app.h"
#include "guifont.h"
#include "guicommon.h"
#include "guictrl.h"
#include "guianim.h"
#include "guiprgbox.h"
#include "mmitheme_tab.h"

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
// default max item name length
/*! @def GUITAB_MAX_ITEM_NAME_LENGTH
@brief Item��ʾ���ֵ���󳤶�
*/
#define GUITAB_MAX_ITEM_NAME_LENGTH     32
/*! @def GUITAB_MAX_ITEM_NUM
@brief TAB Item �������
*/
#define GUITAB_MAX_ITEM_NUM             20

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUITAB_TIMER_STATE_T
@brief ����״̬��ϵͳδʹ��
*/
typedef uint32 GUITAB_TIMER_STATE_T;
// timer status
/*! @def GUITAB_TIMER_NULL
@brief ����״̬��ϵͳδʹ��
*/
#define GUITAB_TIMER_NULL					0X0000
/*! @def GUITAB_TIMER_LEFT_SCROLL
@brief ����״̬��ϵͳδʹ��
*/
#define GUITAB_TIMER_LEFT_SCROLL			0X0001
/*! @def GUITAB_TIMER_RIGHT_SCROLL
@brief ����״̬��ϵͳδʹ��
*/
#define GUITAB_TIMER_RIGHT_SCROLL			0X0002

/*! @typedef GUITAB_ITEM_STATE_T
@brief Item State
*/
typedef uint32 GUITAB_ITEM_STATE_T;
/*! @def GUITAB_ITEMSTATE_NULL
@brief null ״̬
*/
#define GUITAB_ITEMSTATE_NULL           0x00000000
/*! @def GUITAB_ITEMSTATE_NEED_CONTENT
@brief ��Ҫ���ݣ���̬����itemʹ��
*/
#define GUITAB_ITEMSTATE_NEED_CONTENT   0x00000001  // need item content
/*! @def GUITAB_ITEMSTATE_SHOW_IMG
@brief itemֻ��ʾ��ʾͼƬ
*/
#define GUITAB_ITEMSTATE_SHOW_IMG       0x00000002  // only show image in item rectangle
/*! @def GUITAB_ITEMSTATE_SHOW_TEXT
@brief itemֻ��ʾ�ı�
*/
#define GUITAB_ITEMSTATE_SHOW_TEXT      0x00000004  // only show text in item rectangle
/*! @def GUITAB_ITEMSTATE_INVALID
@brief ��Ч��item��������Чʱ��item����ʾ
*/
#define GUITAB_ITEMSTATE_INVALID        0x00000008  // is invalid item
/*! @def GUITAB_ITEMSTATE_UIMG_DTXT
@brief item��ʾ��ͼ���ģ�Ĭ����ͼ����
*/
#define GUITAB_ITEMSTATE_UIMG_DTEXT		0x00000010  // item��ʾ��ͼ���ģ�Ĭ����ͼ����

// define state
/*! @typedef GUITAB_STATE_T
@brief TAB�ؼ�״̬
*/
typedef uint32 GUITAB_STATE_T;

/*! @def GUITAB_STATE_NULL
@brief null״̬
*/
#define GUITAB_STATE_NULL               0x00000000  // no state
// text, item, tab status
/*! @def GUITAB_STATE_DISABLE
@brief ϵͳ����
*/
#define GUITAB_STATE_DISABLE            0x00000001  // disable or not
// item, tab status
/*! @def GUITAB_STATE_NONEED_SCROLL
@brief tab����ʾ������ͷ��Ĭ��Ϊ��ʾ
*/
#define GUITAB_STATE_NONEED_SCROLL      0x00000010  // need scroll card or not

/*! @def GUITAB_STATE_SETTING_ITEM_DATA
@brief ����DATA��ϵͳ����
*/
#define GUITAB_STATE_SETTING_ITEM_DATA  0x00000100  // is setting item content, ���� add_data_ptr ����,��¼��������.
/*! @def GUITAB_STATE_ACTIVE
@brief ϵͳ����
*/
#define GUITAB_STATE_ACTIVE             0x00000200  // active or not
/*! @def GUITAB_STATE_FOCUS
@brief ϵͳ����
*/
#define GUITAB_STATE_FOCUS              0x00000400  // on focus or not
/*! @def GUITAB_STATE_INVALID
@brief ϵͳ����
*/
#define GUITAB_STATE_INVALID            0x00000800  // invalid or not
/*! @def GUITAB_STATE_AUTO_SWITCH
@brief �Զ��л�item��Ŀǰ��ϵͳ����״̬
*/
#define GUITAB_STATE_AUTO_SWITCH        0x00001000  // automatically switch to next item
/*! @def GUITAB_STATE_SINGLE_LINE
@brief ������ʾ��ֻ��ʾitem������ʾtitle��Ĭ��ʱ����ʾtitle
*/
#define GUITAB_STATE_SINGLE_LINE        0x00002000  // display on single line, |item image|_focus item text_|item image|
/*! @def GUITAB_STATE_SYNC_DECODE_ANIM
@brief �Ƿ�ͬ�����룬Ĭ��ʱͬ��
*/
#define GUITAB_STATE_SYNC_DECODE_ANIM   0x00004000  // tab�ϵ�Item�����Ƿ���Ҫͬ����ʾ�����룩��Ĭ���������ͬ����
/*! @def GUITAB_STATE_COMMON_TITLE
@brief tab�Ƿ���ʾͳһ��title����tab title�Ƿ���item���л����ı�
*/
#define GUITAB_STATE_COMMON_TITLE       0x00008000  // TAB��ʾͳһ��TITLE

/*! @def GUITAB_STATE_IS_PAINT
@brief ��tab�ؼ����յ�full paint֮�󣬻��Զ����ø�״̬�����ұ��ֵ��ؼ�������
*/
#define GUITAB_STATE_IS_PAINT           0x00010000  // TAB�Ѿ�ˢ�¹�

/*! @def GUITAB_STATE_IS_ADAPT_WIDTH
@brief ����Ӧ���򣬵�item�ܿ��С��tab���ʱ�Ż�����Ӧ
*/
#define GUITAB_STATE_IS_ADAPT_WIDTH     0x00020000  // ����ӦITEM�����������TAB����

/*! @def GUITAB_STATE_SWITCH_ITEM
@brief ���Ҽ��Ƿ��л�tabҳ����tp���ʱ����״̬�Զ�����ΪTRUE,Ĭ��ֵΪTRUE
*/
#define GUITAB_STATE_SWITCH_ITEM		0x00040000  // ���Ҽ��Ƿ��л�tabҳ


#define GUITAB_STATE_MAX                0x08000000  // TAB״̬�����ֵ
// 0x10000000֮���״̬��Ҫ����

// ͼƬ����ʾ״̬
/*! @typedef GUITAB_IMAGE_STATE_T
@brief ͼƬ����ʾ״̬��ϵͳ����
*/
typedef uint32 GUITAB_IMAGE_STATE_T;
/*! @def GUITAB_IMAGE_NULL
@brief null״̬����ʾitem�е�ͼƬ��û�����ã�ϵͳ����
*/
#define GUITAB_IMAGE_NULL               0x00000000  // image is null, ��ʾitem�е�ͼƬ��û������
/*! @def GUITAB_IMAGE_FILLED
@brief ��ʾitem�е�ͼƬ�����Ѿ������ˣ�ϵͳ����
*/
#define GUITAB_IMAGE_FILLED             0x00000001  // image is filled, ��ʾitem�е�ͼƬ�����Ѿ�������
/*! @def GUITAB_IMAGE_INVALID
@brief ͼƬ������Ч��ϵͳ����
*/
#define GUITAB_IMAGE_INVALID            0x00000002  // image info is invalid
/*! @def GUITAB_IMAGE_NEED_SEL_BG_IMG
@brief �Ƿ���Ҫ��ʾѡ��ʱ�ı���ͼƬ
*/
#define GUITAB_IMAGE_NEED_SEL_BG_IMG     0x00000004	//need select bg image


/*! @enum  GUITAB_SCROLL_STATE_E 
@brief  ���ҹ�������״̬
*/
typedef enum
{
    GUITAB_SCROLL_CARD_STATE_NONE, /*!< ��*/
    GUITAB_SCROLL_CARD_PRESSED,    /*!< ����*/
    GUITAB_SCROLL_CARD_RELEASED,   /*!< ����*/
    GUITAB_SCROLL_CARD_DISABLED,   /*!< ��Ч*/
    GUITAB_SCROLL_CARD_STATE_MAX   /*!< ϵͳ����*/
}GUITAB_SCROLL_STATE_E;

/*! @typedef GUITAB_SCROLL_STATE_T
@brief ���ҹ�������״̬
*/
typedef GUITAB_SCROLL_STATE_E GUITAB_SCROLL_STATE_T;

/*! @enum  GUITAB_SWITCH_TYPE_E 
@brief  �л�item���ͣ���ǰ�л���������л�
*/
typedef enum
{
    GUITAB_SWITCH_NONE,		    /*!< none*/
    GUITAB_SWITCH_PRE,		    /*!< ǰһ��*/
    GUITAB_SWITCH_NEXT,		    /*!< ��һ��*/
    GUITAB_SWITCH_MAX			/*!< ϵͳ����*/
}GUITAB_SWITCH_TYPE_E;

/*! @enum  GUI_TAB_STYLE_E 
@brief ��ʾ���ϵͳ������Ŀǰֻ֧�ֿ�����ʾ
*/
typedef enum
{
    GUI_TAB_ITEM_NONE,                          /*!< ��*/
    GUI_TAB_ITEM_LEFT, 		                    /*!< ����Ĭ��*/
    GUI_TAB_ITEM_RIGHT = GUI_TAB_ITEM_LEFT, 	/*!< ���ң�δʵ��*/
    GUI_TAB_ITEM_MIDDLE = GUI_TAB_ITEM_LEFT,	/*!< ���У�δʵ��*/
    GUI_TAB_ITEM_MAX			                /*!< ϵͳ����*/
} GUI_TAB_STYLE_E;

/*! @enum  GUITAB_ITEM_DATA_TYPE_E 
@brief  item����������
*/
typedef enum
{
    TABITEM_DATA_NONE,				/*!< none*/
    TABITEM_DATA_TEXT_ID,			/*!< �ı���ԴID*/
	TABITEM_DATA_TEXT_BUFFER,		/*!< �ı�buffer*/
    TABITEM_DATA_IMAGE_ID,		    /*!< ͼƬ��ԴID*/
    TABITEM_DATA_ANIM_ID,			/*!< ������ԴID*/
    TABITEM_DATA_ANIM_DATA,		    /*!< ��������*/
    TABITEM_DATA_ANIM_PATH,		    /*!< ����·��*/
	TABITEM_DATA_MAX,				/*!< ϵͳ����*/
}GUITAB_ITEM_DATA_TYPE_E;

/*! @struct  GUITAB_INIT_DATA_T 
@brief  ��ʼ������
*/
typedef struct 
{
    GUI_BOTH_RECT_T              both_rect;			    /*!< ����������*/
    GUITAB_STATE_T               state;					/*!< tab״̬*/
    GUI_TAB_STYLE_E              style;	                /*!< ���䷽ʽ */
    uint32                       max_item_num;	        /*!< ������������ֵΪ ��\link #GUITAB_MAX_ITEM_NUM \endlink*/
    BOOLEAN                      is_active_item;        /*!< ��*/    // to load item actively

}GUITAB_INIT_DATA_T;

/*! @struct  GUITAB_NEED_ITEM_DATA_T 
@brief   �ؼ��ʹ���֮�佻������ʱʹ�õ�item���ݽṹ������̬����item���� 
*/
typedef struct
{
    MMI_HANDLE_T                handle;					/*!< tab�ؼ���handle*/
    BOOLEAN                     is_child;				/*!< �Ƿ�֪ͨ���ӣ�ϵͳ������3D��Чʱʹ��*/
    BOOLEAN                     is_to_left;				/*!< �л�item�ķ���ϵͳ������3D��Чʹ��*/
    GUI_POINT_T                 point;					/*!< item��������ʼ��*/
    uint32                      item_index;				/*!< ��ǰitem������*/
    uint32                      pre_item_index;		    /*!< �л�����ǰitemʱ��ǰһ��item������*/
}GUITAB_NEED_ITEM_DATA_T;

/*! @struct  GUITAB_ANIM_DISP_RESULT_T 
@brief   ���ͽ����Ľ����app
*/
typedef struct
{
    GUIANIM_RESULT_E            result;                 /*!< ����������*/
    uint32                      item_index;             /*!< item����*/
}GUITAB_ANIM_DISP_RESULT_T;


/*! @struct GUITAB_TEXT_STYLE_T
@brief  text style
*/
typedef struct
{
    GUI_RECT_T				    rect;                   /*!< ��ʾ����*/
    uint8					    font_space; 		    /*!< �ּ��*/
	GUI_FONT_T				    font_type;              /*!< �ֺ�*/
	GUI_COLOR_T				    font_color;             /*!< ������ɫ*/
    GUI_ALIGN_E                 align;                  /*!< �Ű淽ʽ*/
} GUITAB_TEXT_STYLE_T;

/*! @struct GUITAB_TEXT_T
@brief text buffer information
*/
typedef struct
{
    uint16					    wstr_len;	                            /*!< �ı�����*/
	wchar					    wstr[GUITAB_MAX_ITEM_NAME_LENGTH+1];    /*!< �ı�buffer*/
}GUITAB_TEXT_T;

/*! @union GUITAB_TEXT_DATA_U
@brief text data union, text id or text buffer 
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;                /*!< text id*/
    GUITAB_TEXT_T               text_buffer;            /*!< text buffer*/
}GUITAB_TEXT_DATA_U;

/*! @struct GUITAB_CAPTION_T
@brief tab caption struct
*/
typedef struct
{
    BOOLEAN                     is_disp_font_bg;        /*!< �Ƿ���ʾ����*/
    MMI_IMAGE_ID_T			    pressed_image_id; 		/*!< ���������µ�ͼƬID*/
	MMI_IMAGE_ID_T			    released_image_id; 			/*!< �������ͷŵ�ͼƬID*/

    GUITAB_ITEM_DATA_TYPE_E     text_type; 				/*!< ��������*/
    GUITAB_TEXT_DATA_U          text;      				/*!< ������Ϣ*/
    GUITAB_TEXT_STYLE_T         text_style;				/*!< ���ַ��*/
} GUITAB_CAPTION_T;

/*! @union GUITAB_IMAGE_DATA_U
@brief   image information union: id, data info, file info
*/
typedef union
{
    GUIANIM_DATA_INFO_T         anim_data;              /*!< ��������*/
    GUIANIM_FILE_INFO_T         anim_path;              /*!< ����·��*/
}GUITAB_IMAGE_DATA_U;

/*! @struct GUITAB_IMAGE_DATA_T
@brief image info data
*/
typedef struct
{
    int16						comp_image_w;           /*!< ͼƬ��ʾ��ѹ�����*/
	int16						comp_image_h;           /*!< ͼƬ��ʾ��ѹ���߶�*/
    GUITAB_IMAGE_STATE_T        state;                  /*!< ��item����ʱ���Ƿ���ʾ����ͼƬ*/
    GUITAB_ITEM_DATA_TYPE_E     image_data_type;        /*!< id, anim data, anim path*/
    GUITAB_IMAGE_DATA_U         image_data_u;           /*!< ������Ϣ*/
}GUITAB_IMAGE_DATA_T;

/*! @struct  GUITAB_ITEM_CONTENT_T 
@brief   item content
*/
typedef struct
{
    GUITAB_CAPTION_T            text_data;              /*!< caption info*/
    MMI_IMAGE_ID_T			    sel_image_id;           /*!< ѡ��״̬��ʱ��ͼƬ*/
	MMI_IMAGE_ID_T			    bg_image_id;            /*!< �����item���еı���ͼ*/
    GUITAB_IMAGE_DATA_T         image_data;             /*!< icon card image*/
}GUITAB_ITEM_CONTENT_T;

/*! @struct _guitab_item_tag
@brief   tab item data
*/
typedef struct _guitab_item_tag
{
    GUITAB_ITEM_CONTENT_T*       item_data_ptr;         /*!< item��������*/
    uint32                       item_width;		    /*!< item��ȣ��ؼ��Լ�����*/
    GUI_RECT_T                   rect;		            /*!< item���򣬿ؼ��Լ�����*/
    GUITAB_ITEM_STATE_T          item_state;	        /*!< item״̬*/
    ADD_DATA                     add_data_ptr;          /*!< �ɼ�¼anim�ؼ����ݣ��������ô�*/
    MMI_HANDLE_T			     child_win_handle;      /*!< tab item��Ӧ���Ӵ���ID*/

    struct _guitab_item_tag      *next_ptr;             /*!< ����ָ��*/
}GUITAB_ITEM_T;

/*! @struct GUITAB_ITEM_LIST_T
@brief  item list
*/
typedef struct  
{
    uint32			            item_num;		        /*!< item��*/
	uint32			            total_width;	        /*!< ����item���*/
    GUITAB_ITEM_T	            *item_ptr;		        /*!< itemָ��*/
}GUITAB_ITEM_LIST_T;

/*! @struct GUITAB_SCROLL_CARD_T
@brief  scroll  card data 
*/
typedef struct 
{
   	GUI_RECT_T				    card_rect;              /*!< show rectangle of scroll card*/
    GUITAB_SCROLL_STATE_T       card_state;             /*!< card state*/
	MMI_IMAGE_ID_T			    pressed_image_id;       /*!< scroll card pressed image */
	MMI_IMAGE_ID_T			    released_image_id;      /*!< scroll card released image*/
	MMI_IMAGE_ID_T			    disabled_image_id;      /*!< scroll card disabled image*/
}GUITAB_SCROLL_CARD_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief �������Item��,��������ڴ�,�����Ŀ���ܳ���GUITAB_MAX_ITEM_NUM = 20
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param max_item_num [in] ���tab����
@retval true/false
@return ���سɹ���ʧ��
@note ���������Ҫ���������ڴ棬������������Ҫ����20�����ʵ����Ҫ�������ô��ͬʱ�޸�GUITAB_MAX_ITEM_NUM�Ķ��壬�������ڴ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetMaxItemNum(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 max_item_num
                                    );

/***************************************************************************//*!
@brief ����Tab���Ӧ���ִ���ID
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param child_win_handle [in] TAB���Ӵ��ھ����ID
@param pos [in] TAB��������
@retval true/false
@return ���سɹ���ʧ��
@note ÿ��tab�����л�ʱ����������ʾ�Լ����Ӵ��ڣ���Ȼ��Щ�Ӵ��ڿ�����ͬһ������
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AddChildWindow(
                                     MMI_CTRL_ID_T     ctrl_id, 
                                     MMI_HANDLE_T      child_win_handle,
                                     uint32            pos
                                     );

/***************************************************************************//*!
@brief �򵥵�Append�ӿ�,ֻ�����ַ���������ͼƬ��ID,Item���������ݲ��ᱻ�滻
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param text_ptr [in] �ı���Ϣ
@param sel_image_id [in] ѡ��ʱ��ʾ��ͼƬ
@param unsel_image_id [in] δѡ��ʱ��ʾ��ͼƬ
@retval true/false
@return ���سɹ���ʧ��
@note �ú������ڼ򵥵ġ�ֻ��ʾ��ԴͼƬѡ��Ŀؼ�����TAB��ʱʹ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendSimpleItem(
                                       MMI_CTRL_ID_T     ctrl_id, 
                                       MMI_STRING_T *    text_ptr,
                                       MMI_IMAGE_ID_T    sel_image_id,
                                       MMI_IMAGE_ID_T    unsel_image_id
                                       );

/***************************************************************************//*!
@brief ��pos��Ӧ��λ�ò���һ��
@author Great.Tian
@param ctrl_id [in] �ؼ�ID
@param item_ptr [in] TAB������
@param pos [in] TAB��������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_InsertItem(
                                  MMI_CTRL_ID_T            ctrl_id, // control id
                                  const GUITAB_ITEM_T      *item_ptr,   // ָ����Item
                                  uint32                    pos     // ָ����λ��, from 0 to total_item_num
                                  );

/***************************************************************************//*!
@brief ɾ��һ��Item��
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param pos [in] TAB��������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteItem(
                                 MMI_CTRL_ID_T ctrl_id, 
                                 uint32 pos
                                 );

/***************************************************************************//*!
@brief ɾ�����е�Item��
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@retval false/ture
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_DeleteAllItems(
                                     MMI_CTRL_ID_T ctrl_id
                                     );

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@retval ����
@return ����
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetItemNum(
                                MMI_CTRL_ID_T ctrl_id
                                );

/***************************************************************************//*!
@brief �޸�pos��Ӧ��Item��Ϣ
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param text_ptr [in] �ַ���ָ��
@param sel_image_id [in] ѡ��ʱͼƬ��ԴID
@param unsel_image_id [in] δѡ��ʱͼƬ��ԴID
@param pos tab[in] ������
@retval true/false
@return �ɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSimpleItem( 
                                    MMI_CTRL_ID_T ctrl_id,
                                    const MMI_STRING_T * text_ptr,
                                    MMI_IMAGE_ID_T sel_image_id,
                                    MMI_IMAGE_ID_T unsel_image_id,
                                    uint32 pos
                                    );

/***************************************************************************//*!
@brief ���õ�ǰ���������Ϊitem_index
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param item_index [in] tab������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetCurSel(
                                MMI_CTRL_ID_T ctrl_id, 
                                uint32 item_index
                                );

/***************************************************************************//*!
@brief  ��õ�ǰ���������
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@retval ��ǰ������ֵ
@return ���ص�ǰ������ֵ
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetCurSel(
                               MMI_CTRL_ID_T ctrl_id
                               );

/*****************************************************************************/
//  Description : get current selected item.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC GUITAB_ITEM_T  * CTRLTAB_GetCurItem(MMI_CTRL_ID_T ctrl_id);

/***************************************************************************//*!
@brief ���ñ���ͼƬ
@author  guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ����ͼƬ��ԴID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundImage(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/***************************************************************************//*!
@brief ���ñ�����ɫ,ע�ⱳ����ɫ�ͱ���ͼƬͬʱ����ʱ����ͼƬ������ʾ
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param bg_color [in] ����ɫ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetBackGroundColor(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         GUI_COLOR_T bg_color
                                         );

/***************************************************************************//*!
@brief �����ı��ķ��:����\��ɫ\�ּ���
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param font [in] �����С
@param font_color [in] ������ɫ
@param font_space [in] �ּ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFontParam( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   GUI_FONT_T font,
                                   GUI_COLOR_T font_color,
                                   uint8 font_space
                                   );

/***************************************************************************//*!
@brief ���ø�����ʾ��Ϣ
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param added_string [in] �����ַ�����Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAddedString( 
                                     MMI_CTRL_ID_T     ctrl_id,
                                     GUITAB_CAPTION_T  added_string
                                     );

/***************************************************************************//*!
@brief  ��ȡ������ʾ��������Ϣ
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@param added_string_ptr [in/out] �����ַ�����Ϣ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAddedString( 
                                     MMI_CTRL_ID_T               ctrl_id,
                                     GUITAB_CAPTION_T *    added_string_ptr
                                     );

/***************************************************************************//*!
@brief  ���ÿؼ�״̬
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�ID
@param src_state [in] ״̬
@param is_true [in] ���û�ȡ��
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetState( 
                            MMI_CTRL_ID_T ctrl_id,
                            GUITAB_STATE_T src_state,
                            BOOLEAN is_true
                            );

/***************************************************************************//*!
@brief ��ÿؼ���״̬�Ƿ�����
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�ID
@param src_state [in]  ״̬
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetState(
                               MMI_CTRL_ID_T ctrl_id,
                               GUITAB_STATE_T src_state
                               );

/***************************************************************************//*!
@brief ���ÿؼ���Item���״̬
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in]  ������
@param src_state [in]  ״̬
@param is_true [in] ���û�ȡ��
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetItemState( 
                                MMI_CTRL_ID_T ctrl_id,
                                uint32 item_index,
                                GUITAB_ITEM_STATE_T  src_state,
                                BOOLEAN is_true
                                );

/***************************************************************************//*!
@brief ���Item��ĳ��״̬�Ƿ�����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in]  ������
@param src_state [in]  ״̬
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetItemState(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_index,
                                   GUITAB_ITEM_STATE_T src_state
                                   );

/***************************************************************************//*!
@brief ����ͼƬ����ʾ״̬
@author xiaoqing.lu
@param image_data_ptr [in] ͼƬ��Ϣָ��
@param src_state [in] ״̬
@param  is_true [in]���û�ȡ������
@note 
*******************************************************************************/
PUBLIC void CTRLTAB_SetImageState( 
                                 GUITAB_IMAGE_DATA_T* image_data_ptr,
                                 GUITAB_IMAGE_STATE_T  src_state,
                                 BOOLEAN is_true
                                 );

/***************************************************************************//*!
@brief ���ͼƬ����ʾ״̬
@author xiaoqing.lu
@param image_data_ptr [in] ͼƬ��Ϣָ��
@param src_state [in]  ״̬
@retval true/false
@return �����Ƿ�״̬�Ƿ�����
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetImageState(
                                    const GUITAB_IMAGE_DATA_T* image_data_ptr,
                                    GUITAB_IMAGE_STATE_T src_state
                                    );

/***************************************************************************//*!
@brief ����Item������
@author yelianna
@param ctrl_id [in] �ؼ�ID
@param item_content_ptr [in] item ��������
@param  item_index [in]λ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemContent(
                                     MMI_CTRL_ID_T                ctrl_id, // control id
                                     const GUITAB_ITEM_CONTENT_T *item_content_ptr,
                                     uint32                      item_index
                                     );

/***************************************************************************//*!
@brief ����ͼ��������Ϣ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param img_ptr [in] ͼƬ����
@param start_point [in] ��ʼλ��
@param index [in] ������
@retval true/false
@return ���سɹ���ʧ��
@note ���÷�ѡ��״̬�µ�ͼ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemImage(
                                  MMI_CTRL_ID_T ctrl_id,   // �ؼ�ID
                                  GUITAB_IMAGE_DATA_T *img_ptr,  // ��������
                                  GUI_POINT_T start_point,
                                  uint32 index             // ������
                                  );

/***************************************************************************//*!
@brief  �����ı�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_ptr [in] ͼƬ����
@param index [in] ��ʼλ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemText(
                                  MMI_CTRL_ID_T ctrl_id, 
                                  GUITAB_TEXT_T *text_ptr, 
                                  uint32 index
                                  );

/***************************************************************************//*!
@brief �����Ƿ��Զ��л�, ���is_trueΪ1,���Զ��л��ļ��Ϊtimer
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param timer [in] ʱ��������ʱ����
@param is_true [in] �Ƿ������Զ��л�
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetAutoSwitch(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 timer, 
                                    BOOLEAN is_true
                                    );

/***************************************************************************//*!
@brief ����Ƿ��Զ��л�Item,����TRUE,��timer�������Զ��л��ļ��
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param timer [in/out] ʱ��������ʱ����
@retval true/false
@return �Ƿ��л�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_GetAutoSwitch(
                                    MMI_CTRL_ID_T ctrl_id,
                                    uint32 *timer
                                    );

/***************************************************************************//*!
@brief �л�����һ��Item
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param type [in] �л�����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SwitchToNextItem(
                                       MMI_CTRL_ID_T ctrl_id,
                                       GUITAB_SWITCH_TYPE_E type
                                       );

/***************************************************************************//*!
@brief Append һ��Item,��GUITAB_AppendSimpleItem��������Ǹýӿڻ��滻ԭ��Item����������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_ptr [in] item����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_AppendItem(
                                 MMI_CTRL_ID_T         ctrl_id,
                                 const GUITAB_ITEM_T   *item_ptr
                                 );

/***************************************************************************//*!
@brief set space between two items
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_space [in] ���
@param is_use_default [in] �Ƿ�ʹ��Ĭ��ֵ
@retval true/false
@return ���سɹ���ʧ��
@note is_use_default: �Ƿ�ʹ��ȱʡֵ��TRUE:ʹ��GUITAB_DEFAULT_ITEM_SPACE = 5��
                  FALSE:����ֵΪMIN(item_space, GUITAB_DEFAULT_ITEM_SPACE)
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemSpace(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_space,
                                   BOOLEAN is_use_default
                                   );

/***************************************************************************//*!
@brief ����TAB��Ŀ��,��������ý�ȡȱʡֵ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param width [in] ͼƬ��ԴID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemWidth(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 width
                                   );

/***************************************************************************//*!
@brief ����tab�ؼ��Ƿ�ѭ����ʾ����������֮ǰ��״̬
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param is_rotative [in] �Ƿ�ѭ���л�
@retval true/false
@return ���سɹ���ʧ��
@note ѭ����ʾ˵�����Դ������һ��Item�͵�һ��Item֮��ֱ���л�.
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTabRotative(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     BOOLEAN is_rotative
                                     );

/***************************************************************************//*!
@brief ����Tab�ؼ���Ĭ��ͼƬID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param def_img_id [in] Ĭ����ʾͼƬ
@param err_img_id [in] ���������ͼƬ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDefaultIcon(
                                     MMI_CTRL_ID_T     ctrl_id, //in
                                     MMI_IMAGE_ID_T    def_img_id,  // default image id
                                     MMI_IMAGE_ID_T    err_img_id   // error image id
                                     );

/***************************************************************************//*!
@brief set the tab item valid or not
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ����
@param is_valid [in] �Ƿ���Ч
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemValid( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint32 item_index,
                                   BOOLEAN is_valid
                                   );


/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param bg_color [in] ����ɫ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemBgColor(
                                          MMI_CTRL_ID_T ctrl_id, 
                                          GUI_COLOR_T bg_color
                                          );


/***************************************************************************//*!
@brief ���ý���������ֱ���ͼ
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ��ԴͼƬID
@param item_index [in] ������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetFocusItemTextBgImage(
                                              MMI_CTRL_ID_T ctrl_id, 
                                              MMI_IMAGE_ID_T image_id,
                                              uint32 item_index
                                              );


/***************************************************************************//*!
@brief set ��ʼλ�õ�item index
@author  xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetStartIndex(
                                    MMI_CTRL_ID_T ctrl_id, 
                                    uint32 item_index
                                    );


/***************************************************************************//*!
@brief ��ÿ�ʼλ�õ�item index
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval �����ʼ��������
@return �����ʼ��������
@note 
*******************************************************************************/
PUBLIC uint32 CTRLTAB_GetStartIndex(
                                   MMI_CTRL_ID_T ctrl_id
                                   );


/***************************************************************************//*!
@brief ������ʾ������ʱ���Ƿ�ͬ�����룬Ĭ�����첽����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_sync_decode [in] true/false
@retval true/false
@return ���سɹ���ʧ��
@note ��������֮ǰ��״̬
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetSyncDecodeAnim(
                                        MMI_CTRL_ID_T  ctrl_id,
                                        BOOLEAN is_sync_decode
                                        );


/***************************************************************************//*!
@brief set the item's default width
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_height [in] tab��߶�
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetItemDefaultHeight(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint32 item_height
                                          );


/***************************************************************************//*!
@brief Set the back ground color.
@author Lianxiang
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ͼƬ��ԴID
@param is_update [in] �Ƿ�����ˢ��
@retval void
@return void
@note ��item���ڽ���״̬ʱ������ʾ����״̬�ı��������øýӿڿɸı佹�㱳��ͼƬ
*******************************************************************************/
PUBLIC void CTRLTAB_SetItemSelBgImage(
                                     MMI_CTRL_ID_T ctrl_id, 
                                     MMI_IMAGE_ID_T image_id,
                                     BOOLEAN is_update
                                     );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param str_ptr [in]  �ַ���
@param is_set [in] ���û�ȡ�����ã���Ϊfalseʱ��str_ptr����ΪNULL
@retval true/false
@return ���سɹ���ʧ��
@note if is_set = TRUE, set title; else if is_set = FALSE, release title
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleInfo(
                                   MMI_CTRL_ID_T ctrl_id,
                                   MMI_STRING_T *str_ptr,
                                   BOOLEAN is_set
                                   );


/***************************************************************************//*!
@brief set title
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_id [in] �ַ���ԴID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleTextId(
                                     MMI_CTRL_ID_T ctrl_id,
                                     MMI_TEXT_ID_T text_id
                                     );


/***************************************************************************//*!
@brief set title icon
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param icon_id [in] ͼƬ��ԴID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTitleIcon(
                                   MMI_CTRL_ID_T ctrl_id,
                                   MMI_IMAGE_ID_T icon_id
                                   );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetDividingLine(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetTpDownBg(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         MMI_IMAGE_ID_T image_id
                                         );

/*****************************************************************************/
//  Description : set DividingLine  image.
//  Global resource dependence : 
//  Author:  guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTAB_SetIsDisplayPrg(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         BOOLEAN is_display_prg
                                         );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

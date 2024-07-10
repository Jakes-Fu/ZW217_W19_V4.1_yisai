/*! @file guitext.h
@brief ���ļ�������text�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Text �ı�(Text)

-# @ref sec_text_function
-# @ref sec_text_def
-# @ref sec_text_create
-# @ref sec_text_function_list
-# @ref sec_text_function_relationship
-# @ref sec_text_app

@section sec_text_function �ؼ��Ĺ���

TextBox�ؼ���һ�������ı���ʾ�ؼ����ܹ��������ֵ���ʾλ�á���ʾ���򡢴�С������ȷ�������ͨ��LCD�ӿ�������û�������Label�ؼ�����Ҫ�������TextBox�ؼ��ܹ���ʾ�����ı����ı�����������ʾ�ı��ģ�����������ʵ�ֽ����������ṩ����༭���ܡ�

Textbox�ؼ������������ԣ�
-# �ܹ����������С��������ɫ������ͨ��LCD����ӿ���ʾ���Ի����֣�
-# �û����������ı�����ʾ����磺�Զ���������ʾ����߿��иߡ��м�ࡢ�ּ�࣬�ָ��ߵ����ԣ�
-# �ؼ��ܹ������û����ã��磺������ʾ�����иߡ��м�ࡢ�ּ������Լ����ÿ����ʾ�������Լ������Ȳ�����
-# �ؼ��ܹ��������ʾ���ֵ�ҳ��������һҳ����ʾ���������û����Խ������ɷ�ҳ�Ĳ�����
-# ����ʾ���Ի��ı���ͼƬ����ɫ(�������ñ���ͼƬʱ���������ÿؼ����ֱ�����ɫ)�����Ը����û��趨��ʾ�ı��߿�
-# ʵ�ִ��ʺͰ�����ҳ/������
-# ʵ�ָ��ƹ���
-# �ַ����������ܣ��������ַ������������url��email��phone number

@section sec_text_def �궨�����

@code
#define CREATE_TEXT_CTRL(_CTRL_ID_) \
        MMK_CREATE_TEXT, _CTRL_ID_
@endcode

- ���ڴ���һ��Text Box�ؼ���
    - _CTRL_ID_ �ؼ�ID 

@section sec_text_create �ؼ�����

������Ȼ�������Ĵ������sample_test_ctrl.c�ļ��С�
- ��һ��������һ�����Կؼ��Ĵ��ڣ�����Table����ΪMMI_TEST_TEXTBOX_WIN_TAB��
@code
WINDOW_TABLE( MMI_TEST_TEXTBOX_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_TEXTBOX_WIN_ID ),
    WIN_FUNC((uint32) HandleTestTextboxWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_TEXTBOX_TITLE),
    CREATE_TEXTBOX_CTRL(
        MMI_TEST_TEXTBOX_CTRL_ID
        ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode
    - �ô��ڵ�Ԫ���д���ID MMI_TEST_TEXTBOX_WIN_ID�����ڴ�����HandleTestTextboxWinMsg�����ڱ���ؼ���Textbox�ؼ������ڵ�Softkey�ؼ��ȣ�����Textbox�ؼ�������Ϊ���ڵĿͻ����򣨼�ȥ������ؼ���Softkey�����򣩡�
    - ������Ҫ��ӱ���ؼ����ı���Դ�Լ�Textbox�ؼ�ID�ʹ���ID����ӷ�ʽ�������ؼ���ͬ�������ﲻ��׸���ˡ�
- �ڶ��������Ӵ��ڵĴ�������
@code
LOCAL MMI_RESULT_E HandleTestTextboxWinMsg(
                                           MMI_WIN_ID_T     win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM           param
                                           )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_TEXTBOX_CTRL_ID;
    MMI_STRING_T        string = {0};

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        MMI_GetLabelTextByLang(TXT_HELLO_TEST_TEXTBOX_TEXT, &string);
        GUITEXTBOX_SetText(ctrl_id , &string, FALSE);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
@endcode
    @note Open��Ϣʱ���������˿ؼ�ΪActive�ؼ��������ÿؼ���ʾ���ı���Ϣ�������ı���ԴTXT_HELLO_TEST_TEXTBOX_TEXT����ӣ�������Լ���ɡ�
- �����������Ӵ������ڵĺ�����ͬʱ��ͷ�ļ�sample_test_ctrl.h�����Ӻ�������
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateTextboxWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_TEXTBOX_WIN_TAB, PNULL);
}
@endcode
- ���Ĳ���
    - ��hello sprd������������ڣ��ڽӿ�HandleHelloSprdWinMsg���������´��룩��
@code
case MSG_APP_1:
        MMIHELLO_CreateTextboxWin();
        break;
@endcode
    - ��ʾ��sprd�����а�1�����ı�����Դ��ڡ�
- ���岽����������
    - ���ȱ�����Դ����Ϊ���������������ַ�����Դ��
    - ��α�������ģ��������hello sprd�����а�1�����򿪴������£�
@image html text_test_win.jpg
ͼ 2 21Textbox�ؼ����Դ���
    @note �������û�����ÿؼ�������ԭ���Ǳ����е�Textbox��ʾ�������ͻ�����MMK��������ʱ���Զ���������������Layout�����û�Ҫ����ô�Ͳ����޸ġ���������ϣ���ôͬ����Ҫ�ڴ��ڵ�Open��Ϣ�������û���Ҫ��ʾ������

@section sec_text_function_list �ؼ��ӿڽ���

-# @ref GUITEXT_SetString
@note �ýӿ��ڲ������·���ռ����������ݣ�����û������ָ��Ҫ�Լ��ͷţ��Է�ֹ�ڴ�й¶�����⣬���øýӿ�֮ǰ������û���Ҫ���������ԣ�����ڴ˽ӿ�֮ǰ����Ϊ��Щ���Ի��������ݵ����½���������ִ��Ч�ʵ��½����������������С����ʾ����
-# @ref GUITEXT_SetRect
@note TextĬ������Ϊȥ��Title,Softkey���м����򡣲�ͬ��Ĭ����������Ҫ���ô˺�����������GUITEXT_SetString()����֮ǰ���ã��������Textbox�ؼ������ı�����������ı����ݵ����½��������µ�ִ��Ч�ʵ��½��� 
-# @ref GUITEXT_SetBorder
@note �߿��ǿؼ��Ļ������ԣ�Ĭ��û�б߿�
-# @ref GUITEXT_SetBg
@note �����ǿؼ��Ļ������ԣ�Ĭ��ͨ�ñ���ͼƬ��
-# @ref GUITEXT_SetFont
@note ��ͬ��Ĭ�����壬����Ҫ���ô˺�����������GUITEXT_SetString()����֮ǰ���ã��������Text���ַ�������������ַ��������½��������µ�ִ��Ч�ʵ��½�����
-# @ref GUITEXT_SetAutoScroll
-# @ref GUITEXT_SetCircularHandle
@note Ĭ��ѭ���������°�����Ϣ���û�����ֱ���޸�mmitheme_text.c�е����ԣ���������ƽ̨Text��
-# @ref GUITEXT_SetHandleTpMsg
@note �ýӿڵ��ûᵼ�¸�Textbox�ؼ�����ճ������ʧЧ�������ʹ�á�
-# @ref GUITEXT_IsDisplayPrg

@section sec_text_function_relationship �ӿڵ��ù�ϵ

�ı��ؼ��Ľӿ��ڱ�����ʱ�������Ⱥ�˳�򣬵���GUITEXT_SetString��÷��������ã�ԭ���Ǹýӿڻ������ʾ���������С���Ƿ��й������ȼ����ı�����ʾ��Ϣ����������������ÿ����һ�����Զ��������ı������½���������Ч���½���

���⣬����ڴ��ڵ���ʾ�����У�Ӧ�ò���Ҫ�ı��ı�����ʾ�������ݣ���ô��Щ�ӿ�Ӧ�÷��ڴ��ڵ�Open��Ϣ�е��þͿ����ˡ�

�����ı������ʾ��񣬶�����Ĭ��ֵ�ģ�ͨ��MMITHEME_GetTextTheme�ӿڻ�ȡ���ýӿ���mmitheme_text.c��ʵ�֡�

@section sec_text_app �ؼ���Ӧ��

��ʵ��Ӧ���У�ʱ��������ʾ�����ı������󣬱�����ƽ̨��Ϸ��˵�����У���Ϸ��˵�����ֱȽ϶࣬ʹ��Textbox�ؼ��ܹ�����ȫ��ĳ��ָ��û�����ǰ���������Ҫ��ʾ��text�Ƚ϶࣬���Է�ҳ��ʾ��������ʾ�������Ա��û�������
@image html text_app_example.jpg
ͼ 2 22Textbox�ؼ���ʵ��Ӧ��
*******************************************************************************/

/*! @addtogroup textGrp Text
@brief text�ؼ�
@details 
    - �Զ��Ű�,��ʾ�ı�\n
    - ֧�ָ��ƹ���,���Ը�����ʾ
@{
*******************************************************************************/


#ifndef _CTRLTEXT_EXPORT_H_
#define _CTRLTEXT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guictrl.h"
#include "guiprgbox.h"
#include "mmitheme_text.h"

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
// text handle
typedef struct _text_obj_tag* CTRL_TEXT_HANDLE_T;

/*! @enum  GUITEXT_SCROLL_MODE_E 
@brief �ı�����ģʽ
*/
typedef enum
{
    GUITEXT_SCROLL_LINE,    /*!<���й���*/
    GUITEXT_SCROLL_PAGE,    /*!<��ҳ����*/
    GUITEXT_SCROLL_PIXEL,   /*!<�����ص����*/
    GUITEXT_SCROLL_RATIO,   /*!<���ؼ��߶ȵİٷֱȹ���*/
    GUITEXT_SCROLL_MAX      /*!<����λ*/
} GUITEXT_SCROLL_MODE_E;

/*! @typedef  GUITEXT_TAG_TYPE_E 
@brief �ı���������
*/
typedef uint32 GUITEXT_TAG_TYPE_E;

/*! @enum  guitext_tag_type 
@brief �ı���������
*/
enum guitext_tag_type
{
    GUITEXT_TAG_NONE        = 0x00, /*!<������*/
    GUITEXT_TAG_PHONENUM    = 0x01, /*!<�����绰����*/
    GUITEXT_TAG_EMAIL       = 0x02, /*!<����Email*/
    GUITEXT_TAG_URL         = 0x04, /*!<����URL*/
    GUITEXT_TAG_SET         = 0x08, /*!<Ӧ��ָ��*/
    GUITEXT_TAG_ALL         = GUITEXT_TAG_PHONENUM | GUITEXT_TAG_EMAIL | GUITEXT_TAG_URL,
};

/*! @struct GUITEXT_INIT_DATA_T
@brief �ı���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T   both_rect;    /*!<�ı�����������*/
} GUITEXT_INIT_DATA_T;

/*! @struct GUITEXT_TAG_DATA_T
@brief �û�ָ����ǩ����
*/
typedef struct
{
    uint16      start_pos;  /*!<��ǩ��ʼλ��*/
    uint16      len;        /*!<��ǩ����*/
} GUITEXT_TAG_DATA_T;

/*! @struct GUITEXT_SET_TAG_T
@brief �û�ָ����ǩ
*/
typedef struct
{
    uint16              tag_num;    /*!<��ǩ����*/
    GUITEXT_TAG_DATA_T  *tag_ptr;   /*!<��ǩ��������*/
} GUITEXT_SET_TAG_T;

/*! @struct GUITEXT_TAG_INFO_T
@brief ��ǩ������Ϣ
*/
typedef struct
{
    wchar               *str_ptr;   /*!<��ǩ�ַ���ָ�� */
    uint16              str_len;    /*!<��ǩ�ַ��ĳ��� */
    GUITEXT_TAG_TYPE_E  type;       /*!<��ǩ��������*/
} GUITEXT_TAG_INFO_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ����ı��ַ���
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param str_ptr   [in] �ı����ݺͳ���
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetString(
								 MMI_CTRL_ID_T	ctrl_id,    //in:
								 MMI_STRING_T	*str_ptr    //in:/out
								 );

/***************************************************************************//*!
@brief �����ı��ַ���
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param str_ptr   [in] �ı�����
@param str_len   [in] �ı�����
@param is_update [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetString(
                                 MMI_CTRL_ID_T      ctrl_id,    //in:
                                 wchar              *str_ptr,   //in:
                                 uint16             str_len,    //in:
                                 BOOLEAN            is_update   //in:
                                 );

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in] ����
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetRect(
                            MMI_HANDLE_T    ctrl_handle,
                            GUI_RECT_T      *rect_ptr
                            );

/***************************************************************************//*!
@brief ���ÿؼ�ҳ�߾�
@author Jassmine
@param ctrl_id		[in] �ؼ�id
@param margin_space [in] ҳ�߾�
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetMargin(
                                 MMI_CTRL_ID_T  ctrl_id,	//in:
                                 uint16			margin_space//in:
                                 );

/*****************************************************************************/
//  Description : set text margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetMarginEx(
                                   MMI_CTRL_ID_T   ctrl_id,    //in
                                   uint16          margin_left,  //in:left margin
                                   uint16          margin_right,   //in:right margin
                                   uint16          margin_top,  //in:top margin
                                   uint16          margin_bottom   //in:bottom margin
                                   );

/***************************************************************************//*!
@brief ���ÿؼ��м��
@author Jassmine
@param ctrl_id		[in] �ؼ�id
@param margin_space [in] �м��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetLineSpace(
                                    MMI_CTRL_ID_T  ctrl_id,	//in:
                                    uint16		   line_space//in:
                                    );

/***************************************************************************//*!
@brief ���ÿؼ��߿�,�����߿�����,����Լ���ɫ
@author Jassmine
@param border_ptr [in] �߿���Ϣ
@param ctrl_id    [in] �ؼ�id
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetBorder(
                                 GUI_BORDER_T   *border_ptr,//in
                                 MMI_CTRL_ID_T  ctrl_id     //in:
                                 );

/***************************************************************************//*!
@brief �����ı�����
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param bg_ptr  [in] ������Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetBg(
                          MMI_CTRL_ID_T     ctrl_id,
                          GUI_BG_T          *bg_ptr
                          );

/***************************************************************************//*!
@brief �����ı������С������ɫ
@author Jassmine
@param ctrl_id        [in] �ؼ�id
@param font_ptr       [in] �����С,PNULL������
@param font_color_ptr [in] ������ɫ,PNULL������
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetFont(
                            MMI_CTRL_ID_T       ctrl_id,        //in:
                            GUI_FONT_T          *font_ptr,      //in:may PNULL
                            GUI_COLOR_T         *font_color_ptr //in:may PNULL
                            );

/***************************************************************************//*!
@brief ��õ�ǰ��ǩ��Ϣ
@author Jassmine
@param ctrl_id              [in] �ؼ�id
@param GUITEXT_TAG_INFO_T   [in] ��ǩ��Ϣ
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetCurTag(
                                 MMI_CTRL_ID_T          ctrl_id,    //in:
                                 GUITEXT_TAG_INFO_T     *tag_ptr    //in/out:
                                 );

/***************************************************************************//*!
@brief �����ı���������
@author Jassmine
@param ctrl_id        [in] �ؼ�id
@param parse_type     [in] ��������,���������
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetTagType(
                               MMI_CTRL_ID_T        ctrl_id,    //in:
                               GUITEXT_TAG_TYPE_E   tag_type,   //in:
                               GUITEXT_SET_TAG_T    *set_tag_ptr//in:may PNULL,GUITEXT_TAG_SET,must set the pointer 
                               );

/***************************************************************************//*!
@brief �����ı����з�ʽ,Ĭ�����϶���
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param align   [in] ���з�ʽ
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetAlign(
                             MMI_CTRL_ID_T  ctrl_id,//in
                             GUI_ALIGN_E    align   //in
                             );

/***************************************************************************//*!
@brief �����ı��Զ�������ʾ
@author Jassmine
@param is_auto_scroll    [in] �Ƿ��Զ������ı�
@param scroll_period_ptr [in] ����ʱ����,PNULL��ʹ��Ĭ��ʱ����
@param ctrl_id           [in] �ؼ�id
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetAutoScroll(
                                  BOOLEAN           is_auto_scroll,     //in:
                                  uint32            *scroll_period_ptr, //in:may PNULL default
                                  MMI_CTRL_ID_T     ctrl_id             //in:
                                  );

/***************************************************************************//*!
@brief �����ı��ؼ��Ƿ�ѭ�������򰴼�
@author Jassmine
@param is_circular_handle [in] �Ƿ�ѭ�������򰴼�
@param ctrl_id            [in] �ؼ�id
@note 
*******************************************************************************/
PUBLIC void CTRLTEXT_SetCircularHandle(
                                      BOOLEAN       is_circular_handle, //in:
                                      MMI_CTRL_ID_T ctrl_id             //in:
                                      );

/***************************************************************************//*!
@brief �����ı��ؼ��Ƿ�������Ϣ
@author Jassmine
@param is_handle [in] �Ƿ�������Ϣ
@param ctrl_id   [in] �ؼ�id
@note Ĭ�ϴ���
*******************************************************************************/
PUBLIC void CTRLTEXT_SetHandleTpMsg(
                                   BOOLEAN          is_handle,  //in:
                                   MMI_CTRL_ID_T    ctrl_id     //in:
                                   );

/***************************************************************************//*!
@brief �����ı��ؼ��Ƿ���ʾ������
@author Jassmine
@param is_display [in] �Ƿ���ʾ������
@param ctrl_id    [in] �ؼ�id
@note Ĭ����ʾ
*******************************************************************************/
PUBLIC void CTRLTEXT_IsDisplayPrg(
                                 BOOLEAN            is_display, //in:
                                 MMI_CTRL_ID_T      ctrl_id     //in:
                                 );

/*****************************************************************************/
// 	Description : set text is support slide,default TRUE
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
/***************************************************************************//*!
@brief �����ı��ؼ��Ƿ�֧�ֻ���
@author Jassmine
@param text_handle [in] �ؼ�handle
@param is_support  [in] �Ƿ�֧�ֻ���
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_IsSlide(
                               MMI_HANDLE_T   text_handle,
                               BOOLEAN        is_support
                               );

/***************************************************************************//*!
@brief �����ı��ؼ��Ƿ�֧�ּ�����
@author Jassmine
@param text_handle [in] �ؼ�handle
@param is_support  [in] �Ƿ�֧�ּ����壬Ĭ��Ϊ֧��
@note ������ƽ̨֧�ּ����������²���Ч
*******************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetClipboardEnabled(
                                           MMI_HANDLE_T     text_handle,
                                           BOOLEAN          is_support
                                           );

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetPosInfo(
                                  MMI_CTRL_ID_T      ctrl_id,
                                  uint16             pos,                //in
                                  GUI_RECT_T         *cursor_rect_ptr    //in/out:
                                  );

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_GetHighlightInfo(
                                        MMI_CTRL_ID_T      ctrl_id,
                                        uint16             *highlight_min_ptr,
                                        uint16             *highlight_max_ptr
                                        );

//zmt add start
/*****************************************************************************/
//  Description : set text string display top index
//  Global resource dependence : 
//  Author: fys
//  Note:2024.06.18
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXT_SetResetTopDisplay(
                                        MMI_CTRL_ID_T ctrl_id, 
                                        int32 top_index
                                        );
//zmt add end
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

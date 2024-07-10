/*****************************************************************************
** File Name:      ctrliconlist_export.h                                        *
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
/*! @page page_IconList �����б�(IconList)

-# @ref sec_iconlist_function
-# @ref sec_iconlist_def
-# @ref sec_iconlist_create
-# @ref sec_iconlist_function_list
-# @ref sec_iconlist_relation
-# @ref sec_iconlist_app

@section sec_iconlist_function �ؼ�����

IconList�ؼ�������ʾIcon����ͼ���б��û�����ѡ������ͼ�б������һ�Ŀǰֻ֧�־Ź������ʾ���ÿҳ���������ʾ�Ÿ�Icon����ͼ��ÿ�к�ÿ�ж���ʾ����Icon����ͼ��

IconList�ؼ��Ĺ����������£�
-# ��һҳ�޷���ʾ���е�Icon����ͼʱ���ڿؼ��ڵ��Ҳ����ʾ���¹�������ͨ�����������������������Icon����ͼ��
-# ��ҳ����ʹ�á�*�����͡�\#����ʵ�֡���*�����������Ϸ�ҳ����\#�����������·�ҳ��Ҳ����ͨ�����ʵ��������ʵ�ַ�ҳ���ܡ���ҳʱ�����л���ǰѡ���Icon��
-# ���Ҽ����������л���ǰѡ���Icon�����ң���ʹ�õ�ǰѡ���Icon�л�Ϊǰ����һ��Icon����һ��Icon��ǰһ��IconΪ���һ��Icon�����һ��Icon�ĺ�һ��IconΪ��һ��Icon��Ҳ����ͨ�����ʵ��������ʵ�ֵ�ͬ���Ҽ��Ĺ��ܡ�
-# ���¼�Ҳ���������л���ǰѡ���Icon���ϣ��£���ʹ�õ�ǰѡ���Icon�л�Ϊǰ����һ��ͬһ�е�Icon����һ�е�ǰһ�������һ�У����һ�еĺ�һ���ǵ�һ�С�����л�����������һ�У���ô�п�����ͬһ���в�����Icon����ʱѡ�����һ��Icon��
-# �л���ǰѡ���Icon�������ǰѡ���Icon���ڵ�ǰ��ʾ��ҳ�ڣ���ô��ɾ������ʾ��Icon�������Ҫ��ʾ���µ�Icon��������ǰѡ���Icon��
-# ����ʱ��֧�ֻ����������������Ŵ��ʵ��ƶ���icon�б��涯��

IconList�ؼ���Ч������ͼ��ʾ�������˾Ź������ʾ������ڿؼ���Icon����С��ÿҳ����ʾ��Icon��������˲��������������˿ؼ�����8��Icon����ͼ����ǰѡ���˵�6��Icon����ͼ���������м�λ����ʾ��ǰѡ���Icon��������Ϣ�����������²���ʾ��ǰѡ���Icon����ź�Icon������
@image html iconlist_effect.jpg 
ͼ 2 35 IconList�ؼ���Ч��ͼ

@section sec_iconlist_def �궨�����

@code
#define CREATE_ICONLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_ICONLIST, _CTRL_ID_
@endcode
- ���ڴ���һ��Iconlist�ؼ���
    - _CTRL_ID_ �ؼ�ID

@section sec_iconlist_create �ؼ�����

���벿����Ȼ����sample_iconlist_ctrl.c�ļ��С�

- ��һ��������һ�����Կؼ��Ĵ��ڣ�����Table����ΪMMI_TEST_ICONLIST_WIN_TAB��
@code
WINDOW_TABLE(MMI_TEST_ICONLIST_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_ICONLIST_WIN_ID),
    WIN_TITLE(TXT_HELLO_TITLE),        
	WIN_FUNC( (uint32)HandleTestIconlistWinMsg),
	CREATE_ICONLIST_CTRL(TEST_ICONLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	END_WIN
};
@endcode
    - �ô��ڵ�Ԫ���д���ID MMI_TEST_ICONLIST_WIN_ID�����ڱ���ؼ������ڴ�����HandleTestIconlistWinMsg��Iconlist�ؼ������ڵ�Softkey�ؼ��ȣ�����Iconlist�ؼ�������Ϊ���ڵĿͻ����򣨼�ȥ������ؼ���Softkey�����򣩡�
- �ڶ��������Ӵ��ڵĴ�������
@code
LOCAL MMI_RESULT_E HandleTestIconlistWinMsg(
                 MMI_WIN_ID_T        win_id,
                 MMI_MESSAGE_ID_E    msg_id,
                 DPARAM              param
                 )
{
    uint16                      icon_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_STRING_T                title_text  = {0};
    MMI_CTRL_ID_T               ctrl_id = MMI_TEST_ICONLIST_CTRL_ID;
    GUIANIM_DATA_INFO_T         data_info = {0};
    GUIICONLIST_ICON_DISP_T     icon_list = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIICONLIST_SetTotalIcon(ctrl_id,MMIHELLO_ICONLIST_ICON_NUM);
        GUIICONLIST_SetCurIconIndex(TRUE,0,ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        //append icon item
        icon_index = *((uint16 *)param);

        MMI_GetLabelTextByLang(TXT_HELLO_TEST,&title_text);

        MMI_WSTRNCPY(icon_list.name_wstr,GUIICONLIST_NAME_MAX_LEN,
            title_text.wstr_ptr,title_text.wstr_len,
            title_text.wstr_len);
        icon_list.name_len = title_text.wstr_len;

        //softkey
        icon_list.softkey_id[0] = TXT_NULL;
        icon_list.softkey_id[1] = TXT_NULL;
        icon_list.softkey_id[2] = STXT_RETURN;

        GUIICONLIST_AppendItem(icon_index,ctrl_id,&icon_list);
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        //append icon
        icon_index = *((uint16 *)param);

        data_info.img_id = HELLO_SPRD_TEST;
        data_info.type   = GUIANIM_TYPE_IMAGE_BMP;
        
        //append icon
        GUIICONLIST_AppendIcon(icon_index,ctrl_id,&data_info,PNULL);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
@endcode
@note ע�⣺Open��Ϣʱ���������˿ؼ�ΪActive�ؼ��������ÿؼ���ʾ���ı���ͼƬ��Ϣ��
- �����������Ӵ������ڵĺ�����ͬʱ��ͷ�ļ�sample_iconlist_ctrl.h�����Ӻ�������
@code
PUBLIC void MMIHELLO_CreateIconlistWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ICONLIST_WIN_TAB, PNULL);
}
@endcode
- ���Ĳ�����hello sprd������������ڣ��ڽӿ�HandleHelloSprdWinMsg���������´��룩��
@code
case MSG_APP_5:
        MMIHELLO_CreateAnimWin();
        break;
@endcode
    - ��ʾ��sprd�����а�5���򿪶��� ���Դ��ڡ�
- ���岽����������
    - ���ȱ�������ģ��������hello sprd�����а�5�����򿪴������£�
@image html iconlist_test_win.jpg
ͼ 2 36 Icnlist�ؼ����Դ���

@section sec_iconlist_function_list �ؼ��ӿڽ���

-# @ref GUIICONLIST_SetTotalIcon
-# @ref GUIICONLIST_SetCurIconIndex
-# @ref GUIICONLIST_GetCurIconIndex
-# GUIICONLIST_AppendItem
-# @ref GUIICONLIST_AppendIcon

@section sec_iconlist_relation �ӿڵ��ù�ϵ

Iconlist�ؼ��Ľӿ��ڱ�����ʱ�����Ⱥ�˳��ģ�GUIICONLIST_SetTotalIcon��GUIICONLIST_SetCurIconIndex������ڼ���Iconlist��Ϣ֮ǰ���ã�ԭ�����������ӿ��ڵ���ʱ�������Ƿ���Ҫ��ʾ����������ǰҳ��һ��ͼƬ����������Ϣ��

@section sec_iconlist_app �ؼ�Ӧ��

Iconlist�ؼ��������ͼƬ������ͼ���������ļ�������У�������ļ����е�ͼƬ������ͼ��ʾʱ������ͼ��ʾ��

*******************************************************************************/

/*! @addtogroup iconlistGrp IconList
@brief iconlist�ؼ�
@details 
    -#  ͼƬ�Թ�����ʽ��ʾ\n
    -#  ֧�ֱ��,ɾ���ȹ���\n
@{
*******************************************************************************/

#ifndef _CTRLICONLIST_EXPORT_H_
#define _CTRLICONLIST_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "mmitheme_iconlist.h"
#include "guiprgbox.h"
#include "guisoftkey.h"
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
/*! @enum GUIICONLIST_STYLE_E
@brief �����б�����
*/
typedef enum
{
    GUIICONLIST_STYLE_ICON,         /*!<ֻ��ͼƬ�б�*/
    GUIICONLIST_STYLE_ICON_UIDT,    /*!<��ͼ����ͼƬ�б�*/
    GUIICONLIST_STYLE_FOLDER_UIDT,  /*!<��ͼ�����ļ����б�*/
    GUIICONLIST_STYLE_MAX           /*!<����λ*/
} GUIICONLIST_STYLE_E;

/*! @enum GUIICONLIST_LOAD_E
@brief Anim���ط�ʽ
*/
typedef enum
{
    GUIICONLIST_LOAD_CUR_PAGE,      /*!<һ���Լ��ص�ǰҳͼƬ*/
    GUIICONLIST_LOAD_ONE_BY_ONE,    /*!<��ǰҳͼƬһ��һ������*/
    GUIICONLIST_LOAD_ALL,           /*!<һ���Լ�������*/
    GUIICONLIST_LOAD_MAX            /*!<����λ*/
} GUIICONLIST_LOAD_E;

/*! @struct GUIICONLIST_LAYOUT_STYLE_E
@brief iconlist���Ű���Ĭ��Ϊ��ֱ�Ű�
*/
typedef enum
{
	GUIICONLIST_LAYOUT_V,           // ��ֱ�Ű棬��֧�����»������л�
	GUIICONLIST_LAYOUT_H,           // ˮƽ�Ű棬��֧��ˮƽ����
	GUIICONLIST_LAYOUT_MAX          // ����λ
}GUIICONLIST_LAYOUT_STYLE_E;

/*! @struct GUIICONLIST_INIT_DATA_T
@brief �����б�ؼ���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<����������*/
} GUIICONLIST_INIT_DATA_T;

/*! @struct GUIICONLIST_DISP_T
@brief ����������Ϣ
*/
typedef struct
{
    MMI_STRING_T    name_str;                           /*!<ͼƬ����*/
    MMI_TEXT_ID_T   softkey_id[GUISOFTKEY_BUTTON_NUM];  /*!<���������*/
} GUIICONLIST_DISP_T;

typedef struct  
{
	uint16            icon_item_hspace;
	uint16            icon_item_vspace;
	uint16            iconlist_hmargin;
	uint16            iconlist_vmargin;
}GUIICONLIST_MARGINSPACE_INFO_T;

#ifdef GUIF_ICONLIST_DELIMITER
//iconlist group info
typedef struct
{
    uint16          start_item_index;         // ��һ��icon����ʼ����ֵ
	MMI_STRING_T    delimiter_str_info;       // �ָ����ڵ��ַ�����Ϣ
} GUIICONLIST_DELIMITER_INFO_T;
#endif
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ��ù����б���
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param style     [in] ���
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetStyle(
                                MMI_HANDLE_T        ctrl_id,
                                GUIICONLIST_STYLE_E style
                                );

/***************************************************************************//*!
@brief ���ù����б�load anim��ʽ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param load_type [in] load anim��ʽ
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetLoadType(
                                    MMI_HANDLE_T        ctrl_id,
                                    GUIICONLIST_LOAD_E  load_type
                                    );

/***************************************************************************//*!
@brief ��ù����б�ؼ�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param bg_ptr    [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetBg(
                                 MMI_CTRL_ID_T  ctrl_id,//in
                                 GUI_BG_T       *bg_ptr //in
                                 );

/***************************************************************************//*!
@brief ��ù����б�ؼ�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param rect_ptr  [in] ����
@return
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetRect(
                                MMI_HANDLE_T    ctrl_id,
                                GUI_RECT_T      *rect_ptr
                                );

/***************************************************************************//*!
@brief ��ù����б�ؼ���ͼƬ����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@return �ܸ���
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetTotalIcon(
									   MMI_CTRL_ID_T	ctrl_id	//in
									   );

/***************************************************************************//*!
@brief ���ù����б�ؼ���ͼƬ����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param total_num [in] �ܸ���
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetTotalIcon(
                                     MMI_CTRL_ID_T    ctrl_id,  //in
                                     uint16           total_num //in
                                     );

/***************************************************************************//*!
@brief �����ʾ�ı���Ϣ
@author Jassmine
@param index       [in] item����
@param ctrl_id     [in] �ؼ�id
@param display_ptr [in] �ı���ʾ��Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendText(
                                      uint16                index,      //in
                                      MMI_CTRL_ID_T         ctrl_id,    //in
                                      GUIICONLIST_DISP_T    *display_ptr//in
                                      );

/***************************************************************************//*!
@brief  �����ʾ�ı��ĸ�����Ϣ
@author xiyuan.ma
@param  index       [in] item����
@param  ctrl_id     [in] �ؼ�id
@param  display_ptr [in] �ı���ʾ��Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendTextAddtion(
											 uint16                index,      //in
											 MMI_CTRL_ID_T         ctrl_id,    //in
											 GUIICONLIST_DISP_T    *display_ptr//in
											 );
/***************************************************************************//*!
@brief ���ͼƬ��Ϣ
@author Jassmine
@param index         [in] item����
@param ctrl_id       [in] �ؼ�id
@param data_info_ptr [in] ͼƬ������Ϣ
@param file_info_ptr [in] ͼƬ�ļ���Ϣ
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIcon(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
									  GUIANIM_DATA_INFO_T   *data_info_ptr,	//in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr  //in
                                      );

/***************************************************************************//*!
@brief ���ͼƬ��Ϣ
@author Jassmine
@param index         [in] item����
@param ctrl_id       [in] �ؼ�id
@param data_info_ptr [in] ͼƬ������Ϣ
@param file_info_ptr [in] ͼƬ�ļ���Ϣ
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_AppendIconEx(
                                      uint16                index,          //in
                                      MMI_CTRL_ID_T         ctrl_id,        //in
									  GUIANIM_DATA_INFO_T   *data_info_ptr,	//in
                                      GUIANIM_FILE_INFO_T   *file_info_ptr , //in
                                      GUIANIM_DISPLAY_INFO_T * display_info
                                      );


/*****************************************************************************/
//  Description : set icon number in one folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconNum(
                                      uint16            index,      //in
                                      uint16            icon_num,   //in
                                      MMI_CTRL_ID_T     ctrl_id     //in
                                      );

/***************************************************************************//*!
@brief ��õ�ǰѡ��item����
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return ��ǰͼƬ����
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetCurIconIndex(
                                          MMI_CTRL_ID_T     ctrl_id //in
                                          );

/***************************************************************************//*!
@brief ���õ�ǰѡ��ͼƬ����
@author Jassmine
@param cur_index [in] ��ǰ����,��0��ʼ
@param ctrl_id   [in] �ؼ�id
@note 
*******************************************************************************/
PUBLIC void CTRLICONLIST_SetCurIconIndex(
                                        uint16			cur_index,  //in:
                                        MMI_CTRL_ID_T	ctrl_id	    //in:
                                        );

/***************************************************************************//*!
@brief ��������ѡ�е�������
@author Jassmine
@param ctrl_id          [in] �ؼ�id
@param selected_max_num [in] ѡ�е�������
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMaxSelectedItem(
                                              MMI_CTRL_ID_T ctrl_id,
                                              uint16        selected_max_num
                                              );

/***************************************************************************//*!
@brief ��ù����б��Ƿ��ڱ��״̬
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return �Ƿ��ڱ��״̬
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_GetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id
                                       );

/***************************************************************************//*!
@brief ���ù����б��Ƿ��ڱ��״̬
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param is_markable [in] �Ƿ���״̬
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetMarkable(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       BOOLEAN          is_markable
                                       );

/***************************************************************************//*!
@brief ��������itemȫѡ����ȫ��ѡset selected all or not
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param is_selected [in] �Ƿ�ѡ��
@return �����Ƿ�ɹ�
@note �������ȫ��ѡ��,�������ѡ�и���ѡ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetAllSelected(
                                          MMI_CTRL_ID_T     ctrl_id,
                                          BOOLEAN           is_selected
                                          );

/***************************************************************************//*!
@brief ���item�Ƿ���ѡ��״̬get selected item or not
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param index   [in] item����
@return �Ƿ�ѡ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_IsSelectedItem(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16        index
                                          );

/***************************************************************************//*!
@brief ���ѡ��item������Ϣ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param index_ptr [in] ѡ����������
@param array_len [in] ѡ���������鳤��
@return ѡ��icon����
@note 
*******************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetSelectedItemIndex(//selected item number
                                               MMI_CTRL_ID_T    ctrl_id,    //in:
                                               uint16*		    index_ptr,  //in/out:may PNULL
                                               uint16		    array_len   //in:
                                               );

/***************************************************************************//*!
@brief ����item�Ƿ�ѡ��
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param index       [in] item����
@param is_selected [in] �Ƿ�ѡ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSelectedItem(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           uint16           index,
                                           BOOLEAN          is_selected
                                           );

/***************************************************************************//*!
@brief ɾ��ĳ��ָ��item
@author Jassmine
@param ctrl_id	[in] �ؼ�id
@param index	[in] item����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveItem(
                                      MMI_CTRL_ID_T ctrl_id,
                                      uint16        index
                                      );

/***************************************************************************//*!
@brief set icon width and height
@author Jassmine
@param ctrl_id	[in] �ؼ�id
@param width	[in] �ؼ�icon���
@param height	[in] �ؼ�icon�߶�
@note ��������ã���ʹ��Ĭ��ֵ
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconWidthHeight(
                                              MMI_CTRL_ID_T     ctrl_id,  //in
                                              uint16            width,    //in
                                              uint16            height    //in
                                              );

/***************************************************************************//*!
@brief ׼������
@author hua.fang
@param from_ctrl_id	[in] ��ʼ�ؼ�id
@param to_ctrl_id	[in] Ŀ��ؼ�id
@note 
���from_ctrl_id��folder���to_ctrl_id��icon��񣬻����ļ�����ͼƬչ���Ķ�����
�����豣֤������ʱ��from_ctrl_id�����ڣ�
���from_ctrl_id��icon���to_ctrl_id��folder��񣬻����ļ����ͼƬ��£�Ķ�����
���from_ctrl_id��icon���to_ctrl_idΪ�գ����������ͼƬ�Ŵ�Ķ�����
���from_ctrl_id�ǿգ�to_ctrl_id��icon��񣬻���ͼƬ��£��Ч��
*******************************************************************************/
PUBLIC void CTRLICONLIST_PrepareSpecial(
                                       MMI_CTRL_ID_T    from_ctrl_id,
                                       MMI_CTRL_ID_T    to_ctrl_id
                                       );
									   
/*****************************************************************************//*!
@brief  ����֧�ֺ��򻬶�
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] �ؼ�ptr
@param  is_v_slide	        [in] �Ƿ�֧�ֺ��򻬶�
@note   ��������˺����ƶ���Ҳ�����ú��򲼾�
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetLayoutStyle(
										  MMI_CTRL_ID_T   ctrl_id,
										  GUIICONLIST_LAYOUT_STYLE_E layout_style
										  );

/*****************************************************************************//*!
@brief  �Ƿ�֧�ֺ��򻬶�
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] �ؼ�ptr
@note   �ж������ƶ�
*****************************************************************************/
PUBLIC GUIICONLIST_LAYOUT_STYLE_E CTRLICONLIST_GetLayoutStyle(
															 MMI_CTRL_ID_T   ctrl_id
															 );

/*****************************************************************************//*!
@brief  �޸�Iconlist�����塢��ɫ��Ϣ
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] �ؼ�id
@param  iconlist_text_style_ptr	    [in] iconlist��������Ϣ�޸�
@note   ��������ã���Ĭ��Ϊiconlist���������ɫ
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconListTextInfo(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   GUI_FONT_ALL_T iconlist_text_style //in
											   );

/*****************************************************************************//*!
@brief  Set iconitem margin & space
@author xiyuan.ma 2011.12
@param  ctrl_id	                                [in] �ؼ�id
@param  GUIICONLIST_MARGINSPACE_INFO_T	        [in] �߾ࡢ���
@note   ��������ã���Ĭ��Ϊiconlist���м�ࡢ�м�ࡢ�������ұ߾�
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetIconItemSpace(
											MMI_CTRL_ID_T                     ctrl_id,       //in
											GUIICONLIST_MARGINSPACE_INFO_T    margin_space   //in
											);

/*****************************************************************************//*!
@brief  ����item��icon�Ƿ�֧������
@author xiyuan.ma 2011.12
@param  Is_ItemIcon_Zoom	        [in] �Ƿ������
@note   
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemIconZoom(
										   MMI_CTRL_ID_T  ctrl_id,
										   BOOLEAN        is_itemicon_zoom
										   );

/*****************************************************************************/
//  Description : �û����Զ���Item��Border����ɫ����񡢴�ϸ 
//  Author: xiyuan.ma 2012.1
//  param   is_select_border	        [in]���õ���select border����unselect
//  param   select_border	            [in]border����ɫ����ϸ�����
//  Note:   is_select_border == TRUEʱ�� ��������select border��
//          is_select_border == FALSEʱ����������unselect border�� 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetItemBorderStyle(
                                              MMI_CTRL_ID_T  ctrl_id,
                                              BOOLEAN        is_select_border,
                                              GUI_BORDER_T   *select_border
                                              );
#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************//*!
@brief  ���÷ָ����ı���
@author xiyuan.ma 2011.12
@param  ctrl_id	       [in] �ؼ�id
@param  *bg_ptr	       [in] �ָ��������ṹ��ָ��
@note   ��������ã���ʹ��͸��
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBg(
										  MMI_CTRL_ID_T  ctrl_id,//in
										  GUI_BG_T       *bg_ptr //in
										  );

/*****************************************************************************//*!
@brief  �ָ����ı����հ������С
@author xiyuan.ma 2012.05
@param  ctrl_id	                   [in] �ؼ�id
@param  *delimiter_bg_space	       [in] �ָ��������հ�����ṹ��ָ��
@note   ����Ҫ���÷ָ������������ʵ�ʷָ�������Сʱ������
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterBgSpace(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   DELIMITER_BG_SPACE_T *delimiter_bg_space //in
											   );

/*****************************************************************************//*!
@brief  ���÷ָ�����ʾ���ַ�����������Ϣ
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] �ؼ�id
@param  *delimiter_text_style_ptr	[in] �ָ������ַ�����������Ϣ
@note   ��������ã���Ĭ��Ϊiconlist���������ɫ
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterStrInfo(
											   MMI_CTRL_ID_T  ctrl_id,//in
											   GUI_FONT_ALL_T *delimiter_text_style_ptr, //in
											   uint16         delimiter_str_left_space
											   );

/*****************************************************************************//*!
@brief  ���÷ָ����ĸ߶�
@author xiyuan.ma 2011.12
@param  ctrl_id	            [in] �ؼ�id
@param  delimiter_height	[in] �ָ����ĸ߶�
@note   ��������ã���Ĭ�ϸ߶�Ϊtheme�е�delimiter�ĸ߶�
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDelimiterHeight(
											  MMI_CTRL_ID_T  ctrl_id,//in
											  uint16         delimiter_height //in
											  );

/***********************************************************************************//*!
@brief  ����iconlist��ÿ�����Ϣ
@author xiyuan.ma 2011.12
@param  *iconlist_ctrl_ptr	[in] �ؼ�ָ��
@param  *group_info_ptr	    [in] �ָ����ṹ��ָ��
@note   ��Ҫ����start_item_index(��ʼitem index)��delimiter_str_info(�ָ����ڵ��ַ���)
        ����ʱ���밴�մ�С�����˳�����start_item_index
***********************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_CreateGroup(
									   MMI_CTRL_ID_T                 ctrl_id,         //in
									   GUIICONLIST_DELIMITER_INFO_T  *group_info_ptr	 //in									
									   );

/*****************************************************************************//*!
@brief  ɾ��ĳ���ָ���
@author xiyuan.ma 2011.12
@param  index	                [in] group id
@param  ctrl_id	                [in] �ؼ�  id
@note   ֻ����ɾ��ĳ���ָ���������ɾ���κ�Ԫ��
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveGroup(
                                       MMI_CTRL_ID_T      ctrl_id, 
                                       uint16             index
                                       );

/*****************************************************************************//*!
@brief  remove all group
@author xiyuan.ma 2012.03
@param  ctrl_id	                [in] �ؼ�  id
@note   
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_RemoveAllDelimiter(
                                              MMI_CTRL_ID_T      ctrl_id
                                              );

/*****************************************************************************//*!
@brief  ��ȡdelimiter group num
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@note   ��ȡ�ָ���������
*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDelimiterGroupNum(
                                               MMI_CTRL_ID_T      ctrl_id
                                               );
#endif

/*****************************************************************************//*!
@brief  �����Ƿ���ʾsoftkey
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@param  is_true	                [in] �Ƿ��û��Ի�
@note   TURE��ʱ���û��Ի�
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDispCustomerSoftkey(
                                                  MMI_CTRL_ID_T ctrl_id, 
                                                  BOOLEAN       is_true
                                                  );

/*****************************************************************************//*!
@brief  iconlist�Ƿ�֧�ֻ���
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@param  is_support_slide	    [in] �Ƿ�֧�ֻ���
@note   ��Ϊ��֧��
*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetSlideState(
                                         MMI_CTRL_ID_T ctrl_id, 
                                         BOOLEAN       is_support_slide
                                         );

/*****************************************************************************/
//  Description : set icon list cache page num to cache icon thumbnail
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONLIST_SetIconCacheNum(
    MMI_HANDLE_T        ctrl_id,
    uint16  num
);

/*****************************************************************************//*!
@brief  �û����Զ���Ĭ��ͼ��Errorͼ
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�    id
@param  def_img_id	            [in] Ĭ��ͼ  id
@param  err_img_id	            [in] Errorͼ id
@note
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetDefaultIconByPtr(
											   MMI_CTRL_ID_T    ctrl_id,
                                               MMI_IMAGE_ID_T   def_img_id,
                                               MMI_IMAGE_ID_T   err_img_id
											   );
/*****************************************************************************/
//  Description : ��װ����ӿ�---get icon list relative height,except border
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLICONLIST_GetRelativeHeight(
                                            MMI_CTRL_ID_T    ctrl_id
                                            );

/*****************************************************************************/
//  Description : ��װ����ӿ�---get icon list display rect height
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLICONLIST_GetDispHeight(
                                        MMI_CTRL_ID_T    ctrl_id
                                        );


/*****************************************************************************/
//  Description : ��װ����ӿ�---get icon list empty string id
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONLIST_SetEmptyStrId(
                                        MMI_CTRL_ID_T    ctrl_id , 
                                        MMI_TEXT_ID_T empty_text_id
                                        );


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

/*! @file guiiconlist.h
@brief ���ļ�������icon list�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date  04/26/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  8/2007       Jassmine              Creat
*******************************************************************************/

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


#ifndef _GUIICONLIST_H_
#define _GUIICONLIST_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_iconlist.h"
#include "guiprgbox.h"
#include "guisoftkey.h"
#include "guianim.h"
#include "ctrliconlist_export.h"

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
#define GUIICONLIST_SetStyle		CTRLICONLIST_SetStyle 

/***************************************************************************//*!
@brief ���ù����б�load anim��ʽ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param load_type [in] load anim��ʽ
@return
@note 
*******************************************************************************/
#define GUIICONLIST_SetLoadType		CTRLICONLIST_SetLoadType

/***************************************************************************//*!
@brief ��ù����б�ؼ�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param bg_ptr    [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_SetBg	CTRLICONLIST_SetBg                               

/***************************************************************************//*!
@brief ��ù����б�ؼ�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param rect_ptr  [in] ����
@return
@note 
*******************************************************************************/
#define GUIICONLIST_SetRect		CTRLICONLIST_SetRect
                              

/***************************************************************************//*!
@brief ��ù����б�ؼ���ͼƬ����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@return �ܸ���
@note 
*******************************************************************************/
#define GUIICONLIST_GetTotalIcon	CTRLICONLIST_GetTotalIcon

/***************************************************************************//*!
@brief ���ù����б�ؼ���ͼƬ����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param total_num [in] �ܸ���
@note 
*******************************************************************************/
#define GUIICONLIST_SetTotalIcon		CTRLICONLIST_SetTotalIcon

/***************************************************************************//*!
@brief �����ʾ�ı���Ϣ
@author Jassmine
@param index       [in] item����
@param ctrl_id     [in] �ؼ�id
@param display_ptr [in] �ı���ʾ��Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_AppendText		CTRLICONLIST_AppendText

/***************************************************************************//*!
@brief  �����ʾ�ı��ĸ�����Ϣ
@author xiyuan.ma
@param  index       [in] item����
@param  ctrl_id     [in] �ؼ�id
@param  display_ptr [in] �ı���ʾ��Ϣ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_AppendTextAddtion		CTRLICONLIST_AppendTextAddtion

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
#define GUIICONLIST_AppendIcon		CTRLICONLIST_AppendIcon

/*****************************************************************************/
//  Description : set icon number in one folder
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetIconNum		CTRLICONLIST_SetIconNum

/***************************************************************************//*!
@brief ��õ�ǰѡ��item����
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return ��ǰͼƬ����
@note 
*******************************************************************************/
#define GUIICONLIST_GetCurIconIndex	CTRLICONLIST_GetCurIconIndex

/***************************************************************************//*!
@brief ���õ�ǰѡ��ͼƬ����
@author Jassmine
@param cur_index [in] ��ǰ����,��0��ʼ
@param ctrl_id   [in] �ؼ�id
@note 
*******************************************************************************/
#define GUIICONLIST_SetCurIconIndex		CTRLICONLIST_SetCurIconIndex

/***************************************************************************//*!
@brief ��������ѡ�е�������
@author Jassmine
@param ctrl_id          [in] �ؼ�id
@param selected_max_num [in] ѡ�е�������
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_SetMaxSelectedItem	CTRLICONLIST_SetMaxSelectedItem

/***************************************************************************//*!
@brief ��ù����б��Ƿ��ڱ��״̬
@author Jassmine
@param ctrl_id [in] �ؼ�id
@return �Ƿ��ڱ��״̬
@note 
*******************************************************************************/
#define GUIICONLIST_GetMarkable	CTRLICONLIST_GetMarkable

/***************************************************************************//*!
@brief ���ù����б��Ƿ��ڱ��״̬
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param is_markable [in] �Ƿ���״̬
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_SetMarkable		CTRLICONLIST_SetMarkable

/***************************************************************************//*!
@brief ��������itemȫѡ����ȫ��ѡset selected all or not
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param is_selected [in] �Ƿ�ѡ��
@return �����Ƿ�ɹ�
@note �������ȫ��ѡ��,�������ѡ�и���ѡ��
*******************************************************************************/
#define GUIICONLIST_SetAllSelected	CTRLICONLIST_SetAllSelected

/***************************************************************************//*!
@brief ���item�Ƿ���ѡ��״̬get selected item or not
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param index   [in] item����
@return �Ƿ�ѡ��
@note 
*******************************************************************************/
#define GUIICONLIST_IsSelectedItem	CTRLICONLIST_IsSelectedItem

/***************************************************************************//*!
@brief ���ѡ��item������Ϣ
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param index_ptr [in] ѡ����������
@param array_len [in] ѡ���������鳤��
@return ѡ��icon����
@note 
*******************************************************************************/
#define GUIICONLIST_GetSelectedItemIndex	CTRLICONLIST_GetSelectedItemIndex

/***************************************************************************//*!
@brief ����item�Ƿ�ѡ��
@author Jassmine
@param ctrl_id     [in] �ؼ�id
@param index       [in] item����
@param is_selected [in] �Ƿ�ѡ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_SetSelectedItem	CTRLICONLIST_SetSelectedItem

/***************************************************************************//*!
@brief ɾ��ĳ��ָ��item
@author Jassmine
@param ctrl_id	[in] �ؼ�id
@param index	[in] item����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
#define GUIICONLIST_RemoveItem	CTRLICONLIST_RemoveItem

/***************************************************************************//*!
@brief set icon width and height
@author Jassmine
@param ctrl_id	[in] �ؼ�id
@param width	[in] �ؼ�icon���
@param height	[in] �ؼ�icon�߶�
@note ��������ã���ʹ��Ĭ��ֵ
*****************************************************************************/
#define GUIICONLIST_SetIconWidthHeight	CTRLICONLIST_SetIconWidthHeight

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
#define GUIICONLIST_PrepareSpecial	CTRLICONLIST_PrepareSpecial
									   
/*****************************************************************************//*!
@brief  ����֧�ֺ��򻬶�
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] �ؼ�ptr
@param  is_v_slide	        [in] �Ƿ�֧�ֺ��򻬶�
@note   ��������˺����ƶ���Ҳ�����ú��򲼾�
*****************************************************************************/
#define GUIICONLIST_SetLayoutStyle	CTRLICONLIST_SetLayoutStyle

/*****************************************************************************//*!
@brief  �Ƿ�֧�ֺ��򻬶�
@author xiyuan.ma
@param  iconlist_ctrl_ptr	[in] �ؼ�ptr
@note   �ж������ƶ�
*****************************************************************************/
#define GUIICONLIST_GetLayoutStyle	CTRLICONLIST_GetLayoutStyle

/*****************************************************************************//*!
@brief  �޸�Iconlist�����塢��ɫ��Ϣ
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] �ؼ�id
@param  iconlist_text_style_ptr	    [in] iconlist��������Ϣ�޸�
@note   ��������ã���Ĭ��Ϊiconlist���������ɫ
*****************************************************************************/
#define GUIICONLIST_SetIconListTextInfo	CTRLICONLIST_SetIconListTextInfo

/*****************************************************************************//*!
@brief  Set iconitem margin & space
@author xiyuan.ma 2011.12
@param  ctrl_id	                                [in] �ؼ�id
@param  GUIICONLIST_MARGINSPACE_INFO_T	        [in] �߾ࡢ���
@note   ��������ã���Ĭ��Ϊiconlist���м�ࡢ�м�ࡢ�������ұ߾�
*****************************************************************************/
#define GUIICONLIST_SetIconItemSpace	CTRLICONLIST_SetIconItemSpace

/*****************************************************************************//*!
@brief  ����item��icon�Ƿ�֧������
@author xiyuan.ma 2011.12
@param  Is_ItemIcon_Zoom	        [in] �Ƿ������
@note   
*****************************************************************************/
#define GUIICONLIST_SetItemIconZoom	CTRLICONLIST_SetItemIconZoom

/*****************************************************************************/
//  Description : �û����Զ���Item��Border����ɫ����񡢴�ϸ 
//  Author: xiyuan.ma 2012.1
//  param   is_select_border	        [in]���õ���select border����unselect
//  param   select_border	            [in]border����ɫ����ϸ�����
//  Note:   is_select_border == TRUEʱ�� ��������select border��
//          is_select_border == FALSEʱ����������unselect border�� 
/*****************************************************************************/
#define GUIICONLIST_SetItemBorderStyle	CTRLICONLIST_SetItemBorderStyle

#ifdef GUIF_ICONLIST_DELIMITER
/*****************************************************************************//*!
@brief  ���÷ָ����ı���
@author xiyuan.ma 2011.12
@param  ctrl_id	       [in] �ؼ�id
@param  *bg_ptr	       [in] �ָ��������ṹ��ָ��
@note   ��������ã���ʹ��͸��
*****************************************************************************/
#define GUIICONLIST_SetDelimiterBg	CTRLICONLIST_SetDelimiterBg

/*****************************************************************************//*!
@brief  �ָ����ı����հ������С
@author xiyuan.ma 2012.05
@param  ctrl_id	                   [in] �ؼ�id
@param  *delimiter_bg_space	       [in] �ָ��������հ�����ṹ��ָ��
@note   ����Ҫ���÷ָ������������ʵ�ʷָ�������Сʱ������
*****************************************************************************/
#define GUIICONLIST_SetDelimiterBgSpace	CTRLICONLIST_SetDelimiterBgSpace

/*****************************************************************************//*!
@brief  ���÷ָ�����ʾ���ַ�����������Ϣ
@author xiyuan.ma 2011.12
@param  ctrl_id	                    [in] �ؼ�id
@param  *delimiter_text_style_ptr	[in] �ָ������ַ�����������Ϣ
@note   ��������ã���Ĭ��Ϊiconlist���������ɫ
*****************************************************************************/
#define GUIICONLIST_SetDelimiterStrInfo	CTRLICONLIST_SetDelimiterStrInfo

/*****************************************************************************//*!
@brief  ���÷ָ����ĸ߶�
@author xiyuan.ma 2011.12
@param  ctrl_id	            [in] �ؼ�id
@param  delimiter_height	[in] �ָ����ĸ߶�
@note   ��������ã���Ĭ�ϸ߶�Ϊtheme�е�delimiter�ĸ߶�
*****************************************************************************/
#define GUIICONLIST_SetDelimiterHeight	CTRLICONLIST_SetDelimiterHeight

/***********************************************************************************//*!
@brief  ����iconlist��ÿ�����Ϣ
@author xiyuan.ma 2011.12
@param  *iconlist_ctrl_ptr	[in] �ؼ�ָ��
@param  *group_info_ptr	    [in] �ָ����ṹ��ָ��
@note   ��Ҫ����start_item_index(��ʼitem index)��delimiter_str_info(�ָ����ڵ��ַ���)
        ����ʱ���밴�մ�С�����˳�����start_item_index
***********************************************************************************/
#define GUIICONLIST_CreateGroup	CTRLICONLIST_CreateGroup

/*****************************************************************************//*!
@brief  ɾ��ĳ���ָ���
@author xiyuan.ma 2011.12
@param  index	                [in] group id
@param  ctrl_id	                [in] �ؼ�  id
@note   ֻ����ɾ��ĳ���ָ���������ɾ���κ�Ԫ��
*****************************************************************************/
#define GUIICONLIST_RemoveGroup	CTRLICONLIST_RemoveGroup

/*****************************************************************************//*!
@brief  remove all group
@author xiyuan.ma 2012.03
@param  ctrl_id	                [in] �ؼ�  id
@note   
/*****************************************************************************/
#define GUIICONLIST_RemoveAllDelimiter	CTRLICONLIST_RemoveAllDelimiter

/*****************************************************************************//*!
@brief  ��ȡdelimiter group num
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@note   ��ȡ�ָ���������
*****************************************************************************/
#define GUIICONLIST_GetDelimiterGroupNum	CTRLICONLIST_GetDelimiterGroupNum
#endif

/*****************************************************************************//*!
@brief  �����Ƿ���ʾsoftkey
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@param  is_true	                [in] �Ƿ��û��Ի�
@note   TURE��ʱ���û��Ի�
*****************************************************************************/
#define GUIICONLIST_SetDispCustomerSoftkey	CTRLICONLIST_SetDispCustomerSoftkey

/*****************************************************************************//*!
@brief  iconlist�Ƿ�֧�ֻ���
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�  id
@param  is_support_slide	    [in] �Ƿ�֧�ֻ���
@note   ��Ϊ��֧��
*****************************************************************************/
#define GUIICONLIST_SetSlideState	CTRLICONLIST_SetSlideState

/*****************************************************************************/
//  Description : set icon list cache page num to cache icon thumbnail
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetIconCacheNum	CTRLICONLIST_SetIconCacheNum

/*****************************************************************************//*!
@brief  �û����Զ���Ĭ��ͼ��Errorͼ
@author xiyuan.ma 2012.02
@param  ctrl_id	                [in] �ؼ�    id
@param  def_img_id	            [in] Ĭ��ͼ  id
@param  err_img_id	            [in] Errorͼ id
@note
/*****************************************************************************/
#define GUIICONLIST_SetDefaultIconByPtr	CTRLICONLIST_SetDefaultIconByPtr

/*****************************************************************************/
//  Description : ��װ����ӿ�---get icon list relative height,except border
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
#define GUIICONLIST_GetRelativeHeight	CTRLICONLIST_GetRelativeHeight

/*****************************************************************************/
//  Description : ��װ����ӿ�---get icon list display rect height
//  Global resource dependence : 
//  Author: arvin.zhang
//  Note:
/*****************************************************************************/
#define GUIICONLIST_GetDispHeight	CTRLICONLIST_GetDispHeight

/*****************************************************************************/
//  Description : �������ع����� ������������Ϊ0
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
#define GUIICONLIST_SetHideSlideBar	CTRLICONLIST_SetHideSlideBar

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/



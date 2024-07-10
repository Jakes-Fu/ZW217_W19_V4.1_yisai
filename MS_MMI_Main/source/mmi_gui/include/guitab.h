/*! @file guitab.h
@brief ���ļ�������tab�ؼ��Ķ���ӿں��������� 
@author Guanyong.Zhang
@version 1.0
@date 06/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 
-# 2007        Guanyong.Zhang     Create                                   *
-# 10/2008        Xiaoqing.Lu        Modified    
*******************************************************************************/

/*! @page page_Tab ѡ�(Tab)

-# @ref sec_tab_function
-# @ref sec_tab_def
-# @ref sec_tab_ctreate
    -# @ref ssec_tab_ctreate_win
    -# @ref ssec_tab_ctreate_cb
    -# @ref ssec_tab_ctreate_append
    -# @ref ssec_tab_ctreate_win2
    -# @ref ssec_tab_ctreate_run
    -# @ref ssec_tab_ctreate_function_list
-# @ref sec_tab_function_list
-# @ref sec_tab_dynamic_data
-# @ref sec_tab_app

@section sec_tab_function �ؼ�����

TAB�ؼ��ֱ�����ѡ��ؼ�������ͬ��Ӧ�ý���򴰿ڹ�����һ��ΪAPP�ṩ��Ϊ����Ľ�����ơ�

��Ҫ�����������£�
-# ������ͬ��ѡ���Item Card����
-# ��ͨ���л�ѡ��л���ͬ���û����棻
-# APP�ɸ�����Ҫ��̬����ѡ��Ķ��٣�
-# APPʹ��TAB�ؼ����ֲ�ͬ�Ľ��档

�ؼ������ʱ�����ؼ���ƺ�ѡ���ƣ��ֱ��������£�
- �ؼ�����
    -# Tab�ؼ������ڴ��ڣ�
    -# Tab�ؼ�ԭ���Ͽ��԰���������ѡ���Item Card����Ŀǰƽ̨���֧��20�
    -# ͨ��Tabѡ���Item Card���л����ڣ�ÿ��ѡ���Ӧһ���Ӵ���Child Window����
    -# ������Tab�ؼ�����ʾ����Tab-rect����
    -# Tab�ؼ���ʾ����Item card��Ӧ�����֣���ѡ���Caption��
    -# Tab�ؼ����������л�Item���������һҳ��ʾ������������ͨ�����Ұ����л���һ��ѡ�
    -# Tab�ؼ��������������л���ѡ���Left-Card ��Right-Card����������������һҳʱ���л�����Ч��������һҳʱ����ͨ�����ʵ������ѡ������л�ѡ�
    -# ���Ұ�������ʱ�ܹ������л�ѡ���
    -# Tab�ؼ���ѡ��������ʾ��
    -# Tab�ؼ���ʾ���ӵ��ַ��������������ʾ��ǰ�Ӵ����е��б������ȣ�������Ϣ��Ӧ�����á�
- �ؼ�ѡ�����
    -# �û��ܹ�����ѡ���ȱʡ�߶ȺͿ�ȣ����ȱʡֵ����Сֵ��
    -# ѡ���Caption�������������ݡ�������ɫ����ʾ������ʾ�����ࡢ���С��Ҳࣩ��
    -# ѡ�ͼƬ������ϵͳ��ԴͼƬID��ϵͳ��Դ����ID��ͼƬ����buffer��ͼƬ�洢·����
    -# Item card�ڽ���״̬���ܹ���ǽ���״̬ʱ�������֣���Ϊitem card�ǽ���״̬����ʾϵͳ��ԴͼƬIDʱ������״̬��ʾ��Ӧ��ͼƬID�����������item card����״̬����ʾ��Χ�߿򣨱߿���ɫ���Զ��壩
    -# Item card�����Ӵ��ڣ�Tab�ؼ�ֻ��ʾ����״̬�µ�ѡ���Ӧ���Ӵ��ڣ�

@section sec_tab_def �궨�����

@code
#define CREATE_TAB_CTRL( _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_) \
        MMK_CREATE_TAB, _TAB_TYPE_, _TAB_STATE_, _CTRL_ID_
@endcode

-���ڴ���ѡ��ؼ���
    - _TAB_TYPE_ TAB�ķ��ʵ����ֻ֧�ֿ������ʾ�����ֶο��Ժ���
    - _TAB_STATE_ TAB��״̬��һ��ΪGUITAB_STATE_NULL���������Ҫ����ѡ�����ô������GUITAB_STATE_NONEED_SCROLL״̬��
    - _CTRL_ID_ �ؼ�ID

@section sec_tab_ctreate �ؼ�����

���������������ʹ��Tab�ؼ����ڴ����ؼ��Ĺ����У����ǽ������۸����ڡ��Ӵ��ڵĸ��

������Tab�ؼ���������ѡ���Item������������ѡ�һʱ����ʾ��Hello SPRD1������������ѡ���ʱ����ʾ��Hello SPRD2����

���ȴ����������ļ���sample_tab_ctrl.c��sample_tab_ctrl.h�����ֱ������sampleģ�����Ӧc��h�ļ����ڡ�

@subsection ssec_tab_ctreate_win ��һ������������

-# TAB�ؼ������ڣ�ע����Ҫ��Ӵ��ڵ�ID�Ϳؼ���ID
@code
WINDOW_TABLE( MMI_SAMPLE_TABCTRL_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTabWinMsg),    
    WIN_ID( MMI_TEST_TAB_WIN_ID),

    CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMI_TEST_TAB_CTRL_ID),

    END_WIN
};
@endcode
-# TAB1�Ӵ��ڣ���ѡ�һ�Ĵ��ڣ�ע����Ҫ��Ӵ��ڵ�ID
@code
WINDOW_TABLE( MMI_SAMPLE_TAB1_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab1ChildWindow),    
    WIN_ID( MMI_TEST_TAB1_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode
-# TAB2�Ӵ��ڣ���ѡ����Ĵ��ڣ�ע����Ҫ��Ӵ��ڵ�ID
@code
WINDOW_TABLE( MMI_SAMPLE_TAB2_CHILD_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleTestTab2ChildWindow),    
    WIN_ID( MMI_TEST_TAB2_CHILD_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode

@subsection ssec_tab_ctreate_cb �ڶ�����ʵ�ִ��ڻص�����

-# ʵ�ָ����ڵĻص�����
@code
LOCAL MMI_RESULT_E HandleTestTabWinMsg(
                    MMI_WIN_ID_T     win_id, 
                    MMI_MESSAGE_ID_E msg_id, 
                    DPARAM           param
                    )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_TAB_CTRL_ID);
        // ��Append TAB�ؼ���ѡ�
        AppendTabInfo();
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(win_id);
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
-# ʵ��Tab1�Ӵ��ڻص�����
@code
LOCAL MMI_RESULT_E HandleTestTab1ChildWindow(
                        MMI_WIN_ID_T     win_id, 
                        MMI_MESSAGE_ID_E msg_id, 
                        DPARAM           param
                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
case MSG_OPEN_WINDOW:
    // ������Ϊ��ʵ��softkey�İ�͸Ч�����û�������ʱ���ع�ע
        MMITHEME_GetHalfTransparentSoftkeyImg(&img_info);
        GUIWIN_SetHalfTransparent(TRUE, win_id, &img_info);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '1', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);
            // ����ʵ����ʾ�ַ���
            DisplayString(w_text, len);
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
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
-# ʵ��Tab2�Ӵ��ڵĻص�����
@code
LOCAL MMI_RESULT_E HandleTestTab2ChildWindow(
                  MMI_WIN_ID_T     win_id, 
                  MMI_MESSAGE_ID_E msg_id, 
                  DPARAM           param
                  )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUISOFTKEY_IMG_T    img_info = {0};
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        // ������Ϊ������softkey��͸��Ч��������ʱ���ع�ע
        MMITHEME_GetHalfTransparentSoftkeyImg(&img_info);
        GUIWIN_SetHalfTransparent(TRUE, win_id, &img_info);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_FULL_PAINT:
        {
            wchar w_text[] = { 'H', 'e', 'l', 'l', 'o', ',', 'S', 'P', 'R', 'D', '2', 0 };
            uint16 len = MMIAPICOM_Wstrlen(w_text);
            // ����ʵ����ʾ�ַ���
            DisplayString(w_text, len);
        }
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(MMI_TEST_TAB_WIN_ID);
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

@subsection ssec_tab_ctreate_append Appendѡ�����ʾ�ַ����ӿڵ�ʵ��

-# Append�ӿ�ʵ��
@code
LOCAL void  AppendTabInfo(void)
{
MMI_STRING_T    str_text;
// ����ѡ��������Ϊ2������ѡ��ڴ�
    GUITAB_SetMaxItemNum(MMI_TEST_TAB_CTRL_ID, 2);
    // ��ȡѡ�1�����ƣ�ע���ַ�����Դ����Ҫ���
    MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB1_TEXT, &str_text);
    // Appendѡ�1����Ϣ�����ơ�ѡ����º͵���ʱ��ͼƬ��ע��ͼƬ��Դ��Ҫ���
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB1_RELEASED);
    // ����ѡ�1��Ӧ�Ĵ���ID
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB1_CHILD_WIN_ID, 0);
    // ��ȡѡ�2�����ƣ�ע���ַ�����Դ����Ҫ���
MMI_GetLabelTextByLang(TXT_HELLO_TEST_TAB2_TEXT, &str_text);
// Appendѡ�2����Ϣ�����ơ�ѡ����º͵���ʱ��ͼƬ��ע��ͼƬ��Դ��Ҫ���
    GUITAB_AppendSimpleItem(
        MMI_TEST_TAB_CTRL_ID,
        &str_text,
        IMAGE_TAB_SELECT1,
        IMAGE_TEST_TAB2_RELEASED);
    // ����ѡ�2��Ӧ�Ĵ���ID
    GUITAB_AddChildWindow(MMI_TEST_TAB_CTRL_ID, MMI_TEST_TAB2_CHILD_WIN_ID, 1);
    // ���õ�ǰ�����ѡ�����һ��һ��Ҫ���ã�����Ҫ
GUITAB_SetCurSel(MMI_TEST_TAB_CTRL_ID, 0);
// ���ý��㴰�ڣ�������㴰��һ��Ҫ������ĵ�ǰѡ���Ӧ
MMK_SetChildWinFocus(MMI_TEST_TAB_WIN_ID, MMI_TEST_TAB1_CHILD_WIN_ID);
}
@endcode
-# ��ʾ�ַ����ӿ�ʵ��
@code
LOCAL void DisplayString(
                         uint16 *wstr_ptr, 
                         uint16 len
                         )
{
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T  start_point     = {1,140};
    GUI_RECT_T   rect            = {0, MMI_TAB_CHILD_WINDOW_TOP, MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_CLIENT_RECT_BOTTOM};
    MMI_STRING_T    text_str  = {0};
    GUISTR_STYLE_T  text_style = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_ALIGN;

    text_str.wstr_ptr = wstr_ptr;
    text_str.wstr_len = len;

    text_style.align = ALIGN_LEFT;
   text_style.font = SONG_FONT_16;
    text_style.font_color = MMI_BLACK_COLOR;
    
    GUI_FillRect(&lcd_dev_info,rect,MMI_WHITE_COLOR);

    GUISTR_DrawTextToLCDSingleLine( 
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                (const GUI_POINT_T       *)& start_point,
                (const MMI_STRING_T     *)& text_str,
                &text_style,
                text_state
                );
}
@endcode

@subsection ssec_tab_ctreate_win2 ���Ĳ�����������

@code
PUBLIC void MMIHELLO_CreateTabWin(void)
{
    // ����������
MMK_CreateParentWin((uint32*)MMI_SAMPLE_TABCTRL_WIN_TAB, PNULL);
// �����Ӵ���1
MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB1_CHILD_WIN_TAB, NULL);
// �����Ӵ���2
MMK_CreateChildWin(MMI_TEST_TAB_WIN_ID, (uint32*)MMI_SAMPLE_TAB2_CHILD_WIN_TAB, NULL);
// ���õ�ǰ�Ľ��㴰�ڣ���һ������Ҫ���������ǰ��ʹ��GUITAB_SetCurSel���õĵ�ǰ���Ӧ�Ĵ�����ͬһ�����ڣ�������ʾ���ܻ���ҡ�
MMK_SetChildWinFocus(MMI_TEST_TAB_WIN_ID, MMI_TEST_TAB1_CHILD_WIN_ID);
}
@endcode

@subsection ssec_tab_ctreate_run ���岽��������Դ����������ģ����

����ʹ�õ���Դ�Լ��ؼ��ʹ��ڵ�ID����ǰ����ӷ�ʽ��ͬ��

��hello sprdӦ�ô���������Ϣ����
@code
case MSG_APP_3:
        MMIHELLO_CreateTabWin();
        break;
@endcode
����hello sprd���ڰ����3����tab���Դ��ڡ��������ͼ��
@image html tab_test_win.jpg
ͼ 2 31 �ؼ���ʾ��ͼ

�����ڸô��ڲ������ҷ������ʱ�򴰿ڽ�����ʾ��Hello SPRD1���͡�Hello SPRD2�������л�����Ҳ�����ô��ʵ��ѡ����л����ڡ�

@subsection ssec_tab_ctreate_function_list ���������ⲿ�ӿڽ��ܡ����Ӵ��ں͸�����

��ʹ��Tab�ؼ�ʱ�������õ��˼���MMK�Ľӿ�
- MMK_CreateParentWin���������������ڵĺ���������MMK_CreateWin�Ĺ�����ȫһ����ֻ�����Ʋ�ͬ��ֻ��ʵ����MMK_CreateWin�ķ�װ��֮����ȡ��ͬ�����ƣ���Ŀǰƽ̨��������Ϊ�˸�Ϊ�����������ָ��Ӵ��ڵĸ��
- MMK_CreateChildWin�ӿ����������Ӵ��ڣ���MMK_CreateWin��Ƚϣ�������һ�������Ÿ����ڵľ���������Ӵ��ڵĴ����ǲ��ᷢ��ˢ������Ϣ�ģ����仰˵���Ӵ��ڵ�ˢ�º͸����ڵ�ˢ��������أ��������ڽ���Full Paint��Ϣ��ʱ�����Ὣ����Ϣת�������ڽ���״̬���Ӵ��ڣ��Ӷ�ʵ���Ӵ��ڵ�ˢ�¡�
- MMK_SetChildWinFocus�ӿ����������ø����������ĳһ���Ӵ��ڴ��ڽ���״̬��
T
AB�ؼ��������Ӵ��ڶ������ڸ����ڣ�TAB�ؼ���ѡ��ֻ��¼�˸�ѡ���Ӧ���Ӵ��ڵ�ID�����л�ѡ���ʱ��TAB�ؼ������͵���MMK_WinGrabFocus�ӿ����л����ڵĽ��㣬MMK_WinGrabFocus�ɿؼ����ã��û����ع��ġ��û����ĵ������ǵ��л���ĳһ���Ӵ��ڵ�ʱ������Ӵ��ڸ��������顣

@note TAB�Ӵ��ڿ�����ͬһ������Ҳ�����ǲ�ͬ�Ĵ��ڣ��Ӵ����п��Դ�������ؼ���

@section sec_tab_function_list �ؼ��ӿڽ���

-# @ref GUITAB_SetMaxItemNum
-# @ref GUITAB_AppendSimpleItem
-# @ref GUITAB_AddChildWindow
-# @ref GUITAB_SetCurSel
-# @ref GUITAB_AppendItem

@section sec_tab_dynamic_data ��̬��������

TAB�ؼ�����֧�־�̬����ԴͼƬ������ѡ�������ͬ���ǵ���������һ�����û�ͨ���ӿ�GUITAB_AppendSimpleItem������ѡ�����Ϣ���������ͼƬIDΪ��̬��ԴͼƬ��

TAB�ؼ�Ҳ����֧��һЩ��̬ͼƬ���ݣ�������Դ�u�̶�ȡͼƬ��Ϣ����Ϊѡ���ͼƬ���ݡ�
ǰ��˵����TAB�ؼ����֧�ֵ�ѡ���Ϊ20�����ټ���ѡ���ͼƬ��Ϣ�Ƕ�̬�����ݣ�����ܻ����ͬʱ������ڴ���������µ���GUITAB_AppendSimpleItem�ӿ�ʧ�ܣ�Ϊ�������ṩ�˶�̬����ѡ����ݵķ����������ǵ�Listbox�ؼ�����Listbox�ؼ��Ĵ���������Ҳ���������Ƶ����⣬ͬ��������Ҳ���õĶ�̬�������ݵķ�����

����û�ʹ�ö�̬�������ݵķ�ʽ����ô����ʹ��GUITAB_AppendItem�ӿ����Item��������Ϣ���ýӿڴ������GUITAB_ITEM_T�����ݽṹ�������£�

-# GUITAB_IMAGE_DATA_U
-# GUITAB_IMAGE_DATA_T
-# GUITAB_ITEM_CONTENT_T
-# GUITAB_ITEM_T

Tab�ؼ���Item�������ʾͼƬ���ı��������߶���ʾ��

��ν�Ķ�̬�������ݣ�ʵ�����Ƕ�̬����ͼ�����ݣ����ı���������û�Ҫ��ʾ���������append��ʱ������롣

���ﲻ�ò�����һ��Item������״̬��
- @ref GUITAB_ITEMSTATE_SHOW_IMG ������ʾͼƬ
- @ref GUITAB_ITEMSTATE_SHOW_TEXT ������ʾ�ı�

������״̬������Item����ʾ���ͣ�����ѡ����һ��Ҳ�����ԡ��򡱵Ĺ�ϵ�����á�

TAB�ؼ��Ƿ���Ҫ��̬���ز������û������ģ�����TAB�ؼ��Լ������ģ�����ͼƬ������ΪTABITEM_DATA_ANIM_ID��TABITEM_DATA_ANIM_DATA��TABITEM_DATA_ANIM_PATH��ʱ����ô�ؼ���ʹ�ö����ؼ�����ʾ��Щ���͵�ͼƬ�����ؼ��������¼��GUITAB_ITEM_T�ṹ�е�add_data_ptr�ֶ�Ϊ0��ʱ�򣬻��Զ�����MSG_CTL_TAB_NEED_ITEM_CONTENT��Ϣ���ؼ��ĸ����ڣ���ע�⣬�Ǹ�������Item���Ӵ���û�й�ϵ��App�ڽ��յ������Ϣ֮�󣬸�����Ϣ����ʱ��Я���Ĳ���������GUITAB_SetItemImage�ӿ�������ͼƬ��������Ϣ�Ϳ����ˡ�

������ʵ����ͼƬ���ݵĶ�̬���ء�

TAB�ؼ�ֻ��¼��ǰҳ��Item�Ķ���ָ�룬��һ��Item���ڵ�ǰҳ��ʱ�򣬿ؼ����ͷŶ�Ӧ�Ķ���ָ���ͼƬ������Ϣ���Խ�ʡ�ڴ棬��Item���½��뵱ǰҳ��ʱ�򣬿ؼ����ٴη�����Ϣ��APP�������»�ȡͼƬ������Ϣ��������������

@section sec_tab_app �ؼ�Ӧ��
Tab�ؼ���Ҫʹ�����л�ͬ�����ʹ��ڵĹ����У�����绰�������м�¼����Ϣ�����䡢��Դ�������ȡ�����ͼ���ں��м�¼�е�Ӧ�ã���
@image html tab_app_example.jpg
ͼ 2 32 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup tabGrp Tab
@brief tab�ؼ�
@details tab�ؼ�
@{
*******************************************************************************/

#ifndef _GUITAB_H_
#define _GUITAB_H_ 

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
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
#include "ctrltab_export.h"
/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*/
/*                         Macro Declaration                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#define GUITAB_SetMaxItemNum       CTRLTAB_SetMaxItemNum
#define GUITAB_AddChildWindow      CTRLTAB_AddChildWindow
#define GUITAB_AppendSimpleItem    CTRLTAB_AppendSimpleItem
#define GUITAB_InsertItem          CTRLTAB_InsertItem
#define GUITAB_DeleteItem          CTRLTAB_DeleteItem
#define GUITAB_DeleteAllItems      CTRLTAB_DeleteAllItems
//#define GUITAB_GetRect             CTRLTAB_GetRect
//#define GUITAB_SetRect             CTRLTAB_SetRect
#define GUITAB_GetItemNum          CTRLTAB_GetItemNum
#define GUITAB_SetSimpleItem       CTRLTAB_SetSimpleItem
#define GUITAB_SetCurSel           CTRLTAB_SetCurSel
#define GUITAB_GetCurSel           CTRLTAB_GetCurSel
#define GUITAB_GetCurItem          CTRLTAB_GetCurItem
#define GUITAB_SetBackGroundImage  CTRLTAB_SetBackGroundImage
#define GUITAB_SetBackGroundColor  CTRLTAB_SetBackGroundColor
#define GUITAB_SetFontParam        CTRLTAB_SetFontParam
#define GUITAB_SetAddedString      CTRLTAB_SetAddedString
#define GUITAB_GetAddedString      CTRLTAB_GetAddedString
#define GUITAB_SetState            CTRLTAB_SetState
#define GUITAB_GetState            CTRLTAB_GetState
#define GUITAB_SetItemState        CTRLTAB_SetItemState
#define GUITAB_GetItemState        CTRLTAB_GetItemState
#define GUITAB_SetImageState       CTRLTAB_SetImageState
#define GUITAB_GetImageState       CTRLTAB_GetImageState
#define GUITAB_SetItemContent      CTRLTAB_SetItemContent
#define GUITAB_SetItemImage        CTRLTAB_SetItemImage
#define GUITAB_SetItemText         CTRLTAB_SetItemText
#define GUITAB_SetAutoSwitch       CTRLTAB_SetAutoSwitch
#define GUITAB_GetAutoSwitch       CTRLTAB_GetAutoSwitch
#define GUITAB_SwitchToNextItem    CTRLTAB_SwitchToNextItem
#define GUITAB_AppendItem          CTRLTAB_AppendItem
#define GUITAB_SetItemSpace        CTRLTAB_SetItemSpace
#define GUITAB_SetItemWidth        CTRLTAB_SetItemWidth
#define GUITAB_SetTabRotative      CTRLTAB_SetTabRotative
#define GUITAB_SetDefaultIcon      CTRLTAB_SetDefaultIcon
#define GUITAB_SetItemValid        CTRLTAB_SetItemValid
#define GUITAB_SetFocusItemBgColor CTRLTAB_SetFocusItemBgColor
#define GUITAB_SetFocusItemTextBgImage CTRLTAB_SetFocusItemTextBgImage
#define GUITAB_SetStartIndex       CTRLTAB_SetStartIndex
#define GUITAB_GetStartIndex       CTRLTAB_GetStartIndex
#define GUITAB_SetSyncDecodeAnim   CTRLTAB_SetSyncDecodeAnim
#define GUITAB_SetItemDefaultHeight CTRLTAB_SetItemDefaultHeight
#define GUITAB_SetItemSelBgImage   CTRLTAB_SetItemSelBgImage
#define GUITAB_SetTitleInfo        CTRLTAB_SetTitleInfo
#define GUITAB_SetTitleTextId      CTRLTAB_SetTitleTextId
#define GUITAB_SetTitleIcon        CTRLTAB_SetTitleIcon
#define GUITAB_SetDividingLine     CTRLTAB_SetDividingLine
#define GUITAB_SetTpDownBg         CTRLTAB_SetTpDownBg
#define GUITAB_SetIsDisplayPrg     CTRLTAB_SetIsDisplayPrg

/***************************************************************************//*!
@brief ��ȡ�ؼ�������
@author guanyong.zhang
@param ctrl_id [in] �ؼ�ID
@retval ����
@return ��ÿؼ�����
@note 
*******************************************************************************/
PUBLIC GUI_RECT_T GUITAB_GetRect(
                                 MMI_CTRL_ID_T  ctrl_id
                                 );

/***************************************************************************//*!
@brief set tab rect
@author Jassmine
@param ctrl_handle [in] �ؼ�ID
@param rect_ptr [in] �ؼ�ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void GUITAB_SetRect(
                           MMI_HANDLE_T     ctrl_handle,
                           GUI_RECT_T       *rect_ptr
                           );

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

/*@}*/

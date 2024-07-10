/*! @file ctrllistbox.h
@brief ���ļ�������listbox�ؼ��Ķ���ӿں���������
@author Great.Tian
@version 1.0
@date 24/07/2012
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 24/07/2012     Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_ListBox �б�(ListBox)

-# @ref sec_listbox_function
-# @ref sec_listbox_usage
    -# @ref ssec_listbox_def
    -# @ref ssec_listbox_create
    -# @ref ssec_listbox_type_def
    -# @ref ssec_listbox_example
    -# @ref ssec_listbox_struct
    -# @ref ssec_listbox_append
      -# @ref sssec_listbox_append_setmax
      -# @ref sssec_listbox_append_item
      -# @ref sssec_listbox_append_data
    -# @ref ssec_listbox_get_index
    -# @ref ssec_listbox_type_note
    -# @ref ssec_listbox_softkey
    -# @ref ssec_listbox_title
    -# @ref ssec_listbox_layout
-# @ref sec_listbox_app

@section sec_listbox_function �ؼ��Ĺ���

�б�ؼ���Listbox���Ǵ��ڻ�������֮һ���б�ؼ��ܽ��û������Ա�����ʽ��ʾ��������Ҫ֧��һ�¹��ܣ�
-# ֧�ֵ�����ʾ��˫����ʾ���б�
-# ����ʾ�����б���ͼ����б�
-# ��ѡ���б�
-# ��ѡ���б�
-# �б�����Ա�ѡ�У�
-# ֧�ֹ����������б����ÿҳ����ʾ���б���Ŀʱ����Ҳ��ʾ���ڲ�����ӹ�������Ϊ�û��ṩ���¹�������ҳ���ܡ�
-# �б��item�����ַ�����һ��ʱ�ܹ�������ʾ��
-# ֧��Get/Set�ӿڣ������û���ȡ/�����б�ؼ������ԡ�

@section sec_listbox_usage �ؼ���ʹ��

@subsection ssec_listbox_def �궨�����

@code
#define CREATE_LISTBOX_CTRL(_LIST_TYPE_, _CTRL_ID_) \
        MMK_CREATE_LISTBOX, _LIST_TYPE_, _CTRL_ID_
@endcode
- ���ڴ���һ��List Box�ؼ���
    - _CTRL_ID_ ��ǰ�ؼ�ID��ΪMMI_CONTROL_ID_Eö������
    - _LIST_TYPE_ List���ͣ�ΪGUI_LISTBOX_TYPE_Eö�����ͣ��б�������е�ѡ��ť�б���ѡ���б��ı��б��������͡�

@subsection ssec_listbox_create ��δ����ؼ�

ƽ̨�У����еĿؼ��ھ�̬����ʱ�Ĵ�����ʽ������ͬ�ġ��������б�Ϊ������ʾЧ������ͼ��
@image html listbox_example_alarm.jpg
ͼ 2 17���Ӵ����б�ؼ�

�������Ӵ���Table�������£�Դ���������ƽ̨��mmialarm.c�ļ����ҵ�����
@code
WINDOW_TABLE( ALARM_MAINWIN_TAB ) =
{
    WIN_FUNC((uint32)HandleAlarmMainWinMsg ),
    WIN_ID( MMIALM_MAINWIN_ID ),
    WIN_TITLE(TXT_ALARM_CLOCK),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIALM_LISTALL_CTRL_ID),
    END_WIN
};
@endcode

��������б������Ԫ���д��ڻص�����ָ�롢����ID������Title������Softkey���б�ؼ���Ԫ�ء����ﴴ�����б�ؼ�������ΪGUILIST_TEXTLIST_E��������ͨ���͵�list�ؼ���

@subsection ssec_listbox_type_def List�ؼ������Ͷ��壺

- @ref GUILIST_TYPE_E

@subsection ssec_listbox_example �����б�ؼ�����

�������Ǿٵ������б�Ŀؼ������Ӳ������ƣ����źܶ���Ҳ����ȥ�鿴����ģ���Դ���룬�����ﻹ�Ǿ�һ���򵥵Ĵ����б�ؼ������Ӱɡ�

- ��һ������������Table
@code
WINDOW_TABLE( MMI_TEST_LIST_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleTestListWinMsg ),
    WIN_ID( MMI_TEST_LIST_WIN_ID ),
    WIN_TITLE(TXT_HELLO_TEST_LIST),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(
                    GUILIST_TEXTLIST_E,
                    MMI_TEST_LIST_CTRL_ID
                    ),
     END_WIN
};
@endcode
@note ����û�пؼ�λ����Ϣ����Ϊ���ڴ���ʱ��ϵͳ���Զ���title��softkey��list�ؼ����������úã����Ҵ�ʱ��list����ʵ�ʾ��Ǵ��ڵĿͻ�����
- �ڶ�������ӿؼ��ʹ��ڵ�ID
���ǵ�������Ȼ��sampleģ�飬���ںͿؼ���ID��Ȼ������sample_id.h�У��������´��룺
@code
// ����ID
typedef enum
{
    MMIHELLO_WIN_ID_START = (MMI_MODULE_SAMPLE << 16),

    MMI_HELLO_SPRD_WIN_ID,
    MMI_TEST_MENU_WIN_ID,
    MMI_TEST_LIST_WIN_ID,

    MMIHELLO_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

// �ؼ�ID��control ID��
typedef enum
{
    MMIHELLO_CTRL_ID_ID_START = MMIHELLO_MAX_WIN_ID,
    MMI_TEST_MENU_CTRL_ID,
    MMI_TEST_LIST_CTRL_ID,
    MMIHELLO_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;
@endcode
- �������������ַ�����Դ
�������Ǵ������б�ؼ��а�������ѡ���Ҫ�õ��ַ�����Դ�����⣬���ڵ�TitleҲ��Ҫһ���ַ�����softkey���ǽ�ʹ��ƽ̨�й��еļ����ַ����Ϳ����ˡ�
@code
��str_table.xls�ļ����������¼��
TXT_HELLO_TEST_LIST 20  Test List   �б����
TXT_HELLO_TEST_LIST1    20  Test List1  Test List1
TXT_HELLO_TEST_LIST2    20  Test List2  Test List2
TXT_HELLO_TEST_LIST3    20  Test List3  Test List3
Ȼ����sample_mdu_def.h�ļ����������´��룺
RES_ADD_STRING(TXT_HELLO_TEST_LIST,"Test menu 2")
RES_ADD_STRING(TXT_HELLO_TEST_LIST1,"Test menu 3")
RES_ADD_STRING(TXT_HELLO_TEST_LIST2,"Test Menu")
RES_ADD_STRING(TXT_HELLO_TEST_LIST3,"Test menu 2")
@endcode
������Դ�����ɡ�
- ���Ĳ������Ӵ��ڴ�����
@code
LOCAL MMI_RESULT_E HandleTestListWinMsg(
                                        MMI_WIN_ID_T     win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM           param
                                        )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_LIST_CTRL_ID;

    lcd_dev_info.lcd_id     = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id   = GUI_BLOCK_MAIN;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMI_TEST_LIST_CTRL_ID);
        GUILIST_SetMaxItem(ctrl_id, MMI_HELLO_LIST_NUM, FALSE);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_CALL, TXT_HELLO_TEST_LIST1, STXT_OK, STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_PHONE, TXT_HELLO_TEST_LIST2, STXT_SELECT,STXT_RETURN);
        AppendListItem(ctrl_id, IMAGE_SECMENU_ICON_SET_DISPLAY, TXT_HELLO_TEST_LIST3, STXT_ANSWER, STXT_RETURN);
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_OK:
        break;

    case MSG_KEYDOWN_CANCEL:
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
����Ҫע��GUILIST_SetMaxItem �����˿ؼ��������Ϊ3��ʹ�ú궨��ĳ�����AppendListItem�ӿ�����Append�б������ݣ�ʵ�����£�
@code
LOCAL void AppendListItem(
                          MMI_CTRL_ID_T    ctrl_id,
                          MMI_IMAGE_ID_T   image_id,
                          MMI_TEXT_ID_T    text_id,
                          MMI_TEXT_ID_T    left_softkey_id,
                          MMI_TEXT_ID_T    right_softkey_id
                          )
{
    CTRLLIST_ITEM_T          item_t     = {0};
    GUILIST_ITEM_DATA_T     item_data  = {0};
    uint32                  item_index = 0;
    // �����б���Ϊ���д���һ��icon��text������
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    // �����б������ݵ�����
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.text_id = image_id;
    // �����б������ݵ�����
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[1].item_data.text_id = text_id;
    // ���ö�Ӧ��softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    // append �б���
    GUILIST_AppendItem(ctrl_id, &item_t);

    return;
}
@endcode
- ���岽�����벢����ģ����
�ڱ���֮ǰ��������Ϊ���������Ѱ��һ����ڣ�����ʵ��һ���ӿڣ�
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateListWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_LIST_WIN_TAB, PNULL);
}
@endcode
���ǵ�hello sprd�Ǹ�С����ɣ����Ǿ����Ǹ����ڴ��������������´��룺
@code
case MSG_APP_DOWN:
        MMIHELLO_CreateListWin();
        break;
@endcode

Ȼ����������ļ���ӵ�ģ���Ӧ��ͷ�ļ���Դ�ļ��Ķ�Ӧ��λ�ã�����vc����������sample_test_ctrl.c�ļ���Ȼ���������ģ��������idld���ڰ������������hello sprd���ڣ�Ȼ�����µ�����Ϳ��Եõ����´����ˣ�
@image html listbox_test_win.jpg
ͼ 2 18�б�ؼ����Դ���

���ʱ�����Ű����¼��ƶ���꣬��ᷢ��softkeyҲ����֮�仯�ģ���Ϊ����֮ǰΪÿ��item���������Լ���softkey�ַ�����

@subsection ssec_listbox_struct �б������ݽṹ���

�б�ؼ����б������ݽṹ�Ƚϸ��ӣ����������ʱ���������ǿؼ�����ʾ������⣬��Ҫ����ϵͳ�ڴ�Ĵ�С�������б�����Ŀ����ģ������б���ܶ���б�ؼ����㲻����һ��������ܴ�Ķ�̬�ڴ������������б������ݣ����ǲ��õķ�ʽ�Ƕ�̬�������ݵķ�ʽ�����б�������ʾ��ʱ���ȥ��APP��ȡҪ��ʾ�����ݣ����Ǻ󻰣����ǻ���������һ���б������ص����ݽṹ��

-# @ref GUIITEM_DATA_TYPE_E
-# @ref GUIITEM_STATE_T
-# @ref GUILIST_STATE_T
-# GUIITEM_DATA_TEXT_BUFFER_T
-# @ref GUIITEM_DATA_U
-# @ref GUIITEM_CONTENT_T
-# @ref GUILIST_ITEM_DATA_T
-# @ref CTRLLIST_ITEM_T

@subsection ssec_listbox_append ���Append�б���

�ڴ���Table�������б�ؼ��ĺֻ꣬�Ǹ���MMK������ڴ���ʱҪ����һ���б�ؼ���ʵ�������Ҷ����˸��б�ؼ������������Լ��ؼ���ID�����ǲ�û�и��ߴ��������ʾ����ؼ���ʵ������ʾ�ؼ�Ҳ�Ǵ��������˵ģ�����ؼ��Լ�����ɡ���Ҫ����ʾ�б�ؼ���ô���Ǳ����������Ҫ��ʾ�����ݡ�

@subsubsection sssec_listbox_append_setmax �����б�������ֵ
�ڴ��ڴ���ʱ���������������б�ؼ�Ҳ�ᱻͬʱ������������˵������ʱ���������б�ؼ�ֻ��һ���ؼ���ʵ������û�������κ��б���������Ϣ������㲻�����б����������Ϣ����ô�����������б�ؼ�ʲô��������ʾ��ԭ��ܼ򵥣��б�ؼ���ʾ�����ݾ����б������ݡ�

�������б�������֮ǰ��������ȸ�������б�ؼ�����ܰ������б�������ֵ����Ϊ�ؼ���ҪΪ�б�������һ���ε��ڴ�������������Ϣ�������б����������ֵ�ܼ򵥣�ֻ��Ҫ���ýӿ�GUILIST_SetMaxItem������ӿڱ����ڴ��ڵ�Open��Ϣ�е��ã�ԭ��ܼ򵥣����ڵ�Open����Full paint��Ϣ֮ǰ���͵ģ���Full paint��Ϣ��ˢ����Ļ����Ϣ��������ڴ���Ϣ֮�����ã���ô���᲻�����á��е��˿��ܻ��ʣ�Ϊʲô������Full paint��Ϣʱ�����أ�full paint��Ϣ�����ȷ��͸������ٷ��͸��ؼ������ǵģ�������Ҳ�������׳�����ĵط�������ֻ�ܽ��յ�һ��Open ��Ϣ�����ڴ��ڴ�����ʱ����յģ�����ʱ�䲻����յ������Ϣ����Full paint��Ϣ���ܽ��ܺܶ�Σ����細�����»�ȡ�����ʱ�����ÿ�ζ�ȥ�������ã�����Ȼ�����˳����ִ��Ч�ʣ��������������������⡣

-# @ref GUILIST_SetMaxItem

@subsubsection sssec_listbox_append_item Append�б���

��һ��ֻ���������б�ؼ�������������������ڴ�ռ䣬���ǻ�û�������б������ݡ�GUILIST_AppendItem�ӿ�������Append�б������ݵĽӿڣ��ýӿڱ�����GUILIST_SetMaxItem�ӿ�֮����ܵ��ã�ԭ��ܼ򵥣���û�������б������ݵ��ڴ�֮ǰ����Append���κ��б������ݶ������ܳɹ��ġ�Append��ʲô��˼������԰����������ӣ����־��ò�����ô���У����Խ���Ϊ�����/���ӡ��б������ݣ�����Append�ӿ��������ݣ����������ӵģ��ӵ�һ����ʼ˳����ӣ�������Insert�����룩����ô���벻��Ҫ��ֻҪ�����Ϳ����ˡ�

-# @ref GUILIST_AppendItem

@subsubsection sssec_listbox_append_data ���ڶ�̬�������ݺ;�̬��������

�û�����GUILIST_AppendItem�ӿ�ʱ������ӵ��б���������һ��CTRLLIST_ITEM_T���͵����ݣ���������һ���������ݵ�ָ�룬����ΪGUILIST_ITEM_DATA_T*������ṹ�����൱���Size�������û����ϸ��������������ж����ٸ��ֽڰɡ�
��ˣ������б�����ٵĿؼ��������һ���԰����е����ݶ�����ȥ�����������б�ֻ���������ݡ����Ƕ����б���ܶ�����ݣ��㲻���ܰ����е��б�����������һ���������������Ϊ�ǽ�ͬʱռ�úܴ���ڴ棬��������1000��ͼƬ���б�ÿ���б�����Ҫ��ʾͼƬ������ͼ��

Ϊ�ˣ������ṩ���������ݵļ��ط�ʽ��
-# ��̬���أ�
    ��̬���رȽϼ򵥣��ڵ���GUILIST_SetMaxItem����ע�⾲̬����ʱ���һ����������ΪFALSE���ӿ�֮����Ϳ���ֱ�ӵ���GUILIST_AppendItem�ӿڣ��ڵ���GUILIST_AppendItem�ӿ�֮ǰ����CTRLLIST_ITEM_T�е��������ݶ�����ȥ������mmialarm.c��Ϊ�����б�������ݵĽӿ�AppendTwoLineCheckItem�������ﲻ�г����ˣ���鿴Դ���롣
-# ��̬���أ�
    ����ھ�̬����������˵����̬���صĹ��̻Ḵ��һЩ���ڵ���GUILIST_SetMaxItem����ע�⾲̬����ʱ���һ����������ΪTRUE���ӿ�֮���û���Append���ݵ�ʱ��ֻ��Ҫ����CTRLLIST_ITEM_T��ֵ��������Ҫ������е�GUILIST_ITEM_DATA_T*�����ݣ��μ��б������ݽṹ���壩�����������˸������ݣ���������֮ǰGUILIST_SetMaxItem�Ѿ����߿ؼ���̬�����ˣ���ʱ�ؼ�Ҳ������������ݵġ����������б�ؼ���ʾ��ʱ�򣬷��ָ�������Ϊ�գ��ͻᷢ��MSG_CTL_LIST_NEED_ITEM_DATA��Ϣ�����ڣ���Я��GUILIST_NEED_ITEM_DATA_T���͵Ĳ�����

-# @ref GUILIST_NEED_ITEM_DATA_T

���ڽ��յ���Ϣ֮�󣬽�����������ȡ�ؼ���ID������������ţ�Ȼ�����GUILIST_SetItemData�ӿ������������ݡ�
�����Ҫ��ʾ��������GUIITEM_CONTENT_T����������ΪGUIITEM_DATA_ANIM_DATA����GUIITEM_DATA_ANIM_PATHʱ���ڽ��յ�MSG_CTL_LIST_NEED_ITEM_DATA��Ϣʱ����Ҳû�б�Ҫ���Ӷ�Ӧ��GUIITEM_DATA_U�е����ݣ�������ΪGUIITEM_DATA_ANIM_PATHʱ��GUIITEM_DATA_U��������GUIANIM_FILE_INFO_T����Ч�ģ�������ΪGUIITEM_DATA_ANIM_DATAʱ��GUIANIM_DATA_INFO_T��Ч������GUIITEM_DATA_TYPE_Eһ��Ҫ��д�á��б�ؼ�����ʾ������ʱ��������ֶ�����ָ��Ϊ�գ���ô�ᷢ����һ����ϢMSG_CTL_LIST_NEED_ITEM_CONTENT�����ڣ�ȥ��Ҫ���ݣ���Я��GUILIST_NEED_ITEM_CONTENT_T���͵Ĳ�����

-# @ref GUILIST_NEED_ITEM_CONTENT_T

�����ڽ��յ���ϢMSG_CTL_LIST_NEED_ITEM_CONTENTʱ��������������ȡ�����Ϣ������GUILIST_SetItemContent�ӿ����þ���Ķ������ݾͿ����ˡ�

�����̬���صĹ�����Щ���ӣ����ܽ������������¼�����
- ��һ�������ÿؼ��������
- �ڶ�����appendѡ�����ݣ�����ҪЯ�����ݣ�
- ������������MSG_CTL_LIST_NEED_ITEM_DATA��Ϣ�����������ݣ������Ҫ��ʾ��������ʱ������������ݲ���Ҫ��д��������������һ��Ҫ��д��
- ���Ĳ�������MSG_CTL_LIST_NEED_ITEM_CONTENT��Ϣ�������ö������ݵ����ݡ�
ƽ̨�Ĵ���������ص���������ο�mmifmm_wintab.c�ļ��У�MMIFMM_PATH_SDCARD_CHILD_WIN_TAB���ڵĴ���

@subsection ssec_listbox_get_index ��ȡ��ǰ��ѡ�е�������

���б�ؼ������У���������Ҽ������ߴ��ʵ����Ļ����������������ڻ���յ�MSG_CTL_OK��MSG_CTL_CANCEL��Ϣ��ͨ�������������Ҫ�ڴ���OK��Ϣ��ʱ��ȥ��ȡ��ǰѡ��������š�
��ȡ��ǰѡ��������źܼ򵥣�ֻҪֱ�ӵ��ýӿ�GUILIST_GetCurItemIndex�Ϳ����ˣ�����Ĳ����ǿؼ���ID��

Example1:

@code
uint16 list_index = 0;
list_index = GUILIST_GetCurItemIndex(ctrl_id );
@endcode

���ڵ�ѡ���ѡ�б�ؼ������Ե��� @ref GUILIST_GetSelectedItemIndex �ӿڡ�

Example2:

���ڵ�ѡ������Ϊֻ��һ��item��ѡ�У����ԣ�
@code
uint16                      cur_selection = 0;
GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
@endcode

Example3��

���ڶ�ѡ��ͬʱ���ж����ѡ�У��Ѿ����������ѡ����Ϊ3��
@code
uint16 sel_items[3] = {0};
uint16 sel_count = 0;
sel_count = GUILIST_GetSelectedItemIndex(ctrl_id,sel_items, sel_count);
@endcode

Example4��

���ö�ѡ������ѡ���
@code
GUILIST_SetMaxSelectedItem(ctrl_id,3);
@endcode

@subsection ssec_listbox_type_note �����б�ؼ����͵�˵��
- @ref GUILIST_TYPE_E;

@subsection ssec_listbox_softkey �����б�ؼ���softkey

�б�ؼ���ÿһ����Լ���softkey��ʾ���ݣ�ǰ��������ݽṹʱ�����ἰ��������˵��һ�£�����Ҫ��ÿ���б����Ӧ��softkey�����úã������Ҫ��̬�ı�softkey����ʾ����ôҲ���޸��б����е�softkey���ݣ�������ֱ�����ô��ڵ�softkey����Ϊ���ʱ�򴰿ڵ�softkey�����б�ؼ������ơ�

@subsection ssec_listbox_title �����б�ؼ���Title

�б�ؼ����˿���softkey��ʾ������֮�⣬������ƴ��ڵ�Title����ʾ���ݣ�������ʾ�ڼ�����¼����Щ���ǿؼ������Ƶģ��û����Բ鿴guilistbox.c���й�title����ؽӿڡ����ﲻ��Ҫ�Ի���Ϊ�б�ؼ��Ƚ����⣬����Ҫ�Լ����ƺܶ����ݡ�

@subsection ssec_listbox_layout �����б�ؼ���Layout

��νLayout�������б�ؼ���ʾ�����岼�֣�������ʾ���е��б�ؼ�ʱ��Я����СIcon��Text����ʾ�Ű棻������ʾ˫��ʱ����Ű�ȡ�

�б����������GUIITEM_STYLE_E���壬�ö��������mmi_theme.h���ҵ��������˶��ַ�����͡�

�б�ؼ���ʾ��ʱ�������������ȥ��ȡ��صĲ�����Ϣ����Щ������Ϣ�Ƕ���ľ�̬���飬��������s_item_layout_data_table����mmi_theme.c�ж��壬����û���Ҫ�޸�Layout��������ʾ��Icon����ؼ�������ߵľ������������ô����ֱ���޸���������ص�Layout��Ϣ�Ϳ����ˡ�

@section sec_listbox_app �ؼ���Ӧ��

�б���ϵͳ����õĿؼ�֮һ����绰���뱾�����б���Ϣ�б���ǩ�б�mp3���������б�ͼƬ�б������б����뷨�б�ȡ���Щ�б��Ӧ���а�����ѡ����ѡ���ı��б�ȵ�Ӧ�á�ͼ 2 19��ʾ�б�ؼ�����Դ�������е�Ӧ�á�
@image html listbox_app_example.jpg
ͼ 2 19�б�ؼ�����Դ�������е�Ӧ��

*******************************************************************************/

/*! @addtogroup listboxGrp ListBox
@brief listbox�ؼ�
@details listbox�ؼ�
@{
*******************************************************************************/

#ifndef  _CTRLLIST_LISTBOX_H_
#define  _CTRLLIST_LISTBOX_H_

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h"
#include "ctrllist_export.h"
#include "guicommon.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guiprgbox.h"
#include "mmi_theme.h"
#include "mmitheme_list.h"
#include "guianim.h"
#include "guistring.h"
#include "type_manager.h"
#include "ctrlbase.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration                                  */
/*----------------------------------------------------------------------------*/
// list type info
#define CTRL_LIST_TYPE     (LIST_GetType())

#define CTRLLIST_WEAKLAYERALPHA 165

#define SCROLLBAR_WIDTH          (MMITHEME_GetScrollBarWidth()) // �������Ŀ��

#define CTRLLIST_MAX_STRLEN      0xFF //list����֧�ֵ�����ַ�����, ����GUILIST_STRING_MAX_NUM

//list�ڲ�״̬
#define INNER_STATE_SET_ITEM_DATA     0x00000001   //list ��Ҫ�첽��������״̬
#define INNER_STATE_ON_ITEM_DATA      0x00000002   //list ���ڲ���item data

#define INNER_STATE_SET_ITEM_CONTENT  0x00000004   //list ��Ҫ�첽����ͼƬ����״̬
#define INNER_STATE_ON_ITEM_CONTENT   0x00000008   //list ���ڲ���item content
#define INNER_STATE_ON_3D             0x00000010   //list ���ڲ���3dЧ��
#define INNER_STATE_STOP_SWITCH       0x00000020   //list ֹͣ�Զ��л�item
#define INNER_STATE_RESET_ITEM_RECT   0x00000040   //list �Ƿ����¼���item��ʼ����
#define INNER_STATE_CUSTORMER_LAYER   0x00000080   //list �Ƿ����¼���item��ʼ����
#define INNER_STATE_SLIDE_ITEM        0x00000100   //list ������򻬶�item״̬�����������򻬶��ؼ�
#define INNER_STATE_ABLE_SLIDE_ITEM   0x00000200   //list �Ƿ�֧��item���򻬶�
#define INNER_STATE_ABLE_LEFT_SLIDE_ITEM    0x00000400   //list �Ƿ�֧��item���󻬶�,�ú������ INNER_STATE_ABLE_SLIDE_ITEM ����ʹ��
#define INNER_STATE_NEED_HIGHBAR_IF_CLICK   0x00000800   //��item�����ʱ��list��Ҫ������״̬��Ĭ������²���ʾ������

typedef uint32 INNER_UPDATE_STATE_T;
#define INNER_STATE_UPDATE_BG         0x00000001   //list ������
#define INNER_STATE_UPDATE_PRG        0x00000002   //list ��������
#define INNER_STATE_UPDATE_SK         0x00000004   //list ��Softkey
#define INNER_STATE_UPDATE_TITLE      0x00000008   //list ��Title
#define INNER_STATE_UPDATE_ITEM       0x00000010   //list ��Item

// list item�ڲ�״̬
#define INNER_STATE_TO_UPDATE_HEIGHT  0x10000000   //list ��Item
#define INNER_STATE_UPDATE_HEIGHT_END 0x20000000   //list ��Item

// invalid value
#define INVALID_ITEM_INDEX            0xFFFF


#define INNERLIST_ITEM_NUMBER         30

//list��tip�ַ��Զ��������ʵ�����
#define LIST_TIP_FONT_FACTOR_LIMIT (10)

//list�ķ�����
#define LIST_GROUP_SPACE DP2PX_VALUE(10)
//list��������������ʾ���
#define LSIT_VIBRATE_TIMER_OUT      (200)
#define LIST_SLIDE_TIP_HIDE_TICK    (200)

#define MMICOM_LIST_SLIDE_ICON_WIDTH_HEIGHT DP2PX_VALUE(24)
#define MMICOM_LIST_SLIDE_ICON_MARGIN       DP2PX_VALUE(8)
#define MMICOM_LIST_SLIDE_ICON_NUM          (2)

typedef uint8 CTRLLIST_STYLE_TD_E;
enum{	
	CTRLLIST_STYLE_NONE = 0,	
	CTRLLIST_STYLE_WECHAT_MINE,
	CTRLLIST_STYLE_WECHAT_OTHER,
	CTRLLIST_STYLE_WECHAT_MINE_MORE,
	CTRLLIST_STYLE_WECHAT_OTHER_MORE,
	CTRLLIST_STYLE_WECHAT_PHOTO,
	CTRLLIST_STYLE_ALARMCLOCK,
	CTRLLIST_STYLE_PEDOMETER,
	CTRLLIST_STYLE_SETTING,
	CTRLLIST_STYLE_WECHAT_TEXTLIST,
};

typedef uint8 CTRLLIST_DIRECTION_KEY_E;
enum
{
    CTRLLIST_DIRECTION_KEY_NONE,
    CTRLLIST_DIRECTION_KEY_DOWN,
    CTRLLIST_DIRECTION_KEY_UP,
    CTRLLIST_DIRECTION_KEY_LEFT,
    CTRLLIST_DIRECTION_KEY_RIGHT,
    CTRLLIST_DIRECTION_KEY_MAX
};

typedef uint8 CTRLLIST_MARK_TYPE_T;
enum
{
    CTRLLIST_MARK_CHECK,
    CTRLLIST_MARK_RADIO,
};


typedef uint8 CTRLLIST_MARKING_TYPE_T;
enum
{
    CTRLLIST_MARKING_NONE,
    CTRLLIST_MARKING_ENABLED,
    CTRLLIST_MARKING_FIXED,
};



/*! @struct CTRLLIST_MOVE_INFO_T
@brief �����ļ�¼��Ϣ
*/
typedef struct
{
    BOOLEAN                      had_moved;                          /*!< �Ѿ�������*/
    BOOLEAN                      list_tp_press_down;                 /*!< ���ʰ���*/
    BOOLEAN                      tp_down_handled;                    /*!< ���ʰ���ʱ�ı���Ѿ�����*/
    GUI_RECT_T                   prev_item_rect;                     /*!< �Ѿ������������*/
    uint16                       prev_item_index;                    /*!< �Ѿ�������item id*/
    CONTINUE_MOVE_TYPE_E         continue_move_type;                 /*!< ����ѡ���ƶ�����*/
} CTRLLIST_MOVE_INFO_T;

/*! @struct CTRLLIST_INIT_PARAM_T
@brief ��ʼ����Ϣ
*/
typedef struct
{
    CTRLBASE_INIT_PARAM_T  base_ctrl_param;

    GUILIST_TYPE_E         type;                  /*!< �ؼ�����*/
    GUIPRGBOX_STYLE_E      scroll_bar_type;       /*!< ����������>*/
    uint16                 max_item_num;          /*!< �ؼ������*/
    uint16                 max_selected_num;      /*!< ��ѡ�����ѡ��*/
    BOOLEAN                is_dynamic_item_data;  /*!< ����ѡ���ƶ�����*/
} CTRLLIST_INIT_PARAM_T;

//list�ڲ�����item���ݵĽṹ
typedef struct
{
    int32                   item_top;        // ����ڿؼ����������
    int32                   item_height;     // �ؼ�ITEM�߶�
    int16                   num_left;
    int16                   mark_left;
    GUI_RECT_T              content_rect[GUIITEM_CONTENT_MAX_NUM];

    CTRLLIST_ITEM_T          item;

    MMI_HANDLE_T            inner_handle[GUIITEM_CONTENT_MAX_NUM]; //ÿ��Ԫ�ض�������Ƕ�ؼ�
} CTRLLIST_ITEM_INFO_T;

/*! @struct CTRLLIST_OBJ_T
@brief �ؼ�����
*/
typedef struct _list_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;          /*!< �ؼ��������� */

    BOOLEAN                         is_full_dirty;           // ���������ȫ������Ҫ����
    BOOLEAN                         is_pos_dirty;           // �����������ʾλ����Ҫ����
    GUILIST_STATE_T                 list_state;         /*!< �ؼ��ⲿ״̬*/
    uint32                          inner_state;        /*!< �ڲ�״̬ */
    int16                           item_base_width; /*!< item�Ļ�׼��ȣ����������� */
    BOOLEAN                         show_num; /*!< �Ƿ���ʾ������� */
    CTRLLIST_MARKING_TYPE_T         marking_type; /*!< �Ƿ�ɱ�ʶ */
    BOOLEAN                         is_mark_icon_left; /*!< �Ƿ񽫱�ʶ��ʾ����� */
    CTRLLIST_MARK_TYPE_T            mark_type;          /*!< List�ı������ */
	uint16                          mark_left_margin;
	uint16                          mark_top_margin;
    uint16                          mark_top_margin_for_multi; 
    uint16                          avatar_checkbox_left_margin;
    uint16                          avatar_checkbox_top_margin;
	BOOLEAN         				is_use_special_check_image; // ?????????check???
	MMI_IMAGE_ID_T  				check_selected_image_id;    // ?????????
	MMI_IMAGE_ID_T  				check_unselected_image_id;  // ???��??????
    //for check box
    BOOLEAN                         is_check_used_for_multi_line;  /*!< ???check???????list style??????FALSE*/
    CONTINUE_MOVE_TYPE_E            continue_move_type; /*!< �����ƶ�������, Listֻ֧�����ƺ����� */
    GUIPRGBOX_STYLE_E               scroll_bar_type;    /*<����������>*/
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    MMITHEME_PRGBOX_TYPE_E          scroll_bar_theme;
#endif
    MMITHEME_LIST_STYLE_T           display_style;      /*!< �ؼ�Ĭ�Ϸ�� */

    uint8                           text_scroll_timer_id;               /*!< �ı�����ʱ��ʱ��ID*/
    uint8                           tp_timer_id;                          /*!< Ϊ֧��TP�����ƶ����ӵ�timer */
    uint8                           switch_timer_id;            // Ϊ֧��TP�����ƶ����ӵ�timer
    uint16                          str_start_pos;    /*!< ���ֹ�����,��ʾ����ʼλ�� */

    // item��Ϣ
    uint16                          move_item_num;     /*!< һ���ƶ���item��*/
    uint16                          max_item_num;      /*!< ������������Item��*/
    uint16                          total_item_num;    /*!< ����ITEM������,С�ڵ��������*/
    uint16                          top_item_idx;      /*!< ��ʾ����˵�Item ���� */
    uint16                          cur_item_idx;      /*!< ��ǰ���ѡ�е�ITEM ���� */
    uint16                          end_item_idx;      /*!< һҳ����ײ���item ���� */
    uint16                          tp_select_item_idx;/*!< tp�����item���� */
    uint16                          tp_select_item_content_idx;   /*!< tp�����tp_select_item_idx ��Ӧ��content���� */
    uint16                          mark_cnt_max;       /*!< ��ѡ������ͬʱѡ�е���Ŀ */
    uint16                          selected_item_num;                /*!< Ŀǰѡ�е�item��Ŀ*/
    uint16                          num_start_index;  /*!< ���ؼ�����ʾ����ʱ,��ֵ��ʾ��ʼ����*/

    CTRLLIST_ITEM_INFO_T            *item_info_ptr;    /*!< �ؼ�����Ϣ������Initʱ�����item memory ָ��, ����ΪCTRLLIST_ITEM_INFO_T */

    // move item, to record pre item info when move highlight bar
    uint16                          pre_top_item_idx;             /*!< �л�ITEM����֮ǰ�����ڶ���ITEM���� */
    uint16                          pre_cur_item_idx;       /*!< �л�ITEM����֮ǰ����ѡ�е�ITEM���� */
    uint16                          pre_end_item_idx;       /*!< �л�ITEM����֮ǰ����ѡ�е�ITEM���� */

    int32                           cur_object_index; /*!< ��ǰ���ڽ����Ŀؼ������� */
    int32                           total_item_height; /*!< item�ܸ߶� */

    // title and prgbox��Ϣ
    GUILIST_TITLE_STYLE_E           title_style;            /*!< �ؼ��б������ʾ����Ƿ���ʾ��ǰ��Ŀ��*/
    IGUICTRL_T                     *prgbox_ptr;                 /*!< ������ʵ��ָ�� */
    GUILIST_TITLE_INDEX_E           title_index_type;   /*!< �Ƿ���Ҫ�ڱ�������ʾ�����͵�ǰ��Ŀ */

    MMI_WIN_ID_T                    win_id;           /*!< ���� ID */
    CTRLLIST_MOVE_INFO_T             move_info;        /*!< ������Ϣ */

    /* keep old softkey id */
    MMI_TEXT_ID_T                   pre_left_sk_id;         /*!< item�л�֮ǰ��Ӧ�Ĵ���softkey ������Դid */
    MMI_TEXT_ID_T                   pre_middle_sk_id;       /*!< item�л�֮ǰ��Ӧ�Ĵ���softkey ������Դid */
    MMI_TEXT_ID_T                   pre_right_sk_id;        /*!< item�л�֮ǰ��Ӧ�Ĵ���softkey ������Դid */

    BOOLEAN                         is_disable_3dmmi; /*!< �Ƿ���ʾ3DЧ����ϵͳ������Ŀǰδʹ�� */
    BOOLEAN                         is_tpdown_select; /*!< tpdown�Ƿ����ITEM */
    BOOLEAN                         is_transparent;   /*!< �Ƿ�͸����ʾ���� */
    uint8                           bg_alpha;         /*!< ????????????? */
    BOOLEAN                         is_click_item;   /*!< ���ʵ��ʱ���Ƿ�����item�� */
    BOOLEAN                         is_tp_down;     /*!< �Ƿ�tpdown */
    BOOLEAN                         is_permit_border;   /*!<�Ƿ�������ʾ�߿�*/
    BOOLEAN                         is_tp_content_valid; /*!< ��Ч��tp content״̬ */
    BOOLEAN                         is_long_pressing;
    BOOLEAN                         is_left_slide_unfold; /*!< ��ǰitem������չ�� */
    BOOLEAN                         is_support_slide_dec_anim; /*!< �Ƿ�֧��anim�ڻ����н��� */

    GUILIST_EMPTY_INFO_T            empty_info;       /*!< ��item��Ϣ */

    GUISTR_STYLE_T                  text_style;       /*!< �û��Զ������ַ�� */
    BOOLEAN                         is_handle_starhash; /*!< �Ƿ�֧��*,\#���л�ҳ�� */
    BOOLEAN                         is_pre_send_ok;  /*!< ǰһ��tp upʱ��״̬*/

    // ������Ϣ
    uint8                           tp_move_timer_id;                   /*!< Ϊ֧��TP�����ƶ����ӵ�timer */

    MMK_TP_SLIDE_E                  slide_state;      /*!< slide״̬ */
    MMK_TP_SLIDE_E                  tpdown_slide_state;  /*!< TPDOWNʱ�Ļ���״̬*/
    GUI_POINT_T                     slide_start_point;   /*!< ��������ʼ�� */
    GUI_POINT_T                     slide_pre_point;     /*!< ����ʱ��ǰһ�� */
    int32                           offset_y;            /*!< ��������ڿؼ�����Y�����ƫ��*/
    GUI_LCD_DEV_INFO                item_layer;          /*!< Item��ʾ�Ĳ���Ϣ */
    BOOLEAN                         is_cus_blt_layer;    /*!< �Ƿ����û�built item layer��Ĭ�������ΪFALSE */

    float                           fling_velocity;      /*!< �����ٶ� */
    int32                           total_distance;
    int32                           total_duration;
    uint16                          cur_msg_loop_item_idx; /*!< ÿ����Ϣѭ����focus��index */

    BOOLEAN                         is_forbid_tip;
    GUI_LCD_DEV_INFO                tip_layer; /*!< tip����ʾ�� */
    GUI_RECT_T                      tip_rect; /*!< tip����ʾ���� */
    uint8                           tip_hide_timer; /*!< tip������timer */
    GUI_LCD_DEV_INFO                split_layer; /*!< split����ʾ�� */
    GUI_RECT_T                      split_rect; /*!< split����ʾ���� */

    BOOLEAN                         is_need_reinit_scrollbar;/*!< �Ƿ���Ҫ���³�ʼ�������� */
    CTRLLIST_GET_DATA_FUNC          get_data_func;

    GUI_RECT_T                      writeback_rect;//˫buffer��д

    uint8                           line_space;
    uint8                           char_space;
    GUI_COLOR_T                     invalid_color;

	//6pixel spec
	BOOLEAN                         is_permit_bottom_margin;   /*!<???????end item??softkey???��??*/
	uint8							end_item_bottom_margin;   /*!<end item??softkey?????*/

    MMI_IMAGE_ID_T                  left_quick_icon_image;    //Qucik????image id
    MMI_IMAGE_ID_T                  right_quick_icon_image;    //Qucik????image id
	BOOLEAN         				is_show_left_triange_image; //??????????????????????????????Focus????
	BOOLEAN         				is_show_right_triange_image; //??????????????????????????????Focus????
    BOOLEAN                         is_handle_left_key;         //?????????
    BOOLEAN                         is_handle_right_key;        //?????????
    MMI_IMAGE_ID_T                  simsel_icon_image;      //Quick action icon for sim select

	BOOLEAN is_through_click; // ???????????
	MMI_HANDLE_T parent_win; //??????id
} CTRLLIST_OBJ_T;

// label class for public or protected method
typedef struct _list_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;
} CTRLLIST_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get list type
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LIST_GetType(void);

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Lin.Lin
@param list_ptr [in]            �ؼ�ָ��
@param max_item_num [in]    �ؼ��������
@param is_dynamic_item_data [in]    �Ƿ�̬����
@retval true/false
@return ���سɹ���ʧ��
@note ��Ϊ��̬����ʱ���ؼ���ʾÿ��item�������򴰿�Ҫ����
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxItemByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16        max_item_num,
    BOOLEAN       is_dynamic_item_data
);

/***************************************************************************//*!
@brief �������ѡ�е�ITEM��
@author James.Zhang
@param list_ptr [in]     �ؼ�ָ��
@param mark_cnt_max [in]    ���ѡ����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetMaxSelectedItemByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16 mark_cnt_max
);

/***************************************************************************//*!
@brief  ���ÿؼ������ϸ����
@author yelianna
@param list_ptr [in]    �ؼ�ָ��
@param item_data_ptr [in]   �������õ�����
@param pos  [in]    �ƶ������õ�item����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetItemDataByPtr(
    CTRLLIST_OBJ_T      *list_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr,
    uint16           pos
);

/***************************************************************************//*!
@brief ����Item��������
@author yelianna
@param list_ptr [in]    �ؼ�ָ��
@param item_content_ptr  [in]   �������õ�����
@param item_index   [in]    �ƶ������õ�item����
@param item_content_index  [in] �������ݵ����꣨item_content_ptrΪ���飩
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetItemContentByPtr(
    CTRLLIST_OBJ_T       *list_ptr,
    const GUIITEM_CONTENT_T *item_content_ptr,
    uint16                    item_index,
    uint16                    item_content_index
);

/***************************************************************************//*!
@brief ���ÿؼ���״̬
@author James.Zhang
@param list_ptr [in]    �ؼ�ָ��
@param src_state    [in]    Ҫ���õ�״̬
@param is_true  [in]     ����/ȡ������
@retval void
@return void
@note
*******************************************************************************/
PUBLIC void LIST_SetListStateByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUILIST_STATE_T    src_state,
    BOOLEAN is_true
);


/***************************************************************************//*!
@brief ��ÿؼ���ĳ��״̬�Ƿ�����
@author James.Zhang
@param list_ptr [in]    �ؼ�ָ��
@param src_state    [in]    Ҫ��õ�״̬
@retval true/false
@return TRUE���ؼ����и�״̬��FALSE���ؼ�û�и�״̬
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetListStateByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUILIST_STATE_T    src_state
);

/***************************************************************************//*!
@brief  ��ÿؼ�����û��Զ�������
@author peng.chen
@param iguictrl_ptr [in]    �ؼ�ָ��
@param index     [in]   Ҫ��õ�Item����
@param user_data_ptr    [in]    �û����ݣ�����Ϊ�κ����ͣ��û��Զ���
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetItemDataByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    uint16 index,
    uint32 *user_data_ptr
);

/***************************************************************************//*!
@brief ������ǰItem����
@author Lin.Lin
@param list_ptr [in]    �ؼ�ָ��
@retval void
@return void
@note ��ɾ��ĳһ����ʱ�����ܴ��ڵ�ǰItem���ô���������ͨ���ýӿڿ��Խ���
*******************************************************************************/
PUBLIC void LIST_ValidCurItemPosByPtr(
    CTRLLIST_OBJ_T  *list_ptr
);

/***************************************************************************//*!
@brief ���ÿؼ��ı���ɫ
@author James.Zhang
@param list_ptr [in]    �ؼ�ָ��
@param back_color [in]   ����ɫ
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetBgColorByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    GUI_COLOR_T    back_color
);

/***************************************************************************//*!
@brief ����List�ؼ���Ĭ��ͼƬID
@author xiaoqing.lu
@param list_ptr [in]    �ؼ�ָ��
@param def_img_id [in]  ��������е�Ĭ��ͼƬ
@param err_img_id [in]  ����ʧ�ܺ��Ĭ��ͼƬ
@retval true/false
@return ���سɹ���ʧ��
@note ����Ĭ��ͼƬ����Ϊ��̬����ͼƬʱ����Ƕ�Ķ����ؼ�����
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetDefaultIconByPtr(
    CTRLLIST_OBJ_T  *list_ptr,
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
);

/***************************************************************************//*!
@brief ���ñ���ͼƬ��ԴID
@author xiaoqing.lu
@param list_ptr [in] �ؼ�ָ��
@param image_id [in] ͼƬ��ԴID
@param is_transparent [in] �Ƿ�͸����ʾ
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetBgImageByPtr(
    CTRLLIST_OBJ_T *list_ptr,
    MMI_IMAGE_ID_T image_id,
    BOOLEAN        is_transparent
);

/***************************************************************************//*!
@brief �����Ƿ�֧��slide״̬
@author Xiaoqing.Lu
@param list_ptr [in] �ؼ�ָ��
@param is_slide_used [in] TRUE:֧�֣�FALSE:��֧��
@retval true/false
@return ���سɹ���ʧ��
@note Ĭ�����������֧��slide״̬
*******************************************************************************/
PUBLIC BOOLEAN LIST_SetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ptr,
    BOOLEAN is_slide_used
);

/***************************************************************************//*!
@brief ��ȡ�Ƿ�֧��slide״̬
@author Xiaoming.Ren
@param ctrl_id [in] �ؼ�HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN LIST_GetSlideStateByPtr(
    CTRLLIST_OBJ_T  *list_ptr
);

/*****************************************************************************/
//  Description : �ж���move item ����move�����ؼ���������������
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SetMoveListItem(
    CTRLLIST_OBJ_T   *list_ptr,
    MMI_MESSAGE_ID_E msg_id,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:����list�ڲ�״̬
/*****************************************************************************/
PUBLIC void LIST_SetItemMoveState(
    CTRLLIST_OBJ_T *list_ptr,
    BOOLEAN         is_true
);

/*****************************************************************************/
//  Description : ���layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemDispLayer(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_RECT_T item_rect
);

/*****************************************************************************/
//  Description : ��ʾ�ַ���
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DisplayText(
    CTRLLIST_OBJ_T *list_ptr,
    MMI_TEXT_ID_T  text_id,
    GUI_RECT_T     *disp_rect_ptr,
    GUI_RECT_T     *clip_rect_ptr,
    GUISTR_STYLE_T *str_style_ptr
);

/*****************************************************************************/
//  Description : ���layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemDispLayer(void);

/*****************************************************************************/
//  Description : �����������
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_GetDispRect(
    GUI_RECT_T item_rect,
    MMITHEME_LIST_CUSTDATA_T user_data,
    BOOLEAN is_true,
    MMI_HANDLE_T win_handle
);

/*****************************************************************************/
//  Description : �ͷŲ�
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemDispLayer(void);

/*****************************************************************************/
//  Description : ��ͼ
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemSlideData(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_POINT_T      *point_ptr,
    GUI_RECT_T       item_rect
);

/*****************************************************************************/
//  Description : �ͷŲ�
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ReleaseItemMistLayer(void);

/*****************************************************************************/
//  Description : ���layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_CreateItemMistLayer(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_RECT_T item_rect
);

/*****************************************************************************/
//  Description : ���layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_LCD_DEV_INFO LIST_GetItemMistLayer(void);

/*****************************************************************************/
//  Description : ���layer
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemDispLayerActive(void);

/*****************************************************************************/
//  Description : ������Ч��
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DisplayItemSlideColor(
    CTRLLIST_OBJ_T   *list_ptr,
    GUI_POINT_T      *tp_point_ptr,
    GUI_RECT_T       item_rect
);

/*****************************************************************************/
//  Description : display text dividing line
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawListItemSplitLine(
    CTRLLIST_OBJ_T *list_ptr,
    uint16         item_index,
    GUI_RECT_T item_rect,
    const GUI_LCD_DEV_INFO *lcd_dev_info_ptr
);

/*****************************************************************************/
//  Description : ����item��ʵ��rect
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CalculateListItemRect(
    GUI_RECT_T     *rect_ptr,   // OUT, item rect
    CTRLLIST_OBJ_T *list_ptr,  // �ؼ�ָ��
    uint16          item_index    // IN, ��ǰҳ��item�����ţ���0��page_item_num - 1
);

/*****************************************************************************/
//  Description : get the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:���list�ڲ�״̬
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetInnerState(
    CTRLLIST_OBJ_T  *list_ptr,
    GUILIST_STATE_T src_state
);

/*****************************************************************************/
//  Description : set the inner state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:����list�ڲ�״̬
/*****************************************************************************/
PUBLIC void LIST_SetInnerState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUILIST_STATE_T src_state,
    BOOLEAN         is_true
);

/*****************************************************************************/
//  Description : unselected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_UnSelectItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
);

/*****************************************************************************/
//  Description : selected item num
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_SelectItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 index
);

/*****************************************************************************/
//  Description : get the appointed item pointer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CTRLLIST_ITEM_INFO_T *LIST_GetItemInfoPtr(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 LIST_GetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description : get the item text
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetListItemContentTextInfo(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    CTRLLIST_ITEM_INFO_T *item_info_ptr,
    int16 content_index,
    MMI_STRING_T *out_str
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetState(
    uint32 *dst_state_ptr,
    uint32  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description : get state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetState(
    uint32 dst_state,
    uint32 src_state
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SetItemState(
    CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemState(
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    GUIITEM_STATE_T src_state
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetAllItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: ���㵥��item����ʾ��ȣ��߶�
//  ��ʾ���ָcontent����ʾ��ȣ�����num��select��mark��ȣ������߿�ȶ�����ͬ��
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ResetItemDisp(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    int32 index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: ���ؼ�������ı䣬��������item top
/*****************************************************************************/
PUBLIC void LIST_ResetItemTop(CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : list type of
//  Global resource dependence :
//  Author:James.Zhang
//  Note:�ú�����������ʱ�����б�
/*****************************************************************************/
PUBLIC BOOLEAN LIST_TypeOf(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get valid rect
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetValidRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *rect_ptr
);
/*****************************************************************************/
//  Description : ��ֹ��ǰ���ڽ����item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DestroyItemDecode(
    CTRLLIST_OBJ_T *list_ctrl_ptr   //in
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    BOOLEAN                  is_page
);

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemData(
    CTRLLIST_OBJ_T      *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN              is_page
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPage(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    BOOLEAN        is_page
);

/*****************************************************************************/
//  Description : destroy item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyItemInlineCtrl(//is destroy anim ctrl
    CTRLLIST_ITEM_INFO_T  *item_ptr,
    uint16               item_content_index
);

/*****************************************************************************/
//  Description : destroy all item data
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DestroyAllItemDataNotInPageExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    uint16         pre_top_index,
    uint16         pre_end_index
);

/*****************************************************************************/
//  Description : get selected icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetSelectIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr
);

/*****************************************************************************/
//  Description : get number icon of list
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetNumberIconId(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                index
);

/*****************************************************************************/
//  Description : �����ؼ�������
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T LIST_AdjustRect(
    const CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get the item page num
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetPageItemNum(CTRLLIST_OBJ_T *list_ctrl_ptr);

/*****************************************************************************/
//  Description : get the item num by height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemNumByHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16              total_height,
    uint16              index,  //from 0 to total_item_num-1
    BOOLEAN             is_next
);

/*****************************************************************************/
//  Description : �ж���move item ����move�����ؼ���������������
//  Global resource dependence :
//  Author:Xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetTotalHeight(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : Get current item index
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetCurItemIndexByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : Get total item number
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetTotalItemNumByPtr(
    CTRLLIST_OBJ_T    *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : draw icon in item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIcon(
    const CTRLLIST_OBJ_T   *list_ctrl_ptr,
    const GUI_RECT_T       *rect_ptr,       //the rect to display this item
    const GUI_RECT_T       *disp_rect_ptr,
    MMI_IMAGE_ID_T          icon_id,
    const GUI_LCD_DEV_INFO *dev_info_ptr
);

/*****************************************************************************/
//  Description : item content������
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgIcon(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    GUI_RECT_T                item_rect,
    GUI_RECT_T                content_rect,
    MMI_IMAGE_ID_T            icon_id
);

/*****************************************************************************/
//  Description : item content������
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_DrawItemContentBgColor(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_info_ptr,
    MMITHEME_COLOR_ID_E       bg_color_id,
    GUI_RECT_T                rect,
    GUI_RECT_T                inter_rect,
    MMI_IMAGE_ID_T            image_id
);


/*****************************************************************************/
//  Description : create item data
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_CreateItemData(
    CTRLLIST_OBJ_T            *list_ctrl_ptr,  // the list control pointer
    CTRLLIST_ITEM_INFO_T       *item_ptr,
    const GUILIST_ITEM_DATA_T *item_data_ptr
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_AdjustItemHeightAndTop(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // �ؼ�ָ��
    uint16 pre_idx,  // �ƶ�����
    uint16 cur_idx
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemHeight(
    CTRLLIST_OBJ_T *list_ctrl_ptr, // �ؼ�ָ��
    uint16 pre_idx,  // �ƶ�����
    uint16 cur_idx
);

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
);

/*****************************************************************************/
//  Description : send need item data message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemDataMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         item_index
);

/*****************************************************************************/
//  Description : send item content click message
//  Parameter: [In] p_list_ctrl
//             [In] item_index
//             [In] item_content_index
//             [Return] none
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendItemContentClickMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index
);

/*****************************************************************************/
//  Description : send need item content message
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_SendNeedItemContentMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16          item_index,
    uint16          item_content_index
);

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void LIST_PostMarkItemMsg(
    const CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16                item_index,
    BOOLEAN               is_selected
);

/*****************************************************************************/
//  Description : is need scroll bar
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsNeedScrollBar(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

/*****************************************************************************/
//  Description : get item state by index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_GetItemStateByIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    GUIITEM_STATE_T item_state
);

/*****************************************************************************/
//  Description : get item's backgroup image id
//  Global resource dependence :
//  Author:
//  Note: if(is_highlight == TRUE): get item's highlisth backgroup image
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T LIST_GetItemBgImageID(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index,
    BOOLEAN         is_highlight
);

/*****************************************************************************/
//  Description : �ж��Ƿ���Ŀ�ͷ
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_ItemIsGroupHead(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16         index
);

/*****************************************************************************/
//  Description : ��ȡ����¼��split�ַ���
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemSplitText(
    CTRLLIST_OBJ_T *list_ctrl_ptr,//IN
    uint16         cur_index,//IN
    MMI_STRING_T   *item_text_ptr//OUT
);

/*****************************************************************************/
//  Description : �ж�item�Ƿ���Խ�����ʾ
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_IsItemGetFocus(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index
);

/*****************************************************************************/
//  Description : post mark item
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUILIST_QRERY_SELECT_ITEM_T LIST_QueryMarkItemMsg(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16               item_index,
    MMI_MESSAGE_ID_E     msg_id
);

/*==============================================================================
Description: ����splitbar������
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC GUI_RECT_T LIST_GetSplitBarRect(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

#ifdef UI_MULTILAYER_SUPPORT
/*==============================================================================
Description: ����splitbar����ʾ��
Global resource dependence: NONE
Author:
Note:
==============================================================================*/
PUBLIC BOOLEAN LIST_CreateSplitBarLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUI_RECT_T *split_rect_ptr
);

/*==============================================================================
Description: ���Split��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void LIST_ReleaseSplitLayer(
    CTRLLIST_OBJ_T *list_ctrl_ptr
);

#endif

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : calc top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalcEndItemByTopIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 top_item_index
);

/*****************************************************************************/
//  Description : reset top item index, end item index, and offset each
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetTopItemAndEndItemIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 move_space
);

/*******************s**********************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetItemPostion(
    CTRLLIST_OBJ_T *list_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : reset offset y by end item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByEndIndex(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    int32 total_height
);

/*****************************************************************************/
//  Description : reset offset y by start item index
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ResetOffsetYByTopIndexExt(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  //in
    uint16 item_index
);

/*****************************************************************************/
//  Description : get item index
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_GetItemIndexByPoint(
    CTRLLIST_OBJ_T *list_ctrl_ptr,  // the list control pointer
    GUI_POINT_T    *point_ptr
);

/*****************************************************************************/
//  Description : handle the tp move message
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LIST_HandleItemTpMove(
    CTRLLIST_OBJ_T   *list_ctrl_ptr,
    GUI_POINT_T      *point_ptr
);

/*****************************************************************************/
//  Description : ListDrawItemAnim
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemAnim(
    CTRLLIST_OBJ_T  *list_ctrl_ptr,
    int32 start_x, // x��ʼ��
    int32 end_x, // x�յ�
    GUI_RECT_T item_rect,
    uint32 sleep_time
);

/*****************************************************************************/
//  Description : display icon bg or color
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawItemIconBg(
    GUI_RECT_T        *rect_ptr,      //in
    CTRLLIST_OBJ_T    *list_ctrl_ptr,      //in
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr    //in
);

/*****************************************************************************/
//  Description : move to next item, �л���Ч��
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 LIST_CalculateMoveItemNum(
    CTRLLIST_OBJ_T *list_ctrl_ptr,            // �ؼ�ָ��
    CTRLLIST_DIRECTION_KEY_E direct_type  // �ƶ�����
);

/*****************************************************************************/
//  Description : ����ǰ��������������Ч������
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void LIST_ChangeCurItemToValidItem(
    CTRLLIST_OBJ_T *list_ctrl_ptr // �ؼ�ָ��
);

/*****************************************************************************/
//  Description : get the item height
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC GUIITEM_THEME_STATE_E LIST_GetItemThemeState(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    const CTRLLIST_ITEM_INFO_T *item_ptr,
    BOOLEAN             is_highlight
);

/*****************************************************************************/
//  Description : ��ȡ�б�ؼ��ı���style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void LIST_GetItemContentEffectStr(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    GUISTR_STYLE_T *str_style_ptr,
    uint16 item_index,
    uint16 content_index
);

/*****************************************************************************/
//  Description : ��ȡ�б�ؼ��ı���style
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T LIST_GetItemContentTextColor(
    CTRLLIST_OBJ_T *list_ctrl_ptr,
    uint16 item_index,
    uint16 content_index
);

/*****************************************************************************/
//  Description : draw highlight item for slide
//  Global resource dependence :
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void LIST_DrawHighLightItem_ForSlide(
                                                            CTRLLIST_OBJ_T *list_ctrl_ptr,
                                                            GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                                            );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _CTRLLIST_LISTBOX_H_  */


/*@}*/

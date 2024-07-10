/*! @file guilistbox.h
@brief ���ļ�������listbox�ؼ��Ķ���ӿں���������
@author Great.Tian
@version 1.0
@date 03/23/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 
-# 03/2004        Great.Tian       Create
-# 02/12/2006     Xiaoqing.Lu      Modify
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
TXT_HELLO_TEST_LIST	20	Test List	�б����
TXT_HELLO_TEST_LIST1	20	Test List1	Test List1
TXT_HELLO_TEST_LIST2	20	Test List2	Test List2
TXT_HELLO_TEST_LIST3	20	Test List3	Test List3
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
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T       ctrl_id = MMI_TEST_LIST_CTRL_ID;

    lcd_dev_info.lcd_id		= GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id	= GUI_BLOCK_MAIN;
    
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
                          MMI_TEXT_ID_T	   left_softkey_id,
                          MMI_TEXT_ID_T	   right_softkey_id
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

#ifndef  _CTRLLIST_EXPORT_H_    
#define  _CTRLLIST_EXPORT_H_   

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h"
#include "guicommon.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guiprgbox.h"
#include "mmi_theme.h"
#include "mmitheme_list.h"
#include "guianim.h"
#include "guistring.h"

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
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
typedef struct _list_obj_tag* CTRL_LIST_HANDLE_T;

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/*! @enum GUILIST_EFFECT_STR_TYPE_E
@brief 
*/
typedef uint8 GUILIST_EFFECT_STR_TYPE_E;
enum
{
    GUILIST_EFFECT_NONE,
    GUILIST_EFFECT_STR,
    GUILIST_EFFECT_REGION
};



/*! @enum GUILIST_TYPE_E
@brief �ؼ���ʾ����
*/
typedef uint8 GUILIST_TYPE_E;
enum
{
    GUILIST_TYPE_NONE,		/*!<  none */
    GUILIST_RADIOLIST_E, 	/*!<  ��ѡ(Radio)���� */
    GUILIST_CHECKLIST_E, 	/*!<  ��ѡ(check)���� */
    GUILIST_TEXTLIST_E,  	/*!<  Text���� */
    GUILIST_TYPE_MAX		/*!<  ϵͳ���� */
};




/*! @enum GUILIST_TITLE_STYLE_E
@brief �ؼ���ʾ��title��Ϣ����
*/
typedef uint8 GUILIST_TITLE_STYLE_E;
enum
{
	GUILIST_TITLE_NONE,/*!<  none */
	GUILIST_TITLE_ONLY,/*!<  �������Ϣ, ��1/10 */
	GUILIST_TITLE_AND_INFO = GUILIST_TITLE_ONLY, /*!<  �������Ϣ, ��1/10 */
	GUILIST_TITLE_MAX /*!<  ϵͳ���� */
};

/*! @enum GUILIST_TITLE_INDEX_E
@brief title����ʾ��������Ϣ���
*/
typedef uint8 GUILIST_TITLE_INDEX_E;
enum
{
	GUILIST_TITLE_INDEX_NONE,/*!<  none */
	GUILIST_TITLE_INDEX_DEFAULT,       /*!<  current index + 1 */
	GUILIST_TITLE_INDEX_EXCLUDE_FIRST, /*!< current index -1, total num - 1 */
	GUILIST_TITLE_INDEX_MAX, /*!<  ϵͳ���� */
};

/*! @enum GUIITEM_DATA_TYPE_E
@brief Item�������ͣ�ͨ����Щ���Ͷ���������Ϣ��UNION
*/
typedef uint8 GUIITEM_DATA_TYPE_E;
enum
{
    GUIITEM_DATA_NONE,  /*!<  none */
    GUIITEM_DATA_TEXT_ID,   /*!<  �ı���ԴID */
	GUIITEM_DATA_TEXT_BUFFER, /*!<  �ı�buffer */
    GUIITEM_DATA_IMAGE_ID,    /*!<  ͼƬ��ԴID */ 
    GUIITEM_DATA_ANIM_DATA,   /*!<  ͼƬ��ϸ�������� */
    GUIITEM_DATA_ANIM_PATH,   /*!<  ͼƬ·�� */
	GUIITEM_DATA_MAX,
};

/*! @enum GUILIST_SLIDE_STATE_E
@brief �ؼ�����״̬���ؼ�����slide��slide�������֪ͨ����
*/
typedef uint8 GUILIST_SLIDE_STATE_E;
enum
{
    GUILIST_SLIDE_NONE,/*!<  none */
    GUILIST_START_SLIDE,/*!<  ��ʼslide״̬ */
    GUILIST_END_SLIDE,/*!<  slide״̬���� */
    GUILIST_STATE_MAX/*!<  ϵͳ���� */
};


/*! @typedef GUIITEM_STATE_T
@brief Item״̬
*/
typedef uint32 GUIITEM_STATE_T;
enum
{
    GUIITEM_STATE_NONE                  = 0x00000000, //item none state, start value
    GUIITEM_STATE_SELECTED              = 0x00000001, //itemѡ��״̬, ��check listѡ�л�text list mark״̬
    GUIITEM_STATE_INVALID               = 0x00000002, //item��Ч״̬
    GUIITEM_STATE_NEED_ITEM_CONTENT     = 0x00000004, //item��Ҫ�첽��������, �ڲ�ʹ�õ�״̬
    GUIITEM_STATE_DISABLE_MARK          = 0x00000008, //��list��������, Ҳ��ֹ���
    GUIITEM_STATE_HAS_NUM   			= 0x00000010, //��list��ʾ�����������£�����ʾ�������
    GUIITEM_STATE_GRAY                  = 0x00000040, //item�Ự״̬
    GUIITEM_STATE_SPLIT                 = 0x00000080, //item��Ϊ�ָ����������ܽ���
    GUIITEM_STATE_COSTMER_FONT_COLOR    = 0x00000100, //item�Զ�������
    GUIITEM_STATE_SELFADAPT_RECT        = 0x00000200, //����Ӧ��ʾ����
    GUIITEM_STATE_CONTENT_CHECK         = 0x00000400, //���item content����
    GUIITEM_STATE_EXPAND                = 0x00000800, //����չ�������ʾ">"
    GUIITEM_STATE_CHECK_SELECT_RECT     = 0x00001000, //����check select����
    GUIITEM_STATE_NOTHOR_SLIDE			= 0x00002000, //item��֧�ֺ����ƶ�
    GUIITEM_STATE_HAS_AVATAR_MASK       = 0x00010000, //
    GUIITEM_STATE_HAS_INITIAL_MASK      = 0x00020000, //
    GUIITEM_STATE_HAS_INITIAL_CHECKBOX  = 0x00040000, //
    GUIITEM_STATE_HAS_AVATAR_CHECKBOX   = 0x00080000, //
    GUIITEM_STATE_HAS_TOGGLE_MARK       = 0x00100000, //
    GUIITEM_STATE_HAS_SIM_SELECTION     = 0x00200000, //
	GUIITEM_STATE_HAS_ACCESSORY_MASK    = 0x00400000, //
    GUIITEM_STATE_MAX                   = 0x08000000, //item none state, Max value
};




/*! @typedef GUILIST_STATE_T
@brief �ؼ���״̬
*/
typedef uint32 GUILIST_STATE_T;

/* list state */
/*! @def GUILIST_STATE_ACTIVE
@brief list active״̬
*/
#define GUILIST_STATE_ACTIVE            0x00000001   //list active״̬
/*! @def GUILIST_STATE_FOCUS
@brief list focus״̬
*/
#define GUILIST_STATE_FOCUS             0x00000002   //list focus״̬
/*! @def GUILIST_STATE_NEED_TITLE
@brief list ��Ҫ����״̬��Ĭ���������Ҫ
*/
#define GUILIST_STATE_NEED_TITLE        0x00000004   //list ��Ҫ����״̬��Ĭ���������Ҫ
/*! @def GUILIST_STATE_NEED_HIGHTBAR
@brief list ��Ҫ������״̬��Ĭ���������Ҫ
*/
#define GUILIST_STATE_NEED_HIGHTBAR     0x00000008   //list ��Ҫ������״̬��Ĭ���������Ҫ
/*! @def GUILIST_STATE_NEED_PRGBAR
@brief list ��Ҫ������״̬��Ĭ���������Ҫ
*/
#define GUILIST_STATE_NEED_PRGBAR       0x00000010   //list ��Ҫ������״̬��Ĭ���������Ҫ
/*! @def GUILIST_STATE_NEED_PRGBAR_BLOCK
@brief list�ϵĹ������Ƿ���Ҫ���飬Ĭ���������Ҫ
*/
#define GUILIST_STATE_NEED_PRGBAR_BLOCK 0x00000020   //list�ϵĹ������Ƿ���Ҫ���飬Ĭ���������Ҫ
/*! @def GUILIST_STATE_SET_PRGBAR_IMG
@brief list�ϵĹ������������û�����
*/
#define GUILIST_STATE_SET_PRGBAR_IMG    0x00000040   //list�ϵĹ������������û�����
/*! @def GUILIST_STATE_NEED_SOFTKEY
@brief list ��Ҫ���״̬��Ĭ���������Ҫ
*/
#define GUILIST_STATE_NEED_SOFTKEY      0x00000080   //list ��Ҫ���״̬��Ĭ���������Ҫ
/*! @def GUILIST_STATE_ENABLE_MARK
@brief list �ܹ�mark״̬
*/
#define GUILIST_STATE_ENABLE_MARK       0x00000100   //list �ܹ�mark״̬
/*! @def GUILIST_STATE_OWN_SOFTKEY
@brief list ��Ҫ�Զ������״̬
*/
#define GUILIST_STATE_OWN_SOFTKEY       0x00000200   //list ��Ҫ�Զ������״̬
/*! @def GUILIST_STATE_INVALID
@brief list ��Ҫ����top index״̬
*/
#define GUILIST_STATE_INVALID           0x00000400   //list ��Ҫ����top index״̬
/*! @def GUILIST_STATE_SYNC_DECODE_ANIM
@brief list�ϵ�Item�����Ƿ���Ҫͬ����ʾ�����룩��Ĭ����������첽��
*/
#define GUILIST_STATE_SYNC_DECODE_ANIM  0x00000800   //list�ϵ�Item�����Ƿ���Ҫͬ����ʾ�����룩��Ĭ����������첽��
/*! @def GUILIST_STATE_CYC_SCROLL_ITEM
@brief list�����¼�����ʱ,�Ƿ�ѭ������,Ĭ���ǿ��Ե�ѭ������
*/
#define GUILIST_STATE_CYC_SCROLL_ITEM   0x00001000   //list�����¼�����ʱ,�Ƿ�ѭ������,Ĭ���ǿ��Ե�ѭ������
/*! @def GUILIST_STATE_HANDLE_LEFT_RIGHT
@brief list�Ƿ������Ұ���, Ĭ�ϲ�����
*/
#define GUILIST_STATE_HANDLE_LEFT_RIGHT 0x00002000   //list�Ƿ������Ұ���, Ĭ�ϲ�����
/*! @def GUILIST_STATE_TEXT_STYLE
@brief listʹ���û��Զ����text style
*/
#define GUILIST_STATE_TEXT_STYLE        0x00004000   //listʹ���û��Զ����text style
/*! @def GUILIST_STATE_DROPDOWNLIST
@brief listʹ����dropdownlist�ؼ���
*/
#define GUILIST_STATE_DROPDOWNLIST      0x00008000   //listʹ����dropdownlist�ؼ���
/*! @def GUILIST_STATE_ENTER
@brief list����
*/
#define GUILIST_STATE_ENTER             0x00010000   //list����
/*! @def GUILIST_STATE_SLIDE_IN
@brief list�������룬���¶���
*/
#define GUILIST_STATE_SLIDE_IN          0x00020000   //list�������룬���¶���
/*! @def GUILIST_STATE_SELFSET_BG
@brief list�û����豳��
*/
#define GUILIST_STATE_SELFSET_BG        0x00040000   //list�û����豳��
/*! @def GUILIST_STATE_SET_CHECK_AUTO
@brief ���ڵ�ѡ�Ͷ�ѡ���б��л�ʱ�Զ����ñ�ѡ�е���
*/
#define GUILIST_STATE_SET_CHECK_AUTO    0x00080000   //���ڵ�ѡ�Ͷ�ѡ���б��л�ʱ�Զ����ñ�ѡ�е���
/*! @def GUILIST_STATE_SPLIT_LINE
@brief list item ��Ҫ���ָ���
*/
#define GUILIST_STATE_SPLIT_LINE        0x00100000   //list item ��Ҫ���ָ���
/*! @def GUILIST_STATE_AUTO_SWITCH
@brief �Զ��л�Item
*/
#define GUILIST_STATE_AUTO_SWITCH       0x00200000   //�Զ��л�Item
/*! @def GUILIST_STATE_TEXT_COLOR
@brief ���״ֻ̬��form�ؼ�ʹ��
*/
#define GUILIST_STATE_TEXT_COLOR        0x00400000   //���״ֻ̬��form�ؼ�ʹ��

#define GUILIST_STATE_AUTO_SCROLL      0x00800000   //���״̬�����м��item����

/*! @def GUILIST_STATE_SPLIT_ONSIDE
@brief item��Ϊ�ָ����������ܽ���
*/
#define GUILIST_STATE_SPLIT_ONSIDE      0x01000000   //item��Ϊ�ָ���ʱ���Ƿ�֧�ַָ���������Ĭ�ϲ�֧��

/*! @def GUILIST_STATE_CHECK_ITEM
@brief content��Ҫ����
*/
#define GUILIST_STATE_CHECK_ITEM        0x02000000   //content��Ҫ����

/*! @def GUILIST_STATE_EFFECT_STR
@brief content������ʾ��Ч
*/
#define GUILIST_STATE_EFFECT_STR        0x04000000   //content������ʾ��Ч

/*! @def GUILIST_STATE_EFFECT_STR
@brief content������ʾ��Ч
*/
#define GUILIST_STATE_TEXTSCROLL_ENABLE 0x08000000   //content������ʾ��Ч

/*! @def GUILIST_STATE_ZOOM_ANIM
@brief list�ϵ�Item�����Ƿ���Ҫ���ţ�Ĭ��������ǲ����ŵ�
*/
#define GUILIST_STATE_ZOOM_ANIM         0x10000000   //list�ϵ�Item�����Ƿ���Ҫ���ţ�Ĭ��������ǲ����ŵ�

#define GUIITEM_GROUP_NO_BG             0x8000

/*! @struct GUILIST_NEED_ITEM_DATA_T
@brief ��̬����itemʱ��֪ͨӦ�ã���Ҫ��ȡ������ʱ��Я���Ĳ���
*/
typedef struct
{
    MMI_HANDLE_T                ctrl_handle;   /*!< �ؼ����*/
    MMI_CTRL_ID_T               ctrl_id;       /*!< �ؼ�ID*/
    uint16                      item_index;    /*!< �ؼ�����*/
}GUILIST_NEED_ITEM_DATA_T;

/*! @struct GUILIST_NEED_ITEM_CONTENT_T
@brief ��̬����itemʱ��֪ͨӦ�ã���Ҫ��ȡ������ʱ��Я���Ĳ���
*/
typedef struct
{
    MMI_HANDLE_T                ctrl_handle;   /*!< �ؼ���� */
    MMI_CTRL_ID_T               ctrl_id;			 /*!< �ؼ�ID */
    uint16                      item_index;		 /*!< �ؼ�����*/
    uint16                      item_content_index; /*!< ����������*/
}GUILIST_NEED_ITEM_CONTENT_T;

typedef GUILIST_NEED_ITEM_CONTENT_T GUILIST_ITEM_CONTENT_CLICK_T;

/*! @union GUIITEM_DATA_U
@brief ����������
*/
typedef union
{
    MMI_TEXT_ID_T               text_id;           /*!< �ı���ԴID */
    MMI_STRING_T                text_buffer;       /*!< �ַ�����Ϣ */
    MMI_IMAGE_ID_T              image_id;          /*!< ͼƬ��ԴID */
    GUIANIM_DATA_INFO_T*        anim_data_ptr;     /*!< ͼƬ����, ��set item contentʱ����, ���������� */
    GUIANIM_FILE_INFO_T*        anim_path_ptr;     /*!< ͼƬ·��, ��set item contentʱ����, ���������� */
}GUIITEM_DATA_U;

/*! @struct GUIITEM_FILE_INFO_T
@brief �ļ���Ϣ
*/
typedef struct
{
    wchar       *full_path_wstr_ptr; /*!<ȫ·����*/
    uint16      full_path_wstr_len;  /*!<ȫ·��������*/
} GUIITEM_FILE_INFO_T;

/*! @struct MMI_BUTTON_T
@brief button��Ϣ
*/
typedef struct
{
    GUIITEM_FILE_INFO_T    image_info;
    MMI_STRING_T           str;
} MMI_BUTTON_T;

/*! @struct GUIITEM_CONTENT_T
@brief ��������
*/
typedef struct
{
    GUIITEM_DATA_U              item_data;      /*!< ����������*/
    MMI_IMAGE_ID_T              icon_id;        /*!< content�����½���ʾСicon��Ŀǰֻ��content����ΪGUIITEM_DATA_IMAGE_IDʱ��Ч*/
    GUIITEM_DATA_TYPE_E         item_data_type; /*!< ��������*/
    MMITHEME_COLOR_ID_E         bg_color_id;    /*!< contentС������Ϊ0��ʱ��Ĭ�ϲ�������ɫ��Ŀǰֻ��content����ΪGUIITEM_DATA_IMAGE_IDʱ��Ч**/
    MMITHEME_COLOR_ID_E         font_color_id;  /*!< ���ֱ���ɫID��Ϊ0��ʱ��ȡĬ��ɫ*/
	BOOLEAN                     is_default;     /*!< �Ƿ���ȱʡֵ*/
#ifdef _SW_ZDT_PRODUCT_
	GUI_COLOR_T                 custom_font_color;
	GUI_COLOR_T                 custom_font_color_focus;
	BOOLEAN                     is_custom_font_color;
	uint8                       font_color_opacity;
	uint8                       font_color_opacity_focus;
	BOOLEAN                     is_opacity;
#endif
}GUIITEM_CONTENT_T;

/*! @struct GUILIST_ITEM_DATA_T
@brief ����softkey���������ݵĽṹ��
*/
typedef struct
{
    GUIITEM_CONTENT_T          item_content[GUIITEM_CONTENT_MAX_NUM]; /*!< item��content, ���֧��˫��5��content*/
    MMI_TEXT_ID_T			   softkey_id[GUISOFTKEY_BUTTON_NUM];     /*!< �����Ӧ��������Դid*/
}GUILIST_ITEM_DATA_T;

/*! @struct CTRLLIST_ITEM_T
@brief �ؼ�ITEM����
@note ��һ��split֮ǰ���ǵ�0�飬֮����ǵ�1�飬��һ��split�����ǵ�1��
*/
typedef struct
{
    uint32                      style_id;
    THEMELIST_ITEM_STYLE_T      *style_ptr;
    GUILIST_ITEM_DATA_T         *data_ptr; /*!< item����*/

    GUIITEM_STATE_T             state;    /*!< item״̬*/

    uint16                      first_letter;  /*!< ������ĸ*/
    uint16                      item_group;    /*!< item�Ĺ��࣬ͬһ������Ϊһ�࣬������ͬ����1��ʼ��0��ʾ��ʹ�ñ�������Ҫ����*/
    uint16                      group_idx;     /*!< item�����������鼴split��Ȼ�ָ��γɵ� */

    uint32                      user_data;     /*!< �û��Զ�������*/
} CTRLLIST_ITEM_T;


/*! @struct GUILIST_INIT_DATA_T
@brief ��ʼ���ؼ�ʱ������
*/
typedef struct 
{
    GUI_BOTH_RECT_T              both_rect;             /*!< �ؼ�������*/
    GUILIST_TYPE_E               type;				    /*!< �ؼ�����*/
	GUIPRGBOX_STYLE_E            scroll_bar_type;       /*!< ����������>*/
    uint16                       max_item_num;		    /*!< �ؼ������*/
    uint16                       max_selected_num;	    /*!< ��ѡ�����ѡ��*/
    BOOLEAN                      is_dynamic_item_data;  /*!< ����ѡ���ƶ�����*/

}GUILIST_INIT_DATA_T;

/*! @struct GUILIST_EMPTY_INFO_T
@brief 
*/
typedef struct
{
    MMI_TEXT_ID_T               text_id;           /*!< �ı���ԴID*/
    MMI_STRING_T                text_buffer;       /*!< wstr_ptr��Ϊ��, ��ʹ��text buffer, ����ʹ��text id��Ĭ�ϵ�id*///
}GUILIST_EMPTY_INFO_T;

/*! @struct GUILIST_POS_INFO_T
@brief 
*/
typedef struct
{
    uint16 top_item_idx;				    /*!< ��ʾ����˵�Item ���� */
    uint16 cur_item_idx;    			  /*!< ��ǰ���ѡ�е�ITEM ���� */
    int32 offset_y;            /*!< ��һ��ITEM�Ķ�������ڿؼ�����Y�����ƫ�� */
}GUILIST_POS_INFO_T;

/*! @typedef GUILIST_ITEM_INFO_T
@brief  list�ڲ�����item���ݵĽṹ
*/
// typedef struct GUILIST_ITEM_INFO_T GUILIST_ITEM_INFO_T;

/*! @struct GUILIST_EMPTY_INFO_T
@brief 
*/
typedef struct  
{
    // �ؼ�����APP
    uint16          item_index;
    uint16          content_index;

    // ������APP��д���ݣ����ظ��ؼ� begin
    GUISTR_STATE_T  state;
    GUI_COLOR_T     font_color;

    GUILIST_EFFECT_STR_TYPE_E effect_type;
    // effect_type == GUILIST_EFFECT_STRʱ���û���д������
    MMI_STRING_T    str_info; 

    // effect_type == GUILIST_EFFECT_REGIONʱ���û���д������
    GUIFONT_REGION_STYLE_T *region_ptr;/*!< ��������������Ϣ*/
    uint16                 region_num;/*!< �������ֵ�������*/

    // end
}GUILIST_EFFECT_STR_T;


/*! @struct GUILIST_MARK_ITEM_T
@brief list item�����Ϣ
*/
typedef struct
{
    MMI_HANDLE_T                    ctrl_handle;   /*!< �ؼ���� */
    MMI_CTRL_ID_T                   ctrl_id;       /*!< �ؼ�id */
    uint16                          item_idx;      /*!< item���� */
    BOOLEAN                         had_maked;     /*!< �Ƿ��� */
}GUILIST_MARK_ITEM_T;

/*! @struct GUILIST_MARK_ITEM_T
@brief list item�����Ϣ
*/
typedef struct
{
    MMI_HANDLE_T                    ctrl_handle;   /*!< �ؼ���� */
    MMI_CTRL_ID_T                   ctrl_id;       /*!< �ؼ�id */
    uint16                          item_idx;      /*!< item���� */
	MMI_MESSAGE_ID_E				msg_id;
    BOOLEAN                         is_check_select; /*!< ѯ��app�Ƿ�ִ��ѡ��/ȡ��ѡ�� */
}GUILIST_QRERY_SELECT_ITEM_T;

/*! @struct GUILIST_ANIM_DISP_RESULT_T
@brief ���ͽ����Ľ����app
*/
typedef struct
{
    GUIANIM_RESULT_E    result;     /*!< ������ */
    uint32              item_index; /*!< item���� */
}GUILIST_ANIM_DISP_RESULT_T;

/*! @struct CTRLLIST_GET_DATA_FUNC
@brief �û��Ի�ӿ�
*/
typedef void (*CTRLLIST_GET_DATA_FUNC)(MMITHEME_LIST_CUSTDATA_T* data_ptr);

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Lin.Lin 
@param ctrl_id	[in]			�ؼ�ID
@param max_item_num [in]	�ؼ��������
@param is_dynamic_item_data [in]	�Ƿ�̬����
@retval true/false
@return ���سɹ���ʧ��
@note ��Ϊ��̬����ʱ���ؼ���ʾÿ��item�������򴰿�Ҫ����
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxItem(
                                  MMI_CTRL_ID_T 		ctrl_id,
                                  uint16				max_item_num,
                                  BOOLEAN               is_dynamic_item_data
                                  );

/***************************************************************************//*!
@brief �������ѡ�е�ITEM��
@author Lin.Lin 
@param ctrl_id	[in]	 �ؼ�ID
@param mark_cnt_max	[in]	���ѡ����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMaxSelectedItem(
                                          MMI_CTRL_ID_T 	ctrl_id,
                                          uint16 		mark_cnt_max
                                          );

/***************************************************************************//*!
@brief ���ÿؼ�title��ʾ���
@author Tracy Zhang
@param ctrl_id	[in]	 �ؼ�ID
@param title_style [in]	���
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTitleStyle(
                                     MMI_CTRL_ID_T				ctrl_id,	//IN:
                                     GUILIST_TITLE_STYLE_E	title_style	//IN:
                                     );

/***************************************************************************//*!
@brief Append�ؼ���
@author Great.Tian 
@param ctrl_id	[in]		�ؼ�ID
@param item_ptr	Item [in]	����ָ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_AppendItem(
                                  MMI_CTRL_ID_T		ctrl_id,		// control id
                                  const CTRLLIST_ITEM_T*   item_ptr	// appointed Item
                                  );


/***************************************************************************//*!
@brief Insert�ؼ���
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@param item_ptr [in]	����ָ��
@param pos	[in]	�ƶ�Insertλ��
@retval true/false
@return ���سɹ���ʧ��
@note posС�������
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_InsertItem(
                                  MMI_CTRL_ID_T		 ctrl_id,		// control id
                                  const CTRLLIST_ITEM_T* item_ptr,	// appointed Item
                                  uint16              pos				// appointed Position, from 0 to total_item_num
                                  );

/***************************************************************************//*!
@brief Removeһ��Item
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@param pos	[in]	�ƶ���ɾ�������������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveItem(
                                  MMI_CTRL_ID_T ctrl_id,	// control id
                                  uint16 		index			// Position, from 0 to total_item_num-1
                                  );


/***************************************************************************//*!
@brief Remove����Item��
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RemoveAllItems(
                                      MMI_CTRL_ID_T 		ctrl_id  // control id
                                      );

/***************************************************************************//*!
@brief Replaceһ��Item
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@param replace_item_ptr [in] �����滻������
@param pos	[in]	�ƶ����滻��item����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ReplaceItem(
                                   MMI_CTRL_ID_T		ctrl_id,			// control id
                                   const CTRLLIST_ITEM_T*  replace_item_ptr,	// appointed Item
                                   uint16            pos					//  appointed Position, from 0 to total_item_num-1
                                   );


/***************************************************************************//*!
@brief ���ÿؼ������ϸ����
@author yelianna
@param ctrl_id	[in]	�ؼ�ID
@param item_data_ptr [in]	�������õ�����
@param pos	[in]	�ƶ������õ�item����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemData(
                                   MMI_CTRL_ID_T	   ctrl_id, // control id
                                   const GUILIST_ITEM_DATA_T* item_data_ptr,
                                   uint16           pos
                                   );

/***************************************************************************//*!
@brief ����Item��������
@author yelianna
@param ctrl_id	[in]	�ؼ�ID
@param item_content_ptr  [in]	�������õ�����
@param item_index	[in]	�ƶ������õ�item����
@param item_content_index [in]	�������ݵ����꣨item_content_ptrΪ���飩
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContent(
                                      MMI_CTRL_ID_T	           ctrl_id, // control id
                                      const GUIITEM_CONTENT_T* item_content_ptr,
                                      uint16                    item_index,
                                      uint16                    item_content_index
                                      );

/***************************************************************************//*!
@brief ����Item��������,����͸������
@author yelianna
@param ctrl_id  [in]	  �ؼ�ID
@param item_content_ptr  [in]   �������õ�����
@param item_index   [in]	  �ƶ������õ�item����
@param item_content_index [in]  �������ݵ����꣨item_content_ptrΪ���飩
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContentWithAlpha(
	MMI_CTRL_ID_T			 ctrl_id, // control id
	const GUIITEM_CONTENT_T *item_content_ptr,
	uint16					  item_index,
	uint16					  item_content_index
);                               

/***************************************************************************//*!
@brief ����Item��������
@author xiaoqing.lu
@param ctrl_id	[in]	�ؼ�ID
@param item_content_ptr  [in]	�������õ�����
@param item_index	[in]	�ƶ������õ�item����
@param item_content_index [in]	�������ݵ����꣨item_content_ptrΪ���飩
@param is_syn_decode	[in]	�Ƿ��첽
@param is_update [in]	�Ƿ�����ˢ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemContentExt(
                                         MMI_CTRL_ID_T	         ctrl_id, // control id
                                         const GUIITEM_CONTENT_T *item_content_ptr,
                                         uint16                  item_index,
                                         uint16                  item_content_index,
                                         BOOLEAN                 is_syn_decode, // ֻ�Զ����ؼ���Ч
                                         BOOLEAN                 is_update // ֻ�Զ����ռ���Ч
                                         );

/***************************************************************************//*!
@brief ��ñ�ѡ�е��������
@author Great.Tian
@param ctrl_id	[in]	�ؼ�ID
@param idx_ptr [in/out]	���ڴ���������������飬����Ϊ��
@param array_len	[in]	�����С
@retval uint16����
@return ���ر�ѡ�е�������
@note �û������ȷ����������鵽��Ӧ�ö����ô�����ȵ��øýӿڻ��������Ȼ���ٷ����ڴ��ȡ���屻ѡ�����������
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemIndex(
                                           MMI_CTRL_ID_T    ctrl_id,    //in:
                                           uint16*		    idx_ptr,    //in/out:selected index array,may PNULL
                                           uint16		    array_len   //in
                                           );


/***************************************************************************//*!
@brief ����ĳһ��Ϊѡ����
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@param item_pos  [in]	�����õ�����
@param is_selected	[in]	TRUE : selected; FALSE: unselected
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSelectedItem(
                                       MMI_CTRL_ID_T 	ctrl_id,
                                       uint16 			item_pos,		//from 0 to total_item_num-1
                                       BOOLEAN 			is_selected		//TRUE : selected; FALSE: unselected
                                       );

/***************************************************************************//*!
@brief ���һ��Itemָ��
@author James.Zhang
@param ctrl_id	[in]	�ؼ�ID
@param index  [in]	����
@retval ָ��
@return ����ĳһ���ָ��
@note 
*******************************************************************************/
PUBLIC const CTRLLIST_ITEM_T* CTRLLIST_GetItem(
                                               MMI_CTRL_ID_T ctrl_id,
                                               uint16        index  //from 0 to total_item_num-1
                                               );


/***************************************************************************//*!
@brief ��õ�ǰ������Item����
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@retval uint16
@return ����Item����
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurItemIndex(
                                      MMI_CTRL_ID_T 		ctrl_id
                                      );


/***************************************************************************//*!
@brief ���õ�ǰ����������
@author  Lin.Lin
@param ctrl_id	[in]	�ؼ�ID
@param current_index	[in]	�����õ�������
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurItemIndex(
                                       MMI_CTRL_ID_T 		ctrl_id,
                                       uint16 				current_index   //current index
                                       );

/***************************************************************************//*!
@brief �ı���ʾ����
@author Guanyong.Zhang 
@param ctrl_id	[in]	�ؼ�ID
@param type	[in]	��ʾ����
@param is_flash [in]	�Ƿ�����ˢ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_ChangeDisplayType(
                                         MMI_CTRL_ID_T		ctrl_id,
                                         GUILIST_TYPE_E	type,
                                         BOOLEAN				is_flash       // if display immediately
                                         );


/***************************************************************************//*!
@brief ���һҳ��ʾ������
@author Lin.Lin 
@param ctrl_id	[in]	�ؼ�ID
@retval uint16
@return ��������
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetPageItemNum(
                                     MMI_CTRL_ID_T 		ctrl_id // control id
                                     );


/***************************************************************************//*!
@brief ��ȡ����Item����
@author Lin.Lin 
@param ctrl_id	[in]	�ؼ�ID
@retval uint16
@return ����������
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetTopItemIndex(
                                      MMI_CTRL_ID_T 		ctrl_id
                                      );


/***************************************************************************//*!
@brief ���ö��˵�Item
@author Lin.Lin 
@param ctrl_id	[in]	�ؼ�ID
@param top_index	[in]	�����õ�Item
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTopItemIndex(
                                       MMI_CTRL_ID_T 		ctrl_id,
                                       uint16 				top_index   //top index
                                       );


/***************************************************************************//*!
@brief I��ǰItem�Ƿ���Ч
@author Lianxiang
@param ctrl_id	[in]	�ؼ�ID
@retval true/false
@return ���سɹ���ʧ��
@note ��Ч��Item���һ���ʾ������ִ�а��������ʲ���
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsCurrItemInvalid(
                                         MMI_CTRL_ID_T 		ctrl_id
                                         );


/***************************************************************************//*!
@brief �Ƿ���ʾTitle��Ϣ
@author Lin.Lin
@param ctrl_id	[in]	�ؼ�ID
@param title_index_type	[in]	Title��Ϣ��ʾ������
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetTitleIndexType(
                                      MMI_CTRL_ID_T		ctrl_id,
                                      GUILIST_TITLE_INDEX_E title_index_type
                                      );

/***************************************************************************//*!
@brief �Ƿ���Ҫ��ʾ������
@author Lin.Lin 
@param ctrl_id	[in]	�ؼ�ID
@param is_need_prgbar 	[in]	TRUE����ʾ��FALSE������ʾ
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbar(
                                  MMI_CTRL_ID_T  		ctrl_id,
                                  BOOLEAN 			is_need_prgbar
                                  );

#ifdef ADULT_WATCH_SUPPORT
/***************************************************************************//*!
@brief set progressbar theme
@author fulu.song
@param ctrl_id	[in]	�ؼ�ID
@param theme 	[in]	theme to apply
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetPrgbarTheme(
    MMI_CTRL_ID_T  ctrl_id,
    MMITHEME_PRGBOX_TYPE_E theme
);
#endif
/***************************************************************************//*!
@brief �������Item��
@author Tracy Zhang
@param ctrl_id [in]	�ؼ�ID
@retval uint16
@return ����������
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalItemNum(							//RETURN: 
                                      MMI_CTRL_ID_T	ctrl_id		//IN:
                                      );

/***************************************************************************//*!
@brief ���������ѡ��
@author hanbing.zhang
@param ctrl_id	[in]	�ؼ�ID
@param is_select_all	[in]	TRUE�����б�ѡ�У�FALSE��ȡ��ѡ������
@retval void
@return void
@note �������ѡ�����У���ô������������ӵ�һ����ʼѡ��
*******************************************************************************/
PUBLIC void CTRLLIST_SetAllSelected(
                                   MMI_CTRL_ID_T ctrl_id,
                                   BOOLEAN       is_select_all //TRUE : selected; FALSE: unselected
                                   );


/***************************************************************************//*!
@brief  ���ÿؼ��Ƿ���Ա��
@author hanbing.zhang
@param ctrl_id	[in]	�ؼ�ID
@param is_enable_mark	[in]	TRUE�����Ա���ǣ�FALSE�����ܱ����
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextListMarkable(
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN       is_enable_mark
                                           );


/***************************************************************************//*!
@brief  ���ÿؼ�Itemǰ�����ֵ���ʼֵ
@author James.Zhang
@param ctrl_id	[in]	�ؼ�ID
@param num_start_index	[in]	��ʼֵ
@retval void
@return void
@note ϵͳֻ����1-20�����ֵ�ͼƬ��ԴID��list�ؼ�ʹ��
*******************************************************************************/
PUBLIC void CTRLLIST_SetNumberListStartIndex(
                                            MMI_CTRL_ID_T ctrl_id,
                                            uint16        num_start_index
                                            );


/***************************************************************************//*!
@brief ����Softkey�Ƿ���Ӧ�ÿ�����ʾ
@author hanbing.zhang
@param ctrl_id	[in]	�ؼ�ID
@param is_own_softkey	[in]	TRUE:�����Զ���softkey��FALSE:ʹ�ÿؼ�softkey
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetOwnSofterKey(
                                    MMI_CTRL_ID_T ctrl_id,
                                    BOOLEAN       is_own_softkey
                                    );

/*****************************************************************************/
//  Description : ���ÿؼ��Ƿ���Ҫ�ָ���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLLIST_SetNeedSplidLine(
                                     MMI_CTRL_ID_T ctrl_id,
                                     BOOLEAN       is_true
                                     );

/*****************************************************************************//*!
@brief Set the item state
@author xiaoqing.lu
@param ctrl_id	[in]	�ؼ�ID
@param item_index [in]		item ����
@param src_state [in]		Ҫ���õ�״̬
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemStateById(
                                        MMI_CTRL_ID_T   ctrl_id, 
                                        uint32          item_index, 
                                        GUIITEM_STATE_T src_state,
                                        BOOLEAN is_true
                                        );

/***************************************************************************//*!
@brief ���ÿؼ���״̬
@author James.Zhang
@param list_ptr	[in]	�ؼ�ָ��
@param src_state	[in]	Ҫ���õ�״̬
@param is_true  [in]	 ����/ȡ������
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetListState( 
                                 MMI_CTRL_ID_T   ctrl_id,
                                 GUILIST_STATE_T    src_state,
                                 BOOLEAN is_true
                                 );


/***************************************************************************//*!
@brief ��ÿؼ���ĳ��״̬�Ƿ�����
@author James.Zhang
@param list_ptr	[in]	�ؼ�ָ��
@param src_state	[in]	Ҫ��õ�״̬
@retval true/false
@return TRUE���ؼ����и�״̬��FALSE���ؼ�û�и�״̬
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListState(
                                    MMI_CTRL_ID_T   ctrl_id,
                                    GUILIST_STATE_T    src_state
                                    );


/***************************************************************************//*!
@brief ��ÿؼ���ĳ��״̬�Ƿ�����
@author James.Zhang
@param ctrl_id	[in]	�ؼ�ID
@param src_state [in]		Ҫ��õ�״̬
@retval true/false
@return TRUE���ؼ����и�״̬��FALSE���ؼ�û�и�״̬
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetListStateById(
                                        MMI_CTRL_ID_T   ctrl_id,
                                        GUILIST_STATE_T src_state
                                        );


/***************************************************************************//*!
@brief  ��ÿؼ�����û��Զ�������
@author peng.chen
@param iguictrl_ptr	[in]	�ؼ�ָ��
@param index	 [in]	Ҫ��õ�Item����
@param user_data_ptr	[in]	�û����ݣ�����Ϊ�κ����ͣ��û��Զ���
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemData(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 index,
                                   uint32* user_data_ptr
                                   );


/***************************************************************************//*!
@brief  ���ÿؼ�����û��Զ�������
@author peng.chen
@param iguictrl_ptr	[in]	�ؼ�ָ��
@param index	 [in]	Ҫ��õ�Item����
@param user_data_ptr	[in]	�û����ݣ�����Ϊ�κ����ͣ��û��Զ���
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemUserData(
                                   MMI_CTRL_ID_T ctrl_id,
                                   uint16 index,
                                   uint32* user_data_ptr
                                   );

/***************************************************************************//*!
@brief ������ǰItem����
@author Lin.Lin
@param ctrl_id [in]	�ؼ�ID
@retval void
@return void
@note ��ɾ��ĳһ����ʱ�����ܴ��ڵ�ǰItem���ô���������ͨ���ýӿڿ��Խ���
*******************************************************************************/
PUBLIC void CTRLLIST_ValidCurItemPos( 
									MMI_CTRL_ID_T	ctrl_id 
									);

/***************************************************************************//*!
@brief ���ÿؼ��ı���ɫ
@author James.Zhang
@param ctrl_id [in]	�ؼ�ID
@param back_color [in]	����ɫ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgColor(
                                  MMI_CTRL_ID_T	ctrl_id ,
                                  GUI_COLOR_T   back_color
                                  );

/***************************************************************************//*!
@brief ����List�ؼ���Ĭ��ͼƬID
@author xiaoqing.lu
@param ctrl_id [in]	�ؼ�ID
@param def_img_id [in]	��������е�Ĭ��ͼƬ
@param err_img_id [in]	����ʧ�ܺ��Ĭ��ͼƬ
@retval true/false
@return ���سɹ���ʧ��
@note ����Ĭ��ͼƬ����Ϊ��̬����ͼƬʱ����Ƕ�Ķ����ؼ�����
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIcon(
                                      MMI_CTRL_ID_T     ctrl_id,        //in
                                      MMI_IMAGE_ID_T    def_img_id,  // default image id
                                      MMI_IMAGE_ID_T    err_img_id   // error image id
                                      );
/*****************************************************************************/
//  Description : ����List�ؼ���Ĭ��ͼƬID,Ϊ���ܹ���image_idΪNULL,�ܹ����óɹ�
//  Global resource dependence :
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDefaultIconEx(
    MMI_CTRL_ID_T     ctrl_id, //in
    MMI_IMAGE_ID_T    def_img_id,  // default image id
    MMI_IMAGE_ID_T    err_img_id   // error image id
);

/***************************************************************************//*!
@brief �����Ƿ���Ҫ��ʾ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true [in] TRUEҪ��ʾ��������FALSE��Ҫ��ʾ��Ĭ��ΪTRUE
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedHiLightBar(
                                      MMI_CTRL_ID_T  ctrl_id,
                                      BOOLEAN is_true
                                      );


/***************************************************************************//*!
@brief  ���ÿ��б���Ϣ
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param empty_ptr [in] ���б���Ϣ
@retval void
@return void
@note ���ڴ���open��Ϣʱ����
*******************************************************************************/
PUBLIC void CTRLLIST_SetEmptyInfo(
                                 MMI_CTRL_ID_T               ctrl_id,   //�ؼ�id
                                 const GUILIST_EMPTY_INFO_T* empty_ptr  //���б���Ϣ
                                 );

/***************************************************************************//*!
@brief �����Ƿ���ʾ�������Ļ���
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true [in] TRUE��ʾ��FALSE����ʾ��Ĭ��ΪTRUE
@retval void
@return void
@note ����ӿ������Ƿ���Ҫ�������ϵĻ���
*******************************************************************************/
PUBLIC void CTRLLIST_SetNeedPrgbarBlock(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN is_true
                                       );


/***************************************************************************//*!
@brief ���ñ���ͼƬ��ԴID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ͼƬ��ԴID
@param is_transparent [in] �Ƿ�͸����ʾ
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetBgImage(
                                  MMI_CTRL_ID_T	ctrl_id,
                                  MMI_IMAGE_ID_T image_id,
                                  BOOLEAN		 is_transparent
                                  );

/*****************************************************************************/
//  Description : set the user bg 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetUserBg(
                               MMI_CTRL_ID_T	ctrl_id,
                               BOOLEAN		is_user_bg
                               );

/***************************************************************************//*!
@brief ������ʾ������ʱ���Ƿ�ͬ�����룬Ĭ�����첽����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_sync_decode [in] TRUE��ͬ����FALSE���첽
@retval true/false
@return ��������֮ǰ��״̬
@note ��������֮ǰ��״̬
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSyncDecodeAnim(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         BOOLEAN is_sync_decode
                                         );

/*****************************************************************************//*!
@brief list�ϵ�Item�����Ƿ���Ҫ���ţ�Ĭ��������ǲ����ŵ�
@author fangfang.yao
@param ctrl_id [in] �ؼ�ID
@param is_zoom [in] TRUE�����ţ�FALSE��������
@retval TRUE/FALSE
@return �����Ƿ�ɹ�
@note �����Ƿ�ɹ�
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetZoomAnim(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_zoom
);

/***************************************************************************//*!
@brief �����Ƿ�ѭ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_cyc_scroll [in] TRUE��ѭ����FALSE����ѭ��
@note Ĭ��ʱѭ�����������ؼ���ѭ������������������/����ʱ���ᷢ����Ϣ��������
*******************************************************************************/
PUBLIC void CTRLLIST_SetCycScroll(
                                  MMI_CTRL_ID_T  ctrl_id,
                                  BOOLEAN is_cyc_scroll
                                  );


/***************************************************************************//*!
@brief �����Ƿ���Ӧ���Ұ���
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param is_handle [in] TRUE����Ӧ��FALSE������Ӧ
@note Ĭ�ϲ���Ӧ���Ұ���
*******************************************************************************/
PUBLIC void CTRLLIST_SetHandleLeftRight(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN        is_handle
                                       );

/***************************************************************************//*!
@brief �����Ƿ�������ʾ�߿�
@author Jassmine
@param ctrl_id		[in] �ؼ�ID
@param is_permit	[in] �Ƿ�������ʾ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_PermitBorder(
									MMI_CTRL_ID_T	ctrl_id,
                                    BOOLEAN         is_permit
                                    );

/***************************************************************************//*!
@brief ���ÿؼ����������ɫ
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param font [in] ����
@param font_color [in] ������ɫ
@retval true/false
@return ���سɹ���ʧ��
@note  Ĭ�ϲ���Ҫ����.��GUILIST_STATE_TEXT_STYLE״̬ͬʱʹ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetTextFont(
                                   MMI_CTRL_ID_T   ctrl_id,
                                   GUI_FONT_T      font,
                                   GUI_COLOR_T     font_color
                                   );
/***************************************************************************//*!
@brief ���ÿؼ���ǰitem��������ɫ
@author dandan.cao
@param ctrl_id [in] �ؼ�ID
@param font [in] ����
@param font_color [in] ������ɫ
@retval true/false
@return ���سɹ���ʧ��
@note  Ĭ�ϲ���Ҫ����.��GUIITEM_STATE_COSTMER_FONT_COLOR״̬ͬʱʹ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLIST_SetOneItemFontColor(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                uint32                 item_index,
                                                GUI_COLOR_T     item_font_color,
                                                BOOLEAN            is_true
                                            );
/*****************************************************************************//*!
@brief ����list text font color
@author xiaoqing.lu
@param ctrl_id	[in]	�ؼ�ID
@param font_color [in]		item ������ɫ
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note Ĭ�ϲ���Ҫ����, ��GUIITEM_STATE_COSTMER_FONT_COLOR״̬ͬʱʹ��
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemTextFontColor(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_COLOR_T     font_color
                                            );

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�HANDLE
@param rect_ptr [in] ����
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetRect(
                            MMI_HANDLE_T    ctrl_handle,
                            GUI_RECT_T      *rect_ptr
                            );


/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�֧��*,\#�л�ҳ
@author Jassmine
@param ctrl_handle [in] �ؼ�HANDLE
@param is_handle [in] TRUE:֧�֣�FALSE:��֧��
@retval true/false
@return ���سɹ���ʧ��
@note Ĭ��֧��
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetHandleStarHash(
                                         MMI_HANDLE_T   ctrl_handle,
                                         BOOLEAN        is_handle
                                         );


/***************************************************************************//*!
@brief �����Ƿ�֧��slide״̬
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�HANDLE
@param is_slide_used [in] TRUE:֧�֣�FALSE:��֧��
@retval true/false
@return ���سɹ���ʧ��
@note  Ĭ�����������֧��slide״̬
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetSlideState(
                                     MMI_CTRL_ID_T ctrl_id,
                                     BOOLEAN is_slide_used
                                     );

/***************************************************************************//*!
@brief ��ȡ�Ƿ�֧��slide״̬
@author Xiaoming.Ren
@param ctrl_id [in] �ؼ�HANDLE
@retval true/false
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetSlideState(
                                     MMI_CTRL_ID_T ctrl_id
                                     );

/***************************************************************************//*!
@brief ���ö���Item��ƫ��
@author bin.ji
@param ctrl_id [in] �ؼ�ID
@param offset_y [in] Y�����ƫ��
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetTopItemOffset(
                                      MMI_CTRL_ID_T ctrl_id,
                                      int32 offset_y
                                      );


/***************************************************************************//*!
@brief get top item offset
@author xiaoqinglu
@param ctrl_id [in] �ؼ�ID
@param offset_y_ptr [in/out] Y�����ƫ��
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetTopItemOffset(
                                        MMI_CTRL_ID_T ctrl_id,
                                        int32* offset_y_ptr
                                        );

/***************************************************************************//*!
@brief �����Ƿ�֧��3d
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param is_disable_3d [in] TRUE֧�֣�FALSE��֧��
@retval true/false
@return ���سɹ���ʧ��
@note Ĭ��֧��
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetDisable3D(
                                    MMI_CTRL_ID_T ctrl_id,
                                    BOOLEAN is_disable_3d
                                    );

/*****************************************************************************/
//  Description : set scroll bar style
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetListScrollBarStyle(
                                    MMI_CTRL_ID_T	  ctrl_id, // control id
                                    GUIPRGBOX_STYLE_E scroll_style
                                    );

/***************************************************************************//*!
@brief ���ñ߾�
@author Lin.Lin
@param ctrl_id [in] �ؼ�ID
@param left_space [in] ��߾�
@param right_space [in] �ұ߾�
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetMargin(
                                 MMI_CTRL_ID_T ctrl_id, // control id
                                 uint16        left_space,
                                 uint16        right_space
                                 );


/***************************************************************************//*!
@brief ������Ƕ�������ı���ͼ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ��ԴͼƬID
@retval true/false
@return ���سɹ���ʧ��
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetPrgBarImageId(
                                        MMI_CTRL_ID_T	ctrl_id,
                                        MMI_IMAGE_ID_T image_id
                                        );


/***************************************************************************//*!
@brief ����Item�Ƿ�һ�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ����
@param is_gray [in] �Ƿ�һ�
@retval true/false
@return ���سɹ���ʧ��
@note �����ûһ���Item������ִ�а��������ʲ���
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemGray(
                                   MMI_CTRL_ID_T ctrl_id, 
                                   uint32 item_index, 
                                   BOOLEAN is_gray
                                   );

/***************************************************************************//*!
@brief ����ITEM�Ƿ���Ч
@author Great.Tian
@param ctrl_id [in] �ؼ�ָ��
@param pos [in] ����
@param is_invalid [in] �Ƿ���Ч
@retval true/false
@return ���سɹ���ʧ��
@note ��������Ч��Item������ִ�а��������ʲ���
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemInvalid(
                                      MMI_CTRL_ID_T ctrl_id,	// control id
                                      uint16        pos,		// ָ����λ��, from 0 to total_item_num-1
                                      BOOLEAN       is_invalid
                                      );


/***************************************************************************//*!
@brief ���Item����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in]  ����
@param rect_ptr [in/out] ���ε�ָ��
@retval true/false
@return ���سɹ���ʧ��
@note Item���������ʱ����ı�
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetItemRect(
                                   MMI_CTRL_ID_T  ctrl_id,    //in:
                                   uint16		  item_index, //in
                                   GUI_RECT_T     *rect_ptr     //in/out
                                   );

/***************************************************************************//*!
@brief �ı��ѡ���Ĭ��softkey ID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_cancel_id  [in] TEXT ID
@param text_select_id [in] TEXT ID
@param text_return_id [in] TEXT ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListDefSoftkeyID(
                                             MMI_CTRL_ID_T ctrl_id,
                                             MMI_TEXT_ID_T       text_cancel_id,
                                             MMI_TEXT_ID_T       text_select_id,
                                             MMI_TEXT_ID_T       text_return_id
                                             );
/*****************************************************************************/
//  Description : set check list default image
//  Global resource dependence :
//  Author:miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImage(
                                             MMI_CTRL_ID_T ctrl_id,
                                             MMI_IMAGE_ID_T       image_unselected,
                                             MMI_IMAGE_ID_T       image_selected
                                             );
/*****************************************************************************/
//  Description : set check list image display left or right
//  Global resource dependence :
//  Author:miao.xi
//  Note:    BOOLEAN  is_left: (1:left 0:right)
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListImagePosition(
                                             MMI_CTRL_ID_T ctrl_id,
                                             BOOLEAN       is_left// 1:left 0:right
                                             );
/***************************************************************************//*!
@brief �ı䵥ѡ���Ĭ��softkey ID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_ok_id [in] TEXT ID 
@param text_return_id  [in/out] TEXT ID
@retval void
@return void
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_ChangeRadioListDefSoftkeyID(
                                                MMI_CTRL_ID_T ctrl_id,
                                                MMI_TEXT_ID_T       text_ok_id,
                                                MMI_TEXT_ID_T       text_return_id
                                                );
                                                                                             
/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetSwitchItemTime(
                                      MMI_CTRL_ID_T ctrl_id,
                                      uint32 switch_time
                                      );

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StartSwitchItem(
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
//  Description : stop auto switch timer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_StopSwitchItem(
                                   MMI_CTRL_ID_T ctrl_id
                                   );

/*****************************************************************************/
//  Description : �����Ƿ��Զ��л�item
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: Ĭ��ʱ���Զ��л�
/*****************************************************************************/
PUBLIC void CTRLLIST_SetAutoSwitchItem(
                                      MMI_CTRL_ID_T  ctrl_id,
                                      BOOLEAN is_true
                                      );

/*****************************************************************************/
//  Description : ����ܵ�ҳ��
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: ÿҳ��ʾ����Ŀ����list����item�߶ȼ�������
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetTotalPageNum(							//RETURN: 
                                      MMI_CTRL_ID_T	ctrl_id		//IN:
                                      );

/*****************************************************************************/
//  Description : ��õ�ǰҳ������
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: �Ե�ǰҳ��������һ��ItemΪ׼����
/*****************************************************************************/
PUBLIC uint16 CTRLLIST_GetCurrentPageIndex(							//RETURN: 
                                          MMI_CTRL_ID_T	ctrl_id		//IN:
                                          );

/***************************************************************************//*!
@brief ��ȡ������item index
@author lianxiang.zhou
@param ctrl_id [in] �ؼ�ID
@param gross_index  [in] TEXT ID
@param pure_index_ptr [in] TEXT ID
@retval true/false
@return �����Ƿ����˴�����index
@note ��ν�����ģ�������split���
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetPureIndex(
                                    MMI_CTRL_ID_T ctrl_id,
                                    uint16 gross_index,
                                    uint16 *pure_index_ptr
                                    );

/***************************************************************************//*!
@brief �����Ƿ�֧�ַָ�������Ч����Ĭ�ϲ�֧��
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true  [in] ���û�ȡ������
@retval true/false
@return 
@note 
*******************************************************************************/
PUBLIC void CTRLLIST_SetSplitOnside(
                                    MMI_CTRL_ID_T	ctrl_id,
                                    BOOLEAN    is_true
                                    );

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu 
@param base_win_handle	[in] Must valid win handle, if not, create failed
@param base_ctrl_handle [in] You can set 0
@param ctrl_id          [in] You can set 0
@param init_data_ptr    [in] if null, will create ctrl by default param
@retval 0 or valid handle
@return ����0������Ч�Ŀؼ�ID
@note ע�⣬����ʼ������init_data_ptrΪ��ʱ��������һ���յĿؼ����ÿؼ��������Item
*******************************************************************************/
PUBLIC MMI_HANDLE_T CTRLLIST_CreateListBox(
                                          MMI_HANDLE_T base_win_handle, 
                                          MMI_HANDLE_T base_ctrl_handle,
                                          MMI_CTRL_ID_T ctrl_id,
                                          GUILIST_INIT_DATA_T *init_data_ptr
                                          );


/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu 
@param ctrl_id          [in] ctrl ID
@retval true or false
@return 
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_IsTpDown(
                                MMI_CTRL_ID_T ctrl_id
                                );

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu 
@param ctrl_id          [in] ctrl ID
@retval item number
@return item number
@note 
*******************************************************************************/
PUBLIC uint16 CTRLLIST_GetSelectedItemNum(
                                         MMI_CTRL_ID_T    ctrl_id//in:
                                         );

/*****************************************************************************/
//  Description : �����Ƿ����û�built item layer
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: Ĭ��FALSE,�ؼ��Զ�built
/*****************************************************************************/
PUBLIC void CTRLLIST_SetCusBltItemLayer(
                                       MMI_CTRL_ID_T  ctrl_id,
                                       BOOLEAN        is_cus_blt_layer
                                       );
                                       
/*****************************************************************************//*!
@brief ���ÿؼ���Item��ʾ��layer
@author Xiaoqing.lu 
@param ctrl_id              [in] ctrl ID
@param lcd_dev_info_ptr     [in] lcd_dev_info_ptr ID
@retval item number
@return item number
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemLayer(
                                    MMI_CTRL_ID_T ctrl_id,
                                    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                    );

/*****************************************************************************//*!
@brief ���ÿؼ���Item��ʾ��layer
@author Xiaoqing.lu 
@param ctrl_id              [in] ctrl ID
@param get_data_func     [in] func ptr
@retval true/false
@return true/false
@note 
*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_RegGetDataFunc(
                                      MMI_CTRL_ID_T ctrl_id,
                                      CTRLLIST_GET_DATA_FUNC own_draw_func
                                      );

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:����list֧�ֺ��򻬶���Ĭ�ϲ�֧��
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemHorMove( 
                                   MMI_CTRL_ID_T ctrl_id,
                                   BOOLEAN         is_true
                                   );

/*****************************************************************************/
//  Description : CTRLLIST_SetItemLayer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetItemLayer(
                                    MMI_CTRL_ID_T ctrl_id,
                                    GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                    );

/***************************************************************************//*!
@brief ���ý�ֹtip��ʾ��״̬
@author Lianxiang.Zhou
@param ctrl_id       [in] ctrl ID
@param is_forbid     [in] �Ƿ��ֹtip
@retval true/false
@return true/false
@note Ĭ������ʾ��
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetForbidTip(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN       is_forbid
);
                                               
#ifdef ATEST_SUPPORT
/*****************************************************************************/
// 	Description : get item content rect
//	Global resource dependence : 
//  Author: lin.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Atest_GetItemContentRect(
                                 MMI_CTRL_ID_T   ctrl_id,
                                 uint16          item_index,
                                 uint16          item_content_index,
                                 GUI_RECT_T*     disp_rect_ptr,//out
                                 GUI_RECT_T*     clip_rect_ptr //out
                                 );
#endif //ATEST_SUPPORT

/***************************************************************************//*!
@brief ���Listλ����Ϣ
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@retval 
@return
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_GetCurPosInfo(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUILIST_POS_INFO_T *pos_info_ptr
                                      );

/***************************************************************************//*!
@brief ����Listλ����Ϣ
@author Great.Tian 
@param ctrl_id	[in]	�ؼ�ID
@retval 
@return
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetCurPosInfo(
                                       MMI_CTRL_ID_T ctrl_id,
                                       GUILIST_POS_INFO_T *pos_info_ptr
                                       );

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void CTRLLIST_ChangeCheckListSoftkeyIndex(
                                                MMI_CTRL_ID_T ctrl_id,
                                                GUISOFTKEY_INDEX  select_cancel_index
                                                );

/*****************************************************************************/
//  Description : ����Item ��ʾ���
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint32 item_index,
    THEMELIST_ITEM_STYLE_T *style_ptr
);

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:����list֧�����󻬶���Ĭ�ϲ�֧�֣��ú�������� CTRLLIST_SetItemHorMove() ����ʹ��
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemLeftMove(
                                                    MMI_CTRL_ID_T ctrl_id,
                                                    BOOLEAN       is_true
                                                );

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author: fangfang.yao
//  Note: item�����ʱ��list�Ƿ���Ҫ������״̬��Ĭ������²���ʾ������
/*****************************************************************************/
PUBLIC void CTRLLIST_SetItemIsNeedHighBarIfClick(
                                                                MMI_CTRL_ID_T ctrl_id,
                                                                BOOLEAN       is_true
                                                            );

/*****************************************************************************/
//  Description : whether support dec anim when sliding or not
//  Global resource dependence :
//  Author:fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLIST_SetIsSupportSlideDecAnim(
                                            MMI_CTRL_ID_T ctrl_id,
                                            BOOLEAN is_support_slide_dec_anim
                                          );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _CTRLLIST_EXPORT_H_  */


/*@}*/



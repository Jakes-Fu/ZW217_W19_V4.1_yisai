 /*! @file guimenu.h
@brief ���ļ�������menu�ؼ��Ķ���ӿں���������
@author Jassmine
@version 1.0
@date 11/2006
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 11/2006      Jassmine              Creat
-# 04/2010      hua.fang        add function: SetItemVisible/SetNodeVisible


**                              SetItemGrayed
*******************************************************************************/

/*! @page page_Menu �˵�(Menu)

��Mocorƽ̨��MMI���������У�Menu�ؼ����ž������صĵ�λ������MMI�������õĿؼ�֮һ������������Ҫ�ԣ����ǰ�����Ϊ��һ���ؼ����ܸ��û���

-# @ref sec_menu_function
-# @ref sec_menu_intro
-# @ref sec_menu_grid
    -# @ref ssec_menu_grid_intro
    -# @ref ssec_menu_grid_item
    -# @ref ssec_menu_grid_item_def
    -# @ref ssec_menu_grid_layout
    -# @ref ssec_menu_grid_modify
-# @ref sec_menu_title
-# @ref sec_menu_slide
    -# @ref ssec_menu_slide_intro
    -# @ref ssec_menu_slide_item
    -# @ref ssec_menu_slide_level_1
    -# @ref ssec_menu_slide_level_2
    -# @ref ssec_menu_slide_item
    -# @ref ssec_menu_slide_layout
    -# @ref ssec_menu_slide_modify
-# @ref sec_menu_list
-# @ref sec_menu_popup
    -# @ref ssec_menu_popup_item
    -# @ref ssec_menu_popup_item_def
    -# @ref ssec_menu_popup_create
    -# @ref ssec_menu_popup_modify
-# @ref sec_menu_link
-# @ref sec_menu_function_list


@section sec_menu_function �ؼ��Ĺ���

�˵��ؼ���GUI������Ŀؼ�����Mocor 10A��ǰ��ƽ̨�У��󲿷ִ��ڶ�����Menu�ؼ���Ϊ�����������ġ����ǰѲ˵���Ϊ���˵��������˵��������˵���������ʽ��

Mocor10A�У��ṩ���������͵����˵����ֱ��ǹ���˵���Title���͵����˵����Լ������˵���

����˵����û��ɸ���LCD Size���ⶨ�幬��˵���ʾ�����ͣ���9����12����16����ȵȡ�����˵���ѡ�������Դﵽ36���������Ϊ�û����õĹ����������������ӵ�ѡ���ͨ�������л�����򴥱ʵ���������ķ�����Ϊ���ӡ�

Title���͵����˵�����ʵ�����ǹ���˵������죬��Ϊ�ܶ�С�����ֻ��С����߶������ʱ�������������ʾ��ÿ��ѡ����·���������Ļ�Ĵ�С���ַ��������޷���ʾ��������������Title���͵����˵���ֻ����ѡ�������˵����ʾ�ڴ��ڵ��Ϸ���

�����˵���Mocor 10A���������˵����ͣ��������˵����ص��ǣ�ÿҳ����ʾ��Ŀ�̶�������9��12��16�ȣ���ҳ���ɶ�̬���ӣ��������ڴ�ռ䣬Ӧ���и����ֵ�ġ�Ŀǰ���Ƕ������10ҳ�����������û�������

�����˵������ʹ�˵��Ĳ��������в���ԣ�����ʵ���˲˵��ļ���Ч������������ͬһ���˵�������ֱ�ӽ����¼��˵���������Ҫ���´����µĴ��ڡ�

Pop�˵���Ϊ������ƽ̨�Ķ����Է�����Ƶģ��书���ڶ����˵�û��ʵ���ϵ�ȴ�𡣵���������ڵ���ʾЧ����ʹ�˻����������ø���Ѥ�����ˡ�ͬ����PopmenuҲ֧�ֶ༶������Ч����

�Ӳ˵�����˼���������˵����¼��˵���Ҳ���Գ�Ϊ�����˵�������ƵĽǶ��������Ӳ˵������ʹ����ͬ���͵Ĺ����ܹ�ͳһ��һ��MMI��ڵ��С������˵���Pop�˵������԰����Ӳ˵�����������ע�⣬���˵��ǲ��ܰ��������˵��ģ������½ڽ��ܡ��Ӳ˵��������ϼ��˵�ͬ����һ�������У�������id��ͬ�����ڴ����ϼ��˵�ʱֱ�Ӵ��������������ⶼ���ϼ��˵���ͬ��

�˵��ؼ��ṩ��Get/Set�ӿ�����ȡ/���ò˵������Լ����⣬����Ӧ��ͨ���ӿں����õ��������λ�õ���Ϣ����̬�˵��ĵ�ǰ���group id��menu id����̬�˵��ĵ�ǰ�ڵ��ţ����������ûҵ���Ч�����ı�Ч����ʾ���⡢����̣����������˵������֣�ѡ���ɫ��ʾ���˵������ֳ�����Ļ��С������ʾ����

@section sec_menu_intro �˵���Ƽ��

-# CREATE_MENU_CTRL
@code
#define CREATE_MENU_CTRL(_GROUP_ID_, _CTRL_ID_) \
        MMK_CREATE_MENU, _GROUP_ID_, _CTRL_ID_
@endcode
    @arg _CTRL_ID_ ��ǰ�ؼ�ID��ΪMMI_CONTROL_ID_Eö������
    @arg _GROUP_ID_ �˵����ID
-# @ref GUIMENU_STYLE_E
-# @ref GUIMENU_ITEM_T
-# @ref GUIMENU_GROUP_T

@note ���ڲ˵���Դ�Ķ���:
- �˵������о�̬�˵��Ͷ�̬�˵��������ͣ���̬�˵�����Դ����Ϊ��̬���ݶ�����ƽ̨�еģ�ƽ̨��ʼ��ʱ��ע��ÿ��ģ��ľ�̬�˵���Դ�������˵��ؼ�ʱ�����ݲ˵����ID��ȡ�˵���Դ��

@section sec_menu_grid �������˵�

��ͼ 2 10��ʾ��ʾ�������ֵ��ǾŹ�����ʽ����ν�ľŹ���˵����������Ÿ�ѡ���3��3�еķ�ʽ���У�ÿ�������Ҫ��ʾ��ͼƬ���ı���������������Ϣ��
@image html menu_9_grid.jpg
ͼ 2 10�Ź���˵�

�Ź���˵�������չΪ12����˵�����4��3����ʾ����ȻҲ������չ�����������з�ʽ��

@subsection ssec_menu_grid_intro �������˵�����Ƹ���

- �������˵��Ծ�����ʽ��ʾ����������Ϣ��ÿ��Item��ͼƬ���ı�����ǰitem������ʶ�ȡ�������ʾ����һҳ����Ŀ�������ĿΪ36�����û��ɵ���������һ��ʱ���Ҳ���ʾ����������ͨ��������tp�ද�������ķ�ʽʹ����һ��item�ɼ���
- ���˵�����֯���ܣ���ͨ��TP����item������֯״̬�����϶�ͼ�����λ�ø��������ڴ�������ƽ̨����֯���ܱ����Ρ�

@subsection ssec_menu_grid_item �˵�������

���鶨����mmi_menutable.c�ļ���
@code
const GUIMENU_ITEM_T menu_mainmenu_icon[] =
{
    {ID_COMMUNICATION_CL,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_CL,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_CL,       0,MENU_NULL},
    {ID_COMMUNICATION_PB,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_PB,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PB,       0,MENU_NULL},
    {ID_COMMUNICATION_BROWSER,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_BROWSER,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_BROWSER,  0,MENU_NULL},
    {ID_MAINMENU_PLAYER,        TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_MULTIMEDIA,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PLAYER,   0,MENU_NULL},
    {ID_COMMUNICATION_MESSAGE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_MESSAGE,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_MESSAGE,  0,MENU_NULL},
    {ID_MAINMENU_TOOLS,         TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_APPS,     IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_TOOLS,    0,MENU_NULL},
    {ID_MAINMENU_PASTIME,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_PASTIME,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_GAME,     0,MENU_NULL},
    {ID_ENTERTAIMENT_FAVORITE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_EXPLORER,               IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_FAVORITE, 0,MENU_NULL},
    {ID_MAINMENU_SETTINGS,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_MAINMENU_ICON_SETTINGS, IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_SET,      0,MENU_NULL},
};
@endcode

ÿһ���ʾһ���˵����������������Ϣ�в˵����ID��softkey��ʾ���ı����˵���Ϊ��ǰ����ʱ��ʾ�����ı�������ͼƬ���ǽ���ͼƬ�ȡ�

@note ���˵�ʹ�ø�������ʾ��ʼ������Ϣ����Ĭ�ϵ���Ŀ������һ�ο���֮����빬�����˵����ؼ���ȡ��������Ϣ��Ȼ�󱣴浽NV�У��Ժ��ٴν������˵����ؼ������ȡNV�е������Ϣ������ʾ��

@warning ����һ��ע�����㣺
-# ���˵������м����˵�����ÿ��item�����һ�����ΪMENU_NULL�����򣬽��������ʾ�쳣����Ϊ��ʹ�ö����ʾ���˵��������˵�֮�󣬼����˵�������´��ڣ�����������ݲ����ͷţ��Ӷ�����ʾ���⣬���ڶ�����⣬��ο���������½ڡ�
-# ���˵����е�item�������Ǿ�̬�������Ϣ�����ܶ��Ŵ������˵�Item����Ϊ��NV�ռ�����ƣ�NV��ֻ�����˲˵�Item��ID�����˵����group ID����ʾ��ʱ����Ȼ��Ҫ���������ڵ������ж�ȡ���ݡ�

@subsection ssec_menu_grid_item_def �˵���Ķ���

@code
MENU_DEF( MENU_MAINMENU_ICON, menu_mainmenu_icon, GUIMENU_STYLE_ICON, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_mainmenu_icon))
@endcode

MENU_DEF��Ĳ�����ǰ��������Ϊ��
- �˵���ID
- �˵������ַ
- �˵����ͣ����Կ���ΪGUIMENU_STYLE_ICON,��
- �ĸ��յ��ı���ͼƬ��NULL��ʾ����Ҫ��
- �˵����������

@subsection ssec_menu_grid_layout �������˵�Layout���

����˵�����ʾ����ͼ����ʾ���ı���ʾ�ȣ��û���Ҫ��ͷ�ļ�mmitheme_menu.h�ж��幬��ͼƬ�����������������ʾ�������������磺
@image html menu_grid_layout.jpg
ͼ 2 11����˵�Layout

����ͼ���Կ���RECT_ICON(16, 30, 223, 253)��ͼƬ��ʾ������width_icon = 32;height_icon = 40Ϊ�о���оࡣ�ؼ�������д������Ϣ�����㹬��˵���ÿ��ͼ�����ʾλ�á�

ͬ���ģ�RECT_TEXT(10, 80, 229, 271)�����ֵ���ʾ����width_text = 20; height_text = 72Ϊ���ֵ��о���о࣬�������ؼ��Ϳ��Լ����ÿ��ѡ���������ʾ����

����˵���Layout��Ϣ�������� GUIMENU_ICONMENU_DATA_T �ṹ���С�

@note
-# Title�������˵�ʱ��title_text_rect����Ч������Ӧ��item_text_display_rect���������������ǿ����򡣵���������е��������û�����ģ����ǿؼ��Լ�����ģ��û��ɲ��ع��ġ��෴�أ��������Title���͵Ĺ���˵�����ôtitle_text_rect��һ����һ�������򡣵�Ȼ��Щ���ǿؼ���������ġ�
-# MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM = page_row_num�� page_col_num������һ��Ҫע��ģ����򽫻������ʾ����
-# item_width��item_height��ͨ��MENUICON_WIDTH��MENUICON_HEIGHT��õģ���������Ƚ����⣬�����빤�̵�make file��������ڹ��̵�make file�ж�������SPECIAL_EFFECT_ICON_SIZE = 48X48 �ĺ궨�壬48X48˵��ͼƬ�Ŀ�Ⱥ͸߶�Ϊ48��֮���������涨������3DЧ�������ƣ���Ϊ����3DЧ��ʱ���ײ��ͨ��make file��ϵͳ��ʼ��ʱ��ȡ�����ֵ�����ڴ���䡣�мǣ�

@subsection ssec_menu_grid_modify ����޸ľŹ���˵�

�Ź���˵����ض���Ŀ��ֻ����һ�飬���������˵��ĳ�ʼ��������Դ��

�޸ľŹ���˵��������޸�ͼƬ��ʾλ�á��ı���ʾλ�á��˵���������������λ����Ϣ���Լ��˵����ͼƬ���˵�ͼƬ�ĸ���ͼƬ��ѡ���Ӧ���ı���ѡ����Ϣ��

����޸�λ����Ϣ����ôֻ��Ҫ�޸�mmitheme_menu.h�ļ��еĺ궨�弴�ɣ��к������ģ�Ӧ�ü�ˡ�

����޸�ѡ��������Ϣ��ֻ��Ҫ�޸Ĳ˵��ľ�̬���飬����menu_mainmenu_icon��

@section sec_menu_title Title���͵Ĺ���˵�

����һ�ڵĹ���˵����ƣ�Title���͵Ĺ���˵�ֻ�ǰ�������ʾ�ڴ��ڵ��Ϸ����뿴��ͼ��
@image html menu_title.jpg
ͼ 2 12Title���͵Ĺ���˵�

Title���͵Ĺ���˵��������˹���˵����������ݽṹ������ͬ�ľ���Layout����ƣ�����ͼ��
@image html menu_title_layout.jpg
ͼ 2 13 Title���͵Ĺ���˵���Layout

��ͼ�е�RECT_ICON(16�� 75��223��263)ΪͼƬ��ʾ����width_icon = 32; height_icon = 25;ΪͼƬ���о����оࡣ���Ϸ���title_rect��21,36, 218, 62����ͼ��û������Ǹ�������Ź���һ�°ɣ���Ϊtitle������ʾ����

@section sec_menu_slide �������˵�

�������˵�����ʾ��ʽ�����빬��˵������ǻ����˵�֧�ֶ�ҳ���ܣ�ͼ 2 14�е��·���ʾ��ҳ���͵�ǰҳ��
@image html menu_slide.jpg
ͼ 2 14�������˵�

@subsection ssec_menu_slide_intro �����˵���Ƹ���

�������˵�ͬ����˵����ƣ��Ծ�����ʽ��ʾ����������Ϣͬ����ÿ��Item��ͼƬ���ı�����ǰitem������ʶ�ȡ���ͬ���ǣ������˵�ÿҳ����ʾ��item�������Ƶģ�����Ź������ͣ�������Ϊ9��ͼ�ꡣ

�����˵����ж�ҳ���ܣ�Ŀǰ��ƣ�������ʾ10ҳ����Ӧ�ÿ������������ˡ��û���ͨ�������򴥱ʻ����л�ҳ�档

�������˵�����֯���ܣ���ͨ��TP����item������֯״̬�����϶�ͼ�����λ�ø��������ڴ�������ƽ̨����֯���ܱ����Ρ�

@subsection ssec_menu_slide_item �˵�������

��mmi_menutable.c�ļ��ж����˻����˵��ľ�̬����
-# menu_mainmenu_slide_page���飬���廬���˵���ʼ����ҳ�Լ�������ҳ��˵��
@code
const GUIMENU_ITEM_T menu_mainmenu_slide_page[] =
{
    {ID_MAINMENU_SLIDE_PAGE1, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_COMMUNICATION},
    {ID_MAINMENU_SLIDE_PAGE2, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PASTIME},
    {ID_MAINMENU_SLIDE_PAGE3, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_TOOLS},
    {ID_MAINMENU_SLIDE_PAGE4, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE4},
    {ID_MAINMENU_SLIDE_PAGE5, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE5},
    {ID_MAINMENU_SLIDE_PAGE6, TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},0, 0, 0, 0, MENU_SLIDE_PAGE6},
};
@endcode
-# �Թ����Ĳ˵�MENU_SLIDE_COMMUNICATIONΪ����
@code
const GUIMENU_ITEM_T menu_slide_communication[] = // ע�⣬����������ƣ������Ϊ����������ֵ
{
    // ��һ��Ĭ��ֵ��������	    ͨѶ¼	    ͨ����¼
    {ID_COMMUNICATION_DIAL,     TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMM_DIALPAD,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DIAL,    0,MENU_NULL},
    {ID_COMMUNICATION_PB,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_PB,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_PB,      0,MENU_NULL},
    {ID_COMMUNICATION_CL,       TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},STXT_MAIN_CL,       IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_CL,      0,MENU_NULL},

    // �ڶ���Ĭ��ֵ������ 	    ��Ϣ���� 	�����
#ifdef BLUETOOTH_SUPPORT
	{ID_MAINMENU_BT,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_BLUETOOTH, IMAGE_MAINMENU_ICON_SELECT,  IMAGE_MAINMENU_ICON_BT,0,MENU_NULL},
#endif
    {ID_COMMUNICATION_MESSAGE,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_MESSAGE,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_MESSAGE, 0,MENU_NULL},
#ifdef BROWSER_SUPPORT
    {ID_COMMUNICATION_BROWSER,  TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMUNICATION_BROWSER,  IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_BROWSER, 0,MENU_NULL},
#endif

    // ������Ĭ��ֵ��Wifi/wlan	    DCD         STK
#ifdef MMI_WIFI_SUPPORT
	{ID_CONNECTION_WIFI,            TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN}, TXT_WIFI, IMAGE_MAINMENU_ICON_SELECT,  IMAGE_MAINMENU_ICON_WIFI,0,MENU_NULL},
#endif
#ifdef DCD_SUPPORT
    {ID_TOOLS_DCD,          TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_DCD_QUICK_INFO,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DCD,     0,MENU_NULL},
#endif
    {ID_COMMUNICATION_STK,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMMON_MAIN_STK,    IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_STK,     0,MENU_NULL},
};
@endcode

��������Ǻͼ����˵����ڶ�������ν�ļ����˵����ǵ�����ѡ��ĳһ���˵����ʱ�򣬴�����һ��˵�������Ǽ����˵��ĺ��壬�������ǵ㵽Ϊֹ���ں���ļ����˵�һ���л���ר�ŵĽ��ܵġ����ǣ���ͻ�����ˣ�ǰ���ڽ��ܹ���˵�ʱ���ر�Ҫ�����˵�������Ƽ����˵�������Ϊʲô�ֳ��ּ����˵����أ���ֱ�����Ի�

��ʵ�����˵������ֻ�ǽ�����guimenu(�˵�)�ؼ�����ʾ���̣��Լ���ʼ���˵�����ʱ�����顣�����˵����õ������˵���ͬʱ��ʾ��һ�������еģ�����һ��������ʾtitle���ڶ���������������item���ݡ���ˣ�����ֻ�ǽ����˼����˵������ݽṹ�ඨ�廬���˵��ĳ�ʼ�����ݰ��ˡ�

�������˵���ҳ����Ϣ��ͨ��menu_mainmenu_slide_page���������鶨��ģ���ÿҳ��ѡ����������ͨ������MENU_SLIDE_COMMUNICATION�����Ĳ˵��鶨������ġ�

ͬ������Щ��Ϣֻ���ǿ���ʱ�ĳ�ʼ����Ϣ��֮�����½������˵�������ʾ����Ϣ���Ǵ�NV�ж��������ˣ�����ֻ��item id��group id��������Ϣ��Ȼ��Ҫͨ����̬�����ѯ��

����MENU_SLIDE_COMMUNICATION��MENU_SLIDE_PASTIME�е�ѡ����ܻ�ͨ����Ϲ�����ϵ�һ��ҳ���ڣ���ѯʱ����Ȼ�����ֲ���ͬһ�������С�

@subsection ssec_menu_slide_level_1 һ���˵���Ķ���

MENU_DEF( MENU_MAINMENU_SLIDE_PAGE, menu_mainmenu_slide_page, GUIMENU_STYLE_SLIDE_PAGE, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_mainmenu_slide_page))

�˵���ID���˵������ַ���˵����ͣ����Կ���ΪGUIMENU_STYLE_SLIDE_PAGE�����ĸ��յ��ı���ͼƬ��NULL��ʾ����Ҫ�����˵������������

@subsection ssec_menu_slide_level_2 �����˵��鶨��

@code
MENU_DEF( MENU_SLIDE_COMMUNICATION, menu_slide_communication, GUIMENU_STYLE_THIRD, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_slide_communication))
@endcode

�˵���ID���˵������ַ���˵����ͣ����Կ���ΪGUIMENU_STYLE_THIRD������������ڻ������˵������޹ؽ�Ҫ�ģ���Ϊ��ʾ��ʱ���ò����������ĸ��յ��ı���ͼƬ��NULL��ʾ����Ҫ�����˵������������

@subsection ssec_menu_slide_item �˵����

@code
{ID_COMMUNICATION_DIAL,     TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_COMM_DIALPAD,      IMAGE_MAINMENU_ICON_SELECT,IMAGE_MAINMENU_ICON_DIAL,    0,MENU_NULL},
@endcode

������Item id��softkey ��Ϣ����ʾ�ı�������ʱ����ͼƬ��itemͼƬ����Ч��Ϣ��

@subsection ssec_menu_slide_layout �����˵���Layout���

��ͼ 2 14��ʾ�������˵���ʾ�빬�����ƣ�����ͬ���Ƕ���һ����ʾҳ�������������page_rect ��80, 274, 160, 285����ʾ����Ȼ��ҳ���ʶֻ��ʾ1�У�����м��Ϊ0��ֻ��Ҫ�����м��Ϳ��Լ�����

�����˵���Layout��Ϣ�������� GUIMENU_SLIDEMENU_DATA_T �ṹ���С�

@note
-# MMITHEME_MENUMAIN_SLIDE_PAGE_NUM ����ҳ����������
-# MMITHEME_MENUMAIN_ICON_ITEM_MAX_NUM = page_row_num�� page_col_num������һ��Ҫע��ģ����򽫻������ʾ����
-# item_width��item_height��ͨ��MENUICON_WIDTH��MENUICON_HEIGHT��õģ���������Ƚ����⣬�����빤�̵�make file��������ڹ��̵�make file�ж�������SPECIAL_EFFECT_ICON_SIZE = 48X48 �ĺ궨�壬48X48˵��ͼƬ�Ŀ�Ⱥ͸߶�Ϊ48��֮���������涨������3DЧ�������ƣ���Ϊ����3DЧ��ʱ���ײ��ͨ��make file��ϵͳ��ʼ��ʱ��ȡ�����ֵ�����ڴ���䡣�мǣ�

@subsection ssec_menu_slide_modify ����޸Ļ����˵�

-# ͨ���޸Ļ����˵���ҳ�Ķ������飬���ﵽ���Ӻͼ��ٳ�ʼ��ҳ���Ŀ�ġ�
-# ͨ���޸�ҳ����¼��˵����飬���ﵽ���Ӻͼ���ҳ������ʾ��Լ���ʾ���ݡ�
-# ͨ���޸�layout������ʾ��λ�á�

@section sec_menu_list �б�˵�

�����б�˵���ʹ�������ڵ�1ƪ6.4һ�����Ѿ����ܵıȽ���ϸ�ˣ���ο�����½ڡ�

@section sec_menu_popup ����ʽ�˵�

����ʽ�˵���PopUp Menu����ͼ 2 15��ʾ������ʽ�˵��Ĵ���û��Title���������ⱳ�����������ʾ��͸����Ч�������Ĳ˵����������һ���˵�������˵���ĳһ������һ���˵������Լ�ͷ��ʾ����һ��������������ʾ�˵�������������û������������ã���������������ʱ�򣬽�����ʾ����������ͼ����
@image html menu_popup.jpg
ͼ 2 15 ����ʽ�˵�

@subsection ssec_menu_popup_item �˵�������

-# ����POP MENU��һ���˵�
@code
const GUIMENU_ITEM_T menu_test_popupmenu_table[] =
{
    {ID_POPUP_MENU_1,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_2,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_3,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,MENU_TEST_SUBMENU_TABLE},
};
@endcode
-# ����POPMENU�ļ����˵�
@code
const GUIMENU_ITEM_T menu_test_submenu_table[] =
{
    {ID_POPUP_MENU_SUB1,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_SUB2,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_SUB3,      TIP_NULL,{STXT_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,0},
};
@endcode

@subsection ssec_menu_popup_item_def �˵��鶨��
-# һ���˵���궨��
@code
MENU_DEF( MENU_TEST_POPUPMENU_TABLE, menu_test_popupmenu_table, GUIMENU_STYLE_POPUP, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_test_popupmenu_table))
@endcode
-# �����˵���궨��
@code
MENU_DEF( MENU_TEST_SUBMENU_TABLE, menu_test_submenu_table, GUIMENU_STYLE_POPUP, TXT_NULL, IMAGE_NULL, IMAGE_NULL, IMAGE_NULL, GUIMENU_ITEM_NUM(menu_test_popupmenu_table))
@endcode
    -# �˵���ID
    -# �˵������ַ
    -# �˵����ͣ����Կ���ΪGUIMENU_STYLE_POPUP��
    -# �ĸ��յ��ı���ͼƬ��NULL��ʾ����Ҫ��
    -# �˵����������

������menu_test_popupmenu_table�Ķ�����Կ����������˵��ĵ��������¼��˵���

@subsection ssec_menu_popup_create �˵�����

����ʽ�˵��������˵���ͬ���ڴ�����ʱ�򣬵���ʽ�˵�����Ҫʹ��CREATE_MENU_CTRL�꣬Ҳ����Ҫ�����ڴ���Table�������Ƕ�̬�������Ǿ�̬��������ͨ���ӿ���ʵ�ֵġ�
-# MMIPOPMENU_CreateStatic ��̬��������ʽ�˵�
-# MMIPOPMENU_CreateDynamic ��̬��������ʽ�˵�

@subsection ssec_menu_popup_modify �޸Ĳ˵����

�����Բ˵��ķ�����£�
-# �˵���ʾ�������������ֵʹ��һ���궨��ĳ�������������MMIMENU_POPUP_LINE_NUM_PAGE��������mmimenu_position.h�ļ��У�Ŀǰ���Ƕ���240x320�Լ�176x220����Ļ������ʽ�˵��������ʾ����Ϊ8����240x400�������ʾ��Ϊ10��
-# �˵��ı����޸ģ�����ʽ�˵��ı�����Ϊ�����������֣��ϰ벿��Ϊ��͸��Ч����ʹ��ͼƬIMAGE_PUBWIN_UP_BG������һ�Ź̶���ԴͼƬ��ͼƬ�Ĵ�С���������޸ġ����ڵ��°벿��Ϊ����ɫ����ɫֵΪMMITHEME_POPUPMENU_BG_COLOR���ڴ����˵��ؼ���ʱ�򣬵���MMITHEME_GetPopupMenuTheme��mmitheme_menu.cʵ�֣���ȡ��
-# �˵������������Ϣ��������MMITHEME_GetPopupMenuTheme�ӿ����޸ġ�

@section sec_menu_link �����˵�

����˼�壬�����˵�����һ���˵������һ���˵�����̬����˵�������Դ��ʱ�����ʵ�����ü����˵��Ĳ˵���ID����ִ�в˵����ʱ�򣬿ؼ������Ƿ��м����˵����Զ���ʾ�����Ͻڽ��ܵ���ʽ�˵�ʱ�ļ����˵����á�

@section sec_menu_function_list �ؼ��ӿڽ���

-# @ref GUIMENU_GetId
-# @ref GUIMENU_InsertNode
-# MMIAPICOM_EnableGrayed

*******************************************************************************/

/*! @addtogroup menuGrp Menu
@brief menu�ؼ�
@details
�˵��ؼ���Ϊ�����ࣺ
- ��̬�˵�\n
  ��̬�˵�����������ͨ���˵������ʽ�Ѿ�ȷ�����˵��ؼ��������Ų˵���������˵��
  �û����ܹ��ٶ�̬��Ӻ�ɾ���˵��\n
  ����\link #GUIMENU_STYLE_ICON \endlink��\link #GUIMENU_STYLE_OPTION_PAGE \endlink��
  \link #GUIMENU_STYLE_TITLE_ICON \endlink��\link #GUIMENU_STYLE_SLIDE_PAGE \endlink��
  \link #GUIMENU_STYLE_CUBE \endlink�����Ǿ�̬�˵���
- ��̬�˵�\n
  ��̬�˵����������û�ͨ���ؼ��ṩ�ķ���������ġ�\n
  ������Ҫ����ʽ�Ĳ˵��ö�̬�˵���
@{
*******************************************************************************/


#ifndef _GUIMENU_H_
#define _GUIMENU_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guictrl.h"
#include "guiprgbox.h"
#include "mmitheme_menu.h"
#include "mmi_link.h"
#include "ui_layer.h"
#include "guiimg.h"
#include "ctrlmenu_export.h"
#include "ctrlmainmenu_export.h"

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
/*! @def GUIMENU_ITEM_NUM
@brief ����˵���ĸ���
*/
#define GUIMENU_ITEM_NUM(_ITEMPTR)     (sizeof(_ITEMPTR)/sizeof(GUIMENU_ITEM_T))

#define GUIMENU_SLIDE_PAGE_NUM           2

/*! @def GUIMENU_DYMAINMENU_STR_MAX_NUM
@brief ��̬���˵��ַ����������ֵ
*/
#define GUIMENU_DYMAINMENU_STR_MAX_NUM     20

#define GUIMENU_BUTTON_NUM              2

#define GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM 8
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @typedef GUIMENU_SOFTKEY_STATE_T
@brief
*/
typedef uint32 GUIMENU_SOFTKEY_STATE_T;
/*! @def GUIMENU_LEFTSK_NEED
@brief
*/
#define GUIMENU_LEFTSK_NEED     0x00000001
/*! @def GUIMENU_MIDSK_NEED
@brief
*/
#define GUIMENU_MIDSK_NEED      0x00000002
/*! @def GUIMENU_RIGHTSK_NEED
@brief
*/
#define GUIMENU_RIGHTSK_NEED    0x00000004

/*! @typedef GUIMENU_MOVE_STATE_T
@brief ֪ͨ�û���δ���˵���move��������ֵ��ΪMSG_NOTIFY_MENU_SET_STATUS��Ϣ�ĸ�����Ϣ���д���
*/
typedef uint32 GUIMENU_MOVE_STATE_T; // ������ �˵�moveʱ�Ĵ���
/*! @def GUIMENU_STATE_MOVE_NONE
@brief ������
*/
#define GUIMENU_STATE_MOVE_NONE    0x00000000
/*! @def GUIMENU_STATE_MOVE_SET
@brief ֪ͨ�û���Ҫ���˵����ó�move״̬
*/
#define GUIMENU_STATE_MOVE_SET     0x00000001 // ������֯����
/*! @def GUIMENU_STATE_MOVE_CANCEL
@brief ֪ͨ�û�ȡ���˵���move״̬
*/
#define GUIMENU_STATE_MOVE_CANCEL  0x00000002 // cancel������
/*! @def GUIMENU_STATE_MOVE_OK
@brief ֪ͨ�û��Ѿ���ɲ˵���move����
*/
#define GUIMENU_STATE_MOVE_OK      0x00000003 // ok������

// ����˵�״̬��Ϊ�Ժ���չʹ��
/*! @typedef GUIMENU_STATE_T
@brief �˵���״̬��Ϣ
*/
typedef uint32 GUIMENU_STATE_T;
/*! @def GUIMENU_STATE_ENTER
@brief ����ø�λ����ʾmenu���ڳ�ʼ״̬��һ��ˢ�º󣬸�״̬����ΪFALSE
*/
#define GUIMENU_STATE_ENTER          0x00000001
/*! @def GUIMENU_STATE_CUSTOMER_TITLE
@brief ����ø�λ����ʾ�˵���ȥ������⣬���û��Լ�����
*/
#define GUIMENU_STATE_CUSTOMER_TITLE 0x00000002 // �û��Զ���title

/*! @typedef LINK_PARAMETER_T
@brief �˵�����ָ�����
*/
typedef void *DY_MENU_LINK_PARAMETER_T;
/*! @typedef NEW_ITEM_LINK_FUNC_T
@brief ��̬�˵�����ָ��
*/
typedef void (*DY_MENU_ITEM_LINK_FUNC_T)(DY_MENU_LINK_PARAMETER_T param1, DY_MENU_LINK_PARAMETER_T param2);

/*! @typedef NEW_ITEM_LINK_DELETE_FUNC_T
@brief ��̬�˵�����ָ��
*/
typedef BOOLEAN (*DY_MENU_ITEM_LINK_DELETE_FUNC_T)(DY_MENU_LINK_PARAMETER_T param1, DY_MENU_LINK_PARAMETER_T param2);

// �������menuʱ�Ķ�������
typedef enum
{
    GUIMENU_ENTER_ANIM_NONE,        // �޶��� */
    GUIMENU_ENTER_ANIM_SLIDE_ICON,  // �����˵�ʱ��itemͼ��Ӱ˸��������м们�� */
    // ��չ
    GUIMENU_ENTER_ANIM_MAX
}GUIMENU_ENTER_ANIM_TYPE_E;

/*! @typedef GUIMENU_FILE_DEVICE_E
@brief ���ݴ洢·������
*/
typedef enum
{
    GUIMENU_DEVICE_SYSTEM,    /*!< C�̣�����ʾ���û���ֻ��Ӧ�ñ��������ļ� */
    GUIMENU_DEVICE_UDISK,     /*!< U�� */
    GUIMENU_DEVICE_SDCARD_1,  /*!< ��1 */ 
    GUIMENU_DEVICE_SDCARD_2,  /*!< ��2 */ 
    GUIMENU_DEVICE_MAX
} GUIMENU_FILE_DEVICE_E;

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
typedef struct  
{
    uint32* icon_layer_buf;//�����������һ����BUF��������layer_info
    uint16 width;
    uint16 height;
}GO_GUIMENU_ITEM_BUF_T;

#endif

typedef enum
{
    GO_SLIDE_SUPPORT_NONE = 0,// ��֧�ֻ���     
    GO_SLIDE_SUPPORT_ONLY_X,//֧��X���򻬶�
    GO_SLIDE_SUPPORT_ONLY_Y,//֧��Y���򻬶�
    GO_SLIDE_SUPPORT_XY,   //��������֧�� 
    GO_SLIDE_SUPPORT_MAX
}GUIMENU_GO_SLIDE_TYPE_E;

//brief ������Ϣ���ݽṹ
//note �ýṹ���ڻ���˵������Ϣ��Ϊ�ӿ�����ٶ�
typedef struct
{
    GUI_LCD_DEV_INFO icon_layer;                    // ����Ϣ */
    uint16 width;                                   // ��� */
    uint16 height;                                  // �߶� */
    BOOLEAN is_draw;
}GUIMENU_ITEM_BUF_T;

typedef struct 
{
    MMIMENU_LANGUAGE_TYPE_E  language_type;                     /*!< ��������   */
    wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1];             /*!< �ַ�����Ϣ */
}GUIMAINMENU_DYNAMIC_STRING_DATA_T;

/*! @struct  GUIMAINMENU_DYNAMIC_DATA_T
@brief ��̬�˵�����
*/
typedef struct
{
    wchar text[GUIMENU_DYMAINMENU_STR_MAX_NUM + 1];             /*!< �ַ�����Ϣ */
    uint16 text_len;                                            /*!< �ַ������� */
    GUIMAINMENU_DYNAMIC_STRING_DATA_T  str_lang_info[GUIMENU_DYMAINMENU_STR_LANG_MAX_NUM];   /*!< �ַ���������Ϣ */  
    BOOLEAN has_icon;                                           /*!< �Ƿ���ͼƬ */
    uint8 *icon_ptr;                                            /*!< ͼƬ��Ϣ */
    uint16 icon_height;                                         /*!< ͼƬʵ�ʸ߶� */
    uint16 icon_width;                                          /*!< ͼƬʵ�ʿ�� */
    DY_MENU_LINK_PARAMETER_T param1ptr;                         /*!< link_function ����1 ,Ҳ��Ϊɾ��ʱ��ʶλ*/
    DY_MENU_LINK_PARAMETER_T param2ptr;                         /*!< link_function ����2 ,Ҳ��Ϊɾ��ʱ��ʶλ*/
    uint32 parm1_size;                                          /*!< link_function ����1�Ĵ�С */
    uint32 parm2_size;                                          /*!< link_function ����2�Ĵ�С */
    DY_MENU_ITEM_LINK_FUNC_T link_function_ptr;                 /*!< ��Ӧ�ó�����ָ�� */
    DY_MENU_ITEM_LINK_DELETE_FUNC_T delete_function_ptr;        /*!< ɾ��Ӧ�ó�����ָ�� */ 
    MMI_IMAGE_ID_T select_icon_id;                              /*!< ����ͼƬid */
    uint32 icon_datasize;                                       /*!< ͼƬ���ݴ�С */
    
} GUIMAINMENU_DYNAMIC_DATA_T;

/*! @struct  GUIMAINMENU_DYNAMIC_INFO_T
@brief ��̬�˵���Ϣ
*/
typedef struct
{
    uint16 page_index;                                      /*!< ��̬�˵�λ����Ϣ,ҳ���� */
    uint16 item_index;                                      /*!< ��̬�˵�λ����Ϣ����� */
    MMI_MENU_ID_T menu_id;                                  /*!< ��̬�˵�menu_id,����"0",��ϵͳ��̬��ֵ */
    GUIMAINMENU_DYNAMIC_DATA_T dynamic_menu_info;     /*!< ��̬�˵�������Ϣ */
} GUIMAINMENU_DYNAMIC_INFO_T;

/*! @struct  GUIMAINMENU_DY_ITEM_T
@brief ��̬�˵���ϸ��Ϣ
*/
typedef struct
{
    GUIMAINMENU_DYNAMIC_INFO_T *ori_data_ptr;          /*!< Դ���� */
    GUIMENU_FILE_DEVICE_E store_dev;                        /*!< �洢λ�� */
} GUIMAINMENU_DY_ITEM_T;

typedef enum
{
    GUIMENU_ITEM_IN_PAGE,  //in page
    GUIMENU_ITEM_IN_BAR,   //bar
    GUIMENU_ITEM_IN_FOLDER //in folder
}GUIMENU_ITEM_POSITION_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUIMENU_GetId                   CTRLMENU_GetId
#define GUIMENU_SetPopMenuItemHeight    CTRLMENU_SetPopItemHeight
#define GUIMENU_SetFirstAndSelectedItem CTRLMENU_SetFirstAndSelItem
#define GUIMENU_SetMenuTitle            CTRLMENU_SetTitle
#define GUIMENU_SetMenuTitleEx          CTRLMENU_SetTitleEx
#define GUIMENU_CreatDynamic            CTRLMENU_CreatDynamic
#define GUIMENU_CreatDynamicEx          CTRLMENU_CreatDynamicEx
#define GUIMENU_InsertNode              CTRLMENU_InsertNode
#define GUIMENU_ReplaceNode             CTRLMENU_ReplaceNode
#if defined (MRAPP_SUPPORT)||defined (HERO_ENGINE_SUPPORT ) 
#define GUIMENU_ReplaceNodeEx           CTRLMENU_ReplaceNodeEx
#endif
#define GUIMENU_SetNodeGrayed           CTRLMENU_SetNodeGrayed
#define GUIMENU_SetNodeGrayedByNodeId   CTRLMENU_SetNodeGrayedByNodeId
#define GUIMENU_SetNodeVisible          CTRLMENU_SetNodeVisible
#define GUIMENU_SetDynamicMenuSoftkey   CTRLMENU_SetDynamicMenuSoftkey
#define GUIMENU_SetMenuBgColor          CTRLMENU_SetBgColor
#define GUIMENU_SetMenuFontColor        CTRLMENU_SetFontColor
#define GUIMENU_GetCheckInfo            CTRLMENU_GetCheckInfo
#define GUIMENU_SetRect                 CTRLMENU_SetRect
#define GUIMENU_GetCurNodeId            CTRLMENU_GetCurNodeId
#define GUIMENU_SetItemGrayed           CTRLMENU_SetItemGrayed
#define GUIMENU_SetItemVisible          CTRLMENU_SetItemVisible
#define GUIMENU_SetStaticItem           CTRLMENU_SetStaticItem
#define GUIMENU_SetMaxWidth             CTRLMENU_SetMaxWidth
#define GUIMENU_SetEffectivePoint       CTRLMENU_SetEffectivePoint
#define GUIMENU_GetState                CTRLMENU_GetState
#define GUIMENU_SetState                CTRLMENU_SetState
#define GUIMENU_SetSelectedItem         CTRLMENU_SetSelectedItem
#define GUIMENU_SetButtonStyle          CTRLMENU_SetButtonStyle
#define GUIMENU_SetPopItemStatus        CTRLMENU_SetPopItemStatus
#define GUIMENU_SetSubMenuStyle         CTRLMENU_SetSubMenuStyle
#define GUIMENU_IsPopMenuEx             CTRLMENU_IsPopMenuEx
#define GUIMENU_GetNodeStr              CTRLMENU_GetNodeStr
#define GUIMENU_GetFontColor            CTRLMENU_GetFontColor
#define GUIMENU_IsFirstDisp             CTRLMENU_IsFirstDisp
#define GUIMENU_GetLevel                CTRLMENU_GetLevel
#define GUIMENU_SetFirstDisp            CTRLMENU_SetFirstDisp
#define GUIMENU_GetRect                 CTRLMENU_GetRect
#ifdef _SW_ZDT_PRODUCT_
#define GUIMENU_SetMenuBgImage          CTRLMENU_SetBgImage
#endif

#define GUIMAINMENU_GetPM               CTRLMAINMENU_GetPM
#define GUIMAINMENU_GetId               CTRLMAINMENU_GetId
#define GUIMAINMENU_InitMoveState       CTRLMAINMENU_InitMoveState
#define GUIMAINMENU_IsMoveState         CTRLMAINMENU_IsMoveState
#define GUIMAINMENU_CancelMoveIcon      CTRLMAINMENU_CancelMoveIcon
#define GUIMAINMENU_GetStyle            CTRLMAINMENU_GetStyle

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/


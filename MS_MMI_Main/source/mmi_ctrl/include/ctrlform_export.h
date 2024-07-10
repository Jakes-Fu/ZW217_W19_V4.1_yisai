/*! @file ctrlform_export.h
@brief ���ļ�������form�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Create
*******************************************************************************/

/*! @page page_Form ����(Form)

-# @ref sec_form_function
-# @ref sec_form_def
-# @ref sec_form_def_child
    -# @ref ssec_form_def_child_text
    -# @ref ssec_form_def_child_edit_text
    -# @ref ssec_form_def_child_edit_phonenumber
    -# @ref ssec_form_def_child_edit_digital
    -# @ref ssec_form_def_child_edit_passwd
    -# @ref ssec_form_def_child_edit_list
    -# @ref ssec_form_def_child_edit_data
    -# @ref ssec_form_def_child_edit_time
    -# @ref ssec_form_def_child_edit_ip
    -# @ref ssec_form_def_child_anim
    -# @ref ssec_form_def_child_label
    -# @ref ssec_form_def_child_button
    -# @ref ssec_form_def_child_downdroplist
    -# @ref ssec_form_def_child_setlist
    -# @ref ssec_form_def_child_owndraw
    -# @ref ssec_form_def_child_form
-# @ref sec_form_layout
-# @ref sec_form_attribute
-# @ref sec_form_child_attribute
-# @ref sec_form_function_list
-# @ref sec_form_app
-# @ref sec_form_usage_add_child

@section sec_form_function �ؼ�����

Form�ؼ��������ؼ���������Ƕ��ƽ̨֧�ֵ����пؼ������������ؼ�����Ŀǰֻ֧��Text��Edit��Anim��Label��Button��Dropdown list��Setting list��Owner draw��Form�ؼ���
-# ��Ƕ�ӿؼ��Զ��Ű棬����Ӧ��ͬ��С��LCD����ʾ���£��Զ���ӹ�������
-# �������Զ��л�������Ӧ���޸ģ�
-# �ӿؼ����Իһ����������أ�
-# �ӿؼ����Ի��Active��������Active�ؼ���ʾ��Ӧ��Title��Softkey��
-# �ӿؼ�����˳����߲�����ʾ��

@section sec_form_def �궨�����

@code
#define CREATE_FORM_CTRL(_LAYOUT_, _CTRL_ID_) \
        MMK_CREATE_FORM, _LAYOUT_, _CTRL_ID_
@endcode
- ���ڶ���Form�ؼ���Ĭ����ʾ�����ȥTitle��Softkey��
    - _LAYOUT_ �ӿؼ��Ű淽ʽ��˳����߲��š�
    - _CTRL_ID_ �ؼ�ID

@section sec_form_def_child �ӿؼ��궨�����

@subsection ssec_form_def_child_text CHILD_TEXT_CTRL

@code
#define CHILD_TEXT_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_TEXT, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Text�ӿؼ�
    - _IS_GET_ACTIVE_ �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_text CHILD_EDIT_TEXT_CTRL

@code
#define CHILD_EDIT_TEXT_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_EDIT_TEXT, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit ��ͨ�ı��༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _STR_MAX_LEN_ �������������ַ�����
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_phonenumber CHILD_EDIT_PHONENUM_CTRL

@code
#define CHILD_EDIT_PHONENUM_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PHONENUM, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit�绰����༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _STR_MAX_LEN_ �������������ַ�����
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_digital CHILD_EDIT_DIGITAL_CTRL

@code
#define CHILD_EDIT_DIGITAL_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DIGITAL, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit���ֱ༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _STR_MAX_LEN_ �������������ַ�����
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_passwd CHILD_EDIT_PASSWORD_CTRL

@code
#define CHILD_EDIT_PASSWORD_CTRL(_IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_PASSWORD, _IS_GET_ACTIVE_, _STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit����༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _STR_MAX_LEN_ �������������ַ�����
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_list CHILD_EDIT_LIST_CTRL

@code
#define CHILD_EDIT_LIST_CTRL(_IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_LIST, _IS_GET_ACTIVE_, _ITEM_NUM_, _ITEM_STR_MAX_LEN_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit�б�༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _ITEM_NUM_    �б�����
    - _ITEM_STR_MAX_LEN_    ÿ���������������ַ�����
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_data CHILD_EDIT_DATE_CTRL

@code
#define CHILD_EDIT_DATE_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_DATE, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit���ڱ༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_time CHILD_EDIT_TIME_CTRL

@code
#define CHILD_EDIT_TIME_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_TIME, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Editʱ��༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_edit_ip CHILD_EDIT_IP_CTRL

@code
#define CHILD_EDIT_IP_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
         CREATE_CHILD_EDIT_IP, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Edit IP��ַ�༭�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_anim CHILD_ANIM_CTRL

@code
#define CHILD_ANIM_CTRL(_IS_BG,_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_ANIM, _IS_BG, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Anim�ӿؼ�
    - _IS_BG    �Ƿ���Form�����������ܹ����Active
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_label CHILD_LABEL_CTRL

@code
#define CHILD_LABEL_CTRL(_ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_LABEL, _ALIGN_, _IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Label�ӿؼ�
    - _ALIGN_   �ַ��Ű淽ʽ
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_button CHILD_BUTTON_CTRL

@code
#define CHILD_BUTTON_CTRL(_IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_BUTTON, _IS_GET_ACTIVE_, _BG_IMAGE_ID, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- Button�ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _BG_IMAGE_ID  ����ͼ����release״̬�µı���ͼƬ
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_downdroplist CHILD_DROPDOWNLIST_CTRL

@code
#define CHILD_DROPDOWNLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_DROPDOWNLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_
@endcode
- �����˵��ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_setlist CHILD_SETLIST_CTRL

@code
#define CHILD_SETLIST_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_) \
        CREATE_CHILD_SETLIST, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_
@endcode
- �����б��ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@subsection ssec_form_def_child_owndraw CHILD_OWNDRAW_CTRL

@code
#define CHILD_OWNDRAW_CTRL(_IS_GET_ACTIVE_, _CTRL_ID_, _PARENT_CTRL_ID_, _CALL_BACK_) \
        CREATE_CHILD_OWNDRAW, _IS_GET_ACTIVE_, _CTRL_ID_,_PARENT_CTRL_ID_, _CALL_BACK_
@endcode
- �Ի��ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID
    - _CALL_BACK_   �Ի�ص�����

@subsection ssec_form_def_child_form CHILD_FORM_CTRL

@code
#define CHILD_FORM_CTRL(_IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_) \
        CREATE_CHILD_FORM, _IS_GET_ACTIVE_, _LAYOUT_, _CTRL_ID_, _PARENT_CTRL_ID_
@endcode
- �����ӿؼ�
    - _IS_GET_ACTIVE_   �Ƿ��ܹ����Active
    - _LAYOUT_  �ӿؼ����з�ʽ
    - _CTRL_ID_ �ؼ�ID
    - _PARENT_CTRL_ID_  ���ؼ�ID

@section sec_form_layout �ӿؼ����з�ʽ

-# GUIFORM_LAYOUT_ORDER
    -# ˳������
    -# Ĭ�Ͽ��ΪFormȥ���߿�ҳ�߾࣬���������
    -# ����ͨ��CTRLFORM_SetChildWidth�ӿں������ÿ��
-# GUIFORM_LAYOUT_SBS
    -# ��������
    -# Ĭ��ÿ���ӿؼ���Ⱦ���
    -# ����ͨ��CTRLFORM_SetChildWidth�ӿں������ÿ��
    -# ��Ĭ�Ͽ�ȣ�����n���ӿؼ�����������n-1���ӿؼ����

@section sec_form_attribute FORM����

-# �������߿�ҳ�߾ࣻ
-# �ӿؼ�ˮƽ����ֱ���
-# ͼƬ����Ŀ�߱�����Ĭ��4��3��
-# ����һ��ռ����Form�ı���4��5��
-# �ӿؼ��������߿����壬�����һ���Active����

@section sec_form_child_attribute �ӿؼ�����
-# �ӿؼ�ˮƽ����ֱ���
-# �ӿؼ��������߿�����
-# �ӿؼ����

@section sec_form_function_list �ؼ��ӿڽ���

-# CTRLFORM_SetRect
-# CTRLFORM_SetVisible
-# CTRLFORM_SetBg
-# CTRLFORM_SetSaveBg
-# CTRLFORM_SetMargin
-# CTRLFORM_SetSpace
-# CTRLFORM_SetChildWidth
-# CTRLFORM_SetAlign
-# CTRLFORM_GetChildDisplay
-# CTRLFORM_SetChildDisplay
-# CTRLFORM_PermitChildBg
@note�ú�����Ҫ����ĳЩ�ؼ�Ҫʹ����������ı�����������Formͨ�õı��������£�
@image html form_bg_example.jpg
ͼ 2 47Form�ӿؼ����������ñ���
-# CTRLFORM_CreatDynaCtrl
-# CTRLFORM_CreatDynaChildCtrl

@section sec_form_app �ؼ�Ӧ��

Form�ؼ���ҪӦ���ڶ���ؼ���ϵĽ��棬�������ӱ༭����
@image html form_app_example.jpg 
ͼ 2 48Form�ؼ���ʵ��Ӧ��
@code
WINDOW_TABLE( ALARM_EDITWIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleAlarmEditWinMsg), 
    WIN_ID(MMIALM_EDIT_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_NAME_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMIALM_NAME_MAX_LEN,MMIALM_EDIT_NAME_EDITBOX_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_TITLE_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_EDIT_TIME_CTRL(TRUE,MMIALM_EDIT_TITLE_EDITBOX_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_RING_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,TRUE,MMIALM_EDIT_RING_TEXT_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIALM_EDIT_MODE_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_SETLIST_CTRL(TRUE,MMIALM_EDIT_MODE_SETLIST_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),

    IALM_EDIT_SELECT_WEEK_LABEL_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMI_EDITWIN_CHILD_FORM_CTRL_ID,MMI_EDITWIN_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON0_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON1_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON2_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON3_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON4_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON5_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_ALARM_WEEK_UNSEL,MMI_EDITWIN_BUTTON6_CTRL_ID,MMI_EDITWIN_CHILD_FORM_CTRL_ID),  
                
    END_WIN
};
@endcode

@section sec_form_usage_add_child ��������Ƕ�ӿؼ�

������һ���ؼ���ΪForm���ӿؼ��أ�������setting listΪ����
-# window_parse.h �����ӿؼ���
-# CTRLFORM_parse.c �����ӿؼ�
-# guisetlist.c ʵ��GUICTRL_SetRect����������
-# guisetlist.c ʵ��GUICTRL_SetBorder�����ñ߿�
-# guisetlist.c ʵ��GUICTRL_SetBg�����ñ���
-# guisetlist.c ʵ��GUICTRL_GetFont���������
-# guisetlist.c ʵ��GUICTRL_SetFont����������
-# guisetlist.c ʵ��GUICTRL_SetProgress�����������Ƿ���ʾ���������޹�����������Ĭ�Ϻ�������
-# guisetlist.c ʵ��GUICTRL_SetCircularHandle�����ò�ѭ���������°������ޣ�����Ĭ�Ϻ�������
-# guisetlist.c ʵ��GUICTRL_GetHeightByWidth�����ݿ�Ȼ�ÿؼ��ĸ߶�
-# guisetlist.c ʵ��GUICTRL_GetDisplayRect�������ʾ����
-# guisetlist.c ʵ��GUICTRL_SetDisplayRect��������ʾ����
-# guisetlist.c ʵ��GUICTRL_GetSelectRect�����ѡ��������cursor,list���ޣ�����Ĭ�Ϻ�������
-# guisetlist.c ʵ��GUICTRL_GetSelectIndex���ѡ�������Ƿ��ǵ�һ���������һ�����ޣ�����Ĭ�Ϻ�������

�ؼ��ڲ��޸ģ�
-# ������ʾ��guicommon.c����һ�����ú���GUI_DisplayBg()
-# �߿���ʾ��guicommon.c����һ�����ú���GUI_DisplayBorder
-# MSG_CTL_GET_ACTIVE��MSG_CTL_LOSE_ACTIVE��Ҫͬ��֪ͨ���丸�ؼ�
-# ����display_rect����������ʾ�ؼ���ĳ������
*******************************************************************************/

/*! @addtogroup formGrp Form
@brief form���ڿؼ�
@details 
    - �����ؼ�
    - ֧���Զ��Ű�
@{
*******************************************************************************/


#ifndef _CTRLFORM_EXPORT_H_
#define _CTRLFORM_EXPORT_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "guiprgbox.h"
#include "mmitheme_form.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                      *
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
--------------------------------------------------------------------------*/
/*! @enum GUIFORM_LAYOUT_E
@brief �ӿؼ����з�ʽ��ʽ
*/
typedef enum
{
    GUIFORM_LAYOUT_ERROR = -1,  //bug1941758
    GUIFORM_LAYOUT_ORDER,   /*!<Ĭ��,�ؼ���������˳������*/
    GUIFORM_LAYOUT_SBS,     /*!<�ӿؼ���������*/
    GUIFORM_LAYOUT_MAX      /*!<����λ*/
} GUIFORM_LAYOUT_E;

/*! @enum GUIFORM_TYPE_E
@brief form����
*/
typedef enum
{
    GUIFORM_TYPE_COMMON,    /*!<��ͨ����*/
    GUIFORM_TYPE_TP,        /*!<����������,��List*/
    GUIFORM_TYPE_MAX        /*!<����λ*/
} GUIFORM_TYPE_E;

/*! @enum GUIFORM_STYLE_E
@brief form���
*/
typedef enum
{
    GUIFORM_STYLE_NORMAL,   /*!<Ĭ��,����*/
    GUIFORM_STYLE_UNIT,     /*!<form��Ϊһ������,��ʾ�����ͱ߿�*/
    GUIFORM_STYLE_MAX       /*!<����λ*/
} GUIFORM_STYLE_E;

/*! @enum GUIFORM_CHILD_WIDTH_E
@brief �ӿؼ��������
*/
typedef enum
{
    GUIFORM_CHILD_WIDTH_AUTO,       /*!<ƽ������*/
    GUIFORM_CHILD_WIDTH_FIXED,      /*!<�̶����*/
    GUIFORM_CHILD_WIDTH_PERCENT,    /*!<���հٷֱȼ���*/
    GUIFORM_CHILD_WIDTH_MAX         /*!<����λ*/
} GUIFORM_CHILD_WIDTH_E;

/*! @enum GUIFORM_CHILD_HEIGHT_E
@brief �ӿؼ��߶�����
*/
typedef enum
{
    GUIFORM_CHILD_HEIGHT_AUTO,      /*!<���ձ������ݿ�ȼ���*/
    GUIFORM_CHILD_HEIGHT_FIXED,     /*!<�̶��߶�*/
    GUIFORM_CHILD_HEIGHT_PERCENT,   /*!<�ӿؼ��߶Ȱ��հٷֱȼ���*/
    GUIFORM_CHILD_HEIGHT_LEFT,      /*!<formʣ�µĸ߶�,���ֲ���*/
    GUIFORM_CHILD_HEIGHT_LEFT_MIN,  /*!<����Formʣ�¸߶�,��Ϊ����߶�;С��Formʣ�¸߶�,��ΪFormʣ�¸߶�*/
    GUIFORM_CHILD_HEIGHT_MAX        /*!<����λ*/
} GUIFORM_CHILD_HEIGHT_E;

/*! @enum GUIFORM_CHILD_ALIGN_E
@brief �����ӿؼ����з�ʽ
*/
typedef enum
{
    GUIFORM_CHILD_ALIGN_ERROR = -1,  //bug1941758
    GUIFORM_CHILD_ALIGN_NONE,   /*!<Ĭ��,����*/
    GUIFORM_CHILD_ALIGN_LEFT,   /*!<����*/
    GUIFORM_CHILD_ALIGN_HMIDDLE,/*!<ˮƽ����*/
    GUIFORM_CHILD_ALIGN_RIGHT,  /*!<����*/
    GUIFORM_CHILD_ALIGN_MAX     /*!<����λ*/
} GUIFORM_CHILD_ALIGN_E;

/*! @enum GUIFORM_CHILD_DISPLAY_E
@brief �ӿؼ���ʾ��ʽ
*/
typedef enum
{
    GUIFORM_CHILD_DISP_NORMAL,  /*!<������ʾ*/
    GUIFORM_CHILD_DISP_GRAY,    /*!<�һ���ʾ*/
    GUIFORM_CHILD_DISP_HIDE,    /*!<����*/
    GUIFORM_CHILD_DISP_MAX      /*!<����λ*/
} GUIFORM_CHILD_DISPLAY_E;

/*! @enum GUIFORM_CHILD_DOCK_E
@brief �ӿؼ��п���ʽ
*/
typedef enum
{
    GUIFORM_CHILD_DOCK_NONE,    /*!<ϵͳĬ��*/
    GUIFORM_CHILD_DOCK_TOP,     /*!<�ö�*/
    GUIFORM_CHILD_DOCK_BOTTOM,  /*!<�õ�*/
    GUIFORM_CHILD_DOCK_LEFT,    /*!<����*/
    GUIFORM_CHILD_DOCK_RIGHT,   /*!<����*/
    GUIFORM_CHILD_DOCK_MAX,
} GUIFORM_CHILD_DOCK_E;

/*! @struct CTRLFORM_INIT_DATA_T
@brief form�ؼ���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T         both_rect;  /*!<��������*/
    GUIFORM_LAYOUT_E        layout_type;/*!<�ӿؼ����з�ʽ*/
} GUIFORM_INIT_DATA_T;

/*! @struct GUIFORM_CHILD_WIDTH_T
@brief �ӿؼ������Ϣ
*/
typedef struct
{
    uint16                  add_data;   /*!<�̶���Ȼ��߰ٷֱ�[0,100]*/
    GUIFORM_CHILD_WIDTH_E   type;       /*!<�������*/
} GUIFORM_CHILD_WIDTH_T;

/*! @struct GUIFORM_CHILD_HEIGHT_T
@brief �ӿؼ��߶���Ϣ
*/
typedef struct
{
    uint16                  add_data;   /*!<�̶��߶�*/
    GUIFORM_CHILD_HEIGHT_E  type;       /*!<�߶�����*/
} GUIFORM_CHILD_HEIGHT_T;

/*! @struct GUIFORM_DYNA_CHILD_T
@brief ��̬�����ӿؼ���Ϣ
*/
typedef struct
{
    BOOLEAN         is_bg;          /*!<�Ƿ���form����*/
    BOOLEAN         is_softkey;     /*!<�Ƿ���softkey*/
    BOOLEAN         is_get_active;  /*!<�Ƿ��ܹ���ý���*/
    CAF_GUID_T      guid;           /*!<id*/
    MMI_HANDLE_T    child_handle;   /*!<�ӿؼ�handle*/
    void            *init_data_ptr; /*!<��ʼ������*/
} GUIFORM_DYNA_CHILD_T;

/*! @struct GUIFORM_NOTIFY_LINE_T
@brief �б仯��Ϣ
*/
typedef struct
{
    BOOLEAN         is_set_size;        /*!<�Ƿ����ƴ�С*/
    BOOLEAN         is_line_num_change; /*!<�Ƿ��������仯*/
    CAF_HANDLE_T    ctrl_handle;        /*!<�ؼ�handle*/
} GUIFORM_NOTIFY_LINE_T;

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ����form�ӿؼ�
@author Jassmine
@param index      [in] �ӿؼ�����
@param win_handle [in] ����handle
@param value_pptr [in] �ӿؼ���ֵ
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ParseChild(
                                   uint32        index,
                                   MMI_HANDLE_T  win_handle,
                                   uint32        **value_pptr
                                   );
/***************************************************************************//*!
@brief ���ü������form
@author fangfang.yao
@param form_handle      [in] form handle
@param child_handle     [in] ��form handle
@return �Ƿ�����ɹ�
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChild(
                             MMI_HANDLE_T    form_handle,
                              MMI_HANDLE_T   child_handle
                             );
 /***************************************************************************//*!
 @brief ����form reset slide frame number
 @author fangfang.yao
 @param form_handle    [in] form handle
 @param frame          [in] form reset slide frame
 @return ��
 @note
 *******************************************************************************/
 PUBLIC void CTRLFORM_SetResetSlideFrame(
                            MMI_HANDLE_T    form_handle,
                            uint16 frame
                            );
/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param rect_ptr    [in] ����
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetRect(
                             MMI_HANDLE_T    ctrl_handle,
                             GUI_RECT_T      *rect_ptr
                             );

/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�ɼ�
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_visible  [in] �Ƿ�ɼ�
@param is_update   [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetVisible(
                                   MMI_HANDLE_T      form_handle,
                                   BOOLEAN           is_visible,
                                   BOOLEAN           is_update
                                   );

/***************************************************************************//*!
@brief ��������ؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return ����
@note 
*******************************************************************************/
PUBLIC GUIFORM_TYPE_E CTRLFORM_GetType(
                                       MMI_HANDLE_T ctrl_handle
                                       );

/***************************************************************************//*!
@brief ���������ؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param type        [in] ����
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetType(
                             MMI_HANDLE_T       ctrl_handle,
                             GUIFORM_TYPE_E     type
                             );

/***************************************************************************//*!
@brief ��������ؼ����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@return ���
@note 
*******************************************************************************/
PUBLIC GUIFORM_STYLE_E CTRLFORM_GetStyle(
                                         MMI_HANDLE_T   ctrl_handle
                                         );

/***************************************************************************//*!
@brief ���������ؼ����
@author Jassmine
@param ctrl_handle [in] �ؼ�handle
@param style       [in] ���
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetStyle(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_STYLE_E   style
                              );

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param form_handle [in] �ؼ�handle
@param bg_ptr      [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetBg(
                              MMI_HANDLE_T   form_handle,//in:
                              GUI_BG_T       *bg_ptr     //in:
                              );

/***************************************************************************//*!
@brief �����Ƿ񱣴汳��rgb565����
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_save     [in] �Ƿ񱣴�
@return �����Ƿ�ɹ�
@note ���Լ��ٱ����������,�����ʾ�ٶ�,���������ڴ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSaveBg(
                                  MMI_HANDLE_T   form_handle,//in:
                                  BOOLEAN        is_save     //in:
                                  );

/***************************************************************************//*!
@brief ���ÿؼ�padding
@author Jassmine
@param form_handle [in] �ؼ�handle
@param padding_ptr [in] ���
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPadding(
                                   MMI_HANDLE_T     form_handle,
                                   GUI_PADDING_T    *padding_ptr
                                   );

/***************************************************************************//*!
@brief ���ÿؼ�padding
@author Jassmine
@param form_handle      [in] �ؼ�handle
@param padding_space    [in] ���
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetPaddingEx(
                                     MMI_HANDLE_T   form_handle,
                                     uint16         padding_space
                                     );

/***************************************************************************//*!
@brief ���ָ���ӿؼ���Id
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_index  [in] �ӿؼ�����
@return �ؼ�Id
@note 
*******************************************************************************/
PUBLIC MMI_CTRL_ID_T CTRLFORM_GetChildCtrlId(
                                             MMI_HANDLE_T   form_handle,    //in:
                                             uint16         child_index     //in:
                                             );

/***************************************************************************//*!
@brief ����Form�ؼ��ķָ���
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param line_width   [in] �ָ��߿��,0����ʾ
@param line_color   [in] �ָ�����ɫ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDividingLine(
                                        MMI_HANDLE_T    form_handle,//in:
                                        uint16          line_width, //in:0 don't display dividing line
                                        GUI_COLOR_T     line_color  //in:
                                        );

/***************************************************************************//*!
@brief ���������ؼ��������ӿؼ�ˮƽ��ֱ���
@author Jassmine
@param form_handle   [in] �ؼ�handle
@param hor_space_ptr [in] ˮƽ���,����Ϊ��
@param ver_space_ptr [in] ��ֱ���,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSpace(
                                 MMI_HANDLE_T    form_handle,    //in:
                                 uint16          *hor_space_ptr, //in:may PNULL
                                 uint16          *ver_space_ptr  //in:may PNULL
                                 );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!

@brief 
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@param layout_type [in] layout_type
@return NONE
@note 
*******************************************************************************/
PUBLIC void CTRLFORM_SetLayoutType(
                              MMI_HANDLE_T      ctrl_handle,
                              GUIFORM_LAYOUT_E   layout_type
                              );

/***************************************************************************//*!
@brief 
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_LAYOUT_E
@note 
*******************************************************************************/
PUBLIC GUIFORM_LAYOUT_E CTRLFORM_GetLayoutType(
                              MMI_HANDLE_T      ctrl_handle
                              );


#endif
/***************************************************************************//*!
@brief �����ӿؼ������ǰһ���ӿؼ���ˮƽ��ֱ���
@author Jassmine
@param form_handle   [in] �ؼ�handle
@param child_handle  [in] �ӿؼ�handle
@param hor_space_ptr [in] ˮƽ���,����Ϊ��
@param ver_space_ptr [in] ��ֱ���,����Ϊ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildSpace(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      MMI_HANDLE_T   child_handle,   //in:
                                      uint16         *hor_space_ptr, //in:may PNULL
                                      uint16         *ver_space_ptr  //in:may PNULL
                                      );

/***************************************************************************//*!
@brief �����ӿؼ����
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_handle [in] �ӿؼ�handle
@param width_ptr    [in] ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildWidth(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_WIDTH_T  *width_ptr      //in:
                                      );

/***************************************************************************//*!
@brief �����ӿؼ��߶�
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_handle [in] �ӿؼ�handle
@param height_ptr   [in] ��
@return �����Ƿ�ɹ�
@note Ĭ�ϸ߶ȸ��ݿ�ȼ���õ�
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildHeight(
                                       MMI_HANDLE_T              form_handle,    //in:
                                       MMI_HANDLE_T              child_handle,   //in:
                                       GUIFORM_CHILD_HEIGHT_T    *height_ptr     //in:
                                       );

/***************************************************************************//*!
@brief ���������ӿؼ��ı���,�߿��������
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param bg_ptr       [in] ����
@param border_ptr   [in] �߿�
@param font_ptr     [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildTheme(
                                      MMI_HANDLE_T   form_handle,    //in:
                                      GUI_BG_T       *bg_ptr,        //in/out:may PNULL
                                      GUI_BORDER_T   *border_ptr,    //in/out:may PNULL
                                      GUI_FONT_ALL_T *font_ptr       //in/out:may PNULL
                                      );

/***************************************************************************//*!
@brief ���ü����ӿؼ��ı���,�߿��������
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param bg_ptr       [in] ����
@param border_ptr   [in] �߿�
@param font_ptr     [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetActiveChildTheme(
                                            MMI_HANDLE_T     form_handle,    //in:
                                            GUI_BG_T         *bg_ptr,        //in/out:may PNULL
                                            GUI_BORDER_T     *border_ptr,    //in/out:may PNULL
                                            GUI_FONT_ALL_T   *font_ptr       //in/out:may PNULL
                                            );

/***************************************************************************//*!
@brief ���ûֻ��ӿؼ��ı���,�߿��������
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param bg_ptr       [in] ����
@param border_ptr   [in] �߿�
@param font_ptr     [in] ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetGrayChildTheme(
                                          MMI_HANDLE_T       form_handle,    //in:
                                          GUI_BG_T           *bg_ptr,        //in/out:may PNULL
                                          GUI_BORDER_T       *border_ptr,    //in/out:may PNULL
                                          GUI_FONT_ALL_T     *font_ptr       //in/out:may PNULL
                                          );

/***************************************************************************//*!
@brief �������������ؼ��Ĵ�С�仯��Χ
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param min_height   [in] ��Сֵ,Ĭ��ֵΪ0
@param max_height   [in] ���ֵ,Ĭ��ֵΪ0xffff
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetSizeRange(
                                     MMI_HANDLE_T   form_handle,    //in:
                                     MMI_HANDLE_T   child_handle,   //in:
                                     uint16         min_height,     //in:
                                     uint16         max_height      //in:
                                     );

/***************************************************************************//*!
@brief ���������ؼ������ӿؼ����з�ʽ
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param align        [in] �ӿؼ���ʾ���з�ʽ
@return �����Ƿ�ɹ�
@note ֻ����ӿؼ��������е������ؼ���Ч
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAlign(
                                 MMI_HANDLE_T           form_handle,    //in:
                                 GUIFORM_CHILD_ALIGN_E  align           //in:
                                 );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!
@brief set the align for all child, support in layout order only.
@author ningbiao.huang
@param form_handle[in] : form ctrl handle
@param child_align[in] : child_align for set
@return GUIFORM_CHILD_ALIGN_E
@note
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetAllChildAlign(
	MMI_HANDLE_T      ctrl_handle,
	GUIFORM_CHILD_ALIGN_E  child_align           //in:
);

/***************************************************************************//*!
@brief get the align for all child,support in layout order only.
@author ningbiao.huang
@param form_handle   [in] form ctrl handle
@return GUIFORM_CHILD_ALIGN_E
@note 
*******************************************************************************/
PUBLIC GUIFORM_CHILD_ALIGN_E CTRLFORM_GetChildAlign(
                              MMI_HANDLE_T      ctrl_handle
                              );
#endif
/***************************************************************************//*!
@brief �����ӿؼ����з�ʽ
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_handle [in] �ӿؼ�handle
@param align        [in] �ӿؼ���ʾ���з�ʽ
@return �����Ƿ�ɹ�
@note ֻ���˳�����е��ӿؼ���Ч
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildAlign(
                                      MMI_HANDLE_T           form_handle,    //in:
                                      MMI_HANDLE_T           child_handle,   //in:
                                      GUIFORM_CHILD_ALIGN_E  align           //in:
                                      );

/***************************************************************************//*!
@brief ����ӿؼ���ʾ��ʽ
@author Jassmine
@param form_handle      [in] �ؼ�handle
@param child_handle     [in] �ӿؼ�handle
@param display_type_ptr [in/out] ��ʾ��ʽ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetChildDisplay(
                                        MMI_HANDLE_T             form_handle,        //in:
                                        MMI_HANDLE_T             child_handle,       //in:
                                        GUIFORM_CHILD_DISPLAY_E  *display_type_ptr   //in/out:
                                        );

/***************************************************************************//*!
@brief �����ӿؼ���ʾ��ʽ
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_handle [in] �ӿؼ�handle
@param display_type [in] ��ʾ��ʽ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplay(
                                        MMI_HANDLE_T             form_handle,
                                        MMI_HANDLE_T             child_handle,
                                        GUIFORM_CHILD_DISPLAY_E  display_type
                                        );

/***************************************************************************//*!
@brief �����ӿؼ���ʾ��ʽ,ֻ����,���Ű�,��ˢ��
@author Jassmine
@param form_handle  [in] �ؼ�handle
@param child_handle [in] �ӿؼ�handle
@param display_type [in] ��ʾ��ʽ
@return �����Ƿ�ɹ�
@note ��formδ�Ű�֮ǰ����,��������ٶ�,����open,full paint msg����
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDisplayOnly(
                                            MMI_HANDLE_T            form_handle,
                                            MMI_HANDLE_T            child_handle,
                                            GUIFORM_CHILD_DISPLAY_E display_type
                                            );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : set child control dock type
//  Global resource dependence : 
//  Author: Jassmine
//  Note: only set one control dock type
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetChildDock(
                                     MMI_HANDLE_T           form_handle,
                                     MMI_HANDLE_T           child_handle,
                                     GUIFORM_CHILD_DOCK_E   dock_type
                                     );
#endif

/***************************************************************************//*!
@brief ���������ؼ��������ӿؼ��Ƿ�����FORM�����䱳��
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_permit   [in] �Ƿ��������ñ���
@return �����Ƿ�ɹ�
@note Ĭ��TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBg(
                                      MMI_HANDLE_T   form_handle,
                                      BOOLEAN        is_permit
                                      );

/***************************************************************************//*!
@brief ���������ؼ��������ӿؼ��Ƿ�����FORM������߿�
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_permit   [in] �Ƿ��������ñ߿�
@return �����Ƿ�ɹ�
@note Ĭ��TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildBorder(
                                          MMI_HANDLE_T   form_handle,
                                          BOOLEAN        is_permit
                                          );

/***************************************************************************//*!
@brief ���������ؼ��������ӿؼ��Ƿ�����FORM�����������С
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_permit   [in] �Ƿ�������������
@return �����Ƿ�ɹ�
@note Ĭ��TRUE
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFont(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        );

/*****************************************************************************/
//  Description : set form all child is permit form set it's font color
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_PermitChildFontColor(
                                        MMI_HANDLE_T     form_handle,
                                        BOOLEAN          is_permit
                                        );

/***************************************************************************//*!
@brief ���������ؼ��Ƿ�֧�ֻ���
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_support  [in] �Ƿ�������
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSlide(
                                MMI_HANDLE_T   form_handle,
                                BOOLEAN        is_support
                                );

/***************************************************************************//*!
@brief �Ƿ���������Զ������ӿؼ���ʾ�����λ��
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_auto     [in] �Ƿ��Զ�������ʾ�����λ��
@return �����Ƿ�ɹ�
@note һ�����������ʾ,�����������������������ʾ
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAutoDir(
                                  MMI_HANDLE_T   form_handle,
                                  BOOLEAN        is_auto
                                  );

/***************************************************************************//*!
@brief �Ƿ�ѭ���������¼�
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_circular [in] �Ƿ�ѭ���������¼�
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleUpDown(
                                                MMI_HANDLE_T    form_handle,
                                                BOOLEAN         is_circular
                                                );

/***************************************************************************//*!
@brief �Ƿ�ѭ���������Ҽ�
@author Jassmine
@param form_handle [in] �ؼ�handle
@param is_circular [in] �Ƿ�ѭ���������Ҽ�
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetCircularHandleLeftRight(
                                                   MMI_HANDLE_T  form_handle,
                                                   BOOLEAN       is_circular
                                                   );

/***************************************************************************//*!
@brief ��ÿؼ��ĸ߶�
@author Jassmine
@param form_handle [in] �ؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC uint16 CTRLFORM_GetHeight(
                                 MMI_HANDLE_T    form_handle //in:
                                 );

/***************************************************************************//*!
@brief ǿ�ƽ����Ű�
@author Jassmine
@param form_handle  [in] form�ؼ�handle
@param is_update    [in] �Ƿ���Ҫˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_ForceLayout(
                                    MMI_HANDLE_T    form_handle //in:
                                    );

/***************************************************************************//*!
@brief ��̬���������ؼ�
@author Jassmine
@param win_handle  [in] ����handle
@param form_handle [in] �ؼ�handle
@param layout_type [in] �ӿؼ����з�ʽ
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaCtrl(
                                      MMI_HANDLE_T       win_handle, //in:
                                      MMI_HANDLE_T       form_handle,//in:
                                      GUIFORM_LAYOUT_E   layout_type //in:
                                      );

/***************************************************************************//*!
@brief ��̬���������ؼ��ӿؼ�
@author Jassmine
@param win_handle     [in] ����handle
@param form_handle    [in] ���ؼ�handle
@param child_data_ptr [in] �ӿؼ���ʼ����
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CreatDynaChildCtrl(
                                           MMI_HANDLE_T          win_handle,     //in:
                                           MMI_HANDLE_T          form_handle,    //in:
                                           GUIFORM_DYNA_CHILD_T  *child_data_ptr //in:
                                           );

/***************************************************************************//*!
@brief ��̬���������ؼ��ӿؼ�
@author Jassmine
@param form_handle  [in] ���ؼ�handle
@param child_handle [in] �ӿؼ�handle
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLFORM_DestroyDynaChildCtrl(
                                             MMI_HANDLE_T   form_handle,    //in:
                                             MMI_HANDLE_T   child_handle    //in:
                                             );

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
/***************************************************************************//*!
@brief Add a exist ctrl to form_ctrl
@author ningbiao.huang
@param win_handle  [in]  win handle
@param form_handle  [in] parent form handle
@param child_handle [in] child ctrl handle
@return TRUE:pass FALSE:fail
@note 
*******************************************************************************/
#endif
#ifdef GUI_DUMP_CTRL_TREE
/*****************************************************************************/
//  Description : dump control tree by form ptr
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DumpTree(
                              MMI_HANDLE_T  form_handle
                              );
#endif

/**--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

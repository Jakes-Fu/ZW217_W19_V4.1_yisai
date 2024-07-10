/*! @file guilabel.h
@brief 
���ļ�������Label�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Label ��ǩ(Label)

-# @ref sec_label_function
-# @ref sec_label_usage
    -# @ref ssec_label_usage_def
    -# @ref ssec_label_usage_create
    -# @ref ssec_label_usage_function_list
-# @ref sec_label_app

@section sec_label_function �ؼ��Ĺ���

Label�ؼ���һ���ı�����ؼ����ܹ��������ֵ���ʾλ�á���ʾ���򡢴�С������ȷ�������ͨ��LCD�ӿ�������û���Label�ؼ��ܹ���ʾ�����ı�����ʾ���ı����ܱ�ֱ�ӱ༭�����ı��Ŀ�ȳ����ؼ�����ʾ����ʱ�����Թ�����ʾ�ı����ݡ�Label�ؼ�������ʾ�ı�֮�⣬���������ı��������ʾһ��ͼ�꣨ICON����Label�ڽ���״̬ʱ���Թ�����ʾ���ǽ���ʱ��ʾʡ�Ե㣬��Ȼ�û������������۽���ͷǽ��㶼���й�����

@section sec_label_usage �ؼ�ʹ��

@subsection ssec_label_usage_def �궨�����

@code
#define CREATE_LABEL_CTRL(_ALIGN_, _CTRL_ID_) \
        MMK_CREATE_LABEL, _ALIGN_, _CTRL_ID_
@endcode

- ���ڴ�����ǩ�ؼ���
    - _ALIGN_ �ı����뷽ʽ
    - _CTRL_ID_ �ؼ�ID

@subsection ssec_label_usage_create �ؼ�����

���б�ؼ���ͬ������Label����ǩ���ؼ�ʱ�����ÿؼ���ID���ؼ���ʾ�ı��Ķ��뷽ʽ�Ϳ����ˣ�������ʾ��������Ҫ�ڴ��ڵ�Open��Ϣ�����õģ����֧�ֺ�����������Ҫ����both rect�����磺

������lab_rect {10, 120, 200, 150}��ʾһ��Label�ؼ�����ô��Ҫ�ڴ���Table�����ӣ�
@code
CREATE_LABEL_CTRL(GUILABEL_LEFT, ctrl_id),
@endcode
Ȼ���ڴ���Open��Ϣ�����ӣ�
@code
GUILABEL_SetRect(ctrl_id, &lab_rect, FALSE); 
@endcode
@note FALSE ��ʾ��������ˢ�¡�

@subsection ssec_label_usage_function_list �ӿڽ���

�ڴ��ڱ�����CREATE_LABEL_CTRL�ֻ꣬��˵�������Ѿ������˿ؼ���ʵ��������������ʾ�κ����ݣ���Ϊ�û���û�ж�������ʲô��Ϣ��

-# @ref GUILABEL_SetText
    - ����Label�ؼ����ı�һ��������ڴ��ڵ�Open��Ϣ����ʱ���þͿ����ˣ���ʱ����Ҫ����ˢ�£���Ϊ���滹����յ�Paint��Ϣ���������ˢ���ˣ���ô���ܻᵼ����Ļˢ�²�ͬ������������������
    - Ȼ����ʵ�ʵ�Ӧ���У��кܶ��������Ҫ��ʱ����Label����ʾ���ݵģ����簴ĳһ��������Label���ݣ����ʱ����Ҫ��������Label��ʾ���ı�����ôҲ��Ҫ��ʱ��ˢ���ˣ����ʱ�򲻻��������������ԭ���ǵ�ǰ���ڻ����ڽ���״̬��
    - ��ʵ����
@code
MMI_STRING_T    item_str = {0};
MMI_GetLabelTextByLang(TXT_CL_TIMER_LAST_CALL,&item_str);
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@endcode
    - ����MMI_GetLabelTextByLang�ӿ���������ȡ�ַ�����ԴTXT_CL_TIMER_LAST_CALL���ı����ݵġ�
    - ��һ�����������Ҫ��ʾһ���Զ�����ַ������硰hello SPRD������ô����ֱ�����ã�
@code
MMI_STRING_T    item_str = {0};
wchar w_text[] = {'F', 'A', 'L', 'S', 'E', 0};
item_str.wstr_ptr = w_text;
item_str.wstr_len = 6;
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@code
-# @ref GUILABEL_SetTextById
    - ��ʵ����
@code
GUILABEL_SetTextById(ctrl_id,TXT_CL_TIMER_LAST_CALL, FALSE);
@endcode
-# @ref GUILABEL_SetBackgroundColor
    - Label�ı���֧����ɫ��ͼƬ����������ͼƬ���������ȼ����ߣ�Ҳ����˵һ���Ѿ�������ͼƬ��������ôLabel�ؼ���������ʾͼƬ�����Ա���ɫ���������ڼ���û������Label�ı���ͼƬ��
    - Label�ؼ�Ĭ�ϵı�����ɫΪMMI_TRANSPARENCE_COLOR����ɫֵ��0x0��Ϊ��ɫ�����ڴ����ؼ���ʱ��ͨ��MMITHEME_GetLabelStyle��ȡ�ġ���ƽ̨�У���ɫ����ʾʱ�Ǳ�͸����ɫ�ʣ����仰˵������û�û����������Label�ؼ��ı���ɫ�Ļ�����ô�ؼ���ֱ����ʾ�ڴ��ڵı����ϣ���ע�⣬�������û�����ñ���ͼƬ��
    - Label�ؼ���ʾ��ʱ��������ֱ�����ɫ��Ĭ����ɫ��ͬ����ô�ᷢ��Full paint��Ϣ�����ڣ��ô���ͳһˢ�±�������ΪLabel���ı���ֱ����ʾ�ڴ��ڵı����ϵģ���Ҳ�ᵼ�´�������ˢ����һ�Ρ��������ɫ��ͬ����ô����Ҫˢ�´��ڵı�����
    - Label�ǿ����ṩ��������ʾ��ʽ�ģ����ַ����Ŀ�ȳ����˿ؼ�����ʾ�����ʱ��Label�ؼ���������һ��Timer��ʵ�ֹ�����ʾ�����ʱ�����û�����ñ�����ɫ����ôÿ��Timer������ʱ��ͻ�����ˢ��һ�δ��ڡ��ںܶ�����£�����ˢ�´��ڻᵼ�³����Ч�ʽ��ͣ�����ҽ�����������Ҫ��Label���ַ�����ȳ���Label������������볬��Label��������ô�����ú�͸��ɫ����ͬ�ı���ɫ�����ס����ʱû����ȫ�����İ취��
    - ��Ȼ������㲻��Ҫ���ǳ����Ч�ʣ�ȷʵϣ����ʾ�ڴ��ڱ����ϵ�Label�ؼ�������ʾ�Ļ�����ô��ǧ��Ҫ��SetText�Ľӿڷ��ڴ��ڵ�Full pain��Ϣ�������ã���Ϊÿ��ˢ��ʱ��Ҫ�������ã�����������֮��ͻ��ͷ��ʼ��ʾ������Ȼ���ܴﵽ������ʾ��Ч����������˵��õ�����Label���ݵķ�ʽ���ڴ���Open��Ϣ��ʱ��
    - Label��һ���Ƚϼ򵥵Ŀؼ����������ڱ��������⣬�������ò�Ƹ�������࣬��ʵ��Ӧ�ù����У������ʹ���˼��Σ���ȻҲ�����������Ĺ����ˡ�������Ҫ���Ҽ���һ�£�����ؼ���û�а���ʵ�ֵĸ�����ʵ�ã����ǽ����Ժ�Ŀ��������о����޸ġ�
    - ��ʵ�������ÿؼ��ı���Ϊ��ɫ����ʾЧ����ͼ 2 20��
@code
GUILABEL_SetBackgroundColor(ctrl_id,0xFFFF);
@endcode
-# GUILABEL_SetBackgroundImage
    - Label�ؼ���Ĭ���������û�б���ͼƬ�ģ�ֻ��Ĭ�ϵı�����ɫ����һ���ڽ������ñ�����ɫ�ӿ�ʱ�Ѿ�����������
���û���Ҫ��ʾ����ͼƬ��ʱ����ô�ڿؼ���ʾ֮ǰ���øýӿڣ���ν�ڿؼ���ʾ֮ǰ���ã�Ҳ����˵���ڿؼ����յ�ctrl paint��Ϣ֮ǰ���������ǽ������ڵ���Ϣ����ʱ���������Ϣ�����̡��㽫�ῴ������������ʱOpen��Ϣ����paint��Ϣ֮ǰ���͵ģ������open��Ϣ��������Ϊ���գ���Ҳ��һ��һ�����ݵİ취����ΪOpen��Ϣֻ��һ�Σ����ú�֮���Ժ�Դ��ڵ��κβ����������������ÿؼ��ı����ˡ���ʵ��������Label�ؼ������������ؼ�Ҳһ������������ǰ�Ľ������б�ؼ�Append�б����ʱ��Ҳ�Ƿ���Open��Ϣ����ɵġ�
    - ��Ȼ���ؼ��ı������ǲ��ܸı�ģ��������Ҫ�ڲ������ڵĹ����У����簴ĳһ�����ı�ؼ��ı�������ô�ڴ�����ʱ�������������������ݡ�
    - ��ʵ����
@code
GUILABEL_SetBackgroundImage(ctrl_id, img_id);
@endcode
-# GUILABEL_SetFontParam
    - Label�ؼ������þ�����ʾһ����ǩ��������Ҫ�����Ծ����ı��������ı��ķ�񣬰����ı�����ɫ�����塢�ּ������ݣ�Ĭ�����ֵ���ڴ����ؼ�ʱ����MMITHEME_GetLabelStyle�ӿ����õģ��û����Ե���GUILABEL_SetFontParam���������ı�����ʾ���
    - ��ʵ����
@code
GUILABEL_SetFontParam(ctrl_id, font, color, space);
@endcode

@section sec_label_app �ؼ���Ӧ��

Label�Ǳ�ǩ����˼������ʾ����������༭���޸ġ��������������Σ����û���Ҫ��ʾһ������硰���������������⣺���������ڣ����ȣ������ʾ�Ĺ��ܾ�����ʾ�û�������ʲô���ݡ���ʱ����ʹ��Label�ؼ����������ճ̵ı༭����ʹ��������ǩ�ؼ�����ͼ 2 20��ʾ�����С����⣺���������ڣ�������ʱ�䣺���������������������͡�������ֹ���ڡ��ȡ�
@image html label_app_example.jpg 
ͼ 2 20Label�ؼ���Ӧ����ͼ
*******************************************************************************/

/*! @addtogroup  labelGrp Label
@brief label�ؼ�
@details 
-#  �����ı���ʾ
@{
*******************************************************************************/


#ifndef _GUILABEL_H_
#define _GUILABEL_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guistring.h"

#include "ctrllabel_export.h"

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
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUILABEL_GetText            CTRLLABEL_GetText
#define GUILABEL_SetText            CTRLLABEL_SetText
#define GUILABEL_SetTextById        CTRLLABEL_SetTextById
#define GUILABEL_SetTextRegion      CTRLLABEL_SetTextRegion
#define GUILABEL_SetIcon            CTRLLABEL_SetIcon
#define GUILABEL_SetMargin          CTRLLABEL_SetMargin
#define GUILABEL_SetMarginEx        CTRLLABEL_SetMarginEx
#define GUILABEL_SetBg              CTRLLABEL_SetBg
#define GUILABEL_SetBackgroundColor CTRLLABEL_SetBackgroundColor
#define GUILABEL_SetFont            CTRLLABEL_SetFont
#define GUILABEL_GetRect            CTRLLABEL_GetRect
#define GUILABEL_SetRect            CTRLLABEL_SetRect
#define GUILABEL_SetTpRect          CTRLLABEL_SetTpRect
#define GUILABEL_SetVisible         CTRLLABEL_SetVisible
#define GUILABEL_SetIconEx          CTRLLABEL_SetIconEx
#define GUILABEL_FlushBg            CTRLLABEL_FlushBg
#define GUILABEL_SetAlign           CTRLLABEL_SetAlign
#define GUILABEL_SetIsAutoScroll    CTRLLABEL_SetIsAutoScroll
#define GUILABEL_SetIsEllipsisEx    CTRLLABEL_SetIsEllipsisEx
#define GUILABEL_SetTextMargin      CTRLLABEL_SetTextMargin

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

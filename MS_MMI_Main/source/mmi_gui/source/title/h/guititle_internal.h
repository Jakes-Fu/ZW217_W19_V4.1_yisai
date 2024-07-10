/*! @file guititle_internal.h
@brief ���ļ�������title�ؼ��Ķ���ӿں��������� 
@author xiaoqing.lu
@version 1.0
@date 11/05/2011
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/05/2011     xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_Title ����(Title)

-# @ref sec_title_function
-# @ref sec_title_create
    -# @ref ssec_title_create_def
    -# @ref ssec_title_create_function_list
-# @ref sec_title_style
-# @ref sec_title_app

@section sec_title_function �ؼ�����

- ����ؼ���Title����һ�������GUI�ؼ���һ������£�����ʾ�ڴ�������������棬����������ڰ���״̬���ؼ�����ôTitle�ؼ���ʾ��״̬�������档����ؼ���ʾһ���ı�����ͼ����ı�����ϣ���ʾ���ڵ����ֺ����ԡ�
- ����ؼ�Ҳ�Ǵ��ڵ�����֮һ��һ������ֻ��һ������ؼ�������IDҲ�ǹ̶�����ġ�
- ����ؼ���ʾ�����ݰ������ı���Ϣ�������ı���Ϣ��Icon�����ڴ����л���ͼ�꣨�����ҷ����ͷicon����
- ����ؼ������ı���Ϣ������ʾ�ڱ���ؼ���������С�������λ�á��û����������ı��ķ�����ı������С������ɫ�ʡ�����ͼƬ�ȡ�
- ����ؼ��ĸ����ı���Ϣһ����ʾ�ڱ���ؼ������½ǣ��û����������ı������ݡ���ʾ�����Լ����ֵ���ʾ���
- ����ؼ���ͼ��һ����ʾ�ڱ���ؼ��������࣬��������һ��˵�����ڡ�
- �����л����ڵ�Icon��ʾ�ڱ���ؼ����������ˣ�ע�⣬һ����������������ؼ���ͼ��ͬʱ��ʾ��
- �����Ǵ��ڵ����ԣ��û�����ͨ���ӿڻ��ĳ�����ڱ������Ϣ��

@section sec_title_create �ؼ�����

�����״̬��һ�����ǿؼ�������֮һ���������Ĵ���Ҫ��Եĸ���һЩ����ΪҪ��д�ı���ID��

@subsection ssec_title_create_def �궨�����

@code
#define WIN_TITLE(_TITEL_ID_) \
		MMK_CREATE_TITLE, _TITEL_ID_
@endcode
- ����һ������ؼ���
    - _TITEL_ID_ �����ı���ID

@note ���һ��������Ҫ��ʾ���⣬�������������Ҫ��̬���õģ�����˵��ؼ������ñ���ؼ���ʾ���ı�������ʱ���¡����������ڴ��ڵ�Table������WIN_TITLE(TXT_NULL)��Ҳ����˵����Դ���յ��ı�����ȻΪ������Ϊ�գ�������Ҫ�������������Title�ؼ�����������Ҳ���޷���̬����Title�ؼ����ı��������㶯̬����Title�ؼ���

@subsection ssec_title_create_function_list �ؼ��ӿ�

Title�ؼ�����Ϊ���ڵ�����֮һ������ӿڶ�����guiwin.h�С�
- @ref GUIWIN_SetTitleText
- @ref GUIWIN_SetTitleTextId

@section sec_title_style �ؼ����

����ؼ����Լ��ķ�񣬵��Ƕ���һ��ƽ̨�����ı���Ӧ�þ���ͳһ�ķ�������Ҫ�޸ı���ķ����Ϣ��ֻ��Ҫ�޸�MMITHEME_GetTitleStyle�еĳ�ʼ����Ϣ���ýӿ���mmi_theme.c��ʵ�֡�

@section sec_title_app �ؼ�Ӧ��

����ؼ���Ӧ�ã�
-# ��ʾ���ڣ�
@image html title_display1.jpg
ͼ 2 39 �ؼ���ʾ��ͼ
-# ��ʾ������Ϣ�ʹ������ݣ�ͼ�С�2/8����ʾ��ǰ���㴦�ڵ�2����������8����
@image html title_display2.jpg
ͼ 2 40 �ؼ���ʾ��ͼ
-# ��ʾ�л����ڵ�ͼ�꣬ͼ�����Ҽ�ͷ�����л���һ����Ϣ��
@image html title_display3.jpg
ͼ 2 41 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup titleGrp Title
@brief title�ؼ�
@details title�ؼ�
@{
*******************************************************************************/

#ifndef _GUI_TITLE_INTERNAL_H_
#define _GUI_TITLE_INTERNAL_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guifont.h"
#include "guititle.h"
#include "mmi_font.h"

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
#define GUITITLE_SetTextByPtr           CTRLTITLE_SetTextByPtr
#define GUITITLE_SetSubTextByPtr        CTRLTITLE_SetSubTextByPtr
#define GUITITLE_SetSubTextParamByPtr   CTRLTITLE_SetSubTextParamByPtr
#define GUITITLE_SetFontColorByPtr      CTRLTITLE_SetFontColorByPtr  
#define GUITITLE_SetRectByPtr           CTRLTITLE_SetRectByPtr
#define GUITITLE_SetIsVisible           CTRLTITLE_SetIsVisible
#define GUITITLE_GetTitlePtr            CTRLTITLE_GetTitlePtr
#define GUITITLE_SetButtonState         CTRLTITLE_SetButtonState
#define GUITITLE_SetButtonTextId        CTRLTITLE_SetButtonTextId
#define GUITITLE_SetButtonTextByPtr     CTRLTITLE_SetButtonTextByPtr
#define GUITITLE_SetButtonImage         CTRLTITLE_SetButtonImage
#define GUITITLE_SetButtonHandleMsgInfo CTRLTITLE_SetButtonHandleMsgInfo
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_GUI_TITLE_INTERNAL_H_
/*@}*/

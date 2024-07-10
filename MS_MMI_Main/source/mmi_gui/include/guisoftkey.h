/*! @file guisoftkey.h
@brief ���ļ�������softkey�ؼ��Ķ���ӿں��������� 
@author jibin
@version 1.0
@date 05/08/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 05/08/2004     jibin       		Create
*******************************************************************************/

/*! @page page_Softkey Softkey(Softkey)

-# @ref sec_softkey_function
-# @ref sec_softkey_design
-# @ref sec_softkey_def
-# @ref sec_softkey_create
-# @ref sec_softkey_function_list
-# @ref sec_softkey_app
-# @ref sec_softkey_tips

@section sec_softkey_function �ؼ�����

- ���ƴ��ڵ�Softkey���ؼ��ֳ��������򣬷ֱ�ͼ����ϵ��������Web���������һһ��Ӧ��Softkey�ؼ����ڴ����е�����֮һ���������ؼ���ͬ����ÿ������ֻ��һ��Softkey�ؼ����������ʹ����е������ؼ���ϵ���ܣ��������в�ͬ�Ŀؼ����ڽ���״̬ʱ�����ʵ��Softkey����������λ�ã����Էֱ��Ͳ�ͬ����Ϣ(�������Ϣ��Ӧ)������ؼ��ʹ��ڣ�����Softkey�ɶ�̬��ʾ��ͬ���ı���Ϣ��
- ���⣬Softkey�ؼ������ս��㡣

@section sec_softkey_design Softkey��ʾ���⼰���

���ʱ���ǵ�����ͨ�������softkey�ؼ�������ʾ�������⣬�������ʾ���µ�����������ʾ�����������£�
-# ��ֻ��һ����������ң�������������ң���������ʱ�����м�ȷֳ���������
-# ������������������ʱ�����ȼ����м���ı���ȣ����м��������Ȳ�����softkey��ȵ�1/3��
-# ����֧����ʾicon��
-# ����֧����ʾ����ʡ�Ե�

@section sec_softkey_def �궨�����

@code
#define WIN_SOFTKEY(_LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_) \
        MMK_CREATE_SOFTKEY, _LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_
@endcode
- ���ڴ���Softkey�ؼ�
    - _LEFT_SOFT_ID_ Softkey�����Text ID
    - _MID_SOFT_ID_ Softkey�����Text ID
    - _RIGHT_SOFT_ID_ Softkey�����Text ID

@section sec_softkey_create �ؼ�����

- WIN_SOFTKEY�������������ֱ�������������ʾ���ı���Ϣ����̬����ʹ�øú꣬����û���Ҫ���ó�ʼ�����ı�����ô�����ڴ���Table�г�ʼ����ʱ������룬����û���Ҫ��̬�޸��ı���Ϣ������Ե�����ص����ýӿڣ���GUIWIN_SetSoftkeyTextId������softkey�ؼ���ֻ��ʾ�̶���Դ���ı���Ҳ����˵��ֻ�����ù̶���Դ���ı�ID��Ϊsoftkey��ʾ�����ݡ�
- Softkey�ı����ǹ̶�����ģ�ÿ������ֱ������ű���ͼƬ����Ϊ�����뵯���Ч������״̬��
- Softkey�ؼ���Title�ؼ�һ������һ��ƽ̨�У�Ӧ�þ���ͳһ�ķ������û���Ҫ�޸�softkey��ʾ��ͼƬ�Լ��ı���񣬿���ֱ���޸�MMITHEME_GetSoftkeyStyle�ӿڵ�ʵ�֡�����ӿ����ڴ����ؼ���ʱ�򱻵��õģ����е�softkey�ؼ��ڴ���ʱ��Ҫ��������ӿڣ���Ҳ�ܹ���֤�ؼ���Ĭ�Ϸ���ͳһ�ԡ���Ȼ��Ϊ��ʵ�ָ��Ի��Ŀؼ����û������ھ���Ĵ��������ò�ͬ����Softkey����Щ�ӿ���GUIWIN_SetSoftkeyStyle��GUIWIN_SetSoftkeyBtnState�ȡ�
- ƽ̨�У��кܶ�Ŀؼ���Softkey�������ܣ�����Editbox�ؼ��������������Ϊ�յ�ʱ�����������ʾ�ı����������ݵ�ʱ���������ʾ��ȷ�������������ʾ��ɾ��������Щ������ҪEditbox�ؼ��Լ����õġ�

@section sec_softkey_function_list �ؼ��ӿڽ���

-# @ref GUIWIN_SetSoftkeyTextId
-# @ref GUIWIN_SetSoftkeyBtnState
-# @ref GUIWIN_SetSoftkeyTPEnable

@section sec_softkey_app �ؼ�Ӧ��

- Softkey������ʾ�����������Web������������ı���Ϣ��
- Softkey�ؼ����ڴ����²���һ����������һ����ֳ�������3�����򣬷ֱ��Ӧ���������������������ÿ�������Ͽ�����ʾ���֡�һ�������ֻ�õ��������������Softkey�ؼ���ʾЧ����ͼ 2 43��ʾ���������ʾ��Option�����������ʾ��Back�����м���ʾ��Contacts�������ʰ���ʱ����ʾ����Ч������������ʱ��Ӧ������
@image html softkey_display.jpg
ͼ 2 43 �ؼ���ʾ��ͼ

@section sec_softkey_tips Softkey�ؼ���ʾ˵��

-# ��û���м����ʱ��������ʾ���ַ����Դﵽһ��Ŀ��
-# ���м����ʱ���м�����ȼ������򣬵�����Ȳ������������1/3
-# ����ʾ���ֳ����Լ�������ʱ���û�������ʡ����ʾ
*******************************************************************************/

/*! @addtogroup softkeyGrp Softkey
@brief softkey�ؼ�
@details softkey�ؼ�
@{
*******************************************************************************/

#ifndef _GUI_SOFTKEY_H_
#define _GUI_SOFTKEY_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guianim.h"
#include "ctrlsoftkey_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUISOFTKEY_SetTextId               CTRLSOFTKEY_SetTextId
#define GUISOFTKEY_SetButtonState          CTRLSOFTKEY_SetButtonState
#define GUISOFTKEY_SetButtonGray           CTRLSOFTKEY_SetButtonGray
#define GUISOFTKEY_GetTextId               CTRLSOFTKEY_GetTextId
#define GUISOFTKEY_SetStyle                CTRLSOFTKEY_SetStyle
#define GUISOFTKEY_SetPressedStyle         CTRLSOFTKEY_SetPressedStyle
#define GUISOFTKEY_SetBgType               CTRLSOFTKEY_SetBgType
#define GUISOFTKEY_Update                  CTRLSOFTKEY_Update
#define GUISOFTKEY_IsTPEnable              CTRLSOFTKEY_IsTPEnable
#define GUISOFTKEY_SetTPEnable             CTRLSOFTKEY_SetTPEnable
#define GUISOFTKEY_SetVisible              CTRLSOFTKEY_SetVisible
#define GUISOFTKEY_GetVisible              CTRLSOFTKEY_GetVisible
#define GUISOFTKEY_SetButtonIconId         CTRLSOFTKEY_SetButtonIconId
#define GUISOFTKEY_SetButtonTextId         CTRLSOFTKEY_SetButtonTextId
#define GUISOFTKEY_Setfont                 CTRLSOFTKEY_Setfont
#define GUISOFTKEY_CreateCtrlDyn           CTRLSOFTKEY_CreateCtrlDyn
#define GUISOFTKEY_SetButtonTextPtr        CTRLSOFTKEY_SetButtonTextPtr
#define GUIBUTTONSOFTKEY_GetStyleFromText  CTRLBUTTONSOFTKEY_GetStyleFromText
#define GUIBUTTONSOFTKEY_GetButtonHandle   CTRLBUTTONSOFTKEY_GetButtonHandle
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
/*! @} */



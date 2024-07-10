/*! @file guisetlist.h
@brief  ���ļ�������setlist�ؼ��Ķ���ӿں��������� 
@author xiaoqing.lu  
@version 1.0
@date 11/04/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/04/2009     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_SetList SetList(SetList)

-# @ref sec_setlist_function
-# @ref sec_setlist_def
-# @ref sec_setlist_create
-# @ref sec_setlist_function_list
-# @ref sec_setlist_app

@section sec_setlist_function �ؼ�����

Setting List�ؼ���Ϊ����ֱ����ʾ��ǰѡ�����ֵ�����Ա���Ŀؼ��������������ԣ�
-# ��ѡ���ܣ�Settinglist�ܰ������ѡ�����ǰֻ��ʾһ��ѡ�����ֵ��
-# ѡ���л���Settinglist����ͨ�����Ҽ��л�ѡ�����ʾ�л�������ݣ����֧�ִ��ʣ����ͨ�����ʵ�����Ҽ�ͷ�л�ѡ�
-# ��ֻ�������ѡ��ʱ����Web�����Զ��л���
-# ��ʾ���ݣ�����ʾѡ�����ݣ�����ͼƬ������
-# ��ʾ��񣺵�����ʾ���߶ȿɵ�
-# ѡ�����ݣ�Settinglist��ѡ�����ݿ������ı���ͼƬ���ؼ��ɰ���һ��������Ϣ��
-# �ؼ������Ҽ�ͷ�ɸ�����Ҫ������ʾ�Ͳ���ʾ
-# �ؼ����������Ƿ�ѭ��
-# ��ѡ���2��ʱ�����Դ򿪴��ڣ�ȫ�����߰������ڶ��ǿ��Եģ�����Ҫ�û����á�����ͬһ��Ŀ���ͳһ
-# �򿪴������б���ʽ��ʾ���б����softkey��

@section sec_setlist_def �궨�����

@code
#define CREATE_SETLIST_CTRL(_CTRL_ID_) \
        MMK_CREATE_SETLIST, _CTRL_ID_
@endcode
- ���ڶ���setlist�ؼ�
    - _CTRL_ID_ �ؼ�ID

@section sec_setlist_create �ؼ�����

Setlist�ؼ�֧�ֶ�̬�����;�̬����������ʽ����̬����ʱ����Ҫ�������궨�����ӵ����ڵ�Table�У�
@code
WINDOW_TABLE( MMISAMPLE_SETLIST_TEST_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleTestSetlistWinMsg),
    WIN_ID( MMISETLIST_TEST_WIN_ID ),            CREATE_SETLIST_CTRL(MMISETLIST_TEST_CTRL_ID),
        END_WIN
};
@endcode

��̬����ʱ�����ú��� @ref GUISETLIST_CreateCtrlDyn
    - �û���̬����ʱ��Ҫ��Ҫ���봰�ھ����id���ؼ�id�����������򣬾�̬����ֻ��Ҫ���ÿؼ�ID��

@section sec_setlist_function_list �ؼ��ӿڽ���

-# @ref GUISETLIST_AddItem
-# @ref GUISETLIST_SetBackGround
-# @ref GUISETLIST_CreateCtrlDyn
-# �������ú���
    - @ref GUISETLIST_SetTitleInfo
    - @ref GUISETLIST_SetTitleTextId

@section sec_setlist_app �ؼ�Ӧ��
Setlist�кܶ�����;���������ӵ����͡��ճ̵����͵ȣ�����ͼ��ͼ��Type�µĿؼ�Ϊsetlist�����Ҽ����л����Ρ�ÿ�ܡ�ÿ�¡�ÿ��ȵȣ�
@image html setlist_display1.jpg
ͼ 2 44 Setlist�ؼ���ʾ��ͼ
�����ʵ����ؼ�����web��ʱ�����Ե������´��ڣ�
@image html setlist_display2.jpg
ͼ 2 45 �ؼ�������������
@image html setlist_display3.jpg
ͼ 2 46 �ؼ�����ȫ������
*******************************************************************************/

/*! @addtogroup setlistGrp SetList
@brief setlistģ��
@details setlistģ��
@{
*******************************************************************************/


#ifndef _GUI_SETLIST_H_
#define _GUI_SETLIST_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guicommon.h"
#include "guictrl.h"
#include "mmi_link.h"
#include "mmitheme_setlist.h"
#include "ctrlsetlist_export.h"
//#include "..\\..\\mmi_ctrl\\source\\Setlist\\h\\ctrlsetlist.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#define GUISETLIST_AddItem            CTRLSETLIST_AddItem
#define GUISETLIST_AddItemById        CTRLSETLIST_AddItemById
#define GUISETLIST_InsertItem         CTRLSETLIST_InsertItem
#define GUISETLIST_ModifyItem         CTRLSETLIST_ModifyItem
#define GUISETLIST_DeleteItem         CTRLSETLIST_DeleteItem
#define GUISETLIST_DeleteAllItem      CTRLSETLIST_DeleteAllItem
#define GUISETLIST_SetBackGround      CTRLSETLIST_SetBackGround
#define GUISETLIST_SetCurIndex        CTRLSETLIST_SetCurIndex
#define GUISETLIST_GetCurIndex        CTRLSETLIST_GetCurIndex
#define GUISETLIST_CreateCtrlDyn      CTRLSETLIST_CreateCtrlDyn
#define GUISETLIST_SetDisplayRect     CTRLSETLIST_SetDisplayRect
#define GUISETLIST_GetDisplayRect     CTRLSETLIST_GetDisplayRect
#ifdef TOUCH_PANEL_SUPPORT
#define GUISETLIST_SetTpRect          CTRLSETLIST_SetTpRect
#endif
#define GUISETLIST_SetBorder          CTRLSETLIST_SetBorder
#define GUISETLIST_SetFont            CTRLSETLIST_SetFont
#define GUISETLIST_SetVisible         CTRLSETLIST_SetVisible
#define GUISETLIST_SetCircular        CTRLSETLIST_SetCircular
#define GUISETLIST_SetArrow           CTRLSETLIST_SetArrow
#define GUISETLIST_CallBack           CTRLSETLIST_CallBack
#define GUISETLIST_SetTitleInfo       CTRLSETLIST_SetTitleInfo
#define GUISETLIST_SetTitleTextId     CTRLSETLIST_SetTitleTextId
#define GUISETLIST_SetCtrlState       CTRLSETLIST_SetCtrlState
#define GUISETLIST_SetStrEffect       CTRLSETLIST_SetStrEffect
#define GUISETLIST_SetNeedArrow       CTRLSETLIST_SetNeedArrow
#define GUISETLIST_SetFontAlign       CTRLSETLIST_SetFontAlign
#define GUISETLIST_SetStyleById       CTRLSETLIST_SetStyleById
#define GUISETLIST_ResetAllItemState  CTRLSETLIST_ResetAllItemState
#define GUISETLIST_GetItemState       CTRLSETLIST_GetItemState
#define GUISETLIST_SetItemState       CTRLSETLIST_SetItemState
#define GUISETLIST_SetDispText        CTRLSETLIST_SetDispText
#define GUISETLIST_SetDefaultText     CTRLSETLIST_SetDefaultText
#define GUISETLIST_GetSelectItemIndex CTRLSETLIST_GetSelectItemIndex
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _GUI_SETLIST_H_
 

/*@}*/

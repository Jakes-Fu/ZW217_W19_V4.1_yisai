/*! @file guistatusbar.h
@brief ���ļ�������statusbar�ؼ��Ķ���ӿں���������
@author xiaoqing.lu
@version 1.0
@date 12/23/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 12/23/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_StatusBar ״̬��(StatusBar)

-# @ref sec_statusbar_function
-# @ref sec_statusbar_create
-# @ref sec_statusbar_function_list
-# @ref sec_statusbar_app

@section sec_statusbar_function �ؼ�����

StatusBar��״̬���ؼ���Status bar control��������ƽ̨�и�����ģ���״̬��ʾ���û���ʹ�û��ܹ���Ϊ����������ز鿴�ֻ�������ģ�鵱ǰ��״̬��״̬������ʾ��״̬Ϊ�û���Ϊ���ĵ�ģ���״̬�����������Ƿ������������Ƿ������Ƿ��ж��Ż���š����״̬�ȡ�

״̬���ؼ��������ؼ�������ͬ�����Ǵ��ڵ�����֮һ��ÿ��������ֻ��ͬʱ����һ��״̬���ؼ���

״̬����Ҫ�������£�
-# ״̬����ʾ��ͬģ��ı�ʾ����ʾ����ΪͼƬ�����֡�ͼƬ�����֣�ÿ����ʾ��Ϊһ��Item��
-# ÿ����ʾ�ɸ��������ڴ��ʵ��ʱ��ʾ��Ӧ��ģ�����ݣ�
-# ״̬��������ʾ�ڲ�ͬ�Ĵ����У�
-# ״̬������ʾ���ֻ�ģ���״̬һ����ģ�鵱ǰ��ʵ��״̬��
-# ״̬��״̬���¾��м�ʱ�ԣ�
-# ״̬������ͳһ�����ԣ������д�������ʾ��״̬����״̬������ͬ�ġ���ĳһ�����ڸı�һ��item״̬ʱ������������ʾ��״̬����Ӧ��item״̬��֮�ı䡣
-# ״̬���ؼ���item״̬�ı��������ڱ�ʾ�ı仯���ָ��û��ģ��ı�״̬��APP���ƣ�������㴰�ڰ���״̬���ؼ�����ô��������ʾ���ؼ���ˢ�µ�ʱ����ÿؼ���ˢ�½ӿڼ�ʱ��ʾ��
-# �����ǵ�ƽ̨������У�״̬��ͨ�������λ�ڴ��ڵĶ�����

@section sec_statusbar_create ״̬���Ĵ���

״̬����������ʾƽ̨ģ��״̬�Ŀؼ���ÿ���������ֻ����һ��״̬���ؼ�����ʵ��������Ϊ���ڵ�һ�����Դ����ڴ��ڵ�ʵ���С�

״̬����Ϊ���ڵ����ԣ����������ܼ򵥣���ֻ��Ҫ�ڴ���Table������һ����Ϳ����ˣ����磬�����ھŹ�������˵�������״̬����
@code
WINDOW_TABLE(MAINMENU_ICON_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    WIN_STATUSBAR,    CREATE_MENU_CTRL(MENU_MAINMENU_ICON,MAINMENUWIN_MENU_CTRL_ID),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode

ֻҪ���ڴ��ڵ�Table��������WIN_STATUSBAR���ԣ������������ʾ��ʱ��ͻ���ʾ״̬����

��Ȼ���û����Զ�̬����״̬�������磬idle������һ����̬�����Ĵ��ڣ���ô��ʹ��״̬����ʱ����Open��Ϣʱ������GUIWIN_CreateStbDyn�ӿڣ��ýӿ�֮��Ҫ���ݴ��ڵ�ID���ɡ�

״̬����Ϊһ��GUI�ؼ������й̶������򣨴��ڵ����϶ˣ��͹̶��Ŀؼ�ID������ͨ���ӿڻ�ȡ�ģ��ӿڶ�����mmitheme_statusbar.c�ļ��У�ʵ�����£�
- ��ȡ�ؼ���ID
@code
MMI_CTRL_ID_T MMITHEME_GetStatusBarCtrlId(void)
{
	return MMICOMMON_STATUSBAR_CTRL_ID;
}
@endcode

- ��ȡ�ؼ�����ʾ����
    - ����GUI_BOTH_RECT_T MMITHEME_GetStatusBarBothRect (void)������ȡ��������������������������ø�״̬���ؼ����ؼ������LCD����ת�Ƕ�������ʾ����ȷ��λ�á�

��ʵ�̶���IDҲ������һ���������ֻ����һ��״̬���ؼ�������Ҳ��֤�ˣ���ͬ�����ǿ���ʹ��ͬһ�ؼ�ID�ġ�

@section sec_statusbar_function_list �ؼ��ӿڽ���

״̬�������ʱ��Ϊ���ڵ����ԣ�����ӿڶ�����guiwin.h�У��û������Ҫ�޸�״̬�������ԣ����Ե���guiwin�ӿڡ��ؼ��ӿ����£�
-# @ref GUIWIN_CreateStbDyn
-# @ref GUIWIN_UpdateStb
-# @ref GUIWIN_InitStbData
-# @ref GUIWIN_AppendStbItemData
-# @ref GUIWIN_SetStbItemVisible
-# GUIWIN_StbItemIsVisible
-# @ref GUIWIN_SetStbItemHandleFunc
-# @ref GUIWIN_SetStbItemIcon
-# @ref GUIWIN_SetStbItemIconCurFrame
-# @ref GUIWIN_SetStbItemIconValid
-# @ref GUIWIN_SetStbItemIconStyle
-# @ref GUIWIN_SetStbItemIconId

@section sec_statusbar_app �ؼ�Ӧ��
״̬���ؼ����Դ������κ�һ�����ڣ�������ͼ�������˵���������ʾ״̬�������ڶ�������
@image html statusbar_app_example.jpg
ͼ 2 37 �ؼ���ʾ��ͼ


*******************************************************************************/

/*! @addtogroup statusbarGrp StatusBar
@brief statusbarģ��
@details ״̬���ؼ���ͷ�ļ������ļ������⿪�ţ�����app��Ӧ�õ���guiwin.h�еķ�װ�ӿ�
@{
*******************************************************************************/

#ifndef _GUI_STATUSBAR_H_
#define _GUI_STATUSBAR_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "ctrlstatusbar_export.h"
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
/*                         Function Declare                                                                                                     */
/*----------------------------------------------------------------------------*/
#define GUISTATUSBAR_CreateCtrlDyn CTRLSTATUSBAR_CreateCtrlDyn
#define GUISTATUSBAR_SetBgImageId CTRLSTATUSBAR_SetBgImageId
#define GUISTATUSBAR_SetBg CTRLSTATUSBAR_SetBg
#define GUISTATUSBAR_UpdateCtrl CTRLSTATUSBAR_UpdateCtrl
#define GUISTATUSBAR_SetBgIsDesplay CTRLSTATUSBAR_SetBgIsDesplay
#define GUISTATUSBAR_ClearCustomerBg CTRLSTATUSBAR_ClearCustomerBg
#define GUISTATUSBAR_CustomCreate CTRLSTATUSBAR_CustomCreate
#define GUISTATUSBAR_SetCommonBg CTRLSTATUSBAR_SetCommonBg
#define GUISTATUSBAR_SetState CTRLSTATUSBAR_SetState
#define GUISTATUSBAR_GetState CTRLSTATUSBAR_GetState
#define GUISTATUSBAR_SetItemBg CTRLSTATUSBAR_SetItemBg
#define GUISTATUSBAR_GetCtrlLayer CTRLSTATUSBAR_GetCtrlLayer
#if defined PDA_UI_DROPDOWN_WIN || defined DROPDOWN_NOTIFY_SHORTCUT
#define GUISTATUSBAR_SetDropDownState CTRLSTATUSBAR_SetDropDownState
#define GUISTATUSBAR_IsDropDown CTRLSTATUSBAR_IsDropDown
#define GUISTATUSBAR_SetPressed CTRLSTATUSBAR_SetPressed
#define GUISTATUSBAR_IsPressed CTRLSTATUSBAR_IsPressed
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                                                                                          */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _GUI_STATUSBAR_H_


/*@}*/

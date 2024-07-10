/*! @file guianim.h
@brief ���ļ�������anim�ؼ��Ķ���ӿں��������� 
@author Lianxiang.Zhou
@version 1.0
@date 04/26/2004
@remarks Copyright: 2003 Spreadtrum, Incoporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Anim ����(Anim)

-# @ref sec_anim_function
-# @ref sec_anim_def
-# @ref sec_anim_create
-# @ref sec_anim_function_list
-# @ref sec_anim_app 

@section sec_anim_function �ؼ�����

Anim�ؼ���һ�������ؼ����ܹ���ʾ�������͵�ͼƬ�Ͷ���������ͼƬ�ı���빦�ܡ�

@section sec_anim_def �궨�����
@code
#define CREATE_ANIM_CTRL(_CTRL_ID_,_WIN_ID_) \
        MMK_CREATE_ANIM, _CTRL_ID_,_WIN_ID_
@endcode
- ���ڴ���һ��Anim�ؼ���
    - _CTRL_ID_ �ؼ�ID 
    - _WIN_ID_ �ؼ��������Ĵ���ID����������0����ʱϵͳ��ʹ�� 

@section sec_anim_create �ؼ�����

���벿����Ȼ����sample_anim_ctrl.c�ļ��С�

- ��һ��������һ�����Կؼ��Ĵ��ڣ�����Table����ΪMMI_TEST_ANIM_WIN_TAB��
@code
WINDOW_TABLE( MMI_TEST_ANIM_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_ANIM_WIN_ID ),
WIN_FUNC((uint32) HandleTestAnimWinMsg ),
WIN_STYLE(WS_HAS_ANIM_BG),
    CREATE_ANIM_CTRL(
        MMI_TEST_ANIM_CTRL_ID,
		MMI_TEST_ANIM_WIN_ID),
    END_WIN
};
@endcode
    - �ô��ڵ�Ԫ���д���ID MMI_TEST_ANIM_WIN_ID�����ڴ�����HandleTestAnimWinMsg����������WS_HAS_ANIM_BG�������ڱ�����ͼƬ��full paint��Ϣ�����ȷ������ڣ�Ȼ�󷢸��ؼ���Anim�ؼ�������Anim�ؼ�������Ϊȫ�����ڡ�
- �ڶ��������Ӵ��ڵĴ�������
@code
LOCAL MMI_RESULT_E HandleTestAnimWinMsg(
                    MMI_WIN_ID_T        win_id,
                    MMI_MESSAGE_ID_E    msg_id,
                    DPARAM              param
                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMI_TEST_ANIM_CTRL_ID;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;

        data_info.img_id = HELLO_SPRD_TEST;
        data_info.type = GUIANIM_TYPE_IMAGE_BMP;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_need_resize = TRUE;
	    display_info.is_bg_color = TRUE;
	    display_info.bg_color = MMI_WINDOW_BACKGROUND_COLOR;

        GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
	case MSG_CTL_ANIM_DISPLAY_IND:
        //��ͼƬ�������,������ʾapp���յ�����Ϣ
        break;
    case MSG_CTL_ANIM_GIF_UPDATE:
        //��gif����ˢ��ÿһ֡,app���յ�����Ϣ
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
@endcode
@note Open��Ϣʱ���������˿ؼ�ΪActive�ؼ��������ÿؼ���ʾ��ͼƬ��Ϣ��
- �����������Ӵ������ڵĺ�����ͬʱ��ͷ�ļ�sample_anim_ctrl.h�����Ӻ�������
@code
PUBLIC void MMIHELLO_CreateAnimWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ANIM_WIN_TAB, PNULL);
}
@endcode
- ���Ĳ�����hello sprd������������ڣ��ڽӿ�HandleHelloSprdWinMsg���������´��룩��
@code
case MSG_APP_4:
        MMIHELLO_CreateAnimWin();
        break;
@endcode
    - ��ʾ��sprd�����а�4���򿪶��� ���Դ��ڡ�
- ���岽����������
    - ���ȱ�������ģ��������hello sprd�����а�4�����򿪴������£�
@image html anim_test_win.jpg
ͼ 2 34 Anim�ؼ����Դ���
    - ����û�����ÿؼ�����Ĭ������Ϊ����������Ϊ����table��û��title��״̬����softkey�������ؼ���

@section sec_anim_function_list �ؼ��ӿڽ���

-# @ref GUIANIM_SetParam
-# GUIANIM_SetDisplayPos
-# @ref GUIANIM_SetDisplayRect
-# @ref GUIANIM_SetAutoAdaptDisplay
-# GUIANIM_DestroyDecTask
-# @ref GUIANIM_PlayAnim
-# @ref GUIANIM_PauseAnim
-# @ref GUIANIM_ResumeAnim

@section sec_anim_app  �ؼ�Ӧ��
�����ؼ���������������ʾ��������ȴ����ڵĶ��������˵������ȣ�Ҳ������ʾ����ͼƬ����ǽֽ��ͼƬԤ���ȡ�
*******************************************************************************/

/*! @addtogroup animGrp Anim
@brief animGrp animģ��
@details 
    -#  ��Դ��ͼƬ�ļ���ʾ
    -# ֧�ָ�ʽΪANIM,BMP,WBMP,GIF,JPG,PNG
@{
*******************************************************************************/

#ifndef _GUIANIM_H_
#define _GUIANIM_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
--------------------------------------------------------------------------*/
#include "ctrlanim_export.h"

/*---------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/
#define GUIANIM_SetParam                    CTRLANIM_SetParam             
#define GUIANIM_AdjustDisplayPosition       CTRLANIM_AdjustDisplayPosition
#define GUIANIM_GetImgWidthHeight           CTRLANIM_GetImgWidthHeight    
#define GUIANIM_GetImgWidthHeightEx         CTRLANIM_GetImgWidthHeightEx  
#define GUIANIM_GetDisplayPoint             CTRLANIM_GetDisplayPoint      
#define GUIANIM_GetZoomWidthRange           CTRLANIM_GetZoomWidthRange    
#define GUIANIM_GetZoomClipRect             CTRLANIM_GetZoomClipRect      
#define GUIANIM_Zoom                        CTRLANIM_Zoom                 
#define GUIANIM_ZoomMove                    CTRLANIM_ZoomMove             
#define GUIANIM_ZoomUtmost                  CTRLANIM_ZoomUtmost           
#define GUIANIM_SetZoomRatio                CTRLANIM_SetZoomRatio         
#define GUIANIM_Rotate                      CTRLANIM_Rotate               
#define GUIANIM_RotateArbitrary             CTRLANIM_RotateArbitrary      
#define GUIANIM_GetAnimType                 CTRLANIM_GetAnimType          
#define GUIANIM_PlayAnim                    CTRLANIM_PlayAnim             
#define GUIANIM_PlayAnimByPtr               CTRLANIM_PlayAnimByPtr        
#define GUIANIM_PauseAnim                   CTRLANIM_PauseAnim            
#define GUIANIM_ResumeAnim                  CTRLANIM_ResumeAnim           
#define GUIANIM_SetCtrlRect                 CTRLANIM_SetCtrlRect          
#define GUIANIM_SetCtrlRectByPtr            CTRLANIM_SetCtrlRectByPtr     
#define GUIANIM_GetDisplayRect              CTRLANIM_GetDisplayRect       
#define GUIANIM_SetDisplayRect              CTRLANIM_SetDisplayRect       
#define GUIANIM_SetDisplayRectByPtr         CTRLANIM_SetDisplayRectByPtr  
#define GUIANIM_SetBgImg                    CTRLANIM_SetBgImg             
#define GUIANIM_SetBgImgByPtr               CTRLANIM_SetBgImgByPtr        
#define GUIANIM_SetBgColor                  CTRLANIM_SetBgColor           
#define GUIANIM_SetBgColorByPtr             CTRLANIM_SetBgColorByPtr      
#define GUIANIM_SetBorder                   CTRLANIM_SetBorder            
#define GUIANIM_SetFrame                    CTRLANIM_SetFrame             
#define GUIANIM_SetDefaultIcon              CTRLANIM_SetDefaultIcon       
#define GUIANIM_SetDefaultIconByPtr         CTRLANIM_SetDefaultIconByPtr  
#define GUIANIM_SetErrorBg                  CTRLANIM_SetErrorBg           
#define GUIANIM_SetAutoAdaptDisplay         CTRLANIM_SetAutoAdaptDisplay  
#define GUIANIM_SetPeriod                   CTRLANIM_SetPeriod            
#define GUIANIM_UpdateDisplay               CTRLANIM_UpdateDisplay        
#define GUIANIM_UpdateRect                  CTRLANIM_UpdateRect           
#define GUIANIM_UpdateRectByPtr             CTRLANIM_UpdateRectByPtr      
#define GUIANIM_DeleteMiniatureFile         CTRLANIM_DeleteMiniatureFile  
#define GUIANIM_IsSupport                   CTRLANIM_IsSupport            
#define GUIANIM_SetVisible                  CTRLANIM_SetVisible           
#define GUIANIM_SetOwnerDraw                CTRLANIM_SetOwnerDraw         
#define GUIANIM_SetOwnerDrawEx              CTRLANIM_SetOwnerDrawEx       
#define GUIANIM_GetRotateSize               CTRLANIM_GetRotateSize        
#define GUIANIM_GetAnimDisplayBitmap        CTRLANIM_GetAnimDisplayBitmap 
#define GUIANIM_IsTpEnable                  CTRLANIM_IsTpEnable
/*--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

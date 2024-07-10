/*! @file guianim.h
@brief 此文件介绍了anim控件的对外接口和数据类型 
@author Lianxiang.Zhou
@version 1.0
@date 04/26/2004
@remarks Copyright: 2003 Spreadtrum, Incoporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Anim 动画(Anim)

-# @ref sec_anim_function
-# @ref sec_anim_def
-# @ref sec_anim_create
-# @ref sec_anim_function_list
-# @ref sec_anim_app 

@section sec_anim_function 控件功能

Anim控件是一个动画控件，能够显示多种类型的图片和动画。具有图片的编解码功能。

@section sec_anim_def 宏定义解析
@code
#define CREATE_ANIM_CTRL(_CTRL_ID_,_WIN_ID_) \
        MMK_CREATE_ANIM, _CTRL_ID_,_WIN_ID_
@endcode
- 用于创建一个Anim控件。
    - _CTRL_ID_ 控件ID 
    - _WIN_ID_ 控件所依赖的窗口ID，可以填入0，暂时系统不使用 

@section sec_anim_create 控件创建

代码部分仍然放在sample_anim_ctrl.c文件中。

- 第一步：创建一个测试控件的窗口，窗口Table名称为MMI_TEST_ANIM_WIN_TAB：
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
    - 该窗口的元素有窗口ID MMI_TEST_ANIM_WIN_ID，窗口处理函数HandleTestAnimWinMsg，窗口属性WS_HAS_ANIM_BG，即窗口背景是图片，full paint消息会优先发给窗口，然后发给控件，Anim控件。其中Anim控件的区域为全屏窗口。
- 第二步：增加窗口的处理函数：
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
        //当图片解码完成,可以显示app会收到此消息
        break;
    case MSG_CTL_ANIM_GIF_UPDATE:
        //当gif动画刷新每一帧,app会收到此消息
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
@note Open消息时我们设置了控件为Active控件，并设置控件显示的图片信息。
- 第三步：增加创建窗口的函数，同时在头文件sample_anim_ctrl.h中增加函数定义
@code
PUBLIC void MMIHELLO_CreateAnimWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ANIM_WIN_TAB, PNULL);
}
@endcode
- 第四步：在hello sprd窗口中增加入口（在接口HandleHelloSprdWinMsg中增加如下代码）：
@code
case MSG_APP_4:
        MMIHELLO_CreateAnimWin();
        break;
@endcode
    - 表示在sprd窗口中按4键打开动画 测试窗口。
- 第五步：编译运行
    - 首先编译运行模拟器，在hello sprd窗口中按4键，打开窗口如下：
@image html anim_test_win.jpg
图 2 34 Anim控件测试窗口
    - 这里没有设置控件区域，默认区域为窗口区域，因为窗口table中没有title、状态栏、softkey等其他控件。

@section sec_anim_function_list 关键接口介绍

-# @ref GUIANIM_SetParam
-# GUIANIM_SetDisplayPos
-# @ref GUIANIM_SetDisplayRect
-# @ref GUIANIM_SetAutoAdaptDisplay
-# GUIANIM_DestroyDecTask
-# @ref GUIANIM_PlayAnim
-# @ref GUIANIM_PauseAnim
-# @ref GUIANIM_ResumeAnim

@section sec_anim_app  控件应用
动画控件可以用来创建显示动画，如等待窗口的动画，主菜单动画等，也可以显示单张图片，如墙纸，图片预览等。
*******************************************************************************/

/*! @addtogroup animGrp Anim
@brief animGrp anim模块
@details 
    -#  资源和图片文件显示
    -# 支持格式为ANIM,BMP,WBMP,GIF,JPG,PNG
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

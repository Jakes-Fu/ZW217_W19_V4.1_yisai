/*! @file guictrl_api.h
@brief 此文件介绍了抽象控件的对外接口和数据类型
@author Jassmine
@version 1.0
@date 04/26/2004    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Ctrl_api Ctrl_api

(暂无)

*******************************************************************************/

/*! @addtogroup ctrl_apiGrp Ctrl_api
@brief ctrl_api模块
@details 文件介绍了抽象控件的对外接口
@{
*******************************************************************************/

#ifndef _GUICTRL_API_H_
#define _GUICTRL_API_H_ 
/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 设置控件的状态
@author James.Zhang
@param ctrl_handle [in] 控件句柄
@param state [in] 控件状态类型
@param is_true [in] TRUE:设置状态;FALSE:清除状态
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetState( 
                                MMI_HANDLE_T    ctrl_handle,
                                GUICTRL_STATE_T state,
                                BOOLEAN is_true
                                );


/***************************************************************************//*!
@brief 获取控件的状态
@author James.Zhang
@param ctrl_handle [in] 控件句柄
@param state [in] 控件状态类型
@return TRUE:获取状态成功;FALSE:获取状态失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetState( 
                                   MMI_HANDLE_T    ctrl_handle,
                                   GUICTRL_STATE_T state
                                   );



/***************************************************************************//*!
@brief 设置控件的矩形
@author James.Zhang
@param ctrl_handle [in] 控件句柄
@param rect_ptr [in] 控件矩形的指针
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetRect( 
                               MMI_HANDLE_T      ctrl_handle,
                               const GUI_RECT_T* rect_ptr
                               );



/***************************************************************************//*!
@brief 获取控件的矩形
@author James.Zhang
@param ctrl_handle [in] 控件句柄
@param rect_ptr [in/out] 控件矩形的指针
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_GetRect( 
                               MMI_HANDLE_T ctrl_handle,
                               GUI_RECT_T*  rect_ptr
                               );



/***************************************************************************//*!
@brief 设置控件的LCD 显示设备
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param lcd_dev_ptr [in] 显示设备的指针
@note 
*******************************************************************************/

PUBLIC void GUIAPICTRL_SetLcdDevInfo(
                                     MMI_HANDLE_T           ctrl_handle,    //in:
                                     const GUI_LCD_DEV_INFO *lcd_dev_ptr    //in:
                                     );


/***************************************************************************//*!
@brief 获取控件的LCD 显示设备
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param lcd_dev_ptr [in/out] 显示设备的指针
@return TRUE:获取显示设备成功;FALSE:获取显示设备失败
@note
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetLcdDevInfo(
                                        MMI_HANDLE_T        ctrl_handle,    //in:
                                        GUI_LCD_DEV_INFO    *lcd_dev_ptr    //in/out:
                                        );



/***************************************************************************//*!
@brief 获取控件显示区域的矩形
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param rect_ptr [in/out] 控件显示区域矩形的指针
@return TRUE:获取显示区域成功;FALSE:获取显示区域失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetDisplayRect(
                                         MMI_HANDLE_T   ctrl_handle,    //in:
                                         GUI_RECT_T     *rect_ptr       //in/out:
                                         );



/***************************************************************************//*!
@brief 设置控件显示区域的矩形
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param rect_ptr [in] 控件显示区域矩形的指针
@param is_update [in] 是否立即更新
@return TRUE:设置显示区域成功;FALSE:设置显示区域失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetDisplayRect(
                                         MMI_HANDLE_T       ctrl_handle,//in:
                                         const GUI_RECT_T   *rect_ptr,  //in/out:
                                         BOOLEAN            is_update   //in:
                                         );



/***************************************************************************//*!
@brief 获取控件垂直的矩形和水平的矩形
@author James.Zhang
@param ctrl_handle [in] 控件句柄
@param both_rect_ptr [in/out] 控件垂直矩形和水平矩形的指针
@return TRUE:获取矩形成功;FALSE:获取矩形失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_GetBothRect(
                                      MMI_HANDLE_T     ctrl_handle,
                                      GUI_BOTH_RECT_T* both_rect_ptr
                                      );



/***************************************************************************//*!
@brief 设置控件垂直的矩形和水平的矩形
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param both_rect_ptr [in] 控件垂直矩形和水平矩形的指针
@return TRUE:设置矩形成功;FALSE:设置矩形失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetBothRect(
                                      MMI_HANDLE_T      ctrl_handle,
                                      GUI_BOTH_RECT_T   *both_rect_ptr
                                      );



/***************************************************************************//*!
@brief 设置控件背景信息
@author Jassmine
@param ctrl_handle [in] 控件句柄
@param bg_ptr [in] 控件背景信息的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_SetBg(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_BG_T       *bg_ptr
                                );

/*****************************************************************************/
// 	Description : Get control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_HandleEvent(
                                      MMI_HANDLE_T     ctrl_handle,
                                      CAF_MESSAGE_ID_E msg_id, 
                                      void* param_ptr
                                      );

/*****************************************************************************/
//  Description : set control state
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetBorder( 
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_BORDER_T *border_ptr
                                ) ;

/*****************************************************************************/
// 	Description : Redraw ctrl [ send MSG_CTL_PAINT to Ctrl]
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_Redraw(
                                 MMI_HANDLE_T     ctrl_handle
                                 );

/***************************************************************************//*!
@brief 设置多个控件隐藏特效
@author James.Zhang
@param win_handle [in] 窗口句柄
@param ctrl_handle_ptr [in] 控件句柄的指针
@param ctrl_num [in] 控件数目
@param is_hide [in] 是否隐藏控件
@return TRUE:设置成功;FALSE:设置失败
@note 多个控件隐藏特效
*******************************************************************************/

PUBLIC BOOLEAN GUIAPICTRL_CtrlsHidle(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_HANDLE_T*   ctrl_handle_ptr,
                                     uint16          ctrl_num,
                                     BOOLEAN         is_hide
                                     );
/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

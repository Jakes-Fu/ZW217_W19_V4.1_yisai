/*! @file ctrlinfo_export.h
@brief 此文件介绍了抽象控件的对外接口和数据类型
@author Jassmine
@version 1.0
@date 04/26/2004  
@remarks Copyright: 2003 Spreadtrum, Incoporated. All Rights Reserved.
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Ctrl Ctrl

(暂无)

*******************************************************************************/

/*! @addtogroup ctrlGrp Ctrl
@brief 文件介绍了抽象控件的对外接口
@details ctrl模块
@{
*******************************************************************************/

#ifndef _CTRLINFO_EXPORT_H_
#define _CTRLINFO_EXPORT_H_ 

/*-------------------------------------------------------------------------
                         Include Files                                    
 --------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "mmk_type.h"

/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                          MACRO DEFINITION                                
 --------------------------------------------------------------------------*/

//临时定义, 仅仅保证编译通过, 将来需要都删除
//MMI_RESULT_E
/*! @def GUI_RESULT_DEFAULT
@brief 默认
*/
#define GUI_RESULT_DEFAULT        MMI_RESULT_TRUE
/*! @def GUI_RESULT_PENOK
@brief 左键OK
*/
#define GUI_RESULT_PENOK          (MMI_RESULT_TRUE+1)
/*! @def GUI_RESULT_CTLOK
@brief 选择键
*/
#define GUI_RESULT_CTLOK          (MMI_RESULT_TRUE+2)
/*! @def GUI_RESULT_CTLMDL
@brief 中间键
*/
#define GUI_RESULT_CTLMDL         (MMI_RESULT_TRUE+3)
/*! @def GUI_RESULT_CTLCANCEL
@brief 右键取消
*/
#define GUI_RESULT_CTLCANCEL      (MMI_RESULT_TRUE+4)

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                 
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief 获取控件接口
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param guid [in] 控件id
@param object_pptr [in/out] 获取控件接口的结果
@return CAF_RESULT_SUCCEEDED:获取成功。其余对应错误代号。
@note 
*******************************************************************************/
PUBLIC CAF_RESULT_E VTLCTRL_QueryInterface(
                                           IGUICTRL_T   *iguictrl_ptr,
                                           CAF_GUID_T   guid,
                                           void         **object_pptr
                                           );


/***************************************************************************//*!
@brief 控件的引用计数值加1
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@return 控件的引用计数值
@note 
*******************************************************************************/
PUBLIC uint32 VTLCTRL_AddRef(
                             IGUICTRL_T     *iguictrl_ptr
                             );



/***************************************************************************//*!
@brief 控件的引用计数值减1
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@return 控件的引用计数值
@note 
*******************************************************************************/
PUBLIC uint32 VTLCTRL_Release(
                              IGUICTRL_T    *iguictrl_ptr
                              );


/***************************************************************************//*!
@brief 获取控件id
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@retval 
@return 
@note 
*******************************************************************************/
PUBLIC CAF_GUID_T VTLCTRL_GetCtrlGuid(
                                      IGUICTRL_T    *iguictrl_ptr
                                      );


/***************************************************************************//*!
@brief 设置控件句柄
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param ctrl_handle [in] 控件句柄
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetCtrlHandle(
                                  IGUICTRL_T    *iguictrl_ptr,
                                  CAF_HANDLE_T  ctrl_handle
                                  );


/***************************************************************************//*!
@brief 获得控件句柄
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@return 控件句柄
@note 
*******************************************************************************/
PUBLIC CAF_HANDLE_T VTLCTRL_GetCtrlHandle(
                                          IGUICTRL_T    *iguictrl_ptr
                                          );


/***************************************************************************//*!
@brief 设置控件的状态
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param state [in] 控件状态类型
@param is_true [in] TRUE:设置状态;FALSE:清除状态
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetState(
                             IGUICTRL_T         *iguictrl_ptr,
                             GUICTRL_STATE_T    state,
                             BOOLEAN            is_true
                             );


/***************************************************************************//*!
@brief 获取控件的状态
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param state [in] 控件状态类型
@return TRUE:获取状态成功;FALSE:获取状态失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetState(
                                IGUICTRL_T          *iguictrl_ptr,
                                GUICTRL_STATE_T     state
                                );


/***************************************************************************//*!
@brief  设置控件的矩形区域
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param rect_ptr [in] 矩形区域的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetRect(
                               IGUICTRL_T           *iguictrl_ptr,
                               const GUI_RECT_T     *rect_ptr
                               );


/***************************************************************************//*!
@brief 获取控件的矩形区域
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param rect_ptr [in/out] 矩形区域的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetRect(
                               IGUICTRL_T   *iguictrl_ptr,
                               GUI_RECT_T   *rect_ptr
                               );


/***************************************************************************//*!
@brief 设置控件的lcd显示设备
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param lcd_dev_ptr [in] 显示设备的指针
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_SetLcdDevInfo(
                                  IGUICTRL_T                *iguictrl_ptr,
                                  const GUI_LCD_DEV_INFO    *lcd_dev_ptr
                                  );


/***************************************************************************//*!
@brief  获取控件的lcd显示设备
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param lcd_dev_ptr [in/out] 显示设备的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetLcdDevInfo(
                                     IGUICTRL_T         *iguictrl_ptr,
                                     GUI_LCD_DEV_INFO   *lcd_dev_ptr
                                     );


/***************************************************************************//*!
@brief 设置控件的属性
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param properties [in] 控件的属性类型
@param is_true [in] TRUE:设置属性;FALSE:清除属性
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProperties(
                                     IGUICTRL_T                 *iguictrl_ptr,
                                     CAF_GUICTRL_PROPERTIES_T   properties,
                                     BOOLEAN                    is_true
                                     );


/***************************************************************************//*!
@brief 获取控件的属性
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@return 控件的属性类型
@note 
*******************************************************************************/
PUBLIC CAF_GUICTRL_PROPERTIES_T VTLCTRL_GetProperties(
                                                      IGUICTRL_T    *iguictrl_ptr
                                                      );


/***************************************************************************//*!
@brief  查询控件是否包括某属性
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param properties [in] 属性类型
@return TRUE:控件包括此属性;FALSE:控件不包括此属性
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_TestProperties(
                                      IGUICTRL_T                *iguictrl_ptr,
                                      CAF_GUICTRL_PROPERTIES_T  properties
                                      );


/***************************************************************************//*!
@brief 重绘控件
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@note 
*******************************************************************************/
PUBLIC void VTLCTRL_Redraw(
                           IGUICTRL_T   *iguictrl_ptr
                           );



/***************************************************************************//*!
@brief 设置控件active
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param is_true [in] TRUE:设置控件active;FALSE:设置控件inactive
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetActive(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 BOOLEAN        is_true
                                 );



/***************************************************************************//*!
@brief 查询控件是否active
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@return [in] TRUE:控件active;FALSE:控件inactive
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_IsActive(
                                IGUICTRL_T  *iguictrl_ptr
                                );


/***************************************************************************//*!
@brief 设置控件的边框信息
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param border_ptr [in] 边框信息的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBorder(
                                 IGUICTRL_T     *iguictrl_ptr,
                                 GUI_BORDER_T   *border_ptr
                                 );



/***************************************************************************//*!
@brief 设置控件的背景信息
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param bg_ptr [in] 背景信息的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBg(
                             IGUICTRL_T     *iguictrl_ptr,
                             GUI_BG_T       *bg_ptr
                             );



/***************************************************************************//*!
@brief 设置控件的字体信息
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param font_ptr [in] 字体信息的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               );




/***************************************************************************//*!
@brief 获取控件的字体信息
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param font_ptr [in/out] 字体信息的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetFont(
                               IGUICTRL_T       *iguictrl_ptr,
                               GUI_FONT_ALL_T   *font_ptr
                               );


/***************************************************************************//*!
@brief 设置是否设置滚动条
@author Jassmine
@param [in] iguictrl_ptr 控件接口的指针
@param [in] is_display TRUE:显示;FALSE:不显示
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetProgress(
                                   IGUICTRL_T   *iguictrl_ptr,
                                   BOOLEAN      is_display
                                   );



/***************************************************************************//*!
@brief 设置是否上下键循环
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param is_circular [in] TRUE:上下键循环;FALSE:上下键不循环
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleUpDown(
                                               IGUICTRL_T   *iguictrl_ptr,
                                               BOOLEAN      is_circular
                                               );



/***************************************************************************//*!
@brief 设置是否左右键循环
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param is_circular [in] TRUE:左右键循环;FALSE:左右键键不循环
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetCircularHandleLeftRight(
                                                  IGUICTRL_T    *iguictrl_ptr,
                                                  BOOLEAN       is_circular
                                                  );



/***************************************************************************//*!
@brief 根据控件宽度获取控件高度
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param width [in] 控件宽度
@param height_ptr [in/out] 控件高度的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetHeightByWidth(
                                        IGUICTRL_T  *iguictrl_ptr,  //in:
                                        uint16      width,          //in:
                                        uint16      *height_ptr     //in/out:
                                        );



/***************************************************************************//*!
@brief 获取控件的矩形显示区域
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param rect_ptr [in/out] 矩形的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetDisplayRect(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      GUI_RECT_T    *rect_ptr       //in/out:
                                      );


/***************************************************************************//*!
@brief 设置控件的矩形显示区域
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param rect_ptr [in] 矩形的指针
@param is_update [in] 是否立即更新
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetDisplayRect(
                                      IGUICTRL_T        *iguictrl_ptr,  //in:
                                      const GUI_RECT_T  *rect_ptr,      //in:
                                      BOOLEAN           is_update       //in:
                                      );


/***************************************************************************//*!
@brief  设置控件选择区域
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param rect_ptr [in] 矩形的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectRect(
                                     IGUICTRL_T     *iguictrl_ptr,  //in:
                                     GUI_RECT_T     *rect_ptr       //in/out:
                                     );



/***************************************************************************//*!
@brief 获取控件选择项的索引值
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param is_first_ptr [in] 是否是头指针
@param is_last_ptr [in] 是否是尾指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       *is_first_ptr,  //in/out:
                                      BOOLEAN       *is_last_ptr    //in/out:
                                      );

/***************************************************************************//*!
@brief 设置控件选择项的索引值
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param is_first [in] 是否是头
@param is_last  [in] 是否是尾
@return TRUE:成功;FALSE:失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetSelectIndex(
                                      IGUICTRL_T    *iguictrl_ptr,  //in:
                                      BOOLEAN       is_first,       //in:
                                      BOOLEAN       is_last         //in:
                                      );


/***************************************************************************//*!
@brief 获取水平和垂直的矩形
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param both_rect_ptr [in] 矩形的指针
@return TRUE:获取成功;FALSE:获取失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_GetBothRect(
                                   IGUICTRL_T       *iguictrl_ptr,  //in:
                                   GUI_BOTH_RECT_T  *both_rect_ptr  //in/out:
                                   );


/***************************************************************************//*!
@brief 设置水平和垂直的矩形
@author Jassmine
@param iguictrl_ptr [in] 控件接口的指针
@param both_rect_ptr [in/out] 矩形的指针
@return TRUE:设置成功;FALSE:设置失败
@note 
*******************************************************************************/
PUBLIC BOOLEAN VTLCTRL_SetBothRect(
                                   IGUICTRL_T               *iguictrl_ptr,  //in:
                                   const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
                                   );



/***************************************************************************//*!
@brief 发送消息给父控件或者窗口
@author Jassmine
@param ctrl_handle [in] 控件接口的句柄
@param code [in] 消息id
@return MMI_RESULT_TRUE:成功MMI_RESULT_FALSE:失败
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotify(
                                       MMI_HANDLE_T     ctrl_handle,
                                       MMI_MESSAGE_ID_E code
                                       );


/***************************************************************************//*!
@brief 发送消息给父控件或者窗口，同时传递参数
@author Jassmine
@param ctrl_handle [in] 控件接口的指针
@param code [in] 消息id
@param notify_ptr [in] 参数的指针
@return MMI_RESULT_TRUE:成功MMI_RESULT_FALSE:失败
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_SendNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr
                                         );


/***************************************************************************//*!
@brief 传递消息给父控件或者窗口
@author Jassmine
@param ctrl_handle [in] 控件接口的指针
@param code [in] 消息id
@return MMI_RESULT_TRUE:成功MMI_RESULT_FALSE:失败
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotify(
                                       MMI_HANDLE_T         ctrl_handle,
                                       MMI_MESSAGE_ID_E     code
                                       );


/***************************************************************************//*!
@brief 传递消息给父控件或者窗口，同时传递参数
@author Jassmine
@param ctrl_handle [in] 控件接口的指针
@param code [in] 消息id
@param notify_ptr [in] 参数的指针
@param size_of_notify [in] 参数的大小
@return MMI_RESULT_TRUE:成功MMI_RESULT_FALSE:失败
@note 
*******************************************************************************/
PUBLIC MMI_RESULT_E CTRLMSG_PostNotifyEx(
                                         MMI_HANDLE_T       ctrl_handle,
                                         MMI_MESSAGE_ID_E   code,
                                         DPARAM             notify_ptr, 
                                         uint32             size_of_notify
                                         );

/*--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

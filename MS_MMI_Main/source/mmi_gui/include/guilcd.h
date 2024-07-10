/*! @file guilcd.h
@brief 此文件封装了LCD相关的方法
@author Jassmine
@version 1.0
@date 06/11/2007    
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Lcd Lcd

(暂无)

*******************************************************************************/

/*! @addtogroup lcdGrp Lcd
@brief lcd模块
@details
封装LCD相关的方法
- 获取LCD的相关信息，如屏幕宽高等
- 刷新屏幕
- 与屏幕旋转相关的信息\n
横屏还是竖屏是以屏幕的物理角度为基准的，竖屏的角度为0度和180度，默认竖屏的高度大
于宽度，同样地，如果角度为90度或270度时，就认为是横屏，横屏的宽度大于高度。例如，
如果屏幕的初始角度为0或180，平台会认为其为竖屏。
@{
*******************************************************************************/


#ifndef _GUILCD_H_
#define _GUILCD_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "dal_img.h"
#include "mmk_type.h"
#include "guigraph.h"
#include "dal_display.h"

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
/*! @def GUI_LCD_ID_E
@brief LCD ID
*/
#define GUI_LCD_ID_E        LCD_ID_E

/*! @def GUI_LCD_0
@brief 0号LCD
*/
#define GUI_LCD_0           LCD_ID_0
/*! @def GUI_MAIN_LCD_ID
@brief 主屏，定义在0号屏上
*/
#define GUI_MAIN_LCD_ID     LCD_ID_0
/*! @def GUI_LCD_1
@brief 1号屏
*/
#define GUI_LCD_1           LCD_ID_1
/*! @def GUI_SUB_LCD_ID
@brief 子屏，定义在1号屏上
*/
#define GUI_SUB_LCD_ID      LCD_ID_1
/*! @def GUI_LCD_2
@brief 2号屏
*/
#define GUI_LCD_2           LCD_ID_2
/*! @def GUI_LCD_3
@brief 3号屏
*/
#define GUI_LCD_3           LCD_ID_3
/*! @def GUI_LCD_4
@brief 4号屏
*/
#define GUI_LCD_4           LCD_ID_4
/*! @def GUI_LCD_5
@brief 5号屏
*/
#define GUI_LCD_5           LCD_ID_5
/*! @def GUI_LCD_6
@brief 6号屏
*/
#define GUI_LCD_6           LCD_ID_6
/*! @def GUI_LCD_7
@brief 7号屏
*/
#define GUI_LCD_7           LCD_ID_7
/*! @def GUI_TV_ID
@brief TV输出屏
*/
#define GUI_TV_ID           TV_ID
/*! @def GUI_LCD_MAX
@brief LCD个数
*/
#define GUI_LCD_MAX         MAX_LCD_ID
        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @struct GUILCD_COORDINATE_PARAM_T
@brief 坐标参数信息
*/
typedef struct
{
    GUI_RECT_T  rect;           /*!< 坐标区域 */
    LCD_ANGLE_E angle;          /*!< 角度 */
    uint16      lcd_width;      /*!< 屏宽 *///logical or physical screen width 
    uint16      lcd_height;     /*!< 屏高 *///logical or physical screen height 
}GUILCD_COORDINATE_PARAM_T;

//刷屏模式
typedef enum
{
    GUILCD_INVALIDATE_MODE_NORMAL,  //正常刷屏
    GUILCD_INVALIDATE_MODE_OSD,     //OSD刷屏，此模式下，主层直接传给底层跟图像层合成，可以减少一个buffer

    GUILCD_INVALIDATE_MODE_MAX
}GUILCD_INVALIDATE_MODE_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief LCD初始化
@author Jassmine
@param lcd_id [in] LCD ID
@return 是否调用成功
@note 
现在只能初始化主屏和子屏，即\link #GUI_MAIN_LCD_ID \endlink和\link #GUI_SUB_LCD_ID \endlink
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_Init(
                           LCD_ID_E     lcd_id
                           );

/***************************************************************************//*!
@brief 获取LCD信息
@author Jassmine
@param lcd_id [in] LCD ID
@param lcd_info_ptr [in/out] 指向LCD信息的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetInfo(
                              LCD_ID_E      lcd_id,
                              LCD_INFO_T    *lcd_info_ptr
                              );

/***************************************************************************//*!
@brief 获取主屏的buffer地址
@author Jassmine
@return 主屏的buffer地址
@note 
该方法获取的是LCD buffer
*******************************************************************************/
PUBLIC GUI_COLOR_T* GUILCD_GetMainLcdBufPtr(void);

/*****************************************************************************/
// Description : set brush mode
// Global resource dependence : 
// Author: Jassmine
// Note:
/*****************************************************************************/
//PUBLIC BOOLEAN GUILCD_SetBrushMode(
//                                   LCD_ID_E             lcd_id,
//                                   LCD_BRUSH_MODE_E     mode
 //                                  );

/***************************************************************************//*!
@brief 获取指定LCD的初始角度
@author Jassmine
@param lcd_id [in] LCD ID
@return LCD的初始角度
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetInitAngle(
                                       LCD_ID_E     lcd_id
                                       );


/***************************************************************************//*!
@brief 获取指定LCD的当前角度
@author Jassmine
@param lcd_id [in] LCD ID
@return LCD的当前角度
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E  GUILCD_GetLogicAngle(
                                         LCD_ID_E   lcd_id
                                         );

/***************************************************************************//*!
@brief 设置指定LCD的当前角度
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_angle [in] LCD的当前角度
@return 
@note 
*******************************************************************************/
PUBLIC void GUILCD_SetLogicAngle(
                                 LCD_ID_E       lcd_id,
                                 LCD_ANGLE_E    logic_angle
                                 );

/***************************************************************************//*!
@brief 刷新指定区域到屏幕上
@author Jassmine
@param lcd_id [in] LCD ID
@param rect [in] 需要刷新的区域
@param update_block_set [in] 需要更新的BLOCK集，用于TVOUT
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRect(
                                     LCD_ID_E       lcd_id,             //in:
                                     GUI_RECT_T     rect,               //in:
                                     uint32         update_block_set    //in:for tv out
                                     );


/***************************************************************************//*!
@brief 刷新整个屏幕
@author Jassmine
@param lcd_id [in] LCD ID
@param update_block_set [in] 需要更新的BLOCK集，用于TVOUT
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_Invalidate(
                                 LCD_ID_E   lcd_id,             //in:
                                 uint32     update_block_set    //in:for tv out
                                 );

/***************************************************************************//*!
@brief 刷新指定区域到屏幕上，不处理多层
@author Jassmine
@param lcd_id [in] LCD ID
@param rect [in] 需要刷新的区域
@param update_block_set [in] 需要更新的BLOCK集，用于TVOUT
@return 是否调用成功
@note 
该方法与\link GUILCD_InvalidateRect() \endlink的区别在于，该方法不处理多层
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_InvalidateRectOnly(
                                         LCD_ID_E       lcd_id,             //in:
                                         GUI_RECT_T     rect,               //in:
                                         uint32         update_block_set    //in:for tv out
                                         );

/***************************************************************************//*!
@brief 获取当前逻辑屏的宽高
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_width_ptr [in/out] 指向逻辑屏宽度的指针
@param logic_height_ptr [in/out] 指向逻辑屏高度的指针
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetLogicWidthHeight(
                                          LCD_ID_E      lcd_id,             //in:
                                          uint16        *logic_width_ptr,   //in/out:may PNULL
                                          uint16        *logic_height_ptr   //in/out:may PNULL
                                          );


/***************************************************************************//*!
@brief 根据旋转角度获取屏幕宽高
@author Jassmine
@param lcd_id [in] LCD ID
@param lcd_angle [in] 旋转角度
@param logic_width_ptr [in/out] 指向宽度的指针
@param logic_height_ptr [in/out] 指向高度的指针
@return 
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_GetWidthHeightByAngle(
                                            LCD_ID_E    lcd_id,             //in:
                                            LCD_ANGLE_E lcd_angle,          //in:
                                            uint16      *logic_width_ptr,   //in/out:may PNULL
                                            uint16      *logic_height_ptr   //in/out:may PNULL
                                            );

/***************************************************************************//*!
@brief 屏幕是否处于横屏状态
@author Jassmine
@param lcd_id [in] LCD ID
@return 屏幕是否处于横屏状态
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_IsLandscape(
                                  LCD_ID_E  lcd_id
                                  );

/***************************************************************************//*!
@brief 将逻辑角度和区域转换为物理角度和区域
@author Jassmine
@param lcd_id [in] LCD ID
@param logic_rect_ptr [in] 逻辑区域
@param logic_angle_ptr [in] 逻辑角度
@param physical_rect_ptr [in/out] 物理区域
@param physical_angle_ptr [in/out] 物理角度
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertRectAndAngle(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUI_RECT_T    *logic_rect_ptr,    //in:
                                          LCD_ANGLE_E   *logic_angle_ptr,   //in:
                                          GUI_RECT_T    *physical_rect_ptr, //in/out:may PNULL
                                          LCD_ANGLE_E   *physical_angle_ptr //in/out:may PNULL
                                          );

/***************************************************************************//*!
@brief 根据角度转换按键
@author Jassmine
@param key_code_ptr [in/out] 输入原按键值，并输出转换后的按键值
@param lcd_id [in] LCD ID
@param logic_angle [in] 角度
@return 
@note 
该方向现在只用于转换方向键，其他按键并不需要转换
*******************************************************************************/
PUBLIC void GUILCD_ConvertKeyCode(
                                  uint32        *key_code_ptr,  //in/out:
                                  LCD_ID_E      lcd_id,         //in:
                                  LCD_ANGLE_E   logic_angle     //in:current angle
                                  );

/***************************************************************************//*!
@brief 获取屏幕旋转的角度
@author Jassmine
@param lcd_id [in] LCD ID
@return 屏幕相对于初始时的旋转角度
@note 
*******************************************************************************/
PUBLIC LCD_ANGLE_E GUILCD_GetRotateAngle(
                                         LCD_ID_E   lcd_id  //in:
                                         );

/***************************************************************************//*!
@brief 根据当前旋转角度，转换屏幕坐标点
@author Jassmine
@param x_ptr [in/out] 输入原屏幕x坐标点，输出转换后的x坐标点
@param y_ptr [in/out] 输入原屏幕y坐标点，输出转换后的y坐标点
@param lcd_id [in] LCD ID
@return 
@note 
*******************************************************************************/
PUBLIC void GUILCD_ConvertTpPoint(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  );
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
/***************************************************************************//*!
@brief 根据当前旋转角度，转换屏幕坐标点
@author zhenpeng.yao
@param x_ptr [in/out] 输入原屏幕x坐标点，输出转换后的x坐标点
@param y_ptr [in/out] 输入原屏幕y坐标点，输出转换后的y坐标点
@param lcd_id [in] LCD ID
@return 
@note support left and right wear,we can get the display angle from LCD_GetWearType
*******************************************************************************/
PUBLIC void GUILCD_ConvertTpPointEx(
                                  uint32    *x_ptr, //in/out:
                                  uint32    *y_ptr, //in/out:
                                  LCD_ID_E  lcd_id  //in:
                                  );
#endif
                                  

/***************************************************************************//*!
@brief 根据当前旋转角度，转换坐标信息
@author robert.wang
@param lcd_id [in] LCD ID
@param logic_coordinate_ptr [in] 指向原坐标信息的指针，不能为空
@param physical_coordinate_ptr [in/out] 指向转换后的坐标信息的指针，可以为空
@return 是否调用成功
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUILCD_ConvertLogicToPhysicalCoordinate(
                                          LCD_ID_E      lcd_id,             //in:
                                          GUILCD_COORDINATE_PARAM_T  *logic_coordinate_ptr, //in:
                                          GUILCD_COORDINATE_PARAM_T  *physical_coordinate_ptr //out:
                                          );

/*****************************************************************************/
// Description : set invalidate mode
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILCD_SetInvalidateMode(
                                        GUILCD_INVALIDATE_MODE_E mode
                                        );

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

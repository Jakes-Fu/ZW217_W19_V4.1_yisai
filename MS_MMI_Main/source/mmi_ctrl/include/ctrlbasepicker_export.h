/***************************************************************************
** File Name:      ctrlbasepicker_export.h                                 *
** Author:         hua.fang                                                *
** Date:           08/17/2012                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBASEPICKER_EXPORT_H_
#define _CTRLBASEPICKER_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @enum CTRLBASEPICKER_TIPS_POS_E
@brief 文字提示的位置
*/
typedef enum
{
    CTRLBASEPICKER_TIPS_POS_TOP,           /*!<顶部位置*/
    CTRLBASEPICKER_TIPS_POS_MIDDLE,        /*!<中间位置*/
    CTRLBASEPICKER_TIPS_POS_NONE,          /*!<没有提示*/
    CTRLBASEPICKER_TIPS_POS_MAX            /*!<保留位*/
} CTRLBASEPICKER_TIPS_POS_E;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 设置提示文字的位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetTipsPos (
    MMI_CTRL_ID_T                 ctrl_id,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
);

/*****************************************************************************/
//  Description : 是否在设置值的时候做动画
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_EnableAnim (
    MMI_CTRL_ID_T         ctrl_id,
    BOOLEAN               enable_anim
);

/*****************************************************************************/
//  Description : 设置picker风格的高度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetHeight (
    MMI_CTRL_ID_T        ctrl_id,
    uint16               height
);

/*****************************************************************************/
//  Description : 设置picker风格的行高
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetLineHeight (
    MMI_CTRL_ID_T    ctrl_id,
    uint16           line_height
);

/*****************************************************************************/
//  Description : 设置picker风格的文字字体
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetFont (
    MMI_CTRL_ID_T      ctrl_id,
    GUI_FONT_ALL_T     *normal_font_ptr,
    GUI_FONT_ALL_T     *active_font_ptr
);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBASEPICKER_EXPORT_H_

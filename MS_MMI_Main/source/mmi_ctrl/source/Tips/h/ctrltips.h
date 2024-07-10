/***************************************************************************
** File Name:      ctrltips.h                                            *
** Author:          xiyuan.ma                                                *
** Date:             07/19/2012                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2012      xiyuan.ma        Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _CTRLTIPS_H_
#define _CTRLTIPS_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "guistring.h"
#include "mmitheme_tips.h"

#include "ctrlbase.h"
#include "ctrltips_export.h"

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
// tips type info
#define CTRL_TIPS_TYPE (CTRLTIPS_GetType())

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// tips init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T base_ctrl_param;
} TIPS_INIT_PARAM_T;

/*! @struct GUITIPS_CTRL_T
@brief tips控件信息
*/
typedef struct _tips_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    BOOLEAN                     is_valid;   /*!< 是否有效 */
    uint16                      timer_id;   /*!< timer_id */
    MMITIPS_STYLE_INFO_T        style_info; /*!< 索引控件主题信息 */
    GUITIPS_TEXT_T              text_info;  /*!< 文本信息 */
    GUI_RECT_T                  text_rect;  /*!< 文本区域信息 */
} TIPS_OBJ_T;

// tips class for public or protected method
typedef struct _tips_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;
    
} TIPS_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*********************************************************************/
//  Description : get tips type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*********************************************************************/
PUBLIC TYPE_TAG_T CTRLTIPS_GetType();

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif

#endif  // _CTRLTIPS_H_

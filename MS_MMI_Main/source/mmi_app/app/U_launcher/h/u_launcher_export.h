/*****************************************************************************
** File Name:      u_launcher_export.h                                       *
** Author: kyle.jin         kyle                                              *
** Date:           14/02/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe go menu se                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      kyle            Creat
******************************************************************************/
#ifndef _U_LAUNCHER_EXPORT_H_
#define _U_LAUNCHER_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"

#include "mmk_type.h"
#include "guistring.h"
#include "mmiset_export.h"
#include "dal_display.h"
#include "graphics_3d_rotate.h"
#include "mmigrid_uidle_se.h"
#include "u_launcher_internal.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GO_CALC_USE_PAF888
//算法输出输入数据类型定义
#ifdef GO_CALC_USE_PAF888
#define GO_CALC_OUTPUT_DATA_TYPE DATA_TYPE_PMARGB888
#define GO_CALC_INPUT_DATA_TYPE DATA_TYPE_PMARGB888
#else
#define GO_CALC_OUTPUT_DATA_TYPE DATA_TYPE_RGB565
#define GO_CALC_INPUT_DATA_TYPE DATA_TYPE_ARGB888
#endif

//#define GO_CALC_ANIM_NO_STRING

#define GO_MAINMENU_TOTAL_FRAME_MAX MMI_MAINSCREEN_WIDTH  //必须被2整除
#define GO_ONE_FRAME_PIX (GO_MAINMENU_TOTAL_FRAME_MAX/MMI_MAINSCREEN_WIDTH)

#define GO_TP_SLIDE_MIN_VELOCITY 400
#define GO_TP_SLIDE_MAX_VELOCITY 800

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : Data Type Adapt
//  Global resource dependence : 
//  Author: KYLE
//  Note:上层数据类型与算法独立数据类型的适配
/*****************************************************************************/
PUBLIC S3D_DATA_TYPE_E SEAPI_DataTypeAdapt(DISPLAY_DATA_TYPE_E data_type);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: KYLE
//  Note:获取算法输出的类型
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E SEAPI_GetCalcOutputDataType(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: KYLE
//  Note:获取算法输入BUF类型
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E SEAPI_GetCalcInputDataType(void);

#if defined(PDA_UI_SUPPORT_MANIMENU_GO)
/*****************************************************************************/
//  Description :获取算法callback
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC go_calc_func UAPIMenu_GetCallBackfunc(GO_MENU_STYLE_E style_index);
/*****************************************************************************/
//  Description :获取算法STYLE slide type
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GO_SLIDE_TYPE_E UAPIMenu_GetCalcStyleSlideType(GO_MENU_STYLE_E style_index);

/*****************************************************************************/
//  Description :获取算法STYLE Total frame
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC uint16 UAPIMenu_GetCalcStyleCalcTime(GO_MENU_STYLE_E style_index);

/*****************************************************************************/
//  Description :获取算法STYLE信息
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GO_CALC_STYLE_INFO_T* UAPIMenu_GetCalcStyleInfo(GO_MENU_STYLE_E style_index);
#endif /*PDA_UI_SUPPORT_MANIMENU_GO*/

#if defined(PDA_UI_SUPPORT_U_IDLE)
/*****************************************************************************/
//  Description :获取算法callback
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC go_calc_func UAPIIdle_GetCallBackfunc(UIDLE_STYLE_E style_index);

/*****************************************************************************/
//  Description :获取算法STYLE slide type
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GO_SLIDE_TYPE_E UAPIIdle_GetCalcStyleSlideType(UIDLE_STYLE_E style_index);

/*****************************************************************************/
//  Description :获取算法STYLE Total frame
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC uint16 UAPIIdle_GetCalcStyleCalcTime(UIDLE_STYLE_E style_index);

/*****************************************************************************/
//  Description :获取算法STYLE信息
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GO_CALC_STYLE_INFO_T* UAPIIdle_GetCalcStyleInfo(UIDLE_STYLE_E style_index);
#endif /*PDA_UI_SUPPORT_U_IDLE*/

#endif /*_U_LAUNCHER_EXPORT_H_*/

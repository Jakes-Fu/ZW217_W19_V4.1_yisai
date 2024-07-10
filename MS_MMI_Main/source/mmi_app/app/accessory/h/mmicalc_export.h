/*****************************************************************************
** File Name:      mmicalc_export.h                                               
** Author:         taul.zhu    
** Date:           2004/0824
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2004/08/24    taul zhu          Create        
** 2010/06/18		ying.xu           Modify
**2012/02/09		murphy.xie        Modify		                             *
******************************************************************************/

#ifndef _MMICALC_EXPORT_H_
#define _MMICALC_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef CALCULATOR_SUPPORT
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"
#include "mmiacc_position.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CALC_MAX_LENGTH             13
#define PI                          3.14159265358979323846
#define LE_KEY_DUMMY                0xFF
#define MAX_CHARACTERS_2_A_KEY      13
#define CALC_TEXT_COLOR             MMI_BLACK_COLOR

#define CALC_KEY_MAP_LENGTH         12  // 1 TO 9, star, 0, hash
#define CALC_FIRST_MAP              MSG_KEYDOWN_1
#define CALC_LAST_MAP               MSG_KEYDOWN_HASH

#define MMI_EOL		NULL

//@zhaohui ,for calc optimize
#define CALC_LINE_OP_STARTX		(s_CALC_LINE_START_X)
#ifdef MMI_CALCULATOR_MINI_SUPPORT
#define CALC_LINE_OP_STARTY(n)		(s_CALC_LINE_END_Y+ s_CALC_LINE_HEIGHT*(n-1))
#else
#define CALC_LINE_OP_STARTY(n)		(s_CALC_LINE_START_Y+ s_CALC_LINE_HEIGHT*(n-1))
#endif
#define CALC_LINE_OP_ENDX		(s_CALC_LINE_START_X + s_CALC_FONT_SIZE )
#ifdef MMI_CALCULATOR_MINI_SUPPORT
#define CALC_LINE_OP_ENDY(n)		(s_CALC_LINE_END_Y+s_CALC_LINE_HEIGHT*(n))
#else
#define CALC_LINE_OP_ENDY(n)		(s_CALC_LINE_START_Y+s_CALC_LINE_HEIGHT*(n))
#endif
#define CALC_LINE_NUM_STARTX         (CALC_LINE_OP_ENDX)
#ifdef MMI_CALCULATOR_MINI_SUPPORT
#define CALC_LINE_NUM_STARTY(n)         (s_CALC_LINE_END_Y+s_CALC_LINE_HEIGHT*(n-1))
#else
#define CALC_LINE_NUM_STARTY(n)         (s_CALC_LINE_START_Y+s_CALC_LINE_HEIGHT*(n-1))
#endif
#define CALC_LINE_NUM_ENDX           (s_CALC_LINE_END_X)
#ifdef MMI_CALCULATOR_MINI_SUPPORT
#define CALC_LINE_NUM_ENDY(n)           (s_CALC_LINE_END_Y+s_CALC_LINE_HEIGHT*(n))
#else
#define CALC_LINE_NUM_ENDY(n)           (s_CALC_LINE_START_Y+s_CALC_LINE_HEIGHT*(n))
#endif
//单行显示时，M信息单独显示
#define CALC_LINE_M_STARTX		(s_CALC_LINE_START_X)
#define CALC_LINE_M_STARTY		(s_CALC_LINE_START_Y)
#define CALC_LINE_M_ENDX		(s_CALC_LINE_END_X)
#define CALC_LINE_M_ENDY		(s_CALC_LINE_START_Y+ (s_CALC_LINE_HEIGHT -s_CALC_FONT_SIZE )/2)

typedef struct
{
    uint32					id;
    GUI_RECT_T				rect;
    //	MMI_IMAGE_ID_T 			select_image_id;
    MMI_IMAGE_ID_T   		unselect_image_id;
} CAL_BUTTON_REGION_T;

//用于创建POP MENU
typedef enum
{
  ACC_CALC_MENU_START = 0,
  ACC_CALC_MENU_MPLUS,
  ACC_CALC_MENU_MSUB,
  ACC_CALC_MENU_MR,
  ACC_CALC_MENU_MCLEAR,
  ACC_CALC_MENU_CLEAR,
  ACC_CALC_MENU_MAX
}ACC_CALC_MENU_E;

//用于区分画不同的数据和区域
#define  ACC_CALC_DISPLAY_M_SYMBOL  1
#define  ACC_CALC_DISPLAY_FIRST_OPERAND  2
#define  ACC_CALC_DISPLAY_OPERATOR 4
#define  ACC_CALC_DISPLAY_EQUAL_SYMBOL 8
#define  ACC_CALC_DISPLAY_SECOND_OPERAND 16
#define  ACC_CALC_DISPLAY_MEMORY_DATA  32
#define  ACC_CALC_DISPLAY_RESULT 64

#endif//#ifdef CALCULATOR_SUPPORT

/*****************************************************************************/
// 	Description : MMIAPICALC_OpenMainWin
//         
//	Global resource dependence : 
//   Author:        zhaohui
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPICALC_OpenMainWin(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  



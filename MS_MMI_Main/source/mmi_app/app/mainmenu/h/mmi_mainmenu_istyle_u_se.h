/*****************************************************************************
** File Name:      mmimenu_u_mainmenu_se.h                                   *
** Author:                                                                   *
** Date:           05/05/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2012      Xiaoqing.Lu        Creat
******************************************************************************/

#ifndef _MMI_MAINMENU_ISTYLE_U_SE_H_
#define _MMI_MAINMENU_ISTYLE_U_SE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_ISTYLE_SUPPORT
#include "u_launcher_export.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_istyle.h"
#endif
#include "mmi_mainmenu_u_se.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#ifdef MMI_PDA_SUPPORT
#ifdef MMI_ISTYLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
typedef struct  
{
    GO_MAINMENU_PAGE_INFO_T page_info;
    GUI_LCD_DEV_INFO        lcd_dev_info;
}U_MAINMENU_PAGE_INFO_T;

typedef void (*MMIMENU_U_GET_PAGE_INDEX_FUNC)(MMIMAINMENU_ISTYLE_DATA_T*, uint16*, uint16*, GUI_POINT_T*);

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获得页面数据
//  Global resource dependence : 
//  Author:  xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_SetPageInfo(
                                     MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                     BOOLEAN is_circles // 是否循环
                                     );

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_UninitUMainmenuEnv(void);

/*****************************************************************************/
//  Description : uninit u mainmenu environment.
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_InitUMainmenuEnv(MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : u mainmenu display
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_Display(
                                 MMIMAINMENU_ISTYLE_DATA_T *menu_ctrl_ptr,
                                 GUI_POINT_T    *point_ptr
                                 );

/*****************************************************************************/
//  Description : release GO all item layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_ReleaseAllItemLayer( void);

/*****************************************************************************/
//  Description : 处理tpup时
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_HandleUMenuSlideOverX(
                                               MMIMAINMENU_ISTYLE_DATA_T   *menu_ctrl_ptr,
                                               GUI_POINT_T      *tp_point_ptr,
                                               BOOLEAN is_circles
                                               );
#ifdef UI_SUPPORT_U_MAINMENU
/*****************************************************************************/
//  Description :顺序切换主菜单ICON
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_ChangeMenuStyle(
                                      MMI_WIN_ID_T win_id
                                      );
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIISTYLE_U_GenerateRandomStyle(
                                            MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIISTYLE_U_DrawUMenuLastFrame(
                                            MMIMAINMENU_ISTYLE_DATA_T    *menu_ctrl_ptr, 
                                            GO_MOVE_DIRECTION_E direction,
                                            BOOLEAN             is_change_page
                                            );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_ResetIstyleItemLayerPtr(
                                              uint16 item_num,
                                              U_MAINMENU_PAGE_INFO_T *umenu_page_info_ptr
                                              );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIISTYLE_U_DistoryPageLayer(void);

#endif // MMI_ISTYLE_SUPPORT
#endif // MMI_PDA_SUPPORT

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMI_MAINMENU_ISTYLE_U_SE_H_

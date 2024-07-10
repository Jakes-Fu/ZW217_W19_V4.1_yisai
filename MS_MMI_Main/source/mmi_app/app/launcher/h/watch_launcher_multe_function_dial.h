

#ifndef WATCH_LAUNCHER_MAIN_H
#define WATCH_LAUNCHER_MAIN_H

#include "guistring.h"
#include "watch_launcher_common.h"
#include "mmiset_nv.h"


#define MTF_DIAL_UPRECT DP2PX_RECT(90, 30, 150, 90)
#define MTF_DIAL_DOWNRECT DP2PX_RECT(90, 150, 150, 210)
#define MTF_DIAL_LEFTRECT DP2PX_RECT(30, 90, 90, 150)
#define MTF_DIAL_RIGHTRECT DP2PX_RECT(150, 90, 210, 150)

#define MTF_DIAL_EDIT_UPRECT DP2PX_RECT(84, 24, 156, 96)
#define MTF_DIAL_EDIT_DOWNRECT DP2PX_RECT(84, 144, 156, 216)
#define MTF_DIAL_EDIT_LEFTRECT DP2PX_RECT(24, 84, 96, 156)
#define MTF_DIAL_EDIT_RIGHTRECT DP2PX_RECT(144, 84, 216, 156)


//#define EDIT_BUTTON_RECT DP2PX_RECT(108, 206, 132, 230)//img rect
#define EDIT_BUTTON_RECT DP2PX_RECT(90, 190, 150, 250)//divice rect
#define LEFT_BUTTON_RECT DP2PX_RECT(0, 102, 30, 138)
#define RIGHT_BUTTON_RECT DP2PX_RECT(210, 102, 240, 138)
#define MTF_DIAL_LIST_ITEM_MAX 6

typedef enum{
	MTF_DIAL_ITEM_POWER = 0,
	MTF_DIAL_ITEM_DATE,
	MTF_DIAL_ITEM_HEART,
	MTF_DIAL_ITEM_STEPS,
	MTF_DIAL_ITEM_COMBUSTION,
	MTF_DIAL_ITEM_STAND,
	MTF_DIAL_ITEM_MAX

}MTF_DIAL_ITEM_TYPE_E;

typedef enum{
	MTF_DIAL_CONTROLER_POSITION_UP = 0,
	MTF_DIAL_CONTROLER_POSITION_DOWN,
	MTF_DIAL_CONTROLER_POSITION_LEFT,
	MTF_DIAL_CONTROLER_POSITION_RIGHT,
	MTF_DIAL_CONTROLER_POSITION_MAX

}MTF_DIAL_CONTROLER_POSITION;

typedef enum{
	MMI_MTF_DIAL_CTRL_ID_START = 0,
	MMI_MTF_DIAL_CTRL_STYPE_LIST_ID,
	MMI_MTF_DIAL_MAX_CTRL_ID
}MMI_MTF_DIAL_CONTROL_IS_E;

/*****************************************************************************
//  Description : center edit MTFWIN
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/

PUBLIC void WatchLauncher_Edit_MTFDial_Win_Enter(void);

/*****************************************************************************
//  Description : center edit MTF list WIN
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/


PUBLIC void WatchLauncher_Edit_MTFDial_List_Win_Enter(void);
/*****************************************************************************
//  Description : on click in Edit win
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/

PUBLIC BOOLEAN OnClickInEditDiaWin(GUI_POINT_T  point);
/*****************************************************************************
//  Description : draw controler on MTF win
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/

PUBLIC void DrawMTFControlerItem( MMI_WIN_ID_T	win_id, GUI_LCD_DEV_INFO lcd_dev_info);
/*****************************************************************************
//  Description : Draw controler on edit win
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/

PUBLIC void DrawMTFEditWinControlerItem( MMI_WIN_ID_T	win_id, GUI_LCD_DEV_INFO lcd_dev_info);


/*****************************************************************************
//  Description : Init MTF dislay ItemType
//  Author: haosheng.cui
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void InitMTFDisplayItemType(void);

#endif


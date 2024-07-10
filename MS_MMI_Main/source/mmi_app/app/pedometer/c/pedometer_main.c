
#include "std_header.h"
#include <stdlib.h>
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmi_modu_main.h"
#include "mmi_textfun.h"
#include "mmi_string.h"

#include "guilcd.h"
#include "guifont.h"
#include "guistring.h"

//#include "watch_slidepage.h"
//#include "watch_launcher_common.h"
//#include "watch_launcher_main.h"

#include "pedometer_export.h"
#include "pedometer_id.h"
#include "pedometer_image.h"
#include "pedometer_text.h"
#include "pedometer_nv.h"

#define MMIIDLE_DATE_MAX_LEN    16

#define WATCH_LAUNCHER_INDICATOR_OFFSET 18

LOCAL MMI_HANDLE_T s_handle = 0;

LOCAL GUI_LCD_DEV_INFO s_indi_layer = {0};


LOCAL void CreateIndicatorLayer(
	MMI_WIN_ID_T win_id,
	uint32 width,
	uint32 height,
	GUI_LCD_DEV_INFO *lcd_dev_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT
	UILAYER_CREATE_T create_info = {0};
	if (PNULL == lcd_dev_ptr)
	{
		return;
	}

	lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
	lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;

	// ´´½¨²ã
	create_info.lcd_id = GUI_MAIN_LCD_ID;
	create_info.owner_handle = win_id;
	create_info.offset_x = 0;
	create_info.offset_y = 0;
	create_info.width = width;
	create_info.height = height;
	create_info.is_bg_layer = FALSE;
	create_info.is_static_layer = TRUE;

	UILAYER_CreateLayer(
		&create_info,
		lcd_dev_ptr
		);

	UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
	UILAYER_Clear(lcd_dev_ptr);
#endif
}

#if 0
PUBLIC MMI_RESULT_E PedometerLauncher_HandleMsg(
	MMI_WIN_ID_T        win_id,        //IN:
	MMI_MESSAGE_ID_E    msg_id,        //IN:
	DPARAM            param        //IN:
	)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	switch (msg_id)
	{
	case MSG_SLIDEPAGE_OPENED:
		{
			GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
			CreateIndicatorLayer(win_id, 240, 240, &s_indi_layer);
			if (!UILAYER_IsBltLayer(&s_indi_layer))
			{
				UILAYER_APPEND_BLT_T append;
				append.layer_level = UILAYER_LEVEL_NORMAL;
				append.lcd_dev_info = s_indi_layer;
				UILAYER_AppendBltLayer(&append);
			}

			break;
		}

	case MSG_SLIDEPAGE_GETFOCUS:
		{
			if (!UILAYER_IsBltLayer(&s_indi_layer))
			{
				UILAYER_APPEND_BLT_T append;
				append.layer_level = UILAYER_LEVEL_NORMAL;
				append.lcd_dev_info = s_indi_layer;
				UILAYER_AppendBltLayer(&append);
			}

			break;
		}

	case MSG_SLIDEPAGE_LOSEFOCUS:
		{
			UILAYER_RemoveBltLayer(&s_indi_layer);
			break;
		}

	case MSG_SLIDEPAGE_PAGECHANGED:
		{

		}
		break;
	default:
		break;
	}
	return recode;
}
#endif

PUBLIC void MMIPEDOMETER_CreateMainWinTable(void)
{
	MMIPEDOMETER_CreateTodayWin();
#if 0
	tWatchSlidePageItem elem[3] = {0};
	MMI_HANDLE_T handle = WatchSLIDEPAGE_CreateHandle();
	uint8 i = 0;

	elem[i].fun_enter_win = MMIPEDOMETER_CreateTodayWin;
	elem[i].win_id = MMI_PEDOMETER_TODAY_EXERCISE_WIN_ID;
	i++;

	elem[i].fun_enter_win = MMIPEDOMETER_CreateStepWeekWin;
	elem[i].win_id = MMI_PEDOMETER_STEP_WEEK_WIN_ID;
	i++;

	elem[i].fun_enter_win = MMIPEDOMETER_CreateFriendRankWin;
	elem[i].win_id = MMI_PEDOMETER_STEP_LIST_WIN_ID;
	i++;

	s_handle = handle;
	WatchSLIDEPAGE_Open(handle, elem, i, 0, FALSE, PedometerLauncher_HandleMsg);
#endif
}

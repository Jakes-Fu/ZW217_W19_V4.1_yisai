
#include "std_header.h"
#include <stdlib.h>
#include <stdio.h>

#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiacc_text.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmi_modu_main.h"
#include "mmi_textfun.h"
#include "mmi_string.h"
#include "mmi_resource.h"
#include "mmi_appmsg.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"

#include "guilcd.h"
#include "guifont.h"
#include "guistring.h"
#include "guitext.h"

#include "pedometer_export.h"
#include "pedometer_id.h"
#include "pedometer_image.h"
#include "pedometer_text.h"
#include "pedometer_nv.h"

PUBLIC MMI_RESULT_E HandlePedometerLoadingWin(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id, 
	DPARAM param
	)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		break;
	case MSG_FULL_PAINT:
		{
			MMI_CTRL_ID_T ctrl_id = MMI_PEDOMETER_LOADING_TEXT_CTRL_ID;
			GUI_BG_T text_bg = {0};
			GUI_COLOR_T text_color = {0};
			GUI_FONT_T text_font = {0};
			GUI_RECT_T text_rect = {40,100,240,240};
			MMI_STRING_T text_loading = {0};
			BOOLEAN isConnectWifi = TRUE;

			GUI_RECT_T rect = {0,0,240,240};
			GUI_FillRect(&lcd_dev_info,rect,MMI_BLACK_COLOR);//填充整个布局的颜色

			text_bg.color = MMI_BLACK_COLOR;
			text_color = MMI_WHITE_COLOR;
			text_font = SONG_FONT_32;

			/*//暂时不需要联网
			if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
			{
				SCI_TRACE_LOW("------connect wifi------");
				MMIRES_GetText(TXT_PEDOMETER_LOADING_DATA,win_id,&text_loading);
			}else
			{
				uint32 sim_ok_num = 0;
				uint16 sim_ok = MN_DUAL_SYS_1;
				sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
				SCI_TRACE_LOW("----------sim_ok_num = %d,sim_ok = %d",sim_ok_num,sim_ok);
				if(0 == sim_ok_num)
				{
					SCI_TRACE_LOW("------no connect wifi------");
					MMIRES_GetText(TXT_PEDOMETER_NO_NET,win_id,&text_loading);
					isConnectWifi = FALSE;
				}else
				{
					SCI_TRACE_LOW("------connect sim------");
					MMIRES_GetText(TXT_PEDOMETER_LOADING_DATA,win_id,&text_loading);
				}
			}*/

			MMIRES_GetText(TXT_PEDOMETER_LOADING_DATA,win_id,&text_loading);
			GUITEXT_SetRect(ctrl_id,&text_rect);
			GUITEXT_SetBg(ctrl_id,&text_bg);
			GUITEXT_SetFont(ctrl_id,&text_font,&text_color);
			GUITEXT_SetString(ctrl_id,text_loading.wstr_ptr,text_loading.wstr_len,FALSE);

			/*if (isConnectWifi)
			{
				MMIPEDOMETER_CreateTodayWin();
			}*/
			MMIPEDOMETER_CreateMainWinTable();
			MMK_CloseWin(win_id);
			
		}
		break;
	case MSG_APP_OK:
	case MSG_APP_CANCEL:
	case MSG_APP_WEB:
		{
			MMK_CloseWin(win_id);
		}
		break;
	default:
		break;

	}

	return recode;
}

WINDOW_TABLE(MMI_PEDOMETER_LOADING_WIN_TAB) = 
{
	WIN_ID(MMI_PEDOMETER_LOADING_WIN_ID),
	WIN_FUNC((uint32)HandlePedometerLoadingWin),
	WIN_HIDE_STATUS,
	CREATE_TEXT_CTRL(MMI_PEDOMETER_LOADING_TEXT_CTRL_ID),
	END_WIN
};

PUBLIC void MMIPEDOMETER_CreateLoadingWin()
{
	MMI_WIN_ID_T win_id = MMI_PEDOMETER_LOADING_WIN_ID;
	MMI_HANDLE_T win_handle = 0;
	GUI_RECT_T rect = {0, 0, 240, 240};
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}
	win_handle = MMK_CreateWin((uint32*)MMI_PEDOMETER_LOADING_WIN_TAB, NULL);
	MMK_SetWinRect(win_handle, &rect);
}

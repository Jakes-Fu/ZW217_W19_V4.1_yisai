
#include "std_header.h"
#include <stdlib.h>
#include "window_parse.h"
#include "sci_types.h"
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
#include "guitext.h"
#include "guibutton.h"
#include "guilistbox.h"
#include "pedometer_export.h"
#include "pedometer_id.h"
#include "pedometer_image.h"
#include "pedometer_text.h"
#include "pedometer_nv.h"
#include "msensor_drv.h"
#include "mmi_common.h"
#include "zdt_gsensor.h"

#define MMI_PEDO_YELLOW_COLOR		0xff00 

#define  MMI_PEDO_MAX_LIST_ITEM		1

int16 pedometer_rank_tp_down_x = 0;
int16 pedometer_rank_tp_down_y = 0;

LOCAL MMI_IMAGE_ID_T rank_image_id[] = 
{
	IMAGE_RANK_NUM1,IMAGE_RANK_NUM2,IMAGE_RANK_NUM3,IMAGE_RANK_NO_NUM,
	IMAGE_RANK_NO_NUM,IMAGE_RANK_NO_NUM,IMAGE_RANK_NO_NUM,
	IMAGE_RANK_NO_NUM,IMAGE_RANK_NO_NUM,IMAGE_RANK_NO_NUM
};

LOCAL MMI_IMAGE_ID_T praise_image_id[] =
{
	IMAGE_ICON_NO_PRAISE,IMAGE_ICON_PRAISE
};

LOCAL int click_index = -1;
LOCAL int praise_index[] = 
{
	0,0,0,0,0,
	0,0,0,0,0
};

LOCAL void AppendListItem(
	MMI_CTRL_ID_T ctrl_id,
	MMI_IMAGE_ID_T image_rank,
	MMI_IMAGE_ID_T image_praise,
	wchar *user_name_wchar,
	wchar *user_step_wchar,
	wchar *user_praise_wchar
	)
{

	MMI_STRING_T user_name = {0};
	MMI_STRING_T user_step = {0};
	MMI_STRING_T user_praise = {0};
	GUILIST_ITEM_T item_t = {0};
	GUILIST_ITEM_DATA_T item_data = {0};
	GUI_RECT_T list_rect = {0,40,240,240};
	GUI_COLOR_T list_color = {0};

	user_name.wstr_ptr = user_name_wchar;
	user_name.wstr_len = MMIAPICOM_Wstrlen(user_name_wchar);
	user_step.wstr_ptr = user_step_wchar;
	user_step.wstr_len = MMIAPICOM_Wstrlen(user_step_wchar);
	user_praise.wstr_ptr = user_praise_wchar;
	user_praise.wstr_len = MMIAPICOM_Wstrlen(user_praise_wchar);

	//(在mmitheme_list.c中定义item_style，在mmitheme_list.h中声明.在guilistbox.c中添加case:)
	item_t.item_style = GUIITEM_STYLE_TWO_LINE_TWO_ICON_AND_THREE_TEXT_MS;
	item_t.item_data_ptr = &item_data;

	item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[0].item_data.image_id = image_rank;

	item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[1].item_data.text_buffer = user_name;

	item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[2].item_data.text_buffer = user_step;

	item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
	item_data.item_content[3].item_data.image_id = image_praise;

	item_data.item_content[4].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	item_data.item_content[4].item_data.text_buffer = user_praise;

	//不画分割线
	GUILIST_SetListState(ctrl_id,GUILIST_STATE_SPLIT_LINE, FALSE );
	//不画高亮条
	GUILIST_SetListState(ctrl_id,GUILIST_STATE_NEED_HIGHTBAR, FALSE );

	list_color = MMI_BLACK_COLOR;
	GUILIST_SetRect(ctrl_id,&list_rect);
	GUILIST_SetBgColor(ctrl_id,list_color);
	GUILIST_SetCurItemIndex(ctrl_id,click_index);
	GUILIST_AppendItem(ctrl_id,&item_t);

	return;
}

LOCAL void DisplayRankList(
	MMI_WIN_ID_T win_id, 
	MMI_CTRL_ID_T ctrl_id
	)
{
	int	i = 0;
	SCI_DATE_T  sys_date = {0};
	uint8 day = 0;
	uint32 error_code = 0;

	wchar user_name_wchar[10] = {'A','9',0};

	int user_praise = 0;
	char user_praise_char[20] = {0};
	wchar user_praise_wchar[20] = {0};
	MMI_STRING_T text_praise = {0};

	int	new_user_step = 0;
	int user_step = 0;
	char user_step_char[20] = {0};
	wchar user_step_wchar[20] = {0};
	MMI_STRING_T text_step = {0};

	MMK_SetAtvCtrl(win_id, ctrl_id);
	GUILIST_RemoveAllItems(ctrl_id);
	GUILIST_SetMaxItem(ctrl_id, 1, FALSE);

	TM_GetSysDate(&sys_date);
	MMI_ReadNVItem(MMINV_PEDOMETER_DATE_NV,&day);
	MMI_ReadNVItem(MMINV_PEDOMETER_PRAISE_NV,&user_praise);
	MMI_ReadNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&user_step);
	SCI_TRACE_LOW("DisplayRankList day = %d,sys_date.mday = %d",day,sys_date.mday);
	SCI_TRACE_LOW("DisplayRankList user_praise = %d,user_step = %d",user_praise,user_step);

	//这里要有取步数的操作,并赋值给new_step
#ifdef WIN32
	new_user_step = 100;
#else
	new_user_step = 2800;//ZDT_GSensor_GetStepOneDay();
#endif
	Trace_Readboy_Log("DisplayRankList new_user_step = %d",new_user_step);

	if (sys_date.mday != day || new_user_step > user_step)
	{
		user_step = new_user_step;
		MMI_WriteNVItem(MMINV_PEDOMETER_DAY1_STEP_NV,&user_step);
		MMI_WriteNVItem(MMINV_PEDOMETER_DATE_NV,&sys_date.mday);
	}

	if (sys_date.mday != day)
	{
		user_praise = 0;
		MMI_WriteNVItem(MMINV_PEDOMETER_PRAISE_NV,&user_praise);
		MMI_WriteNVItem(MMINV_PEDOMETER_DATE_NV,&sys_date.mday);
	}

	itoa(user_praise,user_praise_char,10);
	text_praise.wstr_ptr = MMIAPICOM_StrToWstr((uint8 *)user_praise_char,user_praise_wchar);

	itoa(user_step,user_step_char,10);
	text_step.wstr_ptr = MMIAPICOM_StrToWstr((uint8 *)user_step_char,user_step_wchar);

	for (i = 0;i < 1;i++)
	{
		if (0 == praise_index[i])
		{
			AppendListItem(ctrl_id,rank_image_id[i],IMAGE_ICON_NO_PRAISE,user_name_wchar,user_step_wchar,user_praise_wchar);
		}else
		{
			AppendListItem(ctrl_id,rank_image_id[i],IMAGE_ICON_PRAISE,user_name_wchar,text_step.wstr_ptr,text_praise.wstr_ptr);
		}
	}

}

LOCAL void ListItemCallBack(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id)
{
	MMI_IMAGE_ID_T image_praise = {0};
	int user_praise = 0;
	int num_0 = 0;
	int num_1 = 1;
	click_index = GUILIST_GetCurItemIndex(ctrl_id);

	MMI_ReadNVItem(MMINV_PEDOMETER_PRAISE_NV,&user_praise);
	if (0 == praise_index[click_index])
	{
		praise_index[click_index] = SCI_ALLOCA(sizeof(num_1));
		SCI_MEMCPY(praise_index[click_index],&num_1,sizeof(num_1));
		user_praise = user_praise + 1;
		MMI_WriteNVItem(MMINV_PEDOMETER_PRAISE_NV,&user_praise);
	}else
	{
		praise_index[click_index] = SCI_ALLOCA(sizeof(num_0));
		SCI_MEMCPY(praise_index[click_index],&num_0,sizeof(num_0));
	}

	MMK_SendMsg(win_id,MSG_FULL_PAINT,NULL);
}

PUBLIC MMI_RESULT_E HandlePedometerFriendRankWin(
	MMI_WIN_ID_T win_id, 
	MMI_MESSAGE_ID_E msg_id,
	DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUI_RECT_T rect = {0,0,240,240};
	MMI_CTRL_ID_T ctrl_id = MMI_PEDOMETER_STEP_LIST_CTRL_ID;
	MMI_CheckAllocatedMemInfo();

	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			
		}
		break;
	case MSG_FULL_PAINT:
		{
			GUI_RECT_T title_txt_rect = {0,10,240,45};
			GUI_RECT_T title_img_rect = {63,70,240,240};
			MMI_STRING_T text_rank_title = {0};
			GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
			GUISTR_STYLE_T text_style = {0};

			text_style.align = ALIGN_HMIDDLE;
			text_style.font = SONG_FONT_26;
			text_style.font_color = MMI_PEDO_YELLOW_COLOR;

			{
				GUI_RECT_T clientRect = {0,0,240,240};
				GUI_RECT_T win_rect = {0};
				win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
				GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
			}

			title_txt_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_txt_rect);
			MMIRES_GetText(TXT_PEDOMETER_STEP_LIST,win_id,&text_rank_title);
			GUISTR_DrawTextToLCDInRect(
				(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				&title_txt_rect,
				&title_txt_rect,
				&text_rank_title,
				&text_style,
				text_state,
				GUISTR_TEXT_DIR_AUTO
				);	

			title_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id,title_img_rect);
			GUIRES_DisplayImg(NULL,&title_img_rect,NULL,win_id,IMAGE_IMAGE_NO_DATA,&lcd_dev_info);

			DisplayRankList(win_id,ctrl_id);

		}
		break;
	case MSG_CLOSE_WINDOW:
		{

		}
		break;

		case MSG_TP_PRESS_DOWN:
		{
			GUI_POINT_T   point = {0};
	    		point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			pedometer_rank_tp_down_x = point.x;
			pedometer_rank_tp_down_y = point.y;
		}
		break;
	case MSG_TP_PRESS_UP:
		{
			GUI_POINT_T   point = {0};
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if((pedometer_rank_tp_down_x - point.x) > 40)
			{
			     break;
			}
			else if((point.x - pedometer_rank_tp_down_x) > 40)
			{
				MMIPEDOMETER_CreateStepWeekWin();
				MMK_CloseWin(win_id);
				break;
			}
		}
		break;
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		{
			
		}
		break;
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
	case MSG_CTL_PENOK:
#endif
		{
			//ListItemCallBack(win_id,ctrl_id);
		}
		break;
	default:
		break;
	}

	return recode;
}

WINDOW_TABLE(MMI_PEDOMETER_RANK_WIN_TAB) = 
{
	WIN_ID(MMI_PEDOMETER_STEP_LIST_WIN_ID),
	WIN_FUNC((uint32)HandlePedometerFriendRankWin),
	WIN_HIDE_STATUS,
	END_WIN
};

PUBLIC void MMIPEDOMETER_CreateFriendRankWin()
{
	MMK_CreateWin((uint32*)MMI_PEDOMETER_RANK_WIN_TAB, NULL);
}

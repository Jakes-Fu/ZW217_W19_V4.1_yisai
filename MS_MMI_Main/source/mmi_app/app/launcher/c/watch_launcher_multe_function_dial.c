
/*****************************************************************************
** File Name:      watch_launcher_multe_function_dial.c                                           *
** Author:                                                                      *
** Date:           03/16/2020                                                   *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.         ..*
** Description:    watch charge win
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/16/2021  haosheng.cui        Create
******************************************************************************/


#include "watch_launcher_multe_function_dial.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guistring.h"
#include "ui_layer.h"
#include "graphics_draw.h"

#include "watch_slidepage.h"
#include "watch_launcher_main.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "watch_pb_view.h"
#include "watch_stopwatch.h"
#include "watch_gallery_export.h"
#include "watch_charge_win.h"
#include "mmirecord_main.h"
#include "watch_launcher_editclockwin.h"
#include "watch_sms_main.h"
#include "mmicc_export.h" //for dialpad
#include "mmicom_trace.h"

#include "watch_launcher_common_panel_item.h"
#include "mmiidle_func.h"
#include "watch_common_list.h"

typedef struct OnMTFClickAction
{
	GUI_RECT_T  rect;
	BOOLEAN (*fun)(void);

}OnMTFClickAction;

LOCAL BOOLEAN EnterNextDialWin(void);
LOCAL BOOLEAN EnterPreDialWin(void);
LOCAL BOOLEAN EnterEditMTFDialWin(void);
LOCAL BOOLEAN OnUpBtn(void);
LOCAL BOOLEAN OnDownBtn(void);
LOCAL BOOLEAN OnLeftBtn(void);
LOCAL BOOLEAN OnRightBtn(void);

LOCAL BOOLEAN OnClick(GUI_POINT_T  point,OnMTFClickAction *action);

WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T stepIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_STEP_NUMBER,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_step_number,DP2PX_RECT(0,16,60,32),{NULL,0}};
WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T standIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_STAND_NUMBER,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_stand_number,DP2PX_RECT(0,16,60,32),{NULL,0}};
WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T heartIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_HEART_RATE,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_heart_rate,DP2PX_RECT(0,16,60,32),{NULL,0}};
WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T chargeIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_CHARGE,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_charge,DP2PX_RECT(0,16,60,32),{NULL,0}};
WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T powerIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_CHARGE,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_battery,DP2PX_RECT(0,16,60,32),{NULL,0}};
WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T calorieIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_CALORIE,DP2PX_RECT(0,0,60,60),res_aw_clock_function_ic_calorie,DP2PX_RECT(0,16,60,32),{NULL,0}};


WATCH_PAN_CONTROLLER_DATA_2STR_T dateIcon={WATCH_PAN_CONTROLLER_DATA_TYPE_DAY,DP2PX_RECT(0,0,60,36),{NULL,0},
                                        WATCH_PAN_CONTROLLER_DATA_TYPE_WEEK,DP2PX_RECT(0,36,60,52),{NULL,0}};

typedef enum
{
    WATCH_ARRAY_POWSER_INDEX = 0,
    WATCH_ARRAY_STEP_INDEX,
    WATCH_ARRAY_STAND_INDEX,
    WATCH_ARRAY_HEART_INDEX,
    WATCH_ARRAY_CALORIE_INDEX,
    WATCH_ARRAY_CHARGE_INDEX,
    WATCH_ARRAY_DATE_INDEX,
    WATCH_ARRAY_MAX_INDEX
}WATCH_CONTROLLER_ARRAY_TYPE_E;

WATCHCOM_PAN_DATA_T MTFWINItem[]={
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&powerIcon},
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&stepIcon},
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&standIcon},
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&heartIcon},
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&calorieIcon},
	{DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG,&chargeIcon},
    {DP2PX_RECT(0,0,0,0),WATCH_PAN_CONTROLLER_TYPE_2STR,&dateIcon}
};

OnMTFClickAction ClickActionOnEditMTFWinArray[]={
		{EDIT_BUTTON_RECT,EnterEditMTFDialWin},
		{NULL,NULL}
};
OnMTFClickAction ClickActionOnEditWinArray[]={
		{LEFT_BUTTON_RECT,EnterPreDialWin},
		{RIGHT_BUTTON_RECT,EnterNextDialWin},
		{NULL,NULL}
};
OnMTFClickAction ClickActionOnEditMTFWinListArray[]={
		{MTF_DIAL_UPRECT,OnUpBtn},
		{MTF_DIAL_DOWNRECT,OnDownBtn},
		{MTF_DIAL_LEFTRECT,OnLeftBtn},
		{MTF_DIAL_RIGHTRECT,OnRightBtn},
		{NULL,NULL}
};
MTF_DIAL_ITEM_TYPE_E MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_MAX]={
	MTF_DIAL_ITEM_POWER,
	MTF_DIAL_ITEM_DATE,
	MTF_DIAL_ITEM_STEPS,
	MTF_DIAL_ITEM_COMBUSTION
};
GUI_RECT_T MTFItemPositionArray[MTF_DIAL_CONTROLER_POSITION_MAX]={
	MTF_DIAL_UPRECT,
	MTF_DIAL_DOWNRECT,
	MTF_DIAL_LEFTRECT,
	MTF_DIAL_RIGHTRECT
};

GUI_RECT_T MTFEditItemPositionArray[MTF_DIAL_CONTROLER_POSITION_MAX]={
    MTF_DIAL_EDIT_UPRECT,
    MTF_DIAL_EDIT_DOWNRECT,
    MTF_DIAL_EDIT_LEFTRECT,
    MTF_DIAL_EDIT_RIGHTRECT
};

MTF_DIAL_CONTROLER_POSITION curPosition = MTF_DIAL_CONTROLER_POSITION_UP;


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
	
LOCAL MMI_RESULT_E HandleLauncherEditMTFDialWinMsg(
									  MMI_WIN_ID_T		  win_id,		 //IN:
									  MMI_MESSAGE_ID_E	  msg_id,		 //IN:
									  DPARAM			param		 //IN:
									  );
	
LOCAL MMI_RESULT_E HandleLauncherEditMTFDialListWinMsg(
									  MMI_WIN_ID_T		  win_id,		 //IN:
									  MMI_MESSAGE_ID_E	  msg_id,		 //IN:
									  DPARAM			param		 //IN:
									  );


WINDOW_TABLE(WATCH_LAUNCHER_EDITMTFDIAL_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherEditMTFDialWinMsg),
    WIN_ID(WATCH_LAUNCHER_EDITMTFDIAL_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_EDITMTFDIAL_LIST_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherEditMTFDialListWinMsg),
    WIN_ID(WATCH_LAUNCHER_EDITMTFDIAL_LIST_WIN_ID),
    WIN_TITLE(TXT_AW_MTF_LIST_TITLE_CUSTOM),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMI_MTF_DIAL_CTRL_STYPE_LIST_ID),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

PUBLIC void InitMTFDisplayItemType(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_MTF_UP_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_UP]), return_value);
    if ( MN_RETURN_SUCCESS != return_value )
    {
        MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_UP] = MTF_DIAL_ITEM_POWER;
        MMINV_WRITE(MMINV_SET_MTF_UP_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_UP]));
    }
    MMINV_READ(MMINV_SET_MTF_DOWN_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_DOWN]), return_value);
    if ( MN_RETURN_SUCCESS != return_value )
    {
        MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_DOWN] = MTF_DIAL_ITEM_STEPS;
        MMINV_WRITE(MMINV_SET_MTF_DOWN_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_DOWN]));
    }
    MMINV_READ(MMINV_SET_MTF_LEFT_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_LEFT]), return_value);
    if ( MN_RETURN_SUCCESS != return_value )
    {
        MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_LEFT] = MTF_DIAL_ITEM_DATE;
        MMINV_WRITE(MMINV_SET_MTF_LEFT_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_LEFT]));
    }
    MMINV_READ(MMINV_SET_MTF_RIGHT_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_RIGHT]), return_value);
    if ( MN_RETURN_SUCCESS != return_value )
    {
        MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_RIGHT] = MTF_DIAL_ITEM_COMBUSTION;
        MMINV_WRITE(MMINV_SET_MTF_RIGHT_TYPE, &(MTFItemTypeArray[MTF_DIAL_CONTROLER_POSITION_RIGHT]));
    }
}


LOCAL void MMIMTF_SetItemType(MTF_DIAL_CONTROLER_POSITION position, MTF_DIAL_ITEM_TYPE_E item_type )
{
    MTFItemTypeArray[position]= item_type;
    switch(position)
    {
        case MTF_DIAL_CONTROLER_POSITION_UP:
               MMINV_WRITE(MMINV_SET_MTF_UP_TYPE, &(MTFItemTypeArray[position]));
            break;
        case MTF_DIAL_CONTROLER_POSITION_DOWN:
               MMINV_WRITE(MMINV_SET_MTF_DOWN_TYPE, &(MTFItemTypeArray[position]));
            break;
        case MTF_DIAL_CONTROLER_POSITION_LEFT:
               MMINV_WRITE(MMINV_SET_MTF_LEFT_TYPE, &(MTFItemTypeArray[position]));
            break;
        case MTF_DIAL_CONTROLER_POSITION_RIGHT:
               MMINV_WRITE(MMINV_SET_MTF_RIGHT_TYPE, &(MTFItemTypeArray[position]));
            break;
        default:
            break;
    }
}
LOCAL MMI_RESULT_E HandleLauncherEditMTFDialWinMsg(
									  MMI_WIN_ID_T		  win_id,		 //IN:
									  MMI_MESSAGE_ID_E	  msg_id,		 //IN:
									  DPARAM			param		 //IN:
									  )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

	MMI_CheckAllocatedMemInfo();

	switch (msg_id)
	{
		case MSG_OPEN_WINDOW:
		{
			break;
		}

		case MSG_KEYDOWN_CANCEL:
		{
			MMK_CloseWin(win_id);
			break;
		}

		case MSG_CLOSE_WINDOW:
		{
			break;
		}

		case MSG_FULL_PAINT:
		{
            GUI_RECT_T img_rect = DP2PX_RECT(0, 0, 240, 240);
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, img_rect, MMI_BLACK_COLOR);
            GUIRES_DisplayImg(PNULL,
                               &img_rect,
                               PNULL,
                               win_id,
                               res_aw_clock_function_edit_bg,
                               &lcd_dev_info
                               );
            DrawMTFEditWinControlerItem(win_id,lcd_dev_info);

            break;
        }

		case MSG_APP_WEB:
		{
            MMK_CloseWin(win_id);
			break;
		}
#ifdef TOUCH_PANEL_SUPPORT
		case MSG_TP_PRESS_DOWN:
		{

			break;
		}

		case MSG_TP_PRESS_MOVE:
		{
			break;
		}

        case MSG_TP_PRESS_UP:
        {
            BOOLEAN pointresult = TRUE;
            GUI_POINT_T  point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            pointresult = OnClick(point,ClickActionOnEditMTFWinListArray);
            if(TRUE == pointresult)
            {
                return MMI_RESULT_TRUE;
            }
            MMK_CloseWin(win_id);
            break;
        }
#endif
		case MSG_KEYDOWN_1:
		{
			break;
		}

		case MSG_KEYDOWN_2:
		{
			break;
		}

        case MSG_GET_FOCUS:
		{
            MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
			break;
		}

		default:
			recode = MMI_RESULT_FALSE;
			break;
	}
	MMI_CheckAllocatedMemInfo();
	return recode;
}

LOCAL void InitControlerItemListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id = MMI_MTF_DIAL_CTRL_STYPE_LIST_ID;
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    MMI_STRING_T    Str = {0};
    MMI_STRING_T    Str2 = {0};
    uint16  curSelection   =   MTFItemTypeArray[curPosition];
    int i=0;
    MMI_TEXT_ID_T textarray[]={TXT_AW_MTF_LIST_CHARGE,TXT_AW_MTF_LIST_DATE,TXT_AW_MTF_LIST_HEART,TXT_AW_MTF_LIST_STEP,
        TXT_AW_MTF_LIST_CALORIE,TXT_AW_MTF_LIST_STAND};
    MMI_IMAGE_ID_T imgarray[]={res_aw_clock_function_menu_ic_charge,res_aw_clock_function_menu_ic_date,res_aw_clock_function_menu_ic_heart_rate,
        res_aw_clock_function_menu_ic_step_number,res_aw_clock_function_menu_ic_calorie,res_aw_clock_function_menu_ic_stand_number};
    //WatchCOM_TextList_Create(WATCHCOM_LIST_ITEM__ST *list_content_ptr,  uint32 list_total_num, ctrl_id );
    GUILIST_SetMaxItem(ctrl_id, MTF_DIAL_LIST_ITEM_MAX, FALSE);
    for(i=0;i<MTF_DIAL_LIST_ITEM_MAX;i++)
    {
        if(MTFItemTypeArray[curPosition] == i)
        {
            MMI_GetLabelTextByLang(textarray[i], &Str);
#ifdef  ADULT_WATCH_SUPPORT
            AdultWatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1line(ctrl_id, imgarray[i], Str,res_aw_clock_function_menu_ic_done);
#else
            WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str(ctrl_id, imgarray[i], Str, res_aw_clock_function_menu_ic_done, Str2);
#endif
        }
        else
        {
            MMI_GetLabelTextByLang(textarray[i], &Str);
#ifdef  ADULT_WATCH_SUPPORT
            AdultWatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1line(ctrl_id, imgarray[i], Str,NULL);
#else
            WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str(ctrl_id, imgarray[i], Str, NULL, Str2);
#endif
        }
    }

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,curSelection);

}

LOCAL MMI_RESULT_E HandleLauncherEditMTFDialListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	
    MMI_CTRL_ID_T       ctrl_id = MMI_MTF_DIAL_CTRL_STYPE_LIST_ID;

    uint16 index = 0;

    MMI_CheckAllocatedMemInfo();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            InitControlerItemListCtrl();
	        MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {

            break;
        }

        case MSG_FULL_PAINT:
        {

            break;
        }

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif 
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            index = GUILIST_GetCurItemIndex(ctrl_id);
            MMIMTF_SetItemType(curPosition,index);

            MMK_CloseWin(win_id);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            break;
        }
#endif
        case MSG_KEYDOWN_1:
        {
            break;
        }

        case MSG_KEYDOWN_2:
        {
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}


LOCAL BOOLEAN EnterNextDialWin(void)
{
	MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_KEYDOWN_RIGHT, NULL);
	return TRUE;
}
LOCAL BOOLEAN EnterPreDialWin(void)
{
	MMK_SendMsg(MMK_GetFocusWinHandle(), MSG_KEYDOWN_LEFT, NULL);
	return TRUE;
}

LOCAL BOOLEAN EnterEditMTFDialWin(void)
{
	WatchLauncher_Edit_MTFDial_Win_Enter();

	return TRUE;
}
LOCAL BOOLEAN OnUpBtn(void)
{
	curPosition = MTF_DIAL_CONTROLER_POSITION_UP;

	WatchLauncher_Edit_MTFDial_List_Win_Enter();

	return TRUE;
}
LOCAL BOOLEAN OnDownBtn(void)
{
	curPosition = MTF_DIAL_CONTROLER_POSITION_DOWN;

	WatchLauncher_Edit_MTFDial_List_Win_Enter();

	return TRUE;
}
LOCAL BOOLEAN OnLeftBtn(void)
{
	curPosition = MTF_DIAL_CONTROLER_POSITION_LEFT;
	WatchLauncher_Edit_MTFDial_List_Win_Enter();

	return TRUE;
}
LOCAL BOOLEAN OnRightBtn(void)
{
	curPosition = MTF_DIAL_CONTROLER_POSITION_RIGHT;
	WatchLauncher_Edit_MTFDial_List_Win_Enter();

	return TRUE;
}

LOCAL BOOLEAN OnClick(GUI_POINT_T  point,OnMTFClickAction *action){
	for(;;action++)
	{
		if(action->fun==NULL)
			{
			break;
			}
		if(point.x>action->rect.left
			&&point.x<action->rect.right
			&&point.y>action->rect.top
			&&point.y<action->rect.bottom
			)
			{
			return action->fun();
			}
	}
	return FALSE;
}



/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/
	
PUBLIC BOOLEAN OnClickInEditMTFDialWin(GUI_POINT_T  point)
{
	return OnClick(point,ClickActionOnEditMTFWinArray);
}

PUBLIC BOOLEAN OnClickInEditDialWin(GUI_POINT_T  point)
{
	return OnClick(point,ClickActionOnEditWinArray);
}

PUBLIC void WatchLauncher_Edit_MTFDial_Win_Enter(void)
{

    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EDITMTFDIAL_WIN_ID;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(WATCH_LAUNCHER_EDITMTFDIAL_WINTAB, NULL);

}


PUBLIC void WatchLauncher_Edit_MTFDial_List_Win_Enter(void)
{

    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EDITMTFDIAL_LIST_WIN_ID;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(WATCH_LAUNCHER_EDITMTFDIAL_LIST_WINTAB, NULL);

}

PUBLIC void DrawMTFControlerItem( MMI_WIN_ID_T	win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    int index = MTF_DIAL_CONTROLER_POSITION_UP;
    for(index = MTF_DIAL_CONTROLER_POSITION_UP;index < MTF_DIAL_CONTROLER_POSITION_MAX;index++)
    {
        MMI_IMAGE_ID_T           img_id ;
        switch(MTFItemTypeArray[index])
        {
            case MTF_DIAL_ITEM_POWER:
                 {
                    BOOLEAN  is_charge = MMIIDLE_GetIdleWinInfo()->is_charge;
                    if(is_charge ==TRUE)
                    {
                        MTFWINItem[WATCH_ARRAY_CHARGE_INDEX].rect.top = MTFItemPositionArray[index].top;
                        MTFWINItem[WATCH_ARRAY_CHARGE_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                        MTFWINItem[WATCH_ARRAY_CHARGE_INDEX].rect.left= MTFItemPositionArray[index].left;
                        MTFWINItem[WATCH_ARRAY_CHARGE_INDEX].rect.right = MTFItemPositionArray[index].right;
                        AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_CHARGE_INDEX]);
                    }
                    else
                    {
                        MTFWINItem[WATCH_ARRAY_POWSER_INDEX].rect.top = MTFItemPositionArray[index].top;
                        MTFWINItem[WATCH_ARRAY_POWSER_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                        MTFWINItem[WATCH_ARRAY_POWSER_INDEX].rect.left= MTFItemPositionArray[index].left;
                        MTFWINItem[WATCH_ARRAY_POWSER_INDEX].rect.right = MTFItemPositionArray[index].right;
                        AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_POWSER_INDEX]);
                    }
                }
                break;
            case MTF_DIAL_ITEM_DATE:
                MTFWINItem[WATCH_ARRAY_DATE_INDEX].rect.top = MTFItemPositionArray[index].top;
                MTFWINItem[WATCH_ARRAY_DATE_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                MTFWINItem[WATCH_ARRAY_DATE_INDEX].rect.left= MTFItemPositionArray[index].left;
                MTFWINItem[WATCH_ARRAY_DATE_INDEX].rect.right = MTFItemPositionArray[index].right;
                AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_DATE_INDEX]);
                break;
            case MTF_DIAL_ITEM_HEART:
                MTFWINItem[WATCH_ARRAY_HEART_INDEX].rect.top = MTFItemPositionArray[index].top;
                MTFWINItem[WATCH_ARRAY_HEART_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                MTFWINItem[WATCH_ARRAY_HEART_INDEX].rect.left= MTFItemPositionArray[index].left;
                MTFWINItem[WATCH_ARRAY_HEART_INDEX].rect.right = MTFItemPositionArray[index].right;
                AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_HEART_INDEX]);
                break;
            case MTF_DIAL_ITEM_STEPS:
                MTFWINItem[WATCH_ARRAY_STEP_INDEX].rect.top = MTFItemPositionArray[index].top;
                MTFWINItem[WATCH_ARRAY_STEP_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                MTFWINItem[WATCH_ARRAY_STEP_INDEX].rect.left= MTFItemPositionArray[index].left;
                MTFWINItem[WATCH_ARRAY_STEP_INDEX].rect.right = MTFItemPositionArray[index].right;
                AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_STEP_INDEX]);
                break;
            case MTF_DIAL_ITEM_COMBUSTION:
                MTFWINItem[WATCH_ARRAY_CALORIE_INDEX].rect.top = MTFItemPositionArray[index].top;
                MTFWINItem[WATCH_ARRAY_CALORIE_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                MTFWINItem[WATCH_ARRAY_CALORIE_INDEX].rect.left= MTFItemPositionArray[index].left;
                MTFWINItem[WATCH_ARRAY_CALORIE_INDEX].rect.right = MTFItemPositionArray[index].right;
                AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_CALORIE_INDEX]);
                break;
            case MTF_DIAL_ITEM_STAND:
                MTFWINItem[WATCH_ARRAY_STAND_INDEX].rect.top = MTFItemPositionArray[index].top;
                MTFWINItem[WATCH_ARRAY_STAND_INDEX].rect.bottom= MTFItemPositionArray[index].bottom;
                MTFWINItem[WATCH_ARRAY_STAND_INDEX].rect.left= MTFItemPositionArray[index].left;
                MTFWINItem[WATCH_ARRAY_STAND_INDEX].rect.right = MTFItemPositionArray[index].right;
                AdultWatchCOM_PanController(win_id, lcd_dev_info, MTFWINItem[WATCH_ARRAY_STAND_INDEX]);
                break;
            default:
                break;
        }
    }

}




PUBLIC void DrawMTFEditWinControlerItem( MMI_WIN_ID_T	win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    int index = MTF_DIAL_CONTROLER_POSITION_UP;
    for(index = MTF_DIAL_CONTROLER_POSITION_UP;index < MTF_DIAL_CONTROLER_POSITION_MAX;index++)
    {
        MMI_IMAGE_ID_T           img_id ; 
        switch(MTFItemTypeArray[index])
        {
            case MTF_DIAL_ITEM_POWER:
                img_id = res_aw_clock_function_edit_charge;
                break;
            case MTF_DIAL_ITEM_DATE:
                img_id = res_aw_clock_function_edit_date;
                break;
            case MTF_DIAL_ITEM_HEART:
                img_id = res_aw_clock_function_edit_heart_rate;
                break;
            case MTF_DIAL_ITEM_STEPS:
                img_id = res_aw_clock_function_edit_step_number;
                break;
            case MTF_DIAL_ITEM_COMBUSTION:
                img_id = res_aw_clock_function_edit_calorie;
                break;
            case MTF_DIAL_ITEM_STAND:
                img_id = res_aw_clock_function_edit_stand_number;
                break;
            default:
                img_id = res_aw_clock_function_edit_step_number;
                break;
        }
        GUIRES_DisplayImg(PNULL,
                           &(MTFEditItemPositionArray[index]),
                           PNULL,
                           win_id,
                           img_id,
                           &lcd_dev_info
                           );
    }

}




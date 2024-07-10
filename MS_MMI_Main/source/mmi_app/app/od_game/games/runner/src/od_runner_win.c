#include "window_parse.h"
#include "mmk_app.h"
#include "guibutton.h"

#include "guitext.h"
#include "guimsgbox.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "tb_dal.h"
#include "cafcontrol.h"

#include "mmi_textfun.h"
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmk_timer.h"

#include "os_api.h"
#include "mmiset_export.h"
#include "gui_ucs2b_converter.h"
#include "od_game_res_id_def.h"
#include "od_game_nv.h"
#include "od_game_menu.h"
#include "od_runner.h"

#ifdef OD_RUNNER_SUPPORT

#define OD_SET_COLOR(data,r1,g1,b1) do{\
     	data.r = r1;\
     	data.g = g1;\
     	data.b = b1;\
    }while(0)

extern void od_runner_tp();
OD_U16 od_runner_speeds[] = {3,4,5};
#ifdef TOUCH_PANEL_SUPPORT 
GUI_RECT_T softKeyRect={0}; 
#endif
void od_runner_cfg()
{
	OD_RUNNER_CFG_T *cfgPtr = NULL;
	OD_DASH_RES_T *colorsPtr = NULL; 
	OD_U8 i;
	OD_U32 resId[] = {OD_RUNNER_COIN,OD_RUNNER_HUMAN};
	OD_U8 count;

	if(od_game_is_new_game())
	{
		od_runner_exit_game();
	}

	if(od_runner_get_cfg()==NULL)
	{
		cfgPtr = (OD_RUNNER_CFG_T*)od_game_malloc(sizeof(OD_RUNNER_CFG_T));
		if(cfgPtr == NULL)
		{
			return;
		}
		//screen
		cfgPtr->scrH = od_game_get_screen_height();
		cfgPtr->scrW = od_game_get_screen_width();
		if(cfgPtr->scrH > cfgPtr->scrW)	
		cfgPtr->scale = cfgPtr->scrH/160.0f;
		else
		cfgPtr->scale = cfgPtr->scrW/160.0f;			
		cfgPtr->level = od_game_get_current_level();
		count = sizeof(resId)/sizeof(resId[0]);
		for(i=0;i<count;i++){
			cfgPtr->res[i].image_id = resId[i];
			od_game_image_get_dimension(cfgPtr->res[i].image_id,&cfgPtr->res[i].w,&cfgPtr->res[i].h);
		}
		// dash
		colorsPtr = (OD_DASH_RES_T*)od_game_malloc(sizeof(OD_DASH_RES_T));
		if(colorsPtr==NULL) 
		{
			return;
		}
		colorsPtr->count = OD_DASH_COLOR_COUNT;
		OD_SET_COLOR(colorsPtr->colors[0],255,255,255);
		OD_SET_COLOR(colorsPtr->colors[1],0,105,52);
		OD_SET_COLOR(colorsPtr->colors[2],149,97,52);
		OD_SET_COLOR(colorsPtr->colors[3],64,34,15);
		cfgPtr->dash.image_id = (OD_U32)colorsPtr;
		cfgPtr->time = 36;
		cfgPtr->hoffsetY = OD_HUMAN_MOVE_Y_OFFSET*cfgPtr->scale;
		od_runner_init(cfgPtr);
	}
	
}
void od_runner_play_ring()
{
    od_game_play_tone(OD_RUNNER_RING,1);
}
void od_runner_draw_scores_str(OD_S16 x,OD_S16 y,OD_S16 h, OD_U32 scores,OD_BOOL isOver)
{
    OD_U8 str[10] = {0};
    OD_U8 i=0;
    od_game_rect_t rect;
    od_game_color line_color = {50,50,50};
    od_game_color text_color = {0,0,0};
    OD_S32 width,height;

    od_game_string_get_dimension(L"A",1,0,&width, &height);
    rect.left = x;
    rect.right = od_game_get_screen_width();
    rect.top = y-((height-h)>>1)-2;
    rect.bottom = height + y+5;
	if(isOver)
	{
    	od_game_show_popup_bg(rect.left,rect.top,rect.right-1,rect.bottom-1,0,OD_FALSE,line_color);
	}
    sprintf(str,"x%d",scores);
    od_game_draw_text(str,strlen(str),0,&rect,text_color,0,0);
}

void od_runner_show_start_popup_text()
{
    OD_U32 list_of_strings[] = {OD_GAME_RUNNER_TEXT,OD_GAME_GO_TEXT};
    od_game_show_popup_text(list_of_strings,NULL,2);
}
#ifdef TOUCH_PANEL_SUPPORT 

void od_runner_set_tp_sk()
{
	OD_S32 w,h;
	MMI_STRING_T text_str;
	if(softKeyRect.left!=softKeyRect.right)
	{
		return;
	}
	MMI_GetLabelTextByLang(OD_GAME_BACK_TEXT,&text_str);
	od_game_string_get_dimension(text_str.wstr_ptr,text_str.wstr_len,0,&w, &h);
	
	softKeyRect.left=od_game_get_screen_width()-w;
	softKeyRect.top=od_game_get_screen_height()-h;
	softKeyRect.right=od_game_get_screen_width();
	softKeyRect.bottom=od_game_get_screen_height();	
}
#endif
void od_runner_show_game_over_popup()
{
    OD_U32 list_of_strings[] = {OD_GAME_RUNNER_TEXT,OD_GAME_OVER_TEXT};
    od_game_show_popup_text(list_of_strings,NULL,2);
	
#ifdef TOUCH_PANEL_SUPPORT 
	od_game_draw_game_back_button();
	od_runner_set_tp_sk();
#endif

}



PUBLIC MMI_RESULT_E HandleRunnerWinMsg( 
                                      MMI_WIN_ID_T win_id, 
                                      MMI_MESSAGE_ID_E msg_id, 
                                      DPARAM param
                                       )
{

    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	GUI_POINT_T point={0}; 
    switch(msg_id)
    { 
    case MSG_OPEN_WINDOW:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
	    od_runner_cfg();
        od_runner_game_draw();
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭 
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		od_runner_pause_game();
        //od_runner_exit_game();
        break; 
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        od_runner_game_draw();
    break;
        
    case MSG_FULL_PAINT:
    break;
	 
    case MSG_KEYDOWN_CANCEL:
		MMK_CloseWin(win_id);
         break;        
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_5:
       //
       od_runner_start_game();
       //od_runner_pause_game();
        break; 
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_2:
		od_runner_game_jump();
    break;  
    case MSG_KEYDOWN_LEFT: 
    case MSG_KEYDOWN_4:
        //
    break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_6:
        //
       break;
    case MSG_KEYUP_LEFT: 
    case MSG_KEYUP_4:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_6:
        //
        break;           
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_8:
    break;
#ifdef TOUCH_PANEL_SUPPORT 
    case MSG_TP_PRESS_DOWN:
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	if(GUI_PointIsInRect(point,softKeyRect)){	
		MMK_CloseWin(win_id);
	}
	else
	{
		od_runner_tp();
	}
	break;
#endif	

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}



WINDOW_TABLE(OD_GAME_RUNNER_NEW_WIN_TAB ) = 
{       
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)HandleRunnerWinMsg),    
        WIN_ID(OD_GAME_RUNNER_WIN_ID),
        END_WIN
}; 



void od_runner_enter(void)
{		
	MMK_CreateWin((uint32 *)OD_GAME_RUNNER_NEW_WIN_TAB, PNULL);
}

OD_BOOL od_runner_is_free_game()
{
	return OD_TRUE;
}
#endif

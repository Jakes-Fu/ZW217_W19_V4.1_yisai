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
#ifdef OD_CROSSY_ROAD_SUPPORT
OD_U32 od_crossy_road_image_id_list[] =
{
    OD_CROSSY_ROAD_COCK_FULL,
    OD_CROSSY_ROAD_BLUE_TRUCK,
    OD_CROSSY_ROAD_RED_TRUCK,
    OD_CROSSY_ROAD_BONUS,
    OD_CROSSY_ROAD_TREE_TRUNK,
    OD_CROSSY_ROAD_TREE_CROWN,
    OD_CROSSY_ROAD_LOG_TREE
};


OD_U32 od_crossy_road_ring_id_list[] =
{
OD_CROSSY_ROAD_GAME_OVER_RING,
OD_CROSSY_ROAD_COLLECT_RING,   
OD_CROSSY_ROAD_JUMP_RING,
0//,OD_CROSSY_ROAD_BG_RING
};


void od_crossy_road_exit_game()
{
        od_crossy_road_pause_game();
        od_game_stop_ring();
}


void od_crossy_road_start_game();
void od_crossy_road_draw_scores(OD_U32 scores,OD_U32 bonus_count,OD_U16* scores_frames)
{
    OD_U8 str[10] = {0};
    od_game_rect_t rect;
    od_game_color c = {253,114,13};
    OD_S32 width;
    od_game_color text_color = {0,0,0};
    OD_S32 w,h=0;
    OD_S16 bonus_w,bonus_h;
    
    od_game_string_get_dimension(L"A",1,0,&w, &h);

    od_game_image_get_dimension(OD_CROSSY_ROAD_BONUS,&bonus_w,&bonus_h);

    if(h < bonus_h)
    {
        h = bonus_h;
    }
    rect.left = 0;
    rect.right = od_game_get_screen_width();
    rect.top = 0;
    rect.bottom = h + (od_game_get_screen_height()/40);

    
    if(* scores_frames == 0 || (*scores_frames %2) == 0)
    {
        od_game_show_popup_bg(rect.left,rect.top,rect.right-1,rect.bottom-1,0,OD_FALSE,0);
    }
    else
    {
        od_game_fill_rectangle(rect.left,rect.top,rect.right-1,rect.bottom-1,c);
    }

    if(*scores_frames > 0)
    {
        *scores_frames-=1;
    }
    sprintf(str,"%d",scores);
    rect.left+=3;
    od_game_draw_text(str,strlen(str),0,&rect,text_color,0, 0);
    memset(str,0,10);

    sprintf(str,"x%d",bonus_count);
    rect.right-=3;
    width = od_game_draw_text(str,strlen(str),0,&rect,text_color,2,0);
    rect.left = rect.right-bonus_w-width-2;
    rect.top = (rect.bottom - bonus_h)/2;
    rect.right = rect.left +bonus_w-1;
    rect.bottom = rect.top + bonus_h -1;
    od_game_draw_image_id(rect.left,rect.top,OD_CROSSY_ROAD_BONUS,rect);
}


void od_crossy_road_show_start_popup_text()
{
    OD_U32 list_of_strings[5] = {OD_GAME_CROSSY_ROAD_TEXT,OD_GAME_BY_TEXT,OD_GAME_HIPSTER_WHALE,0,OD_GAME_GO_TEXT};
    od_game_show_popup_text(list_of_strings,NULL,5);
}


#if defined(__MMI_MAINLCD_128X160__) || defined(__MMI_MAINLCD_160X128__)
#define OD_CROSSY_ROAD_GRID_SIZE 11
#elif defined(__MMI_MAINLCD_176X220__)|| defined(__MMI_MAINLCD_220X176__)
#define OD_CROSSY_ROAD_GRID_SIZE 14
#elif defined(__MMI_MAINLCD_240X320__)|| defined(__MMI_MAINLCD_320X240__)
#define OD_CROSSY_ROAD_GRID_SIZE 20
#elif defined(__MMI_MAINLCD_320X480__)
#define OD_CROSSY_ROAD_GRID_SIZE 30
#else
#error crossy road not support this screen size!
#endif
void od_crossy_road_init(OD_U8 level,OD_U16 screen_width,OD_U16 screen_height,OD_U8 grid_size);
PUBLIC MMI_RESULT_E HandleOdCrossyRoadWinMsg( 
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
		//关闭按键音
		MMIAPIENVSET_CloseActModeKeyRingSet();
	    od_crossy_road_init(od_game_get_current_level(),od_game_get_screen_width(),od_game_get_screen_height(),OD_CROSSY_ROAD_GRID_SIZE);
        od_crossy_road_game_draw();
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭 
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
		//恢复按键音
		MMIAPIENVSET_ResetActModeKeyRingSet();
		od_crossy_road_exit_game();
        break; 
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		//关闭按键音		
		MMIAPIENVSET_CloseActModeKeyRingSet();
        od_crossy_road_game_draw();
        break;        
    case MSG_FULL_PAINT:
        break;	 
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;        
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_4:    
        od_crossy_road_move_left();
        break;
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_6:    
        od_crossy_road_move_right();
        break;
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_5:
        od_crossy_road_start_game();
        break;
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_2:
        od_crossy_road_move_up();
        break;
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_8:
        od_crossy_road_move_down();
        break;     
    case MSG_KEYUP_LEFT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_2:
    case MSG_KEYUP_4:
    case MSG_KEYUP_6:
    case MSG_KEYUP_8:
        od_crossy_road_stop_move();
        break;         
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


WINDOW_TABLE(OD_GAME_CROSSY_ROAD_WIN_TAB ) = 
{       
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)HandleOdCrossyRoadWinMsg ),    
        WIN_ID(OD_GAME_CROSSY_ROAD_WIN_ID),
        END_WIN
}; 


void od_crossy_road_enter(void)
{		
	MMK_CreateWin((uint32 *)OD_GAME_CROSSY_ROAD_WIN_TAB, PNULL);
}


OD_U32 od_crossy_road_get_timer_count()
{
	return 35;
}
#endif

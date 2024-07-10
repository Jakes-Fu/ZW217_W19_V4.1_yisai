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
#ifdef OD_STICK_HERO_SUPPORT
OD_U32 od_stick_hero_image_id_list[] =
{
    OD_STICK_HERO_ROLE,
    OD_STICK_HERO_BG    
};


OD_U32 od_stick_hero_ring_id_list[] =
{
    OD_STICK_HERO_GAME_OVER_RING,
    OD_STICK_HERO_COLLECT_RING,
    OD_STICK_HERO_FALLEN_RING,
    OD_STICK_HERO_GROW_LOOP_RING
};


void od_stick_hero_exit_game()
{
    od_stick_hero_pause_game();
    od_game_stop_ring();
}


void od_stick_hero_draw_point_popup(OD_S16 x,OD_S16 y)
{
    od_game_rect_t rect;
    od_game_color text_color = {0,0,0};
    OD_U8* str = "+1";
    OD_S32 width,height;

    od_game_string_get_dimension(L"A",1,0,&width, &height);
    rect.left = x -width;
    rect.right = x+width-1;
    rect.top = y - 2*height;
    rect.bottom = y-1;
    od_game_draw_text(str,strlen(str),0,&rect,text_color,1,0);
 }


void od_stick_hero_draw_scores(OD_U32 scores,OD_U16* scores_frames)
{
    OD_U8 str[10] = {0};
    OD_U8 i=0;
    od_game_rect_t rect;
    od_game_color c = {253,114,13};
    od_game_color text_color = {0,0,0};
    OD_S32 width,height;

    od_game_string_get_dimension(L"A",1,0,&width, &height);
    rect.left = 0;
    rect.right = od_game_get_screen_width();
    rect.top = 0;
    rect.bottom = height + (od_game_get_screen_height()/40);


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
    od_game_draw_text(str,strlen(str),0,&rect,text_color,0,0);
}


void od_stick_hero_show_start_popup_text()
{
    OD_U32 list_of_strings[5] = {OD_GAME_STICK_HERO_TEXT,OD_GAME_BY_TEXT,OD_GAME_KETCHAPP_TEXT,0,OD_GAME_GO_TEXT};
    od_game_show_popup_text(list_of_strings,NULL,5);
}


void od_stick_hero_init(OD_U8 level,OD_U16 screen_width,OD_U16 screen_height);
PUBLIC MMI_RESULT_E HandleOdStickHeroWinMsg( 
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
	 od_stick_hero_init(od_game_get_current_level(),od_game_get_screen_width(),od_game_get_screen_height());
        od_stick_hero_game_draw();
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭 
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        od_stick_hero_exit_game();
        break; 
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        od_stick_hero_game_draw();
        break;        
    case MSG_FULL_PAINT:
        break;	 
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;        
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_5:
        od_stick_hero_key_down();
        break; 
    case MSG_KEYUP_OK:
    case MSG_KEYUP_WEB:
    case MSG_KEYUP_5:
    case MSG_KEYPRESSUP_OK:
    case MSG_KEYPRESSUP_WEB:
    case MSG_KEYPRESSUP_5:
        od_stick_hero_key_up();
        break;         
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    {
        if(!MMK_IsFocusWin(win_id)) return ;
        
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);

        od_stick_hero_key_down();
    }
    break;
    case MSG_TP_PRESS_UP:
    {
        if(!MMK_IsFocusWin(win_id)) return ;
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        od_stick_hero_key_up();        
    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}


WINDOW_TABLE(OD_GAME_STICK_HERO_WIN_TAB ) = 
{       
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleOdStickHeroWinMsg ),    
    WIN_ID(OD_GAME_STICK_HERO_WIN_ID),
    END_WIN
}; 


void od_stick_hero_enter(void)
{		
    MMK_CreateWin((uint32 *)OD_GAME_STICK_HERO_WIN_TAB, PNULL);
}

OD_U32 od_stick_hero_get_timer_count()
{
	return 35;
}
#endif

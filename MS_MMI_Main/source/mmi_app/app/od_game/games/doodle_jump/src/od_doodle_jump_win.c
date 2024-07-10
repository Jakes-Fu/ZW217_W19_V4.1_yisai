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
#ifdef OD_DOODLE_JUMP_SUPPORT

OD_U32 od_doodle_jump_image_id_list[] =
{
    OD_DOODLE_DOODLE_FULL_IMAGE,
    OD_DOODLE_DOODLE_FULL_IMAGE,
    OD_DOODLE_SOLID_PLATFORM_IMAGE,
    OD_DOODLE_BREAKABLE_PLATFORM_IMAGE,
    OD_DOODLE_MONSTER_IMAGE,
    OD_DOODLE_ROCKET_IMAGE,
    OD_DOODLE_ROCKET_FOR_DOODLE_IMAGE
};


OD_U32 od_doodle_jump_ring_id_list[] =
{
    OD_DOODLE_JUMP_GAMEOVER_RING,
    0,//OD_DOODLE_JUMP_BG_RING,
    OD_DOODLE_JUMP_JUMP_RING,
    OD_DOODLE_JUMP_JETPACK_RING,
    0//OD_DOODLE_JUMP_MONSTER_RING
};

void od_game_stop_ring();

void od_doodle_jump_exit_game()
{
        od_doodle_jump_pause_game();
        od_game_stop_ring();
}

void od_doodle_jump_draw_bg(OD_S16 width,OD_S16 height,OD_U8 level)
{
    OD_U16 i=0,start_x,start_y;
    OD_U8 gap;
    od_game_color level0_c = {247, 242,236};
    od_game_color level1_c = {82,137,41};
    od_game_color level2_c = {76, 76,76};
    od_game_color c;
    od_game_color level0_line_c = {239,223, 207};
    od_game_color level1_line_c = {82,131, 39};
    od_game_color level2_line_c = {70,70, 70};
    od_game_color line_color;
 #ifdef __MMI_MAINLCD_128X160__
     gap = 4;
     start_x = 2;
     start_y = 2;
 #elif defined(__MMI_MAINLCD_176X220__)
     gap = 5;
     start_x = 2;
     start_y = 3;
 #elif defined(__MMI_MAINLCD_240X320__)
     gap = 6;
     start_x = 3;
     start_y = 4;
 #elif defined(__MMI_MAINLCD_320X480__)
     gap = 9;
     start_x = 3;
     start_y = 6;
 #else
    gap = 4;
    start_x = 2; 
    start_y = 2;
 #endif
    if(level == 0)
    {
        c = level0_c;
        line_color = level0_line_c;
    }
    else if(level == 1)
    {
        c = level1_c;
        line_color = level1_line_c;
    }
    else
    {
        c = level2_c;
        line_color = level2_line_c;
    }
    od_game_fill_rectangle(0,0,width - 1,height- 1,c);

    for(i=start_y;i<height;i+=gap)
    {
        od_game_draw_line(0,i,width-1,i, line_color);
    }
    
    for(i=start_x;i<width;i+=gap)
    {
        od_game_draw_line(i,0,i,height-1,line_color);
    }
}


void od_doodle_jump_draw_scores(OD_U32 scores)
{
    OD_U8 str[10] = {0};
    OD_U8 i=0;
    od_game_rect_t rect;
    od_game_color line_color = {50,50,50};
    od_game_color text_color = {0,0,0};
    OD_S32 width,height;

    od_game_string_get_dimension(L"A",1,0,&width, &height);
    rect.left = 0;
    rect.right = od_game_get_screen_width();
    rect.top = 0;
    rect.bottom = height + (od_game_get_screen_height()/40);

    od_game_show_popup_bg(rect.left,rect.top,rect.right-1,rect.bottom-1,0,OD_FALSE,line_color);
    sprintf(str,"%d",scores);
    od_game_draw_text(str,strlen(str),0,&rect,text_color,0,0);
}


void od_doodle_jump_show_start_popup_text()
{
    OD_U32 list_of_strings[6] = {OD_GAME_DOODLE_JUMP_TEXT,OD_GAME_BY_TEXT,OD_GAME_LIMA_SKY_TEXT,OD_GAME_COPYRIGHT_TEXT,0,OD_GAME_GO_TEXT};
    od_game_show_popup_text(list_of_strings,NULL,6);
}


void od_doodle_jump_init(OD_U8 level,OD_U16 screen_width,OD_U16 screen_height);
PUBLIC MMI_RESULT_E HandleOdDoodleJumpWinMsg( 
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
	    od_doodle_jump_init(od_game_get_current_level(),od_game_get_screen_width(),od_game_get_screen_height());
        od_doodle_jump_game_draw();
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭 
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        od_doodle_jump_exit_game();
        break; 
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        od_doodle_jump_game_draw();
    break;
        
    case MSG_FULL_PAINT:
    break;
	 
    case MSG_KEYDOWN_CANCEL:
	MMK_CloseWin(win_id);
         break;        
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_5:
        od_doodle_jump_start_game();
        break; 
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_2:
    break;  
    case MSG_KEYDOWN_LEFT: 
    case MSG_KEYDOWN_4:
        od_doodle_jump_move_left();
    break;

    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_6:
        od_doodle_jump_move_right();
       break;
    case MSG_KEYUP_LEFT: 
    case MSG_KEYUP_4:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_6:
        od_doodle_jump_stop_move();
        break;           
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_8:
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    {
        if(!MMK_IsFocusWin(win_id)) return ;
        
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);

		if(od_doodle_jump_is_pause())
		{
	      od_doodle_jump_start_game();
		}
		else
		{
			if(point.x < od_game_get_screen_width()/2)
			{
		        od_doodle_jump_move_left();
			}
			else
			{
		        od_doodle_jump_move_right();
			}
		}
    }
    break;
    case MSG_TP_PRESS_UP:
    {
        if(!MMK_IsFocusWin(win_id)) return ;
        od_doodle_jump_stop_move();
    }
    break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}



WINDOW_TABLE(OD_GAME_DOODLE_JUMP_NEW_WIN_TAB ) = 
{       
        WIN_HIDE_STATUS,
        WIN_FUNC((uint32)HandleOdDoodleJumpWinMsg ),    
        WIN_ID(OD_GAME_DOODLE_JUMP_WIN_ID),
        END_WIN
}; 



void od_doodle_jump_enter(void)
{		
	MMK_CreateWin((uint32 *)OD_GAME_DOODLE_JUMP_NEW_WIN_TAB, PNULL);
}

OD_U32 od_doodle_jump_get_timer_count()
{
	return 35;
}
#endif

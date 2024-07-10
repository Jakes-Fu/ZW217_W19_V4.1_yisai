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
#ifdef OD_SUPER_PET_SUPPORT
OD_U32 od_super_pet_image_id_list[] =
{
    OD_SUPER_PET_BORDER,
    OD_SUPER_PET_SUPER_PETS,
    OD_SUPER_PET_EXPLODE,
    OD_SUPER_PET_RAINBOW
};


OD_U32 od_super_pet_ring_id_list[] =
{
    OD_SUPER_PET_GAME_OVER_RING,
    OD_SUPER_PET_WIN_RING,
    OD_SUPER_PET_COMBO_RING,
    OD_SUPER_PET_MATCH_3_RING
};


OD_U8 od_super_pet_get_story_mode_level()
{
    return od_game_get_nvram_level_2();
}

void od_super_pet_set_story_mode_level(OD_U8 level)
{
    od_game_set_nvram_level_2(level);
}


void od_super_pet_exit_game()
{
    od_super_pet_pause_game();
    od_game_stop_ring();
}


void od_super_pet_draw_point_popup(OD_S16 x,OD_S16 y)
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


void od_super_pet_draw_scores(OD_U32 scores,OD_U16 times)
{
    OD_U8 str[20] = {0};
    OD_U8 i=0;
    od_game_rect_t rect;
    od_game_color c = {255,0,0};
    od_game_color text_color = {0,0,0};
    OD_S32 width,height;

    od_game_string_get_dimension(L"A",1,0,&width, &height);
    rect.left = 0;
    rect.right = od_game_get_screen_width()/2-1;
    rect.top = 0;
    rect.bottom = height + (od_game_get_screen_height()/40);


    sprintf(str,"%d",scores);
    od_game_draw_text(str,strlen(str),0,&rect,text_color,1,0);
    rect.left = rect.right+1;
    rect.right = od_game_get_screen_width()-1;
    sprintf(str,"Time:%d",times);
    od_game_draw_text(str,strlen(str),0,&rect,text_color,1,0);
}


void od_super_pet_show_target_popup_text(OD_U8 level,OD_U16 target_scores)
{
    OD_U8* list_of_items[4] ={NULL};
    OD_U8 i=0,count =0;
    MMI_STRING_T text_str ={0};
	
    MMI_GetLabelTextByLang(OD_GAME_SUPER_PET_LEVEL_TEXT,&text_str);
    list_of_items[count] = od_game_get_format_string(text_str.wstr_ptr,text_str.wstr_len,level+1,0);     
    count++;
    list_of_items[count] = od_game_malloc(100);
    MMI_GetLabelTextByLang(OD_GAME_SUPER_PET_TARGET_TEXT,&text_str);
    MMIAPICOM_Wstrncpy((OD_U16 *) list_of_items[count],text_str.wstr_ptr,text_str.wstr_len);
    count++;
    MMI_GetLabelTextByLang(OD_GAME_SUPER_PET_SCORE_TEXT,&text_str);
    list_of_items[count] = od_game_get_format_string(text_str.wstr_ptr,text_str.wstr_len,target_scores,0);     
    count++;
    list_of_items[count] = od_game_malloc(100);
    MMI_GetLabelTextByLang(OD_GAME_SUPER_PET_PLAY_TEXT,&text_str);
    MMIAPICOM_Wstrncpy((OD_U16 *) list_of_items[count],text_str.wstr_ptr,text_str.wstr_len);
    count++;
    od_game_show_popup_text_with_bg_id(NULL,list_of_items,count,OD_SUPER_PET_POPUP_BG);    
    for(i=0;i<count;i++)
    {
        od_game_free(list_of_items[i]);
    }
}


void od_super_pet_show_start_popup_text()
{
    OD_U32 list_of_strings[5] = {OD_GAME_SUPER_PET_TEXT,OD_GAME_BY_TEXT,OD_GAME_SUPER_PET_IP_TEXT,0,OD_GAME_GO_TEXT};
    od_game_show_popup_text_with_bg_id(list_of_strings,NULL,5,OD_SUPER_PET_POPUP_BG);
}


void od_super_pet_draw_popup_with_text_id(OD_U32 str_id)
{
    OD_U32 list_of_strings[3] = {0};
    list_of_strings[1] = str_id;
    od_game_show_popup_text_with_bg_id(list_of_strings,NULL,3,OD_SUPER_PET_POPUP_BG);
}

void od_super_pet_draw_you_win_text()
{
    od_super_pet_draw_popup_with_text_id(OD_GAME_SUPER_PET_YOU_WIN_TEXT);
}

void od_super_pet_draw_time_out_text()
{
    od_super_pet_draw_popup_with_text_id(OD_GAME_SUPER_PET_TIME_OUT_TEXT);
}

void od_super_pet_draw_game_over_text()
{
    OD_U32 list_of_strings[3] = {OD_GAME_SUPER_PET_PRESS_TEXT,OD_GAME_SUPER_PET_FOR_TEXT,OD_GAME_SUPER_PET_NEW_GAME_TEXT};
    od_game_show_popup_text_with_bg_id(list_of_strings,NULL,3,OD_SUPER_PET_POPUP_BG);
    od_game_draw_game_back_button();
}

void od_super_pet_draw_next_level_text()
{
    OD_U32 list_of_strings[3] = {OD_GAME_SUPER_PET_PRESS_TEXT,OD_GAME_SUPER_PET_FOR_TEXT,OD_GAME_SUPER_PET_NEXT_LEVEL_TEXT};
    od_game_show_popup_text_with_bg_id(list_of_strings,NULL,3,OD_SUPER_PET_POPUP_BG);
    od_game_draw_game_back_button();
}

#if defined(__MMI_MAINLCD_128X160__)
#define OD_SUPER_PET_MAX_ROWS 8
#define OD_SUPER_PET_MAX_COLUMNS 6
#define OD_SUPER_PET_BOTTOM_GAP 2
#define OD_SUPER_PET_GRID_GAP 0
#elif defined(__MMI_MAINLCD_176X220__)
#define OD_SUPER_PET_MAX_ROWS 8
#define OD_SUPER_PET_MAX_COLUMNS 6
#define OD_SUPER_PET_BOTTOM_GAP 2
#define OD_SUPER_PET_GRID_GAP 1
#elif defined(__MMI_MAINLCD_320X240__)
#define OD_SUPER_PET_MAX_ROWS 6
#define OD_SUPER_PET_MAX_COLUMNS 8
#define OD_SUPER_PET_BOTTOM_GAP 2
#define OD_SUPER_PET_GRID_GAP 1
#elif defined(__MMI_MAINLCD_240X320__)
#define OD_SUPER_PET_MAX_ROWS 8
#define OD_SUPER_PET_MAX_COLUMNS 6
#define OD_SUPER_PET_BOTTOM_GAP 2
#define OD_SUPER_PET_GRID_GAP 1
#elif defined(__MMI_MAINLCD_320X480__)
#define OD_SUPER_PET_MAX_ROWS 8
#define OD_SUPER_PET_MAX_COLUMNS 6
#define OD_SUPER_PET_BOTTOM_GAP 12
#define OD_SUPER_PET_GRID_GAP 1
#else
#error super pet not support this screen size!
#endif

PUBLIC MMI_RESULT_E HandleOdSuperPetWinMsg( 
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
        od_super_pet_init(od_game_get_current_level(),od_game_get_screen_width(),od_game_get_screen_height(),OD_SUPER_PET_MAX_ROWS,OD_SUPER_PET_MAX_COLUMNS,OD_SUPER_PET_BOTTOM_GAP,OD_SUPER_PET_GRID_GAP);
        od_super_pet_game_draw();
        break;

    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        // 背光被允许关闭 
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        od_super_pet_exit_game();
        break; 
    case MSG_GET_FOCUS:
        // 背光常亮设置
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        od_super_pet_game_draw();
        break;        
    case MSG_FULL_PAINT:
        break;	 
    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;        
    case MSG_KEYDOWN_OK:
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_5:
        od_super_pet_key_down(0);                  
        break; 
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_2:
        od_super_pet_key_down(1);                  
        break;
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_8:
        od_super_pet_key_down(2);                  
        break;     
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_4:    
        od_super_pet_key_down(3);                  
        break;
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_6:    
        od_super_pet_key_down(4);                  
        break;
    case MSG_KEYUP_LEFT:
    case MSG_KEYUP_RIGHT:
    case MSG_KEYUP_UP:
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_2:
    case MSG_KEYUP_4:
    case MSG_KEYUP_6:
    case MSG_KEYUP_8:
    case MSG_KEYPRESSUP_LEFT:
    case MSG_KEYPRESSUP_RIGHT:
    case MSG_KEYPRESSUP_UP:
    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_2:
    case MSG_KEYPRESSUP_4:
    case MSG_KEYPRESSUP_6:
    case MSG_KEYPRESSUP_8:
        od_super_pet_key_up();
        break;         
    case MSG_KEYLONG_UP:
    case MSG_KEYLONG_2:
        od_super_pet_key_press(1);                  
        break;
    case MSG_KEYLONG_DOWN:
    case MSG_KEYLONG_8:
        od_super_pet_key_press(2);                  
        break;     
    case MSG_KEYLONG_LEFT:
    case MSG_KEYLONG_4:    
        od_super_pet_key_press(3);                  
        break;
    case MSG_KEYLONG_RIGHT:
    case MSG_KEYLONG_6:    
        od_super_pet_key_press(4);                  
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}



WINDOW_TABLE(OD_GAME_SUPER_PET_WIN_TAB ) = 
{       
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleOdSuperPetWinMsg ),    
    WIN_ID(OD_GAME_SUPER_PET_WIN_ID),
    END_WIN
}; 


void od_super_pet_enter(void)
{		
    MMK_CreateWin((uint32 *)OD_GAME_SUPER_PET_WIN_TAB, PNULL);
}

OD_U32 od_super_pet_get_timer_count()
{
	return 35;
}


OD_BOOL od_super_pet_is_free_game()
{
	return OD_TRUE;
}
#endif

#ifndef _OD_GAME_MENU_H_
#define _OD_GAME_MENU_H_
#include "od_type_def.h"

typedef enum
{
    OD_GAME_DOODLE_JUMP,
    OD_GAME_CROSSY_ROAD,
    OD_GAME_STICK_HERO,
    OD_GAME_BUBBLE_PARTY_MIX,
    OD_GAME_SUPER_PET,
	OD_GAME_RUNNER, 
    OD_GAME_TYPE_MAX
}OD_GAME_TYPE;

extern OD_U16 od_game_get_screen_width();
extern OD_U16 od_game_get_screen_height();
extern void *od_game_malloc(OD_U32 size);
extern void od_game_free(void *ptr);
extern void od_game_cancel_timer(OD_U8 timer_id);
extern OD_U8 od_game_start_timer(OD_U32 count, void (*callback) (void));
extern void od_game_draw_image_id(OD_S16 x,OD_S16 y,OD_U32 image_id,od_game_rect_t clip_rect);
extern void od_game_image_get_dimension(OD_U32 image_id, OD_S16 * w, OD_S16 * h);
extern void od_game_gdi_layer_unlock_frame_buffer();
extern void od_game_gdi_layer_lock_frame_buffer();
extern OD_U32 od_game_wstr_to_utf8(OD_U8 * utf8_ptr, OD_U32 utf8_buf_len, const OD_U16 * wstr_ptr,OD_U32 wstr_len);
extern OD_U32 od_game_utf8_to_wstr(OD_U16*wstr_ptr,OD_U32 wstr_len,const OD_U8 *utf8_ptr);
extern OD_S32 od_game_rand();
extern void od_game_layer_blt_previous();
extern void od_game_draw_line(OD_S32 x1,OD_S32 y1,OD_S32 x2,OD_S32 y2,od_game_color c);
extern void od_game_fill_rectangle(OD_S32 x1,OD_S32 y1,OD_S32 x2,OD_S32 y2,od_game_color c);
extern void od_game_play_tone(OD_U32 ring_id,OD_U8 type);
extern OD_BOOL od_game_is_new_game();
extern OD_BOOL od_game_is_pause_game();
extern void od_game_set_game_pause(OD_BOOL is_pause);
extern void od_game_show_popup_text(OD_U32* list_of_strings,OD_U8** list_of_items,OD_U16 count);
extern OD_U32 od_game_free_play_once(OD_U16 code);
extern OD_U8* od_game_get_format_string(OD_U16* wstr,OD_U16 wstr_len,OD_S32 d,OD_U8* s);
extern void od_game_gui_push_and_set_clip(od_game_rect_t* clip_rect,OD_S32 x1, OD_S32 y1,OD_S32 x2,OD_S32 y2);
extern void od_game_string_get_dimension(OD_U16* wstr,OD_U16 wstr_len,OD_U8 font_size,OD_S32* text_width,OD_S32* text_height);
#endif

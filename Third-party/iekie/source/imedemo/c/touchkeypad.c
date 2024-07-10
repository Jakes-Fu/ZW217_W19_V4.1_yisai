
/*
 * touchkeypad.c
 *
 * Copyright (C) 2009 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "kmximmapi.h"
#include "touchkeypad.h"

#include "gui.h"
#include "sms.h"

/***************************************************************************************
 * 
 * Macro 
 *
 ***************************************************************************************/

#define DEFAULT_BK_COLOR (KMX_COLOR_RED(0) | KMX_COLOR_GREEN(0) | KMX_COLOR_BLUE(0))

/* Control Characters */
#define WCHAR_BACKSPACE         0x08
#define WCHAR_TAB               0x09
#define WCHAR_RETURN            0x0D
#define WCHAR_SPACE             0x20


typedef struct tag_KEYPAD
{
    KMX_KEYPAD      api;

    int             metric;
    int             x;
    int             y;
    int             width;
    int             height;
    
    int             bottom_line;

    KMX_IMM         imm;
    
    int             is_timer1_actived           : 1;
    int             is_timer2_actived           : 1;
    int             is_keypad_visible           : 1;    

    int             language;
    int             user_data_size;
    char*           user_data_buffer;
    

    /* brew variables */
    int             old_y;
} KEYPAD;

KEYPAD touch_keypad;


/**************************************************************************************
 *
 * Decleration 
 *
 **************************************************************************************/
void kmx_keypad_get_context(
    KMX_KEYPAD*     keypad,
    KMX_CONTEXT*    context);

void kmx_keypad_set_timer(
    KMX_KEYPAD*   keypad,
    int           timer_id,
    int           time);

void kmx_keypad_echo_button(
    KEYPAD*     keypad,
    const KMX_BUTTON* button);

void kmx_keypad_get_text_size(
    KEYPAD*           keypad,
    const KMX_WCHAR*  text,
    int               length,
    int               font_size,
    int*              width,
    int*              height);

void kmx_keypad_draw_text(
    KEYPAD*                 keypad,
    int                     x,
    int                     y,
    const KMX_WCHAR*        text,
    int                     length,
    int                     font_size,
    KMX_COLOR               c);

void kmx_keypad_draw_rect(
    KEYPAD*                 keypad,
    int                     left,
    int                     top,
    int                     right,
    int                     bottom,
    KMX_COLOR               c);

void kmx_keypad_draw_button(
    KEYPAD*                 keypad,
    const KMX_BUTTON*       buttons,
    int                     num_buttons);

void kmx_keypad_handle_notification(
    KEYPAD * keypad, 
    KMX_MSG * notify_msg);


static void keypad_backup();
static void keypad_restore();

static void keypad_on_paint();

/* Slide show */

static void keypad_start_slide();

static void keypad_stop_slide();

static int keypad_is_slide_prepared();

static void keypad_prepare_slide(int direction, const KMX_RECT* slide_area);

static int screen_to_keypad_coords(int screen_x, int screen_y, int * keypad_x, int * keypad_y);

static int keypad_is_sliding();

static void kmx_keypad_draw_text_internal(
    int                     x,
    int                     y,
    const KMX_WCHAR*        text,
    int                     length,
    int                     font_size,
    KMX_COLOR               c);

static void draw_button_image(
    const KMX_BUTTON*       buttons,
    int                     num_buttons);

static void draw_button_label(
    const KMX_BUTTON*   button);

static void on_notify_ime_change(int ime_id, int extra_info);

static int keypad_initialize_imm(void);

/***************************************************************************************
 *
 * implementions
 *
 ***************************************************************************************/

int keypad_update_size()
{
    int org_keypad_y = touch_keypad.y;

    touch_keypad.y = touch_keypad.bottom_line - touch_keypad.height;
    
    if(touch_keypad.y != org_keypad_y)
        return 1;

    return 0;
}

static void keypad_draw_background()
{
    if (!touch_keypad.is_keypad_visible)
        return;

    if(touch_keypad.old_y > touch_keypad.y || touch_keypad.old_y == 0)
    {
        // backup
        keypad_backup();
    }
    else if(touch_keypad.old_y < touch_keypad.y)
    {
        // restore
        keypad_restore();
    }

    kmx_keypad_draw_rect(&touch_keypad, 0, 0, touch_keypad.width, touch_keypad.height, DEFAULT_BK_COLOR);
}

static void keypad_send_imm_message(
    int         code,
    int         param1,
    int         param2)
{
    KMX_MSG message;
    KMX_OUTPUT output;
    int status;
    int size_changed = 0;
    
    memset(&output, 0, sizeof(output));
    
    message.code = code;
    message.param1 = param1;
    message.param2 = param2;
    status = kmx_imm_handle_message(touch_keypad.imm, &message, &output);
    
    if (status & KMX_FLAG_UPDATE_SIZE)
    {
        kmx_imm_get_window_size(touch_keypad.imm, &(touch_keypad.width), &(touch_keypad.height));
        size_changed = keypad_update_size();
        
        keypad_draw_background();        
    }

    if (status & KMX_FLAG_UPDATE_DISPLAY)
    {
        keypad_on_paint();
        if (keypad_is_slide_prepared())
        {
            keypad_start_slide();
        }
        
    }

    if(size_changed && touch_keypad.is_keypad_visible)
    {
        // kewen.komoxo
        RECT r;
        mle_set_pos(&sms_editor, &r);
        //keypad_notify_editor(keypad, KMX_NOTIFY_MSG_VK_HEIGHT_CHANGED, keypad->y, keypad->height);
    }    
    
    if (output.commit_length > 0)
    {
        if (output.commit_length == 1)
        {
            KMX_WCHAR c = output.commit_text[0];
            if (WCHAR_BACKSPACE == c)
            {
                mle_delete_char(&sms_editor);
                //keypad_notify_editor(keypad, KMX_NOTIFY_MSG_BACKSPACE, 1, 0); 
                return;
            }
            mle_insert_char(&sms_editor, output.commit_text[0]);
            //keypad_notify_editor(keypad, KMX_NOTIFY_MSG_INSERT_CHAR, (int)output.commit_text[0], 0);
            
            return;
        }
        output.commit_text[output.commit_length]  = 0;
        mle_insert_text(&sms_editor, output.commit_text, output.commit_length);
        //keypad_notify_editor(keypad, KMX_NOTIFY_MSG_INSERT_STRING, (int)output.commit_text, 0);
        
    }
}

int keypad_handle_message(int code, int param1, int param2)
{
    int handled = 1;
    int x, y;

    switch(code)
    {
    case KMX_MSG_TIMER:
        keypad_send_imm_message(code, param1, 0);
        break;
    case KMX_MSG_PEN_DOWN:
    case KMX_MSG_PEN_MOVE:
    case KMX_MSG_PEN_UP:
        if (screen_to_keypad_coords(param1, param2, &x, &y))
        {
            if (!keypad_is_sliding())            
                keypad_send_imm_message(code, x, y);
        }
        else
        {
            handled = 0;
        }
        break;
    case KMX_MSG_IME_CHANGE:
        keypad_send_imm_message(code, param1, param2);
    default:
        handled = 0;
        break;
    }

    return handled;
}

static void kmx_keypad_on_time1(void)
{
    keypad_handle_message(KMX_MSG_TIMER, KMX_TIMER_1, 0);
}

static void kmx_keypad_on_time2(void)
{
    keypad_handle_message(KMX_MSG_TIMER, KMX_TIMER_2, 0);
}

void kmx_keypad_get_context(
    KMX_KEYPAD*             keypad,
    KMX_CONTEXT*            context)
{
    context->flags = 0;
    context->text_buffer = sms_editor.text_buf;
    context->text_length = sms_editor.text_len;
    context->text_buffer_length = MLE_MAX_TEXT;
    context->caret_position = sms_editor.caret_pos;
}

void kmx_keypad_set_timer(
    KMX_KEYPAD*                 keypad,
    int                     timer_id,
    int                     time)
{
    switch(timer_id) {
    case KMX_TIMER_1:
        if (touch_keypad.is_timer1_actived)
        {
            /* We dont have to stop timer because it is not repeatable for brew */
            touch_keypad.is_timer1_actived = 0;
            gui_disable_timer(GUI_IME_TIMER_ID_1);
        }
        if (time > 0)
        {
            gui_enable_timer(time, GUI_IME_TIMER_ID_1, kmx_keypad_on_time1);
            touch_keypad.is_timer1_actived = 1;
        }
    	break;
    case KMX_TIMER_2:
        if (touch_keypad.is_timer2_actived)
        {
            touch_keypad.is_timer2_actived = 0;
            gui_disable_timer(GUI_IME_TIMER_ID_2);
        }
        if (time > 0)
        {
            gui_enable_timer(time, GUI_IME_TIMER_ID_2, kmx_keypad_on_time2);
            touch_keypad.is_timer2_actived = 1;
        }
        break;
    }
}

void kmx_keypad_echo_button(
    KEYPAD*                 keypad,
    const KMX_BUTTON*       button)
{
    // DO NOTHING
}

void kmx_keypad_get_text_size(
    KEYPAD*                 keypad,
    const KMX_WCHAR*        text,
    int                     length,
    int                     font_size,
    int*                    width,
    int*                    height)
{
    //TODO:the font maybe altered for se. and the use of font_size? how to measure the height of the string?

    *height = gui_get_text_height(font_size);
    *width = gui_get_text_width(font_size, text, length);
}

void kmx_keypad_draw_text(
    KEYPAD*                 keypad,
    int                     x,
    int                     y,
    const KMX_WCHAR*        text,
    int                     length,
    int                     font_size,
    KMX_COLOR               c)
{
   kmx_keypad_draw_text_internal(keypad->x + x, keypad->y + y, text, length, font_size, c);
}

void kmx_keypad_draw_rect(
    KEYPAD*                 keypad,
    int                     left,
    int                     top,
    int                     right,
    int                     bottom,
    KMX_COLOR               c)
{
    RECT r;
    
    r.left = touch_keypad.x + left;
    r.top = touch_keypad.y + top;
    r.right = r.left + (right - left);
    r.bottom = r.bottom + (bottom - top);

    gui_draw_rect(&r, c);
    //IDisplay_DrawRect(idisplay, &r, 0, kmx_convert_color(c), IDF_RECT_FILL);
}

void kmx_keypad_draw_button(
    KEYPAD*                 keypad,
    const KMX_BUTTON*       buttons,
    int                     num_buttons)
{
    int i;

    if(!keypad)
        return;
    
    draw_button_image(buttons, num_buttons);

    for(i = 0; i < num_buttons; i++)
    {
        draw_button_label(&buttons[i]);
    }
}

void kmx_keypad_handle_notification(
    KEYPAD * keypad, 
    KMX_MSG * notify_msg)
{
    if (!keypad)
        return;

    if (!notify_msg)
        return;

    switch(notify_msg->code)
    {
    case KMX_NOTIFY_MSG_IME_CHANGE:
        on_notify_ime_change(notify_msg->param1, notify_msg->param2);
        break;
    case KMX_NOTIFY_MSG_SLIDE:        
        keypad_prepare_slide(notify_msg->param1, (const KMX_RECT*)notify_msg->param2);                    
        break;
/*
    case KMX_NOTIFY_MSG_COMMIT_COMPOSITION:
        keypad_notify_editor(keypad, KMX_NOTIFY_MSG_END_IME_COMP, 0, 0);
        break;
    case KMX_NOTIFY_MSG_SET_COMPOSITION:
        keypad_notify_editor(keypad, KMX_NOTIFY_MSG_SET_IME_COMP, notify_msg->param1, notify_msg->param2);
        break;
*/
    }    
}

static int keypad_initialize_imm(void)
{

}

void keypad_create(int bottom_line)
{
    touch_keypad.user_data_buffer = gui_load_ime_userdata(&(touch_keypad.user_data_size));

    touch_keypad.api.f_draw_button = kmx_keypad_draw_button;
    touch_keypad.api.f_draw_rect   = kmx_keypad_draw_rect;
    touch_keypad.api.f_draw_text   = kmx_keypad_draw_text;
    touch_keypad.api.f_echo_button = kmx_keypad_echo_button;
    touch_keypad.api.f_get_context = kmx_keypad_get_context;
    touch_keypad.api.f_get_text_size = kmx_keypad_get_text_size;
    touch_keypad.api.f_notify_handler = kmx_keypad_handle_notification;

    touch_keypad.x = 0;
    touch_keypad.bottom_line = bottom_line;

    touch_keypad.metric = KMX_METRIC_240;


}

void keypad_destroy()
{
}

void keypad_redraw()
{
}

int keypad_handle_event(int event, unsigned long wParam, unsigned long lParam)
{
    return 1;
}

void keypad_show()
{
}

void keypad_hide()
{
}



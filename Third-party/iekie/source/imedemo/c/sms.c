
/*
 * sms.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "sms.h"
#include "menubar.h"
#include "titlebar.h"
#include "sms_sender.h"
#include "mainmenu.h"
#include "symbol.h"
#include "presshold_detector.h"

#include "cstar_ime.h"

#include "csk2api.h"

extern int cs_initialized;
MULTI_LINE_EDITOR sms_editor;

static PRESSHOLD_DETECTOR sms_presshold_detector;

int sms_input_method;

static int get_next_input_method(int input_method)
{
    int i;

    for (i = 0; i < gui_input_method_count; i++)
    {
        if (gui_input_method_list[i] == input_method)
        {
            if (i + 1 < gui_input_method_count)
                return gui_input_method_list[i+1];
            else
                return gui_input_method_list[0];
        }
    }

    return 0;
}

static void sms_key_handler(int msg, int key, int time)
{
#if !defined(INTERFACE_08_ONLY)
    int presshold;

    switch (key)
    {
        case GUI_KEY_LEFT:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (!cstar_ime_handle_key(key, FALSE))
                    mle_move_caret_backward(&sms_editor);
            }
            return;

        case GUI_KEY_RIGHT:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (!cstar_ime_handle_key(key, FALSE))
                    mle_move_caret_forward(&sms_editor);
            }
            return;

        case GUI_KEY_UP:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (!cstar_ime_handle_key(key, FALSE))
                    mle_move_caret_prev_line(&sms_editor);
            }
            return;

        case GUI_KEY_DOWN:
            if ( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYREPEAT) )
            {
                if (!cstar_ime_handle_key(key, FALSE))
                    mle_move_caret_next_line(&sms_editor);
            }
            return;
    }

    msg = presshold_detector_check(&sms_presshold_detector, msg, key, time);
    if ( !( (msg == GUI_MSG_KEYDOWN) || (msg == GUI_MSG_KEYPRESSHOLD)) )
        return;

    presshold = (msg == GUI_MSG_KEYPRESSHOLD) ? 1 : 0;

    switch (key)
    {
        case GUI_KEY_0:
        case GUI_KEY_1:
        case GUI_KEY_2:
        case GUI_KEY_3:
        case GUI_KEY_4:
        case GUI_KEY_5:
        case GUI_KEY_6:
        case GUI_KEY_7:
        case GUI_KEY_8:
        case GUI_KEY_9:
        case GUI_KEY_L1:
        case GUI_KEY_L2:
        case GUI_KEY_L3:
        case GUI_KEY_R1:
        case GUI_KEY_R2:
        case GUI_KEY_R3:
            cstar_ime_handle_key(key, presshold);
            break;

        case GUI_KEY_STAR:
            if (!cstar_ime_handle_key(key, presshold))
            {
                cstar_ime_close_input_method();
                symbol_show();
            }
            break;

        case GUI_KEY_POUND:
            if (!cstar_ime_handle_key(key, presshold))
            {
                sms_input_method = get_next_input_method(sms_input_method);
                cstar_ime_set_input_method(sms_input_method);
                sms_update_status();
            }
            break;

        case GUI_KEY_OK:
        case GUI_KEY_LSK:
            if (!cstar_ime_handle_key(key, presshold))
            {
                if (sms_editor.text_len > 0)
                {
                    cstar_ime_close_input_method();
                    sms_sender_show(sms_editor.text_buf, sms_editor.text_len);
                }
            }
            break;

        case GUI_KEY_RSK:
            if (!cstar_ime_handle_key(key, presshold))
            {
                if (sms_editor.text_len > 0)
                {
                    if (presshold)
                        mle_clear_all(&sms_editor);
                    else
                        mle_delete_char(&sms_editor);
                    sms_update_status();
                }
                else
                {
                    cstar_ime_close_input_method();

					sms_input_method = 0;
                    mainmenu_show();
                }
            }
            break;
    }
#endif /* !defined(INTERFACE_08_ONLY) */
}

static CS_WCHAR Key2Zhuyin(CS_UINT iKey)
{
							   /*    0       1       2       3       4       5       6       7       8       9 */	
	static CS_WCHAR digit2zy[] = {0x3122, 0x3105, 0x3109, 0x02C7, 0x02CB, 0x3113, 0x02CA, 0x02D9, 0x311A, 0x311E};
	static CS_WCHAR alpha2zy[] = {
	/*    a       b       c       d       e        f     g*/	
		0x3107, 0x3116, 0x310F, 0x310E, 0x310D, 0x3111, 0x3115,
	/*    h i j k l m n*/
		0x3118, 0x311B, 0x3128, 0x311C, 0x3120, 0x3129, 0x3119,
   /*     o p q */
		0x311F, 0x3123, 0x3106, 
	/* r s t*/
	    0x3110, 0x310B, 0x3114,
	/* u v w*/
		0x3127, 0x3112, 0x310A,
	/* x, y, z */
		0x310C, 0x3117, 0x3108
					   };
	                /*    -         ;       ,       .       /     */

	static CS_WCHAR symbo2zy[] = {0x3126, 0x3124, 0x311D, 0x3121, 0x3125};

	CS_WCHAR wChar = 0;
	switch (iKey)
	{
	case '-':
		wChar = 0x3126;
		break;
	case ';':
		wChar = 0x3124;
		break;
	case ',':
		wChar = 0x311D;
		break;
	case '.':
		wChar = 0x3121;
		break;
	case '/':
		wChar = 0x3125;
		break;
	case ' ':
		wChar = 0x02C9;
		break;
	default:
		break;
	}
	if (iKey >= '0' && iKey <= '9')
		wChar = digit2zy[iKey - '0'];
	if (iKey >= 'a' && iKey <= 'z')
		wChar = alpha2zy[iKey - 'a'];

	return wChar;
}

static void sms_char_handler(int key)
{
#if !defined(INTERFACE_08_ONLY)
    if ( cs_initialized && ((sms_input_method == IM_PINYIN_FULLKEY) || (sms_input_method == IM_PINYIN_FULLKEY_TC)))
    {
        int event = 0;
        int msg = 0;
        if (key == '[')     // page up
        {
            event = CS_EVENT_PAGEPREVIOUS;
        }
        else if (key == ']') // page down
        {
            event = CS_EVENT_PAGENEXT;
        }
        else if ((key >= 'a' && key <= 'z') || key == '\'')
        {
            event = CS_EVENT_APPENDCHAR;
            msg = key;
        }
        else if (key >= '1' && key <= '9')
        {
            event = CS_EVENT_SELECTCAND;
            msg = key - '1' + 1;
        }
        else if (key == 8)  // backspace
        {
            event = CS_EVENT_REMOVECHAR;
        }
        else if (key == 96) //escape
        {
            event = CS_EVENT_REMOVEALL;
        }

        cstar_ime_handle_char(event, msg);
    }

    if ( cs_initialized && ((sms_input_method == IM_ZHUYIN_FULLKEY ) || (sms_input_method == IM_ZHUYIN_FULLKEY_SC)))
    {
        int event = 0;
        int msg = 0;
        CS_WCHAR wChar;
        wChar = Key2Zhuyin(key);

        if (key == '[')     // page up
        {
            event = CS_EVENT_PAGEPREVIOUS;
        }
        else if (key == ']') // page down
        {
            event = CS_EVENT_PAGENEXT;
        }
        else if (wChar)
        {
            event = CS_EVENT_APPENDCHAR;
            msg = wChar;
        }
        else if (key == 8)  // backspace
        {
            event = CS_EVENT_REMOVECHAR;
        }
        else if (key == 96) //escape
        {
            event = CS_EVENT_REMOVEALL;
        }

        cstar_ime_handle_char(event, msg);

    }

    if ( cs_initialized && (sms_input_method == IM_ZHUYIN_MT_FULL) )
    {
        int msg = 0;
        if (key >= 'a' && key <= 'z')
            msg = key ;
        else if (key == '\'')
            msg = 'z' + 1;
        else
            return;

        cstar_ime_handle_key(msg, 0);
    }

	if (cs_initialized && ((sms_input_method > IM_LATINFULLKEY_BASE) || sms_input_method == IM_STROKE))
	{
		int msg = key;
		if (key == '\b')
		{
			cstar_ime_handle_key(GUI_KEY_RSK, 0);
			return;
		}

        if (key == '|')
            msg = 0x0F;
		
		cstar_ime_handle_char(CS_EVENT_CHAR, msg);
		//cstar_ime_handle_key(msg, 0);
	}

#endif /* !defined(INTERFACE_08_ONLY) */
}

void sms_show(void)
{
#if !defined(INTERFACE_08_ONLY)
	if (sms_input_method == 0)
		sms_input_method = gui_input_method_list[0];

    mle_paint(&sms_editor);
    sms_update_status();

    presshold_detector_reset(&sms_presshold_detector);
    gui_set_key_handler(sms_key_handler);
    gui_set_char_handler(sms_char_handler);

    cstar_ime_set_input_method(sms_input_method);
#endif
}

void sms_update_status(void)
{
#if !defined(INTERFACE_08_ONLY)
    WCHAR input_method_name[64] = L"\0";

    titlebar_set_title(L"短消息");
    gui_get_input_method_name(sms_input_method, input_method_name);
    titlebar_set_ime_status(input_method_name);
    //titlebar_set_ime_status(gui_get_input_method_name(sms_input_method));

    if (cstar_ime_is_ui_visible())
        menubar_set(L"确认", L"取消");
    else if (sms_editor.text_len > 0)
        menubar_set(L"发送", L"删除");
    else
        menubar_set(L"", L"取消");
#endif
}

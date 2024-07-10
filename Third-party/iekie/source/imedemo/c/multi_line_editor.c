
/*
 * multi_line_editor.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "resource.h"

#include "multi_line_editor.h"
#include "titlebar.h"

#define MLE_LEFT_MARGIN         2
#define MLE_RIGHT_MARGIN        2

#define MLE_CARET_WIDTH         2

static int mle_get_line(MULTI_LINE_EDITOR* editor, int pos)
{
    int i;

    for (i = 0; i < editor->line_count; i++)
    {
        if (pos <= editor->line_pos[i] + editor->line_len[i])
            break;
    }

    return i;
}

static int mle_get_offset(MULTI_LINE_EDITOR* editor, int line, int pos)
{
    return gui_get_text_width(
        editor->font,
        editor->text_buf + editor->line_pos[line],
        pos - editor->line_pos[line]);
}

static int mle_locate_pos(MULTI_LINE_EDITOR* editor, int line, int offset)
{
    int start, end;
    int pos, w;

    start = editor->line_pos[line];
    end = start + editor->line_len[line];

    pos = start;
    w = 0;

    for (;;)
    {
        if (offset <= w)
            break;
        if (pos + 1 > end)
            break;
        pos++;
        w = gui_get_text_width(
            editor->font,
            editor->text_buf + start,
            pos - start);
    }

    return pos;
}

static void mle_parse_line(MULTI_LINE_EDITOR* editor)
{
    int w, i, j;

    w = (editor->pos.right - editor->pos.left) - (MLE_LEFT_MARGIN + MLE_RIGHT_MARGIN);
    editor->line_count = 0;
    i = 0;
    for (;;)
    {
        j = i;
        while (i < editor->text_len)
        {
            if (gui_get_text_width(editor->font, editor->text_buf + j, i + 1 - j) >= w)
                break;
            else
                i++;
        }
        editor->line_pos[editor->line_count] = j;
        editor->line_len[editor->line_count] = i - j;
        editor->line_count++;
        if (i >= editor->text_len)
            break;
    }
}

static void mle_locate_first_line(MULTI_LINE_EDITOR* editor)
{
    int n;

    n = (editor->pos.bottom - editor->pos.top) / editor->line_height;

    if (editor->line_count <= n)
    {
        editor->first_line = 0;
    }
    else
    {
        editor->first_line = editor->caret_line - (n / 2);
        if (editor->first_line < 0)
            editor->first_line = 0;
        else if (editor->first_line + n > editor->line_count)
            editor->first_line = editor->line_count - n;
    }
}

static void mle_lock(
    MULTI_LINE_EDITOR*      editor)
{
    editor->lock++;
}

static void mle_unlock(
    MULTI_LINE_EDITOR*      editor)
{
    editor->lock--;
    if ( (editor->lock == 0) && (editor->dirty) )
    {
        editor->dirty = FALSE;
        mle_paint(editor);
    }
}

static void mle_set_dirty(
    MULTI_LINE_EDITOR*      editor)
{
    editor->dirty = TRUE;
}

void mle_reset(
    MULTI_LINE_EDITOR*      editor)
{
    editor->pos.left    = 0;
    editor->pos.top     = GUI_BAR_HEIGHT;
    editor->pos.right   = GUI_WIDTH;
    editor->pos.bottom  = GUI_HEIGHT - GUI_BAR_HEIGHT;

    editor->text_len    = 0;

    editor->line_count  = 1;
    editor->first_line  = 0;
    editor->line_pos[0] = 0;
    editor->line_len[0] = 0;

    editor->caret_pos   = 0;
    editor->caret_line  = 0;
    editor->comp_len    = 0;
    editor->hold_offset = -1;

    editor->font        = GUI_SYSTEM_FONT;
    editor->line_height = gui_get_text_height(editor->font);

    editor->bk_color    = RGB(255, 255, 210);
    editor->text_color  = RGB(0, 0, 0);
    editor->caret_color = RGB(0, 0, 255);
    editor->comp_color  = RGB(255, 0, 0);

    editor->lock        = 0;
    editor->dirty       = FALSE;
}

void mle_paint(
    MULTI_LINE_EDITOR*      editor)
{
    RECT r;
    int ox, oy;
    int i, x, y;
    int caret_offset, caret_line;
    int comp_start_offset, comp_start_line, comp_break_offset;
    int comp_start, comp_break;
    const WCHAR* text;
    int text_len;
    WCHAR status[16];

    ox = editor->pos.left + MLE_LEFT_MARGIN;
    oy = editor->pos.top;

    /* draw background */

    gui_draw_rect(&editor->pos, editor->bk_color);

    /* draw text lines */

    x = ox;
    y = oy;
    for (i = editor->first_line; i < editor->line_count; i++)
    {
        text = editor->text_buf + editor->line_pos[i];
        text_len = editor->line_len[i];
        if (text_len > 0)
            gui_draw_text(x, y, editor->font, text, text_len, editor->text_color);
        y += editor->line_height;
        if (y >= editor->pos.bottom)
            break;
    }

    /* draw caret */

    caret_line = editor->caret_line;
    caret_offset = mle_get_offset(editor, caret_line, editor->caret_pos);

    r.left   = ox + caret_offset;
    r.top    = oy + (caret_line - editor->first_line) * editor->line_height;;
    r.right  = r.left + MLE_CARET_WIDTH;
    r.bottom = r.top + editor->line_height;

    gui_draw_rect(&r, editor->caret_color);

    /* draw composition text */

    if (editor->comp_len > 0)
    {
        comp_start = editor->caret_pos - editor->comp_len;
        comp_start_line = mle_get_line(editor, comp_start);
        if (comp_start == editor->line_pos[comp_start_line] + editor->line_len[comp_start_line])
            comp_start_line++;
        comp_start_offset = mle_get_offset(editor, comp_start_line, comp_start);

        if (comp_start_line == caret_line)
        {
            y = (comp_start_line - editor->first_line) * editor->line_height;
            gui_draw_text(
                ox + comp_start_offset,
                oy + y,
                editor->font,
                editor->text_buf + comp_start,
                editor->comp_len,
                editor->comp_color);
            gui_draw_line(
                ox + comp_start_offset,
                oy + y + editor->line_height,
                ox + caret_offset,
                oy + y + editor->line_height,
                editor->comp_color);
        }
        else
        {
            comp_break = editor->line_pos[comp_start_line] + editor->line_len[comp_start_line];
            comp_break_offset = gui_get_text_width(
                editor->font,
                editor->text_buf + editor->line_pos[comp_start_line],
                editor->line_len[comp_start_line]);

            y = (comp_start_line - editor->first_line) * editor->line_height;
            gui_draw_text(
                ox + comp_start_offset,
                oy + y,
                editor->font,
                editor->text_buf + comp_start,
                comp_break - comp_start,
                editor->comp_color);
            gui_draw_line(
                ox + comp_start_offset,
                oy + y + editor->line_height,
                ox + comp_break_offset,
                oy + y + editor->line_height,
                editor->comp_color);

            y = (caret_line - editor->first_line) * editor->line_height;
            gui_draw_text(
                ox,
                oy + y,
                editor->font,
                editor->text_buf + comp_break,
                editor->caret_pos - comp_break,
                editor->comp_color);
            gui_draw_line(
                ox,
                oy + y + editor->line_height,
                ox + caret_offset,
                oy + y + editor->line_height,
                editor->comp_color);
        }
    }

    /* draw scrollbar if partial lines visible */


    /* draw virtual button */
    //gui_draw_button(100, 100, KMX_IMAGE_240_ARABIC_ABC);

    /* set title bar editor status */

    wsprintf(status, L"%d/%d", editor->text_len, MLE_MAX_TEXT);
    titlebar_set_editor_status(status);
}

void mle_get_pos(
    MULTI_LINE_EDITOR*      editor,
    RECT*                   pos)
{
    *pos = editor->pos;
}

void mle_set_pos(
    MULTI_LINE_EDITOR*      editor,
    const RECT*             pos)
{
    mle_lock(editor);

    editor->pos = *pos;
    mle_locate_first_line(editor);

    mle_set_dirty(editor);

    mle_unlock(editor);
}

WCHAR mle_get_context(
	MULTI_LINE_EDITOR*      editor)
{
    int pos = editor->caret_pos - editor->comp_len;
	if (editor->text_len > 0 && pos > 0)
		return editor->text_buf[pos - 1];
	else
		return 0;
}

int mle_get_context_string(
    MULTI_LINE_EDITOR*      editor,
    const unsigned short **context,
    int* length)
{
    int pos = editor->caret_pos - editor->comp_len;
	if (editor->text_len > 0 && pos > 0)
    {
        *context = editor->text_buf;
        *length = pos;

        return pos;
    }
    else
    {
        *context = 0;
        *length = 0;

        return  0;
    }

}

void mle_insert_char(
    MULTI_LINE_EDITOR*      editor,
    WCHAR                   chr)
{
    mle_insert_text(editor, &chr, 1);
}

void mle_insert_text(
    MULTI_LINE_EDITOR*      editor,
    const WCHAR*            text,
    int                     text_len)
{
    mle_lock(editor);
    
    mle_end_composition(editor);

    if (text_len > MLE_MAX_TEXT - editor->text_len)
        text_len = MLE_MAX_TEXT - editor->text_len;

    if ( text && (text_len > 0) )
    {
        memmove(
            editor->text_buf + (editor->caret_pos + text_len),
            editor->text_buf + editor->caret_pos,
            sizeof(WCHAR) * (editor->text_len - editor->caret_pos));
        memcpy(
            editor->text_buf + editor->caret_pos,
            text,
            sizeof(WCHAR) * text_len);
        editor->text_len += text_len;
        editor->caret_pos += text_len;

        mle_parse_line(editor);
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}

void mle_clear_all(
    MULTI_LINE_EDITOR*      editor)
{
    mle_reset(editor);
    mle_paint(editor);
}

void mle_delete_char(
    MULTI_LINE_EDITOR*      editor)
{
    mle_lock(editor);

    mle_end_composition(editor);

    if (editor->caret_pos > 0)
    {
        memmove(
            editor->text_buf + (editor->caret_pos - 1),
            editor->text_buf + editor->caret_pos,
            sizeof(WCHAR) * (editor->text_len - editor->caret_pos));
        editor->text_len--;
        editor->caret_pos--;

        mle_parse_line(editor);
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}

void mle_move_caret_backward(
    MULTI_LINE_EDITOR*      editor)
{
    mle_lock(editor);

    mle_end_composition(editor);

    if (editor->caret_pos > 0)
    {
        editor->caret_pos--;
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}

void mle_move_caret_forward(
    MULTI_LINE_EDITOR*      editor)
{
    mle_lock(editor);

    mle_end_composition(editor);

    if (editor->caret_pos < editor->text_len)
    {
        editor->caret_pos++;
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}

void mle_move_caret_prev_line(MULTI_LINE_EDITOR* editor)
{
    int offset;

    mle_lock(editor);

    mle_end_composition(editor);

    if (editor->caret_line > 0)
    {
        if (editor->hold_offset >= 0)
            offset = editor->hold_offset;
        else
            offset = mle_get_offset(editor, editor->caret_line, editor->caret_pos);

        editor->caret_line--;
        editor->caret_pos = mle_locate_pos(editor, editor->caret_line, offset);
        mle_locate_first_line(editor);

        editor->hold_offset = offset;
        mle_set_dirty(editor);
    }

    mle_unlock(editor);
}

void mle_move_caret_next_line(
    MULTI_LINE_EDITOR*      editor)
{
    int offset;

    mle_lock(editor);

    mle_end_composition(editor);

    if (editor->caret_line + 1 < editor->line_count)
    {
        if (editor->hold_offset >= 0)
            offset = editor->hold_offset;
        else
            offset = mle_get_offset(editor, editor->caret_line, editor->caret_pos);

        editor->caret_line++;
        editor->caret_pos = mle_locate_pos(editor, editor->caret_line, offset);
        mle_locate_first_line(editor);

        editor->hold_offset = offset;
        mle_set_dirty(editor);
    }

    mle_unlock(editor);
}

void mle_end_composition(
    MULTI_LINE_EDITOR*      editor)
{
    mle_lock(editor);

    if (editor->comp_len > 0)
    {
        editor->comp_len = 0;
        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}

void mle_set_composition(
    MULTI_LINE_EDITOR*      editor,
    const WCHAR*            comp_text,
    int                     comp_text_len)
{
    mle_lock(editor);

    if (editor->comp_len > 0)
    {
        memmove(
            editor->text_buf + (editor->caret_pos - editor->comp_len),
            editor->text_buf + editor->caret_pos,
            sizeof(WCHAR) * (editor->text_len - editor->caret_pos));
        editor->text_len -= editor->comp_len;
        editor->caret_pos -= editor->comp_len;
        editor->comp_len = 0;

        mle_parse_line(editor);
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    if (comp_text_len > MLE_MAX_TEXT - editor->text_len)
        comp_text_len = MLE_MAX_TEXT - editor->text_len;

    if ( comp_text && (comp_text_len > 0))
    {
        memmove(
            editor->text_buf + (editor->caret_pos + comp_text_len),
            editor->text_buf + editor->caret_pos,
            sizeof(WCHAR) * (editor->text_len - editor->caret_pos));
        memcpy(
            editor->text_buf + editor->caret_pos,
            comp_text,
            sizeof(WCHAR) * comp_text_len);
        editor->text_len += comp_text_len;
        editor->caret_pos += comp_text_len;
        editor->comp_len = comp_text_len;

        mle_parse_line(editor);
        editor->caret_line = mle_get_line(editor, editor->caret_pos);
        mle_locate_first_line(editor);

        mle_set_dirty(editor);
        editor->hold_offset = -1;
    }

    mle_unlock(editor);
}


/*
 * multi_line_editor.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef MULTI_LINE_EDITOR_H
#define MULTI_LINE_EDITOR_H

#include "gui.h"

#define MLE_MAX_TEXT    256
#define MLE_MAX_LINE    MLE_MAX_TEXT

typedef struct tagMULTI_LINE_EDITOR
{
    RECT        pos;

    int         text_len;
    WCHAR       text_buf[MLE_MAX_TEXT];

    int         line_count;
    int         first_line;
    int         line_pos[MLE_MAX_LINE];
    int         line_len[MLE_MAX_LINE];

    int         caret_pos;
    int         caret_line;
    int         comp_len;
    int         hold_offset;

    int         font;
    int         line_height;

    int         bk_color;
    int         text_color;
    int         caret_color;
    int         comp_color;

    int         lock;
    int         dirty;
} MULTI_LINE_EDITOR;

void mle_reset(
    MULTI_LINE_EDITOR*      editor);

void mle_paint(
    MULTI_LINE_EDITOR*      editor);

void mle_get_pos(
    MULTI_LINE_EDITOR*      editor,
    RECT*                   pos);

void mle_set_pos(
    MULTI_LINE_EDITOR*      editor,
    const RECT*             pos);

WCHAR mle_get_context(
    MULTI_LINE_EDITOR*      editor);

int mle_get_context_string(
    MULTI_LINE_EDITOR*      editor,
    const unsigned short **context,
    int* length);

void mle_insert_char(
    MULTI_LINE_EDITOR*      editor,
    WCHAR                   chr);

void mle_insert_text(
    MULTI_LINE_EDITOR*      editor,
    const WCHAR*            text,
    int                     text_len);

void mle_clear_all(
    MULTI_LINE_EDITOR*      editor);

void mle_delete_char(
    MULTI_LINE_EDITOR*      editor);

void mle_move_caret_backward(
    MULTI_LINE_EDITOR*      editor);

void mle_move_caret_forward(
    MULTI_LINE_EDITOR*      editor);

void mle_move_caret_prev_line(
    MULTI_LINE_EDITOR*      editor);

void mle_move_caret_next_line(
    MULTI_LINE_EDITOR*      editor);

void mle_end_composition(
    MULTI_LINE_EDITOR*      editor);

void mle_set_composition(
    MULTI_LINE_EDITOR*      editor,
    const WCHAR*            comp_text,
    int                     comp_text_len);

#endif /* MULTI_LINE_EDITOR_H */

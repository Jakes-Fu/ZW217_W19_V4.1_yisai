
/*
 * emu_option.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef EMU_OPTION_H
#define EMU_OPTION_H

#include "imedemo.h"

extern int g_stroke_layout;
extern int g_active_state;
extern int g_pinyin_mode;
extern int g_candidate_mode;

void execute_emu_option_dialog(HWND parent);

#endif /* EMU_OPTION_H */

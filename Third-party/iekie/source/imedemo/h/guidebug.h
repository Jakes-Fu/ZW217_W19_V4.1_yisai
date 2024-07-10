
/*
 * guidebug.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef GUIDEBUG_H
#define GUIDEBUG_H

#include "imedemo.h"

extern BOOL g_gui_debug;

void ShowGuiDebugWindow(void);

void CloseGuiDebugWindow(void);

void UpdateGuiDebugWindow(const RECT* r);

#endif /* GUIDEBUG_H */

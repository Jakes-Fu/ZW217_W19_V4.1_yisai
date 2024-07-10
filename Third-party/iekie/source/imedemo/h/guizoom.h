
/*
 * guizoom.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef GUIZOOM_H
#define GUIZOOM_H

#include "imedemo.h"

extern BOOL g_gui_zoom;

void ShowGuiZoomWindow(void);

void CloseGuiZoomWindow(void);

void UpdateGuiZoomWindow(const RECT* r);

#endif /* GUIZOOM_H */

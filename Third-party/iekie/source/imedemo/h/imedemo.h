
/*
 * imedemo.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef IMEDEMO_H
#define IMEDEMO_H

#include "config.h"

extern HINSTANCE g_instance;
extern HWND g_main_window;

void emulator_init(void);
void emulator_cleanup(void);
void emulator_reset(void);

#endif /* IMEDEMO_H */

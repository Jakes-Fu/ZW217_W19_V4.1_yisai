
/*
 * titlebar.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "gui.h"

void titlebar_set(
    const WCHAR*        title,
    const WCHAR*        ime_status,
    const WCHAR*        editor_status);

void titlebar_set_title(
    const WCHAR*        title);

void titlebar_set_ime_status(
    const WCHAR*        ime_status);

void titlebar_set_editor_status(
    const WCHAR*        editor_status);

#endif /* TITLEBAR_H */

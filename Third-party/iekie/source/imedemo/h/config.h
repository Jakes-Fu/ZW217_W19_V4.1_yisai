
/*
 * config.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef _MSC_VER
    /* Disable Microsoft Visial C++ warning: unreferenced formal parameter */
    #pragma warning(disable: 4100)
    /* Disable Microsoft Visial C++ warning: named type definition in parentheses */
    #pragma warning(disable: 4115)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used: nameless struct/union */
    #pragma warning(disable: 4201)
    /* Disable Microsoft Visial C++ warning: nonstandard extension used : bit field types other than int */
    #pragma warning(disable: 4214)
    /* Disable Microsoft Visial C++ warning: unreferenced local function has been removed */
    #pragma warning(disable: 4505)
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

#pragma setlocale("chs")

#endif /* CONFIG_H */

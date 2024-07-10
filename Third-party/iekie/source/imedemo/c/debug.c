
/*
 * debug.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 *
 */

#include "config.h"

#ifdef DEBUG

void DebugAssertFailed(const char* file, int line)
{
    WCHAR msg[512];

    wsprintf(msg, L"Assertion failed: line %d, file %hs\r\n", line, file);
    OutputDebugString(msg);

    /* Enter debugger */

    __asm int 3;
}

#endif /* DEBUG */

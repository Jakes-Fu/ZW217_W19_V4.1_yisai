
/*
 * debug.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 *
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef ASSERT
    #undef ASSERT
#endif

#ifdef DEBUG

    void DebugAssertFailed(const char* file, int line);

    #define ASSERT(exp)     (void)( (exp) || (DebugAssertFailed(__FILE__, __LINE__), 0) )

#else

    #define ASSERT(exp)     ((void)0)

#endif /* DEBUG */

#endif /* DEBUG_H */

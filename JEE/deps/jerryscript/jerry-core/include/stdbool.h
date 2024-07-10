/* stdbool.h: ISO/IEC 9899:1999 (C99), section 7.16 */

/* Copyright (C) ARM Ltd., 2002
 * All rights reserved
 * RCS $Revision: 102466 $
 * Checkin $Date: 2006-08-23 11:37:17 +0100 (Wed, 23 Aug 2006) $
 * Revising $Author: drodgman $
 */

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1

  #ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
    //#define bool _Bool        //bruce for compile
    typedef unsigned char bool;	//bruce for compile
    #define true 1
    #define false 0
  #endif

#define strncasecmp(s1, s2, len) CFLCHAR_StrNICmp(s1, s2, len)  //bruce for compile
#define inline __inline
#ifdef WIN32
#define assert                      //bruce for compile
#endif

#endif /* __bool_true_false_are_defined */

/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __COMMON_H_
#define __COMMON_H_ 1

/* ------------------------------------------------------------------------
** Constants
** ------------------------------------------------------------------------ */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1   /* Boolean true value. */
#define FALSE  0   /* Boolean false value. */

#ifndef NULL
  #define NULL  0
#endif

/* -----------------------------------------------------------------------
** Standard Types
** ----------------------------------------------------------------------- */
typedef unsigned char       BOOLEAN;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint;

typedef signed char         int8;
typedef signed short        int16;
typedef signed long int     int32;

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef unsigned int    size_t;

typedef unsigned char       uchar;
typedef volatile unsigned long  vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char  vu_char;


/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#if 0 
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#else
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


#include "os_api.h"
#include "sci_types.h"

//#define ALIGN(x,a)        __ALIGN_MASK((x),(typeof(x))(a)-1)
//#define __ALIGN_MASK(x,mask)  (((x)+(mask))&~(mask))

#endif  /* __COMMON_H_ */

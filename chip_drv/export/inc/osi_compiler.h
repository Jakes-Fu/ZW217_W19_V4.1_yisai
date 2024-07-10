/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _OSI_COMPILER_H_
#define _OSI_COMPILER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//error no
#define	EBUSY 16	/* Device or resource busy */
#define	EINVAL 22	/* Invalid argument */
#define   EINPROGRESS   144
#define   EOPNOTSUPP   145        /* Operation not supported */
#define   ECANCELED    146        /* Operation canceled */


#define CONFIG_CACHE_LINE_SIZE 32 //from hal_confg.h
// macro for alignment attribute
#define OSI_CACHE_LINE_ALIGNED __attribute__((aligned(CONFIG_CACHE_LINE_SIZE)))
#define OSI_ALIGNED(n) __attribute__((aligned(n)))

// macro for nop instruction
#define OSI_NOP __asm volatile("nop")

// macro for compiler memory access barrier
#define OSI_BARRIER() __asm volatile("" ::: "memory")

// do { ... } while (0) is common trick to avoid if/else error
#define OSI_DO_WHILE0(expr) \
    do                      \
    {                       \
        expr                \
    } while (0)

// just a dead loop
#define OSI_DEAD_LOOP OSI_DO_WHILE0(for (;;);)

// Busy loop wait until condition is true
#define OSI_LOOP_WAIT(cond) OSI_DO_WHILE0(while (!(cond));)

// Busy loop wait util condition is true. When polling peripherals, it is
// needed to avoid read peripheral registers without delay, especially
// when the peripheral is connected to a slow bus. This may cause the bus
// is busy to react CPU register read, and other operations are affected.
#define OSI_POLL_WAIT(cond) OSI_DO_WHILE0(while (!(cond)) { OSI_NOP; OSI_NOP; OSI_NOP; OSI_NOP; })

// Loop wait. Return true if cont_true is meet, return false if cond_false is meet.
#define OSI_LOOP_WAIT_IF(cond_true, cond_false) \
    ({                                          \
        bool _waited;                           \
        for (;;)                                \
        {                                       \
            if (cond_true)                      \
            {                                   \
                _waited = true;                 \
                break;                          \
            }                                   \
            if (cond_false)                     \
            {                                   \
                _waited = false;                \
                break;                          \
            }                                   \
        }                                       \
        _waited;                                \
    })
// macro for fourcc tag
#define OSI_MAKE_TAG(a, b, c, d) ((unsigned)(a) | ((unsigned)(b) << 8) | ((unsigned)(c) << 16) | ((unsigned)(d) << 24))
#define OSI_TAG_EMPTY OSI_MAKE_TAG(' ', ' ', ' ', ' ')


// macro for array dimension
#define OSI_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

// macros for 2^n, and 2^n alignment
#define OSI_IS_POW2(v) (((v) & ((v)-1)) == 0)
#define OSI_IS_ALIGNED(v, n) (((unsigned long)(v) & ((n)-1)) == 0)
#define OSI_ALIGN_UP(v, n) (((unsigned long)(v) + (n)-1) & ~((n)-1))
#define OSI_ALIGN_DOWN(v, n) ((unsigned long)(v) & ~((n)-1))
#define OSI_DIV_ROUND(m, n) (((m) + ((n) >> 1)) / (n))
#define OSI_DIV_ROUND_UP(n, m) (((n) + (m)-1) / (m))

// macro for compare two chars ignoring case
#define OSI_CHAR_CASE_EQU(a, b) (((a) | 0x20) == ((b) | 0x20))

// macro to increase the pointer, and return the original pointer
#define OSI_PTR_INCR_POST(p, n) ({uintptr_t _orig = (p); (p) += (n); _orig; })

// pointer (signed) diff, either can be any pointer type
#define OSI_PTR_DIFF(a, b) ((intptr_t)(a) - (intptr_t)(b))

// Macro for variadic argument count
#define OSI_VA_NARGS_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N
#define OSI_VA_NARGS(...) OSI_VA_NARGS_IMPL(0, ##__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// macros for declaration and definition
#define OSI_DEF_CONST_VAR(decl, ...) static const decl = __VA_ARGS__
#define OSI_DEF_GLOBAL_VAR(decl, ...) decl = __VA_ARGS__
#define OSI_DECL_GLOBAL_VAR(decl, ...) extern decl

// macros to convert CPU endian to little/big endian
#define OSI_TO_LE16(v) (v)
#define OSI_TO_LE32(v) (v)
#define OSI_TO_BE16(v) __builtin_bswap16(v)
#define OSI_TO_BE32(v) __builtin_bswap32(v)

// macros to convert CPU endian from little/big endian
#define OSI_FROM_LE16(v) (v)
#define OSI_FROM_LE32(v) (v)
#define OSI_FROM_BE16(v) __builtin_bswap16(v)
#define OSI_FROM_BE32(v) __builtin_bswap32(v)

// macro for 32bits/16bits/8bits register read and write
#define OSI_REG32_WRITE(address, value) *(volatile uint32_t *)(address) = (value)
#define OSI_REG32_READ(address) (*(volatile uint32_t *)(address))
#define OSI_REG16_WRITE(address, value) *(volatile uint16_t *)(address) = (value)
#define OSI_REG16_READ(address) (*(volatile uint16_t *)(address))
#define OSI_REG8_WRITE(address, value) *(volatile uint8_t *)(address) = (value)
#define OSI_REG8_READ(address) (*(volatile uint8_t *)(address))


// macros for min, max. the variable will be accessed only once
#define OSI_MIN(type, a, b)     ( ((a) < (b)) ? (a) : (b) )

//#define OSI_MIN(type, a, b) ({type _a = (type)(a);type _b = (type)(b); _a < _b? _a : _b;})  //compiler failed
#define OSI_MAX(type, a, b) ({ type _a = (type)(a); type _b = (type)(b);  _a > _b? _a : _b; })
#define OSI_ABS(type, a) ({ type _a = (type)(a); _a > 0? _a : -_a; })

// Range [start, end) and region [start, start+size) macros
#define OSI_IS_IN_RANGE(type, a, start, end) ({type _a = (type)(a); type _start = (type)(start); type _end = (type)(end); _a >= _start && _a < _end; })
#define OSI_IS_IN_REGION(type, a, start, size) ({type _a = (type)(a); type _start = (type)(start); type _end = _start + (type)(size); _a >= _start && _a < _end; })
#define OSI_RANGE_INSIDE(type, start1, end1, start2, end2) ({type _s1 = (type)(start1), _e1 = (type)(end1), _s2 = (type)(start2), _e2 = (type)(end2);  _s1 >= _s2 && _e1 <= _e2; })
#define OSI_REGION_INSIDE(type, start1, size1, start2, size2) ({type _s1 = (type)(start1), _e1 = _s1 + (type)(size1), _s2 = (type)(start2), _e2 = _s2 + (type)(size2);  _s1 >= _s2 && _e1 <= _e2; })


#ifdef __cplusplus
#define OSI_EXTERN_C extern "C"
#define OSI_EXTERN_C_BEGIN extern "C" {
#define OSI_EXTERN_C_END }
#else
#define OSI_EXTERN_C
#define OSI_EXTERN_C_BEGIN
#define OSI_EXTERN_C_END
#endif

#endif

#ifndef STD_H
#define STD_H

#ifndef COMMON_H 
typedef unsigned char           u8;
typedef signed   char           i8;
typedef unsigned short          u16;
typedef          short          i16;
typedef unsigned long           u32;
typedef          long           i32;
#endif

typedef unsigned char           boolean;
typedef          char           s8;
typedef          char           ascii;
typedef          short          s16;
typedef unsigned short          Primitive;
/* chyu */
typedef          long           s32;
/* end of chyu */
typedef          u8  *          pu8;
typedef          u16 *          pu16;
typedef          u32 *          pu32;
typedef          i8  *          pi8;
typedef          i16 *          pi16;
typedef          i32 *          pi32;
typedef          s8  *          ps8;
typedef          s16 *          ps16;
typedef          s32 *          ps32;
#define          VOLATILE       volatile
#define          CONST          const
#define STD_NULL                ((void *) 0)
/* #define NULL                    STD_NULL */

/*
#define goto
#define GO
#define END
*/

#define STD_YES                 1
#define STD_NO                  0

#define STD_TRUE                STD_YES
#define STD_FALSE               STD_NO

#define STD_OK                  STD_YES
#define STD_ERROR               STD_NO

#define STD_ON                  STD_YES
#define STD_OFF                 STD_NO

#define STD_POSITIVE            STD_YES
#define STD_NEGATIVE            STD_NO

#define STD_FORMER              (-1)    /* 0 is used for error              */

#define STD_LATTER              STD_YES

#define std_Bit(i)              ((u32) 1<<i)

#define _PROTO(X)               X

typedef struct std_RingBuf
        {
            u8      *bufBegin;          /* buffer address                   */
            u8      *bufEnd;            /* buffer limit                     */
            u32     bufSize;            /* 2, 4, 8, 16, 32, ..., 1024 in KB */
            u8      *begin;             /* next get char address;           */
            u8      *end;               /* next put char address            */
            u32     count;              /* current buffer char. count       */
        }   std_RingBufS;

#endif  /* STD_H */

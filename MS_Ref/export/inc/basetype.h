/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Hantro Products Oy.                             --
--                                                                            --
--      In the event of publication, the following notice is applicable:      --
--                                                                            --
--                   (C) COPYRIGHT 2004 HANTRO PRODUCTS OY                    --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--         The entire notice above must be reproduced on all copies.          --
--                                                                            --
--------------------------------------------------------------------------------
--
--	Abstract : Basic type definitions.
--
---------------------------------------------------------------------------*/


#ifndef BASETYPE_H_INCLUDED
#define BASETYPE_H_INCLUDED


#ifdef __arm
#define VOLATILE volatile
#else
#define VOLATILE
#endif

typedef unsigned char   u8;
typedef signed char     i8;
typedef unsigned short  u16;
typedef signed short    i16;
typedef unsigned int    u32;
typedef signed int      i32;

#ifdef MP4ENC_ARM11
typedef i16 BLOCK;
#else
typedef i32 BLOCK;
#endif

#ifndef NULL
#ifdef  __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#endif  /* BASETYPE_H_INCLUDED */

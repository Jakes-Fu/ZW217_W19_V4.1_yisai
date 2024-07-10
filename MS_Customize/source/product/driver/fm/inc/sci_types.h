/******************************************************************************
 ** File Name:      sci_types.h                                               *
 ** Author:         jakle zhu                                                 *
 ** DATE:           10/22/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains general types and macros that   *
 **         		are of use to all modules.The values or definitions are   *
 ** 				dependent on the specified target.  T_WINNT specifies     *
 **					Windows NT based targets, otherwise the default is for    *
 **					ARM targets.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/22/2001     Jakle zhu     Create.                                      *
 ******************************************************************************/
#ifndef SCI_TYPES_H
#define SCI_TYPES_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#ifdef  TRUE
#undef  TRUE
#endif

#ifdef  FALSE
#undef  FALSE
#endif

#define  TRUE   1   /* Boolean true value. */
#define  FALSE  0   /* Boolean false value. */

#ifndef  NULL
#define  NULL  0
#endif

#define SCI_TRUE                    TRUE       // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value
#define SCI_NULL                    0x0

typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned char		u8;
typedef unsigned short		uint16;
typedef unsigned short		u16;
typedef unsigned long int	uint32;
typedef unsigned long int	u32;
typedef unsigned long int 	uint64;
typedef unsigned long int 	u64;
typedef unsigned int		uint;

typedef signed char			int8;
typedef signed char			s8;
typedef signed short		int16;
typedef signed short		s16;
typedef signed long int		int32;
typedef signed long int		s32;

#define  PUBLIC
#define  SCI_CALLBACK
#define  EQ              == 
#define  NEQ             != 
#define  OR              || 
#define  AND             && 
#define  PNULL           0
#define  LOCAL           static
#define  CONST           const
#define  VOLATILE        volatile
#define  INLINE          __inline
typedef  uint32          BLOCK_ID;
#define  BCD_EXT         uint8
#define  IRQ_FUNC        __irq
#define  PACK            __packed

#define  SCI_ASSERT(_EXP)    {if(!(_EXP)) SCI_Assert(#_EXP, __FILE__, __LINE__);}
#define  SCI_PASSERT(_EXP, INFO)    {if(!(_EXP)) SCI_Assert(INFO, __FILE__, __LINE__);}

/* some usefule marcos */
#define  Bit(_i)        ((u32) 1<<(_i))
#define  MAX( _x, _y )  (((_x) > (_y)) ? (_x) : (_y))
#define  MIN( _x, _y )  (((_x) < (_y)) ? (_x) : (_y))
#define  WORD_LO(_xxx)  ((uint8) ((int16)(_xxx)))
#define  WORD_HI(_xxx)  ((uint8) ((int16)(_xxx) >> 8))
#define  RND8( _x )     ((((_x) + 7) / 8 ) * 8) /*rounds a number up to the nearest multiple of 8 */
#define  UPCASE( _c )   (((_c) >= 'a' && (_c) <= 'z') ? ((_c) - 0x20) : (_c))
#define  DECCHK( _c )   ((_c) >= '0' && (_c) <= '9')

#define  DTMFCHK( _c )  (((_c) >= '0' && (_c) <= '9') ||\
                         ((_c) >= 'A' && (_c) <= 'D') ||\
                         ((_c) >= 'a' && (_c) <= 'd') ||\
                         ((_c) == '*') ||\
                         ((_c) == '#'))

#define  HEXCHK( _c )   (((_c) >= '0' && (_c) <= '9') ||\
                         ((_c) >= 'A' && (_c) <= 'F') ||\
                         ((_c) >= 'a' && (_c) <= 'f'))

#define  ARR_SIZE( _a )  ( sizeof( (_a) ) / sizeof( (_a[0]) ) )

extern void    SCI_Assert(void *exp, void *file, uint32  line);
extern uint32 SCI_TraceLow (const char *x_format, ...);
extern void    SCI_Sleep(uint32 ms);
extern uint32 Delay(uint32 ms);

#ifdef __cplusplus
}
#endif

#endif  /* SCI_TYPES_H */




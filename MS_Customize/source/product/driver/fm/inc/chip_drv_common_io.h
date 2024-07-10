/******************************************************************************
 ** File Name:    chip_drv_common_io.h                                        *
 ** Author:       Steve.Zhan                                                  *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE                NAME            DESCRIPTION                           *
 **  06/05/2010          Steve.Zhan       Create.                             *
 ******************************************************************************/
#ifndef _CHIP_DRV_COMMON_IO_H_
#define _CHIP_DRV_COMMON_IO_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
    /**---------------------------------------------------------------------------*
    **                               Micro Define                                **
    **---------------------------------------------------------------------------*/
typedef uint32  DRV_WORD;
typedef uint8   DRV_BYTE;
typedef uint16  DRV_WORD16;
typedef uint32  DRV_WORD32;
typedef uint64  DRV_WORD64;
typedef uint32  DRV_ADDRWORD;

#define drv_count32 long

//Bit def
#define BIT_0               0x00000001
#define BIT_1               0x00000002
#define BIT_2               0x00000004
#define BIT_3               0x00000008
#define BIT_4               0x00000010
#define BIT_5               0x00000020
#define BIT_6               0x00000040
#define BIT_7               0x00000080
#define BIT_8               0x00000100
#define BIT_9               0x00000200
#define BIT_10              0x00000400
#define BIT_11              0x00000800
#define BIT_12              0x00001000
#define BIT_13              0x00002000
#define BIT_14              0x00004000
#define BIT_15              0x00008000
#define BIT_16              0x00010000
#define BIT_17              0x00020000
#define BIT_18              0x00040000
#define BIT_19              0x00080000
#define BIT_20              0x00100000
#define BIT_21              0x00200000
#define BIT_22              0x00400000
#define BIT_23              0x00800000
#define BIT_24              0x01000000
#define BIT_25              0x02000000
#define BIT_26              0x04000000
#define BIT_27              0x08000000
#define BIT_28              0x10000000
#define BIT_29              0x20000000
#define BIT_30              0x40000000
#define BIT_31              0x80000000
#define BIT_(__n)           (1<<(__n))
#define MASK_(__n,__w)      (((1<<(__w))-1)<<(__n))
#define VALUE_(__n,__v)     ((__v)<<(__n))

typedef uint32 ChipRegT;
#define REG32(x)                        (*((volatile uint32 *)(x)))

typedef uint16 ChipRegT16;
#define REG16(ChipReg)                  (*((volatile ChipRegT16*) (ChipReg)))
#define REG8(ChipReg)                   (*((volatile uint8*) (ChipReg)))
#define CHIP_REG_OR(reg_addr, value)    (*(volatile uint32 *)(reg_addr) |= (uint32)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile uint32 *)(reg_addr) &= (uint32)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile uint32 *)(reg_addr)  = (uint32)(value))
#define SET_BIT(reg,bit)                ((reg) |= (1<<(bit)))
#define CLR_BIT(reg,bit)                ((reg) &= ~(1<<(bit)))
#define TST_BIT(a,b)                    (((a)>>(b))&1)
#define XOR_BIT(a,b)                    ((a) ^= (1<<(b)))
#undef  ARR_SIZE
#define ARR_SIZE(x)                     (sizeof(x) / sizeof(x[0]) )
#undef  MIN
#define MIN(x,y)                        (((x) < (y)) ? (x): (y))
#undef  MAX
#define MAX(x,y)                        (((x) > (y)) ? (x): (y))
#undef  ABS
#define ABS(x)                          (((x) < (0)) ? (-x):(x))
#define SWAP16(X)                       (((X & 0xFF00) >> 8) | ((X & 0x00FF) << 8))
#define SWAP32(X)                       (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#define SWAP_AB(type,a,b)               { type tmp; tmp=a; a=b; b=tmp; }

#define DRV_MACRO_START do {
#define DRV_MACRO_END   } while (0)

#define SCI_DisableIRQ()
#define SCI_DisableFIQ()
#define SCI_RestoreFIQ()
#define SCI_RestoreIRQ()
#if 0
extern uint16 ADI_Analogdie_reg_read (uint32 addr);
extern void   ADI_Analogdie_reg_write (uint32 addr, uint16 data);

#define ANA_REG_OR(reg_addr, value)     \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val |= (uint16)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_MSK_OR(reg_addr, value, msk)        \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (unsigned short)(~(msk)); \
        adi_tmp_val |= (unsigned short)((value)&(msk)); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_AND(reg_addr, value)    \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (unsigned short)(value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_BIC(reg_addr, value)    \
    do{\
        unsigned short adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val &= (unsigned short)(~value); \
        ADI_Analogdie_reg_write(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_SET(reg_addr, value)    ADI_Analogdie_reg_write(reg_addr, (unsigned short)(value))
#define ANA_REG_GET(reg_addr)           ADI_Analogdie_reg_read(reg_addr)
#endif

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End



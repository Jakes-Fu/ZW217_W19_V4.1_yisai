/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
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
#include "sci_types.h"
#include "hwreg_access.h"
#include "efuse.h"
//#include "osi_api.h"
//#include "drv_names.h"
//#include "hal_chip.h"

#define REG_READ1(addr)			(*(volatile unsigned long *)(addr))
#define REG_WRITE1(addr,b)	   {(*(volatile unsigned long *)(addr)) = b;}
#define EFUSE_MAGIC_NUMBER (0x8810)
#define EFUSE_BASE_S (REG_EFUSE_BASE + 0x1000)
#define EFUSE_READ_BLOCK_S(index) REG_READ1(EFUSE_BASE_S + 4 * (index))
#define EFUSE_WRITE_BLOCK_S(index, val) REG_WRITE1(EFUSE_BASE_S + 4 * (index), val)

#define EFUSE_AP_USE_BIT (1 << 1)

#define REG32(x)                                    (*((volatile uint32 *)(x)))
#define REG_TIMER2_BASE           (0x04806000)
#define TIMER2_HWTIMER_CURVAL_L       (REG_TIMER2_BASE + 0x1C)
#define TIMER2_FULL_COUNT        0xFFFFFFFF
#define TIMER_DELAY_US(_x_)\
    {\
        volatile uint32 time_1 = 0, time_2 = 0;\
        volatile uint32 delay_count = 0;\
        delay_count = _x_ * 2;\
        time_1 = REG32(TIMER2_HWTIMER_CURVAL_L);\
        time_2 = time_1 + delay_count;\
        if (time_2 < time_1)\
        {\
            delay_count -= ((TIMER2_FULL_COUNT - time_1) + 1);\
            while(REG32(TIMER2_HWTIMER_CURVAL_L) >= time_1);\
            while(delay_count > REG32(TIMER2_HWTIMER_CURVAL_L));\
        }\
        else\
        {\
            while(time_2 >= REG32(TIMER2_HWTIMER_CURVAL_L));\
        }\
    }
 
PUBLIC void EFUSE_Sleep(
    uint32  time_in_ms      // Number of milliseconds to sleep.
    )
    
{
    uint32 i;

    for (i = 0; i < time_in_ms; i++)
 	    TIMER_DELAY_US(1000);
 	    
}

void halEfuseOpen(void)
{
    REGT_FIELD_CHANGE1(hwp_efuse->efuse_s_en,
                      REG_EFUSE_EFUSE_S_EN_T,
                     s_vdd_en, 1);   
}

void halEfuseClose(void)
{
    REGT_FIELD_CHANGE1(hwp_efuse->efuse_s_en,
                      REG_EFUSE_EFUSE_S_EN_T,
                      s_vdd_en, 0);
}

static BOOLEAN prvEfuseRead(BOOLEAN is_double, uint32_t block_index, uint32_t *val)
{
    if (val == NULL)
        return FALSE;

    REGT_FIELD_CHANGE2(hwp_efuse->efuse_s_en,
                      REG_EFUSE_EFUSE_S_EN_T,
                      s_vdd_en, 1, // though it should be already set
                      double_bit_en_s, is_double);
    *val = EFUSE_READ_BLOCK_S(block_index);
    return TRUE;
}

BOOLEAN halEfuseSingleRead(uint32_t block_index, uint32_t *val)
{
    return prvEfuseRead(FALSE, block_index, val);
}

BOOLEAN halEfuseDoubleRead(uint32_t block_index, uint32_t *val)
{
    return prvEfuseRead(TRUE, block_index, val);
}

static BOOLEAN prvEfuseWrite(BOOLEAN is_double, uint32_t block_index, uint32_t val)
{
    // "s_auto_check_enable" is a nice feature. However, it will trigger hard
    // fault on fail. So, not enable it.
    REG_EFUSE_EFUSE_PW_SWT_T pw_swt;
    
    REGT_FIELD_CHANGE3(hwp_efuse->efuse_s_en,
                      REG_EFUSE_EFUSE_S_EN_T,
                      s_vdd_en, 1, // though it should be already set
                      double_bit_en_s, is_double,
                      s_auto_check_enable, 0);

    hwp_efuse->efuse_s_magic_number = EFUSE_MAGIC_NUMBER;
    hwp_efuse->efuse_s_flag_clr = 0x3f33;


    pw_swt.b.ns_s_pg_en = 1;
    hwp_efuse->efuse_pw_swt = pw_swt.v;
    EFUSE_Sleep(1);
    
    pw_swt.b.efs_enk1_on = 1;
    hwp_efuse->efuse_pw_swt = pw_swt.v;
    EFUSE_Sleep(1);

    EFUSE_WRITE_BLOCK_S(block_index, val);
    EFUSE_Sleep(1);
    
    pw_swt.b.efs_enk1_on = 0;
    hwp_efuse->efuse_pw_swt = pw_swt.v;
    EFUSE_Sleep(1);

    pw_swt.b.efs_enk2_on = 1;
    pw_swt.b.ns_s_pg_en = 0;
    hwp_efuse->efuse_pw_swt = pw_swt.v;
    hwp_efuse->efuse_s_magic_number = 0;

    return (hwp_efuse->efuse_s_err_flag == 0);
}

BOOLEAN halEfuseSingleWrite(uint32_t block_index, uint32_t val)
{
    return prvEfuseWrite(FALSE, block_index, val);
}

BOOLEAN halEfuseDoubleWrite(uint32_t block_index, uint32_t val)
{
    return prvEfuseWrite(TRUE, block_index, val);
}

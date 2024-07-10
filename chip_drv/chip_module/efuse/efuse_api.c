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
#include "hal_efuse_def.h"
//#include "osi_api.h"
//#include "drv_names.h"
//#include "hal_chip.h"

#define false 0
#define true 1

typedef unsigned char		bool;

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
/**
 * Read or write multiple blocks
 */
bool halEfuseRead(halEfuseReadOp_t *params, uint32_t count)
{
   bool opened = false;
   unsigned n = 0;
   bool ok = 0;
   
    if (params == NULL)
        return false;

    //prvEfuseInit();
    
    for (n = 0; n < count; n++)
    {
        bool is_double = (params[n].op == HAL_EFUSE_OP_DOUBLE_READ ||
                          params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
	#if 0
        bool is_write = (params[n].op == HAL_EFUSE_OP_SINGLE_WRITE ||
                         params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        bool ok = prvEfuseReadBlock(is_double,
                                          params[n].block_index,
                                          params[n].rval, &opened);
      #endif
	EFUSE_HAL_Open();
	*(params[n].rval) = EFUSE1_HAL_Read_EX(params[n].block_index, is_double);
       EFUSE_HAL_Close();
    }

    //if (opened)
       // halEfuseClose();
    return true;
}
/**
 * Read or write multiple blocks
 */
bool halEfuseWrite(halEfuseWriteOp_t *params, uint32_t count)
{
   bool opened = false;
   unsigned n = 0;
   bool ok = true;
   
    if (params == NULL)
        return false;

    //prvEfuseInit();
    
    for (n = 0; n < count; n++)
    {
     
        bool is_double = (params[n].op == HAL_EFUSE_OP_DOUBLE_READ ||
                          params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
#if 0
        bool is_write = (params[n].op == HAL_EFUSE_OP_SINGLE_WRITE ||
                         params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
	#endif
	SCI_TraceLow("secureboot test: [%d]is_double:%d, %d\n",n, is_double, count);
	SCI_TraceLow("secureboot test: [%d]block_index:%d\n",n, params[n].block_index);
	SCI_TraceLow("secureboot test: [%d]wval:%x\n",n, params[n].wval);
//#define SECUREBOOT_TEST
	#ifndef SECUREBOOT_TEST
	#if 0
        ok = prvEfuseWriteBlock(is_double,
                                           params[n].block_index,
                                           params[n].wval, &opened);
	#endif
		EFUSE_HAL_Open();
		EFUSE1_HAL_Write_Ex(params[n].block_index, params[n].wval,is_double);
       	EFUSE_HAL_Close();
	#endif
	
	

        //if (ok)
        //{
            //if (opened)
            //    halEfuseClose();
         //   return false;
        //}
    }

   // if (opened)
     //   halEfuseClose();
    return true;
}
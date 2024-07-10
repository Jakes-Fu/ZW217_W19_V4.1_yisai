/******************************************************************************
 ** File Name:      tb_comm.h                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic common operations for TBIOS.  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/

#ifndef _TB_COMM_H
#define _TB_COMM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sprd_reg_int.h"
#include "_hardware.h"
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//@ lin.liu(2005-02-06). CR19097
//  Add get/select the output clock
//-----------------------------------------------------------------------------
// Normal register interfaces
//-----------------------------------------------------------------------------
// Register OR operation
#define TB_REG_OR(reg_addr, value)    *(volatile uint32 *)(reg_addr) |= value
#define TB_REG_AND(reg_addr, value)   *(volatile uint32 *)(reg_addr) &= value
#define TB_REG_SET(reg_addr, value)   *(volatile uint32 *)(reg_addr)  = (value)
#define TB_REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))

#if 0
//@Daniel.Ding,CR:MS00004239,9/3/2003,begin
#ifdef USE_NUCLEUS
// ISR function pointer prototype.
typedef void (* TB_ISR)(void);
#else
// ISR function pointer prototype.
typedef void (* TB_ISR)(uint32);
#endif
//Daniel.end
#endif


// TIMER callback function prototype.
typedef void (* TB_TIMER_CALLBACK)(void);

// Return value.
#define TB_NULL                 0x0
#define TB_SUCCESS              0x00
#define TB_ISR_NULL             0x01
#define TB_NO_CALLBACK          0x02
#define TB_CALLBACK_ASSIGNED    0x03
#define TB_ISR_ASSIGNED         0x0E
#define TB_PTR_ERROR            0x0F
#define TB_INT_ERROR            0x10
#define TB_PARAM_ERROR          0x20

static  void TB_EnableIRQ(uint32 int_num)
{
    if(int_num < 32)
    {
        *((volatile uint32 *) INT_IRQ_EN)   |= (1UL << int_num);
    }
    else
    {
        while(1);
    }
}

static void TB_DisableIRQ(uint32 int_num)
{
    if(int_num < 32)
    {
        *((volatile uint32 *) INT_IRQ_DISABLE)       |= (1UL << int_num);
    }
    else
    {
        while(1);
    }
}

extern uint32 ISR_RegHandler(uint32 int_num, TB_ISR isr_handler);
extern uint32 ISR_UnRegHandler(uint32 int_num);

#endif // _TB_COMM_H

//End of tb_comm.h

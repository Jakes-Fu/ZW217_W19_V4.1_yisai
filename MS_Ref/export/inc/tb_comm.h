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

#ifndef _TB_COMM_H_
#define _TB_COMM_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "isr_drvapi.h"
#include "rtc_drvapi.h"

#ifndef WIN32
#include "sc_reg.h"
#include "sio.h"
#endif

#include "com_drvapi.h"
#include "chip.h"

#include "timer_drvapi.h"
#include "adc_drvapi.h"
#include "tpc_drvapi.h"
#include "kpd_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
    
#if !defined(PLATFORM_UWS6121E)
/*****************************************************************************/
//  Description:    This function Enable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_EnableINT(int_num)   \
    *((volatile uint32 *) INT_IRQ_EN)       |= (1UL << int_num)

/*****************************************************************************/
//  Description:    This function Disable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_DisableINT(int_num)  \
    *((volatile uint32 *) INT_IRQ_DISABLE)  = (1UL << int_num)
#else
/*****************************************************************************/
//  Description:    This function Enable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_EnableINT(int_num)   \
    INT_HAL_IRQEnable((uint32)int_num)

/*****************************************************************************/
//  Description:    This function Disable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_DisableINT(int_num)  \
    INT_HAL_IRQDisable((uint32)int_num)
#endif

#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H) || defined(PLATFORM_SC8800G)|| defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)

/*****************************************************************************/
//  Description:    This function Enable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_EnableFIQ(int_num)   \
    *((volatile uint32 *) INT_FIQ_EN)       |= (1UL << int_num)

/*****************************************************************************/
//  Description:    This function Disable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_DisableFIQ(int_num)  \
    *((volatile uint32 *) INT_FIQ_DISABLE)  = (1UL << int_num)
#elif defined (PLATFORM_SC8800H)
/*****************************************************************************/
//  Description:    This function Enable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_EnableFIQ(int_num)   \
    *((volatile uint32 *) INT_IRQ_EN)       |= (1 << int_num)

/*****************************************************************************/
//  Description:    This function Disable an interrupt
//  Dependency:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
#define TB_DisableFIQ(int_num)  \
    *((volatile uint32 *) INT_IRQ_DISABLE)  = (1 << int_num)
#elif defined (PLATFORM_UWS6121E)
    /*****************************************************************************/
    //  Description:    This function Enable an interrupt
    //  Dependency:
    //  Author:         Richard.Yang
    //  Note:
    /*****************************************************************************/
#define TB_EnableFIQ(int_num)   \
        INT_HAL_FIQEnable((uint32)int_num)
    
    /*****************************************************************************/
    //  Description:    This function Disable an interrupt
    //  Dependency:
    //  Author:         Richard.Yang
    //  Note:
    /*****************************************************************************/
#define TB_DisableFIQ(int_num)  \
        INT_HAL_FIQDisable((uint32)int_num)

#endif
//-----------------------------------------------------------------------------
// Normal register interfaces
//-----------------------------------------------------------------------------
#define TB_REG_OR(reg_addr, value)    *(volatile uint32 *)(reg_addr) |= (uint32)value
#define TB_REG_AND(reg_addr, value)   *(volatile uint32 *)(reg_addr) &= (uint32)value
#define TB_REG_SET(reg_addr, value)   *(volatile uint32 *)(reg_addr)  = (uint32)(value)
#define TB_REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))
//-----------------------------------------------------------------------------
// Memory manage interfaces
//-----------------------------------------------------------------------------
#define TB_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) \
    SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE)

#define TB_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
    SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE)

#define TB_ALLOC(_SIZE) \
    SCI_ALLOC_APP(_SIZE)

#define TB_FREE(_MEM_PTR) \
    SCI_FREE(_MEM_PTR)

//-----------------------------------------------------------------------------
// Assert
//-----------------------------------------------------------------------------
#define TB_ASSERT(_EXP) \
    SCI_ASSERT(_EXP)


#endif // _TB_COMM_H_

//End of tb_comm.h

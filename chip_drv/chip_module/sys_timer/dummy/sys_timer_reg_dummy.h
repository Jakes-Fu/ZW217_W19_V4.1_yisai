/******************************************************************************
 ** File Name:    sys_timer_reg_v5.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/12/2010    hao.liu    Create.                                     *
 ******************************************************************************/
#ifndef _SYS_TIMER_REG_DUMMY_H_
#define _SYS_TIMER_REG_DUMMY_H_
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

//------------------------------------------------------------------------------
//Register Name       : SYST_ALARM
//Register Offset     : 0x0000
//Register Description:        System timer alarm
//    syst_alarm                      : [31:0 ] System timer alarm value: 
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define SYST_ALARM_MSK                         (0xUL)

//------------------------------------------------------------------------------
//Register Name       : SYST_VALUE
//Register Offset     : 0x0004
//Register Description:        System timer value
//    syst_value                      : [31:0 ] System timer value
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define SYST_VALUE_MSK                         (0xUL)

//------------------------------------------------------------------------------
//Register Name       : SYST_INT
//Register Offset     : 0x000C
//Register Description:        System timer interrupt
//    syst_int_pls_sts                : [4    ] System timer interrupt pulse    
//                                       status 
//    syst_int_clr                    : [3    ] System timer Interrupt clear 
//    syst_int_mask_sts               : [2    ] System timer interrupt masked   
//                                       status
//    syst_int_raw_sts                : [1    ] System timer interrupt raw      
//                                       status 
//    syst_int_en                     : [0    ] System timer interrupt enable
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define SYST_INT_PLS_STS                       (BIT_4)
#define SYST_INT_CLR                           (BIT_3)
#define SYST_INT_MASK_STS                      (BIT_2)
#define SYST_INT_RAW_STS                       (BIT_1)
#define SYST_INT_EN                            (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : SYST_VALUE_SHDW
//Register Offset     : 0x000C
//Register Description:        Shadow of SYST_VALUE
//    syst_value_shdw                 : [31:0 ] Shadow of SYST_VALUE
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define SYST_VALUE_SHDW_MSK                    (0xUL)

//#define SYSTEM_CURRENT_CLOCK (*((volatile uint32 *)SYST_VALUE_SHDW) & 0xFFFFFFFF)
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

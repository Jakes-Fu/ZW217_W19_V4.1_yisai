/******************************************************************************
 ** File Name:      usim_outport.h                                            *
 ** Author:         yongxia.zhang
 ** DATE:           04/28/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the outport interface.                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/28/2004     yongxia.zhang    Create.                                   *
 ******************************************************************************/

#ifndef _USIM_OUTPORT_H_
#define _USIM_OUTPORT_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     TRACE and ASSERT                                      *
 **---------------------------------------------------------------------------*/

#define USIM_TRACE_LOW  SCI_TRACE_LOW
#define USIM_ASSERT     SCI_ASSERT

/**---------------------------------------------------------------------------*
 **                      Interrupt handler register                           *
 **---------------------------------------------------------------------------*/
// @rick.zhang type disagreement with TB_ISR defined in tb_comm.h
//Interrupt Number.
//#define TB_SIM_INT 10

// @rick.zhang type disagreement with TB_ISR defined in tb_comm.h
//ISR function pointer prototype.
//typedef void (* TB_ISR)(void);

//Interrupt handler register func
#define USIMDRV_RegHandler(isr_handler)\
    ISR_RegHandler(TB_SIM_INT,isr_handler)

#endif // _USIM_OUTPORT_H


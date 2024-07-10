/******************************************************************************
 ** File Name:      syscnt_drv.c                                                 *
 ** Author:         Nick.zhao                                                  *
 ** DATE:           01/20/2007                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic interfaces of System Counter     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/20/2007     Nick.zhao        Create.                                   *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "os_import.h"
#include "chip_plf_export.h"
#include "syscnt_drvapi.h"
#include "fiq_drvapi.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Local Data Structure                              *
 **---------------------------------------------------------------------------*/
#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H)||defined(PLATFORM_SC8800G)
#define SYS_ALM_REG     SYS_ALM
#endif

typedef enum
{
    SC_NOT_USED,
    SC_USED
} SYS_CNT_E;

typedef struct
{
    SYSCNT_CALLBACK  callback;
    uint32   param;
} SYSCNT_FUNC_T;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
SYS_CNT_E  s_Syscnt_state;
SYSCNT_FUNC_T s_Syscnt_func;

/*********************************************************************/
//  Description: Initialize the System counter
//  Input:
//      param         not use
//  Return:
//      None
//  Note:
//      None
/*********************************************************************/
void Syscnt_Init()
{
    uint32 status;


    //Init the s_Syscnt_state
    s_Syscnt_state = SC_NOT_USED;

    //Disable SysCnt interrupt and clear syscnt interrupt
    REG32 (SYS_CTL) &=~ (BIT_0);
    REG32 (SYS_CTL) |= (BIT_3);
    //Enable Sys cnt FIQ
	
    CHIPDRV_EnableFIQINT (TB_SYST_INT);
	
    status = FIQ_RegHandler (TB_SYST_INT, Syscnt_ISR);
    SCI_PASSERT ( (FIQ_SUCCESS == status || FIQ_ISR_ASSIGNED == status),/*assert verified*/
                  ("%s, %d, fiq regist status: %d", __MODULE__, __LINE__, status));

}
/*********************************************************************/
//  Description: Set the syscnt timer
//  Input:
//      time     The time to be set (ms)
//      st_fun   The callback function
//      param    The callback function's parameter
//  Return:
//      0        Successfully
//  Note:
//      None
/*********************************************************************/
uint32 Syscnt_SetTimer (uint32 time, SYSCNT_CALLBACK st_fun, uint32 param)
{
    uint32 temp;

    SCI_ASSERT (time>=MIN_SYS_TIMER_TIME);/*assert verified*/
    SCI_ASSERT (NULL != st_fun);/*assert verified*/

    if ( (REG32 (SYS_ALM_REG)) <= (REG32 (SYS_CNT0)))
    {
        s_Syscnt_state = SC_NOT_USED;
    }

    SCI_ASSERT (s_Syscnt_state == SC_NOT_USED);/*assert verified*/

    s_Syscnt_state = SC_USED;

    //Set alarm time
    temp = REG32 (SYS_CNT0);
    REG32 (SYS_ALM_REG) = temp + time;

    //Register callback
    s_Syscnt_func.callback = st_fun;
    s_Syscnt_func.param = param;

    //Enable interrupt
    REG32 (SYS_CTL) |= BIT_0;

    return 0;

}

/*********************************************************************/
//  Description: Clear the current timer
//  Input:
//  Return:
//  Note:
//      None
/*********************************************************************/
void Syscnt_ClearTimer (void)
{

    //Disable SysCnt interrupt and clear syscnt interrupt
    REG32 (SYS_CTL) &=~ (BIT_0);
    REG32 (SYS_CTL) |= (BIT_3);

    //Init the s_Syscnt_state
    s_Syscnt_state = SC_NOT_USED;

    //Clear callback
    s_Syscnt_func.callback = NULL;
    s_Syscnt_func.param = 0;


    return;

}

/*********************************************************************/
//  Description: the Syscnt timer's isr
//  Input:
//      none
//  Return:
//      none
//  Note:
//      None
/*********************************************************************/
void Syscnt_ISR (uint32 int_num)
{
    s_Syscnt_state = SC_NOT_USED;

    //Call the callback
    s_Syscnt_func.callback (s_Syscnt_func.param);
    //Clear the interrupt
    REG32 (SYS_CTL) = (BIT_0|BIT_3);

}


#ifdef   __cplusplus
}
#endif

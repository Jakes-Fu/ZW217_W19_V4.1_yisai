/******************************************************************************
 ** File Name:      sys_timer_phy_v5.c                                               *
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
}


#ifdef   __cplusplus
}
#endif

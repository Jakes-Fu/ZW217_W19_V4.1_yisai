/******************************************************************************
 ** File Name:      sdram_phy.c                                                 *
 ** Author:                                                          *
 ** DATE:                                                           *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:               .*
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2007                      Create.                                   *
 ******************************************************************************/

#include "os_api.h"
#include "chip_plf_export.h"
#include "sdram_drvapi.h"
#ifdef PLATFORM_SC6800H
#include "./v0/emc_reg_v0.h"
#endif

/******************************************************************************
                          Struct define
******************************************************************************/

/******************************************************************************
                          Global Variables
******************************************************************************/
#pragma arm section zidata = "ZIDATA",rwdata = "RWDATA",rodata = "RODATA"

LOCAL SDRAM_TIMING_PARA_T s_sdram_timing_para = {0};

#pragma arm section code = "CODE"
/******************************************************************************/
//  Description:   Intialize SDRAM parameters
//  Parameter:
//      pPara    the address of SDRAM parameters
//  Return:
//      NULL
/******************************************************************************/
PUBLIC void SDRAM_InitPara (SDRAM_TIMING_PARA_T_PTR timing_ptr)
{

    SCI_ASSERT (timing_ptr != NULL);/*assert verified*/

    //Copy SDRAM para to local
    SCI_MEMCPY (&s_sdram_timing_para, timing_ptr, sizeof (SDRAM_TIMING_PARA_T)) ;

}

/******************************************************************************/
//  Description:   Change SDRAM controller settings according the mcu clk
//  Parameter:
//      ahb_clk    the AHB clk
//  Return:
//      NULL
/******************************************************************************/
PUBLIC void SDRAM_SetParam (uint32 ahb_clk)
{
    uint32 i,prescale;
    uint32 temp;

    if (0 == s_sdram_timing_para.row_ref_max)
    {
        SCI_PASSERT (0, ("SDRAM parameter is invalid or SDRAM_InitPara() is not called"));/*assert verified*/
    }

    temp = (1000000000 >> 20) / (ahb_clk >> 20);

    prescale = s_sdram_timing_para.row_ref_max / (4 * temp) - 10;
    REG32 (EXT_MEM_INI) = ( (prescale & 0xff) <<14) |BIT_11;

    //Delay some time
    for (i=0; i<100; i++) {}

    return;
}

#pragma arm section code

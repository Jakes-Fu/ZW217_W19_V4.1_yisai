/******************************************************************************
 ** File Name:      main.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/08/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Richard.Yang     Create.                                   *
 ** 16/09/2003     Xueliang.Wang    Modify for SM5100B module.                *
 **				    CR4013				                                      *
 ******************************************************************************/

#define _MAIN_ENTRY
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include <string.h>
#include <stdio.h>

#include "sci_types.h"
#include "os_api.h"
#include "chip_init_drvapi.h"
#include "spzip_api.h"
//#include "mmu_drvapi.h"

#ifdef MUX_SUPPORT
#include "mux_all.h"
#endif
/*lint -esym(752, SCI_InitAppTasks) */
// extern functions delaims
extern uint32 SCI_InitAtc(void);
extern uint32 SCI_InitAppTasks(void);

#pragma arm section code = "FPU_AONRAM"
#if defined(PLATFORM_UWS6121E)
#ifdef WIN32
void __FPU_Enable(void) {}
#else
__asm void __FPU_Enable(void)
{
    ARM
    //Permit access to VFP/NEON, registers by modifying CPACR
    MRC     p15,0,R1,c1,c0,2
    ORR     R1,R1,#0x00F00000
    MCR     p15,0,R1,c1,c0,2
    //Ensure that subsequent instructions occur in the context of VFP/NEON access permitted
    ISB        //Enable VFP/NEON
    VMRS    R1,FPEXC
    ORR     R1,R1,#0x40000000
    VMSR    FPEXC,R1        //Initialise VFP/NEON registers to 0
    MOV     R2,#0        //Initialise D16 registers to 0
    VMOV    D0, R2,R2
    VMOV    D1, R2,R2
    VMOV    D2, R2,R2
    VMOV    D3, R2,R2
    VMOV    D4, R2,R2
    VMOV    D5, R2,R2
    VMOV    D6, R2,R2
    VMOV    D7, R2,R2
    VMOV    D8, R2,R2
    VMOV    D9, R2,R2
    VMOV    D10,R2,R2
    VMOV    D11,R2,R2
    VMOV    D12,R2,R2
    VMOV    D13,R2,R2
    VMOV    D14,R2,R2
    VMOV    D15,R2,R2
    #if 0
    VMOV    D16,R2,R2
    VMOV    D17,R2,R2
    VMOV    D18,R2,R2
    VMOV    D19,R2,R2
    VMOV    D20,R2,R2
    VMOV    D21,R2,R2
    VMOV    D22,R2,R2
    VMOV    D23,R2,R2
    VMOV    D24,R2,R2
    VMOV    D25,R2,R2
    VMOV    D26,R2,R2
    VMOV    D27,R2,R2
    VMOV    D28,R2,R2
    VMOV    D29,R2,R2
    VMOV    D30,R2,R2
    VMOV    D31,R2,R2
    #endif
    VMRS    R2,FPSCR
    LDR     R3,=0x00086060 //Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.
    AND     R2,R2,R3
    VMSR    FPSCR,R2
    BX      LR
}
#endif
#endif
#pragma arm section code

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif


//@Daniel.Ding,CR:MS00005266,11/12/2003,begin
#ifdef USE_NUCLEUS

#else
    #ifndef WIN32

#if !defined(NANDBOOT_SUPPORT) && defined(MULTI_BIN_SUPPORT)
extern uint32 Image$$EXEC_KERNEL_IMAGE1$$Base;/*lint !e27 !e19*/
extern uint32 Image$$EXEC_KERNEL_IMAGE1$$Length;/*lint !e27 !e19*/
extern uint32 BYTE_HEAP_ADDR[];                 /*lint !e27 !e19*/
extern uint32 LZMA_ClrUnzipRawStatus(void);     /*lint !e27 !e19*/
extern uint32 LZMA_WaitingFinish(uint32 timeout);    /*lint !e27 !e19*/
extern BOOLEAN SPIFLASH_IsOldUNID(void);        /*lint !e27 !e19*/
//extern void MMU_DmaCacheSync (uint32 bufAddr, uint32 bufSize, DMABUFFER_DIRECTION_E dir);

#define MAX_LOAD_TIME_OUT  1000
/*lint -esym(552, g_lzma_dps_knl_err_cnt) */
uint32 g_lzma_dps_knl_err_cnt = 0;

#pragma arm section code = "LOAD_RAM_KERNEL"
uint32 SCI_LoadRamKernel(void)
{
    //uint32 timeout = 0;
    uint32 unzip_data_len = 0;
    uint8* pbuf = NULL;

    LZMA_INPUT_T  in  = {0};
    LZMA_OUTPUT_T out = {0};
	in.zip_buf_addr   = LZMA_GetBzpBinPtr(LZMA_BZP_IMG);
    in.zip_buf_len    = LZMA_GetBzpBinSize(LZMA_BZP_IMG);

	unzip_data_len = LZMA_GetBzpDecSize(LZMA_BZP_IMG);
	if( unzip_data_len > ((uint32)&Image$$EXEC_KERNEL_IMAGE1$$Length ))/*lint !e27 !e19*/
	{
	   SCI_PASSERT(0, ("Unzip Kernel Out Of Ram Size!"));/*assert verified*/
	}
    in.unzip_buf_addr = (uint8*)&Image$$EXEC_KERNEL_IMAGE1$$Base;/*lint !e27 !e19*/
	in.unzip_buf_len = unzip_data_len;

#if ((defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)) && !defined(MULTI_BIN_RAM_32MBIT_LEVEL1))
    /*
    Pay attention! BYTE_HEAP_ADDR[] maybe < 2M, so here need to decompress
    code directly through SPI Flash by LZMA in 32MBIT_LEVEL1 comdition.
    */
    if((SPIFLASH_IsOldUNID()) && (0x1C == (((uint32)in.zip_buf_addr) & 0x1F)))
    {
        g_lzma_dps_knl_err_cnt++;
        pbuf = (uint8*)((((uint32)BYTE_HEAP_ADDR+31) & 0xFFFFFFE0));
        SCI_MEMCPY(pbuf, in.zip_buf_addr, in.zip_buf_len);
        in.zip_buf_addr = pbuf;
        LZMA_ClrUnzipRawStatus();

        MMU_DmaCacheSync((uint32)pbuf, ((in.zip_buf_len + 31) & 0xFFFFFFE0), 0);

        if(LZMA_SUCCESS != LZMA_UnZip(&out, &in))
        {
           SCI_PASSERT(0, ("Unzip Error!"));/*assert verified*/
        }

        if((LZMA_SUCCESS != LZMA_GetUnZipStatus()) || (SCI_SUCCESS != LZMA_WaitingFinish(1)))
        {
            g_lzma_dps_knl_err_cnt = 0xFFFFFFFF;
            SCI_PASSERT(0, ("Unzip Error!"));/*assert verified*/
        }

        SCI_MEMSET(pbuf, 0, in.zip_buf_len);

       	return 0;
    }
#endif

    if(LZMA_SUCCESS != LZMA_UnZip(&out, &in))
    {
       SCI_PASSERT(0, ("Unzip Error!"));/*assert verified*/
    }

	return 0;
}
#pragma arm section code
#endif



    int main(void)
    {


		//Debug_Test = 0;
		//while(Debug_Test == 0);

#if defined(PLATFORM_UWS6121E)
        __FPU_Enable();
#endif
        // Phase1 of chip driver intialization. It initializes critical resource
        // of chip before the refbase starts to boot.

        CHIPDRV_InitOnPhase1 ();

#if !defined(NANDBOOT_SUPPORT) && defined(MULTI_BIN_SUPPORT)
		SCI_LoadRamKernel();
#endif
        SCI_KernelEnter();
    }
#endif

#endif
//Daniel.end.
/*****************************************************************************/
// Description :    This function initialize application tasks and resources.
// Global resource dependence :
// Author :         Xueliang.Wang
// Note :
/*****************************************************************************/
//@Daniel.Ding,CR:MS00005266,11/12/2003,begin
#ifdef USE_NUCLEUS
    #ifdef HTW_INTEGRATION     //use this macro to buile this file for  .
        //NULL
    #else
        PUBLIC void SCI_InitApplication(uint32 mode)
        {
            //@Daniel.Ding,CR:MS00006039,12/4/2003,begin
            SCI_InitAtc();
            // Do nothing.
        }
    #endif
#else   //ThreadX
    PUBLIC void SCI_InitApplication(uint32 mode)
    {
        // User could add initialization of application sources
        // and create their tasks.

		#ifdef MUX_SUPPORT
        MUX_Init();
#endif/*#ifdef _MUX_ENABLE_*/

   		#ifndef _ATC_ONLY

			// Create Application task. Such as MMI task.
			SCI_InitAppTasks();
			SCI_InitAtc();

    	#else
    		// Create AT task.
			SCI_InitAtc();

    	#endif
    }
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

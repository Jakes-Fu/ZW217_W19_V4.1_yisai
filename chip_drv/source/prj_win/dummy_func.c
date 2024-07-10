/******************************************************************************
 ** File Name:      dummy_func.c                                              *
 ** Author:         Lin.liu                                                   *
 ** DATE:           09/04/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Lin.liu          Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "cmddef.h"
#include "diag.h"
#include "Chng_freq_drvapi.h"
#include "mmu_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*
//
// Added by lin.liu for EFS test(2004-05-14).
// These function defined in efs_rpc.c, please add this file to ref_base.a
//
void EFS_RegisterDiagCmdRoutine( void );
void EFS_UnRegisterDiagCmdRoutine( void );


// This is called by diag task to register the callback routine.
void SCI_RegisterDiagCmdRoutine( void )
{
    EFS_RegisterDiagCmdRoutine();
}

// This is called by diag task to un-register the callback routine.
void SCI_UnRegisterDiagCmdRoutine( void )
{
    EFS_UnRegisterDiagCmdRoutine();
}
*/
void PowerButton_SetMode (BOOLEAN  bGPIO_Mode)
{
}

BOOLEAN RM_GetPinTestFlag (void)  // Return the flag.
{
    return FALSE;
}

BOOLEAN RM_GetCalibrationMode (void)
{
    return FALSE;
}

uint32 RM_GetUart0Mode (void)
{
    return 0;
}

void RM_SetCalibrationMode (
    BOOLEAN is_calibration          //calibraton mode flag to set
)
{
}

void CHNG_FREQ_SetArmClk (uint32 handler, CHNG_FREQ_INDEX_E freq_type_index)
{
}

uint32 CHNG_FREQ_GetReqHandler (const char *handler_name)
{
	return 0x46524551;
}

void CHNG_FREQ_RestoreARMClk (uint32 handler)
{
}

uint32 CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode)
{
	return 0;
}

uint32 CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode)
{
	return 0;
}

void SCI_EnableArmHold ( BOOLEAN is_enable )
{
}

void MMU_DmaCacheSync (uint32 bufAddr, uint32 bufSize, DMABUFFER_DIRECTION_E dir)
{
}

void INT_HAL_IRQEnable (uint32 logic_num)
{
}

void INT_HAL_IRQDisable (uint32 logic_num)
{
}


typedef void    *SCI_SEMAPHORE_PTR;

PUBLIC void ISR_ForDebugCheckDelayOp (uint32 value, SCI_SEMAPHORE_PTR sem_ptr)
{}
PUBLIC void INT_ForDebugCheckMemOp (uint32 value)
{}

uint32 ISR_UnRegHandler (
    uint32 logic_num   // Interrupt number
)
{}

typedef enum
{
    ISR_DONE = 0x0,
    CALL_HISR = 0xe05a05a5//magic number

} ISR_EXE_T;

// ISR function pointer prototype.
typedef ISR_EXE_T (* TB_ISR) (uint32);

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

#define SCI_PRIORITY_KERNEL         0

//hisr
typedef enum
{
    CHIPDRV_HISR_PRIO_0 = SCI_PRIORITY_KERNEL,//The highest priority
    CHIPDRV_HISR_PRIO_1 = SCI_PRIORITY_KERNEL + 1,
    CHIPDRV_HISR_PRIO_2 = SCI_PRIORITY_KERNEL + 2,

} CHIPDRV_HISR_PRI_E;

typedef void hisr_func_t (uint32 count, void *data);



uint32 ISR_RegHandler_Ex (
    uint32 logic_num,      // Interrupt number
    TB_ISR isr_handler,   // Interrupt service routine pointer.

    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void              *pHisrData
)
{
}

void TM_SendTestPointRequest (uint32 param1, uint32 param2)
{
}


#ifdef WRE_SUPPORT
PUBLIC void WreInitForAssert(void)
{

}

PUBLIC void WreAddDll(char* ap_name_ptr,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end)
{

}

PUBLIC void WreDelDll(char* ap_name_ptr,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end)
{

}

#endif

/*****************************************************************************/
// Description :   Internal RAM is control by ARM/DSP
// Global resource dependence :
// Author :        windy.wang
// Note :        
/*****************************************************************************/
PUBLIC void DSP_HAL_IRAMSwitchtoARM(BOOLEAN is_enable)
{
}

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Init.
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC void Shm_Data_Init(){

}

//dummy for WIN32
PUBLIC void ADI_init_AUDIO(){
    
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/* diag_routine_sp7100.c  */

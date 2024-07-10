/******************************************************************************
 ** File Name:      prod_outport.h                                              *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           12/28/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Dummy function.                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/28/2006     Liangwen.Zhen    Create.	                                  *
 ******************************************************************************/

#ifndef  _PROD_OUTPORT_H
#define  _PROD_OUTPORT_H
#include "os_api.h"
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void SCI_AfterChangeClk( uint32  clk );
PUBLIC void SCI_BeforeEnterDeepSleep( void );
PUBLIC void SCI_BeforeEnterHalfDeepSleep(void);
PUBLIC void SCI_AfterExitDeepSleep( void );
PUBLIC void SCI_OpenCameraPower(void);
PUBLIC void SCI_BeforeAssertReset(POWER_RESTART_CONDITION_E restart_condition);
PUBLIC BOOLEAN SCI_IsCloseSDCard(void);    
PUBLIC void PowerButton_SetMode( BOOLEAN  bGPIO_Mode );
PUBLIC int16 SCI_PreHandleKeypadInterrupt( uint16  scan_code );
PUBLIC void SCI_PrePowerOff( void );
PUBLIC void SCI_SetCloseSDCard(BOOLEAN 	is_close );
/*****************************************************************************/
//  Description:   	This function is a temporary dummy function and will be 
//                  removed later. It is used in EFS.  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 REF_GetDspParaSize(void);
/*****************************************************************************/
//  Description:   	This function is a temporary dummy function and will be 
//                  removed later. It is used in EFS.  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 REF_GetCalibrationParaSize(void);
/*****************************************************************************/
//  Description:    Dummy function for ATC library.
//  Author:         Benjamin.Wang(Ask to Shijun.Cui)
//	Note:           
/*****************************************************************************/
PUBLIC void ATC_ExInit(void);

PUBLIC void Register_MMI_Test_CmdRoutine( void );

/*****************************************************************************/
//  Description:   	to configure SD spi bus for customer
//
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SD_OpenSPIBusCustomer(BOOLEAN b_open);
 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _PROD_OUTPORT_H

// End of prod_outport.h

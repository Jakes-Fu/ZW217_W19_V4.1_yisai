/******************************************************************************
 ** File Name:      Layer1_diag.h                                             *
 ** Author:                                                      		      *
 ** DATE:                                                                     *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/
#ifndef _LAYER1_DIAG_H
#define _LAYER1_DIAG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

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

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/

/*****************************************************************************/
//  Description:    Register Diag cmd for calibration
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/ 						
PUBLIC void Layer1_RegCaliDiagHandle( void );

/*****************************************************************************/
//  Description:    Diag handle signal from layer1
//	Global resource dependence:
//  Author:         y.j.ke
//	Note:
/*****************************************************************************/    
PUBLIC void Layer1_HandleLayer1Signal(xSignalHeader  sig_ptr);

/*****************************************************************************/
//   Description:   Layer1_HandleDspDebug()
//	Global resource dependence:
//   Author:        tom.fu according to cr160057
//	Note:          this function is implemented only at 8800s now
/*****************************************************************************/    
extern PUBLIC void Layer1_HandleDspDebug( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    );
#ifdef _LAYER1_TRANSMITMAXPWR_CALI     //bug174740
/*****************************************************************************/
//   Description:   set tranmission maxpower parameters 
//   Global resource dependence:
//   Author:        
//   Note:
/*****************************************************************************/    
extern void L1Diag_SetRfTxCtrl(BOOLEAN is_tx_on, uint16 band, uint16 arfcn, uint32 power_level);

/*****************************************************************************/
//   Description:   active or deactive layer1 calibration task
//   Global resource dependence:
//   Author:        
//   Note:
/*****************************************************************************/    
extern void L1Diag_CaliActDeact(BOOLEAN is_act);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif	//_LAYER1_DIAG_H
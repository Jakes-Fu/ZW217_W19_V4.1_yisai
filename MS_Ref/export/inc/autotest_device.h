

/******************************************************************************
 ** File Name:    keypad.h                                                  *
 ** Author:         Richard Yang                                              *
 ** DATE:           19/09/2011                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 19/09/2011     Jackey.Ling     Create.                                   *
 ******************************************************************************/

#ifndef _AUTOTEST_DEVICE_H
#define _AUTOTEST_DEVICE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tb_dal.h"
#include "cmddef.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef uint32 (*DIAG_AUTOTEST_CALLBACK)(     
    uint8 **dest_ptr,       
    uint16 *dest_len_ptr,   
    const uint8 *src_ptr,   
    uint16 src_len          
    );

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/


/*****************************************************************************/
//	Description:	this function is used to init the autotest_device's callback func array.
//									
//	Global resource dependence: 			  
//	Author: 		Jackey.Ling
//	Note:			
/*****************************************************************************/
PUBLIC void DIAG_InitAutotestCallback( void );


/*****************************************************************************/
//  Description:    this function is used to register the autotest_device's callback func.
//                                  
//	Global resource dependence:               
//  Author:         Jackey.Ling
//  Note:           
/*****************************************************************************/
PUBLIC void DIAG_RegisterAutotestCallback( uint32  type,  DIAG_AUTOTEST_CALLBACK  routine );


/*****************************************************************************/
//  Description:    this function is used to unregister the autotest_device's callback func.
//                                  
//	Global resource dependence:                              
//  Author:         Jackey.Ling
//  Note:           
/*****************************************************************************/
PUBLIC void DIAG_UnRegisterAutotestCallback( uint32  type );


/*****************************************************************************/
//  Description:    this function is used to register the autotest_device's module.
//                                  
//	Global resource dependence:                              
//  Author:         Jackey.Ling
//  Note:           
/*****************************************************************************/
PUBLIC void Register_DeviceAutoTest_CmdRoutine (void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* End autotest_device.h*/


/******************************************************************************
 ** File Name:      diag_ex.h                                           *
 ** Author:        gloria.he                                              *
 ** DATE:           2015.07.06                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************


 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.07.09   gloria.he          Create.                               *
 ******************************************************************************/


#ifndef _DIAG_EX_H
#define _DIAG_EX_H

 #include "os_api.h"
 #include "sci_types.h"
 
  /**---------------------------------------------------------------------------*
 **                         data structures                               *
 **---------------------------------------------------------------------------*/
 typedef uint32 (*DIAG_EX_CALLBACK)(     
    uint8 **dest_ptr,       
    uint16 *dest_len_ptr,   
    const uint8 *src_ptr,   
    uint16 src_len          
    );
 /**---------------------------------------------------------------------------*
 **                         Function declaration                               *
 **---------------------------------------------------------------------------*/

 LOCAL uint32 HandleDiagEx( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    );
PUBLIC void DIAG_InitExCallback( void );
PUBLIC void Register_Ex_CmdRoutine (void);
PUBLIC void DIAG_RegisterExCallback( uint32  type,  DIAG_EX_CALLBACK  routine );
PUBLIC void DIAG_UnRegisterExCallback( uint32  type );

#endif



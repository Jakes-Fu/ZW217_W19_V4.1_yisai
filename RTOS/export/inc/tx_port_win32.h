/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2001 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                                                   */
/*  11440 West Bernardo Court               info@expresslogic.com         */
/*  Suite 366                               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** ThreadX Component                                                     */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    tx_port.h                                        Win32/Microsoft    */ 
/*                                                           4.0          */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    William E. Lamie, Express Logic, Inc.                               */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make the ThreadX      */ 
/*    real-time kernel function identically on a variety of different     */ 
/*    processor architectures.  For example, the size or number of bits   */ 
/*    in an "int" data type vary between microprocessor architetures and  */ 
/*    even C compilers for the same microprocessor.  ThreadX does not     */ 
/*    directly use native C data types.  Instead, ThreadX creates its     */ 
/*    own special types that can be mapped to actual data types by this   */ 
/*    file to gaurantee consistency in the interface and functionality.   */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  11-01-1999     William E. Lamie         Initial Win32 Microsoft       */
/*                                            Version 3.0                 */
/*  01-28-2001     William E. Lamie         Changed version string and    */ 
/*                                            comments for version 4.0.   */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef TX_PORT
#define TX_PORT


/* Define various constants for the port.  */ 

#define TX_MINIMUM_STACK            200     /* Minimum stack size       */
#define TX_INT_DISABLE              1       /* Disable interrupts value */
#define TX_INT_ENABLE               0       /* Enable interrupt value   */
#define TX_IRQ_DISABLE              1        /* Disable IRQ interrupts   */
#define TX_IRQ_ENABLE               0        /* Enable IRQ interrupts    */
#define TX_FIQ_DISABLE              1        /* Disable FIQ interrupts   */
#define TX_FIQ_ENABLE               0        /* Enable FIQ interrupts    */


/* Define ThreadX specific types for the port.  */ 

//typedef void                        VOID;
typedef void *                      VOID_PTR;
typedef char                        CHAR;
typedef char *                      CHAR_PTR;
typedef unsigned char               UCHAR;
typedef unsigned char *             UCHAR_PTR;
typedef int                         INT;
/* typedef int *                       INT_PTR;  Win32 doesn't like this!  */
typedef unsigned int                UINT;
/* typedef unsigned int *              UINT_PTR; Win32 doesn't like this!  */
typedef long                        LONG;
typedef long *                      _LONG_PTR;
#if !defined(__GNUC__)
#if _MSC_VER >= 1000
typedef long *                      LONG_PTR_V100;
#else
typedef long *                      LONG_PTR;
#endif
#endif
typedef unsigned long               ULONG;
typedef unsigned long *             _ULONG_PTR;
#if !defined(__GNUC__)
#if _MSC_VER >= 1000
typedef unsigned long *             ULONG_PTR_V100;
#else
typedef unsigned long *             ULONG_PTR;
#endif
#endif

/* Define register constants for the port.  These definitions are 
   prioritized in the order they are defined.  In other words, REG_1
   is assigned to the most used variable, while REG_4 is assigned to
   the least used variable.  */

#define REG_1                       register
#define REG_2                       register
#define REG_3                       register
#define REG_4                       register
#define REG_5                       register


/* Define the port extension field of the thread control block.  Nothing 
   additional is needed for this port so it is defined as white space.  */

#define TX_THREAD_PORT_EXTENSION    HANDLE tx_win32_thread_handle; \
									DWORD  tx_win32_thread_id; \
									HANDLE tx_win32_thread_run_semaphore; \
									UINT   tx_win32_suspension_type; \
									UINT   tx_win32_int_disabled_flag;


/* Define ThreadX interrupt lockout and restore macros for protection on 
   access of critical kernel information.  The restore interrupt macro must 
   restore the interrupt posture of the running thread prior to the value 
   present prior to the disable macro.  In most cases, the save area macro
   is used to define a local function save area for the disable and restore
   macros.  */

/* In Win32, this is basically just critical section manipulation.  */
#include "windows_sci_os.h"

#define TX_INTERRUPT_SAVE_AREA      

#define TX_DISABLE                 EnterCriticalSection(&_tx_win32_critical_section); 
#define TX_RESTORE                 LeaveCriticalSection(&_tx_win32_critical_section);

#define TX_IRQ_SAVE_AREA            
#define TX_DISABLE_IRQ              EnterCriticalSection(&_tx_win32_critical_section);
#define TX_RESTORE_IRQ              LeaveCriticalSection(&_tx_win32_critical_section);
#define TX_FIQ_SAVE_AREA            
#define TX_DISABLE_FIQ              EnterCriticalSection(&_tx_win32_critical_section);
#define TX_RESTORE_FIQ              LeaveCriticalSection(&_tx_win32_critical_section);


/* Define the version ID of ThreadX.  This may be utilized by the application.  */

#ifdef  TX_THREAD_INIT
CHAR                            _tx_version_id[] = 
                                    "Copyright (c) 1996-2001 Express Logic Inc. * ThreadX Win32/Microsoft Version G4.0.4.0 *";
#else
extern  CHAR                    _tx_version_id[];
#endif

extern CRITICAL_SECTION		_tx_win32_critical_section;
extern HANDLE				_tx_win32_semaphore;
extern ULONG			    _tx_win32_global_int_disabled_flag;

#define  MAX_TX_THREAD_CLASS_NUM     8
/* +CR167704 */
#ifdef UEIT_MOCOR  
extern CRITICAL_SECTION     _tx_win32_schedule_ready_critical_section;
extern HANDLE               _tx_win32_schedule_ready_semaphore;
extern ULONG                _tx_win32_schedule_ready;
#endif
/* -CR167704 */

#define	TX_WIN32_MEMORY_SIZE	 (300*1024)

#ifdef UEIT_MOCOR
#define TX_TIMER_PERIODIC		1
#else
#define TX_TIMER_PERIODIC		20
#endif


#endif


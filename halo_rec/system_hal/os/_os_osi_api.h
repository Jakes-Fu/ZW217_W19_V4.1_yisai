/******************************************************************************
 ** File Name:      os_api.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           11/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in sci_types.h   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/14/2002     Xueliang.Wang    Create.                                   *
 ** 09/12/2003     Zhemin.Lin       Modify according to CR:MS00004678         *
 ** 10/11/2004     Richard.Yang     Add Trace interface and server interface  *
 ** 11/28/2005     Shujing.Dong     Modify according to CR:MS00035616         *
 ******************************************************************************/
#ifndef _OS_API_H
#define _OS_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// Basic data types.
#include "platform_common.h"


#ifdef WIN32    
    // Include this file to replace windows.h to avoid compile error
#ifndef OSA_ON_THREADX
//    #include "windows_sci_os.h"
#endif
#endif


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
//---------------------------------------------
// API return values.
//---------------------------------------------
//Success, no error.
#define SCI_SUCCESS                 OSI_SUCCESS

//Normal error.
#define SCI_ERROR                   0xFF

// Thread block ID.
typedef uint32          BLOCK_ID;

//---------------------------------------------
// API input parameters.
//---------------------------------------------
// Boolean value
#define SCI_TRUE                    TRUE       // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value

// Wait option.
#define SCI_NO_WAIT                 OSI_NO_SUSPEND
#define SCI_WAIT_FOREVER            OSI_SUSPEND

// Option used to set/get event.
#define SCI_OR                      OSI_OR
#define SCI_OR_CLEAR                OSI_OR_CONSUME
#define SCI_AND                     OSI_AND
#define SCI_AND_CLEAR               OSI_AND_CONSUME

// Auto start option on thread
#define SCI_DONT_START              OSI_NO_START
#define SCI_AUTO_START              OSI_START

// Indicates if the thread is preemptable.
#define SCI_NO_PREEMPT              OSI_NO_PREEMPT
#define SCI_PREEMPT                 OSI_PREEMPT

// Auto start option on timer.
#define SCI_NO_ACTIVATE             OSI_DISABLE_TIMER
#define SCI_AUTO_ACTIVATE           OSI_ENABLE_TIMER

//@Zhemin.Lin, add, 09/12/2003, CR:MS00004678
//priority inherit mode for mutex
#define SCI_NO_INHERIT 0
#define SCI_INHERIT 1


#if 0
// Thread priority definetion
#define SCI_PRIORITY_KERNEL         0 
#define SCI_PRIORITY_TIME_CRITICAL  5 
#define SCI_PRIORITY_HIGHEST        10
#define SCI_PRIORITY_ABOVE_NORMAL   15
#define SCI_PRIORITY_IDLE           31

// Temp add  for 256 priority
#ifndef WCDMA_AS_PRESENT
#define SCI_PRIORITY_NORMAL         20
#define SCI_PRIORITY_BELOW_NORMAL   24
#define SCI_PRIORITY_LOWEST         28
#define PRI_BASE_LAYER1 3
#else
#define SCI_PRIORITY_NORMAL         24
#define SCI_PRIORITY_BELOW_NORMAL   26
#define SCI_PRIORITY_LOWEST         29
#define PRI_BASE_LAYER1 5
#endif
#define PRI_BASE_ATC_TASK  SCI_PRIORITY_BELOW_NORMAL
#define PRI_BASE_CALIBRATION SCI_PRIORITY_HIGHEST
#define PRI_TDD_CALIBRATION1 SCI_PRIORITY_HIGHEST
#define PRI_TDD_CALIBRATION2 SCI_PRIORITY_HIGHEST
#define PRI_L1C_DUMP_MEM SCI_PRIORITY_ABOVE_NORMAL
#define PRI_DUMMY_RRC_TASK  SCI_PRIORITY_TIME_CRITICAL

#define TX_BASE_HIGH         0
#define TX_BASE_LOW          31 
#define SCI_BASE_START            TX_BASE_HIGH
#define SCI_BASE_END              TX_BASE_LOW
#define MAX_BASE_PRI_NUM          ((SCI_BASE_END - SCI_BASE_START) + 1)

#define TX_CORE_HIGH         0
#define TX_CORE_LOW          31
#define SCI_KERNEL_START          TX_CORE_HIGH
#define SCI_KERNEL_END            TX_CORE_LOW
#define MAX_KERNEL_PRI_NUM        ((SCI_KERNEL_END - SCI_KERNEL_START) + 1)

#define TX_SYSTEM_HIGH       0
#define TX_SYSTEM_NORMALE    16
#define TX_SYSTEM_LOW        31
#define SCI_SYSTEM_START            TX_SYSTEM_HIGH
#define SCI_SYSTEM_END                TX_SYSTEM_LOW
#define MAX_SYSTEM_PRI_NUM        ((SCI_SYSTEM_END - SCI_SYSTEM_START) + 1)

#define  TX_USER_HIGH         0
#define  TX_USER_LOW          31
#define SCI_APP_START             TX_USER_HIGH
#define SCI_APP_END               TX_USER_LOW
#define MAX_APP_PRI_NUM           ((SCI_APP_END - SCI_APP_START) + 1)

#define DECLARE_PRIORITY(_X_) extern const uint32 tx_pri_##_X_;
#define SCI_BASE_PRI(_PRI_) (((_PRI_) < MAX_BASE_PRI_NUM) ? (SCI_BASE_START   + (_PRI_)) : SCI_BASE_END)
#define SCI_TASK_PRI(_X_) (tx_pri_##_X_)
#define SCI_KERNEL_PRI(_PRI_) (((_PRI_) < MAX_KERNEL_PRI_NUM) ? (SCI_KERNEL_START + (_PRI_)) : SCI_KERNEL_END)
#define SCI_SYSTEM_PRI(_PRI_) (((_PRI_) < MAX_SYSTEM_PRI_NUM) ? (SCI_SYSTEM_START + (_PRI_)) : SCI_SYSTEM_END)
#define SCI_APP_PRI(_PRI_) (((_PRI_) < MAX_SYSTEM_PRI_NUM) ? (SCI_APP_START + (_PRI_)) : SCI_APP_END)
#define SCI_DEFINE_TASK_PRI(_X_, _VAL_) const uint32 tx_pri_##_X_ = (_VAL_)
#endif
//---------------------------------------------
// General constants.
//---------------------------------------------

#define SCI_NULL                    0x0
#define SCI_INVALID_BLOCK_ID        0xFFFFFFFF

// Size of Queue item. Number of unsigned long int.
#define SCI_QUEUE_ITEM_SIZE         1

// Name size of thread, timer, queue, etc... 
#define SCI_MAX_NAME_SIZE           OSI_NAME_NUM

//define Heap Type 
#define   CONST_HEAP_MEMORY                0x22222222UL 
#define   DYNAMIC_HEAP_BASE_MEMORY         0x33333333UL 
#define   DYNAMIC_HEAP_APP_MEMORY          0x44444444UL 

// for SDI trace, ims and tcpip use
#define SDI_PARAMS(_argNum, _param) \
	            SDI_PARAMS_##_argNum _param

#define SDI_PARAMS_0()

#define SDI_PARAMS_1(p1)  \
	       , p1

#define SDI_PARAMS_2(p1, p2)  \
	       , p1, p2
 
#define SDI_PARAMS_3(p1, p2, p3)  \
         , p1, p2, p3
  
#define SDI_PARAMS_4(p1, p2, p3, p4)  \
          , p1, p2, p3, p4
   
#define SDI_PARAMS_5(p1, p2, p3, p4, p5)  \
           , p1, p2, p3, p4, p5
    
#define SDI_PARAMS_6(p1, p2, p3, p4, p5, p6)  \
            , p1, p2, p3, p4, p5, p6
     
#define SDI_PARAMS_7(p1, p2, p3, p4, p5, p6, p7)  \
             , p1, p2, p3, p4, p5, p6, p7
     
#define SDI_PARAMS_8(p1, p2, p3, p4, p5, p6, p7, p8)  \
             , p1, p2, p3, p4, p5, p6, p7, p8
      
#define SDI_PARAMS_9(p1, p2, p3, p4, p5, p6, p7, p8, p9)  \
              , p1, p2, p3, p4, p5, p6, p7, p8, p9
       
#define SDI_PARAMS_10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)  \
               , p1, p2, p3, p4, p5, p6, p7, p8, p9, p10

#define SDI_TRACEID(mod, id, format)       SCI_TRACE_LOW(format) 
#define SDI_TRACE(mod, format)             SCI_TRACE_LOW(format)
#define SDI_VTRACEID(mod, id, a, b, format, argNum, param)   SCI_TRACE_LOW(format SDI_PARAMS(argNum, param))
#define SDI_VTRACE(mod, format, argNum, param)               SCI_TRACE_LOW(format SDI_PARAMS(argNum, param))

#define SDI_VTRACEID_L0(mod, id, a, b, format, argNum, param) SCI_TRACE_LOW(format SDI_PARAMS(argNum, param)) 
#define SDI_VTRACE_L0(mod, format, argNum, param)             SCI_TRACE_LOW(format SDI_PARAMS(argNum, param))

#define SCI_TRACE_NULL           SCI_TraceNull
#ifdef WIN32
    //extern  void SCI_Trace(const char *, ...);
    /*#define SCI_TRACE_ERROR      SCI_Trace
    #define SCI_TRACE_HIGH       SCI_Trace
    #define SCI_TRACE_MID        SCI_Trace
    #define SCI_TRACE_LOW        SCI_Trace
    #define SCI_TRACE_BUF        SCI_TraceBuf
    #define SCI_TRACE_DATA       SCI_TraceCapData
    #define SCI_TRACE_SLEEP      SCI_Trace
    #define SCI_TRACE_TPDATA     SCI_TraceTPData*/
    #define SCI_TRACE_ERROR      
    #define SCI_TRACE_HIGH       
    #define SCI_TRACE_MID        
    #define SCI_TRACE_LOW        OSAL_logMsg
    #define SCI_TRACE_BUF        
    #define SCI_TRACE_DATA       
    #define SCI_TRACE_SLEEP      
    #define SCI_TRACE_TPDATA 
    #define SCI_TraceLow(...)
#else

    #ifdef   SCI_TRACE_MODE
        #define SCI_TRACE_BUF        SCI_TraceBuf
        #define SCI_TRACE_MID        SCI_TraceMid
        #define SCI_TRACE_LOW        SCI_TraceLow
        #define SCI_TRACE_HIGH       SCI_TraceHigh
        #define SCI_TRACE_ERROR      SCI_TraceError
        #define SCI_TRACE_DATA       SCI_TraceCapData
        #define SCI_TRACE_SLEEP      SCI_TraceSleep
        #define SCI_TRACE_TPDATA     SCI_TraceTPData
    #else
        #define SCI_TraceBuf(...)                //
        #define SCI_TraceLow(...)               //
        #define SCI_TraceMid(...)                //
        #define SCI_TraceError(...)             //
        #define SCI_TraceHigh(...)              //
        #define SCI_TraceSleep(...)

        #define SCI_TRACE_BUF(...)
        #define SCI_TRACE_MID(...)
        #if defined CSM_DEBUG || defined SCI_DEBUG
        #define SCI_TRACE_LOW         OSAL_logMsg
        #else
        #define SCI_TRACE_LOW(...)
        #endif
        #define SCI_TRACE_HIGH(...) 
        #define SCI_TRACE_ERROR(...)  
        #define SCI_TRACE_DATA(...)
        #define SCI_TRACE_SLEEP(...)
        #define SCI_TRACE_TPDATA(...)
    #endif
#endif  

//---------------------------------------------
// Type define.
//---------------------------------------------
typedef void    *SCI_EVENT_GROUP_PTR;
typedef void    *SCI_THREAD_PTR;
typedef void    *SCI_TIMER_PTR;

//Zhemin.Lin, add, 09/12/2003, CR:MS00004678,
typedef void    *SCI_MUTEX_PTR;
typedef void    *SCI_SEMAPHORE_PTR;

// The prototype of C funtion which execute when timer expires.
typedef void    (*TIMER_FUN)(uint32);

// The prototype of C function for thread execution.
typedef void    (*THREAD_FUN)(uint32, void*);

// Signal head structure.
// Signal vars used when send signals from one task to anther.
// The pointer is a structure whose head is SIGNAL_VARS.
#define _SIGNAL_VARS \
    uint16 SignalCode; \
    uint16 SignalSize; \
    xSignalHeader  Pre; \
    xSignalHeader  Suc; \
    BLOCK_ID       Sender;

#ifndef _BSSIM
    #define SIGNAL_VARS \
            _SIGNAL_VARS
#else
    #define SIGNAL_VARS \
            _SIGNAL_VARS \
            void  *SigP;
#endif // End of _BSSIM

// Signal head structure.
typedef struct xSignalHeaderStruct *xSignalHeader;
typedef struct xSignalHeaderStruct {
  SIGNAL_VARS
} xSignalHeaderRec;

// Memory debug head structure, used to identify the block of memory.
typedef struct MEM_HEADER_tag{
	struct MEM_HEADER_tag * pre;
	struct MEM_HEADER_tag * succ;
	char                  * file_name;
	int                   line;
    unsigned long         size;
    unsigned long         block_num;
} MEM_HEADER_T;    
#define MEM_HEADER_SIZE     sizeof(MEM_HEADER_T)

//@Daniel.Luo for pool optimization
typedef struct POOL_INFO_tag{
	uint16 pool_size;	// size of the pool, in byte
	uint16 pool_num;	// number of the pool
} POOL_INFO_T;


#define  MEM_POOL_DEBUG_MODE        0x0
#define  MEM_POOL_RELEASE_MODE     0x5245


/*================App Memory  Pool Defined==================*/
#define  MPOOL_HANDLE        uint32
typedef struct APP_MEM_CFG_Tag
{
	uint32  min_pool_size;
	uint32  pool_space_size;
	uint32  heap_mem_watermark;
	uint32  pool_mode;
}APP_MEM_CFG_T;


/*===========Shell Debug Used System Pool Information Defined=================*/
#define SYS_POOL_NAME_SIZE  31
typedef struct SYS_POOL_INFO_Tag
{
    uint8   name[SYS_POOL_NAME_SIZE + 1];
	uint32  mem_start_addr;
	uint32  mem_end_addr;
	uint32  total_num;
    uint32  availble_num;
	uint32  max_used_num;
}SYS_POOL_INFO_T;

#define SCI_SYS_POOL_TYPE  uint32
#define SCI_SYS_BLOCK_POOL  0x01
#define SCI_SYS_BYTE_POOL   0x02
#define SCI_SYS_POOL_MAX    0x03

#define POOL_INFO_SEEK_FIRST     0x00
#define POOL_INFO_SEEK_NEXT      0x01
#define SCI_POOL_INFO_END        0x00454E44UL
/*===========Shell Debug Used System Pool Information Defined End=================*/


/********************Defined RTOS Tick Time ***************************************/
typedef struct SCI_TICK_TIME_Tag
{
   uint32 milliseconds;
   uint32 second;
}SCI_TICK_TIME_T;


/********************Defined BLOCKID TO HANDLE ************************************/
typedef struct SCI_OSI_BLOCK_ID_Tag
{
   uint32          used;
   T_osi_HANDLE    threadId;
   T_osi_HANDLE    queueId;
}SCI_OSI_BLOCK_ID_T;


/**---------------------------------------------------------------------------*
 ** MACROES:
 **---------------------------------------------------------------------------*/
#ifdef MTCOFFSET
#else 
#ifdef _WIN_LOG_TOOL
    #define SCI_ALLOC_CONST(_SIZE)        malloc(_SIZE)
    #define SCI_ALLOC_BASE(_SIZE)         malloc(_SIZE)
    #define SCI_ALLOC_BASEZ(_SIZE)        malloc(_SIZE)
    #define SCI_ALLOC_APP(_SIZE)          malloc(_SIZE)
    #define SCI_ALLOC_APPZ(_SIZE)         malloc(_SIZE)
    #define SCI_MPALLOC(_SIZE)            malloc(_SIZE)
    #define SCI_ALLOC(_SIZE)              malloc(_SIZE)
    #define SCI_ALLOCA(_SIZE)             malloc(_SIZE)        
    #define SCI_MPFREE(_MEM_PTR)          free(_MEM_PTR);     (_MEM_PTR) = PNULL;
    #define SCI_FREE(_MEM_PTR)            free(_MEM_PTR);     (_MEM_PTR) = PNULL;
    #define SCI_ASSERT(_EXP)              assert(_EXP)
    #define SCI_PASSERT(_EXP,PRINT_STR)   assert( _EXP );
    #define SCI_DASSERT(_EXP)             assert(_EXP)
#else
	#ifdef WIN32
        #define SCI_ALLOC_CONST(_SIZE)       SCI_MallocEx(_SIZE, CONST_HEAP_MEMORY, __FILE__, __LINE__)
        #define SCI_ALLOC_BASE(_SIZE)        SCI_MallocEx(_SIZE, DYNAMIC_HEAP_BASE_MEMORY, __FILE__, __LINE__)
        #define SCI_ALLOC_BASEZ(_SIZE)       SCI_Malloc_Zero(_SIZE, DYNAMIC_HEAP_BASE_MEMORY, __FILE__, __LINE__)
        #define SCI_ALLOC_APP(_SIZE)         SCI_MallocEx(_SIZE, DYNAMIC_HEAP_APP_MEMORY, __FILE__, __LINE__)      	        
        #define SCI_ALLOC_APPZ(_SIZE)        SCI_Malloc_Zero(_SIZE, DYNAMIC_HEAP_APP_MEMORY, __FILE__, __LINE__)
        #define SCI_MPALLOC(_SIZE, _HANDLE_) SCI_MemPoolMalloc(_SIZE, _HANDLE_, __FILE__, __LINE__)
        #define SCI_ALLOC(_SIZE)             SCI_Malloc(_SIZE, __FILE__, __LINE__)
        #define SCI_ALLOCA(_SIZE)            SCI_mallocMem(_SIZE)	        
        #define SCI_MallocApp(_SIZE, a, b)   SCI_mallocMem(_SIZE)
        #define SCI_MPFREE(_MEM_PTR)         SCI_MemPoolFree(_MEM_PTR); (_MEM_PTR) = PNULL;
        //#define SCI_FREE(_MEM_PTR)           SCI_Free(_MEM_PTR); (_MEM_PTR) = PNULL;
        #define SCI_FREE(_MEM_PTR)       SCI_freeMem(_MEM_PTR);
        //#define SCI_ASSERT(_EXP)             (_EXP) ? (void)0 : SCI_Assert(#_EXP, __FILE__, __LINE__)
        #define SCI_ASSERT(_EXP)             (_EXP) ? (void)0 : ERC_ASSERT(TRUE,"SCI assert",ASSERT_RESET)
				#ifdef __GNUC__
				#define SCI_PASSERT(_EXP,PRINT_STR)  (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __FILE__, __LINE__, SCI_SaveAssertInfo PRINT_STR)				
				#else
        //#define SCI_PASSERT(_EXP,PRINT_STR)  (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __FILE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)
        #define SCI_PASSERT(_EXP,PRINT_STR)  (_EXP) ? (void)0 : ERC_ASSERT(TRUE,"SCI assert",ASSERT_RESET)
				#endif
				
				#define SCI_DASSERT(_EXP)            (_EXP) ? (void)0 : SCI_DAssert(#_EXP, __FILE__, __LINE__)
    #else
         // SCI always used byte memory
     	    #define SCI_ALLOC_CONST(_SIZE)   SCI_mallocMem(_SIZE)
         	#define SCI_ALLOC_BASE(_SIZE)    SCI_mallocMem(_SIZE)
          #define SCI_ALLOC_BASEZ(_SIZE)   SCI_mallocMem(_SIZE)
          #define SCI_ALLOC_APP(_SIZE)     SCI_mallocMem(_SIZE)      	        	    
 	        #define SCI_ALLOC_APPZ(_SIZE)    SCI_mallocMem(_SIZE)                  
     	    #define SCI_ALLOC(_SIZE)         SCI_mallocMem(_SIZE)
		    	   #define SCI_ALLOCA(_SIZE)        SCI_mallocMem(_SIZE)	  
          #define SCI_MallocApp(_SIZE, a, b)  SCI_mallocMem(_SIZE)
          #define SCI_Malloc(_SIZE, __FILE__, __LINE__) SCI_mallocMem(_SIZE)

		        #define SCI_FREE(_MEM_PTR)       SCI_freeMem(_MEM_PTR);

	         #define SCI_ASSERT(_EXP)    (_EXP) ? (void)0 : ERC_ASSERT(TRUE,"SCI assert",ASSERT_RESET)
	         #define SCI_PASSERT(_EXP,PRINT_STR)     (_EXP) ? (void)0 : ERC_ASSERT(TRUE,"SCI assert",ASSERT_RESET)
	         #define SCI_DASSERT(_EXP)    (_EXP) ? (void)0 : ERC_ASSERT(TRUE,"SCI assert",ASSERT_RESET)
	    #endif
    #endif  // End of _WIN_LOG_TOOL
#endif  // End of MTCOFFSET

#define SCI_ASSERT_VERIFIED(_EXP) \
     (_EXP) ? (void)0 : SCI_Assert(#_EXP, __MODULE__, __LINE__)/*assert verified*/;

#define SCI_PASSERT_VERIFIED(_EXP,PRINT_STR) \
     (_EXP) ? (void)0 : (void )SCI_PAssert(#_EXP, __MODULE__, __LINE__, SCI_SaveAssertInfo##PRINT_STR)/*assert verified*/;


#if  0
    void* SCI_MEMCPY(void* _DEST_PTR, CONST  void* _SRC_PTR, uint32 _SIZE) ;

   void*  SCI_MEMSET(void* _DEST_PTR, uint32 _VALUE, uint32 _SIZE) ;
#else
    #define SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE) \
        do  \
        { \
            if ((_SIZE)>0) { memcpy((_DEST_PTR), (_SRC_PTR), (_SIZE));} \
        }while(0);

    #define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
        do\
        {\
            if ((_SIZE)>0) { memset((_DEST_PTR), (_VALUE), (_SIZE));  }	\
        }while(0);
#endif

#define SCI_MEM16CPY(_DEST_PTR, _SRC_PTR, _SIZE_IN_BYTES) \
        SCI_Mem16Cpy((_DEST_PTR), (_SRC_PTR), (_SIZE_IN_BYTES))
        
#define SCI_MEM16SET(_DEST_PTR, _VALUE, _SIZE_IN_BYTES) \
        SCI_Mem16Set((_DEST_PTR), (_VALUE), (_SIZE_IN_BYTES))


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
 /**---------------------------------------------------------------------------*
 ** ASSERT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is typically used to identify logic errors 
//                  during program development, by implementing the expression 
//                  argumnet to evaluate to SCI_FALSE only when the program 
//                  is operating incorrectly. 
//                  After debugging is complete, assertion checking can be 
//                  turned off without modifying the source file by deleting 
//                  the predifinition _DEBUG.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_Assert(
    void    *exp,     // Expression (including pointers) that evaluates 
                      // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line      // Line number where assert
    );


/**---------------------------------------------------------------------------*
 ** EVENT
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a group of 32 event flags.  
//                  All 32 event flags in the group are initialized to zero.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC SCI_EVENT_GROUP_PTR SCI_CreateEvent( // If create successfully, 
                            // return pointer to the control block of created
                            // event; else return SCI_NULL.
    const char *name_ptr    // Pointer to the name of the event flags group.
    );
    
/*****************************************************************************/
//  Description:    This function sets the specified flags in the event group
//                  based on the set option specified.  All threads suspended
//                  on the group whose get request can now be satisfied are 
//                  resumed.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SetEvent(     // If set successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR
    SCI_EVENT_GROUP_PTR group_ptr,  // Pointer to an event flags group control 
                                    // block. 
    uint32      flags_to_set,   // Specifies the event flags to set or      
                                // clear based upon the set option selected.
    uint32      set_option      // Specifies whether the event flags        
                                // specified are ANDed or ORed into the     
                                // current event flags of the group.        
                                // The following are valid selections:      
                                // SCI_AND (0x02)                           
                                // SCI_OR (0x00)                            
                                // Selecting SCI_AND specifies that the     
                                // specified event flags are ANDed into the 
                                // current event flags in the group. This   
                                // option is often used to clear event flags
                                // in a group. Otherwise, if SCI_OR is      
                                // specified, the specified event flags are 
                                // ORed with the current event in the group.
    );

/*****************************************************************************/
//  Description:    This function gets the specified event flags from the 
//                  group, acording to the get option.  The get option also 
//                  specifies whether or not the retrieved flags are cleared. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_GetEvent(     // If get it successfully, return SCI_SUCCESS; 
                                // else return SCI_ERROR                
    SCI_EVENT_GROUP_PTR group_ptr,  // Pointer to an event flags group control
                                    // block. 
    uint32      requested_flags,    // 32-bit unsigned variable that represents   
                                    // the requested event flags.                 
    uint32      get_option,     // Specifies whether all or any of the        
                                // requested event flagsare required. The     
                                // following are valid selections:            
                                // SCI_AND (0x02)                             
                                // SCI_AND_CLEAR (0x03                        
                                // SCI_OR (0x00)                              
                                // SCI_OR_CLEAR (0x01)                        
                                // Selecting SCI_AND or SCI_AND_CLEAR         
                                // specifies that all event flags must be     
                                // present in the group. Selecting SCI_OR or  
                                // SCI_OR_CLEAR specifies that any event flag
                                // is satisfactory. Event flags that satisfy  
                                // the request are cleared (set to zero) if   
                                // SCI_AND_CLEAR or SCI_OR_CLEAR are          
                                // specified.                                 
    uint32      *actual_flags_ptr,  // Pointer to destination of where the
                                    // retrieved event flags are placed. Note 
                                    // that the actual flags obtained may
                                    // contain flags that were not requested.
    uint32      wait_option     // Defines how the service behaves if the
                                // selected event flags are not set. The
                                // wait options are defined as follows:
                                // SCI_NO_WAIT (0x00000000)
                                // SCI_WAIT_FOREVER (0xFFFFFFFF)
                                // timeout value (0x00000001 - 0xFFFFFFFE)
    );

/*****************************************************************************/
//  Description:    The function deletes a group of 32 event flags. 
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteEvent(  // If delete successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR
    SCI_EVENT_GROUP_PTR group_ptr	// Pointer to a previously created event 
                                    // flags group. 
    );
    
#if 0
/**---------------------------------------------------------------------------*
 ** MEMORY MANAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The SCI_Malloc function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different 
//                  memory pool heap according to the size applications needed.
//                  This can avoid memory fragments and speed up memory 
//                  allocating, the shortcoming is that we might waste some 
//                  memory.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_ALLOC
/*****************************************************************************/
PUBLIC void * SCI_Malloc(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );


PUBLIC void * SCI_MallocEx(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    uint32 type, 
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );

//  Description:    The SCI_MallocEx_NoDebug function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different type byte heap
//                  according to type parameter indirected,and no debug information would be
//                  saved.
//	Global resource dependence: 
//  Author: 
//	Note:           Don't use it directly. Please use SCI_ALLOC_CONST/SCI_ALLOC_BASE/SCI_ALLOC_APP
/*****************************************************************************/
PUBLIC void * SCI_MallocExConst_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );

PUBLIC void * SCI_MallocExBase_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );

PUBLIC void * SCI_MallocExBase_Zero_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );

PUBLIC void * SCI_MallocExApp_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );

 


//  Description:    The SCI_Malloc_NoDebug function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different type byte heap
//                  according to type parameter indirected,and no debug information would be
//                  saved.
//	Global resource dependence: 
//  Author: 
//	Note:           Don't use it directly. Please use SCI_ALLOC
/*****************************************************************************/
PUBLIC void * SCI_Malloc_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );


/*****************************************************************************/
//  Description:    The SCI_Malloc_Zero function is used to Allocates memory
//                  in embedded system. It allocates memory from different 
//                  memory block pool and byte pool according to the size applications needed.
//                  if memory allocate succssful , clear all memory content to zero.
//	Global resource dependence: 
//  Author: 
//	Note:           Don't use it directly. Please use SCI_ALLOC_APPZ
/*****************************************************************************/
PUBLIC void * SCI_Malloc_Zero(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    uint32 type, 
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );

 

 //  Description:    The SCI_MallocEx_NoDebug function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different type byte heap
//                  according to type parameter indirected,and no debug information would be
//                  saved. All of the contens of allocated memory had been cleaned with zore;
//	Global resource dependence: 
//  Author: 
//	Note:           Don't use it directly. Please use SSCI_ALLOC_APPZ
/*****************************************************************************/
PUBLIC void * SCI_MallocExApp_Zero_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );   
    
/*****************************************************************************/
//  Description:    The SCI_Free function is used to Deallocates or frees 
//                  a memory block.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_FREE
/*****************************************************************************/
PUBLIC uint32 SCI_Free(     // SCI_SUCCESS or SCI_ERROR
    void *memory_ptr        // Pointer to memory block to be deleted.
    );
 
/*****************************************************************************/
//  Description:    The SCI_Malloc function is used to Allocates memory blocks
//                  in embedded system. It allocates memory from different 
//                  memory pool heap according to the size applications needed.
//                  This can avoid memory fragments and speed up memory 
//                  allocating, the shortcoming is that we might waste some 
//                  memory.
//					This function will return SCI_NULL if no memory fits need.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Don't use it directly. Please use SCI_ALLOCA
/*****************************************************************************/
PUBLIC void * SCI_MallocApp(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size,        // Bytes to allocate.
    const char * file,  // The name of the file which call this function.
    uint32 line         // The line number in the file which call this function.
    );


//  Description:    The SCI_MallocEx_NoDebug function is used to Allocates memory blocks
//                  in embedded system. If the system App Heap memory is not enough, return NULL
//	Global resource dependence: 
//  Author: 
//	Note:           Don't use it directly. Please use SCI_ALLOC_CONST/SCI_ALLOC_BASE/SCI_ALLOC_APP
/*****************************************************************************/
PUBLIC void * SCI_MallocApp_NoDebug(   // Returns a void pointer to the allocated space, 
                            // or SCI_NULL if there is insufficient memory
                            // available.
    uint32 size        // Bytes to allocate.
    );
#endif
/**---------------------------------------------------------------------------*
 ** MESSAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function send signal to proper thread.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignal(   // If send successfully, return SCI_SUCCESS,
                                // else return SCI_ERROR
    xSignalHeader   signal_ptr, // Signal pointer to be sended
    BLOCK_ID        dest_id     // Dest thread ID
    );


/*****************************************************************************/
//  Description:    The function send signal to specified thread, and put the 
//                  signal to the header of queue.
//                  This function return immediately regardless of whether or
//                  not it was successful.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignalFront(   // If send successfully, return SCI_SUCCESS,
                                // else return SCI_ERROR
    xSignalHeader   signal_ptr, // Signal pointer to be sended
    BLOCK_ID        dest_id     // Dest thread ID
    );


/*****************************************************************************/
//  Description:    The function gets a signal from a thread.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSignal(     // Return received signal.
    BLOCK_ID thread_id                  // ID of the thread which receives signal
    );    

/*****************************************************************************/
//  Description:    This function gets a signal from a thread immediatelly, 
//                  if no signal, function returns immediatelly.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC xSignalHeader SCI_PeekSignal(    // Return received signal.
                                        // If no signal, return SCI_NULL.
    BLOCK_ID thread_id                  // ID of the thread which receives signal
    );

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to 
//                  be created are alloced in this function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateAppThread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    );

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to 
//                  be created are alloced in this function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateThread(       // If successful, returns the thread ID
                                        // of the new created thread, 
                                        // else returns SCI_INVALID_BLOCK_ID
    const char  *thread_name,           // Name string of the thread
    const char  *queue_name,            // Queue name string of the thread
    void    (*entry)(uint32, void*),    // Entry function of the thread
    uint32  argc,                       // First parameter for entry function,
    void    *argv,                      // Second parameter for entry function,
    uint32  stack_size,                 // Size of the thread stack in bytes
    uint32  queue_num,          // Number of messages which can be enqueued
    uint32  priority,           // Prority of the thread.
    uint32  preempt,            // Indicates if the thread is preemptable.
    uint32  auto_start          // Specifies whether the thread starts 
                                // immediately or stays in a pure suspended
                                // state. Legal options are SCI_AUTO_START
                                // and SCI_DONT_START.
    );

    
/*****************************************************************************/
//  Description:    The function deletes a thread created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteThread( // If delete successful,return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID of the thread to be deleted.
    );
    
/*****************************************************************************/
//  Description:    The function exit current thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ThreadExit(void);

/*****************************************************************************/
    
/*****************************************************************************/
//  Description:    The function returns the current inused thread ID. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_IdentifyThread(void);   // If the thread is created by 
                                            // user, return thread id; else 
                                            // return SCI_INVALID_BLOCK_ID.


/*****************************************************************************/
//  Description:    The function returns the specify thread exist, or not. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           If thread exist, return SCI_SUCCESS, or return SCI_ERROR
/*****************************************************************************/
PUBLIC uint32 SCI_IsThreadExist(BLOCK_ID  thread_id);   



/*****************************************************************************/
//  Description:    The function returns the specify thread queue aviable, or not. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           If thread queue avilable, return SCI_SUCCESS, or return 
//                  SCI_ERROR/SCI_QUEUE_FULL
/*****************************************************************************/
PUBLIC uint32 SCI_IsThreadQueueAvilable(BLOCK_ID  thread_id); 


/*****************************************************************************/
//  Description:    This function changes the priority of the specified thread. 
//                  Valid priorities range from 0 through 31, where 0 
//                  represents the highest priority level.
//	Global resource dependence: 
//                  The thread should be a previously created thread.
//  Author:         Benjamin.Wang
//	Note:           The preemption threshold of the specified thread is 
//                  automatically set to the new priority. 
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeThreadPriority( // TX_SUCCESS (0x00) Successful 
                                        //  priority change.
                                        // TX_THREAD_ERROR (0x0E) Invalid 
                                        //  application thread pointer.
                                        // TX_PRIORITY_ERROR (0x0F) Specified 
                                        //  new priority is not valid (a value 
                                        //  other than 0-31).
                                        // TX_PTR_ERROR (0x03) Invalid pointer 
                                        //  to previous priority storage location.
                                        // TX_CALLER_ERROR (0x13) Invalid caller 
                                        //  of this service.
    BLOCK_ID    thread_id,              // ID of the thread to be deleted., 
    uint32      new_priority,           // New thread priority level (0-31).
    uint32*     old_priority_ptr        // Pointer to a location to return the 
                                        // thread¡¯s previous priority.
    );
    
/*****************************************************************************/
//  Description:    This function moves the currently executing thread to the 
//                  end of the list of threads ready at the same priority. 
//                  If no other threads of the same or higher priority are 
//                  ready, this function simply returns.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_RelinquishThread(void);

/*****************************************************************************/
//  Description:    The function resumes a specified thread. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ResumeThread( // If resume successful, return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID of the thread to be resumed.
    );

/*****************************************************************************/
//  Description:    The function suspends a specified thread. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_SuspendThread(    // If suspend successful, return 
                                    // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id           // ID of the thread to be suspended.
    );

/*****************************************************************************/
//  Description:    The function terminates a specified thread.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_TerminateThread(  // If terminate successful, return 
                                    // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id           // ID of the thread to be terminated.
    );


/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a 
//                  specified interval. 
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_Sleep(
    uint32  time_in_ms      // Number of milliseconds to sleep.
    );


#if 0
/*****************************************************************************/
//  Description:    The function get the SaveQ of the thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSaveQ(  // If the SaveQ is null, return SCI_NULL; 
                                    // else return the first signal pointer 
                                    // in the SaveQ.
    BLOCK_ID    thread_id           // ID fo thread which SaveQ to be getten.
    );

/*****************************************************************************/
//  Description:    Add the signal to the SaveQ of the thread.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_AddToSaveQ(   // If add it successfully, return the 
                                // SCI_SUCCESS, else return SCI_ERROR.
    BLOCK_ID        thread_id,  // ID fo thread.
    xSignalHeader   sig_ptr     // Pointer to the signal to be added.
    );

/*****************************************************************************/
//  Description:    Deleted the signal from the SaveQ of the thread.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DelFromSaveQ( // If delete successfully, return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID        thread_id,  // ID fo thread.
    xSignalHeader   sig_ptr     // Pointer to the signal to be deleted.
    );

/*****************************************************************************/
//  Description:    The function clear the SaveQ of the thread.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_ClrSaveQ(     // If clear successfully, return the 
                                // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID    thread_id       // ID fo thread which SaveQ to be getten.
    );

/*****************************************************************************/
//  Description:    Get the next signal of the specified signal.
//	Global resource dependence: 
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetNextSig(    // Return the next signal.
    xSignalHeader   sig_ptr             // Pointer to the specified signal.
    );
#endif
   
/**---------------------------------------------------------------------------*
 ** TIMER 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function create a timer with call back function. 
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreateTimer(   // If successful, returns pointer to
                                        // the control block of the timer,
                                        // else return SCI_NULL.
    const char  *timer_name,    // Name string of the timer
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in     
                                // milliseconds.                           
    uint32      auto_activate   // Option to check if auto active the timer
                                // after create.    
    );

/*****************************************************************************/
//  Description:    The function create a period timer with call back function.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreatePeriodTimer( // If successful, returns pointer to
                                            // the control block of the timer,
                                            // else return SCI_NULL.
    const char  *timer_name,    // Name string of the timer
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in
                                // milliseconds. 
    uint32      auto_activate   // Option to check if auto active the timer
                                // after create.
    );

/*****************************************************************************/
//  Description:    The function changed timer's callback function and 
//                  expire time.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,    // Timer control block
    TIMER_FUN   timer_fun,      // Timer callback function
    uint32      timer_expire    // Specifies the expire value in milliseconds. 
    );

/*****************************************************************************/
//  Description:    The function activate a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_ActiveTimer(  // If active successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );
    
/*****************************************************************************/
//  Description:    The function deactive a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeactiveTimer(    // If successful, returns SCI_SUCCESS,
                                    // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr         // Pointer to a previously created
                                    // application timer.
    );

/*****************************************************************************/
//  Description:    The function checks if the timer is still active.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsTimerActive(   // If it is active, returns SCI_TRUE,
                                    // else return SCI_FALSE 
    SCI_TIMER_PTR timer_ptr         // Pointer to a previously created 
                                    // application timer.
    );
        
/*****************************************************************************/
//  Description:    The function deletes a timer created before.
//	Global resource dependence: 
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteTimer(  // If successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );

/*****************************************************************************/
//  Description:    The function exit the specified timer.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_TimerExit(
    SCI_TIMER_PTR timer_ptr     // Pointer to a previously created application
                                // timer.
    );
    
/*****************************************************************************/
//  Description:    The function retrieves information about the specified 
//                  application timer.
//	Global resource dependence: 
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 SCI_GetTimerInfo( // If get successful, returns SCI_SUCCESS,
                                // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,    // Pointer to a previously created application
                                // timer.
    char    *name,              // Pointer to an SCI_MAX_NAME_SIZE character
                                // destination area for the time¡¯s name.
    BOOLEAN *active,            // Pointer to destination for the timer active
                                // indication. If this value is SCI_TRUE, 
                                // the timer is active, else is deactive.
    uint32  *remaining_time,    // Pointer to destination for the number of 
                                // timer ticks left before the timer expires.
    uint32  *reschedule_time    // Pointer to destination for the number of 
                                // timer ticks that will be used to 
                                // automatically reschedule this timer.
    );

/**---------------------------------------------------------------------------*
 ** TIME MACRO:  
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function gets the system time.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           In win32, return the number of seconds elapsed since
//                  midnight (00:00:00), January 1, 1970, coordinated 
//                  universal time, according to the system clock.
//                  In ARM, it is not defined.
/*****************************************************************************/
PUBLIC uint32 SCI_GetCurrentTime(void);

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that 
//                  have elapsed since the system was started.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           The elapsed time is stored as a uint32 value. Therefore, 
//                  the time will wrap around to zero if the system is run 
//                  continuously for 49.7 days. 
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickCount(void);    // Number of ticks.

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that 
//                  have elapsed since the system was started.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           The elapsed time is stored as tow uint32 values. Therefore, 
//                  the time will wrap around to zero if the system is run 
//                  continuously for 49700 days. 
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickTime(   //if successful, return SCI_SUCCESS, else return others
              SCI_TICK_TIME_T* tick_time  //input parameter to save system tick time
              );

/**---------------------------------------------------------------------------*
 ** Mutex
 **---------------------------------------------------------------------------*/
//@Zhemin.lin, 09/12/2003, CR:MS00004678, begin
/*****************************************************************************/
//  Description:    The function is used to create a mutex for inter-thread
//                  mutual exclusion for resource protection.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
SCI_MUTEX_PTR SCI_CreateMutex( //if successful, return the muxtex pointer,
                                          //else return SCI_NULL
    const char *name_ptr,     //mutex name
    uint32 priority_inherit  //inherit option, SCI_INHERIT or SCI_NO_INHERIT
);

/*****************************************************************************/
//  Description:    The function is used to destroy a mutex.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_DeleteMutex( //if successful, return SCI_SUCCESS,
                                          //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr  //mutex pointer
);

/*****************************************************************************/
//  Description:    The function is used to obtain exclusive ownership of the
//                  specified mutex.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_GetMutex( //if successful, return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr, //mutex pointer
    uint32 	wait_option     //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                              //             1~0x0FFFFFFFE wait time(ms)
);

/*****************************************************************************/
//  Description:    The function is used to renounce ownership of a mutex
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_PutMutex( //if successful, return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr //mutex pointer
);

/**---------------------------------------------------------------------------*
 ** Semaphore
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to create a counting semaphore.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
SCI_SEMAPHORE_PTR SCI_CreateSemaphore( //if successful, return semaphore
                                                     //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
);

/*****************************************************************************/
//  Description:    The function is used to destroy the specified counting semaphore
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_DeleteSemaphore ( //if successful return SCI_SUCCESS,
                                           //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
);

/*****************************************************************************/
//  Description:    The function is used to retrieve an instance from the specified
//                  counting semaphore. As a result, the specified semaphore's count
//                  is decreased by one.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
);

/*****************************************************************************/
//  Description:    The function is used to puts an instance into the specified
//                  counting semaphore, which in reality increments the counting
//                  semaphore by one.
//	Global resource dependence: 
//  Author:         Zhemin.Lin
//	Note:           
/*****************************************************************************/
uint32 SCI_PutSemaphore ( //if successful return SCI_SUCCESS,
                                      //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
);

#ifdef SCI_TRACE_MODE
/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_LOW. 
//                  This function put a message to the trace buffer. 
//                  1.msg_trace_info.buf_is_send == TRACE_BUF_SEND
//                    Send the messages to the application tool when buffer 
//                      is full.
//                  2.msg_trace_info.buf_is_send == TRACE_BUF_COVER
//                    Always cover the oldest message when buffer is full.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_TraceSleep(
    const char *x_format, ...);

/**---------------------------------------------------------------------------*
 ** Mutex
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_LOW. 
//                  This function put a message to the trace buffer. 
//                  1.msg_trace_info.buf_is_send == TRACE_BUF_SEND
//                    Send the messages to the application tool when buffer 
//                      is full.
//                  2.msg_trace_info.buf_is_send == TRACE_BUF_COVER
//                    Always cover the oldest message when buffer is full.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_TraceLow(
    const char *x_format, ...);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_LOW.
//                  This function put a message to the trace buffer.
//                  1.msg_trace_info.buf_is_send == TRACE_BUF_SEND
//                    Send the messages to the application tool when buffer
//                      is full.
//                  2.msg_trace_info.buf_is_send == TRACE_BUF_COVER
//                    Always cover the oldest message when buffer is full.
//	Global resource dependence:
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_PutsLow(const char *str);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_MED. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceMid(
    const char *x_format, ...);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer which log
//                  level is no more than LOG_LEVEL_HIGH. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceHigh(
    const char *x_format, ...);

/*****************************************************************************/
//  Description:    This function put a message to the trace buffer, and then 
//                  assert. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_TraceError(
    const char *x_format, ...);    

/*****************************************************************************/
//  Description:    This function trace out buffer.
//	Global resource dependence: DEBUG_PORT_SEND_BUF_SIZE
//  Author: Richard.Yang
//	Note:           1. Size should be not too big because of 
//                  DEBUG_PORT_SEND_BUF_SIZE is limited.
//                  2. Address should be access in byte or word.
/*****************************************************************************/
PUBLIC void SCI_TraceBuf(
    const char  *string_ptr,    // A string inputed.
    void        *addr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    );    

/*****************************************************************************/
//  Description:    This function trace data in hex mode.
//	Global resource dependence: DEBUG_PORT_SEND_BUF_SIZE
//  Author: Richard.Yang
//	Note:           1. Size should be not too big because of 
//                  DEBUG_PORT_SEND_BUF_SIZE is limited.
//                  2. Address should be access in byte .
/*****************************************************************************/
PUBLIC void SCI_TraceCapData(
    uint16      data_type,      // sub data type
    const void  *addr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    ); 
PUBLIC int32 SCI_TraceL1DspData(
    uint16      data_type,
    const void  *src_ptr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    );

PUBLIC int32 SCI_TraceTPData(
    void  *src_ptr,          // Beginer of address to be traced out.
    uint32      size            // Size in bytes to be traced out.
    );

#endif

PUBLIC void SCI_TraceNull( const char *x_format, ...);

PUBLIC BOOLEAN 	SCI_InThreadContext(void);
/* This function should be called only after fiq and irq are both diabled */
PUBLIC void		SCI_EnterAssertMode(void);

/*****************************************************************************/
//  Description:    This function is used to get the pointer to the head pointer
//  				of the created thread list.
//	Global resource dependence: 
//  Author: kevin.wang
//	Note:
/*****************************************************************************/
PUBLIC void *SCI_ThreadListHeadPtr(void);

/*****************************************************************************/
//  Description:    This function judge if mem pool is nearly to empty
//	Global resource dependence: 
//  Author: yi.su
//	Note:       
/*****************************************************************************/
PUBLIC uint32 SCI_GetMempoolInfo(uint32 poolsize, uint32 *available_num);

/*****************************************************************************/
//  Description:    This function is to get the max availd fragment space in the special byte heap
//	Global resource dependence: 
//  Author: Thomas.Chai
//	Note:       
/*****************************************************************************/
PUBLIC uint32  SCI_GetByteHeapAvailableSpace(uint32  heap_type);


/*****************************************************************************/
//  Description:    The SCI_CreateDynamicPool function is used to create  memory pools configuated by application 
//                 with a special memory space. The  pool number should be not less than one. And the total memory space 
//                of  all pools should be equal with the special memory space, if the total space is less than the specail space,
//                the left memory would not be used. And if the total space is more than the special space, then some pools
//                would not be created for not enough memory.
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC  MPOOL_HANDLE  SCI_CreateMemPool(  // return: 0 -- Error,  others --- the created dynamic pool handle
      const char* pool_name,   //Pool Name
      uint8* mem_addr,         //start address of the specail memory 
      uint32 mem_size,          //the spacial memory space size
      APP_MEM_CFG_T*   mem_cfg_ptr   //Memory Pool config information
     );

/*****************************************************************************/
//  Description:    The SCI_DeleteDynamicPool function is used to Release  all memory pools Created 
//                          whith SCI_CreateDynamicPool
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC  uint32 SCI_DeleteMemPool( //return: SCI_SUCCESS -- Successful,  SCI_ERROR--error
	MPOOL_HANDLE   pool_handle,    //pool handle which created by  SCI_CreateDynamicPool
	BOOLEAN   is_force_delete         //FALSE -- will check whether all memory had been free, TRUE --Not Check whether all memory had been free
	);
	
/*****************************************************************************/
//  Description:    The SCI_DPMalloc function is used to alloc memory blocks
//                  in embedded system. It allocates memory first from  dynamic memory pool ,if there is not enough memory,
//                  then  alloc from system memory pool, if they also have not enough memory,then alloc from system heap.
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC void*  SCI_MemPoolMalloc(  // Returns a void pointer to the allocated space, or SCI_NULL if there is insufficient memory
	uint32 size,     // Bytes to allocate.
       MPOOL_HANDLE  pool_handle,    //pool handle which created by  SCI_CreateDynamicPool
       const char * file,    // The name of the file which call this function.
       uint32 line             // The line number in the file which call this function.
       );


/*****************************************************************************/
//  Description:    The SCI_DFFree function is used to Release the memory which alloced  with  SCI_DPMalloc
//	Global resource dependence: 
//  Author: thomas.chai
//	Note:           Don't use it directly. Please use SCI_FREE
/*****************************************************************************/
PUBLIC uint32 SCI_MemPoolFree(     // SCI_SUCCESS or SCI_ERROR
    void *memory_ptr        // Pointer to memory block to be deleted.
    );


/*****************************************************************************/
//  Description:    The SCI_MemPoolMalloc_NoDebug function is used to alloc memory blocks
//                  in embedded system. It allocates memory first from  dynamic memory pool ,if there is not enough memory,
//                  then  alloc from system memory pool, if they also have not enough memory,then alloc from system heap.
//                  This interfce could not save memory debug information
//	Global resource dependence: 
//  Author: thomas.chai
/*****************************************************************************/
PUBLIC void*  SCI_MemPoolMalloc_NoDebug(  // Returns a void pointer to the allocated space, or SCI_NULL if there is insufficient memory
	     uint32 size,     // Bytes to allocate.
       MPOOL_HANDLE  pool_handle    //pool handle which created by  SCI_CreateDynamicPool
     );


/*****************************************************************************/
//  Description:    The SCI_DumpAppMemInfo function is used to dump app heap memory
//	Global resource dependence: 
//  Author: 
//	Note:   
/*****************************************************************************/
PUBLIC void SCI_DumpAppMemInfo(void);

/*****************************************************************************/
//  Description: The SCI_GetSysPoolInfo function is used to get system pool and
//                pool information  
//	Global resource dependence: 
//  Author: 
//	Note:   
/*****************************************************************************/
uint32 SCI_GetSysPoolInfo(           //SCI_SUCCESS -- successful; SCI_POOL_INFO_END -- the latest pool/heap information; others--error
								SYS_POOL_INFO_T* pool_ptr,  //output the system pool information
								uint32 flag  // POOL_INFO_SEEK_FIRST -- first time get the pool information; POOL_INFO_SEEK_NEXT -- get the next pool information
								);


#ifdef LOW_MEMORY_SUPPORT
PUBLIC xSignalHeader SCI_Create_Signal(xSignalHeader _SIG_PTR,  uint16 _SIG_CODE, 
    uint16 _SIG_SIZE, BLOCK_ID _SENDER);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _OS_API_H

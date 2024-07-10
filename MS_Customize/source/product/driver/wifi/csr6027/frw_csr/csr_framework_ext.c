/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #7 $
****************************************************************************/

#include "os_api.h"

#ifdef CSR_THROUGHPUT_TEST_TOOL
#include "wifi_drv.h"
#include "socket_types.h"
#include "socket_api.h"
#include "rtc_drvapi.h"
#endif
#include "sci_log.h"
#include "tcpip_api.h"

#include "csr_framework_ext.h"
#include "csr_panic.h"
#include "csr_util.h"
#include "csr_spi_master.h"
#include "csr_pmem.h"

#include "csr_gpio_debug.h"
#if 0
#include "chip_plf_export.h"
#include "gpio_drvapi.h"
#include "gpio_phy.h"
#endif
#ifdef PLATFORM_SC6530
#include "priority_system.h"
#include "csr_gsched_init.h"

#define CSR_SCI_NAME_SIZE                                      (20)
#define CSR_THREAD_DEFAULT_STACK_SIZE        (4 * 1024)
#define CSR_WIFI_HIP_BH_THREAD_STACK_SIZE 6000
#define CSR_THREAD_DEFAULT_QUEUE_NUM        (50)  // BT uses 200, bruce suggests 50    
#define CSR_THREAD_PRIORITY_HIGHEST   (PRI_CSR_WIFI_BH_TASK)    // 16
#define CSR_THREAD_PRIORITY_NORMAL    (PRI_CSR_FW_TASK)    //30
/*
#define CSR_DEFAULT_PRIORITY                 (PRI_CSR_FW_TASK)  //30
#define CSR_HIGH_PRIORITY                      (PRI_CSR_WIFI_BH_TASK)  //16
#define CSR_THREAD_PRIORITY_HIGHEST   (PRI_CSR_THREAD_HIGHEST)    // 16
#define CSR_THREAD_PRIORITY_HIGH         (PRI_CSR_THREAD_HIGH)    // 26
#define CSR_THREAD_PRIORITY_NORMAL    (PRI_CSR_THREAD_NORMAL)    //30
#define CSR_THREAD_PRIORITY_LOW          (PRI_CSR_THREAD_LOW)    //30
#define CSR_THREAD_PRIORITY_LOWEST    (PRI_CSR_THREAD_LOWEST)    //30
*/
#define CSR_IPERF_TASK_HIGH                   (PRI_IPERF_TASK_HIGH)    //19
#define CSR_IPERF_TASK_NORMAL              (PRI_IPERF_TASK_NORMAL)    //26
#define CSR_IPERF_TASK_LOW                    (PRI_IPERF_TASK_LOW)    //31

#define CSR_QUEUE_LIMIT     50
#define CSR_EVENT_LIMIT     50
#define CSR_MUTEX_LIMIT     50

void *CsrWiFiSchedInit(void);
static void CsrIToA(int n, char s[]);

static int csrQueueCount = 0;
static int csrEventCount = 0;
static int csrMutexCount = 0;
static char csrQueueName[CSR_QUEUE_LIMIT][CSR_SCI_NAME_SIZE];
static char csrEventName[CSR_EVENT_LIMIT][CSR_SCI_NAME_SIZE];
static char csrMutexName[CSR_MUTEX_LIMIT][CSR_SCI_NAME_SIZE];
static char csrGMutexName[CSR_SCI_NAME_SIZE] = "CSR_GMUT-0";

#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "priority_system.h"
#include "csr_gsched_init.h"

#define CSR_SCI_NAME_SIZE                                      (20)
#define CSR_THREAD_DEFAULT_STACK_SIZE        (4 * 1024)
#define CSR_WIFI_HIP_BH_THREAD_STACK_SIZE 6000
#define CSR_THREAD_DEFAULT_QUEUE_NUM        (50)  // BT uses 200, bruce suggests 50    
#define CSR_THREAD_PRIORITY_HIGHEST   (PRI_CSR_WIFI_BH_TASK)    // 16
#define CSR_THREAD_PRIORITY_NORMAL    (PRI_CSR_FW_TASK)    //30
/*
#define CSR_DEFAULT_PRIORITY                 (PRI_CSR_FW_TASK)  //30
#define CSR_HIGH_PRIORITY                      (PRI_CSR_WIFI_BH_TASK)  //16
#define CSR_THREAD_PRIORITY_HIGHEST   (PRI_CSR_THREAD_HIGHEST)    // 16
#define CSR_THREAD_PRIORITY_HIGH         (PRI_CSR_THREAD_HIGH)    // 26
#define CSR_THREAD_PRIORITY_NORMAL    (PRI_CSR_THREAD_NORMAL)    //30
#define CSR_THREAD_PRIORITY_LOW          (PRI_CSR_THREAD_LOW)    //30
#define CSR_THREAD_PRIORITY_LOWEST    (PRI_CSR_THREAD_LOWEST)    //30
*/
#define CSR_IPERF_TASK_HIGH                   (PRI_IPERF_TASK_HIGH)    //19
#define CSR_IPERF_TASK_NORMAL              (PRI_IPERF_TASK_NORMAL)    //26
#define CSR_IPERF_TASK_LOW                    (PRI_IPERF_TASK_LOW)    //31

#define CSR_QUEUE_LIMIT     50
#define CSR_EVENT_LIMIT     50
#define CSR_MUTEX_LIMIT     50

void *CsrWiFiSchedInit(void);
static void CsrIToA(int n, char s[]);

static int csrQueueCount = 0;
static int csrEventCount = 0;
static int csrMutexCount = 0;
static char csrQueueName[CSR_QUEUE_LIMIT][CSR_SCI_NAME_SIZE];
static char csrEventName[CSR_EVENT_LIMIT][CSR_SCI_NAME_SIZE];
static char csrMutexName[CSR_MUTEX_LIMIT][CSR_SCI_NAME_SIZE];
static char csrGMutexName[CSR_SCI_NAME_SIZE] = "CSR_GMUT-0";

#else
#define CSR_SCI_NAME_SIZE                (8)
#define CSR_THREAD_DEFAULT_STACK_SIZE    (4 * 1024)
#define CSR_THREAD_DEFAULT_QUEUE_NUM     (50)  // BT uses 200, bruce suggests 50    
#endif

#if 0 //Enable memory debug
#define CSR_FW_ALLOC_DEBUG_ENABLE
#define CSR_MEASURE_PEAK_MEM_USAGE
void csrDebugDumpRamUsage (void);
#endif

/*lint -e838 -e552 -e843 -e844*/
#ifdef CSR_MEASURE_PEAK_MEM_USAGE
int csr_dynamic_ram_usage_chunks = 0;
int csr_dynamic_ram_usage_bytes = 0;
int csr_dynamic_ram_usage_peak_bytes = 0;
#endif

#ifdef CSR_THROUGHPUT_TEST_TOOL
// Mapping platform socket functions to general socket functions
#define accept(a,b,c)          sci_sock_accept(a,b,c)
#define bind(a,b,c)             sci_sock_bind(a,b,c)
#define close(a)                  sci_sock_socketclose(a)
#define connect(a,b,c)        sci_sock_connect(a,b,c)
#define listen(a,b)              sci_sock_listen(a, b)
#define recv(a,b,c,d)          sci_sock_recv(a,b,c,d)
#define send(a,b,c,d)          sci_sock_send(a,b,c,d)
#define socket(a,b,c)          sci_sock_socket(a,b,c,0)
#define setsockopt(a,b,c)   sci_sock_setsockopt(a,b,c)
#define getsockopt(a,b,c)   sci_sock_getsockopt(a,b,c)
// Defines used by throughput measurement tool
#define CSR_BLOCKS_TO_SEND  (100)
#define CSR_BLOCK_SIZE           (65536)
#define CSR_BUFF_SIZE             (65536)
#define CSR_TCP_SPEED_SOCK  (6666)

extern unsigned long inet_addr(const char * cp);
#endif
#define CSR_MEM_DMA_HDR_BLOCK_SIZE 14
#define CSR_MEM_DMA_BLOCK_COUNT 160
#define CSR_MEM_DMA_ALIGN_SIZE 4096
#define CSR_MEM_DMA_OFFSET 28 /* Needs to be 4 byte aligned */
#ifdef CSR_CSPI_JOIN_READS
// CSR: Add 32 bytes in order to make room for headers towards IP stack while keeping 32 byte alignment
#define CSR_MEM_DMA_READ_OFFSET     (CSR_CSPI_BURST_READ_HEADER_LENGTH+32)
#else
#define CSR_MEM_DMA_READ_OFFSET     (32)
#endif
#ifdef CSR_CSPI_JOIN_WRITES
#define CSR_MEM_DMA_WRITE_OFFSET    (CSR_CSPI_BURST_WRITE_HEADER_LENGTH)
#else
#define CSR_MEM_DMA_WRITE_OFFSET    (0)
#endif


typedef struct tagCsrMemoryDmaPoolType
{
    CsrUint8 data[CSR_MEM_DMA_BLOCK_SIZE];
    CsrUint32 inUse;
	struct tagCsrMemoryDmaPoolType *next;
} CsrMemoryDmaPoolType;

static CsrUint8 *CsrMemoryDmaTmp; 

static CsrMemoryDmaPoolType *CsrMemoryDmaPoolStart;

extern void CsrDebugTraceLow(const char* format, ...);

typedef struct
{
    void (* threadFunction)(void *pointer);
    void *dataPtr;
} spreadtrumFunctionWrapperDataStruct;

static SCI_SEMAPHORE_PTR globalMutex;
#ifdef CSR_THROUGHPUT_TEST_TOOL
static CsrBool tcp_flg = FALSE;
static char *buff = NULL; // Buffer used by throughput measurement tool
extern int wifi_supp_is_on;
#endif
    
#ifdef CSR_THROUGHPUT_TEST_TOOL
static void wlan_csr_cmd(WIFI_CMD_PRINTF_FUNC oprintf, int argc, char *argv[]);
#endif
static void spreadtrumFwExtFunctionWrapper(uint32 argc, void *argv)
{
    spreadtrumFunctionWrapperDataStruct *param = (spreadtrumFunctionWrapperDataStruct *) argv;

    param->threadFunction(param->dataPtr);

    SCI_FREE(param);
    SCI_ThreadExit();
} /*lint !e438*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventCreate
 *
 *  DESCRIPTION
 *      Creates an event and returns a handle to the created event.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS          in case of success
 *          CSR_FE_RESULT_NO_MORE_EVENTS   in case of out of event resources
 *          CSR_FE_RESULT_INVALID_POINTER  in case the eventHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
int csr_event_resource_count = 0; /*lint !e552*/
CsrResult CsrEventCreate(CsrEventHandle *eventHandle)
{
#ifdef PLATFORM_SC6530
    char tmp[8];

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_CREATE);
    CsrStrCpy(csrEventName[csrEventCount], "CSR_EVNT-");
    CsrMemSet(tmp, '\0', sizeof(tmp));
    CsrIToA(csrEventCount, tmp);
    CsrStrCat(csrEventName[csrEventCount], tmp);
    /*
    CsrDebugTraceLow("*****%s In: %ld *eventHandle, %ld eventHandle, %d csrEventCount, %s csrEventName, %d csr_event_resource_count",
        __func__,
        (CsrUint32)*eventHandle,
        (CsrUint32)eventHandle,
        csrEventCount,
        csrEventName[csrEventCount],
        csr_event_resource_count);
    */
    *eventHandle = SCI_CreateEvent(csrEventName[csrEventCount]);
    if (*eventHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    if(++csrEventCount >= CSR_EVENT_LIMIT)
        csrEventCount = 0;
    csr_event_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
    /*
    CsrDebugTraceLow("*****%s Out: %ld *eventHandle, %ld eventHandle, %d csrEventCount, %s csrEventName, %d csr_event_resource_count",
        __func__,
        (CsrUint32)*eventHandle,
        (CsrUint32)eventHandle,
        csrEventCount,
        csrEventName[csrEventCount],
        csr_event_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    char tmp[8];

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_CREATE);
    CsrStrCpy(csrEventName[csrEventCount], "CSR_EVNT-");
    CsrMemSet(tmp, '\0', sizeof(tmp));
    CsrIToA(csrEventCount, tmp);
    CsrStrCat(csrEventName[csrEventCount], tmp);
    /*
    CsrDebugTraceLow("*****%s In: %ld *eventHandle, %ld eventHandle, %d csrEventCount, %s csrEventName, %d csr_event_resource_count",
        __func__,
        (CsrUint32)*eventHandle,
        (CsrUint32)eventHandle,
        csrEventCount,
        csrEventName[csrEventCount],
        csr_event_resource_count);
    */
    *eventHandle = SCI_CreateEvent(csrEventName[csrEventCount]);
    if (*eventHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    if(++csrEventCount >= CSR_EVENT_LIMIT)
        csrEventCount = 0;
    csr_event_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
    /*
    CsrDebugTraceLow("*****%s Out: %ld *eventHandle, %ld eventHandle, %d csrEventCount, %s csrEventName, %d csr_event_resource_count",
        __func__,
        (CsrUint32)*eventHandle,
        (CsrUint32)eventHandle,
        csrEventCount,
        csrEventName[csrEventCount],
        csr_event_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#else
    char name[CSR_SCI_NAME_SIZE] = "CSREVNT";
    
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_CREATE);

    *eventHandle = SCI_CreateEvent(name);
    if (*eventHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    csr_event_resource_count ++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_CREATE);
    return CSR_RESULT_SUCCESS;
#endif    
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventWait
 *
 *  DESCRIPTION
 *      Wait for the event to be set.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS              in case of success
 *          CSR_FE_RESULT_TIMEOUT              in case of timeout
 *          CSR_FE_RESULT_INVALID_HANDLE       in case the eventHandle is invalid
 *          CSR_FE_RESULT_INVALID_POINTER      in case the eventBits pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrEventWait(CsrEventHandle *eventHandle, CsrUint16 timeoutInMs, CsrUint32 *eventBits)
{
    uint32 status;
    uint32 suspend;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_WAIT);
    if (*eventHandle == NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    if (eventBits == NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    if (timeoutInMs == 0)
    {
        suspend = SCI_NO_WAIT;
    }
    else if (timeoutInMs == CSR_EVENT_WAIT_INFINITE)
    {
        suspend = SCI_WAIT_FOREVER;
    }
    else
    {
        suspend = (CsrUint32) timeoutInMs;
    }

    status = SCI_GetEvent(*eventHandle, 0xFFFFFFFF, SCI_OR_CLEAR, (uint32 *) eventBits, suspend);

    if (status == SCI_NO_EVENTS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
        return CSR_FE_RESULT_TIMEOUT;
    }
    else if (status == SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
        return CSR_RESULT_SUCCESS;
    }
    else if (status == SCI_ERROR)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_WAIT);
    return CSR_RESULT_FAILURE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventSet
 *
 *  DESCRIPTION
 *      Set an event.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS              in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE       in case the eventHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrEventSet(CsrEventHandle *eventHandle, CsrUint32 eventBits)
{
    uint32 status;
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_SET);

	if(*eventHandle == NULL)
		return CSR_FE_RESULT_INVALID_HANDLE;
	
    status = SCI_SetEvent(*eventHandle, eventBits, SCI_OR);
    if (status != SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_SET);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_SET);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventDestroy
 *
 *  DESCRIPTION
 *      Destroy the event associated.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrEventDestroy(CsrEventHandle *eventHandle)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_EVENT_DESTROY);

	if(*eventHandle == NULL)
		return;
		
    SCI_DeleteEvent(*eventHandle);
    csr_event_resource_count--;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_EVENT_DESTROY);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexCreate
 *
 *  DESCRIPTION
 *      Create a mutex and return a handle to the created mutex.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_MUTEXES   in case of out of mutex resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case the mutexHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
int csr_sema_resource_count = 0; /*lint !e552*/
CsrResult CsrMutexCreate(CsrMutexHandle *mutexHandle)
{
#ifdef PLATFORM_SC6530
    char tmp[8];

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_CREATE);
    CsrStrCpy(csrMutexName[csrMutexCount], "CSR_MUT-");
    CsrMemSet(tmp, '\0', sizeof(tmp));
    CsrIToA(csrMutexCount, tmp);
    CsrStrCat(csrMutexName[csrMutexCount], tmp);
    /*
    CsrDebugTraceLow("+++++%s In: %ld *mutexHandle, %ld mutexHandle, %d csrMutexCount, %s csrMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)*mutexHandle,
        (CsrUint32)mutexHandle,
        csrMutexCount,
        csrMutexName[csrMutexCount],
        csr_sema_resource_count);
    */
    *mutexHandle = SCI_CreateSemaphore(csrMutexName[csrMutexCount], 1);
    if (*mutexHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    if(++csrMutexCount >= CSR_MUTEX_LIMIT)
        csrMutexCount = 0;
    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("+++++%s Out: %ld *mutexHandle, %ld mutexHandle, %d csrMutexCount, %s csrMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)*mutexHandle,
        (CsrUint32)mutexHandle,
        csrMutexCount,
        csrMutexName[csrMutexCount],
        csr_sema_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    char tmp[8];

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_CREATE);
    CsrStrCpy(csrMutexName[csrMutexCount], "CSR_MUT-");
    CsrMemSet(tmp, '\0', sizeof(tmp));
    CsrIToA(csrMutexCount, tmp);
    CsrStrCat(csrMutexName[csrMutexCount], tmp);
    /*
    CsrDebugTraceLow("+++++%s In: %ld *mutexHandle, %ld mutexHandle, %d csrMutexCount, %s csrMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)*mutexHandle,
        (CsrUint32)mutexHandle,
        csrMutexCount,
        csrMutexName[csrMutexCount],
        csr_sema_resource_count);
    */
    *mutexHandle = SCI_CreateSemaphore(csrMutexName[csrMutexCount], 1);
    if (*mutexHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    if(++csrMutexCount >= CSR_MUTEX_LIMIT)
        csrMutexCount = 0;
    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("+++++%s Out: %ld *mutexHandle, %ld mutexHandle, %d csrMutexCount, %s csrMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)*mutexHandle,
        (CsrUint32)mutexHandle,
        csrMutexCount,
        csrMutexName[csrMutexCount],
        csr_sema_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#else
    char name[CSR_SCI_NAME_SIZE] = "CSRMUTX";

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_CREATE);

    *mutexHandle = SCI_CreateSemaphore(name, 1);
    if (*mutexHandle == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_CREATE);
    return CSR_RESULT_SUCCESS;
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexLock
 *
 *  DESCRIPTION
 *      Lock the mutex refered to by the provided handle.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrMutexLock(CsrMutexHandle *mutexHandle)
{
    uint32 status;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_LOCK);
    
    status = SCI_GetSemaphore(*mutexHandle, SCI_WAIT_FOREVER);
    if (status != SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_LOCK);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_LOCK);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexUnlock
 *
 *  DESCRIPTION
 *      Unlock the mutex refered to by the provided handle.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrMutexUnlock(CsrMutexHandle *mutexHandle)
{
    uint32 status;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_UNLOCK);

    status = SCI_PutSemaphore(*mutexHandle);
    if (status != SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_UNLOCK);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_UNLOCK);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexDestroy
 *
 *  DESCRIPTION
 *      Destroy the previously created mutex.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMutexDestroy(CsrMutexHandle *mutexHandle)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MUTEX_DESTROY);
    SCI_DeleteSemaphore(*mutexHandle);
    csr_sema_resource_count--;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MUTEX_DESTROY);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexCreate
 *
 *  DESCRIPTION
 *      Create a global mutex.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_MUTEXES   in case of out of mutex resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case the mutexHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrGlobalMutexCreate(void)
{
#ifdef PLATFORM_SC6530
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("xxxxx%s In: %ld globalMutex, %s csrGMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)globalMutex,
        csrGMutexName,
        csr_sema_resource_count);
    */
    globalMutex = SCI_CreateSemaphore(csrGMutexName, 1);
    if (globalMutex == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("xxxxx%s Out: %ld globalMutex, %s csrGMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)globalMutex,
        csrGMutexName,
        csr_sema_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("xxxxx%s In: %ld globalMutex, %s csrGMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)globalMutex,
        csrGMutexName,
        csr_sema_resource_count);
    */
    globalMutex = SCI_CreateSemaphore(csrGMutexName, 1);
    if (globalMutex == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }
    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
    /*
    CsrDebugTraceLow("xxxxx%s Out: %ld globalMutex, %s csrGMutexName, %d csr_sema_resource_count",
        __func__,
        (CsrUint32)globalMutex,
        csrGMutexName,
        csr_sema_resource_count);
    */
    return CSR_RESULT_SUCCESS;
#else
    char name[CSR_SCI_NAME_SIZE] = "CSRGMUT";

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_CREATE);

    globalMutex = SCI_CreateSemaphore(name, 1);
    if (globalMutex == SCI_NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    csr_sema_resource_count++;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_CREATE);
    return CSR_RESULT_SUCCESS;
#endif    
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexLock
 *
 *  DESCRIPTION
 *      Lock the global mutex.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrGlobalMutexLock(void)
{
    uint32 status;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_LOCK);
    
    status = SCI_GetSemaphore(globalMutex, SCI_WAIT_FOREVER);
    if (status != SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_LOCK);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_LOCK);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexUnlock
 *
 *  DESCRIPTION
 *      Unlock the global mutex.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrGlobalMutexUnlock(void)
{
    uint32 status;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_UNLOCK);

    status = SCI_PutSemaphore(globalMutex);
    if (status != SCI_SUCCESS)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_UNLOCK);
        return CSR_FE_RESULT_INVALID_HANDLE;
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_UNLOCK);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexDestroy
 *
 *  DESCRIPTION
 *      Destroy the global mutex.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrGlobalMutexDestroy(void)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_GLOBAL_MUTEX_DESTROY);
    SCI_DeleteSemaphore(globalMutex);
    csr_sema_resource_count--;
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_GLOBAL_MUTEX_DESTROY);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadCreate
 *
 *  DESCRIPTION
 *      Create thread function and return a handle to the created thread.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_THREADS   in case of out of thread resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case one of the supplied pointers is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadCreate(void (* threadFunction)(void *pointer), void *pointer, CsrUint32 stackSize, CsrUint16 priority, CsrCharString *threadName, CsrThreadHandle *threadHandle)
{
    // BLOCK_ID tId;
    uint32 stackSizeToUse;
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    char queueName[CSR_SCI_NAME_SIZE] = "CSRTH_Q";
#endif
    spreadtrumFunctionWrapperDataStruct *param = NULL;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_THREAD_CREATE);

    if (threadFunction == NULL || threadHandle == NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_CREATE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    param = SCI_ALLOC(sizeof(spreadtrumFunctionWrapperDataStruct));
    param->threadFunction = threadFunction;
    param->dataPtr = pointer;
    
    if (stackSize < CSR_THREAD_DEFAULT_STACK_SIZE)
    {
        stackSizeToUse = CSR_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        stackSizeToUse = stackSize;
    }

#ifdef PLATFORM_SC6530
    // To safeguard 68xx/88xx priority being set
    if (priority < 32)
        priority += 48;
    
    // Set queue name to thread name + "_Q"
    /*
    CsrDebugTraceLow("qqqqq%s In: %d csrQueueCount, %s csrQueueName",
        __func__,      
        csrQueueCount,
        csrQueueName);
    */
    CsrStrCpy(csrQueueName[csrQueueCount], threadName);
    CsrStrCat(csrQueueName[csrQueueCount], "_Q");
    *threadHandle = (void *)SCI_CreateThread(threadName,
                           csrQueueName[csrQueueCount],
                           spreadtrumFwExtFunctionWrapper,
                           0,
                           param,
                           stackSizeToUse,
                           CSR_THREAD_DEFAULT_QUEUE_NUM,
                           priority,
                           SCI_PREEMPT,
                           SCI_AUTO_START);
    if(++csrQueueCount >= CSR_QUEUE_LIMIT)
        csrQueueCount = 0;
    /*
    CsrDebugTraceLow("qqqqq%s Out: %d csrQueueCount, %s csrQueueName",
        __func__,      
        csrQueueCount,
        csrQueueName);
    */
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    // To safeguard 68xx/88xx priority being set
    if (priority < 32)
        priority += 48;
    
    // Set queue name to thread name + "_Q"
    /*
    CsrDebugTraceLow("qqqqq%s In: %d csrQueueCount, %s csrQueueName",
        __func__,      
        csrQueueCount,
        csrQueueName);
    */
    CsrStrCpy(csrQueueName[csrQueueCount], threadName);
    CsrStrCat(csrQueueName[csrQueueCount], "_Q");
    *threadHandle = (void *)SCI_CreateThread(threadName,
                           csrQueueName[csrQueueCount],
                           spreadtrumFwExtFunctionWrapper,
                           0,
                           param,
                           stackSizeToUse,
                           CSR_THREAD_DEFAULT_QUEUE_NUM,
                           priority,
                           SCI_PREEMPT,
                           SCI_AUTO_START);
    if(++csrQueueCount >= CSR_QUEUE_LIMIT)
        csrQueueCount = 0;
    /*
    CsrDebugTraceLow("qqqqq%s Out: %d csrQueueCount, %s csrQueueName",
        __func__,      
        csrQueueCount,
        csrQueueName);
    */
#else
    *threadHandle = (void *)SCI_CreateThread(threadName,
                           queueName,
                           spreadtrumFwExtFunctionWrapper,
                           0,
                           param,
                           stackSizeToUse,
                           CSR_THREAD_DEFAULT_QUEUE_NUM,
                           priority,
                           SCI_PREEMPT,
                           SCI_AUTO_START);
#endif
    if ((unsigned long)*threadHandle == (unsigned long)SCI_INVALID_BLOCK_ID)
    {
        *threadHandle = 0;
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_CREATE);
        return CSR_FE_RESULT_NO_MORE_THREADS;
    }

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_CREATE);
    return CSR_RESULT_SUCCESS;
}

#ifdef PLATFORM_SC6530
void *CsrWiFiSchedInit(void)
{
    // Set CSR_sched-x thread here
    return CsrSchedInit(0, CSR_THREAD_PRIORITY_NORMAL, CSR_THREAD_DEFAULT_STACK_SIZE);
}
#elif defined(PLARFORM_SC6531EFM)
void *CsrWiFiSchedInit(void)
{
    // Set CSR_sched-x thread here
    return CsrSchedInit(0, CSR_THREAD_PRIORITY_NORMAL, CSR_THREAD_DEFAULT_STACK_SIZE);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadCreateExt
 *
 *  DESCRIPTION
 *      Create thread function and return a handle to the created thread. Allow functions parameters to be set in platform codes.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_THREADS   in case of out of thread resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case one of the supplied pointers is invalid
 *
 *----------------------------------------------------------------------------*/ 
CsrResult CsrThreadCreateExt(void (* threadFunction)(void *pointer), void *pointer, CsrUint32 stackSize, CsrUint16 priority, CsrCharString *threadName, CsrThreadHandle *threadHandle)
{
    CsrResult result;
    
    if (!CsrStrCmp("CSR_WIFI_BH", threadName))
    {
        // Set CSR_WIFI_BH thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_WIFI_HIP_BH_THREAD_STACK_SIZE,
            CSR_THREAD_PRIORITY_HIGHEST,
            threadName,
            threadHandle);
    }
#ifdef PLATFORM_SC6530
    else if (!CsrStrCmp("CSR_WIFI_SCH", threadName))
    {
        // Set CSR_WIFI_SCH thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_THREAD_PRIORITY_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_WMH", threadName))
    {
        // Set CSR_WIFI_WMH thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_THREAD_PRIORITY_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_RXTCP", threadName))
    {
        // Set CSR_WIFI_CMD_RXTCP thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_TXTCP", threadName))
    {
        // Set CSR_WIFI_CMD_TXTCP thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_SPT", threadName))
    {
        // Set CSR_WIFI_CMD_SPT thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
#endif
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    else if (!CsrStrCmp("CSR_WIFI_SCH", threadName))
    {
        // Set CSR_WIFI_SCH thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_THREAD_PRIORITY_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_WMH", threadName))
    {
        // Set CSR_WIFI_WMH thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_THREAD_PRIORITY_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_RXTCP", threadName))
    {
        // Set CSR_WIFI_CMD_RXTCP thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_TXTCP", threadName))
    {
        // Set CSR_WIFI_CMD_TXTCP thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
    else if (!CsrStrCmp("CSR_WIFI_SPT", threadName))
    {
        // Set CSR_WIFI_CMD_SPT thread here
        result = CsrThreadCreate(threadFunction,
            pointer,
            CSR_THREAD_DEFAULT_STACK_SIZE,
            CSR_IPERF_TASK_NORMAL,
            threadName,
            threadHandle);
    }
#endif

    else
    {
        CsrDebugTraceLow("%s: Unrecognised thread name %s",  __func__, threadName);
        *threadHandle = 0;
        result = CSR_RESULT_FAILURE;
    }
    return result;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadGetHandle
 *
 *  DESCRIPTION
 *      Return thread handle of calling thread.
 *
 *  RETURNS
 *      Possible values:
 *	    CSR_RESULT_SUCCESS		   in case of success
 *	    CSR_FE_RESULT_INVALID_POINTER  in case the threadHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadGetHandle(CsrThreadHandle *threadHandle)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_THREAD_GET_HANDLE);
    if (threadHandle == NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_GET_HANDLE);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    *(BLOCK_ID *)threadHandle = SCI_IdentifyThread();

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_GET_HANDLE);
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadEqual
 *
 *  DESCRIPTION
 *      Compare thread handles
 *
 *  RETURNS
 *      Possible values:
 *	    CSR_RESULT_SUCCESS             in case thread handles are identical
 *	    CSR_FE_RESULT_INVALID_POINTER  in case either threadHandle pointer is invalid
 *	    CSR_RESULT_FAILURE             otherwise
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadEqual(CsrThreadHandle *threadHandle1, CsrThreadHandle *threadHandle2)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_THREAD_EQUAL);
    if (threadHandle1 == NULL || threadHandle2 == NULL)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_EQUAL);
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    if (*threadHandle1 == *threadHandle2)
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_EQUAL);
        return CSR_RESULT_SUCCESS;
    }
    else
    {
        CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_EQUAL);
        return CSR_RESULT_FAILURE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadSleep
 *
 *  DESCRIPTION
 *      Sleep for a given period.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadSleep(CsrUint32 sleepTimeInMs)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_THREAD_SLEEP);
    SCI_Sleep(sleepTimeInMs);
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_THREAD_SLEEP);
}


/* SR 21743 */
void CsrThreadCleanupResource (CsrThreadHandle hdle)
{
    SCI_TerminateThread ((BLOCK_ID)hdle);
    SCI_DeleteThread ((BLOCK_ID)hdle);
}




/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemAlloc
 *
 *  DESCRIPTION
 *      Allocate dynamic memory of a given size.
 *
 *  RETURNS
 *      Pointer to allocated memroy, or NULL in case of failure.
 *      Allocated memory is not initialised.
 *
 *----------------------------------------------------------------------------*/
/*#define CSR_FW_ALLOC_DEBUG_ENABLE */  // Enable memory debugging
/* #define CSR_MEMALLOC_PROFILING_EXT */

#ifdef CSR_FW_ALLOC_DEBUG_ENABLE
#define CSR_FW_EXT_MAGIC_NUMBER_HEAD (0x75938bfa)
#define CSR_FW_EXT_MAGIC_NUMBER_TAIL (0x98bf3487)

typedef struct
{
    CsrUint32   magicNumber;
    size_t      size;
#ifdef CSR_MEMALLOC_PROFILING_EXT
    #define CSR_IDX_INVALID (-1)
    int         idx;
    char*       file;
    int         line;
#endif
} csrFwExtAllocMagicHeader;

static void *csrPmemRangeStart = (void *)~0UL;
static void *csrPmemRangeEnd = NULL;

#ifdef CSR_MEMALLOC_PROFILING_EXT
#define CSR_ALLOCHDR_ARRAY_SIZE     (1024)
static csrFwExtAllocMagicHeader* allocHeaders[CSR_ALLOCHDR_ARRAY_SIZE] = {NULL,};

static void mem_profile_add (csrFwExtAllocMagicHeader*  ptr)
{
    int i;
    for (i=0; i<CSR_ALLOCHDR_ARRAY_SIZE; i++) {
        if (allocHeaders[i] != NULL)
            continue;
        ptr->idx = i;
        allocHeaders[i] = ptr;
        return;
    }
    CsrDebugTraceLow("~~ FW_EXT: mem_profile_add - array is full (file=%s, line=%d)",
        ptr->file, ptr->line);
    ptr->idx = CSR_IDX_INVALID;
}

static void mem_profile_remove (csrFwExtAllocMagicHeader*  ptr)
{
    if (ptr == NULL)
        return;
    
    if (ptr->idx == CSR_IDX_INVALID || ptr->idx >= CSR_ALLOCHDR_ARRAY_SIZE ||
        allocHeaders[ptr->idx] == NULL) {
        CsrDebugTraceLow("~~ FW_EXT: mem_profile_remove - Could not find (file=%s, line=%d)",
            ptr->file, ptr->line);
        return;
    }
    allocHeaders[ptr->idx] = NULL;
    ptr->idx = CSR_IDX_INVALID;
}

void mem_profile_dump (void)
{
    int i;
    for (i=0; i<CSR_ALLOCHDR_ARRAY_SIZE; i++) {
        if (allocHeaders[i] == NULL)
            continue;
        CsrDebugTraceLow("%6d @%s:%d", allocHeaders[i]->size, allocHeaders[i]->file, allocHeaders[i]->line);
    }
}
#endif /* CSR_MEMALLOC_PROFILING_EXT */
#endif /* CSR_FW_ALLOC_DEBUG_ENABLE */

#define MEM_ALLOCATION_RETRY_MAX 5

void * CsrMemAllocExt(size_t size, const char* file, const int line)
{
    void *pointer;

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MEM_ALLOC);

#ifdef CSR_FW_ALLOC_DEBUG_ENABLE
    /* Reserve room for header and tail */
    pointer = SCI_ALLOC_APP((CsrUint32)(size + sizeof(csrFwExtAllocMagicHeader) + sizeof(CsrUint32)));
#else
    pointer = SCI_ALLOC_APP(size);
#endif

    if (pointer == SCI_NULL)
    {
    	CsrUint16 retry_mem_alloc = 0;

		for(retry_mem_alloc =0; retry_mem_alloc < MEM_ALLOCATION_RETRY_MAX; retry_mem_alloc ++)
		{
			pointer = SCI_ALLOC_APP(size);

			if(pointer == SCI_NULL)
			{
				CsrDebugTraceLow("CsrMemAllocExt: memory allocation retry still failed. Retries count:%d", retry_mem_alloc);
				CsrThreadSleep(3);
				continue;
			}
			else
			{
				CsrDebugTraceLow("CsrMemAllocExt: memory allocation retry successfully after %d retries", retry_mem_alloc);
				break;
			}
		}
    	if(retry_mem_alloc == MEM_ALLOCATION_RETRY_MAX)
    	{
        	CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_HEAP_EXHAUSTION, "FW_EXT: CsrMemAlloc - Memory allocation failed");
			return NULL;
    	}
    }
#ifdef CSR_FW_ALLOC_DEBUG_ENABLE
    else
    {
        /* Update valid memory ranges */
        /* Continously keep track of the valid memory ranges provided by the memory system in order to detect invalid freeing of memory */
        if (pointer < csrPmemRangeStart)
        {
            csrPmemRangeStart = pointer;
        }

        if (pointer > csrPmemRangeEnd)
        {
            csrPmemRangeEnd = pointer;
        }
        
        /* Add stuff for error detection */
        /* By adding an identifiable number at the beginning and end of allocated memory, it is possible to detect when out-of-bounds writing
           has occured. The size is required in the header in order to find the tail. */
        ((csrFwExtAllocMagicHeader *)pointer)->magicNumber = CSR_FW_EXT_MAGIC_NUMBER_HEAD;
        ((csrFwExtAllocMagicHeader *)pointer)->size = size;
        //CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_FW_EXT_MAGIC_NUMBER_TAIL, (CsrUint16 *)((CsrInt8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + size));

        *(CsrUint8 *)((CsrUint8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + size) = (CsrUint8)(CSR_FW_EXT_MAGIC_NUMBER_TAIL & 0x000000FF);
        *(CsrUint8 *)((CsrUint8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + size + 1) = (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 8) & 0x000000FF);
        *(CsrUint8 *)((CsrUint8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + size + 2) = (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 16) & 0x000000FF);
        *(CsrUint8 *)((CsrUint8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + size + 3) = (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 24) & 0x000000FF);

#ifdef CSR_MEMALLOC_PROFILING_EXT
        ((csrFwExtAllocMagicHeader *)pointer)->file = file;
        ((csrFwExtAllocMagicHeader *)pointer)->line = line;
#endif
    }
    
#ifdef CSR_MEASURE_PEAK_MEM_USAGE
    csr_dynamic_ram_usage_chunks ++;
    csr_dynamic_ram_usage_bytes += size;
    if (csr_dynamic_ram_usage_bytes > csr_dynamic_ram_usage_peak_bytes) {
        csr_dynamic_ram_usage_peak_bytes = csr_dynamic_ram_usage_bytes;
     }
	csrDebugDumpRamUsage();
#endif
#ifdef CSR_MEMALLOC_PROFILING_EXT
    mem_profile_add (pointer);
#endif
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_ALLOC);
    return (CsrInt8 *)pointer + sizeof(csrFwExtAllocMagicHeader); /*lint !e613*/
#else
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_ALLOC);
    return pointer;
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemCalloc
 *
 *  DESCRIPTION
 *      Allocate dynamic memory of a given size calculated as the
 *      numberOfElements times the elementSize.
 *
 *  RETURNS
 *      Pointer to allocated memroy, or NULL in case of failure.
 *      Allocated memory is zero initialised.
 *
 *----------------------------------------------------------------------------*/
void * CsrMemCalloc(size_t numberOfElements, size_t elementSize)
{
    void *pointer;
    size_t size = numberOfElements * elementSize;
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MEM_CALLOC);
    pointer = CsrMemAllocExt(size, __FILE__, __LINE__);
    if (pointer != NULL)
    {
        memset(pointer, 0, size);
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_CALLOC);
    return pointer;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemFree
 *
 *  DESCRIPTION
 *      Free dynamic allocated memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMemFreeExt(void *pointer, char *file, int line)
{
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MEM_FREE);
    if (pointer != NULL)
    {
        CsrUint32 result = SCI_SUCCESS;
#ifdef CSR_FW_ALLOC_DEBUG_ENABLE
        csrFwExtAllocMagicHeader *header = (csrFwExtAllocMagicHeader *)((CsrInt8 *)pointer - sizeof(csrFwExtAllocMagicHeader));

#ifdef CSR_MEMALLOC_PROFILING_EXT
        mem_profile_remove (header);
#endif
        /* Check valid memory ranges */
        if (((void *)header < csrPmemRangeStart) || ((void *)header > csrPmemRangeEnd))
        {
            CsrDebugTraceLow("FW_EXT: CsrMemFree - outside valid range [%s:%d] (header: p%p, start: p%p, end: p%p)",
                              file, line, header, csrPmemRangeStart, csrPmemRangeEnd);
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "CsrMemFree: Outside valid range");
            CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_FREE);
            return;
        }

        /* Check that we are not double-freeing */
        if (header->size == 0xFFFFFFFF)
        {
            CsrDebugTraceLow("FW_EXT: CsrMemFree double free (pointer: %p, magic_h: 0x%0x, size: %d)",
                              pointer, header->magicNumber, header->size);
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "CsrMemFree: double free on pointer");
            CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_FREE);
            return;
        }

        /* Check header and tail for corruption */
        if (header->magicNumber != CSR_FW_EXT_MAGIC_NUMBER_HEAD)
        {
            CsrDebugTraceLow("FW_EXT: CsrMemFree: Invalid magic number in header (pointer: %p, magic_h: 0x%0x, size: %d)",	
                pointer, header->magicNumber, header->size);
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "CsrMemFree: Invalid magic number in header");
            CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_FREE);
            return;
        }
        
        else if (
            //CSR_COPY_UINT16_TO_LITTLE_ENDIAN((CsrUint16 *)((CsrInt8 *)pointer + header->size)) != CSR_FW_EXT_MAGIC_NUMBER_TAIL)
            *(CsrUint8 *)((CsrUint8 *)pointer + header->size) != (CsrUint8)(CSR_FW_EXT_MAGIC_NUMBER_TAIL & 0x000000FF) || 
            *(CsrUint8 *)((CsrUint8 *)pointer + header->size +1) != (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 8) & 0x000000FF) || 
            *(CsrUint8 *)((CsrUint8 *)pointer + header->size + 2) != (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 16) & 0x000000FF) || 
            *(CsrUint8 *)((CsrUint8 *)pointer + header->size + 3) != (CsrUint8)((CSR_FW_EXT_MAGIC_NUMBER_TAIL >> 24) & 0x000000FF))
        {
            CsrDebugTraceLow("FW_EXT: CsrMemFree: Invalid magic number in tail (pointer: %p, magic_t: 0x%0x, size: %d)",
                              pointer, *(CsrUint32 *)((CsrInt8 *)pointer + sizeof(csrFwExtAllocMagicHeader) + header->size), header->size);
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "CsrMemFree: Invalid magic number in tail");
            /* Do not return here - header is OK so freeing should succeed */
        }

#ifdef CSR_MEASURE_PEAK_MEM_USAGE
        csr_dynamic_ram_usage_chunks--;
        csr_dynamic_ram_usage_bytes -= header->size;
#endif        
       
        /* Set memory allocation size to 0xFFFFFFFF to catch future double-frees */
        header->size = 0xFFFFFFFF;

        result = SCI_FREE(header);
#else
        result = SCI_FREE(pointer);
#endif
        if (result != SCI_SUCCESS)
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "CsrMemFree: Free failed");
        }
    }
    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_FREE);
} /*lint !e438*/


#ifdef CSR_MEASURE_PEAK_MEM_USAGE
void csrDebugDumpRamUsage (void)
{
    extern void csr_trace_simple(int id, const char * const format,...);
    CsrDebugTraceLow ("csr_dynamic_ram_usage_chunks = %d\n", csr_dynamic_ram_usage_chunks);
    CsrDebugTraceLow ("csr_dynamic_ram_usage_bytes = %d\n", csr_dynamic_ram_usage_bytes);
    CsrDebugTraceLow ("csr_dynamic_ram_usage_peak_bytes = %d\n", csr_dynamic_ram_usage_peak_bytes);
}
#endif        

#ifdef CSR_GPIO_DEBUG
static void gpioDebugInit(void);
#endif

#ifndef CSR_MEM_ALLOC_INITIAL_DMA
void CsrMemAllocDmaInit(void)
{
    CsrDebugTraceLow ("FW_EXT: initial DMA pool memory is NULL");
}
#else
void CsrMemAllocDmaInit(void)
{
    CsrUint32 i;
    CsrMemoryDmaPoolType *CsrMemoryDmaPool[CSR_MEM_DMA_BLOCK_COUNT];

    CsrMemoryDmaTmp = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrMemoryDmaPoolType) * CSR_MEM_DMA_BLOCK_COUNT + CSR_MEM_DMA_ALIGN_SIZE);

    CsrMemoryDmaPool[0] = (CsrMemoryDmaPoolType *)(((CsrUint32)CsrMemoryDmaTmp + 0xfff)&(~0xfff));

    CsrMemoryDmaPoolStart = CsrMemoryDmaPool[0];

    for (i = 0; i < (CSR_MEM_DMA_BLOCK_COUNT - 1); i++)    
    {   
        CsrMemSet(CsrMemoryDmaPool[i]->data, 0, sizeof(CsrMemoryDmaPool[i]->data));
        CsrMemoryDmaPool[i]->inUse = 0;
        CsrMemoryDmaPool[i+1] = CsrMemoryDmaPool[i] + 1; 
        CsrMemoryDmaPool[i]->next = CsrMemoryDmaPool[i+1];
    }

#if 0
    if (((CsrUint32)CsrMemoryDmaPool) & 0x00000003)    
    {
        CsrDebugTraceLow("FW_EXT: CsrMemoryDmaPool is not aligned %p", CsrMemoryDmaPool);
        //CSR_LOG_TEXT_CRITICAL((CSR_SCHED_QID_INVALID, 0, "FW_EXT: CsrMemoryDmaPool is not aligned %p", CsrMemoryDmaPool));    
    }
#endif
    if (sizeof(CsrMemoryDmaPoolType) != 4096)    /*lint !e506 !e774*/
    {
        CsrDebugTraceLow ("FW_EXT: CsrMemoryDmaPoolType has wrong size %u", sizeof(CsrMemoryDmaPoolType));
        //CSR_LOG_TEXT_CRITICAL((CSR_SCHED_QID_INVALID, 0, "FW_EXT: CsrMemoryDmaPoolType has wrong size %u", sizeof(CsrMemoryDmaPoolType)));    
    }
#ifdef CSR_GPIO_DEBUG
    gpioDebugInit();
#endif
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemAllocDma
 *
 *  DESCRIPTION
 *      Allocate dynamic memory suitable for DMA transfers.
 *
 *  RETURNS
 *      Pointer to allocated memory, or NULL in case of failure.
 *      Allocated memory is not initialised.
 *
 *----------------------------------------------------------------------------*/
void *CsrMemAllocDma(size_t size, CsrBool direction)
{
    CsrMemoryDmaPoolType *pPoolBlock;
    CsrUint8 *blockOffset;

#ifndef CSR_MEM_ALLOC_INITIAL_DMA
    CsrMemoryDmaPoolType *CsrMemoryDmaPool;
#endif
    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MEM_ALLOC_DMA);

    SCI_DisableIRQ();
    SCI_DisableFIQ();

    if (size > CSR_MEM_DMA_BLOCK_SIZE)
    {
        CsrDebugTraceLow("FW_EXT: CsrMemAllocDma size %d is too big for DMA alloc, max is %d", size, CSR_MEM_DMA_BLOCK_SIZE);
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "FW_EXT: CsrMemAllocDma size is too big for DMA alloc");
    }
#ifndef CSR_MEM_ALLOC_INITIAL_DMA
    CsrMemoryDmaTmp = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrMemoryDmaPoolType) + CSR_MEM_DMA_ALIGN_SIZE);
    CsrMemoryDmaPool = (CsrMemoryDmaPoolType *)(((CsrUint32)CsrMemoryDmaTmp + 0xfff)&(~0xfff));
    pPoolBlock = CsrMemoryDmaPool;
#else
    pPoolBlock = CsrMemoryDmaPoolStart;
#endif

    if(!pPoolBlock)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_HEAP_EXHAUSTION, "FW_EXT: CsrMemAllocDma NO memory");
    }
#ifndef CSR_MEM_ALLOC_INITIAL_DMA
    blockOffset = (direction) ? ((CsrUint8*)pPoolBlock) + CSR_MEM_DMA_WRITE_OFFSET : ((CsrUint8*)pPoolBlock) + CSR_MEM_DMA_READ_OFFSET; /*lint !e613*/
    pPoolBlock->inUse = (CsrUint32)(CsrMemoryDmaTmp); /*lint !e613*/
#else
    if(pPoolBlock->inUse) /*lint !e613*/
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "FW_EXT: CsrMemAllocDma Returned block already in use");
    }
    CsrMemoryDmaPoolStart = CsrMemoryDmaPoolStart->next;
    //blockOffset = (direction) ? ((CsrUint8*)pPoolBlock) + CSR_MEM_DMA_WRITE_OFFSET : ((CsrUint8*)pPoolBlock) + 0;
    blockOffset = (direction) ? ((CsrUint8*)pPoolBlock) + CSR_MEM_DMA_WRITE_OFFSET : ((CsrUint8*)pPoolBlock) + CSR_MEM_DMA_READ_OFFSET; /*lint !e613*/
    pPoolBlock->inUse = 1; /*lint !e613*/
#endif
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_ALLOC_DMA);

    return blockOffset;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemFreeDma
 *
 *  DESCRIPTION
 *      Free dynamic memory allocated by CsrMemAllocDma.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMemFreeDma(void *pointer)
{
    CsrMemoryDmaPoolType *ptr;      

    CSR_GPIO_DEBUG_SET(0, CSR_GPIO_MEM_FREE_DMA);

    SCI_DisableIRQ();
    SCI_DisableFIQ();

    ptr = (CsrMemoryDmaPoolType *)(((CsrUint32)pointer) & 0xFFFFF000u);
    if(!ptr->inUse)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER, "FW_EXT: CsrMemFreeDma called on pointer not in use");
    }
#ifndef CSR_MEM_ALLOC_INITIAL_DMA
    CsrMemFree((void *)ptr->inUse);
#else
    ptr->inUse = 0;
    ptr->next = CsrMemoryDmaPoolStart;
    CsrMemoryDmaPoolStart = ptr;
#endif
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    CSR_GPIO_DEBUG_SET(1, CSR_GPIO_MEM_FREE_DMA);
}

void CsrWifiCmdCallbackReg(void)
{
#ifdef CSR_THROUGHPUT_TEST_TOOL
    // Register cmd handler
    WIFI_CmdCallbackReg(wlan_csr_cmd);
#endif
}

#ifdef CSR_GPIO_DEBUG
unsigned int CsrGpioMap[CSR_GPIO_NUM_EVENTS];
static CsrGpioControl CsrGpioControlMap[CSR_GPIO_HIGHEST_NUM + 1];

static const char *CsrGpioEventNames[CSR_GPIO_NUM_EVENTS] =
{
    "unifi_receive_event_batch"
    ,"unifi_ta_sample call outside"
    ,"etherRxFunction call"
    ,"unifi_pause_xmit"
    ,"unifi_restart_xmit"
    ,"BH event loop run"
    ,"unifi_bh"
    ,"Get dropped packets count"
    ,"read_to_host_signals"
    ,"process_to_host_signals"
    ,"process_fh_cmd_queue"
    ,"process_fh_traffic_queue"
    ,"flush_fh_buffer"
    ,"CsrWifiHipControlledPortFindBlockedPorts"
    ,"txApplyTimeout"
    ,"txBatchTimeout"
    ,"txBatchTimeoutHandler"
    ,"CsrWifiRouterMaUnitdataFuncTx"
    ,"CsrWifiHipTxMaUnitdata"
    ,"PTHS parse signals"
    ,"PTHS sanity checks"
    ,"PTHS prepare TH signals"
    ,"PTHS prepare bulk data commands"
    ,"PTHS clear slots"
    ,"PTHS run I/O operations"
    ,"PTHS run I/O operations single (A05 compatibility mode)"
    ,"PTHS run I/O operations chained (B07 fast mode)"
    ,"PTHS receive signals"
    ,"PTHS free cleared slots"
    ,"CsrEventCreate"
    ,"CsrEventWait"
    ,"CsrEventSet"
    ,"CsrEventDestroy"
    ,"CsrMutexCreate"
    ,"CsrGlobalMutexLock"
    ,"CsrMutexUnlock"
    ,"CsrMutexDestroy"
    ,"CsrGlobalMutexCreate"
    ,"CsrGlobalMutexLock"
    ,"CsrGlobalMutexUnlock"
    ,"CsrGlobalMutexDestroy"
    ,"CsrThreadCreate"
    ,"CsrThreadGetHandle"
    ,"CsrThreadEqual"
    ,"CsrThreadSleep"
    ,"CsrMemAlloc"
    ,"CsrMemCalloc"
    ,"CsrMemFree"
    ,"CsrMemAllocDma"
    ,"CsrMemFreeDma"
    ,"SPI init GPIO toggling"
    ,"SPI IRQ handler"
    ,"CsrSpiMasterInitialise"
    ,"CsrSpiMasterDeinitialise"
    ,"SPI opCallback"
    ,"CsrSpiMasterRead"
    ,"CsrSpiMasterReadWrite"
    ,"CsrSpiMasterWrite"
    ,"CsrSpiMasterChipSelectAssert"
    ,"CsrSpiMasterChipSelectNegate"
    ,"CsrSpiMasterIo"
    ,"CsrSpiMasterBusClockFrequencySet"
    ,"CsrSpiMasterCallbackRegister"
    ,"CsrSpiMasterInterruptAcknowledge"
    ,"CsrSdio HISR"
    ,"CsrSdio LISR"
    ,"CsrSdioFunctionDriverRegister"
    ,"CsrSdioFunctionDriverUnregister"
    ,"CsrSdioInsertedAcknowledge"
    ,"CsrSdioRemovedAcknowledge"
    ,"CsrSdioRead8"
    ,"CsrSdioF0Read8"
    ,"CsrSdioWrite8"
    ,"CsrSdioF0Write8"
    ,"CsrSdioRead16"
    ,"CsrSdioRead16L"
    ,"CsrSdioWrite16"
    ,"CsrSdioWrite16L"
    ,"CsrSdioRead"
    ,"CsrSdioWrite"
    ,"CsrSdioFunctionEnable"
    ,"CsrSdioFunctionDisable"
    ,"CsrSdioInterruptEnable"
    ,"CsrSdioInterruptDisable"
    ,"CsrSdioInterruptAcknowledge"
    ,"frameRxFunction"
    ,"TCPIP_RxInd call"
    ,"ioProcessBulkRead"
    ,"ioProcessBulkWrite"
    ,"ioProcessRegRead"
    ,"ioProcessRegWrite"
    ,"iobulkReadComplete"
    ,"iobulkWriteComplete"
};

static void initGpio(unsigned int gpio_id, CsrGpioControl *map)
{
    GPIO_INFO_T gpio_info;
    unsigned int offset;

    GPIO_PHY_GetBaseInfo (gpio_id, &gpio_info);

    map->addr = gpio_info.baseAddr + GPIO_DATA;
    map->bitnum = gpio_info.bit_num;
}

static void gpioEnable(CsrGpioName index, unsigned int gpio_num)
{
    if ((gpio_num <= CSR_GPIO_HIGHEST_NUM) && (index < CSR_GPIO_NUM_EVENTS))
    {
        CsrGpioMap[index] = gpio_num;
    }
}

static void gpioDebugInit(void)
{
    unsigned int addr;
    unsigned int bitnum;

    initGpio(96,&CsrGpioControlMap[1]);
    initGpio(59,&CsrGpioControlMap[2]);
    initGpio(60,&CsrGpioControlMap[3]);
    initGpio(40,&CsrGpioControlMap[4]);
    initGpio(101,&CsrGpioControlMap[5]);
    CsrMemSet(CsrGpioMap, 0, CSR_GPIO_NUM_EVENTS * sizeof(unsigned int));

    addr = CsrGpioControlMap[1].addr;
    bitnum = CsrGpioControlMap[1].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (1<<bitnum)));
    SCI_RestoreIRQ();

    addr = CsrGpioControlMap[2].addr;
    bitnum = CsrGpioControlMap[2].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (1<<bitnum)));
    SCI_RestoreIRQ();

    addr = CsrGpioControlMap[3].addr;
    bitnum = CsrGpioControlMap[3].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (1<<bitnum)));
    SCI_RestoreIRQ();

    addr = CsrGpioControlMap[4].addr;
    bitnum = CsrGpioControlMap[4].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (1<<bitnum)));
    SCI_RestoreIRQ();

    addr = CsrGpioControlMap[5].addr;
    bitnum = CsrGpioControlMap[5].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (1<<bitnum)));
    SCI_RestoreIRQ();
}

void CsrGpioDebugSet(unsigned int value, CsrGpioName index)
{
    unsigned int gpio_num = CsrGpioMap[index];
    unsigned int addr = CsrGpioControlMap[gpio_num].addr;
    unsigned int bitnum = CsrGpioControlMap[gpio_num].bitnum;
    SCI_DisableIRQ();
    CHIP_REG_SET (addr, ((REG32(addr) & ~(1UL<<bitnum)) | (value<<bitnum)));
    SCI_RestoreIRQ();
}
#endif

#ifdef CSR_THROUGHPUT_TEST_TOOL
/*lint -e611*/
   
typedef int (cmd_handler_t) (void *cookie, int argc, char *argv[]);
typedef struct
{
    char *name;
    cmd_handler_t *handler;
    char *help;
} cmd_t;

cmd_t *cmd_find(char *name, cmd_t *cmd_list)
{
    cmd_t *cmd;

    for (cmd = cmd_list; cmd->handler != NULL; cmd++)
    {
        if (CsrStrCmp(cmd->name, name) == 0)
            return cmd;
    }
    return NULL;
}

/*
 * commands implementation
 */
static int cmd_is_on(void *cookie, int argc, char *argv[])
{
    if (wifi_supp_is_on)
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("ON");
    else
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("OFF");
    return 0;
}

static int cmd_test(void *cookie, int argc, char *argv[])
{
    int i;
    
    for (i=0; i<10; i++) {
        static char buf[1024];
        char *p = buf;
        memset (p, 'A', i);
        p[i] = '\0';
        ((WIFI_CMD_PRINTF_FUNC)(cookie))(p);
    }
    return 0;
}

static int cmd_rxtcp_run(void *cookie)
{
    int got_len;
    int SocketFD;
    int ConnectFD;
	int addr_len;
    struct sci_sockaddr rxSockAddr;
    struct sci_sockaddr stSockAddr;
    TB_RTC_TIME_T now;

    ((WIFI_CMD_PRINTF_FUNC)(cookie))("cmd_rxtcp: begin");
    buff = malloc(CSR_BUFF_SIZE);
    if(!buff)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: can't allocate memory to buffer");
        return -1;
    }
    if (!wifi_supp_is_on)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: wifi is off");
        free(buff);
        return -1;
    }
    memset(&rxSockAddr, 0, sizeof(rxSockAddr));
    memset(&stSockAddr, 0, sizeof(stSockAddr));	
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == SocketFD)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: can't create socket");
        free(buff);
        return -1;
    } 
    stSockAddr.family = AF_INET;
    stSockAddr.port = htons(CSR_TCP_SPEED_SOCK);
    inet_aton("0.0.0.0", &stSockAddr.ip_addr);
    if(-1 == bind(SocketFD,(struct sci_sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: bind failed");
        close(SocketFD);
        free(buff);
        return -1;
    }
    if(-1 == listen(SocketFD, 1))
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: listen failed");
        close(SocketFD);
        free(buff);
        return -1;
    }
    SCI_SetArmLogFlag(FALSE);
    TCPIPDEBUG_EnableCap(FALSE);
	addr_len = sizeof(rxSockAddr);
    ConnectFD = accept(SocketFD, (struct sci_sockaddr *)&rxSockAddr, &addr_len);
    if(0 > ConnectFD)
    {
        SCI_SetArmLogFlag(TRUE);
        TCPIPDEBUG_EnableCap(TRUE);
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: accept failed");
        close(SocketFD);
        free(buff);
        return -1;
    }
    RTC_Get(&now);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("connection from %s at %d:%d:%d:%d",
	inet_ntoa(rxSockAddr.ip_addr), now.day, now.hour, now.min, now.sec);
    for(;;)
    {
        got_len = recv(ConnectFD, buff, CSR_BUFF_SIZE, 0);
        if(-1 == got_len)
        {
            SCI_SetArmLogFlag(TRUE);
            TCPIPDEBUG_EnableCap(TRUE);
            ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: recv failed");
            close(ConnectFD);
            close(SocketFD);
            free(buff);
            return -1;
        }
        if(0 == got_len)
        {
            RTC_Get(&now);
            ((WIFI_CMD_PRINTF_FUNC)(cookie))("disconnection from %s at %d:%d:%d:%d",
			inet_ntoa(rxSockAddr.ip_addr), now.day, now.hour, now.min, now.sec);
            close(ConnectFD);
            break;
        }
    }
    SCI_SetArmLogFlag(TRUE);
    TCPIPDEBUG_EnableCap(TRUE);
    close(SocketFD);
    free(buff);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("cmd_rxtcp: end");
    return 0;
}

typedef struct
{
    void *cookie;
} rx_tcp_thread_params;

static void cmd_rxtcp_thread(void *pointer)
{
    rx_tcp_thread_params *params = pointer;
    cmd_rxtcp_run(params->cookie);
    free(pointer);
    tcp_flg = FALSE;
}

static int cmd_rxtcp(void *cookie, int argc, char *argv[])
{
    CsrThreadHandle handle;
    rx_tcp_thread_params *params;

    if (tcp_flg)
        return -1;
    tcp_flg = TRUE;
    
    params = malloc(sizeof(rx_tcp_thread_params));
    if (params == NULL)
        return -1;

    params->cookie = cookie;

#ifdef PLATFORM_SC6530
    CsrThreadCreateExt(cmd_rxtcp_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_RXTCP",
        &handle);
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CsrThreadCreateExt(cmd_rxtcp_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_RXTCP",
        &handle);
#else
    CsrThreadCreate(cmd_rxtcp_thread, params, CSR_THREAD_DEFAULT_STACK_SIZE, CSR_THREAD_PRIORITY_HIGH, "cmd_rxtcp", &handle);
#endif
    return 0; /*lint !e429*/
}

static int cmd_txtcp_run(void *cookie, int argc, char *argv[])
{
    int SocketFD;
    int amount_sent;
    int i;
    uint32 msec_start;
    uint32 time_to_send;
    uint32 time_to_tx = 0;
    long blocks_to_send = CSR_BLOCKS_TO_SEND;
    long block_size = CSR_BLOCK_SIZE;
    unsigned long dot_addr;
    double b_sec;
    int sndbuf_size;
    struct sci_sockaddr stSockAddr;

    ((WIFI_CMD_PRINTF_FUNC)(cookie))("cmd_txtcp: begin");
    if (argc < 1 || argc > 4)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: too few/many pars");
        return -1;
    } 
    for(i = argc-1; i > 0; i--)
    {
        if(CsrStrNICmp(argv[i], "tt-", 3) == 0)
        {
            if(CsrStrLen(argv[i]) > 3)
            {
                time_to_tx = (uint32)(atoi(&argv[i][3]) * 1000);
            }
            else
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: time to transmit is unknown");
                return -1;
            }
            if(time_to_tx == 0)
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: time to transmit is 0");
                return -1;
            }
            continue;
        }
        if(CsrStrNICmp(argv[i], "nb-", 3) == 0)
        {
            if(CsrStrLen(argv[i]) > 3)
            {
                blocks_to_send = atoi(&argv[i][3]);
            }
            else
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: blocks to send is unknown");
                return -1;
            }
            if(blocks_to_send == 0)
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: blocks to send is 0");
                return -1;
            }
            continue;
        }
        if(CsrStrNICmp(argv[i], "bs-", 3) == 0)
        {
            if(CsrStrLen(argv[i]) > 3)
            {
                block_size = atoi(&argv[i][3]);
            }
            else
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: block size is unknown");
                return -1;
            }
            if(block_size == 0)
            {
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: block size is 0");
                return -1;
            }
            continue;
        }
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: Invalid pars");
        return -1;
    }
    dot_addr = inet_addr(argv[0]);
    if(dot_addr == NULL)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: invalid TCP server IP addr");
		return -1;
    }
    buff = malloc(CSR_BUFF_SIZE);
    if(!buff)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: can't allocate memory to buffer");
        return -1;
    }
    if (!wifi_supp_is_on)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: wifi is off");
		free(buff);
        return -1;
    }
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == SocketFD)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: can't create socket");
		free(buff);
        return -1;
    }
    stSockAddr.family = AF_INET;
    stSockAddr.port = htons(CSR_TCP_SPEED_SOCK);
    stSockAddr.ip_addr = dot_addr;
    if (-1 == connect(SocketFD, (struct sci_sockaddr *)&stSockAddr, sizeof(stSockAddr)))
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: connect failed");
        free(buff);
        close(SocketFD);
        return -1;
    }
    sndbuf_size = 65536;
    setsockopt(SocketFD, SO_SNDBUF,&sndbuf_size);
    sndbuf_size = 0;
    getsockopt(SocketFD, SO_SNDBUF,&sndbuf_size);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("SO_SNDBUF value is %d",sndbuf_size);
    SCI_SetArmLogFlag(FALSE);
    TCPIPDEBUG_EnableCap(FALSE);
    msec_start = SCI_GetTickCount();
    if (time_to_tx > 0)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("time to tx is: %ldms", (long int)time_to_tx);
        do
        {
            amount_sent = send(SocketFD, buff, block_size, 0);
            if(amount_sent != block_size)
            {
                SCI_SetArmLogFlag(TRUE);
                TCPIPDEBUG_EnableCap(TRUE);
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: send failed");
                free(buff);
                close(SocketFD);
                return -1;  
            }
            time_to_send = SCI_GetTickCount() - msec_start;
        } while (time_to_send < time_to_tx);
    }
    else
    {
        for(i = 0; i < blocks_to_send; i++)
        {
            amount_sent = send(SocketFD, buff, block_size, 0);
            if(amount_sent != block_size)
            {
                SCI_SetArmLogFlag(TRUE);
                TCPIPDEBUG_EnableCap(TRUE);
                ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: send failed");
                free(buff);
                close(SocketFD);
                return -1;  
            }
        }
        time_to_send = SCI_GetTickCount() - msec_start;
    }
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("time to transfer is: %ldms", (long int)time_to_send);
    b_sec = (double)blocks_to_send * (double)block_size;
    b_sec /= (double)time_to_send;
    b_sec *= (double)1000;
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("KByte sec: %.0lf", b_sec/(double)1000);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("MByte sec: %.1lf", b_sec /(double)1000000);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("MBit sec: %.3lf", b_sec/(1000000/8));
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("KBit sec: %.0lf", b_sec/(1000/8));
    SCI_SetArmLogFlag(TRUE);
    TCPIPDEBUG_EnableCap(TRUE);
    free(buff);
    close(SocketFD);
    ((WIFI_CMD_PRINTF_FUNC)(cookie))("cmd_txtcp: end");
    return 0;
}




//#define CSR_SUPPORT_SPD_ATF 

#ifdef CSR_SUPPORT_SPD_ATF
#include "mmiwlan_profile.h"
#include "mmiwlan_id.h"
#include "guilistbox.h"

struct atf_ctx_s {
    int i;
    int idx;
    int is_connected;
    char *p;
    int found;
};
struct atf_ctx_s atf_ctx;

static BOOLEAN wifi_scan_list_cb(MMIWIFI_PROFILE_T *pstProfile, void *find_data)
{
	uint8 pw_ssid[WIFISUPP_SSID_MAX_LEN * 2 + 4] = {0};
    if (pstProfile == NULL)
        return;
    memcpy (pw_ssid, pstProfile->config_info.ssid.ssid, pstProfile->config_info.ssid.ssid_len);
    if (CsrStrCmp(atf_ctx.p, (char *)&pw_ssid) == 0) {
        atf_ctx.idx = atf_ctx.i;
        atf_ctx.found = 1;
        atf_ctx.is_connected = 
            (MMIWIFI_PROFILE_STATE_CONNECTED == pstProfile->state) ||
            (MMIWIFI_PROFILE_STATE_DHCPED == pstProfile->state);
    }
    atf_ctx.i++;    
    return FALSE;
}
    

static int cmd_atf(void *cookie, int argc, char *argv[])
{
    WIFI_CMD_PRINTF_FUNC cli_printf = (WIFI_CMD_PRINTF_FUNC)cookie;
    memset (&atf_ctx, 0, sizeof(atf_ctx));

    cli_printf("<ATF<");    
    if (argc < 1)
        goto done;
    
	if ((CsrStrCmp(argv[0], "is_on") == 0) && (argc >= 1)) {
        if (wifi_supp_is_on)
            cli_printf ("ON");
        else
            cli_printf ("OFF");
    }
    else if ((CsrStrCmp(argv[0], "scan_idx_by_ssid") == 0) && (argc >= 2)) {
        atf_ctx.p = argv[1];
        MMIWIFI_TraverseProfile(wifi_scan_list_cb, NULL);   
        if (atf_ctx.found)
            cli_printf("FOUND SSID=%s IDX=%d CONN_STATE=%d", atf_ctx.p, atf_ctx.idx, atf_ctx.is_connected);
        else
            cli_printf("NOTFND");
    }
    else if ((CsrStrCmp(argv[0], "trusted_ap_num") == 0) && (argc >= 1)) {
        cli_printf("NUM=%d", GUILIST_GetTotalItemNum(MMIWIFI_LIST_TRUSTED_CTRL_ID));
    }

done:
    cli_printf(">ATF>");
    return 0;
}
#endif /* CSR_SUPPORT_SPD_ATF */



typedef struct
{
    void *cookie;
    int argc;
    char **argv;
} tx_tcp_thread_params;

static void cmd_txtcp_thread(void *pointer)
{
    tx_tcp_thread_params *params = pointer;
    int i;
    cmd_txtcp_run(params->cookie, params->argc, params->argv);
    for (i = 0; i < params->argc; i++)
    {
        free(params->argv[i]);
    }
    free(params->argv);
    free(pointer);
    tcp_flg = FALSE;
}

static int cmd_txtcp(void *cookie, int argc, char *argv[])
{
    CsrThreadHandle handle;
    tx_tcp_thread_params *params;
    int i;

    if (tcp_flg)
        return -1;
    tcp_flg = TRUE;
    
    params = malloc(sizeof(tx_tcp_thread_params));
    if (params == NULL)
        return -1;
    params->cookie = cookie;
    params->argc = argc;

    params->argv = malloc(argc * sizeof(char *)); /*lint !e737*/
    if (params->argv == NULL)
        return -1; /*lint !e429*/
    for (i = 0; i < argc; i++)
    {
        params->argv[i] = malloc(CsrStrLen(argv[i]) + 1);
        if (params->argv[i] == NULL)
            return -1; /*lint !e429*/
        CsrStrNCpyZero(params->argv[i], argv[i], CsrStrLen(argv[i]) + 1);
    }

#ifdef PLATFORM_SC6530
    CsrThreadCreateExt(cmd_txtcp_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_TXTCP",
        &handle);
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CsrThreadCreateExt(cmd_txtcp_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_TXTCP",
        &handle);
#else
    CsrThreadCreate(cmd_txtcp_thread, params, CSR_THREAD_DEFAULT_STACK_SIZE, CSR_THREAD_PRIORITY_HIGH, "cmd_txtcp", &handle);
#endif
    return 0; /*lint !e429*/
}


#ifdef CSR_LOG_ENABLE
#include "csr_spt_at.h"

typedef struct
{
    void *cookie;
    CsrCharString *args;
} spt_thread_params;

static void cmd_spt_thread(void *pointer)
{
    spt_thread_params *params = pointer;
    CsrCharString *resultString = NULL;
    CsrCharString *stringBuffer;
    CsrUint32 resultLength;
    CsrUint32 stringIndex;
#define SPT_MAX_LINE_LENGTH 100
    
    ((WIFI_CMD_PRINTF_FUNC)(params->cookie))("cmd_spt: %s",params->args);
    CsrSptAtCommandHandler(params->args, &resultString);
    resultLength = CsrStrLen(resultString);
    stringBuffer = malloc(SPT_MAX_LINE_LENGTH + 1);
    if (resultString)
    {
        for (stringIndex = 0; stringIndex < resultLength; stringIndex += SPT_MAX_LINE_LENGTH)
        {
            CsrStrNCpyZero(stringBuffer, &resultString[stringIndex], SPT_MAX_LINE_LENGTH + 1);
            ((WIFI_CMD_PRINTF_FUNC)(params->cookie))("cmd_spt: %s",stringBuffer);
        }
        CsrSptAtCommandHandlerResultFree(resultString);
    }    
	free(params->args);
	free(stringBuffer);
	free(pointer);
	((WIFI_CMD_PRINTF_FUNC)(params->cookie))("cmd_spt: end");
}

static int cmd_spt(void *cookie, int argc, char *argv[])
{
    spt_thread_params *params;
    CsrThreadHandle handle;
    int i;
    CsrSize count = 0;
#define SPT_MAX_ARG_LEN 300

    ((WIFI_CMD_PRINTF_FUNC)(cookie))("cmd_spt: begin");
    if (argc < 1)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: wrong number of parameters %d", argc);
        return -1;
    } 
    params = malloc(sizeof(spt_thread_params));
    if (params == NULL)
        return -1;
    params->cookie = cookie;
    params->args = malloc(SPT_MAX_ARG_LEN+1);
    CsrStrNCpyZero(params->args, argv[0], SPT_MAX_ARG_LEN+1);
    count = CsrStrLen(params->args);
    
    for (i = 1; (i < argc) && (count < SPT_MAX_ARG_LEN); i++)
    {
        CsrStrNCat(params->args, ",", SPT_MAX_ARG_LEN+1);
        CsrStrNCat(params->args, argv[i], SPT_MAX_ARG_LEN+1);
        params->args[SPT_MAX_ARG_LEN] = '\0'; // CsrStrNCat does not guarantee terminating zero
        count = CsrStrLen(params->args);
    }
    
#ifdef PLATFORM_SC6530
    CsrThreadCreateExt(cmd_spt_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_SPT",
        &handle);
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CsrThreadCreateExt(cmd_spt_thread,
        params,
        0, // Set in CsrThreadCreateExt()
        0, // Set in CsrThreadCreateExt()
        "CSR_WIFI_SPT",
        &handle);
#else
    CsrThreadCreate(cmd_spt_thread, params, CSR_THREAD_DEFAULT_STACK_SIZE, CSR_THREAD_PRIORITY_NORMAL, "cmd_spt", &handle);
#endif
    return 0; /*lint !e429*/
}
#endif


#ifdef CSR_GPIO_DEBUG
static int cmd_gpio_enable(void *cookie, int argc, char *argv[])
{
    unsigned int gpio_num;
    unsigned int index;
    CsrGpioName i;
    if (argc != 2)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("error: wrong number of parameters %d", argc);
        return -1;
    } 
    gpio_num = atoi(argv[0]);
    index = atoi(argv[1]);
    gpioEnable(index, gpio_num);
    for (i = 0; i < CSR_GPIO_NUM_EVENTS; i++)
    {
        if (CsrGpioMap[i])
        {
            ((WIFI_CMD_PRINTF_FUNC)(cookie))("%s is mapped to GPIO %u", CsrGpioEventNames[i], CsrGpioMap[i]);
        }
    }
    return 0;
}

static int cmd_gpio_print(void *cookie, int argc, char *argv[])
{
    CsrGpioName i;
    for (i = 0; i < CSR_GPIO_NUM_EVENTS; i++)
    {
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("%u: %s", i, CsrGpioEventNames[i]);
    }
    return 0;
}
#endif

static int cmd_help(void *cookie, int argc, char *argv[]);
static cmd_t csr_cmd_tbl[] = 
{
    {"help", cmd_help, " "},
    {"txtcp", cmd_txtcp, "TCP TX (TCP client). \
       Usage: txtcp  <TCP server IP addr> [tt-<time to transmit in seconds> OR nb-<number of blocks>] [bs-<block size>]. \
       E.g., txtcp 192.168.1.100 tt-10 bs-65536 OR txtcp 192.168.1.100 nb-100 bs-65536"},
    {"rxtcp", cmd_rxtcp, "TCP RX (TCP server). \
       Usage:rxtcp. \
       E.g., rxtcp"},
#ifdef CSR_LOG_ENABLE 
    {"spt", cmd_spt, "CSR SPT AT handler. \
    Usage: spt <param1> <param2>... \
    E.g. spt core test benchmark \
    Note that comma is replaced with space compared to standard SPT syntax \
    Use \"spt help\" for more information"},
#endif
#ifdef CSR_GPIO_DEBUG
    {"gpio_enable",cmd_gpio_enable,"GPIO debug configuration. \
    Usage: gpio_enable <pin index> <debug point index> \
    debug point index can be found by using gpio_print \
    E.g. gpio_enable 1 22.\
    To disable a debug point, just map it to pin index 0"},
    {"gpio_print",cmd_gpio_print,"GPIO debug point list \
    List GPIO debug points and their associated index"},
#endif
    {"test", cmd_test, " "},
#ifdef CSR_SUPPORT_SPD_ATF    
    {"atf", cmd_atf, " "},
#endif    
    {"is_on", cmd_is_on, "check if WiFi is on or off "},    
    {"", NULL, ""}, /* should be last */
};

static int cmd_help(void *cookie, int argc, char *argv[])
{
    cmd_t *cmd = csr_cmd_tbl;

    ((WIFI_CMD_PRINTF_FUNC)(cookie))("csr unifi command list");
    for (; cmd->handler != NULL; cmd++)
        ((WIFI_CMD_PRINTF_FUNC)(cookie))("\t%s\t\t%s", cmd->name, cmd->help);
    return 0;
}

/* 
 * main entry 
 */
static void wlan_csr_cmd(WIFI_CMD_PRINTF_FUNC oprintf, int argc, char *argv[])
{
    cmd_t *cmd;

    if (argc == 0) {
        cmd_help ((void *)oprintf, 0, NULL);
        return;
    }

    cmd = cmd_find(argv[0], csr_cmd_tbl);
    if (cmd == NULL) {
        oprintf("Cannot find the cmd %s\n", argv[0]);
        return;
    }
    cmd->handler((void *)oprintf, argc-1, &argv[1]);
}
#endif


void wifi_cmd_printf_func_dummy(const char *x_format, ...)
{
}

/*stub for iperf*/
void iperf_test_start(char* param)
{
    int i, j, argc = 0;
    char *s, *c, *t, *p, tmp[255], ad[20] = "", tt[10] = "tt-", *argv[3] = {"192.168.1.100", "tt-10", "bs-65536"};
    
    /* iperf: -s -p 6666
                 -c 192.168.xx1.100 -t 10 -p 6666 */
    // check pars length
    i = CsrStrLen(param);
    if ((i < 8) || (i > 255))
        return;
    // remove spaces
    s = param;
    c = tmp;
    i = 0;
    while (*s != '\0')
    {
        if (*s != ' ')
        {
            *c++ = *s;
            i++;
        }
        s++;
    }
    *c = '\0';
    c -= i;
    param = c;
    // parse pars
    s = CsrStrStr(param, "-s");
    c = CsrStrStr(param, "-c");
    t = CsrStrStr(param, "-t");
    p = CsrStrStr(param, "-p6666");
    if ((s != NULL) && (c == NULL))
    {
        // -s -> rx
        if (p != NULL)
        {
            // -p 6666 -> valid port
#ifdef CSR_THROUGHPUT_TEST_TOOL
            cmd_rxtcp((void *)wifi_cmd_printf_func_dummy, argc, argv);
#endif
        }
    }
    else if ((s == NULL) && (c != NULL))
    {
        // -c -> tx
        if (p != NULL)
        {
            // -p 6666 -> valid port
            c += 2;
            i = strlen(c) - strlen(p);
            if (t != NULL)
            {
                // -t -> time to tx
                t += 2;
                j = strlen(t) - strlen(p);
                if (j > 0)
                    i = strlen(c) - strlen(t) - 2;
                else
                    j = strlen(t);
                argv[1] = tt;
                strncpy(&argv[1][3], t, j);
                argv[1][j+3] = '\0';
            }
            argv[0] = ad;
            strncpy(argv[0], c, i);
            argv[0][i] = '\0';
            argc += 3;
#ifdef CSR_THROUGHPUT_TEST_TOOL
            cmd_txtcp((void *)wifi_cmd_printf_func_dummy, argc, argv);
#endif
        }
    }
}

void wifi_set_log_flag(unsigned int is_log)
{
}

#ifdef PLATFORM_SC6530
static void CsrIToA(int n, char s[])
{
    int i, j, sign;
    char c;
 
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    for (i = 0, j = strlen(s)-1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
static void CsrIToA(int n, char s[])
{
    int i, j, sign;
    char c;
 
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    for (i = 0, j = strlen(s)-1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
#endif


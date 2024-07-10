

#include "sci_types.h"
#include "os_api.h"
#include "pthread.h"
#include "errno-base.h"

#define pthreadDETACH_STATE_MASK      0x8000
#define pthreadSCHED_PRIORITY_MASK    0x7FFF
#define pthreadDETACH_STATE_SHIFT     15
#define pthreadGET_SCHED_PRIORITY( var )    ( ( var ) & ( pthreadSCHED_PRIORITY_MASK ) )
#define pthreadIS_JOINABLE( var )           !( ( ( var ) & ( pthreadDETACH_STATE_MASK ) ) == pthreadDETACH_STATE_MASK )

#define PTHREAD_STACK_MIN2 8192 
#define tskIDLE_PRIORITY			(228+1) //(77)//APP task priority + 1
#define THREAD_QUEUE_NUM    (10)
#define THREAD_NAME_LEN        (12)
#define THREAD_SPACE_LEN       (96)

#define vTaskSuspendAll() SCI_DisableIRQ()
#define xTaskResumeAll() SCI_RestoreIRQ()

#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 20

typedef BLOCK_ID TaskHandle_t;

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )

#define INVALID_KEY_INDEX (0xFFFFFFFF)

typedef struct
{
    unsigned int key;
    
} pthread_key_slot_t;

static uint8 tls_keyslot_initflag = FALSE;
static pthread_key_slot_t key_slot[configNUM_THREAD_LOCAL_STORAGE_POINTERS];

#endif

/**
 * @brief Thread attribute object.
 */
typedef struct pthread_attr_internal
{
    uint32 uiStackSize;                /**< Stack size. */
    uint16 usSchedPriorityDetachState; /**< Schedule priority 15 bits (LSB) Detach state: 1 bits (MSB) */
} pthread_attr_internal_t;

/**
 * @brief Default pthread_attr_t.
 */
static const pthread_attr_internal_t xDefaultThreadAttributes =
{
    PTHREAD_STACK_MIN2,
    ( PTHREAD_CREATE_JOINABLE << pthreadDETACH_STATE_SHIFT )
};

/**
 * @brief Thread object.
 */
typedef struct pthread_internal
{
    pthread_attr_internal_t xAttr;        /**< Thread attributes. */
    void * ( *pvStartRoutine )( void * ); /**< Application thread function. */
    void * xTaskArg;                      /**< Arguments for application thread function. */
    TaskHandle_t xTaskHandle;             /**< FreeRTOS task handle. */
    //StaticSemaphore_t xJoinBarrier;       /**< Synchronizes the two callers of pthread_join. */
    SCI_SEMAPHORE_PTR  xJoinBarrier;
    //StaticSemaphore_t xJoinMutex;         /**< Ensures that only one other thread may join this thread. */
    SCI_MUTEX_PTR xJoinMutex;
    void * xReturn;                       /**< Return value of pvStartRoutine. */
#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
    void *pvThreadLocalStoragePointers[configNUM_THREAD_LOCAL_STORAGE_POINTERS];
#endif
    char dsc[THREAD_SPACE_LEN]; /**<In AArch32 state, returns a pointer to 96 bytes of 4-byte aligned memory */
    char tsk_name[THREAD_NAME_LEN+1];
	int  pthread_errno;
} pthread_internal_t;

/*----------------------------------------------------------- */
//#define USE_STATIC_ARRAY
#ifdef USE_STATIC_ARRAY
typedef struct pthread_id_map
{
    pthread_internal_t *pthread;
    TaskHandle_t xTaskHandle;
} pthread_id_map_t;

#define MAX_PTHREAD_NUM (50)
static pthread_id_map_t g_pthread_id_tbl[MAX_PTHREAD_NUM];
#else
typedef struct pthread_id_map
{
    pthread_internal_t *pthread;
    TaskHandle_t xTaskHandle;
    struct pthread_id_map *next;
} pthread_id_map_t;

static pthread_id_map_t *g_pthread_id_tbl;
#endif

static int task_no = 0;
static BOOLEAN pthread_create_flag = FALSE;
static SCI_MUTEX_PTR g_pthread_map_mutex = NULL;
static SCI_MUTEX_PTR  g_mutex_once = NULL; 

static pthread_t xTaskGetApplicationTaskTag(TaskHandle_t xTaskHandle);

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )

/* Each task contains an array of pointers that is dimensioned by the
configNUM_THREAD_LOCAL_STORAGE_POINTERS setting in FreeRTOSConfig.h.  The
kernel does not use the pointers itself, so the application writer can use
the pointers for any purpose they wish.  The following two functions are
used to set and query a pointer respectively. */
static void vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet, unsigned int xIndex, void *pvValue );
static void *pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, unsigned int xIndex );

/*-----------------------------------------------------------*/

static pthread_internal_t* prvGetTCBFromHandle(TaskHandle_t Taskhdl)
{
    if (Taskhdl == 0)
    {
        return ( pthread_internal_t *)xTaskGetApplicationTaskTag(SCI_IdentifyThread());
    }
    else
    {
        return ( pthread_internal_t *)xTaskGetApplicationTaskTag(Taskhdl);
    }
}

#endif

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )

static void vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet, unsigned int xIndex, void *pvValue )
{
        pthread_internal_t *pxTCB;

	if( xIndex < configNUM_THREAD_LOCAL_STORAGE_POINTERS )
	{
		pxTCB = prvGetTCBFromHandle( xTaskToSet );
		pxTCB->pvThreadLocalStoragePointers[ xIndex ] = pvValue;
	}
}

#endif /* configNUM_THREAD_LOCAL_STORAGE_POINTERS */
/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )

static void *pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, unsigned int xIndex )
{
        void *pvReturn = NULL;
        pthread_internal_t *pxTCB;

	if( xIndex < configNUM_THREAD_LOCAL_STORAGE_POINTERS )
	{
		pxTCB = prvGetTCBFromHandle( xTaskToQuery );
		pvReturn = pxTCB->pvThreadLocalStoragePointers[ xIndex ];
	}
	else
	{
		pvReturn = NULL;
	}

	return pvReturn;
}

#endif /* configNUM_THREAD_LOCAL_STORAGE_POINTERS */

/*-----------------------------------------------------------*/

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )

static void xTaskRunThreadLocalStorageDestructor(pthread_internal_t *pxThread)
{
    int i;
    void (*destructor_function)(void *);
    
    for (i = 0; i < configNUM_THREAD_LOCAL_STORAGE_POINTERS/2; i++)
    {
        if (key_slot[2*i].key != INVALID_KEY_INDEX)
        {
            destructor_function = (void (*)(void *))pxThread->pvThreadLocalStoragePointers[2*i+1];
            if (destructor_function != NULL)
            {
                destructor_function(NULL);
            }
        }
    }
}

#endif

/*-----------------------------------------------------------*/

static void vTaskRemoveApplictionTaskTag(pthread_internal_t *pxThread)
{
	pthread_id_map_t *p_node;
	pthread_id_map_t *pf;

    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);
#ifdef USE_STATIC_ARRAY
     int i;
    for(i = 0; i < MAX_PTHREAD_NUM; i++)
    {
        if (g_pthread_id_tbl[i].pthread == pxThread)
        {
            g_pthread_id_tbl[i].pthread = NULL;
            g_pthread_id_tbl[i].xTaskHandle = SCI_INVALID_BLOCK_ID;
            break;
        }
    }
#else
    p_node = g_pthread_id_tbl;

    if (p_node == NULL)
    {
        SCI_PutMutex(g_pthread_map_mutex);
        return;
    }

    while ( (p_node->pthread != pxThread) && p_node->next != NULL)
    {
        pf = p_node;
        p_node = p_node->next;
    }

    if ((p_node->pthread == pxThread))
    {
        if (p_node == g_pthread_id_tbl)
        {
            g_pthread_id_tbl = g_pthread_id_tbl->next;
        }
        else
        {
            pf->next = p_node->next;
        }

        SCI_FREE(p_node);
    }
    else
    {
        /* not found */
    }

#endif
    SCI_PutMutex(g_pthread_map_mutex);
}

/*-----------------------------------------------------------*/

static void vTaskSetApplicationTaskTag(TaskHandle_t xTaskHandle, pthread_internal_t *pxThread)
{
    pthread_id_map_t *new_node;
    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);

    #ifdef USE_STATIC_ARRAY
    int i;
    
    for (i = 0; i < MAX_PTHREAD_NUM; i++)
    {
        if (g_pthread_id_tbl[i].pthread == NULL)
        {
            g_pthread_id_tbl[i].pthread = pxThread;
            g_pthread_id_tbl[i].xTaskHandle = pxThread->xTaskHandle;
            break;
        }
    }
    #else
    new_node = (pthread_id_map_t *)SCI_ALLOC(sizeof(pthread_id_map_t));

    if (new_node != NULL)
    {
        new_node->pthread = pxThread;
        new_node->xTaskHandle = pxThread->xTaskHandle;

        if (g_pthread_id_tbl == NULL)
        {
            g_pthread_id_tbl = new_node;
            new_node->next = NULL;
        }
        else
        {
            new_node->next = g_pthread_id_tbl;
            g_pthread_id_tbl = new_node;
        }
    }

    #endif
    SCI_PutMutex(g_pthread_map_mutex);
}

/*-----------------------------------------------------------*/

static pthread_t xTaskGetApplicationTaskTag(TaskHandle_t xTaskHandle)
{
    pthread_internal_t * pxThread = NULL;

    if (xTaskHandle != SCI_INVALID_BLOCK_ID)
    {
		pthread_id_map_t *tmp;
        SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);

     #ifdef USE_STATIC_ARRAY
        int i;
        for(i = 0; i < MAX_PTHREAD_NUM; i++)
        {
            if (g_pthread_id_tbl[i].xTaskHandle == xTaskHandle)
            {
                pxThread = g_pthread_id_tbl[i].pthread;
                break;
            }
        }
      #else
        tmp = g_pthread_id_tbl;
      
        while (tmp != NULL)
        {
            if (tmp->xTaskHandle == xTaskHandle)
            {
                pxThread = tmp->pthread;
                break;
            }

            tmp = tmp->next;
        }
      #endif
        
        SCI_PutMutex(g_pthread_map_mutex);
    }
    
    return ( pthread_t )pxThread;
}

/*-----------------------------------------------------------*/

static void prvExitThread( void )
{
    pthread_internal_t * pxThread = ( pthread_internal_t * ) pthread_self();

    /* If this thread is joinable, wait for a call to pthread_join. */
    if( pthreadIS_JOINABLE( pxThread->xAttr.usSchedPriorityDetachState ) )
    {
        SCI_PutSemaphore(pxThread->xJoinBarrier);

        /* Suspend until the call to pthread_join. The caller of pthread_join
         * will perform cleanup. */
        //vTaskSuspend( NULL );
        SCI_SuspendThread(pxThread->xTaskHandle);
    }
    else
    {
        /* For a detached thread, perform cleanup of thread object. */
        //vPortFree( pxThread );
        //vTaskDelete( NULL );
        vTaskRemoveApplictionTaskTag(pxThread);

#if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
        /* run tls destructor */
        xTaskRunThreadLocalStorageDestructor(pxThread);
#endif

        SCI_FREE( pxThread );
        SCI_ThreadExit();
    }
}

/*-----------------------------------------------------------*/

static void prvRunThread(uint32 argc, void* argv)
{
    pthread_internal_t * pxThread = ( pthread_internal_t * ) argv;

    /* Run the thread routine. */
    if (pxThread != NULL && pxThread->pvStartRoutine != NULL)
    {
        pxThread->xReturn = pxThread->pvStartRoutine( ( void * ) pxThread->xTaskArg );
    }
    
    /* Exit once finished. This function does not return. */
    prvExitThread();
}

static char s_user_perthread_libspace[96];

/* The purpose of this API is to implement errno for each task.
Each thread has its own errno stored in a __user_perthread_libspace blcok.
This API returns a pointer to memory for storing data that is local to particular thread.
__user_perthread_libspace() returns a different address depending on the thread it is called from. 
In AArch32 state, returns a pointer to 96 bytes of 4-byte aligned memory
errno address is defined by __aeabi_errno_addr().
*/
void *__user_perthread_libspace(void)
{
    pthread_internal_t *pxThread = NULL;
    pthread_t pthread;

    /* always return __user_libspace() if no pthread is created */
    if (!pthread_create_flag)
    {
        return s_user_perthread_libspace;
    }

    /* always return __user_libspace() in none-thread context */
    if (SCI_IdentifyThread() == SCI_INVALID_BLOCK_ID )
    {
        return s_user_perthread_libspace;
    }

     pthread = xTaskGetApplicationTaskTag(SCI_IdentifyThread());
     
     if (pthread != 0)
     {
        pxThread = (pthread_internal_t *)pthread;
       
        return pxThread->dsc;
     }
     else
     {
        /* return __user_libspace() if it is not a pthread task  */
         return s_user_perthread_libspace;
     }
 
}

/*-----------------------------------------------------------*/

int pthread_attr_init( pthread_attr_t * attr )
{
    /* Copy the default values into the new thread attributes object. */

    if (attr == NULL)
    {
        SCI_TRACE_LOW("NULL attr\n");
        return EINVAL;
    }

    memset(attr, 0, sizeof(pthread_attr_t));

    attr->stack_size = xDefaultThreadAttributes.uiStackSize;
    attr->sched_priority = xDefaultThreadAttributes.usSchedPriorityDetachState & pthreadSCHED_PRIORITY_MASK;
    attr->flags = xDefaultThreadAttributes.usSchedPriorityDetachState & pthreadDETACH_STATE_MASK;

    return 0;
}

/*-----------------------------------------------------------*/

int pthread_attr_setdetachstate( pthread_attr_t * attr, int detachstate )
{
    int iStatus = 0;

    if( ( detachstate != PTHREAD_CREATE_DETACHED ) && ( detachstate != PTHREAD_CREATE_JOINABLE ) )
    {
        iStatus = EINVAL;
    }
    else
    {
        /* clear and then set msb bit to detachstate) */
        attr->flags&= ~pthreadDETACH_STATE_MASK;
        attr->flags |= ( ( uint16 ) detachstate << pthreadDETACH_STATE_SHIFT );
    }

    return iStatus;
}

/*-----------------------------------------------------------*/

int pthread_attr_getdetachstate( const pthread_attr_t * attr, int * detachstate )
{
    if( pthreadIS_JOINABLE( attr->flags) )
    {
        *detachstate = PTHREAD_CREATE_JOINABLE;
    }
    else
    {
        *detachstate = PTHREAD_CREATE_DETACHED;
    }

    return 0;
}

/*-----------------------------------------------------------*/

int pthread_attr_getschedparam( const pthread_attr_t * attr, struct sched_param * param )
{
    
    param->sched_priority = ( int ) (attr->sched_priority);

    return 0;
}

/*-----------------------------------------------------------*/

int pthread_attr_destroy( pthread_attr_t * attr )
{
    ( void ) attr;

    return 0;
}

/*-----------------------------------------------------------*/

int pthread_equal( pthread_t t1, pthread_t t2 )
{
    return t1 == t2;
}

/*-----------------------------------------------------------*/
int pthread_create( pthread_t * thread,
                    const pthread_attr_t * attr,
                    void *( *startroutine )( void * ),
                    void * arg )
{
    int i = 0;
    int iStatus = 0;
    pthread_internal_t * pxThread = NULL;
    struct sched_param xSchedParam = { 0 };

    if (g_pthread_map_mutex == NULL)
    {
        g_pthread_map_mutex = SCI_CreateMutex("pthread_map", SCI_INHERIT);        
    }

    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);    

    if (tls_keyslot_initflag == FALSE)
    {
        for (i = 0; i < configNUM_THREAD_LOCAL_STORAGE_POINTERS; i++)
        {
            key_slot[i].key = INVALID_KEY_INDEX;
        }

        tls_keyslot_initflag = TRUE;
    }

    SCI_PutMutex(g_pthread_map_mutex);
    
    /* Allocate memory for new thread object. */
    pxThread = ( pthread_internal_t * ) SCI_ALLOC( sizeof( pthread_internal_t ) );

    if( pxThread == NULL )
    {
        /* No memory. */
        iStatus = EAGAIN;
    }

    if( iStatus == 0 )
    {
        memset(pxThread, 0, sizeof(pthread_internal_t));
        /* No attributes given, use default attributes. */
        if( attr == NULL )
        {
            pxThread->xAttr = xDefaultThreadAttributes;
        }
        /* Otherwise, use provided attributes. */
        else
        {
            pxThread->xAttr.uiStackSize = attr->stack_size;
            pxThread->xAttr.usSchedPriorityDetachState =  (uint16)((attr->sched_priority)  & pthreadSCHED_PRIORITY_MASK) ;
            pxThread->xAttr.usSchedPriorityDetachState |= (uint16)attr->flags;
        }

        /* Get priority from attributes */
        xSchedParam.sched_priority = ( int ) pthreadGET_SCHED_PRIORITY( pxThread->xAttr.usSchedPriorityDetachState );

        if (xSchedParam.sched_priority == 0)
        {
            xSchedParam.sched_priority = tskIDLE_PRIORITY & pthreadSCHED_PRIORITY_MASK ;
        }
        /* Set argument and start routine. */
        pxThread->xTaskArg = arg;
        pxThread->pvStartRoutine = startroutine;

        /* If this thread is joinable, create the synchronization mechanisms for
         * pthread_join. */

        if( pthreadIS_JOINABLE( pxThread->xAttr.usSchedPriorityDetachState ) )
        {
            /* These calls will not fail when their arguments aren't NULL. */
            //( void ) xSemaphoreCreateMutexStatic( &pxThread->xJoinMutex );
            //( void ) xSemaphoreCreateBinaryStatic( &pxThread->xJoinBarrier );
            pxThread->xJoinMutex = SCI_CreateMutex("xJoinMutex", SCI_INHERIT);
            pxThread->xJoinBarrier = SCI_CreateSemaphore("xJoinBarrier", 0);
            
        }
    }

    if( iStatus == 0 )
    {
        /* Suspend all tasks to create a critical section. This ensures that
         * the new thread doesn't exit before a tag is assigned. */
        vTaskSuspendAll();

        snprintf(pxThread->tsk_name, sizeof(pxThread->tsk_name), "pthread-%d", task_no);

        /* Create the FreeRTOS task that will run the pthread. */
		//fix no const memory issue
        pxThread->xTaskHandle = SCI_CreateAppThread(pxThread->tsk_name, "pthread_Q", prvRunThread, 1, 
                                                                                pxThread, pxThread->xAttr.uiStackSize, 
                                                                                THREAD_QUEUE_NUM, xSchedParam.sched_priority, 1, FALSE);

        if (pxThread->xTaskHandle == SCI_INVALID_BLOCK_ID)
        {
            /* Task creation failed, no memory. */
            SCI_FREE( pxThread );
            iStatus = EAGAIN;
        }
        else
        {
            /* Store the pointer to the thread object in the task tag. */
            vTaskSetApplicationTaskTag( pxThread->xTaskHandle, ( pthread_internal_t *) pxThread );

            /* Set the thread object for the user. */
            *thread = ( pthread_t ) pxThread;
             task_no++;
             pthread_create_flag = TRUE;
        }
        
        /* End the critical section. */
        xTaskResumeAll();
        
        if (iStatus == 0 && pxThread->xTaskHandle != SCI_INVALID_BLOCK_ID)
        {
            SCI_ResumeThread(pxThread->xTaskHandle);
        }
    }

    return iStatus;
}

/*-----------------------------------------------------------*/

void pthread_exit( void * value_ptr )
{
    pthread_internal_t * pxThread = ( pthread_internal_t * ) pthread_self();

    /* Set the return value. */
    pxThread->xReturn = value_ptr;

    /* Exit this thread. */
    prvExitThread();
}

/*-----------------------------------------------------------*/

int pthread_join( pthread_t pthread, void ** retval )
{
    int iStatus = 0;
    pthread_internal_t * pxThread = ( pthread_internal_t * ) pthread;

    if (pxThread == NULL)
    {
        return EINVAL;
    }

    /* Make sure pthread is joinable. Otherwise, this function would block
     * forever waiting for an unjoinable thread. */
    if( !pthreadIS_JOINABLE( pxThread->xAttr.usSchedPriorityDetachState ) )
    {
        iStatus = EDEADLK;
    }

    /* Only one thread may attempt to join another. Lock the join mutex
     * to prevent other threads from calling pthread_join on the same thread. */
    if( iStatus == 0 )
    {
        if( SCI_GetMutex(pxThread->xJoinMutex, SCI_WAIT_FOREVER ) != SCI_SUCCESS )
        {
            /* Another thread has already joined the requested thread, which would
             * cause this thread to wait forever. */
            iStatus = EDEADLK;
        }
    }

    /* Attempting to join the calling thread would cause a deadlock. */
    if( iStatus == 0 )
    {
        if( pthread_equal( pthread_self(), pthread ) != 0 )
        {
            iStatus = EDEADLK;
        }
    }

    if( iStatus == 0 )
    {
        /* Wait for the joining thread to finish. Because this call waits forever,
         * it should never fail. */
        SCI_GetSemaphore(pxThread->xJoinBarrier, SCI_WAIT_FOREVER);

        /* Create a critical section to clean up the joined thread. */
        vTaskSuspendAll();

        /* Release xJoinBarrier and delete it. */
        SCI_PutSemaphore(pxThread->xJoinBarrier);
        SCI_DeleteSemaphore(pxThread->xJoinBarrier);

        /* Release xJoinMutex and delete it. */
        SCI_PutMutex(pxThread->xJoinMutex );
        SCI_DeleteMutex(pxThread->xJoinMutex );

        /* Delete the FreeRTOS task that ran the thread. */
        SCI_TerminateThread(pxThread->xTaskHandle);
        SCI_DeleteThread(pxThread->xTaskHandle);
        //vTaskDelete( pxThread->xTaskHandle );

        /* Set the return value. */
        if( retval != NULL )
        {
            *retval = pxThread->xReturn;
        }

        vTaskRemoveApplictionTaskTag(pxThread);

    #if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS != 0 )
        /* run tls destructor */
        xTaskRunThreadLocalStorageDestructor(pxThread);
    #endif

        /* Free the thread object. */
        SCI_FREE( pxThread );

        /* End the critical section. */
        xTaskResumeAll();
    }

    return iStatus;
}

/*-----------------------------------------------------------*/

int pthread_detach(pthread_t  thid)
{
    pthread_internal_t*  pxThread = (pthread_internal_t*)thid;

    if (thid == 0)
        return -1;

    pxThread->xAttr.usSchedPriorityDetachState |= PTHREAD_CREATE_DETACHED << pthreadDETACH_STATE_SHIFT;

    pxThread->xJoinMutex = SCI_CreateMutex("xJoinMutex", SCI_INHERIT);
    pxThread->xJoinBarrier = SCI_CreateSemaphore("xJoinBarrier", 0);
            
    return 0;
}


pthread_t pthread_self( void )
{
    /* Return a reference to this pthread object, which is stored in the
     * FreeRTOS task tag. */
    return ( pthread_t ) xTaskGetApplicationTaskTag(SCI_IdentifyThread());
}

/*-----------------------------------------------------------*/

int pthread_key_create(pthread_key_t *key, void (*destructor_function)(void *))
{
    unsigned int k;

    if (key == NULL)
    {
        return EINVAL;
    }

    if (g_pthread_map_mutex == NULL)
    {
        return EINVAL;
    }

    *key = INVALID_KEY_INDEX;
    
    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);

    /* search for the vacancy */
    for (k = 0; k < configNUM_THREAD_LOCAL_STORAGE_POINTERS/2; k++)
    {
        if (key_slot[2*k].key == INVALID_KEY_INDEX)
        {
            key_slot[2*k].key = k;
            *key = k;
            break;
        }
    }

    SCI_PutMutex(g_pthread_map_mutex);
    
    if (*key == INVALID_KEY_INDEX)
    {
        return EINVAL;
    }

    
    k = *key * 2 ;
    
    vTaskSetThreadLocalStoragePointer(0, k, NULL);
    vTaskSetThreadLocalStoragePointer(0, k + 1, (void*) destructor_function);
    
    return 0;
}

/*-----------------------------------------------------------*/

int pthread_key_delete (pthread_key_t key)
{
    int i;

    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);
    
    for (i = 0; i < configNUM_THREAD_LOCAL_STORAGE_POINTERS/2; i++)
    {
        if (key_slot[i *2].key == key)
        {
            key_slot[i *2].key = INVALID_KEY_INDEX;
            break;
        }
    }

    SCI_PutMutex(g_pthread_map_mutex);

    if (i == configNUM_THREAD_LOCAL_STORAGE_POINTERS/2)
    {
        return EINVAL;
    }

    return 0;
    
}

/*-----------------------------------------------------------*/

int pthread_setspecific(pthread_key_t key, const void *value)
{
    int i;

    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);
    
    for (i = 0; i < configNUM_THREAD_LOCAL_STORAGE_POINTERS/2; i++)
    {
        if (key_slot[i *2].key == key)
        {
            break;
        }
    }

    SCI_PutMutex(g_pthread_map_mutex);

    if (i == configNUM_THREAD_LOCAL_STORAGE_POINTERS/2)
    {
        return EINVAL;
    }

    vTaskSetThreadLocalStoragePointer(0, i * 2, (void *)value);

    return 0;
    
}

/*-----------------------------------------------------------*/

void *pthread_getspecific(pthread_key_t key)
{
    int i;
    
    SCI_GetMutex(g_pthread_map_mutex, SCI_WAIT_FOREVER);
    
    for (i = 0; i < configNUM_THREAD_LOCAL_STORAGE_POINTERS/2; i++)
    {
        if (key_slot[i*2].key == key)
        {
            break;
        }
    }

    SCI_PutMutex(g_pthread_map_mutex);

    if (i == configNUM_THREAD_LOCAL_STORAGE_POINTERS/2)
    {
        return NULL;
    }

    return pvTaskGetThreadLocalStoragePointer(0, i*2);
    
}

/*-----------------------------------------------------------*/

int pthread_once(pthread_once_t  *once_control, void (*init_routine)(void))
{
  /* XXX Depending on whether the LOCK_IN_ONCE_T is defined use a
     global lock variable or one which is part of the pthread_once_t
     object.  */

    if (g_mutex_once == NULL)
    {
        g_mutex_once = SCI_CreateMutex("mut_once", SCI_INHERIT);

        if (g_mutex_once == SCI_NULL)
        {
            return EINVAL;
        }
    }
  
    if (*once_control == PTHREAD_ONCE_INIT)
    {
        SCI_GetMutex(g_mutex_once, SCI_WAIT_FOREVER);

      /* XXX This implementation is not complete.  It doesn't take
	 cancellation and fork into account.  */
        if (*once_control == PTHREAD_ONCE_INIT)
	{
            init_routine ();

            *once_control = !PTHREAD_ONCE_INIT;
	}

        SCI_PutMutex(g_mutex_once);
    }

    return 0;
}

int pthread_get_task_id(pthread_t thread)
{
    pthread_internal_t *pxThread = (pthread_internal_t *)thread;

    return pxThread->xTaskHandle;
}

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void(*child)(void))
{
	return 0;
}

int pthread_kill(pthread_t tid, int sig)
{
	return 0;
}

int pthread_setname_np(pthread_t thid, const char *thname)
{
    int len = 0;
    pthread_internal_t* pxThread = (pthread_internal_t*)thid;

    if (pxThread == NULL)
    {
        return EINVAL;
    }

    len = strlen(thname);

	SCI_MEMSET(pxThread->tsk_name, 0, THREAD_NAME_LEN);
	strncpy(pxThread->tsk_name, thname, MIN(THREAD_NAME_LEN, len));
	return 0;
}

int pthread_attr_setstacksize(pthread_attr_t * attr, size_t stack_size)
{
    if (stack_size < PTHREAD_STACK_MIN2 )
    {
    	return EINVAL;
    }

	attr->stack_size = stack_size;
	return 0;
}

int pthread_attr_setschedparam(pthread_attr_t * attr, struct sched_param const * param)
{
    attr->sched_priority = param->sched_priority;
	return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	*attr = PTHREAD_MUTEX_DEFAULT;
	return 0;
}


int pthread_setschedparam(pthread_t thid, int poilcy,
                          struct sched_param const * param)
{
	return 0;
}


int* pthread_errno(void) 
{
	int* ret = PNULL;
    pthread_internal_t *pxTCB = PNULL;

	SCI_DisableIRQ();

	pxTCB = prvGetTCBFromHandle(0);

	SCI_ASSERT(PNULL != pxTCB);

	ret = &pxTCB->pthread_errno;

	SCI_RestoreIRQ();

	return ret;
 }



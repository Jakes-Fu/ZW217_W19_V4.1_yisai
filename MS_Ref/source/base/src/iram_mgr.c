/* TBD:
 * Yes, a very silly way is used to check collision and select runnable client.
 * But could you find a smarter one? Certainly red-black tree can be used 
 * instead of linked list.
 */
#include "ms_ref_base_trc.h"
#include "iram_mgr.h"
#include "os_api.h"
#include "os_import.h"
#include "sci_api.h"
#include "priority_system.h"
/*-----------------------------------------------------------------------------
 * debug facilities
 */
#define IRAM_UNIT_TEST      0
#define IRAM_DEBUG_ON       1
#define IRAM_TRACE_ON       0

#if IRAM_DEBUG_ON
#define IRAM_ASSERT(_x_)    SCI_ASSERT(_x_)
#else
#define IRAM_ASSERT(_x_)    ((void)0)
#endif /* IRAM_DEBUG_ON */

#if IRAM_TRACE_ON
#define IRAM_TRACE(_x_)     SCI_TraceLow _x_
#else
#define IRAM_TRACE(_x_)     ((void)0)
#endif /* IRAM_TRACE_ON */

#define TASK_IRAM_TESER_PRI   PRI_IRAM_TESER_TASK  //30

/*-----------------------------------------------------------------------------
 * feature switch
 */
#define ENABLE_RANGE_CHECK              1
 
/*-----------------------------------------------------------------------------
 * double-linked list
 */
typedef struct _LIST_NODE {
    struct _LIST_NODE *prev, *next;
} LIST_NODE;

static __inline void INIT_LIST_NODE(LIST_NODE *p)
{
    p->prev = p->next = p;
}

/* Add a node before head */
static __inline void LIST_ADD_BEFORE(LIST_NODE *node, LIST_NODE *head)
{
	LIST_NODE *prev = head->prev;
	prev->next = node;
	node->prev = prev;
	node->next = head;
	head->prev = node;
}

static __inline void LIST_DEL(LIST_NODE *p)
{
	p->next->prev = p->prev;
	p->prev->next = p->next;
	
	INIT_LIST_NODE(p);
}

static __inline BOOLEAN LIST_IS_EMPTY(LIST_NODE *head)
{
	return (head->next == head);
}

#define LIST_ENTRY(p, type, member) \
	((type*)((char*)p - (unsigned long)(&((type*)0)->member)))

/*-----------------------------------------------------------------------------
 * macro definitions
 */
#define IRAM_MUTEX_NAME         "IRAM MUTEX"
#define IRAM_EVENT_NAME         "IRAM EVENT"

typedef uint8   CLIENT_STATE;
#define CLIENT_IDLE         ((CLIENT_STATE)0)
#define CLIENT_WAITING      ((CLIENT_STATE)1)   /* client is waiting for area */
#define CLIENT_RUNNING      ((CLIENT_STATE)2)   /* client occupies area */

#define CLIENT_MAGIC        (0x49434C54) /* 'ICLT' */
#define LOCK_MAGIC          (0x494C434B) /* 'ILCK' */
 	
/*-----------------------------------------------------------------------------
 * data structures
 */ 
struct _AREA_NODE; 

typedef struct _IRAM_CLIENT {
    uint32              magic;
    struct _AREA_NODE   *area;
    LIST_NODE           waiting_list;
    uint32              wait_flag;
    BOOLEAN             temporary;
    CLIENT_STATE        state;
    BOOLEAN             forceZI;
} IRAM_CLIENT;

typedef struct _IRAM_LOCK {
    uint32              magic;
    IRAM_CLIENT         *owner;
} IRAM_LOCK;

typedef struct _AREA_NODE {
    uint32              start_addr;
    uint32              length;
    uint32              attri;
    void                *data;
    BLOCK_ID        task_id;
    BOOLEAN         data_avail;
    struct _AREA_NODE   *next;
    LIST_NODE           occupied_list;
} AREA_NODE;

/*-----------------------------------------------------------------------------
 * static data
 */
static SCI_MUTEX_PTR        g_iram_mutex = SCI_NULL;
static SCI_EVENT_GROUP_PTR  g_iram_event = SCI_NULL;
static uint32               g_iram_event_bitmap = 0;
static LIST_NODE            g_iram_occupied_area;
static LIST_NODE            g_iram_waiting_client;

#if ENABLE_RANGE_CHECK
static IRAM_AREA            *g_iram_avail_area = NULL;
static uint32               g_iram_avail_area_count = 0;
#endif /* ENABLE_RANGE_CHECK */


/*-----------------------------------------------------------------------------
 * static functions
 */
/* TBD:
 * These objects might be allocated from a cache.
 */ 
static IRAM_CLIENT *_iram_alloc_client(void)
{
    IRAM_CLIENT *cl;
    cl = (IRAM_CLIENT*)SCI_ALLOC_APP(sizeof(*cl));
    
    if (cl)
        cl->magic = CLIENT_MAGIC;
    
    return cl;
}

static void _iram_free_client(IRAM_CLIENT *cl)
{
    IRAM_ASSERT(cl);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    
    cl->magic = 0; /*lint !e613*/ /* in case OS does not zero freed memory */
    SCI_FREE(cl);
}

static IRAM_LOCK *_iram_alloc_lock(void)
{
    IRAM_LOCK *lock;
    
    lock = (IRAM_LOCK*)SCI_ALLOC_APP(sizeof(*lock));
    if (lock)
        lock->magic = LOCK_MAGIC;
    
    return lock; 
}

static void _iram_free_lock(IRAM_LOCK *lock)
{
    IRAM_ASSERT(lock);/*assert verified*/
    IRAM_ASSERT(LOCK_MAGIC == lock->magic);/*assert verified*/
    
    lock->magic = 0;/*lint !e613*/
    SCI_FREE(lock);
}

static AREA_NODE *_iram_alloc_node(void)
{
    AREA_NODE *node = (AREA_NODE*)SCI_ALLOC_APP(sizeof(AREA_NODE));
    return node;
}

static void _iram_free_node(AREA_NODE *node)
{
    SCI_FREE(node);
} 

static BOOLEAN _iram_lock(void)
{
    BOOLEAN ret;
    
    IRAM_ASSERT(g_iram_mutex);/*assert verified*/
    
    ret = (SCI_SUCCESS == SCI_GetMutex(g_iram_mutex, 
        SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0)); 
    
    if (!ret)
        IRAM_TRACE(("[_iram_lock] get mutex failed"));
    
    return ret;
}

static void _iram_unlock(void)
{
    SCI_PutMutex(g_iram_mutex);
}

/* TBD:
 * Now, only 32 concurrent waiting clients are allowed.
 */
static uint32 _iram_get_wait_flag(void)
{
    uint32 flag;
    
    flag = 1;
    while (g_iram_event_bitmap & flag) {
        flag <<= 1;
    }
    
    IRAM_TRACE(("[_iram_get_wait_flag] flag = 0x%x", flag));
    IRAM_ASSERT(flag);/*assert verified*/

    g_iram_event_bitmap |= flag;
    return flag;
}

static void _iram_free_wait_flag(uint32 flag)
{
    IRAM_TRACE(("[_iram_free_wait_flag] flag = 0x%x", flag));
    
    IRAM_ASSERT(flag);/*assert verified*/
    IRAM_ASSERT(g_iram_event_bitmap & flag);/*assert verified*/
    
    g_iram_event_bitmap &= ~flag;
}

static BOOLEAN _iram_is_collided(uint32 addr1, uint32 len1, uint32 addr2, uint32 len2)
{
    return !((addr1 + len1 <= addr2) || (addr2 + len2 <= addr1));
}

#if ENABLE_RANGE_CHECK
static BOOLEAN _iram_check_area_in_range(const IRAM_AREA *area)
{
    int i;
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_TRACE(("[_iram_check_area_in_range] IN (iram_addr = 0x%x, iram_length = %d)",
        area->iram_addr, area->iram_length));
    
    for (i = 0; i < (int)g_iram_avail_area_count; ++i) {
        const IRAM_AREA *p = &g_iram_avail_area[i];
  /*lint -esym(613, area) */     
        if (area->iram_addr < p->iram_addr) {
            /* avail area has been sorted */
            IRAM_TRACE(("[_iram_check_area_in_range] out of range"));
            return FALSE;
        }
            
        if ((area->iram_addr >= p->iram_addr)
            && (area->iram_addr + area->iram_length <= p->iram_addr + p->iram_length)) {
            IRAM_TRACE(("[_iram_check_area_in_range] in range"));
            return TRUE;
        }
    }
     
    IRAM_TRACE(("[_iram_check_area_in_range] out of range")); 
    return FALSE;       
}

static BOOLEAN _iram_check_in_range(const IRAM_AREA *area, int32 count)
{
    int i;
    
    IRAM_TRACE(("[_iram_check_in_range] IN (area = 0x%x, count = %d)",
        area, count));
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    
    for (i = 0; i < count; ++i) {
        if (!_iram_check_area_in_range(area + i)) {
            IRAM_ASSERT(0 && "area out of range");/*assert verified*/
            return FALSE;  /*lint !e527 */
        }
    }
    
    return TRUE;      
}
#endif /* ENABLE_RANGE_CHECK */

static BOOLEAN _iram_check_collision(const IRAM_AREA *area, int32 count)
{
    int i, j;
    
    IRAM_TRACE(("[_iram_check_collision] IN (area = 0x%x, count = %d)",
        area, count));
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    
    /* check length of the last item */
    IRAM_TRACE(("[_iram_check_collision] (%d) addr = 0x%x, length = %d", count - 1,
        area[count - 1].iram_addr, area[count - 1].iram_length)); 
        
    IRAM_ASSERT(area[count - 1].iram_length);/*assert verified*/
    if (!area[count - 1].iram_length)
        return FALSE;
        
    for (i = 0; i < count - 1; ++i) {
        /* check length of the ith item */
        IRAM_TRACE(("[_iram_check_collision] (%d) addr = 0x%x, length = %d",
            i, area[i].iram_addr, area[i].iram_length));
        
        IRAM_ASSERT(area[i].iram_length);/*assert verified*/
        if (!area[i].iram_length)
            return FALSE;
        
        for (j = i + 1; j < count; ++j) {
            if (_iram_is_collided(area[i].iram_addr, area[i].iram_length,
                    area[j].iram_addr, area[j].iram_length)) {
                IRAM_TRACE(("[_iram_check_collision]  collided with (%d: addr = 0x%x, length = %d)",
                    j, area[j].iram_addr, area[j].iram_length));    
                IRAM_ASSERT(0 && "area collided");/*assert verified*/
                return FALSE;  /*lint !e527 */
            }
        }
    }
    
    return TRUE; 
}

static BOOLEAN _iram_check_area(const IRAM_AREA *area, int32 count)
{
#if ENABLE_RANGE_CHECK
    return _iram_check_in_range(area, count) && _iram_check_collision(area, count);
#else /* ENABLE_RANGE_CHECK */
    return _iram_check_collision(area, count);
#endif /* ENABLE_RANGE_CHECK */        
}

static void _iram_destroy_client(IRAM_CLIENT *client)
{
    AREA_NODE *p;
    
    IRAM_TRACE(("[_iram_destroy_client] IN"));
    
    IRAM_ASSERT(client);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == client->magic);/*assert verified*/
    IRAM_ASSERT(CLIENT_IDLE == client->state);/*assert verified*/
    IRAM_ASSERT(LIST_IS_EMPTY(&client->waiting_list));/*assert verified*/
    
    /* free all area node */
    IRAM_TRACE(("[_iram_destroy_client] free area node:"));
    
    p = client->area;/*lint !e613*/
    while (p) {
        AREA_NODE *q;
        
        IRAM_TRACE(("start_addr = 0x%x, length = %d, data = 0x%x, attri = 0x%x",
            p->start_addr, p->length, p->data, p->attri));
        
        /* The node should not be on the occupied area list */
        IRAM_ASSERT(LIST_IS_EMPTY(&p->occupied_list));/*assert verified*/
        
        if (p->attri & IRAM_ATTRIBUTE_SAVE_CONTEXT) {
            IRAM_ASSERT(p->data);/*assert verified*/
            SCI_FREE(p->data);
        }
        
        q = p;
        p = p->next;
        _iram_free_node(q);
    }
    
    _iram_free_client(client);
}

static IRAM_CLIENT *_iram_create_client(const IRAM_AREA *area, int32 count, BOOLEAN temporary)
{
    IRAM_CLIENT *client;
    int         i;
    AREA_NODE   **p;
    
    IRAM_TRACE(("[_iram_create_client] IN (area = 0x%x, count = %d, temporary = %d)",
        area, count, temporary));
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    
    if (!_iram_check_area(area, count)) {
        IRAM_TRACE(("[_iram_create_client] _iram_check_area failed"));
        return NULL;
    }
        
    client = _iram_alloc_client();
    if (!client) {
        IRAM_TRACE(("[_iram_create_client] fail to alloc client"));
        return NULL;
    }

    client->area = NULL;
    INIT_LIST_NODE(&client->waiting_list);
    client->wait_flag = 0;
    client->temporary = temporary;
    client->state = CLIENT_IDLE;
    client->forceZI = FALSE; /* default to FALSE */
    
    /* build area list */
    IRAM_TRACE(("[_iram_create_client] build area list:"));
    
    p = &client->area;
    for (i = 0; i < count; ++i) {
        AREA_NODE *node;
        
        IRAM_TRACE(("(%d) iram_addr = 0x%x, iram_length = %d, data = 0x%x, attri = 0x%x",
            i, area[i].iram_addr, area[i].iram_length, area[i].data, area[i].attri));
        
        node = _iram_alloc_node();
        if (!node) {
            IRAM_TRACE(("[_iram_create_client] fail to alloc node"));
            break;
        }
        
        node->start_addr = area[i].iram_addr;
        node->length = area[i].iram_length;
        node->attri = area[i].attri;
        if (client->temporary)
            node->attri &= ~IRAM_ATTRIBUTE_SAVE_CONTEXT; /* ignore */
            
        node->data = area[i].data;
        node->data_avail = TRUE;
        node->next = NULL;
        INIT_LIST_NODE(&node->occupied_list);
        
        if (node->attri & IRAM_ATTRIBUTE_SAVE_CONTEXT) {
            node->data = (void*)SCI_ALLOC_APP(node->length);
            if (!node->data) {
                IRAM_TRACE(("[_iram_create_client] fail to alloc data"));
                _iram_free_node(node);
                break;
            }
            
            if (area[i].data) {
                SCI_MEMCPY(node->data, area[i].data, node->length);
            } else {
                node->data_avail = FALSE;
            }
        }
        
        /* Add the node to client's area list */
        *p = node;
        p = &node->next;
    }
    
    if (i != count) {
        /* fail to build area list */
        _iram_destroy_client(client);
        client = NULL;
    }
    
    return client;    
}

static BOOLEAN _iram_try_lock_one_area(AREA_NODE *candi)
{
    LIST_NODE *list;
    
    IRAM_ASSERT(candi);/*assert verified*/
    IRAM_ASSERT(LIST_IS_EMPTY(&candi->occupied_list));/*assert verified*/
    
    IRAM_TRACE(("[_iram_try_lock_one_area] IN (start_addr = 0x%x, length = %d)",
        candi->start_addr, candi->length));
    
    list = g_iram_occupied_area.next;
    while (list != &g_iram_occupied_area) {
        AREA_NODE *node;
        
        node = LIST_ENTRY(list, AREA_NODE, occupied_list);  /*lint !e413 */
        /*lint -esym(613, candi) */
        
        if (_iram_is_collided(node->start_addr, node->length, candi->start_addr, candi->length)) {
            IRAM_TRACE(("[_iram_try_lock_one_area] collided with node (start_addr = 0x%x, length = %d)",
                node->start_addr, node->length));
            return FALSE;
        }
        
        if (candi->start_addr < node->start_addr) {
            LIST_ADD_BEFORE(&candi->occupied_list, list);	
	    candi->task_id =  SCI_IdentifyThread();
            IRAM_TRACE(("[_iram_try_lock_one_area]  node added successfully"));
            return TRUE;
        }
        
        list = list->next;
    }
    
    /* Larger than any area, add to the tail of occupied list */
    LIST_ADD_BEFORE(&candi->occupied_list, &g_iram_occupied_area);
	    candi->task_id =  SCI_IdentifyThread();
    IRAM_TRACE(("[_iram_try_lock_one_area] node added at the tail of the list"));
    return TRUE;
}

static BOOLEAN _iram_try_lock_area(IRAM_CLIENT *cl)
{
    AREA_NODE *last;
    
    IRAM_TRACE(("[_iram_try_lock_area] IN (cl = 0x%x, forceZI = %d)",
        cl, cl->forceZI));
    
    IRAM_ASSERT(cl);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    IRAM_ASSERT(CLIENT_RUNNING != cl->state);/*assert verified*/
    /*lint -esym(613,  cl) */
    last = cl->area;
    IRAM_ASSERT(last);/*assert verified*/
    
    while (last) {
        if (!_iram_try_lock_one_area(last))
            break;
        
        last = last->next;
    }         
    
    if (!last) {
        /* all area be added to occupied list, copy context */
        IRAM_TRACE(("[_iram_try_lock_area] all area added"));
        
        last = cl->area;
        
        while (last) {
            if (cl->forceZI && (last->attri & IRAM_ATTRIBUTE_ZI)) {
                IRAM_TRACE(("[_iram_try_lock_area] force ZI (start_addr = 0x%x, length = %d)",
                    last->start_addr, last->length));
                    
                last->data_avail = FALSE;
            }
            
            if (last->data && last->data_avail) {
                IRAM_TRACE(("[_iram_try_lock_area] restore context (start_addr = 0x%x, length = %d)",
                    last->start_addr, last->length));
                    
                memcpy((void *)last->start_addr, last->data, last->length);
            } else if (last->attri & IRAM_ATTRIBUTE_ZI) {
                IRAM_TRACE(("[_iram_try_lock_area] zi (start_addr = 0x%x, length = %d)",
                    last->start_addr, last->length));
                    
                memset((void *)last->start_addr, 0, last->length);
            }
            
            last = last->next;
        }
        
        return TRUE;
    } else {
        /* at least one area can't be added, undo */
        AREA_NODE *p;
        
        IRAM_TRACE(("[_iram_try_lock_area] fail to add one area"));
        
        p = cl->area;
        while (p != last) {
            /* The node has been added to the list, remove it */
            IRAM_ASSERT(!LIST_IS_EMPTY(&p->occupied_list));/*assert verified*/
            LIST_DEL(&p->occupied_list);
            
            p = p->next;
        }
        
        return FALSE;
    }
}

/* before the function is called, lock must be got */
static BOOLEAN _iram_lock_area(IRAM_CLIENT *cl, uint32 wait)
{
    BOOLEAN ret;
    uint32  flag;
    
    IRAM_TRACE(("[_iram_lock_area] IN (cl = 0x%x, wait = 0x%x)",
        cl, wait));
        
    IRAM_ASSERT(cl);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    
    IRAM_ASSERT(CLIENT_IDLE == cl->state);/*assert verified*/
    if (CLIENT_IDLE != cl->state)
        return FALSE; 
    
    /* As state is idle, it should not be on waiting list */
    IRAM_ASSERT(LIST_IS_EMPTY(&cl->waiting_list));/*assert verified*/
    
    if (_iram_try_lock_area(cl)) {
        IRAM_TRACE(("[_iram_lock_area] try lock area succeeded"));
        cl->state = CLIENT_RUNNING;
        return TRUE;
    }
    
    IRAM_TRACE(("[_iram_lock_area] try lock area failed"));
    
    if (0 == wait) {
        IRAM_TRACE(("[_iram_lock_area] no wait"));
        return FALSE;
    }
    
    /* Put the client on the waiting list */
    cl->wait_flag = _iram_get_wait_flag();
    if (0 == cl->wait_flag) {
        IRAM_TRACE(("[_iram_lock_area] get wait flag failed"));
        return FALSE;
    }
    
    /* add to the tail of waiting list */
    LIST_ADD_BEFORE(&cl->waiting_list, &g_iram_waiting_client);
    cl->state = CLIENT_WAITING;
    
    _iram_unlock();
    
    IRAM_TRACE(("[_iram_lock_area] waiting..."));
    
    SCI_GetEvent(g_iram_event, cl->wait_flag, SCI_AND_CLEAR, &flag, wait);
    
    IRAM_TRACE(("[_iram_lock_area] waked up"));
    
    if (!_iram_lock()) {
        /* If lock failed, Oop... */
        IRAM_ASSERT(0);/*assert verified*/
    }
    
    if (CLIENT_RUNNING == cl->state) {
        /* We get the iram */
        IRAM_TRACE(("[_iram_lock_area] lock area succeeded"));
        
        /* assert the client has been removed from waiting list */
        IRAM_ASSERT(LIST_IS_EMPTY(&cl->waiting_list));/*assert verified*/
        
        SCI_GetEvent(g_iram_event, cl->wait_flag, SCI_AND_CLEAR, &flag, 0); /* avoid race condition */
        ret = TRUE;
    } else {
        /* fail to get the iram, remove the client from waiting list */
        IRAM_TRACE(("[_iram_lock_area]  fail to get iram"));
        IRAM_ASSERT(CLIENT_WAITING == cl->state);/*assert verified*/
        IRAM_ASSERT(!LIST_IS_EMPTY(&cl->waiting_list));/*assert verified*/
    
        LIST_DEL(&cl->waiting_list);
        cl->state = CLIENT_IDLE;
    
        ret = FALSE;
    }
    
    /* free wait flag */
    _iram_free_wait_flag(cl->wait_flag);
    cl->wait_flag = 0;
    return ret;
}

static BOOLEAN _iram_unlock_area(IRAM_CLIENT *cl)
{
    AREA_NODE *p;
    LIST_NODE *list;
    
    IRAM_TRACE(("[_iram_unlock_area] IN (cl = 0x%x)", cl));
    
    IRAM_ASSERT(cl);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    
    IRAM_ASSERT(CLIENT_RUNNING == cl->state);/*assert verified*/
    if (CLIENT_RUNNING != cl->state)
        return FALSE;
    
    /* this client should not be on the waiting list */
    IRAM_ASSERT(LIST_IS_EMPTY(&cl->waiting_list));/*assert verified*/
    
    p = cl->area;
    IRAM_ASSERT(p);/*assert verified*/
    
    /* remove all area of the client from occupied list */
    IRAM_TRACE(("[_iram_unlock_area] remove area from occupied list"));
    
    while (p) {
        IRAM_TRACE(("[_iram_unlock_area] start_addr = 0x%x, length = %d, attri = %d",
            p->start_addr, p->length, p->attri));
        
        IRAM_ASSERT(!LIST_IS_EMPTY(&p->occupied_list));/*assert verified*/
        LIST_DEL(&p->occupied_list);
        
        /* save context if necessary */
        if (p->attri & IRAM_ATTRIBUTE_SAVE_CONTEXT) {
            IRAM_TRACE(("[_iram_unlock_area] save context"));
            
            IRAM_ASSERT(p->data);/*assert verified*/
            memcpy(p->data, (void *)p->start_addr, p->length);
            p->data_avail = TRUE;
        }
        
        p = p->next;
    }
    
    cl->state = CLIENT_IDLE;
    
    /* Try to wake up waiting client */
    list = g_iram_waiting_client.next;
    while (list != &g_iram_waiting_client) {
        IRAM_CLIENT *candi;
        
        candi = LIST_ENTRY(list, IRAM_CLIENT, waiting_list);   /*lint !e413 */
        IRAM_ASSERT(CLIENT_WAITING == candi->state);/*assert verified*/
        
        if (_iram_try_lock_area(candi)) {
            LIST_NODE *n;
            
            IRAM_TRACE(("[_iram_unlock_area] _iram_try_lock_area succeeded, wake it up"));
            
            /* remove the client from waiting list and wake it up */
            n = list;
            list = list->next;
            LIST_DEL(n);
            
            candi->state = CLIENT_RUNNING;
            IRAM_ASSERT(candi->wait_flag);/*assert verified*/
            SCI_SetEvent(g_iram_event, candi->wait_flag, SCI_OR);
        } else {
            IRAM_TRACE(("[_iram_unlock_area] _iram_try_lock_area failed"));
            list = list->next;
        }
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Interface
 */
BOOLEAN IRAM_Init(const IRAM_AREA *area, int32 count)
{
    int i;
    
    IRAM_TRACE(("[IRAM_Init] IN (area = 0x%x, count = %d)", area, count));
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    
#if ENABLE_RANGE_CHECK    
    IRAM_ASSERT(0 == g_iram_avail_area_count);/*assert verified*/
    IRAM_ASSERT(!g_iram_avail_area);/*assert verified*/
#endif /* ENABLE_RANGE_CHECK */    
    IRAM_ASSERT(SCI_NULL == g_iram_mutex);/*assert verified*/
    IRAM_ASSERT(SCI_NULL == g_iram_event);/*assert verified*/

    for (i = 0; i < count; ++i) {
        IRAM_TRACE(("[IRAM_Init] (%d) iram_addr = 0x%x, iram_length = %d",
            i, area[i].iram_addr, area[i].iram_length));
        
        IRAM_ASSERT(area[i].iram_length);/*assert verified*/
        if (0 == area[i].iram_length)
            return FALSE;    
    }
    
    do {
#if ENABLE_RANGE_CHECK    
        int j;
        BOOLEAN collided;
#endif /* ENABLE_RANGE_CHECK */        
        
        INIT_LIST_NODE(&g_iram_occupied_area);
        INIT_LIST_NODE(&g_iram_waiting_client);
        
        g_iram_mutex = SCI_CreateMutex(IRAM_MUTEX_NAME, SCI_INHERIT);
        if (SCI_NULL == g_iram_mutex) {
            IRAM_TRACE(("[IRAM_Init] Fail to create mutex"));
            break;
        }
        
        g_iram_event_bitmap = 0;
        g_iram_event = SCI_CreateEvent(IRAM_EVENT_NAME);
        if (SCI_NULL == g_iram_event) {
            IRAM_TRACE(("[IRAM_Init] Fail to create event"));
            break;
        } 
        
#if ENABLE_RANGE_CHECK
        g_iram_avail_area_count = count; 
        g_iram_avail_area = (IRAM_AREA*)SCI_ALLOC_APP(count * (int32)sizeof(*area));
        if (!g_iram_avail_area) {
            IRAM_TRACE(("[IRAM_Init] fail to alloc _iram_avail_area"));
            break;
        }
            
        SCI_MEMSET(g_iram_avail_area, 0, (count * (int32)sizeof(*area)));
        
        /* check collision and sort */
        collided = FALSE;
        for (i = 0; (i < count - 1) && !collided; ++i) { 
            for (j = i + 1; (j < count) && !collided; ++j) {
                if (_iram_is_collided(area[i].iram_addr, area[i].iram_length,
                        area[j].iram_addr, area[j].iram_length)) {
                    /* collided */
                    IRAM_TRACE(("[IRAM_Init] avail area collided"));    
                    IRAM_ASSERT(0 && "avail area collided");/*assert verified*/
                    collided = TRUE;  /*lint !e527 */
                } else {
                    /* OK, as attri is not used, it is used to store final
                     * position of the item in the sorted list.
                     */
                    if (area[i].iram_addr < area[j].iram_addr)
                        g_iram_avail_area[j].attri++;
                    else
                        g_iram_avail_area[i].attri++;
                }         
            }
        }
        
        if (collided)
            break;     
        
        for (i = 0; i < count; ++i) {
        	g_iram_avail_area[g_iram_avail_area[i].attri].iram_addr = area[i].iram_addr;
        	g_iram_avail_area[g_iram_avail_area[i].attri].iram_length = area[i].iram_length;
       	}

#if IRAM_TRACE_ON
		IRAM_TRACE(("[IRAM_Init] Sorted list:"));
		for (i = 0; i < count; ++i) 
			IRAM_TRACE(("(%d): iram_addr = 0x%x, iram_length = %d", 
				i, g_iram_avail_area[i].iram_addr, g_iram_avail_area[i].iram_length));
#endif /* IRAM_TRACE_ON */       	   
#else /* ENABLE_RANGE_CHECK */
        /* just ignore area & count */
        (void)area;
        (void)count;
#endif /* ENABLE_RANGE_CHECK */
            
        return TRUE;
    } while (0);
    
#if ENABLE_RANGE_CHECK    
    g_iram_avail_area_count = 0;
    if (g_iram_avail_area) {
        SCI_FREE(g_iram_avail_area);
        g_iram_avail_area = NULL;
    }
#endif /* ENABLE_RANGE_CHECK */    
    
    if (SCI_NULL != g_iram_event) {
        SCI_DeleteEvent(g_iram_mutex);
        g_iram_event = SCI_NULL;
    }
    
    if (SCI_NULL != g_iram_mutex) {
        SCI_DeleteMutex(g_iram_mutex);
        g_iram_mutex = SCI_NULL;
    }
    
    return FALSE;
}

/*****************************************************************************/
//  Description:    The function is used to print iram information
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
void IRAM_PrintiramInfo(void)
{
    char  str[100];
    LIST_NODE *list;    
/* Find out who we are! */
    list = g_iram_occupied_area.next;
    if(list == &g_iram_occupied_area)
    {
           sprintf(str,"IRAM no area locked,g_iram_occupied_area = 0x%x, g_iram_occupied_area.next = 0x%x",
		   	((uint32)list),((uint32)list->next));
	    OS_DumpPrint(str, strlen(str));	
    }
	
    while (list != &g_iram_occupied_area)
     {
        AREA_NODE *node;
        node = LIST_ENTRY(list, AREA_NODE, occupied_list);/*lint !e413 */   
		
        sprintf(str, "[_iram_locked__area] (TASK ID = 0x%8x, start_addr = 0x%8x, length = 0x%8x)", 
            ((uint32)node->task_id),((uint32)node->start_addr), ((uint32)node->length ));/*lint !e515 -e515*/
        OS_DumpPrint(str, strlen(str));		
        list = list->next;
    }
}

ICLIENT_HANDLE IRAM_RegisterArea(const IRAM_AREA *area, int32 count)
{
    IRAM_CLIENT *client;
    IRAM_TRACE(("[IRAM_RegisterArea] IN (area = 0x%x, count = %d)", area, count));
    
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    if (!area || (count <= 0))  /*lint !e774 */
        return IRAM_INVALID_HANDLE;
    
    if (!_iram_lock()) {
        IRAM_TRACE(("[IRAM_RegisterArea] lock failed"));
        return IRAM_INVALID_HANDLE;
    }
    
    client = _iram_create_client(area, count, FALSE);
    _iram_unlock();
     
    return client;
}

BOOLEAN IRAM_UnregisterArea(ICLIENT_HANDLE client)
{
    IRAM_CLIENT *cl = client;
    BOOLEAN     ret;
    
    IRAM_TRACE(("[IRAM_UnregisterArea] IN (client = 0x%x)", client));
        
    IRAM_ASSERT(cl);/*assert verified*/
    
    IRAM_TRACE(("[IRAM_UnregisterArea] magic = 0x%x, state = %d",
        cl->magic, cl->state));
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    
    if (!cl || (CLIENT_MAGIC != cl->magic)) /*lint !e774 */
        return FALSE;
        
    if (!_iram_lock()) {
        IRAM_TRACE(("[IRAM_UnregisterArea] lock failed"));
        return FALSE;
    }
    
    IRAM_ASSERT(CLIENT_IDLE == cl->state);/*assert verified*/
    if (CLIENT_IDLE == cl->state) {
        _iram_destroy_client(cl);
        ret = TRUE;
    } else {
        ret = FALSE;
    }
    
    _iram_unlock();
    
    return ret;      
}

ILOCK_HANDLE IRAM_LockArea(const IRAM_AREA *area, int32 count, uint32 wait)
{
    IRAM_CLIENT *cl;
    IRAM_LOCK   *lock;
    
    IRAM_TRACE(("[IRAM_LockArea] IN (area = 0x%x, count = %d, wait = 0x%x, thread = 0x%x)",
        area, count, wait, SCI_IdentifyThread()));
        
    IRAM_ASSERT(area);/*assert verified*/
    IRAM_ASSERT(count > 0);/*assert verified*/
    if (!area || (count <= 0))  /*lint !e774 */
        return IRAM_INVALID_HANDLE;
    
    if (!_iram_lock()) {
        IRAM_TRACE(("[IRAM_LockArea] lock failed"));
        return IRAM_INVALID_HANDLE;
    }
    
    cl = _iram_create_client(area, count, TRUE);
    if (!cl) {
        IRAM_TRACE(("[IRAM_LockArea] create client failed"));
        _iram_unlock();
        return IRAM_INVALID_HANDLE;
    }
    
    lock = _iram_alloc_lock();
    if (!lock) {
        IRAM_TRACE(("[IRAM_LockArea] alloc lock failed"));
        _iram_destroy_client(cl);
        _iram_unlock();
        return IRAM_INVALID_HANDLE;
    }
    
    lock->owner = cl;
    
    if (!_iram_lock_area(cl, wait)) {
        IRAM_TRACE(("[IRAM_LockArea] lock area failed"));
        
        _iram_free_lock(lock);
        lock = NULL;
        _iram_destroy_client(cl);
    }
        
    _iram_unlock();
    return lock;    
}

ILOCK_HANDLE IRAM_LockRegisteredArea(ICLIENT_HANDLE client, uint32 wait, BOOLEAN forceZI)
{
    IRAM_CLIENT *cl = client;
    IRAM_LOCK   *lock;
    
    IRAM_TRACE(("[IRAM_LockRegisteredArea] IN (client = 0x%x, wait = 0x%x, forceZI = %d, thread = 0x%x)",
        client, wait, forceZI, SCI_IdentifyThread()));
    
    IRAM_ASSERT(cl);/*assert verified*/
    if (!cl)  /*lint !e774 */
        return IRAM_INVALID_HANDLE;
    
    if (!_iram_lock()) {
        IRAM_TRACE(("[IRAM_LockRegisteredArea] lock failed"));
        return IRAM_INVALID_HANDLE;
    }
    
    IRAM_TRACE(("[IRAM_LockRegisteredArea] magic = 0x%x", cl->magic));
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    
    if (CLIENT_MAGIC != cl->magic) {
        _iram_unlock();
        return IRAM_INVALID_HANDLE;
    }
    
    lock = _iram_alloc_lock();
    if (!lock) {
        IRAM_TRACE(("[IRAM_LockRegisteredArea] alloc lock failed"));
        _iram_unlock();
        return IRAM_INVALID_HANDLE;
    }
    
    lock->owner = cl; 
    
    cl->forceZI = forceZI;
    if (!_iram_lock_area(cl, wait)) {
        IRAM_TRACE(("[IRAM_LockRegisteredArea] _iram_lock_area failed"));
        _iram_free_lock(lock);
        lock = NULL;
    }
    
    _iram_unlock();
    return lock;
}

BOOLEAN IRAM_UnlockArea(ILOCK_HANDLE lock)
{
    IRAM_LOCK   *lk = lock;
    IRAM_CLIENT *cl;
    BOOLEAN     ret;
    
    IRAM_TRACE(("[IRAM_UnlockArea] IN (lock = 0x%x, thread = 0x%x)", 
        lock, SCI_IdentifyThread()));
    
    IRAM_ASSERT(lk);/*assert verified*/
    IRAM_ASSERT(LOCK_MAGIC == lk->magic);/*assert verified*/
    if (!lk || (LOCK_MAGIC != lk->magic))  /*lint !e774 */
        return FALSE;
    
    cl = lk->owner;
    IRAM_ASSERT(cl);/*assert verified*/
    IRAM_ASSERT(CLIENT_MAGIC == cl->magic);/*assert verified*/
    if (!cl || (CLIENT_MAGIC != cl->magic))  /*lint !e774 */
        return FALSE;
        
    if (!_iram_lock()) {
        IRAM_TRACE(("[IRAM_UnlockArea] get lock failed"));
        return FALSE;
    }
        
    if (!_iram_unlock_area(cl)) {
        IRAM_TRACE(("[IRAM_UnlockArea] _iram_unlock_area failed"));
        ret = FALSE;
    } else {
        IRAM_TRACE(("[IRAM_UnlockArea] area is unlocked"));
        
        _iram_free_lock(lk);
        
        /* destroy temporary client */
        if (cl->temporary) {
            IRAM_TRACE(("[IRAM_UnlockArea] temporary client, destroy it"));
            _iram_destroy_client(cl);
        }
            
        ret = TRUE;
    }
    
    _iram_unlock();
    return ret;
} 

/*-----------------------------------------------------------------------------
 * Unit Test facility
 */
#if IRAM_UNIT_TEST 
 
/* You can register HandleTestIram to diag and test iram mgr module with tools
 * on PC.
 */
#define MAX_AREA_NUM        4

typedef struct _IRAM_CMD   
{
    uint32  seq_num;      // Message sequence number, used for flow control
	uint16  len;          // The totoal size of the packet "sizeof(MSG_HEAD_T)
                          // + packet size"
	uint8   type;         // Main command type
	uint8   subtype;      // Sub command type
	uint8   thread;
	uint8   nrof_area;
	uint32  wait;
	struct
	{
	    uint32 addr;
	    uint32 length;
	    uint32 attri;
	} area[MAX_AREA_NUM];
} IRAM_CMD;

typedef struct _IRAM_SIGNAL
{
    uint16          SignalCode;
    uint16          SignalSize;
    xSignalHeader   Pre;
    xSignalHeader   Suc;
    BLOCK_ID        Sender;
    uint8           nrof_area;
    uint32          wait;
    BOOLEAN         forceZI;
	struct
	{
	    uint32 addr;
	    uint32 length;
	    uint32 attri;
	} area[MAX_AREA_NUM];
} IRAM_SIGNAL;

#define IRAM_CMD_INIT           0
#define IRAM_CMD_REGISTER       1
#define IRAM_CMD_UNREGISTER     2
#define IRAM_CMD_LOCK           3
#define IRAM_CMD_UNLOCK         4

unsigned char *g_iram = 0;

BLOCK_ID test_thread[3];

void send_iram_signal(IRAM_CMD *cmd)
{
    IRAM_SIGNAL *signal = SCI_ALLOC_APP(sizeof(*signal));
    int i;
    
    signal->SignalCode = cmd->subtype;
    signal->SignalSize = sizeof(*signal);
    signal->Sender = SCI_IdentifyThread();
    signal->nrof_area = cmd->nrof_area;
    signal->wait = cmd->wait;
    for (i = 0; i < cmd->nrof_area; ++i) {
        signal->area[i].addr = (uint32)g_iram + cmd->area[i].addr;
        signal->area[i].length = cmd->area[i].length;
        signal->area[i].attri = cmd->area[i].attri;
    }
    
    SCI_SendSignal((xSignalHeader)signal, test_thread[cmd->thread]);
}

void iram_tester(uint32 argc, void*argv)
{
    BLOCK_ID block_id = SCI_IdentifyThread();
    /* unsigned char *data[MAX_AREA_NUM] = {0, 0, 0, 0}; */
    int i;
    ICLIENT_HANDLE client;
    ILOCK_HANDLE lock;   
    BOOLEAN ret;
    
    while (1) 
    {
        IRAM_SIGNAL *signal = (IRAM_SIGNAL *)SCI_GetSignal(block_id);
        //SCI_TraceLow:"[iram_thread] (%d), SignalCode = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1155_112_2_18_1_5_24_35,(uint8*)"dd", argc, signal->SignalCode);
            
        switch (signal->SignalCode)
        {
            case IRAM_CMD_REGISTER:
            {
                IRAM_AREA area[MAX_AREA_NUM];
                
                for (i = 0; i < signal->nrof_area; ++i) {
                    area[i].iram_addr = signal->area[i].addr;
                    area[i].iram_length = signal->area[i].length;
                    area[i].data = 0;
                    area[i].attri = signal->area[i].attri;
                }
                
                client = IRAM_RegisterArea(area, signal->nrof_area);
                SCI_ASSERT(client);/*assert verified*/
                //SCI_TraceLow:"[iram_thread:%d] area registered"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1172_112_2_18_1_5_24_36,(uint8*)"d", argc);
                
                break;
            }
            
            case IRAM_CMD_UNREGISTER:
            {
                ret = IRAM_UnregisterArea(client);
                SCI_ASSERT(ret);/*assert verified*/
                //SCI_TraceLow:"[iram_thread:%d] area unregistered"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1181_112_2_18_1_5_24_37,(uint8*)"d", argc);
                break;
            }
            
            case IRAM_CMD_LOCK:
            {
                if (0 == signal->nrof_area) {
                    lock = IRAM_LockRegisteredArea(client, signal->wait, signal->forceZI);
                } else {
                    IRAM_AREA area[MAX_AREA_NUM];
                    
                    for (i = 0; i < signal->nrof_area; ++i) {
                        area[i].iram_addr = signal->area[i].addr;
                        area[i].iram_length = signal->area[i].length;
                        area[i].data = 0;
                        area[i].attri = signal->area[i].attri;
                    }
                    
                    lock = IRAM_LockArea(area, signal->nrof_area, signal->wait);
                }
                
                //SCI_TraceLow:"[iram_thread:%d] lock area(lock = 0x%x)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1202_112_2_18_1_5_24_38,(uint8*)"dd", argc, lock);
                break;
            }
            
            case IRAM_CMD_UNLOCK:
            {
                ret = IRAM_UnlockArea(lock);
                //SCI_TraceLow:"[iram_thread:%d] unlock area"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1209_112_2_18_1_5_24_39,(uint8*)"d", argc);
                SCI_ASSERT(ret);/*assert verified*/
                break;
            }
            
            default:
                //SCI_ASSERT(0);
                //SCI_TraceLow:"[iram_thread:%d] <WARNING> NOT support SignalCode"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1216_112_2_18_1_5_24_40,(uint8*)"d", argc);
                break;
        }
        
        SCI_FREE(signal);
    }
}



void HandleTestIram(const uint8 *src_ptr)
{
    IRAM_CMD *cmd = (IRAM_CMD *)src_ptr;
    BOOLEAN ret;
    
    //SCI_TraceLow:"[HandleTestIram] subtype = %d, thread = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1232_112_2_18_1_5_24_41,(uint8*)"dd", cmd->subtype, cmd->thread);
    
    switch (cmd->subtype)
    {
        case IRAM_CMD_INIT:
        {
            IRAM_AREA area[MAX_AREA_NUM];
            int i;
            
            g_iram = SCI_ALLOC_APP(20 * 1024);
            //SCI_TraceLow:"[HandleTestIram] g_iram = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,IRAM_MGR_1242_112_2_18_1_5_24_42,(uint8*)"d", g_iram);
            
            for (i = 0; i < cmd->nrof_area; ++i) {
                area[i].iram_addr = cmd->area[i].addr + (uint32)g_iram;
                area[i].iram_length = cmd->area[i].length;
            }
            
            ret = IRAM_Init(area, cmd->nrof_area);
            SCI_ASSERT(ret);/*assert verified*/
            
            for (i = 0; i < 3; ++i) {
                test_thread[i] = SCI_CreateThread("iram tester", "iram tester queue",
                     iram_tester, i, 0, 2 * 1024, 20, TASK_IRAM_TESER_PRI, TRUE, TRUE);
            }
            break;
        }
        
        default:
            send_iram_signal(cmd);
            break;
    }    
}

#endif /* IRAM_UNIT_TEST */

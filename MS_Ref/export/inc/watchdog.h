#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _LIST_HEAD 
{
	struct _LIST_HEAD *prev, *next;
} LIST_HEAD, *PLIST_HEAD;

static __inline void INIT_LIST_HEAD(LIST_HEAD *p)
{
	p->prev = p; p->next = p;
}

static __inline void LIST_ADD(LIST_HEAD *item, LIST_HEAD *head)
{
	LIST_HEAD *next = head->next;
	next->prev = item;
	item->next = next;
	item->prev = head;
	head->next = item;
}

static __inline void LIST_DEL(LIST_HEAD *p)
{
	p->next->prev = p->prev;
	p->prev->next = p->next;
}

static __inline BOOLEAN LIST_IS_EMPTY(LIST_HEAD *head)
{
	return (head->next == head);
}

#define LIST_ENTRY(p, type, member) \
	((type*)((char*)p - (unsigned long)(&((type*)0)->member)))
	
/******************************************************************************
 * Structure definitions
 ******************************************************************************/	
typedef struct _SWDG_OBJECT 
{
	const char* 	task_name;
	BOOLEAN 	task_status;
	uint32 		fresh_tick;
	uint32 		time_out;
	LIST_HEAD 	next;			 /* List of soft watchdog object */ 
} SWDG_OBJECT, *PSWDG_OBJECT;

/******************************************************************************
 * WatchDogTimerInit
 ******************************************************************************/
PUBLIC void WatchDogTimerInit(void);

/******************************************************************************
 * SWDG_RegTask
 ******************************************************************************/
PUBLIC SWDG_OBJECT* SWDG_RegTask(const char* name, uint32 time_out);

/******************************************************************************
 * SWDG_UnregTask
 ******************************************************************************/
PUBLIC void SWDG_UnregTask(SWDG_OBJECT* p);

/******************************************************************************
 * SWDG_SetStatus
 ******************************************************************************/
PUBLIC void SWDG_SetStatus(SWDG_OBJECT* p, BOOLEAN on);

/******************************************************************************
 * SWDG_FreshTick
 ******************************************************************************/
PUBLIC void SWDG_FreshTick(SWDG_OBJECT* p);

#ifdef __cplusplus
}
#endif

#endif /* WATCHDOG_H */

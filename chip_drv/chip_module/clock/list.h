/******************************************************************************
 ** File Name:      list.h                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

#include "sci_types.h"

typedef struct _LIST_HEAD
{
    struct _LIST_HEAD *prev, *next;
} LIST_HEAD, *PLIST_HEAD;

static __inline void INIT_LIST_HEAD (LIST_HEAD *p)
{
    p->prev = p;
    p->next = p;
}

static __inline void LIST_ADD (LIST_HEAD *item, LIST_HEAD *head)
{
    LIST_HEAD *next = head->next;
    next->prev = item;
    item->next = next;
    item->prev = head;
    head->next = item;
}

static __inline void LIST_DEL (LIST_HEAD *p)
{
    p->next->prev = p->prev;
    p->prev->next = p->next;
}

static __inline BOOLEAN LIST_IS_EMPTY (LIST_HEAD *head)
{
    return (head->next == head);
}

#define LIST_ENTRY(p, type, member) \
    ((type*)((char*)p - (unsigned long)(&((type*)0)->member)))

#endif /* LIST_H */


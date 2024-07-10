#ifndef _JS_LIST_H_
#define _JS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member, membertype) ( { \
const membertype  *__mptr = (ptr); \
(type *)( (char *)__mptr - _offsetof(type,member) ); } )

static inline void prefetch(const void *x) {;}
static inline void prefetchw(const void *x) {;}

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new_head,
            struct list_head *prev, struct list_head *next)
{
    next->prev = new_head;
    new_head->next = next;
    new_head->prev = prev;
    prev->next = new_head;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new_head, struct list_head *head)
{
    __list_add(new_head, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new_head, struct list_head *head)
{
    __list_add(new_head, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_remove(struct list_head *item)
{
    item->next->prev = item->prev;
    item->prev->next = item->next;
}

static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

static inline int list_empty_careful(const struct list_head *head)
{
    struct list_head *next = head->next;
    return (next == head) && (next == head->prev);
}

static inline void __list_splice(struct list_head *list,
        struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
        struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

#define list_entry(ptr, type, member, member_type) container_of(ptr, type, member, member_type)


#define list_for_each(pos, head) \
for (pos = (head)->next; prefetch(pos->next), pos != (head); \
            pos = pos->next)

#ifdef __cplusplus
}
#endif

#endif
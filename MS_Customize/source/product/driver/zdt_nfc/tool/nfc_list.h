#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
    struct list_head *next, *prev;//下，上
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(uint16_t)(&((type *)0)->member))) //获取此项的结构

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#define list_get_head(ptr) list_first_entry(ptr, struct buf_entry, list)

#define list_last_entry(ptr, type, member) \
    list_entry((ptr)->prev, type, member)
/**
 * list_for_each    -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next;  pos != (head); \
        pos = pos->next)

/**
 * list_for_each_prev   -   iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev;  pos != (head); \
        pos = pos->prev)

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, type, head, member)                \
    for (pos = list_entry((head)->next, type, member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, type, member))


/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, type, n, head, member)            \
    for (pos = list_entry((head)->next, type, member),  \
        n = list_entry(pos->member.next, type, member); \
         &pos->member != (head);                    \
         pos = n, n = list_entry(n->member.next, type, member))

int list_empty(struct list_head *head);
void list_del(struct list_head *entry);
void list_add_tail(struct list_head *new, struct list_head *head);
void list_move_tail(struct list_head *list, struct list_head *head);

#ifdef __cplusplus
}
#endif
#endif /* LIST_H */

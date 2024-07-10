/**
 *****************************************************************************
 * @file     jee_list.h
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 **/

#ifndef JEE_LIST_H
#define JEE_LIST_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/*
#define LIST_POSITION1 ((void*)0x00100100)
#define LIST_POSITION2 ((void*)0x00200200)
*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef struct _list_node_t
{
    struct _list_node_t *prev;
    struct _list_node_t *next;
} list_node;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

static __inline void init_listnode_head(list_node *p)
{
    p->prev = p;
    p->next = p;
}

static __inline void __listnode_add(list_node *new_node, list_node *prev,
                                    list_node *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static __inline void listnode_add_tail(list_node *new_node, list_node *head)
{
    __listnode_add(new_node, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void listnode_del(list_node *p)
{
    p->next->prev = p->prev;
    p->prev->next = p->next;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline int listnode_empty(list_node *head)
{
    return head->next == head;
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define listnode_entry(p, type, member)                                        \
    ((type *)((char *)p                                                        \
              - (unsigned long)(&((type *)0)->member))) /*lint -e(413) */

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define listnode_for_each(pos, head)                                           \
    for (pos = (head)->next; pos != (head); pos = pos->next)

// clone list
#define listhead_clone(src_head, dst_head, type, member)                       \
    do                                                                         \
    {                                                                          \
        list_node *_p_src = src_head;                                          \
        list_node *_p_dst = dst_head;                                          \
        if (_p_src && _p_dst)                                                  \
        {                                                                      \
            list_node *_find_node = NULL;                                      \
            init_listnode_head(_p_dst);                                        \
            listnode_for_each(_find_node, _p_src)                              \
            {                                                                  \
                type *_src = listnode_entry(_find_node, type, member);         \
                type *_dst = (type *)SCI_ALLOC_APP(sizeof(type));              \
                if (_dst && _src)                                              \
                {                                                              \
                    SCI_MEMCPY((void *)_dst, _src,                             \
                               sizeof(type)); /*lint !e718 -e628*/             \
                    listnode_add_tail(&(_dst->member), _p_dst);                \
                }                                                              \
            }                                                                  \
        }                                                                      \
    } while (0);

// clean list
#define listhead_clean(head, type, member)                                     \
    do                                                                         \
    {                                                                          \
        list_node *_list_head = head;                                          \
        if (_list_head)                                                        \
        {                                                                      \
            list_node *_find_node = _list_head->next;                          \
            while (_find_node != _list_head)/*bug2038346*/                      \
            {                                                                  \
                type *_node_info = listnode_entry(_find_node, type, member);   \
                listnode_del(_find_node);                                      \
                _find_node = _find_node->next;                                 \
                SCI_FREE(_node_info);                                          \
                _node_info = NULL;                                             \
            }                                                                  \
            SCI_FREE(_list_head);                                              \
            _list_head = NULL;                                                 \
        }                                                                      \
    } while (0);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End
#pragma once
#ifndef __LINK_LIST_H
#define __LINK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)  &&  _MSC_VER >= 1200
#include <stdlib.h>
#endif

// Library version

#define LIST_VERSION "0.0.6"

// Memory management macros
// #ifndef LINK_LIST_MALLOC
// #define LINK_LIST_MALLOC os_malloc
// #endif
// 
// #ifndef LINK_LIST_FREE
// #define LINK_LIST_FREE os_free
// #endif

/*
 * link list iterator direction.
 */

typedef enum {
    LINK_LIST_HEAD
  , LINK_LIST_TAIL
} link_list_direction_t;

/*
 * list_t node struct.
 */

typedef struct list_node {
  struct list_node *prev;
  struct list_node *next;
  void *val;
} link_list_node_t;

/*
 * list_t struct.
 */

typedef struct {
  link_list_node_t *head;
  link_list_node_t *tail;
  unsigned int len;
  void (*freeCallback)(void *val);
  int (*match)(void *a, void *b);
} link_list_t;

/*
 * list_t iterator struct.
 */

typedef struct {
	link_list_node_t *next;
	link_list_direction_t direction;
} link_list_iterator_t;

// Node prototypes.

link_list_node_t *
link_list_node_new(void *val);

// link_list_t prototypes.

link_list_t *
link_list_new(void);

link_list_node_t *
link_list_rpush(link_list_t *self, link_list_node_t *node);

link_list_node_t *
link_list_lpush(link_list_t *self, link_list_node_t *node);

link_list_node_t *
link_list_insert_at(link_list_t *self, link_list_node_t *node, int index);

link_list_node_t *
link_list_find(link_list_t *self, void *val);

link_list_node_t *
link_list_at(link_list_t *self, int index);

link_list_node_t *
link_list_rpop(link_list_t *self);

link_list_node_t *
link_list_lpop(link_list_t *self);

void
link_list_remove(link_list_t *self, link_list_node_t *node);

void
link_list_destroy(link_list_t *self);

// list_t iterator prototypes.

link_list_iterator_t *
list_iterator_new(link_list_t *list, link_list_direction_t direction);

link_list_iterator_t *
list_iterator_new_from_node(link_list_node_t *node, link_list_direction_t direction);

link_list_node_t *
list_iterator_next(link_list_iterator_t *self);

void
list_iterator_destroy(link_list_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __LINK_LIST_H */


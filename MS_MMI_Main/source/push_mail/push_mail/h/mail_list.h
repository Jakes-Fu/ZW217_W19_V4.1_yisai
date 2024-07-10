/******************************************************************************
 ** File Name:      mail_list.h                                               *
 ** Author:         lixu                                                   *
 ** DATE:           2010/05/01                                               *
 ** Copyright:               *
 ** Description:                                                              *
 ******************************************************************************/

#ifndef _mail_list_h
#define _mail_list_h

#include "comm_platform.h"

struct mail_list_node_st 
{
  struct mail_list_node_st* next;
  struct mail_list_node_st* prev;
};
typedef struct mail_list_node_st mail_list_node_t;

struct mail_list_st 
{
  struct mail_list_node_st* head;
  struct mail_list_node_st* tail;
};
typedef struct mail_list_st mail_list_t;


mail_list_t* mail_list_create (comm_uint8 mod_id);

void mail_list_destroy (comm_uint8 mod_id, mail_list_t* list);

void mail_list_add_first (mail_list_t* list, mail_list_node_t* node);

void mail_list_add_last (mail_list_t* list, mail_list_node_t* node);

void mail_list_insert_after (mail_list_t* list,
                       mail_list_node_t* nodeToInsert,
                       mail_list_node_t* predecessor);

mail_list_node_t* mail_list_remove (mail_list_t* list, mail_list_node_t* node);

mail_list_node_t *mail_list_pop_first (mail_list_t* list);


#define mail_LIST_IS_EMPTY(m_list) ((m_list)->head == NULL)

mail_list_node_t* mail_list_get_next (const mail_list_node_t* node);


mail_list_node_t* mail_list_get_prev (const mail_list_node_t* node);


mail_list_node_t*mail_list_get_head (const mail_list_t* list);

#define mail_list_get_first mail_list_get_head

mail_list_node_t*
mail_list_get_tail (const mail_list_t* list);

#define mail_list_get_last mail_list_get_tail

size_t
mail_list_get_size (const mail_list_t* list);



#endif 


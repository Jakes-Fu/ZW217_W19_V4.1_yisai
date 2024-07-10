/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_LIST_HEAD_H__
#define __AVC_LIST_HEAD_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct avc_list_head_t {
  struct avc_list_head_t *next, *prev;
} avc_list_head_t;

#define __AVC_LIST_ADD(entry,before,after) {avc_list_head_t *new_= (entry), \
                                            *prev = (before), *next = (after); (next)->prev = (new_); \
                                            (new_)->next = (next); (new_)->prev = (prev); (prev)->next = (new_);}
#define avc_list_init(entry)               do {(entry)->next = (entry); (entry)->prev = (entry);}  while(0)
#define avc_list_add(entry,base)           do {__AVC_LIST_ADD((entry), (base), (base)->next);} while(0)
#define avc_list_add_after(entry,base)     do {__AVC_LIST_ADD((entry), (base), (base)->next);} while(0)
#define avc_list_add_before(entry,base)    do {__AVC_LIST_ADD((entry), (base)->prev, (base));} while(0)
#define avc_list_add_head(entry,head)      avc_list_add_after(entry,head)
#define avc_list_add_tail(entry,head)      avc_list_add_before(entry,head)
#define avc_list_del(entry)                do {(entry)->prev->next = (entry)->next; \
                                               (entry)->next->prev = (entry)->prev; \
                                               (entry)->next = (entry)->prev = (entry);} while(0)
#define avc_list_empty(head)               ((head)->next == (head))
#define avc_list_get_head(head)            (avc_list_empty(head) ? (avc_list_head_t*)NULL : (head)->next)
#define avc_list_get_tail(head)            (avc_list_empty(head) ? (avc_list_head_t*)NULL : (head)->prev)
#define avc_list_is_head(entry, head)      ((entry)->prev == head)
#define avc_list_is_tail(entry, head)      ((entry)->next == head)
#define avc_list_entry(ptr, type, member)  ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#define avc_list_for_each(h, head)         for (h = (head)->next; h != (head); h = h->next)
#define avc_list_for_each_safe(h, n, head) for (h = (head)->next, n = h->next; h != (head);  h = n, n = h->next)
#define avc_list_for_each_entry(p, head, type, member) for (p = avc_list_entry((head)->next, type, member); \
                                                             &p->member != (head); p = avc_list_entry(p->member.next, type, member))
#define avc_list_for_each_entry_safe(p, t, head, type, member) for (p = avc_list_entry((head)->next, type, member), \
                                                                    t = avc_list_entry(p->member.next, type, member); \
                                                                    &p->member != (head); p = t, t = avc_list_entry(t->member.next, type, member))
#define avc_list_get_head_entry(head, type, member) (avc_list_empty(head) ? (type*)NULL : avc_list_entry(((head)->next), type, member))
#define avc_list_get_tail_entry(head, type, member) (avc_list_empty(head) ? (type*)NULL : avc_list_entry(((head)->prev), type, member))

#ifdef __cplusplus
}
#endif
#endif /* __AVC_LIST_HEAD_H__ */

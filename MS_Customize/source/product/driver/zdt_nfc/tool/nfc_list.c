#include "nfc_list.h"

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static  void __list_add(struct list_head *elem,struct list_head *prev,struct list_head *next) {//在两个已知的连续条目之间插入新条目
    next->prev = elem;//A-->B-->C-->A
    elem->next = next;//A<--B<--C<--A
    elem->prev = prev;
    prev->next = elem;
}

#if 0
/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static  void list_add(struct list_head *elem, struct list_head *head) {
    __list_add(elem, head, head->next);
}
#endif
/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(struct list_head *elem, struct list_head *head) {       //尾部添加条目
    __list_add(elem, head->prev, head);
}

//struct buf_entry * list_get_head(struct list_head *entry) {                  //头部获取条目
//    return list_first_entry(entry,struct buf_entry, list);
//}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static  void __list_del(struct list_head *prev, struct list_head *next) { //通过创建上一个/下一个条目来删除列表条目
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
void list_del(struct list_head *entry) { //要从列表中删除的元素
    __list_del(entry->prev, entry->next);
    entry->prev = entry;
    entry->next = entry;
}

#if 0
/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static  void list_del_init(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static  void list_replace(struct list_head *old,
                          struct list_head *elem) {
    elem->next = old->next;
    elem->next->prev = elem;
    elem->prev = old->prev;
    elem->prev->next = elem;
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static  void list_move(struct list_head *list, struct list_head *head) {
    __list_del(list->prev, list->next);
    list_add(list, head);
}
#endif

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that our entry will followed
 */
void list_move_tail(struct list_head *list, struct list_head *head) { //从一个列表中删除并添加为另一个列表的尾部
    __list_del(list->prev, list->next);                                //从连中删除
    list_add_tail(list, head);                                          //还一个空间
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(struct list_head *head) {
    return head->next == head;
}

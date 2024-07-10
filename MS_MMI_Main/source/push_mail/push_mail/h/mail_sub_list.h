/******************************************************************************
 ** File Name:      mail_sub_list.h                                               *
 ** Author:         lixu                                                   *
 ** DATE:           2010/05/01                                               *
 ** Copyright:               *
 ** Description:                                                              *
 ******************************************************************************/

#ifndef _MCL_LIST_H_
#define _MCL_LIST_H_

#include "comm_platform.h"

/******************************************************************************
 * Types
 ******************************************************************************/

/*! \typedef MclList
 *  \brief A generic list. Used as a handle to identify the list.
 */
typedef struct MclListImp MclList;

/*! \typedef MclListIterator
 *  \brief Basically a pointer to a node, used to loop on the elements in the list
 */
typedef struct MclListIteratorImp MclListIterator;

/*! \typedef mcl_list_element_cvt_fp
** \brief Function pointer prototype for list element convert functions.
**/
//typedef int mcl_list_element_cvt_fp(msf_dcvt_t *obj, void **list_element);

/******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!\brief Creates a new double linked list and returns a handle to the list
 * \param modId Module ID, used for memory allocation
 * \return Handle to the list
 *
 * Linked list characteristics:
 * A double linked list is efficient to use when the number of add and remove
 * operations are many. If the list is to be used in a more create once and
 * read many, then an array based list would be to prefer.
 * A linked list generates malloc/free for every add/remove but remove/insert
 * operations are economical. Random access into list is expensive.
 *
 * Array list characteristics:
 * While an array list generates few malloc/free but may use redundant memory
 * and remove/insert operations on lists with many element are expensive.
 * Any access function random or iterative is efficient.
 ******************************************************************************/
MclList* mclLinkedListCreate(comm_uint8 modId);

/*!\brief Destroys the list. Do not use the list handle after this call. If the
 * list is not empty, the list will be emptied and all memory allocated by the
 * list internally will be freed. Note that the actual user data is not freed.
 * It is the responsibility of the user the free the user data.
 * \param modId Module ID, used for memory allocation
 * \param list handle to the list
 ******************************************************************************/
void mclListDestroy(MclList* list);

/*!\brief Adds an element first in the list
 * \param list handle to the list
 * \param data User data to put in the list
 ******************************************************************************/
void mclListAddFirst(MclList* list, void* data);

/*!\brief Adds an element last in the list
 * \param list handle to the list
 * \param data User data to put in the list
 ******************************************************************************/
void mclListAddLast(MclList* list, void* data);

/*!\brief Check if the list is empty
 * \param list handle to the list
 * \return TRUE if the list is empty, FALSE if it contains elements
 ******************************************************************************/
comm_boolean mclListIsEmpty(const MclList* list);

/*!\brief Gets the number of elements in the list
 * \param list handle to the list
 * \return number of elements in the list
 ******************************************************************************/
comm_uint32 mclListGetSize(const MclList *list);

/*!\brief Removes element with same data as provided as parameter
** \param list handle to the list
** \param data User data to put in the list
** \return Returns TRUE if element was removed.
*******************************************************************************/
comm_boolean mclListRemoveElement(MclList* list, void* data);

/* \brief Handles serialization of the mfp_filter_mgr_t class.
**
** \param obj Serialization class object
** \param list The list to de/serialize. NOTE: Pointer to pointer (allocates/frees instance).
** \param cvt_fp Convert function to apply on all elements in list.
**/
/*****************************************************************************/
// 	Description : 该接口在邮件本地管理模块暂不使用
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
#if 1
int MclListCvt(void);
#else
int MclListCvt(msf_dcvt_t *obj, MclList **list, mcl_list_element_cvt_fp *cvt_fp);
#endif
/****************** Positional based functions ******************/


/*!\brief Get an item based on position
 * \param list handle to the list
 * \param index the position of the item. 0 is the first item
 * \return the data in that position
 ******************************************************************************/
void *mclListGetAtPos(const MclList *list, comm_uint32 index);

/*!\brief Insert a new item before another item (based on position). If there is
 * no successive item the new item is added last
 * \param list handle to the list
 * \param index the position of the item. 0 is the first item
 * \param data user data for the element
 *
 * Note: This function should be used with case in conjunction with iterators.
 ******************************************************************************/
void mclListInsertAtPos(MclList *list, comm_uint32 index, void *data);

/*!\brief Remove an item based on position. Changed position of all following
 *  items.
 * \param list handle to the list
 * \param index the position of the item. 0 is the first item
 * \return the data of the removed item
 *
 * Note: This function should be used with case in conjunction with iterators.
 ******************************************************************************/
void* mclListRemoveAtPos(MclList* list, comm_uint32 index);

/*!\brief Changes an items data
 * \param list handle to the list
 * \param index the position of the item. 0 is the first item
 * \param newData the new data to replace
 * \return the old replaced data
 ******************************************************************************/
void *mclListSetAtPos(MclList* list, comm_uint32 index, void *newData);


/****************** Iterator functions ******************/


/*!\brief Get an iterator to use for looping the list. The iterator must be free
 * by a call to mclListDestroyIterator
 * \param list handle to the list
 * \return new iterator pointing on an imaginative slot before the first element
 ******************************************************************************/
MclListIterator* mclListGetIterator(const MclList *list);

/*!\brief Destroys a list iterator
 * \param iterator the iterator to destroy
 ******************************************************************************/
void mclListDestroyIterator(MclListIterator* iterator);

/*!\brief Gets an iterator at end of list.
 * \param list handle to the list
 * \return Iterator pointing at last element.
 ******************************************************************************/
MclListIterator* mclListGetIteratorLast(const MclList *list);

/*!\brief Get the current element based on an iterators position. Note that this
* will return 0 if there are no elements left in the list. But it could also
* return 0 if the user has entered a 0-element. Use the mclListHasNext-function
* to determine if the iterator doesn't point to an element.
* \param iterator current iterator
* \return The next user data the iterator points at.
******************************************************************************/
void* mclListGetCurrent(MclListIterator* iterator);

/*!\brief Get the next element based on an iterators position. Note that this
 * will return 0 if there are no elements left in the list. But it could also
 * return 0 if the user has entered a 0-element. Use the mclListHasNext-function
 * to determine if the iterator is at the end of the list.
 * \param iterator current iterator
 * \return The next user data the iterator points at.
 ******************************************************************************/
void* mclListGetNext(MclListIterator* iterator);

/*!\brief Used to determine if the iterator has reach the end of the list.
 * \param iterator current iterator
 * \return TRUE if there are more elements to iterate over, FALSE otherwise.
 ******************************************************************************/
comm_boolean mclListHasNext(const MclListIterator* iterator);

/*!\brief Removes the element the iterator points at. Internally the iterator is
 * set to point on the element previous to the removed.
 * \param iterator current iterator
 * \return the user data of the removed element
 ******************************************************************************/
void* mclListRemove(MclListIterator* iterator);

/*!\brief Inserts a new element after a certain element
 * \param list handle to the list
 * \param iterator pointing of that certain element.
 * \param data user data for the new element
 ******************************************************************************/
void mclListInsertAfter(MclList *list, const MclListIterator *iterator, void *data);

/*!\brief Inserts a new element before a certain element
* \param list handle to the list
* \param iterator pointing of that certain element.
* \param data user data for the new element
******************************************************************************/
void mclListInsertBefore(MclList *list, const MclListIterator *iterator, void *data);
#ifdef  MAIL_FLDR_FOR_QO7
//huangzengzhi add
void* mclListGetFirst(MclListIterator* iterator);
void* mclListGetPrev(MclListIterator* iterator);
comm_boolean mclListHasPrev(const MclListIterator* iterator);
MclListIterator* mclListGetIteratorAtPos(const MclList* list, comm_uint32 index);
#endif
#endif

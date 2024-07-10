/******************************************************************************
 ** File Name:      mmi_cache.h                                               *
 ** Author:         cheng.luo                                              *
 ** DATE:           01/20/2012                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 general cache											 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/20/2012     cheng.luo        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _MMI_CACHE_H_
#define _MMI_CACHE_H_

#include "sci_types.h"
#include "os_api.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/*cache的hash table的key类型*/
typedef enum
{
	CKT_INT	 = 0,
	CKT_STRING,
    CKT_POINTER,

	CKT_MAX
}MMI_CACHE_KEY_TYPE_E;

typedef struct  
{
    uint32          key_id;
    
    uint32          reserve;
    uint8*          data1_ptr;
    uint32          data1_len;
    uint8*			data2_ptr;
    uint32          data2_len;
}MMI_CACHE_INT_KEY_ITEM_T;

typedef struct  
{
    char*			key_name;

    uint32          reserve;
    uint8*			data1_ptr;
    uint32          data1_len;
    uint8*	        data2_ptr;
    uint32			data2_len;   
}MMI_CACHE_STR_KEY_ITEM_T;

typedef struct  
{
    void*			key_ptr;
    uint32          key_len;
    
    uint32          reserve;
    uint8*			data1_ptr;
    uint32          data1_len;
    uint8*	        data2_ptr;
    uint32			data2_len;   
}MMI_CACHE_PTR_KEY_ITEM_T;

typedef struct MMI_CACHE_INFO_T MMI_CACHE_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*******************************************************************/
//  Interface:		MMICACHE_Init
//  Description : 	MMICACHE_Init
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_CACHE_INFO_T* MMICACHE_Init(
									   const char			*name_ptr,
									   uint32				size,
									   MMI_CACHE_KEY_TYPE_E type
									   );

/*******************************************************************/
//  Interface:		MMICACHE_Destroy
//  Description : 	MMICACHE_Destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC void MMICACHE_Destroy(
							 MMI_CACHE_INFO_T	*cache_ptr
							 );

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByInt
//  Description : 	MMICACHE_RemoveItemByInt
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByInt(
                                        MMI_CACHE_INFO_T			*cache_ptr,
                                        uint32						key_id
                                        );

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByInt
//  Description : 	is_update:if update old record￡?if TURE,?T??è?o?￡????á2?è? 
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByInt(
                                     MMI_CACHE_INFO_T					*cache_ptr,
                                     const MMI_CACHE_INT_KEY_ITEM_T	    *in_ptr,
                                     BOOLEAN                            is_update
                                     );

/*******************************************************************/
//  Interface:		MMICAHCE_GetItemByInt
//  Description : 	MMICAHCE_GetItemByInt
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByInt(
									 MMI_CACHE_INFO_T			*cache_ptr,
									 uint32						key_id,
									 MMI_CACHE_INT_KEY_ITEM_T	*out_ptr
									 );

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByString
//  Description : 	MMICACHE_RemoveItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByString(
                                           MMI_CACHE_INFO_T		*cache_ptr,
                                           const char			*key_name
                                           );

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByString
//  Description : 	MMICACHE_PutItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByString(
                                        MMI_CACHE_INFO_T					*cache_ptr,
                                        const MMI_CACHE_STR_KEY_ITEM_T	    *in_ptr,
                                        BOOLEAN                             is_update
                                        );

/*******************************************************************/
//  Interface:		MMICACHE_GetItemByString
//  Description : 	MMICACHE_GetItemByString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByString(
                                        MMI_CACHE_INFO_T		*cache_ptr,
                                        const char					*key_name,
                                        MMI_CACHE_STR_KEY_ITEM_T	*out_ptr
                                        );

/*******************************************************************/
//  Interface:		MMICACHE_RemoveItemByPtr
//  Description : 	MMICACHE_RemoveItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_RemoveItemByPtr(
                                        MMI_CACHE_INFO_T			*cache_ptr,
                                        void                        *key_ptr,
                                        uint32                      key_len
                                        );

/*******************************************************************/
//  Interface:		MMICACHE_PutItemByPtr
//  Description : 	MMICACHE_PutItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_PutItemByPtr(
                                        MMI_CACHE_INFO_T					*cache_ptr,
                                        const MMI_CACHE_PTR_KEY_ITEM_T	    *in_ptr,
                                        BOOLEAN                             is_update
                                        );

/*******************************************************************/
//  Interface:		MMICACHE_GetItemByPtr
//  Description : 	MMICACHE_GetItemByPtr
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC BOOLEAN MMICACHE_GetItemByPtr(
                                        MMI_CACHE_INFO_T		*cache_ptr,
                                        const void				*key_ptr,
                                        uint32                      key_len,
                                        MMI_CACHE_PTR_KEY_ITEM_T	*out_ptr
                                        );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif 

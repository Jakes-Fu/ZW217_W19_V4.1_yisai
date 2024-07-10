/******************************************************************************
 ** File Name:      efs_item.h                                                *
 ** Author:         lin.liu                                                   *
 ** DATE:           04/12/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes some defines for item and sector       *
 *****************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2001     lin.liu          Create.                                   *
 ******************************************************************************/

#ifndef EFS_ITEM_H
#define EFS_ITEM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "efs_base.h"
#include "efs.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/*
    item's type and max number

   +-------------------+
   | id      | type    |
   |-------------------|
   | 0 -- 13 | 14--15  |
   +-------------------+
*/

typedef uint16                   ITEM_T;

#define ITEM_MASK_TYPE           0xC000
#define ITEM_MASK_ID             0x3FFF         
        
#define EFS_INVALID_ITEM         ((ITEM_T)-1)
#define EFS_DIRTY_ITEM           0 

#define EFS_ITEM_IS_VALID(item)  \
        (    ( (item) != EFS_INVALID_ITEM ) \
          && ( (item) != EFS_DIRTY_ITEM   )    )

/*    
    For index 0 == DIRTY_ITEM, so, Start from 1 to MAX_ITEM
*/
#define ITEM_GET_ID( _item )          ( (ITEM_T)((_item) & ITEM_MASK_ID )  )
#define ITEM_GET_TYPE( _item )        ( (ITEM_T)(((_item) & ITEM_MASK_TYPE) >> 14) )

#define MAKE_ITEM( _id, _type)        (ITEM_T)( ((_id) & ITEM_MASK_ID) | ((_type) << 14) )

//
//  Item type define 
//
typedef enum EFS_ITEM_TYPE_tag
{
    ITEM_TYPE_NVITEM      = 0,    // the item is nvitem.
    ITEM_TYPE_NORMAL,             // the item that contain file data.
    ITEM_TYPE_FILE_HEADER,        // the item include file header infomation.
    ITEM_TYPE_MAX
}EFS_ITEM_TYPE_E;


typedef struct EFS_SECTOR_INFO_tag
{
    uint32  free_space;      // free space size of the sector.
    uint32  dirty_space;     // used to indicate which sector should be reclaim.
    uint16  erase_count;     // erase count of the sector.
    uint16  status;          // status.
}EFS_SECTOR_INFO_S;


typedef enum _REFRESH_STATUS {
    REFRESH_STATUS_IDLE  = 0,   			
    REFRESH_STATUS_ERASE_SPARE,      			
    REFRESH_STATUS_COPY2SPARE,      			
    REFRESH_STATUS_ERASE_NVSECTOR,
    REFRESH_STATUS_COPY2NVSECTOR,	
        #ifdef EFS_SPEED_UP
    REFRESH_STATUS_START,
    #endif
}REFRESH_STATUS_E;


#ifdef EFS_SPEED_UP
typedef enum _REFRESH_INFO {
    EFS_IN_REFRESH = 0,   
    EFS_NO_NEED_REFRESH,
    EFS_DIRECT_ERASE,
    EFS_NEED_REFRESH
}REFRESH_INFO_E;
#endif

typedef struct EFS_SPACE_INFO_tag
{
    uint32  sec_addr;
    uint32  free_space;    
    uint32  dirty_space; 
    uint32  used_space; 
}EFS_SPACE_INFO_S; 

#define	MAX_NVITEM_LENGTH		(8*1024)	// nv item max length, unit byte
#define	MAX_REFRESH_TIME		(800)	 	// refresh sector max time ,unit ms
#define	REFRESH_SLEEP_TIME  	(100)       // sleep time during refresh process,unit ms.
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

//
//  Init the item layer.
//
BOOLEAN EFS_ItemInit(void);

// 
//  Close the item layer.
//
void EFS_ItemClose(void);

// 
// Get length of the given item.
//
uint16 EFS_GetItemLength(ITEM_T   item);

//
// Get the max identifier of the given type. 
//
uint16 EFS_GetMaxItemID(
    uint16  type);

//
// Get the start address of given item.
//
uint32 EFS_ItemToAddr(ITEM_T   id);

/*
    read data from the item, and only in this item.
    return the count that has read.
    maybe the read-count less then length that caller given.
*/
uint16 EFS_ReadItem(    // count of read
    ITEM_T  item_id,    // the item's id
    uint8   *buf,
    uint16  len );

/*
    write data to the item, return the count that has writted.

    if we can't write in this item, system will allocate
    a new item,and write data to the new one.
    the new one has the same id as old one,
    after written successfully, delete the old one
*/
EFS_ERROR_E EFS_WriteItem(   // count of writted
    ITEM_T  item_id,         // item's id
    const uint8   *buf,
    uint16  len  );


/*
    write data to the item, return the count that has writted.

    if we can't write in this item, system will allocate
    a new item,and write data to the new one.
    the new one has the same id as old one,
    after written successfully, delete the old one
*/
EFS_ERROR_E EFS_WriteItemFast(   // count of writted
    ITEM_T          item_id,         // item's id
    const uint8   * buf,
    uint16          item_size,
    uint16          content_len  );

//
// Write data to the item.
// If is in erase state, wait for erase completed, then call EFS_WriteItem().
//
EFS_ERROR_E EFS_WriteItemAlways(
    ITEM_T  identifier,
    uint16  count,
    const uint8   *buf_ptr);

/*
    Try find a free item id. If can't found, return EFS_INVALID_ITEM.
*/
uint16 EFS_MallocItemID( uint32 type );


/*
    Only mark the item as dirty-item
*/
void EFS_FreeItem(ITEM_T item_id);


/*
    erasing the sector
*/
BOOLEAN EFS_EraseSector(
    uint16 sector_no);

// 
// Suspend the erasing optr.
//
BOOLEAN EFS_SuspendErase(void);

//
// Resume the erasing optr
//
BOOLEAN EFS_ResumeErase(void);

BOOLEAN EFS_mark_erase(void);

//
// Try reclaim one sector.
//
BOOLEAN EFS_TryReclaimSector(void);


uint32 EFS_get_free_normal_item_count(void);
uint32 EFS_get_used_normal_item_count(void);


uint32 EFS_RefreshIsIdle(void);
uint16 EFS_GetCurrentWriteSector(void);
uint16 EFS_GetRefreshSector(void);
REFRESH_STATUS_E EFS_GetRefreshStatus(void);
BOOLEAN EFS_SectorSpaceIsAvailable(uint16 dest_sector,uint32 new_nvitem_size);
BOOLEAN EFS_FindAvailSectorInRefreshStatus(ITEM_T  item_id, uint32 size,uint16 *find_sector);
uint32 EFS_WriteItem_in_RefreshStatus( ITEM_T item_id, uint16 len,const uint8  *buf, uint16 dest_sector );
void EFS_RefreshSector(void);
void EFS_ProbeRefreshCondition(void);
void EFS_TraceSectorSpaceInfo(uint32 sec_id);
void EFS_TraceTotalSpaceInfo(void);
void EFS_FreeItem(ITEM_T item);

#ifdef EFS_SPEED_UP
 REFRESH_INFO_E EFS_GetNeedRefreshStatus(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /*  end EFS_ITEM_H  */

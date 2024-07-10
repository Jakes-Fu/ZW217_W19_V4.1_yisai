/******************************************************************************
 ** File Name:    common.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _UBOT_DRV_H_
    #define _UBOT_DRV_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

#include "ubot_common.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define UBOT_CACHE_EMPTY  (0x0000)
#define UBOT_CACHE_READ   (0x0001)
#define UBOT_CACHE_WRITE  (0x0002)
#define UBOT_CACHE_MAX    (0x0003)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct _Ubot_Cache_st
{
    uint16 flag;            /*cache status flag, empty or used for read/write */
    uint16 index_num;       /*cache index number, maybe no use now            */
    uint32 sec_num;         /*usb mass storage sector number                  */
    uint32 policy;          /*reserved*/
}UBOT_Cache_T;

typedef struct _Ubot_Cache_Mgr_st
{
    uint8  cache_enable;
    uint8  allincache;          /*if disk num is less than cache num, then set to ture*/
    uint16 disk;                /*disk index*/
    uint32 handle;              /*handle for bsd*/
    uint32 cache_num;           /*cache number, unit is cache unit size*/
    uint32 cache_unit_size;     /*cache operation size, 512 typical*/
    struct _Ubot_Cache_st * cache_ctrl_ptr;
    uint8 * cache_data_ptr;     /*point to the allocated cache memory*/
}UBOT_Cache_Mgr_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UBOT_ReadCache (uint8* buf,uint32 len);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_ResetCache (void);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_WriteCache (uint8* buf ,uint32 len);
/*****************************************************************************/
//  Description:    Call this function to start data to host if have data in 
//                  the cache;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_SendCache (UBOT_LOCK_E lock);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_HostReset (uint8 bDirection,uint16 wValue,\
                     uint16 wIndex ,uint16 wLen);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UBOT_GetMaxLUN (uint8 bDirection,uint16 wValue,\
                     uint16 wIndex ,uint16 wLen);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC int UBOT_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC UBOT_STATES_E UBOT_DoNothing (uint32 param,void * ptr);
/*****************************************************************************/
//  Description:    Call this function to read data from cache and write them 
//                   to  BSD 
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UBOT_WriteDataToBSD (uint32 len);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UBOT_ReadDataFromBSD (uint32 len);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_DevCompaction  (uint32 id);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_StartReceiveData  (UBOT_LOCK_E lock,\
										uint32  len,\
										uint32 *buf);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_StopReceiveData  (void);

/*****************************************************************************/
//  Description: cache varibable init
//  Author:         
//  parameter:   disk_index: disk index from usb, depend on the sequence of ubot_config
//               handle    : handle for bsd operation
//               cache_num : max cache size 
//               disk_num  : max disk size
//               unit_size : the minium unit size, prefered sector size
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        called when usb plug in   
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_Init( uint32 disk_index, 
                                    uint32 handle, 
                                    uint32 cache_size,
                                    uint32 disk_size,
                                    uint32 unit_size
                                   );

/*****************************************************************************/
//  Description: cache deinit
//  Author:         
//  parameter:   
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        called when usb plug out   
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_DeInit(void);

/*****************************************************************************/
//  Description: flush one packet cache data to device
//  Author:         
//  parameter:   disk_index: disk index from usb, depend on the sequence of ubot_config
//               index_num : index in cache buffer
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        can be called at idle
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_Flush( uint32 disk_index,
                                       uint32 index_num
                                      );

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 


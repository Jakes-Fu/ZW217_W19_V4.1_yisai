/******************************************************************************
 ** File Name:    UBOT_drv.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_ubot_trc.h"
#include "ubot_drv.h"
#include "usb200.h"
#include "bsd.h"
#include "udc_api.h"
#include "ubot_config.h"
#include "ubot_storage.h"
#include "ubot_scsi_sbc.h"
//#include "XSR_Partition.h"
#include "sc_reg.h"
#include "ubot_main.h"
#include "usb_device.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
//#define UBOT_TEST_OUTPOT
//#define UBOT_CACHE_DEBUG
//#define UBOT_UNIT_TEST

#ifdef UBOT_CACHE_DEBUG
#define CACHE_TARCE(_x_)  SCI_TRACE_LOW _x_
#define CACHE_ERR_TRACE(_x_) SCI_TRACE_HIGH _x_
#define CACHE_ING_TRACE(_x_) SCI_TRACE_LOW _x_
#else
#define CACHE_TARCE(...)
#define CACHE_ERR_TRACE(...)
#define CACHE_ING_TRACE(...)
#endif

#ifdef UBOT_UNIT_TEST
#define DISK_INDEX   0
#define HANDLE_VAL   0xffffffff
#define UNIT_SIZE    512

#define CACHE_SIZE   120
#define CACHE_SIZE_1 100
#define CACHE_SIZE_2 10

#define DISK_SIZE    100

/*allloc memory to use as org disk*/
/*alloc  memory to use as des buf */
static  uint8 s_data_buf[DISK_SIZE * UNIT_SIZE];
static  uint8 s_disk_buf[DISK_SIZE * UNIT_SIZE];

//creat task to 
static BLOCK_ID s_ubot_test_task;

#endif
extern uint32 ubot_interface;
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
LOCAL BOOLEAN s_ubot_cache_created = FALSE;

LOCAL UBOT_Cache_Mgr_T * s_ubot_cache_mgr_ptr = NULL;
/*--------------------------- Global Data -----------------------------------*/
__align(4) uint8 g_UBOT_MaxLUN []={NULL,NULL,NULL,NULL};
/*--------------------------- External Data ---------------------------------*/

/*----------------------------------------------------------------------------*
**                         External Function Prototype                           *
**---------------------------------------------------------------------------*/
////extern uint32 UDC_GetConnect(void);

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
#ifdef UBOT_TEST_OUTPOT
LOCAL uint16 s_test_val1;
LOCAL uint16 s_test_val2;
LOCAL uint16 s_test_val3;
LOCAL uint16 s_test_val4;
#endif
typedef enum {
	INTERFACE_FIRST = 0x1,	//udisk only
	INTERFACE_SECOND,
	INTERFACE_THRID,	//two winusb + udisk
	INTERFACE_ERR,
} UBOT_INTERFACE_NUM_T;

LOCAL UBOT_INTERFACE_NUM_T ubot_interface_num = INTERFACE_ERR;

PUBLIC void UBOT_SetInterfaceNum(uint32 num);
LOCAL BOOLEAN UBOT_ResizeCache(uint32);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: flush cache data to device
//  Author:         
//  parameter:   disk_index: disk index from usb, depend on the sequence of ubot_config
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        called when usb plug out, only act when cache flag is UBOT_CACAH_WRITE   
/*****************************************************************************/
LOCAL BOOLEAN _Ubot_Cache_FlushAll(uint32 disk_index)
{
    uint32 i;
    uint32 max_num;
    BSD_STATUS  status;
    UBOT_Cache_Mgr_T * cache_mgr_ptr;
    UBOT_Cache_T * cache_ctrl_ptr;
    uint8*         data_ptr;

    if((disk_index > UBOT_MAX_DISK_NUM) || (NULL == s_ubot_cache_mgr_ptr))
    {
        return FALSE;
    }

    cache_mgr_ptr = s_ubot_cache_mgr_ptr + disk_index;

    max_num =  cache_mgr_ptr->cache_num;
    cache_ctrl_ptr = cache_mgr_ptr->cache_ctrl_ptr;
    data_ptr = cache_mgr_ptr->cache_data_ptr;
    if((NULL == cache_ctrl_ptr) || (NULL == data_ptr))
    {
        /*cache ctrl struct or cache data buffer is not exist*/
        return FALSE;
    }

    for(i = 0; i < max_num; i++)
    {
        if(UBOT_CACHE_WRITE == (cache_ctrl_ptr + i)->flag)
        {
#ifdef UBOT_UNIT_TEST
            SCI_MEMCPY( s_disk_buf+((cache_ctrl_ptr+i)->sec_num)*UNIT_SIZE,
                        data_ptr + i * UNIT_SIZE,
                        UNIT_SIZE
                       );
#else
            /*write back the info*/
            status = BSD_Write(cache_mgr_ptr->handle,
                      (cache_ctrl_ptr + i)->sec_num,
                      1,
                      data_ptr + i * cache_mgr_ptr->cache_unit_size);

            if(BSD_SUCCESS != status)
            {
                return FALSE;
            }
#ifdef UMEM_SUPPORT
            /*here just add some sleep temporarily,because the nor flash write is 
              so slowly,write one sector will consume 4 ms,so if we write 2M data to
              nor flash, it will consume about 20 secends, some time it will take 
              more than one minute, it may cause other lower priorty task QUEUE full.
              here just one temporary method to reslove the QUEUE FULL ASSERT.
            */
            if(i%3 ==0)
            {
                SCI_Sleep(3);                                   
            }
#endif
            
#endif
            /*set cache flag to read, update cache sector flag*/
            (cache_ctrl_ptr + i)->flag = UBOT_CACHE_READ;
        }
    }

    return TRUE;
}
LOCAL BOOLEAN _Ubot_Cache_Read(uint32 disk_index,uint8* desbuf, uint32 sector_index, uint32 sec_num)
{
    uint32 i;
    uint32 max_num;
    uint32 index_num;
    uint32 unit_size;
    BSD_STATUS  status;
    UBOT_Cache_Mgr_T * cache_mgr_ptr;
    UBOT_Cache_T * cache_ctrl_ptr;
    uint8*         data_ptr;

    if((disk_index > UBOT_MAX_DISK_NUM) || (NULL == s_ubot_cache_mgr_ptr))
    {
        return FALSE;
    }

    cache_mgr_ptr = s_ubot_cache_mgr_ptr + disk_index;

    cache_ctrl_ptr = cache_mgr_ptr->cache_ctrl_ptr;
    data_ptr = cache_mgr_ptr->cache_data_ptr;
    if((NULL == cache_ctrl_ptr) || (NULL == data_ptr))
    {
        /*cache ctrl struct or cache data buffer is not exist*/
        return FALSE;
    }

    max_num   = cache_mgr_ptr->cache_num;
    unit_size = cache_mgr_ptr->cache_unit_size;
    /*
    1.if max cache num is more than total sector num, then all operation in cache
    2. else
       2.1. if cache is empty, read from disk directly
       2.2. if cache is in use, compare the sector num with the input param,
         2.2.1 equal, read from cache
         2.2.2 else 
             1. write to disk first
             2. read from device
             3. copy to desnation buf.
             4. update cache sector number
             5. set cache flag to read

    */
    if(cache_mgr_ptr->allincache)
    {
        /*
        1.if max cache num is more than total sector num, then all operation in cache
        */    
        if((sector_index <= max_num) && (sector_index+ sec_num) <= max_num)
        {
            /*param is valid*/
            for(i = sector_index; i < (sec_num + sector_index); i++)
            {
                /*
                for all sector in cache, the cache_index is equal to sector_number
                */
                if((cache_ctrl_ptr + i)->flag == UBOT_CACHE_EMPTY)
                {
                    /*Read data form BSD to cache ;*/
#ifdef UBOT_UNIT_TEST
                    SCI_MEMCPY(data_ptr + i * UNIT_SIZE, s_disk_buf + i * UNIT_SIZE, UNIT_SIZE);
#else
                    status = BSD_Read( cache_mgr_ptr->handle,
                                       i, /*sector index*/
                                       1, /*sector_num*/
                                       data_ptr + i * unit_size
                                     );
                    if(BSD_SUCCESS != status)
                    {
                        return FALSE;
                    }
#endif

                    (cache_ctrl_ptr+i)->flag    = UBOT_CACHE_READ;
                    (cache_ctrl_ptr+i)->sec_num = i;
                }
            }

            /*copy cache buffer to des buf*/
            SCI_MEMCPY( desbuf, 
                        data_ptr+sector_index * unit_size,
                        sec_num * unit_size
                      );

            /*
            don't change cache flag
            for if cache has been written, we should keep write flag
            */
        }
        else{
            return FALSE;
        }
        
    }
    else{
        for(i = sector_index; i < (sec_num + sector_index); i++)
        {
            /*calc index in cache according to sector index*/
            index_num = sector_index % max_num;

            /*2.1 cache is empty*/
            if((cache_ctrl_ptr+index_num)->flag == UBOT_CACHE_EMPTY)
            {
#ifdef UBOT_UNIT_TEST
                SCI_MEMCPY(data_ptr + i * UNIT_SIZE, s_disk_buf + i*UNIT_SIZE, UNIT_SIZE);
#else
                //Read data form BSD to cache ;
                status = BSD_Read( cache_mgr_ptr->handle,
                                   i, /*sector index*/
                                   1,
                                   data_ptr + index_num * unit_size
                                 );

                if(BSD_SUCCESS != status)
                {
                    return FALSE;
                }
#endif
                /*copy cache buffer to des buf*/
                SCI_MEMCPY( desbuf + (i - sector_index)* unit_size,   
                            data_ptr + index_num * unit_size,
                            unit_size
                          );

            }
            else{
                /* 2.2. if cache is in use, compare the sector num with the input param,*/
                if(i == (cache_ctrl_ptr+index_num)->sec_num)
                {
                    /*2.2.1 equal, read from cache*/
                    /*copy cache buffer to des buf*/
                    SCI_MEMCPY( desbuf + (i - sector_index)* unit_size,   
                                data_ptr + index_num*unit_size,
                                unit_size
                              );
                }else{
                    /* 2.2.2 else 
                         1. write to disk first
                         2. read from device
                         3. copy to desnation buf.
                         4. update cache sector number
                         5. set cache flag to read
                    */
#ifdef UBOT_UNIT_TEST
                    SCI_MEMCPY(s_disk_buf+((cache_ctrl_ptr+index_num)->sec_num)*UNIT_SIZE,
                               data_ptr + index_num * UNIT_SIZE, 
                               UNIT_SIZE
                               );
#else

                    status = BSD_Write( cache_mgr_ptr->handle,
                                        (cache_ctrl_ptr+index_num)->sec_num, 
                                        1,
                                        data_ptr + index_num*unit_size
                                      );

                    if(BSD_SUCCESS != status)
                    {
                        return FALSE;
                    }
#endif


#ifdef UBOT_UNIT_TEST
                    SCI_MEMCPY(data_ptr + index_num* unit_size, s_disk_buf+i*UNIT_SIZE, UNIT_SIZE);
#else
                    //Read data form BSD to cache ;
                    status = BSD_Read( cache_mgr_ptr->handle,
                                       i,/*sector index*/
                                       1,
                                       data_ptr + index_num*unit_size
                                      );

                    if(BSD_SUCCESS != status)
                    {
                        return FALSE;
                    }
#endif

                    SCI_MEMCPY( desbuf + (i - sector_index)* unit_size,   
                                data_ptr+index_num*unit_size,
                                unit_size
                              );
                    
                }

                if((cache_ctrl_ptr + index_num)->flag == UBOT_CACHE_EMPTY)
                    (cache_ctrl_ptr + index_num)->flag    = UBOT_CACHE_READ;
                
                (cache_ctrl_ptr + index_num)->sec_num = i;


            }
        }
    }

    return TRUE;
}

LOCAL BOOLEAN _Ubot_Cache_Write(uint32 disk_index, uint8* srcbuf, uint32 sector_index, uint32 sec_num)
{
    uint32 i;
    uint32 max_num;
    uint32 index_num;
    uint32 unit_size;
    BSD_STATUS  status;
    UBOT_Cache_Mgr_T * cache_mgr_ptr;
    UBOT_Cache_T * cache_ctrl_ptr;
    uint8*         data_ptr;

    if((disk_index > UBOT_MAX_DISK_NUM) || (NULL == s_ubot_cache_mgr_ptr))
    {
        return FALSE;
    }

    cache_mgr_ptr = s_ubot_cache_mgr_ptr + disk_index;

    cache_ctrl_ptr = cache_mgr_ptr->cache_ctrl_ptr;
    data_ptr = cache_mgr_ptr->cache_data_ptr;
    if((NULL == cache_ctrl_ptr) || (NULL == data_ptr))
    {
        /*cache ctrl struct or cache data buffer is not exist*/
        return FALSE;
    }

    max_num   = cache_mgr_ptr->cache_num;
    unit_size = cache_mgr_ptr->cache_unit_size;
    /*
    1.if max cache num is more than total sector num, then all operation in cache
    2. else
       2.1. if cache is empty, write to cache directly
       2.2. if cache is in use, compare the sector num with the input param,
         2.2.1 equal, write to cache
         2.2.2 else 
             1. flush to disk first
             2. write to cache
             3. update cache sector number
             4. set cache flag to write

    */
    if(cache_mgr_ptr->allincache)
    {
        /*
         1.if max cache num is more than total sector num, then all operation in cache
        */    
        if((sector_index <= max_num) && (sector_index+ sec_num) <= max_num)
        {
            /*param is valid*/
            /*write to cache buffer*/
            SCI_MEMCPY( data_ptr+sector_index*unit_size,
                        srcbuf, 
                        sec_num * unit_size
                      );

            for(i = sector_index; i < (sec_num + sector_index); i++)
            {
                (cache_ctrl_ptr + i)->flag    = UBOT_CACHE_WRITE;
                (cache_ctrl_ptr + i)->sec_num = i;
            }            
            
        }
        else{
            return FALSE;
        }
        
    }
    else{
        for(i = sector_index; i < (sec_num + sector_index); i++)
        {
            index_num = sector_index % max_num;

            /* 2.1. if cache is empty, write to cache directly*/
            if((cache_ctrl_ptr+index_num)->flag == UBOT_CACHE_EMPTY)
            {
                /*copy src data to cache buffer*/
                SCI_MEMCPY( data_ptr+index_num*unit_size,
                            srcbuf + (i - sector_index)* unit_size,   
                            unit_size
                          );

            }
            else{
                /* 2.2. if cache is in use, compare the sector num with the input param,*/
                if(i == (cache_ctrl_ptr+index_num)->sec_num)
                {
                    /*2.2.1 equal, write to cache directly*/
                    /*write  srcdata to cache buffer*/
                    SCI_MEMCPY( data_ptr+index_num*unit_size,
                                srcbuf + (i - sector_index)* unit_size,   
                                unit_size
                              );

                }else{
                    /* 2.2.2 else 
                         1. flush to disk first
                         2. write to cache
                         3. update cache sector number
                         4. set cache flag to write
                    */
#ifdef UBOT_UNIT_TEST
                    SCI_MEMCPY(s_disk_buf+((cache_ctrl_ptr+index_num)->sec_num)*UNIT_SIZE,
                                data_ptr + index_num * unit_size,
                                UNIT_SIZE
                               );
#else
                    status = BSD_Write( cache_mgr_ptr->handle,
                                        (cache_ctrl_ptr+index_num)->sec_num, 
                                        1,
                                        data_ptr + index_num*unit_size
                                      );
                    if(BSD_SUCCESS != status)
                    {
                        return FALSE;
                    }
#endif
                    //write srcdata to cache buffer;
                    SCI_MEMCPY( data_ptr+index_num*unit_size,
                                srcbuf + (i - sector_index)* unit_size,   
                                unit_size
                              );
                }

            }

            (cache_ctrl_ptr+index_num)->flag    = UBOT_CACHE_WRITE;
            (cache_ctrl_ptr+index_num)->sec_num = i;
            
        }
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description: cache varibable init
//  Author:         
//  parameter:   disk_index: disk index from usb, depend on the sequence of ubot_config
//               handle    : handle for bsd operation
//               cache_num : max cache numer 
//               disk_num  : max disk sector number
//               unit_size : the minium unit size, prefered sector size
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        called when usb plug in   
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_Init(uint32 disk_index, uint32 handle, uint32 cache_size, uint32 disk_size, uint32 unit_size)
{
    uint32 size;
    
    UBOT_Cache_Mgr_T * cache_ptr;
    BOOLEAN ret = FALSE;

    if(disk_index >= UBOT_MAX_DISK_NUM)
    {
        return FALSE;
    }
    
    /*alloc and initiliaze the cache manager ctrl info*/
    if(!s_ubot_cache_created)
    {
        size = sizeof(UBOT_Cache_Mgr_T) * UBOT_MAX_DISK_NUM;
        s_ubot_cache_mgr_ptr = SCI_ALLOCA( size );

        if(s_ubot_cache_mgr_ptr != NULL )
        {
            /*alloc success*/    
            s_ubot_cache_created = TRUE;

            /*var init*/
            SCI_MEMSET(s_ubot_cache_mgr_ptr, 0, size );
        }else
        {
            return FALSE;
        }
    }

    /*alloc disk's cache control and data buffer*/
    if(s_ubot_cache_created)
    {
        cache_ptr = s_ubot_cache_mgr_ptr+disk_index;

        if(cache_size == 0)
            return FALSE;
        
        size =  sizeof(UBOT_Cache_T) * (cache_size+1);
        
        cache_ptr->cache_ctrl_ptr = SCI_ALLOCA(size);
        if(cache_ptr->cache_ctrl_ptr)
        {   /*cache ctrl info alloc success*/

            /*initiliaze to default state*/
            SCI_MEMSET(cache_ptr->cache_ctrl_ptr, 0, size);

            /*cache data buffer alloc*/
            cache_ptr->cache_data_ptr = SCI_ALLOCA(cache_size*unit_size+1);
            if(cache_ptr->cache_data_ptr)
            {
                cache_ptr->cache_enable    = TRUE;
                cache_ptr->disk            = disk_index;
                cache_ptr->handle          = handle;
                cache_ptr->cache_num       = cache_size;
                cache_ptr->cache_unit_size = unit_size;

                if(cache_size >= disk_size)
                    cache_ptr->allincache  = TRUE;
                    
                ret = TRUE;
            }
            else
            {
            	/*free already alloc mem*/
            	SCI_FREE(s_ubot_cache_mgr_ptr);
            	SCI_FREE(cache_ptr->cache_ctrl_ptr);
            }
        }
        else
        {
        	/*free already alloc mem*/
        	SCI_FREE(s_ubot_cache_mgr_ptr);
        }
    }
    return ret;
    
}

/*****************************************************************************/
//  Description: cache deinit
//  Author:         
//  parameter:   
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        called when usb plug out   
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_DeInit(void)
{
    uint32 i;
    UBOT_Cache_Mgr_T * cache_mgr_ptr;
    UBOT_Cache_Mgr_T * cur_ptr;
    
    cache_mgr_ptr = s_ubot_cache_mgr_ptr;

    if(cache_mgr_ptr == NULL)
    {
    	  s_ubot_cache_created = FALSE;
        return FALSE;
    }
    
    for(i = 0; i < UBOT_MAX_DISK_NUM; i++)
    {
        cur_ptr = cache_mgr_ptr + i;

        /*wrieback cache data to disk*/
        _Ubot_Cache_FlushAll(i);        

        /*free cache_data_buf, ctrl_buf*/
        if(cur_ptr->cache_enable)
        {
            if(cur_ptr->cache_data_ptr)
            {
                SCI_FREE(cur_ptr->cache_data_ptr);
            }

            if(cur_ptr->cache_ctrl_ptr)
            {
                SCI_FREE(cur_ptr->cache_ctrl_ptr);
            }

        }
        SCI_MEMSET(cur_ptr, 0xA5, sizeof(UBOT_Cache_Mgr_T));
    }

    
    SCI_FREE(cache_mgr_ptr);
    s_ubot_cache_mgr_ptr = NULL;
    s_ubot_cache_created = FALSE;

    return TRUE;
    
}

/*****************************************************************************/
//  Description: flush one packet cache data to device
//  Author:         
//  parameter:   disk_index: disk index from usb, depend on the sequence of ubot_config
//               index_num : index in cache buffer
//  return value:TRUE: success, 
//               FALSE: fail  
//  Note:        can be called at idle
/*****************************************************************************/
PUBLIC BOOLEAN Ubot_Cache_Flush(uint32 disk_index,uint32 index_num)
{
    uint32 max_num;
    BSD_STATUS  status;
    UBOT_Cache_Mgr_T * cache_mgr_ptr;
    UBOT_Cache_T * cache_ctrl_ptr;
    uint8*         data_ptr;
    
    if((disk_index > UBOT_MAX_DISK_NUM) || (NULL == s_ubot_cache_mgr_ptr))
    {
        return FALSE;
    }

    cache_mgr_ptr = s_ubot_cache_mgr_ptr + disk_index;

    max_num =  cache_mgr_ptr->cache_num;
    if(index_num >= max_num)
    {
        /*param invalid, */
        return FALSE;
    }
    
    cache_ctrl_ptr = cache_mgr_ptr->cache_ctrl_ptr;
    data_ptr = cache_mgr_ptr->cache_data_ptr;
    if((NULL == cache_ctrl_ptr) || (NULL == data_ptr))
    {
        /*cache ctrl struct or cache data buffer is not exist*/
        return FALSE;
    }

    /*only write when sector num in cache struct is equal to input param
      and cache flag is write*/
    if(UBOT_CACHE_WRITE == (cache_ctrl_ptr + index_num)->flag)
    {
        /*write back the info*/
        status = BSD_Write(cache_mgr_ptr->handle,
                  (cache_ctrl_ptr + index_num)->sec_num,
                  1,
                  data_ptr + index_num * cache_mgr_ptr->cache_unit_size);

        if(BSD_SUCCESS != status)
        {
            return FALSE;
        }

        /*set cache flag to read, update cache sector flag*/
        (cache_ctrl_ptr + index_num)->flag = UBOT_CACHE_READ;

    }
    return TRUE;
}


/*****************************************************************************/
//  Description:    
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN UBOT_DWordBufEndianSwap (uint32 * src ,uint32 * des ,uint len)
{
#if defined _UBOT_SC6600I_ || (defined CHIP_ENDIAN_LITTLE && defined SC8800S_LITTLE_ENDIAN)
    int i;
    uint32 *pSrc = src,*pDes = des;
    
    for (i=0;i<len ;i++){
        __asm {
            ldr      r5,[pSrc]
            and      r6,r5,#0xff00
            mov      r6,r6,lsl #8
            orr      r6,r6,r5,lsl #24
            orr      r6,r6,r5,lsr #24
            bic      r5,r5,#0xff000000
            bic      r5,r5,#0xff00
            orr      r6,r6,r5,lsr #8
            str      r6,[pDes]
        }
        pSrc++;
        pDes++;
    }
#endif    
    return TRUE ;
}

/*****************************************************************************/
//  Description:    Ubot get device descriptor
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC int UBOT_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen)
{
    USB_HWORD_VALUE_U value;
    uint16 length = 0;
    uint16 temp = 0;
    char *pData = 0;
	int ret=0;

#ifdef WINUSB_SUPPORT	
	PUBOT_STR_DESC_T pStr = UBOT_GetStrInfo();
#endif

	//save ubot interface num
	UBOT_SetInterfaceNum(INTERFACE_FIRST);
    value.wValue  = wValue;
    switch (value.bmDescriptor.type){
    
    case USB_DEVICE_DESCRIPTOR_TYPE:
        if (wLen >= 0x040){
            length = 18;            
        }
        else{
            length = wLen;
        }
       // pData   = (char *)&g_UBOT_DeviceDescr;
       pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pdev_desc;
        break;
    
    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
        if (wLen >= 0x0ff){
            length  = UBOT_CONFIG_DESCCIPTOR_LEN;           
        }
        else{
            length  = wLen;
        }
        //pData   = (char *)&g_UBOT_ConfigDescr;
        pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pubot_cfg_desc;
        break;
    
    case USB_STRING_DESCRIPTOR_TYPE:
    
        switch (value.bmDescriptor.index){
            //USB 2.0 spec chapter 9.6.7 
            case USB_STR_INDEX_LANGUAGE :
                if (wLen > sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }
                //pData   = (char *)&g_UBOT_LanguageStr;
                pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pusb_str_lang_desc;
                break;
            //USB 2.0 spec chapter 9.6.1
            case USB_STR_INDEX_SERIALNUMBER:
#ifdef WINUSB_SUPPORT
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
				    temp = pStr[USB_STR_INDEX_SERIALNUMBER].len;;
                    if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
				    pData   = pStr[USB_STR_INDEX_SERIALNUMBER].pStr;
				}
				else
#endif
				{
                if (wLen > sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }
                //pData   = (char *)&g_UBOT_SerialNumStr;
                pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pusb_str_serialnum_desc;
				}
                break;
    
            case USB_STR_INDEX_MANUFACTURER:
#ifdef WINUSB_SUPPORT
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
				    temp = pStr[USB_STR_INDEX_MANUFACTURER].len;;
                    if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
				    pData   = pStr[USB_STR_INDEX_MANUFACTURER].pStr;
				}
				else
#endif  
				{
                if (wLen > sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }

                //pData   = (char *)&g_UBOT_StrManufacturer;
				pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pusb_str_manf_desc;
				}	
                break;

            case USB_STR_INDEX_PRODUCT:
#ifdef WINUSB_SUPPORT              
				if(REFPARAM_GetUsbServicesFlag()==0)
				{
				    temp = pStr[USB_STR_INDEX_PRODUCT].len;;
                    if (wLen > temp){
                        length  = temp;        
                    }
                    else{
                        length  = wLen;
                    }
				    pData   = pStr[USB_STR_INDEX_PRODUCT].pStr;
				}
				else	
#endif
				{
                if (wLen > sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }

                //pData   = (char *)&g_UBOT_StrProduct;
                pData   = (char *)((PUBOT_CFG_INFO_T)UBOT_GetCfgInfo())->pusb_str_prod_desc;
				}
                break;

            default:
                break;
        }
        break;
    default:
        break;
    }

#ifdef UBOT_TEST_OUTPOT
    s_test_val1 = wValue;
    s_test_val2 = wIndex;
    s_test_val3 = wLen;
    s_test_val4 = length;
#endif    
    //Write data to EP in & send data to host  ;
    UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
    
	return ret;
}

PUBLIC void UBOT_SetEPINStall(void)
{
    *(volatile uint32 *)USB_DIEPCTL(UBOT_EPx_BULK_IN) |= BIT_21;	//STALL
}

PUBLIC void UBOT_SetEPStall(void)
{
	*(volatile uint32 *)USB_DIEPCTL(UBOT_EPx_BULK_IN) |= BIT_21;	//STALL
    *(volatile uint32 *)USB_DOEPCTL(UBOT_EPx_BULK_OUT) |= BIT_21;
}

//	Add clean all ep stall for MSC TOOL TEST
LOCAL void UBOT_CleanStall(void)
{
	uint32 i;

	for(i = 1; i <USB_EP_NUMBER; i++)
	{
		*(volatile uint32  *) USB_DIEPCTL(i) &= ~BIT_21;
		*(volatile uint32  *) USB_DOEPCTL(i) &= ~BIT_21;
	}

	for(i = 1; i <USB_EP_NUMBER; i++)
	{
		*(volatile uint32  *) USB_DOEPCTL(i) |= BIT_28;
		*(volatile uint32  *) USB_DIEPCTL(i) |= BIT_28;
	}

	* (volatile uint32 *) USB_DOEPCTL (UBOT_EPx_BULK_OUT) |= BIT_26;	// need clear NAK

}

// to distinguish different udisk interface number 
PUBLIC void UBOT_SetInterfaceNum(uint32 num)
{
	ubot_interface_num = num;
}
/*****************************************************************************/
//  Description:    This function is a mass storage protocol class function ;
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC void UBOT_GetMaxLUN (uint8 bDirection,uint16 wValue,\
                     uint16 wIndex ,uint16 wLen)
{
    uint16 length = 0;
    char *pData = 0;

    length = wLen;
    pData  = (char *)&g_UBOT_MaxLUN;

    UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
}
/*****************************************************************************/
//  Description:    This function is a mass storage protocol class function ;
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC void UBOT_HostReset (uint8 bDirection,uint16 wValue, uint16 wIndex ,uint16 wLen)
{
}
/*****************************************************************************/
//  Description:    Null function avoid null function pointer is called
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC UBOT_STATES_E UBOT_DoNothing (uint32 param,void * ptr)
{
    return g_UBOT_manager.state ;
}

/*****************************************************************************/
//  Description:	Copy data from buf to the cache   
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_WriteCache (uint8* buf ,uint32 len)
{
    //Cache must be empty;
#line 200    
    UBOT_ASSERT (!(g_UBOT_manager.cache.num)); /*assert verified*/
    if(len > g_UBOT_manager.cache.buf_size)
    {
	    if(FALSE == UBOT_ResizeCache(len))
	    {
		    return FALSE;
	    }
    }
    //Copy data to cache ;
    memcpy ((char *)g_UBOT_manager.cache.buf,
            (char *)buf,
                    len);
    g_UBOT_manager.cache.num = len;

    UBOT_DWordBufEndianSwap ((uint32*)g_UBOT_manager.cache.buf,
                            (uint32*)g_UBOT_manager.cache.buf,
                            ((g_UBOT_manager.cache.num + 3) >>2) );

    return TRUE;
}

/*****************************************************************************/
//  Description:    Call this function to adjust the cache size
//  Global resource dependence:
//  Author:         xiaobo.zhou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UBOT_ResizeCache(uint32 size)
{
    UBOT_MANAGER_T *umass = &g_UBOT_manager;

    if (NULL != umass->cache.malloc_buf) {
        SCI_FREE(umass->cache.malloc_buf);
        umass->cache.malloc_buf = NULL;
        umass->cache.buf = NULL;
        umass->cache.buf_size = 0;
    }

    umass->cache.malloc_buf = SCI_ALLOCA(size + 32);
    if (NULL != umass->cache.malloc_buf) {
        umass->cache.buf = (uint8 *)(((uint32)umass->cache.malloc_buf + 31) / 32 * 32);
        umass->cache.buf_size = size;
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description:	Copy data from BSD to cache
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 UBOT_ReadDataFromBSD (uint32 len)
{
    uint32 sector_num    =((g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[7] << 8)|
                           g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[8]     );
    uint32 sector_index  = NULL ;
    uint32 count  = 0 ;
    BSD_STATUS      status = TRUE;
	BOOLEAN 	    cache_status = FALSE;

    sector_num    =((g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[7] << 8)|
                     g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[8]     );
    sector_index  =((g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[2] << 24)|
                    (g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[3] << 16)|
                    (g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[4] << 8 )|
                     g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[5]      );

    //Cache must be empty;
    /*
    UBOT_ASSERT (!(g_UBOT_manager.cache.num));
    */

    if(g_UBOT_manager.cache.num)
    {
        //SCI_TRACE_LOW("UBOT_ReadDataFromBSD, num error %x",g_UBOT_manager.cache.num);
        return FALSE;
    }

    if(g_UBOT_manager.cache.buf_size < sector_num * g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size)
    {
	    status = UBOT_ResizeCache(sector_num * g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size);
	    if(FALSE == status)
	    {
		    return FALSE;
	    }
    }

	cache_status = _Ubot_Cache_Read(g_UBOT_manager.cbw.mCBW.bCBWLUN, 
                     (uint8 *)g_UBOT_manager.cache.buf,
                     sector_index,
                     sector_num
                     );
					 
    if(!cache_status)
	{
   		 while (sector_num>32){
    		/*lint -e737 */
        	status = BSD_Read(g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.dev_handle,
                      sector_index+count, \
                      32,\
                      (uint8 *)g_UBOT_manager.cache.buf+count*g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size); 
        	sector_num -=32;
        	count +=32;
        	if (BSD_SUCCESS != status)
        	{
            	return FALSE;
        	}
    	 }

    	  if (sector_num) {
        		status = BSD_Read(g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.dev_handle,
		                      sector_index+count, \
		                      sector_num,\
		                      (uint8 *)g_UBOT_manager.cache.buf+count*g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size); 
        		count += sector_num;
		 
    		}

    		sector_num = count;/*lint !e737*/
	
    		if(BSD_SUCCESS != status)
      		{
          		return FALSE;
       		 }
    }
    g_UBOT_manager.cache.num = sector_num * \
                g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size;

    UBOT_DWordBufEndianSwap ((uint32*)g_UBOT_manager.cache.buf,
                            (uint32*)g_UBOT_manager.cache.buf,
                            ((g_UBOT_manager.cache.num + 3) >>2) );

    return TRUE ;
}

/*****************************************************************************/
//  Description:    Call this function to start data to host if have data in 
//                  the cache;
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_SendCache (UBOT_LOCK_E lock)
{
    uint32 num = g_UBOT_manager.cache.num ;
    uint32 * buf = (uint32 *)g_UBOT_manager.cache.buf;


    do {
        if(num / (uint32)USB_MAX_TRANSFER_SIZE){
            g_UBOT_manager.cache.lock = lock ;

			UsbDevice_Start_send(ubot_interface,buf,USB_MAX_TRANSFER_SIZE);
            while ((g_UBOT_manager.cache.lock == lock)&&\
					(TRUE==(UDC_GetConnect())))
            {
				SCI_Sleep(1);
			};
            if (!(UDC_GetConnect())){
                return FALSE;
            }
            num -=  USB_MAX_TRANSFER_SIZE;
            buf +=  USB_MAX_TRANSFER_SIZE >>2 ;
        }
        else{
            g_UBOT_manager.cache.lock = lock ;

			UsbDevice_Start_send(ubot_interface,buf,num);
            while ((g_UBOT_manager.cache.lock == lock)&&\
             (TRUE==(UDC_GetConnect())))
            {
		SCI_Sleep(1);
	    };
            if (!(UDC_GetConnect())){
                return FALSE;
            }
            num = NULL ;
        }
    }while (num && (TRUE == UDC_GetConnect()));

    if (!UDC_GetConnect()){
        return FALSE;
    }

    UBOT_ResetCache();
    
    return TRUE ;
}
/*****************************************************************************/
//  Description:	Copy data from cache to buffer   
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC uint32 UBOT_ReadCache (uint8* buf,uint32 len)
{
    if ((g_UBOT_manager.cache.num < len)||\
        (g_UBOT_manager.cache.buf_size < len)||
        (g_UBOT_manager.cache.lock != UBOT_READY_LOCK)){
        return NULL ;
    }
 
    UBOT_DWordBufEndianSwap ((uint32*)g_UBOT_manager.cache.buf,
                            (uint32*)g_UBOT_manager.cache.buf,
                            ((g_UBOT_manager.cache.num + 3) >>2) );

    memcpy ((char *)buf,\
            (char *)g_UBOT_manager.cache.buf,
             len );

    return len;
}

/*****************************************************************************/
//  Description:    Call this function to read data from cache and write them 
//                   to  BSD 
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC uint32 UBOT_WriteDataToBSD (uint32 len)
{
    BSD_STATUS  status= TRUE;
    BOOLEAN     cache_status = FALSE;
    uint32      sector_num    =((g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[7] << 8)|
                               g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[8]     );
    
    uint32      sector_index  = NULL ;
    uint32      num = g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength;
    uint32      *buf ;
    uint32      count  = 0 ;

    /*if src data size is larger than case buffer size, then resize the buffer*/
    if(g_UBOT_manager.cache.buf_size < num)
    {
	    if(!UBOT_ResizeCache(num))
	    {
	        /*resize error, return directly*/
		    return FALSE;
	    }
    }
    
    buf = (uint32 *)g_UBOT_manager.cache.buf;

    do {
        if(num / (uint32)USB_MAX_TRANSFER_SIZE){
            //Receive 32K data from host ;
            UBOT_StartReceiveData(UBOT_DATA_OUT_LOCK, USB_MAX_TRANSFER_SIZE, buf);
            while ((g_UBOT_manager.cache.lock == UBOT_DATA_OUT_LOCK)&&\
                    (TRUE == UDC_GetConnect()))
            {
                SCI_Sleep(1);
            };

            if (!UDC_GetConnect()){
                return FALSE;
            }
            num -=  USB_MAX_TRANSFER_SIZE;
            buf +=  USB_MAX_TRANSFER_SIZE >>2 ;
        }
        else{
            //Receive less then 32K data from host ;
            UBOT_StartReceiveData(UBOT_DATA_OUT_LOCK, num, buf);
            while ((g_UBOT_manager.cache.lock == UBOT_DATA_OUT_LOCK)&&\
                    (TRUE == UDC_GetConnect()))
            {
                SCI_Sleep(1);
            };
            
            if (!UDC_GetConnect()){
                return FALSE;
            }
            num = NULL ;
        }
    }while (num && (TRUE == UDC_GetConnect()));

    if (!(UDC_GetConnect())){
        return FALSE;
    }

    //Received data number must equal length ;
    if(g_UBOT_manager.cache.num !=\
                         g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength)
    {
        return FALSE;
    }


   	
   	 	UBOT_DWordBufEndianSwap ((uint32*)g_UBOT_manager.cache.buf,
                            (uint32*)g_UBOT_manager.cache.buf,
                            ((g_UBOT_manager.cache.num + 3) >>2) );

    sector_index  =((g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[2] << 24)|
                    (g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[3] << 16)|
                    (g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[4] << 8 )|
                     g_UBOT_manager.cbw.mCBW.uCBWCB.bCBD[5]      );
 
 
 
     cache_status = _Ubot_Cache_Write(g_UBOT_manager.cbw.mCBW.bCBWLUN, 
                               (uint8 *)g_UBOT_manager.cache.buf,
                               sector_index,
                               sector_num
                               );
      if(!cache_status)
 	  {
			while (sector_num>32){

	          status = BSD_Write(g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.dev_handle,
	                      sector_index+count, \
	                      32,\
	                      (uint8 *)g_UBOT_manager.cache.buf+count*g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size); 
		   sector_num -=32;
		   count +=32;
		   if (BSD_SUCCESS != status)
		      {
		          return FALSE;
		       }
			   
	}

	if (sector_num) {
			status = BSD_Write(g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.dev_handle,
			                      sector_index+count, \
			                      sector_num,\
			                      (uint8 *)g_UBOT_manager.cache.buf+count*g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size); 
			 count += sector_num;
			 
	}
	
    if(BSD_SUCCESS != status)
    {
         return FALSE;
    }
	}

    return len;
}
/*****************************************************************************/
//  Description:    Reset the ubot cache
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_ResetCache (void)
{
    g_UBOT_manager.cache.num   = NULL;
    g_UBOT_manager.cache.lock  = UBOT_UNLOCK;
    g_UBOT_manager.data_out_buf = NULL;
    g_UBOT_manager.data_out_len = NULL;
    return TRUE;
}

/*****************************************************************************/
//  Description:    Function to known the device compaction
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_DevCompaction  (uint32 id)
{
    return TRUE  ;
}

/*****************************************************************************/
//  Description:    Function start to receive the data
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_StartReceiveData  (UBOT_LOCK_E lock,\
                                        uint32  len,\
                                        uint32 *buf)
{
    g_UBOT_manager.cache.lock = lock;
    g_UBOT_manager.data_out_len = len ;
    g_UBOT_manager.data_out_buf = buf ;

    UsbDevice_Start_receive(ubot_interface,buf,len);

    return TRUE;
}


#ifdef UBOT_UNIT_TEST
static void ubot_test_task(uint32 argc, void *argv)
{
    uint32 i,j;
    uint32 k;
    uint32 cache_size;
    uint32 disk_size;
    
    uint8* desBuf;
    uint8* diskData;
    uint32 seed;
    BOOLEAN ret;
    

    SCI_Sleep(20000);

    //SCI_TRACE_LOW:"@@@@@ubot test start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1265_112_2_18_1_54_46_1,(uint8*)"");
    
    /* case1: cache buffer is more than disk size*/
    k = 1; 
    //SCI_TRACE_LOW:"@@@@@test case%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1269_112_2_18_1_54_46_2,(uint8*)"d",k);
    
    /*buf init*/
    k = 1; 
    cache_size = CACHE_SIZE;
    disk_size  = DISK_SIZE;
    desBuf = s_data_buf;   //SCI_ALLOC_APP(UNIT_SIZE * cache_size);
    diskData = s_disk_buf; //SCI_ALLOC_APP(UNIT_SIZE * disk_size);

    /*cache init*/
    Ubot_Cache_Init( DISK_INDEX,
                     HANDLE_VAL, 
                     cache_size,
                     disk_size,
                     UNIT_SIZE
                    );

    SCI_MEMSET(desBuf  , 0xa5, UNIT_SIZE * disk_size);
    SCI_MEMSET(diskData, 0x5a, UNIT_SIZE * disk_size );
    
    /*call random or sequence*/
    for(i= 0; i < 10000; i++)
    {
        seed = SCI_GetTickCount();

        if(seed&0x01)
        {
            //SCI_TRACE_LOW:"@@@@@case%d,read, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1296_112_2_18_1_54_46_3,(uint8*)"dd",k, i);
            ret = _Ubot_Cache_Read(DISK_INDEX, desBuf + (i%disk_size)* UNIT_SIZE , i%disk_size, 1);
    
            if(!ret)
            {
                SCI_ASSERT(0);  /*assert verified*/
            }
        }
        else
        {
            //SCI_TRACE_LOW:"@@@@@case%d,write, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1306_112_2_18_1_54_46_4,(uint8*)"dd",k, i);
            for(j=0;j<UNIT_SIZE;j++)
            {
                *(desBuf + (i%disk_size)* UNIT_SIZE + j) = j%200;
            }
            
            ret = _Ubot_Cache_Write(DISK_INDEX, desBuf, i%disk_size, 1);

            if(!ret)
            {
                SCI_ASSERT(0);   /*assert verified*/
            }
        }
        //SCI_TRACE_LOW:"@@@@@test loop, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1319_112_2_18_1_54_46_5,(uint8*)"d", i);
        
        SCI_Sleep(i%20);
    }
    /*cache close*/
    Ubot_Cache_DeInit();

    /*content compare*/
    for(i=0; i< UNIT_SIZE * disk_size; i++)
    {
        if(*(desBuf+i) != *(diskData+i))
        {
            SCI_ASSERT(0);   /*assert verified*/
        }
    }

    /* case2: cache buffer is equal than disk size*/
    k = 2 ;
    //SCI_TRACE_LOW:"@@@@@test case%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1337_112_2_18_1_54_46_6,(uint8*)"d", k);
    
    /*buf init*/
    cache_size = CACHE_SIZE_1;
    disk_size  = DISK_SIZE;
    desBuf = s_data_buf;   //SCI_ALLOC_APP(UNIT_SIZE * cache_size);
    diskData = s_disk_buf; //SCI_ALLOC_APP(UNIT_SIZE * disk_size);

    /*cache init*/
    Ubot_Cache_Init( DISK_INDEX,
                     HANDLE_VAL, 
                     cache_size,
                     disk_size,
                     UNIT_SIZE
                    );

    SCI_MEMSET(desBuf  , 0x55, UNIT_SIZE * disk_size);
    SCI_MEMSET(diskData, 0xaa, UNIT_SIZE * disk_size );
    
    /*call random or sequence*/
    for(i= 0; i < 10000; i++)
    {
        seed = SCI_GetTickCount();

        if(seed&0x01)
        {
            //SCI_TRACE_LOW:"@@@@@case%d,read, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1363_112_2_18_1_54_46_7,(uint8*)"dd",k, i);
            ret = _Ubot_Cache_Read(DISK_INDEX, desBuf + (i%disk_size)* UNIT_SIZE , i%disk_size, 1);
    
            if(!ret)
            {
                SCI_ASSERT(0);  /*assert verified*/
            }
        }
        else
        {
            //SCI_TRACE_LOW:"@@@@@case%d,write, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1373_112_2_18_1_54_46_8,(uint8*)"dd",k, i);
            for(j=0;j<UNIT_SIZE;j++)
            {
                *(desBuf + (i%disk_size)* UNIT_SIZE + j) = j%200;
            }
            
            ret = _Ubot_Cache_Write(DISK_INDEX, desBuf, i%disk_size, 1);

            if(!ret)
            {
                SCI_ASSERT(0);  /*assert verified*/
            }
        }
        //SCI_TRACE_LOW:"@@@@@test loop, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1386_112_2_18_1_54_46_9,(uint8*)"d", i);
        
        SCI_Sleep(i%20);
    }
    /*cache close*/
    Ubot_Cache_DeInit();

    /*content compare*/
    for(i=0; i< UNIT_SIZE * disk_size; i++)
    {
        if(*(desBuf+i) != *(diskData+i))
        {
            SCI_ASSERT(0);   /*assert verified*/
        }
    }

    /* case3: cache buffer is less than disk size*/
    k = 3;
    //SCI_TRACE_LOW:"@@@@@test case%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1404_112_2_18_1_54_46_10,(uint8*)"d",k);

    cache_size = CACHE_SIZE_2;
    disk_size  = DISK_SIZE;
    desBuf = s_data_buf;   //SCI_ALLOC_APP(UNIT_SIZE * cache_size);
    diskData = s_disk_buf; //SCI_ALLOC_APP(UNIT_SIZE * disk_size);

    /*cache init*/
    Ubot_Cache_Init( DISK_INDEX,
                     HANDLE_VAL, 
                     cache_size,
                     disk_size,
                     UNIT_SIZE
                    );

    SCI_MEMSET(desBuf  , 0x55, UNIT_SIZE * disk_size);
    SCI_MEMSET(diskData, 0xaa, UNIT_SIZE * disk_size );
    
    /*call random or sequence*/
    for(i= 0; i < 10000; i++)
    {
        seed = SCI_GetTickCount();

        if(seed&0x01)
        {
            //SCI_TRACE_LOW:"@@@@@case%d,read, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1429_112_2_18_1_54_46_11,(uint8*)"dd",k, i);
            ret = _Ubot_Cache_Read(DISK_INDEX, desBuf + (i%disk_size)* UNIT_SIZE , i%disk_size, 1);
    
            if(!ret)
            {
                SCI_ASSERT(0);   /*assert verified*/
            }
        }
        else
        {
            //SCI_TRACE_LOW:"@@@@@case%d,write, %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1439_112_2_18_1_54_47_12,(uint8*)"dd",k, i);

            for(j=0;j<UNIT_SIZE;j++)
            {
                *(desBuf + (i%disk_size)* UNIT_SIZE + j) = j%200;
            }
            
            ret = _Ubot_Cache_Write(DISK_INDEX, desBuf, i%disk_size, 1);

            if(!ret)
            {
                SCI_ASSERT(0);   /*assert verified*/
            }
        }
        //SCI_TRACE_LOW:"@@@@@test loop, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1453_112_2_18_1_54_47_13,(uint8*)"d", i);
        
        SCI_Sleep(i%20);
    }
    /*cache close*/
    Ubot_Cache_DeInit();

    /*content compare*/
    for(i=0; i< UNIT_SIZE * disk_size; i++)
    {
        if(*(desBuf+i) != *(diskData+i))
        {
            SCI_ASSERT(0);  /*assert verified*/
        }
    }

    //SCI_TRACE_LOW:"@@@@@test completed!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_DRV_1469_112_2_18_1_54_47_14,(uint8*)"");

    while(1)
    {
        SCI_Sleep(100);
    }

}
	
void ubot_test_init(void)
{
    s_ubot_test_task = SCI_CreateAppThread("ubot_test_task", 
		                                   NULL,
		                                   ubot_test_task,
		                                   0,
		                                   0,
		                                   4096,
		                                   1,
		                                   30,
		                                   SCI_PREEMPT,            						
                                           SCI_AUTO_START
                                          );
		                                                   
}


#endif
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

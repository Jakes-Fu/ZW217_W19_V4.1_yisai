/******************************************************************************
 ** File Name:      Intel_m18_drv.h                                            *
 ** Author:         lin.liu                                                   *
 ** DATE:           27/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Flash Driver                                              *
 **                                                                           *
 **                 used by flash.c                                           *
 **                                                                           *
 **                 user can modify and replace the file, but all functions   *
 **                 must be supported                                         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/11/2001     Lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef _INTEL_M18_H
#define _INTEL_M18_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"   /* basic type  */
#include "flash.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif




/**---------------------------------------------------------------------------*
 **                         MARCO Define                                      *
 **---------------------------------------------------------------------------*/

/*
    Flash Device Information
*/

#define FLASH_MAX_PROGRAM_TIME   100    // 100 ms
#define FLASH_MAX_ERASE_TIME     20000   // 20 s

//for intel 
#define TMPL_M18_PROGRAM_SETUP      0x41
#define TMPL_BASE_FLASH_ADDRESS     0x00
#define TMPL_READ_STATUS_REGISTER   0x70
#define TMPL_CLEAR_STATUS_REGISTER  0x50
#define TMPL_READ_ID_CODES          0x90
#define TMPL_READ_QUERY             0x98
#define TMPL_BLOCK_ERASE            0x20
#define TMPL_CONFIRM                0xD0
#define TMPL_READ_ARRAY             0xff
#define TMPL_CONFIG_SETUP           0x60

/* Status Register Values */
#define TMPL_BLOCK_LOCKED           0x02
#define TMPL_PROGRAM_SUSPENDED      0x04
#define TMPL_VPP_LOW                0x08
#define TMPL_PROGRAM_ERROR          0x10
#define TMPL_ERASE_ERROR            0x20
#define TMPL_ERASE_SUSPENDED        0x40
#define TMPL_STATUS_READY           0x80

/* for intel write buffer command */
#define LOCK_BLOCK_SETUP            0x60
#define UNLOCK_BLOCK                0xd0
#define LOCK_BLOCK                  0x01
#define M18_BUFFER_PROGRAM_SETUP    0xe9
#define BUFFERED_PROGRAM_CONFIRM    0xd0
#define READ_ARRAY                  0xff
#define DEVICE_READY                0x80
#define INTEL_BUFFER_SIZE           64              //Buffer Size ,Byte 

#define TMPL_LOCKDOWN_BIT_SET       0x2F
#define TMPL_LOCK_BIT_SET           0x01            /* 2nd cycle lock bit command */
#define TMPL_LOCK_BIT_CLEAR         0xD0            /* 2nd cycle lock bit command */
#define TMPL_BLOCK_SUSPEND          0xB0
#define TMPL_BLOCK_RESUME           0xD0

#define TMPL_STATUS_READY           0x80

#define mIsStatusReady(A) ( ( A & TMPL_STATUS_READY ) == TMPL_STATUS_READY )

#define INTEL_RESUME_CYCLE(addr_ptr)    FLASH_WR(addr_ptr,TMPL_BLOCK_RESUME);\
                                        FLASH_DELAY_US(30)
#define INTEL_SUSPEND_CYCLE(addr_ptr)   FLASH_WR(addr_ptr,TMPL_BLOCK_SUSPEND);\
                                        FLASH_DELAY_US(20)
#define CHECK_READY_CYCLE(addr_ptr)\
            FLASH_DELAY_US(20)
            
#define INTEL_RESET_CYCLE(addr_ptr)     FLASH_WR(addr_ptr, TMPL_READ_ARRAY);\
            FLASH_DELAY_US(5)
            
/**---------------------------------------------------------------------------*
 **                         Structure                                         *
 **---------------------------------------------------------------------------*/
extern const NOR_FLASH_DRIVER_T  g_Intel_m18_driver;
/**---------------------------------------------------------------------------*
 **                         Function Prototype                                *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to init INTEL flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           
/*****************************************************************************/ 
LOCAL void INTEL_Init(void);  

/*****************************************************************************/
// Description :    This function is used to close INTEL flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           
/*****************************************************************************/
LOCAL void INTEL_Close(void);

/*****************************************************************************/
// Description :    This function is used to read a word from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       // the address that will be read.
// Note :           
/*****************************************************************************/
LOCAL uint16 INTEL_ReadWord(uint32 addr);       

/*****************************************************************************/
// Description :    This function is used to read a word from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address that will be read.
//      buf:        store the data that being read to this buffer.
//      read_len:   the count of bytes that will be read
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Read(uint32 addr, uint8 * buf, uint32 read_len); 

/*****************************************************************************/
// Description :    This function is used to write a word to flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address that will be read.
//      data:       the data to be written
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN  INTEL_WriteWord(uint32 addr, uint16 data);

/*****************************************************************************/
// Description :    This function is used to write word to the flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address that will be written.
//      buf:        Pointer to the buffer containing the data to be written
//      read_len:   number of bytes to write
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Write(uint32 addr, const uint8 * buf, uint32 len);  
             
/*****************************************************************************/
// Description :    This function is used to erase the flash according to the 
//                  addr.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address to erase
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN INTEL_Erase(uint32 addr);

/*****************************************************************************/
// Description :    This function is used to check whether the erase is completed 
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN INTEL_IsEraseCompleted(void);

/*****************************************************************************/
// Description :    This function is used to suspend the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void INTEL_Suspend(void);

/*****************************************************************************/
// Description :    This function is used to resume the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void INTEL_Resume(void);

/*****************************************************************************/
// Description :    This function is used to unlock the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address of flash
// Note :           
/*****************************************************************************/
LOCAL void INTEL_Unlock(uint32  addr);

/*****************************************************************************/
// Description :    This function is used to lock the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address of flash
// Note :           
/*****************************************************************************/
LOCAL void INTEL_Lock(uint32 addr);


/*****************************************************************************/
// Description :    This function is used to do something before reading from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void INTEL_EnterRead(void);

/*****************************************************************************/
// Description :    This function is used to do something after reading from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void INTEL_ExitRead(void);

/*****************************************************************************/
// Description :    This function is used to config flash struct
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/

LOCAL void INTEL_Config(NOR_FLASH_CONFIG_PTR cfg_ptr,NORMCP_SPEC_PTR spec_ptr);

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :         Younger.yang
// Parameter:
// Note :           
/*****************************************************************************/

LOCAL BOOLEAN INTEL_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       counter);   // number of byte to write.
    

/*****************************************************************************/
// Description :    m18 serial flash address convert
// Global resource dependence :
// Author :         Younger.yang
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL uint32 INTEL_EfsAddrConvert(uint32 addr);
    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif 
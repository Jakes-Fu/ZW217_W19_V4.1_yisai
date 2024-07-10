/******************************************************************************
 ** File Name:      spansion_drv.h                                            *
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
#ifndef _SPANSION_H
#define _SPANSION_H

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

#define UNLOCK_ADDR1   0x0AAA
#define UNLOCK_ADDR2   0x0555

/* Address/Command Info */
#define AUTOSELECT_CMD               (0x9090)
#define CFI_QUERY_CMD                (0x9898)
#define CHIP_ERASE_CMD               (0x1010)
#define BLOCK_ERASE_CMD              (0x3030)
#define ERASE_SETUP_CMD              (0x8080)
#define PROGRAM_CMD                  (0xA0A0)
#define RESET_CMD                    (0xF0F0)
#define RESUME_CMD                   (0x3030)
#define SECSI_SECTOR_ENTRY_CMD       (0x8888)
#define SECSI_SECTOR_EXIT_SETUP_CMD  (0x9090)
#define SECSI_SECTOR_EXIT_CMD        (0x0000)
#define SECTOR_ERASE_CMD             (0x3030)
#define SUSPEND_CMD                  (0xB0B0)
#define UNLOCK_BYPASS_ENTRY_CMD      (0x2020)
#define UNLOCK_BYPASS_PROGRAM_CMD    (0xA0A0)
#define UNLOCK_BYPASS_RESET_CMD1     (0x9090)
#define UNLOCK_BYPASS_RESET_CMD2     (0x0000)
#define UNLOCK_DATA1                 (0xAAAA)
#define UNLOCK_DATA2                 (0x5555)
#define WRITE_BUFFER_ABORT_RESET_CMD (0xF0F0)
#define WRITE_BUFFER_LOAD_CMD        (0x2525)
#define WRITE_BUFFER_PGM_CONFIRM_CMD (0x2929)


/******************** AMD Flash Optr **********************************/
// following two cycle


#define UNLOCK_CYCLE(addr)   			FLASH_WR( (addr & 0xFC000000) + UNLOCK_ADDR1, UNLOCK_DATA1); \
	                 			        FLASH_WR( (addr & 0xFC000000) + UNLOCK_ADDR2, UNLOCK_DATA2); 

#define PROGRAM_SETUP_CYCLE(addr)	    FLASH_WR( (addr & 0xFC000000) + UNLOCK_ADDR1, UNLOCK_BYPASS_PROGRAM_CMD);

#define ERASE_SETUP_CYCLE(addr)		    FLASH_WR( (addr & 0xFC000000) + UNLOCK_ADDR1, ERASE_SETUP_CMD);\
								        UNLOCK_CYCLE(addr)

#define RESET_CYCLE(addr)               FLASH_WR( (addr & 0xFC000000), RESET_CMD);\
                                FLASH_DELAY_US(5)

#define ERASE_BLOCK_CYCLE(addr) FLASH_WR(addr        , BLOCK_ERASE_CMD);\
                                FLASH_DELAY_US(20)

#define RESUME_CYCLE(addr)      FLASH_WR(addr        , RESUME_CMD);\
                                FLASH_DELAY_US(30)
                         
#define SUSPEND_CYCLE(addr)     FLASH_WR(addr        , SUSPEND_CMD);\
                                FLASH_DELAY_US(20)            
                                                   
#define WRITE_BUFFER_LOAD(addr) FLASH_WR(addr        , WRITE_BUFFER_LOAD_CMD);

#define WRITE_BUFFER_PGM_CONFIRM(addr)\
                                FLASH_WR(addr        , WRITE_BUFFER_PGM_CONFIRM_CMD);
#define WRITE_BUFFER_SET_COUNT(addr,counter)\
                                FLASH_WR(addr        , counter);

/**********************************************************************/


/**---------------------------------------------------------------------------*
 **                         Structure                                         *
 **---------------------------------------------------------------------------*/
extern const NOR_FLASH_DRIVER_T  g_Spansion_driver;
/**---------------------------------------------------------------------------*
 **                         Function Prototype                                *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to init spansion flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           
/*****************************************************************************/ 
LOCAL void SPANSION_Init(void);  

/*****************************************************************************/
// Description :    This function is used to close spansion flash
// Global resource dependence :
// Author :         Nick.Zhao
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Close(void);

/*****************************************************************************/
// Description :    This function is used to read a word from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       // the address that will be read.
// Note :           
/*****************************************************************************/
LOCAL uint16 SPANSION_ReadWord(uint32 addr);       

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
LOCAL BOOLEAN SPANSION_Read(uint32 addr, uint8 * buf, uint32 read_len); 

/*****************************************************************************/
// Description :    This function is used to write a word to flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address that will be read.
//      data:       the data to be written
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN  SPANSION_WriteWord(uint32 addr, uint16 data);

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
LOCAL BOOLEAN SPANSION_Write(uint32 addr, const uint8 * buf, uint32 len);  
             
/*****************************************************************************/
// Description :    This function is used to erase the flash according to the 
//                  addr.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address to erase
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN SPANSION_Erase(uint32 addr);

/*****************************************************************************/
// Description :    This function is used to check whether the erase is completed 
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN SPANSION_IsEraseCompleted(void);

/*****************************************************************************/
// Description :    This function is used to suspend the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Suspend(void);

/*****************************************************************************/
// Description :    This function is used to resume the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Resume(void);

/*****************************************************************************/
// Description :    This function is used to unlock the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address of flash
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Unlock(uint32  addr);

/*****************************************************************************/
// Description :    This function is used to lock the flash.
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
//      addr:       the address of flash
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Lock(uint32 addr);


/*****************************************************************************/
// Description :    This function is used to do something before reading from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_EnterRead(void);

/*****************************************************************************/
// Description :    This function is used to do something after reading from flash
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_ExitRead(void);

/*****************************************************************************/
// Description :    This function is used to config flash struct
// Global resource dependence :
// Author :         Nick.Zhao
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL void SPANSION_Config(NOR_FLASH_CONFIG_PTR cfg_ptr,NORMCP_SPEC_PTR spec_ptr);

/*****************************************************************************/
// Description :    buffer write function
// Global resource dependence :
// s_flash_sector_size
// Author :         Younger.yang
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL BOOLEAN SPANSION_WriteBuffer(    // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       counter);   // number of byte to write.
    
/*start added by victorxu*/
/*start added by feng tao*/
/*****************************************************************************/
// Description :    m18 serial flash address convert
// Global resource dependence :
// Author :         Younger.yang
// Parameter:
// Note :           
/*****************************************************************************/
LOCAL uint32 SPANSION_EfsAddrConvert(uint32 addr);
/*end added by feng tao*/
/*end added by victorxu*/    
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif 
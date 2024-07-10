/******************************************************************************
 ** File Name:      flash.c                                               *
 ** Author:         lin.liu                                                   *
 ** DATE:           10/01/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Manager AMD flash.                                        *
 **                 This driver support Am29DL322GT                           *
 **                 If change flash, need modify sector number etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01-10-2003     lin.liu          Create.                                   *
 ** 04-19-2003     Lin.liu          Move the config to the memory config file *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"
#include "chip_plf_export.h"
#include "flash.h"
#include "task_monitor.h"
#include "dal_power.h"
#include "umem_cfg.h"
#include "sfc_drvapi.h"
#include "spiflash_spec.h"
#include "norflash_phy.h"
#include "nv_item_id.h"
#include "mem_cfg.h"

#include "ap_clk.h"
#include "ap_apb.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#pragma arm section code = "FLASH_DRV_CODE"


LOCAL BOOLEAN   s_flash_is_simulate = FALSE;

LOCAL uint32    s_flash_start_addr;
LOCAL uint32    s_flash_end_addr;
LOCAL uint32    s_flash_efs_start_addr;
LOCAL uint32    s_flash_efs_end_addr;
LOCAL uint32    s_flash_lcd_sensor_addr;
LOCAL uint32    s_flash_sector_size;
LOCAL uint32    s_flash_sector_num;
LOCAL uint32    s_flash_file_sect_num;
LOCAL uint32    s_flash_size;
LOCAL uint32    s_flash_fixed_nvitem_addr;
LOCAL uint32    s_flash_fixed_nvitem_end_addr;
LOCAL uint32    s_flash_product_info_addr;
LOCAL uint32    s_flash_bank_num;/*lint -esym(551,s_flash_bank_num)*/
LOCAL uint32    s_flash_suspend_flag;
LOCAL uint32    s_flash_mmires_ptr;
LOCAL uint32    s_flash_umem_addr;
LOCAL uint32    s_flash_umem_size;
LOCAL uint32    s_flash_simlock_addr;
LOCAL uint32    s_flash_simlock_end_addr;
LOCAL uint32    s_flash_xfile_addr;
LOCAL uint32    s_flash_xfile_end_addr;
LOCAL uint32    s_flash_auto_sms_addr;
LOCAL uint32    s_flash_auto_sms_end_addr;
LOCAL uint32    s_flash_operator_data_addr;
LOCAL uint32    s_flash_operator_data_end_addr;
LOCAL uint32    s_flash_apn_addr=0;
LOCAL uint32    s_flash_apn_end_addr=0;
LOCAL uint32    s_flash_cfg_nv_addr=0;
LOCAL uint32    s_flash_cfg_nv_end_addr=0;
//LOCAL uint32    s_flash_ims_addr=0;
//LOCAL uint32    s_flash_ims_end_addr=0;

LOCAL uint32    s_flash_crystal_aging_addr;
LOCAL uint32    s_flash_crystal_aging_end_addr;
LOCAL uint32    s_flash_ims_toggle_cfg_addr;
LOCAL uint32    s_flash_ims_toggle_cfg_end_addr;
#ifdef NOR_BOOTLOADER_SUPPORT
LOCAL uint32    s_flash_ps_ptr;
#endif
#ifdef DSP_DOWNLOAD_SUPPORT
LOCAL uint32    s_flash_dsp_code_addr;
#endif

#ifdef FOTA_SUPPORT_ABUP
LOCAL uint32    s_flash_abup_fota_app_addr;
LOCAL uint32    s_flash_abup_fota_app_addr_end;
#endif

#ifdef FOTA_SUPPORT_CMIOT
LOCAL uint32    s_flash_cmiot_fota_app_addr;
LOCAL uint32    s_flash_cmiot_fota_app_addr_end;
#endif

LOCAL SCI_MUTEX_PTR s_flash_mutex_ptr = SCI_NULL;

//pointer to some normcp spec info
LOCAL NORMCP_SPEC_PTR       s_Normcp_spec_ptr = {0};
//pointer to some normcp config info
LOCAL NORMCP_CONFIG_PTR s_Normcp_config_ptr;
//pointer to flash config in the system
LOCAL NOR_FLASH_CONFIG_PTR  s_Nor_Config_ptr= {0};
//pointer to the flash operation
LOCAL NOR_FLASH_DRIVER_PTR  s_Nor_Driver_ptr;

extern uint32 _tx_thread_system_state;

#ifdef FLASH_TYPE_SPI
extern NOR_FLASH_DRIVER_T  g_spiflash_driver;
LOCAL uint8  nMID, nDID1, nDID2;
#else
LOCAL uint32  s_manu_id, s_dev_id, s_ext_id;

extern NOR_FLASH_DRIVER_T  g_Spansion_driver;
extern NOR_FLASH_DRIVER_T  g_Intel_driver;

/*start added by feng tao*/
extern NOR_FLASH_DRIVER_T g_Intel_m18_driver;
#endif

//numonyx  m18 flash serial indicator
LOCAL BOOLEAN s_is_intel_m18_flash = SCI_FALSE;
/*end added by feng tao*/

#ifdef UMEM_NEW_MECHNISM
//extern CONST uint16 NV_MAX_ID;
#endif

#define SECURE_BOOT_ANTI_CLONE_OFFSET (0x1000)

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*start added by feng tao*/
/*****************************************************************************/
//  Description :
//   Verify whether this flash is numonyx m18 flash or not
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/

LOCAL  void FLASH_M18_Type_Set (uint32 Manu_id, uint32 Dev_id, uint32 Extend_id)
{
    if ( (Manu_id==FLASH_INTEL_ID) && ( (Dev_id == 0x8900) || (Dev_id == 0x8901) || (Dev_id == 0x8903) || (Dev_id == 0x887e)))
    {
        s_is_intel_m18_flash = SCI_TRUE;
    }
    else
    {
        s_is_intel_m18_flash = SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description :
//   get flash type, mainly determine  whether this flash is numonyx m18 flash or not
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/

PUBLIC BOOLEAN FLASH_M18_Type_Get (void)
{
    return s_is_intel_m18_flash;
}
/*end added by feng tao*/

/*****************************************************************************/
//  Description :
//  Init the device layer
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_Init (void) /* If Init success, return true, else return false */
{
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    s_Nor_Driver_ptr->_init();

    s_Nor_Driver_ptr->_config (s_Nor_Config_ptr,s_Normcp_spec_ptr);

#ifndef FLASH_TYPE_SPI
    FLASH_SetSuspendFlag (SCI_FALSE);
#endif
    s_flash_mutex_ptr =SCI_CreateMutex ("Flash OPERATION SEMAPHORE", SCI_INHERIT);
    return TRUE;
}

/*****************************************************************************/
//  Description :
//      Close the flash device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      This function should be called when power down.
/*****************************************************************************/
void FLASH_Close (void)
{
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    s_Nor_Driver_ptr->_close();

}

/*****************************************************************************/
//  Description :
//      Get sector size and sector number of the device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_GetDeviceInfo (
    uint32   *sector_size_ptr, // output the sector size  */
    uint16   *sector_num_ptr,  // output the sector number
    uint16   *file_system_sector_num_ptr)  // output the file system sector num.
{
    *sector_size_ptr            = (uint32) s_flash_sector_size;
    *sector_num_ptr             = (uint16) s_flash_sector_num;
    *file_system_sector_num_ptr = (uint16) s_flash_file_sect_num;
}

/*****************************************************************************/
//  Description :
//      Convert address to sector number.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      sector no. 0 is the first sector we managered
/*****************************************************************************/
uint16  FLASH_AddrToSector ( // return the no. of the sector, If addr isn't in flash, return 0xFFFF
    uint32 addr)   // physical address  that will be converted
{

    if (FLASH_ADDR_IS_VALID (addr))
    {
        SCI_ASSERT (addr >= s_flash_efs_start_addr);/*assert verified*/
        addr       -= (uint32) s_flash_efs_start_addr; // get offset

        return (uint16) (addr / s_flash_sector_size);
    }
    else
    {
        // return the invalid sector no.
        return FLASH_INVALID_SECTOR;
    }


}

/*****************************************************************************/
//  Description :
//      Convert sector no. to physical address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
/*****************************************************************************/
uint32 FLASH_SectorToAddr (  /* start physical address of the sector */
    uint16 sector_no)    /* sector's no.                         */
{

    return (sector_no < s_flash_sector_num)
           ?   s_flash_efs_start_addr + (uint32) sector_no * s_flash_sector_size
           :   FLASH_INVALID_ADDR;
}

/*****************************************************************************/
//  Description :
//  Calculate the checksum
//  Input:
//  addr : the start address that want to be check.
//  len  : the word counts that want to be check.
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/

uint16 FLASH_GetCheckSum (uint32 addr, uint32 len)
{
    uint16  check_sum = 0xC50F + (uint16) addr;
    uint32  temp_addr=0;//tao

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + len));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    s_Nor_Driver_ptr->_enter_read();

    if (s_Nor_Driver_ptr->_addrconvert)
    {
        while (len--)
        {
            temp_addr = s_Nor_Driver_ptr->_addrconvert (addr);
            check_sum += * (FLASH_PTR_TYPE) (temp_addr);

            addr      += sizeof (uint16);

            //@lin.liu(2003-12-16). CR: 6250
            if (len && (0 == (len & 0x1FF)))
            {
                s_Nor_Driver_ptr->_exit_read();
                // Enable Handle INT.
                s_Nor_Driver_ptr->_enter_read();
            }

            // End 6250
        }
    }
    else
    {
        while (len--)
        {
            check_sum += * (FLASH_PTR_TYPE) (addr);

            addr      += sizeof (uint16);

            //@lin.liu(2003-12-16). CR: 6250
            if (len && (0 == (len & 0x1FF)))
            {
                s_Nor_Driver_ptr->_exit_read();
                // Enable Handle INT.
                s_Nor_Driver_ptr->_enter_read();
            }// End 6250
        }
    }

    s_Nor_Driver_ptr->_exit_read();

    return check_sum;
}

/*****************************************************************************/
//  Description :
//      Check the content of given area are all 0xFFFF ?
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint32 FLASH_IsEmptySpace (uint32 addr, uint32 end_addr)
{
    /*start modified by feng tao*/
    uint16  tmp, cmp;
    uint32  tmp_addr = 0;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (end_addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    s_Nor_Driver_ptr->_enter_read();

    cmp = 0xFFFF;

    if (s_Nor_Driver_ptr->_addrconvert)
    {
        while (addr < end_addr)
        {
            //tmp = ( *((volatile uint16 *)addr) );//tao
            tmp_addr = s_Nor_Driver_ptr->_addrconvert (addr);
            tmp = REG16 (tmp_addr);


            if (cmp != tmp)
            {
                break;
            }

            addr += sizeof (tmp);

            //@lin.liu(2003-12-16). CR: 6250
            if (0 == (addr & 0x1FF))
            {
                s_Nor_Driver_ptr->_exit_read();
                // Enable Handle INT.
                s_Nor_Driver_ptr->_enter_read();
            }

            // End 6250
        }
    }
    else
    {
        while (addr < end_addr)
        {
            tmp = REG16 (addr);

            if (cmp != tmp)
            {
                break;
            }

            addr += sizeof (tmp);

            //@lin.liu(2003-12-16). CR: 6250
            if (0 == (addr & 0x1FF))
            {
                s_Nor_Driver_ptr->_exit_read();
                // Enable Handle INT.
                s_Nor_Driver_ptr->_enter_read();
            }

            // End 6250
        }
    }

    s_Nor_Driver_ptr->_exit_read();
    return (addr >= end_addr) ? 1 : 0;
    /*end modified by feng tao*/
}

/*****************************************************************************/
//  Description :
//      Read the given count from device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
void FLASH_ReadWordByCount (uint32  addr, uint16 *w16_ptr, uint32  count)
{
    /*start modified by feng tao*/
    uint32 tmp_addr = 0;
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + count * 2));/*assert verified*/

    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    s_Nor_Driver_ptr->_enter_read();

    while (count--)
    {
        if (s_Nor_Driver_ptr->_addrconvert)
        {
            tmp_addr = s_Nor_Driver_ptr->_addrconvert (addr);
            *w16_ptr++   = * ( (FLASH_PTR_TYPE) tmp_addr);
        }
        else
        {
            *w16_ptr++   = * ( (FLASH_PTR_TYPE) addr);
        }

        addr        += 2;

        //@lin.liu(2003-12-16). CR: 6250
        if (count && (0 == (count & 0x1FF)))
        {
            s_Nor_Driver_ptr->_exit_read();
            // Enable Handle INT.
            s_Nor_Driver_ptr->_enter_read();
        }

        // End 6250

    }

    s_Nor_Driver_ptr->_exit_read();
    /*end modified by feng tao*/
}

//@ End CR: 5446


/*****************************************************************************/
//  Description :
//      Read word(two byte) from the given address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint16 FLASH_ReadWord ( // The word that has read from the given address.
    uint32 addr)        // the address that will be read.
{
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr); /*assert verified*/
#ifndef   SDRAM_SIMUL_NOR_FLASH
    return s_Nor_Driver_ptr->_readword (addr);
#else
    return * (uint16 *) addr;
#endif

}

/*****************************************************************************/
//  Description :
//      Read Dword(four byte) from the given address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint32 FLASH_ReadDWord(
	uint32 addr)
{
	uint32 value = 0;
		SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
		SCI_ASSERT (s_Nor_Driver_ptr); /*assert verified*/
#ifndef   SDRAM_SIMUL_NOR_FLASH
		s_Nor_Driver_ptr->_read(addr,(uint8 *)&value,4);
		return value;
#else
		return * (uint32 *) addr;
#endif

}
/*****************************************************************************/
//  Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_Read (     // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8    *buf,       // store the data that being read to this buffer.
    uint32    read_len)  // the count of bytes that will be read.
{
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + read_len));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
#ifndef   SDRAM_SIMUL_NOR_FLASH
    return s_Nor_Driver_ptr->_read (addr,buf,read_len);
#else
    {
        uint8 *flash_addr = (uint8 *) addr;

        while (read_len--)
        {
            *buf++ = *flash_addr++ ;
        }

        return TRUE;
    }
#endif
}


uint16 FLASH_FastSearchWord (
    uint32    addr,         // the start address that will be read
    uint32      end_addr,
    uint16      cmpdata   /* compare data: maybe 0xfffe to kick out 0xffff(empty sector), maybe 0x1000,to find the first available data <=0x1000*/
)

{
    uint16  tmp,cmp;
    uint32  addr_start=addr;
    uint32  tmp_addr = 0;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (end_addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    s_Nor_Driver_ptr->_enter_read();

    cmp = cmpdata;

    while (addr < end_addr)
    {
        if (s_Nor_Driver_ptr->_addrconvert)
        {
            tmp_addr = s_Nor_Driver_ptr->_addrconvert (addr);
            tmp = REG16 (tmp_addr);
        }
        else
        {
            tmp = REG16 (addr);
        }

        if (cmp >= tmp)
        {
            break;
        }

        addr += sizeof (tmp);

        //@lin.liu(2003-12-16). CR: 6250
        if (0 == (addr & 0x1FF))
        {
            s_Nor_Driver_ptr->_exit_read();
            // Enable Handle INT.
            s_Nor_Driver_ptr->_enter_read();
        }

        // End 6250
    }

    s_Nor_Driver_ptr->_exit_read();
    return (addr-addr_start);
}


/*****************************************************************************/
//  Description :
//      The function unlock the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Unlock (
    uint32   addr)  // the sector that contain the address will be unlock.
{
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    s_Nor_Driver_ptr->_unlock (addr);
}

/*****************************************************************************/
//  Description :
//      The function unlock the sector that contain the address. only used for
//  secure boot function
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_UnlockSecureOnly (
    uint32   addr)  // the sector that contain the address will be unlock.
{
    s_Nor_Driver_ptr->_unlock (addr);
}


/*****************************************************************************/
//  Description :
//      The function lock down the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Lock (uint32  addr)
{
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    s_Nor_Driver_ptr->_lock (addr);
}

/*****************************************************************************/
//  Description :
//    write a uint16 , the addr must be even.
//    we check first to avoid flash-reset,
//    if can't program( 0-->1 ), return false;
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/

BOOLEAN  FLASH_WriteWord (
    uint32    addr,
    uint16    data)
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
    BOOLEAN ret;
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
#ifdef __DEBUG_FIXNV_TRACE__

    if (POWER_GetRestartCondition() == RESTART_BY_POWER_BUTTON)
    {
        if (s_flash_efs_start_addr > addr)
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }

#endif
    FLASH_GetSemaphore();
    ret = s_Nor_Driver_ptr->_writeword (addr, data);
    FLASH_ReleaseSemaphore();

    return ret;
#else
    * (uint16 *) addr = data;
    return TRUE;
#endif

}

/*****************************************************************************/
//  Description :
//      The function write data to flash.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
BOOLEAN FLASH_Write (   // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8   *buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
    BOOLEAN ret;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + len));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

#ifdef __DEBUG_FIXNV_TRACE__

    if (POWER_GetRestartCondition() == RESTART_BY_POWER_BUTTON)
    {
        if (s_flash_efs_start_addr > addr)
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }

#endif
    FLASH_GetSemaphore();
    TM_SendTestPointRequest ( (0x08096000), len);
    ret = s_Nor_Driver_ptr->_write (addr, buf, len);
    TM_SendTestPointRequest ( (0x08096001), len);
    FLASH_ReleaseSemaphore();
    return ret;
#else
    {
        uint8 *flash_addr = (uint8 *) addr;

        while (len--)
        {
            *flash_addr++ = *buf++;
        }

        return TRUE;
    }
#endif
}


/*****************************************************************************/
//  Description :
//      The function write data to flash. only used for secure function.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
BOOLEAN FLASH_WriteSecureOnly (   // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8   *buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
    BOOLEAN ret;

    FLASH_GetSemaphore();
    ret = s_Nor_Driver_ptr->_write (addr, buf, len);
    FLASH_ReleaseSemaphore();

    return ret;
#else
    {
        uint8 *flash_addr = (uint8 *) addr;

        while (len--)
        {
            *flash_addr++ = *buf++;
        }

        return TRUE;
    }
#endif
}

/*****************************************************************************/
//  Description :
//      Copy data from one area to another.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
/*****************************************************************************/
BOOLEAN FLASH_Copy (
    uint32 dest,   /* the area we copy to   */
    uint32 sour,   /* the area we copy from */
    uint32  len)   /* the length we want copy*/
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
#if !defined(PLATFORM_UWS6121E)
    uint8     buf[64];  // temp buffer while copy data.
    BOOLEAN ret = SCI_TRUE;
    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest + len));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour + len));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    FLASH_GetSemaphore();

    while (len > 64)
    {
        if (!s_Nor_Driver_ptr->_read (sour, buf, 64))
        {
            ret = SCI_FALSE;
            break;
        }

        if (!s_Nor_Driver_ptr->_write (dest, buf, 64))
        {
            ret = SCI_FALSE;
            break;
        }

        dest += 64;
        sour += 64;
        len  -= 64;
    }

    if (len && ret)
    {
        if (!s_Nor_Driver_ptr->_read (sour, buf, len))
        {
            ret = SCI_FALSE;
        }
        else if (!s_Nor_Driver_ptr->_write (dest, buf, len))
        {
            ret = SCI_FALSE;
        }
    }

    FLASH_ReleaseSemaphore();

    return ret;
#else
    uint8     buf[256];  // temp buffer while copy data.
    BOOLEAN ret = SCI_TRUE;
    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest + len));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour + len));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

    FLASH_GetSemaphore();

    while (len > 256)
    {
        if (!s_Nor_Driver_ptr->_read (sour, buf, 256))
        {
            ret = SCI_FALSE;
            break;
        }

        if (!s_Nor_Driver_ptr->_write (dest, buf, 256))
        {
            ret = SCI_FALSE;
            break;
        }

        dest += 256;
        sour += 256;
        len  -= 256;
    }

    if (len && ret)
    {
        if (!s_Nor_Driver_ptr->_read (sour, buf, len))
        {
            ret = SCI_FALSE;
        }
        else if (!s_Nor_Driver_ptr->_write (dest, buf, len))
        {
            ret = SCI_FALSE;
        }
    }

    FLASH_ReleaseSemaphore();

    /* All Done */
    return ret;
#endif
#else
    uint8 *des_addr = (uint8 *) dest;
    uint8 *src_addr = (uint8 *) sour;

    while (len--)
    {
        *des_addr++ = *src_addr++;
    }

    return TRUE;
#endif
}


/*****************************************************************************/
//  Description :
//      Check can we write data to the area?
//      If some bits of the area from 0 to 1, the write wouldn't success,
//      and return false;
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
//      This function called from EFS Task, so needn't  check erase status.
//
/*****************************************************************************/
BOOLEAN FLASH_CheckRevisable (
    uint32      addr,  // the start address of the area that will be checked.
    const uint8  *buf, // the buffer that contain the data.
    uint32      len)   // the data length.
{
    /*start modified by feng tao*/
    FLASH_PTR_TYPE                addr_ptr  = (FLASH_PTR_TYPE) addr;
    FLASH_PTR_TYPE       tmp_addr_ptr =NULL;
    uint16                        data, flash_data;
    uint8                         ch;

    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + len));/*assert verified*/

    if (0 == len)
    {
        return TRUE;
    }

    s_Nor_Driver_ptr->_enter_read();

    addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);

    if (addr & 1)
    {
        /*
            check first byte
        */
        //  ch = SECOND_BYTE( *addr_ptr );//tao
        if (s_Nor_Driver_ptr->_addrconvert)
        {
            tmp_addr_ptr = (FLASH_PTR_TYPE) s_Nor_Driver_ptr->_addrconvert ( (uint32) addr_ptr);
            ch = SECOND_BYTE (*tmp_addr_ptr);
        }
        else
        {
            ch = SECOND_BYTE (*addr_ptr);
        }

        addr_ptr++;

        if ( (ch & *buf) != *buf)
        {
            s_Nor_Driver_ptr->_exit_read();
            return FALSE;
        }

        buf ++;
        len --;
    }

    while (len > 1)
    {
        if (s_Nor_Driver_ptr->_addrconvert)
        {
            tmp_addr_ptr = (FLASH_PTR_TYPE) s_Nor_Driver_ptr->_addrconvert ( (uint32) addr_ptr);
            flash_data = *tmp_addr_ptr;
            addr_ptr++;
        }
        else
        {
            flash_data = *addr_ptr++;
        }

        ch = *buf++;
        data = MAKE_WORD (ch, *buf);
        buf++;

        if (data != (flash_data & data))
        {
            s_Nor_Driver_ptr->_exit_read();
            return FALSE;
        }

        //@lin.liu(2003-12-16). CR: 6250
        if (0 == (len & 0x1FF))
        {
            s_Nor_Driver_ptr->_exit_read();
            // Enable Handle INT.
            s_Nor_Driver_ptr->_enter_read();
        }

        // End 6250

        len -= 2;         // decrease sizeof(word)
    }

    if (len)
    {
        if (s_Nor_Driver_ptr->_addrconvert)
        {
            tmp_addr_ptr = (FLASH_PTR_TYPE) s_Nor_Driver_ptr->_addrconvert ( (uint32) addr_ptr);
            ch = FIRST_BYTE (*tmp_addr_ptr);
        }
        else
        {
            ch = FIRST_BYTE (*addr_ptr);
        }

        ch                         &= *buf;

        if (ch != *buf)
        {
            s_Nor_Driver_ptr->_exit_read();
            return FALSE;
        }
    }

    s_Nor_Driver_ptr->_exit_read();
    return TRUE;
    /*end modified by feng tao*/

}


/*****************************************************************************/
//  Description :
//      Is the last erase optr completed?
//
//  Global resource dependence :
//      None
//
//  Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:
//      This function MUST be called when system assert only,
/*****************************************************************************/
BOOLEAN FLASH_IsEraseCompleted (void)
{
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    return s_Nor_Driver_ptr->_iserasecompleted();
}
/*****************************************************************************/
//  Description :
//      Erase the sector that contain the address.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/

BOOLEAN FLASH_Erase (
    uint32 addr)
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
    BOOLEAN ret;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/

#ifdef __DEBUG_FIXNV_TRACE__

    if (POWER_GetRestartCondition() == RESTART_BY_POWER_BUTTON)
    {
        if (s_flash_efs_start_addr > addr)
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }

#endif
    FLASH_GetSemaphore();
    TM_SendTestPointRequest ( (0x08096003), addr);
    addr = addr & (~ (s_flash_sector_size -1)); //Keep Erase Address sector-aligned
    ret = s_Nor_Driver_ptr->_erase (addr);
    TM_SendTestPointRequest ( (0x08096004), addr);
    FLASH_ReleaseSemaphore();

    return ret;
#else
    {
        uint32  *flash_addr = (uint32 *) addr;
        uint32 t = 0;

        for (t= 0; t < 16*1024; t++)
        {
            flash_addr[t] = 0xFFFFFFFF;
        }
    }
#endif
}


/*****************************************************************************/
//  Description :
//      Erase the sector that contain the address. ony used for secure boot
//  function.
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/

BOOLEAN FLASH_EraseSecureOnly (
    uint32 addr)
{
#ifndef   SDRAM_SIMUL_NOR_FLASH
    BOOLEAN ret;

    FLASH_GetSemaphore();
    addr = addr & (~ (s_flash_sector_size -1)); //Keep Erase Address sector-aligned
    ret = s_Nor_Driver_ptr->_erase (addr);
    FLASH_ReleaseSemaphore();

    return ret;
#else
    {
        uint32  *flash_addr = (uint32 *) addr;
        uint32 t = 0;

        for (t= 0; t < 16*1024; t++)
        {
            flash_addr[t] = 0xFFFFFFFF;
        }
    }
#endif
}

/*****************************************************************************/
//  Description :
//      Suspend the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Suspend (void)
{
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    s_Nor_Driver_ptr->_suspend();
}

/*****************************************************************************/
//  Description :
//      Resume the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Resume (void)
{
    SCI_ASSERT (s_Nor_Driver_ptr);/*assert verified*/
    s_Nor_Driver_ptr->_resume();
}

/*****************************************************************************/
//  Description :
//  Get flash semaphore to make sure some flash operation be mutual exclusion
//
//  Global resource dependence :
//      s_flash_semaphore_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_GetSemaphore (void)
{
    SCI_ASSERT (s_flash_mutex_ptr);/*assert verified*/

    if (_tx_thread_system_state == 0)
    {
        if (SCI_ERROR == SCI_GetMutex (s_flash_mutex_ptr, SCI_WAIT_FOREVER))
        {
            //SCI_TRACE_LOW:"Flash:Get Semaphore Fail!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT,FLASH_1000_112_2_17_23_5_58_1107, (uint8 *) "");
        }
    }
}

/*****************************************************************************/
//  Description :
//  Releas flash semaphore
//
//  Global resource dependence :
//      s_flash_semaphore_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_ReleaseSemaphore (void)
{
    uint32 ret;
    SCI_ASSERT (s_flash_mutex_ptr);/*assert verified*/

    if (_tx_thread_system_state == 0)
    {
        ret = SCI_PutMutex (s_flash_mutex_ptr);
        SCI_ASSERT (ret == SCI_SUCCESS);/*assert verified*/
    }
}



#ifndef FLASH_TYPE_SPI

/*****************************************************************************/
//  Description:    This function is used to get manufacture ID and device ID
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/

PUBLIC uint32 FLASH_ReadID (uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id)
{


    uint32          b_1, b_2, b_3;
    uint32          flash_type;
    FLASH_PTR_TYPE  ptr = (FLASH_PTR_TYPE) 0x0;

    RESET_FLASH;/*lint !e413 ,the pointer is right, and its value is zero*/

    * (ptr + 0x555) = 0xAA;/*lint !e413 ,the pointer is right, and its value is zero*/
    * (ptr + 0x2AA) = 0x55;/*lint !e413 ,the pointer is right, and its value is zero*/

    // enter autoselect mode
    * (ptr + 0x555) = 0x90;/*lint !e413 ,the pointer is right, and its value is zero*/

    s_manu_id = * (ptr + 0x00);/*lint !e413 ,the pointer is right, and its value is zero*/
    s_dev_id  = * (ptr + 0x01);/*lint !e413 ,the pointer is right, and its value is zero*/
    s_ext_id  = 0x0;
    s_manu_id &= 0xFF;

    switch (s_manu_id)
    {
        case FLASH_SST_ID://sst flash.
            flash_type = FLASH_SST;
            break;

        case FLASH_INTEL_ID: //intel flash
        case FLASH_ST_ID:
            flash_type = FLASH_INTEL;
            break;

        case FLASH_SAMSUNG_ID:
            flash_type = FLASH_SAMSUNG;
            break;

        case FLASH_AMD_ID:
        case FLASH_CFEON_ID:
            b_1 = (* (ptr + 0x01)) & 0xFF;/*lint !e413 ,the pointer is right, and its value is zero*/
            b_2 = (* (ptr + 0x0E)) & 0xFF;/*lint !e413 ,the pointer is right, and its value is zero*/
            b_3 = (* (ptr + 0x0F)) & 0xFF;/*lint !e413 ,the pointer is right, and its value is zero*/
            s_dev_id = (b_1 << 16) + (b_2 << 8) + b_3;

            flash_type = FLASH_AMD;
            break;
        default:
            flash_type = FLASH_OTHER;
            break;
    }

    *pDev_id    = s_dev_id;
    *pManu_id   = s_manu_id;
    *pExtend_id = FLASH_GetExtID (s_manu_id, s_dev_id);
    s_ext_id = *pExtend_id;
    FLASH_M18_Type_Set (s_manu_id,s_dev_id,s_ext_id);
    RESET_FLASH;/*lint !e413 ,the pointer is right, and its value is zero*/

    return flash_type;
}

PUBLIC void FLASH_GetID (uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id)
{
    *pManu_id = s_manu_id;
    *pDev_id = s_dev_id;
    *pExtend_id = s_ext_id;

}
#endif
/*****************************************************************************/
//  Description:    This function is used to get flash info,
//                  and config flash struct.
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/

void FLASH_Config_struct (void)
{
    DFILE_CONFIG_T    *dfile_cfg_ptr = NULL;

    s_flash_bank_num              = s_Nor_Config_ptr->bank_num;
    s_flash_sector_num            = s_Nor_Config_ptr->sect_num;
    s_flash_file_sect_num         = s_Nor_Config_ptr->file_sect_num;
    s_flash_sector_size           = s_Nor_Config_ptr->sect_size;
    s_flash_start_addr            = s_Nor_Config_ptr->start_addr;
    s_flash_efs_start_addr        = s_Nor_Config_ptr->efs_start_addr;
    s_flash_size                  = s_Nor_Config_ptr->flash_size;
    s_flash_fixed_nvitem_addr     = s_Nor_Config_ptr->fixnv_addr;
    s_flash_product_info_addr     = s_Nor_Config_ptr->prodinfo_addr;
    s_flash_mmires_ptr            = s_Nor_Config_ptr->mmi_res;
    s_flash_umem_addr             = s_Nor_Config_ptr->umem_addr;
    s_flash_umem_size             = s_Nor_Config_ptr->umem_size;
    s_flash_end_addr              = s_flash_start_addr + s_flash_size;
    s_flash_efs_end_addr          = s_flash_efs_start_addr + (s_flash_sector_num * s_flash_sector_size);
    s_flash_fixed_nvitem_end_addr = s_flash_fixed_nvitem_addr + s_flash_sector_size;
    s_flash_lcd_sensor_addr       = s_Nor_Config_ptr->spload_addr;

    dfile_cfg_ptr = SPIFLASH_GetDFileConfig();

    if (dfile_cfg_ptr && (DFILE_INVAILD_ADDR != dfile_cfg_ptr->res_addr))
    {
        s_flash_mmires_ptr = dfile_cfg_ptr->res_addr & ~DFILE_FLAGS_MASK;
    }

    SCI_ASSERT (s_flash_mmires_ptr);/*assert verified*/
    SCI_ASSERT (s_flash_sector_num >= 2);/*assert verified*/
    SCI_ASSERT (s_flash_sector_size>= 4096);/*assert verified*/
}

/*****************************************************************************/
//  Description:    This function is used to get MMIResPtr
//  Global resource dependence:
//  s_flash_mmires_ptr
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
uint32 FLASH_GetMMIResPtr (void)
{
    return s_flash_mmires_ptr;
}

/*****************************************************************************/
//  Description:    This function is used to get lcd and sensor  load address
//  Global resource dependence:
//  s_flash_mmires_ptr
//  Author:         tao.feng
//  Note:
/*****************************************************************************/
uint32 FLASH_GetSperateLoadAddr (void)
{
    return s_flash_lcd_sensor_addr;
}

/*****************************************************************************/
//  Description :
//      Get fixed nvitem address
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/

void FLASH_GetFixedNvitemAddr (uint32 *start_addr, uint32 *end_addr)
{
    SCI_ASSERT (start_addr != NULL);/*assert verified*/
    SCI_ASSERT (end_addr   != NULL);/*assert verified*/
    *start_addr = s_flash_fixed_nvitem_addr;
    *end_addr   = s_flash_fixed_nvitem_end_addr;
}

void FLASH_GetFotaNvitemAddr (uint32 *start_addr, uint32 *end_addr)
{
    SCI_ASSERT (start_addr != NULL);/*assert verified*/
    SCI_ASSERT (end_addr   != NULL);/*assert verified*/
    *start_addr = s_flash_cfg_nv_addr;
    *end_addr   = s_flash_cfg_nv_end_addr;
}
/*****************************************************************************/
//  Description:    This function is used to get Flash start address.
//                  It equals FLASH_BASE_ADDR
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetStartAddr (void)
{
    return s_flash_start_addr;
}

/*****************************************************************************/
//  Description:    This function is used to get Flash end address.
//                  It equals FLASH_BASE_ADDR + FLASH_SIZE.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetEndAddr (void)
{
    return s_flash_end_addr;
}

/*****************************************************************************/
//  Description:    This function is used to get umem addree
//  Global resource dependence:
//  Author:         Zhou_Xiaobo
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemAddr (void)
{
    return s_flash_umem_addr;
}

/*****************************************************************************/
//  Description:    This function is used to get umem size
//  Global resource dependence:
//  Author:         Zhou_Xiaobo
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemSize (void)
{
    return s_flash_umem_size;
}

#ifdef DSP_DOWNLOAD_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to get dsp bin address
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetDspCodeAddr (void)
{
    return s_flash_dsp_code_addr;
}
#endif

/*****************************************************************************/
//  Description:    This function is used to get umem id num
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

#ifdef UMEM_SUPPORT
#ifdef UMEM_NEW_MECHNISM

PUBLIC uint32 FLASH_GetUmemIdNum (void)
{

    uint32 id_num = (FLASH_GetUmemSize() /FLASH_GetUmemVirSize (UMEM_TYPE_FS));

    if (id_num>= (NV_UDISK_MAX_NUM))
    {
        //id resource limit
        SCI_ASSERT (0); /*assert to do*/
    }

    return id_num;
}
#endif
#endif


/*****************************************************************************/
//  Description:    This function is used to get umem vir sect size
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemVirSize (UMEM_TYPE_E index)
{
#ifdef UMEM_SUPPORT
    uint32 umem_size = 0;
    uint32 vir_sec_size = 512;

    umem_size = FLASH_GetUmemSize();

    if (FLASH_M18_Type_Get())
    {
        if (umem_size > 4*1024*1024)
        {
            SCI_ASSERT (0); /*assert to do*/
        }
        else
        {
            vir_sec_size = 1024;
        }
    }
    else
    {
        if (umem_size<=0x200000)
        {
            vir_sec_size  =  512;
        }
        else if ( (umem_size>0x200000) && (umem_size<=0x400000))
        {
            vir_sec_size  =  1024;
        }
        else if ( (umem_size>0x400000) && (umem_size<=0x800000))
        {
            vir_sec_size  =  2048;
        }
        else if ( (umem_size>0x800000) && (umem_size<=0x1000000))
        {
            vir_sec_size  =  4096;
        }
        else
        {
            SCI_ASSERT (0); /*assert to do*/
        }
    }

    return vir_sec_size;
#else
    return 0xffffffff;
#endif

}


/*****************************************************************************/
//  Description:    This function is used to get
//                  flash product infomation address
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
uint32 FLASH_GetProductInfoAddr (void)
{
    return s_flash_product_info_addr;

}
/*****************************************************************************/
//  Description:    This function is used to get
//                  flash simlock data address
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
uint32 FLASH_GetSimlockDataAddr (void)
{
    return s_flash_simlock_addr;

}
/*****************************************************************************/
//  Description :
//      Check use SRAM for simulate ?
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_IsSimulate (void)
{
    return s_flash_is_simulate;
}
/*****************************************************************************/
//  Description :
//      Set the flag if erasing/programming need to be suspend
//      when system need response interrupt.
//  Global resource dependence :
//      s_flash_bank_num,s_flash_suspend_flag
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_SetSuspendFlag (BOOLEAN flag)
{
    /*
        if(s_flash_bank_num < 2)
            s_flash_suspend_flag = TRUE;
        else
            s_flash_suspend_flag = flag;
    */
    s_flash_suspend_flag = TRUE;
}
/*****************************************************************************/
//  Description :
//      get the flag if erasing/programming need to be suspend
//      when system need response interrupt.
//  Global resource dependence :
//      s_flash_bank_num
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 FLASH_GetSuspendFlag (void)
{
    return s_flash_suspend_flag;

}

PUBLIC void *FLASH_GetBzpBinAddr (void)
{
    return (void *) (SPIFLASH_GetDFileConfig());
}


PUBLIC void *FLASH_GetBzpBinAddrFromFlashB (void)
{
    return (void *) (SPIFLASH_GetDFileConfigForFlashB());
}

#ifdef FOTA_SUPPORT_ABUP
uint32 FLASH_GetAbupFotaAppAddr (void)
{
    return s_flash_abup_fota_app_addr;
}
#endif

/*****************************************************************************/
//  Description :
//     get patition end addr
//  Global resource dependence :
//
//  Author:
//      henry.he
//  Note:
//
/*****************************************************************************/
LOCAL uint32 FLASH_GetPartitionEndAddr (uint32 addr, NOR_FLASH_CONFIG_PTR cfg_ptr)
{
    uint32 i;
    uint32 partition_start_addr = addr;
    uint32 next_partition_addr = 0xFFFFFFFF;
    uint32 partition_end_addr = 0xFFFFFFFF;
    uint32 cfg_addr_count = 0;

    const uint32 *flash_cfg_addr_table[] =
    {
#ifndef STONE_IMAGE_SUPPORT
        &s_flash_mmires_ptr,
#endif
        &s_flash_fixed_nvitem_addr,
        &s_flash_product_info_addr,
        &s_flash_efs_start_addr,
        &s_flash_umem_addr,
        &s_flash_lcd_sensor_addr,
        &s_flash_end_addr,
    };

    SCI_ASSERT (addr < s_flash_end_addr); /*assert to do*/

    cfg_addr_count = sizeof (flash_cfg_addr_table) /sizeof (flash_cfg_addr_table[0]);

    for (i=0; i<cfg_addr_count; i++)
    {
        if (partition_start_addr < *flash_cfg_addr_table[i])
        {
            next_partition_addr = MIN (*flash_cfg_addr_table[i], next_partition_addr);
        }
    }

    SCI_ASSERT (next_partition_addr <= s_flash_end_addr); /*assert to do*/
    SCI_ASSERT (next_partition_addr > partition_start_addr); /*assert to do*/

    partition_end_addr = next_partition_addr - 1;

    return partition_end_addr;
}


/*****************************************************************************/
//  Description :
//     get productinfo patition size
//  Global resource dependence :
//
//  Author:
//      henry.he
//  Note:
//
/*****************************************************************************/
uint32 FLASH_GetProductInfoSize (void)
{
    uint32 ProductInfoEndAddr = 0;
    uint32 ProductInfoSize = 0;

    ProductInfoEndAddr = FLASH_GetPartitionEndAddr (FLASH_GetProductInfoAddr(), NULL);

    ProductInfoSize = ProductInfoEndAddr - FLASH_GetProductInfoAddr();

    return ProductInfoSize;

}

/*****************************************************************************/
//  Description :
//     lock ps and res patition
//  Global resource dependence :
//
//  Author:
//      henry.he
//  Note:
//
/*****************************************************************************/
LOCAL uint32 FLASH_Lock_PS_RES (NOR_FLASH_CONFIG_PTR cfg_ptr)
{
    uint32 CodePartitionEndAddr = 0;
    uint32 LockSize = 0;

    if (cfg_ptr)
    {

#ifdef NOR_BOOTLOADER_SUPPORT
        CodePartitionEndAddr = FLASH_GetPartitionEndAddr (cfg_ptr->ps_addr, cfg_ptr);
#elif MODEM_PLATFORM /*lint -e553*/
        CodePartitionEndAddr = FLASH_GetPartitionEndAddr (cfg_ptr->start_addr, cfg_ptr);
#elif STONE_IMAGE_SUPPORT
        CodePartitionEndAddr = FLASH_GetPartitionEndAddr (cfg_ptr->start_addr, cfg_ptr);
#else
        CodePartitionEndAddr = FLASH_GetPartitionEndAddr (cfg_ptr->mmi_res, cfg_ptr);
#endif


#ifdef FLASH_TYPE_SPI
        LockSize = SPIFLASH_LockRegion (cfg_ptr->start_addr, CodePartitionEndAddr);
#endif

    }

    return LockSize;
}


#ifdef FLASH_TYPE_SPI
//bringup_tmp note:

/*
此.c文件无法正确 #include "sfc_reg_uws6121e.h"
1、chip_drv.mk 已添加 MSRCPATH += chip_drv/chip_module/sfc/uws6121e 头文件路径
2、已排除头文件预编译宏冲突 #ifndef _SFC_REG_6121E_H_
*/
typedef union {
	REG32 v;
	struct {
		REG32 quad_mode : 1; // [0]
		REG32 spi_wprotect_pin : 1; // [1]
		REG32 spi_hold_pin : 1; // [2]
		REG32 __3_3 : 1;
		REG32 sample_delay : 3; // [6:4]
		REG32 __7_7 : 1;
		REG32 clk_divider : 8; // [15:8]
		REG32 cmd_quad : 1; // [16]
		REG32 tx_rx_size : 2; // [18:17]
		REG32 __31_19 : 13;
		} b;
} REG_SPI_FLASH_SPI_CONFIG_T;

#define REG_SPI_FLASH_BASE         	0x02000000
#define REG_SPI_FLASH_EXT_BASE 		0x02040000


#if (defined(MULTI_FLASH_SUPPORT) || defined(FLASH_SIZE_128MBITX32MBIT))
static void prvFlashClkInit(void)
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG1;
	REG_AP_CLK_CGM_SPIFLASH2_SEL_CFG_T CFG2;
	REG_AP_APB_CLK_AP_EN0_T clken;
	REG_AP_APB_CLK_AP_EN1_T clken1;
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk1;
	REG_AP_APB_CFG_CLK_SPIFLASH2_T ap_clk2;
	REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG;


	hwp_iomux_6121e->gpio_18  =  0X01 << 2;  //SET FUNC= FLASH
	hwp_iomux_6121e->gpio_19  =  0X01 << 2;  //SET FUNC= FLASH
	hwp_iomux_6121e->gpio_20  =  0X01 << 2;  //SET FUNC= FLASH
	hwp_iomux_6121e->gpio_21  =  0X01 << 2;  //SET FUNC= FLASH
	hwp_iomux_6121e->gpio_22  =  0X01 << 2;  //SET FUNC= FLASH
	hwp_iomux_6121e->gpio_23  =  0X01 << 2;  //SET FUNC= FLASH

	hwp_iomux_6121e->pad_gpio_18 = 0x1fe000; //SET PULL/DRV
    hwp_iomux_6121e->pad_gpio_19 = 0x2fe000;
    hwp_iomux_6121e->pad_gpio_20 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_21 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_22 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_23 = 0x3fe000;

	ap_clk1.v = hwp_apApb->cfg_clk_spiflash1;
    ap_clk1.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk1.v;

	ap_clk2.v = hwp_apApb->cfg_clk_spiflash2;
    ap_clk2.b.spiflash2_freq = 0xc;
	hwp_apApb->cfg_clk_spiflash2 = ap_clk2.v;

	clken.v = hwp_apApb->clk_ap_en0;
    clken.b.clk_en_spiflash1_fr = 1;
	clken.b.clk_en_spiflash2_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;

	CFG1.v = hwp_apClk->cgm_spiflash1_sel_cfg;
    CFG1.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG1.v;

	CFG2.v = hwp_apClk->cgm_spiflash2_sel_cfg;
    CFG2.b.cgm_spiflash2_sel = 0x4;
	hwp_apClk->cgm_spiflash2_sel_cfg = CFG2.v;

	SPI_CFG.v = (*(volatile uint32 *)(REG_SPI_FLASH_BASE + 0x14)); //hwp_spiFlash->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    (*(volatile uint32 *)(REG_SPI_FLASH_BASE + 0x14)) = SPI_CFG.v;

	SPI_CFG.v = (*(volatile uint32 *)(REG_SPI_FLASH_EXT_BASE + 0x14)); //hwp_spiFlash->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    (*(volatile uint32 *)(REG_SPI_FLASH_EXT_BASE + 0x14)) = SPI_CFG.v;

	clken1.v = hwp_apApb->clk_ap_en1;
	clken1.b.clk_en_spiflash1 = 1;
	clken1.b.clk_en_spiflash2 = 1;
	hwp_apApb->clk_ap_en1 = clken1.v;

}

#else

static void prvFlashClkInit(void) //flash1 used only
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG1;
	REG_AP_APB_CLK_AP_EN0_T clken;
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk1;
	REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG;

	ap_clk1.v = hwp_apApb->cfg_clk_spiflash1;
    ap_clk1.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk1.v;

	clken.v = hwp_apApb->clk_ap_en0;
    clken.b.clk_en_spiflash1_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;

	CFG1.v = hwp_apClk->cgm_spiflash1_sel_cfg;
    CFG1.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG1.v;

	SPI_CFG.v = (*(volatile uint32 *)(REG_SPI_FLASH_BASE + 0x14)); //hwp_spiFlash->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    (*(volatile uint32 *)(REG_SPI_FLASH_BASE + 0x14)) = SPI_CFG.v;
}


#endif



//#define FIXNV_SECTOR_NUM 3
/*****************************************************************************/
//  Description :
//     init flash config and get flash driver ptr.
//  Global resource dependence :
//      s_Normcp_spec_ptr s_Nor_Config_ptr s_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 FLASH_SystemInit (void)
{
    DFILE_CONFIG_T    *dfile_cfg_ptr = NULL;
    SPIFLASH_SPEC_T *pSpiFlashSpec = NULL;
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    SPIFLASH_SPEC_T *pSpiFlashSpec_csn = NULL;
#endif
    uint32 cs = 0;

    s_Nor_Driver_ptr = &g_spiflash_driver;

#if !defined(PLATFORM_UWS6121E)
    * (volatile uint32 *) 0x20A00204 |= 3;
#endif

	prvFlashClkInit();

    SFCDRV_CSCfg(cs);     /*create sfc reg base_addr*/

    if (SPIFLASH_ReadID (cs, &nMID, &nDID1, &nDID2) != TRUE)
    {
        return SCI_FALSE;
    }

    pSpiFlashSpec = (SPIFLASH_SPEC_T *) SPIFLASH_SPEC_Get (nMID, nDID1, nDID2);

    if (NULL == pSpiFlashSpec)
    {
        return SCI_FALSE;
    }

    s_flash_bank_num              = pSpiFlashSpec->spiflash_cfg->bank_num;
    s_flash_sector_num            = pSpiFlashSpec->spiflash_cfg->sect_num;
    s_flash_file_sect_num         = pSpiFlashSpec->spiflash_cfg->file_sect_num;
    s_flash_sector_size           = pSpiFlashSpec->spiflash_cfg->sect_size;
    s_flash_start_addr            = pSpiFlashSpec->spiflash_cfg->start_addr;
    s_flash_efs_start_addr        = pSpiFlashSpec->spiflash_cfg->efs_start_addr;
    s_flash_size                  = pSpiFlashSpec->spiflash_cfg->flash_size;
    s_flash_fixed_nvitem_addr     = pSpiFlashSpec->spiflash_cfg->fixnv_addr;
    s_flash_product_info_addr     = pSpiFlashSpec->spiflash_cfg->prodinfo_addr;
    s_flash_mmires_ptr            = pSpiFlashSpec->spiflash_cfg->mmi_res;
    s_flash_umem_addr             = pSpiFlashSpec->spiflash_cfg->umem_addr;
    s_flash_umem_size             = pSpiFlashSpec->spiflash_cfg->umem_size;
    s_flash_lcd_sensor_addr       = pSpiFlashSpec->spiflash_cfg->spload_addr;
    s_flash_simlock_addr       	  =pSpiFlashSpec->spiflash_cfg->simlock_addr;
    #ifdef FOTA_SUPPORT_ABUP
    s_flash_abup_fota_app_addr    =pSpiFlashSpec->spiflash_cfg->abup_fota_app_addr;
    s_flash_abup_fota_app_addr_end = pSpiFlashSpec->spiflash_cfg->abup_fota_app_addr + pSpiFlashSpec->spiflash_cfg->abup_fota_app_size;
    #endif
    #ifdef FOTA_SUPPORT_CMIOT
    s_flash_cmiot_fota_app_addr    =pSpiFlashSpec->spiflash_cfg->fota_addr;
    s_flash_cmiot_fota_app_addr_end = pSpiFlashSpec->spiflash_cfg->fota_addr + pSpiFlashSpec->spiflash_cfg->fota_size;
    #endif

    //s_flash_xfile_addr = pSpiFlashSpec->spiflash_cfg->xfile_addr;
    //s_flash_xfile_end_addr = s_flash_xfile_addr+ pSpiFlashSpec->spiflash_cfg->xfile_size;

   //s_flash_auto_sms_addr = pSpiFlashSpec->spiflash_cfg->auto_sms_addr;
    //s_flash_auto_sms_end_addr = s_flash_auto_sms_addr + pSpiFlashSpec->spiflash_cfg->auto_sms_size;

    s_flash_operator_data_addr = pSpiFlashSpec->spiflash_cfg->operator_data_addr;
    s_flash_operator_data_end_addr=s_flash_operator_data_addr + pSpiFlashSpec->spiflash_cfg->operator_data_size;

#ifdef CHIP_VER_UWS6121E
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    s_flash_apn_addr = pSpiFlashSpec->spiflash_cfg->apn_addr;
    s_flash_apn_end_addr=s_flash_apn_addr + pSpiFlashSpec->spiflash_cfg->apn_size;
#endif
    s_flash_cfg_nv_addr = pSpiFlashSpec->spiflash_cfg->config_nv_addr;
    s_flash_cfg_nv_end_addr=s_flash_cfg_nv_addr + pSpiFlashSpec->spiflash_cfg->config_nv_size;
    //s_flash_ims_addr = pSpiFlashSpec->spiflash_cfg->operator_data_addr;
    //s_flash_ims_end_addr=s_flash_apn_addr + pSpiFlashSpec->spiflash_cfg->operator_data_size;
#endif
    s_flash_crystal_aging_addr = pSpiFlashSpec->spiflash_cfg->crystal_aging_addr;
    s_flash_crystal_aging_end_addr =s_flash_crystal_aging_addr + pSpiFlashSpec->spiflash_cfg->crystal_aging_size;

    s_flash_ims_toggle_cfg_addr = pSpiFlashSpec->spiflash_cfg->ims_toggel_cfg_addr;
    s_flash_ims_toggle_cfg_end_addr =s_flash_ims_toggle_cfg_addr + pSpiFlashSpec->spiflash_cfg->ims_toggel_cfg_size;


#ifdef NOR_BOOTLOADER_SUPPORT
    s_flash_ps_ptr                = pSpiFlashSpec->spiflash_cfg->ps_addr;
#endif
#ifdef DSP_DOWNLOAD_SUPPORT
	s_flash_dsp_code_addr         = pSpiFlashSpec->spiflash_cfg->dsp_addr;
#endif

#ifdef SIM_LOCK_SUPPORT
    s_flash_simlock_end_addr =s_flash_simlock_addr+s_flash_sector_size;
#else
    s_flash_simlock_end_addr =s_flash_simlock_addr;
#endif
    s_flash_end_addr              = s_flash_start_addr + s_flash_size;
    s_flash_efs_end_addr          = s_flash_efs_start_addr + (s_flash_sector_num * s_flash_sector_size);
    s_flash_fixed_nvitem_end_addr = s_flash_fixed_nvitem_addr + FIXNV_SECTOR_NUM*s_flash_sector_size;

    SPIFLASH_Init (cs);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (SFC_MAX_CS - 1)
#endif
    {
        // initial flash cs1, if flash id is invalid, return directly
	*(volatile unsigned int *)(0x5010c098) = 0x1;
	*(volatile unsigned int *)(0x5010c09C) = 0x1;
	*(volatile unsigned int *)(0x5010c0A0) = 0x1;
	*(volatile unsigned int *)(0x5010c0A4) = 0x1;
	*(volatile unsigned int *)(0x5010c0A8) = 0x1;
	*(volatile unsigned int *)(0x5010c0AC) = 0x1;
        cs = 1;
        SFCDRV_CSCfg(cs);     /*create sfc reg base_addr*/

        if (SPIFLASH_ReadID (cs, &nMID, &nDID1, &nDID2) != TRUE)
        {
            return SCI_FALSE;
        }

        pSpiFlashSpec_csn = (SPIFLASH_SPEC_T *) SPIFLASH_SPEC_Get (nMID, nDID1, nDID2);

        if (NULL == pSpiFlashSpec_csn)
        {
            return SCI_FALSE;
        }

        s_flash_end_addr = pSpiFlashSpec_csn->spiflash_cfg->start_addr + pSpiFlashSpec_csn->spiflash_cfg->flash_size;

        SPIFLASH_Init (cs);

        //Retrun back to CS0 Read by default.
        cs = 0;
        SPIFLASH_ResetRegister (0);
    }

    dfile_cfg_ptr = SPIFLASH_GetDFileConfig();

    if (dfile_cfg_ptr && (DFILE_INVAILD_ADDR != dfile_cfg_ptr->res_addr))
    {
        s_flash_mmires_ptr = dfile_cfg_ptr->res_addr & ~DFILE_FLAGS_MASK;
#ifdef NOR_BOOTLOADER_SUPPORT
        s_flash_mmires_ptr += s_flash_ps_ptr;
#endif
    }

    FLASH_Lock_PS_RES (pSpiFlashSpec->spiflash_cfg);

#ifdef FOTA_SUPPORT_ABUP
    abup_get_flash_info(pSpiFlashSpec->spiflash_cfg);
#endif

    return SCI_TRUE;
}

#else
/*****************************************************************************/
//  Description :
//     init flash config and get flash driver ptr.
//  Global resource dependence :
//      s_Normcp_spec_ptr s_Nor_Config_ptr s_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/

PUBLIC uint32 FLASH_InitHAL (NORMCP_ENTITY_PTR entity_ptr)
{
    s_Normcp_spec_ptr = entity_ptr->normcp_spec_ptr;
    SCI_ASSERT (s_Normcp_spec_ptr);/*assert verified*/

    s_Normcp_config_ptr = entity_ptr->normcp_config_ptr;;
    SCI_ASSERT (s_Normcp_config_ptr);/*assert verified*/

    s_Nor_Config_ptr = entity_ptr->nor_flash_cfg_ptr;
    SCI_ASSERT (s_Nor_Config_ptr);/*assert verified*/

    if (entity_ptr->normcp_spec_ptr->flash_drv_mode == FLASH_INTEL_MODE)
    {
        s_Nor_Driver_ptr = &g_Intel_driver;
    }
    else if (entity_ptr->normcp_spec_ptr->flash_drv_mode == FLASH_AMD_MODE)
    {
        s_Nor_Driver_ptr = &g_Spansion_driver;
    }
    else if (entity_ptr->normcp_spec_ptr->flash_drv_mode == FLASH_INTEL_M18_MODE)
    {
        s_Nor_Driver_ptr = &g_Intel_m18_driver;
    }
    else
    {
        SCI_ASSERT (SCI_FALSE);/*assert verified*/
    }

    //set flash config
    //Config the flash struct
    FLASH_Config_struct();
    return SCI_TRUE;
}
#endif


uint32 g_flash_rfpara_addr;
static uint32 *gp_flash_rfpara_block1_addr, *gp_flash_rfpara_block2_addr;
#define FLASH_RFPARA_TOTAL_SIZE            (32)
#define FLASH_RFPARA_SHAREMEM_SIZE     (20)
static uint32 *FLASH_BinarySearchAddrFunc(uint32 *pfront_addr, uint32 whole_size, uint32 unit_size)
{
    uint32 *pfront, *pmid;

    pfront = pfront_addr;
   while (whole_size > FLASH_RFPARA_TOTAL_SIZE)
    {
        pmid = pfront + (whole_size >> 3);    //uint32* + value,  value need div4
        if (*pmid != 0xFFFFFFFF)
         {
            pfront = pmid;
            whole_size >>= 1;
        }
        else
        {
            whole_size >>= 1;
        }
    }
   if (*pfront == 0xFFFFFFFF)
   {
        return NULL; //no find valid data ,error blue
   }
   else
   {
        return pfront;    //return point addr
   }
}
void FLASH_GetRfParaAsCrystalAgeing(void)
{
    uint32 *pblock1, *pblock2;
    uint32 *pfront;
    uint32 size;

    pblock1 = (uint32 *)gp_flash_rfpara_block1_addr;
    pblock2 = (uint32 *)gp_flash_rfpara_block2_addr;

    if ((*pblock1 == 0xFFFFFFFF) && (*pblock2 == 0xFFFFFFFF))      /*理论上只有第一次获取时，两个block都没有值，全部是0xFFFFFFFF*/
    {
        g_flash_rfpara_addr = (uint32)pblock2 + (FLASH_SECTOR_SIZE - FLASH_RFPARA_TOTAL_SIZE);     /*设置当前地址为block2最后一个数据地址*/
    }
    else
    {
        pfront = pblock1;
        size = FLASH_SECTOR_SIZE;   //64KB
        if (*pblock2 != 0xFFFFFFFF)    //第二块有数据
        {
           pfront = pblock2;
        }
        pfront = FLASH_BinarySearchAddrFunc(pfront, size, FLASH_RFPARA_TOTAL_SIZE);
        if (pfront == NULL)
        {
            SCI_PASSERT(0, ("rfpara_1 flash_addr not finded!"));/*assert to do*/
        }

         //如果检测到是block2的尾巴，需要再检查block1的首地址是否有数
        if (((uint32)pfront + FLASH_RFPARA_TOTAL_SIZE) == ((uint32)pblock2 + size))
        {
            if (*pblock1 != 0xFFFFFFFF)    //第一块有数据
            {
             /*第二块尾巴数据不是真正的最新数据，导致该情况的原因可能是
             正在准备擦除block2时掉电，block2没有擦除*/
                pfront = pblock1;
                pfront = FLASH_BinarySearchAddrFunc(pfront, size, FLASH_RFPARA_TOTAL_SIZE);
                if (pfront == NULL)
                {
                    SCI_PASSERT(0, ("rfpara_2 flash_addr not finded!"));/*assert to do*/
                }
            }
        }
        g_flash_rfpara_addr = (uint32)pfront;
    }
    ipc_set_rfpara((uint32 *)g_flash_rfpara_addr);

}

void FLASH_WriteRfParaAsCrystalAgeing(uint32 *src_addr)
{
    uint32 *pblock1, *pblock2, *pblock;
    uint32 flash_curr_write_addr;

    pblock1 = (uint32 *)gp_flash_rfpara_block1_addr;
    pblock2 = (uint32 *)gp_flash_rfpara_block2_addr;

    if ((g_flash_rfpara_addr + FLASH_RFPARA_TOTAL_SIZE) & (FLASH_SECTOR_SIZE - 1))   //next write_addr still at current block
    {
        flash_curr_write_addr = g_flash_rfpara_addr + FLASH_RFPARA_TOTAL_SIZE;
        FLASH_Write(flash_curr_write_addr, (uint8 *)src_addr, FLASH_RFPARA_SHAREMEM_SIZE);

        //补充检查另一个block是否是擦除状态,防止由于掉电导致未擦除或擦除不彻底
        if ((g_flash_rfpara_addr & (~(FLASH_SECTOR_SIZE - 1))) == (uint32)pblock1)
        {
            pblock = pblock2;
        }
        else
        {
            pblock = pblock1;
        }
        if ((*pblock != 0xFFFFFFFF) &&  (*((uint32 *)((uint32)pblock + (FLASH_SECTOR_SIZE -FLASH_RFPARA_TOTAL_SIZE))) != 0xFFFFFFFF))    /*之前擦除并没有完成*/
        {
            FLASH_Erase((uint32)pblock);
        }
    }
    else
    {
        if ((g_flash_rfpara_addr & (~(FLASH_SECTOR_SIZE - 1))) == (uint32)pblock1)
        {
            flash_curr_write_addr = (uint32)pblock2;
            pblock = pblock1;
        }
        else
        {
            flash_curr_write_addr = (uint32)pblock1;
            pblock = pblock2;
        }
        FLASH_Write(flash_curr_write_addr, (uint8 *)src_addr, FLASH_RFPARA_SHAREMEM_SIZE);
        FLASH_Erase((uint32)pblock);
    }
    g_flash_rfpara_addr = flash_curr_write_addr;
}

#define FLASH_RFPARA_STACK_SIZE					(0x1000)
#define FLASH_RFPARA_QUEUE_NUM					(0x0010)
BLOCK_ID flash_rfpara_store_id;
static uint32 *gp_rfpara_saddr;

LOCAL void FLASH_RfParaStoreThread(uint32 argc, void *argv)
{
	xSignalHeader signal;
	BLOCK_ID    tid = SCI_IdentifyThread();

	for(;;)
	{
		signal = (xSignalHeader)SCI_GetSignal(tid);
              FLASH_WriteRfParaAsCrystalAgeing(gp_rfpara_saddr);
              ////test code
              SCI_TRACE_LOW("rfpara flash addr=0x%x,1=0x%x,2=0x%x,3=0x%x,4=0x%x,5=0x%x",g_flash_rfpara_addr,*((uint32 *)g_flash_rfpara_addr),
                                          *((uint32 *)(g_flash_rfpara_addr+4)), *((uint32 *)(g_flash_rfpara_addr+8)), *((uint32 *)(g_flash_rfpara_addr+12)),*((uint32 *)(g_flash_rfpara_addr+16)));
              SCI_FREE(signal);
		signal = NULL;
	}
}

void FLASH_RfParaStoreCtrl(uint32 *saddr)
{
#if defined(CHIP_VER_UWS6121E)
        xSignalHeader signal;

        gp_rfpara_saddr = saddr;

        // send to upper level notify cable connect
        signal = SCI_ALLOC (sizeof (*signal));
        signal->SignalCode = 1;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, flash_rfpara_store_id);
#endif
}

PUBLIC void FLASH_RfParaStoreThreadInit()
{
#if defined(CHIP_VER_UWS6121E)
       gp_flash_rfpara_block1_addr = (uint32 *)SPIFLASH_GetCrystalAgingAddress();
       gp_flash_rfpara_block2_addr = (uint32 *)(SPIFLASH_GetCrystalAgingAddress() + FLASH_SECTOR_SIZE);
       FLASH_GetRfParaAsCrystalAgeing();

	flash_rfpara_store_id = SCI_CreateThread("T_RfParaFS_HANDLER",
									"Q_RFFS",
									FLASH_RfParaStoreThread,
									0,
									0,
									FLASH_RFPARA_STACK_SIZE,
									FLASH_RFPARA_QUEUE_NUM,
									100,//PRI_USB_VBUS,
									SCI_PREEMPT,
									SCI_AUTO_START);
#endif
}

#pragma arm section code

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*      End flash.c       */


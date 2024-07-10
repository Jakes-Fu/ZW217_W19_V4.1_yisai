/******************************************************************************
 ** File Name:      fdl_update_fixnv.c                                                *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           27/06/2002                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/06/2002     Leo.Feng         Create.                                   *
 ** 11/12/2003     Zhemin.Lin       Enhanced according to cr6198              *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "fdl_main.h"
#include "flash.h"
#include "flash_drv.h"
#include "fdl_crc.h"
#include "update_fixnv.h"
#include "mem_cfg.h"
#include "common.h"
//#define FIXNV_SECTOR_NUM 2

#if defined(PLATFORM_UWS6121E)
#define FDL_SendTrace rprintf
#define FDL_SendErrLog rprintf
#endif

/**---------------------------------------------------------------------------*
 **          The sector structure and item structure                          *
 **               ( The unit is word(16bits) )                                *

      +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ --> 0x0
      | First Item                                                      |
      |        +-------------------------------------------------+ -----|--->base_addr
      |        | the ID of this item. ( 0 or valid item )        |      |
      |        |-------------------------------------------------|      |
      |        |length (include backup ID, lenth and content) |      |
      |        |-------------------------------------------------|      |
      |        |content    |      |
      |        |-------------------------------------------------|      |
      |         .................................................       |
      +-----------------------------------------------------------------+ --> word boundary.
      | Another Item.                                                   |
      |                                                                 |
      | All the item head field and following area are 0xFFFF indicate  |
      |     free space                                                  |
      +-----------------------------------------------------------------+
      |                                                                 |
      |   ..................  Free Space  ..........................    |
      |                                                                 |
      +-----------------------------------------------------------------+ --> 0xFFF8 ( If sector size is 64K )
      |   System Reserved Space.                                        |
      |        +-------------------------------------------------+      |
      |        | fixnv length. (uint16)                 . |      |
      |        |                                                 |      |
      |        |-------------------------------------------------|      |
      |        | reserved   (uint16)                             |      |
      |        |-------------------------------------------------|      |
      |        | reserved   (uint16)                             |      |
      |        |                                                 |      |
      |        |-------------------------------------------------|      |
      |        |     sector status (uint16)                      |      |
      |        |                                                 |      |
      |        +-------------------------------------------------+      |
      +-----------------------------------------------------------------+
 **---------------------------------------------------------------------------*/
#define	FIX_NV_LEN_OFFSET           (16)
#define PRD_TAIL_INFO_LEN           (8)
#define PRD_INFO_CRC_OFFSET         (6)
#define PRD_INFO_OFFSET_ADDR     (12)
#define	PRD_STATUS_OFFSET           (2)
#define SECTOR_STATUS_OFFSET        (2)
#if defined(PLATFORM_UWS6121E)
#define FLASH_START_ADRESS          (0x0)
#elif defined(PLATFORM_SC6531EFM) ||
#define FLASH_START_ADRESS          (0x10000000)
#define	SLAVE_SECTOR_BUF_ADDESS     (0x14180000)
#else
#define FLASH_START_ADRESS          (0x30000000)
#define	SLAVE_SECTOR_BUF_ADDESS     (0x34180000)
#endif
#define INVALID_ADDR            (0xFFFFFFFF)

#define FIX_NV_IMEI_FLAG_OFFSET     (4)
#define FIX_NV_IMEI_FLAG_MAGIC      (0x13e1)
#if defined(PLATFORM_UWS6121E)
//#define SRAM_START_ADDR         (0x800000)  ////move to common.h
#define SRAM_SIZE				(0x40000)
#elif defined(PLATFORM_SC6531EFM)
#define SRAM_START_ADDR         (0x14100000)
#else
#define SRAM_START_ADDR         (0x34100000)
#endif
#define SECTOR_GOOD_STATUS        (0x55aa)   // The sector is "GOOD"
#define FLASH_SIZE_2MB              (0x200000)

#define INVALID_VALUE         (0xFFFFFFFF)
#define INVALID_VALUE_U16     (0xFFFF)

extern uint32    s_intel_m18_serial;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
static uint32	master_address=0,slave_address=0,sector_size=0;

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

extern unsigned long g_checksum;
extern sys_stage_s      global_sys_stage;

extern FLASH_PARTITION_PTR    s_flash_partition_info_ptr ;

extern unsigned long Get_CheckSum (const unsigned char *src, int len);
extern NOR_FLASH_CONFIG_PTR get_flash_partition_info_InPS(void);
extern uint16 calculate_fixNV_length( uint32  start_addr, uint32 end_addr);


uint32 get_valid_nv_address(void);


void clean_slave_sector (uint32 slave_address,uint32 efs_start_address,uint32 efs_size)
{
    uint16 status;
    uint32 slave_id;
    uint8  h_byte;
    uint8  l_byte;
    uint32 sector_address;

    READ_SPARE_STATUS (status,slave_address);

    if (status == 0)
    {
        return;
    }

    h_byte = status>>8;
    l_byte = status & 0xff;

    if ( (h_byte | l_byte) != 0xff)
    {
        return;
    }

    if ( (h_byte != 0xff) && (h_byte < efs_size/sector_size))
    {
        sector_address = (efs_start_address + h_byte*sector_size);

        if (sector_address != slave_address)
        {
            FDL_FlashErase (sector_address,sector_size);
            FDL_FlashWriteArea (sector_address, (char *) slave_address,sector_size);
            slave_id =0;
            WRITE_SPARE_STATUS (slave_id);
        }
    }
}

void initialize_nv_status (NOR_FLASH_CONFIG_PTR ptr,uint32 base_address)
{
    uint32 efs_size;
    uint32 efs_start_address;

    if (ptr)
    {
        sector_size = ptr->sect_size ;
        master_address = ptr->fixnv_addr + base_address;
        efs_start_address = ptr->efs_start_addr + base_address;
		efs_size = ptr->sect_size* ptr->sect_num;
		slave_address = efs_start_address ;

    }
}


#ifdef FLASH_TYPE_SPI

LOCAL uint16 fixnv_crc = INVALID_VALUE_U16;

/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
LOCAL BOOLEAN verify_fixnv(uint32 addr)
{
    uint16 fixnv_status;
    uint16 prod_sec_dedicate_crc = 0;
    uint16 prod_sec_figure_crc = 1;
    uint16 prod_sec_figure_crc_old = 1;
    uint16 fixnv_dedicate_crc = 0;
    uint16 fixnv_figure_crc = 1;
    uint32 calc_fixnv_len;
    uint32 prod_crc_length;
    uint32 productinfo_offset;
    uint32 fixnv_length = 0;
    uint32 fixnv_address = addr;

#if defined(PLATFORM_UWS6121E)
    return SCI_TRUE;
#endif

	//这个判断在这里需要修改，因为UIS8910的FLASH空间为0x60000000永远大于0x800000的内存空间
    if ((fixnv_address >= SRAM_START_ADDR) && fixnv_address < (SRAM_START_ADDR + SRAM_SIZE))
    {
        fixnv_status = *(uint16 *)(fixnv_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET);
    }
    else
    {
        fixnv_status = FDL_FlashReadWord(fixnv_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET);
    }

    if ((fixnv_status != NV_BOTH_INVALID) && (fixnv_status != NV_MOK_SINVALID)
            && (fixnv_status != NV_MOK_SOK) && (fixnv_status != NV_MINVALID_SOK)
            && (fixnv_status != NV_SOK_PRD_INFO) && (fixnv_status != NV_BOTH_INVALID_OLD)
            && (fixnv_status != NV_MINVALID_SOK_OLD) && (fixnv_status != NV_MOK_SINVALID_OLD)
            && (fixnv_status != NV_MOK_SOK_OLD) && (fixnv_status != NV_SOK_PRD_INFO_OLD)
       )
    {
        //FDL_SendTrace("STATUS ERR! %s %d fixnv_status=0x%x, fixnv_address=0x%x",
                      //__func__, __LINE__, fixnv_status, fixnv_address);

        return SCI_FALSE;
    }

    if ((fixnv_address >= SRAM_START_ADDR) && fixnv_address < (SRAM_START_ADDR + SRAM_SIZE))
    {
        fixnv_length = *(uint16 *)(fixnv_address + FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET);
    }
    else
    {
        fixnv_length = FDL_FlashReadWord(fixnv_address + FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET);
    }

    calc_fixnv_len = calculate_fixNV_length(fixnv_address, fixnv_address + FIXNV_SECTOR_NUM*sector_size);

    if ((fixnv_length > FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET)
            || (fixnv_length <= sizeof(uint16))
            || (fixnv_length != calc_fixnv_len)
       )
    {
        FDL_SendTrace("LENGTH ERROR! %s %d fixnv_length=0x%x, fixnv_address=0x%x, calc_fixnv_len=0x%x",
                      __func__, __LINE__, fixnv_length, fixnv_address, calc_fixnv_len);

        return SCI_FALSE;
    }

    if ((fixnv_address >= SRAM_START_ADDR) && fixnv_address < (SRAM_START_ADDR + SRAM_SIZE))
    {
        fixnv_dedicate_crc = *(uint16 *)(fixnv_address);
    }
    else
    {
        fixnv_dedicate_crc = FDL_FlashReadWord(fixnv_address);
    }

    if ((INVALID_VALUE_U16 != fixnv_crc)
            && (fixnv_address >= SRAM_START_ADDR)
        )
    {
        if ((fixnv_dedicate_crc != fixnv_crc)
                 && (fixnv_dedicate_crc != EndianConv_16(fixnv_crc))
            )
        {
            FDL_SendTrace("CRC ERROR! %s %d fixnv_dedicate_crc=0x%x, fixnv_crc=0x%x, fixnv_address=0x%x",
                          __func__, __LINE__, fixnv_dedicate_crc, fixnv_crc, fixnv_address);

            return SCI_FALSE;
        }
    }
    else
    {
        fixnv_figure_crc = crc16(0, (uint8 const *)fixnv_address + sizeof(uint16), fixnv_length - sizeof(uint16));

        if ((fixnv_dedicate_crc != fixnv_figure_crc)
                && (fixnv_dedicate_crc != EndianConv_16(fixnv_figure_crc))
           )
        {
            FDL_SendTrace("figure_crc ERROR! %s %d fixnv_dedicate_crc=0x%x, fixnv_figure_crc=0x%x, fixnv_address=0x%x",
                          __func__, __LINE__, fixnv_dedicate_crc, fixnv_figure_crc, fixnv_address);

            return SCI_FALSE;
        }
    }

    if ((fixnv_address >= SRAM_START_ADDR) && fixnv_address < (SRAM_START_ADDR + SRAM_SIZE))
    {
        prod_sec_dedicate_crc = *(uint16 *)(fixnv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET);
        productinfo_offset = *(uint32 *)(fixnv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_OFFSET_ADDR);
    }
    else
    {
        prod_sec_dedicate_crc = FDL_FlashReadWord(fixnv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET);
        productinfo_offset = FDL_FlashReadDWord (fixnv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_OFFSET_ADDR);
    }

    if (prod_sec_dedicate_crc != INVALID_VALUE_U16)
    {
        //productinfo_offset = Get_productinfo_offset();
        prod_crc_length = FIXNV_SECTOR_NUM*sector_size - productinfo_offset - FIX_NV_LEN_OFFSET;

        if ((prod_crc_length >= FIXNV_SECTOR_NUM*sector_size) || (prod_crc_length < FIX_NV_LEN_OFFSET))
        {
            FDL_SendErrLog("ERROR! %s %d prod_crc_length=0x%x", __func__, __LINE__, prod_crc_length);
        }

        prod_sec_figure_crc = crc16(0, fixnv_address + productinfo_offset, prod_crc_length);

        if ((prod_sec_dedicate_crc != prod_sec_figure_crc)
                && (prod_sec_dedicate_crc != EndianConv_16(prod_sec_figure_crc))
           )
        {
            prod_sec_figure_crc_old = crc16(0, fixnv_address + sizeof(uint16), FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET);

            if ((prod_sec_dedicate_crc == prod_sec_figure_crc_old)
                    || (prod_sec_dedicate_crc == EndianConv_16(prod_sec_figure_crc_old))
                )
            {
                return SCI_TRUE;
            }

            FDL_SendTrace("ERROR! %s %d prod_sec_dedicate_crc=0x%x prod_sec_figure_crc=0x%x fixnv_address=0x%x prod_sec_figure_crc_old=0x%x",
                          __func__, __LINE__, prod_sec_dedicate_crc, prod_sec_figure_crc, fixnv_address, prod_sec_figure_crc_old);

            return SCI_FALSE;
        }
    }
    else
    {
        NV_STATUS_TYPE_E  fixnv_status;

        if ((fixnv_address >= SRAM_START_ADDR) && fixnv_address < (SRAM_START_ADDR + SRAM_SIZE))
        {
            fixnv_status = *(uint16 *)(fixnv_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET);
        }
        else
        {
            fixnv_status = FDL_FlashReadWord(fixnv_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET);
        }

        if (fixnv_status == NV_SOK_PRD_INFO)
        {
            FDL_SendTrace("PROD STATUS ERROR! %s %d fixnv_status=0x%x, fixnv_address=0x%x",
                          __func__, __LINE__, fixnv_status, fixnv_address);

            return SCI_FALSE;
        }
    }

    //FDL_SendTrace("fixnv_crc=0x%x, fixnv_dedicate_crc=0x%x, fixnv_figure_crc=0x%x, fixnv_address=0x%x",
    //                fixnv_crc, fixnv_dedicate_crc, fixnv_figure_crc, fixnv_address);

    return SCI_TRUE;
}

/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      master_address, slave_address, sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
uint32	update_fixnv(char * fix_nv_address, uint32 fix_nv_size)
{
    uint32 explore_fixnv_addr = INVALID_ADDR;
    uint32 valide_fixnv_addr=INVALID_ADDR;
    uint32 calc_fixnv_len;
    uint32 prod_crc_length;
    uint32 productinfo_offset = 0xFFFFFFFF;
    uint32 min_productinfo_length;
    uint16 prod_sec_dedicate_crc = 0;
    uint16 prod_sec_figure_crc = 1;
    uint32 valid_prod_offset_addr = 0;
    uint16 valid_productinfo_crc = 0;
    BOOLEAN is_dload_phasecheck;

    // 这里假设上层函数改写了FIXNV 但没有改写 CRC，所以重写CRC
    // fixnv 起始地址的两字节存放 fixnv 的 CRC
    // PC tool 传来CRC 是大端，这里改成小端
    fixnv_crc = * (uint16 *) fix_nv_address ;
    fixnv_crc = EndianConv_16(fixnv_crc);
    * (uint16 *) fix_nv_address = fixnv_crc;

    * ((uint16 *) &fix_nv_address[FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET]) = NV_MINVALID_SOK;
	/* To be compatible with the latest nv tool,
	add the following code snippet, @vine 2016.10.27 */
	if ( fix_nv_size >= 0x8 &&
		*((uint32*)&fix_nv_address[fix_nv_size - 8]) == 0xFFFFFFFF &&
		*((uint32*)&fix_nv_address[fix_nv_size - 4]) == 0xFFFFFFFF )
	{
		fix_nv_size -= 0x8;
		*(uint16 *)fix_nv_address = crc16(0, fix_nv_address + sizeof(uint16),
			fix_nv_size - sizeof(uint16));
	}

    * ((uint32 *) &fix_nv_address[FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET]) = fix_nv_size;
    * ((uint16 *) &fix_nv_address[fix_nv_size]) = INVALID_VALUE_U16;

    productinfo_offset = Get_productinfo_offset();
    *(uint32 *)(fix_nv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_OFFSET_ADDR) = productinfo_offset;

    // 假设上层函数改写了FIXNV 但没有改写CRC 和状态位，所以只检查FIXNV 长度和结构
    // 注意: calculate_fixNV_length 函数会依赖 FIX_NV_LEN_OFFSET 写入的 fix_nv_size
    calc_fixnv_len = calculate_fixNV_length((uint32)fix_nv_address, (uint32)fix_nv_address + FIXNV_SECTOR_NUM*sector_size);

    if ((fix_nv_size > FIXNV_SECTOR_NUM*sector_size - FIX_NV_LEN_OFFSET)
            || (fix_nv_size <= sizeof(uint16))
            || (fix_nv_size != calc_fixnv_len)
       )
    {
        FDL_SendErrLog("LENGTH ERROR! %s %d fix_nv_size=0x%x, fix_nv_address=0x%x, calc_fixnv_len=0x%x",
                      __func__, __LINE__, fix_nv_size, fix_nv_address, calc_fixnv_len);

        return 0;
    }

    is_dload_phasecheck = Get_phasecheck_flag();

    //FDL_SendTrace("%s %d %d 0x%x 0x%x", __func__, __LINE__, is_dload_phasecheck, calc_fixnv_len, fix_nv_address);

    if (!is_dload_phasecheck)
    {
        if (verify_fixnv(master_address))
        {
             valide_fixnv_addr=master_address;
        }
        else
        {
         // nor flash, pc tool has no phase check download, product info need back up, if the product info in new upgrade software
        // is different with the current flash in phone  we should read out from the phone then update
           NOR_FLASH_CONFIG_PTR flash_partition_info_in_ps = NULL;
           flash_partition_info_in_ps = get_flash_partition_info_InPS();

            if (flash_partition_info_in_ps != NULL){
                explore_fixnv_addr = flash_partition_info_in_ps->fixnv_addr + FLASH_START_ADRESS;
                if (verify_fixnv(explore_fixnv_addr)){
                    valide_fixnv_addr = explore_fixnv_addr;
                }else{
                    explore_fixnv_addr = flash_partition_info_in_ps->efs_start_addr + FLASH_START_ADRESS;
                    if (verify_fixnv(explore_fixnv_addr)){
                        valide_fixnv_addr = explore_fixnv_addr;
                    }
                }
            }
        }
        FDL_memset((void *)(fix_nv_address + fix_nv_size), 0xFF, FIXNV_SECTOR_NUM*sector_size - fix_nv_size - FIX_NV_LEN_OFFSET);
#if 1
        prod_crc_length = FIXNV_SECTOR_NUM*sector_size - productinfo_offset - FIX_NV_LEN_OFFSET;
        if ((prod_crc_length >= FIXNV_SECTOR_NUM*sector_size) || (prod_crc_length < FIX_NV_LEN_OFFSET))
        {
            FDL_SendErrLog("ERROR! %s %d prod_crc_length=0x%x", __func__, __LINE__, prod_crc_length);
        }
        prod_sec_figure_crc = crc16(0, fix_nv_address + productinfo_offset, prod_crc_length);
        *(uint16 *)(fix_nv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET) = prod_sec_figure_crc;
#else
        *(uint16 *)(fix_nv_address + sector_size - PRD_INFO_CRC_OFFSET) = INVALID_VALUE;
#endif
        if (valide_fixnv_addr !=INVALID_ADDR)
        {
            valid_productinfo_crc = FDL_FlashReadWord(valide_fixnv_addr + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET);
            valid_prod_offset_addr = FDL_FlashReadDWord(valide_fixnv_addr + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_OFFSET_ADDR);

            if( (valid_productinfo_crc != INVALID_VALUE_U16)
                && (valid_prod_offset_addr != INVALID_VALUE)
            )
            {
                // 如果升级软件和flash里保存的valid_prod_offset_addr 不一样，productinfo 要能读写到新的地址。
                if ((uint16)productinfo_offset == valid_prod_offset_addr)
                {
                    memcpy((void *)((uint32)fix_nv_address + productinfo_offset),
                	        (void *)(valide_fixnv_addr + productinfo_offset),
                            FIXNV_SECTOR_NUM*sector_size - productinfo_offset - FIX_NV_LEN_OFFSET);
                }
                else
                {


                    min_productinfo_length = MIN(FIXNV_SECTOR_NUM*sector_size - valid_prod_offset_addr, FIXNV_SECTOR_NUM*sector_size - productinfo_offset);
                    min_productinfo_length -= FIX_NV_LEN_OFFSET;

                    memcpy((void *)((uint32)fix_nv_address + productinfo_offset),
                	        (void *)(valide_fixnv_addr + valid_prod_offset_addr),
                            min_productinfo_length);
                }

                prod_crc_length = FIXNV_SECTOR_NUM*sector_size - productinfo_offset - FIX_NV_LEN_OFFSET;

                if ((prod_crc_length >= FIXNV_SECTOR_NUM*sector_size) || (prod_crc_length < FIX_NV_LEN_OFFSET))
                {
                    FDL_SendErrLog("ERROR! %s %d prod_crc_length=0x%x", __func__, __LINE__, prod_crc_length);
                }

                prod_sec_figure_crc = crc16(0, fix_nv_address + productinfo_offset, prod_crc_length);
                *(uint16 *)(fix_nv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET) = prod_sec_figure_crc;
            }
        }
    }
    else
    {
        // 上层改写 productinfo 不会更新productinfo CRC，所以重写CRC
        prod_crc_length = FIXNV_SECTOR_NUM*sector_size - productinfo_offset - FIX_NV_LEN_OFFSET;

        if ((prod_crc_length >= FIXNV_SECTOR_NUM*sector_size) || (prod_crc_length < FIX_NV_LEN_OFFSET))
        {
            FDL_SendErrLog("ERROR! %s %d prod_crc_length=0x%x", __func__, __LINE__, prod_crc_length);
        }

        prod_sec_figure_crc = crc16(0, fix_nv_address + productinfo_offset, prod_crc_length);
        *(uint16 *)(fix_nv_address + FIXNV_SECTOR_NUM*sector_size - PRD_INFO_CRC_OFFSET) = prod_sec_figure_crc;
    }

    //FDL_SendTrace("%s %d %d", __func__, __LINE__, verify_fixnv((uint32)fix_nv_address));

    if (FDL_FlashWriteArea(slave_address, fix_nv_address, FIXNV_SECTOR_NUM*sector_size) == FIXNV_SECTOR_NUM*sector_size)
    {
        if (!verify_fixnv(slave_address))
        {
            FDL_SendErrLog("ERROR! %s %d", __func__, __LINE__);
            return 0;
        }

        * ((uint16 *) &fix_nv_address[FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET]) = NV_MOK_SOK;

        if (FDL_FlashWriteArea(master_address, (char *) fix_nv_address, FIXNV_SECTOR_NUM*sector_size) == FIXNV_SECTOR_NUM*sector_size)
        {
            if (!verify_fixnv(master_address))
            {
                FDL_SendErrLog("ERROR! %s %d", __func__, __LINE__);
                return 0;
            }

            FDL_FlashWriteWord(slave_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
            FDL_FlashWriteWord(master_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
            FDL_FlashErase_Block(slave_address, FIXNV_SECTOR_NUM*sector_size);
            FDL_FlashWriteWord(slave_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, SECTOR_GOOD_STATUS);

            return 1;
        }
    }

    return 0;
}

/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
void update_sector_by_slave(void)
{
    uint16 valid_fixnv_status;
    uint32 valid_fixnv_addr = INVALID_ADDR;
    uint8 * sector_buf_ptr = (uint8 *)SRAM_START_ADDR;

    valid_fixnv_addr = get_valid_nv_address();

    rprintf("%s %d valid_fixnv_addr=0x%x, slave_address=0x%x, master_address=0x%x",
                  __func__, __LINE__, valid_fixnv_addr, slave_address, master_address);

    if (INVALID_ADDR == valid_fixnv_addr)
    {
        FDL_SendTrace("I can not find valid fixnv! %s %d", __func__, __LINE__);
        return;
    }

    valid_fixnv_status = FDL_FlashReadWord(valid_fixnv_addr + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET);

    if (valid_fixnv_addr == master_address)
    {
        FDL_SendTrace("%s %d valid_fixnv_addr=0x%x", __func__, __LINE__, valid_fixnv_addr);

        if ((valid_fixnv_status == NV_SOK_PRD_INFO)
                || (valid_fixnv_status == NV_MINVALID_SOK)
                || (valid_fixnv_status == NV_MOK_SOK)
           )
        {
            FDL_FlashWriteWord(master_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
        }
    }
    else
    {
        // 如果slave 是有效的，就拷贝slave 到master
        if ((valid_fixnv_status == NV_SOK_PRD_INFO)
                || (valid_fixnv_status == NV_MINVALID_SOK)
                || (valid_fixnv_status == NV_MOK_SOK)
                || (valid_fixnv_status == NV_MINVALID_SOK_OLD)
                || (valid_fixnv_status == NV_MOK_SINVALID_OLD)
                || (valid_fixnv_status == NV_MOK_SOK_OLD)
                || (valid_fixnv_status == NV_SOK_PRD_INFO_OLD)
           )
        {
            FDL_FlashRead(valid_fixnv_addr, sector_buf_ptr, FIXNV_SECTOR_NUM*sector_size);

            *(uint16 *)(sector_buf_ptr + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET) = NV_MOK_SOK;

            if (FDL_FlashWriteArea(master_address, (char *) sector_buf_ptr, FIXNV_SECTOR_NUM*sector_size) == FIXNV_SECTOR_NUM*sector_size)
            {
                if (!verify_fixnv(master_address))
                {
                    FDL_SendErrLog("ERROR! %s %d", __func__, __LINE__);
                }

                if ((valid_fixnv_status == NV_SOK_PRD_INFO)
                        || (valid_fixnv_status == NV_MINVALID_SOK)
                        || (valid_fixnv_status == NV_MOK_SOK)
                    )
                {
                    FDL_FlashWriteWord(valid_fixnv_addr + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
                }

                FDL_FlashWriteWord(master_address + FIXNV_SECTOR_NUM*sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
                FDL_FlashErase_Block(valid_fixnv_addr, FIXNV_SECTOR_NUM*sector_size);
            }
        }
    }

    return;
}

extern uint32 flash_cs0_size;
extern uint32 flash_cs1_size;

/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
extern void rprintf(const char *fmt, ...);

uint32 get_valid_nv_address(void)
{
    uint32 i;
    uint32 explore_fixnv_addr = INVALID_ADDR;
    static uint32 valide_fixnv_addr = INVALID_ADDR;
    NOR_FLASH_CONFIG_PTR flash_partition_info_in_ps = NULL;

    if ((valide_fixnv_addr != INVALID_ADDR)
            && (fixnv_crc == INVALID_VALUE_U16)
        )
    {
        if (verify_fixnv(valide_fixnv_addr))
        {
            return valide_fixnv_addr;
        }
    }

    if (fixnv_crc != INVALID_VALUE_U16)
    {
        explore_fixnv_addr = s_flash_partition_info_ptr->NvAdress;

        if (verify_fixnv(explore_fixnv_addr))
        {
            valide_fixnv_addr = explore_fixnv_addr;
            return valide_fixnv_addr;
        }
        else
        {
            FDL_SendErrLog("%s %d fixnv_crc=0x%x explore_fixnv_addr=0x%x",
                            __func__, __LINE__, fixnv_crc, explore_fixnv_addr);

            return INVALID_ADDR;
        }
    }

    flash_partition_info_in_ps = get_flash_partition_info_InPS();

    if (flash_partition_info_in_ps != NULL)
    {
        explore_fixnv_addr = flash_partition_info_in_ps->fixnv_addr + FLASH_START_ADRESS;

        if (verify_fixnv(explore_fixnv_addr))
        {
            valide_fixnv_addr = explore_fixnv_addr;
            return valide_fixnv_addr;
        }

        explore_fixnv_addr = flash_partition_info_in_ps->efs_start_addr + FLASH_START_ADRESS;

        if (verify_fixnv(explore_fixnv_addr))
        {
            valide_fixnv_addr = explore_fixnv_addr;
            return valide_fixnv_addr;
        }
    }

    if (fixnv_crc == INVALID_VALUE_U16)
    {
        explore_fixnv_addr = s_flash_partition_info_ptr->NvAdress;

        if (verify_fixnv(explore_fixnv_addr))
        {
            valide_fixnv_addr = explore_fixnv_addr;
            return valide_fixnv_addr;
        }
    }

    explore_fixnv_addr = s_flash_partition_info_ptr->EfsAdress;

    if (verify_fixnv(explore_fixnv_addr))
    {
        valide_fixnv_addr = explore_fixnv_addr;
        return valide_fixnv_addr;
    }

    for(i=1; i<((flash_cs0_size + flash_cs1_size)/sector_size); i++)
    {
        explore_fixnv_addr = (flash_cs0_size + flash_cs1_size + FLASH_START_ADRESS) - (i * sector_size);

        if (flash_partition_info_in_ps != NULL)
        {
            if ((explore_fixnv_addr == flash_partition_info_in_ps->fixnv_addr + FLASH_START_ADRESS)
                    || (explore_fixnv_addr == flash_partition_info_in_ps->efs_start_addr + FLASH_START_ADRESS)
               )
            {
                continue;
            }
        }

        if ((explore_fixnv_addr == s_flash_partition_info_ptr->EfsAdress)
                || (explore_fixnv_addr == s_flash_partition_info_ptr->NvAdress)
           )
        {
            continue;
        }

        if (verify_fixnv(explore_fixnv_addr))
        {
            valide_fixnv_addr = explore_fixnv_addr;
            return valide_fixnv_addr;
        }

    }

//    FDL_SendTrace("%s %d explore_fixnv_addr=0x%x valide_fixnv_addr=0x%x",
//                    __func__, __LINE__, explore_fixnv_addr, valide_fixnv_addr);

    return INVALID_ADDR;
}

#else

void prepare_slave_image (char *fix_nv_address,uint16 fix_nv_size)
{
    * ( (uint16 *) &fix_nv_address[sector_size - SECTOR_STATUS_OFFSET]) = NV_MINVALID_SOK;
    * ( (uint16 *) &fix_nv_address[sector_size - FIX_NV_LEN_OFFSET]) = fix_nv_size;
	*((uint16 *) &fix_nv_address[sector_size - FIX_NV_IMEI_FLAG_OFFSET]) = 0xFFFF;
	if(s_intel_m18_serial == 1)
	{
		*((uint16 *)&fix_nv_address[sector_size - FIX_NV_IMEI_FLAG_OFFSET]) = FIX_NV_IMEI_FLAG_MAGIC;
	}

	*((uint16 *)&fix_nv_address[fix_nv_size]) = 0XFFFF;
}

uint32	update_fixnv(char * fix_nv_address, uint32 fix_nv_size)
{
    uint16 fixnv_crc = 0;
    uint16 new_fixnv_crc = 0;
	uint32 valid_fixnv_nv_addr;

    fixnv_crc = * (uint16 *) fix_nv_address ;
    fixnv_crc = EndianConv_16(fixnv_crc);   //The first two bytes is CRC Value, which come from PC Tool,and is big endian
	* (uint16 *) fix_nv_address = fixnv_crc; //After Changing CRC value from big endian to little endian,
	                                         //the new CRC value should replace the previous value.


    if (fix_nv_size > sector_size-FIX_NV_LEN_OFFSET)
    {
        return 0;
    }
	if(fix_nv_size < sizeof(uint16))
	{
		return 0;
	}
    new_fixnv_crc =   crc16 (0, fix_nv_address+sizeof (uint16),fix_nv_size-sizeof (uint16));

    if ((new_fixnv_crc != fixnv_crc) &&
		(new_fixnv_crc != EndianConv_16(fixnv_crc)))
    {
        return 0;
    }

	prepare_slave_image(fix_nv_address,fix_nv_size);
	if(fix_nv_size != calculate_fixNV_length((uint32)fix_nv_address, sector_size + (uint32)fix_nv_address))
	{
		return 0;
	}

    // now write fix nv to slave sector.  slave is still invalid .
	valid_fixnv_nv_addr = get_valid_nv_address();
	if(valid_fixnv_nv_addr == slave_address)
    {
        * ( (uint16 *) &fix_nv_address[sector_size - SECTOR_STATUS_OFFSET]) = NV_MOK_SINVALID;

        if (FDL_FlashWriteArea (master_address, (char *) fix_nv_address, sector_size) == sector_size)
        {
            new_fixnv_crc = crc16 (0, master_address+sizeof (uint16),fix_nv_size-sizeof (uint16));

            if ((new_fixnv_crc != fixnv_crc) &&
				(new_fixnv_crc != EndianConv_16(fixnv_crc)))
            {
                return 0;
            }

		    FDL_FlashErase(slave_address,sector_size);
            return 1;
        }
    }
    else
    {

        if (FDL_FlashWriteArea (slave_address, fix_nv_address, sector_size) == sector_size)
        {
			FDL_FlashRead(slave_address, (uint8 *)fix_nv_address, fix_nv_size);
			new_fixnv_crc = crc16(0, fix_nv_address+sizeof(uint16),fix_nv_size-sizeof(uint16));

            if ((new_fixnv_crc != fixnv_crc) &&
				(new_fixnv_crc != EndianConv_16(fixnv_crc)))
            {
                FDL_FlashErase (slave_address,sector_size);
                return 0;
            }

            * ( (uint16 *) &fix_nv_address[sector_size - SECTOR_STATUS_OFFSET]) = NV_MOK_SINVALID;

            if (FDL_FlashWriteArea (master_address, (char *) fix_nv_address, sector_size) == sector_size)
            {

                new_fixnv_crc = crc16 (0, master_address+sizeof (uint16),fix_nv_size-sizeof (uint16));

                if ((new_fixnv_crc != fixnv_crc) &&
					(new_fixnv_crc != EndianConv_16(fixnv_crc)))
                {
                    return 0;
                }
				FDL_FlashErase(slave_address,sector_size);

                if((master_address !=valid_fixnv_nv_addr) &&
					(INVALID_ADDR != valid_fixnv_nv_addr))
                {
                    FDL_FlashErase(valid_fixnv_nv_addr,sector_size);
                }

				return 1;
			}
		}
	}
	return 0;
}

uint32	update_productinfo(char * prod_address,uint32 write_size)
{
	uint16 new_crc = 0;
	uint16 figure_crc;
	uint16 bak_status;
	uint32 product_info_addr = 0XFFFFFFFF;
	uint32 product_info_sec_addr = 0XFFFFFFFF;
	uint32 product_info_sec_size = 0;
	uint32 slave_sec_size = 0;
	product_info_addr = s_flash_partition_info_ptr->ProdinfoAdress;
	product_info_sec_size = FDL_FlashCheckSectorSize(product_info_addr);
	product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;
	slave_sec_size = FDL_FlashCheckSectorSize(slave_address);
	if(write_size > product_info_sec_size)
	{
		return 0;
	}
	new_crc =   crc16(0, prod_address, slave_sec_size -PRD_TAIL_INFO_LEN);
	bak_status = *((uint16 *)&prod_address[product_info_sec_size - PRD_STATUS_OFFSET]) ;
	*((uint16 *)&prod_address[slave_sec_size - PRD_STATUS_OFFSET]) = NV_SOK_PRD_INFO;
	*((uint16 *)&prod_address[slave_sec_size - PRD_INFO_CRC_OFFSET]) = new_crc;
	if(slave_address != product_info_sec_addr)
	{
		 if(FDL_FlashWriteArea(slave_address, (char *)prod_address, slave_sec_size) == slave_sec_size)
		 {
		 	FDL_FlashRead(slave_address,(uint8 *)prod_address,slave_sec_size);
			figure_crc = crc16(0, prod_address, slave_sec_size -PRD_TAIL_INFO_LEN);
			if((figure_crc != new_crc) &&
			   (figure_crc != EndianConv_16(new_crc)))
			{
				FDL_FlashErase(slave_address, slave_sec_size);
				return 0;
			}
			*((uint16 *)&prod_address[product_info_sec_size - PRD_STATUS_OFFSET]) = bak_status;
			if(FDL_FlashWriteArea(product_info_sec_addr, (char *)prod_address, product_info_sec_size) ==product_info_sec_size)
			{
				FDL_FlashRead(product_info_sec_addr,(uint8 *)prod_address,product_info_sec_size);
				if(slave_sec_size >= product_info_sec_size)
				{
					uint32 j =0;
					for(j=product_info_sec_size; j<slave_sec_size; j++)
					{
						*(prod_address+j) = 0xFF;
					}
				}
				else
				{
					return 0;
				}
				figure_crc = crc16(0, prod_address, slave_sec_size - PRD_TAIL_INFO_LEN);
				if((figure_crc != new_crc) &&
				   (figure_crc != EndianConv_16(new_crc)))
				{
					return 0;
				}
				FDL_FlashErase(slave_address, slave_sec_size);
                return 1;
            }
        }
    }
	else
	{
        return 0;
	}
	return 0;
}



uint32 get_valid_nv_address(void)
{
	NV_STATUS_TYPE_E  slave_in_ps_status;
	NV_STATUS_TYPE_E  fixnv_in_ps_status;
	uint16 slave_in_ps_figure_crc;
	uint16 slave_in_ps_dedicated_crc;
	uint32 slave_in_ps_fixnv_size;
	uint16 fixnvnv_in_ps_figure_crc;
	uint16 fixnv_in_ps_dedicated_crc;
	uint32 fixnv_in_ps_fixnv_size;
	uint32 valid_nv_addr;
	uint32 valid_nv_len;
	uint32 fixnvnv_address_in_ps;
	uint32 slave_address_in_ps;
	uint32 fixnv_sector_size;
	uint32 slave_sector_size;
	uint32 sector_size;
	uint32 fixnv_size_in_ps;
	BOOLEAN is_find_valid_nv =FALSE;
	NOR_FLASH_CONFIG_PTR flash_partition_info_in_ps = NULL;
	uint8 * slave_sec_buf = (uint8 *)SLAVE_SECTOR_BUF_ADDESS;

	rprintf ("get_valid_nv_address:get_flash_partition_info_InPS\n");
	flash_partition_info_in_ps = get_flash_partition_info_InPS();
	rprintf ("get_flash_partition_info_InPS end\n");


	if(flash_partition_info_in_ps != NULL)
	{
		fixnvnv_address_in_ps = flash_partition_info_in_ps->fixnv_addr + FLASH_START_ADRESS;
		fixnv_sector_size   = FDL_FlashCheckSectorSize(fixnvnv_address_in_ps);
		fixnv_in_ps_status = FDL_FlashReadWord(fixnvnv_address_in_ps + fixnv_sector_size-2);
		fixnv_in_ps_fixnv_size =  FDL_FlashReadWord(fixnvnv_address_in_ps + fixnv_sector_size-8);
		fixnv_in_ps_dedicated_crc  = FDL_FlashReadWord(fixnvnv_address_in_ps);
		if((fixnv_in_ps_status == NV_MOK_SINVALID)  \
			&& (fixnv_in_ps_fixnv_size >= sizeof(uint16)) && (fixnv_in_ps_fixnv_size <= (fixnv_sector_size - 8)))
		{
			fixnvnv_in_ps_figure_crc = crc16(0, fixnvnv_address_in_ps + sizeof(uint16), fixnv_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == fixnvnv_in_ps_figure_crc) ||
			   (fixnv_in_ps_dedicated_crc == EndianConv_16(fixnvnv_in_ps_figure_crc)))
			{
				valid_nv_addr = fixnvnv_address_in_ps;
				fixnv_size_in_ps = fixnv_in_ps_fixnv_size;
				sector_size = FDL_FlashCheckSectorSize(valid_nv_addr);

			    if(fixnv_size_in_ps == calculate_fixNV_length(valid_nv_addr, sector_size+valid_nv_addr))
                {
					return valid_nv_addr;
				}
			}
		}
		slave_address_in_ps = flash_partition_info_in_ps->efs_start_addr + FLASH_START_ADRESS;
		slave_sector_size   = FDL_FlashCheckSectorSize(slave_address_in_ps );
		slave_in_ps_status = FDL_FlashReadWord(slave_address_in_ps + slave_sector_size-2);
		slave_in_ps_fixnv_size =  FDL_FlashReadWord(slave_address_in_ps + slave_sector_size-8);
		slave_in_ps_dedicated_crc  = FDL_FlashReadWord(slave_address_in_ps);
		if(((slave_in_ps_status  == NV_MINVALID_SOK) || (slave_in_ps_status  == NV_SOK_PRD_INFO )) \
			&& (slave_in_ps_fixnv_size >= sizeof(uint16)) && (slave_in_ps_fixnv_size <=  (slave_sector_size - 8)))
		{
			FDL_FlashRead(slave_address_in_ps, slave_sec_buf, slave_in_ps_fixnv_size);
			slave_in_ps_figure_crc = crc16(0, slave_sec_buf + sizeof(uint16), slave_in_ps_fixnv_size - sizeof(uint16));
			if((slave_in_ps_dedicated_crc  == slave_in_ps_figure_crc) ||
			   (slave_in_ps_dedicated_crc  == EndianConv_16(slave_in_ps_figure_crc)))
			{
				valid_nv_addr = slave_address_in_ps;
				fixnv_size_in_ps = slave_in_ps_fixnv_size;
				sector_size = FDL_FlashCheckSectorSize(valid_nv_addr);
			       if(fixnv_size_in_ps == calculate_fixNV_length(valid_nv_addr, sector_size+valid_nv_addr))
				{
					return valid_nv_addr;
				}
			}
		}
	}
	else
	{
		fixnvnv_address_in_ps = master_address;
		fixnv_sector_size   = FDL_FlashCheckSectorSize(fixnvnv_address_in_ps);
		fixnv_in_ps_status = FDL_FlashReadWord(fixnvnv_address_in_ps + fixnv_sector_size-2);
		fixnv_in_ps_fixnv_size =  FDL_FlashReadWord(fixnvnv_address_in_ps + fixnv_sector_size-8);
		fixnv_in_ps_dedicated_crc  = FDL_FlashReadWord(fixnvnv_address_in_ps);
		if((fixnv_in_ps_status  == NV_MOK_SINVALID) \
			&& (fixnv_in_ps_fixnv_size >= sizeof(uint16)) && (fixnv_in_ps_fixnv_size <= (fixnv_sector_size - 8)))
		{
			fixnvnv_in_ps_figure_crc = crc16(0, fixnvnv_address_in_ps + sizeof(uint16), fixnv_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == fixnvnv_in_ps_figure_crc)||
			   (fixnv_in_ps_dedicated_crc == EndianConv_16(fixnvnv_in_ps_figure_crc)))
			{
				valid_nv_addr = fixnvnv_address_in_ps;
				fixnv_size_in_ps = fixnv_in_ps_fixnv_size;
				sector_size = FDL_FlashCheckSectorSize(valid_nv_addr);
			       if(fixnv_size_in_ps == calculate_fixNV_length(valid_nv_addr, sector_size+valid_nv_addr))
				{
					return valid_nv_addr;
                }
			}

		}
		slave_address_in_ps = slave_address;
		slave_sector_size   = FDL_FlashCheckSectorSize(slave_address_in_ps );
		slave_in_ps_status = FDL_FlashReadWord(slave_address_in_ps + slave_sector_size-2);
		slave_in_ps_fixnv_size =  FDL_FlashReadWord(slave_address_in_ps + slave_sector_size-8);
		slave_in_ps_dedicated_crc  = FDL_FlashReadWord(slave_address_in_ps);
		if(((slave_in_ps_status  == NV_MINVALID_SOK) || (slave_in_ps_status  == NV_SOK_PRD_INFO )) \
			&& (slave_in_ps_fixnv_size >= sizeof(uint16)) && (slave_in_ps_fixnv_size <=  (slave_sector_size - 8)))
        {
			FDL_FlashRead(slave_address_in_ps, slave_sec_buf, slave_in_ps_fixnv_size);
			slave_in_ps_figure_crc = crc16(0, slave_sec_buf + sizeof(uint16), slave_in_ps_fixnv_size - sizeof(uint16));
			if((slave_in_ps_dedicated_crc  == slave_in_ps_figure_crc) ||
			   (slave_in_ps_dedicated_crc  == EndianConv_16(slave_in_ps_figure_crc)))
			{
				valid_nv_addr = slave_address_in_ps;
				fixnv_size_in_ps = slave_in_ps_fixnv_size;
				sector_size = FDL_FlashCheckSectorSize(valid_nv_addr);
			    if(fixnv_size_in_ps == calculate_fixNV_length(valid_nv_addr, sector_size+valid_nv_addr))
				{
					return valid_nv_addr;
                }
			}
		}
	}

	is_find_valid_nv = lookup_valid_nv_address(&valid_nv_addr);
	if(is_find_valid_nv)
	{
		sector_size = FDL_FlashCheckSectorSize(valid_nv_addr);
		valid_nv_len = FDL_FlashReadWord(valid_nv_addr + sector_size-8);
		if(valid_nv_len == calculate_fixNV_length(valid_nv_addr,sector_size+valid_nv_addr))
		{
			return valid_nv_addr;
		}
	}
	return INVALID_ADDR;
}

#endif



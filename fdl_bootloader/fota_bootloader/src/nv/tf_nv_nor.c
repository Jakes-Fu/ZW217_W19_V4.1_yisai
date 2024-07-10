/******************************************************************************
 ** File Name:      TF_nv.c                                               *
 ** Author:           tao.feng                                              *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 21/10/2010     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef  FLASH_TYPE_SPI
#include "tf_spiflash.h"
#else
#include "tf_drv_nor.h"
#endif
#include "tf_parse_nor.h"
#include "tf_nv_nor.h"
#include "tf_main.h"
#include "tf_cfg.h"
#include "update_fixnv.h"
#include "umem_cfg.h"
#include "umem_main.h"
#include "mem_cfg.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro  Definition                                     *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         extern Variables                                  *
 **---------------------------------------------------------------------------*/



/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
#define	TF_FIX_NV_LEN_OFFSET		    (16)//(8)
#define	TF_SECTOR_STATUS_OFFSET			(2)
#define TF_PRD_TAIL_INFO_LEN            (8)
#define TF_PRD_INFO_CRC_OFFSET			(6)
#define	TF_PRD_STATUS_OFFSET			(2)
#define	TF_SECTOR_STATUS_OFFSET			(2)
#define TF_FLASH_START_ADRESS           (0x0)
#define TF_SRAM_BASE_ADDR 	            (0x04000000)
#define TF_SINGLE_CS_MAX_FLASH_SIZE 	(0x1000000)
#define TF_EFS_INVALID_ITEM         	(0xffff)
#define TF_ERR_NV_LEN                   (0xffff)
#define TF_ITEM_MASK_ID            		(0x1fff) 
#define TF_UMEM_IMG_VALID_FLAG          (0xAABB)
#define TF_UMEM_IMG_INFO_SIZE           (0x6)
//#define FIXNV_SECTOR_NUM                (2)
#define SECTOR_GOOD_STATUS              (0x55aa)   // The sector is "GOOD"

#define TF_RAND_TO_UINT32( _addr ) \
   		 if( (_addr) & 0x3 ){ _addr += 0x4 - ((_addr) & 0x3); }

#define TF_UartTrace rprintf
/**---------------------------------------------------------------------------*
 **                         Local variables								                               *
 **---------------------------------------------------------------------------*/
LOCAL uint32	master_address=0,slave_address=0;
PUBLIC uint32 g_sector_size=0;
#ifdef UMEM_CTRL_SEC_EN
LOCAL uint8    img_ctrl_sec_buf[32*1024] ={0x0};
#endif

extern  TF_PARTITION_T    s_flash_part;
/**---------------------------------------------------------------------------*
 **                         extern func								                               *
 **---------------------------------------------------------------------------*/
extern uint16 crc16(uint16 crc, uint8 const *buffer, uint32 len);

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void *TF_GetFixedNvitemAddr(uint identifier, uint32 search_start, uint32 search_end)
{
	uint32  start_addr, end_addr;
	uint16  id, len;
	volatile uint16 *flash_ptr;

	start_addr = search_start;
	end_addr = search_end;
	start_addr += sizeof(uint32); // skip update flag.    

	while (start_addr < end_addr)
	{
		flash_ptr = (volatile uint16 *)(start_addr);
		id  = *flash_ptr++;
		len = *flash_ptr;
		if (0xffff == id)
		{
			return NULL;
		}
		if (id == identifier)      
		{    
			return (void *)(start_addr);            
		}
		else
		{       
			start_addr += 4 + len + (len & 0x1); // work boundary
			TF_RAND_TO_UINT32( start_addr )
		}        
	}
	return NULL;
}


/*****************************************************************************/
//  Discription: TF init nv status
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_Initialize_NV_Status(void)
{
	uint32 efs_size;
	uint32 efs_start_address;

	TF_PARTITION_PTR flash_part_info_ptr = PNULL;
	NORMCP_SPEC_PTR normcp_spec_info_ptr = PNULL;
#ifndef FLASH_TYPE_SPI
	normcp_spec_info_ptr = TF_GetNormcpSpec();
#endif
	flash_part_info_ptr = TF_GetPartitionInfo();

	master_address = flash_part_info_ptr->NvAdress; // FIXNV ADDRESS
	efs_start_address = flash_part_info_ptr->EfsAdress;
	efs_size = flash_part_info_ptr->EfsSize;
	slave_address = efs_start_address ; // EFS START ADDRESS
}

/*****************************************************************************/
//  Discription: TF cal fixnv len
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  uint16 TF_Calculate_Fixnv_Length( uint32  start_addr, uint32 end_addr)
{
	uint32  cur_addr;    
	uint16  id, fix_dedicate_len, len;
	BOOLEAN is_sram_fixnv =FALSE;

	//@读取SECOTR大小数据尾记录的长度信息
	if((start_addr >= TF_SRAM_BASE_ADDR) && (start_addr <= (TF_SRAM_BASE_ADDR+TF_SINGLE_CS_MAX_FLASH_SIZE)))
	{
		is_sram_fixnv= TRUE;
	}

	if(is_sram_fixnv)
		fix_dedicate_len = *(uint16 *)(end_addr - TF_FIX_NV_LEN_OFFSET);
	else
		fix_dedicate_len = TF_Read_Word(end_addr - TF_FIX_NV_LEN_OFFSET);

	//@跳过2字节的CRC和2字节的TIME STAMP
	cur_addr = start_addr +sizeof(uint32); // skip update flag.

	//@开始FIXNV结构扫描 
	while( cur_addr < end_addr )
	{
		if(is_sram_fixnv)
		{
			id   = *(uint16 *)(cur_addr);
			len = *(uint16 *)(cur_addr+2);
		}
		else
		{			
			id   = TF_Read_Word(cur_addr);
			len = TF_Read_Word(cur_addr+2);
		}
		cur_addr += 4;
		if( TF_EFS_INVALID_ITEM == id ) // Reach tail
		{
			cur_addr -= 4;
			break; // Stop loop
		}
		else if(id > TF_ITEM_MASK_ID)
		{
			return TF_ERR_NV_LEN;
		}
		if((len == 0x0) || (id ==0))
		{
			return TF_ERR_NV_LEN;
		}
		cur_addr += len + (len & 0x1); // WORD boundary
		// For struct need uint32 boundary, please care.
		cur_addr +=3;
		cur_addr &=0xfffffffc;
	}

	//@如果发现数据长度不符合记录长度，也返回错误值
	if(fix_dedicate_len != (uint16)(cur_addr - start_addr))
	{
		return TF_ERR_NV_LEN;
	}
	
	return fix_dedicate_len;
 }



/*****************************************************************************/
//  Discription: TF prepare slave image
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TF_Prepare_Slave_image(char *fix_nv_address, uint16 fix_nv_size)
{
	uint16 back_id, back_id_len_ps, back_id_len_ram;
	volatile uint16 *flash_ptr;
	uint32 i, num, back_id_add_ps, back_id_add_ram, id_start_addr_ps;
	TF_CUST_BACK_ITEM_PTR nv_back_ptr = PNULL;

	TF_UPDATE_INFO_PTR tf_update_info = PNULL;
	tf_update_info = TF_GetUpdateInfo();
	*((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - TF_SECTOR_STATUS_OFFSET]) = NV_MINVALID_SOK;
	*((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - TF_FIX_NV_LEN_OFFSET]) = fix_nv_size;
	*((uint16 *)&fix_nv_address[fix_nv_size]) = 0xFFFF;
	
	num = TF_GetBackItemTableSize();
	nv_back_ptr = TF_GetBackItemTable();
	id_start_addr_ps = tf_update_info->tf_update_flash.fixnv_addr;

    TF_UartTrace("Backup NVitem Table Size %d\n", num);
	
	for (i = 0; i < num; i++)
	{
		back_id = nv_back_ptr->id;
		back_id_add_ps = TF_GetFixedNvitemAddr(back_id, id_start_addr_ps, (id_start_addr_ps+FIXNV_SECTOR_NUM*g_sector_size));
		if (back_id_add_ps == NULL)
		{
		    TF_UartTrace("ERROR: Get Nvitem 0x%x Addr in Flash failed\n", back_id);
			return FALSE;			
		}
		back_id_add_ram = TF_GetFixedNvitemAddr(back_id, (uint32)fix_nv_address, (uint32)fix_nv_address + fix_nv_size);
		if (back_id_add_ram == NULL)
		{	
		    TF_UartTrace("ERROR: Get Nvitem 0x%x Addr in RAM failed\n", back_id);
			return FALSE;
		}

		flash_ptr = (volatile uint16 *)(back_id_add_ps);
		flash_ptr++;
		back_id_len_ps = *flash_ptr;
		flash_ptr = (volatile uint16 *)(back_id_add_ram);
		flash_ptr++;
		back_id_len_ram = *flash_ptr;
		if (back_id_len_ps != back_id_len_ram)
		{
		    TF_UartTrace("ERROR: Nvitem 0x%x Length not match\n", back_id);
			return FALSE;
		}
		TF_FlashRead(back_id_add_ps, (uint8 *)back_id_add_ram, back_id_len_ps+4);	// 4: sizeof(item id) + sizeof(item length)
		nv_back_ptr++;
	}
    // 这里假设上层函数改写了FIXNV 但没有改写 CRC，所以重写CRC
    // fixnv 起始地址的两字节存放 fixnv 的 CRC
	 *((uint16 *)&fix_nv_address[0]) = crc16(0, fix_nv_address+sizeof(uint16), fix_nv_size-sizeof(uint16));
	 return TRUE;
}


#ifdef FLASH_TYPE_SPI


/**********************************************************************************/
//  Description :
//      
//    Global resource dependence :
//      g_sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//      
/**********************************************************************************/
LOCAL BOOLEAN TF_verify_fixnv(uint32 addr)
{
    NV_STATUS_TYPE_E  fixnv_status;
	uint32 fixnv_length = 0,product_nv_offset;
    uint32 fixnv_address = addr;
    uint16 prod_sec_dedicate_crc = 0;
    uint16 prod_sec_figure_crc = 1;
	uint16 fixnv_dedicate_crc = 0;
	uint16 fixnv_figure_crc = 1;

#if defined(PLATFORM_UWS6121E)
    return SCI_TRUE;
#endif
    
    fixnv_status = TF_Read_Word(fixnv_address + FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET);
    
    if ((fixnv_status != NV_BOTH_INVALID) && (fixnv_status != NV_MOK_SINVALID)
        && (fixnv_status != NV_MOK_SOK) && (fixnv_status != NV_MINVALID_SOK)
        && (fixnv_status != NV_SOK_PRD_INFO) && (fixnv_status != NV_BOTH_INVALID_OLD)
        && (fixnv_status != NV_MINVALID_SOK_OLD) && (fixnv_status != NV_MOK_SINVALID_OLD)
        && (fixnv_status !=NV_SOK_PRD_INFO_OLD ) && (fixnv_status != NV_SOK_PRD_INFO_OLD)
    )
    {
	    TF_DebugTrace("%s %d fixnv_status=0x%x, fixnv_address=0x%x", 
            __func__, __LINE__, fixnv_status, fixnv_address);
            
        return SCI_FALSE;
    }
	
    fixnv_length = TF_Read_Word(fixnv_address + g_sector_size - FIX_NV_LEN_OFFSET); 

	if((fixnv_length > g_sector_size - FIX_NV_LEN_OFFSET)
	    || (fixnv_length <= sizeof(uint16))
	    || (fixnv_length != TF_Calculate_Fixnv_Length(fixnv_address, fixnv_address + g_sector_size))
	)
	{
	    TF_DebugTrace("%s %d fixnv_length=0x%x, fixnv_address=0x%x", 
	        __func__, __LINE__, fixnv_length, fixnv_address);

        //if(fixnv_length == 0) while(1);
	        
		return SCI_FALSE;
    }
    	
	fixnv_dedicate_crc = TF_Read_Word(fixnv_address);
	fixnv_figure_crc = crc16(0, (uint8 const *)fixnv_address + sizeof(uint16), fixnv_length - sizeof(uint16));
	
	if((fixnv_dedicate_crc != fixnv_figure_crc)
//	    && (fixnv_dedicate_crc != EndianConv_16(fixnv_figure_crc))
	)
	{
	    TF_DebugTrace("%s %d fixnv_dedicate_crc=0x%x, fixnv_figure_crc=0x%x, fixnv_address=0x%x", 
	        __func__, __LINE__, fixnv_dedicate_crc, fixnv_figure_crc, fixnv_address);
	        
		return SCI_FALSE;
	}
#if 1
   product_nv_offset = s_flash_part.ProdinfoAdress -s_flash_part.NvAdress;
    prod_sec_dedicate_crc = TF_Read_Word(fixnv_address + g_sector_size - PRD_INFO_CRC_OFFSET);
  //  prod_sec_figure_crc = crc16(0, fixnv_address + sizeof(uint16), g_sector_size - FIX_NV_LEN_OFFSET);
   prod_sec_figure_crc = crc16(0, fixnv_address + product_nv_offset, g_sector_size-product_nv_offset - FIX_NV_LEN_OFFSET);
   
    if((prod_sec_dedicate_crc != 0xFFFF)
        && (prod_sec_dedicate_crc != 0x0000)
    )
    {
        if((prod_sec_dedicate_crc != prod_sec_figure_crc)
//            && (prod_sec_dedicate_crc != EndianConv_16(prod_sec_figure_crc))
        )
        {
            TF_DebugTrace("%s %d prod_sec_dedicate_crc=0x%x, prod_sec_figure_crc=0x%x, fixnv_address=0x%x", 
                __func__, __LINE__, prod_sec_dedicate_crc, prod_sec_figure_crc, fixnv_address);
                
            return SCI_FALSE;
        }    
    }
    else
    {
        NV_STATUS_TYPE_E  fixnv_status;
        
        fixnv_status = TF_Read_Word(fixnv_address + g_sector_size - SECTOR_STATUS_OFFSET); 
                
        if(fixnv_status == NV_SOK_PRD_INFO)
        { 
            TF_DebugTrace("%s %d fixnv_status=0x%x, fixnv_address=0x%x", 
                __func__, __LINE__, fixnv_status, fixnv_address);
                
            return SCI_FALSE;
        }            
    }
#endif
	return SCI_TRUE;	
}

#if defined(FOTA_SUPPORT_ABUP)
extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;
#endif

PUBLIC TFLOAD_ERR_TYPE TF_Update_Fixnv(char *fix_nv_address, uint32 fix_nv_size)
{
    uint32 valid_fixnv_nv_addr;
    uint32 valid_product_nv_offset, product_nv_offset;
    uint16 fixnv_crc;
    uint16 prod_sec_dedicate_crc = 0;
    uint16 prod_sec_figure_crc = 1; 

    TF_UPDATE_INFO_PTR tf_update_info = PNULL;
#if defined(FOTA_SUPPORT_ABUP)
	s_flash_part.NvAdress = s_platform_patitiion_config.fixnv_addr;
	s_flash_part.ProdinfoAdress = s_platform_patitiion_config.prodinfo_addr;
	master_address = s_platform_patitiion_config.fixnv_addr;
	slave_address = s_platform_patitiion_config.efs_start_addr;
#endif

    fixnv_crc = *(uint16 *)fix_nv_address;                              
    memset((void *)(fix_nv_address + fix_nv_size), 0xFF, (FIXNV_SECTOR_NUM*g_sector_size - fix_nv_size));    
    *((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET]) = NV_MINVALID_SOK;   

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
    
    *((uint32 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - FIX_NV_LEN_OFFSET]) = fix_nv_size;
    *((uint16 *)&fix_nv_address[fix_nv_size]) = 0xFFFF;
    //TF_ShowProgress(40, 100);//20%
#ifdef FOTA_SUPPORT_ABUP
    abup_patch_ratio(40);
#endif

    tf_update_info = TF_GetUpdateInfo(); 
    valid_fixnv_nv_addr = tf_update_info->tf_update_flash.fixnv_addr;//get old fix nv addr
    valid_product_nv_offset = *(uint32 *)(valid_fixnv_nv_addr + FIXNV_SECTOR_NUM*g_sector_size - PRD_INFO_OFFSET);//get old product nv addr
    product_nv_offset = s_flash_part.ProdinfoAdress - s_flash_part.NvAdress;

    //copy product nv
    if (valid_product_nv_offset != 0xffffffff)
    {
        if (valid_product_nv_offset >= product_nv_offset )
        {
            memcpy((void *)(fix_nv_address + product_nv_offset), (void *)(valid_fixnv_nv_addr + valid_product_nv_offset ), (FIXNV_SECTOR_NUM*g_sector_size - valid_product_nv_offset - FIX_NV_LEN_OFFSET)); 		
        }
        else
        {
            memcpy((void *)(fix_nv_address + product_nv_offset), (void *)(valid_fixnv_nv_addr + valid_product_nv_offset ), (FIXNV_SECTOR_NUM*g_sector_size - product_nv_offset - FIX_NV_LEN_OFFSET)) ; 		
        }
        
       *((uint32 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - PRD_INFO_OFFSET]) = product_nv_offset;
    }	

#if 1
    //read old product nv crc
    prod_sec_dedicate_crc = TF_Read_Word(valid_fixnv_nv_addr + FIXNV_SECTOR_NUM*g_sector_size - PRD_INFO_CRC_OFFSET); 
    //get new product nv crc
    prod_sec_figure_crc = crc16(0, fix_nv_address + product_nv_offset, FIXNV_SECTOR_NUM*g_sector_size - product_nv_offset - FIX_NV_LEN_OFFSET);
    //set new product nv crc   
    if ((prod_sec_dedicate_crc != 0xFFFF) && (prod_sec_dedicate_crc != 0x00))
    {
        *((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - PRD_INFO_CRC_OFFSET]) = prod_sec_figure_crc;// CRC会改变
    }

    TF_UartTrace("%s %d valid_fixnv_nv_addr=0x%x, slave_address=0x%x, master_address=0x%x\n", 
        __func__, __LINE__, valid_fixnv_nv_addr, slave_address, master_address);
#endif

    if (valid_fixnv_nv_addr == slave_address)
    {
        *((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET]) = NV_MOK_SINVALID;

        if (TF_FlashWriteArea(master_address, (char *)fix_nv_address, FIXNV_SECTOR_NUM*g_sector_size) == FIXNV_SECTOR_NUM*g_sector_size)
        {
            //TF_ShowProgress(70, 100);//70%
		#ifdef FOTA_SUPPORT_ABUP
            abup_patch_ratio(70);
		#endif
    	     TF_FlashErase(slave_address, FIXNV_SECTOR_NUM*g_sector_size);
            return ERR_SUCCESS;
        }
    }
    else
    {
        if (TF_FlashWriteArea(slave_address, fix_nv_address, FIXNV_SECTOR_NUM*g_sector_size) == FIXNV_SECTOR_NUM*g_sector_size)
        {			
            if (!TF_verify_fixnv(slave_address))
            {
                TF_UartTrace("ERROR %s %d", __func__, __LINE__);
                return ERR_NO_FIND_VALID_FIXNV;
            }

            *((uint16 *)&fix_nv_address[FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET]) = NV_MOK_SOK;
            //TF_ShowProgress(60, 100);//60%
		#ifdef FOTA_SUPPORT_ABUP
            abup_patch_ratio(60);
		#endif

            if (TF_FlashWriteArea(master_address, (char *)fix_nv_address, FIXNV_SECTOR_NUM*g_sector_size) == FIXNV_SECTOR_NUM*g_sector_size)
            {
                if (!TF_verify_fixnv(master_address))
                {
                    TF_UartTrace("ERROR %s %d", __func__, __LINE__);
                    return ERR_NO_FIND_VALID_FIXNV;
                }
                
                //TF_ShowProgress(80, 100);//80%
			#ifdef FOTA_SUPPORT_ABUP
                abup_patch_ratio(80);
			#endif
                TF_Write_Word(slave_address + FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
                TF_Write_Word(master_address + FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
				
                TF_FlashErase(slave_address, FIXNV_SECTOR_NUM*g_sector_size);
                TF_Write_Word(slave_address + FIXNV_SECTOR_NUM*g_sector_size - SECTOR_STATUS_OFFSET, SECTOR_GOOD_STATUS);

                return ERR_SUCCESS;
    		}
    	}
    }

    TF_UartTrace("%s %d valid_fixnv_nv_addr=0x%x", __func__, __LINE__, valid_fixnv_nv_addr);
    return ERR_FLASH_PROGRAM; 
}

PUBLIC TFLOAD_ERR_TYPE TF_Update_Sector_By_Slave(void)
{
    NV_STATUS_TYPE_E  slave_status;
    uint32 valid_fixnv_addr;
    uint32 slave_address;
    uint32 master_address;
    uint8 * sector_buf_ptr = (uint8 *)TF_SRAM_BASE_ADDR;
    TF_PARTITION_INFO *tf_type_info_ptr = PNULL;
  	NOR_FLASH_CONFIG_PTR   flash_part_info_ptr = PNULL;	

	//step1: 获取Slave Sector地址
    tf_type_info_ptr = TF_GetPartTable();
    flash_part_info_ptr = TF_GetPartInfoInPs();
    
    if(flash_part_info_ptr != NULL)
    {
        valid_fixnv_addr = flash_part_info_ptr->efs_start_addr + FLASH_START_ADRESS ;
    }
    else
    {
        valid_fixnv_addr = tf_type_info_ptr[FLASH].base_address;      
    }
        
    master_address = tf_type_info_ptr[NV].base_address;
    slave_address = valid_fixnv_addr;

    if(slave_address == master_address)
    {
        TF_DebugTrace("%s %d slave_address=0x%x", __func__, __LINE__, slave_address);
        return ERR_NO_FIND_FLASH_PARTITION_INFO;
    }

    slave_status = FDL_FlashReadWord(slave_address + g_sector_size - SECTOR_STATUS_OFFSET);

    // 如果slave 是有效的，就拷贝slave 到master
    if(TF_verify_fixnv(slave_address))
    {
        if((slave_status == NV_SOK_PRD_INFO) 
            || (slave_status == NV_MINVALID_SOK) 
            || (slave_status == NV_MOK_SOK)
        )
        {    
            TF_FlashRead(slave_address, sector_buf_ptr, g_sector_size);

            slave_status = *(uint16 *)(sector_buf_ptr + g_sector_size - SECTOR_STATUS_OFFSET);

            if(slave_status != NV_SOK_PRD_INFO)
            {
                //*(uint16 *) &sector_buf_ptr[g_sector_size - PRD_INFO_CRC_OFFSET] = 0xFFFF;
            }
            
            *(uint16 *) &sector_buf_ptr[g_sector_size - SECTOR_STATUS_OFFSET] = NV_MOK_SOK;
            
            TF_FlashWriteArea(master_address, (char *)sector_buf_ptr,  g_sector_size);
            TF_Write_Word(slave_address + g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
        }
        
        TF_Write_Word(master_address + g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID); 
        return ERR_SUCCESS;
    }
    else if(TF_verify_fixnv(master_address))
    {
        if((slave_status == NV_SOK_PRD_INFO) 
            || (slave_status == NV_MINVALID_SOK) 
            || (slave_status == NV_MOK_SOK)
        )
        {
            TF_Write_Word(slave_address + g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
        }  
        
        TF_Write_Word(master_address + g_sector_size - SECTOR_STATUS_OFFSET, NV_MOK_SINVALID);
        return ERR_SUCCESS;
    }    
    else
    {
        TF_DebugTrace("%s %d slave_status=0x%x, slave_address=0x%x", 
            __func__, __LINE__, slave_status, slave_address);
        return ERR_NO_FIND_VALID_FIXNV;
    }
    
    return ERR_SUCCESS;
}


#else

/*****************************************************************************/
//  Discription: TF update fixnv
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Fixnv(char * fix_nv_address,uint32 fix_nv_size)
{
	uint32 valid_fixnv_nv_addr;
	TF_UPDATE_INFO_PTR tf_update_info =PNULL;


	tf_update_info = TF_GetUpdateInfo();

	//@step1 简单检查，也为后面的CODE，保证范围
	if(fix_nv_size > g_sector_size-TF_FIX_NV_LEN_OFFSET)
	{
		//@不予下载，否则破坏FIXNV
		return ERR_FIXNV_SIZE;
	} 
	//@step2 CRC计算与记录的CRC比较，确定工具传输下来的FIXNV是有效的
	if(fix_nv_size < sizeof(uint16))
	{
		return ERR_FIXNV_SIZE;
	}

	//@准备好往SLAVE中下载的FIXNV的数据
	if(!TF_Prepare_Slave_image(fix_nv_address,fix_nv_size))
	{
		return ERR_BACKUP_NV;
	}

	//@step3 增加对待操作FIXNV区域数据，FIXNV结构的检查，做到万无一失
	if(fix_nv_size != TF_Calculate_Fixnv_Length((uint32)fix_nv_address, g_sector_size + (uint32)fix_nv_address))
	{
		//@不予下载，否则破坏FIXNV
		return ERR_BACKUP_NV;
	}
	//TF_ShowProgress(40, 100);//20%

	//@务必要调用get_valid_nv_address()获取当前FIXNV的真正在FLASH中的有效地址，只有调用该函数保证的输出，才能保证真正的FIXNV得不到破坏
	valid_fixnv_nv_addr = tf_update_info->tf_update_flash.fixnv_addr;
	
	/*完整fixnv数据与将要下载的配置中的slave sector重合，那么不能利用SLAVE缓冲，一下子全破坏，直接下到目的区即可：SLAVE=原有FIXNV必然推导出MASTER!=原有FIXNV*/
	if(valid_fixnv_nv_addr == slave_address)
	{
		*((uint16 *)&fix_nv_address[g_sector_size - TF_SECTOR_STATUS_OFFSET]) = NV_MOK_SINVALID;
		if(TF_FlashWriteArea(master_address, (char *)fix_nv_address, g_sector_size) == g_sector_size)
		{
			//TF_ShowProgress(70, 100);//70%
		       TF_FlashErase(slave_address,g_sector_size);
			return ERR_SUCCESS;
		}
	}
	else
	{
             if(TF_FlashWriteArea(slave_address, fix_nv_address, g_sector_size) == g_sector_size)
		{
			//@兼容M18
			TF_FlashRead(slave_address, (uint8 *)fix_nv_address, fix_nv_size);
			*((uint16 *)&fix_nv_address[g_sector_size - TF_SECTOR_STATUS_OFFSET]) = NV_MOK_SINVALID;
			//TF_ShowProgress(60, 100);//60%
			if(TF_FlashWriteArea(master_address, (char *)fix_nv_address, g_sector_size) == g_sector_size)
			{
				//TF_ShowProgress(80, 100);//80%
				TF_FlashErase(slave_address,g_sector_size);
				return ERR_SUCCESS;
			}
		}
	}
	return ERR_FLASH_PROGRAM; 
}


/*****************************************************************************/
//  Discription: TF update sector by slave
//  Global resource dependence: should be called after .pac and .bin have been parsed 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Sector_By_Slave(void)
{
	
	uint32 slave_addr = 0xFFFFFFFF;
	uint32 slave_size = 0xFFFFFFFF;
	uint8 * slave_sec_buf = (uint8 *)TF_SRAM_BASE_ADDR;
	NV_STATUS_TYPE_E  slave_status = 0XFFFF;  

	NOR_FLASH_CONFIG_PTR   flash_part_info_ptr = PNULL;
	TF_PARTITION_INFO *tf_type_info_ptr =PNULL;

	tf_type_info_ptr = TF_GetPartTable();
	flash_part_info_ptr = TF_GetPartInfoInPs();
	//flash_partition_info_in_ps = get_flash_partition_info_InPS();

	//step1: 获取Slave Sector地址
	if(flash_part_info_ptr != NULL)
	{
		slave_addr = flash_part_info_ptr->efs_start_addr + FLASH_START_ADRESS ;
	}
	else
	{
		slave_addr = tf_type_info_ptr[FLASH].base_address;		
	}

	//step2: 检查SLAVE中数据的类型和有效性
	slave_size= TF_GetSecsize(slave_addr );	
	slave_status = TF_Read_Word(slave_addr + slave_size - SECTOR_STATUS_OFFSET);
	if(slave_status == NV_SOK_PRD_INFO)
	{
		uint32 product_info_addr = 0XFFFFFFFF;
		uint32 product_info_sec_addr = 0XFFFFFFFF;
		uint32 product_info_sec_size = 0;
		uint16 prod_sec_dedicate_crc = 0;
		uint16 prod_sec_figure_crc =1;
		

		//此处两地址只要是目标地址即可。
		product_info_addr = tf_type_info_ptr[PhaseCheck].base_address;
		product_info_sec_size = TF_GetSecsize(product_info_addr);
		product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;		

		//@为了支持M18
		TF_FlashRead(slave_addr, slave_sec_buf, slave_size);

		//@计算PRODUCT INFO的CRC与上次下载的记录值比较
		 //PRODUCT INFO的CRC的位置记录在SLAVE SIZE的后6BYTE，注意不是PRODUCT SIZE的，因为可能不同
		prod_sec_dedicate_crc = *(uint16 *)&slave_sec_buf[slave_size -PRD_INFO_CRC_OFFSET];
		//CRC的计算区是SLAVE SIZE的开头到后8BYTE
		prod_sec_figure_crc     = crc16(0, slave_sec_buf, slave_size -PRD_TAIL_INFO_LEN);			
		
		/*如果SLAVE SEC区数据是好的*/
		if(prod_sec_dedicate_crc == prod_sec_figure_crc)
		{
			//该函数有擦
			//把Slave数据Copy到Prod Sector中，该情况下不需要考虑各种公用的情况，只需要把数据拿到合适的位置，还需要下载
		if(TF_FlashWriteArea(product_info_sec_addr, (char *)slave_sec_buf,  product_info_sec_size) != product_info_sec_size)		
			{
				return ERR_FLASH_PROGRAM;
			}
				
		}
	}
	else if(slave_status == NV_MINVALID_SOK)
	{
		uint32 slave_fixnv_length = TF_Read_Word(slave_addr + slave_size - FIX_NV_LEN_OFFSET); 
		uint16 slave_fixnv_dedicate_crc = 0;
		uint16 slave_fixnv_figure_crc =1;
		if(slave_fixnv_length > (slave_size - FIX_NV_LEN_OFFSET))
		{
			return ERR_FIXNV_SIZE;
		}

		//@为了支持M18
		TF_FlashRead(slave_addr, slave_sec_buf, slave_size);

		//@计算FIXNV的CRC与上次下载时记录的CRC比较
		slave_fixnv_dedicate_crc = *(uint16 *)&slave_sec_buf[0];		
		slave_fixnv_figure_crc = crc16(0, slave_sec_buf + sizeof(uint16), slave_fixnv_length -sizeof(uint16));

		/*加FIXNV STATUS状态主要是用来区别该SECTOR在擦除过程中部分时间段
		区域出现全零情况,在全零情况下crc无法检测出问题*/
		if((slave_fixnv_dedicate_crc == slave_fixnv_figure_crc))
		{
			uint32 fix_nv_size = 0;

			//@step3:增加对FIXNV结构的检查，思想是确保每次往FIXNV写都是有效的写。后面也一样，需要每次对FIXNV区域的写都是有效的，剩下的只可能是FLASH问题
			//@获得记录再SLAVE中FIXNV的SIZE与结构扫描下来的SIZE进行比对，相同认为是最终有效的FIXNV，且完成了对 FIXNV结构的检查
			fix_nv_size = TF_Read_Word(slave_addr + slave_size - FIX_NV_LEN_OFFSET);
		       if(fix_nv_size != TF_Calculate_Fixnv_Length(slave_addr, slave_size+slave_addr))
			{			
				return ERR_FIXNV_SIZE;
			}
		
			*(uint16 *) &slave_sec_buf[slave_size - SECTOR_STATUS_OFFSET] = NV_MOK_SINVALID;
			*(uint16 *) &slave_sec_buf[slave_size - FIX_NV_LEN_OFFSET] = slave_fixnv_length;
		if(TF_FlashWriteArea(tf_type_info_ptr[NV].base_address, (char *)slave_sec_buf, slave_size) != slave_size)
			{
				return ERR_FLASH_PROGRAM;
			}
		}
	}


	return ERR_SUCCESS;
	//保留SLAVE不擦，可认为以上只是做一些防数据丢失的准备。另外寻找FIXNV的逻辑需要用到SLAVE，那个逻辑更完备，仍需要走。
}


/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Productinfo(char * prod_address,uint32 write_size)
{
	uint16 new_crc = 0;
	uint16 figure_crc;
	uint16 bak_status;
	uint32 product_info_addr = 0XFFFFFFFF;
	uint32 product_info_sec_addr = 0XFFFFFFFF;
	uint32 product_info_sec_size = 0;
	uint32 slave_sec_size = 0;
	TF_PARTITION_PTR   flash_part_info_ptr = PNULL;

	flash_part_info_ptr = TF_GetPartitionInfo();

	//此处两地址只要是目标地址即可。
	product_info_addr = flash_part_info_ptr->ProdinfoAdress;
	product_info_sec_size = TF_GetSecsize(product_info_addr);
	product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;
	slave_sec_size = TF_GetSecsize(slave_address);
	
	if(write_size > product_info_sec_size)
	{
		return ERR_PRODUCT_SIZE;
	} 
	
	new_crc =   crc16(0, prod_address, slave_sec_size -TF_PRD_TAIL_INFO_LEN);
	bak_status = *((uint16 *)&prod_address[product_info_sec_size - TF_PRD_STATUS_OFFSET]) ;
	*((uint16 *)&prod_address[slave_sec_size - TF_PRD_STATUS_OFFSET]) = NV_SOK_PRD_INFO;
	*((uint16 *)&prod_address[slave_sec_size - TF_PRD_INFO_CRC_OFFSET]) = new_crc;
	
	if(slave_address != product_info_sec_addr)
	{
		 /*保存product info 数据到slave sector*/
		 if(TF_FlashWriteArea(slave_address, (char *)prod_address, slave_sec_size) == slave_sec_size)
		 {
		 	//@m18
		 	TF_FlashRead(slave_address,(uint8 *)prod_address,slave_sec_size);
			figure_crc = crc16(0, prod_address, slave_sec_size -TF_PRD_TAIL_INFO_LEN);
			if(figure_crc != new_crc)
			{
				TF_FlashErase(slave_address, slave_sec_size);
				return ERR_CRC;
			}
			//@一是恢复状态，二CRC计算也是一致
			*((uint16 *)&prod_address[product_info_sec_size - TF_PRD_STATUS_OFFSET]) = bak_status;
			if(TF_FlashWriteArea(product_info_sec_addr, (char *)prod_address, product_info_sec_size) ==product_info_sec_size)
			{
				//@m18 与 SLAVE SIZE与PROD SIZE不一致两个考虑
				TF_FlashRead(product_info_sec_addr,(uint8 *)prod_address,product_info_sec_size);
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
					return ERR_PRODUCT_SIZE;
				}
				figure_crc = crc16(0, prod_address, slave_sec_size - TF_PRD_TAIL_INFO_LEN);
				if(figure_crc != new_crc)
				{
					return ERR_CRC;
				}
				TF_FlashErase(slave_address, slave_sec_size);
				return ERR_SUCCESS;
			}
		 }
	}
	else
	{
		/*靠配置来确保不会进入该处理流程,如果进入请修改
		flash_cfg.c来确保EfsAdress != ProdinfoAdress*/
		return ERR_PRODUCT_ADDR;
	}
	return ERR_FLASH_PROGRAM; 
}


/*****************************************************************************/
//  Discription: TF common update func, will be called 
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Common(char * buf,uint32 write_size,  FILE_ID_TYPE type)
{
	uint16 new_crc = 0;
	uint16 figure_crc;
	uint16 bak_status;
	uint32 base_addr = 0XFFFFFFFF;
	uint32 base_sec_addr = 0XFFFFFFFF;
	uint32 product_info_addr = 0XFFFFFFFF;
	uint32 product_info_sec_addr = 0XFFFFFFFF;
	uint32 product_info_sec_size = 0;
	uint32 sec_size = 0;
	uint32 slave_sec_size = 0;
	TF_PARTITION_INFO *tf_type_info_ptr =PNULL;
	BOOLEAN is_share_sec_with_product = FALSE;

	tf_type_info_ptr = TF_GetPartTable();

	//此处两地址只要是目标地址即可。
	base_addr = tf_type_info_ptr[type].base_address;
	sec_size = TF_GetSecsize(base_addr);
	base_sec_addr = (base_addr/sec_size) * sec_size;
	slave_sec_size = TF_GetSecsize(slave_address);

	product_info_addr = tf_type_info_ptr[PhaseCheck].base_address;
	product_info_sec_size = TF_GetSecsize(product_info_addr);
	product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;

	if(base_sec_addr == product_info_sec_addr)
	{
		is_share_sec_with_product = TRUE;
	}
	
	if(write_size > sec_size)
	{
		return ERR_PRODUCT_SIZE;
	} 

	if(is_share_sec_with_product)
	{
		new_crc =   crc16(0, buf, slave_sec_size -TF_PRD_TAIL_INFO_LEN);
		bak_status = *((uint16 *)&buf[sec_size - TF_PRD_STATUS_OFFSET]) ;
		*((uint16 *)&buf[slave_sec_size - TF_PRD_STATUS_OFFSET]) = NV_SOK_PRD_INFO;
		*((uint16 *)&buf[slave_sec_size - TF_PRD_INFO_CRC_OFFSET]) = new_crc;
	}
	
	if(slave_address != base_sec_addr)
	{
		 /*保存product info 数据到slave sector*/
		 if(TF_FlashWriteArea(slave_address, (char *)buf, slave_sec_size) == slave_sec_size)
		 {

			if(is_share_sec_with_product)
			{
			 	//@m18
			 	TF_FlashRead(slave_address,(uint8 *)buf,slave_sec_size);
				figure_crc = crc16(0, buf, slave_sec_size -TF_PRD_TAIL_INFO_LEN);
				if(figure_crc != new_crc)
				{
					TF_FlashErase(slave_address, slave_sec_size);
					return ERR_CRC;
				}
				//@一是恢复状态，二CRC计算也是一致
				*((uint16 *)&buf[sec_size - TF_PRD_STATUS_OFFSET]) = bak_status;
			}
		if(TF_FlashWriteArea(base_sec_addr, (char *)buf, sec_size) ==sec_size)
			{
				if(is_share_sec_with_product)
				{
					//@m18 与 SLAVE SIZE与PROD SIZE不一致两个考虑
					TF_FlashRead(base_sec_addr,(uint8 *)buf,sec_size);
					if(slave_sec_size >= sec_size)
					{
						uint32 j =0;
						for(j=sec_size; j<slave_sec_size; j++)
						{
							*(buf+j) = 0xFF;
						}
					}
					else
					{
						return ERR_PRODUCT_SIZE;
					}
					figure_crc = crc16(0, buf, slave_sec_size - TF_PRD_TAIL_INFO_LEN);
					if(figure_crc != new_crc)
					{
						return ERR_CRC;
					}
				}
				TF_FlashErase(slave_address, slave_sec_size);
				return ERR_SUCCESS;
			}
		 }
	}
	else
	{
		/*靠配置来确保不会进入该处理流程,如果进入请修改
		flash_cfg.c来确保EfsAdress != ProdinfoAdress*/
		return ERR_PRODUCT_ADDR;
	}
	return ERR_FLASH_PROGRAM; 
}
#endif


/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Udisk_Image(uint32 addr, char * buf, uint32 size, uint32 total_file_size)
{
	uint8  udisk_img_flag[6];
	uint32 udisk_size,udisk_img_base_addr, udisk_addr;
	uint32 umem_sec_size;
	TF_PARTITION_PTR   flash_part_info_ptr = PNULL;
	static  BOOLEAN is_ctrl_info_gen =FALSE;

#ifdef UMEM_CTRL_SEC_EN
	uint8   *image_state_ptr=PNULL;
	uint8    set_size_buf[2];
	uint16   byte_per_sec;
	uint16  *image_mapper_ptr=PNULL,*image_erasenum_ptr=PNULL,*image_excption_state_ptr=PNULL;
	uint32  umem_vir_sec_num,umem_data_sec_num,umem_phy_sec_num, i;
	uint32  umem_ctrl_stru_size, image_vir_sec_num, fix_nv_size, fix_nv_checksum;
	SEC_CTRL_T   *umem_info_ptr = PNULL;
	UMEM_CUSTDEFINE_T umem_cust_info ;

	flash_part_info_ptr = TF_GetPartitionInfo();
	//step1 : init umem ctrl sec
	udisk_addr =  flash_part_info_ptr->UmemAdress;
	udisk_size  =  flash_part_info_ptr->UmemSize;
	umem_sec_size  = TF_GetSecsize(udisk_addr);
	umem_cust_info.umem_rsv_phy_sec_num = 1;
	if(TF_Get_Flash_Type())
	{
		umem_cust_info.umem_vir_sec_size = 1024;
	}
	else
	{
		umem_cust_info.umem_vir_sec_size = 512;
	}

	if(!is_ctrl_info_gen)
	{
		TF_FlashRead(udisk_addr + umem_sec_size + 11, set_size_buf, 2);
		byte_per_sec = (set_size_buf[1]<<8) | set_size_buf[0];
		if(byte_per_sec != umem_cust_info.umem_vir_sec_size)
		{
			return ERR_UDISK_IMG_TYPE;
		}
		umem_phy_sec_num  = flash_part_info_ptr->UmemSize /umem_sec_size;
		umem_data_sec_num =  umem_phy_sec_num -  umem_cust_info.umem_rsv_phy_sec_num ;   
		umem_vir_sec_num    =  (umem_data_sec_num * umem_sec_size) / umem_cust_info.umem_vir_sec_size;
		umem_ctrl_stru_size   =  sizeof(SEC_CTRL_T)  + 3*umem_vir_sec_num + 2*umem_data_sec_num + 2;

		if(total_file_size > (udisk_size - umem_sec_size))
		{
			return ERR_UDISK_SIZE_OVERFLOW;
		}
		umem_info_ptr = (SEC_CTRL_T *)&img_ctrl_sec_buf[0];
		umem_info_ptr->sec_ctrl_start      = 0xfe;
		umem_info_ptr->sec_ctrl_stop       = 0xfe;
		umem_info_ptr ->sec_ctrl_index    = 0x0;

		umem_info_ptr->sec_ctrl_indicator =  flash_part_info_ptr->UmemAdress/umem_sec_size;
		umem_info_ptr->sec_state            = (uint8*)(flash_part_info_ptr->UmemAdress  + sizeof(SEC_CTRL_T));
		umem_info_ptr->sec_mapper        = (uint16*)(flash_part_info_ptr->UmemAdress  + sizeof(SEC_CTRL_T)+umem_vir_sec_num);
		umem_info_ptr->sec_erasenum     = (uint16*)(flash_part_info_ptr->UmemAdress  + sizeof(SEC_CTRL_T)+umem_vir_sec_num*3);
		umem_info_ptr->sec_exception_state =  (uint16*)(flash_part_info_ptr->UmemAdress +sizeof(SEC_CTRL_T)+3*umem_vir_sec_num + 2*umem_data_sec_num);

		TF_Memset((uint8*)( (uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T)),   0x00, umem_vir_sec_num);//init sec state 
		TF_Memset((uint8*)( (uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + umem_vir_sec_num),  0xff, umem_vir_sec_num*2);//init sec mapper
		TF_Memset((uint8*)( (uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + 3*umem_vir_sec_num),  0xff, umem_data_sec_num*2);//init sec erase num
		TF_Memset((uint8*)( (uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + 3*umem_vir_sec_num + 2*umem_data_sec_num), 0xff, 2);//init sec exception state 

		//step2:fill ctrl sec according to download umem image.
		image_vir_sec_num  =  (udisk_size - umem_sec_size) / umem_cust_info.umem_vir_sec_size;
		if((udisk_size - umem_sec_size) % umem_cust_info.umem_vir_sec_size !=  0)
		{
			image_vir_sec_num++;
		}
		image_state_ptr       = (uint8*)((uint32)&img_ctrl_sec_buf[0]  + sizeof(SEC_CTRL_T));
		image_mapper_ptr   = (uint16*)((uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + umem_vir_sec_num);
		image_erasenum_ptr= (uint16*)((uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + umem_vir_sec_num*3);
		image_excption_state_ptr = (uint16*)((uint32)&img_ctrl_sec_buf[0] + sizeof(SEC_CTRL_T) + 3*umem_vir_sec_num + 2*umem_data_sec_num);
		for(i=0; i<image_vir_sec_num; i++)
		{
			image_state_ptr[i] = 0x02;
			image_mapper_ptr[i] = i;
		}

		//step3 : write ctrl sec info into flash.(ctrl sec info located at the beginning of udisk space)
		TF_FlashErase(udisk_addr, umem_sec_size );
		if(TF_FlashWriteArea(udisk_addr, (char*)&img_ctrl_sec_buf[0], umem_ctrl_stru_size) != umem_ctrl_stru_size)
		{
			return ERR_FLASH_PROGRAM;
		}
		is_ctrl_info_gen = TRUE;
	}
	if(!TF_FlashWriteArea((addr+umem_sec_size) , buf, size))
	{
		return ERR_FLASH_PROGRAM;
	}
#else
	flash_part_info_ptr = TF_GetPartitionInfo();
	udisk_addr =  flash_part_info_ptr->UmemAdress;
	udisk_size  =  flash_part_info_ptr->UmemSize;
	umem_sec_size  = TF_GetSecsize(udisk_addr);
	if(!is_ctrl_info_gen)
	{

		udisk_img_base_addr =  udisk_addr +  udisk_size - TF_UMEM_IMG_INFO_SIZE;
		
		*(uint16*)&udisk_img_flag[0] = TF_UMEM_IMG_VALID_FLAG;
		*(uint16*)&udisk_img_flag[2] = ( total_file_size>>16) & 0xffff ;
		*(uint16*)&udisk_img_flag[4] = (total_file_size & 0xffff) ;

		if(total_file_size > (udisk_size - TF_UMEM_IMG_INFO_SIZE))
		{
			return ERR_UDISK_SIZE_OVERFLOW;
		}
		if(TF_FlashWriteArea(udisk_img_base_addr, (char*)&udisk_img_flag[0], TF_UMEM_IMG_INFO_SIZE) != TF_UMEM_IMG_INFO_SIZE)
		{
			return ERR_FLASH_PROGRAM;
		}
		is_ctrl_info_gen = TRUE;
	}	
	if(TF_FlashWriteArea(addr , buf, size) != size)	
	{
		return ERR_FLASH_PROGRAM;
	}
#endif
       return ERR_SUCCESS;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

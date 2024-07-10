/******************************************************************************
 ** File Name:      TF_parse.c                                               *
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
#include "tf_parse_nor.h"
#include "cmddef.h"
#include "tf_load.h"
#include "tf_main.h"
//#include "tf_display.h"
//#include "flash.h"
#include "normcp_spec.h"
#include "update_fixnv.h"
#ifdef  FLASH_TYPE_SPI
#include "spiflash_spec.h"
#include "spiflash.h"
#include "tf_spiflash.h"
#else
#include "tf_drv_nor.h"
#endif



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
LOCAL uint32 s_flash_partition_info_table[MAX_PATITION_NUM] = {0};
LOCAL uint32 s_flash_partition_info_num= 0;
LOCAL uint32 s_flash_partition_info_num_in_cfg = 0;
#ifdef FLASH_TYPE_SPI
LOCAL SPIFLASH_SPEC_T   s_spiflash_info;
#else
LOCAL NORMCP_SPEC_T 	s_normcp_info;
#endif
LOCAL  uint32			    s_share_fixnv_sector_flag = 0;
PUBLIC TF_PARTITION_T    s_flash_part;
LOCAL uint8 buf[2048] ={0};
extern  uint32	g_sector_size;

static NOR_FLASH_CONFIG_PTR g_flash_cfg;

#ifdef TF_LITTLE_ENDIAN
#define BYTESWITCH(s) ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))
#else
#define BYTESWITCH(s) (s)
#endif

#define TF_UartTrace rprintf

/**---------------------------------------------------------------------------*
 **                         extern Variables                                  *
 **---------------------------------------------------------------------------*/
extern TF_UPDATE_INFO_PTR tf_update_info_ptr;

/**---------------------------------------------------------------------------*
 **                         extern Func                                  *
 **---------------------------------------------------------------------------*/
extern void TF_UartTrace(const char *x_format, ...);
extern unsigned short crc16(unsigned short crc, unsigned char const *buffer, unsigned int len);


/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  number shift  
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL uint32 TF_Equal_1K_LeftShift(uint32 count)
{
	uint32 i;

	for(i = 0;i<22;i++)
	{
		if((0x400<<i) == count)
		{
			return 1;
		}
	}
	return 0;
}

/*****************************************************************************/
//  Description:  Get entity table in ps. 
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
#ifdef FLASH_TYPE_SPI
LOCAL SPIFLASH_SPEC_PTR TF_GetEntityTabInPs(
                            const uint8 *parsebuf,
                            uint32 *entity_cnt,
                            uint32 *entity_size,
                            uint32 *cfg_offset)
#else
LOCAL NORMCP_ENTITY_PTR TF_GetEntityTabInPs(
		 					const uint8 *parsebuf,
							uint32 *entity_cnt, 
							uint32 *entity_size, 
		 					uint32 *cfg_offset)
#endif
{
#ifdef FLASH_TYPE_SPI
	SPIFLASH_SPEC_PTR entity_tab_ptr = (SPIFLASH_SPEC_PTR)((uint32)parsebuf + FLASH_CFG_OFFSET);
#else
	NORMCP_ENTITY_PTR entity_tab_ptr = (NORMCP_ENTITY_PTR)((uint32)parsebuf + FLASH_CFG_OFFSET);
#endif
	NOR_FLASH_CONFIG_PTR flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)((uint32)parsebuf + FLASH_CFG_OFFSET);
	uint32 i;
	uint32 total_sector_count;
	uint32 entity_element_size = 0;
	uint32 entity_flash_cfg_offset = 0;

	i = 0;

	{
		uint32 enter_addr = (uint32)entity_tab_ptr;
		uint32 flash_cfg_addr = enter_addr;
		VOLATILE NOR_FLASH_CONFIG_PTR _flash_cfg_ptr = 0;
		uint32 j = 0;
		uint32 k = 0;
		uint32 _flash_cfg_ptr_array[2] = {0};

		while((flash_cfg_addr -enter_addr)< MAX_SECARCH_RANGE)
		{
			flash_cfg_addr = enter_addr + (j<<2);
#if defined(PLATFORM_UWS6121E)
            _flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) flash_cfg_addr;
#else
			_flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)((*(uint32*)flash_cfg_addr) + parsebuf);
#endif

#if defined(PLATFORM_UWS6121E)
            if ( (_flash_cfg_ptr->start_addr == 0x60000000) 
#else
            if ( (_flash_cfg_ptr->start_addr == 0) 
#endif
                && TF_Equal_1K_LeftShift(_flash_cfg_ptr->sect_size))//avoid access illegal address   
			{
				if((_flash_cfg_ptr->flash_size & (_flash_cfg_ptr->sect_size -1)) == 0)
				{
					total_sector_count = _flash_cfg_ptr->flash_size/_flash_cfg_ptr->sect_size;
					if((_flash_cfg_ptr->file_sect_num<total_sector_count)&&
				  		 (_flash_cfg_ptr->bank_num<=total_sector_count)&&
				   		(_flash_cfg_ptr->sect_num<total_sector_count))
					{	
						if(((_flash_cfg_ptr->efs_start_addr&(_flash_cfg_ptr->sect_size-1))==0)&&
			     				((_flash_cfg_ptr->fixnv_addr&(_flash_cfg_ptr->sect_size-1))==0))
			     			//	((_flash_cfg_ptr->mmi_res&(_flash_cfg_ptr->sect_size-1))==0))	/* because STONE_IMAGE_SUPPORT set mmi_res as 0xFFFFFFFF */
						{
							_flash_cfg_ptr_array[k] = flash_cfg_addr;
							++k;
							if(k==1)
							{
								entity_flash_cfg_offset = (uint32)flash_cfg_addr - enter_addr;
							}
							if(k==2)
							{
								entity_element_size = _flash_cfg_ptr_array[1]- _flash_cfg_ptr_array[0];
								break;
							}
						}
					}

				}
			}	
			j++;
			if((k==1) && (j>100))
			{				
				break;
			}
		}
		if(k == 0)
		{
			*entity_cnt = 0;
			return NULL;
		}
		if(k == 1)
		{
			*entity_cnt = 1;
			*entity_size = 12;
			*cfg_offset = entity_flash_cfg_offset;
			 return entity_tab_ptr;
		}
	}	
	//@error
	if(entity_element_size < 12)
	{
		*entity_cnt = 0;
		TF_UartTrace("TF_GetEntityTabInPs : entity_element_size=0x%x", entity_element_size);
		return NULL;		
	}
	if(entity_flash_cfg_offset>= entity_element_size)
	{
		*entity_cnt = 0;
		TF_UartTrace("TF_GetEntityTabInPs : entity_flash_cfg_offset=0x%x, entity_element_size=0x%x", entity_flash_cfg_offset, entity_element_size);
		return NULL;		
	}
	
	{
		uint32 enter_addr = (uint32)entity_tab_ptr;
		uint32 flash_cfg_addr = enter_addr;
		i=0;

		while(1)
		{			
			flash_cfg_addr = enter_addr + entity_element_size * i + entity_flash_cfg_offset;
			flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)((*(uint32 *)flash_cfg_addr) + parsebuf);//

#if defined(PLATFORM_UWS6121E)
            if ( (flash_cfg_ptr->start_addr == 0x60000000) 
#else
            if ( (flash_cfg_ptr->start_addr == 0) 
#endif
                && TF_Equal_1K_LeftShift(flash_cfg_ptr->sect_size))
			{
				if((flash_cfg_ptr->flash_size & (flash_cfg_ptr->sect_size -1)) == 0)
				{
					total_sector_count = flash_cfg_ptr->flash_size/flash_cfg_ptr->sect_size;
					if((flash_cfg_ptr->file_sect_num<total_sector_count)&&
				  		 (flash_cfg_ptr->bank_num<=total_sector_count)&&
				   		(flash_cfg_ptr->sect_num<total_sector_count))
					{
						if(((flash_cfg_ptr->efs_start_addr&(flash_cfg_ptr->sect_size-1))==0)&&
			     				((flash_cfg_ptr->fixnv_addr&(flash_cfg_ptr->sect_size-1))==0))
			     			//	((flash_cfg_ptr->mmi_res&(flash_cfg_ptr->sect_size-1))==0))	/* because STONE_IMAGE_SUPPORT set mmi_res as 0xFFFFFFFF */
						{
							i++;
							continue;
						}
					}
				}			
			}	
			break;			
		}
	
	}	
	*entity_cnt = i;
	*entity_size = entity_element_size;
	*cfg_offset = entity_flash_cfg_offset;
	TF_UartTrace("TF_GetEntityTabInPs : *entity_cnt=0x%x, *entity_size=0x%x, *cfg_offset=0x%x", *entity_cnt, *entity_size, *cfg_offset);
	return entity_tab_ptr;
}


/*****************************************************************************/
//  Description:  Get flash partition info  in ps. 
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
#ifdef FLASH_TYPE_SPI
LOCAL  SPIFLASH_SPEC_PTR TF_GetEntityInPs(
									const uint8* parsebuf,
									FIL *fp, 
									uint32 manu_id, 
									uint32 devc_id, 
									uint32 extend_id,
									uint32 *index,
									uint32 *cfg_offset)
{
	uint32 table_index 				 = 0;
	uint32 table_num 				 = 0; 
	uint32 entity_size = 0;
	uint32 offset, uReadSize;
	DWORD file_pos;
	SPIFLASH_SPEC_PTR entity_ptr 	 = PNULL;
	SPIFLASH_SPEC_PTR entity_tab_ptr = PNULL; //TF_GetEntityTabInPs(parsebuf, &table_num, &entity_size, cfg_offset);
	SPIFLASH_SPEC_PTR flash_spec_ptr;

    table_num = 2;
    entity_size = 32;
    entity_tab_ptr = parsebuf + FLASH_CFG_OFFSET;
    *cfg_offset = 28;

	if(fp !=NULL)
	{
		file_pos = fp->fptr;
	}
	for(table_index = 0; table_index < 2; ++table_index)
	{	
	    rprintf("table_index = %u\n", table_index);
        
		if(fp !=NULL)
		{
			offset = (uint32)(FLASH_CFG_OFFSET + (table_index * entity_size)) - MAX_PS_PARSE_SIZE;
            rprintf("offset = %d\n", offset);
			f_lseek(fp, file_pos + offset);
			f_read(fp, &s_spiflash_info, sizeof(SPIFLASH_SPEC_T), &uReadSize);
			if(uReadSize != sizeof(SPIFLASH_SPEC_T))
			{
				*index = PNULL; 
				break;
			}
			flash_spec_ptr = (SPIFLASH_SPEC_PTR)&s_spiflash_info;
            rprintf("flash_spec_ptr->nMID = 0x%x\n", flash_spec_ptr->nMID);
            rprintf("flash_spec_ptr->nDID1 = 0x%x\n", flash_spec_ptr->nDID1);
            rprintf("flash_spec_ptr->nDID2 = 0x%x\n", flash_spec_ptr->nDID2);
		}
		else
		{
            flash_spec_ptr = (SPIFLASH_SPEC_PTR)(((uint32)((SPIFLASH_SPEC_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)))));
		}
		if( (  manu_id   == flash_spec_ptr->nMID)
			&&(devc_id   == flash_spec_ptr->nDID1)
			&&(extend_id == flash_spec_ptr->nDID2))
		{
			entity_ptr = ((SPIFLASH_SPEC_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)));
			break;
		}
	}
	if(PNULL != index)
	{
		if(table_index == table_num) 
		{
			*index = 0xFFFFFFFF; 
			entity_ptr = PNULL;
		}
		else
		{
			*index = table_index;
		}
	}
	return entity_ptr;
}
#else   /* #ifdef FLASH_TYPE_SPI */
LOCAL  NORMCP_ENTITY_PTR TF_GetEntityInPs(
									const uint8* parsebuf,
									FIL *fp, 
									uint32 manu_id, 
									uint32 devc_id, 
									uint32 extend_id,
									uint32 *index,
									uint32 *cfg_offset)
{
	uint32 table_index 				 = 0;
	uint32 table_num 				 = 0; 
	uint32 entity_size = 0;
	uint32 offset, uReadSize;
	DWORD file_pos;
	NORMCP_ENTITY_PTR entity_ptr 	 = PNULL;
	NORMCP_ENTITY_PTR entity_tab_ptr = TF_GetEntityTabInPs(parsebuf, &table_num,&entity_size,cfg_offset);
	NORMCP_SPEC_PTR    flash_spec_ptr;
	
	
	TF_UartTrace("TF_GetEntityInPs : manu_id=0x%x, devc_id=0x%x,extend_id=0x%x", manu_id, devc_id, extend_id);

	if(fp !=NULL)
	{
		file_pos = fp->fptr;
	}
	for(table_index=0; table_index < table_num; ++table_index)
	{	
		if(fp !=NULL)
		{
			offset = (uint32)(((NORMCP_ENTITY_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)))->normcp_spec_ptr)- MAX_PS_PARSE_SIZE;
			f_lseek(fp, file_pos + offset);
			f_read(fp, &s_normcp_info, sizeof(NORMCP_SPEC_T), &uReadSize);
			if(uReadSize != sizeof(NORMCP_SPEC_T))
			{
				*index = PNULL; 
				break;
			}
			flash_spec_ptr = (NORMCP_SPEC_PTR)&s_normcp_info;//(NORMCP_SPEC_PTR)((uint32)(((NORMCP_ENTITY_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)))->normcp_spec_ptr));//
		}
		else
		{
			flash_spec_ptr = (NORMCP_SPEC_PTR)(((uint32)(((NORMCP_ENTITY_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)))->normcp_spec_ptr))+0);
		}
		
		if( (  manu_id   == flash_spec_ptr->manu_id)
			&&(devc_id   == flash_spec_ptr->devc_id)
			&&(extend_id == flash_spec_ptr->extend_id))
		{		
			entity_ptr = ((NORMCP_ENTITY_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)));
			break;
		}
	}

	if(PNULL != index)
	{
		if(table_index == table_num) 
		{
			*index = 0xFFFFFFFF; 
			entity_ptr = PNULL;
		}
		else
		{
			*index = table_index;
		}
	}
	return entity_ptr;
}
#endif

/*****************************************************************************/
//  Description:  add  flash partition info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL uint32 TF_Add_Partition_info(uint32 partition_addr)
{
	uint32 return_addr = partition_addr;
	uint32 i =0;
	uint32 j =0;


	if(s_flash_partition_info_num == 0)
	{
		s_flash_partition_info_table[0] = partition_addr;
		s_flash_partition_info_num++;
		return return_addr;
	}

	if(s_flash_partition_info_num_in_cfg == s_flash_partition_info_num)
	{
		return return_addr;
	}

	if(partition_addr > s_flash_partition_info_table[s_flash_partition_info_num - 1])
	{
		s_flash_partition_info_table[s_flash_partition_info_num] = partition_addr;
		s_flash_partition_info_num++;
		return return_addr;
	}
	for(i=0; i< s_flash_partition_info_num; ++i)
	{		
		if(partition_addr <s_flash_partition_info_table[i])
		{
			for(j = s_flash_partition_info_num; j > i; j--)
			{
				s_flash_partition_info_table[j] = s_flash_partition_info_table[j-1];
			}
			s_flash_partition_info_table[i] = partition_addr;			
			s_flash_partition_info_num++;
			return return_addr;
		}
		else if(partition_addr == s_flash_partition_info_table[i])
		{
			return return_addr;
		}
		else
		{
			//continue;
		}
			
	}
	return return_addr;
}


/*****************************************************************************/
//  Description:  add  flash partition info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL uint32 TF_Get_Partition_Size(uint32 partition_addr)
{
	uint32 i = 0;
    
	if (s_flash_partition_info_num == 0)
	{
		return 0;
	}
    
	for (i = 0; i < s_flash_partition_info_num; i++)
	{
		if (partition_addr == s_flash_partition_info_table[i])
		{
			if (i == (s_flash_partition_info_num - 1))
			{
				return (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr & (SINGLE_CS_MAX_FLASH_SIZE-1)));
			}
			else
			{
				if (s_flash_partition_info_table[i+1] <= s_flash_partition_info_table[i])
				{
					return 0;
				}

				if ((s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]) >= SINGLE_CS_MAX_FLASH_SIZE)
				{
					return (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr & (SINGLE_CS_MAX_FLASH_SIZE-1)));
				}
				else
				{
					return (s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]);
				}
			}			
		}
	}
    
	if (i == (s_flash_partition_info_num - 1))
	{
		return 0;
	}
	return 0;
}


/*****************************************************************************/
//  Description:    flash partition info address sort and generate partition size
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
#ifdef FLASH_TYPE_SPI
LOCAL  void  TF_Get_Partition_Info(
				TF_PARTITION_PTR	 flash_partition_ptr,
				NOR_FLASH_CONFIG_PTR nor_flash_cfg_ptr)
{
#ifdef SECURE_BOOT_SUPPORT
    flash_partition_ptr->PsAdress       = TF_Add_Partition_info(nor_flash_cfg_ptr->ps_addr) - 0x400;
#else
    flash_partition_ptr->PsAdress       = TF_Add_Partition_info(nor_flash_cfg_ptr->ps_addr);
#endif
    flash_partition_ptr->ResAdress      = TF_Add_Partition_info(nor_flash_cfg_ptr->mmi_res);
    flash_partition_ptr->UmemAdress     = TF_Add_Partition_info(nor_flash_cfg_ptr->umem_addr);
    flash_partition_ptr->NvAdress       = TF_Add_Partition_info(nor_flash_cfg_ptr->fixnv_addr);
    flash_partition_ptr->ProdinfoAdress = TF_Add_Partition_info(nor_flash_cfg_ptr->prodinfo_addr);
    flash_partition_ptr->EfsAdress      = TF_Add_Partition_info(nor_flash_cfg_ptr->efs_start_addr);
    flash_partition_ptr->sploadaddr     = TF_Add_Partition_info(nor_flash_cfg_ptr->spload_addr);
#ifdef CHIP_VER_UWS6121E
    flash_partition_ptr->cp_addr        = TF_Add_Partition_info(nor_flash_cfg_ptr->cp_addr);
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    flash_partition_ptr->apn_addr       = TF_Add_Partition_info(nor_flash_cfg_ptr->apn_addr);
#endif
    flash_partition_ptr->config_nv_addr = TF_Add_Partition_info(nor_flash_cfg_ptr->config_nv_addr);
    flash_partition_ptr->operdata_addr  = TF_Add_Partition_info(nor_flash_cfg_ptr->operator_data_addr);
#endif
    s_flash_partition_info_num_in_cfg   = s_flash_partition_info_num;
    flash_partition_ptr->FlashSize      = nor_flash_cfg_ptr->flash_size;
    flash_partition_ptr->PSSize         = TF_Get_Partition_Size(nor_flash_cfg_ptr->ps_addr);
    flash_partition_ptr->ResSize        = TF_Get_Partition_Size(nor_flash_cfg_ptr->mmi_res);
    flash_partition_ptr->NvSize         = TF_Get_Partition_Size(nor_flash_cfg_ptr->fixnv_addr);
    flash_partition_ptr->sploadsize     = TF_Get_Partition_Size(nor_flash_cfg_ptr->spload_addr);
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    flash_partition_ptr->apn_size       = TF_Get_Partition_Size(nor_flash_cfg_ptr->apn_addr);
#endif
    flash_partition_ptr->config_nv_size = TF_Get_Partition_Size(nor_flash_cfg_ptr->config_nv_addr);
    flash_partition_ptr->operdata_size  = TF_Get_Partition_Size(nor_flash_cfg_ptr->operator_data_addr);
    flash_partition_ptr->UmemSize       = nor_flash_cfg_ptr->umem_size;
    flash_partition_ptr->EfsSize        = (nor_flash_cfg_ptr->sect_num) * nor_flash_cfg_ptr->sect_size;
    //flash_partition_ptr->Sec_size      = nor_flash_cfg_ptr->sect_size;
    g_sector_size = nor_flash_cfg_ptr->sect_size;
    if((flash_partition_ptr->ProdinfoAdress/g_sector_size) == (flash_partition_ptr->NvAdress/g_sector_size))
    {
        s_share_fixnv_sector_flag = 1;
    }
}
#else   /* #ifdef FLASH_TYPE_SPI */
LOCAL  void  TF_Get_Partition_Info(
				TF_PARTITION_PTR	 flash_partition_ptr,
				NOR_FLASH_CONFIG_PTR nor_flash_cfg_ptr)
{

    flash_partition_ptr->PsAdress   = TF_Add_Partition_info(nor_flash_cfg_ptr->start_addr);
    flash_partition_ptr->ResAdress  = TF_Add_Partition_info(nor_flash_cfg_ptr->mmi_res);
    flash_partition_ptr->UmemAdress = TF_Add_Partition_info(nor_flash_cfg_ptr->umem_addr);
    flash_partition_ptr->NvAdress   = TF_Add_Partition_info(nor_flash_cfg_ptr->fixnv_addr);
    flash_partition_ptr->ProdinfoAdress = TF_Add_Partition_info(nor_flash_cfg_ptr->prodinfo_addr);
    flash_partition_ptr->EfsAdress  = TF_Add_Partition_info(nor_flash_cfg_ptr->efs_start_addr);
    flash_partition_ptr->sploadaddr = TF_Add_Partition_info(nor_flash_cfg_ptr->spload_addr);

    s_flash_partition_info_num_in_cfg = s_flash_partition_info_num;

    flash_partition_ptr->FlashSize  = nor_flash_cfg_ptr->flash_size;
    flash_partition_ptr->PSSize     = TF_Get_Partition_Size(nor_flash_cfg_ptr->start_addr);
    flash_partition_ptr->ResSize    = TF_Get_Partition_Size(nor_flash_cfg_ptr->mmi_res);
    flash_partition_ptr->NvSize     = TF_Get_Partition_Size(nor_flash_cfg_ptr->fixnv_addr);
    flash_partition_ptr->sploadsize = TF_Get_Partition_Size(nor_flash_cfg_ptr->spload_addr);
    flash_partition_ptr->UmemSize   = nor_flash_cfg_ptr->umem_size;	
    flash_partition_ptr->EfsSize    = (nor_flash_cfg_ptr->sect_num) * nor_flash_cfg_ptr->sect_size;

    if(s_normcp_info.flash_drv_mode == FLASH_INTEL_M18_MODE)
    {
        flash_partition_ptr->EfsSize   = ((nor_flash_cfg_ptr->sect_num)>>1) * nor_flash_cfg_ptr->sect_size;
    }
    else
    {
        flash_partition_ptr->EfsSize   = (nor_flash_cfg_ptr->sect_num) * nor_flash_cfg_ptr->sect_size;
    }
    //flash_partition_ptr->Sec_size      = nor_flash_cfg_ptr->sect_size;
    sector_size = nor_flash_cfg_ptr->sect_size;
    if((flash_partition_ptr->ProdinfoAdress/g_sector_size)==(flash_partition_ptr->NvAdress/g_sector_size))
    {
        s_share_fixnv_sector_flag = 1;
    }

}
#endif

/*****************************************************************************/
//  Description:  Get flash partition info  in ps. 
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  NOR_FLASH_CONFIG_PTR TF_Get_Partition(const uint8 *parsebuf, FIL *fp)
{
	uint32 manufacturer_ID, device_ID, extend_ID, index;
#ifdef FLASH_TYPE_SPI
	SPIFLASH_SPEC_PTR entity_ptr = PNULL;
#else
    NORMCP_ENTITY_PTR entity_ptr = PNULL;
#endif
	NOR_FLASH_CONFIG_PTR flash_cfg = PNULL;
	TF_UPDATE_INFO_PTR tf_update_info_ptr = PNULL;
	uint32 cfg_offset = 0;

	tf_update_info_ptr = TF_GetUpdateInfo();
	manufacturer_ID = tf_update_info_ptr->tf_update_flash.manu_id;
	device_ID = tf_update_info_ptr->tf_update_flash.dev_id;
	extend_ID = tf_update_info_ptr->tf_update_flash.ext_id;

#ifdef SECURE_BOOT_SUPPORT
    flash_cfg = (NOR_FLASH_CONFIG_PTR)(parsebuf + 0x400 + 0x3C4);
#else
    flash_cfg = (NOR_FLASH_CONFIG_PTR)(parsebuf + 0x3C4);
#endif

    g_flash_cfg = flash_cfg;
    TF_UartTrace("start_addr: 0x%x\n", g_flash_cfg->start_addr);
    TF_UartTrace("efs_addr: 0x%x\n", g_flash_cfg->efs_start_addr);
    TF_UartTrace("fixnv_addr: 0x%x\n", g_flash_cfg->fixnv_addr);
    TF_UartTrace("prodinfo_addr: 0x%x\n", g_flash_cfg->prodinfo_addr);
    TF_UartTrace("mmi_res: 0x%x\n", g_flash_cfg->mmi_res);
    TF_UartTrace("umem_addr: 0x%x\n", g_flash_cfg->umem_addr);
    TF_UartTrace("simlock_addr: 0x%x\n", g_flash_cfg->simlock_addr);
    TF_UartTrace("ps_addr: 0x%x\n", g_flash_cfg->ps_addr);
    TF_UartTrace("operator_addr: 0x%x\n", g_flash_cfg->operator_data_addr);
    TF_UartTrace("crystal_aging_addr: 0x%x\n", g_flash_cfg->crystal_aging_addr);
#ifdef CHIP_VER_UWS6121E
    TF_UartTrace("cp_addr: 0x%x\n", g_flash_cfg->cp_addr);
#endif

#if 0
	entity_ptr = TF_GetEntityInPs(parsebuf, fp,  manufacturer_ID,device_ID,extend_ID,&index,&cfg_offset);
	if (entity_ptr==NULL)
	{
		TF_UartTrace("TF_Get_Partition : entity_ptr=0x%x", entity_ptr);
		return NULL;
	}
#endif
    TF_UartTrace("TF_Get_Partition : flash_cfg=0x%x\n", g_flash_cfg);
	
	//flash_cfg  = (NOR_FLASH_CONFIG_PTR)(*(uint32 *)(((uint32)entity_ptr)+cfg_offset) + parsebuf);
	return g_flash_cfg;	
}


/*****************************************************************************/
//  Description:   calc fixnv length
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL uint16 TF_Calculate_FixNV_Length( uint32  start_addr, uint32 end_addr)
{
	uint32  cur_addr;    
	uint16  id, fix_dedicate_len, len;
	BOOLEAN is_sram_fixnv =SCI_FALSE;

	//@读取SECOTR大小数据尾记录的长度信息
	if((start_addr >= SRAM_START_ADDR) && (start_addr <= (SRAM_START_ADDR+SINGLE_CS_MAX_FLASH_SIZE)))
	{
		is_sram_fixnv= SCI_TRUE;
	}

	if(is_sram_fixnv)
		fix_dedicate_len = *(uint16 *)(end_addr - FIX_NV_LEN_OFFSET);
	else
		fix_dedicate_len = TF_Read_Word(end_addr - FIX_NV_LEN_OFFSET);

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
		if( EFS_INVALID_ITEM == id ) // Reach tail
		{
			cur_addr -= 4;
			break; // Stop loop
		}
		else if(id > ITEM_MASK_ID)
		{
			return ERR_NV_LEN;
		}
		if((len == 0x0) || (id ==0))
		{
			return ERR_NV_LEN;
		}
		cur_addr += len + (len & 0x1); // WORD boundary
		// For struct need uint32 boundary, please care.
		cur_addr +=3;
		cur_addr &=0xfffffffc;
	}

	//@如果发现数据长度不符合记录长度，也返回错误值
	if(fix_dedicate_len != (uint16)(cur_addr - start_addr))
	{
		return ERR_NV_LEN;
	}
	
	return fix_dedicate_len;
 }

/*****************************************************************************/
//  Description:   get VALID NV ADDRESS
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  TF_Lookup_Valid_NV_Address(uint32 *valid_nv_addr_out )
{
	uint32 end_addr;
	uint32 sect_size;
	uint16 sector_figure_crc;
	uint16 sector_dedicated_crc;
	uint32 sector_fixnv_size;
	BOOLEAN is_find_valid_nv = FALSE;
	NV_STATUS_TYPE_E sec_status;

	end_addr   =  0 + s_flash_part.FlashSize;
	/*兼容256M flash*/
	if(s_flash_part.FlashSize > SINGLE_CS_MAX_FLASH_SIZE)
	{
		end_addr   =  FLASH_START_ADRESS + SINGLE_CS_MAX_FLASH_SIZE;
		//Enable Write Flash
		while( end_addr > 0)
		{
			sect_size   = TF_GetSecsize(end_addr -1);//必须要减一
			end_addr -= sect_size;
			sec_status = TF_Read_Word(end_addr + sect_size-SECTOR_STATUS_OFFSET);

			if((sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
			    ((sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
			{
				sector_dedicated_crc = TF_Read_Word(end_addr);
				sector_fixnv_size = TF_Read_Word(end_addr + sect_size - FIX_NV_LEN_OFFSET);
				if((sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof(uint16)))
				{
					sector_figure_crc = crc16(0, (uint8*)(end_addr + sizeof(uint16)), sector_fixnv_size - sizeof(uint16));
			if((sector_dedicated_crc == sector_figure_crc)||(sector_dedicated_crc == BYTESWITCH(sector_figure_crc)))
					{
						*valid_nv_addr_out = end_addr;
						is_find_valid_nv = TRUE;
						break;
					}
				}
			}
		}

		end_addr  =  FLASH_START_ADRESS_CS3 + SINGLE_CS_MAX_FLASH_SIZE;
		while( end_addr > FLASH_START_ADRESS_CS3)
		{
			sect_size   = TF_GetSecsize(end_addr -1);//必须要减一
			end_addr -= sect_size;
			sec_status = TF_Read_Word(end_addr + sect_size-SECTOR_STATUS_OFFSET);

			if((sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
			    ((sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
			{
				sector_dedicated_crc = TF_Read_Word(end_addr);
				sector_fixnv_size = TF_Read_Word(end_addr + sect_size - FIX_NV_LEN_OFFSET);
				if((sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof(uint16)))
				{
					sector_figure_crc = crc16(0, (uint8*)(end_addr + sizeof(uint16)), sector_fixnv_size - sizeof(uint16));
			if((sector_dedicated_crc == sector_figure_crc)||(sector_dedicated_crc == BYTESWITCH(sector_figure_crc)))
					{
						*valid_nv_addr_out = end_addr;
						is_find_valid_nv = TRUE;
						break;
					}
				}
			}
		}
	}
	else
	{
		while( end_addr > 0)
	       {
	             sect_size   = TF_GetSecsize(end_addr -1);//必须要减一
	   	      end_addr -= sect_size;
	             sec_status = TF_Read_Word(end_addr + sect_size-SECTOR_STATUS_OFFSET);
	         
	             if((sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
	                ((sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
	             {
	             		sector_dedicated_crc = TF_Read_Word(end_addr);
	             		sector_fixnv_size = TF_Read_Word(end_addr + sect_size - FIX_NV_LEN_OFFSET);
	             		if((sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof(uint16)))
	             		{
					sector_figure_crc = crc16(0, (uint8*)(end_addr + sizeof(uint16)), sector_fixnv_size - sizeof(uint16));
			if((sector_dedicated_crc == sector_figure_crc)||(sector_dedicated_crc == BYTESWITCH(sector_figure_crc)))
					{
						*valid_nv_addr_out = end_addr;
						is_find_valid_nv = TRUE;
						break;
					}
	             		}
	             }
	        }
	 }
	
	 return is_find_valid_nv;
}


/*****************************************************************************/
//  Description:   get VALID NV ADDRESS
//  Global resource dependence:  should be called after TF_GetPartInfoInPac()
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL  uint32 TF_Get_Valid_NV_Address(void)
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
	
	//@ 获取在Flash中可能的FIXNV地址
	flash_partition_info_in_ps = TF_GetPartInfoInPs();
	//@Hyy 思想是flash中的PS或者部分PS可能能帮助快速定位到FIXNV可能的区域
	if(flash_partition_info_in_ps != NULL)
	{
		//@step1: 在flash中fixnv nv地址处找有效nv
		fixnvnv_address_in_ps = flash_partition_info_in_ps->fixnv_addr + 0;
		fixnv_sector_size   = TF_GetSecsize(fixnvnv_address_in_ps);
		fixnv_in_ps_status = TF_Read_Word(fixnvnv_address_in_ps + fixnv_sector_size-2);
		fixnv_in_ps_fixnv_size =  TF_Read_Word(fixnvnv_address_in_ps + fixnv_sector_size-8);
		fixnv_in_ps_dedicated_crc  = TF_Read_Word(fixnvnv_address_in_ps);
		if((fixnv_in_ps_status  == NV_MOK_SINVALID) \
			&& (fixnv_in_ps_fixnv_size >= sizeof(uint16)) && (fixnv_in_ps_fixnv_size <= (fixnv_sector_size - 8)))
		{
			//@状态和CRC的CHECK			
			fixnvnv_in_ps_figure_crc = crc16(0, fixnvnv_address_in_ps + sizeof(uint16), fixnv_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == fixnvnv_in_ps_figure_crc)||(fixnv_in_ps_dedicated_crc == BYTESWITCH(fixnvnv_in_ps_figure_crc)))
			{
				valid_nv_addr = fixnvnv_address_in_ps;
				fixnv_size_in_ps = fixnv_in_ps_fixnv_size;
				
				sector_size = TF_GetSecsize(valid_nv_addr);

				//@增加对结构的检查，最终保证找到的是有效的FIXNV
			       if(fixnv_size_in_ps == TF_Calculate_FixNV_Length(valid_nv_addr, sector_size+valid_nv_addr))
				{			
					return valid_nv_addr;
				}
			}
			
		}
		
		//@step2: 在flash中slave sector找有效NV
		slave_address_in_ps = flash_partition_info_in_ps->efs_start_addr + 0;
		slave_sector_size   = TF_GetSecsize(slave_address_in_ps );
		slave_in_ps_status = TF_Read_Word(slave_address_in_ps + slave_sector_size-2);
		slave_in_ps_fixnv_size =  TF_Read_Word(slave_address_in_ps + slave_sector_size-8);
		slave_in_ps_dedicated_crc  = TF_Read_Word(slave_address_in_ps);
		if(((slave_in_ps_status  == NV_MINVALID_SOK) || (slave_in_ps_status  == NV_SOK_PRD_INFO )) \
			&& (slave_in_ps_fixnv_size >= sizeof(uint16)) && (slave_in_ps_fixnv_size <=  (slave_sector_size - 8)))
		{
			TF_FlashRead(slave_address_in_ps, slave_sec_buf, slave_in_ps_fixnv_size);
			/*加FIXNV STATUS状态主要是用来区别该SECTOR在擦除过程中部分时间段
			区域出现全零情况,在全零情况下crc无法检测出问题*/
			
			slave_in_ps_figure_crc = crc16(0, slave_sec_buf + sizeof(uint16), slave_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == slave_in_ps_figure_crc)||(fixnv_in_ps_dedicated_crc == BYTESWITCH(slave_in_ps_figure_crc)))
			{
				valid_nv_addr = slave_address_in_ps;
				fixnv_size_in_ps = slave_in_ps_fixnv_size;
				
				sector_size = TF_GetSecsize(valid_nv_addr);
				//@增加对结构的检查，最终保证找到的是有效的FIXNV
			       if(fixnv_size_in_ps == TF_Calculate_FixNV_Length(valid_nv_addr, sector_size+valid_nv_addr))
				{			
					return valid_nv_addr;  
				}
			}			
		}
	}
	else
	/*这种情况下ps中没有找到对应flash 配置信息,可能为第一次下载或是
	ps已经被破坏，或者上次下载到第一个SECTOR断电*/
	//@hyy 套用上面的流程，目的是在INPS找不到的情况下，利用将要下的目的地址帮助加速找到FIXNV，其实是对可能性的一种利用。
	{
		//@step1: 在将要下的FIXNV地址处找有效nv
		fixnvnv_address_in_ps = s_flash_part.NvAdress;
		fixnv_sector_size   = TF_GetSecsize(fixnvnv_address_in_ps);
		fixnv_in_ps_status = TF_Read_Word(fixnvnv_address_in_ps + fixnv_sector_size-2);
		fixnv_in_ps_fixnv_size =  TF_Read_Word(fixnvnv_address_in_ps + fixnv_sector_size-8);
		fixnv_in_ps_dedicated_crc  = TF_Read_Word(fixnvnv_address_in_ps);
		if((fixnv_in_ps_status  == NV_MOK_SINVALID) \
			&& (fixnv_in_ps_fixnv_size >= sizeof(uint16)) && (fixnv_in_ps_fixnv_size <= (fixnv_sector_size - 8)))
		{
			//@状态和CRC的CHECK			
			fixnvnv_in_ps_figure_crc = crc16(0, fixnvnv_address_in_ps + sizeof(uint16), fixnv_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == fixnvnv_in_ps_figure_crc)||(fixnv_in_ps_dedicated_crc == BYTESWITCH(fixnvnv_in_ps_figure_crc)))
			{
				valid_nv_addr = fixnvnv_address_in_ps;
				fixnv_size_in_ps = fixnv_in_ps_fixnv_size;
				
				sector_size = TF_GetSecsize(valid_nv_addr);

				//@增加对结构的检查，最终保证找到的是有效的FIXNV
			       if(fixnv_size_in_ps == TF_Calculate_FixNV_Length(valid_nv_addr, sector_size+valid_nv_addr))
				{			
					return valid_nv_addr;
				}
			}
			
		}
		
		//@step2: 在将要下的BACKUP地址处找有效NV
		slave_address_in_ps = s_flash_part.EfsAdress;
		slave_sector_size   = TF_GetSecsize(slave_address_in_ps );
		slave_in_ps_status = TF_Read_Word(slave_address_in_ps + slave_sector_size-2);
		slave_in_ps_fixnv_size =  TF_Read_Word(slave_address_in_ps + slave_sector_size-8);
		slave_in_ps_dedicated_crc  = TF_Read_Word(slave_address_in_ps);
		if(((slave_in_ps_status  == NV_MINVALID_SOK) || (slave_in_ps_status  == NV_SOK_PRD_INFO )) \
			&& (slave_in_ps_fixnv_size >= sizeof(uint16)) && (slave_in_ps_fixnv_size <=  (slave_sector_size - 8)))
		{
			TF_FlashRead(slave_address_in_ps, slave_sec_buf, slave_in_ps_fixnv_size);
			/*加FIXNV STATUS状态主要是用来区别该SECTOR在擦除过程中部分时间段
			区域出现全零情况,在全零情况下crc无法检测出问题*/
			slave_in_ps_figure_crc = crc16(0, slave_sec_buf + sizeof(uint16), slave_in_ps_fixnv_size - sizeof(uint16));
			if((fixnv_in_ps_dedicated_crc == slave_in_ps_figure_crc)||(fixnv_in_ps_dedicated_crc == BYTESWITCH(slave_in_ps_figure_crc)))
			{
				valid_nv_addr = slave_address_in_ps;
				fixnv_size_in_ps = slave_in_ps_fixnv_size;
				
				sector_size = TF_GetSecsize(valid_nv_addr);
				//@增加对结构的检查，最终保证找到的是有效的FIXNV
			       if(fixnv_size_in_ps == TF_Calculate_FixNV_Length(valid_nv_addr, sector_size+valid_nv_addr))
				{			
					return valid_nv_addr;  
				}
			}			
		}
	}

	//@step3: 执行全Flash扫描，该功能对M18系 SLAVE区域这种CASE无能为力。
	is_find_valid_nv = TF_Lookup_Valid_NV_Address(&valid_nv_addr);
	if(is_find_valid_nv)
	{
		sector_size = TF_GetSecsize(valid_nv_addr);
		valid_nv_len = TF_Read_Word(valid_nv_addr + sector_size-8);
		//@同样必须结构扫描，保证找到的FIXNV是有效的。
		if(valid_nv_len == TF_Calculate_FixNV_Length(valid_nv_addr,sector_size+valid_nv_addr))
		{
			return valid_nv_addr;
		}
	}
	return ERR_VALID_NV_ADDR;	
}


#if 0
/*****************************************************************************/
//  Description:   get top or top-bottom structure flash top small sector info
//  Global resource dependence: should be called after TF_GetPartInfoInPac() has been called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC FLASH_STRUCT_E  TF_GetFlashStru(uint32 *small_sec_size, uint32 *sma_sec_num)
{
#ifdef FLASH_TYPE_SPI
    return FLASH_BOTTOM_STRUCT;
#else
	FLASH_STRUCT_E stru_indicator   = (FLASH_STRUCT_E)((s_normcp_info.flash_top_struct) & 0x0000000F);
	
	if((stru_indicator == FLASH_TOP_STRUCT)  || (stru_indicator ==  FLASH_TOP_BOTTOM_STRUCT) || (stru_indicator ==  FLASH_BOTTOM_STRUCT))
	{
		*small_sec_size =  ((s_normcp_info.flash_top_struct) & 0xFFFFF000) >> 12;
		*sma_sec_num =   ((s_normcp_info.flash_top_struct) & 0x00000FF0) >> 4 ;
	}
       return stru_indicator;
#endif
}
#endif

#ifdef FLASH_TYPE_SPI
/*****************************************************************************/
PUBLIC SPIFLASH_SPEC_PTR  TF_GetSpiFlashSpec(void)
{
	return (SPIFLASH_SPEC_PTR)&s_spiflash_info;
}
#else
//  Description:   get nomcp spec info in pac
//  Global resource dependence: should be called after TF_GetPartInfoInPac() has been called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC NORMCP_SPEC_PTR  TF_GetNormcpSpec(void)
{
	return (NORMCP_SPEC_PTR)&s_normcp_info;
}
#endif

/*****************************************************************************/
//  Discription: This func will parse ps.bin and get flash partition info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TF_PARTITION_PTR TF_GetPartInfoInPac(const uint8 *parsebuf, FIL *fp)
{
	NOR_FLASH_CONFIG_PTR flash_partition_info_ptr = PNULL;
	
	flash_partition_info_ptr = TF_Get_Partition(parsebuf, fp);
	if (!flash_partition_info_ptr)
	{
		TF_UartTrace("TF_GetPartitionInfo : flash_partition_info_ptr=0x%x", flash_partition_info_ptr);
		//TF_Show_Error(ERR_NO_FIND_FLASH_PARTITION_INFO);
	}

	TF_Get_Partition_Info((TF_PARTITION_PTR)(&s_flash_part), flash_partition_info_ptr);
	return (TF_PARTITION_PTR)(&s_flash_part);
}


/*****************************************************************************/
//  Discription: This func will parse ps.bin and get flash partition info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  NOR_FLASH_CONFIG_PTR TF_GetPartInfoInPs(void)
{
	uint32 manufacturer_ID,device_ID,extend_ID, index, cfg_offset;
	TF_UPDATE_INFO_PTR tf_update_info_ptr =PNULL;
#ifdef FLASH_TYPE_SPI
    SPIFLASH_SPEC_PTR entity_ptr = PNULL;
#else
	NORMCP_ENTITY_PTR entity_ptr = PNULL;
#endif
	NOR_FLASH_CONFIG_PTR flash_cfg = PNULL;

	tf_update_info_ptr = TF_GetUpdateInfo();
	manufacturer_ID = tf_update_info_ptr->tf_update_flash.manu_id;
	device_ID  = tf_update_info_ptr->tf_update_flash.dev_id;
	extend_ID  = tf_update_info_ptr->tf_update_flash.ext_id;
	TF_FlashRead(0,  buf,  sizeof(buf)/sizeof(buf[0]));
	entity_ptr = TF_GetEntityInPs(buf, NULL, manufacturer_ID, device_ID, extend_ID, &index, &cfg_offset);
	if(entity_ptr==NULL)
	{
		TF_UartTrace("TF_Get_Partition : entity_ptr=0x%x", entity_ptr);
		return NULL;
	}
	//@找到FLASH中对应该FLASH的FLASH CFG在FLASH中的地址。
	flash_cfg  = (NOR_FLASH_CONFIG_PTR)(*(uint32 *)(((uint32)entity_ptr)+cfg_offset) + buf);

	return flash_cfg;	
}

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence:should be called after TF_GetPartInfoInPac has beedb called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_InEfsSpace(uint32 addr)
{
	return ((addr >=s_flash_part.EfsAdress) && (addr < s_flash_part.EfsAdress + s_flash_part.EfsSize));
}

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence:should be called after TF_GetPartInfoInPac has beedb called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
 PUBLIC uint32 TF_EfsAddrConvert(uint32 addr)
{
     uint32 temp_addr;
	 
	if((addr >=s_flash_part.EfsAdress) && (addr < s_flash_part.EfsAdress + s_flash_part.EfsSize))
	{
		if(addr & 0x10)
		{
			temp_addr = 0x10;
			addr = (addr+ s_flash_part.EfsSize) & (~temp_addr);
		}	
	}
	return addr;
}


/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence: should be called after TF_GetPartInfoInPac has beedb called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_PARTITION_PTR TF_GetPartitionInfo(void)
{
	return (TF_PARTITION_PTR)&s_flash_part;
}


/*****************************************************************************/
//  Discription: This func will get  valid fixnv address in ps and update tf update info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TFLOAD_ERR_TYPE TF_UpdateFixnvAddress(void)
{
	uint32 fixnv_addr;
	TF_UPDATE_INFO_PTR tf_update_info_ptr =PNULL;
	
	fixnv_addr = TF_Get_Valid_NV_Address();
	if(fixnv_addr == 0XFFFFFFFF)
	{
		return ERR_NO_FIND_VALID_FIXNV;
	}

	tf_update_info_ptr = TF_GetUpdateInfo();
	tf_update_info_ptr->tf_update_flash.fixnv_addr = fixnv_addr;

	return ERR_SUCCESS;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

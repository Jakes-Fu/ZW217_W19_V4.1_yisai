/******************************************************************************
 ** File Name:      bl_backup_nvitem.c                                        *
 ** Author:         jiayong.yang                                              *
 ** DATE:           2011-07-11                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the function which used to update fixnv *
 **                 when fdl1 transfer dl_nv/fix_nv data from AP(or PC) to    *
 **                 ram bsd device.                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/11/2011     Jiayong.Yang     Create.                                   *
 *****************************************************************************/
#include <string.h>
#include "sci_types.h"
#include "Sci_ftl_api.h"
#include "nand_partition_operation.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

LOCAL uint8	s_nand_buffer[2048]={0};

PUBLIC uint32 SCI_GetAssertFlag (void)
{
    return 0;
}

/*
	function: read data from specified partition.
	parameter: 
	        part_id: partition ID;
	return value:
	        if ret > 0 nand partition handle;
	        else error value;
*/

uint32 nand_partition_open(uint32 part_id)
{
    uint32      ftlHandle;
	  uint32			nandId=0;
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, part_id, &ftlHandle, 4))
		{
			return NAND_HDL_INVALID;
		}
		return ftlHandle;
}
/*
	function: read data from specified partition.
	parameter: 
	        handle : nand partition handle,return by nand_open API;
	        offset : offset from beginning of the partition,unit is byte;
	        size   : data length to be read from partition;
	        buffer : buffer address where read data will be saved
	return value:
	        if ret > 0 actually read data length;
	        else error value;
	  
*/

uint32 nand_partition_read(uint32 handle,uint32 offset,uint32 size,uint8 *buffer)
{
		SCI_FTL_PARTITION_INFO part_info;
		uint32 start_sector;
		uint32 end_sector;
		uint32 sector_size;
		uint32 offset_in_sector;
		uint32 read_sector_num;
		uint32 read_size=0;

		if(size == 0)
		{
			return 0;
		}
		
		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(handle,&part_info))
		{
			return 0;
		}
		sector_size = part_info.sctSize;
		read_sector_num = sizeof(s_nand_buffer)/sector_size;
		
		start_sector = offset/sector_size;
		if(start_sector >part_info.sctTotalNum)
		{
			return 0;
		}
		offset_in_sector = offset%sector_size;
		if((offset+size) > part_info.sctTotalNum *sector_size)
		{
				size = part_info.sctTotalNum *sector_size-offset;
		}
		end_sector = (offset+size)/sector_size;
		if(offset_in_sector)
		{
			if(SCI_FTL_ERROR_NONE != SCI_FTL_Read(handle, start_sector, 1, s_nand_buffer))
			{
				return 0;
			}

			memcpy(buffer,&s_nand_buffer[offset_in_sector],sector_size - offset_in_sector);
			
			start_sector++;
			read_size = (sector_size - offset_in_sector);
			buffer += read_size;
			
		}
		while(start_sector<=end_sector)
		{
				if(start_sector+read_sector_num < end_sector)
				{
					if(SCI_FTL_ERROR_NONE != SCI_FTL_Read(handle, start_sector, read_sector_num, buffer))
					{
						return read_size;
					}
					
					buffer+=sizeof(s_nand_buffer);
					start_sector+=read_sector_num;
					read_size += sizeof(s_nand_buffer);
				}
				else
				{
					if(SCI_FTL_ERROR_NONE != SCI_FTL_Read(handle, start_sector, end_sector-start_sector, s_nand_buffer))
					{
						return read_size;
					}
					
					memcpy(buffer,s_nand_buffer,size-read_size);
					start_sector = end_sector;
					read_size = size;

					break;
				}
		}
		return read_size;
}

/*
	function: write data to specified partition.
	parameter: 
	        handle : nand partition handle,return by nand_open API;
	        offset : offset from beginning of the partition,unit is byte;
	        size   : data length to be written to partition;
	        buffer : buffer address where read data will be saved
	return value:
	        if ret > 0 actually read data length;
	        else error value;
	notes: 
	        size and offset MUST be sector_size align.
	  
*/
uint32 nand_partition_write(uint32 handle,uint32 offset,uint32 size,uint8 *buffer)
{
		uint32 start_sector;
		uint32 end_sector;
		uint32 sector_size;
		uint32 split_flag=0;
		uint32 write_size;

		SCI_FTL_PARTITION_INFO part_info;
		
		if(size == 0)
		{
			return 0;
		}
		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(handle,&part_info))
		{
			return 0;
		}
		sector_size = part_info.sctSize;
		if(offset%sector_size)
		{
			return 0;
		}
		start_sector = offset/sector_size;
		if(start_sector >part_info.sctTotalNum)
		{
			return 0;
		}
		if((offset+size) > part_info.sctTotalNum *sector_size)
		{
				size = part_info.sctTotalNum *sector_size-offset;
		}
		end_sector = (offset+size)/sector_size;

		if(size%sector_size)
		{
			split_flag = 1;
		}
		if(start_sector!=end_sector)
		{
		    if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(handle, start_sector, end_sector - start_sector+1-split_flag, buffer))
		    {
			    return 0;
		    }
		}
    write_size = sector_size * (end_sector - start_sector+1-split_flag);
		buffer += write_size;
		if(split_flag)
		{
			memset(s_nand_buffer,0xff,sector_size);
			memcpy(s_nand_buffer,buffer,size-write_size);
			if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(handle, end_sector, 1, s_nand_buffer))
			{
				return write_size;
			}
		}
		
		return size;
}
/*
	function: erase the specified partition.
	parameter: 
	        handle : nand partition handle,return by nand_open API;
	return value:
	  
*/
uint32 nand_partitioin_erase(uint32 handle)
{
		SCI_FTL_PARTITION_INFO part_info;
		
		SCI_FTL_GetPartInfo(handle,&part_info);
	  SCI_FTL_Erase(handle,0,part_info.sctTotalNum);
	  return 0;
}
/*
	function: close the specified partition.
	parameter: 
	        handle : nand partition handle,return by nand_open API;
	return value:
	  
*/
int32 nand_partition_ioctl(uint32 handle,uint32 cmd)
{
		SCI_FTL_PARTITION_INFO part_info;
		
		if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(handle,&part_info))
		{
			return -1;
		}
		switch(cmd)
		{
			case NAND_IOCTL_GETSECTORSIZE:
				return part_info.sctSize;
			case NAND_IOCTL_GETPARTSIZE:
				return (part_info.sctSize * part_info.sctTotalNum);
			default:
				break;
				
		}
		return -2;
}
void nand_partition_close(uint32 handle)
{
	SCI_FTL_Close(handle);
}
/*
	function: close the specified partition.
	parameter: 
	        src : partition  from which data is copied ;
	        dst : partition  to which data is copied ;
	return value:
	  
*/
uint32 nand_partiition_copy(uint32 src,uint32 dst)
{
	SCI_FTL_PARTITION_INFO src_info;
	SCI_FTL_PARTITION_INFO dst_info;
	uint32	sector = 0;
	uint32  read_sector_num;
	
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(src,&src_info))
	{
			return 0;
	}
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(dst,&dst_info))
	{
		  return 0;
	}
	read_sector_num = sizeof(s_nand_buffer)/src_info.sctSize;
	if(src_info.sctTotalNum <= dst_info.sctTotalNum)
	{
		SCI_FTL_Erase(dst,0,dst_info.sctTotalNum);
		while(sector<src_info.sctTotalNum)
		{
			if(read_sector_num + sector <= src_info.sctTotalNum)
			{
				SCI_FTL_Read(src, sector, read_sector_num, s_nand_buffer);
				SCI_FTL_Write(dst, sector, read_sector_num, s_nand_buffer);
				sector += read_sector_num;
			}
			else
			{
				SCI_FTL_Read(src, sector, src_info.sctTotalNum-sector, s_nand_buffer);
				SCI_FTL_Write(dst, sector,src_info.sctTotalNum-sector, s_nand_buffer);
				sector = src_info.sctTotalNum;
			}
		}
	}
	return 1;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
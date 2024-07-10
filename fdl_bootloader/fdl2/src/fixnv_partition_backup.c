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
#include "sci_ftl_api.h"
#include "nand_partitionCfg.h"
#include "nand_partition_operation.h"
#include "fdl_nand.h"
#include "NAND_PartitionId.h"
#include "nand_spec.h"
#include "sc_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef POWER_OFF_PROTECT_SUPPORT
typedef struct NV_ITEM_INFO_S
{
	uint16 id;
	uint16 size;
	uint32 addr;
}NV_ITEM_INFO_T;

#define MAX_NV_ITEM_NUM      2048
#define NV_READ_BUF_SIZE      0x10000   
#define INVALID_TIMESTAMP    0xffffffff
#define INVALID_ITEM_ID         0xffff
#define INVALID_ITEM_SIZE     0xffff

static NV_ITEM_INFO_T g_NVItemInfoGroup[MAX_NV_ITEM_NUM];
static __align(4) unsigned char g_NVPartReadBuf[NV_READ_BUF_SIZE];
#if  defined(PLATFORM_SC6800H)
static unsigned short g_checkIdGroup[]={0x1,0x2,0x194,0x193};
#elif  defined(PLATFORM_SC8800G)
static unsigned short g_checkIdGroup[]={0x1,0x2,0x194,0x193,0x516};
/* item id description
  NV_L1_DOWNLOAD_PARAMETER          = 1, 
  NV_L1_CALIBRATION_PARAMETER       = 2,
  NV_CLASSMARK                      = 3,
  NV_GPS_PARAM                      = 403,   // gps param nv 0x193
  NV_TV_RSSI_COMP                   = 404,   // cmmb cali param 0x194
  NV_L1_CALIBRATION_PARAMETER_TD    0x516
*/
#endif
static BOOLEAN BuildNvItemLink(uint32 handle, NV_ITEM_INFO_T* iteminfo,unsigned int infocnt,unsigned int* pTimeStamp);
static BOOLEAN CheckSpecialId(NV_ITEM_INFO_T* iteminfo);

static BOOLEAN JudgeIsPartDateError(unsigned int handle,unsigned int* pTimeStamp)
{


	memset(g_NVItemInfoGroup,0,sizeof(NV_ITEM_INFO_T)*MAX_NV_ITEM_NUM);//init nvitem group
	if(!BuildNvItemLink(handle,g_NVItemInfoGroup,MAX_NV_ITEM_NUM,pTimeStamp))
	{
		return FALSE;
	}
	if(!CheckSpecialId(g_NVItemInfoGroup))
	{
		return FALSE;
	}
	return TRUE;
}

static BOOLEAN BuildNvItemLink(uint32 handle, NV_ITEM_INFO_T* iteminfo,unsigned int infocnt,unsigned int* pTimeStamp)
{

	unsigned int offset=0;
	unsigned int actual_read_size;
	unsigned int partSize;
	unsigned int alreadyReadSize=0;
	unsigned int beforeReadSize=0;
	unsigned int cnt = 0;
	unsigned int* ptimestamp;
	unsigned short* ptmpdate;
	BOOLEAN status = TRUE;
	unsigned int readSize;

	partSize=nand_partition_ioctl(handle,NAND_IOCTL_GETPARTSIZE);

	//memset(g_NVItemInfoGroup,0,sizeof());
	while(offset<partSize)
	{
		if(offset>=alreadyReadSize)
		{
			if(alreadyReadSize+NV_READ_BUF_SIZE>partSize)
			{
				if(alreadyReadSize<partSize)
				{
					readSize=partSize-alreadyReadSize;
				}
				else
				{
					status = FALSE;
					break;
				}
			}
			else
			{
				readSize=NV_READ_BUF_SIZE;
			}
			actual_read_size = nand_partition_read(handle,alreadyReadSize,readSize,g_NVPartReadBuf);
			if(actual_read_size == 0)
			{
				status= FALSE;
				break;
			}
			beforeReadSize=alreadyReadSize;
			alreadyReadSize+=actual_read_size;
		}
		
		if(0==offset)
		{
			ptimestamp=(unsigned int*)g_NVPartReadBuf;
			if(INVALID_TIMESTAMP==*ptimestamp||0==*ptimestamp)
			{
				status = FALSE;
				break;
			}
			*pTimeStamp=*ptimestamp;
			offset+=sizeof(*ptimestamp);
		}
		else
		{
			if(offset>=alreadyReadSize)
			{
				continue;
			}
			if(offset<beforeReadSize)
			{
				status = FALSE;
				break;
			}
			ptmpdate=(unsigned short*)(g_NVPartReadBuf+offset-beforeReadSize);
			if(INVALID_ITEM_ID==*ptmpdate)
			{
				status = TRUE;
				break;
			}
			if(INVALID_ITEM_SIZE==*(ptmpdate+1)||0==*(ptmpdate+1))
			{
				status = FALSE;
				break;
			}
			iteminfo[cnt].id=*ptmpdate;
			iteminfo[cnt].size=*(ptmpdate+1);
			iteminfo[cnt].addr=offset;
			offset=offset+(sizeof(*ptmpdate))*2;
			offset += iteminfo[cnt].size;
			offset=(offset+3)&0xfffffffc;
			cnt++;
			if(cnt>=infocnt)
			{
				status= FALSE;
				break;
			}
			if(offset>partSize)
			{
				status = FALSE;
				break;
			}
			
		}
	}

	if(TRUE==status)
	{
		while(alreadyReadSize<partSize)
		{
			if(alreadyReadSize+NV_READ_BUF_SIZE>partSize)
			{
					readSize=partSize-alreadyReadSize;
			}
			else
			{
				readSize=NV_READ_BUF_SIZE;
			}
			actual_read_size = nand_partition_read(handle,alreadyReadSize,readSize,g_NVPartReadBuf);
			if(actual_read_size==0)
			{
				status= FALSE;
				break;
			}
			alreadyReadSize+=actual_read_size;
		}
	}
	return status;

}

static BOOLEAN FindNvItemId(NV_ITEM_INFO_T* iteminfo,unsigned short itemId)
{
	unsigned int cnt;
	BOOLEAN status = FALSE;
	if(NULL==iteminfo||INVALID_ITEM_ID==itemId)
	{
		return status;
	}
	
	for(cnt=0;cnt<MAX_NV_ITEM_NUM;cnt++)
	{
		if(0==iteminfo[cnt].id)
		{
			break;
		}
		if(itemId==iteminfo[cnt].id)
		{
			status=TRUE;
			break;
		}
	}
	return status;
}

static BOOLEAN CheckSpecialId(NV_ITEM_INFO_T* iteminfo)
{
	unsigned int cnt;
	unsigned int spcialIdNum;
	spcialIdNum=sizeof(g_checkIdGroup)/sizeof(unsigned short);
	for(cnt=0;cnt<spcialIdNum;cnt++)
	{
		if(!FindNvItemId(iteminfo,g_checkIdGroup[cnt]))
		{
			return FALSE;
		}
	}
	return TRUE;
}


PUBLIC void BackupOrUpdateNvPart(void)
{
	
	BOOLEAN FixNvStatus,ProNvStatus;
	unsigned int FixNvtimestamp,ProNvtimestamp;
	uint32	master_part_handle=0;
	uint32  backup_part_handle=0;

	master_part_handle=nand_partition_open(SCI_PARTITION_DOWNLOADED_NV);
	if(master_part_handle==NAND_HDL_INVALID)
	{
		
		return ;
	}
	backup_part_handle=nand_partition_open(SCI_PARTITION_PROTECT_NV);
	if(backup_part_handle==NAND_HDL_INVALID)
	{
    nand_partition_close(master_part_handle);				
		return ;
	}	
	FixNvStatus=JudgeIsPartDateError(master_part_handle, &FixNvtimestamp);
	ProNvStatus=JudgeIsPartDateError(backup_part_handle,&ProNvtimestamp);

	if(FALSE==FixNvStatus&&FALSE==ProNvStatus)
	{
    nand_partition_close(backup_part_handle);
    nand_partition_close(master_part_handle);		
		return;
	}	
	else if(FALSE==FixNvStatus&&TRUE==ProNvStatus)
	{
		nand_partiition_copy(backup_part_handle, master_part_handle);
	}

	else if(TRUE==FixNvStatus&&FALSE==ProNvStatus)
	{
		nand_partiition_copy(master_part_handle,backup_part_handle);
	}
	else if(TRUE==FixNvStatus&&TRUE==ProNvStatus)
	{
		if(FixNvtimestamp!=ProNvtimestamp)
		{
			nand_partiition_copy(master_part_handle, backup_part_handle);
		}
	}
  nand_partition_close(backup_part_handle);
  nand_partition_close(master_part_handle);
	return;
}

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
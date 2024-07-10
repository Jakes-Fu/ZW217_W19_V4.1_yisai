
#include "sci_types.h"
#include "NAND_PartitionId.h"
#include "sci_ftl_api.h"
#include "boot1_copy_kernel_img.h"
#include "bootparam_def.h"
#include "bl_trace.h"
#include <string.h>

#define PG_MAX_SIZE	512
static uint8			s_tmpBuf[PG_MAX_SIZE];

/******************************************************************************
 * copy_img
 *
 * Parameters:
 * 		ftlHandle - handle of partition
 *      dst   - Destition address.	This address maybe 0
 *      pgId  - First pageID, measured from the start of OS partition.
 *      pgNum - Size of OS image in pages.
 *
 * Return values:
 *      0 - The OS image is copied to SDRAM successfully.
 *      1 - NAND flash error.
 ******************************************************************************/
static BOOLEAN _copyImg(
	SCI_FTL_HANDLE ftlHandle,
	uint8* dst,
	uint32 sctId, uint32 sctNum, uint32 sctSize
)
{
	uint32 rRet;

	rRet = SCI_FTL_Read(ftlHandle, sctId, 1, s_tmpBuf);
	if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK != rRet))
	{
		return FALSE;
	}
	memcpy(dst,s_tmpBuf,sctSize);
	dst += sctSize;
	sctId++;
	sctNum--;
	if(0 == sctNum)
	{
		return TRUE;
	}
	rRet = SCI_FTL_Read(ftlHandle, sctId, sctNum, dst);
	if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK != rRet))
	{
		return FALSE;
	}
	return TRUE;
}

#ifdef DEV_MANAGE_SUPPORT
#define UPDATE_TRIGGER 0x01
BOOLEAN copyUaImg(void)
{
	uint32	rRet;
	uint32	nandId = 0;
	SCI_FTL_HANDLE	ftlHandle;
	SCI_FTL_PARTITION_INFO ftlPartitionInfo;
	unsigned int update_flag = 0;
	do
	{
		rRet = SCI_FTL_Open(nandId, SCI_PARTITION_DU, &ftlHandle, 0);
		if(SCI_FTL_ERROR_NONE != rRet)
		{
			break;
		}
		rRet = SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo);
		if(SCI_FTL_ERROR_NONE != rRet)
		{
			break;
		}
		rRet = SCI_FTL_Read(ftlHandle, ftlPartitionInfo.sctTotalNum-1, 1, s_tmpBuf);
		if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK != rRet))
		{
			break;
		}
		update_flag = *(unsigned long *)&s_tmpBuf[ftlPartitionInfo.sctSize-4];
	    if (UPDATE_TRIGGER == (UPDATE_TRIGGER&update_flag))
	    {
	        break;
	    }
		SCI_FTL_Close(ftlHandle);

		rRet = SCI_FTL_Open(nandId, SCI_PARTITION_UA, &ftlHandle, 0);
		if(SCI_FTL_ERROR_NONE != rRet)
		{
			break;
		}
		rRet = SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo);
		if(SCI_FTL_ERROR_NONE != rRet)
		{
			break;
		}
		if(FALSE == _copyImg(ftlHandle, SDRAM_BASE_ADDR, 0, ftlPartitionInfo.sctTotalNum,ftlPartitionInfo.sctSize))
		{
			break;
		}
		SCI_FTL_Close(ftlHandle);
		return TRUE;
	}while(0);

	SCI_FTL_Close(ftlHandle);
	return FALSE;
}

#undef UPDATE_TRIGGER
#endif


#ifdef _LITTLE_ENDIAN
#define BYTESWITCH(s) ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))
#endif
static const uint8 MAGIC[] = { 'S', 'T', 'B', 'P' };

BOOLEAN copyKernelImg(void)
{
	uint32	rRet;
	uint32	nandId;
	SCI_FTL_HANDLE	ftlHandle;
	SCI_FTL_PARTITION_INFO ftlPartitionInfo;

	BOOT_PARAM*	bootParam;
	OS_INFO*	osInfo;
	uint32		OSPartSz;
	
#if 0
    uint32 i;
#endif

	nandId = 0;

	rRet = SCI_FTL_Open(nandId, SCI_PARTITION_OS, &ftlHandle, 0);
	if(SCI_FTL_ERROR_NONE != rRet)
	{
		return FALSE;
	}
	rRet = SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo);
	if(SCI_FTL_ERROR_NONE != rRet)
	{
		return FALSE;
	}
	rRet = SCI_FTL_Read(ftlHandle, 0, 1, s_tmpBuf);
	if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK != rRet))
	{
		return FALSE;
	}

	bootParam = (BOOT_PARAM*)s_tmpBuf;
#ifdef _LITTLE_ENDIAN
    ( bootParam->size)			= BYTESWITCH ( bootParam->size);
    ( bootParam->total_size)	= BYTESWITCH ( bootParam->total_size);
    ( bootParam->os_info_size)	= BYTESWITCH ( bootParam->os_info_size);
    ( bootParam->os_offset)		= BYTESWITCH ( bootParam->os_offset);
#endif
#if 0
	if (bootParam->trace_on)
	{
		BL_TRACE0 ("\r\nboot parameters:\r\n");
		BL_TRACE0 ("magic:        ");
		for (i = 0; i < sizeof bootParam->magic; i++)
		{
			BL_TRACE ("%c", bootParam->magic[i]);
		}
		BL_TRACE0 ("\r\n");
		BL_TRACE ("size:         %d(bytes)\r\n",	bootParam->size);
		BL_TRACE ("total size:   %d(bytes)\r\n",	bootParam->total_size);
		BL_TRACE ("time stamp:   0x%x\r\n",		bootParam->time_stamp);
		BL_TRACE ("current os:   %d\r\n",			bootParam->current_os);
		BL_TRACE ("num of os:    %d\r\n",			bootParam->num_of_os);
		BL_TRACE ("os info size: %d(bytes)\r\n",	bootParam->os_info_size);
		BL_TRACE ("os offset:    %d(bytes)\r\n",	bootParam->os_offset);
	}
#endif
	if(
		(!(
		(bootParam->magic[0] == MAGIC[0])
		&&(bootParam->magic[1] == MAGIC[1])
		&&(bootParam->magic[2] == MAGIC[2])
		&&(bootParam->magic[3] == MAGIC[3])
	))
	||(sizeof (BOOT_PARAM) != bootParam->size)
	||(sizeof (OS_INFO) != bootParam->os_info_size)
	||(0 == bootParam->num_of_os)
	||(bootParam->current_os >= bootParam->num_of_os)
	)
	{
		SCI_FTL_Close(ftlHandle);
		BL_TRACE0 ("Incompatible version of boot parameters.\r\n");
		return FALSE;
	}

	for( ; ; )
	{
		osInfo = (OS_INFO*)(&s_tmpBuf[bootParam->os_offset+sizeof(OS_INFO)*bootParam->current_os]);
#ifdef _LITTLE_ENDIAN
		osInfo->offset	= BYTESWITCH (osInfo->offset);
		osInfo->size	= BYTESWITCH (osInfo->size);
#endif
#if 0
		if (bootParam->trace_on)
		{
			BL_TRACE0 ("\r\nOS infomation:\r\n");
			BL_TRACE ("name:        %s\r\n",			osInfo->name);
			BL_TRACE ("description: %s\r\n",			osInfo->description);
			BL_TRACE ("version:     %s\r\n",			osInfo->version);
			BL_TRACE ("offset:      %d(sectors)\r\n",	osInfo->offset);
			BL_TRACE ("size:        %d(sectors)\r\n",	osInfo->size);
		}
#endif
		OSPartSz = ftlPartitionInfo.sctTotalNum;
		if(
			(osInfo->offset >= OSPartSz)
			||((osInfo->offset+osInfo->size) > OSPartSz)
		)
		{
			BL_TRACE0 ("Invalid OS offset or size, exceeding the boundary "
				"of the OS partition.\r\n");
			if(0 != bootParam->current_os)
			{
				bootParam->current_os = 0;
				continue;
			}
			else
			{
				SCI_FTL_Close(ftlHandle);
				return FALSE;
			}
		}
		break;
	}

	if(FALSE == _copyImg(ftlHandle, SDRAM_BASE_ADDR, osInfo->offset, osInfo->size,ftlPartitionInfo.sctSize))
	{
		BL_TRACE ("copy_img failed\r\n");
	}
	SCI_FTL_Close(ftlHandle);
	return TRUE;
}

BOOLEAN LoadImg(void)
{
#ifdef DEV_MANAGE_SUPPORT
	if(TRUE == copyUaImg())
	{
		return TRUE;
	}
#endif
	if(TRUE == copyKernelImg())
	{
		return TRUE;
	}
	return FALSE;
}


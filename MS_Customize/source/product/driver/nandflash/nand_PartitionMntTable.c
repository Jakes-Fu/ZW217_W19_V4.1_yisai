
/******************************************************************************
 ** File Name:      prod_param.c                                               *
 ** Author:                                                                    *
 ** DATE:                                                                      *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some product parameter defination.       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/
#include "sci_ftlmnt_Engin.h"
#include "NAND_PartitionId.h"
#include "xsr_partition.h"

static const SCI_FTLMNT_PARTITION_INFO sysPartMntTbl =
{
	0,
	{
		{SCI_PARTITION_CALIPARAM,			BML_PRODUCT_PART_NAME,			0,	0,	(const void*)0},
		{SCI_PARTITION_DOWNLOADED_NV,		BML_DOWNLOADED_NV_PART_NAME,	0,	0,	(const void*)0},
		{SCI_PARTITION_PROTECT_NV,		BML_PROTECT_NV_NAME,	0,	0,	(const void*)0},
		{SCI_PARTITION_MMI_RESOURCE,		BML_MMIRESOURCE_NAME,			0,	0,	(const void*)0},
#ifndef VM_SUPPORT
		{SCI_PARTITION_DEMANDONOS,			BML_DEMANDONOS_NAME,			0,	0,	(const void*)0},
#endif
		{SCI_PARTITION_OSC,					BML_KERNELOS2_NAME,				0,	0,	(const void*)0},
		{SCI_PARTITION_NV,					STL_NV_FS_PART,					0,	0,	(const void*)4},
		{SCI_PARTITION_DU,					STL_DU_PART,					0,	0,	(const void*)4},
	    {SCI_PARTITION_DSPCODE,				BML_DSPCODE_PART_NAME,			0,	0,	(const void*)0},

	    {SCI_PARTITION_FILESYSTEM,			STL_UDISK_FS_PART,				0,	0,	(const void*)7},
	    {SCI_PARTITION_HIDDEN_FILESYSTEM,	STL_UDISK_HIDDEN_FS_PART,		0,	0,	(const void*)7},
	    {SCI_PARTITION_ROMDISK_FILESYSTEM,	BML_ROMDISK_FS_PART,			0,	0,	(const void*)7},
	  	{0,									0,								0,	0,	(const void*)0}
	} /*lint !e785*/
};

const SCI_FTLMNT_PARTITION_INFO* SCI_GetPartitionMntTable(void)
{
	return &sysPartMntTbl;
}



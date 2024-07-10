/******************************************************************************
 ** File Name:      sci_ftlmnt_Engin.h                                        *
 ** Author:         Jason.Wu                                                  *
 ** DATE:           2010.12.14                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.12.14   Jason.Wu              Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _FTL_MNT_ENGIN_H_
#define _FTL_MNT_ENGIN_H_
/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
#define SCI_FTLMNT_REMAIN_AREA	((uint32)-1)
typedef struct
{
	//-------------------------------------------------------------
	//	partId or father id, all is father id in current platform
	//-------------------------------------------------------------
	uint32					id;
	/* name in BSD */
	const char				*bsdName;
	/* 分区起始位置, used in slave partition*/
	uint32					sctStart;
	uint32					sctNum;
	/* 扩展用 param that used in SCI_FTL_Open */
	const void				*extParam;
}SCI_FTLMNT_PARTITION_ELM;

typedef struct
{
	uint32							nandId;
	const SCI_FTLMNT_PARTITION_ELM	partE[100];
}SCI_FTLMNT_PARTITION_INFO;

/*************************************************************************************/
/*   Author : Jason.Wu                                                               */
/*   Descript:                                                                       */
/*       MOCOR platform call this function to mount ftl partition to BSD module.     */
/*   Param: none                                                                     */
/*   Return: the number of partitions that has been mounted to BSD                   */
/*                                                                                   */
/*                                                                                   */
/*************************************************************************************/
PUBLIC uint32 SCI_FTLMnt_Init(void);


/*************************************************************************************/
/*   Author: Jason.Wu                                                                */
/*   Descript:                                                                       */
/*        cutom drv must relize this function . SCI_FTLMnt_Engin will use this api to*/
/*        get mount table , that indicate the relation ship between partition ID and */
/*        BSD name.                                                                  */
/*        MOCOR platform will access nand data by bsd module                         */
/*                                                                                   */
/*                                                                                   */
/*                                                                                   */
/*************************************************************************************/
PUBLIC const SCI_FTLMNT_PARTITION_INFO* SCI_GetPartitionMntTable(void);

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

/******************************************************************************
 ** File Name:      nand_partition_operation.h                                *
 ** Author:         jiayong.yang                                              *
 ** DATE:           2011-07-11                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the fdl update fixnv data struct        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/11/2011     jiayong.yang      Create.                                   *
 *****************************************************************************/
 
#ifndef FDL_NAND_PART_OPERATION_H
#define FDL_NAND_PART_OPERATION_H

#define	NAND_HDL_INVALID	0xFFFFFFFF

#define NAND_IOCTL_GETSECTORSIZE	0
#define NAND_IOCTL_GETPARTSIZE    1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
uint32 nand_partition_open(uint32 part_id);
uint32 nand_partition_read(uint32 handle,uint32 offset,uint32 size,uint8 *buffer);
uint32 nand_partition_write(uint32 handle,uint32 offset,uint32 size,uint8 *buffer);
uint32 nand_partitioin_erase(uint32 handle);
int32 nand_partition_ioctl(uint32 handle,uint32 cmd);
void nand_partition_close(uint32 handle);
uint32 nand_partiition_copy(uint32 src,uint32 dst);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FDL_NAND_PART_OPERATION_H */
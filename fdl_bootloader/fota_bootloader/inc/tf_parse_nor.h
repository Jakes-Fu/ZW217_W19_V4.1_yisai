/******************************************************************************
 ** File Name:      TF_parse.h                                                *
 ** Author:           tao.feng                                                 *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of TF download process  *
            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/12/2010     tao.feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _TF_PARSE_H_
#define _TF_PARSE_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "flash.h"
#include "tf_cfg.h"
#include "ff.h"


/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/
#define MAX_PATITION_NUM			 	16
#define MAX_PS_PARSE_SIZE                  	8*1024
#define SINGLE_CS_MAX_FLASH_SIZE         (0x1000000)
#define DUAL_CS_OFFSET					0xc000000	
#define MAX_SECARCH_RANGE      	      	       0x1000
#define FLASH_CFG_OFFSET                         0x64//0x60	//6800H: 0x60 09A: 0x40
#define FLASH_START_ADRESS             	       0x0
#define SRAM_BASE_ADDR 				0x4000000 ////move to common.h
#define SLAVE_SECTOR_BUF_ADDESS           (0x04180000)
#define FIX_NV_LEN_OFFSET				(16)
#define PRD_TAIL_INFO_LEN              		(8)
#define PRD_INFO_CRC_OFFSET			(6)
#define PRD_INFO_OFFSET	        		(12)//(4)
#define PRD_STATUS_OFFSET				(2)
#define SECTOR_STATUS_OFFSET			(2)
#define ERR_NV_LEN                              		0xffff
#define ITEM_MASK_ID            		      		0x1fff         
#define EFS_INVALID_ITEM         	     		0xffff
#define ERR_VALID_NV_ADDR              		0xffffffff
#define FLASH_START_ADRESS_CS3      	0x3C000000


/**---------------------------------------------------------------------------*
 **                         Struct Defination                                  *
 **---------------------------------------------------------------------------*/
/*struct definition*/
#if 0
typedef struct flash_partition_s
{
	uint32 NvAdress;
	uint32 PsAdress;
	uint32 ResAdress;
	uint32 EfsAdress;
	uint32 ProdinfoAdress;
	uint32 EfsSize;
	uint32 PSSize;                /*ps partition size     */
	uint32 ResSize;	              /*res partiton size  */
	uint32 NvSize ;               /*fixed nv partiton size     */    
	uint32 FlashSize;
	uint32 UmemAdress;
	uint32 sploadaddr; /*lcd and sensor addr*/
	uint32 sploadsize;/*lcd and sensor size*/
	uint32 UmemSize;
	uint32 Sec_size;
}TF_PARTITION_T,*TF_PARTITION_PTR;
#endif
	
typedef struct flash_partition_s
{
    uint32 NvAdress;
    uint32 PsAdress;
    uint32 ResAdress;
    uint32 EfsAdress;
    uint32 ProdinfoAdress;
    uint32 UmemAdress;
    uint32 sploadaddr;       /*lcd  and sensor bin load address*/
    uint32 operdata_addr;
#ifdef CHIP_VER_UWS6121E
    uint32 apn_addr;
    uint32 apn_size;
    uint32 config_nv_addr;
    uint32 config_nv_size;	 
    uint32 cp_addr;
    uint32 cp_size;	 
#endif
    uint32 EfsSize;
    uint32 PSSize;                /*ps partition size             */
    uint32 ResSize;               /*res partiton size            */
    uint32 NvSize ;               /*fixed nv partiton size     */
    uint32 FlashSize;
    uint32 UmemSize;
    uint32 sploadsize;
    uint32 operdata_size;
}TF_PARTITION_T,*TF_PARTITION_PTR;


/**---------------------------------------------------------------------------*
 **                         	Export Func                                   *
 **---------------------------------------------------------------------------*/
#if 0
/*****************************************************************************/
//  Description:   get top or top-bottom structure flash top small sector info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC FLASH_STRUCT_E  TF_GetFlashStru(uint32 *small_sec_size, uint32 *sma_sec_num);
#endif

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC NORMCP_SPEC_PTR  TF_GetNormcpSpec(void);


/*****************************************************************************/
//  Discription: This func will parse ps.bin and get flash partition info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TF_PARTITION_PTR TF_GetPartInfoInPac(const uint8 *parsebuf, FIL *fp);

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_InEfsSpace(uint32 addr);

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
 PUBLIC uint32 TF_EfsAddrConvert(uint32 addr);

/*****************************************************************************/
//  Description:   get nomcp spec info
//  Global resource dependence: should be called after TF_GetPartInfoInPac has beedb called
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_PARTITION_PTR TF_GetPartitionInfo(void);


/*****************************************************************************/
//  Discription: This func will parse ps.bin and get flash partition info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  NOR_FLASH_CONFIG_PTR TF_GetPartInfoInPs(void);


/*****************************************************************************/
//  Discription: This func will get  valid fixnv address in ps and update tf update info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TFLOAD_ERR_TYPE TF_UpdateFixnvAddress(void);



#endif //_TF_PARSE_H_

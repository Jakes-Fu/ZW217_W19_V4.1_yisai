/******************************************************************************
 ** File Name:      TF_cfg.c                                               *
 ** Author:           tao.feng                                              *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/10/2010     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "tf_cfg.h"
#include "nv_item_id.h"
//#ifdef TF_LOAD_PROTECT
#include "pinmap.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "tb_comm.h"
#include "tb_hal.h"
#include "gpio_drv.h"
#include "tf_load.h"
//#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
//#undef __MODULE__ 
//#define __MODULE__ "tf_cfg.c"

const uint16 g_tfload_dir_wstr[] = { 'b', 'l', '_','u', 'p','d', 'a' ,'t', 'e',0 };
const uint16 g_tfload_updatefile_wstr[]= { 't', 'f', 'l', 'o', 'a', 'd', '.', 'p', 'a', 'c', 0 };
const uint16 g_tfload_bootloader_wstr[]= { 't', 'f', 'b', 'o', 'o', 't', '.', 'b', 'i', 'n', 0 };
const uint16 m_szPacPrdName[] = { 'N', 'O', 'R', '_', 'F', 'L', 'A', 'S', 'H', '_', 'U','I', 'X', '8', '9', '1', '0', 0 };

//disable irq/fiq
#define TF_IRQ_FIQ_DISABLE  			  0xC0  

#define _L(x) L ## x

/*file id enum,should compatible witch .xml*/
LOCAL const uint16* sz_fileID[MAX_FILE_ID_TYPE] = 
{
    _L("FDL"),
    _L("NV"),
    _L("PS"),
    _L("MMIRes"),
    _L("EraseUdisk"),
    _L("UDiSK"),
    _L("FLASH"),
    _L("PhaseCheck"),
    _L("SPLOAD"),
    _L("CP"),
    _L("OperatorData"),
    _L("ConfigNV"),
    _L("APN")
};

/* It is for display download status string */
LOCAL const char* sz_filetype[MAX_FILE_ID_TYPE] = 
{
    "Download FDL",
    "Download NV",
    "Download PS",
    "Download MMIRes",
    "EraseUdisk",
    "Download UDiSK",
    "FLASH",
    "PhaseCheck",
    "Spload",
    "Download CP",
    "Download IMS",
    "Download ConfigNV",
    "Download APN",
};

/*.pac  file info*/
LOCAL TF_PARTITION_INFO partition_info[MAX_FILE_ID_TYPE] = 
{
    {FDL,           TF_PARTITION_TYPE_IGNORE,       0,          0,  0},  //FDL
    {NV,            TF_PARTITION_TYPE_FILE,         0,          0,  0},  //NV
    {PS,            TF_PARTITION_TYPE_FILE,         0,          0,  0},  //PS
    {MMIRes,        TF_PARTITION_TYPE_FILE,         0,          0,  0},  //MMIRes
    {EraseUdisk,    TF_PARTITION_TYPE_IGNORE,       0,          0,  0},  //EraseUdisk
    {UDiSK,         TF_PARTITION_TYPE_IGNORE,       0,          0,  0},  //UDiSK
    {FLASH,         TF_PARTITION_TYPE_IGNORE,       0,          0,  0},  //FLASH
    {PhaseCheck,    TF_PARTITION_TYPE_IGNORE,       0,          0,  0},  //PhaseCheck
    {SPLOAD,        TF_PARTITION_TYPE_FILE,         0,          0,  0},  //spload
    {CP,            TF_PARTITION_TYPE_FILE,         0,          0,  0},  //cp
    {IMS,           TF_PARTITION_TYPE_FILE,         0,          0,  0},  //delta_nv
    {ConfigNV,      TF_PARTITION_TYPE_FILE,         0,          0,  0},  //config_nv
    {APN,           TF_PARTITION_TYPE_FILE,         0,          0,  0},  //apn data
};


/* It is for display tf download err type */
LOCAL const char* sz_error_type[TF_LOAD_ERR_TYPE_MAX] = 
{
    "Download Success",          // ERR_SUCCESS
    "can't find load file",      // ERR_FIND_FILE
    "open file fail",            // ERR_OPEN_FILE
    "no find ps in pac",         // ERR_NO_PS_IN_PAC
    "too many file",             // ERR_TOO_MANY_FILE
    "find no update file",       // ERR_NO_UPDATE_FILE
    "file id err",               // ERR_FILE_ID
    "read file fail",            // ERR_READ_FILE
    ".pac version err",          // ERR_PAC_VERSION
    ".pac not match",            // ERR_PAC_PRDNAME
    ".pac file num err" ,        // ERR_PAC_FILE_NUM,
    ".pac file size err",        // ERR_PAC_FILE_SIZE
    "backup fixnv fail",         // ERR_BACKUP_FIXNV
    "backup product info fail",  // ERR_BACKUP_PRODUCTINFO
    "open flash fail",           // ERR_FLASH_OPEN
    "erase flash fail",          // ERR_FLASH_ERASE
    "program flash fail",        // ERR_ FLASH_PROGRAM
    "read flash fail",           // ERR_FLASH_READ
    "udisk size overflow",       // ERR_UDISK_SIZE_OVERFLOW,
    "udisk img type not compatible with flash type",                 // ERR_UDISK_IMG_TYP
    "no find flash partition info in ps",                            //ERR_NO_FIND_FLASH_PARTITION_INFO
    "no find compatible file or operatuion in partition info table", //ERR_NO_FIND_COMPATIBLE_FILE
    "no defined file type",      // ERR_NO_DEFINED_FILE_TYPE
    "backup nv fail",            // ERR_BACKUP_NV
    "fixnv size overflow",       // ERR_FIXNV_SIZE
    "fixnv structure destroy",   // ERR_FIXNV_STRU
    "product size err",          // ERR_PRODUCT_SIZE
    "crc err",                   // ERR_CRC
    "err product addr cfg",      // ERR_PRODUCT_ADDR
    "no find valid fixnv"        // ERR_NO_FIND_VALID_FIXNV
}; /*lint !e785*/

/*customer defined fixnv backup  item id*/
LOCAL const TF_CUST_BACK_ITEM_T tf_cust_back_item_table[] =
{
    {0x02,    "calibration"},
    {0x05,    "Imei"},
    {0x179,   "Imei2"},
    {0x191,   "BlueTooth"},
    {0x0D,    "BandSelect"},
    // {0x19A,   "MMITest"},
    {0x199,   "WIFI"},
    {0x1F1,   "SimlockUserData"},
    {0x26D,   "RfCalibGSM"},
    {0x26E,   "RfCalibLTE"},
    {0x26F,   "GoldenBoard"},
    {0x1BF,   "License"},
    {0x1AF,   "EngTestResult"}
};

TF_UPDATE_INFO tf_update_info;

/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Function name:	TF_GetBackItemTable
//  Description:This function is used to get back item table.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_CUST_BACK_ITEM_PTR TF_GetBackItemTable(void)
{
    return (TF_CUST_BACK_ITEM_T*)&tf_cust_back_item_table[0];
}

/*****************************************************************************/
//  Function name: TF_GetBackItemTable size
//  Description:This function is used to get back item table.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_GetBackItemTableSize(void)
{
    return sizeof(tf_cust_back_item_table)/sizeof(tf_cust_back_item_table[0]);
}

/*****************************************************************************/
//  Function name:  TF_GetPartTable
//  Description:This function is used to get part info table.(part info  from .pac)
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_PARTITION_INFO *TF_GetPartTable(void)
{

    return (TF_PARTITION_INFO*)&partition_info[0];
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif  // End of 


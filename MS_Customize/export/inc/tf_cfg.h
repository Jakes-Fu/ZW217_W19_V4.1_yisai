
#ifndef _TF_CFG_H_
#define _TF_CFG_H_
#include "sci_types.h"


//file name/dir name only support ascii name.
#undef  TF_LFN_EN

//define share ram base address
#define  TFLOAD_SHARE_RAM_ADDRESS          0x800000  //iram start address
//defien max err type num
#define TF_LOAD_ERR_TYPE_MAX  		 	    0x40

extern const uint16 g_tfload_dir_wstr[];
extern const uint16 g_tfload_updatefile_wstr[];
extern const uint16 g_tfload_bootloader_wstr[];
extern const uint16 m_szPacPrdName[];

//customer dedicated update file dir
#define TFLOAD_DEFAULT_DIR             		  (g_tfload_dir_wstr)// "bl_update"
//customer dedicated update file dir len
#define TFLOAD_DEFAULT_DIR_DIR_LEN        9
//customer dedicated update file name
#define TFLOAD_DEFAULT_FILENAME             (g_tfload_updatefile_wstr)// "tfload.pac"
//customer dedicated update file name
#define TFLOAD_DEFAULT_FILELEN           	  10
//customer dedicated bootloader file name
#define TFLOAD_DEFAULT_BOOTNAME            (g_tfload_bootloader_wstr)// "tfboot.bin"
//customer dedicated bootloader file len
#define TFLOAD_DEFAULT_BOOTLEN           	  10
//PAC product name
#define TFLOAD_PRODUCT_NAME            		(m_szPacPrdName)// PRODUCT NAME: NAND_FLASH_6800H" or "NOR_FLASH_6800H" or "SC8800G"
//define tfload version flag
#define  TF_VERSION_FLASG				  0x54465631 //TFV1

#define		INVALID_NVID		(0xFFFF)

/*cutsomer defined should backup item id and description*/
typedef struct _F_CUST_BACK_ITEM_T
{
	uint16 id;
	char *item_desc;
}TF_CUST_BACK_ITEM_T, *TF_CUST_BACK_ITEM_PTR;

/*.pac include file type*/
typedef enum
{
	TF_PARTITION_TYPE_FILE=0,//should download to flash
	TF_PARTITION_TYPE_CMD,//should execute this cmd
	TF_PARTITION_TYPE_IGNORE,//should ignore this file or operation
	TF_PARTITION_TYPE_MAX
}TF_PARTITION_TYPE_ENUM;

#ifdef NANDBOOT_SUPPORT
#ifdef PLATFORM_SC8800G
/*.pac include file id,should compatible with sz_fileID and  sz_filetype*/
typedef enum
{
	FDL=0x0,
	FDL2,
	NV,
	BootLoader,
	KernelImg2,
	UserImg,
	MMIRes,
	DSPCode,
	OmadmFota,
	Preload,
	FLASH,
	EraseDU,
	PhaseCheck,
	KernelImg,
	RomDisk,
	MAX_FILE_ID_TYPE
}FILE_ID_TYPE;
#else	//PLATFORM_SC6800H
/*.pac include file id,should compatible with sz_fileID and  sz_filetype*/
typedef enum
{
	FDL=0x0,
	FDL2,
	NV,
	BootLoader,
	KernelImg,
	UserImg,
	MMIRes,
	FLASH,
	PhaseCheck,
	FormatFS,
	FormatHFS,
	Preload,
	RomDisk,
	MAX_FILE_ID_TYPE
}FILE_ID_TYPE;
#endif

#else
/*.pac include file id,should compatible with sz_fileID and  sz_filetype*/
typedef enum
{
	FDL=0x0,
	NV,
	PS,
	MMIRes,
	EraseUdisk,
	UDiSK,
	FLASH,//efs start address
	PhaseCheck,
	SPLOAD, //lcd,sensor driver partiton
#if defined(CHIP_VER_UWS6121E)
	CP, // modem code
	IMS,
	ConfigNV,
	APN,
#endif
	MAX_FILE_ID_TYPE
}FILE_ID_TYPE; // FOR_SPI_FLASH
#endif

/*download process err id,should compatible with sz_error_type*/
typedef enum
{
	ERR_SUCCESS=0x0,
	ERR_FIND_FILE,
	ERR_OPEN_FILE,
	ERR_NO_PS_IN_PAC,
	ERR_TOO_MANY_FILE,
	ERR_NO_UPDATE_FILE,
	ERR_FILE_ID,
	ERR_READ_FILE,
	ERR_PAC_VERSION,
	ERR_PAC_PRDNAME,
	ERR_PAC_FILE_NUM,
	ERR_PAC_FILE_SIZE,
	ERR_BACKUP_FIXNV,
	ERR_BACKUP_PRODUCTINFO,
	ERR_FLASH_OPEN,
	ERR_FLASH_ERASE,
	ERR_FLASH_PROGRAM,
	ERR_FLASH_READ,
	ERR_UDISK_SIZE_OVERFLOW,
	ERR_UDISK_IMG_TYPE,
	ERR_NO_FIND_FLASH_PARTITION_INFO,
	ERR_NO_FIND_COMPATIBLE_FILE,
	ERR_NO_DEFINED_FILE_TYPE,
	ERR_BACKUP_NV,
	ERR_FIXNV_SIZE,
	ERR_FIXNV_STRU,
	ERR_PRODUCT_SIZE,
	ERR_CRC,
	ERR_PRODUCT_ADDR,
	ERR_NO_FIND_VALID_FIXNV
}TFLOAD_ERR_TYPE;

/*tf load file info*/
typedef struct TF_PARTITION_INFO_TAG
{
	FILE_ID_TYPE	 file_id;
	TF_PARTITION_TYPE_ENUM  file_type;
	uint32 part_id;
	uint32 base_address;	//n1stVbn
	uint32 size;			//nNumOfBlks
} TF_PARTITION_INFO;


/*****************************************************************************/
//  Function name:	TF_GetBootExecAddress
//  Description:This function is used to get tf bootloader.bin start exec address.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_GetBufSize(void);


/*****************************************************************************/
//  Function name:	TF_GetBackItemTable
//  Description:This function is used to get back item table.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_CUST_BACK_ITEM_PTR TF_GetBackItemTable(void);

/*****************************************************************************/
//  Function name:	TF_GetBackItemTable size
//  Description:This function is used to get back item table.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_GetBackItemTableSize(void);


/*****************************************************************************/
//  Function name:	TF_GetPartTable
//  Description:This function is used to get part info table.
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TF_PARTITION_INFO *TF_GetPartTable(void);

#endif // _TF_CFG_H_


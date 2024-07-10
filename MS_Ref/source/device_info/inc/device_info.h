#ifndef _DEVICE_INFO_H
#define _DEVICE_INFO_H
/******************************************************************************
 ** File Name:      device_info.h                                               *
 ** Author:         gloria.he                                                 *
 ** DATE:           07/08/2015                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/08/2015     			Create.                                      *
 ******************************************************************************/
#ifdef __cplusplus
    extern   "C"
{
#endif
#include "sci_types.h"
#include "os_api.h"
#include "cmddef.h" 
#include "variant_config_nv.h"

/**---------------------------------------------------------------------------*
 **                        Mocor   Define                                *
 **---------------------------------------------------------------------------*/

#define _HWID_          "HWID"
#define _HWID_V1_       "HW1.0"
#define _HWID_V2_       "HW2.0"

#define _PRODUCT_VERSION_ 	"0.0000.0.00.00"  //temp
#define _PRODUCT_NAME_ 	"Unisoc"
#define _OS_NAME_	"ARM Cortex-A5"
#define _PROCESSOR_MODEL_  "Threadx4.0"

#ifdef MMI_MULTI_SIM_SYS_DUAL
#define TA_VER_SUK1 "TA-1155"
#define TA_VER_SUK2 "TA-1148"
#else
#define TA_VER_SUK1 "TA-1171"
#define TA_VER_SUK2 "TA-1169"
#endif


#define TA_TYPE_LEN 7
#define TA_CODE_LEN 12

#ifdef DEV_INFO_DEBUG
#define DEV_INFO_LOG   SCI_TraceLow
#else
#define DEV_INFO_LOG
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 typedef enum
{
	HWID_E =0,
	PRODUCT_NAME_E =1,
	VERSION_E  =2,
	OS_NAME_E =3,
	PROCESSOR_MODEL_E =4,
	 MAX_DEVICE_INFO_NUM
	
}DEVICE_INFO_E;

typedef struct _MSG_DEVINFO_READ_RES_T
{ 
    MSG_HEAD_T   msg_head;
    uint8   cmd; 
    uint8   type;
    uint8   status;
    uint8   reserved;
    union {
        uint8   hWid[4];
        uint8   product_type[16];
        uint8   product_code[16];//real size is 12 byte
        uint8   version[32];
        uint8   product_name[30];
    }dev_info;
}MSG_DEVINFO_READ_RES_T;

typedef struct _MSG_DEVINFO_WRITE_RES_T
{ 
	MSG_HEAD_T  msg_head;    
	uint16  status; 
	uint16	length;
	
}MSG_DEVINFO_WRITE_RES_T;

typedef struct _MSG_DEVINFO_T
{ 
	uint8   cmd; 
	uint8	type;
    uint16  reserved;
}MSG_DEVINFO_T;


typedef enum
{
	DEVICE_INFO_READ		=0,
	DEVICE_INFO_WRITE                   = 1

}DEVICE_INFO_CMD_E;

typedef enum
{
	HWID_T	=0,
	PRODUCT_TYPE_T		=1,
	PRODUCT_CODE_T                   = 2,
	VERISON_T				=3,
	PRODUCT_NAME_T		=4

}DEVICE_INFO_T;
typedef enum
{
	ERR_NONE=0,
	ERR_NOT_ENOUGH_RESOURCE,
	ERR_INVALID_PARAM,
	ERR_NV_ACCESS,
	ERR_NOT_ALLOWED
}ERR_DEVOPER_E;
	
/**---------------------------------------------------------------------------*
 **                         Function declaration                               *
 **---------------------------------------------------------------------------*/
PUBLIC	 uint32 HandleDeviceInfoReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len);

PUBLIC	uint8* HWID_Read(void);
PUBLIC  void HW_VER_Read(char * hw_ver);

PUBLIC uint8* PRODUCT_TYPE_Read(void);
PUBLIC	uint8* PRODUCT_NAME_Read(void);
PUBLIC	uint8* VERSION_Read(void);
PUBLIC 	uint8* OS_NAME_Read(void);
PUBLIC 	uint8* PROCESSOR_MODEL_Read(void);
	
PUBLIC	ERR_DEVOPER_E PRODUCT_CODE_Write(uint8 * data);
PUBLIC	ERR_DEVOPER_E PRODUCT_CODE_Read(uint8 * data);
PUBLIC	ERR_DEVOPER_E LP_CODE_Read(uint8 * data);
PUBLIC	ERR_DEVOPER_E Ndt_Version_Read(uint8 * data);


#ifdef __cplusplus
}
#endif

#endif  // End of _DEVICE_INFO_H



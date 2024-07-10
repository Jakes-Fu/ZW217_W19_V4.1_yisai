/******************************************************************************
 ** File Name:    common.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _UBOT_COMMON_H_
#define _UBOT_COMMON_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "sci_types.h"


#include "os_api.h"
#include "usb200.h"
#include "usb_drv.h"
#include "bsd.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
//"USB Mass Storage Class Bulk Only Transport 1.0"
#define UBOT_CLASS_RESET            0xFF       //Chapacter 3.1 P7
#define UBOT_CLASS_GET_MAX_LUN      0xFE       //Chapacter 3.2 P7

#define UBOT_CBW_LEN                31         //Chapacter 5.1 P13
#define UBOT_CSW_LEN                13         //Chapacter 5.2 P14

#define UBOT_CBW_SIGNATURE          0x43425355 //Chapacter 5.1 P13
#define UBOT_CSW_SIGNATURE          0x53425355 //Chapacter 5.2 P14

#define UBOT_CBD_LEN                16          //Chapacter 5.1 P13

#define UBOT_INSTRUCTION_NUM        32          //"MSC Compliance 0.9"

#define UBOT_MAX_ALLOC_LEN          256
#define UBOT_MAX_TRANSFER_LEN       (64*1024)   //"MSC Compliance 0.9"


#define  UBOT_EPx_BULK_IN            5
#define  UBOT_EPx_BULK_OUT           6



#define  UBOT_CONFIGRATION_NUM       1
#define  UBOT_CLASS_IN_INTERFACE     0
#define  UBOT_SUBCLASS_IN_INTERFACE  0
#define  UBOT_PROTOCOL_IN_INTERFACE  0

#define  UBOT_DEVICE_RELEASE_NUM     1

#define  UBOT_INTERFACE_NUM          1

#define  UBOT_POWER_VALUE            100
#define  UBOT_MAX_POWER              (UBOT_POWER_VALUE/2)

#define  UBOT_MAX_DISK_NUM           4

#define  USB_STR_MANUFACTURER_DESCRIPTOR_MIN 24
#define  USB_STR_SERIALNUMBER_DESCRIPTOR_MIN 24

#define  UBOT_CONFIG_DESCCIPTOR_LEN (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T) * 2)

#define UBOT_SCSI_VENDER_STR_LEN        8
#define UBOT_SCSI_PRODUCT_ID_STR_LEN    20

//Special define micro ;
#define UBOT_CACHE_PAGE_SIZE        USB_FIFO_MAX_BYTE
#define UBOT_CACHE_SIZE             (UBOT_MAX_TRANSFER_LEN+1024)
#define UBOT_CACHE_PAGE_NUM         (UBOT_CACHE_SIZE/UBOT_CACHE_PAGE_SIZE)


//#define UBOT_ASSERT(_EXP) SCI_ASSERT(_EXP)
//#define UBOT_ERR_TRACE          SCI_TRACE_LOW
//#define UBOT_INF_TRACE          SCI_TRACE_LOW
//#define UBOT_DEBUG_TRACE        SCI_TRACE_LOW
#define UBOT_ASSERT(_EXP)
#define UBOT_ERR_TRACE(x_format, ...)
#define UBOT_INF_TRACE(x_format, ...)
#define UBOT_DEBUG_TRACE(x_format, ...)

#define UBOT_BSD_OPEN_CHECK(status) {  \
                        if(BSD_SUCCESS != status)\
                                   USB_ErrorHandle(USB_ERR_BSD_OPENDEV);}

#define UBOT_BSD_CLOSE_CHECK(status) {  \
                        if(BSD_SUCCESS != status)\
                                   USB_ErrorHandle(USB_ERR_BSD_CLOSEDEV);}


// for match different HW_ID for MS
#define UBOT_DEV_STRING_INDEX	6
#define UBOT_DEV_STRING_DESC_LEN	0x80

#define NV_IMEI_ID			5
#define MAX_ID_LENGTH 		9
#define MAX_IMEI_LENGTH		(MAX_ID_LENGTH - 1)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum {
    UBOT_UNLOCK,
    UBOT_CBW_LOCK,
    UBOT_DATA_IN_LOCK,
    UBOT_DATA_OUT_LOCK,
    UBOT_CSW_LOCK,
    UBOT_READY_LOCK
}UBOT_LOCK_E;

typedef struct {
    volatile uint32 num ;
    UBOT_LOCK_E     lock ;
    uint32          reserved_mmu [8];
    volatile uint32 buf_size;
    volatile uint8  *malloc_buf;
    volatile uint8  *buf  ; //buf = ((malloc_buf+3)>>2)<<2
}UBOT_CACHE_T;

typedef enum {
    UBOT_STATE_READY,
    UBOT_STATE_CBW,
    UBOT_STATE_DATA_IN,
    UBOT_STATE_DATA_OUT,
    UBOT_STATE_CSW,
    UBOT_STATE_DISCONNECT,
    UBOT_STATE_COMMAND_ERR,
    UBOT_STATE_STALL,
    UBOT_STATE_CACHE_FULL,
    UBOT_STATE_MAX
}UBOT_STATES_E;

typedef union _bot_cbd_ {
    volatile uint8      cmd ;
    volatile uint8      bCBD[UBOT_CBD_LEN];
}UBOT_CBD_U;

typedef union {
    struct _mCBW{
        volatile uint32      dwCBWSignature;
        volatile uint32      dwCBWTag;
        volatile uint32      dwCBWDataTransferLength;

	union {
             struct {
#if defined CHIP_ENDIAN_LITTLE
                volatile uint8 bReserved     :6;
                volatile uint8 bObsolete     :1;
                volatile uint8 bDirection    :1;
#else
                volatile uint8 bDirection    :1;
                volatile uint8 bObsolete     :1;
                volatile uint8 bReserved     :6;
#endif
	      }mBits;
             volatile uint8 bFlag;
        }bmCBWFlags;

        volatile uint8       bCBWLUN;
        volatile uint8       bCBWCBLength;
        UBOT_CBD_U           uCBWCB;

    }mCBW;
    volatile uint8 bCBWBuf[UBOT_CBW_LEN];
}UBOT_CBW_U, * pUBOT_CBW_U;


typedef union _ubot_csw_u {
    struct {
        volatile uint32      dwCSWSignature;
        volatile uint32      dwCSWTag;
        volatile uint32      dwCSWDataResidue;
        volatile uint8       bwCSWStatus;
    }mCSW;
    volatile uint8 bCSWBuf[UBOT_CSW_LEN];
}UBOT_CSW_U, * pUBOT_CSW_U;

typedef enum {
    UBOT_DEV_NO_PRESENT,
    UBOT_DEV_PRESENT
}UBOT_DEVICE_STATE_E;

typedef enum {
    UBOT_CBD_SUCESS,
    UBOT_CBD_UNDEFINED_CMD,
    UBOT_CBD_NOREG_FUCTION,
    UBOT_CBD_PARAMER_ERR,
    UBOT_CBD_NO_EXECUTE,       //For debug ;
    UBOT_CBD_NO_DEVICE,
    UBOT_CBD_MAX_ERR
}UBOT_CBD_ERR_E;

typedef union _ubot_dev_state_u {
    struct {	
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN	

        volatile uint32 bState       :1; //TRUE : Exist  FALSE ://Exit
        volatile uint32 bHidden      :1; //TRUE : Hidden FALSE ://Eyeable
        volatile uint32 bReserved    :30;

#else	

        volatile uint32 bReserved    :30;
        volatile uint32 bHidden      :1; //TRUE : Hidden FALSE ://Eyeable
        volatile uint32 bState       :1; //TRUE : Exist  FALSE ://Exit

#endif	
    }mBits;
    volatile uint32 mValue;
}UBOT_DEV_STATE_U, *pUBOT_DEV_STATE_U;

typedef struct _ubot_device_inf{
    uint8               *dev_name;
    const uint8         *vendor;
    const uint8         *product;
    UBOT_DEV_STATE_U    dev_state;  
    //uint32    dev_state;  
    BSD_HANDLE          dev_handle;
    uint32              page_size;
    uint32              page_number;
    uint32              cache_number; /*cache number, 0: no cache*/

}UBOT_DEVICE_INF_T,*PUBOT_DEVICE_INF_T;

typedef struct _ubot_device{
    UBOT_DEVICE_INF_T   inf;
    UBOT_DEVICE_STATE_E state;
    uint32              page_size;
    uint32              page_number;
    uint32              cache_number; /*cache number, 0: no cache*/
}UBOT_DEVICE_T;

typedef struct _ubot_state_t{
    volatile uint32             data_out_len ;
    volatile uint32             *data_out_buf ;
    volatile UBOT_CACHE_T       cache;
             UBOT_DEVICE_T      dev[UBOT_MAX_DISK_NUM];
    volatile UBOT_CBD_ERR_E     cbd_err;
    volatile UBOT_CBW_U         cbw;
    volatile UBOT_STATES_E      state;
    volatile UBOT_CSW_U         csw;
}UBOT_MANAGER_T;

typedef void (* USB_FUNCTION)(uint16 req,uint16 wValue,\
                              uint16 wIndex ,uint16 wLen);

typedef UBOT_STATES_E (* UBOT_FUNCTION)(uint32 param,void * ptr);

typedef UBOT_CBD_ERR_E (* UBOT_CBD_FUNCTION) (UBOT_CBD_U *pCBD);

typedef struct _ubot_scsi_func {
    uint8              cmd;
    uint8              reserved_1;
    uint16             reserved_2;
    UBOT_CBD_FUNCTION  proc;
}UBOT_CBD_PROC_T;



typedef struct _USB_STRING_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    bString[1];
} USB_STRING_DESCRIPTOR_T, *PUSB_STRING_DESCRIPTOR_T;

typedef struct _USB_STRING_INTERFACE_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Interface[40];
} USB_STRING_INTERFACE_DESCRIPTOR_T,*PUSB_STRING_INTERFACE_DESCRIPTOR_T;

typedef struct _USB_STRING_CONFIGURATION_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Configuration[54];
} USB_STRING_CONFIGURATION_DESCRIPTOR_T,*PUSB_STRING_CONFIGURATION_DESCRIPTOR_T;


typedef struct _USB_STRING_SERIALNUMBER_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    SerialNum[USB_STR_SERIALNUMBER_DESCRIPTOR_MIN];
} USB_STRING_SERIALNUMBER_DESCRIPTOR_T,*PUSB_STRING_SERIALNUMBER_DESCRIPTOR_T;

typedef struct _USB_STRING_PRODUCT_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    Product[30];
} USB_STRING_PRODUCT_DESCRIPTOR_T,*PUSB_STRING_PRODUCT_DESCRIPTOR_T;

typedef struct _USB_STRING_MANUFACTURER_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    Manufacturer[USB_STR_MANUFACTURER_DESCRIPTOR_MIN];
} USB_STRING_MANUFACTURER_DESCRIPTOR_T,*PUSB_STRING_MANUFACTURER_DESCRIPTOR_T;


typedef struct _ubot_config_descriptor{
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface;
    USB_ENDPOINT_DESCRIPTOR_T      ep1;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
}UBOT_CONFIG_DESC_T,*PUBOT_CONFIG_DESC_T;

typedef struct 
{
	uint32	type;
	uint32	len;
	uint8	*pStr;
} UBOT_STR_DESC_T, *PUBOT_STR_DESC_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

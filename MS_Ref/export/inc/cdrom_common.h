/******************************************************************************
 ** File Name:    UCOM_common.h                                               *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-11-8                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2005-11-      weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _CDROM_COMMON_H_
#define _CDROM_COMMON_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "sci_types.h"
#include "sc_reg.h"
#include "os_api.h"
#include "usb200.h"
#include "usb_drv.h"
#include "bsd.h"
#include "gadget.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                             Debug Flag                                 *
 **---------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/


#define CDROM_BSD_OPEN_CHECK(status) {  \
                    if(BSD_SUCCESS != status)\
                        USB_ErrorHandle(USB_ERR_BSD_OPENDEV);}

#define CDROM_BSD_CLOSE_CHECK(status) {  \
                    if(BSD_SUCCESS != status)\
                        USB_ErrorHandle(USB_ERR_BSD_CLOSEDEV);}

//"USB Mass Storage Class Bulk Only Transport 1.0"
#define CDROM_CLASS_RESET            0xFF       //Chapacter 3.1 P7
#define CDROM_CLASS_GET_MAX_LUN      0xFE       //Chapacter 3.2 P7  

#define CDROM_CBW_LEN                31         //Chapacter 5.1 P13  
#define CDROM_CSW_LEN                13         //Chapacter 5.2 P14 

#define CDROM_CBW_SIGNATURE          0x43425355 //Chapacter 5.1 P13 
#define CDROM_CSW_SIGNATURE          0x53425355 //Chapacter 5.2 P14 

#define CDROM_CBD_LEN                16          //Chapacter 5.1 P13

#define CDROM_INSTRUCTION_NUM        32          //"MSC Compliance 0.9"

#define CDROM_MAX_ALLOC_LEN          256
#define CDROM_MAX_TRANSFER_LEN       (64*1024)   //"MSC Compliance 0.9"

#define  CDROM_CONFIGRATION_NUM       1
#define  CDROM_CLASS_IN_INTERFACE     0
#define  CDROM_SUBCLASS_IN_INTERFACE  0
#define  CDROM_PROTOCOL_IN_INTERFACE  0

#define  CDROM_DEVICE_RELEASE_NUM     1

#define  CDROM_INTERFACE_NUM          1

#define  CDROM_EPx_BULK_IN            1
#define  CDROM_EPx_BULK_OUT           2
#define  CDROM_EPx_NUM                2

#define  CDROM_POWER_VALUE			  100
#define  CDROM_MAX_POWER              (CDROM_POWER_VALUE/2) 

#define  CDROM_MAX_DISK_NUM           2
#define UCOM_VENDER_STR_LEN        8

#define CDROM_SCSI_VENDER_STR_LEN        8
#define CDROM_SCSI_PRODUCT_ID_STR_LEN    20

//Special define micro ;
#define CDROM_CACHE_PAGE_SIZE        USB_FIFO_MAX_BYTE
#define CDROM_CACHE_SIZE             (CDROM_MAX_TRANSFER_LEN+1024)
#define CDROM_CACHE_PAGE_NUM         (CDROM_CACHE_SIZE/CDROM_CACHE_PAGE_SIZE)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum {
	CDROM_UNLOCK,
	CDROM_CBW_LOCK,
	CDROM_DATA_IN_LOCK,
	CDROM_DATA_OUT_LOCK,
	CDROM_CSW_LOCK,
	CDROM_READY_LOCK,
}CDROM_LOCK_E;
typedef enum {
    CDROM_CMD_PLUG_IN, 
    CDROM_CMD_PLUG_OUT,
    CDROM_CMD_EXECUTE,
    CDROM_CMD_MAX
}CDROM_COMMAND_E;
typedef struct {
    volatile uint32 num ;
    CDROM_LOCK_E     lock ;
    uint32          reserved_mmu [8];
    volatile uint8  *malloc_buf ;
    volatile uint8  *buf ;
}CDROM_CACHE_T;

typedef enum {
    CDROM_STATE_READY,
    CDROM_STATE_CBW,
    CDROM_STATE_DATA_IN,
    CDROM_STATE_DATA_OUT,
    CDROM_STATE_CSW,
    CDROM_STATE_DISCONNECT,
    CDROM_STATE_COMMAND_ERR,
    CDROM_STATE_STALL,
    CDROM_STATE_CACHE_FULL,
    CDROM_STATE_MAX
}CDROM_STATES_E;

typedef union _cdrom_cbd_ {
    volatile uint8      cmd ;
    volatile uint8      bCBD[CDROM_CBD_LEN];
}CDROM_CBD_U;

typedef union {
    struct __mCBW{
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
        CDROM_CBD_U           uCBWCB;
    }mCBW;
    volatile uint8 bCBWBuf[CDROM_CBW_LEN];
}CDROM_CBW_U, * pCDROM_CBW_U;

typedef union _cdrom_csw_u {
    struct {
        volatile uint32      dwCSWSignature;
        volatile uint32      dwCSWTag;
        volatile uint32      dwCSWDataResidue;
        volatile uint8       bwCSWStatus;
    }mCSW;
    volatile uint8 bCSWBuf[CDROM_CSW_LEN];
}CDROM_CSW_U, * pCDROM_CSW_U;

typedef enum {
    CDROM_DEV_NO_PRESENT,
    CDROM_DEV_PRESENT
}CDROM_DEVICE_STATE_E;

typedef enum {
    CDROM_CBD_SUCESS,
    CDROM_CBD_UNDEFINED_CMD,
    CDROM_CBD_NOREG_FUCTION,
    CDROM_CBD_PARAMER_ERR,
    CDROM_CBD_NO_EXECUTE,       //For debug ;
    CDROM_CBD_NO_DEVICE,
    CDROM_CBD_MAX_ERR
}CDROM_CBD_ERR_E;

typedef union _cdrom_dev_state_u {
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
}CDROM_DEV_STATE_U, *pCDROM_DEV_STATE_U;

typedef struct _cdrom_device_inf{
    uint8               *dev_name;
    const uint8         *vendor;
    const uint8         *product;
    CDROM_DEV_STATE_U    dev_state;  
    //uint32    dev_state;  
    BSD_HANDLE          dev_handle;
    uint32              page_size;
    uint32              page_number;

}CDROM_DEVICE_INF_T;

typedef struct _cdrom_device{
    CDROM_DEVICE_INF_T   inf;
    CDROM_DEVICE_STATE_E state;
}CDROM_DEVICE_T;


typedef CDROM_STATES_E (* CDROM_FUNCTION)(uint32 param,void * ptr);

typedef CDROM_CBD_ERR_E (* CDROM_CBD_FUNCTION) (CDROM_CBD_U *pCBD);

typedef struct _cdrom_scsi_func {
    uint8              cmd;
    uint8              reserved_1;
    uint16             reserved_2;
    CDROM_CBD_FUNCTION  proc;
}CDROM_CBD_PROC_T;




typedef struct _CDROM_state_t{
    volatile uint32             data_out_len ;
    volatile uint32             *data_out_buf ;
    volatile CDROM_CACHE_T  cache;
    CDROM_DEVICE_T      ubot_dev[CDROM_MAX_DISK_NUM];
    volatile CDROM_CBD_ERR_E     cbd_err;
    volatile CDROM_CBW_U         cbw;
    volatile CDROM_STATES_E      ubot_state;
    volatile CDROM_CSW_U         csw;    

}CDROM_MANAGER_T;

typedef void (* USB_FUNCTION)(uint16 req,uint16 wValue,\
                              uint16 wIndex ,uint16 wLen);



/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
extern CDROM_MANAGER_T g_CDROM_manager;
extern const CDROM_FUNCTION g_CDROM_function[];
extern uint8 g_CDROM_MaxLUN [];
extern CDROM_DEVICE_INF_T CDROM_DiskInf[]; 
extern uint8 *g_CDROM_CBD_Response;
extern USB_EP_T       *ep0;
extern USB_EP_T       *ep1;
extern USB_EP_T       *ep2;

extern USB_REQUEST_T  *ep0req;   
extern USB_REQUEST_T  *ep1req;   
extern USB_REQUEST_T  *ep2req;   

extern uint32				g_ep1_transfer_end;

PUBLIC BOOLEAN UCOM_GetModemUbotEnable(void);

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

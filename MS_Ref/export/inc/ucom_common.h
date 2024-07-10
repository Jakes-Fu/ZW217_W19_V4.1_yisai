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
#ifndef _UCOM_COMMON_H_
#define _UCOM_COMMON_H_
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
#include "sio.h"
#include "osi_fifo.h"

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
#define UCOM_DEV_STRING_INDEX	5
#define UCOM_DEV_STRING_DESC_LEN	0x60

#define PRODUCT_TYPE_LEN	0x40
#define NV_IMEI_ID			5
#define MAX_ID_LENGTH 		9
#define MAX_IMEI_LENGTH		(MAX_ID_LENGTH - 1)
#define RM_TYPE_LENGTH		7



#define  UCOM_CONFIGRATION_NUM       1
#define  UCOM_CLASS_IN_INTERFACE     0
#define  UCOM_SUBCLASS_IN_INTERFACE  0
#define  UCOM_PROTOCOL_IN_INTERFACE  0

#define  UCOM_DEVICE_RELEASE_NUM     1

//here is the def of one interface
#define  UCOM_INTERFACE_NUM          1

#define  UCOM_EP1_BULK_IN            1
#define  UCOM_EP2_BULK_OUT           2
#define  UCOM_EPx_BULK_IN            3
#define  UCOM_EPx_BULK_OUT           4
#define  UCOM_EPx_NUM                2

//here is the def of two interface
#define  UCOM_INTERFACE_TWO          2
#define  UCOM_EPx_NUM_TWOIF          4
#define  UCOM_INTERFACE_EP_NUM       2
#define  UCOM_INTERFACE1_BULKIN      1
#define  UCOM_INTERFACE1_BULKOUT     2
#define  UCOM_INTERFACE2_BULKIN      3
#define  UCOM_INTERFACE2_BULKOUT     4

//here is the def of three interface
#define  UCOM_INTERFACE_THREE          3
#define  UCOM_EPx_NUM_THREEIF          6
#define  UCOM_INTERFACE3_BULKIN      5
#define  UCOM_INTERFACE3_BULKOUT     6


#define  UCOM_POWER_VALUE			 100
#define  UCOM_MAX_POWER              (UCOM_POWER_VALUE/2)


#define  USB_STR_MANUFACTURER_DESCRIPTOR_MIN 24
#define  USB_STR_SERIALNUMBER_DESCRIPTOR_MIN 24

#define  UCOM_CFG_DESCR_LEN (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T)*\
                                     UCOM_EPx_NUM)

#define  UCOM_CFG_DESCR_LEN_TWOINF (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)*UCOM_INTERFACE_TWO+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T)*\
                                     UCOM_EPx_NUM_TWOIF)

#define  UCOM_CFG_DESCR_LEN_THREEINF (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)*UCOM_INTERFACE_THREE+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T)*\
                                     UCOM_EPx_NUM_THREEIF)


#define UCOM_CFG_DESCR_LEN_FOURIF (sizeof(USB_CONFIGURATION_DESCRIPTOR_T) + \
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T) * 4 + \
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T) * 8)

#define UCOM_VENDER_STR_LEN        8


//ucom max virtual num
#define  UCOM_MAX_COM_NUM       4

//ucom open flag
#define  UCOM_ALREADY_OPEN      0x53494F49

/* +cr250844 */
#ifndef PRJ_TDPS_UEIT
#define UCOM_TRACE(...)
#define UCOM_ASSERT(...)
#endif


typedef enum {
    UCOM_DEV_NO_PRESENT = 0x00,
    UCOM_DEV_PRESENT,
    UCOM_DEV_READY  ,
    UCOM_DEV_BUSY   ,
    UCOM_DEV_MAX
}UCOM_DEVICE_STATE_E;

typedef enum
{
    UCOM_TX_STATE_IDLE,
    UCOM_TX_STATE_DONE,
    UCOM_TX_STATE_STARTED,
} UCOM_TX_STATE_E;

typedef struct UCOM_buf_tag
{
   uint32 buf_size;
   uint8 *usb_buf;
} UCOM_buf_T, *UCOM_buf_P;

typedef struct _ucom_device{
    UCOM_DEVICE_STATE_E  state;
    UCOM_TX_STATE_E tx_state;
    UCOM_buf_T  rx_buf;
    UCOM_buf_T  tx_buf;
    osiFifo_t rx_fifo;
    osiFifo_t tx_fifo;
    UartCallback  callback;
    uint32 open_flag;    //com already open or not
    uint32 data_statis;
    uint32 data_statis_send;
    uint32 data_statis_recv;
} UCOM_DEVICE_T;

typedef struct _UCOM_state_t{
    UCOM_DEVICE_T      dev[UCOM_MAX_COM_NUM];
} UCOM_MANAGER_T;

typedef void (* USB_FUNCTION)(uint16 req,uint16 wValue,\
                              uint16 wIndex ,uint16 wLen);

typedef  union _UCOM_INDEX_U{
    uint32 bValue;
    struct
    {
        uint32 reserved    :20;
    	uint32 type        :3;
    	uint32 recipient   :3;
    	uint32 request     :6;
    }mBits;
}UCOM_INDEX_U;


typedef struct _UCOM_function_t{
    UCOM_INDEX_U  index;
    USB_FUNCTION  func_ptr;
}UCOM_FUNCTION_T;

#define UCOM_MAX_FUNC_NUM 12

//usb string descriptor struct
typedef struct _UCOM_STRING_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    bString[1];
} UCOM_STRING_DESCRIPTOR_T, *PUCOM_STRING_DESCRIPTOR_T;

typedef struct _UCOM_STRING_INTERFACE_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Interface[22];
} UCOM_STRING_INTERFACE_DESCRIPTOR_T,*PUCOM_STRING_INTERFACE_DESCRIPTOR_T;

typedef struct _UCOM_STRING_CONFIGURATION_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Configuration[16];
} UCOM_STRING_CONFIGURATION_DESCRIPTOR_T,*PUCOM_STRING_CONFIGURATION_DESCRIPTOR_T;


typedef struct _UCOM_STRING_SERIALNUMBER_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    SerialNum[USB_STR_SERIALNUMBER_DESCRIPTOR_MIN];
} UCOM_STRING_SERIALNUMBER_DESCRIPTOR_T,*PUCOM_STRING_SERIALNUMBER_DESCRIPTOR_T;

typedef struct _UCOM_STRING_PRODUCT_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    Product[30];
} UCOM_STRING_PRODUCT_DESCRIPTOR_T,*PUCOM_STRING_PRODUCT_DESCRIPTOR_T;

typedef struct _UCOM_STRING_MANUFACTURER_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    Manufacturer[USB_STR_MANUFACTURER_DESCRIPTOR_MIN];
} UCOM_STRING_MANUFACTURER_DESCRIPTOR_T,*PUCOM_STRING_MANUFACTURER_DESCRIPTOR_T;

typedef struct _USB_STRING_CONFIGURATION_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Configuration[54];

} USB_STRING_CONFIGURATION_DESCRIPTOR_T,*PUSB_STRING_CONFIGURATION_DESCRIPTOR_T;

typedef struct _USB_STRING_INTERFACE_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Interface[40];
} USB_STRING_INTERFACE_DESCRIPTOR_T,*PUSB_STRING_INTERFACE_DESCRIPTOR_T;


/* +cr250844 */
#ifndef PRJ_TDPS_UEIT
typedef struct _ucom_config_descriptor{
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
    USB_ENDPOINT_DESCRIPTOR_T      ep3;
}UCOM_CONFIG_DESC_T;
#endif
/* -cr250844 */

typedef struct _ucom_config_descriptor_twointerface{
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface1;
    USB_ENDPOINT_DESCRIPTOR_T      ep1;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
    USB_INTERFACE_DESCRIPTOR_T     interface2;
    USB_ENDPOINT_DESCRIPTOR_T      ep3;
    USB_ENDPOINT_DESCRIPTOR_T      ep4;
}UCOM_CONFIG_DESC_TOWIF_T,*PUCOM_CONFIG_DESC_TOWIF_T;

typedef struct _ucom_config_descriptor_threeinterface {
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface1;
    USB_ENDPOINT_DESCRIPTOR_T      ep1;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
    USB_INTERFACE_DESCRIPTOR_T     interface2;
    USB_ENDPOINT_DESCRIPTOR_T      ep3;
    USB_ENDPOINT_DESCRIPTOR_T      ep4;
    USB_INTERFACE_DESCRIPTOR_T     interface3;
    USB_ENDPOINT_DESCRIPTOR_T      ep5;
    USB_ENDPOINT_DESCRIPTOR_T      ep6;
} UCOM_CONFIG_DESC_THREEIF_T;

typedef struct _ucom_config_descriptor_fourinterface {
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface1;
    USB_ENDPOINT_DESCRIPTOR_T      ep1;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
    USB_INTERFACE_DESCRIPTOR_T     interface2;
    USB_ENDPOINT_DESCRIPTOR_T      ep3;
    USB_ENDPOINT_DESCRIPTOR_T      ep4;
    USB_INTERFACE_DESCRIPTOR_T     interface3;
    USB_ENDPOINT_DESCRIPTOR_T      ep5;
    USB_ENDPOINT_DESCRIPTOR_T      ep6;
    USB_INTERFACE_DESCRIPTOR_T     interface4;
    USB_ENDPOINT_DESCRIPTOR_T      ep7;
    USB_ENDPOINT_DESCRIPTOR_T      ep8;
} UCOM_CONFIG_DESC_FOUR_IF_T;

typedef struct 
{
	uint32	type;
	uint32	len;
	uint8	*pStr;
} UCOM_STR_DESC_T, *PUCOM_STR_DESC_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
extern UCOM_MANAGER_T g_UCOM_manager;
extern  uint8* s_ucom_temp_buf;

PUBLIC uint32 UCOM_GetCOnfigDescriptor(uint8 **cfg_ptr);
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

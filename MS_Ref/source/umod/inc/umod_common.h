/******************************************************************************
 ** File Name:    UMOD_common.h                                                 *
 ** Author:       Jim.Lin                                                 *
 ** DATE:         2007-1-30                                                   *
 ** Copyright:    2007 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2007-01-30     Jim.Lin     Create.                                     *
 ** 2007-02-06    Jim.Lin      modify the comment
 ** 2007-6-21     Elly.Zhou    delete no-use macro
 *****************************************************************************/

#ifndef _UMOD_COMMON_H_
    #define _UMOD_COMMON_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "sci_types.h"
#ifdef _UMOD_SC8800H_
#include "sc8800h_reg.h"
#else
#include "sc6600m_reg.h"
#endif
#include "os_api.h"
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

#define  USB_CDC_INTERFACE_NUM  2
#define  USB_COMMUNICATION_INTERFACE_EPx_NUM    1
#define  UMOD_EPx_NUM                2
#define UMOD_SCSI_VENDER_STR_LEN        8



#define  USB_STR_MANUFACTURER_DESCRIPTOR_MIN 24
#define  USB_STR_SERIALNUMBER_DESCRIPTOR_MIN 24


//umod max virtual num
#define  UMOD_MAX_COM_NUM       2

//umod open flag
#define  UMOD_ALREADY_OPEN      0x53494F49

//Special define micro ;
#define USB_RECV_LIMIT  (USB_FIFO_MAX_BYTE*2) 

#ifdef _DEBUG
    #define UMOD_TRACE()  SCI_TRACE_LOW
    #define UMOD_ASSERT(_EXP) SCI_ASSERT(_EXP)
#else
    #define UMOD_TRACE
#endif



/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum {
    UMOD_DEV_NO_PRESENT = 0x00,
    UMOD_DEV_PRESENT,
    UMOD_DEV_READY  ,
    UMOD_DEV_BUSY   , 
    UMOD_DEV_MAX
}UMOD_DEVICE_STATE_E;

typedef struct UMOD_buf_tag
{  
   uint32  read;
   uint32  write;  
   uint8   usb_buf[USB_RECV_LIMIT];
}UMOD_buf_T, *UMOD_buf_P;

typedef struct _umod_device{
    UMOD_DEVICE_STATE_E  state;
    UMOD_buf_T  rx_buf;
    UMOD_buf_T  tx_buf;
    void   (*callback)(uint32 event);
    uint32 open_flag;    //com already open or not
}UMOD_DEVICE_T;

typedef struct _UMOD_state_t{
    UMOD_DEVICE_T      dev[UMOD_MAX_COM_NUM];
}UMOD_MANAGER_T;

typedef void (* USB_FUNCTION)(uint8 bDirection,uint16 wValue,\
                              uint16 wIndex ,uint16 wLen);



/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
extern UMOD_MANAGER_T g_UMOD_manager;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

//when using the pc drv of profilic, the vendor command is sent many times
//so we have to use a timer to judge the last state of usb endp
//#define OLD_USB_DRV 
#ifdef OLD_USB_DRV
/*****************************************************************************/
//  Description:    for timer handler, to judge whether is connect on or off
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           called by timer handler
/*****************************************************************************/
PUBLIC void UMOD_DetectTimerHdl(uint32 param);
#endif


#define UMOD_CLASS_RESET            0xFF       //Chapacter 3.1 P7
#define UMOD_CLASS_GET_MAX_LUN      0xFE       //Chapacter 3.2 P7  
#define UMOD_CLASS_GET_LINE_CODING 0x21
#define UMOD_CLASS_SET_CTRL_LINE_ST  0x22
#define UMOD_CLASS_SET_LINE_CODING  0X20



#define  UMOD_CONFIGRATION_NUM       1
#define  UMOD_CLASS_CDC_DEVICE       2
#define  UMOD_SUBCLASS_IN_INTERFACE  0
#define  UMOD_PROTOCOL_IN_INTERFACE  0

#define  UMOD_DEVICE_RELEASE_NUM     1


#define  UMOD_EPx_BULK_IN            1
#define  UMOD_EPx_BULK_OUT           2
#define  UMOD_EPx_INTERRUPT_IN   3


#define  UMOD_POWER_VALUE			 100
#define  UMOD_MAX_POWER              (UMOD_POWER_VALUE/2) 

#define  UMOD_CONFIG_DESCCIPTOR_LEN (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T)*\
                                     UMOD_EPx_NUM)

 #define UMOD_CDC_CONFIG_DESCCIPTOR_LEN  0x3e


typedef struct _USB_STRING_DESCRIPTOR {
    char    bLength;
    char    bDescriptorType;
    char    bString[1];
} USB_STRING_DESCRIPTOR_T, *PUSB_STRING_DESCRIPTOR_T;

typedef struct _USB_STRING_INTERFACE_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Interface[22];
} USB_STRING_INTERFACE_DESCRIPTOR_T,*PUSB_STRING_INTERFACE_DESCRIPTOR_T;

typedef struct _USB_STRING_CONFIGURATION_DESCRIPTOR {
    char  bLength;
    char  bDescriptorType;
    char  Configuration[16];
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


typedef struct _umod_config_descriptor{
    USB_CONFIGURATION_DESCRIPTOR_T config;
    USB_INTERFACE_DESCRIPTOR_T     interface0;
    USB_CALL_MANAGETMENT_FUNCTIONAL_DESCRIPTOR_T call_func;
    USB_UNION_FUNCTIONAL_DESCRIPTOR_T   union_func;
    USB_ABSTRACT_CONTROL_MANAGETMENT_FUNCTIONAL_DESCRIPTOR_T ACM_func;
   USB_ENDPOINT_DESCRIPTOR_T      ep3;
    USB_INTERFACE_DESCRIPTOR_T     interface1;
   
    USB_ENDPOINT_DESCRIPTOR_T      ep1;
    USB_ENDPOINT_DESCRIPTOR_T      ep2;
}UMOD_CONFIG_DESC_T;


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

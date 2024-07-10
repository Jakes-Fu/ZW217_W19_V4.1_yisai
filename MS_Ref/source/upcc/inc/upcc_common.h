/******************************************************************************
 ** File Name:    UPCC_common.h                                               *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/13/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/13/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_COMMON_H_
    #define _UPCC_COMMON_H_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "sci_types.h"
#include "os_api.h"
#include "usb200.h" 
#include "usb_drv.h"

/**---------------------------------------------------------------------------*
 **                         COMPILER FLAG                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {    
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/ 
#ifdef _DEBUG
    #define UPCC_ASSERT(_EXP) SCI_ASSERT(_EXP)
    #define UPCC_TRACE_LOW(_EXP) SCI_TRACE_LOW _EXP
#else
    #define UPCC_ASSERT(_EXP)
    #define UPCC_TRACE_LOW(_EXP)
#endif

//Appendix A. Video Device Class Codes(USB Device Class Definition for Video Devices)
//Video Interface Class Code
#define USB_CC_VIDEO    0x0e

//Video Interface Subclass Codes
#define USB_VIDEO_SC_UNDEFINED                     0x00
#define USB_VIDEO_SC_VIDEOCONTROL                  0x01
#define USB_VIDEO_SC_VIDEOSTREAMING                0x02
#define USB_VIDEO_SC_VIDEO_INTERFACE_COLLECTION    0x03

//Video Interface Protocol Codes
#define USB_VIDEO_PC_PROTOCOL_UNDEFINED    0x00

//control request type
#define USB_VIDEO_RC_UNDEFINED    0x00
#define USB_VIDEO_RC_SET_CUR      0x01
#define USB_VIDEO_RC_GET_CUR      0x02
#define USB_VIDEO_RC_GET_MIN      0x03
#define USB_VIDEO_RC_GET_MAX      0x04
#define USB_VIDEO_RC_GET_DEF      0x05


//definition for descriptors
//device definition
#define UPCC_CLASS_IN_INTERFACE          0
#define UPCC_SUBCLASS_IN_INTERFACE       0
#define UPCC_PROTOCOL_IN_INTERFACE       0   
#define UPCC_DEVICE_RELEASE_NUM          1

//config definition
#define UPCC_CONFIGRATION_NUM            1//support only one configuration 
#define UPCC_CONFIG_VALUE                1
#define UPCC_POWER_VALUE                 100
#define UPCC_CONFIG_MAX_POWER            (UPCC_POWER_VALUE/2)
#define UPCC_CONFIG_SELF_POWER           0xc0//Self-powered

//interface definition
#define UPCC_INTERFACE_NUM               1//support only one interface
#define UPCC_INTERFACE_VC_INDEX          0//video control interface

//endpoint definition
#define UPCC_EPx_NUM                     2//support two bulk-in endpoint
#define UPCC_EPx_BULK_IN_VIDEO           1//endpoint for video streaming
#define UPCC_EPx_BULK_IN_IMAGE           2//endpoint for still image capture

//string definition
#define UPCC_STR_SERIALNUMBER_DESCRIPTOR_LEN    24
#define UPCC_STR_PRODUCT_DESCRIPTOR_LEN         24
#define UPCC_STR_MANUFACTURER_DESCRIPTOR_LEN    24


#define UPCC_DEVICE_DESC_MAX_LEN        64
#define UPCC_DEVICE_DESC_MIN_LEN        18
#define UPCC_CONFIG_DESC_MAX_LEN        0xff
                                   
#define  UPCC_CONFIG_DESCCIPTOR_LEN (sizeof(USB_CONFIGURATION_DESCRIPTOR_T)+\
                                     sizeof(USB_INTERFACE_DESCRIPTOR_T)*UPCC_INTERFACE_NUM+\
                                     sizeof(USB_ENDPOINT_DESCRIPTOR_T)*UPCC_EPx_NUM)
 

/* Maybe we just need one cache buffer */
#define UPCC_MAX_TRANSFER_LEN       (32*1024)
#define JPEG_FRAME_STEP   			32
#define UPCC_CACHE_SIZE             (UPCC_MAX_TRANSFER_LEN+1024)
#define UPCC_DATA_ALIGN             1024
#define UPCC_PC_RECEIVE_BLOCK_SIZE  1024//PC端驱动每次读取的数据块大小
#define UPCC_FRAME_LEN              12

//define the lenght of the control parameter block
#define UPCC_VC_REQUEST_PARAM_LEN           2

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//configuration descriptor
typedef struct upcc_config_descriptor_tag
{
    USB_CONFIGURATION_DESCRIPTOR_T        config;
    USB_INTERFACE_DESCRIPTOR_T            interface;
    USB_ENDPOINT_DESCRIPTOR_T             ep_video;
    USB_ENDPOINT_DESCRIPTOR_T             ep_image;
}UPCC_CONFIG_DESC_T;

//serial number string descriptor
typedef struct upcc_string_serialnumber_descriptor_tag
{
    char    bLength;
    char    bDescriptorType;
    char    SerialNum[UPCC_STR_SERIALNUMBER_DESCRIPTOR_LEN];
}UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T;

//product string descriptor
typedef struct upcc_string_product_descriptor_tag
{
    char    bLength;
    char    bDescriptorType;
    char    Product[UPCC_STR_PRODUCT_DESCRIPTOR_LEN];
}UPCC_STRING_PRODUCT_DESCRIPTOR_T;

//manufacture string descriptor
typedef struct upcc_string_manufacturer_descriptor_tag
{
    char    bLength;
    char    bDescriptorType;
    char    Manufacturer[UPCC_STR_MANUFACTURER_DESCRIPTOR_LEN];
}UPCC_STRING_MANUFACTURER_DESCRIPTOR_T;


//video control enum
typedef enum 
{
    VC_CONTROL_INIT = 0,
    VC_BRIGHTNESS_CONTROL,
    VC_CONTRAST_CONTROL,
    VC_HUE_CONTROL,
    VC_FRAME_RATE_CONTROL,
    VC_FRAME_SIZE_PATTERN_CONTROL,
    VC_VIDEO_STREAMING_CONTROL,
    VC_CONTROL_MAX
}UPCC_VC_CONTROL_E;

//#define UPCC_CMD_START  ((USB_SERVICE_UPCC << 16) & 0xFFFFFFFFUL)
#define UPCC_CMD_START  (0)

//message define
typedef enum {
    UPCC_CMD_EXECUTE = UPCC_CMD_START,
	UPCC_CMD_PLUG_IN,
	UPCC_CMD_PLUG_OUT,
    UPCC_CMD_EXCEPTION,   
	UPCC_CMD_SETBACKLIGHT  = 0x0100,
	UPCC_CMD_SETBRIGHT     = 0x0200,
	UPCC_CMD_SETCONTRAST   = 0x0300,
	UPCC_CMD_SETGAIN       = 0x0400,
	UPCC_CMD_SETPWRLINE    = 0x0500,
	UPCC_CMD_SETHUE        = 0x0600,
	UPCC_CMD_SETSATURA     = 0x0700,
	UPCC_CMD_SETSHARP      = 0x0800,
	UPCC_CMD_SETGAMMA      = 0x0900,
	UPCC_CMD_SETWBTEMP     = 0x0A00,
	UPCC_CMD_SETWBCOM      = 0x0B00,
	UPCC_CMD_SETWBTEMPAUTO = 0x0C00,
	UPCC_CMD_SETWBCOMAUTO  = 0x0D00,
	UPCC_CMD_SETDIGITAL    = 0x0E00,
	UPCC_CMD_SETDIGILIM    = 0x0F00,
	UPCC_CMD_SETHUEAUTO    = 0x1000,
	UPCC_CMD_SETANALOG     = 0x1100,
	UPCC_CMD_SETANALOCK    = 0x1200,
    UPCC_CMD_MAX
}UPCC_COMMAND_E;

//video streaming control
typedef enum
{
    UPCC_VIDEO_STREAMING_STOP = 0,
    UPCC_VIDEO_STREAMING_START
}UPCC_VIDEO_STREAMING_CONT_E;

typedef struct upcc_cache_tag
{
    volatile uint32  num ;
    volatile uint8   *malloc_buf;
	volatile uint8   *align_buf;
	volatile int32    cur_index;
    volatile uint8   *buf;//[UPCC_CACHE_SIZE];
    volatile BOOLEAN lock;
}UPCC_CACHE_T;

typedef struct upcc_manager_tag
{
    volatile uint32             data_out_len ;
    volatile UPCC_CACHE_T       cache;
}UPCC_MANAGER_T;

typedef struct {
	xSignalHeaderRec	sig;
	uint8		  request;
	uint8        command;
	uint8        param;
}UPCC_SIGNAL_T;

//define the handle function for request
typedef void (*UPCC_REQUEST_HANDLE_FUNC)(uint8 bRequest, uint16 wParam, uint16 wLen);

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                          COMPILER FLAG                                     *
**----------------------------------------------------------------------------*/    
#ifdef __cplusplus
    }
#endif    


/**---------------------------------------------------------------------------*/   
#endif //_UPCC_COMMON_H_
//end UPCC_common.h

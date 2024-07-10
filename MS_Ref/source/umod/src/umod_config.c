/******************************************************************************
 ** File Name:    model.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "usb_drv.h"
#include "umod_common.h"

#define USB_FIFO_INTERRUPT_MAX_BYTE     16

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
__align(4) const USB_DEVICE_DESCRIPTOR_T g_UMOD_DeviceDescr =
{
    sizeof(USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(USB_SPECIFICATION_VERSION),  // USB 1.1
    UMOD_CLASS_CDC_DEVICE,    //jim 1-11
    UMOD_SUBCLASS_IN_INTERFACE,
    UMOD_PROTOCOL_IN_INTERFACE,
    USB_FIFO_MAX_BYTE,
 USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_VENDER_ID),   // SPREADTRUM VID
 USB_WORD_SWAP_ENDIAN(USB_SPREADTRUM_USB_MODEM_ID),
 USB_WORD_SWAP_ENDIAN(UMOD_DEVICE_RELEASE_NUM),
  USB_STR_INDEX_MANUFACTURER,
   USB_STR_INDEX_PRODUCT,
  USB_STR_INDEX_SERIALNUMBER2,
   UMOD_CONFIGRATION_NUM   //UBOT_CONFIGRATION_NUM jim1-11
   
};

__align(4) const UMOD_CONFIG_DESC_T g_UMOD_ConfigDescr = 
{
    // Following is InterfaceDescr
    sizeof(USB_CONFIGURATION_DESCRIPTOR_T),
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN(UMOD_CDC_CONFIG_DESCCIPTOR_LEN),  //USB_WORD_SWAP_ENDIAN(UBOT_CONFIG_DESCCIPTOR_LEN), Jim 1-12
    USB_CDC_INTERFACE_NUM,		        // only one interface
     1,		                        // argument
    USB_STR_INDEX_LANGUAGE,//USB_STR_INDEX_LANGUAGE, JIM 1-12
    0x80,                           // Self-powered ;
    
    UMOD_MAX_POWER  ,
    // Following is InterfaceDescr
   //add by jim 1-12  for cdc communication interface
   sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    NULL,
    NULL,
    USB_COMMUNICATION_INTERFACE_EPx_NUM,
    USB_CLASS_COMMUNICATION_CLASS_DEVICE,
    USB_SUBCLASS_CDC_ACM,
    USB_PROTOCOL_CODE_CDC_AT_COMMANDS,
    USB_STR_INDEX_LANGUAGE,//USB_STR_INDEX_LANGUAGE, JIM1-12


    sizeof(USB_CALL_MANAGETMENT_FUNCTIONAL_DESCRIPTOR_T),   //Call Management Functional Descriptor, jim 1-12
    USB_FUNCTION_DESCRIPTOR_TYPE,
    USB_CALL_FUNCTION_SUBDESCRIPTOR_TYPE,
    0x03,
    0x01,

   sizeof(USB_UNION_FUNCTIONAL_DESCRIPTOR_T ),   //Union Funtional descriptor, jim 1-12
   USB_FUNCTION_DESCRIPTOR_TYPE,
   USB_UNION_FUNCTION_SUBDESCRIPTOR_TYPE,
   0x00,
   0x01,

  sizeof(USB_ABSTRACT_CONTROL_MANAGETMENT_FUNCTIONAL_DESCRIPTOR_T),  //Abstract Control Management,jim 1-12
   USB_FUNCTION_DESCRIPTOR_TYPE,
   USB_ACM_FUNCTION_SUBDESCRIPTOR_TYPE,
   0x02,

  //following is EP3 descriptor
   sizeof(USB_ENDPOINT_DESCRIPTOR_T),    //EP3 descriptor  for interrupt IN, jim 1-12 
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK | UMOD_EPx_INTERRUPT_IN,
    USB_ENDPOINT_TYPE_INTERRUPT,
    USB_WORD_SWAP_ENDIAN(USB_FIFO_INTERRUPT_MAX_BYTE),
    0X10,      //interrupt inquiary time
   

	 
    sizeof(USB_INTERFACE_DESCRIPTOR_T),
    USB_INTERFACE_DESCRIPTOR_TYPE,
    1,//the NO.1 interface,  Jim  1-12
    NULL,
    UMOD_EPx_NUM,
    USB_CLASS_DATA_CLASS_DEVICE, //  jim 1-12
    NULL,
    NULL,
   USB_STR_INDEX_LANGUAGE,//USB_STR_INDEX_LANGUAGE, JIM1-12 
    
    // Following is EP1 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    USB_ENDPOINT_DIRECTION_MASK | UMOD_EPx_BULK_IN,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(USB_FIFO_MAX_BYTE),
    NULL,
    
    // Following is EP2 Descriptor
    sizeof(USB_ENDPOINT_DESCRIPTOR_T),
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    UMOD_EPx_BULK_OUT,
    USB_ENDPOINT_TYPE_BULK,
    USB_WORD_SWAP_ENDIAN(USB_FIFO_MAX_BYTE),
    NULL
};

__align(4) const USB_STRING_LANGUAGE_DESCRIPTOR_T g_UMOD_LanguageStr []={
    sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (USB_LANGUAGE_ENGLISH)
};

__align(4) const USB_STRING_MANUFACTURER_DESCRIPTOR_T  g_UMOD_StrManufacturer[] =
{
    sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {
    'S', 0,'p', 0,'r', 0,'e', 0,'a', 0,'d', 0,'t', 0,
    'r', 0,'u', 0,'m', 0,' ', 0,' ', 0}
};

__align(4) const USB_STRING_PRODUCT_DESCRIPTOR_T  g_UMOD_StrProduct[] =
{
    sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,    
	{'U', 0,'S', 0,'B', 0,' ', 0,'M', 0,'o', 0,'d', 0,'e', 0,'m', 0,' ',0,'1',0,'.',0,'0',0}
};

//LOCAL const unsigned char s_UMOD_hex_string[]="0123456789ABCDEF";

__align(4) USB_STRING_SERIALNUMBER_DESCRIPTOR_T g_UMOD_SerialNumStr []={
    sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    //UNICODE character string; 
    { 'S',0,'p',0,'r',0,'e',0,'a',0,'d',0,'t',0,'r',0,
      'u',0,'m',0,'_',0,'M',0
    }
    //It must be modified in initialization.
};

#if 0
__align(4) LOCAL const uint8 g_UMOD_VenderStr  [UBOT_SCSI_VENDER_STR_LEN]={
    "Generic"
};


__align(4) LOCAL const uint8 g_UMOD_ProductIDStrNand\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "NAND DISK         "
};
__align(4) LOCAL const uint8 g_UMOD_ProductIDStrSD\
                                         [UBOT_SCSI_PRODUCT_ID_STR_LEN]={
    "SDCard DISK       "
};

#endif


//extern SD_CARD_T* 	g_mySDCard;
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/






/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

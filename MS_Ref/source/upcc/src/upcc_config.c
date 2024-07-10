/******************************************************************************
 ** File Name:    UPCC_config.c                                               *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/13/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file contain the descriptor of PC Camera.              *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/13/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_CONFIG_C_
    #define _UPCC_CONFIG_C_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "upcc_config.h"
#include "sc_reg.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
 
 #define UPCC_UVC_SUPPORT
 
#define CC_VIDEO							0x0E

#define SC_UNDEFINED						0x00
#define SC_VIDEOCONTROL						0X01
#define SC_VIDEOSTREAMING					0x02
#define SC_VIDEO_INTERFACE_COLLECTION		0x03

#define PC_PROTOCOL_UNDEFINED				0x00

#define CS_UNDEFINED						0x20
#define CS_DEVICE							0x21
#define CS_CONFIGURATION					0x22
#define CS_STRING							0x23
#define CS_INTERFACE						0x24
#define CS_ENDPOINT							0x25

#define VC_DESCRIPTOR_UNDEFINED				0x00
#define VC_HEADER							0x01
#define VC_INPUT_TERMINAL					0x02
#define VC_OUTPUT_TERMINAL					0x03
#define VC_SELECTOR_UNIT					0x04
#define VC_PROCESSING_UNIT					0x05
#define VC_EXTENSION_UNIT					0x06

#define EP_UNDEFINED						0x00
#define EP_GENERAL							0x01
#define EP_ENDPOINT							0x02
#define EP_INTERRUPT						0x03

#define VS_UNDEFINED						0x00
#define VS_INPUT_HEADER						0x01
#define VS_OUTPUT_HEADER					0x02
#define VS_STILL_IMAGE_FRAME				0x03
#define VS_FORMAT_UNCOMPRESSED				0x04
#define VS_FRAME_UNCOMPRESSED				0x05
#define VS_FORMAT_MJPEG						0x06
#define VS_FRAME_MJPEG						0x07
#define VS_FORMAT_MJPEG1					0x08
#define VS_FORMAT_MJPEG2PS					0x09
#define VS_FORMAT_MJPEG2TS					0x0A
#define VS_FORMAT_MJPEG4SL					0x0B
#define VS_FORMAT_DV						0x0C
#define VS_COLORFORMAT						0x0D
#define VS_FORMAT_VENDOR					0x0E
#define VS_FRAME_VENDOR						0x0F


__align(4) __packed const uint8 g_UPCC_DeviceDescr[]=   /*lint !e140 */
{
//	0x12,0x01,0x10,0x01,0xEF,0x02,0x01,0x40,0x8D,0x0E,0x04,0x00,0x00,0x01,0x01,0x02,0x03,0x01
	0x12,                               //bLength	
	0x01,                               //bDescriptor Type
	0x10,0x01,                          //bcdUSB
	0xEF,                               //bDeviceClass
	0x02,                               //bDeviceSubClass
	0x01,                               //bDeviceProtocol
	0x40,                               //bMaxPacketSize0
	0x82,0x17,                          //idVendor
	0x04,0x00,                          //idproduct
	0x00,0x01,                          //bcdDevice
	0x00,                               //iManufacture
	0x00,                               //iProduct
	0x00,                               //iSerialNumber
	0x01                                //bNumConfigurations  
};

__align(4) __packed /*const*/ uint8 g_UPCC_ConfigDescr[] =   /*lint !e140 */
{
/********************** Configuration Desc **************************************************************/
 0x09,									//bLength															0x00
 0x02,									//bDescriptor Type													0x01
 0xE7,0x00,								//Total Length														0x02
 0x02,									//number of interfaces												0x04
 0x01,									//configuration value												0x05
 0x00,									//index of string descriptor										0x06
 0x80,									//configuration characterics										0x07
 0xFA,									//max power consumption												0x08
/********************** Interface Association Desc ******************************************************/
 0x08,									//size of descriptor												0x09
 0x0B,									//bDescriptor type													0x0A
 0x00,									//first video control interface number								0x0B
 0x02,									//number of video stream interfaces include first video control		0x0C
 CC_VIDEO,								//video interface class code										0x0D
 SC_VIDEO_INTERFACE_COLLECTION,			//video interface sub-class code									0x0E
 PC_PROTOCOL_UNDEFINED,					//protocol.is set as spec mentioned									0x0F
 0x01,									//index of string descriptor.iFunction								0x10
/********************** Video Control Interface Desc **************************************************/
 0x09,									//size of descriptor												0x11
 0x04,									//bDescriptor type													0x12
 0x00,									//Interface number													0x13
 0x00,									//Alternate setting													0x14
 0x01,									//number of endpoints used in this interface						0x15
 CC_VIDEO,								//video interface class code										0x16
 SC_VIDEOCONTROL,						//video interface sub-class code									0x17
 PC_PROTOCOL_UNDEFINED,					//protocol is set as spec mentioned									0x18
 0x01,									//index of string descriptor.iFunction								0x19
/********************** Class-Specific VC Interface header Desc **************************************/
 0x0D,									//size of descriptor												0x1A
 CS_INTERFACE,							//descriptor type													0x1B
 VC_HEADER,								//descriptor sub-type												0x1C
 0x00,0x01,								//video device class release number									0x1D
 0x32,0x00,								//total return number for class-specific interface descriptor		0x1F
 0xE8,0x03,0x00,0x00,					//device clock frequency in Hz										0x21
 0x01,									//the number of videostreaming interfaces in the video interface collection 0x25
 0x01,									//first videostreaming interface number								0x26
/********************** Input Terminal Descriptor ******************************************************/
 0x11,									//size of descriptor												0x27
 CS_INTERFACE,							//descriptor type													0x28
 VC_INPUT_TERMINAL,						//descriptor sub-type												0x29
 0x01,									//terminal id														0x2A
 0x01,0x02,								//terminal type ITT_CAMERA											0x2B
 0x00,									//associated output terminal id										0x2D
 0x02,									//index of string descriptor. iTerminal								0x2E
 0x00,0x00,								//no optical zoom supported											0x2F
 0x00,0x00,								//no optical zoom supported											0x31
 0x00,0x00,								//no optical zoom supported											0x33
 0x02,									//size of bmcontrol													0x35
 0x00,0x00,								//camera control supported											0x36
 										// D0 : Scanning mode
										// D1 : Auto-Exposure Mode
										// D2 : Auto-Exposure priority
										// D3 : exposure time(absolute)
										// D4 : exposure time(relative)
										// D5 : focus (absolute)
										// D6 : focus (relative)
										// D7 : Iris (absolute)
										// D8 : Iris (relative)
										// D9 : Zoom (absolute)
										// D10: zoom (relative)
										// D11: PanTilt (absolute)
										// D12: PanTilt (relative)
										// D13: Roll (absolute)
										// D14: Roll (relative)
										// D15: Reserved
										// D16: Reserved
/********************** Output Terminal Descriptor ******************************************************/
 0x09,									//size of descriptor												0x38
 CS_INTERFACE,							//descriptor type													0x39
 VC_OUTPUT_TERMINAL,					//descriptor sub-type												0x3A
 0x02,									//terminal id														0x3B
 0x01,0x01,								//terminal type TT_Streaming										0x3C
 0x00,									//associated output terminal id										0x3E
 0x03,									//unit/terminal id which is connected								0x3F
 0x03,									//index of string descriptor. iTerminal								0x40
/********************** Processing Unit Descriptor ******************************************************/
 0x0B,									//size of descriptor												0x41
 CS_INTERFACE,							//descriptor type													0x42
 VC_PROCESSING_UNIT,					//descriptor sub-type												0x43
 0x03,									//unit id															0x44
 0x01,									//is connected to unit 1 output										0x45
 0x00,0x00,								//digital multipier magnification									0x46
 0x02,									//size of bmcontrol 												0x48
 0x00,0x00,								//EACH BIT MENTIONED FEATURE SUPPORTED IN VIDEOSTREAMING P66		0x49
 										// D0 : brightness
										// D1 : Contrast
										// D2 : Hue
										// D3 : Saturation
										// D4 : Sharpness
										// D5 : Gamma
										// D6 : white balance temperature
										// D7 : White balance component
										// D8 : backlight compensation
										// D9 : Gain
										// D10: Power line frequency
										// D11: Hue, auto
										// D12: white balance temperature, auto
										// D13: white balance component, auto
										// D14: Digital multipliter
										// D15: Digital multipliter limit
 0x01,									//index of string descriptor. iProcessing
/********************** Standard VC Interrupt Endpoint Descriptor ****************************************/
 0x07,									//size of descriptor
 0x05,									//endpoint descriptor
 0x83,									//In Endpoint 3
 0x03, 									//interrupt transfer type
 0x10,0x00,								//packet size 
 0x01,									//polling interval
/********************** Class Specific VC Interrupt Endpoint Descriptor ***********************************/
 0x05,									//size of descriptor
 CS_ENDPOINT,							//descriptor type
 EP_INTERRUPT,							//descriptor sub-type
 0x10,0x00,								//packet size
/********************** Standard Video Streaming Interface Descriptor ****************************************/
 0x09,									//size of descriptor
 0x04,									//descriptor type
 0x01,									//interface number
 0x00,									//alternate interface
 0x01,									//number of endpoints used by this interface
 CC_VIDEO,								//interface class
 SC_VIDEOSTREAMING,						//interface sub-class
 PC_PROTOCOL_UNDEFINED, 				//interface protocol
 0x02,									//index of string descriptor. iInterface
/********************** Video Streaming Data bulk Endpoint Descriptor *******************************************/
 0x07,									//size of descriptor
 0x05,									//endpoint descriptor
 0x81,									//In Endpoint 1
 0x02,									//bulk transfer type
 0x40,0x00,								//max packet size
 0x00,									//polling interval
/********************** Video Streaming class-spcific Header Descriptor *******************************************/
 0x0E,									//size of descriptor
 CS_INTERFACE,							//descriptor type
 VS_INPUT_HEADER,						//descriptor sub-type
 0x01,									//one format descriptor follows
 0x7F,0x00,								//total size of cleass-specific videostreaming interface descriptor
 0x81,									//In endpoint 1
 0x00, 									//no dynamic format supported
 0x02,									//this interface is connected to terminal 2 output
 0x02,									//DEVICE SUPPORT STILL IMAGE CAPTURE METHOD 3
 0x00,									//it does not support hardware trigger
 0x00,									//host how to support hardware trigger
 0x01,									//size of each bmaControls
 0x00,									//bmaControls bitfield
/********************** Video Streaming class-spcific Format Descriptor *******************************************/
 0x0B,									//size of descriptor
 CS_INTERFACE,							//descriptor type
 VS_FORMAT_MJPEG,						//video format is mjpeg
 0x01,									//first video format desc
 0x02,									//2 frame desc follow
 0x01,									//use fixed sample
 0x01,									//default fame index is 1
 0x00,									//aspectRatio-X
 0x00,									//aspectRatio-Y
 0x00, 									//non-interlace stream
 0x00,									//copy protect
/********************** Video Streaming class-spcific Frame Descriptor *******************************************/
 0x26,									//size of descriptor
 CS_INTERFACE,							//descriptor type
 VS_FRAME_MJPEG,						//mjpeg frame
 0x01,									//first frame desc
 0x00,									//still image capture is NOT supported in this frame setting
 0x40,0x01,								//width : 320
 0xF0,0x00,								//height : 240
 0x80,0x94,0x00,0x00,					//min bit-rate bit/s
 0x00,0xEC,0x0D,0x00,					//max bit-rate bit/s
 0x00,0xC0,0x00,0x00,					//max video/image size byte
 0x20,0xA1,0x07,0x00,					//default frame interval(ns)
 0x00,									//interval type
 0x20,0xA1,0x07,0x00,					//min frame interval
 0x20,0xA1,0x07,0x00,					//max frame interval
 0x00,0x00,0x00,0x00,					//frame interval step is NOT supported
/********************** Video Streaming class-spcific Frame Descriptor *******************************************/
 0x26,
 CS_INTERFACE,
 VS_FRAME_MJPEG,
 0x02,
 0x00,
 0x40,0x01,
 0xF0,0x00, 
 0x80,0x94,0x00,0x00,
 0x00,0xEC,0x0D,0x00,
 0x00,0xC0,0x00,0x00,
 0x20,0xA1,0x07,0x00, 
 0x00,
 0x20,0xA1,0x07,0x00,
 0x20,0xA1,0x07,0x00,
 0x00,0x00,0x00,0x00,
/********************** Video Streaming class-spcific Still Image Frame Descriptor **********************************/
 0x14,									//descriptor size
 CS_INTERFACE,							//descriptor type
 VS_STILL_IMAGE_FRAME,					//still image
 0x00,									//In endpoint 1
 0x03,									//number of pattern
 0x40,0x01,								//width : 320
 0xF0,0x00,								//height: 240
 0x40,0x01,								//width : 320
 0xF0,0x00,								//height: 240
 0x40,0x01,								//width : 320
 0xF0,0x00,								//height: 240
 0x02,									//number of pattern compression
 0x03,		 							//compression of image in pattern 1
 0x02,									//compression of image in pattern 2
/********************** Video Streaming class-spcific Color Format Descriptor **************************************/
 0x06,									//descriptor size
 CS_INTERFACE,
 VS_COLORFORMAT,
 0x00,0x00,0x00
};

//language string descriptor
__align(4) const USB_STRING_LANGUAGE_DESCRIPTOR_T g_UPCC_LanguageStr []=
{
    sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (USB_LANGUAGE_ENGLISH)
};

//manufacture string descriptor
__align(4) const UPCC_STRING_MANUFACTURER_DESCRIPTOR_T  g_UPCC_StrManufacturer[] =
{
    sizeof(UPCC_STRING_MANUFACTURER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651 */
    'S', 0,'p', 0,'r', 0,'e', 0,'a', 0,'d', 0,'t', 0,
    'r', 0,'u', 0,'m', 0,' ', 0,' ', 0
	}
};

//product string descriptor
__align(4) const UPCC_STRING_PRODUCT_DESCRIPTOR_T  g_UPCC_StrProduct[] =
{
    sizeof(UPCC_STRING_PRODUCT_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    {/*lint !e651 */
    'P', 0,'C', 0,' ', 0,'C', 0,'a', 0,'m', 0,'e', 0,'r', 0,
    'a', 0,'1', 0,'.', 0,'0', 0
	}
};

//serial number string descriptor
__align(4) const UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T  g_UPCC_SerialNumStr[] =
{
    sizeof(UPCC_STRING_SERIALNUMBER_DESCRIPTOR_T),
    USB_STRING_DESCRIPTOR_TYPE,
    //UNICODE character string; 
    { /*lint !e651 */
		'S',0,'p',0,'r',0,'e',0,'a',0,'d',0,'t',0,'r',0,
		'u',0,'m',0,'_',0,'M',0
	}
    //It must be modified in initialization.
};
PUBLIC uint32 UPCC_GetCOnfigDescriptor(uint8 ** cfg_ptr )
{
	*cfg_ptr = g_UPCC_ConfigDescr;
	return sizeof(g_UPCC_ConfigDescr); 
}

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/
 
 
/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*/
#endif //_UPCC_CONFIG_C_

/******************************************************************************
 ** File Name:      sx_udc.h                                                                                    *
 ** Author:                                                                                             *
 ** DATE:                                                                                           *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.                                   *
 ** Description:                                                                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/
#ifndef _SX_UDC_H
#define _SX_UDC_H


#include "dal_chr.h"
#include "sci_types.h"
#include "tb_drv.h"
#include "gadget.h"
#include "usblist.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifdef FLASH_SUPPORT
#if WIN32
#define __packed  
#endif
#endif

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define USB_12M_CLK 0x00
#define USB_48M_CLK 0x01
#define USB_MAX_CLK 0x02

#define UDC_DETECT_TIMER        (2000) //500ms

#define USB_SPECIFICATION_VERSION                 0x0110
#define USB_MAXIMUM_STRING_LENGTH                 255


#define USB_GETSTATUS_SELF_POWERED                0x01
#define USB_GETSTATUS_REMOTE_WAKEUP_ENABLED       0x02


#define USB_DEVICE_DESCRIPTOR_TYPE                0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE         0x02
#define USB_STRING_DESCRIPTOR_TYPE                0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE             0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE              0x05
#define USB_POWER_DESCRIPTOR_TYPE                 0x06

#define USB_STR_INDEX_LANGUAGE                        0x00
#define USB_STR_INDEX_MANUFACTURER                    0x01
#define USB_STR_INDEX_PRODUCT                         0x02
#define USB_STR_INDEX_SERIALNUMBER                    0x03
#define USB_STR_INDEX_CONFIGURATION                   0x04
#define USB_STR_INDEX_INTERFACE                       0x05

#define USB_LANGUAGE_ENGLISH                          0x0409

#define USB_DESCRIPTOR_MAKE_TYPE_AND_INDEX(d, i) ((uint16)((uint16)d<<8 | i))

//
// Values for bmAttributes field of an
// endpoint descriptor
//

#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03


//
// definitions for bits in the bmAttributes field of a
// configuration descriptor.
//
#define USB_CONFIG_POWERED_MASK                   0xc0

#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

//
// Endpoint direction bit, stored in address
//

#define USB_ENDPOINT_DIRECTION_MASK               0x80

// test direction bit in the bEndpointAddress field of
// an endpoint descriptor.
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//
// USB defined request codes
// see chapter 9 of the USB 1.0 specifcation for
// more information.
//

// These are the correct values based on the USB 1.0
// specification

#define USB_REQUEST_GET_STATUS                    0x00
#define USB_REQUEST_CLEAR_FEATURE                 0x01

#define USB_REQUEST_SET_FEATURE                   0x03

#define USB_REQUEST_SET_ADDRESS                   0x05
#define USB_REQUEST_GET_DESCRIPTOR                0x06
#define USB_REQUEST_SET_DESCRIPTOR                0x07
#define USB_REQUEST_GET_CONFIGURATION             0x08
#define USB_REQUEST_SET_CONFIGURATION             0x09
#define USB_REQUEST_GET_INTERFACE                 0x0A
#define USB_REQUEST_SET_INTERFACE                 0x0B
#define USB_REQUEST_SYNC_FRAME                    0x0C
#define USB_REQUEST_MAX                           0x0D

//
// defined USB device classes
//


#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF

//
// USB defined Feature selectors
//

#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005


//Define direction ;
#define USB_HOST_IN     0x01        //From device to host ;
#define USB_HOST_OUT    0x00        //From host to device ;

//Define request type;
#define USB_REQ_STANDARD    0x00
#define USB_REQ_CLASS       0x01
#define USB_REQ_VENDOR      0x02
#define USB_REQ_RESERVED    0x03

//Define accepter ;
#define USB_REC_DEVICE      0x00
#define USB_REC_INTERFACE   0x01
#define USB_REC_ENDPOINT    0x02
#define USB_REC_OTHER       0x03

#define USB_MAX_REQ_TYPE    0x04
#define USB_MAX_RECIPIENT   0x05
#define USB_MAX_REQUEST     0x30

#define USB_BUS_POWERED                           0x80
#define USB_SELF_POWERED                          0x40
#define USB_REMOTE_WAKEUP                         0x20

//
// USB power descriptor added to core specification
//

#define USB_SUPPORT_D0_COMMAND      0x01
#define USB_SUPPORT_D1_COMMAND      0x02
#define USB_SUPPORT_D2_COMMAND      0x04
#define USB_SUPPORT_D3_COMMAND      0x08

#define USB_SUPPORT_D1_WAKEUP       0x10
#define USB_SUPPORT_D2_WAKEUP       0x20


#define USB_CLASS_CODE_MASSSTORAGE_CLASS_DEVICE             0x08

#define USB_SUBCLASS_CODE_RBC                               0x01
#define USB_SUBCLASS_CODE_SFF8020I                          0x02
#define USB_SUBCLASS_CODE_QIC157                            0x03
#define USB_SUBCLASS_CODE_UFI                               0x04
#define USB_SUBCLASS_CODE_SFF8070I                          0x05
#define USB_SUBCLASS_CODE_SCSI                              0x06

#define USB_PROTOCOL_CODE_CBI0                              0x00
#define USB_PROTOCOL_CODE_CBI1                              0x01
#define USB_PROTOCOL_CODE_BULK                              0x50





/*********** UDC REGISTER DEFINITION **********************/
#define SX_USBDREG(x) (x)

#define SX_UDC_USB_CTL_REG                  SX_USBDREG(0x0000)
#define SX_UDC_USB_STS_REG                  SX_USBDREG(0x0004)
#define SX_UDC_USB_EP_RST_REG               SX_USBDREG(0x000C)
#define SX_UDC_USB_SIE_CTL_REG              SX_USBDREG(0x0010)
#define SX_UDC_USB_INT_CTL_REG              SX_USBDREG(0x0014)
#define SX_UDC_USB_INT_STS_REG              SX_USBDREG(0x0018)
#define SX_UDC_USB_INT_CLR_REG              SX_USBDREG(0x001C)
#define SX_UDC_USB_INT_RAW_REG              SX_USBDREG(0x0020)
#define SX_UDC_USB_FRM_NUM_REG              SX_USBDREG(0x0024)
#define SX_UDC_USB_TIM_OUT_REG              SX_USBDREG(0x0028)

#define SX_UDC_USB_EP0_IN_SIZE_REG          SX_USBDREG(0x0040)
#define SX_UDC_USB_EP0_OT_SIZE_REG          SX_USBDREG(0x0044)
#define SX_UDC_USB_EP0_SEND_SIZE_REG        SX_USBDREG(0x0048)
#define SX_UDC_USB_EP0_RECV_SIZE_REG        SX_USBDREG(0x004C)

#define SX_UDC_USB_RMT_SEND_REG             SX_USBDREG(0x0050)
#define SX_UDC_USB_CFG_VALUE_REG            SX_USBDREG(0x0054)
#define SX_UDC_USB_ITF_ALT_REG              SX_USBDREG(0x0058)
#define SX_UDC_USB_REQ_SETUP_LOW_REG        SX_USBDREG(0x005C)
#define SX_UDC_USB_REQ_SETUP_HIG_REG        SX_USBDREG(0x0060)

#define SX_UDC_USB_EP0_CTL_REG              SX_USBDREG(0x0064)
#define SX_UDC_USB_EP0_INT_CTL_REG          SX_USBDREG(0x0068)
#define SX_UDC_USB_EP0_INT_STS_REG          SX_USBDREG(0x006C)
#define SX_UDC_USB_EP0_INT_CLR_REG          SX_USBDREG(0x0070)
#define SX_UDC_USB_EP0_INT_RAW_REG          SX_USBDREG(0x0074)
#define SX_UDC_USB_EP0_DMA_IN_WAT_REG       SX_USBDREG(0x0078)
#define SX_UDC_USB_EP0_DMA_OT_WAT_REG       SX_USBDREG(0x007C)

#define SX_UDC_USB_EP1_CTL_REG              SX_USBDREG(0x00C0)
#define SX_UDC_USB_EP1_SEND_DATA_REG        SX_USBDREG(0x00C4)
#define SX_UDC_USB_EP1_TRN_SIZE_REG         SX_USBDREG(0x00C8)
#define SX_UDC_USB_EP1_INT_CTL_REG          SX_USBDREG(0x00CC)
#define SX_UDC_USB_EP1_INT_STS_REG          SX_USBDREG(0x00D0)
#define SX_UDC_USB_EP1_INT_CLR_REG          SX_USBDREG(0x00D4)
#define SX_UDC_USB_EP1_INT_RAW_REG          SX_USBDREG(0x00D8)
#define SX_UDC_USB_EP1_DMA_WAT_REG          SX_USBDREG(0x00DC)

#define SX_UDC_USB_EP2_CTL_REG              SX_USBDREG(0x0100)
#define SX_UDC_USB_EP2_RECV_DATA_REG        SX_USBDREG(0x0104)
#define SX_UDC_USB_EP2_TRN_SIZE             SX_USBDREG(0x0108)
#define SX_UDC_USB_EP2_INT_CTL_REG          SX_USBDREG(0x010C)
#define SX_UDC_USB_EP2_INT_STS_REG          SX_USBDREG(0x0110)
#define SX_UDC_USB_EP2_INT_CLR_REG          SX_USBDREG(0x0114)
#define SX_UDC_USB_EP2_INT_RAW_REG          SX_USBDREG(0x0118)
#define SX_UDC_USB_EP2_DMA_WAT_REG          SX_USBDREG(0x011C)

#define SX_UDC_USB_EP3_CTL_REG              SX_USBDREG(0x0140)
#define SX_UDC_USB_EP3_SEND_DATA_REG        SX_USBDREG(0x0144)
#define SX_UDC_USB_EP3_TRN_SIZE_REG         SX_USBDREG(0x0148)
#define SX_UDC_USB_EP3_INT_CTL_REG          SX_USBDREG(0x014C)
#define SX_UDC_USB_EP3_INT_STS_REG          SX_USBDREG(0x0150)
#define SX_UDC_USB_EP3_INT_CLR_REG          SX_USBDREG(0x0154)
#define SX_UDC_USB_EP3_INT_RAW_REG          SX_USBDREG(0x0158)
#define SX_UDC_USB_EP3_DMA_WAT_REG          SX_USBDREG(0x015C)

#define SX_UDC_USB_EP4_CTL_REG              SX_USBDREG(0x0180)
#define SX_UDC_USB_EP4_RECV_DATA_REG        SX_USBDREG(0x0184)
#define SX_UDC_USB_EP4_TRN_SIZE             SX_USBDREG(0x0188)
#define SX_UDC_USB_EP4_INT_CTL_REG          SX_USBDREG(0x018C)
#define SX_UDC_USB_EP4_INT_STS_REG          SX_USBDREG(0x0190)
#define SX_UDC_USB_EP4_INT_CLR_REG          SX_USBDREG(0x0194)
#define SX_UDC_USB_EP4_INT_RAW_REG          SX_USBDREG(0x0198)
#define SX_UDC_USB_EP4_DMA_WAT_REG          SX_USBDREG(0x019C)
/*************************************************************/


/*********** BIT DEFINITION OF REGISTER **********************/
#define SX_UDC_USB_CTL_ENABLE_BIT                       (1<<0) // R/W
#define SX_UDC_USB_CTL_DISABLE_BIT                      (0<<0) // R/W
#define SX_UDC_USB_CTL_SELFPWR_BIT                      (1<<1) // R/W
#define SX_UDC_USB_CTL_BUSPWR_BIT                       (0<<1) // R/W


#define SX_UDC_USB_INT_CLR_SOFPAK_BIT                       (1<<0) // R/W
#define SX_UDC_USB_INT_CLR_SUSPND_BIT                       (1<<1) // R/W
#define SX_UDC_USB_INT_CLR_RESUME_BIT                       (1<<2) // R/W
#define SX_UDC_USB_INT_CLR_RESET_BIT                        (1<<3) // R/W
#define SX_UDC_USB_INT_CLR_UNPID_BIT                        (1<<4) // R/W
#define SX_UDC_USB_INT_CLR_PIDERR_BIT                       (1<<5) // R/W
#define SX_UDC_USB_INT_CLR_STACHG_BIT                       (1<<6) // R/W

#define SX_UDC_USB_EP0_INT_CLR_TRNEND_BIT                   (1<<0) // R/W       Transcation end
#define SX_UDC_USB_EP0_INT_CLR_TOGERR_BIT                   (1<<1) // R/W       data toggle err
#define SX_UDC_USB_EP0_INT_CLR_INTIMOUT_BIT                 (1<<2) // R/W       in wait ack timeout
#define SX_UDC_USB_EP0_INT_CLR_OTTIMOUT_BIT                 (1<<3) // R/W       out wait data timeout
#define SX_UDC_USB_EP0_INT_CLR_STTIMOUT_BIT                 (1<<4) // R/W       setup wait data timeout
#define SX_UDC_USB_EP0_INT_CLR_CRC16ERR_BIT                 (1<<5) // R/W       crc16 err
#define SX_UDC_USB_EP0_INT_CLR_BUFEMPTY_BIT                 (1<<6) // R/W       buffer empty send in
#define SX_UDC_USB_EP0_INT_CLR_BUFFULL_BIT                  (1<<7) // R/W       buffer full send out
#define SX_UDC_USB_EP0_INT_CLR_STTRNEND_BIT                 (1<<8) // R/W       setup transcation end
#define SX_UDC_USB_EP0_INT_CLR_TRNSFEND_BIT                 (1<<9) // R/W       ep0 transfer end
#define SX_UDC_USB_EP0_INT_CLR_INDMATMO_BIT                 (1<<10) // R/W      ep0 in wait dma ack timeout
#define SX_UDC_USB_EP0_INT_CLR_OTDMATMO_BIT                 (1<<11) // R/W      ep0 out wait dma ack timeout
#define SX_UDC_USB_EP0_INT_CLR_BUFNORDY_BIT                 (1<<12) // R/W      ep0 buffer not ready to receive
#define SX_UDC_USB_EP0_INT_CLR_DATNORDY_BIT                 (1<<13) // R/W      ep0 data not ready to host

#define SX_UDC_USB_EP1_INT_CLR_TRNEND_BIT                   (1<<0)  // R/W
#define SX_UDC_USB_EP1_INT_CLR_TIMOUT_BIT                   (1<<2)  // R/W
#define SX_UDC_USB_EP1_INT_CLR_BUFEMP_BIT                   (1<<6)  // R/W
#define SX_UDC_USB_EP1_INT_CLR_TRNSFEND_BIT                 (1<<9)  // R/W
#define SX_UDC_USB_EP1_INT_CLR_OTDMATMO_BIT                 (1<<10) // R/W
#define SX_UDC_USB_EP1_INT_CLR_DATNORDY_BIT                 (1<<13) // R/W

#define SX_UDC_USB_EP2_INT_CLR_TRNEND_BIT                   (1<<0)  // R/W
#define SX_UDC_USB_EP2_INT_CLR_TOGERR_BIT                   (1<<1) // R/W       data toggle err
#define SX_UDC_USB_EP2_INT_CLR_TIMOUT_BIT                   (1<<3) // R/W       out wait data timeout
#define SX_UDC_USB_EP2_INT_CLR_CRC16ERR_BIT                 (1<<5) // R/W       crc16 err
#define SX_UDC_USB_EP2_INT_CLR_BUFFULL_BIT                  (1<<7) // R/W       buffer full send out
#define SX_UDC_USB_EP2_INT_CLR_TRNSFEND_BIT                 (1<<9) // R/W       ep0 transfer end
#define SX_UDC_USB_EP2_INT_CLR_OTDMATMO_BIT                 (1<<11) // R/W      ep0 out wait dma ack timeout
#define SX_UDC_USB_EP2_INT_CLR_BUFNORDY_BIT                 (1<<12) // R/W      ep0 buffer not ready to receive


#define SX_UDC_USB_EP3_INT_CLR_TRNEND_BIT                   (1<<0)  // R/W
#define SX_UDC_USB_EP3_INT_CLR_TIMOUT_BIT                   (1<<2)  // R/W
#define SX_UDC_USB_EP3_INT_CLR_BUFEMP_BIT                   (1<<6)  // R/W
#define SX_UDC_USB_EP3_INT_CLR_TRNSFEND_BIT                 (1<<9)  // R/W
#define SX_UDC_USB_EP3_INT_CLR_OTDMATMO_BIT                 (1<<10) // R/W
#define SX_UDC_USB_EP3_INT_CLR_DATNORDY_BIT                 (1<<13) // R/W

#define SX_UDC_USB_EP4_INT_CLR_TRNEND_BIT                   (1<<0)  // R/W
#define SX_UDC_USB_EP4_INT_CLR_TOGERR_BIT                   (1<<1) // R/W       data toggle err
#define SX_UDC_USB_EP4_INT_CLR_TIMOUT_BIT                   (1<<3) // R/W       out wait data timeout
#define SX_UDC_USB_EP4_INT_CLR_CRC16ERR_BIT                 (1<<5) // R/W       crc16 err
#define SX_UDC_USB_EP4_INT_CLR_BUFFULL_BIT                  (1<<7) // R/W       buffer full send out
#define SX_UDC_USB_EP4_INT_CLR_TRNSFEND_BIT                 (1<<9) // R/W       ep0 transfer end
#define SX_UDC_USB_EP4_INT_CLR_OTDMATMO_BIT                 (1<<11) // R/W      ep0 out wait dma ack timeout
#define SX_UDC_USB_EP4_INT_CLR_BUFNORDY_BIT                 (1<<12) // R/W      ep0 buffer not ready to receive

/*************************************************************/



#if 0
#define SX_UDC_FUNC_ADDR_REG    SX_USBDREG(0x0140)
#define SX_UDC_PWR_REG      SX_USBDREG(0x0144)
#define SX_UDC_EP_INT_REG       SX_USBDREG(0x0148)
#define SX_UDC_EP_INT_EN_REG    SX_USBDREG(0x015c)
#define SX_UDC_USB_INT_EN_REG   SX_USBDREG(0x016c)
#define SX_UDC_FRAME_NUM1_REG   SX_USBDREG(0x0170)
#define SX_UDC_FRAME_NUM2_REG   SX_USBDREG(0x0174)
#define SX_UDC_EP0_FIFO_REG SX_USBDREG(0x01c0)
#define SX_UDC_EP1_FIFO_REG SX_USBDREG(0x01c4)
#define SX_UDC_EP2_FIFO_REG SX_USBDREG(0x01c8)
#define SX_UDC_EP3_FIFO_REG SX_USBDREG(0x01cc)
#define SX_UDC_EP4_FIFO_REG SX_USBDREG(0x01d0)

#define SX_UDC_EP1_DMA_CON      SX_USBDREG(0x0200)
#define SX_UDC_EP1_DMA_UNIT SX_USBDREG(0x0204)
#define SX_UDC_EP1_DMA_FIFO SX_USBDREG(0x0208)
#define SX_UDC_EP1_DMA_TTC_L    SX_USBDREG(0x020c)
#define SX_UDC_EP1_DMA_TTC_M    SX_USBDREG(0x0210)
#define SX_UDC_EP1_DMA_TTC_H    SX_USBDREG(0x0214)

#define SX_UDC_EP2_DMA_CON      SX_USBDREG(0x0218)
#define SX_UDC_EP2_DMA_UNIT SX_USBDREG(0x021c)
#define SX_UDC_EP2_DMA_FIFO SX_USBDREG(0x0220)
#define SX_UDC_EP2_DMA_TTC_L    SX_USBDREG(0x0224)
#define SX_UDC_EP2_DMA_TTC_M    SX_USBDREG(0x0228)
#define SX_UDC_EP2_DMA_TTC_H    SX_USBDREG(0x022c)

#define SX_UDC_EP3_DMA_CON      SX_USBDREG(0x0240)
#define SX_UDC_EP3_DMA_UNIT SX_USBDREG(0x0244)
#define SX_UDC_EP3_DMA_FIFO SX_USBDREG(0x0248)
#define SX_UDC_EP3_DMA_TTC_L    SX_USBDREG(0x024c)
#define SX_UDC_EP3_DMA_TTC_M    SX_USBDREG(0x0250)
#define SX_UDC_EP3_DMA_TTC_H    SX_USBDREG(0x0254)

#define SX_UDC_EP4_DMA_CON      SX_USBDREG(0x0258)
#define SX_UDC_EP4_DMA_UNIT SX_USBDREG(0x025c)
#define SX_UDC_EP4_DMA_FIFO SX_USBDREG(0x0260)
#define SX_UDC_EP4_DMA_TTC_L    SX_USBDREG(0x0264)
#define SX_UDC_EP4_DMA_TTC_M    SX_USBDREG(0x0268)
#define SX_UDC_EP4_DMA_TTC_H    SX_USBDREG(0x026c)

#define SX_UDC_INDEX_REG        SX_USBDREG(0x0178)

/* indexed registers */

#define SX_UDC_MAXP_REG     SX_USBDREG(0x0180)

#define SX_UDC_EP0_CSR_REG      SX_USBDREG(0x0184)

#define SX_UDC_IN_CSR1_REG      SX_USBDREG(0x0184)
#define SX_UDC_IN_CSR2_REG      SX_USBDREG(0x0188)

#define SX_UDC_OUT_CSR1_REG SX_USBDREG(0x0190)
#define SX_UDC_OUT_CSR2_REG SX_USBDREG(0x0194)
#define SX_UDC_OUT_FIFO_CNT1_REG    SX_USBDREG(0x0198)
#define SX_UDC_OUT_FIFO_CNT2_REG    SX_USBDREG(0x019c)

#define SX_UDC_FUNCADDR_UPDATE  (1<<7)

#define SX_UDC_PWR_ISOUP        (1<<7) // R/W
#define SX_UDC_PWR_RESET        (1<<3) // R
#define SX_UDC_PWR_RESUME       (1<<2) // R/W
#define SX_UDC_PWR_SUSPEND      (1<<1) // R
#define SX_UDC_PWR_ENSUSPEND    (1<<0) // R/W

#define SX_UDC_PWR_DEFAULT      0x00

#define SX_UDC_INT_EP4      (1<<4) // R/W (clear only)
#define SX_UDC_INT_EP3      (1<<3) // R/W (clear only)
#define SX_UDC_INT_EP2      (1<<2) // R/W (clear only)
#define SX_UDC_INT_EP1      (1<<1) // R/W (clear only)
#define SX_UDC_INT_EP0      (1<<0) // R/W (clear only)

#define SX_UDC_USBINT_RESET (1<<2) // R/W (clear only)
#define SX_UDC_USBINT_RESUME    (1<<1) // R/W (clear only)
#define SX_UDC_USBINT_SUSPEND   (1<<0) // R/W (clear only)

#define SX_UDC_INTE_EP4     (1<<4) // R/W
#define SX_UDC_INTE_EP3     (1<<3) // R/W
#define SX_UDC_INTE_EP2     (1<<2) // R/W
#define SX_UDC_INTE_EP1     (1<<1) // R/W
#define SX_UDC_INTE_EP0     (1<<0) // R/W

#define SX_UDC_USBINTE_RESET    (1<<2) // R/W
#define SX_UDC_USBINTE_SUSPEND  (1<<0) // R/W


#define SX_UDC_INDEX_EP0        (0x00)
#define SX_UDC_INDEX_EP1        (0x01) // ??
#define SX_UDC_INDEX_EP2        (0x02) // ??
#define SX_UDC_INDEX_EP3        (0x03) // ??
#define SX_UDC_INDEX_EP4        (0x04) // ??

#define SX_UDC_ICSR1_CLRDT      (1<<6) // R/W
#define SX_UDC_ICSR1_SENTSTL    (1<<5) // R/W (clear only)
#define SX_UDC_ICSR1_SENDSTL    (1<<4) // R/W
#define SX_UDC_ICSR1_FFLUSH (1<<3) // W   (set only)
#define SX_UDC_ICSR1_UNDRUN (1<<2) // R/W (clear only)
#define SX_UDC_ICSR1_PKTRDY (1<<0) // R/W (set only)

#define SX_UDC_ICSR2_AUTOSET    (1<<7) // R/W
#define SX_UDC_ICSR2_ISO        (1<<6) // R/W
#define SX_UDC_ICSR2_MODEIN (1<<5) // R/W
#define SX_UDC_ICSR2_DMAIEN (1<<4) // R/W

#define SX_UDC_OCSR1_CLRDT      (1<<7) // R/W
#define SX_UDC_OCSR1_SENTSTL    (1<<6) // R/W (clear only)
#define SX_UDC_OCSR1_SENDSTL    (1<<5) // R/W
#define SX_UDC_OCSR1_FFLUSH (1<<4) // R/W
#define SX_UDC_OCSR1_DERROR (1<<3) // R
#define SX_UDC_OCSR1_OVRRUN (1<<2) // R/W (clear only)
#define SX_UDC_OCSR1_PKTRDY (1<<0) // R/W (clear only)

#define SX_UDC_OCSR2_AUTOCLR    (1<<7) // R/W
#define SX_UDC_OCSR2_ISO        (1<<6) // R/W
#define SX_UDC_OCSR2_DMAIEN (1<<5) // R/W

#define SX_UDC_EP0_CSR_OPKRDY   (1<<0)
#define SX_UDC_EP0_CSR_IPKRDY   (1<<1)
#define SX_UDC_EP0_CSR_SENTSTL  (1<<2)
#define SX_UDC_EP0_CSR_DE       (1<<3)
#define SX_UDC_EP0_CSR_SE       (1<<4)
#define SX_UDC_EP0_CSR_SENDSTL  (1<<5)
#define SX_UDC_EP0_CSR_SOPKTRDY (1<<6)
#define SX_UDC_EP0_CSR_SSE  (1<<7)

#define SX_UDC_MAXP_8       (1<<0)
#define SX_UDC_MAXP_16      (1<<1)
#define SX_UDC_MAXP_32      (1<<2)
#define SX_UDC_MAXP_64      (1<<3)
#endif

#define REQ_IS_IDLE        0
#define REQ_IN_QUEUE       1
#define REQ_IS_SENDING     2
#define REQ_SPLIT_SENDING  3



/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
#if 0
enum sx_udc_cmd_e {
    SX_UDC_P_ENABLE = 1,    /* Pull-up enable        */
    SX_UDC_P_DISABLE    = 2,    /* Pull-up disable       */
    SX_UDC_P_RESET  = 3,    /* UDC reset, in case of */
};
struct sx_udc_mach_info
{
    void (*udc_command) (enum sx_udc_cmd_e);
    void (*vbus_draw) (uint32 ma);
    uint32 vbus_pin;
    uint8 vbus_pin_inverted;
};
#endif



//Stand USB device request command type define ;
typedef union _USB_STD_DEV_REQ_U
{
    struct
    {
        union
        {
            struct bit_map
            {
                uint direction :1;
                uint command   :2;
                uint receiver  :5;
            } mBits;
            uint8 bValue;
        } bmReqType;
        uint8  bReqCode;
        uint16 wValue;
        uint16 wIndex;
        uint16 wLength;
    } sData;
    uint8 bBuf[8];
} USB_STD_DEV_REQ_U;

typedef __packed struct _USB_DEVICE_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint16   bcdUSB;
    uint8    bDeviceClass;
    uint8    bDeviceSubClass;
    uint8    bDeviceProtocol;
    uint8    bMaxPacketSize0;
    uint16   idVendor;
    uint16   idProduct;
    uint16   bcdDevice;
    uint8    iManufacturer;
    uint8    iProduct;
    uint8    iSerialNumber;
    uint8    bNumConfigurations;
} USB_DEVICE_DESCRIPTOR_T, *PUSB_DEVICE_DESCRIPTOR_T;

//
// values for bmAttributes Field in
// USB_CONFIGURATION_DESCRIPTOR
//
typedef __packed struct _USB_CONFIGURATION_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint16   wTotalLength;
    uint8    bNumInterfaces;
    uint8    bConfigurationValue;
    uint8    iConfiguration;
    uint8    bmAttributes;
    uint8    MaxPower;
} USB_CONFIGURATION_DESCRIPTOR_T, *PUSB_CONFIGURATION_DESCRIPTOR_T;

typedef __packed struct _USB_INTERFACE_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bInterfaceNumber;
    uint8    bAlternateSetting;
    uint8    bNumEndpoints;
    uint8    bInterfaceClass;
    uint8    bInterfaceSubClass;
    uint8    bInterfaceProtocol;
    uint8    iInterface;
} USB_INTERFACE_DESCRIPTOR_T, *PUSB_INTERFACE_DESCRIPTOR_T;


typedef __packed struct _USB_ENDPOINT_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bEndpointAddress;
    uint8    bmAttributes;
    uint16   wMaxPacketSize;
    uint8    bInterval;
} USB_ENDPOINT_DESCRIPTOR_T, *PUSB_ENDPOINT_DESCRIPTOR_T;

typedef __packed struct _USB_STRING_LANGUAGE_DESCRIPTOR
{
    uint8  bLength;
    uint8  bDescriptorType;
    uint16 ulanguageID;
} USB_STRING_LANGUAGE_DESCRIPTOR_T,*PUSB_STRING_LANGUAGE_DESCRIPTOR_T;

typedef __packed struct _USB_POWER_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
    uint8    bCapabilitiesFlags;
    uint16   EventNotification;
    uint16   D1LatencyTime;
    uint16   D2LatencyTime;
    uint16   D3LatencyTime;
    uint8    PowerUnit;
    uint16   D0PowerConsumption;
    uint16   D1PowerConsumption;
    uint16   D2PowerConsumption;
} USB_POWER_DESCRIPTOR_T, *PUSB_POWER_DESCRIPTOR_T;


typedef __packed struct _USB_COMMON_DESCRIPTOR
{
    uint8    bLength;
    uint8    bDescriptorType;
} USB_COMMON_DESCRIPTOR_T, *PUSB_COMMON_DESCRIPTOR_T;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*/


#define USB_SPREADTRUM_VENDER_ID          0x1782
#define USB_SPREADTRUM_CHIP_ID            0x4d00
#define USB_SPREADTRUM_PID_TWOINTERFACE   0x3d00
#define USB_SPREADTRUM_VCOM_MODEM_ID      0x3d01  /*single modem*/
#define USB_SPREADTRUM_CDROM_MODEM_ID     0x3d02  /*modem + cdrom*/

#define USB_SPREADTRUM_MASS_STORAGE_ID    0x4d01
#define USB_SPREADTRUM_VCOM_ID            0x4d00
#define USB_SPREADTRUM_PC_CAMERA_ID       0x4d04

#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
#define USB_WORD_SWAP_ENDIAN(x)   ((x)&0xFFFF)
#else
#define USB_WORD_SWAP_ENDIAN(x)   ((((x) & 0x0FF) << 8) | (((x) >> 8) & 0x0FF))&0xFFFF /*lint !e572*/
#endif

#define USB_DWORD_SWAPENDIAN(x){\
        (((x & 0x0FF00)<< 8)|((x & 0x0FF)<<24) |\
         ((x & 0xFF000000)>>24)|((x & 0xFF0000)>>8))}


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct  usb_isr_tag
{
    TB_ISR     usb_irq;
    TB_ISR     dma_epin;
    TB_ISR     dma_epout;
    TB_ISR     dma_ep1;
    TB_ISR     dma_ep2;
    TB_ISR     dma_ep3;
    TB_ISR     dma_ep4;
} USB_ISR_PTR_T;

typedef enum
{
    USB_SUCCESS = 0,
    USB_CONNECTING,
    USB_OTHER
} USB_ERROR_E;

typedef union
{
    struct
    {
#ifdef CHIP_ENDIAN_LITTLE
        uint   index   :8;
        uint   type    :8;
#else
        uint   type    :8;
        uint   index   :8;
#endif
    } bmDescriptor;
    short wValue;
} USB_HWORD_VALUE_U;

typedef enum
{
    USB_DEFAULT_POWER = 0,
    USB_BUS_POWER     = 0,
    USB_SELF_POWER    = 1
} USB_POWER_E;

typedef enum
{
    USB_ERR_INACK_TIMEOUT   = 0x0,
    USB_ERR_INVALID_CLASSCMD,
    USB_ERR_INVALID_VENDORCMD,
    USB_ERR_BSD_GETDEV_INFO,
    USB_ERR_BSD_OPENDEV    ,
    USB_ERR_BSD_CLOSEDEV   ,
    USB_ERR_PARAM_ERR ,
    USB_ERR_MAX
} USB_ERROR_TYPE_E;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SxUDC_Disable (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SxUDC_Enable (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
//LOCAL void sx_udc_enableirq(struct sx_udc *dev);
LOCAL void SxUDC_EnableIrq (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
//LOCAL void sx_udc_disableirq(struct sx_udc *dev);
LOCAL void sx_udc_disableirq (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 SxUDC_GetConnectState (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
//LOCAL void SxUDC_Config(struct sx_udc *dev);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 SxUDC_Connect (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 SxUDC_Disconnect (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL int SxUDC_PullUp (USB_GADGET_T *gadget, int is_on);

LOCAL void SxUDC_ClkReset (void);


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void SxUDC_SetHostReq (uint32 state);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL uint32 SxUDC_GetHostReq();
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_DWordEndianSwap (uint32 dWord);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           len is the number of word !
/*****************************************************************************/
//PUBLIC BOOLEAN USB_DWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_RxDWordFromFIFO (uint32 *buf ,uint8 len ,uint8 ep_id);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_TxDWordToFIFO (uint32 *buf ,uint8 len ,uint8 ep_id);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Connect (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Disconnect (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EnableIrq (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_DisableIrq (void);

/*****************************************************************************/
//  Description:    This function reset the usb clock
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Reset (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T USB_irq (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epin (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epout (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep1 (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep2 (uint32 param);


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep3 (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep4 (uint32 param);


/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataOutCtrl (BOOLEAN ctrl,uint32 ep_id);
/*****************************************************************************/
//  Description:    Call this function to control epx data in ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataInCtrl (BOOLEAN ctrl,uint32 ep_id);

/*****************************************************************************/
//  Description:    This function set usb is self-power or bus-power
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_PowerSet (USB_POWER_E power);
/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxFIFOReset (uint32 ep_id);

/*****************************************************************************/
//  Description:    This function is called when usb protctol error occurs
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           value: usb error type
/*****************************************************************************/
PUBLIC void USB_ErrorHandle (USB_ERROR_TYPE_E value);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function will expend 30 ms so it can't be used in
//                  interrupt handler ;
/*****************************************************************************/
PUBLIC void USB_ClkReset (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_EP1InDmaReq (uint32 *pBuf,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_EP2OutDmaReq (uint32 *pBuf,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_DisableAllEndPointDMA (void);

/*****************************************************************************/
//  Description:    Set plug out count
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void USB_SetPlugOutCount (BOOLEAN is_increase);

PUBLIC void USB_LDO_Control (BOOLEAN flag);
PUBLIC void UDC_HWInit (void);
PUBLIC void UDC_HWUNInit (void);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/


// End  from usbdrv.h
typedef struct sx_udc SX_UDC_T;
struct sx_request;

typedef struct sx_ep
{
    LIST_HEAD_T                             queue;
    USB_EP_T                                ep;
    USB_GADGET_T                            *gadget;
    SX_UDC_T                                *dev;
    struct sx_request                       *cur_req;

    uint32                                  halted : 1;
    uint32                                  already_seen : 1;
    uint32                                  setup_stage : 1;
} SX_EP_T;


/* Warning : ep0 has a fifo of 16 bytes */
/* Don't try to set 32 or 64            */
/* also testusb 14 fails  wit 16 but is */
/* fine with 8                          */
#define EP0_FIFO_SIZE        8
#define EP_FIFO_SIZE        64
#define SELF_POWER_STATE    0x02

#define SX_EP_FIFO_SIZE 128


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define SX_ENDPOINTS       5

typedef struct sx_request
{
    LIST_HEAD_T     queue;      /* ep's requests */
    USB_REQUEST_T       req;
} SX_REQUEST_T;

enum ep0_state
{
    EP0_IDLE,
    EP0_IN_DATA_PHASE,
    EP0_OUT_DATA_PHASE,
    EP0_END_XFER,
    EP0_STALL,
};

static const char *ep0states[]=
{
    "EP0_IDLE",
    "EP0_IN_DATA_PHASE",
    "EP0_OUT_DATA_PHASE",
    "EP0_END_XFER",
    "EP0_STALL",
};


typedef struct sx_udc
{
    uint32                  address;
    SX_EP_T                 ep0;
    SX_EP_T                 *iep;
    SX_EP_T                 *oep;
    uint32                  iep_cnt;
    uint32                  oep_cnt;
    USB_GADGET_T            gadget;
    USB_GADGET_DRIVER_T     *driver;
    uint16                  devstatus;

    volatile uint32         connected;
    volatile uint32         hostreq;
    int                     ep0state;

    uint32                  got_irq : 1;
    uint32                  req_std : 1;
    uint32                  req_config : 1;
    uint32                  req_pending : 1;
    uint8                   vbus;
    uint8                   test_mode;
} SX_UDC_T;


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

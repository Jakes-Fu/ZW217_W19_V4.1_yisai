/******************************************************************************
 ** File Name:    usb_device.h                                                    *
 ** Author:       jiayong.yang                                                *
 ** DATE:         11/05/2011                                                    *
 ** Copyright:    2011 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 11/05/2011     weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _UDB_DEV_H_
#define _UDB_DEV_H_
#include "sci_types.h"
#include "gadget.h"


#define MAX_INTERFACE_NUM 4

#define EVENT_CTRL            0x0001
#define EVENT_SEND            0x0002
#define EVENT_RECEIVE         0x0003
#define EVENT_NOTIFICATION    0x0004
#define EVENT_DISCONNECT      0x0005

#define SEND_DMA_FLG			    0x0001
#define RECV_DMA_FLG			    0x0002

#define USB_PROTOCOL_UCOM     0xFF000000
#define USB_PROTOCOL_UPCC     0x0E020000
#define USB_PROTOCOL_UBOT     0x08065000
#define USB_PROTOCOL_CDROM    USB_PROTOCOL_UBOT

#define UDB_INVALID_INFNO			0xFFFFFFFF


typedef uint32 (* USB_TANSFER_COMPLETE) (uint8 event,uint8 *buf,uint32 size);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
void* UsbDevice_Create(struct usb_gadget_driver *driver,USB_GADGET_T *gadget);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
void UsbDevice_Delete(USB_GADGET_T *gadget);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
void UsbDevice_Start_send(uint32 interface,uint32 *buffer,uint32 length);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
int32 UsbDevice_Start_receive(uint32 interface,uint32 *buffer,uint32 length);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
int32 UsbDevice_Ctrl_Transfer(uint32 *buffer,uint32 length);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
uint32 UsbDevice_StopTransfer(uint8 interface);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
uint32 UsbDevice_OpenInterface(uint32 protocol,uint32 callback,uint32 mode);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         jiayong.yang 
//	Note:           
/*****************************************************************************/
#endif
// End 


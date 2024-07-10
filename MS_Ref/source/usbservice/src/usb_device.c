#include "gadget.h"
#include "usb_ch9.h"
#include "sci_log.h"
#include "usb_drv.h"
#include "ref_param.h"
#include "usbservice_api.h"
#include "usblist.h"
#include "sc_reg.h"
#include "usb_device.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

typedef struct USB_Interface
{
    uint32          protocol;
    USB_REQUEST_T *outreq;    // For bulk out responses
    USB_REQUEST_T   *inreq;     // For bulk in responses
    USB_REQUEST_T   *intreq;        // For bulk in responses

    USB_EP_T        *ep_in;
    USB_EP_T        *ep_out;
    USB_EP_T        *ep_int;
    USB_TANSFER_COMPLETE transfer_complete;
} USB_Interface_T;

typedef struct USB_Dev
{
    /* lock protects: state, all the req_busy's, and cbbuf_cmnd */
    USB_EP_T        *ep0;       // Handy copy of gadget->ep0
    USB_REQUEST_T   *ep0req;    // For control responses
    uint32          intf_count;
    USB_Interface_T intf[MAX_INTERFACE_NUM];
} USB_Dev_T;

uint32 interface_mode[MAX_INTERFACE_NUM*2] = {0};
static USB_Dev_T usb_device;

/*****************************************************************************/
//  Description:    call back function of transfer of speicified endpoint
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void UsbDevice_TransferComplete (USB_EP_T *ep,USB_REQUEST_T *req)
{
    uint8 interface_index  = ep->interface_no;

    if (ep==usb_device.ep0)
    {
        usb_device.intf[interface_index].transfer_complete ( (interface_index <<4) |EVENT_CTRL,req->buf,req->length);
    }

    if (usb_device.intf[interface_index].ep_int == ep)
    {
        usb_device.intf[interface_index].transfer_complete ( (interface_index <<4) |EVENT_NOTIFICATION,req->buf,req->length);
    }
    else if (usb_device.intf[interface_index].ep_in == ep)
    {
        usb_device.intf[interface_index].transfer_complete ( (interface_index <<4) |EVENT_SEND,req->buf,req->length);
    }
    else if (usb_device.intf[interface_index].ep_out == ep)
    {
        usb_device.intf[interface_index].transfer_complete ( (interface_index <<4) |EVENT_RECEIVE,req->buf,req->actual);
    }
}

/*****************************************************************************/
//  Description:    Open interface by protocol
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
uint32 UsbDevice_OpenInterface (uint32 protocol,uint32 callback,uint32 mode)
{
    uint32 i;

    for (i=0; i<MAX_INTERFACE_NUM; i++)
    {
        if (usb_device.intf[i].protocol == protocol)
        {
            usb_device.intf[i].transfer_complete = (USB_TANSFER_COMPLETE) callback;
            interface_mode[i] = mode;
            return i;
        }
    }

    return UDB_INVALID_INFNO;
}
/*****************************************************************************/
//  Description:    release the transfer request of the specified interface
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
uint32 UsbDevice_StopTransfer (uint8 interface)
{
    if (usb_device.intf[interface].ep_in)
    {
        ;//usb_ep_dequeue(usb_device.intf[interface].ep_in,usb_device.intf[interface].inreq);
    }
    else
    {
        ;//usb_device.intf[interface].transfer_complete((interface <<4)|EVENT_SEND,NULL,0);
    }

    if (usb_device.intf[interface].ep_out)
    {
        usb_ep_dequeue (usb_device.intf[interface].ep_out,usb_device.intf[interface].outreq);
    }
    else
    {
        usb_device.intf[interface].transfer_complete ( (interface <<4) |EVENT_RECEIVE,NULL,0);
    }

    return 0;
}
/*****************************************************************************/
//  Description:    start control transfer .
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
int32 UsbDevice_Ctrl_Transfer (uint32 *buffer,uint32 length)
{
    if (!length||!buffer)
    {
        return 0;
    }

    usb_device.ep0req->length = length;
    usb_device.ep0req->buf = buffer;
    usb_device.ep0req->complete = UsbDevice_TransferComplete;
    usb_ep_queue (usb_device.ep0, usb_device.ep0req);
    return 0;
}
/*****************************************************************************/
//  Description:   start out transfer of the interface.
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
int32 UsbDevice_Start_receive (uint32 interface,uint32 *buffer,uint32 length)
{
    if (usb_device.intf[interface].outreq->status==0)
    {
        usb_device.intf[interface].outreq->dma = interface_mode[interface]&RECV_DMA_FLG;
        usb_device.intf[interface].outreq->buf = buffer;
        usb_device.intf[interface].outreq->length = length;
        usb_device.intf[interface].outreq->actual = 0;
        usb_device.intf[interface].outreq->complete = UsbDevice_TransferComplete;
        usb_ep_queue (usb_device.intf[interface].ep_out,usb_device.intf[interface].outreq);
        return 1;
    }

    return 0;
}

/*****************************************************************************/
//  Description:    start in transfer of the interface.
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
void UsbDevice_Start_send (uint32 interface,uint32 *buffer,uint32 length)
{
    if (usb_device.intf[interface].inreq->status==0)
    {

        usb_device.intf[interface].inreq->dma = interface_mode[interface]&SEND_DMA_FLG;
        usb_device.intf[interface].inreq->buf = buffer;
        usb_device.intf[interface].inreq->length = length;
        usb_device.intf[interface].inreq->complete = UsbDevice_TransferComplete;
        usb_ep_queue (usb_device.intf[interface].ep_in,usb_device.intf[interface].inreq);
    }
}

/*****************************************************************************/
//  Description:    create the usb device
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
void *UsbDevice_Create (struct usb_gadget_driver *driver,USB_GADGET_T *gadget)
{
    uint32 interface_index = 0;
    USB_COMMON_DESCRIPTOR_T *head;
    USB_INTERFACE_DESCRIPTOR_T *inf;
    USB_ENDPOINT_DESCRIPTOR_T *ep;
    uint8 *usb_config_ptr= (uint8 *) 1;
    uint16  length=0;
    uint16  offset=0;
    uint32 protocol;
    uint8 index=0,i;

    if (!driver||!driver->get_config_descriptor)
    {
        return NULL;
    }

    driver->get_config_descriptor (&usb_config_ptr);

    if (usb_config_ptr==NULL)
    {
        return NULL;
    }

    length= (usb_config_ptr[3]<<8) |usb_config_ptr[2];
    SCI_MEMSET (&usb_device,0,sizeof (usb_device));
    usb_device.ep0 = gadget->ep0;
    usb_device.ep0req  = usb_ep_alloc_request (usb_device.ep0);

    while (offset < length)
    {
        head = (USB_COMMON_DESCRIPTOR_T *) (usb_config_ptr + offset);

        if (head->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE)
        {
            inf = (USB_INTERFACE_DESCRIPTOR_T *) head;
            protocol = (inf->bInterfaceClass<<24) | (inf->bInterfaceSubClass<<16) | (inf->bInterfaceProtocol<<8);

            if (usb_device.intf[interface_index].protocol!=0)
            {
                if (usb_device.intf[interface_index].ep_out||usb_device.intf[interface_index].ep_in)
                {
                    interface_index++;
                }
            }

            index=0;

            for (i=0; i<interface_index; i++)
            {
                if ( (usb_device.intf[i].protocol&0xffffff00) == protocol)
                {
                    index++;
                }
            }

            protocol |= index;
            usb_device.intf[interface_index].protocol = protocol;
        }

        if (head->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE)
        {
            ep = (USB_ENDPOINT_DESCRIPTOR_T *) head;

            if (ep->bmAttributes == 2)
            {
                if ( (ep->bEndpointAddress & 0x80) ==0)
                {
                    usb_device.intf[interface_index].ep_out = usb_gadget_alloc_ep(gadget, ep->bEndpointAddress);
                    usb_device.intf[interface_index].ep_out->interface_no =interface_index;
                    usb_device.intf[interface_index].outreq = usb_ep_alloc_request (usb_device.intf[interface_index].ep_out);

                    //usb_device.intf[interface_index].outreq->complete = interface_complete[interface_index];
                    if (usb_device.intf[interface_index].outreq == NULL)
                    {
                        goto failed;
                    }
                }
                else
                {
                    usb_device.intf[interface_index].ep_in = usb_gadget_alloc_ep(gadget, ep->bEndpointAddress);
                    usb_device.intf[interface_index].ep_in->interface_no =interface_index;
                    usb_device.intf[interface_index].inreq = usb_ep_alloc_request (usb_device.intf[interface_index].ep_in);

                    if (usb_device.intf[interface_index].inreq == NULL)
                    {
                        goto failed;
                    }
                }

                if (usb_device.intf[interface_index].ep_int!=NULL)
                {
                    usb_device.intf[interface_index].ep_int->interface_no =interface_index;
                }
            }

            if (ep->bmAttributes==3)
            {
                usb_device.intf[interface_index].ep_int = usb_gadget_alloc_ep(gadget, ep->bEndpointAddress);
                usb_device.intf[interface_index].intreq = usb_ep_alloc_request (usb_device.intf[interface_index].ep_int);

                if (usb_device.intf[interface_index].intreq == NULL)
                {
                    goto failed;
                }
            }

        }

        offset += head->bLength;
    }

    usb_device.intf_count = interface_index;
    return &usb_device;
failed:
    UsbDevice_Delete (gadget);
    return NULL;
}
/*****************************************************************************/
//  Description:    release the usb device
//  Global resource dependence:
//  Author:         jiayong.yang
//  Note:
/*****************************************************************************/
void UsbDevice_Delete (USB_GADGET_T *gadget)
{
    uint8 i;

    if (usb_device.ep0req)
    {
        usb_ep_free_request (usb_device.ep0,usb_device.ep0req);
    }

    for (i=0; i< MAX_INTERFACE_NUM; i++)
    {
        if (usb_device.intf[i].outreq)
        {
            usb_ep_free_request (usb_device.intf[i].ep_out,usb_device.intf[i].outreq);
        }

        if (usb_device.intf[i].inreq)
        {
            usb_ep_free_request (usb_device.intf[i].ep_in,usb_device.intf[i].inreq);
        }

        if (usb_device.intf[i].intreq)
        {
            usb_ep_free_request (usb_device.intf[i].ep_int,usb_device.intf[i].intreq);
        }
    }

    SCI_MEMSET (&usb_device,0,sizeof (usb_device));
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

/******************************************************************************
 ** File Name:    UPM_main_v3.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/

#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "usb20_app_config.h"
#include "chip_plf_export.h"
#include "ucom_common.h"
#include "upcc_api.h"

extern uint32 UBOT_GetCOnfigDescriptor (uint8 **para);
extern int VCOM_GetDevDescriptor(void *setup_, void *buf, unsigned size);
extern void VCOM_command(void *setup);
extern uint32 VCOM_GetCOnfigDescriptor (uint8 **para);
extern void VCOM_SetConfig(uint8 cfgnum);
#ifdef USB_MTP_SUPPORT
extern uint32 MTP_GetConfigDescriptor (uint8 **para);
#endif

USB_APP_OP_T usb_app_op[] =
{
    {NULL,NULL,NULL,NULL},
    {NULL,NULL,UCOM_GetCOnfigDescriptor,NULL},
    {NULL,NULL,UBOT_GetCOnfigDescriptor,NULL},
    {VCOM_GetDevDescriptor,VCOM_command,VCOM_GetCOnfigDescriptor,VCOM_SetConfig},
#ifdef USB_WEBCAMERA_SUPPORT
    {NULL,NULL,UPCC_GetCOnfigDescriptor,NULL},
#else
    {NULL,NULL,NULL,NULL},
#endif
#ifdef USB_MTP_SUPPORT
    {NULL,NULL,MTP_GetConfigDescriptor,NULL},
#else
    {NULL,NULL,NULL,NULL},
#endif
};

LOCAL USB_APP_E g_app_type = USB_APP_NONE;

static USB_EPCfg_T USB_EP_CFG[12] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

PUBLIC void USB_SetService(USB_APP_E app_type,void *para)
{
    if (app_type >= USB_APP_MAX)
    {
        return;
    }

    g_app_type = app_type;

}

PUBLIC USB_APP_OP_T *USB_GetCurrentAppOperation()
{
    if (g_app_type == USB_APP_NONE)
    {
        return NULL;
    }

    return &usb_app_op[g_app_type];
}

PUBLIC USB_EPCfg_T *USB_GetEPCFG()
{
    return &USB_EP_CFG[0];
}

void InsertUSBEpCfg(USB_ENDPOINT_DESCRIPTOR_T *ep)
{
    uint8 i;

    for (i=0; i<sizeof (USB_EP_CFG) /sizeof (USB_EP_CFG[0]); i++)
    {
        if (USB_EP_CFG[i].ep_address==0)
        {
            USB_EP_CFG[i].ep_address = ep->bEndpointAddress;
            USB_EP_CFG[i].ep_type = ep->bmAttributes&0x03;
            USB_EP_CFG[i].mps = cpu_to_le16 (ep->wMaxPacketSize);
            break;
        }
        else if (USB_EP_CFG[i].ep_address==ep->bEndpointAddress)
        {
            break;
        }
    }
}

void usb_parse_configuration (uint8 IsFS)
{
    uint8  *usb_config_ptr ;
    USB_COMMON_DESCRIPTOR_T *head;
    USB_ENDPOINT_DESCRIPTOR_T *ep;
    uint32  length=0;
    uint32  offset=0;

    usb_config_ptr = (uint8 *) IsFS;

    if (usb_app_op[g_app_type].get_app_configure_descriptor!=NULL)
    {
        length = usb_app_op[g_app_type].get_app_configure_descriptor (&usb_config_ptr);
    }

    offset = 0;
    SCI_MEMSET (USB_EP_CFG,0,sizeof (USB_EP_CFG));

    while (offset < length)
    {
        head = (USB_COMMON_DESCRIPTOR_T *) (usb_config_ptr + offset);

        if (head->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE)
        {
            ep = (USB_ENDPOINT_DESCRIPTOR_T *) head;
            InsertUSBEpCfg (ep);
        }

        offset += head->bLength;
    }
}

uint32 usb_GetOutEPMask (uint8 type)
{
    USB_EPCfg_T *ep_cfg_ptr;
    uint32  mask;

    mask = 0;

    ep_cfg_ptr = USB_GetEPCFG();

    if (ep_cfg_ptr == NULL)
    {
        return mask;
    }

    while (ep_cfg_ptr->ep_address)
    {
        if ( (ep_cfg_ptr->ep_address & 0x80) && (type==1))
        {
            mask |= (BIT_0<< (ep_cfg_ptr->ep_address&0x7f));
        }

        if ( ( (ep_cfg_ptr->ep_address & 0x80) ==0) && (type==0))
        {
            mask |= (BIT_0<< ( (ep_cfg_ptr->ep_address&0x7f) +16));
        }

        ep_cfg_ptr++;
    }

    return mask;
}

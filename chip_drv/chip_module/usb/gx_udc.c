/******************************************************************************
** File Name:    usb_drv_e.c                                                 *
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
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
/******************************************************************************
** File Name:    usb_drv_e.c                                                 *
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
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "sx_udc.h"
#include "udc_api.h"
#include "ldo_drvapi.h"
#include "task_monitor.h"
#include "usb20_app_config.h"
#include "usb20_drvapi.h"
#include "gpio_prod_api.h"
#include "usb_device.h"
#include "usb_phy.h"
#include "usb_ch9.h"
#include "cpu_endian.h"

#include "analog_reg.h"
#include "sys_ctrl.h"
#include "udc_internal.h"

// #define _DEBUG_USB20_

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#endif

INPUT_BUFFER_INIT (int32,64)

extern BLOCK_ID usb_isr_id;

LOCAL const char gadget_name[] = "gx_udc";

#ifdef WINUSB_SUPPORT
LOCAL	uint8	s_charge_product_str[] = "Unisoc-Charging Mode";
#endif
LOCAL struct sx_udc *the_controller;
LOCAL uint32    gx_usb_suspend_flag=0;
LOCAL uint32    s_setup_packet_backup[2]={0}; // backup the last setup message
LOCAL uint32    s_nak_enable=0;

void *g_ppp_data_buf = NULL;
uint32 g_ppp_send_status = 0;

#define CTRL_BUF_LEN (128 * 4)
#pragma arm section zidata = "USB_DATA_IN_IRAM"
LOCAL volatile __attribute__((aligned(32))) uint32 s_setup_packet[CTRL_BUF_LEN / 4]= {0};
#pragma arm section zidata

#ifdef _DEBUG_USB20_
#define REGISTER_MAP_LEN (0xC00)
LOCAL uint32 usb_register_map[REGISTER_MAP_LEN>>2]= {0};
#endif

LOCAL void gx_udc_init (void);

void dump_usb_registers (void)
{
#ifdef _DEBUG_USB20_
    uint32 i;
    uint32 *src,*dest;

    src = (uint32 *) USB_REG_BASE;
    dest = (uint32 *) usb_register_map;

    for (i=0; i<REGISTER_MAP_LEN/4; i++)
    {
        *dest++ = *src++;
    }

#endif
}

LOCAL void udc_pullup (enum sx_udc_cmd_e cmd)
{
#if !defined(PLATFORM_UWS6121E)
    switch (cmd)
    {
        case SX_UDC_P_ENABLE :
            break;

        case SX_UDC_P_DISABLE :
            break;

        case SX_UDC_P_RESET :
            break;

        default:
            break;
    }
#endif
}

LOCAL struct sx_udc memory;

LOCAL void gx_udc_SetHostReq (uint32 state)
{
    the_controller->hostreq = state;
}

LOCAL uint32 gx_udc_GetHostReq()
{
    return the_controller->hostreq;
}

LOCAL void gx_udc_SetConnectState (uint32 state)
{
    the_controller->connected = state;
}

LOCAL uint32 gx_udc_GetConnectState (void)
{
    return the_controller->connected;
}

LOCAL uint32 gx_udc_USB_Connect()
{
    if (FALSE == gx_udc_GetConnectState())
    {
        gx_udc_SetConnectState (TRUE);
    }

    return 0;
}

LOCAL uint32 gx_udc_USB_Disconnect()
{
    if (TRUE == gx_udc_GetConnectState())
    {
        gx_udc_SetConnectState (FALSE);
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function is called when usb protctol error occurs
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           value: usb error type
/*****************************************************************************/

extern void mon_Event(uint32 evt);
static void gx_udc_USB_DisableIrq (void);
static void gx_udc_USB_EnableIrq (void);
/*------------------------- I/O ----------------------------------*/

unsigned udc_ctrl_buf_size_()
{
    return (CTRL_BUF_LEN);
}

void *udc_ctrl_buffer_()
{
    return (void *)s_setup_packet;
}

static void _udc_ep0_change_state(struct sx_udc *udc, uint8 state)
{
    udc->ep0state = state;
}

static struct sx_ep *_udc_get_ep(struct sx_udc *udc, uint8 address)
{
    uint8 num = (address & 0x7f);
    if (num == 0) {
        return &udc->ep0;
    } else {
        if (address & 0x80)
            return &udc->iep[num - 1];
        else
            return &udc->oep[num - 1];
    }
    return NULL;
}

void udc_hw_stop_xfer_(uint8 ep_addr)
{
    BOOLEAN dirin = (ep_addr & 0x80) ? TRUE : FALSE;
    uint8 num = ep_addr & 0x7f;

    if (dirin) {
        USB_DIEPCTL_U epctl;
        epctl.dwValue = REG_R(USB_DIEPCTL(num));
        if (!epctl.mBits.ep_enable)
            return;

        epctl.mBits.ep_disable = 1;
        epctl.mBits.set_nak = 1;
        REG_W(USB_DIEPCTL(num), epctl.dwValue);

        REG_WAIT_SET(USB_DIEPINT(num), BIT_1, 1000); // epdis
        REG_W(USB_DIEPINT(num), BIT_1);
    } else {
        USB_DOEPCTL_U epctl;
        epctl.dwValue = REG_R(USB_DOEPCTL(num));
        if (!epctl.mBits.ep_enable)
            return;

        epctl.mBits.ep_disable = 1;
        epctl.mBits.set_nak = 1;
        REG_W(USB_DOEPCTL(num), epctl.dwValue);

        REG_WAIT_SET(USB_DOEPINT(num), BIT_1, 1000); // epdis
        REG_W(USB_DIEPINT(num), BIT_1);
    }
}

static void _hw_ep_disable(uint8 ep);

void udc_hw_start_xfer_(uint8 ep_addr, uint16 mps, void *buffer, uint32 size)
{
    BOOLEAN dirin = (ep_addr & 0x80) ? TRUE : FALSE;
    uint8 num = ep_addr & 0x7f;
    uint32 epctl_v;
    uint32 eptsiz_v;
    uint16 npkt = 1;
    if (size != 0) {
        npkt = (size + mps - 1) / mps;
    }

    if (dirin) {
        if (num == 0) {
            USB_DIEPCTL0_U diepctl0;
            USB_DIEPTSIZ0_U dieptsiz0;

            dieptsiz0.dwValue = 0;
            dieptsiz0.mBits.xfersize = size;
            dieptsiz0.mBits.pktcnt = npkt;
            eptsiz_v = dieptsiz0.dwValue;

            diepctl0.dwValue = REG_R(USB_DIEP0CTL);
            diepctl0.mBits.cnak = 1;
            diepctl0.mBits.epena = 1;
            diepctl0.mBits.txfnum = 0;
            epctl_v = diepctl0.dwValue;
        } else {
            USB_DIEPCTL_U diepctl;
            USB_DIEPTSIZ_U dieptsiz;

            dieptsiz.dwValue = 0;
            dieptsiz.mBits.transfer_size = size;
            dieptsiz.mBits.packet_count = npkt;
            dieptsiz.mBits.mult_count = 1;
            eptsiz_v = dieptsiz.dwValue;

            diepctl.dwValue = REG_R(USB_DIEPCTL(num));
            diepctl.mBits.clear_nak = 1;
            diepctl.mBits.ep_enable = 1;
            diepctl.mBits.tx_fifo_number = num;
            epctl_v = diepctl.dwValue;
        }

        REG_W(USB_DIEPTSIZ(num), eptsiz_v);
        if (size != 0) {
            MMU_DmaCacheSync((uint32)buffer, size, DMABUFFER_TO_DEVICE);
            REG_W(USB_DIEPDMA(num), (uint32)buffer);
        }
        REG_W(USB_DIEPCTL(num), epctl_v);
    } else {
        if (num == 0) {
            USB_DOEPCTL0_U doepctl0;
            USB_DOEPTSIZ0_U doeptsiz0;

            doeptsiz0.dwValue = 0;
            doeptsiz0.mBits.xfersize = size;
            doeptsiz0.mBits.pktcnt = 1;
            eptsiz_v = doeptsiz0.dwValue;

            doepctl0.dwValue = REG_R(USB_DOEP0CTL);
            doepctl0.mBits.cnak = 1;
            doepctl0.mBits.epena = 1;
            epctl_v = doepctl0.dwValue;
        } else {
            USB_DOEPCTL_U doepctl;
            USB_DOEPTSIZ_U doeptsiz;

            doeptsiz.dwValue = 0;
            doeptsiz.mBits.transfer_size = size;
            doeptsiz.mBits.packet_count = npkt;
            eptsiz_v = doeptsiz.dwValue;

            doepctl.dwValue = REG_R(USB_DOEPCTL(num));
            doepctl.mBits.clear_nak = 1;
            doepctl.mBits.ep_enable = 1;
            epctl_v = doepctl.dwValue;
        }

        REG_W(USB_DOEPTSIZ(num), eptsiz_v);
        if (size != 0) {
            MMU_DmaCacheSync((uint32)buffer, size, DMABUFFER_FROM_DEVICE);
            REG_W(USB_DOEPDMA(num), (uint32)buffer);
        }
        REG_W(USB_DOEPCTL(num), epctl_v);
    }
}

static void _udc_continue_schedule(struct sx_ep *ep)
{
    if (!list_empty(&ep->queue) && ep->cur_req == NULL) {
        SX_REQUEST_T *req = list_entry(ep->queue.next, struct sx_request, queue);
//mon_Event(0xee440000|3);
        list_del_init (&req->queue);
        ep->cur_req = req;
        if (ep->ep.address & 0x7f) {
            udc_hw_start_xfer_(ep->ep.address, ep->ep.maxpacket, req->req.buf, req->req.length);
        }
    }
}

static void gx_udc_done(struct sx_ep *ep, struct sx_request *req, int status)
{
    if (ep->cur_req == req) {
        ep->cur_req = NULL;
    }

    _udc_continue_schedule(ep);

    if (req) {
        req->req.status = REQ_IS_IDLE;
        if (req->req.complete)
            req->req.complete (&ep->ep, &req->req);
    }
}

static int _udc_get_status(struct sx_udc *udc, struct usb_ctrlrequest *setup)
{
    uint8 *ctrlbuf;
    uint16 reply;

    if (setup->wLength != 2)
        return SCI_ERROR;

    switch (setup->bRequestType & USB_RECIP_MASK) {
    case USB_RECIP_DEVICE:
        /*
         * bit 0 => self powered
         * bit 1 => remote wakeup
         */
        reply = cpu_to_le16(1);
        break;

    case USB_RECIP_INTERFACE:
        reply = 0;
        break;

    case USB_RECIP_ENDPOINT:
    {
        uint8 ep_address = le16_to_cpu(setup->wIndex);
        struct sx_ep *ep = _udc_get_ep(udc, ep_address);
        reply = 0;
        if (ep) {
            reply = cpu_to_le16(ep->halted ? 1 : 0);
        }
    }
        break;
    default:
        return SCI_ERROR;
        break;
    }

    ctrlbuf = (uint8 *)udc_ctrl_buffer_();
    memcpy(ctrlbuf, &reply, 2);
    udc_hw_start_xfer_(0x80, 64, ctrlbuf, 2);

    return 0;
}

static void _udc_set_test_mode(uint8 test_mode)
{
    USB_DCTL_U dctl;
    dctl.dwValue = REG_R(USB_DCTL);
    switch (test_mode)
    {
    case 1: // TEST_J
    case 2: // TEST_K
    case 3: // TEST_SE0_NAK
    case 4: // TEST_PACKET
    case 5: // TEST_FORCE_EN
        dctl.mBits.tstctl = test_mode;
        REG_OR(USB_DCTL, dctl.dwValue);
        break;

    default:
        break;
    }
}

static void _udc_halt_ep(uint8 ep_, uint8 halt)
{
    BOOLEAN dirin = (ep_ & 0x80) ? TRUE : FALSE;
    uint8 num = ep_ & 0x7f;
    USB_DIEPCTL_U epctl;
    uint8 type;
    struct sx_ep *ep = _udc_get_ep(the_controller, ep_);

    if (num == 0) {
        if (halt) { // can not clear ep0
            USB_DIEPCTL0_U epctl;
            epctl.dwValue = 0;
            epctl.mBits.stall = 1;
            epctl.mBits.cnak = 1;

            if (dirin) {
                REG_OR(USB_DIEPCTL(0), epctl.dwValue);
            } else {
                REG_OR(USB_DOEPCTL(0), epctl.dwValue);
            }
        }
        return;
    }

    epctl.dwValue = REG_R(dirin ? USB_DIEPCTL(num) : USB_DOEPCTL(num));
    type = epctl.mBits.ep_type;
    if (type == USB_ENDPOINT_XFER_ISOC)
        return;

    if (dirin) {
        if (halt) {
            epctl.mBits.stall_handshak = 1;
            epctl.mBits.set_nak = 1;
            if (epctl.mBits.ep_enable)
                epctl.mBits.ep_disable = 1;
        } else {
            epctl.mBits.stall_handshak = 0;
            if (type == USB_ENDPOINT_XFER_BULK || type == USB_ENDPOINT_XFER_INT)
                epctl.mBits.set_data0_pid = 1;
        }
        REG_W(USB_DIEPCTL(num), epctl.dwValue);
    } else {
        USB_DOEPCTL_U doepctl;
        doepctl.dwValue = REG_R(USB_DOEPCTL(num));
        if (halt) {
            doepctl.mBits.stall_handshak = 1;
        } else {
            doepctl.mBits.stall_handshak = 0;
            if (type == USB_ENDPOINT_XFER_BULK || type == USB_ENDPOINT_XFER_INT)
                doepctl.mBits.set_data0_pid = 1;
        }
        REG_W(USB_DOEPCTL(num), doepctl.dwValue);
    }

    ep->halted = halt ? 1 : 0;
}

static int _udc_ctrl_feature(struct usb_ctrlrequest *ctrl)
{
    struct sx_udc       *dev = &memory;

    BOOLEAN set = ctrl->bRequest == USB_REQ_SET_FEATURE;
    uint8 recip = (ctrl->bRequestType & USB_RECIP_MASK);
    uint16 value = ctrl->wValue;
    uint16 index = ctrl->wIndex;
    uint16 length = ctrl->wLength;

    TM_SendTestPointRequest((0xBBDD0000|(value)),(index << 16)|(length));
    
    switch (recip)
    {
    case USB_RECIP_DEVICE:
        switch (value)
        {
        case USB_DEVICE_TEST_MODE:
            if ((index & 0xff) != 0)
                return -1;
            if (!set)
                return -1;
            if (length != 0)
                return -1;

            dev->test_mode = (index >> 8);
            return 1;

        default:
            return -1;
        }
        break;

    case USB_RECIP_ENDPOINT:
        _hw_ep_disable(index & 0xff);
        return 0;

    default:
        return -1;
    }

    return -1;
}

/*****************************************************************************/
//  Description:   process setup transfer.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
static void usb_standard_command (struct usb_ctrlrequest *setup)
{
    struct sx_udc *dev = the_controller;

    switch (setup->bRequest)
    {
        case USB_REQ_SET_ADDRESS:
            udc_hw_set_address_(setup->wValue & 0xff);
            break;

        case USB_REQ_SET_CONFIGURATION:
            if (dev->driver && dev->driver->setup)
                dev->driver->setup(&dev->gadget, setup);
            gx_usb_suspend_flag |= BIT_1;
            break;

        case USB_REQ_SET_FEATURE:
        case USB_REQ_CLEAR_FEATURE:
            _udc_ctrl_feature(setup);
            break;

        case USB_REQ_GET_STATUS:
            _udc_get_status(dev, setup);
            break;

        default:
            if (dev->driver && dev->driver->setup)
                dev->driver->setup(&dev->gadget, setup);
            break;
    }
}

void udc_hw_ep_enable_(uint8 ep, int type, uint16 mps)
{
    BOOLEAN dirin = ep & 0x80;
    uint8 num = ep & 0x7f;
    uint32 epctl_v;
    if (num == 0) {
        switch (mps) {
        case 64:
            mps = 0;
            break;

        case 32:
            mps = 1;
            break;

        case 16:
            mps = 2;
            break;

        case 8:
        default:
            mps = 3;
            break;
        }
    }

    if (dirin) {
        if (num == 0) {
            USB_DIEPCTL0_U diepctl0;
            diepctl0.dwValue = REG_R(USB_DIEP0CTL);
            diepctl0.mBits.mps = mps;
            diepctl0.mBits.eptype = 0;
            diepctl0.mBits.snak = 1;
            epctl_v = diepctl0.dwValue;
        } else {
            USB_DIEPCTL_U diepctl;
            diepctl.dwValue = REG_R(USB_DIEPCTL(num));
            diepctl.mBits.mps = mps;
            diepctl.mBits.ep_type = type;
            diepctl.mBits.active_ep = 1;
            diepctl.mBits.set_nak = 1;
            epctl_v = diepctl.dwValue;
        }
        REG_W(USB_DIEPCTL(num), epctl_v);
    } else {
        if (num == 0) {
            USB_DOEPCTL0_U doepctl0;
            doepctl0.dwValue = REG_R(USB_DOEP0CTL);
            doepctl0.mBits.mps = mps;
            doepctl0.mBits.eptype = 0;
            doepctl0.mBits.snak = 1;
            epctl_v = doepctl0.dwValue;
        } else {
            USB_DOEPCTL_U doepctl;
            doepctl.dwValue = REG_R(USB_DOEPCTL(num));
            doepctl.mBits.mps = mps;
            doepctl.mBits.ep_type = type;
            doepctl.mBits.active_ep = 1;
            doepctl.mBits.set_nak = 1;
            epctl_v = doepctl.dwValue;
        }
        REG_W(USB_DOEPCTL(num), epctl_v);
    }
}

static void _rx_fifo_flush()
{
    USB_GRSTCTL_U grstctl;
    grstctl.dwValue = 0;
    grstctl.mBits.rxfflush = 1;
    REG_OR(USB_GRSTCTL, grstctl.dwValue);
}

static void _tx_fifo_flush(uint8 num)
{
    USB_GRSTCTL_U grstctl;
    grstctl.dwValue = 0;
    grstctl.mBits.txfnum = num;
    grstctl.mBits.txfflush = 1;
    REG_OR(USB_GRSTCTL, grstctl.dwValue);
}

static void _tx_fifo_flush_all()
{
    _tx_fifo_flush(0x10);
}

static void _hw_ep_disable(uint8 ep)
{
    BOOLEAN dirin = ep & 0x80;
    uint8 num = ep & 0x7f;
    USB_DCTL_U dctl;
    volatile uint32 timeout;

    dctl.dwValue = 0;
    if (dirin) {
        USB_DIEPCTL_U epctl;
        epctl.dwValue = REG_R(USB_DIEPCTL(num));
        if (epctl.mBits.ep_enable) {
            udc_hw_stop_xfer_(ep);
        }

        epctl.mBits.active_ep = 0;
        epctl.mBits.ep_enable = 0;
        epctl.mBits.set_nak = 1;
        REG_W(USB_DIEPCTL(num), epctl.dwValue);
    } else {
        USB_DOEPCTL_U epctl;
        epctl.dwValue = REG_R(USB_DOEPCTL(num));
        if (!epctl.mBits.ep_enable) {
            udc_hw_stop_xfer_(ep);
        }

        epctl.mBits.active_ep = 0;
        epctl.mBits.ep_enable = 0;
        epctl.mBits.set_nak = 1;
        REG_W(USB_DOEPCTL(num), epctl.dwValue);

        dctl.mBits.sgoutnak = 1;
        REG_OR(USB_DCTL, dctl.dwValue);
    }
}

static void udc_request_setup()
{
    void *setup = udc_ctrl_buffer_();
    _udc_ep0_change_state(the_controller, EP0_SETUP);
    udc_hw_start_xfer_(0, 64, setup, 8);
}

static void udc_enum_done()
{
    uint32 oep_msk, iep_msk;
    USB_DCTL_U dctl;
    USB_DSTS_U dsts;
    USB_EPCfg_T *ep_cfg;

    dsts.dwValue = REG_R(USB_DSTS);
    usb_parse_configuration(dsts.mBits.enumspd);
    ep_cfg = USB_GetEPCFG();

    udc_hw_ep_enable_(0, USB_EP_TYPE_CONTROL, 64);
    udc_hw_ep_enable_(0x80, USB_EP_TYPE_CONTROL, 64);

    while (ep_cfg && ep_cfg->ep_address) {
        udc_hw_ep_enable_(ep_cfg->ep_address, (USB_EP_TYPE_E)ep_cfg->ep_type, ep_cfg->mps);
        ep_cfg++;
    }

    oep_msk = usb_GetOutEPMask(1);
    iep_msk = usb_GetOutEPMask(0);
    REG_OR(USB_DAINTMSK, 0x10001 | oep_msk | iep_msk);

    udc_request_setup();

    dctl.dwValue = 0;
    dctl.mBits.cgoutnak = 1;
    dctl.mBits.cgnpinnak = 1;
    REG_OR(USB_DCTL, dctl.dwValue);
}

void udc_hw_set_address_(uint8 addr)
{
    USB_DCFG_U dcfg;
    dcfg.dwValue = REG_R(USB_DCFG);
    dcfg.mBits.devaddr = addr;
    REG_W(USB_DCFG, dcfg.dwValue);
}

void udc_hw_reset_()
{
    USB_DCTL_U dctl;

    dctl.dwValue = 0;
    dctl.mBits.tstctl = 1;
    REG_AND(USB_DCTL, ~(dctl.dwValue));

    _tx_fifo_flush_all();
    _rx_fifo_flush();

    _hw_ep_disable(0);
    _hw_ep_disable(0x80);

    udc_hw_set_address_(0);
}

static void udc_reset_handle()
{
    USB_EPCfg_T *ep_cfg = USB_GetEPCFG();
    struct sx_ep *ep = _udc_get_ep(the_controller, 0);
    struct sx_request *req;
    struct sx_udc *udc = the_controller;

    udc_hw_reset_();

    udc->test_mode = 0;
    ep->halted = 0;
    ep->cur_req = NULL;
    list_del_init(&ep->queue);

    while (ep_cfg && ep_cfg->ep_address) {
        ep = _udc_get_ep(udc, ep_cfg->ep_address);
        _hw_ep_disable(ep_cfg->ep_address);
        ep->halted = 0;
        ep->cur_req = NULL;
        list_del_init(&ep->queue);
        ep_cfg++;
    }
}

static void usb_setup_handle()
{
    struct sx_udc *dev = the_controller;
    struct usb_ctrlrequest *ctrl_req = (struct usb_ctrlrequest *)udc_ctrl_buffer_();
    struct usb_ctrlrequest setup;

    s_setup_packet_backup[0] = ((uint32*)ctrl_req)[0];
    s_setup_packet_backup[1] = ((uint32*)ctrl_req)[1];

    setup.bRequestType = ctrl_req->bRequestType;
    setup.bRequest = ctrl_req->bRequest;
    setup.wLength = le16_to_cpu(ctrl_req->wLength);
    setup.wIndex = le16_to_cpu(ctrl_req->wIndex);
    setup.wValue = le16_to_cpu(ctrl_req->wValue);

    if (setup.wLength) {
        if (setup.bRequestType & USB_DIR_IN) {
            _udc_ep0_change_state(dev, EP0_DATA_IN);
        } else {
            _udc_ep0_change_state(dev, EP0_DATA_OUT);
        }
    } else {
        _udc_ep0_change_state(dev, EP0_STATUS_IN);
    }

    TM_SendTestPointRequest((0xBBCC0000|((setup.bRequestType & USB_TYPE_MASK) >> 5)), setup.bRequest);

    switch (setup.bRequestType & USB_TYPE_MASK) {
        case USB_TYPE_STANDARD:
            usb_standard_command(&setup);
            break;

        case USB_TYPE_CLASS:
        case USB_TYPE_VENDOR:
            if (dev->driver && dev->driver->setup)
                dev->driver->setup(&dev->gadget, &setup);
            break;

        default:
            break;
    }

    if (dev->ep0state == EP0_STATUS_IN) {
        udc_hw_start_xfer_(0x80, 64, NULL, 0);
    }
}

/*****************************************************************************/
//  Description:    out endpoint6 handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/

LOCAL void usb_EPnOut_handle (uint8 ep_id,uint16 int_status)
{
    volatile USB_DOEPTSIZ_U *doeptsiz_ptr = (USB_DOEPTSIZ_U *) USB_DOEPTSIZ (ep_id);
    struct sx_ep *ep = _udc_get_ep(the_controller, ep_id);
    volatile USB_DOEPINT_U  doepint;
    struct sx_request *req = ep->cur_req;

    doepint.dwValue = int_status;
    TM_SendTestPointRequest ( (0xBBB00000|ep_id), doepint.dwValue);

    if (doepint.mBits.transfer_com)
    {
        if (req != NULL) {
            req->req.actual = req->req.length - doeptsiz_ptr->mBits.transfer_size;
            MMU_DmaCacheSync((uint32)req->req.buf, req->req.actual, DMABUFFER_FROM_DEVICE);
            gx_udc_done(ep, req, SCI_SUCCESS);
        }
    }
    else if (doepint.mBits.nak || doepint.mBits.nyet)
    {
        REG_AND(USB_DOEPMSK, ~BIT_14); // Disable endpoint INT
    }
}

/*****************************************************************************/
//  Description:   out endpoint0 handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_EP0_out_handle (uint8 ep_id,uint16 int_status)
{
    struct sx_ep        *ep = _udc_get_ep(the_controller, 0);
    struct sx_request   *req = ep->cur_req;
    struct sx_udc       *dev = &memory;
    USB_DOEPINT_U       doepint;
    uint32              transfer_size;
    BOOLEAN             setup;

    doepint.dwValue = int_status;

    TM_SendTestPointRequest ( (0xBBBA0000), doepint.dwValue);
    setup = doepint.mBits.timeout_condi ? TRUE : FALSE; // for ep0, timeout_condi -> setup
#if defined(PLATFORM_UWS6121E)
    setup = (setup || (doepint.mBits.stuppktrcvd ? TRUE : FALSE));
#endif
    if (setup)
        doepint.mBits.transfer_com = 0;

    if (dev->ep0state == EP0_SETUP && !doepint.mBits.timeout_condi)
        doepint.mBits.transfer_com = 0;

    if (doepint.mBits.timeout_condi)
    {
        usb_setup_handle();
    }

    if (doepint.mBits.transfer_com)
    {
        *(volatile uint32 *) USB_DOEP0CTL |= BIT_27; // snak
        if (dev->ep0state == EP0_DATA_OUT) {
            _udc_ep0_change_state(dev, EP0_STATUS_IN);
            udc_hw_start_xfer_(0x80, 64, NULL, 0);

            if (dev->test_mode) {
                _udc_set_test_mode(dev->test_mode);
            }
        } else if (dev->ep0state == EP0_STATUS_OUT) {
            if (req != NULL) {
                req->req.actual = req->req.length; // TODO
                gx_udc_done(ep, req, SCI_SUCCESS);
            }
            udc_request_setup();
        } else {
            mon_Event(0xee770000|1);
            mon_Event(dev->ep0state);
        }
    }

    if (doepint.mBits.nak)
    {
        if (dev->ep0state == EP0_DATA_OUT)
        {
            if (list_empty (&ep->queue)) {
                REG_AND(USB_DOEPMSK, ~BIT_13);
            }
        }
    }

    if (doepint.mBits.ahb_error) {
        mon_Event(0xee7700e0|1);
    }
}

/*****************************************************************************/
//  Description:   out endpoint handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void usb_EP_out_handle (uint8 ep_id, uint16 int_status)
{
    if (ep_id == 0)
    {
        usb_EP0_out_handle(ep_id,int_status);
    }
    else
    {
        usb_EPnOut_handle (ep_id,int_status);
    }
}

/*****************************************************************************/
//  Description:   in endpoint handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
static void usb_EP_in_handle (uint8 ep_id, uint16 int_status)
{
    struct sx_ep        *ep = _udc_get_ep(the_controller, (ep_id | 0x80));
    struct sx_udc       *dev = the_controller;
    struct sx_request   *req = ep->cur_req;
    USB_DIEPINT_U  diepint;

    diepint.dwValue = int_status;
    TM_SendTestPointRequest ( (0xBBB10000|ep_id),  diepint.dwValue);
    if (ep_id == 0)
    {
        if (diepint.mBits.transfer_com)
        {
            if (dev->ep0state == EP0_DATA_IN) {
                _udc_ep0_change_state(dev, EP0_STATUS_OUT);
                udc_hw_start_xfer_(0, 64, NULL, 0);
            } else if (dev->ep0state == EP0_STATUS_IN) {
                USB_DIEPTSIZ0_U dieptsiz0;
                dieptsiz0.dwValue = REG_R(USB_DIEPTSIZ(0));
                if (req != NULL) {
                    req->req.actual = req->req.length; // TODO
                    gx_udc_done(ep, req, SCI_SUCCESS);
                }

                if (dev->test_mode != 0)
                    _udc_set_test_mode(dev->test_mode);

                udc_request_setup();
            } else {
                mon_Event(0xee770000|2);
                mon_Event(dev->ep0state);
            }

            if(s_nak_enable==1)
                *(volatile uint32 *) USB_DIEPMSK |= BIT_13;
        }

        if(diepint.mBits.nak)
        {
            if(s_nak_enable==0)
                *(volatile uint32 *) USB_DIEPMSK &= ~BIT_13;
        }
    }
    else
    {
        if (diepint.mBits.transfer_com)
        {
            USB_DIEPTSIZ_U dieptsiz;
            dieptsiz.dwValue = REG_R(USB_DIEPTSIZ(ep_id));

            if (req != NULL) {
                req->req.actual = req->req.length - dieptsiz.mBits.transfer_size;
                gx_udc_done(ep, req,SCI_SUCCESS);
            }
        }
    }
}

PUBLIC uint32 Gx_UDC_USB_Resume (BOOLEAN param)
{
    volatile USB_PCGCCTL_U *pcg_ctrl_ptr = (USB_PCGCCTL_U *) USB_PCGCCTL;
    uint32 i;

    if ( (gx_usb_suspend_flag==3))
    {
#if !defined(PLATFORM_UWS6121E)
        pcg_ctrl_ptr->mBits.GateHclk = 0;

        for (i=0; i<2; i++){}

        pcg_ctrl_ptr->mBits.StopPclk = 0;
        * (volatile uint32 *) (USB_PHY_CTRL) &= ~BIT_18;
#endif
        gx_usb_suspend_flag = BIT_1;
        SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
    }

    return 0;
}

/*****************************************************************************/
//  Description:   initialize the usb core.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
PUBLIC void usb_core_init(void)
{
    USB_GAHBCFG_U gahbcfg;
    USB_GRSTCTL_U grstctl;
    USB_GUSBCFG_U gusbcfg;
    USB_GINTMSK_U gintmsk;
    USB_DOEPMSK_U doepmsk;
    USB_DIEPMSK_U diepmsk;
    USB_DCTL_U dctl;
    USB_DCFG_U dcfg;
    uint8 i = 0;

    REG_W(USB_GAHBCFG, 0);

    grstctl.dwValue = 0;
    grstctl.mBits.csftrst = 1;
    REG_W(USB_GRSTCTL, grstctl.dwValue);

    OS_TickDelay(2);

    gusbcfg.dwValue = 0;
    gusbcfg.mBits.force_dev_mod = 1;
    gusbcfg.mBits.toutcal = 7;
    gusbcfg.mBits.srpcap = 0;
    gusbcfg.mBits.hnpcap = 0;
    gusbcfg.mBits.phyintf = 0;
    gusbcfg.mBits.usbtrdtim = 9;
    REG_W(USB_GUSBCFG, gusbcfg.dwValue);

    dctl.dwValue = REG_R(USB_DCTL);
    dctl.mBits.sftdiscon = 1;
    REG_W(USB_DCTL, dctl.dwValue);
    OS_TickDelay(2);
    dctl.dwValue = REG_R(USB_DCTL);
    dctl.mBits.sftdiscon = 0;
    REG_W(USB_DCTL, dctl.dwValue);

    dcfg.dwValue = 0;
    dcfg.mBits.devspeed = 0; // usb20
    REG_W(USB_DCFG, dcfg.dwValue);

    OS_TickDelay(16);

    { // only to create a block, config fifos
        USB_GRXFSIZ_U grxfsiz;
        USB_GNPTXFSIZ_U gnptxfsiz;
        USB_DIEPTXF_U dieptxf;
        uint32 start_addr = 0;
        uint32 time_out;

        grxfsiz.mBits.rxfdep = 1024;
        REG_W(USB_GRXFSIZ, grxfsiz.dwValue);
        start_addr += 1024;

        gnptxfsiz.mBits.nptxfstaddr = start_addr;
        gnptxfsiz.mBits.nptxfdep = 256;
        REG_W(USB_GNPTXFSIZ, gnptxfsiz.dwValue);
        start_addr += 256;

        for (i = 1; i < 3; ++i) {
            dieptxf.dwValue = 0;
            dieptxf.mBits.ineptxfsize = 512;
            dieptxf.mBits.ineptxfstraddr = start_addr;
            REG_W(USB_DIEPTXF(i), dieptxf.dwValue);
            start_addr += 512;
        }

        for (i = 3; i <= 8; ++i) {
            dieptxf.dwValue = 0;
            dieptxf.mBits.ineptxfsize = 128;
            dieptxf.mBits.ineptxfstraddr = start_addr;
            REG_W(USB_DIEPTXF(i), dieptxf.dwValue);
            start_addr += 128;
        }

        grstctl.dwValue = 0;
        grstctl.mBits.rxfflush = 1;
        REG_W(USB_GRSTCTL, grstctl.dwValue);
        for (time_out = 0; time_out < 1000 && (REG_R(USB_GRSTCTL) & BIT_4); time_out++) {}

        grstctl.dwValue = 0;
        grstctl.mBits.txfflush = 1;
        grstctl.mBits.txfnum = 0x10; // all
        REG_W(USB_GRSTCTL, grstctl.dwValue); // flush rx fifo
        for (time_out = 0; time_out < 1000 && (REG_R(USB_GRSTCTL) & BIT_5); time_out++) {}
    }

    for (i = 0; i < (USB_EPx_NUMBER + 1); ++i) {
        REG_OR(USB_DIEPCTL(i), BIT_27);
        REG_OR(USB_DOEPCTL(i), BIT_27);
    }

    doepmsk.dwValue = 0;
    doepmsk.mBits.setupmsk = 1;
    doepmsk.mBits.ahberrmsk = 1;
    doepmsk.mBits.xfercompmsk = 1;
    doepmsk.mBits.stsphsercvdmsk = 1;
    REG_W(USB_DOEPMSK, doepmsk.dwValue);

    diepmsk.dwValue = 0;
    diepmsk.mBits.xfercompimsk = 1;
    diepmsk.mBits.ahberrmsk = 1;
    diepmsk.mBits.timeoutmsk = 1;
    REG_W(USB_DIEPMSK, diepmsk.dwValue);

    gintmsk.dwValue = 0;
    gintmsk.mBits.oepintmsk = 1;
    gintmsk.mBits.iepintmsk = 1;
    // gintmsk.mBits.wkupintmsk = 1;
    gintmsk.mBits.enumdonemsk = 1;
    gintmsk.mBits.usbrstmsk = 1;
    // gintmsk.mBits.usbsuspmsk = 1;
    // gintmsk.mBits.erlususpmsk = 1;
    REG_W(USB_GINTMSK, gintmsk.dwValue);

    REG_W(USB_GINTSTS, 0xffffffff);

    gahbcfg.dwValue = 0;
    gahbcfg.mBits.dma_enable = 1;
    gahbcfg.mBits.burst_length = 7; // BURST_INCR16
    gahbcfg.mBits.global_int_mask = 1;
    REG_W(USB_GAHBCFG, gahbcfg.dwValue);
}

static void udc_clear_all_interrupt()
{
    USB_GINTSTS_U gintsts;
    uint16 daint, index;

    gintsts.dwValue = REG_R(USB_GINTSTS);
    REG_W(USB_GINTSTS, 0xffffffff);

    if (gintsts.mBits.iepint) {
        USB_DIEPCTL_U diepctl;
        diepctl.dwValue = 0;
        diepctl.mBits.set_nak = 1;

        daint = REG_R(USB_DAINT) & 0xffff;
        for (index = 0; index < USB_EPx_NUMBER; ++index) {
            if (daint & (1 << index)) {
                REG_OR(USB_DIEPCTL(index), diepctl.dwValue);
                REG_W(USB_DIEPINT(index), REG_R(USB_DIEPINT(index)));
            }
        }
    }

    if (gintsts.mBits.oepint) {
        USB_DOEPCTL_U doepctl;
        doepctl.dwValue = 0;
        doepctl.mBits.set_nak = 1;

        daint = (REG_R(USB_DAINT) >> 16) & 0xffff;
        for (index = 0; index < USB_EPx_NUMBER; ++index) {
            if (daint & (1 << index)) {
                REG_OR(USB_DOEPCTL(index), doepctl.dwValue);
                REG_W(USB_DOEPINT(index), REG_R(USB_DOEPINT(index)));
            }
        }
    }
}

/*
*  gx_udc_irq - interrupt handler
*/
LOCAL ISR_EXE_T Gx_UDC_ISR_Handler (uint32 param)
{
    // volatile USB_PCGCCTL_U *pcg_ctrl_ptr = (USB_PCGCCTL_U *) USB_PCGCCTL;
    struct sx_udc *dev = &memory;
    volatile USB_INTSTS_U  usb_int;
    ISR_EXE_T result = ISR_DONE;
    uint32 i;

    usb_int.dwValue = REG_R(USB_GINTSTS);

    //mon_Event(0xfeef0000|0xff);
    //mon_Event(usb_int.dwValue);

    REG_W(USB_GINTSTS, usb_int.dwValue); // clear usbc interrupt
    usb_int.dwValue &= REG_R(USB_GINTMSK);

    TM_SendTestPointRequest ( (0xBBB00000), usb_int.dwValue);

    if (FALSE == gx_udc_GetConnectState())
    {
        gx_udc_USB_Connect ();
    }

    if (FALSE == gx_udc_GetHostReq())
    {
        mon_Event(0xfeef0000|0xf1);
        if (usb_int.mBits.usbsusp)
        {
            * (volatile uint32 *) USB_GINTSTS &= ~BIT_11;
        }
        gx_udc_USB_DisableIrq();
        return CALL_HISR;
    }

    /* Driver not installed */
    if (!dev->driver)
    {
        mon_Event(0xfeef0000|0xf2);
        udc_clear_all_interrupt();
        return ISR_DONE;
    }

    if (usb_int.mBits.usbsusp)
    {
        if (dev->gadget.speed != USB_SPEED_UNKNOWN
           && dev->driver
           && dev->driver->suspend)
        {
            dev->driver->suspend (&dev->gadget);
        }

        dev->ep0state = EP0_STATE_INVALID;

#if !defined(PLATFORM_UWS6121E)
        if (gx_usb_suspend_flag == BIT_1)
        {
            pcg_ctrl_ptr->mBits.StopPclk = 1;

            for (i=0; i<2; i++){}

            pcg_ctrl_ptr->mBits.GateHclk = 1;
            gx_usb_suspend_flag |= BIT_0;

            for (i=0; i<2; i++){}

            pcg_ctrl_ptr->mBits.GateHclk = 1;

            * (volatile uint32 *) (USB_PHY_CTRL) |= BIT_18;
            GPIO_EnableIntCtl (GPIO_USB_RESUME_ID);
            SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        }
#endif
    }

    if (usb_int.mBits.wkupint)
    {
        if (dev->gadget.speed != USB_SPEED_UNKNOWN
            && dev->driver
            && dev->driver->resume)
        {
            dev->driver->resume (&dev->gadget);
        }

#if !defined(PLATFORM_UWS6121E)
        pcg_ctrl_ptr->mBits.GateHclk = 0;

        for (i=0; i<2; i++){}

        pcg_ctrl_ptr->mBits.StopPclk = 0;
        GPIO_DisableIntCtl (GPIO_USB_RESUME_ID);
#endif
    }

     // in endpoint interrupt
    if (usb_int.mBits.iepint)
    {
        USB_DIEPCTL_U diepctl;
        USB_DIEPINT_U diepint;

        uint32  isr_status = 0;
        uint8	ep_id = 0;
        uint16  daint;

        diepctl.dwValue = 0;
        diepctl.mBits.set_nak = 1;

        REG_AND(USB_GINTMSK, ~BIT_18); // disable EP in interrupt
        daint = REG_R(USB_DAINT) & 0xFFFF;

        for (ep_id = 0; ep_id < 10; ep_id++) {
            if (daint & (1 << ep_id)) {
                REG_OR(USB_DIEPCTL(ep_id), diepctl.dwValue);

                diepint.dwValue = REG_R(USB_DIEPINT(ep_id));
                REG_W(USB_DIEPINT(ep_id), diepint.dwValue); // clear some isr
                diepint.dwValue &= REG_R(USB_DIEPMSK);

                if (diepint.mBits.nak) {
                    REG_AND(USB_DIEPMSK, ~BIT_13);  // disable nak interrupt
                }

                isr_status = diepint.dwValue & 0xffff;
                isr_status |= ((ep_id | 0x80) << 16);
                IsrWriteBuffer(isr_status);
                //mon_Event(0xfeef0000|3);
            }
        }
        result = CALL_HISR;
    }

    // out endpoint interrupt
    if (usb_int.mBits.oepint)
    {
        USB_DOEPCTL_U doepctl;
        USB_DOEPINT_U doepint;

        uint32  isr_status = 0;
        uint16  daint;
        uint8	ep_id = 0;

        doepctl.dwValue = 0;
        doepctl.mBits.set_nak = 1;

        REG_AND(USB_GINTMSK, ~BIT_19); // Disable ep out
        daint = (REG_R(USB_DAINT) >> 16) & 0xffff;

        for (ep_id = 0; ep_id < 10; ep_id++) {
            if (daint & (1 << ep_id)) {
                REG_OR(USB_DOEPCTL(ep_id), doepctl.dwValue);

                doepint.dwValue = REG_R(USB_DOEPINT(ep_id));
                REG_W(USB_DOEPINT(ep_id), doepint.dwValue);
                doepint.dwValue &= REG_R(USB_DOEPMSK);

                if (doepint.mBits.nak) {
                    REG_AND(USB_DOEPMSK, ~BIT_13); // disable nak interupt
                }
                isr_status = doepint.dwValue & 0xffff;
                isr_status |= (ep_id << 16);
                IsrWriteBuffer(isr_status);
                //mon_Event(0xfeef0000|4);
            }
        }
        result = CALL_HISR;
    }

    // reset interrupt
    if (usb_int.mBits.usbrst)
    {
#if !defined(PLATFORM_UWS6121E)
        if ( (gx_usb_suspend_flag==3))
        {
            GPIO_DisableIntCtl (GPIO_USB_RESUME_ID);

            pcg_ctrl_ptr->mBits.GateHclk = 0;

            for (i=0; i<2; i++){}

            pcg_ctrl_ptr->mBits.StopPclk = 0;
            * (volatile uint32 *) (USB_PHY_CTRL) &= ~BIT_18;
            gx_usb_suspend_flag = BIT_1;
            SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
        }
#endif
       // mon_Event(0xfeef0000|1);
        udc_reset_handle();

        for (i=1; i<USB_EP_NUMBER; i++)
        {
            if (i%2 == 0)
            {
                * (volatile uint32 *) USB_DOEPCTL (i)  |= BIT_28; // data pid 0
            }
            else
            {
                * (volatile uint32 *) USB_DIEPCTL (i)  |= BIT_28; // data pid 0
            }
        }
        result=ISR_DONE;
    }

    // enumeration done interrupt
    if (usb_int.mBits.enumdone)
    {
        mon_Event(0xfeef0000|2);
        udc_enum_done();
        result=ISR_DONE;
    }

    return result;
}

static void Gx_UDC_INAndOUTHandle (uint32 param)
{
    uint16 int_status;
    uint8 ep_id;
    uint8 ep_address;

    int_status = param & 0xFFFF;

    ep_address = (param >> 16);
    ep_id = ep_address & 0x7f;

    if (g_ppp_data_buf && (ep_address == 0x82))
    {
        sio_ppp_free_tx_buf(g_ppp_data_buf);
		g_ppp_data_buf = NULL;
		g_ppp_send_status = 0;
        sio_ppp_tx_enable();
		return;
    }

    if (ep_address & USB_DIR_IN) {
        usb_EP_in_handle(ep_id, int_status);
    } else {
        usb_EP_out_handle(ep_id, int_status);
    }
}

static void GxUDCHisrFunc (uint32 i, void *d)
{
    int32 int_ep = 0;
    xSignalHeader signal;

    if (FALSE == gx_udc_GetHostReq())
    {
        gx_udc_SetHostReq (TRUE);

        // send to upper level notify cable connect
        signal = SCI_ALLOC (sizeof (*signal));
        signal->SignalCode = USB_HOST_PLUG_IN;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
        udc_clear_all_interrupt();
        USB_Disconnect();
        gx_udc_USB_EnableIrq();
    }

    while (!threadReadBuffer (&int_ep))
    {
        Gx_UDC_INAndOUTHandle (int_ep);
    }

    REG_OR(USB_GINTMSK, BIT_18 | BIT_19); // enable ep in/out mask
}


/*------------------------- s3c2410_ep_ops ----------------------------------*/

PUBLIC  struct sx_ep *to_sx_ep (struct usb_ep *ep)
{
    return container_of (ep, struct sx_ep, ep);   /*lint !e413 */
}

PUBLIC  struct sx_udc *to_sx_udc (struct usb_gadget *gadget)
{
    return container_of (gadget, struct sx_udc, gadget);   /*lint !e413 */
}

PUBLIC  struct sx_request *to_sx_req (struct usb_request *req)
{
    return container_of (req, struct sx_request, req);   /*lint !e413 */
}

/*
*  gx_udc_ep_enable
*/
LOCAL int gx_udc_ep_enable (struct usb_ep *_ep, const struct usb_endpoint_descriptor *desc)
{
    return 0;
}


/*
* gx_udc_ep_disable
*/
LOCAL int gx_udc_ep_disable (struct usb_ep *_ep)
{
    return 0;
}

/*
* gx_udc_alloc_request
*/
LOCAL struct usb_request *gx_udc_alloc_request (struct usb_ep *_ep)
{
    struct sx_request *req;

    if (!_ep) {
        return NULL;
    }

    req = SCI_ALLOC (sizeof (struct sx_request));

    if (!req) {
        return NULL;
    }

    SCI_MEMSET (req, 0, sizeof (struct sx_request));
    _INIT_LIST_HEAD (&req->queue);

    return &(req->req);
}

/*
* gx_udc_free_request
*/
LOCAL void gx_udc_free_request (struct usb_ep *_ep, struct usb_request *_req)
{
    struct sx_ep    *ep = to_sx_ep (_ep);
    struct sx_request   *req = to_sx_req (_req);

    if (!ep || !_req) {
        return;
    }

    if (ep->cur_req == req) {
        udc_hw_stop_xfer_(ep->ep.address);
        ep->cur_req = NULL;
        _udc_continue_schedule(ep);
    }

    if (!list_empty(&req->queue)) {
        list_del_init(&req->queue);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GX_UDC_1066_112_2_17_23_7_45_1447,(uint8*)"");
    }

    SCI_FREE(req);
}

static int gx_udc_queue(struct usb_ep *_ep, struct usb_request *_req)
{
    struct sx_udc *dev;
    struct sx_request *req = to_sx_req (_req);
    struct sx_ep *ep = to_sx_ep (_ep);
    uint32 ep_address;

    if (req->req.status != REQ_IS_IDLE)
        return SCI_ERROR;
    dev = ep->dev;
    ep_address = ep->ep.address;

    if (ep_address == 0)
    {
        uint8 *ctrlbuf = (uint8 *)udc_ctrl_buffer_();

        if (req->req.length)
            memcpy(ctrlbuf, req->req.buf, req->req.length);

        if (dev->ep0state == EP0_DATA_IN) {
            if (ep->cur_req == NULL) {
                ep->cur_req = req;
                udc_hw_start_xfer_(0x80, 64, ctrlbuf, req->req.length);
            } else {
                list_add_tail(&req->queue, &ep->queue);
            }
        } else if (dev->ep0state == EP0_DATA_OUT) {
            if (ep->cur_req == NULL) {
                ep->cur_req = req;
                udc_hw_start_xfer_(0, 64, ctrlbuf, req->req.length);
            } else {
                list_add_tail(&req->queue, &ep->queue);
            }
        } else {
            mon_Event(0xee770000|3);
            mon_Event(dev->ep0state);
        }
    }
    else if ((ep_address & USB_DIR_IN) == 0)  // out transfer
    {
        SCI_DisableIRQ();
        req->req.status = REQ_IN_QUEUE;
        if (ep->cur_req == NULL) {
            ep->cur_req = req;
            SCI_ASSERT(((uint32)req->req.buf % 32) == 0);
            udc_hw_start_xfer_(ep_address, ep->ep.maxpacket, req->req.buf, req->req.length);
        } else {
            list_add_tail(&req->queue, &ep->queue);
            //mon_Event(0xee440000|1);
        }
        SCI_RestoreIRQ();
    }
    else // in transfer
    {
        SCI_DisableIRQ();
        req->req.status = REQ_IN_QUEUE;
        if (ep->cur_req == NULL) {
            ep->cur_req = req;
            SCI_ASSERT(((uint32)req->req.buf % 4) == 0);
            udc_hw_start_xfer_(ep_address, ep->ep.maxpacket, req->req.buf, req->req.length);
        } else {
            list_add_tail(&req->queue, &ep->queue);
            //mon_Event(0xee440000|2);
        }
        SCI_RestoreIRQ();
    }

    return SCI_SUCCESS;
}

/*
*  gx_udc_dequeue
*/
LOCAL int gx_udc_dequeue (struct usb_ep *_ep, struct usb_request *_req)
{
    struct sx_ep    *ep = to_sx_ep (_ep);
    struct sx_request   *req = to_sx_req (_req);

    SCI_DisableIRQ();

    if (req == ep->cur_req) {
        udc_hw_stop_xfer_(_ep->address);
    }

    if (req) {
        gx_udc_done(ep, req, SCI_ERROR);
    }

    SCI_RestoreIRQ();

    return 0;
}

/*
* gx_udc_set_halt
*/
LOCAL int gx_udc_set_halt (struct usb_ep *_ep, int value)
{
    return 0;
}

LOCAL struct usb_ep_ops gx_ep_ops;


/*------------------------- usb_gadget_ops ----------------------------------*/

/*
*  gx_udc_get_frame
*/
LOCAL int gx_udc_get_frame (struct usb_gadget *_gadget)
{
    return 0;
}

/*
*  gx_udc_wakeup
*/
LOCAL int gx_udc_wakeup (struct usb_gadget *_gadget)
{
    return 0;
}

/*
*  gx_udc_set_selfpowered
*/
LOCAL int gx_udc_set_selfpowered (struct usb_gadget *gadget, int value)
{
    return 0;
}

LOCAL int gx_udc_vbus_session (struct usb_gadget *gadget, int is_active)
{
    return 0;
}

/*****************************************************************************/
//  Description:   udc_USB_SetPllDividor
//  Global resource dependence:
//  Author:        Daniel.Ding
//  Note:          set usb 12m and 48m clk dividor according to PLL
/*****************************************************************************/
LOCAL void gx_udc_vbus_irq (uint32 gpio_state)
{
    xSignalHeader signal;

    signal = SCI_ALLOC (sizeof (*signal));

#if !defined(PLATFORM_UWS6121E)
    GPIO_DisableIntCtl (GPIO_USB_RESUME_ID);
#endif

    if (gpio_state)                             // cable inserted
    {
        SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
        gx_udc_USB_Connect ();
        signal->SignalCode = USB_VBUS_PLUG_IN;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
    }
    else                                        // cable removed
    {
        gx_udc_USB_DisableIrq();
        gx_udc_USB_Disconnect ();
        gx_udc_SetHostReq (FALSE);
#if !defined(PLATFORM_UWS6121E)
        * (volatile uint32 *) AHB_CTL0 &= ~BIT_5; // usbd enable
#endif
        USB_LDO_Control (FALSE);
        signal->SignalCode = USB_VBUS_PLUG_OUT;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
        SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        gx_usb_suspend_flag = 0;
        s_nak_enable = 0;
    }
}

LOCAL int gx_vbus_draw (struct usb_gadget *_gadget, uint32 ma)
{
    return 0;
}

LOCAL int gx_udc_pullup (struct usb_gadget *gadget, int is_on)
{
    return 0;
}

static struct usb_ep *gx_udc_alloc_ep(struct usb_gadget *gadget, uint8 addr)
{
    struct sx_udc *udc = to_sx_udc(gadget);
    struct sx_ep *ep = _udc_get_ep(udc, addr);

    return ep == NULL ? NULL : &ep->ep;
}

LOCAL struct usb_gadget_ops gx_ops;

/*
 *  usb_gadget_register_driver
 */
/*lint -e552 */
PUBLIC int32 USBGadgetRegisterDriver (struct usb_gadget_driver *driver)
{
    struct sx_udc *udc = the_controller;
    int32       retval;
    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);

#ifdef WINUSB_SUPPORT
	/* match diff hw id */
	usb_set_product_info(s_charge_product_str);
	usb_set_sn_info();
#endif
    /* Sanity checks */
    if (!udc)
        return SCI_ERROR;

    udc->ep0state = EP0_STATE_INVALID;

    if (udc->driver)
        return SCI_ERROR;

    if (!driver->bind || !driver->setup || driver->speed < USB_SPEED_FULL)
        return SCI_ERROR;

    if (!driver->unbind)
        return SCI_ERROR;

    UsbDevice_Create(driver, &udc->gadget);
    if ( (retval = driver->bind (&udc->gadget)) != SCI_SUCCESS)
        goto register_error;

    udc->driver = driver;

    if (FALSE == gx_udc_GetHostReq())
    {
        // mark the host connect
        gx_udc_SetHostReq (TRUE);
    }

    gx_udc_init();

    return SCI_SUCCESS;

register_error:
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GX_UDC_1394_112_2_17_23_7_45_1448,(uint8*)"");
    udc->driver = NULL;
    return SCI_ERROR;
}

/*
*  usb_gadget_unregister_driver
*/
PUBLIC int32 USBGadgetUnregisterDriver (struct usb_gadget_driver *driver)
{
    struct sx_udc *udc = the_controller;

    if (!udc)
    {
        return SCI_ERROR;
    }

    if (!driver || driver != udc->driver || !driver->unbind)
    {
        return SCI_ERROR;
    }

    driver->unbind (&udc->gadget);
    UsbDevice_Delete(&udc->gadget);

    udc->driver = NULL;
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GX_UDC_1419_112_2_17_23_7_45_1449,(uint8*)"");

    return 0;
}

static void gx_udc_USB_EnableIrq (void)
{
    CHIPDRV_EnableIRQINT (TB_USB_INT);
}

static void gx_udc_USB_DisableIrq (void)
{
    CHIPDRV_DisableIRQINT (TB_USB_INT);
}

LOCAL void gx_udc_init (void)
{
    gx_udc_USB_DisableIrq();

    USB_Connect();
    * (volatile uint32 *) USB_DOEPMSK &= ~BIT_13;

    gx_udc_USB_EnableIrq();
}

PUBLIC void UDC_HWInit (void)
{
    gx_udc_init();
}

PUBLIC void UDC_HWUNInit (void)
{
    gx_udc_USB_DisableIrq();
    USB_Disconnect();
}

static void _udc_init_usb_ep(USB_EP_T *ep, uint8 addr, uint16 mps, const USB_EP_OPS_T *ops)
{
    ep->address = addr;
    ep->maxpacket = mps;
    ep->ops = ops;
}

static void gx_udc_data_init(void)
{
    static SX_EP_T _udc_iep_room[USB_EPx_NUMBER];
    static SX_EP_T _udc_oep_room[USB_EPx_NUMBER];
    static BOOLEAN _data_configed = FALSE;

    struct sx_ep *ep;
    struct sx_udc *udc = &memory;
    uint8 i;

    if (_data_configed) {
        return;
    }

    _data_configed = TRUE;

    /* udc operation */
    gx_ops.get_frame        = gx_udc_get_frame;
    gx_ops.wakeup           = gx_udc_wakeup;
    gx_ops.set_selfpowered      = gx_udc_set_selfpowered;
    gx_ops.pullup           = gx_udc_pullup;
    gx_ops.vbus_session     = gx_udc_vbus_session;
    gx_ops.vbus_draw        = gx_vbus_draw;
    gx_ops.alloc_ep         = gx_udc_alloc_ep;

    /* endpoint operation */
    gx_ep_ops.enable        = gx_udc_ep_enable;
    gx_ep_ops.disable       = gx_udc_ep_disable;
    gx_ep_ops.alloc_request     = gx_udc_alloc_request;
    gx_ep_ops.free_request      = gx_udc_free_request;
    gx_ep_ops.queue         = gx_udc_queue;
    gx_ep_ops.dequeue       = gx_udc_dequeue;
    gx_ep_ops.set_halt      = gx_udc_set_halt;

    udc->address = USB_REG_BASE;
    udc->gadget.ops = &gx_ops;
    udc->gadget.ep0 = &udc->ep0.ep;
    udc->gadget.name = gadget_name;

    udc->ep0.gadget = &udc->gadget;
    udc->ep0.dev = udc;
    _INIT_LIST_HEAD(&udc->ep0.queue);
    _udc_init_usb_ep(&udc->ep0.ep, 0, 64, &gx_ep_ops);

    udc->iep = &_udc_iep_room[0];
    udc->iep_cnt = USB_EPx_NUMBER;
    for (i = 0; i < udc->iep_cnt; ++i) {
        ep = &udc->iep[i];
        ep->gadget = &udc->gadget;
        ep->dev = udc;
        _INIT_LIST_HEAD(&ep->queue);
        _udc_init_usb_ep(&ep->ep, ((i + 1) | 0x80), 512, &gx_ep_ops);
    }

    udc->oep = &_udc_oep_room[0];
    udc->oep_cnt = USB_EPx_NUMBER;
    for (i = 0; i < udc->oep_cnt; ++i) {
        ep = &udc->oep[i];
        ep->gadget = &udc->gadget;
        ep->dev = udc;
        _INIT_LIST_HEAD(&ep->queue);
        _udc_init_usb_ep(&ep->ep, (i + 1), 512, &gx_ep_ops);
    }
}

/*
*  probe - binds to the platform device
*/
LOCAL uint32 gx_udc_probe (void)
{
    uint32 status;

    the_controller = &memory;

    gx_udc_data_init();

    SCI_DisableIRQ();

    status = ISR_RegHandler_Ex (TB_USB_INT, Gx_UDC_ISR_Handler, GxUDCHisrFunc, CHIPDRV_HISR_PRIO_0, NULL);

    if (TB_SUCCESS == status)
    {
        CHIPDRV_EnableIRQINT (TB_USB_INT);
    }

    GPIO_VBUS_RegIntHandler (gx_udc_vbus_irq);

    SCI_RestoreIRQ();

    return SCI_SUCCESS;
}

PUBLIC uint32 UDC_Init (void)
{
    uint32 ret;

    ret = gx_udc_probe();

    USB_Disconnect();

    return ret;
}

PUBLIC uint32 UDC_GetConnect (void)
{
    return gx_udc_GetConnectState();
}

PUBLIC uint32 UDC_GetHostReq (void)
{
    return gx_udc_GetHostReq();
}

PUBLIC void UDC_ClkReset (void)
{
}

PUBLIC void UDC_PowerSet (uint32 power)
{
}

PUBLIC void UDC_Config (void)
{
}

/*****************************************************************************/
//  Description:    udc_USB_Reset
//  Global resource dependence:
//  Author:
//  Note:           Call this function to reset udc
/*****************************************************************************/
PUBLIC void UDC_Reset (void)
{
    ;//sx_udc_USB_Reset();
}

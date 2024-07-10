
#include "os_api.h"
#include "chip_plf_export.h"
#include "usb_ch9.h"
#include "usb20_app_config.h"
#include "task_monitor.h"
#include "chg_drvapi.h"
#include "usb20_drvapi.h"
#include "cpu_endian.h"
#include "udc_internal.h"

typedef struct udc_ep {
    usb_xfer_t *cur_xfer;
    uint32 xfer_size;
    uint16 mps;
    uint8 address;
    uint8 zero : 1;
} udc_ep_t;

typedef struct udc_polling {
    udc_ep_t ep0;
    udc_ep_t iep[USB_EPx_NUMBER];
    udc_ep_t oep[USB_EPx_NUMBER];
    int test_mode;
    int ep0_state;
} udc_polling_t;

static udc_polling_t g_polling_udc;
static USB_APP_OP_T *usb_op_ptr = NULL;

static udc_ep_t *_get_ep(uint8 ep_addr)
{
    udc_polling_t *udc = &g_polling_udc;
    uint8 num = ep_addr & 0x7f;
    if (num == 0) {
        return &udc->ep0;
    }

    if (ep_addr & USB_DIR_IN) {
        if (num > ARRAY_SIZE(udc->iep))
            return NULL;
        return &udc->iep[num -1];
    } else {
        if (num > ARRAY_SIZE(udc->oep))
            return NULL;
        return &udc->oep[num - 1];
    }
}

void udc_polling_cancel_xfer(uint8 ep_, usb_xfer_t *xfer)
{
    udc_ep_t *ep = _get_ep(ep_);
    if (ep == NULL || xfer == NULL)
        return;

    if (ep->cur_xfer != xfer)
        return;

    ep->cur_xfer = NULL;
    udc_hw_stop_xfer_(ep_);
    xfer->actual = 0;
    xfer->status = -2;
}

int udc_polling_start_xfer(uint8 ep_, usb_xfer_t *xfer)
{
    udc_ep_t *ep = _get_ep(ep_);
    if (ep == NULL || xfer == NULL)
        return -1;

    if (ep->cur_xfer)
        return -2;

    ep->zero = 0;
    if ((ep_ & USB_DIR_IN) && xfer->zlp) {
        ep->zero = (xfer->length % ep->mps == 0) ? 1 : 0;
    }

    xfer->actual = 0;
    xfer->status = -2;
    ep->cur_xfer = xfer;
    ep->xfer_size = xfer->length;
    udc_hw_start_xfer_(ep_, ep->mps, xfer->buf, xfer->length);

    return 0;
}

static void udc_reset_pipe_polling(uint16 epno)
{
    uint8 epnum = epno & 0x7f;
    if (epnum == 0)
        return;

    if (epno & USB_DIR_IN) {
        REG_OR(USB_DIEPCTL(epnum), BIT_28);
    } else {
        REG_OR(USB_DOEPCTL(epnum), BIT_28);
    }
}

static void udc_set_feature_polling(struct usb_ctrlrequest *setup)
{
    switch (setup->bRequestType & USB_RECIP_MASK) {
    case USB_RECIP_DEVICE:
        if (setup->wValue == USB_DEVICE_TEST_MODE) {
            g_polling_udc.test_mode = (setup->wIndex >> 8);
        }
        break;

    default:
        break;
    }
}

static int udc_get_status_polling(struct usb_ctrlrequest *setup, void *buf, unsigned size)
{
    uint16 reply = 0;

    if (setup->wLength != 2 || size < 2)
        return -1;

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
        reply = 0;
        break;

    default:
        return -1;
    }

    memcpy(buf, &reply, 2);
    return 2;
}

static void udc_setup_polling()
{
    udc_polling_t *udc = &g_polling_udc;
    int length = 0;
    uint32 vendor_ack = 0;
    const uint32 ctrl_buf_size = udc_ctrl_buf_size_();
    struct usb_ctrlrequest *setup_ = (struct usb_ctrlrequest *)udc_ctrl_buffer_();
    struct usb_ctrlrequest setup = *setup_;
    void *ctrl_buf = (void *)setup_;

    usb_op_ptr = USB_GetCurrentAppOperation();
    if (usb_op_ptr == NULL) {
        return;
    }

    if (setup.wLength == 0) {
        udc->ep0_state = EP0_STATUS_IN;
    } else {
        if (setup.bRequestType & USB_DIR_IN) {
            udc->ep0_state = EP0_DATA_IN;
        } else {
            udc->ep0_state = EP0_DATA_OUT;
        }
    }

    switch (setup.bRequestType & USB_TYPE_MASK) {
    case USB_TYPE_STANDARD: // standard
        switch (setup.bRequest) {
        case USB_REQ_SET_FEATURE:
            udc_set_feature_polling(&setup);
            break;

        case USB_REQ_SET_ADDRESS:
            udc_hw_set_address_(setup.wValue & 0xff);
            break;

        case USB_REQ_GET_DESCRIPTOR:
            if (usb_op_ptr && usb_op_ptr->get_descriptor)
                length = usb_op_ptr->get_descriptor(&setup, ctrl_buf, ctrl_buf_size);
            break;

        case USB_REQ_GET_STATUS:
            length = udc_get_status_polling(&setup, ctrl_buf, ctrl_buf_size);
            break;

        case USB_REQ_SET_CONFIGURATION:
            if (usb_op_ptr && usb_op_ptr->set_config) {
                usb_op_ptr->set_config(setup.wValue & 0xff);
            }
            break;

        case USB_REQ_CLEAR_FEATURE:
            if ((setup.bRequestType & USB_RECIP_MASK) == USB_RECIP_ENDPOINT) {
                udc_reset_pipe_polling(setup.wIndex & 0xff);
            }
            break;

        default:
            break;
        }
        break;

        case USB_TYPE_CLASS: // class
            switch (setup.bRequestType & USB_RECIP_MASK) {
            case USB_RECIP_INTERFACE:
                if (usb_op_ptr && usb_op_ptr->app_command) {
                    usb_op_ptr->app_command(&setup);
                }
                break;

            default:
                break;
            }
            break;

        case USB_TYPE_VENDOR:
            memcpy(ctrl_buf, &vendor_ack, 4);
            length = 4;
            break;

        default:
            break;
    }

    if (udc->ep0_state == EP0_STATUS_IN) {
        udc_hw_start_xfer_(0x80, 64, NULL, 0);
    } else if (length > 0) {
        if (udc->ep0_state == EP0_DATA_IN) {
            udc_hw_start_xfer_(0x80, 64, ctrl_buf, length);
        } else if (udc->ep0_state == EP0_DATA_OUT) {
            udc_hw_start_xfer_(0, 64, ctrl_buf, length);
        }
    }
}

static void udc_reset_polling(void)
{
    udc_polling_t *udc = &g_polling_udc;
    udc_hw_reset_();
    udc->test_mode = 0;
    udc->ep0_state = EP0_STATE_INVALID;
}

static void _request_setup()
{
    void *setup = (void *)udc_ctrl_buffer_();

    g_polling_udc.ep0_state = EP0_SETUP;

    udc_hw_start_xfer_(0, 64, setup, 8);
}

static void udc_enumdone_polling(void)
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
        udc_ep_t *ep = _get_ep(ep_cfg->ep_address);
        udc_hw_ep_enable_(ep_cfg->ep_address, (USB_EP_TYPE_E)ep_cfg->ep_type, ep_cfg->mps);
        if (ep) {
            ep->cur_xfer = NULL;
            ep->zero = 0;
        }
        ep_cfg++;
    }

    oep_msk = usb_GetOutEPMask(1);
    iep_msk = usb_GetOutEPMask(0);
    REG_OR(USB_DAINTMSK, 0x10001 | oep_msk | iep_msk);

    _request_setup();

    dctl.dwValue = 0;
    dctl.mBits.cgoutnak = 1;
    dctl.mBits.cgnpinnak = 1;
    REG_OR(USB_DCTL, dctl.dwValue);
}

static void udc_iep_polling(uint8 epnum, uint16 epint_)
{
    udc_polling_t *udc = &g_polling_udc;
    USB_DIEPINT_U epint;
    USB_DIEPTSIZ_U eptsiz;
    epint.dwValue = epint_;
    eptsiz.dwValue = REG_R(USB_DIEPTSIZ(epnum));

    if (epint.mBits.nak) {
        REG_AND(USB_DIEPMSK, ~BIT_13);
    }

    if (epnum == 0) {
        if (udc->ep0_state == EP0_DATA_IN) {
            udc->ep0_state = EP0_STATUS_OUT;
            udc_hw_start_xfer_(0, 64, NULL, 0);
        } else if (udc->ep0_state == EP0_STATUS_IN) {
            if (udc->test_mode != 0) {
                USB_DCTL_U dctl;
                dctl.dwValue = REG_R(USB_DCTL);

                switch (udc->test_mode) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    dctl.mBits.tstctl = udc->test_mode;
                    REG_W(USB_DCTL, dctl.dwValue);
                    break;

                default:
                    break;
                }
            }
            _request_setup();
        }
    } else {
        udc_ep_t *ep = _get_ep((0x80 | epnum));

        if (ep && ep->cur_xfer) {
            usb_xfer_t *xfer = ep->cur_xfer;
            xfer->actual += ep->xfer_size - eptsiz.mBits.transfer_size;
            if (ep->zero == 1) {
                ep->zero = 0;
                ep->xfer_size = 0;
                udc_hw_start_xfer_(ep->address, ep->mps, NULL, 0);
            } else {
                ep->xfer_size = 0;
                ep->cur_xfer = NULL;
                xfer->status = 0;
                if (xfer->callback) {
                    xfer->callback(ep->address, xfer);
                }
            }
        }
    }
}

static void udc_oep_polling(uint8 epnum, uint16 epint_)
{
    udc_polling_t *udc = &g_polling_udc;
    USB_DOEPINT_U epint;
    USB_DOEPTSIZ_U eptsiz;

    epint.dwValue = epint_;
    eptsiz.dwValue = REG_R(USB_DOEPTSIZ(epnum));

    TM_SendTestPointRequest ((0xBBBA0000|epnum), epint.dwValue);

    if (epnum == 0) {
        BOOLEAN setup = epint.mBits.timeout_condi ? TRUE : FALSE; // for ep0, timeout_condi -> setup
        setup = (setup || (epint.mBits.stuppktrcvd ? TRUE : FALSE));

        if (setup) {
            epint.mBits.transfer_com = 0;
        }

        if (epint.mBits.timeout_condi) {
            udc_setup_polling();
        }
    }

    if (epint.mBits.transfer_com) {
        if (epnum == 0) {
            if (udc->ep0_state == EP0_DATA_OUT) {
                udc->ep0_state = EP0_STATUS_IN;
                udc_hw_start_xfer_(0x80, 64, NULL, 0);
            } else if (udc->ep0_state == EP0_STATUS_OUT) {
                _request_setup();
            }
        } else {
            udc_ep_t *ep = _get_ep(epnum);
            if (ep && ep->cur_xfer) {
                usb_xfer_t *xfer = ep->cur_xfer;
                xfer->actual += ep->xfer_size - eptsiz.mBits.transfer_size;
                ep->cur_xfer = NULL;
                ep->xfer_size = 0;
                MMU_DmaCacheSync((uint32)(xfer->buf), xfer->length, DMABUFFER_FROM_DEVICE);
                if (xfer->callback) {
                    xfer->status = 0;
                    xfer->callback(epnum, xfer);
                }
            }
        }
    }
}

void udc_polling_init()
{
    static BOOLEAN _init = FALSE;
    udc_polling_t *udc = &g_polling_udc;
    uint8 index;

    if (_init) {
        return;
    }

    _init = TRUE;

    memset(udc, 0, sizeof(*udc));
    udc->test_mode = 0;
    udc->ep0_state = EP0_STATE_INVALID;

#define UDC_USB20_MPS 512
#define UDC_EP0_MPS 64

    udc->ep0.mps = UDC_EP0_MPS;

    for (index = 0; index < ARRAY_SIZE(udc->iep); ++index) {
        udc_ep_t *ep = &udc->iep[index];
        ep->mps = UDC_USB20_MPS;
        ep->address = (index + 1) | 0x80;
        ep->cur_xfer = NULL;
    }

    for (index = 0; index < ARRAY_SIZE(udc->oep); ++index) {
        udc_ep_t *ep = &udc->oep[index];
        ep->mps = UDC_USB20_MPS;
        ep->address = (index + 1);
        ep->cur_xfer = NULL;
    }
}

void udc_polling_handler()
{
    uint8 index;
    uint16 daint, epint;
    USB_GINTSTS_U gintsts;
    gintsts.dwValue = REG_R(USB_GINTSTS);
    REG_W(USB_GINTSTS, gintsts.dwValue); // write to clear

    gintsts.dwValue &= REG_R(USB_GINTMSK);

    TM_SendTestPointRequest ((0xBBB00000), gintsts.dwValue);
    udc_polling_init();

    if (gintsts.mBits.usbrst) {
        udc_reset_polling();
    }

    if (gintsts.mBits.enumdone) {
        udc_enumdone_polling();
    }

    if (gintsts.mBits.iepint) {
        USB_DIEPCTL_U diepctl;
        diepctl.dwValue = 0;
        diepctl.mBits.set_nak = 1;

        daint = REG_R(USB_DAINT) & 0xffff;
        for (index = 0; index < USB_EPx_NUMBER + 1; ++index) {
            if (daint & (1 << index)) {
                REG_OR(USB_DIEPCTL(index), diepctl.dwValue);

                epint = REG_R(USB_DIEPINT(index));
                REG_W(USB_DIEPINT(index), epint);
                epint &= REG_R(USB_DIEPMSK);
                udc_iep_polling(index, epint);
            }
        }
    }

    if (gintsts.mBits.oepint) {
        USB_DOEPCTL_U doepctl;
        doepctl.dwValue = 0;
        doepctl.mBits.set_nak = 1;

        daint = (REG_R(USB_DAINT) >> 16) & 0xffff;
        for (index = 0; index < USB_EPx_NUMBER + 1; ++index) {
            if (daint & (1 << index)) {
                REG_OR(USB_DOEPCTL(index), doepctl.dwValue);

                epint = REG_R(USB_DOEPINT(index));
                REG_W(USB_DOEPINT(index), epint);
                epint &= REG_R(USB_DOEPMSK);
                udc_oep_polling(index, epint);
            }
        }
    }
}

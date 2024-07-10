/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include <stddef.h>
#include <string.h>
#include "errno.h"

#include "udc_dwc.h"
#include "usb_platform_uws6121e.h"
#include "usb_ch9.h"
#include "osi_compiler.h"
#include <stdlib.h>
#include "os_api.h"
#include "chip_drv_common_inter.h"
#include "mmu_drvapi.h"
#include "uws6121e_int_cfg.h"

// #define FORCE_FULL_SPEED

#define DWC_EP_MPS_FULL (64)
#define DWC_EP_MPS_HIGH (512)
#define DWC_EP0_MPS (64)
INPUT_BUFFER_INIT (int32,64)

#define ISRAM(n) (OSI_IS_IN_REGION(unsigned, n, CONFIG_RAM_PHY_ADDRESS, CONFIG_RAM_SIZE) || \
                  OSI_IS_IN_REGION(unsigned, n, CONFIG_SRAM_PHY_ADDRESS, CONFIG_SRAM_SIZE))

#ifdef _DEBUG_USB20_
#define REGISTER_MAP_LEN (0xC00)
LOCAL uint32 usb_register_map[REGISTER_MAP_LEN>>2]= {0};
#endif
enum dwc_speed
{
    DWC_SPEED_HIGH_20 = 0,
    DWC_SPEED_FULL_20 = 1,
    DWC_SPEED_LOW_11 = 2,
    DWC_SPEED_FULL48_11 = 3
};

enum dwc_state
{
    DWC_NORMAL = 0,
    DWC_SUSPEND,
    DWC_OFF
};

typedef TAILQ_ENTRY(dwc_xfer) dwc_xfer_entry_t;
typedef TAILQ_HEAD(, dwc_xfer) dwc_xfer_head_t;
typedef struct dwc_xfer
{
    usbXfer_t xfer; ///< keep first member
    uint32_t xfer_size;
    dwc_xfer_entry_t anchor;
    uint8_t zero : 1;
    uint8_t occupied : 1;
} dwcXfer_t;

typedef struct dwc_tx_fifo dwcTxFifo_t;
typedef TAILQ_ENTRY(dwc_tx_fifo) dwcTxFifoIter_t;
typedef TAILQ_HEAD(dwc_tx_fifo_head, dwc_tx_fifo) dwcTxFifoHead_t;
struct dwc_tx_fifo
{
    dwcTxFifoIter_t iter;
    uint16_t bytes;
    uint8_t index : 4;
    uint8_t using_ep : 4;
    uint8_t last_ep : 4;
};


typedef struct dwc_ep dwcEp_t;
typedef STAILQ_ENTRY(dwc_ep) dwcEpIter_t;
typedef STAILQ_HEAD(, dwc_ep) dwcEpHead_t;
struct dwc_ep
{
    usbEp_t ep;
    uint8_t type : 2;
    uint8_t periodic : 1;
    uint8_t used : 1;
    uint8_t halted : 1;
    uint8_t dynamic_fifo : 1;
    uint8_t pending : 1;
    uint32_t dma_max;
    dwcTxFifo_t *fifo;
    dwcXfer_t *xfer_schedule;
    dwc_xfer_head_t xfer_q;
    dwcEpIter_t iter;
};


#define DWC_TXFIFO_COUNT (8)

typedef struct dwcUdc
{
    udc_t *gadget;
    bool is_configured;
    dwcEp_t iep[15 + 1];
    dwcEp_t oep[15 + 1];
    uint8_t iep_count; ///< include ep0
    uint8_t oep_count; ///< include ep0
    uint8_t test_mode;
    uint8_t dedicated_fifo : 1; ///< reserved fifo for each enabled in endpoint
    uint8_t connected : 1;
    uint8_t disable_schedule : 1;
    uint16_t current_epno;
    uint16_t tx_fifo_map;
    ep0State_t ep0_state;
    enum dwc_state pm_level;
    dwcXfer_t ep0_xfer;
    dwcXfer_t setup_xfer;
    dwcEpHead_t pending_eps;
    dwcTxFifoHead_t avail_txfifo;
    dwcTxFifo_t txfifo_room[DWC_TXFIFO_COUNT];
    uint8_t *ep0_buf;   ///< make sure 8 bytes is enough
    uint8_t *setup_buf; ///< make sure 8 bytes is enough
} dwcUdc_t;

#define likely(X)    __builtin_expect((X),1)
#define unlikely(X)  __builtin_expect((X),0)
extern uint32 OS_TickDelay (uint32 ticks);
extern void drvUsbReset(void);
#ifdef PPP_USB
extern void ppp_clear_send_status(void);
#endif

static dwcUdc_t _the_controller;;
usb_device_request_t ctrl;;

static const unsigned char musb_test_packet[53] = {

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,

    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,

    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd,

    0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

};

static const unsigned char musb_test_fifo[40] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

#define TEST_DMA_LNKLIST_NODE_NUM 32

typedef struct linklist_node_s
{
    unsigned int addr;
    unsigned short frag_len;
    unsigned short blk_len;

    unsigned int list_end : 1;
    unsigned int sp : 1;
    unsigned int ioc : 1;
    unsigned int pad0 : 5;
    unsigned int haddr : 4;
    unsigned int pad : 20;
    unsigned int rsv;
} linklist_node_t;

#define EP2DWCEP(ep) (dwcEp_t *)(ep)


static dwcUdc_t *prvGadgetGetDwc(udc_t *udc)
{
    return (dwcUdc_t *)udc->controller_priv;
}

static dwcXfer_t *prvXferGetDwcXfer(usbXfer_t *xfer)
{
    return (dwcXfer_t *)xfer;
}

/**
 * Select EP for indexed registers access
 */
static void musbSelectEp(uint8_t ep) { MUSB_WRITE8(0x0E, ep); }

static dwcEp_t *prvAddressGetEndpoint(dwcUdc_t *dwc, uint8_t address)
{
    uint8_t num = USB_GET_ADDR(address);
    if (USB_EP_GET_DIR(address) == USB_DIR_IN)
    {
        if (num > dwc->iep_count)
            return NULL;
        return &dwc->iep[num];
    }
    else
    {
        if (num > dwc->oep_count)
            return NULL;
        return &dwc->oep[num];
    }
}

static dwcEp_t *prvGetDwcEp0(dwcUdc_t *dwc)
{
    if (dwc->ep0_state & EP0_STATE_DIRIN_MASK)
        return &dwc->iep[0];
    else
        return &dwc->oep[0];
}

static void prvChangeEp0State(dwcUdc_t *dwc, ep0State_t st)
{
    dwc->ep0_state = st;
}

static void prvSetAddress(dwcUdc_t *dwc, uint8_t addr)
{
    MUSB_WRITE8(MUSB_FADDR, addr);
}

static int prvEpQueueLocked(dwcUdc_t *dwc, dwcEp_t *ep, dwcXfer_t *xfer);
static void prvEpStopLocked(dwcUdc_t *dwc, dwcEp_t *ep);
static void prvStallEp0(dwcUdc_t *dwc, dwcEp_t *ep);
static int prvStallEpLocked(dwcUdc_t *dwc, dwcEp_t *ep_, bool halt);
static void prvEpDisableLocked(dwcUdc_t *dwc, dwcEp_t *ep);
static void prvEpCancelXferInQueue(dwcEp_t *ep);
static void prvRxFifoFlush(dwcUdc_t *dwc);
static void prvTxFifoFlush(dwcUdc_t *dwc);
static void prvHwInit(dwcUdc_t *dwc, bool reset_int);
static void prvExitHibernation(dwcUdc_t *dwc);
static void prvTxFifoFreeLocked(dwcUdc_t *dwc, dwcEp_t *ep);
static void prvGiveBackEpCurrent(dwcUdc_t *dwc, dwcEp_t *ep);

void UdcStartRxDma(dwcUdc_t *dwc, dwcEp_t *ep, uint32_t length);
/**
 * Write to EP fifo. This should be called only for EP0.
 * Hardware doesn't support EP fifo access except EP0.
 */
void musbWriteFifo(unsigned ep, const void *data, unsigned size)
{
    const uint8_t *pdata = (const uint8_t *)data;
    unsigned n;
    for ( n = 0; n < size; n++)
        MUSB_WRITE8(MUSB_EP0_FIFO + 4 * ep, *pdata++);
}

/**
 * EP0 tx transfer. It should be called once at most for each transaction.
 */
void UdcEp0Tx(udc_t *udc, const void *data, unsigned size)
{
    unsigned trans;
    uint16_t csr;
    // It will be called after SETUP phase, so set SVDRXPKTRDY
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY);

    while (size > 0)
    {
        trans = OSI_MIN(unsigned, size, MUSB_EP0_FIFOSIZE);
        musbWriteFifo(0, data, trans);

        data = (const char *)data + trans;
        size -= trans;

        csr = MUSB_CSR0_TXPKTRDY | (size == 0 ? MUSB_CSR0_P_DATAEND : 0);
        MUSB_WRITE16(MUSB_CSR_EP0, csr);
        if (size > 0)
            OSI_LOOP_WAIT((MUSB_READ16(MUSB_CSR_EP0) & MUSB_CSR0_TXPKTRDY) == 0);
    }
}

static void prvDisconnectISR(dwcUdc_t *dwc)
{
    unsigned i;
    dwc->gadget->speed = USB_SPEED_UNKNOWN;
    dwc->test_mode = 0;
    dwc->connected = 0;
    MUSB_WRITE8(MUSB_INDEX, 0);

    /*endpoint0 handle*/
    MUSB_CLR16(MUSB_INTRTXE, EP0EN);
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0RESET);
    MUSB_WRITE16(MUSB_COUNT_EP0, MUSB_COUNT0RESET);

    for (i = 1; i < dwc->iep_count; ++i)
    {
        prvEpDisableLocked(dwc, &dwc->iep[i]);
        prvEpCancelXferInQueue(&dwc->iep[i]);
    }

    for (i = 1; i < dwc->oep_count; ++i)
    {
        prvEpDisableLocked(dwc, &dwc->oep[i]);
        prvEpCancelXferInQueue(&dwc->oep[i]);
    }

    udcDisconnect(dwc->gadget);
    prvRxFifoFlush(dwc);
    prvTxFifoFlush(dwc);
}

static void prvRxFifoFlush(dwcUdc_t *dwc)
{
    dwcEp_t *ep;
    unsigned i;
    for( i = 1; i < dwc->iep_count; ++i)
    {
        ep = &dwc->iep[i];
        MUSB_SET16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO);
    }
}

static void prvTxFifoFlush(dwcUdc_t *dwc)
{
    unsigned i;
    dwcEp_t *ep;
    for (i = 1; i < dwc->iep_count; ++i)
    {
        ep = &dwc->iep[i];
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO);
    }
}

static void prvEpProgramZlpLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    MUSB_SET8(MUSB_CSRL_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);
    OSI_LOOP_WAIT(MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT);
}

void UdcStartTxDma(dwcUdc_t *dwc, unsigned ep, unsigned size);

static uint32_t prvEpXferStartLocked(dwcUdc_t *dwc, dwcEp_t *ep, void *buf,/* uint32_t offset,*/uint32_t length, bool cached)
{
    //void *rbuf = buf+offset;
    #if 0  //will sync later
    if (cached)
    {
        if (ep->ep.ue.se.dirin)
            //osiDCacheClean(buf, length);
            MMU_DmaCacheSync((uint32)buf, length,DMABUFFER_TO_DEVICE);

        else
            //osiDCacheInvalidate(buf, length);
            MMU_DmaCacheSync((uint32)buf, length,DMABUFFER_FROM_DEVICE);

    }
    #endif

    if (ep->ep.ue.se.dirin && (ep->ep.ue.se.num != 0) &&(dwc->pm_level != DWC_SUSPEND))
    {
        dwc->iep[ep->ep.ue.se.num].xfer_schedule->xfer.buf = buf;
        UdcStartTxDma(dwc, ep->ep.ue.se.num, length);
    }
    else
    {
        if (ep->ep.ue.se.num != 0 && (dwc->pm_level != DWC_SUSPEND))
        {
            dwc->oep[ep->ep.ue.se.num].xfer_schedule->xfer.buf = buf;
            UdcStartRxDma(dwc, ep, length);
        }
    }
    return length;
}

static void prvEp0Zlp(dwcUdc_t *dwc)
{
    dwcEp_t *ep = NULL;
    USB_LOG_TRACE("ep0 zlp status %d", dwc->ep0_state);

    if (dwc->ep0_state == EP0_STATUS_OUT)
        ep = &dwc->oep[0];
    else if (dwc->ep0_state == EP0_STATUS_IN)
        ep = &dwc->iep[0];

    if (ep == NULL)
    {
        USB_ERR_TRACE("[USB][UDC]ep0 zlp status %d mismatch", dwc->ep0_state);
        SCI_ASSERT(0);
    }
    else
    {
        prvEpProgramZlpLocked(dwc, ep);
    }
}

/**
 * Handle GET_STATUS device request.
 */
bool UdcCtrlGetStatus(udc_t *udc, usb_device_request_t *ctrl)
{
    uint16_t reply;
    uint8_t epnum;
     bool is_in;
    if (ctrl->wLength != 2)
        return false;


    switch (ctrl->bRequestType & USB_RECIP_MASK)
    {
    case USB_RECIP_DEVICE:
        reply = 0; // [0]: self power, [1]: remote wakeup
        break;

    case USB_RECIP_INTERFACE:
        reply = 0; // currently, the data result should be zero
        break;

    case USB_RECIP_ENDPOINT:
    {
        epnum = ctrl->wIndex;
        if (epnum == 0)
        {
            reply = 0;
            break;
        }

        is_in = epnum & USB_DIR_IN;
        epnum &= 0x0f;

        musbSelectEp(epnum);
        if (is_in)
            reply = MUSB_READ16(MUSB_TXCSR_EPN(epnum)) & MUSB_TXCSR_P_SENDSTALL ? 1 : 0;
        else
            reply = MUSB_READ16(MUSB_RXCSR_EPN(epnum)) & MUSB_TXCSR_H_RXSTALL ? 1 : 0;
        musbSelectEp(0);
        break;
    }

    default:
        return false;
    }

    UdcEp0Tx(udc, &reply, 2);
    return true;
}

/**
 * Start RX DMA for EP_OUT. This shouldn't be called for EP0. EP0 always
 * use fifo access. The buffer and linklist is inside udc.
 */

PUBLIC __align (64) UdcLinkList_t aligned_rx_linklist[15];     ///RX_dma linklist
PUBLIC __align (64) UdcLinkList_t aligned_tx_linklist[15];     ///TX_dma linklist


PUBLIC __align (64) UdcLinkList_t debug_dma_linklist_rx[2];
PUBLIC __align (64) uint8 Debug_Dmabuf[2048]={0};
void UdcStartRxDma(dwcUdc_t *dwc, dwcEp_t *ep, uint32_t length)
{
    unsigned rx_ch;
    udc_t *udc = dwc->gadget;
    udc->rx_linklist[ep->ep.ue.se.num - 1].addr = (uint32_t)dwc->oep[ep->ep.ue.se.num].xfer_schedule->xfer.buf;
    udc->rx_linklist[ep->ep.ue.se.num - 1].frag_len = 32;
    udc->rx_linklist[ep->ep.ue.se.num - 1].blk_len = length;
    udc->rx_linklist[ep->ep.ue.se.num - 1].link_end = 1;
    udc->rx_linklist[ep->ep.ue.se.num - 1].sp = 0;
    udc->rx_linklist[ep->ep.ue.se.num - 1].ioc = 1;

    //osiDCacheClean(&udc->rx_linklist[ep->ep.ue.se.num - 1], sizeof(udc->rx_linklist[ep->ep.ue.se.num - 1]));
    //osiDCacheInvalidate(dwc->oep[ep->ep.ue.se.num].xfer_schedule->xfer.buf, length);
    MMU_DmaCacheSync((uint32)&udc->rx_linklist[ep->ep.ue.se.num - 1], sizeof(udc->rx_linklist[ep->ep.ue.se.num - 1]),DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)dwc->oep[ep->ep.ue.se.num].xfer_schedule->xfer.buf, length,DMABUFFER_FROM_DEVICE);


    rx_ch = ep->ep.ue.se.num + 15;
    MUSB_SET32(MUSB_DMA_CHN_INTR(rx_ch), CHN_USBRX_INT_EN | CHN_LLIST_INT_EN);
    MUSB_WRITE32(MUSB_DMA_CHN_LLIST_PTR(rx_ch), (uint32_t)&udc->rx_linklist[ep->ep.ue.se.num - 1]);

    
    MUSB_SET32(MUSB_DMA_CHN_CFG(rx_ch), CHN_EN);
    udc->node_start = MUSB_READ32(MUSB_DMA_CHN_LLIST_PTR(rx_ch));
}

/**
 * Start TX DMA for EP_IN. This shouldn't be called for EP0. EP0 always
 * use fifo access. The buffer and linklist is inside udc.
 *
 * Based on the buffer model, it will wait finish inside.
 */

void UdcStartTxDma(dwcUdc_t *dwc, unsigned ep, unsigned size)
{
    udc_t *udc = dwc->gadget;
    unsigned tx_ch = ep;
    udc->tx_linklist[ep - 1].addr = (uint32_t)dwc->iep[ep].xfer_schedule->xfer.buf;
    udc->tx_linklist[ep - 1].frag_len = 32;
    udc->tx_linklist[ep - 1].blk_len = size;
    udc->tx_linklist[ep - 1].link_end = 1;
    udc->tx_linklist[ep - 1].sp = 0;
    udc->tx_linklist[ep - 1].ioc = 1;

    //mark temporary,what this for?
    //osiDCacheClean(&udc->tx_linklist[ep - 1], sizeof(udc->tx_linklist[ep - 1]));
    //osiDCacheClean(dwc->iep[ep].xfer_schedule->xfer.buf, size);
    MMU_DmaCacheSync((uint32)&udc->tx_linklist[ep - 1], sizeof(udc->tx_linklist[ep - 1]),DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)dwc->iep[ep].xfer_schedule->xfer.buf, size,DMABUFFER_TO_DEVICE);


    MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), CHN_USBRX_INT_EN | CHN_LLIST_INT_EN);
    MUSB_WRITE32(MUSB_DMA_CHN_LLIST_PTR(tx_ch), (uint32_t)&udc->tx_linklist[ep - 1]);
    MUSB_SET32(MUSB_DMA_CHN_CFG(tx_ch), CHN_EN);
}

extern unsigned ppp_tx_ep_num;
/*ep xfer directly,without schedule*/
void UdcStartTxDirectly(unsigned ep, void *buf,unsigned size)
{
    dwcUdc_t *dwc = &_the_controller;
    udc_t *udc = dwc->gadget;
    unsigned tx_ch = ep;
    udc->tx_linklist[ep - 1].addr = (uint32_t)buf;
    udc->tx_linklist[ep - 1].frag_len = 32;
    udc->tx_linklist[ep - 1].blk_len = size;
    udc->tx_linklist[ep - 1].link_end = 1;
    udc->tx_linklist[ep - 1].sp = 0;
    udc->tx_linklist[ep - 1].ioc = 1;

    //mark temporary,what this for?
    //osiDCacheClean(&udc->tx_linklist[ep - 1], sizeof(udc->tx_linklist[ep - 1]));
    //osiDCacheClean(dwc->iep[ep].xfer_schedule->xfer.buf, size);
    MMU_DmaCacheSync((uint32)&udc->tx_linklist[ep - 1], sizeof(udc->tx_linklist[ep - 1]),DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)buf, size,DMABUFFER_TO_DEVICE);

    MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), CHN_USBRX_INT_EN | CHN_LLIST_INT_EN);
    MUSB_WRITE32(MUSB_DMA_CHN_LLIST_PTR(tx_ch), (uint32_t)&udc->tx_linklist[ep - 1]);
    MUSB_SET32(MUSB_DMA_CHN_CFG(tx_ch), CHN_EN);
}

void UdcConfigEp(udc_t *udc, dwcEp_t *ep)
{
    if (ep->ep.ue.se.dirin)
    {
        musbSelectEp(ep->ep.ue.se.num);
        MUSB_WRITE8(MUSB_TXFIFOSZ, MUSB_FIFOSZ_DPB | MUSB_FIFOSZ_SIZE_512BYTE); // 2x512
        MUSB_WRITE16(MUSB_TXFIFOADD, MUSB_TXFIFO_STARTADD(ep->ep.ue.se.num));         // address
        MUSB_WRITE16(MUSB_TXMAXP_EPN(ep->ep.ue.se.num), udc->speed == USB_SPEED_HIGH ? 512 : 64);
        if ((ep->type == USB_ENDPOINT_XFER_BULK) || (ep->type == USB_ENDPOINT_XFER_INT))
            MUSB_WRITE16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_AUTOSET |
                                                         MUSB_TXCSR_DMAENAB |
                                                         MUSB_TXCSR_DMAMODE |
                                                         MUSB_TXCSR_CLRDATATOG |
                                                         MUSB_TXCSR_FLUSHFIFO);
        if (ep->type == USB_ENDPOINT_XFER_ISOC)
            MUSB_WRITE16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_AUTOSET |
                                                         MUSB_TXCSR_P_ISO |
                                                         MUSB_TXCSR_DMAENAB |
                                                         MUSB_TXCSR_DMAMODE |
                                                         MUSB_TXCSR_CLRDATATOG |
                                                         MUSB_TXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO); // flush fifo twice
        MUSB_SET16(MUSB_INTRTXE, (1 << ep->ep.ue.se.num) | 1);
        USB_LOG_TRACE("UdcConfigEp ep->ep.dirin ep->type=%d, ep->ep.address=%x\n", ep->type, ep->ep.ue.address);
    }
    else
    {
        musbSelectEp(ep->ep.ue.se.num);
        MUSB_WRITE8(MUSB_RXFIFOSZ, MUSB_FIFOSZ_DPB | MUSB_FIFOSZ_SIZE_512BYTE); // 2x512
        MUSB_WRITE16(MUSB_RXFIFOADD, MUSB_RXFIFO_STARTADD(ep->ep.ue.se.num));         // address
        MUSB_WRITE16(MUSB_RXMAXP_EPN(ep->ep.ue.se.num), udc->speed == USB_SPEED_HIGH ? 512 : 64);
        MUSB_CLR16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_RXPKTRDY);
        if ((ep->type == USB_ENDPOINT_XFER_BULK) || (ep->type == USB_ENDPOINT_XFER_INT))
            MUSB_WRITE16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_AUTOCLEAR |
                                                         MUSB_RXCSR_DMAENAB |
                                                         MUSB_RXCSR_DMAMODE |
                                                         MUSB_RXCSR_CLRDATATOG |
                                                         MUSB_RXCSR_FLUSHFIFO);
        if (ep->type == USB_ENDPOINT_XFER_ISOC)
            MUSB_WRITE16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_AUTOCLEAR |
                                                         MUSB_RXCSR_P_ISO |
                                                         MUSB_RXCSR_DMAENAB |
                                                         MUSB_RXCSR_DMAMODE |
                                                         MUSB_RXCSR_CLRDATATOG |
                                                         MUSB_RXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO); // flush fifo twice
        MUSB_SET16(MUSB_INTRRXE, 1 << ep->ep.ue.se.num);
        USB_LOG_TRACE("UdcConfigEp ep->ep.dirout ep->type=%d, ep->ep.address=%x\n", ep->type, ep->ep.ue.address);
    }
}

/**
 * @brief process set feature
 *
 * @return
 *      - (-1)  fail
 *      - 0     need process by the bind driver
 *      - other process complete
 */
static void prvProcessSetFeature(dwcUdc_t *dwc, const usb_device_request_t *ctrl)
{
    uint16_t index = ctrl->wIndex;
    uint8_t i;
    REG_ANALOG_G2_ANALOG_USB20_USB20_TRIMMING_T analog_usb20_usb20_trimming;
    analog_usb20_usb20_trimming.v = hwp_analogG2->analog_usb20_usb20_trimming;
    analog_usb20_usb20_trimming.b.analog_usb20_usb20_tunehsamp = 0x2;
    analog_usb20_usb20_trimming.b.analog_usb20_usb20_tfregres = 0x1E;
    hwp_analogG2->analog_usb20_usb20_trimming = analog_usb20_usb20_trimming.v;

    musbSelectEp(0);
    switch (index >> 8)
    {
    case TEST_J:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_J);
        break;
    case TEST_K:
        //osiDelayUS(10000);
        OS_TickDelay(333);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_K);
        break;
    case TEST_SE0_NAK:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_SE0_NAK);
        break;
    case TEST_PACKET:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        for ( i = 0; i < 53; i++)
            *(volatile unsigned char *)Mentor_USB_EP0_FIFO = musb_test_packet[i];
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_PACKET);
        MUSB_SET8(MUSB_CSRL_EP0, 0x2);
        break;
    case 0xc0:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_FORCE_HS);
        break;
    case 0xc1:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_FORCE_FS);
        break;
    case 0xc2:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        for ( i = 0; i < 40; i++)
            *(volatile unsigned char *)Mentor_USB_EP0_FIFO = musb_test_fifo[i];
        MUSB_SET8(MUSB_CSRL_EP0, 0x2);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_FIFO_ACCESS);
        break;
    case 0xc3:
        //osiDelayUS(10000);
        SCI_Sleep(10);
        MUSB_WRITE8(MUSB_TESTMODE, MUSB_TEST_FORCE_HOST);
        break;

    default:
        break;
    }
    dwc->test_mode = 1;
}

static void prvProcessControl(dwcUdc_t *dwc, usb_device_request_t *ctrl)
{
    ep0State_t next_state;
    int r = 0;
    udevDrv_t *drv1;
    udevDrv_t *drv;
    if (ctrl->wLength == 0)
        next_state = EP0_STATUS_IN;
    else if (USB_EP_GET_DIR(ctrl->bRequestType) == USB_DIR_IN)
        next_state = EP0_DATA_IN;
    else
        next_state = EP0_DATA_OUT;
    prvChangeEp0State(dwc, next_state);
    drv1 = udcGetDriver(dwc->gadget);

    if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD)
    {
        switch (ctrl->bRequest)
        {
        case USB_REQ_SET_ADDRESS:
            USB_LOG_TRACE("usb set address %u", ctrl->wValue);
            if (!(dwc->ep0_state & EP0_STATE_DATA_MASK))
                prvEp0Zlp(dwc);
            dwc->connected = 1;
            prvSetAddress(dwc, ctrl->wValue);
            udcConnect(dwc->gadget);
            r = 1;
            break;

        case USB_REQ_GET_STATUS:
            USB_LOG_TRACE("USB_REQ_GET_STATUS\n");
            r = UdcCtrlGetStatus(drv1->udc, ctrl);
            break;

        case USB_REQ_CLEAR_FEATURE:
            //clear featueËµÃ÷usb³öÏÖÁËÒì³££¬ÖÐ¶Ï´¦Àí³ÌÐòÀï²»Ó¦¸Ã³öÏÖËÀµÈ
            //MUSB_SET8(MUSB_CSRL_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);
            //OSI_LOOP_WAIT(MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT);
            r = 1;
            break;

        case USB_REQ_SET_FEATURE:
            USB_LOG_TRACE("usb enter test mode (%x/%x/%x/%x/%x)", ctrl->bRequestType, ctrl->bRequest, ctrl->wValue, ctrl->wIndex, ctrl->wLength);
            MUSB_SET8(MUSB_CSRL_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);
            prvProcessSetFeature(dwc, ctrl);
            r = 1;
            break;

        default:
            break;
        }
    }
    if (r == 0)
    {
        drv = udcGetDriver(dwc->gadget);
        if (drv)
        {
            if (((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS) && (ctrl->wLength != 0))
                MUSB_SET16(MUSB_CSR_EP0, MUSB_TXCSR_CLRDATATOG);
            r = udevDriverSetup(drv, ctrl);
            if (!(dwc->ep0_state & EP0_STATE_DATA_MASK))
                prvEp0Zlp(dwc);
        }
        else
        {
            r = -1;
        }
    }

    if (r < 0)
        goto stall;

    return;

stall:
    USB_ERR_TRACE("[USB][UDC]control fail, stall ep. (%x/%x/%x/%x/%x)",
             ctrl->bRequestType, ctrl->bRequest, ctrl->wValue,
             ctrl->wIndex, ctrl->wLength);

    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_SENDSTALL);
}

static void prvDwcRmtWakeup_(dwcUdc_t *dwc)
{
    if (dwc->pm_level == DWC_SUSPEND)
    {
        prvExitHibernation(dwc);
        udcResume(dwc->gadget);
        dwc->pm_level = DWC_NORMAL;
    }
}

static void prvTxFifoFreeLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    // give back fifo to the queue head (LRU), but do not set
    // ep->fifo to NULL and not set fifo->last_ep for quick allocating
    if (ep->fifo)
    {
        ep->fifo->using_ep = 0;
        TAILQ_INSERT_HEAD(&dwc->avail_txfifo, ep->fifo, iter);
    }
}

static void prvPendingEpLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    if (ep->pending == 0)
    {
        ep->pending = 1;
        STAILQ_INSERT_TAIL(&dwc->pending_eps, ep, iter);
    }
}

static void prvScheduleEpLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    dwcXfer_t *dx;
    uint32_t size;
    if (ep->xfer_schedule){
        USB_ERR_TRACE("[USB][UDC]ep xfer_schedule busy  (ep: %x)\n", ep->ep.ue.address);
        return;
    }

    dx = TAILQ_FIRST(&ep->xfer_q);
    if (dx == NULL){
        
        //USB_ERR_TRACE("[USB][UDC]ep xfer_q is empty  (ep: %x)\n", ep->ep.ue.address);
        return;
    }

    if (dwc->disable_schedule && ep->ep.ue.se.num != 0)
    {
        //USB_LOG_TRACE("[USB][UDC]ep pending (ep: %x)\n", ep->ep.ue.address);
        prvPendingEpLocked(dwc, ep);
        return;
    }

    if (dwc->pm_level == DWC_SUSPEND && ep->ep.ue.se.dirin && (dwc->gadget->feature & UDC_FEATURE_WAKEUP_ON_WRITE))
        prvDwcRmtWakeup_(dwc);

    TAILQ_REMOVE(&ep->xfer_q, dx, anchor);
    ep->xfer_schedule = dx;
    size = dx->xfer.length;
    if (size > ep->dma_max)
        size = ep->dma_max;
    
    dx->xfer_size = size;
    prvEpXferStartLocked(dwc, ep, dx->xfer.buf, size, !dx->xfer.uncached);
}

static int prvEpQueueLocked(dwcUdc_t *dwc, dwcEp_t *ep, dwcXfer_t *dx)
{
    if (unlikely(dx->occupied))
    {
        //USB_LOG_TRACE("[USB][UDC]ep queue busy. (ep: %x, ep0_state: %d)\n", ep->ep.ue.address, dwc->ep0_state);
        return -EBUSY;
    }

    if (ep->ep.ue.se.dirin && dx->xfer.zlp)
        dx->zero = (dx->xfer.length % ep->ep.mps) == 0 ? 1 : 0;

    dx->xfer.status = -EINPROGRESS;
    dx->xfer.actual = 0;
    dx->occupied = 1;

    TAILQ_INSERT_TAIL(&ep->xfer_q, dx, anchor);
    prvScheduleEpLocked(dwc, ep);

    return 0;
}

static void prvStallEp0(dwcUdc_t *dwc, dwcEp_t *ep)
{
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_SENDSTALL);
}

static int prvStallEpLocked(dwcUdc_t *dwc, dwcEp_t *ep, bool halt)
{
    if (ep->ep.ue.se.num == 0)
    {
        if (halt) // can not clear ep0
            prvStallEp0(dwc, ep);

        return 0;
    }

    if (ep->type == USB_ENDPOINT_XFER_ISOC)
        return -EINVAL;

    if (ep->ep.ue.se.dirin)
    {
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO);
        MUSB_CLR16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_TXPKTRDY);

        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_P_SENTSTALL |
                                                   MUSB_TXCSR_P_SENDSTALL);
    }
    else
    {
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO);
        MUSB_CLR16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_RXPKTRDY);

        MUSB_SET16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_P_SENTSTALL |
                                                   MUSB_RXCSR_P_SENDSTALL);
    }

    return 0;
}

static void prvDwcEpHalt(udc_t *udc, usbEp_t *ep_, bool halt)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    dwcEp_t *ep = EP2DWCEP(ep_);
    prvStallEpLocked(dwc, ep, halt);
}

static void prvEpCancelXferInQueue(dwcEp_t *ep)
{
    dwcXfer_t *dx;
    usbXfer_t *x;

    while ((dx = TAILQ_FIRST(&ep->xfer_q)))
    {
        TAILQ_REMOVE(&ep->xfer_q, dx, anchor);
        dx->occupied = 0;
        x = &dx->xfer;
        x->status = -ECANCELED;
        if (x->complete)
            x->complete(&ep->ep, x);
    }
}

static void prvEpStopLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    if (ep->xfer_schedule)
    {
        TAILQ_INSERT_HEAD(&ep->xfer_q, ep->xfer_schedule, anchor);
        ep->xfer_schedule = NULL;
    }
}

static void prvEpDisableLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    if (ep->ep.ue.se.dirin)
        MUSB_CLR16(MUSB_INTRTXE, (1 << ep->ep.ue.se.num));
    else
        MUSB_CLR16(MUSB_INTRRXE, (1 << ep->ep.ue.se.num));

    prvEpStopLocked(dwc, ep);
    if (ep->ep.ue.se.dirin)
    {
        prvTxFifoFreeLocked(dwc, ep);
        ep->fifo = NULL;
        MUSB_WRITE16(MUSB_TXMAXP_EPN(ep->ep.ue.se.num), MUSB_TXMAXPRESET);
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_TXCSR_EPN(ep->ep.ue.se.num), MUSB_TXCSR_FLUSHFIFO);
    }
    else
    {
        MUSB_WRITE16(MUSB_RXMAXP_EPN(ep->ep.ue.se.num), MUSB_RXMAXPRESET);
        MUSB_SET16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_RXCSR_EPN(ep->ep.ue.se.num), MUSB_RXCSR_FLUSHFIFO);
    }
}

static int prvEpEnableLocked(dwcUdc_t *dwc, dwcEp_t *ep)
{
    udc_t *udc = dwc->gadget;
    USB_LOG_TRACE("[USB][UDC]prvEpEnableLocked ep = %x enable\n", ep->ep.ue.address);

    UdcConfigEp(udc, ep);
    musbSelectEp(0);
    return true;
}

static void prvGiveBackComplete_(dwcUdc_t *dwc, dwcEp_t *ep, dwcXfer_t *dx, bool schedule)
{
    usbXfer_t *xfer ;
    if (dx)
    {
        dx->occupied = 0;
        xfer = &dx->xfer;
        if (xfer->complete)
            xfer->complete(&ep->ep, xfer);
    }

    if (schedule)
    {
        prvScheduleEpLocked(dwc, ep);
    }
}

static void prvGiveBackComplete(dwcUdc_t *dwc, dwcEp_t *ep, dwcXfer_t *dx)
{
    prvGiveBackComplete_(dwc, ep, dx, true);
}

static void prvGiveBackEpCurrent(dwcUdc_t *dwc, dwcEp_t *ep)
{
    dwcXfer_t *dx = ep->xfer_schedule;
    ep->xfer_schedule = NULL;

   if (!ep->ep.ue.se.dirin && !dx->xfer.uncached)
        MMU_DmaCacheSync((uint32)dx->xfer.buf, dx->xfer.actual,DMABUFFER_FROM_DEVICE);

    if((dwc->pm_level != DWC_SUSPEND))
        prvGiveBackComplete(dwc, ep, dx);
}

static void prvEpInDoneISR(dwcUdc_t *dwc, dwcEp_t *ep)
{
    bool transfer_done;
    uint32_t size;
    dwcXfer_t *dx = ep->xfer_schedule;
    if (dx == NULL)
    {
        USB_ERR_TRACE("[USB][UDC]ep %x done no transfer. (ctrl stage %d)", ep->ep.ue.address, dwc->ep0_state);
        //SCI_ASSERT(0);
        return;
    }

    dx->xfer.actual += dx->xfer_size;
    transfer_done = (dx->xfer.length == dx->xfer.actual);
    if (!transfer_done)
    {
        size = dx->xfer.length - dx->xfer.actual;
        USB_LOG_TRACE("[USB][UDC]tx continue:ep %x ,remain size:%d ", ep->ep.ue.address,size);
        
        SCI_ASSERT(0);
        
        if (size > ep->dma_max)
            size = ep->dma_max;
        dx->xfer_size = size;
        prvEpXferStartLocked(dwc, ep, (uint8_t *)dx->xfer.buf + dx->xfer.actual, size, !dx->xfer.uncached);
    }
    else
    {
        if (dx->zero)
        {
            dx->zero = 0;
            dx->xfer_size = 0;
            UdcStartTxDma(dwc, ep->ep.ue.se.num, 0);
        }
        else
        {
            dx->xfer.status = 0;
            dx->xfer_size = 0;
            prvGiveBackEpCurrent(dwc, ep);
        }
    }
    return;
}

static void prvEpOutDoneISR(dwcUdc_t *dwc, dwcEp_t *ep)
{
    dwcXfer_t *dx = ep->xfer_schedule;
    if (dx == NULL)
    {
        USB_ERR_TRACE("[USB][UDC]ep %x done no transfer. (ctrl stage %d)", ep->ep.ue.address, dwc->ep0_state);
        return;
    }

    dx->xfer.status = 0;
    dx->xfer_size = 0;
    dx->xfer.actual = dwc->gadget->rx_linklist[ep->ep.ue.se.num - 1].blk_len - ((MUSB_READ32(MUSB_DMA_CHN_LEN(ep->ep.ue.se.num + 15)) & 0xFFFF0000) >> 16);
    prvGiveBackEpCurrent(dwc, ep);
    return;
}

static void prvExitHibernation(dwcUdc_t *dwc)
{
    udcPlatResume(dwc->gadget);
    //osiDelayUS(10);
     OS_TickDelay(1);

}

static void prvResetISR(dwcUdc_t *dwc)
{
    MUSB_WRITE8(MUSB_FADDR, 0);

#ifdef FORCE_FULL_SPEED
    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN);
    MUSB_CLR8(MUSB_POWER, MUSB_POWER_HSENAB);
#else
    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN | MUSB_POWER_HSENAB);
#endif

    MUSB_WRITE8(MUSB_INTRUSBE, MUSB_INTR_RESET | MUSB_INTR_RESUME | MUSB_INTR_SUSPEND);
    MUSB_WRITE16(MUSB_INTRTXE, EP0EN);
}

/**
 * Read from EP fifo. This should be called only for EP0.
 * Hardware doesn't support EP fifo access except EP0.
 */
void musbReadEP0Fifo(unsigned ep, void *data, unsigned size)
{
    uint8_t *pdata = (uint8_t *)data;
    unsigned n;
    for ( n = 0; n < size; n++)
        *pdata++ = MUSB_READ8(MUSB_EP0_FIFO + 4 * ep);
}

static void prvEpISR(dwcUdc_t *dwc, dwcEp_t *ep, uint16_t count)
{
    uint8_t *data;
    usbXfer_t *xfer;
    uint8_t i;
    dwcEp_t *ep0_out;
    dwcXfer_t *dx;
    if (count == sizeof(usb_device_request_t))
    {
        musbReadEP0Fifo(0, &ctrl, sizeof(usb_device_request_t));
        prvProcessControl(dwc, &ctrl);
    }
    else if ((ctrl.bRequestType & USB_DIR_IN) == USB_DIR_OUT)
    {
        if (ctrl.wLength != 0)
        {
            data = malloc(count);
            for ( i = 0; i < count; i++)
                data[i] = MUSB_READ8(MUSB_EP0_FIFO);
            MUSB_SET16(MUSB_CSR_EP0, MUSB_TXCSR_CLRDATATOG);

            ep0_out = &dwc->oep[0];
            ep0_out->xfer_schedule->xfer.buf = (void *)data;
            dx = ep->xfer_schedule;

            if (dx)
            {
                dx->occupied = 0;
                xfer = &dx->xfer;
                xfer->actual = ctrl.wLength;
                if (xfer->complete)
                    xfer->complete(&ep->ep, xfer);
            }
            free(data);
        }
    }
}

void musb_rx_handle(dwcUdc_t *dwc, uint32_t rx_ch)
{
    //MUSB_SET32(MUSB_DMA_CHN_INTR(rx_ch), CHN_USBRX_LAST_INT_CLR | CHN_LLIST_INT_CLR);//move to prvDwcIsr
    prvEpOutDoneISR(dwc, &(dwc->oep[rx_ch - 15]));
}

void musb_tx_handle(dwcUdc_t *dwc, uint32_t tx_ch)
{
    //MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), CHN_LLIST_INT_CLR);//move to prvDwcIsr
    #ifdef PPP_USB
    if(ppp_tx_ep_num == tx_ch)
    {
        //ppp_tx_ep_num == 0xff;
        ppp_clear_send_status();
        return;
    }
    #endif
    prvEpInDoneISR(dwc, &(dwc->iep[tx_ch]));
}

static void prvEnterHibernation(dwcUdc_t *dwc)
{
    // clear any pending interrupts, since dwc2 will not able to
    // clear them after entering hibernation
    // MUSB_WRITE8(MUSB_INTRUSBE, 0);

    //osiDelayUS(10);
    OS_TickDelay(1);

    udcPlatSuspend(dwc->gadget);
}

//suspend means usb cable plugout
static void prvSuspendISR(dwcUdc_t *dwc)
{
    udcSuspend(dwc->gadget);
    dwc->pm_level = DWC_SUSPEND;
    prvEnterHibernation(dwc);

    hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0);
    //osiDelayUS(1000);
    SCI_Sleep(1);

}
bool dwcUsbIsSuspend(udc_t *udc)
{
    dwcUdc_t *dwc;
    if(!udc)
        return false;
    dwc = prvGadgetGetDwc(udc);
    return (dwc->pm_level == DWC_SUSPEND);

}


static void prvResumeISR(dwcUdc_t *dwc)
{
    MUSB_WRITE8(MUSB_INTRUSBE, MUSB_INTR_RESET | MUSB_INTR_RESUME | MUSB_INTR_SUSPEND);
    prvExitHibernation(dwc);
    udcResume(dwc->gadget);
    dwc->pm_level = DWC_NORMAL;
}

static void prvSetEpMps(dwcUdc_t *dwc, dwcEp_t *ep, uint16_t mps)
{
    uint32_t max_dma_size = 0xffc0;
    max_dma_size -= (max_dma_size % mps);

    ep->ep.mps = mps;
    ep->dma_max = max_dma_size;
}
uint8_t g_usbrst_count = 0;

void prvDwcPollingISR(uint32 irq_num)
{
    dwcUdc_t *dwc = &_the_controller;
    dwcEp_t *ep;
    uint32_t critical;
    unsigned retry_cnt = 3;
    uint16_t csr0 ;
    uint16_t count0;
    uint16_t ep_mps = 0;
    uint32_t i;
    uint16_t tx_en;
    uint16_t rx_en;
    volatile unsigned int_status;
    uint32 isr_status = 0;

    uint8_t musb_intrusb = 0;

    musb_intrusb = MUSB_READ8(MUSB_INTRUSB);
isr_retry:
    if (musb_intrusb & MUSB_INTR_RESUME)
    {
        //to do
        USB_LOG_TRACE("[USB][UDC_poll]usb irq, resume 0x%x\n", musb_intrusb);
        if (dwc->pm_level == DWC_SUSPEND)
        {
            //(REG_SYS_CTRL_AON_CLOCK_EN1_T)(hwp_sysCtrl->aon_clock_en1).b.usb_ref_en = 1;

            REGT_FIELD_CHANGE1(hwp_sysCtrl->aon_clock_en1,
                              REG_SYS_CTRL_AON_CLOCK_EN1_T, usb_ref_en, 1);

            hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
            //osiDelayUS(1000);
            SCI_Sleep(1);
            prvResumeISR(dwc);
        }
        else
        {
            // abnormal state, reset the controller
            prvHwInit(dwc, true);

        }
        return;
    }

    if (musb_intrusb & MUSB_INTR_SUSPEND)
    {
        USB_LOG_TRACE("[USB][UDC_poll]usb irq, suspend dwc->connected:%d\n", dwc->connected);
        if (!!dwc->connected)
            prvSuspendISR(dwc);
        else if(g_usbrst_count == 4)
        {
            g_usbrst_count = 0;
            drvUsbStop();
            drvUsbRestart();
        }
        return;
    }

    if (musb_intrusb & MUSB_INTR_RESET)
    {
        USB_LOG_TRACE("[USB][UDC_poll]usb irq, usbrst g_usbrst_count: %d\n", g_usbrst_count);
        g_usbrst_count++;
        /*
        if (dwc->pm_level == DWC_SUSPEND)
        {
            prvExitHibernation(dwc);
            drvUsbReset();
        }
        */
        prvDisconnectISR(dwc);
        hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
        prvResetISR(dwc);


        if (MUSB_READ8(MUSB_POWER) & MUSB_POWER_HSMODE)
        {
            ep_mps = DWC_EP_MPS_HIGH;
            dwc->gadget->speed = USB_SPEED_HIGH;
        }
        else
        {
            ep_mps = DWC_EP_MPS_FULL;
            dwc->gadget->speed = USB_SPEED_FULL;
        }
        dwc->gadget->ep0->mps = DWC_EP0_MPS;
        for ( i = 1; i < dwc->iep_count; ++i)
            prvSetEpMps(dwc, &dwc->iep[i], ep_mps);
        for ( i = 1; i < dwc->oep_count; ++i)
            prvSetEpMps(dwc, &dwc->oep[i], ep_mps);
        USB_LOG_TRACE(" [USB][UDC_poll]pusbResetISR dwc->gadget->speed = %d\n", dwc->gadget->speed);
    }

    if (MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT)
    {
        //USB_LOG_TRACE("usb irq, ep0\n");
        musbSelectEp(0);

        csr0 = MUSB_READ16(MUSB_CSR_EP0);
        count0 = MUSB_READ16(MUSB_COUNT_EP0);

        if (csr0 & MUSB_CSR0_P_DATAEND) // It is set by CPU, and wait auto clear
        {
            return;
        }

        if (csr0 & MUSB_CSR0_P_SENTSTALL) // clear SENTSTALL
        {
            csr0 &= ~MUSB_CSR0_P_SENTSTALL;
            MUSB_WRITE16(MUSB_CSR_EP0, csr0);
        }

        if (csr0 & MUSB_CSR0_P_SETUPEND) // clear SETUPEND
        {
            MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDSETUPEND);
        }

        if (csr0 & MUSB_CSR0_RXPKTRDY)
        {
            prvEpISR(dwc, &dwc->iep[0], count0);
            return;
        }
    }

    //dwc->disable_schedule = 1;

    int_status = 0;
    tx_en = MUSB_READ16(MUSB_INTRTXE);
    for (i = 1; i < 16; i++)
    {
        isr_status = 0;
        if (tx_en & (1 << i))
        {
            musbSelectEp(i);
            int_status = MUSB_READ32(MUSB_DMA_CHN_INTR(i));
            if ((int_status & CHN_LLISTINT_MASK_STATUS) == CHN_LLISTINT_MASK_STATUS)
            {
                MUSB_SET32(MUSB_DMA_CHN_INTR(i), CHN_LLIST_INT_CLR);
                musb_tx_handle(dwc, i);

            }
        }
    }

    int_status = 0;
    rx_en = MUSB_READ16(MUSB_INTRRXE);
    for (i = 1; i < 16; i++)
    {
        isr_status = 0;
        if (rx_en & (1 << i))
        {
            musbSelectEp(i);
            int_status = MUSB_READ32(MUSB_DMA_CHN_INTR(i + 15));
            if (((int_status & CHN_LLISTINT_MASK_STATUS) == CHN_LLISTINT_MASK_STATUS) || ((int_status & CHN_USBRX_INT_MASK_STATUS) == CHN_USBRX_INT_MASK_STATUS))
            {  
                MUSB_SET32(MUSB_DMA_CHN_INTR(i + 15), CHN_USBRX_LAST_INT_CLR | CHN_LLIST_INT_CLR);
                musb_rx_handle(dwc, i + 15);
            }
        }
    }

/*
    dwc->disable_schedule = 0;
    while ((ep = STAILQ_FIRST(&dwc->pending_eps)) != NULL)
    {
        ep->pending = 0;
        STAILQ_REMOVE_HEAD(&dwc->pending_eps, iter);
        prvScheduleEpLocked(dwc, ep);
        //OSI_NOP; // a while
        __asm
        {
            nop
        }
    },
*/
    if (retry_cnt-- > 0)
        goto isr_retry;

}


static void GxUDCHisrFunc (uint32 i, void *d)
{
    dwcUdc_t *dwc = &_the_controller;
    dwcEp_t *ep;
    int32 int_ep = 0;

    while (!threadReadBuffer (&int_ep))
    {
        if(USB_DIR_IN == (int_ep&0xff)) //TX
        {
            musb_tx_handle(dwc, (int_ep>>16)&0xff );
        }
        else //RX
        {
            musb_rx_handle(dwc, (int_ep>>16)&0xff );
        }
    }

    dwc->disable_schedule = 0;
    while ((ep = STAILQ_FIRST(&dwc->pending_eps)) != NULL)
    {
        ep->pending = 0;
        STAILQ_REMOVE_HEAD(&dwc->pending_eps, iter);
        prvScheduleEpLocked(dwc, ep);
        //OSI_NOP; // a while
        __asm
        {
            nop
        }
    }
    CHIPDRV_EnableIRQINT (TB_USB_INT);
}

static ISR_EXE_T prvDwcISR(uint32 irq_num)
{
    dwcUdc_t *dwc = &_the_controller;
    dwcEp_t *ep;
    uint32_t critical;
    unsigned retry_cnt = 1;
    uint16_t csr0 ;
    uint16_t count0;
    uint16_t ep_mps = 0;
    uint32_t i;
    uint16_t tx_en;
    uint16_t rx_en;
    volatile unsigned int_status;
    uint32 isr_status = 0;

    uint8_t musb_intrusb = 0;

    CHIPDRV_DisableIRQINT(TB_USB_INT);
    //if usb ip is not enabled,need return directly,otherwise will system cause abort when read register
    musb_intrusb = MUSB_READ8(MUSB_INTRUSB);
isr_retry:
    if (musb_intrusb & MUSB_INTR_RESUME)
    {
        //to do
        USB_LOG_TRACE("[USB][UDC]usb irq, resume 0x%x\n", musb_intrusb);
        if (dwc->pm_level == DWC_SUSPEND)
        {
            //(REG_SYS_CTRL_AON_CLOCK_EN1_T)(hwp_sysCtrl->aon_clock_en1).b.usb_ref_en = 1;

            REGT_FIELD_CHANGE1(hwp_sysCtrl->aon_clock_en1,
                              REG_SYS_CTRL_AON_CLOCK_EN1_T, usb_ref_en, 1);

            hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
            //osiDelayUS(1000);
            SCI_Sleep(1);
            prvResumeISR(dwc);
        }
        else
        {
            // abnormal state, reset the controller
            prvHwInit(dwc, true);

        }
        goto isr_usbdone;
    }

    if (musb_intrusb & MUSB_INTR_SUSPEND)
    {
        USB_LOG_TRACE("[USB][UDC]usb irq, suspend dwc->connected:%d\n", dwc->connected);
        if (!!dwc->connected)
            prvSuspendISR(dwc);
        else if(g_usbrst_count == 4)
        {
            g_usbrst_count = 0;
            drvUsbStop();
            drvUsbRestart();
        }
        goto isr_usbdone;
    }

    if (musb_intrusb & MUSB_INTR_RESET)
    {
        USB_LOG_TRACE("[USB][UDC]usb irq, usbrst g_usbrst_count: %d\n", g_usbrst_count);
        g_usbrst_count++;
/*
        if (dwc->pm_level == DWC_SUSPEND)
        {
            prvExitHibernation(dwc);
            drvUsbReset();
        }
*/
        prvDisconnectISR(dwc);
        hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
        prvResetISR(dwc);


        if (MUSB_READ8(MUSB_POWER) & MUSB_POWER_HSMODE)
        {
            ep_mps = DWC_EP_MPS_HIGH;
            dwc->gadget->speed = USB_SPEED_HIGH;
        }
        else
        {
            ep_mps = DWC_EP_MPS_FULL;
            dwc->gadget->speed = USB_SPEED_FULL;
        }
        dwc->gadget->ep0->mps = DWC_EP0_MPS;
        for ( i = 1; i < dwc->iep_count; ++i)
            prvSetEpMps(dwc, &dwc->iep[i], ep_mps);
        for ( i = 1; i < dwc->oep_count; ++i)
            prvSetEpMps(dwc, &dwc->oep[i], ep_mps);
        USB_LOG_TRACE("[USB][UDC] pusbResetISR dwc->gadget->speed = %d\n", dwc->gadget->speed);
    }

    if (MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT)
    {
        //USB_LOG_TRACE("usb irq, ep0\n");
        musbSelectEp(0);

        csr0 = MUSB_READ16(MUSB_CSR_EP0);
        count0 = MUSB_READ16(MUSB_COUNT_EP0);

        if (csr0 & MUSB_CSR0_P_DATAEND) // It is set by CPU, and wait auto clear
        {
            goto isr_usbdone;
        }

        if (csr0 & MUSB_CSR0_P_SENTSTALL) // clear SENTSTALL
        {
            csr0 &= ~MUSB_CSR0_P_SENTSTALL;
            MUSB_WRITE16(MUSB_CSR_EP0, csr0);
        }

        if (csr0 & MUSB_CSR0_P_SETUPEND) // clear SETUPEND
        {
            MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDSETUPEND);
        }

        if (csr0 & MUSB_CSR0_RXPKTRDY)
        {
            prvEpISR(dwc, &dwc->iep[0], count0);
            goto isr_usbdone;
        }
    }

    dwc->disable_schedule = 1;

    int_status = 0;
    tx_en = MUSB_READ16(MUSB_INTRTXE);
    for (i = 1; i < 16; i++)
    {
        isr_status = 0;
        if (tx_en & (1 << i))
        {
            musbSelectEp(i);
            int_status = MUSB_READ32(MUSB_DMA_CHN_INTR(i));
            if ((int_status & CHN_LLISTINT_MASK_STATUS) == CHN_LLISTINT_MASK_STATUS)
            {
                //musb_tx_handle(dwc, i);
                MUSB_SET32(MUSB_DMA_CHN_INTR(i), CHN_LLIST_INT_CLR);
                isr_status |= (i << 16);  //save ep_id
                isr_status |= USB_DIR_IN;
                IsrWriteBuffer (isr_status);
                goto isr_callHisr;
            }
        }
    }

    int_status = 0;
    rx_en = MUSB_READ16(MUSB_INTRRXE);
    for (i = 1; i < 16; i++)
    {
        isr_status = 0;
        if (rx_en & (1 << i))
        {
            musbSelectEp(i);
            int_status = MUSB_READ32(MUSB_DMA_CHN_INTR(i + 15));
            if (((int_status & CHN_LLISTINT_MASK_STATUS) == CHN_LLISTINT_MASK_STATUS) || ((int_status & CHN_USBRX_INT_MASK_STATUS) == CHN_USBRX_INT_MASK_STATUS))
            {  //musb_rx_handle(dwc, i + 15);
                MUSB_SET32(MUSB_DMA_CHN_INTR(i+15), CHN_USBRX_LAST_INT_CLR | CHN_LLIST_INT_CLR);
                isr_status |= ((i+15) << 16);
                isr_status |= USB_DIR_OUT;
                IsrWriteBuffer (isr_status);
                goto isr_callHisr;
            }
        }
    }


    dwc->disable_schedule = 0;
    while ((ep = STAILQ_FIRST(&dwc->pending_eps)) != NULL)
    {
        ep->pending = 0;
        STAILQ_REMOVE_HEAD(&dwc->pending_eps, iter);
        prvScheduleEpLocked(dwc, ep);
        //OSI_NOP; // a while
        __asm
        {
            nop
        }
    }
    if (retry_cnt-- > 0)
        goto isr_retry;

isr_usbdone:
    CHIPDRV_EnableIRQINT (TB_USB_INT);
    return ISR_DONE;
isr_callHisr:
    return CALL_HISR;
}

static bool prvDwcRmtWakeup(udc_t *udc)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    //(REG_SYS_CTRL_AON_CLOCK_EN1_T)(hwp_sysCtrl->aon_clock_en1).b.usb_ref_en = 1;

    REGT_FIELD_CHANGE1(hwp_sysCtrl->aon_clock_en1, \
                      REG_SYS_CTRL_AON_CLOCK_EN1_T, usb_ref_en, 1);

    hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
    //osiDelayUS(1000);
    SCI_Sleep(1);
    MUSB_SET8(MUSB_POWER, MUSB_POWER_RESUME);
    //osiDelayUS(10000);
    SCI_Sleep(10);
    MUSB_CLR8(MUSB_POWER, MUSB_POWER_RESUME);
    prvDwcRmtWakeup_(dwc);
    return true;
}

static void prvHwInit(dwcUdc_t *dwc, bool reset_int)
{
    if (!reset_int)
        return;
    USB_LOG_TRACE("[USB][UDC]prvHwInit\n");

#ifdef FORCE_FULL_SPEED
    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN | MUSB_POWER_ENSUSPEND);
    MUSB_CLR8(MUSB_POWER, MUSB_POWER_HSENAB);
#else
    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN | MUSB_POWER_HSENAB | MUSB_POWER_ENSUSPEND);
#endif

    MUSB_SET16(MUSB_INTRTXE, EP0EN);

    MUSB_WRITE8(MUSB_INTRUSBE, 0);
    MUSB_WRITE8(MUSB_INTRUSBE, MUSB_INTR_RESET | MUSB_INTR_RESUME | MUSB_INTR_SUSPEND);
    //(REG_SYS_CTRL_AON_CLOCK_EN1_T)(hwp_sysCtrl->aon_clock_en1).b.usb_ref_en = 1;

    REGT_FIELD_CHANGE1(hwp_sysCtrl->aon_clock_en1, \
                      REG_SYS_CTRL_AON_CLOCK_EN1_T, usb_ref_en, 1);
}

static void prvHwExit(dwcUdc_t *dwc)
{
    uint8_t i;
    for (i = 0; i < dwc->iep_count; ++i)
    {
        prvEpDisableLocked(dwc, &dwc->iep[i]);
        prvEpCancelXferInQueue(&dwc->iep[i]);
    }
    for (i = 0; i < dwc->oep_count; ++i)
    {
        prvEpDisableLocked(dwc, &dwc->oep[i]);
        prvEpCancelXferInQueue(&dwc->oep[i]);
    }
}

static void prvPlatformInit(udc_t *udc)
{
    udcPlatSetPower(udc, true);
    udcPlatEnable(udc);
    udcPlatSetClock(udc, true);
}

static void prvPlatformExit(udc_t *udc)
{
    udcPlatDisable(udc);
    udcPlatSetClock(udc, false);
    udcPlatSetPower(udc, false);
}

void vcom_DwcStart(udc_t *udc)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
        
    USB_LOG_TRACE("[USB][VCOM]VCOM_DwcStart\n");
    prvPlatformInit(udc);

    prvHwInit(dwc, true);
    dwc->pm_level = DWC_NORMAL;
    //OSI_LOGI(0x10009141, "usb start");
}

static void prvDwcStart(udc_t *udc)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
        
    USB_LOG_TRACE("[USB][UDC]prvDwcStart\n");
    prvPlatformInit(udc);
    ISR_RegHandler_Ex (TB_USB_INT, prvDwcISR, GxUDCHisrFunc, CHIPDRV_HISR_PRIO_0, NULL);
    prvHwInit(dwc, true);
    CHIPDRV_EnableIRQINT (TB_USB_INT);
    dwc->pm_level = DWC_NORMAL;
}

static void prvDwcStop(udc_t *udc)
{
    dwcUdc_t *dwc ;
    CHIPDRV_DisableIRQINT (TB_USB_INT);
    USB_LOG_TRACE("[USB][UDC]prvDwcStop\n");

    dwc = prvGadgetGetDwc(udc);
    if (dwc == NULL)
        return;

    dwc->pm_level = DWC_OFF;
    prvDisconnectISR(dwc);
    prvHwExit(dwc);
    prvPlatformExit(udc);
}

static int prvDwcEpQueue(udc_t *udc, usbEp_t *ep_, usbXfer_t *xfer)
{
    dwcXfer_t *dx = prvXferGetDwcXfer(xfer);
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    dwcEp_t *ep = EP2DWCEP(ep_);
    int r = 0;
    if (ep_->ue.se.num == 0)
    {
        ep = prvGetDwcEp0(dwc);
        ep->xfer_schedule = dx;
        if (ep->ep.ue.se.dirin)
            UdcEp0Tx(udc, dx->xfer.buf, dx->xfer.length);
        r = 0;
    }
    else
    {
        dwc->current_epno = ep_->ue.se.num;
        r = prvEpQueueLocked(dwc, ep, dx);
    }
    return r;
}

static void prvDwcEpDequeue(udc_t *udc, usbEp_t *ep_, usbXfer_t *xfer)
{
    dwcUdc_t *dwc;
    dwcEp_t *ep;
    dwcXfer_t *dx = prvXferGetDwcXfer(xfer);
    if (!dx->occupied)
    {
        //osiExitCritical(critical);
        return;
    }

    dwc = prvGadgetGetDwc(udc);
    ep = EP2DWCEP(ep_);
    if (ep_->ue.se.num == 0)
        ep = prvGetDwcEp0(dwc);

    if (dx == ep->xfer_schedule)
        prvEpStopLocked(dwc, ep);

    dx->occupied = 0;
    TAILQ_REMOVE(&ep->xfer_q, dx, anchor);
    xfer->status = -ECANCELED;

    prvGiveBackComplete(dwc, ep, dx);
}

static void prvDwcEpDequeueAll(udc_t *udc, usbEp_t *ep_)
{
    dwcXfer_t *x, *tx;
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    dwcEp_t *ep = EP2DWCEP(ep_);
    if (ep_->ue.se.num == 0)
        ep = prvGetDwcEp0(dwc);

    if (ep->xfer_schedule != NULL)
        prvEpStopLocked(dwc, ep);

    TAILQ_FOREACH_SAFE(x, &ep->xfer_q, anchor, tx)
    {
        x->occupied = 0;
        x->xfer.status = -ECANCELED;
        TAILQ_REMOVE(&ep->xfer_q, x, anchor);

        prvGiveBackComplete_(dwc, ep, x, false);

    }

}

static int prvDwcEpEnable(udc_t *udc, usbEp_t *ep_)
{
    dwcUdc_t *dwc;
    dwcEp_t *ep;
    int result;
    USB_LOG_TRACE("[USB][UDC]DWC ep %x enable\n", ep_->ue.address);

    dwc = prvGadgetGetDwc(udc);
    ep = EP2DWCEP(ep_);
    result = prvEpEnableLocked(dwc, ep);
    return result;
}

static void prvDwcEpDisable(udc_t *udc, usbEp_t *ep_)
{
    dwcUdc_t *dwc;
    dwcEp_t *ep;
    USB_LOG_TRACE("[USB][UDC]DWC ep %x disable", ep_->ue.address);

    dwc = prvGadgetGetDwc(udc);
    ep = EP2DWCEP(ep_);

    prvEpDisableLocked(dwc, ep);
    prvEpCancelXferInQueue(ep);
}

static usbEp_t *prvDwcEpAllocate(udc_t *udc, usb_endpoint_descriptor_t *desc)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    bool dirin = USB_EP_GET_DIR(desc->bEndpointAddress) == USB_DIR_IN;
    uint8_t type = USB_EP_GET_XFER_TYPE(desc->bmAttributes);
    uint8_t epcount;
    dwcEp_t *eps;
    dwcEp_t *ep = NULL;
    uint8_t i;
    if (dirin)
    {
        eps = dwc->iep;
        epcount = dwc->iep_count;
    }
    else
    {
        eps = dwc->oep;
        epcount = dwc->oep_count;
    }

    for ( i = 1; i < epcount; ++i)
    {
        ep = &eps[i];
        if (!ep->used)
        {
            ep->used = 1;
            ep->type = type & 0x3;
            ep->halted = 0;
            ep->periodic = 0;
            ep->dynamic_fifo = 0;
            if (type == USB_ENDPOINT_XFER_ISOC || type == USB_ENDPOINT_XFER_INT)
                ep->periodic = dirin ? 1 : 0;
            if (dirin && dwc->dedicated_fifo == 0 && ep->periodic == 0)
                ep->dynamic_fifo = 1;
            break;
        }
    }
    if (ep == NULL)
        return NULL;

    return &ep->ep;
}

static void prvDwcEpFree(udc_t *udc, usbEp_t *ep_)
{
    dwcEp_t *ep = EP2DWCEP(ep_);
    ep->used = 0;
}

static usbXfer_t *prvDwcXferAllocate(udc_t *udc)
{
    dwcXfer_t *dx = (dwcXfer_t *)calloc(1, sizeof(*dx));
    return dx != NULL ? &dx->xfer : NULL;
}

static void prvDwcXferFree(udc_t *udc, usbXfer_t *xfer)
{
    dwcXfer_t *dx = (dwcXfer_t *)xfer;
    free(dx);
}

static void prvDwcGdbPollIntr(udc_t *udc)
{
    dwcUdc_t *dwc = prvGadgetGetDwc(udc);
    prvDwcPollingISR(TB_USB_INT);
    //if (dwc->connected)
        //prvDwcISR(TB_USB_INT);
}

static void prvDwcInit(udc_t *udc, dwcUdc_t *dwc)
{
    static uint8_t _ep0_dma[96] OSI_CACHE_LINE_ALIGNED;// OSI_SECTION_SRAM_BSS;
    uint8_t i ;
    dwc->iep_count = 15 + 1;
    dwc->oep_count = 15 + 1;
    dwc->ep0_buf = _ep0_dma;        // 32 bytes
    dwc->setup_buf = &_ep0_dma[32]; // 64 bytes
    dwc->gadget = udc;
    dwc->dedicated_fifo = 0;
    dwc->connected = 0;
    dwc->tx_fifo_map |= (1 << 0); // ep 0x80 use fifo 0

    STAILQ_INIT(&dwc->pending_eps);
    memset(&dwc->iep[0], 0, sizeof(dwcEp_t) * dwc->iep_count);
    memset(&dwc->oep[0], 0, sizeof(dwcEp_t) * dwc->oep_count);
    for ( i = 0; i < dwc->iep_count; ++i)
    {
        dwc->iep[i].xfer_schedule = NULL;
        dwc->iep[i].ep.ue.se.num = i;
        dwc->iep[i].ep.ue.se.dirin = 1;

        TAILQ_INIT(&dwc->iep[i].xfer_q);
    }

    for ( i = 0; i < dwc->oep_count; ++i)
    {
        dwc->oep[i].xfer_schedule = NULL;
        dwc->oep[i].ep.ue.se.num = i;
        dwc->oep[i].ep.ue.se.dirin = 0;

        TAILQ_INIT(&dwc->oep[i].xfer_q);
    }

    dwc->iep[0].periodic = 0;

    udc->controller_priv = (unsigned long)dwc;
}

void dwcUdcInit(udc_t *udc)
{
    dwcUdc_t *dwc = &_the_controller;
    if (udc == NULL)
        return;

    udc->rx_linklist = &aligned_rx_linklist[0];
    udc->tx_linklist = &aligned_tx_linklist[0];

    prvDwcInit(udc, dwc);
    udc->speed = USB_SPEED_UNKNOWN;
    udc->ep0 = &dwc->oep[0].ep;
    udc->ops.start = prvDwcStart;
    udc->ops.stop = prvDwcStop;
    udc->ops.remote_wakeup = prvDwcRmtWakeup;
    udc->ops.enqueue = prvDwcEpQueue;
    udc->ops.dequeue = prvDwcEpDequeue;
    udc->ops.dequeue_all = prvDwcEpDequeueAll;
    udc->ops.ep_enable = prvDwcEpEnable;
    udc->ops.ep_disable = prvDwcEpDisable;
    udc->ops.ep_stall = prvDwcEpHalt;
    udc->ops.ep_alloc = prvDwcEpAllocate;
    udc->ops.ep_free = prvDwcEpFree;
    udc->ops.xfer_alloc = prvDwcXferAllocate;
    udc->ops.xfer_free = prvDwcXferFree;
    // gdb mode
    udc->ops.gdb_poll = prvDwcGdbPollIntr;

    //osiRegisterBlueScreenHandler(false, NULL, (osiCallback_t)prvDwcGdbPollIntr, udc);
}

void DwcUdcExit(udc_t *udc)
{
    dwcUdc_t *dwc;
    if (!udc)
        return;

    prvDwcStop(udc);
    dwc = prvGadgetGetDwc(udc);

    if (dwc)
    {
        udc->controller_priv = 0;
        memset(dwc, 0, sizeof(*dwc));
    }
}

void dump_usb_registers (void)
{
#ifdef _DEBUG_USB20_
    uint32 i;
    uint32 *src,*dest;

    src = (uint32 *) MUSB_REG_BASE;
    dest = (uint32 *) usb_register_map;

    for (i=0; i<REGISTER_MAP_LEN/4; i++)
    {
        *dest++ = *src++;
    }

#endif
}

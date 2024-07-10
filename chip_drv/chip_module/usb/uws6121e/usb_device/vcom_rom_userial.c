/******************************************************************************
 ** File Name:    usb.c                                                     *
 ** Author:       weihua.wang                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file is usb driver file .It include all driver funciton*
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005     weihua.wang     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
//#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "usb_drv.h"
//#include "usb20_drvapi.h"
#include "vcom_usb.h"
//#include "usb_ch9.h"
#include "ref_outport.h"
#include "chip_plf_export.h"
//#include "usb20_app_config.h"
#include "mmu_drvapi.h"
#include "auto_download.h"
#include "osi_fifo.h"
//#include "usb_device_uws6121e.h"
#include "usb_utils.h"
#include "usb_serial.h"
//#include "threadx_assert.h"

#include "usb_ch9.h"
#include "usb_cdc.h"
#include "usb_platform_uws6121e.h"
#include "udc_dwc.h"
//#include "efuse_drvapi.h"
#include "ldo_drvapi.h"


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
#define STRID_MANUFACTUR 1
#define STRID_PRODUCT 2
#define STRID_FUNCTION 3

#define LANGID_EN_US 0x0409
#define CTRL_INTERFACE 0
#define DATA_INTERFACE 1
#define ACM_EP_BULK_OUT 1
#define ACM_EP_BULK_IN 1
#define ACM_EP_INTR_IN 2
#define ROM_USB_PID 0x4d16
/**----------------------------------------------------------------------------*
**                         Variable Prototype                                 **
**----------------------------------------------------------------------------*/

const char *const gStrings[] = {
    "UNISOC",                       //STRID_MANUFACTUR
    "UNISOC CDC-ACM Serial Port",   //STRID_PRODUCT
    "CDC-ACM Serial Port",          //STRID_FUNCTION
};

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/*--------------------------- Local Data ------------------------------------*/


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/**
 * Buffer Model:
 *
 * There are 2 buffers for rx, one for DMA and the other for fifo. API will
 * always get from fifo. At rx interrupt, rx data will be copied from rx
 * DMA buffer to rx fifo. After all rx data are copied, rx DMA will be
 * started again.
 *
 * There is one buffer for tx. And in concept, there are 2 buffers share the
 * same memory, ont for DMA and the other for fifo. API will always put to
 * fifo. At flush, tx DMA will be started, and wait tx done.
 *
 * The total size is: RX_DMA_SIZE + RX_FIFO_SIZE + TX_FIFO_SIZE
 */
#define RX_DMA_SIZE (512)
#define RX_FIFO_SIZE (512)
#define TX_DMA_SIZE (TX_FIFO_SIZE)
#define TX_FIFO_SIZE (512)

typedef struct
{
    uint32_t addr;
    uint16_t frag_len;
    uint16_t blk_len;
    uint32_t link_end : 1;
    uint32_t sp : 1;
    uint32_t ioc : 1;
    uint32_t : 5;
    uint32_t haddr : 4;
    uint32_t : 20;
    uint32_t : 32; // This word is needed
} romUdcLinkList_t;

//static_assert(sizeof(romUdcLinkList_t) == 16, "romUdcLinkList_t size error");

typedef struct romUdc
{
    uint8_t speed;
    bool is_configured;
    struct usb_cdc_line_state line_coding;
    void *rx_dma_buf;
    void *tx_dma_buf;
    romUdcLinkList_t rx_linklist;
    romUdcLinkList_t tx_linklist;
    osiFifo_t rx_fifo;
    osiFifo_t tx_fifo;
} romUdc_t;

struct romUserial
{
    bool inited;
    uint8_t state;
    romUdc_t udc;
};

typedef struct romUserial romUserial_t;

__align(32) uint8 gUserialRxDmaBuf[RX_DMA_SIZE];
__align(32) uint8 gUserialRxFifoBuf[RX_FIFO_SIZE];
__align(32) uint8 gUserialTxBuf[TX_DMA_SIZE];

static romUserial_t gRomUserial;
void rom_Userial_disconnect(void);

extern void udcPlatformStart(void);
extern void udcPlatformStop(void);


/*__align (4) */static const usb_device_descriptor_t gDeviceDescriptor =
{
    sizeof (usb_device_descriptor_t),
    USB_DT_DEVICE,
    0x0200, // USB 2.0
    USB_CLASS_MISC,
    0x00,
    0x00,
    0x40,
    USB_SPREADTRUM_VENDER_ID,  // SPREADTRUM VID
    ROM_USB_PID,
    0x01,
    STRID_MANUFACTUR,
    STRID_PRODUCT,
    0x00,
    0x01
};
/*__align (4) */static const usb_device_qualifier_t gDeviceQualifier = {
    sizeof(usb_device_qualifier_t),
    USB_DT_DEVICE_QUALIFIER,
    0x0200,
    USB_CLASS_MISC,
    0x02,
    0x01,
    64,
    1,
    0,
};
struct romUdcConfig
{
    usb_config_descriptor_t config;
    usb_interface_assoc_descriptor_t iad;
    usb_interface_descriptor_t ctrl_intf;
    usb_cdc_header_descriptor_t cdc_header;
    usb_cdc_cm_descriptor_t cdc_cm;
    usb_cdc_acm_descriptor_t cdc_acm;
    usb_cdc_union_descriptor_t cdc_union;
    usb_endpoint_descriptor_t notif_ep;
    usb_interface_descriptor_t data_intf;
    usb_endpoint_descriptor_t in_ep;
    usb_endpoint_descriptor_t out_ep;
} __attribute__((packed));
typedef struct romUdcConfig romUdcConfig_t;

static const romUdcConfig_t gConfig = {
    //config
    {
        sizeof(usb_config_descriptor_t),            //.bLength
        USB_DT_CONFIG,                              //.bDescriptorType =
        sizeof(gConfig),                            //.wTotalLength = 
        2,                                          //.bNumInterface = 
        1,                                          //.bConfigurationValue =
        0,                                          //.iConfiguration =
        0x80,                                       //.bmAttributes =
        500 / 2,                                    //.bMaxPower = 
    },
    //.iad = 
    {
        sizeof(usb_interface_assoc_descriptor_t),   // .bLength = 
        USB_DT_INTERFACE_ASSOCIATION,               //.bDescriptorType = 
        0,                                          //.bFirstInterface = 
        2,                                          //.bInterfaceCount =
        USB_CLASS_COMM,                             //.bFunctionClass = 
        UDESCSUB_CDC_ACM,                           //.bFunctionSubClass = 
        UCDC_ACM_PROTO_AT_V25TER,                   //.bFunctionProtocol =
        STRID_FUNCTION,                             //.iFunction =
    },
    //.ctrl_intf = 
    {
        sizeof(usb_interface_descriptor_t),         //.bLength = 
        USB_DT_INTERFACE,                           //.bDescriptorType = 
        CTRL_INTERFACE,                             //.bInterfaceNumber = 
        0,                                          //.bAlternateSetting = 
        1,                                          //.bNumEndpoints = 
        USB_CLASS_COMM,                             //.bInterfaceClass =
        UDESCSUB_CDC_ACM,                           //.bInterfaceSubClass =
        UCDC_ACM_PROTO_AT_V25TER,                   //.bInterfaceProtocol = 
        STRID_FUNCTION,                             //.iInterface =
    },
    //.cdc_header =
    {
        sizeof(usb_cdc_header_descriptor_t),        // .bLength =
        USB_DT_CS_INTERFACE,                        //.bDescriptorType = 
        UDESCSUB_CDC_HEADER,                        //.bDescriptorSubtype =
        0x0110,                                     //.bcdCDC =
    },
    //.cdc_cm = 
    {
        sizeof(usb_cdc_cm_descriptor_t),            //bLength = 
        USB_DT_CS_INTERFACE,                        //.bDescriptorType = 
        UDESCSUB_CDC_CM,                            //.bDescriptorSubtype = 
        0,                                          //.bmCapabilities =
        DATA_INTERFACE,                             //.bDataInterface = 
    },
    //.cdc_acm = 
    {
        sizeof(usb_cdc_acm_descriptor_t),           //.bLength = 
        USB_DT_CS_INTERFACE,                        //.bDescriptorType = 
        UDESCSUB_CDC_ACM,                           //.bDescriptorSubtype = 
        USB_CDC_ACM_HAS_LINE,                       //.bmCapabilities = 
    },
    //.cdc_union = 
    {
        sizeof(usb_cdc_union_descriptor_t),         //.bLength = 
        USB_DT_CS_INTERFACE,                        //.bDescriptorType = 
        UDESCSUB_CDC_UNION,                         //.bDescriptorSubtype = 
        CTRL_INTERFACE,                             //.bMasterInterface = 
        {DATA_INTERFACE},                           //.bSlaveInterface = 
    },
    //.notif_ep = 
    {
        sizeof(usb_endpoint_descriptor_t),          //.bLength = 
        USB_DT_ENDPOINT,                            //.bDescriptorType =
        USB_DIR_IN | ACM_EP_INTR_IN,                //.bEndpointAddress =
        USB_ENDPOINT_XFER_INT,                      //.bmAttributes =
        16,                                         //.wMaxPacketSize = 
        10,                                         //.bInterval = 
    },
    //.data_intf = 
    {
        sizeof(usb_interface_descriptor_t),         //.bLength = 
        USB_DT_INTERFACE,                           //.bDescriptorType = 
        DATA_INTERFACE,                             //.bInterfaceNumber = 
        0,                                          //.bAlternateSetting = 
        2,                                          //.bNumEndpoints = 
        USB_CLASS_CDC_DATA,                         //.bInterfaceClass = 
        0,                                          //.bInterfaceSubClass = 
        0,                                          // .bInterfaceProtocol =
        STRID_FUNCTION,                             //.iInterface = 
    },
    //.in_ep = 
    {
        sizeof(usb_endpoint_descriptor_t),          //.bLength = 
        USB_DT_ENDPOINT,                            //.bDescriptorType = 
        USB_DIR_IN | ACM_EP_BULK_IN,                //.bEndpointAddress = 
        USB_ENDPOINT_XFER_BULK,                     //.bmAttributes = 
        512,                                        //.wMaxPacketSize = 
        0,                                          //.bInterval = 
    },
    //.out_ep = 
    {
        sizeof(usb_endpoint_descriptor_t),          //.bLength = 
        USB_DT_ENDPOINT,                            //.bDescriptorType = 
        USB_DIR_OUT | ACM_EP_BULK_OUT,              //.bEndpointAddress = 
        USB_ENDPOINT_XFER_BULK,                     //.bmAttributes = 
        512,                                        //.wMaxPacketSize = 
        0,                                          //.bInterval = 
    },
};

/**
 * Select EP for indexed registers access
 */
static void musbSelectEp(uint8_t ep) { MUSB_WRITE8(MUSB_INDEX, ep); }

/**
 * Read from EP fifo. This should be called only for EP0.
 * Hardware doesn't support EP fifo access except EP0.
 */
void romMusbReadFifo(unsigned ep, void *data, unsigned size)
{
    uint8_t *pdata = (uint8_t *)data;
    unsigned n = 0;
    for (n = 0; n < size; n++)
        *pdata++ = MUSB_READ8(MUSB_EP0_FIFO + 4 * ep);
}

/**
 * Write to EP fifo. This should be called only for EP0.
 * Hardware doesn't support EP fifo access except EP0.
 */
void romMusbWriteFifo(unsigned ep, const void *data, unsigned size)
{
    const uint8_t *pdata = (const uint8_t *)data;
    unsigned n = 0;
    for (n = 0; n < size; n++)
        MUSB_WRITE8(0x20 + 4 * ep, *pdata++);
}

/**
 * Start RX DMA for EP_OUT. This shouldn't be called for EP0. EP0 always
 * use fifo access. The buffer and linklist is inside udc.
 */
void romUdcStartRxDma(romUdc_t *udc, unsigned ep)
{
    unsigned rx_ch = ep + 15;
    udc->rx_linklist.addr = (uint32_t)udc->rx_dma_buf;
    udc->rx_linklist.frag_len = 32;
    udc->rx_linklist.blk_len = RX_DMA_SIZE;
    udc->rx_linklist.link_end = 1;
    udc->rx_linklist.sp = 0;
    udc->rx_linklist.ioc = 1;
    MMU_DmaCacheSync((uint32)&udc->rx_linklist, sizeof(udc->rx_linklist),DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)udc->rx_dma_buf, RX_DMA_SIZE,DMABUFFER_FROM_DEVICE);

    //romDCacheClean(&udc->rx_linklist, sizeof(udc->rx_linklist));
    //romDCacheInvalidate(udc->rx_dma_buf, RX_DMA_SIZE);

    MUSB_SET32(MUSB_DMA_CHN_INTR(rx_ch), 0x14);
    MUSB_WRITE32(MUSB_DMA_CHN_LLIST_PTR(rx_ch), (uint32_t)&udc->rx_linklist);
    MUSB_SET32(MUSB_DMA_CHN_CFG(rx_ch), 0x1);
}

/**
 * Start TX DMA for EP_IN. This shouldn't be called for EP0. EP0 always
 * use fifo access. The buffer and linklist is inside udc.
 *
 * Based on the buffer model, it will wait finish inside.
 */
void romUdcStartTxDma(romUdc_t *udc, unsigned ep, unsigned size)
{
    unsigned tx_ch = ep;
    udc->tx_linklist.addr = (uint32_t)udc->tx_dma_buf;
    udc->tx_linklist.frag_len = 32;
    udc->tx_linklist.blk_len = size;
    udc->tx_linklist.link_end = 1;
    udc->tx_linklist.sp = 0;
    udc->tx_linklist.ioc = 1;

    MMU_DmaCacheSync((uint32)&udc->tx_linklist, sizeof(udc->tx_linklist),DMABUFFER_TO_DEVICE);
    MMU_DmaCacheSync((uint32)udc->tx_dma_buf, size,DMABUFFER_TO_DEVICE);

    MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), 0x14);
    MUSB_WRITE32(MUSB_DMA_CHN_LLIST_PTR(tx_ch), (uint32_t)&udc->tx_linklist);
    MUSB_SET32(MUSB_DMA_CHN_CFG(tx_ch), 0x1);

    //OSI_LOOP_WAIT(MUSB_READ32(MUSB_DMA_CHN_INTR(tx_ch)) & (1 << 18));

    //MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), 1 << 26);
}

/**
 * Configure CDC-ACM EPs.
 */
void romUdcConfigEp(romUdc_t *udc)
{
    musbSelectEp(ACM_EP_INTR_IN);
    MUSB_WRITE8(MUSB_TXFIFOSZ, 0x16);                             // 2x512
    MUSB_WRITE16(MUSB_TXFIFOADD, 8 + 256 * ACM_EP_INTR_IN + 128); // address
    MUSB_WRITE16(MUSB_TXMAXP_EPN(ACM_EP_INTR_IN), udc->speed == USB_SPEED_HIGH ? 512 : 64);
    MUSB_WRITE16(MUSB_TXCSR_EPN(ACM_EP_INTR_IN), MUSB_TXCSR_AUTOSET |
                                 MUSB_TXCSR_DMAENAB |
                                 MUSB_TXCSR_DMAMODE |
                                 MUSB_TXCSR_CLRDATATOG |
                                 MUSB_TXCSR_FLUSHFIFO);
    MUSB_SET16(MUSB_TXCSR_EPN(ACM_EP_INTR_IN), MUSB_TXCSR_FLUSHFIFO); // flush fifo twice

    musbSelectEp(ACM_EP_BULK_IN);
    MUSB_WRITE8(MUSB_TXFIFOSZ, 0x16);                             // 2x512
    MUSB_WRITE16(MUSB_TXFIFOADD, 8 + 256 * ACM_EP_BULK_IN + 128); // address
    MUSB_WRITE16(MUSB_TXMAXP_EPN(ACM_EP_BULK_IN), udc->speed == USB_SPEED_HIGH ? 512 : 64);
    MUSB_WRITE16(MUSB_TXCSR_EPN(ACM_EP_BULK_IN), MUSB_TXCSR_AUTOSET |
												MUSB_TXCSR_DMAENAB |
												MUSB_TXCSR_DMAMODE |
												MUSB_TXCSR_CLRDATATOG |
												MUSB_TXCSR_FLUSHFIFO);
    MUSB_SET16(MUSB_TXCSR_EPN(ACM_EP_BULK_IN), MUSB_TXCSR_FLUSHFIFO); // flush fifo twice

    musbSelectEp(ACM_EP_BULK_OUT);
    MUSB_WRITE8(MUSB_RXFIFOSZ, 0x16);                        // 2x512
    MUSB_WRITE16(MUSB_RXFIFOADD, 8 + 256 * ACM_EP_BULK_OUT); // address
    MUSB_WRITE16(MUSB_RXMAXP_EPN(ACM_EP_BULK_OUT), udc->speed == USB_SPEED_HIGH ? 512 : 64);
    MUSB_CLR16(MUSB_RXCSR_EPN(ACM_EP_BULK_OUT), MUSB_RXCSR_RXPKTRDY);
    MUSB_WRITE16(MUSB_RXCSR_EPN(ACM_EP_BULK_OUT), MUSB_RXCSR_AUTOCLEAR |
												MUSB_RXCSR_DMAENAB |
												MUSB_RXCSR_DMAMODE |
												MUSB_RXCSR_CLRDATATOG |
												MUSB_RXCSR_FLUSHFIFO);
    MUSB_SET16(MUSB_RXCSR_EPN(ACM_EP_BULK_OUT), MUSB_RXCSR_FLUSHFIFO); // flush fifo twice

    MUSB_WRITE16(MUSB_INTRTXE, (1 << ACM_EP_INTR_IN) | (1 << ACM_EP_BULK_IN) | 1);
    MUSB_WRITE16(MUSB_INTRRXE, 1 << ACM_EP_BULK_OUT);
}

/**
 * EP0 tx transfer. It should be called once at most for each transaction.
 */
void romUdcEp0Tx(romUdc_t *udc, const void *data, unsigned size)
{
    unsigned trans=0;
    uint16_t csr=0;

    // It will be called after SETUP phase, so set SVDRXPKTRDY
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY);

    while (size > 0)
    {
        trans = OSI_MIN(unsigned, size, MUSB_EP0_FIFOSIZE);
        romMusbWriteFifo(0, data, trans);

        data = (const char *)data + trans;
        size -= trans;

        csr = MUSB_CSR0_TXPKTRDY | (size == 0 ? MUSB_CSR0_P_DATAEND : 0);
        MUSB_WRITE16(MUSB_CSR_EP0, csr);

        if (size > 0)
            OSI_LOOP_WAIT((MUSB_READ16(MUSB_CSR_EP0) & MUSB_CSR0_TXPKTRDY) == 0);
    }
}

/**
 * EP0 rx transfer. It should be called once at most for each transaction.
 * It is simplified. For current application, the maximum size is less than 64.
 *
 * Return false on error.
 */
bool romUdcEp0Rx(romUdc_t *udc, void *data, unsigned size)
{
    uint16_t count = 0;

    if (size > MUSB_EP0_FIFOSIZE)
        return false;

    // It will be called after SETUP phase, so set SVDRXPKTRDY
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY);

    OSI_LOOP_WAIT((MUSB_READ16(MUSB_INTRTX) & (1 << 0)) != 0 &&
                  MUSB_READ16(MUSB_CSR_EP0) & MUSB_CSR0_RXPKTRDY);

    count = MUSB_READ16(MUSB_COUNT_EP0);
    if (count != size)
        return false;

    romMusbReadFifo(0, data, count);
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);
    return true;
}

/**
 * Output string descriptor. langid isn't used, due to we will only support
 * LANGID_EN_US.
 *
 * Return false on error.
 */
bool romUdcEp0OutString(romUdc_t *udc, uint16_t langid, uint8_t id, uint16_t max_length)
{
    uint8 desc_buf[64]={0};
    usb_string_descriptor_t *desc = desc_buf;
    unsigned slen = 0;
    unsigned n = 0;
    if (id == 0)
    {
        //desc = (usb_string_descriptor_t *)calloc(1, 2 + 2);
        desc->bLength = 4;
        desc->bDescriptorType = USB_DT_STRING;
        desc->wData[0] = LANGID_EN_US;
        romUdcEp0Tx(udc, desc, desc->bLength);
        return true;
    }

    if (id < OSI_ARRAY_SIZE(gStrings))
    {
        slen = strlen(gStrings[id]);
        //desc = (usb_string_descriptor_t *)calloc(1, 2 + slen * 2);
        desc->bLength = sizeof(usb_string_descriptor_t) + (slen - 1) * 2;
        desc->bDescriptorType = USB_DT_STRING;
        for (n = 0; n < slen; n++) // simplified ASCII->UNICODE
            desc->wData[n] = (uint8_t)gStrings[id][n];

        romUdcEp0Tx(udc, desc, desc->bLength);
        return true;
    }

    return false;
}

/**
 * Handle GET_STATUS device request.
 */
bool romUdcCtrlGetStatus(romUdc_t *udc, usb_device_request_t *ctrl)
{
    uint16_t reply = 0;
    uint8_t epnum = 0;
    bool is_in = false;

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
            reply = MUSB_READ16(MUSB_TXCSR_EPN(epnum)) & MUSB_RXCSR_P_SENDSTALL ? 1 : 0;
        musbSelectEp(0);
        break;
    }

    default:
        return false;
    }

    romUdcEp0Tx(udc, &reply, 2);
    return true;
}

/**
 * Handle GET_DESCRIPTOR device request.
 */
bool romUdcCtrlGetDesc(romUdc_t *udc, usb_device_request_t *ctrl)
{
    unsigned wLength=0;
    romUdcConfig_t config={0};
    switch (ctrl->wValue >> 8)
    {
    case USB_DT_DEVICE:
        wLength = OSI_MIN(unsigned, ctrl->wLength, sizeof(gDeviceDescriptor));
        romUdcEp0Tx(udc, &gDeviceDescriptor, wLength);
        return true;

    case USB_DT_CONFIG:
    case USB_DT_OTHER_SPEED_CONFIG:
        wLength = OSI_MIN(unsigned, ctrl->wLength, sizeof(gConfig));
        if (udc->speed == USB_SPEED_HIGH)
        {
            romUdcEp0Tx(udc, &gConfig, wLength);
        }
        else
        {
            //config = (romUdcConfig_t *)calloc(1,sizeof(romUdcConfig_t));
            memcpy(&config, &gConfig, sizeof(romUdcConfig_t));
            config.notif_ep.bInterval = 32;
            config.in_ep.wMaxPacketSize = 64;
            config.out_ep.wMaxPacketSize = 64;
            romUdcEp0Tx(udc, &config, wLength);
        }
        return true;

    case USB_DT_DEVICE_QUALIFIER:
        wLength = OSI_MIN(unsigned, ctrl->wLength, sizeof(gDeviceQualifier));
        romUdcEp0Tx(udc, &gDeviceQualifier, wLength);
        return true;

    case USB_DT_STRING:
        return romUdcEp0OutString(udc, ctrl->wIndex, ctrl->wValue & 0xff, ctrl->wLength);

    default:
        return false;
    }
}

/**
 * Handle device request
 */
bool romUdcProcessControl(romUdc_t *udc, usb_device_request_t *ctrl)
{
    if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD)
    {
        switch (ctrl->bRequest)
        {
        case USB_REQ_GET_STATUS:
            return romUdcCtrlGetStatus(udc, ctrl);

        case USB_REQ_CLEAR_FEATURE:
            return false; // TODO

        case USB_REQ_SET_FEATURE:
            return false; // TODO

        case USB_REQ_SET_ADDRESS:
            gRomUserial.state = VCOM_DEV_PRESENT;
            //USB_LOG_TRACE("set address,cur 32 Tick:%d",SLEEP_GetCur32kNum());
            MUSB_WRITE16(MUSB_CSRL_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);

            OSI_LOOP_WAIT(MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT);
            MUSB_WRITE8(MUSB_FADDR, ctrl->wValue & 0xff);
            
            return true;

        case USB_REQ_GET_DESCRIPTOR:
            //USB_LOG_TRACE("%s USB_REQ_GET_DESCRIPTOR\n",__FUNCTION__);
            return romUdcCtrlGetDesc(udc, ctrl);

        case USB_REQ_SET_CONFIGURATION:
            USB_LOG_TRACE("%s USB_REQ_SET_CONFIGURATION\n",__FUNCTION__);
            MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);

            OSI_LOOP_WAIT(MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT);
            if (!udc->is_configured)
            {
                romUdcConfigEp(udc);
                romUdcStartRxDma(udc, ACM_EP_BULK_OUT);
                udc->is_configured = true;
            }
            return true;

        default:
            return false;
        }
    }

    if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS)
    {
        switch (ctrl->bRequest)
        {
        case UCDC_SET_LINE_CODING:
            return romUdcEp0Rx(udc, &udc->line_coding, sizeof(udc->line_coding));

        case UCDC_GET_LINE_CODING:
            romUdcEp0Tx(udc, &udc->line_coding, sizeof(udc->line_coding));
            return true;

        case UCDC_SET_CONTROL_LINE_STATE:
            USB_LOG_TRACE("%s UCDC_SET_CONTROL_LINE_STATE value=%d \n",__FUNCTION__,ctrl->wValue);
            MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_DATAEND);
            return true; // do nothing

        default:
            return false;
        }
    }

    return false;
}

/**
 * Handle EP0
 */
void romUdcIsrEp0(romUdc_t *udc)
{
    uint16_t csr = 0;
    uint16_t count =0;
    usb_device_request_t ctrl;

    musbSelectEp(0);

    csr = MUSB_READ16(MUSB_CSR_EP0);
    count = MUSB_READ16(MUSB_COUNT_EP0);
    //USB_LOG_TRACE("intr ep0,csr:0x%x,count=%d",csr,count);

    if (csr & MUSB_CSR0_P_DATAEND) // It is set by CPU, and wait auto clear
        return;

    if (csr & MUSB_CSR0_P_SENTSTALL) // clear SENTSTALL
    {
        csr &= ~MUSB_CSR0_P_SENTSTALL;
        MUSB_WRITE16(MUSB_CSR_EP0, csr);
    }

    if (csr & MUSB_CSR0_P_SETUPEND) // clear SETUPEND
        MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDSETUPEND);

    // We are using polling mode, so it is not needed to record ep0 state
    if (!(csr & MUSB_CSR0_RXPKTRDY))
        return;

    if (count != sizeof(usb_device_request_t))
        return;

    // In case RESET isn't reported
    if (udc->speed == USB_SPEED_VARIABLE)
    {
        udc->speed = (MUSB_READ8(MUSB_POWER) & MUSB_POWER_HSMODE)
                         ? USB_SPEED_HIGH
                         : USB_SPEED_FULL;
    }

    romMusbReadFifo(0, &ctrl, sizeof(usb_device_request_t));

    //USB_LOG_TRACE("device request %08x%08x", OSI_TO_BE32(((unsigned *)&ctrl)[0]),
            //OSI_TO_BE32(((unsigned *)&ctrl)[1]));

    if (!romUdcProcessControl(udc, &ctrl))
    {
        USB_ERR_TRACE("ep0 send stall");
        MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0_P_SVDRXPKTRDY | MUSB_CSR0_P_SENDSTALL);
    }
}
static void romUserialResetISR(void)
{
    MUSB_WRITE8(MUSB_FADDR, 0);

    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN | MUSB_POWER_HSENAB);

    MUSB_WRITE8(MUSB_INTRUSBE, MUSB_INTR_RESET | MUSB_INTR_RESUME | MUSB_INTR_SUSPEND);
    MUSB_WRITE16(MUSB_INTRTXE, EP0EN);
}
static void romUserialStop(void)
{
    USB_LOG_TRACE("%s start\n", __FUNCTION__);
    udcPlatformStop();
    osiFifoReset(&gRomUserial.udc.rx_fifo);
    osiFifoReset(&gRomUserial.udc.tx_fifo);
    gRomUserial.inited = false;
    gRomUserial.state = VCOM_DEV_NO_PRESENT;

}

static void udelay(unsigned int us)
{
    unsigned int start;
    unsigned int ticks;
    start = hwp_timer2->hwtimer_curval_l;
    ticks = us*2;  //timer freq = 2000000HZ  0.5us + 1
    while(!(((unsigned)((hwp_timer2->hwtimer_curval_l) - start)) > ticks))
    {
    }
}

/**
 * MUSB Polling
 */
void romUdcIsrPoll(romUdc_t *udc)
{
    uint8_t intusb = MUSB_READ8(MUSB_INTRUSB);
    unsigned rx_ch = 0;
    unsigned tx_ch = 0;
    unsigned blk_len = 0;
    unsigned count = 0;
    uint8_t power = 0;
    

    if (intusb & MUSB_INTR_RESET)
    {
        USB_LOG_TRACE("%s MUSB_INTR_RESET\n",__FUNCTION__);
        rom_Userial_disconnect();
        hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0x05);
        romUserialResetISR();
        power = MUSB_READ8(MUSB_POWER);
        udc->speed = (power & MUSB_POWER_HSMODE) ? USB_SPEED_HIGH : USB_SPEED_FULL;
        udc->is_configured = false;
    }
    if (intusb & MUSB_INTR_SUSPEND)
    {
        USB_LOG_TRACE("%s MUSB_INTR_SUSPEND\n",__FUNCTION__);
        hwp_aonClk->cgm_usb_ahb_sel_cfg = AON_CLK_CGM_USB_AHB_SEL(0);
        udelay(100);
        gRomUserial.state = VCOM_DEV_NO_PRESENT;
        udc->is_configured = false;
    }

    if (MUSB_READ16(MUSB_INTRTX) & MUSB_EP0_INTERRUPT)
        romUdcIsrEp0(udc);

    if (udc->is_configured)
    {
        rx_ch = ACM_EP_BULK_OUT + 15;
        if (MUSB_READ32(MUSB_DMA_CHN_INTR(rx_ch)) & (1 << 18))
        {
            // get dma receive data len
            blk_len = MUSB_READ32(MUSB_DMA_CHN_LEN(rx_ch)) >> 16;
            count = RX_DMA_SIZE - blk_len;

            //USB_LOG_TRACE("userial rx count %d", count);
            if (count <= osiFifoSpace(&udc->rx_fifo))
            {
                MUSB_SET32(MUSB_DMA_CHN_INTR(rx_ch), (1 << 26) | (1 << 28));

                //romDCacheInvalidate(udc->rx_dma_buf, count);
                MMU_DmaCacheSync((uint32)udc->rx_dma_buf, count,DMABUFFER_FROM_DEVICE);
                osiFifoPut(&udc->rx_fifo, udc->rx_dma_buf, count);

                romUdcStartRxDma(udc, ACM_EP_BULK_OUT);
            }
        }

        tx_ch = ACM_EP_BULK_IN;
        if (MUSB_READ32(MUSB_DMA_CHN_INTR(tx_ch)) & (1 << 18))
        {
            MUSB_SET32(MUSB_DMA_CHN_INTR(tx_ch), CHN_LLIST_INT_CLR);
        }

    }
}

static void rom_UserialHwInit(void)
{
    MUSB_SET8(MUSB_POWER, MUSB_POWER_SOFTCONN | MUSB_POWER_HSENAB | MUSB_POWER_ENSUSPEND);

    MUSB_SET16(MUSB_INTRTXE, EP0EN);

    MUSB_WRITE8(MUSB_INTRUSBE, 0);
    MUSB_WRITE8(MUSB_INTRUSBE, MUSB_INTR_RESET | MUSB_INTR_RESUME | MUSB_INTR_SUSPEND);
    //(REG_SYS_CTRL_AON_CLOCK_EN1_T)(hwp_sysCtrl->aon_clock_en1).b.usb_ref_en = 1;

    REGT_FIELD_CHANGE1(hwp_sysCtrl->aon_clock_en1, \
                      REG_SYS_CTRL_AON_CLOCK_EN1_T, usb_ref_en, 1);
}

void rom_Userial_DwcStart(void)
{
    USB_LOG_TRACE("[USB][VCOM]rom_Userial_DwcStart\n");
    udcPlatformStart();

    rom_UserialHwInit();
}

void rom_Userial_disconnect(void)
{
    uint8_t i=0;
//disconnect
    MUSB_WRITE8(MUSB_INDEX, 0);

    /*endpoint0 handle*/
    MUSB_CLR16(MUSB_INTRTXE, EP0EN);
    MUSB_WRITE16(MUSB_CSR_EP0, MUSB_CSR0RESET);
    MUSB_WRITE16(MUSB_COUNT_EP0, MUSB_COUNT0RESET);
    
    MUSB_CLR16(MUSB_INTRTXE, (1 << ACM_EP_INTR_IN));
    MUSB_CLR16(MUSB_INTRRXE, (1 << ACM_EP_BULK_OUT));
    MUSB_CLR16(MUSB_INTRTXE, (1 << ACM_EP_BULK_IN));
    
/*
    for ( i = 1; i < 16; ++i){
        MUSB_CLR16(MUSB_INTRTXE, (1 << i));
        MUSB_CLR16(MUSB_INTRRXE, (1 << i));
    
        MUSB_WRITE16(MUSB_TXMAXP_EPN(i), MUSB_TXMAXPRESET);
        MUSB_SET16(MUSB_TXCSR_EPN(i), MUSB_TXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_TXCSR_EPN(i), MUSB_TXCSR_FLUSHFIFO);
        
        MUSB_WRITE16(MUSB_RXMAXP_EPN(i), MUSB_RXMAXPRESET);
        MUSB_SET16(MUSB_RXCSR_EPN(i), MUSB_RXCSR_FLUSHFIFO);
        MUSB_SET16(MUSB_RXCSR_EPN(i), MUSB_RXCSR_FLUSHFIFO);
    }
*/
}

void rom_Userial_DwcStop(void)
{
    rom_Userial_disconnect();
    udcPlatformStop();
}

/**
 * Open USB serial
 */
static void romUserialOpen(romUserial_t *d)
{
/*
    if (d->inited){
        USB_LOG_TRACE("romUserial already inited!\n");
        return;
    }
*/
    memset(d, 0, sizeof(romUserial_t));
    rom_Userial_DwcStart();

    d->udc.speed = USB_SPEED_VARIABLE;
    d->udc.is_configured = false;
    d->udc.line_coding.dwDTERate = 115200;
    d->udc.line_coding.bCharFormat = UCDC_STOP_BIT_1;
    d->udc.line_coding.bParityType = UCDC_PARITY_NONE;
    d->udc.line_coding.bDataBits = 8;
    d->udc.rx_dma_buf = gUserialRxDmaBuf;
    d->udc.tx_dma_buf = gUserialTxBuf;

    d->state = VCOM_DEV_NO_PRESENT;
    osiFifoInit(&d->udc.rx_fifo, gUserialRxFifoBuf, RX_FIFO_SIZE);
    osiFifoInit(&d->udc.tx_fifo, gUserialTxBuf, TX_FIFO_SIZE);
    
    d->inited = true;
    romUdcIsrPoll(&d->udc);
    //USB_LOG_TRACE("%s done\n", __FUNCTION__);

}
/**
 * Flush, force output tx bytes.
 */
void romUserialFlush(romUserial_t *d)
{
    unsigned tx_bytes = osiFifoBytes(&d->udc.tx_fifo);
    
    //USB_LOG_TRACE("%s tx_bytes=%d\n",__FUNCTION__,tx_bytes);
    if (tx_bytes > 0)
    {
        romUdcStartTxDma(&d->udc, ACM_EP_BULK_IN, tx_bytes);
        osiFifoReset(&d->udc.tx_fifo);
    }
}
/**
 * Polling
 */
void romUserialPoll(romUserial_t *d)
{
    romUserialFlush(d);
    romUdcIsrPoll(&d->udc);
}

/**
 * Get char from USB serial
 */
int romUserialGetChar(romUserial_t *d)
{
    int ch = -1;
    char ch1;
    romUdcIsrPoll(&d->udc);

    if (!d->udc.is_configured)
        return -1;

    if (!osiFifoIsEmpty(&d->udc.rx_fifo)){
        //romUserialFlush(d);
        osiFifoGet(&d->udc.rx_fifo,&ch1,1);
        ch = ch1;
        ch = ch & 0xff;
    }
    
    return ch;
}

/**
 * Get char from USB serial
 */
int romUserialPutChar(romUserial_t *d, uint8_t ch)
{
    romUdcIsrPoll(&d->udc);

    if (!d->udc.is_configured)
        return -1;

    if (osiFifoIsFull(&d->udc.tx_fifo))
        romUserialFlush(d);
    return osiFifoPut(&d->udc.tx_fifo, &ch,1);
}




/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/

PUBLIC void USB_Connect (void)
{
    VCOM_usb_boot();

}
PUBLIC void USB_Disconnect (void)
{

    if (!gRomUserial.inited)
        return;

    romUserialStop();
}


/*****************************************************************************/
//  Description:    usb register init and local var init
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_usb_boot(void)
{
    //romUserialParamCalibration();
    //if(drvUsbIsConnected())
    udcPlatformStop();
    udelay(100);
    romUserialOpen(&gRomUserial);
    USB_LOG_TRACE("%s,cur 32 Tick:%d",__FUNCTION__,SLEEP_GetCur32kNum());
}

PUBLIC void VCOM_Open(BOOLEAN ready)
{
    //USB_DCFG_U dcfg;
    romUserial_t *d = &gRomUserial;
    //dcfg.dwValue = *(volatile uint32 *)USB_DCFG;

    if(d->udc.is_configured == false)
    {
        USB_LOG_TRACE("%s,boot",__FUNCTION__);
        VCOM_usb_boot();
    }
    else
    {
        //vcom_data_init();
        USB_LOG_TRACE("%s,poll",__FUNCTION__);
        d->state = ready ? VCOM_DEV_READY : VCOM_DEV_PRESENT;
        romUserialPoll(d);
    }

    if (AutoDL_IsDloaderMode())
    {
        USB_Disconnect();
        OS_TickDelay(30);

        USB_Connect();
    }
}

/*****************************************************************************/
//  Description:    complete the handshake and receive data handle
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           -1:got none from fifo, else return the received data
/*****************************************************************************/
PUBLIC int32 VCOM_GetChar()
{
    romUserial_t *d = &gRomUserial;
    int32 ch = -1;
    
    ch = romUserialGetChar(d);

    if(ch != -1)
    {
        d->state = VCOM_DEV_READY;
        
        //USB_LOG_TRACE("%s,%d getchar:0x%x\n",__FUNCTION__,__LINE__,ch);
    }

    return (int32)ch;

}

/*****************************************************************************/
//  Description:    pull down usb ldo
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_disconnect (void)
{
    if (!gRomUserial.inited)
        return;
    romUserialStop();

}


/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8 nchar: char to be sent
//  Note:           when this func is called, it's indicated that current irs is disable
//                  , no interrupt can happen,
/*****************************************************************************/
PUBLIC void VCOM_PutTxChar(uint8 nchar)
{
    romUserial_t *d = &gRomUserial;

    romUserialPutChar(d,nchar);


}

/*****************************************************************************/
//  Description:    put chars to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8* data_ptr: chars to be sent
//                  int32 len: total data size to be sent
//  Note:           called when dump memory only! to reduce time, 0x7e will be convert here
/*****************************************************************************/
PUBLIC void VCOM_PutTxChars (uint8* data, int32 len)
{
    romUserial_t *d = &gRomUserial;
    uint32 index = 0;
    uint8 val = 0;
    int ret = 0;
    
    for (index = 0; index < len; ++index) {
        val = data[index];
        if (0x7E == val || 0x7D == val) {
            uint8 val_1 = 0x7D;
            VCOM_PutTxChar(val_1);
            val = val ^ 0x20;
            VCOM_PutTxChar(val);
        } else {
            VCOM_PutTxChar(val);
        }
    }
}

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_Flush(void)
{
    romUserial_t *d = &gRomUserial;
    romUserialFlush(d);
}

/*****************************************************************************/
//  Description:    get usb virtual com state
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       none
//  Note: ret value:VCOM_DEV_NO_PRESENT
//                  VCOM_DEV_PRESENT
//                  VCOM_DEV_READY
//                  VCOM_DEV_BUSY
/*****************************************************************************/
PUBLIC uint32 VCOM_GetDevState (void)
{
    return gRomUserial.state;
}


const usbSerialDriverOps_t *usbVcomGetOps(void)
{
   return PNULL;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

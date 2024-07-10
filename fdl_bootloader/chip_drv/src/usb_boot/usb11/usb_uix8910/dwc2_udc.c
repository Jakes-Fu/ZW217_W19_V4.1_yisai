/*
 * drivers/usb/gadget/dwc2_udc_otg.c
 * Designware DWC2 on-chip full/high speed USB OTG 2.0 device controllers
 *
 * Copyright (C) 2008 for Samsung Electronics
 *
 * BSP Support for Samsung's UDC driver
 * available at:
 * git://git.kernel.org/pub/scm/linux/kernel/git/kki_ap/linux-2.6-samsung.git
 *
 * State machine bugfixes:
 * Marek Szyprowski <m.szyprowski@samsung.com>
 *
 * Ported to u-boot:
 * Marek Szyprowski <m.szyprowski@samsung.com>
 * Lukasz Majewski <l.majewski@samsumg.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

//#include <linux/usb/ch9.h>
#include "usbdescriptors.h"

//#include "byteorder.h"
//#include "unaligned.h"

#include "global_macros.h"
#include "usbc.h"

#include "usbdevice.h"

#include "ep0.h"
#include "dwc2_udc.h"

/***********************************************************/

#define DEBUG_L0 1
#define DEBUG_L1 0
#define DEBUG_L2 0
#define DEBUG_L3 0

#define dwc2_debug(COND, fmt, ...) \
	do { \
		if (COND) rprintf(fmt, ##__VA_ARGS__); \
	} while(0)

#define EP0_CON		0
#define EP_MASK		0xF

enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,          /* enumerating */
	USB_SPEED_LOW, USB_SPEED_FULL,      /* usb 1.1 */
	USB_SPEED_HIGH,             /* usb 2.0 */
	USB_SPEED_WIRELESS,         /* wireless (usb 2.5) */
	USB_SPEED_SUPER,            /* usb 3.0 */
};

#define USB_TYPE_MASK			(0x03 << 5)
#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

#define USB_RECIP_MASK			0x1f
#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03

#define USB_DEVICE_SELF_POWERED		0	/* (read only) */
#define USB_DEVICE_TEST_MODE		2	/* (wired high speed only) */
#define USB_DEVICE_BATTERY		2	/* (wireless) */
#define USB_DEVICE_B_HNP_ENABLE		3	/* (otg) dev may initiate HNP */
#define USB_DEVICE_WUSB_DEVICE		3	/* (wireless)*/
#define USB_DEVICE_A_HNP_SUPPORT	4	/* (otg) RH port supports HNP */
#define USB_DEVICE_A_ALT_HNP_SUPPORT	5	/* (otg) other RH port does */
#define USB_DEVICE_DEBUG_MODE		6	/* (special devices only) */

#define UDELAY udelay
#undef  MIN
#define MIN(x, y)  ((x > y) ? y : x)

struct dwc2_udc	*the_controller;

/* Max packet size*/
static unsigned int ep0_fifo_size = 64;
static unsigned int ep_fifo_size =  512;

static int reset_available = 1;
static struct usb_device_instance *udc_device = NULL;

#define EP0_DMA_BUF_SIZE		512
#define EP_DMA_BUF_SIZE			512
#define EP_DMA_MAX_RX_SIZE		16384
#define EP_DMA_MAX_TX_SIZE		16384

#define DWC2_IS_ALIGNED(x)	!(x & 0x3)

#pragma arm section zidata = "ZIDATA_AREA_5"
static uint8 tx_dma_buf[EP_DMA_MAX_TX_SIZE] __attribute__((aligned(4)));
#pragma arm section zidata

static uint8 ep0_dma_buf[EP0_DMA_BUF_SIZE] __attribute__((aligned(4)));
static uint8 ep_dma_buf[EP_DMA_BUF_SIZE] __attribute__((aligned(4)));
static struct urb tmp_ep0_urb __attribute__((aligned(4)));
static struct urb *ep0_urb = &tmp_ep0_urb;
static uint32 usb_ctrl_dma_addr; /* = & ep0_urb->device_request, maybe need alignment */
struct usb_endpoint_instance *ep0_endpoint;

struct ep_status tx_ep;
struct ep_status rx_ep;
int g_rx_dma_is_started = 0;

/*
  Local declarations.
*/
static int dwc2_ep_enable(struct dwc2_ep *ep);
static int dwc2_queue(struct dwc2_ep *ep, int in);
static void dwc2_ep0_setup(struct dwc2_udc *dev);
static void dwc2_xfer_done(struct dwc2_ep *ep, int status);
static void dwc2_stop_activity(struct dwc2_udc *dev);
static int dwc2_udc_init(struct dwc2_udc *dev);
static void dwc2_udc_set_address(struct dwc2_udc *dev, unsigned char address);
static void dwc2_reconfig_usbd(struct dwc2_udc *dev, int init);
static void dwc2_update_endpoints(struct dwc2_udc *dev, enum usb_device_speed speed);
static int dwc2_udc_set_halt(struct dwc2_ep *ep, int value);
static void dwc2_udc_set_nak(struct dwc2_ep *ep);

int dwc2_platform_init(void);
/***********************************************************/

static void   *regs_otg;
static struct dwc2_usbotg_reg *reg = (struct dwc2_usbotg_reg *)(REG_USBC_BASE);

/***********************************************************/

/* Bulk-Only Mass Storage Reset (class-specific request) */
#define GET_MAX_LUN_REQUEST	0xFE
#define BOT_RESET_REQUEST	0xFF

/***********************************************************/

static int dwc2_setdma_rx(struct dwc2_ep *ep)
{
	uint32 ctrl;
	uint32 pktcnt;
	uint32 ep_num = ep_index(ep);
	uint32 *buf = (uint32 *)(ep->buf + ep->actual);
	uint32 length = ep->length - ep->actual;

	ep->done = 0;

	if (ep_num == EP0_CON || length == 0)
		pktcnt = 1;
	else
		pktcnt = (length - 1)/(ep->maxpacket) + 1;

	ctrl =  readl(&reg->out_endp[ep_num].doepctl);
	if (ep_num != 0) {
		ctrl = (ctrl & ~DEPCTL_TYPE_MASK) |
			(ep->bmAttributes << DEPCTL_TYPE_BIT);
		ctrl = (ctrl & ~DEPCTL_MPS_MASK) |
			(ep->maxpacket << DEPCTL_MPS_BIT);
		ctrl |= DEPCTL_USBACTEP;
		if (ctrl & DEPCTL_DPID_MASK)
			ctrl |= DEPCTL_SETD1PID;
		else
			ctrl |= DEPCTL_SETD0PID;
	}

	writel((unsigned int) ep->buf, &reg->out_endp[ep_num].doepdma);
	writel(DOEPT_SIZ_PKT_CNT(pktcnt) | DOEPT_SIZ_XFER_SIZE(length),
		   &reg->out_endp[ep_num].doeptsiz);
	writel(DEPCTL_EPENA|DEPCTL_CNAK|ctrl, &reg->out_endp[ep_num].doepctl);

	dwc2_debug(DEBUG_L3 != 0,
		   "%s: EP%d RX DMA start : DOEPDMA = 0x%x,"
		   "DOEPTSIZ = 0x%x, DOEPCTL = 0x%x\n"
		   "\tbuf = 0x0x%x, pktcnt = %d, xfersize = %d\n",
		   __func__, ep_num,
		   readl(&reg->out_endp[ep_num].doepdma),
		   readl(&reg->out_endp[ep_num].doeptsiz),
		   readl(&reg->out_endp[ep_num].doepctl),
		   (uint32)buf, pktcnt, length);

	return 0;
}

static void dwc2_ep0_start_recv(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[EP0_CON];
	
	ep->buf = (char *)usb_ctrl_dma_addr;
	ep->length = 0;/*sizeof(struct usb_device_request);*/
	ep->actual = 0;
	dwc2_setdma_rx(ep);
}

static int dwc2_setdma_tx(struct dwc2_ep *ep)
{
	uint32 ctrl = 0;
	uint32 pktcnt;
	uint32 ep_num = ep_index(ep);
	uint32 *buf = (uint32 *)(ep->buf + ep->actual);
	uint32 length = ep->length - ep->actual;

	dwc2_debug(DEBUG_L3,"EP->LENGTH = 0x%x, ep->actual = 0x%x, ep->maxpacket = 0x%x--------\n",
				ep->length, ep->actual, ep->maxpacket);
	dwc2_debug(DEBUG_L3,"ep->maxpacket = 0x%x \n", ep->maxpacket);

	ep->done = 0;

	if (length == 0)
		pktcnt = 1;
	else
		pktcnt = ( length - 1)/(ep->maxpacket) + 1;

	/* Flush the endpoint's Tx FIFO */
	writel(TX_FIFO_NUMBER(ep->fifo_num), &reg->grstctl);
	writel(TX_FIFO_NUMBER(ep->fifo_num) | TX_FIFO_FLUSH, &reg->grstctl);
	while (readl(&reg->grstctl) & TX_FIFO_FLUSH)
		;

	writel((unsigned long) buf, &reg->in_endp[ep_num].diepdma);
	writel(DIEPT_SIZ_PKT_CNT(pktcnt) | DIEPT_SIZ_XFER_SIZE(length),
		   &reg->in_endp[ep_num].dieptsiz);

	dwc2_debug(DEBUG_L3,"EP%d, DIEPTSIZE = 0x%x, DIEPDMA = 0x%x\n",
			ep_num, readl(&reg->in_endp[ep_num].dieptsiz),
			readl(&reg->in_endp[ep_num].diepdma));

	ctrl = readl(&reg->in_endp[ep_num].diepctl);

	dwc2_debug(DEBUG_L3,"%s:EP%d, DIEPCTL = 0x%x\n",__func__, ep_num, ctrl);
	/* Write the FIFO number to be used for this endpoint */
	ctrl &= DIEPCTL_TX_FIFO_NUM_MASK;
	ctrl |= DIEPCTL_TX_FIFO_NUM(ep->fifo_num);

	if (ep_num != 0) {
		ctrl = (ctrl & ~DEPCTL_TYPE_MASK) |
			(ep->bmAttributes << DEPCTL_TYPE_BIT);
		ctrl = (ctrl & ~DEPCTL_MPS_MASK) |
			(ep->maxpacket << DEPCTL_MPS_BIT);
		ctrl |= DEPCTL_USBACTEP;
		if (ctrl & DEPCTL_DPID_MASK)
			ctrl |= DEPCTL_SETD1PID;
		else
			ctrl |= DEPCTL_SETD0PID;
	}

	/* Clear reserved (Next EP) bits */
	ctrl = (ctrl&~(EP_MASK<<DEPCTL_NEXT_EP_BIT));

	writel(DEPCTL_EPENA|DEPCTL_CNAK|ctrl , &reg->in_endp[ep_num].diepctl);

#ifdef DEBUG
	{
		uint32 i;
		for (i = 0; i < 64; i++) {
			rprintf("%02x", ((uint8 *)buf)[i]);
			if ((i & 7) == 7)
				rprintf(" ");
		}
		rprintf("\n");
	}
#endif

	dwc2_debug(DEBUG_L3,
		"%s:EP%d TX DMA start : DIEPDMA = 0x%x,"
		"DIEPTSIZ = 0x%x, DIEPCTL = 0x%x\n"
		"\tbuf = 0x0x%x, pktcnt = %d, xfersize = %d, ep_fifo_num = %d, ctrl = 0x%x\n",
		__func__, ep_num,
		readl(&reg->in_endp[ep_num].diepdma),
		readl(&reg->in_endp[ep_num].dieptsiz),
		readl(&reg->in_endp[ep_num].diepctl),
		(uint32)buf, pktcnt, length, ep->fifo_num, ctrl);

	return 0;
}

static void dwc2_ep0_status_phase_in(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[EP0_CON];
	ep->buf = (void *)usb_ctrl_dma_addr;
	ep->length = 0;
	ep->actual = 0;
	dwc2_setdma_tx(ep);
}

static void dwc2_ep0_status_phase_out(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[EP0_CON];
	ep->buf = (void *)usb_ctrl_dma_addr;
	ep->length = 0;
	ep->actual = 0;
	dwc2_setdma_rx(ep);
}

static int dwc2_ep0_complete_rx(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[EP0_CON];
	uint32 ep_tsr = 0, xfer_size = 0;

	ep_tsr = readl(&reg->out_endp[EP0_CON].doeptsiz);
	xfer_size = ep->length - (ep_tsr & DOEPT_SIZ_XFER_SIZE_MAX_EP0);

	dwc2_debug(DEBUG_L3,
		   "%s: EP0 RX DMA done : DOEPDMA = 0x%x,"
		   "DOEPTSIZ = 0x%x, DOEPCTL = 0x%x\n",
		   __func__,
		   readl(&reg->out_endp[EP0_CON].doepdma),
		   readl(&reg->out_endp[EP0_CON].doeptsiz),
		   readl(&reg->out_endp[EP0_CON].doepctl));

	if (xfer_size < ep->length) {
		dwc2_debug(DEBUG_L3,
				"%s: EP0 RX DMA failed, miss %d bytes\n",
				__func__, ep->length - xfer_size);
	}
	dwc2_ep0_start_recv(dev);
	return 0;
}

static int dwc2_ep0_complete_tx(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[EP0_CON];
	uint32 ep_tsr = 0, xfer_size = 0;

	ep_tsr = readl(&reg->in_endp[EP0_CON].dieptsiz);
	xfer_size = ep->length - (ep_tsr & DOEPT_SIZ_XFER_SIZE_MAX_EP0);

	dwc2_debug(DEBUG_L3,
		"%s:EP0 TX DMA done : DIEPDMA = 0x%x,"
		"DIEPTSIZ = 0x%x, DIEPCTL = 0x%x\n",
		__func__,
		readl(&reg->in_endp[EP0_CON].diepdma),
		readl(&reg->in_endp[EP0_CON].dieptsiz),
		readl(&reg->in_endp[EP0_CON].diepctl));


	if (xfer_size < ep->length) {
		dwc2_debug(DEBUG_L3,
				"%s: EP0 TX DMA failed, miss %d bytes\n",
				__func__, ep->length - xfer_size);
	}

	dwc2_debug(DEBUG_L3, " ==> Wait for setup packet\n");
	dwc2_ep0_start_recv(dev);
	return 0;
}


/*
 *	dwc2_xfer_done - retire a request; caller blocked irqs
 */
static void dwc2_xfer_done(struct dwc2_ep *ep, int status)
{
	unsigned int stopped = ep->stopped;

	dwc2_debug(DEBUG_L1, "%s: EP%d 0x%x, stopped = %d\n",
		  __func__, ep_index(ep), (uint32)ep, stopped);

	ep->stopped = 1;

#ifdef DEBUG
	{
		int i, len = ep->actual;

		rprintf("pkt[%d] = ", len);
		if (len > 64)
			len = 64;
		for (i = 0; i < len; i++) {
			rprintf("%02x", ((uint8 *)ep->buf)[i]);
			if ((i & 7) == 7)
				rprintf(" ");
		}
		rprintf("\n");
	}
#endif
	ep->done = 1;

	ep->stopped = stopped;
}

static void dwc2_complete_rx(struct dwc2_udc *dev, uint8 ep_num)
{
	struct dwc2_ep *ep = &dev->ep[ep_num];
	uint32 ep_tsr = 0, xfer_size = 0, is_short = 0;

	if (ep_num == 0) {
		dwc2_ep0_complete_rx(dev);
		return;
	}

	ep_tsr = readl(&reg->out_endp[ep_num].doeptsiz);
	xfer_size = ep->length - (ep_tsr & DOEPT_SIZ_XFER_SIZE_MAX_EP);

	/*
	 * NOTE:
	 *
	 * Please be careful with proper buffer allocation for USB request,
	 * which needs to be aligned to CACHELINE_SIZE, not only
	 * with starting address, but also its size shall be a cache line
	 * multiplication.
	 *
	 * This will prevent from corruption of data allocated immediatelly
	 * before or after the buffer.
	 *
	 * For armv7, the cache_v7.c provides proper code to emit "ERROR"
	 * message to warn users.
	 */

	ep->actual += MIN(xfer_size, ep->length - ep->actual);
	is_short = !!(xfer_size % ep->maxpacket);

	dwc2_debug(DEBUG_L3 != 0,
		   "%s: RX DMA done : ep = %d, rx bytes = %d/%d, "
		   "is_short = %d, DOEPTSIZ = 0x%x, remained bytes = %d\n",
		   __func__, ep_num, ep->actual, ep->length,
		   is_short, ep_tsr, ep->length - ep->actual);

	if (is_short || ep->actual == ep->length) {
		dwc2_xfer_done(ep, 0);
	} else {
		/* continue transfer */
		dwc2_setdma_rx(ep);
	}
}

static void dwc2_complete_tx(struct dwc2_udc *dev, uint8 ep_num)
{
	struct dwc2_ep *ep = &dev->ep[ep_num];
	uint32 ep_tsr = 0, xfer_size = 0;

	if (ep_num == 0) {
		dwc2_ep0_complete_tx(dev);
		return;
	}

	ep_tsr = readl(&reg->in_endp[ep_num].dieptsiz);
	xfer_size = ep->length;
	ep->actual += MIN(xfer_size, ep->length - ep->actual);

	dwc2_debug(DEBUG_L3,
		"%s: TX DMA done : ep = %d, tx bytes = %d/%d, "
		"DIEPTSIZ = 0x%x, remained bytes = %d\n",
		__func__, ep_num, ep->actual, ep->length,
		ep_tsr, ep->length - ep->actual);

	if (ep->actual == ep->length) {
		dwc2_xfer_done(ep, 0);
	} else {
		/* continue transfer */
		dwc2_setdma_tx(ep);
	}
}

static void dwc2_inep_intr(struct dwc2_udc *dev)
{
	uint32 ep_intr, ep_intr_status;
	uint8 ep_num = 0;

	ep_intr = readl(&reg->daint);
	dwc2_debug(DEBUG_L3,
		"*** %s: EP In interrupt : DAINT = 0x%x\n", __func__, ep_intr);

	ep_intr &= DAINT_MASK;

	while (ep_intr) {
		if (ep_intr & DAINT_IN_EP_INT(1)) {
			ep_intr_status = readl(&reg->in_endp[ep_num].diepint);
			dwc2_debug(DEBUG_L3,
				   "\tEP%d-IN : DIEPINT = 0x%x\n",
				   ep_num, ep_intr_status);

			/* Interrupt Clear */
			writel(ep_intr_status, &reg->in_endp[ep_num].diepint);

			if (ep_num == 0 && ep_intr_status & SETUP_PKT) {
				dwc2_debug(DEBUG_L3, "IN EP0 timeout\n");
			}

			if (ep_intr_status & AHB_ERROR) {
				dwc2_debug(DEBUG_L3, "IN EP%d AHB_ERROR\n", ep_num);
			}

			if (ep_intr_status & EPDISBLD) {
				dwc2_debug(DEBUG_L3, "IN EP%d DISABLED\n", ep_num);
			}

			if (ep_intr_status & TRANSFER_DONE) {
				dwc2_debug(DEBUG_L3, "IN EP%d XFER COMPLETE\n", ep_num);

				dwc2_complete_tx(dev, ep_num);
			}
		}
		ep_num++;
		ep_intr >>= 1;
	}
}

static void dwc2_outep_intr(struct dwc2_udc *dev)
{
	uint32 ep_intr, ep_intr_status;
	uint8 ep_num = 0;

	ep_intr = readl(&reg->daint);
	dwc2_debug(DEBUG_L3 != 0,
		   "*** %s: EP OUT interrupt : DAINT = 0x%x\n",
		   __func__, ep_intr);

	ep_intr = (ep_intr >> DAINT_OUT_BIT) & DAINT_MASK;

	while (ep_intr) {
		if (ep_intr & 0x1) {
			ep_intr_status = readl(&reg->out_endp[ep_num].doepint);
			dwc2_debug(DEBUG_L3 != 0,
				   "\tEP%d-OUT : DOEPINT = 0x%x\n",
				   ep_num, ep_intr_status);

			/* Interrupt Clear */
			writel(ep_intr_status, &reg->out_endp[ep_num].doepint);

			/* Don't process TRANSFER_DONE interrupt if it is a setup packet */
			if (ep_num == 0 &&
				(ep_intr_status & (CTRL_OUT_EP_SETUP_PHASE_DONE | SETUP_PKT))) {
				ep_intr_status &= ~TRANSFER_DONE;
			}

			if (ep_intr_status & AHB_ERROR) {
				dwc2_debug(DEBUG_L3, "OUT EP%d AHB_ERROR\n", ep_num);
			}

			if (ep_intr_status & EPDISBLD) {
				dwc2_debug(DEBUG_L3, "OUT EP%d DISABLED\n", ep_num);
			}

			if (ep_intr_status & TRANSFER_DONE) {
				dwc2_debug(DEBUG_L3, "OUT EP%d TRANSFER DONE\n", ep_num);
				dwc2_complete_rx(dev, ep_num);
			}

			/* Handle setup phase done (control ep) */
			if (ep_num == 0 && ep_intr_status & CTRL_OUT_EP_SETUP_PHASE_DONE) {
				dwc2_debug(DEBUG_L3, "OUT EP%d SETUP PHASE DONE\n", ep_num);
				dwc2_ep0_complete_rx(dev);
				dwc2_ep0_setup(dev);
			}
		}
		ep_num++;
		ep_intr >>= 1;
	}
}

/*
 *	usb client interrupt handler.
 */
static int dwc2_udc_irq(int irq, void *_dev)
{
	struct dwc2_udc *dev = _dev;
	uint32 intr_status;
	uint32 usb_status, gintmsk;

	intr_status = readl(&reg->gintsts);
	gintmsk = readl(&reg->gintmsk);

	if (intr_status & GINTMSK_INIT)
		dwc2_debug(DEBUG_L2,
		  "\n*** %s : GINTSTS=0x%x, GINTMSK : 0x%x,"
		  "DAINT : 0x%x, DAINTMSK : 0x%x\n",
		  __func__, intr_status, gintmsk,
		  readl(&reg->daint), readl(&reg->daintmsk));
	else
		return 0;

	/* EP INTR */
	if (intr_status & INT_IN_EP) {
		writel(INT_IN_EP, &reg->gintsts);
		dwc2_inep_intr(dev);
	}

	if (intr_status & INT_OUT_EP) {
		writel(INT_OUT_EP, &reg->gintsts);
		dwc2_outep_intr(dev);
	}

	if (intr_status & INT_ENUMDONE) {
		dwc2_debug(DEBUG_L0, "%s:   *** Speed Detection: ", __func__);

		writel(INT_ENUMDONE, &reg->gintsts);
		usb_status = (readl(&reg->dsts) & 0x6);

		if (usb_status & (USB_FULL_30_60MHZ | USB_FULL_48MHZ)) {
			dwc2_debug(DEBUG_L0,
				   "Full Speed\n");
			dwc2_update_endpoints(dev, USB_SPEED_FULL);

		} else {
			dwc2_debug(DEBUG_L0,
				"High Speed: (dsts = 0x%x)\n",
				usb_status);
			dwc2_update_endpoints(dev, USB_SPEED_HIGH);
		}
	}

	if (intr_status & INT_DISCONN) {
		dwc2_debug(DEBUG_L2, "\tDISCONN interrupt\n");
		usbd_device_event_irq(udc_device, DEVICE_RESET, 0);
		writel(INT_DISCONN, &reg->gintsts);
		reset_available = 1;
	}

	if (intr_status & INT_EARLY_SUSPEND) {
		dwc2_debug(DEBUG_L2, "\tEarly suspend interrupt\n");
		writel(INT_EARLY_SUSPEND, &reg->gintsts);
	}

	if (intr_status & INT_SUSPEND) {
		usb_status = readl(&reg->dsts);
		dwc2_debug(DEBUG_L2,
			"\tSuspend interrupt :(DSTS):0x%x\n", usb_status);
		writel(INT_SUSPEND, &reg->gintsts);
		usbd_device_event_irq(udc_device, DEVICE_BUS_INACTIVE, 0);
		dwc2_stop_activity(dev);
		reset_available = 1;
	}

	if (intr_status & INT_RESUME) {
		dwc2_debug(DEBUG_L2, "\tResume interrupt\n");
		writel(INT_RESUME, &reg->gintsts);
		usbd_device_event_irq(udc_device, DEVICE_BUS_ACTIVITY, 0);
	}

	if (intr_status & INT_RESET) {
		usb_status = readl(&reg->gotgctl);
		dwc2_debug(DEBUG_L2,
			"\tReset interrupt - (GOTGCTL):0x%x\n", usb_status);
		writel(INT_RESET, &reg->gintsts);
		usbd_device_event_irq(udc_device, DEVICE_RESET, 0);
		if ((usb_status & 0xc0000) == (0x3 << 18)) {
			if (reset_available) {
				dwc2_debug(DEBUG_L2,
					"\t\tOTG core got reset!!\n");
				dwc2_reconfig_usbd(dev, 0);
				reset_available = 0;
			}
			dwc2_ep0_start_recv(dev);
		}
	}

	return 0;
}

/** Queue one request
 *  Kickstart transfer if needed
 */
static int dwc2_queue(struct dwc2_ep *ep, int in)
{
	uint32 ep_num = ep_index(ep);
	ep->done = 0;
	ep->actual = 0;

	/* kickstart this i/o queue? */
	dwc2_debug(DEBUG_L3, "\n*** %s: EP%d-%s , len = %d, buf = 0x%x"
		", stopped = %d\n",
		__func__, ep_num, ep_is_in(ep) ? "in" : "out",
		ep->length, (uint32)ep->buf,
		ep->stopped);

#ifdef DEBUG
	{
		int i, len = ep->length;

		rprintf("pkt = ");
		if (len > 64)
			len = 64;
		for (i = 0; i < len; i++) {
			rprintf("%02x ", ((uint8 *)ep->buf)[i]);
		}
		rprintf("\n");
	}
#endif

	if (!ep->stopped) {
		if (in) {
			dwc2_setdma_rx(ep);
		} else {
			dwc2_setdma_tx(ep);
		}
	}

	return 0;
}

/****************************************************************/
/* End Point 0 related functions								*/
/****************************************************************/

/**
 * udc_set_address - set the USB address for this device
 * @address:
 *
 * Called from control endpoint function
 * after it decodes a set address setup packet.
 */
static void dwc2_udc_set_address(struct dwc2_udc *dev, unsigned char address)
{
	uint32 ctrl = readl(&reg->dcfg);
	writel(DEVICE_ADDRESS(address) | ctrl, &reg->dcfg);

	dwc2_debug(DEBUG_L2 != 0,
		   "%s: USB OTG 2.0 Device address=%d, DCFG=0x%x\n",
		   __func__, address, readl(&reg->dcfg));

	dev->usb_address = address;
}

static void dwc2_udc_get_address(struct dwc2_udc *dev)
{
	dev->usb_address = (readl(&reg->dcfg) >> 4) & 0x7f;
}

static void dwc2_udc_ep0_set_stall(struct dwc2_ep *ep)
{
	struct dwc2_udc *dev;
	uint32 ep_ctrl = 0;

	dev = ep->dev;
	ep_ctrl = readl(&reg->in_endp[EP0_CON].diepctl);

	/* set the disable and stall bits */
	if (ep_ctrl & DEPCTL_EPENA)
		ep_ctrl |= DEPCTL_EPDIS;

	ep_ctrl |= DEPCTL_STALL;

	writel(ep_ctrl, &reg->in_endp[EP0_CON].diepctl);

	dwc2_debug(DEBUG_L2 != 0,
		   "%s: set ep%d stall, DIEPCTL0 = 0x0x%x\n",
		   __func__, ep_index(ep), readl(&reg->in_endp[EP0_CON].diepctl));
	/*
	 * The application can only set this bit, and the core clears it,
	 * when a SETUP token is received for this endpoint
	 */
	dwc2_ep0_start_recv(dev);
}

static int dwc2_udc_get_status(struct dwc2_udc *dev,
		struct usb_device_request *crq)
{
	struct dwc2_ep *ep = &dev->ep[0];
	uint8 ep_num = crq->wIndex & 0x7F;
	uint16 g_status = 0;

	dwc2_debug(DEBUG_L1 != 0,
		   "%s: *** USB_REQ_GET_STATUS\n", __func__);
	dwc2_debug(DEBUG_L1 != 0,
			"crq->brequest:0x%x\n", crq->bmRequestType & USB_RECIP_MASK);
	switch (crq->bmRequestType & USB_RECIP_MASK) {
	case USB_RECIP_INTERFACE:
		g_status = 0;
		dwc2_debug(DEBUG_L1 != 0,
			   "\tGET_STATUS:USB_RECIP_INTERFACE, g_stauts = %d\n",
			   g_status);
		break;

	case USB_RECIP_DEVICE:
		g_status = 0x1; /* Self powered */
		dwc2_debug(DEBUG_L1 != 0,
			   "\tGET_STATUS: USB_RECIP_DEVICE, g_stauts = %d\n",
			   g_status);
		break;

	case USB_RECIP_ENDPOINT:
		if (crq->wLength > 2) {
			dwc2_debug(DEBUG_L1 != 0,
				   "\tGET_STATUS:Not support EP or wLength\n");
			return 1;
		}

		g_status = dev->ep[ep_num].stopped;
		dwc2_debug(DEBUG_L1 != 0,
			   "\tGET_STATUS: USB_RECIP_ENDPOINT, g_stauts = %d\n",
			   g_status);

		break;

	default:
		return 1;
	}

	memcpy(crq, &g_status, sizeof(g_status));
	ep->buf = (void *)usb_ctrl_dma_addr;
	ep->length = 2;
	ep->actual = 0;
	dwc2_setdma_tx(ep);
	return 0;
}

static void dwc2_udc_set_nak(struct dwc2_ep *ep)
{
	uint8		ep_num;
	uint32		ep_ctrl = 0;

	ep_num = ep_index(ep);
	dwc2_debug(DEBUG_L1, "%s: ep_num = %d, ep_type = %d\n", __func__, ep_num, ep->ep_type);

	if (ep_is_in(ep)) {
		ep_ctrl = readl(&reg->in_endp[ep_num].diepctl);
		ep_ctrl |= DEPCTL_SNAK;
		writel(ep_ctrl, &reg->in_endp[ep_num].diepctl);
		dwc2_debug(DEBUG_L1, "%s: set NAK, DIEPCTL%d = 0x%x\n",
			__func__, ep_num, readl(&reg->in_endp[ep_num].diepctl));
	} else {
		ep_ctrl = readl(&reg->out_endp[ep_num].doepctl);
		ep_ctrl |= DEPCTL_SNAK;
		writel(ep_ctrl, &reg->out_endp[ep_num].doepctl);
		dwc2_debug(DEBUG_L1, "%s: set NAK, DOEPCTL%d = 0x%x\n",
			  __func__, ep_num, readl(&reg->out_endp[ep_num].doepctl));
	}

	return;
}


static void dwc2_udc_ep_set_stall(struct dwc2_ep *ep)
{
	uint8		ep_num;
	uint32		ep_ctrl = 0;

	ep_num = ep_index(ep);
	dwc2_debug(DEBUG_L1, "%s: ep_num = %d, ep_type = %d\n", __func__, ep_num, ep->ep_type);

	if (ep_is_in(ep)) {
		ep_ctrl = readl(&reg->in_endp[ep_num].diepctl);

		/* set the disable and stall bits */
		if (ep_ctrl & DEPCTL_EPENA)
			ep_ctrl |= DEPCTL_EPDIS;

		ep_ctrl |= DEPCTL_STALL;

		writel(ep_ctrl, &reg->in_endp[ep_num].diepctl);
		dwc2_debug(DEBUG_L1, "%s: set stall, DIEPCTL%d = 0x%x\n",
			  __func__, ep_num, readl(&reg->in_endp[ep_num].diepctl));

	} else {
		ep_ctrl = readl(&reg->out_endp[ep_num].doepctl);

		/* set the stall bit */
		ep_ctrl |= DEPCTL_STALL;

		writel(ep_ctrl, &reg->out_endp[ep_num].doepctl);
		dwc2_debug(DEBUG_L1, "%s: set stall, DOEPCTL%d = 0x%x\n",
			  __func__, ep_num, readl(&reg->out_endp[ep_num].doepctl));
	}

	return;
}

static void dwc2_udc_ep_clear_stall(struct dwc2_ep *ep)
{
	uint8		ep_num;
	uint32		ep_ctrl = 0;

	ep_num = ep_index(ep);
	dwc2_debug(DEBUG_L1, "%s: ep_num = %d, ep_type = %d\n", __func__, ep_num, ep->ep_type);

	if (ep_is_in(ep)) {
		ep_ctrl = readl(&reg->in_endp[ep_num].diepctl);

		/* clear stall bit */
		ep_ctrl &= ~DEPCTL_STALL;

		/*
		 * USB Spec 9.4.5: For endpoints using data toggle, regardless
		 * of whether an endpoint has the Halt feature set, a
		 * ClearFeature(ENDPOINT_HALT) request always results in the
		 * data toggle being reinitialized to DATA0.
		 */
		if (ep->bmAttributes == USB_ENDPOINT_XFER_INT
			|| ep->bmAttributes == USB_ENDPOINT_XFER_BULK) {
			ep_ctrl |= DEPCTL_SETD0PID; /* DATA0 */
		}

		writel(ep_ctrl, &reg->in_endp[ep_num].diepctl);
		dwc2_debug(DEBUG_L1, "%s: cleared stall, DIEPCTL%d = 0x%x\n",
			__func__, ep_num, readl(&reg->in_endp[ep_num].diepctl));

	} else {
		ep_ctrl = readl(&reg->out_endp[ep_num].doepctl);

		/* clear stall bit */
		ep_ctrl &= ~DEPCTL_STALL;

		if (ep->bmAttributes == USB_ENDPOINT_XFER_INT
			|| ep->bmAttributes == USB_ENDPOINT_XFER_BULK) {
			ep_ctrl |= DEPCTL_SETD0PID; /* DATA0 */
		}

		writel(ep_ctrl, &reg->out_endp[ep_num].doepctl);
		dwc2_debug(DEBUG_L1, "%s: cleared stall, DOEPCTL%d = 0x%x\n",
			  __func__, ep_num, readl(&reg->out_endp[ep_num].doepctl));
	}

	return;
}

static int dwc2_udc_set_halt(struct dwc2_ep *ep, int value)
{
	uint8 ep_num = ep_index(ep);

	if (ep->done == 0)
		dwc2_xfer_done(ep, 0);

	dwc2_debug(DEBUG_L1, "%s: ep_num = %d, value = %d\n", __func__, ep_num, value);

	if (value == 0) {
		ep->stopped = 0;
		dwc2_udc_ep_clear_stall(ep);
	} else {
		ep->stopped = 1;
		dwc2_udc_ep_set_stall(ep);
	}

	return 0;
}

static void dwc2_udc_ep_activate(struct dwc2_ep *ep)
{
	uint8 ep_num;
	uint32 ep_ctrl = 0, daintmsk = 0;

	ep_num = ep_index(ep);

	/* Read DEPCTLn register */
	if (ep_is_in(ep)) {
		ep_ctrl = readl(&reg->in_endp[ep_num].diepctl);
		daintmsk = 1 << ep_num;
	} else {
		ep_ctrl = readl(&reg->out_endp[ep_num].doepctl);
		daintmsk = (1 << ep_num) << DAINT_OUT_BIT;
	}

	dwc2_debug(DEBUG_L1, "%s: EPCTRL%d = 0x%x, ep_is_in = %d\n",
		__func__, ep_num, ep_ctrl, ep_is_in(ep));

	/* If the EP is already active don't change the EP Control
	 * register. */
	if (!(ep_ctrl & DEPCTL_USBACTEP)) {
		ep_ctrl = (ep_ctrl & ~DEPCTL_TYPE_MASK) |
			(ep->bmAttributes << DEPCTL_TYPE_BIT);
		ep_ctrl = (ep_ctrl & ~DEPCTL_MPS_MASK) |
			(ep->maxpacket << DEPCTL_MPS_BIT);
		ep_ctrl |= (DEPCTL_SETD0PID | DEPCTL_USBACTEP | DEPCTL_SNAK);

		if (ep_is_in(ep)) {
			writel(ep_ctrl, &reg->in_endp[ep_num].diepctl);
			dwc2_debug(DEBUG_L1, "%s: USB Ative EP%d, DIEPCTRL%d = 0x%x\n",
				  __func__, ep_num, ep_num,
				  readl(&reg->in_endp[ep_num].diepctl));
		} else {
			writel(ep_ctrl, &reg->out_endp[ep_num].doepctl);
			dwc2_debug(DEBUG_L1, "%s: USB Ative EP%d, DOEPCTRL%d = 0x%x\n",
				  __func__, ep_num, ep_num,
				  readl(&reg->out_endp[ep_num].doepctl));
		}
	}

	/* Unmask EP Interrtupt */
	writel(readl(&reg->daintmsk)|daintmsk, &reg->daintmsk);
	dwc2_debug(DEBUG_L1, "%s: DAINTMSK = 0x%x\n", __func__, readl(&reg->daintmsk));

}

static int dwc2_udc_clear_feature(struct dwc2_ep *ep, struct usb_device_request *crq)
{
	uint8 ep_num = ep_index(ep);

	dwc2_debug(DEBUG_L1 != 0,
		   "%s: ep_num = %d, is_in = %d\n",
		   __func__, ep_num, ep_is_in(ep));

	if (crq->wLength != 0) {
		dwc2_debug(DEBUG_L1 != 0,
			   "\tCLEAR_FEATURE: wLength is not zero.....\n");
		return 1;
	}

	switch (crq->bmRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		switch (crq->wValue) {
		case USB_DEVICE_REMOTE_WAKEUP:
			dwc2_debug(DEBUG_L1 != 0,
				   "\tOFF:USB_DEVICE_REMOTE_WAKEUP\n");
			break;

		case USB_DEVICE_TEST_MODE:
			dwc2_debug(DEBUG_L1 != 0,
				   "\tCLEAR_FEATURE: USB_DEVICE_TEST_MODE\n");
			/** @todo Add CLEAR_FEATURE for TEST modes. */
			break;
		}
		break;

	case USB_RECIP_ENDPOINT:
		dwc2_debug(DEBUG_L1 != 0,
			   "\tCLEAR_FEATURE:USB_RECIP_ENDPOINT, wValue = %d\n",
			   crq->wValue);

		if (crq->wValue == USB_ENDPOINT_HALT) {
			if (ep_num == 0) {
				dwc2_udc_ep0_set_stall(ep);
				return 0;
			}

			dwc2_udc_ep_clear_stall(ep);
			dwc2_udc_ep_activate(ep);
			ep->stopped = 0;
		}
		break;
	}

	return 0;
}

static int dwc2_udc_set_feature(struct dwc2_ep *ep, struct usb_device_request *crq)
{
	uint8 ep_num = ep_index(ep);

	dwc2_debug(DEBUG_L1 != 0,
		   "%s: *** USB_REQ_SET_FEATURE , ep_num = %d\n",
			__func__, ep_num);

	if (crq->wLength != 0) {
		dwc2_debug(DEBUG_L1 != 0,
			   "\tSET_FEATURE: wLength is not zero.....\n");
		return 1;
	}

	switch (crq->bmRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		switch (crq->wValue) {
		case USB_DEVICE_REMOTE_WAKEUP:
			dwc2_debug(DEBUG_L1 != 0,
				   "\tSET_FEATURE:USB_DEVICE_REMOTE_WAKEUP\n");
			break;
		case USB_DEVICE_B_HNP_ENABLE:
			dwc2_debug(DEBUG_L1 != 0,
				   "\tSET_FEATURE: USB_DEVICE_B_HNP_ENABLE\n");
			break;

		case USB_DEVICE_A_HNP_SUPPORT:
			/* RH port supports HNP */
			dwc2_debug(DEBUG_L1 != 0,
				   "\tSET_FEATURE:USB_DEVICE_A_HNP_SUPPORT\n");
			break;

		case USB_DEVICE_A_ALT_HNP_SUPPORT:
			/* other RH port does */
			dwc2_debug(DEBUG_L1 != 0,
				   "\tSET: USB_DEVICE_A_ALT_HNP_SUPPORT\n");
			break;
		}

		return 0;

	case USB_RECIP_INTERFACE:
		dwc2_debug(DEBUG_L1 != 0,
			   "\tSET_FEATURE: USB_RECIP_INTERFACE\n");
		break;

	case USB_RECIP_ENDPOINT:
		dwc2_debug(DEBUG_L1 != 0,
			   "\tSET_FEATURE: USB_RECIP_ENDPOINT\n");
		if (crq->wValue == USB_ENDPOINT_HALT) {
			if (ep_num == 0) {
				dwc2_udc_ep0_set_stall(ep);
				return 0;
			}
			ep->stopped = 1;
			dwc2_udc_ep_set_stall(ep);
		}
		return 0;
	}

	return 1;
}

/*
 * WAIT_FOR_SETUP (OUT_PKT_RDY)
 */
static void dwc2_ep0_setup(struct dwc2_udc *dev)
{
	struct dwc2_ep *ep = &dev->ep[0];
	struct usb_device_request *crq = &ep0_urb->device_request;
	uint8 ep_num;
	int ret;

	dwc2_debug(DEBUG_L2 != 0,
		   "%s: bmRequestType = 0x%x(%s), bRequest = 0x%x"
		   "\twLength = 0x%x, wValue = 0x%x, wIndex= 0x%x\n",
		   __func__, crq->bmRequestType,
		   (crq->bmRequestType & USB_DIR_IN) ? "IN" : "OUT",
		   crq->bRequest,
		   crq->wLength, crq->wValue, crq->wIndex);

#ifdef DEBUG
	{
		int i, len = sizeof(*crq);
		char *p = (char *)crq;

		rprintf("pkt = ");
		for (i = 0; i < len; i++) {
			rprintf("%02x", ((uint8 *)p)[i]);
			if ((i & 7) == 7)
				rprintf(" ");
		}
		rprintf("\n");
	}
#endif

	if (crq->bRequest == GET_MAX_LUN_REQUEST &&
		crq->wLength != 1) {
		dwc2_debug(DEBUG_L1 != 0,
			   "\t%s:GET_MAX_LUN_REQUEST:invalid",
			   __func__);
		dwc2_debug(DEBUG_L1 != 0,
			   "wLength = %d, setup returned\n",
			   crq->wLength);

		dwc2_udc_ep0_set_stall(ep);

		return;
	} else if (crq->bRequest == BOT_RESET_REQUEST &&
		 crq->wLength != 0) {
		/* Bulk-Only *mass storge reset of class-specific request */
		dwc2_debug(DEBUG_L1 != 0,
			   "%s:BOT Rest:invalid wLength =%d, setup returned\n",
			   __func__, crq->wLength);

		dwc2_udc_ep0_set_stall(ep);

		return;
	}

	/* Set direction of EP0 */
	if (crq->bmRequestType & USB_DIR_IN) {
		ep->bEndpointAddress |= USB_DIR_IN;
	} else {
		ep->bEndpointAddress &= ~USB_DIR_IN;
	}
	/* cope with automagic for some standard requests. */
	dev->req_std = (crq->bmRequestType & USB_TYPE_MASK)
		== USB_TYPE_STANDARD;

	dev->req_pending = 1;

	/* Handle some SETUP packets ourselves */
	if (dev->req_std) {
		switch (crq->bRequest) {
		case USB_REQ_SET_ADDRESS:
			dwc2_debug(DEBUG_L0 != 0,
			   "%s: *** USB_REQ_SET_ADDRESS (%d)\n",
			   __func__, crq->wValue);
			if (crq->bmRequestType
				!= (USB_TYPE_STANDARD | USB_RECIP_DEVICE))
				break;

			dwc2_udc_set_address(dev, crq->wValue);
			dwc2_ep0_status_phase_in(dev);
			return;

		case USB_REQ_SET_CONFIGURATION:
			dwc2_debug(DEBUG_L0 != 0,
				   "%s: *** USB_REQ_SET_CONFIGURATION (%d)\n",
				   __func__, crq->wValue);
			dwc2_debug(DEBUG_L0 != 0, "================= READY!\n");
			usbd_device_event_irq(udc_device, DEVICE_CONFIGURED, 0);
			break;

		case USB_REQ_GET_DESCRIPTOR:
			dwc2_debug(DEBUG_L0 != 0,
				   "%s: *** USB_REQ_GET_DESCRIPTOR (0x%x)\n",
				   __func__, crq->wValue);
			break;

		case USB_REQ_SET_INTERFACE:
			dwc2_debug(DEBUG_L0 != 0,
				   "%s: *** USB_REQ_SET_INTERFACE (%d)\n",
				   __func__, crq->wValue);
			break;

		case USB_REQ_GET_CONFIGURATION:
			dwc2_debug(DEBUG_L0 != 0,
				   "%s: *** USB_REQ_GET_CONFIGURATION\n",
				   __func__);
			break;

		case USB_REQ_GET_STATUS:
			if (!dwc2_udc_get_status(dev, crq))
				return;

			break;

		case USB_REQ_CLEAR_FEATURE:
			ep_num = crq->wIndex & 0x7f;

			if (!dwc2_udc_clear_feature(&dev->ep[ep_num], crq))
				return;

			break;

		case USB_REQ_SET_FEATURE:
			ep_num = crq->wIndex & 0x7f;

			if (!dwc2_udc_set_feature(&dev->ep[ep_num], crq))
				return;

			break;

		default:
			dwc2_debug(DEBUG_L1 != 0,
				   "%s: *** Default of crq->bRequest=0x%x"
				   "happened.\n", __func__, crq->bRequest);
			break;
		}
	}

	ep0_urb->buffer = ep0_dma_buf;
	ep0_urb->buffer_length = EP0_DMA_BUF_SIZE;
	ret = ep0_recv_setup(ep0_urb);
	if (ret < 0) {
		/* setup processing failed, force stall */
		dwc2_udc_ep0_set_stall(ep);

		dwc2_debug(DEBUG_L2 != 0,
			   "\tdev->driver->setup failed (%d),"
				" bRequest = %d\n",
				ret, crq->bRequest);

	} else if (crq->bmRequestType & USB_REQ_DIRECTION_MASK) {
		/* send data to host */
		ep->buf = ep0_urb->buffer;
		ep->length = ep0_urb->actual_length;
		ep->actual = 0;
		dwc2_setdma_tx(ep);

	} else if (dev->req_pending) {
		dev->req_pending = 0;
		dwc2_debug(DEBUG_L2 != 0,
			   "\tdev->req_pending...\n");
	}

	if (crq->bmRequestType & USB_REQ_DIRECTION_MASK)
		dwc2_ep0_status_phase_out(dev);
	else
		dwc2_ep0_status_phase_in(dev);
}

/*
 *	udc_reinit - initialize software state
 */
static void dwc2_udc_reinit(struct dwc2_udc *dev)
{
	unsigned int i;

	/* basic endpoint records init */
	for (i = 0; i < DWC2_MAX_ENDPOINTS; i++) {
		struct dwc2_ep *ep = &dev->ep[i];

		ep->buf = 0;
		ep->length = 0;
		ep->actual = 0;
		ep->done = 0;
		ep->stopped = 0;
	}

	/* the rest was statically initialized, and is read-only */
}

#define BYTES2MAXP(x)	(x / 8)
#define MAXP2BYTES(x)	(x * 8)

/* until it's enabled, this UDC should be completely invisible
 * to any USB host.
 */
static int dwc2_udc_init(struct dwc2_udc *dev)
{
	dwc2_reconfig_usbd(dev, 1);
	reset_available = 0;

	dwc2_debug(DEBUG_L0 != 0,
		   "DWC2 USB 2.0 OTG Controller Core Initialized : 0x%x\n",
			readl(&reg->gintmsk));

	dev->speed = USB_SPEED_UNKNOWN;

	return 0;
}

static void dwc2_stop_activity(struct dwc2_udc *dev)
{
	int i;

	dev->speed = USB_SPEED_UNKNOWN;

	/* prevent new request submissions, kill any outstanding requests  */
	for (i = 1; i < DWC2_MAX_ENDPOINTS; i++) {
		struct dwc2_ep *ep = &dev->ep[i];
		if (ep->done == 0)
			dwc2_xfer_done(ep, -1);
		ep->stopped = 1;
	}

	/* report disconnect; the driver is already quiesced */
	usbd_device_event_irq(udc_device, DEVICE_RESET, 0);
}

static void dwc2_reconfig_usbd(struct dwc2_udc *dev, int init)
{
	/* 2. Soft-reset OTG Core and then unreset again. */
	int i;
	unsigned int uTemp = 0;
	uint32 dflt_gusbcfg;
	uint32 rx_fifo_sz, tx_fifo_sz1, tx_fifo_sz2, np_tx_fifo_sz;

	writel(CORE_SOFT_RESET, &reg->grstctl);
	dwc2_debug(DEBUG_L0, "Reseting OTG controller\n");

	dflt_gusbcfg =
		1<<30	   /* Force device mode */
		|0<<15		/* PHY Low Power Clock sel*/
		|1<<14		/* Non-Periodic TxFIFO Rewind Enable*/
		|0x9<<10	/* Turnaround time*/
		|0<<9 | 0<<8	/* [0:HNP disable,1:HNP enable][ 0:SRP disable*/
				/* 1:SRP enable] H1= 1,1*/
		|0<<7		/* Ulpi DDR sel*/
		|0<<6		/* 0: high speed utmi+, 1: full speed serial*/
		|0<<4		/* 0: utmi+, 1:ulpi*/
#ifdef CONFIG_USB_GADGET_DWC2_OTG_PHY_BUS_WIDTH_8
		|0<<3		/* phy i/f  0:8bit, 1:16bit*/
#else
		|1<<3		/* phy i/f  0:8bit, 1:16bit*/
#endif
		|0x7<<0;	/* HS/FS Timeout**/

	writel(dflt_gusbcfg, &reg->gusbcfg);

	if (init) {
		/* 3. Put the OTG device core in the disconnected state.*/
		uTemp = readl(&reg->dctl);
		uTemp |= SOFT_DISCONNECT;
		writel(uTemp, &reg->dctl);

		UDELAY(5000); /* delay 5ms to allow bus see disconnect */

		/* 4. Make the OTG device core to connect state.*/
		uTemp = readl(&reg->dctl);
		uTemp = uTemp & ~SOFT_DISCONNECT;
		writel(uTemp, &reg->dctl);

		/* 5. Configure OTG Core to initial settings of device mode.*/
		/* [][1: full speed(30Mhz) 0:high speed]*/
		writel(EP_MISS_CNT(1) | DEV_SPEED_HIGH_SPEED_20 | 1<<24, &reg->dcfg);
		//writel(EP_MISS_CNT(1) | DEV_SPEED_FULL_SPEED_20, &reg->dcfg);
	}

	UDELAY(20000);

	/* 6. Unmask the core interrupts*/
	writel(GINTMSK_INIT, &reg->gintmsk);

	/* 7. Set NAK bit of EP0, EP1, EP2*/
	writel(DEPCTL_EPDIS|DEPCTL_SNAK, &reg->out_endp[EP0_CON].doepctl);
	writel(DEPCTL_EPDIS|DEPCTL_SNAK , &reg->in_endp[EP0_CON].diepctl);

	for (i = 1; i < DWC2_MAX_ENDPOINTS; i++) {
		writel(DEPCTL_EPDIS|DEPCTL_SNAK, &reg->out_endp[i].doepctl);
		writel(DEPCTL_EPDIS|DEPCTL_SNAK, &reg->in_endp[i].diepctl);
	}

	/* 8. Unmask EPO interrupts*/
	writel(((1 << EP0_CON) << DAINT_OUT_BIT)
		   | (1 << EP0_CON), &reg->daintmsk);

	/* 9. Unmask device OUT EP common interrupts*/
	writel(DOEPMSK_INIT, &reg->doepmsk);

	/* 10. Unmask device IN EP common interrupts*/
	writel(DIEPMSK_INIT, &reg->diepmsk);

	rx_fifo_sz = RX_FIFO_SIZE;
	np_tx_fifo_sz = NPTX_FIFO_SIZE;
	tx_fifo_sz1 = PTX_FIFO_SIZE1;
	tx_fifo_sz2 = PTX_FIFO_SIZE2;

	/* 11. Set Rx FIFO Size (in 32-bit words) */
	writel(rx_fifo_sz, &reg->grxfsiz);

	/* 12. Set Non Periodic Tx FIFO Size */
	writel((np_tx_fifo_sz << 16) | rx_fifo_sz,
		   &reg->gnptxfsiz);

	/* EP1 & EP2 */
	for (i = 1; i < 3; i++)
		writel((rx_fifo_sz + np_tx_fifo_sz + tx_fifo_sz1*(i-1)) |
			(tx_fifo_sz1 << 16), &reg->dieptxf[i-1]);

	/* EP3, EP4, ... */
	for (i = 3; i < DWC2_MAX_HW_ENDPOINTS; i++)
		writel((rx_fifo_sz + np_tx_fifo_sz + (tx_fifo_sz1*2) + tx_fifo_sz2*(i-1)) |
			(tx_fifo_sz2 << 16), &reg->dieptxf[i-1]);

	/* Flush the RX FIFO */
	writel(RX_FIFO_FLUSH, &reg->grstctl);
	while (readl(&reg->grstctl) & RX_FIFO_FLUSH)
		dwc2_debug(DEBUG_L1, "%s: waiting for DWC2_UDC_OTG_GRSTCTL\n", __func__);

	/* Flush all the Tx FIFO's */
	writel(TX_FIFO_FLUSH_ALL, &reg->grstctl);
	writel(TX_FIFO_FLUSH_ALL | TX_FIFO_FLUSH, &reg->grstctl);
	while (readl(&reg->grstctl) & TX_FIFO_FLUSH)
		dwc2_debug(DEBUG_L1, "%s: waiting for DWC2_UDC_OTG_GRSTCTL\n", __func__);

	/* 13. Clear NAK bit of EP0, EP1, EP2*/
	/* For Slave mode*/
	/* EP0: Control OUT */
	writel(DEPCTL_EPDIS | DEPCTL_CNAK,
		   &reg->out_endp[EP0_CON].doepctl);

	/* 14. Initialize OTG Link Core.*/
	writel(GAHBCFG_INIT, &reg->gahbcfg);
}

static void dwc2_update_endpoints(struct dwc2_udc *dev, enum usb_device_speed speed)
{
	unsigned int ep_ctrl;
	int i;

	if (speed == USB_SPEED_HIGH) {
		ep0_fifo_size = 64;
		ep_fifo_size = 512;
		dev->speed = USB_SPEED_HIGH;
	} else {
		ep0_fifo_size = 64;
		ep_fifo_size = 64;
		dev->speed = USB_SPEED_FULL;
	}

	for (i = 1; i < DWC2_MAX_ENDPOINTS; i++) {
		dev->ep[i].maxpacket = ep_fifo_size;
		dev->ep[i].stopped = 0;
		dev->ep[i].done = 1;

		if (ep_is_in(&dev->ep[i]))
			ep_ctrl = readl(&reg->in_endp[i].diepctl);
		else
			ep_ctrl = readl(&reg->out_endp[i].doepctl);

		ep_ctrl = (ep_ctrl & ~DEPCTL_TYPE_MASK) |
			(dev->ep[i].bmAttributes << DEPCTL_TYPE_BIT);
		ep_ctrl = (ep_ctrl & ~DEPCTL_MPS_MASK) |
			(dev->ep[i].maxpacket << DEPCTL_MPS_BIT);
		ep_ctrl |= (DEPCTL_SETD0PID | DEPCTL_USBACTEP | DEPCTL_SNAK);

		if (ep_is_in(&dev->ep[i])) {
			writel(ep_ctrl, &reg->in_endp[i].diepctl);
			dwc2_debug(DEBUG_L2, "%s: EP%d DIEPCTL 0x%x\n",
					__func__, i, readl(&reg->in_endp[i].diepctl));
		} else {
			writel(ep_ctrl, &reg->out_endp[i].doepctl);
			dwc2_debug(DEBUG_L2, "%s: EP%d DOEPCTL 0x%x\n",
					__func__, i, readl(&reg->out_endp[i].doepctl));
		}
	}

	dev->ep[0].maxpacket = ep0_fifo_size;

	/* EP0 - Control IN (64 bytes)*/
	ep_ctrl = readl(&reg->in_endp[EP0_CON].diepctl);
	writel(ep_ctrl|(0<<0), &reg->in_endp[EP0_CON].diepctl);

	/* EP0 - Control OUT (64 bytes)*/
	ep_ctrl = readl(&reg->out_endp[EP0_CON].doepctl);
	writel(ep_ctrl|(0<<0), &reg->out_endp[EP0_CON].doepctl);
}

static int dwc2_ep_enable(struct dwc2_ep *ep)
{
	ep->done = 1;
	ep->stopped = 0;

	/* Reset halt state */
	dwc2_udc_set_nak(ep);
	dwc2_udc_set_halt(ep, 0);

	dwc2_udc_ep_activate(ep);

	dwc2_debug(DEBUG_L2, "%s: enabled EP %d, stopped = %d, maxpacket = %d\n",
		  __func__, ep_index(ep), ep->stopped, ep->maxpacket);
	return 0;
}

static struct dwc2_udc memory;

static void dwc2_udc_init_memory(struct dwc2_udc *dev)
{
	memset(dev, 0, sizeof(struct dwc2_udc));
	
	dev->usb_address = 0;
	
	dev->ep[0].maxpacket = EP0_FIFO_SIZE;
	dev->ep[0].dev = &memory;
	dev->ep[0].bEndpointAddress = 0;
	dev->ep[0].bmAttributes = 0;
	dev->ep[0].ep_type = 0;

	dev->ep[1].maxpacket = EP_FIFO_SIZE;
	dev->ep[1].dev = &memory;
	dev->ep[1].bEndpointAddress = USB_DIR_IN | 1;
	dev->ep[1].bmAttributes = USB_ENDPOINT_XFER_INT;
	dev->ep[1].ep_type = ep_interrupt;
	dev->ep[1].fifo_num = 1;

	dev->ep[2].maxpacket = EP_FIFO_SIZE;
	dev->ep[2].dev = &memory;
	dev->ep[2].bEndpointAddress = USB_DIR_OUT | 2;
	dev->ep[2].bmAttributes = USB_ENDPOINT_XFER_BULK;
	dev->ep[2].ep_type = ep_bulk_in;
	dev->ep[2].fifo_num = 2;

	dev->ep[3].maxpacket = EP_FIFO_SIZE;
	dev->ep[3].dev = &memory;
	dev->ep[3].bEndpointAddress = USB_DIR_IN | 3;
	dev->ep[3].bmAttributes = USB_ENDPOINT_XFER_BULK;
	dev->ep[3].ep_type = ep_bulk_out;
	dev->ep[3].fifo_num = 3;
}

/*
 *	probe - binds to the platform device
 */

static int dwc2_udc_probe(uint32 regbase)
{
	struct dwc2_udc *dev = &memory;
	int retval = 0;

	dwc2_debug(DEBUG_L1, "%s: regbase = 0x%x\n", __func__, regbase);

	dwc2_udc_init_memory(dev);
	reg = (struct dwc2_usbotg_reg *)regbase;

	regs_otg = (void *)regbase;

	the_controller = dev;

	usb_ctrl_dma_addr = (uint32) &ep0_urb->device_request;

	dwc2_udc_reinit(dev);

	return retval;
}

/************************************************************************************/
/********** udc layer *********************/

static int enabled;
void udc_irq(void)
{
	if (!enabled)
		return;

	dwc2_udc_irq(1, (void *)the_controller);
}

void udc_connect(void)
{
	/* noop */
}

void udc_disconnect(void)
{
	/* noop */
}

void udc_enable(struct usb_device_instance *device)
{
	/* Save the device structure pointer */
	udc_device = device;

	enabled = 1;
}

void udc_disable(void)
{
	enabled = 0;
}

int is_usbd_high_speed(void)
{
	if (!(readl(&reg->dcfg) & 0x3))
		return 1;
	else
		return 0;
}

void udc_startup_events(struct usb_device_instance *device)
{
	/* The DEVICE_INIT event puts the USB device in the state STATE_INIT. */
	usbd_device_event_irq(device, DEVICE_INIT, 0);

	/*
	 * The DEVICE_CREATE event puts the USB device in the state
	 * STATE_ATTACHED.
	 */
	usbd_device_event_irq(device, DEVICE_CREATE, 0);

	/* Resets the address to 0 */
	usbd_device_event_irq(device, DEVICE_RESET, 0);

	udc_enable(device);
}

void udc_hot_startup(struct usb_device_instance *device)
{
	struct dwc2_udc *dev = the_controller;

	usbd_device_event_irq(device, DEVICE_CONFIGURED, 0);
	udc_enable(device);

	dwc2_udc_get_address(dev);
	udc_device->address = dev->usb_address;
}

int udc_init(void)
{
	int ret;
	uint32 regbase = REG_USBC_BASE;

	ret = dwc2_platform_init();
	if (ret < 0)
		goto end;

	if (!dwc2_udc_probe(regbase)) {
		struct dwc2_udc *dev = the_controller;
		return dwc2_udc_init(dev);
	}

	ret = 0;
end:
	return ret;
}

int udc_soft_init(void)
{
	/* FIXME */
	return 0;
}

void udc_clear_halt(unsigned int ep, int is_in)
{
	struct dwc2_udc *dev = the_controller;
	dwc2_udc_set_halt(&dev->ep[ep], 0);
}

void udc_setup_ep(struct usb_device_instance *device,
		  uint32 ep, struct usb_endpoint_instance *endpoint)
{
	struct dwc2_udc *dev = the_controller;

	if (ep == 0) {
		dev->ep[0].bEndpointAddress = 0;
		dev->ep[0].bmAttributes = 0;
		dev->ep[0].ep_type = ep_control;
		ep0_endpoint = endpoint;
		ep0_endpoint->endpoint_address = 0xff;
		usbd_init_urb(ep0_urb, device, endpoint, ep0_dma_buf, EP0_DMA_BUF_SIZE);
	} else if (ep < DWC2_MAX_ENDPOINTS){
		int ep_addr = endpoint->endpoint_address;
		if (USB_DIR_IN == (ep_addr & USB_ENDPOINT_DIR_MASK)) {
			dev->ep[ep].bEndpointAddress = USB_DIR_IN | ep;
			dev->ep[ep].bmAttributes = endpoint->rcv_attributes;
			dev->ep[ep].ep_type = ep_bulk_out;
			dev->ep[ep].fifo_num = ep;
			dev->ep[ep].maxpacket = endpoint->tx_packetSize;
		} else {
			dev->ep[ep].bEndpointAddress = USB_DIR_OUT | ep;
			dev->ep[ep].bmAttributes = endpoint->tx_attributes;
			dev->ep[ep].ep_type = ep_bulk_in;
			dev->ep[ep].fifo_num = ep;
			dev->ep[ep].maxpacket = endpoint->rcv_packetSize;
		}

		dwc2_ep_enable(&dev->ep[ep]);
	} else {
		usberr("ERROR: invalid EP number %u\n", ep);
	}
}

int udc_endpoint_read(struct usb_endpoint_instance *endpoint, int short_return)
{
	struct dwc2_udc *dev = the_controller;
	struct dwc2_ep *ep;
	struct urb *urb;
	uint8 *data;
	uint32 xfer, last, total;
	uint32 ep_num;
	int aligned = 0;
	int ret = 0;

	if (!endpoint->rcv_urb || endpoint->state) {
		usberr("ERROR: %s, EP state %d\n", __func__, endpoint->state);
		return -1;
	}
	ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	ep = &dev->ep[ep_num];

	urb = endpoint->rcv_urb;
	if (urb->buffer == 0 || urb->buffer_length == 0)
		return 0;
	data = urb->buffer;
	last = urb->buffer_length;
	total = last;

	while(1) {
		aligned = DWC2_IS_ALIGNED((uint32)data);
		if (aligned) {
			ep->length = MIN(last, EP_DMA_MAX_RX_SIZE);
			ep->buf = data;
			/*invalidate_dcache_range(data, data+last);*/
		} else {
			ep->length = MIN(last, EP_DMA_BUF_SIZE);
			ep->buf = ep_dma_buf;
		}
		ret = dwc2_queue(ep, 1);
		if (ret) {
			usberr("ERROR: %s failed %d\n", __func__, ret);
			break;
		} else {
			while(!ep->done && !ep->stopped)
				udc_irq();
			xfer = ep->actual;
			if (!aligned)
				memcpy(data, ep_dma_buf, xfer);
			data += xfer;
			last -= xfer;
			dwc2_debug(DEBUG_L3,
					"############ recv %u bytes, last %u bytes.\n", xfer, last);
			if ((short_return && xfer > 0) || last == 0) {
				usbd_rcv_complete(endpoint, total-last, 0);
				break;
			}
			if (ep->stopped) {
				return -1;
				break;
			}
		}
	}

	return ret;
}

/* Called to start packet transmission. */
int udc_endpoint_write(struct usb_endpoint_instance *endpoint)
{
	struct dwc2_udc *dev = the_controller;
	struct dwc2_ep *ep;
	struct urb *urb;
	uint8 *data;
	uint32 xfer, last, total;
	uint32 ep_num;
	int aligned = 0;
	int ret = 0;


	if (!endpoint->tx_urb || endpoint->state) {
		usberr("ERROR: %s, EP state %d\n", __func__, endpoint->state);
		return -1;
	}
	ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	ep = &dev->ep[ep_num];

	urb = endpoint->tx_urb;
	if (urb->buffer == 0 || urb->buffer_length == 0)
		return 0;
	data = urb->buffer;
	last = urb->buffer_length;
	total = last;


	while (1) {
		aligned = DWC2_IS_ALIGNED((uint32)data);
		if (aligned) {
			ep->length = MIN(last, EP_DMA_MAX_TX_SIZE);
			ep->buf = data;
			/*flush_dcache_range(data, data+last);*/
		} else {
			ep->length = MIN(last, EP_DMA_BUF_SIZE);
			ep->buf = ep_dma_buf;
			memcpy(ep_dma_buf, data, ep->length);
			/*flush_dcache_range(ep_dma_buf, ep_dma_buf+ep->length);*/
		}
		ret = dwc2_queue(ep, 0);
		if (ret) {
			usberr("ERROR: %s failed %d\n", __func__, ret);
			break;
		} else {
			while(!ep->done && !ep->stopped)
				udc_irq();
			xfer = ep->actual;
			data += xfer;
			last -= xfer;
			dwc2_debug(DEBUG_L3,
					"############ send %u bytes, last %u bytes.\n", xfer, last);
			if (last == 0) {
				endpoint->last = total;
				usbd_tx_complete(endpoint);
				break;
			}
			if (ep->stopped) {
				ret = -1;
				break;
			}
		}
	}

	return ret;
}

static void udc_rx_complete(uint8 ep_num)
{
	struct ep_status *ep = &rx_ep;
	uint32 ep_tsr = 0, xfer_size = 0, is_short = 0;

	ep_tsr = readl(&reg->out_endp[ep_num].doeptsiz);
	xfer_size = ep->length - (ep_tsr & DOEPT_SIZ_XFER_SIZE_MAX_EP);

	ep->actual += MIN(xfer_size, ep->length - ep->actual);
	//is_short = !!(xfer_size % ep->maxpacket);

	dwc2_debug(DEBUG_L3 != 0,
		   "%s: RX DMA done : ep = %d, rx bytes = %d/%d, "
		   "is_short = %d, DOEPTSIZ = 0x%x, remained bytes = %d\n",
		   __func__, ep_num, ep->actual, ep->length,
		   is_short, ep_tsr, ep->length - ep->actual);

	//if (is_short || ep->actual == ep->length) {
		ep->done = 1;
		g_rx_dma_is_started = 0;
	//} else {
	//	rprintf("%s: ERR\n", __func__);
	//}
}

static void udc_tx_complete(uint8 ep_num)
{
    struct ep_status *ep = &tx_ep;
    uint32 ep_tsr = 0, xfer_size = 0;

    ep_tsr = readl(&reg->in_endp[ep_num].dieptsiz);
    xfer_size = ep->length;
    ep->actual += MIN(xfer_size, ep->length - ep->actual);

    dwc2_debug(DEBUG_L3,
            "%s: TX DMA done: ep = %d, tx bytes = %d/%d, "
            "DIEPTSIZ = 0x%x, remain bytes = %d\n",
            __func__, ep_num, ep->actual, ep->length,
            ep_tsr, ep->length - ep->actual);

    if (ep->actual == ep->length) {
        ep->done = 1;
    } else if (ep_tsr != 0) {
        rprintf("%s: ERR\n", __func__);
    }
}

static void udc_outep_intr(void)
{
	uint32 ep_intr, ep_intr_status;
	uint8 ep_num = 0;

	ep_intr = readl(&reg->daint);
	//serial_puts("outep\n");
	dwc2_debug(DEBUG_L3,
		   "## %s: EP-OUT interrupt: DAINT = 0x%x\n", __func__, ep_intr);

	ep_intr = (ep_intr >> DAINT_OUT_BIT) & DAINT_MASK;

	while (ep_intr) {
		if (ep_intr & 0x1) {
			ep_intr_status = readl(&reg->out_endp[ep_num].doepint);
			dwc2_debug(DEBUG_L3,
				   "EP%d-OUT: DOEPINT = 0x%x\n", ep_num, ep_intr_status);

			/* Interrupt Clear */
			writel(ep_intr_status, &reg->out_endp[ep_num].doepint);

			if (ep_intr_status & AHB_ERROR) {
				dwc2_debug(1, "EP%d-OUT ahb error\n", ep_num);
			}

			if (ep_intr_status & EPDISBLD) {
				dwc2_debug(1, "EP%d-OUT disable\n", ep_num);
			}

			if (ep_intr_status & TRANSFER_DONE) {
				dwc2_debug(1, "EP%d-OUT transfer done\n", ep_num);
				if (ep_num == 0)
				{
				    rprintf("drop ep0-out data\n");
				}
				else
				{
				    udc_rx_complete(ep_num);
				}
			}
		}
		ep_num++;
		ep_intr >>= 1;
	}
}


static void udc_inep_intr(void)
{
	uint32 ep_intr, ep_intr_status;
	uint8 ep_num = 0;

	ep_intr = readl(&reg->daint);
	//serial_puts("inep\n");
	dwc2_debug(DEBUG_L3,
		"## %s: EP-In interrupt: DAINT = 0x%x\n", __func__, ep_intr);

	ep_intr &= DAINT_MASK;

	while (ep_intr) {
		if (ep_intr & DAINT_IN_EP_INT(1)) {
			ep_intr_status = readl(&reg->in_endp[ep_num].diepint);
			dwc2_debug(DEBUG_L3,
				   "##EP%d-IN : DIEPINT = 0x%x\n", ep_num, ep_intr_status);

			/* Interrupt Clear */
			writel(ep_intr_status, &reg->in_endp[ep_num].diepint);

			if (ep_num == 0 && ep_intr_status & SETUP_PKT) {
				dwc2_debug(DEBUG_L3, "IN EP0 timeout\n");
			}

			if (ep_intr_status & AHB_ERROR) {
				dwc2_debug(DEBUG_L3, "IN EP%d-IN AHB_ERROR\n", ep_num);
			}

			if (ep_intr_status & EPDISBLD) {
				dwc2_debug(DEBUG_L3, "IN EP%d-IN DISABLED\n", ep_num);
			}

			if (ep_intr_status & TRANSFER_DONE) {
				dwc2_debug(DEBUG_L3, "IN EP%d-IN XFER COMPLETE\n", ep_num);
				udc_tx_complete(ep_num);
			}
		}
		ep_num++;
		ep_intr >>= 1;
	}
}

static int udc_raw_irq(void)
{
	uint32 intr_status, gintmsk;
	uint32 usb_status;

	intr_status = readl(&reg->gintsts);
	gintmsk = readl(&reg->gintmsk);

	if (intr_status & GINTMSK_INIT)
		dwc2_debug(DEBUG_L2,
		  "\n## %s : GINTSTS=0x%x, GINTMSK: 0x%x, DAINT: 0x%x, DAINTMSK: 0x%x\n",
		  __func__, intr_status, gintmsk, readl(&reg->daint), readl(&reg->daintmsk));
	else
		return 0;

	 /* EPin */
	if (intr_status & INT_IN_EP) {
		writel(INT_IN_EP, &reg->gintsts);
		udc_inep_intr();
	}

       /* EPout */
	if (intr_status & INT_OUT_EP) {
		writel(INT_OUT_EP, &reg->gintsts);
		udc_outep_intr();
	}

	return 0;
}

static int udc_set_rx_dma(struct ep_status *ep)
{
    uint32 ctrl;
    uint32 pktcnt;
    uint32 ep_num = 2; // EP2-OUT
    uint32 *buf = (uint32 *)(ep->buf + ep->actual);
    uint32 length = ep->length - ep->actual;

    if (g_rx_dma_is_started) {
        //rprintf("rx dma is started\n");
        return 0;
    }
    //serial_puts("rx\n");

    ep->done = 0;

    if (length == 0)
        pktcnt = 1;
    else
        pktcnt = (length - 1) / (ep->maxpacket) + 1;

    ctrl = readl(&reg->out_endp[ep_num].doepctl);
    if (ep_num != 0) {
        ctrl = (ctrl & ~DEPCTL_TYPE_MASK) |
                (ep->bmAttributes << DEPCTL_TYPE_BIT);
        ctrl = (ctrl & ~DEPCTL_MPS_MASK) |
                (ep->maxpacket << DEPCTL_MPS_BIT);
        ctrl |= DEPCTL_USBACTEP;
        if (ctrl & DEPCTL_DPID_MASK)
            ctrl |= DEPCTL_SETD1PID;
        else
            ctrl |= DEPCTL_SETD0PID;
    }

    writel((unsigned int) ep->buf, &reg->out_endp[ep_num].doepdma);
    writel(DOEPT_SIZ_PKT_CNT(pktcnt) | DOEPT_SIZ_XFER_SIZE(length),
            &reg->out_endp[ep_num].doeptsiz);
    writel(DEPCTL_EPENA|DEPCTL_CNAK|ctrl, &reg->out_endp[ep_num].doepctl);

    dwc2_debug(DEBUG_L3,
            "%s: EP%d-OUT RX DMA start : DOEPDMA = 0x%x,"
            "DOEPTSIZ = 0x%x, DOEPCTL = 0x%x\n"
            "buf = 0x%x, pktcnt = %d, xfersize = %d\n",
            __func__, ep_num,
            readl(&reg->out_endp[ep_num].doepdma),
            readl(&reg->out_endp[ep_num].doeptsiz),
            readl(&reg->out_endp[ep_num].doepctl),
            (uint32)buf, pktcnt, length);

    g_rx_dma_is_started = 1;

    return 0;
}

static int udc_set_tx_dma(struct ep_status *ep)
{
	uint32 ctrl = 0;
	uint32 pktcnt;
	uint32 ep_num = 1; // EP1-IN
	uint32 *buf = (uint32 *)(ep->buf + ep->actual);
	uint32 length = ep->length - ep->actual;

	//serial_puts("tx\n");
	dwc2_debug(DEBUG_L3, "length = %u, actual = %u, maxpacket = %u\n",
		    ep->length, ep->actual, ep->maxpacket);

	ep->done = 0;

	if (length == 0)
		pktcnt = 1;
	else
		pktcnt = (length - 1) / (ep->maxpacket) + 1;

	/* Flush the endpoint's Tx FIFO */
	writel(TX_FIFO_NUMBER(ep->fifo_num), &reg->grstctl);
	writel(TX_FIFO_NUMBER(ep->fifo_num) | TX_FIFO_FLUSH, &reg->grstctl);
	while (readl(&reg->grstctl) & TX_FIFO_FLUSH);

	writel((unsigned long) buf, &reg->in_endp[ep_num].diepdma);
	writel(DIEPT_SIZ_PKT_CNT(pktcnt) | DIEPT_SIZ_XFER_SIZE(length),
		   &reg->in_endp[ep_num].dieptsiz);

	dwc2_debug(DEBUG_L3, "EP%d, DIEPTSIZE = 0x%x, DIEPDMA = 0x%x\n",
                ep_num, readl(&reg->in_endp[ep_num].dieptsiz),
                readl(&reg->in_endp[ep_num].diepdma));

	ctrl = readl(&reg->in_endp[ep_num].diepctl);

	dwc2_debug(DEBUG_L3, "%s:EP%d, DIEPCTL = 0x%x\n",__func__, ep_num, ctrl);

	/* Write the FIFO number to be used for this endpoint */
	ctrl &= DIEPCTL_TX_FIFO_NUM_MASK;
	ctrl |= DIEPCTL_TX_FIFO_NUM(ep->fifo_num);

	if (ep_num != 0) {
		ctrl = (ctrl & ~DEPCTL_TYPE_MASK) |
			(ep->bmAttributes << DEPCTL_TYPE_BIT);
		ctrl = (ctrl & ~DEPCTL_MPS_MASK) |
			(ep->maxpacket << DEPCTL_MPS_BIT);
		ctrl |= DEPCTL_USBACTEP;
		if (ctrl & DEPCTL_DPID_MASK)
			ctrl |= DEPCTL_SETD1PID;
		else
			ctrl |= DEPCTL_SETD0PID;
	}

	/* Clear reserved (Next EP) bits */
	ctrl = (ctrl&~(EP_MASK<<DEPCTL_NEXT_EP_BIT));
	writel(DEPCTL_EPENA|DEPCTL_CNAK|ctrl , &reg->in_endp[ep_num].diepctl);

	dwc2_debug(DEBUG_L3,
		"%s:EP%d TX DMA start : DIEPDMA = 0x%x,"
		"DIEPTSIZ = 0x%x, DIEPCTL = 0x%x\n"
		"\tbuf = 0x0x%x, pktcnt = %d, xfersize = %d, ep_fifo_num = %d, ctrl = 0x%x\n",
		__func__, ep_num,
		readl(&reg->in_endp[ep_num].diepdma),
		readl(&reg->in_endp[ep_num].dieptsiz),
		readl(&reg->in_endp[ep_num].diepctl),
		(uint32)buf, pktcnt, length, ep->fifo_num, ctrl);

	return 0;
}

extern int rx_buf_actual;
int udc_endpoint_raw_read(uint8 *buf, int len)
{
    uint32 remain = len;
    int ret = 0;

    if (len > EP_DMA_MAX_RX_SIZE) {
        rprintf("Raw read ERR: len %d > EP_DMA_MAX_TX_SIZE\n", len);
        return -1;
    }

    rx_ep.buf = buf;
    rx_ep.length = len;
    rx_ep.actual = 0;
    rx_ep.maxpacket = 64;
    rx_ep.fifo_num = 2;
    rx_ep.bmAttributes = USB_ENDPOINT_XFER_BULK;
    //dwc2_debug(1, "\nRaw read: buf 0x%x, len %d\n", rx_ep.buf, rx_ep.length);

    udc_set_rx_dma(&rx_ep);
    udc_raw_irq();

    remain -= rx_ep.actual;
    rx_buf_actual = rx_ep.actual;
    if (rx_buf_actual > 0)
        //dwc2_debug(DEBUG_L3, "## recv %u bytes, remain %u bytes.\n", rx_ep.actual, remain);
        rprintf("rx %u\n", rx_buf_actual);

    return 0;
}

int udc_endpoint_raw_wirte(uint8 *buf, int len)
{
    uint32 remain = len;
    int ret = 0, i;

    if (len > EP_DMA_MAX_TX_SIZE) {
         rprintf("Raw write ERR: len %d > EP_DMA_MAX_TX_SIZE\n", len);
         return -1;
    }
    
    memcpy(tx_dma_buf, buf, len);
    tx_ep.buf = tx_dma_buf;
    tx_ep.length = len;
    tx_ep.actual = 0;
    tx_ep.maxpacket = 64;
    tx_ep.fifo_num = 1;
    tx_ep.bmAttributes = USB_ENDPOINT_XFER_BULK;
    dwc2_debug(DEBUG_L3, "\nRaw write: buf 0x%x, len %d\n", tx_ep.buf, tx_ep.length);

    rprintf("Send:\n");
    for (i = 0; i < len; i++)
    {
        rprintf("%02x ", tx_dma_buf[i]);
        if (((i %16) == 0) && (i != 0))
            rprintf("\n");
    }
	rprintf("\n\n");
    
    udc_set_tx_dma(&tx_ep);
    while (!tx_ep.done)
        udc_raw_irq();
    
    remain -= tx_ep.actual;
    dwc2_debug(DEBUG_L3, "## send %u bytes, remain %u bytes.\n", tx_ep.actual, remain);
    //rprintf("tx %u\n", tx_ep.actual);
}

/*
 * Designware DWC2 on-chip full/high speed USB device controllers
 * Copyright (C) 2005 for Samsung Electronics
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __DWC2_UDC_OTG_PRIV__
#define __DWC2_UDC_OTG_PRIV__

/* USB2.0 OTG Controller register */
struct dwc2_usbotg_phy {
	uint32 phypwr;
	uint32 phyclk;
	uint32 rstcon;
};

/* Device Logical IN Endpoint-Specific Registers */
struct dwc2_dev_in_endp {
	uint32 diepctl;
	uint8  res1[4];
	uint32 diepint;
	uint8  res2[4];
	uint32 dieptsiz;
	uint32 diepdma;
	//uint8  res3[4]; /* dtxfsts */
	uint32 dtxfsts;
	uint32 diepdmab;
};

/* Device Logical OUT Endpoint-Specific Registers */
struct dwc2_dev_out_endp {
	uint32 doepctl;
	uint8  res1[4];
	uint32 doepint;
	uint8  res2[4];
	uint32 doeptsiz;
	uint32 doepdma;
	uint8  res3[4];
	uint32 doepdmab;
};

struct ep_fifo {
	uint32 fifo;
	uint8  res[4092];
};

/* USB2.0 OTG Controller register */
struct dwc2_usbotg_reg {
	/* Core Global Registers */
	uint32 gotgctl; /* OTG Control & Status */
	uint32 gotgint; /* OTG Interrupt */
	uint32 gahbcfg; /* Core AHB Configuration */
	uint32 gusbcfg; /* Core USB Configuration */
	uint32 grstctl; /* Core Reset */
	uint32 gintsts; /* Core Interrupt */
	uint32 gintmsk; /* Core Interrupt Mask */
	uint32 grxstsr; /* Receive Status Debug Read/Status Read */
	uint32 grxstsp; /* Receive Status Debug Pop/Status Pop */
	uint32 grxfsiz; /* Receive FIFO Size */
	uint32 gnptxfsiz; /* Non-Periodic Transmit FIFO Size */
	uint8  res1[216];
	uint32 dieptxf[15]; /* Device Periodic Transmit FIFO size register */
	uint8  res2[1728];
	/* Device Configuration */
	uint32 dcfg; /* Device Configuration Register */
	uint32 dctl; /* Device Control */
	uint32 dsts; /* Device Status */
	uint8  res3[4];
	uint32 diepmsk; /* Device IN Endpoint Common Interrupt Mask */
	uint32 doepmsk; /* Device OUT Endpoint Common Interrupt Mask */
	uint32 daint; /* Device All Endpoints Interrupt */
	uint32 daintmsk; /* Device All Endpoints Interrupt Mask */
	uint8  res4[224];
	struct dwc2_dev_in_endp in_endp[16];
	struct dwc2_dev_out_endp out_endp[16];
	uint8  res5[768];
	struct ep_fifo ep[16];
};

/*===================================================================== */
/*definitions related to CSR setting */

/* DWC2_UDC_OTG_GOTGCTL */
#define B_SESSION_VALID		(0x1<<19)
#define A_SESSION_VALID		(0x1<<18)

/* DWC2_UDC_OTG_GAHBCFG */
#define PTXFE_HALF			(0<<8)
#define PTXFE_ZERO			(1<<8)
#define NPTXFE_HALF			(0<<7)
#define NPTXFE_ZERO			(1<<7)
#define MODE_SLAVE			(0<<5)
#define MODE_DMA			(1<<5)
#define BURST_SINGLE			(0<<1)
#define BURST_INCR			(1<<1)
#define BURST_INCR4			(3<<1)
#define BURST_INCR8			(5<<1)
#define BURST_INCR16			(7<<1)
#define GBL_INT_UNMASK			(1<<0)
#define GBL_INT_MASK			(0<<0)

/* DWC2_UDC_OTG_GRSTCTL */
#define AHB_MASTER_IDLE		(1u<<31)
#define CORE_SOFT_RESET		(0x1<<0)

/* DWC2_UDC_OTG_GINTSTS/DWC2_UDC_OTG_GINTMSK core interrupt register */
#define INT_RESUME			(1u<<31)
#define INT_DISCONN			(0x1<<29)
#define INT_CONN_ID_STS_CNG		(0x1<<28)
#define INT_OUT_EP			(0x1<<19)
#define INT_IN_EP			(0x1<<18)
#define INT_ENUMDONE			(0x1<<13)
#define INT_RESET			(0x1<<12)
#define INT_SUSPEND			(0x1<<11)
#define INT_EARLY_SUSPEND		(0x1<<10)
#define INT_NP_TX_FIFO_EMPTY		(0x1<<5)
#define INT_RX_FIFO_NOT_EMPTY		(0x1<<4)
#define INT_SOF			(0x1<<3)
#define INT_DEV_MODE			(0x0<<0)
#define INT_HOST_MODE			(0x1<<1)
#define INT_GOUTNakEff			(0x01<<7)
#define INT_GINNakEff			(0x01<<6)

#define FULL_SPEED_CONTROL_PKT_SIZE	8
#define FULL_SPEED_BULK_PKT_SIZE	64

#define HIGH_SPEED_CONTROL_PKT_SIZE	64
#define HIGH_SPEED_BULK_PKT_SIZE	512

#define RX_FIFO_SIZE			(1024)
#define NPTX_FIFO_SIZE			(1024)
#define PTX_FIFO_SIZE1			(768) /* for ep1, ep2 */
#define PTX_FIFO_SIZE2			(256)

#define DEPCTL_TXFNUM_0		(0x0<<22)
#define DEPCTL_TXFNUM_1		(0x1<<22)
#define DEPCTL_TXFNUM_2		(0x2<<22)
#define DEPCTL_TXFNUM_3		(0x3<<22)
#define DEPCTL_TXFNUM_4		(0x4<<22)

/* Enumeration speed */
#define USB_HIGH_30_60MHZ		(0x0<<1)
#define USB_FULL_30_60MHZ		(0x1<<1)
#define USB_LOW_6MHZ			(0x2<<1)
#define USB_FULL_48MHZ			(0x3<<1)

/* DWC2_UDC_OTG_GRXSTSP STATUS */
#define OUT_PKT_RECEIVED		(0x2<<17)
#define OUT_TRANSFER_COMPLELTED	(0x3<<17)
#define SETUP_TRANSACTION_COMPLETED	(0x4<<17)
#define SETUP_PKT_RECEIVED		(0x6<<17)
#define GLOBAL_OUT_NAK			(0x1<<17)

/* DWC2_UDC_OTG_DCTL device control register */
#define NORMAL_OPERATION		(0x1<<0)
#define SOFT_DISCONNECT		(0x1<<1)

/* DWC2_UDC_OTG_DAINT device all endpoint interrupt register */
#define DAINT_OUT_BIT			(16)
#define DAINT_MASK			(0xFFFF)

/* DWC2_UDC_OTG_DIEPCTL0/DOEPCTL0 device
   control IN/OUT endpoint 0 control register */
#define DEPCTL_EPENA			(0x1<<31)
#define DEPCTL_EPDIS			(0x1<<30)
#define DEPCTL_SETD1PID		(0x1<<29)
#define DEPCTL_SETD0PID		(0x1<<28)
#define DEPCTL_SNAK			(0x1<<27)
#define DEPCTL_CNAK			(0x1<<26)
#define DEPCTL_STALL			(0x1<<21)
#define DEPCTL_TYPE_BIT		(18)
#define DEPCTL_TYPE_MASK		(0x3<<18)
#define DEPCTL_CTRL_TYPE		(0x0<<18)
#define DEPCTL_ISO_TYPE		(0x1<<18)
#define DEPCTL_BULK_TYPE		(0x2<<18)
#define DEPCTL_INTR_TYPE		(0x3<<18)
#define DEPCTL_DPID_MASK		(0x1<<16)
#define DEPCTL_USBACTEP		(0x1<<15)
#define DEPCTL_NEXT_EP_BIT		(11)
#define DEPCTL_MPS_BIT			(0)
#define DEPCTL_MPS_MASK		(0x7FF)

#define DEPCTL0_MPS_64			(0x0<<0)
#define DEPCTL0_MPS_32			(0x1<<0)
#define DEPCTL0_MPS_16			(0x2<<0)
#define DEPCTL0_MPS_8			(0x3<<0)
#define DEPCTL_MPS_BULK_512		(512<<0)
#define DEPCTL_MPS_INT_MPS_16		(16<<0)

#define DIEPCTL0_NEXT_EP_BIT		(11)


/* DWC2_UDC_OTG_DIEPMSK/DOEPMSK device IN/OUT endpoint
   common interrupt mask register */
/* DWC2_UDC_OTG_DIEPINTn/DOEPINTn device IN/OUT endpoint interrupt register */
#define SETUP_PKT					(0x1<<15)
#define BACK2BACK_SETUP_RECEIVED	(0x1<<6)
#define INTKNEPMIS			(0x1<<5)
#define INTKN_TXFEMP			(0x1<<4)
#define NON_ISO_IN_EP_TIMEOUT		(0x1<<3)
#define CTRL_OUT_EP_SETUP_PHASE_DONE	(0x1<<3)
#define AHB_ERROR			(0x1<<2)
#define EPDISBLD			(0x1<<1)
#define TRANSFER_DONE			(0x1<<0)

#define USB_PHY_CTRL_EN0                (0x1 << 0)

/* OPHYPWR */
#define PHY_0_SLEEP                     (0x1 << 5)
#define OTG_DISABLE_0                   (0x1 << 4)
#define ANALOG_PWRDOWN                  (0x1 << 3)
#define FORCE_SUSPEND_0                 (0x1 << 0)

/* URSTCON */
#define HOST_SW_RST                     (0x1 << 4)
#define PHY_SW_RST1                     (0x1 << 3)
#define PHYLNK_SW_RST                   (0x1 << 2)
#define LINK_SW_RST                     (0x1 << 1)
#define PHY_SW_RST0                     (0x1 << 0)

/* OPHYCLK */
#define COMMON_ON_N1                    (0x1 << 7)
#define COMMON_ON_N0                    (0x1 << 4)
#define ID_PULLUP0                      (0x1 << 2)
#define CLK_SEL_24MHZ                   (0x3 << 0)
#define CLK_SEL_12MHZ                   (0x2 << 0)
#define CLK_SEL_48MHZ                   (0x0 << 0)

#define EXYNOS4X12_ID_PULLUP0		(0x01 << 3)
#define EXYNOS4X12_COMMON_ON_N0	(0x01 << 4)
#define EXYNOS4X12_CLK_SEL_12MHZ	(0x02 << 0)
#define EXYNOS4X12_CLK_SEL_24MHZ	(0x05 << 0)

/* Device Configuration Register DCFG */
#define DEV_SPEED_HIGH_SPEED_20         (0x0 << 0)
#define DEV_SPEED_FULL_SPEED_20         (0x1 << 0)
#define DEV_SPEED_LOW_SPEED_11          (0x2 << 0)
#define DEV_SPEED_FULL_SPEED_11         (0x3 << 0)
#define EP_MISS_CNT(x)                  (x << 18)
#define DEVICE_ADDRESS(x)               (x << 4)

/* Core Reset Register (GRSTCTL) */
#define TX_FIFO_FLUSH                   (0x1 << 5)
#define RX_FIFO_FLUSH                   (0x1 << 4)
#define TX_FIFO_NUMBER(x)               (x << 6)
#define TX_FIFO_FLUSH_ALL               TX_FIFO_NUMBER(0x10)

/* Masks definitions */
#define GINTMSK_INIT	(INT_OUT_EP | INT_IN_EP | INT_RESUME | INT_ENUMDONE\
			| INT_RESET | INT_SUSPEND)
#define DOEPMSK_INIT	(CTRL_OUT_EP_SETUP_PHASE_DONE | AHB_ERROR|TRANSFER_DONE)
#define DIEPMSK_INIT	(NON_ISO_IN_EP_TIMEOUT|AHB_ERROR|TRANSFER_DONE)
#define GAHBCFG_INIT	(PTXFE_HALF | NPTXFE_HALF | MODE_DMA | BURST_INCR16\
			| GBL_INT_UNMASK)

/* Device Endpoint X Transfer Size Register (DIEPTSIZX) */
#define DIEPT_SIZ_PKT_CNT(x)                      (x << 19)
#define DIEPT_SIZ_XFER_SIZE(x)                    (x << 0)

/* Device OUT Endpoint X Transfer Size Register (DOEPTSIZX) */
#define DOEPT_SIZ_PKT_CNT(x)                      (x << 19)
#define DOEPT_SIZ_XFER_SIZE(x)                    (x << 0)
#define DOEPT_SIZ_XFER_SIZE_MAX_EP0               (0x7F << 0)
#define DOEPT_SIZ_XFER_SIZE_MAX_EP                (0x7FFF << 0)

/* Device Endpoint-N Control Register (DIEPCTLn/DOEPCTLn) */
#define DIEPCTL_TX_FIFO_NUM(x)                    (x << 22)
#define DIEPCTL_TX_FIFO_NUM_MASK                  (~DIEPCTL_TX_FIFO_NUM(0xF))

/* Device ALL Endpoints Interrupt Register (DAINT) */
#define DAINT_IN_EP_INT(x)                        (x << 0)
#define DAINT_OUT_EP_INT(x)                       (x << 16)













/*-------------------------------------------------------------------------*/
#define EP0_FIFO_SIZE		64
#define EP_FIFO_SIZE		512
#define EP_FIFO_SIZE2		1024
#define DWC2_MAX_HW_ENDPOINTS	16
#define DWC2_MAX_ENDPOINTS	4

#define TEST_J_SEL		0x1
#define TEST_K_SEL		0x2
#define TEST_SE0_NAK_SEL	0x3
#define TEST_PACKET_SEL		0x4
#define TEST_FORCE_ENABLE_SEL	0x5

/* ************************************************************************* */
/* IO
 */

enum ep_type {
	ep_control, ep_bulk_in, ep_bulk_out, ep_interrupt
};

struct dwc2_ep {
	struct dwc2_udc *dev;

	unsigned int maxpacket;

	/* transfer */
	char *buf;
	unsigned length;
	unsigned actual;
	int done;

	uint8 stopped;
	uint8 bEndpointAddress;
	uint8 bmAttributes;

	enum ep_type ep_type;
	int fifo_num;
};

struct dwc2_udc {
	int ep0state;
	struct dwc2_ep ep[DWC2_MAX_ENDPOINTS];

	unsigned char usb_address;

	unsigned req_pending:1, req_std:1;

	int speed;
};

struct ep_status {
    unsigned int maxpacket;
    
    char *buf;
    unsigned int length;
    unsigned int actual;
    int done;
    
    uint8 bEndpointAddress;
    uint8 bmAttributes;
    int fifo_num;
};

#define ep_is_in(EP) (((EP)->bEndpointAddress&USB_DIR_IN) == USB_DIR_IN)
#define ep_index(EP) ((EP)->bEndpointAddress&0xF)
#define ep_maxpacket(EP) ((EP)->maxpacket)

//#define readl(addr) (*((volatile uint32 *)(addr)))
//#define writel(data, addr) \
//        do {*((volatile uint32 *)(addr)) = (data);} while(0)

#ifdef CONFIG_SYS_CACHELINE_SIZE
#define CACHELINE_SIZE	CONFIG_SYS_CACHELINE_SIZE
#else
#define CACHELINE_SIZE	32
#endif

#endif	/* __DWC2_UDC_OTG_PRIV__ */

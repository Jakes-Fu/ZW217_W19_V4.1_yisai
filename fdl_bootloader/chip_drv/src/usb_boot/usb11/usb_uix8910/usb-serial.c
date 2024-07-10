/*
 * (C) Copyright 2003
 * Gerry Hamel, geh@ti.com, Texas Instruments
 *
 * (C) Copyright 2006
 * Bryan O'Donoghue, bodonoghue@codehermit.ie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include "sci_types.h"
#include "unaligned.h"
#include "usbserial.h"
#include "dwc2_udc.h"
#include "usbser.h"
#include "usb_cdc_acm.h"
#include "usbdescriptors.h"
#include "udc.h"

//#define DEBUG

#ifdef DEBUG
#define DBG(fmt,args...)
#else
#define DBG(fmt,args...) do{}while(0)
#endif

#if 1
#define ERR(fmt,args...)
#else
#define ERR(fmt,args...) do{}while(0)
#endif

/*
 * Defines
 */
#define NUM_CONFIGS		1
#ifdef CONFIG_USB_ACM_TWO_CHANS
#define MAX_INTERFACES 4
#define NUM_ENDPOINTS  6
#define NUM_ACM_INTERFACES 4
#else
#define MAX_INTERFACES 2
#define NUM_ENDPOINTS  3
#define NUM_ACM_INTERFACES 2
#endif

/*
 * Buffers to hold input and output data
 */


/*
 * Instance variables
 */
static struct usb_device_instance device_instance[1];
static struct usb_bus_instance bus_instance[1];
static struct usb_configuration_instance config_instance[NUM_CONFIGS];
static struct usb_interface_instance interface_instance[MAX_INTERFACES];
static struct usb_alternate_instance alternate_instance[MAX_INTERFACES];
/* one extra for control endpoint */
static struct usb_endpoint_instance endpoint_instance[NUM_ENDPOINTS+1];

/*
 * Global flag
 */
int usbser_configured_flag = 0;
volatile int rx_buf_rindex = 0;
volatile int rx_buf_actual = 0;

#pragma arm section zidata = "ZIDATA_AREA_4"
volatile int rx_buf_rindex;
volatile int rx_buf_actual;
volatile char rx_buf[16*1024] __attribute__((aligned(4)));
#pragma arm section zidata

/* Indicies, References */
static unsigned short rx_endpoint[2] = {0, 0};
static unsigned short tx_endpoint[2] = {0, 0};

/* Standard USB Data Structures */
static struct usb_interface_descriptor interface_descriptors[MAX_INTERFACES];
static struct usb_endpoint_descriptor *ep_descriptor_ptrs[NUM_ENDPOINTS];
static struct usb_configuration_descriptor	*configuration_descriptor = 0;
static struct usb_device_descriptor device_descriptor = {
	sizeof(struct usb_device_descriptor), // .bLength = 
	USB_DT_DEVICE,	// .bDescriptorType =
	cpu_to_le16(USB_BCD_VERSION),	// .bcdUSB =
#ifdef CONFIG_USB_ACM_TWO_CHANS /* enable interface association */
	0xEF,	// .bDeviceClass =		
	0x02,	// .bDeviceSubClass =	
	0x01,	// .bDeviceProtocol =	
#else
	COMMUNICATIONS_DEVICE_CLASS,	// .bDeviceClass = 	s
	0x00,	// .bDeviceSubClass =	
	0x00,	// .bDeviceProtocol =	
#endif
	EP0_MAX_PACKET_SIZE,				// .bMaxPacketSize0 =	
	cpu_to_le16(CONFIG_USBD_VENDORID),	// .idVendor =		
#ifdef CONFIG_USB_ACM_TWO_CHANS
	cpu_to_le16(CONFIG_USBD_PRODUCTID_RDAACM),	// .idProduct =		
#else
	cpu_to_le16(CONFIG_USBD_PRODUCTID_CDCACM),	// .idProduct =		
#endif
	cpu_to_le16(USBTTY_BCD_DEVICE),				// .bcdDevice =		
	0,											// .iManufacturer =	
	0,											// .iProduct =		
	0,											// .iSerialNumber =	
	NUM_CONFIGS								// .bNumConfigurations =	
};


#if 1//defined(CONFIG_USBD_HS)
static struct usb_qualifier_descriptor qualifier_descriptor = {
	sizeof(struct usb_qualifier_descriptor),	// .bLength = 
	USB_DT_QUAL,			// .bDescriptorType =
	cpu_to_le16(USB_BCD_VERSION),	// .bcdUSB =		
	COMMUNICATIONS_DEVICE_CLASS,	// .bDeviceClass =		
	0x00,					// .bDeviceSubClass =	
	0x00,					// .bDeviceProtocol =	
	EP0_MAX_PACKET_SIZE,	// .bMaxPacketSize0 =	
	NUM_CONFIGS			// .bNumConfigurations =	
};
#endif

/*
 * Static CDC ACM specific descriptors
 */

struct acm_config_desc {
	struct usb_configuration_descriptor configuration_desc;

#ifdef CONFIG_USB_ACM_TWO_CHANS
	/* function 1 */
	struct usb_function_descriptor	function_desc;
#endif

	/* Master Interface */
	struct usb_interface_descriptor interface_desc;

	struct usb_class_header_function_descriptor usb_class_header;
	struct usb_class_call_management_descriptor usb_class_call_mgt;
	struct usb_class_abstract_control_descriptor usb_class_acm;
	struct usb_class_union_function_descriptor usb_class_union;
	struct usb_endpoint_descriptor notification_endpoint;

	/* Slave Interface */
	struct usb_interface_descriptor data_class_interface;
	struct usb_endpoint_descriptor data_endpoints[2];

#ifdef CONFIG_USB_ACM_TWO_CHANS
	/* function 2 */
	struct usb_function_descriptor	function_desc2;

	/* Master Interface 2 */
	struct usb_interface_descriptor interface_desc2;

	struct usb_class_header_function_descriptor usb_class_header2;
	struct usb_class_call_management_descriptor usb_class_call_mgt2;
	struct usb_class_abstract_control_descriptor usb_class_acm2;
	struct usb_class_union_function_descriptor usb_class_union2;
	struct usb_endpoint_descriptor notification_endpoint2;

	/* Slave Interface 2 */
	struct usb_interface_descriptor data_class_interface2;
	struct usb_endpoint_descriptor data_endpoints2[2];
#endif
} __attribute__((packed));

static struct acm_config_desc acm_configuration_descriptors[NUM_CONFIGS] = {
	{
		{ // .configuration_desc =
			sizeof(struct usb_configuration_descriptor),	// .bLength =
			USB_DT_CONFIG,	// .bDescriptorType = 
			cpu_to_le16(sizeof(struct acm_config_desc)),	// .wTotalLength =
			NUM_ACM_INTERFACES,	// .bNumInterfaces = 
			1,						// .bConfigurationValue = 
			0,						// .iConfiguration = 
			BMATTRIBUTE_SELF_POWERED|BMATTRIBUTE_RESERVED, // .bmAttributes =
			USBTTY_MAXPOWER		// .bMaxPower = 
		},
		/* Interface 1 */
		{ // .interface_desc = 
			sizeof(struct usb_interface_descriptor),	// .bLength  = 
			USB_DT_INTERFACE,					// .bDescriptorType = 
			0,									// .bInterfaceNumber = 
			0,									// .bAlternateSetting = 
			0x01,								// .bNumEndpoints = 
			COMMUNICATIONS_INTERFACE_CLASS_CONTROL,	// .bInterfaceClass =
			COMMUNICATIONS_ACM_SUBCLASS,		// .bInterfaceSubClass = 
			COMMUNICATIONS_V25TER_PROTOCOL,	// .bInterfaceProtocol = 
			0,									// .iInterface = 
		},
		{ // .usb_class_header = 
			sizeof(struct usb_class_header_function_descriptor), // .bFunctionLength	=
			CS_INTERFACE,								 // .bDescriptorType	= 
			USB_ST_HEADER,								 // .bDescriptorSubtype	= 
			cpu_to_le16(110),								 // .bcdCDC	= 
		},
		{ // .usb_class_call_mgt = 
			sizeof(struct usb_class_call_management_descriptor),	// .bFunctionLength	=
			CS_INTERFACE,									// .bDescriptorType	= 
			USB_ST_CMF,										// .bDescriptorSubtype = 
			0x00,											// .bmCapabilities = 
			0x01,											// .bDataInterface = 
		},
		{ // .usb_class_acm = 
			sizeof(struct usb_class_abstract_control_descriptor),	// .bFunctionLength	=
			CS_INTERFACE,									// .bDescriptorType	= 
			USB_ST_ACMF,									// .bDescriptorSubtype = 
			0x00,											// .bmCapabilities = 
		},
		{ // .usb_class_union = 
			sizeof(struct usb_class_union_function_descriptor),	// .bFunctionLength	=
			CS_INTERFACE,								// .bDescriptorType	= 
			USB_ST_UF,									// .bDescriptorSubtype = 
			0x00,										// .bMasterInterface	= 
			0x01,										// .bSlaveInterface0	= 
		},
		{ // .notification_endpoint = 
			sizeof(struct usb_endpoint_descriptor),			// .bLength =
			USB_DT_ENDPOINT,							// .bDescriptorType	= 
			UDC_INT_ENDPOINT | USB_DIR_IN,				// .bEndpointAddress = 
			USB_ENDPOINT_XFER_INT,						// .bmAttributes = 
			cpu_to_le16(CONFIG_USBD_SERIAL_INT_PKTSIZE),	// .wMaxPacketSize
			0xb,											// .bInterval = 
		},

		/* Interface 2 */
		{ // .data_class_interface = 
			sizeof(struct usb_interface_descriptor),	// .bLength =
			USB_DT_INTERFACE,					// .bDescriptorType	= 
			0x01,								// .bInterfaceNumber = 
			0x00,								// .bAlternateSetting = 
			0x02,								// .bNumEndpoints	= 
			COMMUNICATIONS_INTERFACE_CLASS_DATA,	// .bInterfaceClass =
			DATA_INTERFACE_SUBCLASS_NONE,		// .bInterfaceSubClass = 
			DATA_INTERFACE_PROTOCOL_NONE,		// .bInterfaceProtocol = 
			0,									// .iInterface = 
		},
		{ // .data_endpoints = 
			{
				sizeof(struct usb_endpoint_descriptor),	// .bLength =
				USB_DT_ENDPOINT,					// .bDescriptorType	= 
				UDC_OUT_ENDPOINT | USB_DIR_OUT,		// .bEndpointAddress = 
				USB_ENDPOINT_XFER_BULK,				// .bmAttributes =
				cpu_to_le16(CONFIG_USBD_SERIAL_BULK_PKTSIZE),	// .wMaxPacketSize	=
				0x80,	// .bInterval = 
			},
			{
				sizeof(struct usb_endpoint_descriptor),	// .bLength =
				USB_DT_ENDPOINT,					// .bDescriptorType	= 
				UDC_IN_ENDPOINT | USB_DIR_IN,		// .bEndpointAddress = 
				USB_ENDPOINT_XFER_BULK,				// .bmAttributes =
				cpu_to_le16(CONFIG_USBD_SERIAL_BULK_PKTSIZE),	// .wMaxPacketSize	=
				0x80,								// .bInterval = 
			},
		},
	},
};

static const struct rs232_emu rs232_desc={
	115200,	// .dter		=	
	0x00,	// .stop_bits	=	
	0x00,	// .parity		=	
	0x08	// .data_bits	=	
};

/*
 * Static Function Prototypes
 */

static void usbser_init_instances (void);
static void usbser_init_endpoints (void);
static void usbser_init_terminal_type(void);
static void usbser_event_handler (struct usb_device_instance *device,
				usb_device_event_t event, int data);
static int usbser_cdc_setup(struct usb_device_request *request,
				struct urb *urb);


/*
 * Test whether a character is in the RX buffer
 */

int usbser_tstc (int chan)
{
	udc_irq();

	return 0;
}

/*
 * Read a single byte from the usb client port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int usbser_getc (int chan)
{
    unsigned char c = -1;
    
    if (rx_buf_rindex != rx_buf_actual)
    {
        //rprintf("r %d, a %d\n", rx_buf_rindex, rx_buf_actual);
        c = rx_buf[rx_buf_rindex++];
        if (rx_buf_rindex == rx_buf_actual)
        {
            rx_buf_rindex = 0;
            rx_buf_actual = 0;
        }
        return c;
    }
    else if ((rx_buf_rindex == rx_buf_actual) && (rx_buf_rindex == 0))
    {
        //rprintf("r %d, a %d\n", rx_buf_rindex, rx_buf_actual);
        if (chan == 0)
            usbser_read(chan, rx_buf, sizeof(rx_buf));
        else
            udc_endpoint_raw_read(rx_buf, sizeof(rx_buf));
        if (rx_buf_actual > 0) 
        {
            c = rx_buf[rx_buf_rindex++];
            if (rx_buf_rindex == rx_buf_actual)
            {
                rx_buf_rindex = 0;
                rx_buf_actual = 0;
            }
            //rprintf("usbser_getc: 0x%x, actual %d\n", c, rx_buf_actual);
            return c;
        } 
        else
        {
            return -1;
        }
    }
    else
    {
        rprintf("usbser_getc: err rindex %d, actual %d\n", rx_buf_rindex, rx_buf_actual);
        return 0;
    }
}

/*
 * Output a single byte to the usb client port.
 */
void usbser_putc (int chan, const char c)
{
	usbser_write(chan, (uint8 *)&c, 1);
}


int usbser_read(int chan, unsigned char *buf, unsigned len)
{
	int ret, i;
	struct usb_endpoint_instance *ep_out = NULL;
	struct urb *urb = NULL;
	unsigned short ep = rx_endpoint[chan];

	for(i = 1; i <= NUM_ENDPOINTS; i++) {
		if(endpoint_instance[i].endpoint_address == (ep | USB_DIR_OUT))
			ep_out = &endpoint_instance[i];
	}
	if(!ep_out)
		return -1;

	if (!usbser_configured())
		return 0;

	urb = ep_out->rcv_urb;
	usbd_setup_urb(urb, buf, len);

	ret = udc_endpoint_read(ep_out, 1);
	usbd_free_urb(urb);
	if (!ret)
		return len;
	rprintf("usber read error %d\n", ret);
	return -1;
}

int usbser_write(int chan, const unsigned char *_buf, unsigned int len)
{
	int ret, i;
	struct usb_endpoint_instance *ep_in = NULL;
	struct urb *urb = NULL;
	unsigned char *buf = (unsigned char *)_buf;
	unsigned short ep = tx_endpoint[chan];

	for (i = 1; i <= NUM_ENDPOINTS; i++) {
		if (endpoint_instance[i].endpoint_address == (ep | USB_DIR_IN))
			ep_in = &endpoint_instance[i];
	}
	if (!ep_in)
		return -1;

	if (!usbser_configured()) {
		return 0;
	}

	urb = ep_in->tx_urb;
	usbd_setup_urb(urb, buf, len);

	ret = udc_endpoint_write(ep_in);
	usbd_free_urb(urb);
	if (!ret)
		return len;

	rprintf("usber write error %d\n", ret);
	return -1;
}
/*
 * Initialize the usb client port.
 *
 */

int drv_usbser_init (void)
{
	int ret;

	/* Decide on which type of UDC device to be.
	 */
	usbser_init_terminal_type();

	/* Now, set up USB controller and infrastructure */
	ret = udc_init ();		/* Basic USB initialization */
	if (ret)
		return ret;

	usbser_init_instances ();

	usbser_init_endpoints ();

	udc_startup_events (device_instance);/* Enable dev, init udc pointers */
	udc_connect ();		/* Enable pullup for host detection */

	return 0;
}


#define init_wMaxPacketSize(x)	\
	get_unaligned(&ep_descriptor_ptrs[(x) - 1]->wMaxPacketSize);

#define init_write_wMaxPacketSize(val, x)	 (put_unaligned(le16_to_cpu(val),\
			&ep_descriptor_ptrs[(x) - 1]->wMaxPacketSize));

static struct urb urb_pool[NUM_ENDPOINTS];
static uint8 int_ep_buffer[URB_BUF_SIZE] __attribute__((aligned(4)));

static int endpoint_is_int(struct usb_endpoint_instance *instance)
{
	if (!instance)
		return 0;

	if (instance->tx_attributes == USB_ENDPOINT_XFER_INT)
		return 1;
	else
		return 0;
}
static void usbser_init_instances (void)
{
	int i;

	/* initialize device instance */
	memset (device_instance, 0, sizeof (struct usb_device_instance));
	//device_instance->device_state = STATE_INIT;
	device_instance->device_state = STATE_CONFIGURED;
	device_instance->device_descriptor = &device_descriptor;
#if 1 //defined(CONFIG_USBD_HS)
	device_instance->qualifier_descriptor = &qualifier_descriptor;
#endif
	device_instance->event = usbser_event_handler;
	device_instance->cdc_recv_setup = usbser_cdc_setup;
	device_instance->bus = bus_instance;
	device_instance->configurations = NUM_CONFIGS;
	device_instance->configuration_instance_array = config_instance;

	/* initialize bus instance */
	memset (bus_instance, 0, sizeof (struct usb_bus_instance));
	bus_instance->device = device_instance;
	bus_instance->endpoint_array = endpoint_instance;
	bus_instance->max_endpoints = 1;
	bus_instance->maxpacketsize = 64;

	/* configuration instance */
	memset (config_instance, 0,
		sizeof (struct usb_configuration_instance));
	config_instance->interfaces = NUM_ACM_INTERFACES;
	config_instance->configuration_descriptor = configuration_descriptor;
	config_instance->interface_instance_array = interface_instance;

	/* interface instance */
	memset (interface_instance, 0,
		sizeof (struct usb_interface_instance));
	interface_instance->alternates = 1;
	interface_instance->alternates_instance_array = alternate_instance;

	/* alternates instance */
	memset (alternate_instance, 0,
		sizeof (struct usb_alternate_instance));
	alternate_instance->interface_descriptor = interface_descriptors;
	alternate_instance->endpoints = NUM_ENDPOINTS;
	alternate_instance->endpoints_descriptor_array = ep_descriptor_ptrs;

	/* endpoint instances */
	memset (&endpoint_instance[0], 0,
		sizeof (struct usb_endpoint_instance));
	endpoint_instance[0].endpoint_address = 0;
	endpoint_instance[0].rcv_packetSize = EP0_MAX_PACKET_SIZE;
	endpoint_instance[0].rcv_attributes = USB_ENDPOINT_XFER_CONTROL;
	endpoint_instance[0].tx_packetSize = EP0_MAX_PACKET_SIZE;
	endpoint_instance[0].tx_attributes = USB_ENDPOINT_XFER_CONTROL;
	udc_setup_ep (device_instance, 0, &endpoint_instance[0]);

	for (i = 1; i <= NUM_ENDPOINTS; i++) {
		memset (&endpoint_instance[i], 0,
			sizeof (struct usb_endpoint_instance));

		endpoint_instance[i].endpoint_address =
			ep_descriptor_ptrs[i - 1]->bEndpointAddress;

		endpoint_instance[i].rcv_attributes =
			ep_descriptor_ptrs[i - 1]->bmAttributes;

		endpoint_instance[i].rcv_packetSize = init_wMaxPacketSize(i);

		endpoint_instance[i].tx_attributes =
			ep_descriptor_ptrs[i - 1]->bmAttributes;

		endpoint_instance[i].tx_packetSize = init_wMaxPacketSize(i);


		urb_link_init (&endpoint_instance[i].rcv);
		urb_link_init (&endpoint_instance[i].rdy);
		urb_link_init (&endpoint_instance[i].tx);
		urb_link_init (&endpoint_instance[i].done);

		if (endpoint_instance[i].endpoint_address & USB_DIR_IN) {
			endpoint_instance[i].tx_urb = &urb_pool[i - 1];
			/* function only has one interrupt endpoint */
			if (endpoint_is_int(&endpoint_instance[i]))
				usbd_init_urb (endpoint_instance[i].tx_urb, device_instance,
					&endpoint_instance[i], int_ep_buffer, URB_BUF_SIZE);
			else // bulk in&out re-use buffer with application driver
				usbd_init_urb (endpoint_instance[i].tx_urb, device_instance,
					&endpoint_instance[i], NULL, 0);
		} else {
			endpoint_instance[i].rcv_urb = &urb_pool[i - 1];
			usbd_init_urb (endpoint_instance[i].rcv_urb, device_instance,
					&endpoint_instance[i], NULL, 0);
		}
	}
}

static void usbser_init_endpoints (void)
{
	int i;

	bus_instance->max_endpoints = NUM_ENDPOINTS + 1;

	for (i = 0; i < NUM_ENDPOINTS; i++) {
		if (((ep_descriptor_ptrs[i]->bmAttributes &
			USB_ENDPOINT_XFERTYPE_MASK) ==
			USB_ENDPOINT_XFER_BULK)
			&& is_usbd_high_speed()) {

			init_write_wMaxPacketSize(CONFIG_USBD_SERIAL_BULK_HS_PKTSIZE, i+1);
		}

		endpoint_instance[i + 1].tx_packetSize =  init_wMaxPacketSize(i + 1);
		endpoint_instance[i + 1].rcv_packetSize =  init_wMaxPacketSize(i + 1);

		udc_setup_ep (device_instance, i + 1, &endpoint_instance[i+1]);
	}
}

/* usbser_init_terminal_type
 *
 * Do some late binding for our device type.
 */
static void usbser_init_terminal_type(void)
{
	/* CDC ACM */
	/* Assign endpoint descriptors */
	ep_descriptor_ptrs[0] =
		&acm_configuration_descriptors[0].notification_endpoint;
	ep_descriptor_ptrs[1] =
		&acm_configuration_descriptors[0].data_endpoints[0];
	ep_descriptor_ptrs[2] =
		&acm_configuration_descriptors[0].data_endpoints[1];
#ifdef CONFIG_USB_ACM_TWO_CHANS
	ep_descriptor_ptrs[3] =
		&acm_configuration_descriptors[0].notification_endpoint2;
	ep_descriptor_ptrs[4] =
		&acm_configuration_descriptors[0].data_endpoints2[0];
	ep_descriptor_ptrs[5] =
		&acm_configuration_descriptors[0].data_endpoints2[1];
#endif

#if 1 // defined(CONFIG_USBD_HS)
	qualifier_descriptor.bDeviceClass =
		COMMUNICATIONS_DEVICE_CLASS;
#endif
	/* Assign endpoint indices */
	tx_endpoint[0] = UDC_IN_ENDPOINT;
	rx_endpoint[0] = UDC_OUT_ENDPOINT;
#ifdef CONFIG_USB_ACM_TWO_CHANS
	tx_endpoint[1] = UDC_IN_ENDPOINT2;
	rx_endpoint[1] = UDC_OUT_ENDPOINT2;
#endif

	/* Configuration Descriptor */
	configuration_descriptor =
		(struct usb_configuration_descriptor*)
		&acm_configuration_descriptors;

}

int usbser_configured (void)
{
	udc_irq();
	return usbser_configured_flag;
}

/******************************************************************************/

static void usbser_event_handler (struct usb_device_instance *device,
				  usb_device_event_t event, int data)
{
	switch (event) {
	case DEVICE_RESET:
	case DEVICE_BUS_INACTIVE:
		usbser_configured_flag = 0;
		break;
	case DEVICE_CONFIGURED:
		usbser_configured_flag = 1;
		break;

	case DEVICE_ADDRESS_ASSIGNED:
		usbser_init_endpoints ();

	default:
		break;
	}
}

/******************************************************************************/

int usbser_cdc_setup(struct usb_device_request *request, struct urb *urb)
{
	switch (request->bRequest){

		case ACM_SET_CONTROL_LINE_STATE:	/* Implies DTE ready */
			break;
		case ACM_SEND_ENCAPSULATED_COMMAND :	/* Required */
			break;
		case ACM_SET_LINE_ENCODING :		/* DTE stop/parity bits
							 * per character */
			break;
		case ACM_GET_ENCAPSULATED_RESPONSE :	/* request response */
			break;
		case ACM_GET_LINE_ENCODING :		/* request DTE rate,
							 * stop/parity bits */
			memcpy (urb->buffer , &rs232_desc, sizeof(rs232_desc));
			urb->actual_length = sizeof(rs232_desc);

			break;
		default:
			return 1;
	}
	return 0;
}

void dump_rx_buf(void)
{
    int i;
    
	rprintf("Recv:\n");
    for (i = 0; i < rx_buf_actual; i++)
    {
        rprintf("%02x ", rx_buf[i]);
        if (((i %16) == 0) && (i != 0))
            rprintf("\n");
    }
	rprintf("\n");
}
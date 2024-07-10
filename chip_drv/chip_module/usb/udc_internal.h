#ifndef _USB__UDC_INTERNAL_H_
#define _USB__UDC_INTERNAL_H_

enum udc_ep0_state {
    EP0_STATE_INVALID = 0xf0,
#define EP0_STATE_INVALID_MASK 0xf0
    EP0_SETUP = 0,
#define EP0_STATE_NOT_SETUP_MASK 0x4 // 0b100
#define EP0_STATE_DIRIN_MASK 0x2 // 0b10
#define EP0_STATE_DATA_MASK 0x1 // 0b1
    EP0_DATA_IN = 0x7,  // 0b111
    EP0_DATA_OUT = 0x5, // 0b101
    EP0_STATUS_IN = 0x6, // 0b110
    EP0_STATUS_OUT = 0x4, // 0b100
};

#define REG_R(addr) (*(volatile uint32 *)(addr))
#define REG_W(addr, val) (*(volatile uint32 *)(addr) = (val))
#define REG_OR(addr, val) (*(volatile uint32 *)(addr) |= (val))
#define REG_AND(addr, val) (*(volatile uint32 *)(addr) &= (val))

static void REG_WAIT_SET(uint32 addr, uint32 bits, uint32 timeout)
{
    uint32 i;
    for (i = 0; i < timeout; ++i) {
        if ((REG_R(addr) & bits) == bits)
            break;
    }
}

/**
 * @brief control buffer size
 */
unsigned udc_ctrl_buf_size_();

/**
 * @brief control buffer
 */
void *udc_ctrl_buffer_();

/**
 * @brief reset the hardware usb device controller
 */
void udc_hw_reset_();

/**
 * @breif enable hw usb endpoint
 *
 * @param ep    the ep address
 * @param type  the ep transfer type
 * @param mps   max packet size
 */
void udc_hw_ep_enable_(uint8 ep, int type, uint16 mps);

/**
 * @brief set usb address to controller
 *
 * @param addr  the address
 */
void udc_hw_set_address_(uint8 addr);

/**
 * @brief start transfer for specific endpoint
 *
 * @param ep_addr   the endpoint address
 * @param mps       the endpoint max packet size
 * @param buffer    the transfer buffer
 * @param size      transfer size
 */
void udc_hw_start_xfer_(uint8 ep_addr, uint16 mps, void *buffer, uint32 size);

/**
 * @brief stop transfer for specific endpoint
 *
 * @param ep_addr   the endponit address
 */
void udc_hw_stop_xfer_(uint8 ep_addr);

#endif

/******************************************************************************
 ** File Name:    usb_boot.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         4/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
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
#include "common.h"
//#include "fdl_crc.h"
#include "usb_boot.h"
#include "drv_usb.h"
#include "virtual_com.h"
#include "fdl_main.h"
#include "fdl_channel.h"

#ifndef PLATFORM_UWS6121E
#ifdef PLATFORM_SC6530

#ifdef CHIP_VER_6531
#include "int_reg_r2p0.h"
#include "usb_reg_r3p0.h"
#else
#include "int_reg_v5.h"
#include "usb_reg_v5.h"
#endif

#elif defined(PLATFORM_SC6531EFM)

#include "int_reg_r2p0.h"
#include "usb_reg_r3p0.h"

#else
#include "int_reg_v0.h"
#include "usb_reg_v0.h"
#endif
#endif

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
#define USB_BUFF_SIZE  0x4000
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

#ifndef PLATFORM_UWS6121E
/*--------------------------- Local Data ------------------------------------*/
__align (4) LOCAL uint8 s_usb_snd_buff[64];
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
extern USB_rx_buf_T     buf_manager ;
extern uint32  free_buffer_length;
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL void usb_init (uint32 ext_clk26M);
#endif
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
void usb_boot (uint32 ext_clk26M)
{
#if defined(PLATFORM_UWS6121E)
    drv_usbser_init();
#else
    buf_manager.read    = 0 ;
    buf_manager.write   = 0 ;
    usb_init (ext_clk26M);
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
void usb_varinit (void)
{
#if !defined(PLATFORM_UWS6121E)
    //int i;

    buf_manager.read    = 0 ;
    buf_manager.write   = 0 ;
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
void usb_init (uint32 ext_clk26M)
{
#if !defined(PLATFORM_UWS6121E)
    //int i;
    USB_DEV_REG_T        *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

#if defined (PLATFORM_SC6800H)
    * (volatile uint32 *) (0x20900000) |= BIT_2;
#endif

    //Enable USB device ;
    ctl_usb->ctrl.mBits.en  = 1;

    USB_EP0Config ();
    USB_EP1Config ();
    USB_EP2Config ();
    USB_EP3Config ();
    *(volatile uint32 *) USB_TIME_SET = 0x0f;  //mdl auto download issue	
    free_buffer_length = USB_RECV_LIMIT;
    //Open USB_LDO and pullup 1.5K resister;
    //handled in entry of init.s
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
void usb_write (unsigned char *write_buf, unsigned int write_len)
{
#if !defined(PLATFORM_UWS6121E)
    //unsigned short crc;
    unsigned int i;
    unsigned int packet_count;
    unsigned int split_flag=0;  //whether need to split the last 64 byte packet.
    unsigned int send_length = 0;

    packet_count = (write_len >> 6);

    if ( (write_len & 0x3f) ==0)
    {
        split_flag = 1;
    }
    else
    {
        split_flag = 0;
    }

    //Ep3 max pack size is 64 bytes .
    if (packet_count > 0)
    {

        for (i=0; i< (packet_count-split_flag); i++)
        {
            USB_EPxSendData (USB_EP3 , (unsigned int *) (write_buf + send_length),64);
            send_length += 64;
        }

        if (split_flag) //because of no empty packet, here split the last 64-Byte packet into 2 32-byte packets.
        {
            USB_EPxSendData (USB_EP3 , (unsigned int *) (write_buf + send_length), 32);
            send_length += 32;
            USB_EPxSendData (USB_EP3 , (unsigned int *) (write_buf + send_length),32);
        }
        else
        {
            USB_EPxSendData (USB_EP3 , (unsigned int *) (write_buf + send_length), write_len-send_length);
        }
    }
    else
    {
        USB_EPxSendData (USB_EP3 , (unsigned int *) write_buf,write_len);
    }
#endif
}

/*****************************************************************************/
//  Description:    turn off usb ldo
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
uint8 usb_ldo_off (void)
{
#if !defined(PLATFORM_UWS6121E)
    * (volatile unsigned long *) 0x8b00003C |= 1<<13;
    * (volatile unsigned long *) 0x8b00003C &= ~ (1<<12);

    return TRUE;
#else
    return TRUE;
#endif
}
//Add for USB
LOCAL void usb_irq (void)
{
#if !defined(PLATFORM_UWS6121E)

    USB_DEV_REG_T *ctl = (USB_DEV_REG_T *) USB_REG_BASE;
    USB_DEV_INT_STS_U status ;

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    if (REG32(INT1_IRQ_RAW_STS) & (BIT_5))
#else
    if (REG32(INT_IRQ_RAW_STS) & (1<<TB_USB_INT))
#endif
    {
        status.dwValue = ctl->int_sts.dwValue ;

        //dispatch handler message according to usb int status ;
        if (status.dwValue  & USB_INT_EPx_MASK)
        {
            if (status.mBits.ep2)
            {
                USB_Ep2_handler();
            }

            if (status.mBits.ep3)
            {
                USB_Ep3_handler();
            }

            if (status.mBits.ep0)
            {
                USB_Ep0_handler();
            }

            if (status.mBits.ep1)
            {
                USB_Ep1_handler();
            }
        }

        //clear usb interrupt ;
        ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
    }
#endif
}

//extern char VCOM_GetChar (void);
extern int VCOM_GetSingleChar (void);
static int FDL_UsbOpen (struct FDL_ChannelHandler  *channel, unsigned int baudrate)
{
    return 0;
}

static int FDL_UsbRead (struct FDL_ChannelHandler  *channel, 
        const unsigned char *buf, unsigned int len)
{
    return -1;
}

static char FDL_UsbGetChar (struct FDL_ChannelHandler  *channel)
{
#if defined(PLATFORM_UWS6121E)
    //rprintf("%s: channel 0x%x\n", __func__, channel);
    if (!channel)
        return usbser_getc(1);
    else
        return usbser_getc(0); /* read ch from chan 0 */
#else
	USB_DOWNLOAD_STATUS_E download_status = USB_DOWNLOAD_OK;
	return VCOM_GetChar(&download_status);
#endif
}

static int FDL_UsbGetSingleChar (struct FDL_ChannelHandler  *channel)
{
#if defined(PLATFORM_UWS6121E)
    //rprintf("%s: channel 0x%x\n", __func__, channel);
    if (!channel)
        return usbser_getc(1);
    else
        return usbser_getc(0); /* read ch from chan 0 */
#else
	return VCOM_GetSingleChar();
#endif
}

static int FDL_UsbWrite (struct FDL_ChannelHandler  *channel, 
        const unsigned char *buf, unsigned int len)
{
#if defined(PLATFORM_UWS6121E)
	udc_endpoint_raw_wirte(buf, len);
#else
	usb_write ( (unsigned char *) buf, len);
#endif
    
    return 0;
}

static int FDL_UsbPutChar (struct FDL_ChannelHandler  *channel, const unsigned char ch)
{
#if defined(PLATFORM_UWS6121E)
    usbser_putc(0, ch);
#else
	s_usb_snd_buff[0] = ch;
    USB_EPxSendData (USB_EP3 , (unsigned int *) &s_usb_snd_buff, 1);
#endif

    return 0;
}

static int FDL_UsbSetBaudrate (struct FDL_ChannelHandler  *channel,  unsigned int baudrate)
{
    return 0;
}
static int FDL_UsbClose (struct FDL_ChannelHandler  *channel)
{
    return 0;
}
struct FDL_ChannelHandler gUSBChannel =
{
    FDL_UsbOpen,
    FDL_UsbRead,
    FDL_UsbGetChar,
    FDL_UsbGetSingleChar,
    FDL_UsbWrite,
    FDL_UsbPutChar,
    FDL_UsbSetBaudrate,
    FDL_UsbClose,
    0
};
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

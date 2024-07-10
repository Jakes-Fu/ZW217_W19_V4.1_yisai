/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

//#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('U', 'P', 'D', 'L')
// #define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

//#include "boot_platform.h"
//#include "osi_fifo.h"
//#include "drv_names.h"
//#include "hal_config.h"
//#include "osi_log.h"
//#include "osi_api.h"
#include "hal_rom_api.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "efuse_drv.h"
#include "fdl_main.h"
#include "fdl_channel.h"
#include "global_macros.h"
#include "hal_efuse_def.h"

//#include "hwregs.h"
typedef volatile struct
{
    uint32_t analog_usb20_usb20_test_pin;       // 0x00000000
    uint32_t analog_usb20_usb20_utmi_ctl1;      // 0x00000004
    uint32_t analog_usb20_usb20_batter_pll;     // 0x00000008
    uint32_t analog_usb20_usb20_utmi_ctl2;      // 0x0000000c
    uint32_t analog_usb20_usb20_trimming;       // 0x00000010
    uint32_t analog_usb20_reg_sel_cfg_0;        // 0x00000014
    uint32_t __24[250];                         // 0x00000018
    uint32_t analog_usb20_usb20_test_pin_set;   // 0x00000400
    uint32_t analog_usb20_usb20_utmi_ctl1_set;  // 0x00000404
    uint32_t analog_usb20_usb20_batter_pll_set; // 0x00000408
    uint32_t analog_usb20_usb20_utmi_ctl2_set;  // 0x0000040c
    uint32_t __1040[1];                         // 0x00000410
    uint32_t analog_usb20_reg_sel_cfg_0_set;    // 0x00000414
    uint32_t __1048[250];                       // 0x00000418
    uint32_t analog_usb20_usb20_test_pin_clr;   // 0x00000800
    uint32_t analog_usb20_usb20_utmi_ctl1_clr;  // 0x00000804
    uint32_t analog_usb20_usb20_batter_pll_clr; // 0x00000808
    uint32_t analog_usb20_usb20_utmi_ctl2_clr;  // 0x0000080c
    uint32_t __2064[1];                         // 0x00000810
    uint32_t analog_usb20_reg_sel_cfg_0_clr;    // 0x00000814
} HWP_ANALOG_G2_T;
#define REG_ANALOG_G2_BASE (0x5150f000)
#define hwp_analogG2 ((HWP_ANALOG_G2_T *)REG_ACCESS_ADDRESS(REG_ANALOG_G2_BASE))
#define ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(n) (((n)&0x7) << 11)
#define ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(n) (((n)&0x1f) << 16)
#define ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(n) (((n)&0x3f) << 21)
//#define HAL_EFUSE_DOUBLE_BLOCK_USB 90

typedef struct
{
    struct FDL_ChannelHandler ops;
    void *userial;
} fdlUsbChannel_t;

struct FDL_ChannelHandler gUSBChannel;
fdlUsbChannel_t g_UserialCh;
//uint8 g_userial_Inited;
static void fdlUsbParamCalibration(void)
{
    uint32_t usb_cfg;
    uint8_t tuneotg = 0;
    uint8_t tfregres = 0;
    uint8_t tfhsres = 0;
    bool low = halEfuseDoubleRead(HAL_EFUSE_DOUBLE_BLOCK_USB, &usb_cfg);
	rprintf("userial fdlUsbParamCalibration:usb_cfg = 0x%x,low=%d\n",usb_cfg,low);

    if (low && (usb_cfg & 0x1) == 1)
    {
        tfhsres = usb_cfg >> 4;
        tfregres = usb_cfg >> 9;
        tuneotg = usb_cfg >> 1;
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(0x1f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(0x3f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(0x7);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(tfhsres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(tfregres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(tuneotg);
    }

}

static int _read(struct FDL_ChannelHandler *ch, const unsigned char *data, unsigned int size)
{
	struct FDL_ChannelHandler *d;
    uint8_t *data8 = (uint8_t *)data;
    size_t bytes = 0;
	if(ch != NULL)
	{
    	d = ch;
	}
	else
		d = &gUSBChannel;


    while (bytes < size)
    {
        int c = halRomUserialGetChar(d->priv);
        if (c < 0)
            break;

        bytes++;
        *data8++ = c;
    }
	 //rprintf("userial _read:%d\n",bytes);
    return bytes;
}
static int _getChar(struct FDL_ChannelHandler *ch)
{
	struct FDL_ChannelHandler *d;
	int c;

	if(ch != NULL)
	{
    	d = ch;
	}
	else
		d = &gUSBChannel;


	c = halRomUserialGetChar(d->priv);

	//rprintf("userial _getChar:0x%x\n",c);

    return c;
}

static int _putChar(struct FDL_ChannelHandler *ch,const unsigned char c)
{
	struct FDL_ChannelHandler *d;
	int status;
	if(ch != NULL)
	{
    	d = ch;
	}
	else
		d = &gUSBChannel;

	status = halRomUserialPutChar(d->priv,c);

	//rprintf("userial _putChar:0x%x,status:%d\n",c,status);
    return status;
}
static int _write(struct FDL_ChannelHandler *ch, const unsigned char *data, unsigned int size)
{
	struct FDL_ChannelHandler *d;
    const uint8_t *data8 = (const uint8_t *)data;
    size_t bytes = 0;

	if(ch != NULL)
	{
    	d = ch;
	}
	else
		d = &gUSBChannel;


    while (bytes < size)
    {
        if (halRomUserialPutChar(d->priv, *data8) < 0)
            break;

        bytes++;
        data8++;
    }

	//rprintf("userial _write:%d\n",bytes);
    return bytes;
}

static void _flush(struct FDL_ChannelHandler *ch)
{
	struct FDL_ChannelHandler *d;

	if(ch != NULL)
	{
    	d = ch;
	}
	else
		d = &gUSBChannel;

    halRomUserialFlush(d->priv);
}

static void _destroy(struct FDL_ChannelHandler *ch)
{
    _flush(ch);
    //free(ch);
}

//static void _flushInput(struct FDL_ChannelHandler *ch) {}
static bool _connected(struct FDL_ChannelHandler *ch) { return true; }
static bool _setBaud(struct FDL_ChannelHandler *ch, uint32_t baud) { return true; }

struct FDL_ChannelHandler gUSBChannel =
{
    _connected,
    _read,
    _getChar,
    _getChar,
    _write,
    _putChar,
    _setBaud,
    _destroy,
    0
};

void usb_boot (unsigned long ext_clk26M)
{
	//fdlUsbChannel_t *ch = (fdlUsbChannel_t *)malloc(sizeof(fdlUsbChannel_t));

	rprintf("userial usb_boot\n");
	fdlUsbParamCalibration();
	gUSBChannel.priv = halRomGetUserial();

	halRomUserialOpen(gUSBChannel.priv);
	//g_userial_Inited = 55;
	rprintf("userial usb_boot done!\n");
}

PUBLIC struct FDL_ChannelHandler *ResetDumpUsbChannelGet(void)
{
   return &gUSBChannel;
}

#if 0
FDL_ChannelHandler *fdlOpenUsbSerial()
{
	fdlUsbChannel_t *ch = (fdlUsbChannel_t *)malloc(sizeof(fdlUsbChannel_t));
	if (ch == NULL)
	    return NULL;
	//fdlUsbParamCalibration();
	halRomApiInit();
	ch->userial = halRomGetUserial();
	halRomUserialOpen(ch->userial);
	ch->ops.Open= _connected;
	ch->ops.Read = _read;
	ch->ops.GetChar = _getChar;
	ch->ops.GetSingleChar = _getChar;
	ch->ops.Write = _write;
	ch->ops.GetChar = _putChar;
	ch->ops.SetBaudrate = _setBaud;
	ch->ops.flush_input = _flushInput;
	ch->ops.flush = _flush;
	ch->ops.close = _destroy;
	return &ch->ops;
}
#endif

#include "fdl_channel.h"
#include "sci_types.h"
#include "sio_drv.h"
#include "usb_boot.h"

#define BOOT_FLAG_USB                   (0x5A)
#define BOOT_FLAG_UART1                 (0x6A)
#define BOOT_FLAG_UART0                 (0x7A)
#define BOOT_FLAG_AUTODOWNLOAD          (0x8A)

extern uint32 FDL_GetBootMode (void);
extern struct FDL_ChannelHandler gUart1_DownloadChannel;
extern void rprintf(const char *fmt, ...);
//#define FDL_UART0_CHANNEL_USE

/******************************************************************************/
//  Description:    find a useable channel
//  Global resource dependence:
//  Author:         junqiang.wang
//  Note:
/******************************************************************************/


PUBLIC struct FDL_ChannelHandler *FDL_ChannelGet(void)
{
    uint32 bootMode = 0;
    //char ch;

    struct FDL_ChannelHandler *channel;
    bootMode = FDL_GetBootMode();

    switch (bootMode)
    {
#if !defined(PLATFORM_UWS6121E) // for fdl1 size
        case BOOT_FLAG_UART1:
            channel = &gUart1Channel;
            break;
        case BOOT_FLAG_UART0:
            channel = &gUart0Channel;
            break;
#endif
        case BOOT_FLAG_USB:
            channel = &gUSBChannel;
            break;
        case BOOT_FLAG_AUTODOWNLOAD:
            channel = &gUSBChannel;
			//channel = &gUart1_DownloadChannel;
			rprintf("BOOT_FLAG_AUTODOWNLOAD\n");
            break;
        default:
            channel = &gUSBChannel;
            break;
    }

    return channel;
}

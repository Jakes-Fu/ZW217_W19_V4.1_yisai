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
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "usb_drv.h"
#include "usb20_drvapi.h"
#include "vcom_usb.h"
#include "usb_ch9.h"
#include "ref_outport.h"
#include "chip_plf_export.h"
#include "usb20_app_config.h"
#include "mmu_drvapi.h"
#include "auto_download.h"
#include "osi_fifo.h"

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
#define DATA_OUT    1
#define DATA_IN     0

#ifdef USB_RECV_LIMIT
#undef USB_RECV_LIMIT
#endif

#define USB_RECV_LIMIT  (0x1000)
#define VCOM_SEND_MAX_TICK 16384

#define VCOM_TX_DMA_SIZE 512
#define VCOM_RX_DMA_SIZE 512
#define VCOM_TX_BUF_SIZE (1024 * 2)
#define VCOM_RX_BUF_SIZE (1024 * 2)

#define VCOM_BUF_SIZE (VCOM_TX_DMA_SIZE + VCOM_RX_DMA_SIZE + VCOM_TX_BUF_SIZE + VCOM_RX_BUF_SIZE)

static __align(32) uint32 _vcom_buf[VCOM_BUF_SIZE / 4];
static __align(32) uint32 _vcom1_buf[VCOM_BUF_SIZE / 4];

enum {
    VCOM_TX_IDLE,
    VCOM_TX_RUNNING,
    VCOM_TX_DONE,
    VCOM_TX_FAIL,
};

typedef struct _vcom_device
{
    uint32  state;
    uint8 tx_ep;
    uint8 rx_ep;
    void *tx_dma_buf;
    void *rx_dma_buf;
    uint32 tx_dma_size;
    uint32 rx_dma_size;
    osiFifo_t tx_fifo;
    osiFifo_t rx_fifo;
    usb_xfer_t tx_xfer;
    usb_xfer_t rx_xfer;
    int tx_state;
} VCOM_DEVICE_T;

/**----------------------------------------------------------------------------*
**                         Variable Prototype                                 **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
__align (4) const USB_DEVICE_DESCRIPTOR_T DeviceDescrSerial =
{
    sizeof (USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (0x0200), // USB 2.0
    0x00,
    0x00,
    0x00,
    0x40,
    USB_WORD_SWAP_ENDIAN (USB_SPREADTRUM_VENDER_ID),  // SPREADTRUM VID
    USB_WORD_SWAP_ENDIAN (USB_SPREADTRUM_CHIP_ID),
    USB_WORD_SWAP_ENDIAN (0x01),
    0x00,0x00,0x00,0x01
};

//one interface
#define CONFIG_LEN 0x20
__align (4) const uint8 ConfigDescrSerial[] =
{
    0x09,0x02,CONFIG_LEN,0x00,0x01,0x01,0x00,0xc0,0x32,
    0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x83,0x02,0x00,0x02,0x00, //Ep 3 In
    0x07,0x05,0x04,0x02,0x00,0x02,0x00, //Ep 4 OUT
};
__align (4) const uint8 AutoDL_ConfigDescrSerial[] =
{
    0x09,0x02,CONFIG_LEN,0x00,0x01,0x01,0x00,0xc0,0x32,
    0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x85,0x02,0x00,0x02,0x00, //Ep 3 In
    0x07,0x05,0x06,0x02,0x00,0x02,0x00, //Ep 4 OUT
};

static VCOM_DEVICE_T usb_dev_ctl;
static VCOM_DEVICE_T usb_dev1_ctl;

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
uint32 VCOM_GetCOnfigDescriptor(uint8 **cfg_ptr)
{
    uint32 length = CONFIG_LEN;

    if (cfg_ptr)
    {
        if (AutoDL_IsDloaderMode())  //autodownload
        {
            *cfg_ptr = (uint8 *) &AutoDL_ConfigDescrSerial[0]; //vcom
        }
        else
        {
            *cfg_ptr = (uint8 *) &ConfigDescrSerial[0];
        }
    }

    return length;
}

int VCOM_GetDevDescriptor(void *setup_, void *buf, unsigned size)
{
    struct usb_ctrlrequest *setup = (struct usb_ctrlrequest *)setup_;
    uint16 w_length = setup->wLength;
    int len = -1;
    uint8 *p = NULL;

    switch (setup->wValue >> 8) {
    case USB_DT_DEVICE:
        p = (uint8 *)&DeviceDescrSerial;
        len = MIN(18, w_length);
        break;

    case USB_DT_CONFIG:
        if (AutoDL_IsDloaderMode()) { // auto download
            p = (uint8 *)&AutoDL_ConfigDescrSerial[0];
            usb_dev_ctl.tx_ep = 0x85;
            usb_dev_ctl.rx_ep = 0x06;
        } else {
            p = (uint8 *)&ConfigDescrSerial[0];
            usb_dev_ctl.tx_ep = 0x83;
            usb_dev_ctl.rx_ep = 0x04;
        }
        len = MIN(CONFIG_LEN, w_length);
        break;

    default:
        break;
    }

    if (len > size)
        len = size;

    if (len > 0 && p)
        memcpy(buf, p, len);

    return len;
}

static void vcom_rx_complete(uint8 ep, usb_xfer_t *xfer)
{
    VCOM_DEVICE_T *vcom;
    if (xfer == NULL)
        return;

    if (xfer->status != 0) {
        return;
    }

    vcom = (VCOM_DEVICE_T *)xfer->ctx;
    osiFifoPut(&vcom->rx_fifo, xfer->buf, xfer->actual);

    xfer->callback = vcom_rx_complete;
    xfer->ctx = vcom;
    xfer->buf = vcom->rx_dma_buf;
    xfer->length = vcom->rx_dma_size;
    xfer->actual = 0;

    udc_polling_start_xfer(vcom->rx_ep, xfer);
}

void VCOM_SetConfig(uint8 cfgnum)
{
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;

    osiFifoReset(&vcom->tx_fifo);
    osiFifoReset(&vcom->rx_fifo);

    vcom->rx_xfer.callback = vcom_rx_complete;
    vcom->rx_xfer.ctx = vcom;
    vcom->rx_xfer.buf = vcom->rx_dma_buf;
    vcom->rx_xfer.length = vcom->rx_dma_size;
    vcom->rx_xfer.actual = 0;

    udc_polling_start_xfer(vcom->rx_ep, &vcom->rx_xfer);
}

static void vcom_data_init()
{
    uint32 offset = 0;
    uint8 *data_buf = (uint8 *)&_vcom_buf[0];

    usb_dev_ctl.tx_dma_buf = data_buf;
    usb_dev_ctl.tx_dma_size = VCOM_TX_DMA_SIZE;
    offset += VCOM_TX_DMA_SIZE;

    usb_dev_ctl.rx_dma_buf = data_buf + offset;
    usb_dev_ctl.rx_dma_size = VCOM_RX_DMA_SIZE;
    offset += VCOM_RX_DMA_SIZE;

    osiFifoInit(&usb_dev_ctl.tx_fifo, data_buf + offset, VCOM_TX_BUF_SIZE);
    offset += VCOM_TX_BUF_SIZE;

    osiFifoInit(&usb_dev_ctl.rx_fifo, data_buf + offset, VCOM_RX_BUF_SIZE);

    usb_dev_ctl.tx_state = VCOM_TX_IDLE;
}

/*****************************************************************************/
//  Description:    usb register init and local var init
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_usb_boot(void)
{
    USB_Disconnect();

    // SCI_TraceLow:"Enter VCOM_usb_boot\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCOM_USB20_302_112_2_17_23_8_4_1487,(uint8*)"");

    usb_dev_ctl.state = VCOM_DEV_NO_PRESENT;
    usb_dev_ctl.tx_ep = 0xff;
    usb_dev_ctl.rx_ep = 0xff;

    vcom_data_init();

    USB_SetService(USB_APP_VCOM, NULL);
    USB_Connect();
}

PUBLIC void VCOM_Open(BOOLEAN ready)
{
    USB_DCFG_U dcfg;
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;
    dcfg.dwValue = *(volatile uint32 *)USB_DCFG;

    if(dcfg.mBits.devaddr == 0)
    {
        VCOM_usb_boot();
    }
    else
    {
        usb_dev_ctl.state = ready ? VCOM_DEV_READY : VCOM_DEV_PRESENT;
        usb_dev_ctl.tx_ep = 0x83;
        usb_dev_ctl.rx_ep = 0x04;
        vcom_data_init();

        udc_polling_handler();
        vcom->rx_xfer.callback = vcom_rx_complete;
        vcom->rx_xfer.ctx = vcom;
        vcom->rx_xfer.buf = vcom->rx_dma_buf;
        vcom->rx_xfer.length = vcom->rx_dma_size;
        vcom->rx_xfer.actual = 0;
        udc_polling_start_xfer(vcom->rx_ep, &vcom->rx_xfer);
    }

    USB_SetService (USB_APP_VCOM,NULL);

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
    int condition = FALSE;
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;
    int32 ch = -1;

    do {
        udc_polling_handler();

        if (!osiFifoIsEmpty(&vcom->rx_fifo)) {
            char ch1;
            int len = osiFifoGet(&vcom->rx_fifo, &ch1, 1);
            if (len == 1) {
                ch = (int32)ch1;
                vcom->state = VCOM_DEV_READY;
            }
        }
    } while (condition);

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
    USB_Disconnect();
}

static void vcom_tx_complete(uint8 ep, usb_xfer_t *xfer)
{
    VCOM_DEVICE_T *vcom;
    if (xfer == NULL) {
        return;
    }

    vcom = (VCOM_DEVICE_T *)xfer->ctx;
    if (xfer->status == 0) {
        vcom->tx_state = VCOM_TX_DONE;
    } else {
        vcom->tx_state = VCOM_TX_FAIL;
    }
}

static int vcom_tx_start(VCOM_DEVICE_T *vcom, uint32 length)
{
    int len;

    vcom->tx_xfer.callback = vcom_tx_complete;
    vcom->tx_xfer.ctx = vcom;
    vcom->tx_xfer.buf = vcom->tx_dma_buf;
    vcom->tx_xfer.length = length;
    vcom->tx_xfer.actual = 0;
    vcom->tx_xfer.status = 0;
    vcom->tx_xfer.zlp = 1;

    vcom->tx_state = VCOM_TX_RUNNING;

    udc_polling_init();

    len = udc_polling_start_xfer(vcom->tx_ep, &vcom->tx_xfer);

    if (len == 0) {
        uint32 otk, ntk;
        otk = ntk = SCI_GetTickCount();
        do {
            udc_polling_handler();
            ntk = SCI_GetTickCount();
        } while ((ntk - otk) < VCOM_SEND_MAX_TICK && vcom->tx_state == VCOM_TX_RUNNING);

        if (vcom->tx_state == VCOM_TX_RUNNING) {
            udc_polling_cancel_xfer(vcom->tx_ep, &vcom->tx_xfer);
        }
    }

    return (vcom->tx_state == VCOM_TX_DONE) ? 0 : -1;
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
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;
    if (vcom->state != VCOM_DEV_READY) {
        return;
    }

    osiFifoPut(&vcom->tx_fifo, &nchar, 1);

    if (osiFifoBytes(&vcom->tx_fifo) >= vcom->tx_dma_size) {
        int len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
        if (len > 0) {
            len = vcom_tx_start(vcom, len);
        }

        if (len < 0) {
            vcom->state = VCOM_DEV_NO_PRESENT;
            VCOM_usb_boot();
        }
    }
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
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;
    uint32 index;

    if (vcom->state != VCOM_DEV_READY) {
        return;
    }

    for (index = 0; index < len; ++index) {
        uint8 val = data[index];
        if (0x7E == val || 0x7D == val) {
            uint8 val_1 = 0x7D;
            osiFifoPut(&vcom->tx_fifo, &val_1, 1);
            val = val ^ 0x20;
            osiFifoPut(&vcom->tx_fifo, &val, 1);
        } else {
            osiFifoPut(&vcom->tx_fifo, &val, 1);
        }

        if (osiFifoBytes(&vcom->tx_fifo) >= vcom->tx_dma_size) {
            int len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
            if (len > 0) {
                len = vcom_tx_start(vcom, len);
            }

            if (len < 0) {
                vcom->state = VCOM_DEV_NO_PRESENT;
                VCOM_usb_boot();
                break;
            }
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
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;

    // if virtual is not ready,then exit with doing nothing
    if (vcom->state != VCOM_DEV_READY) {
        return;
    }

    while (!osiFifoIsEmpty(&vcom->tx_fifo)) {
        int len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
        if (len <= 0)
            break;

        len = vcom_tx_start(vcom, len);
        if (len < 0)
            break;
    }
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
    return usb_dev_ctl.state;
}

PUBLIC void VCOM_command (void *setup_)
{
    uint8 epnum = 0;
    uint8 cmd = 0;
    struct usb_ctrlrequest *setup = (struct usb_ctrlrequest *)setup_;

    epnum = (setup->wValue >> 8) & 0xff;
    cmd = setup->wValue & 0xff;
    if (cmd == 1) {
        usb_dev_ctl.state = VCOM_DEV_READY;
    } else {
        usb_dev_ctl.state = VCOM_DEV_PRESENT;
    }
}

static void vcom1_data_init()
{
    uint32 offset = 0;
    uint8 *data_buf = (uint8 *)&_vcom1_buf[0];

    usb_dev1_ctl.tx_dma_buf = data_buf;
    usb_dev1_ctl.tx_dma_size = VCOM_TX_DMA_SIZE;
    offset += VCOM_TX_DMA_SIZE;

    usb_dev1_ctl.rx_dma_buf = data_buf + offset;
    usb_dev1_ctl.rx_dma_size = VCOM_RX_DMA_SIZE;
    offset += VCOM_RX_DMA_SIZE;

    osiFifoInit(&usb_dev1_ctl.tx_fifo, data_buf + offset, VCOM_TX_BUF_SIZE);
    offset += VCOM_TX_BUF_SIZE;

    osiFifoInit(&usb_dev1_ctl.rx_fifo, data_buf + offset, VCOM_RX_BUF_SIZE);

    usb_dev1_ctl.tx_state = VCOM_TX_IDLE;
}

PUBLIC void VCOM1_Open(BOOLEAN ready)
{
    VCOM_DEVICE_T *vcom = &usb_dev1_ctl;

    usb_dev1_ctl.state = ready ? VCOM_DEV_READY : VCOM_DEV_PRESENT;
    usb_dev1_ctl.tx_ep = 0x84;
    usb_dev1_ctl.rx_ep = 0x03;
    vcom1_data_init();

    udc_polling_handler();
    vcom->rx_xfer.callback = vcom_rx_complete;
    vcom->rx_xfer.ctx = vcom;
    vcom->rx_xfer.buf = vcom->rx_dma_buf;
    vcom->rx_xfer.length = vcom->rx_dma_size;
    vcom->rx_xfer.actual = 0;
    udc_polling_start_xfer(vcom->rx_ep, &vcom->rx_xfer);
}

PUBLIC void VCOM1_PutTxChar(uint8 nchar)
{
    VCOM_DEVICE_T *vcom = &usb_dev1_ctl;
    if (vcom->state != VCOM_DEV_READY) {
        return;
    }

    osiFifoPut(&vcom->tx_fifo, &nchar, 1);

    if (osiFifoBytes(&vcom->tx_fifo) >= vcom->tx_dma_size) {
        int len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
        if (len > 0) {
            len = vcom_tx_start(vcom, len);
        }

        if (len < 0) {
            vcom->state = VCOM_DEV_NO_PRESENT;
            VCOM_usb_boot();
        }
    }
}

PUBLIC void VCOM1_Flush(void)
{
    VCOM_DEVICE_T *vcom = &usb_dev1_ctl;

    // if virtual is not ready,then exit with doing nothing
    if (vcom->state != VCOM_DEV_READY) {
        return;
    }

    while (!osiFifoIsEmpty(&vcom->tx_fifo)) {
        int len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
        if (len <= 0)
            break;

        len = vcom_tx_start(vcom, len);
        if (len < 0)
            break;
    }
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

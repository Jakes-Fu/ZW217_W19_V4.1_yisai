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
//#include "usb20_app_config.h"
#include "mmu_drvapi.h"
#include "auto_download.h"
#include "osi_fifo.h"
//#include "usb_device_uws6121e.h"
#include "usb_utils.h"
#include "usb_serial.h"
#include "threadx_assert.h"


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

#define VCOM_BUF_SIZE (VCOM_TX_DMA_SIZE + VCOM_RX_DMA_SIZE + VCOM_TX_BUF_SIZE + VCOM_RX_BUF_SIZE) //5*1024

static __align(32) uint32 _vcom_buf[VCOM_BUF_SIZE / 4]; //256*4 = 1024
//static __align(32) uint32 _vcom1_buf[VCOM_BUF_SIZE / 4]; //256*4 =1024

enum _vcom_tx_state_e{
    VCOM_TX_IDLE,
    VCOM_TX_RUNNING,
    VCOM_TX_DONE,
    VCOM_TX_FAIL
};

enum _vcom_xfer_state_e{
    VCOM_XFER_DISABLE,
    VCOM_XFER_ENABLE
};

typedef struct _vcom_device
{
    uint32  state;
    usbEp_t *tx_ep;
    usbEp_t *rx_ep;
    void *tx_dma_buf;
    void *rx_dma_buf;
    uint32 tx_dma_size;
    uint32 rx_dma_size;
    osiFifo_t tx_fifo;
    osiFifo_t rx_fifo;
    usbXfer_t *tx_xfer;
    usbXfer_t *rx_xfer;
    int tx_state;
    usbSerial_t *cdc;
    udc_t *controller;
    uint32_t name;
    uint32 xfer_state;
} VCOM_DEVICE_T;


/**----------------------------------------------------------------------------*
**                         Variable Prototype                                 **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
extern void vcom_usb_Connect (udcNotifier_t udc_Notifier,uint8 is_assert);
extern void vcom_usb_Disconnect (void);
extern void vcom_udc_polling_handler(udc_t *udc);

/*--------------------------- Local Data ------------------------------------*/
#if 0
__align (4) const usb_device_descriptor_t DeviceDescrSerial =
{
    sizeof (usb_device_descriptor_t),
    USB_DT_DEVICE,
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
    
static const usb_endpoint_descriptor_t vcom_data_in = {
    USB_DT_ENDPOINT_SIZE,           //bLength
    USB_DT_ENDPOINT,                   //bDescriptorType
    USB_DIR_IN,                             //bEndpointAddress
    USB_ENDPOINT_XFER_BULK,        //bmAttributes
    cpu_to_le16(512),                   //wMaxPacketSize
    /*note: these two are only used in audio endpoints*/
    /*use USB_DT_ENDPOINT_SIZE in blength,not sizeof*/
    0,
};

static const usb_endpoint_descriptor_t vcom_data_out = {
    USB_DT_ENDPOINT_SIZE,           //bLength
    USB_DT_ENDPOINT,                   //bDescriptorType
    USB_DIR_OUT,                          //bEndpointAddress
    USB_ENDPOINT_XFER_BULK,        //bmAttributes
    cpu_to_le16(512),                   //wMaxPacketSize
    /*note: these two are only used in audio endpoints*/
    /*use USB_DT_ENDPOINT_SIZE in blength,not sizeof*/
    0,
};
#endif

static VCOM_DEVICE_T usb_dev_ctl;
#if VCOM1_SUPPORT
static VCOM_DEVICE_T usb_dev1_ctl;
#endif
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/

static void vcom_rx_complete(usbEp_t *ep, struct usb_xfer *xfer)
{
    VCOM_DEVICE_T *vcom;
    if (xfer == NULL)
        return;

    if (xfer->status != 0) {
        USB_LOG_TRACE("%s,%d xfer status:%d\n",__FUNCTION__,__LINE__,xfer->status);
        return;
    }
    USB_LOG_TRACE("%s,%d xfer sucess\n",__FUNCTION__,__LINE__);

    vcom = (VCOM_DEVICE_T *)xfer->param;
    osiFifoPut(&vcom->rx_fifo, xfer->buf, xfer->actual);

    xfer->complete = vcom_rx_complete;
    xfer->param = vcom;
    xfer->buf = vcom->rx_dma_buf;
    xfer->length = vcom->rx_dma_size;
    xfer->actual = 0;
    udcEpQueue(vcom->controller, vcom->rx_ep, vcom->rx_xfer);

    //udc_polling_start_xfer(vcom->rx_ep, xfer);//for compiler temp
}

static int8 vcom_data_init(void)
{
    uint32 offset = 0;
    uint8 *data_buf = (uint8 *)&_vcom_buf[0];
    usb_dev_ctl.name = DRV_NAME_USRL_COM1;

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
    
    usb_dev_ctl.xfer_state = VCOM_XFER_DISABLE;
    return 0;
}
static void vcom_data_deinit(void)
{
    usb_dev_ctl.state = VCOM_DEV_NO_PRESENT;
    usb_dev_ctl.tx_state = VCOM_TX_IDLE;
    
    osiFifoReset(&(usb_dev_ctl.tx_fifo));
    osiFifoReset(&(usb_dev_ctl.rx_fifo));
}
#if VCOM1_SUPPORT
static int8 vcom1_data_init(void)
{
    uint8 *fifo_end;
    uint8 *dma_start;
    uint8 *data_buf = (uint8 *)SCI_ALLOC_APP(VCOM_BUF_SIZE + 32);

    if (data_buf == SCI_NULL){
        USB_ERR_TRACE("[USB][VCOM]fifo buffer alloc failed\n");
        return -1;
    }
    fifo_end = data_buf + VCOM_TX_BUF_SIZE + VCOM_RX_BUF_SIZE;
    dma_start =  (uint8 *)(((uint32)fifo_end + 31) / 32 * 32);
    usb_dev1_ctl.name = DRV_NAME_USRL_COM3;

    usb_dev1_ctl.tx_dma_buf = dma_start;
    usb_dev1_ctl.tx_dma_size = VCOM_TX_DMA_SIZE;

    usb_dev1_ctl.rx_dma_buf = dma_start + VCOM_TX_DMA_SIZE;
    usb_dev1_ctl.rx_dma_size = VCOM_RX_DMA_SIZE;

    osiFifoInit(&usb_dev1_ctl.tx_fifo, data_buf, VCOM_TX_BUF_SIZE);

    osiFifoInit(&usb_dev1_ctl.rx_fifo, data_buf + VCOM_TX_BUF_SIZE, VCOM_RX_BUF_SIZE);

    usb_dev1_ctl.tx_state = VCOM_TX_IDLE;
    
    usb_dev1_ctl.xfer_state = VCOM_XFER_DISABLE;
    return 0;
}

static void vcom1_data_deinit(void)
{
    usb_dev1_ctl.state = VCOM_DEV_NO_PRESENT;
    usb_dev1_ctl.tx_state = VCOM_TX_IDLE;
    
    osiFifoReset(&(usb_dev1_ctl.tx_fifo));
    osiFifoReset(&(usb_dev1_ctl.rx_fifo));
    
    if(usb_dev1_ctl.tx_fifo.data)
        SCI_FREE(usb_dev1_ctl.tx_fifo.data);
}
#endif
static void _vcom_udcEventNotifier(void *ctx, udcEvent_t event)
{
    //USB_LOG_TRACE("udc event %x", event);
    if (event == UDC_CONNECTED)
    {
        USB_LOG_TRACE("udc connected!\n");
        usb_dev_ctl.state = VCOM_DEV_PRESENT;
    }
    else if (event == UDC_DISCONNECT)
    {
        USB_LOG_TRACE("udc disconnected!\n");
        usb_dev_ctl.state = VCOM_DEV_NO_PRESENT;
    }

}
PUBLIC void USB_Connect (void)
{
    VCOM_usb_boot();

}
PUBLIC void USB_Disconnect (void)
{
    vcom_usb_Disconnect();
    
    vcom_data_deinit();
    #if VCOM1_SUPPORT
    vcom1_data_deinit();
    #endif

}


/*****************************************************************************/
//  Description:    usb register init and local var init
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_usb_boot(void)
{
    uint8 is_assert = 0;
    USB_ERR_TRACE("%s enter\n",__FUNCTION__);

    vcom_usb_Disconnect();

    usb_dev_ctl.state = VCOM_DEV_NO_PRESENT;

    //USB_SetService(USB_APP_VCOM, NULL);
    if(vcom_data_init() < 0){
        return;
    }

#if VCOM1_SUPPORT
    is_assert = SCI_GetAssertFlag();

    //confirmed,assert mode only need one port:u2s_diag,same with runmode
    is_assert = 0;
    if(is_assert){
        if(vcom1_data_init() < 0)
            return;
        usb_dev1_ctl.state = VCOM_DEV_NO_PRESENT;
    }
#endif
    vcom_usb_Connect(_vcom_udcEventNotifier,is_assert);
}

/*
 ** Start debug usb serial, return 0 on success. 
 */
static int vcom_tranfer_enable(VCOM_DEVICE_T *vcom)
{
    udc_t *udc = vcom->controller;
    int result = 0;
    if ((result = udcEpEnable(udc, vcom->tx_ep)) < 0)
        return result;
    if ((result = udcEpEnable(udc, vcom->rx_ep)) < 0)
    {
        udcEpDisable(udc, vcom->tx_ep);
        return result;
    }

    USB_LOG_TRACE("%s,%d \n",__FUNCTION__,__LINE__);
    return 0;
}
/*
 ** Stop debug usb serial, return 0 on success.
 */
static int vcom_tranfer_disable(VCOM_DEVICE_T *vcom)
{
    udc_t *udc = vcom->controller;
    int result;
    udcEpDisable(udc, vcom->tx_ep);
    udcEpDisable(udc, vcom->rx_ep);

    USB_LOG_TRACE("%s,%d \n",__FUNCTION__,__LINE__);
    return 0;
}


PUBLIC void VCOM_Open(BOOLEAN ready)
{
    //USB_DCFG_U dcfg;
    VCOM_DEVICE_T *vcom = &usb_dev_ctl;
    //dcfg.dwValue = *(volatile uint32 *)USB_DCFG;

    if(vcom->controller == PNULL)
    {
        VCOM_usb_boot();
    }
    else
    {
        //vcom_data_init();
        vcom->state = ready ? VCOM_DEV_READY : VCOM_DEV_PRESENT;

        vcom_udc_polling_handler(vcom->controller);

        USB_LOG_TRACE("VCOM_Open:ready:%d enable transfer\n",ready);
        
        if(vcom->xfer_state != VCOM_XFER_ENABLE)
        {
            vcom_tranfer_enable(vcom);
            vcom_rx_start(vcom);
            vcom->xfer_state = VCOM_XFER_ENABLE;
        }
    }

    //USB_SetService (USB_APP_VCOM,NULL);

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
        vcom_udc_polling_handler(vcom->controller);

        if (!osiFifoIsEmpty(&vcom->rx_fifo)) {
            char ch1;
            int len = osiFifoGet(&vcom->rx_fifo, &ch1, 1);
            if (len == 1) {
                ch = (int32)ch1;
                //USB_LOG_TRACE("%s,%d getchar:0x%x\n",__FUNCTION__,__LINE__,(char)ch);
                vcom->state = VCOM_DEV_READY;
            }
        }
    } while (condition);

    //USB_LOG_TRACE("%s,%d getchar:0x%x\n",__FUNCTION__,__LINE__,ch);

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
    vcom_usb_Disconnect();
    vcom_data_deinit();
    #if VCOM1_SUPPORT
    vcom1_data_deinit();
    #endif
    USB_LOG_TRACE("%s done\n", __FUNCTION__);
}

static void vcom_tx_complete(usbEp_t *ep, struct usb_xfer *xfer)
{
#if 1
    VCOM_DEVICE_T *vcom;
    if (xfer == NULL) {
        return;
    }

    vcom = (VCOM_DEVICE_T *)xfer->param;
    if (xfer->status == 0) {
        vcom->tx_state = VCOM_TX_DONE;
    } else {
        vcom->tx_state = VCOM_TX_FAIL;
    }
    //USB_LOG_TRACE("%s,%d xfer->status:%d\n",__FUNCTION__,__LINE__,xfer->status);
#endif
}

static int vcom_tx_start(VCOM_DEVICE_T *vcom, uint32 length)
{
    int ret = 0;
    uint32 otk, ntk;
    vcom->tx_xfer->complete = vcom_tx_complete;
    vcom->tx_xfer->param = vcom;
    vcom->tx_xfer->buf = vcom->tx_dma_buf;
    vcom->tx_xfer->length = length;
    vcom->tx_xfer->actual = 0;
    vcom->tx_xfer->status = 0;
    vcom->tx_xfer->zlp = 1;
    vcom->tx_xfer->uncached = 0;

    vcom->tx_state = VCOM_TX_RUNNING;

    //USB_LOG_TRACE("%s,%d length:%d\n",__FUNCTION__,__LINE__,length);

    //udc_polling_init();
    ret = udcEpQueue(vcom->controller, vcom->tx_ep, vcom->tx_xfer);

    if(ret < 0)
    {
        USB_ERR_TRACE("%s epqueue fail\n", __FUNCTION__);
        return ret;
    }

    otk = ntk = SCI_GetTickCount();
    do {
        vcom_udc_polling_handler(vcom->controller);
        ntk = SCI_GetTickCount();
    } while ((ntk - otk) < VCOM_SEND_MAX_TICK && vcom->tx_state == VCOM_TX_RUNNING);

    if (vcom->tx_state == VCOM_TX_RUNNING) {
        USB_ERR_TRACE("%s tx_Timeout\n", __FUNCTION__);
        //cancel transfer
        udcEpDequeueAll(vcom->controller, vcom->tx_ep);
        vcom->tx_xfer->actual = 0;
        vcom->tx_xfer->status = -2;
        ret = -2;
    }
    else if(vcom->tx_state == VCOM_TX_FAIL)
    {
        USB_ERR_TRACE("%s tx failed\n", __FUNCTION__);
        ret = -1;
    }
    else if(vcom->tx_state == VCOM_TX_DONE)
    {
        //USB_LOG_TRACE("%s tx success,len=%d\n", __FUNCTION__,length);
        ret = 0;
    }

    return ret;

}

static int vcom_rx_start(VCOM_DEVICE_T *vcom)
{
    int ret = 0;

    vcom->rx_xfer->complete = vcom_rx_complete;
    vcom->rx_xfer->param = vcom;
    vcom->rx_xfer->buf = vcom->rx_dma_buf;
    vcom->rx_xfer->length = vcom->rx_dma_size;
    vcom->rx_xfer->actual = 0;
    //udc_polling_start_xfer(vcom->rx_ep, &vcom->rx_xfer);
    ret = udcEpQueue(vcom->controller, vcom->rx_ep, vcom->rx_xfer);
    USB_LOG_TRACE("%s,%d epenqueue:%d\n",__FUNCTION__,__LINE__,ret);

    return ret;

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
    int len = 0;
    if (vcom->state != VCOM_DEV_READY) {
        USB_LOG_TRACE("%s,VCOM is not ready\n",__FUNCTION__);
        return;
    }
    //USB_LOG_TRACE("%s,%d putchar:0x%x\n",__FUNCTION__,__LINE__,nchar);

    osiFifoPut(&vcom->tx_fifo, &nchar, 1);

    if (osiFifoBytes(&vcom->tx_fifo) >= vcom->tx_dma_size) {
        len = osiFifoGet(&vcom->tx_fifo, vcom->tx_dma_buf, vcom->tx_dma_size);
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
    uint32 index = 0;

    if (vcom->state != VCOM_DEV_READY) {
        USB_LOG_TRACE("%s,VCOM is not ready\n",__FUNCTION__);
        return;
    }
    //USB_LOG_TRACE("%s,%d len=%d\n",__FUNCTION__,__LINE__,len);

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
    USB_LOG_TRACE("%s,%d\n",__FUNCTION__,__LINE__);

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
#if VCOM1_SUPPORT

PUBLIC uint32 VCOM1_GetDevState (void)
{
    return usb_dev1_ctl.state;
}

PUBLIC void VCOM1_Open(BOOLEAN ready)
{
    //USB_DCFG_U dcfg;
    VCOM_DEVICE_T *vcom = &usb_dev1_ctl;
    //dcfg.dwValue = *(volatile uint32 *)USB_DCFG;

    if(vcom->controller == PNULL)
    {
        VCOM_usb_boot();
    }
    else
    {
        //vcom1_data_init();
        vcom->state = ready ? VCOM_DEV_READY : VCOM_DEV_PRESENT;

        vcom_udc_polling_handler(vcom->controller);

        USB_LOG_TRACE("VCOM1_Open:ready:%d enable transfer\n",ready);
        if(vcom->xfer_state != VCOM_XFER_ENABLE)
        {
            vcom_tranfer_enable(vcom);
            vcom_rx_start(vcom);
            vcom->xfer_state = VCOM_XFER_ENABLE;
        }

    }

    //USB_SetService (USB_APP_VCOM,NULL);

    if (AutoDL_IsDloaderMode())
    {
        USB_Disconnect();
        OS_TickDelay(30);

        USB_Connect();
    }
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

PUBLIC void VCOM1_PutTxChars (uint8* data, int32 len)
{

    VCOM_DEVICE_T *vcom = &usb_dev1_ctl;
    uint32 index;

    if (vcom->state != VCOM_DEV_READY) {
        return;
    }
    USB_LOG_TRACE("%s,%d len=%d\n",__FUNCTION__,__LINE__,len);

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

#endif
/*
 ** USB serial bind 
 */
static int prvVcomBind(uint32_t name, usbSerial_t *cdc)
{
    udc_t *controller;
    VCOM_DEVICE_T *vcom ;//

    if (!CDC_PTR_SANE(cdc) || cdc->epin_desc == NULL || cdc->epout_desc == NULL)
        return -1;
    USB_LOG_TRACE("%s port:%d bind\n",__FUNCTION__,name);
    if(name ==DRV_NAME_USRL_COM1)
        vcom = &usb_dev_ctl;
    #if VCOM1_SUPPORT
    else if(name ==DRV_NAME_USRL_COM3)
        vcom = &usb_dev1_ctl;
    #endif
    else
        return -1;
    vcom->tx_xfer = NULL;
    vcom->rx_xfer = NULL;
    vcom->tx_ep = NULL;
    vcom->rx_ep = NULL;
    vcom->controller =  controller = cdc->func->controller;
    vcom->cdc = cdc;
    vcom->tx_ep = udcEpAlloc(controller, cdc->epin_desc);
    if (vcom->tx_ep == NULL)
        goto failed;
    vcom->rx_ep = udcEpAlloc(controller, cdc->epout_desc);
    if (vcom->rx_ep == NULL)
        goto failed;
    vcom->tx_xfer = udcXferAlloc(controller); //assert 模式下不应该用动态alloc，有可能导致二次assert
    if (vcom->tx_xfer == NULL)
        goto failed;
    vcom->rx_xfer = udcXferAlloc(controller);
    if (vcom->rx_xfer == NULL)
        goto failed;
    cdc->epin_desc->bEndpointAddress = vcom->tx_ep->ue.address;
    cdc->epout_desc->bEndpointAddress = vcom->rx_ep->ue.address;
    cdc->priv = (unsigned long)vcom;
    return 0;

failed:
    USB_ERR_TRACE("%s port:%d bind failed\n",__FUNCTION__,name);        // NULL pointers can be handled
    udcXferFree(controller, vcom->tx_xfer);
    udcXferFree(controller, vcom->rx_xfer);
    udcEpFree(controller, vcom->tx_ep);
    udcEpFree(controller, vcom->rx_ep);
    vcom->tx_xfer = NULL;
    vcom->rx_xfer = NULL;
    vcom->tx_ep = NULL;
    vcom->rx_ep = NULL;
    return -1;
}
/*
 ** USB serial unbind 
 */
static void prvVcomUnbind(usbSerial_t *cdc)
{
    VCOM_DEVICE_T *vcom;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;
    vcom = (VCOM_DEVICE_T *)cdc->priv;
    USB_LOG_TRACE("%s port:%d\n",__FUNCTION__,vcom->name);

    // NULL pointers can be handled
    udcXferFree( cdc->func->controller, vcom->tx_xfer);
    udcXferFree(cdc->func->controller, vcom->rx_xfer);
    udcEpFree(cdc->func->controller, vcom->tx_ep);
    udcEpFree(cdc->func->controller, vcom->rx_ep);
    vcom->tx_xfer = NULL;
    vcom->rx_xfer = NULL;
    vcom->tx_ep = NULL;
    vcom->rx_ep = NULL;
    cdc->priv = (unsigned long)NULL;

}
 /*
  ** USB serial enable
  */
static int prvVcomEnable(usbSerial_t *cdc)
{
    VCOM_DEVICE_T *vcom;

    if (!CDC_PRIV_PTR_SANE(cdc))
        return -EINVAL;
    vcom = (VCOM_DEVICE_T *)cdc->priv;
    USB_LOG_TRACE("prvVcomEnable port:%d\n",vcom->name);

    osiFifoReset(&vcom->tx_fifo);
    osiFifoReset(&vcom->rx_fifo);

    vcom->state = VCOM_DEV_PRESENT;
    //vcom->state = VCOM_DEV_READY;
    vcom_tranfer_enable(vcom);
    vcom_rx_start(vcom); //enable or open?
    vcom->xfer_state = VCOM_XFER_ENABLE;

    return 0;
}
  
/*
 ** USB serial disable
 */
static void prvVcomDisable(usbSerial_t *cdc)
{
    VCOM_DEVICE_T *vcom;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;
    vcom = (VCOM_DEVICE_T *)cdc->priv;
    USB_LOG_TRACE("prvVcomDisable port:%d\n",vcom->name);
    vcom_tranfer_disable(vcom);
    vcom->xfer_state = VCOM_XFER_DISABLE;
}
  
/*
 ** USB serial (peer) open
 */
static bool prvVcomOpen(usbSerial_t *cdc)
{
    VCOM_DEVICE_T *vcom;

    if (!CDC_PRIV_PTR_SANE(cdc))
        return false;
    vcom = (VCOM_DEVICE_T *)cdc->priv;

    USB_LOG_TRACE("prvVcomOpen port:%d\n",vcom->name);


    vcom->state = VCOM_DEV_READY;
    //vcom_rx_start(vcom); //enable or open?

    return true;
}
  
/*
 ** USB serial (peer) close
 */
static void prvVcomClose(usbSerial_t *cdc)
{
    VCOM_DEVICE_T *vcom;

    if (!CDC_PRIV_PTR_SANE(cdc))
        return;
    vcom = (VCOM_DEVICE_T *)cdc->priv;

    USB_LOG_TRACE("prvVcomClose port:%d\n",vcom->name);
    //vcom_tranfer_disable(vcom);
    vcom->state = VCOM_DEV_PRESENT;

}
 /**
 * USB serial (peer) Resume
 */
static void prvVcomResume(usbSerial_t *cdc)
{
    USB_LOG_TRACE("prvVcomResume");
}
/**
 * USB serial (peer) Suspend
 */
static void prvVcomSuspend(usbSerial_t *cdc)
{
    
    VCOM_DEVICE_T *vcom;

    if (!CDC_PRIV_PTR_SANE(cdc))
        return;
    vcom = (VCOM_DEVICE_T *)cdc->priv;
    /*suspend means cable plug out*/
    //VCOM_disconnect();
    
    USB_LOG_TRACE("prvVcomSuspend");
    vcom_data_deinit();
    if(vcom->xfer_state == VCOM_XFER_ENABLE)
    {
        prvVcomDisable(cdc);
    }

}

static const usbSerialDriverOps_t gDVcomUsbOps = {
    prvVcomBind,
    prvVcomUnbind,
    prvVcomEnable,
    prvVcomDisable,
    prvVcomOpen,
    prvVcomClose,
    prvVcomResume,
    prvVcomSuspend,
};

const usbSerialDriverOps_t *usbVcomGetOps(void)
{
   return &gDVcomUsbOps;
}


void assert_test(void)
{
    int32 ch;
    //SCI_ASSERT(0);
    //SCI_Sleep(5000);
    VCOM_usb_boot();
    SCI_Sleep(2000);
    while(1)
    {
        ch =VCOM_GetChar();
        if(ch != -1)
        {
            USB_LOG_TRACE("VCOM_GetChar:0x%x\n",ch);

            VCOM_PutTxChar(ch);
            VCOM_Flush();
        }
    }

}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

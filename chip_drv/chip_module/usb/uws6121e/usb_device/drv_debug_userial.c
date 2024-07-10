/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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

//#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('U', 'S', 'R', 'L')
// #define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "usb_drv.h"
#include "usb_serial.h"
#include "osi_compiler.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <osi_fifo.h>
#include "sio.h"
#include "../inc/queue.h"
//#include "ucom_api.h"
#include "usbservice_api.h"
#include "os_param.h"
#include "usb_utils.h"
#include "vcom_usb.h"
/**
 * \brief usb serial for debugging
 *
 * There are several modes of userial debugging
 * - trace and cmd: either host protocol or diag protocol
 *   - rx will notify command engine through callback
 *   - tx will be mixed with trace
 *   - should poll at blue screen
 * - cmd only: either host protocol or diag protocol
 *   - rx will notify command engine through callback
 *   - tx will be sent directly
 *   - should poll at blue screen
 * - modem/mos trace: all inputs can be dropped
 *   - the difference between modem trace and mos trace is the shared memory
 *   - all rx data can be dropped
 *   - tx data is uncachable
 *   - should poll at blue screen
 *
 * The output of debugging usb serial should be self-flow-controlled. When
 * data is less than a threshold, the remaining data should be transfered
 * in next timer callback. Otherwise, the port will repeatedly transfer small
 * chunk of data, and affect the total usb performance.
 *
 * With the self-flow-control, the traces in the usb serial for trace can be
 * handled. However, it is needed to reduce the trace in trace port, and only
 * output trace for abnormal cases.
 *
 * \paragraph resource allocation
 *
 * The instances of \p drvDebugUserialPort_t are global variables.
 * - usb related resources ep/xfer are allocated at bind, and released
 *   at unbind.
 * - rx dma buffer and fifo memory are allocated at
 *   \p drvDebugUserialPortCreate, and never released. Before rx dma buffer
 *   is allocated, rx will use global receive-and-drop buffer.
 *
 * \paragraph low power and sleep
 *
 * It is a usb device, and use the usb device low power management.
 */

//#define OSI_MAKE_TAG(a, b, c, d) ((unsigned)(a) | ((unsigned)(b) << 8) | ((unsigned)(c) << 16) | ((unsigned)(d) << 24))

#define OUTPUT_AT_TXDONE (1)
#define OUTPUT_AT_TIMEOUT (2)
#define OUTPUT_AT_BLUE_SCREEN (3)

#define MODEM_USB_TX_MAX_SIZE (4096)
#define MODEM_PORT_AP_USB (5)
#define MODEM_LOG_BUFFER_OK (0x4F4B)
#define MODEM_MAX_BUF_LEN (512 * 1024) // for sanity check
#define MODEM_MIN_BUF_LEN (1024)       // for sanity check

#define TRACE_CONT_SIZE_MIN (1024)
#define TRACE_TX_POLL_INTERVAL (20)
#define SEND_PACKET_BY_TRACE_TIMEOUT (200)
#define SEND_PACKET_DIRECT_TIMEOUT (50)
static drvUSBWorkMode_t ucom_type = UCOM_CREATE_COM_DEBUG;
//uint32 last_ucom_type=0xffff;

typedef volatile struct
{
    uint32_t head;            // log buffer base address
    uint32_t length;          // log buffer total length (2^n BYTES)
    uint32_t rd_index;        // log buffer read offset
    uint32_t wr_index;        // log buffer write offset
    uint32_t cur_wr_index;    // log buffer current write offset
    uint16_t port;            // 0 -> modem uart; 2 -> close(do not send); 4 -> IPC+UART; 5 -> IPC+USB; 6 -> IPC+FLASH
    uint16_t status;          // buffer available: 0K(0x4F4B)
    uint32_t overflow_index;  // write index of protect area
    uint32_t remain_send_len; // remain the send length
    uint32_t cur_send_len;    // current the send length
    uint32_t protect_len;     // log buffer protect length
} modemLogBuf_t;

/**
 * \brief debug port status
 */
typedef union {
    struct
    {
        bool usb_rx_enabled : 1;     ///< for usb debug port only, whether is enabled
        bool usb_tx_enabled : 1;
        bool usb_host_opened : 1; ///< for usb debug port only, whether host has opened
    }us;
    uint32_t status; ///< combined status word
} drvUserialPortStatus_t;

/**
 * \brief debug port base data struct
 *
 * This is base of debug port. That means the operation pointer must be
 * the first member, and private data can follow.
 */
typedef struct drvDebugPort
{
    /** debug port device name */
    unsigned name;
    /** baud rate for uart */
    unsigned baud;
    /** debug port status */
    drvUserialPortStatus_t status;
} drvUcomPort_t;

typedef struct
{
    unsigned portname;
    unsigned baud;
    drvUserialPortStatus_t portstatus;

    bool blue_screen_mode;      // whether in blue screen ----same as s_ucom_isassert?
    bool inited;                // whether initialized, just to avoid duplicated create
    bool usb_reserve;
    SCI_MUTEX_PTR   tx_lock ;           // mutex for send packet
    //SCI_TIMER_PTR *tx_timer;       // timer to trigger tx again
    unsigned rx_dma_size;       // rx dma size
    unsigned rx_fifo_size;      // rx fifo size
    void *rx_dma_buf;           // rx dma buffer, allocated at create
    osiFifo_t rx_fifo;          // rx fifo, initialized at create
    unsigned tx_dma_size;       // tx dma size
    unsigned tx_fifo_size;      // tx fifo size
    void *tx_dma_buf;           // tx dma buffer, allocated at create
    osiFifo_t tx_fifo;          //tx fifo, initialized at create
    //osiCallback_t rx_cb;        // rx callback
    UartCallback  callback;
    //modemLogBuf_t *cp_logbuf;   // cp log buffer ctrl in shared memory
    //modemLogBuf_t *zsp_logbuf;  // zsp log buffer ctrl in shared memory
    //modemLogBuf_t *mos_logbuf;  // mos log buffer ctrl in shared memory
    //modemLogBuf_t *xfer_logbuf; // cp/zsp/mos log buffer in transfer

    usbSerial_t *cdc;
    usbXfer_t *tx_xfer;
    usbXfer_t *rx_xfer;
    usbEp_t *tx_ep;
    usbEp_t *rx_ep;
} drvDebugUserialPort_t;

#define UCOM_INVALID_VALUE    0xff

static uint8 rxDoneInBs = 0;
//for pc sysc,to save current COM_DATA/COM_DEBUG phy_port number
static uint16 s_com_data_phyport = UCOM_INVALID_VALUE;/*lint -esym(551,s_com_data_phyport)*/
static uint16 s_com_debug_phyport = UCOM_INVALID_VALUE;
static uint16 s_com_debug2_phyport = UCOM_INVALID_VALUE;
unsigned ppp_tx_ep_num = 0xff;
static void prvUserialRxStartLocked(drvDebugUserialPort_t *d);
static void prvUserialRxDoneCb(usbEp_t *ep, usbXfer_t *xfer);
static bool prvUserialStartTxLocked(drvDebugUserialPort_t *d, const void *data, unsigned size, bool uncached);
static void prvUserialTxDoneCb(usbEp_t *ep, usbXfer_t *xfer);
LOCAL uint32 ucom_test_callback (uint32 event);
extern BOOLEAN drvUsbSetWorkMode(drvUSBWorkMode_t mode);
extern void UdcStartTxDirectly(unsigned ep, void *buf,unsigned size);

#if 0//USB_DEBUG_ENABLE
char usbLog_str[250] = {0};
PUBLIC unsigned int SCI_TraceLow_ForUsb(const char * x_format, ...)
{
    va_list     args;
    int         nBuf;

    const  uint16  buf_len=500-1;
    SCI_DisableIRQ();
    va_start(args, x_format);
    nBuf = _vsnprintf(usbLog_str, buf_len,x_format, args);
    va_end(args);
    SCI_RestoreIRQ();
    Debug_Uart4_puts(usbLog_str);  //uart output
    return nBuf;
}
#endif

static uint32 prvDummyRxCallback(uint32 event)
{
    if(event == EVENT_DATA_TO_READ)
    {
        rxDoneInBs = 1;
    }
    else if(event == EVENT_WRITE_COMPLETE)
    {
        //USB_LOG_TRACE("%s[%d] tx done\n",__FUNCTION__,d->portname);
    }
    return TRUE;

}

static drvDebugUserialPort_t gDuserials[USB_MAX_NUM] =
{
    /*portname,baudrate,portstatus,  bsmode  inited  usb_reserve  tx_lock   rx_dma_size     rx_fifo_size    rx_dma_buf  rx_fifo  tx_dma_size  tx_fifo_size  tx_dma_buf   tx_fifo   callback    cdc */
    {DRV_NAME_USRL_COM0,921600,0,       0,      0,      0,          PNULL,     512,            1024,           PNULL,   PNULL,      512,         1024,       PNULL,       PNULL,    PNULL,      PNULL, PNULL, PNULL, PNULL, PNULL }, //AT
    {DRV_NAME_USRL_COM1,115200,0,       0,      0,      0,          PNULL,     512,            512,            PNULL,   PNULL,      2*1024,      4*1024,     PNULL,       PNULL,    PNULL,      PNULL, PNULL, PNULL, PNULL, PNULL }, //diag
    {DRV_NAME_USRL_COM2,115200,0,       0,      0,      0,          PNULL,     2*1024,       4 *1024,          PNULL,   PNULL,      2*1024,      4 *1024,    PNULL,       PNULL,    PNULL,      PNULL, PNULL, PNULL, PNULL, PNULL }, //debug host
    {DRV_NAME_USRL_COM3,115200,0,       0,      0,      0,          PNULL,      0,               64,           PNULL,   PNULL,      4*1024,      8*1024,     PNULL,       PNULL,    PNULL,      PNULL, PNULL, PNULL, PNULL, PNULL }, //modem log
    {DRV_NAME_USRL_COM4,921600,0,       0,      0,      0,          PNULL,     512,            1024,           PNULL,   PNULL,      512,         1024,       PNULL,       PNULL,    PNULL,      PNULL, PNULL, PNULL, PNULL, PNULL }, //PPP_AT
};
/**
 * Find the predefined debugging usb serial by device name
 */
static drvDebugUserialPort_t *prvUserialFindByName(unsigned name)
{
    unsigned n;
    for ( n = 0; n < OSI_ARRAY_SIZE(gDuserials); n++)
    {
        if (gDuserials[n].portname == name)
            return &gDuserials[n];
    }
    return NULL;
}

/**
 * Start tx transfer.
 */
#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1))
static bool prvUserialStartTxLocked(drvDebugUserialPort_t *d, const void *data, unsigned size, bool uncached)
{
    usbXfer_t *x = d->tx_xfer;
    int ret;
    if(x->status == -EINPROGRESS )
    {
        USB_ERR_TRACE("[USB][UCOM]tx_xfer busy, port:%d\n",d->portname);
        return false;
    }
    x->buf = data;//(void *)ALIGN(data, 4); // though it should be aligned
    x->zlp = 1;
    x->length = size;
    x->param = d;
    x->status = 0; // though it will set to -EINPROGRESS at queue
    x->uncached = uncached;
    x->complete = prvUserialTxDoneCb;

    ret = udcEpQueue(d->cdc->func->controller, d->tx_ep, x);
    if (ret < 0){
        USB_ERR_TRACE("[USB][UCOM]tx enqueue fail, port:%d,ret=%d\n",d->portname,ret);
        return false;
    }
    //USB_LOG_TRACE("tx enqueue success  port:%d,ret=%d\n",d->portname,ret);

    return true;
}
/**
 * TX done callback, may be called in usb isr, or blue screen poll
 */
static void prvUserialTxDoneCb(usbEp_t *ep, usbXfer_t *xfer)
{
    drvDebugUserialPort_t *d = (drvDebugUserialPort_t *)xfer->param;
    if (xfer->status != 0)
    {
        USB_ERR_TRACE("[USB][UCOM]port:%d tx fail, status/%d, size/%d\n",d->portname, xfer->status, xfer->actual);
        //if(d->callback) //走到这里一般是cancelled，callback上报给上层也没有意义
            //d->callback(EVENT_WRITE_COMPLETE);
    }
    else
    {
        //USB_LOG_TRACE("%s[%d] tx done,length:%d\n",__FUNCTION__,d->portname,xfer->actual);
        if(d->callback)
            d->callback(EVENT_WRITE_COMPLETE);
    }
}
#if 0
/**
 * Debug usb serial output, called in timer isr or manual trigger
 */
static void prvUserialOutputTimeout(void *param)
{
    drvDebugUserialPort_t *d = (drvDebugUserialPort_t *)param;
    prvUserialTraceOutputLocked(d, OUTPUT_AT_TIMEOUT);
    prvUserialModemLogOutputLocked(d, OUTPUT_AT_TIMEOUT);
    prvUserialMosLogOutputLocked(d, OUTPUT_AT_TIMEOUT);
}
#endif

/**
 * RX done callback, may be called in usb isr, or blue screen poll
 */
static void prvUserialRxDoneCb(usbEp_t *ep, usbXfer_t *xfer)
{
    drvDebugUserialPort_t *d = (drvDebugUserialPort_t *)xfer->param;
    if (xfer->status != 0)
    {
        USB_ERR_TRACE("[USB][UCOM]port:%d rx fail, status/%d, size/%d\n",d->portname, xfer->status, xfer->actual);
    }
    else
    {
        USB_LOG_TRACE("[USB][UCOM]port:%d rx done, status/%d, size/%d\n",d->portname, xfer->status, xfer->actual);

        // In case host doesn't send UCDC_SET_CONTROL_LINE_STATE,
        // "open" should be set forcedly to enabled write.
        if (!d->portstatus.us.usb_host_opened)
        {
            d->portstatus.us.usb_host_opened = true;
            //prvUserialOutputTimeout(d); // fake timeout to trigger output
        }

        if (d->rx_dma_buf != NULL)
        {
            // cache invalidate is called before callback
            osiFifoPut(&d->rx_fifo, xfer->buf, xfer->actual);
            
            prvUserialRxStartLocked(d);
            //d->rx_cb(d->rx_cb_ctx);
            if(d->callback)
                d->callback(EVENT_DATA_TO_READ);
        } 
        
    }
}

/**
 * Start RX, called in rx done callback, or device enable
 */
static void prvUserialRxStartLocked(drvDebugUserialPort_t *d)
{
    int ret ;
    usbXfer_t *x = d->rx_xfer;
    if (!d->portstatus.us.usb_rx_enabled)
        return;

    rxDoneInBs = 0;
    if (d->rx_dma_buf == NULL)
    {
        x->buf = gDummyUsbRxBuf;
        x->length = USB_DUMMY_RX_BUF_SIZE;
        x->uncached = true;
    }
    else
    {
        x->buf = d->rx_dma_buf;
        x->length = d->rx_dma_size;
        x->uncached = false;
    }
    x->param = d;
    x->complete = prvUserialRxDoneCb;

    ret = udcEpQueue(d->cdc->func->controller, d->rx_ep, x);

    //USB_LOG_TRACE("port:%d rx start 0x%08x/%d return/%d", d->portname, x->buf, x->length, ret);
    if (ret < 0)
    {
        USB_ERR_TRACE("[USB][UCOM]port %d start failed. enqueue xfer return (%d)\n",d->portname, ret);

        // failure except disconnected are not permitted
       // if (ret != -ENOENT)
            //SCI_ASSERT(0);
    }
    else
        USB_LOG_TRACE("port:%d rx start success 0x%08x/%d return/%d \n", d->portname, x->buf, x->length, ret);

}

/**
 * Start debug usb serial, return 0 on success.
 */
static int prvUserialStartLocked(drvDebugUserialPort_t *d)
{
    udc_t *udc = d->cdc->func->controller;
    int result;

    if ((result = udcEpEnable(udc, d->tx_ep)) < 0)
        return result;

    if ((result = udcEpEnable(udc, d->rx_ep)) < 0)
    {
        udcEpDisable(udc, d->tx_ep);
        return result;
    }
    return 0;
}


/**
 * Start debug usb serial, return 0 on success.
 */
static int prvUserialEnableTx(drvDebugUserialPort_t *d)
{
    udc_t *udc = d->cdc->func->controller;
    int result;

    if ((result = udcEpEnable(udc, d->tx_ep)) < 0)
        return result;

    return 0;
}

/**
 * Start debug usb serial, return 0 on success.
 */
static int prvUserialEnableRx(drvDebugUserialPort_t *d)
{
    udc_t *udc = d->cdc->func->controller;
    int result;

    if ((result = udcEpEnable(udc, d->rx_ep)) < 0)
    {
        return result;
    }
    return 0;
}


/**
 * Stop debug usb serial.
 */
static void prvUserialStopLocked(drvDebugUserialPort_t *d)
{
    udc_t *udc = d->cdc->func->controller;
    if (d->rx_ep)
    {
        udcEpDequeueAll(udc, d->rx_ep);
        udcEpDisable(udc, d->rx_ep);
    }
    if (d->tx_ep)
    {
        udcEpDequeueAll(udc, d->tx_ep);
        udcEpDisable(udc, d->tx_ep);
    }
}
/**
 * Wait direct tx transfer finish
 */
uint32 tx_CallCnt=0;
uint32 tx_TimeoutCnt=0;
uint32 tx_TimeoutCnt_bs=0;

static bool prvUserialWaitDirectTxFinish(drvDebugUserialPort_t *d)
{
//等SCI_GetTickCount OK后统计timeout次数,每个port应该单独计数

    uint32 end = SCI_GetTickCount() + SEND_PACKET_DIRECT_TIMEOUT;
    tx_CallCnt++;

       while (SCI_GetTickCount() < end){
            if(d->tx_xfer->status != -EINPROGRESS)
               return TRUE;
            SCI_Sleep(2);
       }
       tx_TimeoutCnt++;
       USB_ERR_TRACE("[USB][UCOM]%s port:%d tx_CallCnt=%d,tx_TimeoutCnt=%d\n", __FUNCTION__ ,d->portname,tx_CallCnt,tx_TimeoutCnt);
       return FALSE;
}

/**
 * Wait direct tx transfer finish in blue screen mode
 */
static bool prvUserialWaitDirectTxFinishBs(drvDebugUserialPort_t *d)
{
    uint32 end = SCI_GetTickCount() + SEND_PACKET_DIRECT_TIMEOUT;
        while (SCI_GetTickCount() < end){
                if(d->tx_xfer->status != -EINPROGRESS)
                       return TRUE;
                udcGdbPollIntr(d->cdc->func->controller);
        }
        USB_ERR_TRACE("[USB][UCOM]%s port:%d tx_TimeoutCnt_bs=%d\n", __FUNCTION__ ,d->portname,tx_TimeoutCnt_bs++);
        return FALSE;
}
/**
 * Wait direct Rx transfer finish in blue screen mode
 */
static bool prvUserialWaitDirectRxFinishBs(drvDebugUserialPort_t *d)
{
        uint32 end = SCI_GetTickCount() + 2000; //2s timeout
        while (osiFifoIsEmpty(&d->rx_fifo)/*d->rx_xfer->status == -EINPROGRESS*/){
            udcGdbPollIntr(d->cdc->func->controller);

            if(SCI_GetTickCount() >= end)
                return FALSE; //timeout
        }
    
        return TRUE;
        
}

/**
 * Send packet through debug usb serial.
 */
static uint32 prvUserialSendPacket(drvDebugUserialPort_t *d, const void *data, unsigned size)
{
     int32 result = 0;
     bool ok;
    if (!d->portstatus.us.usb_host_opened){
        
        //USB_ERR_TRACE("%s port:%d usb_host_opened=0\n", __FUNCTION__ ,d->portname);

        return false;
    }
    if (!d->portstatus.us.usb_tx_enabled){
        
        USB_ERR_TRACE("[USB][UCOM]%s port:%d usb_tx_enabled=0\n", __FUNCTION__ ,d->portname);

        return false;
    }

    if (d->blue_screen_mode)
    {
        ok = /*prvUserialWaitDirectTxFinishBs(d) &&*/
               prvUserialStartTxLocked(d, data, size, false) && 
               prvUserialWaitDirectTxFinishBs(d);
    return ok ? size:0;
    }


    //mutex will cause assert in timer callback or intr callback
    /*
    result=SCI_GetMutex (d->tx_lock, SCI_WAIT_FOREVER);
    if (SCI_SUCCESS != result)
    {
        USB_LOG_TRACE("%d is busy\n",__FUNCTION__);
        return;
    }
    */
    ok = false;
    #if 0
    if (d->port.mode.trace_enable)
    {
        d->trace_no_wait = true;
        ok = prvUserialSendPacketByTrace(d, data, size);
        d->trace_no_wait = false;
    }
    else
    #endif
    {
            /*prvUserialWaitDirectTxFinish(d) &&*/   //bringup_tmp,SCI_GetTickCount not work normally
        ok =  prvUserialStartTxLocked(d, data, size, false);

    }   

    //SCI_PutMutex(d->tx_lock);
    return ok ? size:0;
}

/**
 * Read from debug usb serial, return read bytes
 */
static int prvUserialRead(drvDebugUserialPort_t *d, void *data, unsigned size)
{
    return (d->rx_dma_buf == NULL) ? 0 : osiFifoGet(&d->rx_fifo, data, size);
}

/**
 * Set trace enable or disable
 */
static void prvUserialSetTraceEnable(drvDebugUserialPort_t *d, bool enable)
{
    udc_t *udc = d->cdc->func->controller;

    if (enable)
    {
        //unsigned critical = osiEnterCritical();
        d->portstatus.us.usb_tx_enabled = (prvUserialEnableTx(d) == 0);
        //osiExitCritical(critical);
    }
    else
    {
        if (d->tx_ep)
        {
            udcEpDequeueAll(udc, d->tx_ep);
            udcEpDisable(udc, d->tx_ep);
            d->portstatus.us.usb_tx_enabled =false;
        }

    }
        
}

/**
 * Set rx callback
 */
static void prvUserialSetRxCallback(drvDebugUserialPort_t *d, UartCallback cb, void *param)
{
    //drvDebugUserialPort_t *d = OSI_CONTAINER_OF(p, drvDebugUserialPort_t, port);
    //d->rx_cb_ctx = param;
    d->callback= (cb == NULL) ? prvDummyRxCallback : cb;
}

/**
 * Enter blue screen mode
 */
static void prvUserialBsEnter(void *param)
{
    drvDebugUserialPort_t *d = (drvDebugUserialPort_t *)param;
    d->blue_screen_mode = true;
    d->callback = prvDummyRxCallback;
#if 0
    if (d->port.mode.trace_enable)
    {
#ifdef CONFIG_KERNEL_HOST_TRACE
        // It will make USB easier to copy the data to RAM (stack).
        uint8_t event_data[GDB_EVENT_DATA_SIZE];
        memcpy(event_data, gBlueScreenEventData, GDB_EVENT_DATA_SIZE);

        prvUserialSendPacket(&d->port, event_data, GDB_EVENT_DATA_SIZE);
#endif
    }
#endif
}
/**
 * Release udc related resources
 */
static void prvUserialFreeUdcResources(drvDebugUserialPort_t *d, udc_t *controller)
{
    // NULL pointers can be handled
    udcXferFree(controller, d->tx_xfer);
    udcXferFree(controller, d->rx_xfer);
    udcEpFree(controller, d->tx_ep);
    udcEpFree(controller, d->rx_ep);

    d->tx_xfer = NULL;
    d->rx_xfer = NULL;
    d->tx_ep = NULL;
    d->rx_ep = NULL;
}

/**
 * USB serial bind
 */
static int prvUserialBind(uint32_t name, usbSerial_t *cdc)
{
    udc_t *controller;
    drvDebugUserialPort_t *d = prvUserialFindByName(name);
    if (d == NULL)
        return -1;

    if (!CDC_PTR_SANE(cdc) || cdc->epin_desc == NULL || cdc->epout_desc == NULL)
        return -1;

    USB_LOG_TRACE("%s port:%d bind\n",__FUNCTION__,name);

    //unsigned critical = osiEnterCritical();
    d->tx_xfer = NULL;
    d->rx_xfer = NULL;
    d->tx_ep = NULL;
    d->rx_ep = NULL;

    controller = cdc->func->controller;
    d->cdc = cdc;
    d->tx_ep = udcEpAlloc(controller, cdc->epin_desc);
    if (d->tx_ep == NULL)
        goto failed;
    d->rx_ep = udcEpAlloc(controller, cdc->epout_desc);
    if (d->rx_ep == NULL)
        goto failed;
    d->tx_xfer = udcXferAlloc(controller);
    if (d->tx_xfer == NULL)
        goto failed;
    d->rx_xfer = udcXferAlloc(controller);
    if (d->rx_xfer == NULL)
        goto failed;

    cdc->epin_desc->bEndpointAddress = d->tx_ep->ue.address;
    cdc->epout_desc->bEndpointAddress = d->rx_ep->ue.address;
    cdc->priv = (unsigned long)d;
    if(d->portname == USB_COM4) //ppp_at port
    {
        ppp_tx_ep_num = d->tx_ep->ue.se.num; //save tx_ep number
    }
    //osiExitCritical(critical);
    return 0;

failed:
    prvUserialFreeUdcResources(d, controller);
    USB_ERR_TRACE("[USB][UCOM]prvUserialBind fail\n");
    //osiExitCritical(critical);
    return -1;
}

/**
 * USB serial unbind
 */
static void prvUserialUnbind(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;

    d = (drvDebugUserialPort_t *)cdc->priv;
    USB_LOG_TRACE("%s CDC serial %4c unbind",__FUNCTION__,d->portname);

    //unsigned critical = osiEnterCritical();
    prvUserialFreeUdcResources(d, cdc->func->controller);
    cdc->priv = (unsigned long)NULL;
    //osiExitCritical(critical);
}

/**
 * USB serial enable
 */
static int prvUserialEnable(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return -EINVAL;

    d = (drvDebugUserialPort_t *)cdc->priv;

    //uint32_t critical = osiEnterCritical();
    d->portstatus.us.usb_host_opened = false; //
    d->portstatus.us.usb_tx_enabled = (prvUserialEnableTx(d) == 0);
    d->portstatus.us.usb_rx_enabled = (prvUserialEnableRx(d) == 0);

    USB_LOG_TRACE("%s port:%d enable,usb_host_opened:%d,usb_tx_enabled:%d,usb_rx_enabled:%d\n",__FUNCTION__,d->portname,d->portstatus.us.usb_host_opened,d->portstatus.us.usb_tx_enabled,d->portstatus.us.usb_tx_enabled);
    prvUserialRxStartLocked(d);
    //osiExitCritical(critical);
    return 0;
}

/**
 * USB serial disable
 */
static void prvUserialDisable(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;

    d = (drvDebugUserialPort_t *)cdc->priv;
    USB_LOG_TRACE("prvUserialDisable %d disable\n", d->portname);

    //uint32_t critical = osiEnterCritical();
    d->portstatus.us.usb_rx_enabled = false;
    d->portstatus.us.usb_tx_enabled = false;
    d->portstatus.us.usb_host_opened = false;
    prvUserialStopLocked(d);
    //osiExitCritical(critical);
}

/**
 * USB serial (peer) open
 */
static bool prvUserialOpen(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return false;

    d = (drvDebugUserialPort_t *)cdc->priv;

    USB_LOG_TRACE("prvUserialOpen %d open\n", d->portname);

    //uint32_t critical = osiEnterCritical();
    if (!d->portstatus.us.usb_host_opened)
    {
        d->portstatus.us.usb_host_opened = true;
        //prvUserialStartTxTimer(d);

    }
    //osiExitCritical(critical);
    return true;
}

/**
 * USB serial (peer) close
 */
static void prvUserialClose(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;

    d = (drvDebugUserialPort_t *)cdc->priv;

    USB_LOG_TRACE("prvUserialClose %d close", d->portname);

    //uint32_t critical = osiEnterCritical();
    d->portstatus.us.usb_host_opened = false;
    
    //osiExitCritical(critical);
}
/**
 * USB serial (peer) Resume
 */
static void prvUserialResume(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;

    d = (drvDebugUserialPort_t *)cdc->priv;

    USB_ERR_TRACE("prvUserialResume %d\n", d->portname);

    //uint32_t critical = osiEnterCritical();
    if(!d->portstatus.us.usb_host_opened){
        d->portstatus.us.usb_host_opened = true;
    }
    //osiExitCritical(critical);
}
/**
 * USB serial (peer) Suspend
 */
static void prvUserialSuspend(usbSerial_t *cdc)
{
    drvDebugUserialPort_t *d;
    if (!CDC_PRIV_PTR_SANE(cdc))
        return;

    d = (drvDebugUserialPort_t *)cdc->priv;

    USB_ERR_TRACE("prvUserialSuspend %d\n", d->portname);

    //uint32_t critical = osiEnterCritical();
    d->portstatus.us.usb_host_opened = false;
    if(d->callback)
        d->callback (EVENT_SHUTDOWN_COMPLETE);
    
    //osiExitCritical(critical);
}
static const usbSerialDriverOps_t gDuserialUsbOps = {
    prvUserialBind,
    prvUserialUnbind,
    prvUserialEnable,
    prvUserialDisable,
    prvUserialOpen,
    prvUserialClose,
    prvUserialResume,
    prvUserialSuspend,
};

const usbSerialDriverOps_t *usbDebugSerialGetOps(void)
{
    return &gDuserialUsbOps;
}

/*****************************************************************************/
//  Description:    Ucom component exit function
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void UCOM_exit (void)
{
    SIO_CONTROL_S m_dcb_data;
    uint8 i;
    drvDebugUserialPort_t *d;
    USB_LOG_TRACE("%s enter \n",__FUNCTION__);

#ifdef MUX_SUPPORT
    if (MUX_IsUsbDataEnable())
    {
        SCI_DisableIRQ();
        MUX_Close (COM_DATA);
        MUX_Create (COM_DATA, s_com_data_phyport,&m_dcb_data);
        SCI_RestoreIRQ();
    }
#else
    if ( (s_com_data_phyport != UCOM_INVALID_VALUE)
            && (s_com_data_phyport != VIR_COM0)
            && (s_com_data_phyport != VIR_COM1)
            && (s_com_data_phyport != VIR_COM2))
    {
        // Change COM_DATA from usb virutal com to uart
        m_dcb_data.flow_control = 0;
        m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);

        SCI_DisableIRQ();
        SIO_Close (COM_DATA);

        SIO_Create (COM_DATA,s_com_data_phyport,&m_dcb_data);
        SCI_RestoreIRQ();
    }
#endif

    if ( (s_com_debug_phyport != UCOM_INVALID_VALUE)
            && (s_com_debug_phyport != VIR_COM0)
            && (s_com_debug_phyport != VIR_COM1)
            && (s_com_debug_phyport != VIR_COM2)
            && (s_com_debug_phyport != VIR_COM3)
            && (s_com_debug_phyport != VIR_COM4))
    {
        // Change COM_DEBUG from usb virutal com to uart

        m_dcb_data.flow_control = 0;
        m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DEBUG);

        SCI_DisableIRQ();
        SIO_Close (COM_DEBUG);

        m_dcb_data.baud_rate = 921600;
        SIO_Create (COM_DEBUG,s_com_debug_phyport,&m_dcb_data);                     /*lint !e718 */
        SCI_RestoreIRQ();
    }

    if ( (s_com_debug2_phyport != UCOM_INVALID_VALUE)
            && (s_com_debug2_phyport != VIR_COM0)
            && (s_com_debug2_phyport != VIR_COM1)
            && (s_com_debug2_phyport != VIR_COM2)
            && (s_com_debug2_phyport != VIR_COM3)
            && (s_com_debug2_phyport != VIR_COM4))
    {
        // Change COM_DEBUG from usb virutal com to uart
        SCI_DisableIRQ();
        SIO_Close (COM_DEBUG2);
        SCI_RestoreIRQ();
    }
    // clear local var
    s_com_data_phyport  = UCOM_INVALID_VALUE;
    s_com_debug_phyport = UCOM_INVALID_VALUE;
    s_com_debug2_phyport = UCOM_INVALID_VALUE;
}


PUBLIC void UCOM_CreatForCOM (uint32 type)
{
    SIO_CONTROL_S m_dcb_data={0,0,0,0,0};
    
    //if(last_ucom_type == type)
        //return;
    
    USB_LOG_TRACE("UCOM_CreatForCOM\n ");
    
    //last_ucom_type = type;
    switch (type)
    {
        case UCOM_CREATE_COM_NPI:
            SCI_DisableIRQ();
            /* 校准模式不需要AT port
            //Change COM_DATA from uart to usb virutal com
            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);

            SIO_Close (COM_DATA);
            SIO_Create (COM_DATA,VIR_COM0,&m_dcb_data); //AT
            */
            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG);

            SIO_Close(COM_DEBUG);
            SIO_Create(COM_DEBUG,VIR_COM1,&m_dcb_data);//DIAG

            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG2);

            SIO_Close(COM_DEBUG2);
            SIO_Create(COM_DEBUG2,VIR_COM4,&m_dcb_data);//MODEM LOG
            SCI_RestoreIRQ();

            break;
        case UCOM_CREATE_COM_DATADISK:
            //Change COM_DATA from uart to usb virutal com
            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);
            s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);

#ifdef MUX_SUPPORT
            MUX_Close (COM_DATA);
            MUX_Create (COM_DATA, VIR_COM0,&m_dcb_data);
#else
            SIO_Close (COM_DATA);
         SIO_Create (COM_DATA,VIR_COM0,&m_dcb_data);
#endif
            break;

        case UCOM_CREATE_COM_DATA:
            {
                //Change COM_DATA from uart to usb virutal com
                m_dcb_data.flow_control = 0;
                m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);
                s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);

#ifdef MUX_SUPPORT
                MUX_Close (COM_DATA);
                MUX_Create (COM_DATA, VIR_COM0,&m_dcb_data);
#else
                SIO_Close (COM_DATA);
                SIO_Create (COM_DATA,VIR_COM0,&m_dcb_data);
#endif
            }
            break;

        case UCOM_CREATE_COM_DEBUG:
            type = UCOM_CREATE_COM_DEBUGDATA;
        case UCOM_CREATE_COM_DEBUGDATA:    /*lint !e616  !e825*/
            {
                s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);
                s_com_debug_phyport = SIO_GetPhyPortNo (COM_DEBUG);
                //s_com_debug2_phyport = SIO_GetPhyPortNo (COM_DEBUG2);
#ifdef MUX_SUPPORT
                MUX_Close (COM_DATA);
                MUX_Create (COM_DATA, VIR_COM0,&m_dcb_data);
#else
                /*important, mux and sio share same port, so we should disable sio port when using mux func
                   otherwise we will misoperate com_data and uart0 to error state
                */
                m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DATA);
                SIO_Close (COM_DATA);
                SIO_Create (COM_DATA, VIR_COM0, &m_dcb_data);
#endif
                m_dcb_data.flow_control = 0;
                m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG);

                SIO_Close (COM_DEBUG);
                //UCOM_SetEndpointMap(type);
                SIO_Create (COM_DEBUG,VIR_COM1,&m_dcb_data);//AP LOG
            }
            break;

        case UCOM_CREATE_COM_DEBUGDATA_MODEM:
            s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);
            s_com_debug_phyport = SIO_GetPhyPortNo (COM_DEBUG);
            s_com_debug2_phyport = SIO_GetPhyPortNo (COM_DEBUG2);
#ifdef MUX_SUPPORT
            MUX_Close(COM_DATA);
            MUX_Create(COM_DATA, VIR_COM0,&m_dcb_data);
#else
#ifdef PPP_USB

            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DATA2);//PPP_AT
            SIO_Close(COM_DATA2);
            SIO_Create(COM_DATA2, VIR_COM3, &m_dcb_data);
#endif

            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DATA);//AT
            SIO_Close(COM_DATA);
            SIO_Create(COM_DATA, VIR_COM0, &m_dcb_data);
#endif

            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG);

            SIO_Close(COM_DEBUG);
            SIO_Create(COM_DEBUG,VIR_COM1,&m_dcb_data);//AP LOG

            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG2);

            SIO_Close(COM_DEBUG2);
            SIO_Create(COM_DEBUG2,VIR_COM4,&m_dcb_data); //CP LOG
            
            break;
        case DRV_USB_UDISK:
        case DRV_USB_CHARGER_ONLY:
            /*restore uart log*/
            UCOM_exit();
           break;
        default:
            break;
    }

}

PUBLIC void UCOM_SetWorkMode(uint8 service_type, uint8 calibmode)
{
    uint8 assert_mode  =  0;
    USB_LOG_TRACE("UCOM_SetWorkMode  service_type:%d:\n",service_type);
    OSPARAM_GetAssertMode(&assert_mode);

    if (calibmode)
    {
        ucom_type= UCOM_CREATE_COM_NPI;
    }
    else
    {

        /*USB_SERVICE_UCOM is partly used in datashare and VirtUart(what are these functions for?)
        * in datashare UCOM is covered by PLATFORM_ANTISW3
        * VirtUart is not used
        */
        if(USB_SERVICE_UCOM == service_type)
        {
            drvUsbSetWorkMode(UCOM_CREATE_COM_DEBUG);
            return;
        }
        else if(USB_SERVICE_LOG == service_type)
        {
            if (SCI_ASSERT_DEBUG_MODE == assert_mode)
                ucom_type = UCOM_CREATE_COM_DEBUGDATA_MODEM; // pid = 0x0006
            else
                ucom_type = UCOM_CREATE_COM_DEBUGDATA_MODEM; // pid = 0x0006
        }
        else if(USB_SERVICE_UDISK == service_type)
        {
            ucom_type = DRV_USB_UDISK;
        }
        else
        {
            ucom_type = DRV_USB_CHARGER_ONLY;
            USB_ERR_TRACE("[USB][UCOM]UCOM_SetWorkMode DRV_USB_CHARGER_ONLY!");
            //drvUsbSetWorkMode(ucom_type);
            //return;
        }
        USB_LOG_TRACE("UCOM_SetWorkMode,ucom_type:%d\n",ucom_type);

    }
    UCOM_CreatForCOM(ucom_type);
    drvUsbSetWorkMode(ucom_type);
}

PUBLIC drvUSBWorkMode_t UCOM_GetUcomType(void)
{
    return ucom_type;

}
PUBLIC void UCOM_SetUcomType(drvUSBWorkMode_t mode)
{
    ucom_type = mode;

}

/*****************************************************************************/
//  Description:    usb virtual com creat
//  Global resource dependence:
//  Author:         weihua.wang
//  Param:          uint32 port_num: usb virtual com num USB_COM0/USB_COM1
//                  UART_INIT_PARA_T* :include param for uart, only useful for uart
//                  UartCallback func :uplayer callback
//  Note:
/*****************************************************************************/
PUBLIC uint32 UCOM_Creat (uint32 port_num,
                          UART_INIT_PARA_T *para,
                          UartCallback func)
{
    uint8 *fifo_start;
    uint8 *fifo_end;
    uint8 *dma_start;
    uint8 *rx_buf;
    uint8 *tx_buf;
    void *rx_fifo_buf;
    void *tx_fifo_buf;
    drvDebugUserialPort_t *d;
    SCI_ASSERT (func != NULL); /*assert verified*/
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    USB_LOG_TRACE(" UCOM_Creat port_num = %d\n",port_num);

    d = prvUserialFindByName(port_num);
    if(d->inited)
         return UART_SUCCESS;

    switch(d->portname)
    {
        case DRV_NAME_USRL_COM0: //AT
            d->rx_dma_size = 512;
            d->rx_fifo_size = 1024;
            d->tx_dma_size = 512;
            d->tx_fifo_size=1024; //not used
            break;
        case DRV_NAME_USRL_COM1: //AP LOG
            d->rx_dma_size = 512;
            d->rx_fifo_size = 512;
            d->tx_dma_size = 2*1024;
            d->tx_fifo_size= 4*1024;//used only in assert dump mode
            break;      
        case DRV_NAME_USRL_COM2: //DEBUG HOST,not used
            d->rx_dma_size = 512;
            d->rx_fifo_size = 512;
            d->tx_dma_size = 512;
            d->tx_fifo_size= 512;//not used
            break;  
        case DRV_NAME_USRL_COM3://CP LOG
            d->rx_dma_size = 0;
            d->rx_fifo_size = 64;
            d->tx_dma_size = 4*1024;
            d->tx_fifo_size = 8*1024;//tx fifo is not used CURRENTLY,but tx_fifio_size is used by sio
            break;
    #ifdef PPP_USB
        case DRV_NAME_USRL_COM4://PPP AT
            d->rx_dma_size = 2*1024;
            d->rx_fifo_size = 2*1024;
            d->tx_dma_size = 2*1024;
            d->tx_fifo_size = 2*1024;//tx fifo is not used CURRENTLY ,but tx_fifio_size is used by sio
            break;
    #endif
        default:
            break;

    }

    d->callback = func;
    d->tx_lock =SCI_CreateMutex("DEBUG USERIAL MUTEX", SCI_INHERIT);
    if(d->tx_lock ==SCI_NULL )
    {
        USB_ERR_TRACE("[USB][UCOM]MUTEX tx_lock create failed\n");
        return UART_FAILURE;
    }

    fifo_start = SCI_ALLOC_APP(d->rx_dma_size + d->rx_fifo_size + d->tx_dma_size + d->tx_fifo_size + 32);
    if (fifo_start == SCI_NULL){
        USB_ERR_TRACE("[USB][UCOM]fifo buffer alloc failed\n");
        SCI_DeleteMutex(d->tx_lock);
        return UART_FAILURE;
    }
    fifo_end = fifo_start + d->rx_fifo_size + d->tx_fifo_size;
    dma_start =  (uint8 *)(((uint32)fifo_end + 31) / 32 * 32);

    osiFifoInit(&d->rx_fifo, fifo_start, d->rx_fifo_size);
    osiFifoInit(&d->tx_fifo, fifo_start + d->rx_fifo_size, d->tx_fifo_size);

    d->rx_dma_buf = dma_start;
    d->tx_dma_buf = dma_start + d->rx_dma_size;

    d->inited = TRUE;
    d->callback (EVENT_INIT_COMPLETE); //inform uplayer
    return UART_SUCCESS;
}

/*****************************************************************************/
//  Description:  com port close
//  Global resource dependence:
//  Author:       weihua.wang
//  Param :       uint32 port_num: usb virtual com number
//  Note  :
/*****************************************************************************/
PUBLIC uint32 UCOM_Close (uint32 port_num)
{
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d= prvUserialFindByName(port_num);
    USB_LOG_TRACE("UCOM_Close port_num = %d\n",port_num);

    if (TRUE != d->inited)
        return UART_PORT_RECLOSE_ERR;
    //STOP Transfer
    d->portstatus.us.usb_rx_enabled = false;
    d->portstatus.us.usb_tx_enabled = false;
    d->portstatus.us.usb_host_opened = false;
    //prvUserialStopLocked(d); //already stopped before
    d->blue_screen_mode = false;
    d->inited = false;
    
    if(d->rx_fifo.data){
        SCI_FREE(d->rx_fifo.data); //tx_fifo/rx_fifo/tx_dma/rx_dma buffer的首地址存放在rx_fifo.data
        d->rx_fifo.data = NULL;
        d->tx_fifo.data = NULL;
        d->rx_dma_buf = NULL;
        d->tx_dma_buf = NULL;
    }
    if(d->tx_lock)
        SCI_DeleteMutex(d->tx_lock);
    if(d->callback)
        d->callback (EVENT_SHUTDOWN_COMPLETE);

    return UART_SUCCESS;
}

//for compiler, used in mux_driver.c
PUBLIC uint32 UCOM_IsInPortIdle (uint32 port_num)
{
    return TRUE;
}


//not use by now
LOCAL BOOLEAN UCOM_PortIsOpenForRead (uint32 port_num)
{
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d= prvUserialFindByName(port_num);
    //USB_LOG_TRACE("UCOM_PortIsOpenForRead[%d] = %d\n",port_num,d->portstatus.us.usb_host_opened && d->portstatus.us.usb_rx_enabled);
    return d->portstatus.us.usb_host_opened && d->portstatus.us.usb_rx_enabled;

}

PUBLIC BOOLEAN UCOM_PortIsEnabled (uint32 port_num)
{
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d= prvUserialFindByName(port_num);
    //USB_LOG_TRACE("UCOM_PortIsOpenForRead[%d] = %d\n",port_num,d->portstatus.us.usb_host_opened && d->portstatus.us.usb_rx_enabled);
    return d->portstatus.us.usb_tx_enabled && d->portstatus.us.usb_rx_enabled;
}

PUBLIC BOOLEAN UCOM_PortIsOpenForWrite (uint32 port_num)
{
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d= prvUserialFindByName(port_num);
    //USB_LOG_TRACE("UCOM_PortIsOpenForWrite[%d] = %d\n",port_num,d->portstatus.us.usb_host_opened && d->portstatus.us.usb_tx_enabled);
    return d->portstatus.us.usb_host_opened && d->portstatus.us.usb_tx_enabled;

}


/*****************************************************************************/
//  Description:   This function read data from receiving buffer.It is called *
//                 when up layfer receives the event of "EVENT_DATA_TO_READ". *
//  Author:        weihua.wang                                                *
//  Param :        uint32 port_num : usb virtual com number index             *
//                 uint8* buffer   : des buffer header address                *
//                 uint32 length   : size to be read                          *
//  Return:        if success,return size read in effect
//  Note  :                                                                   *
/*****************************************************************************/
PUBLIC uint32 UCOM_ReadData (uint32 port_num, uint8 *buffer, uint32 length)
{
    int32 count;
    drvDebugUserialPort_t *d = prvUserialFindByName(port_num);

    count = prvUserialRead(d,buffer,length);
    //USB_LOG_TRACE("UCOM_ReadData count=%d port_num = %d\n",count,port_num);
    return count;
}

/*****************************************************************************/
//  Description:   This function record data which will be written to         *
//                 tx fifo and enable interrupt. The lengthof data should be  *
//                 not larger than FIFO_SIZE(64B).                            *
//  Author:        weihua.wang                                                *
//  Param :        uint32 port_num : usb virtual com number index             *
//                 uint8* buffer   : src buffer header address                *
//                 uint32 length   : size to be writed                        *
//  Return:        if success,return size writed in effect                    *
//  Note  :                                                                   *
/*****************************************************************************/
PUBLIC uint32 UCOM_WriteData (uint32 port_num, const uint8 *buffer, uint32 length)
{
    uint32 write_size = 0;
    drvDebugUserialPort_t *d;
    SCI_ASSERT(port_num < USB_MAX_NUM);
    //USB_LOG_TRACE("UCOM_WriteData length=%d port_num = %d\n",length,port_num);
    
    if (buffer == NULL || length == 0){
        USB_ERR_TRACE("[USB][UCOM]UCOM_WriteData[%d] failed, invalid param length=%d buffer = %p\n",port_num,length,buffer);
        return 0;
    }
    
    if(port_num == USB_COM4 && ppp_tx_ep_num != 0xff) //ppp
    {
        UdcStartTxDirectly(ppp_tx_ep_num,buffer,length); //transfer directly without schedule,to improve ppp speed
        return length;
    }

    d = prvUserialFindByName(port_num);
    
    write_size = prvUserialSendPacket(d,buffer,length);

    return write_size;
}

/*****************************************************************************/
//  Description:    call by sio to set usb in_endp enable/disable
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  BOOLEAN is_enable: TRUE/FALSE
//  Note:           this func is called after UPlayer callback
/*****************************************************************************/
PUBLIC void UCOM_TxEnable (uint32 port_num, BOOLEAN is_enable)
{
    uint32 endp;
    udc_t *udc;
    int result;
    drvDebugUserialPort_t *d;
    if (port_num >= USB_MAX_NUM)
        return;
    USB_LOG_TRACE("%s[%d] set_enable=%d\n",__FUNCTION__,port_num,is_enable);

    d = prvUserialFindByName(port_num);
    //udc = d->cdc->func->controller;
    
    d->portstatus.us.usb_tx_enabled = is_enable;
/*
    if( port_num == USB_COM4 || port_num == USB_COM0){  //for ppp_at and AT port
        d->portstatus.us.usb_tx_enabled = is_enable;
        //if(is_enable && d->callback)
            //d->callback(EVENT_WRITE_COMPLETE);

        return;
    }


    if(!is_enable){
        if (d->tx_ep){
         udcEpDequeueAll(udc, d->tx_ep);
         udcEpDisable(udc, d->tx_ep);
        }
    }
*/
}

/*****************************************************************************/
//  Description:    call by sio to set the usb out_endp enable/disable
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  BOOLEAN is_enable: TRUE/FALSE
//  Note:           this func is called after UPlayer callback
/*****************************************************************************/
PUBLIC void UCOM_RxEnable (uint32 port_num,BOOLEAN is_enable)
{
    drvDebugUserialPort_t *d;
    udc_t *udc ;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    USB_LOG_TRACE("%s[%d] set_enable=%d\n",__FUNCTION__,port_num,is_enable);

    d  = prvUserialFindByName(port_num);
    

    if(is_enable){
        if(!d->portstatus.us.usb_rx_enabled) 
            d->portstatus.us.usb_rx_enabled = (prvUserialEnableRx(d) == 0);
        prvUserialRxStartLocked(d);
    }else{
           udc = d->cdc->func->controller;
           d->portstatus.us.usb_rx_enabled = false;
            if (d->rx_ep)
            {
                udcEpDequeueAll(udc, d->rx_ep);
                udcEpDisable(udc, d->rx_ep);
            }
    }

}

/*****************************************************************************/
//  Description:    return the usb in endp fifo cnt
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//  Note:           here only return max fifo size for in endp
/*****************************************************************************/
PUBLIC uint32 UCOM_GetTxFIFOcnt(uint32 port_num)
{
    drvDebugUserialPort_t *d = prvUserialFindByName(port_num);

    return (d->tx_dma_buf == NULL) ? 0 : osiFifoSpace(&d->tx_fifo);
    //return 0;
}

PUBLIC uint32 UCOM_RxDataAvail(uint32 port_num)
{
    drvDebugUserialPort_t *d = prvUserialFindByName(port_num);
    return (d->rx_dma_buf == NULL) ?  0: osiFifoBytes(&d->rx_fifo);
}

PUBLIC uint32 UCOM_TxFifoIsEmpty(uint32 port_num)
{
    drvDebugUserialPort_t *d = prvUserialFindByName(port_num);
    return (d->tx_dma_buf == NULL) ? 0 : osiFifoIsEmpty(&d->tx_fifo);
    //return 0;

}


/*****************************************************************************/
//  Description:    get char from fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//  Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen,
/*****************************************************************************/
PUBLIC int32 UCOM_GetRxChar (uint32 port_num)
{
    int32 ch = -1;
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d = prvUserialFindByName(port_num);

    if(d->blue_screen_mode && UCOM_GetUcomType()== UCOM_CREATE_COM_NPI){ //校准模式下assert需要保持原UCOM端口进行dump

        prvUserialWaitDirectRxFinishBs(d);

        if (!osiFifoIsEmpty(&d->rx_fifo)){
            prvUserialRead(d,&ch,1);
        }
    }

     return ch;
}

/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  uint8 nchar: char to be sent
//  Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen,
/*****************************************************************************/
PUBLIC void UCOM_PutTxChar (uint32 port_num,uint8 nchar)
{
    drvDebugUserialPort_t *d;
    int len;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    //USB_LOG_TRACE("%s[%d]:0x%x\n",__FUNCTION__,port_num,nchar);
    d = prvUserialFindByName(port_num);
    if(d->blue_screen_mode == false){
        prvUserialBsEnter(d);   
        //VCOM_Open(VCOM_GetDevState() == VCOM_DEV_READY);
        #if VCOM1_SUPPORT
        //VCOM1_Open(VCOM1_GetDevState() == VCOM_DEV_READY);
        #endif
    }
    osiFifoPut(&d->tx_fifo, &nchar, 1);

    if (osiFifoBytes(&d->tx_fifo) >= d->tx_dma_size) {
        len = osiFifoGet(&d->tx_fifo, d->tx_dma_buf, d->tx_dma_size);
        if (len > 0) {
            len = prvUserialSendPacket(d,d->tx_dma_buf,len);
        }
    }

#if 1
     //prvUserialSendPacket(d,&nchar,1);
#else
if (port_num == DRV_NAME_USRL_COM3)
    VCOM1_PutTxChar (nchar);
else
    VCOM_PutTxChar (nchar);
#endif

}

/*****************************************************************************/
//  Description:    put chars to fifo in loop mode, it will do '0x7e' convert in func
//                  to reduce the time
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//                  uint8* data: data to be send
//                  int32 len  : data size to be sent
//  Note:           function is called when dump memory, to reduce the time of repeat call
/*****************************************************************************/
//not used by now
PUBLIC void UCOM_PutTxChars (uint32 port_num,uint8 *data, int32 len)
{
    drvDebugUserialPort_t *d;
    int size;
    uint32 index;
    uint8 val;
    uint8 val_1;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d = prvUserialFindByName(port_num);
    //USB_LOG_TRACE("%s[%d],len=%d\n",__FUNCTION__,port_num,len);
    if(d->blue_screen_mode == false)
        prvUserialBsEnter(d);

    //osiFifoPut(&d->tx_fifo, data, len);

    for (index = 0; index < len; ++index) {
        val = data[index];
        if (0x7E == val || 0x7D == val) {
            val_1 = 0x7D;
            osiFifoPut(&d->tx_fifo, &val_1, 1);
            val = val ^ 0x20;
            osiFifoPut(&d->tx_fifo, &val, 1);
        } else {
            osiFifoPut(&d->tx_fifo, &val, 1);
        }

        if (osiFifoBytes(&d->tx_fifo) >= d->tx_dma_size) {
            size = osiFifoGet(&d->tx_fifo, d->tx_dma_buf, d->tx_dma_size);
            if (size > 0) {
                prvUserialSendPacket(d,d->tx_dma_buf,size);
            }
        }
    }

    #if 0
    //prvUserialBsEnter(d);

    if (port_num == DRV_NAME_USRL_COM3)
        VCOM1_PutTxChars (data,len);
    else
        VCOM_PutTxChars (data,len);
    #endif

}

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number
//  Note:
/*****************************************************************************/
PUBLIC void UCOM_Flush (uint32 port_num)
{
    //uint8 *data;
    int32 size = 0;
    int len = 0;
    drvDebugUserialPort_t *d;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    d = prvUserialFindByName(port_num);
    USB_LOG_TRACE("%s[%d]\n",__FUNCTION__,port_num);

    if(!UCOM_TxFifoIsEmpty(port_num)){
       if(d->tx_dma_buf != NULL){
            size =osiFifoBytes(&d->tx_fifo);

        //data = (uint8 *)malloc(size);
            len =  osiFifoGet(&d->tx_fifo, d->tx_dma_buf, size);
            //prvUserialStartTxLocked(d, d->tx_dma_buf, size, false);
        prvUserialSendPacket(d,d->tx_dma_buf,len);
        }

    }

}

LOCAL const sio_operation_st ucom_operation =
{
    UCOM_Creat,
    UCOM_Close,
    UCOM_GetTxFIFOcnt,
    UCOM_ReadData,
    UCOM_WriteData,
    UCOM_GetRxChar,
    UCOM_PutTxChar,
    UCOM_TxEnable,
    UCOM_RxEnable,
    UCOM_Flush,
    UCOM_PutTxChars,
    UCOM_RxDataAvail,
};

PUBLIC const sio_operation_st *ucom_get_operation (void)
{
    return &ucom_operation;
}

uint8 my_rbuf[32];

LOCAL uint32 ucom_test_callback (uint32 event)
{
    uint32 num=0,read_len=0;
    uint32 phy_port;


    xSignalHeader signal;
    SCI_ASSERT (event < COM_MAX_EVENT);/*assert verified*/

    //phy_port = sio_port[COM_DEBUG2].phy_port;

    //here we just for uart
    switch (event)
    {
        case EVENT_DATA_TO_READ:
            {       
                read_len = UCOM_RxDataAvail(DRV_NAME_USRL_COM4);
                USB_LOG_TRACE("ready to read:read_len=%d\n",read_len);
                UCOM_ReadData(DRV_NAME_USRL_COM4,my_rbuf,read_len);
                UCOM_WriteData (DRV_NAME_USRL_COM4, my_rbuf, read_len);
                //USB_LOG_TRACE(my_rbuf);
            }
            break;
        case EVENT_WRITE_COMPLETE:
            {
                USB_LOG_TRACE("write complete\n");
            }
            break;
        case EVENT_INIT_COMPLETE:
            USB_LOG_TRACE("create  complete\n");
            break;
        case EVENT_SHUTDOWN_COMPLETE:
            USB_LOG_TRACE("shutdown  complete\n");
            break;
        default:
            break;

    }
    return read_len;
}

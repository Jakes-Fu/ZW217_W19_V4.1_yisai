/******************************************************************************
 ** File Name:    UCOM_main.c                                                 *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-11-8                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2005-11-8     weihua.wang     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_ucom_trc.h"
#include "os_api.h"
#include "udc_api.h"
#include "ucom_main.h"
#include "ucom_api.h"
#include "dal_chr.h"
#include "com_drv.h"
#include "sio.h"
#include "ref_outport.h"
#include "ref_param.h"
#include "tasks_id.h"
#include "ucom_drv.h"
#include "sci_log.h"
#include "vcom_usb.h"
#include "task_monitor.h"
#include "dal_power.h"
#include "usbservice_api.h"
#include "udc_api.h"
#include "os_param.h"
#include "gadget.h"
#include "ucom_config.h"
#include "usb_device.h"


#ifdef MUX_SUPPORT
#include "mux_all.h"
#include "mux_driver.h"
#endif
#ifdef USB_PROTOCOL_SUPPORT_USB20
#include "usb20_app_config.h"
#endif
#include "doidleonpage.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef USB_VCOM_AUTORUN_SUPPORT	
    extern PUBLIC int cdrom_bind (USB_GADGET_T *gadget);
    extern PUBLIC int cdrom_unbind (USB_GADGET_T *gadget);
    extern void CDROM_GetMaxLUN (uint16 req,uint16 wValue, uint16 wIndex ,uint16 wLen);
#endif


#ifdef WINUSB_SUPPORT

typedef enum {
    UBOT_CMD_PLUG_IN,
    UBOT_CMD_PLUG_OUT,
    UBOT_CMD_EXECUTE,
    UBOT_CMD_MAX
}UBOT_COMMAND_E;

#define UMS_CLASS_GET_MAX_LUN   0xFE
#define UMS_CLASS_RESET         0xFF

#define INTERFACE_THRID			0x03	
LOCAL uint32 g_usb_configuration = 1;
LOCAL uint8 ubot_task_execute_flag = 0;
LOCAL uint32 g_usb_interface = 1;

extern BLOCK_ID		ubot_main_id;
extern void UBOT_SetInterfaceNum(uint32 num);
extern void UBOT_GetMaxLUN (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen);

#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
//#define _UCOM_DEBUG

#define UCOM_INVALID_NUM    0xffff
#define UCOM_STACK_SIZE 0x1000
#define UCOM_QUEUE_NUM  0x0010

uint32 UCOM_interface[USB_MAX_NUM]={0xffffffff,0xffffffff, 0xffffffff, 0xffffffff};

/*----------------------------------------------------------------------------*
**                         Local Data                                        *
**---------------------------------------------------------------------------*/
LOCAL UCOM_PortTbl_T s_ucom_port_index[UCOM_MAX_COM_NUM] =
{
    { USB_COM0, 0x04,  0x83 },
    { USB_COM1, 0x02,  0x81 },
    { USB_COM2, 0xff, 0xff },
    { USB_COM3, 0xff, 0xff },
};

//for pc sysc,to save current COM_DATA/COM_DEBUG phy_port number
LOCAL uint16 s_com_data_phyport = UCOM_INVALID_NUM;/*lint -esym(551,s_com_data_phyport)*/
LOCAL uint16 s_com_debug_phyport = UCOM_INVALID_NUM;
LOCAL uint16 s_com_debug2_phyport = UCOM_INVALID_NUM;

//mainly used for usb device descriptor of one interface/two interface
LOCAL uint16 s_com_work_mode = UCOM_CREATE_COM_DEBUG;
//to distinguish whether in assert mode
LOCAL BOOLEAN s_ucom_isassert;
//mainly used for usb device descriptor of one interface used as modem or debug
LOCAL BOOLEAN b_ucom_modem_ubot_enable = FALSE;

static uint8 *s_ucom_alloc_buf = NULL;

/*--------------------------- Global Data -----------------------------------*/
UCOM_MANAGER_T      g_UCOM_manager ;
UCOM_FUNCTION_T     g_ucom_functions [UCOM_MAX_FUNC_NUM];
BLOCK_ID        ucom_main_id;

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    get cur usbvirtual com work mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           cft and at modem may use one com, when used for cft, usb send data through
//                  dma. bt at through usb irq
/*****************************************************************************/
LOCAL uint32 UCOM_PrintStatistics (uint32 param)
{
    UCOM_DEVICE_T *ucom;
    uint32 recv, send;
    uint8 index;
    for (index = 0; index < UCOM_MAX_COM_NUM; ++index)
    {
        ucom = &g_UCOM_manager.dev[index];
        if (ucom->data_statis != 0 && (ucom->data_statis_send != 0 || ucom->data_statis_recv != 0))
        {
            SCI_DisableIRQ();
            recv = ucom->data_statis_recv;
            send = ucom->data_statis_send;
            ucom->data_statis_recv = 0;
            ucom->data_statis_send = 0;
            SCI_RestoreIRQ();
            SCI_TraceLow("ucom %d data statis send %u, recv %u", index, send, recv);
        }
    }

    return 0;
}

static uint32 UcomSetBuf(UCOM_DEVICE_T *ucom, uint32 rxsize, uint32 txsize, uint8 *buf)
{
    osiFifoInit(&ucom->rx_fifo, buf, rxsize);
    osiFifoInit(&ucom->tx_fifo, buf + rxsize, txsize);
    return txsize + rxsize;
}

static uint32 UcomSetDmaBuf(UCOM_DEVICE_T *ucom, uint32 rxsize, uint32 txsize, uint8 *buf)
{
    ucom->rx_buf.usb_buf = buf;
    ucom->rx_buf.buf_size = rxsize;
    ucom->tx_buf.usb_buf = buf + rxsize;
    ucom->tx_buf.buf_size = txsize;

    if (rxsize == 0)
        ucom->rx_buf.usb_buf = NULL;
    if (txsize == 0)
        ucom->tx_buf.usb_buf = NULL;

    return txsize + rxsize;
}

static BOOLEAN UcomInitBuffer()
{
    uint8 *dma_buf, *buf_end, *alloc_buf;
    uint32 alloc_size=0, buf_off = 0, dma_off = 0;
    UCOM_DEVICE_T *ucom = &g_UCOM_manager.dev[0];
    ucomWorkMode_t wm = UCOM_GetWorkMode();

    if (wm == UCOM_WORK_WITH_UBOT) { // two interface
        uint32 index;
        const uint32 dma_size = 512;
        const uint32 tx_buf_size = 2048;
        const uint32 rx_buf_size = 1024;
        alloc_size = (dma_size * 4 + (tx_buf_size + rx_buf_size) * 2) + 32;
        alloc_buf = SCI_ALLOC_APP(alloc_size);
        if (alloc_buf == NULL)
            return FALSE;

        buf_end = alloc_buf + (tx_buf_size + rx_buf_size) * 2;
        dma_buf = (uint8 *)(((uint32)buf_end + 31) / 32 * 32);

        for (index = 0; index < 2; ++index) {
            buf_off += UcomSetBuf(ucom, rx_buf_size, tx_buf_size, alloc_buf + buf_off);
            dma_off += UcomSetDmaBuf(ucom, dma_size, dma_size, dma_buf + dma_off);
            ucom += 1;
        }
    }else if (wm == UCOM_WORK_DEBUG_DATA_UMS) { // three interface
        uint32 index;
        const uint32 at_dma_size = 512;
        const uint32 at_buf_size = 1024;
        const uint32 diag_dma_size = 1024;
        const uint32 diag_tx_buf_size = 4 * 1024;
        const uint32 diag_rx_buf_size = 1024;
        const uint32 dma_size = 512;
        const uint32 tx_buf_size = 2048;
        const uint32 rx_buf_size = 1024;
        alloc_size = (dma_size * 4 + (tx_buf_size + rx_buf_size) * 2) + 32;
        
        alloc_size = at_dma_size * 2 + at_buf_size * 2;
        alloc_size += diag_dma_size * 2 + diag_tx_buf_size + diag_rx_buf_size; 
        alloc_size += dma_size * 2 + tx_buf_size + rx_buf_size + 32;     
        alloc_buf = SCI_ALLOC_APP(alloc_size);
        

         if (alloc_buf == NULL)
             return FALSE;
 
        buf_end = alloc_buf + diag_tx_buf_size + diag_rx_buf_size+ at_buf_size * 2;
        buf_end += tx_buf_size + rx_buf_size;
        dma_buf = (uint8 *)(((uint32)buf_end + 31) / 32 * 32);

        //AT     
        buf_off += UcomSetBuf(ucom, at_buf_size, at_buf_size, alloc_buf + buf_off);
        dma_off += UcomSetDmaBuf(ucom, dma_size, dma_size, dma_buf + dma_off);
        
        //DIAG
        ucom += 1;
        buf_off += UcomSetBuf(ucom, diag_rx_buf_size, diag_tx_buf_size, alloc_buf + buf_off);
        dma_off += UcomSetDmaBuf(ucom, diag_dma_size, diag_dma_size, dma_buf + dma_off);

        //
        ucom += 1;
        buf_off += UcomSetBuf(ucom, rx_buf_size, tx_buf_size, alloc_buf + buf_off);
        dma_off += UcomSetDmaBuf(ucom, dma_size, dma_size, dma_buf + dma_off);
               
     } else if (wm == UCOM_WORK_DATA_DEBUG) { // two interface
        const uint32 at_dma_size = 512;
        const uint32 at_buf_size = 1024;
        const uint32 diag_dma_size = 1024;
        const uint32 diag_tx_buf_size = 4 * 1024;
        const uint32 diag_rx_buf_size = 1024;
        alloc_size = at_dma_size * 2 + at_buf_size * 2;
        alloc_size += diag_dma_size * 2 + diag_tx_buf_size + diag_rx_buf_size;
        alloc_size += 32;
        alloc_buf = SCI_ALLOC_APP(alloc_size);
        if (alloc_buf == NULL)
            return FALSE;

        buf_end = alloc_buf + at_buf_size * 2 + diag_tx_buf_size + diag_rx_buf_size;
        dma_buf = (uint8 *)(((uint32)buf_end + 31) / 32 * 32);

        // UCOM AT
        buf_off += UcomSetBuf(ucom, at_buf_size, at_buf_size, alloc_buf);
        dma_off += UcomSetDmaBuf(ucom, at_dma_size, at_dma_size, dma_buf + dma_off);

        // UCOM Diag
        ucom += 1;
        UcomSetBuf(ucom, diag_rx_buf_size, diag_tx_buf_size, alloc_buf + buf_off);
        UcomSetDmaBuf(ucom, diag_dma_size, diag_dma_size, dma_buf + dma_off);
    } else if (wm == UCOM_WORK_DEBUG_DATA_MODEM) { // four interface
        const uint32 at_dma_size = 512; // at
        const uint32 at_buf_size = 1024;
        const uint32 diag_tx_dma_size = 2048;  // diag
        const uint32 diag_rx_dma_size = 512;
        const uint32 diag_tx_size = 4 * 1024;
        const uint32 diag_rx_size = 512;
        const uint32 modem_dma_size = 2 * 1024;  // modem
        const uint32 modem_buf_size = 4 * 1024;
        const uint32 cp_tx_dma_size = 4 * 1024;    // cp trace
        const uint32 cp_tx_size = 8 * 1024;
        const uint32 cp_rx_size = 64;
        uint32 nondma_size = at_buf_size * 2 + diag_tx_size + diag_rx_size +
                             modem_buf_size * 2 + cp_tx_size + cp_rx_size;
        uint32 dma_size = (at_dma_size + modem_dma_size) * 2 + cp_tx_dma_size
                            + diag_tx_dma_size + diag_rx_dma_size;
        alloc_size += nondma_size + dma_size + 32;
        alloc_buf = SCI_ALLOC_APP(alloc_size);
        if (alloc_buf == NULL)
            return FALSE;

        buf_end = alloc_buf + nondma_size;
        dma_buf = (uint8 *)(((uint32)buf_end + 31) / 32 * 32);

        // UCOM AT
        ucom->data_statis = 1;
        ucom->data_statis_send = 0;
        ucom->data_statis_recv = 0;
        buf_off += UcomSetBuf(ucom, at_buf_size, at_buf_size, alloc_buf);
        dma_off += UcomSetDmaBuf(ucom, at_dma_size, at_dma_size, dma_buf);

        // UCOM Diag
        ucom += 1;
        buf_off += UcomSetBuf(ucom, diag_rx_size, diag_tx_size, alloc_buf + buf_off);
        dma_off += UcomSetDmaBuf(ucom, diag_rx_dma_size, diag_tx_dma_size, dma_buf + dma_off);

        // UCOM modem
        ucom += 1;
        buf_off += UcomSetBuf(ucom, modem_buf_size, modem_buf_size, alloc_buf + buf_off);
        dma_off += UcomSetDmaBuf(ucom, modem_dma_size, modem_dma_size, dma_buf + dma_off);

        // UCOM CP Trace
        ucom += 1;
        UcomSetBuf(ucom, cp_rx_size, cp_tx_size, alloc_buf + buf_off);
        UcomSetDmaBuf(ucom, 0, cp_tx_dma_size, dma_buf + dma_off);
    } else { // one interface
        const uint32 dma_size = 1024;
        const uint32 buf_size = 4*1024;
        alloc_size = 2 * (buf_size + dma_size) + 32;
        alloc_buf = SCI_ALLOC_APP(alloc_size);
        if (alloc_buf == NULL)
            return FALSE;

        buf_end = alloc_buf + 2 * buf_size;
        dma_buf = (uint8 *)(((uint32)buf_end + 31) / 32 * 32);
        UcomSetBuf(ucom, buf_size, buf_size, alloc_buf);
        UcomSetDmaBuf(ucom, dma_size, dma_size, dma_buf);
    }

    s_ucom_alloc_buf = alloc_buf;

    return TRUE;
}

/*****************************************************************************/
//  Description:    This is ucom component init funcion
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void UCOM_init(void)
{
    uint8 i;
    if (!UcomInitBuffer())
        return;
    SCI_MEMSET (&g_ucom_functions[0], 0 , (sizeof (UCOM_FUNCTION_T) *UCOM_MAX_FUNC_NUM));

    for (i=0; i<UCOM_MAX_FUNC_NUM; i++) {
        g_ucom_functions[i].func_ptr = DoNothing;
    }

    USB_RegCallback (USB_REQ_STANDARD,USB_REC_DEVICE,USB_REQUEST_GET_DESCRIPTOR, UCOM_GetDevDescriptor);
    USB_RegCallback (USB_REQ_STANDARD,USB_REC_DEVICE,USB_REQUEST_SET_CONFIGURATION, UCOM_SetConfigure);

    USB_RegCallback (USB_REQ_VENDOR, USB_REC_DEVICE, USB_REQUEST_CLEAR_FEATURE, UCOM_VendorCommand);
    USB_RegCallback (USB_REQ_VENDOR, USB_REC_DEVICE, USB_REQUEST_SET_FEATURE, UCOM_VendorCommand);
    USB_RegCallback (USB_REQ_VENDOR, USB_REC_DEVICE, 0xCC, UCOM_VendorCommand);

    USB_RegCallback (USB_REQ_CLASS, USB_REC_INTERFACE, 0x20, UCOM_ClassCommand);
    USB_RegCallback (USB_REQ_CLASS, USB_REC_INTERFACE, 0x21, UCOM_ClassCommand);
    USB_RegCallback (USB_REQ_CLASS, USB_REC_INTERFACE, 0x22, UCOM_ClassCommand);

#ifdef USB_VCOM_AUTORUN_SUPPORT
    USB_RegCallback (USB_REQ_CLASS,USB_REC_INTERFACE, 0xFE, CDROM_GetMaxLUN);
#endif

#ifdef   WINUSB_SUPPORT
	USB_RegCallback (USB_REQ_VENDOR,USB_REC_DEVICE,USB_REQUEST_MS_VENDOR,UCOM_VendorCommand);
	USB_RegCallback (USB_REQ_VENDOR,USB_REC_INTERFACE,USB_REQUEST_MS_VENDOR,UCOM_VendorCommand);
#endif

    USB_RegCallback (USB_REQ_RESERVED, USB_REC_DEVICE, USB_REQUEST_CLEAR_FEATURE, UCOM_VendorCommand);
    USB_RegCallback (USB_REQ_RESERVED, USB_REC_DEVICE, USB_REQUEST_SET_FEATURE, UCOM_VendorCommand);
	
    s_ucom_isassert = FALSE;
    DoIdle_RegisterCallback (UCOM_PrintStatistics);
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

#ifdef MUX_SUPPORT
    if (MUX_IsUsbDataEnable())
    {
        SCI_DisableIRQ();
        MUX_Close (COM_DATA);
        MUX_Create (COM_DATA, s_com_data_phyport,&m_dcb_data);
        SCI_RestoreIRQ();
    }
#else
    if ( (s_com_data_phyport != UCOM_INVALID_NUM)
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

    if ( (s_com_debug_phyport != UCOM_INVALID_NUM)
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
        SIO_Create (COM_DEBUG,s_com_debug_phyport,&m_dcb_data);                     /*lint !e718 */
        SCI_RestoreIRQ();
    }

	if ( (s_com_debug2_phyport != UCOM_INVALID_NUM)
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
    s_com_data_phyport  = UCOM_INVALID_NUM;
    s_com_debug_phyport = UCOM_INVALID_NUM;
    s_com_debug2_phyport = UCOM_INVALID_NUM;
    s_com_work_mode = UCOM_CREATE_COM_DEBUG;

    for (i = 0; i < UCOM_MAX_COM_NUM; ++i) {
        UCOM_DEVICE_T *udev = &g_UCOM_manager.dev[0];
        osiFifoReset(&udev->rx_fifo);
		osiFifoReset(&udev->tx_fifo);
        udev->rx_buf.usb_buf = NULL;
        udev->rx_buf.buf_size = 0;
        udev->tx_buf.usb_buf = NULL;
        udev->tx_buf.buf_size = 0;
        udev->data_statis = 0;
        udev->data_statis_send = 0;
        udev->data_statis_recv = 0;
    }

    SCI_FREE (s_ucom_alloc_buf);
    s_ucom_alloc_buf = NULL;
}

static void UCOM_TxContinue(uint8 port)
{
    UCOM_DEVICE_T *ucom = &g_UCOM_manager.dev[port];

    SCI_DisableIRQ();
    if (!osiFifoIsEmpty(&ucom->tx_fifo) && ucom->tx_state == UCOM_TX_STATE_DONE) {
        uint32 fifo_size = osiFifoBytes(&ucom->tx_fifo);
        if (fifo_size > ucom->tx_buf.buf_size)
            fifo_size = ucom->tx_buf.buf_size;
        osiFifoGet(&ucom->tx_fifo, ucom->tx_buf.usb_buf, fifo_size);
        ucom->tx_state = UCOM_TX_STATE_STARTED;
        UCOM_EPxSendData(port, (uint32 *)ucom->tx_buf.usb_buf, fifo_size);
    }
    SCI_RestoreIRQ();
}

/*****************************************************************************/
//  Description:    This is USLA's main body .
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void UCOM_main (uint32 argc, void *argv)
{
    xSignalHeader   signal;
    BLOCK_ID    tid = SCI_IdentifyThread();
    BOOLEAN is_ucom_main_task_exist = TRUE;

    while (is_ucom_main_task_exist)
    {
        signal = (xSignalHeader) SCI_GetSignal (tid);

        switch (signal->SignalCode)
        {
            case UCOM_CMD_PLUG_IN:
                UCOM_init();
                USB_SetFreq (USB_FREQ_SET_LVL_HIGH);
                break;

            case UCOM_CMD_PLUG_OUT:
                UCOM_exit();
                UDC_PowerSet (TRUE); //Self power;
                is_ucom_main_task_exist = FALSE;
                break;

            case UCOM_CMD_TX_CONTINUE:
                UCOM_TxContinue(*((uint8 *)signal + sizeof(*signal)));
                break;

            default :
                break;
        }

        SCI_FREE (signal);
        signal = NULL;
    }

    // Empty the signal queue.
    while (1) /*lint !e716*/
    {
        signal = (void*)SCI_PeekSignal(SCI_IdentifyThread());
        if (signal)
        {
            SCI_FREE(signal);
        }
        else
        {
            break;
        }
    }

    SCI_ThreadExit ();
}

/*****************************************************************************/
//  Description:    Function is called when ep1 transfer complete
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void UCOM_Send_Complete(uint8 device_id, uint8 *buffer, uint32 length)
{
    xSignalHeader signal;
    uint32 fifo_size;
    UCOM_DEVICE_T *ucom = &g_UCOM_manager.dev[device_id];
    if (ucom->tx_state != UCOM_TX_STATE_STARTED) {
        return;
    }

    if (ucom->data_statis != 0)
    {
        ucom->data_statis_send += length;
    }
    
    SCI_DisableIRQ();
    fifo_size = osiFifoBytes(&ucom->tx_fifo);
    if (fifo_size != 0) {
        signal = SCI_ALLOC_APP(sizeof(*signal) + 1);
        *((uint8 *)signal + sizeof(*signal)) = device_id;
        signal->SignalCode = UCOM_CMD_TX_CONTINUE;
        signal->SignalSize = sizeof(*signal) + 1;
        signal->Sender = SCI_IdentifyThread();
        ucom->tx_state = UCOM_TX_STATE_DONE;
        SCI_SendSignal(signal, ucom_main_id);
    } else {
        ucom->tx_state = UCOM_TX_STATE_IDLE;
    }
    SCI_RestoreIRQ();

    ucom->callback(EVENT_WRITE_COMPLETE);
}

/*****************************************************************************/
//  Description:    Function is called when ep2 transfer complete
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void UCOM_Receive_Complete (uint8 device_id, uint8 *buffer,uint32 length)
{
    UCOM_DEVICE_T *ucom = &g_UCOM_manager.dev[device_id];

    if (ucom->open_flag == UCOM_ALREADY_OPEN) {
        int len = osiFifoPut(&ucom->rx_fifo, buffer, length);
        if ((uint32)len != length)
        {
            SCI_TraceLow("ucom recv complete, rx fifo overflow, recv %u, save %d", length, len);
        }

        if (ucom->data_statis != 0)
        {
            ucom->data_statis_recv += len;
        }

        if (ucom->state == UCOM_DEV_READY) {
            UsbDevice_Start_receive(UCOM_interface[device_id],
                                    (uint32 *)ucom->rx_buf.usb_buf,
                                    ucom->rx_buf.buf_size);
        }

        if (ucom->state == UCOM_DEV_READY && !UCOM_IsInAssert() && ucom->callback) {
            ucom->callback(EVENT_DATA_TO_READ);
        }
    }
}

static uint32 IntfNumToPort(uint8 intfnum)
{
    uint32 index;
    for (index = 0; index < USB_MAX_NUM; ++index) {
        if (intfnum == UCOM_interface[index])
            return index + USB_COM0;
    }

    UCOM_ASSERT(0);
    return 0xffffffff; // avoid warning
}

LOCAL void UCOM_Transfer_Complete(uint8 event,uint8 *buffer,uint32 length)
{
    uint32 device_id = 0;
	uint8  interface_num = event>>4;
	switch(event&0x0F)
	{
		case EVENT_CTRL:
			break;
		case EVENT_SEND:
            device_id = IntfNumToPort(interface_num);
			UCOM_Send_Complete(device_id ,buffer,length);
			break;

		case EVENT_RECEIVE:
            device_id = IntfNumToPort(interface_num);
			UCOM_Receive_Complete(device_id ,buffer,length);
			break;

		default:
			break;
	}
}

/*****************************************************************************/
//  Description:    Function bind ucom component to the udc driver
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
void UCOM_SetWorkMode(USB_SERVICE_E service_type, uint8 calibmode)
{
    uint8 assert_mode  =  0;
    uint32 ucom_type = UCOM_CREATE_COM_DEBUG;
#if 0
    if (USB_SERVICE_UCOM == service_type) {
#if defined (USB_VCOM_AUTORUN_SUPPORT)
        ucom_type = UCOM_CREATE_COM_DATADISK; //pid = 0x3d02
#elif defined (USB_LOG)
        ucom_type = UCOM_CREATE_COM_DEBUGDATA; //pid = 3d00
#else
        ucom_type = UCOM_CREATE_COM_DATA; //pid = 4d00
#endif
    } else if (USB_SERVICE_LOG == service_type) {
#ifdef USB_LOG
        ucom_type = UCOM_CREATE_COM_DEBUGDATA;
#else
        ucom_type = UCOM_CREATE_COM_DATA;
#endif
    } 
#endif
    if ( (USB_SERVICE_UCOM == service_type) ||  (USB_SERVICE_LOG == service_type))
    {
        ucom_type = UCOM_CREATE_COM_DEBUGDATA_MODEM; // pid = 0x0006
    }
#ifdef WINUSB_SUPPORT
    else if (USB_SERVICE_UCOM_UMS == service_type)
    {
        ucom_type = UCOM_CREATE_COM_DEBUGDATA_UMS;
    }
#endif
    else 
    {
    	return;
    }    

    if (calibmode) {
        SIO_CONTROL_S m_dcb_data;
        m_dcb_data.flow_control = 0;
        m_dcb_data.baud_rate = SIO_GetBaudRate(COM_DEBUG);
        SCI_DisableIRQ();
        SIO_Close(COM_DEBUG);
        SIO_Create(COM_DEBUG, VIR_COM0, &m_dcb_data);
        SCI_RestoreIRQ();
    } else {
        OSPARAM_GetAssertMode(&assert_mode);
        if (SCI_ASSERT_DEBUG_MODE == assert_mode) {
            switch (ucom_type) {
            case UCOM_CREATE_COM_DEBUG:
            case UCOM_CREATE_COM_DATADISK:
            case UCOM_CREATE_COM_DEBUGDATA_UMS:
			case UCOM_CREATE_COM_DEBUGDATA_MODEM:
                UCOM_CreatForCOM(ucom_type);
                break;

            default:
                UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUGDATA);
                break;
            }
        } else if (SCI_ASSERT_RELEASE_MODE == assert_mode) {
            if (UCOM_CREATE_COM_DATADISK == ucom_type) {
                UCOM_CreatForCOM(UCOM_CREATE_COM_DATADISK);
            } else if (UCOM_CREATE_COM_DEBUGDATA_MODEM == ucom_type) {
                UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUGDATA_MODEM);
            }
#ifdef WINUSB_SUPPORT
            else if (UCOM_CREATE_COM_DEBUGDATA_UMS == ucom_type)
            {
                UCOM_CreatForCOM (UCOM_CREATE_COM_DEBUGDATA_UMS);
            }
#endif 
			else {
                UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUGDATA);
            }
        }
    }
}

static int ucom_bind (USB_GADGET_T *gadget)
{
    xSignalHeader   signal;
    uint32 call_back = (uint32)UCOM_Transfer_Complete;
    uint32 index, ucom_used;
    
#ifdef WINUSB_SUPPORT	
	UCOM_SetStrInfo();	
#endif

    if (s_com_work_mode == UCOM_CREATE_COM_DATADISK) {
 #ifdef USB_VCOM_AUTORUN_SUPPORT
        cdrom_bind(gadget);
 #endif
        ucom_used = 1;
    } else if (s_com_work_mode == UCOM_CREATE_COM_DEBUGDATA_MODEM) {
        ucom_used = 4;
    }
#ifdef WINUSB_SUPPORT
    else if (s_com_work_mode == UCOM_CREATE_COM_DEBUGDATA_UMS)  /*AT+DIAG+UDISK*/
    {
#ifdef UCOM_UMS_SUPPORT
        ubot_bind(gadget);
#endif
        ucom_used = 2;
    } 
#endif   
    else {
        ucom_used = 2;
    }

    for (index = 0; index < ucom_used; ++index) {
        UCOM_interface[index] = UsbDevice_OpenInterface(USB_PROTOCOL_UCOM|(USB_COM0 + index), call_back, index);
    }

    ucom_main_id = SCI_CreateThread ("T_UCOM_MAIN_HANDLER",
                                     "Q_UCOM",
                                     UCOM_main,
                                     0,
                                     0,
                                     UCOM_STACK_SIZE,
                                     UCOM_QUEUE_NUM,
                                     PRI_UCOM_MAIN,
                                     SCI_PREEMPT,
                                     SCI_AUTO_START);

#ifdef CONFIG_USB_GADGET_DUALSPEED
    gadget->speed       = USB_SPEED_HIGH;
#else
    gadget->speed       = USB_SPEED_FULL;
#endif

    signal = SCI_ALLOC_APP (sizeof (*signal));
    signal->SignalCode = UCOM_CMD_PLUG_IN;
    signal->SignalSize = sizeof (*signal);
    signal->Sender = SCI_IdentifyThread();
    SCI_SendSignal (signal, ucom_main_id);

#ifdef USB_PROTOCOL_SUPPORT_USB20
    USB_SetService (USB_APP_UCOM,NULL);
#endif
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    Function unbind ucom component to the udc driver
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
static void ucom_unbind (USB_GADGET_T *gadget)
{
    xSignalHeader signal;
    uint32 index;
#ifdef USB_VCOM_AUTORUN_SUPPORT
    if(s_com_work_mode== UCOM_CREATE_COM_DATADISK) {
        cdrom_unbind(gadget);
    }
#endif

#ifdef WINUSB_SUPPORT
    if (s_com_work_mode == UCOM_CREATE_COM_DEBUGDATA_UMS)
    {
        ubot_task_execute_flag = 0;
        ubot_unbind(gadget);
    }
#endif

    signal = SCI_ALLOC_APP (sizeof (*signal));
    signal->SignalCode = UCOM_CMD_PLUG_OUT;
    signal->SignalSize = sizeof (*signal);
    signal->Sender = SCI_IdentifyThread();
    SCI_SendSignal (signal, ucom_main_id);
	
#ifdef WINUSB_SUPPORT
    if(REFPARAM_GetUsbServicesFlag() == 0)
        UCOM_DevStop();    
#endif

    for (index = 0; index < USB_MAX_NUM; ++index) {
        UCOM_interface[index] = 0xffffffff;
    }

#ifdef USB_PROTOCOL_SUPPORT_USB20
    USB_SetService (USB_APP_NONE,NULL);
#endif
}
/*****************************************************************************/
//  Description:    Null Function
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
static void ucom_disconnect (USB_GADGET_T *gadget) {}

/*****************************************************************************/
//  Description:    EP0 setup Function
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
static int ucom_setup (USB_GADGET_T *gadget,const USB_CTRLREQUEST_T *ctrl)
{
    USB_FUNCTION fun_ptr = NULL;
#ifdef UCOM_UMS_SUPPORT    
    xSignalHeader	signalout; 
#endif

    
    USB_HWORD_VALUE_U value;
    uint16 wLen = ctrl->wLength;
    uint16 length = 0;
    char *pData = 0;
    
#ifdef WINUSB_SUPPORT
	if(s_com_work_mode == UCOM_CREATE_COM_DEBUGDATA_UMS)
		UBOT_SetInterfaceNum(INTERFACE_THRID);
#endif
    if ( (ctrl->bRequestType&USB_TYPE_MASK) == USB_TYPE_RESERVED)
    {
        return SCI_SUCCESS;
    }

    fun_ptr = USB_FindCallback ( ( (ctrl->bRequestType&USB_TYPE_MASK) >>5),
                                 (ctrl->bRequestType & USB_RECIP_MASK),
                                 (ctrl->bRequest));

    if (NULL != fun_ptr)
    {
        fun_ptr ( ( (ctrl->bRequestType) | (ctrl->bRequest<<8)),
                  (ctrl->wValue),
                  (ctrl->wIndex),
                  (ctrl->wLength));
    }

#ifdef WINUSB_SUPPORT
    if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS) //class
    {
//  add for WINUSB+MSC mode begin   
        switch (ctrl->bRequest)
        {
            case UMS_CLASS_GET_MAX_LUN:
                //              SCI_TRACE_LOW("\nUBOT_CLASS_GET_MAX_LUN\n");
                UBOT_GetMaxLUN ( (ctrl->bRequestType&USB_DIR_IN),\
                                 (ctrl->wValue),\
                                 (ctrl->wIndex),\
                                 (ctrl->wLength));

            	  if(0 == ubot_task_execute_flag)
            	  {
            	         ubot_task_execute_flag = 1;
                            /*Start process masstorage .*/
            		  signalout = SCI_ALLOC_APP(sizeof(*signalout));
            		  signalout->SignalCode = UBOT_CMD_EXECUTE;
            		  signalout->SignalSize = sizeof(*signalout);
            		  signalout->Sender = SCI_IdentifyThread();
            		  SCI_SendSignal(signalout, ubot_main_id);	
            	  }
		  
                break;
            case UMS_CLASS_RESET:
                //              SCI_TRACE_LOW("\nUBOT_CLASS_RESET\n");
                UBOT_HostReset ( (ctrl->bRequestType&USB_DIR_IN),\
                                 (ctrl->wValue),\
                                 (ctrl->wIndex),\
                                 (ctrl->wLength));
                break;
            default :
                USB_ErrorHandle (USB_ERR_INVALID_CLASSCMD);
                break;
        }
//  add for WINUSB+MSC mode begin          
    }
    else    //standard 
    {
        switch (ctrl->bRequestType & USB_RECIP_MASK)
        {
//	add for MSC TOOL TEST begin         
            case USB_RECIP_DEVICE:
                switch (ctrl->bRequest)
                {
                    case USB_REQUEST_GET_DESCRIPTOR:
                        value.wValue = ctrl->wValue;
                        if (value.bmDescriptor.type == USB_STRING_DESCRIPTOR_TYPE)
                        {
                            switch (value.bmDescriptor.index)
                            {
                                // add for MSC TOOL TEST begin					
                    			case USB_STR_INDEX_CONFIGURATION:
                    			   if (wLen > sizeof(USB_STRING_CONFIGURATION_DESCRIPTOR_T))
                                   {
                	                    length  = sizeof(USB_STRING_CONFIGURATION_DESCRIPTOR_T);        
                	               }
                                   else
                                   {
                                    	length  = wLen;
                                   }
                    				
                    		  	   pData   = (uint8 *)Ubot_Get_String_Index_Config();
                                   UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
                    		  	break;	
                    		  
                                case USB_STR_INDEX_INTERFACE:
                    				
                    				if (wLen > sizeof(USB_STRING_INTERFACE_DESCRIPTOR_T))
                                    {
                    			        length  = sizeof(USB_STRING_INTERFACE_DESCRIPTOR_T);        
                    			    }
                    			    else
                                    {
                				        length  = wLen;
                				    }
                    					
                    				pData   = (uint8 *)Ubot_Get_String_Interface_Desc();
                                    UsbDevice_Ctrl_Transfer((uint32 *)pData,length);
                    		  	break;		  
                                default:
                                break;    
                            }
                        }
                }
                break;
                
            case USB_RECIP_INTERFACE:               
			  switch(ctrl->bRequest)
			  {
			     case USB_REQUEST_SET_INTERFACE: 
	
			           g_usb_interface = ctrl->wValue;
				        // EPI0_config (0, 1, FALSE, NULL);
				       break; 
				 case USB_REQUEST_GET_INTERFACE:
					  // EPI0_config (ctrl->wLength, 1, TRUE, &g_usb_interface);
					   UsbDevice_Ctrl_Transfer((uint32 *)&g_usb_configuration,ctrl->wLength);
					   break;
				 default:
				       break;
			  }           
            break;
            default:
            break;    
        }

    }
    
    if(USB_REQ_SET_CONFIGURATION == ctrl->bRequest)
    {
        if((REFPARAM_GetUsbServicesFlag() == 0) && (USB_GetCaliMode() == 0))
             UCOM_DevReady();
    }
       
#endif
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    Ucom suspend Function
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
static void ucom_suspend (USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:    Ucom resume Function
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
static void ucom_resume (USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL const USB_GADGET_DRIVER_T ucom_driver =
{
    "UCOM",
#ifdef CONFIG_USB_GADGET_DUALSPEED
    USB_SPEED_HIGH,
#else
    USB_SPEED_FULL,
#endif
    ucom_bind,
    ucom_unbind,
    ucom_setup,
    ucom_disconnect,
    ucom_suspend,
    ucom_resume,
    UCOM_GetCOnfigDescriptor
};

const USB_GADGET_DRIVER_T *UCOM_GetHandler (void)
{
    return &ucom_driver;
}

/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:          used for auto_download
/******************************************************************************/
static void UCOM_SetEndpointMap (uint8 work_mode)
{
    UCOM_PortTbl_T *table = &s_ucom_port_index[0];

    switch(work_mode)
    {
       case UCOM_CREATE_COM_DATADISK:
            table[0].rx_endp = 0x04;
            table[0].tx_endp = 0x83;
            break;

        case UCOM_CREATE_COM_DATA:
            table[0].rx_endp = 0x02;
            table[0].tx_endp = 0x81;
            break;
#ifdef WINUSB_SUPPORT
        case UCOM_CREATE_COM_DEBUGDATA_UMS:
#endif
        case UCOM_CREATE_COM_DEBUGDATA:    /*lint !e616  !e825*/
            table[0].rx_endp = 0x02;
            table[0].tx_endp = 0x81;
            table[1].rx_endp = 0x04;
            table[1].tx_endp = 0x83;
            break;

        case UCOM_CREATE_COM_DEBUGDATA_MODEM:
            table[0].rx_endp = 0x02;
            table[0].tx_endp = 0x81;
            table[1].rx_endp = 0x04;
            table[1].tx_endp = 0x83;
            table[2].rx_endp = 0x01;
            table[2].tx_endp = 0x82;
            table[3].rx_endp = 0x03;
            table[3].tx_endp = 0x84;
            break;

        default:
            break;
    }
}

PUBLIC void UCOM_CreatForCOM (uint32 type)
{
    SIO_CONTROL_S m_dcb_data={0,0,0,0,0};
    
   
    UCOM_SetEndpointMap(type);
    
    switch (type)
    {
        case UCOM_CREATE_COM_DATADISK:
            b_ucom_modem_ubot_enable =  TRUE;
            s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);
            s_com_debug_phyport = UCOM_INVALID_NUM;

            //Change COM_DATA from uart to usb virutal com
            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);

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
                //save last phyport number of COM_DATA for the purpose of resotre
                s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);
                s_com_debug_phyport = UCOM_INVALID_NUM;

                //Change COM_DATA from uart to usb virutal com
                m_dcb_data.flow_control = 0;
                m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);

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
                //save last phyport number of COM_DEBUG for the purpose of resotre
                s_com_debug_phyport = SIO_GetPhyPortNo (COM_DEBUG);
                s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);
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
                UCOM_SetEndpointMap(type);
                SIO_Create (COM_DEBUG,VIR_COM1,&m_dcb_data);
            }
            break;

        case UCOM_CREATE_COM_DEBUGDATA_MODEM:
            s_com_debug_phyport = SIO_GetPhyPortNo(COM_DEBUG);
            s_com_data_phyport = SIO_GetPhyPortNo(COM_DATA);			
            s_com_debug2_phyport = SIO_GetPhyPortNo(COM_DEBUG2);
#ifdef MUX_SUPPORT
            MUX_Close(COM_DATA);
            MUX_Create(COM_DATA, VIR_COM0,&m_dcb_data);
#else
#ifdef PPP_USB

	     m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DATA2);
            SIO_Close(COM_DATA2);
            SIO_Create(COM_DATA2, VIR_COM3, &m_dcb_data);
#endif

            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DATA);
            SIO_Close(COM_DATA);
            SIO_Create(COM_DATA, VIR_COM0, &m_dcb_data);
#endif
		
            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG);

            SIO_Close(COM_DEBUG);
            SIO_Create(COM_DEBUG,VIR_COM1,&m_dcb_data);

            m_dcb_data.flow_control = 0;
            m_dcb_data.baud_rate = SIO_GetBaudRate (COM_DEBUG2);

            SIO_Close(COM_DEBUG2);
            SIO_Create(COM_DEBUG2,VIR_COM4,&m_dcb_data);


            //  TODO:  modem
            break;
#ifdef WINUSB_SUPPORT
        case UCOM_CREATE_COM_DEBUGDATA_UMS:
			{
                //save last phyport number of COM_DEBUG for the purpose of resotre
                s_com_debug_phyport = SIO_GetPhyPortNo (COM_DEBUG);
                s_com_data_phyport  = SIO_GetPhyPortNo (COM_DATA);

                //Change COM_DEBUG from uart to usb virutal com

                //Change COM_DATA from uart to usb virutal com
#ifdef MUX_SUPPORT
                MUX_Close (COM_DATA);
                MUX_Create (COM_DATA, VIR_COM0,&m_dcb_data);
#else
                /*important, mux and sio share same port, so we should disable sio port when using mux func
                   otherwise we will misoperate com_data and uart0 to error state
                */
                m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DATA);
                SIO_Close (COM_DATA);
                SIO_Create (COM_DATA,VIR_COM0,&m_dcb_data);
#endif
                m_dcb_data.flow_control = 0;
                m_dcb_data.baud_rate    = SIO_GetBaudRate (COM_DEBUG);

                SIO_Close (COM_DEBUG);	                
		        SIO_Create (COM_DEBUG,VIR_COM1,&m_dcb_data);
	
            }
			break;
#endif 	
        default:
            break;
    }

     s_com_work_mode = type;
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
    UCOM_DEVICE_T *dev_ptr = NULL;

    UCOM_ASSERT (func != NULL); /*assert verified*/
    UCOM_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    dev_ptr = UCOM_GetDevfromPort (port_num);

    if (UCOM_ALREADY_OPEN == dev_ptr->open_flag) {
        return UART_PORT_REOPEN_ERR;
    }

    //buffer init
    dev_ptr->state        = UCOM_DEV_NO_PRESENT;
    dev_ptr->callback     = func;  //register callback
    dev_ptr->open_flag    = UCOM_ALREADY_OPEN;

    dev_ptr->callback (EVENT_INIT_COMPLETE); //inform uplayer

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
    UCOM_DEVICE_T *dev_ptr = NULL;

    UCOM_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    //get dev ptr
    dev_ptr = UCOM_GetDevfromPort (port_num);

    if (UCOM_ALREADY_OPEN != dev_ptr->open_flag)
        return UART_PORT_RECLOSE_ERR;

    dev_ptr->state = UCOM_DEV_NO_PRESENT;
    dev_ptr->open_flag    = 0;
    dev_ptr->callback (EVENT_SHUTDOWN_COMPLETE);
    dev_ptr->data_statis_send = 0;
    dev_ptr->data_statis_recv = 0;

    // here, setting the value maybe hold a candle to the sun
    s_ucom_isassert = FALSE;

    return UART_SUCCESS;
}

LOCAL BOOLEAN UCOM_PortIsOpen (uint32 port_num)
{
    UCOM_DEVICE_T *dev_ptr = NULL;

    UCOM_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    // get dev ptr
    dev_ptr = UCOM_GetDevfromPort (port_num);
    return (dev_ptr->state!=UCOM_DEV_NO_PRESENT);
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
    uint32 count;
    UCOM_DEVICE_T *ucom = NULL;
    SCI_ASSERT(port_num < USB_MAX_NUM);
    if (buffer == NULL || length == 0)
        return 0;

    if (FALSE == UDC_GetConnect())
        return 0;

    ucom = UCOM_GetDevfromPort(port_num);
    if (UCOM_ALREADY_OPEN != ucom->open_flag)
        return UART_FAILURE;

    if (ucom->state != UCOM_DEV_READY)
        return 0;

    count = osiFifoBytes(&ucom->rx_fifo);
    if (length > count)
        length = count;

    count = osiFifoGet(&ucom->rx_fifo, buffer, length);
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
    UCOM_DEVICE_T *ucom = NULL;
    SCI_ASSERT(port_num < USB_MAX_NUM);
    if (buffer == NULL || length == 0)
        return 0;

    if (FALSE == UDC_GetConnect())
        return 0;

    ucom = UCOM_GetDevfromPort(port_num);
    if (UCOM_ALREADY_OPEN != ucom->open_flag)
        return UART_FAILURE;

    if (ucom->state != UCOM_DEV_READY)
        return 0;

    SCI_DisableIRQ();

#ifdef MODEM_LOG_USB_ZERO_COPY
    if (USB_COM3 == port_num)
    {
       if (ucom->tx_state == UCOM_TX_STATE_IDLE)
        {
            ucom->tx_state = UCOM_TX_STATE_STARTED;
            UCOM_EPxSendData(USB_COM3, (uint32 *)buffer, length);
	     SCI_RestoreIRQ();
	    return length;
       }
	 else
	 {
	         SCI_RestoreIRQ();
	    	return 0;

	 }
        
    }
#endif

#if 0
    if (ucom->tx_state == UCOM_TX_STATE_IDLE) {
        uint32 len = length;
        if (len > ucom->tx_buf.buf_size)
            len = ucom->tx_buf.buf_size;

        memcpy(ucom->tx_buf.usb_buf, buffer, len);
        ucom->tx_state = UCOM_TX_STATE_STARTED;
        UCOM_EPxSendData(port_num, (uint32 *)ucom->tx_buf.usb_buf, len);

        write_size += len;
        buffer += len;
        length -= len;
    }
#else
    if (ucom->tx_state == UCOM_TX_STATE_IDLE) {
        uint32 len = length;

        ucom->tx_state = UCOM_TX_STATE_STARTED;
        UCOM_EPxSendData(port_num, (uint32 *)buffer, len);
        write_size += len;
        buffer += len;
        length -= len;
    }
#endif
    if (length != 0) {
        write_size += osiFifoPut(&ucom->tx_fifo, buffer, length);
    }

    SCI_RestoreIRQ();
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
    UCOM_DEVICE_T *dev_ptr  = NULL;
    uint32 endp;

    if (port_num >= USB_MAX_NUM)
        return;

    dev_ptr = UCOM_GetDevfromPort (port_num);

    // if current usb disconnect, return directly
    if (FALSE == UDC_GetConnect())
        return;

    // if current virtual com is not ready, then do null
    if (dev_ptr->state != UCOM_DEV_READY)
        return;

    // in assert mode,no interrupt occur,return directly
    if (UCOM_IsInAssert())
        return;

    if (!is_enable)
        return;

    if (!osiFifoIsFull(&dev_ptr->tx_fifo) && dev_ptr->callback) {
        dev_ptr->callback(EVENT_WRITE_COMPLETE);
    }
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
    UCOM_DEVICE_T *dev_ptr  = NULL;

    UCOM_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    if (port_num >= USB_MAX_NUM)
        return;

    // if current usb disconnect, return directly
    if (FALSE == UDC_GetConnect())
        return;

    dev_ptr = UCOM_GetDevfromPort (port_num);

    // if current virtual com is not ready, then do null
    if (dev_ptr->state != UCOM_DEV_READY)
        return;

    if (is_enable) {
        if (!osiFifoIsEmpty(&dev_ptr->rx_fifo) && dev_ptr->callback) {
            dev_ptr->callback(EVENT_DATA_TO_READ);
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
    UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(port_num);
    return ucom ? osiFifoSpace(&ucom->tx_fifo) : 0;
}

PUBLIC uint32 UCOM_RxDataAvail(uint32 port_num)
{
    UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(port_num);
    return ucom ? osiFifoBytes(&ucom->rx_fifo) : 0;
}

PUBLIC uint32 UCOM_TxFifoIsEmpty(uint32 port_num)
{
    UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(port_num);
    return ucom ? osiFifoIsEmpty(&ucom->tx_fifo) : 0;
}

PUBLIC uint32 UCOM_TxIsIdle(uint32 port_num)
{
    UCOM_DEVICE_T *ucom = UCOM_GetDevfromPort(port_num);
    if (ucom->tx_state == UCOM_TX_STATE_IDLE)
        return 1;
    else
        return 0;
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
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    if (s_ucom_isassert)
    {
        return VCOM_GetChar();
    }

    return -1;
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
    UCOM_DEVICE_T *dev_ptr = NULL;
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/
    dev_ptr = UCOM_GetDevfromPort (port_num);

    if (!s_ucom_isassert)
    {
        UCOM_DEVICE_T *ucom_diag = &g_UCOM_manager.dev[1];
        UCOM_DEVICE_T *ucom_cp = &g_UCOM_manager.dev[3];
        s_ucom_isassert = TRUE;
        VCOM_Open(ucom_diag->state == UCOM_DEV_READY);
        VCOM1_Open(ucom_cp->state == UCOM_DEV_READY);
        //_UCOM_AssertInit(port_num,FALSE);
    }

    //if virtual is not ready,then exit with doing nothing
    if (dev_ptr->state != UCOM_DEV_READY)
    {
        return;
    }

    if (port_num == USB_COM3)
        VCOM1_PutTxChar (nchar);
    else
        VCOM_PutTxChar (nchar);
    return;
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
PUBLIC void UCOM_PutTxChars (uint32 port_num,uint8 *data, int32 len)
{
    /*ignore the check for input param to reduce time cost*/
    VCOM_PutTxChars (data, len);
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
    SCI_ASSERT (port_num < USB_MAX_NUM); /*assert verified*/

    if (port_num >= USB_MAX_NUM)
    {
        return;
    }

    if (s_ucom_isassert)
    {
        if (port_num == USB_COM3)
            VCOM1_Flush();
        else
            VCOM_Flush();
        return;
    }
}

/*****************************************************************************/
//  Description:    get dev point index from endp
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           called only in ucom project(internal)
/*****************************************************************************/
PUBLIC UCOM_DEVICE_T *UCOM_getDevfromEndp (uint8 ep_addr)
{
    uint8 num = ep_addr & 0x7f;
    uint8 i;

    SCI_ASSERT(num >= 1 && num <= USB_EPx_NUMBER);

    for (i = 0; i < UCOM_MAX_COM_NUM; ++i) {
        UCOM_PortTbl_T *c = &s_ucom_port_index[i];
        if (c->rx_endp == ep_addr || c->tx_endp == ep_addr) {
            return &g_UCOM_manager.dev[i];
        }
    }

    return NULL;
}

/*****************************************************************************/
//  Description:    get devptr according to port_num
//  Global resource dependence:
//  Author:         weihua.wang
//  param :         uint8 port_num: physical usb protnum
//  Note:
/*****************************************************************************/
PUBLIC UCOM_DEVICE_T *UCOM_GetDevfromPort (uint8 port_num)
{
    uint8 i;
    uint8 index = 0;

    SCI_ASSERT(port_num < USB_MAX_NUM); /*assert verified*/

    for (i = 0; i< UCOM_MAX_COM_NUM; i++) {
        if (s_ucom_port_index[i].port_num == port_num)
            return &g_UCOM_manager.dev[i];
    }

    return NULL;
}

/*****************************************************************************/
//  Description:    get assert mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UCOM_IsInAssert (void)
{
    return s_ucom_isassert;
}

/*****************************************************************************/
//  Description:   get vcom mode as modem + ubot, to judge the device and
//                  configure descriptor type for one interface
//  Global resource dependence:
//  Author:      xinwei.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN UCOM_GetModemUbotEnable (void)
{
    return b_ucom_modem_ubot_enable;
}

/*****************************************************************************/
//  Description:   set vcom mode as modem + ubot, to judge the device and
//                  configure descriptor type for one interface
//  Global resource dependence:
//  Author:         xinwei.zhang
//  Note:
/*****************************************************************************/
PUBLIC void UCOM_SetModemUbotEnable (BOOLEAN b_enable)
{
    b_ucom_modem_ubot_enable = b_enable;
}

PUBLIC ucomWorkMode_t UCOM_GetWorkMode()
{
    if (AutoDL_IsDloaderMode()) {
        return UCOM_WORK_AUTO_DWLD;
    } if (UCOM_GetModemUbotEnable()) {
        return UCOM_WORK_WITH_UBOT;
    } else {
        switch (s_com_work_mode) {
        case UCOM_CREATE_COM_DEBUGDATA:
            return UCOM_WORK_DATA_DEBUG;

        case UCOM_CREATE_COM_DEBUGDATA_MODEM:
            return UCOM_WORK_DEBUG_DATA_MODEM;

        case UCOM_CREATE_COM_DATA:
            return UCOM_WORK_COM_DATA;
#ifdef WINUSB_SUPPORT	           
        case UCOM_CREATE_COM_DEBUGDATA_UMS:
             return UCOM_WORK_DEBUG_DATA_UMS;
#endif
        case UCOM_CREATE_COM_DEBUG:
        default:
            return UCOM_WORK_COM_DEBUG;
        }
    }
}

/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:          used for auto_download
/******************************************************************************/
PUBLIC uint32 UCOM_SetDloaderEndpointMap (void)
{
    s_ucom_port_index[0].rx_endp = 0x02;
    s_ucom_port_index[0].tx_endp = 0x83;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description:
// Dependence:
// Author:        Haifeng.Yang
// Note:          used for auto_download
/******************************************************************************/
#ifdef USB_PROTOCOL_SUPPORT_USB20
PUBLIC uint32 UCOM_IsInPortIdle (uint32 port_num)
{
    return TRUE;
}
#endif

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

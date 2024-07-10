/******************************************************************************
 ** File Name:    usb_drv_e.c                                                 *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
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
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#if defined(PLATFORM_SC6800H)
#include "v0/usb_reg_v0.h"
//#include "../int/v0/int_reg_v0.h"
#endif
#include "clock_drvapi.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "usb_drv.h"
#include "dma_drv_internal.h"
#include "ldo_drvapi.h"
#include "ref_outport.h"
#include "tasK_monitor.h"

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
#if defined(PLATFORM_SC6800H)||defined(PLATFORM_SC8800G)||defined(PLATFORM_SC6600L)
#define USB_LDO_ENABLE                  (0x1)
#define USB_LDO_DISABLE                 (0x2)
#define USB_INTERPACKET_TIMEOUT_SET
#endif

#define USB_12M_CLK 0x00
#define USB_48M_CLK 0x01
#define USB_MAX_CLK 0x02
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
// USB device handle
PUBLIC DEVICE_HANDLE g_dev_usb = SCI_NULL;

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
extern USB_ISR_PTR_T g_USB_irq_table;

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
//extern void SCI_UPM_EnableDeepSleep (uint32 mode);

//@Thomas.Chai Use For Usb Trace
LOCAL uint32 s_usb_dma_req_cnt = 0;
#define MAX_USB_EP3_DMA_SIZE   4096//2048
LOCAL uint8 s_usb_ep3_dma_buff[MAX_USB_EP3_DMA_SIZE];

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL void USB_EnableClk (uint32 clk_val, uint32 pll_clk)
{
}


/*****************************************************************************/
//  Description:   USB_SetPllDividor
//  Global resource dependence:
//  Author:        Daniel.Ding
//  Note:          set usb 12m and 48m clk dividor according to PLL
/*****************************************************************************/
PUBLIC void USB_SetPllDividor (void)
{
}

PUBLIC void USB_LDO_Control (BOOLEAN flag)
{
    if (flag)
    {
        LDO_TurnOnLDO (LDO_LDO_USB);
    }
    else
    {
        LDO_TurnOffLDO (LDO_LDO_USB);
    }
}

PUBLIC void USB_ClkReset (void)
{
    // Reset usb device
    DEVICE_Reset (g_dev_usb);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function was called in CHARGE module,
//                  when charge in event coming .
/*****************************************************************************/
PUBLIC void USB_Connect (void)
{
    /*pll init*/
    USB_SetPllDividor();

    //Enable USB LDO
    USB_LDO_Control (TRUE);

    //Open usb clock
    DEVICE_Open (g_dev_usb);

    //SCI_TRACE_LOW:"USB LDO enable  ! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_113_112_2_17_23_7_58_1461,(uint8*)"");
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function was called in CHARGE module,
//                  when charge out event coming .
/*****************************************************************************/
PUBLIC void USB_Disconnect (void)
{
    //Disable USB LDO
    USB_LDO_Control (FALSE);
    //SCI_TRACE_LOW:"USB LDO disable!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_125_112_2_17_23_7_58_1462,(uint8*)"");

#if 0
    SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);

    //Send a signal to manager ;
    if (UPM_GetConnectState())
    {
        UPM_SetConnectState (FALSE);

        if (UPM_SendCommand (UPM_MANAGER,UPM_CMD_PLUG_OUT,NULL,NULL))
        {
            //SCI_TRACE_LOW:"USB send plugout signal to UPM!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_137_112_2_17_23_7_58_1463,(uint8*)"");
        }
    }

#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function was called in UPM module,when set service ;
/*****************************************************************************/
PUBLIC void USB_RegIrq (USB_ISR_PTR_T *pIrq)
{
    SCI_ASSERT (pIrq);/*assert verified*/
    g_USB_irq_table = * pIrq;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EnableIrq (void)
{
    CHIPDRV_EnableIRQINT(TB_USB_INT);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_DisableIrq (void)
{
    CHIPDRV_DisableIRQINT(TB_USB_INT);
}

/*****************************************************************************/
//  Description:    This function initialize global variables and enalbe some
//                  control registers.
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Init (void)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

#ifdef PLATFORM_SC6800H
    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);

    //Disable USB LDO
    USB_LDO_Control (FALSE);

    REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN

    SCI_DisableIRQ();

    SCI_ASSERT (DEVICE_Find ("USB", &g_dev_usb) == CLOCK_SUCCESS);/*assert verified*/

    //Open usb device
    DEVICE_Open (g_dev_usb);

    // Reset usb device
    DEVICE_Reset (g_dev_usb);

    USB_PowerSet (1); //Self power;

#endif

    //Enable USB device ;
    ctl_usb->ctrl.mBits.en      = TRUE;

    SCI_RestoreIRQ();

    USB_INTERPACKET_TIMEOUT_SET

    //Register irq to OS .
    ISR_RegHandler (TB_USB_INT, (TB_ISR) USB_irq);

    //Register dma channel callback func
    DMA_HAL_ChnIntConfig (DMA_USB_EP1, DMA_CHN_INT_TYPE_TRANSDONE, USB_dma_ep1);
    DMA_HAL_ChnIntConfig (DMA_USB_EP2, DMA_CHN_INT_TYPE_TRANSDONE, USB_dma_ep2);
}

/*****************************************************************************/
//  Description:    This function reset the usb clock
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Reset (void)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);

    /*pll init*/
    USB_SetPllDividor();
    ctl_usb->int_clr.dwValue  = 0x7F;
    ctl_ep0->int_clr.dwValue  = 0xffffffff;
    ctl_ep1->int_clr.dwValue  = 0xffffffff;
    ctl_ep2->int_clr.dwValue  = 0xffffffff;
    ctl_ep3->int_clr.dwValue  = 0xffffffff;
    ctl_ep4->int_clr.dwValue  = 0xffffffff;

    // Reset usb device
    DEVICE_Reset (g_dev_usb);

    USB_PowerSet (1); //Self power;

    // Enable USB device ;
    ctl_usb->ctrl.mBits.en      = TRUE;

    USB_INTERPACKET_TIMEOUT_SET
}

/*****************************************************************************/
//  Description:    This function set usb is self-power or bus-power
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_PowerSet (USB_POWER_E power)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    //Set USB power ;
    ctl_usb->ctrl.mBits.power       = power;
}

/*****************************************************************************/
//  Description:    This function is called when usb protctol error occurs
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           value: usb error type
/*****************************************************************************/
PUBLIC void USB_ErrorHandle (USB_ERROR_TYPE_E value)
{
    SCI_ASSERT (value < USB_ERR_MAX);/*assert verified*/

    switch (value)
    {
        case USB_ERR_INACK_TIMEOUT:
            {
                //SCI_TRACE_LOW:"usb intoken ack timeout"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_277_112_2_17_23_7_58_1464,(uint8*)"");
                USB_LDO_Control (FALSE);
            }
            break;
        case USB_ERR_INVALID_CLASSCMD:
            {
                USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;

                //SCI_TRACE_LOW:"USB_REQ_CLASS error setup value:0x%08x,0x%08x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_286_112_2_17_23_7_58_1465,(uint8*)"dd",ep0_ctrl->setup_high.dwValue,ep0_ctrl->setup_low.dwValue);
                USB_LDO_Control (FALSE);
            }
            break;
        case USB_ERR_INVALID_VENDORCMD:
            {
                USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;

                //SCI_TRACE_LOW:"USB_REQ_VENDOR error setup value:0x%08x,0x%08x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_295_112_2_17_23_7_58_1466,(uint8*)"dd",ep0_ctrl->setup_high.dwValue,ep0_ctrl->setup_low.dwValue);
                USB_LDO_Control (FALSE);
            }
            break;
        case USB_ERR_BSD_GETDEV_INFO:
            {
                //SCI_TRACE_LOW:"bsd get device info error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_301_112_2_17_23_7_58_1467,(uint8*)"");
                USB_LDO_Control (FALSE);
            }
            break;
        case USB_ERR_BSD_OPENDEV:
            {
                //SCI_TRACE_LOW:"bsd open device fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_307_112_2_17_23_7_58_1468,(uint8*)"");
                USB_LDO_Control (FALSE);
            }
            break;
        case USB_ERR_BSD_CLOSEDEV:
            {
                //SCI_TRACE_LOW:"bsd close device fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_313_112_2_17_23_7_58_1469,(uint8*)"");
            }
            break;
        case USB_ERR_PARAM_ERR:
            {
                //SCI_TRACE_LOW:"UPCC requese type error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_E_318_112_2_17_23_7_58_1470,(uint8*)"");
                USB_LDO_Control (FALSE);
            }
            break;
        default:
            break;
    }

}

/*****************************************************************************/
//  Description:    Call this function to control epx data in ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataInCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    USB_EPx_REG_T *ctrl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctrl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    switch (ep_id)
    {
        case USB_EP0_IN:
            ctrl_ep0->ctrl.mBits.data_ready = ctrl;
            break;
        case USB_EP1:
            ctrl_ep1->ctrl.mBits.data_ready = ctrl;
            break;
        case USB_EP3:
            ctrl_ep3->ctrl.mBits.data_ready = ctrl;
            break;
        default:
            break ;
    }
}

/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataOutCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    USB_EPx_REG_T *ctrl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctrl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    switch (ep_id)
    {
        case USB_EP0_OUT:
            ctrl_ep0->ctrl.mBits.buf_ready  = ctrl;
            break;
        case USB_EP2:
            ctrl_ep2->ctrl.mBits.buf_ready  = ctrl;
            break;
        case USB_EP4:
            ctrl_ep4->ctrl.mBits.buf_ready  = ctrl;
            break;
        default:
            break ;
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
uint32 USB_EP1InDmaReq (uint32 *pBuf,uint32 len)
{
    USB_EPx_REG_T *ctl_ep1   = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_DEV_REG_T *ctl_usb   = (USB_DEV_REG_T *) USB_REG_BASE;
    DMA_CHNCFG_T dma_ctl_ep1 = {0};

    dma_ctl_ep1.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep1.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep1.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep1.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep1.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep1.src_address = (uint32) pBuf;
    dma_ctl_ep1.dest_address   = USB_EP1_FIFO;
    dma_ctl_ep1.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep1.block_length   = USB_FIFO_MAX_BYTE;
    dma_ctl_ep1.total_length   = len;
    dma_ctl_ep1.src_step       = 4;  // src increament
    dma_ctl_ep1.dest_step      = 0;  // des nochange
    dma_ctl_ep1.src_blockmode  = DMA_BLOCKMODE_INCR4;
    dma_ctl_ep1.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

    DMA_HAL_ChnConfig (DMA_USB_EP1, &dma_ctl_ep1);

    //fifo reset for endp1
    ctl_usb->ep_rest.mBits.ep1_fifo_rst = 1;

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep1.src_address;
        cacheConfig[0].bufferSize = dma_ctl_ep1.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex (DMA_USB_EP1, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP1,TRUE);

    SCI_ASSERT (len <= USB_MAX_TRANSFER_SIZE);/*assert verified*/

    ctl_ep1->ctrl.mBits.pack_size       = (len > USB_FIFO_MAX_BYTE) ?USB_FIFO_MAX_BYTE:len ;
    ctl_ep1->transfer_size.mBits.num    = len;

    //Set data ready ;
    USB_EPxDataInCtrl (TRUE,USB_EP1);

    return 0;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
uint32 USB_EP2OutDmaReq (uint32 *pBuf,uint32 len)
{
    USB_EPx_REG_T *ctl_ep2   = (USB_EPx_REG_T *) USB_EP2_BASE ;
    DMA_CHNCFG_T dma_ctl_ep2 = {0};

    dma_ctl_ep2.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep2.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep2.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep2.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep2.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep2.src_address = USB_EP2_FIFO;
    dma_ctl_ep2.dest_address   = (uint32) pBuf;
    dma_ctl_ep2.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep2.block_length   = USB_FIFO_MAX_BYTE;
    dma_ctl_ep2.total_length   = len;
    dma_ctl_ep2.src_step       = 0;  // src nochange
    dma_ctl_ep2.dest_step      = 4;  // des increament
    dma_ctl_ep2.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    dma_ctl_ep2.dest_blockmode = DMA_BLOCKMODE_INCR4;

    DMA_HAL_ChnConfig (DMA_USB_EP2, &dma_ctl_ep2);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep2.dest_address;
        cacheConfig[0].bufferSize = dma_ctl_ep2.total_length;
        cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
        DMA_HAL_ChnEnable_Ex (DMA_USB_EP2, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP2,TRUE);

    SCI_ASSERT (len <= USB_MAX_TRANSFER_SIZE);/*assert verified*/

    ctl_ep2->ctrl.mBits.pack_size   = (len > USB_FIFO_MAX_BYTE) ?USB_FIFO_MAX_BYTE:len ;
    ctl_ep2->transfer_size.mBits.num    = len;

    //Set data ready ;
    USB_EPxDataOutCtrl (TRUE,USB_EP2);

    return 0;
}


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:           the type of "len " is BYTE
/*****************************************************************************/
uint32 USB_EP3InDmaReq (uint32 *pBuf,uint32 len)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    USB_EPx_REG_T *ctl_ep3   = (USB_EPx_REG_T *) USB_EP3_BASE ;
    DMA_CHNCFG_T dma_ctl_ep3 = {0};
    uint32 *dma_buf_ptr = (uint32 *) s_usb_ep3_dma_buff;
    uint32 dma_len, word_len = 0;

    s_usb_dma_req_cnt ++;


    ctl_ep3->int_clr.dwValue                = 0x3fff ;  //Clear
    ctl_usb->ep_rest.mBits.ep3_fifo_rst = 1;   //Fifo clear
    dma_len = (len + 63) & (~0x3F); //length should be the multiple of 64

    SCI_PASSERT ( (dma_len <= MAX_USB_EP3_DMA_SIZE),/*assert verified*/
                  ("%d, %d", dma_len, MAX_USB_EP3_DMA_SIZE));

    //swap word section and save them to dma buffer
    word_len = len >> 2;
    USB_FIFODWordBufEndianSwap (pBuf, dma_buf_ptr, word_len);

    //swap the left byte of pBuff
    if (len&0x03) //The left buffer have data
    {
        dma_buf_ptr[word_len] =  0;
        SCI_MEMCPY ( (uint8 *) &dma_buf_ptr[word_len], (uint8 *) &pBuf[word_len], len&0x03); //copy the left data
        //swap the left data
        dma_buf_ptr[word_len] = USB_FIFODWordEndianSwap (dma_buf_ptr[word_len]);
    }

    dma_ctl_ep3.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep3.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep3.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep3.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep3.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep3.src_address = (uint32) s_usb_ep3_dma_buff;
    dma_ctl_ep3.dest_address   = USB_EP3_FIFO;
    dma_ctl_ep3.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep3.block_length   = USB_FIFO_MAX_BYTE>>1;
    dma_ctl_ep3.total_length   = dma_len;
    dma_ctl_ep3.src_step       = 4;  // src increament
    dma_ctl_ep3.dest_step      = 0;  // des nochange
    dma_ctl_ep3.src_blockmode  = DMA_BLOCKMODE_INCR4;
    dma_ctl_ep3.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

    DMA_HAL_ChnConfig (DMA_USB_EP3, &dma_ctl_ep3);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep3.src_address;
        cacheConfig[0].bufferSize = dma_ctl_ep3.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex (DMA_USB_EP3, SCI_TRUE, cacheConfig, 1);
    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP3,TRUE);

    ctl_ep3->ctrl.mBits.pack_size       = (len > USB_FIFO_MAX_BYTE) ?USB_FIFO_MAX_BYTE:len ;

    ctl_ep3->transfer_size.mBits.num    = dma_ctl_ep3.total_length;

    //Set data ready ;
    USB_EPxDataInCtrl (TRUE,USB_EP3);

    return 0;
}

/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxFIFOReset (uint32 ep_id)
{
    USB_DEV_REG_T *ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

    //Clear fifo
    ctrl_usb->ep_rest.dwValue |= 0x1 << ep_id ;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

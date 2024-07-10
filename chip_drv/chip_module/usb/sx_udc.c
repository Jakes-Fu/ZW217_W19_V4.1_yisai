/******************************************************************************
 ** File Name:    sx_udc.c                                                    *
 ** Author:       SpreadTrum                                                  *
 ** DATE:         8/25/2010                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 8/25/2010                     Create.                                     *
 *****************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "sx_udc.h"
#include "udc_api.h"
#include "ldo_drvapi.h"
#include "power_manager.h"
#include "usbservice_api.h"
#include "ref_outport.h"
#include "gpio_prod_api.h"
#include "task_monitor.h"
#include "usb_timer.h"
#include "usb_device.h"
#include "clock_drvapi.h"
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#include "priority_base.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif



typedef enum EP_Status_e
{
    EP_STATUS_IDLE  = 0,            /* idle */
    EP_STATUS_SEND_DATA = 1,        /* send data packet */
    EP_STATUS_SEND_EMPTY = 2   /* send empty packet*/
}EP_STATUS_E;

LOCAL EP_STATUS_E   ep_status[16] = {EP_STATUS_IDLE};

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#ifdef PLATFORM_SC6530
//define DMA req_id_addr for USB module
#define USB_EP0_IN_REQ               (17)
#define USB_EP0_OUT_REQ              (18)
#define USB_EP1_REQ                  (19)
#define USB_EP2_REQ                  (20)
#define USB_EP3_REQ                  (21)
#define USB_EP4_REQ                  (22)
#define USB_EP0_IN_REQ_ADDR       (DMA_CTL_REG_BASE+0x2000+(USB_EP0_IN_REQ-1)*0x04)
#define USB_EP0_OUT_REQ_ADDR      (DMA_CTL_REG_BASE+0x2000+(USB_EP0_OUT_REQ-1)*0x04)
#define USB_EP1_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP1_REQ-1)*0x04)
#define USB_EP2_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP2_REQ-1)*0x04)
#define USB_EP3_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP3_REQ-1)*0x04)
#define USB_EP4_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP4_REQ-1)*0x04)

#define USB_DMA_CONNECT(req_addr, chn) {                                      \
        CHIP_REG_SET(req_addr, chn);\
    }
#endif

#if defined(PLATFORM_SC6531EFM)
//define DMA req_id_addr for USB module

#define USB_EP0_IN_REQ            (13)    //(channel 13)
#define USB_EP0_OUT_REQ           (14)    //(channel 14)
#define USB_EP1_REQ               (15)    //(channel 15)
#define USB_EP2_REQ               (16)    //(channel 16)
#define USB_EP3_REQ               (17)    //(channel 17)
#define USB_EP4_REQ               (18)    //(chanel 18)
#define USB_EP0_IN_REQ_ADDR       (DMA_CTL_REG_BASE+0x2000+(USB_EP0_IN_REQ-1)*0x04)
#define USB_EP0_OUT_REQ_ADDR      (DMA_CTL_REG_BASE+0x2000+(USB_EP0_OUT_REQ-1)*0x04)
#define USB_EP1_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP1_REQ-1)*0x04)
#define USB_EP2_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP2_REQ-1)*0x04)
#define USB_EP3_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP3_REQ-1)*0x04)
#define USB_EP4_REQ_ADDR          (DMA_CTL_REG_BASE+0x2000+(USB_EP4_REQ-1)*0x04)

#define USB_DMA_CONNECT(req_addr, chn) {                                      \
        CHIP_REG_SET(req_addr, chn);\
    }
#endif

#if defined(PLATFORM_UWS6121E)
//define DMA req_id_addr for USB module

#define USB_EP0_IN_REQ            (13)    //(channel 13)
#define USB_EP0_OUT_REQ           (14)    //(channel 14)
#define USB_EP1_REQ               (15)    //(channel 15)
#define USB_EP2_REQ               (16)    //(channel 16)
#define USB_EP3_REQ               (17)    //(channel 17)
#define USB_EP4_REQ               (18)    //(chanel 18)
#define USB_EP0_IN_REQ_ADDR       (REG_DMA_BASE+0x40+(USB_EP0_IN_REQ-1)*0x04)
#define USB_EP0_OUT_REQ_ADDR      (REG_DMA_BASE+0x40+(USB_EP0_OUT_REQ-1)*0x04)
#define USB_EP1_REQ_ADDR          (REG_DMA_BASE+0x40+(USB_EP1_REQ-1)*0x04)
#define USB_EP2_REQ_ADDR          (REG_DMA_BASE+0x40+(USB_EP2_REQ-1)*0x04)
#define USB_EP3_REQ_ADDR          (REG_DMA_BASE+0x40+(USB_EP3_REQ-1)*0x04)
#define USB_EP4_REQ_ADDR          (REG_DMA_BASE+0x40+(USB_EP4_REQ-1)*0x04)

#define USB_DMA_CONNECT(req_addr, chn) {                                      \
        CHIP_REG_SET(req_addr, chn);\
    }
#endif

#define USB_LDO_ENABLE                  (0x1)
#define USB_LDO_DISABLE                 (0x2)
#define USB_INTERPACKET_TIMEOUT_SET
#define REGISTER_MAP_LEN    (0x400)
#if defined(PLATFORM_SC6800H)
#define USB_SOFT_RESET_BIT              (BIT_6)

#define USB_DMA_CONNECT(req_addr, chn) {                                      \
        CHIP_REG_SET(req_addr, chn);\
    }
#endif

#define MAX_USB_EP3_DMA_SIZE            4096//2048
#define WORD_ADDRESS_MASK               0x00000003UL
//define dword endian swap
#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))
#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#undef UDC_DEBUG
#define UDC_DEBUG( x )  RM_SendRes(x,strlen(x))

#undef UDC_DEBUG
#define UDC_DEBUG( x )
#define USB_BASE_EP(n)                    (USB_EPx_BASE + 0x40*(n-1))

//#define USB_DEBUG_INFO

LOCAL uint8 *s_usb_device_descriptor = NULL;  /*lint -esym(551,s_usb_device_descriptor)*/
LOCAL uint8 *s_usb_config_descriptor = NULL;  /*lint -esym(551,s_usb_config_descriptor)*/
LOCAL uint8    s_descriptor_type = 0;
LOCAL DEVICE_HANDLE usb_clk_device = SCI_NULL;
/**---------------------------------------------------------------------------*
**--------------------------- Local Data ------------------------------------**
**---------------------------------------------------------------------------*/
LOCAL const uint32 *s_USB_fifo_add_table [USB_EPx_NUMBER]=
{
    (uint32 *) USB_EP0_IN_FIFO,
    (uint32 *) USB_EP0_OUT_FIFO,
    (uint32 *) USB_EP1_FIFO,
    (uint32 *) USB_EP2_FIFO,
    (uint32 *) USB_EP3_FIFO,
    (uint32 *) USB_EP4_FIFO
};                                                          /*lint !e785 */

LOCAL PLL_DEVICE_REQ_T  s_usbdev_pll_req_12m;
LOCAL PLL_DEVICE_REQ_T  s_usbdev_pll_req_48m;
LOCAL BOOLEAN usb_pll_is_set; /*flag to indicate whether usb pll is registered*/
LOCAL PLL_DEVICE_REQ_T  s_usbdev_pll_req;
LOCAL uint32 s_usb_dma_req_cnt = 0;

LOCAL USB_EP_OPS_T    sx_ep_ops;
LOCAL USB_GADGET_OPS_T   sx_ops;

LOCAL const char        gadget_name[] = "sx_udc";
LOCAL const char        driver_desc[] = "MYGOD";

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E) 
typedef struct SX_UDC_TASK_MSG_TAG
{
    SIGNAL_VARS 
    uint32 int_status;
} SX_UDC_TASK_MSG_T;

LOCAL BLOCK_ID s_udc_task_id;
#define TASK_SX_UDC_SRV_PRI    (30+1)
#define SX_UDC_STACK_SIZE       (1024*4)
#define SX_UDC_QUEUE_NUM        (24)
#endif

LOCAL SX_UDC_T          *the_udc_controller;
//LOCAL struct clk      *udc_clock;
//LOCAL struct clk      *usb_bus_clock;
LOCAL uint32            base_addr;
LOCAL SX_UDC_MACH_INFO_T   *udc_info;
LOCAL SX_UDC_MACH_INFO_T    udc_cfg;
LOCAL  SX_UDC_T memory;
LOCAL int s_fifo_blocking;
LOCAL uint32    s_setup_packet_backup[2]= {0}; /*lint -esym(551,s_setup_packet_backup)*/
//typedef enum usb_dma_type_e
typedef enum  usb_dma_type_e
{
    DMA_TYPE_USB_EP0_IN=0,
    DMA_TYPE_USB_EP0_OUT,
    DMA_TYPE_USB_EP1,
    DMA_TYPE_USB_EP2,
    DMA_TYPE_USB_EP3,
    DMA_TYPE_USB_EP4,
    DMA_TYPE_USB_MAX

} USB_DMA_TYPE_E;

typedef enum
{
    Sx_UDC_EP0 = 0x01,
    Sx_UDC_EP1,
    Sx_UDC_EP2,
    Sx_UDC_EP3,
    Sx_UDC_EP4,
    Sx_HOST_PLUG_IN,
    Sx_UDC_MAX
} Sx_UDC_EPINT_E;

typedef struct USB_ISR_MSG
{
    uint32 endpoint;
    uint32 int_status;
} USB_ISR_MSG_T;
INPUT_BUFFER_INIT (USB_ISR_MSG_T,8)

/*----------------------------------------------------------------------------*
**                         Local Data Type                                    *
**---------------------------------------------------------------------------*/
//typedef void (*irq_handler_t) (uint32);

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL void SxUDC_Reset (void);
LOCAL BOOLEAN SxUDC_TxDWordToFIFO (uint32 *buf, uint8 len, uint8 ep_id);
LOCAL void SxUDC_Done (SX_EP_T *ep, SX_REQUEST_T *req, int status);
LOCAL int  SxUDC_Queue (USB_EP_T *_ep, USB_REQUEST_T *_req);
LOCAL void SxUDC_WriteLoop (USB_EP_T *_ep, USB_REQUEST_T *_req);
LOCAL uint32 SxUDC_GetHostReq (void);
LOCAL void SxUDC_EnableUSBIrq (void);
LOCAL void SxUDC_DisableUSBIrq (void);
LOCAL void SxUDC_VbusIrq (uint32 gpio_state);

#ifdef USB_DEBUG_INFO
LOCAL uint32 usb_register_map[REGISTER_MAP_LEN>>2]= {0};
#endif

LOCAL uint32 s_dma_channel[DMA_TYPE_USB_MAX] = {0};
/*----------------------------------------------------------------------------*
**                         External Data                                      *
**---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
**                         External Function Prototype                        *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:   out endpoint0 handler.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void USB_InitEPStatus (void)
{    /*lint !e401 */
    uint32 i=0;

    for (i=0; i<8; i++)
    {
        ep_status[i] = EP_STATUS_IDLE;
    }
}
/*****************************************************************************/
//  Description:   set endpoint transfer status.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL void USB_SetEPIdle (uint8 ep_id)
{
    ep_status[ep_id]=EP_STATUS_IDLE;
}
/*****************************************************************************/
//  Description:   set endpoint transfer status.
//  Global resource dependence:
//  Author:        jiayong.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_IsEPIdle (uint8 epno)
{
    return (ep_status[epno]==EP_STATUS_IDLE);
}



/******************************************************************************/
//  Description:    SxUDC_ReadRegister
//  Global resource dependence:
//  Author:
//  Note:           Call this function to read udc register ;
/******************************************************************************/
LOCAL uint32 SxUDC_ReadRegister (uint32 reg)
{
    return * (volatile uint32 *) (base_addr+reg);
}

/******************************************************************************/
//  Description:    SxUDC_WriteRegister
//  Global resource dependence:
//  Author:
//  Note:           Call this function to write udc register ;
/******************************************************************************/
LOCAL void SxUDC_WriteRegister (uint32 value, uint32 reg)
{
    * (volatile uint32 *) (base_addr+reg) = value;
}

/******************************************************************************/
//  Description:    SxUDC_Pull
//  Global resource dependence:
//  Author:
//  Note:           Call this function to pull udc command ;
/******************************************************************************/
LOCAL void SxUDC_Pull (SX_UDC_CMD_E cmd)
{
    switch (cmd)
    {
        case SX_UDC_P_ENABLE :
            break;
        case SX_UDC_P_DISABLE :
            break;
        case SX_UDC_P_RESET :
            break;
        default:
            break;
    }
}
USB_EPx_REG_T g_usb_ep1_reg;
void dump_usb_registers (void)
{
#ifdef USB_DEBUG_INFO
    uint32 i;
    uint32 *src,*dest;
    USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;

    g_usb_ep1_reg = *ctl;
    src = (uint32 *) USB_REG_BASE;
    dest = (uint32 *) usb_register_map;

    for (i=0; i<REGISTER_MAP_LEN/4; i++)
    {
        *dest++ = *src++;
    }
#endif

}

#if 1
#ifdef USB_DEBUG_INFO
uint32 usb_trace_info3[256];
uint32 usb_trace_info4[256];
uint8   usb_index3= 0;
uint8   usb_index4= 0;
#endif
void record_usb_trace3 (uint32 para1)
{
#ifdef USB_DEBUG_INFO
    usb_trace_info3[usb_index3] = para1;
    usb_index3++;
#endif
}
void record_usb_trace4 (uint32 para1)
{
#ifdef USB_DEBUG_INFO
    usb_trace_info4[usb_index4++] = para1;
#endif
}
uint32 ep1_queue_count[4] = {0};
#endif
/******************************************************************************/
//  Description:    SxUDC_PullUp
//  Global resource dependence:
//  Author:
//  Note:           Call this function to pull up udc vbus ;
/******************************************************************************/
LOCAL int SxUDC_PullUp (USB_GADGET_T *gadget, int is_on)
{
    return 0;
}
/******************************************************************************/
//  Description:    SxUDC_DMAEPIn
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb dma in  ;
/******************************************************************************/
LOCAL void SxUDC_DMAEPIn (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep0in()");
}

/******************************************************************************/
//  Description:    SxUDC_DMAEPOut
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb dma out ;
/******************************************************************************/
LOCAL void SxUDC_DMAEPOut (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep0out()");
}

/******************************************************************************/
//  Description:    SxUDC_DMAEP1
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb ep1 dma transfer ;
/******************************************************************************/
LOCAL void SxUDC_DMAEP1 (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep1()");
}

/******************************************************************************/
//  Description:    SxUDC_DMAEP2
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb ep2 dma transfer ;
/******************************************************************************/
LOCAL void SxUDC_DMAEP2 (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep2()");
}

/******************************************************************************/
//  Description:    SxUDC_DMAEP3
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb ep3 dma transfer ;
/******************************************************************************/
LOCAL void SxUDC_DMAEP3 (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep3()");
}

/******************************************************************************/
//  Description:    SxUDC_DMAEP4
//  Global resource dependence:
//  Author:
//  Note:           Call this function when usb ep4 dma transfer ;
/******************************************************************************/
LOCAL void SxUDC_DMAEP4 (uint32 param)
{
    UDC_DEBUG ("sx_udc_USB_dma_ep4()");
}

/******************************************************************************/
//  Description:    SxUDC_FIFODWordEndianSwap
//  Global resource dependence:
//  Author:
//  Note:           Call this function for dword endian swap
/******************************************************************************/
LOCAL uint32 SxUDC_FIFODWordEndianSwap (uint32 DWord)
{
#ifdef _LITTLE_ENDIAN_DEBUG
    return DWORD_ENDIAN_SWAP (DWord);
#else
    return DWord;
#endif
}

/******************************************************************************/
//  Description:    SxUDC_FIFODWordBufEndianSwap
//  Global resource dependence:
//  Author:
//  Note:           Call this function for dword buffer endian swap
/******************************************************************************/
LOCAL BOOLEAN SxUDC_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len)
{
#ifdef _LITTLE_ENDIAN_DEBUG

    while (len-- > 0)
    {
        *des++  = DWORD_ENDIAN_SWAP (*src);
        src++;
    }

#else
    memcpy (des, src, len<<2);
#endif
    return TRUE ;
}


/******************************************************************************/
//  Description:    SxUDC_ErrorHandle
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle error when it happens
/******************************************************************************/
LOCAL void SxUDC_ErrorHandle (USB_ERROR_TYPE_E value)
{
    SCI_ASSERT (value < USB_ERR_MAX);/*assert verified*/

    switch (value)
    {
        case USB_ERR_INACK_TIMEOUT:
            {
                UDC_DEBUG ("usb intoken ack timeout");
                UDC_LDOControl (FALSE);
            }
            break;
        case USB_ERR_INVALID_CLASSCMD:
            {
                //USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;

                UDC_DEBUG ("USB_REQ_CLASS error setup ");
                UDC_LDOControl (FALSE);
            }
            break;
        case USB_ERR_INVALID_VENDORCMD:
            {
                //USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;

                UDC_DEBUG ("USB_REQ_VENDOR error setup");
                UDC_LDOControl (FALSE);
            }
            break;
        case USB_ERR_BSD_GETDEV_INFO:
            {
                UDC_DEBUG ("bsd get device info error");
                UDC_LDOControl (FALSE);
            }
            break;
        case USB_ERR_BSD_OPENDEV:
            {
                UDC_DEBUG ("bsd open device fail");
                UDC_LDOControl (FALSE);
            }
            break;
        case USB_ERR_BSD_CLOSEDEV:
            {
                UDC_DEBUG ("bsd close device fail");
            }
            break;
        case USB_ERR_PARAM_ERR:
            {
                UDC_DEBUG ("UPCC requese type error");
                UDC_LDOControl (FALSE);
            }
            break;
        default:
            break;
    }

}


/******************************************************************************/
//  Description:    SxUDC_RxDWordFromFIFO
//  Global resource dependence:
//  Author:
//  Note:           Call this function to read data from ep fifo to the buf
/******************************************************************************/
LOCAL BOOLEAN SxUDC_RxDWordFromFIFO (
    uint32 *buf ,
    uint8 len ,
    uint8 ep_id)
{
    int i;

    SCI_ASSERT (len   <= USB_FIFO_MAX_WORD);/*assert verified*/
    SCI_ASSERT (ep_id < USB_EPx_NUMBER);/*assert verified*/
    SCI_ASSERT ( ( (uint32) buf & WORD_ADDRESS_MASK) ==0);/*assert verified*/

    for (i=0; i<len; i++)
    {
        * (buf + i) = SxUDC_FIFODWordEndianSwap (*s_USB_fifo_add_table [ep_id]) ;/*lint !e661*/
    }

    return TRUE ;
}       //Send data to FIFO ;

/******************************************************************************/
//  Description:    SxUDC_AllocDma
//  Global resource dependence:
//  Author:
//  Note:           Alloc/Free the dma channel for usb
/******************************************************************************/



LOCAL uint32 SxUDC_AllocDma (USB_DMA_TYPE_E usb_dma_type)
{
    uint32 channel = 0;
#ifdef PLATFORM_SC6800H

    switch (usb_dma_type)
    {
        case DMA_TYPE_USB_EP0_IN:
            return (DMA_USB_EP0_IN);
        case DMA_TYPE_USB_EP0_OUT:
            return (DMA_USB_EP0_OUT);
        case DMA_TYPE_USB_EP1:
            return (DMA_USB_EP1);
        case DMA_TYPE_USB_EP2:
            return (DMA_USB_EP2);
        case DMA_TYPE_USB_EP3:
            return (DMA_USB_EP3);
        case DMA_TYPE_USB_EP4:
            return (DMA_USB_EP4);
        default:
            break;
    }

#else  //PLATFORM_SC6530

    if (s_dma_channel[usb_dma_type])
    {
        return (s_dma_channel[usb_dma_type]);
    }

    channel = DMA_HAL_AllocChn (DMA_CHN_TRSF_TYPE_STANDARD);
    SCI_PASSERT ( (channel != DMA_CHANNELID_INVALID), ("SxUDC_AllocDma")); /*assert to do*/
    s_dma_channel[usb_dma_type] = channel;

    switch (usb_dma_type)
    {
        case DMA_TYPE_USB_EP0_IN:
            USB_DMA_CONNECT (USB_EP0_IN_REQ_ADDR, channel);
            break;
        case DMA_TYPE_USB_EP0_OUT:
            USB_DMA_CONNECT (USB_EP0_OUT_REQ_ADDR, channel);
            break;
        case DMA_TYPE_USB_EP1:
            USB_DMA_CONNECT (USB_EP1_REQ_ADDR, channel);
            break;
        case DMA_TYPE_USB_EP2:
            USB_DMA_CONNECT (USB_EP2_REQ_ADDR, channel);
            break;
        case DMA_TYPE_USB_EP3:
            USB_DMA_CONNECT (USB_EP3_REQ_ADDR, channel);
            break;
        case DMA_TYPE_USB_EP4:
            USB_DMA_CONNECT (USB_EP4_REQ_ADDR, channel);
            break;
        default:
            break;
    }

    return (channel);
#endif
}

LOCAL uint32 SxUDC_FreeDma (USB_DMA_TYPE_E usb_dma_type)
{

    uint32 chan = s_dma_channel[usb_dma_type];
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

    if (0 != chan)
    {
        switch (usb_dma_type)
        {
            case DMA_TYPE_USB_EP0_IN:
                USB_DMA_CONNECT (USB_EP0_IN_REQ_ADDR, 0);
                break;
            case DMA_TYPE_USB_EP0_OUT:
                USB_DMA_CONNECT (USB_EP0_OUT_REQ_ADDR, 0);
                break;
            case DMA_TYPE_USB_EP1:
                USB_DMA_CONNECT (USB_EP1_REQ_ADDR, 0);
                break;
            case DMA_TYPE_USB_EP2:
                USB_DMA_CONNECT (USB_EP2_REQ_ADDR, 0);
                break;
            case DMA_TYPE_USB_EP3:
                USB_DMA_CONNECT (USB_EP3_REQ_ADDR, 0);
                break;
            case DMA_TYPE_USB_EP4:
                USB_DMA_CONNECT (USB_EP4_REQ_ADDR, 0);
                break;
            default:
                break;
        }

        DMA_HAL_FreeChn (chan);
        s_dma_channel[usb_dma_type] = 0;
    }

#endif
    return 0;
}
LOCAL uint32 SXUDC_FreeAllDMA()
{
    USB_DMA_TYPE_E i;

    for (i=DMA_TYPE_USB_EP0_IN; i<DMA_TYPE_USB_MAX; i++)
    {
        SxUDC_FreeDma (i);
    }

    return 0;
}
uint32 _usb_transfer_timeout=0;
LOCAL void SxUDC_EPn_DMASoftRequest (uint32 ep_no)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
#if 0
    extern PUBLIC void DMA_PHY_ChnEnable (uint32 dma_channel, BOOLEAN is_enable);
    extern PUBLIC void DMA_PHY_ChnRequest (uint32 dma_channel);

    uint32 dma_channel=0;
    uint32 index = DMA_TYPE_USB_EP0_OUT;
    //#define USB_BASE_EP(n)                    (USB_EPx_BASE + 0x40*(n-1))
    USB_EPx_REG_T *ctl_ep = (USB_EPx_REG_T *) USB_BASE_EP (ep_no) ;

    dma_channel = s_dma_channel[index+ep_no];

    if (dma_channel)
    {
        DMA_PHY_ChnEnable (dma_channel,SCI_TRUE);
        DMA_PHY_ChnRequest (dma_channel);
    }
    else
    {
        //SCI_TRACE_LOW:"ep_no(%d) is invalid !!!\n"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT,SX_UDC_617_112_2_17_23_7_47_1450, (uint8 *) "d",ep_no);
    }

    if ( (ep_no & BIT_0) && (ctl_ep->ctrl.mBits.data_ready  == 0))
    {
        ctl_ep->ctrl.mBits.data_ready  = 1;
    }

#else
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    USB_EPx_REG_T *ctl_ep = (USB_EPx_REG_T *) USB_BASE_EP (ep_no) ;
    SX_UDC_T *dev = &memory;
    SX_REQUEST_T    *req =NULL;
    SX_EP_T *ep  = NULL;
    ep = &dev->ep[ep_no];
    _usb_transfer_timeout++;

    ctl_ep->int_clr.dwValue= 0x3FFF;
    ctl_ep->int_ctrl.dwValue= 0;
    ctl_ep->ctrl.mBits.data_ready  = 0;
    ctl_ep->ctrl.mBits.buf_ready  = 0;
    switch (ep_no)
    {
        case 1:
            ctl_usb->ep_rest.mBits.ep1_fifo_rst = 1;        
            break;
        case 2:
            ctl_usb->ep_rest.mBits.ep2_fifo_rst = 1;        
            break;
        case 3:
            ctl_usb->ep_rest.mBits.ep3_fifo_rst = 1;        
            break;     
        case 4:
            ctl_usb->ep_rest.mBits.ep4_fifo_rst = 1;        
            break;     
        default:
            break;
    }


    if (list_empty (&ep->queue))
    {
        UDC_DEBUG ("\nsEP queue is empty\n");
        req = NULL;
    }
    else
    {
        UDC_DEBUG ("\nEP queue is inempty\n");
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);             /*lint !e413 */
    }

    SxUDC_Done (ep, req,SCI_SUCCESS);
#endif
    //dump_dma_req_cid_registers();
    //SCI_PASSERT(0,("ep_no = %d",ep_no));
#endif
}
/******************************************************************************/
//  Description:    SxUDC_EP3InDmaReq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle EP3 dma in request
/******************************************************************************/
LOCAL uint32 SxUDC_EP3InDmaReq (uint32 *pBuf,uint32 len)
{

    uint32 channel_id = SxUDC_AllocDma (DMA_TYPE_USB_EP3);  //CID very important

    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    USB_EPx_REG_T *ctl_ep3   = (USB_EPx_REG_T *) USB_EP3_BASE ;
    DMA_CHNCFG_T dma_ctl_ep3 = {0};
    uint32 dma_len = 0;


    // s_usb_dma_req_cnt ++;

    ctl_ep3->int_clr.dwValue            = 0x3fff ;  //Clear
    ctl_usb->ep_rest.mBits.ep3_fifo_rst = 1;        //Fifo clear

    /*for 6800h/6600l, the limition of DMA transfer size should be multiple of 64
      but usb transfer size can set the exact length, so we donot need to padding 0x7E
    */
    dma_len = (len + 63) & (~0x3F);                 //length should be the multiple of 64  chip  bug


    dma_ctl_ep3.type           = DMA_CHN_TYPE_HARD;//DMA_CHN_TYPE_HARD;
    dma_ctl_ep3.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep3.src_address    = (uint32) pBuf;
    dma_ctl_ep3.dest_address   = USB_EP3_FIFO;
    dma_ctl_ep3.block_length   = USB_FIFO_MAX_BYTE>>1;
    dma_ctl_ep3.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep3.src_step       = 4;
    dma_ctl_ep3.dest_step      = 0;
    dma_ctl_ep3.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep3.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep3.dest_datawidth = DMA_DATAWIDTH_WORD;


    dma_ctl_ep3.src_blockmode  = DMA_BLOCKMODE_INCR4;
    dma_ctl_ep3.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    /*ABCD--> DCBA*/
    dma_ctl_ep3.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    dma_ctl_ep3.total_length = dma_len;

    DMA_HAL_ChnConfig (channel_id,&dma_ctl_ep3);
    DMA_HAL_ChnIntConfig (channel_id, DMA_CHN_INT_TYPE_NONE, SCI_NULL);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep3.src_address;
        cacheConfig[0].bufferSize = dma_ctl_ep3.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP3,TRUE);


    if (len > 64)
    {
        ctl_ep3->ctrl.mBits.pack_size = USB_FIFO_MAX_BYTE;
    }
    else
    {
        ctl_ep3->ctrl.mBits.pack_size = len;
    }


    ctl_ep3->transfer_size.mBits.num    = len;

    //Set data ready ;
    ctl_ep3->ctrl.mBits.data_ready = 1;


    return 0;
}
/******************************************************************************/
//  Description:    SxUDC_EP1InDmaReq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle EP1 dma in request
/******************************************************************************/
LOCAL uint32 SxUDC_EP1InDmaReq (uint32 *pBuf,uint32 len)
{
    uint32 channel_id = SxUDC_AllocDma (DMA_TYPE_USB_EP1);  //CID very important
    USB_EPx_REG_T *ctl_ep1   = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_DEV_REG_T *ctl_usb   = (USB_DEV_REG_T *) USB_REG_BASE;
    DMA_CHNCFG_T dma_ctl_ep1 = {0};
    uint32 dma_len =0;

    //fifo reset for endp1
    ctl_usb->ep_rest.mBits.ep1_fifo_rst = 1;
    ctl_ep1->dma_time.mBits.time            = 400;
    dma_len = (len + 63) & (~0x3F);                 //length should be the multiple of 64  chip  bug


    dma_ctl_ep1.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep1.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep1.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep1.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep1.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep1.src_address    = (uint32) pBuf;
    dma_ctl_ep1.dest_address   = USB_EP1_FIFO;
    dma_ctl_ep1.requestmode    = DMA_CHN_REQMODE_NORMAL;
    
    if(0x40 == dma_len)
    {
        dma_ctl_ep1.block_length   = 64;
    }
    else
    {
        dma_ctl_ep1.block_length   = 32;
    }
    
    dma_ctl_ep1.total_length   = dma_len;
    dma_ctl_ep1.src_step       = 4;  // src increament
    dma_ctl_ep1.dest_step      = 0;  // des nochange
    dma_ctl_ep1.src_blockmode  = DMA_BLOCKMODE_INCR4;
    dma_ctl_ep1.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    /*ABCD--> DCBA*/
    dma_ctl_ep1.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    DMA_HAL_ChnConfig (channel_id,&dma_ctl_ep1);
    DMA_HAL_ChnIntConfig (channel_id, DMA_CHN_INT_TYPE_NONE, SCI_NULL);
    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep1.src_address;
        cacheConfig[0].bufferSize = dma_ctl_ep1.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP1,TRUE);

    SCI_ASSERT (len <= USB_MAX_TRANSFER_SIZE);/*assert verified*/

    //    ctl_ep1->ctrl.mBits.pack_size     = (len > USB_FIFO_MAX_BYTE)?USB_FIFO_MAX_BYTE:len ;
    if (len > 64)
    {
        ctl_ep1->ctrl.mBits.pack_size = USB_FIFO_MAX_BYTE;
    }
    else
    {
        ctl_ep1->ctrl.mBits.pack_size = len;
    }


    ctl_ep1->transfer_size.mBits.num    = len;

    //Set data ready ;
    ctl_ep1->ctrl.mBits.data_ready = 1;

    return 0;
}


/******************************************************************************/
//  Description:    SxUDC_EP2OutDmaReq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle EP2 dma out request
/******************************************************************************/
LOCAL uint32 SxUDC_EP2OutDmaReq (uint32 *pBuf,uint32 len)
{
    uint32 channel_id = SxUDC_AllocDma (DMA_TYPE_USB_EP2);
    USB_EPx_REG_T *ctl_ep2   = (USB_EPx_REG_T *) USB_EP2_BASE ;
    DMA_CHNCFG_T dma_ctl_ep2 = {0};
    uint32 dma_len = 64;
    ctl_ep2->dma_time.mBits.time            = 400;

    dma_len = (len + 63) & (~0x3F);                 //length should be the multiple of 64  chip  bug

    dma_ctl_ep2.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep2.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep2.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep2.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep2.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep2.src_address    = USB_EP2_FIFO;
    dma_ctl_ep2.dest_address   = (uint32) pBuf;
    dma_ctl_ep2.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep2.block_length   = USB_FIFO_MAX_BYTE;
    dma_ctl_ep2.total_length   = dma_len;
    dma_ctl_ep2.src_step       = 0;  // src nochange
    dma_ctl_ep2.dest_step      = 4;  // des increament
    dma_ctl_ep2.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    dma_ctl_ep2.dest_blockmode = DMA_BLOCKMODE_INCR4;
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    /*ABCD--> DCBA*/
    dma_ctl_ep2.endian_switchmode = DMA_ENDIANSWITCHMODE_FULL;
#endif
    DMA_HAL_ChnConfig (channel_id, &dma_ctl_ep2);
    DMA_HAL_ChnIntConfig (channel_id, DMA_CHN_INT_TYPE_NONE, SCI_NULL);
    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep2.dest_address;
        cacheConfig[0].bufferSize = dma_ctl_ep2.total_length;
        cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
        DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP2,TRUE);

    SCI_ASSERT (len <= USB_MAX_TRANSFER_SIZE);/*assert verified*/

    //  ctl_ep2->ctrl.mBits.pack_size   = (len > USB_FIFO_MAX_BYTE)?USB_FIFO_MAX_BYTE:len ;
    if (len > 64)
    {
        ctl_ep2->ctrl.mBits.pack_size = USB_FIFO_MAX_BYTE;
    }
    else
    {
        ctl_ep2->ctrl.mBits.pack_size = len;
    }

    ctl_ep2->transfer_size.mBits.num    = len;

    //Set data ready ;
    ctl_ep2->ctrl.mBits.buf_ready  = 1;

    return 0;
}
/******************************************************************************/
//  Description:    SxUDC_EP2OutDmaReq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle EP2 dma out request
/******************************************************************************/
LOCAL uint32 SxUDC_EP4OutDmaReq (uint32 *pBuf,uint32 len)
{
    uint32 channel_id = SxUDC_AllocDma (DMA_TYPE_USB_EP4);  //CID very important
    USB_EPx_REG_T *ctl_ep   = (USB_EPx_REG_T *) USB_EP4_BASE ;
    DMA_CHNCFG_T dma_ctl_ep = {0};
    uint32 dma_len = 64;
    ctl_ep->dma_time.mBits.time            = 400;
    dma_len = (len + 63) & (~0x3F);                 //length should be the multiple of 64  chip  bug
    dma_ctl_ep.type           = DMA_CHN_TYPE_HARD;
    dma_ctl_ep.priority       = DMA_CHN_PRIORITY_0;
    dma_ctl_ep.workmode       = DMA_CHN_WORKMODE_NORMAL;
    dma_ctl_ep.src_datawidth  = DMA_DATAWIDTH_WORD;
    dma_ctl_ep.dest_datawidth = DMA_DATAWIDTH_WORD;
    dma_ctl_ep.src_address    = USB_EP4_FIFO;
    dma_ctl_ep.dest_address   = (uint32) pBuf;
    dma_ctl_ep.requestmode    = DMA_CHN_REQMODE_NORMAL;
    dma_ctl_ep.block_length   = USB_FIFO_MAX_BYTE;
    dma_ctl_ep.total_length   = dma_len;
    dma_ctl_ep.src_step       = 0;  // src nochange
    dma_ctl_ep.dest_step      = 4;  // des increament
    dma_ctl_ep.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    dma_ctl_ep.dest_blockmode = DMA_BLOCKMODE_INCR4;

    DMA_HAL_ChnConfig (channel_id, &dma_ctl_ep);
    DMA_HAL_ChnIntConfig (channel_id, DMA_CHN_INT_TYPE_NONE, SCI_NULL);
    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) dma_ctl_ep.dest_address;
        cacheConfig[0].bufferSize = dma_ctl_ep.total_length;
        cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;
        DMA_HAL_ChnEnable_Ex (channel_id, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (DMA_USB_EP2,TRUE);

    SCI_ASSERT (len <= USB_MAX_TRANSFER_SIZE);/*assert verified*/

    //  ctl_ep2->ctrl.mBits.pack_size   = (len > USB_FIFO_MAX_BYTE)?USB_FIFO_MAX_BYTE:len ;
    if (len > 64)
    {
        ctl_ep->ctrl.mBits.pack_size = USB_FIFO_MAX_BYTE;
    }
    else
    {
        ctl_ep->ctrl.mBits.pack_size = len;
    }

    ctl_ep->transfer_size.mBits.num    = len;

    //Set data ready ;
    ctl_ep->ctrl.mBits.buf_ready  = 1;

    return 0;
}
/******************************************************************************/
//  Description:    SxUDC_Done
//  Global resource dependence:
//  Author:
//  Note:           Call this function when thre request is finished
/******************************************************************************/
LOCAL void SxUDC_Done (SX_EP_T *ep,
                       SX_REQUEST_T *req, int status)
{
    uint32 halted = ep->halted;

    if (NULL == req)
    {
        return;
    }

    SCI_DisableIRQ();
    list_del_init (&req->queue);
    req->req.status = REQ_IS_IDLE;

    if (ep->bEndpointAddress==1)
    {
        ep1_queue_count[1]--;
    }

    SCI_RestoreIRQ();
    ep->halted = 1;
    req->req.complete (&ep->ep, &req->req);
    ep->halted = halted;
}



/******************************************************************************/
//  Description:    SxUDC_WriteFIFO
//  Global resource dependence:
//  Author:
//  Note:           Call this function to send data by fifo not dma
/******************************************************************************/
LOCAL int SxUDC_WriteFIFO (SX_EP_T *ep, SX_REQUEST_T *req)
{
    USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_EPx_REG_T *ctrl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctrl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_DEV_REG_T *ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    int ep_id = ep->bEndpointAddress;
    uint32 len = req->req.length;
    uint8 *pBuf = req->req.buf;
    int size = ( (len + 3) >>2) & 0x3FF ;
    //int totallen;
    uint32 i;

    switch (ep_id)
    {
        case 3:
            ctrl_ep3->ctrl.mBits.pack_size      = len ;
            ctrl_ep3->transfer_size.mBits.num       = len ;
            //Clear fifo
            ctrl_usb->ep_rest.mBits.ep3_fifo_rst = TRUE;
            SxUDC_TxDWordToFIFO ( (uint32 *) pBuf ,size,USB_EP3);
            ctrl_ep3->ctrl.mBits.data_ready = 1;
            break;
        case 1:
            ctrl_ep1->ctrl.mBits.pack_size          = len ;
            ctrl_ep1->transfer_size.mBits.num       = len ;
            //Clear fifo
            ctrl_usb->ep_rest.mBits.ep1_fifo_rst = TRUE;
            SxUDC_TxDWordToFIFO ( (uint32 *) pBuf ,size,USB_EP1);
            ctrl_ep1->ctrl.mBits.data_ready = 1;
            break;
        case 0:

            if (len > 64)
            {
                ctrl_ep0->transfer_size_in.mBits.num    = len ;

                for (i=0; i< (len >> 6); i++)
                {
                    uint32 cur_time,start_time;
                    ctrl_ep0->ctrl.mBits.pack_size          = USB_FIFO_MAX_BYTE;
                    ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;
                    SxUDC_TxDWordToFIFO ( (uint32 *) (pBuf + (i<<6)), USB_FIFO_MAX_WORD, USB_EP0_IN);
                    ctrl_ep0->ctrl.mBits.data_ready = 1;
                    ep0_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
                    start_time = SCI_GetTickCount();

                    while (! (ctrl_ep0->int_sts.mBits.transaction_end))                 //wait until transfer end
                    {
                        if (!SxUDC_GetConnectState())
                        {
                            break;
                        }

                        cur_time = SCI_GetTickCount();

                        if ( (cur_time - start_time >2) || (cur_time < start_time))
                        {
                            SxUDC_VbusIrq (0);
                        }
                    }
                }

                if (len%64)
                {
                    ctrl_ep0->ctrl.mBits.pack_size          = (len - (i<<6));
                    ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;
                    SxUDC_TxDWordToFIFO ( (uint32 *) (pBuf + (i<<6)), ( ( ( (len- (i<<6)) + 3) >>2) & 0x3FF), USB_EP0_IN);
                    ctrl_ep0->ctrl.mBits.data_ready = 1;
                }
            }
            else
            {
                ctrl_ep0->ctrl.mBits.pack_size          = len ;
                ctrl_ep0->transfer_size_in.mBits.num    = len ;
                //Clear fifo
                ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;
                SxUDC_TxDWordToFIFO ( (uint32 *) pBuf ,size,USB_EP0_IN);
                ctrl_ep0->ctrl.mBits.data_ready = 1;
            }

            break;
        default:
            break ;
    }

    return SCI_SUCCESS;
}


/******************************************************************************/
//  Description:    SxUDC_ReadFIFO
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get data by fifo not dma
/******************************************************************************/
LOCAL int SxUDC_ReadFIFO (SX_EP_T *ep,
                          SX_REQUEST_T  *req)
{
    //SX_UDC_T *dev = the_udc_controller;
    //USB_EPx_REG_T *ctrl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctrl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    int ep_id = ep->bEndpointAddress;
    uint32 len = req->req.length;
    uint8 *pBuf = req->req.buf;
    int size = NULL ;
    int num  = NULL ;

    switch (ep_id)
    {
        case 4:

            /* for ubot upcc */
            {
                num = ctrl_ep4->receive_num.mBits.num;

                if (num)
                {
                    size = ( (num + 3) >>2) & 0xFF ;
                    SxUDC_RxDWordFromFIFO ( (uint32 *) pBuf ,size,USB_EP4);
                }
            }

            break;
        case 2:

            if (len)
            {
                size = ( (len + 3) >>2) & 0xFF ;
                SxUDC_RxDWordFromFIFO ( (uint32 *) pBuf ,size,USB_EP2);
            }

            num = len;

            break;
        case 0:
            num = len;
            size = ( (num + 3) >>2) & 0xFF ;
            ctrl_ep0->ctrl.mBits.pack_size = len;
            SxUDC_RxDWordFromFIFO ( (uint32 *) pBuf ,size,USB_EP0_OUT);
            ctrl_ep0->ctrl.mBits.buf_ready  = 1;
            break;
        default:
            num = NULL ;
            break ;
    }

    return num ;
}

/******************************************************************************/
//  Description:    SxUDC_ReadFIFOCrq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get control request data
/******************************************************************************/
LOCAL void SxUDC_ReadFIFOCrq (USB_CTRLREQUEST_T *crq)
{
    //char                tmpchr[30];
    USB_EP0_REG_T           *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;

    crq->bRequestType   = (ep0_ctrl->setup_low.mBits.direction&0x01) <<7 |
                          (ep0_ctrl->setup_low.mBits.type&0x03) <<5 |
                          (ep0_ctrl->setup_low.mBits.recipient&0x1F);
    crq->bRequest       = ep0_ctrl->setup_low.mBits.request;
    crq->wValue         = ep0_ctrl->setup_low.mBits.value;
    crq->wIndex         = ep0_ctrl->setup_high.mBits.index;
    crq->wLength        = ep0_ctrl->setup_high.mBits.length;
}


/******************************************************************************/
//  Description:    SxUDC_HandleEp0
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle ep0 irq
/******************************************************************************/
void SxUDC_Setup_Handler (USB_CTRLREQUEST_T *ctrl_req_ptr)
{
    USB_HWORD_VALUE_U value;

    value.wValue = ctrl_req_ptr->wValue;

    switch (ctrl_req_ptr->bRequestType & USB_TYPE_MASK)
    {
        case USB_TYPE_STANDARD:
            {
                switch (ctrl_req_ptr->bRequest)
                {
                    case USB_REQ_GET_DESCRIPTOR:
                        {
                            switch (value.bmDescriptor.type)
                            {
                                case USB_DEVICE_DESCRIPTOR_TYPE:
                                    //if(s_descriptor_type==0)
                                    //  s_descriptor_type = value.bmDescriptor.type;
                                    break;
                                case USB_CONFIGURATION_DESCRIPTOR_TYPE:

                                    if (s_descriptor_type==0)
                                    {
                                        s_descriptor_type = value.bmDescriptor.type;
                                    }

                                    break;
                                default:
                                    break;
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        case USB_TYPE_CLASS:
            break;
        case USB_TYPE_VENDOR:
            break;
        default:
            break;
    }
}
LOCAL void USB_EPnConfig (uint8 ep_no)
{

    USB_EPx_REG_T *ctl_ep = (USB_EPx_REG_T *) USB_BASE_EP (ep_no) ;

    if (ep_no==0)
    {
        return;
    }

    ctl_ep->ctrl.mBits.pack_size        = 0x40   ;
    ctl_ep->transfer_size.mBits.num     = 0x40 ;

    ctl_ep->ctrl.mBits.type             = 0 ;    // Bulk endpoint

    ctl_ep->ctrl.mBits.ep_en            = 1 ;

    ctl_ep->int_clr.dwValue             = 0x3fff ;
    ctl_ep->int_ctrl.dwValue = 0;
    ctl_ep->int_ctrl.mBits.transfer_end = 1 ;

    if (ep_no & BIT_0)
    {
        ctl_ep->int_ctrl.mBits.data_not_ready = 1 ;
    }
    else
    {
        ctl_ep->int_ctrl.mBits.buf_not_ready = 1 ;
    }

    ctl_ep->ctrl.mBits.data_ready  = 0;
    ctl_ep->ctrl.mBits.buf_ready =0 ;
}
void    USB_Configure (uint8 *usb_config_ptr)
{
    USB_COMMON_DESCRIPTOR_T *head;
    USB_ENDPOINT_DESCRIPTOR_T *ep;
    uint16  length= (usb_config_ptr[3]<<8) |usb_config_ptr[2];
    uint16  offset=0;


    while (offset < length)
    {
        head = (USB_COMMON_DESCRIPTOR_T *) (usb_config_ptr + offset);

        if (head->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE)
        {
            ep = (USB_ENDPOINT_DESCRIPTOR_T *) head;
            USB_EPnConfig (ep->bEndpointAddress&0x7F);
            // if(ep->bEndpointAddress & USB_DIR_IN)
            {
                //Sometimes EP3 transfer suspended because of loss of USB DMA request,
                //so we add timer to minitor EP3 transfer.
                USB_CreateTimer (ep->bEndpointAddress&0x7F,SxUDC_EPn_DMASoftRequest);
            }
        }

        offset += head->bLength;
    }
}
void SxUDC_USB_Configure (uint8 *buffer,uint32 size)
{
    uint16 descriptor_size =0;

    if (size == 0)
    {
        return;
    }

    switch (s_descriptor_type)
    {
        case USB_DEVICE_DESCRIPTOR_TYPE:
            descriptor_size = buffer[0];

            if (size != descriptor_size)
            {
                return;
            }

            if (s_descriptor_type == buffer[1])
            {
                s_usb_device_descriptor = buffer;
                s_descriptor_type = 0;
            }

            break;
        case USB_CONFIGURATION_DESCRIPTOR_TYPE:
            descriptor_size = buffer[2]| (buffer[3]<<8);

            if (size == descriptor_size)
            {
                if (s_descriptor_type == buffer[1])
                {
                    s_usb_config_descriptor = buffer;
                    USB_Configure (buffer);
                    s_descriptor_type=0xff;
                }
            }

            break;
        default:
            break;
    }
}
LOCAL void usb_setup_handle (SX_UDC_T *dev,USB_CTRLREQUEST_T       *crq_ptr)
{
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    dev->req_std = (crq_ptr->bRequestType & USB_TYPE_MASK) >>5;

    switch (dev->req_std)
    {
        case USB_REQ_STANDARD:
            dev->driver->setup (&dev->gadget, crq_ptr);
            break;
        case USB_REQ_CLASS:
            {
                dev->driver->setup (&dev->gadget, crq_ptr);

                if ( (crq_ptr->bRequestType == 0x22) &&
                        (crq_ptr->bRequest == 0x21) &&
                        (crq_ptr->wLength==0))
                {
                    ctl_ep0->ctrl.mBits.buf_ready  = 1;
                }
            }
            break;
        case USB_REQ_VENDOR:
            dev->driver->setup (&dev->gadget, crq_ptr);
            break;
        case USB_TYPE_RESERVED:
            dev->driver->setup (&dev->gadget, crq_ptr);
            break;
        default:
            break;
    }
}

#if 0
/******************************************************************************/
//  Description:    SxUDC_StartTransfer
//  Global resource dependence:
//  Parameters:     req: transfer request,
//                  ep:  endpoint to do the transfer
//  Note:           Call this function to start the transfer
/******************************************************************************/
LOCAL void SxUDC_StartTransfer (SX_REQUEST_T    *req,SX_EP_T *ep)
{
    if ( (req == NULL) || (ep == NULL))
    {
        return;
    }

    if ( (ep->bEndpointAddress & 0x7f) == 1)
    {
        USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;
        USB_ActiveTimer (ep->bEndpointAddress & 0x7F);

        if (req->req.dma==0)
        {
            if (req->req.length)
            {
                if (req->req.status == REQ_IN_QUEUE)
                {
                    ctl->int_ctrl.dwValue = 0;
                    req->req.status = REQ_IS_SENDING;
                    SxUDC_WriteFIFO (ep,req);
                    ctl->int_ctrl.mBits.transfer_end = 1;
                }
            }
            else if (req->req.length==0)
            {
                SxUDC_Done (ep, req,SCI_SUCCESS);
            }
        }
        else
        {
            if (req->req.status==REQ_IN_QUEUE)
            {
                SxUDC_EP1InDmaReq (req->req.buf, req->req.length);
                req->req.status = REQ_IS_SENDING;
            }

            ctl->int_ctrl.dwValue = 0;
            ctl->int_ctrl.mBits.transfer_end = 1;
        }
    }
    else if ( (ep->bEndpointAddress & 0x7f) == 2)
    {
        USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP2_BASE ;
        USB_ActiveTimer (ep->bEndpointAddress & 0x7F);

        if (req) /*lint !e774*/
        {
            if (req->req.dma==0)
            {
                ctl->ctrl.mBits.buf_ready  = 1;
            }
            else
            {

                SxUDC_EP2OutDmaReq ( (uint32 *) req->req.buf, (uint32) req->req.length);
                req->req.status = REQ_IS_SENDING;
            }
        }

        ctl->int_ctrl.mBits.buf_not_ready = 0 ;
        ctl->int_clr.mBits.buf_not_ready = 1 ;
        ctl->int_ctrl.mBits.transfer_end = 1;
    }
    else if ( (ep->bEndpointAddress & 0x7f) == 3)
    {
        USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP3_BASE ;

        if (req == NULL) /*lint !e774*/
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }
        else
        {
            USB_ActiveTimer (ep->bEndpointAddress & 0x7F);

            if (req->req.dma==0)
            {
                if (req->req.length)
                {
                    if (req->req.status == REQ_IN_QUEUE)
                    {
                        ctl->int_ctrl.dwValue = 0;
                        req->req.status = REQ_IS_SENDING;
                        SxUDC_WriteFIFO (ep,req);
                        ctl->int_ctrl.mBits.transfer_end = 1;
                        record_usb_trace3 ( (0xBB<<24) | (req->req.length<<8) |req->req.status); /*lint !e648  !e737*/
                    }
                }
                else if (req->req.length==0)
                {
                    SxUDC_Done (ep, req,SCI_SUCCESS);
                }
            }
            else
            {
                ctl->int_ctrl.mBits.transfer_end = 0;

                if (req->req.status==REQ_IN_QUEUE)
                {
                    SxUDC_EP3InDmaReq (req->req.buf, req->req.length);
                    req->req.status = REQ_IS_SENDING;
                    record_usb_trace3 ( (0xCC<<24) | (req->req.length<<8) |req->req.status); /*lint !e648 !e737*/
                }

                ctl->int_ctrl.dwValue = 0;
                ctl->int_ctrl.mBits.transfer_end = 1;
            }
        }
    }
    else if ( (ep->bEndpointAddress & 0x7f) == 4)
    {
        USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP4_BASE ;

        if (req->req.dma==0)
        {
            ctl->ctrl.mBits.buf_ready  = 1;
        }
        else
        {
            SxUDC_EP4OutDmaReq ( (uint32 *) req->req.buf, (uint32) req->req.length);
        }

        ctl->int_ctrl.mBits.buf_not_ready = 0 ;
        ctl->int_clr.mBits.buf_not_ready = 1 ;
        ctl->int_ctrl.mBits.transfer_end = 1;
    }
}
#endif
LOCAL void SxUDC_HandleEp0 (SX_UDC_T *dev ,uint32 int_status)
{
    SX_REQUEST_T            *req;
    USB_CTRLREQUEST_T       crq;
    SX_EP_T         *ep = &dev->ep[0];

    USB_EPx_INT_U status ;

    status.dwValue = int_status;


    if (list_empty (&ep->queue))
    {
        req = NULL;
    }
    else
    {
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);    /*lint !e413 */
    }

    if (status.mBits.setup_end)
    {
        SxUDC_ReadFIFOCrq (&crq);

        s_setup_packet_backup[0] = ( (uint32 *) &crq) [0];
        s_setup_packet_backup[1] = ( (uint32 *) &crq) [1];

        dev->req_std = (crq.bRequestType & USB_TYPE_MASK);

        if (crq.wLength)
        {
            if (crq.bRequestType & USB_DIR_IN)
            {
                dev->ep0state = EP0_IN_DATA_PHASE;
            }
            else
            {
                dev->ep0state = EP0_OUT_DATA_PHASE;
            }
        }
        else
        {
            dev->ep0state = EP0_IDLE;
        }

        SxUDC_Setup_Handler (&crq);
        usb_setup_handle (dev, &crq);
        //dev->driver->setup (&dev->gadget, &crq);
    }


    if ( (status.mBits.transfer_end))
    {
        if (req)
        {
            if (dev->ep0state == EP0_OUT_DATA_PHASE)
            {
                SxUDC_ReadFIFO (ep,req);
            }

            SxUDC_Done (ep, req,SCI_SUCCESS);
        }

        dev->ep0state = EP0_IDLE;
    }


    /* clear ep0 interrupt */
}

/******************************************************************************/
//  Description:    SxUDC_HandleEp1
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle ep1 irq
/******************************************************************************/
LOCAL void SxUDC_HandleEp1 (SX_EP_T *ep ,uint32 int_status)
{
    SX_REQUEST_T    *req;
    SX_UDC_T *dev = the_udc_controller;
    USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_INT_U status ;

    status.dwValue = int_status;


    if (status.mBits.in_timeout)
    {
        SxUDC_ErrorHandle (USB_ERR_INACK_TIMEOUT);
    }

    if (list_empty (&ep->queue))
    {
        req = NULL;
    }
    else
    {
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);   /*lint !e413 */
    }

    if (status.mBits.transfer_end)
    {
        USB_SetEPIdle (1);

        if (req == NULL)
        {
            ;
        }
        else
        {

            ctl->ctrl.mBits.data_ready = 0;
            ctl->int_ctrl.mBits.data_not_ready = 0;

            //ctl->int_ctrl.mBits.transfer_end = 1;
            if (req->req.dma)
            {
                req->req.dma = 0; /*lint !e613 */
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
                DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP1],FALSE);
#elif defined PLATFORM_SC6800H
                DMA_HAL_ChnEnable (DMA_USB_EP1,FALSE);
#endif
            }

            USB_StopTimer (ep->bEndpointAddress & 0x7F);
            SxUDC_Done (ep, req,SCI_SUCCESS);
            record_usb_trace3 ( (0xCC<<24) | (req->req.length<<8) |ep1_queue_count[1]); /*lint !e648 */
            ctl->int_ctrl.mBits.data_not_ready = 1;
        }
    }

    if (status.mBits.data_not_ready)
    {

        if (req == NULL)
        {

            USB_CTRLREQUEST_T       crq;
            ctl->int_ctrl.mBits.data_not_ready = 0;
            crq.bRequestType = USB_TYPE_RESERVED;
            crq.bRequest = 1;
            crq.wIndex = USB_EP1;
            usb_setup_handle (dev, &crq);

        }
        else
        {


            if (req->req.dma==0)
            {
                if (req->req.length)
                {
                    if (req->req.status == REQ_IN_QUEUE)
                    {
                        ctl->int_ctrl.dwValue = 0;
                        req->req.status = REQ_IS_SENDING;
                        SxUDC_WriteFIFO (ep,req);
                        ctl->int_ctrl.mBits.transfer_end = 1;

                    }
                }
                else if (req->req.length==0)
                {
                    SxUDC_Done (ep, req,SCI_SUCCESS);
                }
            }
            else
            {

                //ctl->int_ctrl.mBits.transfer_end = 0;
                if (req->req.status==REQ_IN_QUEUE)
                {
                    SxUDC_EP1InDmaReq (req->req.buf, req->req.length);
                    req->req.status = REQ_IS_SENDING;
                }

                ctl->int_ctrl.dwValue = 0;
                ctl->int_ctrl.mBits.transfer_end = 1;
            }
        }
    }
}
uint32 usb_ep2_crc_err_time = 0;
uint32 read_fifo[16]={0};

/******************************************************************************/
//  Description:    SxUDC_HandleEp2
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle ep2 irq
/******************************************************************************/
LOCAL void SxUDC_HandleEp2 (SX_EP_T *ep ,uint32 int_status)
{
    SX_REQUEST_T    *req;
    // SX_UDC_T *dev = the_udc_controller;
    USB_EPx_REG_T *ctl_ep = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_INT_U status ;

    status.dwValue = int_status;//ctl->int_sts.dwValue ;

    if (list_empty (&ep->queue))
    {
        req = NULL;
    }
    else
    {
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);             /*lint !e413 */
    }

    if (status.mBits.in_timeout)
    {
        SxUDC_ErrorHandle (USB_ERR_INACK_TIMEOUT);
    }

    if (status.mBits.crc16_err)
    {
        usb_ep2_crc_err_time ++;

        SxUDC_RxDWordFromFIFO ( (uint32 *) read_fifo ,16,USB_EP2);
		
    }
    if (status.mBits.buf_not_ready)
    {
        if (req)
        {
            if (req->req.dma==0)
            {
                ctl_ep->ctrl.mBits.buf_ready  = 1;
            }
            else
            {
                SxUDC_EP2OutDmaReq ( (uint32 *) req->req.buf, (uint32) req->req.length);
                req->req.status = REQ_IS_SENDING;
            }
            USB_ActiveTimer (ep->bEndpointAddress & 0x7F);
            ctl_ep->int_ctrl.mBits.transfer_end = 1;
        }

        ctl_ep->int_ctrl.mBits.buf_not_ready = 0 ;
        ctl_ep->int_clr.mBits.buf_not_ready = 1 ;
    }

    if (status.mBits.transfer_end)
    {
        USB_SetEPIdle (2);
        USB_StopTimer (ep->bEndpointAddress & 0x7F);

        if (req)
        {
            req->req.length = ctl_ep->receive_num.mBits.num;
            req->req.actual = ctl_ep->receive_num.mBits.num;

            if (req->req.actual)
            {
                if (req->req.dma==0)
                {
                    SxUDC_ReadFIFO (ep,req);
                }
                else
                {
                    req->req.dma = 0; /*lint !e613 */
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
                    //DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP2],FALSE);
#elif defined PLATFORM_SC6800H
                    DMA_HAL_ChnEnable (DMA_USB_EP2,FALSE);
#endif
                }

                ctl_ep->ctrl.mBits.buf_ready  = 0;
                SxUDC_Done (ep, req,SCI_SUCCESS);
            }
            else
            {
                ctl_ep->ctrl.mBits.buf_ready  = 1;
                ctl_ep->int_ctrl.mBits.transfer_end = 1;
                ctl_ep->int_ctrl.mBits.buf_not_ready = 0;
            }
        }
        else
        {
              SCI_ASSERT (0); /*assert to do*/
        }
    }
}

uint32 g_ep3_intimeout_cnt = 0;
/******************************************************************************/
//  Description:    SxUDC_HandleEp3
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle ep3 irq
/******************************************************************************/
LOCAL void SxUDC_HandleEp3 (SX_EP_T *ep ,uint32 int_status)
{
    SX_REQUEST_T    *req;
    // SX_UDC_T *dev = the_udc_controller;
    USB_EPx_INT_U status ;
    USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP3_BASE ;
    status.dwValue = int_status;//ctl->int_sts.dwValue ;

    if (list_empty (&ep->queue))
    {
        UDC_DEBUG ("sx_udc_handle_ep2() EP queue is empty");
        req = NULL;
    }
    else
    {
        UDC_DEBUG ("sx_udc_handle_ep2() EP queue is inempty");
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);             /*lint !e413 */
    }

    if (status.mBits.in_timeout)
    {
        ctl->int_ctrl.mBits.in_timeout = 0;
        g_ep3_intimeout_cnt ++;
        SxUDC_Done (ep, req,SCI_SUCCESS);
    }


    if (status.mBits.transfer_end)
    {
        USB_SetEPIdle (3);

        if (req == NULL)
        {
            ctl->int_clr.mBits.transfer_end = 1;
        }
        else
        {

            ctl->ctrl.mBits.data_ready = 0;
            ctl->int_ctrl.mBits.data_not_ready = 0;
            ctl->int_ctrl.mBits.transfer_end = 0;

            if (req->req.dma)
            {
                req->req.dma=0; /*lint !e613 */

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
                DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP3],FALSE);
#elif defined PLATFORM_SC6800H
                DMA_HAL_ChnEnable (DMA_USB_EP3,FALSE);
#endif
            }

            record_usb_trace3 ( (0xDD<<24) | (req->req.length<<8) |req->req.status); /*lint !e648   !e737*/
            //Sometimes EP3 transfer suspended because of loss of USB DMA request,
            //so we add timer to minitor EP3 transfer.
            USB_StopTimer (ep->bEndpointAddress & 0x7F);
            SxUDC_Done (ep, req,SCI_SUCCESS);


        }
    }

    if (status.mBits.data_not_ready)
    {

        if (req == NULL)
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }
        else
        {

            if (req->req.dma==0)
            {
                if (req->req.length)
                {
                    if (req->req.status == REQ_IN_QUEUE)
                    {
                        ctl->int_ctrl.dwValue = 0;

                        req->req.status = REQ_IS_SENDING;
                        SxUDC_WriteFIFO (ep,req);
                        ctl->int_ctrl.mBits.transfer_end = 1;
                        record_usb_trace3 ( (0xBB<<24) | (req->req.length<<8) |req->req.status); /*lint !e648  !e737*/
                    }

                   // ctl->int_ctrl.mBits.in_timeout = 1 ;
                }
                else if (req->req.length==0)
                {
                    SxUDC_Done (ep, req,SCI_SUCCESS);
                }
            }
            else
            {
                ctl->int_ctrl.mBits.transfer_end = 0;


                if (req->req.status==REQ_IN_QUEUE)
                {
                    //Sometimes EP3 transfer suspended because of loss of USB DMA request,
                    //so we add timer to minitor EP3 transfer.
                    //USB_ActiveTimer (ep->bEndpointAddress & 0x7F);
                    SxUDC_EP3InDmaReq (req->req.buf, req->req.length);
                    req->req.status = REQ_IS_SENDING;
                    record_usb_trace3 ( (0xCC<<24) | (req->req.length<<8) |req->req.status); /*lint !e648 !e737*/
                }

                ctl->int_ctrl.dwValue = 0;
                ctl->int_ctrl.mBits.transfer_end = 1;
            }
        }
    }
}
uint32 g_ep4_outimeout_cnt = 0;
/******************************************************************************/
//  Description:    SxUDC_HandleEp4
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle ep4 irq
/******************************************************************************/
LOCAL void SxUDC_HandleEp4 (SX_EP_T *ep ,uint32 int_status)
{
    SX_REQUEST_T    *req;
    // SX_UDC_T *dev = the_udc_controller;
    USB_EPx_REG_T *ctl_ep = (USB_EPx_REG_T *) USB_EP4_BASE ;
    USB_EPx_INT_U status ;
    status.dwValue = int_status;



    if (list_empty (&ep->queue))
    {
        req = NULL;
    }
    else
    {
        req = list_entry (ep->queue.next, SX_REQUEST_T, queue);             /*lint !e413 */
    }

    if (status.mBits.out_timeout)
    {
        ctl_ep->int_ctrl.mBits.out_timeout = 0;
        g_ep4_outimeout_cnt ++;
        SxUDC_Done (ep, req,SCI_SUCCESS);
    }

    if (status.mBits.buf_not_ready)
    {

        if (req)
        {
            if (req->req.dma==0)
            {

                ctl_ep->ctrl.mBits.buf_ready  = 1;
            }
            else
            {

                SxUDC_EP4OutDmaReq ( (uint32 *) req->req.buf, (uint32) req->req.length);
            }

            ctl_ep->int_ctrl.mBits.transfer_end = 1;
            ctl_ep->int_ctrl.mBits.out_timeout = 1;
        }

        ctl_ep->int_ctrl.mBits.buf_not_ready = 0 ;
        ctl_ep->int_clr.mBits.buf_not_ready = 1 ;
    }

    if (status.mBits.transfer_end)
    {
        USB_SetEPIdle (4);

        if (req)
        {
            req->req.length = ctl_ep->receive_num.mBits.num;
            req->req.actual = ctl_ep->receive_num.mBits.num;

            if (req->req.actual)
            {
                if (req->req.dma==0)
                {
                    SxUDC_ReadFIFO (ep,req);
                }

                ctl_ep->ctrl.mBits.buf_ready  = 0;
                SxUDC_Done (ep, req,SCI_SUCCESS);
            }
            else
            {
                ctl_ep->ctrl.mBits.buf_ready  = 1;
                ctl_ep->int_ctrl.mBits.transfer_end = 1;
                ctl_ep->int_ctrl.mBits.buf_not_ready = 0;
            }
        }
        else
        {
            SCI_ASSERT (0); /*assert to do*/
        }
    }
}

/******************************************************************************/
//  Description:    SxUDC_ClearAllInt
//  Global resource dependence:
//  Author:
//  Note:           Call this function to clear all the interrupt status bits
/******************************************************************************/
LOCAL void SxUDC_ClearAllInt (void)
{
    USB_DEV_REG_T *usb_ctrl_ptr = (USB_DEV_REG_T *) USB_REG_BASE;
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

    //clear all endp interrupt sts
    ctl_ep0->int_clr.dwValue  = USB_INT_EPx_MASK;
    ctl_ep1->int_clr.dwValue  = USB_INT_EPx_MASK;
    ctl_ep2->int_clr.dwValue  = USB_INT_EPx_MASK;
    ctl_ep3->int_clr.dwValue  = USB_INT_EPx_MASK;
    ctl_ep4->int_clr.dwValue  = USB_INT_EPx_MASK;
    usb_ctrl_ptr->int_clr.dwValue = USB_INT_DEV_MASK ;
}

/******************************************************************************/
//  Description:    SxUDC_Irq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle udc irq
/******************************************************************************/
static ISR_EXE_T Sx_UDC_ISR_Handler (uint32 param)
{
    //    xSignalHeader signal;
    USB_ISR_MSG_T   msg;
    SX_UDC_T *dev = &memory;

    USB_DEV_REG_T   *usb_ctrl_ptr = (USB_DEV_REG_T *) USB_REG_BASE;
    USB_DEV_INT_STS_U  int_sts;

    ISR_EXE_T result = ISR_DONE;

    // mark cable connect
    if (FALSE == SxUDC_GetConnectState())
    {
        SxUDC_Connect ();
    }

    if (FALSE == SxUDC_GetHostReq())
    {

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        return CALL_HISR;//return here, otherwise donot pop up USB application list on LCD
    }

    /* Driver not connected */
    if (!dev->driver)
    {
        /* Clear interrupts */
        // has interrupt status
        if (usb_ctrl_ptr->int_sts.dwValue)
        {
            //cleart all usb int
            SxUDC_ClearAllInt();
        }

        return ISR_DONE;
    }

    int_sts.dwValue = usb_ctrl_ptr->int_sts.dwValue;

    /* RESET */
    TM_SendTestPointRequest (* (uint32 *) (0x80000004),int_sts.dwValue);

    if (int_sts.mBits.host_rst)
    {
        UDC_DEBUG ("\nRESET INTERRUPT\n");
        /* two kind of reset :
         * - reset start -> pwr reg = 8
         * - reset end   -> pwr reg = 0
         **/

        dev->gadget.speed = USB_SPEED_UNKNOWN;
        dev->address = 0;

        dev->ep0state = EP0_IDLE;
        dev->gadget.speed = USB_SPEED_FULL;
        usb_ctrl_ptr->int_clr.mBits.host_rst = 1;
        return ISR_DONE;
    }

    /* RESUME */
    if (int_sts.mBits.resume)
    {
        UDC_DEBUG ("\nRESUME INTERRUPT\n");
        usb_ctrl_ptr->int_clr.mBits.resume = 1;

        if (dev->gadget.speed != USB_SPEED_UNKNOWN
                && dev->driver
                && dev->driver->resume)
        {
            dev->driver->resume (&dev->gadget);
        }
    }

    /* SUSPEND */
    if (int_sts.mBits.suspend)
    {
        /* clear interrupt */
        UDC_DEBUG ("\nSUSPEND INTERRUPT\n");
        usb_ctrl_ptr->int_clr.mBits.suspend = 1;

        if (dev->gadget.speed != USB_SPEED_UNKNOWN
                && dev->driver
                && dev->driver->suspend)
        {
            dev->driver->suspend (&dev->gadget);
        }

        dev->ep0state = EP0_IDLE;
    }

    /* EP0 */
    if (int_sts.mBits.ep0)
    {
        USB_EP0_REG_T *ctl = (USB_EP0_REG_T *) USB_EP0_BASE ;
        msg.endpoint = Sx_UDC_EP0;
        msg.int_status = ctl->int_sts.dwValue;
        ctl->int_clr.dwValue = msg.int_status;
        CHIPDRV_DisableIRQINT (TB_USB_INT);
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        IsrWriteBuffer (msg);
        result=CALL_HISR;
    }

    if (int_sts.mBits.ep1)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;
        uint32  status;

        status = ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_13)
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP1;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        IsrWriteBuffer (msg);
        result=CALL_HISR;
        CHIPDRV_DisableIRQINT (TB_USB_INT);
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep2)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP2_BASE ;
        uint32  status;

        status= ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_12)
        {
            ctl->int_ctrl.mBits.buf_not_ready = 0;
        }

        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        msg.endpoint = Sx_UDC_EP2;
        IsrWriteBuffer (msg);
        result=CALL_HISR;
        CHIPDRV_DisableIRQINT (TB_USB_INT);
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep3)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP3_BASE ;
        uint32  status;

        status = ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_13)
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP3;

        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        IsrWriteBuffer (msg);
        result=CALL_HISR;
        CHIPDRV_DisableIRQINT (TB_USB_INT);
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep4)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP4_BASE ;
        uint32  status;

        status= ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_12)
        {
            ctl->int_ctrl.mBits.buf_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP4;

        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);

        IsrWriteBuffer (msg);
        result=CALL_HISR;
        CHIPDRV_DisableIRQINT (TB_USB_INT);
        ctl->int_clr.dwValue = status;
    }

    /*add other endpoint here*/

    return result;
}


LOCAL void Sx_UDC_EPnHandle (USB_ISR_MSG_T *msg)
{
    SX_UDC_T *dev = &memory;


    switch (msg->endpoint)
    {
        case Sx_UDC_EP0:
            SxUDC_HandleEp0 (dev,msg->int_status);
            CHIPDRV_EnableIRQINT (TB_USB_INT);
            break;
        case Sx_UDC_EP1:
            SxUDC_HandleEp1 (&dev->ep[1],msg->int_status);
            CHIPDRV_EnableIRQINT (TB_USB_INT);
            break;
        case Sx_UDC_EP2:
            SxUDC_HandleEp2 (&dev->ep[2],msg->int_status);
            CHIPDRV_EnableIRQINT (TB_USB_INT);
            break;
        case Sx_UDC_EP3:
            SxUDC_HandleEp3 (&dev->ep[3],msg->int_status);
            CHIPDRV_EnableIRQINT (TB_USB_INT);
            break;
        case Sx_UDC_EP4:
            SxUDC_HandleEp4 (&dev->ep[4],msg->int_status);
            CHIPDRV_EnableIRQINT (TB_USB_INT);
            break;
        default:
            break;
    }
}

LOCAL void  SxUDCHisrFunc (uint32 i, void *d)
{
    xSignalHeader signal;
    USB_ISR_MSG_T msg;

    if (FALSE == SxUDC_GetHostReq())
    {

        SxUDC_SetHostReq (TRUE);

        // send to upper level notify cable connect
        signal = SCI_ALLOC_APP (sizeof (*signal));
        signal->SignalCode = USB_HOST_PLUG_IN;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
        SxUDC_ClearAllInt();
        UDC_HWUNInit();


    }

    while (!threadReadBuffer (&msg))
    {
        Sx_UDC_EPnHandle (&msg);
    }
}

#if defined(PLATFORM_SC6531EFM)	|| defined(PLATFORM_UWS6121E)
LOCAL void  SxUDCVbusFunc (void)
{
    xSignalHeader signal;
    USB_ISR_MSG_T msg;

    // send to upper level notify cable connect
     signal = SCI_ALLOC_APP (sizeof (*signal));
     signal->SignalCode = USB_HOST_PLUG_IN;
     signal->SignalSize = sizeof (*signal);
     signal->Sender = SCI_IdentifyThread();
     SCI_SendSignal (signal, usb_isr_id);
     SxUDC_ClearAllInt();
     UDC_HWUNInit();
}

/*****************************************************************************/
//  Function name:	Sx_UDC_SendSignal
//  Description:  This function send the signal to sx udc task
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Sx_UDC_SendSignal(USB_ISR_MSG_T msg)
{
	SX_UDC_TASK_MSG_T *sx_udc_task_sig_ptr    =  SCI_NULL;	

	TM_SendTestPointRequest (msg.endpoint,10241008);
	TM_SendTestPointRequest (msg.int_status,10241009);
	//UDC_DEBUG("\nSx_UDC_SendSignal\n");

	SCI_CREATE_SIGNAL(
        sx_udc_task_sig_ptr,
        msg.endpoint,
        sizeof (SX_UDC_TASK_MSG_T),
        SCI_IDENTIFY_THREAD());
	sx_udc_task_sig_ptr->int_status = msg.int_status;
	SCI_SEND_SIGNAL((xSignalHeaderRec *)sx_udc_task_sig_ptr, s_udc_task_id);  	 
}
/******************************************************************************/
//  Description:    SxUDC_Irq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle udc irq
/******************************************************************************/
LOCAL ISR_EXE_T Sx_UDC_ISR_Handler_Ex (uint32 param)
{
    xSignalHeader signal;
    USB_ISR_MSG_T   msg;
    SX_UDC_T *dev = &memory;

    USB_DEV_REG_T   *usb_ctrl_ptr = (USB_DEV_REG_T *) USB_REG_BASE;
    USB_DEV_INT_STS_U  int_sts;

    ISR_EXE_T result = ISR_DONE;

    TM_SendTestPointRequest (0x1,10240947);
    // mark cable connect
    if (FALSE == SxUDC_GetConnectState())
    {
        SxUDC_Connect ();
    }

    if (FALSE == SxUDC_GetHostReq())
    {
        CHIPDRV_DisableIRQINT (TB_USB_INT);

        SxUDC_SetHostReq (TRUE);
        msg.endpoint = Sx_HOST_PLUG_IN;
        Sx_UDC_SendSignal (msg);

        return ISR_DONE;//return here, otherwise donot pop up USB application list on LCD
    }

    /* Driver not connected */
    if (!dev->driver)
    {
        /* Clear interrupts */
        // has interrupt status
        if (usb_ctrl_ptr->int_sts.dwValue)
        {
            //cleart all usb int
            SxUDC_ClearAllInt();
        }

        return ISR_DONE;
    }

    int_sts.dwValue = usb_ctrl_ptr->int_sts.dwValue;

    /* RESET */
    TM_SendTestPointRequest (* (uint32 *) (0x80000004),int_sts.dwValue);
    TM_SendTestPointRequest (0x2,10240948);

    if (int_sts.mBits.host_rst)
    {
        UDC_DEBUG ("\nRESET INTERRUPT\n");
        /* two kind of reset :
         * - reset start -> pwr reg = 8
         * - reset end   -> pwr reg = 0
         **/

        dev->gadget.speed = USB_SPEED_UNKNOWN;
        dev->address = 0;

        dev->ep0state = EP0_IDLE;
        dev->gadget.speed = USB_SPEED_FULL;
        usb_ctrl_ptr->int_clr.mBits.host_rst = 1;
        return ISR_DONE;
    }

    /* RESUME */
    if (int_sts.mBits.resume)
    {
        UDC_DEBUG ("\nRESUME INTERRUPT\n");
        usb_ctrl_ptr->int_clr.mBits.resume = 1;

        if (dev->gadget.speed != USB_SPEED_UNKNOWN
                && dev->driver
                && dev->driver->resume)
        {
            dev->driver->resume (&dev->gadget);
        }
    }

    /* SUSPEND */
    if (int_sts.mBits.suspend)
    {
        /* clear interrupt */
        UDC_DEBUG ("\nSUSPEND INTERRUPT\n");
        usb_ctrl_ptr->int_clr.mBits.suspend = 1;

        if (dev->gadget.speed != USB_SPEED_UNKNOWN
                && dev->driver
                && dev->driver->suspend)
        {
            dev->driver->suspend (&dev->gadget);
        }

        dev->ep0state = EP0_IDLE;
    }

    /* EP0 */
    if (int_sts.mBits.ep0)
    {
        USB_EP0_REG_T *ctl = (USB_EP0_REG_T *) USB_EP0_BASE ;

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        msg.endpoint = Sx_UDC_EP0;
        msg.int_status = ctl->int_sts.dwValue;
        ctl->int_clr.dwValue = msg.int_status;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        Sx_UDC_SendSignal (msg);
        result=ISR_DONE;
    }

    if (int_sts.mBits.ep1)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;
        uint32  status;

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        status = ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_13)
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP1;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        Sx_UDC_SendSignal (msg);
        result=ISR_DONE;
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep2)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP2_BASE ;
        uint32  status;

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        status= ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_12)
        {
            ctl->int_ctrl.mBits.buf_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP2;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        Sx_UDC_SendSignal (msg);
        result=ISR_DONE;
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep3)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP3_BASE ;
        uint32  status;

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        status = ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_13)
        {
            ctl->int_ctrl.mBits.data_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP3;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        Sx_UDC_SendSignal (msg);
        result=ISR_DONE;
        ctl->int_clr.dwValue = status;

    }

    if (int_sts.mBits.ep4)
    {
        volatile USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP4_BASE ;
        uint32  status;

        CHIPDRV_DisableIRQINT (TB_USB_INT);

        status= ctl->int_sts.dwValue;
        msg.int_status  =  status & ctl->int_ctrl.dwValue;

        if (status & BIT_12)
        {
            ctl->int_ctrl.mBits.buf_not_ready = 0;
        }

        msg.endpoint = Sx_UDC_EP4;
        TM_SendTestPointRequest (ctl->int_ctrl.dwValue,msg.int_status);
        Sx_UDC_SendSignal (msg);
        result=ISR_DONE;
        ctl->int_clr.dwValue = status;
    }

    /*add other endpoint here*/

    return result;
}

LOCAL void _SX_UDC_SrvThread (uint32 argc, void *argv)
{
    SX_UDC_TASK_MSG_T *	signal;
    SX_UDC_T *dev = &memory;    

    for(;;)
    {
        signal = (SX_UDC_TASK_MSG_T *) SCI_GetSignal (s_udc_task_id);
       
        TM_SendTestPointRequest (signal->SignalCode,10240956);//endpoint
        TM_SendTestPointRequest (signal->int_status,10240957);//int_status
        switch (signal->SignalCode)
        {
            case Sx_UDC_EP0:
                SxUDC_HandleEp0 (dev,signal->int_status);
                break;
            case Sx_UDC_EP1:                
                SxUDC_HandleEp1 (&dev->ep[1],signal->int_status);
                break;
            case Sx_UDC_EP2:
                SxUDC_HandleEp2 (&dev->ep[2],signal->int_status);
                break;
            case Sx_UDC_EP3:
                SxUDC_HandleEp3 (&dev->ep[3],signal->int_status);
                break;
            case Sx_UDC_EP4:
                SxUDC_HandleEp4 (&dev->ep[4],signal->int_status);
                break;
	        case Sx_HOST_PLUG_IN:
                SxUDCVbusFunc();
                break;
            default:
                break;
        }
        TM_SendTestPointRequest (s_udc_task_id,10240958);
        CHIPDRV_EnableIRQINT (TB_USB_INT);
        
        SCI_FREE(signal);
        signal = NULL;
    }
}
#endif
/******************************************************************************/
//  Description:    to_sx_ep
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get sx_ep address from usb_ep address
/******************************************************************************/
LOCAL SX_EP_T *to_sx_ep (USB_EP_T *ep)
{
    return CONTAINER_OF (ep, SX_EP_T, ep);                              /*lint !e413 */
}

/******************************************************************************/
//  Description:    to_sx_udc
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get sx_udc address from usb_gadget address
/******************************************************************************/
LOCAL SX_UDC_T *to_sx_udc (USB_GADGET_T *gadget)
{
    return CONTAINER_OF (gadget, SX_UDC_T, gadget);                     /*lint !e413 */
}

/******************************************************************************/
//  Description:    to_sx_req
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get sx_request address from usb_request address
/******************************************************************************/
LOCAL SX_REQUEST_T *to_sx_req (USB_REQUEST_T *req)
{
    return CONTAINER_OF (req, SX_REQUEST_T, req);                       /*lint !e413 */
}

/******************************************************************************/
//  Description:    SxUDC_EpEnable
//  Global resource dependence:
//  Author:
//  Note:           Call this function to enable endpoint.
/******************************************************************************/
LOCAL int SxUDC_EpEnable (USB_EP_T *_ep,
                          const USB_DT_ENDPOINT_DESCRIPTOR_T *desc)
{
    return 0;
}


/******************************************************************************/
//  Description:    SxUDC_EpDisable
//  Global resource dependence:
//  Author:
//  Note:           Call this function to disable endpoint.
/******************************************************************************/
LOCAL int SxUDC_EpDisable (USB_EP_T *_ep)
{
    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_AllocRequest
//  Global resource dependence:
//  Author:
//  Note:           Call this function to alloc one usb request.
/******************************************************************************/
LOCAL USB_REQUEST_T *SxUDC_AllocRequest (USB_EP_T *_ep)
{
    SX_REQUEST_T *req;
//    SX_EP_T *ep;

    if (!_ep)
    {
        return NULL;
    }

    req = SCI_ALLOC_APP (sizeof (SX_REQUEST_T));

    if (!req)
    {
        return NULL;
    }

    SCI_MEMSET (req, 0, sizeof (SX_REQUEST_T));

    _INIT_LIST_HEAD (&req->queue);

//    ep = to_sx_ep (_ep);

    return & (req->req);
}

/******************************************************************************/
//  Description:    SxUDC_FreeRequest
//  Global resource dependence:
//  Author:
//  Note:           Call this function to free one usb request.
/******************************************************************************/
LOCAL void SxUDC_FreeRequest (USB_EP_T *_ep, USB_REQUEST_T *_req)
{
    SX_EP_T         *ep = NULL;
    SX_REQUEST_T    *req = NULL;

    if (!_ep || !_req)
    {
        return;
    }

    ep = to_sx_ep (_ep);
    req = to_sx_req (_req);


    if (!ep || !req)
    {
        return;
    }

    if (list_empty (&req->queue))
    {
        SCI_FREE (req);
    }
    else
    {
        UDC_DEBUG ("\nsx_udc_free_request() queue not empty. can not release\n");
    }
}

/******************************************************************************/
//  Description:    SxUDC_FIFOStatus
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get udc fifo status
/******************************************************************************/
LOCAL int SxUDC_FIFOStatus (USB_EP_T *_ep)
{
    return s_fifo_blocking;
}

/******************************************************************************/
//  Description:    SxUDC_ReadLoop
//  Global resource dependence:
//  Author:
//  Note:           Call this function to read fifo without interrupt
/******************************************************************************/
LOCAL void SxUDC_ReadLoop (USB_EP_T *_ep, USB_REQUEST_T *_req)
{
    USB_DEV_REG_T   *usb_ctrl_ptr = (USB_DEV_REG_T *) USB_REG_BASE;
    SX_UDC_T        *dev = the_udc_controller;
    USB_DEV_REG_T     *ctl    = (USB_DEV_REG_T *) USB_REG_BASE;
    //USB_EP0_REG_T   *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;
    //SX_REQUEST_T    *req = to_sx_req (_req);
    //SX_EP_T         *ep = to_sx_ep (_ep);
    //USB_CTRLREQUEST_T   crq;

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    if (REG32(INT1_IRQ_RAW_STS) & (BIT_5))
#elif  1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)  /*暂时打桩*/
    if (INT_PHY_GetIrqIdStatus(TB_USB_INT))
#else
    if (REG32 (INT_IRQ_RAW_STS) & (1<<TB_USB_INT))
#endif
    {
        if (usb_ctrl_ptr->int_sts.dwValue  & USB_INT_EPx_MASK)
        {
            if (usb_ctrl_ptr->int_sts.mBits.ep0)
            {
                USB_EP0_REG_T   *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;
                SxUDC_HandleEp0 (dev,ep0_ctrl->int_sts.dwValue);
            }

            if (usb_ctrl_ptr->int_sts.mBits.ep1)
            {
                USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
                SxUDC_HandleEp1 (&dev->ep[1],ctl_ep1->int_sts.dwValue);
            }

            if (usb_ctrl_ptr->int_sts.mBits.ep2)
            {
                USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
                SxUDC_Queue (_ep, _req);
                SxUDC_HandleEp2 (&dev->ep[2],ctl_ep2->int_sts.dwValue);
            }

            if (usb_ctrl_ptr->int_sts.mBits.ep3)
            {
                USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
                SxUDC_HandleEp3 (&dev->ep[3],ctl_ep3->int_sts.dwValue);
            }

            if (usb_ctrl_ptr->int_sts.mBits.ep4)
            {
                USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
                SxUDC_Queue (_ep, _req);
                SxUDC_HandleEp4 (&dev->ep[4],ctl_ep4->int_sts.dwValue);
            }

            ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
        }
    }

    return;
}


/******************************************************************************/
//  Description:    SxUDC_WriteLoop
//  Global resource dependence:
//  Author:
//  Note:           Call this function to write fifo without interrupt
/******************************************************************************/
LOCAL void SxUDC_WriteLoop (USB_EP_T *_ep, USB_REQUEST_T *_req)
{
    USB_EPx_REG_T *ctl_epx = NULL ;
    uint32 i;
    uint32 send_len;
    uint32 *write_buf;
    uint32 old_tick, new_tick;
    uint32 ret;
    SX_REQUEST_T    *req = to_sx_req (_req);
    SX_EP_T         *ep = to_sx_ep (_ep);


    ret = TRUE;
    send_len = req->req.length;
    write_buf = req->req.buf;

    if (send_len == 0)
    {
        return;
    }

    ctl_epx = (USB_EPx_REG_T *) (USB_EP0_BASE + ( (ep->bEndpointAddress) +1) *0x40);

    //Clear transfer end int_en bit
    ctl_epx->int_clr.mBits.transfer_end = 1 ;

    //Epx max packsize is 64 bytes .
    if ( (send_len >> 6) > 0)
    {
        for (i=0; i< (send_len >> 6); i++)
        {
            //          SCI_TRACE_LOW("\nUCOM_write 64\n");
            req->req.buf = (uint32 *) (write_buf + (i<<6));
            req->req.length = USB_FIFO_MAX_BYTE;
            old_tick = new_tick = SCI_GetTickCount();
            SxUDC_WriteFIFO (ep,req);

            while (! (ctl_epx->int_sts.mBits.transfer_end))                 //wait until transfer end
            {
                new_tick = SCI_GetTickCount();

                if ( (new_tick - old_tick) > 2000/*UCOM_DRV_TX_MAX_TIME*/)
                {
                    s_fifo_blocking=TRUE;                                   // it means the system is blocking.
                    break;
                }

                if (!SxUDC_GetConnectState())
                {
                    break;
                }
            }

            ctl_epx->int_clr.mBits.transfer_end = 1 ;
        }

        if (send_len%64)
        {
            //          SCI_TRACE_LOW("\nUCOM_write tail\n");
            req->req.buf = (uint32 *) (write_buf + (i<<6));
            req->req.length = (send_len - (i<<6));
            old_tick = new_tick = SCI_GetTickCount();
            SxUDC_WriteFIFO (ep,req);

            while (! (ctl_epx->int_sts.mBits.transfer_end))                 //wait until transfer end
            {
                new_tick = SCI_GetTickCount();

                if ( (new_tick - old_tick) > 2000/*UCOM_DRV_TX_MAX_TIME*/)
                {
                    s_fifo_blocking=TRUE;                                   // it means the system is blocking.
                    break;
                }

                if (!SxUDC_GetConnectState())
                {
                    break;
                }
            }

            ctl_epx->int_clr.mBits.transfer_end = 1 ;
        }
    }
    else
    {
        req->req.buf = (uint32 *) (write_buf);
        req->req.length = (send_len);
        old_tick = new_tick = SCI_GetTickCount();
        SxUDC_WriteFIFO (ep,req);

        while (! (ctl_epx->int_sts.mBits.transfer_end))                 //wait until transfer end
        {
            new_tick = SCI_GetTickCount();

            if ( (new_tick - old_tick) > 2000/*UCOM_DRV_TX_MAX_TIME*/)
            {
                s_fifo_blocking=TRUE;                                   // it means the system is blocking.
                break;
            }

            if (!SxUDC_GetConnectState())
            {
                break;
            }
        }

        ctl_epx->int_clr.mBits.transfer_end = 1 ;
    }

    return ;
}

/******************************************************************************/
//  Description:    SxUDC_Queue
//  Global resource dependence:
//  Author:
//  Note:           Call this function to deal with request in the queue
/******************************************************************************/

LOCAL int SxUDC_Queue (USB_EP_T *_ep, USB_REQUEST_T *_req)
{
    SX_UDC_T    *dev = NULL;
    //int32 size = 0;
    //int32 num = 0;
    SX_REQUEST_T    *req = to_sx_req (_req);
    SX_EP_T         *ep = to_sx_ep (_ep);
    //USB_DEV_REG_T *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

    dev = ep->dev;

    if (ep->bEndpointAddress == 0)                              /* deal with ep0 */
    {
        if (dev->ep0state == EP0_IN_DATA_PHASE)                 /* ep0 send to host */
        {
            SxUDC_USB_Configure ( (uint8 *) req->req.buf,req->req.length);

            if (SCI_SUCCESS==SxUDC_WriteFIFO (ep,req))          /* ep0 send fifo */
            {
                dev->ep0state = EP0_IDLE;
            }

        }
        else if (dev->ep0state == EP0_OUT_DATA_PHASE)           /* ep0 read from host */
        {
            list_add_tail (&req->queue, &ep->queue);
            //          num = SxUDC_ReadFIFO(ep,req);                       /* ep0 read fifo */
            //          return num;
        }
        else                                                    /* ep0 unknown state */
        {
            UDC_DEBUG ("\nsx_udc_queue() ep0 state error\n");
            return SCI_ERROR;
        }
    }
    else if (ep->bEndpointAddress == 1)             /* deal with ep1 */
    {

        SCI_DisableIRQ();
        list_add_tail (&req->queue, &ep->queue);
        req->req.status = REQ_IN_QUEUE;

        SCI_RestoreIRQ();
        ctl_ep1->int_ctrl.mBits.data_not_ready = 1;                          /* ep1 send fifo */
        USB_ActiveTimer (ep->bEndpointAddress & 0x7F);
    }
    else if (ep->bEndpointAddress == 2)             /* deal with ep2 */
    {
        SCI_DisableIRQ();
        list_add_tail (&req->queue, &ep->queue);
        //ep1_queue_count[2]++;
        req->req.status = REQ_IN_QUEUE;
        //record_usb_trace4((0xAA<<24)|(req->req.length<<8)|ep1_queue_count[2]);
        SCI_RestoreIRQ();

        ctl_ep2->int_ctrl.mBits.buf_not_ready = 1;
        //USB_ActiveTimer (ep->bEndpointAddress & 0x7F);		
    }
    else if (ep->bEndpointAddress == 3)             /* deal with ep3 */
    {

        SCI_DisableIRQ();
        list_add_tail (&req->queue, &ep->queue);
        req->req.status = REQ_IN_QUEUE;

        if (req->req.length <= 64)
        {
            req->req.dma = 0;
        }
        else
        {
            req->req.dma = 1;
        }

        record_usb_trace3 ( (0xAA<<24) | (req->req.length<<8) |req->req.status); /*lint !e648  !e737*/
        SCI_RestoreIRQ();
        ctl_ep3->int_ctrl.mBits.data_not_ready = 1;                          /* ep1 send fifo */
        ctl_ep3->int_ctrl.mBits.transfer_end = 0;
        USB_ActiveTimer (ep->bEndpointAddress & 0x7F);
    }
    else if (ep->bEndpointAddress  == 4)                /* deal with ep4 */
    {
        SCI_DisableIRQ();
        list_add_tail (&req->queue, &ep->queue);
        req->req.status = REQ_IN_QUEUE;
        SCI_RestoreIRQ();
        //SxUDC_StartTransfer (req,ep);
        ctl_ep4->int_ctrl.mBits.buf_not_ready = 1;
    }

    return SCI_SUCCESS;
}

/******************************************************************************/
//  Description:    SxUDC_Dequeue
//  Global resource dependence:
//  Author:
//  Note:           Call this function to remove the reqeust in the queue
/******************************************************************************/
LOCAL int SxUDC_Dequeue (USB_EP_T *_ep, USB_REQUEST_T *_req)
{
    USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
    struct sx_ep    *ep = to_sx_ep (_ep);
    SX_REQUEST_T    *req = to_sx_req (_req);

    if (!list_empty (&ep->queue))
    {
        req->req.actual = 0;
        SxUDC_Done (ep,req,SCI_ERROR);

        if (ep->bEndpointAddress==1)
        {
            ctl_ep1->int_ctrl.mBits.data_not_ready = 0;
        }

        if (ep->bEndpointAddress==2)
        {
            ctl_ep2->int_ctrl.mBits.buf_not_ready = 0;
        }

        if (ep->bEndpointAddress==3)
        {
            ctl_ep3->int_ctrl.mBits.data_not_ready = 0;
        }

        if (ep->bEndpointAddress==4)
        {
            ctl_ep4->int_ctrl.mBits.buf_not_ready = 0;
        }
    }

    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_SetHalt
//  Global resource dependence:
//  Author:
//  Note:           Call this function to set the endpoint to halt
/******************************************************************************/
LOCAL int SxUDC_SetHalt (USB_EP_T *_ep, int value)
{
    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_GetFrame
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get one frame
/******************************************************************************/
LOCAL int SxUDC_GetFrame (USB_GADGET_T *_gadget)
{
    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_WakeUp
//  Global resource dependence:
//  Author:
//  Note:           Call this function to wakeup the udc
/******************************************************************************/
LOCAL int SxUDC_WakeUp (USB_GADGET_T *_gadget)
{
    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_SetSelfpowered
//  Global resource dependence:
//  Author:
//  Note:           Call this function to set udc to self power
/******************************************************************************/
LOCAL int SxUDC_SetSelfpowered (USB_GADGET_T *gadget, int value)
{
    return 0;
}

/******************************************************************************/
//  Description:    SxUDC_VbusSession
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get vbus session
/******************************************************************************/
LOCAL int SxUDC_VbusSession (USB_GADGET_T *gadget, int is_active)
{
    return 0;
}


#if 0
/*****************************************************************************/
//  Description:   udc_USB_SetPllDividor
//  Global resource dependence:
//  Author:
//  Note:          set usb 12m and 48m clk dividor according to PLL
/*****************************************************************************/
LOCAL void SxUDC_SetPllDividor (void)
{
    uint32  pll_clk ;
    PLL_TYPE_E  pll_type;
    pll_type = PLLTYPE_V;
    pll_clk = CHIP_GetVPllClk();

    /*
    * add code here, to check whether the pll clock is valid
    * according to usb 12Mclk,48Mclk
    */
    SxUDC_EnableClk (USB_12M_CLK,pll_clk);
    SxUDC_EnableClk (USB_48M_CLK,pll_clk);
}
#endif
/*****************************************************************************/
//  Description:    SxUDC_SetHostReq
//  Global resource dependence:
//  Author:
//  Note:          Call this function when first get host request
/*****************************************************************************/
LOCAL void SxUDC_SetHostReq (uint32 state)
{
    the_udc_controller->hostreq = state;
}

/*****************************************************************************/
//  Description:    SxUDC_GetHostReq
//  Global resource dependence:
//  Author:
//  Note:          Call this function to get host request statue
/*****************************************************************************/
LOCAL uint32 SxUDC_GetHostReq()
{
    return the_udc_controller->hostreq;
}

/*****************************************************************************/
//  Description:    SxUDC_SetConnectState
//  Global resource dependence:
//  Author:
//  Note:          Call this function to set vbus connection statue
/*****************************************************************************/
LOCAL void SxUDC_SetConnectState (uint32 state)
{
    the_udc_controller->connected = state;
}

/*****************************************************************************/
//  Description:    SxUDC_GetConnectState
//  Global resource dependence:
//  Author:
//  Note:          Call this function to get host connection statue
/*****************************************************************************/
LOCAL uint32 SxUDC_GetConnectState (void)
{
    return the_udc_controller->connected;
}

/*****************************************************************************/
//  Description:    SxUDC_Connect
//  Global resource dependence:
//  Author:
//  Note:          Call this function to set usb statue
/*****************************************************************************/
LOCAL uint32 SxUDC_Connect()
{
    if (FALSE == SxUDC_GetConnectState())
    {
        SxUDC_SetConnectState (TRUE);
    }

    return 0;
}

/*****************************************************************************/
//  Description:    SxUDC_Disconnect
//  Global resource dependence:
//  Author:
//  Note:          Call this function to set disconnection state
/*****************************************************************************/
LOCAL uint32 SxUDC_Disconnect()
{
    if (TRUE == SxUDC_GetConnectState())
    {
        SxUDC_SetConnectState (FALSE);
    }

    return 0;
}

/*****************************************************************************/
//  Description:    SxUDC_VbusIrq
//  Global resource dependence:
//  Author:
//  Note:          Call this function to handle vbus interrupt
/*****************************************************************************/
LOCAL void SxUDC_VbusIrq (uint32 gpio_state)
{
    xSignalHeader signal;

    signal = SCI_ALLOC_APP (sizeof (*signal));

    if (gpio_state)                             //cable inserted
    {
        SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
        SxUDC_Connect ();
        signal->SignalCode = USB_VBUS_PLUG_IN;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
        USB_InitEPStatus();
    }
    else                                        //cable removed
    {
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        AHB_CTL0_U *ahb_ctl0_ptr = (AHB_CTL0_U *) (AHB_CTL0);
#endif
        SxUDC_Disconnect();
        SxUDC_SetHostReq (FALSE);
        SxUDC_DisableUSBIrq();
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

        if(usb_clk_device !=SCI_NULL)
        {
		    DEVICE_Close(usb_clk_device);
		}
#else
        ahb_ctl0_ptr->mBits.usbd_eb = 0;
#endif
        USB_LDO_Control (FALSE);
        signal->SignalCode = USB_VBUS_PLUG_OUT;
        signal->SignalSize = sizeof (*signal);
        signal->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signal, usb_isr_id);
        UDC_DisableAllEndPointDMA ();
        SXUDC_FreeAllDMA();
        //could not delete timer in irq, remove it to UDC_HWUNInit
        //USB_FreeAllTimers();
        SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
    }
}

/*****************************************************************************/
//  Description:    SxVbusDraw
//  Global resource dependence:
//  Author:
//  Note:           Call this function to handle set vbus power draw
/*****************************************************************************/
LOCAL int SxVbusDraw (USB_GADGET_T *_gadget, uint32 ma)
{
    return 0;
}

/*****************************************************************************/
//  Description:    SxUDC_Disable
//  Global resource dependence:
//  Author:
//  Note:           Call this function to disable udc
/*****************************************************************************/
LOCAL void SxUDC_Disable()
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

    ctl_usb->ctrl.mBits.en      = FALSE;
}

/*****************************************************************************/
//  Description:    SxUDC_Enable
//  Global resource dependence:
//  Author:
//  Note:           Call this function to enable udc
/*****************************************************************************/
LOCAL void SxUDC_Enable()
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;

    ctl_usb->ctrl.mBits.en      = TRUE;
}

/*****************************************************************************/
//  Description:    SxUDC_EnableIrq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to enable udc irq
/*****************************************************************************/
LOCAL void SxUDC_EnableIrq()
{
    /* Enable all udc level interrupts */
    SxUDC_WriteRegister (0x7F, SX_UDC_USB_INT_CTL_REG);
    /* Enable ep0 interrupt */
    SxUDC_WriteRegister (0x3FFF, SX_UDC_USB_EP0_INT_CTL_REG);
}

/*****************************************************************************/
//  Description:    SxUDC_Reinit
//  Global resource dependence:
//  Author:
//  Note:           Call this function to re-init udc
/*****************************************************************************/
LOCAL void SxUDC_Reinit()
{
    SX_UDC_T *dev = &memory;
    uint32 i;

    /* device/ep0 records init */
    _INIT_LIST_HEAD (&dev->gadget.ep_list);
    _INIT_LIST_HEAD (&dev->gadget.ep0->ep_list);
    dev->ep0state = EP0_IDLE;

    for (i = 0; i < SX_ENDPOINTS; i++)
    {
        SX_EP_T  *ep = &dev->ep[i];

        if (i != 0)
        {
            list_add_tail (&ep->ep.ep_list, &dev->gadget.ep_list);
        }

        ep->dev = dev;
        //        ep->desc = NULL;
        ep->halted = 0;
        _INIT_LIST_HEAD (&ep->queue);
    }
}

/*****************************************************************************/
//  Description:    SxUDC_TxDWordToFIFO
//  Global resource dependence:
//  Author:
//  Note:           Call this function to send data from buf to endpoint
/*****************************************************************************/
LOCAL BOOLEAN SxUDC_TxDWordToFIFO (uint32 *buf, uint8 len, uint8 ep_id)
{
    int i;

    SCI_ASSERT (len   <= USB_FIFO_MAX_WORD);/*assert verified*/
    SCI_ASSERT (ep_id < USB_EPx_NUMBER);/*assert verified*/
    SCI_ASSERT ( ( (uint32) buf & WORD_ADDRESS_MASK) ==0);/*assert verified*/

    for (i=0; i<len; i++)
    {
        * (uint32 *) s_USB_fifo_add_table [ep_id] = \
                SxUDC_FIFODWordEndianSwap (* (buf + i));
    }

    return TRUE ;
}

/*****************************************************************************/
//  Description:    SxUDC_EP0Config
//  Global resource dependence:
//  Author:
//  Note:           Call this function to config endpoint0
/*****************************************************************************/
LOCAL BOOLEAN SxUDC_EP0Config (void)
{
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
    //config DMA's USB channel ;
    //Set max packet size is 8 Byte ;
    ctl_ep0->ctrl.mBits.pack_size   = 0x8 ;

    ctl_ep0->int_clr.dwValue = 0x3fff;
    ctl_ep0->int_ctrl.dwValue = 0;
    ctl_ep0->int_ctrl.mBits.setup_end       = 1 ;
    ctl_ep0->int_ctrl.mBits.in_timeout      = 1 ;
    ctl_ep0->int_ctrl.mBits.transfer_end    = 1 ;
    ctl_ep0->int_ctrl.mBits.transaction_end = 1 ;

    return TRUE ;
}


/*****************************************************************************/
//  Description:    SxUDC_Config
//  Global resource dependence:
//  Author:
//  Note:           Call this function to config udc
/*****************************************************************************/
LOCAL void SxUDC_Config()
{
    uint32 regval;

    //enable usb controller  //set self power
    regval = SxUDC_ReadRegister (SX_UDC_USB_CTL_REG);
    regval |= SELF_POWER_STATE;
    SxUDC_WriteRegister (regval, SX_UDC_USB_CTL_REG);

    /* dev->gadget.speed = USB_SPEED_UNKNOWN; */
    //dev->gadget.speed = USB_SPEED_FULL;

    /* Set timeout for usb device controller */
    SxUDC_WriteRegister (0x0F, SX_UDC_USB_TIM_OUT_REG);

    if (udc_info && udc_info->udc_command)
    {
        udc_info->udc_command (SX_UDC_P_ENABLE);
    }
}


/*****************************************************************************/
//  Description:    SxUDC_PowerSet
//  Global resource dependence:
//  Author:
//  Note:           Call this function to set udc self-power
/*****************************************************************************/
LOCAL void SxUDC_PowerSet (uint32 power)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    //Set USB power ;
    ctl_usb->ctrl.mBits.power       = power;
}

/*****************************************************************************/
//  Description:    SxUDC_EnableIrq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to enable USB irq
/*****************************************************************************/
LOCAL void SxUDC_EnableUSBIrq (void)
{
    CHIPDRV_EnableIRQINT (TB_USB_INT);
}

/*****************************************************************************/
//  Description:    SxUDC_DisableUSBIrq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to disable USB irq
/*****************************************************************************/
LOCAL void SxUDC_DisableUSBIrq (void)
{
    CHIPDRV_DisableIRQINT (TB_USB_INT);
}


/*****************************************************************************/
//  Description:    SxUDC_Reset
//  Global resource dependence:
//  Author:
//  Note:           Call this function to reset udc
/*****************************************************************************/
LOCAL void SxUDC_Reset (void)
{
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    AHB_CTL0_U       *pAhb    = (AHB_CTL0_U *) AHB_CTL0;
#endif

    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);

    SCI_DisableIRQ();

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
     SCI_ASSERT(DEVICE_Find("USB", &usb_clk_device) == CLOCK_SUCCESS);/*assert verified*/

	 DEVICE_Close(usb_clk_device);
#else
    pAhb->mBits.usbd_eb         = FALSE;
#endif
    SCI_RestoreIRQ();
    SxUDC_ClkReset();
    SCI_DisableIRQ();
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

    DEVICE_Open(usb_clk_device);
#else
    pAhb->mBits.usbd_eb = TRUE;
#endif
    SxUDC_PowerSet (1); //Self power;

    SCI_RestoreIRQ();

}

/*****************************************************************************/
//  Description:    SxUDC_Init
//  Global resource dependence:
//  Author:
//  Note:           Call this function to init udc
/*****************************************************************************/
LOCAL void SxUDC_Init (void)
{

    //Disable USB Interrupt at first ;
    SxUDC_DisableUSBIrq();

#ifdef PLATFORM_SC6800H
    REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#else  //SC6530
#if defined(CHIP_VER_6531)
	CHIP_REG_OR (APB_MISC_CTL0, USB_1K5_EN);
#else
    CHIP_REG_AND (APB_MISC_CTL0,~BIT_6);
#endif
#endif

    //reset usb ahb clk first
    SxUDC_Reset();
//just for sc6530c
   if( 0x6530C000 == CHIP_GetHWChipID()) 
    {
             CHIP_REG_OR(APB_MISC_CTL0, BIT_11);
             CHIP_REG_AND(APB_MISC_CTL0, ~BIT_10);
    }
   
    {
        //Connect USB ;
        USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
        USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
        USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
        USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
        USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
        USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

#if defined(CHIP_VER_6531EFM)
	ctl_usb->ctrl.mBits.en      = TRUE;
#endif

        ctl_usb->int_clr.dwValue  = 0x7F;
        ctl_ep0->int_clr.dwValue  = 0xffffffff;
        ctl_ep1->int_clr.dwValue  = 0xffffffff;
        ctl_ep2->int_clr.dwValue  = 0xffffffff;
        ctl_ep3->int_clr.dwValue  = 0xffffffff;
        ctl_ep4->int_clr.dwValue  = 0xffffffff;

        ctl_ep0->int_ctrl.dwValue &= ~0x3FFF;

#if !defined(CHIP_VER_6531EFM)
        ctl_usb->ctrl.mBits.en      = FALSE;
        SCI_Sleep (30);

        ctl_usb->ctrl.mBits.en      = TRUE;
#endif

        ctl_ep0->int_ctrl.mBits.setup_end = 1 ;

        ctl_ep1->ctrl.mBits.ep_en  = FALSE;
        ctl_ep2->ctrl.mBits.ep_en  = FALSE;
        ctl_ep3->ctrl.mBits.ep_en  = FALSE;
        ctl_ep4->ctrl.mBits.ep_en  = FALSE;
    }

    /* Set timeout for usb device controller */
    SxUDC_WriteRegister (0x0F, SX_UDC_USB_TIM_OUT_REG);

#if defined(CHIP_VER_6531EFM)
    CHIP_REG_OR (APB_MISC_CTL0, USB_1K5_EN);
#endif

    //Enable USB Interrupt at last;
    SxUDC_EnableUSBIrq();

}

PUBLIC void UDC_HWInit (void)
{
    //uint32 regval;
    UDC_LDOControl (FALSE);
    SxUDC_Init();
    SCI_Sleep (30);
    UDC_LDOControl (TRUE);
}
PUBLIC void UDC_HWUNInit (void)
{
    // mark the host connect
#if !defined(PLATFORM_SC6530) && !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    AHB_CTL0_U *ahb_ctl0_ptr = (AHB_CTL0_U *) (AHB_CTL0);
#endif
    //Sometimes EP3 transfer suspended because of loss of USB DMA request,
    //so we add timer to minitor EP3 transfer
    USB_FreeAllTimers();
    CHIPDRV_DisableIRQINT (TB_USB_INT);
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))


	    DEVICE_Reset(usb_clk_device);

    CHIP_REG_OR (APB_MISC_CTL0,BIT_6);
#else
    * (volatile uint32 *) AHB_SOFT_RST |= USB_SOFT_RESET_BIT;
    ahb_ctl0_ptr->mBits.usbd_eb = 0;
#endif
    UDC_LDOControl (FALSE);

}
/*****************************************************************************/
//  Description:    SxUDC_ClkReset
//  Global resource dependence:
//  Author:
//  Note:           Call this function to reset udc clock
/*****************************************************************************/
LOCAL void SxUDC_ClkReset (void)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))


    DEVICE_Reset(usb_clk_device);
#else
    * (volatile uint32 *) AHB_SOFT_RST &= ~USB_SOFT_RESET_BIT;
    OS_TickDelay (10);

    * (volatile uint32 *) AHB_SOFT_RST |= USB_SOFT_RESET_BIT;
    OS_TickDelay (10);

    * (volatile uint32 *) AHB_SOFT_RST &= ~USB_SOFT_RESET_BIT;
    OS_TickDelay (10);
#endif
}

/*****************************************************************************/
//  Description:    SxUDC_DisableAllEndPointDMA
//  Global resource dependence:
//  Author:
//  Note:           Call this function to disable udc endpoint dma
/*****************************************************************************/
LOCAL BOOLEAN SxUDC_DisableAllEndPointDMA (void)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

    if (s_dma_channel[DMA_TYPE_USB_EP1] != 0)
    {
        DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP1],FALSE);
    }

    if (s_dma_channel[DMA_TYPE_USB_EP2] != 0)
    {
        DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP2],FALSE);
    }

    if (s_dma_channel[DMA_TYPE_USB_EP3] != 0)
    {
        DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP3],FALSE);
    }

    if (s_dma_channel[DMA_TYPE_USB_EP4] != 0)
    {
        DMA_HAL_ChnEnable (s_dma_channel[DMA_TYPE_USB_EP4],FALSE);
    }

#elif defined PLATFORM_SC6800H
    DMA_HAL_ChnEnable (DMA_USB_EP1,FALSE);
    DMA_HAL_ChnEnable (DMA_USB_EP2,FALSE);
    DMA_HAL_ChnEnable (DMA_USB_EP3,FALSE);
    DMA_HAL_ChnEnable (DMA_USB_EP4,FALSE);
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description:    SxUDC_DataConfig
//  Global resource dependence:
//  Author:
//  Note:           Call this function to prepare the date udc need
/*****************************************************************************/
LOCAL void SxUDC_DataConfig()
{
    /* udc operation */
    sx_ops.get_frame        = SxUDC_GetFrame;
    sx_ops.wakeup           = SxUDC_WakeUp;
    sx_ops.set_selfpowered  = SxUDC_SetSelfpowered;
    sx_ops.pullup           = SxUDC_PullUp;
    sx_ops.vbus_session     = SxUDC_VbusSession;
    sx_ops.vbus_draw        = SxVbusDraw;

    /* endpoint operation */
    sx_ep_ops.enable        = SxUDC_EpEnable;
    sx_ep_ops.disable       = SxUDC_EpDisable;
    sx_ep_ops.alloc_request = SxUDC_AllocRequest;
    sx_ep_ops.free_request  = SxUDC_FreeRequest;
    sx_ep_ops.queue         = SxUDC_Queue;
    sx_ep_ops.dequeue       = SxUDC_Dequeue;
    sx_ep_ops.set_halt      = SxUDC_SetHalt;
    sx_ep_ops.fifo_status   = SxUDC_FIFOStatus;
    sx_ep_ops.read_loop     = SxUDC_ReadLoop;
    sx_ep_ops.write_loop    = SxUDC_WriteLoop;

    base_addr               = USB_REG_BASE;

    memory.address          = USB_REG_BASE;
    memory.gadget.ops       = &sx_ops;
    memory.gadget.ep0       = &memory.ep[0].ep;
    memory.gadget.name      = gadget_name;

    /* control endpoint */
    memory.ep[0].num        = 0;
    memory.ep[0].ep.name        = "ep0";
    memory.ep[0].ep.ops     = &sx_ep_ops;
    memory.ep[0].ep.maxpacket   = EP0_FIFO_SIZE;
    memory.ep[0].dev        = &memory;
    memory.ep[0].fifo_size  = EP_FIFO_SIZE;

    /* first group of endpoints */
    memory.ep[1].num        = 1;
    memory.ep[1].ep.name        = "ep1-bulk";
    memory.ep[1].ep.ops     = &sx_ep_ops;
    memory.ep[1].ep.maxpacket   = EP_FIFO_SIZE;
    memory.ep[1].dev        = &memory;
    memory.ep[1].fifo_size  = EP_FIFO_SIZE;
    memory.ep[1].bEndpointAddress = 1;
    memory.ep[1].bmAttributes   = USB_ENDPOINT_XFER_BULK;

    memory.ep[2].num        = 2;
    memory.ep[2].ep.name        = "ep2-bulk";
    memory.ep[2].ep.ops     = &sx_ep_ops;
    memory.ep[2].ep.maxpacket   = EP_FIFO_SIZE;
    memory.ep[2].dev        = &memory;
    memory.ep[2].fifo_size  = EP_FIFO_SIZE;
    memory.ep[2].bEndpointAddress = 2;
    memory.ep[2].bmAttributes   = USB_ENDPOINT_XFER_BULK;

    memory.ep[3].num        = 3;
    memory.ep[3].ep.name        = "ep3-bulk";
    memory.ep[3].ep.ops     = &sx_ep_ops;
    memory.ep[3].ep.maxpacket   = EP_FIFO_SIZE;
    memory.ep[3].dev        = &memory;
    memory.ep[3].fifo_size  = EP_FIFO_SIZE;
    memory.ep[3].bEndpointAddress = 3;
    memory.ep[3].bmAttributes   = USB_ENDPOINT_XFER_BULK;

    memory.ep[4].num        = 4;
    memory.ep[4].ep.name        = "ep4-bulk";
    memory.ep[4].ep.ops     = &sx_ep_ops;
    memory.ep[4].ep.maxpacket   = EP_FIFO_SIZE;
    memory.ep[4].dev        = &memory;
    memory.ep[4].fifo_size  = EP_FIFO_SIZE;
    memory.ep[4].bEndpointAddress = 4;
    memory.ep[4].bmAttributes   = USB_ENDPOINT_XFER_BULK;

    /* mach depend info : bsp */
    udc_cfg.udc_command     = SxUDC_Pull;
}

/*****************************************************************************/
//  Description:    SxUDC_Probe
//  Global resource dependence:
//  Author:
//  Note:           Call this function to set udc hardware
/*****************************************************************************/
static uint32 SxUDC_Probe()
{
    uint32 status;
    //int32 irq;
    //SX_UDC_T *udc = &memory;

    the_udc_controller = &memory;
    udc_info = &udc_cfg;

    SxUDC_DataConfig();

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    //Reducing USB MIPS scheme
    status = ISR_RegHandler (TB_USB_INT, Sx_UDC_ISR_Handler_Ex);
    s_udc_task_id  = SCI_CreateThread("T_SX_UDC_SRV",
									"Q_SX_UDC_SRV",
									_SX_UDC_SrvThread ,
									0,
									0,
									SX_UDC_STACK_SIZE ,
									SX_UDC_QUEUE_NUM,
									TASK_SX_UDC_SRV_PRI ,
									SCI_PREEMPT,
									SCI_AUTO_START);
#else
    //reg udc isr
    status = ISR_RegHandler_Ex (TB_USB_INT, Sx_UDC_ISR_Handler, SxUDCHisrFunc, CHIPDRV_HISR_PRIO_0, NULL);
#endif
    //reg vbus isr
    GPIO_VBUS_RegIntHandler (SxUDC_VbusIrq);

    //remove dma channel callback func;
    //we donot use DMA interruptor,just use USB transfer_end interruptor.

    // init buf
    SxUDC_Reinit();

    // config endpoint
    SxUDC_Config();
    SxUDC_EnableIrq();

    //  SCI_RestoreIRQ();

    return SCI_SUCCESS;
}


/*****************************************************************************/
//  Description:    USBGadgetRegisterDriver
//  Global resource dependence:
//  Author:
//  Note:           Call this function to register gadget driver to the udc
/*****************************************************************************/
PUBLIC int32 USBGadgetRegisterDriver (USB_GADGET_DRIVER_T *driver)
{
    SX_UDC_T *udc = the_udc_controller;
    int32       retval;
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    UDC_DEBUG ("usb_gadget_register_driver()");


    SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);
    UDC_LDOControl (FALSE);

    SxUDC_Reinit();
	
#ifdef USB_DEBUG_INFO
    usb_index3 = 0;
    usb_index4=0;
#endif

    /* Sanity checks */
    if (!udc)
    {
        UDC_DEBUG ("USBGadgetRegisterDriver(): no udc\n");
        return SCI_ERROR;
    }

    if (udc->driver)
    {
        UDC_DEBUG ("USBGadgetRegisterDriver(): already driver\n");
        return SCI_ERROR;
    }

    if (!driver->bind || !driver->setup || driver->speed < USB_SPEED_FULL)
    {
        UDC_DEBUG ("USBGadgetRegisterDriver(): invalid driver\n");
        return SCI_ERROR;
    }

    if (!driver->unbind)
    {

        UDC_DEBUG ("USBGadgetRegisterDriver(): no unbind method\n");
        return SCI_ERROR;
    }


    // CHIPDRV_DisableIRQINT (TB_USB_INT);
    if (FALSE == SxUDC_GetHostReq())
    {
        // mark the host connect
        SxUDC_SetHostReq (TRUE);
    }

    /* Hook the driver */
    udc->driver = driver;

    SxUDC_Init();//below code is instead of SxUDC_Init()
#if 0
    {


        //Disable USB Interrupt at first ;
        SxUDC_DisableUSBIrq();
#ifdef PLATFORM_SC6800H
        REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN
#else  //SC6530
        CHIP_REG_AND (APB_MISC_CTL0,~BIT_6);
#endif
        //reset usb ahb clk first
        SxUDC_Reset();

        ctl_ep0->int_clr.dwValue  = 0xffffffff;
        ctl_ep0->int_ctrl.dwValue &= ~0x3FFF;
        ctl_ep0->int_ctrl.mBits.setup_end = 1 ;

        ctl_ep1->ctrl.mBits.ep_en  = FALSE;
        ctl_ep2->ctrl.mBits.ep_en  = FALSE;
        ctl_ep3->ctrl.mBits.ep_en  = FALSE;
        ctl_ep4->ctrl.mBits.ep_en  = FALSE;

        //Enable USB Interrupt at last;
        SxUDC_EnableUSBIrq();

    }//up code is instead of SxUDC_Init()
#endif
    UsbDevice_Create (driver,&udc->gadget);
    UDC_DEBUG ("\nusb_gadget_register_driver(): binding gadget driver\n");

    if ( (retval = driver->bind (&udc->gadget)) != SCI_SUCCESS)
    {
        goto register_error;
    }

    SxUDC_EP0Config();
    ctl_ep0->ctrl.mBits.buf_ready  = 1;
    ctl_ep0->ctrl.mBits.data_ready = 0;

    s_usb_device_descriptor = NULL;
    s_usb_config_descriptor = NULL;
    s_descriptor_type = 0;

    SCI_Sleep (30);
    UDC_LDOControl (TRUE);

    UDC_DEBUG ("\nusb_gadget_register_driver(): bind gadget success\n");
    return SCI_SUCCESS;

register_error:
    UDC_DEBUG ("\nusb_gadget_register_driver(): bind gadget failed\n");
    UDC_LDOControl (TRUE);
    udc->driver = NULL;
    return SCI_ERROR;
}

/*****************************************************************************/
//  Description:    USBGadgetUnregisterDriver
//  Global resource dependence:
//  Author:
//  Note:           Call this function to unregister gadget driver from the udc
/*****************************************************************************/
PUBLIC int32 USBGadgetUnregisterDriver (USB_GADGET_DRIVER_T *driver)
{
    SX_UDC_T *udc = the_udc_controller;

    UDC_DEBUG ("\nusb_gadget_unregister_driver(): enter\n");

    if (!udc)
    {
        return SCI_ERROR;
    }

    if (!driver || driver != udc->driver || !driver->unbind)
    {
        return SCI_ERROR;
    }

    driver->unbind (&udc->gadget);
    UsbDevice_Delete (&udc->gadget);

    udc->driver = NULL;

    SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
    UDC_DEBUG ("\nusb_gadget_unregister_driver(): exit\n");
    return 0;
}


/*****************************************************************************/
//  Description:    UDC_Init
//  Global resource dependence:
//  Author:
//  Note:           Call this function to init udc
/*****************************************************************************/
PUBLIC uint32  UDC_Init (void)
{
    uint32 ret;

    ret = SxUDC_Probe();

    return ret;
}

/*****************************************************************************/
//  Description:    UDC_Exit
//  Global resource dependence:
//  Author:
//  Note:           Call this function to exit udc
/*****************************************************************************/
PUBLIC uint32 UDC_Exit (void)
{
    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    UDC_GetConnect
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get udc vbus connection
/*****************************************************************************/
PUBLIC uint32 UDC_GetConnect (void)
{
    return SxUDC_GetConnectState();
}

/*****************************************************************************/
//  Description:    UDC_GetHostReq
//  Global resource dependence:
//  Author:
//  Note:           Call this function to get host request activity
/*****************************************************************************/
PUBLIC uint32 UDC_GetHostReq (void)
{
    return SxUDC_GetHostReq();
}

/*****************************************************************************/
//  Description:    UDC_ClkReset
//  Global resource dependence:
//  Author:
//  Note:           Call this function reset usb clock
/*****************************************************************************/
PUBLIC void UDC_ClkReset (void)
{
    SxUDC_ClkReset ();
}

/*****************************************************************************/
//  Description:    UDC_PowerSet
//  Global resource dependence:
//  Author:
//  Note:           Call this function to set udc self-power
/*****************************************************************************/
PUBLIC void UDC_PowerSet (uint32 power)
{
    SxUDC_PowerSet (power);
}

/*****************************************************************************/
//  Description:    udc_USB_config
//  Global resource dependence:
//  Author:
//  Note:           Call this function to config udc
/*****************************************************************************/
PUBLIC void UDC_Config (void)
{
    SxUDC_Config();
}


/*****************************************************************************/
//  Description:    UDC_DisableAllEndPointDMA
//  Global resource dependence:
//  Author:
//  Note:           Call this function to disable all endpoint dma
/*****************************************************************************/
PUBLIC BOOLEAN UDC_DisableAllEndPointDMA (void)
{
    return SxUDC_DisableAllEndPointDMA();
}

/*****************************************************************************/
//  Description:    UDC_LDOControl
//  Global resource dependence:
//  Author:
//  Note:          Call this function to set ldo statue
/*****************************************************************************/
PUBLIC void UDC_LDOControl (BOOLEAN flag)
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

/*****************************************************************************/
//  Description:    UDC_UCOMIntCtrl
//  Global resource dependence:
//  usb bulk in,out endp int contrl
//  endp_num : endp numer
//  is_able      : int enable or disable
//  ep_type    : endp type, bulk in or bulk out
//  Author:
//  Note:          Call this function to set UCOM interrupt type; just for AT
//                 especially for EP1
/*****************************************************************************/
PUBLIC void UDC_UCOMIntCtrl (uint32 endp_num, BOOLEAN is_able, uint32 ep_type)
{
#if 0
    USB_EPx_REG_T *ctrl_epx = NULL;

    //set bit of data not ready to produce the int
    ctrl_epx = (USB_EPx_REG_T *) (USB_EP0_BASE +
                                  (endp_num) * 0x40);/*lint !e737*/

    if (0x00 == ep_type)
    {
        /*usb bulk in int ctrl*/
        if (is_able)
        {
            //only set this bit when it's zero
            if (ctrl_epx->int_ctrl.mBits.data_not_ready == 0)
            {
                ctrl_epx->int_ctrl.mBits.data_not_ready = 1;
            }
        }
        else
        {
            ctrl_epx->int_clr.mBits.data_not_ready = 1; //int clear
            ctrl_epx->int_ctrl.mBits.data_not_ready = 0;//int disable
        }
    }
    else if (0x02 == ep_type)
    {

        /*usb bulk out int ctrl*/
        if (is_able)
        {
            ctrl_epx->ctrl.mBits.buf_ready          = 1;  //buff ready
            ctrl_epx->int_ctrl.mBits.transfer_end = 1;  //int enable
        }
        else
        {
            ctrl_epx->int_clr.mBits.transfer_end = 1;//int clear
            ctrl_epx->ctrl.mBits.buf_ready          = 0;  //buff ready
            ctrl_epx->int_ctrl.mBits.transfer_end = 0;//int disable
        }
    }

    /*    else if(UCOM_EP_BK_IN_DMA == ep_type)
        {
            do nothing, int freq is controled outside
        }*/
#endif
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

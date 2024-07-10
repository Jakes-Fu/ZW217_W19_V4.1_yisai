/******************************************************************************
 ** File Name:    usb_drv_i.c                                                 *
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
#include "usb_drv.h"
#include "chip_plf_export.h"
#ifdef PLATFORM_SC6800H
#include "v0/usb_reg_v0.h"
#endif
//#include "upm_api.h"
//#include "deepsleep_drvapi.h"
#include "dma_drv_internal.h"
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
#define WORD_ADDRESS_MASK        0x00000003UL

//define word endian swap
#define WORD_ENDIAN_SWAP(word) (((word & 0xFF)<<8) |((word & 0xFF00) >>8))

//define dword endian swap
#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
LOCAL const uint32 *s_USB_fifo_add_table [USB_EPx_NUMBER]=
{
    (uint32 *) USB_EP0_IN_FIFO,
    (uint32 *) USB_EP0_OUT_FIFO,
    (uint32 *) USB_EP1_FIFO,
    (uint32 *) USB_EP2_FIFO,
    (uint32 *) USB_EP3_FIFO,
    (uint32 *) USB_EP4_FIFO
};

PUBLIC USB_ISR_PTR_T g_USB_irq_table =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
/*--------------------------- Local Data ------------------------------------*/
LOCAL uint32 s_usb_plug_in_count = 0;
LOCAL uint32 s_usb_plug_out_count = 0;
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL void USB_ClearAllInt (void)
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

/*****************************************************************************/
//  Description:    Set plug in count
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void USB_SetPlugInCount (BOOLEAN is_increase)
{
    if (is_increase)
    {
        s_usb_plug_in_count++;
    }
    else if (s_usb_plug_in_count > 0)
    {
        s_usb_plug_in_count--;
    }

    //SCI_TRACE_LOW:"USB_SetPlugInCount: count %d, is increase %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_106_112_2_17_23_7_59_1471,(uint8*)"dd", s_usb_plug_in_count, is_increase);
}

/*****************************************************************************/
//  Description:    Set plug out count
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void USB_SetPlugOutCount (BOOLEAN is_increase)
{
    if (is_increase)
    {
        s_usb_plug_out_count++;
    }
    else if (s_usb_plug_out_count > 0)
    {
        s_usb_plug_out_count--;
    }

    //SCI_TRACE_LOW:"USB_SetPlugOutCount: count %d, is increase %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_124_112_2_17_23_7_59_1472,(uint8*)"dd", s_usb_plug_out_count, is_increase);

}

/*****************************************************************************/
//  Description:    Get plug in count
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_GetPlugInCount (void)
{
    //SCI_TRACE_LOW:"USB_GetPlugInCount: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_134_112_2_17_23_7_59_1473,(uint8*)"d", s_usb_plug_in_count);
    return s_usb_plug_in_count;
}

/*****************************************************************************/
//  Description:    Get plug out count
//  Global resource dependence:
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_GetPlugOutCount (void)
{
    //SCI_TRACE_LOW:"USB_GetPlugOutCount: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_144_112_2_17_23_7_59_1474,(uint8*)"d", s_usb_plug_out_count);
    return s_usb_plug_out_count;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T USB_irq (uint32 param)
{
#if 0
    USB_DEV_REG_T *usb_ctrl_ptr = (USB_DEV_REG_T *) USB_REG_BASE;
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    if ( (UPM_GetUPMState()))
    {
        if (UPM_NO_SERVICE == UPM_GetService())
        {
            //Clear all interrupt ;
            if (usb_ctrl_ptr->int_sts.dwValue)
            {
                //SCI_TRACE_LOW:"USB irq is 0x%x "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_165_112_2_17_23_7_59_1475,(uint8*)"d",usb_ctrl_ptr->int_raw.dwValue);

                //cleart all usb int
                USB_ClearAllInt();

                if (!UPM_GetConnectState())
                {

                    UPM_SetUSBState (TRUE);

                    if (0 == USB_GetPlugInCount())
                    {
                        USB_SetPlugInCount (TRUE);
                        UPM_SetConnectState (TRUE);
                        UPM_SendCommand (UPM_MANAGER,UPM_CMD_CONNECT,NULL,NULL);
                        //SCI_TRACE_LOW:"USB_irq:USB send PLUG_IN signal to UPM "
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_180_112_2_17_23_7_59_1476,(uint8*)"");
                    }

                }

                USB_Disconnect ();
            }
        }
        else
        {
            if (!UPM_GetConnectState())
            {
                if (ctl_ep0->int_sts.mBits.setup_end)
                {

                    UPM_SetUSBState (TRUE);

                    if (0 == USB_GetPlugInCount())
                    {
                        USB_SetPlugInCount (TRUE);
                        UPM_SetConnectState (TRUE);
                        UPM_SendCommand (UPM_MANAGER,UPM_CMD_CONNECT,NULL,NULL);
                        //SCI_TRACE_LOW:"USB_irq:USB send PLUG_IN signal to UPM and service"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_202_112_2_17_23_7_59_1477,(uint8*)"");
                    }

                }
            }

            if (UPM_GetServiceState() && g_USB_irq_table.usb_irq)
            {
                g_USB_irq_table.usb_irq (param);
            }
            else
            {
                //cleart all usb int
                USB_ClearAllInt();

                //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_217_112_2_17_23_7_59_1478,(uint8*)"");
            }
        }
    }

#endif
    return ISR_DONE;

}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epin (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_epin)
    {
        g_USB_irq_table.dma_epin (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_239_112_2_17_23_7_59_1479,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epout (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_epout)
    {
        g_USB_irq_table.dma_epout (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_255_112_2_17_23_7_59_1480,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep1 (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_ep1)
    {
        g_USB_irq_table.dma_ep1 (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_271_112_2_17_23_7_59_1481,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep2 (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_ep2)
    {
        g_USB_irq_table.dma_ep2 (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_287_112_2_17_23_7_59_1482,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep3 (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_ep3)
    {
        g_USB_irq_table.dma_ep3 (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_303_112_2_17_23_7_59_1483,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep4 (uint32 param)
{
    if (/*UPM_GetServiceState() &&*/ g_USB_irq_table.dma_ep4)
    {
        g_USB_irq_table.dma_ep4 (param);
    }
    else
    {
        //SCI_TRACE_LOW:"USB current UPM's service is disabled !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_319_112_2_17_23_7_59_1484,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_FIFODWordEndianSwap (uint32 DWord)
{
#ifdef _LITTLE_ENDIAN_DEBUG
    return DWORD_ENDIAN_SWAP (DWord);
#else
    return DWord;
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len)
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

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_DWordEndianSwap (uint32 DWord)
{
#ifdef _LITTLE_ENDIAN_DEBUG
    return (DWord);
#else
    return DWORD_ENDIAN_SWAP (DWord);
#endif
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint16 USB_WordEndianSwap (uint16 word)
{
    return WORD_ENDIAN_SWAP (word);
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_DWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len)
{
    while (len-- > 0)
    {
        *des++  = DWORD_ENDIAN_SWAP (*src);
        src++;
    }

    return TRUE ;
}


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_RxDWordFromFIFO (
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
        * (buf + i) = USB_FIFODWordEndianSwap (*s_USB_fifo_add_table [ep_id]) ;
    }

    return TRUE ;
}       //Send data to FIFO ;
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_TxDWordToFIFO (
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
        * (uint32 *) s_USB_fifo_add_table [ep_id] = \
                USB_FIFODWordEndianSwap (* (buf + i));
    }

    return TRUE ;
}       //Send data to FIFO ;

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_DisableAllEndPointDMA (void)
{
    DMA_HAL_ChnEnable (DMA_USB_EP1,SCI_FALSE);
    DMA_HAL_ChnEnable (DMA_USB_EP2,SCI_FALSE);
    DMA_HAL_ChnEnable (DMA_USB_EP3,SCI_FALSE);

    //SCI_TRACE_LOW:"USB : Disable all end-point DMA !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USB_DRV_I_450_112_2_17_23_8_0_1485,(uint8*)"");

    return TRUE;

}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
//Eof

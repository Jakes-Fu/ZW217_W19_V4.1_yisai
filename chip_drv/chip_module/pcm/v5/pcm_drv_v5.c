/******************************************************************************
 ** File Name:    pcm_drv_v5.c                                                                                            *
 ** Author:       Thomas.Chai                                                                                               *
 ** DATE:         4/26/2006                                                                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.                                       *
 ** Description:                                                                                                                   *
  ** 09/20/2010    Steve.Zhan cleanup code ,and refactoring some functions.                          *
 *****************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "dma_drv_internal.h"
#include "task_monitor.h"
#include "clock_drvapi.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **     Macro Data
 **---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
**                            Macro Definitions                               *
**---------------------------------------------------------------------------*/
#define _PCM_DEBUG_  0
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **     Local Data
 **---------------------------------------------------------------------------*/




/**---------------------------------------------------------------------------*
 **        Static Data
 **---------------------------------------------------------------------------*/
LOCAL PCM_HAL_T  s_pcm_handle;

__align (4) LOCAL uint16 s_pcm_tx_buffer[PCM_DATA_LEN];
__align (4) LOCAL uint16 s_pcm_rx_buffer[PCM_DATA_LEN];

LOCAL uint32 s_tx_dma_chan_id =IIS_DMA_NO_CHN_VAL;
LOCAL uint32 s_rx_dma_chan_id =IIS_DMA_NO_CHN_VAL;
LOCAL DMA_CHNCFG_T s_tx_dma_cfg;
LOCAL DMA_CHNCFG_T s_rx_dma_cfg;
LOCAL DEVICE_HANDLE s_dev_pcm = SCI_NULL;

/*****************************************************************************/
//  Description:    This function disable/enable PCM IRQ interrupt
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_IRQIntEnable (BOOLEAN  flag)
{
    if (flag)
    {
        CHIPDRV_EnableIRQINT (PCM_IRQ_INT);
    }
    else
    {
        CHIPDRV_DisableIRQINT (PCM_IRQ_INT);
    }
}



/*****************************************************************************/
//  Description:    This function disable/enable PCM Tx interrupt
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_TxIntEnable (BOOLEAN  flag)
{
    volatile PCM_CTL_REG_T *reg_ptr = (PCM_CTL_REG_T *) (PCM_CTL_BASE);

    if (flag)
    {
        reg_ptr->iis_ien |= IIS_INT_STS_TX_FIFO_EMPTY;
    }
    else
    {
        reg_ptr->iis_ien &= ~ (IIS_INT_STS_TX_FIFO_EMPTY |
                               IIS_INT_STS_TX_FIFO_FULL);
    }

}
/*****************************************************************************/
//  Description:    This function disable/enable PCM Rx interrupt
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_RxIntEnable (BOOLEAN  flag)
{
    volatile PCM_CTL_REG_T *reg_ptr = (PCM_CTL_REG_T *) (PCM_CTL_BASE);

    if (flag)
    {
        reg_ptr->iis_ien |= IIS_INT_STS_RX_FIFO_FULL;
    }
    else
    {
        reg_ptr->iis_ien &= ~ (IIS_INT_STS_RX_FIFO_FULL |
                               IIS_INT_STS_SLAVE_TIMEOUT);
    }

}


/*****************************************************************************/
//  Description:    check input param
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
LOCAL ERR_PCM_E PCM_CheckParam (PCM_PARA_T *pcm_param_ptr)
{
    SCI_ASSERT (NULL != pcm_param_ptr);/*assert verified*/

    if ( (pcm_param_ptr->sync_mode > MULTI_SLOT_OPERATOR_MODE)
            || (pcm_param_ptr->operate_mode > SLAVE_MODE)   /*lint !e685 comfirmed by xuepeng*/
            || (pcm_param_ptr->data_bit_num > TX_32BITS_PER_FRAME)
            || (pcm_param_ptr->pcm_slot == 0)
            || (pcm_param_ptr->pcm_slot > (PCM_SLOT0|PCM_SLOT1|PCM_SLOT2))) /*lint !e655 !e685 comfirmed by xuepeng*/
    {
        return ERR_PCM_PARAM_SCALE_WRONG;
    }

    return   ERR_PCM_SUCCESS;
}



/*****************************************************************************/
//  Description:    This function initalize PCM default paramer
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/

LOCAL void PCM_InitParam (PCM_HAL_T *pcm_hal_ptr)
{
    SCI_ASSERT (NULL != pcm_hal_ptr);/*assert verified*/

    //Clear all items
    SCI_MEMSET ( (uint8 *) pcm_hal_ptr, 0, sizeof (PCM_HAL_T));

    //Set paramer
    pcm_hal_ptr->dev_para.sync_mode    = SHORT_FRAME_SYNC_MODE;
    pcm_hal_ptr->dev_para.operate_mode = SLAVE_MODE;
    pcm_hal_ptr->dev_para.data_bit_num = TX_16BITS_PER_FRAME;
    pcm_hal_ptr->dev_para.tx_watermark = FIFO_EMPTY_WATERMARK;
    pcm_hal_ptr->dev_para.rx_watermark = FIFO_FULL_WATERMARK;
}

/*****************************************************************************/
//  Description:   config PCM sync mode
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_SetSyncMode (PCM_PARA_T *param)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;
    PCM_SYNC_MODE_E sync = param->sync_mode;

    reg_ptr->iis_ctl0 &= ~ (IIS_CTL0_IISC_SHRTH | IIS_CTL0_SYNC_MODE);

    switch (sync)
    {
        case SHORT_FRAME_SYNC_MODE:
            reg_ptr->iis_ctl0 |= (IIS_CTL0_IISC_SHRTH);
            break;
        case LONG_FRAME_SYNC_MODE:
            reg_ptr->iis_ctl0 |= (0x0 << 8);
            break;
        case MULTI_SLOT_OPERATOR_MODE:
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

}

/*****************************************************************************/
//  Description:   config PCM operate mode
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_SetOperateMode (PCM_PARA_T *param)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;
    PCM_OPERATE_MODE_E mode = param->operate_mode;

    switch (mode)
    {
        case MASTER_MODE:
            reg_ptr->iis_ctl0 &= ~IIS_CTL0_SLAVE_MODE;
            break;
        case SLAVE_MODE:
            reg_ptr->iis_ctl0 |=  IIS_CTL0_SLAVE_MODE;
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

}

/*****************************************************************************/
//  Description:   config PCM channel bit number
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_SetTxRxBitNum (PCM_PARA_T *param)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;
    DATA_BIT_NUM_E bitnum = param->data_bit_num;

    switch (bitnum)
    {
        case TX_8BITS_PER_FRAME:
            reg_ptr->iis_ctl0 &= ~ (IIS_CTL0_BIT_PER_CHN_MASK);  //Clear SPI tansmit data bit number
            reg_ptr->iis_ctl0 |= (IIS_CTL0_BIT_PER_CHN_8 <<
                                  IIS_CTL0_BIT_PER_CHN_SHIFT);    //Set SPI tansmit data bit number
            s_pcm_handle.byte_per_frame = 1;
            break;

        case TX_16BITS_PER_FRAME:
            reg_ptr->iis_ctl0 &= ~ (IIS_CTL0_BIT_PER_CHN_MASK);  //Clear SPI tansmit data bit number
            reg_ptr->iis_ctl0 |= (IIS_CTL0_BIT_PER_CHN_16 <<
                                  IIS_CTL0_BIT_PER_CHN_SHIFT);      //Set SPI tansmit data bit number
            s_pcm_handle.byte_per_frame = 2;
            break;

        case TX_32BITS_PER_FRAME:
            reg_ptr->iis_ctl0 &= ~ (IIS_CTL0_BIT_PER_CHN_MASK);  //Clear SPI tansmit data bit number
            reg_ptr->iis_ctl0 |= (IIS_CTL0_BIT_PER_CHN_32 <<
                                  IIS_CTL0_BIT_PER_CHN_SHIFT);      //Set SPI tansmit data bit number
            s_pcm_handle.byte_per_frame = 4;
            break;

        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }
}


/*****************************************************************************/
//  Description:   config PCM tx,rx watermark value
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_SetWaterMark (PCM_PARA_T *param)
{
    PCM_SET_RX_WATERMARK (param);
    PCM_SET_TX_WATERMARK (param);
}

/*****************************************************************************/
//  Description:   config PCM slot
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_SetPcmSlot (PCM_PARA_T *param)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;
    reg_ptr->iis_ctl2 &= ~ (IIS_CTL2_PCM_SLOT_MASK);
    reg_ptr->iis_ctl2 |= (param->pcm_slot << IIS_CTL2_PCM_SLOT_SHIFT);

    // PCM_CYCLE = 0
    reg_ptr->iis_ctl2 &= ~ (IIS_CTL2_PCM_CYCLE_MASK);
}

/*****************************************************************************/
//  Description:   config PCM default value
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL  void PCM_ResetRegist (void)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;

    DEVICE_Reset(s_dev_pcm);

    // Use default value
    reg_ptr->iis_ctl1 = 0x3F11;

    // Enable PCM mode
    reg_ptr->iis_ctl0 |= IIS_CTL0_PCM_EB;
}

/*****************************************************************************/
//  Description:   config PCM with parameter
//  Author:        Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL uint32 PCM_DoParam (PCM_PARA_T *param)
{
    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;

    SCI_ASSERT (NULL != param);/*assert verified*/

    // Close all interupt
    reg_ptr->iis_iclr = 0x3F;
    reg_ptr->iis_ien = 0x0;

    //Reset All Register
    PCM_ResetRegist();

    //Set operate mode
    PCM_SetOperateMode (param);

    //Set Sync Mode
    PCM_SetSyncMode (param);

    //Set Slot
    PCM_SetPcmSlot (param);

    //Set Water Mark
    PCM_SetWaterMark (param);

    //Set Data bit number
    PCM_SetTxRxBitNum (param);

    return ERR_PCM_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function clear Tx Buffer to default
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL ERR_PCM_E PCM_ResetTxRxBuff (PCM_HAL_T *pcm_hal_ptr)
{
    int32 aRes = ERR_PCM_SUCCESS;
    SCI_ASSERT (NULL != pcm_hal_ptr);/*assert verified*/
    SCI_MEMSET ( ( (uint8 *) &pcm_hal_ptr->tx_buf), 0, sizeof (PCM_BUF_T));
    return aRes;
}


/*********************************************************************/
//  Description:  Enable PCM Module
//  Author:         Thomas.Chai
//  Note:
/*********************************************************************/
LOCAL void PCM_EnableModule (BOOLEAN isEnable)
{
    if (isEnable)
    {
        // Enable PCM device.
        DEVICE_Open (s_dev_pcm);
    }
    else
    {
        // Disable PCM device.
        DEVICE_Close (s_dev_pcm);
    }
}

/*****************************************************************************/
//  Description:    This function Enable/Disable PCM DMA
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
LOCAL void PCM_DmaEnable (BOOLEAN isEnable)
{

    volatile PCM_CTL_REG_T *reg_ptr = (volatile PCM_CTL_REG_T *) PCM_CTL_BASE;

    if (isEnable)
    {
        reg_ptr->iis_ctl0 |= IIS_CTL0_PCM_DMA_EB;
    }
    else
    {
        reg_ptr->iis_ctl0 &= ~IIS_CTL0_PCM_DMA_EB;
    }

}

/*****************************************************************************/
//  Description:   the callback function of Transmit DMA total length
//
//  Author:         Thomas.Chai
//  Note:           DMA Transmit is word to Word
//
/*****************************************************************************/
LOCAL void PCM_DMATx_CallBack (uint32 num)
{
    CALL_RET_T aRet;

    DMA_HAL_ChnEnable (s_tx_dma_chan_id, SCI_FALSE);

    if (CLOSE_STATUS == s_pcm_handle.pcm_status)
    {
        return;
    }

    aRet.value = s_pcm_handle.tx_buf.size >> 1;
    aRet.error = ERR_PCM_SUCCESS;

#if _PCM_DEBUG_
    TM_SendTestPointRequest (0x33 , SCI_GetTickCount());
#endif

    //Reset to default internal buffer
    s_tx_dma_cfg.src_address  = (uint32) s_pcm_tx_buffer;
    s_tx_dma_cfg.total_length = 2* (PCM_FIFO_SIZE_IN_WORD - s_pcm_handle.dev_para.tx_watermark) *HALF_WORD_SIZE;
    s_tx_dma_cfg.block_length = (PCM_FIFO_SIZE_IN_WORD - s_pcm_handle.dev_para.tx_watermark) *HALF_WORD_SIZE;

    //Switch to outside buffer
    SCI_ASSERT (NULL != s_pcm_handle.dev_para.tx_CallBack);/*assert verified*/
    s_pcm_handle.dev_para.tx_CallBack (aRet);

    SCI_ASSERT (0 != s_tx_dma_cfg.total_length);/*assert verified*/
    SCI_ASSERT (0 != s_tx_dma_cfg.block_length);/*assert verified*/
    SCI_ASSERT (NULL != s_tx_dma_cfg.src_address);/*assert verified*/

    DMA_HAL_ChnConfig (s_tx_dma_chan_id, &s_tx_dma_cfg);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = s_tx_dma_cfg.src_address;
        cacheConfig[0].bufferSize = s_tx_dma_cfg.total_length;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;

        DMA_HAL_ChnEnable_Ex (s_tx_dma_chan_id, SCI_TRUE, cacheConfig, 1);
    }

    //    DMA_HAL_ChnEnable (s_tx_dma_chan_id, SCI_TRUE);

}

/*****************************************************************************/
//  Description:   the callback function of Receive DMA total length
//
//  Author:         Thomas.Chai
//  Note:           DMA Transmit is half word to half Word
//
/*****************************************************************************/
LOCAL void PCM_DMARx_CallBack (uint32 num)
{
    CALL_RET_T aRet;

    DMA_HAL_ChnEnable (s_rx_dma_chan_id, SCI_FALSE);

    if (CLOSE_STATUS == s_pcm_handle.pcm_status)
    {
        return;
    }

    aRet.value = s_pcm_handle.rx_buf.size >> 1;
    aRet.error = ERR_PCM_SUCCESS;

#if _PCM_DEBUG_
    TM_SendTestPointRequest (0x44 , SCI_GetTickCount());
#endif

    //Reset to default internal buffer
    s_rx_dma_cfg.dest_address  = (uint32) s_pcm_rx_buffer;
    s_rx_dma_cfg.total_length   = 2*s_pcm_handle.dev_para.rx_watermark*HALF_WORD_SIZE;
    s_rx_dma_cfg.block_length   = s_pcm_handle.dev_para.rx_watermark*HALF_WORD_SIZE;

    //Switch to outside buffer
    SCI_ASSERT (NULL != s_pcm_handle.dev_para.rx_CallBack);/*assert verified*/
    s_pcm_handle.dev_para.rx_CallBack (aRet);

    SCI_ASSERT (0 != s_rx_dma_cfg.total_length);/*assert verified*/
    SCI_ASSERT (0 != s_rx_dma_cfg.block_length);/*assert verified*/
    SCI_ASSERT (NULL != s_rx_dma_cfg.dest_address);/*assert verified*/

    DMA_HAL_ChnConfig (s_rx_dma_chan_id, &s_rx_dma_cfg);

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = s_rx_dma_cfg.dest_address;
        cacheConfig[0].bufferSize = s_rx_dma_cfg.total_length;
        cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;

        DMA_HAL_ChnEnable_Ex (s_rx_dma_chan_id, SCI_TRUE, cacheConfig, 1);
    }
    //    DMA_HAL_ChnEnable (s_rx_dma_chan_id, SCI_TRUE);
}

/*****************************************************************************/
//  Description:   Config DMA of PCM Tx, which is connected with UART3 Tx
//
//  Author:         Thomas.Chai
//  Note:           DMA Transmit is half word to half Word
//
/*****************************************************************************/
LOCAL  void  PCM_TxDMA_Config (void)
{
    //DC Register DMA Call Back Function
    DMA_HAL_ChnIntConfig (s_tx_dma_chan_id, DMA_CHN_INT_TYPE_TRANSDONE, PCM_DMATx_CallBack);

    s_tx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_tx_dma_cfg.priority       = DMA_CHN_PRIORITY_0;
    s_tx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_tx_dma_cfg.src_datawidth  = DMA_DATAWIDTH_HALFWORD;
    s_tx_dma_cfg.dest_datawidth = DMA_DATAWIDTH_HALFWORD;
    s_tx_dma_cfg.src_address  = (uint32) s_pcm_tx_buffer;
    s_tx_dma_cfg.dest_address   = PCM_CTL_BASE;
    s_tx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_tx_dma_cfg.block_length   = PCM_FIFO_SIZE_IN_WORD*HALF_WORD_SIZE;
    s_tx_dma_cfg.total_length   = 64*HALF_WORD_SIZE;
    s_tx_dma_cfg.src_step       = 2;  // src increament
    s_tx_dma_cfg.dest_step      = 0;  // des nochange
    s_tx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_tx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

    DMA_HAL_ChnConfig (s_tx_dma_chan_id, &s_tx_dma_cfg);
}

/*****************************************************************************/
//  Description:   Config DMA of PCM Rx, which is connected with UART3 Rx
//
//  Author:         Thomas.Chai
//  Note:           DMA Transmit is half word to half Word
//
/*****************************************************************************/
LOCAL  void  PCM_RxDMA_Config (void)
{
    //DC Register DMA Call Back Function
    DMA_HAL_ChnIntConfig (s_rx_dma_chan_id, DMA_CHN_INT_TYPE_TRANSDONE, PCM_DMARx_CallBack);

    s_rx_dma_cfg.type           = DMA_CHN_TYPE_HARD;
    s_rx_dma_cfg.priority       = DMA_CHN_PRIORITY_1;
    s_rx_dma_cfg.workmode       = DMA_CHN_WORKMODE_NORMAL;
    s_rx_dma_cfg.src_datawidth  = DMA_DATAWIDTH_HALFWORD;
    s_rx_dma_cfg.dest_datawidth = DMA_DATAWIDTH_HALFWORD;

#if defined (CHIP_ENDIAN_LITTLE)
    s_rx_dma_cfg.src_address  = PCM_CTL_BASE + 2;//Notice:sc8800g,dma has changed tolittle endian, need to change base address to
    //fetch the high 16bits.
#else
    s_rx_dma_cfg.src_address  = PCM_CTL_BASE;//Notice:sc8800h,dma default big endian, fetch the high 16bits.
#endif

    s_rx_dma_cfg.dest_address   = (uint32) s_pcm_rx_buffer;
    s_rx_dma_cfg.requestmode    = DMA_CHN_REQMODE_NORMAL;
    s_rx_dma_cfg.block_length   = PCM_FIFO_SIZE_IN_WORD*HALF_WORD_SIZE;
    s_rx_dma_cfg.total_length   = 64*HALF_WORD_SIZE;
    s_rx_dma_cfg.src_step       = 0;  // src nochange
    s_rx_dma_cfg.dest_step      = 2;  // des increament
    s_rx_dma_cfg.src_blockmode  = DMA_BLOCKMODE_SINGLETRANS;
    s_rx_dma_cfg.dest_blockmode = DMA_BLOCKMODE_SINGLETRANS;

    DMA_HAL_ChnConfig (s_rx_dma_chan_id, &s_rx_dma_cfg);
}

/*****************************************************************************/
//  Description:    This function initalize PCM Paramer
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
PUBLIC void PCM_Init (void)
{

    CLOCK_STATUS status = DEVICE_Find ("PCM", &s_dev_pcm);
    SCI_ASSERT (status == CLOCK_SUCCESS); /*assert verified*/

    //Close PCM IRQ Interrupt
    PCM_IRQIntEnable (FALSE);

    //Close Tx & Rx Interrupt, And Be opened when user want to send or receive datas
    PCM_TxIntEnable (FALSE);
    PCM_RxIntEnable (FALSE);

    PCM_InitParam (&s_pcm_handle);

    //Clear Tx buffer
    PCM_ResetTxRxBuff (&s_pcm_handle);
}

LOCAL void IIS_DMA_Open(void)
{
    IIS_DMA_ALLOC(DMA_IIS_TX_OFFSET,s_tx_dma_chan_id);
    IIS_DMA_ALLOC(DMA_IIS_RX_OFFSET,s_rx_dma_chan_id);
}


LOCAL void IIS_DMA_Close(void)
{
    IIS_DMA_FREE(DMA_IIS_TX_OFFSET,s_tx_dma_chan_id);
    IIS_DMA_FREE(DMA_IIS_RX_OFFSET,s_rx_dma_chan_id);
}



/*****************************************************************************/
//  Description:    Open a PCM interface, and start to transmit data. you can set
//                  a new paramer for PCM, and you also set the paramer to zero,
//                  then it will use the default setting to PCM. If the new paramer
//                  is unvaild, it also will use the default setting
//  Author:         Thomas.Chai
//  Note:
//
/*****************************************************************************/
PUBLIC ERR_PCM_E  PCM_Open (PCM_PARA_T *paramer)
{
    int32 aRes = ERR_PCM_SUCCESS;

    SCI_ASSERT (CLOSE_STATUS == s_pcm_handle.pcm_status);/*assert verified*/

    //Change PCM Status
    s_pcm_handle.pcm_status = OPEN_STATUS;

    SCI_MEMSET ( ( (uint8 *) &s_tx_dma_cfg), 0,sizeof (DMA_CHNCFG_T));
    SCI_MEMSET ( ( (uint8 *) &s_rx_dma_cfg), 0,sizeof (DMA_CHNCFG_T));

    //Set the codec paramer
    if (paramer)
    {
        aRes = PCM_CheckParam (paramer);

        if (aRes == ERR_PCM_SUCCESS) //Paramer is vaild , update the default paramer
        {
            SCI_MEMCPY ( (uint8 *) &s_pcm_handle.dev_para, (uint8 *) paramer, sizeof (PCM_PARA_T));
        }
        else
        {
            //SCI_TRACE_LOW:"PCM Parameter Error\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DRV_V5_608_112_2_17_23_6_3_1114,(uint8*)"");
            SCI_ASSERT (0);/*assert to do*/
        }
    }

    //PCM Module enable
    PCM_EnableModule (TRUE);

    //Set the codec paramer to registers
    PCM_DoParam (&s_pcm_handle.dev_para);

    //Clear Tx&Rx buffer
    PCM_ResetTxRxBuff (&s_pcm_handle);

    IIS_DMA_Open();

    // Stop dma transfer channel
    DMA_HAL_ChnEnable (s_tx_dma_chan_id, SCI_FALSE);
    DMA_HAL_ChnEnable (s_rx_dma_chan_id, SCI_FALSE);

    PCM_TxDMA_Config();
    PCM_RxDMA_Config();

    {
        DMA_CAHCECONFIG_T cacheConfig[1];
        cacheConfig[0].bufferAddr = (uint32) s_pcm_tx_buffer;
        cacheConfig[0].bufferSize = 64*HALF_WORD_SIZE;
        cacheConfig[0].dir = DMABUFFER_TO_DEVICE;

        DMA_HAL_ChnEnable_Ex (s_tx_dma_chan_id, SCI_TRUE, cacheConfig, 1);

        cacheConfig[0].bufferAddr = (uint32) s_pcm_rx_buffer;
        cacheConfig[0].bufferSize = 64*HALF_WORD_SIZE;
        cacheConfig[0].dir = DMABUFFER_FROM_DEVICE;

        DMA_HAL_ChnEnable_Ex (s_rx_dma_chan_id, SCI_TRUE, cacheConfig, 1);

    }
    //    DMA_HAL_ChnEnable (s_tx_dma_chan_id, SCI_TRUE);
    //    DMA_HAL_ChnEnable (s_rx_dma_chan_id, SCI_TRUE);

    PCM_DmaEnable (TRUE);

    //SCI_TRACE_LOW:"PCM_Open End\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DRV_V5_651_112_2_17_23_6_3_1115,(uint8*)"");

    return aRes;
}


/*****************************************************************************/
//  Description:    stop PCM transmit, and close pcm interface.
//  Global resource dependence:
//  Author:         Livy.Liu&Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC ERR_PCM_E PCM_Close (void)
{
    SCI_ASSERT (OPEN_STATUS == s_pcm_handle.pcm_status);/*assert verified*/

    //Change PCM Status
    s_pcm_handle.pcm_status = CLOSE_STATUS;

    //Disable DMA
    DMA_HAL_ChnEnable (s_tx_dma_chan_id, SCI_FALSE);
    DMA_HAL_ChnEnable (s_rx_dma_chan_id, SCI_FALSE);

    //disable PCM INT
    PCM_TxIntEnable (FALSE);
    PCM_RxIntEnable (FALSE);

    //PCM DMA Disable
    PCM_DmaEnable (FALSE);

    //PCM Module enable
    PCM_EnableModule (FALSE);

    IIS_DMA_Close();

    //SCI_TRACE_LOW:"==PCM Close===\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PCM_DRV_V5_684_112_2_17_23_6_3_1116,(uint8*)"");

    return ERR_PCM_SUCCESS;
}



/*****************************************************************************/
//  Description:    Give the outside buffer pointer to PCM data sending buffer
//                  pointer. and start the transmit interrupt to transmit data
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC int32 PCM_SendData (uint8 *data_ptr,uint32 data_len)
{
    uint32 len = 0;
    SCI_ASSERT (NULL != data_ptr);/*assert verified*/
    len = (data_len&0x01) ? (data_len-1) :data_len;
    s_pcm_handle.tx_buf.size = len;
    s_tx_dma_cfg.src_address  = (uint32) data_ptr;
    s_tx_dma_cfg.total_length   = len;
    s_tx_dma_cfg.block_length   = (PCM_FIFO_SIZE_IN_WORD - s_pcm_handle.dev_para.tx_watermark) *HALF_WORD_SIZE;

    return (len >> 1);
}


/*****************************************************************************/
//  Description:    Move received data from fifo to outside buffer
//  Global resource dependence:
//  Author:         Thomas.Chai
//  Note:
/*****************************************************************************/
PUBLIC int32 PCM_ReceiveData (uint8 *buff_ptr, uint32 data_len)
{
    uint32 aLen = 0;
    SCI_ASSERT (NULL != buff_ptr);/*assert verified*/
    aLen = (data_len&0x01) ? (data_len-1) :data_len;
    s_pcm_handle.rx_buf.size = aLen;
    s_rx_dma_cfg.dest_address  = (uint32) buff_ptr;
    s_rx_dma_cfg.total_length   = aLen;
    s_rx_dma_cfg.block_length   = s_pcm_handle.dev_para.rx_watermark*HALF_WORD_SIZE;

    return (aLen >> 1);
}

/*****************************************************************************/
//  Description:    Switch IIS/PCM Control Right to DSP or Retrun Back.
//  Global resource dependence:
//  Author:         ken.kuang
//  Note:           when TO DSP , we open Device ,and Disable ARM Control.
//                  when return ARM, we clsoe Device ,and Enable ARM Control.
/*****************************************************************************/
PUBLIC uint32 PCM_Ctrl_Switch (PCM_CTRL_STATUS_E ctrl)
{
    switch (ctrl)
    {
        case TO_DSP_CTRL:
            {
                //PCM Module enable
                PCM_EnableModule (TRUE); //Clock is Set by Clock Moudle.
                // DSP Access IIS/PCM
                IIS_PCM_SWITCH_TO_ARM (0,FALSE);
            }
            break;
        case TO_ARM_CTRL:
            {
                //PCM Module disable
                PCM_EnableModule (FALSE);
                // ARM Access IIS/PCM
                IIS_PCM_SWITCH_TO_ARM (0,TRUE);
            }
            break;
        default:
            SCI_ASSERT (0);/*assert verified*/
            break;
    }

    return ERR_PCM_SUCCESS;
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of pcm_drv_v0.c

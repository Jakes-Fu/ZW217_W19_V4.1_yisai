/**************************************************************************

  Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
  All rights reserved.

  This document contains proprietary information blonging to
  SPREADTRUM. Passing on and copying this document, use and
  communication of its contents is not permitted without prior
  writed authorization.

***************************************************************************/

/***************************************************************************

  Reversion information:
  File Name:    sim_drv.c
  Version:      0.1
  Author:       Raislin.Kong
  Date:         2002.2
  Description:  This is the sim driver file
***************************************************************************/
/***************************************************************************

                Edit Histroy
  Date          Name            Description
  --------------------------------------------------------------------------
  2001.2        Raislin.Kong    Create
  2002.10       Raislin.Kong    change the SIM driver and move it under the new driver framework
  2005.12.31    Yongxia.zhang
  2008.4.7  wuding.yang  divide the chip_drv and the ISO7816-3 protocol into 2 parts
***************************************************************************/

#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "sim_drvapi.h"
#include "ldo_drvapi.h"
#include "clock_drvapi.h"
#include "chip_drvapi.h"
#include "../int/int_phy.h"

#ifdef   __cplusplus
extern   "C"
{
#endif
//---------------marco define---------------

//---------------Golbal variable--------------------------


//---------------Local Variable----------------------------
static sim_s *s_sim_register_map[SIMDRV_CARD_NUM];//the register of the SIM interface

#define INT_RX_BUF_MAX 64
typedef struct
{
    uint16  p_rd;
    uint16  p_wr;
    uint8  buf[INT_RX_BUF_MAX];   //the buf used to save rx data in rx int function
} SIM_INT_RX_BUF_T;

typedef struct
{
    SIMDRV_CARD_ID_E card_id;
    TB_SIM_INT_TYPE_E int_type;
} SIM_INT_RECORD;

INPUT_BUFFER_INIT (SIM_INT_RECORD, 8)

static SIM_INT_RX_BUF_T sim_rx_int_buf[SIMDRV_CARD_NUM] = {0};

static  DMA_CAHCECONFIG_T cacheRxConfig[2];//RX2
static  DMA_CAHCECONFIG_T cacheTxConfig[2];//TX2

// Sim device handle
LOCAL DEVICE_HANDLE s_dev_sim0 = SCI_NULL;
LOCAL DEVICE_HANDLE s_dev_sim1 = SCI_NULL;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
static CHIP_TYPE_E s_simdrv_chip_type = SC6531EFM;
#elif defined(PLATFORM_UWS6121E) 
static CHIP_TYPE_E s_simdrv_chip_type = UIX8910;
#else
static CHIP_TYPE_E s_simdrv_chip_type = SC6530;
#endif

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
// sim dma config
#define SIM_DMA_NO_CHN_VAL 0xffffffff

#define DMA_SIM_TX_REQID                      0
#define DMA_SIM_RX_REQID                      1

#define SIM_VOLT_VAL1        1800
#define SIM_VOLT_VAL2        3000

#if defined(PLATFORM_SC6530)
#define SIM_REQ_ID_ADDR(_req_offset) (DMA_CTL_REG_BASE + 0x2028 + (_req_offset)*0x04)
#elif defined(PLATFORM_SC6531EFM)
#define SIM_REQ_ID_ADDR(_req_offset) (DMA_CTL_REG_BASE + 0x2018 + (_req_offset)*0x04)
#elif defined(PLATFORM_UWS6121E)
#define SIM_REQ_ID_ADDR(_req_offset) (REG_DMA_BASE + 0x40 + (_req_offset)*0x04)
#endif

#define SIM_DMA_ALLOC(_req_id,_chn_id)               \
    DRV_MACRO_START                                 \
    if( SIM_DMA_NO_CHN_VAL == _chn_id ){             \
        _chn_id = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);    \
        CHIP_REG_SET(SIM_REQ_ID_ADDR(_req_id),_chn_id);  \
    }                                               \
    DRV_MACRO_END

#define SIM_DMA_FREE(_req_id,_chn_id)                \
    DRV_MACRO_START                                 \
    if( SIM_DMA_NO_CHN_VAL != _chn_id ){             \
        CHIP_REG_SET(SIM_REQ_ID_ADDR(_req_id),0);        \
        DMA_HAL_FreeChn(_chn_id);                       \
        _chn_id = SIM_DMA_NO_CHN_VAL;                    \
    }                                               \
    DRV_MACRO_END

LOCAL uint32 s_sim_dma_tx_chn = SIM_DMA_NO_CHN_VAL;
LOCAL uint32 s_sim_dma_rx_chn = SIM_DMA_NO_CHN_VAL;
#endif
//---------------Local Function declare--------------------

/**********************************************************************
//    Description:
//      This is the function that dispatch different SIM interrupt to
//    their Handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static ISR_EXE_T SIMDRV_InteruptHandler (uint32 lparam);

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMDRV_IntCallBack (
    SIMDRV_CARD_ID_E card_id,
    TB_SIM_INT_TYPE_E int_type //the exeption interrupt type
);

/**********************************************************************
//    Description:
//      Hisr function
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_HisrFunc (uint32 i, void *d);

/**********************************************************************
//    Description:
//      Be called by Hisr
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_HandleHisr (SIM_INT_RECORD int_record);

/**********************************************************************
//    Description:
//      write one byte to a SIM_INT_RX_BUF_T buffer.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static BOOLEAN SIMDRV_WrIntRxBuf (SIM_INT_RX_BUF_T *ptr,uint8 value);

/**********************************************************************
//    Description:
//      write one byte to a SIM_INT_RX_BUF_T buffer.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static BOOLEAN SIMDRV_RdIntRxBuf (SIM_INT_RX_BUF_T *ptr,uint8 *value);

/**********************************************************************
//    Description:
//      Copy rx fifo data to sim_rx_int_buf[*],be called by lisr.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_WriteIntRxBufData (uint8 card_id,uint8 w_count);

//--------------------------functions define-------------------------------//

/**********************************************************************
//    Description:
//      write one byte to a SIM_INT_RX_BUF_T buffer.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static BOOLEAN SIMDRV_WrIntRxBuf (SIM_INT_RX_BUF_T *ptr,uint8 value)
{
    ptr->buf[ptr->p_wr++] = value;

    if (ptr->p_wr > (INT_RX_BUF_MAX - 1))
    {
        ptr->p_wr = 0;
    }

    SCI_PASSERT ( (ptr->p_wr != ptr->p_rd), ("SIMDRV_WrIntRxBuf: buffer full!")); /*assert to do*/
    return 0; /*lint !e527 comfirmed by windy.wang*/
}

/**********************************************************************
//    Description:
//      Get one data from a SIM_INT_RX_BUF_T buffer.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static BOOLEAN SIMDRV_RdIntRxBuf (SIM_INT_RX_BUF_T *ptr,uint8 *value)
{
    if (ptr->p_rd != ptr->p_wr)
    {
        *value = ptr->buf[ptr->p_rd++];

        if (ptr->p_rd > (INT_RX_BUF_MAX - 1))
        {
            ptr->p_rd = 0;
        }

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//    Description:
//      Copy rx fifo data to sim_rx_int_buf[*],be called by lisr.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_WriteIntRxBufData (uint8 card_id,uint8 w_count)
{
    uint32 i;
    uint8 value;

    SCI_ASSERT (card_id < SIMDRV_CARD_NUM); /*assert verified*/

    for (i = 0; i < w_count; i++)
    {
        value = SIMDRV_RecvData ( (uint8) card_id);
        SIMDRV_WrIntRxBuf (&sim_rx_int_buf[card_id],value);
    }

    return;
}

/**********************************************************************
//    Description:
//      Get one data from sim_rx_int_buf[*],be called by hisr rx callback function.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
uint8 SIMDRV_ReadIntRxBufData (uint8 card_id)
{
    uint8 value;

    SCI_ASSERT (card_id < SIMDRV_CARD_NUM); /*assert verified*/

    if (!SIMDRV_RdIntRxBuf (&sim_rx_int_buf[card_id],&value))
    {
        SCI_PASSERT (0, ("SIMDRV_RdIntRxBuf: buffer empty!"));/*assert to do*/
        return 0; /*lint !e527 comfirmed by windy.wang*/
    }

    return value;
}

/**********************************************************************
//    Description:
//      Return data number of sim_rx_int_buf[*],be called by hisr callback function.
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetRxIntBufCount (uint8 card_id)
{
    int16 write,read;

    SCI_ASSERT (card_id < SIMDRV_CARD_NUM); /*assert verified*/
    write = sim_rx_int_buf[card_id].p_wr;
    read = sim_rx_int_buf[card_id].p_rd;

    return ( (INT_RX_BUF_MAX + (write - read)) %INT_RX_BUF_MAX);
}

/**********************************************************************
//    Description:
//      Reset sim_rx_int_buf[*].
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
void SIMDRV_ClearRxIntBuf (uint8 card_id)
{
    SCI_ASSERT (card_id < SIMDRV_CARD_NUM); /*assert verified*/
    sim_rx_int_buf[card_id].p_wr = 0;
    sim_rx_int_buf[card_id].p_rd = 0;
}

/**********************************************************************
//    Description:
//      Initialize the Driver timer of the idle mode
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
uint32 SIMDRV_InitDriver (uint8 card_id, TB_CALLBACK handler)
{
    uint32 status;

    s_simdrv_chip_type = CHIP_GetChipType();

    if (SIMDRV_CARD_ONE == card_id)
    {
        // Get SIM device handle
        SCI_ASSERT (DEVICE_Find ("SIM0", &s_dev_sim0) == CLOCK_SUCCESS); /*assert verified*/

        status = ISR_RegCallback (TB_SIM_INT, handler); //register SIM1's interrupt callback

        if (TB_SUCCESS != status)
        {
            return status;
        }

        //status = ISR_RegHandler_Ex(TB_SIM_INT, SIMDRV_InteruptHandler, SIMDRV_HisrFunc, CHIPDRV_HISR_PRIO_0, NULL);
        status = ISR_RegHandler_Ex (TB_SIM_INT, SIMDRV_InteruptHandler, SIMDRV_HisrFunc, CHIPDRV_HISR_PRIO_1, NULL);

        if (TB_SUCCESS != status)
        {
            return status;
        }

        s_sim_register_map[SIMDRV_CARD_ONE] = (sim_s *) SIM0_BASE;

        if ((SC6531 == s_simdrv_chip_type) || (SC6530C == s_simdrv_chip_type) || (SC6531EFM == s_simdrv_chip_type) || (SC6531EFM_AB == s_simdrv_chip_type))
        {
            DEVICE_Open (s_dev_sim0);
        }
    }
    else if (SIMDRV_CARD_TWO == card_id)
    {
        // Get SIM device handle
        SCI_ASSERT (DEVICE_Find ("SIM1", &s_dev_sim1) == CLOCK_SUCCESS); /*assert verified*/

        //SC8800G SIM0/1 have the same interrupt,so this function should return TB_SUCCESS or TB_CALLBACK_ASSIGNED(it's ok)
        status = ISR_RegCallback (TB_SIM1_INT, handler); //register SIM2's interrupt callback

        if (TB_PTR_ERROR == status)
        {
            return status;
        }

        //SC8800G SIM0/1 have the same interrupt,so this function should return TB_SUCCESS or TB_ISR_ASSIGNED(it's ok)
        status = ISR_RegHandler_Ex (TB_SIM1_INT, SIMDRV_InteruptHandler, SIMDRV_HisrFunc, CHIPDRV_HISR_PRIO_1, NULL);

        if (TB_PTR_ERROR == status)
        {
            return status;
        }

        s_sim_register_map[SIMDRV_CARD_TWO] = (sim_s *) SIM1_BASE;

    }
    else
    {
        SCI_ASSERT (FALSE);/*assert to do*/
    }

    //SCI_TRACE_LOW:"SIMDRV%d:init sim drv"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_347_112_2_17_23_6_26_1139,(uint8*)"d",card_id);
    return TB_SUCCESS;
}

/**********************************************************************
//    Description:
//      Notify the SIM layer the exeption interrupt
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMDRV_IntCallBack (
    SIMDRV_CARD_ID_E card_id,
    TB_SIM_INT_TYPE_E int_type //the exeption interrupt type
)
{
    TB_MSG msg;
    TB_SIM_INT_T exp_int;

    exp_int.int_type = int_type;
    msg.message = SIM_EXCEPTION_INT;
    msg.wparam = card_id;
    msg.lparam = &exp_int;

    if (SIMDRV_CARD_ONE == card_id)
    {
        ISR_Callback (TB_SIM_INT,&msg);
    }
    else if (SIMDRV_CARD_TWO == card_id)
    {
        ISR_Callback (TB_SIM1_INT,&msg);
    }
    else
    {
        SCI_ASSERT (FALSE);/*assert verified*/
    }
}

/**********************************************************************
//    Description:
//      This is the function that dispatch different SIM interrupt to
//    their Handle function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static ISR_EXE_T SIMDRV_InteruptHandler (uint32 lparam)
{
    SIMDRV_CARD_ID_E card_id = SIMDRV_CARD_ONE;
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
    uint32 isr_status = INT_PHY_IRQGetUint0Status();
#else
    uint32 isr_status = CHIPDRV_GetIRQStatus(); // Get interrupt status.
#endif
    uint32 sim_int_status;
    SIM_INT_RECORD int_record;
    ISR_EXE_T result = ISR_DONE;
    uint8 fifo_count = 0;
    uint32 sim_reg_sts0[SIMDRV_CARD_NUM] = {0,0};
    uint32 sim_reg_ie[SIMDRV_CARD_NUM] = {0,0};

    sim_reg_sts0[SIMDRV_CARD_ONE] = s_sim_register_map[SIMDRV_CARD_ONE]->sts0;
    sim_reg_sts0[SIMDRV_CARD_TWO] = s_sim_register_map[SIMDRV_CARD_TWO]->sts0;
    sim_reg_ie[SIMDRV_CARD_ONE] = s_sim_register_map[SIMDRV_CARD_ONE]->ie;
    sim_reg_ie[SIMDRV_CARD_TWO] = s_sim_register_map[SIMDRV_CARD_TWO]->ie;

    if (SC6810 == s_simdrv_chip_type)
    {
        //SC8800G sim0/1 have the same interrupt.
        if (0 != (sim_reg_sts0[SIMDRV_CARD_ONE] & (uint32)SIMSTS0_REG_MASK)) //0x39F is SC8800G ie default
        {
            card_id = SIMDRV_CARD_ONE;
        }
        else if (0 != (sim_reg_sts0[SIMDRV_CARD_TWO] & (uint32)SIMSTS0_REG_MASK)) //0x39F is SC8800G ie default
        {
            //sim_sc8800g_2nd_int_flag = TRUE;
            card_id = SIMDRV_CARD_TWO;
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }
    else
    {
        if ( (uint32) INTCTL_SIM_IRQ == (isr_status & ( (uint32) INTCTL_SIM_IRQ)))
        {
            card_id = SIMDRV_CARD_ONE;
        }
        else
        {
            if (INTCTL_SIM1_IRQ == (isr_status & INTCTL_SIM1_IRQ))
            {
                card_id = SIMDRV_CARD_TWO;
            }
            else
            {
                SCI_ASSERT (0);/*assert verified*/
            }
        }
    }

    int_record.card_id = card_id;
    sim_int_status = sim_reg_sts0[card_id] & sim_reg_ie[card_id];

    //There is RX parity error interrupt
    if (sim_int_status & SIMSTS0_B_RX_PARITY_ERR)
    {
        SIMDRV_DisableSIMRXInt ( (uint8) card_id);
        int_record.int_type = TB_SIMINT_RX_PARITY_ERROR;
        IsrWriteBuffer (int_record);
        s_sim_register_map[card_id]->iclr = SIMICLR_B_RX_PARITY_ERR;  //clear the RX parity Error interrupt
        result = CALL_HISR;
    }

    //There is TX parity error interrupt
    if (sim_int_status & SIMSTS0_B_TX_PARITY_ERR)
    {
        SIMDRV_DisableSIM_TxParityInt ( (uint8) card_id);
        int_record.int_type = TB_SIMINT_TX_PARITY_ERROR;
        IsrWriteBuffer (int_record);
        s_sim_register_map[card_id]->iclr = SIMICLR_B_TX_PARITY_ERR; //clear the TX parity Error interrupt
        result = CALL_HISR;
    }

    //There is Card Un response interrupt
    if (sim_int_status & SIMSTS0_B_UNRESP_CARD)
    {
        int_record.int_type = TB_SIMINT_CARD_UNRESPONSE;
        IsrWriteBuffer (int_record);
        s_sim_register_map[card_id]->iclr = SIMICLR_B_UNRESP_CARD; //clear the Card Un response interrupt
        result = CALL_HISR;
    }

    if (SC6810 != s_simdrv_chip_type)//SC6810 has no this interrupt
    {
        //There is Card in interrupt
        if (sim_int_status & SIMSTS0_B_CARD_IN)
        {
            int_record.int_type = TB_SIMINT_CARD_IN;
            IsrWriteBuffer (int_record);
            s_sim_register_map[card_id]->iclr = SIMICLR_B_CARD_IN;  //clear the Card in interrupt
            result = CALL_HISR;
        }
    }

    //There is Early ATR interrupt
    if (sim_int_status & SIMSTS0_B_EARLY_ATR)
    {
        if ( (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_CARD_OUT_LATCHED) != 0)
        {
            int_record.int_type = TB_SIMINT_CARD_OUT;
            IsrWriteBuffer (int_record);
            s_sim_register_map[card_id]->ie = 0;
            result = CALL_HISR;
        }

        s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_EN;
        s_sim_register_map[card_id]->iclr = SIMICLR_B_EARLY_ATR; //clear the Early ATR interrupt
    }

    //There is Active done interrupt
    if (sim_int_status & SIMSTS0_B_ACTIVE_DONE)
    {
        //We have no handler to handle this interrupt
        s_sim_register_map[card_id]->iclr = SIMICLR_B_ACTIVE_DONE; //clear the active done interrupt
    }

    if (SC6810 != s_simdrv_chip_type)//SC6810 has no this interrupt
    {
        //There is card out interrupt
        if (sim_int_status & SIMSTS0_B_CARD_OUT)
        {
            int_record.int_type = TB_SIMINT_CARD_OUT;
            IsrWriteBuffer (int_record);
            s_sim_register_map[card_id]->iclr = SIMICLR_B_CARD_OUT;  //clear the Card out interrupt
            result = CALL_HISR;
        }
    }

    //There is the TX Empty interrupt
    if (sim_int_status & SIMSTS0_B_TX_FIFO_EMPTY)
    {
        SIMDRV_DisableSIMTXEmptyInt ( (uint8) card_id);
        s_sim_register_map[card_id]->iclr = SIMICLR_B_TX_EMPTY; //clear the TX Empty interrupt
        int_record.int_type = TB_SIMINT_TX_EMPTY;
        IsrWriteBuffer (int_record);
        result = CALL_HISR;
    }

    //There is RX full interrupt
    if (sim_int_status & SIMSTS0_B_RX_FIFO_FULL)
    {
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_TOUT;     //disable the rx time out interrupt
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;     //disable the rx interrupt

        fifo_count = SIMDRV_GetRxFifoCount ( (uint8) card_id);

        if (fifo_count != 0)
        {
            SIMDRV_WriteIntRxBufData (card_id,fifo_count);
            int_record.int_type = TB_SIMINT_RX_FULL;
            IsrWriteBuffer (int_record);
            result = CALL_HISR;
        }
        else
        {
            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_TOUT;    //Enable the rx time out interrupt
            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
        }

        s_sim_register_map[card_id]->iclr = SIMICLR_B_RX_FULL;
    }

    //There is Rx time out interrupt
    if (sim_int_status & SIMSTS0_B_RX_TOUT)
    {
        s_sim_register_map[card_id]->iclr = SIMICLR_B_RX_TOUT;   //clear the RX full int
        s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_TOUT;     //disable the rx time out interrupt

        fifo_count = SIMDRV_GetRxFifoCount ( (uint8) card_id);

        if (fifo_count != 0)
        {
            SIMDRV_WriteIntRxBufData (card_id,fifo_count);
            int_record.int_type = TB_SIMINT_RX_FULL;
            IsrWriteBuffer (int_record);
            result = CALL_HISR;
        }
        else
        {
            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_TOUT;
        }
    }

    return result;
}

/**********************************************************************
//    Description:
//      Hisr function
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_HisrFunc (uint32 i, void *d)
{
    SIM_INT_RECORD  int_type = {0};

    while (!threadReadBuffer (&int_type))
    {
        SIMDRV_HandleHisr (int_type);
    }
}

/**********************************************************************
//    Description:
//      Be called by Hisr
//    Global resource dependence :
//    Author:Mingwei.Zhang
//    Note:
***********************************************************************/
static void SIMDRV_HandleHisr (SIM_INT_RECORD int_record)
{
    SIMDRV_CARD_ID_E card_id;
    TB_SIM_INT_TYPE_E int_type ;

    card_id = int_record.card_id;
    int_type = int_record.int_type;

    SCI_ASSERT (card_id < SIMDRV_CARD_NUM); /*assert verified*/

    switch (int_type)
    {
        case TB_SIMINT_RX_PARITY_ERROR:
        case TB_SIMINT_TX_PARITY_ERROR:
            SIMDRV_ClearRxIntBuf (card_id);
            SIMDRV_IntCallBack (card_id, int_type);
            break;

        case TB_SIMINT_CARD_UNRESPONSE:
        case TB_SIMINT_CARD_IN:
        case TB_SIMINT_CARD_OUT:
        case TB_SIMINT_TX_EMPTY:
            SIMDRV_IntCallBack (card_id, int_type);
            break;

        case TB_SIMINT_RX_FULL:

            if (SIMDRV_GetRxIntBufCount (card_id))
            {
                SIMDRV_IntCallBack (card_id, int_type);    //Receive the RX data
            }

            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_TOUT;      //Enable the rx time out interrupt
            s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;      //Enable the rx interrupt
            break;

        default:
            SCI_ASSERT (0); /*assert verified*/
            break;
    }

    return ;
}


/**********************************************************************
//    Description:
//      Enable SIM Tx Empty Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMTXEmptyInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie |= SIMIE_B_TX_EMPTY;
}

/**********************************************************************
//    Description:
//      Disable SIM Tx Empty Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMTXEmptyInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_EMPTY;
}

/**********************************************************************
//    Description:
//      Enable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMRXInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_FULL;
    s_sim_register_map[card_id]->ie |= SIMIE_B_RX_TOUT;
}

/**********************************************************************
//    Description:
//      Disable SIM Rx Full and Time Out Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMRXInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_FULL;
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_RX_TOUT;
}

/**********************************************************************
//    Description:
//      Disable SIM Tx Parity Err Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIM_TxParityInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie &= ~SIMIE_B_TX_PARITY_ERR;
}

/**********************************************************************
//    Description:
//      Disable All the SIM Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMAllInt (uint8 card_id)
{
    s_sim_register_map[card_id]->ie = 0;
}

/**********************************************************************
//    Description:
//      Enable the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMCLK (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl1 |= SIMCTL1_B_CLK_ENABLE;
}

/**********************************************************************
//    Description:
//      Disable the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMCLK (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_ENABLE;
}

/**********************************************************************
//    Description:
//      Check wether the SIM clock is enabled
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsSIMCLKEnabled (uint8 card_id)
{
    if (s_sim_register_map[card_id]->ctl1 & SIMCTL1_B_CLK_ENABLE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      Send a data to SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SendData (uint8 card_id, uint32 data)
{
    s_sim_register_map[card_id]->tx = data;
}

/**********************************************************************
//    Description:
//      Receive a data from SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_RecvData (uint8 card_id)
{
    uint32 temp = s_sim_register_map[card_id]->rx;
    return temp;
}

/**********************************************************************
//    Description:
//      Enable SIM Tx
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableTx (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_EN;
}

/**********************************************************************
//    Description:
//      Disable SIM Tx
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableTx (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_TX_EN;
}

/**********************************************************************
//    Description:
//      Reset the chip's SIM Tx FIFO
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ResetTxFifo (uint8 card_id)
{
    uint8 i = 0;
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_FIFO_RST;

    for (i = 0; i < 10; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_TX_FIFO_RST;
}

/**********************************************************************
//    Description:
//      Reset the chip's SIM Rx FIFO
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ResetRxFifo (uint8 card_id)
{
    uint8 i = 0;
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_RX_FIFO_RST;

    for (i = 0; i < 10; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_RX_FIFO_RST;
}

/**********************************************************************
//    Description:
//      Set the Rx water mark. When Baseband chip get the num of data from sim card,
//      it will occour a SIM Rx Full Int.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetRxWaterMark (uint8 card_id, uint8 water_mark)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    s_sim_register_map[card_id]->ctl2 = ((s_sim_register_map[card_id]->ctl2 & 0xff00) | water_mark);
#else
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id],water_mark);
#endif
}

/**********************************************************************
//    Description:
//      Enable SIM RX
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_EnableRx (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_RX_EN;
}

/**********************************************************************
//    Description:
//      Disable SIM RX
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableRx (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_RX_EN;
}

/**********************************************************************
//    Description:
//      Get the num of data in the SIM Tx FIFO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint8 SIMDRV_GetTxFifoCount (uint8 card_id)
{
    uint8 temp = (uint8) GET_TX_FIFO_COUNT (s_sim_register_map[card_id]);
    return temp;
}

/**********************************************************************
//    Description:
//      Get the num of data in the SIM Rx FIFO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint8 SIMDRV_GetRxFifoCount (uint8 card_id)
{
    uint8 temp = (uint8) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);
    return temp;
}

/**********************************************************************
//    Description:
//      Disable the Baseband chip's retransmit function.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableRetransmit (uint8 card_id)
{
    s_sim_register_map[card_id]->she &= ~ (SIM_SEH_RX_RETRX_LMT_MASK |
                                           SIM_SEH_TX_RETRX_LMT_MASK);
}

/**********************************************************************
//    Description:
//      Set the SIM RST pin
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMRST (uint8 card_id, BOOLEAN value)
{
    if (value)
    {
        s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_SIM_RST;
    }
    else
    {
        s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_SIM_RST;
    }
}

/**********************************************************************
//    Description:
//      To get wether the SIM RST pin high?
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsSIMRSTHigh (uint8 card_id)
{
    if (0 == (SIMCTL0_B_SIM_RST & s_sim_register_map[card_id]->ctl0))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/**********************************************************************
//    Description:
//      Set SIM active
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMActive (uint8 card_id)
{
    //we do not use this any more
    //s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_EN_ACTIVE;
}

/**********************************************************************
//    Description:
//      Set SIM deactive
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMDeactive (uint8 card_id)
{
    //we do not use this any more
    //s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_EN_ACTIVE;
}

/**********************************************************************
//    Description:
//      change the convention and logic level of baseband chip
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ChangeConvetion (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 ^= (SIMCTL0_B_BIT_CONVENTION | SIMCTL0_B_LOGIC_LEVEL);
}

/**********************************************************************
//    Description:
//      change the parity of baseband chip
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ChangeParity (uint8 card_id)
{
    s_sim_register_map[card_id]->ctl0 ^= SIMCTL0_B_EVEN_PARITY;
}

/**********************************************************************
//    Description:
//      Set the SIM clock's divide factor
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMClkDivisor (uint8 card_id, uint32 clk_divisor)
{
    s_sim_register_map[card_id]->rx_clkd = clk_divisor;
    s_sim_register_map[card_id]->tx_clkd = clk_divisor;
}

/**********************************************************************
//    Description:
//      config the SIM clock
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIMDRV_CLK_MODE_E SIMDRV_ConfigSIMClk (uint8 card_id)
{

    SIMDRV_CLK_MODE_E clk_mode = SIM_ARM_CLK_DIV_2;//assign a default value

    uint32 cur_clk = 0;
    uint32 clk_divisor = 0;
    uint16 clk_dvd_factors[] = {2,4,8,16,32,64,128,256};//clk division factors

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    s_sim_register_map[card_id]->ctl2 = ((s_sim_register_map[card_id]->ctl2 & 0xff00) | 1);
    s_sim_register_map[card_id]->ctl2 = ((s_sim_register_map[card_id]->ctl2 & 0x00ff) | (0 << 8));
#else
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id],1);
    s_sim_register_map[card_id]->ctl1 = SET_TX_INT_MARK (s_sim_register_map[card_id],0);
#endif
    s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_POLARITY;
    s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_ENABLE;

    s_sim_register_map[card_id]->ctl1 &= ~ (SIM_CTL1_CLOCK_MODE_MASK);

    cur_clk = CHIP_GetAPBClk();

    if (ARM_CLK_13M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_4;

        s_sim_register_map[card_id]->ctl1 |= (SIM_CTL1_CLOCK_MODE_ARM_CLK_DIV4 <<
                                              SIM_CTL1_CLOCK_MODE_SHIFT);
    }
    else if (ARM_CLK_26M == cur_clk)
    {
        clk_mode = SIM_ARM_CLK_DIV_8;

        s_sim_register_map[card_id]->ctl1 |= (SIM_CTL1_CLOCK_MODE_ARM_CLK_DIV8 <<
                                              SIM_CTL1_CLOCK_MODE_SHIFT);
    }
    else
    {
        SCI_ASSERT (FALSE);/*assert to do*/
    }

    clk_divisor = TB_SIM_DEFAULT_FACTOR_F/TB_SIM_DEFAULT_FACTOR_D * clk_dvd_factors[clk_mode];
    SIMDRV_SetSIMClkDivisor (card_id,clk_divisor);

    return clk_mode;
}

/**********************************************************************
//    Description:
//      Set the logic level of SIM CLK pin when it stoped
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetSIMClkStopLevel (uint8 card_id, BOOLEAN bHighLevel)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    if (0 == card_id)
    {
        if(bHighLevel)
        {
            *(volatile uint32 *)APB_MISC_CTL0 |= SIM0_CLK_PL;
        }
        else
        {
            *(volatile uint32 *)APB_MISC_CTL0 &= ~SIM0_CLK_PL;
        }
    }
    else if (1 == card_id)
    {
        if(bHighLevel)
        {
            *(volatile uint32 *)APB_MISC_CTL0 |= SIM1_CLK_PL;
        }
        else
        {
            *(volatile uint32 *)APB_MISC_CTL0 &= ~SIM1_CLK_PL;
        }
    }
    else if (2 == card_id)
    {
        if(bHighLevel)
        {
            *(volatile uint32 *)APB_MISC_CTL0 |= SIM2_CLK_PL;
        }
        else
        {
            *(volatile uint32 *)APB_MISC_CTL0 &= ~SIM2_CLK_PL;
        }
    }
    else
   {
        //SCI_ASSERT(0);
   }

#else
    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        if (bHighLevel)
        {
            s_sim_register_map[card_id]->ctl1 |= SIMCTL1_B_CLK_POLARITY;
        }
        else
        {
            s_sim_register_map[card_id]->ctl1 &= ~SIMCTL1_B_CLK_POLARITY;
        }
    }
    else//SC6800H3
    {
        if (bHighLevel)
        {
            if (0 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_26;
            }
            else if (1 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_27;
            }
            else if (2 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_28;
            }
            else
            {
                * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_29;
            }
        }
        else
        {
            if (0 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_26;
            }
            else if (1 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_27;
            }
            else if (2 == card_id)
            {
                * (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_28;
            }
            else
            {
                * (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_29;
            }
        } 
    }
#endif
}

/**********************************************************************
//    Description:
//      Set the working voltage of the sim card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_SetSIMVoltage (uint8 card_id, SIMDRV_WORKING_VOLT_E work_vol)
{
    LDO_ID_E ldo_id = LDO_LDO_SIM0;
    LDO_VOLT_LEVEL_E volt_level = LDO_VOLT_LEVEL0;

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    if (0 == card_id)
    {
        ldo_id = LDO_LDO_SIM0;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 1.8v."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1177_112_2_17_23_6_28_1140,(uint8*)"d",card_id);
            volt_level = LDO_VOLT_LEVEL0;
        }
        else if (work_vol == SIMDRV_WORKING_VOLT_3V)
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 3.0v."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1182_112_2_17_23_6_28_1141,(uint8*)"d",card_id);
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
            volt_level = LDO_VOLT_LEVEL1;
#else
            volt_level = LDO_VOLT_LEVEL2;
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::do not change the working volt."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1187_112_2_17_23_6_28_1142,(uint8*)"d",card_id);
            return;
        }
    }
    else if (1 == card_id)
    {
        ldo_id = LDO_LDO_SIM1;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
    		//SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 1.8v."
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1196_112_2_17_23_6_28_1143,(uint8*)"d",card_id);
    		volt_level = LDO_VOLT_LEVEL0;
        }
        else if (work_vol == SIMDRV_WORKING_VOLT_3V)
        {
             //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 3.0v."
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1201_112_2_17_23_6_28_1144,(uint8*)"d",card_id);
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
            volt_level = LDO_VOLT_LEVEL1;
#else
            volt_level = LDO_VOLT_LEVEL2;
#endif
        }
        else
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::do not change the working volt."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1206_112_2_17_23_6_28_1145,(uint8*)"d",card_id);
            return;
        }
    }
    else if (2 == card_id)
    {
        #if !defined(PLATFORM_SC6531EFM)  && !defined(PLATFORM_UWS6121E)   
        ldo_id = LDO_LDO_SIM2;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
		    //SCI_TRACE_LOW:"SIMDRV2::set the working volt to 1.8v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1215_112_2_17_23_6_28_1146,(uint8*)"");
            volt_level = LDO_VOLT_LEVEL0;
        }
        else
        {
		    //SCI_TRACE_LOW:"SIMDRV2::set the working volt to 3v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1220_112_2_17_23_6_28_1147,(uint8*)"");
#if defined(CHIP_VER_6531)
            volt_level = LDO_VOLT_LEVEL1;
#else
            volt_level = LDO_VOLT_LEVEL2;
#endif
        }
        #endif
    }
    else//sim ldo 3
    {
        ldo_id = 0; //for pclint warning
        volt_level = 0; //for pclint warning
        //SCI_ASSERT(0);
    }

   #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)   
    switch(volt_level)
    {
        case LDO_VOLT_LEVEL0:
        case LDO_VOLT_LEVEL2:
                LDO_SetVoltValue(ldo_id, SIM_VOLT_VAL1);
                break;
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)    
        case LDO_VOLT_LEVEL1:                        //coverity 41903 dead code for uix8910
                LDO_SetVoltValue(ldo_id, SIM_VOLT_VAL2);
                break;
       default:
                break;
#endif
    }
    #else
    LDO_SetVoltLevel(ldo_id, volt_level);
    #endif
#else
    if (0 == card_id)
    {
        ldo_id = LDO_LDO_SIM0;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 1.8v."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1237_112_2_17_23_6_28_1148,(uint8*)"d",card_id);
            volt_level = LDO_VOLT_LEVEL0;
        }
        else if (work_vol == SIMDRV_WORKING_VOLT_3V)
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 3.0v."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1242_112_2_17_23_6_28_1149,(uint8*)"d",card_id);
            if (SC6810 == s_simdrv_chip_type)
            {
                volt_level = LDO_VOLT_LEVEL2;
            }
            else
            {
                volt_level = LDO_VOLT_LEVEL3;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::do not change the working volt."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1254_112_2_17_23_6_28_1150,(uint8*)"d",card_id);
            return;
        }
    }
    else if (1 == card_id)
    {
        ldo_id = LDO_LDO_SIM1;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
    		//SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 1.8v."
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1263_112_2_17_23_6_28_1151,(uint8*)"d",card_id);
    		volt_level = LDO_VOLT_LEVEL0;
        }
        else if (work_vol == SIMDRV_WORKING_VOLT_3V)
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::set the working volt to 3.0v."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1268_112_2_17_23_6_28_1152,(uint8*)"d",card_id);
            if (SC6810 == s_simdrv_chip_type)
            {
                volt_level = LDO_VOLT_LEVEL2;
            }
            else
            {
                volt_level = LDO_VOLT_LEVEL3;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"\r\nSIMDRV%d::do not change the working volt."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1280_112_2_17_23_6_28_1153,(uint8*)"d",card_id);
            return;
        }
    }
    else if (2 == card_id)
    {
        #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)  
        ldo_id = LDO_LDO_SIM2;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
		    //SCI_TRACE_LOW:"SIMDRV2::set the working volt to 1.8v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1289_112_2_17_23_6_28_1154,(uint8*)"");
            volt_level = LDO_VOLT_LEVEL0;
        }
        else
        {
		    //SCI_TRACE_LOW:"SIMDRV2::set the working volt to 3v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1294_112_2_17_23_6_28_1155,(uint8*)"");
		    volt_level = LDO_VOLT_LEVEL3;
        }
        #endif
    }
    else//sim ldo 3
    {
        ldo_id = LDO_LDO_SIM3;
        if (work_vol == SIMDRV_WORKING_VOLT_1_8V)
        {
		    //SCI_TRACE_LOW:"SIMDRV3::set the working volt to 1.8v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1303_112_2_17_23_6_28_1156,(uint8*)"");
		    volt_level = LDO_VOLT_LEVEL0;
        }
        else
        {
		    //SCI_TRACE_LOW:"SIMDRV3::set the working volt to 3v."
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1308_112_2_17_23_6_28_1157,(uint8*)"");
		    volt_level = LDO_VOLT_LEVEL3;
        }
    }
    LDO_SetVoltLevel(ldo_id, volt_level);
#endif
}

/**********************************************************************
//    Description:
//      .get the ctl0 register's value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetCTL0Value (uint8 card_id)
{
    uint32 temp = s_sim_register_map[card_id]->ctl0;
    return temp;
}

/**********************************************************************
//    Description:
//       set the ctl0 register's value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_SetCTL0Value (uint8 card_id, uint32 data)
{
    s_sim_register_map[card_id]->ctl0 = data;
}

/**********************************************************************
//    Description:
//      .get the FIFO size
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint16 SIMDRV_GetFIFOBufSize (uint8 card_id)
{
    uint16 temp = SIM_FIFO_BUF_SIZE;
    return temp;
}

/**********************************************************************
//    Description:
//      .To get wether the SIM card is outlatched
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDRV_IsCardOutLatched (uint8 card_id)
{
    if ( (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_CARD_OUT_LATCHED) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************
//    Description:
//      .clear some regs before sim reset
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_ClearBeforeReset (uint8 card_id)
{
    uint32 i = 0;
    uint32 rx_fifo_count = 0;
    uint32 temp = 0;

    //clear the rx fifo if needed
    rx_fifo_count = (uint16) GET_RX_FIFO_COUNT (s_sim_register_map[card_id]);

    for (i = 0; i < rx_fifo_count; i++)
    {
        temp = s_sim_register_map[card_id]->rx;
    }

    //clear previous interrupt status
    s_sim_register_map[card_id]->iclr = 0xffff;
    s_sim_register_map[card_id]->she = 0xA;//Enable Rx retransmit 10 times for ***case. ATR bytes will be retransmit. 
    s_sim_register_map[card_id]->wdt = SIM_DEFAULT_WDT;
    s_sim_register_map[card_id]->wdt1 = SIM_DEFAULT_WDT1;
    //set the rx water mark to 0,for the rx timeout interrupt is not enable during the ATR
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    s_sim_register_map[card_id]->ctl2 = ((s_sim_register_map[card_id]->ctl2 & 0xff00) | 1);
#else
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id],1);
#endif
}

/**********************************************************************
//    Description:
//      .begin to reset sim card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_StartToReset (uint8 card_id)
{
    uint8 i = 0;
    uint8 sim_controller_id = 0;

    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        sim_controller_id = card_id;
    }
    else
    {
        if ((0 == card_id) || (2 == card_id))
        {
            sim_controller_id = 0;
        }
        else
        {
            sim_controller_id = 1;
        }
    }

    s_sim_register_map[sim_controller_id]->ie = SIM_DEFAULT_IE;
    s_sim_register_map[sim_controller_id]->ctl0 |= SIMCTL0_B_TX_EN|SIMCTL0_B_RX_EN;

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    s_sim_register_map[sim_controller_id]->ctl0 |= SIMCTL0_B_POWER_EN;
#else
    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        s_sim_register_map[sim_controller_id]->ctl0 |= SIMCTL0_B_POWER_EN;
    }
    else
    {
        if ((0 == card_id) || (1 == card_id))
        {
            s_sim_register_map[sim_controller_id]->ctl0 |= SIMCTL0_B_POWER_EN;
        }
        else if (2 == card_id)
        {
            * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_20;//LDO_SIM2_FORCE_ON
        }
        else
        {
            * (volatile uint32 *) GR_LDO_VCFG1 |= BIT_21;//LDO_SIM3_FORCE_ON
        }
    }
#endif
    s_sim_register_map[sim_controller_id]->ctl1 |= SIMCTL1_B_CLK_ENABLE;

    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[sim_controller_id]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }
}

/**********************************************************************
//    Description:
//      .clear some regs before send a command to SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_BeforeSendData (uint8 card_id)
{
    uint8 i = 0;
    uint32 tx_transmit_num = 0;

    //Clear the Rx/Tx FIFO to avoid invalid data
    s_sim_register_map[card_id]->ctl0 |= (SIMCTL0_B_RX_FIFO_RST|SIMCTL0_B_TX_FIFO_RST);

    for (i = 0; i < 10; i++)
    {
    }

    s_sim_register_map[card_id]->ctl0 &= ~ (SIMCTL0_B_RX_FIFO_RST|SIMCTL0_B_TX_FIFO_RST);

    //set the rx water mark to 1,wait for the one byte ack byte
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    s_sim_register_map[card_id]->ctl2 = ((s_sim_register_map[card_id]->ctl2 & 0xff00) | 1);
#else
    s_sim_register_map[card_id]->ctl1 = SET_RX_INT_MARK (s_sim_register_map[card_id],1);
#endif

    //enable the tx and rx
    s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_EN|SIMCTL0_B_RX_EN;

    //enable the retransmit fucntion to handler rx parity error.
    s_sim_register_map[card_id]->she = SIM_ENABLED_SHE;

    //enable the watch dog
    s_sim_register_map[card_id]->wdt = SIM_ENABLED_WDT;

    tx_transmit_num = (uint16) (GET_TX_FIFO_COUNT (s_sim_register_map[card_id]));
    return tx_transmit_num;
}

/**********************************************************************
//    Description:
//      Get the SIM LDO voltage stable time in ms.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_GetLDOStableTime (void)
{
    uint32 ldo_stable_time = 125;//ms

    //how about SC8800G?
    ldo_stable_time= 1;//In fact, 6600R/H is 0.125ms

    return ldo_stable_time;
}

/**********************************************************************
//    Description:
//      When Vsim stable, init the sim regs.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIMDRV_CLK_MODE_E SIMDRV_InitAfterLDOValid (uint8 card_id)
{
    SIMDRV_CLK_MODE_E clk_mode;

    //enable SIM module
    if (0 == card_id)
    {
        if ((SC6531 == s_simdrv_chip_type) || (SC6530C == s_simdrv_chip_type) ||(SC6531EFM == s_simdrv_chip_type) || (SC6531EFM_AB == s_simdrv_chip_type))
        {
            //SC6500/6531/6531EFM has already open!
        }
        else
        {
            DEVICE_Open (s_dev_sim0);
        }
    }
    else
    {
        DEVICE_Open (s_dev_sim1);
    }

    clk_mode = SIMDRV_ConfigSIMClk (card_id);

    //set the SIM register value
    s_sim_register_map[card_id]->sts0   = SIM_DEFAULT_STS0;//clear the sts0
    s_sim_register_map[card_id]->sts1   = SIM_DEFAULT_STS1;//clear the sts1
    s_sim_register_map[card_id]->ie     = 0;//disable all the interrupter at the beginning
    s_sim_register_map[card_id]->iclr   = SIM_DEFAULT_ICLR;//clear all interrupter
    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        s_sim_register_map[card_id]->ctl0 = SIM_DEFAULT_CTL0;
    }
    else
    {
        if (SIMCTL0_B_POWER_EN == (s_sim_register_map[card_id]->ctl0 & SIMCTL0_B_POWER_EN))
        {
            s_sim_register_map[card_id]->ctl0 = (SIM_DEFAULT_CTL0|SIMCTL0_B_POWER_EN);
        }
        else
        {
            s_sim_register_map[card_id]->ctl0 = SIM_DEFAULT_CTL0;
        }
    }
    s_sim_register_map[card_id]->she    = SIM_DEFAULT_SHE;
    s_sim_register_map[card_id]->tgc    = SIM_DEFAULT_TGC;
    s_sim_register_map[card_id]->wdt    = SIM_DEFAULT_WDT;
    s_sim_register_map[card_id]->wdt1   = SIM_DEFAULT_WDT1;
    s_sim_register_map[card_id]->imsk   = SIM_DEFAULT_IMASKED;

    //enable SIM interrupt
    if (0 == card_id)
    {
        CHIPDRV_EnableIRQINT (TB_SIM_INT);
    }
    else
    {
        CHIPDRV_EnableIRQINT (TB_SIM1_INT);
    }

    return clk_mode;
}

/**********************************************************************
//    Description:
//      power off the SIM card
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_Poweroff (uint8 card_id)
{
    //SIM module is not initial yet, no need to power off sim module
    if (NULL == s_sim_register_map[card_id])
    {
        return;
    }

    //SCI_TRACE_LOW:"SIMDRV%d:power off"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_1599_112_2_17_23_6_28_1158,(uint8*)"d",card_id);
    s_sim_register_map[card_id]->ie = 0;
    SIMDRV_RejectSIMCard (card_id);
    SIMDRV_SetSIMVoltage (card_id,SIMDRV_WORKING_VOLT_1_8V);

    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
        if(SIM_DMA_NO_CHN_VAL != s_sim_dma_rx_chn) 
            DMA_HAL_ChnEnable (s_sim_dma_rx_chn, FALSE);
        if(SIM_DMA_NO_CHN_VAL != s_sim_dma_tx_chn) 
            DMA_HAL_ChnEnable (s_sim_dma_tx_chn, FALSE);
#else
        DMA_HAL_ChnEnable (DMA_SIM0_RX, FALSE);
        DMA_HAL_ChnEnable (DMA_SIM0_TX, FALSE);
#endif
    }
    else if (1 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))	
        if(SIM_DMA_NO_CHN_VAL != s_sim_dma_rx_chn) 
            DMA_HAL_ChnEnable (s_sim_dma_rx_chn, FALSE);
        if(SIM_DMA_NO_CHN_VAL != s_sim_dma_tx_chn) 
            DMA_HAL_ChnEnable (s_sim_dma_tx_chn, FALSE);
#else
        DMA_HAL_ChnEnable (DMA_SIM1_RX, FALSE);
        DMA_HAL_ChnEnable (DMA_SIM1_TX, FALSE);
#endif
    }

    if (0 == card_id)
    {
        CHIPDRV_DisableIRQINT (TB_SIM_INT);
    }
    else if (1 == card_id)
    {
        CHIPDRV_DisableIRQINT (TB_SIM1_INT);
    }
    else
    {
        SCI_ASSERT (FALSE);/*assert verified*/
    }

    //disable SIM module
    if (0 == card_id)
    {
        DEVICE_Close (s_dev_sim0);
    }
    else if (1 == card_id)
    {
        DEVICE_Close (s_dev_sim1);
    }
    else
    {
        SCI_ASSERT (FALSE);/*assert verified*/
    }
}

/**********************************************************************
//    Description:
//      Reject the SIM card(deactive the SIM card and Power off the SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDRV_RejectSIMCard (uint8 card_id)
{
    uint32 i = 0;
    uint8 sim_controller_id = 0;

    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        sim_controller_id = card_id;
    }
    else if ((SC6530_ID_BASE < s_simdrv_chip_type) && (SC6530_ID_MAX > s_simdrv_chip_type))
    {
        sim_controller_id = 0;
    }
    else if ((SC6531EFM_ID_BASE < s_simdrv_chip_type) && (SC6531EFM_ID_MAX > s_simdrv_chip_type))
    {
        sim_controller_id = 0;
    }
    else
    {
        if ((0 == card_id) || (2 == card_id))
        {
            sim_controller_id = 0;
        }
        else
        {
            sim_controller_id = 1;
        }
    }

    s_sim_register_map[sim_controller_id]->ctl0 &= ~SIMCTL0_B_SIM_RST;

    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[sim_controller_id]->ctl1 &= ~SIMCTL1_B_CLK_ENABLE;

    for (i = 0; i < 100; i++)
    {
    }

    s_sim_register_map[sim_controller_id]->ctl0 |= SIMCTL0_B_TX_DATA_OUT_LOW;

    for (i = 0; i < 100; i++)
    {
    }

#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    if (0 == card_id)
    {
        LDO_TurnOffLDO(LDO_LDO_SIM0);
    }
    else if(1 == card_id)
    {
        LDO_TurnOffLDO(LDO_LDO_SIM1);
    }
    else if(2 == card_id)
    {
        #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E) 
        LDO_TurnOffLDO(LDO_LDO_SIM2);
        #endif
    }
    else
    {
        //SCI_ASSERT(0);
    }    
#else
    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        s_sim_register_map[sim_controller_id]->ctl0 &= ~SIMCTL0_B_POWER_EN;
        if (SC6810 == s_simdrv_chip_type)
        {
            if (0 == card_id)
            {
                LDO_TurnOffLDO(LDO_LDO_SIM0);
            }
            else
            {
                LDO_TurnOffLDO(LDO_LDO_SIM1);
            }
        }
    }
    else
    {
    	if ((0 == card_id) || (1 == card_id))
    	{
    		s_sim_register_map[sim_controller_id]->ctl0 &= ~SIMCTL0_B_POWER_EN;
    	}
    	else if (2 == card_id)
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_20;//LDO_SIM2_FORCE_ON
    	}
    	else
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_21;//LDO_SIM3_FORCE_ON
    	}
    }
#endif
}

/**********************************************************************
//    Description:
//      Have received all the data from sim card, so clear some regs about Rx.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_CompleteRX (uint8 card_id)
{
    //disable the watch dog
    s_sim_register_map[card_id]->wdt = SIM_DEFAULT_WDT;

    //disable the rx and tx
    s_sim_register_map[card_id]->ctl0 &= ~ (SIMCTL0_B_RX_EN|SIMCTL0_B_TX_EN);

    //disable the retransmit fucntion,the rx parity error handler by software.
    s_sim_register_map[card_id]->she = SIM_DEFAULT_SHE;
}

/**********************************************************************
//    Description:
//      SIM DMA channel init
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_Init (uint8 card_id,
                      DMA_HAL_ISR_HANDLER sim_tx_handler,
                      DMA_HAL_ISR_HANDLER sim_rx_handler,
                      DMA_CHNCFG_PTR sim_tx_dma_cfg,
                      DMA_CHNCFG_PTR sim_rx_dma_cfg)
{
    //Initialize the SIM DMA channel.
    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
        //Alloc a idle dma channel with your uid     
        SIM_DMA_ALLOC(DMA_SIM_TX_REQID,s_sim_dma_tx_chn);
        SIM_DMA_ALLOC(DMA_SIM_RX_REQID,s_sim_dma_rx_chn);
        
        DMA_HAL_ChnIntConfig (s_sim_dma_tx_chn, DMA_CHN_INT_TYPE_TRANSDONE, sim_tx_handler);
        DMA_HAL_ChnIntConfig (s_sim_dma_rx_chn, DMA_CHN_INT_TYPE_TRANSDONE, sim_rx_handler);     

        DMA_HAL_ChnConfig (s_sim_dma_tx_chn, sim_tx_dma_cfg);
        DMA_HAL_ChnConfig (s_sim_dma_rx_chn, sim_rx_dma_cfg);
#else
        DMA_HAL_ChnIntConfig (DMA_SIM0_TX, DMA_CHN_INT_TYPE_TRANSDONE, sim_tx_handler);
        DMA_HAL_ChnIntConfig (DMA_SIM0_RX, DMA_CHN_INT_TYPE_TRANSDONE, sim_rx_handler);
        DMA_HAL_ChnConfig (DMA_SIM0_TX, sim_tx_dma_cfg);
        DMA_HAL_ChnConfig (DMA_SIM0_RX, sim_rx_dma_cfg);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
#else
        DMA_HAL_ChnIntConfig (DMA_SIM1_TX, DMA_CHN_INT_TYPE_TRANSDONE, sim_tx_handler);
        DMA_HAL_ChnIntConfig (DMA_SIM1_RX, DMA_CHN_INT_TYPE_TRANSDONE, sim_rx_handler);
        DMA_HAL_ChnConfig (DMA_SIM1_TX, sim_tx_dma_cfg);
        DMA_HAL_ChnConfig (DMA_SIM1_RX, sim_rx_dma_cfg);
#endif
    }
}

/**********************************************************************
//    Description:
//      SIM DMA TX channel config
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Cfg (uint8 card_id, DMA_CHNCFG_PTR tx_dma_cfg)
{
    if (0 == card_id)
    {

        cacheTxConfig[0].bufferAddr = tx_dma_cfg->src_address;
        cacheTxConfig[0].bufferSize = tx_dma_cfg->total_length;
        cacheTxConfig[0].dir = DMABUFFER_TO_DEVICE;
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
		DMA_HAL_ChnConfig (s_sim_dma_tx_chn, tx_dma_cfg);
#else
        DMA_HAL_ChnConfig (DMA_SIM0_TX, tx_dma_cfg);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        cacheTxConfig[1].bufferAddr = tx_dma_cfg->src_address;
        cacheTxConfig[1].bufferSize = tx_dma_cfg->total_length;
        cacheTxConfig[1].dir = DMABUFFER_TO_DEVICE;
        DMA_HAL_ChnConfig (DMA_SIM1_TX, tx_dma_cfg);
#endif

    }
}

/**********************************************************************
//    Description:
//      SIM DMA RX channel config
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Cfg (uint8 card_id, DMA_CHNCFG_PTR rx_dma_cfg)
{
    if (0 == card_id)
    {
        cacheRxConfig[0].bufferAddr = rx_dma_cfg->dest_address;
        cacheRxConfig[0].bufferSize = rx_dma_cfg->total_length;
        cacheRxConfig[0].dir = DMABUFFER_FROM_DEVICE;
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
		DMA_HAL_ChnConfig (s_sim_dma_rx_chn, rx_dma_cfg);
#else
        DMA_HAL_ChnConfig (DMA_SIM0_RX, rx_dma_cfg);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        cacheRxConfig[1].bufferAddr = rx_dma_cfg->dest_address;
        cacheRxConfig[1].bufferSize = rx_dma_cfg->total_length;
        cacheRxConfig[1].dir = DMABUFFER_FROM_DEVICE;
        DMA_HAL_ChnConfig (DMA_SIM1_RX, rx_dma_cfg);
#endif
    }
}

/**********************************************************************
//    Description:
//      SIM DMA TX channel enable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Enable (uint8 card_id)
{
    //enable sim tx fifo control by DMA
    s_sim_register_map[card_id]->she |= SIM_SEH_DMA_TX_EN_MASK;

    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
		DMA_HAL_ChnEnable_Ex (s_sim_dma_tx_chn, SCI_TRUE, &cacheTxConfig[0], 1);
#else
        DMA_HAL_ChnEnable_Ex (DMA_SIM0_TX, SCI_TRUE, &cacheTxConfig[0], 1);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        DMA_HAL_ChnEnable_Ex (DMA_SIM1_TX, SCI_TRUE, &cacheTxConfig[1], 1);
#endif
    }
}

/**********************************************************************
//    Description:
//      SIM DMA TX channel disable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_TX_Disable (uint8 card_id)
{
    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
        DMA_HAL_ChnEnable (s_sim_dma_tx_chn, FALSE);
#else
        DMA_HAL_ChnEnable (DMA_SIM0_TX, FALSE);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        DMA_HAL_ChnEnable (DMA_SIM1_TX, FALSE);
#endif
    }

    //disable sim tx fifo control by DMA
    s_sim_register_map[card_id]->she &= ~ (SIM_SEH_DMA_TX_EN_MASK);
}

/**********************************************************************
//    Description:
//      SIM DMA RX channel enable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Enable (uint8 card_id)
{
    //enable sim rx fifo control by DMA
    s_sim_register_map[card_id]->she |= SIM_SEH_DMA_RX_EN_MASK;

    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
        DMA_HAL_ChnEnable_Ex (s_sim_dma_rx_chn, SCI_TRUE, &cacheRxConfig[0], 1);
#else
        DMA_HAL_ChnEnable_Ex (DMA_SIM0_RX, SCI_TRUE, &cacheRxConfig[0], 1);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        DMA_HAL_ChnEnable_Ex (DMA_SIM1_RX, SCI_TRUE, &cacheRxConfig[1], 1);
#endif
    }
}

/**********************************************************************
//    Description:
//      SIM DMA RX channel disable
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_RX_Disable (uint8 card_id)
{
    if (0 == card_id)
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
        DMA_HAL_ChnEnable (s_sim_dma_rx_chn, FALSE);
#else
        DMA_HAL_ChnEnable (DMA_SIM0_RX, FALSE);
#endif
    }
    else
    {
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))

#else
        DMA_HAL_ChnEnable (DMA_SIM1_RX, FALSE);
#endif
    }

    //disable sim rx fifo control by DMA
    s_sim_register_map[card_id]->she &= ~ (SIM_SEH_DMA_RX_EN_MASK);

}

/**********************************************************************
//    Description:
//      Get the transfered bytes in one SIM DMA transfer.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIMDRV_DMA_RX_GetTransferedBytes (uint8 card_id)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    if (0 == card_id)
    {
        return DMA_HAL_StdChn_Transfered_Data(s_sim_dma_rx_chn);
    }
#endif

    return 0;
}

/**********************************************************************
//    Description:
//      clear SIM DMA Int
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DMA_ClearAllInt (uint8 card_id)
{
}

/**********************************************************************
//    Description:
//      SC6800H3 chip, SIM0/1/2/3 switch.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_L_Switch (uint8 card_id)
{
#if (defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
    uint32 tmp = 0;
    tmp =	* (volatile uint32 *) APB_PIN_CTL0;
    tmp &= ~SIM_SEL_MSK;
    tmp |= card_id << SIM_SEL_SHIFT;
    * (volatile uint32 *) APB_PIN_CTL0 = tmp;    
#else
    if ((SC6800H == s_simdrv_chip_type) || (SC6810 == s_simdrv_chip_type))
    {
        
    }
    else
    {
    	if (0 == card_id)
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_24;
    	}
    	else if (1 == card_id)
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 &= ~BIT_25;
    	}
    	else if (2 == card_id)
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 |= BIT_24;
    	}
    	else
    	{
    		* (volatile uint32 *) GR_LDO_VCFG1 |= BIT_25;
    	}
    }
#endif
}

/**********************************************************************
//    Description:
//     Enable the SIM's LDO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_EnableSIMLDO (uint8 card_id)
{
    if (SC6810 == s_simdrv_chip_type)
    {
        if (0 == card_id)
        {
            LDO_TurnOnLDO (LDO_LDO_SIM0);
        }
        else
        {
            LDO_TurnOnLDO (LDO_LDO_SIM1);
        }
    }
    else if ((SC6530_ID_BASE < s_simdrv_chip_type) && (SC6530_ID_MAX > s_simdrv_chip_type))
    {
        if (0 == card_id)
        {
            LDO_TurnOnLDO (LDO_LDO_SIM0);
        }
        else if(1 == card_id)
        {
            LDO_TurnOnLDO (LDO_LDO_SIM1);
        }
        else
        {
            #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)  
            LDO_TurnOnLDO (LDO_LDO_SIM2);
            #endif
        }
    }
    else if ((SC6531EFM_ID_BASE < s_simdrv_chip_type) && (SC6531EFM_ID_MAX > s_simdrv_chip_type))
    {
        if (0 == card_id)
        {
            LDO_TurnOnLDO (LDO_LDO_SIM0);
        }
        else if(1 == card_id)
        {
            LDO_TurnOnLDO (LDO_LDO_SIM1);
        }
        else
        {
            #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)   
            LDO_TurnOnLDO (LDO_LDO_SIM2);
            #endif
        }
    }
        
}

/**********************************************************************
//    Description:
//     Disable the SIM's LDO.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_DisableSIMLDO (uint8 card_id)
{
    if (SC6810 == s_simdrv_chip_type)
    {
        if (0 == card_id)
        {
            LDO_TurnOffLDO (LDO_LDO_SIM0);
        }
        else
        {
            LDO_TurnOffLDO (LDO_LDO_SIM1);
        }
    }
    else if ((SC6530_ID_BASE < s_simdrv_chip_type) && (SC6530_ID_MAX > s_simdrv_chip_type))
    {
        if (0 == card_id)
        {
            LDO_TurnOffLDO (LDO_LDO_SIM0);
        }
        else if(1 == card_id)
        {
            LDO_TurnOffLDO (LDO_LDO_SIM1);
        }
        else
        {
            #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)   
            LDO_TurnOffLDO (LDO_LDO_SIM2);
            #endif
        }
    }
    else if ((SC6531EFM_ID_BASE < s_simdrv_chip_type) && (SC6531EFM_ID_MAX > s_simdrv_chip_type))
    {
        if (0 == card_id)
        {
            LDO_TurnOffLDO (LDO_LDO_SIM0);
        }
        else if(1 == card_id)
        {
            LDO_TurnOffLDO (LDO_LDO_SIM1);
        }
        else
        {
            #if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)    
            LDO_TurnOffLDO (LDO_LDO_SIM2);
            #endif
        }
    }
}

/**********************************************************************
//    Description:
//     Set the SIM I/O pin's state.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_Set_IO_State (uint8 card_id, BOOLEAN state)
{
    if (state)
    {
        s_sim_register_map[card_id]->ctl0 &= ~SIMCTL0_B_TX_DATA_OUT_LOW;
    }
    else
    {
        s_sim_register_map[card_id]->ctl0 |= SIMCTL0_B_TX_DATA_OUT_LOW;
    }
}

/**********************************************************************
//    Description:
//      trace the register value
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDRV_PrintRegisterBuf (uint8 card_id)
{
    uint32 sim_tx,sim_rx = 0,sim_sts0,sim_sts1,sim_ie,sim_iclr,sim_ctl0;
    uint32 sim_ctl1,sim_rx_clkd,sim_she,sim_tgc,sim_wdt,sim_imsk;
    uint32 sim_tx_clkd,sim_wdt1,sim_sts2,sim_ctl2;
    sim_tx = s_sim_register_map[card_id]->tx;
    //sim_rx = s_sim_register_map[card_id]->rx;
    sim_sts0 = s_sim_register_map[card_id]->sts0;
    sim_sts1 = s_sim_register_map[card_id]->sts1;
    sim_ie = s_sim_register_map[card_id]->ie;
    sim_iclr = s_sim_register_map[card_id]->iclr;
    sim_ctl0 = s_sim_register_map[card_id]->ctl0;
    sim_ctl1 = s_sim_register_map[card_id]->ctl1;
    sim_rx_clkd = s_sim_register_map[card_id]->rx_clkd;
    sim_she = s_sim_register_map[card_id]->she;
    sim_tgc = s_sim_register_map[card_id]->tgc;
    sim_wdt = s_sim_register_map[card_id]->wdt;
    sim_imsk = s_sim_register_map[card_id]->imsk;
    sim_tx_clkd = s_sim_register_map[card_id]->tx_clkd;
    sim_wdt1 = s_sim_register_map[card_id]->wdt1;
    sim_sts2 = s_sim_register_map[card_id]->sts2;
    sim_ctl2 = s_sim_register_map[card_id]->ctl2;
    SCI_TRACE_LOW("SIMDRV:reg 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x.",sim_tx,sim_rx,sim_sts0,sim_sts1,sim_ie,sim_iclr,sim_ctl0,sim_ctl1,sim_rx_clkd,sim_she,sim_tgc,sim_wdt,sim_imsk,sim_tx_clkd,sim_wdt1,sim_sts2,sim_ctl2);
    //SCI_TRACE_LOW:"SIMDRV:reg 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_DRV_2167_112_2_17_23_6_29_1159,(uint8*)"ddddddddddddddd",sim_tx,sim_rx,sim_sts0,sim_sts1,sim_ie,sim_iclr,sim_ctl0,sim_ctl1,sim_rx_clkd,sim_she,sim_tgc,sim_wdt,sim_imsk,sim_tx_clkd,sim_wdt1,sim_sts2);
}

/**********************************************************************
//    Description:
//      PowerOff all the SIM card and disable the SIM module
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
void SIM_PowerOffSIM (void)
{
    SIMDRV_Poweroff (0);
    SIMDRV_Poweroff (1);
}
#ifdef   __cplusplus
}
#endif



/*******************************************************************************
 ** File Name:      sdslave_phy_v0.c                                           *
 ** Author:         andy.chen                                                  *
 ** Date:           12/29/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.12        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "../sdslave_phy.h"
#include "isr_drvapi.h"
#include "ldo_drvapi.h"
/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
#define ISR_MSGQUEUE_SIZE   5   // 1+4 redundance    

#define _HANDLE_VERIFY(handle)   (((handle) == &s_sdslavePort)?(SCI_TRUE):(SCI_FALSE))
/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
typedef struct SDSLAVE_PHY_TAG
{
    volatile SDSLAVE_REGCFG_T *sdslave_cfg;
    uint32 base_clk;
    SDSLAVE_CALLBACK sigCallBack;
} SDSLAVE_PHY_T;

typedef struct
{
    uint32 msg;
    SDSLAVE_PHY_HANDLE sdslave_handler;
} ISR_MsgQueue_T;

INPUT_BUFFER_INIT (ISR_MsgQueue_T, 5)

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/
LOCAL SDSLAVE_PHY_T s_sdslavePort;

/*******************************************************************************
 **                    LOCAL Function Definitions                              *
 ******************************************************************************/

/******************************************************************************/
//  Description: Set basic clk of sdio
//  Author: andy.chen
//  Param
//
//  Return:
//      uint32 value :the frequency that be used acctually
//  Note: This function must be applied according different platform
/******************************************************************************/
LOCAL uint32 __BaseClkSet (uint32 base_clk)
{
    uint32 clk = 0;

    //Select the clk source of SDIO
    REG32 (GR_CLK_GEN5) &= ~ (BIT_17|BIT_18);

    if (base_clk >= SDIO_BASE_CLK_96M)
    {
        clk = SDIO_BASE_CLK_96M;
    }
    else if (base_clk >= SDIO_BASE_CLK_64M)
    {
        clk = SDIO_BASE_CLK_64M;
        REG32 (GR_CLK_GEN5) |= (1<<17);
    }
    else if (base_clk >= SDIO_BASE_CLK_48M)
    {
        clk = SDIO_BASE_CLK_48M;
        REG32 (GR_CLK_GEN5) |= (2<<17);
    }
    else
    {
        clk = SDIO_BASE_CLK_26M;
        REG32 (GR_CLK_GEN5) |= (3<<17);
    }

    return clk;
}

/*****************************************************************************/
//  Description: Get Normal int Status register
//  Author: andy.chen
//  Param
//      sdslave_handler: the handle of slave
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL uint32 __IntRawStatus_Get (SDSLAVE_PHY_HANDLE sdslave_handler)
{
    SCI_ASSERT (_HANDLE_VERIFY (sdslave_handler));/*assert verified*/
    return sdslave_handler->sdslave_cfg->INT_STA_RAW;
}

/*****************************************************************************/
//  Description: Clear Normal int Status register
//  Author: andy.chen
//  Param
//      sdslave_handler: the handle of slave
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void __IntStatus_Clr (SDSLAVE_PHY_HANDLE sdslave_handler, uint32 status)
{
    SCI_ASSERT (_HANDLE_VERIFY (sdslave_handler));/*assert verified*/
    sdslave_handler->sdslave_cfg->INT_STA_CLR = status;
}

/*****************************************************************************/
//  Description: Enable Normal int Signal register ,
//  Author: andy.chen
//  Param
//      sdhost_handler: the handle of host driver
//      msg:            that int you want to Enable
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL void __IntMask_EnDis (SDSLAVE_PHY_HANDLE sdslave_handler, uint32 mask, BOOLEAN is_en)
{
    volatile uint32 tmp;

    SCI_ASSERT (_HANDLE_VERIFY (sdslave_handler));/*assert verified*/

    tmp = sdslave_handler->sdslave_cfg->INT_STA_EN;

    if (is_en)
    {
        tmp |= mask;
    }
    else
    {
        tmp &= ~mask;
    }

    sdslave_handler->sdslave_cfg->INT_STA_EN = tmp;
}

/*****************************************************************************/
//  Description: transfer intstatus to msg to hal level
//  Author: andy.chen
//  Param
//      sdslave_handler: the handle of slave
//
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL uint32 __IntStsToMsg (uint32 sts)
{
    uint32 msg = 0;

    // normal status
    if (0 != (sts&INT_WR_TRANS_CMP))
    {
        msg |= SIG_WR_TRANS_CMP;
    }

    if (0 != (sts&INT_RD_TRANS_CMP))
    {
        msg |= SIG_RD_TRANS_CMP;
    }

    if (0 != (sts&INT_DMA_INT))
    {
        msg |= SIG_DMA_INT;
    }

    if (0 != (sts&INT_BUF_RD_RDY))
    {
        msg |= SIG_BUF_RD_RDY;
    }

    if (0 != (sts&INT_BUF_WR_RDY))
    {
        msg |= SIG_BUF_WR_RDY;
    }

    if (0 != (sts&INT_WRITE_START))
    {
        msg |= SIG_WRITE_START;
    }

    if (0 != (sts&INT_READ_START))
    {
        msg |= SIG_READ_START;
    }

    if (0 != (sts&INT_OCR_SET))
    {
        msg |= SIG_OCR_SET;
    }

    // err status
    if (0 != (sts&INT_ERR_CMDCRC))
    {
        msg |= SIG_ERR_CMDCRC;
    }

    if (0 != (sts&INT_ERR_CMDILL))
    {
        msg |= SIG_ERR_CMDILL;
    }

    if (0 != (sts&INT_ERR_DATACRC))
    {
        msg |= SIG_ERR_DATACRC;
    }

    if (0 != (sts&INT_ERR_AHB))
    {
        msg |= SIG_ERR_AHB;
    }

    return msg;
}

/*****************************************************************************/
//  Description: This function is called by interrupt service .is event happened ,this funtion will
//  clear the event and call the callback of application
//  Author: andy.chen
//  Param
//      isrnum: the number of arm interrupt
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
LOCAL ISR_EXE_T __IrqHandle (uint32 isrnum)
{
    uint32 sts;
    ISR_MsgQueue_T buffer;

    SDSLAVE_PRINT ("sdio slave int-sta:0x%x",s_sdslavePort.sdslave_cfg->INT_STA_RAW);
    sts = __IntRawStatus_Get (&s_sdslavePort);
    __IntStatus_Clr (&s_sdslavePort, sts);
    SDSLAVE_PRINT ("sdio slave int-sta after clear:0x%x",s_sdslavePort.sdslave_cfg->INT_STA_RAW);

    buffer.sdslave_handler = &s_sdslavePort;
    buffer.msg = __IntStsToMsg (sts);
    IsrWriteBuffer (buffer);

    return CALL_HISR;
}

/*****************************************************************************/
//  Description:    This function is SDIO 's HISR.
//                  1. THE priority is higher than normal task.
//                  2. Is not real ISR.
//  Author:         steve.zhan
//  Note:
/*****************************************************************************/
LOCAL void __HisrFunc (uint32 cnt, void *pData)
{
    ISR_MsgQueue_T buffer;

    while (!threadReadBuffer (&buffer))
    {
        SDSLAVE_PRINT ("sdio slave __HisrFunc buffer.msg:0x%x",buffer.msg);
        buffer.sdslave_handler->sigCallBack (buffer.msg);
    }
}

/*******************************************************************************
 **                    PUBLIC Function Definitions                             *
 ******************************************************************************/
PUBLIC void SDSLAVE_PHY_SetOCRReady (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    handle->sdslave_cfg->OCR_SUPPORT |= BIT_24;
}

PUBLIC uint32 SDSLAVE_PHY_GetOCRVaue (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    return handle->sdslave_cfg->OCR_CURRENT;
}

PUBLIC SDSLAVE_PHY_HANDLE SDSLAVE_PHY_Init (SDSLAVE_CALLBACK fun)
{
    uint32 status;

    // SDIO EN
    REG32 (AHB_CTL0) |= (AHB_CTL0_SDIO_EN);         //AHB ENABLE SDIO
    REG32 (AHB_SOFT_RST) |= (AHB_SDIO_SOFT_RST);    //AHB reset SDIO
    REG32 (AHB_SOFT_RST) &= ~ (AHB_SDIO_SOFT_RST);

    // slot select
    REG32 (AHB_SDIO_CTL) = AHB_SDIO_CTRL_SLAVE;     //select slot2 for slave

    s_sdslavePort.sdslave_cfg = (SDSLAVE_REGCFG_T *) ( (volatile uint32 *) (SDIO2_BASE_ADDR));
    s_sdslavePort.base_clk    = __BaseClkSet (SDIO_BASE_CLK_96M);
    s_sdslavePort.sigCallBack = fun;

    // isr register
    status = ISR_RegHandler_Ex (TB_SDIO_INT, (TB_ISR) __IrqHandle, __HisrFunc, CHIPDRV_HISR_PRIO_1, NULL);
    SCI_ASSERT (TB_SUCCESS == status);/*assert verified*/
    CHIPDRV_EnableIRQINT (TB_SDIO_INT);
    __IntMask_EnDis (&s_sdslavePort, INT_OCR_SET|INT_DMA_RW|INT_ERR, SCI_TRUE);

    // ocr set
    s_sdslavePort.sdslave_cfg->OCR_SUPPORT = (SDIO_OCR_25V|SDIO_OCR_28V|SDIO_OCR_30V);

    // DMA EN and dma no limit
    s_sdslavePort.sdslave_cfg->SLAVE_CTL0 = BIT_16 | (0x7<<17);
    SDSLAVE_PRINT ("sdio slave int-sta after init:0x%x",s_sdslavePort.sdslave_cfg->INT_STA_RAW);

    return &s_sdslavePort;
}

PUBLIC void SDSLAVE_PHY_Deinit (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/

    SDSLAVE_PHY_RST (handle, RST_ALL);
    handle->sdslave_cfg = PNULL;
    handle->sigCallBack = PNULL;
    handle->base_clk    = NULL;
    handle              = PNULL;
    REG32 (AHB_CTL0) &= ~ (AHB_CTL0_SDIO_EN);        //AHB ENABLE SDIO
}

PUBLIC void SDSLAVE_PHY_RST (SDSLAVE_PHY_HANDLE handle, SDSLAVE_RST_TYPE_E rst_type)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/

    switch (rst_type)
    {
        case RST_SDIO:
            handle->sdslave_cfg->SLAVE_CTL0 |= BIT_2;
            break;
        case RST_AHB:
            handle->sdslave_cfg->SLAVE_CTL0 |= BIT_1;
            break;
        case RST_ALL:
            handle->sdslave_cfg->SLAVE_CTL0 |= BIT_0;
            break;
        default:
            SDSLAVE_ASSERT (0); /*assert verified*/
            break;
    }
}

/*****************************************************************************/
//  Description:  Set operation voltage
//  Author: andy.chen
//  Param
//      sdhost_handler: the handle of host driver
//      voltage:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDSLAVE_PHY_CfgVoltage (SDSLAVE_PHY_HANDLE handle, uint32 vol)
{
    LDO_VOLT_LEVEL_E ldo_volt_level;

    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/

    if (vol&SDIO_OCR_30V)       // 2.9-3.1, set 3.0
    {
        ldo_volt_level = LDO_VOLT_LEVEL1;
        SCI_ASSERT (0 == (vol& (~SDIO_OCR_30V))); /*assert verified*/
    }
    else if (vol&SDIO_OCR_28V)  // 2.7-2.9, set 2.8
    {
        ldo_volt_level = LDO_VOLT_LEVEL0;
        SCI_ASSERT (0 == (vol& (~SDIO_OCR_28V))); /*assert verified*/
    }
    else if (vol&SDIO_OCR_25V)  // 2.4-2.6, set 2.5
    {
        ldo_volt_level = LDO_VOLT_LEVEL2;
        SCI_ASSERT (0 == (vol& (~SDIO_OCR_25V))); /*assert verified*/
    }
    else                        // other voltage is invalid
    {
        SCI_ASSERT (0); /*assert verified*/
    }

    LDO_SetVoltLevel (LDO_LDO_SDIO, ldo_volt_level);
}

/*****************************************************************************/
//  Description:  Open or close power supply of card(mmc \sd\sdio card.etc.)
//  Author: Jason.wu
//  Param
//      sdhost_handler: the handle of host driver
//      on_off:
//  Return:
//      NONE
//  Note:
/*****************************************************************************/
PUBLIC void SDSLAVE_PHY_PowrOnOff (SDSLAVE_PHY_HANDLE handle, BOOLEAN is_on)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/

    if (is_on)
    {
        LDO_TurnOnLDO (LDO_LDO_SDIO);
    }
    else
    {
        LDO_TurnOffLDO (LDO_LDO_SDIO);
    }
}

PUBLIC void SDSLAVE_PHY_SetDmaAddr (SDSLAVE_PHY_HANDLE handle, uint32 addr)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    handle->sdslave_cfg->DMA_SYS_ADD = addr;
}

PUBLIC uint32 SDSLAVE_PHY_GetBlkcnt (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    return ( (handle->sdslave_cfg->BLK_SIZE_COUNT) &BLK_CNT_MASK) >>16;
}

PUBLIC uint32 SDSLAVE_PHY_GetBlklen (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    return ( (handle->sdslave_cfg->BLK_SIZE_COUNT) &BLK_LEN_MASK);
}

PUBLIC uint32 SDSLAVE_PHY_GetTransMode (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    return ( ( (handle->sdslave_cfg->CMD_TRANSMODE) &BLK_MODE_MASK) ? (SDIO_BLK_MODE) : (SDIO_BYTE_MODE));
}

PUBLIC void SDSLAVE_PHY_SetCardInt (SDSLAVE_PHY_HANDLE handle)
{
    SCI_ASSERT (_HANDLE_VERIFY (handle));/*assert verified*/
    handle->sdslave_cfg->SLAVE_CTL0 |= BIT_3;
}

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif


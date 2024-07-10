/******************************************************************************
 ** File Name:      lzma_drv.c                                                 *
 ** Author:         changde                                              *
 ** DATE:           09/19/2011                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                  *
 **                                                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/19/2011     changde          Create.                                   *
 **                                                                           *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "lzma_reg_uws6121e.h"
#include "lzma_hal.h"
#include "../lzma_phy.h"

extern void _LzmaReset (void);
extern void _LzmaEnable (void);
extern void _LzmaDisable (void);

//lint -esym(628,_CHIPDRV_EnableIRQINT)
//lint -esym(718,_CHIPDRV_EnableIRQINT)
//lint -esym(746,_CHIPDRV_EnableIRQINT)
/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/
typedef struct LZMA_REG_DES_T
{
    volatile uint32 ctl_reg;
    volatile uint32 int_sts;
    volatile uint32 int_msk;
    volatile uint32 src_saddr;
    volatile uint32 dst_saddr;
    volatile uint32 src_len;
    volatile uint32 dst_len;

    volatile uint32 buffer_saddr;
    volatile uint32 buffer_len;
    volatile uint32 buffer_start_len;

    volatile uint32 proc_pos;
    volatile uint32 trans_len;
	volatile uint32 unzip_size;
}LZMA_REG_DES_T;


void lzma_event(uint32 e)
{
    mon_Event(e);
}

#define LZMA_ASSERT  SCI_ASSERT

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define LZMA_REG_OR       CHIP_REG_OR
#define LZMA_REG_AND      CHIP_REG_AND
#define LZMA_REG_GET      CHIP_REG_GET
#define LZMA_REG_SET      CHIP_REG_SET

/**---------------------------------------------------------------------------*
 **                         Variable Define                                   *
 **---------------------------------------------------------------------------*/
LOCAL LZMA_CallBack s_lzma_int_table[8] = {0};

/**---------------------------------------------------------------------------*
 **                         Local Define                                      *
 **---------------------------------------------------------------------------*/

LOCAL void LZMA_Pin_Config(void)
{

}

/*****************************************************************************
 *  Description:  _LZMA_Reset
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
LOCAL  void _LZMA_Reset(void)
{
    _LzmaReset();
 }

/*****************************************************************************
 *  Description:  _LZMA_Enable( is_en )
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
LOCAL void _LZMA_Enable( uint32 is_en)
{
    if(is_en)
    {
        _LzmaEnable();
	}
	else
	{
       _LzmaDisable();
        hwp_lzma->lzma_cmd_reg = 0;
        hwp_lzma->lzma_status_reg = 0;
	}
}

/*****************************************************************************
 *  Description:  _LZMA_BufferEn
 *  Author:                                                                  *
 *  Note:  If set to 1, Controller will decomp data to buffer and transfer data
 *         to DST_SADDR after decomp is finish                               *
******************************************************************************/
PUBLIC void _LZMA_BufferEn(uint32 is_en)
{
}

/*****************************************************************************
 *  Description:  _LZMA_BufferFull
 *  Author:                                                                  *
 *  Note:
 *  0: decompression will finish when decomp size meet  unzip size or
       BUFFER_START_LEN + DST_LENGTH.
    1: decompression will finish when decomp size meet unzip size or BUFFER_LENGTH
******************************************************************************/
PUBLIC void _LZMA_BufferFull(uint32 is_en)
{

}

PUBLIC void LZMA_ClrAllIntsts( void )
{
   hwp_lzma->lzma_status_reg &= ~(LZMA_AXI_ERR|LZMA_DEC_ERR|LZMA_DEC_DONE);
}

PUBLIC void LZMA_IntEn( uint32 chn_id , uint32 is_en)
{
	if(is_en)
	{
            hwp_lzma->lzma_irq_mask =hwp_lzma->lzma_irq_mask| LZMA_DEC_DONEIRQMASK;
	}
	else
	{
            hwp_lzma->lzma_irq_mask = hwp_lzma->lzma_irq_mask&(~LZMA_DEC_DONEIRQMASK);
	}
}

PUBLIC void LZMA_SetClk(void)
{

}

/*****************************************************************************
 *  Description:  LZMA_Init
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC void LZMA_Init(void)
{

    //enable LZMA mode
    _LZMA_Enable(SCI_TRUE);

    //reset ADI module
    _LZMA_Reset();

     LZMA_SetClk();

    //ISR_RegHandler(TB_ZIP0_INT, (TB_ISR)LZMA_IsrHandler);
    //CHIPDRV_EnableIRQINT(TB_MIDI_FM_ZIP_INT);
}

/*****************************************************************************
 *  Description:  LZMA_StartRun
 *  Author:                                                                  *
 *  Note: Set 1 to begin decompression, auto clear                            *
******************************************************************************/
PUBLIC void LZMA_StartRun(void )
{
    uint32 refbyte_en,cabac_movebits,cabac_totalbits;
    refbyte_en= 0;
    cabac_totalbits = 0;
    cabac_movebits =1;
    hwp_lzma->lzma_config_reg3 = (refbyte_en << 2) + (cabac_movebits << 1) + cabac_totalbits;
    hwp_lzma->lzma_cmd_reg = LZMA_START;
}

/*****************************************************************************
 *  Description:  LZMA_SetSrcCfg
 *  Author:                                                                  *
 *  Note  : Set the addres and length of compressed data source              *
******************************************************************************/
PUBLIC void LZMA_SetSrcCfg(uint32 src_start_addr, uint32 src_len)
{
	//LZMA_REG_SET(LZMA_SRC_SADDR, src_start_addr);
	//LZMA_REG_SET(LZMA_SRC_LEN,   src_len);
	//lzma_event(0x12171a01);
       //lzma_event(src_len);
       //SCI_ASSERT(src_len % 1024 == 0);

       hwp_lzma->lzma_dma_raddr_reg = src_start_addr;
      //LZMA_REG_STREAM_LEN(stream_len[BlockNum]);
       hwp_lzma->lzma_config_reg2 = LZMA_REG_STREAM_LEN(src_len);
}


#define _DICT_SIZE_8910_  1024
/*****************************************************************************
 *  Description:  LZMA_SetDstCfg
 *  Author:                                                                  *
 *  Note  : Set the addres and size of buffer used for  decompressed data   *
******************************************************************************/
PUBLIC void LZMA_SetDstCfg(uint32 dst_start_addr, uint32 dst_len)
{
	//LZMA_REG_SET(LZMA_DST_SADDR, dst_start_addr);
	//LZMA_REG_SET(LZMA_DST_LEN,   dst_len);
	//lzma_event(0x12171a02);
	//lzma_event(dst_len);
	SCI_ASSERT(dst_len % 1024 == 0);
	hwp_lzma->lzma_dma_waddr_reg = dst_start_addr;

       // LZMA_REG_DICT_SIZE(dict_size[BlockNum]) +LZMA_REG_BLOCK_SIZE(block_size[BlockNum]);
       hwp_lzma->lzma_config_reg1 = LZMA_REG_DICT_SIZE(_DICT_SIZE_8910_) +LZMA_REG_BLOCK_SIZE(dst_len);
}

/*****************************************************************************
 *  Description:  LZMA_SetDstCfg
 *  Author:                                                                  *
 *  Note  : If you use mid buffer for decompressing process,
            Plz set the addres and size of mid buffer                       *
******************************************************************************/
PUBLIC void LZMA_SetBufCfg(uint32 buf_start_addr, uint32 buf_len)
{
	//LZMA_REG_SET(LZMA_BUFFER_SADDR, buf_start_addr);
	//LZMA_REG_SET(LZMA_BUFFER_LEN,   buf_len);
}

PUBLIC void LZMA_SetBufStart(uint32 buf_start_len)
{
	//LZMA_REG_SET(LZMA_BUFFER_START_LEN, buf_start_len);
}

/*****************************************************************************
 *  Description:  LZMA_SetDstCfg
 *  Author:                                                                  *
 *  Note  : Polling the decompressed flag                                    *
 *          If timeout<0, it will wait forever                               *
******************************************************************************/
PUBLIC uint32 LZMA_WaitingFinish(uint32 timeout)
{
    uint32 pre_tick = 0;
    uint32 cur_tick = 0;

    if(timeout == 0)
    {
        while ((hwp_lzma->lzma_status_reg != 1) )
        {
           // Delay(100);
        }
    }
    else
    {
        pre_tick = timer_TimRealTickGet(); //timer_TimRealTickGet
        while ((hwp_lzma->lzma_status_reg != 1))
        {
            cur_tick = timer_TimRealTickGet();

            if (cur_tick < pre_tick)
            {
                continue;
            }

            if (cur_tick - pre_tick >= timeout)
            {
                //SCI_TraceLow:"LZMA_WaitingFinish: time out !!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_PHY_V5_259_112_2_17_23_5_17_1059,(uint8*)"");
                return SCI_FALSE;
            }
        }
    }

    return SCI_SUCCESS;
}

/*****************************************************************************
 *  Description:
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note: file size decoded from file header.                               *
******************************************************************************/
PUBLIC uint32 LZMA_GetUnZipRawStatus(void)
{

	return (hwp_lzma->lzma_status_reg == 0);
}

/*****************************************************************************
 *  Description:
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note: file size decoded from file header.                               *
******************************************************************************/
PUBLIC uint32 LZMA_ClrUnzipRawStatus(void)
{

     hwp_lzma->lzma_status_reg = 0;
     return 0;
}

/*****************************************************************************
 *  Description:
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note: file size decoded from file header.                               *
******************************************************************************/
PUBLIC uint32 LZMA_GetUnzipSize(void)
{

	return (LZMA_REG_BLOCK_SIZE( hwp_lzma->lzma_config_reg1));
}

/*****************************************************************************
 *  Description:
 *  Global Resource Dependence:                                              *
 *  Author:                                                                  *
 *  Note:   indecates how many bytes have been transferred              *
******************************************************************************/
PUBLIC uint32 LZMA_GetTransferredLen(void)
{
    SCI_ASSERT(SCI_FALSE);
    return 1;//(CHIP_REG_GET( LZMA_TRANS_LEN ));
}

/*****************************************************************************
 *  Description: LZMA_RegCallback
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC void LZMA_RegCallback( uint32 chan_id, LZMA_CallBack callback )
{
    SCI_ASSERT( chan_id <1);
    if(s_lzma_int_table[chan_id] == PNULL)
    {
        s_lzma_int_table[chan_id] = callback;
    }
    else
    {
        SCI_PASSERT(0, ("LZMA_RegCallback chan[%d]'s callback already exist.", chan_id)); /*assert to do*/
    }
}
PUBLIC void LZMA_UNRegCallback( uint32 chan_id, LZMA_CallBack callback )
{
    SCI_ASSERT( chan_id < 1);
    if(s_lzma_int_table[chan_id] != PNULL)
    {
        s_lzma_int_table[chan_id] = PNULL;
    }
    else
    {
        SCI_PASSERT(0, ("LZMA_UNRegCallback chan[%d]'s callback has been unregisterred.", chan_id)); /*assert to do*/
    }
}

/*****************************************************************************
 *  Description: LZMA_IsrHandler
 *  Author:                                                                  *
 *  Note:                                                                    *
******************************************************************************/
PUBLIC ISR_EXE_T LZMA_IsrHandler ( uint32 data )
{
    uint32 i=0;
    uint32 int_sts = hwp_lzma->lzma_status_reg;

    for( i=0; i<LZMA_INT_MAX_SIZE; i++)
    {
        // get its state of channel's interrupt
        if( int_sts)
        {
            if( s_lzma_int_table[i] != PNULL )
            {
                (*s_lzma_int_table[i])(PNULL);
            }
            else
            {
                SCI_PASSERT(0, ("LZMA_IsrHandler[%d]'s callback is PNULL.", i));/*assert verified*/
            }

            // clear its int flag to permit the next int occured.
            hwp_lzma->lzma_status_reg= int_sts;
        }
    }

    return ISR_DONE;
}

//#if defined(CHIP_DRV_SC6531) || defined(CHIP_DRV_SC6531EFM) || defined(CHIP_DRV_UIX8910)
#if 0
#error

#define LZMA_IDX_1ST    0   // only one for sc6531efm
#define LZMA_IDX_2ND    1
#define LZMA_IDX_MAX    2

LOCAL LZMA_CallBack s_lzma_int_table_ext[LZMA_IDX_MAX][8] = {0};

PUBLIC ISR_EXE_T LZMA_IsrHandlerExt (uint32 id, uint32 data )
{
    uint32 i = 0;
    uint32 int_sts = 0;

    if(LZMA_IDX_MAX <= id)
        return ISR_DONE;

    int_sts = LZMA_REG_GET((uint32)LZMA_INT + id * ((uint32)(0x100000)));//CHIP_REG_GET,LZMA_REG_GET

    for( i=0; i<LZMA_INT_MAX_SIZE; i++)
    {
        // get its state of channel's interrupt
        if( int_sts & ((1<<LZMA_INTSTS_BITPOS)<<i))
        {
            if( s_lzma_int_table_ext[id][i] != PNULL )
            {
                (*s_lzma_int_table_ext[id][i])(PNULL);
            }
            else
            {
                SCI_PASSERT(0, ("LZMA_IsrHandler[%d]'s callback is PNULL.", i));/*assert verified*/
            }

            // clear its int flag to permit the next int occured.
            LZMA_REG_OR((uint32)LZMA_INT + id *((uint32) 0x100000), ((1<<LZMA_INTCLR_BITPOS)<<i));
        }
    }

    return ISR_DONE;
}

PUBLIC ISR_EXE_T LZMA_IsrHandler1st (uint32 data )
{
    return LZMA_IsrHandlerExt(LZMA_IDX_1ST, data);
}

PUBLIC ISR_EXE_T LZMA_IsrHandler2nd (uint32 data )
{
    return LZMA_IsrHandlerExt(LZMA_IDX_2ND, data);
}

PUBLIC void LZMA_RegCallbackExt(uint32 id, uint32 chan_id, LZMA_CallBack callback )
{
    if(LZMA_IDX_MAX <= id)
        return;

    //SCI_ASSERT( chan_id < 8);
    if(s_lzma_int_table_ext[id][chan_id] == PNULL)
    {
        s_lzma_int_table_ext[id][chan_id] = callback;
    }
    else
    {
        SCI_PASSERT(0, ("LZMA_RegCallback chan[%d]'s callback already exist.", chan_id)); /*assert to do*/
    }
}

PUBLIC void LZMA_UNRegCallbackExt(uint32 id, uint32 chan_id, LZMA_CallBack callback )
{
    if(LZMA_IDX_MAX <= id)
        return;

    //SCI_ASSERT( chan_id < 8);
    if(s_lzma_int_table_ext[id][chan_id] != PNULL)
    {
        s_lzma_int_table_ext[id][chan_id] = PNULL;
    }
    else
    {
        SCI_PASSERT(0, ("LZMA_UNRegCallback chan[%d]'s callback has been unregisterred.", chan_id)); /*assert to do*/
    }
}

PUBLIC void LZMA_ResetExtAll(uint32 cycles)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    LZMA_REG_SET(AHB_RST0_SET, ZIP0_SOFT_RST_SET);
    while( cycles-- ){}
    LZMA_REG_SET(AHB_RST0_CLR, ZIP0_SOFT_RST_CLR);
#else
    LZMA_REG_SET(AHB_RST0_SET, ZIP0_SOFT_RST_SET | ZIP1_SOFT_RST_SET);
    while( cycles-- ){}
    LZMA_REG_SET(AHB_RST0_CLR, ZIP0_SOFT_RST_CLR | ZIP1_SOFT_RST_CLR);
#endif
}

PUBLIC void LZMA_ResetExt(uint32 id, uint32 cycles)
{
    uint32 val = 0;

    if(LZMA_IDX_MAX <= id)
        return;

    if(LZMA_IDX_1ST == id)
        val = ZIP0_SOFT_RST_SET;
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    else if(LZMA_IDX_2ND == id)
        val = ZIP1_SOFT_RST_SET;
#endif

    LZMA_REG_SET(AHB_RST0_SET, val);
    while( cycles-- ){}
    LZMA_REG_SET(AHB_RST0_CLR, val);
}

PUBLIC void LZMA_EnableExtAll(uint32 is_en)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if(is_en)
    {
        LZMA_REG_SET(AHB_EB0_SET, ZIP0_EB_SET);
	}
	else
	{
        LZMA_REG_SET(AHB_EB0_CLR, ZIP0_EB_CLR);
	}
#else
    if(is_en)
    {
        LZMA_REG_SET(AHB_EB0_SET, ZIP0_EB_SET | ZIP1_EB_SET);
	}
	else
	{
        LZMA_REG_SET(AHB_EB0_CLR, ZIP0_EB_CLR | ZIP1_EB_CLR);
	}
#endif
}

PUBLIC void LZMA_EnableExt(uint32 id, uint32 is_en)
{
    uint32 val = 0;

    if(LZMA_IDX_MAX <= id)
        return;

    if(LZMA_IDX_1ST == id)
        val = ZIP0_EB_SET;
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    else if(LZMA_IDX_2ND == id)
        val = ZIP1_EB_SET;
#endif

    if(is_en)
    {
        LZMA_REG_SET(AHB_EB0_SET, val);
	}
	else
	{
        LZMA_REG_SET(AHB_EB0_CLR, val);
	}
}

PUBLIC void LZMA_BufferEnExt(uint32 id, uint32 is_en)
{
    uint32 value = 0;

    if(LZMA_IDX_MAX <= id)
        return;

    value = LZMA_REG_GET((uint32)LZMA_CTRL_STS + id * ((uint32)0x100000));

    if(is_en)
    {
        value |= LZMA_CTRL_BUFFER_EN_BIT;
    }
    else
    {
        value &= ~LZMA_CTRL_BUFFER_EN_BIT;
    }
    LZMA_REG_SET((uint32)LZMA_CTRL + id * ((uint32)0x100000), value);
}

PUBLIC void LZMA_BufferFullExt(uint32 id, uint32 is_en)
{
    uint32 value = LZMA_REG_GET((uint32)LZMA_CTRL_STS + id * ((uint32)0x100000));

    if(LZMA_IDX_MAX <= id)
        return;

    if(is_en)
    {
        value |= LZMA_CTRL_BUFFER_FULL_BIT;
	}
	else
	{
        value &= ~LZMA_CTRL_BUFFER_FULL_BIT;
	}
    LZMA_REG_SET((uint32)LZMA_CTRL + id * ((uint32)0x100000), value);
}

PUBLIC uint32 LZMA_GetUnzipStatusExt(uint32 id)
{
    if(LZMA_IDX_MAX <= id)
        return 0x1E000000;

	return ((CHIP_REG_GET(((uint32)LZMA_INT + id * ((uint32)0x100000)))) & 0x1E000000);
}

PUBLIC uint32 LZMA_ClrUnzipStatusExt(uint32 id)
{
    if(LZMA_IDX_MAX <= id)
        return 0x1E000000;

	return CHIP_REG_OR(((uint32)LZMA_INT + id * ((uint32)0x100000)), 0x1E00);
}

PUBLIC void LZMA_ClrAllIntstsExt(uint32 id)
{
    if(LZMA_IDX_MAX <= id)
        return;

    LZMA_REG_OR((uint32)LZMA_INT + id * ((uint32)0x100000), (LZMA_INTCLR_BUF_LEN_ERR_BIT|
		                   LZMA_INTCLR_DEC_ERR_BIT    |
		                   LZMA_INTCLR_DST_LEN_ERR_BIT|
		                   LZMA_INTCLR_FINISH_BIT     |
		                   LZMA_INTCLR_MST_ERR_BIT)   );
}

PUBLIC void LZMA_IntEnExt(uint32 id, uint32 chn_id , uint32 is_en)
{
    if(LZMA_IDX_MAX <= id)
        return;

    chn_id &= 0xFF;

	if(is_en)
	{
        LZMA_REG_OR((uint32)LZMA_INT + id * ((uint32)0x100000), (1<<chn_id));
	}
	else
	{
        LZMA_REG_AND((uint32)LZMA_INT + id * ((uint32)0x100000), ~(uint32)(1<<chn_id));
	}
}

PUBLIC void LZMA_SetClkExt(uint32 id)
{
    if(LZMA_IDX_1ST == id)
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        LZMA_REG_OR(CGM_ZIP0_CFG, (3<<CGM_ZIP0_SEL_SHIFT));
#else
        LZMA_REG_OR(APB_CLK_CFG0, (1<<CLK_ZIP0_SEL_SHIFT));
#endif
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    else
        LZMA_REG_OR(APB_CLK_CFG0, (1<<CLK_ZIP1_SEL_SHIFT));
#endif
}

PUBLIC void LZMA_InitExt(uint32 id)
{
	if(LZMA_IDX_MAX <= id)
        return;

    //enable LZMA mode
    LZMA_EnableExt(id, SCI_TRUE);

    //reset ADI module
    LZMA_ResetExtAll(100);

    LZMA_SetClkExt(id);

    if(LZMA_IDX_1ST == id)
    {
    	ISR_RegHandler(TB_MIDI_FM_ZIP_INT, (TB_ISR)LZMA_IsrHandler1st);
    	CHIPDRV_EnableIRQINT(TB_MIDI_FM_ZIP_INT);
    }
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    else
    {
        ISR_RegHandler(TB_ZIP1_INT, (TB_ISR)LZMA_IsrHandler2nd);
        CHIPDRV_EnableIRQINT(TB_ZIP1_INT);
    }
#endif
 }

PUBLIC void LZMA_SetClkExtAll(void)
{
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
        LZMA_REG_OR(CGM_ZIP0_CFG, (3<<CGM_ZIP0_SEL_SHIFT));
#else
    LZMA_REG_OR(APB_CLK_CFG0, ((1<<CLK_ZIP0_SEL_SHIFT) | (1 << CLK_ZIP1_SEL_SHIFT)));
#endif
}

PUBLIC void LZMA_InitExtAll(void)
{

    //enable LZMA mode
    LZMA_EnableExtAll(SCI_TRUE);

    //reset ADI module
    LZMA_ResetExtAll(100);

    LZMA_SetClkExtAll();

	ISR_RegHandler(TB_MIDI_FM_ZIP_INT, (TB_ISR)LZMA_IsrHandler1st);
	CHIPDRV_EnableIRQINT(TB_MIDI_FM_ZIP_INT);
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    ISR_RegHandler(TB_ZIP1_INT, (TB_ISR)LZMA_IsrHandler2nd);
    CHIPDRV_EnableIRQINT(TB_ZIP1_INT);
#endif
}

PUBLIC void LZMA_StartRunExt(uint32 id)
{
    uint32 val = LZMA_REG_GET((uint32)LZMA_CTRL_STS + id * ((uint32)0x100000));
    val |= LZMA_CTRL_START_BIT;
    LZMA_REG_SET((uint32)LZMA_CTRL + id * ((uint32)0x100000), val);
}

PUBLIC void LZMA_SetSrcCfgExt(uint32 id, uint32 src_start_addr, uint32 src_len)
{
    if(LZMA_IDX_MAX <= id)
        return;
	LZMA_REG_SET((uint32)LZMA_SRC_SADDR + id * ((uint32)0x100000), src_start_addr);
	LZMA_REG_SET((uint32)LZMA_SRC_LEN + id * ((uint32)0x100000),   src_len);
}

PUBLIC void LZMA_SetDstCfgExt(uint32 id, uint32 dst_start_addr, uint32 dst_len)
{
    if(LZMA_IDX_MAX <= id)
        return;
	LZMA_REG_SET((uint32)LZMA_DST_SADDR + id * ((uint32)0x100000), dst_start_addr);
	LZMA_REG_SET((uint32)LZMA_DST_LEN + id * ((uint32)0x100000),   dst_len);
}

PUBLIC void LZMA_SetBufCfgExt(uint32 id, uint32 buf_start_addr, uint32 buf_len)
{
    if(LZMA_IDX_MAX <= id)
        return;
	LZMA_REG_SET((uint32)LZMA_BUFFER_SADDR + id * ((uint32)0x100000), buf_start_addr);
	LZMA_REG_SET((uint32)LZMA_BUFFER_LEN + id * ((uint32)0x100000),   buf_len);
}

PUBLIC void LZMA_SetBufStartExt(uint32 id, uint32 buf_start_len)
{
    if(LZMA_IDX_MAX <= id)
        return ;
	LZMA_REG_SET((uint32)LZMA_BUFFER_START_LEN + id * ((uint32)0x100000), buf_start_len);
}

PUBLIC uint32 LZMA_WaitingFinishExt(uint32 id, uint32 timeout)
{
    uint32 pre_tick = 0;
    uint32 cur_tick = 0;

    if(LZMA_IDX_MAX <= id)
        return SCI_FALSE;

    if(timeout == 0)
    {
        while ( !(LZMA_REG_GET((uint32)LZMA_INT + id * ((uint32)0x100000)) & LZMA_INTRAW_FINISH_BIT) )
        {
           // Delay(100);
        }
    }
    else
    {
        pre_tick = SCI_GetTickCount();
        while ( !(LZMA_REG_GET((uint32)LZMA_INT + id * ((uint32)0x100000)) & LZMA_INTRAW_FINISH_BIT) )
        {
            cur_tick = SCI_GetTickCount();

            if (cur_tick < pre_tick)
            {
                continue;
            }

            if (cur_tick - pre_tick >= timeout)
            {
                //SCI_TraceLow:"LZMA_WaitingFinish: time out !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_PHY_V5_259_112_2_17_23_5_17_1059,(uint8*)"");
                return SCI_FALSE;
            }
        }
    }

    return SCI_SUCCESS;
}

PUBLIC uint32 LZMA_GetUnzipSizeExt(uint32 id)
{
    if(LZMA_IDX_MAX <= id)
        return 0;
	return (CHIP_REG_GET( (uint32)LZMA_UNZIP_SIZE + id * ((uint32)0x100000)));
}

PUBLIC uint32 LZMA_GetTransferredLenExt(uint32 id)
{
    if(LZMA_IDX_MAX <= id)
        return 0;
	return (CHIP_REG_GET( (uint32)LZMA_TRANS_LEN + id * ((uint32)0x100000)));
}

#endif




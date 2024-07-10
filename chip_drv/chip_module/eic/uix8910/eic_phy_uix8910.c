/******************************************************************************
 ** File Name:                eic.c                                           *
 ** Author:         lichd                                                   *
 ** DATE:           06/21/2011                                              *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                             *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                               *
 ** ------------------------------------------------------------------------*
 ** DATE               NAME             DESCRIPTION                            *
 **                                                                            *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "tb_comm.h"
#include "arm_reg.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#ifdef CHIP_VER_6530
#include "sc6530_reg_base.h"
#include "sc6530_reg_global.h"
#endif
#include "../eic_phy.h"
#include "../eic_cfg.h"
#ifdef CHIP_VER_6530
#include "sc6530_eic_cfg.h"
#endif
#include "eic_reg_uix8910.h"
/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
 **                        Macro Define                                        *
 **---------------------------------------------------------------------------*/
#define _EIC_DEBUG
#ifdef _EIC_DEBUG
#include "sci_log.h"
#define EICDRV_ASSERT                SCI_ASSERT
#define EICDRV_TraceLow              SCI_TRACE_LOW


#else
#define EICDRV_ASSERT(_x_)           ((void)0)
#define EICDRV_TraceLow(_x_)            ((void)0)


#endif

/**----------------------------------------------------------------------------*
 **                        Protocol Define                                     *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                        Data Define                                         *
 **---------------------------------------------------------------------------*/
LOCAL DEVICE_HANDLE s_dev_eic_ddie = SCI_NULL; 
LOCAL DEVICE_HANDLE s_dev_eic_adie = SCI_NULL; 

/* Define the int_table for each group */
LOCAL EIC_CALLBACK s_eic_dbnc_int_table[ EIC_DBNC_SIZE ] = {0};

/**----------------------------------------------------------------------------*
 **                        Local Func Declare                                  *
 **---------------------------------------------------------------------------*/

/*  Description:    Get the group config information of a base_addr
 *  Author:         mei
 *  Note:
 */
LOCAL EIC_GROUP_DES_T* _eic_phy_get_grpcfg( uint32 base_addr ) 
{
    uint32 table_size =0;
    EIC_GROUP_DES_T* eic_group_cfg_ptr = EIC_GetGroupCfg( &table_size );
    
    //uniform addressing for id index
    while(  table_size-- )
    {
        if(eic_group_cfg_ptr->base_addr == base_addr)  
            return eic_group_cfg_ptr;
            
        eic_group_cfg_ptr++;
    }
    return SCI_NULL;
}
/**----------------------------------------------------------------------------*
 **                        Public Func Define                                  *
 **---------------------------------------------------------------------------*/

/*  Description:    Open the eic module
 *  Author:         mei
 *  Note:
 */
PUBLIC void EIC_PHY_Reset( void )
{
    /* Reset eic model in D-die */
    DEVICE_Reset(s_dev_eic_ddie);

    /* reset the eic in a-die */
    DEVICE_Reset(s_dev_eic_adie);   
}

PUBLIC void EIC_PHY_Open( void )
{
    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_Open" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_95_112_2_17_23_4_20_949,
        (uint8*)"");

    /* Open D-die eic module */
    SCI_ASSERT (DEVICE_Find ("EIC_D", &s_dev_eic_ddie) == CLOCK_SUCCESS);    
    DEVICE_Open(s_dev_eic_ddie);
    SCI_ASSERT (DEVICE_Find ("EIC_A", &s_dev_eic_adie) == CLOCK_SUCCESS);
    DEVICE_Open(s_dev_eic_adie);

    ISR_RegHandler (TB_ANA_EIC_INT, (TB_ISR)EICD_DbncIsr); 
    CHIPDRV_EnableIRQINT(TB_ANA_EIC_INT);

    
    /* Reset eic model */
    EIC_PHY_Reset();
}


/*  Description:    Close the eic module
 *  Author:         mei
 *  Note:
 */
PUBLIC void EIC_PHY_Close( void )
{
    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_Close" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_133_112_2_17_23_4_20_950,
        (uint8*)"");
    
    DEVICE_Close(s_dev_eic_ddie);
    DEVICE_Close(s_dev_eic_adie);
}


/*  Description:    Enable/Disable the interrupt for a eic's channel
 *                  is_en: TRUE--enable FALSE--disable
 *  Author:         mei
 *  Note:
 */
PUBLIC void EIC_PHY_IntEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id, 
                           BOOLEAN is_en)
{
    uint32 addr = 0;
    uint32 grp_type = eic_group_cfg_ptr->type;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_IntEn[%d] is %d" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_149_112_2_17_23_4_20_951,
        (uint8*)"dd",chan_id, is_en);

    if(grp_type == EIC_DBNC)
        addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_INT_EN;
    else
        /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_SetTrg not support grptype=%d.",grp_type));

    if(is_en)    
    {
        EIC_REG_OR(addr, (uint32)(1<<chan_id)); 
    }
    else
    {
        EIC_REG_AND(addr, ~(uint32)(1<<chan_id));
    }
}


/*  Description:    Enable/Disable the de-bounce for a eic's channel
 *                  is_en: TRUE--enable FALSE--disable
 *  Author:         mei
 *  Note:
 */
PUBLIC void EIC_PHY_DbncEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id,
                            BOOLEAN is_en)
{
    uint32 addr = eic_group_cfg_ptr->base_addr + EIC_CHANNEL_CTRL(chan_id);
    uint32 grp_type = eic_group_cfg_ptr->type;

    //EICDRV_TraceLow:"EICDRV: EIC_PHY_DbncEn[%d] is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_182_112_2_17_23_4_20_952,(uint8*)"dd",chan_id, is_en);

    if( grp_type == EIC_DBNC )
    {
        if( is_en )
        {
            EIC_REG_OR(addr, EICCTRL_DBNC_EN); 
        }
        else
        {
            EIC_REG_AND(addr, ~EICCTRL_DBNC_EN); 
        }
    }
    else
    {
        SCI_PASSERT(0, ("EIC_LATCH not support _TrgEn."));/*assert verified*/
    }  
}


/*  Description:    Register EIC_DBNC_DMSK must be set to 1 before reading            
 *                  corresponding channel of register EIC_DBNC_DATA
 *  Author:         mei
 *  Note:           Only eic_dbnc support this feature
 */
PUBLIC void EIC_PHY_MskData( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id)
{
    uint32 addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_DMSK;
    uint32 grp_type = eic_group_cfg_ptr->type;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_MskData[%d] " */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_209_112_2_17_23_4_20_953,
        (uint8*)"d",chan_id);
    
    if( grp_type == EIC_DBNC )
    {
        EIC_REG_OR(addr, 1<<chan_id); 
    }
    else
    {   /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_GetData not support _TrgEn."));
    }
}

/*  Description:    read corresponding channel of register EIC_DBNC_DATA
 *  Author:         mei
 *  Note:           Only eic_dbnc support this feature
 */
PUBLIC uint32 EIC_PHY_GetData( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id)
{
    uint32 addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_DATA;
    uint32 grp_type = eic_group_cfg_ptr->type;
    uint32 result=0;

    if( grp_type == EIC_DBNC )
    {
        result = (EIC_REG_GET(addr)) & (1<<chan_id); 
    }
    else
    {
        /* assert verified */
        SCI_PASSERT(0, ("EIC_PHY_GetData not support _TrgEn."));
    }

    result = (result>0) ? 1:0;
    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_GetData[%d] == %d" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_237_112_2_17_23_4_20_954,(uint8*)"dd",chan_id, result);
    
    return (result);
}

/*  Description:    set corresponding channel of EIC interrupt trig level
 *  Author:         mei
 *  Note:           1: high level, 0:low level
 */
PUBLIC void EIC_PHY_SetTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id, 
                            EIC_TRG_TYPE_E trg_type_e )
{
    uint32 addr     = 0;
    uint32 grp_type = eic_group_cfg_ptr->type;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_SetTrg[%d] is %d" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_251_112_2_17_23_4_20_955,
        (uint8*)"dd",chan_id,trg_type_e);

    if(grp_type == EIC_DBNC)
    {
        addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_INT_EV;
        
        if(HIGH_LEVEL == trg_type_e)
        {
            EIC_REG_OR(addr,  (uint32)(1<<chan_id)); 
        }
        else
        {
            EIC_REG_AND(addr, ~(uint32)(1<<chan_id)); 
        }         
    }
    else
        /*assert verified*/  
        SCI_PASSERT(0, ("EIC_PHY_SetTrg not support grptype."));
}

/*  Description:    set the bit of Register EIC_DBNC_TRG_CTRL to generate start
 *                  trig.
 *  Author:         mei
 *  Note:           Before the first int will be occured when the trig condition
 *                  met, you must set StartTrg bit of a channel.
 */
PUBLIC void EIC_PHY_StartTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr,
    uint32 chan_id)
{
    uint32 grp_type = eic_group_cfg_ptr->type;
    uint32 addr     = eic_group_cfg_ptr->base_addr + EIC_DBNC_TRG_CTRL;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_StartTrg[%d]" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_411_112_2_17_23_4_21_956,
        (uint8*)"d",chan_id);

    if(grp_type == EIC_DBNC)
    {
        EIC_REG_OR(addr, (1<<chan_id));
    }
    else
    {   /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_StartTrg not sup EIC_LATCH."));
    }
}

/*  Description:    set debounce count(ms) of corresponding channel of EIC
 *  Author:         mei
 *  Note:           Only eic_dbnc support this feature
 */
PUBLIC void EIC_PHY_SetDbnc( EIC_GROUP_DES_T* eic_group_cfg_ptr,
    uint32 chan_id, uint32 cnt)
{
    uint32 addr     = eic_group_cfg_ptr->base_addr + EIC_CHANNEL_CTRL(chan_id);
    uint32 grp_type = eic_group_cfg_ptr->type;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_SetDbnc[%d] %d" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_432_112_2_17_23_4_21_957,
        (uint8*)"dd",chan_id,cnt);

    if( grp_type == EIC_DBNC )
    {
        EIC_REG_AND(addr, ~EICCTRL_DBNC_CNT_BITMSK);
        EIC_REG_OR (addr, cnt);    
    }
    else
    {   /*assert verified*/
        SCI_PASSERT(0, ("EIC_LATCH not support seting Debounce."));
    }
}


/******************************************************************************/
//  Description:    Set whether interrupt active with chip sleep
//                  SCI_TRUE --interrupt active with chip sleep    reg.bit=0 
//                  SCI_FALSE--interrupt active without chip sleep reg.bit=1
//  Author:         lichd
//  Note:           Only eic_latch support this feature          
/******************************************************************************/
PUBLIC void EIC_PHY_TrgSleepEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id, BOOLEAN is_en)
{
#if !defined(PLATFORM_UWS6121E)
    uint32 addr     = eic_group_cfg_ptr->base_addr + EIC_LATCH_INT_MODE;
    uint32 grp_type = eic_group_cfg_ptr->type;

    if( grp_type == EIC_LATCH )
    {
        if(is_en)
        {
            EIC_REG_AND(addr, ~(uint32)(1<<chan_id));
        }
        else
        {
            EIC_REG_OR(addr, (uint32)(1<<chan_id));
        }
    }
    else
    {
        SCI_PASSERT(0, ("EIC_DBNC not support seting _TrgSleepEn."));/*assert verified*/
    }    
#endif
}

/*  Description:    get raw inerrupt status of corresponding channel of EIC
 *  Author:         mei
 *  Note:           SC2720 only suport DBNC
 */
PUBLIC uint32 EIC_PHY_GetIntSts( EIC_GROUP_DES_T* eic_group_cfg_ptr,
    uint32 chan_id)
{
    uint32 addr     = 0;
    uint32 grp_type = eic_group_cfg_ptr->type;

    if(grp_type == EIC_DBNC)
        addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_INT_RAWSTS;
    else
        /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_GetIntSts not support grptype=%d.",grp_type));    
  
    return (EIC_REG_GET(addr) & (1<<chan_id));  
}

/*  Description:    get masked inerrupt status of corresponding channel of EIC
 *  Author:         mei
 *  Note:           SC2720 only suport DBNC
 */
PUBLIC uint32 EIC_PHY_GetMskIntSts( EIC_GROUP_DES_T* eic_group_cfg_ptr,
    uint32 chan_id)
{
    uint32 addr     = 0;
    uint32 grp_type = eic_group_cfg_ptr->type;

    if(grp_type == EIC_DBNC)
        addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_INT_MSKSTS;
    else
        /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_GetMskIntSts not support grptype=%d.",grp_type));

    return (EIC_REG_GET(addr) & (1<<chan_id));  
}

/******************************************************************************/
//  Description:    Reverse the int polarity of a eic's channel
//  Author:         lichd
//  Note:           Only eic_latch support this feature          
/******************************************************************************/
PUBLIC void EIC_PHY_ReverseTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id )
{
#if !defined(PLATFORM_UWS6121E)
    uint32 grp_type = eic_group_cfg_ptr->type;
    uint32 addr     = eic_group_cfg_ptr->base_addr + EIC_LATCH_INT_POL;

    if(grp_type == EIC_LATCH)
    {
        uint32 value = (EIC_REG_GET(addr)&(1<<chan_id)) ^ (1<<chan_id);
        EIC_REG_SET(addr, value);
    }
    else
    {
        SCI_PASSERT(0, ("EIC_PHY_ReverseTrg not sup EIC_DBNC."));/*assert verified*/
    }
#endif
}

/*  Description:    clear detected inerrupt of corresponding channel of EIC
 *  Author:         mei
 *  Note:           SC2720 only suport DBNC
 */
PUBLIC void EIC_PHY_ClrIntSts( EIC_GROUP_DES_T* eic_group_cfg_ptr,
    uint32 chan_id)
{
    uint32 addr     = 0;
    uint32 grp_type = eic_group_cfg_ptr->type;

    /* EICDRV_TraceLow:"EICDRV: EIC_PHY_ClrIntSts[%d] " */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_542_112_2_17_23_4_21_958,
        (uint8*)"d",chan_id);

    if(grp_type == EIC_DBNC)
        addr = eic_group_cfg_ptr->base_addr + EIC_DBNC_INT_CLR;
    else
        /*assert verified*/
        SCI_PASSERT(0, ("EIC_PHY_SetTrg not support grptype=%d.",grp_type));
    
    EIC_REG_OR( addr, (1<<chan_id) );  
}

/*  Description:    the isr of EIC_DBNC module
 *  Author:         mei
 *  Note:           SC2720 only suport DBNC
 */
PUBLIC ISR_EXE_T EICD_DbncIsr( uint32 data )
{
    uint32          i=0;
    uint32          state=0;    
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_phy_get_grpcfg(EIC_DBNC_REG_BASE); 

    SCI_TRACE_LOW("EICD_DbncIsr");

    for( i=0; i<EIC_DBNC_SIZE; i++)
    {
        /* get its state of channel's interrupt */
        if( EIC_PHY_GetMskIntSts(eic_group_cfg_ptr, i) )
        {
            /* disable interrupt to avoid interrupt continus */
            EIC_PHY_IntEn( eic_group_cfg_ptr, i, SCI_FALSE );
        
            if( s_eic_dbnc_int_table[i] != PNULL )
            {
                EIC_PHY_MskData(eic_group_cfg_ptr, i);
                state =  EIC_PHY_GetData(eic_group_cfg_ptr, i);
                (*s_eic_dbnc_int_table[i])(i, state);
            }
            else
            {   /*assert verified*/
                SCI_PASSERT(0, ("EICD_DbncIsr chan[%d]'s callback is PNULL.",i));
            }
            
            /* clear its int flag to permit the next int occured. */
            EIC_PHY_ClrIntSts(eic_group_cfg_ptr, i);

            /* EICDRV_TraceLow:"EICD_DbncIsr chan[%d]'s callback is DONE." */
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_589_112_2_17_23_4_21_959,
                (uint8*)"d", i);

            
            /* ready for next event */
            EIC_PHY_IntEn(eic_group_cfg_ptr, i, SCI_TRUE);
            EIC_PHY_StartTrg(eic_group_cfg_ptr, i);
        }
    }    

    return ISR_DONE;
}

/******************************************************************************/
//  Description:    the isr of EICA_DBNC module
//  Author:         lichd
//  Note:     
//                  For SC8810, it's a channel of analog(INT24).
/******************************************************************************/
PUBLIC ISR_EXE_T EICA_DbncIsr( uint32 data )
{
    uint32          i=0;
    uint32          state=0;
#if !defined(PLATFORM_UWS6121E) 
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_phy_get_grpcfg( EICA_DBNC_REG_BASE );      
    for( i=0; i<EICA_DBNC_SIZE; i++)
    {
        // get its state of channel's interrupt
        if( EIC_PHY_GetMskIntSts(eic_group_cfg_ptr, i) )
        {
            // disable int to avoid interrupt continus
            EIC_PHY_IntEn( eic_group_cfg_ptr, i, SCI_FALSE );
        
            if( s_eica_dbnc_int_table[i] != PNULL )
            {
                EIC_PHY_MskData(eic_group_cfg_ptr, i);
                state =  EIC_PHY_GetData(eic_group_cfg_ptr, i);            
                (*s_eica_dbnc_int_table[i])( i, state);
            }
            else
            {    
                SCI_PASSERT(0, ("EICA_DbncIsr chan[%d]'s callback is PNULL.", i));/*assert verified*/
            }
            // clear its int flag to permit the next int occured.
            EIC_PHY_ClrIntSts(eic_group_cfg_ptr, i);

            //EICDRV_TraceLow:"EICA_DbncIsr chan[%d]'s callback is DONE."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_630_112_2_17_23_4_21_960,(uint8*)"d", i);

            // Ready for next trig signal
            EIC_PHY_IntEn(eic_group_cfg_ptr, i, SCI_TRUE);            
            EIC_PHY_StartTrg(eic_group_cfg_ptr, i);
        }
    } 
#endif
    return ISR_DONE;
}

/******************************************************************************/
//  Description:    the isr of EICA_LATCH module
//  Author:         lichd
//  Note:     
//                  For SC8810, it's the channel of SpecialLatch(INT00).
/******************************************************************************/
PUBLIC ISR_EXE_T EIC_LatchIsr( uint32 data )
{
    uint32          i=0;
    uint32          state=0;
#if !defined(PLATFORM_UWS6121E)  
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_phy_get_grpcfg( EIC_LATCH_REG_BASE ); 
    for( i=0; i<EIC_LATCH_SIZE; i++)
    {
        // get its state of channel's interrupt
        if( EIC_PHY_GetMskIntSts(eic_group_cfg_ptr, i) )
        {
            // disable int to avoid interrupt continus
            EIC_PHY_IntEn( eic_group_cfg_ptr, i, SCI_FALSE );
            
            if( s_eic_latch_int_table[i] != PNULL )
            {
                //             
                (*s_eic_latch_int_table[i])( i, state);
            }
            else
            {    
                SCI_PASSERT(0, ("EIC_LATCH Isr chan[%d]'s callback is PNULL.", i));/*assert verified*/
            }
            
            // clear its int flag to permit the next int occured.
            EIC_PHY_ClrIntSts(eic_group_cfg_ptr, i);
            
            // Ready for next trig signal 
            EIC_PHY_IntEn(eic_group_cfg_ptr, i, SCI_TRUE);               
            //EIC_PHY_ReverseTrg(eic_group_cfg_ptr, i);          
        }
    } 
#endif
    return ISR_DONE;
}

/******************************************************************************/
//  Description:    the isr of EIC_ASYNC EIC_SYNC module
//  Author:         lichd
//  Note:     
//                  For SC8810, it's a channel of analog(INT07).
/******************************************************************************/
PUBLIC ISR_EXE_T EIC_A_SYNCIsr( uint32 data )
{
    uint32 i=0;
    uint32 chan_id   = 0;
    uint32 state     = 0;   
    EIC_GROUP_DES_T* eic_group_cfg_ptr = {0}; 
#if !defined(PLATFORM_UWS6121E)
    eic_group_cfg_ptr = _eic_phy_get_grpcfg( EIC_ASYNC_REG_BASE );    
    for(chan_id=0; chan_id<8; chan_id++)
    {
        if( EIC_PHY_GetMskIntSts(eic_group_cfg_ptr, chan_id) )
        {
            // disable int to avoid interrupt continus
            EIC_PHY_IntEn( eic_group_cfg_ptr, chan_id, SCI_FALSE );
            
            if( s_eic_async_int_table[chan_id] != PNULL )
            {
                EIC_PHY_MskData(eic_group_cfg_ptr, i);
                state =  EIC_PHY_GetData(eic_group_cfg_ptr, chan_id);              
                (*s_eic_async_int_table[chan_id])( chan_id, state);
            }
            else
            {    
                SCI_PASSERT(0, ("EIC_A_SYNCIsr async chan[%d]'s callback is PNULL.", chan_id));/*assert verified*/
            }

            //clear its int flag
            EIC_PHY_ClrIntSts(eic_group_cfg_ptr, chan_id);

            // Ready for next trig signal 
            EIC_PHY_IntEn(eic_group_cfg_ptr, chan_id, SCI_TRUE);            
        }
    }

    eic_group_cfg_ptr = _eic_phy_get_grpcfg( EIC_SYNC_REG_BASE );   
    for(chan_id=0; chan_id<8; chan_id++)
    {
        if( EIC_PHY_GetMskIntSts(eic_group_cfg_ptr, chan_id) )
        {
            // disable int to avoid interrupt continus
            EIC_PHY_IntEn( eic_group_cfg_ptr, chan_id, SCI_FALSE );
            
            if( s_eic_sync_int_table[chan_id] != PNULL )
            {
                EIC_PHY_MskData(eic_group_cfg_ptr, i);
                state =  EIC_PHY_GetData(eic_group_cfg_ptr, chan_id);               
                (*s_eic_sync_int_table[chan_id])(chan_id, state);
            }
            else
            {    
                SCI_PASSERT(0, ("EIC_A_SYNCIsr sync chan[%d]'s callback is PNULL.", chan_id));/*assert verified*/
            }

            //clear its int flag
            EIC_PHY_ClrIntSts(eic_group_cfg_ptr, chan_id);
            
            // Ready for next trig signal 
            EIC_PHY_IntEn(eic_group_cfg_ptr, chan_id, SCI_TRUE);            
        }
    }
#endif
    return ISR_DONE;
}

/*  Description:    Register the int callback for EICD_DbncIsr
 *  Author:         mei
 *  Note:           SC2720 only suport DBNC
 */
PUBLIC void EICD_Dbnc_RegCallback( uint32 chan_id, EIC_CALLBACK callback )
{ 
    /*assert to do*/
    SCI_ASSERT( chan_id < EIC_DBNC_SIZE);

    /* EICDRV_TraceLow:"EICD_Dbnc_RegCallback" */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_758_112_2_17_23_4_21_961,
        (uint8*)"");
    
    if(s_eic_dbnc_int_table[chan_id] == PNULL)
    {
        s_eic_dbnc_int_table[chan_id] = callback;
    }
    else
    {
    /* EICDRV_TraceLow:"EICD_Dbnc_RegCallback-----callback is NULL." */
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_766_112_2_17_23_4_21_962,
        (uint8*)"");
    }
}

/******************************************************************************/
//  Description:    Register the int callback for EICA_DbncIsr
//  Author:         lichd
//  Note:     
//                  
/******************************************************************************/
PUBLIC void EICA_Dbnc_RegCallback( uint32 chan_id, EIC_CALLBACK callback )
{
#if !defined(PLATFORM_UWS6121E)
    SCI_ASSERT( chan_id < EICA_DBNC_SIZE); /*assert to do*/

    //EICDRV_TraceLow:"EICA_Dbnc_RegCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_778_112_2_17_23_4_21_963,(uint8*)"");
    
    if(s_eica_dbnc_int_table[chan_id] == PNULL)
    {
        s_eica_dbnc_int_table[chan_id] = callback;
    }
    else
    {
    //EICDRV_TraceLow:"EICA_Dbnc_RegCallback-----callback is NULL."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,EIC_PHY_V5_786_112_2_17_23_4_21_964,(uint8*)"");
    }
 #endif
}

/******************************************************************************/
//  Description:    Register the int callback for EIC_LatchIsr
//  Author:         lichd
//  Note:     
//                  
/******************************************************************************/
PUBLIC void EIC_Latch_RegCallback( uint32 chan_id, EIC_CALLBACK callback )
{
#if !defined(PLATFORM_UWS6121E)
    SCI_ASSERT( chan_id < EIC_LATCH_SIZE); /*assert to do*/
    if(s_eic_latch_int_table[chan_id] == PNULL)
    {
        s_eic_latch_int_table[chan_id] = callback;
    }
    else
    {
        //SCI_PASSERT(0, ("EIC_Latch_RegCallback chan[%d]'s callback already exist.", chan_id)); /*assert to do*/
    }
#endif
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

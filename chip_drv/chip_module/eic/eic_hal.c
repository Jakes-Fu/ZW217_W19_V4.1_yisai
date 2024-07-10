/******************************************************************************
 ** File Name:                eic_hal.c                                        *
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
#include "sci_types.h"
#include "tb_comm.h"
#include "os_api.h"

#include "gpio_drvapi.h"
#include "eic_phy.h"
#include "eic_cfg.h"
/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
 **                        Variable Define                                     *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                        Extern Declare                                      *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                        Local Func Declare                                  *
 **---------------------------------------------------------------------------*/
static uint32 eic_is_init = SCI_FALSE;

/******************************************************************************/
//  Description:    Get the channel id in a group
//  Author:         lichd
//  Note:           
/******************************************************************************/
LOCAL uint32 _eic_get_chanid( uint32 id ) 
{
    uint32 table_size =0;
    EIC_GROUP_DES_T* eic_group_cfg_ptr = EIC_GetGroupCfg( &table_size );

    //uniform addressing for id index
    while( (id >= eic_group_cfg_ptr->size) && (table_size--) )
    {
        id = id - eic_group_cfg_ptr->size;
        eic_group_cfg_ptr++;
    }
    
    if(id <= eic_group_cfg_ptr->size)
    {
        return id;
    }
    else 
    {
        SCI_PASSERT(0, ("_eic_get_chanid id is invalid.")); /*assert verified*/
    }
}

/******************************************************************************/
//  Description:    Get the group config information of the id index
//  Author:         lichd
//  Note:           
/******************************************************************************/
LOCAL EIC_GROUP_DES_T* _eic_get_grpcfg( uint32 id ) 
{
    uint32 table_size =0;
    EIC_GROUP_DES_T* eic_group_cfg_ptr = EIC_GetGroupCfg( &table_size );

    //uniform addressing for id index
    while( (id >= eic_group_cfg_ptr->size) && (table_size--) )
    {
        id = id - eic_group_cfg_ptr->size;
        eic_group_cfg_ptr++;
    }

    if(id <= eic_group_cfg_ptr->size)
    {
        return (eic_group_cfg_ptr);
    }
    else 
    {
        SCI_PASSERT(0, ("_eic_get_grpcfg id is invalid.")); /*assert verified*/
        return 0;//lint !e527 for Coverity & Pclint tools
    }
}


/**----------------------------------------------------------------------------*
 **                        Public Func Declare                                 *
 **---------------------------------------------------------------------------*/
PUBLIC EIC_GROUP_DES_T* EICTEST_GetCfg( uint32 base_addr ) 
{
    uint32 table_size =0;
    EIC_GROUP_DES_T* eic_group_cfg_ptr = EIC_GetGroupCfg( &table_size );
    EIC_GROUP_DES_T* cfg_ptr= {0};

    //uniform addressing for id index
    while( table_size-- )
    {
        if(base_addr == eic_group_cfg_ptr->base_addr)
        {
            cfg_ptr = eic_group_cfg_ptr;
            return cfg_ptr;
        }
        eic_group_cfg_ptr++;
    }

    return cfg_ptr;
}

/******************************************************************************/
//  Description:    eic HAL init
//  Author:         lichd
//  Note:           
/******************************************************************************/
void EIC_HAL_Init (void)
{
    if(!eic_is_init)
    {
	    eic_is_init = SCI_TRUE;
	    EIC_PHY_Open( );  
	}
}

/******************************************************************************/
//  Description:    Get the level of a channel
//  Author:         lichd
//  Note:                 
/******************************************************************************/
BOOLEAN EIC_HAL_GetValue (uint32 id_num)
{
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    EIC_PHY_MskData	(eic_group_cfg_ptr, chan_id);

	return (EIC_PHY_GetData(eic_group_cfg_ptr, chan_id));
}

/******************************************************************************/
//  Description:    Set trig condition for the interrupt
//  Author:         lichd
//  Note:                 
/******************************************************************************/
void EIC_HAL_SetIntSense (uint32 id_num, GPIO_INT_TYPE sense_type)
{
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );
    EIC_TRG_TYPE_E eic_trg_type = 0;

    switch(sense_type)
    {
        case GPIO_INT_LEVEL_HIGH: eic_trg_type=HIGH_LEVEL; break;

        case GPIO_INT_LEVEL_LOW:  eic_trg_type=LOW_LEVEL; break;

        case GPIO_INT_EDGES_BOTH: eic_trg_type=BOTH_EDGE; break;

        case GPIO_INT_EDGES_RISING: eic_trg_type=RISING_EDGE; break;

        case GPIO_INT_EDGES_FALLING: eic_trg_type=FALLING_EDGE; break;

        default: break;
    }

    EIC_PHY_SetTrg(eic_group_cfg_ptr, chan_id, eic_trg_type); 
}

/******************************************************************************/
//  Description:    Enable or disable the interrupt
//  Author:         lichd
//  Note:                 
/******************************************************************************/
void EIC_HAL_EnableInt (uint32 id_num, uint32 is_en)
{
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    EIC_PHY_IntEn(eic_group_cfg_ptr, chan_id, is_en); 
}

/******************************************************************************/
//  Description:    Set the trig parameter for EIC_DBNC interrupt.
//  Author:         lichd
//  Note:           EIC_DBNC specified       
/******************************************************************************/
void EIC_HAL_TrgCfg (uint32 id_num, BOOLEAN valide_value, uint32 dbnc_cnt_ms)
{
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    EIC_HAL_SetIntSense(id_num, (GPIO_INT_TYPE)valide_value); 
    EIC_PHY_SetDbnc(eic_group_cfg_ptr, chan_id, dbnc_cnt_ms);
}

/******************************************************************************/
//  Description:    Register callback from EIC_DBNC module
//  Author:         lichd
//  Note:           
/******************************************************************************/
BOOLEAN EIC_DBNC_RegCallback (
                                uint32           id_num,
                                BOOLEAN          valide_value,    
                                BOOLEAN          shaking_en,
                                uint32           shaking_interval_ms,
                                EIC_CALLBACK     eic_callback
)
{
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    SCI_TRACE_LOW("EIC_DBNC_RegCallback id_num=%d, chan_id=%d", id_num, chan_id);

    if(!eic_is_init)
    {
	    eic_is_init = SCI_TRUE;
	    EIC_PHY_Open( );  
	}

    EIC_HAL_TrgCfg(id_num, valide_value, shaking_interval_ms);
    
    EIC_PHY_DbncEn(eic_group_cfg_ptr, chan_id, shaking_en);

    if(EIC_D_DIE == eic_group_cfg_ptr->die)
        EICD_Dbnc_RegCallback(chan_id, eic_callback);
	else
        EICA_Dbnc_RegCallback(chan_id, eic_callback);

    EIC_PHY_IntEn(eic_group_cfg_ptr, chan_id, SCI_TRUE);
    EIC_PHY_StartTrg(eic_group_cfg_ptr, chan_id);

    return SCI_TRUE;
}

/******************************************************************************/
//  Description:    Set the trig parameter for EIC_LATCH interrupt.
//             :    valide_value:  1--HIGH_LEVEL 0--LOW_LEVEL
//             :    trg_with_sleep:1--with sleep 0--without sleep
//  Author:         lichd
//  Note:           EIC_LATCH specified       
/******************************************************************************/
void EIC_HAL_TrgWithSleep (uint32 id_num, BOOLEAN valide_value, BOOLEAN trg_with_sleep_en)
{
#if !defined(PLATFORM_UWS6121E)
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    EIC_HAL_SetIntSense(id_num, (GPIO_INT_TYPE)valide_value); 
    EIC_PHY_TrgSleepEn(eic_group_cfg_ptr, chan_id, trg_with_sleep_en);
#endif
}

/******************************************************************************/
//  Description:    Register callback from EIC_LATCH module
//  Author:         lichd
//  Note:           
/******************************************************************************/
BOOLEAN EIC_LATCH_RegCallback (
                                uint32           id_num,
                                BOOLEAN          valide_value,    
                                BOOLEAN          sleepout_en,
                                EIC_CALLBACK     eic_callback
)
{
#if !defined(PLATFORM_UWS6121E)
    uint32           chan_id           = _eic_get_chanid( id_num );
    EIC_GROUP_DES_T* eic_group_cfg_ptr = _eic_get_grpcfg( id_num );

    EIC_HAL_SetIntSense(id_num, (GPIO_INT_TYPE)valide_value); 
    
    EIC_PHY_TrgSleepEn(eic_group_cfg_ptr, chan_id, sleepout_en);

    EIC_Latch_RegCallback(chan_id, eic_callback);
    
    EIC_PHY_IntEn(eic_group_cfg_ptr, chan_id, SCI_TRUE);
#endif
    return SCI_TRUE;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

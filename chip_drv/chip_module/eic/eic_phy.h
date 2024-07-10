/******************************************************************************
 ** File Name:    eic_phy.h                                                   *
 ** Author:       lichd                                                       *
 ** DATE:         6/14/2011                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **                                    *
 ******************************************************************************/
#ifndef _EIC_PHY_H_
#define _EIC_PHY_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#include "sci_types.h"
#ifdef CHIP_VER_6530
#include "sc6530_eic_cfg.h"
#endif
#include "eic_cfg.h"

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Function Declare                        **
**----------------------------------------------------------------------------*/
PUBLIC void   EIC_PHY_MskData( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id);
PUBLIC uint32 EIC_PHY_GetData( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id);

PUBLIC void EIC_PHY_Open( void );
PUBLIC void EIC_PHY_Close( void );
PUBLIC void EIC_PHY_IntEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id,
                           BOOLEAN is_en);
PUBLIC void EIC_PHY_DbncEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id,
                            BOOLEAN is_en);
PUBLIC void EIC_PHY_SetTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id,
                            EIC_TRG_TYPE_E trg_type_e );
PUBLIC void EIC_PHY_StartTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id);
PUBLIC void EIC_PHY_SetDbnc( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id, uint32 cnt);
PUBLIC void EIC_PHY_TrgSleepEn( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id, BOOLEAN is_en);

PUBLIC uint32 EIC_PHY_GetIntSts( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id);
PUBLIC void EIC_PHY_ClrIntSts( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id);
PUBLIC void EIC_PHY_ReverseTrg( EIC_GROUP_DES_T* eic_group_cfg_ptr, uint32 chan_id );

PUBLIC ISR_EXE_T EICD_DbncIsr( uint32 data );
PUBLIC ISR_EXE_T EICA_DbncIsr( uint32 data );
PUBLIC ISR_EXE_T EIC_LatchIsr( uint32 data );
PUBLIC ISR_EXE_T EIC_A_SYNCIsr( uint32 data);

PUBLIC void EICD_Dbnc_RegCallback( uint32 chan_id, EIC_CALLBACK callback );
PUBLIC void EICA_Dbnc_RegCallback( uint32 chan_id, EIC_CALLBACK callback );
PUBLIC void EIC_Latch_RegCallback( uint32 chan_id, EIC_CALLBACK callback );

PUBLIC void EIC_PHY_EIC_DBNC_IE_EN( void );
PUBLIC void EIC_PHY_EIC_DBNC_IE_UNEN( void );
PUBLIC void EIC_PHY_EIC_DBNC_DMSK_EN( void );
PUBLIC void EIC_PHY_EIC_DBNC_DMSK_UNEN( void );
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End _EIC_PHY_H_


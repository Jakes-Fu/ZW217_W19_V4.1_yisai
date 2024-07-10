/******************************************************************************
 ** File Name:         sc6531efm_eic_cfg.c                                        *
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
#include "sc6531efm_eic_cfg.h"
#include "../eic/eic_cfg.h"

/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
 **                        Variable Define                                     *
 **---------------------------------------------------------------------------*/



// config EIC for SC6531EFM
LOCAL const EIC_GROUP_DES_T  s_eic_group_table[] =
{
    /*	base_addr			    group_type    size  die    */    
    {EIC_DBNC_REG_BASE,  	EIC_DBNC,         16,   EIC_D_DIE},
    {EICA_DBNC_REG_BASE,	EIC_DBNC,         16,   EIC_A_DIE},        
    {EIC_LATCH_REG_BASE,	EIC_LATCH,        8,    EIC_D_DIE},
    {EIC_ASYNC_REG_BASE,	EIC_ASYNC,        8,    EIC_D_DIE},    
    {EIC_SYNC_REG_BASE,     EIC_SYNC,         8,    EIC_D_DIE}, 
};

/**----------------------------------------------------------------------------*
 **                        Public Func Declare                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to Get chip_spec configuration
//  Author:         lichd
//  Note:           
/*****************************************************************************/
PUBLIC EIC_GROUP_DES_T* EIC_GetGroupCfg( uint32 *pSize )
{
    *pSize = sizeof(s_eic_group_table)/sizeof(s_eic_group_table[0]);

    return (EIC_GROUP_DES_T*)(s_eic_group_table);
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

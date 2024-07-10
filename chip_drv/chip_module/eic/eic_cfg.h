/******************************************************************************
 ** File Name:    eic_cfg.h                                                   *
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
#ifndef _EIC_CFG_H_
#define _EIC_CFG_H_
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

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
static __inline void EIC_REG_SET (uint32 reg_addr, uint32 value)
{
    if(!ANA_IS_ANA_REG(reg_addr))
    {
        CHIP_REG_SET (reg_addr,value);
    }
    else
    {
        ANA_REG_SET (reg_addr,value);
    }

    return;
}

static __inline uint32 EIC_REG_GET (uint32 reg_addr)
{
    if(!ANA_IS_ANA_REG(reg_addr))
    {
        return REG32 (reg_addr);
    }
    else
    {
        return ANA_REG_GET (reg_addr);
    }
}

static __inline void EIC_REG_AND (uint32 reg_addr, uint32 value)
{
    if(!ANA_IS_ANA_REG(reg_addr))
    {
        CHIP_REG_AND (reg_addr,value);
    }
    else
    {
        ANA_REG_AND (reg_addr,value);
    }
}

static __inline void EIC_REG_OR (uint32 reg_addr, uint32 value)
{
    if(!ANA_IS_ANA_REG(reg_addr))
    {
        CHIP_REG_OR (reg_addr,value);
    }
    else
    {
        ANA_REG_OR (reg_addr,value);
    }
}

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum _eic_trg_type_e
{
    LOW_LEVEL = 0x0,
    HIGH_LEVEL,
    BOTH_LEVEL,
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE
}  EIC_TRG_TYPE_E;

typedef enum _eic_die_type_e
{
    EIC_D_DIE = 0x0,
    EIC_A_DIE,
} EIC_DIE_TYPE_E;

typedef enum _eic_grou_type_e
{
    EIC_DBNC = 0x0,
    EIC_LATCH,
    EIC_ASYNC,
    EIC_SYNC 	
}  EIC_GROUP_TYPE_E;

// Define the description of a group
typedef struct _eic_group_des_t
{
    uint32 base_addr;
    EIC_GROUP_TYPE_E type;
    uint32 size;
    EIC_DIE_TYPE_E   die;
}  EIC_GROUP_DES_T;

// Define the description of a group
typedef struct _pmic_group_des_t
{
    uint32 base_addr;
    EIC_GROUP_TYPE_E type;
    uint32 size;
    EIC_DIE_TYPE_E   die;
}  PMIC_GROUP_DES_T;

typedef void (*EIC_CALLBACK)(uint32 id, uint32 state);


PUBLIC EIC_GROUP_DES_T* EIC_GetGroupCfg( uint32 *pSize );

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End _EIC_CFG_H_


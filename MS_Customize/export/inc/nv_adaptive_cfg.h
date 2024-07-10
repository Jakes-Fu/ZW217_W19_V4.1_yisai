/******************************************************************************
 ** File Name:      nv_adaptive_cfg.h                                         *
 ** Author:         abe.yue                                                   *
 ** DATE:           04/07/2020                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configure about get adaptive  *
					version.												  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/07/2020     abe.yue          Create.                                   *
 ******************************************************************************/
#ifndef _NV_ADAPTIVE_CFG_H
#define _NV_ADAPTIVE_CFG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "gpio_drvapi.h"
#include "adc_drvapi.h"
#include "os_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structure 									  *
 **---------------------------------------------------------------------------*/
typedef struct
{
	uint32 gpio_num;		//the GPIO number used to identify the adaptive version
	uint32 version_bit;		//the bit of the adaptive version corresponding to the gpio munber
}VERSION_GPIO_CFG_T;


typedef enum crystal_type {
	DCXO = 1,
	TSX,
}crystal_type_t;


/**---------------------------------------------------------------------------*
 **                       Function Prototypes                                 *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get RF version from adc
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_Get_RF_VERSION(void);

/*****************************************************************************/
//  Description:    This function get PAversion from gpio
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_Get_PA_VERSION(void);

/*****************************************************************************/
//  Description:    This function get Crystal Type from gpio
//  Author:         abe.yue
//  Note:
/*****************************************************************************/
PUBLIC crystal_type_t SCI_Get_Crystal_Type(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif//_NV_ADAPTIVE_CFG_H

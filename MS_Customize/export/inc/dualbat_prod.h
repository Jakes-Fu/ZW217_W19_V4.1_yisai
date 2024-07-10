/******************************************************************************
 ** File Name:      dualbat_drvapi.h                                          *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           21/06/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 dual battery module.                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/06/2011     Mingwei.Zhang     Create.                                  *
 ******************************************************************************/

#ifndef _DUALBAT_PROD_H_
#define _DUALBAT_PROD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 #include "dualbat_drvapi.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define DBAT_ADC_CHANNEL_MAIN   8
#define DBAT_ADC_CHANNEL_AUX    0

/**---------------------------------------------------------------------------*
 **                         Type Definations                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    Get product function table,when system power on.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_PROD_FUN_T* DBAT_Prod_GetFun(void);

/*****************************************************************************/
//  Description:    Get product config table,when system power on.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC DBAT_PROD_CFG_T * DBAT_Prod_GetCfg(void);

#ifdef __cplusplus
}
#endif

#endif  // _DUALBAT_PROD_H_



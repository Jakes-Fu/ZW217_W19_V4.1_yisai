/******************************************************************************
 ** File Name:      tb_drv.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operations for driver.        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     Xueliang.Wang     Create.                                  *
 ******************************************************************************/

#ifndef _TB_DRV_H_
#define _TB_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip.h"
#include "sc_reg.h"
#include "os_api.h"
#include "isr_drvapi.h"
#include "rtc_drvapi.h"
#ifndef WIN32
#include "sio.h"
#endif
#include "timer_drvapi.h"
#include "adc_drvapi.h"
#include "tpc_drvapi.h"
#include "kpd_drvapi.h"
#include "tb_comm.h"

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  API mappings.
/*****************************************************************************/

#ifndef WIN32
#define DRV_RegHandler          ISR_RegHandler
#define DRV_UnRegHandler        ISR_UnRegHandler
#define DRV_Callback            ISR_Callback

#else
#define DRV_RegHandler(a, b) 0
#define DRV_UnRegHandler(a)  0
#define DRV_Callback(a)  0
#endif


#endif  // _TB_DRV_H

//  End of tb_drv.h

/******************************************************************************
 ** File Name:      tb_dal.h                                                  *
 ** Author:         Richard.Yang                                              *
 ** DATE:           10/11/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the peripheral device.                                 *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in sci_types.h   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/11/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _TB_DAL_H_
#define _TB_DAL_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// Basic data types.
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#define ATC_MODE   0x01
#define DATA_MODE  0x02
#define MIX_MODE   0x03

#define COM_USER          0
#define COM_DATA          0
#define COM_DEBUG         1
#endif

#ifndef WIN32
#include "sio.h"
#endif

// Keypad Device
#include "dal_keypad.h"

// RTC time Device
#include "dal_time.h"

// Power Device
#include "dal_power.h"

// DigitalCamera Device
//#include "dal_dc.h"

// Charge Device
#include "chg_drvapi.h"

// LCD Device
#include "dal_lcd.h"

// Img Device
#include "dal_img.h"

// Audio Device
#include "dal_audio.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // End of _OS_API_H

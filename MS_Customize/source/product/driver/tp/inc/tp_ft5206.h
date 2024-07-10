/******************************************************************************
 ** File Name:      tp_ft5206.h                                               *
 ** DATE:           2011.03.15                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.15   Sunny.Fei             Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _TP_FT5206_H_
#define _TP_FT5206_H_

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
//#include "os_api.h"
#include "tp_srv.h"
#include "tp_multi_drv.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define CHIP_WORK_MODE_ACTIVE       0x0
#define CHIP_WORK_MODE_MONITOR      0x1
#define CHIP_WORK_MODE_HIBERNATE    0x3

//Configure register
#define REG_VALID_DETECT            0x80
#define REG_ZOOM_DISTANCE           0x97
#define REG_CHIP_MODE               0xa5
#define REG_PERIODACTIVE            0x88

#define REG_GESTURE_ID              0x01
#define REG_NUM_FINGER              0x02
#define REG_1ST_FINGER              0x03
#define REG_2ED_FINGER              0x09
#define REG_3RD_FINGER              0x0F
#define REG_4TH_FINGER              0x15
#define REG_5TH_FINGER              0x1B
#define REG_GT_ZOOM_OUT             0x49
#define REG_GT_ZOOM_IN              0x48
#define REG_FIRMWARE_ID				0xa6
#define REG_CALI_TPC			0xfc

#define TOUCH_VALID_THRESHOLD       0x16
#define TOUCH_VAL_ZOOM_THRESHOLD    0x30
#define TOUCH_PERIODACTIVE          0x6

//touchpanel firmware update
#define TP_ERR_READID   			0x02
#define TP_ERR_ECC    				0x03
#define TP_OK						0x04
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

extern PUBLIC TPC_OPERATION_T tpc_ft5206_ops;

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _TP_FT5206_H_

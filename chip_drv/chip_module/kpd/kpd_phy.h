/******************************************************************************
 ** File Name:      kpd_phy.h                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/

#ifndef _KPD_PHY_H_
#define _KPD_PHY_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
#define KPDDRV_PRINT        SCI_TraceLow
#define MAX_SCAN_KEY_CODE   ((KPD_COL_NUM * KPD_ROW_NUM) - 1)

typedef struct KPD_KEY_tag
{
    uint16  key_code;
    uint8   state;
} KPD_KEY_T;

typedef enum
{
    KPD_PRESS_INT_RESERVED = 0,
    KPD_PRESS1_INT = 1,
    KPD_PRESS2_INT,
    KPD_PRESS3_INT,
    KPD_PRESS4_INT,
    KPD_RELEASE1_INT,
    KPD_RELEASE2_INT,
    KPD_RELEASE3_INT,
    KPD_RELEASE4_INT,
    KPD_LONGPRESS1_INT,
    KPD_LONGPRESS2_INT,
    KPD_LONGPRESS3_INT,
    KPD_LONGPRESS4_INT
} KPD_INT_TYPE_E;

typedef enum
{
    KPD_NUM_1 = 0x01,
    KPD_NUM_2,
    KPD_NUM_3,
    KPD_NUM_4,
    KPD_NUM_MAX
} KPD_NUM_E;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

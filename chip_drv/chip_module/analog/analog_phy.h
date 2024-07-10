/******************************************************************************
 ** File Name:      analog_phy.h                                        *
 ** Author:         Jeff.Li                                                   *
 ** DATE:           11/06/2010                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Analog interface.                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/06/2010     Jeff.Li          Create for new chip driver architecture.  *
 *****************************************************************************/

#ifndef _ANALOG_PHY_H_
#define _ANALOG_PHY_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACOR Definitions                                 *
 **---------------------------------------------------------------------------*/
//#define DEBUG_ANA
#ifdef DEBUG_ANA
#define ANA_PHY_PRINT SCI_TRACE_LOW
#else
#define ANA_PHY_PRINT(_x_) 
#endif

#define ANA_SW_INDEX_MAX                10
#define ANA_INVALID_VALUE               0xffff
#define VIBR_INIT_V 0xC
/* set convert time to 500ms, VIBR_CONVERT_V = convert_time(ms)*32.768KHz */
/* 20120307: Will.Jiang_cr171523 : 0x4000 ---> 0x400,  0x400 * ( 1/ 32.768KHz) = 33.554 ms */
/* 0x98    (5ms) */
/* 0x130   (10ms) */

#define VIBR_CONVERT_V 0x98

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef void (*ANA_SET_VALUE_FUNC) (uint32 value);

typedef struct
{
    uint16 min_vbat_value;
    uint16 max_vbat_value;
    uint8  min_level;
    uint8  max_level;
} ANA_SW_T, *ANA_SW_T_PTR;

typedef struct
{
    ANA_DEV_ID_E        id;
    uint32              max_real_value;
    uint32              cur_value;
    ANA_SW_T_PTR        switch_tab_ptr;
    uint32              switch_tab_index;
    ANA_SET_VALUE_FUNC  set_value_func;
} ANA_DEV_T, *ANA_DEV_T_PTR;

typedef struct
{
    uint32 reg_addr;
    uint32 mask;
    uint32 shift;
    uint32 value;
    uint32 step;
    uint32 interval;
}LEVEL_INFO_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Get analog device information table
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC ANA_DEV_T_PTR ANA_GetDevCfgTab (void);

PUBLIC uint32 ANA_GetLCMBackLightStatus (void);

PUBLIC void ANA_PHY_Init (void);

PUBLIC void ANA_PHY_SelectDevFunc (ANA_DEV_ID_E dev_id, uint32 value);

PUBLIC void ANA_PHY_SetDevParam (ANA_DEV_ID_E dev_id, ANA_DEV_PARAM_T* param);
PUBLIC void ANA_PHY_SetCodecTpFunc(uint32 value);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif    // _ANALOG_MODULE_HW_H

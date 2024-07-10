/*****************************************************************************
 **  File Name:    efuse_phy.h                                               *
 **  Author:       Jenny Deng                                                *
 **  Date:         20/10/2009                                                *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 **  Description:  This file defines the basic operation interfaces of       *
 **                EFuse initilize and operation. It provides read and       *
 **                writer interfaces of 0~5 EFUSE_. Efuse 0 for Sn block.     *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.  *
 *****************************************************************************
 *****************************************************************************
 **  Edit History                                                            *
 **--------------------------------------------------------------------------*
 **  DATE               Author              Operation                        *
 **  20/10/2009         Jenny.Deng          Create.                          *
 **   26/06/2012        windy.wang          Update                           *
 *****************************************************************************/

#ifndef EFUSE_PHY_H
#define EFUSE_PHY_H

#include "sci_types.h"
#include "efuse_drvapi.h"

/***********************structure define**************************************/
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned char		bool;
typedef int int32_t;
#define EFUSE_READ_TIMEOUT       2 //ms
#define EFUSE_WRITE_TIMEOUT      3 //ms
#define EFUSE_BIST_TIMEOUT       3 //ms

#define MAGIC_NUM                0x8810
#define PMIC2720_MAGIC_NUM       (0x2720)

#define BASE_ADC_P0   785   // 3.6V
#define BASE_ADC_P1   917   // 4.2V
#define VOL_P0        3600
#define VOL_P1        4200
#define ADC_DATA_OFFSET 128
#define false 0
#define true 1

typedef enum
{
    EFUSE_PROG_MODE   = 0,
    EFUSE_READ_MODE,
    EFUSE_NORMAL_RD_FLAG_CLR_MODE,
//    EFUSE_BIST_MODE
} EFUSE_MODE_CTL_E;

typedef enum
{
    EFUSE_MIN_ID,
    EFUSE_HASH_START_ID,
    EFUSE_HASH_END_ID,
    EFUSE_IMEI_START_ID,
    EFUSE_IMEI_END_ID,
    EFUSE_UID_START_ID,
    EFUSE_UID_END_ID,
    EFUSE_CAL_ID,//calibaration and CCCV ID
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    EFUSE_LDOA_REF_CAL_ID,
    EFUSE_LDOB_REF_CAL_ID,
    EFUSE_LDODCXO_REF_CAL_ID,
    EFUSE_ADC_CAL_HEADMIC_ID,
    EFUSE_ADC_CAL_BIG_ID,
    EFUSE_ADC_CAL_SMALL_START_ID,
    EFUSE_ADC_CAL_SMALL_END_ID,

    EFUSE_ADC_CAL_SMALL_ID,

    EFUSE_BT_CAL_ID,
    EFUSE_AUD_CAL_ID,
    EFUSE_RC_CAL_ID,  //calibration RC1M
    EFUSE_IB_CAL_ID,
    EFUSE_CHRG_CV_CAL_ID,
    EFUSE_LDOCORE_SLP_CAL_ID,
    EFUSE_CPLL_LDO_CAL_ID,
#endif
#if defined(PLATFORM_UWS6121E)
    EFUSE_RG_LDO_RTC_CAL_ID,
#endif
    EFUSE_MAX_ID
} EFUSE_BLK_FUN_INDEX;

typedef struct
{
    bool efusevalid;
    uint16_t deltaA; ///< Block16 bat sense deltaA for AuxADC Scale3@4.2V
    uint16_t deltaB; ///< Block17 bat sense deltaB for AuxADC Scale3@3.6V
    uint16_t deltaG; ///< Block20 bat_det deltaG  Scale1@0.2V
    uint16_t deltaH; ///< Block21 bat_det deltaH  Scale1@1.4V
    uint16_t deltaE; ///< Block18 adc1 deltaE for AuxADC Scale10@ 0.1V
    uint16_t deltaF; ///< Block19 adc1 deltaE for AuxADC Scale10@ 1V
    uint16_t deltaC; ///< Block22 adc1 deltaE for AuxADC Scale10@ 0.3V
    uint16_t deltaD; ///< Block23 adc1 deltaE for AuxADC Scale10@ 3V
    uint16_t deltaJ; ///< Block24 adc1 deltaE for AuxADC Scale10@ 0.5V
    uint16_t deltaK; ///< Block25 adc1 deltaE for AuxADC Scale10@ 4V
} adcPmicEfuse_data_t;

PUBLIC void EFUSE_PHY_Init (BOOLEAN is_enable);
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Read (uint32 block_id, uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Read_Ex (uint32 block_id, uint32 *des_data_ptr);
PUBLIC EFUSE_RETURN_E EFUSE_PHY_Write ( uint32 block_id, uint32 write_data);
PUBLIC BOOLEAN EFUSE_PHY_SplitData (uint8 *src_dat_ptr, uint8 *des_dat_ptr, uint32 blk_num, uint32 bit_num, BOOLEAN is_split);
PUBLIC uint32 EFUSE_PHY_GetAppBitLen (EFUSE_BLK_FUN_INDEX blk_hal);
PUBLIC uint32 EFUSE_PHY_GetAppBlkLen (EFUSE_BLK_FUN_INDEX blk_hal);
PUBLIC uint32 EFUSE_PHY_GetBlkId (EFUSE_BLK_FUN_INDEX fun_index);
PUBLIC uint32 EFUSE_PHY_GetBlkNumFromBit (uint32 length);
PUBLIC uint32 *EFUSE_PHY_GetEfusePtr (void);
PUBLIC BOOLEAN EFUSE_PHY_IsBlkIdValid (uint32 block_id);
PUBLIC EFUSE_RETURN_E EFUSE_PHY_IsLock (uint32 block_id);
PUBLIC void EFUSE_PHY1_Open(void);
PUBLIC void EFUSE_PHY1_Close(void);
PUBLIC uint32 EFUSE_PHY1_Read(int page_index);
PUBLIC uint32 EFUSE_PHY1_Read_EX(int page_index, uint8 doubles);
PUBLIC void EFUSE_PHY1_Write(int page_index, uint32 val);
PUBLIC void EFUSE_PHY1_Write_Ex(int page_index, uint32 val, uint8 doubles);
PUBLIC void EFUSE_PHY1_enWCNuart(void);

PUBLIC void drvEfusePmicOpen(void);
PUBLIC void drvEfusePmicClose(void);
PUBLIC bool prvAdcEffuseValid(void);
PUBLIC uint32_t prvAdcGetEffuseBits(int32_t bit_index, int32_t length);

PUBLIC uint8_t prvAdcScale_23_calibrated_get(void);
PUBLIC void prvAdcScale00EffuseGet(void);
PUBLIC void prvAdcScale01EffuseGet(void);
PUBLIC void prvAdcScale02EffuseGet(void);
PUBLIC void prvAdcScale03EffuseGet(void);
PUBLIC void prvAdcScale03_bat_EffuseGet(void);
PUBLIC uint32_t prvAdcScale00Tovol(uint16_t adcvalue);
PUBLIC uint32_t prvAdcScale01Tovol(uint16_t adcvalue);
PUBLIC uint32_t prvAdcScale02Tovol(uint16_t adcvalue);
PUBLIC uint32_t prvAdcScale03Tovol(uint16_t adcvalue);
#endif


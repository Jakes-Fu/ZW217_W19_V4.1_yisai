/******************************************************************************
 ** File Name:      analog_drvapi.h                                              *
 ** Author:         Jeff.Li                                                   *
 ** DATE:           24/02/2009                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Analog interface.                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/02/2009     Jeff.Li          Create.                                   *
 *****************************************************************************/

#ifndef _ANALOG_DRVAPI_H_
#define _ANALOG_DRVAPI_H_

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
#define ANA_DEV_VAL_MIN         0
#define ANA_DEV_VAL_LOW         25

#define ANA_DEV_VAL_HIGH        75
#define ANA_DEV_VAL_MAX         100

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define ANA_DEV_VAL_DEF         70//vibrator default value for NEMOG
#else
#define ANA_DEV_VAL_DEF         50
#endif

#define ANA_DEV_CLOSE           0
#define ANA_DEV_OPEN            100

#define ANA_LCM_BL_IB4_IB5_OPEN            0
#define ANA_LCM_BL_IB4_IB5_CLOSE           1

#define ANA_VBAT_VALUE_NUM              5
#define ANA_VBAT_STATIS_PERIOD          0x07
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    ANA_DEV_ID_LCM_BL = 0x00,
    ANA_DEV_ID_LCM_BRIGHTNESS,
    ANA_DEV_ID_KPD_BL,
    ANA_DEV_ID_VIBRATOR,
    ANA_DEV_ID_SD_PWR,
    ANA_DEV_ID_FLASH_LIGHT,
    ANA_DEV_ID_MAX

}
ANA_DEV_ID_E;

typedef struct
{
    uint32 statis_vbat;
    uint16 vbat_tab_index;
    uint16 vbat_tab[ANA_VBAT_VALUE_NUM];

} ANA_VBAT_T;


typedef struct
{
    ANA_DEV_ID_E        id;
    uint32 tuning_step;     //step between tunings
    uint32 tuning_interval; //delay between tuning steps(uint: ms)
}ANA_DEV_PARAM_T;

typedef enum
{
	ANA_DEV_ID_TP,
	ANA_DEV_ID_AUDIO
}
ANA_TP_AUDIO_DEV_ID;
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Initialize analog module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void ANA_Init (void);

/*****************************************************************************/
//  Description:    Set analog value for analog device
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SetDevValule (ANA_DEV_ID_E dev_id, uint32 value);

/*****************************************************************************/
//  Description:    Select Fucntion for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SelectDevFunc (ANA_DEV_ID_E dev_id, uint32 value);

/*****************************************************************************/
//  Description:    Set Params for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SetDevParams (ANA_DEV_ID_E dev_id, ANA_DEV_PARAM_T* param);
/*****************************************************************************/
//  Description:   Set TP and Audio Codec
//  Author:    Tianyu.Yang  
//  Note:
/*****************************************************************************/
PUBLIC void ANA_DecisionCodecTpFunc (ANA_TP_AUDIO_DEV_ID dev_id, uint32 value);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif    // _ANALOG_DRVAPI_H_

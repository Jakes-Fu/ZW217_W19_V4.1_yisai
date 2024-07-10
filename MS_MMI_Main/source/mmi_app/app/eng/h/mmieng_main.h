/****************************************************************************
** File Name:      mmieng_main.h							                                                 *
** Author:          allen								                                                 *
** Date:             2004.08.17														*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.      				*
** Description:    									               					*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 08/2004      allen               Create
** 
****************************************************************************/
#ifndef  _MMIENG_MAIN_H_    
#define  _MMIENG_MAIN_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif

#ifdef CMMB_SUPPORT
#include "mmimtv_main.h"
//#include "mmieng.h"
//#include "mmieng_internal.h"
#endif

typedef enum
{
	MMI_VOICEPARAM_NULL,
	
	MMI_HANDHOLD_SIDETONE_GAIN,
	MMI_EARPHONE_SIDETONE_GAIN,
	
	MMI_HANDHOLD_DIGITAL_GAIN,
	MMI_HANDFREE_DIGITAL_GAIN,
	MMI_EARPHONE_DIGITAL_GAIN,
	
	MMI_HANDHOLD_PGA_GAIN,
	MMI_HANDFREE_PGA_GAIN,
	MMI_EARPHONE_PGA_GAIN,
	
	MMI_HANDHOLD_DIGITAL_SCALE,
	MMI_HANDFREE_DIGITAL_SCALE,
	MMI_EARPHONE_DIGITAL_SCALE
} MMI_VOICEPARAM_E;

#ifdef CMMB_SUPPORT
typedef enum
{
    MMIENG_MTV_CHANNEL_PLAY,
    MMIENG_MTV_CHANNEL_RF_TEST
} MMIENG_MTV_CHANNEL_DISP_MODE_E;

typedef struct CMMB_CHANNEL_INFO_T_tag
{
    uint16	bler_err_blk;
    uint16	bler_num_blk;
    int16	rssi_min;
    int16	rssi_max;
} CMMB_CHANNEL_INFO_T;

//cmmbtest.ini≈‰÷√Œƒº˛√Ë ˆ
typedef struct MMIENG_CMMB_INI_FILE_INFO_T_tag
{
    uint32  network_id;
    uint32  service_id;
    uint32  rf_test_freq;

    CMMB_CHANNEL_INFO_T channel_info;
} MMIENG_CMMB_INI_FILE_INFO_T;
#endif

#define CMMB_TEST_FREQ_LEN 6
#define CMMB_TEST_BLER_LEN 10
#define CMMB_TEST_RSSI_LEN 10

PUBLIC void MMIENG_SetMonkeyTestMode(BOOLEAN flag);
PUBLIC void GprsTest_InitModule(void);

#ifdef __cplusplus
    }
#endif

#endif


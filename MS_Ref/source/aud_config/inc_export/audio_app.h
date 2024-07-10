/******************************************************************************
 ** File Name:      audio_app.h                                               *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines audio general application interface.    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/30/2010     shujing.dong     Create.                                   *
 ******************************************************************************/

 /*! \file audio_app.h
  *  \author shujing.dong
  *  \date june 30, 2010
  *  \brief This file defines audio general application interface.
  */

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "audio_api.h"
#include "audio_config.h"

#define MAX_ADSP_AUDIO_PATH_NUM 3
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

typedef enum 
{
    DATA_DUMP_TO_FILE = 0,
    DATA_DUMP_TO_ARM_LOG = 1,
}AUD_DUMP_TO_FLAG_E;

typedef enum 
{
    DUMP_SAVE_TO_RAM = 0,
    DUMP_SAVE_TO_FLASH = 1,
}AUD_DUMP_SAVE_FLAG_E;


typedef enum {
	AUDIO_VOLTE_INIT,
	AUDIO_VOLTE_START,
	AUDIO_VOLTE_STOP,
	AUDIO_VOLTE_DEINIT,
	AUDIO_VOLTE_PARASET,
	AUDIO_VOLTE_MIX
}AUDIO_VOLTE_COMMAND_E_;

typedef uint32 AUDIO_VOLTE_COMMAND_E;

typedef enum {
	NORMAL_CALL_ENABLE = 0x1,
	VOLTE_CALL_ENABLE =0x2,
}AUDIO_VOLTE_CALL_ENALBE_E_;

typedef uint32 AUDIO_VOLTE_CALL_ENALBE_E;

typedef struct {
	READDATA_CALLBACK_PFUNC readdataClkFunc;
	WRITEDATA_CALLBACK_PFUNC writedataClkFunc;
	void * readparam;
	void * writeparam;
}AUDIO_VOLTE_COMMAND_PARA_INIT_T;

/**
 * @sampleRate : 0:8000; 1:16000; 2:32000; 3:48000
 * @uiformat : 3--amr_nb, 8--amr_wb, 11--pcm,14--evs_primary,15--evs_amrwb_io.
 * @uiFrameTypeIndex : DSP: RateReq.
 * @dtxFlag : DSP: dtx_flag.
 * @evsMode : DSP: evs_mode.
 * @evsSidInteval : DSP: evs_sid_inteval.
 */
typedef struct {
	uint32 sampleRate;
	uint32 uiformat;
	uint32 uiFrameTypeIndex;
	uint32 dtxFlag;
	uint32 evsMode;
	uint32 evsSidInteval;
}AUDIO_VOLTE_COMMAND_PARA_PARASET_T;

typedef struct {
	uint32 audioPathNum;
	uint32 mixCtrl[MAX_ADSP_AUDIO_PATH_NUM];
	AUDIO_VOLTE_COMMAND_PARA_PARASET_T para[MAX_ADSP_AUDIO_PATH_NUM];
}AUDIO_VOLTE_COMMAND_PARA_MIX_T;

typedef struct {
	AUDIO_VOLTE_CALL_ENALBE_E eEnableFlag;
}AUDIO_VOLTE_COMMAND_PARA_ENABLE_T;

typedef struct {
	AUDIO_VOLTE_COMMAND_PARA_INIT_T tInit;
	AUDIO_VOLTE_COMMAND_PARA_ENABLE_T tEnableFlag;
	AUDIO_VOLTE_COMMAND_PARA_PARASET_T tParaSet;
	AUDIO_VOLTE_COMMAND_PARA_MIX_T tParaMix;
}AUDIO_VOLTE_COMMAND_PARA_U;

typedef struct {
	AUDIO_VOLTE_COMMAND_E eCommand;
	AUDIO_VOLTE_COMMAND_PARA_U uPara;
}AUDIO_VOLTE_COMMAND_T;

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC void AUDIO_APP_EnableDataFetch(void)
//! \param  is_enable
//! \return AUDIO_RESULT_E
//! \brief  Description: This function is used to set data_fetch enable/disable.
//! \author Author: shujing.dong
//! \note   Note:Nothing.           
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDataFetch(  // If succeed, return AUDIO_NO_ERROR,
                                        // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
//!    BOOLEAN bOn,             //start/stop
//!    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
//!    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
//!    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
//!    )
//! \param bOn              start/stop
//! \param bwrite_to_flash  TRUE:change NV; FALSE: not change nv, active untill reopen phone.
//! \param bdump_to_log     TRUE:DUMP_TO_ARM_LOG  FALSE:DUMP_TO_FILE
//! \param uiposflag        uint32:32 posisions.bit_0:pos_0;bit_1:pos_1....bit_31:pos_31;
//! \return process result. \sa AUDIO_RESULT_E
//! \brief  Description: This function is dynamic open/close dump pcm in audio.
//! \author Author: yaye.jiang
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_APP_EnableDatadump(
    BOOLEAN bOn,             //start/stop
    AUD_DUMP_SAVE_FLAG_E uidump_save, // 1:change NV; 0: active untill reopen phone
    AUD_DUMP_TO_FLAG_E uidump_path, // 0:DUMP_TO_FILE; 1:DUMP_TO_ARM_LOG
    uint32 uiposflag         //posflag:bit_0:pos_0; bit_1:pos_1;.....bit_n:pos_n
    );

PUBLIC void AUDIO_EnableTxClarity(BOOLEAN bEnable);

PUBLIC BOOLEAN AUDIO_GetTxClarityEnable(void);

PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_EnableVoiceCodec(
	AUDIO_VOLTE_COMMAND_T *ptCommand);

PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_Write_Data(uint16 *buff, uint32 buff_size);
PUBLIC AUDIO_RESULT_E AUDIO_VOLTE_Read_Data(uint16 *buff, uint32 *buff_size);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of audio_api.c


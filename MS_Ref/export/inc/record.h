/******************************************************************************
 ** File Name:      record.h                                              *
 ** Author:         Jing.Li                                                   *
 ** DATE:           04/12/2005                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/12/2005     Jing.Li          Create.                                   *
 ******************************************************************************/
#ifndef _RECORD_H
#define _RECORD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_types.h"
#include "audio_api.h"
#include "audio_config.h"
#include "sci_service.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//format of record file
typedef enum{
    ERR_REC_NONE = SCI_SUCCESS,
    ERR_REC_PARAM,
    ERR_REC_MEM,
    ERR_REC_FILE,
    ERR_REC_DOING,
    ERR_REC_CONFLICT,
    ERR_REC_DATA,
    ERR_REC_MAX
}ERR_REC_E;

typedef enum{
	AUDIO_SUCCESS = ( AUDIO_SERVICE<<8 ) | 1,
	RECORD_SUCCESS,
	RECORD_WRITE_FAILED,
	RECORD_WRITE_HEADER_FAILED,
	MAX_AUDIO_EVENTS_NUM
}AUDIOSVR_MSG_SERVICE_E;


typedef struct RECSRV_SIG_tag
{
	SIGNAL_VARS
    BOOLEAN is_header_err;	 
} RECORD_WRITE_ERR_SIG_T;

typedef enum
{
	START_RECORD,
	RECORD_READ_VOICE ,
	RECORD_WRITE_HEADER,
	RECORD_WRITE_HEADER_STATUS,
	RECORD_GET_FREE_SPACE,
	STOP_RECORD,
	PAUSE_RECORD,
	RESUME_RECORD
} RECORD_MSG_SERVICE_E;

typedef xSignalHeaderRec SIGNALS_NO_PARA_T;

typedef SIGNALS_NO_PARA_T RECORD_WR_HEADER_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_FINISH_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_READ_VOICE_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_START_RECORD_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_STOP_RECORD_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_PAUSE_RECORD_SIG_T;
typedef SIGNALS_NO_PARA_T RECORD_RESUME_RECORD_SIG_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to initialize the memory  and create file
//					for recording.If initialization is successful, it will start to
// 					record. If failed, it will return error.
//	Global resource dependence:
//  Author:         Jing Li
//	Note:           
/*****************************************************************************/
PUBLIC ERR_REC_E AUD_StartRecord (
	AUD_REC_FORMAT_E record_format,
	signed short *file_name
	);
	
/*****************************************************************************/
//  Description:    This function pauses the recording.
//	Global resource dependence:	none
//  Author:         Jing Li
//	Note:           
/*****************************************************************************/
PUBLIC void AUD_PauseRecord(void);

/*****************************************************************************/
//  Description:    This function resumes the recording.
//	Global resource dependence:
//  Author:         Jing Li
//	Note:           
/*****************************************************************************/
PUBLIC void AUD_ResumeRecord(void);

/*****************************************************************************/
//  Description:    This function stops the recording.
//	Global resource dependence:
//  Author:         Jing Li
//	Note:           
/*****************************************************************************/
PUBLIC void AUD_StopRecord (void);
	
/*****************************************************************************/
//  Description:    Record initial function. It initializes the static and global
//					variables.
//	Global resource dependence:
//  Author:         Jing Li
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN AdpcmRecord_Init(
            uint32 uiPriority     //adpcm record thread priority
            );

void Record_Init(void);

/*****************************************************************************/
//  Description:    default callbackfunc supply to REF,when event happen
//  Global resource dependence : none
//  Author:         Jing Li
//  Note:           
/*****************************************************************************/
PUBLIC void Record_EventCallBackFunc(
    BLOCK_ID task_id,   // task_id which task want to receive the event
    uint32 event_id,    // event id 
    void *argv          // data follow the event
    );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
	
#ifdef __cplusplus
    }  
#endif

#endif  // _RECORD_H

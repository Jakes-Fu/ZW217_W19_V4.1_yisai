/****************************************************************************************
** File Name:		logsave_output.h													*	
** Author:			William Qian														*
** DATE:			02/23/2010															*
** CopyRight		2010 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This is the head file for main function of Log          			*
*****************************************************************************************

*****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 02/23/2010		William Qian		Create.										   	*
*****************************************************************************************/

#ifndef _LOGSAVE_OUTPUT_H
#define _LOGSAVE_OUTPUT_H

 /**------------------------------------------------------------------------------------*
 **                         Dependencies                                                *
 **-------------------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/
#define ARM_LOG_HEADER_LENGTH   12
#define MAX_ENCODE_LOG_SIZE	       2072
#define MAX_ENCODE_LOG_HEAD_SIZE 24

#define LAST_DSP_DATA_LEN 500//the data len of the last dsp data packet,word 
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         gloable variable                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Init
	DESCRIPTION:    Output init
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_Init(
        void
        );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_Trace_Message
	DESCRIPTION:    Send Trace Message to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_Send_Trace_Message(
                char*   format_str_ptr,
                uint16  length
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_Signal_Message
	DESCRIPTION:    Send Signal Message to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_Send_Signal_Message(
                uint8*  frame_head_ptr,
                uint16  head_length,
                uint8*  content_ptr,
                uint16  content_length
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_ArmLog_Encode_DiagMessage
	DESCRIPTION:    arm log encode diag msg head and context
	NOTES:          taiqiang.xu
------------------------------------------------------------------------------*/
extern uint32  LogSaveOutput_ArmLog_Encode_DiagMessage(
                uint8*   data_ptr,
                uint32   length,
                BOOLEAN isHead   
                );
                
/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_Signal_Message
	DESCRIPTION:    Send Signal Message to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
/*extern void LogSaveOutput_Send_Signal_Message(
                uint32      log_id,
                BLOCK_ID    sender,
                BLOCK_ID    receiver,
                uint16      msg_type,
                uint8       state,
                uint8*      content_ptr,
                uint16      content_length,
                LOG_FPTR    RepktGprsMsg_fptr
                );
*/
/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_Assert_Message
	DESCRIPTION:    Send Signal Message to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_Send_Assert_Message(
                void*   exp,
                void*   file,
                uint32  line
                );

/*------------------------------------------------------------------------------
//  FUNCTION NAME:  LogSaveOutput_IQRecord_Callback
// 	Description:    iq record callback
//	Note:           William Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_IQRecord_Callback(
            uint16  data_length
            );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_IQ_Data
	DESCRIPTION:    Send IQ Data to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern uint32 LogSaveOutput_Send_CP_LOG(
                uint8* log_buff_ptr,
                uint32 log_size);

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Send_DSP_LOG
	DESCRIPTION:    Send DSP Log to LogSave Task
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern uint32 LogSaveOutput_Send_DSP_LOG(
                uint8* log_buff_ptr,
                uint32 log_size);

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_SetReady
	DESCRIPTION:    Set Logsave Task Ready
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_SetReady(
                void
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_SetReadyAuto
	DESCRIPTION:    Set Logsave Task Ready auto runing
	NOTES:          taiqiang.xu
------------------------------------------------------------------------------*/
extern void LogSaveOutput_SetReadyAutoRuning(
                void
                );
                
/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_SetIQReadyEx
	DESCRIPTION:    Set Logsave Task IQ Ready extend
	NOTES:          taiqiang.xu
------------------------------------------------------------------------------*/
extern void LogSaveOutput_SetIQReadyEx(
                void
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_SetNotReady
	DESCRIPTION:    Set Logsave Task Not Ready
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_SetNotReady(
                void
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_SetIQNotReadyEx
	DESCRIPTION:    Set Logsave Task IQ Not Ready extend
	NOTES:          taiqiang.xu
------------------------------------------------------------------------------*/
extern void LogSaveOutput_SetIQNotReadyEx(
                void
                );

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_Fat_TraceLow
	DESCRIPTION:    FAT module trace low function
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern void LogSaveOutput_Fat_TraceLow(
                const char *x_format, ...
                );

extern void LogSaveOutput_GetArmLogBufInfoAfterAssert(uint8** addr, uint32* len);
extern void LogSaveOutput_GetDspLogBufInfoAfterAssert(uint8** addr, uint32* len);
extern void LogSaveOutput_GetDspIqBufInfoAfterAssert(uint8** addr, uint32* len);

/*------------------------------------------------------------------------------
	FUNCTION NAME:  LogSaveOutput_GetLogsaveStart
	DESCRIPTION:    get logsave start or not
	NOTES:          William.Qian
------------------------------------------------------------------------------*/
extern BOOLEAN LogSaveOutput_GetLogsaveStart(
				void
				);

extern BOOLEAN LogSaveOutput_GetAssertMode(void);

extern void LogSaveOutput_SetAssertMode(BOOLEAN assert_mode);

extern void LogSaveOutput_SetDspAssertId(uint16 assert_id);

extern uint16 LogSaveOutput_GetDspAssertId(void);

#endif /* End logsave_output.h*/
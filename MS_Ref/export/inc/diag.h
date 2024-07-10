/******************************************************************************
 ** File Name:      diag.h                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/08/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file include some defines for diag.c.                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _DIAG_H
#define _DIAG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "cmddef.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define 	NV_ID_CALIBRATION	2
#define 	NV_ID_IMEI			5
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct diag_sw_version_tag
{
	char * sw_version_ptr;
} DIAG_SW_VERSION_T;  /*DIAG_SWVER_F*/

typedef enum
{
    FLOW_UPLINK = 0,
    FLOW_DONWLINK
} FLOW_DIR_E;

/**---------------------------------------------------------------------------*
 **                         System Memory Access Map                          *
 **---------------------------------------------------------------------------*/
#define READ_ACC        (1 << 0)
#define WRITE_ACC       (1 << 1)
#define BYTE_ACC        (1 << 2)
#define WORD_ACC        (1 << 3)
#define DWORD_ACC       (1 << 4)


#define   DIAG_SHELL_PACKET_E           uint32
#define  DIAG_PACKET_HEAD_FALG       0x01
#define  DIAG_PACKET_DATA_FALG         0x02
#define  DIAG_PACKET_END_FALG           0x04


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef uint32 (*DIAG_CMD_ROUTINE)( uint8 **dest, uint16 *dest_len, const uint8 *src, uint16 src_len);

typedef uint32 (*DIAG_CMD_ROUTINE_INIT)( void );

typedef struct DIAG_CMD_ROUTINE_INIT_tag
{
    struct DIAG_CMD_ROUTINE_INIT_tag   * next;
    DIAG_CMD_ROUTINE_INIT                init_function;
}DIAG_CMD_ROUTINE_INIT_T;


//struct of Diag response msg to pc
typedef struct
{
				uint16  parameter_type;
				uint16 operation_status;
				uint16 length;

}DIAG_TOOL_CNF_T;


/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Main thread of diagnostic task. It manages the command 
//                  messages from client application tools and send response
//                  back to channel manager. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void thread_diag_entry(  // None return value.
    uint32 argc,    // The count of arguments that follow in argv.
    void *argv);    // Arguments array.

uint32 DIAG_SetFlow(
    BLOCK_ID thread_ID,
    FLOW_DIR_E direction,
    uint32 count);
    
    
//
// Decrisption:
//  When Diag receive a command from remote, It will call the diag command routine callback(if it is not NULL)
//  to allow the client (driver or others) to handle the command first.
//  If the routine return non-zero(that's mean the client has done the command), the diag wouldn't
//  handle the command and output the result(returned by the client) to the remote.
//  If the routine return zero(that's mean the client not handle the command), the diag handle the
//  Command continue.
//
// Author :  Lin.liu
// Note   :
// Modify List:
//    lin.liu               Create.(2004-04-08)
//




//extern DIAG_CMD_ROUTINE_

//
// Description:
//   Client call this function to register its command routine.
// Author : lin.liu
//
// Input Param:
//   type    : the command type relative to the routine
//   routine : the callback function pointer.
//
// Note:
//   At one time, only one routine can be registered for this type. If other client call 
//   this function to register another callback, it will overwrite the old one.
//
void DIAG_RegisterCmdRoutine( diag_cmd_code_enum_type   type,
    DIAG_CMD_ROUTINE routine );
    
//
// Description:
//    Client call this function to unregister its command routine. 
// Author : lin.liu
//
// Param:
//   type : the command type that want to be unregister.
//
// Note:
//
void DIAG_UnRegisterCmdRoutine( diag_cmd_code_enum_type   type );

//
// Description:
//    Init the command routine table. All routine be cleared.
// Author : lin.liu
//
// Note:
//    All the routine will be cleared, and no routine be called.
// 
void DIAG_InitCmdRoutine( void );    


/*
 ************************************************
 * Added function for Shell application
 * Added by Eddie.Li
 ************************************************
 */
typedef void * HANDLE;

typedef struct DIAG_PRIMARY_CMD_S
{
    const char * primary_cmd;                            /* 一级命令行 */
    const char * help;                                    /* 命令格式   */
} DIAG_PRIMARY_CMD_T;

typedef struct DIAG_SECONDLY_CMD_S
{
    const char   *secondly_cmd;                        /*  二级命令行 */
    uint32       max_argc;                              /* 最大参数个数， 可以省略这个变量 */
    void         (*handler)(int argc, char *argv[]);    /* 命令行handler */
} DIAG_SECONDLY_CMD_T;

typedef struct DIAG_SECONDLY_CMD_HANDLER_LIST_S
{
    DIAG_SECONDLY_CMD_T                      * sec_cmd_handler;
    struct DIAG_SECONDLY_CMD_HANDLER_LIST_S * next;
    struct DIAG_SECONDLY_CMD_HANDLER_LIST_S * prev;
} DIAG_SECONDLY_CMD_HANDLER_LIST_T;

typedef struct DIAG_PRIMARY_CMD_LIST_S
{
    DIAG_PRIMARY_CMD_T                  * regester_cmd;
    DIAG_SECONDLY_CMD_HANDLER_LIST_T    * cmd_handler_header;
    struct DIAG_PRIMARY_CMD_LIST_S     * next;
    struct DIAG_PRIMARY_CMD_LIST_S     * prev;
} DIAG_PRIMARY_CMD_LIST_T;

typedef enum 
{
    SHELL_MSG_REQ     = 0,
    SHELL_MSG_RES,
    SHELL_MSG_IND
} DIAG_SHELL_MSG_TYPE_T;

typedef enum 
{
    SHELL_DATA_TEXT     = 0,
    SHELL_DATA_BINARY,
    SHELL_DATA_MEM_INFO,
    SHELL_DATA_SCRIPTS  /* Added for support Cmd Script! */
} DIAG_SHELL_DATA_TYPE_T;

typedef enum 
{
    SHELL_SUCCESS     = 0,
    SHELL_CMD_ERROR,
    SHELL_PENDING /* Added for support Cmd Script! */
} DIAG_SHELL_ERR_CODE_T;

typedef enum
{
    DIAG_PARSE_OK                          = 0,
    DIAG_PARSE_PRIMARY_CMD_NOT_FOUND,
    DIAG_PARSE_SECONDLY_CMD_NOT_FOUND,
    DIAG_PARSE_TOO_MANY_ARG,
    DIAG_PARSE_HELP_CMD
}DIAG_PARSE_RESULT_T;

typedef enum 
{
    DIAG_PARSING_IDLE             = 0,
    DIAG_PARSING_PRIMARY_CMD,    
    DIAG_PARSING_SECONDLY_CMD,   
    DIAG_PARSING_ARG, 
    DIAG_PARSING_END
}DIAG_PARSING_STATE_T;

typedef struct DIAG_CONSOLE_CMD_REQ_S
{
    MSG_HEAD_T              msg_header;
    uint32/*DIAG_SHELL_MSG_TYPE_T*/   msg_type;
    uint32                  cmd_line_len;
} DIAG_CONSOLE_CMD_REQ_T;

typedef struct DIAG_CONSOLE_CMD_RES_S
{
    MSG_HEAD_T              msg_header;
    uint32/*DIAG_SHELL_MSG_TYPE_T*/   msg_type;
    uint32/*DIAG_SHELL_ERR_CODE_T*/   err_code;
} DIAG_CONSOLE_CMD_RES_T;

typedef struct DIAG_CONSOLE_CMD_IND_S
{
    MSG_HEAD_T              msg_header;
    uint32/*DIAG_SHELL_MSG_TYPE_T*/   msg_type;
    uint32/*DIAG_SHELL_DATA_TYPE_T*/  data_type;
    uint16                  seq_num;
    uint16                  frag;                  
    uint32                  data_len;
    uint32                  status; /* Added for support Cmd Script! */
} DIAG_CONSOLE_CMD_IND_T;

HANDLE DIAG_RegisterPriCmd(DIAG_PRIMARY_CMD_T *reg_cmd);
int32  DIAG_RegisterSecCmd(HANDLE handle, DIAG_SECONDLY_CMD_T *sec_cmd_handler);
void   DIAG_UnregisterPriCmd(HANDLE handle);
void   DIAG_SendFormatInfo(const char *x_format, ...);
void   DIAG_SendInfo(DIAG_SHELL_DATA_TYPE_T type, uint8 *info, uint32 info_len);
void DIAG_SendInfoEx(DIAG_SHELL_DATA_TYPE_T type, uint8 * info, uint32 info_len,  uint32  packet_flag);
void   DIAG_SendScriptInfo(DIAG_SHELL_ERR_CODE_T status);
/* Shell function end */

void DIAG_SendInfoHead(DIAG_SHELL_DATA_TYPE_T type,  uint32 total_len);
void DIAG_SendInfoData(DIAG_SHELL_DATA_TYPE_T type, uint8 * info, uint32 info_len);
void DIAG_SendInfoEnd(DIAG_SHELL_DATA_TYPE_T type);

DIAG_PARSE_RESULT_T DIAG_ParseCmdline(const char *cmdline, uint32 cmdline_len);

/*****************************************************************************/
//  Description:    This function init the diagnostic task. 
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void InitDiag(void); 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _DIAG_H

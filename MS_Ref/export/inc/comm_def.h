/******************************************************************************
 ** File Name:      comm_def.h                                                *
 ** Author:         Richard Yang                                              *
 ** DATE:           12/07/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/07/2002     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _COMM_DEF_H
#define _COMM_DEF_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "comm_id.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// 所有消息的头的结构
#define  STRUCT_HEAD \
unsigned short SignalCode;\
unsigned short Size;

// 键盘消息
typedef uint32  KEY_CODE_T;
typedef struct
{
	STRUCT_HEAD
	KEY_CODE_T  key_code;   // 键盘扫描码
} COMM_SOFTKEY_T, *COMM_SOFTKEY_PTR;


/* MAX cell number monitored by RRC module */
#define DIAG_CELL_MAX 7

// @Xueliang.Wang add(2003-01-02)
#define DIAG_ACCESS_MCU_MAX_LENGTH  1000


typedef enum
{
	BYTE_ACCESS,
	WORD_ACCESS, 
	DWORD_ACCESS
} ADDR_ACESS_MODE_E;

typedef enum
{
    MCU_ACCESS_SUCCESS,
	MCU_ACCESS_MODE_ERR,
	MCU_ACCESS_ADDR_ERR,
	MCU_ACCESS_SIZE_ERR,
	MCU_ACCESS_RANGE_ERR
} DIAG_MCU_ACCESS_ERR_E;

typedef struct
{
    STRUCT_HEAD
	uint32  mode;
	void *  addr;   // In request packet, it is the access address
	                // In response packet, it is error code.
	uint32  access_size;    // Number of BYTE, WORD or DWORD according to mode.
}DIAG_MCU_READ_T, *DIAG_MCU_READ_PTR;

typedef struct
{
    STRUCT_HEAD
	uint32  mode;
	void *  addr;   // In request packet, it is the access address
	                // In response packet, it is error code.
	uint32  access_size;    // Number of BYTE, WORD or DWORD according to mode.
}DIAG_MCU_WRITE_T, *DIAG_MCU_WRITE_PTR;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*End comm_def.h*/


/******************************************************************************
 ** File Name:      server.h                                                  *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file includes some common message type defines for   *
 **                 other files.                                              * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/11/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _SERVICE_H
#define _SERVICE_H

#include "sci_types.h"
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define  BIT_OFFSET_MASK  (0x07)
#define  MAX_EVENT_NUM   88  
        
#define  SET_BITS_ON     1 	
#define  SET_BITS_OFF    0	
#define  SET_EVENT       SET_BITS_ON
#define  DISABLE_EVENT   SET_BITS_OFF		

#define	ARRAY_SIZE(num) ( (num + 7) / 8 )

typedef struct
{
    uint16   flags;                       // state of the client
    uint16   ref_cnt;                     // ref cnt of this client
    uint32   task_ID;                     // Client thread ID
    uint8    event_num;                   // total event number
    uint8    bit_array[MAX_EVENT_NUM / 8];// bit array to hold the evnet map
} CLIENT_INFO_T;                     

typedef struct
{
    uint16              flags;      // state of the server
    uint16              ref_cnt;    // sum of the ref cnts of all clients of this server
    uint8               event_num;  // total event number
    uint8               dummy_0;    // No used
    uint16              dummy_1;    // No used
    uint32              server_ID;  // Server thread ID
   	struct reg_info_tag *pre;       // Pre server pointer
    struct reg_info_tag *suc;       // Next server pointer
} SERVER_INFO_T;                     

typedef struct reg_info_tag
{
    union
    {
        CLIENT_INFO_T   client_info; // Client information
        SERVER_INFO_T   server_info; // Server information
    }INFO;
	struct reg_info_tag * pre;       // Pre Client information
	struct reg_info_tag * suc;       // Next Client information
    REG_CALLBACK        m_callback;  // Callback function for server to call       
} REG_INFO_T;                                 


PUBLIC void SCI_InitServer(void); // Init Server

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // End server.h
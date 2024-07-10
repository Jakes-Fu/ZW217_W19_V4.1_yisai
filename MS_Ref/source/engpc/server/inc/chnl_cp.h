/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_CP_H_
#define _CHNL_CP_H_

#include "sci_types.h"
#include "chnl.h"

PUBLIC void chnl_cp_init();

PUBLIC int cp_read_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int cp_write_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC void CPDumpCmd();
#ifdef UAL_AT_SUPPORT
PUBLIC int at_send_data_to_cp(CHNL_DATA_TYPE type, char* buff, int len, void (*rsp_callback)( char* outBuf, int outBufLen));
PUBLIC void at_remove_callback(void);
#endif
#endif
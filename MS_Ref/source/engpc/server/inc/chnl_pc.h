/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_PC_H_
#define _CHNL_PC_H_

#include "sci_types.h"
#include "chnl.h"
typedef enum{
    PC_CONNECT_COM,
    PC_CONNECT_WIFI,

    PC_CONNECT_TOTAL
}PC_CONNECT_TYPE;

void activeCurConnectType(PC_CONNECT_TYPE type);
PC_CONNECT_TYPE getCurConnectType();

PUBLIC void pc_write_empty_diag();
PUBLIC void pc_write_at_ok();
PUBLIC int pc_read_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int pc_write_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int pc_write_diag(char* buff, int len);
PUBLIC int pc_write_package(char* buff, int len);
PUBLIC void chnl_pc_init(); 

#endif
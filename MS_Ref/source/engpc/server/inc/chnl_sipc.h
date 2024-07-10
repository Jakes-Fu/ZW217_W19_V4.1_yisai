/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_SIPC_H_
#define _CHNL_SIPC_H_

#include "sci_types.h"
#include "chnl.h"

typedef enum
{
    CP_CP,
    CP_WCN,

    CP_UNKNOWN,    
}CP_TYPE;

PUBLIC int chnl_sipc_read_data(CP_TYPE cp, CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int chnl_sipc_write_data(CP_TYPE cp, CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int chnl_sipc_init();
PUBLIC void chnl_sipc_uninit();
PUBLIC BOOLEAN chnl_sipc_isModemAssert();

#endif
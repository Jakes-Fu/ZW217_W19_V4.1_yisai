/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_WCN_H_
#define _CHNL_WCN_H_

#include "sci_types.h"

PUBLIC void chnl_wcn_init();

PUBLIC int wcn_read_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int wcn_write_data(CHNL_DATA_TYPE type, char* buff, int len);

#endif
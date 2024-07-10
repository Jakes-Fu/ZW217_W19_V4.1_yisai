/******************************************************************************
 ** File Name:      modulemgr.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define modulemgr interface                                     *
 ******************************************************************************/

#ifndef _MODULE_MGR_H_
#define _MODULE_MGR_H_

#include "sci_types.h"

PUBLIC int processDiag(char *buf, int len, char *rsp, int rsp_len, int* cp_process);
PUBLIC int processAT(char *buf, int len, char *rsp, int rsp_len, int* cp_process);

#endif
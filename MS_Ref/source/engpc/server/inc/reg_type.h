/******************************************************************************
 ** File Name:      reg_type.h                                          *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.         *
 ** Description:    define reg interface                 *
 ******************************************************************************/

#ifndef _CHNL_REG_TYPE_H_
#define _CHNL_REG_TYPE_H_

#include "sprd_fts_type.h"

#define TABLE_DIAG_AT_REG_BEGIN(table) \
    const struct eng_callback table[] = {

#define DIAG_REG(type, subtype, eng_diag_func) \
    DIAG_REG_LOW(type, subtype, 0xFF, 0, eng_diag_func)

#define DIAG_REG_LOW(type, subtype, diag_ap_cmd, also_need_to_cp, eng_diag_func) \
    {diag_ap_cmd, type, subtype, "", also_need_to_cp, eng_diag_func, NULL, NULL, NULL},

#define DIAG_REG_FULL(type, subtype, diag_ap_cmd, also_need_to_cp, eng_diag_func, eng_set_writeinterface_func, eng_cmd_match) \
    {diag_ap_cmd, type, subtype, "", also_need_to_cp, eng_diag_func, NULL, eng_set_writeinterface_func, eng_cmd_match},

#define AT_REG(at_cmd, eng_linuxcmd_func) \
    {0xFF, 0xFF, 0xFF, at_cmd, 0, NULL, eng_linuxcmd_func, NULL, NULL},

#define TABLE_DIAG_AT_REG_END() \
    };


#endif
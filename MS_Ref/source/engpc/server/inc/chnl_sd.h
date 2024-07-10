/******************************************************************************
 ** File Name:      chnl.h                                                    *
 ** Author:         qingjun.yu                                                *
 ** DATE:           02/03/2020                                                *
 ** Copyright:      2003 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    define chnl interface                                     *
 ******************************************************************************/

#ifndef _CHNL_SD_H_
#define _CHNL_SD_H_

#include "sci_types.h"

enum
{
    CP_DUMP_LOG,
    CP_DUMP_MEMORY,
    CP_DUMP_MINIDUMP,
    CP_DUMP_SBLOCK,
    CP_DUMP_SBUF,
    CP_DUMP_SMSG,

    CP_DUM_TOTAL
};

PUBLIC void chnl_sd_dump2File(int type, uint8* src, int len);

#endif

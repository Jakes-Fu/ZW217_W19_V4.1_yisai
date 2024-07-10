/*
 * Copyright (C) 2016-2019 Alibaba Group Holding Limited
 */

#ifndef _LS_HAL_H_
#define _LS_HAL_H_

#include "ls_osa.h"
#include "ls_hal_km.h"
#include "ls_hal_sst.h"
#include "ls_hal_crypt.h"

#ifdef WIN32
#define LS_HAL_LOG  ls_osa_print_win
#else
#define LS_HAL_LOG(_f, _a ...)  \
        ls_osa_print("%s %d: "_f,  __FUNCTION__, __LINE__, ##_a)
#endif

#endif /* _LS_HAL_H_ */


/**
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 *
 * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 **/
/**
 *****************************************************************************
 * @file     pm_adapter.h
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date     
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#ifndef PM_ADAPTER_H
#define PM_ADAPTER_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Variables                                  *
 **---------------------------------------------------------------------------*/

//#define PM_SFS_SUPPORT

#define PM_TRACE_LOW(_format,...) \
            do \
            { \
                SCI_TRACE_LOW("{%s}[%s]<%d> "_format, "PM", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
            }while(0)

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End
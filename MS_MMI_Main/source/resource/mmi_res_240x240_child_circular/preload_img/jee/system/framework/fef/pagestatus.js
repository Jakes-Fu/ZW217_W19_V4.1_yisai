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
 * @file     pagestatus.js
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-12
 * @history
 *****************************************************************************
 **/


export const PAGE_STATUS = {
    "PAGE_STATUS_NONE": 0x00,
    "PAGE_STATUS_INIT": 0x01,
    "PAGE_STATUS_READY": 0x02,
    "PAGE_STATUS_SHOW": 0X03,
    "PAGE_STATUS_HIDE": 0x04,
    "PAGE_STATUS_DESTROY": 0x05,
    "PAGE_STATUS_UNKNOWN": 0x80
};

console.log("pagestatus.js ok");
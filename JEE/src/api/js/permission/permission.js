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
 * @file     resourceload.js
 * @brief    -
 * @author  zhigao.sun@unisoc.com
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
 **/



export function check_permission(permission_id) {

  console.log("check_permission: start :" + permission_id);

  var permission_status = js_check_permission_status(permission_id);

  console.log("permission_status = " + permission_status);

  return permission_status;

}




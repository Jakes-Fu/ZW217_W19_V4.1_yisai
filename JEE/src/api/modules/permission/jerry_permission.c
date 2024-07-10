/*
 * Copyright (c) 2015-2020 by UNISOC Technology(Shanghai), Inc.  ALL RIGHTS
 * RESERVED.
 *
 * copyrighted works and confidential proprietary information of
 * UNISOC Technology(Shanghai), Inc. They may not be modified, copied,
 * reproduced, distributed, or disclosed to third parties in any manner, medium,
 * or form, in whole or in part, without the prior written consent of UNISOC
 * Technology(Shanghai), Inc.
 */

/**
 *****************************************************************************
 * @file     jerry_permission.c
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
#include "jerry_permission.h"
#include "jerry_bindings.h"
#include "os_api.h"
#include "permission_check.h"
//#include "pm_list.h"


DECLARE_HANDLER(js_check_permission_status) {
  SCI_TRACE_LOW("js_check_permission_status..start ");
  if (JERRY_UNLIKELY(args_cnt < 1)) {
    return jerry_create_number(0);
  }

  int permission_id = jerry_get_number_value(args_p[0]);

  SCI_TRACE_LOW("jerry_get_number_value(args_p[0]) permission_id = %d\n",
                permission_id);
  int32 result = 0;

  SCI_TRACE_LOW("perm_check_permission_status(permission_id..start");
  result = perm_check_permission_status(permission_id);
  SCI_TRACE_LOW("result = %d\n", result);
  SCI_TRACE_LOW("perm_check_permission_status(permission_id..end");
  return jerry_create_number(result);
}

void js_register_permission_functions() {
  REGISTER_HANDLER(js_check_permission_status);
};

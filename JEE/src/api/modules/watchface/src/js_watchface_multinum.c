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
  * @file     js_watchface_multinum.c
  * @brief    -
  * @author   qingjun.yu@unisoc.com
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

#include "jerry_bindings.h"
#include "js_watchface_multinum.h"
#include "ctrlwf_export.h"

#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "caf.h"
#include "ctrlbutton_export.h"
#include "guianim.h"
#include "ctrlbase.h"
#include "urjs_art.h"

#include "mmi_image.h"



#include "watchface_ctrl_adapter.h"


DECLARE_HANDLER(js_watchface_multinum_ctl_create) {
    if (JERRY_UNLIKELY(args_cnt < 7)) {
        return jerry_create_number(0);
    }

    MMI_WIN_ID_T win_id = jerry_get_number_value(args_p[0]);

    double center_position_x = jerry_get_number_value(args_p[1]);
    double center_position_y = jerry_get_number_value(args_p[2]);
    int data_type = jerry_get_number_value(args_p[3]);
    char** pStrArray = NULL;
    uint32_t length = 0;
    if (jerry_value_is_array(args_p[4])) {
        length = jerry_get_array_length(args_p[4]);
        pStrArray = art_malloc((length + 1) * sizeof(char*));
        memset(pStrArray, 0, length + 1);
        for (uint32_t i = 0; i < length; i++) {
            char str_buf[256] = { 0 };
            jerry_value_t item_val = jerry_get_property_by_index(args_p[4], i);
            if (jerry_value_is_string(item_val)) {
                jerry_size_t str_size = jerry_get_utf8_string_size(item_val);
                jerry_size_t string_end = jerry_string_to_utf8_char_buffer(item_val, str_buf, str_size);
                str_buf[string_end] = 0;
                char path[MAX_PATH] = {0};
                if (-1 == js_jee_get_native_path(str_buf, path, sizeof(path) - 1))
                {
                    aloge("invalidate path: %s", str_buf);
                    pStrArray[i] = NULL;
                }else{
                    pStrArray[i] = art_strdup(path);
                    JS_DEBUG_LOG("IMG[%d]:%s \n", i, pStrArray[i]);
                }
            }
            jerry_release_value(item_val);
        }
    }



    jerry_string_t img_path = JERRY_GET_ARG(5, string);

    float scale = jerry_get_number_value(args_p[6]);

    WATCHFACE_RATIO_POSITION_T wrp = { center_position_x, center_position_y };
    int32 result = 0;
    WATCHFACE_CTRL_PARAM_T     ctrl_param = {0};

    ctrl_param.center_position.ratio_x = center_position_x;
    ctrl_param.center_position.ratio_y = center_position_y;
    ctrl_param.scale = scale;
    SCI_MEMCPY(ctrl_param.p_image_list, pStrArray, length*sizeof(pStrArray));
    ctrl_param.p_image_none = jerry_string_data(&img_path);
    ctrl_param.type = WATCHFACE_TYPE_MULTI_NUM_STEPS+data_type;
    result = WATCHFACE_MultiNum_Create(win_id, &ctrl_param);

    for (uint32_t i = 0; i < length; i++) {
        if(pStrArray[i] != NULL){
          art_free(pStrArray[i]);
        }
    }
    art_free(pStrArray);
    pStrArray = NULL;
    jerry_string_destroy(&img_path);
    return jerry_create_number(result);
}



DECLARE_HANDLER(js_watchface_multinum_ctl_destroy) {
    if (JERRY_UNLIKELY(args_cnt < 1)) {
        return jerry_create_number(0);
    }
    int handle = jerry_get_number_value(args_p[0]);
    //int32 result = pal_watchface_day_ctl_destroy(handle);
    int32 result = WATCHFACE_MultiNum_Destory(handle);

    return jerry_create_number(result);
}



void js_register_watchface_multinum_functions() {

    REGISTER_HANDLER(js_watchface_multinum_ctl_create);
    REGISTER_HANDLER(js_watchface_multinum_ctl_destroy);

};

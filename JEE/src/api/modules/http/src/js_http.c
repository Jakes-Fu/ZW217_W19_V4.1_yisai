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
  * @file     js_http.c
  * @brief    -
  * @author   qi.zhang
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
#include "sci_types.h"
#include "ual_cms_export.h"
#include "native_http.h"
#include "urjs_art.h"
#include "permission_check.h"

jerry_value_t parseHttpSuccessParam(jerry_value_t object, http_callback_param_t param)
{
    jerry_value_t prop_value = jerry_create_string("Success");
    jerry_value_t prop_name = jerry_create_string((const jerry_char_t *)"result");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    if(PNULL == param.p_body || param.body_size == 0)
    {
        return jerry_create_undefined();
    }
    char* p_buffer = jerry_buffer_allocate(param.body_size + 1);
    if (PNULL == p_buffer)
    {
        return jerry_create_undefined();
    }
    memset(p_buffer, 0 ,param.body_size + 1);
    memcpy(p_buffer, param.p_body, param.body_size);

    jerry_string_t contents = jerry_string_create_with_buffer(p_buffer, param.body_size);
    prop_value = jerry_jval_create_string(&contents);
    prop_name = jerry_create_string((const jerry_char_t *)"body");
    jerry_string_destroy(&contents);
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_name);
    jerry_release_value(prop_value);
    return object;
}
jerry_value_t parseHttpErrorParam(jerry_value_t object, http_callback_param_t param)
{
    jerry_value_t prop_value = jerry_create_string("Failed");
    jerry_value_t prop_name = jerry_create_string((const jerry_char_t *)"result");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);
    prop_value = jerry_create_number((uint32)param.p_body);
    JS_DEBUG_LOG("http request errcode= %d",(uint32)param.p_body);
    prop_name = jerry_create_string((const jerry_char_t *)"errcode");
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
}

void http_error_response_call(http_callback_param_t param)
{
    jerry_value_t cb_name = jerry_create_string((jerry_char_t *)"ResponseFun");
    jerry_value_t cb_func = jerry_get_property((jerry_value_t)param.handle, cb_name);
    jerry_value_t obj_param = parseHttpErrorParam((jerry_value_t)param.handle, param);
    jerry_value_t call_func = jerry_call_function(cb_func, param.handle, &obj_param, 1);
    /* Check the call function return value if there is any error */
    if (TRUE == jerry_value_is_error(call_func))
    {
        aloge("http_response_call call function error is %d\n",
              jerry_get_error_type(call_func));
    }
    jerry_release_value(call_func);
    jerry_release_value(cb_name);
    jerry_release_value(cb_func);
}
void http_success_response_call(http_callback_param_t param)
{
    jerry_value_t cb_name = jerry_create_string((jerry_char_t *)"ResponseFun");
    jerry_value_t cb_func = jerry_get_property((jerry_value_t)param.handle, cb_name);
    jerry_value_t obj_param = parseHttpSuccessParam((jerry_value_t)param.handle, param);
    jerry_value_t call_func = jerry_call_function(cb_func, param.handle, &obj_param, 1);
    /* Check the call function return value if there is any error */
    if (TRUE == jerry_value_is_error(call_func))
    {
        aloge("http_response_call call function error is %d\n",
              jerry_get_error_type(call_func));
    }
    jerry_release_value(call_func);
    jerry_release_value(cb_name);
    jerry_release_value(cb_func);
}

boolean http_callback(http_callback_param_t param)
{
    if(param.result == HTTP_REQUEST_SUCESS)
    {
        if(PNULL != param.p_body)
        {
            http_success_response_call(param);
        }
    }
    else
    {
        if(PNULL != param.p_body)
        {
            http_error_response_call(param);
        }
    }
    return jerry_create_null();
}
/*****************************************************************************/
//  Description : js_http_post_request_start
//  Parameter: [In] string:url
//             [In] string:content type
//             [In] string:post body
//             [In] object:http_request(handle)
//             [Return] boolean
//  Author: qi.zhang
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_http_post_request_start)
{
    JERRY_CHECK_ARGS(4, string, string, string, object);
    http_request_result_e req_res = HTTP_REQUEST_RESULT_MAX;
    boolean               result = TRUE;
    jerry_string_t js_http_url = JERRY_GET_ARG(0, string);
    jerry_string_t js_content_type = JERRY_GET_ARG(1, string);
    jerry_string_t js_extra_data = JERRY_GET_ARG(2, string);
    jerry_value_t  obj = JERRY_GET_ARG(3, object);
    grant_permission_status_e permission_status = PREMISSION_STATUS_NONE;
    permission_status = perm_check_permission_status(PERM_INTERNET);
    if ((permission_status == PREMISSION_STATUS_DENIED)
        || (permission_status == PREMISSION_STATUS_NONE))
    {
        JS_DEBUG_LOG("perm_check_permission_status = NULL!\n");
        return jerry_create_null();
    }
    JS_DEBUG_LOG("perm_check_permission_status = ALLOW!\n");
#ifdef JS_COMPENT_HTTP //for 217 ram cutdown
    http_set_content_type(jerry_string_data(&js_content_type));
    req_res = http_start_request(obj, jerry_string_data(&js_http_url), jerry_string_data(&js_extra_data), HTTP_POST_REQUEST, http_callback);
#endif
    if(HTTP_REQUEST_SUCESS != req_res)
    {
        JS_DEBUG_LOG("http get request failed\n");
        result = FALSE;
    }
    return jerry_create_number(result);
}
/*****************************************************************************/
//  Description : js_http_get_request_start
//  Parameter: [In] string:url
//             [In] object:http_request(handle)
//             [Return] boolean
//  Author: qi.zhang
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_http_get_request_start)
{
    JERRY_CHECK_ARGS(2, string, object);
    http_request_result_e req_res = HTTP_REQUEST_RESULT_MAX;
    boolean               result = TRUE;
    http_request_type_e  type = HTTP_GET_REQUEST;
    jerry_string_t js_http_url = JERRY_GET_ARG(0, string);
    jerry_value_t  obj = JERRY_GET_ARG(1, object);
    grant_permission_status_e permission_status = PREMISSION_STATUS_NONE;
    permission_status = perm_check_permission_status(PERM_INTERNET);
    if ((permission_status == PREMISSION_STATUS_DENIED)
        || (permission_status == PREMISSION_STATUS_NONE))
    {
        JS_DEBUG_LOG("perm_check_permission_status = NULL!\n");
        return jerry_create_null();
    }
    JS_DEBUG_LOG("perm_check_permission_status = ALLOW!\n");
#ifdef JS_COMPENT_HTTP //for 217 ram cutdown
    req_res = http_start_request(obj, jerry_string_data(&js_http_url), PNULL, HTTP_GET_REQUEST, http_callback);
#endif
    if(HTTP_REQUEST_SUCESS != req_res)
    {
        JS_DEBUG_LOG("http get request failed\n");
        result = FALSE;
    }
    return jerry_create_number(result);
}
/*****************************************************************************/
//  Description : js_http_options_request_start
//  Parameter: [In] string:url
//             [In] object:http_request(handle)
//             [Return] boolean
//  Author: qi.zhang
/*****************************************************************************/
DECLARE_PUBLIC_HANDLER(js_http_options_request_start)
{
    JERRY_CHECK_ARGS(2, string, object);
    http_request_result_e req_res = HTTP_REQUEST_RESULT_MAX;
    boolean               result = TRUE;
    http_request_type_e  type = HTTP_GET_REQUEST;
    jerry_string_t js_http_url = JERRY_GET_ARG(0, string);
    jerry_value_t  obj = JERRY_GET_ARG(1, object);
    grant_permission_status_e permission_status = PREMISSION_STATUS_NONE;
    permission_status = perm_check_permission_status(PERM_INTERNET);
    if ((permission_status == PREMISSION_STATUS_DENIED)
        || (permission_status == PREMISSION_STATUS_NONE))
    {
        JS_DEBUG_LOG("perm_check_permission_status = NULL!\n");
        return jerry_create_null();
    }
    JS_DEBUG_LOG("perm_check_permission_status = ALLOW!\n");
#ifdef JS_COMPENT_HTTP //for 217 ram cutdown
    req_res = http_start_request(obj, jerry_string_data(&js_http_url), PNULL, HTTP_OPTIONS_REQUEST, http_callback);
#endif
    if(HTTP_REQUEST_SUCESS != req_res)
    {
        JS_DEBUG_LOG("http get request failed\n");
        result = FALSE;
    }
    return jerry_create_number(result);
}

void js_register_http_functions() {
    REGISTER_HANDLER(js_http_post_request_start);
    REGISTER_HANDLER(js_http_get_request_start);
    REGISTER_HANDLER(js_http_options_request_start);
};


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
 * @file     js_div.c
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/

/**---------------------------------------------------------------------------*

 * **                         Dependencies *

 * **---------------------------------------------------------------------------*/

/*lint -save -e746 -e718 -e716 -e628 -e578 -e516 -e515 */
#include <string.h>

#include "os_api.h"

#include "js_div.h"

#include "urjs_art.h"
// realization
#include "form.h"

/**---------------------------------------------------------------------------*
 * **                         Compiler Flag *
 * **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 * **                         Global Variables *
 * **---------------------------------------------------------------------------*/
#define PROP_FLEX_DIRECTION "flex-direction"
#define PROP_FLEX_WRAP "flex-wrap"
#define PROP_JUSTIFY_CONTENT "justify-content"
#define PROP_ALIGN_ITEMS "align-items"

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

/**---------------------------------------------------------------------------*
 * **                         Local Variables *
 * **---------------------------------------------------------------------------*/

// flex-direction support param: row,column;
LOCAL const char *s_value_flex_direction[] = {"row", "column"};

// flex-wrap support param: wrap,nowrap;
LOCAL const char *s_value_flex_wrap[] = {"wrap", "nowrap"};

// justify-content support param: flex-start,flex-end,center;
LOCAL const char *s_value_justify_content[] = {"flex-start", "flex-end",
                                               "center"};

// align-items support param: stretch,flex-start,flex-end,center;
LOCAL const char *s_value_align_items[] = {"stretch", "flex-start", "flex-end",
                                           "center"};
/**---------------------------------------------------------------------------*
 * **                         Local Functions *
 * **---------------------------------------------------------------------------*/

LOCAL int get_prop_value_index(const char **s_values, int s_values_len,
                               const char *prop_value)
{
    char *value = NULL;
    int index = -1;

    for (int i = 0; i < s_values_len; i++)
    {
        value = (char *)s_values[i];
        if (strncmp(prop_value, value, strlen(prop_value)) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

LOCAL char *get_prop_value_string(jerry_value_t object, const char *_prop_name)
{
    /*lint -e64*/
    jerry_value_t prop_name =
        jerry_create_string((jerry_char_t *)_prop_name); /*lint !e64 */
    jerry_value_t value = jerry_get_property((jerry_value_t)object, prop_name);
    jerry_release_value(prop_name);

    jerry_size_t str_size = 0;
    char *value_str = NULL;

    str_size = jerry_get_string_size(value) + 1;
    JERRY_VLA(jerry_char_t, _value_str, str_size);
    str_size = jerry_string_to_char_buffer(value, _value_str,
                                           str_size); /*lint !e119 */
    if (str_size == 0)
    {
        return NULL;
    }
    _value_str[str_size] = '\0';
    value_str = (char *)art_malloc(str_size + 1);
    if (value_str == NULL)
    {
        return NULL;
    }
    memcpy(value_str, _value_str, (size_t)(str_size + 1));
    // value_str = _value_str;

    jerry_release_value(value);
    jerry_release_value(_value_str); /*lint !e64 */

    return value_str;
}

LOCAL BOOLEAN js_div_prop_value_change(uint32 form_ctrl_handle,
                                       const char *prop_name,
                                       const char *prop_value)
{
    if (NULL == prop_name || NULL == prop_value)
    {
        return FALSE;
    }

    int prop_value_index = -1;
    if (strncmp(PROP_FLEX_DIRECTION, prop_name, strlen(prop_name)) == 0)
    {
        prop_value_index = get_prop_value_index(
            s_value_flex_direction, ARRAY_LENGTH(s_value_flex_direction),
            prop_value);
        if (prop_value_index != -1)
        {
            js_native_form_set_flex_direction(form_ctrl_handle,
                                              prop_value_index);
        }
    }
    else if (strncmp(PROP_FLEX_WRAP, prop_name, strlen(prop_name)) == 0)
    {
        prop_value_index = get_prop_value_index(
            s_value_flex_wrap, ARRAY_LENGTH(s_value_flex_wrap), prop_value);
        if (prop_value_index != -1)
        {
            js_native_form_set_flex_wrap(form_ctrl_handle, prop_value_index);
        }
    }
    else if (strncmp(PROP_JUSTIFY_CONTENT, prop_name, strlen(prop_name)) == 0)
    {
        prop_value_index = get_prop_value_index(
            s_value_justify_content, ARRAY_LENGTH(s_value_justify_content),
            prop_value);
        if (prop_value_index != -1)
        {
            js_native_form_set_justify_content(form_ctrl_handle,
                                               prop_value_index);
        }
    }
    else if (strncmp(PROP_ALIGN_ITEMS, prop_name, strlen(prop_name)) == 0)
    {
        prop_value_index = get_prop_value_index(
            s_value_align_items, ARRAY_LENGTH(s_value_align_items), prop_value);
        if (prop_value_index != -1)
        {
            js_native_form_set_align_items(form_ctrl_handle, prop_value_index);
        }
    }
    else
    {
        alogi("Unknow prop_name =%s", prop_name);
        return FALSE;
    }
    if (prop_value_index == -1)
    {
        alogi("Wrong prop_value_indexd prop_value=%s ", prop_value);
        return FALSE;
    }
    return TRUE;
}

/**---------------------------------------------------------------------------*
 * **                         External Functions *
 * **---------------------------------------------------------------------------*/

DECLARE_PUBLIC_HANDLER(js_div_create)
{
    JERRY_CHECK_ARGS(2, object, number); /*lint !e64 */
    int handle = 0;
    jerry_value_t obj = JERRY_GET_ARG(0, object);
    int page_handle = (int)jerry_get_number_value(args_p[1]);

    handle = js_native_form_create(obj, page_handle);
    return jerry_create_number((double)handle);
}

DECLARE_PUBLIC_HANDLER(js_div_delete)
{
    JERRY_CHECK_ARGS(1, number); /*lint !e64 */
    int div_handle = (int)jerry_get_number_value(args_p[0]);

    js_native_form_destory(div_handle);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_div_addchild)
{
    JERRY_CHECK_ARGS(3, number, number, number); /*lint !e64 */
    int page_handle = (int)jerry_get_number_value(args_p[0]);
    int div_handle = (int)jerry_get_number_value(args_p[1]);
    int child_handle = (int)jerry_get_number_value(args_p[2]);

    js_native_form_add_child(page_handle, div_handle, child_handle);

    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_div_removechild)
{
    JERRY_CHECK_ARGS(2, number, number); /*lint !e64 */
    int div_handle = (int)jerry_get_number_value(args_p[0]);
    int child_handle = (int)jerry_get_number_value(args_p[1]);

    js_native_form_remove_child(div_handle, child_handle);
    return jerry_create_null();
}

DECLARE_PUBLIC_HANDLER(js_div_set_property)
{
    JERRY_CHECK_ARGS(3, number, string, string); /*lint !e64 */
    int form_handle = (int)jerry_get_number_value(args_p[0]);
    jerry_string_t _prop_name = JERRY_GET_ARG(1, string);
    jerry_string_t _prop_value = JERRY_GET_ARG(2, string);

    const char *prop_name = jerry_string_data(&_prop_name);
    // prop_value malloc in get_prop_value_string
    char *prop_value = jerry_string_data(&_prop_value);

    bool ret = js_div_prop_value_change(form_handle, prop_name, prop_value);

    jerry_string_destroy(&_prop_name);
    jerry_string_destroy(&_prop_value);
    return jerry_create_boolean(ret);
}

void js_register_gui_div_functions()
{
    REGISTER_HANDLER(js_div_create);
    REGISTER_HANDLER(js_div_delete);
    REGISTER_HANDLER(js_div_addchild);
    REGISTER_HANDLER(js_div_removechild);
    REGISTER_HANDLER(js_div_set_property);
}

/**---------------------------------------------------------------------------*
 * **                         Compiler Flag *
 * **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*lint -restore */
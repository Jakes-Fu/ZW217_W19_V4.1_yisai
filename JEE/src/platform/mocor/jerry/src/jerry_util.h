/* Copyright 2015-present Samsung Electronics Co., Ltd. and other contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JERRY_UTIL_H
#define JERRY_UTIL_H

#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerry_string.h"
#include "jerry_magic_strings.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// Return value should be released with jerry_string_destroy()
// jerry_string_t jerry_file_read(const char* path);

char* jerry_buffer_allocate(size_t size);
char* jerry_buffer_allocate_from_number_array(size_t size,
                                              const jerry_value_t array);
char* jerry_buffer_reallocate(char* buffer, size_t size);
void jerry_buffer_release(char* buff);

void print_stacktrace(void);

/* Constructors */
jerry_value_t jerry_jval_create_string(const jerry_string_t* v);
jerry_value_t jerry_jval_create_byte_array(uint32_t len, const char* data);
jerry_value_t jerry_jval_create_function(jerry_external_handler_t handler);
jerry_value_t jerry_jval_create_error_without_error_flag(const char* msg);

/* Type Converters */
bool jerry_jval_as_boolean(jerry_value_t);
double jerry_jval_as_number(jerry_value_t);
char *js_value_to_string(const jerry_value_t value);
void js_add_function(const jerry_value_t obj, const char *name,
                     jerry_external_handler_t func);
bool jerry_str_is_utf8(const void* str, int size);

jerry_string_t jerry_jval_as_string(jerry_value_t);
jerry_value_t jerry_jval_as_object(jerry_value_t);
jerry_value_t jerry_jval_as_array(jerry_value_t);
jerry_value_t jerry_jval_as_function(jerry_value_t);
bool jerry_buffer_as_string(jerry_value_t jval, jerry_string_t* out_string);

jerry_string_t jerry_file_read_by_path(const char* path);
const uint8_t* js_read_file(const char* file_name, size_t* out_size_p);

jerry_value_t jerry_jval_get_property(jerry_value_t jobj, const char* name);
void jerry_jval_set_property_number(jerry_value_t jobj, const char* name,
                                    double v);
void jerry_jval_set_property_jval(jerry_value_t jobj, const char* name,
                                  jerry_value_t value);
void jerry_jval_set_property_string_raw(jerry_value_t jobj, const char* name,
                                        const char* v);
void jerry_jval_set_method(jerry_value_t jobj, const char* name,
                           jerry_external_handler_t handler);

void js_call_global_obj_function_string(const char* obj_name, const char* func_name, const char* str);

double jerry_port_get_current_time (void);

void *unisoc_malloc(size_t size);
void *unisoc_calloc(size_t count, size_t size);
void *unisoc_memset(void *s, int c, size_t n);
void unisoc_free(void *mem);
char *unisoc_strdup(const char *s);

/* Note:
 *      Defines started with underscores should not be used
 *      outside of this header.
 */
#define JERRY_CREATE_ERROR(TYPE, message) \
  jerry_create_error(JERRY_ERROR_##TYPE, (const jerry_char_t*)message)


#ifndef JERRY_ASSERT
#define JERRY_ASSERT(x) ((void)(x))
#endif

#define JERRY_ALLOC(type) /* Allocate (type)-sized, (type*)-typed memory */ \
  (type*)jerry_buffer_allocate(sizeof(type))

#define JERRY_CALLOC(num, type) \
  (type*)jerry_buffer_allocate((num * sizeof(type)))

#define JERRY_RELEASE(ptr) /* Release memory allocated by JERRY_ALLOC() */ \
  do {                                                                     \
    jerry_buffer_release((char*)ptr);                                      \
    ptr = NULL;                                                            \
  } while (0)

#define JERRY_DEFINE_NATIVE_HANDLE_INFO_THIS_MODULE(name)                  \
  static void jerry_##name##_destroy(jerry_##name##_t* wrap);              \
  static const jerry_object_native_info_t this_module_native_info = {      \
    .free_cb = (jerry_object_native_free_callback_t)jerry_##name##_destroy \
  }

#define JS_ERROR_LOG(...) jerry_port_log (JERRY_LOG_LEVEL_ERROR, __VA_ARGS__)
#define JS_WARNING_LOG(...) jerry_port_log (JERRY_LOG_LEVEL_WARNING, __VA_ARGS__)
#define JS_DEBUG_LOG(...) jerry_port_log (JERRY_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define JS_TRACE_LOG(...) jerry_port_log (JERRY_LOG_LEVEL_TRACE, __VA_ARGS__)

// #define JS_ERROR_LOG(fmt, ...)   SCI_TraceLow("[JS_LOG]" fmt, ##__VA_ARGS__)
// #define JS_WARNING_LOG(fmt, ...)   SCI_TraceLow("[JS_LOG]" fmt, ##__VA_ARGS__)
// #define JS_DEBUG_LOG(fmt, ...)   SCI_TraceLow("[JS_LOG]" fmt, ##__VA_ARGS__)
// #define JS_TRACE_LOG(fmt, ...)   SCI_TraceLow("[JS_LOG]" fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* JERRY_UTIL_H */

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


#include "jerry_util.h"
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#if defined(__linux__) && !defined(__OPENWRT__)
#include <execinfo.h>
#endif

void force_terminate(void);

bool jerry_jval_as_boolean(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_boolean(jval));
  return jerry_get_boolean_value(jval);
}


double jerry_jval_as_number(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_number(jval));
  return jerry_get_number_value(jval);
}

jerry_string_t jerry_jval_as_string(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_string(jval));

  jerry_size_t size = jerry_get_utf8_string_size(jval);

  if (size == 0)
    return jerry_string_create();

  char* buffer = jerry_buffer_allocate(size + 1);
  jerry_char_t* jerry_buffer = (jerry_char_t*)(buffer);

  size_t check = jerry_string_to_utf8_char_buffer(jval, jerry_buffer, size);

  JERRY_ASSERT(check == size);
  buffer[size] = '\0';

  jerry_string_t res = jerry_string_create_with_buffer(buffer, size);

  return res;
}


jerry_value_t jerry_jval_as_object(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_object(jval));
  return jval;
}


jerry_value_t jerry_jval_as_array(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_array(jval));
  return jval;
}


jerry_value_t jerry_jval_as_function(jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_function(jval));
  return jval;
}



jerry_value_t jerry_jval_create_string(const jerry_string_t* v) {
  jerry_value_t jval;

  const jerry_char_t* data = (const jerry_char_t*)(jerry_string_data(v));
  jerry_size_t size = jerry_string_size(v);

  if (jerry_is_valid_utf8_string(data, size)) {
    jval = jerry_create_string_sz_from_utf8(data, size);
  } else {
    jval = JERRY_CREATE_ERROR(TYPE, "Invalid UTF-8 string");
  }

  return jval;
}


jerry_value_t jerry_jval_create_byte_array(uint32_t len, const char* data) {
  if (data == NULL) {
    len = 0;
  }

  jerry_value_t jval = jerry_create_array(len);

  for (uint32_t i = 0; i < len; i++) {
    jerry_value_t val = jerry_create_number((double)data[i]);
    jerry_set_property_by_index(jval, i, val);
    jerry_release_value(val);
  }

  return jval;
}


jerry_value_t jerry_jval_create_function(jerry_external_handler_t handler) {
  jerry_value_t jval = jerry_create_external_function(handler);

  JERRY_ASSERT(jerry_value_is_constructor(jval));

  return jval;
}


jerry_value_t jerry_jval_create_error_without_error_flag(const char* msg) {
  jerry_value_t jval =
      jerry_create_error(JERRY_ERROR_COMMON, (const jerry_char_t*)(msg));
  return jerry_get_value_from_error(jval, true);
}


jerry_value_t jerry_jval_get_property(jerry_value_t jobj, const char* name) {
  JERRY_ASSERT(jerry_value_is_object(jobj));

  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)(name));
  jerry_value_t res = jerry_get_property(jobj, prop_name);
  jerry_release_value(prop_name);

  if (jerry_value_is_error(res)) {
    jerry_release_value(res);
    return jerry_create_undefined();
  }

  return res;
}

void jerry_jval_set_property_number(jerry_value_t jobj, const char* name,
                                    double v) {
  jerry_value_t jval = jerry_create_number(v);
  jerry_jval_set_property_jval(jobj, name, jval);
  jerry_release_value(jval);
}

void jerry_val_set_property_by_index(jerry_value_t jarr, uint32_t idx,
                                      jerry_value_t jval) {
  JERRY_ASSERT(jerry_value_is_object(jarr));

  jerry_value_t ret_val = jerry_set_property_by_index(jarr, idx, jval);
  JERRY_ASSERT(!jerry_value_is_error(ret_val));
  jerry_release_value(ret_val);
}


jerry_value_t jerry_jval_get_property_by_index(jerry_value_t jarr,
                                               uint32_t idx) {
  JERRY_ASSERT(jerry_value_is_object(jarr));

  jerry_value_t res = jerry_get_property_by_index(jarr, idx);

  if (jerry_value_is_error(res)) {
    jerry_release_value(res);
    return jerry_create_undefined();
  }

  return res;
}

void jerry_jval_set_method(jerry_value_t jobj, const char* name,
                           jerry_external_handler_t handler) {
  JERRY_ASSERT(jerry_value_is_object(jobj));

  jerry_value_t jfunc = jerry_create_external_function(handler);
  jerry_jval_set_property_jval(jobj, name, jfunc);
  jerry_release_value(jfunc);
}


void jerry_jval_set_property_jval(jerry_value_t jobj, const char* name,
                                  jerry_value_t value) {
  JERRY_ASSERT(jerry_value_is_object(jobj));

  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)(name));
  jerry_value_t ret_val = jerry_set_property(jobj, prop_name, value);
  jerry_release_value(prop_name);

  JERRY_ASSERT(!jerry_value_is_error(ret_val));
  jerry_release_value(ret_val);
}

void jerry_jval_set_property_string_raw(jerry_value_t jobj, const char* name,
                                        const char* v) {
  jerry_value_t jval = jerry_create_string((const jerry_char_t*)v);
  jerry_jval_set_property_jval(jobj, name, jval);
  jerry_release_value(jval);
}

jerry_string_t jerry_file_read_by_path(const char* path) {
  uint8_t* buffer = NULL;
  int len = 0;

  buffer = js_read_file(path, &len);

  if (NULL == buffer) {
    jerry_string_t empty_content = jerry_string_create();
    return empty_content;
  }

  //JS_DEBUG_LOG("jerry_file_read_by_path len(%d).", len);
  jerry_string_t contents = jerry_string_create_with_buffer(buffer, len);

  return contents;
}

const uint8_t* js_read_file(const char* file_name, /**< source code */
        size_t* out_size_p) /**< [out] number of bytes successfully read from source */
{
    uint8_t* src = NULL;
    size_t bytes_read = 0;
    int file_size = 0;

    int fd = art_open_js_file(file_name, (unsigned int)(SFS_MODE_READ | SFS_MODE_OPEN_EXISTING));
    if (fd > 0)
    {
        file_size = art_get_js_file_size(fd);
        if (file_size > 0)
        {
            src = art_malloc(file_size + 1);
            if (src)
            {
                memset(src, 0, file_size + 1);
                bytes_read = art_read_js_file(fd, src, file_size);
            }
            art_close_js_file(fd);
            *out_size_p = bytes_read;
        }
		else
		{
            art_close_js_file(fd);
		}
    }
    return (const uint8_t*)src;
} /* read_file */

char* jerry_buffer_allocate(size_t size) {
  char* buffer = (char*)(art_calloc(size, sizeof(char)));
	
  if (buffer == NULL) {
    JS_DEBUG_LOG("Out of memory");
    force_terminate();
  }
  return buffer;
}


char* jerry_buffer_allocate_from_number_array(size_t size,
                                              const jerry_value_t array) {
  char* buffer = jerry_buffer_allocate(size);
  for (size_t i = 0; i < size; i++) {
    jerry_value_t jdata = jerry_jval_get_property_by_index(array, i);
    buffer[i] = jerry_jval_as_number(jdata);
    jerry_release_value(jdata);
  }
  return buffer;
}


char* jerry_buffer_reallocate(char* buffer, size_t size) {
  JERRY_ASSERT(buffer != NULL);
  char* newbuffer = (char*)(realloc(buffer, size));
  if (newbuffer == NULL) {
    JS_DEBUG_LOG("Out of memmory");
    force_terminate();
  }
  return newbuffer;
}


void jerry_buffer_release(char* buffer) {
  if (buffer) {
	  art_free(buffer);
  }
}

void print_stacktrace(void) {
// #if !defined(NDEBUG) && defined(__linux__) && defined(DEBUG) && \
//     !defined(__OPENWRT__)
//   // TODO: support other platforms
//   const int numOfStackTrace = 100;
//   void* buffer[numOfStackTrace];
//   char command[256];

//   int nptrs = backtrace(buffer, numOfStackTrace);
//   char** strings = backtrace_symbols(buffer, nptrs);

//   if (strings == NULL) {
//     perror("backtrace_symbols");
//     exit(EXIT_FAILURE);
//   }

//   JS_DEBUG_LOG("\n[Backtrace]:\n");
//   for (int j = 0; j < nptrs - 2; j++) { // remove the last two
//     int idx = 0;
//     while (strings[j][idx] != '\0') {
//       if (strings[j][idx] == '(') {
//         break;
//       }
//       idx++;
//     }
//     snprintf(command, sizeof(command), "addr2line %p -e %.*s", buffer[j], idx,
//              strings[j]);

//     if (system(command)) {
//       break;
//     }
//   }

//   free(strings);
// #endif // !defined(NDEBUG) && defined(__linux__) && defined(DEBUG) &&
//        // !defined(__OPENWRT__)
}

char *js_value_to_string(const jerry_value_t value)
{
    int len;
    char *str;

    len = jerry_get_utf8_string_size(value);

    str = (char*)art_malloc(len + 1);
    if (str)
    {
        jerry_string_to_char_buffer(value, (jerry_char_t*)str, len);
        str[len] = '\0';
    }

    return str;
}

void js_add_function(const jerry_value_t obj, const char *name,
                     jerry_external_handler_t func)
{
    jerry_value_t str = jerry_create_string((const jerry_char_t *)name);
    jerry_value_t jfunc = jerry_create_external_function(func);

    jerry_set_property(obj, str, jfunc);

    jerry_release_value(str);
    jerry_release_value(jfunc);
}

bool jerry_str_is_utf8(const void* str, int size)
{
    bool ret = true;
    unsigned char* start = (unsigned char*)str;
    unsigned char* end = (unsigned char*)str + size;

    while (start < end)
    {
        if (*start < 0x80)              // (10000000): ASCII < 0x80
        {
            start++;
        }
        else if (*start < (0xC0))       // (11000000): 0x80 <= invalid ASCII < 0xC0
        {
            ret = false;
            break;
        }
        else if (*start < (0xE0))       // (11100000): two bytes UTF8
        {
            if (start >= end - 1)
            {
                break;
            }

            if ((start[1] & (0xC0)) != 0x80)
            {
                ret = false;
                break;
            }

            start += 2;
        }
        else if (*start < (0xF0))       // (11110000): three bytes UTF-8
        {
            if (start >= end - 2)
            {
                break;
            }

            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                ret = false;
                break;
            }

            start += 3;
        }
        else
        {
            ret = false;
            break;
        }
    }

    return ret;
}

void *unisoc_malloc(size_t size) {
  return art_malloc(size);
}

void *unisoc_calloc(size_t count, size_t size) {
 	return art_calloc(count, size);
}

void *unisoc_memset(void *s, int c, size_t n) {
  return memset(s,c,n);
}

void unisoc_free(void *mem) {
  if (NULL != mem) {
	  art_free(mem);
  }
}

char *unisoc_strdup(const char *s)
{
    return art_strdup(s);
}

void force_terminate(void) {
	abort();
  exit(EXIT_FAILURE);
}

void js_call_global_obj_function_string(const char* obj_name, const char* func_name, const char* str) {
    jerry_value_t global_object = jerry_get_global_object();

    jerry_value_t _obj_name = jerry_create_string((jerry_char_t*)obj_name);
    jerry_value_t _obj_obj = jerry_get_property((jerry_value_t)global_object, _obj_name);

    jerry_value_t _func_name = jerry_create_string((jerry_char_t*)func_name);
    jerry_value_t _func_obj = jerry_get_property((jerry_value_t)_obj_obj, _func_name);

    jerry_value_t param = jerry_create_string((jerry_char_t*)str);
    jerry_release_value(jerry_call_function(_func_obj, _obj_obj, &param, 1));

    jerry_release_value(param);
    jerry_release_value(_func_obj);
    jerry_release_value(_func_name);
    jerry_release_value(_obj_obj);
    jerry_release_value(_obj_name);
    jerry_release_value(global_object);
}
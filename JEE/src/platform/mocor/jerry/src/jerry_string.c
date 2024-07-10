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


// #include "jerry_def.h"
#include "jerry_string.h"
#include "jerry_util.h"


#include <string.h>


jerry_string_t jerry_string_create(void) {
  jerry_string_t str;

  str.size = 0;
  str.data = NULL;

  return str;
}


jerry_string_t jerry_string_create_with_size(const char* data, size_t size) {
  jerry_string_t str;

  str.size = size;

  if (data && size > 0) {
    str.data = jerry_buffer_allocate(size);
    memcpy(str.data, data, size);
  } else {
    str.data = NULL;
  }

  return str;
}


jerry_string_t jerry_string_create_with_buffer(char* buffer, size_t size) {
  jerry_string_t str;

  str.size = size;

  if (size > 0) {
    JERRY_ASSERT(buffer != NULL);
    str.data = buffer;
  } else {
    str.data = NULL;
  }

  return str;
}


void jerry_string_destroy(jerry_string_t* str) {
  JERRY_RELEASE(str->data);
  str->size = 0;
}


bool jerry_string_is_empty(const jerry_string_t* str) {
  return str->size == 0;
}

void jerry_string_append(jerry_string_t* str, const char* data, size_t size) {
  JERRY_ASSERT(data != NULL);

  if (data == NULL || size == 0) {
    return;
  }

  if (str->data != NULL) {
    str->data = jerry_buffer_reallocate(str->data, str->size + size);
  } else {
    JERRY_ASSERT(str->size == 0);
    str->data = jerry_buffer_allocate(size);
  }

  memcpy(str->data + str->size, data, size);
  str->size += size;
}


const char* jerry_string_data(const jerry_string_t* str) {
  if (str->data == NULL) {
    return "";
  }

  return str->data;
}


unsigned jerry_string_size(const jerry_string_t* str) {
  return str->size;
}

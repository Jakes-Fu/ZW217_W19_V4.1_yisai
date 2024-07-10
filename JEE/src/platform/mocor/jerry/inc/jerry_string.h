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

#ifndef JERRY_STRING_H
#define JERRY_STRING_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
  size_t size;
  char* data;
} jerry_string_t;

// Create new string
jerry_string_t jerry_string_create(void);
jerry_string_t jerry_string_create_with_size(const char* data, size_t size);
jerry_string_t jerry_string_create_with_buffer(char* buffer, size_t size);

// Destroy string
void jerry_string_destroy(jerry_string_t* str);

// Check if string is empty
bool jerry_string_is_empty(const jerry_string_t* str);

// Append `data` to tail of the string.
void jerry_string_append(jerry_string_t* str, const char* data, size_t size);

// Returns pointer to the bytes (never returns NULL)
const char* jerry_string_data(const jerry_string_t* str);

unsigned jerry_string_size(const jerry_string_t* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* JERRY_STRING_H */

/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        oneos_clock.h
 *
 * @brief       The header file for system clock.
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-12   OneOS Team      First Version
 ***********************************************************************************************************************
 */
#ifndef __OS_CLOCK_H__
#define __OS_CLOCK_H__

#include "os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// system tick number of per second, need redefine
#define OS_TICK_PER_SECOND 1000
extern os_tick_t os_tick_get(void);
extern os_tick_t os_tick_from_ms(os_uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __OS_CLOCK_H__ */

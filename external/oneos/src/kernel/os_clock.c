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
 * @file        oneos_clock.c
 *
 * @brief       This file implements the the operating system tick handling.
 *
 * @revision
 * Date         Author          Notes
 * 2020-10-13   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include "os_clock.h"
#include "os_api.h"

/**
 ***********************************************************************************************************************
 * @brief           This function get os tick.
 *
 * @param[in]       None.
 *
 * @return          System global tick.
 ***********************************************************************************************************************
 */
os_tick_t os_tick_get(void)
{
    return SCI_GetTickCount();
}

/**
 ***********************************************************************************************************************
 * @brief           This function convert milliseconds to ticks.
 *
 * @param[in]       The number of milliseconds to convert.
 *
 * @return          The converted ticks.
 ***********************************************************************************************************************
 */
os_tick_t os_tick_from_ms(os_uint32_t ms)
{
    return ms * OS_TICK_PER_SECOND / 1000;
}
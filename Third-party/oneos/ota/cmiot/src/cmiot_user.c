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
 * @file        cmiot_user.c
 *
 * @brief       Implement user functions
 *
 * @revision
 * Date         Author          Notes
 * 2020-06-16   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include "cmiot_user.h"
#include "cmiot_config.h"
#ifdef OS_USING_SHELL
// #include "shell.h"
#endif
#ifdef CMIOT_USING_CTW
#include <os_clock.h>
#endif
#include <os_task.h>
#include <os_util.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 ***********************************************************************************************************************
 * @brief           This function will return network type
 * @param        void
 *
 * @retval          network type
 ***********************************************************************************************************************
 */
cmiot_char *cmiot_get_network_type(void)
{
    return "-";
}

/**
 ***********************************************************************************************************************
 * @brief                This function will get device unique id, max len is CMIOT_MID_MAXLEN
 * @param[out]      uid        The memory to saved unique id
 *
 * @retval              void
 ***********************************************************************************************************************
 */
void cmiot_get_uniqueid(cmiot_char *uid)
{
#ifdef CMIOT_USING_MULTI_SLAVE
    memcpy(uid, "slave1", sizeof("slave1") - 1);
#else
    memcpy(uid, "112233445566778899AABBCCDDEEFF", sizeof("112233445566778899AABBCCDDEEFF") - 1);
#endif
}

/**
 ***********************************************************************************************************************
 * @brief           This function will return app part name
 * @param        void
 *
 * @retval          app part name
 ***********************************************************************************************************************
 */
cmiot_char *cmiot_app_name()
{
    return CMIOT_ONEOS_APP;
}

/**
 ***********************************************************************************************************************
 * @brief           This function will return download part name
 * @param        void
 *
 * @retval          download part name
 ***********************************************************************************************************************
 */
cmiot_char *cmiot_download_name()
{
    return CMIOT_ONEOS_UPDATE;
}

/**
 ***********************************************************************************************************************
 * @brief           This function for output data
 * @param[in]       data     The data
 * @param[in]       len       The data len
 *
 * @retval          void
 ***********************************************************************************************************************
 */
void cmiot_printf(cmiot_char *data, cmiot_uint32 len)
{
    os_kprintf("%s", data);
}

/**
 ***********************************************************************************************************************
 * @brief           This function for sleep
 * @param[in]    time       The sleep time, ms
 *
 * @retval          void
 ***********************************************************************************************************************
 */
void cmiot_msleep(cmiot_uint32 time)
{
    os_task_msleep(time);
}

#ifdef CMIOT_USING_CTW
/**
 ***********************************************************************************************************************
 * @brief           This function will return relative now time
 * @param        void
 *
 * @retval          relative now time, ms
 ***********************************************************************************************************************
 */
cmiot_uint32 cmiot_get_now_time(void)
{
    return os_tick_get() * 1000 / OS_TICK_PER_SECOND;
}
#endif

#ifdef CMIOT_USING_MULTI_SLAVE
/**
 ***********************************************************************************************************************
 * @brief           This function for reboot
 * @param[in]    void
 *
 * @retval          void
 ***********************************************************************************************************************
 */
void cmiot_reboot_device(void)
{
    extern void os_hw_cpu_reset(void);
    os_hw_cpu_reset();
}
#endif

#if defined(OS_USING_SHELL) && !defined(CMIOT_USING_MULTI_SLAVE)
/**
 ***********************************************************************************************************************
 * @brief              This function will start the upgrade process
 * @param[in]       argc       The input param num
 * @param[in]       argv       The input param
 *
 * @retval             void
 ***********************************************************************************************************************
 */
os_err_t cmiot_cv(int argc, char **argv)
{
    cmiot_int8 rst = cmiot_upgrade();
    os_kprintf("start upgrade result:%d\r\n", rst);
    return 0;
}

/**
 ***********************************************************************************************************************
 * @brief              This function will start the report upgrade process
 * @param[in]       argc       The input param num
 * @param[in]       argv       The input param
 *
 * @retval             void
 ***********************************************************************************************************************
 */
os_err_t cmiot_ru(int argc, char **argv)
{
    cmiot_int8 rst = cmiot_report_upgrade();
    os_kprintf("start report upgrade result:%d\r\n", rst);
    return 0;
}

#ifdef CMIOT_USING_MULTI_MASTER
/**
 ***********************************************************************************************************************
 * @brief              This function will start the upgrade process for slave
 * @param[in]       argc       The input param num
 * @param[in]       argv       The input param
 *
 * @retval             void
 ***********************************************************************************************************************
 */
os_err_t cmiot_cv_slave(int argc, char **argv)
{
    if (argc < 2)
    {
        os_kprintf("At least input one param.\r\n");
        return 0;
    }
    cmiot_int8 rst = cmiot_upgrade_slave(atoi(argv[1]));
    os_kprintf("start upgrade result:%d\r\n", rst);
    return 0;
}

/**
 ***********************************************************************************************************************
 * @brief              This function will start the report upgrade process for slave
 * @param[in]       argc       The input param num
 * @param[in]       argv       The input param
 *
 * @retval             void
 ***********************************************************************************************************************
 */
os_err_t cmiot_ru_slave(int argc, char **argv)
{
    if (argc < 2)
    {
        os_kprintf("At least input one param.\r\n");
        return 0;
    }

    cmiot_int8 rst = cmiot_report_upgrade_slave(atoi(argv[1]));
    os_kprintf("start report upgrade result:%d\r\n", rst);
    return 0;
}
#endif
#endif

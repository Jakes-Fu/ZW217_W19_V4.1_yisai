/**
 ***********************************************************************************************************************
 * Copyright (c) 2021, China Mobile Communications Group Co.,Ltd.
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
 * @file        tcp_atest.c
 *
 * @brief       This is tcp test file based atest.
 *
 * @revision
 * Date         Author          Notes
 * 2020-10-13   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#if 0 //defined(CMS_CONNECT_MQTT) || defined(CMS_CONNECT_TCP)
#include "oneos_config.h"
#include "cms_con_tcp.h"
#include <shell.h>
#include <atest.h>
#include <os_errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <os_task.h>
#include <os_clock.h>

static int        scode      = 301;
static void *     tcp_handle = NULL;
static os_task_t *recv_task  = NULL;
#define TCP_RX_MAX_LENGTH 1024

static float get_time_s(void)
{
    return ((float)os_tick_get() / OS_TICK_PER_SECOND);
}

static uint8_t recv_buff[TCP_RX_MAX_LENGTH];
static int     cms_tcp_recv_once(uint32_t timeout)
{
    int rc = cms_tcp_recv(tcp_handle, recv_buff, sizeof(recv_buff), timeout);
    if (rc <= 0)
        return rc;
    printf("%s(time:%02f,len:%d)", __func__, get_time_s(), rc);
    for (int i = 0; i < rc; i++)
    {
        if (i % 0x10 == 0)
            printf("\r\n");
        printf("%02X ", recv_buff[i]);
    }
    printf("\r\n");
    return rc;
}
static void atest_cms_tcp_recv(void)
{
    int rc = cms_tcp_recv_once(10 * 1000);
    tp_assert_true(rc > 0);
}

static void atest_cms_tcp_recv_process(void *param)
{
    while (cms_tcp_get_state(tcp_handle) == cms_con_state_connect)
    {
        os_task_msleep(100);
        cms_tcp_recv_once(1);
    }
    printf("exit tcp recv task!\r\n");
    recv_task = NULL;
}

static void atest_cms_tcp_recv_task(void)
{
    if (recv_task != NULL)
    {
        os_task_destroy(recv_task);
        recv_task = NULL;
        return;
    }
    recv_task = os_task_create("tcp_recv_test", atest_cms_tcp_recv_process, NULL, 1024, 22);
    os_task_startup(recv_task);
}

static void atest_cms_tcp_init(void)
{
    if (tcp_handle != NULL)
        return;
    tcp_handle = cms_tcp_init(scode, TCP_RX_MAX_LENGTH);
    tp_assert_true(tcp_handle != NULL);
}

static void atest_cms_tcp_deinit(void)
{
    if (tcp_handle == NULL)
        return;
    cms_tcp_deinit(tcp_handle);
    tcp_handle = NULL;
}

static void atest_cms_tcp_connect(void)
{
    int rc;
    if (cms_tcp_get_state(tcp_handle) == cms_con_state_connect)
        return;
    rc = cms_tcp_connect(tcp_handle);
    tp_assert_true(rc == cms_connect_success);
    rc = cms_tcp_get_state(tcp_handle);
    tp_assert_true(rc == cms_con_state_connect);
}

static void atest_cms_tcp_disconnect(void)
{
    cms_tcp_disconnect(tcp_handle);
    tp_assert_true(cms_tcp_get_state(tcp_handle) == cms_con_state_disconnect);
}

static uint8_t buf[128];
static void    atest_cms_tcp_send(void)
{
    for (int i = 0; i < sizeof(buf); i++)
    {
        buf[i] = i % 10 + '0';
    }
    int rc = cms_tcp_send(tcp_handle, buf, sizeof(buf));
    tp_assert_true(rc == sizeof(buf));
}

static void atest_cms_mqtt_get_state(void)
{
    int rc = cms_tcp_get_state(tcp_handle);
    if (rc == cms_con_state_connect)
        printf("tcp is connected!\r\n");
    else
        printf("tcp is not connected!\r\n");
}

static void cms_tcp_all(void)
{
    ATEST_UNIT_RUN(atest_cms_tcp_init);
    ATEST_UNIT_RUN(atest_cms_tcp_connect);
    ATEST_UNIT_RUN(atest_cms_tcp_send);
    ATEST_UNIT_RUN(atest_cms_tcp_disconnect);
    ATEST_UNIT_RUN(atest_cms_tcp_deinit);
}

ATEST_TC_EXPORT(cms.tcp.total, cms_tcp_all, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.init, atest_cms_tcp_init, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.deinit, atest_cms_tcp_deinit, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.connect, atest_cms_tcp_connect, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.disconnect, atest_cms_tcp_disconnect, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.recv.once, atest_cms_tcp_recv, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.recv.task, atest_cms_tcp_recv_task, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.state, atest_cms_mqtt_get_state, NULL, NULL, TC_PRIORITY_LOW);
ATEST_TC_EXPORT(cms.tcp.send, atest_cms_tcp_send, NULL, NULL, TC_PRIORITY_LOW);
#endif
#include "cms_con_tcp.h"
#include "os_util.h"
#include "cms_mqtt_test.h"
#include "cms_test_log.h"

static int        scode      = 301;
// static int scode = 1003;
static void *tcp_handle = NULL;
static void *task_handle = NULL;
#define TCP_RX_MAX_LENGTH 1024
#define LOG_TAG_TEST "cms_con_test"

//## license data
//#define RTC_APPID "b9f7d29d22c6417e96dce9173da0c87c"
//#define RTC_CUSTOMER_KEY "8fdd238ff9a84dbd8802b77ebab1dbb3"
//#define RTC_CUSTOMER_SECRET "7225ba4a115a40c9b7741ea1e29f78a9"
//#define RTC_APPID "4af1d33f790e46138cd398930d7dab96"
#define RTC_APPID "8ddb57deea47473fb5e0f203b3a4f2a6"
#define RTC_CUSTOMER_KEY "cc0b02c4871e46b59700b0b9031c990b"
#define RTC_CUSTOMER_SECRET "99635868a54146ddbf80d912e929bfc3"
#define RTC_LICENSE_KEY "969000000111103"
#define RTC_CREDENTIAL "4919e6ecb4463a23b4ef757891f9a72651d53cb48b6961daf161ec6930cb3a77"
#define CMS_RTC_SEND_BUF_LEN 1024

uint8_t *cms_get_rtc_appid(void)
{
    return RTC_APPID;
}

uint8_t *cms_get_rtc_customer_key(void)
{
    return RTC_CUSTOMER_KEY;
}

uint8_t *cms_get_rtc_customer_secret(void)
{
    return RTC_CUSTOMER_SECRET;
}

uint8_t *cms_get_rtc_license_key(void)
{
    return RTC_LICENSE_KEY;
}

uint8_t *cms_get_rtc_credentail(void)
{
    return RTC_CREDENTIAL;
}

#define DEFAULT_PRINTF_MAX_LEN 128
void cms_printf_128(char *data)
{
    if (!data)
    {
        LOG_TEST_D(LOG_TAG_TEST, " param is NULL, exit...\r\n");
        return;
    }

    uint8_t tmp[DEFAULT_PRINTF_MAX_LEN + 1] = {0};
    int datalen = strlen(data);
    int count = datalen / DEFAULT_PRINTF_MAX_LEN + (datalen % DEFAULT_PRINTF_MAX_LEN ? 1 : 0);
    for (int i = 0; i < count; i++)
    {
        memset(tmp, 0x0, sizeof(tmp));
        strncpy(tmp, data + i * DEFAULT_PRINTF_MAX_LEN, ((datalen - i * DEFAULT_PRINTF_MAX_LEN) > DEFAULT_PRINTF_MAX_LEN) ? sizeof(tmp) : (datalen - i * DEFAULT_PRINTF_MAX_LEN));
        tmp[DEFAULT_PRINTF_MAX_LEN] = '\0';
        LOG_TEST_D(LOG_TAG_TEST, "%s", tmp);
    }
}

void cms_make_rtc_post_data(uint8_t *data)
{
    char *str = data;
    snprintf(str, CMS_RTC_SEND_BUF_LEN, "{\"APPID\":\"%s\",\"CUSTOMER_KEY\":\"%s\",\"CUSTOMER_SECRET\":\"%s\",\"CREDENTIAL\":\"%s\",\"CUSTOM\":\"%s\"}",
             cms_get_rtc_appid(),
             cms_get_rtc_customer_key(),
             cms_get_rtc_customer_secret(),
             cms_get_rtc_credentail(),
             cms_get_rtc_license_key());
}

static uint8_t recv_buff[TCP_RX_MAX_LENGTH];
static int cms_tcp_recv_once(uint32_t timeout)
{
#if 0
    int rc = cms_tcp_recv(tcp_handle, recv_buff, sizeof(recv_buff), timeout);
    if (rc <= 0)
        return rc;
    printf("%s(time:%02f,len:%d)", __func__, get_time_s(), rc);
    for (int i = 0; i < rc; i++)
    {
        if (i % 0x10 == 0)
            printf("\r\n");
        printf("%02X ", recv_buff[i]);
    }
    printf("\r\n");
    return rc;
#else
    //LOG_D(LOG_TAG_TEST, "start to recv data!\r\n");
    int rc = cms_tcp_recv(tcp_handle, recv_buff, sizeof(recv_buff), timeout);
    if (rc <= 0)
        return rc;

    LOG_TEST_D(LOG_TAG_TEST, "recv buf=\r\n");
    cms_printf_128(recv_buff);
    //## parse cert
    char *cert = NULL;
    cert = strstr(recv_buff, "{\"cert\":\"");
    if (cert == NULL)
    {
        LOG_TEST_D(LOG_TAG_TEST, "cert not found\r\n");
        goto END;
    }

    return rc;
END:
// tcp_handle
#endif
}

#define CERTIFICATE_MAX_LEN (1024)
static void acms_tcp_test_recv_process(int32_t argc, void *param)
{
    char *cert = NULL;
    int idx = 0;
    while (cms_tcp_get_state(tcp_handle) == cms_con_state_connect)
    {

        int rc = cms_tcp_recv(tcp_handle, recv_buff, sizeof(recv_buff), 1000 * 3); // 3s
        if (rc == 0)
        {
            msleep(100);
            continue;
        }
        else if (rc < 1)
        {
            break;
        }

        LOG_TEST_D(LOG_TAG_TEST, "recv buf=\r\n");
        cms_printf_128(recv_buff);

        //## parse cert data
        //cert = strstr(recv_buff, "{\"cert\":\"");
        cert = strstr(recv_buff, "{\"success\":true,\"cert\":\"");
        if (cert == NULL)
        {
            LOG_TEST_D(LOG_TAG_TEST, "cert not found\r\n");
            break;
        }

        char str_certificate[CERTIFICATE_MAX_LEN] = {0};
        cert += 24;
        while ((str_certificate[idx++] = *cert++) != '\"');
        str_certificate[idx - 1] = 0;
        LOG_TEST_D(LOG_TAG_TEST, "str_certificate=\r\n");
        cms_printf_128(str_certificate);
        break;
    }

    LOG_TEST_D(LOG_TAG_TEST, "exit tcp recv task!\r\n");
    //recv_task = NULL;
    cms_tcp_deinit(tcp_handle);
    tcp_handle = NULL;
}

int cms_tcp_test_entry()
{
    // init
    int iResult = -1;
    if (tcp_handle != NULL)
    {
        LOG_TEST_D(LOG_TAG_TEST, "tcp_handle has been inited, exited...\r\n");
        return iResult;
    }

    tcp_handle = cms_tcp_init(scode, TCP_RX_MAX_LENGTH);
    if (tcp_handle == NULL)
    {
        LOG_TEST_D(LOG_TAG_TEST, "tcp_handle is NULL...\r\n");
        return iResult;
    }

    // connect
    iResult = cms_tcp_connect(tcp_handle);
    if (iResult)
    {
        LOG_TEST_D(LOG_TAG_TEST, "cms tcp connect failed!...\r\n");
        goto T_ERROR;
        return iResult;
    }

    //send

    LOG_TEST_D(LOG_TAG_TEST, "start to send data!...\r\n");
    uint8_t buf[CMS_RTC_SEND_BUF_LEN] = {0};
    cms_make_rtc_post_data(buf);
    LOG_TEST_D(LOG_TAG_TEST, "send buf=\r\n");
    cms_printf_128(buf);
    msleep(100);
    iResult = cms_tcp_send(tcp_handle, buf, sizeof(buf));
    if (iResult <= 0)
    {
        LOG_TEST_D(LOG_TAG_TEST, "cms tcp send failed!...\r\n");
        goto T_ERROR;
    }

    LOG_TEST_D(LOG_TAG_TEST, "startup recv task!...\r\n");
    // create task recv data
    task_handle = task_create_with_name("cms_tcp_test", acms_tcp_test_recv_process, NULL, 2048, 80);
    if (task_handle == NULL)
    {
        LOG_TEST_D(LOG_TAG_TEST, "cms tcp recv task create failed!...\r\n");
        goto T_ERROR;
    }

    iResult = 0;
    return iResult;

T_ERROR:
    // deinit tcp connect
    cms_tcp_deinit(tcp_handle);
    tcp_handle = NULL;
    iResult = -1;
    return iResult;
}

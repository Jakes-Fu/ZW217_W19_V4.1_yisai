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

#if 0//defined(CMS_CONNECT_MQTT) || defined(CMS_CONNECT_TCP)
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
#include "cms_test_log.h"
#include "cms_con_mqtt.h"

static void *mqtt_handle = NULL;
static int   scode       = 300;
char         topic_str[] = "svr/300/req";

#define MQTT_ATEST_BUFF_DEEP 1000
#define LOG_TAG_TEST "cms_mqtt_test"

static void message_handler(void *handle, const char *topic_name, void *payload, size_t payload_len)
{
    LOG_TEST_D(LOG_TAG_TEST, "topic:%s\r\n", topic_name);
    uint8_t *buf = (uint8_t *)payload;
    for (int i = 0; i < payload_len; i++)
    {
        LOG_TEST_D(LOG_TAG_TEST, "%02X ", buf[i]);
    }
    LOG_TEST_D(LOG_TAG_TEST,"\r\n");
}

int cms_mqtt_test_entry()
{
    // init
    int rval = -1;
    if (mqtt_handle != NULL) {
        LOG_TEST_D(LOG_TAG_TEST, "mqtt_handle has been inited, exited...\r\n");
        rval = 0;
        return rval;
    }
    cms_mqtt_param param_init = {5000, MQTT_ATEST_BUFF_DEEP, MQTT_ATEST_BUFF_DEEP};
    mqtt_handle          = cms_mqtt_init(scode, &param_init);

    if (mqtt_handle == NULL) {
        LOG_TEST_D(LOG_TAG_TEST, "cms_mqtt init failed...\r\n");
        return rval;
    }

    cms_mqtt_connect_param param_connect;
    memset(&param_connect, 0, sizeof(cms_mqtt_connect_param));
    param_connect.client_id    = "12345678";
    param_connect.username     = "oneos";
    param_connect.password     = "123456";
    param_connect.keep_alive_s = 300;
    rval = cms_mqtt_connect(mqtt_handle, &param_connect);
    if (rval) {
        LOG_TEST_D(LOG_TAG_TEST, "cms mqtt connect failed!...\r\n");
        goto T_ERROR;
        return rval;
    }

    //subscribe
    msleep(20);
    LOG_TEST_D(LOG_TAG_TEST, "start to subscribe msg!...\r\n");
    rval = cms_mqtt_subscribe(mqtt_handle, topic_str, mqtt_qos1, message_handler);
    if (rval) {
        LOG_TEST_D(LOG_TAG_TEST, "cms mqtt subscribe failed!...\r\n");
        goto T_ERROR;
        return rval;
    }

    //publish
    LOG_TEST_D(LOG_TAG_TEST, "start to publish msg!...\r\n");
    char long_buf[MQTT_ATEST_BUFF_DEEP] = {0};
    cms_mqtt_message mqtt_message;

    memset(long_buf, '0', MQTT_ATEST_BUFF_DEEP - 100);

    mqtt_message.qos        = mqtt_qos0;
    mqtt_message.payload    = (void *)long_buf;
    mqtt_message.payloadlen = MQTT_ATEST_BUFF_DEEP - 100;
    rval                    = cms_mqtt_publish(mqtt_handle, topic_str, &mqtt_message);
    if (rval) {
        LOG_TEST_D(LOG_TAG_TEST, "cms mqtt publish once failed!...\r\n");
        goto T_ERROR;
        return rval;
    }

    memset(long_buf, '1', MQTT_ATEST_BUFF_DEEP - 100);
    mqtt_message.qos        = mqtt_qos1;
    mqtt_message.payload    = (void *)long_buf;
    mqtt_message.payloadlen = MQTT_ATEST_BUFF_DEEP - 100;
    rval                      = cms_mqtt_publish(mqtt_handle, topic_str, &mqtt_message);
    if (rval) {
        LOG_TEST_D(LOG_TAG_TEST, "cms mqtt publish second failed!...\r\n");
        goto T_ERROR;
        return rval;
    }

    LOG_TEST_D(LOG_TAG_TEST, "end cms_mqtt_test_entry!...\r\n");
    //return rval;

T_ERROR:
    // deinit tcp connect
    cms_mqtt_deinit(mqtt_handle);
    mqtt_handle = NULL;
    return rval;
}

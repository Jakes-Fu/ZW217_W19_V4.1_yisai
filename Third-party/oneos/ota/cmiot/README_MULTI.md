# 多设备升级适配说明

- [**简介**](#简介)
- [**API介绍**](#API介绍)
- [**主设备适配**](#主设备适配)
- [**从设备适配**](#从设备适配)
- [**应用示例**](#应用示例)

## 简介

本方案为主设备有通信能力，从设备没有通信能力，借助主设备的通信来升级自身的场景的方案。主从设备有私有的内部通信协议，用户需要做好主从设备的数据传输。大体的流程为：主设备发送一个消息给从设备，从设备执行对应的操作并返回结果给主设备。



## API介绍

**API列表**

| **接口**          | **说明**                                                 |
| :---------------- | :------------------------------------------------------- |
| cmiot_send_cmd    | 主设备发送命令给从设备并接收结果的接口，需要用户实现     |
| cmiot_receive_cmd | 从设备执行主设备发送的命令并返回结果的接口，需要用户调用 |

**API说明**

**cmiot_send_cmd**

该函数用于主设备发送命令给从设备并接收结果，其函数原型如下：

```c
cmiot_int8 *cmiot_send_cmd(cmiot_int16 slave_num, cmiot_char *cmd, cmiot_int16 cmd_len, cmiot_char *resp, cmiot_int16 *resp_len);
```

| **参数**   | **说明**                       |
| :--------- | :----------------------------- |
| slave_num  | 从设备编号                     |
| cmd        | 主设备发送命令                 |
| cmd_len    | 主设备发送命令的长度           |
| resp       | 从设备响应填入的地址           |
| resp_len   | 从设备响应的长度               |
| **返回**   | **说明**                       |
| cmiot_int8 | 网络类型，用户自定义，不能为空 |

**API说明**

**cmiot_receive_cmd**

该函数用于从设备执行主设备发送的命令并返回结果，其函数原型如下：

```c
cmiot_int8 *cmiot_receive_cmd(cmiot_char *cmd, cmiot_int16 cmd_len, cmiot_char *resp, cmiot_int16 *resp_len);
```

| **参数**   | **说明**                       |
| :--------- | :----------------------------- |
| cmd        | 主设备发送的命令               |
| cmd_len    | 主设备发送的命令长度           |
| resp       | 从设备响应的填入地址           |
| resp_len   | 从设备响应的长度               |
| **返回**   | **说明**                       |
| cmiot_int8 | 网络类型，用户自定义，不能为空 |



## 主设备适配

用户在调用cmiot_upgrade_slave和cmiot_report_upgrade_slave时，可以带一个从设备ID，这个ID会传递到cmiot_send_cmd的第一个参数slave_num，用户可以根据这个选择给哪个从设备发送命令。用户实现cmiot_send_cmd时需要做的工作：在主设备端将cmd_len长度的cmd和resp_len参数通过主从设备的连接介质（如uart，485总线，蓝牙）发送出去并等待从设备的响应，从设备响应后，将结果填入resp，resp长度填入resp_len。



## 从设备适配

从设备在接收到主设备的命令完整命令后，申请接收到的resp_len长度的内存，将接收到的主设备命令作为cmiot_receive_cmd的第一个参数，其长度作为第二个参数，申请的内存作为第三个参数，收到的resp_len作为第四个参数，来调用cmiot_receive_cmd。cmiot_receive_cmd返回后，将resp_len长度的resp返回给主设备，并释放内存。



## 应用示例

主设备示意代码：

cmiot_int8 cmiot_send_cmd(cmiot_int16 slave_num, cmiot_char *cmd, cmiot_int16 cmd_len, cmiot_char *resp, cmiot_int16 *resp_len)
{
    选择slave_num的从设备;

​    发送cmd_len个字节长度的cmd和resp_len数值；

​    while(！从设备返回flag && 没有超时) 

​        等待从设备的返回；

​    memcpy(resp， 从设备的响应内容resp_slave， 从设备的响应内容长度resp_slave_len）；

​    *resp_len = 从设备的响应内容长度resp_slave_len；

}



void master_receive_task(void *parameter)

{

​    持续接收从设备的有效数据填入从设备的返回内容msg_slave；

​    如果接收完整的返回，解析出响应内容resp_slave和响应内容长度resp_slave_len，并设置从设备返回flag；

}



从设备示意代码：

void slave_receive_task(void *parameter)

{

​    持续接收主设备的有效数据填入主设备的命令cmd_master中；

​    如果接收完整的命令，解析出主设备命令cmd和命令的长度cmd_len，解析出响应的最大长度resp_len；

​    申请resp_len长度的空间resp；

​    调用cmiot_receive_cmd（cmd， cmd_len， resp，resp_len）；

​    将resp和resp_len返回给主设备；

}



基于潘多拉的示例代码（基于oneos2.x，仅做测试用）：

master：

```
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
 * @file        main.c
 *
 * @brief       User application entry
 *
 * @revision
 * Date         Author          Notes
 * 2020-11-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include <board.h>
#include <os_task.h>
#include <os_event.h>
#include <cmiot_user.h>
#include <stdio.h>

#define CMD_BEGIN_STR "CMIOT_CMD:"
static os_event_t rx_notice_evt;
static os_device_t *device;
static struct serial_configure uart_config = OS_SERIAL_CONFIG_DEFAULT;
static os_size_t total_len = 0;
static os_size_t g_header_len = 0;
static char data[300] = {0};

void cmiot_print_raw(const char *name, const char *buf, os_size_t size)
{
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
#define WIDTH_SIZE     32

    for (int i = 0; i < size; i += WIDTH_SIZE)
    {
        os_kprintf("[cmiot] %s: %04X-%04X: ", name, i, i + WIDTH_SIZE);
        for (int j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                os_kprintf("%02X ", buf[i + j]);
            }
            else
            {
                os_kprintf("   ");
            }
            if ((j + 1) % 8 == 0)
            {
                os_kprintf(" ");
            }
        }
        os_kprintf("  ");
        for (int j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                os_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        os_kprintf("\r\n");
    }
}

static os_err_t rx_indicate(os_device_t *dev, struct os_device_cb_info *info)
{
    if (info->size > 0)
    {
        os_event_send(&rx_notice_evt, 1);
    }
    return OS_EOK;
}

static os_err_t test_getchar(os_device_t *dev, char *ch, os_int32_t timeout)
{
    {
        os_err_t result = OS_EOK;

        while (os_device_read_nonblock(dev, 0, ch, 1) == 0)
        {
            result = os_event_recv(&rx_notice_evt, 1, OS_EVENT_OPTION_OR | OS_EVENT_OPTION_CLEAR, timeout, OS_NULL);

            if (OS_EOK != result)
            {
                return result;
            }
        }

        return OS_EOK;
    }
}

static void user_task(void *parameter)
{
    os_size_t read_len = 0;
    char prev_read_char = 0;
    char curr_read_char = 0;
    cmiot_int16 data_len = 0;
    cmiot_int16 len_item_len = 0;
    char header[sizeof(CMD_BEGIN_STR)] = {0};
    cmiot_bool header_ok = cmiot_false;
    while (1)
    {
        test_getchar(device, &curr_read_char, OS_WAIT_FOREVER);
        // os_kprintf("master byte:%02x, header_ok:%d, read_len:%d\r\n", curr_read_char, header_ok, read_len);
        if (total_len != 0)
        {
            continue;
        }

        if (header_ok == cmiot_false && read_len < sizeof(header) - 1)    // read header
        {
            header[read_len++] = curr_read_char;
            header[read_len] = 0;
            // os_kprintf("master header byte:%02x, byte pos:%d\r\n", curr_read_char, read_len);
            if (curr_read_char == 0 || strstr(CMD_BEGIN_STR, header) == NULL)    // is not sub str
            {
                read_len = 0;
            }
            if (!memcmp(CMD_BEGIN_STR, header, sizeof(CMD_BEGIN_STR)))    // header last byte
            {
                header_ok = cmiot_true;
                read_len = 0;
            }
            continue;
        }

        if (header_ok == cmiot_true && read_len < sizeof(data) - 1)    // read value
        {
            data[read_len++] = curr_read_char;
            // os_kprintf("master data byte:%02x, byte pos:%d\r\n", curr_read_char, read_len);
        }

        if (data_len == 0 && curr_read_char == '\n' && prev_read_char == '\r')
        {
            data_len = atoi(data);
            len_item_len = read_len;
            // os_kprintf("data:%s, data_len:%d, len_item_len:%d\r\n", data, data_len, read_len);
        }
        if (data_len < 0)
        {
            g_header_len = 0;
            total_len = read_len;
            len_item_len = 0;
            read_len = 0;
            data_len = 0;
            header_ok = cmiot_false;
            continue;
        }
        if (data_len && data_len <= read_len - len_item_len)
        {
            g_header_len = len_item_len;
            // cmiot_print_raw("cmiot data", data + g_header_len, data_len);
            total_len = data_len;
            len_item_len = 0;
            read_len = 0;
            data_len = 0;
            header_ok = cmiot_false;
        }
        prev_read_char = curr_read_char;
    }
}

int main(void)
{
    os_kprintf("--------------------oneos version:%s\r\n", CMIOT_FIRMWARE_VERSION);
    os_task_t *task;

    os_event_init(&rx_notice_evt, "test");

    device = os_device_find("uart2");
    if (NULL == device)
    {
        os_kprintf("AT Parser initialize failed! Can not open the device.");
        return 0;
    }

    uart_config.baud_rate = 115200;
    os_device_control(device, OS_DEVICE_CTRL_CONFIG, &uart_config);

    os_err_t result = os_device_open(device);
    if (result != OS_EOK)
    {
        os_kprintf("AT Parser initialize failed! Can not open the device.");
        return OS_EOK;
    }

    struct os_device_cb_info cb_info = {
        .type = OS_DEVICE_CB_TYPE_RX,
        .data = 0, /* The AT Parser operator is passed through a callback structure */
        .cb = rx_indicate,
    };

    result = os_device_control(device, OS_DEVICE_CTRL_SET_CB, &cb_info);
    if (result != OS_EOK)
    {
        os_kprintf("Set AT Parse device receive indicate failed.");
        return OS_EOK;
    }

    task = os_task_create("user", user_task, NULL, 2048, 3);
    OS_ASSERT(task);
    os_task_startup(task);

    return 0;
}

cmiot_extern cmiot_int8
cmiot_send_cmd(cmiot_int16 slave_num, cmiot_char *cmd, cmiot_int16 cmd_len, cmiot_char *resp, cmiot_int16 *resp_len)
{
    char data_len_str[20] = {CMD_BEGIN_STR};
    snprintf(data_len_str + strlen(data_len_str),
             sizeof(data_len_str) - strlen(data_len_str),
             "%d\r\n",
             cmd_len + 2);                                                   // data len and resp len
    os_device_write_block(device, 0, data_len_str, strlen(data_len_str));    // send header
    int sent_size = 0;
    while (sent_size < (int)cmd_len)
    {
        sent_size += os_device_write_block(device, 0, cmd + sent_size, cmd_len - sent_size);    // send data
    }
    os_device_write_block(device, 0, resp_len, 2);    // send resp len

    if (resp != NULL && resp_len != NULL)
    {
        int time = 50;
        while (total_len == 0 && time > 0)
        {
            os_task_msleep(100);
            --time;
        }
        if (!memcmp(data, "-1", sizeof("-1") - 1) || total_len == 0)
        {
            total_len = 0;
            g_header_len = 0;
            return E_CMIOT_FAILURE;
        }
        cmiot_print_raw("cmiot data resp", data + g_header_len, total_len);
        memcpy(resp, data + g_header_len, total_len);
        *resp_len = total_len;
        // memset(data, 0 ,sizeof(data));
        total_len = 0;
        g_header_len = 0;
    }
    return 0;
}
```



slave:

```
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
 * @file        main.c
 *
 * @brief       User application entry
 *
 * @revision
 * Date         Author          Notes
 * 2020-11-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include <board.h>
#include <os_task.h>
#include <os_event.h>
#include <cmiot_user.h>
#include <stdio.h>

#define CMD_BEGIN_STR "CMIOT_CMD:"
static os_event_t rx_notice_evt;
static os_device_t *device;
static struct serial_configure uart_config = OS_SERIAL_CONFIG_DEFAULT;
static char data[300] = {0};

void cmiot_print_raw(const char *name, const char *buf, os_size_t size)
{
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
#define WIDTH_SIZE     32

    for (int i = 0; i < size; i += WIDTH_SIZE)
    {
        os_kprintf("[cmiot] %s: %04X-%04X: ", name, i, i + WIDTH_SIZE);
        for (int j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                os_kprintf("%02X ", buf[i + j]);
            }
            else
            {
                os_kprintf("   ");
            }
            if ((j + 1) % 8 == 0)
            {
                os_kprintf(" ");
            }
        }
        os_kprintf("  ");
        for (int j = 0; j < WIDTH_SIZE; j++)
        {
            if (i + j < size)
            {
                os_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        os_kprintf("\r\n");
    }
}

static os_err_t rx_indicate(os_device_t *dev, struct os_device_cb_info *info)
{
    if (info->size > 0)
    {
        os_event_send(&rx_notice_evt, 1);
    }
    return OS_EOK;
}

static os_err_t test_getchar(os_device_t *dev, char *ch, os_int32_t timeout)
{
    {
        os_err_t result = OS_EOK;

        while (os_device_read_nonblock(dev, 0, ch, 1) == 0)
        {
            result = os_event_recv(&rx_notice_evt, 1, OS_EVENT_OPTION_OR | OS_EVENT_OPTION_CLEAR, timeout, OS_NULL);

            if (OS_EOK != result)
            {
                return result;
            }
        }

        return OS_EOK;
    }
}

#include <cmiot_user.h>
cmiot_extern cmiot_int8 cmiot_receive_cmd(cmiot_char *cmd,
                                          cmiot_int16 cmd_len,
                                          cmiot_char *resp,
                                          cmiot_int16 *resp_len);

static void user_task(void *parameter)
{
    os_size_t read_len = 0;
    char prev_read_char = 0;
    char curr_read_char = 0;
    cmiot_int16 data_len = 0;
    cmiot_int16 resp_len = 0;
    cmiot_int16 len_item_len = 0;
    char header[sizeof(CMD_BEGIN_STR)] = {0};
    cmiot_bool header_ok = cmiot_false;
    while (1)
    {
        test_getchar(device, &curr_read_char, OS_WAIT_FOREVER);
        // os_kprintf("slave byte:%02x, header_ok:%d, read_len:%d\r\n", curr_read_char, header_ok, read_len);

        if (resp_len)
        {
            continue;
        }

        if (header_ok == cmiot_false && read_len < sizeof(header) - 1)    // read header
        {
            header[read_len++] = curr_read_char;
            header[read_len] = 0;
            // os_kprintf("slave header byte:%02x, byte pos:%d\r\n", curr_read_char, read_len);
            if (curr_read_char == 0 || strstr(CMD_BEGIN_STR, header) == NULL)    // is not sub str
            {
                read_len = 0;
            }
            if (!memcmp(CMD_BEGIN_STR, header, sizeof(CMD_BEGIN_STR)))    // header last byte
            {
                header_ok = cmiot_true;
                read_len = 0;
            }
            continue;
        }

        if (header_ok == cmiot_true && read_len < sizeof(data) - 1)    // read value
        {
            data[read_len++] = curr_read_char;
            // os_kprintf("slave data byte:%02x, byte pos:%d\r\n", curr_read_char, read_len);
        }

        if (data_len == 0 && curr_read_char == '\n' && prev_read_char == '\r')
        {
            data_len = atoi(data);
            len_item_len = read_len;
            // os_kprintf("data:%s, data_len:%d, len_item_len:%d\r\n", data, data_len, read_len);
        }

        if (data_len && data_len <= read_len - len_item_len)
        {
            cmiot_print_raw("cmiot_data", data, read_len);
            resp_len = ((data[read_len - 1] << 8) | data[read_len - 2]);
            // os_kprintf("resp_len:%d, resp_len1:%d, resp_len2:%d\r\n", resp_len, data[read_len - 1], data[read_len -
            // 2]);
            char *resp = NULL;
            if (resp_len)
            {
                resp = malloc(resp_len);
                if (resp == NULL)
                {
                    os_kprintf("malloc falied.\r\n");
                    os_device_write_block(device, 0, CMD_BEGIN_STR "-1\r\n", sizeof(CMD_BEGIN_STR) - 1 + 4);
                    data_len = 0;
                    read_len = 0;
                    len_item_len = 0;
                    header_ok = cmiot_false;
                    resp_len = 0;
                    continue;
                }
                memset(resp, 0, resp_len);
            }
            int result = cmiot_receive_cmd(data + len_item_len, data_len - 2, resp, &resp_len);    // 2 byte is resp len
            if (result != E_CMIOT_SUCCESS)
            {
                os_kprintf("cmiot_receive_cmd falied.\r\n");
                os_device_write_block(device, 0, CMD_BEGIN_STR "-1\r\n", sizeof(CMD_BEGIN_STR) - 1 + 4);
                data_len = 0;
                len_item_len = 0;
                header_ok = cmiot_false;
                read_len = 0;
                resp_len = 0;
                if (resp)
                {
                    free(resp);
                    resp = NULL;
                }
                continue;
            }
            cmiot_print_raw("cmiot_data", resp, resp_len);
            char data_len_str[20] = {CMD_BEGIN_STR};
            snprintf(data_len_str + strlen(data_len_str),
                     sizeof(data_len_str) - strlen(data_len_str),
                     "%d\r\n",
                     resp_len);    // data len
            // os_kprintf("slave send header:%s\r\n", data_len_str);
            os_device_write_block(device, 0, data_len_str, strlen(data_len_str));    // send header
            int sent_size = 0;
            while (sent_size < resp_len)
            {
                sent_size += os_device_write_block(device, 0, resp + sent_size, resp_len - sent_size);    // send data
            }
            free(resp);
            resp = NULL;
            data_len = 0;
            len_item_len = 0;
            header_ok = cmiot_false;
            read_len = 0;
            resp_len = 0;
        }
        prev_read_char = curr_read_char;
    }
}

int main(void)
{
    os_kprintf("--------------------oneos version:%s\r\n", CMIOT_FIRMWARE_VERSION);
    os_task_t *task;

    os_event_init(&rx_notice_evt, "test");

    device = os_device_find("uart2");
    if (NULL == device)
    {
        os_kprintf("AT Parser initialize failed! Can not open the device.");
        return 0;
    }

    uart_config.baud_rate = 115200;
    os_device_control(device, OS_DEVICE_CTRL_CONFIG, &uart_config);

    os_err_t result = os_device_open(device);
    if (result != OS_EOK)
    {
        os_kprintf("AT Parser initialize failed! Can not open the device.");
        return OS_EOK;
    }

    struct os_device_cb_info cb_info = {
        .type = OS_DEVICE_CB_TYPE_RX,
        .data = 0, /* The AT Parser operator is passed through a callback structure */
        .cb = rx_indicate,
    };

    result = os_device_control(device, OS_DEVICE_CTRL_SET_CB, &cb_info);
    if (result != OS_EOK)
    {
        os_kprintf("Set AT Parse device receive indicate failed.");
        return OS_EOK;
    }

    task = os_task_create("user", user_task, NULL, 2048, 3);
    OS_ASSERT(task);
    os_task_startup(task);

    return 0;
}
```


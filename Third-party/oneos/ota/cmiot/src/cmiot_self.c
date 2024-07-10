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
 * @file        cmiot_self.c
 *
 * @brief       Implement self functions
 *
 * @revision
 * Date         Author          Notes
 * 2020-06-16   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#include "cmiot_type.h"
#include "cmiot_config.h"
#include "cmiot_user.h"
#include "cmiot_client.h"

cmiot_uint16 cmiot_get_recieve_buf_len()
{
    return CMIOT_RECIEVE_BUF_MAXLEN;
}

cmiot_uint16 cmiot_get_send_buf_len(void)
{
    return CMIOT_SEND_BUF_MAXLEN;
}

cmiot_uint8 cmiot_get_default_protocol(void)
{
    return CMIOT_DEFAULT_NETWORK_PROTOCOL;
}

#ifndef CMIOT_USING_CMS
cmiot_char *cmiot_get_manufacturer(void)
{
    return CMIOT_FOTA_SERVICE_OEM;
}

cmiot_char *cmiot_get_model_number(void)
{
    return CMIOT_FOTA_SERVICE_MODEL;
}

cmiot_char *cmiot_get_product_id(void)
{
    return CMIOT_FOTA_SERVICE_PRODUCT_ID;
}

cmiot_char *cmiot_get_product_sec(void)
{
    return CMIOT_FOTA_SERVICE_PRODUCT_SEC;
}

cmiot_char *cmiot_get_device_type(void)
{
    return CMIOT_FOTA_SERVICE_DEVICE_TYPE;
}

cmiot_char *cmiot_get_platform(void)
{
    return CMIOT_FOTA_SERVICE_PLATFORM;
}
#else
cmiot_uint8 cmiot_get_update_type(cmiot_uint32 download_package_type)
{
    if (download_package_type == CMIOT_SERVER_FULL_UPGRADE_CODE)
    {
        return CMIOT_CLIENT_FULL_UPGRADE_CODE;
    }
    else
    {
        return CMIOT_FOTA_ALGORITHM;
    }
}
#endif

cmiot_uint32 cmiot_get_try_time(void)
{
    return CMIOT_DEFAULT_TRY_TIME;
}

cmiot_uint8 cmiot_get_try_count(void)
{
    return CMIOT_DEFAULT_TRY_COUNT;
}

cmiot_uint32 cmiot_get_utc_time(void)
{
    return CMIOT_DEFAULT_UTC_TIME;
}

cmiot_char *cmiot_get_apk_version(void)
{
    return CMIOT_FOTA_OS_VERSION;
}

cmiot_char *cmiot_get_firmware_version(void)
{
    return CMIOT_FIRMWARE_VERSION;
}

cmiot_uint8 cmiot_get_default_segment_size(void)
{
    return CMIOT_DEFAULT_SEGMENT_SIZE_INDEX;
}

cmiot_int8 cmiot_get_num_of_extra_blocks(void)
{
#ifdef CMIOT_ALGORITHM_FULL_UPGRADE
    return CMIOT_ONE_EXTRA_BLOCKS;
#else
    return CMIOT_TWO_EXTRA_BLOCKS;
#endif
}

#ifdef CMIOT_ALGORITHM_LUCKPAND
cmiot_uint32 cmiot_get_diff_block_size(void)
{
    return CMIOT_FOTA_CONTRAST_BLOCK_SIZE * 1024;
}
#endif

#ifdef CMIOT_USING_CTW
cmiot_uint32 cmiot_get_ctw_product_id(void)
{
    return CMIOT_CTW_PRODUCT_ATTR_ID;
}

cmiot_uint16 cmiot_get_ctw_fota_service_id(void)
{
    return CMIOT_CTW_FOTA_SERVICE_ID;
}

cmiot_uint8 cmiot_get_ctw_msg_type(void)
{
    return CMIOT_CTW_MSG_TYPE;
}
cmiot_bool cmiot_send_empty_data_to_ctw(void)
{
    return cmiot_true;
}
#endif

#ifndef CMIOT_USING_MULTI_SLAVE
/**
 ***********************************************************************************************************************
 * @brief           检测是否有新版本升级，网络正常才能使用
 *
 * @param[in]       version: 如果新版本检测成功，存放新版本号，字符长度最大CMIOT_VERSION_NAME_MAX
 *
 * @return          E_CMIOT_SUCCESS：检测成功
 *                  E_CMIOT_FAILURE：无网络等异常原因
 *                  E_CMIOT_NOMEMORY：内存不足，初始化失败
 *                  E_CMIOT_LAST_VERSION：当前版本已经是最新
 *                  E_CMIOT_PAKAGE_TOO_LARGE：检测到新版本，但是版本数据超限，无法升级
 *                  E_CMIOT_NEW_VERSION_EXIST：新版本已下载，请重启升级
 ***********************************************************************************************************************
 */
cmiot_int8 cmiot_check_version(char version[CMIOT_VERSION_NAME_MAX])
{
    cmiot_uint8 ret;
#if CMIOT_DEFAULT_NETWORK_PROTOCOL == CMIOT_PROTOCOL_COAP
    ret = cmiot_check_version_coap(version);
#else
    ret = cmiot_check_version_http(version);
#endif
    return ret;
}
/**
 ***********************************************************************************************************************
 * @brief           检测到新版本后，可调用此接口下载新版本
 *
 * @param
 *
 * @return          E_CMIOT_SUCCESS：下载成功，重启终端后会进入还原流程
 *                  E_CMIOT_FAILURE：无网络等异常原因
 *                  E_CMIOT_NOMEMORY：内存不足，初始化失败
 ***********************************************************************************************************************
 */
cmiot_int8 cmiot_only_upgrade(void)
{
    cmiot_uint8 ret;
#if CMIOT_DEFAULT_NETWORK_PROTOCOL == CMIOT_PROTOCOL_COAP
    ret = cmiot_only_upgrade_coap();
#else
    ret = cmiot_only_upgrade_http();
#endif
    return ret;
}

cmiot_int8 cmiot_upgrade_inner(void)
{
#if CMIOT_DEFAULT_NETWORK_PROTOCOL == CMIOT_PROTOCOL_COAP
    return cmiot_upgrade_coap();
#else
    return cmiot_upgrade_http();
#endif
}
/**
 ***********************************************************************************************************************
 * @brief           一键升级接口,检测到新版本后直接升级
 *
 * @param
 *
 * @return          E_CMIOT_SUCCESS：检测到新版本并下载成功，重启终端后会进入还原流程
 *                  E_CMIOT_FAILURE：无网络等异常原因
 *                  E_CMIOT_NOMEMORY：内存不足，初始化失败
 *                  E_CMIOT_LAST_VERSION：当前版本已经是最新
 *                  E_CMIOT_PAKAGE_TOO_LARGE：检测到新版本，但是版本数据过大，无法升级
 ***********************************************************************************************************************
 */
cmiot_int8 cmiot_upgrade(void)
{
#ifdef CMIOT_USING_MULTI_MASTER
    cmiot_reset_config();
#endif
    return cmiot_upgrade_inner();
}

cmiot_int8 cmiot_report_upgrade_inner(void)
{
#if CMIOT_DEFAULT_NETWORK_PROTOCOL == CMIOT_PROTOCOL_COAP
    return cmiot_report_upgrade_coap();
#else
    return cmiot_report_upgrade_http();
#endif
}
/**
 ***********************************************************************************************************************
 * @brief           升级结果上报接口
 *
 * @param
 *
 * @return          E_CMIOT_SUCCESS：升级结果上报成功
 *                  E_CMIOT_FAILURE：无网络等异常原因上报失败
 *                  E_CMIOT_NO_UPGRADE：未检测到升级，无需上报
 ***********************************************************************************************************************
 */
cmiot_int8 cmiot_report_upgrade(void)
{
#ifdef CMIOT_USING_MULTI_MASTER
    cmiot_reset_config();
#endif
    return cmiot_report_upgrade_inner();
}

#endif

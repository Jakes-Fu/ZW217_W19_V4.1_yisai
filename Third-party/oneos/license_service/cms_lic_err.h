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
 * @file        cms_lic_err.h
 *
 * @brief       
 *
 * @revision
 * Date         Author          Notes
 * 2021-12-1    OneOS Team      First Version
 ***********************************************************************************************************************
 */
#ifndef __CMS_LIC_ERR_H__
#define __CMS_LIC_ERR_H__

/* Error Code */
typedef enum{
    LICENSE_PROCESS_OK                     = 0x00U,
    LICENSE_READ_HID_FAIL                  = 0x01U,
    LICENSE_CPID_NOT_MATCH                 = 0x02U,
    LICENSE_NULL_POINTER                   = 0x03U,
    LICENSE_LIC_SIZE_INVALID               = 0x04U,
    LICENSE_SM4_INIT_FAIL                  = 0x05U,
    LICENSE_DECRYPT_FAIL                   = 0x06U,
    LICENSE_BODY_LEN_INVALID               = 0x07U,
    LICENSE_HID_VERIFY_FAIL                = 0x08U,
    LICENSE_PARA_NOT_ENOUGH                = 0x09U,
    LICENSE_CPID_NONE_TABLE                = 0x0AU,
    LICENSE_FLASH_PART_INVALID             = 0x0BU,
    LICENSE_MAGIC_MALLOC_FAIL              = 0x0CU,
    LICENSE_MAGIC_READ_FAIL                = 0x0DU,
    LICENSE_MAGIC_WRITE_FAIL               = 0x0EU,
    LICENSE_FLASH_ERASE_FAIL               = 0x0FU,
    LICENSE_WRITE_FAIL                     = 0x10U,
    LICENSE_READ_POINTER_NULL              = 0x11U,
    LICENSE_READ_FAIL                      = 0x12U,
    LICENSE_UART_NO_HEADER                 = 0x14U,
    LICENSE_UART_RECV_TIMEOUT              = 0x15U,
    LICENSE_UART_CHECKSUM_FAIL             = 0x16U,
    LICENSE_CMD_ERR                        = 0x17U, 
    LICENSE_NET_POINTER_NULL               = 0x18U,
    LICENSE_NET_CREATE_CON_FAIL            = 0x19U,
    LICENSE_NET_SEND_MSG_FAIL              = 0x1AU,
    LICENSE_NET_RECV_TIME_OUT              = 0x1BU,
    LICENSE_NET_RECV_CMD_INVALID           = 0x1CU,
    LICENSE_NET_RECV_CPID_INVALID          = 0x1DU,
    LICENSE_NET_LIC_VERIFY_FAIL            = 0x1EU,
    LICENSE_NET_LIC_WRITE_FAIL             = 0x1FU,
    LICENSE_NET_LIC_MAKE_FAIL              = 0x20U,
    LICENSE_READ_DID_FAIL                  = 0x21U,
}lic_errorcode;

#endif
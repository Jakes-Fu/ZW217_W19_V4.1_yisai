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
 * @file        cms_lic_interface.h
 *
 * @brief       Provides license component basic definition.
 *
 * @revision
 * Date         Author          Notes
 * 2021-12-01   OneOS Team      First version.
 ***********************************************************************************************************************/
#ifndef __ONEOS_LICENSE_H__
#define __ONEOS_LICENSE_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#include "cms_license.h"
#include "cms_lic_net.h"

int cms_lic_license_verify(uint16_t componentid, uint8_t *key_para, uint16_t para_len);

/*
    input:
            componentid --组件id
            inbuf       --缓冲区地址
            bufsize     --缓冲区大小

    返回值： 0-成功(成功, output为输出结果)；其他-失败(失败, output为无效值)

    output:
            inbuf       --lic地址
            licsize     --lic大小
*/
int cms_lic_get_external_lic(uint16_t componentid, uint8_t *inbuf, uint16_t bufsize, uint16_t *licsize);


#if defined(__cplusplus)
}
#endif
#endif






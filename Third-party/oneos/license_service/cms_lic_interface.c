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
 * @file        cms_lic_interface.c
 *
 * @brief       Provides api for license verify function.
 *
 * @revision
 * Date         Author          Notes
 * 2021-12-01   OneOS Team      First version.
 ***********************************************************************************************************************/
#include "cms_lic_err.h" 
#include "cms_lic_interface.h"

//#include "cms_lic_net.h"

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
int cms_lic_get_external_lic(uint16_t componentid, uint8_t *inbuf, uint16_t bufsize, uint16_t *licsize)
{
    int ret = 0;
    int cnt = 1;

    if(inbuf == NULL){
        return LICENSE_NULL_POINTER;
    }

    g_license.lic = inbuf;
    g_license.len = bufsize;
    
    //第三方license
    if (componentid == 1003){
        while(cnt < 4){
            ret = lic_online_apply_license(componentid);
            if(ret){
                //printf("the %d times apply license failed. ret = %d \r\n",cnt,ret);
                LOG_D(DBG_TAG,"the %d times apply license failed. ret = %d \r\n",cnt,ret);
                cnt++;
                continue;
            }
            //success
            *licsize = g_license.len;
            break;
         }
    }
    return ret;
}




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
 * @file        cms_lic_online.c
 *
 * @brief       Provides apply license online.
 *
 * @revision
 * Date         Author          Notes
 * 2021-12-1    OneOS Team      First Version
 ***********************************************************************************************************************/

#include "cms_lic_net.h"
#include "cms_id.h"
#include "cms_lic_err.h"
#include "os_mutex.h"
#include "os_types.h"


#ifndef CMS_ID_WAIT_TIME_OUT
#define CMS_ID_WAIT_TIME_OUT 10000
#endif

#ifndef CON_BUFF_SIZE
#define CON_BUFF_SIZE       1056
#endif

#define CMS_ID_SERVICEID_LICENSE  1005

#define CMD_VER             0x00
#define CMD_REQ_LIC         0x03
#define CMD_RESP_LIC        0x83
#define CMD_VERIFY_NOTICE   0x43

#define PAYLOAD_CMD_SIZE        2
#define PAYLOAD_LEN_SIZE        2
#define PAYLOAD_CMPTID_SIZE     2
#define PAYLOAD_STATUS_SIZE     2


RTC_LIC_ST g_license = {0};


#if defined(CMS_CONNECT_TCP) || defined(CMS_CONNECT_MQTT)
#include "cms_con_tcp.h"
static void *cms_connect_init(uint16_t scode)
{
    void *handle = cms_tcp_init(scode, CON_BUFF_SIZE);
    if (cms_tcp_connect(handle))
    {
        LOG_E(DBG_TAG,"connect failed.");
        cms_tcp_deinit(handle);
        handle = NULL;
    }
    return handle;
}
#define cms_close_connect(handle)                       cms_tcp_deinit(handle)
#define cms_send_msg(handle, buf, len)                  cms_tcp_send(handle, buf, len)
#define cms_recv_msg(handle, buff, max_length, timeout) cms_tcp_recv(handle, buff, max_length, timeout)

#elif defined(CMS_CONNECT_UDP) || defined(CMS_CONNECT_COAP) || defined(CMS_CONNECT_LWM2M)
#include "cms_con_udp.h"
#define cms_connect_init(scode)                         cms_udp_init(scode, CON_BUFF_SIZE)
#define cms_close_connect(handle)                       cms_udp_deinit(handle)
#define cms_send_msg(handle, buf, len)                  cms_udp_send(handle, buf, len)
#define cms_recv_msg(handle, buff, max_length, timeout) cms_udp_recv(handle, buff, max_length, timeout)
#endif


int lic_online_apply_external_req(uint16_t component_id, uint8_t *msg, uint8_t len)
{
    uint8_t pos = 0;
    int ret = 0;
    int did_len = 0;

    if(NULL == msg){
        return LICENSE_NET_POINTER_NULL;
    }

    memset(msg, 0, len);

    msg[pos++] = CMD_VER;
    msg[pos++] = CMD_REQ_LIC;

    msg[pos++] = 0;
    msg[pos++] = len - PAYLOAD_CMD_SIZE - PAYLOAD_LEN_SIZE;

    msg[pos++] = (uint8_t)(component_id >> 8);
    msg[pos++] = (uint8_t)(component_id & 0x00FF);

    did_len = len - PAYLOAD_CMD_SIZE - PAYLOAD_LEN_SIZE - PAYLOAD_CMPTID_SIZE;
    ret = cms_id_get_id(DID, &msg[pos], did_len);
    if(ret){
        return LICENSE_READ_DID_FAIL;
    }

    return ret;
}



int lic_onelince_handle_resp(uint16_t component_id, uint8_t *payload, size_t payload_len)
{
    uint8_t pos = 0;
    uint8_t key_para[KEY_PARA_MAX_LEN] = {0};
    uint16_t size = 0;
    uint16_t lic_size = 0;
    uint16_t comptid = 0;
    int ret = 0;
    uint8_t *msg = payload;

    /*1. Data analysis*/
    pos++;
    if(*(msg+pos) != CMD_RESP_LIC){
        return LICENSE_NET_RECV_CMD_INVALID;
    }
    pos++;

    size = *(msg+pos) << 8;
    pos++;
    size |= *(msg+pos);
    pos++;
    lic_size = size - 2;

    comptid = *(msg+pos) << 8;
    pos++;
    comptid |= *(msg+pos);
    pos++;
    if(comptid != component_id ){
        return LICENSE_NET_RECV_CPID_INVALID;
    }

    if(component_id == COMPONENT_RTC){
        LOG_I(DBG_TAG,"g_license.len[%d],lic_size[%d]", g_license.len,lic_size);
        if(g_license.len < lic_size){
            return LICENSE_LIC_SIZE_INVALID;
        }
        g_license.len = lic_size;
        memset(g_license.lic, 0, lic_size);
        memcpy(g_license.lic, msg+pos, lic_size);
    }
    return ret;
}

void lic_online_verify_notice(uint16_t component_id, uint8_t *msg, uint8_t len, int status)
{
    uint8_t pos = 0;

    if(NULL == msg){
        return;
    }

    memset(msg, 0, len);

    /* command */
    msg[pos++] = CMD_VER;
    msg[pos++] = CMD_VERIFY_NOTICE;

    /* length */
    msg[pos++] = 0;
    msg[pos++] = len - PAYLOAD_CMD_SIZE - PAYLOAD_LEN_SIZE;

    /*payload */
    msg[pos++] = (uint8_t)(component_id >> 8);
    msg[pos++] = (uint8_t)(component_id & 0xFF);

    msg[pos++] = (uint8_t)((status >> 8)  & 0xFF);
    msg[pos++] = (uint8_t)(status         & 0xFF);
}


int lic_online_apply_func(void *handle, uint16_t component_id)
{
    uint8_t msglen = 0;
    uint8_t reg_msg[CON_BUFF_SIZE] = {0};
    int ret, recvsize,sendsize = 0;
    size_t did_len = 0;

    /* send apply license apply.*/
    ret = cms_id_get_id_len(DID, &did_len);
    if(ret){
        return LICENSE_READ_DID_FAIL;
    }
    LOG_I(DBG_TAG,"did_len[%d]",did_len);
    msglen = PAYLOAD_CMD_SIZE + PAYLOAD_LEN_SIZE + PAYLOAD_CMPTID_SIZE + did_len;
    uint8_t msg[msglen];
    ret = lic_online_apply_external_req(component_id, msg, msglen);
    if(ret){
        LOG_E(DBG_TAG,"lic_online: get apply msg failed. ");
        return ret;
    }

    sendsize = cms_send_msg(handle, msg, msglen);
    if(sendsize != msglen){
        return LICENSE_NET_SEND_MSG_FAIL;
    }

    memset(reg_msg, 0, CON_BUFF_SIZE);
    recvsize = cms_recv_msg(handle, reg_msg, CON_BUFF_SIZE, CMS_ID_WAIT_TIME_OUT);
    if(recvsize <= 0){
        /* recv timeout */
        LOG_E(DBG_TAG,"recv timeout.");
        ret = LICENSE_NET_RECV_TIME_OUT;
    }
    else{
        /*recv license resp.*/
        LOG_I(DBG_TAG,"component_id[%d]", component_id);
        if(recvsize >= 10){
            ret = lic_onelince_handle_resp(component_id, reg_msg, recvsize);

            msglen = PAYLOAD_CMD_SIZE + PAYLOAD_LEN_SIZE + PAYLOAD_CMPTID_SIZE + PAYLOAD_STATUS_SIZE;
            uint8_t replymsg[msglen];
            lic_online_verify_notice(component_id, replymsg, msglen, ret);
            sendsize = cms_send_msg(handle, replymsg, msglen);
            if(sendsize != msglen){
                ret = LICENSE_NET_SEND_MSG_FAIL;
            }
        }
        else{
            /* wrong status，flow end */
            ret = LICENSE_NET_LIC_MAKE_FAIL;
        }
    }
    return ret;
}

uint8_t g_init_flag = 0xff;
static os_mutex_t *g_cms_lic_mutex = NULL;
void lic_online_init(void)
{
    //g_cms_lic_mutex = mutex_create();
    g_cms_lic_mutex = os_mutex_create("cms_lic", OS_INHERIT);
}

int lic_online_apply_license(uint16_t component_id)
{
    int ret = 0;
    uint16_t regscode  = CMS_ID_SERVICEID_LICENSE;
    void *   conhandle = NULL;

    /* 0.initialize the lock */
    if(0xff == g_init_flag){
        lic_online_init();
        g_init_flag = 0;
    }

    //mutex_lock(g_cms_lic_mutex);
    os_mutex_recursive_lock(g_cms_lic_mutex, OS_WAIT_FOREVER);

    /* 1.create connect */
    conhandle = cms_connect_init(regscode);
    if(NULL == conhandle){
        return LICENSE_NET_CREATE_CON_FAIL;
    }

    /* 2.start up apply license onlince */
    ret = lic_online_apply_func(conhandle, component_id);

    /* 3.close connect */
    cms_close_connect(conhandle);

    //mutex_unlock(g_cms_lic_mutex);
    os_mutex_recursive_unlock(g_cms_lic_mutex);

    return ret;
}

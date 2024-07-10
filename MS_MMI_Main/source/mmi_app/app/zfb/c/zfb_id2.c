/*************************************************************************
 ** File Name:      mmicc_id.c                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "zfb_app.h"
#include "zfb_id2.h"
#include "id2_client.h"
#include "zdthttp_app.h"
#include "cJSON.h"
#include "alipay_vendor.h"

const char *zfb_token = "i136GFe9xioO8hFUi0I06375BDBDDFCB";
const char *zfb_pk = "EJMZkxFuSKX";
const char *zfb_accesskey = "LTAI5t6ReesDdGErU6qG8rST";
const char *zfb_accesssec = "s8TzZIGTwUMzfoux0IzPH0cVQK0FAN";
const char *zfb_id2_url = "http://card.ecellsz.com:8100/util/OtpGetId2?";

static BOOLEAN s_zfb_id2_is_init_ok = FALSE;
static uint8 s_zfb_id2_getotp_timer_id = 0;

//
/*
调试用接口
productKey：“r5oWHGVkfIw”
wcoJgbhaWRqzmWBqZ9VebQ==
*/
/*
* 注意：
*     空发流程需要跟服务端交互，这里的demo仅用于介绍空发流程中 设备端对接步骤，demo本身无法直接运行
*/
#if 0
int id2_client_otp_entry(char *token)
{
    int ret;
    bool is_prov;
    uint8_t* auth_code = NULL;
    uint32_t auth_code_len = 256;
    uint8_t* base64_otp_auth_code = NULL;
    uint32_t base64_auth_code_len;

    // Step1：初始化
    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    // Step2：查询当前的烧录状态。已烧录则终止流程
    ret = id2_client_get_prov_stat(&is_prov);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client get prov stat fail, %d\r\n", ret);
        goto _out;
    }
    if (is_prov == true) {
        ID2_DBG_LOG("id2 has provisioned.\n\n");
        goto _out;
    }

    // Step3：生成”空发认证码“。注意：生成的”空发认证码“需要做一次base64编码
    auth_code = (uint8_t *)ls_osa_malloc(256);
    ret = id2_client_get_otp_auth_code((uint8_t *)token, strlen(token), auth_code, &auth_code_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 get otp auth code fail, %d\n", ret);
        goto _out1;
    } else {
        base64_otp_auth_code = (uint8_t *)ls_osa_malloc(384);
        base64_auth_code_len = 384;
        ret = plat_base64_encode(auth_code, auth_code_len, base64_otp_auth_code, &base64_auth_code_len);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 get_base64_otp_auth_code fail, %d\n", ret);
            goto _out2;
        }
        if (base64_auth_code_len < 384) {
            base64_otp_auth_code[base64_auth_code_len] = '\0';
        }
        ID2_DBG_LOG("id2 get_otp_auth_code success. auth_code:%s\n", (char *) base64_otp_auth_code);
    }

    // Step4：将空发认证码(base64_otp_auth_code)上传到服务端，向ID2服务申请烧录数据otp_data

    // Step5：将服务端返回的烧录数据otp_data保存到本地。注意：Step3和Step5需要在一个进程中完成
    //  以下otp_data已失效，load_otp_data会失败。厂商对接时需要使用服务端返回的otp_data
    char *otp_data = "AzAwMEZGRkZGREIxRDhEMTFGM0REMDIwMLkqcceGX003DLQa8DqOwUUdpXj0bUYVvVvoY2ACQU1sKxbHfoykpP62fQSTo2UXpBeb5HBf8MCFMv7ixBa40dDmf/hvSpEWLdeX3rQU3jii";
    uint32_t base64_otp_data_len = strlen(otp_data);
    uint8_t *raw_otp_data = NULL;
    uint32_t raw_otp_data_len;
    if (base64_otp_data_len > 0) {
        raw_otp_data = ls_osa_malloc(256);
        raw_otp_data_len = 256;
        ret = plat_base64_decode((uint8_t *)otp_data, base64_otp_data_len, raw_otp_data, &raw_otp_data_len);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 otp_data_base64_decode_fail, %d\n", ret);
            goto _out3;
        }
        ret = id2_client_load_otp_data(raw_otp_data, raw_otp_data_len);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 client_load_otp_data, %d\n", ret);
            goto _out3;
        }
        
        // Step6：查询烧录结果
        bool is_prov = false;
        ret = id2_client_get_prov_stat(&is_prov);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 client get prov stat fai after load, %d\r\n", ret);
            goto _out3;
        }
        if (is_prov == false) {
            ID2_DBG_LOG("id2 client get prov stat fai after load, %d\r\n", ret);
            ret = -1;
            goto _out3;
        }

        ID2_DBG_LOG("id2 id2_client_load_otp_data success.\n");
    }
    
_out3:
    if (NULL != raw_otp_data) {
        ls_osa_free(raw_otp_data);
    }
    
_out2:
    if (NULL != base64_otp_auth_code) {
        ls_osa_free(base64_otp_auth_code);
    }

_out1:
    if (NULL != auth_code) {
        ls_osa_free(auth_code);
    }
_out:
    id2_client_cleanup();

    return ret == IROT_SUCCESS ? 0 : -1;
}
#endif

LOCAL BOOLEAN is_unreserved_char(uint8 c)
{
    if( (c>='0'&&c<='9') ||
        (c>='a'&&c<='z') ||
        (c>='A'&&c<='Z') ||
        c=='-' || c=='_' || c=='.' /*|| c=='/'|| c=='!' || c=='~' || c=='*' || c=='\''  || c=='(' || c==')'*/) 
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL uint32 encode_url(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr)
{
    uint8 c = 0;
    uint32 ret_len = 0;

    if(PNULL == dst_ptr || PNULL == url_ptr)
    {
        return 0;
    }

    while (0 != (c = *url_ptr++))
    {
        if (is_unreserved_char(c))
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = c;
            }
            else
            {
                break;
            }
        }
        else if (' ' == c)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = '+';
            }
            else
            {
                break;
            }
        }
        else
        {
            char str[10] = {0};
            uint16 len = 0;

            sprintf(str, "%02X", c);
            len = strlen(str);

            if (ret_len + len <= dst_len)
            {
                dst_ptr[ret_len ++] = '%';

                SCI_MEMCPY(dst_ptr+ ret_len, str, len);

                ret_len += len;
            }
            else
            {
                break;
            }
        }
    }

    return ret_len;
}

static void ZFB_ID2_SendTimer_Handle(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(timer_id == s_zfb_id2_getotp_timer_id && 0 != s_zfb_id2_getotp_timer_id)
    {
        MMK_StopTimer(s_zfb_id2_getotp_timer_id);
        s_zfb_id2_getotp_timer_id = 0;
    }
    APP_SendSigTo_Pay(SIG_ZFB_INIT_ID2_GET_OTP_CFG,PNULL,0);
}

static void ZFB_ID2_SendTimer_Start(uint32 time_ms)
{
    if(0 != s_zfb_id2_getotp_timer_id)
    {
        MMK_StopTimer(s_zfb_id2_getotp_timer_id);
        s_zfb_id2_getotp_timer_id = 0;
    }
    s_zfb_id2_getotp_timer_id = MMK_CreateTimerCallback(time_ms, 
                                                                        ZFB_ID2_SendTimer_Handle, 
                                                                        PNULL, 
                                                                        FALSE);
}

static void ZFB_ID2_SendTimer_Stop(void)
{
    if(0 != s_zfb_id2_getotp_timer_id)
    {
        MMK_StopTimer(s_zfb_id2_getotp_timer_id);
        s_zfb_id2_getotp_timer_id = 0;
    }
}

int ZFB_HTTP_OtpGetId2_Send_CB(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint32 get_ok = 0;
    int ret = -1;
    SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send_CB(%d) Rcv_len=%d,%s",is_ok,Rcv_len,pRcv);
    ZFB_ID2_SendTimer_Stop();
    if(is_ok)
    {
        if(pRcv != NULL && Rcv_len > 2)
        {
            int recode = 0;
            int remain = 0;
            char *msg_char;
            uint16 msg_len = 0;
            cJSON *json = PNULL;
            cJSON *item_json = PNULL;
        	json=cJSON_Parse(pRcv);
        	if (json) 
        	{
                item_json = cJSON_GetObjectItem(json,"code");
                if(item_json)
                {
                    recode = cJSON_GetObjectInt(item_json);
                    SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send_CB code=%d",recode);
                }
                else
                {
                    SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send_CB ERR  item code");
                }
                if(recode == 200)
                {
                    cJSON *data_json = PNULL;
                    item_json = cJSON_GetObjectItem(json,"data");
                    if(item_json)
                    {
                        data_json = cJSON_GetObjectItem(item_json,"provisionData");
                        if(data_json)
                        {
                            msg_char = cJSON_GetObjectString(data_json);
                            if(msg_char)
                            {
                                msg_len = strlen(msg_char);
                                SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send_CB msg_len=%d",msg_len);
                                if(msg_len > 0)
                                {
                                    ZFB_DB_PROV_Set(msg_char,msg_len);
                                    get_ok = 1;
                                    ret = 0;
                                }
                            }
                        }
                        data_json = cJSON_GetObjectItem(item_json,"remaining");
                        if(data_json)
                        {
                            remain = cJSON_GetObjectInt(data_json);
                            SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send_CB remaining=%d",remain);
                        }  
                    }
                }
                cJSON_Delete(json);
        	}
            else
            {
                SCI_TRACE_LOW("YJ_HTTP_Send_Notify_CB ERR  cJSON_Parse");
            }
        }
    }
    
    APP_SendSigTo_Pay(SIG_ZFB_INIT_ID2_GET_OTP_CFG,PNULL,get_ok);
    return 0;
}

BOOLEAN  ZFB_HTTP_OtpGetId2_Send(uint8 * base64_otp_auth_code)
{
    BOOLEAN ret = FALSE;
    char send_buf[1024] = {0};
    uint16 len = 0;
    uint32 encode_len = 0;
    SCI_TRACE_LOW("ZFB_HTTP_OtpGetId2_Send");
    ZDT_HTTPSendTrace(base64_otp_auth_code,strlen(base64_otp_auth_code));
    sprintf(send_buf,"deviceAuthCode=");
    len = strlen(send_buf);
    encode_len = encode_url(send_buf+len,1024-len, base64_otp_auth_code);
    len += encode_len;
    ret = MMIZDT_HTTP_AppSend(TRUE,(char *)zfb_id2_url,(uint8 *)send_buf,len,1000,0,3,0,PNULL,0,ZFB_HTTP_OtpGetId2_Send_CB);
    if(ret)
    {
        ZFB_ID2_SendTimer_Start(25000);
    }
    return ret;
}

int zfb_id2_client_init(void)
{
    int ret = IROT_ERROR_ITEM_NOT_FOUND;
    bool is_prov;
    uint8_t* auth_code = NULL;
    uint32_t auth_code_len = 256;
    uint8_t* base64_otp_auth_code = NULL;
    uint32_t base64_auth_code_len;
    ID2_DBG_LOG("zfb_id2_client_init start");
    // Step1：初始化
    #ifdef WIN32
    ret = IROT_SUCCESS;
    return ret;
    #else
    ret = id2_client_init();
    #endif
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2_client_init fail init, %d", ret);
        return -1;
    }
    ID2_DBG_LOG("zfb_id2_client_init Step1");
    
    // Step2：查询当前的烧录状态。已烧录则终止流程
    #ifdef WIN32
    ret = IROT_SUCCESS;
    is_prov = true;
    #else
    ret = id2_client_get_prov_stat(&is_prov);
    #endif
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client get prov stat fail, %d\r\n", ret);
    #ifndef WIN32
        id2_client_cleanup();
    #endif
        return -1;
    }
    ID2_DBG_LOG("zfb_id2_client_init Step2");
    if (is_prov == true) {
        ID2_DBG_LOG("id2 has provisioned.OK");
        s_zfb_id2_is_init_ok = TRUE;
    #ifndef WIN32
        id2_client_cleanup();
    #endif
        return 0;
    }
    ID2_DBG_LOG("zfb_id2_client_init Step3");
    if(ZFB_DB_PROV_GetLen() > 0)
    {
        ID2_DBG_LOG("zfb_id2_client_init ProvData Exsit");
        if(0 == zfb_id2_client_cleanup())
        {
            return 0;
        }
        else
        {
            ZFB_DB_PROV_ListDel();
        }
        return 2;
    }
    // Step3：生成”空发认证码“。注意：生成的”空发认证码“需要做一次base64编码
    auth_code = (uint8_t *)ls_osa_malloc(256);
    #ifdef WIN32
        ret = IROT_SUCCESS;
    #else
        ret = id2_client_get_otp_auth_code((uint8_t *)zfb_token, strlen(zfb_token), auth_code, &auth_code_len);
    #endif
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 get otp auth code fail, %d\n", ret);
        if(auth_code != NULL)
        {
            ls_osa_free(auth_code);
        }
    #ifndef WIN32
        id2_client_cleanup();
    #endif
        return -1;
    } else {
        //ZFB_HexTraceU8(auth_code,auth_code_len);
        base64_otp_auth_code = (uint8_t *)ls_osa_malloc(385);
        base64_auth_code_len = 384;
        ret = plat_base64_encode(auth_code, auth_code_len, base64_otp_auth_code, &base64_auth_code_len);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 get_base64_otp_auth_code fail, %d\n", ret);
            if(auth_code != NULL)
            {
                ls_osa_free(auth_code);
            }
            if(base64_otp_auth_code != NULL)
            {
                ls_osa_free(base64_otp_auth_code);
            }
    #ifndef WIN32
            id2_client_cleanup();
    #endif
            return -1;
        }
        if (base64_auth_code_len < 385) {
            base64_otp_auth_code[base64_auth_code_len] = '\0';
        }
        ID2_DBG_LOG("id2 get_otp_auth_code success. auth_code:%s\n", (char *) base64_otp_auth_code);
        //ZFB_HexTraceU8(base64_otp_auth_code,base64_auth_code_len);
    }
    ID2_DBG_LOG("id2_client_test Step3 ret=%d,code_len=%d,%d",ret,auth_code_len,base64_auth_code_len);
    if(auth_code != NULL)
    {
        ls_osa_free(auth_code);
    }
    // Step4：将空发认证码(base64_otp_auth_code)上传到服务端，向ID2服务申请烧录数据otp_data
    if(ZFB_HTTP_OtpGetId2_Send(base64_otp_auth_code))
    {
        if(base64_otp_auth_code != NULL)
        {
            ls_osa_free(base64_otp_auth_code);
        }
        return 1;
    }

    if(base64_otp_auth_code != NULL)
    {
        ls_osa_free(base64_otp_auth_code);
    }
    #ifndef WIN32
    id2_client_cleanup();
    #endif
    return ret == IROT_SUCCESS ? 0 : -1;
}

int zfb_id2_client_cleanup(void)
{
    char *otp_data = ZFB_DB_PROV_Get();
    int ret = IROT_ERROR_ITEM_NOT_FOUND;
    uint32_t base64_otp_data_len = 0;
    uint8_t *raw_otp_data = NULL;
    uint32_t raw_otp_data_len;
    bool is_prov = false;
    if(otp_data == NULL)
    {
        ID2_DBG_LOG("zfb_id2_client_cleanup fail no otp");
    #ifndef WIN32
        id2_client_cleanup();
    #endif
        return -1;
    }
    ID2_DBG_LOG("zfb_id2_client_cleanup start");
    base64_otp_data_len = strlen(otp_data);
    if (base64_otp_data_len > 0)
    {
        //ZFB_ID2TraceExt(otp_data,base64_otp_data_len);
        raw_otp_data = ls_osa_malloc(256);
        raw_otp_data_len = 256;
        ret = plat_base64_decode((uint8_t *)otp_data, base64_otp_data_len, raw_otp_data, &raw_otp_data_len);
        if (ret != IROT_SUCCESS) 
        {
            ID2_DBG_LOG("id2 otp_data_base64_decode_fail, %d\n", ret);
        }
        else
        {
            //ZFB_ID2TraceExt(raw_otp_data,raw_otp_data_len);
#ifdef WIN32
            ret = IROT_SUCCESS;
#else
            ret = id2_client_load_otp_data(raw_otp_data, raw_otp_data_len);
#endif
            if (ret != IROT_SUCCESS) 
            {
                ID2_DBG_LOG("id2 client_load_otp_data, %d\n", ret);
            }
            else
            {
                // Step6：查询烧录结果
    #ifdef WIN32
                ret = IROT_SUCCESS;
    #else
                ret = id2_client_get_prov_stat(&is_prov);
    #endif
                if (ret != IROT_SUCCESS) 
                {
                    ID2_DBG_LOG("id2 client get prov stat fai after load, %d\r\n", ret);
                }
                else
                {
                    if (is_prov == false) 
                    {
                        ID2_DBG_LOG("id2 client get prov stat fai after load, %d\r\n", ret);
                        ret = -1;
                    }
                    else
                    {
                        s_zfb_id2_is_init_ok = TRUE;
                        ID2_DBG_LOG("id2 id2_client_load_otp_data success.\n");
                    }
                }
            }
        }
    }

    if (NULL != raw_otp_data) {
        ls_osa_free(raw_otp_data);
    }
#ifndef WIN32
    id2_client_cleanup();
#endif
    return ret == IROT_SUCCESS ? 0 : -1;
}

BOOLEAN zfb_id2_client_is_ok(void)
{
    return s_zfb_id2_is_init_ok;
}

#if 0
/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#define ID2_AUTH_CODE_BUF_LEN   512

int id2_client_unit_test(void);
int id2_client_otp_demo(char *token);
int id2_client_generate_authcode(void);
int id2_client_decrypt_data(char *cipher_data, uint32_t cipher_len);

extern int ls_string_to_hex(const uint8_t* in, uint32_t in_len, uint8_t* out, uint32_t* out_len);

static int id2_client_test_get_id(void)
{
    irot_result_t ret;
    uint32_t version = 0;
    uint32_t id2_len = 0;
    uint8_t id2[ID2_ID_LEN + 1] = {0};

    ID2_DBG_LOG("====> ID2 Client Test Get ID Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    ret = id2_client_get_version(&version);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client get version fail\n");
        goto _out;
    }

    ret = id2_client_get_id(id2, &id2_len);
    if (ret != IROT_ERROR_SHORT_BUFFER ||
        id2_len != ID2_ID_LEN) {
        ID2_DBG_LOG("get client id2 fail, %d\n", ret);
        ret = IROT_ERROR_GENERIC;
        goto _out;
    }

    ret = id2_client_get_id(id2, &id2_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get client id2 fail, %d\n", ret);
        goto _out;
    }

    ret = id2_client_get_id(id2, &id2_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get client id2 fail, %d\n", ret);
        goto _out;
    }

    if (id2_len != ID2_ID_LEN) {
        ID2_DBG_LOG("invalid id2 length, %d\n", id2_len);
        goto _out;
    }

    ID2_DBG_LOG("ID2: %s\n", id2);

_out:
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Get ID End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_get_challenge_auth_code(void)
{
    irot_result_t ret;
    uint32_t auth_code_len = 0;
    uint8_t auth_code[ID2_AUTH_CODE_BUF_LEN] = {0};
    char *server_random = "55B83408399FA660F05C82E4F25333DC";
    char *extra = "abcd1234";

    ID2_DBG_LOG("====> ID2 Client Test Get Challenge Auth Code Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    ret = id2_client_get_challenge_auth_code(
              server_random, NULL, 0, auth_code, &auth_code_len);
    if (ret != IROT_ERROR_SHORT_BUFFER) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        ret = IROT_ERROR_GENERIC;
        goto _out;
    }

    ret = id2_client_get_challenge_auth_code(
              server_random, NULL, 0, auth_code, &auth_code_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        goto _out;
    }

    ID2_DBG_LOG("authcode[no extra]:\n [%d] %s\n", auth_code_len, auth_code);

    auth_code_len = ID2_AUTH_CODE_BUF_LEN;
    ret = id2_client_get_challenge_auth_code(server_random,
                     (uint8_t *)extra, strlen(extra), auth_code, &auth_code_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        goto _out;
    }

    ID2_DBG_LOG("authcode[extra = %s]:\n [%d] %s\n", extra, auth_code_len, auth_code);

_out:
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Get Challenge Auth Code End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_get_timestamp_auth_code(void)
{
    irot_result_t ret;
    uint32_t auth_code_len = 0;
    uint8_t auth_code[ID2_AUTH_CODE_BUF_LEN] = {0};
    char *timestamp = "1605617436545";
    char *extra = "abcd1234";

    ID2_DBG_LOG("====> ID2 Client Test Get Timestamp Auth Code Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    ret = id2_client_get_timestamp_auth_code(
              timestamp, NULL, 0, auth_code, &auth_code_len);
    if (ret != IROT_ERROR_SHORT_BUFFER) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        ret = IROT_ERROR_GENERIC;
        goto _out;
    }

    ret = id2_client_get_timestamp_auth_code(
              timestamp, NULL, 0, auth_code, &auth_code_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        goto _out;
    }

    ID2_DBG_LOG("authcode[no extra]:\n [%d] %s\n", auth_code_len, auth_code);

    auth_code_len = ID2_AUTH_CODE_BUF_LEN;
    ret = id2_client_get_timestamp_auth_code(timestamp,
                     (uint8_t *)extra, strlen(extra), auth_code, &auth_code_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("get challenge auth code fail, %d\n", ret);
        goto _out;
    }

    ID2_DBG_LOG("authcode[extra = %s]:\n [%d] %s\n", extra, auth_code_len, auth_code);

_out:
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Get Timestamp Auth Code End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_get_device_challenge(void)
{
    irot_result_t ret;
    uint32_t i = 0;
    uint32_t random_len = 0;
    uint8_t random[ID2_MAX_DEVICE_RANDOM_LEN + 4] = {0};

    ID2_DBG_LOG("====> ID2 Client Test Get Device Challenge Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    for (i = 1; i < ID2_MAX_DEVICE_RANDOM_LEN + 4; i += 4) {
        random_len = i;
        ret = id2_client_get_device_challenge(random, &random_len);
        if (ret != IROT_SUCCESS) {
            ID2_DBG_LOG("id2 client get device challenege fail, %d\n", ret);
            goto _out;
        }

        ID2_DBG_LOG("device random: [%d] %s\n", random_len, random);
        ls_osa_msleep(10);
    }

_out:
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Get Device Challenege End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_get_secret(void)
{
    irot_result_t ret;
    uint32_t secret_len = 0;
    uint8_t secret[ID2_DERIV_SECRET_LEN + 1] = {0};
    const char* seed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    ID2_DBG_LOG("====> ID2 Client Test Get Secret Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    ret = id2_client_get_secret(seed, secret, &secret_len);
    if (ret != IROT_ERROR_SHORT_BUFFER ||
        secret_len != ID2_DERIV_SECRET_LEN) {
        ID2_DBG_LOG("get client secret fail, %d\n", ret);
        ret = IROT_ERROR_GENERIC;
        goto _out;
    }

    ret = id2_client_get_secret(seed, secret, &secret_len);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client get secret fail, %d\n", ret);
        goto _out;
    }

    ID2_DBG_LOG("device secret: [%d] %s\n", secret_len, secret);

_out:
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Get Secret End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_encrypt_and_decrypt(char* text, char* ciphertext)
{
    irot_result_t ret = IROT_SUCCESS;
    uint32_t text_len = strlen(text);
    uint32_t input_len;
    uint8_t* enc_out = NULL;
    uint8_t* dec_out = NULL;
    uint32_t enc_out_len;
    uint32_t decrypt_out_len;

    ID2_DBG_LOG("====> ID2 Client Test Encrypt/Decrypt Start.\n");

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    // =======================================start for decrypt prepare
    enc_out = (uint8_t *)ls_osa_malloc(strlen(ciphertext));
    enc_out_len = strlen(ciphertext);
    ret = ls_string_to_hex((const uint8_t*) ciphertext, strlen(ciphertext), enc_out, &enc_out_len);
    if (ret < 0) {
        ls_osa_print("cipher text string to hex error.\n");
        ls_osa_free(enc_out);
        return IROT_ERROR_BAD_PARAMETERS;
    }
    input_len = enc_out_len;
    // =======================================end for decrypt prepare

    // decrypt
    dec_out = (uint8_t *)ls_osa_malloc(input_len);
    decrypt_out_len = input_len;
    ret = id2_client_decrypt(enc_out, enc_out_len, dec_out, &decrypt_out_len);
    if (ret != IROT_SUCCESS) {
        ls_osa_print("id2 sym decrypt fail, %d.\n", ret);
        goto _out;
    }

    if (memcmp(text, dec_out, text_len) != 0) {
        ls_osa_print("signature compare error.\n");
        ret = IROT_ERROR_GENERIC;
        goto _out;
    }

_out:
    if (dec_out != NULL) {
        ls_osa_free(dec_out);
    }

    if (enc_out != NULL) {
        ls_osa_free(enc_out);
    }
    id2_client_cleanup();

    ID2_DBG_LOG("====> ID2 Client Test Encrypt/Decrypt End.\n");

    return ret == IROT_SUCCESS ? 0 : -1;
}

static int id2_client_test_get_device_info(void)
{
    retval_e ret;
    uint32_t buf_len = 17;
    size_t buf_size = 17;
    uint8_t dev_buf[16 + 1] = {0};

    ID2_DBG_LOG( "====> ID2 Alipay Test Get Device Info Start.\n");

    ret = alipay_get_mac(dev_buf, &buf_len);
    if (ret != RV_OK) {
        ID2_DBG_LOG("id2 alipay get mac fail, %d\n", ret);
        return -1;
    }
    ID2_DBG_LOG("Get Device Mac, %s\n", (char *)dev_buf);

    buf_size = 17;
    ret = alipay_get_imei((char *)dev_buf, &buf_size);
    if (ret != RV_OK) {
        ID2_DBG_LOG("id2 alipay get imei fail, %d\n", ret);
        return -1;
    }
    ID2_DBG_LOG("Get Device Imei, %s\n", (char *)dev_buf);

    buf_size = 17;
    ret = alipay_get_sn((char *)dev_buf, &buf_size);
    if (ret != RV_OK) {
        ID2_DBG_LOG("id2 alipay get sn fail, %d\n", ret);
        return -1;
    }
    ID2_DBG_LOG("Get Device Sn, %s\n", (char *)dev_buf);

    buf_size = 17;
    ret = alipay_get_model((char *)dev_buf, &buf_size);
    if (ret != RV_OK) {
        ID2_DBG_LOG("id2 alipay get model fail, %d\n", ret);
        return -1;
    }
    ID2_DBG_LOG("Get Device Model, %s\n", (char *)dev_buf);

    buf_size = 17;
    ret = alipay_get_product_key((char *)dev_buf, &buf_size);
    if (ret != RV_OK) {
        ID2_DBG_LOG("id2 alipay get product key fail, %d\n", ret);
        return -1;
    }
    ID2_DBG_LOG("Get Product Key, %s\n", (char *)dev_buf);

    ID2_DBG_LOG("====> ID2 Alipay Test Get Device Info End.\n");

    return ret == RV_OK ? 0 : -1;
}

static int id2_client_test_storage(void)
{
    retval_e ret;
    uint8_t * buf_content = PNULL;
    const char *content_string = "this is a string to text alipay storeage!";
    uint32_t str_len = strlen(content_string);
    uint32_t buf_len = str_len+1;
    ID2_DBG_LOG("====> ID2 Alipay Test Storage Info Start str_len=%d",str_len);

    ret = alipay_storage_write(ALIPAY_ITEM_SHARE_KEY, (const uint8_t *)content_string, str_len);
    if (ret != RV_OK) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Write Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Write Pass.\n\n");
    }
    buf_content = ls_osa_malloc(buf_len+1);
    SCI_MEMSET(buf_content,0,buf_len+1);
    ret = alipay_storage_read(ALIPAY_ITEM_SHARE_KEY, buf_content, &buf_len);
    if (ret != RV_OK) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Read Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Read Pass.%s",buf_content);
    }
    if (buf_len != str_len && 0 != strncmp((char *)buf_content, content_string,str_len)) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Write and Read Not Equal.\n\n");
        return -1;
    }

    ret = alipay_storage_exists(ALIPAY_ITEM_SHARE_KEY);
    if (ret != RV_OK) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Exist Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Exist Pass.\n\n");
    }

    ret = alipay_storage_delete(ALIPAY_ITEM_SHARE_KEY);
    if (ret != RV_OK) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Delete Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Delete Pass.\n\n");
    }
    ret = alipay_storage_exists(ALIPAY_ITEM_SHARE_KEY);
    if (ret != RV_NOT_FOUND) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Exist Fail After Delete.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storeage Exist Pass After Delete.\n\n");
    }

    ID2_DBG_LOG("====> ID2 Alipay Test Storage Info End.\n");

    return RV_OK;
}
/*
* 注意： 本测试程序仅用于设备端SDK部分接口功能验证，至于服务端challenge获取、authcode有效性验证等需要跟服务端配合完成
*/
int id2_client_unit_test(void)
{
    int ret;
    bool is_prov;
    char *text = "0123456789ABCDEF";
    char *ciphertext = PNULL;;

    ret = id2_client_init();
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client init fail, %d\n", ret);
        return -1;
    }

    ret = id2_client_get_prov_stat(&is_prov);
    if (ret != IROT_SUCCESS) {
        ID2_DBG_LOG("id2 client get prov stat fail, %d\r\n", ret);
        return -1;
    }

    if (is_prov == false) {
        ID2_DBG_LOG("no id2 in device, need to prov first!!\n");
        return -1;
    }

    ret = id2_client_test_get_id();
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Get ID Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Get ID Pass.\n\n");
    }

    ret = id2_client_test_get_challenge_auth_code();
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Get Challenge Auth Code Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Get Challenge Auth Code Pass.\n\n");
    }

    ret = id2_client_test_get_timestamp_auth_code();
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Get Timestamp Auth Code Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Get Timestamp Auth Code Pass.\n\n");
    }

    ret = id2_client_test_get_device_challenge();
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Get Device Challenge Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Get Device Challenge Pass.\n\n");
    }

    ret = id2_client_test_get_secret();
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Get Secret Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Get Secret Pass.\n\n");
    }

    // 注意：加密数据需要调用服务端接口获取，这里仅展示本地解密接口使用
    ciphertext = ZFB_DB_PROV_Get();
    ret = id2_client_test_encrypt_and_decrypt(text, ciphertext);
    if (ret < 0) {
        ID2_DBG_LOG("=================>ID2 Client Test Encrypt/Decrypt Fail.\n\n");
        //return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Client Test Encrypt/Decrypt Pass.\n\n");
    }

    ret = id2_client_test_get_device_info();
    if (ret != 0) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Get Device Info Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Get Device Info Pass.\n\n");
    }

    ret = id2_client_test_storage();
    if (ret != 0) {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storage Info Fail.\n\n");
        return -1;
    } else {
        ID2_DBG_LOG("=================>ID2 Alipay Test Storage Info Pass.\n\n");
    }

    return 0;
}
#endif

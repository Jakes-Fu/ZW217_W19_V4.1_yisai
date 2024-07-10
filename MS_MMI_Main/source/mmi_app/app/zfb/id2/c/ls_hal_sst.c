/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include "ls_hal.h"
#include "zfb_app.h"

#define KV_LIST_LEN   ZFB_DB_KV_MAX_SUM
#define SST_MAX_NAME_LEN  ZFB_DB_KV_MAX_KEY_SIZE

uint32_t ls_hal_kv_init(void)
{
    //ls_osa_print("ls_hal_kv_init start");
    return SST_HAL_SUCCESS;
}

void ls_hal_kv_deinit(void)
{
    //ls_osa_print("ls_hal_kv_deinit start");
    return;
}

uint32_t ls_hal_kv_set(const char *key, const void *value, uint32_t len)
{
    uint32 key_len;
    uint32 cur_key_len;
    int i;
    //ls_osa_print("ls_hal_kv_set start");
    if (NULL == key || NULL == value) 
    {
        ls_osa_print("invalid input args.\n");
        return SST_HAL_ERROR_BAD_PARAMETERS;
    }
    key_len = strlen(key);
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_set start len=%d,key=%s",len,key);
#endif
    if (SST_MAX_NAME_LEN < key_len) 
    {
        ls_osa_print("invalid key name length, %d", key_len);
        return SST_HAL_ERROR_BAD_PARAMETERS;
    }
    
    if(len > ZFB_DB_KV_MAX_VAL_SIZE)
    {
        ls_osa_print("KV Set ERR Val_len > %d",ZFB_DB_KV_MAX_VAL_SIZE);
        return SST_HAL_ERROR_SHORT_BUFFER;
    }		

    for (i = 0; i < KV_LIST_LEN; i++) 
    {
        if (g_kv_list[i].used) 
        {
            cur_key_len = strlen(g_kv_list[i].key);
            if(cur_key_len == key_len && 0 == strncmp(g_kv_list[i].key, key,key_len))
            {
                SCI_MEMSET(g_kv_list[i].value, 0, ZFB_DB_KV_MAX_VAL_SIZE+8);
                SCI_MEMCPY(g_kv_list[i].value, value, len);
                g_kv_list[i].len = len;
                g_kv_list[i].used = 1;
#ifdef ALIPAY_DEBUG
                ls_osa_print("set key[%d]=%s, val_len=%d",i,g_kv_list[i].key,g_kv_list[i].len);
                ZFB_HexTraceID2Set(g_kv_list[i].value,g_kv_list[i].len);
#endif
                ZFB_DB_KV_ListModify();
                return SST_HAL_SUCCESS;
            }
            else
            {
                continue;
            }
        }
        
        SCI_MEMSET(&g_kv_list[i], 0, sizeof(sst_kv_t));
        strncpy((char *)(g_kv_list[i].key), key, key_len);
        if(len > ZFB_DB_KV_MAX_VAL_SIZE)
        {
            ls_osa_print("KV Set ERR Val_len > %d",ZFB_DB_KV_MAX_VAL_SIZE);
            len = ZFB_DB_KV_MAX_VAL_SIZE;
        }
        SCI_MEMCPY(g_kv_list[i].value, value, len);
        g_kv_list[i].len = len;
        g_kv_list[i].used = 1;
#ifdef ALIPAY_DEBUG
        ls_osa_print("set key[%d]=%s, val_len=%d",i,g_kv_list[i].key,g_kv_list[i].len);
        ZFB_HexTraceID2Set(g_kv_list[i].value,g_kv_list[i].len);
#endif
        ZFB_DB_KV_ListModify();
        return SST_HAL_SUCCESS;
    }
	ls_osa_print("ls_hal_kv_set NO_SPACE");
    return SST_HAL_ERROR_STORAGE_NO_SPACE;
}

uint32_t ls_hal_kv_exist(const char *key)
{
    uint32 key_len;
    uint32 cur_key_len;
    int i;
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_exist start");
#endif
    if (NULL == key) {
        ls_osa_print("invalid input args.\n");
        return SST_HAL_ERROR_BAD_PARAMETERS;
    }
    key_len = strlen(key);
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_exist key=%s",key);
#endif
    for (i = 0; i < KV_LIST_LEN; i++) 
    {
        if (g_kv_list[i].used) 
        {
            cur_key_len = strlen(g_kv_list[i].key);
            if(cur_key_len == key_len && 0 == strncmp(g_kv_list[i].key, key,key_len))
            {
                ls_osa_print("ls_hal_kv_exist OK key[%d]=%s",i,key);
                return SST_HAL_SUCCESS;
            }
        }
    }
#ifdef ALIPAY_DEBUG
	ls_osa_print("ls_hal_kv_exist NOT_FOUND");
#endif
    return SST_HAL_ERROR_ITEM_NOT_FOUND;
}

uint32_t ls_hal_kv_get(const char *key, void *buffer, uint32_t *buffer_len)
{
    uint32 key_len;
    uint32 cur_key_len;
    int i;
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_get start");
#endif
    if (NULL == key || NULL == buffer_len) 
    {
        ls_osa_print("kv invalid input args.\n");
        return SST_HAL_ERROR_BAD_PARAMETERS;
    }
    key_len = strlen(key);
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_get len=%d,key=%s",*buffer_len,key);
#endif
    for (i = 0; i < KV_LIST_LEN; i++) 
    {
        if (g_kv_list[i].used) 
        {
            cur_key_len = strlen(g_kv_list[i].key);
            if(cur_key_len == key_len && 0 == strncmp(g_kv_list[i].key, key,key_len))
            {
                if(buffer == NULL || *buffer_len == 0 || *buffer_len < g_kv_list[i].len)
                {
#ifdef ALIPAY_DEBUG
                    ls_osa_print("kv short buffer, %d %d.\n", *buffer_len, g_kv_list[i].len);
#endif
                    *buffer_len = g_kv_list[i].len;
                    return SST_HAL_ERROR_SHORT_BUFFER;
                }
                *buffer_len = g_kv_list[i].len;
                SCI_MEMCPY(buffer, g_kv_list[i].value, g_kv_list[i].len);
#ifdef ALIPAY_DEBUG
                ls_osa_print("kv get key[%d]=%s, val_len=%d",i,g_kv_list[i].key,g_kv_list[i].len);
                ZFB_HexTraceID2Get(g_kv_list[i].value,g_kv_list[i].len);
#endif
                return SST_HAL_SUCCESS;
            }
        }
	}
#ifdef ALIPAY_DEBUG
	ls_osa_print("ls_hal_kv_get NOT_FOUND");
#endif
	return SST_HAL_ERROR_ITEM_NOT_FOUND;
}

uint32_t ls_hal_kv_del(const char *key)
{
    uint32 key_len;
    uint32 cur_key_len;
    int i;
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_del start");
#endif
    if (NULL == key) {
        ls_osa_print("invalid input args");
        return SST_HAL_ERROR_BAD_PARAMETERS;
    }
    key_len = strlen(key);
#ifdef ALIPAY_DEBUG
    ls_osa_print("ls_hal_kv_del key=%s",key);
#endif
    for (i = 0; i < KV_LIST_LEN; i++) 
    {
        if (g_kv_list[i].used) 
        {
            cur_key_len = strlen(g_kv_list[i].key);
            if(cur_key_len == key_len && 0 == strncmp(g_kv_list[i].key, key,key_len))
            {
                SCI_MEMSET(&g_kv_list[i], 0, sizeof(sst_kv_t));
                ZFB_DB_KV_ListModify();
#ifdef ALIPAY_DEBUG
                ls_osa_print("ls_hal_kv_del OK key[%d]=%s",i,key);
#endif
                break;
            }
        }
    }
#ifdef ALIPAY_DEBUG
	ls_osa_print("ls_hal_kv_del over");
#endif
    return SST_HAL_SUCCESS;
}


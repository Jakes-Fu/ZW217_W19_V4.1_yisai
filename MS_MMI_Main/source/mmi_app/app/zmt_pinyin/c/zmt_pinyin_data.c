/*****************************************************************************
** File Name:      zmt_pinyin_data.c                                              *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "cjson.h"
#include "dal_time.h"
#include "zmt_pinyin_main.h"
#include "zmt_pinyin_id.h"
#include "mbedtls/md5.h"

extern PINYIN_INFO_TEXT_T pinyin_info_text[PINYIN_ICON_LIST_ITEM_MAX][PINYIN_SHENG_ITEM_MAX];
extern PINYIN_INFO_NUM_T pinyin_info_num[PINYIN_ICON_LIST_ITEM_MAX];
extern PINYIN_AUDIO_INFO_T * pinyin_audio_info[PINYIN_AUDIO_ITEM_MAX];
extern int pinyin_request_status;
extern int pinyin_request_idx;
extern BOOLEAN pinyin_request_now;
extern PINYIN_READ_INFO_T pinyin_read_info;

// 判断字符是否需要被URL编码  
static int Pinyin_is_url_safe(char c)
{
    return isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~';
}

// 将单个字符编码为URL格式（%XX）  
static void Pinyin_encode_char(char c, char *dest)
{
    static const char hex[] = "0123456789ABCDEF";
    dest[0] = '%';
    dest[1] = hex[(c >> 4) & 0xF];
    dest[2] = hex[c & 0xF];
    dest[3] = '\0'; // 确保字符串以null终止（虽然这里只使用了前三个字符）  
}

// urlencode函数  
PUBLIC int Pinyin_urlencode(const char *src, char *dest, size_t dest_size)
{  
    const char *src_end = src + strlen(src);
    char encoded_char[4]; // 存储单个编码后的字符（包括'\0'）  
    char *p = dest;
    int len = 0;
  
    while (src < src_end && len < dest_size - 1) { // 保留一个字节给'\0'  
        if (Pinyin_is_url_safe(*src)) {  
            *p++ = *src++;  
            len++;  
        } else {  
            Pinyin_encode_char(*src++, encoded_char);  
            // 检查是否有足够的空间来存储编码后的字符  
            if (len + 3 > dest_size - 1) {  
                // 如果没有足够空间，则截断并返回错误  
                *p = '\0';  
                return -1; // 或其他错误代码  
            }  
            strcpy(p, encoded_char); // 使用strcpy是安全的，因为我们知道encoded_char的长度  
            p += 3;  
            len += 3;  
        }  
    }  

    *p = '\0'; // 确保字符串以null终止  
    return 1; // 成功  
}

PUBLIC void Pinyin_ReleaseAudioPath(void)
{
    uint8 i = 0;
    for(i = 0;i < PINYIN_AUDIO_ITEM_MAX;i++)
    {
        if(pinyin_audio_info[i] != NULL){
            if(pinyin_audio_info[i]->audio_uri != NULL){
                SCI_FREE(pinyin_audio_info[i]->audio_uri);
                pinyin_audio_info[i]->audio_uri = NULL;
            }
            if(pinyin_audio_info[i]->audio_data != NULL){
                SCI_FREE(pinyin_audio_info[i]->audio_data);
                pinyin_audio_info[i]->audio_data = NULL;
            }
            SCI_FREE(pinyin_audio_info[i]);
            pinyin_audio_info[i] = NULL;
        }
    }
}

LOCAL void Pinyin_ParseAudioPath(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint16 size = 0;
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            cJSON * baseUrl = cJSON_GetObjectItem(root, "baseUrl");
            for(i = 0;i < PINYIN_AUDIO_ITEM_MAX && i < pinyin_info_num[idx].num;i++)
            {
                char text_str[20] = {0};
                if(pinyin_audio_info[i] != NULL){
                    SCI_FREE(pinyin_audio_info[i]);
                    pinyin_audio_info[i] = NULL;
                }
                pinyin_audio_info[i] = (PINYIN_AUDIO_INFO_T *)SCI_ALLOC_APP(sizeof(PINYIN_AUDIO_INFO_T));
                memset(pinyin_audio_info[i], 0, sizeof(PINYIN_AUDIO_INFO_T));

                pinyin_audio_info[i]->audio_len = 0;
                if(pinyin_audio_info[i]->audio_uri != NULL){
                    SCI_FREE(pinyin_audio_info[i]->audio_uri);
                    pinyin_audio_info[i]->audio_uri = NULL;
                }
                size = strlen(baseUrl->valuestring) + 20;
                pinyin_audio_info[i]->audio_uri = SCI_ALLOC_APPZ(size + 1);
                memset(pinyin_audio_info[i]->audio_uri, 0, size + 1);
                Pinyin_urlencode(pinyin_info_text[idx][i].text, text_str, 20);
                sprintf(pinyin_audio_info[i]->audio_uri, "%s%s.mp3", baseUrl->valuestring, text_str);
                SCI_TRACE_LOW("%s: [%d]->audio_uri = %s", __FUNCTION__, i, pinyin_audio_info[i]->audio_uri);
            }
            pinyin_request_status = 1;
        }
        else
        {
            pinyin_request_status = -1;
        }
    }
    else
    {
        pinyin_request_status = -2;
    }
    if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID)){
        MMK_SendMsg(ZMT_PINYIN_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

PUBLIC void Pinyin_RequestAudioPath(void)
{
    char url[200] = {0};
    char * query_str = NULL;
#ifdef WIN32
    char * data_buf = "{\"baseUrl\":\"http://8.130.95.8:8866/file/pinyin/\"}";
    Pinyin_ParseAudioPath(1, data_buf, strlen(data_buf), 0);
#else
    query_str = makeBaseQueryUrlString(PINYIN_APP_ID, PINYIN_APP_SECRET);
    sprintf(url, PINYIN_AUDIO_BASE_URL_PATH, query_str);
    SCI_FREE(query_str);
    //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    MMIZDT_HTTP_AppSend(TRUE, PINYIN_BASE_URL_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Pinyin_ParseAudioPath);
#endif
}

LOCAL void Pinyin_ParseAudioData(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_PINYIN_READ_WIN_ID);
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        if(pinyin_audio_info[pinyin_request_idx] == NULL){
            SCI_TRACE_LOW("%s: 01empty audio info!!", __FUNCTION__);
            return;
        }
        //SCI_TRACE_LOW("%s: pinyin_request_idx = %d", __FUNCTION__, pinyin_request_idx);
        pinyin_audio_info[pinyin_request_idx]->audio_len = Rcv_len;
        if(pinyin_audio_info[pinyin_request_idx]->audio_data != PNULL)
        {
            SCI_FREE(pinyin_audio_info[pinyin_request_idx]->audio_data);
            pinyin_audio_info[pinyin_request_idx]->audio_data = NULL;
        }
        pinyin_audio_info[pinyin_request_idx]->audio_data = SCI_ALLOCA(Rcv_len);
        SCI_MEMSET(pinyin_audio_info[pinyin_request_idx]->audio_data, 0, Rcv_len);
        SCI_MEMCPY(pinyin_audio_info[pinyin_request_idx]->audio_data, pRcv, Rcv_len);

        if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
            char file_path[50] = {0};
            wchar file_name[50] = {0};
            char text_str[20] = {0};
            Pinyin_urlencode(pinyin_info_text[idx][pinyin_request_idx].text, text_str, 20);
            sprintf(file_path, PINYIN_MP3_DATA_BASE_PATH, idx, text_str);
            GUI_UTF8ToWstr(file_name, 50, file_path, strlen(file_path));
            if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name))){
                zmt_file_data_write(pRcv, Rcv_len, file_path);
            }
        }
    }
    else
    {
        if(pinyin_audio_info[pinyin_request_idx] == NULL){
            SCI_TRACE_LOW("%s: 02empty audio info!!", __FUNCTION__);
            return;
        }
        pinyin_audio_info[pinyin_request_idx]->audio_len = -2;
    }
    pinyin_request_now = FALSE;
    if(MMK_IsFocusWin(ZMT_PINYIN_READ_WIN_ID) || MMK_IsFocusWin(ZMT_PINYIN_TABLE_TIP_WIN_ID)){
        Pinyin_PlayAudioMp3(idx, pinyin_info_text[idx][pinyin_read_info.cur_read_idx].text);
    }
}

PUBLIC void Pinyin_RequestAudioData(uint8 idx, char * url)
{
    if(pinyin_request_now){
        SCI_TRACE_LOW("%s: pinyin_request_now = %d", __FUNCTION__, pinyin_request_now);
        return;
    }
    //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    pinyin_request_now = TRUE;
    pinyin_request_idx = idx;
    MMIZDT_HTTP_AppSend(TRUE, url, PNULL, 0, 1000, 0, 0, 6000, 0, 0, Pinyin_ParseAudioData);
}


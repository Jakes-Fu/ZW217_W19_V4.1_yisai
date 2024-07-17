/*****************************************************************************
** File Name:      zmt_yinbiao_data.c                                              *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#include <stdio.h>  
#include <string.h>  
#include <ctype.h>  
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "cjson.h"
#include "dal_time.h"
#include "zmt_yinbiao_main.h"
#include "zmt_yinbiao_id.h"
#include "mbedtls/md5.h"

extern YINBIAO_INFO_TEXT_T yinbiao_info_text[YINBIAO_ICON_LIST_ITEM_MAX][YINBIAO_DAN_ITEM_MAX];
extern YINBIAO_INFO_NUM_T yinbiao_info_num[YINBIAO_ICON_LIST_ITEM_MAX];
extern YINBIAO_AUDIO_INFO_T * yinbiao_audio_info[YINBIAO_AUDIO_ITEM_MAX];
extern int yinbiao_request_status;
extern int yinbiao_request_idx;
extern BOOLEAN yinbiao_request_now;
extern YINBIAO_READ_INFO_T yinbiao_read_info;
  
// 判断字符是否需要被URL编码  
static int Yinbiao_is_url_safe(char c)
{
    return isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~';
}

// 将单个字符编码为URL格式（%XX）  
static void Yinbiao_encode_char(char c, char *dest)
{
    static const char hex[] = "0123456789ABCDEF";
    dest[0] = '%';
    dest[1] = hex[(c >> 4) & 0xF];
    dest[2] = hex[c & 0xF];
    dest[3] = '\0'; // 确保字符串以null终止（虽然这里只使用了前三个字符）  
}

// urlencode函数  
PUBLIC int Yinbiao_urlencode(const char *src, char *dest, size_t dest_size)
{  
    const char *src_end = src + strlen(src);
    char encoded_char[4]; // 存储单个编码后的字符（包括'\0'）  
    char *p = dest;
    int len = 0;
  
    while (src < src_end && len < dest_size - 1) { // 保留一个字节给'\0'  
        if (Yinbiao_is_url_safe(*src)) {  
            *p++ = *src++;  
            len++;  
        } else {  
            Yinbiao_encode_char(*src++, encoded_char);  
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

PUBLIC void Yinbiao_ReleaseAudioPath(void)
{
    uint8 i = 0;
    for(i = 0;i < YINBIAO_AUDIO_ITEM_MAX;i++)
    {
        if(yinbiao_audio_info[i] != NULL){
            if(yinbiao_audio_info[i]->audio_uri != NULL){
                SCI_FREE(yinbiao_audio_info[i]->audio_uri);
                yinbiao_audio_info[i]->audio_uri = NULL;
            }
            if(yinbiao_audio_info[i]->audio_data != NULL){
                SCI_FREE(yinbiao_audio_info[i]->audio_data);
                yinbiao_audio_info[i]->audio_data = NULL;
            }
            SCI_FREE(yinbiao_audio_info[i]);
            yinbiao_audio_info[i] = NULL;
        }
    }
}

LOCAL void Yinbiao_ParseAudioPath(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint16 size = 0;
        char text[20] = {0};
        uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            cJSON * baseUrl = cJSON_GetObjectItem(root, "baseUrl");
            for(i = 0;i < YINBIAO_AUDIO_ITEM_MAX && i < yinbiao_info_num[idx].num;i++)
            {
                if(yinbiao_audio_info[i] != NULL){
                    SCI_FREE(yinbiao_audio_info[i]);
                    yinbiao_audio_info[i] = NULL;
                }
                yinbiao_audio_info[i] = (YINBIAO_AUDIO_INFO_T *)SCI_ALLOC_APP(sizeof(YINBIAO_AUDIO_INFO_T));
                memset(yinbiao_audio_info[i], 0, sizeof(YINBIAO_AUDIO_INFO_T));

                yinbiao_audio_info[i]->audio_len = 0;
                if(yinbiao_audio_info[i]->audio_uri != NULL){
                    SCI_FREE(yinbiao_audio_info[i]->audio_uri);
                    yinbiao_audio_info[i]->audio_uri = NULL;
                }
                size = strlen(baseUrl->valuestring) + 20;
                yinbiao_audio_info[i]->audio_uri = SCI_ALLOC_APPZ(size + 1);
                memset(yinbiao_audio_info[i]->audio_uri, 0, size + 1);
                memset(text, 0, 20);
                Yinbiao_urlencode(yinbiao_info_text[idx][i].text, text, 20);
                sprintf(yinbiao_audio_info[i]->audio_uri, "%s%s.mp3", baseUrl->valuestring, text);
                SCI_TRACE_LOW("%s: [%d]->audio_uri = %s", __FUNCTION__, i, yinbiao_audio_info[i]->audio_uri);
            }
            yinbiao_request_status = 1;
        }
        else
        {
            yinbiao_request_status = -1;
        }
    }
    else
    {
        yinbiao_request_status = -2;
    }
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID)){
        MMK_SendMsg(ZMT_YINBIAO_READ_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

PUBLIC void Yinbiao_RequestAudioPath(void)
{
    char url[200] = {0};
    char * query_str = NULL;
#ifdef WIN32
    char * data_buf = "{\"baseUrl\":\"http://8.130.95.8:8866/file/english/IPA/\"}";
    Yinbiao_ParseAudioPath(1, data_buf, strlen(data_buf), 0);
#else
    query_str = makeBaseQueryUrlString(YINBIAO_APP_ID, YINBIAO_APP_SECRET);
    sprintf(url, YINBIAO_AUDIO_BASE_URL_PATH, query_str);
    SCI_FREE(query_str);
    //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    MMIZDT_HTTP_AppSend(TRUE, YINBIAO_BASE_URL_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Yinbiao_ParseAudioPath);
#endif
}

LOCAL void Yinbiao_ParseAudioData(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    uint8 idx = MMK_GetWinAddDataPtr(ZMT_YINBIAO_READ_WIN_ID);
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        if(yinbiao_audio_info[yinbiao_request_idx] == NULL){
            SCI_TRACE_LOW("%s: 01empty audio info!!", __FUNCTION__);
            return;
        }
        SCI_TRACE_LOW("%s: yinbiao_request_idx = %d", __FUNCTION__, yinbiao_request_idx);
        yinbiao_audio_info[yinbiao_request_idx]->audio_len = Rcv_len;
        if(yinbiao_audio_info[yinbiao_request_idx]->audio_data != PNULL)
        {
            SCI_FREE(yinbiao_audio_info[yinbiao_request_idx]->audio_data);
            yinbiao_audio_info[yinbiao_request_idx]->audio_data = NULL;
        }
        yinbiao_audio_info[yinbiao_request_idx]->audio_data = SCI_ALLOCA(Rcv_len);
        SCI_MEMSET(yinbiao_audio_info[yinbiao_request_idx]->audio_data, 0, Rcv_len);
        SCI_MEMCPY(yinbiao_audio_info[yinbiao_request_idx]->audio_data, pRcv, Rcv_len);

        if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
            char file_path[50] = {0};
            wchar file_name[50] = {0};
            char text[20] = {0};
            Yinbiao_urlencode(yinbiao_info_text[idx][yinbiao_request_idx].text, text, 20);
            sprintf(file_path, YINBIAO_MP3_DATA_BASE_PATH, idx, text);
            GUI_UTF8ToWstr(file_name, 50, file_path, strlen(file_path));
            if(!MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name))){
                zmt_file_data_write(pRcv, Rcv_len, file_path);
            }
        }
    }
    else
    {
        if(yinbiao_audio_info[yinbiao_request_idx] == NULL){
            SCI_TRACE_LOW("%s: 02empty audio info!!", __FUNCTION__);
            return;
        }
        yinbiao_audio_info[yinbiao_request_idx]->audio_len = -2;
    }
    yinbiao_request_now = FALSE;
    if(MMK_IsFocusWin(ZMT_YINBIAO_READ_WIN_ID) || MMK_IsFocusWin(ZMT_YINBIAO_TABLE_TIP_WIN_ID)){
        Yinbiao_PlayAudioMp3(idx, yinbiao_info_text[idx][yinbiao_read_info.cur_read_idx].text);
    }
}

PUBLIC void Yinbiao_RequestAudioData(uint8 idx, char * url)
{
    if(yinbiao_request_now){
        SCI_TRACE_LOW("%s: yinbiao_request_now = %d", __FUNCTION__, yinbiao_request_now);
        return;
    }
    SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    yinbiao_request_now = TRUE;
    yinbiao_request_idx = idx;
    MMIZDT_HTTP_AppSend(TRUE, url, PNULL, 0, 1000, 0, 0, 6000, 0, 0, Yinbiao_ParseAudioData);
}


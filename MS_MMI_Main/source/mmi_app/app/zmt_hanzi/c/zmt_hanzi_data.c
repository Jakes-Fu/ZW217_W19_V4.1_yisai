/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "cjson.h"
#include "zmt_hanzi_main.h"
#include "zmt_hanzi_id.h"
#include "zmt_hanzi_text.h"
#ifdef LISTENING_PRATICE_SUPPORT
#include "zmt_main_file.h"
#include "zmt_listening_export.h"
#endif
#ifndef WIN32
#include "mbedtls/md5.h"
#endif
#include "mbedtls/md5.h"

extern HANZI_LEARN_INFO_T * hanzi_learn_info;
extern HANZI_BOOK_INFO_T hanzi_book_info;
extern BOOLEAN is_open_auto_play;
extern int cur_chapter_unmaster_idx[HANZI_CHAPTER_WORD_MAX];
extern uint8 cur_new_hanzi_page_idx;

int8 hanzi_book_count = 0;
HANZI_PUBLISH_BOOK_INFO * hanzi_publish_info[HANZI_PUBLISH_BOOK_MAX];
HANZI_CONTENT_INFO_T * hanzi_content_info[HANZI_CONTENT_CHAPTER_MAX];
int16 hanzi_chapter_count = 0;
int16 hanzi_chapter_children_count[HANZI_CONTENT_CHAPTER_MAX] = {0};
HANZI_BOOK_HANZI_INFO * hanzi_detail_info[HANZI_CHAPTER_WORD_MAX];
int16 hanzi_detail_count = 0;
int16 hanzi_detail_cur_idx = 0;
BOOLEAN hanzi_is_load_local = FALSE;
LOCAL uint8 * Hanzi_MakeMd5Str(char * sign)
{
    mbedtls_md5_context md5_ctx = {0};
    char digest[16] = {0};
    uint8 i = 0;
    uint8 *md5 = (uint8 *)SCI_ALLOC_APPZ(33);
#ifndef WIN32
    mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_starts(&md5_ctx);
    mbedtls_md5_update(&md5_ctx, sign, strlen(sign));
    mbedtls_md5_finish(&md5_ctx, digest);
    mbedtls_md5_free(&md5_ctx);
#endif
    for(i = 0; i < 16; i++)
    {
        sprintf(md5+(i*2),"%02x",digest[i]);
    }
    return md5;
}

PUBLIC void Hanzi_UpdateBookInfo(void)
{
    char url[200] = {0};
    char * query_str = makeBaseQueryUrlString(HANZI_BOOK_APP_ID, HANZI_BOOK_APP_SECRET);
    sprintf(url, HANZI_BOOK_PUBLISH_PATH, query_str);
    SCI_FREE(query_str);
    SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    hanzi_is_load_local = FALSE;
    MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseBookInfo);
}

PUBLIC void Hanzi_ReleaseLearnInfo(void)
{
    if(hanzi_learn_info != NULL)
    {
        SCI_FREE(hanzi_learn_info);
        hanzi_learn_info = NULL;
    }
}

PUBLIC void Hanzi_WriteLearnInfo(void)
{
    char * out = NULL;
    char file_path[50] = {0};
    cJSON * root;
    cJSON * update_times;
    cJSON * publish_id;
    cJSON * book_id;
    cJSON * unit_id;
    
    if(hanzi_learn_info == NULL)
    {
        SCI_TRACE_LOW("%s: hanzi_learn_info empty!!", __FUNCTION__);
        return;
    }
    
    root = cJSON_CreateObject();
    update_times = cJSON_CreateNumber(hanzi_learn_info->update_times);
    cJSON_AddItemToObject(root, "update_times", update_times);
    book_id = cJSON_CreateNumber(hanzi_learn_info->book_id);
    cJSON_AddItemToObject(root, "book_id", book_id);
    unit_id = cJSON_CreateNumber(hanzi_learn_info->unit_id);
    cJSON_AddItemToObject(root, "unit_id", unit_id);

    out = cJSON_PrintUnformatted(root);
    strcpy(file_path, HANZI_CARD_LEARN_INFO_PATH);
#ifndef WIN32
	//#ifndef TF_CARD_SUPPORT
    if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024)
 //#endif
#endif
    {
        if(zmt_file_exist(file_path)){
            zmt_file_delete(file_path);
        }
        zmt_file_data_write(out, strlen(out), file_path);
    }
    
    cJSON_Delete(root);
    SCI_FREE(out);
}

PUBLIC void Hanzi_UpdateLearnInfo(uint16 book_id, uint16 unit_id)
{
    if(hanzi_learn_info == NULL){
        uint32 cur_times = MMIAPICOM_GetCurTime();
        hanzi_learn_info = SCI_ALLOC_APPZ(sizeof(HANZI_LEARN_INFO_T));
        memset(hanzi_learn_info, 0, sizeof(HANZI_LEARN_INFO_T));
        hanzi_learn_info->update_times = cur_times;
    }
    hanzi_learn_info->book_id = book_id;
    hanzi_learn_info->unit_id = unit_id;
}

PUBLIC BOOLEAN Hanzi_LoadLearnInfo(void)
{   
    BOOLEAN load_result = FALSE;
    char file_path[50] = {0};
    char * data_buf = PNULL;
    uint32 data_size = 0;

    if(hanzi_learn_info != NULL){
        return load_result;
    }

    strcpy(file_path, HANZI_CARD_LEARN_INFO_PATH);
    if(zmt_file_exist(file_path))
    {
        data_buf = zmt_file_data_read(file_path, &data_size);
        if(data_buf != PNULL && data_size > 0)
        {
            cJSON * root;
            cJSON * update_times;
            cJSON * book_id;
            cJSON * unit_id;

            root = cJSON_Parse(data_buf);
            if(root != NULL && root->type != cJSON_NULL)
            {
                update_times = cJSON_GetObjectItem(root, "update_times");
                book_id = cJSON_GetObjectItem(root, "book_id");
                unit_id = cJSON_GetObjectItem(root, "unit_id");
                
                if(hanzi_learn_info == NULL){
                    hanzi_learn_info = SCI_ALLOC_APPZ(sizeof(HANZI_LEARN_INFO_T));
                }
                memset(hanzi_learn_info, 0, sizeof(HANZI_LEARN_INFO_T));
                hanzi_learn_info->update_times = update_times->valueint;
                hanzi_learn_info->book_id = book_id->valueint;
                hanzi_learn_info->unit_id = unit_id->valueint;
                
                load_result = TRUE;
                cJSON_Delete(root);
            }
            SCI_FREE(data_buf);
        }
    }
    if(load_result){
        uint32 cur_times = MMIAPICOM_GetCurTime();
        if(cur_times - hanzi_learn_info->update_times >= HANZI_BOOK_UPDATE_INTERVAL_TIMES)
        {
            hanzi_learn_info->update_times = cur_times;
            Hanzi_UpdateBookInfo();
            load_result = FALSE;
        }
    }
    return load_result;
}

PUBLIC void Hanzi_WriteExistUnmasterHanzi(uint16 book_id, uint16 chap_id,cJSON * hanzis, uint8 write_count)
{
    uint16 i = 0;
    uint16 j = 0;
    uint16 count = 0;
    char file_path[50] = {0};
    char * pRcv = NULL;
    uint32 data_size = 0;
    cJSON * roots;
    cJSON * hanziss;
    cJSON * hanzis_item;
    cJSON * hanzis_items;
    cJSON * texts;
    cJSON * audios;
    cJSON * pinyins;
    cJSON * similar_words;
    cJSON * annotations;

    sprintf(file_path, HANZI_CARD_NEW_HANZI_PATH, book_id, chap_id);
    if(zmt_tfcard_exist() && zmt_file_exist(file_path)){
        pRcv = zmt_file_data_read(file_path, &data_size);
    }else{
        return;
    }
    if(pRcv != NULL && data_size > 2)
    {
        roots = cJSON_Parse(pRcv);
        if(roots != NULL && roots->type != cJSON_NULL)
        {
            hanziss = cJSON_GetObjectItem(roots, "hanzis");
            count = cJSON_GetArraySize(hanziss);
            for(i = 0; i < cJSON_GetArraySize(hanziss);i++)
            {
                BOOLEAN is_exist = FALSE;
                hanzis_item = cJSON_GetArrayItem(hanziss, i);
                texts = cJSON_GetObjectItem(hanzis_item, "text");
                audios = cJSON_GetObjectItem(hanzis_item, "audio");
                pinyins = cJSON_GetObjectItem(hanzis_item, "pinyin");
                similar_words = cJSON_GetObjectItem(hanzis_item, "similar_word");
                annotations = cJSON_GetObjectItem(hanzis_item, "annotation");

                //SCI_TRACE_LOW("%s: texts->valuestring = %s", __FUNCTION__, texts->valuestring);
                //SCI_TRACE_LOW("%s: pinyins->valuestring = %s", __FUNCTION__, pinyins->valuestring);
                for(j = 0;j < write_count;j++)
                {
                    uint8 idx = cur_chapter_unmaster_idx[j] - 1;
                    //SCI_TRACE_LOW("%s: hanzi_detail_info[%d]->word = %s", __FUNCTION__, idx, hanzi_detail_info[idx]->word);
                    //SCI_TRACE_LOW("%s: hanzi_detail_info[%d]->pingy = %s", __FUNCTION__, idx, hanzi_detail_info[idx]->pingy);
                    if(texts->valuestring != NULL && pinyins->valuestring != NULL)
                    {
                        if(0 == strcmp(texts->valuestring, hanzi_detail_info[idx]->word) && 
                            0 == strcmp(pinyins->valuestring, hanzi_detail_info[idx]->pingy))
                        {
                            is_exist = TRUE;
                            break;
                        }
                    }
                }
                SCI_TRACE_LOW("%s: is_exist = %d", __FUNCTION__, is_exist);
                if(is_exist){
                    continue;
                }

                hanzis_items = cJSON_CreateObject();
                if(texts->valuestring != NULL){
                    cJSON_AddStringToObject(hanzis_items, "text", texts->valuestring);
                }else{
                    cJSON_AddStringToObject(hanzis_items, "text", "");
                }
                if(audios->valuestring != NULL){
                    cJSON_AddStringToObject(hanzis_items, "audio", audios->valuestring);
                }else{
                    cJSON_AddStringToObject(hanzis_items, "audio", "");
                }
                if(pinyins->valuestring != NULL){
                    cJSON_AddStringToObject(hanzis_items, "pinyin", pinyins->valuestring);
                }else{
                    cJSON_AddStringToObject(hanzis_items, "pinyin", "");
                }
                if(similar_words->valuestring != NULL){
                    cJSON_AddStringToObject(hanzis_items, "similar_word", similar_words->valuestring);
                }else{
                    cJSON_AddStringToObject(hanzis_items, "similar_word", "");
                }
                if(annotations->valuestring != NULL){
                    cJSON_AddStringToObject(hanzis_items, "annotation", annotations->valuestring);
                }else{
                    cJSON_AddStringToObject(hanzis_items, "annotation", "");
                }
                cJSON_AddItemToArray(hanzis, hanzis_items);
            }
            cJSON_Delete(roots);
        }
        SCI_FREE(pRcv);
    }
}

PUBLIC void Hanzi_WriteUnmasterHanzi(uint16 grade_id, uint16 chap_id, uint8 write_count)
{
    uint16 i = 0;
    uint16 index = 0;
    char * out = NULL;
    char file_path[50] = {0};
    char * pRcv = NULL;
    uint32 data_size = 0;
    cJSON * root;
    cJSON * hanzis;
    cJSON * hanzis_item;
    cJSON * text;
    cJSON * audio;
    cJSON * pinyin;
    cJSON * similar_word;
    cJSON * annotation;

    if(write_count <= 0){
        SCI_TRACE_LOW("%s: UnmasterHanzi no to write", __FUNCTION__);
        return;
    }
    SCI_TRACE_LOW("%s: write_count = %d", __FUNCTION__, write_count);

    root = cJSON_CreateObject();
    hanzis = cJSON_CreateArray();

    sprintf(file_path, HANZI_CARD_NEW_HANZI_PATH, grade_id, chap_id);
    Hanzi_WriteExistUnmasterHanzi(grade_id, chap_id, hanzis, write_count);
    for(i = 0;i < HANZI_CHAPTER_WORD_MAX;i++)
    {       
        if(cur_chapter_unmaster_idx[i] != 0)
        {
            index = cur_chapter_unmaster_idx[i] - 1;
            hanzis_item = cJSON_CreateObject();
            if(hanzi_detail_info[index]->word != NULL){
                text = cJSON_CreateString(hanzi_detail_info[index]->word);
                cJSON_AddItemToObject(hanzis_item, "text", text);
            }else{
                cJSON_AddStringToObject(hanzis_item, "text", "");
            }
            
            if(hanzi_detail_info[index]->audio_uri != NULL){
                audio = cJSON_CreateString(hanzi_detail_info[index]->audio_uri);
                cJSON_AddItemToObject(hanzis_item, "audio", audio);
            }else{
                cJSON_AddStringToObject(hanzis_item, "audio", "");
            }
            
            if(hanzi_detail_info[index]->pingy != NULL){
                pinyin = cJSON_CreateString(hanzi_detail_info[index]->pingy);
                cJSON_AddItemToObject(hanzis_item, "pinyin", pinyin);
            }else{
                cJSON_AddStringToObject(hanzis_item, "pinyin", "");
            }
            
            if(hanzi_detail_info[index]->similar_word != NULL){
                similar_word = cJSON_CreateString(hanzi_detail_info[index]->similar_word);
                cJSON_AddItemToObject(hanzis_item, "similar_word", similar_word);
            }else{
                cJSON_AddStringToObject(hanzis_item, "similar_word", "");
            }
            
            if(hanzi_detail_info[index]->annotation != NULL){
                annotation = cJSON_CreateString(hanzi_detail_info[index]->annotation);
                cJSON_AddItemToObject(hanzis_item, "annotation", annotation);
            }else{
                cJSON_AddStringToObject(hanzis_item, "annotation", "");
            }

            cJSON_AddItemToArray(hanzis, hanzis_item);
        }
    }
    cJSON_AddItemToObject(root, "hanzis", hanzis);
    out = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    //SCI_TRACE_LOW("%s: out = %s", __FUNCTION__, out);
#ifndef WIN32
//	#ifndef TF_CARD_SUPPORT
    if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
        if(zmt_file_exist(file_path)){
            zmt_file_delete(file_path);
        }
        zmt_file_data_write(out, strlen(out), file_path);
    }
#else
    zmt_file_data_write(out, strlen(out), file_path);
#endif
//#endif
    SCI_FREE(out);
}

PUBLIC void Hanzi_ReleaseBookInfo(void)
{
    uint8 i = 0;
    for(i = 0; i < HANZI_PUBLISH_BOOK_MAX;i++)
    {
        if(hanzi_publish_info[i] != NULL){
            if(hanzi_publish_info[i]->book_name != NULL){
                SCI_FREE(hanzi_publish_info[i]->book_name);
                hanzi_publish_info[i]->book_name = NULL;
            }
            if(hanzi_publish_info[i]->publish_name != NULL){
                SCI_FREE(hanzi_publish_info[i]->publish_name);
                hanzi_publish_info[i]->publish_name = NULL;
            }
            SCI_FREE(hanzi_publish_info[i]);
            hanzi_publish_info[i] = NULL;
        }
    }
}

PUBLIC void Hanzi_ParseBookInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint8 m = 0;
        uint16 size = 0;
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            hanzi_book_count = cJSON_GetArraySize(root);
            for(i = 0;i < cJSON_GetArraySize(root) && i < HANZI_PUBLISH_BOOK_MAX;i++)
            {
                cJSON *list_item =  cJSON_GetArrayItem(root, i);
                cJSON *id = cJSON_GetObjectItem(list_item, "id");
                cJSON *bookName = cJSON_GetObjectItem(list_item, "bookName");
                cJSON *publishName = cJSON_GetObjectItem(list_item, "publishName");

                if(hanzi_publish_info[i] != NULL){
                    SCI_FREE(hanzi_publish_info[i]);
                    hanzi_publish_info[i] = NULL;
                }
                hanzi_publish_info[i] = (HANZI_PUBLISH_BOOK_INFO *)SCI_ALLOC_APP(sizeof(HANZI_PUBLISH_BOOK_INFO));
                memset(hanzi_publish_info[i], 0, sizeof(HANZI_PUBLISH_BOOK_INFO));

                hanzi_publish_info[i]->id = id->valueint;

                if(hanzi_publish_info[i]->book_name != NULL){
                    SCI_FREE(hanzi_publish_info[i]->book_name);
                    hanzi_publish_info[i]->book_name = NULL;
                }
                size = strlen(bookName->valuestring);
                hanzi_publish_info[i]->book_name = SCI_ALLOC_APPZ(size + 1);
                memset(hanzi_publish_info[i]->book_name, 0, size + 1);
                SCI_MEMCPY(hanzi_publish_info[i]->book_name, bookName->valuestring, size);
                
                if(hanzi_publish_info[i]->publish_name != NULL){
                    SCI_FREE(hanzi_publish_info[i]->publish_name);
                    hanzi_publish_info[i]->publish_name = NULL;
                }
                size = strlen(publishName->valuestring);
                hanzi_publish_info[i]->publish_name = SCI_ALLOC_APPZ(size + 1);
                memset(hanzi_publish_info[i]->publish_name, 0, size + 1);
                SCI_MEMCPY(hanzi_publish_info[i]->publish_name, publishName->valuestring, size);

            }
            if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024)
            {
                if(!hanzi_is_load_local)
                {
                    char * out = NULL;
                    char file_path[30] = {0};
                    strcpy(file_path, HANZI_CARD_BOOK_INFO_PATH);
                    if(zmt_file_exist(file_path)){
                        zmt_file_delete(file_path);
                    }
                    out = cJSON_PrintUnformatted(root);
                    zmt_file_data_write(out, strlen(out), file_path);
                    SCI_FREE(out);
                }
            }
            cJSON_Delete(root);
        }
        else
        {
            hanzi_book_count = -2;
        }
    }
    else
    {
        hanzi_book_count = -1;
    }
    if(hanzi_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    hanzi_is_load_local = FALSE;
    if(MMK_IsFocusWin(MMI_HANZI_MAIN_WIN_ID)){
        MMK_SendMsg(MMI_HANZI_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

PUBLIC void Hanzi_requestBookInfo(void)
{
    char file_path[30] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    strcpy(file_path, HANZI_CARD_BOOK_INFO_PATH);
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            hanzi_is_load_local = TRUE;
            Hanzi_ParseBookInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        char url[200] = {0};
        char * query_str = makeBaseQueryUrlString(HANZI_BOOK_APP_ID, HANZI_BOOK_APP_SECRET);
        sprintf(url, HANZI_BOOK_PUBLISH_PATH, query_str);
        SCI_FREE(query_str);
        SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
        hanzi_is_load_local = FALSE;
        MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseBookInfo);
    }
}

PUBLIC void Hanzi_ReleaseChapterInfo(void)
{
    uint8 i,j = 0;
    for(i = 0;i < HANZI_CONTENT_CHAPTER_MAX;i++)
    {
        if(hanzi_content_info[i] != NULL){
            if(hanzi_content_info[i]->content_name != NULL){
                SCI_FREE(hanzi_content_info[i]->content_name);
                hanzi_content_info[i]->content_name = NULL;
            }
            for(j = 0;j < HANZI_CONTENT_CHAPTER_MAX;j++){
                if(hanzi_content_info[i]->chapter[j] != NULL){
                    if(hanzi_content_info[i]->chapter[j]->chapter_name != NULL){
                        SCI_FREE(hanzi_content_info[i]->chapter[j]->chapter_name);
                        hanzi_content_info[i]->chapter[j]->chapter_name = NULL;
                    }
                    SCI_FREE(hanzi_content_info[i]->chapter[j]);
                    hanzi_content_info[i]->chapter[j]= NULL;
                }
            }
            SCI_FREE(hanzi_content_info[i]);
            hanzi_content_info[i] = NULL;
        }
    }
}

PUBLIC void Hanzi_ParseChapterInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint8 m = 0;
        uint16 size = 0;
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            for(i = 0;i < cJSON_GetArraySize(root) && i < HANZI_CONTENT_CHAPTER_MAX;i++)
            {              
                cJSON *list_item =  cJSON_GetArrayItem(root, i);
                cJSON *contentId = cJSON_GetObjectItem(list_item, "id");
                cJSON *contentName = cJSON_GetObjectItem(list_item, "contentName");
                cJSON *chapters = cJSON_GetObjectItem(list_item, "chapters");
                if(hanzi_content_info[i] != NULL){
                    SCI_FREE(hanzi_content_info[i]);
                    hanzi_content_info[i] = NULL;
                }
                hanzi_content_info[i] = (HANZI_CONTENT_INFO_T *)SCI_ALLOC_APP(sizeof(HANZI_CONTENT_INFO_T));
                memset(hanzi_content_info[i], 0, sizeof(HANZI_CONTENT_INFO_T));
                if(contentName != NULL && contentName->type != cJSON_NULL)
                {
                    if(hanzi_content_info[i]->content_name != NULL){
                        SCI_FREE(hanzi_content_info[i]->content_name);
                        hanzi_content_info[i]->content_name = NULL;
                    }
                    
                    size = strlen(contentName->valuestring);
                    hanzi_content_info[i]->content_name = SCI_ALLOC_APP(size+1);
                    memset(hanzi_content_info[i]->content_name, 0, size+1);
                    strncpy(hanzi_content_info[i]->content_name, contentName->valuestring, size);
                    //SCI_TRACE_LOW("%s: info[%d].content_name = %s", __FUNCTION__, i, hanzi_content_info[i]->content_name);

                    hanzi_content_info[i]->content_id = contentId->valueint;
                    //SCI_TRACE_LOW("%s: info[%d].content_id = %d", __FUNCTION__, i, hanzi_content_info[i]->content_id);            
                }
                for(m = 0;m < cJSON_GetArraySize(chapters) && m < HANZI_CONTENT_CHAPTER_MAX;m++)
                {
                    cJSON *chapters_item =  cJSON_GetArrayItem(chapters, m);
                    cJSON *chaptersId = cJSON_GetObjectItem(chapters_item, "id");
                    cJSON *chaptersName = cJSON_GetObjectItem(chapters_item, "chapterName");

                    if(hanzi_content_info[i]->chapter[m] != NULL){
                        SCI_FREE(hanzi_content_info[i]->chapter[m]);
                        hanzi_content_info[i]->chapter[m] = NULL;
                    }
                    hanzi_content_info[i]->chapter[m] = SCI_ALLOC_APP(sizeof(HANZI_CONTENT_CHAPTER_INFO_T));
                    memset(hanzi_content_info[i]->chapter[m], 0, sizeof(HANZI_CONTENT_CHAPTER_INFO_T));
                    
                    if(hanzi_content_info[i]->chapter[m]->chapter_name != NULL){
                        SCI_FREE(hanzi_content_info[i]->chapter[m]->chapter_name);
                        hanzi_content_info[i]->chapter[m]->chapter_name = NULL;
                    }
                    size = strlen(chaptersName->valuestring);
                    hanzi_content_info[i]->chapter[m]->chapter_name = SCI_ALLOC_APP(size+1);
                    memset(hanzi_content_info[i]->chapter[m]->chapter_name, 0, size+1);
                    strncpy(hanzi_content_info[i]->chapter[m]->chapter_name, chaptersName->valuestring, size);
                    //SCI_TRACE_LOW("%s: info[%d].chapter[%d]->chapter_name = %s", __FUNCTION__, i, m, chaptersName->valuestring);

                    hanzi_content_info[i]->chapter[m]->chapter_id = chaptersId->valueint;
                }
                hanzi_chapter_children_count[i] = m;
            }
            hanzi_chapter_count = i;
		//	#ifndef TF_CARD_SUPPORT
            if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
                if(!hanzi_is_load_local)
                {
                    char * out = NULL;
                    char file_path[30] = {0};
                    sprintf(file_path, HANZI_CARD_GRADE_CHAPTER_PATH, hanzi_book_info.cur_book_idx+1, hanzi_book_info.cur_book_idx+1);
                    if(zmt_file_exist(file_path)){
                        zmt_file_delete(file_path);
                    }
                    out = cJSON_PrintUnformatted(root);
                    zmt_file_data_write(out, strlen(out), file_path);
                    SCI_FREE(out);
                }
            }
	//	#endif
		 cJSON_Delete(root);
        }
        else
        {

            hanzi_chapter_count = -2;
        }
    }
    else
    {

        hanzi_chapter_count = -1;
    }
    if(hanzi_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    hanzi_is_load_local = FALSE;
    MMK_SendMsg(MMI_HANZI_CHAPTER_WIN_ID, MSG_FULL_PAINT, PNULL);
}

PUBLIC void Hanzi_requestChapterInfo(uint8 grade_id)
{
    char file_path[30] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    sprintf(file_path, HANZI_CARD_GRADE_CHAPTER_PATH, grade_id, grade_id);
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            hanzi_is_load_local = TRUE;
            Hanzi_ParseChapterInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        char url[200] = {0};
        char * query_str = makeBaseQueryUrlString(HANZI_BOOK_APP_ID, HANZI_BOOK_APP_SECRET);
        sprintf(url, HANZI_BOOK_GRADE_PATH, grade_id, query_str);
        SCI_FREE(query_str);
        SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
        hanzi_is_load_local = FALSE;
        MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseChapterInfo);
    }
}

PUBLIC void Hanzi_ReleaseDetailInfo(void)
{
    uint16 i = 0;
    for(i = 0;i < HANZI_CHAPTER_WORD_MAX;i++)
    {
        if(hanzi_detail_info[i] != NULL){
            if(hanzi_detail_info[i]->word != NULL){
                SCI_FREE(hanzi_detail_info[i]->word);
                hanzi_detail_info[i]->word = NULL;
            }
            if(hanzi_detail_info[i]->pingy != NULL){
                SCI_FREE(hanzi_detail_info[i]->pingy);
                hanzi_detail_info[i]->pingy = NULL;
            }
            if(hanzi_detail_info[i]->audio_uri != NULL){
                SCI_FREE(hanzi_detail_info[i]->audio_uri);
                hanzi_detail_info[i]->audio_uri = NULL;
            }
            if(hanzi_detail_info[i]->audio_data != NULL){
                SCI_FREE(hanzi_detail_info[i]->audio_data);
                hanzi_detail_info[i]->audio_data = NULL;
            }
            if(hanzi_detail_info[i]->similar_word != NULL){
                SCI_FREE(hanzi_detail_info[i]->similar_word);
                hanzi_detail_info[i]->similar_word = NULL;
            }
            if(hanzi_detail_info[i]->annotation != NULL){
                SCI_FREE(hanzi_detail_info[i]->annotation);
                hanzi_detail_info[i]->annotation = NULL;
            }
            if(hanzi_detail_info[i]->remark != NULL){
                SCI_FREE(hanzi_detail_info[i]->remark);
                hanzi_detail_info[i]->remark = NULL;
            }
            SCI_FREE(hanzi_detail_info[i]);
            hanzi_detail_info[i] = NULL;
        }
    }
}

LOCAL void Hanzi_DetailMakeTermRemark(char* remark, char * similar_word, char * annotation)
{
    if(remark != NULL)
    {
        if(similar_word != NULL && strlen(similar_word) > 0){
            char word[20] = {0};
            MMI_STRING_T text_string = {0};
            MMIRES_GetText(HANZI_SIMILAR_WORD, MMI_HANZI_DETAIL_WIN_ID, &text_string);
            GUI_WstrToUTF8(word, 20, text_string.wstr_ptr, text_string.wstr_len);
            strcat(remark, word);
            strcat(remark, similar_word);
            strcat(remark, "\n");
        }
        if(annotation != NULL && strlen(annotation) > 0){
            strcat(remark, annotation);
        }
    }
}


LOCAL BOOLEAN Hanzi_DetailMakeAudioUrl(uint8 idx, char * text, char * pinyin, char * baseUrl)
{
    //http://8.130.95.8:8866/file/audio/word/000f310ee13acf86b7e347d5dd52f11f/name/li%C3%A1n.mp3
    char url[100] = {0};
    char * sn = NULL;
    if(text == NULL || pinyin == NULL)
    {
        SCI_TRACE_LOW("%s: param is null", __FUNCTION__);
        return FALSE;
    }
    sn = Hanzi_MakeMd5Str(text);
    if(baseUrl != NULL){
        sprintf(url, HANZI_BOOK_AUDIO_PATH, baseUrl, sn, pinyin);
    }else{
        sprintf(url, HANZI_BOOK_AUDIO_PATH, HANZI_BOOK_AUDIO_BASE_PATH, sn, pinyin);
    }
    SCI_FREE(sn);
    hanzi_detail_info[idx]->audio_uri = (char *)SCI_ALLOC_APP(strlen(url) + 1);
    memset(hanzi_detail_info[idx]->audio_uri, 0, strlen(url) + 1);
    SCI_MEMCPY(hanzi_detail_info[idx]->audio_uri, url, strlen(url));
    //SCI_TRACE_LOW("%s: [%d]->audio_uri = %s", __FUNCTION__, idx, hanzi_detail_info[idx]->audio_uri);
    return TRUE;
}

LOCAL void Hanzi_AddDetailInfo(uint8 idx, cJSON* text, cJSON * audio, cJSON * pinyin, cJSON * similar_word, cJSON *annotation, cJSON * baseUrl)
{
    BOOLEAN result = FALSE;
    uint32 size = 0;
    uint32 similar_word_size = 0;
    uint32 annotation_size = 0;
    hanzi_detail_info[idx] = (HANZI_BOOK_HANZI_INFO *)SCI_ALLOC_APP(sizeof(HANZI_BOOK_HANZI_INFO));
    memset(hanzi_detail_info[idx], 0, sizeof(HANZI_BOOK_HANZI_INFO));

    if(text != NULL && text->type != cJSON_NULL && text->valuestring != NULL){
        size = strlen(text->valuestring);
        if(size > 0){
            hanzi_detail_info[idx]->word = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[idx]->word, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[idx]->word, text->valuestring, size);
        }else{
            hanzi_detail_info[idx]->word = NULL;
        }
    }else{
        hanzi_detail_info[idx]->word = NULL;
    }

    if(pinyin != NULL && pinyin->type != cJSON_NULL && pinyin->valuestring != NULL){
        size = strlen(pinyin->valuestring);
        if(size > 0){
            hanzi_detail_info[idx]->pingy = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[idx]->pingy, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[idx]->pingy, pinyin->valuestring, size);
        }else{
            hanzi_detail_info[idx]->pingy = NULL;
        }
    }else{
        hanzi_detail_info[idx]->pingy = NULL;
    }

    if(baseUrl == NULL && audio != NULL && audio->type != cJSON_NULL && audio->valuestring != NULL){
        size = strlen(audio->valuestring);
        if(size > 0){
            hanzi_detail_info[idx]->audio_uri = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[idx]->audio_uri, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[idx]->audio_uri, audio->valuestring, size);
        }else{
            hanzi_detail_info[idx]->audio_uri = NULL;
            hanzi_detail_info[idx]->audio_len = -1;
        }
    }else{
        result = Hanzi_DetailMakeAudioUrl(idx, text->valuestring, pinyin->valuestring, baseUrl->valuestring);
        if(!result){
            hanzi_detail_info[idx]->audio_uri = NULL;
            hanzi_detail_info[idx]->audio_len = -1;
        }else{
            hanzi_detail_info[idx]->audio_len = 0;
        }
    }
                        
    if(similar_word != NULL && similar_word->type != cJSON_NULL && similar_word->valuestring != NULL){
        similar_word_size = strlen(similar_word->valuestring);
        if(similar_word_size > 0){
            hanzi_detail_info[idx]->similar_word = (char *)SCI_ALLOC_APP(similar_word_size + 1);
            memset(hanzi_detail_info[idx]->similar_word, 0, similar_word_size + 1);
            SCI_MEMCPY(hanzi_detail_info[idx]->similar_word, similar_word->valuestring, similar_word_size);
        }else{
            hanzi_detail_info[idx]->similar_word = NULL;
        }
    }else{
        hanzi_detail_info[idx]->similar_word = NULL;
    }
                        
    if(annotation != NULL && annotation->type != cJSON_NULL && annotation->valuestring != NULL){
        annotation_size = strlen(annotation->valuestring);
        if(annotation_size > 0){
            hanzi_detail_info[idx]->annotation = (char *)SCI_ALLOC_APP(annotation_size + 1);
            memset(hanzi_detail_info[idx]->annotation, 0, annotation_size + 1);
            SCI_MEMCPY(hanzi_detail_info[idx]->annotation, annotation->valuestring, annotation_size);
        }else{
            hanzi_detail_info[idx]->annotation = NULL;
        }
    }else{
        hanzi_detail_info[idx]->annotation = NULL;
    }

    size = similar_word_size + annotation_size + 30;
    hanzi_detail_info[idx]->remark = (char *)SCI_ALLOC_APP(size + 1);
    memset(hanzi_detail_info[idx]->remark, 0, size + 1);
    Hanzi_DetailMakeTermRemark(hanzi_detail_info[idx]->remark, similar_word->valuestring, annotation->valuestring);          
}

PUBLIC void Hanzi_ParseDetailInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint8 j = 0;
        uint16 req_chapter_id = 0;
        uint32 size = 0;
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            SCI_TRACE_LOW("%s: cur_chapter_idx = %d, cur_section_children_idx = %d", __FUNCTION__, hanzi_book_info.cur_chapter_idx, hanzi_book_info.cur_section_children_idx);
            if(cJSON_GetArraySize(root) > hanzi_book_info.cur_section_children_idx)
            {
                cJSON *list_item =  cJSON_GetArrayItem(root, hanzi_book_info.cur_section_children_idx);
                cJSON *id = cJSON_GetObjectItem(list_item, "id");
                cJSON *baseUrl = cJSON_GetObjectItem(list_item, "baseUrl");
                cJSON *hanzis = cJSON_GetObjectItem(list_item, "hanzis");
                req_chapter_id = hanzi_content_info[hanzi_book_info.cur_section_idx]->chapter[hanzi_book_info.cur_section_children_idx]->chapter_id;
                if(id->valueint == req_chapter_id)
                {
                    hanzi_detail_count = cJSON_GetArraySize(hanzis);
                    SCI_TRACE_LOW("%s: hanzi_detail_count = %d", __FUNCTION__, hanzi_detail_count);
                    if(hanzi_detail_count > HANZI_CHAPTER_WORD_MAX){
                        hanzi_detail_count = HANZI_CHAPTER_WORD_MAX;
                    }
                    for(j = 0;j < cJSON_GetArraySize(hanzis) && j < HANZI_CHAPTER_WORD_MAX;j++)
                    {
                        cJSON *hanzis_item =  cJSON_GetArrayItem(hanzis, j);
                        cJSON *text = cJSON_GetObjectItem(hanzis_item, "text");
                        cJSON *audio = cJSON_GetObjectItem(hanzis_item, "audio");
                        cJSON *pinyin = cJSON_GetObjectItem(hanzis_item, "pinyin");
                        cJSON * similar_word = cJSON_GetObjectItem(hanzis_item, "similar_word");
                        cJSON *annotation = cJSON_GetObjectItem(hanzis_item, "annotation");
                        
                        Hanzi_AddDetailInfo(j, text, audio, pinyin, similar_word, annotation, baseUrl);
                    }
				//	#ifndef TF_CARD_SUPPORT
                    if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
                        if(!hanzi_is_load_local)
                        {
                            char * out = NULL;
                            char file_path[40] = {0};
                            sprintf(file_path, HANZI_CARD_CHAPTER_WORD_PATH, 
                                hanzi_book_info.cur_book_idx+1, 
                                hanzi_content_info[hanzi_book_info.cur_section_idx]->content_id
                            );
                            if(zmt_file_exist(file_path)){
                                zmt_file_delete(file_path);
                            }
                            out = cJSON_PrintUnformatted(root);
                            zmt_file_data_write(out, strlen(out), file_path);
                            SCI_FREE(out);
                        }
                    }
			//	#endif
                }
                else
                {
                    hanzi_detail_count = -2;
                }
            }
            else
            {
                hanzi_detail_count = -2;
            }
            cJSON_Delete(root);
        }
        else
        {
            hanzi_detail_count = -2;
        }
    }
    else
    {
        hanzi_detail_count = -1;
    }
    if(hanzi_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    hanzi_is_load_local = FALSE;
    if(is_open_auto_play && hanzi_detail_count > 0){
        HanziDetail_PlayPinyinAudio();
    }
    MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

PUBLIC void Hanzi_requestDetailInfo(uint8 grade_id, uint16 req_id)
{
    char file_path[40] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    SCI_TRACE_LOW("%s: grade_id = %d, req_id = %d", __FUNCTION__,grade_id, req_id);
    sprintf(file_path, HANZI_CARD_CHAPTER_WORD_PATH, grade_id, req_id);
    Hanzi_ReleaseDetailInfo();
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            hanzi_is_load_local = TRUE;
            Hanzi_ParseDetailInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        char url[220] = {0};
        char * query_str = makeBaseQueryUrlString(HANZI_BOOK_APP_ID, HANZI_BOOK_APP_SECRET);
        sprintf(url, HANZI_BOOK_CHAPTER_PATH, req_id, query_str);
        SCI_FREE(query_str);
        SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
        hanzi_is_load_local = FALSE;
        MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseDetailInfo);
    }
}

PUBLIC void Hanzi_ParseUnmasterDetailInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint16 i = 0;
        uint16 j = 0;
        uint16 read_idx = 0;
        uint8 read_count = 0;
        cJSON * root;
        cJSON * hanzis;
        cJSON * hanzis_item;
        cJSON * text;
        cJSON * audio;
        cJSON * pinyin;
        cJSON * similar_word;
        cJSON * annotation;

        root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            hanzis = cJSON_GetObjectItem(root, "hanzis");
            hanzi_detail_count = cJSON_GetArraySize(hanzis);
            read_idx = cur_new_hanzi_page_idx * HANZI_CHAPTER_WORD_MAX;
            read_count = (cur_new_hanzi_page_idx + 1) * HANZI_CHAPTER_WORD_MAX;
            for(i = read_idx; i < cJSON_GetArraySize(hanzis) && i < read_count;i++)
            {
                hanzis_item = cJSON_GetArrayItem(hanzis, i);
                text = cJSON_GetObjectItem(hanzis_item, "text");
                audio = cJSON_GetObjectItem(hanzis_item, "audio");
                pinyin = cJSON_GetObjectItem(hanzis_item, "pinyin");
                similar_word = cJSON_GetObjectItem(hanzis_item, "similar_word");
                annotation = cJSON_GetObjectItem(hanzis_item, "annotation");

                Hanzi_AddDetailInfo(j, text, audio, pinyin, similar_word, annotation, NULL);
                j++;
            }
            cJSON_Delete(root);
        }
        else
        {
            hanzi_detail_count = -3;
        }
    }
    else
    {
        hanzi_detail_count = -3;
    }
    if(hanzi_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    hanzi_is_load_local = FALSE;
    if(is_open_auto_play && hanzi_detail_count > 0){
        HanziDetail_PlayPinyinAudio();
    }
    MMK_PostMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

PUBLIC void Hanzi_RequestNewWord(uint16 grade_id, uint16 chap_id)
{
    char file_path[50] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    sprintf(file_path, HANZI_CARD_NEW_HANZI_PATH, grade_id, chap_id);
    Hanzi_ReleaseDetailInfo();
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            hanzi_is_load_local = TRUE;
            Hanzi_ParseUnmasterDetailInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        //char url[200] = {0};
        hanzi_is_load_local = FALSE;
        Hanzi_ParseUnmasterDetailInfo(0, data_buf, file_len,0);
        //sprintf(url, "v1/card/chapters/hanzi?F_cardId=%s&&F_section_id=%d", HANZI_BASE_DEVICE_IMEI, section_id);
        //MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseUnmasterDetailInfo);
    }
}

PUBLIC void Hanzi_DeleteOneNewWord(uint16 idx, uint16 count)
{
    uint8 i = 0;
    uint8 j = 0;
    uint16 size = 0;
    HANZI_BOOK_HANZI_INFO * new_hanzi_detail[HANZI_CHAPTER_WORD_MAX];
    SCI_TRACE_LOW("%s: idx = %d, hanzi_detail_count = %d", __FUNCTION__, idx, count);
    if(count == 1 && idx == 0){
        hanzi_detail_count = 0;
        MMI_CloseHanziDetailWin();
        return;
    }
    for(i = 0;i < count && i < HANZI_CHAPTER_WORD_MAX;i++)
    {
        if(i == idx){
            continue;
        }
        new_hanzi_detail[j] = SCI_ALLOC_APPZ(sizeof(HANZI_BOOK_HANZI_INFO));
        memset(new_hanzi_detail[j], 0, sizeof(HANZI_BOOK_HANZI_INFO));

        size = strlen(hanzi_detail_info[i]->word);
        new_hanzi_detail[j]->word = (char *)SCI_ALLOC_APP(size + 1);
        memset(new_hanzi_detail[j]->word, 0, size + 1);
        SCI_MEMCPY(new_hanzi_detail[j]->word, hanzi_detail_info[i]->word, size);

        size = strlen(hanzi_detail_info[i]->pingy);
        new_hanzi_detail[j]->pingy = (char *)SCI_ALLOC_APP(size + 1);
        memset(new_hanzi_detail[j]->pingy, 0, size + 1);
        SCI_MEMCPY(new_hanzi_detail[j]->pingy, hanzi_detail_info[i]->pingy, size);

        if(hanzi_detail_info[i]->audio_uri != NULL){
            size = strlen(hanzi_detail_info[i]->audio_uri);
            new_hanzi_detail[j]->audio_uri = (char *)SCI_ALLOC_APP(size + 1);
            memset(new_hanzi_detail[j]->audio_uri, 0, size + 1);
            SCI_MEMCPY(new_hanzi_detail[j]->audio_uri, hanzi_detail_info[i]->audio_uri, size);
        }else{
            new_hanzi_detail[j]->audio_uri = NULL;
        }
        
        new_hanzi_detail[j]->audio_len = hanzi_detail_info[i]->audio_len;

        if(hanzi_detail_info[i]->audio_data != NULL){
            size = strlen(hanzi_detail_info[i]->audio_data);
            new_hanzi_detail[j]->audio_data = (char *)SCI_ALLOC_APP(size + 1);
            memset(new_hanzi_detail[j]->audio_data, 0, size + 1);
            SCI_MEMCPY(new_hanzi_detail[j]->audio_data, hanzi_detail_info[i]->audio_data, size);
        }else{
            new_hanzi_detail[j]->audio_data = NULL;
        }

        if(hanzi_detail_info[i]->similar_word != NULL){
            size = strlen(hanzi_detail_info[i]->similar_word);
            new_hanzi_detail[j]->similar_word = (char *)SCI_ALLOC_APP(size + 1);
            memset(new_hanzi_detail[j]->similar_word, 0, size + 1);
            SCI_MEMCPY(new_hanzi_detail[j]->similar_word, hanzi_detail_info[i]->similar_word, size);
        }else{
            new_hanzi_detail[j]->similar_word = NULL;
        }

        if(hanzi_detail_info[i]->annotation != NULL){
            size = strlen(hanzi_detail_info[i]->annotation);
            new_hanzi_detail[j]->annotation = (char *)SCI_ALLOC_APP(size + 1);
            memset(new_hanzi_detail[j]->annotation, 0, size + 1);
            SCI_MEMCPY(new_hanzi_detail[j]->annotation, hanzi_detail_info[i]->annotation, size);
        }else{
            new_hanzi_detail[j]->annotation = NULL;
        }

        if(hanzi_detail_info[i]->remark != NULL){
            size = strlen(hanzi_detail_info[i]->remark);
            new_hanzi_detail[j]->remark = (char *)SCI_ALLOC_APP(size + 1);
            memset(new_hanzi_detail[j]->remark, 0, size + 1);
            SCI_MEMCPY(new_hanzi_detail[j]->remark, hanzi_detail_info[i]->remark, size);
        }else{
            new_hanzi_detail[j]->remark = NULL;
        }
        j++;
    }
    Hanzi_ReleaseDetailInfo();
    for(i = 0;i < count - 1 && i < HANZI_CHAPTER_WORD_MAX;i++)
    {
        hanzi_detail_info[i] = SCI_ALLOC_APPZ(sizeof(HANZI_BOOK_HANZI_INFO));
        memset(hanzi_detail_info[i], 0, sizeof(HANZI_BOOK_HANZI_INFO));

        size = strlen(new_hanzi_detail[i]->word);
        hanzi_detail_info[i]->word = (char *)SCI_ALLOC_APP(size + 1);
        memset(hanzi_detail_info[i]->word, 0, size + 1);
        SCI_MEMCPY(hanzi_detail_info[i]->word, new_hanzi_detail[i]->word, size);

        size = strlen(new_hanzi_detail[i]->pingy);
        hanzi_detail_info[i]->pingy = (char *)SCI_ALLOC_APP(size + 1);
        memset(hanzi_detail_info[i]->pingy, 0, size + 1);
        SCI_MEMCPY(hanzi_detail_info[i]->pingy, new_hanzi_detail[i]->pingy, size);

        if(new_hanzi_detail[i]->audio_uri != NULL){
            size = strlen(new_hanzi_detail[i]->audio_uri);
            hanzi_detail_info[i]->audio_uri = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[i]->audio_uri, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[i]->audio_uri, new_hanzi_detail[i]->audio_uri, size);
        }else{
            hanzi_detail_info[i]->audio_uri = NULL;
        }

        hanzi_detail_info[i]->audio_len = new_hanzi_detail[i]->audio_len;

        if(new_hanzi_detail[i]->audio_data != NULL){
            size = strlen(new_hanzi_detail[i]->audio_data);
            hanzi_detail_info[i]->audio_data = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[i]->audio_data, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[i]->audio_data, new_hanzi_detail[i]->audio_data, size);
        }else{
            hanzi_detail_info[i]->audio_data = NULL;
        }

        if(new_hanzi_detail[i]->similar_word != NULL){
            size = strlen(new_hanzi_detail[i]->similar_word);
            hanzi_detail_info[i]->similar_word = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[i]->similar_word, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[i]->similar_word, new_hanzi_detail[i]->similar_word, size);
        }else{
            hanzi_detail_info[i]->similar_word = NULL;
        }

        if(new_hanzi_detail[i]->annotation != NULL){
            size = strlen(new_hanzi_detail[i]->annotation);
            hanzi_detail_info[i]->annotation = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[i]->annotation, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[i]->annotation, new_hanzi_detail[i]->annotation, size);
        }else{
            hanzi_detail_info[i]->annotation = NULL;
        }

        if(new_hanzi_detail[i]->remark != NULL){
            size = strlen(new_hanzi_detail[i]->remark);
            hanzi_detail_info[i]->remark = (char *)SCI_ALLOC_APP(size + 1);
            memset(hanzi_detail_info[i]->remark, 0, size + 1);
            SCI_MEMCPY(hanzi_detail_info[i]->remark, new_hanzi_detail[i]->remark, size);
        }else{
            hanzi_detail_info[i]->remark = NULL;
        }
    }
    for(i = 0;i < j;i++)
    {
        if(new_hanzi_detail[i] != NULL){
            if(new_hanzi_detail[i]->word != NULL){
                SCI_FREE(new_hanzi_detail[i]->word);
                new_hanzi_detail[i]->word = NULL;
            }
            if(new_hanzi_detail[i]->pingy != NULL){
                SCI_FREE(new_hanzi_detail[i]->pingy);
                new_hanzi_detail[i]->pingy = NULL;
            }
            if(new_hanzi_detail[i]->audio_uri != NULL){
                SCI_FREE(new_hanzi_detail[i]->audio_uri);
                new_hanzi_detail[i]->audio_uri = NULL;
            }
            if(new_hanzi_detail[i]->audio_data != NULL){
                SCI_FREE(new_hanzi_detail[i]->audio_data);
                new_hanzi_detail[i]->audio_data = NULL;
            }
            if(new_hanzi_detail[i]->similar_word != NULL){
                SCI_FREE(new_hanzi_detail[i]->similar_word);
                new_hanzi_detail[i]->similar_word = NULL;
            }
            if(new_hanzi_detail[i]->annotation != NULL){
                SCI_FREE(new_hanzi_detail[i]->annotation);
                new_hanzi_detail[i]->annotation = NULL;
            }
            if(new_hanzi_detail[i]->remark != NULL){
                SCI_FREE(new_hanzi_detail[i]->remark);
                new_hanzi_detail[i]->remark = NULL;
            }
            SCI_FREE(new_hanzi_detail[i]);
            new_hanzi_detail[i] = NULL;
        }
    }
    if(count > 1)
    {
        hanzi_detail_count--;
        MMK_SendMsg(MMI_HANZI_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL);
        if(is_open_auto_play){
            HanziDetail_PlayPinyinAudio();
        }
    }
    else
    {
        hanzi_detail_count = 0;
        MMK_CloseWin(MMI_HANZI_DETAIL_WIN_ID);
    }
}

PUBLIC void Hanzi_SaveDeleteNewWord(uint16 grade_id, uint16 chap_id)
{
    char file_path[50] = {0};
    uint16 i = 0;
    char * out = NULL;
    cJSON * root;
    cJSON * hanzis;
    cJSON * hanzis_item;
    cJSON * text;
    cJSON * audio;
    cJSON * pinyin;
    cJSON * similar_word;
    cJSON * annotation;
    
    sprintf(file_path, HANZI_CARD_NEW_HANZI_PATH, grade_id, chap_id);
    if(hanzi_detail_count < 1)
    {
        if(zmt_file_exist(file_path)){
            zmt_file_delete(file_path);
        }
        return;
    }
    root = cJSON_CreateObject();
    hanzis = cJSON_CreateArray();
    for(i = 0;i < hanzi_detail_count && i < HANZI_CHAPTER_WORD_MAX;i++)
    {
        hanzis_item = cJSON_CreateObject();
        
        cJSON_AddStringToObject(hanzis_item, "text", hanzi_detail_info[i]->word);

        cJSON_AddStringToObject(hanzis_item, "pinyin", hanzi_detail_info[i]->pingy);

        cJSON_AddStringToObject(hanzis_item, "audio", hanzi_detail_info[i]->audio_uri);

        if(hanzi_detail_info[i]->similar_word != NULL){
            cJSON_AddStringToObject(hanzis_item, "similar_word", hanzi_detail_info[i]->similar_word);
        }else{
            cJSON_AddStringToObject(hanzis_item, "similar_word", "");
        }

        if(hanzi_detail_info[i]->annotation != NULL){
            cJSON_AddStringToObject(hanzis_item, "annotation", hanzi_detail_info[i]->annotation);
        }else{
            cJSON_AddStringToObject(hanzis_item, "annotation", "");
        }

        cJSON_AddItemToArray(hanzis, hanzis_item);
    }
    cJSON_AddItemToObject(root, "hanzis", hanzis);
    
#ifndef WIN32
    if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024)
    {
        if(zmt_file_exist(file_path)){
            zmt_file_delete(file_path);
        }
        out = cJSON_PrintUnformatted(root);
        zmt_file_data_write(out, strlen(out), file_path);
        SCI_FREE(out);
    }
#else
    out = cJSON_PrintUnformatted(root);
    zmt_file_data_write(out, strlen(out), file_path);
    SCI_FREE(out);
#endif
    cJSON_Delete(root);
}

PUBLIC void Hanzi_ParseMp3Response(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        if(hanzi_detail_info[hanzi_detail_cur_idx] == NULL){
            SCI_TRACE_LOW("%s: hanzi_detail_info empty", __FUNCTION__);
            return;
        }
        hanzi_detail_info[hanzi_detail_cur_idx]->audio_len = Rcv_len;
        if(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data != PNULL)
        {
            SCI_FREE(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data);
            hanzi_detail_info[hanzi_detail_cur_idx]->audio_data = NULL;
        }
        hanzi_detail_info[hanzi_detail_cur_idx]->audio_data = SCI_ALLOCA(Rcv_len);
        SCI_MEMSET(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data, 0, Rcv_len);
        SCI_MEMCPY(hanzi_detail_info[hanzi_detail_cur_idx]->audio_data, pRcv, Rcv_len);
	//	#ifndef TF_CARD_SUPPORT
        if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
            char file_path[30] = {0};
            sprintf(file_path, HANZI_CARD_WORD_AUDIO_PATH, hanzi_book_info.cur_book_idx+1, hanzi_detail_info[hanzi_detail_cur_idx]->pingy);
            if(zmt_file_exist(file_path)){
                zmt_file_delete(file_path);
            }
            zmt_file_data_write(pRcv, Rcv_len, file_path);
        }else{
        #ifdef LISTENING_PRATICE_SUPPORT
            MMI_CreateListeningTipWin(PALYER_PLAY_NO_SPACE_TIP);
        #endif
        }
	//	#endif
    }
    else
    {
        if(hanzi_detail_info[hanzi_detail_cur_idx] == NULL){
            SCI_TRACE_LOW("%s: hanzi_detail_info empty", __FUNCTION__);
            return;
        }
        hanzi_detail_info[hanzi_detail_cur_idx]->audio_len = -2;
    }
    HanziDetail_PlayPinyinAudio();
}


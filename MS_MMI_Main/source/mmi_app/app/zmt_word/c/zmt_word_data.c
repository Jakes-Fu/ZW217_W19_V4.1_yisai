/*************************************************************************
 ** File Name:      mmiacc_id.c                                          *
 ** Author:         haiyang.hu                                           *

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "cjson.h"
#include "zmt_word_main.h"
#include "zmt_word_id.h"
#include "zmt_word_text.h"
//#include "zmt_main_file.h"
#include "zmt_hanzi_main.h"
//#include "zmt_listening_export.h"


BOOLEAN word_is_load_local = FALSE;

extern WORD_LEARN_INFO_T * word_learn_info;
extern WORD_BOOK_INFO_T word_book_info;
extern int8 word_publish_count;
extern int8 word_chapter_count;
extern int16 word_detail_count;
extern WORD_BOOK_PUBLISH_INFO_T * word_publish_info[WORD_PUBLISH_MAX];
extern WORD_BOOK_CHAPTER_T * word_chapter_info[WORD_CHAPTER_NUM_MAX];
extern WORD_BOOK_DETAIL_T * new_word_detail_info[WORD_CHAPTER_WORD_MAX];
extern int chapter_unmaster_idx[WORD_CHAPTER_WORD_MAX];
extern int16 word_detail_cur_idx;
extern BOOLEAN word_open_auto_play;
extern BOOLEAN is_open_new_word;

PUBLIC void Word_UpdateBookInfo(void)
{
    char url[200] = {0};
    char * query_str = NULL;
    query_str = makeBaseQueryUrlString(WORD_BOOK_APP_ID, WORD_BOOK_APP_SECRET);
    sprintf(url, WORD_BOOK_PUBLISH_PATH, query_str);
    SCI_FREE(query_str);
    //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
    word_is_load_local = FALSE;
    MMIZDT_HTTP_AppSend(TRUE, WORD_BOOK_HEADR_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Word_ParseBookInfo);
}

PUBLIC void Word_ReleaseLearnInfo(void)
{
    if(word_learn_info != NULL)
    {
        SCI_FREE(word_learn_info);
        word_learn_info = NULL;
    }
}

PUBLIC void Word_WriteLearnInfo(void)
{
    char * out = NULL;
    char file_path[50] = {0};
    cJSON * root;
    cJSON * update_times;
    cJSON * publish_id;
    cJSON * book_id;
    cJSON * unit_id;
    
    if(word_learn_info == NULL)
    {
        SCI_TRACE_LOW("%s: word_learn_info empty!!", __FUNCTION__);
        return;
    }
    
    root = cJSON_CreateObject();
    update_times = cJSON_CreateNumber(word_learn_info->update_times);
    cJSON_AddItemToObject(root, "update_times", update_times);
    publish_id = cJSON_CreateNumber(word_learn_info->publish_id);
    cJSON_AddItemToObject(root, "publish_id", publish_id);
    book_id = cJSON_CreateNumber(word_learn_info->book_id);
    cJSON_AddItemToObject(root, "book_id", book_id);
    unit_id = cJSON_CreateNumber(word_learn_info->unit_id);
    cJSON_AddItemToObject(root, "unit_id", unit_id);

    out = cJSON_PrintUnformatted(root);
    strcpy(file_path, WORD_LEARN_INFO_PATH);
    if(zmt_file_exist(file_path)){
        zmt_file_delete(file_path);
    }
    zmt_file_data_write(out, strlen(out), file_path);
    cJSON_Delete(root);
    SCI_FREE(out);
}

PUBLIC void Word_UpdateLearnInfo(uint16 publish_id, uint16 book_id, uint16 unit_id)
{
    if(word_learn_info == NULL){
        uint32 cur_times = MMIAPICOM_GetCurTime();
        word_learn_info = SCI_ALLOC_APPZ(sizeof(WORD_LEARN_INFO_T));
        memset(word_learn_info, 0, sizeof(WORD_LEARN_INFO_T));
        word_learn_info->update_times = cur_times;
    }
    word_learn_info->publish_id = publish_id;
    word_learn_info->book_id = book_id;
    word_learn_info->unit_id = unit_id;
}

PUBLIC BOOLEAN Word_LoadLearnInfo(void)
{   
    BOOLEAN load_result = FALSE;
    char file_path[50] = {0};
    char * data_buf = PNULL;
    uint32 data_size = 0;

    if(word_learn_info != NULL){
        return load_result;
    }

    strcpy(file_path, WORD_LEARN_INFO_PATH);
    if(zmt_file_exist(file_path))
    {
        data_buf = zmt_file_data_read(file_path, &data_size);
        if(data_buf != PNULL && data_size > 0)
        {
            cJSON * root;
            cJSON * update_times;
            cJSON * publish_id;
            cJSON * book_id;
            cJSON * unit_id;

            root = cJSON_Parse(data_buf);
            if(root != NULL && root->type != cJSON_NULL)
            {
                update_times = cJSON_GetObjectItem(root, "update_times");
                publish_id = cJSON_GetObjectItem(root, "publish_id");
                book_id = cJSON_GetObjectItem(root, "book_id");
                unit_id = cJSON_GetObjectItem(root, "unit_id");
                
                if(word_learn_info == NULL){
                    word_learn_info = SCI_ALLOC_APPZ(sizeof(WORD_LEARN_INFO_T));
                }
                memset(word_learn_info, 0, sizeof(WORD_LEARN_INFO_T));
                word_learn_info->update_times = update_times->valueint;
                word_learn_info->publish_id = publish_id->valueint;
                word_learn_info->book_id = book_id->valueint;
                word_learn_info->unit_id = unit_id->valueint;
                
                load_result = TRUE;
                cJSON_Delete(root);
            }
            SCI_FREE(data_buf);
        }
    }
    if(load_result){
        uint32 cur_times = MMIAPICOM_GetCurTime();
        if(cur_times - word_learn_info->update_times >= WORD_BOOK_UPDATE_INTERVAL_TIMES)
        {
            word_learn_info->update_times = cur_times;
            Word_UpdateBookInfo();
            load_result = FALSE;
        }
    }
    return load_result;
}

LOCAL void Word_WriteExistUnmasterChapterWord(uint16 book_id, uint16 chap_id,cJSON * word, uint8 write_count)
{
    uint8 i = 0;
    uint8 j = 0;
    char file_path[50] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    BOOLEAN exist_chapter = FALSE;
    cJSON * exist_content_item = NULL;
    
    sprintf(file_path,WORD_BOOK_NEW_WORD_PATH, book_id, chap_id);
    data_buf = zmt_file_data_read(file_path, &file_len);
    SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
    if(data_buf != PNULL && file_len > 0)
    {
        cJSON * root;
        cJSON * words;
        cJSON * word_items;
        cJSON * texts;
        cJSON * audios;
        cJSON * phonetics;
        cJSON * translations;

        root = cJSON_Parse(data_buf);
        if(root != NULL && root->type != cJSON_NULL)
        {
            words = cJSON_GetObjectItem(root, "word");
            for(i = 0;i < cJSON_GetArraySize(words) && i < WORD_CHAPTER_WORD_MAX;i++)
            {
                BOOLEAN is_exist = FALSE;
                cJSON *word_item =  cJSON_GetArrayItem(words, i);
                cJSON *text = cJSON_GetObjectItem(word_item, "text");
                cJSON *audio = cJSON_GetObjectItem(word_item, "audio");
                cJSON *phonetic = cJSON_GetObjectItem(word_item, "phonetic");
                cJSON *translation = cJSON_GetObjectItem(word_item, "translation");

                for(j = 0;j < write_count;j++)
                {
                    uint8 idx = chapter_unmaster_idx[j] - 1;
                    SCI_TRACE_LOW("%s: detail[%d]->word = %s", __FUNCTION__, idx, word_chapter_info[word_book_info.cur_chapter_idx]->detail[idx]->word);
                    if(text->valuestring != NULL)
                    {
                         SCI_TRACE_LOW("%s: texts->valuestring = %s", __FUNCTION__, text->valuestring);
                        if(0 == strcmp(text->valuestring, word_chapter_info[word_book_info.cur_chapter_idx]->detail[idx]->word))
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
                
                word_items = cJSON_CreateObject();
                    
                texts = cJSON_CreateString(text->valuestring);
                cJSON_AddItemToObject(word_items, "text", texts);

                audios = cJSON_CreateString(audio->valuestring);
                cJSON_AddItemToObject(word_items, "audio", audios);

                phonetics = cJSON_CreateString(phonetic->valuestring);
                cJSON_AddItemToObject(word_items, "phonetic", phonetics);

                translations = cJSON_CreateString(translation->valuestring);
                cJSON_AddItemToObject(word_items, "translation", translations);

                cJSON_AddItemToArray(word, word_items);
            }
            cJSON_Delete(root);
        }
        SCI_FREE(data_buf);
    }
}

PUBLIC void Word_WriteUnmasterChapterWord(uint16 book_id, uint16 chap_id, char * chap_name, uint8 write_count)
{
    uint16 i = 0;
    uint16 index = 0;
    char * out = NULL;
    char file_path[50] = {0};
    char * pRcv = NULL;
    uint32 data_size = 0;
    cJSON * root;
    cJSON * chapter_id;
    cJSON * chapter_name;
    cJSON * baseUrl;
    cJSON * word;
    cJSON * word_item;
    cJSON * text;
    cJSON * audio;
    cJSON * phonetic;
    cJSON * translation;

    if(write_count <= 0){
        SCI_TRACE_LOW("%s: UnmasterWord no to write", __FUNCTION__);
        return;
    }
    SCI_TRACE_LOW("%s: write_count = %d", __FUNCTION__, write_count);

    root = cJSON_CreateObject();
    chapter_id = cJSON_CreateNumber(chap_id);
    cJSON_AddItemToObject(root, "id", chapter_id);

    chapter_name = cJSON_CreateString(chap_name);
    cJSON_AddItemToObject(root, "contentName", chapter_name);

    baseUrl = cJSON_CreateString(WORD_BOOK_AUDIO_BASE_URL);
    cJSON_AddItemToObject(root, "baseUrl", baseUrl);

    word = cJSON_CreateArray();

    sprintf(file_path,WORD_BOOK_NEW_WORD_PATH, book_id, chap_id);
    if(zmt_file_exist(file_path)){
        Word_WriteExistUnmasterChapterWord(book_id, chap_id, word, write_count);
    }
    for(i = 0;i < WORD_CHAPTER_WORD_MAX && i < write_count;i++)
    {
        if(chapter_unmaster_idx[i] != 0)
        {
            index = chapter_unmaster_idx[i] - 1;
            word_item = cJSON_CreateObject();
            if(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->word != NULL){
                text = cJSON_CreateString(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->word);
                cJSON_AddItemToObject(word_item, "text", text);
            }else{
                cJSON_AddStringToObject(word_item, "text", "");
            }

            if(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->audio_uri != NULL){
                audio = cJSON_CreateString(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->audio_uri);
                cJSON_AddItemToObject(word_item, "audio", audio);
            }else{
                cJSON_AddStringToObject(word_item, "audio", "");
            }

            if(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->phonetic != NULL){
                phonetic = cJSON_CreateString(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->phonetic);
                cJSON_AddItemToObject(word_item, "phonetic", phonetic);
            }else{
                cJSON_AddStringToObject(word_item, "phonetic", "");
            }

            if(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->translation != NULL){
                translation = cJSON_CreateString(word_chapter_info[word_book_info.cur_chapter_idx]->detail[index]->translation);
                cJSON_AddItemToObject(word_item, "translation", translation);
            }else{
                cJSON_AddStringToObject(word_item, "translation", "");
            }
            cJSON_AddItemToArray(word, word_item);
        }
    }
    cJSON_AddItemToObject(root, "word", word);

    out = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if(zmt_file_exist(file_path)){
        zmt_file_delete(file_path);
    }
    zmt_file_data_write(out, strlen(out), file_path);
    SCI_FREE(out);
}

PUBLIC void Word_ReleaseBookInfo(void)
{
    uint8 i = 0;
    uint8 j = 0;
    for(i = 0;i < WORD_PUBLISH_MAX;i++)
    {
        if(word_publish_info[i] != NULL)
        {
            for(j = 0;j < WORD_PUBLISH_BOOK_MAX;j++)
            {
                if(word_publish_info[i]->item_info[j] != NULL)
                {
                    if(word_publish_info[i]->item_info[j]->book_name != NULL)
                    {
                        SCI_FREE(word_publish_info[i]->item_info[j]->book_name);
                        word_publish_info[i]->item_info[j]->book_name = NULL;
                    }
                    SCI_FREE(word_publish_info[i]->item_info[j]);
                    word_publish_info[i]->item_info[j] = NULL;
                }
            }
			if(word_publish_info[i]->publish_name != NULL){
				SCI_FREE(word_publish_info[i]->publish_name);
				word_publish_info[i]->publish_name = NULL;
			}
			SCI_FREE(word_publish_info[i]);
			word_publish_info[i] = NULL;
        }
    }
}

PUBLIC void Word_ParseBookInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint8 m = 0;
        uint16 size = 0;
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            word_publish_count = cJSON_GetArraySize(root);
            for(i = 0;i < word_publish_count && i < WORD_PUBLISH_MAX;i++)
            {              
                cJSON *list_item =  cJSON_GetArrayItem(root, i);
                cJSON *publishId = cJSON_GetObjectItem(list_item, "id");
                cJSON *publishName = cJSON_GetObjectItem(list_item, "publishName");
                cJSON *book = cJSON_GetObjectItem(list_item, "book");
                
                if(word_publish_info[i] != NULL){
                    SCI_FREE(word_publish_info[i]);
                    word_publish_info[i] = NULL;
                }
                word_publish_info[i] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_PUBLISH_INFO_T));
                memset(word_publish_info[i], 0, sizeof(WORD_BOOK_PUBLISH_INFO_T));
                
                word_publish_info[i]->publish_id = publishId->valueint;
                
                if(word_publish_info[i]->publish_name != NULL){
                    SCI_FREE(word_publish_info[i]->publish_name);
                    word_publish_info[i]->publish_name = NULL;
                }
                size = strlen(publishName->valuestring);
                word_publish_info[i]->publish_name = SCI_ALLOC_APPZ(size + 1);
                memset(word_publish_info[i]->publish_name, 0, size + 1);
                SCI_MEMCPY(word_publish_info[i]->publish_name, publishName->valuestring, size);

                word_publish_info[i]->item_count = cJSON_GetArraySize(book);
                for(m = 0;m < cJSON_GetArraySize(book) && m < WORD_PUBLISH_BOOK_MAX;m++)
                {
                    cJSON *book_item =  cJSON_GetArrayItem(book, m);
                    cJSON *bookId = cJSON_GetObjectItem(book_item, "id");
                    cJSON *bookName = cJSON_GetObjectItem(book_item, "bookName");
                    cJSON *grade = cJSON_GetObjectItem(book_item, "grade");
                    
                    if(word_publish_info[i]->item_info[m] != NULL){
                        SCI_FREE(word_publish_info[i]->item_info[m]);
                        word_publish_info[i]->item_info[m] = NULL;
                    }
                    word_publish_info[i]->item_info[m] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_PUBLISH_ITEM_INFO_T));
                    memset(word_publish_info[i]->item_info[m], 0, sizeof(WORD_BOOK_PUBLISH_ITEM_INFO_T));

                    word_publish_info[i]->item_info[m]->book_id = bookId->valueint;
                    word_publish_info[i]->item_info[m]->grade = grade->valueint;

                    if(word_publish_info[i]->item_info[m]->book_name != NULL){
                        SCI_FREE(word_publish_info[i]->item_info[m]->book_name);
                        word_publish_info[i]->item_info[m]->book_name = NULL;
                    }
                    size = strlen(bookName->valuestring);
                    word_publish_info[i]->item_info[m]->book_name = SCI_ALLOC_APPZ(size + 1);
                    memset(word_publish_info[i]->item_info[m]->book_name, 0, size + 1);
                    SCI_MEMCPY(word_publish_info[i]->item_info[m]->book_name, bookName->valuestring, size);
                }
            }

            if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
                if(!word_is_load_local)
                {
					  char * out = NULL;
                    char file_path[30] = {0};
                    strcpy(file_path, WORD_PUBLISH_NAME_PATH);
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
            word_publish_count = -2;
        }	
	
    }
    else
    {
        word_publish_count = -1;
    }
    if(word_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
	SCI_TRACE_LOW("%s: word_publish_count = %d", __FUNCTION__, word_publish_count);
    word_is_load_local = FALSE;
    if(MMK_IsFocusWin(MMI_WORD_MAIN_WIN_ID)){
        MMK_SendMsg(MMI_WORD_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
}

PUBLIC void Word_requestBookInfo(void)
{
    char file_path[30] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    
    strcpy(file_path, WORD_PUBLISH_NAME_PATH);
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            word_is_load_local = TRUE;
            Word_ParseBookInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        char url[200] = {0};
        char * query_str = NULL;
        query_str = makeBaseQueryUrlString(WORD_BOOK_APP_ID, WORD_BOOK_APP_SECRET);
        sprintf(url, WORD_BOOK_PUBLISH_PATH, query_str);
        SCI_FREE(query_str);
        //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
        word_is_load_local = FALSE;
        MMIZDT_HTTP_AppSend(TRUE, WORD_BOOK_HEADR_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Word_ParseBookInfo);
    }
}

PUBLIC void Word_ReleaseChapterDetailInfo(void)
{
    uint8 i =0;
    uint8 j = 0;
    for(i = 0;i < WORD_CHAPTER_NUM_MAX;i++)
    {
        if(word_chapter_info[i] != NULL){
            for(j = 0;j < WORD_CHAPTER_WORD_MAX;j++)
            {
                if(word_chapter_info[i]->detail[j] != NULL){
                    if(word_chapter_info[i]->detail[j]->word != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]->word);
                        word_chapter_info[i]->detail[j]->word =NULL;
                    }
                    if(word_chapter_info[i]->detail[j]->audio_uri != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]->audio_uri);
                        word_chapter_info[i]->detail[j]->audio_uri =NULL;
                    }
                    if(word_chapter_info[i]->detail[j]->audio_data != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]->audio_data);
                        word_chapter_info[i]->detail[j]->audio_data =NULL;
                    }
                    if(word_chapter_info[i]->detail[j]->phonetic != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]->phonetic);
                        word_chapter_info[i]->detail[j]->phonetic =NULL;
                    }
                    if(word_chapter_info[i]->detail[j]->translation != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]->translation);
                        word_chapter_info[i]->detail[j]->translation =NULL;
                    }
                    SCI_FREE(word_chapter_info[i]->detail[j]);
                    word_chapter_info[i]->detail[j] = NULL;
                }
            }
            if(word_chapter_info[i]->chapter_name != NULL){
                SCI_FREE(word_chapter_info[i]->chapter_name);
                word_chapter_info[i]->chapter_name = NULL;
            }
            SCI_FREE(word_chapter_info[i]);
            word_chapter_info[i] = NULL;
        }
    }
}

LOCAL char * Word_ReplaceSpaceString(char * str, char* replacement)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int cnt = 0;
    int new_str_len = 0;
    char* new_str = NULL;
    int str_len = strlen(str);
    int replacement_len = strlen(replacement);
 
    // 计算需要替换的空字符串数量
    for (i = 0; i < str_len; ++i) {
        char str_s[5] = {0};
        sprintf(str_s, "%c", str[i]);
        if (0 == strcmp(str_s, " ")) {
            cnt++;
        }
    }
 
    // 如果没有需要替换的空字符串，直接返回
    if (cnt == 0) {
        return new_str;
    }
 
    // 计算新字符串的长度和需要的空间
    new_str_len = str_len - cnt * 1 + cnt * replacement_len;
 
    // 为新字符串分配空间
    new_str = (char*)SCI_ALLOC_APPZ(new_str_len + 1);
    memset(new_str, 0, new_str_len + 1);
 
    // 进行替换操作
    for (i = 0; i < str_len; ++i) {
        char str_s[5] = {0};
        sprintf(str_s, "%c", str[i]);
        if (0 == strcmp(str_s, " ")){
            for (k = 0; k < replacement_len; ++k) {
                new_str[j++] = replacement[k];
            }
        } else {
            new_str[j++] = str[i];
        }
    }
 
    // 添加结束符
    new_str[new_str_len] = '\0';
    SCI_TRACE_LOW("%s: new_str = %s", __FUNCTION__, new_str);
 
    return new_str;
}

LOCAL BOOLEAN Word_DetailMakeAudioUrl(WORD_BOOK_DETAIL_T * detail, char * text, char * baseUrl)
{
    //http://8.130.95.8:8866/file/english/taxi.mp3
    char * str = NULL;
    char * data = NULL;
    int len = 0;
    char url[100] = {0};
    if(text == NULL)
    {
        SCI_TRACE_LOW("%s: param is null", __FUNCTION__);
        return FALSE;
    }
    data = Word_ReplaceSpaceString(text, "%20");
    if(data != NULL){
        if(baseUrl != NULL){
            sprintf(url, WORD_BOOK_AUDIO_BASE_PATH, baseUrl, data);
        }else{
            sprintf(url, WORD_BOOK_AUDIO_BASE_PATH, WORD_BOOK_AUDIO_BASE_URL, data);
        }
        SCI_FREE(data);
    }else{
        if(baseUrl != NULL){
            sprintf(url, WORD_BOOK_AUDIO_BASE_PATH, baseUrl, text);
        }else{
            sprintf(url, WORD_BOOK_AUDIO_BASE_PATH, WORD_BOOK_AUDIO_BASE_URL, text);
        }
    }
    detail->audio_uri = (char *)SCI_ALLOC_APP(strlen(url) + 1);
    memset(detail->audio_uri, 0, strlen(url) + 1);
    SCI_MEMCPY(detail->audio_uri, url, strlen(url));
    //SCI_TRACE_LOW("%s: detail->audio_uri = %s", __FUNCTION__, detail->audio_uri);
    return TRUE;
}

LOCAL void Word_AddDetailInfo(WORD_BOOK_DETAIL_T * detail, cJSON* text, cJSON * audio, cJSON * phonetic, cJSON * translation, cJSON * baseUrl)
{
    BOOLEAN result = FALSE;
    uint32 size = 0;

    if(text != NULL && text->type != cJSON_NULL && text->valuestring != NULL){
        size = strlen(text->valuestring);
        if(size > 0){
            detail->word = (char *)SCI_ALLOC_APP(size + 1);
            memset(detail->word, 0, size + 1);
            SCI_MEMCPY(detail->word, text->valuestring, size);
        }else{
            detail->word = NULL;
        }
    }else{
        detail->word = NULL;
    }

    if(baseUrl == NULL && audio != NULL && audio->type != cJSON_NULL && audio->valuestring != NULL){
        size = strlen(audio->valuestring);
        if(size > 0){
            detail->audio_uri = (char *)SCI_ALLOC_APP(size + 1);
            memset(detail->audio_uri, 0, size + 1);
            SCI_MEMCPY(detail->audio_uri, audio->valuestring, size);
            detail->audio_len = 0;
        }else{
            detail->audio_uri = NULL;
            detail->audio_len = -1;
        }
    }else{
        result = Word_DetailMakeAudioUrl(detail, text->valuestring, baseUrl->valuestring);
        if(!result){
            detail->audio_uri = NULL;
            detail->audio_len = -1;
        }else{
            detail->audio_len = 0;
        }
    }

    if(phonetic != NULL && phonetic->type != cJSON_NULL && phonetic->valuestring != NULL){
        size = strlen(phonetic->valuestring);
        if(size > 0){
            detail->phonetic = (char *)SCI_ALLOC_APP(size + 1);
            memset(detail->phonetic, 0, size + 1);
            SCI_MEMCPY(detail->phonetic, phonetic->valuestring, size);
        }else{
            detail->phonetic = NULL;
        }
    }else{
        detail->phonetic = NULL;
    }

    if(translation != NULL && translation->type != cJSON_NULL && translation->valuestring != NULL){
        size = strlen(translation->valuestring);
        if(size > 0){
            detail->translation = (char *)SCI_ALLOC_APP(size + 1);
            memset(detail->translation, 0, size + 1);
            SCI_MEMCPY(detail->translation, translation->valuestring, size);
        }else{
            detail->translation = NULL;
        }
    }else{
        detail->translation = NULL;
    }
}

PUBLIC void Word_ParseChapterInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint8 i = 0;
        uint16 j = 0;
        uint16 size = 0;
        cJSON *root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            cJSON *baseUrl = cJSON_GetObjectItem(root, "baseUrl");
            cJSON *content = cJSON_GetObjectItem(root, "content");
            word_chapter_count = cJSON_GetArraySize(content);
            for(i = 0;i < cJSON_GetArraySize(content) && i < WORD_CHAPTER_NUM_MAX;i++)
            {
                cJSON *content_item =  cJSON_GetArrayItem(content, i);
                cJSON *id = cJSON_GetObjectItem(content_item, "id");
                cJSON *contentName = cJSON_GetObjectItem(content_item, "contentName");
                cJSON *word = cJSON_GetObjectItem(content_item, "word");

                if(word_chapter_info[i] != NULL){
                    SCI_FREE(word_chapter_info[i]);
                    word_chapter_info[i] = NULL;
                }
                word_chapter_info[i] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_CHAPTER_T));
                memset(word_chapter_info[i], 0, sizeof(WORD_BOOK_CHAPTER_T));
                
                word_chapter_info[i]->chapter_id = id->valueint;
                if(word_chapter_info[i]->chapter_name != NULL){
                    SCI_FREE(word_chapter_info[i]->chapter_name);
                    word_chapter_info[i]->chapter_name = NULL;
                }
                size = strlen(contentName->valuestring);
                word_chapter_info[i]->chapter_name = SCI_ALLOC_APPZ(size + 1);
                memset(word_chapter_info[i]->chapter_name, 0, size + 1);
                SCI_MEMCPY(word_chapter_info[i]->chapter_name, contentName->valuestring, size);

                word_chapter_info[i]->detail_count = cJSON_GetArraySize(word);
                for(j = 0;j < cJSON_GetArraySize(word) && j < WORD_CHAPTER_WORD_MAX;j++)
                {
                    cJSON *wordt_item =  cJSON_GetArrayItem(word, j);
                    cJSON *text = cJSON_GetObjectItem(wordt_item, "text");
                    cJSON *audio = cJSON_GetObjectItem(wordt_item, "audio");
                    cJSON *phonetic = cJSON_GetObjectItem(wordt_item, "phonetic");
                    cJSON *translation = cJSON_GetObjectItem(wordt_item, "translation");

                    if(word_chapter_info[i]->detail[j] != NULL){
                        SCI_FREE(word_chapter_info[i]->detail[j]);
                        word_chapter_info[i]->detail[j] = NULL;
                    }
                    word_chapter_info[i]->detail[j] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_DETAIL_T));
                    memset(word_chapter_info[i]->detail[j], 0, sizeof(WORD_BOOK_DETAIL_T));

                    Word_AddDetailInfo(word_chapter_info[i]->detail[j], text, audio, phonetic, translation, baseUrl);
                }
            }
			
            if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
                if(!word_is_load_local)
                {
                    char * out = NULL;
                    char file_path[40] = {0};
                    uint16 book_id = word_publish_info[word_book_info.cur_publish_idx]->item_info[word_book_info.cur_book_idx]->book_id;
                    sprintf(file_path, WORD_BOOK_CHAPTER_PATH, book_id, book_id);
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
            word_chapter_count = -2;
        }
	
    }
    else
    {
        word_chapter_count = -1;
    }
    if(word_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    word_is_load_local = FALSE;
    MMK_SendMsg(MMI_WORD_CHAPTER_WIN_ID, MSG_FULL_PAINT, PNULL);
}

PUBLIC void Word_requestChapterDetailInfo(uint16 book_id)
{
    char file_path[40] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    sprintf(file_path, WORD_BOOK_CHAPTER_PATH, book_id, book_id);
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            word_is_load_local = TRUE;
            Word_ParseChapterInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
        char url[200] = {0};
        char * query_str = makeBaseQueryUrlString(WORD_BOOK_APP_ID, WORD_BOOK_APP_SECRET);
        sprintf(url,WORD_BOOK_ID_PATH,book_id, query_str);
        SCI_FREE(query_str);
        //SCI_TRACE_LOW("%s: url = %s", __FUNCTION__, url);
        word_is_load_local = FALSE;
        MMIZDT_HTTP_AppSend(TRUE, WORD_BOOK_HEADR_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Word_ParseChapterInfo);
    }
}

PUBLIC Word_ReleaseNewWordInfo(void)
{
    uint8 i = 0;
    for(i = 0; i < WORD_CHAPTER_WORD_MAX;i++)
    {
        if(new_word_detail_info[i] != NULL){
            if(new_word_detail_info[i]->word != NULL){
                SCI_FREE(new_word_detail_info[i]->word);
                new_word_detail_info[i]->word = NULL;
            }
            if(new_word_detail_info[i]->audio_uri != NULL){
                SCI_FREE(new_word_detail_info[i]->audio_uri);
                new_word_detail_info[i]->audio_uri = NULL;
            }
            if(new_word_detail_info[i]->audio_data != NULL){
                SCI_FREE(new_word_detail_info[i]->audio_data);
                new_word_detail_info[i]->audio_data = NULL;
            }
            if(new_word_detail_info[i]->phonetic != NULL){
                SCI_FREE(new_word_detail_info[i]->phonetic);
                new_word_detail_info[i]->phonetic = NULL;
            }
            if(new_word_detail_info[i]->translation != NULL){
                SCI_FREE(new_word_detail_info[i]->translation);
                new_word_detail_info[i]->translation = NULL;
            }
            SCI_FREE(new_word_detail_info[i]);
            new_word_detail_info[i] = NULL;
        }
    }
}

LOCAL Word_ParseNewWordInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        uint16 i = 0;
        uint16 j = 0;
        cJSON * root = cJSON_Parse(pRcv);
        if(root != NULL && root->type != cJSON_NULL)
        {
            cJSON * baseUrl = cJSON_GetObjectItem(root, "baseUrl");
            cJSON * word = cJSON_GetObjectItem(root, "word");
            word_detail_count = cJSON_GetArraySize(word);
            for(i = 0;i < cJSON_GetArraySize(word) && i < WORD_CHAPTER_WORD_MAX;i++)
            {
                cJSON *wordt_item =  cJSON_GetArrayItem(word, i);
                cJSON *text = cJSON_GetObjectItem(wordt_item, "text");
                cJSON *audio = cJSON_GetObjectItem(wordt_item, "audio");
                cJSON *phonetic = cJSON_GetObjectItem(wordt_item, "phonetic");
                cJSON *translation = cJSON_GetObjectItem(wordt_item, "translation");
                
                if(new_word_detail_info[i] != NULL){
                    SCI_FREE(new_word_detail_info[i]);
                    new_word_detail_info[i] = NULL;
                }
                new_word_detail_info[i] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_DETAIL_T));
                memset(new_word_detail_info[i], 0, sizeof(WORD_BOOK_DETAIL_T));

                Word_AddDetailInfo(new_word_detail_info[i], text, audio, phonetic, translation, baseUrl);  
            }
            cJSON_Delete(root);
        }
        else
        {
            word_detail_count = -3;
        }
    }
    else
    {
        if(is_open_new_word){
            word_detail_count = -3;
        }else{
            word_detail_count = -2;
        }
    }
    if(word_is_load_local){
        if(pRcv != PNULL)
        {
        	SCI_FREE(pRcv);
        	pRcv = PNULL;
        }
    }
    word_is_load_local = FALSE;
    if(word_open_auto_play && word_detail_count > 0){
        WordDetail_PlayPinyinAudio();
    }
    MMK_PostMsg(MMI_WORD_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
}

PUBLIC void Word_RequestNewWord(uint16 book_id, uint16 chapter_id)
{
    char file_path[50] = {0};
    char * data_buf = PNULL;
    uint32 file_len = 0;
    sprintf(file_path, WORD_BOOK_NEW_WORD_PATH, book_id, chapter_id);
    Word_ReleaseNewWordInfo();
    if(zmt_file_exist(file_path)){
        data_buf = zmt_file_data_read(file_path, &file_len);
        SCI_TRACE_LOW("%s: file_len = %d", __FUNCTION__, file_len);
        if(data_buf != PNULL && file_len > 0)
        {
            word_is_load_local = TRUE;
            Word_ParseNewWordInfo(1, data_buf, file_len,0);
        }
    }
    else
    {
     //   char url[200] = {0};
        word_is_load_local = FALSE;
        Word_ParseNewWordInfo(0, data_buf, file_len,0);
      /* sprintf(url, "v1/card/chapters/hanzi?F_cardId=%s&&F_section_id=%d", HANZI_BASE_DEVICE_IMEI, section_id);
        MMIZDT_HTTP_AppSend(TRUE, HANZI_BOOK_HEADER_PATH, url, strlen(url), 1000, 0, 0, 0, 0, 0, Hanzi_ParseUnmasterDetailInfo);*/
    }
}

PUBLIC void Word_DeleteOneNewWord(uint8 idx, uint8 count)
{
    uint8 i = 0;
    uint8 j = 0;
    uint16 size = 0;
    WORD_BOOK_DETAIL_T * new_word_detail[WORD_CHAPTER_WORD_MAX];
    SCI_TRACE_LOW("%s: cur_idx = %d, count = %d", __FUNCTION__, idx, count);
    if(count == 1 && idx == 0){
        word_detail_count = 0;
        MMI_CloseWordDetailWin();
        return;
    }
    for(i = 0;i < count && i < WORD_CHAPTER_WORD_MAX;i++)
    {
        if(i == idx){
            continue;
        }
        new_word_detail[j] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_DETAIL_T));
        memset(new_word_detail[j], 0, sizeof(WORD_BOOK_DETAIL_T));

        size = strlen(new_word_detail_info[i]->word);
        new_word_detail[j]->word = SCI_ALLOC_APPZ(size+1);
        memset(new_word_detail[j]->word, 0, size+1);
        SCI_MEMCPY(new_word_detail[j]->word, new_word_detail_info[i]->word, size);

        size = strlen(new_word_detail_info[i]->audio_uri);
        new_word_detail[j]->audio_uri = SCI_ALLOC_APPZ(size+1);
        memset(new_word_detail[j]->audio_uri, 0, size+1);
        SCI_MEMCPY(new_word_detail[j]->audio_uri, new_word_detail_info[i]->audio_uri, size);

        if(new_word_detail_info[i]->audio_data != NULL){
            size = strlen(new_word_detail_info[i]->audio_data);
            new_word_detail[j]->audio_data = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail[j]->audio_data, 0, size+1);
            SCI_MEMCPY(new_word_detail[j]->audio_data, new_word_detail_info[i]->audio_data, size);
            new_word_detail[j]->audio_len = strlen(new_word_detail[j]->audio_data);
        }else{
            new_word_detail[j]->audio_data = NULL;
            new_word_detail[j]->audio_len = 0;
        }

        if(new_word_detail_info[i]->phonetic != NULL){
            size = strlen(new_word_detail_info[i]->phonetic);
            new_word_detail[j]->phonetic = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail[j]->phonetic, 0, size+1);
            SCI_MEMCPY(new_word_detail[j]->phonetic, new_word_detail_info[i]->phonetic, size);
        }else{
            new_word_detail[j]->phonetic = NULL;
        }

        if(new_word_detail_info[i]->translation != NULL){
            size = strlen(new_word_detail_info[i]->translation);
            new_word_detail[j]->translation = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail[j]->translation, 0, size+1);
            SCI_MEMCPY(new_word_detail[j]->translation, new_word_detail_info[i]->translation, size);
        }else{
            new_word_detail[j]->translation = NULL;
        }
        
        j++;
    }

    Word_ReleaseNewWordInfo();    
    for(i = 0;i < count - 1 && i < WORD_CHAPTER_WORD_MAX;i++)
    {
        new_word_detail_info[i] = SCI_ALLOC_APPZ(sizeof(WORD_BOOK_DETAIL_T));
        memset(new_word_detail_info[i], 0, sizeof(WORD_BOOK_DETAIL_T));

        size = strlen(new_word_detail[i]->word);
        new_word_detail_info[i]->word = SCI_ALLOC_APPZ(size+1);
        memset(new_word_detail_info[i]->word, 0, size+1);
        SCI_MEMCPY(new_word_detail_info[i]->word, new_word_detail[i]->word, size);

        size = strlen(new_word_detail[i]->audio_uri);
        new_word_detail_info[i]->audio_uri = SCI_ALLOC_APPZ(size+1);
        memset(new_word_detail_info[i]->audio_uri, 0, size+1);
        SCI_MEMCPY(new_word_detail_info[i]->audio_uri, new_word_detail[i]->audio_uri, size);       

        if(new_word_detail[i]->audio_data != NULL){
            size = strlen(new_word_detail[i]->audio_data);
            new_word_detail_info[i]->audio_data = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail_info[i]->audio_data, 0, size+1);
            SCI_MEMCPY(new_word_detail_info[i]->audio_data, new_word_detail[i]->audio_data, size);
            new_word_detail_info[i]->audio_len = strlen(new_word_detail_info[i]->audio_data);
        }else{
            new_word_detail_info[i]->audio_data = NULL;
            new_word_detail_info[i]->audio_len = 0;
        }

        if(new_word_detail[i]->phonetic != NULL){
            size = strlen(new_word_detail[i]->phonetic);
            new_word_detail_info[i]->phonetic = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail_info[i]->phonetic, 0, size+1);
            SCI_MEMCPY(new_word_detail_info[i]->phonetic, new_word_detail[i]->phonetic, size);
        }else{
            new_word_detail_info[i]->phonetic = NULL;
        }

        if(new_word_detail[i]->translation != NULL){
            size = strlen(new_word_detail[i]->translation);
            new_word_detail_info[i]->translation = SCI_ALLOC_APPZ(size+1);
            memset(new_word_detail_info[i]->translation, 0, size+1);
            SCI_MEMCPY(new_word_detail_info[i]->translation, new_word_detail[i]->translation, size);
        }else{
            new_word_detail_info[i]->translation = NULL;
        }
    }
    
    SCI_TRACE_LOW("%s: j = %d", __FUNCTION__, j);
    for(i = 0;i < j;i++)
    {
        if(new_word_detail[i] != NULL){
            if(new_word_detail[i]->word != NULL){
                SCI_FREE(new_word_detail[i]->word);
                new_word_detail[i]->word = NULL;
            }
            if(new_word_detail[i]->audio_uri != NULL){
                SCI_FREE(new_word_detail[i]->audio_uri);
                new_word_detail[i]->audio_uri = NULL;
            }
            if(new_word_detail[i]->audio_data != NULL){
                SCI_FREE(new_word_detail[i]->audio_data);
                new_word_detail[i]->audio_data = NULL;
            }
            if(new_word_detail[i]->phonetic != NULL){
                SCI_FREE(new_word_detail[i]->phonetic);
                new_word_detail[i]->phonetic = NULL;
            }
            if(new_word_detail[i]->translation != NULL){
                SCI_FREE(new_word_detail[i]->translation);
                new_word_detail[i]->translation = NULL;
            }
            SCI_FREE(new_word_detail[i]);
            new_word_detail[i] = NULL;
        }
    }
    
    if(count > 1)
    {
        word_detail_count--;
        MMK_SendMsg(MMI_WORD_DETAIL_WIN_ID, MSG_FULL_PAINT, PNULL);
        if(word_open_auto_play){
            WordDetail_PlayPinyinAudio();
        }
    }
    else
    {
        word_detail_count = 0;
        MMK_CloseWin(MMI_WORD_DETAIL_WIN_ID);
    }
}

PUBLIC void Word_SaveDeleteNewWord(uint16 book_id, uint16 chap_id, char * chap_name)
{
    char file_path[50] = {0};
    char * out;
    uint8 i = 0;
    cJSON * root;
    cJSON * id;
    cJSON * contentName;
    cJSON * baseUrl;
    cJSON * word;
    cJSON * word_item;
    cJSON * text;
    cJSON * audio;
    cJSON * phonetic;
    cJSON * translation;

    sprintf(file_path, WORD_BOOK_NEW_WORD_PATH, book_id, chap_id);
    if(word_detail_count < 1)
    {
        if(zmt_file_exist(file_path)){
            zmt_file_delete(file_path);
        }
        return;
    }
    root = cJSON_CreateObject();

    id = cJSON_CreateNumber(chap_id);
    cJSON_AddItemToObject(root, "id", id);

    contentName = cJSON_CreateString(chap_name);
    cJSON_AddItemToObject(root, "contentName", contentName);

    baseUrl = cJSON_CreateString(WORD_BOOK_AUDIO_BASE_URL);
    cJSON_AddItemToObject(root, "baseUrl", baseUrl);

    word = cJSON_CreateArray();
    for(i = 0;i < word_detail_count && i < WORD_CHAPTER_WORD_MAX;i++)
    {
        word_item = cJSON_CreateObject();

        text = cJSON_CreateString(new_word_detail_info[i]->word);
        cJSON_AddItemToObject(word_item, "text", text);

        audio = cJSON_CreateString(new_word_detail_info[i]->audio_uri);
        cJSON_AddItemToObject(word_item, "audio", audio);

        phonetic = cJSON_CreateString(new_word_detail_info[i]->phonetic);
        cJSON_AddItemToObject(word_item, "phonetic", phonetic);

        translation = cJSON_CreateString(new_word_detail_info[i]->translation);
        cJSON_AddItemToObject(word_item, "translation", translation);

        cJSON_AddItemToArray(word, word_item);
    }
    cJSON_AddItemToObject(root, "word", word);

    if(zmt_file_exist(file_path)){
        zmt_file_delete(file_path);
    }
    out = cJSON_PrintUnformatted(root);
    zmt_file_data_write(out, strlen(out), file_path);
    SCI_FREE(out);
    cJSON_Delete(root);
}

PUBLIC void Word_ParseMp3Response(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id)
{
    SCI_TRACE_LOW("%s: is_ok = %d, Rcv_len = %d", __FUNCTION__, is_ok, Rcv_len);
    if (is_ok && pRcv != PNULL && Rcv_len> 2)
    {
        if(is_open_new_word)
        {
            if(new_word_detail_info[word_detail_cur_idx] == NULL){
                return;
            }
            new_word_detail_info[word_detail_cur_idx]->audio_len = Rcv_len;
            if(new_word_detail_info[word_detail_cur_idx]->audio_data != NULL){
                SCI_FREE(new_word_detail_info[word_detail_cur_idx]->audio_data);
                new_word_detail_info[word_detail_cur_idx]->audio_data = NULL;
            }
            new_word_detail_info[word_detail_cur_idx]->audio_data = SCI_ALLOC_APPZ(Rcv_len);
            SCI_MEMSET(new_word_detail_info[word_detail_cur_idx]->audio_data, 0, Rcv_len);
            SCI_MEMCPY(new_word_detail_info[word_detail_cur_idx]->audio_data, pRcv, Rcv_len);
        }
        else
        {
            if(word_chapter_info[word_book_info.cur_chapter_idx] == NULL ||
                word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx] == NULL){
                return;
            }
            word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_len = Rcv_len;
            if(word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data != PNULL){
                SCI_FREE(word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data);
                word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data = NULL;
            }
            word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data = SCI_ALLOCA(Rcv_len);
            SCI_MEMSET(word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data, 0, Rcv_len);
            SCI_MEMCPY(word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_data, pRcv, Rcv_len);
        }
	
        if(zmt_tfcard_exist() && zmt_tfcard_get_free_kb() > 100 * 1024){
            char file_path[40] = {0};
            if(is_open_new_word){
                sprintf(file_path, WORD_BOOK_AUDIO_PATH, 
                    word_publish_info[word_book_info.cur_publish_idx]->item_info[word_book_info.cur_book_idx]->book_id, 
                    new_word_detail_info[word_detail_cur_idx]->word
                );
            }else{
                sprintf(file_path, WORD_BOOK_AUDIO_PATH, 
                    word_publish_info[word_book_info.cur_publish_idx]->item_info[word_book_info.cur_book_idx]->book_id, 
                    word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->word
                );
            }
            if(zmt_file_exist(file_path)){
                zmt_file_delete(file_path);
            }
            zmt_file_data_write(pRcv, Rcv_len, file_path);
        }

    }
    else
    {
        if(is_open_new_word){
            if(new_word_detail_info[word_detail_cur_idx] == NULL){
                return;
            }
            new_word_detail_info[word_detail_cur_idx]->audio_len = -2;
        }else{
            if(word_chapter_info[word_book_info.cur_chapter_idx] == NULL ||
                word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx] == NULL){
                return;
            }
            word_chapter_info[word_book_info.cur_chapter_idx]->detail[word_detail_cur_idx]->audio_len = -2;
        }
    }
    WordDetail_PlayPinyinAudio();
}


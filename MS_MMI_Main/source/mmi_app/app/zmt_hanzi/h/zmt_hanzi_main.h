#ifndef ZMT_HANZI_MAIN_H
#define ZMT_HANZI_MAIN_H

#include "sci_types.h"
#include "mmk_type.h"
//#include "dsl_homework_main.h"
/*----------------------------------------------------------------------------*/
/* Compiler Flag */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
extern "C"
 {
#endif
/*******************************************************************/
// Description : open hello sprd windows
// Global resource dependence : 
/******************************************************************/
#define HANZI_CARD_USE_OFF_DATA 0

#define HANZI_CARD_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define HANZI_CARD_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10

#define HANZI_BOOK_UPDATE_INTERVAL_TIMES 7*24*3600 //7ÃÏ

#define HANZI_BOOK_APP_ID "00000002"
#define HANZI_BOOK_APP_SECRET "ZmtHanziBookSec"

#define HANZI_BASE_DEVICE_IMEI "868750061006384"
#define HANZI_BOOK_HEADER_PATH "http://8.130.95.8:8866/"
#define HANZI_BOOK_AUDIO_BASE_PATH "http://8.130.95.8:8866/file/"
#define HANZI_BOOK_PUBLISH_PATH "hanzi/getPublisher?%s"
#define HANZI_BOOK_GRADE_PATH "hanzi/get?gradeId=%d&%s"
#define HANZI_BOOK_CHAPTER_PATH "hanzi/get?contentId=%d&%s"
#define HANZI_BOOK_AUDIO_PATH "%saudio/word/%s/name/%s.mp3"

#define HANZI_CARD_LEARN_INFO_PATH "E:/hanzi/learn_info.json"
#define HANZI_CARD_BOOK_INFO_PATH "E:/hanzi/publish_name.json"
#define HANZI_CARD_GRADE_CHAPTER_PATH "E:/hanzi/grade_%d/grade_%d.json"
#define HANZI_CARD_CHAPTER_WORD_PATH "E:/hanzi/grade_%d/%d.json"
#define HANZI_CARD_WORD_AUDIO_PATH "E:/hanzi/grade_%d/%s.mp3"
#define HANZI_CARD_NEW_HANZI_PATH "E:/hanzi/grade_%d/unmaster_word_%d.json"//grade_id,//section_id

#define HANZI_PUBLISH_BOOK_MAX 10
#define HANZI_CONTENT_CHAPTER_MAX 20
#define HANZI_CHAPTER_WORD_MAX 100

#define HANZI_LISTEN_SET_SYMBOL_NUM 3
#define HANZI_LISTEN_SET_INTERVAL_3 3000
#define HANZI_LISTEN_SET_INTERVAL_5 5000
#define HANZI_LISTEN_SET_INTERVAL_10 10000
#define HANZI_LISTEN_SET_REPEAT_1 1
#define HANZI_LISTEN_SET_REPEAT_3 3
#define HANZI_LISTEN_SET_REPEAT_5 5

typedef struct
{
    uint32 update_times;
    uint16 book_id;
    uint16 unit_id;
}HANZI_LEARN_INFO_T;

typedef struct
{
    uint8 id;
    char * book_name;
    char * publish_name;
}HANZI_PUBLISH_BOOK_INFO;

typedef struct HANZI_BOOK_INFO_T
{
    int cur_book_idx;
    int cur_chapter_idx;
    int cur_section_idx;
    int cur_section_children_idx;
}HANZI_BOOK_INFO_T;

typedef struct HANZI_CONTENT_CHAPTER_INFO_T
{
    uint16 chapter_id;
    char * chapter_name;
}HANZI_CONTENT_CHAPTER_INFO_T;

typedef struct HANZI_CONTENT_INFO_T
{
    char * content_name;
    uint16 content_id;
    HANZI_CONTENT_CHAPTER_INFO_T * chapter[HANZI_CONTENT_CHAPTER_MAX];
}HANZI_CONTENT_INFO_T;

typedef struct
{
    char * word;//∫∫◊÷
    char * pingy;//∆¥“Ù
    char * audio_uri;//∫∫◊÷“Ù∆µpath
    int32 audio_len;//“Ù∆µ≥§∂»//0:”–“Ù∆µurl,-1:Œﬁ“Ù∆µurl
    char * audio_data;
    char * similar_word;
    char * annotation;
    char * remark;//–ŒΩ¸◊÷+◊¢ Õ
}HANZI_BOOK_HANZI_INFO;

typedef enum
{
    HANZI_LISTEN_NONE = 0,
    HANZI_LISTEN_NOW,
    HANZI_LISTEN_PAUSE,
    HANZI_LISTEN_INFO,
}HANZI_LISTEN_STAUS_TYPE_E;

typedef struct
{
    HANZI_LISTEN_STAUS_TYPE_E status;
    uint8 style;
    uint8 repeat;
    uint16 interval;
    uint8 listen_idx;
}HANZI_LISTEN_INFO_T;

PUBLIC void MMI_CreateHanziTipsWin(void);
PUBLIC void MMI_CreateHanziWin(void);
PUBLIC void MMI_CreateHanziChapterWin(void);
PUBLIC void MMI_CreateHanziDetailWin(void);
PUBLIC void MMI_CreateHanziListenWin(void);
PUBLIC MMI_RESULT_E MMI_CloseHanziDetailWin(void);
PUBLIC void MMIZMT_CloseHanziPlayer(void);

PUBLIC void Hanzi_ReleaseBookInfo(void);
PUBLIC void Hanzi_ParseBookInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);
PUBLIC void Hanzi_requestBookInfo(void);

PUBLIC void Hanzi_ReleaseChapterInfo(void);
PUBLIC void Hanzi_ParseChapterInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);
PUBLIC void Hanzi_requestChapterInfo(uint8 grade_id);

PUBLIC void Hanzi_ReleaseDetailInfo(void);
PUBLIC void Hanzi_ParseDetailInfo(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);
PUBLIC void Hanzi_requestDetailInfo(uint8 grade_id, uint16 req_id);

PUBLIC void Hanzi_ParseMp3Response(BOOLEAN is_ok,uint8 * pRcv,uint32 Rcv_len,uint32 err_id);
PUBLIC void HanziDetail_PlayPinyinAudio(void);

PUBLIC void Hanzi_UpdateBookInfo(void);
PUBLIC void Hanzi_ReleaseLearnInfo(void);
PUBLIC void Hanzi_WriteLearnInfo(void);
PUBLIC void Hanzi_UpdateLearnInfo(uint16 book_id, uint16 unit_id);
PUBLIC BOOLEAN Hanzi_LoadLearnInfo(void);
#ifdef _cplusplus
}
#endif
#endif
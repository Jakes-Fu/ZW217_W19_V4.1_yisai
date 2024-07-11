/*
 * @Author: guojiaqi
 * @Date: 
 * @LastEditors: guojiaqi
 * @LastEditTime: 
 * @FilePath: dsl_poetry_main.h
 * @Description: 
 */

#ifndef _ZMT_POETRY_MAIN_H_
#define _ZMT_POETRY_MAIN_H_
#include "sci_types.h"
#include "mmk_type.h"
/*----------------------------------------------------------------------------*/
/* Compiler Flag */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
extern "C"
 {
#endif
#define BASE_POETRY_TEST_URL "http://open-api-test.readboy.com/shici/"
#define BASE_POETRY_URL "http://open.readboy.com/shici/"
#define BASE_DEVICE_IMEI "868750061006384"

#define POETRY_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define POETRY_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10

#define POETRY_GRADE_TOTAL_NUM 18
#define POETRY_GRADE_LIST_SHOW_ITEM 5
#define POETRY_ITEM_LIST_SHOW_ITEM 4

#define POETRY_PAGE_SIZE 14

#define OFFLINE_DATA  1
#define TEST_API  0

#define DEPOSIT_TEST_API 0

#define MAX_POETRY_LIST_SIZE 350
#define MAX_POETRY_SENTENCES_SIZE 30

#define POETRY_TIME_DIFF_FROM_1970_TO_1980     ((365*10+2)*24*3600)

typedef struct
{
    int grade_idx;
    int poetry_idx;
    int detail_idx;
}ZMT_POETRY_SHELECT_INFO;

typedef struct
{
	char name[100];
}ZMT_POETRY_GRADE_NAME;

typedef struct
{
    uint8 curPage;
    int16 total;
    uint8 curIndex;
    uint8 selectedIndex;
    GUI_RECT_T btn_rect_1;
    GUI_RECT_T btn_rect_2;
}ZMT_POETRY_PAGE_T;

typedef struct
{
    char id[15];
    char title[100];
    char author[10];
    char years[10];
} ZMT_POETRY_T;
typedef struct
{
    ZMT_POETRY_T poetry[MAX_POETRY_LIST_SIZE];
} ZMT_POETRY_LIST_T;

typedef struct
{
    char *audio_data;
    int audio_data_len;
}ZMT_POETRY_AUDIO_DATA_T;

typedef struct
{
    char audio[200];
}ZMT_POETRY_DETAIL_AUDIO_T;

typedef struct
{
    char * id;
    char * title;
    char * titleAudio;
    char * type;
    char * title_content;
    char * content;
    char * comment;
    char * translation;
    char * appreciation;
    uint8 sentences_num;
    ZMT_POETRY_DETAIL_AUDIO_T *sen_audio[MAX_POETRY_SENTENCES_SIZE];
} ZMT_POETRY_DETAIL_T;

typedef struct
{
    uint16 id;
    char title[100];
} ZMT_POETRY_GRADE_INFO_T;

typedef struct
{
    ZMT_POETRY_GRADE_INFO_T grade_info[30];
} ZMT_POETRY_GRADE_LIST_T;

typedef struct
{
    ZMT_POETRY_GRADE_LIST_T all_grade[18];
} ZMT_POETRY_ALL_GRADE_LIST_T;

typedef struct
{
	uint8 favorite[9000];
} ZMT_POETRY_TEMP_FAVORITE_STATUS_LIST_T;

PUBLIC BOOLEAN MMI_IsPoetryItemWinOpen(void);
PUBLIC BOOLEAN MMI_IsPoetryDetailWinOpen(void);

#ifdef _cplusplus
}
#endif
#endif
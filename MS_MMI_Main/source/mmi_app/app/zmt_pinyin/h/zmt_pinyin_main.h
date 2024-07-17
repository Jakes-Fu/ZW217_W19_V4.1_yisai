/*****************************************************************************
** File Name:      zmt_pinyin_main.h                                               *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#ifndef ZMT_PINYIN_MAIN_H
#define ZMT_PINYIN_MAIN_H

#include "sci_types.h"
#include "mmk_type.h"
#include "guistring.h"

#define PINYIN_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define PINYIN_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10

#define PINYIN_WIN_BG_COLOR GUI_RGB2RGB565(80, 162, 254)
#define PINYIN_TITLE_BG_COLOR GUI_RGB2RGB565(108, 181, 255)

#define PINYIN_ICON_LIST_ITEM_MAX 6

#define PINYIN_DAN_ITEM_MAX 6
#define PINYIN_FU_ITEM_MAX 7
#define PINYIN_QIAN_ITEM_MAX 5
#define PINYIN_HOU_ITEM_MAX 4
#define PINYIN_SHENG_ITEM_MAX 21
#define PINYIN_ZHENG_ITEM_MAX 16

#define PINYIN_AUDIO_ITEM_MAX PINYIN_SHENG_ITEM_MAX

#define PINYIN_APP_ID "00000004"
#define PINYIN_APP_SECRET "ZmtPinyinSec"
#define PINYIN_BASE_URL_HEADER_PATH "http://8.130.95.8:8866/"
#define PINYIN_AUDIO_BASE_URL_PATH "hanzi/getPinyinAudio?%s"
#define PINYIN_MP3_DATA_BASE_PATH "E:/zmt_pinyin/pinyin_%d/%s.mp3"

typedef struct
{
    char text[20];
}PINYIN_INFO_TEXT_T;

typedef struct
{
    uint8 num;
}PINYIN_INFO_NUM_T;

typedef struct
{
    uint8 cur_icon_idx;
    uint8 cur_read_idx;
    uint8 cur_read_py_idx;
    BOOLEAN is_play;
    BOOLEAN is_circulate;
    BOOLEAN is_single;
}PINYIN_READ_INFO_T;

typedef struct
{
    char * audio_uri;
    char * audio_data;
    int16 audio_len;
}PINYIN_AUDIO_INFO_T;

PUBLIC void MMI_CreatePinyinMainWin(void);

PUBLIC void Pinyin_PlayAudioMp3(uint8 idx, char * text);
PUBLIC void Pinyin_ReleaseAudioPath(void);
PUBLIC void Pinyin_RequestAudioPath(void);
PUBLIC void Pinyin_RequestAudioData(uint8 idx, char * url);


#endif
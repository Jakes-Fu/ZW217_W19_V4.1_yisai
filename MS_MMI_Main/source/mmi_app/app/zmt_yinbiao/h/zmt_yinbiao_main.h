/*****************************************************************************
** File Name:      zmt_yinbiao_main.h                                               *
** Author:         fys                                               *
** Date:           2024/07/08                                                *
******************************************************************************/
#ifndef ZMT_YINBIAO_MAIN_H
#define ZMT_YINBIAO_MAIN_H

#include "sci_types.h"
#include "mmk_type.h"
#include "guistring.h"

#define YINBIAO_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define YINBIAO_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10

#define YINBIAO_WIN_BG_COLOR GUI_RGB2RGB565(80, 162, 254)
#define YINBIAO_TITLE_BG_COLOR GUI_RGB2RGB565(108, 181, 255)

#define YINBIAO_ICON_LIST_ITEM_MAX 8

#define YINBIAO_DAN_ITEM_MAX 12
#define YINBIAO_FU_ITEM_MAX 8
#define YINBIAO_QIAN_ITEM_MAX 3
#define YINBIAO_HOU_ITEM_MAX 6
#define YINBIAO_SHENG_ITEM_MAX 6
#define YINBIAO_ZHENG_ITEM_MAX 10
#define YINBIAO_HALF_ITEM_MAX  2

#define YINBIAO_AUDIO_ITEM_MAX YINBIAO_DAN_ITEM_MAX

#define YINBIAO_APP_ID "00000005"
#define YINBIAO_APP_SECRET "ZmtYinbiaoSec"
#define YINBIAO_BASE_URL_HEADER_PATH "http://8.130.95.8:8866/"
#define YINBIAO_AUDIO_BASE_URL_PATH "english/getIPAAudio?%s"
#define YINBIAO_MP3_DATA_BASE_PATH "E:/zmt_yinbiao/yinbiao_%d/%s.mp3"

typedef struct
{
    uint8 text[10];
}YINBIAO_INFO_TEXT_T;

typedef struct
{
    uint8 num;
}YINBIAO_INFO_NUM_T;

typedef struct
{
    uint8 cur_icon_idx;
    uint8 cur_read_idx;
    uint8 cur_read_py_idx;
    BOOLEAN is_play;
    BOOLEAN is_circulate;
    BOOLEAN is_single;
}YINBIAO_READ_INFO_T;

typedef struct
{
    char * audio_uri;
    char * audio_data;
    int16 audio_len;
}YINBIAO_AUDIO_INFO_T;


PUBLIC void MMI_CreateYinbiaoMainWin(void);

PUBLIC void Yinbiao_ReleaseAudioPath(void);
PUBLIC void Yinbiao_RequestAudioPath(void);
PUBLIC void Yinbiao_RequestAudioData(uint8 idx, char * url);
PUBLIC void Yinbiao_PlayAudioMp3(uint8 idx, char * text);

#endif
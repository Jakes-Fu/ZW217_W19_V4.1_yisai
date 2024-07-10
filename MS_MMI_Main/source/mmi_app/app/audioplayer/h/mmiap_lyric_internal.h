/*****************************************************************************
** File Name:      mmiap_lyric_internal.h                                   *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine           Create
** 11/2009       xingdong.li        Modified
******************************************************************************/
#ifdef MMIAP_LYRIC_SUPPORT
#ifndef _MMIMP3_LYRIC_INTERNAL_H_
#define _MMIMP3_LYRIC_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_export.h"
#include "gui_ucs2b_converter.h"
#include "mmiap_position.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_LYRIC_SUFFIX             ".lrc"  //Mp3 lyric文件后缀名
#define MMIMP3_LYRIC_SUFFIX_LEN         6       //Mp3 lyric文件后缀名的长度
#define MMIMP3_SUFFIX_ACTUAL_LEN        4

#define MMIMP3_LYRIC_TAG_LEN            50          //lyric tag len
#define MMIMP3_LYRIC_TAG_NUM            1000        //lyric tag num
#define MMIMP3_LYRIC_SENTENCE_MAX_LEN   1000        //lyric sentence len
#define MMIMP3_LYRIC_SENTENCE_NUM       1000        //lyric sentence num
#define MMIMP3_LYRIC_FILE_MAX_SIZE      (100<<10)   //100K
#define MMIMP3_LYRIC_DELAY_TIME         50          //用于调整歌词同步
#define MMIMP3_LYRIC_SECTION_STC_NUM    100         //将歌词文件分段，每段100句
#define MMIMP3_LYRIC_TEMP_WCHAR_LEN     (1024*2)    //解析歌词文件时每次解析Unicode码长度，2k个
#define MMIMP3_LYRIC_TEMP_DATA_LEN      (1024)      //解析歌词文件时每次读取数据长度，1k

#define MMIMP3_LYRIC_FILE_HEAD_MAX_LEN      (3)     //歌词文件格式头长度，上限
#define MMIMP3_LYRIC_FILE_UNICODE_HEAD_LEN  (2)     //歌词文件格式头长度，usc16
#define MMIMP3_LYRIC_FILE_UTF8_HEAD_LEN     (3)     //歌词文件格式头长度，utf8
#define MMIMP3_LYRIC_FILE_ASCI_HEAD_LEN     (0)     //歌词文件格式头长度，asci

#define MMIMP3_LYRIC_ARTIST             "ar"
#define MMIMP3_LYRIC_NAME               "ti"
#define MMIMP3_LYRIC_TRACK              "al"
#define MMIMP3_LYRIC_AUTHER             "by"
#define MMIMP3_LYRIC_OFFSET             "offset"

#define LYRIC_LEFT_SQUARE_BRACKET      ('[')
#define LYRIC_RIGHT_SQUARE_BRACKET     (']')
#define LYRIC_COLON                    (':')
#define LYRIC_POINT                    ('.')
#define LYRIC_NUMBER_ZERO              ('0')
#define LYRIC_NUMBER_NINE              ('9')
#define LYRIC_SPACE                    (' ')
#define LYRIC_SENTENCE_END_CHAR_D      (0x0d)
#define LYRIC_SENTENCE_END_CHAR_A      (0x0a)

#define LYRIC_DECIMAL_BASE              10
#define LYRIC_TAG_MIN_LEN               1
#define LYRIC_TAG_DECIMAL_MAX_LEN       2

#define MMIMP3_CHINESE_CHAR_HEAD        0x80
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//lyric exist state type
typedef enum
{
    MMIMP3_LYRIC_NOT_EXIST,     //歌词文件不存在
    MMIMP3_LYRIC_EXIST,         //存在
    MMIMP3_LYRIC_PARSED,        //解析完毕
    MMIMP3_LYRIC_TOO_LARGE,     //超过上限MMIMP3_LYRIC_FILE_MAX_SIZE
    MMIMP3_LYRIC_FORMAT_ERROR,  //歌词格式错误
    MMIMP3_LYRIC_TYPE_MAX
} MMIMP3_LYRIC_TYPE_E;

//mp3 tag type
typedef enum
{
    MMIMP3_LYRIC_TYPE_NONE,
    MMIMP3_LYRIC_SONG_ARTIST,   //艺人名
    MMIMP3_LYRIC_SONG_NAME,     //曲名
    MMIMP3_LYRIC_TRACK_NAME,    //专辑名
    MMIMP3_LYRIC_AUTHER_NAME,   //LRC作者名
    MMIMP3_LYRIC_TIME_OFFSET,   //时间标签位移
    MMIMP3_LYRIC_TYPE_TIME,
    MMIMP3_LYRIC_MAX
} MMIMP3_LYRIC_TAG_TYPE_E;

//lyric parsed sentence info
typedef struct 
{
    uint32      offset;
    uint32      length;
} MMIMP3_LYRIC_PARSED_INFO_T;

//lyric time tag info
typedef struct 
{
    uint32   sentence_num; //此时间对应的句子
    int32    time;
} MMIMP3_LYRIC_TIME_TAG_INFO_T;

//lyric sentence structure
typedef struct 
{
//    BOOLEAN     is_ucs2;
    wchar       *lyric_content_ptr;
    uint16      length;
    uint32      sentence_id;
} MMIMP3_LYRIC_SENTENCE_T;

//lyric sentence info which has been read
typedef struct 
{
    uint32   sentence_num;      //内含的句子个数
    uint32   next_vacant_pos;   //循环保存时的当前位置
    MMIMP3_LYRIC_SENTENCE_T lyric_sentence[MMIMP3_LYRIC_DISPLAY_LINE_NUM];//句子内容
} MMIMP3_LYRIC_DISPLAY_SENTENCE_INFO_T;

//lyric file info
typedef struct
{
    uint32                          cur_wchar_pos;        //current position
    uint32                          file_len;             //lyric file len
    MMIFILE_HANDLE                  file_handle;          //歌词文件handle
    GUI_CHAR_CODE_TYPE_E            file_code_type;
    uint16                          file_head_len;
    BOOLEAN                         is_read_finished;
    BOOLEAN                         is_buf_parse_finished;
    BOOLEAN                         is_multi_tag_line;    //当前解析部分是否为多tag行
    uint8                           *temp_parse_buf_ptr;  //临时buffer，解析全文用(时间较紧，放弃stack方式)
    uint16                          *temp_unicode_buf_ptr;//临时buffer，解析全文用,unicode
    uint32                          temp_buf_unicode_num; //临时buffer的unicode个数
    uint32                          temp_buf_start_offset;//临时buffer的数据起始偏移(指有效数据绝对偏移)
    uint32                          temp_buf_end_offset;  //临时buffer的数据结束偏移(指有效数据绝对偏移)
    uint32                          time_tag_num;         //歌词时间标签总个数
    uint32                          sentence_num;         //歌词句子总个数
    uint32                          file_read_offset;       //歌词文件解析的偏移
    MMIMP3_LYRIC_TIME_TAG_INFO_T    *time_tag_ptr;        //时间标签
    MMIMP3_LYRIC_PARSED_INFO_T      *parsed_info_ptr[MMIMP3_LYRIC_SENTENCE_NUM/MMIMP3_LYRIC_SECTION_STC_NUM]; //句子解析详情
} MMIMP3_LYRIC_FILE_INFO_T;

//lyric display info
typedef struct
{
    MMIMP3_LYRIC_TYPE_E     lyric_type;                 //歌词类型
    uint8                   lyric_disp_timer_id;        //歌词播放timer
    uint32                  lyric_cur_time_tag_index;   //歌词播放时间标签序号
    uint32                  lyric_display_start_tag;    //歌词显示的起始句
    uint32                  lyric_display_end_tag;      //歌词显示的结束句
    int32                   lyric_time_offset;          //歌词显示延时
    MMIMP3_LYRIC_DISPLAY_SENTENCE_INFO_T lyric_display_sentence;  //句子内容
} MMIMP3_LYRIC_DISPLAY_INFO_T;


typedef struct
{
    BOOLEAN is_update_backlight; //是否更新背光
    BOOLEAN is_light_on; //是否背光常亮
    BOOLEAN is_update_lyric; //是否更新歌词
    BOOLEAN is_update_lyric_now; //是否立刻更新歌词    
}MMIAP_LYRIC_WIN_CALLBACK_PARAM_T;

typedef struct
{
    MMI_IMAGE_ID_T bg_img_id;//background img id
    GUI_LCD_DEV_INFO lcd_dev_info;//bg lcd dev info
    GUI_RECT_T bg_rect;//bg rect
    GUI_RECT_T text_rect; //lyric display rect
    GUI_FONT_T  font_type; //font type
    uint8 line_space; //line space
    uint8 char_space;
    uint8 margin_v;//text margin v
    uint8 margin_h;// text margin h
    uint8 max_alpha;// text
    uint8 min_alpha;     
    GUI_COLOR_T focus_text_color;
    uint32 max_line_num; //    
}MMIAP_DISPLAY_LYRIC_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : play mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_OpenLyric(
                    void
                    );

/*****************************************************************************/
//  Description : stop lyric
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_StopLyric(void);

/*****************************************************************************/
//  Description : display lyric immediately
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DisplayLyricImmediately(
                    void
                    );

/*****************************************************************************/
//  Description : pause lyric
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_PauseLyric(void);

/*****************************************************************************/
//  Description : resume lyric
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_ResumeLyric(void);

/*****************************************************************************/
//  Description : seek lyric by time offset
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMP3_SeekLyric(
                uint32     mp3_offset,
                BOOLEAN    is_update
                );

/*****************************************************************************/
//  Description : Handle lyric display style with light always on or not.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_HandleLyricBackLight(
                      BOOLEAN  need_light_always_on
                      );

/*****************************************************************************/
//  Description : Get whether lyric file exist.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsLyricExist(
                      void
                      );

/*****************************************************************************/
//  Description : Set the flag lyric file exist.
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetLyricExistState(
                      MMIMP3_LYRIC_TYPE_E lyric_type
                      );

/*****************************************************************************/
//  Description : init lyric info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitLyricInfo(
    void
);

/*****************************************************************************/
//  Description : destroy lyric ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyLyricInfo(
    void
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif//MMIAP_LYRIC_SUPPORT

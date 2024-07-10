/*****************************************************************************
** File Name:               mmiap_common.h                                   *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap common data              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAP_COMMON_H_
#define _MMIAP_COMMON_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvaud_api.h"
#include "mmisrvfmm_export.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#ifdef WIN32
    typedef __int64    uint64_t;
#else
    typedef unsigned long long uint64_t;
#endif

#define MMIMP3_DISPLAY_TIME_LEN     15
#define MMIMP3_ZERO_OFFSET          0                         //zero play offset


#define MMIMP3_LIST_GUI_LIMIT   2000

#define MMIMP3_SUFFIX_MP3           ".mp3"  //Mp3文件后缀名

//@CR241798 2011.05.30
#define MMIMP3_SUFFIX_MP2           ".mp2"  //Mp2文件后缀名
#define MMIMP3_SUFFIX_MP1           ".mp1"  //Mp1文件后缀名
//@CR241798 2011.05.30

#define MMIMP3_SUFFIX_WMA           ".wma"  //WMA文件后缀名
#define MMIMP3_SUFFIX_AAC           ".aac"  //AAC文件后缀名
#define MMIMP3_SUFFIX_M4A           ".m4a"  //M4A文件后缀名
#define MMIMP3_SUFFIX_AMR           ".amr"  //AMR文件后缀名
#define MMIMP3_SUFFIX_MID           ".mid"  //MID文件后缀名
#define MMIMP3_SUFFIX_MIDI          ".midi" //MIDI文件后缀名
#define MMIMP3_SUFFIX_WAV           ".wav"  //WAV文件后缀名
#ifdef DRM_SUPPORT
#define MMIMP3_SUFFIX_DRM           ".dm"   //DRM文件后缀名
#define MMIMP3_SUFFIX_DCF           ".dcf"  //DCF文件后缀名

#define MMIMP3_SUFFIX_DRM_NO_DOT    "dm"   //DRM文件后缀名
#define MMIMP3_SUFFIX_DCF_NO_DOT    "dcf"  //DCF文件后缀名
#endif

#define MMIMP3_DEFAULT_SUFFIX_LEN   6       //Mp3文件后缀名的长度


//列表文件后缀名
#define MMIMP3_PLAY_LIST_SUFFIX     ".lst"
#define MMIMP3_PLAY_LIST_SUFFIX_UCS  (g_mmimp3_play_list_suffix_wstr)//"\x00\x2E\x00\x6C\x00\x73\x00\x74\x00\x00"
#define MMIMP3_PLAY_LIST_SUFFIX_UCS_LEN     4


//书签信息文件名称 "mp3.dat"
#define MMIMP3_MUSIC_MARK_FILE_NAME  (g_mmimp3_mark_file_name_wstr) //"\x00\x6D\x00\x70\x00\x33\x00\x2E\x00\x64\x00\x61\x00\x74\x00\x00"
#define MMIMP3_MUSIC_MARK_FILE_NAME_LEN     7


#define MMIMP3_MY_MUSIC_MENU_INDEX      0
#define MMIMP3_PLAY_LIST_1_INDEX        1
#define MMIMP3_PLAY_LIST_2_INDEX        2
#define MMIMP3_PLAY_LIST_3_INDEX        3
#define MMIMP3_RESERVE_LIST_NUM         3
#define MMIMP3_SPECIAL_LIST_NUM         4
#define MMIMP3_PLAY_LIST_FMM_ARRAY      0xFFFF
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 

//Mp3 state
typedef enum
{
#ifdef MP3_BACKGROUND_SUPPORT
    MMIMP3_MP3,      //Mp3文件后缀名
    //@CR241798 2011.05.30
    MMIMP3_MP2,      //Mp3文件后缀名
    MMIMP3_MP1,      //Mp3文件后缀名
    //@CR241798 2011.05.30
#endif    
#ifdef WMA_BACKGROUND_SUPPORT
    MMIMP3_WMA,        //WMA文件后缀名
#endif
#ifdef AAC_BACKGROUND_SUPPORT
    MMIMP3_AAC,         //AAC文件后缀名
    MMIMP3_M4A,         //M4A文件后缀名
#endif    
#ifdef AMR_BACKGROUND_SUPPORT
    MMIMP3_AMR,          //AMR文件后缀名
#endif
#if defined MIDI_BACKGROUND_SUPPORT && !defined MMI_SOUNDBANK_MODE_COMPACT
    MMIMP3_MID,          //MID文件后缀名
    MMIMP3_MIDI,         //MIDI文件后缀名
#endif
#ifdef WAV_BACKGROUND_SUPPORT
    MMIMP3_WAV,          //WAV文件后缀名
#endif
#ifdef DRM_SUPPORT
    MMIMP3_DRM,           //DRM文件后缀名
    MMIMP3_DCF,           //DCF文件后缀名
#endif
    MMIMP3_TYPE_MAX
}MMIMP3_TYPE_E;

//copy list data
typedef struct
{
    wchar                       file_name[FMM_SEARCH_FILENAME_MAXLEN+1];
    uint16                      file_name_len;
} MMIMP3_FILE_INFO_T;

//文件body
typedef struct
{
//    SCI_DATE_T  modify_date;    //修改日期
//    SCI_TIME_T  modify_time;    //修改时间
    uint32      time;   //修改时间
    uint32      size;   //文件大小
    uint16              full_path_len;
    wchar               full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1];   //MMIFILE_FULL_PATH_MAX_LEN
} MMIMP3_LIST_FILE_BODY_T;

typedef enum
{
    MP3_START_DEL_SIG,
    MP3_STOP_DEL_SIG,
    MP3_DEL_SIG_MAX
}MP3_DEL_SIG_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}MP3_DEL_SIG_T;


extern const wchar g_mmimp3_play_list_suffix_wstr[];
extern const wchar g_mmimp3_all_list_name_wstr[];
extern const wchar g_mmimp3_mark_file_name_wstr[];

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Get the audio folder.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetDefaultAudioFolder(
                    wchar *folder_name_ptr,
                    uint16 *folder_name_len_ptr
                    );
                    
/*****************************************************************************/
//  Description : Get the music filter.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetMusicFilter(
    MMIFMM_FILTER_T * filter_ptr
);

/*****************************************************************************/
//  Description : Initialize the mp3 file loading filter.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InitFileFilter(
    void
);

/*****************************************************************************/
//  Description : To judge whether the file is the type which is loaded by mp3 player.
//  Global resource dependence :
//  Author: xingdong.li
//  Note: 依据后缀判断
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsPlayerLoadType(
    MMISRVAUD_RING_FMT_E audio_type
);

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetSysFileFullPathForRead(
    MMIFILE_DEVICE_E default_dev,          //[IN]
    const wchar    *path_name_ptr,         //[IN]
    uint16         path_name_len,          //[IN]
    const wchar    *file_name_ptr,         //[IN]
    uint16         file_name_len,          //[IN]
    wchar          *full_path_name_ptr,    //[OUT]
    uint16         *full_path_len_ptr      //[OUT]
);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


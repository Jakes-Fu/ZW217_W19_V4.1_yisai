/*****************************************************************************
** File Name:        audioplayer_demoapp_play_win_data.c                         *
** Author:           zirui.li                                                *
** Date:             12/13/2021                                              *
** Copyright:                                                                *
** Description: This file is used to define audioplayer test data function   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 12/13/2021           zirui.li              Create                         *
******************************************************************************/

#include "audioplayer_demoapp_play_win_data.h"
#include "os_api.h"

#define AUDIO_SOURCE_TAG_LEN    32  //artist,album描述一般长度，与AUDIO_ID3_TAG_T对应

typedef struct
{
    wchar artist[AUDIO_SOURCE_TAG_LEN + 1];   //歌手
    wchar title[AUDIO_SOURCE_TAG_LEN + 1];    //歌名
    uint32 cur_time;   //当前播放时间(单位:秒)
    uint32 total_time;  //总时长(单位:秒)
}AUDIOPLAYER_DATA_PLAY_INFO_T;

LOCAL   AP_STATUS_E     s_ap_status = AP_STATUS_STOP;
LOCAL   AUDIOPLAYER_DATA_PLAY_INFO_T  s_ap_data_play_info = {0};

/*****************************************************************************/
//  Description: Audioplayer test get audioplayer status
//  Parameter: [In] None
//             [Out] None
//             [Return] AP_STATUS_E  //audioplayer status
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC AP_STATUS_E Audioplayer_Data_Get_Apstatus(void)
{
    return s_ap_status;
}

/*****************************************************************************/
//  Description: Audioplayer test set audioplayer status
//  Parameter: [In] status   //audioplayer status
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Apstatus(AP_STATUS_E status)
{
    SCI_TRACE_LOW("[Audioplayer Test]set audioplayer status:%d", status);
    s_ap_status = status;
}

/*****************************************************************************/
//  Description: Audioplayer test set artist
//  Parameter: [In] p_wstr   //artist str
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Artist(wchar* p_wstr)
{
    SCI_TRACE_LOW("[Audioplayer Test]set artist:%ls", p_wstr);
    SCI_MEMSET(s_ap_data_play_info.artist, 0, AUDIO_SOURCE_TAG_LEN*sizeof(wchar));
    SCI_MEMCPY(s_ap_data_play_info.artist, p_wstr, MIN(AUDIO_SOURCE_TAG_LEN, MMIAPICOM_Wstrlen(p_wstr))*sizeof(wchar));
}

/*****************************************************************************/
//  Description: Audioplayer test get artist
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_STRING_T
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T Audioplayer_Data_Get_Artist(void)
{
    MMI_STRING_T  artist_str = {0};
    artist_str.wstr_ptr = s_ap_data_play_info.artist;
    artist_str.wstr_len = MMIAPICOM_Wstrlen(artist_str.wstr_ptr);
    return artist_str;
}

/*****************************************************************************/
//  Description: Audioplayer test set title
//  Parameter: [In] p_wstr   //title str
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Title(wchar* p_wstr)
{
    SCI_TRACE_LOW("[Audioplayer Test]set title:%ls", p_wstr);
    SCI_MEMSET(s_ap_data_play_info.title, 0, AUDIO_SOURCE_TAG_LEN*sizeof(wchar));
    SCI_MEMCPY(s_ap_data_play_info.title, p_wstr, MIN(AUDIO_SOURCE_TAG_LEN, MMIAPICOM_Wstrlen(p_wstr))*sizeof(wchar));
}

/*****************************************************************************/
//  Description: Audioplayer test get title
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_STRING_T
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T Audioplayer_Data_Get_Title(void)
{
    MMI_STRING_T  title_str = {0};
    title_str.wstr_ptr = s_ap_data_play_info.title;
    title_str.wstr_len = MMIAPICOM_Wstrlen(title_str.wstr_ptr);
    return title_str;
}

/*****************************************************************************/
//  Description: Audioplayer test set play progress
//  Parameter: [In] cur_time   //当前播放进度时间
//             [In] total_time   //总时间
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Play_Progress(uint32 cur_time, uint32 total_time)
{
    s_ap_data_play_info.cur_time = cur_time;
    s_ap_data_play_info.total_time = total_time;
}

/*****************************************************************************/
//  Description: Audioplayer test get play progress
//  Parameter: [In] None
//             [Out] None
//             [Return] uint32   //percent
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 Audioplayer_Data_Get_Play_Progress(void)
{
    uint32 percent = 0;
    if (0 == s_ap_data_play_info.total_time)
    {
        SCI_TRACE_LOW("[Audioplayer Test]total time is 0!");
        return 0;
    }
    percent = (s_ap_data_play_info.cur_time)*100/(s_ap_data_play_info.total_time);
    return percent;
}

/*****************************************************************************/
//  Description: Audioplayer test reset info
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Reset_Info(void)
{
    SCI_TRACE_LOW("[Audioplayer Test]reset data info");
    SCI_MEMSET(&s_ap_data_play_info, 0, sizeof(AUDIOPLAYER_DATA_PLAY_INFO_T));
}

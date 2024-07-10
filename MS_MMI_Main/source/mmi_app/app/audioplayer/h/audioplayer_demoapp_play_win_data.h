/*****************************************************************************
** File Name:        audioplayer_demoapp_play_win_data.h                         *
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

#ifndef _AUDIOPLAYER_DEMOAPP_PLAY_WIN_DATA_H_
#define _AUDIOPLAYER_DEMOAPP_PLAY_WIN_DATA_H_
#include "sci_types.h"
#include "mmk_type.h"
typedef enum
{
    AP_STATUS_STOP,
    AP_STATUS_PAUSE,
    AP_STATUS_PLAYING,
    AP_STATUS_MAX
}AP_STATUS_E;


/*****************************************************************************/
//  Description: Audioplayer test get audioplayer status
//  Parameter: [In] None
//             [Out] None
//             [Return] AP_STATUS_E  //audioplayer status
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC AP_STATUS_E Audioplayer_Data_Get_Apstatus(void);

/*****************************************************************************/
//  Description: Audioplayer test set audioplayer status
//  Parameter: [In] status   //audioplayer status
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Apstatus(AP_STATUS_E status);

/*****************************************************************************/
//  Description: Audioplayer test set artist
//  Parameter: [In] p_wstr   //artist str
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Artist(wchar* p_wstr);

/*****************************************************************************/
//  Description: Audioplayer test get artist
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_STRING_T
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T Audioplayer_Data_Get_Artist(void);

/*****************************************************************************/
//  Description: Audioplayer test set title
//  Parameter: [In] p_wstr   //title str
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Title(wchar* p_wstr);

/*****************************************************************************/
//  Description: Audioplayer test get title
//  Parameter: [In] None
//             [Out] None
//             [Return] MMI_STRING_T
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T Audioplayer_Data_Get_Title(void);

/*****************************************************************************/
//  Description: Audioplayer test set play progress
//  Parameter: [In] cur_time   //当前播放进度时间
//             [In] total_time   //总时间
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Set_Play_Progress(uint32 cur_time, uint32 total_time);

/*****************************************************************************/
//  Description: Audioplayer test get play progress
//  Parameter: [In] None
//             [Out] None
//             [Return] uint32   //percent
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 Audioplayer_Data_Get_Play_Progress(void);

/*****************************************************************************/
//  Description: Audioplayer test reset info
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: zirui.li
//  Note:
/*****************************************************************************/
PUBLIC void Audioplayer_Data_Reset_Info(void);
#endif

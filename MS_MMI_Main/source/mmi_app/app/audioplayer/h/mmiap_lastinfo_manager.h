/*****************************************************************************
** File Name:      mmiap_lastinfo_manager.h                                  *
** Author:                                                                   *
** Date:           5/29/2012                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mp3 history list                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/5/29      robert.wang       Create                                   *
******************************************************************************/

#ifdef MMIAP_F_HISTORY_PLAYLIST
#ifndef _MMIMP3_HISTORYLIST_H_
#define _MMIMP3_HISTORYLIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmimp3_export.h"
#include "mmiap_play.h"
#include "mmiap_list_control.h"


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
#define MMIAP_DEFAULT_DIR              MMIMULTIM_DIR_MUSIC    //默认文件夹
#define MMIAP_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC)  //默认文件夹名称长度


#define MMIAP_HISTORY_FILENAME         L"mp3_history.dat"   //保存历史记录的默认文件名
#define MMIAP_HISTORY_FILENAME_LEN     21                         //保存历史记录的默认文件名长度
#define MMIAP_HISTORY_VER             "MP3HISTORY.12.00.01"        //默认文件版本号
#define MMIAP_HISTORY_VER_LEN          20                         //默认文件的版本号长度


#define MMIAP_HIS_FILEARRAY_FILENAME         L"mp3_filearray.dat"   //保存FILE ARRAY的默认文件名
#define MMIAP_HIS_FILEARRAY_FILENAME_LEN     17                     //保存FILE ARRAY的默认文件名长度
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   

typedef struct
{
    MMIFILE_DEVICE_E list_dev;//device 
}MMIAP_HISTORY_NV_INFO_T;

//历史记录列表文件的头信息
typedef struct
{
    uint8 version[MMIAP_HISTORY_VER_LEN+1]; //历史记录文件版本号    
}MMIAP_HISTORY_HEAD_T;

typedef struct
{
    uint32 file_size;
    SFS_DATE_T modify_date;
    SFS_TIME_T modify_time;
}MMIAP_HISTORY_LIST_FILE_INFO_T;

typedef struct
{
    BOOLEAN is_review_button_focus;
}MMIAP_HISTORY_PANEL_INFO_T;


//历史记录列表信息
typedef struct
{
    MMIMP3_PLAY_LIST_INFO_T cur_list_info;
    MMIMP3_PLAYING_DYN_INFO_T dyn_playing_info;
    MMIMP3_REVIEW_INFO_T review_info;
    MMIAP_HISTORY_PANEL_INFO_T panel_info;
    uint32 active_index;
    MMIMP3_OLD_INFO_T old_info;
}MMIAP_HISTORY_CONTEXT_T;

typedef struct
{
    MMIAP_HISTORY_CONTEXT_T context;
}MMIAP_HISTORY_IN_PARAM_INFO_T;

typedef struct
{
    MMIAP_HISTORY_CONTEXT_T context;
}MMIAP_HISTORY_OUT_PARAM_INFO_T;


typedef struct
{
    MMIAP_HISTORY_HEAD_T header;
    MMIAP_HISTORY_LIST_FILE_INFO_T history_list_file_info;    
    MMIAP_HISTORY_CONTEXT_T context;
}MMIAP_HISTORY_INFO_T;

/*****************************************************************************/
//  Description : 保存历史记录列表信息到默认文件中
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAP_SaveToHistoryFile(
                                             MMIAP_HISTORY_IN_PARAM_INFO_T *param_ptr //in
                                            );

/*****************************************************************************/
//  Description : 读取历史记录信息
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_ReadFromHistoryFile(
                                                    MMIAP_HISTORY_OUT_PARAM_INFO_T *param_ptr //out
                                                    );
                                                      
/*****************************************************************************/
//  Description : set history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAP_SetHistoryInitFlag(BOOLEAN is_init);

/*****************************************************************************/
//  Description : get history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsHistoryInit(void);

/*****************************************************************************/
//  Description : save mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SavePlayListInfo(void);

/*****************************************************************************/
//  Description : get history mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetHistoryPlayListInfo(void);


/*****************************************************************************/
//  Description : save mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetHistoryPanelInfo(MMIAP_HISTORY_PANEL_INFO_T *info_ptr //out
                                                 );

/*****************************************************************************/
//  Description : reset nv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResetLastInfoMgrNV(
    MMIAP_HISTORY_NV_INFO_T *info_ptr //out
);
                                                 
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif

#endif //#ifdef MMIAP_F_HISTORY_PLAYLIST


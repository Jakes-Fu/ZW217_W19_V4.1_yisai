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
#define MMIAP_DEFAULT_DIR              MMIMULTIM_DIR_MUSIC    //Ĭ���ļ���
#define MMIAP_DEFAULT_DIR_LEN          MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC)  //Ĭ���ļ������Ƴ���


#define MMIAP_HISTORY_FILENAME         L"mp3_history.dat"   //������ʷ��¼��Ĭ���ļ���
#define MMIAP_HISTORY_FILENAME_LEN     21                         //������ʷ��¼��Ĭ���ļ�������
#define MMIAP_HISTORY_VER             "MP3HISTORY.12.00.01"        //Ĭ���ļ��汾��
#define MMIAP_HISTORY_VER_LEN          20                         //Ĭ���ļ��İ汾�ų���


#define MMIAP_HIS_FILEARRAY_FILENAME         L"mp3_filearray.dat"   //����FILE ARRAY��Ĭ���ļ���
#define MMIAP_HIS_FILEARRAY_FILENAME_LEN     17                     //����FILE ARRAY��Ĭ���ļ�������
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   

typedef struct
{
    MMIFILE_DEVICE_E list_dev;//device 
}MMIAP_HISTORY_NV_INFO_T;

//��ʷ��¼�б��ļ���ͷ��Ϣ
typedef struct
{
    uint8 version[MMIAP_HISTORY_VER_LEN+1]; //��ʷ��¼�ļ��汾��    
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


//��ʷ��¼�б���Ϣ
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
//  Description : ������ʷ��¼�б���Ϣ��Ĭ���ļ���
//  Global resource dependence :  none
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAP_SaveToHistoryFile(
                                             MMIAP_HISTORY_IN_PARAM_INFO_T *param_ptr //in
                                            );

/*****************************************************************************/
//  Description : ��ȡ��ʷ��¼��Ϣ
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


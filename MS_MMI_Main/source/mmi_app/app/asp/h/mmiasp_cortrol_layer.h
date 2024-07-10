/****************************************************************************
** File Name:      mmiasp_cortrol_layer.h                                   *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the ANSWER PHONE           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/
#ifndef _MMIASP_CORTROL_LAYER_H
#define _MMIASP_CORTROL_LAYER_H

#include "window_parse.h"
#include "sci_types.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_nv.h"

#include "mmipb_export.h"
#include "mmiasp_id.h"
#include "mmiasp_image.h"
#include "mmiasp_text.h"
#include "mmiasp_menutable.h"
#include "mmifmm_export.h"
#include "cafapplet_gen.h"

#include "mmisrvaud_api.h"

typedef BOOLEAN   (* PRINGSELECT_CALLBACKFUNC)(BOOLEAN is_fixed_id, uint32  fixed_id,  wchar   *asp_file_name);

/* ����������� */
#define MMIASP_MAX_MSG_NUM       200//10000000


/* �ؼ�λ�ú궨�� */
#define MMIASP_CALL_NAME_POS_LEFT         10
#define MMIASP_CALL_NAME_POS_TOP          10
#define MMIASP_CALL_NAME_POS_RIGHT        10
#define MMIASP_CALL_NAME_POS_BOTTOM       10
#define MMIASP_CALL_NUM_POS_LEFT          10
#define MMIASP_CALL_NUM_POS_TOP           10
#define MMIASP_CALL_NUM_POS_RIGHT         10
#define MMIASP_CALL_NUM_POS_BOTTOM        10
#define MMIASP_CALL_DURATION_POS_LEFT     10
#define MMIASP_CALL_DURATION_POS_TOP      10
#define MMIASP_CALL_DURATION_POS_RIGHT    10
#define MMIASP_CALL_DURATION_POS_BOTTOM   10
#define MMIASP_PLAY_TIME_POS_LEFT         10
#define MMIASP_PLAY_TIME_POS_TOP          10
#define MMIASP_PLAY_TIME_POS_RIGHT        10
#define MMIASP_PLAY_TIME_POS_BOTTOM       10

/**/
#define MMIASP_WAIT_ASP_TIME_ITEM_MAX     5

/**/
#define MMIASP_MAX_TIME_LENS              16

#define MMIASP_SYSTEM_FILE_NEED_SPACE       2000  //ϵͳ �ļ���Ҫ����С�ռ�
#define MMIASP_RECORD_FILE_NEED_SPACE       20000 //¼�� �ļ���Ҫ����С�ռ�
/**/
#define MMIASP_SEND_MMS_MAX_SIZE     (1024*97)

#define ASP_SWITCH_FLAG_TYPE_NUM    2
#define ASP_AUDIO_TYPE_NUM          1
#define ASP_AUDIO_TIME_TYPE_NUM     4
#define ASP_WAIT_TIME_TYPE_NUM      5
#define ASP_SPEAKER_FLAG_TYPE_NUM   2
#define ASP_SAVE_PATH_TYPE_NUM      MMI_DEVICE_NUM

// �����¼����¼ʱ�öԷ������������Ľ�����Ҫ�õ�һЩ����....
#define ASP_DEFAULT_MAGIC_GAIN     0x100  // ħ����ֵ....
#define ASP_DEFAULT_BKMUSIC_GAIN   0x3    // ��������ֵ....
#define ASP_DEFAULT_UPLINK_GAIN    0x16a7 // ��bkmusic_as.c��DEFAULT_UPLINK_GAIN��
#define ASP_DEFAULT_DOWNLINK_GAIN  0x5a7  // ��bkmusic_as.c��DEFAULT_DOWNLINK_GAIN��

typedef enum
{ 
    MMIASP_SYSTEM_FILE,
    MMIASP_RECORD_FILE,
    MMIASP_MAX
}MMIASP_SAVE_TYPE_E;

typedef enum
{ 
    MMIASP_ERROR_NONE,
    MMIASP_ERROR_NO_FREESPACE,       // �洢�ռ䲻��
    MMIASP_ERROR_EXCEED_ITEMS_COUNT, // ������Ŀ������
    MMIASP_ERROR_MAX
}MMIASP_ERROR_CODE_E;

typedef enum
{
    MMIASP_SETTINGS_OFF,
    MMIASP_SETTINGS_ON,
    MMIASP_SETTINGS_ONOFF
}MMIASP_SETTINGS_ONOFF_E;

typedef enum
{
    MMIASP_RECORD_TIME_1M,
    MMIASP_RECORD_TIME_3M,
    MMIASP_RECORD_TIME_5M,
    MMIASP_RECORD_TIME_10M,
    MMIASP_RECORD_TIME_MAX
}MMIASP_RECORD_TIME_E;

typedef enum
{
    MMIASP_WAIT_TIME_1S,
    MMIASP_WAIT_TIME_5S,
    MMIASP_WAIT_TIME_10S,
    MMIASP_WAIT_TIME_15S,
    MMIASP_WAIT_TIME_20S,
    MMIASP_WAIT_TIME_MAX
}MMIASP_WAIT_TIME_E;

typedef enum
{
    MMIASP_SETTINGS_PLAY_RECORD_TIME_1S,
    MMIASP_SETTINGS_PLAY_RECORD_TIME_5S,
    MMIASP_SETTINGS_PLAY_RECORD_TIME_10S,
    MMIASP_SETTINGS_PLAY_RECORD_TIME_15S,
    MMIASP_SETTINGS_PLAY_RECORD_TIME_20S,
    MMIASP_SETTINGS_PLAY_RECORD_TIME_MAX
}MMIASP_SETTINGS_PLAY_RECORD_TIME_E;


typedef enum
{
    MMIASP_SETTING_FIX_AUDIO,
    MMIASP_SETTING_MORE_AUDIO,
    MMIASP_SETTING_AUDIO_MAX,
}MMIASP_SETTINGS_AUDIO_E;


typedef enum
{
    MMIASP_MACHINE_CALL_NONE,          //  NONE state
    MMIASP_MACHINE_CALL_PLAYAUDIO,      // �������ֽ׶�
    MMIASP_MACHINE_CALL_RECORDING,      // ¼�Է������Ľ׶�....
    MMIASP_MACHINE_CALL_SPEAKING,       // ͨ��״̬ �û���������....
    MMIASP_RECORD_STATE_MAX             
}MMIASP_MACHINE_CALL_STATE_E;           // ����¼����ͨ����״̬

typedef enum
{
    MMIASP_STATE_NONE = 0,
  
    MMIASP_STATE_OPENED,                 //��
    MMIASP_STATE_PLAY,                   //����
    MMIASP_STATE_PAUSE,                  //��ͣ
    MMIASP_STATE_STOP,                   //ֹͣ   
    MMIASP_STATE_MAX
}MMIASP_PLAY_STATE_E;                  //����״̬

typedef enum
{   
    MMIASP_RECORD_BUTTON_PLAY_PAUSE,//����ֹͣ¼��
    MMIASP_RECORD_BUTTON_STOP,      //ֹͣ���Ż���¼��
    MMIASP_RECORD_BUTTON_RECORD_PAUSE,  //¼��    
    MMIASP_RECORD_BUTTON_LIST,      //�б�¼���ļ�
    MMIASP_RECORD_BUTTON_INC_VOLUME,        //����
    MMIASP_RECORD_BUTTON_DEC_VOLUME,
    MMIASP_RECORD_BUTTON_MAX
}MMIASP_RECORD_BUTTON_E;

typedef enum
{
    MMIASP_JUST_START_APPLET,
    MMIASP_AUTO_PLAY_MESSEAGES,
    MMIASP_START_DEFAULT
    
}MMIASP_START_APPLET_E;

typedef struct _MMIASP_DATE_INFO_T
{
    
    SCI_DATE_T sys_date ;
    SCI_TIME_T sys_time ;

}MMIASP_DATE_INFO_T;

typedef struct  _MMIASP_MSG_INFO_T
{
    uint32      id;                                      //�̶���¼���id
    uint32      suffix_id;    
    BOOLEAN     read_flag;
    BOOLEAN     mark_flag;    
    BOOLEAN     set_ring_flag;
        
    wchar*      record_path;                             //���Դ洢·��

    uint8       call_num[MMIPB_NUMBER_MAX_STRING_LEN+1]; //�绰����
    wchar       time[MMIASP_MAX_TIME_LENS+1];            //��ʼ¼����ʱ��
    MMIASP_DATE_INFO_T  date_info;

    struct      _MMIASP_MSG_INFO_T *next_ptr;
} MMIASP_MSG_INFO_T;

typedef struct  _MMIASP_NV_SETTING_T
{

    BOOLEAN                 asp_flag[MMI_DUAL_SYS_MAX];
    uint16                  fixed_ring_id;          //�̶�����ID
    MMIASP_WAIT_TIME_E      wait_time;              //�绰��֮ͨǰ �ĵȴ�ʱ��   
    uint32                  play_record_time;       //�����ͨ�� ���Է�����¼���ĳ�ʱʱ��
    MMIASP_RECORD_TIME_E    record_time;            //�����ͨ�� ¼���ĳ�ʱʱ��
   
        
    BOOLEAN                 speaker_flag;
    wchar                   record_path_for_client[MMIFILE_FILE_NAME_MAX_LEN+1];

    MMIFILE_DEVICE_E         record_save_dev;
    MMIASP_SETTINGS_AUDIO_E audio_type;
    
} MMIASP_NV_SETTING_T;


typedef struct
{
    CAF_APPLET_T            caf_applet;
    MMIASP_START_APPLET_E   start_type;
    MMIASP_MSG_INFO_T*      list_head_ptr;

    MMIASP_NV_SETTING_T     nv_info;
    uint16                  cur_gui_list_index;
    uint16                  new_msg_num;
    uint32                  total_time;           //��ʱ��
    uint32                  process_time;         //���Ž��� 
    HAUDIO                  bkmusic_audio_handle;//��¼��handle
    HAUDIO                  record_handle;       //¼��  handle
    
    
    uint8                   wait_timer_id;        //�绰��֮ͨǰ�ĵȴ���ʱTIMER ID
    uint8                   record_timer_id;      //�绰��ͨ���¼����ʱTIMER ID
    uint8                   count_time_id;   
    uint8                   answer_ring_time_id;   //��¼�ﲥ�ų�ʱTIMER ID  
    uint8                   progress_time_id;   //��¼�ﲥ�Ž�����ˢ��TIME ID
    uint32                  record_time_count;
    uint32                  suffix_id;

    BOOLEAN                 is_record_ring;    //�Ƿ�Ϊ��¼������¼��
    MMIASP_PLAY_STATE_E           play_state; // play state
    MMIASP_MACHINE_CALL_STATE_E   call_state; // 

} MMIASP_APPLET_T;
/*****************************************************************************/
//  Description : get the status of asp
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_NvInit(MMIASP_APPLET_T* applet_ptr);
/*****************************************************************************/
//   Description : 
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC void MMIAPIASP_CreateMsgPlayWin(void);
/*****************************************************************************/
//  Description :  
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_RegMenuGroup(void);
/*****************************************************************************/
//  Description : creat timer for play audio
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegWinIdNameArr(void);
/**********************************************************************************/
//  Description :  You can call this fun if you want to know the msg list total num
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_GetListMsgTotalNum(MMIASP_APPLET_T* applet_ptr);
/*****************************************************************************/
//   Description : set list head
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC void MMIASP_SetListHead(
                               MMIASP_MSG_INFO_T** msg_list_pptr,
                               const MMIASP_MSG_INFO_T* head_node_ptr
                               );
/*****************************************************************************/
//   Description :  ͨ�������ȡ����
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetNameByNumber(
                                      uint8*          call_num_ptr,//in
                                      MMI_STRING_T*   name_ptr //out
                                      );
/*****************************************************************************/
//  Description :  add msg to list
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC MMIASP_MSG_INFO_T *MMIASP_AddMsg2ListEnd(
                                                 MMIASP_MSG_INFO_T **list_head_pptr, 
                                                 MMIASP_MSG_INFO_T *node_ptr
                                                 );
/*****************************************************************************/
//   Description : write msg sys file
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_WriteSysFile(void);
/*****************************************************************************/
//   Description : 
//   Global resource dependence : none
//   Author: 
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_ReadSysFileWithApplet(MMIASP_APPLET_T* applet_ptr);
/*****************************************************************************/
//   Description : set asp info
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_SetCurMsgInfo(
                                    MMIASP_APPLET_T* applet_ptr,
                                    MMIASP_MSG_INFO_T* msg_info_ptr
                                    );
/*****************************************************************************/
//  Description :  del msg
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIASP_MsgDelete(
                                     MMIASP_MSG_INFO_T *list_head_ptr, 
                                     MMIASP_MSG_INFO_T *node_ptr
                                     );
/**********************************************************************************/
//  Description :  
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_DelMarkNodeFromList(MMIASP_APPLET_T* applet_ptr);
/*****************************************************************************/
//  Description : get the status of asp
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_PLAY_STATE_E MMIASP_GetPlayStatus(void);
/*****************************************************************************/
//  Description : Set the status of mpeg4
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_SetPlayStatus(MMIASP_PLAY_STATE_E cur_status);
/*****************************************************************************/
//  Description : pause play
//  Global resource dependence :                                
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Pause(void);
/*****************************************************************************/
//  Description : resume play
//  Global resource dependence :                                
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Resume(void);
/*****************************************************************************/
//  Description : stop play
//  Global resource dependence :                                
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_Stop(void);
/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Play(const wchar* file_name_ptr);
/*****************************************************************************/
//  Description : set the settings to nv
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_SetSetting2NV(MMIASP_NV_SETTING_T set_nv);
/*****************************************************************************/
//  Description : init setting param
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC MMIASP_NV_SETTING_T MMIASP_GetSettingNvValue(void);
/**********************************************************************************/
//  Description :  You can call this fun if you want to know the msg list total num
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
//PUBLIC wchar *MMIASP_GetCurFilePathByIndex(uint16 ctrl_index);
/**********************************************************************************/
//  Description :  You can call this fun if you want to know the msg list total num
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC MMIASP_MSG_INFO_T *MMIASP_GetCurNodeByGuiIndex(
                                                      const MMIASP_APPLET_T* applet_ptr,
                                                      uint16 gui_list_index
                                                      );
/*****************************************************************************/
//  Description : play prev file
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Prev(void);

/*****************************************************************************/
//  Description : play next file
//  Global resource dependence : 
//  Author: byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_Next(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIASP_GetRecordFileDev(MMIASP_SAVE_TYPE_E file_type);
/**********************************************************************************/
//  Description :  
//  Global resource dependence :none
//  Author:byte.guo
//  Note:  
/**********************************************************************************/
PUBLIC uint16 MMIASP_GetNewMsgNum(MMIASP_APPLET_T* applet_ptr);
/*****************************************************************************/
//  Description : stop record 
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_StopRecord(void);
/*****************************************************************************/
//  Description :  allocate memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_MSG_INFO_T *MMIASP_MsgInfoMalloc(void);
/*****************************************************************************/
//  Description :  free memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:byte.guo
//  Note: Call the fun when record finished
/*****************************************************************************/
PUBLIC void MMIASP_MsgListFree(MMIASP_APPLET_T   *applet_ptr);
/*****************************************************************************/
//  Description :  free memory of MMIASP_MSG_INFO_T
//  Global resource dependence :none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_MsgInfoListFree(MMIASP_MSG_INFO_T* msg_list_header_ptr);

/*****************************************************************************/
//   Description : MMIAPIASP_StartApplet
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC MMIASP_APPLET_T* MMIASP_StartApplet(MMIASP_START_APPLET_E start_type);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_CloseAspApplet(void);
/*****************************************************************************/
//  Description : At first,play fixed music for client
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_PlayAudioInCall(
                                      MMIASP_APPLET_T* applet_ptr,
                                      const MMIASP_MSG_INFO_T* msg_info
                                      );
/**********************************************************************************/
//  Description :
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/**********************************************************************************/
PUBLIC BOOLEAN MMIASP_CheckVersionNumber(
                                         MMIFILE_HANDLE file_handle,
                                         const wchar* right_version_ptr,
                                         uint16       right_version_len
                                         );
/**********************************************************************************/
//  Description :
//  Global resource dependence :none
//  Author:byte.guo
//  Note:
/**********************************************************************************/
PUBLIC BOOLEAN MMIASP_CreateNewSysFile(void);

/*****************************************************************************/
//  Description : call back fun
//  Global resource dependence : none
//  Author: byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StopRecordTimer(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: ����Ϊ��λ
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetWaitTime(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetRecordTime(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_ResetSetting(MMIASP_NV_SETTING_T* nv_set_ptr);
/*****************************************************************************/
//  Description : record time out will call this fun
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIASP_RecordTimeout(void);
/*****************************************************************************/
//  Description :  start call answer
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_StartAnswerCall(void);
/*****************************************************************************/
//  Description :  stop  answer call when call finished
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_StopAnswerCall(void);
/*****************************************************************************/
//  Description :  
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_HandlePlayAudioEnd(void);
/*****************************************************************************/
//  Description :  ��¼���Ƿ��ڹ���״̬.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_IsMechineWorking(void);
/*****************************************************************************/
//  Description :  �˺���ר��RealseCallʱ����.......
//  Global resource dependence :none
//  Author:jun.hu
//  Note: ֻ�ڹҶ������绰ʱ����...
/*****************************************************************************/
PUBLIC void MMIASP_RealseCall(void);
/*****************************************************************************/
//  Description : ���ô�¼ͨ����״̬.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIASP_SetCallState(MMIASP_MACHINE_CALL_STATE_E call_state);
/*****************************************************************************/
//  Description : ��ô�¼ͨ����״̬.....
//  Global resource dependence :none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC MMIASP_MACHINE_CALL_STATE_E MMIASP_GetCallState(void);
/*****************************************************************************/
//  Description : �õ�Ӧ�������,���û���򷵻�NONE,��ʾ����Ӧ��....
//  Global resource dependence : none
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetAnswerCondition(void);

/*****************************************************************************/
// Description : Get time info
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
PUBLIC void MMIASP_GetTimeInfo(MMIASP_APPLET_T*    applet_ptr);
/*****************************************************************************/
//  Description : �����¼��������.
//  Global resource dependence : none
//  Author:jun.hu
//  Note: ��ԭ������ʱ��˳����������1����δ���Ѷ�������˳��
//        2������ҵ�Ѵ��ڵ�ʱ���ϵ ������һ���µ�����.....
/*****************************************************************************/
PUBLIC void MMIASP_SortRecordList(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetNewRecordNum(void);
/*****************************************************************************/
//  Description : ��ȡ���µ�������....
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_GetLastestUnreadRecord(
                                             MMIASP_MSG_INFO_T* msg_info_ptr, // out
                                             uint16*            node_index_ptr// out
                                             );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIASP_GetRecordNum(void);
/*****************************************************************************/
//  Description : ��ʼ������¼��....
//  Global resource dependence : none
//  Author:jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StartPlayAnswerMusic(void);
/*****************************************************************************/
//  Description : stop progress refresh timer
//  Global resource dependence : none
//  Author: koki gao
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_StopProgressTimer(void);

/*****************************************************************************/
//  Description : creat progress refresh timer
//  Global resource dependence : none
//  Author: koki gao
//  Note:
/*****************************************************************************/
PUBLIC void CreateProgressTimer(void);

/*****************************************************************************/
//  Description : notify handle function for play after record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_HandleNotify_ply(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : notify handle function for record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_HandleNotify_rec(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);

/*****************************************************************************/
//  Description : play record after setting play times
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_AudioPlay(
                            MMISRV_HANDLE_T  audio_handle,
                            uint32           uiOffset,
                            uint32           play_times
                            );

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify
                        );

/*****************************************************************************/
//  Description : get current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_GetPlayHandle(void);

/*****************************************************************************/
//  Description : get current play handle(for preview ring)
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_GetPreviewHandle(void);

/*****************************************************************************/
//  Description : get current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_GetRecordHandle(void);

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_SetPlayHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : set current play handle(for preview ring)
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_SetPreviewHandle(MMISRV_HANDLE_T handle);

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIASP_SetRecordHandle(MMISRV_HANDLE_T handle);
#endif


/****************************************************************************
** File Name:      mmimtv_main.h                                            *
** Author:         xin.li                                                   *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mtv control module.    *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        xin.li           Create
****************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_MAIN_H_
#define _MMIMTV_MAIN_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mtv_api.h"
#include "mmimtv_data.h "
#include "mmimtv_text.h "
#include "mmimtv_export.h "
#ifdef MBBMS_SUPPORT
#include "mbbms_service.h"
#endif

#include "dal_time.h"
#include "mmifmm_export.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMTV_NET_NUMBER               5       //����������
#define MMIMTV_SERVICE_NUMBER           40      //����ɰ���Ƶ��������
#define MMIMTV_EB_MAX_NUMBER            50      //�����㲥�������
#define MMIMTV_FREQ_STR_LEN             6       //�ֶ������������Ƶ����󳤶�
#define MMIMTV_DISPLAY_TIME_LEN         15
#define MMIMTV_DISPLAY_DATE_LEN         20

#define MMIMTV_RECORD_TIME_DISPLAY_INTERVAL     1000

#define MMIMTV_CHINESE_CODE             0x00636869
#define MMIMTV_ENGLISH_CODE             0x00656e67 

#define MMIMTV_DSP_VOLUME               255

//MBBMS switch play flag
#define MMIMBBMS_SWITCH_PLAY_CMMB       (1 << 0)
#define MMIMBBMS_SWITCH_PLAY_MSK        (1 << 1)

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMTV_PLAY_TYPE_NETWORK,
    MMIMTV_PLAY_TYPE_RECORD,
    MMIMTV_PLAY_TYPE_ID_MAX 
} MMIMTV_PLAY_TYPE_E;


typedef enum
{
    MMIMTV_PLAY_NONE,
    MMIMTV_PLAY_WAITING,
    MMIMTV_PLAY_PLAYING,
    MMIMTV_PLAY_STOPPED,
    MMIMTV_PLAY_STOPPING,
    MMIMTV_PLAY_PAUSED,
    MMIMTV_PLAY_ID_MAX
} MMIMTV_PLAY_STATUS_TYPE_E;


typedef enum
{
    MMIMTV_RECORD_NONE,
    //MMIMTV_RECORD_WAITING,
    MMIMTV_RECORD_RECORDING,
    //MMIMTV_RECORD_STOPPED,
    //MMIMTV_RECORD_STOPPING,
    MMIMTV_RECORD_ID_MAX
} MMIMTV_RECORD_STATUS_TYPE_E;

//ESG parse state
typedef enum
{
    MMIMTV_ESG_PARSE_NONE,
    MMIMTV_ESG_PARSE_PARSING,
    MMIMTV_ESG_PARSE_PARSED,
    MMIMTV_ESG_PARSE_ID_MAX 
} MMIMTV_ESG_PARSE_TYPE_E;

typedef enum
{
    MMIMTV_SNAPSHOT_ERROR_NONE,
    MMIMTV_SNAPSHOT_NO_SPACE,
    MMIMTV_SNAPSHOT_ENCODE_ERROR,
    MMIMTV_SNAPSHOT_ERROR_MAX
} MMIMTV_SNAPSHOT_ERROR_TYPE_E;


typedef enum
{
    MMIMTV_MUTE_NONE,
    MMIMTV_MUTE_LOST_FOCUS,
    MMIMTV_MUTE_MENU_SET,
    MMIMTV_MUTE_ID_MAX
} MMIMTV_MUTE_STATUS_TYPE_E;


typedef enum
{
    MMIMTV_AUTO_SEARCH,
    MMIMTV_MANUAL_SEARCH,
    MMIMTV_CITY_SEARCH,
    MMIMTV_ESG_SEARCH,
    MMIMTV_SEARCH_ID_MAX
} MMIMTV_NETWORK_SEARCH_TYPE_E;


typedef enum
{
    MMIMTV_RESET_NONE,
    MMIMTV_RESET_BY_CHANNEL,
    MMIMTV_RESET_BY_DTL,
    MMIMTV_RESET_BY_OTHER,
    MMIMTV_RESET_ID_MAX
} MMIMTV_RESET_STATUS_TYPE_E;

typedef enum
{
    //MMIMTV_AUDIO_LANGUAGE,
    //MMIMTV_ZOOM_MODE,
    //MMIMTV_RECORD_STORE, 
    MMIMTV_SNAPSHOT_STORE, 
    MMIMTV_OSD_TRANSPARENCY,
    MMIMTV_OSD_DISPLAY_TIME,
    //MMIMTV_WATCH_TIME_TIP,
    //MMIMTV_BROADCAST_BG_PIC, 
    MMIMTV_SETTING_MAX,
    MMIMTV_SUBTITLE_STREAM,  
    MMIMTV_AUDIO_MODE
}MMIMTV_SETTING_LIST_E;

typedef enum
{
#ifndef MBBMS_SUPPORT
    MMIMTV_NET_LIST,
    MMIMTV_SEARCH,
    MMIMTV_RECORD_LIST,
    MMIMTV_EB_LIST,
    //MMIMTV_BOOKING_RECORD,
    MMIMTV_OTHER_APP,
    MMIMTV_SETTING,
    MMIMTV_SYSTEM_INFO,
    MMIMTV_HELP,
    MMIMTV_EXIT,
#else
	MMIMTV_ADD_FAVORITE,    //MBBMS���źŲ�����ʱ����ITEM��Ӱ��Ϊ�����б�
	MMIMTV_MAIN_INTERFACE,  //MBBMS���źŲ�����ʱ����ITEM��Ӱ��Ϊ����
	MMIMTV_EB_LIST,
	MMIMTV_OTHER_APP,
	MMIMTV_SETTING,
	MMIMTV_EXIT,
	
	MMIMTV_NET_LIST,
    MMIMTV_SEARCH,
    MMIMTV_RECORD_LIST,
	MMIMTV_SYSTEM_INFO,
    MMIMTV_HELP,
#endif
    MMIMTV_MAIN_MENU_MAX
} MMIMTV_MAIN_MENU_E;

typedef enum
{
    MMIMTV_TO_OTHER_APP_DIAL,
    MMIMTV_TO_OTHER_APP_SMS,
    MMIMTV_TO_OTHER_APP_PHONEBOOK,
    MMIMTV_TO_OTHER_APP_MENU_MAX
} MMIMTV_TO_OTHER_APP_MENU_E;

typedef enum
{
    OSD_NONE = 0,

    OSD_ENTER_WARNING,
    OSD_INITIALIZING,
    OSD_NO_LAST_SERVICE,
    OSD_IDLE,
    OSD_SHORTCUT_ICON,
    OSD_MAIN_MENU,
    OSD_QUIT_QUERY,
    
    OSD_NET_LIST,
    OSD_NET_LIST_OPT,
    OSD_DEL_NET_QUERY,
    OSD_SERVICE_LIST,
    OSD_SERVICE_LIST_OPT,
    OSD_DEL_SERVICE_QUERY,
    
    OSD_SEARCH_WAITING,
    OSD_SEARCH_INVALID_FRQ,

    OSD_SEARCH_MENU,
    OSD_MANUAL_SEARCH,
    OSD_CITY_SEARCH,
    OSD_CITY_SEARCH_OPT,
    OSD_CITY_EDIT,
    OSD_CITY_ADD,
    OSD_CITY_DEL_QUERY,
    
    OSD_EB_LIST,
    OSD_EB_LIST_OPT,
    OSD_EB_DETAIL,
    OSD_DEL_EB_QUERY,
    OSD_DEL_ALL_EB_QUERY,
    
    OSD_BOOKING_RECORD,
    OSD_BOOKING_NAME,
    OSD_BOOKING_SERVICE_LIST,
    OSD_BOOKING_TIME_INFO,
    //OSD_START_RECORDING_QUERY,
    //OSD_STOP_RECORDING_QUERY,
    OSD_ESG_LIST,
    OSD_SEARCH_ESG_QUERY,
    OSD_SEARCH_ESG_WAITING,
    OSD_PUB,
    OSD_TO_OTHER_APP,
    
    OSD_SETTING,
    OSD_SETTING_AUDIO_LANGUAGE,
    OSD_SETTING_SUBTITLE,
    OSD_SETTING_AUDIO_MODE,
    OSD_SETTING_ZOOM_MODE,
    OSD_SETTING_SCREEN_ADJUST,
    OSD_SETTING_RECORD_STORE,
    OSD_SETTING_SNAPSHOT_STORE,
    OSD_SETTING_TRANSPARENCY,
    OSD_SETTING_DISPLAY_TIME,
    //OSD_SETTING_WATCH_TIP,
    OSD_SETTING_BROADCAST_BG_PIC,

    OSD_HELP,
    
    //OSD_CA,
    //OSD_CA_NO_CA,
    //OSD_CA_ERROR,
    //OSD_CA_NEED_AUTHORIZE,
    //OSD_CA_EXPIRE,
    
    //OSD_ESG_EMPTY,
    //OSD_ESG_SERVICE_LIST,
    //OSD_ESG_DETAIL,
    //OSD_ESG_UPDATE_CNF,
    //OSD_ESG_UPDATE_WAITING,

    //OSD_RECORD_LIST,
	OSD_RECORD_LOADING,
    OSD_RECORD_PLAY,

    OSD_PLAY_CONNECTING,
#ifdef MBBMS_SUPPORT
    OSD_FAVORITE_OPT,
    OSD_READ_EB_QUERY,
    OSD_SUBSCRIBLE_QUERY,
    OSD_CMMB_MODE_WARNING,
#endif

    OSD_ID_MAX
} MMIMTV_OSD_SCENE_TYPE_E;  //��ǰ�򿪵�OSD����

// msg/key remapping
typedef enum
{
    MMIMTV_OPEN_SCENE = 0,
    MMIMTV_CLOSE_SCENE,
    MMIMTV_APP_WEB,
    MMIMTV_APP_LEFT,
    MMIMTV_APP_RIGHT,
    MMIMTV_APP_UP,
    MMIMTV_APP_DOWN,
    MMIMTV_APP_UPSIDE,
    MMIMTV_APP_DOWNSIDE,
    MMIMTV_APP_OK,
    MMIMTV_APP_CANCEL,
    MMIMTV_APP_TP_DOWN,
    MMIMTV_APP_STAR,
    MMIMTV_APP_HASH,
    MMIMTV_APP_CAMERA,
    MMIMTV_CTL_EDITBOX_UPDATE_STRNUM,
	MMIMTV_CTL_LISTBOX_INDEX_MOVED,
    MMIMTV_APP_0,
    MMIMTV_APP_1,
    MMIMTV_APP_2,
    MMIMTV_APP_3,
    MMIMTV_APP_4,
    MMIMTV_APP_5,
    MMIMTV_APP_6,
    MMIMTV_APP_7,
    MMIMTV_APP_8,
    MMIMTV_APP_9,
    //MMIMTV_CTL_OK,
    //MMIMTV_CTL_CANCEL,
    //MMIMTV_CTL_MDISK,
    MMIMTV_APP_ID_MAX,
} MMIMTV_KEY_MAP_TYPE_E;

typedef enum
{
    MMIMTV_OP_NONE,
    MMIMTV_OP_EXIT,         //need quit mtv
    MMIMTV_OP_PLAY_NEXT,    //need play next service(channel)
    MMIMTV_OP_PLAY_RECORD,  //need play record file
    MMIMTV_OP_SEARCH,       //need search program
    MMIMTV_OP_SLIDE,        //need slide play, ���ػط�ʱ֧�ֵ���϶�
    MMIMTV_OP_MAX
}MMIMTV_OP_TYPE_E;      //STOP֮������Ҫ���Ĳ���

//OSD scene msg handle func pointer
typedef void (*MMIMTV_OSD_MSG_PFUNC)(MMIMTV_KEY_MAP_TYPE_E msg_id, DPARAM param);

//mtv play info
typedef struct MMIMTV_INFO_T_tag
{
    uint32      net_eid;                        //network id
    uint32      service_id;                     //service id
    MTV_STIMI_SERVICE_CLASS_E service_type;
    uint32      audio_language;                 //audio language
    uint32      sub_language;                   //sub language
#ifdef MBBMS_SUPPORT
    uint32      frequency;                      //ͨ��Ƶ��Ż�ȡ��ӦƵ�ʷ�������
    uint32      freq_no;                         //MBBMSͨ��Ƶ��ŷ��𲥷�
#endif
    
    wchar       record_file_full_name[MMIMTV_FULL_PATH_MAX_LEN +1]; //¼���ļ�ȫ·�����ƣ����ڱ��ػط�
    MMIMTV_OP_TYPE_E op_type;

    BOOLEAN     is_need_reset_Vchip;            //for CR157456 �ط�¼���л���ֱ��Ҳ��ҪRESET���� @��ʱ�޸ģ�
    BOOLEAN     is_playing_broadcast;           //�Ƿ����ڲ��Ź㲥��Ŀ
    BOOLEAN     is_recived_stop_ack;            //�˳�ʱ���Ƿ��յ��첽ֹͣȷ����Ϣ
#ifdef MBBMS_SUPPORT
    BOOLEAN     is_need_play_after_search;      //MBBMS�������𲥷�
#endif

    uint8       record_tip_timer_id;            //¼����ʾ��ʱ��ID
    uint32      record_tip_time_count;          //¼����ʾ����
    BOOLEAN     is_file_too_small;              //whether file has been recorded too small
    BOOLEAN     has_been_recorded;              //�Ƿ���¼��
    BOOLEAN     is_stream_end;                  //�Ѳ�����¼���ļ�ĩβ
    uint32      tp_offset;                      //TP press down offset, ���ػط�ʱ֧�ֵ���϶�

    //ESG
    MMIMTV_ESG_PARSE_TYPE_E         esg_parse_status;          //ESG����״̬
    
    MMIMTV_RESET_STATUS_TYPE_E      mtv_start_status;       //start type
    MMIMTV_RECORD_STATUS_TYPE_E     record_reset_status;    //record reset type

    MMIMTV_PLAY_TYPE_E              play_type;              //play type
    MMIMTV_PLAY_STATUS_TYPE_E       play_status;            //play status
    MMIMTV_RECORD_STATUS_TYPE_E	    record_status;          //record status
} MMIMTV_INFO_T;


typedef struct MMIMTV_NETWORK_SEARCH_MODE_T_tag
{	
    MMIMTV_NETWORK_SEARCH_TYPE_E    search_type;
    uint32      center_freq;
    uint32      esg_netid;
    uint32      esg_serviceid;
} MMIMTV_NETWORK_SEARCH_MODE_T;

typedef struct MMIMTV_PROG_ID_T_tag
{
	uint32      net_id;
	uint32		center_freq;
	uint32      service_id;
    uint32      audio_language;
    uint32      sub_language;
}MMIMTV_PROG_ID_T;

typedef struct MMIMTV_NUM_SWITCH_INFO_T_Tag
{
    uint8       input_count;
    uint32      input_num;
}MMIMTV_NUM_SWITCH_INFO_T;

typedef struct MMIMTV_AUDIO_LANGUAGE_SCENE_T_tag
{
    uint32      audio_lang_count;
    uint32      *lang_code_ptr;
    uint32      audio_language_code;
}MMIMTV_AUDIO_LANGUAGE_SCENE_T;

typedef struct
{
    BOOLEAN             is_exist;
    uint32              net_id;
    uint32              service_id;
    SCI_DATE_T          min_date;
    SCI_DATE_T          max_date;
    SCI_DATE_T          current_date;
    MMIMTV_SERVICE_T    proglist;
}MMIMTV_ESG_LIST_SCENE_T;


typedef struct mtv_stimi_sig_data_tag
{
    SIGNAL_VARS
    uint32 data_type;       //data type,see  MTV_STIMI_SIG_DATA_TYPE_E
    uint32 operate_status;  //operate status ,see MTV_STIMI_ERR_NUM_E
    uint8  *data_ptr;        //when send broadcast msg to mmi,it is broadcast data
    uint32 data_len;        //when send broadcast msg to mmi,it is broadcast data length
    uint32 signal_value;    //when send TV signal msg to mmi,it is signal level value
    uint32 key_domain_id; //MSK key domain id
}MTV_STIMI_SIG_DATA_T;

// signal structure to mmi
#if 0
typedef struct mtv_stimi_sig_to_mmi_tag
{
    SIGNAL_VARS//signal
    MTV_STIMI_SIG_DATA_T  data;    //operate status  
 }MTV_STIMI_SIG_T;
#endif
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : enter mtv
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EnterMTV(void);

/*****************************************************************************/
// 	Description : exit mtv
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ExitMTV(void);

/*****************************************************************************/
// 	Description : whether mtv player is opened
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsMTVOpen(void);

/*****************************************************************************/
// 	Description : handle key/tp msg etc.
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleMsg(
                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                             DPARAM                 param   //the param of the message
                             );

/*****************************************************************************/
// 	Description : handle timer msg
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleTimerMsg(
                                  uint8 timer_id    //timer id
                                  );

/*****************************************************************************/
// 	Description : handle lose/get focus msg
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleFocusMsg(
                                  MMI_MESSAGE_ID_E msg_id   //the identify of message
                                  );

/*****************************************************************************/
// 	Description : handle TP Press msg
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleTPMsg(
                               DPARAM param     //the param of the message
                               );

/*****************************************************************************/
// 	Description : handle MTV REF/PS msg
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIMTV_HandlePSMsg(
                                       PWND     app_ptr,    //pointer of application
                                       uint16   msg_id,     //the identify of message
                                       DPARAM   param       //the param of the message
                                       );

/*****************************************************************************/
// 	Description : turn up volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TurnUpVolume(void);

/*****************************************************************************/
// 	Description : turn down volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TurnDownVolume(void);

/*****************************************************************************/
// 	Description : register MTV win id name array
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : start recording
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_StartRecord(void);

/*****************************************************************************/
// 	Description : stop recording
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_StopRecord(void);

/*****************************************************************************/
// 	Description : play record file
//	Global resource dependence : none
//  Author: xin.li
//	Note: ��Դ������callback����, ����¼���ļ�
/*****************************************************************************/
PUBLIC void MMIMTV_PlayRecordCallBack(MMIFMM_DATA_INFO_T *list_data_ptr, uint32 index);

/*****************************************************************************/
// 	Description : get current playing info 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_INFO_T* MMIMTV_GetPlayInfo(void);

/*****************************************************************************/
// 	Description : set current focus OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetFocusOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene);

/*****************************************************************************/
// 	Description : get focus OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_SCENE_TYPE_E MMIMTV_GetFocusOSDScene(void);

/*****************************************************************************/
// 	Description : set previous OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:���ڱ��жϳ����Ļָ�
/*****************************************************************************/
PUBLIC void MMIMTV_SetPreOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene);

/*****************************************************************************/
// 	Description : set LAST OSD scene
//	Global resource dependence : none
//  Author: 
//	Note:����OSD_PUBǰ�����ȷ�ϸ÷����ĸ�����������Ҫ���ô˺������洢ǰ������
/*****************************************************************************/
PUBLIC void MMIMTV_SetLastOSDScene(void);

/*****************************************************************************/
// 	Description : get previous OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:���ڱ��жϳ����Ļָ�
/*****************************************************************************/
PUBLIC MMIMTV_OSD_SCENE_TYPE_E MMIMTV_GetPreOSDScene(void);

/*****************************************************************************/
// 	Description : set source OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note: �����Դ�������������ⳡ���ķ���
/*****************************************************************************/
PUBLIC void MMIMTV_SetSourceOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene);

/*****************************************************************************/
// 	Description : set parameter for OSD scene restore
//	Global resource dependence : none
//  Author: 
//	Note:���ó����ָ���Ҫ�Ĳ���
/*****************************************************************************/
PUBLIC void MMIMTV_SetOSDRestoreParam(uint32 param);

/*****************************************************************************/
//  Description : Get Program hint info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const MMIMTV_PROG_HINT_INFO_T* MMIMTV_GetProgramHintInfo(void);

/*****************************************************************************/
//  Description : Get Record play time info
//  Global resource dependence : 
//  Author: xin.li
//  Note: �ط�¼�ƽ�Ŀʱ��ȡʱ����Ϣ,������ǰ����ʱ�估��Ŀ��ʱ��
/*****************************************************************************/
PUBLIC void MMIMTV_GetRecordPlayTimeInfo(uint32 *time_count, uint32 *time_total);

/*****************************************************************************/
//  Description : Get frequency user input
//  Global resource dependence : 
//  Author: xin.li
//  Note: ��ȡ�û�֮ǰ�����Ƶ��
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetLastInputFreq(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetVideoSpec(
                        MMIMTV_ZOOM_MODE_E zoom_mode   //video layer size mode
                        );

/*****************************************************************************/
// 	Description : open OSD_TO_OTHER_APP scene
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenOtherFunction(void);

/*****************************************************************************/
//  Description : Get current audio language
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MTV_MMI_TEXT_ID_E MMIMTV_GetCurrentAudioLanguage(void);

/*****************************************************************************/
// 	Description : �������ñ���ͼƬ�������л�ʱ��Ҫ���裬����ᵼ������ģ��
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetBG(void);

/*****************************************************************************/
// 	Description : whether 6600V chip is running
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsVRunning(void);

/*****************************************************************************/
// 	Description : close current scene directly
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseFocusScene(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIMTV_DateCompare(SCI_DATE_T date1,   
                                SCI_DATE_T date2
                                );

/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIMTV_MovePrevDay(
                               SCI_DATE_T  date_in,
                               SCI_DATE_T  *date_out_ptr
                               );

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIMTV_MoveNextDay(
                               SCI_DATE_T  date_in,
                               SCI_DATE_T  *date_out_ptr
                               );

/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MIMTV_GetRecordTimeCount(void);

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC MMIMTV_SNAPSHOT_ERROR_TYPE_E MMIMTV_Snapshot(void);

/*****************************************************************************/
// 	Description : open OSD_ESG_LIST scene  
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenESGList(void);

//#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : ���CMMB��VоƬ������س�ʼ��
//	Global resource dependence : none
//  Author: xin.li
//	Note: MBBMS��CMMB���ɵ���
/*****************************************************************************/
PUBLIC void MMIMTV_InitCMMB(void);

/*****************************************************************************/
// 	Description : ���Ŵ���ʧȥ����ʱ��CMMB OSD͸��������
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetLoseFocusOSDSpec(void);

/*****************************************************************************/
// 	Description : ���Ŵ��ڻ�ý���ʱ��CMMB OSD͸��������
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetGetFocusOSDSpec(void);


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetDelayPowerOff(BOOLEAN is_delay);

/*****************************************************************************/
// 	Description : ��Ƶ���л������Ƶ����
//	Global resource dependence : 
//  Author: xin.li
//	Note: ���ݵ�ǰ���������µ�Ƶ���б�
/*****************************************************************************/
PUBLIC void MMIMTV_AddServiceItemToTabCtrl(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
// 	Description : Ƶ���л�������ʱ����
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TimeOutPlay(void);

/*****************************************************************************/
// 	Description : ���ּ��л�Ƶ������
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_NumSwitchPlay(void);

/*****************************************************************************/
//  Description : get current OSD transparency value
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetTransparencyValue(MMIMTV_OSD_TRANSPARENCY_E transparency);

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : �ر�CMMB�������Դ���ͷ�
//	Global resource dependence : none
//  Author: xin.li
//	Note: ��MBBMS����
/*****************************************************************************/
PUBLIC void MMIMBBMS_CloseCMMB(void);

/*****************************************************************************/
// 	Description : �Ƿ��跢�𲥷�
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNeedPlayService(void);

/*****************************************************************************/
// 	Description : �Ƿ���ָ�����
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNeedResumePlay(void);

/*****************************************************************************/
// 	Description : �����Ƿ��貥�ű�־
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetIsNeedPlayService(BOOLEAN is_need_play);

/*****************************************************************************/
// 	Description : restore resume play flag
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ResetResumePlayFlag(void);

/*****************************************************************************/
// 	Description : parse emergency broadcast data
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ParseEmergencyBroadcastData(MTVSERVICE_CMMB_BROADCAST_IND_T *sig_data, 
                                                    uint32 *eb_grade);   //pointer of stimi signal data struct
/*****************************************************************************/
//  Description : set update status of MSK   
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
PUBLIC void SetMSKUpdateStatus(uint32 mskupdatestatus);

/*****************************************************************************/
// 	Description : ������ּ���̨��Ϣ
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearNumSwitchInfo(void);

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_RequestHandle(void);

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_GetAudioHandle(void);

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_SetAudioHandle(MMISRV_HANDLE_T handle);

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMTV_MAIN_H_ */
#endif

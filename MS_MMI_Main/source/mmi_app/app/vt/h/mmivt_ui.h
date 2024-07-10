/*****************************************************************************
** File Name:      mmivt_ui.h                                                   *
** Author:                                                                   *
** Date:           21/12/2009                                               *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                 *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 21/12/2009     samboo.shen       Create
******************************************************************************/

#ifndef _MMIVT_UI_H_
#define _MMIVT_UI_H_ 

//#include "mmivt_export.h"
#ifdef VT_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmicc_export.h"
#include "cafapplet.h"
#include "mmivt_kernel.h"
#include "mmivt_export.h"
#include "mmivt_kernel_refAdapter.h"

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
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIVT_UI_MSG_DIAL,      //���ز���
    MMIVT_UI_MSG_REMOTE_CONNECT,    //�Է�����
    MMIVT_UI_MSG_REMOTE_INCOMING,   //����
    MMIVT_UI_MSG_CONNECT,   //���ؽ���
    MMIVT_UI_MSG_CONNECT_END,   //�ײ��Ѿ���ɽ�������������ͨ��
    MMIVT_UI_MSG_HANGUP,    //�Ҷ�
    MMIVT_UI_MSG_DISCONNECT,//�Է��Ҷϵ绰������ʱ�绰����һ����ȫ�Ҷϣ�
    MMIVT_UI_MSG_DISC_END,  //�ײ��Ѿ���ɹҶ�
    MMIVT_UI_MSG_REMOTE_FB_REQ, //�յ��Է����������������У�
    MMIVT_UI_MSG_FB_ACCEPT, //ͬ�����
    MMIVT_UI_MSG_FB_REJECT, //�ܾ�����
    MMIVT_UI_MSG_FB_REQ,    //�������
    MMIVT_UI_MSG_QUIT,    //VTͨ���������ر�������ش���(�Ҷ���Ϣ��ʾ���ڣ��Ҷ���ʾ���ڣ�������ʾ���ڣ�����ѯ�ʴ��ڣ�����ȴ����ڹر�ʱ��Ҫ����)
    MMIVT_UI_MSG_REDIAL, //�����Զ��ز�

    MMIVT_UI_MSG_REMOTE_ALERTING, //VT�������Է���ʼ����
    MMIVT_UI_MSG_ENTER_MMR, //�����ý�����״̬
    MMIVT_UI_MSG_QUIT_MMR, //�˳���ý�����״̬
    MMIVT_UI_MSG_REMOTE_NO_SIGNAL_START, //�Զ�������
    MMIVT_UI_MSG_REMOTE_NO_SIGNAL_STOP, //�Զ����ݻظ�
    MMIVT_UI_MSG_REMOTE_CAMERA_OFF, //�Զ˹ر�����ͷ
    MMIVT_UI_MSG_REMOTE_CAMERA_ON, //�Զ˿�������ͷ
    MMIVT_UI_MSG_REF_ERROR_IND, //REF�ײ㷢���쳣
    MMIVT_UI_MSG_UPDATE_AUDIO_DEVICE, //���������豸
    MMIVT_UI_MSG_MAX
}MMIVT_UI_MSG_E;    //VT UI�ϵ���ز���

typedef enum
{
    MMIVT_UI_STATE_IDLE,           //��VT��ʾ״̬��������ʾVT����
    MMIVT_UI_STATE_MO_CALLING, //���˺���������ʾ״̬
    MMIVT_UI_STATE_CONNECTED,  //ͨ��������ʾ״̬
    MMIVT_UI_STATE_MT_INCOMING,  //���������ʾ״̬
    MMIVT_UI_STATE_CONNECTING, //����ָ��ʹ��������ʾ״̬���û�ѡ����������ڵȴ�ȷ�ϣ�
    MMIVT_UI_STATE_CONN_DISCONNECTING, //��֮ͨ��Ҷ��н�����ʾ״̬
    MMIVT_UI_STATE_CONN_DISCONNECTED,  //��֮ͨ��Ҷ���ɽ�����ʾ״̬
    MMIVT_UI_STATE_UNCONN_DISCONNECTING,   //δ��ͨ�Ҷ��н�����ʾ״̬
    MMIVT_UI_STATE_UNCONN_DISCONNECTED,    //δ��ͨ�Ҷ���ɽ�����ʾ״̬
    MMIVT_UI_STATE_FB_CALL_QUIT,   //�����˳���ʾ������ʾ״̬
    MMIVT_UI_STATE_FB_CALL_QUERY,  //����ѯ�ʽ�����ʾ״̬
    MMIVT_UI_STATE_FB_CALL_PROMPT, //������ʾ������ʾ״̬
    MMIVT_UI_STATE_FB_WAITING, //����ȴ�������ʾ״̬
    MMIVT_UI_STATE_MAX
}MMIVT_UI_STATE_E;// VT UI�ϵ���Ӧ״̬

typedef struct
{
    MMIVT_UI_STATE_E cur_state; //��ǰ״̬
    MMIVT_UI_STATE_E last_state; //ǰһ��״̬
    //sub_state (�������Ƿ����)
    MMIVT_UI_MSG_E msg;
}MMIVT_CALL_UI_STATE_T;//��¼VT CALL��ǰ״̬


typedef struct  
{
    MN_DUAL_SYS_E dual_sys;
    uint8* tele_num;
    uint8 tele_len;
}MMIVT_UI_INFO_DIAL_T; //UIִ��MMIVT_UI_MSG_DIAL���м���������


typedef struct  
{
    int32 redialed_times; //�ز��Ĵ���
    int32 redial_times_max; //�ز���������
}MMIVT_UI_REDIAL_INFO_T; 
//typedef struct
//{
//    MN_DUAL_SYS_E dual_sys;
//    uint8* tele_num;
//    uint8 tele_len;
//}VT_UI_INFO_REMOTE_INCOMING_T; //UIִ��MMIVT_UI_MSG_REMOTE_INCOMING���м���������

//****ͨ�����������ʾ��Ϣ
typedef enum
{
    MMIVT_OPTION_BANK_MODE_NORMAL, //��ͨģʽ��ֻ��ʾ��һ��ѡ��icon
    MMIVT_OPTION_BANK_MODE_VIDEO_OPTION, //��Ƶ��ʾ����ģʽ����ʾ�ڶ���ѡ��icon
    MMIVT_OPTION_BANK_MODE_VOLUME, //������ʾģʽ��ֻ��ʾ��һ��ѡ��icon������������
    MMIVT_OPTION_BANK_MODE_USERINPUT,  //ֻ��ʾ���������
    MMIVT_OPTION_BANK_MODE_MAX
}MMIVT_OPTION_BANK_MODE_E;//ͨ������ѡ�������ʾ��ʽ

typedef enum
{
    MMIVT_1LEVEL_AUDIO_OPTION,    //��Ͳ�����л�ѡ��
    MMIVT_1LEVEL_VIDEO_OPTION,    //��Ƶ����ѡ��
    MMIVT_1LEVEL_CAMERA_OPTION,   //����ͷ�л�ѡ��
    MMIVT_1LEVEL_DISPLAY_MODE_OPTION, //Զ������ʾѡ��
    MMIVT_1LEVEL_OPTION_FOCUS_MAX
}MMIVT_1LEVEL_OPTION_E;//ͨ�����ڵ�һ��ѡ��

typedef enum
{
    MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION,    //��Ļ����ѡ��
    MMIVT_2LEVEL_VIDEO_CONTRAST_OPTION,    //�Աȶ�ѡ��
    MMIVT_2LEVEL_VIDEO_BRIGHT_OPTION,  //����ѡ��
    MMIVT_2LEVEL_VIDEO_ROTATE_OPTION,  //��תѡ��
    MMIVT_2LEVEL_VIDEO_OPTION_MAX
}MMIVT_2LEVEL_VIDEO_OPTION_E;//��ʾ���õڶ���ѡ��



typedef struct  
{
    MMIVT_OPTION_BANK_MODE_E option_bank_mode;//ͨ������ѡ�������ʾ��ʽ
    MMIVT_1LEVEL_OPTION_E option_focus;//��һ��ѡ���λ��
    MMIVT_2LEVEL_VIDEO_OPTION_E video_sub_option_focus;//��ʾ���õڶ���ѡ���λ��
    //BOOLEAN is_option_active; //��һ��ѡ���Ƿ񼤻�
    BOOLEAN is_video_sub_option_active;//�ڶ���ѡ���Ƿ񼤻�����ƶ���������ڷǼ���״̬������OK�������뼤��״̬�����Ե�����Ӧ������
    MMIVT_IMAGE_ROTATE_MODE_E image_rotate_mode;//��ת�����������������λ��
    MMIVT_IMAGE_ROTATE_STATUS_E image_local_rotate_status;//��ǰͼ�����ת״̬

    BOOLEAN is_handfree_show; //TRUE:����������ʾ�´�����Ϊhandfree,FALSE:����Ϊhandhold
    MMIVT_DISP_MODE_E display_mode; //Զ�˽�����ʾ��ʽ
    MMIVT_CAMERA_ID_E camera_id; //camera�л�
    MMIVT_VIDEO_LEVEL_E lcd_bright_level;
    MMIVT_VIDEO_LEVEL_E contrast_level;
    MMIVT_VIDEO_LEVEL_E brightness_level;
    MMIVT_VIDEO_LEVEL_E zoom_level;

    BOOLEAN is_recording; //�Ƿ�����¼��
    MMIVT_RECORD_TYPE_E record_type;
    BOOLEAN is_replacing; //�����Ƿ��������
    BOOLEAN is_remote_replacing; //Զ���Ƿ������
    BOOLEAN is_camera_on; //����ͷ�Ƿ��
    BOOLEAN is_hangup_by_user; //�Ƿ����û��Ҷ�
    
}VT_DISPLAY_INFO_T; //��¼��ǰ��ʾ��������״̬
//******ͨ�����������ʾ��Ϣ

typedef struct 
{
    uint32 ui_call_time_count;  //��ʾʱ��
    MMIVT_CALL_UI_STATE_T ui_state_info; //ui��״̬��Ϣ
    VT_DISPLAY_INFO_T ui_display_info;//ui��ͨ�����ڽ�����ʾ״̬
    MMIVT_UI_REDIAL_INFO_T redial_info; //�Զ��ز�����Ϣ
}MMIVT_INSTANCE_T;

typedef struct 
{
    CAF_APPLET_T caf_applet;
    MMIVT_INSTANCE_T vt_instance;
}MMIVT_APPLET_T;   //vt applet����



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : MMIVT_HandleUIMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��������ps��ui�Ĳ�����������Ӧ��״̬ԾǨ��
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleUIMsg(
    MMIVT_UI_MSG_E msg,	//��Ҫ�����msg
    DPARAM parm	//msg�����Ϣ
    );

/*****************************************************************************/
//  Description : Get VT Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ���Է���PNULL�����ú�����Ҫ���ж�
/*****************************************************************************/
PUBLIC MMIVT_APPLET_T *MMIVT_GetAppletInstance(void);

/*****************************************************************************/
//  Description : MMIVT_StartVTApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StartVTApplet(void);

/*****************************************************************************/
//  Description :MMIVT_StopVTApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StopVTApplet(void);

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_Redial(void);
    
/*****************************************************************************/
// 	Description : MMIVT_RegWinIdNameArr
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : MMIVT_StopRecord
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVT_StopRecord(void);

/*****************************************************************************/
//  Description : ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�ײ��������
/*****************************************************************************/
PUBLIC void MMIVT_ErrorNotify(MMIVT_REF_RET_E err_type);

/*****************************************************************************/
//  Description : MMIVT_ErrorNotifyCallBack
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�ײ��������
/*****************************************************************************/
//PUBLIC void MMIVT_ErrorNotifyCallBack(DVT_MW_RET_E err_type);

PUBLIC  BOOLEAN MMIAPIVT_GetAudioPlayResumeStatus(void);

PUBLIC void MMIAPIVT_SetAudioPlayResumeStatus(BOOLEAN  status);



#ifdef   __cplusplus
    }
#endif
    
#endif //VT_SUPPORT
    
#endif //_MMIVT_UI_H_
    

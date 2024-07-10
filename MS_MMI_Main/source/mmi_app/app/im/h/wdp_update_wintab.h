#ifndef _WDP_update_wintab_H_    
#define _WDP_update_wintab_H_   

#include "sci_types.h"
#include "mmiim_id.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmifmm_export.h"

typedef enum
{
	SG_WIN_ID_START = MMIIM_MAX_WIN_ID,

	SG_WIN_ID_SOGOU_SETTING,					//�ѹ����뷨����
	SG_WIN_ID_SKIN_MANAGEMENT,					//Ƥ������
	SG_WIN_ID_SKIN_SELECT,						//Ƥ��ѡ��
	SG_WIN_ID_SKIN_OPTION,						//Ƥ��ѡ��
	SG_WIN_ID_SKIN_SORT,						//Ƥ������
	SG_WIN_ID_SKIN_PREVIEW,						//Ƥ��Ԥ��	
	SG_WIN_ID_SKIN_PREVIEW2,					//Ĭ��Ƥ��Ԥ��	
	SG_WIN_ID_MORE_SKIN_DOWNLOAD,				//����Ƥ������
	SG_WIN_ID_SKIN_BROWSE,						//Ƥ���������

	SG_WIN_ID_VOCABULARY_UPDATE,				//�ʿ����

	SG_WIN_ID_UPDATE_SETTINGS,					//��������
	SG_WIN_ID_SYSTEM_UPDATE_SETTINGS,			//ϵͳ��������
	SG_WIN_ID_VOCABULARY_UPDATE_NOTIFICATION,	//�ʿ������ʾ
	SG_WIN_ID_CONNECT_REMIND,					//��������
	SG_WIN_ID_ACCESS_POINT,						//�����
	SG_WIN_ID_AUTOMATIC_UPDATE_SETTINGS,		//�Զ���������
	SG_WIN_ID_VOCABULARY_UPDATE_FREQUENCY,		//�ʿ����Ƶ��
	SG_WIN_ID_VOCABULARY_UPDATE_TIME,			//�ʿ����ʱ��
	SG_WIN_ID_NETWORKING_REMIND,				//GPRS��������
	SG_WIN_ID_AUTOUPDATEWORD_REMIND,			//�Զ����´ʿ�����
	SG_WIN_ID_UPDATE_WATCHING,					//���¼��Ӵ���
	SG_WIN_ID_UPDATE_CANCEL_QUERY,				//ȡ�����´���
	SG_WIN_ID_UPDATE_TIME_OUT_QUERY,			//��ʱ����
	SG_WIN_ID_UPDATE_WORDS_CONTINUE,			//���´ʿ��������
	SG_WIN_ID_SIMCARDSELECT,					// SIM CARD SELECT WINDOW
	SG_WIN_ID_MAX
}SG_WINDOW_ID_E;
typedef enum
{
#ifdef SOGOU_USE_PDA_SKIN
	SOGOU_SETTING_MENU_SKIN_MANAGEMENT,
#endif	
	SOGOU_SETTING_MENU_VOCABULARY_UPDATE,
	SOGOU_SETTING_MENU_UPDATE_SETTINGS,
	SOGOU_SETTING_MENU_RESET_DEFAULT
}SG_UPDATE_SOGOU_SETTING_MENU;//6_7_1
typedef enum
{
	SKIN_MANAGEMENT_MENU_SKIN_SELECT,
	SKIN_MANAGEMENT_MENU_DOWNLOAD_SKIN
}SG_UPDATE_SKIN_MANAGEMENT_MENU;//6_7_1
typedef enum
{
	SKIN_OPTION_MENU_VIEW,
	SKIN_OPTION_MENU_2,//CLOSE OR STARTUP
	SKIN_OPTION_MENU_DELETE,
	SKIN_OPTION_MENU_SORT
}SG_UPDATE_SKIN_OPTION_MENU;//6_7_1
typedef enum
{
	SG_UPDATE_SETTINGS_MENU_SYSTEM_SETTINGS,
	SG_UPDATE_SETTINGS_MENU_AUTOMATIC_SETTINGS
}SG_UPDATE_SETTINGS_MENU;//6_7_1
typedef enum
{
	SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_FREQUENCY,
	SG_UPDATE_AUTOMATIC_SETTINGS_MENU_VOCABULARY_UPDATE_TIME
}SG_UPDATE_AUTOMATIC_SETTINGS_MENU;//6_7_1
typedef enum
{
	SG_UPDATE_TYPE_NONE,
	SG_UPDATE_TYPE_VOCABULARY,
	SG_UPDATE_TYPE_SKIN_DIR,
	SG_UPDATE_TYPE_SKIN_FILE,
	SG_UPDATE_TYPE_MAX
}SG_UPDATE_TYPE_E;

typedef enum
{
	SG_CTRL_ID_START = MMIIM_MAX_CTRL_ID,
	SG_CTRL_ID_MENU_SOGOU_SETTING,
	SG_CTRL_ID_MENU_SKIN_MANAGEMENT,
	SG_CTRL_ID_MENU_SKIN_SELECT,
	SG_CTRL_ID_MENU_SKIN_OPTION,
	SG_CTRL_ID_SKIN_SORT,
	SG_CTRL_ID_ANIM_SKIN_PREVIEW,
	SG_CTRL_ID_MENU_SKIN_BROWSE,

	SG_CTRL_ID_MENU_VOCABULARY_UPDATE,
	SG_CTRL_ID_MENU_UPDATE_SETTINGS,
	SG_CTRL_ID_LIST_SYSTEM_UPDATE_SETTINGS,
	SG_CTRL_ID_LIST_VOCABULARY_UPDATE_NOTIFICATION,
	SG_CTRL_ID_LIST_CONNECT_REMIND,
	SG_CTRL_ID_LIST_ACCESS_POINT,
	SG_CTRL_ID_LIST_AUTOMATIC_UPDATE_SETTINGS,
	SG_CTRL_ID_LIST_VOCABULARY_UPDATE_FREQUENCY,
	SG_CTRL_ID_TIMEEDITBOX_VOCABULARY_UPDATE_TIME,

	SG_CTRL_ID_TEXTBOX_NETWORKING_REMIND,
	SG_CTRL_ID_TEXTBOX_AUTOUPDATEWORD_REMIND,
	SG_CTRL_ID_LIST_SIMCARDSELECT,
#ifdef MMI_IM_PDA_SUPPORT
    SG_CTRL_ID_VOCABULARY_UPDATE_TIME_FORM_ID,
    SG_CTRL_ID_AUTOUPDATEWORD_REMIND_FORM_ID,
    SG_CTRL_ID_NETWORKING_REMIND_FORM_ID,

    SG_CTRL_ID_SETTING_MENU_FORM_ID,
    SG_CTRL_ID_SKIN_MANAGEMENT_FORM_ID,
    SG_CTRL_ID_SKIN_MANAGEMENT_LABEL_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_FORM_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_LABEL_ID,
    SG_CTRL_ID_UPDATE_SETTING_FORM_ID,
    SG_CTRL_ID_UPDATE_SETTING_LABEL_ID,
    SG_CTRL_ID_RESET_DEFAULT_FORM_ID,
    SG_CTRL_ID_RESET_DEFAULT_LABEL_ID,
    SG_CTRL_ID_SKIN_MANAGEMENT_MENU_FORM_ID,
    SG_CTRL_ID_SKIN_SELECT_FORM_ID,
    SG_CTRL_ID_SKIN_SELECT_LABEL_ID,
    SG_CTRL_ID_MORE_SKIN_DOWNLOAD_FORM_ID,
    SG_CTRL_ID_MORE_SKIN_DOWNLOAD_LABEL_ID,
    SG_CTRL_ID_SKIN_OPTION_MENU_FORM_ID,
    SG_CTRL_ID_VIEW_FORM_ID,
    SG_CTRL_ID_VIEW_LABEL_ID,
    SG_CTRL_ID_SETUP_FORM_ID,
    SG_CTRL_ID_SETUP_LABEL_ID,
    SG_CTRL_ID_DELETE_FORM_ID,
    SG_CTRL_ID_DELETE_LABEL_ID,
    SG_CTRL_ID_SORT_FORM_ID,
    SG_CTRL_ID_SORT_LABEL_ID,

    SG_CTRL_ID_UPDATE_SETTING_MENU_FORM_ID,
    SG_CTRL_ID_SYSTEM_SETTING_FORM_ID,
    SG_CTRL_ID_SYSTEM_SETTING_LABEL_ID,
    SG_CTRL_ID_AUTOUPDATE_SETTING_FORM_ID,
    SG_CTRL_ID_AUTOUPDATE_SETTING_LABEL_ID,

    SG_CTRL_ID_AUTOUPDATE_SETTING_MENU_FORM_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_SETTING_FORM_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_FREQUENCY_LABEL_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_TIME_MENU_FORM_ID,
    SG_CTRL_ID_VOCABULARY_UPDATE_TIME_LABEL_ID,
#endif
	SG_MAX_CTRL_ID,		
}SG_CONTROL_ID_E;
typedef enum
{
	VOCABULARY_UPDATE_FREQUENCY_E_ONE,
	VOCABULARY_UPDATE_FREQUENCY_E_SEVEN,
 	VOCABULARY_UPDATE_FREQUENCY_E_FIFTEEN,
	VOCABULARY_UPDATE_FREQUENCY_E_THIRTY,
	VOCABULARY_UPDATE_FREQUENCY_E_NEVER,
	VOCABULARY_UPDATE_FREQUENCY_MAX
}VOCABULARY_UPDATE_FREQUENCY_E;
typedef struct vocabulary_update_frequency_tag
{
	VOCABULARY_UPDATE_FREQUENCY_E frequency;
	SCI_TIME_T time;
}VOCABULARY_UPDATE_FREQUENCY_T;
//ϵͳ��������
typedef struct system_update_settings_data_tag
{
	BOOLEAN is_need_update_notif;
	BOOLEAN is_need_connect_remind;
}SYSTEM_UPDATE_SETTINGS_DATA_T;
// Ƥ���б���Ϣ
typedef struct mmisg_pic_list_tag
{
    uint32                  time;                                   // ����ʱ��
    uint32                  file_size;                              // �ļ���С       
    uint16                  filename_len;                           // �ļ������ȣ��ַ���
    uint16                  pad;                                    // ���
    wchar                   filename[MMIFMM_FULL_FILENAME_LEN +1];  // �ļ���
} MMISG_PIC_List_T;
// Ƥ���б���������
typedef enum
{
    MMISG_SORT_TYPE_TIME,  // ��ʱ��
    MMISG_SORT_TYPE_NAME,  // ������
    MMISG_SORT_TYPE_MAX
} MMISG_SORT_TYPE_E;

//wait win message
#define MSG_SG_UPD_WAIT_SETING_NETWORK					MSG_SG_UPD_WAIT_BASE+1
#define MSG_SG_UPD_WAIT_CONNECTING						MSG_SG_UPD_WAIT_BASE+3
#define MSG_SG_UPD_WAIT_SENDING							MSG_SG_UPD_WAIT_BASE+4
#define MSG_SG_UPD_WAIT_RECEIVING						MSG_SG_UPD_WAIT_BASE+6
#define MSG_SG_UPD_WAIT_UNPACKING						MSG_SG_UPD_WAIT_BASE+7

#define MSG_SG_UPD_WAIT_SET_NETWORK_FALED				MSG_SG_UPD_WAIT_BASE+8
#define MSG_SG_UPD_WAIT_RECEIVE_FAILED					MSG_SG_UPD_WAIT_BASE+12
#define MSG_SG_UPD_WAIT_UNPACK_FAILED					MSG_SG_UPD_WAIT_BASE+13

#define MSG_SG_UPD_WAIT_CONNECTING_TIME_OUT				MSG_SG_UPD_WAIT_BASE+14
#define MSG_SG_UPD_WAIT_SUCCEED							MSG_SG_UPD_WAIT_BASE+15

#define SG_DEVICE_NAME   MMIFILE_DEVICE_SDCARD //MMIFILE_DEVICE_UDISK
#define SG_DEVICE_NAME_LEN MMIFILE_DEVICE_SDCARD_LEN //MMIFILE_DEVICE_UDISK_LEN
#define SG_SKIN_DIR L"\x0040\x0073\x006F\x0067\x006F\x0075"	//@sogou
#define SG_SKIN_DIR_LEN 6 //unicode len
#define SG_SKIN_FIND_SUFFIX L"\x002A\x002E\x006A\x0070\x0067"	//*.jpg

#ifdef WIN32
#define SG_D_SKIN_DEFAULT_FILE L"D:\\@sogou\\Ĭ��Ƥ��.jpg"//Ĭ��Ƥ��
#define SG_D_SKIN_CONFIG_FILE L"D:\\@sogou\\curskin" //Ƥ�������ļ�
#define SG_D_SKIN_CONFIG_PATH L"D:\\@sogou\\" //Ƥ�������ļ�Ŀ¼
#define SG_D_BUILD_TIME L"D:\\@sogou\\buildtime"//�ʿ��ļ�����ʱ��//6_19_1
#define SG_E_SKIN_DEFAULT_FILE L"E:\\@sogou\\Ĭ��Ƥ��.jpg"//Ĭ��Ƥ��
#define SG_E_SKIN_CONFIG_FILE L"E:\\@sogou\\curskin" //Ƥ�������ļ�
#define SG_E_SKIN_CONFIG_PATH L"E:\\@sogou\\" //Ƥ�������ļ�Ŀ¼
#define SG_E_BUILD_TIME L"E:\\@sogou\\buildtime"//�ʿ��ļ�����ʱ��//6_19_1
#else
#define SG_D_SKIN_DEFAULT_FILE L"\x0044\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x9ED8\x8BA4\x76AE\x80A4\x002E\x006A\x0070\x0067"//Ĭ��Ƥ��
#define SG_D_SKIN_CONFIG_FILE L"\x0044\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x0063\x0075\x0072\x0073\x006B\x0069\x006E" //Ƥ�������ļ�
#define SG_D_SKIN_CONFIG_PATH L"\x0044\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C" //Ƥ�������ļ�Ŀ¼
#define SG_D_BUILD_TIME L"\x0044\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x0062\x0075\x0069\x006C\x0064\x0074\x0069\x006D\x0065"//�ʿ��ļ�����ʱ��//6_19_1
#define SG_E_SKIN_DEFAULT_FILE L"\x0045\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x9ED8\x8BA4\x76AE\x80A4\x002E\x006A\x0070\x0067"//Ĭ��Ƥ��
#define SG_E_SKIN_CONFIG_FILE L"\x0045\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x0063\x0075\x0072\x0073\x006B\x0069\x006E" //Ƥ�������ļ�
#define SG_E_SKIN_CONFIG_PATH L"\x0045\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C" //Ƥ�������ļ�Ŀ¼
#define SG_E_BUILD_TIME L"\x0045\x003A\x005C\x0040\x0073\x006F\x0067\x006F\x0075\x005C\x0062\x0075\x0069\x006C\x0064\x0074\x0069\x006D\x0065"//�ʿ��ļ�����ʱ��//6_19_1
#endif

/*****************************************************************************/
//  Description : ������ģʽ����
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_OpenSogouIMSetInputModeWin(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : ��Ƥ���������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SoGouOpenSkinBrowseWin(void);
#endif
/*****************************************************************************/
//  Description : �Զ���������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SogouAutoUpdateWordRemind(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : ����ѡ���Ƥ���ļ�
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGSetupSelectedSkinFile(uint16* pfileName,uint16 nameLen);

/*****************************************************************************/
//  Description : ��ȡƤ���ļ��б�
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIIM_GetSkinFileList(void);

/*****************************************************************************/
//  Description : ����Ƥ���ļ��б�ռ�
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGAllocSkinFileList(uint16 size);

/*****************************************************************************/
//  Description : �ͷ�Ƥ���ļ��б�
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGFreeSkinFileList(void);

/*****************************************************************************/
//  Description : ��ȡƤ���ļ�id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8* MMIAPIIM_SGGetSkinInfPicId(void);

/*****************************************************************************/
//  Description : ����Ƥ���ṹ�ռ�
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGAllocSkinInf(void);
#endif
/*****************************************************************************/
//  Description : ���ø�������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetUpdateType(SG_UPDATE_TYPE_E type);

/*****************************************************************************/
//  Description : ��ȡ��������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SG_UPDATE_TYPE_E MMIAPIIM_SGGetUpdateType(void);

#ifdef SOGOU_USE_PDA_SKIN
/*****************************************************************************/
//  Description : ��ȡ����Ƥ����ȫ·��
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPIIM_SGGetDownloadSkinFullPath(void);
#endif
/*****************************************************************************/
//  Description : ��ȡ��ʾ�ַ���
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16* MMIAPIIM_GetDisplayStr(void);

/*****************************************************************************/
//  Description : �Ƿ�ͨ��WIFI���и���
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetIsOverWifi(void);
PUBLIC BOOLEAN MMIAPIIM_SGDelAutoEvent(void);//6_12_1

#endif


/*
*���ļ�ΪVM lib�ڡ��ⲿ�ӿڵĺ���ԭ�ͼ����ݽṹ���壬�벻Ҫ�޸�����ļ���
*/

#ifndef _HERO_ENGINE_API_H_
#define _HERO_ENGINE_API_H_

#include "mmk_type.h"

/*image process cmd*/
#define HR_IMG_GET_INFO 3001
#define HR_IMG_DEC_JPG 3002
#define HR_IMG_GET_STAT 3003
#define HR_IMG_DEC_GIF 3004
#define HR_IMG_FREE_GIF 3005
#define HR_IMG_DRAW_JPG 3010 //����JPGͼƬ

/*mr AUDIO type*/
#define HR_ACI_AUDIO_MIDI 1
#define HR_ACI_AUDIO_WAV 2
#define HR_ACI_AUDIO_MP3 3
#define HR_ACI_AUDIO_AMR 4
#define HR_ACI_AUDIO_PCM 5
#define HR_ACI_AUDIO_AAC 6
#define HR_ACI_AUDIO_M4A 7

/*audio process cmd*/
#define HR_ACI_OPEN	         1       //���豸
#define HR_ACI_LOAD_FILE	     2	     //����һ���ļ�
#define HR_ACI_LOAD_BUF       3	     //���ػ�������
#define HR_ACI_PLAY           4	     //�ӵ�ǰ��λ�ÿ�ʼ����
#define HR_ACI_PAUSE          5       //��ͣ����
#define HR_ACI_RESUME         6       //��������
#define HR_ACI_STOP           7       //ֹͣ����
#define HR_ACI_CLOSE          8       //�ر��豸
#define HR_ACI_STATUS         9       //��ȡ��ǰ�豸��״̬
#define HR_ACI_POSITION_TIME  10      //���ò���λ�ã��Ѿ������˶���ʱ��
#define HR_ACI_POSITION_LEN   11      //���ò���λ�ã��Ѿ�����������ļ����߻������ʼλ�õ�ƫ����
#define HR_ACI_TOTALTIME      12      //��ȡ���׸�Ĳ���ʱ��
#define HR_ACI_PLAYTIME       13      //��ȡ��ǰ�Ĳ��Ž���ʱ��
#define HR_ACI_PLAYLEN        14      //��ȡ��ǰ�Ĳ��Ž��ȣ������ʼλ�õ�ƫ����
#define HR_ACI_PLAYTIME_MS    15      //��ȡ��ǰ�Ĳ��Ž���ʱ��(ms)
#define HR_ACI_SETTIME_MS     31      //���õ�ǰ�Ĳ��Ž���ʱ��(ms)

#define HR_ACI_MULTIPATH_OPEN     22  //��ͨ�����ż�������
#define HR_ACI_MULTIPATH_PLAY     23  //��ͨ�������Ѽ��ص�����
#define HR_ACI_MULTIPATH_STOP     24  //��ͨ������ֹͣ
#define HR_ACI_MULTIPATH_CLOSE    25  //�ͷŶ�ͨ�����ż��ص�����


/*PLAT CMD CODES DEFINE*/
#define HR_PLAT_ADAPT_CPUCLK 1	//����CPUƵ��
#define HR_PLAT_GET_KEY_NUM 2		//��ȡkeyӳ�����Ŀ
#define HR_PLAT_GET_KEY_STATE 3    //��ѯĳ����״̬
#define HR_PLAT_NES_ACTION_IND 4	//ģ������Ϸ������ر�֪ͨ
#define HR_PLAT_ENVINFO_WRITE 5	//MRPӦ��д��¼
#define HR_PLAT_ENVINFO_READ 6	//MRPӦ�ö���¼
#define HR_PLAT_SET_KEYSOUND 7	//�����������رտ���: 0 �رգ� 1 ����
#define HR_PLAT_SET_NESWAPSAVEDEV 8	//����wap���ص�.nes������ʹ�õĴ洢����
#define HR_PLAT_SET_LCD_REFRESH_MODE 9	//����LCDˢ����ʽ:0 ͬ����1�첽
#define HR_PLAT_SET_TP_SOUND         10//���ô�����ʾ��:0 �رգ�1����
#define HR_PLAT_SET_LCD_ROTATE       101//����LCD��ת�Ƕ�

#ifdef HERO_ENGINE_HMGS_SUPPORT
#define HR_GET_PHONE_NUMBER_LOCATION__x   902 // ��ѯ�ֻ����������
#endif

// 20181107 start
#ifdef HERO_ENGINE_TTS_SUPPORT
#define HR_PLAY_TTS_REQ__x   910
#define HR_STOP_TTS_REQ__x   911
#define HR_GET_TTS_TYPE   912
#endif
// 20181107 end

#define HR_PLAT_CALL_INTERFACE_SUPPORT   920  //��ֲ��hr_call�ܷ�����֧��

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
#define HERO_GET_TTS_SWITCH__x    9800
#define HERO_SET_TTS_SWITCH__x    9801
#endif

#define HERO_HTTP_SUPPORT__x				9700
#define HERO_HTTP_GET_REQ__x				9701
#define HERO_HTTP_POST_REQ__x				9702
#define HERO_HTTP_STOP_REQ__x				9703
#define HERO_HTTP_DECODE_PKG__x				9704
#define HERO_HTTP_UPDATE_EXTDATA__x			9705

//add 20200507 begin
#define HERO_APP_GET_PRICE_REQ				9720
#define HERO_APP_REST_PRICE_REQ				9721 
#define HERO_APP_IS_SHOW_NETTIP_REQ			9722
#define HERO_APP_GET_TOOLBAR_TYPE_REQ		9723
#define HERO_APP_GET_MENU_SWTICE_REQ		9724
//add 20200507 end

#define HR_GET_DEVICE_SUPPORT  8001
#define HR_CHECK_GPRS_SUPPORT  8002

#define HR_PLAT_CONNECT_RESULT 1001	//NO-BLOCKING modeʱ��ѯ��ǰ����״̬
#define HR_PLAT_SET_CONNECT_TIMEOUT 1002		//�������ӳ�ʱʱ��
#define HR_PLAT_APP_ACTIVE2FRONT_REQ 1003		//֪ͨ��ֲ�㽫��̨���е�Ӧ�ü��ǰ̨�����С�
#define HR_PLAT_SET_ACTIVE_SIM 1004		//��sim������ʱ��������sim��
#define HR_PLAT_APP_PICSTOP_IND      1006  //ֹͣ��idle����ʾͼ��
#define HR_PLAT_SMS_RECIVE_ACTION    1011  //�����յ��¶���ʱ�Ƿ���ʾ��ʾ����
#define HR_PLAT_GET_BACKLIGHT_STATE  1020  //��ȡ��ǰ����״̬
#define HR_PLAT_GET_SC_REQ           1106  //��ȡ�������ĵĺ��������
#define HR_PLAT_VM_PAUSE_MODE 1202		//����VM��pauseʱ��ֲ���Ƿ񻹽��¼�������VM��
#define HR_PLAT_GET_TC_INFO 1205 	//	��ȡ�ֻ��Ƿ��Ǵ�������Ϣ��
#define HR_PLAT_GET_LANG_INFO 1206 	//	��ȡ�ֻ�����������Ϣ��
#define HR_PLAT_GET_RAND_NUM 1211   //ȡ�������
#define HR_PLAT_GET_CUR_SCENE        1213  //��ȡ��ǰ�龰ģʽ
#define HR_PLAT_FORBID_HUP2IDLE      1214  //���θý����°��һ�����idle�Ĺ���
#define HR_PLAT_GET_CELLINFO_START   1215  //��ʼ��cell id
#define HR_PLAT_GET_CELLINFO_END     1216  //������ȡcell id
#define HR_PLAT_WEATHER_UPDATED_IND 1217  //
#define HR_PLAT_TCARD_STATUS         1218  //��ȡ�洢��״̬
#define HR_PLAT_GET_FILE_POS         1231  //��õ�ǰ�ļ�ָ��λ��

#define HR_PLAT_GET_SOUND_VOLUME    1301	//��ȡ������С������
#define HR_PLAT_SET_SOUND_VOLUME    1302	//���������Ĵ�С

#define HR_PLAT_CHECK_WIFI          1327   //���WIFI�Ƿ���
#define HR_PLAT_SET_WIFI_SWITCH     1328   //����ΪWIFIģʽ
#define HR_PLAT_BACKSTAGE_SUPPORT   1391   //�Ƿ�֧�ֺ�̨����

//modify 20211211 begin
#define HR_PLAT_GET_DEVIDE_TYPE     1051  //��ȡ�豸����
//modify 20211211 end

#define HR_PLAT_CAMERA_INIT         2500
#define HR_PLATEX_CAMERA_PREVIEW    2501
#define HR_PLAT_CAMERA_PAUSE        2502
#define HR_PLAT_CAMERA_RESUME       2503
#define HR_PLAT_CAMERA_CLOSE        2504
#define HR_PLATEX_CAMERA_CAPTURE    2505
#define HR_PLAT_CAMERA_ZOOM_MAX     2506
#define HR_PLAT_CAMERA_ZOOM_IN      2507
#define HR_PLAT_CAMERA_ZOOM_OUT     2508
#define HR_PLATEX_CAMERA_GETDATA    2509

/* motion sensor */
#define	HR_MOTION_STOP_LISTEN    4001
#define	HR_MOTION_PWRON           4002
#define	HR_MOTION_PWROFF          4003
#define	HR_MOTION_LISTEN_SHAKE   4004
#define	HR_MOTION_LISTEN_TILT    4005
#define	HR_MOTION_GET_MAX_ACC    4006

#define HR_PLATEX_CREATE_LIST 1
#define HR_PLATEX_SET_LISTITEM 2
#define HR_PLATEX_GET_MP3_DIR 11
#define HR_PLATEX_GET_PIC_DIR 12		
#define HR_PLATEX_GET_LCD_INFO 1001
#define HR_PLATEX_FREE_LCD_BUF 1002
#define HR_PLATEX_SOCKET_BIND 1003
#define HR_PLATEX_APP_BACKRUN_IND 1004//֪ͨ��ֲ�㱾Ӧ����Ҫ��̨����
#define HR_PLATEX_APP_PICIND_REQ 1005  //������ֲ����ʾһ��ͼƬ��ͼƬ��ʾ��λ������ֲ��ȷ����������idle������icon�������������ط�����ʾ�ȵ������
#define HR_PLATEX_START_LOCALAPP 1006  //��������Ӧ��
#define HR_PLATEX_SET_WALLPAPER  1009  //����ǽֽ������
#define HR_PLATEX_ALLOC_MEM          1014  //�����ڴ���չ�ӿ�
#define HR_PLATEX_FREE_MEM           1015  //�ͷ��ڴ���չ�ӿ�
#define HR_PLATEX_GET_BUILD_DT       1116  //��ȡ���̱�������ʱ��
#define HR_PLATEX_GET_HW_VER         1117  //��ȡӲ���汾��
#define HR_PLATEX_SET_FILE_DEV       1204
#define HR_PLATEX_UCS_TO_GB          1207
#define HR_PLATEX_SET_RINGS          1208
#define HR_PLATEX_GET_NES_KEYSCONFIG 1210
#define HR_PLATEX_GET_NES_DIR_NAME 1220
#define HR_PLATEX_GET_NES_TITLE_NAME 1221
#define HR_PLATEX_LCD_SLEEP_OFF      1222  //����lcd���ⳣ����lcd ���ܽ���sleep		
#define HR_PLATEX_LCD_SLEEP_ON       1223	//�ָ�lcd �������ã�lcd �ɶ�ʱ����sleep		
#define HR_PLATEX_GET_CELL_INFO      1224  //��ȡС����Ϣ
#define HR_PLATEX_GET_NETWORK_ID     1226  //��ȡ����ID
#define HR_PLATEX_GET_FILE_CRTTIME   1232  //��ȡ�ļ�����ʱ��
#define HR_PLATEX_GET_DISK_SPACE 1305//���ָ���̷���ʣ��ռӴ�С��
#define HR_PLATEX_GET_DISK_NAME 1306//���ָ���̷�����ʾ����
#define HR_PLATEX_GET_DUALSIM_INFO 1307//��õ�ǰsim��״̬-for dual sim sys

#define HR_PLATEX_SET_BLT_LAYER                  1308 //���ò���ˢ�µĲ�
#define HR_PLATEX_CREATE_LAYER                   1309 //������
#define HR_PLAT_RELEASE_LAYER                    1310 //�ͷŲ�
#define HR_PLATEX_GET_BASE_LAYER                 1311 //���Base����
#define HR_PLAT_SET_ACTIVE_LAYER                 1312 //���ü����
#define HR_PLATEX_SET_LAYER_POS                  1313 //���ò�λ��
#define HR_PLATEX_SET_PNGBG_LAYER                1314 //����PNG������
#define HR_PLATEX_GET_ACTICE_LAYER               1315 //��ȡ�������Ϣ
#define HR_PLATEX_LAYER_APPEND                   1316 //�����
#define HR_PLAT_SET_LAYER_ALPHA                  1317 //���ò��͸��ֵ
#define HR_PLAT_SET_LAYER_NO_ALPHA              1318 //�رղ��͸������
#define HR_PLAT_SET_LAYER_TRANSPARENT           1319 //���ò��͸��ɫ
#define HR_PLAT_SET_LAYER_NO_TRANSPARENT        1320 //�رղ��͸��ɫ����
#define HR_PLATEX_LOCK_INVALIDATE                1321 //����ˢ����
#define HR_PLATEX_UNLOCK_INVALIDATE              1322 //ȡ��ˢ����

#define HR_PLATEX_FMGR_OPERATION_BASE 		1400
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_INIT 	(HR_PLATEX_FMGR_OPERATION_BASE + 1)
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_SET 	(HR_PLATEX_FMGR_OPERATION_BASE + 2)
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_CLEAR 	(HR_PLATEX_FMGR_OPERATION_BASE + 3)
#define HR_PLATEX_BROWSE_FMGR 				(HR_PLATEX_FMGR_OPERATION_BASE + 4)
#define HR_PLATEX_BROWSE_FMGR_GET_PATH 		(HR_PLATEX_FMGR_OPERATION_BASE + 5)
#define HR_PLATEX_BROWSE_FMGR_EXIT 			(HR_PLATEX_FMGR_OPERATION_BASE + 6)
#define HR_PLATEX_DOVE_SET_SMS_STRATEGY    4101

#define HR_PLATEX_CREATE_PB_OBJ 4041
#define HR_PLATEX_GET_A_PB_INFO 4042
#define HR_PLATEX_CLOSE_PB_OBJ 4043
#define HR_PLATEX_SET_TIMES 5002
#ifdef __HR_CFG_MULTITP_SUPPORT__
#define HR_PLATEX_GET_TP_POINTS 4200
#endif
#define HR_PLATEX_LOCK_IRAM   0xA0003 //����IRAM����IRAM��ʼ��ַ�ͳ���
#define HR_PLATEX_UNLOCK_IRAM 0xA0004 //�ͷ�IRAM

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
#define HR_PLATEX_SHORTCUT_SUPPORT   0xB0000 //�Ƿ�֧��ShortCut
#define HR_PLATEX_SHORTCUT_INSTALL   0xB0001 //��װapp
#define HR_PLATEX_SHORTCUT_REMOVE    0xB0002 //ж��app
#define HR_PLATEX_SHORTCUT_PROGESS   0xB0003 //�������ؽ���
#endif


#define HR_PLATEX_CHECK_WIFI_SUPPORT         0x90003   //����Ƿ�֧��WIFI
#define HR_PLATEX_GET_HEADSET_EX             9510//0x90004   //��ȡ��չ���� // 20190430

#define HR_PLATEX_GET_ABLUM_PATH             0x9000A   //��ȡϵͳ���Ĭ��·��
#define HR_PLATEX_IMAGE_ENCODE               0x9000B   //��RGB565�����ݱ���Ϊָ����ʽ��ͼƬ
#define HR_PLATEX_GET_TOUCHSCREEN_TYPE      0x9000C   //��ȡ��������
#define HR_PLATEX_GET_CAMERA_SUPPORT        0x9000D   //��ȡ�Ƿ�֧��CAMERA�ӿ�

/* begin:added by Tommy.yan 20120120 */
#define HR_PLATEX_SEND_SMS_LOCALUI		1008	//���ñ���UI���Ͷ���
#define HR_PLATEX_SAVE_SMS				1010	//����һ�����ŵ��ռ�����
#define HR_PLAT_GET_SMS_STATUS 		1100  	//��ȡ����ģ��״̬
#define HR_PLAT_GET_SMS_NUM			1101 	//��ȡ���������ж��ŵ�����
#define HR_PLATEX_GET_SMS_INFO			1102 	//��ȡָ��һ�����ŵ���Ϣ(���롢״̬��ʱ���)
#define HR_PLATEX_GET_SMS_CONTENT 		1103 	//��ȡһ������(��������)
#define HR_PLATEX_DELETE_SMS			1104 	//ɾ��һ������
#define HR_PLAT_GET_SMS_CAPACITY		1105 	//��ȡ�ֻ���sim�����Ŵ洢����
#define HR_PLATEX_SET_SMS_STATE		0x9000F //�޸��ռ���/���������״̬
#define HR_PLATEX_REPLY_SMS_REQ        1130  //���ñ���UI�ظ�����
#define HR_PLATEX_SEND_MMS           	1203   	//���Ͳ���
/* end:added by Tommy.yan 20120120 */

//2010-02-03 add begin
#define HR_PLATEX_SKY_QQ 1332
//2010-02-03 add end

// �ж��Ƿ�Ϊ�Լ��������
#define HR_PLATEX_GET_VM_TYPE   1610

//modify 20181117 begin
#define HR_START_DSM_REQ__x 				 900
#define HR_DSM_APP_VERSION__x				 901
// 950 - 970 �ѱ�ռ��
#define HR_START_HERO_APP_REQ__x			 950

#define HERO_APP_API_BASE 9000
#define HERO_APP_API_SEND_SMS 9001
#define HERO_APP_API_GET_AVAILABLE_SIM 9002
#define HERO_APP_API_ADD_KEY 9003
#define HERO_APP_API_GET_VERSION 9004
#define HERO_APP_API_SET_CYCLE 9005
#define HERO_APP_API_SET_START_PARAMETER 9006
//modify 20181117 end

#ifdef HERO_APP_SHORTCUT_SUPPORT
#define HERO_APP_API_GET_SHORTCUT_DATA 9300
#define HERO_APP_API_SET_SHORTCUT_DATA 9301
#endif

//2009-7-7 add begin
#define HR_GET_DATA_ACCOUNT_REQ          1107
#define HR_GET_DATA_ACCOUNT              1108
#define HR_SET_DATA_ACCOUNT              1109
#define HR_SET_DATA_ACCOUNT_USE_FLAG     1110
#define HR_SET_DATA_ACCOUNT_MTK_UI       1111
//2009-7-7 add end

#define HR_VOICE_RECORD_START             2700
#define HR_VOICE_RECORD_PAUSE             2701
#define HR_VOICE_RECORD_RESUME            2702
#define HR_VOICE_RECORD_STOP              2703
#define HR_VOICE_RECORD_STREAM            2704

#define HR_LCD_ROTATE_NORMAL			0
#define HR_LCD_ROTATE_90				1
#define HR_LCD_ROTATE_180				2
#define HR_LCD_ROTATE_270				3
#define HR_LCD_MIRROR					4
#define HR_LCD_MIRROR_ROTATE_90			5
#define HR_LCD_MIRROR_ROTATE_180		6
#define HR_LCD_MIRROR_ROTATE_270		7

/*����ֻ�ƽ̨û�ж�����������ͣ���#if ����ڵĶ�����Ч*/
#if 0
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  long int  int32;                /* signed 32 bit value */
typedef  unsigned char      uint8;       /* Signed 8  bit value */
typedef  signed char        int8;        /* Signed 8  bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
#endif

//Type��
#define HR_CALL_RING 	0
#define HR_SMS_RING 	1
#define HR_ALARM_RING 	2


#define HR_FILE_RDONLY     1//��ֻ���ķ�ʽ���ļ���
#define HR_FILE_WRONLY     2//��ֻд�ķ�ʽ���ļ���
#define HR_FILE_RDWR       4//�Զ�д�ķ�ʽ���ļ���
#define HR_FILE_CREATE     8//����ļ������ڣ��������ļ���
#define HR_FILE_SHARE      16//�Թ���ķ�ʽ���ļ���

#define HR_IS_FILE     	   1      //�ļ�
#define HR_IS_DIR      	   2      //Ŀ¼
#define HR_IS_INVALID  	   8  //��Ч(���ļ�����Ŀ¼)

/* file browser */
#define HR_FMGR_TYPE_ALL 			1
#define HR_FMGR_TYPE_FOLDER 		2
#define HR_FMGR_TYPE_FOLDER_DOT 	3
#define HR_FMGR_TYPE_UNKNOW 		4

/* image */
#define HR_FMGR_TYPE_BMP 		    5
#define HR_FMGR_TYPE_JPG 			6
#define HR_FMGR_TYPE_JPEG 			7	
#define HR_FMGR_TYPE_GIF 			8
#define HR_FMGR_TYPE_PNG 			9
#define HR_FMGR_TYPE_EMS 			10
#define HR_FMGR_TYPE_ANM 			11
#define HR_FMGR_TYPE_WBMP 			12
#define HR_FMGR_TYPE_WBM 			13

/* audio */
#define HR_FMGR_TYPE_IMY 			14
#define HR_FMGR_TYPE_MID 			15
#define HR_FMGR_TYPE_MIDI 			16
#define HR_FMGR_TYPE_WAV 			17
#define HR_FMGR_TYPE_AMR 			18
#define HR_FMGR_TYPE_AAC 			19
#define HR_FMGR_TYPE_DAF 			20
#define HR_FMGR_TYPE_VM 			21
#define HR_FMGR_TYPE_AWB 			22
#define HR_FMGR_TYPE_AIF 			23
#define HR_FMGR_TYPE_AIFF 			24
#define HR_FMGR_TYPE_AIFC 			25
#define HR_FMGR_TYPE_AU 			26
#define HR_FMGR_TYPE_SND 			27
#define HR_FMGR_TYPE_M4A 			28
#define HR_FMGR_TYPE_MMF  			29
#define HR_FMGR_TYPE_WMA 			30

/* video */
#define HR_FMGR_TYPE_3GP 			31
#define HR_FMGR_TYPE_MP4 			32
#define HR_FMGR_TYPE_AVI 			33	

/* others */
#define HR_FMGR_TYPE_JAD 			34
#define HR_FMGR_TYPE_JAR 			35
#define HR_FMGR_TYPE_VCF 			36
#define HR_FMGR_TYPE_VCS 			37
#define HR_FMGR_TYPE_THEME 			38
#define HR_FMGR_TYPE_TOO 			39
#define HR_FMGR_TYPE_NES 			40
#define HR_FMGR_TYPE_ZIP 			41
#define HR_FMGR_TYPE_ZPK          	42


#define HR_FILE_HANDLE int32
#define HR_SEARCH_HANDLE int32

#define HR_SUCCESS  0    //�ɹ�
#define HR_FAILED   -1    //ʧ��
#define HR_IGNORE   1     //������
#define HR_WAITING   2     //�첽(������)ģʽ
//#define MR_NO_SUPPORT   -2     //��֧��

#define HR_PLAT_VALUE_BASE 1000    //����ĳЩ����ֵ�Ļ���ֵ
#define HERO_APP_AUDIO_VOL_MAX 5

//�绰����������ֵ
#define HR_SMS_NOT_READY	1000
#define HR_SMS_READY		1001

#define HR_PHB_MAX_NAME 				(36*2) 
#define HR_PHB_MAX_NUMBER 			    (48*2) 
#define HR_SMS_MAX_CONTENT            (160)
#define HR_SMS_MAX_NUMBER			21
 


#define HR_PLATEX_PHB_OPERATION_BASE 		   4000
#define HR_PLATEX_PHB_SEARCH_ENTRY           (HR_PLATEX_PHB_OPERATION_BASE + 11) 
#define HR_PLATEX_PHB_SET_ENTRY              (HR_PLATEX_PHB_OPERATION_BASE + 12)
#define HR_PLATEX_PHB_GET_ENTRY                (HR_PLATEX_PHB_OPERATION_BASE + 16) 
#define HR_PLATEX_PHB_COPY_ENTRY               (HR_PLATEX_PHB_OPERATION_BASE + 17) 
#define HR_PLATEX_PHB_SET_OWNER_ENTRY        (HR_PLATEX_PHB_OPERATION_BASE + 21)  
#define HR_PLATEX_PHB_GET_OWNER_ENTRY         (HR_PLATEX_PHB_OPERATION_BASE + 22) 
#define HR_PLATEX_PHB_DELETE_ENTRY_ALL          (HR_PLATEX_PHB_OPERATION_BASE + 31) 
#define HR_PLATEX_PHB_GET_COUNT                (HR_PLATEX_PHB_OPERATION_BASE + 32) 
#define HR_PLATEX_PHB_GET_STATUS                (HR_PLATEX_PHB_OPERATION_BASE + 33)
#define HR_PLATEX_GET_PB_TOTAL_NUM             (HR_PLATEX_PHB_OPERATION_BASE + 44)

#ifdef HERO_CAMERA_SUPPORT
#define HR_CAMERA_SUPPORT	 		6510
#define HR_CAMERA_OPEN				6511
#define HR_CAMERA_CLOSE 			6512
#define HR_CAMERA_CAPTURE	 		6513
#define HR_CAMERA_CAPTURE_RELEASE   6514
#define HR_CAMERA_GET_ROTATION_MODE 6515
#endif

#ifdef HERO_QR_DECODE_SUPPORT
#define HR_QR_DECODE_SUPPORT		6610
#define HR_QR_DECODE_OPEN			6611
#define HR_QR_DECODE_CLOSE			6612
#define HR_QR_DECODE_PUSH_IMG		6613
#endif

//modify 20181114 begin
#define HERO_APP_API_GET_CELL_INFO  9007

#define HR_INIT_CALL_LOG_OPERATE		9100
#define HR_DESTORY_CALL_LOG_OPERATE		9102
#define HR_GET_CALL_LOG_COUNT			9103
#define HR_GET_CALL_LOG_BY_INDEX		9104
#define HR_DELETE_CALL_LOG_BY_INDEX		9105

#define HR_INIT_SMS_OPERATE		9120
#define HR_DESTORY_SMS_OPERATE		9121
#define HR_GET_SMS_COUNT		9122
#define HR_GET_SMS_BY_INDEX		9123
#define HR_DELETE_SMS_BY_INDEX		9124
#define HR_DELETE_SMS_IS_MEMORY_FULL		9125

#define HR_GET_AMAM_INFO				9200
#define HR_SET_AMAM_INFO				9201

#define HR_GET_EXT_PARAM				9140

#define HR_REQ_POWER_OFF				9205
#define HR_REQ_SET_VOLUME				9206
#define HR_REQ_SET_CUR_SENCE			9207

#define HR_READ_OLALA_CFG_DATA			9220
#define HR_WRITE_OLALA_CFG_DATA			9221

#define HR_SET_FORWARD_REQ				9230
#define HR_CANCEL_FORWARD_REQ			9231

#define HR_FAMILY_DURABLE_DATA_LIST_IS_FULL 9241
#define HR_FAMILY_DURABLE_DATA_LIST_SIZE 9242
#define HR_FAMILY_DURABLE_DATA_ADD 9243
#define HR_FAMILY_DURABLE_DATA_FIND_START 9244
#define HR_FAMILY_DURABLE_DATA_FIND_NEXT 9245
#define HR_FAMILY_DURABLE_DATA_REMOVE 9246
#define HR_FAMILY_CLEAR_ALL_DURABLE_DATA 9247
#define HR_FAMILY_GET_GLOBAL_BUF 9248

// smart
#define HR_START_UP_PLAT_APP  9520 
#define HR_GET_AUDIO_PLAY_LIST_DIR 9521 
#define HR_GET_SMART_AUTH_ID  9522 
#define HR_GET_CUSTOMER_KEY  9523 
#define HR_GET_CUSTOMER_HELP_STR  9504 
#define HR_GB_2_UNICODE  9505 
//modify 20181120 begin
#define HR_GET_REMINDER_NV_DATA  9506
#define HR_SET_REMINDER_NV_DATA  9507
//modify 20181120 end
//modify 20181114 end

#define HERO_FAMILY_TCP_SUPPORT 9750
#define HERO_FAMILY_TCP_INIT_CONNECTION  9751
#define HERO_FAMILY_TCP_START_CONNECTION 9752
#define HERO_FAMILY_TCP_STOP_CONNECTION 9753
#define HERO_FAMILY_TCP_IS_CONNECTED 9754
#define HERO_FAMILY_TCP_SEND_DATA 9755

//modify 20210203 begin
#define HERO_FM_AMR_STREAM_OPEN 9811
#define HERO_FM_AMR_STREAM_CLOSE 9812
#define HERO_FM_AMR_STREAM_PUSH 9813
#define HERO_FM_AMR_STREAM_RESET 9814
#define HERO_FM_AMR_STREAM_GET_CACHE_SIZE 9815
#define HERO_FM_AMR_STREAM_SET_VOLUME 9816
#define HERO_FM_AMR_STREAM_GET_MAX_CACHE_SIZE 9817
#define HERO_FM_AMR_STREAM_PAUSE 9818
#define HERO_FM_AMR_STREAM_RESUME 9819
#define HERO_FM_AMR_STREAM_GET_CURRENT_TIME 9820
#define HERO_FM_AMR_STREAM_GET_VOLUME 9821
//modify 20210203 end

#define HERO_EXTERN_CHECK_APP_SUPPORT 9860

// modify 20210301
#define HR_PHONE_FREE_SIZE_STATE 9901
// modify 20210301
typedef struct
{
	int32 index;
	int8 type;
	int8 state;
}T_HR_SET_SMS_STATE_REQ;

typedef struct
{
	int32 index;//��ʾʱ������
	int8 type; //HR_SMS_INBOX, HR_SMS_OUTBOX
}T_HR_GET_SMS_INFO_REQ;

typedef struct
{
	int8 t_stamp[6];  /*t_stamp[0]��t_stamp[1]��t_stamp[2]��
					   t_stamp[3]ʱt_stamp[4]��t_stamp[5]��*/
	int8 num[HR_SMS_MAX_NUMBER];		/*���ͷ�����*/  
	int8 sms_status;	/*Read or Unread */
	int8 sms_storage;   /* HR_SMS_STORAGE_ME , HR_SMS_STORAGE_SIM*/
}T_HR_MSG_INFO;

typedef enum
{
	HR_SMS_NOBOX ,
	HR_SMS_UNREAD,
	HR_SMS_INBOX,
	HR_SMS_OUTBOX,
	HR_SMS_DRAFTS,
	HR_SMS_AWAITS,
	HR_SMS_DATA,
	HR_SMS_UNSENT,
	HR_SMS_READED,
	HR_SMS_SENT
}T_HR_SMS_BOX_E;

/**
 * ���Ŵ洢����
 */
typedef enum
{
	/** �洢��SIM���� */
	HR_SMS_STORAGE_SIM = 0,
	/** �洢���ֻ��� */
	HR_SMS_STORAGE_ME	
} HR_SMS_STORAGE_E;

typedef struct
{
    int8  number[HR_PHB_MAX_NUMBER]; 
    int32 simid; 
}T_HR_SMS_REPLY_ENTRY_REQ;

typedef struct
{
    int8 number[HR_PHB_MAX_NUMBER]; 
    int8 content[HR_SMS_MAX_CONTENT]; 
	int32 status;
	int32 flag; 
}T_HR_SMS_SAVE_ENTRY_REQ;

/**
 * ���Ͷ�������
 */
typedef struct
{
	/** Ŀ�ĺ���(ASCII) */
	char *addr;
	/** UNICODE BE��ʽ������ */
	char *content;
} HR_SMS_SEND_REQ_T;

/**
 * ������ŵı��flag
 */
typedef enum
{
    HR_SMSSAVE_DISP_SAVINGSCREEN = 0x0010,
    HR_SMSSAVE_DISP_RESULTSCREEN = 0x0020,
    HR_SMSSAVE_DISP_SIM_OPT = 0x0200,
    HR_SMSSAVE_TO_SIM1 = 0x0400,
    HR_SMSSAVE_TO_SIM2 = 0x0800,
    HR_SMSSAVE_TO_SIM3 = 0x0800,
    HR_SMSSAVE_TO_SIM4 = 0x0800,
    HR_SMSSAVE_ENUM_END
}T_HR_SMS_SAVE_FLAG_E;

typedef enum
{
    SAFE_HISTORY_UNREAD,
    SAFE_HISTORY_READ
}SAFE_HISTORY_STATUS_E;


typedef enum
{
    SAFE_SIMCARD_1,
    SAFE_SIMCARD_2,
    SAFE_SIMCARD_3,
    SAFE_SIMCARD_4
}SAFE_SIMCARD_E;

/**
 * ��������������
 */
typedef struct
{
	/** ���ŵķ��ͷ����� */
	int8 number[HR_PHB_MAX_NUMBER];
	/** UNICODE BE��ʽ������ */
	int8 content[HR_SMS_MAX_CONTENT];
	/** ����״̬0: ��ʾδ�� 1: ��ʾ�Ѷ� */
	int32 status;
	/** �ο� #HR_SMS_SAVE_FLAG_E, Ŀǰ HR_SMSSAVE_TO_SIM1 or HR_SMSSAVE_TO_SIM2 */
	int32 flag;
} HR_SMS_SAVE_ENTRY_REQ_T;

enum
{
	HR_SMS_OP_GET_ME_SIZE,
	HR_SMS_OP_GET_SIM_SIZE,
	HR_SMS_OP_DELETE,
	HR_SMS_OP_GET_MSG_CONTENT,
	//wsl add for safe
	HR_SMS_OP_SAVE_SMS,
	HR_SMS_OP_SET_STATUS
};

enum {
   HR_KEY_0,               //���� 0
   HR_KEY_1,               //���� 1
   HR_KEY_2,               //���� 2
   HR_KEY_3,               //���� 3
   HR_KEY_4,               //���� 4
   HR_KEY_5,               //���� 5
   HR_KEY_6,               //���� 6
   HR_KEY_7,               //���� 7
   HR_KEY_8,               //���� 8
   HR_KEY_9,               //���� 9
   HR_KEY_STAR,            //���� *
   HR_KEY_POUND,           //���� #
   HR_KEY_UP,              //���� ��
   HR_KEY_DOWN,            //���� ��
   HR_KEY_LEFT,            //���� ��
   HR_KEY_RIGHT,           //���� ��
   HR_KEY_POWER,           //���� �һ���
   HR_KEY_SOFTLEFT,        //���� �����
   HR_KEY_SOFTRIGHT,       //���� �����
   HR_KEY_SEND,            //���� ������
   HR_KEY_SELECT,          //���� ȷ��/ѡ����������м���ȷ�ϼ���������Ϊ�ü���
   HR_KEY_VOLUME_UP,       //���� �����
   HR_KEY_VOLUME_DOWN,     //���� �����
   HR_KEY_CLEAR,
   HR_KEY_A,               //��Ϸģ����A��
   HR_KEY_B,               //��Ϸģ����B��
   HR_KEY_CAPTURE,         //���ռ�

   HR_KEY_QWERTY_A,
   HR_KEY_QWERTY_B,
   HR_KEY_QWERTY_C,
   HR_KEY_QWERTY_D,
   HR_KEY_QWERTY_E,
   HR_KEY_QWERTY_F,
   HR_KEY_QWERTY_G,
   HR_KEY_QWERTY_H,
   HR_KEY_QWERTY_I,
   HR_KEY_QWERTY_J,
   HR_KEY_QWERTY_K,
   HR_KEY_QWERTY_L,
   HR_KEY_QWERTY_M,
   HR_KEY_QWERTY_N,
   HR_KEY_QWERTY_O,
   HR_KEY_QWERTY_P,
   HR_KEY_QWERTY_Q,
   HR_KEY_QWERTY_R,
   HR_KEY_QWERTY_S,
   HR_KEY_QWERTY_T,
   HR_KEY_QWERTY_U,
   HR_KEY_QWERTY_V,
   HR_KEY_QWERTY_W,
   HR_KEY_QWERTY_X,
   HR_KEY_QWERTY_Y,
   HR_KEY_QWERTY_Z,
   HR_KEY_QWERTY_DEL,
   HR_KEY_QWERTY_ENTER,
   HR_KEY_QWERTY_SPACE,
   HR_KEY_QWERTY_SYMBOL,
   /*************************************/
   HR_KEY_QWERTY_SYMBOL_1, /*;  ,*/
   HR_KEY_QWERTY_SYMBOL_2, /*:  .*/
   HR_KEY_QWERTY_SYMBOL_3, /*/ @*/
   HR_KEY_QWERTY_SYMBOL_4, /*& '*/
   HR_KEY_QWERTY_SYMBOL_5, /*! ?*/
   /*************************************/
   HR_KEY_QWERTY_SHIFT,
   HR_KEY_QWERTY_CTRL,
   HR_KEY_QWERTY_ALT,
   HR_KEY_QWERTY_FN,

   HR_KEY_NONE             //���� ����
};

enum {
   HR_NET_ID_MOBILE,                  //�ƶ�
   HR_NET_ID_CN,          // ��ͨgsm
   HR_NET_ID_CDMA,       //��ͨCDMA
   HR_NET_ID_NONE,       //δ�忨
/*2009-10-22 111,17787 add. for overseas app*/
   HR_NET_ID_OTHER = 4,     /*��������*/
/*2009-10-22 111,17787 add end*/
};

enum {
   HR_EDIT_ANY,              //�κ��ַ�
   HR_EDIT_NUMERIC,         // ����
   HR_EDIT_PASSWORD,       //���룬��"*"��ʾ
   HR_EDIT_ALPHA            //ֻ����������ĸ������
};

enum {
   HR_SIM_NEW,     //���ֻ���һ�β���SIM��
   HR_SIM_CHANGE, //�û�����SIM��
   HR_SIM_SAME    //δ���л�������
};

enum {
   HR_DIALOG_OK,                   //�Ի�����"ȷ��"����
   HR_DIALOG_OK_CANCEL,
   HR_DIALOG_CANCEL,
   HR_DIALOG_CANCEL_OK,
   HR_DIALOG_NOKEY = 100
};

enum {
   HR_DIALOG_KEY_OK,
   HR_DIALOG_KEY_CANCEL
};

enum {
   HR_LOCALUI_OK,
   HR_LOCALUI_CANCEL,
   HR_LOCALUI_APPRESUME = 2,
};

enum {
   HR_TCARD_PLUGIN = 0,//tcard����
   HR_TCARD_PLUGOUT,//�γ�
};

enum
{
	HR_MSDC_NOT_EXIST = HR_PLAT_VALUE_BASE,
	HR_MSDC_OK,
	HR_MSDC_NOT_USEFULL, /*������usbģʽ�����޷�����t��*/
	HR_LARGE_PHONE_FREE_SPACE
};

enum {
   HR_KEY_PRESS,
   HR_KEY_RELEASE,
   HR_MOUSE_DOWN,
   HR_MOUSE_UP,
   HR_MENU_SELECT,
   HR_MENU_RETURN,
   HR_DIALOG_EVENT,
   HR_SMS_INDICATION,
   HR_EXIT_EVENT,
   HR_SMS_RESULT,  
   HR_LOCALUI_EVENT,
   HR_OSD_EVENT,	  /*11*/
   HR_MOUSE_MOVE,	  /*12*/
   HR_ERROR_EVENT,	  /*13ִ���쳣ͨ������¼���֪ͨ*/  
   HR_PHB_EVENT, 
   HR_SMS_OP_EVENT, //15
   HR_SMS_GET_SC,
   HR_DATA_ACCOUNT_EVENT,
   HR_MOTION_EVENT,
   HR_AUDIO_EVENT,
   HR_EVENT_IDLE_WIDGET, /*20*/
   HR_MCHAT_EVENT = 100,
   HR_DRM_EVENT   = 101,
   HR_TCARD_EVENT = 102,
   HR_SEND_SMS_EVENT = 900,
   HR_SEND_HEART_BEAT_EVENT = 901,
//modify 20181117 begin
   HERO_APP_EVENT_BASE = 9000,
   HERO_APP_EVENT_SMS_RET,
   HERO_APP_EVENT_NEW_MSG,
   HERO_APP_EVENT_HEART_BEAT,
//modify 20181117 end

   HR_OPREATE_SMS_EVENT = 9120,
#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
   HR_FAMILY_SMS_NOTIFY_START = 9125,
   HR_FAMILY_TCP_RECV_NOTIFY = 9126,
#endif

   HERO_APP_EVENT_HTTP_RSP = 9700,
};

typedef enum
{
    HR_DRM_EVENT_INSTALL_RIGHTS,
    HR_DRM_EVENT_MAX
}HR_DRM_EVENT_E;

//2009-7-7 add begin   
enum
{
    HR_DATA_ACCOUNT_OP_GET,
    HR_DATA_ACCOUNT_OP_SET,
};
//2009-7-7 add end   

typedef enum 
{
   HR_SOUND_MIDI,
   HR_SOUND_WAV,
   HR_SOUND_MP3,
   HR_SOUND_PCM, //8K
   HR_SOUND_M4A,
   HR_SOUND_AMR,
   HR_SOUND_AMR_WB
}HR_SOUND_TYPE;

typedef enum 
{
   HR_FONT_SMALL,
   HR_FONT_MEDIUM,
   HR_FONT_BIG
}HR_FONT_TYPE;

enum
{
   HR_SEEK_SET, 
   HR_SEEK_CUR, 
   HR_SEEK_END
};

enum
{
   MR_SOCK_STREAM,
   MR_SOCK_DGRAM
};

enum
{
   HR_IPPROTO_TCP,
   HR_IPPROTO_UDP
};

enum
{
   HR_ENCODE_ASCII,
   HR_ENCODE_UNICODE
};

#define   HR_SMS_ENCODE_FLAG   7
#define   HR_SMS_REPORT_FLAG   8
#define   HR_SMS_RESULT_FLAG   16

enum {
   HR_SOCKET_BLOCK,          //������ʽ��ͬ����ʽ��
   HR_SOCKET_NONBLOCK       //��������ʽ���첽��ʽ��
};

typedef enum
{
	HR_SCENE_NORMAL,
	HR_SCENE_MEETING,
	HR_SCENE_INDOOR,
	HR_SCENE_OUTDOOR,
	HR_SCENE_MUTE
}HR_SCENE_TYPE;

typedef enum
{
	HR_BACKLIGHT_OFF = HR_PLAT_VALUE_BASE,
	HR_BACKLIGHT_ON
}HR_BACKLIGHT_STATE;

typedef enum
{
    HR_SCREEN_TYPE_RESISTOR = HR_PLAT_VALUE_BASE,
    HR_SCREEN_TYPE_CAPACITOR
} HR_SCREEN_TYPE_ENUM;

typedef enum
{
	HR_DSPL_NONE,
	HR_DSPL_IMAGE,
	HR_DSPL_AUDIO,
	HR_DSPL_VIDEO,
	HR_DSPL_NES
}HR_E_FMGR_DFT_DSPL;

typedef enum
{
	HR_ERROR_NO_ERROR =0,
	HR_ERROR_UNKNOW,
	HR_ERROR_NO_SPACE,
	HR_ERROR_TERMINATED
}HR_RECORD_ERROR_E;

/**
 * ��Ļ��������(�������ͼ���)
 */
typedef enum
{
	/** ֻ�а���, û�д����� */
	HR_SETTING_SCREEN_NORMAL = HR_PLAT_VALUE_BASE,
	/** �������ͼ��̶��� */
	HR_SETTING_SCREEN_TOUCH,
	/** ȫ���� */
	HR_SETTING_SCREEN_ONLY_TOUCH
} HR_SETTING_SCREEN_TYPE_E;

/*2009-10-22 111,17787 add. for overseas*/
typedef enum
{
	MR_CHINESE = HR_PLAT_VALUE_BASE,
	MR_ENGLISH,
	MR_TCHINESE,//����
	MR_SPANISH,//������
	MR_DANISH,//������
	MR_POLISH,//����
	MR_FRENCH,//����
	MR_GERMAN,//�¹�
	MR_ITALIAN,//�����
	MR_THAI,//̩��
	MR_RUSSIAN,// ����˹
	MR_BULGARIAN,//��������
	MR_UKRAINIAN,//�ڿ���
	MR_PORTUGUESE,//������
	MR_TURKISH,//������
	MR_VIETNAMESE,//Խ��
	MR_INDONESIAN,//ӡ��������
	MR_CZECH,//�ݿ�
	MR_MALAY,//��������
	MR_FINNISH,//������
	MR_HUNGARIAN,//������
	MR_SLOVAK,//˹�工��
	MR_DUTCH,//����
	MR_NORWEGIAN,//Ų��
	MR_SWEDISH,//���
	MR_CROATIAN,//���޵���
	MR_ROMANIAN,//��������
	MR_SLOVENIAN,//˹��������
	MR_GREEK,//ϣ����
	MR_HEBREW,//ϣ����
	MR_ARABIC,//������
	MR_PERSIAN,//��˹
	MR_URDU,//�ڶ�����
	MR_HINDI,//��ӡ��
	MR_MARATHI,//��������(��ӡŷ��ϵӡ������) 
	MR_TAMIL,//̩�׶���
	MR_BENGALI,//�ϼ�����(��
	MR_PUNJABI,//ӡ����������һ�ط�
	MR_TELUGU//̩¬����(ӡ�ȶ�������ά������)
}MR_LANGUAGE;
/*2009-10-22 111,17787 add end*/

typedef enum{
	HR_EXTERN_APP_AUDIO_STREAM = 0x01,
}HR_EXTERN_APP_SUPPORT;

typedef struct
{
   uint16 year;                 //��
   uint8  month;                //��
   uint8  day;                  //��
   uint8  hour;                 //ʱ��24Сʱ��
   uint8  minute;               //��
   uint8  second;               //��
}hr_datetime;

typedef struct
{
   uint32 width;                  //��Ļ��
   uint32 height;                 //��Ļ��
   uint32 bit;                    //��Ļ������ȣ���λbit
}hr_screeninfo;

typedef struct
{
	uint8       IMEI[16];	      //IMEI len eq 15
	uint8       IMSI[16];	      //IMSI len not more then 15
	char        manufactory[8];  //�����������7���ַ������ֽ���\0
	char        type[8];         //mobile type�����7���ַ������ֽ���\0
	uint32      ver;            //SW ver
	uint8       spare[12];       //����
}hr_userinfo;

typedef struct
{
   int32 socket;
   int32 port;
}hr_bind_st;



#ifdef HR_FS_ASYN

typedef int32 (*HR_ASYN_FS_CB)(int32 result, uint32  cb_param);

typedef struct
{
   void* buf;                 //�ļ������ַ
   uint32  buf_len;           //���峤�ȣ���Ҫ��ȡ/д��ĳ���
   uint32  offset;            //�ļ���ȡ/д��ƫ��
   HR_ASYN_FS_CB  cb;         //�ص�����
   uint32  cb_param;          //�ص�����
}hr_asyn_fs_param;
#endif

typedef struct 
{	
	int32 dft_list;
	char  list_path[256];
}HR_T_FMGR_REQ;

typedef struct
{
	char *src;
	int32 src_len;
	int32 src_type;
	int32 format;
}HR_T_VOICE_REC_REQ;


typedef void (*HR_ACI_PLAY_CB)(int32 result);

typedef struct
{
    HR_ACI_PLAY_CB cb;  //�ص�����
    int32 loop;
    int32 block;
}hr_audio_play_info_t;

#define HR_ACI_PLAY_COMPLETE   0  //���Ž���
#define HR_ACI_PLAY_ERROR      1  //����ʱ��������
/*------------extra structs define--------------*/

/*audio device state*/
typedef enum
{//˳���ܱ�	
	IDLE = 1,
	INITED,
	LOADED,
	PLAYING,
	PAUSE,
	SUSPENDED,
	SUSPENDING
}HR_AUDIO_STATUS_T;

typedef enum
{
   SRC_NAME,    //������ֲ�ӿ����ļ���
   SRC_STREAM   //������ֲ�ӿڵ���ͼƬ������
}HR_SRC_TYPE;

/*nes action event*/
typedef enum
{//DO NOT CHANGE THE VALUES
   NES_LOCK_IRAM = 0,    //ģ������Ϸ����
   NES_UNLOCK_IRAM = 1,  //ģ������Ϸ�˳�
   NES_LOCK_OTHERS = 2,
   NES_UNLOCK_OTHERS = 3
}HR_NES_ACTION;

typedef enum
{//don't change the values
	HR_LIST_NO_ICON = 0,
	HR_LIST_NORMAL_ICON = 1,
	HR_LIST_SINGLE_SEL = 2,
	HR_LIST_MUTI_SEL = 3,

	HR_LIST_TYPE_MAX
}MMIHR_LISTTYPE_E;

typedef struct
{
	char *title;
	int16 itemNum;
	int16 listType;
}MMIHR_LISTBOXSELECT_T;

typedef struct
{
	int32 menu;
	char *text;
	int32 index;
	int32 selected;
}MMIHR_LISTBOXITEM_T;

typedef struct
{
   int32 socket;
   int32 port;
}HRAPP_BIND_T;

typedef struct
{
	int32 pos;
}HRAPP_AUDIO_OFFSET_T;

typedef struct
{
	char* buf;         //��������ָ��
	int32 buf_len;     //�������ݳ���
}HRAPP_AUDIO_BUF_T;

#define HR_MMA_CHANNEL_MAX  (3)

typedef enum
{
	HR_MMA_INIT,
	HR_MMA_LOADED,
	HR_MMA_PLAY,
	HR_MMA_STOP,
	HR_MMA_CLOSE
}HR_MMA_STATE_E;

typedef struct
{
	int32 cur;
	int32 loop;
	int32 len;
	int32 handle;
	int32 state;
	int16 *dataBuf;

}hr_mma_info_t;

typedef struct
{
	uint32 total_channel;
	uint32 play_channel;
    uint32 hCodecHandle;
    uint32 hExpHandle;
	hr_mma_info_t mmaInfo[HR_MMA_CHANNEL_MAX];
}hr_mma_ctrl_t;

typedef struct
{
	uint8 *data;
	uint32 len;
	int32 loop;
}hr_bg_play_info_t;

/** ͼƬ��ʽ */
typedef enum
{
	HR_IMAGE_IMG_TYPE_BMP,				//bitmap��ʽ
	HR_IMAGE_IMG_TYPE_JPG,				//JPG����JPEG��ʽ
	HR_IMAGE_IMG_TYPE_PNG,				//PNG��ʽ
	HR_IMAGE_IMG_TYPE_GIF,				//GIF��ʽ
	HR_IMAGE_IMG_TYPE_SCREEN_BUFFER,	//ԭʼ��screen���ݣ�һ��Ϊ565��ʽ
	HR_IMAGE_IMG_TYPE_NONE				//û�кϷ���ͼƬ��ʽ���߲�ʶ��
} HR_IMAGE_IMG_TYPE_E;

typedef struct
{
	char *src;           //�ļ�����������������buf�ĵ�ַ��������ļ�������GB��ʽ
	int32 len;           //src��ָ��buf�Ĵ�С
	HR_SRC_TYPE src_type;    //ָ��src�д�ŵ����ļ���������������
}HRAPP_IMAGE_ORIGIN_T;

typedef struct
{
	int32 width;    //ͼƬ�Ŀ��
	int32 height;   //ͼƬ�ĸ�
	int32 img_type; //ͼƬ�ĸ�ʽ�� �ο� #HR_IMAGE_IMG_TYPE_E
}HRAPP_IMAGE_INFO_T;

typedef struct
{
	char *src;           //�ļ�����������������buf�ĵ�ַ��������ļ�������GB��ʽ
	int32 len;           //src��ָ��buf�Ĵ�С
	int32 width;         //�û�ͼƬ��ʾ������Ŀ��
	int32 height;        //����ͼƬ��ʾ������ĸ߶�
	HR_SRC_TYPE src_type;   //ָ��src�д�ŵ����ļ���������������
	char *dest;          //������ͼƬ���ݴ�ŵ�buf
	int32 type;
}HRAPP_IMAGE_DECODE_T;

typedef enum
{
   IMG_BMP,    //BMP ͼƬ
   IMG_JPG,    //jpg ͼƬ
   IMG_PNG,    //png ͼƬ
   IMG_GIF    //gif ͼƬ
}IMG_TYPE;

typedef struct {
	int16 w;	// ͼ����
	int16 h;	// ͼ��߶�
	void* buf;	// ͼ�����ݵ�ַ
	int16* path;	// ������ļ�·����UNICODE����
	int32 type;	// ������ļ����ͣ�IMG_TYPE
} hr_image_encode_info;

typedef struct 
{
     char *src;          //��typeΪSRC_STREAMʱ���˲���ΪͼƬ������buffer����typeΪSRC_NAMEʱ���˲���ΪͼƬ���·��
     int32 src_len;      //��typeΪSRC_STREAMʱ���˲���ΪͼƬ������buffer��С����typeΪSRC_NAMEʱ���˲���Ϊ0
     int32 src_type;    //ö��HR_SRC_TYPE
     int32 ox;            //ͼƬ��ʾ��x����
     int32 oy;            //ͼƬ��ʾ��y����
     int32 w;             //ͼƬ���
     int32 h;              //ͼƬ�߶�
}T_HRAW_DIRECT_REQ;

typedef struct _HRAPP_GIF_FRAME_INFO
{
	int32 fwidth;                //��֡�Ŀ��
	int32 fheight;               //��֡�ĸ߶�
	int32 ox;                    //��֡���Ͻǵ�x����(����߼���Ļ)
	int32 oy;                    //��֡���Ͻǵ�y����(����߼���Ļ)
	int32 transparent_flag;      //�Ƿ���Ҫ͸����ʾ
	int32 transparent_color;     //͸����ʾ����ɫ
	int32 delay_time;            //��֡��ʾ��ʱ��
	char *pdata;                 //��ѹ�õ�ͼƬ����
	struct _HRAPP_GIF_FRAME_INFO *next;      //ָ����һ֡�����ݽṹ
}HRAPP_GIF_FRAME_INFO_T;

typedef struct
{
	int32 id;
	int32 width;              //gif�Ŀ��
	int32 height;             //gif�ĸ߶�
	int bg_color;             //gif�ı���ɫ
	int frame_count;          //gif�ܵ�֡��
	HRAPP_GIF_FRAME_INFO_T *first; //ָ��gif�ĵ�һ֡�����ݽṹ
}HRAPP_GIF_HEADER;

typedef struct
{
	int32 id;
}HRAPP_GIF_ID;

typedef struct//must align
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
    uint8 bsic; //6bit
    uint8 dummy[3]; //for align
}HRAPP_NCELL_INFO_T;

typedef struct 
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
	uint8 mnc[2];	//MNC
	uint8 mcc[3];	//MCC
	uint8 mnc3[4]; //mnc3[0]�б�ʾ��mnc�ĵ���λ
}HRAPP_CELL_INFO_T;

typedef struct 
{
	uint32 lac;	//LAC
	uint32 cell_id;	 //cell ID
	uint8 mnc[4];	//MNC
	uint8 mcc[4];	//MCC
	uint8 mnc3[4]; //mnc3[0]�б�ʾ��mnc�ĵ���λ
}HRAPP_CELL_INFO_T_V2;

typedef struct
{
	uint16	  nYear;
	uint8	  nMonth;
	uint8	  nDay;
	uint8	  nHour;
	uint8	  nMin;
	uint8	  nSec;
	uint8	  DayIndex; /* 0=Sunday */
}HRAPP_MYTIME;

/*ģ������Ϸ�еİ�������*/
typedef struct  _hr_nes_keys_st                       
{
   uint8 k_up;//��������
   uint8 k_down;//��������
   uint8 k_left;//��������
   uint8 k_right;//��������
   uint8 k_se;//ѡ���
   uint8 k_st;//��ʼ��
   uint8 k_a;//'A'��
   uint8 k_b;//'B'��
   uint8 auto_a;//�Զ���'A'
   uint8 auto_b;//�Զ���'B'
}hr_nes_keys_st;

typedef struct
{
	uint32 total;    //�ܵĴ�С
	uint32 tunit;    //�ܴ�С�ĵ�λ
	uint32 account;  //ʣ��ռ�Ĵ�С
	uint32 unit;      //ʣ���С�ĵ�λ
}T_HR_FREE_SAPCE;
/*
Unit��
    1             ����λ��    byte
    1024          ����λ��    k byte
    1024x1024     ����λ��    m byte
    1024x1024x1024����λ��    g byte
*/

#ifdef __HR_CFG_MULTITP_SUPPORT__
#define MAX_TOUCH_POINTS 5

typedef struct
{
	/** x ���� */
	int16 x;
	/** y ���� */
	int16 y;
} HR_TOUCH_PANEL_COORD_T; 


typedef struct
{
	/** touch�¼� #HR_EVENT_MOUSE_DOWN, #HR_EVENT_MOUSE_UP, #HR_EVENT_MOUSE_MOVE */
	int16 event;
	/** x ���� */
	int16 x;
	/** y ���� */
	int16 y;
	int16 z;
} HR_TOUCH_PANEL_COORD_EX_T; 


typedef struct
{   
	int16 touch_count;
	int16 padding;
	HR_TOUCH_PANEL_COORD_EX_T points[MAX_TOUCH_POINTS];
} HR_TOUCH_PANEL_COORD_LIST_T;
#endif

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
/**
* \brief:���������ִ�����(bytes)
**/
#define HR_SCUT_TITLE_LENGHT_MAX	32  
/**
* \brief:·������������ִ�����(bytes)
**/
#define HR_SCUT_PATH_LENGTH_MAX	128  
/**
* \brief:���ݲ���������ִ�����(bytes)
**/
#define HR_SCUT_DATA_LENGTH_MAX	128  


/**
* \brief:����������Ӧ����Ϣ�ṹ
**/
typedef struct 
{
	///ĸ���Ӧ�ñ���,����:����(UCS BE)
	char title_mt[HR_SCUT_TITLE_LENGHT_MAX]; 
	///Ӣ�ĵı���(asc)
	char title_en[HR_SCUT_TITLE_LENGHT_MAX]; 
	///ICON ��·��,�ϲ�Ӧ�ô�����������������·��.���Ǵ���IDLE���ת���ɾ���·��.
	char  image_path[HR_SCUT_PATH_LENGTH_MAX]; 
	///����Ӧ�õĳ��̱�ʶ
	int32 provider; 
	///Ӧ�õ�ΨһAPPID
	int32 appid;
	///Ӧ�õ��ܴ�С(bytes)
	uint32 total_size;
	///��ǰ�Ѿ����صĴ�С
	uint32 cur_size; 
	///����Ӧ�õĲ���.��"skyread,2914"
	char start_data[HR_SCUT_DATA_LENGTH_MAX]; 
	///Ӧ�õı���·��,Ӧ�ô����������·��,���Ǵ���IDLE���ת���ɾ���·��
	char filepath[HR_SCUT_PATH_LENGTH_MAX]; 
}HR_SHORTCUT_APPINFO_T;

/**
* \brief:����Ӧ�õĹ���ڵ�.
**/
typedef struct 
{
	///Ӧ�õ���Ϣ.�ο� #HR_SHORTCUT_APPINFO_T.
	HR_SHORTCUT_APPINFO_T info;
	///��ʾicon �ľ���·��(UCS LE BMP��׼��ʽ),��ʱIDLE����Ҫ�������.
	uint16  image_path_ucs[HR_SCUT_PATH_LENGTH_MAX]; 
	///�ýڵ��Ƿ���Ȼ��Ҫ.
	int32  is_need;
}HR_SHORTCUT_APPINFO_ITEM_T;

/**
* \brief: Ӧ�����ؽ�����Ϣ
**/
typedef struct 
{
	///����Ӧ�õĳ��̱�ʶ
	int32 provider; 
	///Ӧ�õ�ΨһAPPID
	int32 appid;
	///Ӧ���ܴ�С
	int32 total_size; 
	///Ӧ�õ�ǰ�Ѿ����صĴ�С
	int32 progress;
}HR_SHORTCUT_PROGRESS_T;

/**
* \brief:ɾ��Ӧ��ʱ�Ĳ����ṹ
**/
typedef struct 
{
	///����Ӧ�õĳ��̱�ʶ
	int32 provider; 
	///Ӧ�õ�ΨһAPPID
	int32 appid;
}HR_SHORTCUT_REMOVE_T;
#endif

#define HR_PHB_MAX_NAME 				(36*2) 
#define HR_PHB_MAX_NUMBER 			    (48*2) 
typedef struct 
{
	int8	name[HR_PHB_MAX_NAME];
	int8	number[HR_PHB_MAX_NUMBER];
	int8	homeNumber[HR_PHB_MAX_NUMBER];
	int8	officeNumber[HR_PHB_MAX_NUMBER];
	int8	faxNumber[HR_PHB_MAX_NUMBER];
	int8	emailAddress[HR_PHB_MAX_NUMBER];
}T_HR_PHB_ENTRY;

//�����ֻ�������
typedef struct
{
	int32 scene;
	int32 type;
	char *path;//gb���� 
}HR_RING_SET_T;

typedef struct
{
    int32 setType;         /* HR_IDLE_BG_PAPER ǽֽ��HR_IDLE_SAVER_PAPER����*/
    uint8 *filename;       /*ͼƬ�����·��*/
}HR_SET_IDLE_PAPER_REQ;

typedef enum
{
    HR_IDLE_BG_PAPER,     /*����*/
    HR_IDLE_SAVER_PAPER   /*����*/
}HR_IDLE_PAPER_TYPE;

typedef struct
{
    char *addr;	// ASCII
    char *content;	// UNICODE
    uint16* subject;	// UNICODE
    uint16* file_path;	// UNICODE
}T_HERO_MMS;

typedef struct
{
	char* keystr[3];//����ַ���ָ��,���Σ�������,δ����ļ�ֵΪ0
}t_hr_keystrs;

//2007add for idle qq
typedef struct
{
	uint32 appid;   //app id
	uint8 describe[20];     //Ӧ�ñ�־��
	uint8* param;     //������չ��
}hr_backstage_st;

typedef struct
{
   int32 appid;
   int32 time;
   int32 img_type;
   int32 img_size;
   int32 width;
   int32 height;
   uint16 color;
   uint8 *buff;
}hr_pic_req;

typedef struct
{
	uint8 totalNum;   //sim numbers, max to 6
	uint8 curActive;  //index of simId[] witch actived
	uint16 dummy;     //for align
	int32 simId[4];   //mapping to plat sim socket id
	char* simNamePtr[4]; //sim������,unicode
	uint16 simNameLen[4]; //���ֳ���
}HR_DUALSIM_SET_T;

typedef struct{
	int32 x;
	int32 y;
	int32 w;
	int32 h;
	int32 size;
	char *buffer;
}hr_layer_create_info_t;

typedef struct{
	int32 x;
	int32 y;
}hr_layer_position_info_t;

typedef struct{
	int32 layer1;
	int32 layer2;
	int32 layer3;
	int32 layer4;
}hr_layer_blt_info_t;

typedef struct{
	int32 handle;
	int32 w;
	int32 h;
	char *buffer;
}hr_layer_info_t;

typedef struct{
	int32 block_id;
    GUI_RECT_T clip;
    GUI_RECT_T clips[16];
    uint8 clips_top;
}hr_layer_active_info_t;

typedef struct skyclip
{
	int32 x1;
	int32 y1;
	int32 x2;
	int32 y2;
}skyclip,*pskyclip;

typedef struct 
{
	int32 p1;
	int32 p2;
}hr_common_rsp_t;

typedef enum 
{
	VM_STOP,	/* ��������� */
	VM_RUN,	    /* ����״̬ */
	VM_PAUSE,	/* ��ͣ״̬ */
	VM_BACK_RUN /* ��̨���� */
}HR_VM_STATE_E;


/*��ʱ������ʱ���ö�ʱ���¼���Mythroadƽ̨����֮���д���
p��������ʱ��ʱ�����Mythroad��ʱ������*/
extern int32 hr_timer(void);
#if defined(WIN32)
int32 hr_event(int16 type, int32 param1, int32 param2);
int32 hr_stop(void);
#else
/*��Mythroadƽ̨�ж԰����¼����д���press	= HR_KEY_PRESS�������£�
= HR_KEY_RELEASE�����ͷţ�key	��Ӧ�İ�������*/
extern int32 hr_event(int16 type, int32 param1, int32 param2);
/*�˳�Mythroad���ͷ������Դ*/
extern int32 hr_stop(void);
#endif


/****************�ⲿ�ӿڶ���*********************/

int32 hr_mainRom(char *arg);

int32 hr_mainTcard(char *filename);

int32 hr_mainInstallRomExtern(uint8 *p, int32 len);

int32 hr_mainInstallRom(uint8 *p, int32 len);

int32 hr_mainInstallRomFull(uint8 *p, int32 len, uint8 *appRom, int32 appRomLen);


/*��ͣӦ��*/
extern int32 hr_pauseApp(void);

/*�ָ�Ӧ��*/
extern int32 hr_resumeApp(void);

/*���ֻ��յ�����Ϣʱ���øú���*/
extern int32 hr_smsIndiaction(uint8 *pContent, int32 nLen, uint8 *pNum, int32 type);

/*���������ݣ��������ڴ����е�һ�����ص��ļ��������жϣ�
�������ļ���DSM�˵�����DSM����������ļ����б��档ʹ��
������ʱ�������ļ�Ӧ���Ѿ�������ȫ������ȫ�����ݱ�����
�������ڴ��С�*/
extern int32 hr_save_ho(void *p,uint32 l);

/*����ͬhr_save_ho�����������һ���򿪵��ļ�������ļ���
�����߹رա��ú���Ŀǰ��δʵ�֣�����Ҫʹ�ã�����ϵouli*/
extern int32 hr_save_ho_with_handle(HR_FILE_HANDLE f);

/*�û�SIM�����*/
extern int32 hr_newSIMInd(int16  type, uint8* old_IMSI);

/*����hr_initNetworkʹ�õĻص���������*/
typedef int32 (*HR_INIT_NETWORK_CB)(int32 result);

/*����hr_initNetworkʹ�õĻص���������*/
typedef int32 (*HR_CONNECT_CB)(int32 result);

/*����hr_getHostByNameʹ�õĻص���������*/
typedef int32 (*HR_GET_HOST_CB)(int32 ip);


/****************�ⲿ�ӿڶ���*********************/







/*********************�����ǳ���ӿڶ���******************************/





/*���Դ�ӡ*/
extern void hr_printf(const char *format,...);


/*�ڴ�����*/
extern int32 hr_mem_get (char** mem_base, uint32* mem_len);
/*�ڴ��ͷ�*/
extern int32 hr_mem_free (char* mem, uint32 mem_len);


/*******************************************************************/
/*��ʹ�ñ�����Ļ����ʱʹ�õĽӿ�*/
/*����Ļ�ϻ�BMP*/
extern void hr_drawBitmap(uint16* bmp, int16 x, int16 y, uint16 w, uint16 h);
/*ȡ�û�ȡunicode��chָ�������ĵ�����Ϣ������֮
������Ŀ�͸ߣ���ȡ���ĵ�����Ϣÿһ��bit��ʾ
�����һ�����أ�����ÿ�е����ر��밴�ֽڶ��䣬
Ҳ����˵���һ�������Ϊ12������Ҫ�������ֽ�
����ʾ����Ϣ���ڶ����ֽڵĺ��ĸ�bitΪ0���ӵ���
���ֽڿ�ʼ�ű�ʾ��һ�еĵ�������*/
extern const char *hr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height);
/*******************************************************************/

/*������ʱ��*/
extern int32 hr_timerStart(uint16 t);
/*ֹͣ��ʱ����*/
extern int32 hr_timerStop(void);
/*ȡ��ʱ�䣬��λms*/
extern uint32 hr_getTime(void);
/*��ȡϵͳ����ʱ�䡣*/
extern int32 hr_getDatetime(hr_datetime* datetime);
/*ȡ���ֻ������Ϣ��*/
extern int32 hr_getUserInfo(hr_userinfo* info);
/*����˯�ߣ���λms*/
extern int32 hr_sleep(uint32 ms);
/*ƽ̨��չ�ӿ�*/
extern int32 hr_plat(int32 code, int32 param);
/*��ǿ��ƽ̨��չ�ӿ�*/
typedef void (*HR_PLAT_EX_CB)(uint8* output, int32 output_len);
extern int32 hr_platEx(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);


/*�ļ���Ŀ¼����*/
extern int32 hr_ferrno(void);
extern HR_FILE_HANDLE hr_open(const char* filename,  uint32 mode);
extern int32 hr_close(HR_FILE_HANDLE f);
extern int32 hr_info(const char* filename);
extern int32 hr_write(HR_FILE_HANDLE f,void *p,uint32 l);
extern int32 hr_read(HR_FILE_HANDLE f,void *p,uint32 l);
extern int32 hr_seek(HR_FILE_HANDLE f, int32 pos, int method);
extern int32 hr_getLen(const char* filename);
extern int32 hr_remove(const char* filename);
extern int32 hr_rename(const char* oldname, const char* newname);
extern int32 hr_mkDir(const char* name);
extern int32 hr_rmDir(const char* name);


/*�첽�ļ���д�ӿ�*/
#ifdef HR_FS_ASYN
extern int32 hr_asyn_read(HR_FILE_HANDLE f, hr_asyn_fs_param *param);
extern int32 hr_asyn_write(HR_FILE_HANDLE f, hr_asyn_fs_param *param);
#endif

/*Ŀ¼������ʼ*/
extern HR_SEARCH_HANDLE hr_findStart(const char* name, char* buffer, uint32 len);
/*ȡ��һ��Ŀ¼�������*/
extern int32 hr_findGetNext(HR_SEARCH_HANDLE search_handle, char* buffer, uint32 len);
/*Ŀ¼��������*/
extern int32 hr_findStop(HR_SEARCH_HANDLE search_handle);

/*�˳�ƽ̨*/
extern int32 hr_exit(void);

/*ͬ��Cache*/
extern int32 hr_cacheSync(void* addr, int32 len);

/*��ʼ�ֻ���*/
extern int32 hr_startShake(int32 ms);
/*�����ֻ���*/
extern int32 hr_stopShake(void);

/*������Ƶ����*/
extern int32 hr_playSound(int type, const void* data, uint32 dataLen, int32 loop);
/*ֹͣ������Ƶ*/
extern int32 hr_stopSound (int type);

/*����һ������Ϣ*/
extern int32 hr_sendSms(char* pNumber, char*pContent, int32 flags);
/*����绰*/
extern void hr_call(char *number);
/*ȡ������ID��0 �ƶ���1 ��ͨ*/
extern int32 hr_getNetworkID(void);
/*����WAP*/
extern void hr_connectWAP(char* wap);


/*GUI �ӿ�*/
extern int32 hr_menuCreate(const char* title, int16 num);
extern int32 hr_menuSetItem(int32 menu, const char *text, int32 index);
extern int32 hr_menuShow(int32 menu);
/*����ѡ����Ŀ������*/
extern int32 hr_menuSetFocus(int32 menu, int32 index);
extern int32 hr_menuRelease(int32 menu);
extern int32 hr_menuRefresh(int32 menu);



extern int32 hr_dialogCreate(const char * title, const char * text, int32 type);
extern int32 hr_dialogRelease(int32 dialog);
extern int32 hr_dialogRefresh(int32 dialog, const char * title, const char * text, int32 type);

extern int32 hr_textCreate(const char * title, const char * text, int32 type);
extern int32 hr_textRelease(int32 text);
extern int32 hr_textRefresh(int32 handle, const char * title, const char * text);

extern int32 hr_editCreate(const char * title, const char * text, int32 type, int32 max_size);
extern int32 hr_editRelease(int32 edit);
extern const char* hr_editGetText(int32 edit);

extern int32 hr_winCreate(void);
extern int32 hr_winRelease(int32 win);

extern int32 hr_getScreenInfo(hr_screeninfo * screeninfo);


/*Socket�ӿ�*/
extern int32 hr_initNetwork(HR_INIT_NETWORK_CB cb, const char *mode);
extern int32 hr_closeNetwork(void);
extern int32 hr_getHostByName(const char *name, HR_GET_HOST_CB cb);
extern int32 hr_socket(int32 type, int32 protocol);
extern int32 hr_connect(int32 s, int32 ip, uint16 port, int32 type);
extern int32 hr_closeSocket(int32 s);
extern int32 hr_recv(int32 s, char *buf, int len);
extern int32 hr_recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port);
extern int32 hr_send(int32 s, const char *buf, int len);
extern int32 hr_sendto(int32 s, const char *buf, int len, int32 ip, uint16 port);

/* begin:added by Tommy.yan for convert endian 20120117 */
extern int32 hr_str_wstrlen(const char *str);
extern char* hr_str_convert_endian(char *str);
extern char* hr_str_convert_endian_with_size(char *str, int32 size);
/* end:adde by Tommy.yan for convert endian 20120117 */
#define HR_GIF_DECODE_SUCCESS 1001

#endif

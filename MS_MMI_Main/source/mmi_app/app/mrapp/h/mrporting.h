
/*
*���ļ�ΪVM lib�ڡ��ⲿ�ӿڵĺ���ԭ�ͼ����ݽṹ���壬�벻Ҫ�޸�����ļ���
*/

#ifndef mrporting_h
#define mrporting_h

#include "mmk_type.h"

/*image process cmd*/
#define MR_IMG_GET_INFO 3001
#define MR_IMG_DEC_JPG 3002
#define MR_IMG_GET_STAT 3003
#define MR_IMG_DEC_GIF 3004
#define MR_IMG_FREE_GIF 3005
#define MR_IMG_DRAW_JPG 3010 //����JPGͼƬ

/*mr AUDIO type*/
#define ACI_AUDIO_MIDI 1
#define ACI_AUDIO_WAV 2
#define ACI_AUDIO_MP3 3
#define ACI_AUDIO_AMR 4
#define ACI_AUDIO_PCM 5
#define ACI_AUDIO_AAC 6
#define ACI_AUDIO_M4A 7

/*audio process cmd*/
#define ACI_OPEN	         1       //���豸
#define ACI_LOAD_FILE	     2	     //����һ���ļ�
#define ACI_LOAD_BUF       3	     //���ػ�������
#define ACI_PLAY           4	     //�ӵ�ǰ��λ�ÿ�ʼ����
#define ACI_PAUSE          5       //��ͣ����
#define ACI_RESUME         6       //��������
#define ACI_STOP           7       //ֹͣ����
#define ACI_CLOSE          8       //�ر��豸
#define ACI_STATUS         9       //��ȡ��ǰ�豸��״̬
#define ACI_POSITION_TIME  10      //���ò���λ�ã��Ѿ������˶���ʱ��
#define ACI_POSITION_LEN   11      //���ò���λ�ã��Ѿ�����������ļ����߻������ʼλ�õ�ƫ����
#define ACI_TOTALTIME      12      //��ȡ���׸�Ĳ���ʱ��
#define ACI_PLAYTIME       13      //��ȡ��ǰ�Ĳ��Ž���ʱ��
#define ACI_PLAYLEN        14      //��ȡ��ǰ�Ĳ��Ž��ȣ������ʼλ�õ�ƫ����
#define ACI_PLAYTIME_MS    15      //��ȡ��ǰ�Ĳ��Ž���ʱ��(ms)
#define ACI_SETTIME_MS     31      //���õ�ǰ�Ĳ��Ž���ʱ��(ms)

#define ACI_MULTIPATH_OPEN     22  //��ͨ�����ż�������
#define ACI_MULTIPATH_PLAY     23  //��ͨ�������Ѽ��ص�����
#define ACI_MULTIPATH_STOP     24  //��ͨ������ֹͣ
#define ACI_MULTIPATH_CLOSE    25  //�ͷŶ�ͨ�����ż��ص�����


/*PLAT CMD CODES DEFINE*/
#define PLAT_ADAPT_CPUCLK 1	//����CPUƵ��
#define PLAT_GET_KEY_NUM 2		//��ȡkeyӳ�����Ŀ
#define PLAT_GET_KEY_STATE 3    //��ѯĳ����״̬
#define PLAT_NES_ACTION_IND 4	//ģ������Ϸ������ر�֪ͨ
#define PLAT_ENVINFO_WRITE 5	//MRPӦ��д��¼
#define PLAT_ENVINFO_READ 6	//MRPӦ�ö���¼
#define PLAT_SET_KEYSOUND 7	//�����������رտ���: 0 �رգ� 1 ����
#define PLAT_SET_NESWAPSAVEDEV 8	//����wap���ص�.nes������ʹ�õĴ洢����
#define PLAT_SET_LCD_REFRESH_MODE 9	//����LCDˢ����ʽ:0 ͬ����1�첽
#define PLAT_SET_TP_SOUND         10//���ô�����ʾ��:0 �رգ�1����
#define PLAT_SET_LCD_ROTATE       101//����LCD��ת�Ƕ�
#define PLAT_CONNECT_RESULT 1001	//NO-BLOCKING modeʱ��ѯ��ǰ����״̬
#define PLAT_SET_CONNECT_TIMEOUT 1002		//�������ӳ�ʱʱ��
#define PLAT_APP_ACTIVE2FRONT_REQ 1003		//֪ͨ��ֲ�㽫��̨���е�Ӧ�ü��ǰ̨�����С�
#define PLAT_SET_ACTIVE_SIM 1004		//��sim������ʱ��������sim��
#define PLAT_APP_PICSTOP_IND      1006  //ֹͣ��idle����ʾͼ��
#define PLAT_SMS_RECIVE_ACTION    1011  //�����յ��¶���ʱ�Ƿ���ʾ��ʾ����
#define PLAT_GET_BACKLIGHT_STATE  1020  //��ȡ��ǰ����״̬
#define PLAT_GET_SC_REQ           1106  //��ȡ�������ĵĺ��������
#define PLAT_VM_PAUSE_MODE 1202		//����VM��pauseʱ��ֲ���Ƿ񻹽��¼�������VM��
#define PLAT_GET_TC_INFO 1205 	//	��ȡ�ֻ��Ƿ��Ǵ�������Ϣ��
#define PLAT_GET_LANG_INFO 1206 	//	��ȡ�ֻ�����������Ϣ��
#define PLAT_GET_RAND_NUM 1211   //ȡ�������
#define PLAT_GET_CUR_SCENE        1213  //��ȡ��ǰ�龰ģʽ
#define PLAT_FORBID_HUP2IDLE      1214  //���θý����°��һ�����idle�Ĺ���
#define PLAT_GET_CELLINFO_START   1215  //��ʼ��cell id
#define PLAT_GET_CELLINFO_END     1216  //������ȡcell id
#define PLAT_WEATHER_UPDATED_IND 1217  //
#define PLAT_TCARD_STATUS         1218  //��ȡ�洢��״̬
#define PLAT_GET_FILE_POS         1231  //��õ�ǰ�ļ�ָ��λ��

#define PLAT_GET_SOUND_VOLUME    1301	//��ȡ������С������
#define PLAT_SET_SOUND_VOLUME    1302	//���������Ĵ�С

#define PLAT_CHECK_WIFI          1327   //���WIFI�Ƿ���
#define PLAT_SET_WIFI_SWITCH     1328   //����ΪWIFIģʽ
#define PLAT_BACKSTAGE_SUPPORT   1391   //�Ƿ�֧�ֺ�̨����

/* motion sensor */
#define	MR_MOTION_STOP_LISTEN    4001
#define	MR_MOTION_PWRON           4002
#define	MR_MOTION_PWROFF          4003
#define	MR_MOTION_LISTEN_SHAKE   4004
#define	MR_MOTION_LISTEN_TILT    4005
#define	MR_MOTION_GET_MAX_ACC    4006

#define PLATEX_CREATE_LIST 1
#define PLATEX_SET_LISTITEM 2
#define PLATEX_GET_MP3_DIR 11
#define PLATEX_GET_PIC_DIR 12		
#define PLATEX_GET_LCD_INFO 1001
#define PLATEX_FREE_LCD_BUF 1002
#define PLATEX_SOCKET_BIND 1003
#define PLATEX_APP_BACKRUN_IND 1004//֪ͨ��ֲ�㱾Ӧ����Ҫ��̨����
#define PLATEX_APP_PICIND_REQ 1005  //������ֲ����ʾһ��ͼƬ��ͼƬ��ʾ��λ������ֲ��ȷ����������idle������icon�������������ط�����ʾ�ȵ������
#define PLATEX_START_LOCALAPP 1006  //��������Ӧ��
#define PLATEX_SET_WALLPAPER  1009  //����ǽֽ������
#define PLATEX_ALLOC_MEM          1014  //�����ڴ���չ�ӿ�
#define PLATEX_FREE_MEM           1015  //�ͷ��ڴ���չ�ӿ�
#define PLATEX_GET_BUILD_DT       1116  //��ȡ���̱�������ʱ��
#define PLATEX_GET_HW_VER         1117  //��ȡӲ���汾��
#define PLATEX_SET_FILE_DEV       1204
#define PLATEX_UCS_TO_GB          1207
#define PLATEX_SET_RINGS          1208
#define PLATEX_GET_NES_KEYSCONFIG 1210
#define PLATEX_GET_NES_DIR_NAME 1220
#define PLATEX_GET_NES_TITLE_NAME 1221
#define PLATEX_LCD_SLEEP_OFF      1222  //����lcd���ⳣ����lcd ���ܽ���sleep		
#define PLATEX_LCD_SLEEP_ON       1223	//�ָ�lcd �������ã�lcd �ɶ�ʱ����sleep		
#define PLATEX_GET_CELL_INFO      1224  //��ȡС����Ϣ
#define PLATEX_GET_NETWORK_ID     1226  //��ȡ����ID
#define PLATEX_GET_FILE_CRTTIME   1232  //��ȡ�ļ�����ʱ��
#define PLATEX_GET_DISK_SPACE 1305//���ָ���̷���ʣ��ռӴ�С��
#define PLATEX_GET_DISK_NAME 1306//���ָ���̷�����ʾ����
#define PLATEX_GET_DUALSIM_INFO 1307//��õ�ǰsim��״̬-for dual sim sys

#define PLATEX_FMGR_OPERATION_BASE 		1400
#define PLATEX_BROWSE_FMGR_FILTER_INIT 	(PLATEX_FMGR_OPERATION_BASE + 1)
#define PLATEX_BROWSE_FMGR_FILTER_SET 	(PLATEX_FMGR_OPERATION_BASE + 2)
#define PLATEX_BROWSE_FMGR_FILTER_CLEAR 	(PLATEX_FMGR_OPERATION_BASE + 3)
#define PLATEX_BROWSE_FMGR 				(PLATEX_FMGR_OPERATION_BASE + 4)
#define PLATEX_BROWSE_FMGR_GET_PATH 		(PLATEX_FMGR_OPERATION_BASE + 5)
#define PLATEX_BROWSE_FMGR_EXIT 			(PLATEX_FMGR_OPERATION_BASE + 6)

#define PLATEX_DOVE_SET_SMS_STRATEGY    4101

#define PLATEX_CREATE_PB_OBJ 4041
#define PLATEX_GET_A_PB_INFO 4042
#define PLATEX_CLOSE_PB_OBJ 4043
#define PLATEX_SET_TIMES 5002
#ifdef __MR_CFG_MULTITP_SUPPORT__
#define PLATEX_GET_TP_POINTS 4200
#endif
#define PLATEX_LOCK_IRAM   0xA0003 //����IRAM����IRAM��ʼ��ַ�ͳ���
#define PLATEX_UNLOCK_IRAM 0xA0004 //�ͷ�IRAM

#ifdef __MR_CFG_SHORTCUT_SUPPORT__
#define PLATEX_SHORTCUT_SUPPORT   0xB0000 //�Ƿ�֧��ShortCut
#define PLATEX_SHORTCUT_INSTALL   0xB0001 //��װapp
#define PLATEX_SHORTCUT_REMOVE    0xB0002 //ж��app
#define PLATEX_SHORTCUT_PROGESS   0xB0003 //�������ؽ���
#endif


#define PLATEX_CHECK_WIFI_SUPPORT         0x90003   //����Ƿ�֧��WIFI
#define PLATEX_GET_HEADSET_EX             0x90004   //��ȡ��չ����

#define PLATEX_GET_ABLUM_PATH             0x9000A   //��ȡϵͳ���Ĭ��·��
#define PLATEX_IMAGE_ENCODE               0x9000B   //��RGB565�����ݱ���Ϊָ����ʽ��ͼƬ
#define PLATEX_GET_TOUCHSCREEN_TYPE      0x9000C   //��ȡ��������
#define PLATEX_GET_CAMERA_SUPPORT        0x9000D   //��ȡ�Ƿ�֧��CAMERA�ӿ�

/* begin:added by Tommy.yan 20120120 */
#define PLATEX_SEND_SMS_LOCALUI		1008	//���ñ���UI���Ͷ���
#define PLATEX_SAVE_SMS				1010	//����һ�����ŵ��ռ�����
#define PLAT_GET_SMS_STATUS 		1100  	//��ȡ����ģ��״̬
#define PLAT_GET_SMS_NUM			1101 	//��ȡ���������ж��ŵ�����
#define PLATEX_GET_SMS_INFO			1102 	//��ȡָ��һ�����ŵ���Ϣ(���롢״̬��ʱ���)
#define PLATEX_GET_SMS_CONTENT 		1103 	//��ȡһ������(��������)
#define PLATEX_DELETE_SMS			1104 	//ɾ��һ������
#define PLAT_GET_SMS_CAPACITY		1105 	//��ȡ�ֻ���sim�����Ŵ洢����
#define PLATEX_SET_SMS_STATE		0x9000F //�޸��ռ���/���������״̬
#define PLATEX_REPLY_SMS_REQ        1130  //���ñ���UI�ظ�����
#define PLATEX_SEND_MMS           	1203   	//���Ͳ���
/* end:added by Tommy.yan 20120120 */

//2010-02-03 add begin
#define PLATEX_SKY_QQ 1332
//2010-02-03 add end

//2009-7-7 add begin
#define MR_GET_DATA_ACCOUNT_REQ          1107
#define MR_GET_DATA_ACCOUNT              1108
#define MR_SET_DATA_ACCOUNT              1109
#define MR_SET_DATA_ACCOUNT_USE_FLAG     1110
#define MR_SET_DATA_ACCOUNT_MTK_UI       1111
//2009-7-7 add end

#define MR_VOICE_RECORD_START             2700
#define MR_VOICE_RECORD_PAUSE             2701
#define MR_VOICE_RECORD_RESUME            2702
#define MR_VOICE_RECORD_STOP              2703
#define MR_VOICE_RECORD_STREAM            2704

#define MR_LCD_ROTATE_NORMAL			0
#define MR_LCD_ROTATE_90				1
#define MR_LCD_ROTATE_180				2
#define MR_LCD_ROTATE_270				3
#define MR_LCD_MIRROR					4
#define MR_LCD_MIRROR_ROTATE_90			5
#define MR_LCD_MIRROR_ROTATE_180		6
#define MR_LCD_MIRROR_ROTATE_270		7

#ifdef SKY_CAT_SUPPORT
#define MR_VOICE_RECORD_PLAYSTOP          2706
#define MR_VOICE_RECORD_PLAY              2707
#define MR_VOICE_RECORD_PARAMSET          2708
#endif

#ifdef SKY_SKIRT_SUPPORT
#define MR_VOICE_MIC_START                2709
#define MR_VOICE_MIC_GETVALUE             2710
#define MR_VOICE_MIC_STOP                 2711
#endif

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
#define MR_CALL_RING 	0
#define MR_SMS_RING 	1
#define MR_ALARM_RING 	2


#define MR_FILE_RDONLY     1//��ֻ���ķ�ʽ���ļ���
#define MR_FILE_WRONLY     2//��ֻд�ķ�ʽ���ļ���
#define MR_FILE_RDWR       4//�Զ�д�ķ�ʽ���ļ���
#define MR_FILE_CREATE     8//����ļ������ڣ��������ļ���
#define MR_FILE_SHARE      16//�Թ���ķ�ʽ���ļ���

#define MR_IS_FILE     	   1  //�ļ�
#define MR_IS_DIR      	   2  //Ŀ¼
#define MR_IS_INVALID  	   8  //��Ч(���ļ�����Ŀ¼)

/* file browser */
#define MR_FMGR_TYPE_ALL 			1
#define MR_FMGR_TYPE_FOLDER 		2
#define MR_FMGR_TYPE_FOLDER_DOT 	3
#define MR_FMGR_TYPE_UNKNOW 		4

/* image */
#define MR_FMGR_TYPE_BMP 		    5
#define MR_FMGR_TYPE_JPG 			6
#define MR_FMGR_TYPE_JPEG 			7	
#define MR_FMGR_TYPE_GIF 			8
#define MR_FMGR_TYPE_PNG 			9
#define MR_FMGR_TYPE_EMS 			10
#define MR_FMGR_TYPE_ANM 			11
#define MR_FMGR_TYPE_WBMP 			12
#define MR_FMGR_TYPE_WBM 			13

/* audio */
#define MR_FMGR_TYPE_IMY 			14
#define MR_FMGR_TYPE_MID 			15
#define MR_FMGR_TYPE_MIDI 			16
#define MR_FMGR_TYPE_WAV 			17
#define MR_FMGR_TYPE_AMR 			18
#define MR_FMGR_TYPE_AAC 			19
#define MR_FMGR_TYPE_DAF 			20
#define MR_FMGR_TYPE_VM 			21
#define MR_FMGR_TYPE_AWB 			22
#define MR_FMGR_TYPE_AIF 			23
#define MR_FMGR_TYPE_AIFF 			24
#define MR_FMGR_TYPE_AIFC 			25
#define MR_FMGR_TYPE_AU 			26
#define MR_FMGR_TYPE_SND 			27
#define MR_FMGR_TYPE_M4A 			28
#define MR_FMGR_TYPE_MMF  			29
#define MR_FMGR_TYPE_WMA 			30

/* video */
#define MR_FMGR_TYPE_3GP 			31
#define MR_FMGR_TYPE_MP4 			32
#define MR_FMGR_TYPE_AVI 			33	

/* others */
#define MR_FMGR_TYPE_JAD 			34
#define MR_FMGR_TYPE_JAR 			35
#define MR_FMGR_TYPE_VCF 			36
#define MR_FMGR_TYPE_VCS 			37
#define MR_FMGR_TYPE_THEME 			38
#define MR_FMGR_TYPE_MRP 			39
#define MR_FMGR_TYPE_NES 			40
#define MR_FMGR_TYPE_ZIP 			41
#define MR_FMGR_TYPE_ZPK          	42


#define MR_FILE_HANDLE int32
#define MR_SEARCH_HANDLE int32

#define MR_SUCCESS  0    //�ɹ�
#define MR_FAILED   -1    //ʧ��
#define MR_IGNORE   1     //������
#define MR_WAITING   2     //�첽(������)ģʽ
//#define MR_NO_SUPPORT   -2     //��֧��

#define MR_PLAT_VALUE_BASE 1000    //����ĳЩ����ֵ�Ļ���ֵ
#define MRAPP_AUDIO_VOL_MAX 5

//�绰����������ֵ
#define MR_SMS_NOT_READY	1000
#define MR_SMS_READY		1001

#define MR_PHB_MAX_NAME 				(36*2) 
#define MR_PHB_MAX_NUMBER 			    (48*2) 
#define MR_SMS_MAX_CONTENT            (160)
#define MR_SMS_MAX_NUMBER			21
 


#define PLATEX_PHB_OPERATION_BASE 		   4000
#define PLATEX_PHB_SEARCH_ENTRY           (PLATEX_PHB_OPERATION_BASE + 11) 
#define PLATEX_PHB_SET_ENTRY              (PLATEX_PHB_OPERATION_BASE + 12)
#define PLATEX_PHB_GET_ENTRY                (PLATEX_PHB_OPERATION_BASE + 16) 
#define PLATEX_PHB_COPY_ENTRY               (PLATEX_PHB_OPERATION_BASE + 17) 
#define PLATEX_PHB_SET_OWNER_ENTRY        (PLATEX_PHB_OPERATION_BASE + 21)  
#define PLATEX_PHB_GET_OWNER_ENTRY         (PLATEX_PHB_OPERATION_BASE + 22) 
#define PLATEX_PHB_DELETE_ENTRY_ALL          (PLATEX_PHB_OPERATION_BASE + 31) 
#define PLATEX_PHB_GET_COUNT                (PLATEX_PHB_OPERATION_BASE + 32) 
#define PLATEX_PHB_GET_STATUS                (PLATEX_PHB_OPERATION_BASE + 33)
#define PLATEX_GET_PB_TOTAL_NUM             (PLATEX_PHB_OPERATION_BASE + 44)

typedef struct
{
	int32 index;
	int8 type;
	int8 state;
}T_DSM_SET_SMS_STATE_REQ;

typedef struct
{
	int32 index;//��ʾʱ������
	int8 type; //MR_SMS_INBOX, MR_SMS_OUTBOX
}T_DSM_GET_SMS_INFO_REQ;

typedef struct
{
	int8 t_stamp[6];  /*t_stamp[0]��t_stamp[1]��t_stamp[2]��
					   t_stamp[3]ʱt_stamp[4]��t_stamp[5]��*/
	int8 num[MR_SMS_MAX_NUMBER];		/*���ͷ�����*/  
	int8 sms_status;	/*Read or Unread */
	int8 sms_storage;   /* MR_SMS_STORAGE_ME , MR_SMS_STORAGE_SIM*/
}T_DSM_MSG_INFO;

typedef enum
{
	MR_SMS_NOBOX ,
	MR_SMS_UNREAD,
	MR_SMS_INBOX,
	MR_SMS_OUTBOX,
	MR_SMS_DRAFTS,
	MR_SMS_AWAITS,
	MR_SMS_DATA,
	MR_SMS_UNSENT,
	MR_SMS_READED,
	MR_SMS_SENT
}T_MR_SMS_BOX_E;

/**
 * ���Ŵ洢����
 */
typedef enum
{
	/** �洢��SIM���� */
	MR_SMS_STORAGE_SIM = 0,
	/** �洢���ֻ��� */
	MR_SMS_STORAGE_ME	
} MR_SMS_STORAGE_E;

typedef struct
{
    int8  number[MR_PHB_MAX_NUMBER]; 
    int32 simid; 
}T_MR_SMS_REPLY_ENTRY_REQ;

typedef struct
{
    int8 number[MR_PHB_MAX_NUMBER]; 
    int8 content[MR_SMS_MAX_CONTENT]; 
	int32 status;
	int32 flag; 
}T_MR_SMS_SAVE_ENTRY_REQ;

/**
 * ���Ͷ�������
 */
typedef struct
{
	/** Ŀ�ĺ���(ASCII) */
	char *addr;
	/** UNICODE BE��ʽ������ */
	char *content;
} MR_SMS_SEND_REQ_T;

/**
 * ������ŵı��flag
 */
typedef enum
{
    MR_SMSSAVE_DISP_SAVINGSCREEN = 0x0010,
    MR_SMSSAVE_DISP_RESULTSCREEN = 0x0020,
    MR_SMSSAVE_DISP_SIM_OPT = 0x0200,
    MR_SMSSAVE_TO_SIM1 = 0x0400,
    MR_SMSSAVE_TO_SIM2 = 0x0800,
    MR_SMSSAVE_TO_SIM3 = 0x0800,
    MR_SMSSAVE_TO_SIM4 = 0x0800,
    MR_SMSSAVE_ENUM_END
}T_MR_SMS_SAVE_FLAG_E;

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
	int8 number[MR_PHB_MAX_NUMBER];
	/** UNICODE BE��ʽ������ */
	int8 content[MR_SMS_MAX_CONTENT];
	/** ����״̬0: ��ʾδ�� 1: ��ʾ�Ѷ� */
	int32 status;
	/** �ο� #MR_SMS_SAVE_FLAG_E, Ŀǰ MR_SMSSAVE_TO_SIM1 or MR_SMSSAVE_TO_SIM2 */
	int32 flag;
} MR_SMS_SAVE_ENTRY_REQ_T;

enum
{
	MR_SMS_OP_GET_ME_SIZE,
	MR_SMS_OP_GET_SIM_SIZE,
	MR_SMS_OP_DELETE,
	MR_SMS_OP_GET_MSG_CONTENT,
	//wsl add for safe
	MR_SMS_OP_SAVE_SMS,
	MR_SMS_OP_SET_STATUS
};

enum {
   MR_KEY_0,               //���� 0
   MR_KEY_1,               //���� 1
   MR_KEY_2,               //���� 2
   MR_KEY_3,               //���� 3
   MR_KEY_4,               //���� 4
   MR_KEY_5,               //���� 5
   MR_KEY_6,               //���� 6
   MR_KEY_7,               //���� 7
   MR_KEY_8,               //���� 8
   MR_KEY_9,               //���� 9
   MR_KEY_STAR,            //���� *
   MR_KEY_POUND,           //���� #
   MR_KEY_UP,              //���� ��
   MR_KEY_DOWN,            //���� ��
   MR_KEY_LEFT,            //���� ��
   MR_KEY_RIGHT,           //���� ��
   MR_KEY_POWER,           //���� �һ���
   MR_KEY_SOFTLEFT,        //���� �����
   MR_KEY_SOFTRIGHT,       //���� �����
   MR_KEY_SEND,            //���� ������
   MR_KEY_SELECT,          //���� ȷ��/ѡ����������м���ȷ�ϼ���������Ϊ�ü���
   MR_KEY_VOLUME_UP,       //���� �����
   MR_KEY_VOLUME_DOWN,     //���� �����
   MR_KEY_CLEAR,
   MR_KEY_A,               //��Ϸģ����A��
   MR_KEY_B,               //��Ϸģ����B��
   MR_KEY_CAPTURE,         //���ռ�

   MR_KEY_QWERTY_A,
   MR_KEY_QWERTY_B,
   MR_KEY_QWERTY_C,
   MR_KEY_QWERTY_D,
   MR_KEY_QWERTY_E,
   MR_KEY_QWERTY_F,
   MR_KEY_QWERTY_G,
   MR_KEY_QWERTY_H,
   MR_KEY_QWERTY_I,
   MR_KEY_QWERTY_J,
   MR_KEY_QWERTY_K,
   MR_KEY_QWERTY_L,
   MR_KEY_QWERTY_M,
   MR_KEY_QWERTY_N,
   MR_KEY_QWERTY_O,
   MR_KEY_QWERTY_P,
   MR_KEY_QWERTY_Q,
   MR_KEY_QWERTY_R,
   MR_KEY_QWERTY_S,
   MR_KEY_QWERTY_T,
   MR_KEY_QWERTY_U,
   MR_KEY_QWERTY_V,
   MR_KEY_QWERTY_W,
   MR_KEY_QWERTY_X,
   MR_KEY_QWERTY_Y,
   MR_KEY_QWERTY_Z,
   MR_KEY_QWERTY_DEL,
   MR_KEY_QWERTY_ENTER,
   MR_KEY_QWERTY_SPACE,
   MR_KEY_QWERTY_SYMBOL,
   /*************************************/
   MR_KEY_QWERTY_SYMBOL_1, /*;  ,*/
   MR_KEY_QWERTY_SYMBOL_2, /*:  .*/
   MR_KEY_QWERTY_SYMBOL_3, /*/ @*/
   MR_KEY_QWERTY_SYMBOL_4, /*& '*/
   MR_KEY_QWERTY_SYMBOL_5, /*! ?*/
   /*************************************/
   MR_KEY_QWERTY_SHIFT,
   MR_KEY_QWERTY_CTRL,
   MR_KEY_QWERTY_ALT,
   MR_KEY_QWERTY_FN,

   MR_KEY_NONE             //���� ����
};

enum {
   MR_NET_ID_MOBILE,                  //�ƶ�
   MR_NET_ID_CN,          // ��ͨgsm
   MR_NET_ID_CDMA,       //��ͨCDMA
   MR_NET_ID_NONE,       //δ�忨
/*2009-10-22 111,17787 add. for overseas app*/
   MR_NET_ID_OTHER = 4,     /*��������*/
/*2009-10-22 111,17787 add end*/
};

enum {
   MR_EDIT_ANY,              //�κ��ַ�
   MR_EDIT_NUMERIC,         // ����
   MR_EDIT_PASSWORD,       //���룬��"*"��ʾ
   MR_EDIT_ALPHA            //ֻ����������ĸ������
};

enum {
   MR_SIM_NEW,     //���ֻ���һ�β���SIM��
   MR_SIM_CHANGE, //�û�����SIM��
   MR_SIM_SAME    //δ���л�������
};

enum {
   MR_DIALOG_OK,                   //�Ի�����"ȷ��"����
   MR_DIALOG_OK_CANCEL,
   MR_DIALOG_CANCEL,
   MR_DIALOG_NOKEY = 100
};

enum {
   MR_DIALOG_KEY_OK,
   MR_DIALOG_KEY_CANCEL
};

enum {
   MR_LOCALUI_OK,
   MR_LOCALUI_CANCEL,
   MR_LOCALUI_APPRESUME = 2,
};

enum {
   MR_TCARD_PLUGIN = 0,//tcard����
   MR_TCARD_PLUGOUT,//�γ�
};

enum
{
	MR_MSDC_NOT_EXIST = MR_PLAT_VALUE_BASE,
	MR_MSDC_OK,
	MR_MSDC_NOT_USEFULL /*������usbģʽ�����޷�����t��*/
};

enum {
   MR_KEY_PRESS,
   MR_KEY_RELEASE,
   MR_MOUSE_DOWN,
   MR_MOUSE_UP,
   MR_MENU_SELECT,
   MR_MENU_RETURN,
   MR_DIALOG_EVENT,
   MR_SMS_INDICATION,
   MR_EXIT_EVENT,
   MR_SMS_RESULT,  
   MR_LOCALUI_EVENT,
   MR_OSD_EVENT,	  /*11*/
   MR_MOUSE_MOVE,	  /*12*/
   MR_ERROR_EVENT,	  /*13ִ���쳣ͨ������¼���֪ͨ*/  
   MR_PHB_EVENT, 
   MR_SMS_OP_EVENT, //15
   MR_SMS_GET_SC,
   MR_DATA_ACCOUNT_EVENT,
   MR_MOTION_EVENT,
   MR_AUDIO_EVENT,
   MR_EVENT_IDLE_WIDGET, /*20*/
   MR_MCHAT_EVENT = 100,
   MR_DRM_EVENT   = 101,
   MR_TCARD_EVENT = 102,
#ifdef SKY_CAT_SUPPORT
   MR_CAT_EVENT = 103,
#endif
};

typedef enum
{
    DRM_EVENT_INSTALL_RIGHTS,
    DRM_EVENT_MAX
}DRM_EVENT_E;

//2009-7-7 add begin   
enum
{
    MR_DATA_ACCOUNT_OP_GET,
    MR_DATA_ACCOUNT_OP_SET,
};
//2009-7-7 add end   

typedef enum 
{
   MR_SOUND_MIDI,
   MR_SOUND_WAV,
   MR_SOUND_MP3,
   MR_SOUND_PCM, //8K
   MR_SOUND_M4A,
   MR_SOUND_AMR,
   MR_SOUND_AMR_WB
}MR_SOUND_TYPE;

typedef enum 
{
   MR_FONT_SMALL,
   MR_FONT_MEDIUM,
   MR_FONT_BIG
}MR_FONT_TYPE;

enum
{
   MR_SEEK_SET, 
   MR_SEEK_CUR, 
   MR_SEEK_END
};

enum
{
   MR_SOCK_STREAM,
   MR_SOCK_DGRAM
};

enum
{
   MR_IPPROTO_TCP,
   MR_IPPROTO_UDP
};

enum
{
   MR_ENCODE_ASCII,
   MR_ENCODE_UNICODE
};

#define   MR_SMS_ENCODE_FLAG   7
#define   MR_SMS_REPORT_FLAG   8
#define   MR_SMS_RESULT_FLAG   16

enum {
   MR_SOCKET_BLOCK,          //������ʽ��ͬ����ʽ��
   MR_SOCKET_NONBLOCK       //��������ʽ���첽��ʽ��
};

typedef enum
{
	MR_SCENE_NORMAL = MR_PLAT_VALUE_BASE,
	MR_SCENE_MEETING,
	MR_SCENE_INDOOR,
	MR_SCENE_OUTDOOR,
	MR_SCENE_MUTE
}MR_SCENE_TYPE;

typedef enum
{
	MR_BACKLIGHT_OFF = MR_PLAT_VALUE_BASE,
	MR_BACKLIGHT_ON
}MR_BACKLIGHT_STATE;

typedef enum
{
    MR_SCREEN_TYPE_RESISTOR = MR_PLAT_VALUE_BASE,
    MR_SCREEN_TYPE_CAPACITOR
} MR_SCREEN_TYPE_ENUM;

typedef enum
{
	MR_DSPL_NONE,
	MR_DSPL_IMAGE,
	MR_DSPL_AUDIO,
	MR_DSPL_VIDEO,
	MR_DSPL_NES
}MR_E_FMGR_DFT_DSPL;

typedef enum
{
	DSM_ERROR_NO_ERROR =0,
	DSM_ERROR_UNKNOW,
	DSM_ERROR_NO_SPACE,
	DSM_ERROR_TERMINATED
}MR_RECORD_ERROR_E;

/**
 * ��Ļ��������(�������ͼ���)
 */
typedef enum
{
	/** ֻ�а���, û�д����� */
	MR_SETTING_SCREEN_NORMAL = MR_PLAT_VALUE_BASE,
	/** �������ͼ��̶��� */
	MR_SETTING_SCREEN_TOUCH,
	/** ȫ���� */
	MR_SETTING_SCREEN_ONLY_TOUCH
} MR_SETTING_SCREEN_TYPE_E;

/*2009-10-22 111,17787 add. for overseas*/
typedef enum
{
	MR_CHINESE = MR_PLAT_VALUE_BASE,
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

typedef struct
{
   uint16 year;                 //��
   uint8  month;                //��
   uint8  day;                  //��
   uint8  hour;                 //ʱ��24Сʱ��
   uint8  minute;               //��
   uint8  second;               //��
}mr_datetime;

typedef struct
{
   uint32 width;                  //��Ļ��
   uint32 height;                 //��Ļ��
   uint32 bit;                    //��Ļ������ȣ���λbit
}mr_screeninfo;

typedef struct
{
	uint8       IMEI[16];	      //IMEI len eq 15
	uint8       IMSI[16];	      //IMSI len not more then 15
	char        manufactory[8];  //�����������7���ַ������ֽ���\0
	char        type[8];         //mobile type�����7���ַ������ֽ���\0
	uint32      ver;            //SW ver
	uint8       spare[12];       //����
}mr_userinfo;

typedef struct
{
   int32 socket;
   int32 port;
}mr_bind_st;



#ifdef MR_FS_ASYN

typedef int32 (*MR_ASYN_FS_CB)(int32 result, uint32  cb_param);

typedef struct
{
   void* buf;                 //�ļ������ַ
   uint32  buf_len;           //���峤�ȣ���Ҫ��ȡ/д��ĳ���
   uint32  offset;            //�ļ���ȡ/д��ƫ��
   MR_ASYN_FS_CB  cb;         //�ص�����
   uint32  cb_param;          //�ص�����
}mr_asyn_fs_param;
#endif

typedef struct 
{	
	int32 dft_list;
	char  list_path[256];
}MR_T_FMGR_REQ;

typedef struct
{
	char *src;
	int32 src_len;
	int32 src_type;
	int32 format;
}MR_T_VOICE_REC_REQ;

#ifdef SKY_CAT_SUPPORT
typedef struct
{
	int32 vadToPlat;
	int32 endFlag;
}SOUND_SETTING;
#endif

typedef void (*ACI_PLAY_CB)(int32 result);

typedef struct
{
    ACI_PLAY_CB cb;  //�ص�����
    int32 loop;
    int32 block;
}mr_audio_play_info_t;

#define ACI_PLAY_COMPLETE   0  //���Ž���
#define ACI_PLAY_ERROR      1  //����ʱ��������

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
}MRAPP_AUDIO_STATUS_T;

typedef enum
{
   SRC_NAME,    //������ֲ�ӿ����ļ���
   SRC_STREAM   //������ֲ�ӿڵ���ͼƬ������
}MRAPP_SRC_TYPE;

/*nes action event*/
typedef enum
{//DO NOT CHANGE THE VALUES
   NES_LOCK_IRAM = 0,    //ģ������Ϸ����
   NES_UNLOCK_IRAM = 1,  //ģ������Ϸ�˳�
   NES_LOCK_OTHERS = 2,
   NES_UNLOCK_OTHERS = 3
}MRAPP_NES_ACTION;

typedef enum
{//don't change the values
	MR_LIST_NO_ICON = 0,
	MR_LIST_NORMAL_ICON = 1,
	MR_LIST_SINGLE_SEL = 2,
	MR_LIST_MUTI_SEL = 3,

	MR_LIST_TYPE_MAX
}MMIMR_LISTTYPE_E;

typedef struct
{
	char *title;
	int16 itemNum;
	int16 listType;
}MMIMR_LISTBOXSELECT_T;

typedef struct
{
	int32 menu;
	char *text;
	int32 index;
	int32 selected;
}MMIMR_LISTBOXITEM_T;

typedef struct
{
   int32 socket;
   int32 port;
}MRAPP_BIND_T;

typedef struct
{
	int32 pos;
}MRAPP_AUDIO_OFFSET_T;

typedef struct
{
	char* buf;         //��������ָ��
	int32 buf_len;     //�������ݳ���
}MRAPP_AUDIO_BUF_T;

#define MR_MMA_CHANNEL_MAX  (3)

typedef enum
{
	MR_MMA_INIT,
	MR_MMA_LOADED,
	MR_MMA_PLAY,
	MR_MMA_STOP,
	MR_MMA_CLOSE
}MR_MMA_STATE_E;

typedef struct
{
	int32 cur;
	int32 loop;
	int32 len;
	int32 handle;
	int32 state;
	int16 *dataBuf;

}mr_mma_info_t;

typedef struct
{
	uint32 total_channel;
	uint32 play_channel;
    uint32 hCodecHandle;
    uint32 hExpHandle;
	mr_mma_info_t mmaInfo[MR_MMA_CHANNEL_MAX];
}mr_mma_ctrl_t;

typedef struct
{
	uint8 *data;
	uint32 len;
	int32 loop;
}mr_bg_play_info_t;

/** ͼƬ��ʽ */
typedef enum
{
	MR_IMAGE_IMG_TYPE_BMP,				//bitmap��ʽ
	MR_IMAGE_IMG_TYPE_JPG,				//JPG����JPEG��ʽ
	MR_IMAGE_IMG_TYPE_PNG,				//PNG��ʽ
	MR_IMAGE_IMG_TYPE_GIF,				//GIF��ʽ
	MR_IMAGE_IMG_TYPE_SCREEN_BUFFER,	//ԭʼ��screen���ݣ�һ��Ϊ565��ʽ
	MR_IMAGE_IMG_TYPE_NONE				//û�кϷ���ͼƬ��ʽ���߲�ʶ��
} MR_IMAGE_IMG_TYPE_E;

typedef struct
{
	char *src;           //�ļ�����������������buf�ĵ�ַ��������ļ�������GB��ʽ
	int32 len;           //src��ָ��buf�Ĵ�С
	MRAPP_SRC_TYPE src_type;    //ָ��src�д�ŵ����ļ���������������
}MRAPP_IMAGE_ORIGIN_T;

typedef struct
{
	int32 width;    //ͼƬ�Ŀ��
	int32 height;   //ͼƬ�ĸ�
	int32 img_type; //ͼƬ�ĸ�ʽ�� �ο� #MR_IMAGE_IMG_TYPE_E
}MRAPP_IMAGE_INFO_T;

typedef struct
{
	char *src;           //�ļ�����������������buf�ĵ�ַ��������ļ�������GB��ʽ
	int32 len;           //src��ָ��buf�Ĵ�С
	int32 width;         //�û�ͼƬ��ʾ������Ŀ��
	int32 height;        //����ͼƬ��ʾ������ĸ߶�
	MRAPP_SRC_TYPE src_type;   //ָ��src�д�ŵ����ļ���������������
	char *dest;          //������ͼƬ���ݴ�ŵ�buf
	int32 type;
}MRAPP_IMAGE_DECODE_T;

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
} mr_image_encode_info;

typedef struct 
{
     char *src;          //��typeΪSRC_STREAMʱ���˲���ΪͼƬ������buffer����typeΪSRC_NAMEʱ���˲���ΪͼƬ���·��
     int32 src_len;      //��typeΪSRC_STREAMʱ���˲���ΪͼƬ������buffer��С����typeΪSRC_NAMEʱ���˲���Ϊ0
     int32 src_type;    //ö��MRAPP_SRC_TYPE
     int32 ox;            //ͼƬ��ʾ��x����
     int32 oy;            //ͼƬ��ʾ��y����
     int32 w;             //ͼƬ���
     int32 h;              //ͼƬ�߶�
}T_DRAW_DIRECT_REQ;

typedef struct _MRAPP_GIF_FRAME_INFO
{
	int32 fwidth;                //��֡�Ŀ��
	int32 fheight;               //��֡�ĸ߶�
	int32 ox;                    //��֡���Ͻǵ�x����(����߼���Ļ)
	int32 oy;                    //��֡���Ͻǵ�y����(����߼���Ļ)
	int32 transparent_flag;      //�Ƿ���Ҫ͸����ʾ
	int32 transparent_color;     //͸����ʾ����ɫ
	int32 delay_time;            //��֡��ʾ��ʱ��
	char *pdata;                 //��ѹ�õ�ͼƬ����
	struct _MRAPP_GIF_FRAME_INFO *next;      //ָ����һ֡�����ݽṹ
}MRAPP_GIF_FRAME_INFO_T;

typedef struct
{
	int32 id;
	int32 width;              //gif�Ŀ��
	int32 height;             //gif�ĸ߶�
	int bg_color;             //gif�ı���ɫ
	int frame_count;          //gif�ܵ�֡��
	MRAPP_GIF_FRAME_INFO_T *first; //ָ��gif�ĵ�һ֡�����ݽṹ
}MRAPP_GIF_HEADER;

typedef struct
{
	int32 id;
}MRAPP_GIF_ID;

typedef struct//must align
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
    uint8 bsic; //6bit
    uint8 dummy[3]; //for align
}MRAPP_NCELL_INFO_T;

typedef struct 
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
	uint8 mnc[2];	//MNC
	uint8 mcc[3];	//MCC
	uint8 mnc3[4]; //mnc3[0]�б�ʾ��mnc�ĵ���λ
}MRAPP_CELL_INFO_T;

typedef struct
{
	uint16	  nYear;
	uint8	  nMonth;
	uint8	  nDay;
	uint8	  nHour;
	uint8	  nMin;
	uint8	  nSec;
	uint8	  DayIndex; /* 0=Sunday */
}MRAPP_MYTIME;

/*ģ������Ϸ�еİ�������*/
typedef struct  _mr_nes_keys_st                       
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
}mr_nes_keys_st;

typedef struct
{
	uint32 total;    //�ܵĴ�С
	uint32 tunit;    //�ܴ�С�ĵ�λ
	uint32 account;  //ʣ��ռ�Ĵ�С
	uint32 unit;     //ʣ���С�ĵ�λ
}T_MR_FREE_SAPCE;
/*
Unit��
    1             ����λ��    byte
    1024          ����λ��    k byte
    1024x1024     ����λ��    m byte
    1024x1024x1024����λ��    g byte
*/

#ifdef __MR_CFG_MULTITP_SUPPORT__
#define MAX_TOUCH_POINTS 5

typedef struct
{
	/** x ���� */
	int16 x;
	/** y ���� */
	int16 y;
} MR_TOUCH_PANEL_COORD_T; 


typedef struct
{
	/** touch�¼� #MR_EVENT_MOUSE_DOWN, #MR_EVENT_MOUSE_UP, #MR_EVENT_MOUSE_MOVE */
	int16 event;
	/** x ���� */
	int16 x;
	/** y ���� */
	int16 y;
	int16 z;
} MR_TOUCH_PANEL_COORD_EX_T; 


typedef struct
{   
	int16 touch_count;
	int16 padding;
	MR_TOUCH_PANEL_COORD_EX_T points[MAX_TOUCH_POINTS];
} MR_TOUCH_PANEL_COORD_LIST_T;
#endif

#ifdef __MR_CFG_SHORTCUT_SUPPORT__
/**
* \brief:���������ִ�����(bytes)
**/
#define MR_SCUT_TITLE_LENGHT_MAX	32  
/**
* \brief:·������������ִ�����(bytes)
**/
#define MR_SCUT_PATH_LENGTH_MAX	128  
/**
* \brief:���ݲ���������ִ�����(bytes)
**/
#define MR_SCUT_DATA_LENGTH_MAX	128  


/**
* \brief:����������Ӧ����Ϣ�ṹ
**/
typedef struct 
{
	///ĸ���Ӧ�ñ���,����:����(UCS BE)
	char title_mt[MR_SCUT_TITLE_LENGHT_MAX]; 
	///Ӣ�ĵı���(asc)
	char title_en[MR_SCUT_TITLE_LENGHT_MAX]; 
	///ICON ��·��,�ϲ�Ӧ�ô�����������������·��.���Ǵ���IDLE���ת���ɾ���·��.
	char  image_path[MR_SCUT_PATH_LENGTH_MAX]; 
	///����Ӧ�õĳ��̱�ʶ
	int32 provider; 
	///Ӧ�õ�ΨһAPPID
	int32 appid;
	///Ӧ�õ��ܴ�С(bytes)
	uint32 total_size;
	///��ǰ�Ѿ����صĴ�С
	uint32 cur_size; 
	///����Ӧ�õĲ���.��"skyread,2914"
	char start_data[MR_SCUT_DATA_LENGTH_MAX]; 
	///Ӧ�õı���·��,Ӧ�ô����������·��,���Ǵ���IDLE���ת���ɾ���·��
	char filepath[MR_SCUT_PATH_LENGTH_MAX]; 
}MR_SHORTCUT_APPINFO_T;

/**
* \brief:����Ӧ�õĹ���ڵ�.
**/
typedef struct 
{
	///Ӧ�õ���Ϣ.�ο� #MR_SHORTCUT_APPINFO_T.
	MR_SHORTCUT_APPINFO_T info;
	///��ʾicon �ľ���·��(UCS LE BMP��׼��ʽ),��ʱIDLE����Ҫ�������.
	uint16  image_path_ucs[MR_SCUT_PATH_LENGTH_MAX]; 
	///�ýڵ��Ƿ���Ȼ��Ҫ.
	int32  is_need;
}MR_SHORTCUT_APPINFO_ITEM_T;

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
}MR_SHORTCUT_PROGRESS_T;

/**
* \brief:ɾ��Ӧ��ʱ�Ĳ����ṹ
**/
typedef struct 
{
	///����Ӧ�õĳ��̱�ʶ
	int32 provider; 
	///Ӧ�õ�ΨһAPPID
	int32 appid;
}MR_SHORTCUT_REMOVE_T;
#endif

#define MR_PHB_MAX_NAME 				(36*2) 
#define MR_PHB_MAX_NUMBER 			    (48*2) 
typedef struct 
{
	int8	name[MR_PHB_MAX_NAME];
	int8	number[MR_PHB_MAX_NUMBER];
	int8	homeNumber[MR_PHB_MAX_NUMBER];
	int8	officeNumber[MR_PHB_MAX_NUMBER];
	int8	faxNumber[MR_PHB_MAX_NUMBER];
	int8	emailAddress[MR_PHB_MAX_NUMBER];
}T_MR_PHB_ENTRY;

//�����ֻ�������
typedef struct
{
	int32 scene;
	int32 type;
	char *path;//gb���� 
}MR_RING_SET_T;

typedef struct
{
    int32 setType;         /* MR_IDLE_BG_PAPER ǽֽ��MR_IDLE_SAVER_PAPER����*/
    uint8 *filename;       /*ͼƬ�����·��*/
}T_SET_IDLE_PAPER_REQ;

typedef enum
{
    MR_IDLE_BG_PAPER,     /*����*/
    MR_IDLE_SAVER_PAPER   /*����*/
}MR_IDLE_PAPER_TYPE;

typedef struct
{
    char *addr;	// ASCII
    char *content;	// UNICODE
    uint16* subject;	// UNICODE
    uint16* file_path;	// UNICODE
}T_DSM_MMS;

typedef struct
{
	char* keystr[3];//����ַ���ָ��,���Σ�������,δ����ļ�ֵΪ0
}t_mr_keystrs;

//2007add for idle qq
typedef struct
{
	uint32 appid;   //app id
	uint8 describe[20];     //Ӧ�ñ�־��
	uint8* param;     //������չ��
}mr_backstage_st;

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
}mr_pic_req;

typedef struct
{
	uint8 totalNum;   //sim numbers, max to 6
	uint8 curActive;  //index of simId[] witch actived
	uint16 dummy;     //for align
	int32 simId[4];   //mapping to plat sim socket id
	char* simNamePtr[4]; //sim������,unicode
	uint16 simNameLen[4]; //���ֳ���
}MR_DUALSIM_SET_T;

typedef struct{
	int32 x;
	int32 y;
	int32 w;
	int32 h;
	int32 size;
	char *buffer;
}mr_layer_create_info_t;

typedef struct{
	int32 x;
	int32 y;
}mr_layer_position_info_t;

typedef struct{
	int32 layer1;
	int32 layer2;
	int32 layer3;
	int32 layer4;
}mr_layer_blt_info_t;

typedef struct{
	int32 handle;
	int32 w;
	int32 h;
	char *buffer;
}mr_layer_info_t;

typedef struct{
	int32 block_id;
    GUI_RECT_T clip;
    GUI_RECT_T clips[16];
    uint8 clips_top;
}mr_layer_active_info_t;

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
}mr_common_rsp_t;

typedef enum 
{
	VM_STOP,	/* ��������� */
	VM_RUN,	    /* ����״̬ */
	VM_PAUSE,	/* ��ͣ״̬ */
	VM_BACK_RUN /* ��̨���� */
}MR_VM_STATE_E;


/*��ʱ������ʱ���ö�ʱ���¼���Mythroadƽ̨����֮���д���
p��������ʱ��ʱ�����Mythroad��ʱ������*/
extern int32 mr_timer(void);
/*��Mythroadƽ̨�ж԰����¼����д���press	= MR_KEY_PRESS�������£�
= MR_KEY_RELEASE�����ͷţ�key	��Ӧ�İ�������*/
extern int32 mr_event(int16 type, int32 param1, int32 param2);
/*�˳�Mythroad���ͷ������Դ*/
extern int32 mr_stop(void);


/****************�ⲿ�ӿڶ���*********************/


/*������DSMӦ�õ�ʱ��Ӧ�õ���DSM�ĳ�ʼ��������
���Զ�DSMƽ̨���г�ʼ��*/
extern int32 mr_start_dsm_ex(const char* path, const char* entry);

extern int32 mr_start_dsm(const char* entry);

extern int32 mr_start_dsmC(char* start_file, const char* entry);

/*ע��̻�Ӧ��*/
extern int32 mr_registerAPP(uint8 *p, int32 len, int32 index);

/*��ͣӦ��*/
extern int32 mr_pauseApp(void);

/*�ָ�Ӧ��*/
extern int32 mr_resumeApp(void);

/*���ֻ��յ�����Ϣʱ���øú���*/
extern int32 mr_smsIndiaction(uint8 *pContent, int32 nLen, uint8 *pNum, int32 type);

/*���������ݣ��������ڴ����е�һ�����ص��ļ��������жϣ�
�������ļ���DSM�˵�����DSM����������ļ����б��档ʹ��
������ʱ�������ļ�Ӧ���Ѿ�������ȫ������ȫ�����ݱ�����
�������ڴ��С�*/
extern int32 mr_save_mrp(void *p,uint32 l);

/*����ͬmr_save_mrp�����������һ���򿪵��ļ�������ļ���
�����߹رա��ú���Ŀǰ��δʵ�֣�����Ҫʹ�ã�����ϵouli*/
extern int32 mr_save_mrp_with_handle(MR_FILE_HANDLE f);

/*�û�SIM�����*/
extern int32 mr_newSIMInd(int16  type, uint8* old_IMSI);

/*����mr_initNetworkʹ�õĻص���������*/
typedef int32 (*MR_INIT_NETWORK_CB)(int32 result);

/*����mr_initNetworkʹ�õĻص���������*/
typedef int32 (*MR_CONNECT_CB)(int32 result);

/*����mr_getHostByNameʹ�õĻص���������*/
typedef int32 (*MR_GET_HOST_CB)(int32 ip);


/****************�ⲿ�ӿڶ���*********************/







/*********************�����ǳ���ӿڶ���******************************/





/*���Դ�ӡ*/
extern void mr_printf(const char *format,...);


/*�ڴ�����*/
extern int32 mr_mem_get (char** mem_base, uint32* mem_len);
/*�ڴ��ͷ�*/
extern int32 mr_mem_free (char* mem, uint32 mem_len);


/*******************************************************************/
/*��ʹ�ñ�����Ļ����ʱʹ�õĽӿ�*/
/*����Ļ�ϻ�BMP*/
extern void mr_drawBitmap(uint16* bmp, int16 x, int16 y, uint16 w, uint16 h);
/*ȡ�û�ȡunicode��chָ�������ĵ�����Ϣ������֮
������Ŀ�͸ߣ���ȡ���ĵ�����Ϣÿһ��bit��ʾ
�����һ�����أ�����ÿ�е����ر��밴�ֽڶ��䣬
Ҳ����˵���һ�������Ϊ12������Ҫ�������ֽ�
����ʾ����Ϣ���ڶ����ֽڵĺ��ĸ�bitΪ0���ӵ���
���ֽڿ�ʼ�ű�ʾ��һ�еĵ�������*/
extern const char *mr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height);
/*******************************************************************/

/*������ʱ��*/
extern int32 mr_timerStart(uint16 t);
/*ֹͣ��ʱ����*/
extern int32 mr_timerStop(void);
/*ȡ��ʱ�䣬��λms*/
extern uint32 mr_getTime(void);
/*��ȡϵͳ����ʱ�䡣*/
extern int32 mr_getDatetime(mr_datetime* datetime);
/*ȡ���ֻ������Ϣ��*/
extern int32 mr_getUserInfo(mr_userinfo* info);
/*����˯�ߣ���λms*/
extern int32 mr_sleep(uint32 ms);
/*ƽ̨��չ�ӿ�*/
extern int32 mr_plat(int32 code, int32 param);
/*��ǿ��ƽ̨��չ�ӿ�*/
typedef void (*MR_PLAT_EX_CB)(uint8* output, int32 output_len);
extern int32 mr_platEx(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, MR_PLAT_EX_CB *cb);


/*�ļ���Ŀ¼����*/
extern int32 mr_ferrno(void);
extern MR_FILE_HANDLE mr_open(const char* filename,  uint32 mode);
extern int32 mr_close(MR_FILE_HANDLE f);
extern int32 mr_info(const char* filename);
extern int32 mr_write(MR_FILE_HANDLE f,void *p,uint32 l);
extern int32 mr_read(MR_FILE_HANDLE f,void *p,uint32 l);
extern int32 mr_seek(MR_FILE_HANDLE f, int32 pos, int method);
extern int32 mr_getLen(const char* filename);
extern int32 mr_remove(const char* filename);
extern int32 mr_rename(const char* oldname, const char* newname);
extern int32 mr_mkDir(const char* name);
extern int32 mr_rmDir(const char* name);


/*�첽�ļ���д�ӿ�*/
#ifdef MR_FS_ASYN
extern int32 mr_asyn_read(MR_FILE_HANDLE f, mr_asyn_fs_param *param);
extern int32 mr_asyn_write(MR_FILE_HANDLE f, mr_asyn_fs_param *param);
#endif

/*Ŀ¼������ʼ*/
extern MR_SEARCH_HANDLE mr_findStart(const char* name, char* buffer, uint32 len);
/*ȡ��һ��Ŀ¼�������*/
extern int32 mr_findGetNext(MR_SEARCH_HANDLE search_handle, char* buffer, uint32 len);
/*Ŀ¼��������*/
extern int32 mr_findStop(MR_SEARCH_HANDLE search_handle);

/*�˳�ƽ̨*/
extern int32 mr_exit(void);

/*ͬ��Cache*/
extern int32 mr_cacheSync(void* addr, int32 len);

/*��ʼ�ֻ���*/
extern int32 mr_startShake(int32 ms);
/*�����ֻ���*/
extern int32 mr_stopShake(void);

/*������Ƶ����*/
extern int32 mr_playSound(int type, const void* data, uint32 dataLen, int32 loop);
/*ֹͣ������Ƶ*/
extern int32 mr_stopSound (int type);

/*����һ������Ϣ*/
extern int32 mr_sendSms(char* pNumber, char*pContent, int32 flags);
/*����绰*/
extern void mr_call(char *number);
/*ȡ������ID��0 �ƶ���1 ��ͨ*/
extern int32 mr_getNetworkID(void);
/*����WAP*/
extern void mr_connectWAP(char* wap);


/*GUI �ӿ�*/
extern int32 mr_menuCreate(const char* title, int16 num);
extern int32 mr_menuSetItem(int32 menu, const char *text, int32 index);
extern int32 mr_menuShow(int32 menu);
/*����ѡ����Ŀ������*/
extern int32 mr_menuSetFocus(int32 menu, int32 index);
extern int32 mr_menuRelease(int32 menu);
extern int32 mr_menuRefresh(int32 menu);



extern int32 mr_dialogCreate(const char * title, const char * text, int32 type);
extern int32 mr_dialogRelease(int32 dialog);
extern int32 mr_dialogRefresh(int32 dialog, const char * title, const char * text, int32 type);

extern int32 mr_textCreate(const char * title, const char * text, int32 type);
extern int32 mr_textRelease(int32 text);
extern int32 mr_textRefresh(int32 handle, const char * title, const char * text);

extern int32 mr_editCreate(const char * title, const char * text, int32 type, int32 max_size);
extern int32 mr_editRelease(int32 edit);
extern const char* mr_editGetText(int32 edit);

extern int32 mr_winCreate(void);
extern int32 mr_winRelease(int32 win);

extern int32 mr_getScreenInfo(mr_screeninfo * screeninfo);


/*Socket�ӿ�*/
extern int32 mr_initNetwork(MR_INIT_NETWORK_CB cb, const char *mode);
extern int32 mr_closeNetwork(void);
extern int32 mr_getHostByName(const char *name, MR_GET_HOST_CB cb);
extern int32 mr_socket(int32 type, int32 protocol);
extern int32 mr_connect(int32 s, int32 ip, uint16 port, int32 type);
extern int32 mr_closeSocket(int32 s);
extern int32 mr_recv(int32 s, char *buf, int len);
extern int32 mr_recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port);
extern int32 mr_send(int32 s, const char *buf, int len);
extern int32 mr_sendto(int32 s, const char *buf, int len, int32 ip, uint16 port);

/* begin:added by Tommy.yan for convert endian 20120117 */
extern int32 mr_str_wstrlen(const char *str);
extern char* mr_str_convert_endian(char *str);
extern char* mr_str_convert_endian_with_size(char *str, int32 size);
/* end:adde by Tommy.yan for convert endian 20120117 */
#define MR_GIF_DECODE_SUCCESS 1001

#endif

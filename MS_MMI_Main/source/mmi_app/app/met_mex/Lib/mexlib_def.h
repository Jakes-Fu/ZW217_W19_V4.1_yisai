/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Def.h
* �ļ���ʶ��
* ����ժҪ�� //����ƽ̨�������͡�ö��
*
************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_DEF_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_DEF_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
#define __MEXLIB_DEF_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
#define __MEXLIB_DEF_CLIENT_H__
#endif

#if !defined(__MEXLIB_H__)
	#error Please Include MexLib.h instead of MexLib_Def.h!!!
#endif

#ifdef MEX_PLATFORM_SPREAD
    typedef signed long int   int32;
    typedef unsigned long int uint32;
#else
    typedef signed int        int32;
    typedef unsigned int      uint32;
#endif

typedef void*			funcptr;
typedef signed short	int16;
typedef unsigned short	uint16;
typedef signed char		int8;
typedef unsigned char	uint8;
typedef unsigned char	byte;
#if !defined(MEX_RVCT_4_0_CPP)

#if defined(_MSC_VER)
#if (_MSC_VER < 1400) || !defined(__cplusplus)
typedef unsigned short	wchar_t;
#endif
#else
typedef unsigned short	wchar_t;
#endif

#endif /* !MEX_RVCT_4_0_CPP */
typedef unsigned int	size_t;

// mex color type
typedef	uint16          mex_color;

#ifndef __cplusplus
typedef unsigned char	bool;
#endif

#define true			(1)
#define false			(0)

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#ifndef TRUE
#define TRUE			(1)
#define FALSE			(0)
#endif
#endif
#ifndef NULL
#define NULL		    (0)
#endif

#ifdef __cplusplus
#define EXTERN	extern "C"
#else
#define EXTERN	extern
#endif

#ifdef __cplusplus
#define MEX_DECLARE_NEW_DELETE	\
	void *operator new(uint32 sz)	{ return malloc(sz); } \
	void operator delete(void *p)	{ free(p); }
#else
#define MEX_DECLARE_NEW_DELETE
#endif

#if defined(MEX_PLATFORM_MTK) || defined(MEX_PLATFORM_SPREAD)
#define MEX_EXPORT EXTERN
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_WIN32) || defined(MEX_PLATFORM_SIMULATOR)
#define MEX_EXPORT EXTERN __declspec( dllexport )
#endif

//max file path length
#define MEX_FILE_NAME_MAX_LENGTH			100			//like "D:\MexApp\HelloWorld\HelloWorld.mex"
#define MEX_SHORT_FILE_NAME_MAX_LENGTH 		30			//like "HelloWorld.mex"


typedef enum
{
	E_MSR_NoError,
	E_MSR_NoMemory,
	E_MSR_NoLib,
	E_MSR_UserError = 50,
	E_MSR_Other = 200,
	E_MSR_MAX
} E_Mex_Startup_Result;

typedef enum
{
	E_MIMR_Ok,
	E_MIMR_NoMemory,
	E_MIMR_NoPermission,
	E_MIMR_UserError = 50,
	E_MIMR_Other = 200,
	E_MIMR_MAX
} E_Mex_InitMode_Result;

//key event 
typedef enum
{
	E_MKE_Up,
	E_MKE_Down,
	E_MKE_Repeat,
	E_MKE_MAX
} E_Mex_KeyEvent;

typedef enum
{
	E_MME_Down,
	E_MME_Move,
	E_MME_Up,
	E_MME_MAX
} E_Mex_MouseEvent;


typedef enum
{
	E_MRM_None,
	E_MRM_Tiny,
	E_MRM_Small,
	E_MRM_Full,
	E_MRM_MAX
} E_Mex_RunMode;

/*MTK hardware version*/
typedef enum
{
	E_MMH_6205B,
	E_MMH_6208,
	E_MMH_6217,
	E_MMH_6218,
	E_MMH_6218B,
	E_MMH_6219,	
	E_MMH_6223,
	E_MMH_6223P,
	E_MMH_6225,
	E_MMH_6226,
	E_MMH_6226D,
	E_MMH_6226M,
	E_MMH_6227,
	E_MMH_6227D,
	E_MMH_6228,
	E_MMH_6229,
	E_MMH_6230,
	E_MMH_6235,
	E_MMH_6235B,
	E_MMH_6238,
	E_MMH_6239,
	E_MMH_6253,
	E_MMH_6253T,
	E_MMH_6268,
	E_MMH_6268A,
	E_MMH_6268T,
	E_MMH_6516,
	E_MMH_UNKNOW_VER
}E_Mex_MTK_HWVER;

/*MTK software version*/
typedef enum
{
	E_MMS_O7B,
	E_MMS_O8A,
	E_MMS_O9A,
	E_MMS_UNKNOW_VER
}E_Mex_MTK_SWVER;

/*SPREAD hardware version*/
typedef enum
{
	E_MSH_6600L,
	E_MSH_UNKNOW_VER
}E_Mex_PHONE_HWVER;

/*SPREAD software version*/
typedef enum
{
	E_MSS_O7B,
	E_MSS_UNKNOW_VER
}E_Mex_SPREAD_SWVER;


/* platform enum */
typedef enum
{
	E_MPP_MTK,
	E_MPP_SPREAD,
	E_MPP_MSTAR,
	E_MPP_UNKNOW_PLFM,
	E_MPP_SPREAD_LITTLE,

    E_MPP_MAX_PLATFORM = 0xF0
}E_Mex_Phone_Platform;

typedef enum
{
	E_MSS_None = 0,
	E_MSS_Idle = -1
}E_Mex_Sys_ScreenId;

typedef enum
{
	E_MPC_DualSim = 0,								//�Ƿ�֧��˫�� ����0��ʾ��֧�� 1��ʾ֧��
	E_MPC_MaxIntMem = 1,							//���֧���ڲ��ڴ棨byte��
	E_MPC_MaxExtMem = 2,							//���֧���ⲿ�ڴ棨byte��
	E_MPC_MaxReservedMem = 3,						//���֧�ָ�ȫ������ʹ�õı����ڴ棨byte��
	E_MPC_SystemRootDir = 4,						//ϵͳ��Ŀ¼ ���� wchar_t *   unicode����
	E_MPC_AppRootDir = 5,							//Ӧ�ó����Ŀ¼ ���� wchar_t *  unicode����
	E_MPC_Imei = 6,									//�ֻ��豸 imei��  ���û�гɹ���ȡ ����0   ����ɹ���ȡ ���� int8 [16] ascii���ַ���
	E_MPC_Master_Service_Type = 7,					//sim1 ����״̬ Ϊ0 ���ʾ�޷���   Ϊ1 Ϊ����״̬����
	E_MPC_Slave_Service_Type = 8,					//sim2 ����״̬ Ϊ0 ���ʾ�޷���   Ϊ1 �����״̬����
	E_MPC_Master_Imsi = 9,							//sim1 imsi�� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [17] ascii���ַ���
	E_MPC_Slave_Imsi = 10,							//sim2 imsi�� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [17] ascii���ַ���
	E_MPC_Master_SmsCent = 11,						//sim1 �������ĺ� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [27] ascii���ַ���						
	E_MPC_Slave_SmsCent = 12,						//sim2 �������ĺ� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [27] ascii���ַ���	
	E_MPC_Master_CellInfo = 13,						//sim1 ��վ��Ϣ  ���� �ṹMex_Cell_Info��ָ��   Ҫ����ʱ������ж���Ϣ����Ч��					
	E_MPC_Slave_CellInfo = 14,						//sim2 ��վ��Ϣ  ���� �ṹMex_Cell_Info��ָ��	Ҫ����ʱ������ж���Ϣ����Ч��
	E_MPC_Master_PLMN = 15,							//�ݲ�����
	E_MPC_Slave_PLMN = 16,							//�ݲ�����
	E_MPC_Master_Sim_Remove = 17,					//sim1 �Ƿ񱻰ε� ���� 1 ��ʾSIM1 ���ε�
	E_MPC_Slave_Sim_Remove = 18,						//sim2 �Ƿ񱻰ε� ���� 1 ��ʾSIM2 ���ε�
	E_MPC_Master_Sim_Valid = 19,						//sim1 �Ƿ�Ϸ� ���� 1 ��ʾSIM1 �Ϸ�						
	E_MPC_Slave_Sim_Valid = 20,						//sim2 �Ƿ�Ϸ� ���� 1 ��ʾSIM2 �Ϸ�
	E_MPC_Master_Sim_Service = 21,					//sim1 ������ID   ����ö�� E_MEX_Net_Service_ID
	E_MPC_Slave_Sim_Service = 22,					//sim2 ������ID   ����ö�� E_MEX_Net_Service_ID
	E_MPC_PHONE_CODE_VERSION = 23,					//PHONEԴ����汾�� ����ö�� E_Mex_MTK_SWVER or E_Mex_SPREAD_SWVER
	E_MPC_Profile = 24,								//��ȡ��ǰ�龰ģʽ	����ö�� E_MEX_Phs_Profile
	E_MPC_RingVolumeLevel = 25,						//��ȡ��ǰ��������	����uint8  ���� 0 - 6 �߼�						
	E_MPC_BacklightOn = 26,							//��ȡ��ǰ����״̬  0��ʾ����ر�  1 ��ʾ���⿪��
	E_MPC_Language = 27,								//��ȡ��ǰ�ֻ�����	����ö�� E_MEX_Phs_Language
	E_MPC_TimeFmt = 28,								//��ȡ��ǰʱ���ʽ  0 ��ʾ12Сʱ�� 1��ʾ24Сʱ��
	E_MPC_DateFmt = 29,								//��ȡ��ǰ���ڸ�ʽ	����ö��E_MEX_Phs_DateFormat
	E_MPC_FlightMode = 30,							//��ȡ��ǰ�Ƿ�Ϊ�ɺ�ģʽ  0Ϊ�� 1Ϊ��
	E_MPC_CurCityName = 31,							//��ȡ��ǰ����ʱ���������� ���� wchar_t * unicode���� �Һ͵�ǰ���������й�ϵ
	E_MPC_TimeZone = 32,								//��ȡ��ǰ���õ�ʱ����GMTʱ��Ĳ�ֵ ,��Ϊʱ�����float�͵�,���Է���ֵ�Ŵ���100��,�û����Լ�����Ӧת����
	E_MPC_AudPlyRun = 33,							//mp3�Ƿ����ڲ���
	E_MPC_FMRun = 34,								//fm�Ƿ����ڲ���
	E_MPC_CurWPID = 35,								//��ǰ��ֽID Ϊ0 ���Զ����ֽ
	E_MPC_CurWPPath = 36,							//�Զ����ֽ·��	����int8 *   unicode�����·��  ע�� ��E_MPC_CurWPID���ز�Ϊ0ʱ ��ֵ���ܻ����	
	E_MPC_Usb_MassStorage = 37,						//usb�Ƿ��ڴ洢״̬
	E_MPC_Tcard_State = 38,							//�洢��״̬ 0 Ϊ�޴洢�� 1Ϊ�д洢��
	E_MPC_Tcard_Drive = 39,							//��ȡ�洢���̷���ĸ һ��Ϊ D,E..... �޴洢��ʱ�÷���ֵ������
	E_MPC_Tcard_FreeSpace = 40,						//��ȡ�洢��ʣ��ռ�  ��λ byte
	E_MPC_System_FreeSpace = 41,						//��ȡϵͳ��ʣ��ռ�  ��λ byte
	E_MPC_ClamClose = 42,							//��ȡ�Ƿ�ϸ�  ����1 ��ʾ��ǰ�Ѻϸ� �������ֱ��� ��ԶΪ0��
	E_MPC_BT_Receiving = 43,							//�����Ƿ��ڿ�������������״̬ 0Ϊ�� 1Ϊ��
	E_MPC_MSG_Interrupt = 44,						//��ǰ�Ƿ��ж� ����Ҫ���жϵ�ǰ�Ƿ�������������磩
	E_MPC_Screen_Width = 45,							//��Ļ�Ŀ�
	E_MPC_Screen_Height = 46,						//��Ļ�ĸ�
	E_MPC_Touch_Supprot = 47,						//�Ƿ�֧�ִ���
	E_MPC_KeyPadLock = 48,							//��ǰ�������Ƿ���
	E_MPC_EarphonePlugged = 49,						//�Ƿ�������
	E_MPC_LcdSleep = 50,								//lcd���Ƿ���������
	E_MPC_Master_NWK_ACCOUNT = 51,					//sim1 ����ʱ���˺�
	E_MPC_Slave_NWK_ACCOUNT = 52,					//sim2 ����ʱ���˺�
	E_MPC_Screen_AllowBltDbBuf = 53,					//�Ƿ�����˫����滭
	E_MPC_SUPPORT_CAMERA_NUM = 54,					//֧������ͷ����Ŀ 0 ��1 ��2
	E_MPC_CMAERA_ZOOM_LIMIT = 55,					//����ͷ��������� 
	E_MPC_BACK_CAMERA_HORIZONTAL = 56,				//������ͷ�Ƿ�Ĭ��Ϊˮƽ�����
	E_MPC_CAMERA_PREVIEW_MAINLCD_ROTATE = 57,		//Ԥ����Ļ��ת�Ƕ�
	E_MPC_LCM_MAINLCD_ROTATE = 58,					//����Ļ��ת�Ƕ�
	E_MPC_PHONEBOOK_READY = 59,                      //PhoneBook�Ƿ�׼����.����1��ʾPhoneBook�Ѿ�׼����
	E_MPC_PHONEBOOK_ENTRY_INDEX_PTR = 60,            //PhoneBook��Ŀ���������ָ��
	E_MPC_PHONEBOOK_ENTRY_COUNT = 61,                //PhoneBook��Ŀ����
	E_MPC_PHONEBOOK_MEM_PHONE_TOTAL = 62,            //PhoneBook�ֻ�����,�������ֻ��˿ɴ洢�绰��Ŀ����
	E_MPC_PHONEBOOK_MEM_PHONE_USED = 63,             //PhoneBook�ֻ�����,������ʹ�õ���Ŀ����
	E_MPC_PHONEBOOK_MEM_SIM1_TOTAL = 64,             //PhoneBook sim1����
	E_MPC_PHONEBOOK_MEM_SIM1_USED = 65,              //PhoneBook sim1��ʹ������
    E_MPC_PHONEBOOK_MEM_SIM2_TOTAL = 66,             //PhoneBook sim2����
    E_MPC_PHONEBOOK_MEM_SIM2_USED = 67,              //PhoneBook sim2��ʹ������

	E_MPC_MEX_PLATFOEM_STATUS_ICON_ID = 68,			//  ϵͳ״̬�� ƽ̨ͼ�� id
	E_MPC_MEX_MASTER_QQ_STATUS_ICON_ID = 69,			//  ϵͳ״̬�� QQ1ͼ�� id
	E_MPC_MEX_SLAVE_QQ_STATUS_ICON_ID = 70,			//  ϵͳ״̬�� QQ2ͼ�� id

	E_MPC_MEX_PLATFOEM_ICON_IMAGE_ID = 71,			// ƽ̨ͼ��ͼƬID
	E_MPC_MEX_QQ_ONLINE_ICON_IMAGE_ID = 72,			// QQ����ͼ��ͼƬID
	E_MPC_MEX_QQ_OFFLINE_ICON_IMAGE_ID = 73,			// QQ����ͼ��ͼƬID
	E_MPC_MEX_QQ_MSG_ICON_IMAGE_ID = 74,				// QQ����Ϣͼ��ͼƬID
	E_MPC_MEX_QQ_HIDE_ICON_IMAGE_ID = 75,			// QQ����Ϣͼ��ͼƬID
	E_MPC_MEX_QQ_AWAY_ICON_IMAGE_ID = 76,			// QQ�뿪��Ϣͼ��ͼƬID

	E_MPC_MEX_DEFUALT_FOLDER_PHOTO = 77,				// Ĭ����Ƭ/ͼƬ���·��
	E_MPC_MEX_DEFUALT_FOLDER_EBOOKS = 78,			// Ĭ�ϵ�������·�� 
	E_MPC_MEX_DEFUALT_FOLDER_VIDEO = 79,				// Ĭ����Ƶ���ݴ��·��	
	E_MPC_MEX_DEFUALT_FOLDER_TEMP = 80,				// Ĭ��ϵͳ��ʱ�ļ���
	E_MPC_MEX_DEFUALT_FOLDER_MUSIC = 81,				// Ĭ��MP3��Ÿ���·��
	E_MPC_MEX_DEFUALT_FOLDER_AUDIO = 82,				// Ĭ���������·��


	E_MPC_DualBattery = 83,							//�Ƿ�֧��˫���
	E_MPC_Master_Battery_Status = 84,				//����ص�״̬   �ο�E_MEX_Battery_Status
	E_MPC_Master_Battery_Level = 85,					//����صĵ���
	E_MPC_Slave_Battery_Status = 86,					//�����״̬		�ο�E_MEX_Battery_Status
	E_MPC_Slave_Battery_Level = 87,					//����ص���   
	E_MPC_Max_Battery_Level = 88,					//��ص�������


	E_MPC_Master_Sim_Signal = 89,					//����sim�ź� 0 �� E_MPC_Max_Sim_Signal 0 Ϊ���ź�  
	E_MPC_Slave_Sim_Signal = 90,
	E_MPC_Max_Sim_Signal = 91,
	E_MPC_Invaild_Sim_Signal = 92,

	E_MPC_MEX_Widget_AudioPlayer = 93,				//�Ƿ�֧�� ���沥����
	E_MPC_AudPly_Play_Handle = 94,					// ����/��ͣ��ť�¼�
	E_MPC_AudPly_Stop_Handle = 95,					//	ֹͣ��ť�¼�
	E_MPC_AudPly_Next_Handle = 96,					// ��һ��
	E_MPC_AudPly_Prev_Handle = 97,					// ��һ��
	E_MPC_AudPly_Inc_Vol_Handle = 98,				// ��������
	E_MPC_AudPly_Dec_Vol_Handle = 99,				// ��������
	E_MPC_AudPly_Fast_Forward_Handle = 100,			// ���
	E_MPC_AudPly_Rewind_Handle = 101,					// ����
	E_MPC_AudPly_Cencal_Seek_Handle = 102,			// ȡ���������

	E_MPC_AudPly_GetInitState = 103,					//��ȡ����״̬	
	E_MPC_AudPly_Is_Stop = 104,						//�Ƿ���ֹͣ
	E_MPC_AudPly_GetMusicName = 105,					//��ȡ��ǰ����������
	E_MPC_AudPly_GetPlayedMs = 106,					//��ȡ��ǰ�����Ѳ��ź�����
	E_MPC_AudPly_GetTotalMs = 107,					//��ȡ��ǰ������ʱ��
	E_MPC_AudPly_MusicIndex = 108,					//��ȡ��ǰ�����ڲ����б��е�������
	E_MPC_AudPly_TotalMusicNum = 109,					//��ȡ��ǰ�����б����ܸ�����
	E_MPC_AudPly_Volume = 110,						//��ȡ��ǰ����
	E_MPC_AudPly_Max_Volume = 111,					//��ȡ�������

	E_MPC_AudPly_Init = 112,							//��ʼ��������

	E_MPC_AudPly_Spectrum_Display = 113,				//�Ƿ�֧��Ƶ����ʾ
	E_MPC_AudPly_Start_Calc_Spectrum = 114,			//��ʼ����Ƶ����Ϣ
	E_MPC_AudPly_Stop_Calc_Spectrum = 115,			//ֹͣ����Ƶ����Ϣ
	E_MPC_AudPly_Get_Spectrum = 116,					//��ȡƵ����Ϣ  ���� U8 top_p[16] U8 val_p[16]

	E_MPC_DEFAULT_NW_MODE = 117,
	E_MPC_DEFAULT_NW_SIM = 118,
	
	E_MPC_MexDir_In_Phone = 119,
	E_MPC_PHONE_HW_VERSION = 120,						//PHONEӲ���汾�� ����ö�� E_Mex_MTK_HWVER or E_Mex_SPREAD_HWVER
	E_MPC_MexPlatFromVersion = 121,					//��ȡƽ̨�汾��
	E_MPC_MexVenderID = 122,							//������
	E_MPC_MexProjectID = 123,							//��Ŀ���
	E_MPC_MexPhoneUA = 124,							//���ͺ�
	E_MPC_MexMachineID = 125,							//�������
	E_MPC_MinFreeSpace = 126,							//ƽ̨������Ҫ�Ĵ洢���ռ�  ��λbyte
	E_MPC_VideoIsPlaying = 127,						//�Ƿ��ڲ�����Ƶ
	E_MPC_VideoIsRecing = 128,

	E_MPC_EXTERNALLIB_GUI = 129,						//�Ƿ�֧��GUI���ÿ�

    E_MPC_Master_DummyInCall = 130,                   //sim1�Ƿ�֧��ģ������ 1-֧��, 0-��֧��
    E_MPC_Slave_DummyInCall = 131,                    //sim2�Ƿ�֧��ģ������ 1-֧��, 0-��֧��

	E_MPC_MaxIntMem_App = 132,						//�ڲ��ڴ���App�ڴ�ռ�õĴ�С
	E_MPC_MaxIntMem_Med = 133,						//�ڲ��ڴ���Med�ڴ�ռ�õĴ�С

	E_MPC_GetActiveScreenId = 134,					//��ȡ��ǰscreen id
	E_MPC_SysMinUpdateDelay = 135,					//��ȡϵͳtick���ٵȴ�ʱ��
	E_MPC_ScrWarmUpDelay = 136,						//��Ļ����->�ָ��ĵȴ�ʱ��

	E_MPC_CanFileOptionInBackLightOff = 137,			//�����ⰴ���ܷ�����ļ�

	E_MPC_SPECIAL_VERSION = 138,						//ȡ������汾ID, ��qq�汾,msn�汾�ȵ�

	E_MPC_MEX_VERSION = 139,							//��ȡƽ̨�汾��

	E_MPC_MEX_PATCH_VERSION = 140,					//��ȡƽ̨������

	E_MPC_PauseBgMusicOnLaunch = 141,					//�Ƿ���ƽ̨����ʱ��ͣ�������ֵĲ���

	E_MPC_MEX_SDK_VER = 142,							//��ȡƽ̨SDK�汾��(int)
	
#if MEX_SDK_VER >= 1002
    E_MPC_PhnSetWallPaperTypeMask = 143,              //��ȡ�ֻ���֧�ֵı�ֽ����
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1003
		E_MPC_MaxSocNum = 144,						//��ȡ���֧��ͬʱ�򿪵�socket��
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1007
	E_MPC_DeleteScreenHotShutdown = 145,				//mex��Ļ��ɾ��ʱ���Ƿ�hotshutdown
#endif /* MEX_SDK_VER */

#if MEX_SDK_VER >= 1008
	E_MPC_SUPPORT_ENCODEJPGTOMEM = 146,			//�Ƿ�֧��ֱ��ѹ��jpg���ڴ���
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1009
	E_MPC_AnyAlarmActivated = 147,				//�Ƿ������崦�ڼ���״̬
	E_MPC_CanConfigScreen = 148,					//�Ƿ����������Ļ��С
#endif

#if MEX_SDK_VER >= 1010
	E_MPC_Master_Have_Missed_Call = 149,			//�����Ƿ���δ����δ���绰
	E_MPC_Slave_Have_Missed_Call = 150,			//�����Ƿ���δ����δ���绰
#endif

#if MEX_SDK_VER >= 1011
	E_MPC_Has_File_Read_Cache = 151,				//�ļ���д�Ƿ���Ҫ���л���
#endif

#if MEX_SDK_VER >= 1012
	E_MPC_DCM_Address = 152,                      // DCM (dynamic code memory) Address [TCM (tightly coupled memory)] (uint8 *)
	E_MPC_DCM_Length = 153,                       // DCM (dynamic code memory) Length (uint32)
	E_MPC_Sram_Address = 154,                     // Sram Address (uint8 *)
	E_MPC_Sram_Length = 155,                      // Sram Length (uint32)
	E_MPC_Sram_Left_Size = 156,                   // Sram ��ǰʣ���С(uint32)
	E_MPC_Sram_Max_Alloc_Size = 157,              // Sram ��ǰ����������ֵ(uint32)
	E_MPC_Mdi_Support_Mid_Play_File = 158,		// Mex Mdi �Ƿ�֧��ѭ������mid�ļ�
	E_MPC_Mdi_Support_Wav_Play_File = 159,		// Mex Mdi �Ƿ�֧��ѭ������Wav�ļ�
	E_MPC_Mdi_Support_Mp3_Play_File = 160,		// Mex Mdi �Ƿ�֧��ѭ������Mp3�ļ�
	E_MPC_SwitchStackToRun_Funcptr = 161,         // uint32 INT_SwitchStackToRun(void *stack_start, uint32 stack_size, funcptr func, uint32 argc, ...)
    E_MPC_Mix_Control_Interface = 162,            //����������ƽӿ�
#endif


#if MEX_SDK_VER >= 1013
    E_MPC_SleepTime_For_SoundPlay = 163,          //sound play ʱϵͳsleeptime
#endif

#if MEX_SDK_VER >= 1017
	E_MPC_MSN_ICON_ID = 164,
	E_MPC_MSN_IMAGE_ONLINE_ID = 165,
	E_MPC_MSN_IMAGE_OFFLINE_ID = 166,	
	E_MPC_MSN_IMAGE_AWAY_ID = 167,	
	E_MPC_MSN_IMAGE_BUSY_ID = 168,	
	E_MPC_MSN_IMAGE_MSG_ID = 169,	
	E_MPC_MSN_IMAGE_IDLE_ID = 170,	
#endif

//#if MEX_SDK_VER >= 1018                   //֧����������
	E_MPC_Third_NWK_ACCOUNT = 171,					//sim1 ����ʱ���˺�
	E_MPC_Third_Service_Type = 172,					//sim3 ����״̬ Ϊ0 ���ʾ�޷���   Ϊ1 Ϊ����״̬����
	E_MPC_Third_Imsi = 173,							//sim3 imsi�� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [17] ascii���ַ���
	E_MPC_Third_SmsCent = 174,						//sim3 �������ĺ� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [27] ascii���ַ���						
	E_MPC_Third_CellInfo = 175,						//sim3 ��վ��Ϣ  ���� �ṹMex_Cell_Info��ָ��   Ҫ����ʱ������ж���Ϣ����Ч��					
	E_MPC_Third_PLMN = 176,							//�ݲ�����
	E_MPC_Third_Sim_Remove = 177,				        //sim3 �Ƿ񱻰ε� ���� 1 ��ʾSIM1 ���ε�
	E_MPC_Third_Sim_Valid = 178,						//sim3 �Ƿ�Ϸ� ���� 1 ��ʾSIM1 �Ϸ�						
	E_MPC_Third_Sim_Service = 179,					//sim3 ������ID   ����ö�� E_MEX_Net_Service_ID
    E_MPC_PHONEBOOK_MEM_SIM3_TOTAL = 180,             //PhoneBook sim3����
    E_MPC_PHONEBOOK_MEM_SIM3_USED = 181,              //PhoneBook sim3��ʹ������
	E_MPC_Third_Sim_Signal = 182,					    //sim3�ź� 0 �� E_MPC_Max_Sim_Signal 0 Ϊ���ź�  
//#endif

	E_MPC_Reserved_Start = 0x400,			//Ԥ��λ ��1024��ʼ
	E_MPC_Reserved_UpdateDelay,
	E_MPC_Reserved_Pre_StartUp,	
	E_MPC_Reserved_Tiny_Loaing_Page,		//Tiny����ҳ����ʾ���ֱ�����Ϣ������Mex_Config_Loading_Page *
	E_MPC_Reserved_Man_Loaing_Page,			//Man����ҳ����ʾ���ֱ�����Ϣ������Mex_Config_Loading_Page *
	E_MPC_Phone_Platform,					//ȡ���ֻ����ں���ƽ̨������E_Mex_Phone_Platform
	E_MPC_Reserved_Enable_Select_DataAccount, //�Ƿ�������û�����������������
	E_MPC_Reserved_Entry_Select_DataAccount,  //���������������
	E_MPC_Reserved_BreathLed_PowerOn = 0x408,   // ���غ���ָ��mmi_breath_led_power_on_qq, ����ԭ��void mmi_breath_led_power_on_qq(void)      
	E_MPC_Reserved_BreathLed_PowerOff = 0x409,  // ���غ���ָ��mmi_power_off_QQ_breath_led, ����ԭ��void mmi_power_off_QQ_breath_led(void)
	E_MPC_Download_font_size= 0x40A,
	E_MPC_GB2312_SUPPORT = 0x40B, //֧��GB2312,1 not support, 0 support.
	E_MPC_Gprs_sim_card = 0x40C,
    E_MPC_Reserved_Dewav_GetPID = 0x500,        //��ȡ��ΪPID����ָ�룬����ԭ��kal_char * release_version_information(void)
    E_MPC_Reserved_HuaQin_GetPID = 0x501,       //��ȡ���ڵ�PID
    E_MPC_Reserved_Youle_Support = 0x502,       //�Ƿ���Youle����汾
	E_MPC_Reserved_End	=	0x800,			//Ԥ��λ��2048����
#if MEX_SDK_VER >= 1016
	E_MPC_FuncPtrMallocForProgData	= 0x1001,	//��ȡ�Զ��������ص��ڴ溯��ָ�룬�ڲ�ʹ��
	E_MPC_FuncPtrFreeForProgData	= 0x1002,	//��ȡ�Զ��������ص��ڴ溯��ָ�룬�ڲ�ʹ��
#endif

#if MEX_SDK_VER >= 1022
	E_MPC_FETION_ICON_ID,
	E_MPC_FETION_IMAGE_ONLINE_ID,
	E_MPC_FETION_IMAGE_OFFLINE_ID, 
	E_MPC_FETION_IMAGE_AWAY_ID,	
	E_MPC_FETION_IMAGE_BUSY_ID,	
	E_MPC_FETION_IMAGE_MSG_ID, 
	E_MPC_FETION_IMAGE_HIDE_ID,	
#endif

#if MEX_SDK_VER >= 1023                   //��ȡ�ֻ�֧�ֵ�sim����
    E_MPC_Numbers_Sim,                            
#endif
//#ifdef __MET_MEX_SIM_PLUS__
	E_MPC_Fouth_Service_Type =0x1101,					//sim4 ����״̬ Ϊ0 ���ʾ�޷���   Ϊ1 �����״̬����
	E_MPC_Fouth_Imsi =0x1103,							//sim4 imsi�� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [17] ascii���ַ���
	E_MPC_Fouth_SmsCent =0x1105,						//sim4 �������ĺ� ��ȡʧ�ܷ���0  ��ȡ�ɹ� ���� int8 [27] ascii���ַ���	
	E_MPC_Fouth_CellInfo =0x1107,						//sim4��վ��Ϣ  ���� �ṹMex_Cell_Info��ָ��	Ҫ����ʱ������ж���Ϣ����Ч��
	E_MPC_Fouth_PLMN =0x1109,							//�ݲ�����
	E_MPC_Fouth_Sim_Remove =0x1111,						//sim4 �Ƿ񱻰ε� ���� 1 ��ʾSIM2 ���ε�
	E_MPC_Fouth_Sim_Valid =0x1113,						//sim4�Ƿ�Ϸ� ���� 1 ��ʾSIM2 �Ϸ�
	E_MPC_Fouth_Sim_Service =0x1115,					//sim4 ������ID   ����ö�� E_MEX_Net_Service_ID
	E_MPC_Fouth_NWK_ACCOUNT =0x1117,					//sim4 ����ʱ���˺�
	E_MPC_PHONEBOOK_MEM_SIM4_TOTAL =0x1120, 			//PhoneBook sim4����
	E_MPC_PHONEBOOK_MEM_SIM4_USED =0x1121,				//PhoneBook sim4��ʹ������
	E_MPC_Fouth_Sim_Signal =0x1123,
    E_MPC_Third_DummyInCall =0x1124,                   //sim1�Ƿ�֧��ģ������ 1-֧��, 0-��֧��
   	E_MPC_Fouth_DummyInCall =0x1125,                    //sim2�Ƿ�֧��ģ������ 1-֧��, 0-��֧��
//#endif/*__MET_MEX_SIM_PLUS__*/
	E_MPC_FuncPtrInvoke_Post_Event = 0x1719,
	
#if defined(MEX_SUPPORT_FOREIGN_VERSION)
	E_MPC_PHONE_CHINESE_IS_DEFINED = 0x1720,
#endif
	E_MPC_MAX  
	
} E_Mex_Phone_Config;

typedef enum
{
	E_MMP_Internal,
	E_MMP_External,
	E_MMP_Reserved,
	E_MMP_MAX
} E_Mex_MemoryPool;


//Sys permission type , add before E_MPT_Max
typedef enum
{
	E_MAPT_SmsSend,
	E_MAPT_SmsRead,	
	E_MAPT_SmsDelete,
	E_MAPT_PhonebookAdd,
	E_MAPT_PhonebookRead,
	E_MAPT_PhonebookUpdate,
	E_MAPT_PhonebookDelete,
	E_MAPT_HookSms,
	E_MAPT_HookCall,
	E_MAPT_CallLogRead,
	E_MAPT_CallLogDelete,
	E_MAPT_Charge,
	E_MAPT_SysAnalyse,
    E_MAPT_MakeCall,
    E_MAPT_SmsSave,
    
	E_MAPT_MAX
}E_Mex_App_Permission_Type;

typedef enum
{
	E_MAEIT_Version,
	E_MAEIT_RegistWidth,
	E_MAEIT_RegistHeight,
	E_MAEIT_PermissionL,
	E_MAEIT_PermissionH,
	E_MAEIT_ProductId,
	E_MAEIT_RequireSdkVer,
	E_MAEIT_TargetOption,
	E_MAEIT_MaxMemSize,
	E_MAEIT_Reserved6,
	E_MAEIT_MAX
}E_Mex_App_Extra_Info_Type;

typedef enum
{
	E_MAT_Kernel,
	E_MAT_LibUpdate,
	E_MAT_HandleIcon,
	E_MAT_HandleWidget,
	E_MAT_HandleFull,
	E_MAT_Internal,
	E_MAT_WidgetUseExtMem,
	E_MAT_WidgetNoExitOnSwitch,
	E_MAT_Reserved2,
	E_MAT_Reserved3,
	E_MAT_Reserved4,
	E_MAT_Reserved5,
	E_MAT_MAX
} E_Mex_App_Type;

typedef enum
{
	E_MMMF_Malloc,				//ret vm ptr, size
	E_MMMF_Free,				//ret void, vm ptr
	E_MMMF_Memcpy,				//ret vm ptr, vm ptr, mem ptr, size
	E_MMMF_ToMem,				//ret mem ptr, mem ptr, vm ptr, size
	E_MMMF_PMalloc,				//ret mem ptr, key str, size, balreadyexist
	E_MMMF_PFree,				//ret void, vm ptr

	E_MMMF_RegLaunchOnScreen,	//ret bSuccess, appid, screen id
	E_MMMF_RegLaunchOnStartup,	//ret bSuccess, appid
	E_MMMF_RegLaunchOnFullAppExit,	//ret bSuccess, appid

	E_MMMF_Invalidate,				//ret bAccepted, appid
	E_MMMF_LaunchOnExit,			//ret void, int* launch parameters
	E_MMMF_GetLastFullScreenBuf,	//ret void, pbuf, pwidth, pheight
	E_MMMF_RegistInitParam,			//ret bSuccess, pInitParam
	E_MMMF_SetCurrScreen,			//ret void, iCurrScreen

	E_MMMF_DisableQuitMsg,			//ret bool, appid, bDisable

	E_MMMF_RegisterIntent,		//future use
	E_MMMF_CallIntent,			//future use
	E_MMMF_GetAppPosition,		//ret bIsExist, appid, pPosX, pPosY
	E_MMMF_SetAppPosition,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetWidgetAlwaysTop,	//ret bIsExist, appid, pPosX, pPosY
	E_MMMF_SetWidgetVisiable,	//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_GetDeskAppImgPro,	//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetAppPriority,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_AllowAcceptKey,		//ret bIsExist, appid, iPosX, iPosY
	E_MMMF_SetWidgetFullModeMemSize,	//ret bSuccess, appid, memsize	
	E_MMMF_GetFreeMemory,
	E_MMMF_SetManCurAppId,
	E_MMMF_AppIsInMemory,
	E_MMMF_Reserved15,
	E_MMMF_Reserved16,

	E_MMMF_MAX
} E_Mex_Mgr_Manager_Func;

typedef enum
{
	E_MSSS_LeaveStart,
	E_MSSS_LeaveEnd,
	E_MSSS_EnterStart,
	E_MSSS_EnterEnd,
	E_MSSS_MAX
} E_Mex_ScreenSwitch_Step;

typedef enum
{
	E_MMT_SPLIT_APP_SYS_MANAGER_BEGIN	= 0,	//below this line are messages used by app -> sys -> manager

	E_MMT_Mgr_Init,								//params (0-3): /
	E_MMT_Mgr_Update,							//params (0-3): /
	E_MMT_Mgr_Destroy,							//params (0-3): bExitAll
	E_MMT_Mgr_MouseEvent,						//params (0-3): mouse event, x, y
	E_MMT_Mgr_KeyEvent,							//params (0-3): keyEvent, keyCode
	E_MMT_Mgr_GetKeyState, 						//params (0-3): keyCode
	E_MMT_Mgr_SetKeyState, 						//params (0-3): keyEvent, keyCode
	E_MMT_Mgr_SetTimer,							//params (0-3): timer, params, bIsInternal
	E_MMT_Mgr_KillTimer,						//params (0-3): timerid, bIsInternal
	E_MMT_Mgr_SetTimerInterval,					//params (0-3): timerid, iNewTime, pOldTime,bIsInternal
	E_MMT_Mgr_GetRunMode,						//params (0-3): appid
	E_MMT_Mgr_SwitchRunMode,					//params (0-3): appid, newMode
	E_MMT_Mgr_GetRegistry,						//params (0-3): /
	E_MMT_Mgr_AppWantExit,						//params (0-3): appid
	E_MMT_Mgr_AppLaunched,						//params (0-3): appid, internal, insystemdir, apptype
	E_MMT_Mgr_GetAppAttribute,					//params (0-3): appid
	E_MMT_Mgr_SetAppAttribute,					//params (0-3): appid, attribute
	E_MMT_Mgr_PauseTimer,						//params (0-3): appid, timerid, bPause
	E_MMT_Mgr_ManagerFunc,						//params (0-3): function category, param0, param1, param2
	E_MMT_Mgr_PaintPause,						//params (0-3): /
	E_MMT_Mgr_PaintResume,						//params (0-3): /
	E_MMT_Mgr_SRamNeedLock,						//params (0-3): appid
	E_MMT_Mgr_SRamUnlocked,						//params (0-3): appid
	E_MMT_Mgr_UpdateThread,						//params (0-3): /
	E_MMT_Mgr_SetThreadTimer,					//params (0-3): timer, params
	E_MMT_Mgr_KillThreadTimer,					//params (0-3): timerid
	E_MMT_Mgr_SetThreadTimerInterval,			//params (0-3): timerid, iNewTime, pOldTime

	E_MMT_Mgr_CircleDeviceEvent,				//params (0-3): Circle input device, input type(clockwise or not)

	E_MMT_Mgr_DualSimModeChanged,				//params (0-3): Old mode, New Mode (see: enum E_Mex_Dual_Sim_Mode)


	E_MMT_Mgr_KeyPadLockChanged,				//params (0-3):  event(see: enum E_Mex_KeyPad_Lock_Event)

	E_MMT_Mgr_LcdRotate,						//params (0-3): E_Mex_LcdRotateType, newWidth , newHeight	

	E_MMT_Mgr_RecvSms,							//reveive new sms
	E_MMT_Mgr_SlideEvent,						//slide on/off

	E_MMT_Mgr_User						= 50,	//params (0-3): tbd, user messages

	E_MMT_SPLIT_APP_SYS_MANAGER_END		= 60,	//above this line are messages used by app -> sys -> manager


	E_MMT_SPLIT_MANAGER_APP_BEGIN		= 61,	//below this line are messages used by manager -> app

	E_MMT_InitMode,								//params (0-3): new run mode, app return <0 on error
	E_MMT_ExitMode,								//params (0-3): old run mode
	E_MMT_MouseEvent,							//params (0-3): mouse event, x, y
	E_MMT_KeyEvent,								//params (0-3): keyEvent, keyCode
	E_MMT_CharInput,							//params (0-3): character (can be unicode)
	E_MMT_Paint,								//params (0-3): buffer ptr, width, height, Mex_Point*
	E_MMT_Timer,								//params (0-3): timer id, timer parameter
	E_MMT_ScreenSwitch,							//params (0-3): eswitchstep
	E_MMT_ManagerDisplay,						//params (0-3): is now showing
	E_MMT_ManagerShutDown,						//params (0-3): /
	E_MMT_SRamNeedLock,							//params (0-3): appid
	E_MMT_SRamUnlocked,							//params (0-3): appid
	E_MMT_Destroy,								//params (0-3): /
	E_MMT_PaintPause,							//params (0-3): /
	E_MMT_PaintResume,							//params (0-3): /
	E_MMT_BackLightOff,							//params (0-3): /
	E_MMT_BackLightOn,							//params (0-3): /
	
	E_MMT_CircleDeviceEvent,					//params (0-3): Circle input device, input type(clockwise or not)

	E_MMT_DualSimModeChanged,					//params (0-3): Old mode, New Mode (see: enum E_Mex_Dual_Sim_Mode)

	E_MMT_KeyPadLockChanged,					//params (0-3):  event(see: enum E_Mex_KeyPad_Lock_Event)

	E_MMT_LcdRotate,							//params (0-3): E_Mex_LcdRotateType, newWidth , newHeight
	E_MMT_AppDumpIn,							//params (0-3): /						
	E_MMT_AppDumpOut,							//params (0-3): /
	E_MMT_AppWantExited,							//params (0-3): /
	E_MMT_AppHasExited,							//params (0-3): /
	E_MMT_GetDeskAppImgPro,						//params (0-3): /
	E_MMT_RecvSms,								//reveive new sms
	E_MMT_SlideEvent,							//params(0) 0:Close 1:Open
	E_MMT_User							= 200,	//params (0-3): tbd, user messages

	E_MMT_SPLIT_MANAGER_APP_END			= 250,	//above this line are messages used by manager -> app

	E_MMT_MAX
} E_Mex_MessageType;

typedef enum
{
	E_MLRT_Rotate_0,
	E_MLRT_Rotate_90,
	E_MLRT_Rotate_180,
	E_MLRT_Rotate_270,
	E_MLRT_Rotate_max
}E_Mex_LcdRotateType;

typedef enum 
{
	E_MKC_KEY_0, 						//���� 0
	E_MKC_KEY_1, 						//���� 1
	E_MKC_KEY_2, 						//���� 2
	E_MKC_KEY_3, 						//���� 3
	E_MKC_KEY_4, 						//���� 4
	E_MKC_KEY_5, 						//���� 5
	E_MKC_KEY_6,						//���� 6
	E_MKC_KEY_7, 						//���� 7
	E_MKC_KEY_8, 						//���� 8
	E_MKC_KEY_9, 						//���� 9
	E_MKC_KEY_SOFTLEFT, 				//���� �����
	E_MKC_KEY_SOFTRIGHT,				//���� �����
	E_MKC_KEY_ENTER,					//���� ȷ��/ѡ��
	E_MKC_KEY_UP,						//���� ��
	E_MKC_KEY_DOWN, 					//���� ��
	E_MKC_KEY_LEFT, 					//���� ��
	E_MKC_KEY_RIGHT,					//���� ��
	E_MKC_KEY_SEND, 					//���� ������
	E_MKC_KEY_END,						//���� �һ���
	E_MKC_KEY_CLEAR,					//���� �����
	E_MKC_KEY_STAR, 					//���� *
	E_MKC_KEY_POUND,					//���� #
	E_MKC_KEY_VOLUME_UP, 				//���� �����
	E_MKC_KEY_VOLUME_DOWN, 				//���� �����
	E_MKC_KEY_OTHER,					//��������
	E_MKC_KEY_EXTRA_1,
	
	E_MKC_KEY_A,                        //��Ӷ�ȫ���̵�֧��
	E_MKC_KEY_B,
    E_MKC_KEY_C,
    E_MKC_KEY_D,
	E_MKC_KEY_E,
	E_MKC_KEY_F,
	E_MKC_KEY_G,
	E_MKC_KEY_H,
	E_MKC_KEY_I,
	E_MKC_KEY_J,
	E_MKC_KEY_K,
	E_MKC_KEY_L,
	E_MKC_KEY_M,
	E_MKC_KEY_N,
	E_MKC_KEY_O,
	E_MKC_KEY_P,
	E_MKC_KEY_Q,
	E_MKC_KEY_R,
	E_MKC_KEY_S,
	E_MKC_KEY_T,
	E_MKC_KEY_U,
	E_MKC_KEY_V,
	E_MKC_KEY_W,
	E_MKC_KEY_X,
    E_MKC_KEY_Y,
    E_MKC_KEY_Z,
    E_MKC_KEY_SPACE,
    E_MKC_KEY_TAB,
    E_MKC_KEY_DEL,
    E_MKC_KEY_ALT,
    E_MKC_KEY_CTRL,
    E_MKC_KEY_WIN,
    E_MKC_KEY_SHIFT,
    E_MKC_KEY_QUESTION,
    E_MKC_KEY_PERIOD,
    E_MKC_KEY_COMMA,
    E_MKC_KEY_EXCLAMATION,
    E_MKC_KEY_APOSTROPHE,
    E_MKC_KEY_AT,
    E_MKC_KEY_BACKSPACE,
    E_MKC_KEY_QWERTY_ENTER,
    E_MKC_KEY_FN,
    E_MKC_KEY_SYMBOL,
    E_MKC_KEY_NUM_LOCK,
    E_MKC_KEY_QWERTY_MENU,
    E_MKC_KEY_OK,
    E_MKC_MAX_QWERTY_KEYS,
    
    E_MKC_KEY_MAX,
	E_MKC_KEY_INVALID = 0xFE			//�Ƿ�ֵ
} E_Mex_KeyCode;

typedef enum 
{
	E_MCI_ClockWise,
	E_MCI_AntiClockWise,
	E_MCI_MAX,
	E_MCI_INVALID = 0xFE			//�Ƿ�ֵ
} E_Mex_CircleInput;


typedef enum
{

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
    // MTK SYSTEM DEFINE
    //��mtkϵͳ������mex������ת��
	// ϵͳģ��ID ͨ��Ϊtask id   ���Բο��ļ� stack_config.h
	E_M2MDC_MOD_NVRAM,			//NVRAM
	E_M2MDC_MOD_SMS,			//SMS
	E_M2MDC_MOD_SMS_2,			//SMS_2
	E_M2MDC_MOD_SIM,			//SIM
	E_M2MDC_MOD_SIM_2,
	E_M2MDC_MOD_L4C,
	E_M2MDC_MOD_L4C_2,
	E_M2MDC_MOD_PHB,
	E_M2MDC_MOD_PHB_2,
	E_M2MDC_MOD_AS,
	E_M2MDC_MOD_AS_2,
	E_M2MDC_MOD_MED,
	E_M2MDC_MOD_MMI,			//MMI

	//task id end  
	//SAP
	E_M2MDC_MMI_L4C_SAP,
	//END
	//����   �ɲο��ļ� GlobalConstants.h
	E_M2MDC_KEY_0,
	E_M2MDC_KEY_1,
	E_M2MDC_KEY_2,
	E_M2MDC_KEY_3,
	E_M2MDC_KEY_4,
	E_M2MDC_KEY_5,
	E_M2MDC_KEY_6,
	E_M2MDC_KEY_7,
	E_M2MDC_KEY_8,
	E_M2MDC_KEY_9,
	E_M2MDC_KEY_UP_ARROW,
	E_M2MDC_KEY_DOWN_ARROW,
	E_M2MDC_KEY_LEFT_ARROW,
	E_M2MDC_KEY_RIGHT_ARROW,
	E_M2MDC_KEY_ENTER,
//#if defined(__MET_MEX_SUPPORT__)&& defined(MEX_G7_UI_SUPPORT)
 //   E_M2MDC_KEY_End,  
//#endif
	E_M2MDC_KEY_STAR,
	E_M2MDC_KEY_POUND,
	E_M2MDC_KEY_LSK,
	E_M2MDC_KEY_RSK,
	//END
	//KEY EVENT
	E_M2MDC_KEY_EVENT_UP,
	E_M2MDC_KEY_EVENT_DOWN,
	//END
	//ProtocolEvents msg

	//end
	//MSG ID
	E_M2MDC_MSG_MMI_CALL_RING_IND,				//MSG_ID_MMI_CC_CALL_RING_IND
	E_M2MDC_MSG_MMI_SMS_DELIVER_IND,			//MSG_ID_MMI_SMS_DELIVER_MSG_IND
	E_M2MDC_MSG_NW_GET_IMEI_REQ,				//MSG_ID_MMI_NW_GET_IMEI_REQ
	E_M2MDC_MSG_NW_GET_IMEI_RSP,				//MSG_ID_MMI_NW_GET_IMEI_RSP
	E_M2MDC_PRT_GET_IMSI_REQ,					//PRT_GET_IMSI_REQ
	E_M2MDC_PRT_GET_IMSI_RSP,					//PRT_GET_IMSI_RSP
	E_M2MDC_MSG_EM_START_REQ,					//MSG_ID_MMI_EM_START_REQ
	E_M2MDC_MSG_EM_START_RSP,					//MSG_ID_MMI_EM_START_RSP
	E_M2MDC_MSG_EM_STATUS_IND,					//MSG_ID_MMI_EM_STATUS_IND
	E_M2MDC_MSG_EM_STOP_REQ,					//MSG_ID_MMI_EM_STOP_REQ
	E_M2MDC_GET_GPRS_DATA_ACCOUNT_REQ,			//PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ
	E_M2MDC_GET_GPRS_DATA_ACCOUNT_RSP,			//PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP
	E_M2MDC_SET_GPRS_DATA_ACCOUNT_REQ,			//PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ
	E_M2MDC_SET_GPRS_DATA_ACCOUNT_RSP,			//PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP
	E_M2MDC_MSG_GET_HOST_BY_NAME_IND,			//MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND
	E_M2MDC_MSG_SOC_NOTIFY_IND,					//MSG_ID_APP_SOC_NOTIFY_IND

	//end

	//sms
	E_M2MDC_SMSAL_MAX_MSG_LEN,
	E_M2MDC_SMSAL_MTI_DELIVER,
	E_M2MDC_SMSAL_MTI_SUBMIT,
	E_M2MDC_SMSAL_DEFAULT_DCS,
	E_M2MDC_SMSAL_UCS2_DCS,
	E_M2MDC_SMSAL_DEFAULT_PID,
	E_M2MDC_SMS_APP_NOBOX,
	E_M2MDC_SMS_INVALID_INDEX,
	E_M2MDC_MAX_DIGITS_SMS,
	E_M2MDC_ENCODING_LENGTH,
	E_M2MDC_SMS_SEND_SAVE_NIL,
	E_M2MDC_SEND_CASE_SEND_ONLY,
	E_M2MDC_FRM_SMS_OK,							//MMI_FRM_SMS_OK
	//end
	//ENCODE TYPE
	E_M2MDC_ENCODE_ASCII,
	E_M2MDC_ENCODE_UNICODE,
/*	E_M2MDC_ENCODE_H_ASCII,
	E_M2MDC_ENCODE_H_UNICODE,
*/
	//END
	//INPUT TYPE   �ο� wgui_categories_inputsenum.h
	E_M2MDC_INPUT_NUMERIC, 				
	E_M2MDC_INPUT_ALPHANUMERIC_UPPERCASE,
	E_M2MDC_INPUT_ALPHANUMERIC_LOWERCASE,
	E_M2MDC_INPUT_ALPHANUMERIC_SENTENCECASE,
	E_M2MDC_INPUT_NUMERIC_PASSWORD, 		
	E_M2MDC_INPUT_ALPHANUMERIC_PASSWORD, 
	E_M2MDC_INPUT_SM_PINYIN,				
	E_M2MDC_INPUT_SM_STROKE,				
	E_M2MDC_INPUT_SM_MULTITAP_PINYIN,	
	//END
	//EM_STRUCT   �ο�em_struct.h
	E_M2MDC_RR_EM_CELL_SELECT_PARA_INFO,			//RR_EM_CELL_SELECT_PARA_INFO
	E_M2MDC_RR_EM_CHANNEL_DESCR_INFO,				//RR_EM_CHANNEL_DESCR_INFO     
	E_M2MDC_RR_EM_CTRL_CHANNEL_DESCR_INFO,			//RR_EM_CTRL_CHANNEL_DESCR_INFO
	E_M2MDC_RR_EM_RACH_CTRL_PARA_INFO,				//RR_EM_RACH_CTRL_PARA_INFO    
	E_M2MDC_RR_EM_LAI_INFO,							//RR_EM_LAI_INFO               
	E_M2MDC_RR_EM_RADIO_LINK_COUNTER_INFO,			//RR_EM_RADIO_LINK_COUNTER_INFO
	E_M2MDC_RR_EM_MEASUREMENT_REPORT_INFO,			//RR_EM_MEASUREMENT_REPORT_INFO
	E_M2MDC_RR_EM_CONTROL_MSG_INFO,					//RR_EM_CONTROL_MSG_INFO
	E_M2MDC_RR_EM_TBF_INFO,							//RR_EM_TBF_INFO
	E_M2MDC_RR_EM_GPRS_GENERAL_INFO,				//RR_EM_GPRS_GENERAL_INFO
	//EN
	//sscstring �ο�sscstringhandle.h
	E_M2MDC_SSC_AUTO,
	E_M2MDC_SSC_SCHINESE,
	E_M2MDC_SSC_TCHINESE,
	E_M2MDC_SSC_ENGLISH,
	E_M2MDC_SSC_DEFAULT,
	//END

	//FS
	E_M2MDC_FS_READ_WRITE,       
	E_M2MDC_FS_READ_ONLY,        
	E_M2MDC_FS_OPEN_SHARED,      
	E_M2MDC_FS_OPEN_NO_DIR,      
	E_M2MDC_FS_OPEN_DIR,         
	E_M2MDC_FS_CREATE,           
	E_M2MDC_FS_CREATE_ALWAYS,    
	E_M2MDC_FS_COMMITTED,		
	E_M2MDC_FS_CACHE_DATA,	    
	E_M2MDC_FS_LAZY_DATA,		
	E_M2MDC_FS_NONBLOCK_MODE,	
	E_M2MDC_FS_PROTECTION_MODE,	
	E_M2MDC_FS_NOBUSY_CHECK_MODE,
	E_M2MDC_FS_DI_BASIC_INFO,
	E_M2MDC_FS_DI_FREE_SPACE,
	E_M2MDC_FS_DI_FAT_STATISTICS,
	//END
	//GDI&LCD
	E_M2MDC_GDI_COLOR_TRANSPARENT,
	E_M2MDC_CAMERA_PREVIEW_LCM_MAINLCD,				//MDI_CAMERA_PREVIEW_LCM_MAINLCD
	E_M2MDC_GDI_LAYER_ENABLE_LAYER_0,
	E_M2MDC_GDI_LAYER_ENABLE_LAYER_1,
	E_M2MDC_GDI_LAYER_ROTATE_0,
	//END
	//CAMERA RESULT
	E_M2MDC_MDI_RES_CAMERA_SUCCEED,
	//END
	//mdi audio  �ο� Mdi_audio.h
	E_M2MDC_MDI_DEVICE_SPEAKER,						//MDI_DEVICE_SPEAKER
	E_M2MDC_MDI_DEVICE_MICROPHONE,					//MDI_DEVICE_MICROPHONE
	E_M2MDC_MDI_DEVICE_SPEAKER2,					//MDI_DEVICE_SPEAKER2
	E_M2MDC_MDI_DEVICE_LOUDSPEAKER,					//MDI_DEVICE_LOUDSPEAKER
	E_M2MDC_MDI_DEVICE_SPEAKER_BOTH,				//MDI_DEVICE_SPEAKER_BOTH
	E_M2MDC_MDI_DEVICE_BT_HEADSET,					//MDI_DEVICE_BT_HEADSET
	E_M2MDC_MDI_DEVICE_AUTO_SELECT,					//�Զ�ѡ��  

	E_M2MDC_MDI_AUDIO_SUCCESS,						//MDI_AUDIO_SUCCESS
	E_M2MDC_MDI_AUDIO_FAIL,							//MDI_AUDIO_FAIL
	E_M2MDC_MDI_AUDIO_END_OF_FILE,					//MDI_AUDIO_END_OF_FILE
	E_M2MDC_MDI_AUDIO_TERMINATED,					//MDI_AUDIO_TERMINATED
	E_M2MDC_MDI_AUDIO_BAD_FORMAT,					//MDI_AUDIO_BAD_FORMAT

	E_M2MDC_MDI_FORMAT_WAV,							//MDI_FORMAT_WAV
	E_M2MDC_MDI_FORMAT_SMF,							//MDI_FORMAT_SMF

    // �ο�enum Media_Format
	E_M2MDC_MEDIA_FORMAT_PCM_8K,                    //MEDIA_FORMAT_PCM_8K
	E_M2MDC_MEDIA_FORMAT_PCM_16K,                   //MEDIA_FORMAT_PCM_16K

    // �ο�enum Media_Status
    E_M2MDC_MEDIA_SUCCESS,                          //MEDIA_SUCCESS
    E_M2MDC_MEDIA_FAIL,                             //MEDIA_FAIL

    // �ο�enum med_result_enum
    E_M2MDC_MED_RES_OK,                             //MED_RES_OK
    E_M2MDC_MED_RES_AUDIO_END,                      //MED_RES_AUDIO_END

	E_M2MDC_MDI_AUD_VOL_EX_NUM,						//MDI_AUD_VOL_NUM

	//�ο��ļ�devices.h
	E_M2MDC_DEVICE_AUDIO_PLAY_INFINITE,				//DEVICE_AUDIO_PLAY_INFINITE
	E_M2MDC_DEVICE_AUDIO_PLAY_ONCE,					//DEVICE_AUDIO_PLAY_ONCE

	E_M2MDC_VOL_TYPE_MEDIA,							//VOL_TYPE_MEDIA

	//end
	//SOC
	E_M2MDC_SOCK_STREAM,
	E_M2MDC_SOCK_DGRAM,
	E_M2MDC_SOCK_SMS,
	E_M2MDC_SOCK_RAW,

	E_M2MDC_SOC_OOBINLINE,
	E_M2MDC_SOC_LINGER,
	E_M2MDC_SOC_NBIO,
	E_M2MDC_SOC_ASYNC,

	E_M2MDC_SOC_NODELAY,
	E_M2MDC_SOC_KEEPALIVE,
	E_M2MDC_SOC_RCVBUF,
	E_M2MDC_SOC_SENDBUF,

	E_M2MDC_SOC_NREAD,
	E_M2MDC_SOC_PKT_SIZE,
	E_M2MDC_SOC_SILENT_LISTEN,
	E_M2MDC_SOC_QOS,

	E_M2MDC_SOC_TCP_MAXSEG,
	E_M2MDC_SOC_IP_TTL,
	E_M2MDC_SOC_LISTEN_BEARER,
	E_M2MDC_SOC_UDP_ANY_FPORT,

	E_M2MDC_SOC_WIFI_NOWAKEUP,
	E_M2MDC_SOC_UDP_NEED_ICMP,
	E_M2MDC_SOC_IP_HDRINCL,
	E_M2MDC_SOC_NON_AUTO_ACT_BEARER,
	E_M2MDC_SOC_AUTO_DEACT_BEARER,

	E_M2MDC_SOC_READ,
	E_M2MDC_SOC_WRITE,
	E_M2MDC_SOC_ACCEPT,
	E_M2MDC_SOC_CONNECT,
	E_M2MDC_SOC_CLOSE,

	E_M2MDC_SOC_SUCCESS,
	E_M2MDC_SOC_ERROR,
	E_M2MDC_SOC_WOULDBLOCK,
	E_M2MDC_SOC_LIMIT_RESOURCE,
	E_M2MDC_SOC_INVALID_SOCKET,
	E_M2MDC_SOC_INVALID_ACCOUNT,
	E_M2MDC_SOC_NAMETOOLONG,
	E_M2MDC_SOC_ALREADY,
	E_M2MDC_SOC_OPNOTSUPP,
	E_M2MDC_SOC_CONNABORTED,
	E_M2MDC_SOC_INVAL,
	E_M2MDC_SOC_PIPE,
	E_M2MDC_SOC_NOTCONN,
	E_M2MDC_SOC_MSGSIZE,
	E_M2MDC_SOC_BEARER_FAIL,
	E_M2MDC_SOC_CONNRESET,
	E_M2MDC_SOC_DHCP_ERROR,
	E_M2MDC_SOC_IP_CHANGED,
	E_M2MDC_SOC_ADDRINUSE,
	E_M2MDC_SOC_CANCEL_ACT_BEARER,

	E_M2MDC_MAX_SOCKET_NUM,
	E_M2MDC_PF_INET,
	E_M2MDC_IPPROTO_IP,
	//END
	//profile
	E_M2MDC_PROFILE_GENERAL,
	E_M2MDC_PROFILE_MEETING,
	E_M2MDC_PROFILE_OUTDOOR,
	E_M2MDC_PROFILE_INDOOR,
	E_M2MDC_PROFILE_HEADSET,
	E_M2MDC_PROFILE_SILENT,
	E_M2MDC_PROFILE_BT,
	//end
	//date format
	E_M2MDC_DF_DD_MMM_YYYY_H,
	E_M2MDC_DF_DD_MM_YYYY_S,
	E_M2MDC_DF_MM_DD_YYYY_S,
	E_M2MDC_DF_YYYY_MM_DD_S,
	E_M2MDC_DF_YYYY_MM_DD_H,
	E_M2MDC_DF_MMM_DD_YYYY,
	//end

	//TextEncodeing   �ο� Conversions.h
	E_M2MDC_BIG_5_ENCODING,					//BIG_5_ENCODING_TYPE
	E_M2MDC_GB2312_ENCODING,				//GB2312_ENCODING_TYPE
	//end

	E_M2MDC_ALWAYS_ASK_SIM_INFO_1,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_2,
	
	E_M2MDC_IPPROTO_ICMP,
	E_M2MDC_IPPROTO_HOPOPTS,
	E_M2MDC_IPPROTO_IGMP,
	E_M2MDC_IPPROTO_IPV4,
	E_M2MDC_IPPROTO_IPIP, 
	E_M2MDC_IPPROTO_TCP,
	E_M2MDC_IPPROTO_UDP,
	E_M2MDC_IPPROTO_RAW,  

    //mmi_frm_sms_app_msgbox_enum
    E_M2MDC_SMS_APP_UNREAD,         //MMI_FRM_SMS_APP_UNREAD
    E_M2MDC_SMS_APP_INBOX,          //MMI_FRM_SMS_APP_INBOX
    E_M2MDC_SMS_APP_OUTBOX,         //MMI_FRM_SMS_APP_OUTBOX
    E_M2MDC_SMS_APP_DRAFTS,         //MMI_FRM_SMS_APP_DRAFTS
    E_M2MDC_SMS_APP_AWAITS,         //MMI_FRM_SMS_APP_AWAITS
    E_M2MDC_SMS_APP_DATA,           //MMI_FRM_SMS_APP_DATA
    //end

	//��©��vol up/down and send
	E_M2MDC_KEY_VOL_UP,
	E_M2MDC_KEY_VOL_DOWN,
	E_M2MDC_KEY_SEND,

#if MEX_SDK_VER >= 1005
    E_M2MDC_MDI_FORMAT_MP3,                         //MDI_FORMAT_DAF
    E_M2MDC_MDI_FORMAT_AMR,                         //MDI_FORMAT_AMR

    E_M2MDC_MEDIA_FORMAT_AMR,                       //MEDIA_FORMAT_AMR
#endif /* MEX_SDK_VER */
#if MEX_SDK_VER >= 1012
	//��©��key clear
	E_M2MDC_KEY_CLEAR,
#endif
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
    // SPREAD SYSTEM DEFINE
    //��Spreadϵͳ������mex������ת��
	E_S2MDC_BLM_MEX_MEM_INTERNAL,					// BLOCK_MEM_POOL_MET_MEX_MEM_INTERNAL (block_mem.h)
	E_S2MDC_BLM_MEX_MEM_EXTERNAL,					// BLOCK_MEM_POOL_MET_MEX_MEM_EXTERNAL (block_mem.h)
	E_S2MDC_BLM_MEX_MEM_RESERVED,					// BLOCK_MEM_POOL_MET_MEX_MEM_RESERVED (block_mem.h)
	E_S2MDC_BLM_NOT_USE,							// NOT_USE (block_mem_def.h)

	//Audio begin
	E_S2MDC_ADR_NO_ERROR,							// AUDIO_RESULT_E(Audio_api.h)
    E_S2MDC_ADR_AUDIO_STOP,
	//Audio end

	//Socket begin
	E_S2MDC_PF_INET,								//pf_inet
	E_S2MDC_IPPROTO_IP,								//ip NOT FIND YET RETURN -1
	E_S2MDC_SOC_ERROR,								// NOT FIND YET,  RETURN -1
	E_S2MDC_SOC_SUCCESS,							// SUCCESS (App_tcp_if.h)
	E_S2MDC_SOC_CLOSE,								// NOT FIND YET,  RETURN 0
	E_S2MDC_SOC_WOULDBLOCK,
    
	E_S2MDC_SOC_READ,								
	E_S2MDC_SOC_WRITE,
	E_S2MDC_SOC_ACCEPT,
	E_S2MDC_SOC_CONNECT,
	

	
	E_S2MDC_SOC_STREAM,								// SOCK_STREAM(App_tcp_if.h)
	E_S2MDC_SOC_DGRAM,								// SOCK_DGRAM(App_tcp_if.h)
	E_S2MDC_SOC_RAW,								// SOCK_RAW(App_tcp_if.h)
	E_S2MDC_SOC_SMS, 								// SOCK_SMS(sprd have no)
	
	E_S2MDC_SOC_DEBUG,								// SO_DEBUG(App_tcp_if.h)
	E_S2MDC_SOC_ACCEPTCONN,							// SO_ACCEPTCONN(App_tcp_if.h)
	E_S2MDC_SOC_REUSEADDR,							// SO_REUSEADDR( App_tcp_if.h)
	E_S2MDC_SOC_KEEPALIVE,							// SO_KEEPALIVE(App_tcp_if.h)
	E_S2MDC_SOC_OOBINLINE,							// SO_OOBINLINE(App_tcp_if.h)
	E_S2MDC_SOC_LINGER,
	E_S2MDC_SOC_ASYNC,

	E_S2MDC_SOC_NBIO,								// SO_NBIO (App_tcp_if.h)
	E_S2MDC_SOC_BIO,								// SO_BIO(App_tcp_if.h)
	E_S2MDC_SOC_MYADDR,								// SO_MYADDR(App_tcp_if.h)
	E_S2MDC_SOC_RXDATA,								// SO_RXDATA(App_tcp_if.h)
	E_S2MDC_SOC_TXDATA,								// SO_TXDATA(App_tcp_if.h)

	E_S2MDC_SOC_AF_INET,							// AF_INET(App_tcp_if.h)
	    
	//Socket end
	E_S2MDC_ADR_AUDIO_MAX_VOL,                      // MAX SOUND VOLUME 

#if (MEX_SDK_VER >= 1018)   //֧����SIM��
    E_S2MDC_MN_DUAL_SYS_1,
    E_S2MDC_MN_DUAL_SYS_2,
    E_S2MDC_MN_DUAL_SYS_3,
    E_S2MDC_CC_SIM1_CALL,
    E_S2MDC_CC_SIM2_CALL,
    E_S2MDC_CC_SIM3_CALL,   
#endif

#if (MEX_SDK_VER >= 1019)   //֧������ת��
    E_S2MDC_SONG_FONT_16,
    E_S2MDC_SONG_FONT_12,
#endif

#if (MEX_SDK_VER >= 1020)   //����SOCKET״̬��
    E_M2MDC_SOC_SS_NOFDREF,           /* no file table ref any more */
    E_M2MDC_SOC_SS_ISCONNECTED,       /* socket connected to a peer */
    E_M2MDC_SOC_SS_ISCONNECTING,      /* in process of connecting to peer */
    E_M2MDC_SOC_SS_ISDISCONNECTING,   /*  in process  of disconnecting */
    E_M2MDC_SOC_SS_CANTSENDMORE,      /* can't send more data to peer */
    E_M2MDC_SOC_SS_CANTRCVMORE,       /* can't receive more data from peer */
    E_M2MDC_SOC_SS_RCVATMARK,         /* at mark on input */
    E_M2MDC_SOC_SS_PRIV,              /* privileged for broadcast, raw... */
    E_M2MDC_SOC_SS_NBIO,              /* non-blocking ops */
    E_M2MDC_SOC_SS_ASYNC,             /* async i/o notify */
    E_M2MDC_SOC_SS_UPCALLED,          /* zerocopy data has been upcalled (for select) */
    E_M2MDC_SOC_SS_INUPCALL,          /* inside zerocopy upcall (reentry guard) */
    E_M2MDC_SOC_SS_UPCFIN,            /* inside zerocopy upcall (reentry guard) */
    E_M2MDC_SOC_SS_WASCONNECTING,     /* SS_ISCONNECTING w/possible pending error */
	E_M2MDC_MET_MEX_SEND,
#endif

#if	(MEX_SDK_VER >= 1021)
    E_S2MDC_ADR_AUDIO_ERROR_IND,
    E_S2MDC_ADR_AUDIO_PLAYEND_IND,
#endif

    E_S2MDC_MED_PCM_SRC_OK,                             //AUDIO_STREAM_GET_SRC_SUCCESS
    E_S2MDC_MED_PCM_SRC_ERROR,                      //MED_RES_AUDIO_END
    E_S2MDC_MED_WAV_TYPE_PCM,
#endif
//#ifdef __MET_MEX_SIM_PLUS__

	E_M2MDC_MOD_SMS_3 =0x400,
	E_M2MDC_MOD_SMS_4 =0x401,
	E_M2MDC_MOD_SIM_3 =0x402,
	E_M2MDC_MOD_SIM_4 =0x403,
	E_M2MDC_MOD_L4C_3 =0x404,
	E_M2MDC_MOD_L4C_4 =0x405,
	E_M2MDC_MOD_PHB_3 =0x406,
	E_M2MDC_MOD_PHB_4 =0x407,
	E_M2MDC_MOD_AS_3 =0x408,
	E_M2MDC_MOD_AS_4 =0x409,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_3 =0x40A,
	E_M2MDC_ALWAYS_ASK_SIM_INFO_4 =0x40B,
//#endif/*__MET_MEX_SIM_PLUS__*/
#if defined(__MET_MEX_SUPPORT__)&& defined(MEX_G7_UI_SUPPORT)
	E_M2MDC_KEY_End,  
#endif
#if defined(MEX_SUPPORT_KEY_EXTRA_1)
    E_M2MDC_KEY_EXTRA_1,
#endif    
	E_SYS2MDC_MAX                                   // system define max	
} E_SYS2MEX_Def_Convert;


//��mtkϵͳ�ṹ��mex�ṹ��ת��
typedef enum
{
	E_M2MSC_SockAddr_Struct,

    //struct convert for hook
    E_M2MSC_Hk_Sms_Deliver_Msg,  // mmi_sms_deliver_msg_ind_struct -> Mex_Hk_Sms_Deliver_Msg
    E_M2MSC_Hk_Cc_Call_Ring,     // mmi_cc_call_ring_ind_struct -> Mex_Hk_Cc_Call_Ring
    //end
    
	E_M2MSC_MAX
}E_MTK2MEX_Struct_Convert;

//����ϵͳ��������
typedef enum
{
	E_MESA_NONE = 0,					//��
	E_MESA_PHONEBOOK = 1,				//�绰��
	E_MESA_SMS = 2,						//����
	E_MESA_NEWSMS = 3,					//�¶���
	E_MESA_SMSINBOX = 4,				//�ռ���
	E_MESA_WAP = 5,						//�����						
	E_MESA_CAMERA = 6,					//�����
	E_MESA_VIDEOREC = 7,				//��Ƶ¼��
	E_MESA_AUDIOPLAYER = 8,				//��Ƶ������
	E_MESA_VIDEOPLAYER = 9,				//��Ƶ������
	E_MESA_PHOTO = 10,					//���
	E_MESA_FM = 11,						//FM
	E_MESA_TV = 12,						//TV
	E_MESA_CALCULATOR = 13,				//������
	E_MESA_ALRAM = 14,					//����
	E_MESA_STOPWATCH = 15,				//���
	E_MESA_CALENDAR = 16,				//����
	E_MESA_FILEMGR = 17,					//�ļ��������
	E_MESA_DIGITHANDLER = 18,			//���ų���
	E_MESA_MAINMENU = 19,				//���˵�

	E_MESA_WAP_TO_URL = 20,				//�������ַ

	E_MESA_MEX_PLATFORM = 21,			//����Mexƽ̨
	E_MESA_MEX_SPECIAL_ENTRY = 22,		//����Mexƽ̨�������
	E_MESA_IDLE_SCREEN = 23,				//�ص���������
	E_MESA_GO_BACK_HISTORY = 24,			//�ص��ϸ�����
#if MEX_SDK_VER >= 1010
	E_MESA_CALLHISTORY = 25,
#endif

#if MEX_SDK_VER >= 1014
	E_MESA_BT = 26,                      //����
	E_MESA_BT_ENTRY_POWER_SWITCH = 27,   //����״̬�л� mmi_bt_entry_power_switch
	E_MESA_BT_ENTRY_CLOSE_ALL_CONNS = 28,//�Ͽ����������豸 mmi_bt_entry_release_all_connections
#endif

#if MEX_SDK_VER >= 1019
    E_MESA_DATETIME_SET = 29,
#endif

#if (MEX_MTK_CODE_VERSION >= 0x1032)
	E_MESA_FUNANDGAMES = 30,				//��Ϸ������
	E_MESA_MULTIMEDIA = 31,				//��ý��
	E_MESA_ORGANIZER = 32,				//������
	E_MESA_SETTINGS = 33,				//����
	E_MESA_PROFILE = 34,					//�龰ģʽ
	E_MESA_EXTRA = 35,					//���ӹ���
	E_MESA_WLAN = 36,					//���߾�����
#endif
	E_MESA_MAX
}E_MEX_Entry_System_App;

//**
typedef enum
{
	E_MPS_None,
	E_MPS_Pause,
	E_MPS_HotReboot,
	E_MPS_ColdReboot,
	E_MPS_HotShutdown,
	E_MPS_ColdShutdown,
	E_MPS_MAX
} E_Mex_PlatformShutdown;

typedef enum
{
	E_MEE_OK,					//����ɹ�
	E_MEE_USBMS,				//usb���ڴ洢ģʽ
	E_MEE_NOCARD,				//�޴��濨
	E_MEE_SPACESHORTAGE,		//�洢�ռ䲻��
	E_MEE_INTMEMINITERR,		//Internal�ڴ��ʼ��ʧ��
	E_MEE_EXTMEMINITERR,		//External�ڴ��ʼ��ʧ��
	E_MEE_RESMEMINITERR,		//Reserved�ڴ��ʼ��ʧ��
	E_MEE_PREPARETFLASHERR,		//T���ļ�����ʧ��
	E_MEE_STARTUPTINYERR,		//����ManagerTinyʧ��
	E_MEE_MAX
}E_Mex_Entry_Error;
typedef enum
{
	E_MDWM_None,
	E_MDWM_CheckExist,					//���ز����ڵ�
	E_MDWM_CheckExistAndVersion,		//���ز����ڵĺͰ汾�͵�
	E_MDWM_Resume,						//��������
	E_MDWM_AllCorver,					//��ͷ����
	E_MDWM_Max
}E_Mex_Downloader_Work_Mode;

//mexDownloader������Ĵ���ʽ
typedef enum
{
	E_MDDM_DownUncompress,				//���±߽�ѹ
	E_MDDM_OnlyDownload,				//ֻ����
	E_MDDM_UncompressFile,				//ֻ��ѹ
	E_MDDM_Max
}E_Mex_Downloader_Deal_Method;

//mexƽ̨����״̬������ƽ̨�ָ�������ԭ�����
typedef enum
{
	E_MSES_None,
	E_MSES_StartManagerTiny,
	E_MSES_StartMmcMonitor,
	E_MSES_StartManagerFull,
	E_MSES_StartPolicygetter,
	E_MSES_StartDownloader,
	E_MSES_StartMexApp,
	E_MSES_Max
}E_Mex_System_Entry_Step;

//�ֻ���sim��ģʽ����
typedef enum
{
	E_MDSM_DualSIM,
	E_MDSM_OnlySIM1,
	E_MDSM_OnlySIM2,
	E_MDSM_FlightMode,
	E_MDSM_Max
}E_Mex_Dual_Sim_Mode;

typedef enum
{
	E_MKLE_KeyPad_Lock,
	E_MKLE_KeyPad_UnLock,	
	E_MKLE_Max
}E_Mex_KeyPad_Lock_Event;

#if MEX_SDK_VER >= 1009
//sync  mmi_fmgr_filetype_enum
typedef enum
{
	E_MFF_TYPE_ALL,      /* this type represent all files (included not supported types, like *.doc */
    E_MFF_TYPE_UNKNOWN,
    E_MFF_TYPE_FOLDER,
    E_MFF_TYPE_FOLDER_DOT,
    
    // Image type
    E_MFF_TYPE_BMP,
    E_MFF_TYPE_JPG,
    E_MFF_TYPE_JPEG,    
    E_MFF_TYPE_GIF,
    E_MFF_TYPE_PNG,
    E_MFF_TYPE_WBMP,
    E_MFF_TYPE_WBM,
    E_MFF_TYPE_M3D,
    E_MFF_TYPE_EMS,
    E_MFF_TYPE_ANM,
    E_MFF_TYPE_SVG,

    // Audio type
    E_MFF_TYPE_IMY,
    E_MFF_TYPE_MID,
    E_MFF_TYPE_MIDI,
    E_MFF_TYPE_WAV,
    E_MFF_TYPE_AMR,
    E_MFF_TYPE_AAC,
    E_MFF_TYPE_MP3,
    E_MFF_TYPE_MP2,
    E_MFF_TYPE_VM,
    E_MFF_TYPE_AWB,
    E_MFF_TYPE_AIF,
    E_MFF_TYPE_AIFF,
    E_MFF_TYPE_AIFC,
    E_MFF_TYPE_AU,
    E_MFF_TYPE_SND,
    E_MFF_TYPE_M4A,
    E_MFF_TYPE_MMF,
    E_MFF_TYPE_WMA,

    // Video type
    E_MFF_TYPE_3PG,
    E_MFF_TYPE_MP4,
    E_MFF_TYPE_MPG,
    E_MFF_TYPE_AVI,
    E_MFF_TYPE_3G2,
    E_MFF_TYPE_SDP,
    E_MFF_TYPE_RAM,
    
   
    E_MFF_TYPE_WML,
    E_MFF_TYPE_HTML,
    E_MFF_TYPE_HTM,
    E_MFF_TYPE_XHTML,
    
    E_MFF_TYPE_JAD,
    E_MFF_TYPE_JAR,

    E_MFF_TYPE_LRC,
    E_MFF_TYPE_THEME,
    E_MFF_TYPE_EBOOK_TXT,
    E_MFF_TYPE_EBOOK_PDB,
    E_MFF_TYPE_FOTA_FULL,

    E_MFF_TYPE_URL,
    
    E_MFF_TYPE_COUNT
}E_Mex_Fmgr_Filtertype;

typedef enum
{
	E_MFST_SELECT_FILE							= 0x01,
	E_MFST_SELECT_FOLDER 						= 0x02,
	E_MFST_SELECT_FILE_OR_FOLDER 				= 0x04,
	E_MFST_SELECT_BROWSE						= 0x08,
	E_MFST_SELECT_REPEAT						= 0x10,
	E_MFST_SELECT_FILE_SET						= 0x20,
	E_MFST_SELECT_ARCHIVE						= 0x30,
	E_MFST_SELECT_ARCHIVE_WITH_SINGLE_OBJECT	= 0x40,
}E_Mex_Fmgr_SelType;


typedef enum
{
	E_MFR_SUCCESS,
	E_MFR_BUSY,
	E_MFR_MEMERROR,
	E_MFR_MAX
}
E_Mex_Fmgr_Reuslt;

typedef void(*Fmgr_CallBack)(wchar_t *path,bool bIsShort);


#endif


#define MEX_LOG_SYSTEM			0x00000001
#define MEX_LOG_MEMORY			0x00000002
#define MEX_LOG_MANAGER			0x00000004
#define MEX_LOG_STDLIB			0x00000008
#define MEX_LOG_OSLIB			0x00000010
#define MEX_LOG_CONFIG			0x00000020
#define MEX_LOG_SOC				0x00000040
#define MEX_LOG_PHS				0x00000080
#define MEX_LOG_MDI				0x00000100
#define MEX_LOG_GUI				0x00000200
#define MEX_LOG_REG				0x00000400
#define MEX_LOG_LIB				0x00000800
#define MEX_LOG_APP0			0x00010000
#define MEX_LOG_APP1			0x00020000
#define MEX_LOG_APP2			0x00040000
#define MEX_LOG_APP3			0x00080000


///////////////////////////////////////////////////////
// target-specific macro definitions
///////////////////////////////////////////////////////

#define MEX_H_LAUNCH_FUNC( cat )							MexLib_Entry_##cat
#define MEX_C_LIB_FUNC_NAME( cat )							g_pMexLib_##cat##_Funcs
#define MEX_H_GET_FUNC_DEFINE_BASE( cat, fn )				(MEX_C_LIB_FUNC_NAME(cat)[E_MF_##cat##_##fn])

#define MEX_H_ENUM_FUNC_START( cat )						typedef enum {
#define MEX_H_ENUM_FUNC( cat, fn )							E_MF_##cat##_##fn,
#define MEX_H_ENUM_FUNC_END( cat )							MEX_H_NB_FUNC(cat) } E_Mex_##cat##_Funcs;

#define MEX_H_NB_FUNC( cat )								E_MF_##cat##_MAX

#if defined(MEX_TARGET_SERVER)

	#define MEX_H_DECLARE_FUNC( ftype, fname, fparams )		extern ftype mex_##fname##fparams;
	#define MEX_C_OVERRIDE_FUNC( cat, fn )					((funcptr)MEX_H_GET_FUNC_DEFINE_BASE(cat, fn)) = (funcptr)mex_##fn

#endif

#if defined(MEX_TARGET_CLIENT)

	#define MEX_H_DECLARE_FUNC( ftype, fname, fparams )		typedef ftype (*fd_##fname)fparams;

	#define MEX_H_GET_FUNC_DEFINE( cat, fname )				((fd_##fname)MEX_H_GET_FUNC_DEFINE_BASE( cat, fname ))
	#define MEX_C_OVERRIDE_FUNC( cat, fn )					MEX_C_LIB_FUNC_NAME(cat)[E_MF_##cat##_##fn] = (funcptr)mex_##fn
#endif

typedef funcptr* (*MexApp_LoadLib_Func)(const wchar_t *szLibName, const wchar_t *szFullFileName);
typedef void (*MexApp_FreeLib_Func)(funcptr * libFuncPtr);

typedef E_Mex_Startup_Result (*MexApp_Start_Func)(int32 iMyAppId, MexApp_LoadLib_Func fnLoadLib, MexApp_FreeLib_Func fnFreeLib);
typedef void (*MexApp_Exit_Func)(int32 iMyAppId, MexApp_FreeLib_Func fnFreeLib);

typedef int32 (*MexApp_MsgHandler_Func)(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMsgType, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3);

typedef void (*MexFuncPtr)(void);

//���뷨�ص�����
//����1: ����֮��Ľ��
//����2: �Ƿ�ȷ������
typedef void (*Mex_Input_Func)(uint8 * inputBuf, bool bIsFirm);


#endif

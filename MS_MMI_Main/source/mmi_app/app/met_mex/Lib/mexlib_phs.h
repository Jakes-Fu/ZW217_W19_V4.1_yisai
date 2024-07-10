/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
*
* �ļ����ƣ� // MexLib_Phs.h
* �ļ���ʶ��
* ����ժҪ�� //mexƽ̨�ֻ�״̬��ؿ�
*
************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_PHS_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_PHS_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
#define __MEXLIB_PHS_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
#define __MEXLIB_PHS_CLIENT_H__
#endif

//common macro definitions for PHS
#define MEX_H_LAUNCH_FUNC_PHS					MEX_H_LAUNCH_FUNC( Phs )
#define MEX_C_LIB_FUNC_NAME_PHS					MEX_C_LIB_FUNC_NAME( Phs )

#define MEX_H_GET_FUNC_DEFINE_PHS(x)			MEX_H_GET_FUNC_DEFINE( Phs, x )

#define MEX_H_ENUM_FUNC_START_PHS				MEX_H_ENUM_FUNC_START( Phs )
#define MEX_H_ENUM_FUNC_PHS(x)					MEX_H_ENUM_FUNC( Phs, x )
#define MEX_H_ENUM_FUNC_END_PHS					MEX_H_ENUM_FUNC_END( Phs )

#define MEX_H_NB_FUNC_PHS						MEX_H_NB_FUNC( Phs )
//end common macro definitions

//������Ӫ�����ͣ�only for gsm900��
typedef enum
{
	E_MEX_NET_ID_CMCC,		//�й��ƶ�
	E_MEX_NET_ID_CNGSM,		//�й���ͨgsm
	E_MEX_NET_ID_UNKNOW,	//Ŀǰ��֧�ֵ�����
	E_MEX_NET_ID_NONE		//�޷���
}E_MEX_Net_Service_ID;

//SIM��id
typedef enum
{
	E_MEX_SIM_NONE,
	E_MEX_SIM_1,
	E_MEX_SIM_2,
	E_MEX_SIM_3,
	E_MEX_SIM_4,
	E_MEX_SIM_MAX
}E_MEX_Sim_Card_ID;

//˫��ͨ������
//�ڷ��Ͷ��Ż��߲���绰֮ǰ ��Ҫ����
typedef enum
{
	E_MAC_CALL_CHANNEL,
	E_MAC_CALLLOG_CHANNEL,
	E_MAC_SMS_CHANNEL,
	E_MAC_PHB_CHANNEL,
	E_MAC_MAX
}E_MEX_AD_CHANNEL;
//�¼�״̬
typedef enum
{
	E_MEX_PHS_NEEDUPDATE,	//��Ҫ����
	E_MEX_PHS_PROCESSING,	//������
	E_MEX_PHS_COMPLETED,	//���
	E_MEX_PHS_ERROR			//����
}E_MEX_Phs_State;


/*��Ӧϵͳ�е� �ṹ MESSAGE*/
typedef struct
{
	int32 src_mod_id;
	int32 dest_mod_id;
	int32 sap_id;
	int32 msg_id;
	void *local_para_ptr;
	void *peer_buff_ptr;
}Mex_MTK_Message_Struct;

typedef int(*i_Callback_i)(int);

//�龰ģʽ
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_PROFILE_GENERAL		( ConvertSysDef(E_M2MDC_PROFILE_GENERAL) )
#define MEX_PROFILE_MEETING		( ConvertSysDef(E_M2MDC_PROFILE_MEETING) )
#define MEX_PROFILE_OUTDOOR		( ConvertSysDef(E_M2MDC_PROFILE_OUTDOOR) )
#define MEX_PROFILE_INDOOR		( ConvertSysDef(E_M2MDC_PROFILE_INDOOR) )
#define MEX_PROFILE_HEADSET		( ConvertSysDef(E_M2MDC_PROFILE_HEADSET) )
#define MEX_PROFILE_SILENT		( ConvertSysDef(E_M2MDC_PROFILE_SILENT) )
#define MEX_PROFILE_BT			( ConvertSysDef(E_M2MDC_PROFILE_BT) )
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)//�μ�MMIENVSET_ENV_MODE_E
enum{
	MEX_PROFILE_GENERAL = 100,
	MEX_PROFILE_MEETING,
	MEX_PROFILE_OUTDOOR,
	MEX_PROFILE_INDOOR,
	MEX_PROFILE_HEADSET,
	MEX_PROFILE_SILENT,
	MEX_PROFILE_BT
};
#endif

/*
//�龰ģʽ
typedef enum
{
E_MPP_GENERAL,			//��׼
E_MPP_MEETING,			//����
E_MPP_OUTDOOR,			//����
E_MPP_INDOOR,			//����
E_MPP_HEADSET,			//����
E_MPP_SILENT,			//����
E_MPP_BT,				//����
E_MPP_UNKNOW			//δ֪
}E_MEX_Phs_Profile;
*/
//�����״̬
typedef enum
{
	E_MPHS_PERMANENT,		//����
	E_MPHS_SHORT_TIME,		//���� ���� һ�� ��ʱ���Զ�ֹͣ
	E_MPHS_OFF,				//�ر�,�����̹ر�
#if (MEX_SDK_VER >= 1025)
	E_MPHS_OFF_REALTIME,     //���̹ر�(for FAUI)
#endif
}E_MEX_Phs_HW_State;

typedef enum
{
	E_MPL_CN,				//��������
	E_MPL_TR,				//��������
	E_MPL_EN,				//Ӣ��
	E_MPL_UNKONW			//δ֪����
}E_MEX_Phs_Language;

//��ص�״̬
typedef enum
{
	E_MBS_NORMAL,			//��ͨ״̬
	E_MBS_CHARGE_IN,		//���ڳ��
	E_MBS_CHARGE_COMPLETE,	//������
	E_MBS_MAX
}E_MEX_Battery_Status;

//���ڸ�ʽ
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_DF_DD_MMM_YYYY_H	( ConvertSysDef(E_M2MDC_DF_DD_MMM_YYYY_H) )
#define MEX_DF_DD_MM_YYYY_S		( ConvertSysDef(E_M2MDC_DF_DD_MM_YYYY_S) )
#define MEX_DF_MM_DD_YYYY_S		( ConvertSysDef(E_M2MDC_DF_MM_DD_YYYY_S) )
#define	MEX_DF_YYYY_MM_DD_S		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_S) )
#define MEX_DF_YYYY_MM_DD_H		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_S) )
#define MEX_DF_MMM_DD_YYYY		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_H) )
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)//�μ�MMISET_DATE_DISPLAY_TYPE_E
enum
{
	MEX_DF_YYYY_MM_DD_H = 100,//MMISET_DATE_YMD
	MEX_DF_MM_DD_YYYY_S,//MMISET_DATE_MDY
	MEX_DF_DD_MMM_YYYY_H,//MMISET_DATE_DMY
	MEX_DF_DD_MM_YYYY_S,
	MEX_DF_YYYY_MM_DD_S,
	MEX_DF_MMM_DD_YYYY,
};
#endif
/*
typedef enum
{
E_MPDF_DD_MMM_YYYY_H,	// DD-MM-YYYY
E_MPDF_DD_MM_YYYY_S,	// DD/MM/YYYY
E_MPDF_MM_DD_YYYY_S,	// MM/DD/YYYY
E_MPDF_YYYY_MM_DD_H,	// YYYY/MM/DD
E_MPDF_MMM_DD_YYYY,		// MM,DD,YYYY
E_MPDF_MMM_MAX
}E_MEX_Phs_DateFormat;
*/
typedef struct
{
	uint16 lac;
	uint16 cell_id;
	uint8 mnc[3];
	uint8 mcc[4];
	uint32 time_stamp;		//��Ϣ��ȡʱ��
}Mex_Cell_Info;

#define MEX_PHS_SMS_INDEX_INVALID   0xFFFF

// MEX PHS SMS ENCODE
typedef enum
{
	E_MPSE_ENCODE_ASCII,
	E_MPSE_ENCODE_UNICODE
} E_MEX_PHS_SMS_ENCODE;

// MEX PHS SMS RESULT
typedef enum
{
	E_MPSR_RESULT_SUCCESS,          // ���ͳɹ�
	E_MPSR_RESULT_FAIL              // ����ʧ��
} E_MEX_PHS_SMS_RESULT;

// MEX PHS SMS MSGBOX TYPE
typedef enum
{
	E_MPSM_INBOX,                   // �ռ���
	E_MPSM_OUTBOX,                  // �ѷ���Ϣ
	E_MPSM_DRAFTS,                  // �ݸ���
	E_MPSM_UNSENT,                  // ������

	E_MPSM_MAX
} E_MEX_PHS_SMS_MSGBOX;

// MEX OUTCALL EVENT
typedef enum
{
	E_MOE_SS_PARSING_STRING_SUCCESS,            // L4���������ɹ�
	E_MOE_SS_PARSING_STRING_FAIL,               // L4���������ʧ��

	E_MOE_ACTIVE_CALL_SUCCESS,                  // �绰�Ѱ�ͨ
	E_MOE_ACTIVE_CALL_FAIL,                     // �绰δ��ͨ

	E_MOE_CONNECTED,                            // ͨ���ѿ�ʼ
#if MEX_SDK_VER >= 1011
	E_MOE_CALL_RELEASED,                        // ͨ���ѽ���
#endif

	E_MPOS_MAX
} E_MEX_OUTCALL_EVENT;

// Mex Phs Sms Handle
typedef void* MEX_PHS_SMS_HANDLE;
// Mex Phs Sms CallBack
typedef void (*Mex_Phs_Sms_CB) (MEX_PHS_SMS_HANDLE handle, E_MEX_PHS_SMS_RESULT res);
// Mex OutCall Handle
typedef void* MEX_OUTCALL_HANDLE;
// Mex OutCall CallBack
typedef void (*MexOutCallCB) (MEX_OUTCALL_HANDLE handle, E_MEX_OUTCALL_EVENT event);
// Mex Phs Read Sms CB
// ����1: msgBox MSGBOX������
// ����2: index ��������
// ����3: pContent ָ���������
// ����4: iContentMemLen �������ݵĳ���, ���ֽڼ���, ������L'\0' ����������
typedef void (*Mex_Phs_Read_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, const wchar_t *pContent, uint16 iContentMemLen, bool success);
// Mex Phs Del Sms CB
// ����1: msgBox MSGBOX������
// ����2: index ��������
// ����3: success �Ƿ�ɾ���ɹ�
typedef void (*Mex_Phs_Del_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, bool success);
// Mex Phs Save Sms CB
typedef void (*Mex_Phs_Save_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, bool success);

// Mex Phs Sms Send Req
typedef struct
{
	const char             *number;        // �ռ��˵绰���룬ֻ����ASCII����
	uint32                  numberLen;     // �ռ��˵绰���볤��
	const char             *content;       // ��������
	uint32                  contentLen;    // �������ݳ��ȣ����ֽڼ��� in bytes
	E_MEX_PHS_SMS_ENCODE    encode;        // �������ݱ��뷽ʽ��ASCII��UNICODE
	E_MEX_Sim_Card_ID       simId;         // ָ�����Ͷ��ŵ� Sim Card��SIM1��SIM2
	Mex_Phs_Sms_CB          smsCB;         // ���Ͷ��ŵĻص�����
} Mex_Phs_Sms_Send_Req;

//typedef for sms
#define MEX_H_PHS_SMS_TS_LEN	7	///< time stamp length

//typedef for phone book
#define MEX_H_PHS_PHB_NAME_LEN           30
#define MEX_H_PHS_PHB_NUMBER_LEN         40
#define MEX_H_PHS_PHB_EMAILADD_LEN       60
#define MEX_H_PHS_PHB_URL_LEN            128
#define MEX_H_PHS_PHB_GROUPINDEX_INVALID 0xFFFF
#define MEX_H_PHS_PHB_ENTRYINDEX_INVALID 0xFFFF

typedef enum
{
	E_MEX_PHB_LOCATION_NONE = -1,
	E_MEX_PHB_LOCATION_PHONE,
	E_MEX_PHB_LOCATION_SIM1,
	E_MEX_PHB_LOCATION_SIM2,
    E_MEX_PHB_LOCATION_SIM3,
	E_MEX_PHB_LOCATION_ALL
} E_Mex_Phs_Phb_Location;


typedef enum
{
	E_MEX_PHB_RESULT_SUCCESS,            // �ɹ�
	E_MEX_PHB_RESULT_BUSY,               // �绰��æ
	E_MEX_PHB_RESULT_WRONG_INDEX,        // ����ֵ����
	E_MEX_PHB_RESULT_WRONG_LOCATION,     // �洢λ�ô���
	E_MEX_PHB_RESULT_NO_SPACE,           // �洢�ռ䲻��
	E_MEX_PHB_RESULT_NO_PERMISSION,      // Ӧ�ó���Ȩ�޲���

	E_MEX_PHB_RESULT_ERROR_UNKNOWN       // δ֪����
} E_Mex_Phs_Phb_Result;


typedef enum
{
	E_MEX_PHB_EVENT_ADDENTRY_SUCCESS,    // �����Ŀ�ɹ�
	E_MEX_PHB_EVENT_ADDENTRY_FAIL,       // �����Ŀʧ��
	E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS, // ������Ŀ�ɹ�
	E_MEX_PHB_EVENT_UPDATEENTRY_FAIL,    // ������Ŀʧ��
	E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS, // ɾ����Ŀ�ɹ�
	E_MEX_PHB_EVENT_REMOVEENTRY_FAIL,    // ɾ����Ŀʧ��

	E_MEX_PHB_EVENT_MAX
} E_Mex_Phs_Phb_Event;

typedef void (*Mex_Phs_Phb_CB) (uint16 uIndex, E_Mex_Phs_Phb_Event eEvent);

//Phone book name struct
typedef struct
{
	wchar_t name[MEX_H_PHS_PHB_NAME_LEN + 1];
	uint8   nameLen; /* Name Length */
} Mex_Phs_Phb_Name;

//Phone book number struct
typedef struct
{
	wchar_t number[MEX_H_PHS_PHB_NUMBER_LEN + 1];
	uint8   numberLen; /* Number Length */
} Mex_Phs_Phb_Number;

//Phone book E-Mail address struct
typedef struct
{
	char  address[MEX_H_PHS_PHB_EMAILADD_LEN + 1];
	uint8 addressLen;
} Mex_Phs_Phb_Email;

typedef struct
{
	Mex_Phs_Phb_Name        name;              // ����
	Mex_Phs_Phb_Number      number;            // �绰����
	Mex_Phs_Phb_Number      number_home;       // ��ͥ�绰(��ѡ)
	Mex_Phs_Phb_Number      number_company;    // ��˾�绰(��ѡ)
	Mex_Phs_Phb_Number      number_fax;        // �������(��ѡ)
	Mex_Phs_Phb_Name        name_company;      // ��˾����(��ѡ)
	Mex_Phs_Phb_Email       email;             // ��������(��ѡ)
	Mex_DateTime            birthday;          // ����
	uint16                  groupIndex;        // ����Ⱥ������
	E_Mex_Phs_Phb_Location  location;          // �洢λ��PHONE, SIM1, SIM2
} Mex_Phs_Phb_Entry;


typedef struct
{
	uint16                  groupIndex;        // ����Ⱥ������
	Mex_Phs_Phb_Name        groupName;         // Ⱥ������
} Mex_Phs_Phb_Group;


typedef enum
{
	E_MPCC_SUCCESS,
	E_MPCC_FAILED,
	E_MPCC_ALREADY_OPEN,
	E_MPCC_NOTSUPPORT_FORMAT,
	E_MPCC_MEM_ERROR,
	E_MPCC_MAX
}E_Mex_Phs_Camera_Code;
#if MEX_SDK_VER >= 1006

typedef enum
{
	E_MPCOF_RGB565,
	E_MPCOF_RGB888,
	E_MPCOF_YUV420,
	E_MPCOF_JPEG,
	E_MPCOF_UNKNOWN
}E_Mex_Phs_Camera_OutPut_Format;
#endif

typedef enum
{
	E_MPCR_0,
	E_MPCR_90,
	E_MPCR_180,
	E_MPCR_270
}E_Mex_Phs_Camera_Rotate;


//typedef for hook message
#define MEX_H_PHS_HK_MAX_MSG_NUM	8
#define MEX_H_PHS_HK_MAX_FUN_NUM	8

#define MEX_H_PHS_HK_UNSPEC_MOD		0xFFFF

typedef uint8 (*Mex_Hk_Func_Ptr)(void* MsgStruct, void* peerBuff);

//Message Id for hook
typedef enum
{
    E_MHMI_MSG_ID_INVALID                   = 0,      //��ϢID 0��ʾΪ��Ч����Ϣ
	E_MHMI_MSG_ID_SMS_DELIVER_MSG_IND       = 1,      //�����ŵ���ϢId
	E_MHMI_MSG_ID_CC_CALL_RING_IND          = 2,      //���绰����ϢId
	E_MHMI_MSG_ID_WILL_PLAY_ALARMTONE       = 3,      //��Ҫ����������

	E_MHMI_MSG_ID_MAX
} E_Mex_Hk_Msg_Id;

#if MEX_SDK_VER >= 1009
//Common Hook Result
#define MEX_H_PHS_HK_RES_COMMON_IGNORE           0x00
#define MEX_H_PHS_HK_RES_COMMON_HIJACK           0x01

//SMS Hook Result
#define MEX_H_PHS_HK_RES_SMS_HIDE_MSG_IND        0x02
#define MEX_H_PHS_HK_RES_SMS_HIJACK_AND_HIDE     (MEX_H_PHS_HK_RES_COMMON_HIJACK | MEX_H_PHS_HK_RES_SMS_HIDE_MSG_IND)

#endif /* MEX_SDK_VER */

//dcs enum
typedef enum
{
	E_MHSD_DCS_DEFAULT        = 0x00,  /* GSM 7-bit */
	E_MHSD_DCS_8BIT           = 0x04,  /* 8-bit */
	E_MHSD_DCS_UCS2           = 0x08,  /* UCS2 Big-Endian */
	E_MHSD_DCS_RESERVED       = 0x0c,  /* reserved alphabet,
									   currently, MMI shall display "not support alphabet"
									   or "cannot display" when receive dcs indicated this value */

									   E_MHSD_DCS_MAX                     // Invalid
} E_Mex_Hk_Sms_Dcs;

//MsgStruct for E_MHMI_MSG_ID_SMS_DELIVER_MSG_IND
typedef struct
{
	Mex_Phs_Phb_Number  sca_number;           // �������ĺ���
	Mex_Phs_Phb_Number  oa_number;            // ���ź���
	uint8   pid;                              // Э���ʶ
	uint8   dcs;                              // ���ű��뷽ʽ
	uint8   ori_dcs;                          // ԭʼ���뷽ʽ, �ɺ���
	uint16  index;                            // ��������
	uint16  dest_port;                        // Ŀ��˿�, �ɺ���
	uint16  src_port;                         // Դ�˿�, �ɺ���
	uint8   mti;                              // ��Ϣ���ͱ�ʶ
	uint8   display_type;                     // ��ʾ��ʽ
	uint8   storage_type;                     // �洢��ʽ
	uint32  concat_info;                      // Unknown
	uint16  no_msg_data;                      // �������ݳ���(memory size in bytes), ������������, ���ֽڼ���
	uint8  *msg_data;                         // ��������
} Mex_Hk_Sms_Deliver_Msg;

//MsgStruct for E_MHMI_MSG_ID_CC_CALL_RING_IND
typedef struct
{
	uint8   call_id;                          // ������
	Mex_Phs_Phb_Number  num;                  // �������
	Mex_Phs_Phb_Number  sub_addr;             // �Ӻ���
	Mex_Phs_Phb_Number  redirect_num;         // ת�Ӻ���
	uint8   name_present;                     // �Ƿ����������Ϣ
	uint8   cnap_info_exist;                  // Unknown
	uint8   tag;                              // Unknown
	uint8   name[30];                         // ����
	uint8   dcs;                              // ���ݱ��뷽ʽ
	uint8   length;                           // ���ݳ���, ���ֽڼ���
	uint8   auto_answer;                      // �Զ�����
	uint8   call_type;                        // ��������
} Mex_Hk_Cc_Call_Ring;

typedef struct
{
	bool state;
	uint16 index_mgr;
	uint16	index_func;
} Mex_Hk_Info, *Mex_Hk_Info_Ptr;

#define MAX_CALLLOG_NUMBER_LENGTH 41
#define MAX_CALLLOG_PBNAME_LENGTH 41


typedef enum
{
	E_MCS_CALLLOG_READY,            // ��׼����
	E_MCS_CALLLOG_BUSY,             // ϵͳ��æ
	E_MCS_CALLLOG_NEED_UPDATE,      // ��Ҫ����
	E_MCS_CALLLOG_UPDATING,         // ���ڸ�����
	E_MCS_CALLLOG_NOT_SUPPORTED,    // ��֧��

	E_MCS_CALLLOG_MAX
} E_MEX_CALLOG_STATUS;


typedef enum
{
	E_MCT_CALLLOG_DIALED,           // �Ѱε绰
	E_MCT_CALLLOG_MISSED,           // δ������
	E_MCT_CALLLOG_RECVD,            // �ѽ�����
	E_MCT_CALLLOG_MIXED,            // ȫ��

#if MEX_SDK_VER >= 1002
	E_MCT_CALLLOG_MAX
#endif /* MEX_SDK_VER */
} E_MEX_CALLLOG_TYPE;


// CallLog ����
typedef struct
{
	wchar_t  number[MAX_CALLLOG_NUMBER_LENGTH+1];    // �绰����
	uint32   numberLen;                              // �绰���볤��
	wchar_t  pbName[MAX_CALLLOG_PBNAME_LENGTH+1];    // ����
	uint32   pbNameLen;                              // ��������

	uint32       nTimes;         // ��¼����
	Mex_DateTime startTime;      // ��ʼʱ��
	Mex_DateTime duration;       // ͨ��ʱ��
} Mex_CallLog_Info;

//JPGͼƬѹ������
typedef enum
{
	E_MEX_JPG_QTY_LOW,
	E_MEX_JPG_QTY_NORMAL,
	E_MEX_JPG_QTY_HIGH,
	E_MEX_JPG_QTY_MAX
}E_Mex_JPG_Encode_Qty;


#if MEX_SDK_VER >= 1002

// E_MEX_WallPaper_Type
typedef enum
{
	E_MWPT_WallPaper     = 0x0001, // wall paper on main lcd
	E_MWPT_SubWallPaper  = 0x0002, // wall paper on sub lcd
	E_MWPT_ScreenSaver   = 0x0004, // screen saver on main lcd
	E_MWPT_PowerOn       = 0x0008, // wall paper when power on
	E_MWPT_PowerOff      = 0x0010, // wall paper when power off

	E_MWPT_MAX                     // invalid
} E_MEX_WallPaper_Type;

#endif /* MEX_SDK_VER */
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_SDK_VER >= 1013

typedef void(*camera_cap_callback)(void);
typedef struct
{
	int32 appId;
	camera_cap_callback data_cb;
}Mex_Camera_Cap_Req;


typedef enum
{
	E_MPAT_ALERT_NONE,
	E_MPAT_RING,					//����
	E_MPAT_VIBRATION_ONLY,			//ֻ��
	E_MPAT_VIBRATION_AND_RING,		//������
	E_MPAT_VIBRATION_THEN_RING,		//�𶯺�����
	E_MPAT_SILENT,					//����
}E_Mex_Pro_Alert_Type;
#endif
#endif

#if MEX_SDK_VER >= 1033
typedef enum
{
	E_MMSD_IDLE,
	E_MMSD_UP,
	E_MMSD_DOWN,
	E_MMSD_LEFT,
	E_MMSD_RIGHT,
}E_Mex_MotionSensor_Dir;
typedef void(*MexLib_MotionSensor_CallBack)(E_Mex_MotionSensor_Dir dir_result);
#endif


//apis for phone status
MEX_H_DECLARE_FUNC( void,						Phs_ReFresh_Imei,			(void) )
MEX_H_DECLARE_FUNC( void,						Phs_ReFresh_Imsi,			(E_MEX_Sim_Card_ID sim_id) )
MEX_H_DECLARE_FUNC( void,						Phs_ReFresh_SmsCent,		(E_MEX_Sim_Card_ID sim_id) )

MEX_H_DECLARE_FUNC( void,						Phs_Set_Backlight,			(E_MEX_Phs_HW_State sate) )
MEX_H_DECLARE_FUNC( void,						Phs_Set_Vibrator,			(E_MEX_Phs_HW_State sate) )

//api for call
MEX_H_DECLARE_FUNC( bool,						Phs_MakePhoneCall,			(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber) )
MEX_H_DECLARE_FUNC( bool,						Phs_IncomingCall,			(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber) )

// APIs for sms send
MEX_H_DECLARE_FUNC( MEX_PHS_SMS_HANDLE,         Phs_SmsSendStart,           (Mex_Phs_Sms_Send_Req *req) )
MEX_H_DECLARE_FUNC( bool,                       Phs_SmsSendRetry,           (MEX_PHS_SMS_HANDLE handle) )
MEX_H_DECLARE_FUNC( bool,                       Phs_SmsSendFinish,          (MEX_PHS_SMS_HANDLE handle) )
//apis for SMS
MEX_H_DECLARE_FUNC( bool,						Phs_NewSms,					(E_MEX_Sim_Card_ID eSimId, const char* pSmsCenter, const char* pSmsNumber, const wchar_t *pSmsContent, const Mex_DateTime *pDt) )

//apis for phone book
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_GetPhbEntryIndex,		(uint16** ppuStoreIndex, uint16* puEntryCount) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_SearchPhbEntry,			(wchar_t* szKeyword, bool bIsPinyin, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask, uint16* puResultCount) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_GetPhbEntry,            (uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_GetPhbEntryEx,          (uint16 uIndex, Mex_Phs_Phb_Entry* psEntry))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_GetPhbMemoryStatus,     (E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_AddPhbEntry,			(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_AddPhbEntryEx,			(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* sEntry))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_UpdatePhbEntry,			(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_UpdatePhbEntryEx,		(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_RemovePhbEntry,         (uint16 uIndex))
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_GetPhbGroup,            (uint16 groupIndex, Mex_Phs_Phb_Group *pGroup) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_SetPhbCB,               (Mex_Phs_Phb_CB pCallBack) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_ClearPhbCB,             (void) )


//apis for hook message
MEX_H_DECLARE_FUNC( bool,						Phs_SetHookMsg,				(E_Mex_Hk_Msg_Id eMsgId, E_MEX_Sim_Card_ID eSimId, Mex_Hk_Func_Ptr hkFuncPtr, Mex_Hk_Info_Ptr p) )
MEX_H_DECLARE_FUNC( bool,						Phs_UnhookMsg,				(Mex_Hk_Info_Ptr p) )

MEX_H_DECLARE_FUNC( int8,						Phs_Start_Camera,			(int32 offset_x, int32 offset_y, int32 iImgWidth, int32 iImgHeight, int32 iDispWidth, int32 iDispHeight, void *buffer,int32 buf_size) )
MEX_H_DECLARE_FUNC( int8,						Phs_Start_Preview,			(void) )
MEX_H_DECLARE_FUNC( int8,						Phs_Stop_Preview,			(void) )
MEX_H_DECLARE_FUNC( int8,						Phs_Stop_Camera,			(void) )
MEX_H_DECLARE_FUNC( int8,						Phs_Camera_Capture_To_Mem,	(void **buffer,uint32 *captured_size) )
MEX_H_DECLARE_FUNC( void,						Phs_Camera_Zoom_In,			(void) )
MEX_H_DECLARE_FUNC( void,						Phs_Camera_Zoom_Out,		(void) )
MEX_H_DECLARE_FUNC( E_Mex_Phs_Camera_Rotate,	Phs_Camera_Get_Rotate,		(void) )
MEX_H_DECLARE_FUNC( void *,						Phs_Camera_Get_Buffer,		(void) )
MEX_H_DECLARE_FUNC( void,						Phs_Snapshot_To_Jpg,		(wchar_t *fileName) )

// APIs for call log (Call History)
MEX_H_DECLARE_FUNC(uint32,                      Phs_GetNumOfCallLog,        (E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType) )
MEX_H_DECLARE_FUNC(bool,                        Phs_GetCallLog,             (E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog) )
MEX_H_DECLARE_FUNC(E_MEX_CALLOG_STATUS,         Phs_GetCallLogStatus,       (E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType) )
MEX_H_DECLARE_FUNC(bool,                        Phs_UpdateCallLog,          (E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType) )
MEX_H_DECLARE_FUNC(bool,						PhsSetDateTime,				(Mex_DateTime pdt) )

MEX_H_DECLARE_FUNC(void,						PhsConnectWap,				(char *) )
// LookUpNumber
MEX_H_DECLARE_FUNC( E_Mex_Phs_Phb_Result,       Phs_LookUpPhbNumber,        (const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName) )
// Outgoing call
MEX_H_DECLARE_FUNC( MEX_OUTCALL_HANDLE,         Phs_OutCallStart,           (E_MEX_Sim_Card_ID eSimId, const char* number, MexOutCallCB callback) )
MEX_H_DECLARE_FUNC( bool,                       Phs_OutCallFinish,          (MEX_OUTCALL_HANDLE handle) )
MEX_H_DECLARE_FUNC( bool,                       Phs_OutCallDtmfBegin,       (MEX_OUTCALL_HANDLE handle, char keycode) )
MEX_H_DECLARE_FUNC( bool,                       Phs_OutCallDtmfEnd,         (MEX_OUTCALL_HANDLE handle) )
// Sms
MEX_H_DECLARE_FUNC( bool,                       Phs_SmsReady,               (void) )
MEX_H_DECLARE_FUNC( uint16,                     Phs_GetNumOfSms,            (E_MEX_PHS_SMS_MSGBOX msgBox) )
MEX_H_DECLARE_FUNC( bool,                       Phs_ReadSmsContent,         (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Read_Sms_CB callback) )
MEX_H_DECLARE_FUNC( char *,                     Phs_GetSmsAddr,             (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index) )
MEX_H_DECLARE_FUNC( bool,                       Phs_GetSmsDateTime,         (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_DateTime *pDt) )
MEX_H_DECLARE_FUNC( bool,                       Phs_GetSmsCache,            (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, wchar_t *pContent, uint16 iContentMemLen) )
MEX_H_DECLARE_FUNC( E_MEX_Sim_Card_ID,          Phs_GetSmsSimId,            (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index) )
MEX_H_DECLARE_FUNC( bool,                       Phs_DelSms,                 (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Del_Sms_CB callback) )
MEX_H_DECLARE_FUNC( void,                       Phs_CancelSmsCB,            (void) )
MEX_H_DECLARE_FUNC( bool,                       Phs_SaveSms,                (E_MEX_PHS_SMS_MSGBOX msgBox, E_MEX_Sim_Card_ID simId, const wchar_t *pContent, const char *pAddr, Mex_Phs_Save_Sms_CB callback) )
MEX_H_DECLARE_FUNC( bool,                       Phs_SmsMsgBoxExist,         (E_MEX_PHS_SMS_MSGBOX msgBox) )
MEX_H_DECLARE_FUNC( bool,                       Phs_SmsIsUnread,            (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index) )

#if MEX_SDK_VER >= 1002
MEX_H_DECLARE_FUNC( bool,						Phs_EncodeToJpeg,			(void *image_ptr,int32 image_width,int32 image_height, int8 jpg_qty, wchar_t *file_name,bool bAbsPath) )
MEX_H_DECLARE_FUNC( bool,                       Phs_PhnSetWallPaper,        (E_MEX_WallPaper_Type eType, wchar_t *pFileName, bool bAbsPath) )

#endif
#if MEX_SDK_VER >= 1006
MEX_H_DECLARE_FUNC( int8,						Phs_Start_Camera_Ext,		(int32 offset_x, int32 offset_y, int32 iDispWidth, int32 iDispHeight, bool bPreviewUseSram, int8 outputformat) )
MEX_H_DECLARE_FUNC( int8,						Phs_Start_Preview_Ext,		(void) )
MEX_H_DECLARE_FUNC( int8,						Phs_Stop_Preview_Ext,		(void) )
MEX_H_DECLARE_FUNC( int8,						Phs_Stop_Camera_Ext,		(void) )
MEX_H_DECLARE_FUNC( void*,						Phs_GetCameraOutPutData, 	(void) )
MEX_H_DECLARE_FUNC( void,						Phs_SetCameraNightMode,  	(bool) )
#endif

#if MEX_SDK_VER >= 1008
MEX_H_DECLARE_FUNC( bool,						Phs_EncodeToJpegBuffer,		(void *image_ptr,int32 image_width,int32 image_height,int8 jpg_qty,void *buffer,uint32 buffer_size,uint32 *jpg_size) )
#endif
#if MEX_SDK_VER >= 1009
MEX_H_DECLARE_FUNC( uint8,                      Phs_GetHookResult,			(void) )
MEX_H_DECLARE_FUNC( void,						Phs_SetPhoneProfile,		(int32 profileId) )
#endif
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_SDK_VER >= 1013
MEX_H_DECLARE_FUNC( void,						Phs_Camera_Capture_Req,			(Mex_Camera_Cap_Req *req) )
MEX_H_DECLARE_FUNC( int8,						Phs_Get_CurrProfile_AlertType,	(void) )
MEX_H_DECLARE_FUNC(	void,						Phs_SwitchCamera,				(bool) )

#endif
#endif
#if MEX_SDK_VER >= 1022
MEX_H_DECLARE_FUNC(	int32,						Phs_Aq_getCity,				    (char *pNumber, wchar_t *CityName) )
MEX_H_DECLARE_FUNC(	int32,						Phs_Aq_DivertCall,				(char *pNumber, i_Callback_i callback) )
MEX_H_DECLARE_FUNC(	int32,						Phs_Aq_CancelDivert,			(i_Callback_i callback) )
MEX_H_DECLARE_FUNC(	int32,						Phs_Aq_QueryDivert,				(i_Callback_i callback) )
#endif
#if MEX_SDK_VER >= 1026
MEX_H_DECLARE_FUNC( void*,						Phs_GetCameraOutPutData_Ext, 	(uint32 *puiSize) )
#endif
#if MEX_SDK_VER >= 1027
MEX_H_DECLARE_FUNC(uint32,                      Phs_GetMissedCallNumber,    (void) )
MEX_H_DECLARE_FUNC(bool,						Phs_ClearMissedCallNum,     (void) )
MEX_H_DECLARE_FUNC( void,						Phs_Set_Can_PowerOff, 	(bool IsCanPowerOff))
#endif
#if MEX_SDK_VER >= 1029
MEX_H_DECLARE_FUNC( E_MEX_Phs_HW_State,		Phs_Get_Backlight_State,	(void))
#endif
#if MEX_SDK_VER >= 1030
MEX_H_DECLARE_FUNC(bool,						Phs_ClearUnReadSms,     (void) )
#endif
#if MEX_SDK_VER >= 1033
MEX_H_DECLARE_FUNC(void,						Phs_MotionSensor_Start,     (MexLib_MotionSensor_CallBack callback,int16 x_ponit,int16 y_point,int16 motionSensitivity) )
MEX_H_DECLARE_FUNC(void,						Phs_MotionSensor_Stop, 	(void) )
#endif

MEX_H_ENUM_FUNC_START_PHS
MEX_H_ENUM_FUNC_PHS( Phs_ReFresh_Imei )
MEX_H_ENUM_FUNC_PHS( Phs_ReFresh_Imsi )
MEX_H_ENUM_FUNC_PHS( Phs_ReFresh_SmsCent )
MEX_H_ENUM_FUNC_PHS( Phs_Set_Backlight )
MEX_H_ENUM_FUNC_PHS( Phs_Set_Vibrator )
//Call
MEX_H_ENUM_FUNC_PHS( Phs_MakePhoneCall )
MEX_H_ENUM_FUNC_PHS( Phs_IncomingCall )
// APIs for sms send
MEX_H_ENUM_FUNC_PHS( Phs_SmsSendStart )
MEX_H_ENUM_FUNC_PHS( Phs_SmsSendRetry )
MEX_H_ENUM_FUNC_PHS( Phs_SmsSendFinish )
//SMS
MEX_H_ENUM_FUNC_PHS( Phs_NewSms )
//Phone book
MEX_H_ENUM_FUNC_PHS( Phs_GetPhbEntryIndex )
MEX_H_ENUM_FUNC_PHS( Phs_SearchPhbEntry )
MEX_H_ENUM_FUNC_PHS( Phs_GetPhbEntry )
MEX_H_ENUM_FUNC_PHS( Phs_GetPhbEntryEx )
MEX_H_ENUM_FUNC_PHS( Phs_GetPhbMemoryStatus )
MEX_H_ENUM_FUNC_PHS( Phs_AddPhbEntry )
MEX_H_ENUM_FUNC_PHS( Phs_AddPhbEntryEx )
MEX_H_ENUM_FUNC_PHS( Phs_UpdatePhbEntry )
MEX_H_ENUM_FUNC_PHS( Phs_UpdatePhbEntryEx )
MEX_H_ENUM_FUNC_PHS( Phs_RemovePhbEntry )
MEX_H_ENUM_FUNC_PHS( Phs_GetPhbGroup )
MEX_H_ENUM_FUNC_PHS( Phs_SetPhbCB )
MEX_H_ENUM_FUNC_PHS( Phs_ClearPhbCB )

//Hook
MEX_H_ENUM_FUNC_PHS( Phs_SetHookMsg )
MEX_H_ENUM_FUNC_PHS( Phs_UnhookMsg )

MEX_H_ENUM_FUNC_PHS( Phs_Start_Camera )
MEX_H_ENUM_FUNC_PHS( Phs_Start_Preview )
MEX_H_ENUM_FUNC_PHS( Phs_Stop_Preview )
MEX_H_ENUM_FUNC_PHS( Phs_Stop_Camera )
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Capture_To_Mem )
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Zoom_In )
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Zoom_Out )
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Get_Rotate )
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Get_Buffer )
MEX_H_ENUM_FUNC_PHS( Phs_Snapshot_To_Jpg )

// APIs for call log (Call History)
MEX_H_ENUM_FUNC_PHS( Phs_GetNumOfCallLog )
MEX_H_ENUM_FUNC_PHS( Phs_GetCallLog )
MEX_H_ENUM_FUNC_PHS( Phs_GetCallLogStatus )
MEX_H_ENUM_FUNC_PHS( Phs_UpdateCallLog )
MEX_H_ENUM_FUNC_PHS( PhsSetDateTime )

MEX_H_ENUM_FUNC_PHS( PhsConnectWap )
// LookUpNumber
MEX_H_ENUM_FUNC_PHS( Phs_LookUpPhbNumber )
// Outgoing call
MEX_H_ENUM_FUNC_PHS( Phs_OutCallStart )
MEX_H_ENUM_FUNC_PHS( Phs_OutCallFinish )
MEX_H_ENUM_FUNC_PHS( Phs_OutCallDtmfBegin )
MEX_H_ENUM_FUNC_PHS( Phs_OutCallDtmfEnd )
// Sms
MEX_H_ENUM_FUNC_PHS( Phs_SmsReady )
MEX_H_ENUM_FUNC_PHS( Phs_GetNumOfSms )
MEX_H_ENUM_FUNC_PHS( Phs_ReadSmsContent )
MEX_H_ENUM_FUNC_PHS( Phs_GetSmsAddr )
MEX_H_ENUM_FUNC_PHS( Phs_GetSmsDateTime )
MEX_H_ENUM_FUNC_PHS( Phs_GetSmsCache )
MEX_H_ENUM_FUNC_PHS( Phs_GetSmsSimId )
MEX_H_ENUM_FUNC_PHS( Phs_DelSms )
MEX_H_ENUM_FUNC_PHS( Phs_CancelSmsCB )
MEX_H_ENUM_FUNC_PHS( Phs_SaveSms )
MEX_H_ENUM_FUNC_PHS( Phs_SmsMsgBoxExist )
MEX_H_ENUM_FUNC_PHS( Phs_SmsIsUnread )

#if MEX_SDK_VER >= 1002
MEX_H_ENUM_FUNC_PHS( Phs_EncodeToJpeg )
MEX_H_ENUM_FUNC_PHS( Phs_PhnSetWallPaper )

#endif

#if MEX_SDK_VER >= 1006
MEX_H_ENUM_FUNC_PHS(Phs_Start_Camera_Ext )
MEX_H_ENUM_FUNC_PHS(Phs_Start_Preview_Ext )
MEX_H_ENUM_FUNC_PHS(Phs_Stop_Preview_Ext )
MEX_H_ENUM_FUNC_PHS(Phs_Stop_Camera_Ext )
MEX_H_ENUM_FUNC_PHS(Phs_GetCameraOutPutData )
MEX_H_ENUM_FUNC_PHS(Phs_SetCameraNightMode )
#endif
#if MEX_SDK_VER >= 1008
MEX_H_ENUM_FUNC_PHS( Phs_EncodeToJpegBuffer )
#endif

#if MEX_SDK_VER >= 1009
MEX_H_ENUM_FUNC_PHS( Phs_GetHookResult )
MEX_H_ENUM_FUNC_PHS( Phs_SetPhoneProfile )
#endif
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_SDK_VER >= 1013
MEX_H_ENUM_FUNC_PHS( Phs_Camera_Capture_Req )
MEX_H_ENUM_FUNC_PHS( Phs_Get_CurrProfile_AlertType )
MEX_H_ENUM_FUNC_PHS( Phs_SwitchCamera )
#endif
#endif
#if MEX_SDK_VER >= 1022
MEX_H_ENUM_FUNC_PHS( Phs_Aq_getCity )
MEX_H_ENUM_FUNC_PHS( Phs_Aq_DivertCall )
MEX_H_ENUM_FUNC_PHS( Phs_Aq_CancelDivert )
MEX_H_ENUM_FUNC_PHS( Phs_Aq_QueryDivert )
#endif
#if MEX_SDK_VER >= 1026
MEX_H_ENUM_FUNC_PHS( Phs_GetCameraOutPutData_Ext )
#endif
#if MEX_SDK_VER >= 1027
MEX_H_ENUM_FUNC_PHS( Phs_GetMissedCallNumber )
MEX_H_ENUM_FUNC_PHS( Phs_ClearMissedCallNum )
MEX_H_ENUM_FUNC_PHS( Phs_Set_Can_PowerOff )
#endif
#if MEX_SDK_VER >= 1029
MEX_H_ENUM_FUNC_PHS( Phs_Get_Backlight_State )
#endif
#if MEX_SDK_VER >= 1030
MEX_H_ENUM_FUNC_PHS( Phs_ClearUnReadSms )
#endif
#if MEX_SDK_VER >= 1033
MEX_H_ENUM_FUNC_PHS( Phs_MotionSensor_Start )
MEX_H_ENUM_FUNC_PHS( Phs_MotionSensor_Stop )
#endif

MEX_H_ENUM_FUNC_END_PHS


#if defined (MEX_TARGET_CLIENT)
#define Phs_ReFresh_Imei			MEX_H_GET_FUNC_DEFINE_PHS( Phs_ReFresh_Imei )
#define Phs_ReFresh_Imsi			MEX_H_GET_FUNC_DEFINE_PHS( Phs_ReFresh_Imsi )
#define	Phs_ReFresh_SmsCent			MEX_H_GET_FUNC_DEFINE_PHS( Phs_ReFresh_SmsCent )
#define	Phs_Set_Backlight			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Set_Backlight )
#define	Phs_Set_Vibrator			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Set_Vibrator )
//api for call
#define Phs_MakePhoneCall			MEX_H_GET_FUNC_DEFINE_PHS( Phs_MakePhoneCall )
#define Phs_IncomingCall			MEX_H_GET_FUNC_DEFINE_PHS( Phs_IncomingCall )
// APIs for sms send
#define Phs_SmsSendStart            MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsSendStart )
#define Phs_SmsSendRetry            MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsSendRetry )
#define Phs_SmsSendFinish           MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsSendFinish )
//apis for SMS
#define Phs_NewSms					MEX_H_GET_FUNC_DEFINE_PHS( Phs_NewSms )
//apis for phone book
#define Phs_GetPhbEntryIndex		MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetPhbEntryIndex )
#define Phs_SearchPhbEntry			MEX_H_GET_FUNC_DEFINE_PHS( Phs_SearchPhbEntry )
#define Phs_GetPhbEntry             MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetPhbEntry )
#define Phs_GetPhbEntryEx           MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetPhbEntryEx )
#define Phs_GetPhbMemoryStatus      MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetPhbMemoryStatus )
#define Phs_AddPhbEntry				MEX_H_GET_FUNC_DEFINE_PHS( Phs_AddPhbEntry )
#define Phs_AddPhbEntryEx			MEX_H_GET_FUNC_DEFINE_PHS( Phs_AddPhbEntryEx )
#define Phs_UpdatePhbEntry			MEX_H_GET_FUNC_DEFINE_PHS( Phs_UpdatePhbEntry )
#define Phs_UpdatePhbEntryEx		MEX_H_GET_FUNC_DEFINE_PHS( Phs_UpdatePhbEntryEx )
#define Phs_RemovePhbEntry          MEX_H_GET_FUNC_DEFINE_PHS( Phs_RemovePhbEntry )
#define Phs_GetPhbGroup             MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetPhbGroup )
#define Phs_SetPhbCB                MEX_H_GET_FUNC_DEFINE_PHS( Phs_SetPhbCB )
#define Phs_ClearPhbCB              MEX_H_GET_FUNC_DEFINE_PHS( Phs_ClearPhbCB )

//apis for hook message
#define Phs_SetHookMsg				MEX_H_GET_FUNC_DEFINE_PHS( Phs_SetHookMsg )
#define Phs_UnhookMsg				MEX_H_GET_FUNC_DEFINE_PHS( Phs_UnhookMsg )

#define Phs_Start_Camera			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Start_Camera )
#define Phs_Start_Preview			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Start_Preview )
#define Phs_Stop_Preview			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Stop_Preview )
#define Phs_Stop_Camera				MEX_H_GET_FUNC_DEFINE_PHS( Phs_Stop_Camera )
#define Phs_Camera_Capture_To_Mem	MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Capture_To_Mem )
#define Phs_Camera_Zoom_In			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Zoom_In )
#define Phs_Camera_Zoom_Out			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Zoom_Out )
#define Phs_Camera_Get_Rotate		MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Get_Rotate )
#define Phs_Camera_Get_Buffer		MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Get_Buffer )
#define Phs_Snapshot_To_Jpg		MEX_H_GET_FUNC_DEFINE_PHS( Phs_Snapshot_To_Jpg )

// APIs for call log (Call History)
#define Phs_GetNumOfCallLog         MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetNumOfCallLog )
#define Phs_GetCallLog              MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetCallLog )
#define Phs_GetCallLogStatus        MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetCallLogStatus )
#define Phs_UpdateCallLog           MEX_H_GET_FUNC_DEFINE_PHS( Phs_UpdateCallLog )

#define PhsSetDateTime				MEX_H_GET_FUNC_DEFINE_PHS( PhsSetDateTime )

#define PhsConnectWap				MEX_H_GET_FUNC_DEFINE_PHS( PhsConnectWap )
// LookUpNumber
#define Phs_LookUpPhbNumber         MEX_H_GET_FUNC_DEFINE_PHS( Phs_LookUpPhbNumber )
// Outgoing call
#define Phs_OutCallStart            MEX_H_GET_FUNC_DEFINE_PHS( Phs_OutCallStart )
#define Phs_OutCallFinish           MEX_H_GET_FUNC_DEFINE_PHS( Phs_OutCallFinish )
#define Phs_OutCallDtmfBegin        MEX_H_GET_FUNC_DEFINE_PHS( Phs_OutCallDtmfBegin )
#define Phs_OutCallDtmfEnd          MEX_H_GET_FUNC_DEFINE_PHS( Phs_OutCallDtmfEnd )
// Sms
#define Phs_SmsReady                MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsReady )
#define Phs_GetNumOfSms             MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetNumOfSms )
#define Phs_ReadSmsContent          MEX_H_GET_FUNC_DEFINE_PHS( Phs_ReadSmsContent )
#define Phs_GetSmsAddr              MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetSmsAddr )
#define Phs_GetSmsDateTime          MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetSmsDateTime )
#define Phs_GetSmsCache             MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetSmsCache )
#define Phs_GetSmsSimId             MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetSmsSimId )
#define Phs_DelSms                  MEX_H_GET_FUNC_DEFINE_PHS( Phs_DelSms )
#define Phs_CancelSmsCB             MEX_H_GET_FUNC_DEFINE_PHS( Phs_CancelSmsCB )
#define Phs_SaveSms                 MEX_H_GET_FUNC_DEFINE_PHS( Phs_SaveSms )
#define Phs_SmsMsgBoxExist          MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsMsgBoxExist )
#define Phs_SmsIsUnread             MEX_H_GET_FUNC_DEFINE_PHS( Phs_SmsIsUnread )

#if MEX_SDK_VER >= 1002
#define Phs_EncodeToJpeg			MEX_H_GET_FUNC_DEFINE_PHS( Phs_EncodeToJpeg )
#define Phs_PhnSetWallPaper			MEX_H_GET_FUNC_DEFINE_PHS( Phs_PhnSetWallPaper )

#endif

#if MEX_SDK_VER >= 1006
#define Phs_Start_Camera_Ext			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Start_Camera_Ext )
#define Phs_Start_Preview_Ext			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Start_Preview_Ext )
#define Phs_Stop_Preview_Ext			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Stop_Preview_Ext )
#define Phs_Stop_Camera_Ext			MEX_H_GET_FUNC_DEFINE_PHS( Phs_Stop_Camera_Ext )
#define Phs_GetCameraOutPutData			MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetCameraOutPutData )
#define Phs_SetCameraNightMode			MEX_H_GET_FUNC_DEFINE_PHS( Phs_SetCameraNightMode )
#endif

#if MEX_SDK_VER >= 1008
#define Phs_EncodeToJpegBuffer			MEX_H_GET_FUNC_DEFINE_PHS( Phs_EncodeToJpegBuffer )
#endif
#if MEX_SDK_VER >= 1009
#define Phs_GetHookResult               MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetHookResult )
#define Phs_SetPhoneProfile				MEX_H_GET_FUNC_DEFINE_PHS( Phs_SetPhoneProfile )
#endif

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#if MEX_SDK_VER >= 1013
#define Phs_Camera_Capture_Req				MEX_H_GET_FUNC_DEFINE_PHS( Phs_Camera_Capture_Req )
#define Phs_Get_CurrProfile_AlertType   	MEX_H_GET_FUNC_DEFINE_PHS( Phs_Get_CurrProfile_AlertType )
#define Phs_SwitchCamera					MEX_H_GET_FUNC_DEFINE_PHS( Phs_SwitchCamera )
#endif
#endif
#if MEX_SDK_VER >= 1022
#define Phs_Aq_getCity					    MEX_H_GET_FUNC_DEFINE_PHS( Phs_Aq_getCity )
#define Phs_Aq_DivertCall					MEX_H_GET_FUNC_DEFINE_PHS( Phs_Aq_DivertCall )
#define Phs_Aq_CancelDivert					MEX_H_GET_FUNC_DEFINE_PHS( Phs_Aq_CancelDivert )
#define Phs_Aq_QueryDivert					MEX_H_GET_FUNC_DEFINE_PHS( Phs_Aq_QueryDivert )
#endif
#if MEX_SDK_VER >= 1026
#define Phs_GetCameraOutPutData_Ext			MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetCameraOutPutData_Ext )
#endif
#if MEX_SDK_VER >= 1027
#define Phs_GetMissedCallNumber     MEX_H_GET_FUNC_DEFINE_PHS( Phs_GetMissedCallNumber )
#define Phs_ClearMissedCallNum		MEX_H_GET_FUNC_DEFINE_PHS( Phs_ClearMissedCallNum )
#define Phs_Set_Can_PowerOff		MEX_H_GET_FUNC_DEFINE_PHS( Phs_Set_Can_PowerOff )
#endif
#if MEX_SDK_VER >= 1029
#define Phs_Get_Backlight_State		MEX_H_GET_FUNC_DEFINE_PHS( Phs_Get_Backlight_State )
#endif
#if MEX_SDK_VER >= 1030
#define Phs_ClearUnReadSms		MEX_H_GET_FUNC_DEFINE_PHS( Phs_ClearUnReadSms )
#endif
#if MEX_SDK_VER >= 1033
#define Phs_MotionSensor_Start		MEX_H_GET_FUNC_DEFINE_PHS( Phs_MotionSensor_Start )
#define Phs_MotionSensor_Stop		MEX_H_GET_FUNC_DEFINE_PHS( Phs_MotionSensor_Stop )
#endif
#endif


//write comments below for VA to display them
#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)

//ˢ��Imei��Ϣ��ϵͳ����һ��ʱ���ˢ��Imei��Ϣ
//���� sim_id��ָ��sim�� �ο�ö��E_MEX_Sim_Card_ID
//���� ��
//ע�⣺�첽���������øú����󣬲���������ȥˢ�£���������ʵʱ�Խϸߵĳ���
void Phs_ReFresh_Imei (void);

//ˢ�»�վ��Ϣ��ϵͳ����һ��ʱ���ˢ�»�վ��Ϣ
//���� sim_id��ָ��sim�� �ο�ö��E_MEX_Sim_Card_ID
//���� ��
//ע�⣺�첽���������øú����󣬲���������ȥˢ�£���������ʵʱ�Խϸߵĳ���
void Phs_ReFresh_Imsi (E_MEX_Sim_Card_ID sim_id);

//ˢ�»�վ��Ϣ��ϵͳ����һ��ʱ���ˢ�»�վ��Ϣ
//���� sim_id��ָ��sim�� �ο�ö��E_MEX_Sim_Card_ID
//���� ��
//ע�⣺�첽���������øú����󣬲���������ȥˢ�£���������ʵʱ�Խϸߵĳ���
void Phs_ReFresh_SmsCent (E_MEX_Sim_Card_ID sim_id);


//�����ֻ������״̬
//���� state �ο�ö�� E_MEX_Phs_HW_State
//����ֵ ��
void Phs_Set_Backlight(E_MEX_Phs_HW_State state);

//�����ֻ���״̬
//���� state �ο�ö�� E_MEX_Phs_HW_State
//����ֵ ��
void Phs_Set_Vibrator(E_MEX_Phs_HW_State state);

//api for call
/**************************************************************************
* �������ƣ� Phs_MakePhoneCall
* ���������� ����绰
* ����˵���� [IN:eSimId] SIM��
* ����˵���� [IN:wszNumber]	����ĵ绰����
* �� �� ֵ�� bool �Ƿ�ɹ�
* ����˵���� Ӧ�ó���������Ȩ��E_MAPT_MakeCall
**************************************************************************/
bool Phs_MakePhoneCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber);

/**************************************************************************
* �������ƣ� Phs_IncomingCall
* ���������� ����
* ����˵���� [IN:eSimId] SIM��
* ����˵���� [IN:wszNumber]	�������
* �� �� ֵ�� bool �Ƿ�ɹ�
* ����˵���� ��
**************************************************************************/
bool Phs_IncomingCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber);

/**************************************************************************
* �������ƣ� Phs_SmsSendStart
* ���������� ���Ͷ��ſ�ʼ
* ����˵���� [IN] req ���Ͷ�������ṹ��ָ��
* �� �� ֵ�� ���Ͷ��ŵľ����ʧ��ʱ���ؾ�� NULL
* ����˵���� ��
**************************************************************************/
MEX_PHS_SMS_HANDLE Phs_SmsSendStart(Mex_Phs_Sms_Send_Req *req);

/**************************************************************************
* �������ƣ� Phs_SmsSendRetry
* ���������� �������·��Ͷ���
* ����˵���� [IN] handle ���Ͷ��ŵľ��
* �� �� ֵ�� �Ƿ�ɹ�
*            [bool:TRUE]  �ɹ�
*            [bool:FALSE] ʧ��
* ����˵���� ��
**************************************************************************/
bool Phs_SmsSendRetry(MEX_PHS_SMS_HANDLE handle);

/**************************************************************************
* �������ƣ� Phs_SmsSendFinish
* ���������� ���Ͷ������
* ����˵���� [IN] handle ���Ͷ��ŵľ��
* �� �� ֵ���Ƿ�ɹ�
*            [bool:TRUE]  �ɹ�
*            [bool:FALSE] ʧ��
* ����˵���� ��
**************************************************************************/
bool Phs_SmsSendFinish(MEX_PHS_SMS_HANDLE handle);

// ģ���յ�һ������
// ����1: [IN] SIM�����
// ����2: [IN] �������ĺ���, eg:+8613800210500
// ����3: [IN] ���ź���, eg: 13800138000
// ����4: [IN] ��������, ���֧��70���ַ�
// ����5: [IN] ����ʱ��
bool Phs_NewSms(E_MEX_Sim_Card_ID eSimId, const char* pSmsCenter, const char* pSmsNumber, const wchar_t *pSmsContent, const Mex_DateTime *pDt);

//apis for phone book
/**************************************************************************
* �������ƣ� Phs_GetPhbEntryIndex
* ���������� ��ȡ�绰����Ŀ����
* ����˵���� [OUT:ppuStoreIndex]		����绰����Ŀ����
*            [OUT:puEntryCount]		�绰����Ŀ��
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵����
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntryIndex(uint16** ppuStoreIndex, uint16* puEntryCount);


/**************************************************************************
* �������ƣ� Phs_SearchPhbEntry
* ���������� ���ݹؼ��ּ����绰��
* ����˵���� [IN:szKeyword]			�����ؼ���
*            [IN:bIsPinyin]			�Ƿ�ƴ������
*            [IN:eLocation]			�����绰����Ŀ��λ�ã�ȫ�����ֻ���SIM1��SIM2
*            [OUT:ppuSearchBitmask]		�����������������
*            [OUT:puResultCount]		�����������Ŀ��
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_SearchPhbEntry(wchar_t* szKeyword, bool bIsPinyin, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask, uint16* puResultCount);

/**************************************************************************
* �������ƣ� Phs_GetPhbEntry
* ���������� ��ȡ�绰����Ŀ������
* ����˵���� [IN:uIndex]				�����绰����Ŀ��������
*            [OUT:psName]				����
*            [OUT:psNumber]			����
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* �������ƣ� Phs_GetPhbEntryEx
* ���������� ��ȡ�绰����Ŀ����ϸ����
* ����˵���� [IN:uIndex]				�����绰����Ŀ��������
*            [OUT:psEntry]				���������룬��ͥ�绰����˾�绰��E-Mail�������
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry);

/**************************************************************************
* �������ƣ� Phs_GetPhbMemoryStatus
* ���������� ��ȡָ��λ�õ绰���Ĵ洢״̬
* ����˵���� [IN:eLocation]			��ȡ�绰����Ŀ��λ�ã�ȫ�����ֻ���SIM1��SIM2
*            [OUT:pTotal] �ܴ�С
*            [OUT:pUsed] ��ʹ�ô�С
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbMemoryStatus(E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed);

/**************************************************************************
* �������ƣ� Phs_AddPhbEntry
* ���������� ��ӵ绰����Ŀ
* ����˵���� [IN:eLocation]			�绰����Ŀ��λ�ã�ȫ�����ֻ���SIM1��SIM2
*            [IN:psName]				����
*            [IN:psNumber]				����
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_AddPhbEntry(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* �������ƣ� Phs_AddPhbEntryEx
* ���������� ��ӵ绰����Ŀ
* ����˵���� [IN:eLocation]			�绰����Ŀ��λ�ã�ȫ�����ֻ���SIM1��SIM2
*            [IN:sEntry]				���������룬��ͥ�绰����˾�绰��E-Mail�������
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_AddPhbEntryEx(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* sEntry);

/**************************************************************************
* �������ƣ� Phs_UpdatePhbEntry
* ���������� ���µ绰����Ŀ
* ����˵���� [IN:uIndex]				�����绰����Ŀ��������
*            [IN:psName]				����
*            [IN:psNumber]				����
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_UpdatePhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* �������ƣ� Phs_UpdatePhbEntryEx
* ���������� ���µ绰����Ŀ
* ����˵���� [IN:uIndex]				�����绰����Ŀ��������
*            [IN:psEntry]				���������룬��ͥ�绰����˾�绰��E-Mail�������
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_UpdatePhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry);

/**************************************************************************
* �������ƣ� Phs_RemovePhbEntry
* ���������� ��������ɾ���绰����Ŀ
* ����˵���� [IN:uIndex] ��Ŀ����ֵ
* �� �� ֵ�� E_Mex_Phs_Phb_Result
* ����˵���� ��
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_RemovePhbEntry(uint16 uIndex);

//��ȡȺ����Ϣ
//����1��[IN] groupIndex Ⱥ������
//����2��[OUT] pGroup ���Ⱥ����ϸ��Ϣ
//���أ�E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_GetPhbGroup(uint16 groupIndex, Mex_Phs_Phb_Group *pGroup);

//ע��绰���ص�����
//����1��[IN] pCallBack �ص�����ָ��
//���أ�E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_SetPhbCB(Mex_Phs_Phb_CB pCallBack);

//����绰���ص�����
//����1����
//���أ�E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_ClearPhbCB(void);

//apis for hook message
/**************************************************************************
* �������ƣ� mex_SetHookMsg
* ���������� ������ҪHook��MSG�Լ��䷢��ģ�飬��ע��Hook�Ļص�����������
*            �����ж��Ƿ������MSG��������������
* ����˵����
*            [IN:eMsgId]     ��ǰ��Ҫ�����MSG��ID
*            [IN:eSimId]     ��Ҫ����������һ��SIM������Ϣ
*            [IN:hkFuncPtr]  Hook�Ļص�����ָ��
*            [OUT:pHkInfo]   Hook��Ϣ�ṹ��ָ��
* �� �� ֵ��
*            [bool:true]	�ɹ�
*            [bool:false]	ʧ��
* ����˵���� ��
**************************************************************************/
bool Phs_SetHookMsg(E_Mex_Hk_Msg_Id eMsgId, E_MEX_Sim_Card_ID eSimId, Mex_Hk_Func_Ptr hkFuncPtr, Mex_Hk_Info_Ptr p);

/**************************************************************************
* �������ƣ� Phs_UnhookMsg
* ���������� ȡ��ָ����Hook
* ����˵���� [IN:p] Hook����Ϣ��ָ��
* �� �� ֵ�� [bool:TRUE]  �ɹ�
*            [bool:FALSE] ʧ��
* ����˵���� ��
**************************************************************************/
bool Phs_UnhookMsg(Mex_Hk_Info_Ptr p);

// APIs for call log (Call History)
/**************************************************************************
* �������ƣ� Phs_GetNumOfCallLog
* ���������� ��ȡͨ����¼������
* ����˵���� [IN] simId SIM��
*            [IN] callType ͨ����¼����
* �� �� ֵ�� ͨ����¼������
* ����˵���� ��
**************************************************************************/
uint32 Phs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);

/**************************************************************************
* �������ƣ� Phs_GetCallLog
* ���������� ��ȡָ����ͨ����¼
* ����˵���� [IN] simId SIM��
*            [IN] callType ͨ����¼����
*            [IN] index ͨ����¼����(��index ��ͨ����¼)
*            [OUT] callLog ����ָ����ͨ����¼
* �� �� ֵ���Ƿ�ɹ�
*            [bool:TRUE]  �ɹ�
*            [bool:FALSE] ʧ��
* ����˵���� ��
**************************************************************************/
bool Phs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog);


//��ȡͨ����¼ģ���״̬
//����1��[IN] simId SIM��
//����2��[IN] callType ͨ����¼����
//���أ�E_MEX_CALLOG_STATUS ͨ����¼ģ���״̬
E_MEX_CALLOG_STATUS Phs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);

//����ͨ����¼
//����1��[IN] simId SIM��
//����2��[IN] callType ͨ����¼����
//���أ��Ƿ�ɹ�
bool Phs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);


/**************************************************************************
* �������ƣ� Phs_Start_Camera
* ������������ʼ������ͷ������Ӳ���Ͽ�������ͷ
* ����˵����
int32 offset_x �ɼ�ͼ�����Ͻ���ʼλ��x����
int32 offset_y �ɼ�ͼ�����Ͻ���ʼλ��x����
int32 iImgWidth �ɼ�ͼ��Ŀ��
int32 iImgHeight �ɼ�ͼ��ĸ߶�
int32 iDispWidth  ��ʾͼ��Ŀ��  �벻Ҫ������Ļ
int32 iDispHeight ��ʾͼ��ĸ߶�  �벻Ҫ������Ļ
void *buffer	 ͼ�����ݱ��浽�Ļ�����
int32 buf_size   ��������С һ�㻺�����Ĵ�СΪ��ʾͼ���������*2
* �� �� ֵ����E_Mex_Phs_Camera_Code
* ����˵���� ��
**************************************************************************/

int8 Phs_Start_Camera(int32 offset_x, int32 offset_y, int32 iImgWidth, int32 iImgHeight, int32 iDispWidth, int32 iDispHeight, void *buffer,int32 buf_size);

/**************************************************************************
* �������ƣ� Phs_Start_Preview
* ��������������Ԥ��ģʽ
* ����˵����
* �� �� ֵ����E_Mex_Phs_Camera_Code
* ����˵���� ��
**************************************************************************/

int8 Phs_Start_Preview(void);

/**************************************************************************
* �������ƣ� Phs_Stop_Preview
* ����������ֹͣԤ��
* ����˵����
* �� �� ֵ����E_Mex_Phs_Camera_Code
* ����˵���� ��
**************************************************************************/

int8 Phs_Stop_Preview(void);

/**************************************************************************
* �������ƣ� Phs_Stop_Camera
* �����������ر�����ͷ
* ����˵����
* �� �� ֵ����E_Mex_Phs_Camera_Code
* ����˵���� ��
**************************************************************************/

int8 Phs_Stop_Camera(void);

/**************************************************************************
* �������ƣ� Phs_Camera_Capture_To_File
* ��������������ǰͼ�񱣴�Ϊjpg��ʽͼƬ����ŵ�������
* ����˵����void **buffer  ָ�򻺳����׵�ַ��ָ��
uint32 *captured_size ��С
* �� �� ֵ����E_Mex_Phs_Camera_Code
* ����˵���� ��
**************************************************************************/

int8 Phs_Camera_Capture_To_Mem(void **buffer,uint32 *captured_size);

/**************************************************************************
* ����������������������Ϊ��������
* ����˵����
**************************************************************************/

void Phs_Camera_Zoom_In(void);

void Phs_Camera_Zoom_Out(void);



/**************************************************************************
* �������ƣ� Phs_Camera_Get_Rotate
* ������������ȡ����ͷ��ת��ʽ
* ����˵����
* �� �� ֵ����E_Mex_Phs_Camera_Rotate
* ����˵���� ��
**************************************************************************/

E_Mex_Phs_Camera_Rotate Phs_Camera_Get_Rotate(void);
/**************************************************************************
* �������ƣ� Phs_Camera_Get_Buffer
* ������������ȡ��������ַ
* ����˵����
* �� �� ֵ�����ݻ������׵�ַ
* ����˵���� �������õ���ͼ�����ݣ����ܻ���Ϊ����ͷ��
��ת�Ƕȣ����õ���ת��ͼ�������Ҫ�õ�����Ļ��ͬ�����ͼ��
�����Phs_Camera_Get_Rotate�õ�����ת��ʽ��ת������
**************************************************************************/

void *Phs_Camera_Get_Buffer(void);
/**************************************************************************
* �������ƣ� Phs_Snapshot_To_Jpg
* ��������������ǰ�㱣��Ϊjpg�ļ�
* ����˵��������·��
* �� �� ֵ��
* ����˵���� ��
**************************************************************************/

void Phs_Snapshot_To_Jpg(wchar_t *fileName);

/**************************************************************************
* �������ƣ� PhsSetDateTime
* �������������õ�ǰϵͳʱ��
* ����˵����Mex_DateTime pdt   ���õ�ϵͳʱ��
* �� �� ֵ���Ƿ����óɹ�
* ����˵���� ��
**************************************************************************/
bool PhsSetDateTime(Mex_DateTime pdt);

// ���ݵ绰�����������
// ����1: [IN] pNumber �绰����
// ����2: [OUT] pName ����
// ����ֵ: E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_LookUpPhbNumber(const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName);

// ��ʼ�δ�绰
// ����1: [IN] eSimId SIM�����
// ����2: [IN] number Ҫ�γ��ĵ绰����
// ����3: [IN] callback �δ�绰�Ļص�����
// ����: MEX_OUTCALL_HANDLE ʧ��ʱ����NULL
MEX_OUTCALL_HANDLE Phs_OutCallStart(E_MEX_Sim_Card_ID eSimId, const char* number, MexOutCallCB callback);

// �ͷ�Outgoing Call ���
// ����1: [IN] handle Outgoing Call ���
// ����: bool �Ƿ�ɹ�
bool Phs_OutCallFinish(MEX_OUTCALL_HANDLE handle);

// ��ͨ�������У�ģ�ⰴ����ʼ
// ����1: [IN] handle Outgoing Call ���
// ����2: [IN] keycode ����'0'~'9', '*', '#'
// ����: bool �Ƿ�ɹ�
bool Phs_OutCallDtmfBegin(MEX_OUTCALL_HANDLE handle, char keycode);

// ��ͨ�������У�ģ�ⰴ������
// ����1: [IN] handle Outgoing Call ���
// ����: bool �Ƿ�ɹ�
bool Phs_OutCallDtmfEnd(MEX_OUTCALL_HANDLE handle);

// ����ģ���Ƿ�׼����
// ����1: ��
// ����: bool �Ƿ�׼����
bool Phs_SmsReady(void);

// ��ȡMSGBOX�ж��ŵ���Ŀ
// ����1: [IN] msgBox MSGBOX������
// ����1: MEX_PHS_SMS_INDEX_INVALID ��ʾ��ȡʧ��
// ����2: ����ֵ ��ʾMSGBOX�ж��ŵ���Ŀ
uint16 Phs_GetNumOfSms(E_MEX_PHS_SMS_MSGBOX msgBox);

// ��ȡһ������
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����3: [IN] callback ��ȡ���Żص�����
// ����: bool �Ƿ�ɹ�
bool Phs_ReadSmsContent(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Read_Sms_CB callback);

// ��ȡSms Address
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����: ָ��Sms Addr ��ָ��, ʧ��ʱ����NULL
char *Phs_GetSmsAddr(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// ��ȡ���ŵ�����
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����3: [OUT] pDt �����������
// ����: bool �Ƿ�ɹ�
bool Phs_GetSmsDateTime(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_DateTime *pDt);

// ��ȡ���ŵ�һС���ַ�����(ͨ��Ϊǰ10������)
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����: ָ��������ݵ�ָ��, ʧ��ʱ����NULL
bool Phs_GetSmsCache(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, wchar_t *pContent, uint16 iContentMemLen);

// ��ȡ��������SIM1 ��SIM2
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����: E_MEX_Sim_Card_ID, ������E_MEX_SIM_NONEʱ, ��ʾ��ȡʧ��
// ע��:
// 1. �˽ӿڽ����ռ����еĶ�������
E_MEX_Sim_Card_ID Phs_GetSmsSimId(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// ɾ������
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����3: [IN] callback ɾ�����Żص�
// ����: bool �Ƿ�ɹ�
bool Phs_DelSms(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Del_Sms_CB callback);

// ȡ��Ϣ���Żص�Mex_Phs_Read_Sms_CB & Mex_Phs_Del_Sms_CB
// ����: ��
// ����: ��
// ע��:
// 1. ��Ӧ�ó���ǿ���˳�ʱ������ȡ�����Żص�
// 2. ��Ӧ�ó�����ʹ�ö��Žӿ�ʱ������ȡ�����Żص�
void Phs_CancelSmsCB(void);

// ������ŵ�Msg Box ��
// ע��:
// 1. �˽ӿڲ��ܽ����ű��浽�ռ���(Inbox)��
// 2. ���ֻ�ϵͳ��û����Ӧ������ʱ������Ҳ��ʧ��
bool Phs_SaveSms(E_MEX_PHS_SMS_MSGBOX msgBox, E_MEX_Sim_Card_ID simId, const wchar_t *pContent, const char *pAddr, Mex_Phs_Save_Sms_CB callback);

// ��������Ƿ����
// ����1: [IN] msgBox MSGBOX������
// ����: bool �����Ƿ����
bool Phs_SmsMsgBoxExist(E_MEX_PHS_SMS_MSGBOX msgBox);

// �������Ƿ�δ��
// ����1: [IN] msgBox MSGBOX������
// ����2: [IN] index ��������
// ����: true ��ʾ����δ��, false ��ʾ�����Ѷ�
bool Phs_SmsIsUnread(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// ���ñ�ֽ�����������ػ�����
// ����1: [IN] eType ��ֽ������
// ����2: [IN] pFileName ͼ���ļ���·��
// ����3: [IN] bAbsPath Ϊ��ʱ����ʾΪ����·��
// ����: �����Ƿ�ɹ�
bool Phs_PhnSetWallPaper(E_MEX_WallPaper_Type eType, wchar_t *pFileName, bool bAbsPath);


//new camera api

// ��һ���ƶ���ߵ�RGB565����ѹ��ΪJPG�ļ�
// ����1: [IN] image_ptr �ƶ���RGB565��ʽ������
// ����2: [IN] image_width ͼ��Ŀ��
// ����3: [IN] image_height ͼƬ�ĸ߶�
// ����4: [IN] jpg_qty ͼƬѹ������   �ο�ö��E_Mex_JPG_Encode_Qty
// ����5: [IN] file_name ѹ����ͼƬ�ı���·��
// ����6: [IN] bAbsPath ����5�Ƿ�Ϊ����·��
// ����: �Ƿ�ɹ�
bool Phs_EncodeToJpeg(void *image_ptr,int32 image_width,int32 image_height, int8 jpg_qty, wchar_t *file_name,bool bAbsPath);

//��������ͷ(��ʼ���Լ�����ͷ����ӵ�)
// ����1: [IN] offset_x �ɼ�ͼ�����Ͻ���ʼλ��x����
// ����2: [IN] offset_y �ɼ�ͼ�����Ͻ���ʼλ��y����
// ����3: [IN] iDispWidth ͼƬ�Ŀ��
// ����4: [IN] iDispHeight ͼƬ�ĸ߶�
// ����5: [IN] bPreviewUseSram �Ƿ�������ͷʹ��SRAM   һ�����������Ϊfalse
// ����6: [IN] outputformat ����ͷ�����ͼ���ʽ �ο�ö��E_Mex_Phs_Camera_OutPut_Format
// ����: �ο�ö��E_Mex_Phs_Camera_Code
int8 Phs_Start_Camera_Ext(int32 offset_x, int32 offset_y, int32 iDispWidth, int32 iDispHeight, bool bPreviewUseSram, int8 outputformat);

//��������ͷԤ��
//����: �ο�ö��E_Mex_Phs_Camera_Code
int8 Phs_Start_Preview_Ext(void);

//�ر�����ͷԤ��
//����: �ο�ö��E_Mex_Phs_Camera_Code
int8 Phs_Stop_Preview_Ext(void);

//�ر�����ͷ
//����: �ο�ö��E_Mex_Phs_Camera_Code
int8 Phs_Stop_Camera_Ext(void);

//��ȡ����ͷ�ɼ���������
//���� �ɼ�����ͼ������  ͼ���ʽ����СΪPhs_Start_Camera_Ext���õĲ���
//˵�� Ϊ�˱�֤���ݵ�ͬ��������ÿ����Ҫʹ������ʱ����һ���ú������ú������ص����ݵ�ַ�ᷢ���仯
void *Phs_GetCameraOutPutData(void);

//��ȡ����ͷ�ɼ���������
//���� �ɼ�����ͼ������  ͼ���ʽ����СΪPhs_Start_Camera_Ext���õĲ���
//˵�� Ϊ�˱�֤���ݵ�ͬ��������ÿ����Ҫʹ������ʱ����һ���ú������ú������ص����ݵ�ַ�ᷢ���仯
void *Phs_GetCameraOutPutData_Ext(uint32 *puiSize);

//��������ͷ��Ԥ��ģʽ�Ƿ�Ϊҹ��ģʽ
//���� bNightMode �Ƿ�Ϊҹ��ģʽ
void Phs_SetCameraNightMode(bool bNightMode);

//��Hook�ص������л�ȡHook Result
//����: ��
//����: Hook Result
uint8 Phs_GetHookResult(void);

// �����龰ģʽ
void Phs_SetPhoneProfile(int32 profileId);
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//��������ͷ����һ������
void Phs_Camera_Capture_Req(Mex_Camera_Cap_Req *req);


//��ȡ��ǰ�龰ģʽ�������ͷ���ֵ�ο�E_Mex_Pro_Alert_Type
int8 Phs_Get_CurrProfile_AlertType(void);

//�л�����ͷ
//���� �Ƿ��л�Ϊ��������ͷ
void Phs_SwitchCamera(bool);

// ���ݺ����ȡ������
int32 Phs_Aq_getCity(char *pNumber, wchar_t *CityName);

// ������ת����pNumber,callbackΪ����ص�,0ʧ��,1�ɹ�
int32 Phs_Aq_DivertCall(char *pNumber, i_Callback_i callback);

// ȡ��������ת��,callbackΪ����ص�,0ʧ��,1�ɹ�
int32 Phs_Aq_CancelDivert(i_Callback_i callback);

// ��ѯ������ת��,callbackΪ����ص�,-1ʧ��,����ֵ��Ӧ����
/*
typedef enum _MN_SS_STATUS_E
{
    MN_SS_NOT_PROVITION    = 0,        // bit3 = 0;
    MN_SS_NOT_REGISTERED   = 0x04,      // bit3 = 1 but bit2 = 0;
    MN_SS_PROVITION        = 0x05,
    MN_SS_NOT_ACTIVE       = 0x06,      // bit3,bit2 all= 1, bit1 = 0;
    MN_SS_ACTIVE_OPERATIVE = 0x07,      // bit3,bit2,bit1 = 1, bit4 = 0;
    MN_SS_ACITVE_QUIESCENT = 0x0F      // bit4,bit3,bit2,bit1 all equal to 1
} MN_SS_STATUS_E;
*/
int32 Phs_Aq_QueryDivert(i_Callback_i callback);

#endif

#if MEX_SDK_VER >= 1027
/**************************************************************************
* �������ƣ� Phs_GetMissedCallNumber
* ���������� ��ȡδ����������(�û��鿴֮������)
* ����˵���� [NULL]
* �� �� ֵ�� δ����������
* ����˵���� ��
**************************************************************************/
uint32 Phs_GetMissedCallNumber(void);

/**************************************************************************
* �������ƣ� Phs_ClearMissedCallNum
* ���������� �����û�δ������(�û��鿴֮������)
* ����˵���� [NULL]
* �� �� ֵ�� �Ƿ�ɹ�
* ����˵���� ��
**************************************************************************/
bool Phs_ClearMissedCallNum(void);

void Phs_Set_Can_PowerOff(bool IsCanPowerOff);

E_MEX_Phs_HW_State Phs_Get_Backlight_State(void);


/**************************************************************************
* �������ƣ� Phs_ClearUnReadSms
* ���������� �����û�δ������(�û��鿴֮������)
* ����˵���� [NULL]
* �� �� ֵ�� �Ƿ�ɹ�
* ����˵���� ��
**************************************************************************/
bool Phs_ClearUnReadSms(void);
#endif
#if MEX_SDK_VER >= 1033
void Phs_MotionSensor_Start(MexLib_MotionSensor_CallBack callback,int16 x_ponit,int16 y_point,int16 motionSensitivity);
void Phs_MotionSensor_Stop(void);
#endif
#endif
#endif

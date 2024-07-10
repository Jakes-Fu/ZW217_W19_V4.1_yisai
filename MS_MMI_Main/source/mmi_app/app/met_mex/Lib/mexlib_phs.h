/************************************************************************
* 版权所有 (C)2010,扬讯科技。
*
* 文件名称： // MexLib_Phs.h
* 文件标识：
* 内容摘要： //mex平台手机状态相关库
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

//网络运营商类型（only for gsm900）
typedef enum
{
	E_MEX_NET_ID_CMCC,		//中国移动
	E_MEX_NET_ID_CNGSM,		//中国联通gsm
	E_MEX_NET_ID_UNKNOW,	//目前不支持的类型
	E_MEX_NET_ID_NONE		//无服务
}E_MEX_Net_Service_ID;

//SIM卡id
typedef enum
{
	E_MEX_SIM_NONE,
	E_MEX_SIM_1,
	E_MEX_SIM_2,
	E_MEX_SIM_3,
	E_MEX_SIM_4,
	E_MEX_SIM_MAX
}E_MEX_Sim_Card_ID;

//双卡通道类型
//在发送短信或者拨打电话之前 需要设置
typedef enum
{
	E_MAC_CALL_CHANNEL,
	E_MAC_CALLLOG_CHANNEL,
	E_MAC_SMS_CHANNEL,
	E_MAC_PHB_CHANNEL,
	E_MAC_MAX
}E_MEX_AD_CHANNEL;
//事件状态
typedef enum
{
	E_MEX_PHS_NEEDUPDATE,	//需要更新
	E_MEX_PHS_PROCESSING,	//处理中
	E_MEX_PHS_COMPLETED,	//完成
	E_MEX_PHS_ERROR			//错误
}E_MEX_Phs_State;


/*对应系统中的 结构 MESSAGE*/
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

//情景模式
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_PROFILE_GENERAL		( ConvertSysDef(E_M2MDC_PROFILE_GENERAL) )
#define MEX_PROFILE_MEETING		( ConvertSysDef(E_M2MDC_PROFILE_MEETING) )
#define MEX_PROFILE_OUTDOOR		( ConvertSysDef(E_M2MDC_PROFILE_OUTDOOR) )
#define MEX_PROFILE_INDOOR		( ConvertSysDef(E_M2MDC_PROFILE_INDOOR) )
#define MEX_PROFILE_HEADSET		( ConvertSysDef(E_M2MDC_PROFILE_HEADSET) )
#define MEX_PROFILE_SILENT		( ConvertSysDef(E_M2MDC_PROFILE_SILENT) )
#define MEX_PROFILE_BT			( ConvertSysDef(E_M2MDC_PROFILE_BT) )
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)//参见MMIENVSET_ENV_MODE_E
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
//情景模式
typedef enum
{
E_MPP_GENERAL,			//标准
E_MPP_MEETING,			//会议
E_MPP_OUTDOOR,			//户外
E_MPP_INDOOR,			//室内
E_MPP_HEADSET,			//耳机
E_MPP_SILENT,			//无声
E_MPP_BT,				//蓝牙
E_MPP_UNKNOW			//未知
}E_MEX_Phs_Profile;
*/
//背光灯状态
typedef enum
{
	E_MPHS_PERMANENT,		//永久
	E_MPHS_SHORT_TIME,		//短暂 或者 一次 超时后自定停止
	E_MPHS_OFF,				//关闭,非立刻关闭
#if (MEX_SDK_VER >= 1025)
	E_MPHS_OFF_REALTIME,     //立刻关闭(for FAUI)
#endif
}E_MEX_Phs_HW_State;

typedef enum
{
	E_MPL_CN,				//简体中文
	E_MPL_TR,				//繁体中文
	E_MPL_EN,				//英文
	E_MPL_UNKONW			//未知语言
}E_MEX_Phs_Language;

//电池的状态
typedef enum
{
	E_MBS_NORMAL,			//普通状态
	E_MBS_CHARGE_IN,		//正在充电
	E_MBS_CHARGE_COMPLETE,	//充电完成
	E_MBS_MAX
}E_MEX_Battery_Status;

//日期格式
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define MEX_DF_DD_MMM_YYYY_H	( ConvertSysDef(E_M2MDC_DF_DD_MMM_YYYY_H) )
#define MEX_DF_DD_MM_YYYY_S		( ConvertSysDef(E_M2MDC_DF_DD_MM_YYYY_S) )
#define MEX_DF_MM_DD_YYYY_S		( ConvertSysDef(E_M2MDC_DF_MM_DD_YYYY_S) )
#define	MEX_DF_YYYY_MM_DD_S		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_S) )
#define MEX_DF_YYYY_MM_DD_H		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_S) )
#define MEX_DF_MMM_DD_YYYY		( ConvertSysDef(E_M2MDC_DF_YYYY_MM_DD_H) )
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)//参见MMISET_DATE_DISPLAY_TYPE_E
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
	uint32 time_stamp;		//信息获取时间
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
	E_MPSR_RESULT_SUCCESS,          // 发送成功
	E_MPSR_RESULT_FAIL              // 发送失败
} E_MEX_PHS_SMS_RESULT;

// MEX PHS SMS MSGBOX TYPE
typedef enum
{
	E_MPSM_INBOX,                   // 收件箱
	E_MPSM_OUTBOX,                  // 已发信息
	E_MPSM_DRAFTS,                  // 草稿箱
	E_MPSM_UNSENT,                  // 发件箱

	E_MPSM_MAX
} E_MEX_PHS_SMS_MSGBOX;

// MEX OUTCALL EVENT
typedef enum
{
	E_MOE_SS_PARSING_STRING_SUCCESS,            // L4层分析号码成功
	E_MOE_SS_PARSING_STRING_FAIL,               // L4层分析号码失败

	E_MOE_ACTIVE_CALL_SUCCESS,                  // 电话已拔通
	E_MOE_ACTIVE_CALL_FAIL,                     // 电话未拔通

	E_MOE_CONNECTED,                            // 通话已开始
#if MEX_SDK_VER >= 1011
	E_MOE_CALL_RELEASED,                        // 通话已结束
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
// 参数1: msgBox MSGBOX的类型
// 参数2: index 短信索引
// 参数3: pContent 指向短信内容
// 参数4: iContentMemLen 短信内容的长度, 按字节计算, 结束符L'\0' 不计算在内
typedef void (*Mex_Phs_Read_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, const wchar_t *pContent, uint16 iContentMemLen, bool success);
// Mex Phs Del Sms CB
// 参数1: msgBox MSGBOX的类型
// 参数2: index 短信索引
// 参数3: success 是否删除成功
typedef void (*Mex_Phs_Del_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, bool success);
// Mex Phs Save Sms CB
typedef void (*Mex_Phs_Save_Sms_CB) (E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, bool success);

// Mex Phs Sms Send Req
typedef struct
{
	const char             *number;        // 收件人电话号码，只能是ASCII编码
	uint32                  numberLen;     // 收件人电话号码长度
	const char             *content;       // 短信内容
	uint32                  contentLen;    // 短信内容长度，按字节计算 in bytes
	E_MEX_PHS_SMS_ENCODE    encode;        // 短信内容编码方式，ASCII或UNICODE
	E_MEX_Sim_Card_ID       simId;         // 指定发送短信的 Sim Card，SIM1或SIM2
	Mex_Phs_Sms_CB          smsCB;         // 发送短信的回调函数
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
	E_MEX_PHB_RESULT_SUCCESS,            // 成功
	E_MEX_PHB_RESULT_BUSY,               // 电话本忙
	E_MEX_PHB_RESULT_WRONG_INDEX,        // 索引值错误
	E_MEX_PHB_RESULT_WRONG_LOCATION,     // 存储位置错误
	E_MEX_PHB_RESULT_NO_SPACE,           // 存储空间不足
	E_MEX_PHB_RESULT_NO_PERMISSION,      // 应用程序权限不足

	E_MEX_PHB_RESULT_ERROR_UNKNOWN       // 未知错误
} E_Mex_Phs_Phb_Result;


typedef enum
{
	E_MEX_PHB_EVENT_ADDENTRY_SUCCESS,    // 添加条目成功
	E_MEX_PHB_EVENT_ADDENTRY_FAIL,       // 添加条目失败
	E_MEX_PHB_EVENT_UPDATEENTRY_SUCCESS, // 更新条目成功
	E_MEX_PHB_EVENT_UPDATEENTRY_FAIL,    // 更新条目失败
	E_MEX_PHB_EVENT_REMOVEENTRY_SUCCESS, // 删除条目成功
	E_MEX_PHB_EVENT_REMOVEENTRY_FAIL,    // 删除条目失败

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
	Mex_Phs_Phb_Name        name;              // 姓名
	Mex_Phs_Phb_Number      number;            // 电话号码
	Mex_Phs_Phb_Number      number_home;       // 家庭电话(可选)
	Mex_Phs_Phb_Number      number_company;    // 公司电话(可选)
	Mex_Phs_Phb_Number      number_fax;        // 传真号码(可选)
	Mex_Phs_Phb_Name        name_company;      // 公司名称(可选)
	Mex_Phs_Phb_Email       email;             // 电子信箱(可选)
	Mex_DateTime            birthday;          // 生日
	uint16                  groupIndex;        // 来电群组索引
	E_Mex_Phs_Phb_Location  location;          // 存储位置PHONE, SIM1, SIM2
} Mex_Phs_Phb_Entry;


typedef struct
{
	uint16                  groupIndex;        // 来电群组索引
	Mex_Phs_Phb_Name        groupName;         // 群组名称
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
    E_MHMI_MSG_ID_INVALID                   = 0,      //消息ID 0表示为无效的消息
	E_MHMI_MSG_ID_SMS_DELIVER_MSG_IND       = 1,      //来短信的消息Id
	E_MHMI_MSG_ID_CC_CALL_RING_IND          = 2,      //来电话的消息Id
	E_MHMI_MSG_ID_WILL_PLAY_ALARMTONE       = 3,      //将要播放闹铃音

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
	Mex_Phs_Phb_Number  sca_number;           // 短信中心号码
	Mex_Phs_Phb_Number  oa_number;            // 短信号码
	uint8   pid;                              // 协议标识
	uint8   dcs;                              // 短信编码方式
	uint8   ori_dcs;                          // 原始编码方式, 可忽略
	uint16  index;                            // 短信索引
	uint16  dest_port;                        // 目标端口, 可忽略
	uint16  src_port;                         // 源端口, 可忽略
	uint8   mti;                              // 信息类型标识
	uint8   display_type;                     // 显示方式
	uint8   storage_type;                     // 存储方式
	uint32  concat_info;                      // Unknown
	uint16  no_msg_data;                      // 短信内容长度(memory size in bytes), 不包含结束符, 按字节计算
	uint8  *msg_data;                         // 短信内容
} Mex_Hk_Sms_Deliver_Msg;

//MsgStruct for E_MHMI_MSG_ID_CC_CALL_RING_IND
typedef struct
{
	uint8   call_id;                          // 来电编号
	Mex_Phs_Phb_Number  num;                  // 来电号码
	Mex_Phs_Phb_Number  sub_addr;             // 子号码
	Mex_Phs_Phb_Number  redirect_num;         // 转接号码
	uint8   name_present;                     // 是否包含姓名信息
	uint8   cnap_info_exist;                  // Unknown
	uint8   tag;                              // Unknown
	uint8   name[30];                         // 姓名
	uint8   dcs;                              // 数据编码方式
	uint8   length;                           // 数据长度, 按字节计算
	uint8   auto_answer;                      // 自动接听
	uint8   call_type;                        // 来电类型
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
	E_MCS_CALLLOG_READY,            // 已准备好
	E_MCS_CALLLOG_BUSY,             // 系统正忙
	E_MCS_CALLLOG_NEED_UPDATE,      // 需要更新
	E_MCS_CALLLOG_UPDATING,         // 正在更新中
	E_MCS_CALLLOG_NOT_SUPPORTED,    // 不支持

	E_MCS_CALLLOG_MAX
} E_MEX_CALLOG_STATUS;


typedef enum
{
	E_MCT_CALLLOG_DIALED,           // 已拔电话
	E_MCT_CALLLOG_MISSED,           // 未接来电
	E_MCT_CALLLOG_RECVD,            // 已接来电
	E_MCT_CALLLOG_MIXED,            // 全部

#if MEX_SDK_VER >= 1002
	E_MCT_CALLLOG_MAX
#endif /* MEX_SDK_VER */
} E_MEX_CALLLOG_TYPE;


// CallLog 定义
typedef struct
{
	wchar_t  number[MAX_CALLLOG_NUMBER_LENGTH+1];    // 电话号码
	uint32   numberLen;                              // 电话号码长度
	wchar_t  pbName[MAX_CALLLOG_PBNAME_LENGTH+1];    // 姓名
	uint32   pbNameLen;                              // 姓名长度

	uint32       nTimes;         // 记录次数
	Mex_DateTime startTime;      // 开始时间
	Mex_DateTime duration;       // 通话时长
} Mex_CallLog_Info;

//JPG图片压缩质量
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
	E_MPAT_RING,					//响铃
	E_MPAT_VIBRATION_ONLY,			//只震动
	E_MPAT_VIBRATION_AND_RING,		//震动响铃
	E_MPAT_VIBRATION_THEN_RING,		//震动后响铃
	E_MPAT_SILENT,					//安静
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

//刷新Imei信息，系统会在一定时间后刷新Imei信息
//参数 sim_id：指定sim卡 参考枚举E_MEX_Sim_Card_ID
//返回 无
//注意：异步操作，调用该函数后，并不会立即去刷新，不适用于实时性较高的程序
void Phs_ReFresh_Imei (void);

//刷新基站信息，系统会在一定时间后刷新基站信息
//参数 sim_id：指定sim卡 参考枚举E_MEX_Sim_Card_ID
//返回 无
//注意：异步操作，调用该函数后，并不会立即去刷新，不适用于实时性较高的程序
void Phs_ReFresh_Imsi (E_MEX_Sim_Card_ID sim_id);

//刷新基站信息，系统会在一定时间后刷新基站信息
//参数 sim_id：指定sim卡 参考枚举E_MEX_Sim_Card_ID
//返回 无
//注意：异步操作，调用该函数后，并不会立即去刷新，不适用于实时性较高的程序
void Phs_ReFresh_SmsCent (E_MEX_Sim_Card_ID sim_id);


//设置手机背光灯状态
//参数 state 参考枚举 E_MEX_Phs_HW_State
//返回值 无
void Phs_Set_Backlight(E_MEX_Phs_HW_State state);

//设置手机震动状态
//参数 state 参考枚举 E_MEX_Phs_HW_State
//返回值 无
void Phs_Set_Vibrator(E_MEX_Phs_HW_State state);

//api for call
/**************************************************************************
* 函数名称： Phs_MakePhoneCall
* 功能描述： 拨打电话
* 参数说明： [IN:eSimId] SIM卡
* 参数说明： [IN:wszNumber]	拨打的电话号码
* 返 回 值： bool 是否成功
* 其它说明： 应用程序必须具有权限E_MAPT_MakeCall
**************************************************************************/
bool Phs_MakePhoneCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber);

/**************************************************************************
* 函数名称： Phs_IncomingCall
* 功能描述： 来电
* 参数说明： [IN:eSimId] SIM卡
* 参数说明： [IN:wszNumber]	来电号码
* 返 回 值： bool 是否成功
* 其它说明： 无
**************************************************************************/
bool Phs_IncomingCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber);

/**************************************************************************
* 函数名称： Phs_SmsSendStart
* 功能描述： 发送短信开始
* 参数说明： [IN] req 发送短信请求结构体指针
* 返 回 值： 发送短信的句柄，失败时返回句柄 NULL
* 其它说明： 无
**************************************************************************/
MEX_PHS_SMS_HANDLE Phs_SmsSendStart(Mex_Phs_Sms_Send_Req *req);

/**************************************************************************
* 函数名称： Phs_SmsSendRetry
* 功能描述： 尝试重新发送短信
* 参数说明： [IN] handle 发送短信的句柄
* 返 回 值： 是否成功
*            [bool:TRUE]  成功
*            [bool:FALSE] 失败
* 其它说明： 无
**************************************************************************/
bool Phs_SmsSendRetry(MEX_PHS_SMS_HANDLE handle);

/**************************************************************************
* 函数名称： Phs_SmsSendFinish
* 功能描述： 发送短信完成
* 参数说明： [IN] handle 发送短信的句柄
* 返 回 值：是否成功
*            [bool:TRUE]  成功
*            [bool:FALSE] 失败
* 其它说明： 无
**************************************************************************/
bool Phs_SmsSendFinish(MEX_PHS_SMS_HANDLE handle);

// 模拟收到一条短信
// 参数1: [IN] SIM卡编号
// 参数2: [IN] 短信中心号码, eg:+8613800210500
// 参数3: [IN] 短信号码, eg: 13800138000
// 参数4: [IN] 短信内容, 最大支持70个字符
// 参数5: [IN] 短信时间
bool Phs_NewSms(E_MEX_Sim_Card_ID eSimId, const char* pSmsCenter, const char* pSmsNumber, const wchar_t *pSmsContent, const Mex_DateTime *pDt);

//apis for phone book
/**************************************************************************
* 函数名称： Phs_GetPhbEntryIndex
* 功能描述： 获取电话簿条目索引
* 参数说明： [OUT:ppuStoreIndex]		输出电话簿条目索引
*            [OUT:puEntryCount]		电话簿条目数
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明：
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntryIndex(uint16** ppuStoreIndex, uint16* puEntryCount);


/**************************************************************************
* 函数名称： Phs_SearchPhbEntry
* 功能描述： 根据关键字检索电话簿
* 参数说明： [IN:szKeyword]			检索关键字
*            [IN:bIsPinyin]			是否拼音检索
*            [IN:eLocation]			检索电话簿条目的位置，全部、手机、SIM1或SIM2
*            [OUT:ppuSearchBitmask]		检索结果的掩码数组
*            [OUT:puResultCount]		检索结果的条目数
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_SearchPhbEntry(wchar_t* szKeyword, bool bIsPinyin, E_Mex_Phs_Phb_Location eLocation, uint8** ppuSearchBitmask, uint16* puResultCount);

/**************************************************************************
* 函数名称： Phs_GetPhbEntry
* 功能描述： 获取电话簿条目的内容
* 参数说明： [IN:uIndex]				单条电话簿条目的索引号
*            [OUT:psName]				姓名
*            [OUT:psNumber]			号码
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* 函数名称： Phs_GetPhbEntryEx
* 功能描述： 获取电话簿条目的详细内容
* 参数说明： [IN:uIndex]				单条电话簿条目的索引号
*            [OUT:psEntry]				姓名，号码，家庭电话，公司电话，E-Mail，传真等
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry);

/**************************************************************************
* 函数名称： Phs_GetPhbMemoryStatus
* 功能描述： 获取指定位置电话簿的存储状态
* 参数说明： [IN:eLocation]			获取电话簿条目的位置，全部、手机、SIM1或SIM2
*            [OUT:pTotal] 总大小
*            [OUT:pUsed] 已使用大小
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_GetPhbMemoryStatus(E_Mex_Phs_Phb_Location eLocation, uint32 *pTotal, uint32 *pUsed);

/**************************************************************************
* 函数名称： Phs_AddPhbEntry
* 功能描述： 添加电话簿条目
* 参数说明： [IN:eLocation]			电话簿条目的位置，全部、手机、SIM1或SIM2
*            [IN:psName]				姓名
*            [IN:psNumber]				号码
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_AddPhbEntry(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* 函数名称： Phs_AddPhbEntryEx
* 功能描述： 添加电话簿条目
* 参数说明： [IN:eLocation]			电话簿条目的位置，全部、手机、SIM1或SIM2
*            [IN:sEntry]				姓名，号码，家庭电话，公司电话，E-Mail，传真等
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_AddPhbEntryEx(E_Mex_Phs_Phb_Location eLocation, Mex_Phs_Phb_Entry* sEntry);

/**************************************************************************
* 函数名称： Phs_UpdatePhbEntry
* 功能描述： 更新电话簿条目
* 参数说明： [IN:uIndex]				单条电话簿条目的索引号
*            [IN:psName]				姓名
*            [IN:psNumber]				号码
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_UpdatePhbEntry(uint16 uIndex, Mex_Phs_Phb_Name* psName, Mex_Phs_Phb_Number* psNumber);

/**************************************************************************
* 函数名称： Phs_UpdatePhbEntryEx
* 功能描述： 更新电话簿条目
* 参数说明： [IN:uIndex]				单条电话簿条目的索引号
*            [IN:psEntry]				姓名，号码，家庭电话，公司电话，E-Mail，传真等
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_UpdatePhbEntryEx(uint16 uIndex, Mex_Phs_Phb_Entry* psEntry);

/**************************************************************************
* 函数名称： Phs_RemovePhbEntry
* 功能描述： 根据索引删除电话簿条目
* 参数说明： [IN:uIndex] 条目索引值
* 返 回 值： E_Mex_Phs_Phb_Result
* 其它说明： 无
**************************************************************************/
E_Mex_Phs_Phb_Result Phs_RemovePhbEntry(uint16 uIndex);

//获取群组信息
//参数1：[IN] groupIndex 群组索引
//参数2：[OUT] pGroup 输出群组详细信息
//返回：E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_GetPhbGroup(uint16 groupIndex, Mex_Phs_Phb_Group *pGroup);

//注册电话薄回调函数
//参数1：[IN] pCallBack 回调函数指针
//返回：E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_SetPhbCB(Mex_Phs_Phb_CB pCallBack);

//清除电话薄回调函数
//参数1：无
//返回：E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_ClearPhbCB(void);

//apis for hook message
/**************************************************************************
* 函数名称： mex_SetHookMsg
* 功能描述： 设置需要Hook的MSG以及其发送模块，并注册Hook的回调函数用来分
*            析并判断是否允许该MSG继续被正常处理
* 参数说明：
*            [IN:eMsgId]     当前需要处理的MSG的ID
*            [IN:eSimId]     需要拦截来自哪一个SIM卡的消息
*            [IN:hkFuncPtr]  Hook的回调函数指针
*            [OUT:pHkInfo]   Hook信息结构体指针
* 返 回 值：
*            [bool:true]	成功
*            [bool:false]	失败
* 其它说明： 无
**************************************************************************/
bool Phs_SetHookMsg(E_Mex_Hk_Msg_Id eMsgId, E_MEX_Sim_Card_ID eSimId, Mex_Hk_Func_Ptr hkFuncPtr, Mex_Hk_Info_Ptr p);

/**************************************************************************
* 函数名称： Phs_UnhookMsg
* 功能描述： 取消指定的Hook
* 参数说明： [IN:p] Hook的信息的指针
* 返 回 值： [bool:TRUE]  成功
*            [bool:FALSE] 失败
* 其它说明： 无
**************************************************************************/
bool Phs_UnhookMsg(Mex_Hk_Info_Ptr p);

// APIs for call log (Call History)
/**************************************************************************
* 函数名称： Phs_GetNumOfCallLog
* 功能描述： 获取通话记录的条数
* 参数说明： [IN] simId SIM卡
*            [IN] callType 通话记录类型
* 返 回 值： 通话记录的条数
* 其它说明： 无
**************************************************************************/
uint32 Phs_GetNumOfCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);

/**************************************************************************
* 函数名称： Phs_GetCallLog
* 功能描述： 获取指定的通话记录
* 参数说明： [IN] simId SIM卡
*            [IN] callType 通话记录类型
*            [IN] index 通话记录索引(第index 条通话记录)
*            [OUT] callLog 返回指定的通话记录
* 返 回 值：是否成功
*            [bool:TRUE]  成功
*            [bool:FALSE] 失败
* 其它说明： 无
**************************************************************************/
bool Phs_GetCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType, uint32 index, Mex_CallLog_Info *callLog);


//获取通话记录模块的状态
//参数1：[IN] simId SIM卡
//参数2：[IN] callType 通话记录类型
//返回：E_MEX_CALLOG_STATUS 通话记录模块的状态
E_MEX_CALLOG_STATUS Phs_GetCallLogStatus(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);

//更新通话记录
//参数1：[IN] simId SIM卡
//参数2：[IN] callType 通话记录类型
//返回：是否成功
bool Phs_UpdateCallLog(E_MEX_Sim_Card_ID simId, E_MEX_CALLLOG_TYPE callType);


/**************************************************************************
* 函数名称： Phs_Start_Camera
* 功能描述：初始化摄像头，并在硬件上开启摄像头
* 参数说明：
int32 offset_x 采集图像左上角起始位置x坐标
int32 offset_y 采集图像左上角起始位置x坐标
int32 iImgWidth 采集图像的宽度
int32 iImgHeight 采集图像的高度
int32 iDispWidth  显示图像的宽度  请不要超出屏幕
int32 iDispHeight 显示图像的高度  请不要超出屏幕
void *buffer	 图像数据保存到的缓冲区
int32 buf_size   缓冲区大小 一般缓冲区的大小为显示图像的像素数*2
* 返 回 值：见E_Mex_Phs_Camera_Code
* 其它说明： 无
**************************************************************************/

int8 Phs_Start_Camera(int32 offset_x, int32 offset_y, int32 iImgWidth, int32 iImgHeight, int32 iDispWidth, int32 iDispHeight, void *buffer,int32 buf_size);

/**************************************************************************
* 函数名称： Phs_Start_Preview
* 功能描述：开启预览模式
* 参数说明：
* 返 回 值：见E_Mex_Phs_Camera_Code
* 其它说明： 无
**************************************************************************/

int8 Phs_Start_Preview(void);

/**************************************************************************
* 函数名称： Phs_Stop_Preview
* 功能描述：停止预览
* 参数说明：
* 返 回 值：见E_Mex_Phs_Camera_Code
* 其它说明： 无
**************************************************************************/

int8 Phs_Stop_Preview(void);

/**************************************************************************
* 函数名称： Phs_Stop_Camera
* 功能描述：关闭摄像头
* 参数说明：
* 返 回 值：见E_Mex_Phs_Camera_Code
* 其它说明： 无
**************************************************************************/

int8 Phs_Stop_Camera(void);

/**************************************************************************
* 函数名称： Phs_Camera_Capture_To_File
* 功能描述：将当前图像保存为jpg格式图片并存放到缓冲区
* 参数说明：void **buffer  指向缓冲区首地址的指针
uint32 *captured_size 大小
* 返 回 值：见E_Mex_Phs_Camera_Code
* 其它说明： 无
**************************************************************************/

int8 Phs_Camera_Capture_To_Mem(void **buffer,uint32 *captured_size);

/**************************************************************************
* 功能描述：以下两个函数为调整焦距
* 参数说明：
**************************************************************************/

void Phs_Camera_Zoom_In(void);

void Phs_Camera_Zoom_Out(void);



/**************************************************************************
* 函数名称： Phs_Camera_Get_Rotate
* 功能描述：获取摄像头旋转方式
* 参数说明：
* 返 回 值：见E_Mex_Phs_Camera_Rotate
* 其它说明： 无
**************************************************************************/

E_Mex_Phs_Camera_Rotate Phs_Camera_Get_Rotate(void);
/**************************************************************************
* 函数名称： Phs_Camera_Get_Buffer
* 功能描述：获取缓冲区地址
* 参数说明：
* 返 回 值：数据缓冲区首地址
* 其它说明： 从这里拿到的图像数据，可能会因为摄像头的
旋转角度，而得到旋转的图像，如果需要得到与屏幕相同方向的图像
请根据Phs_Camera_Get_Rotate得到的旋转方式而转换数据
**************************************************************************/

void *Phs_Camera_Get_Buffer(void);
/**************************************************************************
* 函数名称： Phs_Snapshot_To_Jpg
* 功能描述：将当前层保存为jpg文件
* 参数说明：保存路径
* 返 回 值：
* 其它说明： 无
**************************************************************************/

void Phs_Snapshot_To_Jpg(wchar_t *fileName);

/**************************************************************************
* 函数名称： PhsSetDateTime
* 功能描述：设置当前系统时间
* 参数说明：Mex_DateTime pdt   设置的系统时间
* 返 回 值：是否设置成功
* 其它说明： 无
**************************************************************************/
bool PhsSetDateTime(Mex_DateTime pdt);

// 根据电话号码查找姓名
// 参数1: [IN] pNumber 电话号码
// 参数2: [OUT] pName 姓名
// 返回值: E_Mex_Phs_Phb_Result
E_Mex_Phs_Phb_Result Phs_LookUpPhbNumber(const Mex_Phs_Phb_Number *pNumber, Mex_Phs_Phb_Name *pName);

// 开始拔打电话
// 参数1: [IN] eSimId SIM卡编号
// 参数2: [IN] number 要拔出的电话号码
// 参数3: [IN] callback 拔打电话的回调函数
// 返回: MEX_OUTCALL_HANDLE 失败时返回NULL
MEX_OUTCALL_HANDLE Phs_OutCallStart(E_MEX_Sim_Card_ID eSimId, const char* number, MexOutCallCB callback);

// 释放Outgoing Call 句柄
// 参数1: [IN] handle Outgoing Call 句柄
// 返回: bool 是否成功
bool Phs_OutCallFinish(MEX_OUTCALL_HANDLE handle);

// 在通话过程中，模拟按键开始
// 参数1: [IN] handle Outgoing Call 句柄
// 参数2: [IN] keycode 按键'0'~'9', '*', '#'
// 返回: bool 是否成功
bool Phs_OutCallDtmfBegin(MEX_OUTCALL_HANDLE handle, char keycode);

// 在通话过程中，模拟按键结束
// 参数1: [IN] handle Outgoing Call 句柄
// 返回: bool 是否成功
bool Phs_OutCallDtmfEnd(MEX_OUTCALL_HANDLE handle);

// 短信模块是否准备好
// 参数1: 无
// 返回: bool 是否准备好
bool Phs_SmsReady(void);

// 获取MSGBOX中短信的数目
// 参数1: [IN] msgBox MSGBOX的类型
// 返回1: MEX_PHS_SMS_INDEX_INVALID 表示获取失败
// 返回2: 其它值 表示MSGBOX中短信的数目
uint16 Phs_GetNumOfSms(E_MEX_PHS_SMS_MSGBOX msgBox);

// 读取一条短信
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 参数3: [IN] callback 读取短信回调函数
// 返回: bool 是否成功
bool Phs_ReadSmsContent(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Read_Sms_CB callback);

// 获取Sms Address
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 返回: 指向Sms Addr 的指针, 失败时返回NULL
char *Phs_GetSmsAddr(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// 获取短信的日期
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 参数3: [OUT] pDt 输出短信日期
// 返回: bool 是否成功
bool Phs_GetSmsDateTime(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_DateTime *pDt);

// 获取短信的一小部分分内容(通常为前10个汉字)
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 返回: 指向短信内容的指针, 失败时返回NULL
bool Phs_GetSmsCache(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, wchar_t *pContent, uint16 iContentMemLen);

// 获取短信来自SIM1 或SIM2
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 返回: E_MEX_Sim_Card_ID, 当返回E_MEX_SIM_NONE时, 表示获取失败
// 注意:
// 1. 此接口仅对收件箱中的短信有用
E_MEX_Sim_Card_ID Phs_GetSmsSimId(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// 删除短信
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 参数3: [IN] callback 删除短信回调
// 返回: bool 是否成功
bool Phs_DelSms(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Del_Sms_CB callback);

// 取消息短信回调Mex_Phs_Read_Sms_CB & Mex_Phs_Del_Sms_CB
// 参数: 无
// 返回: 无
// 注意:
// 1. 当应用程序被强制退出时，必须取消短信回调
// 2. 当应用程序不再使用短信接口时，必须取消短信回调
void Phs_CancelSmsCB(void);

// 保存短信到Msg Box 中
// 注意:
// 1. 此接口不能将短信保存到收件箱(Inbox)中
// 2. 当手机系统中没有相应的信箱时，保存也会失败
bool Phs_SaveSms(E_MEX_PHS_SMS_MSGBOX msgBox, E_MEX_Sim_Card_ID simId, const wchar_t *pContent, const char *pAddr, Mex_Phs_Save_Sms_CB callback);

// 检查信箱是否存在
// 参数1: [IN] msgBox MSGBOX的类型
// 返回: bool 信箱是否存在
bool Phs_SmsMsgBoxExist(E_MEX_PHS_SMS_MSGBOX msgBox);

// 检查短信是否未读
// 参数1: [IN] msgBox MSGBOX的类型
// 参数2: [IN] index 短信索引
// 返回: true 表示短信未读, false 表示短信已读
bool Phs_SmsIsUnread(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

// 设置壁纸、屏保、开关机画面
// 参数1: [IN] eType 壁纸的类型
// 参数2: [IN] pFileName 图像文件的路径
// 参数3: [IN] bAbsPath 为真时，表示为绝对路径
// 返回: 设置是否成功
bool Phs_PhnSetWallPaper(E_MEX_WallPaper_Type eType, wchar_t *pFileName, bool bAbsPath);


//new camera api

// 将一块制定宽高的RGB565数据压缩为JPG文件
// 参数1: [IN] image_ptr 制定的RGB565格式的数据
// 参数2: [IN] image_width 图像的宽度
// 参数3: [IN] image_height 图片的高度
// 参数4: [IN] jpg_qty 图片压缩质量   参考枚举E_Mex_JPG_Encode_Qty
// 参数5: [IN] file_name 压缩后图片的保存路径
// 参数6: [IN] bAbsPath 参数5是否为绝对路径
// 返回: 是否成功
bool Phs_EncodeToJpeg(void *image_ptr,int32 image_width,int32 image_height, int8 jpg_qty, wchar_t *file_name,bool bAbsPath);

//开启摄像头(初始化以及摄像头物理加电)
// 参数1: [IN] offset_x 采集图像左上角起始位置x坐标
// 参数2: [IN] offset_y 采集图像左上角起始位置y坐标
// 参数3: [IN] iDispWidth 图片的宽度
// 参数4: [IN] iDispHeight 图片的高度
// 参数5: [IN] bPreviewUseSram 是否让摄像头使用SRAM   一般情况下设置为false
// 参数6: [IN] outputformat 摄像头输出的图像格式 参考枚举E_Mex_Phs_Camera_OutPut_Format
// 返回: 参考枚举E_Mex_Phs_Camera_Code
int8 Phs_Start_Camera_Ext(int32 offset_x, int32 offset_y, int32 iDispWidth, int32 iDispHeight, bool bPreviewUseSram, int8 outputformat);

//开启摄像头预览
//返回: 参考枚举E_Mex_Phs_Camera_Code
int8 Phs_Start_Preview_Ext(void);

//关闭摄像头预览
//返回: 参考枚举E_Mex_Phs_Camera_Code
int8 Phs_Stop_Preview_Ext(void);

//关闭摄像头
//返回: 参考枚举E_Mex_Phs_Camera_Code
int8 Phs_Stop_Camera_Ext(void);

//获取摄像头采集到的数据
//返回 采集到的图像数据  图像格式，大小为Phs_Start_Camera_Ext设置的参数
//说明 为了保证数据的同步，请在每次需要使用数据时调用一个该函数，该函数返回的数据地址会发生变化
void *Phs_GetCameraOutPutData(void);

//获取摄像头采集到的数据
//返回 采集到的图像数据  图像格式，大小为Phs_Start_Camera_Ext设置的参数
//说明 为了保证数据的同步，请在每次需要使用数据时调用一个该函数，该函数返回的数据地址会发生变化
void *Phs_GetCameraOutPutData_Ext(uint32 *puiSize);

//设置摄像头的预览模式是否为夜间模式
//参数 bNightMode 是否为夜间模式
void Phs_SetCameraNightMode(bool bNightMode);

//在Hook回调函数中获取Hook Result
//参数: 无
//返回: Hook Result
uint8 Phs_GetHookResult(void);

// 设置情景模式
void Phs_SetPhoneProfile(int32 profileId);
#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
//请求摄像头拍照一桢数据
void Phs_Camera_Capture_Req(Mex_Camera_Cap_Req *req);


//获取当前情景模式响铃类型返回值参考E_Mex_Pro_Alert_Type
int8 Phs_Get_CurrProfile_AlertType(void);

//切换摄像头
//参数 是否切换为后置摄像头
void Phs_SwitchCamera(bool);

// 根据号码获取城市名
int32 Phs_Aq_getCity(char *pNumber, wchar_t *CityName);

// 无条件转接至pNumber,callback为结果回调,0失败,1成功
int32 Phs_Aq_DivertCall(char *pNumber, i_Callback_i callback);

// 取消无条件转接,callback为结果回调,0失败,1成功
int32 Phs_Aq_CancelDivert(i_Callback_i callback);

// 查询无条件转接,callback为结果回调,-1失败,其他值对应如下
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
* 函数名称： Phs_GetMissedCallNumber
* 功能描述： 获取未接来电数量(用户查看之后会归零)
* 参数说明： [NULL]
* 返 回 值： 未接来电数量
* 其它说明： 无
**************************************************************************/
uint32 Phs_GetMissedCallNumber(void);

/**************************************************************************
* 函数名称： Phs_ClearMissedCallNum
* 功能描述： 归零用户未接来电(用户查看之后会归零)
* 参数说明： [NULL]
* 返 回 值： 是否成功
* 其它说明： 无
**************************************************************************/
bool Phs_ClearMissedCallNum(void);

void Phs_Set_Can_PowerOff(bool IsCanPowerOff);

E_MEX_Phs_HW_State Phs_Get_Backlight_State(void);


/**************************************************************************
* 函数名称： Phs_ClearUnReadSms
* 功能描述： 归零用户未读短信(用户查看之后会归零)
* 参数说明： [NULL]
* 返 回 值： 是否成功
* 其它说明： 无
**************************************************************************/
bool Phs_ClearUnReadSms(void);
#endif
#if MEX_SDK_VER >= 1033
void Phs_MotionSensor_Start(MexLib_MotionSensor_CallBack callback,int16 x_ponit,int16 y_point,int16 motionSensitivity);
void Phs_MotionSensor_Stop(void);
#endif
#endif
#endif

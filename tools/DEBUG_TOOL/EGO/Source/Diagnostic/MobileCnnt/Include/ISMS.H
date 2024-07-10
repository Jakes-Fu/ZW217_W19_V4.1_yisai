// File Name:   ISMS.h 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 定义手机的短消息接口。
///////////////////////////////////////////////////////////////////////////////

#if !defined(_ISMS_H_)
#define _ISMS_H_

#pragma once

// ----------------------------------------------------------------------------
// 读短信息的进度指示, 主窗口如果需要,可处理此消息,消息响应体 OnXXX( WPARAM, LPARAM )
//  wParam表过程的状态， lParam 表位置
#define WM_SMS_PROGRESS  (WM_USER+2000)

// 状态
#define SMS_START        0   // lParam 表示总数
#define SMS_PROCEED      1   // lParam 表示当前数目
#define SMS_END          2   // lParam not used
// ----------------------------------------------------------------------------

// 定义支持的提示信息的语言
typedef enum _MULTI_LANG_E
{
    ML_CHINESE_SIMPLE = 0,
    ML_ENGLISH,
    ML_MAX_NUM
} MULTI_LANG_E;

// 定义网络的状态(说明: 这些状态的定义以AT用户手册而定，不能随便乱改)
typedef enum _NET_STATUS_E
{
    NET_UNREGISTERED = 0, // 未注册
    NET_REGISTERED,       // 已注册
    NET_SEARCHING    = 2, // 正在搜索
    NET_DENIED,           // 注册被拒绝
    NET_UNKNOWN,          // 未知
    NET_ROAMING      = 5, // 漫游
} NET_STATUS_E;

// 定义短信的模式(说明: 这些模式的定义以AT用户手册而定，不能随便乱改)
typedef enum _SMS_MODE_E
{
    SMS_MODE_PDU = 0,
    SMS_MODE_TEXT,
} SMS_MODE_E;

// 短信的存储位置
typedef enum _SMS_STORE_PLACE_E
{
    SMS_SP_MS = 0,
    SMS_SP_SIM,
	SMS_SP_SIM2,
	SMS_SP_SIM3,
	SMS_SP_SIM4,
    SMS_SP_UNKOWN,
} SMS_STORE_PLACE_E;

// 短信息的状态
typedef enum _SMS_STATUS_E
{
    SMS_STAT_UNREAD = 0,
    SMS_STAT_READ,
    SMS_STAT_UNSEND,
    SMS_STAT_SEND,
    SMS_STAT_UNKNOWN
} SMS_STATUS_E;

typedef enum _SMS_TP_STATUS_E
{
     SMS_RECEIVED_BY_SME = 0,     // the sms receive by SME successfuly
     SMS_NOT_CONFIRM_RECEIVED,  // the short message is forwarded by the SC to the SME but the SC is unable to confirm delivery
     SMS_SM_REPLACED_BY_SC,   // short message replaced by the SC
     SMS_SEND_LATER,          // temporary error happened while forward short message, transfer it again later
     SMS_NOT_SEND_AGAIN,      // permanent error happened while forward short message, SC is not making any more transfer attempts
     SMS_INVALID_STATUS_REPORT // the invalid status report 
} SMS_TP_STATUS_E;

typedef enum _SMS_ERROR_E
{
	SMS_E_NULL = 0,
	SMS_E_CONNECTION_FAIL,
	SMS_E_DISABLEECHO_FAIL, 
	SMS_E_SETMODE_FAIL,
	SMS_E_QUERYNETSTATUS_FAIL, 

	// [[ 注意：不可更改网络状态的顺序
    SMS_E_NET_UNREG,  //Unregistered network!
	SMS_E_NET_REG,     //Registered network!
	SMS_E_NET_SEARCH,  //Searching network!
	SMS_E_NET_DENIED,  //Denied register network!
	SMS_E_NET_UNKNOWN, //Unknown status!
	SMS_E_NET_ROAMING, //Roaming network!
	// ]
		
	SMS_E_QUERY_CENTER, //Query ServiceCenter Fail
	SMS_E_SET_CENTER,   //Set ServiceCenter Fail			
	SMS_E_REG_INCSMS,   //Register incoming SMS Fail			
	SMS_E_SELECT_SP,    //Select SMS's store place Fail
	SMS_E_QUERYCAPACITY,//Query capacity Fail			
	SMS_E_READALLSMS,   //Read all SMS Fail
	SMS_E_READSMS,      //Read SMS Fail			
	SMS_E_SENDSMS,      //Send SMS Fail			
	SMS_E_DELETESMS,    //Delete SMS Fail
	SMS_E_DELETEALLSMS,  //Delete All SMS Fail
	SMS_E_SET_ACTIVE_SIM // Set active SIM
}SMS_ERROR_E;

const int SMS_NUM_MAX_LEN     = 20;  // 号码的最大长度
const int SMS_CONTENT_MAX_LEN = 160; // 短信内容的最大长度(字节数)
const int SMS_ASCII_MAX_LEN   = 160; // ASCII码短信内容的最大长度(字节数)
const int SMS_UCS2_MAX_LEN    = 140; // ucs2码短信内容的最大长度(字节数)
const int SMS_DATE_MAX_LEN    = 19;  // 日期的最大长度

// 定义短信的信息结构，发送及接收时用到
typedef struct _SMS_INFO_T
{
    // 发送或接收的电话号码
    // 用作发送时必须设置此项
    _TCHAR szNumber[SMS_NUM_MAX_LEN + 1];      

    // 短信内容中是否有两个字节表示一个字符的(比如一个汉字)，这里没有
    // 采用自己判断是因为有可能内容中含有非可见字符(比如0x90等);
    // !!! 假如此值为TRUE，发送时我们需要将其转换为Unicode编码方式 !!!
    // 用作发送时必须设置此项
    BOOL bIsUcs2;                
    
    // 短信内容的有效长度(字节数)
    // 用作发送时必须设置此项
    int  nContentLen;                        
    // 短信内容
    // 用作发送时必须设置此项
    _TCHAR szContent[SMS_CONTENT_MAX_LEN + 1]; // 短信内容

	// 短信内容是不具有头信息
	BOOL bUdhi;
	// 短信的拆合信息
	int  nRef;        //长短信拆分后的统一标识
	int  nTotal;        //长短信拆分后的总条数
	int  nSeq;    //短信序列号
    // 短信是否有状态报告
	BOOL bRSS;

    // 短信有效期，以小时为单位(Hours)
    // 用作发送时可选的设置项，默认为24小时(一天)
    int  nValidDate;

    //
    // 该条短信在存储位置中的索引(只对用在接收时有效)
    int  nIndex;
    // 接收到的短信的日期
    _TCHAR szDate[SMS_DATE_MAX_LEN + 1];
    // 该短信的存储位置(只对用在接收时有效)
    SMS_STORE_PLACE_E eStorePlace;
    // 该短信的状态(只对用在接收时有效)
    SMS_STATUS_E eStatus;

	BOOL bExistSC;
} SMS_INFO, * PSMS_INFO;



class IPort;
class ISMS
{
public:
    virtual BOOL         StartLog( LPCTSTR pszFileName ) = 0;
    virtual void         StopLog( void )                 = 0;

    // 设置接受进度信息的窗口句柄, 上层可依据需要来设置此值
    virtual void         SetWndForProgress( HWND hWnd ) = 0;
	virtual void         SetUIThreadForProgress(CWinThread * pThread) = 0;

    // 设置端口连接通道，如果需要不同的连接，可以更改此接口，并更改本库中
    // 用到的发送及接收的接口
    virtual void         SetPort( IPort * pPort ) = 0;
	virtual IPort *      GetPort( void ) = 0;
    // 检查是否连接手机
    virtual BOOL         CheckConnection(void) = 0;

    // 禁止命令的回应
    virtual BOOL         DisableEcho(void) = 0;

    // 查询网络状态
    virtual NET_STATUS_E QueryNetStatus(void) = 0;
    // 获取网络状态描述
	// 考虑到多国语言的问题，在GUI端给出描述字符串
    //virtual LPCTSTR      GetNetStatusDescription( NET_STATUS_E eNetStatus ) = 0;

    // 查询短信中心号码
    virtual LPCTSTR      QueryServiceCenter(void) = 0;
    // 设置短信中心号码
	virtual BOOL         SetServiceCenter( LPCTSTR pszCenter ) = 0;

    // 设置短信的模式(Text或PDU)
    virtual BOOL         SetSMSMode( SMS_MODE_E eSMSMode = SMS_MODE_TEXT ) = 0;
    // 注册MS收到短信后自动通知DTE功能
    virtual BOOL         RegisterIncomingSMS(void) = 0;

    // 选择短信的存储位置(手机存储器或SIM卡)
    virtual BOOL         SelectStorePlace( SMS_STORE_PLACE_E eStorePlace ) = 0;

	// 选择短信的存储位置(手机存储器或SIM卡)
    virtual BOOL         SelectStorePlaceAll( SMS_STORE_PLACE_E eStorePlaceRD, SMS_STORE_PLACE_E eStorePlaceWS ) = 0;

    // 获取短信的容量(总数，已使用个数)
    virtual BOOL         QueryCapacity( int &nTotal, int &nUsed ) = 0;

	// 获取短信的存储区以及容量(总数，已使用个数)
    virtual LPCSTR         QueryStoreCapacity() = 0;

    //
    virtual void         ClearAllIncomingIndex(void) = 0;
    // 获取刚刚收到或最近收到的短信息的ID(此函数可供循环调用)
    virtual BOOL         GetIncomingIndex( int &nIndex ) = 0;
    // 读取所有短信息
    virtual BOOL         ReadAllSMS(int &nCount) = 0;
    // 获取读取后的短信息
    virtual PSMS_INFO    GetSMS( int nIndex ) = 0;

    // 读取指定的短信息
    virtual BOOL         ReadSMS( PSMS_INFO pSMSInfo, int nIndex ) = 0;

    // 发送短信息, bMore表示发送完本条后，是否继续发送其他消息(不释放链路，效率高些)
    virtual BOOL         SendSMS( PSMS_INFO pSMSInfo, BOOL bMore = FALSE ) = 0;

    // 删除短信息
    virtual BOOL         DeleteSMS( int nIndex ) = 0;
    // 删除全部短信息
    virtual BOOL         DeleteAllSMS(void) = 0;
	// 设置状态报告
	virtual void         SetSRR(BOOL bSRR=FALSE) =0;
    
    // 获取操作失败的信息
    virtual DWORD        GetErrCode(void) const = 0;

    virtual void         Release(void) = 0;

	virtual int          GetUnusedRefNum(void) = 0;
	
	// Set active sim
	virtual BOOL         SetActiveSim(SMS_STORE_PLACE_E eStorePlace) = 0;
	
	virtual BOOL		 ParseSMSContent(LPSTR pBuf,PSMS_INFO pSMSInfo) = 0;

	//teana hu 2009.08.04
	virtual BOOL		 ParseSMSStatus(LPSTR pBuf,PSMS_INFO pSMSInfo) = 0;

	virtual BOOL		 SendLongSmsPreSet(LPSTR lpParam) = 0;

public:
	ISMS() {};
	virtual ~ISMS() {};
};

#ifdef  SMS_EXPORTS
#define SMS_API extern "C" __declspec(dllexport)
#else
#define SMS_API extern "C" __declspec(dllimport)
#endif

SMS_API BOOL CreateSMS( ISMS ** psms );

#endif // _ISMS_H_

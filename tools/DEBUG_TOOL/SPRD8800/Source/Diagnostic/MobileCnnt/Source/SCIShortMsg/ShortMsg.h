// File Name:   ShortMsg.h 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 定义手机的短消息接口
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHORTMSG_H__3327AF64_04AA_4DF0_AA4B_19B847F3FAFA__INCLUDED_)
#define AFX_SHORTMSG_H__3327AF64_04AA_4DF0_AA4B_19B847F3FAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISMS.H"
#include "Log.h"
#include "MsgIDDef.h"
#include "Converter.h"

#pragma  warning(push,3)
#include <deque>
#pragma  warning(pop)

class CCritialSection
{
public:
    CCritialSection( CRITICAL_SECTION &crs ) { m_pCRS = &crs; ::EnterCriticalSection(m_pCRS); }
    ~CCritialSection() { ::LeaveCriticalSection(m_pCRS); }

private:
    CRITICAL_SECTION * m_pCRS;
};

class CShortMsg : public ISMS  
{
public:

    virtual BOOL         StartLog( LPCTSTR pszFileName );
    virtual void         StopLog( void );

    // 设置接受进度信息的窗口句柄, 上层可依据需要来设置此值
    virtual void         SetWndForProgress( HWND hWnd );
	virtual void         SetUIThreadForProgress(CWinThread * pUIThread);

	// 设置端口连接通道，如果需要不同的连接，可以更改此接口，并更改本库中
    // 用到的发送及接收的接口
    virtual void         SetPort( IPort * pPort );
    virtual IPort *      GetPort( void ) { return m_pPort; }

    // 检查是否连接手机
    virtual BOOL         CheckConnection(void);

    // 禁止命令的回应
    virtual BOOL         DisableEcho(void);

    // 查询网络状态
    virtual NET_STATUS_E QueryNetStatus(void);
    // 获取网络状态描述
	// 考虑到多国语言的问题，在GUI端给出描述字符串
    //virtual LPCTSTR      GetNetStatusDescription( NET_STATUS_E eNetStatus );

    // 查询短信中心号码
    virtual LPCTSTR      QueryServiceCenter(void);
    // 设置短信中心号码
	virtual BOOL         SetServiceCenter( LPCTSTR pszCenter );

    // 设置短信的模式(Text或PDU)
    virtual BOOL         SetSMSMode( SMS_MODE_E eSMSMode = SMS_MODE_TEXT );
    // 注册MS收到短信后自动通知DTE功能
    virtual BOOL         RegisterIncomingSMS(void);

    // 选择短信的存储位置(手机存储器或SIM卡)
    virtual BOOL         SelectStorePlace( SMS_STORE_PLACE_E eStorePlace );

	virtual BOOL         SelectStorePlaceAll( SMS_STORE_PLACE_E eStorePlaceRD, SMS_STORE_PLACE_E eStorePlaceWS );

    // 获取短信的容量(总数，已使用个数)
    virtual BOOL         QueryCapacity( int &nTotal, int &nUsed );

	// 获取短信的存储区以及容量(总数，已使用个数)
    virtual LPCSTR         QueryStoreCapacity( );

    //
    virtual void         ClearAllIncomingIndex(void);
    // 获取刚刚收到或最近收到的短信息的ID(此函数可供循环调用)
    virtual BOOL         GetIncomingIndex( int &nIndex );

    // 读取所有短信息
    virtual BOOL         ReadAllSMS(int &nCount);
    // 获取读取后的短信息
    virtual PSMS_INFO    GetSMS( int nIndex );

    // 读取指定的短信息
    virtual BOOL         ReadSMS( PSMS_INFO pSMSInfo, int nIndex );

    // 发送短信息, bMore表示发送完本条后，是否继续发送其他消息(不释放链路，效率高些)
    virtual BOOL         SendSMS( PSMS_INFO pSMSInfo, BOOL bMore = FALSE );

    // 删除短信息
    virtual BOOL         DeleteSMS( int nIndex );
    // 删除全部短信息
    virtual BOOL         DeleteAllSMS(void);

	virtual void         SetSRR(BOOL bSRR=FALSE);
    
    // 获取操作失败的信息
    virtual DWORD        GetErrCode(void) const { return m_dwErrCode; }

    virtual void         Release(void);
	// 获得长短信未用的参考号码
	virtual int          GetUnusedRefNum(void);
	// set active sim
	virtual BOOL         SetActiveSim(SMS_STORE_PLACE_E eStorePlace);
	
	virtual BOOL		 ParseSMSContent(LPSTR pBuf, PSMS_INFO pSMSInfo);

	//teana hu 2009.08.04
	virtual BOOL		 ParseSMSStatus(LPSTR pBuf,PSMS_INFO pSMSInfo);

	virtual BOOL		 SendLongSmsPreSet(LPSTR lpParam);

public:
	CShortMsg();
	virtual ~CShortMsg();
protected:

	BOOL ParseAllSMS_TXT( LPSTR pBuf,  int nBufSize);
    BOOL ParseAllSMS_PDU( LPSTR pBuf,  int nBufSize);

    BOOL ParseSMS_TXT( LPSTR pBuf,  int nBufSize, PSMS_INFO pSMSInfo);
    BOOL ParseSMS_PDU( LPSTR pBuf,  int /* nBufSize */, PSMS_INFO pSMSInfo);
    
    BOOL ParsePDU( LPSTR pBuf, int nBufLen, int nBodyLen, PSMS_INFO pSMSInfo );
    void PDUConvert( LPSTR pszPDU );
    int  Transfer7Bitsto8Bits( LPBYTE tpdu_ptr, BYTE sms_length, LPBYTE offset_octets_ptr,
                               LPBYTE offset_bits_ptr, LPBYTE user_data_ptr );
    BYTE GetBits( LPBYTE byte_in, BYTE bits_length, LPBYTE offset_bits, LPBYTE offset_octets );
	
	int Transfer8Bitsto7Bits(const char* pSrc, unsigned char* pDst, int nSrcLength);
   
	void ParseIndex(  LPSTR &pBuf, PSMS_INFO psms );
	int  ParseCodec(  LPSTR &pBuf, PSMS_INFO /* psms */ );
	void ParseStatus( LPSTR &pBuf, PSMS_INFO psms );
	void ParseNum(    LPSTR &pBuf, PSMS_INFO psms );
	void ParseSRR(    LPSTR &pBuf, PSMS_INFO psms );
	void SkipUnused(  LPSTR &pBuf );
    void ParseDate(   LPSTR &pBuf, PSMS_INFO psms );
	void ParseContent(LPSTR &pBuf, int nBufSize, PSMS_INFO psms, int nCodec );
	void GetSRRContent( int nIndx, CString &strContent );

    BOOL SendSMS_TXT( PSMS_INFO pSMSInfo, BOOL bMore );
    BOOL SendSMS_PDU( PSMS_INFO pSMSInfo, BOOL bMore );

    void ProcRcvedContent(LPSTR pContent, int &nLen);
    void ProcSendContent(PSMS_INFO psms, LPSTR pszSend, int nSMSSize);

    SMS_STATUS_E GetSMSStatus(LPCSTR pszState);
	
	int GetSMSValidDate(int nHours);

    LPSTR FindStr(LPCSTR pSrc, int nSrcLen, LPCSTR pDes, int nDesLen);
    // 执行发送并等待结果返回
    // 此函数要求返回结果中含有"OK"或">"，否则此函数永远不会返回TRUE
    // 接收的内容存放至RcvBuf中
	BOOL ExecuteCmd(LPCSTR pszCmd, DWORD dwDelayTime = 200);

    // 
    BOOL ParseIncomingIndex( LPCSTR pszBuf, int nLen );
	
	BYTE Map7BitCharTo8Bit(LPBYTE lpData_in);
	
	BOOL IsBM();
 
    // 依据ID获取提示信息
private:

    CLog    m_Log;

    HWND    m_hProgressWnd;
	CWinThread * m_pUIThread;
    // 
	IPort * m_pPort;


    //LPCTSTR      m_pszErrMsg;
	DWORD          m_dwErrCode;

    ///////////////////////////////////////////////////////////////////////////
    // 定义发送接收的缓冲区
    // 需要发送命令时，我们使用SendBuf，接收到的数据我们放在RcvBuf中
	enum { 
        MAX_BUF_LEN = 380, 
    };
    char m_szSendBuf[MAX_BUF_LEN + 1];
	char m_szRcvBuf[MAX_BUF_LEN + 1];
    // 接收到的数据的长度
    DWORD m_dwRcvSize; 
    ///////////////////////////////////////////////////////////////////////////
    
    // 当前选择的存储区
    SMS_STORE_PLACE_E m_eStorePlace;

    // 选中的短信模式
    SMS_MODE_E m_eSMSMode;
	
	// 当前设置的状态报告: 0-无，1-有
	BOOL m_bSRR;

    int       m_nUsedCount;
	PSMS_INFO m_pSMSInfo;

    std::deque<int>m_deqIncomingIdx;

    CRITICAL_SECTION m_CriticalSection;
	//存储“参考数字”
	enum { 
        MAX_REF_NUM = 255, 
    };
	BYTE m_arrRefNum[MAX_REF_NUM+1];
	
private:
    // define the language type and prompt
	CConverter m_converter;
	_TCHAR m_szTempW[MAX_BUF_LEN + 1];
	char   m_szTempA[MAX_BUF_LEN + 1];

	int   m_nRetryTimes;
	DWORD m_nReadOneTimeout;
	DWORD m_nReadAllTimeout;
    int   m_nDelOneTimeout;
    int   m_nDelAllTimeout;
    int   m_nSendTimeout;
    int   m_nGetINCMTimeout;

    static const char AT_END_FLAG[];
    static const char AT_AT[];
	static const char AT_RES_OK[];
    static const char AT_DISABLE_ECHO[];
    static const char AT_RES_ERROR[];
	static const char AT_SMS_INPUT_PROMPT[];
	static const char AT_SMS_INPUT_END_FLAG[];

	static const char AT_SMS_MODE[];
	
	static const char AT_DIS_AUTOREP_NETSTS[]; 
    static const char AT_QUERY_NETSTS[];      
	static const char AT_RES_QUERY_NETSTS_PRE[];   

	// 中心号码
	static const char AT_GET_SMS_CENTER[];
	static const char AT_SET_SMS_CENTER[];
	static const char AT_SAVE_SMSCENTER[];
	static const char AT_RESTORE_SMSCENTER[];
	static const char AT_RES_SMS_CENTER[];

    // 短信自动提示
    static const char AT_REG_INMSG_INDEX[];
    static const char AT_RES_INMSG_PREFIX[];

	// 存贮位置及数量
	static const char AT_SMS_STORE_MS[];
    static const char AT_SMS_STORE_SIM[];  
	
	static const char AT_SEL_SMS_MS[];
	static const char AT_SEL_SMS_SIM[];
	static const char AT_SEL_SMS_MS2[];
	static const char AT_GET_SMS_NUM[];
	static const char AT_RES_SMS_PRE[];
	static const char AT_SEL_STORE[];
	static const char AT_GET_STORE[];

    static const char AT_GET_ALLSMS_TXT[];
    static const char AT_GET_ALLSMS_PDU[];
	static const char AT_RES_GET_ALLSMS_PRE[];
	static const char AT_GET_IDXSMS[];
	static const char AT_RES_GET_IDXSMS_PRE[];

	static const char AT_SMS_SENDMODE[];
	static const char AT_SMS_SENDMODE_SRR[];
	static const int  AT_SMS_UCS2;
	static const int  AT_SMS_ASCII;
	static const char AT_SMS_SEND_TXT[];
    static const char AT_SMS_SEND_PDU[];
	static const char AT_SMS_SEND_IDX[];

	static const char AT_DEL_ALLSMS[];
	static const char AT_DEL_IDXSMS[];

	static const char AT_SET_ACTIVE_SIM[];
	static const char AT_QUERY_ACTIVE_SIM[];
	static const char AT_RES_ACTIVE_SIM[];
	
	//teana hu 2009.05.13
	static const char AT_LONG_SMS_PRE_SET[];

    static const short SMS_PDU_7BIT;
    static const short SMS_PDU_8BIT;
    static const short SMS_PDU_UNICODE;

    static const LPCSTR m_arrSMSStatus[];
};

#endif // !defined(AFX_SHORTMSG_H__3327AF64_04AA_4DF0_AA4B_19B847F3FAFA__INCLUDED_)

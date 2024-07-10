// File Name:   ShortMsg.h 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: �����ֻ��Ķ���Ϣ�ӿ�
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

    // ���ý��ܽ�����Ϣ�Ĵ��ھ��, �ϲ��������Ҫ�����ô�ֵ
    virtual void         SetWndForProgress( HWND hWnd );
	virtual void         SetUIThreadForProgress(CWinThread * pUIThread);

	// ���ö˿�����ͨ���������Ҫ��ͬ�����ӣ����Ը��Ĵ˽ӿڣ������ı�����
    // �õ��ķ��ͼ����յĽӿ�
    virtual void         SetPort( IPort * pPort );
    virtual IPort *      GetPort( void ) { return m_pPort; }

    // ����Ƿ������ֻ�
    virtual BOOL         CheckConnection(void);

    // ��ֹ����Ļ�Ӧ
    virtual BOOL         DisableEcho(void);

    // ��ѯ����״̬
    virtual NET_STATUS_E QueryNetStatus(void);
    // ��ȡ����״̬����
	// ���ǵ�������Ե����⣬��GUI�˸��������ַ���
    //virtual LPCTSTR      GetNetStatusDescription( NET_STATUS_E eNetStatus );

    // ��ѯ�������ĺ���
    virtual LPCTSTR      QueryServiceCenter(void);
    // ���ö������ĺ���
	virtual BOOL         SetServiceCenter( LPCTSTR pszCenter );

    // ���ö��ŵ�ģʽ(Text��PDU)
    virtual BOOL         SetSMSMode( SMS_MODE_E eSMSMode = SMS_MODE_TEXT );
    // ע��MS�յ����ź��Զ�֪ͨDTE����
    virtual BOOL         RegisterIncomingSMS(void);

    // ѡ����ŵĴ洢λ��(�ֻ��洢����SIM��)
    virtual BOOL         SelectStorePlace( SMS_STORE_PLACE_E eStorePlace );

	virtual BOOL         SelectStorePlaceAll( SMS_STORE_PLACE_E eStorePlaceRD, SMS_STORE_PLACE_E eStorePlaceWS );

    // ��ȡ���ŵ�����(��������ʹ�ø���)
    virtual BOOL         QueryCapacity( int &nTotal, int &nUsed );

	// ��ȡ���ŵĴ洢���Լ�����(��������ʹ�ø���)
    virtual LPCSTR         QueryStoreCapacity( );

    //
    virtual void         ClearAllIncomingIndex(void);
    // ��ȡ�ո��յ�������յ��Ķ���Ϣ��ID(�˺����ɹ�ѭ������)
    virtual BOOL         GetIncomingIndex( int &nIndex );

    // ��ȡ���ж���Ϣ
    virtual BOOL         ReadAllSMS(int &nCount);
    // ��ȡ��ȡ��Ķ���Ϣ
    virtual PSMS_INFO    GetSMS( int nIndex );

    // ��ȡָ���Ķ���Ϣ
    virtual BOOL         ReadSMS( PSMS_INFO pSMSInfo, int nIndex );

    // ���Ͷ���Ϣ, bMore��ʾ�����걾�����Ƿ��������������Ϣ(���ͷ���·��Ч�ʸ�Щ)
    virtual BOOL         SendSMS( PSMS_INFO pSMSInfo, BOOL bMore = FALSE );

    // ɾ������Ϣ
    virtual BOOL         DeleteSMS( int nIndex );
    // ɾ��ȫ������Ϣ
    virtual BOOL         DeleteAllSMS(void);

	virtual void         SetSRR(BOOL bSRR=FALSE);
    
    // ��ȡ����ʧ�ܵ���Ϣ
    virtual DWORD        GetErrCode(void) const { return m_dwErrCode; }

    virtual void         Release(void);
	// ��ó�����δ�õĲο�����
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
    // ִ�з��Ͳ��ȴ��������
    // �˺���Ҫ�󷵻ؽ���к���"OK"��">"������˺�����Զ���᷵��TRUE
    // ���յ����ݴ����RcvBuf��
	BOOL ExecuteCmd(LPCSTR pszCmd, DWORD dwDelayTime = 200);

    // 
    BOOL ParseIncomingIndex( LPCSTR pszBuf, int nLen );
	
	BYTE Map7BitCharTo8Bit(LPBYTE lpData_in);
	
	BOOL IsBM();
 
    // ����ID��ȡ��ʾ��Ϣ
private:

    CLog    m_Log;

    HWND    m_hProgressWnd;
	CWinThread * m_pUIThread;
    // 
	IPort * m_pPort;


    //LPCTSTR      m_pszErrMsg;
	DWORD          m_dwErrCode;

    ///////////////////////////////////////////////////////////////////////////
    // ���巢�ͽ��յĻ�����
    // ��Ҫ��������ʱ������ʹ��SendBuf�����յ����������Ƿ���RcvBuf��
	enum { 
        MAX_BUF_LEN = 380, 
    };
    char m_szSendBuf[MAX_BUF_LEN + 1];
	char m_szRcvBuf[MAX_BUF_LEN + 1];
    // ���յ������ݵĳ���
    DWORD m_dwRcvSize; 
    ///////////////////////////////////////////////////////////////////////////
    
    // ��ǰѡ��Ĵ洢��
    SMS_STORE_PLACE_E m_eStorePlace;

    // ѡ�еĶ���ģʽ
    SMS_MODE_E m_eSMSMode;
	
	// ��ǰ���õ�״̬����: 0-�ޣ�1-��
	BOOL m_bSRR;

    int       m_nUsedCount;
	PSMS_INFO m_pSMSInfo;

    std::deque<int>m_deqIncomingIdx;

    CRITICAL_SECTION m_CriticalSection;
	//�洢���ο����֡�
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

	// ���ĺ���
	static const char AT_GET_SMS_CENTER[];
	static const char AT_SET_SMS_CENTER[];
	static const char AT_SAVE_SMSCENTER[];
	static const char AT_RESTORE_SMSCENTER[];
	static const char AT_RES_SMS_CENTER[];

    // �����Զ���ʾ
    static const char AT_REG_INMSG_INDEX[];
    static const char AT_RES_INMSG_PREFIX[];

	// ����λ�ü�����
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

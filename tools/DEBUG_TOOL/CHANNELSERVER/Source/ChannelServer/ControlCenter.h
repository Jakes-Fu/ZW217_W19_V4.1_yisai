// ControlCenter.h: interface for the CControlCenter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLCENTER_H__E4E65A84_F76A_414F_B4F9_1A9583E22A0F__INCLUDED_)
#define AFX_CONTROLCENTER_H__E4E65A84_F76A_414F_B4F9_1A9583E22A0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
//#include "DPSrv.h"

#include "ProtoChan.h"
#include "diagchan.h"
#include "SPLog.h"

#define MAX_TOOL_NAME 32
#define ANONY_MSG_COUNT 256 //106
#define MAX_REG_TOOL 32

#define TYPE_DEF_COUNT 20

#define WM_NOTIFY_ASSERT ( WM_USER+277 )
#define WM_EVENT_ENDIAN  ( WM_NOTIFY_ASSERT+1 )

#define MAX_IP_ADDR_LEN 16
#define MAX_HOST_NAME_LEN 64

#define MAX_DATA_COUNT  32

struct Client_Info
{
	TCHAR szHostName[MAX_HOST_NAME_LEN];
	TCHAR szIP[MAX_IP_ADDR_LEN];
	TCHAR szToolName[MAX_TOOL_NAME];
	DWORD bConnected;
	time_t time;		// Connect time
	DWORD dwBytesReceived;
	DWORD dwPackagesReeceived;
	DWORD dwBytesSent;
	DWORD dwPackagesSent;
	IProtocolPackage* lpPkg;
	BOOL bFirstReceive;			// Used to compatible with CommSock
};

class CControlCenter : public IProtocolObserver
{
public:
	CControlCenter();
	virtual ~CControlCenter();

public:
	int OnHiLvlData( LPVOID lpData, ULONG ulDataSize );
	int OnHiLvlEvent( unsigned int event,void* lpEventData );

	virtual int OnChannelEvent( unsigned int event,void* lpEventData );
	virtual int OnChannelData(LPVOID lpData, ULONG ulDataSize );

	void OnTimerInterval();
	UINT GetClientInfo( Client_Info** lppInfo );

    void TellClientsConnectState(BOOL bConnect);

	void StartLog();
	void StopLog();

public:
	int				  m_nTimeout;			// in second
	IProtocolChannel* m_lpLowChan;
	IProtocolChannel* m_lpLogChan;
	ICommChannel*     m_lpHighChan;
    
	//IDP* m_pConvertor;

/*	// [[ for self debug
	FILE *  m_pFileSmp;
	FILE *  m_pFileOrg;
	IProtocolPackage*  m_pSMPPacket;
	// ]]
*/
	HWND     m_hWnd;
	CWnd*    m_pWnd;
	int      m_nEndian;
	int      m_nChannelType;
	DWORD    m_dwDownData;
	DWORD    m_dwUpData;
	BOOL     m_bAssert;
	BOOL     m_bGotOneRightPacket;

protected:
	void ProcessData( DWORD,PRT_BUFF*);
	BOOL Reg( DWORD,BYTE );
	void Unreg( DWORD,BYTE,BOOL bClear=FALSE);
	void ProcessRequest( DWORD,PRT_BUFF*);

	void AddClient( DWORD dwClientId );
	void RemoveClient( DWORD dwClientId );
	void RefreshClientInfo();

	// Send to phone
	DWORD SendDown( PRT_BUFF* lpBuff );

	// Send to client tool
	DWORD SendUp( WORD wClientId,PRT_BUFF* lpBuff,BOOL bResponse = FALSE );
	DWORD SendUp( WORD wClientId,DWORD sn,BYTE type,BYTE subtype,BOOL bResponse = FALSE );
	DWORD SendUp( WORD wClientId,BYTE* lpBuff,DWORD dwSize );

	// Tell all clients the endian is decided
	void TellClientsEndian();

	void LoadTypeDef();
	bool IsLastPackage( DIAG_HEADER* lpHead );

	//BOOL ConvertEndian( PRT_BUFF* lpBuff,BOOL bToMs );
	BOOL OnItcReq(DWORD dwClientId,PRT_BUFF* lpBuff );
	// To check the input buffer contain a host info struct
	// sent by commsock
	BOOL IsCommSockData( BYTE* lpBuff );
	
protected:
	CMap<WORD,WORD,Client_Info*,Client_Info*> m_Clients;
	//CMap<DWORD,DWORD,PRT_BUFF*,PRT_BUFF*> m_Messages;
	WORD m_RegedMsg[ANONY_MSG_COUNT][MAX_REG_TOOL];
    WORD m_clientData[ANONY_MSG_COUNT][MAX_REG_TOOL];

	BYTE  m_TypeDef[TYPE_DEF_COUNT*2];
	DWORD m_nTypeDefCount;
	DWORD m_nSequenceNumber;

	CRITICAL_SECTION m_csClient;
	//CRITICAL_SECTION m_csMessage;
	//CRITICAL_SECTION m_csConvert;

	//long m_nOldToolCount;
	BOOL m_bLog;

	// Log messages received from low level channel
	CSpLog m_ChannelLog;

	// Log messages sent to client tool
	CSpLog m_ToolLog;

	// Log request and response
	CSpLog m_RequestLog;
};

#endif // !defined(AFX_CONTROLCENTER_H__E4E65A84_F76A_414F_B4F9_1A9583E22A0F__INCLUDED_)

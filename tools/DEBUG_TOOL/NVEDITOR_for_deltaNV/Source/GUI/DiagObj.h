// DiagObj.h: interface for the CDiagObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIAGOBJ_H__98D70C23_F74C_446E_B010_863E3583E3A0__INCLUDED_)
#define AFX_DIAGOBJ_H__98D70C23_F74C_446E_B010_863E3583E3A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)

#include "comm_def.h"
#include "Common.h"
#include "cmddef.h"
#include "DiagChan.h"
#include "SpLog.h"


#define DIAG_WAIT_TIME      3000

typedef enum
{
	DIAG_SUCCESS,
	DIAG_E_FAIL,
	DIAG_E_INVALIDPARAM,
	DIAG_E_NOT_CONNECTED,
	DIAG_E_TIMEOUT,
	DIAG_E_CHANNEL_ERROR,
	DIAG_E_CONNECTED,
	DIAG_E_CREATE_CHANNEL	

} DIAG_RETURN_VALUE_E;



#define  CONVERT_INT(Src,Dst)   {\
	(Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
	MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
	}

#define  CONVERT_SHORT(Src,Dst) {(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));}



class CDiagObj :public IProtocolObserver
{
public:
	CDiagObj(void);
	virtual ~CDiagObj(void);
public:
	/**
	*  Called when some channel event happened
	*  @param [in] event		: event id
	*  @param [in] lpEventData	: event data pointer,different event has different event data
	*
	*  @return value is not used now
	*/
	virtual int OnChannelEvent( unsigned int event,void* lpEventData );

	/**
	*  Called when some data packages received
	*  @param [in] lpData		: data pointer
	*  @param [in] ulDataSize	: data size
	*
	*  @return value is not used now
	*/
	virtual int OnChannelData(LPVOID lpData, ULONG ulDataSize );

public:

	DWORD	Open(LPCTSTR lpszConfig);
	void	Close();

	DWORD   ReadNVItem(WORD nNVItemID,BYTE **lpRcvData,DWORD &dwSize);
	DWORD   WriteNVItem(WORD nNVItemID,BYTE *lpSendData,DWORD dwSize);
	void	DestroyData(PRT_BUFF* lpData );  

	BOOL	RegMsg( unsigned char chType );
	BOOL	UnregMsg( unsigned char chType );
	
	BOOL	IsConnected();

	void	Log(LPCTSTR pszLog);

	LPTSTR  GetLastErrorMsg() {return m_LastErrorMsg;}

	void    ClearNVItemBuf();

private:
	IProtocolChannel *	m_pChannel;
	CHANNEL_ATTRIBUTE   m_tChannelAttr; 
	CSpLog m_Log;
	BOOL   m_bBigEndian;
	BOOL   m_bLog;
	int    m_nObserverID;
	BOOL   m_bOpen;

	_TCHAR m_LastErrorMsg[1024];
	int    m_nRetries;

	std::vector<PRT_BUFF*> m_vNVItemBuf;
	DWORD  m_dwNVItemSize;
	HANDLE m_hNVItemEvent;
	DWORD  m_dwReadNVItemTimeout;
	DWORD  m_dwWriteNVItemTimeout;

};


#endif // !defined(AFX_DIAGOBJ_H__98D70C23_F74C_446E_B010_863E3583E3A0__INCLUDED_)

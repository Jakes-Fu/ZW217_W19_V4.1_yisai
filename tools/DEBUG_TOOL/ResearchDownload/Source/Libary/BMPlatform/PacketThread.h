#ifndef _PACKETTHREAD_H__
#define _PACKETTHREAD_H__

#pragma once

#include "Global.h"
#include "BMPackage.h"
#include "Sparce2Raw.h"


typedef struct _ESCAPE_PACKET_T
{
	LPBYTE pBuf;
	DWORD  dwSize;
	unsigned __int64  llTotal;
}ESCAPE_PACKET_T,*ESCAPE_PACKET_PTR;

#define MAX_ESCAPE_PACKET_NUM  (25)
#define MAX_ESCAPE_PACKET_LEN  (3*1024*1024+16)

class CPacketThread
{
public:
	CPacketThread(void);
	~CPacketThread(void);
	BOOL Start(BOOL bTransCode = TRUE, BOOL bSupportRawData = FALSE, DWORD dwFlushSize = 0);
	void Stop();
	BOOL SetFileInfo(BMFileInfo* pBMFileInfo);
	BOOL GetOnePacket(ESCAPE_PACKET_PTR *pPacket);
	static int  ProcessSparseData(HANDLE h, void* pBuf, UINT32 nLen);

public:
	CBMPkg  m_pkg;

private:
	static DWORD WINAPI GetThreadFunc(LPVOID lpParam);
	DWORD    PacketFunc();
	DWORD    m_dwThreadID;
	HANDLE   m_hThread;
	BOOL	 m_bTransCode;

private:
	BOOL				m_bWorking;
	ESCAPE_PACKET_T		m_packet[MAX_ESCAPE_PACKET_NUM];	
	int					m_nCurPacketPos;
	int					m_nNextEmptyPacket;
	BMFileInfo *		m_pBMFileInfo;
	LPBYTE              m_pFileBuf;
    BOOL                m_bSupportRawData;
    DWORD               m_dwFlushSize;
	int					m_nCurPacketOffset;
	ESCAPE_PACKET_T		m_tmpPacket;
	//CRITICAL_SECTION	m_csLock;
	__int64				m_nTotal;
};

#endif

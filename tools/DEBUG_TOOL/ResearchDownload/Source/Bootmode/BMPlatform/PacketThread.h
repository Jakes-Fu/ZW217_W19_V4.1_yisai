#ifndef _PACKETTHREAD_H__
#define _PACKETTHREAD_H__

#pragma once

#include "Global.h"
#include "BMPackage.h"


typedef struct _ESCAPE_PACKET_T
{
	LPBYTE pBuf;
	DWORD  dwSize;
	unsigned __int64  llTotal;
}ESCAPE_PACKET_T,*ESCAPE_PACKET_PTR;

#define MAX_ESCAPE_PACKET_NUM  (100)
#define MAX_ESCAPE_PACKET_LEN  (128*1024+16)

class CPacketThread
{
public:
	CPacketThread(void);
	~CPacketThread(void);
	BOOL Start(BOOL bTransCode = TRUE);
	void Stop();
	BOOL SetFileInfo(BMFileInfo* pBMFileInfo);
	BOOL GetOnePacket(ESCAPE_PACKET_PTR *pPacket);

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
	//CRITICAL_SECTION	m_csLock;
};

#endif

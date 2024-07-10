#include "stdafx.h"
#include "PacketThread.h"

CPacketThread::CPacketThread(void)
{
	m_bTransCode = TRUE;
	m_bWorking = FALSE;
	m_dwThreadID = 0;
	m_hThread = NULL;
	m_nCurPacketPos = 0;
	m_nNextEmptyPacket = 0;
	m_pBMFileInfo = NULL;
	for(int i = 0; i< MAX_ESCAPE_PACKET_NUM; i++)
	{
		m_packet[i].pBuf = new BYTE[MAX_ESCAPE_PACKET_LEN];
		m_packet[i].dwSize = 0;
	}

	m_pFileBuf = NULL;
	m_pFileBuf = new BYTE[MAX_MAP_SIZE];

	//InitializeCriticalSection(&m_csLock);
	
}

CPacketThread::~CPacketThread(void)
{
	for(int i = 0; i< MAX_ESCAPE_PACKET_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_packet[i].pBuf);	
		m_packet[i].dwSize = 0;
	}
	Stop();
	SAFE_DELETE_ARRAY(m_pFileBuf);
	//DeleteCriticalSection( &m_csLock );
}

BOOL CPacketThread::Start(BOOL bTransCode/* = TRUE*/)
{
	if(m_hThread)
		Stop();

	m_bWorking = TRUE;
	m_nCurPacketPos = 0;
	m_nNextEmptyPacket = 0;
	m_bTransCode = bTransCode;
	m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)GetThreadFunc,(LPVOID)this,0,&m_dwThreadID);
	if(m_hThread != NULL)
		return TRUE;

	return FALSE;

}

void CPacketThread::Stop()
{
	m_bWorking = FALSE;
	m_bTransCode = TRUE;
	if(m_hThread)
	{
		WaitForSingleObject(m_hThread,10);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

}

BOOL CPacketThread::SetFileInfo(BMFileInfo* pBMFileInfo)
{
	if(pBMFileInfo == NULL)
		return FALSE;

	m_pBMFileInfo = pBMFileInfo;

	return TRUE;
}

BOOL CPacketThread::GetOnePacket(ESCAPE_PACKET_PTR *pPacket)
{		
RETRY:
	if(m_nNextEmptyPacket - m_nCurPacketPos > 0)
	{
		//EnterCriticalSection(&m_csLock);
		*pPacket = &(m_packet[m_nCurPacketPos%MAX_ESCAPE_PACKET_NUM]);
		m_nCurPacketPos++;
		//LeaveCriticalSection(&m_csLock);
		//SetEvent(m_hEventInput);
		return TRUE;
	}

	while(m_bWorking && m_nNextEmptyPacket ==  m_nCurPacketPos)
	{
		Sleep(1);
	}

	if(!m_bWorking)
		return FALSE;

	if(m_nNextEmptyPacket == m_nCurPacketPos)
		return FALSE;
	
	goto RETRY;
}


DWORD WINAPI CPacketThread::GetThreadFunc(LPVOID lpParam)
{
	CPacketThread *pThis = (CPacketThread *)lpParam;
	return pThis->PacketFunc();

}

DWORD CPacketThread::PacketFunc()
{
	BMFileInfo* pBMFileInfo = m_pBMFileInfo;

	unsigned __int64 llLeft = pBMFileInfo->llCodeSize;
	DWORD dwMaxLength		= pBMFileInfo->dwMaxLength;
	DWORD dwSize			= dwMaxLength;

	BOOL bNeedRemap			= FALSE;
	LPBYTE pCurData			= (LPBYTE)pBMFileInfo->lpCode;
	DWORD  dwLeft2			= pBMFileInfo->dwFirstMapSize;
	unsigned __int64 llTotal= 0;

	BM_PACKAGE bp;
	bp.header.type			= BSL_CMD_MIDST_DATA;

	if( 
		!pBMFileInfo->bChangeCode && 
		pBMFileInfo->dwFirstMapSize < pBMFileInfo->llCodeSize &&
		pBMFileInfo->hFDLCode != NULL
	  )
	{
		LARGE_INTEGER liOffset;
		bNeedRemap = TRUE;
		liOffset.QuadPart = pBMFileInfo->llCodeOffset;
		if (::SetFilePointer(pBMFileInfo->hFDLCode,liOffset.LowPart, &liOffset.HighPart,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			TCHAR szError[MAX_PATH] = {0};
			_stprintf(szError,_T("SetFilePointer failed in PacketFunc, ErrorCode=0x%x\n"),GetLastError());	
			m_bWorking = FALSE;
			return FALSE;
		}
	}
	else
	{
		dwLeft2 = (DWORD)llLeft;  // no file map
	}

	int nIndex =  0;

	if(dwMaxLength == 0)
		dwMaxLength = 0x1000;

	if(!bNeedRemap)	
	{
		while( llLeft > 0 && m_bWorking )
		{
			if(llLeft >= dwMaxLength )
			{
				dwSize = dwMaxLength ;    
			}
			else
			{
				dwSize = (DWORD)llLeft;
			}	

			bp.header.len = (unsigned short)dwSize;
			bp.data = pCurData;

			while(m_nNextEmptyPacket - m_nCurPacketPos >= 90 && m_bWorking)
			{				
				Sleep(1);
			}

			if(!m_bWorking)
			{
				return 0;
			}

			nIndex  = m_nNextEmptyPacket%MAX_ESCAPE_PACKET_NUM;

			m_pkg.Package(&bp,m_packet[nIndex].pBuf,(int *)&m_packet[nIndex].dwSize,m_bTransCode);
			llTotal = pBMFileInfo->llCodeSize - llLeft + dwSize;
			m_packet[nIndex].llTotal = llTotal;	
			m_nNextEmptyPacket++;
			llLeft  -= dwSize;
			pCurData += dwSize;
		}
	}
	else
	{
		do 
		{
			DWORD dwReadSize = llLeft > MAX_MAP_SIZE ? MAX_MAP_SIZE : (DWORD)llLeft;
			DWORD dwRead = 0;
			memset(m_pFileBuf,0,MAX_MAP_SIZE);
			if(!::ReadFile(pBMFileInfo->hFDLCode,m_pFileBuf,dwReadSize,&dwRead,NULL) || dwRead != dwReadSize)
			{
				m_bWorking = FALSE;
				return 0;
			}

			dwLeft2 = dwReadSize;

			pCurData = m_pFileBuf;

			while( dwLeft2 > 0 && m_bWorking )
			{
				if(dwLeft2 >= dwMaxLength )
				{
					dwSize = dwMaxLength ;    
				}
				else
				{
					dwSize = dwLeft2;				
				}	

				bp.header.len = (unsigned short)dwSize;
				bp.data = pCurData;

				while(m_nNextEmptyPacket - m_nCurPacketPos >= 90 && m_bWorking)
				{				
					Sleep(1);
				}

				if(!m_bWorking)
				{
					return 0;
				}

				nIndex		= m_nNextEmptyPacket%MAX_ESCAPE_PACKET_NUM;
				m_pkg.Package(&bp,m_packet[nIndex].pBuf,(int *)&m_packet[nIndex].dwSize,m_bTransCode);
				llTotal = pBMFileInfo->llCodeSize - llLeft + dwSize;
				m_packet[nIndex].llTotal = llTotal;		
				m_nNextEmptyPacket++;
				dwLeft2		-= dwSize;
				llLeft		-= dwSize;
				pCurData	+= dwSize;
			}

		} while(llLeft != 0);
	}
	return 0;
}
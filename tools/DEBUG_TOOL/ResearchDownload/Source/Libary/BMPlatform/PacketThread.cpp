#include "stdafx.h"
#include "PacketThread.h"

#pragma comment(lib, "Sparce2Raw.lib")

CPacketThread::CPacketThread(void)
{
	m_bTransCode = TRUE;
	m_bWorking = FALSE;
    m_bSupportRawData = FALSE;
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
	m_nCurPacketOffset = 0;

	m_tmpPacket.pBuf = new BYTE[MAX_ESCAPE_PACKET_LEN];
	m_tmpPacket.dwSize = 0;
	m_nTotal = 0;
	
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

BOOL CPacketThread::Start(BOOL bTransCode/* = TRUE*/, BOOL bSupportRawData/* = FALSE*/, DWORD dwFlushSize/* = 0*/)
{
	if(m_hThread)
		Stop();

	m_bWorking = TRUE;
	m_nCurPacketPos = 0;
	m_nNextEmptyPacket = 0;
	m_bTransCode = bTransCode;
    m_bSupportRawData = bSupportRawData;
    m_dwFlushSize = dwFlushSize;
	m_nCurPacketOffset = 0;
	m_nTotal = 0;
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
    DWORD dwMaxLength		= m_bSupportRawData ? m_dwFlushSize : pBMFileInfo->dwMaxLength;
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

				while(m_nNextEmptyPacket - m_nCurPacketPos >= (MAX_ESCAPE_PACKET_NUM - 5) && m_bWorking)
				{				
					Sleep(1);
				}

				if(!m_bWorking)
				{
					return 0;
				}

				nIndex  = m_nNextEmptyPacket%MAX_ESCAPE_PACKET_NUM;


				if (dwSize > MAX_ESCAPE_PACKET_LEN)
				{
					m_bWorking = FALSE;
					return 0;
				}


				if(!m_bSupportRawData)
				{
					m_pkg.Package(&bp,m_packet[nIndex].pBuf,(int *)&m_packet[nIndex].dwSize,m_bTransCode);
				}
				else
				{
					BOOL bOdd = FALSE;
					int nOddSize = dwSize;
					if(nOddSize % 2)
					{
						nOddSize++;
						bOdd = TRUE;
					}
					memcpy(m_packet[nIndex].pBuf, bp.data, dwSize);
					m_packet[nIndex].dwSize = nOddSize;
					if(bOdd)
					{
						m_packet[nIndex].pBuf[dwSize] = 0;    
					}
				}


				llTotal = pBMFileInfo->llCodeSize - llLeft + dwSize;
				m_packet[nIndex].llTotal = llTotal;	
				m_nNextEmptyPacket++;
				llLeft  -= dwSize;
				pCurData += dwSize;
			}
		}
		
	}
	else
	{
		DWORD dwRead = 0;
		DWORD dwSparceMagic = 0;

		if (_tcscmp(pBMFileInfo->szRepID, _T("super")) == 0)
		{
			if (!::ReadFile(pBMFileInfo->hFDLCode, &dwSparceMagic, sizeof(dwSparceMagic), &dwRead, NULL) || dwRead != sizeof(dwSparceMagic))
			{
				m_bWorking = FALSE;
				return 0;
			}

			if (HFILE_ERROR == SetFilePointer(pBMFileInfo->hFDLCode, -4, 0, FILE_CURRENT))
			{
				m_bWorking = FALSE;
				return 0;
			}
		}

		

		if (dwSparceMagic == SPARSE_HEADER_MAGIC && m_bSupportRawData)
		{
			CSparce2Raw csr;
			csr.SetWritePtr(ProcessSparseData);

			if (S2R_SUCCESS != csr.Simg2Img(pBMFileInfo->hFDLCode, (HANDLE)this))
			{
				m_bWorking = FALSE;
				return 0;
			}

			int nIndex		= m_nNextEmptyPacket % MAX_ESCAPE_PACKET_NUM;
			memcpy(m_packet[nIndex].pBuf, m_tmpPacket.pBuf, m_tmpPacket.dwSize);
			m_packet[nIndex].dwSize = m_tmpPacket.dwSize;
			m_nTotal += m_tmpPacket.dwSize;

			m_packet[nIndex].llTotal = m_nTotal;		
			m_nNextEmptyPacket++;			
		}
		else
		{
			do 
			{
				DWORD dwReadSize = 0;
				if(!m_bSupportRawData)
				{
					dwReadSize = llLeft > MAX_MAP_SIZE ? MAX_MAP_SIZE : (DWORD)llLeft;
				}
				else
				{
					dwReadSize = llLeft > m_dwFlushSize ? m_dwFlushSize : (DWORD)llLeft;
				}
				memset(m_pFileBuf,0,MAX_MAP_SIZE);
				if(!::ReadFile(pBMFileInfo->hFDLCode,m_pFileBuf,dwReadSize,&dwReadSize,NULL) || dwReadSize != dwReadSize)
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

					while(m_nNextEmptyPacket - m_nCurPacketPos >= (MAX_ESCAPE_PACKET_NUM - 5) && m_bWorking)
					{				
						Sleep(1);
					}

					if(!m_bWorking)
					{
						return 0;
					}

					nIndex		= m_nNextEmptyPacket%MAX_ESCAPE_PACKET_NUM;
					if(!m_bSupportRawData)
					{
						m_pkg.Package(&bp,m_packet[nIndex].pBuf,(int *)&m_packet[nIndex].dwSize,m_bTransCode);
					}
					else
					{
						BOOL bOdd = FALSE;
						int nOddSize = dwSize;
						if(nOddSize % 2)
						{
							nOddSize++;
							bOdd = TRUE;
						}
						memcpy(m_packet[nIndex].pBuf, bp.data, dwSize);
						m_packet[nIndex].dwSize = nOddSize;
						if(bOdd)
						{
							m_packet[nIndex].pBuf[dwSize] = 0;    
						}
					}
					llTotal = pBMFileInfo->llCodeSize - llLeft + dwSize;
					m_packet[nIndex].llTotal = llTotal;		
					m_nNextEmptyPacket++;
					dwLeft2		-= dwSize;
					llLeft		-= dwSize;
					pCurData	+= dwSize;
				}

			} while(llLeft != 0);
		}
		
	}
	return 0;
}

int CPacketThread::ProcessSparseData(HANDLE h, void* pBuf, UINT32 nLen)
{
	if (INVALID_HANDLE_VALUE == h)
	{
		return 0;
	}

	CPacketThread* pThread = (CPacketThread*)h;
	DWORD dwLeft2 = nLen;

	LPBYTE pCurData = (LPBYTE)pBuf;
	DWORD dwMaxLength = pThread->m_dwFlushSize;
	DWORD dwSize = 0;

	while( dwLeft2 > 0 && pThread->m_bWorking )
	{
		if(dwLeft2 >= dwMaxLength )
		{
			dwSize = dwMaxLength ;    
		}
		else
		{
			dwSize = dwLeft2;				
		}	

		while(pThread->m_nNextEmptyPacket - pThread->m_nCurPacketPos >= (MAX_ESCAPE_PACKET_NUM - 5) && pThread->m_bWorking)
		{				
			Sleep(1);
		}

		if(!pThread->m_bWorking)
		{
			return 0;
		}


		if ((pThread->m_nCurPacketOffset + dwSize) > dwMaxLength)
		{
			DWORD dwCurSize = dwMaxLength - pThread->m_nCurPacketOffset;
			DWORD dwLeftSize = dwSize - dwCurSize;
			memcpy(pThread->m_tmpPacket.pBuf+pThread->m_nCurPacketOffset, pCurData, dwCurSize);

			int nIndex		= pThread->m_nNextEmptyPacket % MAX_ESCAPE_PACKET_NUM;
			pThread->m_tmpPacket.dwSize = dwMaxLength;
			memcpy(pThread->m_packet[nIndex].pBuf, pThread->m_tmpPacket.pBuf, pThread->m_tmpPacket.dwSize);
			pThread->m_packet[nIndex].dwSize = pThread->m_tmpPacket.dwSize;
			pThread->m_nTotal += pThread->m_tmpPacket.dwSize;

			pThread->m_packet[nIndex].llTotal = pThread->m_nTotal;		
			pThread->m_nNextEmptyPacket++;				

			pThread->m_nCurPacketOffset = 0;

			memcpy(pThread->m_tmpPacket.pBuf+pThread->m_nCurPacketOffset, pCurData+dwCurSize, dwLeftSize);
			pThread->m_nCurPacketOffset += dwLeftSize;
			pThread->m_tmpPacket.dwSize = dwLeftSize;
			
		}
		else
		{
			memcpy(pThread->m_tmpPacket.pBuf+pThread->m_nCurPacketOffset, pCurData, dwSize);
			pThread->m_nCurPacketOffset += dwSize;
			pThread->m_tmpPacket.dwSize += dwSize;
		}

		dwLeft2		-= dwSize;
		pCurData	+= dwSize;
	}

	return nLen;
}
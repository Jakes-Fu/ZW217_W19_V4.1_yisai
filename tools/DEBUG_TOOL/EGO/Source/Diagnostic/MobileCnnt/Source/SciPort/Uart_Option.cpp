#include "stdafx.h"
#include "Uart.h"
#include <process.h>

const int  CUart::MAX_RETRY_NUM           = 8;

UINT WINAPI ThreadProc(
        LPVOID lpParameter   // thread data
)
{	
    CUart* pATHingeImp = (CUart*)lpParameter;

	SetEvent(pATHingeImp->m_hEventFlag);

    MSG msg;

	//PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	while(::GetMessage(&msg,NULL,pATHingeImp->m_dwMsgId,pATHingeImp->m_dwMsgId))
    {
		pATHingeImp->Receive((void*)msg.wParam, msg.lParam);
    }

	
    return 0;

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL CUart::CreateScene(HWND hWnd)
{
	if(m_hThread != NULL)
		return TRUE;

	//create the thread
    m_hThread = CreateMsgThread();
    if(NULL == m_hThread)
    {
        return FALSE;
    }

	//get the unique message id
	m_dwMsgId = RegisterWindowMessage(_T("UART Data Message"));
    if(0 == m_dwMsgId)
    {
        return FALSE;
    }

    m_hGuiWnd = hWnd;

    return TRUE;
}

BOOL CUart::DestroyScene()
{
    //detroy the message window
	if (m_dwThreadId != NULL)
	{  
		PostThreadMessage(m_dwThreadId,WM_QUIT,0,0);
		CloseHandle(m_hThread);
		m_hThread = NULL; 
	}
   
    return TRUE;
}

HANDLE CUart::CreateMsgThread()
{
    HANDLE hdl = (HANDLE)_beginthreadex(NULL,0,ThreadProc,(void*)this,0,&m_dwThreadId);
	WaitForSingleObject(m_hEventFlag,1000);
	return hdl;
}

/*
UINT CUart::Receive(
		void* pData,  //the data pointer
		DWORD dwSize  //the data block size
		)
{
    if( NULL == pData || dwSize == 0)
    {
        return 0;
    }

	//parse packet with '\r\n'
	BYTE *pBuf = new BYTE[dwSize];
	memcpy(pBuf, pData, dwSize);
	LPSTR lpTemp = (LPSTR)pBuf;
	if(*lpTemp == '\n')
	{
		lpTemp ++;
		dwSize --;
	}
	if(dwSize == 0)
	{
		return 0;
	}
	LPSTR lpFind = strstr(lpTemp, "\r");
	int nLen = 0;
	while(lpFind != NULL)
	{
		nLen = lpFind - lpTemp;
		nLen = nLen > dwSize ? dwSize : nLen;
		
		if(m_dwProDataLen > 0)
		{
			strncat(m_szProData, lpTemp, nLen);
		}
		else
		{
			memcpy(m_szProData, lpTemp, nLen);
		}
		strcat(m_szProData, "\r\n");
		m_dwProDataLen = m_dwProDataLen + nLen + 2;

		if(m_dwProDataLen > 2) //2=strlen("\r\n"), skip
		{
			if(m_bCMT)
			{
				strcat(m_szCMT, m_szProData);
				m_bCMT = FALSE;
				ProcessData(m_szCMT, strlen(m_szCMT));
				memset(m_szCMT, 0, MAX_RCV_BUF_1);
			}
			else if(strstr(m_szProData, "+CMT:") != NULL && !m_bCMT) //闪信，需要与下一条组合才完整
			{
				m_bCMT = TRUE;
				memcpy(m_szCMT, m_szProData, m_dwProDataLen);
			}
			else if(m_bCDS)
			{
				strcat(m_szCDS, m_szProData);
				m_bCDS = FALSE;
				ProcessData(m_szCDS, strlen(m_szCDS));
				memset(m_szCDS, 0, MAX_RCV_BUF_1);
			}
			else if(strstr(m_szProData, "+CDS:") != NULL && !m_bCDS) //短消息状态报告，需要与下一条组合才完整
			{
				m_bCDS = TRUE;
				memcpy(m_szCDS, m_szProData, m_dwProDataLen);
			}
			else if(m_bRING)
			{
				if(strstr(m_szProData, "+CLIP:") != NULL)
				{
					strcat(m_szRING, m_szProData);
					ProcessData(m_szRING, strlen(m_szRING));
				}
				else
				{
					ProcessData(m_szRING, strlen(m_szRING));
					ProcessData(m_szProData, m_dwProDataLen);
				}
				m_bRING = FALSE;
				memset(m_szRING, 0, 1024);
			}
			else if(strstr(m_szProData, "RING") != NULL && !m_bRING) //来电提示，需要与下一条组合才能得到来电号码
			{
				m_bRING = TRUE;
				memcpy(m_szRING, m_szProData, m_dwProDataLen);
			}
			else
			{
				ProcessData(m_szProData, m_dwProDataLen);
			}
		}
		
		memset(m_szProData, 0, MAX_RCV_BUF_1);
		m_dwProDataLen = 0;
		lpTemp = lpTemp + nLen;
		dwSize = dwSize - nLen;
		
		if(dwSize >= 2) // skip \r\n
		{
			lpTemp += 2;
			dwSize -= 2;
		}
		else if(dwSize == 1) //skip \r
		{
			lpTemp ++;
			dwSize --;
		}
		
		if(dwSize <= 0)
		{
			break;
		}
		lpFind = strstr(lpTemp, "\r");
	}
	
	if(dwSize > 0)
	{
		memcpy(m_szProData, lpTemp, dwSize);
		m_dwProDataLen += dwSize;
		if(m_dwProDataLen == 2 && m_szProData[0] == '>' && m_szProData[1] == ' ') //for write sms
		{
			ProcessData(m_szProData, m_dwProDataLen);
			memset(m_szProData, 0, MAX_RCV_BUF_1);
			m_dwProDataLen = 0;
		}
	}

	if(pBuf != NULL)
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	FreeMem( (LPBYTE)pData);   
    return 0;
}
*/
UINT CUart::Receive(
		void* pData,  //the data pointer
		DWORD dwSize  //the data block size
		)
{
    if( NULL == pData || dwSize == 0)
    {
        return 0;
    }

	LPSTR lpTemp = (LPSTR)pData;
	if(*lpTemp == '\r' && *(lpTemp + 1) == '\n' && m_szProData[0] != '\0')
	{
		if(m_bLoadSMS && strstr(m_szProData, "OK\r\n") != NULL)
		{
			m_bLoadSMS = FALSE;
		}

		if(m_bCMT)
		{
			strcat(m_szCMT, "\r\n");
			strcat(m_szCMT, m_szProData);
			strcat(m_szCMT, "\r\n");
			m_bCMT = FALSE;
			ProcessData(m_szCMT, strlen(m_szCMT));
			memset(m_szCMT, 0, MAX_RCV_BUF_1);
		}
		else if(strstr(m_szProData, "+CMT:") != NULL && !m_bCMT)
		{
			m_bCMT = TRUE;
			memcpy(m_szCMT, m_szProData, m_dwProDataLen);
		}
		else if(m_bCDS)
		{
			if(m_szProData[0] != '\0')
			{
				strcat(m_szCDS, "\r\n");


				strcat(m_szCDS, m_szProData);
				strcat(m_szCDS, "\r\n");
				m_bCDS = FALSE;
				ProcessData(m_szCDS, strlen(m_szCDS));
				memset(m_szCDS, 0, MAX_RCV_BUF_1);
			}
		}
		else if(strstr(m_szProData, "+CDS:") != NULL && !m_bCDS)
		{
			m_bCDS = TRUE;
			memcpy(m_szCDS, m_szProData, m_dwProDataLen);
		}
		else if(strstr(m_szProData, "+CMGL:") != NULL && !m_bLoadSMS)
		{
			m_bLoadSMS = TRUE;
		}
		else if(m_bRING)
		{
			strcat(m_szRING, "\r\n");
			if(strstr(m_szProData, "+CLIP:") != NULL)
			{
				strcat(m_szRING, m_szProData);
				strcat(m_szRING, "\r\n");
				ProcessData(m_szRING, strlen(m_szRING));
			}
			else
			{
				ProcessData(m_szRING, strlen(m_szRING));
				m_szProData[m_dwProDataLen] = '\r';
				m_szProData[m_dwProDataLen + 1] = '\n';
				m_dwProDataLen += 2;
				ProcessData(m_szProData, m_dwProDataLen);
			}
			m_bRING = FALSE;
			memset(m_szRING, 0, 1024);
		}
		else if(strstr(m_szProData, "RING") != NULL && !m_bRING && strstr(m_szProData, "WINSTRING") == NULL)
		{
			m_bRING = TRUE;
			memcpy(m_szRING, m_szProData, m_dwProDataLen);
			if(m_bLoadSMS)
			{
				m_szProData[m_dwProDataLen] = *lpTemp;
				m_szProData[m_dwProDataLen + 1] = *(lpTemp + 1);
				m_dwProDataLen += 2;
				ProcessData(m_szProData, m_dwProDataLen);
			}
		}
		else
		{
			m_szProData[m_dwProDataLen] = *lpTemp;
			m_szProData[m_dwProDataLen + 1] = *(lpTemp + 1);
			m_dwProDataLen += 2;
			ProcessData(m_szProData, m_dwProDataLen);
		}

		memset(m_szProData, 0, MAX_RCV_BUF_1);
		m_dwProDataLen = 0;
		lpTemp += 2;
		dwSize -=2;
	}

	BOOL bEmptyBuf = TRUE;

	while(dwSize > 0)
	{
		bEmptyBuf = TRUE;

		while(*lpTemp != '\r' && *lpTemp != '\n' && dwSize > 0)
		{
			bEmptyBuf = FALSE;
			if(m_dwProDataLen >= MAX_RCV_BUF_1)
			{
				memset(m_szProData, 0, MAX_RCV_BUF_1);
				m_dwProDataLen = 0;
				break;
			}
			m_szProData[m_dwProDataLen] = *lpTemp;
			m_dwProDataLen ++;
			lpTemp ++;
			dwSize --;
		}
		
		if(dwSize <= 0 && m_dwProDataLen != 2 && m_szProData[0] != '>' && m_szProData[1] != ' ') //for write sms
		{
			break;
		}

		if(!bEmptyBuf)
		{
			if(m_bLoadSMS && strstr(m_szProData, "OK\r\n") != NULL)
			{
				m_bLoadSMS = FALSE;
			}

			if(m_bCMT)
			{
				strcat(m_szCMT, "\r\n");
				strcat(m_szCMT, m_szProData);
				strcat(m_szCMT, "\r\n");
				m_bCMT = FALSE;
				ProcessData(m_szCMT, strlen(m_szCMT));
				memset(m_szCMT, 0, 1024);
			}
			else if(strstr(m_szProData, "+CMT:") != NULL && !m_bCMT)
			{
				m_bCMT = TRUE;
				memcpy(m_szCMT, m_szProData, m_dwProDataLen);
			}
			else if(m_bCDS)
			{
				if(m_szProData[0] != '\0')
				{
					strcat(m_szCDS, "\r\n");
					strcat(m_szCDS, m_szProData);
					strcat(m_szCDS, "\r\n");
					m_bCDS = FALSE;
					ProcessData(m_szCDS, strlen(m_szCDS));
					memset(m_szCDS, 0, 1024);
				}
			}
			else if(strstr(m_szProData, "+CDS:") != NULL && !m_bCDS)
			{
				m_bCDS = TRUE;
				memcpy(m_szCDS, m_szProData, m_dwProDataLen);
			}
			else if(strstr(m_szProData, "+CMGL:") != NULL && !m_bLoadSMS)
			{
				m_bLoadSMS = TRUE;
			}
			else if(m_bRING)
			{
				strcat(m_szRING, "\r\n");
				if(strstr(m_szProData, "+CLIP:") != NULL)
				{
					strcat(m_szRING, m_szProData);
					strcat(m_szRING, "\r\n");
					ProcessData(m_szRING, strlen(m_szRING));
				}
				else
				{
					ProcessData(m_szRING, strlen(m_szRING));
					m_szProData[m_dwProDataLen] = '\r';
					m_szProData[m_dwProDataLen + 1] = '\n';
					m_dwProDataLen += 2;
					ProcessData(m_szProData, m_dwProDataLen);
				}
				m_bRING = FALSE;
				memset(m_szRING, 0, 1024);
			}
			else if(strstr(m_szProData, "RING") != NULL && !m_bRING && strstr(m_szProData, "WINSTRING") == NULL)
			{
				m_bRING = TRUE;
				memcpy(m_szRING, m_szProData, m_dwProDataLen);
				if(m_bLoadSMS)
				{
					m_szProData[m_dwProDataLen] = '\r';
					m_szProData[m_dwProDataLen + 1] = '\n';
					m_dwProDataLen += 2;
					ProcessData(m_szProData, m_dwProDataLen);
				}
			}
			else
			{
				m_szProData[m_dwProDataLen] = '\r';
				m_szProData[m_dwProDataLen + 1] = '\n';
				m_dwProDataLen += 2;
				ProcessData(m_szProData, m_dwProDataLen);
			}
			memset(m_szProData, 0, MAX_RCV_BUF_1);
			m_dwProDataLen = 0;
		}
		
		if(dwSize <= 0)
		{
			break;
		}
		lpTemp ++;
		dwSize -- ;
	}

	FreeMem( (LPBYTE)pData);   
    return 0;
}

void CUart::ProcessData(void* pData, DWORD dwSize)
{
	if(pData == NULL || dwSize == 0)
	{
		return;
	}
	LPSTR lpString = (LPSTR)pData;

	if(m_bPostAll)  //for ATCmd Page
	{
		memset(m_szPostData, 0, MAX_RCV_BUF_1);
		memcpy(m_szPostData, lpString, dwSize);
		for(int i =0; i < MAX_RETRY_NUM; i++)
		{
			if(PostMessage(m_hGuiWnd, m_dwGuiMsgId, WPARAM(m_szPostData), dwSize))
			{
				break;
			}
		}
	}
	else if( !m_bSyn && (IsAsynRespond(lpString) || IsIMEI(lpString))) // for Async.
	{
		memset(m_szPostData, 0, MAX_RCV_BUF_1);
		memcpy(m_szPostData, lpString, dwSize);
		for(int i =0; i < MAX_RETRY_NUM; i++)
		{
			if(PostMessage(m_hGuiWnd, m_dwGuiMsgId, WPARAM(m_szPostData), dwSize))
			{
				break;
			}
		}
	}
	//else //for Sync.
	{
		if(strlen(m_szRcvBuf) < MAX_BUF_LENGTH)
		{
			if(m_szRcvBuf[0] == '\0')
			{
				memcpy(m_szRcvBuf, lpString, dwSize);
			}
			else
			{
				char szTmp[MAX_RCV_BUF_1] = {0};
				strncpy(szTmp, lpString, dwSize);
				strcat(m_szRcvBuf, szTmp);
			}
			m_dwRcvLen += dwSize;
		}
		SetEvent( m_hEventWaitResult );
	}
}

BOOL CUart::IsAsynRespond(LPCSTR lpRespond)
{
	if(lpRespond == NULL)
	{
		return FALSE;
	}
	int nNum = sizeof(AsynCmd) / sizeof(AsynCmd[0]);

	for(int i = 0; i < nNum; i++)
	{
		if(strstr(lpRespond, AsynCmd[i]) != NULL && strstr(lpRespond, "WINSTRING") == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUart::IsIMEI(LPCSTR lpString)
{
	char szText[MAX_RCV_BUF_1] = {0};

	LPCSTR lpFind = strstr(lpString, "\r\n");
	if(lpFind != NULL)
	{
		strncpy(szText, lpString, lpFind - lpString );
	}
	else
	{
		strcpy(szText, lpString);
	}
	
	int nCount = strlen(szText);
	if(nCount != 15)  //IMEI length
	{
		return FALSE;
	}
	else
	{
		for(int i = 0; i < nCount; i++)
		{
			if(0 == isdigit(szText[i]))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
}

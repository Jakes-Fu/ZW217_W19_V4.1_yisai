////////////////////////////////////////////////////////////////////////
// PipeEx.cpp: implementation of the CPipeEx class.
// ���ܣ��ܵ���д����
// ���ߣ�̷����
// ���ڣ�2018.07.26
// �汾��Version 0.0.1 
// ��Ȩ��CopyRight (c) RDA .����.�й�
// ��ʷ��
//       2018-07-26 create by LaoTan
// �÷���
//     #include "PipeEx.h"
//
//     CPipeEx m_sPipe;
//
//     if(m_sPipe.Create())
//     {
//         m_sPipe.xxxxxx();
//     }
//
//     m_sPipe.Close();
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <tlhelp32.h>
#include "PipeEx.h"
#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipeEx::CPipeEx()
{
	m_hStdInWrite = INVALID_HANDLE_VALUE;
	m_hStdOutRead = INVALID_HANDLE_VALUE;

	m_hStdInRead   = INVALID_HANDLE_VALUE;
	m_hStdOutWrite = INVALID_HANDLE_VALUE;

	m_hJob = NULL;
}

CPipeEx::~CPipeEx()
{
    Close();
}

/** 
* @brief             <p> �����ܵ� </p>
* @note              <p> ֻʹ�ñ�׼�����豸��д����׼����豸�Ķ� </p>
* @return value BOOL <p> �ɹ�,����TRUE;ʧ�ܣ�����FALSE </p>

*/
BOOL CPipeEx::Create()
{
	BOOL bResult = FALSE;

	TCHAR szCmdLine[MAX_PATH];
	CString strStmp;

	SECURITY_ATTRIBUTES sa;	
	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	
	m_hJob = CreateJobObject(NULL,(LPCTSTR)_T("Myjob"));

	if (!CreatePipe(&m_hStdInRead,&m_hStdInWrite,&sa,0)) 
	{
		m_strErr = _T("Create StdIn Pipe Error");
		Close();

		return bResult;
	} 	
	
	if (!CreatePipe(&m_hStdOutRead,&m_hStdOutWrite,&sa,0)) 
	{
		m_strErr = _T("Create StdOut Pipe Error");
		Close();

		return bResult;
	} 	

// 	COMMTIMEOUTS ctos;
// 	ctos.ReadIntervalTimeout = MAXWORD;
// 	ctos.ReadTotalTimeoutConstant = 0;
// 	ctos.ReadTotalTimeoutMultiplier = 0;
//     SetCommTimeouts(m_hStdOutRead,&ctos);     //< ����ʱ����д���豸����,Read()
// 	
// 	ctos.WriteTotalTimeoutMultiplier = 100;
// 	ctos.WriteTotalTimeoutConstant = 1000;
// 	SetCommTimeouts(m_hStdInWrite,&ctos);     //< ����ʱ����д���豸����,Write()
// 

	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	
	/// ʹcmd�͹ܵ�����
	si.hStdError   = m_hStdOutWrite;
	si.hStdInput   = m_hStdInRead;
	si.hStdOutput  = m_hStdOutWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	
	/// �õ�ϵͳĿ¼
	GetSystemDirectory(szCmdLine,sizeof(szCmdLine));		
	_tcscat(szCmdLine, _T("\\cmd.exe") );
    
	/// ��������ִ������
	if (!CreateProcess(NULL,szCmdLine,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&m_ProcessInfo)) //< ���������ȹ���
	{
		m_strErr.Format(_T("CreateProcess Error,Err No: %d"),GetLastError() );
		Close();

		return bResult;
	}	
	
	bResult = TRUE;

	if(m_hJob != NULL)
	{
		AssignProcessToJobObject(m_hJob,m_ProcessInfo.hProcess); //< ��ӵ���������
	}
	
	ResumeThread(m_ProcessInfo.hThread);                         //< ���н���
	
	/// �رղ�ʹ�õľ��
	CloseHandle(m_hStdInRead);
	CloseHandle(m_hStdOutWrite);
	
	m_bIsOpen = bResult;

	return bResult;	
}


/** 
* @brief               <p> ������ܵ�дһ������ </p>
* @param char* strFmt  <p> ��ʽ���ַ��� </p>
* @param ...           <p> �ɱ���� </p>
* @return value BOOL   <p> д����ɹ�������TURE;ʧ�ܣ�����FALSE </p>
*/
BOOL CPipeEx::Write(TCHAR *strFmt, ...)
{
	TCHAR szCmdLine[MAX_PATH];
	DWORD dwWrite;
	BOOL bRetu = FALSE;

	va_list  args;
	va_start(args, strFmt);
	_vsntprintf(szCmdLine, sizeof(szCmdLine), strFmt, args);
	va_end(args);

	_tcscat(szCmdLine,_T("\r\n"));

	int nBytes = WideCharToMultiByte(CP_ACP,0,szCmdLine,-1,NULL,0,NULL,NULL);
	char *pszCmd = new char[nBytes + 1];
	WideCharToMultiByte(CP_ACP,0,szCmdLine, -1, pszCmd, nBytes,NULL,NULL);

// 	CString strErr;
// 	strErr.Format(_T("CPipeEx::Write: \n %s"), szCmdLine);
// 	::AfxMessageBox(strErr);

	bRetu = WriteFile(m_hStdInWrite, (BYTE *)pszCmd, strlen(pszCmd),&dwWrite,NULL);

	return bRetu;
}



/** 
* @brief                    <p> ������ܵ���ȡ������Ӧ </p>
* @param LPVOID pDstBuff    <p> ������ݵĻ����� </p>
* @param DWORD dwReadSize   <p> ��ȡ�����ݴ�С(ʵ�ʶ��ص����ݴ�С) </p>
* @return value BOOL        <p> ����ִ��״̬ </p>
*/
BOOL CPipeEx::Read(LPBYTE pDstBuff, DWORD& dwReadSize)
{
	OVERLAPPED m_osRead;          //< �����ص���/д
	BOOL bStatus = FALSE;
	DWORD dwLength = 0;
	CString strTmp;
	
	if(!m_hStdOutRead)
	{
		return FALSE;
	}
	
	memset(&m_osRead,0,sizeof(OVERLAPPED));

	/// ������ݴ�Ż�����
	memset(pDstBuff,0x00,dwReadSize);

    bStatus = ReadFile(m_hStdOutRead,pDstBuff,dwReadSize,&dwReadSize,&m_osRead);
	
	return bStatus;
}


/** 
* @brief                     <p> �����Ϣ��LOG�� </p>
* @param CString &strTrace   <p> �����LOG��Ϣ </p>
* @return value void         <p> �޷���ֵ </p>
*/
void CPipeEx::Trace(CString &strTrace)
{
	::AfxGetMainWnd()->SendMessage(WM_RECEIVETEXT_MESSAGE,strTrace.GetLength(),(LPARAM)(LPCTSTR)strTrace);	
}


/** 
* @brief  <p> �رչܵ� </p>
*/
void CPipeEx::Close()
{
	if(m_hJob)
	{
	    TerminateJobObject(m_hJob,0);                            //< ��ֹ������ҵ�ӽ���
	    ::WaitForSingleObject(m_ProcessInfo.hProcess,INFINITE);  //< �ȴ����̽���
		CloseHandle(m_hJob);	
	}

	if(NULL != m_hStdInWrite)
	{
		CloseHandle(m_hStdInWrite);
		m_hStdInWrite = NULL;
	}
	
	if(NULL != m_hStdOutRead)
	{
		CloseHandle(m_hStdOutRead);	
		m_hStdOutRead = NULL;
	}

	m_bIsOpen = FALSE;

	return;
}

/** 
* @brief  <p>  ��ȡ��ǰ�ܵ��Ƿ�� </p>
*/
BOOL CPipeEx::IsOpen()
{
    return m_bIsOpen;
}


/** 
* @brief  <p> �������Ĵ�����Ϣ </p>
*/
CString CPipeEx::GetLastErr()
{
    return m_strErr;
}


/** 
* @brief  <p> ���ÿ���̨���� </p.
*/
void CPipeEx::SetEcho(BOOL bIsOn)
{
	CString strEcho;

	if(IsOpen())
	{
       if(bIsOn)
	   {
		   strEcho = _T("echo ON");
	   }
	   else
	   {
		   strEcho = _T("echo OFF");
	   }

	   Write((LPTSTR)(LPCTSTR)strEcho);
   }

	return;
}



/** 
* @brief                     <p> �ļ�����</p>
* @param CString strSurFile  <p> Դ�ļ��� </p>
* @return value CString      <p> Ŀ���ļ���</p>
*/
CString CPipeEx::CopyToFile(CString strSurFile)
{
	CString strSurPath(_T("")),strDstFile;

	int nPos = strSurFile.ReverseFind(_T('\\'));

	if(nPos != -1)
	{
        strSurPath = strSurFile.Left(nPos + 1);
	}

	strDstFile = strSurPath + _T("PDL1.img");

	CopyFile(strSurFile, strDstFile, FALSE);

	return strDstFile;
}


/** 
* @brief                          <p> ɱ��ָ������ </p>
* @param CString strProcessName   <p> ��������</p>
* @return value BOOL              <p> �ɹ�,����TRUE; ʧ��,����FALSE </p>
*/
BOOL CPipeEx::KillProcess(CString strProcessName)
{
	PROCESSENTRY32 MyPe32;
	TCHAR *ProcessName = (TCHAR *)(LPCTSTR)strProcessName;
	BOOL bResult = FALSE;

	/// ��ȡϵͳ���̿���
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return bResult;
	}

	/// ʹ��PROCESSENTRY32�ṹǰ���������Ĵ�С
	MyPe32.dwSize = sizeof( PROCESSENTRY32 );

	/// �ڿ��վ���л�ȡ��һ������
	if( !Process32First( hProcessSnap, &MyPe32 ) )
	{
		CloseHandle( hProcessSnap );     
		return bResult;
	}

	/// �������ս���,ɱ��ָ������
	do
	{
		if (_tcsicmp(ProcessName, MyPe32.szExeFile) == 0)
		{						
			if(KillProcessByProcessID(MyPe32.th32ProcessID))
			{
				bResult = TRUE;	
			}			
		}

	} while( Process32Next( hProcessSnap, &MyPe32 ) );

	CloseHandle( hProcessSnap );

	return bResult;
}

/**
* @brief                   <p> ͨ������IDɱ������ </p>
* @param DWORD dwProcessID <p> ����ID </p>         
* @return value BOOL       <p> �ɹ�,����TRUE;ʧ�ܣ�����FALSE </p>
*/
BOOL CPipeEx::KillProcessByProcessID(DWORD dwProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);

	if(hProcess)
	{
		::TerminateProcess(hProcess,0);

		return TRUE;
	}

	return FALSE;
}

/** 
*  ɱ��CMD����
*/
void CPipeEx::KillCmd()
{
	while(TRUE)
	{
		if(!KillProcess(_T("cmd.exe")))
		{
			break;
		}
	}

	return;
}


/**
* @brief                       <p> ͨ������IDɱ������ </p>
* @param CString strDeviceDesc <p> �����豸���豸������Ϣ</p>  
* @param int& nPort            <p> ���ض˿ں�</p>
* @return value BOOL           <p> �ɹ�,����TRUE;ʧ�ܣ�����FALSE </p>
*/
BOOL CPipeEx::FindUartDevice(CString strDeviceDesc, int& nPort)
{
	LPBYTE pBuffer = new BYTE[5 * 1024];

	HDEVINFO DevInfo;
	SP_DEVINFO_DATA DevInfoData;
	CString strPortInfo,strDevicePort;
	BOOL bFindATPort = FALSE;
	nPort = -1;

	/// NULL:  all class, to find ALL devices presended  on system
	DevInfo = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (DevInfo == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	try
	{
		DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		for (DWORD i = 0; SetupDiEnumDeviceInfo(DevInfo, i, &DevInfoData); i++)
		{
			DWORD dwPropertyDataType;
			DWORD dwBuffSizes = 5 * 1024;

			/// �õ�ָ���豸������
			if(!SetupDiGetDeviceRegistryProperty(
				DevInfo,				    	                   //< �豸��Ϣ
				&DevInfoData,				                       //< ָ���豸��Ϣ���ݽṹʵ��
				SPDRP_FRIENDLYNAME,				                   //< ��ȡ�豸��ʲô����
				&dwPropertyDataType,                               //< ��ȡ��������������
				(PBYTE)pBuffer,                                    //< ����������ݵĻ�����
				dwBuffSizes,                                       //< �������Ĵ�С
				&dwBuffSizes))                                     //< ����Ļ�������С
			{				
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)               //< ��������С����
				{
					::AfxMessageBox(_T("Insufficlent size of Allocated buffer!"));  //< �ڴ治�� �����·����ڴ�������
					break;
				}
				else   //< ERROR_INVALID_DATA (NO_ERROR/ERROR_NO_MORE_ITEMS)
				{
					continue;;
				}
			}

			/// ��ȡAT�˿���Ϣ
			strPortInfo.Format(_T("%s"),pBuffer);
			if(strPortInfo.Find(strDeviceDesc) != -1 )
			{
				int nLeft  = strPortInfo.ReverseFind( _T('(') );
				int nRight = strPortInfo.ReverseFind( _T(')') );				
				strDevicePort = strPortInfo.Mid(nLeft + 1,nRight - nLeft - 1);   //< AT�ں� e.g:COM5
				strDevicePort.TrimLeft();
				strDevicePort = strDevicePort.Mid(3);
				strDevicePort.TrimRight();
				nPort = _ttoi(strDevicePort);
				bFindATPort = TRUE;
				break;
			}			
		}

		SetupDiDestroyDeviceInfoList(DevInfo);                                 //< �ͷ��豸��Ϣռ�õ��ڴ�
	}

	catch (...)
	{
		bFindATPort = FALSE;
	}

	/// �ͷŷ�����ڴ�
	delete []pBuffer;
	pBuffer = NULL;

	if (bFindATPort)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
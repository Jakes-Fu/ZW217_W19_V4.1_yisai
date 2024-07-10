////////////////////////////////////////////////////////////////////////
// PipeEx.cpp: implementation of the CPipeEx class.
// 功能：管道读写管理
// 作者：谭红云
// 日期：2018.07.26
// 版本：Version 0.0.1 
// 版权：CopyRight (c) RDA .重庆.中国
// 历史：
//       2018-07-26 create by LaoTan
// 用法：
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
* @brief             <p> 建立管道 </p>
* @note              <p> 只使用标准输入设备的写、标准输出设备的读 </p>
* @return value BOOL <p> 成功,返回TRUE;失败，返回FALSE </p>

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
//     SetCommTimeouts(m_hStdOutRead,&ctos);     //< 将超时参数写入设备控制,Read()
// 	
// 	ctos.WriteTotalTimeoutMultiplier = 100;
// 	ctos.WriteTotalTimeoutConstant = 1000;
// 	SetCommTimeouts(m_hStdInWrite,&ctos);     //< 将超时参数写入设备控制,Write()
// 

	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	
	/// 使cmd和管道关联
	si.hStdError   = m_hStdOutWrite;
	si.hStdInput   = m_hStdInRead;
	si.hStdOutput  = m_hStdOutWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	
	/// 得到系统目录
	GetSystemDirectory(szCmdLine,sizeof(szCmdLine));		
	_tcscat(szCmdLine, _T("\\cmd.exe") );
    
	/// 创建进程执行命令
	if (!CreateProcess(NULL,szCmdLine,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&m_ProcessInfo)) //< 创建进程先挂起
	{
		m_strErr.Format(_T("CreateProcess Error,Err No: %d"),GetLastError() );
		Close();

		return bResult;
	}	
	
	bResult = TRUE;

	if(m_hJob != NULL)
	{
		AssignProcessToJobObject(m_hJob,m_ProcessInfo.hProcess); //< 添加到工作集中
	}
	
	ResumeThread(m_ProcessInfo.hThread);                         //< 运行进程
	
	/// 关闭不使用的句柄
	CloseHandle(m_hStdInRead);
	CloseHandle(m_hStdOutWrite);
	
	m_bIsOpen = bResult;

	return bResult;	
}


/** 
* @brief               <p> 向输入管道写一条命令 </p>
* @param char* strFmt  <p> 格式化字符串 </p>
* @param ...           <p> 可变参数 </p>
* @return value BOOL   <p> 写命令成功，返回TURE;失败，返回FALSE </p>
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
* @brief                    <p> 从输入管道读取命令响应 </p>
* @param LPVOID pDstBuff    <p> 存放数据的缓冲区 </p>
* @param DWORD dwReadSize   <p> 读取的数据大小(实际读回的数据大小) </p>
* @return value BOOL        <p> 命令执行状态 </p>
*/
BOOL CPipeEx::Read(LPBYTE pDstBuff, DWORD& dwReadSize)
{
	OVERLAPPED m_osRead;          //< 用于重叠读/写
	BOOL bStatus = FALSE;
	DWORD dwLength = 0;
	CString strTmp;
	
	if(!m_hStdOutRead)
	{
		return FALSE;
	}
	
	memset(&m_osRead,0,sizeof(OVERLAPPED));

	/// 清空数据存放缓冲区
	memset(pDstBuff,0x00,dwReadSize);

    bStatus = ReadFile(m_hStdOutRead,pDstBuff,dwReadSize,&dwReadSize,&m_osRead);
	
	return bStatus;
}


/** 
* @brief                     <p> 输出信息到LOG框 </p>
* @param CString &strTrace   <p> 输出的LOG信息 </p>
* @return value void         <p> 无返回值 </p>
*/
void CPipeEx::Trace(CString &strTrace)
{
	::AfxGetMainWnd()->SendMessage(WM_RECEIVETEXT_MESSAGE,strTrace.GetLength(),(LPARAM)(LPCTSTR)strTrace);	
}


/** 
* @brief  <p> 关闭管道 </p>
*/
void CPipeEx::Close()
{
	if(m_hJob)
	{
	    TerminateJobObject(m_hJob,0);                            //< 终止所有作业子进程
	    ::WaitForSingleObject(m_ProcessInfo.hProcess,INFINITE);  //< 等待进程结束
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
* @brief  <p>  获取当前管道是否打开 </p>
*/
BOOL CPipeEx::IsOpen()
{
    return m_bIsOpen;
}


/** 
* @brief  <p> 返回最后的错误信息 </p>
*/
CString CPipeEx::GetLastErr()
{
    return m_strErr;
}


/** 
* @brief  <p> 设置控制台回显 </p.
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
* @brief                     <p> 文件改名</p>
* @param CString strSurFile  <p> 源文件名 </p>
* @return value CString      <p> 目标文件名</p>
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
* @brief                          <p> 杀掉指定进程 </p>
* @param CString strProcessName   <p> 进程名称</p>
* @return value BOOL              <p> 成功,返回TRUE; 失败,返回FALSE </p>
*/
BOOL CPipeEx::KillProcess(CString strProcessName)
{
	PROCESSENTRY32 MyPe32;
	TCHAR *ProcessName = (TCHAR *)(LPCTSTR)strProcessName;
	BOOL bResult = FALSE;

	/// 获取系统进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return bResult;
	}

	/// 使用PROCESSENTRY32结构前先设置它的大小
	MyPe32.dwSize = sizeof( PROCESSENTRY32 );

	/// 在快照句柄中获取第一个进程
	if( !Process32First( hProcessSnap, &MyPe32 ) )
	{
		CloseHandle( hProcessSnap );     
		return bResult;
	}

	/// 遍历快照进程,杀掉指定进程
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
* @brief                   <p> 通过进程ID杀掉进程 </p>
* @param DWORD dwProcessID <p> 进程ID </p>         
* @return value BOOL       <p> 成功,返回TRUE;失败，返回FALSE </p>
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
*  杀掉CMD进程
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
* @brief                       <p> 通过进程ID杀掉进程 </p>
* @param CString strDeviceDesc <p> 串口设备的设备描述信息</p>  
* @param int& nPort            <p> 返回端口号</p>
* @return value BOOL           <p> 成功,返回TRUE;失败，返回FALSE </p>
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

			/// 得到指定设备的属性
			if(!SetupDiGetDeviceRegistryProperty(
				DevInfo,				    	                   //< 设备信息
				&DevInfoData,				                       //< 指向设备信息数据结构实例
				SPDRP_FRIENDLYNAME,				                   //< 获取设备的什么属性
				&dwPropertyDataType,                               //< 获取的属性数据类型
				(PBYTE)pBuffer,                                    //< 存放属性数据的缓冲区
				dwBuffSizes,                                       //< 缓冲区的大小
				&dwBuffSizes))                                     //< 请求的缓冲区大小
			{				
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)               //< 缓冲区大小不足
				{
					::AfxMessageBox(_T("Insufficlent size of Allocated buffer!"));  //< 内存不够 ，重新分配内存再搜索
					break;
				}
				else   //< ERROR_INVALID_DATA (NO_ERROR/ERROR_NO_MORE_ITEMS)
				{
					continue;;
				}
			}

			/// 获取AT端口信息
			strPortInfo.Format(_T("%s"),pBuffer);
			if(strPortInfo.Find(strDeviceDesc) != -1 )
			{
				int nLeft  = strPortInfo.ReverseFind( _T('(') );
				int nRight = strPortInfo.ReverseFind( _T(')') );				
				strDevicePort = strPortInfo.Mid(nLeft + 1,nRight - nLeft - 1);   //< AT口号 e.g:COM5
				strDevicePort.TrimLeft();
				strDevicePort = strDevicePort.Mid(3);
				strDevicePort.TrimRight();
				nPort = _ttoi(strDevicePort);
				bFindATPort = TRUE;
				break;
			}			
		}

		SetupDiDestroyDeviceInfoList(DevInfo);                                 //< 释放设备信息占用的内存
	}

	catch (...)
	{
		bFindATPort = FALSE;
	}

	/// 释放分配的内存
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
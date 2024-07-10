////////////////////////////////////////////////////////////////////////
// PipeEx.h: interface for the CPipeEx class.
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

#if !defined(AFX_PIPEEX_H__24EED06B_69DE_41DA_AA10_2E959B9DC669__INCLUDED_)
#define AFX_PIPEEX_H__24EED06B_69DE_41DA_AA10_2E959B9DC669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4819)  
#pragma comment(lib,"setupapi.lib")
#pragma comment(lib,"hid.lib")

// Declare the C libraries used  
extern "C" {  
#include <setupapi.h>  // Must link in setupapi.lib  
#include "hidsdi.h"    // Must link in hid.lib  
}  

#define MAX_PIPE_BUFFER_SIZE         8192            //< 最大管道缓冲区大小
#define WM_RECEIVETEXT_MESSAGE       WM_USER + 1999

class CPipeEx  
{
public:
	void SetEcho(BOOL bIsOn);
	CString GetLastErr();

	BOOL IsOpen();
	void Close();
	void Trace(CString &strTrace);
	BOOL Read(LPBYTE pDstBuff,DWORD& dwReadSize);
	BOOL Write(TCHAR *strFmt,...);
	BOOL Create();

	void KillCmd();

	CString CopyToFile(CString strSurFile);
	BOOL FindUartDevice(CString strDeviceDesc, int& nPort);
	///////////////////////////////////////////////////////
	CPipeEx();
	virtual ~CPipeEx();

private:
	CString m_strErr;
	HANDLE m_hJob;                                 //< 作业对象句柄

	HANDLE m_hStdInRead;                           //< 标准输入读写句柄
	HANDLE m_hStdInWrite;

	HANDLE m_hStdOutRead;                          //< 标准输出读写句柄
	HANDLE m_hStdOutWrite;

	PROCESS_INFORMATION m_ProcessInfo;             //< 进程信息
	LPSECURITY_ATTRIBUTES m_lpPipeAttributes;      //< 管道属性
	BOOL m_bIsOpen;                                //< 管道是否已打开

private:
	BOOL KillProcess(CString strProcessName);
	BOOL KillProcessByProcessID(DWORD dwProcessID);
};

#endif // !defined(AFX_PIPEEX_H__24EED06B_69DE_41DA_AA10_2E959B9DC669__INCLUDED_)

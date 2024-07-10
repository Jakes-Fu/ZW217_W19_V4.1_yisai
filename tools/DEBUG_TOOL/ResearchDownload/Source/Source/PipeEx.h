////////////////////////////////////////////////////////////////////////
// PipeEx.h: interface for the CPipeEx class.
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

#define MAX_PIPE_BUFFER_SIZE         8192            //< ���ܵ���������С
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
	HANDLE m_hJob;                                 //< ��ҵ������

	HANDLE m_hStdInRead;                           //< ��׼�����д���
	HANDLE m_hStdInWrite;

	HANDLE m_hStdOutRead;                          //< ��׼�����д���
	HANDLE m_hStdOutWrite;

	PROCESS_INFORMATION m_ProcessInfo;             //< ������Ϣ
	LPSECURITY_ATTRIBUTES m_lpPipeAttributes;      //< �ܵ�����
	BOOL m_bIsOpen;                                //< �ܵ��Ƿ��Ѵ�

private:
	BOOL KillProcess(CString strProcessName);
	BOOL KillProcessByProcessID(DWORD dwProcessID);
};

#endif // !defined(AFX_PIPEEX_H__24EED06B_69DE_41DA_AA10_2E959B9DC669__INCLUDED_)

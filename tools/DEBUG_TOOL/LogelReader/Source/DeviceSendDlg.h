#if !defined(AFX_DEVICESENDDLG_H__217A2ECE_280C_4ACE_9B7D_AC259EB5592E__INCLUDED_)
#define AFX_DEVICESENDDLG_H__217A2ECE_280C_4ACE_9B7D_AC259EB5592E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceSendDlg.h : header file
//
#include "LoadDll.h"	// Added by ClassView
#include "segmentstruct.h"

#define TIMER_EVENT       100
#define DELAY_DEFAULT     10000
#define SEND_PORT_DEFAULT 5000
#define SIZE_DEFAULT      10
#define DEVICE_DEFAULT	   "G"
#define WM_RECV_DATA    WM_APP + 300

#define WM_LOGEL_RENEWLOG   WM_USER + 5001  // Notify Logel to renew a log

#define MSG_RENEW_LOG 209

#define SEGMENTINFO_SIZE sizeof(SEGMENTINFO_T)
#define DISKINFO_SIZE sizeof(DISKINFO_T)
#define SECT_SIZE 512

#define BUF_SIZE 0x10000

#define READ_FILE(buffer, size, count, stream) fread(buffer, size, count, stream)
//#define READ_FILE(hFile, lpBuffer, nNumberOfBytesToRead, pNumberOfBytesRead, lpOverlapped) ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)

#define SEEK_FILE(stream, offset, origin) fseek(stream, offset, origin)
//#define SEEK_FILE(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod) SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod) 

#define CLOSE_FILE(fp) fclose(fp)
//#define CLOSE_FILE(hResult) CloseHandle(hResult)
/////////////////////////////////////////////////////////////////////////////
// CDeviceSendDlg dialog

class CDeviceSendDlg : public CDialog
{
// Construction
public:
	CDeviceSendDlg(CWnd* pParent = NULL);   // standard constructor

public:
    CLoadDll m_dllSocObj;
public:
	UINT  m_uiTimeEvent;
	BOOL  m_bFlag;  //TRUE as Send ,FLASE as Pause.
	BYTE* m_pBuffer;
	char    m_szFileDir[MAX_PATH];

public:
	//test
	FILE *m_pFile;
	//
	HANDLE m_hResult;
	DWORD m_dwTotalData;
	DWORD m_dwSendLen;
	int m_nLogCount;
	int m_nSendCount;
	DWORD m_dwStartSecPos;
	DWORD* m_dwSize;
	FILE* m_fpData;
	CString m_strPathName;
	BOOL m_bSaveTmpFile;
	TCHAR m_tmpFile[MAX_PATH];

public:
	void ReleaseResource();

private:
	void SetListStr(LPCTSTR lpStr);
	BOOL LoadData();
	void GetLogData(int nSecSize, DWORD nSecStartPos, int nSecNum);
	BOOL OpenDiskAndRead();
	void GetFileName();
	//test
	BOOL MapToBuf();

// Dialog Data
	//{{AFX_DATA(CDeviceSendDlg)
	enum { IDD = IDD_DEVICE_DLG };
	CListBox	m_mylist;
	UINT	m_uiEdelay;
	UINT	m_uiEport;
	UINT	m_uiEsize;
	CString	m_strDevice;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceSendDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceSendDlg)
	afx_msg void OnSetupBtn();
	afx_msg void OnSendBtn();
	afx_msg void OnStopBtn();
	afx_msg void OnClearBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLoadBtn();
	afx_msg void OnOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICESENDDLG_H__217A2ECE_280C_4ACE_9B7D_AC259EB5592E__INCLUDED_)

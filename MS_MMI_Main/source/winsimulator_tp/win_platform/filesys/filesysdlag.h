#ifndef __FILESYS_H__
#define __FILESYS_H__

#include "StdAfx.h"
#include "FeatureDllItf.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CTRLPAN_TASK_ID 50-1	

#include "resource.h"


class CFileSysDlg : public CPropertyPage 
{
	DECLARE_DYNCREATE(CFileSysDlg)
// Construction
public:
	AFX_EXT_CLASS CFileSysDlg();
	~CFileSysDlg();


	CImageList m_ImageListFile;
	CImageList m_ImageListDisk;
// Dialog Data
	//{{AFX_DATA(CFileSysDlg)
	enum { IDD = IDD_FILESYSTEM_PAGE };
	CComboBox	m_cmbDev;
	CTreeCtrl	m_TreeDir;
	CListCtrl	m_ListDisk;
	CListCtrl	m_ListFile;
	CString	m_strDeviceType;
	UINT	m_nImgSize;
	//}}AFX_DATA
	CString m_oldname;
	BOOL	m_bStartRename;
	BOOL    m_bStartNewDir;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	private:
		void	  InitDiskList();
		void	  BuildColumns(int nCols, int * nWidth, CString * csCol);
		void      OnContextMenu();
		
		CString   GetNameFromPath(CString path);
		CString   GetCurPath();
		void      BrowseDir(CString strDirName);
		
		void      EnumAllDevice();

		void ReadFileToPC( const TCHAR* szSrcPath, TCHAR* szPCPath );
		void ReadDirFilesToPC( const TCHAR* szSrcPath, TCHAR* szPCPath );
		void WriteFileFromPC( const TCHAR* szSrcPath, TCHAR* szPCPath );
		void WriteDirFromPC( const TCHAR* szSrcPath, TCHAR* szPCPath );
		void CreateDir( TCHAR* szDir );
		void GetFileListAndDeleteFile(); //从配置文件读取文件信息，并删除相关文件
		void SetButtonText(); //设置按钮文字
		void SendRecFindMsg(CString strDirName); //发送递归查找的消息
		void SendRecDelDirMsg(CString strDirName); //发送删除目录的消息
		BOOL CopyStringToClipboard(CString source); //复制字符串到剪贴板
	public:
		void DealCPMsg(void* pmsg);
		
		

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileSysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListDisk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeDir(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListFile(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnEndlabeleditListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonUnregister();
	afx_msg void OnMenuitemDelfile();
	afx_msg void OnMenuitemFiletopc();
	afx_msg void OnMenuitemFilefrompc();
	afx_msg void OnMenuitemDirfrompc();
	afx_msg void OnMenuitemOpentxt();
	afx_msg void OnMenuitemNewdir();
	afx_msg void OnMenuitemDeldir();
	afx_msg void OnButtonCreateimg();
	afx_msg void OnClose();
	afx_msg void OnMenuitemAttr();
	afx_msg void OnBeginlabeleditListFile(NMHDR* pNMHDR, LRESULT* pResult);
	//apple add
	afx_msg void OnSelchangedDevice();
	afx_msg void OnButtonLoadimg();
	afx_msg void OnDblClickFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif //FILESYS


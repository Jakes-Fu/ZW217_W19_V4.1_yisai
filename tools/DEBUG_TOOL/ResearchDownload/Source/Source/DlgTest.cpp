// DlgTest.cpp : 实现文件
//

#include "stdafx.h"
#include "DLoader.h"
#include "DlgTest.h"
#include "HostProtocol.h"

// CDlgTest 对话框

IMPLEMENT_DYNAMIC(CDlgTest, CDialog)

CDlgTest::CDlgTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest::IDD, pParent)
{

}

CDlgTest::~CDlgTest()
{
}

void CDlgTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE, m_ctlFile);
}


BEGIN_MESSAGE_MAP(CDlgTest, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgTest::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CDlgTest 消息处理程序

void CDlgTest::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile;

	m_ctlFile.GetWindowText(strFile);

	if(strFile.IsEmpty())
	{
		::AfxMessageBox(_T("select file，first"));
		return;
	}

	CHostProtocol sHostProtocol;
	sHostProtocol.SetFDL1Path(strFile);
	CFile sFile;
	if(!sFile.Open(_T("Test.Log"), CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
	{
		::AfxMessageBox(_T("create Test.Log Fail"));
		return;
	}

	int nPacketNums = sHostProtocol.CountPackets();

	BYTE *pDstBuff = new BYTE[3072];
	int nBuffLen = 3072;
    int nPackSize = 0;

	nPackSize = sHostProtocol.Assemble_Patket_Connect(pDstBuff, nBuffLen);
	sFile.Write(pDstBuff, nPackSize);

	nPackSize = sHostProtocol.Assemble_Patket_Start(pDstBuff, nBuffLen);
	sFile.Write(pDstBuff, nPackSize);


	for(int i = 0; i < nPacketNums; i++)
	{
		nPackSize = sHostProtocol.Assemble_Packet_Data(i,pDstBuff, nBuffLen);
		sFile.Write(pDstBuff, nPackSize); 
	}

	nPackSize = sHostProtocol.Assemble_Patket_End(pDstBuff, nBuffLen);
	sFile.Write(pDstBuff, nPackSize);

	delete []pDstBuff;
	pDstBuff = NULL;

	sFile.Close();

	::AfxMessageBox(_T("End"));

	return;
}

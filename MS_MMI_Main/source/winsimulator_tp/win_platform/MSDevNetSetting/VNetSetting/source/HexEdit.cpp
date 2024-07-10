// HexEdit.cpp : implementation file
//

//******************************************************************//
//	�ļ����ͣ�	Implementation File
//	���ܣ�		ʮ��������ֵ����༭����
//	���ߣ�		Wang Yifeng
//	������ڣ�	2005_08_08
//	��ע��		�����WM_CHAR����Ϣ��Ӧ��ʵ��ʮ��������ֵ����
//******************************************************************//

#include "stdafx.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

// �ж��Ƿ�Ϊʮ��������ĸ
BOOL CHexEdit::ishexc(UINT nChar)
{
    switch(nChar)
    {
    case 'a':    case 'b':    case 'c':    case 'd':    case 'e':    case 'f':    
    case 'A':    case 'B':    case 'C':    case 'D':    case 'E':    case 'F':
        return TRUE;
        break;
    default:
        break;
    }
    return FALSE;
}

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// ϵͳ���� ?
	if (nChar== 8)
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	// ��ȡ�������λ��
	POINT caret;
	::GetCaretPos(&caret);
    
	// ���� CharFormPos() ���ع��λ�ô����ַ�����������������
	// ���ֽ�Ϊ���������ֽ�Ϊ����
	caret.x= LOWORD(CharFromPos(caret));	
	CString text;
	GetWindowText(text);

	// �����ʮ�������������ͣ�ϵͳĬ�ϴ������಻������
	if ( isdigit(nChar) || ishexc(nChar) )
    {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }		
}

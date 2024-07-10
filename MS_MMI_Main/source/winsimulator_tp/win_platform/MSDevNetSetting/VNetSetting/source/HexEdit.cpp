// HexEdit.cpp : implementation file
//

//******************************************************************//
//	文件类型：	Implementation File
//	功能：		十六进制数值输入编辑框类
//	作者：		Wang Yifeng
//	完成日期：	2005_08_08
//	备注：		添加了WM_CHAR的消息响应，实现十六进制数值输入
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

// 判断是否为十六进制字母
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
	// 系统按键 ?
	if (nChar== 8)
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	// 获取输入光标的位置
	POINT caret;
	::GetCaretPos(&caret);
    
	// 函数 CharFormPos() 返回光标位置处的字符所处的行数和列数
	// 高字节为行数，低字节为列数
	caret.x= LOWORD(CharFromPos(caret));	
	CString text;
	GetWindowText(text);

	// 如果是十六进制数字类型，系统默认处理；其余不做处理
	if ( isdigit(nChar) || ishexc(nChar) )
    {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }		
}

// FmlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "FmlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmlDlg dialog


CFmlDlg::CFmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFmlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFmlDlg)
	m_strFml = _T("");
	m_nStartPos = 0;
	//}}AFX_DATA_INIT

    m_pMathFml  = new CMathFormula();
}


void CFmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmlDlg)
	DDX_Text(pDX, IDC_EDIT_FML, m_strFml);
	DDX_Text(pDX, IDC_EDIT_STARTPOS, m_nStartPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFmlDlg, CDialog)
	//{{AFX_MSG_MAP(CFmlDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmlDlg message handlers

BOOL CFmlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    GetDlgItem(IDC_EDIT_FML)->SetFocus();

	return TRUE;  
}

BOOL CFmlDlg::ParserFml(CString strFml)
{
    if (strFml == "")       
    {
        AfxMessageBox("math formula string can't be empty !");
        return FALSE;
    }
    CString strFormula= strFml;
    if (!CheckToken(strFml)) 
        return FALSE;
    
    CString strToken;    
    int     pos= -1;
    
    //get addr
    pos =GetAddr(strFml,strToken);    
    if(5 != pos)
    {
        AfxMessageBox("Invalid math formula string!\nThe math formula must be: [ADDR]&&[MASK]<</>>[MOVE]*[A1]");
        return FALSE;
    }

	if(!IsOnlyOneOperator(strFml))
	{
		AfxMessageBox("Invalid math formula string!\nFor example: \"[D127]+100\" is right; \"[D127]+100*2>>8-50\" is not support!");
		return FALSE;
	}
    
    char *stopstring;
    WORD wAddr= (WORD)strtol(strToken.GetBuffer(strToken.GetLength()), &stopstring, 16);
    strFml.ReleaseBuffer();
    
    m_Filter_Data.wAddr= wAddr;
    m_Filter_Data.dwStartPos = m_nStartPos;
    
    //get mask
    pos= GetToken(strFormula,MATH_MASK,strToken);   
    if (pos != -1)
    {
		WORD wMask = (WORD)GetCoeff(strFormula, strToken, MATH_MASK);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_mask(wMask);
    }     
    
    //get move right
    pos= GetToken(strFormula,MATH_MOVER,strToken);   
    if (pos != -1)
    {
		WORD wMove = (WORD)GetCoeff(strFormula, strToken, MATH_MOVER);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_move(wMove,TRUE);
    }  
    
    //get move left
    pos= GetToken(strFormula,MATH_MOVEL,strToken);   
    if (pos != -1)
    {
		WORD wMove = (WORD)GetCoeff(strFormula, strToken, MATH_MOVEL);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_move(wMove,FALSE);
    }  
    
    
    //get mult
    pos= GetToken(strFormula,MATH_MUL,strToken);   
    if (pos != -1)
    {
		double fMul = GetCoeff(strFormula, strToken, MATH_MUL);
        strFml.ReleaseBuffer();           
        m_pMathFml->set_mult(fMul);
    }  
	
	//add
	pos= GetToken(strFormula,MATH_ADD,strToken);   
    if (pos != -1)
    {
		WORD wAdd = (WORD)GetCoeff(strFormula, strToken, MATH_ADD);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_add(wAdd);
    } 

	//sub
	pos= GetToken(strFormula,MATH_SUB,strToken);   
    if (pos != -1)
    {
		WORD wSub = (WORD)GetCoeff(strFormula, strToken, MATH_SUB);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_sub(wSub);
    } 

	//div
	pos= GetToken(strFormula,MATH_DIV,strToken);   
    if (pos != -1)
    {
		double fDiv = GetCoeff(strFormula, strToken, MATH_DIV);
        strFml.ReleaseBuffer();    
        m_pMathFml->set_div(fDiv);
    } 
    
    return TRUE;
}

BOOL CFmlDlg::CheckToken(CString strFml)
{
//    int   nlen= strFml.GetLength();
//    char* pstr= strFml.GetBuffer(nlen);
//    int idx= 0;
//    while (*(pstr+idx) != '[' && idx<nlen); 
//    {
//
//    }
//    
////    for(int idx=0; idx < nlen; idx++)
////    {
////             
////    }
//
//    strFml.ReleaseBuffer();
    return TRUE;
}

int CFmlDlg::GetToken(CString strFml, int token_idx, CString& strToken)
{
    int nFind= strFml.FindOneOf(m_pMathFml->GetMathFlagDesc(token_idx));
    if (nFind == -1)
        return -1;

    int pos_head =-1;
    int pos_end  =-1;

    //pos_head= strFml.Find('[' , nFind); 
	pos_head= strFml.Find('[');  
    if (pos_head == -1)
        return -1;
    
    pos_head++;
    //pos_end = strFml.Find(']' , pos_head);
	pos_end = strFml.Find(']');
    strToken= strFml.Mid(pos_head, pos_end - pos_head);

    return pos_end;

}
int CFmlDlg::GetSign(CString strFml,int idx, CString& strSign)
{
	UNREFERENCED_PARAMETER(strSign);
    int   nlen= strFml.GetLength();
    char* pstr= strFml.GetBuffer(nlen);    
    for (int i= idx; i< nlen; i++)
    {
        switch(*(pstr+i))
        {
        case '&': return MATH_MASK;
        case '+': return MATH_ADD; 
        case '-': return MATH_SUB;
        case '*': return MATH_MUL;           
        case '/': return MATH_DIV; 
        case '[': return -1; 
        case ']': return -1; 
        default: ;
        }
    }
    return -1;
}

int CFmlDlg::GetAddr(CString strFml, CString& strAddr)
{
    int pos_head =-1;
    int pos_end  =-1;

    pos_head= strFml.Find('[');            
    if (pos_head == -1)
        return -1;
    
    pos_head++;
    pos_end = strFml.Find(']' , pos_head);
    strAddr = strFml.Mid(pos_head, pos_end - pos_head);

    return pos_end;    

}



void CFmlDlg::OnOK() 
{
    UpdateData();
    if(!ParserFml(m_strFml))
    {
        return;
    }
    
	CDialog::OnOK();
}

void CFmlDlg::OnCancel() 
{
    if (m_pMathFml != NULL)
    {
        delete m_pMathFml;
    }
	
	CDialog::OnCancel();
}

double CFmlDlg::GetCoeff(CString strFml, CString strToken, int nSymbol)
{
	static const char * szFlag[] = { 
        { "&" },
        { ">>" },
        { "<<" },
        { "+" },
        { "-" },
        { "*" },
        { "/" },
    };

	CString strSymbol = szFlag[nSymbol];
	CString strRet = strFml;
	strRet.Replace("[", "");
	strRet.Replace("]", "");
	strRet.Replace(strToken, "");
	strRet.Replace(strSymbol, "");

	return atof(strRet);
}

BOOL CFmlDlg::IsOnlyOneOperator(CString strFml)
{
	int nCount = 0;
	if(strFml.Find("&") != -1)
	{
		nCount ++;
	}
	if(strFml.Find(">>") != -1)
	{
		nCount ++;
	}
	if(strFml.Find("<<") != -1)
	{
		nCount ++;
	}
	if(strFml.Find("+") != -1)
	{
		nCount ++;
	}
	if(strFml.Find("-") != -1)
	{
		nCount ++;
	}
	if(strFml.Find("*") != -1)
	{
		nCount ++;
	}
	if(strFml.Find("/") != -1)
	{
		nCount ++;
	}

	if(nCount > 1)
	{
		return FALSE;
	}

	return TRUE;
}
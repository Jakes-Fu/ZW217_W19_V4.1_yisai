// NewItemInfo.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "NewItemInfo.h"
#include "FileParser.h"      //

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewItemInfo dialog

const _TCHAR * CNewItemInfo::m_lpItemType[] =
{
        _T("CHAR"),
		_T("SHORT"),
		_T("LONG"),
		_T("ARRAY"),
		_T("STRUCT"),
		_T("ASTRING"),
		_T("WSTRING")
};

// 条目类型的数量
const int CNewItemInfo::m_nTypeCount = sizeof m_lpItemType / sizeof m_lpItemType[ 0 ];
// ]]

CNewItemInfo::CNewItemInfo(CWnd* pParent /*=NULL*/)
: CDialog(CNewItemInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewItemInfo)
	m_strName = _T("");
	m_fIsArray = FALSE;
	m_nArraySize = 0;
	m_strDesc = _T("");
	m_strDefaultValue = _T("");
	m_nItemID = 0;
	m_nStrLen = 1;
	//}}AFX_DATA_INIT
    m_fNeedID = TRUE;
	
    ClearItemType();
}

void CNewItemInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewItemInfo)
	DDX_Control(pDX, IDC_EDIT_STR_LEN, m_edtStrLen);
	DDX_Control(pDX, IDC_EDIT_ID, m_ctrlID);
	DDX_Control(pDX, IDC_EDIT_ARRAY_SIZE, m_ctrlArraySize);
	DDX_Control(pDX, IDC_CHECK_ARRAY, m_ctrlArray);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctlCombo);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Check(pDX, IDC_CHECK_ARRAY, m_fIsArray);
	DDX_Text(pDX, IDC_EDIT_ARRAY_SIZE, m_nArraySize);
	DDX_Text(pDX, IDC_EDIT_DES, m_strDesc);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strDefaultValue);
	DDX_Text(pDX, IDC_EDIT_ID, m_nItemID);
	DDX_Text(pDX, IDC_EDIT_STR_LEN, m_nStrLen);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewItemInfo, CDialog)
//{{AFX_MSG_MAP(CNewItemInfo)
ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
ON_BN_CLICKED(IDC_CHECK_ARRAY, OnCheckArray)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewItemInfo message handlers
void CNewItemInfo::OnSelchangeComboType() 
{
    //m_uItemType = m_ctlCombo.GetCurSel();	
	CString strTxt;
	m_ctlCombo.GetLBText(m_ctlCombo.GetCurSel(),strTxt);
	ITEMTYPE eSelItemType = (ITEMTYPE)GetItemType(strTxt.operator LPCTSTR());
    if(ASTRING_ITEM_TYPE == eSelItemType || WSTRING_ITEM_TYPE == eSelItemType )
    {
		m_edtStrLen.EnableWindow (TRUE);
    }
	else
	{
		m_edtStrLen.EnableWindow (FALSE);
	}
}

BOOL CNewItemInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    // [[填充类型列表框 add by wei anli 03.06.12
	ASSERT( m_strArrType.GetSize() > 0 );
	
    m_ctlCombo.ResetContent();
	m_ctrlArray.EnableWindow( FALSE );
	m_ctrlArraySize.EnableWindow( FALSE );
	m_edtStrLen.EnableWindow(FALSE);
	
    for( int i=0; i<m_strArrType.GetSize(); i++ )
    {
		if ( m_strArrType.GetAt( i ) == "ARRAY" )
		{
			m_ctrlArray.EnableWindow( TRUE );
			continue;
		}
        m_ctlCombo.AddString( m_strArrType.GetAt( i ) );
    }
	
    m_ctlCombo.SetCurSel( 0 );
	m_ctrlID.SetReadOnly( m_fNeedID ? FALSE : TRUE );
    
    // ]]
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

UINT CNewItemInfo::GetItemType( LPCTSTR lpszTypeName )
{    
    if ( 0 == _tcscmp( lpszTypeName, _T("CHAR") ) )
    {
        return CHAR_ITEM_TYPE;
    }
    else if ( 0 == _tcscmp( lpszTypeName, _T("SHORT") ) )
    {
        return SHORT_ITEM_TYPE;
    }
    else if ( 0 == _tcscmp( lpszTypeName, _T("LONG") ) )
    {
        return LONG_ITEM_TYPE;
    }
    else if ( 0 == _tcscmp( lpszTypeName, _T("STRUCT") ) )
    {
        return STRUCT_ITEM_TYPE;
    }
	else if ( 0 == _tcscmp( lpszTypeName, _T("ASTRING") ) )
    {
        return ASTRING_ITEM_TYPE;
    }
	else if ( 0 == _tcscmp( lpszTypeName, _T("WSTRING") ) )
    {
        return WSTRING_ITEM_TYPE;
    }
    else
    {
		ASSERT( 0 );
        return NONE_ITEM_TYPE;   
    }
}

bool CNewItemInfo::CheckStringLen() const
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_STR_LEN);
    if (NULL != pWnd)
    {
        CString strLen;
        pWnd->GetWindowText(strLen);
        UINT nValue = _ttoi(strLen);
        
        if (0xFFFF0000 & nValue)    // 限定范围至0 ~ 65535
        {
            AfxMessageBox( _T("String Length should be in the range 0 ~ 65535") );
            
            CEdit* pEdit = DYNAMIC_DOWNCAST(CEdit, pWnd);
            if (NULL != pEdit)
            {
                pEdit->SetFocus();
                pEdit->SetSel(0, -1);
            }
            
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void CNewItemInfo::AddItemType(const CString &strType)
{
    m_strArrType.Add(strType);
}

void CNewItemInfo::ClearItemType()
{
    m_strArrType.RemoveAll();
}

// 

void CNewItemInfo::SetItemFullType()
{
    for (int i = 0; i < m_nTypeCount; i++)
    {
        m_strArrType.Add(m_lpItemType[i]);
    }
}

void CNewItemInfo::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	//
    m_strArrType.RemoveAll();
	
	CDialog::PostNcDestroy();
}

void CNewItemInfo::OnOK() 
{
	// TODO: Add extra validation here
	//
    if ( !UpdateData() )
		return;
	
	CString strType;
    m_ctlCombo.GetWindowText( strType );
	if ( strType.IsEmpty() )
	{
		::AfxMessageBox( _T("Type of array element must be specified" ) );
		return;
	}
	

    if ( !CheckStringLen() )
        return ;

	m_nItemType = GetItemType( strType );

	if( (ASTRING_ITEM_TYPE == m_nItemType || WSTRING_ITEM_TYPE == m_nItemType) && m_nStrLen <= 0)
	{
		::AfxMessageBox( _T("String length must larger than zero") );
		return;
	}
	
	if ( 1 == m_ctrlArray.GetCheck() )
	{
		if ( m_nArraySize <= 0 )
		{
			::AfxMessageBox( _T("A positive array size must be specified") );
			return;
		}
	}
	
	EndDialog( IDOK );
}

void CNewItemInfo::OnCheckArray() 
{
	// TODO: Add your control notification handler code here
	if ( 1 == m_ctrlArray.GetCheck() )
	{
		int nIndex = m_ctlCombo.FindString( -1, _T("STRUCT") );
		ASSERT( CB_ERR != nIndex );
		m_ctlCombo.DeleteString( nIndex );
		
		m_ctrlArraySize.EnableWindow( TRUE );
	}
	else
	{
		m_ctlCombo.AddString( _T("STRUCT") );
		m_ctrlArraySize.EnableWindow( FALSE );
	}
}

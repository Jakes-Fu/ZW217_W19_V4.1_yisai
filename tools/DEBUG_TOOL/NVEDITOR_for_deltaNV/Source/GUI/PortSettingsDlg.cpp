// PortSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"
#include "PortSettingsDlg.h"
#include "DevHound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortSettingsDlg dialog


CPortSettingsDlg::CPortSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortSettingsDlg)
	m_nPort = -1;
	m_nBaudrate = -1;
	m_strFdl = _T("");
	m_bCanNoFdl = TRUE;
	//}}AFX_DATA_INIT
    m_nProduct = 0;
    m_bNewFDLFile = TRUE;

	//@Hongliang Xin, 2009-3-25
	//[[
	m_dwFDLBase = 0;
	m_dwFDL2Base = 0;

    /*CR212808 xmzhang add begin*/          
    m_bNvBaseStr = FALSE;     
    memset(&m_dwNvBaseAddr,0,sizeof(NvBaseAddr));
    /*CR212808 xmzhang add end*/ 
	//]]
}


void CPortSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortSettingsDlg)
	DDX_Control(pDX, IDC_COMBO_PORT, m_ctlPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ctlBaud);
	DDX_Text(pDX, IDC_EDIT_FDL, m_strFdl);
	DDX_Control(pDX, IDC_PRODUCT_COMBO, m_cmbProduct );
    DDX_Control( pDX,IDC_EDIT_FDL2,m_edtFdl2 );
    DDX_Control( pDX,IDC_FDL2_BROWSE,m_btnFdl2 );
    DDX_Text( pDX,IDC_EDIT_FDL2,m_strFdl2 );
	DDX_Control( pDX,IDC_EDIT_FDL_BASE,m_edtFDLBase );
    DDX_Control( pDX,IDC_EDIT_FDL2_BASE,m_edtFDL2Base );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CPortSettingsDlg)
    ON_CBN_SELCHANGE(IDC_PRODUCT_COMBO, OnSelchangeProductType)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FDL_BROWSE, OnFdlBrowse)
    ON_BN_CLICKED( IDC_FDL2_BROWSE,OnFdl2Browse )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortSettingsDlg message handlers

BOOL CPortSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_editNvBase.SubclassDlgItem(IDC_EDIT_NV_BASE,this);
/*
    // Search valible serial ports and open them
    HKEY  hOpenKey;
    DWORD dwRet;
    
    if((dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
        NULL,
        KEY_QUERY_VALUE | KEY_READ,
        &hOpenKey)) == ERROR_SUCCESS)
    {
        const cLen = 255;
        DWORD dwValueLen;
        DWORD dwDataLen;
        TCHAR szValueName[cLen];
        TCHAR szData[cLen];
        DWORD dwType;
        int i = 0;
        
        while(1)
        {
            dwValueLen = cLen;
            dwDataLen = cLen;
            szValueName[0] = 0;
            szData[0] = 0;
            
            dwRet = ::RegEnumValue(hOpenKey,
                i++,
                szValueName,
                &dwValueLen,
                NULL,
                &dwType,
                (BYTE*)szData,
                &dwDataLen
                );
            
            if(dwRet != ERROR_SUCCESS )
                break;
            
            int port;
            int nRet = _stscanf( szData,_T("COM%d"),&port );
            
            if( nRet == 0 )
                continue;
            
            m_ctlPort.AddString( szData );
        }
        RegCloseKey(hOpenKey);        
    }    
*/

	CString strText;
	int nIndex = -1;
/*
	strText.Format( _T("COM%d"), m_nPort );
	nIndex = m_ctlPort.FindString( -1, strText );
	nIndex = ( nIndex == CB_ERR ) ? -1 : nIndex;
	m_ctlPort.SetCurSel( nIndex );
*/	
	strText.Format( _T("%d"), m_nBaudrate );
	nIndex = m_ctlBaud.FindString( -1, strText );
	nIndex = ( nIndex == CB_ERR ) ? -1 : nIndex;
	m_ctlBaud.SetCurSel( nIndex );

	CDevHound dh;
	vector<DEV_INFO> vPorts;
	dh.ScanPort(vPorts);
	UINT nCount = vPorts.size();
	for(UINT j = 0; j<nCount; j++)
	{
		int nCur = m_ctlPort.AddString( vPorts[j].szFriendlyName );
		m_ctlPort.SetItemData(nCur,vPorts[j].nPortNum);
		if(vPorts[j].nPortNum == (unsigned int)m_nPort)
		{
			nIndex = nCur;
		}
	}
	
	m_ctlPort.SetCurSel( nIndex );	
	

	CheckDlgButton( IDC_NEW_FDL_CHK , m_bNewFDLFile );

#define BUF_SIZE 1024
    TCHAR szSection[BUF_SIZE];
    memset(szSection,0,BUF_SIZE);
    
    ::GetPrivateProfileSection( _T("Product list"),szSection,BUF_SIZE, m_strIniFilePath);
    
    CStringArray KeyAndData;
    if(!EnumKeys(szSection,&KeyAndData))
    {
        return FALSE;
    }
    
    for(int i=0;i< KeyAndData.GetSize();i+=2)
    {
        Product_Type pt;
        _tcscpy( pt.szProductType, KeyAndData[i] );

        CString strData = KeyAndData[i+1];

         /*CR212808 xmzhang modify begin*/
        if(_tcsicmp( pt.szProductType, _T("shark") ) == 0)
        {
            _tcscpy(pt.szBase,strData);
        }
        else
        {
            _stscanf(strData,_T("0x%X"), &pt.lNVBase);
        }

        if( _tcsicmp( pt.szProductType, _T("customize") ) == 0 ||_tcsicmp( pt.szProductType, _T("shark") ) == 0)
        /*CR212808 xmzhang modify end*/
        {
            pt.bReadOnly = FALSE;            
        }
        else
        {
            pt.bReadOnly = TRUE;            
        }
        
        m_ProductList.Add(pt);
    }
    

    int nSel = 0;
    for( i=0;i<m_ProductList.GetSize();i++)
    {
        if( m_strProduct.CompareNoCase(m_ProductList[i].szProductType) == 0 )
        {
            nSel = i;
        }
        
        m_cmbProduct.AddString(m_ProductList[i].szProductType);
    }
    
    m_cmbProduct.SetCurSel( nSel );
    OnSelchangeProductType();

    strText.Format(_T("0x%X"),m_dwFDLBase);
    m_edtFDLBase.SetWindowText(strText);

	strText.Format(_T("0x%X"),m_dwFDL2Base);
    m_edtFDL2Base.SetWindowText(strText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortSettingsDlg::OnSelchangeProductType() 
{
    m_nProduct = m_cmbProduct.GetCurSel();
    ASSERT(m_nProduct != -1);
    if(-1 == m_nProduct)
    {
        m_nProduct = 0;
    }

    /*CR212808 xmzhang modify begin*/
    m_strProduct = m_ProductList[m_nProduct].szProductType;
    CString strTemp = m_strProduct;
    strTemp.MakeLower();
    if(strTemp.Find(_T("shark") ) != -1)
    {
        m_bNvBaseStr = TRUE;
    }
    else
    {
        m_bNvBaseStr = FALSE;
    }
    
    CString strText;
    if(TRUE == m_bNvBaseStr)
    {
        strText.Format(_T("%s"),m_ProductList[m_nProduct].szBase);
    }
    else
    {
        strText.Format(_T("0x%X"),m_ProductList[m_nProduct].lNVBase);
    }
    /*CR212808 xmzhang modify end*/
    m_editNvBase.SetWindowText(strText);
    
    m_editNvBase.SetReadOnly(m_ProductList[m_nProduct].bReadOnly);  
	
	BOOL bNand = TRUE;
	if(strTemp.Find(_T("nand") ) == -1)
	{
		bNand = FALSE;
	}
    
    //@ Liu Kai 2005-9-28
    if( bNand )
    {
        // Nand flash need another fdl file
        m_edtFdl2.EnableWindow();
        m_btnFdl2.EnableWindow();
		m_edtFDL2Base.EnableWindow();
    }
    else
    {
        m_edtFdl2.EnableWindow( FALSE );
        m_btnFdl2.EnableWindow( FALSE );
		m_edtFDL2Base.EnableWindow( FALSE );
    }
	/*CR212808 xmzhang add begin*/
	if(m_bNvBaseStr == TRUE)
	{
            m_edtFdl2.EnableWindow();
            m_btnFdl2.EnableWindow();
            m_edtFDL2Base.EnableWindow();
	}
	/*CR212808 xmzhang add end*/
}


void CPortSettingsDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

    CString strText;
/*
    m_ctlPort.GetWindowText( strText );
	if ( strText.IsEmpty() )
	{
		::AfxMessageBox( _T("Please select a port number") );
		return;
	}
    ::_stscanf( strText, _T("COM%d"), &m_nPort );
*/
	int nIndex = m_ctlPort.GetCurSel();
	if(nIndex == -1)
	{
		::AfxMessageBox( _T("Please select a port number") );
		return;
	}

	m_nPort = m_ctlPort.GetItemData(nIndex);

    m_ctlBaud.GetWindowText( strText );
	if ( strText.IsEmpty() )
	{
		::AfxMessageBox( _T("Please select a Baudrate") );
		return;
	}
    ::_stscanf( strText, _T("%d"), &m_nBaudrate );

	if ( !m_bCanNoFdl && m_strFdl.IsEmpty() )
	{
		::AfxMessageBox( _T("You must select a FDL file") );
		return;
	}
    
    m_bNewFDLFile = IsDlgButtonChecked( IDC_NEW_FDL_CHK );

    m_strProduct = m_ProductList[m_nProduct].szProductType;

    /*CR212808 xmzhang modify begin*/
    CString strWindowText;
    m_editNvBase.GetWindowText(strWindowText);
    if(m_bNvBaseStr == TRUE)
    {
        _tcscpy(m_dwNvBaseAddr.szAddr,strWindowText);
        _tcscpy(m_ProductList[m_nProduct].szBase,strWindowText);
    }
    else
    {
        long lRet;
        if(!strWindowText.Left(2).CompareNoCase(_T("0x")))
        {
            // Hex mode
            _stscanf(strWindowText,_T("%x"),&lRet);
        }
        else
        {
            // Decimal mode
            _stscanf(strWindowText,_T("%d"),&lRet);
        }
        m_dwNvBaseAddr.dwAddr = m_ProductList[m_nProduct].lNVBase = lRet;
    }
    /*CR212808 xmzhang modify end*/

	//@Hongliang Xin, 2009-3-25
	//[[
	m_dwFDLBase = m_edtFDLBase.GetDigit();
	m_dwFDL2Base = m_edtFDL2Base.GetDigit();
	//]]

    if( !m_strProduct.CompareNoCase( _T("Nand") ) )
    {
        if( m_strFdl2.IsEmpty() )
        {
            ::AfxMessageBox( _T("You must select a FDL file") );
            return;
        }
    }
    
	EndDialog( IDOK );
//	CDialog::OnOK();
}

void CPortSettingsDlg::OnFdlBrowse() 
{
	static TCHAR BASED_CODE szFilter[] = _T("Binary Files (*.bin)|*.bin|All files (*.*)|*.*||");

    UpdateData();
	CFileDialog dlg(TRUE, NULL, m_strFdl, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strFdl = dlg.GetPathName();
        UpdateData(FALSE);

        return;
    }
}

void CPortSettingsDlg::OnFdl2Browse()
{
    static TCHAR BASED_CODE szFilter[] = _T("Binary Files (*.bin)|*.bin|All files (*.*)|*.*||");
    
    UpdateData();
    CFileDialog dlg(TRUE, NULL, m_strFdl, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        // Copy the file path name
        m_strFdl2 = dlg.GetPathName();
        UpdateData(FALSE);
        
        return;
    }
}

int CPortSettingsDlg::EnumKeys(TCHAR* pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);
    
    int number = 0;
    while(1)
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find('=');
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);
        
        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);
        
        number++;
        
        pSection += nLen + 1;
        
    }
    
    return number;
}
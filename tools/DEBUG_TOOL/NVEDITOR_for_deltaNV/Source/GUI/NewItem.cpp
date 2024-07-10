// NewItem.cpp : implementation file
//

#include "stdafx.h"
#include "nveditor.h"

#include "FileParser.h"
#include "NewItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewItem dialog


CNewItem::CNewItem(CWnd* pParent /*=NULL*/)
	: CDialog(CNewItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewItem)
	m_szItemName = _T("");
	m_szItemType = _T("");
	m_szItemParent = _T("");
	m_szItemDesc = _T("");
	m_nItemID = 0;
	m_nArraySize = 0;
	m_bCheckArray = FALSE;
	m_nStrlen = 1;
	//}}AFX_DATA_INIT

    m_pProject = NULL;
    m_nTotalItemCount = 0;
    m_nCurrItemIndex = 0;
    m_nModuleIndex = 0;
}


void CNewItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewItem)
	DDX_Control(pDX, IDC_EDIT_STR_LENGTH, m_edtStrLen);
	DDX_Control(pDX, IDC_ITEM_ARRAY_SIZE, m_ctlItemArrSize);
	DDX_Control(pDX, IDC_ITEM_TYPE, m_ctlItemType);
	DDX_Control(pDX, IDC_ITEM_PARENT, m_ctlItemParent);
	DDX_Control(pDX, IDOK, m_ctlBtnOk);
	DDX_Control(pDX, IDC_BTN_LAST, m_ctlBtnLast);
	DDX_Control(pDX, ID_BTN_NEXT, m_ctlBtnNext);
	DDX_Text(pDX, IDC_ITEM_NAME, m_szItemName);
	DDX_CBString(pDX, IDC_ITEM_TYPE, m_szItemType);
	DDX_CBString(pDX, IDC_ITEM_PARENT, m_szItemParent);
	DDX_Text(pDX, IDC_ITEM_DESCRIPTION, m_szItemDesc);
	DDX_Text(pDX, IDC_ITEM_ID, m_nItemID);
	DDX_Text(pDX, IDC_ITEM_ARRAY_SIZE, m_nArraySize);
	DDX_Check(pDX, IDC_CHECK_ARRAY, m_bCheckArray);
	DDX_Text(pDX, IDC_EDIT_STR_LENGTH, m_nStrlen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewItem, CDialog)
	//{{AFX_MSG_MAP(CNewItem)
	ON_BN_CLICKED(IDC_BTN_LAST, OnBtnLast)
	ON_BN_CLICKED(ID_BTN_NEXT, OnBtnNext)
	ON_EN_CHANGE(IDC_ITEM_NAME, OnChangeItemName)
	ON_CBN_SELCHANGE(IDC_ITEM_TYPE, OnSelchangeItemType)
	ON_BN_CLICKED(IDC_CHECK_ARRAY, OnCheckArray)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewItem message handlers

void CNewItem::SetProject(CFileParser *pProject, UINT nModuleIndex)
{
   ASSERT(NULL != pProject);
   if(pProject == NULL)
   {
	   return;
   }

   m_pProject = pProject;
   m_nTotalItemCount = m_pProject->GetModuleItemCount (nModuleIndex);
   m_nCurrItemIndex = m_nTotalItemCount;
   m_nModuleIndex = nModuleIndex;   
}

BOOL CNewItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	INT nCmbIndex = m_ctlItemType.AddString (_T("CHAR"));
    m_ctlItemType.SetItemData (nCmbIndex, CHAR_ITEM_TYPE);
    nCmbIndex = m_ctlItemType.AddString (_T("SHORT"));
    m_ctlItemType.SetItemData (nCmbIndex, SHORT_ITEM_TYPE);
    nCmbIndex = m_ctlItemType.AddString (_T("LONG"));
    m_ctlItemType.SetItemData (nCmbIndex, LONG_ITEM_TYPE);

    // [[ delete by wei anli 03.06.12 (不需要ARRAY类型的ARRAY数组)
    //nCmbIndex = m_ctlItemType.AddString ("ARRAR");
    //m_ctlItemType.SetItemData (nCmbIndex, ARRAY_ITEM_TYPE);
    // ]]
    nCmbIndex = m_ctlItemType.AddString (_T("ASTRING"));
    m_ctlItemType.SetItemData (nCmbIndex, ASTRING_ITEM_TYPE);

	nCmbIndex = m_ctlItemType.AddString (_T("WSTRING"));
    m_ctlItemType.SetItemData (nCmbIndex, WSTRING_ITEM_TYPE);

    nCmbIndex = m_ctlItemType.AddString (_T("STRUCT"));
    m_ctlItemType.SetItemData (nCmbIndex, STRUCT_ITEM_TYPE);
    
    m_ctlItemArrSize.EnableWindow (m_bCheckArray);

	m_edtStrLen.EnableWindow(FALSE);

    LoadParent();
    
    m_ctlItemType.SetCurSel (0);
    m_ctlItemParent.SetCurSel (0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewItem::OnBtnLast() 
{
	if(m_pProject == NULL)
		return;

    // TODO: Add your control notification handler code here    
    ModuleItem tmpItem = m_pProject->GetModuleItem(m_nModuleIndex, m_nCurrItemIndex - 1);
    
    m_szItemName = tmpItem.szItemName ;
    m_nItemID    = tmpItem.nItemID ;
    m_ctlItemType.SetCurSel (tmpItem.eItemType);
    for(int nCurr = 0; nCurr < m_ctlItemParent.GetCount (); nCurr++)
    {
        if(m_ctlItemParent.GetItemData (nCurr) == (DWORD)(tmpItem.nParentItemIndex))
        {
            m_ctlItemParent.SetCurSel (nCurr);
        }
    }
    m_szItemDesc  = tmpItem.szItemDescription ;

    UpdateData(FALSE);
    
    m_nCurrItemIndex--;
    
    if(0 == m_nCurrItemIndex)
    {
        m_ctlBtnLast.EnableWindow (FALSE);
    }
}

void CNewItem::OnBtnNext() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    
    if ( (m_szItemName == "") || 
         (m_nItemID == INVALID_MODULE_ID))        
    {
       AfxMessageBox(_T("The Item Name Can't is NULL"));
       return;
    }

	ITEMTYPE eItemType = (ITEMTYPE)m_ctlItemType.GetItemData (m_ctlItemType.GetCurSel ());  

	if ( m_nStrlen <= 0 && (ASTRING_ITEM_TYPE == eItemType || WSTRING_ITEM_TYPE == eItemType))        
    {
       AfxMessageBox(_T("String lenght must be larger than zero!"));
       return;
    }

	if(m_bCheckArray && m_nArraySize <=0 )
	{
		AfxMessageBox(_T("Array size must be larger than zero!"));
	}

    // TODO: Add your control notification handler code here	
    if(m_nTotalItemCount == m_nCurrItemIndex )
    {            
        ModuleItem tmpItem ;

        ZeroMemory(&tmpItem, sizeof(ModuleItem));
        _tcscpy(tmpItem.szItemName , m_szItemName.operator LPCTSTR ());        
        tmpItem.eItemType = eItemType;        
        tmpItem.nParentItemIndex = m_ctlItemParent.GetItemData (m_ctlItemParent.GetCurSel ());
        if(INVALID_MODULE_ID == tmpItem.nParentItemIndex)
        {
            tmpItem.nItemID = m_nItemID;
        }
        else
        {
            tmpItem.nItemID = (m_pProject->GetModuleItem(m_nModuleIndex, tmpItem.nParentItemIndex)).nItemID;
        }
	
		int nLen = m_szItemDesc.GetLength() + 1;
		
		if (nLen > 1)
		{
			tmpItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
			_tcscpy(tmpItem.szItemDescription, m_szItemDesc.operator LPCTSTR ());
		}
       //_tcscpy(tmpItem.szItemDescription , m_szItemDesc.operator LPCTSTR ());

        tmpItem.nItemLength = tmpItem.eItemType;

        int nItemIndex = 0; 

        if(!m_bCheckArray)
        {  
			if(ASTRING_ITEM_TYPE == tmpItem.eItemType || WSTRING_ITEM_TYPE == tmpItem.eItemType )
			{
				//tmpItem.nItemLength = (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? m_nStrlen : m_nStrlen*2;		
				_stprintf(tmpItem.szItemName , _T("%s"), m_szItemName.operator LPCTSTR ());	
				tmpItem.nArraySize = m_nStrlen;
				int nParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
            
				TCHAR szParentName[MAX_PATH];
            
				tmpItem.nParentItemIndex = nParent;
				_tcscpy(szParentName, tmpItem.szItemName);
				//_tcscpy(tmpItem.szItemDescription , _T(""));
		
				tmpItem.nItemLength =  (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? 1 : 2;
				tmpItem.eItemType = (ITEMTYPE)tmpItem.nItemLength;
			
				for(UINT i = 0; i < m_nStrlen; i++)
				{  					
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);				
					nItemIndex = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
				}
			}
			else
			{
				nItemIndex = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);  
			}
        }
        else
        {    			
            tmpItem.eItemType = ARRAY_ITEM_TYPE;
            tmpItem.nArraySize = m_nArraySize;
            int nParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
            
            TCHAR szParentName[MAX_PATH];

			          
            tmpItem.nParentItemIndex = nParent;
            _tcscpy(szParentName, tmpItem.szItemName);
            //_tcscpy(tmpItem.szItemDescription , _T(""));

            tmpItem.eItemType = eItemType;				

            for(int i = 0; i < m_nArraySize; i++)
            {   
				if(ASTRING_ITEM_TYPE == eItemType || WSTRING_ITEM_TYPE == eItemType )
				{		
					tmpItem.nParentItemIndex = nParent;
					tmpItem.eItemType = eItemType;
					tmpItem.nArraySize = m_nStrlen;						
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);		
					
					int nSubParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
            
					TCHAR szSubParentName[MAX_PATH];
            
					tmpItem.nParentItemIndex = nSubParent;
					_tcscpy(szSubParentName, tmpItem.szItemName);
					//_tcscpy(tmpItem.szItemDescription , _T(""));	
		
			
					tmpItem.nItemLength =  (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? 1 : 2;
					tmpItem.eItemType = (ITEMTYPE)tmpItem.nItemLength;
				
					for(UINT j = 0; j < m_nStrlen; j++)
					{  					
						_stprintf(tmpItem.szItemName , _T("%s[%d]"), szSubParentName, j);				
						m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
					}
				}
				else
				{
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);
					m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
				}     
                
            }
        }

        if(STRUCT_ITEM_TYPE == tmpItem.eItemType )
        {
            int nCmbItemIndex;        
            nCmbItemIndex = m_ctlItemParent.AddString (tmpItem.szItemName );
            m_ctlItemParent.SetItemData (nCmbItemIndex, nItemIndex);
        }

        m_szItemName = _T("");
        m_nItemID    = 0;
        m_nArraySize = 0;
        m_bCheckArray = FALSE;
        
        m_szItemDesc = _T("");
        m_ctlItemParent.EnableWindow (TRUE);
        m_ctlItemArrSize.EnableWindow (m_bCheckArray);

        UpdateData(FALSE);
        
        m_ctlItemType.SetCurSel (0);
        m_ctlItemParent.SetCurSel (0);

        m_nTotalItemCount++;
        m_nCurrItemIndex ++;        
        
    }
    else
    { 
        m_nCurrItemIndex++;
        if(m_nTotalItemCount > m_nCurrItemIndex)
        {
            ModuleItem tmpItem = m_pProject->GetModuleItem(m_nModuleIndex, m_nCurrItemIndex);
            
            m_szItemName = tmpItem.szItemName ;
            m_nItemID    = tmpItem.nItemID ;
            m_ctlItemType.SetCurSel (tmpItem.eItemType);
            
            for(int nCurr = 0; nCurr < m_ctlItemParent.GetCount (); nCurr++)
            {
                if(m_ctlItemParent.GetItemData (nCurr) == (DWORD)(tmpItem.nParentItemIndex))
                {
                    m_ctlItemParent.SetCurSel (nCurr);
                }
            }

 			m_szItemDesc  = tmpItem.szItemDescription ;

			
            
        }
        else
        {
            m_szItemName = _T("");
            m_nItemID    = 0;
            m_ctlItemType.SetCurSel (0);
            m_ctlItemParent.SetCurSel (0);
            m_szItemDesc = _T("");
        }

        UpdateData(FALSE);
    }
    
    m_ctlBtnLast.EnableWindow (TRUE);
}

void CNewItem::OnOK() 
{
	// TODO: Add extra validation here
	// TODO: Add extra validation here
	UpdateData(TRUE);
    
    if ( (m_szItemName == "") || 
		(m_nItemID == INVALID_MODULE_ID))        
    {
		AfxMessageBox(_T("The Item Name Can't is NULL"));
		return;
    }

	ITEMTYPE eItemType = (ITEMTYPE)m_ctlItemType.GetItemData (m_ctlItemType.GetCurSel ());  

	if ( m_nStrlen <= 0 && (ASTRING_ITEM_TYPE == eItemType || WSTRING_ITEM_TYPE == eItemType))        
    {
       AfxMessageBox(_T("String lenght must be larger than zero!"));
       return;
    }

	if(m_bCheckArray && m_nArraySize <=0 )
	{
		AfxMessageBox(_T("Array size must be larger than zero!"));
	}
	
    // TODO: Add your control notification handler code here	
    if(m_nTotalItemCount == m_nCurrItemIndex )
    {    
        ModuleItem tmpItem ;
		
        ZeroMemory(&tmpItem, sizeof(ModuleItem));
        _tcscpy(tmpItem.szItemName , m_szItemName.operator LPCTSTR ());
        tmpItem.nItemID = m_nItemID;
        tmpItem.eItemType = eItemType;
        tmpItem.nParentItemIndex = m_ctlItemParent.GetItemData (m_ctlItemParent.GetCurSel ());
        //_tcscpy(tmpItem.szItemDescription , m_szItemDesc.operator LPCTSTR ());

		int nLen = m_szItemDesc.GetLength() + 1;
		
		if (nLen > 1)
		{	
			tmpItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
			_tcscpy(tmpItem.szItemDescription, m_szItemDesc.operator LPCTSTR ());
		}

		
        //if(ARRAY_ITEM_TYPE > tmpItem.eItemType)
        //{
        tmpItem.nItemLength = tmpItem.eItemType;
        //}
        if(!m_bCheckArray)
        {           
			if(ASTRING_ITEM_TYPE == tmpItem.eItemType || WSTRING_ITEM_TYPE == tmpItem.eItemType )
			{
				//tmpItem.nItemLength = (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? m_nStrlen : m_nStrlen*2;		
				_stprintf(tmpItem.szItemName , _T("%s"), m_szItemName.operator LPCTSTR ());	
				tmpItem.nArraySize = m_nStrlen;
				tmpItem.nItemLength = (ASTRING_ITEM_TYPE == eItemType) ? m_nStrlen : m_nStrlen * 2;
				int nParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
            
				TCHAR szParentName[MAX_PATH];
            
				tmpItem.nParentItemIndex = nParent;
				_tcscpy(szParentName, tmpItem.szItemName);
				//_tcscpy(tmpItem.szItemDescription , _T(""));	
		
				tmpItem.nItemLength =  (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? 1 : 2;
				tmpItem.eItemType = (ITEMTYPE)tmpItem.nItemLength;
				tmpItem.nArraySize = 0;
			
				for(UINT i = 0; i < m_nStrlen; i++)
				{  					
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);				
					m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
				}
			}
			else
			{
				m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);  
			}
        }
        else
        {
			tmpItem.eItemType = ARRAY_ITEM_TYPE;
			tmpItem.nArraySize = m_nArraySize;
			int nParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
			
			TCHAR szParentName[MAX_PATH];	

			tmpItem.nParentItemIndex = nParent;
			_tcscpy(szParentName, tmpItem.szItemName);
			//_tcscpy(tmpItem.szItemDescription , _T(""));
			tmpItem.eItemType = eItemType;			

			for(int i = 0; i < m_nArraySize; i++)
			{   
				if(ASTRING_ITEM_TYPE == eItemType || WSTRING_ITEM_TYPE == eItemType )
				{
					tmpItem.nParentItemIndex = nParent;
					tmpItem.eItemType = eItemType;	
					tmpItem.nArraySize = m_nStrlen;
					tmpItem.nItemLength = ASTRING_ITEM_TYPE == eItemType ? m_nStrlen : m_nStrlen * 2;
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);		
					
					int nSubParent = m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem);
            
					TCHAR szSubParentName[MAX_PATH];
            
					tmpItem.nParentItemIndex = nSubParent;
					_tcscpy(szSubParentName, tmpItem.szItemName);
					//_tcscpy(tmpItem.szItemDescription , _T(""));
			
					tmpItem.nItemLength =  (ASTRING_ITEM_TYPE == tmpItem.eItemType) ? 1 : 2;
					tmpItem.eItemType = (ITEMTYPE)tmpItem.nItemLength;
					tmpItem.nArraySize = 0;
				
					for(UINT j = 0; j < m_nStrlen; j++)
					{  					
						_stprintf(tmpItem.szItemName , _T("%s[%d]"), szSubParentName, j);				
						m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
					}
				}
				else
				{
					_stprintf(tmpItem.szItemName , _T("%s[%d]"), szParentName, i);
					m_pProject->InsertModuleItem (m_nModuleIndex, tmpItem); 
				}
				
			}
        }
    }
	
	CDialog::OnOK();
}

void CNewItem::OnChangeItemName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_ctlBtnNext.EnableWindow (TRUE);
    m_ctlBtnOk.EnableWindow (TRUE);
}

void CNewItem::LoadParent()
{
    int nCmbItemIndex;

    nCmbItemIndex = m_ctlItemParent.AddString( _T("ROOT") );
    m_ctlItemParent.SetItemData (nCmbItemIndex, (DWORD)INVALID_MODULE_ID);

    for(int nIndex = 0; nIndex < m_nTotalItemCount; nIndex++)
    {
        ModuleItem tmpItem = m_pProject->GetModuleItem (m_nModuleIndex, nIndex);
        if(STRUCT_ITEM_TYPE == tmpItem.eItemType )
        {
            nCmbItemIndex = m_ctlItemParent.AddString (tmpItem.szItemName );
            m_ctlItemParent.SetItemData (nCmbItemIndex, nIndex );
        }
    }
}

void CNewItem::OnSelchangeItemType() 
{
	// TODO: Add your control notification handler code here
    //ITEMTYPE eSelItemType;

    /*
	eSelItemType = (ITEMTYPE)m_ctlItemType.GetItemData (m_ctlItemType.GetCurSel ());
    if(STRUCT_ITEM_TYPE == eSelItemType)
    {
       m_ctlItemParent.EnableWindow (FALSE);
    }
    */
	ITEMTYPE eSelItemType = (ITEMTYPE)m_ctlItemType.GetItemData (m_ctlItemType.GetCurSel ());
    if(ASTRING_ITEM_TYPE == eSelItemType || WSTRING_ITEM_TYPE == eSelItemType )
    {
		m_edtStrLen.EnableWindow (TRUE);
    }
	else
	{
		m_edtStrLen.EnableWindow (FALSE);
	}

}

void CNewItem::OnCheckArray() 
{
	// TODO: Add your control notification handler code here
    UpdateData();

	m_ctlItemArrSize.EnableWindow (m_bCheckArray);
}

// MySuperGrid.cpp : implementation file
//

#include "stdafx.h"
#include "NVEditor.h"
#include "MySuperGrid.h"
#include "ComboInListView.h"
#include "ListEditCtrl.h"

#include "NewItemInfo.h"

#include "PasteSettingsDlg.h"
#include <atlconv.h>

#include "DlgExport.h"


#include "EditItemID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define GET_ITEM_INDEX( pNode )     ( int )( GetData( ( pNode ) )->GetItemData() )
#define GET_MODULE_INDEX( pNode )   GET_ITEM_INDEX( pNode )


enum ListColumn
{
    NAME_COL,
    CONTENT_COL,
    DATA_TYPE_COL,
    DESCRIPTION_COL,    
    ID_COL,    
    TYPE_COL,
    STATE_COL,
    
};

static LPTSTR lpszCols[] = 
{
    _T("Item Name"),
    _T("Item Value"),
    _T("Data Type"),
    _T("Item Description"),
    _T("Item ID"),
    _T("Item Type"),
    _T("Item State"),    
    0
};

static const char *pItemTypeText[] =
{
    "NONE",
    "CHAR",
    "SHORT",
    "NONE",
    "LONG",
    "ARRAY",
    "ASTRING",
    "STRUCT",
	"WSTRING"
};

/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid
extern CNVEditorApp theApp;

CMySuperGrid::CMySuperGrid()
{
	m_bDrag = FALSE;

    //Added By Leo.Feng
    m_pProject = NULL;
    m_pRootItem    = NULL;

    // @ZHANG Ming : 2003-05-06
    m_bModuleSelected = FALSE;

    m_PasteInfo.bValid = FALSE;
	m_nSelectedModuleIndex = (UINT)(-1);
	m_nSelectedItemIndex = (UINT)(-1);
	m_pCurFindItem = NULL;
	m_pFindStartItem = NULL;
	m_bDispHex = TRUE;
	m_bModifyID = FALSE;
	m_bCustMode = FALSE;
	m_pSyncCallBack = NULL;
}

CMySuperGrid::~CMySuperGrid()
{
	m_pProject  = NULL;
	m_pRootItem = NULL; 
	m_pCurFindItem = NULL;
	m_pFindStartItem = NULL;
}


BEGIN_MESSAGE_MAP(CMySuperGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CMySuperGrid)
	ON_WM_CREATE()
	ON_COMMAND(ID_INSERT_AFTER, OnInsertAfter)
	ON_COMMAND(ID_INSERT_BEFORE, OnInsertBefore)
	ON_COMMAND(ID_MODIFY_ITEMID, OnModifyItemID)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_INSERT_AS_CHILD, OnInsertAsChild)
  	ON_COMMAND(ID_REMOVE_ITEM, OnRemoveItem)
	ON_COMMAND(ID_SET_READONLY, OnSetReadonly)
	ON_COMMAND(ID_ERASE_READONLY, OnEraseReadonly)
	ON_COMMAND(ID_SET_APPENDABLE, OnSetAppendable)
	ON_COMMAND(ID_ERASE_APPENDABLE, OnEraseAppendable)
	ON_COMMAND(ID_MODULE_ERASEREADONLY, OnModuleErasereadonly)
	ON_COMMAND(ID_MODULE_SET_READONLY, OnModuleSetReadonly)
	ON_COMMAND(ID_COPY_ITEM, OnCopyItem)
	ON_COMMAND(ID_PASTE_AFTER, OnPasteAfter)
	ON_COMMAND(ID_PASTE_AS_CHILD, OnPasteAsChild)
	ON_COMMAND(ID_CUT_ITEM, OnCutItem)
	ON_COMMAND(ID_EXPORT_ITEM, OnExportItem)
	ON_COMMAND(ID_MARK_AS_CUSTNV, OnMarkAsCustNV)
	ON_COMMAND(ID_UNMARK_AS_CUSTNV, OnUnmarkAsCustNV)
	ON_COMMAND(ID_UNMARK_ALL_CHANGE_AS_CUSTNV, OnUnmarkAllChangeAsCustNV)
	ON_COMMAND(ID_MARK_ALL_CHANGE_AS_CUSTNV, OnMarkAllChangeAsCustNV)
	ON_COMMAND(ID_UNMARK_ALL_CUSTNV, OnUnmarkAllCustNV)
	ON_COMMAND(ID_MARK_AS_CUSTBASE, OnMarkAsCustbase)
	ON_COMMAND(ID_UNMARK_AS_CUSTBASE, OnUnmarkAsCustbase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid message handlers

int CMySuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this	
	///////////////////

	//associate imagelist with listviewctrl, 
	//this imagelist is only for the first column re: the tree column	
	if(!m_image.Create(IDB_FOLDERS,16,1,RGB(0, 255, 255)))
		return -1;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	//If you want to assiciate images to subitems, create an imagelist
	//and call SetSubItemImageList
	//now the next time you want to map an image from this imagelist to a
	//particular subitem call SetSubItemText(col,text,image-index);

	CImageList imageSubItem;
	if(!imageSubItem.Create(IDB_SUBITEMS,16,1,RGB(0, 255, 255)))
		return -1;

    if(!SetSubItemImageList(&imageSubItem))
		return -1;
	return 0;
}



void CMySuperGrid::InitializeGrid()
{
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	SetExtendedStyle(LVS_EX_GRIDLINES);

    
	//initialize the columns
	LV_COLUMN   lvColumn;	
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 300;
	for(int x = 0; lpszCols[x]!=NULL; x++)
    {
		//make the secondary columns smaller
		if(x)
		  lvColumn.cx = 150;

		lvColumn.pszText = lpszCols[x];
		InsertColumn(x,&lvColumn);
    }
}


//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CMySuperGrid::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


//override, like red!
COLORREF CMySuperGrid::GetCellRGB()
{
	return RGB(192,0,0);
}


//this is my override of GetIcon, override this to set what ever icon suits you
int CMySuperGrid::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{			
// 		int n = GetData(pItem)->GetImage();
// 		if(n!=-1)
// 			return n;

		CItemInfo *lp = GetData(pItem);
		int n = lp->GetImage();
		if( n == 4) //root
		{
			return n;
		}
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			if(lp->GetChanged() && lp->IsCustItem())
			{
				IsCollapsed(pItem) ? iImage = 12/*close icon*/:iImage = 11;/*open icon*/
			}
			else if (!lp->GetChanged() && lp->IsCustItem())
			{
				IsCollapsed(pItem) ? iImage = 9/*close icon*/:iImage = 8;/*open icon*/
			}
			else if (lp->GetChanged() && !lp->IsCustItem())
			{
				IsCollapsed(pItem) ? iImage = 6/*close icon*/:iImage = 5;/*open icon*/
			}
			else
			{
				IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
			}

		}
		else
		{
			if(lp->GetChanged() && lp->IsCustItem())
			{
				iImage = 13; 
			}
			else if (!lp->GetChanged() && lp->IsCustItem())
			{
				iImage = 10;
			}
			else if (lp->GetChanged() && !lp->IsCustItem())
			{
				iImage = 7;//doc icon
			}
			else
			{
				iImage = 2;
			}
			
		}
		return iImage;
	}
	return 0;
}

//override
void CMySuperGrid::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	USES_CONVERSION;
	//put some extra validation here 
	CString str = (CString)plvItem->pszText;
    if(CONTENT_COL == plvItem->iSubItem)
	{
		TCHAR szContent[MAX_ITEM_CONTENT] = {0};			
		CString strContent = plvItem->pszText;
		m_hSelectedItem.bChanged = CompareItem(m_hSelectedItem.szItemContent, strContent);
		SetItemChanged(lpItem, m_hSelectedItem.bChanged);
		SetCustItem(lpItem, m_hSelectedItem.bCustItem);
		SetCustBaseItem(lpItem, m_hSelectedItem.bCustBase);
		if(!strContent.IsEmpty() && 
			(m_hSelectedItem.eItemType == CHAR_ITEM_TYPE ||
			m_hSelectedItem.eItemType == SHORT_ITEM_TYPE ||
			m_hSelectedItem.eItemType == LONG_ITEM_TYPE))
		{
			
			unsigned int nValue;
			strContent.MakeUpper();
			if(strContent.Find(_T("0X"))==0)
			{
				_stscanf(strContent.operator LPCTSTR(),_T("0X%X"),&nValue);
			}
			else
			{
				_stscanf(strContent.operator LPCTSTR(),_T("%d"),&nValue);
			}
			
			if(m_bDispHex)
			{
				_stprintf(szContent,_T("0x%X"),nValue);
			}
			else
			{
				_stprintf(szContent,_T("%u"),nValue);
			}

			if(NULL != m_hSelectedItem.szItemDescription && !m_pProject->CheckValue(m_hSelectedItem.szItemDescription, szContent, m_hSelectedItem.eItemType))
			{
				CString str;
				str.Format(_T("ERROR: [ItemName: %s]:item value %s is not valid."),
					m_hSelectedItem.szItemName, szContent);							
				AfxMessageBox(str);
				return;
			}
			_tcscpy(m_hSelectedItem.szItemContent, szContent);

			str = szContent;
		}
		else
		{
			_tcscpy(m_hSelectedItem.szItemContent, str);	
		}
        
	}
	else if (ID_COL == plvItem->iSubItem)
	{
		str.MakeUpper();
		if(str.Find(_T("0X")) == 0)
		{
			_stscanf(str.operator LPCTSTR(), _T("0X%X"), &m_hSelectedItem.nItemID);
		}
		else
		{
			_stscanf(str.GetBuffer(0), _T("%d"), &m_hSelectedItem.nItemID);
		}		
	}
    else
	{
        //m_hSelectedItem.szItemDescription = str;
		int nLen = ::_tcslen(str) + 1;
		
		if (1 < nLen)
		{
			if(!m_pProject->CheckValue(str.GetBuffer(0), m_hSelectedItem.szItemContent, m_hSelectedItem.eItemType))
			{
				CString str;
				str.Format(_T("Warning: [ItemName: %s]:item value %s is not valid."),
					m_hSelectedItem.szItemName, m_hSelectedItem.szItemContent);							
				AfxMessageBox(str);
			}
			m_hSelectedItem.szItemDescription = (TCHAR*)realloc(m_hSelectedItem.szItemDescription, nLen * sizeof(TCHAR));
			_tcscpy(m_hSelectedItem.szItemDescription, str);

		}
		else
		{
			if (NULL != m_hSelectedItem.szItemDescription)
			{
				free(m_hSelectedItem.szItemDescription);
				m_hSelectedItem.szItemDescription = NULL;
			}
			
		}
		
	}
	

   m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex, m_hSelectedItem);

    CItemInfo *lp = GetData(lpItem);
    if(lp!=NULL)
    {
        if(plvItem->iSubItem==0)
            lp->SetItemText(str);
        else //subitem data 
            lp->SetSubItemText(plvItem->iSubItem-1, str);
        UpdateData(lpItem, lp); 
        //update internal nodes, don't use bUpdateRow here hence we know the item and
        //can optimize the darn thing a little bit.
        SetItemText(plvItem->iItem, plvItem->iSubItem, str.GetBuffer(1));
    }

	if(CONTENT_COL == plvItem->iSubItem)
	{
		int nChild = NumChildren(lpItem);	
		if(nChild > 0)
		{
			if(m_hSelectedItem.eItemType == ASTRING_ITEM_TYPE)
			{
				POSITION pos = GetHeadPosition(lpItem);
				char *pBuf = W2A(plvItem->pszText);
				ModuleItem mi;
				int i = 0;
				int nLen = strlen(pBuf);
				CString strSub;
				while(pos)
				{
					CTreeItem *pChild = GetNextChild(lpItem,pos);

					mi = m_pProject->GetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex+i+1);
					BYTE data = (i<nLen) ? ((BYTE)pBuf[i]) : (BYTE)0;
					
					if(m_bDispHex)
					{
						_stprintf(mi.szItemContent,_T("0x%X"),data);
					}
					else
					{
						_stprintf(mi.szItemContent,_T("%u"),data);
					}
					
				
					strSub = mi.szItemContent;
					m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex+i+1, mi);

					
					CItemInfo *lpSub = GetData(pChild);
					if(lpSub!=NULL)
					{			
						lpSub->SetSubItemText(plvItem->iSubItem-1, strSub);
						UpdateData(pChild, lpSub); 		
						if(!IsCollapsed(lpItem))
						{
							SetItemText(plvItem->iItem+i+1, plvItem->iSubItem, strSub.GetBuffer(1));
						}
					}
					i++;
				}
			}
			else if(m_hSelectedItem.eItemType == WSTRING_ITEM_TYPE && nChild > 0)
			{
				POSITION pos = GetHeadPosition(lpItem);
				wchar_t *pBuf = plvItem->pszText;
				ModuleItem mi;
				int i = 0;
				int nLen = _tcslen(pBuf);
				CString strSub;
				while(pos)
				{
					CTreeItem *pChild = GetNextChild(lpItem,pos);

					mi = m_pProject->GetModuleItem(m_nSelectedModuleIndex,m_nSelectedItemIndex+i+1);
					
					wchar_t data = (i<nLen) ? ((wchar_t)pBuf[i]) : (wchar_t)0;
					
					if(m_bDispHex)
					{
						_stprintf(mi.szItemContent,_T("0x%X"), data);
					}
					else
					{
						_stprintf(mi.szItemContent,_T("%u"), data);
					}

					strSub = mi.szItemContent;
					m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex+i+1, mi);

					
					CItemInfo *lpSub = GetData(pChild);
					if(lpSub!=NULL)
					{			
						lpSub->SetSubItemText(plvItem->iSubItem-1, strSub);
						UpdateData(pChild, lpSub); 				
						if(!IsCollapsed(lpItem))
						{
							SetItemText(plvItem->iItem+i+1, plvItem->iSubItem, strSub.GetBuffer(1));
						}
					}
					i++;
				}
			}
		}
		else
		{
			// bug 468727
			if (m_hSelectedItem.nParentItemIndex == INVALID_MODULE_ITEM_INDEX)
			{
				UpdatedTextColor(lpItem);
				return;
			}
			CTreeItem *pParent = GetParentItem(lpItem);
			if(pParent != NULL )
			{
				CItemInfo *lpParent = GetData(pParent);
				if(lpParent != NULL)
				{
					UINT nParentIndex = lpParent->GetItemData();
					if(nParentIndex != INVALID_MODULE_ITEM_INDEX)
					{
						ModuleItem mi = m_pProject->GetModuleItem(m_nSelectedModuleIndex,nParentIndex);

						if(mi.eItemType == ASTRING_ITEM_TYPE)
						{
							int nChildIndex = 0;
							POSITION pos = GetHeadPosition(pParent);						
							while(pos)
							{
								CTreeItem *pChild = GetNextChild(lpItem,pos);
								if(pChild == lpItem)
								{
									break;
								}
								else
								{
									nChildIndex++;
								}
							}

							ASSERT(nChildIndex<(int)mi.nArraySize);
							
							CString strItem = (CString)plvItem->pszText;
							char szBuf[2] = {0};
							strItem.MakeUpper();
							UINT nValue = 0;
							if(strItem.Find(_T("0X"))==0)
							{								
								_stscanf(strItem,_T("0X%X"),&nValue);
								szBuf[0] = (char)nValue;								
							}
							else
							{								
								_stscanf(strItem,_T("%d"),&nValue);
								szBuf[0] = (char)nValue;	
							}

							mi.szItemContent[nChildIndex] = (TCHAR)*A2W(szBuf);

							m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex - nChildIndex -1, mi);

							CString strParent = mi.szItemContent;

							CItemInfo *lpParent = GetData(pParent);
							if(lpParent!=NULL)
							{			
								lpParent->SetSubItemText(plvItem->iSubItem-1, strParent);
								UpdateData(pParent, lpParent); 				
								SetItemText(plvItem->iItem - nChildIndex -1 , plvItem->iSubItem, strParent.GetBuffer(1));
							}
							
						}
						else if(mi.eItemType == WSTRING_ITEM_TYPE)
						{
							int nChildIndex = 0;
							POSITION pos = GetHeadPosition(pParent);						
							while(pos)
							{
								CTreeItem *pChild = GetNextChild(lpItem,pos);
								if(pChild == lpItem)
								{
									break;
								}
								else
								{
									nChildIndex++;
								}
							}
							CString strItem = (CString)plvItem->pszText;
							wchar_t szBuf[2] = {0};
							strItem.MakeUpper();
							UINT nValue = 0;
							if(strItem.Find(_T("0X"))==0)
							{								
								_stscanf(strItem,_T("0X%X"),&nValue);
								szBuf[0] = (wchar_t)nValue;								
							}
							else
							{								
								_stscanf(strItem,_T("%d"),&nValue);
								szBuf[0] = (wchar_t)nValue;	
							}

							mi.szItemContent[nChildIndex] = szBuf[0];

							m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex - nChildIndex -1, mi);

							CString strParent = mi.szItemContent;

							CItemInfo *lpParent = GetData(pParent);
							if(lpParent!=NULL)
							{			
								lpParent->SetSubItemText(plvItem->iSubItem-1, strParent);
								UpdateData(pParent, lpParent); 				
								SetItemText(plvItem->iItem - nChildIndex -1 , plvItem->iSubItem, strParent.GetBuffer(1));
							}
						}
					}
				}
			}
		}
	}

	UpdatedTextColor(lpItem);
// 	if(CONTENT_COL == plvItem->iSubItem)
// 	{
// 		//Refresh();
// 		/*SetValueChange();*/
// 		//m_bShowModify = TRUE;
// 		if (m_hSelectedItem.bChanged)
// 		{
// 			lp->SetTextClr(RGB(255,0,0));
// 		    lp->SetChanged(TRUE);
// 		}
// 		else
// 		{
// 			lp->SetTextClr(RGB(0,0,0));
// 		    lp->SetChanged(FALSE);
// 		}
// 		
// 		
// 	}
	
    //else
	//	CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}
			
BOOL CMySuperGrid::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	if(ht.iItem!=-1)
	{
#ifdef _WHAT_EVER_
		CTreeItem*pSelItem = GetTreeItem(ht.iItem);
		if(pSelItem!=NULL)
		{	
			CItemInfo* pInfo = GetData(pSelItem);
			if((ht.iSubItem==0) && (pInfo->GetItemText()==CString(_T("Hello World"))))
			{
				AfxMessageBox("no can do buddy");
				return 0;
			}

			for(int i=0;i<pInfo->GetItemCount(); i++)
			{
				if(i==ht.iSubItem-1)
				{
					CString str=pInfo->GetSubItem(i);
					if(str == CString(_T("Ethan Hunt"))) 
					{
						AfxMessageBox("Tom Cruise\nliked him in a few good men");
						return 0;
					}
				}
			}
		}
#endif
	}

    //OnItemSelchanged();

	return 1;
}

//override called when OnLButtondown
void CMySuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	UNREFERENCED_PARAMETER(point);
	UNREFERENCED_PARAMETER(nFlags);
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
			if(ctrlType==pInfo->CONTROLTYPE::combobox) 
			{
					CStringList* list=NULL;
					pInfo->GetListData(ht.iSubItem-1, list);
					CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
					UNREFERENCED_PARAMETER(pList);

			}
		}								
		/*
		else //activate default edit control
			CSuperGridCtrl::OnControlLButtonDown(nFlags, point, ht);
		*/
	}
}

BOOL CMySuperGrid::OnVkReturn()
{
	BOOL bResult=FALSE;
	int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
    int iSubItem = GetCurSubItem();
	if( (-1 != iSubItem) && iItem != -1)
	{
		CTreeItem* pSelItem = GetTreeItem(iItem);
		if(pSelItem!=NULL)
		{	
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			CItemInfo* pInfo = GetData(pSelItem);
			CItemInfo::CONTROLTYPE ctrlType;
			if(pInfo->GetControlType(iSubItem-1, ctrlType))
			{	
				switch(ctrlType)
				{
					/*put in your own control here*/
					case pInfo->CONTROLTYPE::datecontrol:break;
					case pInfo->CONTROLTYPE::spinbutton:break;
					case pInfo->CONTROLTYPE::dropdownlistviewwhatevercontrol:break;
					case pInfo->CONTROLTYPE::combobox: 
						{

							CStringList* list=NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox * pList = ShowList(iItem, iSubItem, list);
							UNREFERENCED_PARAMETER(pList);
							bResult=TRUE; //I'll handle it from here
						}break;
					default:break;
				}
			}
		}
	}     

	CTreeItem * pSelItem = GetTreeItem( iItem );
    int nIndex = GetData( pSelItem )->GetItemData();
	int nCurSelMdl = GetSelectedModuleIndex();
    ModuleItem miItem = m_pProject->GetModuleItem(nCurSelMdl, nIndex );

	// enable editing ID

	CString strFind = GetItemText(iItem, iSubItem);

	if (m_bModifyID && (ID_COL == iSubItem) && (INVALID_MODULE_ID == m_hSelectedItem.nParentItemIndex) && (!strFind.IsEmpty()))
	{
		CEditItemID  editIDdlg;
		editIDdlg.SetProject(m_pProject, nCurSelMdl, nIndex);

		if (IDOK == editIDdlg.DoModal())
		{
			UINT uNewID = editIDdlg.GetNewID();
			m_hSelectedItem.nItemID = uNewID;
			m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex, m_hSelectedItem);
			UpdateItemID(pSelItem, uNewID);
			Refresh();
		}	

	}
	
// 	int nFlag = -1;
// 	if (strFind.IsEmpty())
// 	{
// 		nFlag = 0;
// 	}
    
    //Added By Leo.Feng, Disable edit unless content column 
    // Kai Liu 2003.3.4 enable editing description column
    if( !((CONTENT_COL == iSubItem ) || (DESCRIPTION_COL == iSubItem) /*|| (ID_COL == iSubItem && (0 != nFlag))*/) || 
        ((ARRAY_ITEM_TYPE == (GetData(GetTreeItem(iItem)))->GetItemType ())/* && !((ID_COL == iSubItem) && -1 == nFlag)*/) || // the item type must be not struct type
		((STRUCT_ITEM_TYPE == (GetData(GetTreeItem(iItem)))->GetItemType ())/* && !((ID_COL == iSubItem) && -1 == nFlag)*/)||  // the item type must be not struct type
		(miItem.bReadOnly && SUPER_PERMISSION != theApp.GetPermission()))
    {
        bResult = TRUE;
    }

	return bResult;
}

#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CMySuperGrid::ShowList(int nItem, int nCol, CStringList *lstItems)
{
	CString strFind = GetItemText(nItem, nCol);

	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();//dropdown area
	
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
	pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}

int CMySuperGrid::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent=0;
	if(pWnd!=NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if(pFont!=NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while(pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}

//HOWTO: 
void CMySuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str)
{
	CTreeItem *pSelItem = GetTreeItem(nIndex);
	if(pSelItem != NULL)
	{
		SetRedraw(0);
		BOOL bUpdate = FALSE;
		if(!IsCollapsed(pSelItem))
			bUpdate = TRUE;//Children are expanded, want to see update right away if not no visual update

		CItemInfo* lpRelative = new CItemInfo();
		lpRelative->SetItemText(str);
		lpRelative->AddSubItemText(_T("I am"));
		lpRelative->AddSubItemText(_T("now"));
		lpRelative->AddSubItemText(_T("going to insert"));
		lpRelative->AddSubItemText(_T("some items"));

		CTreeItem* pParent = InsertItem(pSelItem, lpRelative, bUpdate);
		for(int i=0; i < GetNumCol(); i++)
		{
			CItemInfo* lpItemInfo = new CItemInfo();
			CString strItem;
			strItem.Format(_T("Item %d"), i);
			//add items text
			lpItemInfo->SetItemText(strItem);
			//add subitem text
			for(int y=0;y < GetNumCol()-1; y++) 
			{
				CString str;
				str.Format(_T("SubItem %d of %s"), y, lpItemInfo->GetItemText());
				lpItemInfo->AddSubItemText(str);
			}
			//set combobox in last col
			lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, GetNumCol()-2);
			CStringList list;
			for(int x = 0; x < 3; x++)
			{
				CString str;
				str.Format(_T("ListItem %d"),x);
				list.AddTail(str);
			}
			lpItemInfo->SetListData(GetNumCol()-2, &list);

			InsertItem(pParent, lpItemInfo);
		}
		SetRedraw(1);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}

void CMySuperGrid::HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;

	pDC->TextOut(10, nLineY, _T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); //print from current node
	if(nIndex!=-1)
	{
		//GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent =(CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);

			CString strData = lp->GetItemText();
			strData+='\t';
			nLineY+=cy;
			for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
			{
				CString str = lp->GetSubItem(nCol);
				strData+=str;
				strData+='\t';
			}	
			pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
			nLineY+=cy;
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent,pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  

				if(!IsChildOf(pParent, pCur))
					break;
				else
				if(pCur==pItem)
					break;
				
				CItemInfo* lp = GetData(pCur);
				CString strData = lp->GetItemText();
				strData+='\t';
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					strData+=str;
					strData+='\t';
				}	
				pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
				nLineY+=cy;
				pItem=pCur;
			}
		}
	 }
}

void CMySuperGrid::HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;

	pDC->TextOut(10,nLineY,_T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	if(!GetItemCount())
		return;

	int nIndex=0;//has to be the root 
	if(nIndex!=-1)
	{
		//do a GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);
			if(lp->GetCheck())//very hard :)=
			{
				CString strData=lp->GetItemText();
				strData+='\t';
				nLineY+=cy;
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
	
				{
					CString str = lp->GetSubItem(nCol);
					strData+=str;
					strData+='\t';
				}	
				pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
				nLineY+=cy;
			}
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  
				if(!IsChildOf(pParent, pCur))
					break;
				else
				if(pCur==pItem)
					break;
				

				CItemInfo* lp = GetData(pCur);
				if(lp->GetCheck())
				{
					CString strData = lp->GetItemText();
					strData+='\t';
					for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					{
						CString str = lp->GetSubItem(nCol);
						strData+=str;
						strData+='\t';
					}	
					pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
					nLineY+=cy;
				}
				pItem=pCur;
			}
		}//while
	 }//if
}

//HOWTO: Search nodeptr that have a specific item and subitems also shows you how to select the node and delete it
void CMySuperGrid::HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void)
{
	//one Item and two Subitems
	CTreeItem *pNode =	Search(__T("Hello World"),_T("Happy"),_T("Programming"),NULL);
	
	if(pNode!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));
	//one Item and one Subitem
	CTreeItem *pNode1 = Search(_T("Mission: Impossible"),_T("Allan Nielsen"),NULL);
	if(pNode1!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode1);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());

	}
	else AfxMessageBox(_T("not found"));
	
	//one Item and one Subitem
	CTreeItem *pNode2 = Search(_T("Training Agent"),_T("Mr. Bean"),NULL);
	if(pNode2!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode2);
		AfxMessageBox(_T("Found Item") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));

	CTreeItem *pNode3 = Search(_T("BC"),NULL);
	if(pNode3!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode3);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
		int nIndex = SelectNode(pNode3);
		if(nIndex!=-1)
		if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
			DeleteItemEx(pNode3, nIndex);
	}
	else AfxMessageBox(_T("not found"));

	CTreeItem *pItem = NULL;

	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)GetNextRoot(pos); 
		pItem = SearchEx(pRoot, "AB");
		if(pItem!=NULL)
		{
			CItemInfo *pInfo = GetData(pItem);
			AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
			int nIndex = SelectNode(pItem);
			if(nIndex!=-1)
			if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
				DeleteItemEx(pItem, nIndex);
		}
	}
		
	if( pItem == NULL )
		AfxMessageBox(_T("not found"));
}

//this is just one way to search items...strItem must match and then all subitems must be
//a match before returning the node
//the search function here search all nodes regardless if collapsed or expanded
CMySuperGrid::CTreeItem* CMySuperGrid::Search(const CString& strItem,...)
{
	if(!GetItemCount())
		return NULL;

	va_list args;
	va_start(args, strItem);
	int nCount=0;
	
	for(;;)
	{
		LPCTSTR lpsz = va_arg(args, LPCTSTR);
		if(lpsz==NULL)
			break;
	   nCount++;
	}


	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
		CTreeItem *pItem = pParent;
		CItemInfo* lp = GetData(pParent);
		CString strData = lp->GetItemText();
		if(strData==strItem)//must be a match before going further...suit you self
		{
			va_start(args, strItem);
			int nResult=0;
			for(int i=0; i<nCount;i++)
			{
				LPCTSTR lpsz = va_arg(args, LPCTSTR);   
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					if(!str.CompareNoCase(lpsz))
					{
						nResult++;
						break;
					}
				}	
			}
			va_end(args);
			if(nCount==nResult)//all args was found return node
				return pParent;
		}

		//GetNext ....loop through all children 
		for(;;)
		{
			CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not, set to TRUE if only visible items must be included in the search*/);	  
			if(!IsChildOf(pParent, pCur))
				break;
			else
			if(pCur==pItem)
				break;
			CItemInfo* lp = GetData(pCur);
			CString strData = lp->GetItemText();
			if(strData==strItem)//must be a match before going further
			{
				va_start(args, strItem);
				int nResult=0;
				for(int i=0; i<nCount;i++)
				{
					LPCTSTR lpsz = va_arg(args, LPCTSTR);   
					for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					{
						CString str = lp->GetSubItem(nCol);
						if(!str.CompareNoCase(lpsz))
						{
							nResult++;
							break;
						}
					}	
				}
				va_end(args);
				if(nCount==nResult)//all args was found return node
					return pCur;
			}
			pItem=pCur;//next;
		}
	}	
	return NULL;
}



void CMySuperGrid::SortData()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex==-1)
		return;

	if(AfxMessageBox(_T("Sort roots and all children(Yes)\nor just sort the rootitems(No)"),MB_YESNO)==IDYES)
		SortEx(1);			
	else
		SortEx(0);
}

void CMySuperGrid::OnFind(const CString& strTarget, const DWORD dwFlag)
{
	int nIndex = GetSelectedItem();
	if(nIndex != -1)
	{
		m_pFindStartItem = GetTreeItem(nIndex);
	}
	else
	{
		m_pFindStartItem = m_pRootItem;
	}
	m_pCurFindItem = NULL;	


	OnFindNext(strTarget,dwFlag);

}

BOOL CMySuperGrid::OnFindNext(CString strTarget, const DWORD dwFlag)
{
	strTarget.MakeLower();

	if(m_pFindStartItem == NULL)
	{
		return FALSE;
	}

	BOOL bDescript = FALSE;
	if(m_pCurFindItem == NULL)
	{
		m_pCurFindItem =  Find(m_pFindStartItem,strTarget,dwFlag,bDescript,TRUE);
	}
	else
	{
		m_pCurFindItem =  Find(m_pCurFindItem, strTarget,dwFlag,bDescript,FALSE);
	}

	if(m_pCurFindItem==NULL)
	{
		AfxMessageBox(_T("Find over!"));
		return FALSE;
	}
	else
	{
		if(bDescript)
		{
			SetCurSubItem(DESCRIPTION_COL);
		}
		else
		{
			SetCurSubItem(0);
		}
		SelectNode(m_pCurFindItem);	
		return TRUE;
	}
}
CMySuperGrid::CTreeItem* CMySuperGrid::Find(CTreeItem *pStartPosition, 
											const CString& strItem,
											const DWORD dwFlag,
											BOOL &bDescript,
											BOOL bCheckFirst /*= TRUE*/)
{
	if(bCheckFirst)
	{
		if(IsMatch(pStartPosition,strItem,dwFlag,bDescript))
		{
			return pStartPosition;
		}
	}

	//查找子孙节点
	CTreeItem *pFindItem = DeepFind(pStartPosition,strItem,dwFlag,bDescript,FALSE);
	if(pFindItem != NULL)
	{
		return pFindItem;
	}

	//??????
	CTreeItem *pParent = GetParentItem(pStartPosition);
	if(pParent != NULL) 
	{
		POSITION pos = GetHeadPosition(pParent);
		BOOL bFindCur = FALSE;
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pParent, pos);
			if(pChild == pStartPosition)
			{
				bFindCur=TRUE;
				continue;
			}
			if(!bFindCur)
			{
				continue;
			}

			if(IsMatch(pChild,strItem,dwFlag,bDescript))
			{
				return pChild;
			}			

			//查找兄弟字节点		
			pFindItem = DeepFind(pChild,strItem,dwFlag,bDescript,FALSE);
			if(pFindItem!=NULL)
			{				
				return pFindItem;
			}
		} 
	}
	else
	{
		return NULL;
	}

	//查找叔父及其子孙节点
	CTreeItem *pSupperParent = NULL;
	do 
	{		
		pSupperParent = GetParentItem(pParent);
		if(pSupperParent != NULL) 
		{	
			CTreeItem *pParentBrother = NULL;
			POSITION pos = GetHeadPosition(pSupperParent);
			BOOL bFindCur = FALSE;
			while (pos)
			{
				CTreeItem *pChild = GetNextChild(pSupperParent, pos);
				if(pChild == pParent)
				{
					bFindCur=TRUE;
					continue;
				}
				if(!bFindCur)
				{
					continue;
				}
				pParentBrother = pChild;
				break;			
			} 	

			if(pParentBrother != NULL)
			{		
				if(IsMatch(pParentBrother,strItem,dwFlag,bDescript))
				{
					return pParentBrother;
				}			

				CTreeItem * pFindItem = Find(pParentBrother,strItem,dwFlag,bDescript,FALSE);
				if(pFindItem != NULL)
				{
					return pFindItem;
				}
			}
			
			pParent = pSupperParent;
			
		} 
	}while( pSupperParent != m_pRootItem && pSupperParent!= NULL);		

	return NULL;
}
BOOL CMySuperGrid::IsMatch(CTreeItem *pItem,const CString& strItem,const DWORD dwFlag,BOOL &bDescript)
{
	if(pItem == NULL)
	{
		return FALSE;
	}

	CItemInfo* lp = GetData(pItem);
	if(lp==NULL)
	{
		return FALSE;
	}

	// target is empty, only find changed item
	if( strItem.IsEmpty() )
	{
		if( GET_FIND_FLAG_CHANGED(dwFlag) != 0 )
		{
			// root can not be changed always.
			if(pItem == m_pRootItem)
			{
				return FALSE;
			}

			ModuleItem mi = m_pProject->GetModuleItem(GetModuleIndexOfNode(pItem),lp->GetItemData());
			if(mi.bChanged)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}


	CString strName = lp->GetItemText();
	strName.MakeLower();

	BOOL bFullMatch = GET_FIND_FLAG_FULLMATCH(dwFlag)!=0 ? TRUE: FALSE;

	BOOL bFind = FALSE;
	if(bFullMatch)
	{
		bFind = strName.CompareNoCase(strItem)==0 ? TRUE: FALSE;
	}
	else
	{
		bFind = strName.Find(strItem)!=-1 ? TRUE: FALSE;
	}

	if(bFind)
	{
		if(GET_FIND_FLAG_CHANGED(dwFlag) != 0)
		{
			// root can not be changed always.
			if(pItem != m_pRootItem)
			{
				ModuleItem mi = m_pProject->GetModuleItem(GetModuleIndexOfNode(pItem),lp->GetItemData());
				if(mi.bChanged)
				{
					return TRUE;
				}
			}				
		}
		else
		{
			return TRUE;
		}		
	}
	else // find description
	{
		CString strDesc = lp->GetSubItem(DESCRIPTION_COL-1);
		strDesc.MakeLower();
		bFind = FALSE;
		if(bFullMatch)
		{
			bFind = strDesc.CompareNoCase(strItem)==0 ? TRUE: FALSE;
		}
		else
		{
			bFind = strDesc.Find(strItem)!=-1 ? TRUE: FALSE;
		}
		
		if(bFind)
		{
			if(GET_FIND_FLAG_CHANGED(dwFlag) == 0)
			{
				bDescript = TRUE;
				return TRUE;
			}
		}
	}

	return FALSE;
}

CMySuperGrid::CTreeItem* CMySuperGrid::DeepFind(CTreeItem *pStartPosition, 
												const CString& strItem,
												const DWORD dwFlag,
												BOOL &bDescript,
												BOOL bCheckFirst/* = TRUE*/)
{
	if(bCheckFirst)
	{
		if(IsMatch(pStartPosition,strItem,dwFlag,bDescript))
		{
			return pStartPosition;
		}
	}

	POSITION pos = GetHeadPosition(pStartPosition);
	while (pos)
	{
		CTreeItem *pChild = GetNextChild(pStartPosition, pos);
		if(IsMatch(pChild,strItem,dwFlag,bDescript))
		{
			return pChild;
		}

		pChild = DeepFind(pChild, strItem,dwFlag,bDescript,FALSE);
		if(pChild!=NULL)
			return pChild;
	}    
	return NULL;
}

//another search thing
CMySuperGrid::CTreeItem* CMySuperGrid::SearchEx(CTreeItem *pStartPosition, const CString& strItem)
{
	CItemInfo* lp = GetData(pStartPosition);
	//if(lp->GetCheck()) another condition here maybe
	CString strData = lp->GetItemText();
	if(strData == strItem)
	{
		return pStartPosition;
	}

	const int nChildren = NumChildren(pStartPosition);
	if (nChildren > 0)
	{
		POSITION pos = GetHeadPosition(pStartPosition);
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pStartPosition, pos);
			CItemInfo* lp = GetData(pChild);
			CString strData = lp->GetItemText();
			if(strData==strItem)
			{
				return pChild;
			}
		}
	}

	POSITION pos = GetHeadPosition(pStartPosition);
	while (pos)
	{
		CTreeItem *pChild = GetNextChild(pStartPosition, pos);
		CItemInfo* lp = GetData(pChild);
		CString strData = lp->GetItemText();
		if(strData == strItem)
		{
			return pChild;
		}

		pChild = SearchEx(pChild, strItem);
		if(pChild!=NULL)
			return pChild;
	}    
	return NULL;
}

BOOL CMySuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	UNREFERENCED_PARAMETER(pItem);
	UNREFERENCED_PARAMETER(nIndex);
	return 0;
}

BOOL CMySuperGrid::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	UNREFERENCED_PARAMETER(pItem);
	UNREFERENCED_PARAMETER(iItem);
	return 1;
}

BOOL CMySuperGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	UNREFERENCED_PARAMETER(iItem);

    if ( IsModuleNode( pItem ) )
    {
        SetModuleExpanded( pItem, TRUE );
    }
    else if ( IsItemNode( pItem ) )
    {
//        ASSERT( GetSelectedModuleIndex() != INVALID_MODULE_ID );
        SetItemExpanded( GetModuleIndexOfNode( pItem ),
                         pItem,
                         TRUE );
    }

	return 1;
}

BOOL CMySuperGrid::OnCollapsing(CTreeItem *pItem)
{
	UNREFERENCED_PARAMETER(pItem);
	return 1;
}

BOOL CMySuperGrid::OnItemCollapsed(CTreeItem *pItem)
{
    if ( IsModuleNode( pItem ) )
    {
        SetModuleExpanded( pItem, FALSE );
    }
    else if ( IsItemNode( pItem ) )
    {
//        ASSERT( GetSelectedModuleIndex() != INVALID_MODULE_ID );
        SetItemExpanded( GetModuleIndexOfNode( pItem ),
                         pItem,
                         FALSE );
    }

	return 1;
}

CImageList *CMySuperGrid::CreateDragImageEx(int nItem)
{
	UNREFERENCED_PARAMETER(nItem);

		if(m_bDrag)
			return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
		else
			return NULL;
}

void CMySuperGrid::_DeleteAll()
{
	DeleteAll();//call CSuperGridCtrl::DeleteAll();
	//add some new data
	CItemInfo* lp = new CItemInfo();
	lp->SetImage(4);
	//add item text
	lp->SetItemText(_T("New data"));
	//Create root item
	CTreeItem * pRoot = InsertRootItem(lp);//previous we call CreateTreeCtrl(lp)
	if( pRoot == NULL )
		return;
	//insert items	
	int nCol = GetNumCol();
	for(int i=0; i < nCol; i++)
	{
		CItemInfo* lpItemInfo = new CItemInfo();
		CString strItem;
		strItem.Format(_T("Item %d"),i);
		//add items text
		lpItemInfo->SetItemText(strItem);
		//add subitem text
		for(int y=0;y < nCol-1; y++) 
		{
			CString str;
			str.Format(_T("subItem %d of %s"),y,lpItemInfo->GetItemText());
			lpItemInfo->AddSubItemText(str);
			lpItemInfo->AddSubItemText(str);
		}
		//insert the iteminfo with ParentPtr
		CTreeItem* pParent = InsertItem(pRoot, lpItemInfo);
		//other nodes
		if(i%nCol)
		{
			CTreeItem* pParent1=NULL;
			//CTreeItem* pParent2=NULL;
			for(int x=0; x < nCol; x++)
			{
				CItemInfo* lpItemInfo = new CItemInfo();
				CString strItem;
				strItem.Format(_T("Item %d"),x);
				lpItemInfo->SetItemText(strItem);
				for(int z=0; z < nCol-1; z++) 
				{
					CString str;
					str.Format(_T("subItem %d of %s"),z, lpItemInfo->GetItemText());
					lpItemInfo->AddSubItemText(str);
				}
				pParent1 = InsertItem(pParent, lpItemInfo);
				
			}
		}
	}
	//expand one level
	Expand(pRoot, 0 /*listview index 0*/); 
	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	SetItemState(0, uflag, uflag);
}

void CMySuperGrid::DynamicUpdateSomeItems(int nItem)
{
	CTreeItem*pItem = GetTreeItem(nItem);
	CItemInfo *lp = GetData(pItem);
	if(lp!=NULL)
	{
		lp->SetItemText("Dynamic");
		//subitems
		lp->SetSubItemText(0,"updates");
		lp->SetSubItemText(1, "are");
		lp->SetSubItemText(2, "not always",2/*image index 2*/);
		lp->SetSubItemText(3, "fun");
		//update internal nodes
		UpdateData(pItem, lp, TRUE); 
		//set bUpdateRow to TRUE here, this would update the listview row e.g write the content of CItemInfo
	}
}

void CMySuperGrid::SetNewImage(int nItem)
{
	CTreeItem*pItem = GetTreeItem(nItem);
	CItemInfo *lp = GetData(pItem);
	if(lp!=NULL)
	{
//		lp->SetSubItemImage(3/*subitem*/,4/*image index*/);
//		lp->SetSubItemImage(2/*subitem*/,4/*image index*/);
		//update internal node
		UpdateData(pItem, lp, TRUE); 
		InvalidateItemRect(nItem);
	}
}

///////////////////////////////////////////////////////////////////////////////
//Added by Leo.Feng at 2003-01-15
BOOL CMySuperGrid::SetProject(CFileParser *pProject)
{
   if( (NULL == pProject) ||
       (NULL != m_pProject) )
   {
       return FALSE;
   }
   
    m_pProject = pProject;

    return TRUE;
}

void CMySuperGrid::CloseProject()
{
    if(NULL == m_pProject)
    {
        return;
    }

    DeleteAll();
    m_pProject = NULL;

    // @ZHANG Ming : 2003-05-06
    m_bModuleSelected = FALSE;
}

void CMySuperGrid::Refresh()
{
    if(NULL == m_pProject)
    {
        return;
    }

    /*
    LPTSTR lpszCols[] = 
    {
    _T("Item Name"), _T("Item ID"), _T("Item Type"),_T("Item State"),
    _T("Data Type"), _T("Item Value"), _T("Item Description"), 0
    };
    */

    int nSelectedItem = -1;
    int nSel = GetSelectedItem();
    if ( -1 != nSel )
    {
        CTreeItem * pSelItem = GetTreeItem(nSel);
	
        if ( IsItemNode( pSelItem ) )
        {
            nSelectedItem = GetData( pSelItem )->GetItemData();
        }
    }

    DeleteAll();

    //Show the project     
    CItemInfo* lp = new CItemInfo();
    lp->SetImage(4);              //Set Root item image;
    lp->SetItemText (m_bCustMode ? _T("Customer NV") : m_pProject->m_szPrjName);
    lp->AddSubItemText (m_pProject->m_szTargetFileName );
    lp->AddSubItemText (_T("Project"));
    lp->AddSubItemText ( m_pProject->m_szPrjDesc );
    lp->AddSubItemText ( m_pProject->m_szPrjFileName );
    lp->AddSubItemText (_T(""));
    lp->AddSubItemText (_T(""));
    
    m_pRootItem = InsertRootItem(lp);
    lp->SetItemType (STRUCT_ITEM_TYPE);


    //Insert Module
    int nModuleIndex;
    int nTotalModuleCount;
    
    nTotalModuleCount = m_pProject->GetModuleCount();
    
    for(nModuleIndex = 0; nModuleIndex < nTotalModuleCount; nModuleIndex++)
    {            
        AddModule (m_pRootItem, nModuleIndex);                        
    }

    ExpandProject();

//	SetProcessWorkingSetSize(AfxGetApp()->m_hInstance, -1, 1800000);

    if ( GetSelectedModuleIndex() != INVALID_MODULE_ID )
    {
// 		UINT nModuleItemIndex;
// 		UINT nTotalModuleItemCount;
// 		
// 		nTotalModuleItemCount = m_pProject->GetModuleItemCount(nModuleIndex);
		
		

        CTreeItem * pModuleNode = GetModuleNodeFromIndex( 
                                         GetSelectedModuleIndex() );


// 		for(nModuleItemIndex = 0; nModuleItemIndex < nTotalModuleItemCount; nModuleItemIndex++)
// 		{
// 			AddModuleItem(pModuleNode, GetSelectedModuleIndex(), &nModuleItemIndex, TRUE); 
// 		}

        if ( NULL != pModuleNode )
        {
            if ( -1 != nSelectedItem )
            {
                CTreeItem * pItemNode = GetItemNodeFromIndex(
                                        pModuleNode, nSelectedItem );
                if ( NULL != pItemNode )
				{
                    SelectNode( pItemNode );
				}
            }
            else
            {
                SelectNode( pModuleNode );
            }
        }   
    }
}

void CMySuperGrid::AddModule(CTreeItem *htPriItem, UINT nModuleIndex)
{
    
    Module tmpModule = m_pProject->GetModule(nModuleIndex);
	if (m_bCustMode && !tmpModule.bCustBase)
	{
		return;
	}
    
    CItemInfo* lpItemInfo = new CItemInfo();

    lpItemInfo->SetItemText ( tmpModule.szModuleName  );
    lpItemInfo->AddSubItemText ( tmpModule.szModuleFileName  );
    lpItemInfo->AddSubItemText (_T("Module" ));
	lpItemInfo->AddSubItemText ( tmpModule.szModuleDescription );
    lpItemInfo->AddSubItemText (_T(""));    
    lpItemInfo->AddSubItemText (_T(""));
    lpItemInfo->AddSubItemText (_T(""));
    lpItemInfo->SetItemData (nModuleIndex);
    lpItemInfo->SetItemType (STRUCT_ITEM_TYPE);

	if (tmpModule.bLoadFailed)
	{
		lpItemInfo->SetTextClr(RGB(189,189,0));
	}

    CTreeItem *htModuleItem = InsertItem (htPriItem ,lpItemInfo);    

    //Insert Module Item  delete by Joly 5/26/2015
    UINT nModuleItemIndex;
    UINT nTotalModuleItemCount;
    
    nTotalModuleItemCount = m_pProject->GetModuleItemCount(nModuleIndex);
    
    for(nModuleItemIndex = 0; nModuleItemIndex < nTotalModuleItemCount; nModuleItemIndex++)
    {   	
        AddModuleItem (htModuleItem, nModuleIndex, &nModuleItemIndex, TRUE);  
// 		CItemInfo* lpItemInfoTmp = NULL;
// 		InsertItem (htModuleItem, lpItemInfoTmp);  
    }

}

void CMySuperGrid::AddModuleItem(CTreeItem *htParentItem, UINT nModuleIndex, UINT *nModuleItemIndex, BOOL bFirstItem,BOOL bCustItems /*= FALSE*/)
{    
    int nItemIndex;
    int nTotalModuleItemCount;
    int nParentIndex;
    TCHAR szItemId[12];
	TCHAR szContent[MAX_ITEM_CONTENT] = {0};

	USES_CONVERSION;
    nParentIndex = (GetData(htParentItem))->GetItemData();

    nTotalModuleItemCount = m_pProject->GetModuleItemCount(nModuleIndex);
    for(nItemIndex = *nModuleItemIndex; nItemIndex < nTotalModuleItemCount; nItemIndex++)
    {
        ModuleItem tmpModuleItem = m_pProject->GetModuleItem(nModuleIndex, nItemIndex); 

        if ( (FALSE == bFirstItem) &&
            (INVALID_MODULE_ID == tmpModuleItem.nParentItemIndex) )             
        {
            *nModuleItemIndex = nItemIndex - 1;
            return;
        }

        else if((INVALID_MODULE_ID != tmpModuleItem.nParentItemIndex) &&
                (0 < nParentIndex) &&
               ((int)tmpModuleItem.nParentItemIndex < nParentIndex))            
        {
			if( m_bCustMode && !bCustItems )
			{
				continue;
			}
			else
			{
				*nModuleItemIndex = nItemIndex - 1;
			}
            return;
        }        
        else
        {
			if ( m_bCustMode && !tmpModuleItem.bCustItem && !tmpModuleItem.bCustBase )
			{				
				continue;
			}
            CItemInfo* lpItemInfo = new CItemInfo();
			memset(szContent,sizeof(szContent),0);	
			CString strContent = tmpModuleItem.szItemContent;
			if(!strContent.IsEmpty() && 
				(tmpModuleItem.eItemType == CHAR_ITEM_TYPE ||
				tmpModuleItem.eItemType == SHORT_ITEM_TYPE ||
				tmpModuleItem.eItemType == LONG_ITEM_TYPE))
			{
				unsigned int nValue;
				strContent.MakeUpper();
				if(strContent.Find(_T("0X"))==0)
				{
					_stscanf(strContent.operator LPCTSTR(),_T("0X%X"),&nValue);
				}
				else
				{
					_stscanf(strContent.operator LPCTSTR(),_T("%d"),&nValue);
				}
				
				if(m_bDispHex)
				{
					_stprintf(szContent,_T("0x%X"),nValue);
				}
				else
				{
					_stprintf(szContent,_T("%u"),nValue);
				}
			}
			else
			{
				_tcscpy(szContent,tmpModuleItem.szItemContent);
			}
            
            lpItemInfo->SetItemText ( tmpModuleItem.szItemName );
            lpItemInfo->AddSubItemText (szContent);			
            lpItemInfo->AddSubItemText (pItemTypeText[tmpModuleItem.eItemType]);
            lpItemInfo->AddSubItemText (tmpModuleItem.szItemDescription);     

            // Add prefix "0x" to item id
            _tcscpy( szItemId,_T("0x") );
            _itot(tmpModuleItem.nItemID, szItemId+2, 16);
            lpItemInfo->AddSubItemText ( szItemId );
            
            lpItemInfo->AddSubItemText (_T("Item"));
            lpItemInfo->AddSubItemText (_T("Inactive"));            
            lpItemInfo->SetItemData (nItemIndex);
            lpItemInfo->SetItemType (tmpModuleItem.eItemType);
            
            
            if( tmpModuleItem.bChanged )
            {
				if(tmpModuleItem.bReadOnly)
				{
					lpItemInfo->SetTextClr( RGB(255,0,255) );	
				}
				else
				{
					lpItemInfo->SetTextClr( RGB(255,0,0) );	
				}
            }
            else
            {
				if (tmpModuleItem.bLoadFailed)
				{
					lpItemInfo->SetTextClr(RGB(189,189,0));
				}
				else if(tmpModuleItem.bReadOnly)
				{
					lpItemInfo->SetTextClr( GetSysColor (COLOR_GRAYTEXT) );
				}				
				else
				{					
					lpItemInfo->SetTextClr( GetSysColor (COLOR_WINDOWTEXT) );	
				}
            }
			
            CTreeItem *htItem = InsertItem (htParentItem ,lpItemInfo);     

			SetItemChanged(htItem,tmpModuleItem.bChanged);//|tmpModuleItem.bLoadFailed);  // add by Joly
			SetCustItem(htItem, tmpModuleItem.bCustItem);
			SetCustBaseItem(htItem, tmpModuleItem.bCustBase);
			
            if( (STRUCT_ITEM_TYPE == tmpModuleItem.eItemType ) ||
                (ARRAY_ITEM_TYPE == tmpModuleItem.eItemType ) ||
				(ASTRING_ITEM_TYPE == tmpModuleItem.eItemType ) ||
				(WSTRING_ITEM_TYPE == tmpModuleItem.eItemType ))
            {
                 nItemIndex++;

                 AddModuleItem(htItem, nModuleIndex, (UINT *)&nItemIndex, FALSE,m_bCustMode ? TRUE : FALSE);
				 if(ASTRING_ITEM_TYPE == tmpModuleItem.eItemType )
				 {
					int nChild = NumChildren(htItem);
					if(nChild> 0)
					{
						ModuleItem mi = m_pProject->GetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData());
						mi.bChanged = FALSE;
						char * pBuf = new char[nChild+1];
						memset(pBuf,0,nChild+1);
						POSITION pos = GetHeadPosition(htItem);
						int i = 0;
						while(pos)
						{
							CTreeItem *htSubItem = GetNextChild(htItem,pos);
							CString str = GetData(htSubItem)->GetSubItem(CONTENT_COL-1);
							int nValue = 0;
							str.MakeUpper();
							if(str.Find(_T("0X"))==0)
							{
								_stscanf(str.operator LPCTSTR(),_T("0X%X"),&nValue);
							}
							else
							{
								_stscanf(str.operator LPCTSTR(),_T("%d"),&nValue);
							}

							pBuf[i++]= (char)nValue;

							ModuleItem miSub = m_pProject->GetModuleItem(nModuleIndex,(GetData(htSubItem))->GetItemData());
							
							if(miSub.bChanged)
							{
								mi.bChanged = TRUE;
							}
						}
				 		CString strParent = A2W(pBuf);
						GetData(htItem)->SetSubItemText(CONTENT_COL-1,strParent);
						_tcscpy(mi.szItemContent,strParent.operator LPCTSTR());

						if(mi.bChanged)
						{							
							if(mi.bReadOnly)
							{
								GetData(htItem)->SetTextClr( RGB(255,0,255) );	
							}
							else
							{
								GetData(htItem)->SetTextClr( RGB(255,0,0) );
							}

							GetData(htItem)->SetChanged(TRUE);
						}
						else
						{
							if (tmpModuleItem.bLoadFailed)
							{
								lpItemInfo->SetTextClr(RGB(189,189,0));
							}
							else if(mi.bReadOnly)
							{						
								GetData(htItem)->SetTextClr( GetSysColor (COLOR_GRAYTEXT) );
							}										
							else
							{							
								GetData(htItem)->SetTextClr( GetSysColor (COLOR_WINDOWTEXT) );
							}
							
							GetData(htItem)->SetChanged(FALSE);
						}
						
						// update parent item length and size
						mi.nArraySize = nChild;
						mi.nItemLength = nChild;

						m_pProject->SetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData(),mi);

						delete [] pBuf;
						pBuf = 0;		
					}
					else
					{
						ModuleItem mi = m_pProject->GetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData());
						// update parent item length and size
						mi.nArraySize = 0;
						mi.nItemLength = 0;
						m_pProject->SetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData(),mi);
					}
				 }
				 else if(WSTRING_ITEM_TYPE == tmpModuleItem.eItemType )
				 {
					int nChild = NumChildren(htItem);
					if(nChild> 0)
					{
						ModuleItem mi = m_pProject->GetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData());
						mi.bChanged = FALSE;

						wchar_t * pBuf = new wchar_t[nChild+1];
						memset(pBuf,0,(nChild+1)*sizeof(wchar_t));
						POSITION pos = GetHeadPosition(htItem);
						int i = 0;
						while(pos)
						{
							CTreeItem *htSubItem = GetNextChild(htItem,pos);
							CString str = GetData(htSubItem)->GetSubItem(CONTENT_COL-1);
							int nValue = 0;
							str.MakeUpper();
							if(str.Find(_T("0X"))==0)
							{
								_stscanf(str.operator LPCTSTR(),_T("0X%X"),&nValue);
							}
							else
							{
								_stscanf(str.operator LPCTSTR(),_T("%d"),&nValue);
							}

							pBuf[i++]= (wchar_t)nValue;

							ModuleItem miSub = m_pProject->GetModuleItem(nModuleIndex,(GetData(htSubItem))->GetItemData());
							
							if(miSub.bChanged)
							{
								mi.bChanged = TRUE;
							}
						}
						CString strParent = pBuf;
						GetData(htItem)->SetSubItemText(CONTENT_COL-1,strParent);

						_tcscpy(mi.szItemContent,strParent.operator LPCTSTR());

						if(mi.bChanged)
						{
							if(mi.bReadOnly)
							{
								GetData(htItem)->SetTextClr( RGB(255,0,255) );	
							}
							else
							{
								GetData(htItem)->SetTextClr( RGB(255,0,0) );
							}
							GetData(htItem)->SetChanged(TRUE);
						}
						else
						{
							if(mi.bReadOnly)
							{						
								GetData(htItem)->SetTextClr( GetSysColor (COLOR_GRAYTEXT) );
							}
							else
							{							
								GetData(htItem)->SetTextClr( GetSysColor (COLOR_WINDOWTEXT) );
							}
							GetData(htItem)->SetChanged(FALSE);
						}

						// update parent item length and size
						mi.nArraySize = nChild;
						mi.nItemLength = nChild*2;

						m_pProject->SetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData(),mi);

						delete [] pBuf;
						pBuf = 0;	
					}
					else
					{
						ModuleItem mi = m_pProject->GetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData());
						// update parent item length and size
						mi.nArraySize = 0;
						mi.nItemLength = 0;
						m_pProject->SetModuleItem(nModuleIndex,(GetData(htItem))->GetItemData(),mi);
					}
				 }
            }
        }
    }

   *nModuleItemIndex = nItemIndex - 1;
}

INT CMySuperGrid::GetSelectedModuleIndex()
{
    if(m_bModuleSelected)
    {
      return m_nSelectedModuleIndex;
    }
    else
    {
      return INVALID_MODULE_ID;
    }
}

Module_Ptr CMySuperGrid::GetSelectedModule()
{
    if(m_bModuleSelected)
    {
      return &m_hSelectedModule;
    }
    else
    {
      return NULL;
    }
}

BOOL CMySuperGrid::IsSelectedModule()
{
    return m_bModuleSelected;
}

void CMySuperGrid::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
    /*
    int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
    if(0 > iItem)
    {
        m_bModuleSelected = FALSE;
        return;
    }
    */

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if (pNMListView->uNewState & LVIS_SELECTED)
    {
        /*
        POSITION pos = GetFirstSelectedItemPosition();
        if (pos == NULL)
        {
           m_bModuleSelected = FALSE;
           return;
        }
    
        int iItem = GetNextSelectedItem(pos);    
        */

        int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
        if(0 > iItem)
        {
            m_bModuleSelected = FALSE;
            return;
        }

        CTreeItem *htSelectedItem = GetTreeItem(iItem);     
        

        if((NULL != htSelectedItem ) && (GetParentItem(htSelectedItem) != NULL))
        {
            CTreeItem *htParentItem = GetParentItem(htSelectedItem);

            if(htParentItem == m_pRootItem)
            {
                OnModuleSelchanged((GetData(htSelectedItem))->GetItemData());
            }
            else
            {            
                while(GetParentItem(htParentItem) != m_pRootItem)
                {
                   htParentItem = GetParentItem(htParentItem);
                }

                OnModuleSelchanged((GetData(htParentItem))->GetItemData());
                OnItemSelchanged((GetData(htSelectedItem))->GetItemData());
            }

            m_bModuleSelected = TRUE;
        }    
        else
        {
            m_bModuleSelected = FALSE;
        }
    }

    * pResult = 0;
}

void CMySuperGrid::OnModuleSelchanged(UINT nSelectModuleIndex)
{    
    m_nSelectedModuleIndex = nSelectModuleIndex;
    m_hSelectedModule = m_pProject->GetModule(m_nSelectedModuleIndex);
}

void CMySuperGrid::OnItemSelchanged(UINT nSelectItemIndex)
{
    m_nSelectedItemIndex = nSelectItemIndex;
    m_hSelectedItem = m_pProject->GetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex);
}

#define IDC_EDITCTRL 0x1234
CEdit* CMySuperGrid::EditLabelEx(int nItem, int nCol)
{
	CRect rect;
	int offset = 0;
	if(!EnsureVisible(nItem, TRUE)) 
		return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size(offset + rect.left,0);		
		Scroll(size);
		rect.left -= size.cx;
	}
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;

	// Get Column alignment	
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);

	DWORD dwStyle;
	if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;	

	//YOU MAY WANNA COMMENT THIS OUT,YOU DECIDE ..
	//now you could take into account here, that an subitem might have an icon
	CTreeItem *p = GetTreeItem(nItem);
	if(p!=NULL)
	{
		CItemInfo *pInfo = GetData(p);
		if(pInfo!=NULL)
		{
// 			int iSubImage = pInfo->GetSubItemImage(nCol-1); 
// 			if(iSubImage!=-1)
// 			{
// 				//m_cxImage is actually the width of the "tree" imagelist not your subitem imagelist..
// 				//remember that, when you create your bitmap, I was to lazy getting the icon size of the subitem imagelist
// 				rect.left+=m_cxImage;
//			}
		}
	}
	//////////////////////////////////
	dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	CEdit *pEdit;
    if(nCol == CONTENT_COL && m_hSelectedItem.eItemType != ASTRING_ITEM_TYPE && m_hSelectedItem.eItemType != WSTRING_ITEM_TYPE)
        pEdit = new CListEditCtrl(nItem, nCol, GetItemText(nItem, nCol));
    else
        pEdit = new CListEditCtrlLite(nItem,nCol,GetItemText(nItem,nCol));

    ((CListEditCtrl *)pEdit)->SetItemType(m_hSelectedItem.eItemType,m_hSelectedItem.nItemLength);

	pEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);	
	//pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE); //funny thing happend here, uncomment this, 
												//and then edit an item, 
												//enter a long text so that the ES_AUTOHSCROLL comes to rescue
												//yes that's look funny, ???.
	return pEdit;
}


void CMySuperGrid::OnInsertAfter() 
{
    int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;

    CTreeItem * pSelItem = GetTreeItem(nSel);

	ASSERT( !IsModuleNode( pSelItem ) && !IsProjectNode( pSelItem ) );
    CTreeItem * pParentItem = GetParentItem(pSelItem);

    ModuleItem miNewItem;
	ModuleItem miArrayItem;
        
    // Show a dialog to get item info
    if ( !RetrieveItemInfo( pParentItem,  &miNewItem, &miArrayItem ) )
		return;

	// The new item is inserted behind the selected item, namely the last
	// subitem of the selected one.
	pSelItem = GetLastItemOfTree( pSelItem );
	InsertModuleItem( pParentItem, 
		              GetData( pSelItem )->GetItemData(),
					  &miNewItem,
					  &miArrayItem );

    Refresh();
}


void CMySuperGrid::OnInsertBefore() 
{
    int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
        
	CTreeItem * pSelItem = GetTreeItem( nSel );
	ASSERT( !IsModuleNode( pSelItem ) && !IsProjectNode( pSelItem ) );
    CTreeItem * pParentItem = GetParentItem( pSelItem );

	ModuleItem miItem;
	ModuleItem miArrayItem;
        
    // Show a dialog to get item info
    if ( !RetrieveItemInfo( pParentItem,  &miItem, &miArrayItem ) )
		return;
      

	BOOL fAfter = TRUE;
	int nIndex;

	// Get the previous sibling item of the selected one.
	pSelItem = GetPrev( pParentItem, pSelItem, TRUE, FALSE );
    if ( pParentItem == pSelItem )
    {
		// No previous sibling item.
        if ( IsModuleNode( pParentItem ) )
        {
			// Inserted as the first item of a module.
            nIndex = 0;
			fAfter = FALSE;
        }
        else
        {
			// Inserted as the first subitem of the parent item.
			// And its position is just behind the parent item.
            nIndex = GetData( pParentItem )->GetItemData();
        }      
    }
    else
    {
		// The new item is behind the previous sibling item of the
		// select one.
        nIndex = GetData( GetLastItemOfTree( pSelItem ) )->GetItemData();     
    }

    InsertModuleItem( pParentItem, nIndex, &miItem, &miArrayItem, fAfter );
	
	Refresh();
}

void CMySuperGrid::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNREFERENCED_PARAMETER(pWnd);

    int nIndex = GetSelectedItem();
    if ( -1 == nIndex )
        return;

    CTreeItem * pSelItem = GetTreeItem( nIndex );
    ASSERT( NULL != pSelItem );

    if ( /*IsItemNode( pSelItem )*/ m_pProject != NULL && pSelItem != NULL)
    {
        // Now only item node has pop menu.
        CMenu menu;
        CMenu * pSubMenu;

        VERIFY( menu.LoadMenu( IDM_INSERT_POP ) );
        pSubMenu = menu.GetSubMenu( 0 );
        ASSERT( NULL != pSubMenu );

        ASSERT( NULL != m_pProject );
       // ASSERT( GetSelectedModuleIndex() != INVALID_MODULE_ID );
        ModuleItem item = m_pProject->GetModuleItem( GetSelectedModuleIndex(),
                                                     GET_ITEM_INDEX( pSelItem ) );

        if ( ( ARRAY_ITEM_TYPE != item.eItemType ) 
            && ( STRUCT_ITEM_TYPE != item.eItemType ) )
        {
            pSubMenu->EnableMenuItem( ID_INSERT_AS_CHILD, 
                                      MF_BYCOMMAND | MF_GRAYED );
        }

        if( !m_PasteInfo.bValid )
        {
            pSubMenu->EnableMenuItem( ID_PASTE_AFTER,MF_BYCOMMAND | MF_GRAYED );
            pSubMenu->EnableMenuItem( ID_PASTE_AS_CHILD,MF_BYCOMMAND | MF_GRAYED );
        } 

		if(!IsItemNode(pSelItem))
		{
			pSubMenu->EnableMenuItem( ID_INSERT_BEFORE,MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_INSERT_AFTER, MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_INSERT_AS_CHILD,MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_REMOVE_ITEM, MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_CUT_ITEM, MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_COPY_ITEM, MF_BYCOMMAND | MF_GRAYED );
			pSubMenu->EnableMenuItem( ID_PASTE_AFTER,MF_BYCOMMAND | MF_GRAYED );
            pSubMenu->EnableMenuItem( ID_PASTE_AS_CHILD,MF_BYCOMMAND | MF_GRAYED );
		}

		CItemInfo *lp = GetData(pSelItem);
		if(lp && (lp->IsCustItem() || !lp->IsCustBaseItem()))
		{
			pSubMenu->EnableMenuItem( ID_MARK_AS_CUSTNV, MF_BYCOMMAND | MF_GRAYED );
		}
		
		if (lp && !lp->IsCustItem())
		{
			pSubMenu->EnableMenuItem( ID_UNMARK_AS_CUSTNV, MF_BYCOMMAND | MF_GRAYED );
		}

		CTreeItem* lpRootItem = GetTreeItem(0);	
		if (!HasCustNVItem(lpRootItem))
		{
			pSubMenu->EnableMenuItem( ID_UNMARK_ALL_CUSTNV, MF_BYCOMMAND | MF_GRAYED );
		}
		if (!HasChangeItem(lpRootItem))
		{
			pSubMenu->EnableMenuItem( ID_MARK_ALL_CHANGE_AS_CUSTNV, MF_BYCOMMAND | MF_GRAYED );
			
		}
		if (!HasChangeItem(lpRootItem,TRUE))
		{
			pSubMenu->EnableMenuItem( ID_UNMARK_ALL_CHANGE_AS_CUSTNV, MF_BYCOMMAND | MF_GRAYED );
		}

		if (!m_bModifyID || INVALID_MODULE_ID != item.nParentItemIndex || !IsItemNode(pSelItem))
		{
			pSubMenu->EnableMenuItem(ID_MODIFY_ITEMID, MF_BYCOMMAND | MF_GRAYED);
		}

		if (m_bCustMode)
		{	
			pSubMenu->DeleteMenu( 21,MF_BYPOSITION );
			pSubMenu->DeleteMenu( 11,MF_BYPOSITION );
			pSubMenu->DeleteMenu( 6,MF_BYPOSITION );
			pSubMenu->DeleteMenu( 4,MF_BYPOSITION );
			pSubMenu->DeleteMenu( 2,MF_BYPOSITION );
			pSubMenu->DeleteMenu( ID_INSERT_BEFORE,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_INSERT_BEFORE,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_INSERT_AFTER, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_INSERT_AS_CHILD,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_REMOVE_ITEM, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_CUT_ITEM, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_COPY_ITEM, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_PASTE_AS_CHILD, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_PASTE_AFTER, MF_BYCOMMAND );
			
            pSubMenu->DeleteMenu( ID_MODIFY_ITEMID,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_MARK_AS_CUSTBASE,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_UNMARK_AS_CUSTBASE,MF_BYCOMMAND );
		}
		else if (SUPER_PERMISSION != theApp.GetPermission())
		{
			pSubMenu->DeleteMenu( 21,MF_BYPOSITION );
// 			pSubMenu->DeleteMenu( 14,MF_BYPOSITION );
// 			pSubMenu->DeleteMenu( ID_MARK_AS_CUSTNV,MF_BYCOMMAND );
// 			pSubMenu->DeleteMenu( ID_UNMARK_AS_CUSTNV,MF_BYCOMMAND );
// 			pSubMenu->DeleteMenu( ID_UNMARK_ALL_CUSTNV, MF_BYCOMMAND );
// 			pSubMenu->DeleteMenu( ID_MARK_ALL_CHANGE_AS_CUSTNV,MF_BYCOMMAND );
// 			pSubMenu->DeleteMenu( ID_UNMARK_ALL_CHANGE_AS_CUSTNV, MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_MARK_AS_CUSTBASE,MF_BYCOMMAND );
			pSubMenu->DeleteMenu( ID_UNMARK_AS_CUSTBASE,MF_BYCOMMAND );
		}
		else if (SUPER_PERMISSION == theApp.GetPermission())
		{
			if( IsProjectNode(pSelItem) || ChildNotHasCustBase(pSelItem))
			{
				pSubMenu->EnableMenuItem( ID_MARK_AS_CUSTBASE, MF_BYCOMMAND | MF_GRAYED );
			}
			if ( IsProjectNode(pSelItem) || !HasCustBaseNV(pSelItem) )
			{
				pSubMenu->EnableMenuItem( ID_UNMARK_AS_CUSTBASE, MF_BYCOMMAND | MF_GRAYED );
			}
			
		}

		pSubMenu->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this );

    }

	
}

void CMySuperGrid::OnInsertAsChild() 
{
    int nSel = GetSelectedItem();    
	if ( -1 == nSel )
		return;

	CTreeItem * pParentNode = GetTreeItem( nSel );
	ASSERT( !IsProjectNode( pParentNode ) && !IsModuleNode( pParentNode ) );

	ModuleItem miItem;
	ModuleItem miArrayItem;

	if ( !RetrieveItemInfo( pParentNode, &miItem, &miArrayItem ) )
		return;

	//////////////////////////////////////////////////////////////////////////
	// ???????
	int nIndex = GetData( pParentNode )->GetItemData();	
    if( !IsModuleNode( pParentNode ) )
    {
		ModuleItem miParent;
        miParent = m_pProject->GetModuleItem( GetSelectedModuleIndex(),nIndex );    
		// ?????/??,?????/?????????
		if ( ARRAY_ITEM_TYPE == miParent.eItemType || STRUCT_ITEM_TYPE == miParent.eItemType )
		{
			CTreeItem *pLastNode = GetLastItemOfTree( pParentNode );
			if(pLastNode != NULL)
			{
				ModuleItem miLast = m_pProject->GetModuleItem(
												   GetSelectedModuleIndex(),
												   GetData( pLastNode )->GetItemData() );
				nIndex = miLast.nItemIndex;
			}
		}
	}

	// The new subitem is inserted just behind the parent node.
	InsertModuleItem( pParentNode, 
		              nIndex,
					  &miItem,
					  &miArrayItem );
 
    Refresh();
}

void CMySuperGrid::OnRemoveItem() 
{
    int nRet = AfxMessageBox(IDS_REMOVE_ITEM,MB_OKCANCEL);
    if(nRet != IDOK)
        return;

    int nIndex = GetSelectedItem();
    if(nIndex!=-1)
    {
        CTreeItem *pSelItem = GetTreeItem(nIndex);
        RemoveItem(pSelItem,TRUE);
    }
}
    
void CMySuperGrid::RemoveItem(CTreeItem* pItem,BOOL bFresh)
{
    ASSERT(pItem != NULL);

    if(!ItemHasChildren(pItem))
    {
        RemoveLeafItem(pItem);
    }
    else
    {
        POSITION pos = GetTailPosition(pItem);
        while(pos != NULL)
        {
            CTreeItem* pCurItem = GetPrevChild(pItem,pos);
            RemoveItem(pCurItem,FALSE);
        }

        RemoveLeafItem(pItem);
    }

    if(bFresh)
        Refresh();
}
    
// Remove the leaf node of the tree list
void CMySuperGrid::RemoveLeafItem(CTreeItem* pItem)
{
    ASSERT(pItem != NULL);
    int nIndex = GetData(pItem)->GetItemData();
    m_pProject->RemoveModuleItem(m_nSelectedModuleIndex,nIndex);
}


// Function:
//      ????ITEMTYPE???
// 
// Discription:  add by wei anli 03.06.12
// -----------------------------------------------------------------------------
BOOL CMySuperGrid::GetItemTypeName( // ????, TRUE;??,FALSE
    CString &strTypeName,           // [OUT] ?????,???????
    ITEMTYPE it                     // [IN]  ??(ENUM?)
)
{
    if(it == CHAR_ITEM_TYPE)
    {  
        strTypeName = _T("CHAR");
    }
    else if(it == SHORT_ITEM_TYPE)
    {
        strTypeName = _T("SHORT");
    }
    else if(LONG_ITEM_TYPE == it)
    {
        strTypeName = _T("LONG");
    }
    else if(ARRAY_ITEM_TYPE == it)
    {
        strTypeName = _T("ARRAY");
    }
    else if(ASTRING_ITEM_TYPE == it)
    {
        strTypeName = _T("ASTRING");
    }
	else if(WSTRING_ITEM_TYPE == it)
    {
        strTypeName = _T("WSTRING");
    }
    else if(STRUCT_ITEM_TYPE == it)
    {
        strTypeName = _T("STRUCT");
    }
    else
    {
        strTypeName = _T(""); 
        return FALSE;
    }

    return TRUE;
}

void CMySuperGrid::OnSetReadonly() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if(nSel != -1)
    {
        CTreeItem * pSelItem = GetTreeItem( nSel );
        int nIndex = GetData( pSelItem )->GetItemData();

        int nCurSelMdl = GetSelectedModuleIndex();
        ModuleItem miItem = m_pProject->GetModuleItem(nCurSelMdl, 
                                          nIndex );

		miItem.bReadOnly = TRUE;

		m_pProject->SetModuleItem( nCurSelMdl, nIndex, miItem );
		int nCount = m_pProject->GetModuleItemCount( nCurSelMdl );

		for( int i=nIndex+1; i<nCount; i++ )
		{
			ModuleItem miItem = m_pProject->GetModuleItem( nCurSelMdl,
				                                           i );
			if ( miItem.nParentItemIndex < nIndex )
			{
				break;
			}
			miItem.bReadOnly = TRUE;
			m_pProject->SetModuleItem( nCurSelMdl, i, miItem );
		}
	}
}

void CMySuperGrid::OnEraseReadonly() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if(nSel != -1)
    {
        CTreeItem * pSelItem = GetTreeItem(nSel);
        int nIndex = GetData(pSelItem)->GetItemData();

        int nCurSelMdl = GetSelectedModuleIndex();
        ModuleItem miItem = m_pProject->GetModuleItem(nCurSelMdl, 
                                          nIndex );

		miItem.bReadOnly = FALSE;

		m_pProject->SetModuleItem( nCurSelMdl, nIndex, miItem );
		int nCount = m_pProject->GetModuleItemCount( nCurSelMdl );

		for( int i=nIndex+1; i<nCount; i++ )
		{
			ModuleItem miItem = m_pProject->GetModuleItem( nCurSelMdl,
				                                           i );
			if ( miItem.nParentItemIndex < nIndex )
			{
				break;
			}
			miItem.bReadOnly = FALSE;
			m_pProject->SetModuleItem( nCurSelMdl, i, miItem );
		}
	}	
}

void CMySuperGrid::OnSetAppendable() 
{
	// TODO: Add your command handler code here
	int nCurSelMdl = GetSelectedModuleIndex();
	
	m_pProject->SetModuleAppendable( nCurSelMdl, TRUE );
}

void CMySuperGrid::OnEraseAppendable() 
{
	// TODO: Add your command handler code here
	int nCurSelMdl = GetSelectedModuleIndex();

	m_pProject->SetModuleAppendable( nCurSelMdl, FALSE );
	
}

void CMySuperGrid::OnModuleErasereadonly() 
{
	// TODO: Add your command handler code here
	int nCurSelMdl = GetSelectedModuleIndex();
	if ( nCurSelMdl == -1 )
	{
		return;
	}

	int nCount = m_pProject->GetModuleItemCount( nCurSelMdl );

	for ( int i=0; i<nCount; i++ )
	{
		ModuleItem miItem = m_pProject->GetModuleItem( nCurSelMdl,
			i );

		miItem.bReadOnly = FALSE;

		m_pProject->SetModuleItem( nCurSelMdl, i, miItem );
	}
}

void CMySuperGrid::OnModuleSetReadonly() 
{
	// TODO: Add your command handler code here
	int nCurSelMdl = GetSelectedModuleIndex();
	if ( -1 == nCurSelMdl )
	{
		return;
	}

	int nCount = m_pProject->GetModuleItemCount( nCurSelMdl );
	for ( int i=0; i<nCount; i++ )
	{
		ModuleItem miItem = m_pProject->GetModuleItem( nCurSelMdl,
			i );
		miItem.bReadOnly = TRUE;
		m_pProject->SetModuleItem( nCurSelMdl, i, miItem );
	}
}


BOOL CMySuperGrid::RetrieveItemInfo( CTreeItem  * pParentNode, 
		                             ModuleItem * pNewItem   ,
						             ModuleItem * pArrayItem  )
{
	ASSERT( NULL != pParentNode );
	ASSERT( !IsProjectNode( pParentNode ) );
	ASSERT( NULL != pNewItem );
	ASSERT( NULL != pArrayItem );

	ASSERT( NULL != m_pProject );

	CNewItemInfo dlg;
	ModuleItem miParent;
    if( IsModuleNode( pParentNode ) )
    {
        miParent.eItemType = NONE_ITEM_TYPE;
    }
    else
    {
        miParent = m_pProject->GetModuleItem( 
		                                   GetSelectedModuleIndex(),
		                                 GetData( pParentNode )->GetItemData() );
    }

	ASSERT( IsModuleNode( pParentNode )
		|| ( STRUCT_ITEM_TYPE == miParent.eItemType )
		|| ( ARRAY_ITEM_TYPE == miParent.eItemType ) );

	if ( ARRAY_ITEM_TYPE == miParent.eItemType )
	{
		ASSERT( ItemHasChildren( pParentNode ) );

		POSITION pos = GetHeadPosition( pParentNode );
        CTreeItem *pChildNode = GetNextChild( pParentNode, pos );
        ModuleItem miChild = m_pProject->GetModuleItem(
			                               GetSelectedModuleIndex(),
                                           GetData( pChildNode )->GetItemData() );

        CString strType;
        GetItemTypeName( strType, miChild.eItemType );
        dlg.AddItemType( strType );

		if( ASTRING_ITEM_TYPE == miChild.eItemType ||
			WSTRING_ITEM_TYPE == miChild.eItemType)
		{	
			//???
			dlg.m_nStrLen = miChild.nArraySize;
		}

		dlg.m_strName = miChild.szItemName;
		int nFind = dlg.m_strName.ReverseFind(_T('['));

		if(nFind != -1)
		{
			dlg.m_strName = dlg.m_strName.Left(nFind);
		}

		CString strTmp;
		strTmp.Format(_T("[%d]"),NumChildren(pParentNode));
		dlg.m_strName += strTmp;		

	}
	else
	{            
        dlg.SetItemFullType();
	}

	if ( IsModuleNode( pParentNode ) )
	{
		dlg.m_fNeedID = TRUE;
	}
	else
	{
		// Parent node is struct or array, so the ID of the new subitem
		// must be the same as that of the parent.
		dlg.m_fNeedID = FALSE;
		dlg.m_nItemID = miParent.nItemID;
		// We assign the ID of the parent to the m_nItemID field of the
		// dialog to ease subsequent process.
	}

	if ( IDCANCEL == dlg.DoModal() )
		return FALSE;

	// Guarantee that if no ID is needed, the dialog does not change
	// the m_nItemID field we have assigned.
	ASSERT( ( dlg.m_fNeedID ) || ( dlg.m_nItemID == miParent.nItemID ) );

    _tcscpy( pNewItem->szItemName, dlg.m_strName );
	int nLen = dlg.m_strDesc.GetLength() + 1;
	
	if (nLen > 1)
	{	
		pNewItem->szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
		_tcscpy(pNewItem->szItemDescription, dlg.m_strDesc);
	}
	
   // _tcscpy( pNewItem->szItemDescription, dlg.m_strDesc );
    pNewItem->nItemID = dlg.m_nItemID; 
	pNewItem->szItemContent[ 0 ] = '\0';

	if ( dlg.m_fIsArray )
	{
		pNewItem->eItemType = ARRAY_ITEM_TYPE;

		pArrayItem->eItemType = ( ITEMTYPE_ENUM )dlg.m_nItemType;
		ASSERT( ( ARRAY_ITEM_TYPE != pArrayItem->eItemType ) 
			&& ( STRUCT_ITEM_TYPE != pArrayItem->eItemType ) );

		pArrayItem->nItemLength = dlg.m_nItemType;
		::_stprintf( pArrayItem->szItemContent, _T("%s"), dlg.m_strDefaultValue );
		/*pArrayItem->szItemDescription[ 0 ] = '\0';*/

		if(ASTRING_ITEM_TYPE == pArrayItem->eItemType)
		{
			pArrayItem->nArraySize = dlg.m_nStrLen;
			pArrayItem->nItemLength = dlg.m_nStrLen;
		}
		
		if(WSTRING_ITEM_TYPE == pArrayItem->eItemType)
		{
			pArrayItem->nArraySize = dlg.m_nStrLen;
			pArrayItem->nItemLength = dlg.m_nStrLen*2;
		}


		pNewItem->nArraySize = dlg.m_nArraySize;
		pNewItem->nItemLength = pNewItem->nArraySize * pArrayItem->nItemLength;
	}
	else
	{
		pNewItem->eItemType = ( ITEMTYPE_ENUM )dlg.m_nItemType;
		ASSERT( ARRAY_ITEM_TYPE != pNewItem->eItemType );
		pNewItem->nItemLength = pNewItem->eItemType;

		if(ASTRING_ITEM_TYPE == pNewItem->eItemType)
		{
			pNewItem->nArraySize = dlg.m_nStrLen;
			pNewItem->nItemLength = dlg.m_nStrLen;
		}
		
		if(WSTRING_ITEM_TYPE == pNewItem->eItemType)
		{
			pNewItem->nArraySize = dlg.m_nStrLen;
			pNewItem->nItemLength = dlg.m_nStrLen*2;
		}

		if ( STRUCT_ITEM_TYPE != pNewItem->eItemType )
		{
			::_stprintf( pNewItem->szItemContent, _T("%s"), dlg.m_strDefaultValue );
		}
	}

    return TRUE;
}

int CMySuperGrid::InsertStringItem( int                nIndex   ,
		                             const ModuleItem & miParent ,
						             ModuleItem &       miItem    )
{
	ASSERT( NULL != m_pProject );
	ASSERT( ASTRING_ITEM_TYPE == miParent.eItemType || WSTRING_ITEM_TYPE == miParent.eItemType);
		
	USES_CONVERSION;
	miItem.nItemID = miParent.nItemID;
	miItem.nParentItemIndex = nIndex;

	if(ASTRING_ITEM_TYPE == miParent.eItemType)
	{		
		miItem.eItemType = CHAR_ITEM_TYPE;
		miItem.nItemLength = (UINT)miItem.eItemType;
	}
	else if( WSTRING_ITEM_TYPE == miParent.eItemType )
	{
		miItem.eItemType = SHORT_ITEM_TYPE;
		miItem.nItemLength = (UINT)miItem.eItemType;
	}
	
	int nNewIndx = nIndex;

	CString strParent = miParent.szItemContent;
	char *pBuf =  W2A(strParent.operator LPCTSTR());
	CString strValue;

	for ( UINT i=0; i<miParent.nArraySize; i++ )
	{   
		_stprintf( miItem.szItemName , _T("%s[%d]"), miParent.szItemName, i );

		if(ASTRING_ITEM_TYPE == miParent.eItemType )
		 {		
			if(strlen(pBuf) > i)
			{
				strValue.Format(_T("0x%X"),(BYTE)pBuf[i]);
				_tcscpy(miItem.szItemContent,strValue.operator LPCTSTR());
			}
			else
			{
				_tcscpy(miItem.szItemContent,_T("0x0"));
			}
	
		 }
		 else if(WSTRING_ITEM_TYPE == miParent.eItemType )
		 {
			if(strParent.GetLength() > (int)i)
			{
				strValue.Format(_T("0x%X"),strParent.GetAt(i));
				_tcscpy(miItem.szItemContent,strValue.operator LPCTSTR());
			}
			else
			{
				_tcscpy(miItem.szItemContent,_T("0x0"));
			}
		 }
		nNewIndx = m_pProject->InsertModuleItemAllByOnce ( GetSelectedModuleIndex(), 
										        miItem, 
										        nNewIndx ); 
	}

    // modify by yph 
    /*CR229856 delete by xmzhang begin*/
    //m_pProject->InsertModuleItemAllByOnceHelper(nNewIndx, nIndex, miParent.nArraySize);
    /*CR229856 delete by xmzhang begin*/

	return nNewIndx;
}


void CMySuperGrid::InsertArrayItem( int                nIndex   ,
		                            const ModuleItem & miParent ,
						            ModuleItem &       miItem    )
{
	ASSERT( NULL != m_pProject );
	ASSERT( ARRAY_ITEM_TYPE == miParent.eItemType );
		
	miItem.nItemID = miParent.nItemID;
	miItem.nParentItemIndex = nIndex;

	for ( UINT i=0; i<miParent.nArraySize; i++ )
	{   
		_stprintf( miItem.szItemName , _T("%s[%d]"), miParent.szItemName, i );
		nIndex = m_pProject->InsertModuleItem ( GetSelectedModuleIndex(), 
										        miItem, 
										        nIndex ); 

        if( ASTRING_ITEM_TYPE == miItem.eItemType || WSTRING_ITEM_TYPE == miItem.eItemType)
		{
			ModuleItem miSubItem;			
			nIndex = InsertStringItem(nIndex,miItem,miSubItem);
		}

	}
}
int CMySuperGrid::InsertModuleItemEx(int nParentIndex,int nModuleIndex,int nItemIndex,ModuleItem * pNewItem,BOOL fAfter,BOOL bUpdateTree/* = FALSE*/)
{
	
	if ( nParentIndex == -1 )
	{
		pNewItem->nParentItemIndex = INVALID_MODULE_ID;
	}
	else
	{
		pNewItem->nParentItemIndex = nParentIndex;
		
		ModuleItem miParent = m_pProject->GetModuleItem(nModuleIndex,nParentIndex );
		
		if ( ARRAY_ITEM_TYPE == miParent.eItemType )
		{
			miParent.nArraySize++;
			m_pProject->SetModuleItem( nModuleIndex,nParentIndex,miParent );
		}
	}
	
    if ( ARRAY_ITEM_TYPE == pNewItem->eItemType || 
		ASTRING_ITEM_TYPE == pNewItem->eItemType ||
		WSTRING_ITEM_TYPE == pNewItem->eItemType)
    {
        // And we mark the new node as expanded
        pNewItem->bExpanded = TRUE;
    }
	
	int nIndex = m_pProject->InsertModuleItem( nModuleIndex , *pNewItem, nItemIndex,fAfter,bUpdateTree );

	return nIndex;
}

void CMySuperGrid::InsertModuleItem( CTreeItem  * pParentNode ,
						             int          nIndex      ,
						             ModuleItem * pNewItem    ,
						             ModuleItem * pArrayItem  ,
						             BOOL		  fAfter      ,
                                     BOOL         bExpandArray
                                   )
{
	ASSERT( NULL != pParentNode );
	ASSERT( !IsProjectNode( pParentNode ) );
	ASSERT( NULL != pNewItem );

	ASSERT( NULL != m_pProject );

	if ( IsModuleNode( pParentNode ) )
	{
		pNewItem->nParentItemIndex = INVALID_MODULE_ID;
	}
	else
	{
		pNewItem->nParentItemIndex = GetData( pParentNode )->GetItemData();

		ModuleItem miParent = m_pProject->GetModuleItem( 
		                                 GetSelectedModuleIndex(),
		                                 GET_ITEM_INDEX( pParentNode ) );
		
		if ( ARRAY_ITEM_TYPE == miParent.eItemType )
		{
			miParent.nArraySize++;
			m_pProject->SetModuleItem( GetSelectedModuleIndex(),
									   GET_ITEM_INDEX( pParentNode ),
                                       miParent );
		}
	}

    if ( ARRAY_ITEM_TYPE == pNewItem->eItemType || 
		 ASTRING_ITEM_TYPE == pNewItem->eItemType ||
		 WSTRING_ITEM_TYPE == pNewItem->eItemType)
    {
        // And we mark the new node as expanded
        pNewItem->bExpanded = TRUE;
    }

	nIndex = m_pProject->InsertModuleItem( GetSelectedModuleIndex() , 
		                                   *pNewItem                , 
										   nIndex                   ,
										   fAfter                   );
	if(  (ASTRING_ITEM_TYPE == pNewItem->eItemType ||
		  WSTRING_ITEM_TYPE == pNewItem->eItemType) && bExpandArray)
	{
		InsertStringItem(nIndex,*pNewItem, *pArrayItem);
	}

	if ( ARRAY_ITEM_TYPE == pNewItem->eItemType && bExpandArray)
	{
		ASSERT( NULL != pArrayItem );
		InsertArrayItem( nIndex, *pNewItem, *pArrayItem );    
	}
}

CMySuperGrid::CTreeItem * CMySuperGrid::GetLastItemOfTree( CTreeItem * pRoot )
{
	ASSERT( NULL != pRoot );

	while ( ItemHasChildren( pRoot ) )
	{
		POSITION pos = GetTailPosition( pRoot );
        pRoot = GetNextChild( pRoot, pos );
    }

	return pRoot;
}

BOOL CMySuperGrid::IsModuleNode( CTreeItem * pNode )
{
	ASSERT( NULL != pNode );
	return GetParentItem( pNode ) == m_pRootItem;
}

BOOL CMySuperGrid::IsProjectNode( CTreeItem * pNode )
{
	ASSERT( NULL != pNode );
    ASSERT( NULL != m_pRootItem );
	return pNode == m_pRootItem;
}

BOOL CMySuperGrid::IsItemNode( CTreeItem * pNode )
{
    ASSERT( NULL != pNode );
    return !IsProjectNode( pNode ) && !IsModuleNode( pNode );
}
		   
     
void CMySuperGrid::SetModuleExpanded( CTreeItem * pModuleNode, BOOL fExpanded )
{
    ASSERT( NULL != pModuleNode );
    ASSERT( IsModuleNode( pModuleNode ) );
    ASSERT( NULL != m_pProject );

    int nModuleIndex = GET_MODULE_INDEX( pModuleNode );
    Module module = m_pProject->GetModule( nModuleIndex );
    module.bExpanded = fExpanded;
    m_pProject->SetModule( nModuleIndex, module );
}
    
BOOL CMySuperGrid::IsModuleExpanded( CTreeItem * pModuleNode )
{
    ASSERT( NULL != pModuleNode );
    ASSERT( IsModuleNode( pModuleNode ) );
    ASSERT( NULL != m_pProject );

    Module module = m_pProject->GetModule( GET_MODULE_INDEX( pModuleNode ) );
    return module.bExpanded;
}

    
void CMySuperGrid::SetItemExpanded( int         nModuleIndex,
                                    CTreeItem * pItemNode, 
                                    BOOL        fExpanded )
{
    ASSERT( NULL != pItemNode );
    ASSERT( IsItemNode( pItemNode ) );
    ASSERT( NULL != m_pProject );

    int nItemIndex = GET_ITEM_INDEX( pItemNode );
    ModuleItem item = m_pProject->GetModuleItem( nModuleIndex, nItemIndex );
    item.bExpanded = fExpanded;
    m_pProject->SetModuleItem( nModuleIndex, nItemIndex, item, FALSE );
}

BOOL CMySuperGrid::IsItemExpanded( int nModuleIndex, CTreeItem *pItemNode )
{
    ASSERT( NULL != pItemNode );
    ASSERT( IsItemNode( pItemNode ) );
    ASSERT( NULL != m_pProject );

    ModuleItem item = m_pProject->GetModuleItem( nModuleIndex, 
                                                 GET_ITEM_INDEX( pItemNode ) );

    return item.bExpanded;
}

#define EXPAND_NODE( pNode )        Expand( ( pNode ), NodeToIndex( ( pNode ) ) )

void CMySuperGrid::ExpandProject( void )
{  
    ASSERT( NULL != m_pRootItem );

    EXPAND_NODE( m_pRootItem );
    POSITION pos = GetHeadPosition( m_pRootItem );
    while ( NULL != pos )
    {
        CTreeItem * pChildItem = GetNextChild( m_pRootItem, pos );
        ExpandModule( pChildItem );
    }
}


void CMySuperGrid::ExpandModule( CTreeItem *pModuleNode )
{
    ASSERT( NULL != pModuleNode );
    ASSERT( IsModuleNode( pModuleNode ) );

    if ( IsModuleExpanded( pModuleNode ) && ItemHasChildren( pModuleNode ) )
    {
        EXPAND_NODE( pModuleNode );

		POSITION pos = GetHeadPosition( pModuleNode );
        while ( NULL != pos )
        {
            CTreeItem * pChildItem = GetNextChild( pModuleNode, pos );
            ExpandItem( GET_MODULE_INDEX( pModuleNode ),
                        pChildItem );
        }
    }
}
    

void CMySuperGrid::ExpandItem( int nModuleIndex, CTreeItem *pItemNode )
{
    ASSERT( NULL != pItemNode );
    ASSERT( IsItemNode( pItemNode ) );

    if ( IsItemExpanded( nModuleIndex, pItemNode ) && ItemHasChildren( pItemNode ) )
    {
        EXPAND_NODE( pItemNode );

		POSITION pos = GetHeadPosition( pItemNode );
        while ( NULL != pos )
        {
            CTreeItem * pChildItem = GetNextChild( pItemNode, pos );
            ExpandItem( nModuleIndex, pChildItem );
        }
    }
}


CMySuperGrid::CTreeItem * 
CMySuperGrid::GetModuleNodeFromIndex( int nModuleIndex )
{
    ASSERT( NULL != m_pRootItem );

    POSITION pos = GetHeadPosition( m_pRootItem );
    while ( NULL != pos )
    {
        CTreeItem * pModuleNode = GetNextChild( m_pRootItem, pos );
        ASSERT( IsModuleNode( pModuleNode ) );
        if ( nModuleIndex == GET_MODULE_INDEX( pModuleNode ) )
            return pModuleNode;
    }

    return NULL;
}


CMySuperGrid::CTreeItem * 
CMySuperGrid::GetItemNodeFromIndex( int nModuleIndex, int nItemIndex )
{
    CTreeItem * pModuleNode = GetModuleNodeFromIndex( nModuleIndex );
    if ( NULL == pModuleNode )
        return NULL;

    return GetItemNodeFromIndex( pModuleNode, nItemIndex );
}


CMySuperGrid::CTreeItem * 
CMySuperGrid::GetItemNodeFromIndex( CTreeItem * pRoot, int nItemIndex )
{
    ASSERT( NULL != pRoot );
    ASSERT( !IsProjectNode( pRoot ) );

    POSITION pos = GetHeadPosition( pRoot );
    while ( NULL != pos )
    {
        CTreeItem * pChild = GetNextChild( pRoot, pos );
        if ( GET_ITEM_INDEX( pChild ) == nItemIndex )
            return pChild;

        CTreeItem *pResult = GetItemNodeFromIndex( pChild, nItemIndex );
        if ( NULL != pResult )
            return pResult;
    }

    return NULL;
}


CMySuperGrid::CTreeItem * 
CMySuperGrid::GetNodeByName( LPCTSTR     lpszName, 
                             CTreeItem * pRoot   ,
                             BOOL        fExact   )
{
    ASSERT( NULL != lpszName );
    ASSERT( _T('\\') != *lpszName );  // No leading backslash is allowed.
    ASSERT( _T('\0') != *lpszName );  // No empty string is allowed.
    ASSERT( NULL != pRoot );
    /* The following code is too dangerous, i prefer the caller
     * to do some extra stuffs to guarantee the validity of the parameters.
     *
    if ( _T('\\') == *lpszName )
        // Skip the leading backslash and continue the match
        // exactly from the root node
        return GetNodeByName( lpszName + 1, m_pRootItem, TRUE );
    
    if ( NULL == pRoot )
    {
        // NULL means the user just wants to search from the root
        // of the entire tree.
        ASSERT( NULL != m_pRootItem );
        return GetNodeByName( lpszName, m_pRootItem, fExact );
    }
     *
     */

    POSITION pos = GetHeadPosition( pRoot );
    if ( NULL == pos )
        // No node to match at all.
        return NULL;

    // Get the first node name of the path to match the name of children
    // of pRoot.
    TCHAR szNodeName[MAX_NAME_LENGTH];
    int i=0;
    while ( ( _T('\\') != lpszName[i] ) && ( _T('\0') != lpszName[i] ) )
    {
        szNodeName[i] = lpszName[i];
        ++i;
    }
    szNodeName[i] = _T('\0');

    while ( NULL != pos )
    {
        CTreeItem * pChild = GetNextChild( pRoot, pos );
        ASSERT( NULL != pChild );
        CItemInfo * pInfo = GetData( pChild );
        ASSERT( NULL != pInfo );

        if ( 0 == ::_tcsicmp( pInfo->GetItemText(), szNodeName ) )
        {
            if ( _T('\0') == lpszName[i] )
                return pChild;

            // A path is specified, so we must match the remaining
            // part exactly.
            CTreeItem * pResult = GetNodeByName( lpszName + i + 1, // Skip the backslash
                                                 pChild,
                                                 TRUE );
            if ( NULL != pResult )
                return pResult;
        }

        if ( FALSE == fExact )
        {
            // Search is continued on the child node.
            CTreeItem * pResult = GetNodeByName( lpszName,
                                                 pChild,
                                                 fExact );
            if ( NULL != pResult )
                return pResult;
        }
    }

    return NULL;
}


BOOL CMySuperGrid::GetModuleIndexByName( LPCTSTR lpszName, int * pnModuleIndex )
{
    ASSERT( NULL != lpszName );
    ASSERT( NULL != pnModuleIndex );
    ASSERT( NULL != m_pRootItem );

    CTreeItem * pModuleNode = GetNodeByName( lpszName, m_pRootItem, TRUE );
    if ( NULL == pModuleNode )
    {
        return FALSE;
    }
    else
    {
        ASSERT( TRUE == IsModuleNode( pModuleNode ) );
        *pnModuleIndex = GET_MODULE_INDEX( pModuleNode );
        return TRUE;
    }
}


BOOL CMySuperGrid::LoadNVValue( int                         nNumberOfDirs,
                                const char                * lpszDirs,
                                int                         nNumberOfValueEntries,
                                const NVVALUE_TABLE_ENTRY * pValueEntry )
{
    ASSERT( nNumberOfDirs > 0 );
    ASSERT( NULL != lpszDirs );
    ASSERT( nNumberOfValueEntries > 0 );
    ASSERT( NULL != pValueEntry );

    NODE_INFO * pDirNode = new NODE_INFO [nNumberOfDirs];
    if ( NULL == pDirNode )
    {
        ::SetLastError( ERROR_OUTOFMEMORY );
        return FALSE;
    }

#ifdef _UNICODE
    TCHAR wszDirs[MAX_PATH] = _T("");
#endif

    for ( int i=0; i<nNumberOfDirs; ++i )
    {
#ifdef _UNICODE
        MultiByteToWideChar(CP_ACP,0, lpszDirs,-1, wszDirs,MAX_PATH );
        if ( NULL == ( pDirNode[i].pDir = GetNodeByName( wszDirs,
                                                         m_pRootItem,
                                                         TRUE ) ) )
#else
        if ( NULL == ( pDirNode[i].pDir = GetNodeByName( lpszDirs,
                                                         m_pRootItem,
                                                         TRUE ) ) )
#endif
        {
            TRACE( _T("Load NV Value : Unknown dir %s\n"), lpszDirs );

            ::SetLastError( ERROR_INVALID_PARAMETER );
            delete[] pDirNode;
            return FALSE;
        }

        TCHAR szModuleName[MAX_NAME_LENGTH];
        TCHAR * p = szModuleName;
        while ( _T('\\') != *lpszDirs )
        {
            *(p++) = *(lpszDirs++);
        }
        *p = _T('\0');

        VERIFY( TRUE == GetModuleIndexByName( szModuleName, &pDirNode[i].nModuleIndex ) );

        while ( '\0' != *( lpszDirs++ ) )
            ;
    }

    for ( ; nNumberOfValueEntries>0; nNumberOfValueEntries--, pValueEntry++ )
    {
        CTreeItem * pNode = NULL;
        for ( int i=0; i<nNumberOfDirs; ++i )
        {
#ifdef _UNICODE
            MultiByteToWideChar(CP_ACP,0, pValueEntry->Name,-1, wszDirs,MAX_PATH );
            pNode = GetNodeByName( wszDirs,
                                   pDirNode[i].pDir,
                                   FALSE );      
#else
            pNode = GetNodeByName( pValueEntry->Name,
                                   pDirNode[i].pDir,
                                   FALSE );      
#endif
            if ( NULL != pNode )
            {
                ASSERT( TRUE == IsItemNode( pNode ) );

                ModuleItem item = m_pProject->GetModuleItem( 
                                               pDirNode[i].nModuleIndex, 
                                               GET_ITEM_INDEX( pNode ) );

                if ( pValueEntry->Type == item.eItemType )
                {
                    DWORD dwMask = 0;
                    switch ( pValueEntry->Type ) 
                    {
                    case CHAR_ITEM_TYPE:
                        dwMask = 0x000000FF; break;
                    case SHORT_ITEM_TYPE:
                        dwMask = 0x0000FFFF; break;
                    case LONG_ITEM_TYPE:
                        dwMask = 0xFFFFFFFF; break;
                    default:
                        ASSERT(0); break;
                    }
                        
                    ::_stprintf( item.szItemContent, _T("0x%x"), dwMask & pValueEntry->Value );
                    m_pProject->SetModuleItem( pDirNode[i].nModuleIndex,
                                               GET_ITEM_INDEX( pNode ),
                                               item );
                    break;
                }
                else
                {
                    pNode = NULL;
                }
            }
        }

        if ( NULL == pNode )
        {
            TRACE( _T("Load NV Value : Unknown item name %s\n"), pValueEntry->Name );

            ::SetLastError( ERROR_INVALID_PARAMETER );
            delete[] pDirNode;
            return FALSE;
        }
    }

    delete[] pDirNode;
    Refresh();

    return TRUE;
}


int CMySuperGrid::GetModuleIndexOfNode( CTreeItem * pNode )
{
    ASSERT( NULL != pNode );
    ASSERT( FALSE == IsProjectNode( pNode ) );

    while ( FALSE == IsModuleNode( pNode ) )
    {
        pNode = GetParentItem( pNode );
        ASSERT( NULL != pNode );
    }

    return GET_MODULE_INDEX( pNode );
}

void CMySuperGrid::OnCopyItem() 
{
    int nIndex = GetSelectedItem();
    if(nIndex!=-1)
    {
        CTreeItem* lpItem = GetTreeItem(nIndex);
        m_PasteInfo.strItemPath.Empty();
        GetNodePath( lpItem,m_PasteInfo.strItemPath );
        m_PasteInfo.nPasteType = PASTE_TYPE_COPY;
        m_PasteInfo.bValid = TRUE;
    }
}

void CMySuperGrid::OnCutItem() 
{
    int nIndex = GetSelectedItem();
    if(nIndex!=-1)
    {
        CTreeItem* lpItem = GetTreeItem(nIndex);
        m_PasteInfo.strItemPath.Empty();
        GetNodePath( lpItem,m_PasteInfo.strItemPath );
        m_PasteInfo.nPasteType = PASTE_TYPE_CUT;
        m_PasteInfo.bValid = TRUE;
    }
}

void CMySuperGrid::GetNodePath( CTreeItem* lpItem,CString& strPath )
{
    if( NULL == lpItem )
    {
        return;
    }

    if( IsRoot( lpItem ) )
    {
        return;
    }

    CItemInfo* lpInfo = GetData( lpItem );

    if( !strPath.IsEmpty() )
    {
        strPath.Insert( 0,'\\' );
    }

    strPath.Insert( 0,lpInfo->GetItemText() );

    GetNodePath( GetParentItem( lpItem ),strPath );
}

void CMySuperGrid::OnPasteAfter() 
{
    PasteItem( PASTE_POS_AFTER );
}

void CMySuperGrid::OnPasteAsChild() 
{
    PasteItem( PASTE_POS_CHILD );	
}

void CMySuperGrid::CopyPasteItem( CString& strDstPath,CString& strSrcPath,int nPastePos,CString& strNewName,int nNewId )
{
    CTreeItem* lpSrcItem = GetNodeByName( strSrcPath,GetRootItem(0),TRUE );
    if( NULL == lpSrcItem )
    {
        // Could not find source item
        return;
    }
    
    CTreeItem* lpDstItem = GetNodeByName( strDstPath,GetRootItem(0),TRUE );
    if( NULL == lpDstItem )
    {
        // Could not find destinate item
        return;
    }
    
    //CItemInfo* lpInfo = GetData( lpSrcItem );
    
    ModuleItem miNewItem;
    ModuleItem miArrayItem;
    
    miNewItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpSrcItem ),GET_ITEM_INDEX( lpSrcItem ) );
    if( !strNewName.IsEmpty() )
    {
        // Use new name
        _tcscpy( miNewItem.szItemName,strNewName );
    }

    if( -1 != nNewId )
    {
        // Use new id
        miNewItem.nItemID = nNewId;
    }

    if( ARRAY_ITEM_TYPE == miNewItem.eItemType ||
		ASTRING_ITEM_TYPE == miNewItem.eItemType ||
		WSTRING_ITEM_TYPE == miNewItem.eItemType)
    {
        // Array item is a bit special
        miArrayItem.nItemLength = miNewItem.eItemType;
        miArrayItem.szItemContent[0] = '\0';
       // miArrayItem.szItemDescription[0] = '\0';
        POSITION pos = GetHeadPosition( lpSrcItem );
        CTreeItem* lpChild = GetNextChild( lpSrcItem,pos );
        ModuleItem miChildItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpChild ),GET_ITEM_INDEX( lpChild ) );
        miArrayItem.eItemType = miChildItem.eItemType;

		// must set it zero, because InsertModuleItem in the CopyPasteItem is insert one by one
		// and the array-size will increment in the InsertModuleItem.
		miArrayItem.nArraySize = 0;
		miNewItem.nArraySize = 0;
    }
    
    CTreeItem* lpParentItem,*lpAfterItem;
    if( PASTE_POS_AFTER == nPastePos )
    {
        lpParentItem = GetParentItem( lpDstItem ); 
        lpAfterItem = GetLastItemOfTree( lpDstItem );
    }
    else
    {
        lpParentItem = lpDstItem;
        lpAfterItem = lpDstItem;
    }

    InsertModuleItem( lpParentItem,GET_ITEM_INDEX( lpAfterItem ), &miNewItem,&miArrayItem,TRUE,FALSE );

    BOOL bHasChildren = ItemHasChildren( lpSrcItem );
    // Get child item name list first
    CStringArray arrChildren;
    if( bHasChildren )
    {
        POSITION pos = GetTailPosition( lpSrcItem );
        while( NULL != pos )
        {
            CTreeItem* lpChild = GetPrevChild( lpSrcItem,pos );
            CItemInfo* lpInfo = GetData( lpChild );
            arrChildren.Add( lpInfo->GetItemText() );
        }            
    }

    Refresh();

    if( bHasChildren )
    {
        // Copy subitems
        CString strDstSubPath = strDstPath;
        if( PASTE_POS_AFTER == nPastePos )
        {
            int nIndex = strDstSubPath.ReverseFind( '\\' );
            strDstSubPath = strDstSubPath.Left( nIndex );            
        }
        
        if( strNewName.IsEmpty() )
        {
            int nIndex = strSrcPath.ReverseFind( '\\' );
            strDstSubPath += strSrcPath.Right( strSrcPath.GetLength() - nIndex );
        }
        else
        {
            strDstSubPath += _T("\\");
            strDstSubPath += strNewName;
        }

        for( int i=0;i<arrChildren.GetSize();i++ )
        {
            CString strSrcSubPath = strSrcPath + _T("\\") + arrChildren[i];
			CString strEmpty = _T("");
            CopyPasteItem( strDstSubPath,strSrcSubPath,PASTE_POS_CHILD,strEmpty,nNewId );
        }
    }
}

void CMySuperGrid::CopyPasteItemEx( int nParentIndex,CString& strSrcPath,int nPastePos,int nModuleIndex,int& nIndex,CString& strNewName,int nNewId)
{
	//////////////////////////////////////////////////////////////////////////
	CTreeItem* lpSrcItem = GetNodeByName( strSrcPath,GetRootItem(0),TRUE );
    if( NULL == lpSrcItem )
    {
        // Could not find source item
        return;
    }
    BOOL bUpdateTree = FALSE;
    ModuleItem miNewItem;
	int nSrcItemIndex = GET_ITEM_INDEX( lpSrcItem );
	int nSrcModuleIndex = GetModuleIndexOfNode( lpSrcItem );
	m_pProject->GetItemByName(strSrcPath,nSrcModuleIndex,nSrcItemIndex);
	if ( nModuleIndex==nSrcModuleIndex && nIndex <= nSrcItemIndex)
	{
		bUpdateTree = TRUE;
	}
    //miNewItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpSrcItem ),GET_ITEM_INDEX( lpSrcItem ) );
	miNewItem = m_pProject->GetModuleItem( nSrcModuleIndex,nSrcItemIndex);
    if( !strNewName.IsEmpty() )
    {
        // Use new name
        _tcscpy( miNewItem.szItemName,strNewName );
    }

    if( -1 != nNewId )
    {
        // Use new id
        miNewItem.nItemID = nNewId;
    }
	if( ARRAY_ITEM_TYPE == miNewItem.eItemType ||
		ASTRING_ITEM_TYPE == miNewItem.eItemType ||
		WSTRING_ITEM_TYPE == miNewItem.eItemType)
    {
		miNewItem.nArraySize = 0;
    }
	if (miNewItem.szItemDescription && _tcslen(miNewItem.szItemDescription))
	{
		int nLen = _tcslen(miNewItem.szItemDescription) +1;
		CString strDesc(miNewItem.szItemDescription);
		miNewItem.szItemDescription = (TCHAR*)malloc(nLen * sizeof(TCHAR));
		_tcscpy(miNewItem.szItemDescription, strDesc.operator LPCTSTR());
	}

	nIndex = InsertModuleItemEx( nParentIndex,nModuleIndex,nIndex, &miNewItem,TRUE,bUpdateTree);

    BOOL bHasChildren = ItemHasChildren( lpSrcItem );
    // Get child item name list first
    CStringArray arrChildren;
    if( bHasChildren )
    {
		ModuleItem miParentItem;
		miParentItem = m_pProject->GetModuleItem(nModuleIndex,nIndex);

        POSITION pos = GetHeadPosition( lpSrcItem );
        while( NULL != pos )
        {
            CTreeItem* lpChild = GetNextChild( lpSrcItem,pos );
            CItemInfo* lpInfo = GetData( lpChild );
            arrChildren.Add( lpInfo->GetItemText() );
        }            

        for( int i=0;i<arrChildren.GetSize();i++ )
        {
            CString strSrcSubPath = strSrcPath + _T("\\") + arrChildren[i];
			CString strEmpty = _T("");
            CopyPasteItemEx(miParentItem.nItemIndex, strSrcSubPath,PASTE_POS_CHILD,nModuleIndex,nIndex,strEmpty,nNewId);
        }
    }
}

void CMySuperGrid::PasteItem( int nPastePos )
{
    int nIndex = GetSelectedItem();
    if( nIndex == -1 )
    {
        return;
    }   

    CTreeItem* lpDstItem = GetTreeItem(nIndex);
    CTreeItem* lpParentItem = GetParentItem( lpDstItem );
    CTreeItem* lpIdItem = NULL;
    BOOL bInputId;
    ModuleItem miItem;
    
    if( IsModuleNode( lpParentItem ) )
    {
        if( PASTE_POS_CHILD == nPastePos )
        {
            bInputId = FALSE;
            lpIdItem = lpDstItem;
        }
        else
        {
            bInputId = TRUE;
        }
    }
    else
    {
        // The new item id must be same with its parent item
        bInputId = FALSE;
        lpIdItem = lpParentItem;
    }

    int nId = -1;
    if( !bInputId )
    {
        miItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpIdItem ),GET_ITEM_INDEX( lpIdItem ) );
        nId = miItem.nItemID;
    }
    
    CString strName;
    int nSlash = m_PasteInfo.strItemPath.ReverseFind( '\\' );
    if( -1 != nSlash )
    {
        strName = m_PasteInfo.strItemPath.Right( m_PasteInfo.strItemPath.GetLength() - nSlash - 1 );
    }

    CPasteSettingsDlg dlg;
    dlg.m_strNewName = strName;
    dlg.m_nNewId = nId;
    if( IDOK != dlg.DoModal() )
    {
        return;
    }

    CString strDstPath;
    GetNodePath( lpDstItem,strDstPath );

    if( PASTE_POS_CHILD == nPastePos )
    {
        miItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpDstItem ),GET_ITEM_INDEX( lpDstItem ) );
    }
    else
    {
        CTreeItem* lpParentItem = GetParentItem( lpDstItem );
        if( IsModuleNode( lpParentItem ) )
        {
            miItem.eItemType = STRUCT_ITEM_TYPE;
        }
        else
        {
            miItem = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpParentItem ),GET_ITEM_INDEX( lpParentItem ) );
        }
    }
     
    if( STRUCT_ITEM_TYPE != miItem.eItemType )
    {
        // Only struct and module item can have children.
        AfxMessageBox( _T("Items can not be coppied here,this item or its parent have no children!"),MB_ICONSTOP );
        return;
    }

    CWaitCursor ws;

    LockWindowUpdate();

    CString strNewName = dlg.GetNewName();
  //  CopyPasteItem( strDstPath,m_PasteInfo.strItemPath,nPastePos,strNewName,dlg.GetNewId() );
	CTreeItem* lpAfterItem;
	ModuleItem stParentItem;
	int nParentIndex = -1;

    if( PASTE_POS_AFTER == nPastePos )
    {
        lpParentItem	= GetParentItem( lpDstItem ); 
        lpAfterItem		= GetLastItemOfTree( lpDstItem );
		stParentItem	= m_pProject->GetModuleItem( GetModuleIndexOfNode( lpAfterItem ),GET_ITEM_INDEX( lpDstItem ) );
		nParentIndex	= stParentItem.nParentItemIndex;
    }
    else
    {
        lpParentItem	= lpDstItem;
        lpAfterItem		= lpDstItem;
		stParentItem		= m_pProject->GetModuleItem( GetModuleIndexOfNode( lpAfterItem ),GET_ITEM_INDEX( lpDstItem ) );
		nParentIndex		= stParentItem.nItemIndex;
    }
	int nModuleIndex	= GetModuleIndexOfNode( lpAfterItem );
	int nStartIndex		= GET_ITEM_INDEX( lpAfterItem );
	

	CopyPasteItemEx(nParentIndex,m_PasteInfo.strItemPath,nPastePos,nModuleIndex,nStartIndex,strNewName,dlg.GetNewId());

    if( PASTE_TYPE_CUT == m_PasteInfo.nPasteType )
    {
        CTreeItem* lpSrcItem = GetNodeByName( m_PasteInfo.strItemPath,GetRootItem(0),TRUE );
        RemoveItem( lpSrcItem,TRUE );
        m_PasteInfo.bValid = FALSE;
    }
	Refresh();
    UnlockWindowUpdate();
}

void CMySuperGrid::OnExportItem() 
{
	// Open file for write first
//    static TCHAR BASED_CODE szFilter[] = _T("NV export Files (*.nv)|*.nv|All files (*.*)|*.*||");

//	CFileDialog dlg(FALSE, _T("nv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
    CDlgExport dlg;
	if( IDOK != dlg.DoModal() )
    {
        // User cancel
        return;
    }	

	CWaitCursor wc;
	CStdioFile file;
	LPCTSTR lpStrFile = dlg.m_strFile.operator LPCTSTR();
	CString strError;
	
	try
	{
		BOOL bCreateMode = TRUE;
		if (dlg.m_bChanged && dlg.m_bAppendMode)
		{	
			if(!file.Open(lpStrFile,CFile::modeWrite|CFile::typeText))     
			{     
				if (!file.Open(lpStrFile,CFile::modeCreate|CFile::modeWrite| CFile::typeText))
				{
					strError.Format(_T("Create merge nv file fail,Errorcde = 0x%x."),GetLastError());
					AfxMessageBox(strError);
					return;
				}
			} 
			else
			{
				bCreateMode = FALSE;
			}
			file.SeekToEnd();
		}
		else
		{
			if (!file.Open(lpStrFile,CFile::modeCreate|CFile::modeWrite| CFile::typeText))
			{
				strError.Format(_T("Create nv file fail,Errorcde = 0x%x."),GetLastError());
				AfxMessageBox(strError);
				return;
			}
		}
		int nIndex = GetSelectedItem();
		if(nIndex!=-1)
		{
			// Export item to file
			CTreeItem* lpItem = GetTreeItem(nIndex);
			// Write file header
			if (bCreateMode)
			{
				file.WriteString( g_szNVFileHeader);//_T( "// Spreadtrum nv export file version 1.0 \n\n" ) 
			}
			else
			{
				AddComments(lpItem,file);
			}
			
			ExportItem( lpItem,file, dlg.m_bReadOnly,dlg.m_bChanged	);
		}

		file.Close();
		
	}
	catch( CFileException* pfe )
	{
		// Error occurs
		pfe->ReportError( MB_ICONSTOP );
		AfxMessageBox( _T( "Export failed!" ) );
		pfe->Delete();
	}
}

void CMySuperGrid::AddComments(CTreeItem* lpItem,CStdioFile& file)
{
	if (HasChangeItem(lpItem))
	{	
		TCHAR szComments[MAX_PATH*2]= {0};
		TCHAR szPCName[MAX_PATH]	= {0};
		TCHAR szUserName[MAX_PATH]	= {0};
	
		DWORD dwPCNameBufLen		= MAX_PATH*sizeof(TCHAR);  
		DWORD dwUserNameBufLen		= MAX_PATH*sizeof(TCHAR);  
		SYSTEMTIME  currentTime;
		GetLocalTime( &currentTime);
		if ( GetComputerName(szPCName, &dwPCNameBufLen) && GetUserName(szUserName, &dwUserNameBufLen))
		{
			_stprintf(szComments,_T("\n\n// Update on %4d-%02d-%02d %02d:%02d:%02d.%03d by %s on %s\n"),
				currentTime.wYear,
				currentTime.wMonth,
				currentTime.wDay,
				currentTime.wHour,
				currentTime.wMinute,
				currentTime.wSecond,
				currentTime.wMilliseconds,
				szUserName,
				szPCName
				);
		}
		else
		{
			_stprintf(szComments,_T("\n\n// Update on %4d-%02d-%02d %02d:%02d:%02d.%03d\n"),
				currentTime.wYear,
				currentTime.wMonth,
				currentTime.wDay,
				currentTime.wHour,
				currentTime.wMinute,
				currentTime.wSecond,
				currentTime.wMilliseconds);
		}
		file.WriteString(szComments);
	}
}
BOOL CMySuperGrid::IsExistCustNV(BOOL bCustBase/* = FALSE*/)
{
	CTreeItem* lpRootItem = GetTreeItem(0);	
	if (bCustBase)
	{
		return HasCustBaseNV(lpRootItem);
	}
	else
	{
		return HasCustNVItem(lpRootItem);
	}
	
}

BOOL CMySuperGrid::ExportCustNV(LPCTSTR lpCustNVFile,BOOL bCustBase/* = FALSE*/)
{

	CStdioFile file;
	CString strError;
	if(!file.Open(lpCustNVFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText))     
	{     
		AfxMessageBox(_T("Save customer.nv fail."));
		return FALSE;
	}  
	CTreeItem* lpItem = GetTreeItem(0);
	file.WriteString( g_szNVFileHeader);//_T( "// Spreadtrum nv export file version 1.0 \n\n" ) 
	//AddComments(lpItem,file);
	if (bCustBase)
	{
		ExportItem( lpItem,file, FALSE,FALSE,FALSE,TRUE);
	}
	else
	{
		ExportItem( lpItem,file, FALSE,FALSE,TRUE,FALSE);
	}
	
	file.Close();
	
	return TRUE;

}
BOOL CMySuperGrid::HasCustBaseNV(CTreeItem* lpItem)
{
	BOOL bHasCustBase = FALSE;
	if (NULL == lpItem)
	{
		return FALSE;
	}

	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );		
			bHasCustBase = HasCustBaseNV( lpChild);
			if (bHasCustBase)
			{
				break;
			}
		}		
	}
	else
	{	
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );	
		if(mi.bCustBase)
		{
			bHasCustBase = TRUE;
		}		
		
	}
	return bHasCustBase;
}

BOOL CMySuperGrid::HasCustNVItem(CTreeItem* lpItem)
{
	BOOL bHasCustNV = FALSE;
	if (NULL == lpItem)
	{
		return FALSE;
	}
	
	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );		
			bHasCustNV = HasCustNVItem( lpChild);
			if (bHasCustNV)
			{
				break;
			}
		}		
	}
	else
	{	
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );		
		if(mi.bCustItem)
		{
			bHasCustNV = TRUE;
		}		
		
	}
	return bHasCustNV;
}

BOOL CMySuperGrid::HasChangeItem(CTreeItem* lpItem,BOOL bCustNV/* = FALSE*/)
{
	BOOL bChanged = FALSE;
	if (NULL == lpItem)
	{
		return FALSE;
	}
	
	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );
		
			bChanged = HasChangeItem( lpChild);
			if (bChanged)
			{
				break;
			}
		}		
	}
	else
	{	
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
		
		if (bCustNV)
		{
			if( mi.bChanged && mi.bCustItem)
			{
				bChanged = TRUE;
			}
		}
		else
		{
			if( mi.bChanged && mi.bCustBase)
			{
				bChanged = TRUE;
			}
		}		
		
	}
	return bChanged;

}
void CMySuperGrid::ExportItem( CTreeItem* lpItem,CStdioFile& file,BOOL bReadOnly /*= FALSE*/, BOOL bChanged /*= FALSE*/,BOOL bCustNV/* = FALSE */,BOOL bCustBaseNV /*= FALSE*/)
{
	ASSERT( lpItem );

	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );
			ExportItem( lpChild,file, bReadOnly, bChanged,bCustNV );
		}		
	}
	else
	{
		// Export current item	
		
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
		BOOL bExport  = TRUE;
		if( bReadOnly && !mi.bReadOnly)
		{
			bExport = FALSE;
		}

		if( bExport && bChanged && !mi.bChanged)
		{
			bExport = FALSE;
		}
		if(bExport && bCustNV && !mi.bCustItem)
		{
			bExport = FALSE;
		}
		if (bExport && bCustBaseNV &&!mi.bCustBase)
		{
			bExport = FALSE;
		}

		if(bExport)
		{
			CString strPath;
			GetNodePath( lpItem,strPath );
			strPath += "=";
			strPath += mi.szItemContent;
			strPath += "\n";
			file.WriteString( strPath );
		}
	}
}

void CMySuperGrid::SetParent2CustBase(CTreeItem* pChild,BOOL bCustBase/* = FALSE*/)
{
	if (NULL == pChild)
	{
		return;
	}
	CTreeItem* pParent = GetParentItem(pChild);
	if (IsProjectNode(pParent))
	{
		return;
	}
	int nModuleIndex = GetModuleIndexOfNode( pParent );
	if (IsModuleNode(pParent))
	{
		Module stMdu= m_pProject->GetModule(nModuleIndex);
		stMdu.bCustBase = bCustBase;
		m_pProject->SetModule(nModuleIndex,stMdu);
		return;
	}
	
	ModuleItem mi = m_pProject->GetModuleItem( nModuleIndex,GET_ITEM_INDEX( pParent ) );
	mi.bCustBase = bCustBase;
	m_pProject->SetModuleItem(nModuleIndex, GET_ITEM_INDEX( pParent ), mi);
    SetParent2CustBase(pParent,bCustBase);
}
void CMySuperGrid::ImportNVItem(LPCTSTR lpPath,LPCTSTR lpValue,int nLineNo,CDlgMsg& dlgMsg,BOOL bHasCustBase,BOOL bCustItem,BOOL bCustBase)
{
	extern BOOL ValueIsValid( TCHAR* );
	if( !ValueIsValid( (TCHAR*)lpValue ) )
	{
		// szValue is not a valid number
		CString strText;			
		strText.Format( _T( "The value in line %d is not a number!" ),nLineNo );
		dlgMsg.m_agItem.Add(strText);
		return;
	}
				
	CTreeItem* lpItem = GetNodeByName( lpPath,GetRootItem(0),TRUE );
	if( lpItem )
	{
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
		if( ARRAY_ITEM_TYPE == mi.eItemType || 
			STRUCT_ITEM_TYPE == mi.eItemType ||
			ASTRING_ITEM_TYPE == mi.eItemType ||
			WSTRING_ITEM_TYPE == mi.eItemType)
		{
			// the item has child item,could not be imported
			CString strText;
			strText.Format( _T( "%s %s" ),lpPath,_T( "has child items,can not be imported!" ) );
			dlgMsg.m_agItem.Add(strText);
		}
		else
		{					
			if( !mi.bReadOnly || (mi.bReadOnly && SUPER_PERMISSION == theApp.GetPermission()) )
			{
				if (bCustItem)
				{
					if ( !bHasCustBase || (bHasCustBase && mi.bCustBase) )
					{
						mi.bCustItem = TRUE;
					}
					
				}
				if (bCustBase)
				{
					mi.bCustBase = TRUE;
					SetParent2CustBase(lpItem,bCustBase);
				}
				else
				{
					mi.bChanged = CompareItem(mi.szItemContent, lpValue);  
					_tcscpy( mi.szItemContent,lpValue );	
				}
				m_pProject->SetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ),mi,TRUE );
			}					
		}
	}
	else
	{
		// Could not find specified item
		CString strText;
		strText.Format( _T("Can not find item: %s" ),lpPath );
		dlgMsg.m_agItem.Add(strText);
	}			

}
// Used for gui mode
void CMySuperGrid::ImportItem( CString strFile,BOOL bCustItem /*= FALSE*/,BOOL bCustBase /*= FALSE*/) 
{
//#define MAX_DISP_ERR_NUM 50
	CWaitCursor wc;
	CDlgMsg dlgMsg;
	BOOL bHasCustBase = IsExistCustNV(TRUE);
#define MAX_NV_PATH_LEN		(1024*2)
#define MAX_NV_VALUE_LEN	(1024*100)
	TCHAR* pszPath = new TCHAR [MAX_NV_PATH_LEN];
	TCHAR* pszValue = new TCHAR [MAX_NV_VALUE_LEN];
	if(NULL == pszPath || NULL == pszValue)
	{
		SAFE_DELETE_ARRAY(pszPath);
		SAFE_DELETE_ARRAY(pszValue);
		return;
	}
	
	
	try
	{
		CStdioFile file( strFile.GetBuffer(0),CFile::modeRead | CFile::typeText );
		CString strLine;
		//CString strError;
		int nLineNo = 0;

		//int nErrNum = 0;
		while( file.ReadString( strLine ) )
		{
			nLineNo++;
			strLine.TrimLeft();
			if( strLine.IsEmpty() )
			{
				// Blank line
				continue;
			}

			
			if( strLine.Find(_T("//"),0) == 0 || strLine.Find(_T("#"),0) == 0 || strLine.Find(_T('%'), 0) != -1)
			{
				// Comment line
				continue;
			}
			
			ZeroMemory(pszPath,MAX_NV_PATH_LEN*sizeof(TCHAR));
			ZeroMemory(pszValue,MAX_NV_VALUE_LEN*sizeof(TCHAR));
			_stscanf( strLine,_T("%[^'=']=%s"),pszPath,pszValue );

			/*CR212418 xmzhang add begin*/
			TCHAR *tmp = pszPath;
			UINT lentmp = _tcslen(pszPath)-1;
			tmp += (_tcslen(pszPath)-1);
			//去除=号前的空格和制表符
			while((0x20 == *tmp||0x09 == *tmp)&&( lentmp > 0))
			{
				*tmp = 0x00;
				tmp--;
				lentmp--;
			}
			/*CR212418 xmzhang add end*/
			// suppport --> Spreadtrum nv export file version 1.1
			// agc_gain_main_center_array[0-9]=0x64D,0x64E,0x64F,0x94F,0x64E,0x64F,0x94F,0x64E,0x64F,0x94F		
			
			CStringArray strArray;
			int nCount = SplitStr(pszValue, strArray);
			if ( nCount >1 )
			{
				CString strTitle = GetStrFileTitle(pszPath);
				CString strPrePath = GetStrFilePath(pszPath,FALSE,TRUE);
				TCHAR szPrefix[MAX_PATH] = {0};
				TCHAR szStart[MAX_PATH] = {0};
				TCHAR szEnd[MAX_PATH] = {0};
				_stscanf( strTitle.operator LPCTSTR(),_T("%[^[][%[^-]-%[^]]"),szPrefix,szStart,szEnd );
				unsigned int nStart = GetDigit(szStart);
				unsigned int nEnd	= GetDigit(szEnd);
				int nItemCount  = nEnd - nStart +1;
				if (nEnd > nStart && nCount == nItemCount)
				{
					for (int nIndex = 0; nIndex < nItemCount; ++nIndex)
					{
						CString strRelPath;
						strRelPath.Format(_T("%s%s[%d]"),strPrePath,szPrefix,nIndex+nStart);
						CString strRelValue = strArray.GetAt(nIndex);

						ImportNVItem(strRelPath.operator LPCTSTR(),strRelValue.operator LPCTSTR(), nLineNo,dlgMsg,bHasCustBase, bCustItem, bCustBase);
					}
				}
				else
				{
					CString strText;
					strText.Format( _T( "Invalid start-end range in line %d!" ),nLineNo );
					dlgMsg.m_agItem.Add(strText);
					continue;
				}
			}
			else
			{
				ImportNVItem(pszPath,pszValue, nLineNo,dlgMsg,bHasCustBase, bCustItem, bCustBase);
			}
					
		}
			
		// Update gui
		Refresh();

		if(dlgMsg.m_agItem.GetSize()>0)
		{
			dlgMsg.m_strTitle.Format(_T("There were %d errors!"),dlgMsg.m_agItem.GetSize());
			dlgMsg.DoModal();
		}
	}
	catch( CFileException* pfe )
	{
		// Error occurs
		pfe->ReportError( MB_ICONSTOP );
		AfxMessageBox( _T( "Import failed!" ) );
		pfe->Delete();
	}
	SAFE_DELETE_ARRAY(pszPath);
	SAFE_DELETE_ARRAY(pszValue);
}
//Joly[[
unsigned int CMySuperGrid::StrToInt(CString& strContent)
{
	unsigned int nValue = 0;
	strContent.MakeUpper();
	if (strContent.IsEmpty())
	{
		return 0;
	}
	if( 0 == strContent.Find(_T("0X")) )
	{
		_stscanf( strContent.operator LPCTSTR(), _T("0X%X"), &nValue );
	}
	else
	{
		_stscanf( strContent.operator LPCTSTR(), _T("%d"), &nValue );
	}

	return nValue;
}
//]]

BOOL CMySuperGrid::CompareItem(LPCTSTR lpszOpt1, LPCTSTR lpszOpt2)
{
    CString strOpt1(lpszOpt1);
    CString strOpt2(lpszOpt2);

    BOOL isDiff = FALSE;

	// Joly[[ modify for 345940
    unsigned int nOpt1 = 0, nOpt2 = 0;

	nOpt1 = StrToInt(strOpt1);

	nOpt2 = StrToInt(strOpt2);
	
	if (nOpt2 != nOpt1)
	{
		isDiff = TRUE;
	}

//     if (strOpt2.IsEmpty() && (0 == strOpt1.CompareNoCase(_T("0x0"))))
//     {
//         isDiff = FALSE;
//     }
//     else if (strOpt1.IsEmpty() && (0 == strOpt2.CompareNoCase(_T("0x0"))))
//     {
//         isDiff = FALSE;
//     }
//     else
//     {
//         isDiff = (0 == strOpt1.CompareNoCase(strOpt2)) ? FALSE : TRUE;
//     }
	//]]

    return isDiff;
}

// Used for gui mode
BOOL CMySuperGrid::ImportItem( CStringArray &agItemValues , BOOL bOneOK /*= FALSE*/) 
{
	CWaitCursor wc;

	int nCount = agItemValues.GetSize()/2;

	CString strError=_T("");

	BOOL bOK = FALSE;

	for(int i = 0; i< nCount; i++)
	{
		CString strPath = agItemValues[2*i];
		CString strValue = agItemValues[2*i+1];

		extern BOOL ValueIsValid( TCHAR* );

		if( !ValueIsValid( (LPTSTR)(LPCTSTR)strValue ) )
		{
			// szValue is not a valid number
			CString strText;
			strText.Format( _T( "[%s] is not a number in [%s]!\n" ),strValue,strPath );
			strError += strText;
			continue;
		}

		CTreeItem* lpItem = GetNodeByName( (LPTSTR)(LPCTSTR)strPath,GetRootItem(0),TRUE );
		if( lpItem )
		{
			ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
			if( ARRAY_ITEM_TYPE == mi.eItemType || 
				STRUCT_ITEM_TYPE == mi.eItemType ||
				ASTRING_ITEM_TYPE == mi.eItemType ||
				WSTRING_ITEM_TYPE == mi.eItemType)
			{
				// the item has child item,could not be imported
				CString strText;
				strText.Format( _T( "%s %s\n" ),(LPTSTR)(LPCTSTR)strPath,_T( "has child items,can not be imported!" ) );
				strError += strText;
			}
			else
			{
				mi.bChanged = _tcsicmp( mi.szItemContent,(LPTSTR)(LPCTSTR)strValue );
				_tcscpy( mi.szItemContent,(LPTSTR)(LPCTSTR)strValue );
				m_pProject->SetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ),mi,TRUE );
				bOK = TRUE;
			}
		}
		else
		{
			// Could not find specified item
			CString strText;
			strText.Format( _T( "%s %s\n" ),_T("Can not find item:"),(LPTSTR)(LPCTSTR)strPath );
			strError += strText;
		}			
	}
	
	// Update gui
	Refresh();

	// Report error
	if( !strError.IsEmpty() )
	{
		if(bOneOK && bOK)
		{
			return TRUE;
		}

		AfxMessageBox( strError );
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL CMySuperGrid::TestItemChange( CStringArray &agItemValues, BOOL bOneOK/* = FALSE*/ )
{
	CWaitCursor wc;

	int nCount = agItemValues.GetSize()/2;
	BOOL bChange = FALSE;

	BOOL bFindOne = FALSE;

	for(int i = 0; i< nCount; i++)
	{
		CString strPath = agItemValues[2*i];
		CString strValue = agItemValues[2*i+1];

		CTreeItem* lpItem = GetNodeByName( (LPTSTR)(LPCTSTR)strPath,GetRootItem(0),TRUE );
		if( lpItem )
		{
			ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
			if( ARRAY_ITEM_TYPE == mi.eItemType || 
				STRUCT_ITEM_TYPE == mi.eItemType ||
				ASTRING_ITEM_TYPE == mi.eItemType ||
				WSTRING_ITEM_TYPE == mi.eItemType)
			{
				bChange = TRUE;
				break;
			}
			else
			{
				bFindOne = TRUE;
				bChange = _tcsicmp( mi.szItemContent,(LPTSTR)(LPCTSTR)strValue );
				if(bChange)
					break;
			}
		}
		else
		{
			if(!bOneOK)
			{
				bChange = TRUE;
				break;
			}
		}			
	}

	if(bOneOK && !bFindOne)
	{
		return TRUE;
	}
	
	return bChange;
}
void CMySuperGrid::SetCustBaseItem(CTreeItem *pItem,BOOL bCustBase/*=FALSE*/)
{
	CItemInfo *lp = GetData(pItem);
	lp->SetCustBaseItem(bCustBase);
	
	CTreeItem *pParent = GetParentItem(pItem);
	if(pParent != NULL && pParent != m_pRootItem )
	{
		if(bCustBase)
		{
			SetCustBaseItem(pParent,bCustBase);
		}
		else
		{
			BOOL bSetCustBase = FALSE;
			POSITION pos = GetHeadPosition(pParent);
			while(pos)
			{
				CTreeItem *pChild = GetNextChild(pParent,pos);
				if(pChild != NULL)
				{
					if(GetData(pChild)->IsCustBaseItem())
					{
						bSetCustBase = TRUE;
						break;
					}
				}
			}
			SetCustBaseItem(pParent,bSetCustBase);
		}
	}
}

void CMySuperGrid::SetCustItem(CTreeItem *pItem,BOOL bCustItem/* = FALSE*/)
{
	CItemInfo *lp = GetData(pItem);
	lp->SetCustItem(bCustItem);
	
	CTreeItem *pParent = GetParentItem(pItem);
	if(pParent != NULL && pParent != m_pRootItem )
	{
		if(bCustItem)
		{
			SetCustItem(pParent,bCustItem);
		}
		else
		{
			BOOL bSetCustItem = FALSE;
			POSITION pos = GetHeadPosition(pParent);
			while(pos)
			{
				CTreeItem *pChild = GetNextChild(pParent,pos);
				if(pChild != NULL)
				{
					if(GetData(pChild)->IsCustItem())
					{
						bSetCustItem = TRUE;
						break;
					}
				}
			}
			SetCustItem(pParent,bSetCustItem);
		}
	}
}
void CMySuperGrid::SetItemChanged(CTreeItem *pItem,BOOL bChanged /*= TRUE*/)
{
	CItemInfo *lp = GetData(pItem);
	lp->SetChanged(bChanged);

	CTreeItem *pParent = GetParentItem(pItem);
	if(pParent != NULL && pParent != m_pRootItem )
	{
		if(bChanged)
		{
			SetItemChanged(pParent,bChanged);
		}
		else
		{
			BOOL bChged = FALSE;
			POSITION pos = GetHeadPosition(pParent);
			while(pos)
			{
				CTreeItem *pChild = GetNextChild(pParent,pos);
				if(pChild != NULL)
				{
					if(GetData(pChild)->GetChanged())
					{
						bChged = TRUE;
						break;
					}
				}
			}
			SetItemChanged(pParent,bChged);
		}
	}
}

//[[ add by Joly
void CMySuperGrid::UpdateItemID(CTreeItem* lpItem, UINT uNewID/*, int nCount*/)
{
	ModuleItem mi = m_pProject->GetModuleItem(GetModuleIndexOfNode(lpItem), GET_ITEM_INDEX(lpItem));
	
	mi.nItemID = uNewID;	
	m_pProject->SetModuleItem(GetModuleIndexOfNode(lpItem), GET_ITEM_INDEX(lpItem), mi);

	if(ItemHasChildren(lpItem))
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition(lpItem);
		while(pos)
		{
			CTreeItem* lpChild = GetNextChild(lpItem,pos);
			UpdateItemID(lpChild, uNewID/*, nCount++*/);
		}		
	}
}

void CMySuperGrid::OnModifyItemID()
{
	int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
	
    CTreeItem * pSelItem = GetTreeItem(nSel);
	int nIndex = GetData( pSelItem )->GetItemData();
	int nCurSelMdl = GetSelectedModuleIndex();

	CEditItemID  editIDdlg;
	editIDdlg.SetProject(m_pProject, nCurSelMdl, nIndex);
	
	if (IDOK == editIDdlg.DoModal())
	{
		UINT uNewID = editIDdlg.GetNewID();
		m_hSelectedItem.nItemID = uNewID;
		m_pProject->SetModuleItem(m_nSelectedModuleIndex, m_nSelectedItemIndex, m_hSelectedItem);
		UpdateItemID(pSelItem, uNewID);
		Refresh();
	}
}
//]]

// void CMySuperGrid::OnLButtonDown(UINT nFlags, CPoint point) 
// {
// 	// TODO: Add your message handler code here and/or call default
// 	if( GetFocus() != this) 
// 		SetFocus();
// 	
// 	LVHITTESTINFO ht;
// 	ht.pt = point;
// 	SubItemHitTest(&ht);
// 	if(OnItemLButtonDown(ht))
// 	{
// 		BOOL bSelect=1;
// 		bSelect = HitTestOnSign(point, ht);
// 		if(bSelect && ht.iItem == -1)
// 		{
// 
// 		}
// 		else
// 		{
// 			CSuperGridCtrl::OnLButtonDown(nFlags, point);
// 		}
// 	}
// 	
// }

void CMySuperGrid::OnMarkAsCustNV() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
	
	CTreeItem * pSelItem = GetTreeItem(nSel);
	MarkCustNV(pSelItem,TRUE);
	Refresh();
	UpdateAllViews();
}

void CMySuperGrid::OnUnmarkAsCustNV() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
	
	CTreeItem * pSelItem = GetTreeItem(nSel);
	if (pSelItem)
	{
		MarkCustNV(pSelItem,FALSE);
		Refresh();
		UpdateAllViews();
	}
}
void CMySuperGrid::OnUnmarkAllCustNV() 
{
	// TODO: Add your command handler code here
	if( AfxMessageBox( _T("Are you sure to unmark all items as customer nv?"),MB_YESNO ) == IDYES )
	{
		CTreeItem* lpRootItem = GetTreeItem(0);	
		if (lpRootItem)
		{
			MarkCustNV(lpRootItem,FALSE);
			Refresh();
			UpdateAllViews();
		}
	}
}

void CMySuperGrid::MarkCustNV( CTreeItem* lpItem,BOOL bCustNV)
{
	if (NULL == lpItem)
	{
		return;
	}
	
	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );
			MarkCustNV( lpChild,bCustNV );
		}		
	}
	else
	{
		// Export current item	
		
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );	
	
		if( bCustNV && !mi.bCustItem && mi.bCustBase && (!mi.bReadOnly || (mi.bReadOnly && SUPER_PERMISSION == theApp.GetPermission())) )
		{
			mi.bCustItem = TRUE;
			m_pProject->SetModuleItem(GetModuleIndexOfNode( lpItem ), GET_ITEM_INDEX( lpItem ), mi);
		}
		else if (!bCustNV && mi.bCustItem)
		{
			mi.bCustItem = FALSE;
			m_pProject->SetModuleItem(GetModuleIndexOfNode( lpItem ), GET_ITEM_INDEX( lpItem ), mi);
		}
	}
}

void CMySuperGrid::UpdatedTextColor(CTreeItem* lpItem)
{
	if (NULL == lpItem)
	{
		return;
	}
	ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
	if(mi.bChanged)
	{							
		if(mi.bReadOnly)
		{
			GetData(lpItem)->SetTextClr( COLOR_PINKISH_RED_TEXT );	
		}
		else
		{
			GetData(lpItem)->SetTextClr( COLOR_RED_TEXT );
		}
	}
	else
	{
		if(mi.bReadOnly)
		{						
			GetData(lpItem)->SetTextClr( GetSysColor (COLOR_GRAYTEXT) );
		}										
		else
		{							
			GetData(lpItem)->SetTextClr( GetSysColor (COLOR_WINDOWTEXT) );
		}
	}
	Invalidate();
	if(mi.bChanged)
	{
		UpdateAllViews();
	}
	
	
}

void CMySuperGrid::OnUnmarkAllChangeAsCustNV() 
{
	// TODO: Add your command handler code here
	CTreeItem* lpRootItem = GetTreeItem(0);	
	if (lpRootItem)
	{
		MarkAllChangedCustNV(lpRootItem,FALSE);
		Refresh();
		UpdateAllViews();
	}
}

void CMySuperGrid::OnMarkAllChangeAsCustNV() 
{
	// TODO: Add your command handler code here
	CTreeItem* lpRootItem = GetTreeItem(0);	
	if (lpRootItem)
	{
		MarkAllChangedCustNV(lpRootItem,TRUE);
		Refresh();
		UpdateAllViews();
	}
	
}

void CMySuperGrid::UpdateAllViews()
{
	if (m_pSyncCallBack)
	{
		m_pSyncCallBack();
	}
}
void CMySuperGrid::MarkAllChangedCustNV( CTreeItem* lpItem,BOOL bCustNV)
{
	if (NULL == lpItem)
	{
		return;
	}
	
	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );
			MarkAllChangedCustNV( lpChild,bCustNV );
		}		
	}
	else
	{
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );
		if (mi.bChanged)
		{
			if( bCustNV && !mi.bCustItem && mi.bCustBase)
			{
				mi.bCustItem = TRUE;
				m_pProject->SetModuleItem(GetModuleIndexOfNode( lpItem ), GET_ITEM_INDEX( lpItem ), mi);
			}
			else if (!bCustNV && mi.bCustItem)
			{
				mi.bCustItem = FALSE;
				m_pProject->SetModuleItem(GetModuleIndexOfNode( lpItem ), GET_ITEM_INDEX( lpItem ), mi);
			}
		}	
	}
}

void CMySuperGrid::OnMarkAsCustbase() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
	
	CTreeItem * pSelItem = GetTreeItem(nSel);
	MarkCustBaseNV(pSelItem,TRUE);	
	SetParent2CustBase(pSelItem,TRUE);
	Refresh();
	UpdateAllViews();
}

void CMySuperGrid::OnUnmarkAsCustbase() 
{
	// TODO: Add your command handler code here
	int nSel = GetSelectedItem();
    if ( -1 == nSel )
		return;
	
	CTreeItem * pSelItem = GetTreeItem(nSel);
	MarkCustBaseNV(pSelItem,FALSE);
	if(!ParentHasCustBase(pSelItem))
	{
		SetParent2CustBase(pSelItem,FALSE);
	}
	Refresh();
	UpdateAllViews();
}

void CMySuperGrid::MarkCustBaseNV(CTreeItem* lpItem,BOOL bCustBase)
{
	if (NULL == lpItem)
	{
		return;
	}

	if (IsProjectNode(lpItem))
	{
		return;
	}
	int nModuleIndex = GetModuleIndexOfNode( lpItem );
	if (IsModuleNode(lpItem))
	{
		Module stMdu= m_pProject->GetModule(nModuleIndex);
		stMdu.bCustBase = bCustBase;
		m_pProject->SetModule(nModuleIndex,stMdu);
	}
	else 
	{
		ModuleItem mi = m_pProject->GetModuleItem( nModuleIndex,GET_ITEM_INDEX( lpItem ) );	
		if( bCustBase && !mi.bCustBase)
		{
			mi.bCustBase = TRUE;
			m_pProject->SetModuleItem(nModuleIndex, GET_ITEM_INDEX( lpItem ), mi);
		}
		else if (!bCustBase && mi.bCustBase)
		{
			mi.bCustBase = FALSE;
			m_pProject->SetModuleItem(nModuleIndex, GET_ITEM_INDEX( lpItem ), mi);
		}
	}

	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );
			MarkCustBaseNV(lpChild,bCustBase );
		}		
	}
}

BOOL CMySuperGrid::ParentHasCustBase(CTreeItem* pChild)
{
	if ( NULL == pChild)
	{
		return TRUE;
	}
	CTreeItem* pParent = GetParentItem(pChild);
	return HasCustBaseNV(pParent);
	
}

BOOL CMySuperGrid::ChildNotHasCustBase(CTreeItem* lpItem)
{
	BOOL bNoCustBase = TRUE;
	if (NULL == lpItem)
	{
		return FALSE;
	}
	
	if( ItemHasChildren( lpItem ) )
	{
		// Process child items,do not export items who have child items
		POSITION pos = GetHeadPosition( lpItem );
		while( pos )
		{
			CTreeItem* lpChild = GetNextChild( lpItem,pos );		
			bNoCustBase = ChildNotHasCustBase( lpChild);
			if (!bNoCustBase)
			{
				break;
			}
		}		
	}
	else
	{	
		ModuleItem mi = m_pProject->GetModuleItem( GetModuleIndexOfNode( lpItem ),GET_ITEM_INDEX( lpItem ) );	
		if(!mi.bCustBase)
		{
			bNoCustBase = FALSE;
		}		
		
	}
	return bNoCustBase;
	
}

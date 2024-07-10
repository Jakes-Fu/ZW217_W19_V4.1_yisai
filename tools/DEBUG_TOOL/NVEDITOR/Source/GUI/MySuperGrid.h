#if !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
#define AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySuperGrid.h : header file
//
#include "SuperGridCtrl.h"
#include "FileParser.h"

#include "NVValueFile.h"
#include "DlgMsg.h"
class CNewItemInfo;

#define PASTE_TYPE_CUT      0
#define PASTE_TYPE_COPY     1
#define PASTE_POS_AFTER     0
#define PASTE_POS_CHILD     1
/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid window

// an example of how to use the CSuperGridCtrl class.
#define COLOR_PINKISH_RED_TEXT	(RGB(255,0,255))
#define COLOR_RED_TEXT			(RGB(255,0,0))
#define COLOR_VIOLET_TEXT		(RGB(148,0,211))
#define COLOR_BLUE_TEXT			(RGB(0,0,255))

typedef void (WINAPI* PFCBSync)();
const TCHAR g_szNVFileHeader[100]	= _T( "// Spreadtrum nv export file version 1.0 \n\n" );

class CMySuperGrid : public CSuperGridCtrl
{
struct PASTE_INFO
{
    BOOL bValid;            
    int nPasteType;         // 0 means cut,1 means copy
    CString strItemPath;
};

// Construction
public:
	CMySuperGrid();
	
// Attributes
public:
// Operations
public:
	//HOWTO:
	void UpdateAllViews();
	void WINAPI SetSyncCBFunc(PFCBSync pSyncFunc) { m_pSyncCallBack = pSyncFunc;}
	BOOL ExportCustNV(LPCTSTR lpCustNVFile,BOOL bCustBase = FALSE);
	BOOL IsExistCustNV(BOOL bCustBase = FALSE);
	void InitializeGrid(void);
	void HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str);
	void HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC);
	void HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC);
	void HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void);
	void SortData(void);
	void _DeleteAll(void);
	void DynamicUpdateSomeItems(int nItem);
	void SetNewImage(int nItem);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;
	//search item and subitems 
	//usage:
	//	CTreeItem *pResult = Search("Item 1","subitem x","Subitem y","subitem 0", NULL); //must finish with NULL terminator thank you
	//  if(pResult!=NULL) .....bla. bla. bla.
	// returns the pointer to node that matches the search criteria or NULL if not found
	CTreeItem *Search(const CString& strItem, ...);
	CTreeItem* SearchEx(CTreeItem *pStartPosition, const CString& strItem);
	// Overrides
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySuperGrid)
	//}}AFX_VIRTUAL

	void OnFind(const CString& strTarget, const DWORD dwFlag);
	BOOL OnFindNext( CString strTarget,const  DWORD dwFlag);
    CTreeItem* Find(CTreeItem *pStartPosition, const CString& strItem,const DWORD dwFlag,BOOL &bDescript,BOOL bCheckFirst = TRUE);
    CTreeItem* DeepFind(CTreeItem *pStartPosition,const  CString& strItem,const DWORD dwFlag,BOOL &bDescript,BOOL bCheckFirst = TRUE);
	BOOL IsMatch(CTreeItem *pItem,const CString& strItem,const DWORD dwFlag, BOOL &bDescript);

	void UpdateItemID(CTreeItem* lpItem, UINT uNewID/*, int nCount*/);  // enable edit ID
public:
	virtual ~CMySuperGrid();

protected:
	CImageList m_image;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);
	// Generated message map functions

protected:

	// Function:
    //      获取给定ITEMTYPE的名称
    // 
    // Discription:  add by wei anli 03.06.12
    // -----------------------------------------------------------------------------
    BOOL CMySuperGrid::GetItemTypeName( // 已知类型， TRUE；未知，FALSE
        CString &strTypeName,           // [OUT] 类型的名称，为空表类型未知
        ITEMTYPE it                     // [IN]  类型(ENUM值)
    );

	//{{AFX_MSG(CMySuperGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInsertAfter();
	afx_msg void OnInsertBefore();
	afx_msg void OnModifyItemID();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInsertAsChild();
   	afx_msg void OnRemoveItem();
	afx_msg void OnSetReadonly();
	afx_msg void OnEraseReadonly();
	afx_msg void OnSetAppendable();
	afx_msg void OnEraseAppendable();
	afx_msg void OnModuleErasereadonly();
	afx_msg void OnModuleSetReadonly();
	afx_msg void OnCopyItem();
	afx_msg void OnPasteAfter();
	afx_msg void OnPasteAsChild();
	afx_msg void OnCutItem();
	afx_msg void OnExportItem();
	afx_msg void OnMarkAsCustNV();
	afx_msg void OnUnmarkAsCustNV();
	afx_msg void OnUnmarkAllChangeAsCustNV();
	afx_msg void OnMarkAllChangeAsCustNV();
	afx_msg void OnUnmarkAllCustNV();
	afx_msg void OnMarkAsCustbase();
	afx_msg void OnUnmarkAsCustbase();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Implementation
public:
	//Added by Leo.Feng
	void SetCustMode(BOOL bCustMode = FALSE){ m_bCustMode = bCustMode; }
    void CloseProject();
    void Refresh();
	void Invalidated(){Invalidate();}
	BOOL SetProject(CFileParser *pProject);
    void AddModule(CTreeItem *htPriItem, UINT nModuleIndex);
    void AddModuleItem(CTreeItem *htParentItem, UINT nModuleIndex, UINT *nModuleItemIndex,BOOL bFirstItem,BOOL bCustItems = FALSE);
    INT  GetSelectedModuleIndex();
	Module_Ptr GetSelectedModule();
    BOOL IsSelectedModule();

    BOOL LoadNVValue( int                         nNumberOfDirs,
                      const char                * lpszDirs,
                      int                         nNumberOfValueEntries,
                      const NVVALUE_TABLE_ENTRY * pValueEntry );

	void ImportItem( CString strFile,BOOL bCustItem = FALSE,BOOL bCustBase = FALSE);
	BOOL ImportItem( CStringArray &agItemValues, BOOL bOneOK = FALSE);
	BOOL TestItemChange(CStringArray &agItemValues, BOOL bOneOK = FALSE );
	void SetCustItem(CTreeItem *pItem,BOOL bCustItem=FALSE);
	void SetItemChanged(CTreeItem *pItem,BOOL bChanged = TRUE);
	void SetCustBaseItem(CTreeItem *pItem,BOOL bCustBase=FALSE);

	void SetDispHex(BOOL bHex) { m_bDispHex = bHex; Refresh();}

	void SetModifyID(BOOL bModify) {m_bModifyID = bModify;}  // Joly 

private:
	void ImportNVItem(LPCTSTR lpPath,LPCTSTR lpValue,int nLineNo,CDlgMsg& dlgMsg,BOOL bHasCustBase,BOOL bCustItem,BOOL bCustBase);
	void SetParent2CustBase(CTreeItem* pChild,BOOL bCustBase = FALSE);
	BOOL RetrieveItemInfo( CTreeItem   *pParentNode, 
		                   ModuleItem  *pNewItem   ,
						   ModuleItem  *pArrayItem );

	void InsertArrayItem( int                nIndex   ,
		                  const ModuleItem & miParent ,
						  ModuleItem &       miItem    );

	void InsertModuleItem( CTreeItem        * pParentNode,
						   int                nIndex,
						   ModuleItem       * pNewItem,
						   ModuleItem       * pArrayItem = NULL,
						   BOOL				  fAfter = TRUE,
                           BOOL               bExpandArray = TRUE
                         );
	int InsertModuleItemEx(int nParentIndex,int nModuleIndex,int nItemIndex,ModuleItem * pNewItem,BOOL fAfter,BOOL bUpdateTree = FALSE);

	int InsertStringItem( int                nIndex   ,
		                  const ModuleItem & miParent ,
						  ModuleItem &       miItem    );

	CTreeItem * GetLastItemOfTree( CTreeItem * pRoot );

	BOOL IsModuleNode( CTreeItem * pNode );
	BOOL IsProjectNode( CTreeItem * pNode );
    BOOL IsItemNode( CTreeItem * pNode );
		                   
    void SetModuleExpanded( CTreeItem * pModuleNode, BOOL fExpanded );
    BOOL IsModuleExpanded( CTreeItem * pModuleNode );

    void SetItemExpanded( int nModuleIndex, CTreeItem * pItemNode, BOOL fExpanded );
    BOOL IsItemExpanded( int nModuleIndex, CTreeItem * pItemNode );

    void ExpandModule( CTreeItem * pModuleNode );
    void ExpandItem( int nModuleIndex, CTreeItem * pItemNode );
    void ExpandProject( void );

    CTreeItem * GetModuleNodeFromIndex( int nModuleIndex );
    CTreeItem * GetItemNodeFromIndex( int nModuleIndex, int nItemIndex );
    CTreeItem * GetItemNodeFromIndex( CTreeItem * pRoot, int nItemIndex );

    CTreeItem * GetNodeByName( LPCTSTR     lpszName, 
                               CTreeItem * pRoot   ,
                               BOOL        fExact   );

    BOOL GetModuleIndexByName( LPCTSTR lpszName, int * pnModuleIndex );

    int GetModuleIndexOfNode( CTreeItem * pNode );

    typedef struct _NODE_INFO
    {
        CTreeItem * pDir;
        int         nModuleIndex;
    } NODE_INFO, *PNODE_INFO;

    void OnItemSelchanged(UINT nSelectItemIndex);
    void OnModuleSelchanged(UINT nSelectModuleIndex);
    void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
    CEdit* EditLabelEx(int nItem, int nCol);

    CFileParser * m_pProject;

    Module      m_hSelectedModule;
    UINT        m_nSelectedModuleIndex;
    ModuleItem  m_hSelectedItem;
    UINT        m_nSelectedItemIndex;

    BOOL  m_bModuleSelected;

    CTreeItem *m_pRootItem;

	CTreeItem *m_pCurFindItem;
	CTreeItem *m_pFindStartItem;
	BOOL  m_bDispHex;
	BOOL  m_bModifyID;
	BOOL  m_bCustMode;

//End of added by Leo.Feng

    //@# Kai Liu 2003.3.11
    // [
    void RemoveItem(CTreeItem*,BOOL bFresh);
    void RemoveLeafItem(CTreeItem*);
    // ]

    void GetNodePath( CTreeItem* lpItem,CString& strPath );
    void PasteItem( int nPastePos );
    void CopyPasteItem( CString& strDstPath,CString& strSrcPath,int nPastePos,CString& strNewName,int nNewId );
	void CopyPasteItemEx(int nParentIndex, CString& strSrcPath,int nPastePos,int nModuleIndex,int& nIndex,CString& strNewName,int nNewId);

	void ExportItem( CTreeItem*,CStdioFile& file, BOOL bReadOnly = FALSE, BOOL bChanged = FALSE,BOOL bCustNV = FALSE,BOOL bCustBaseNV = FALSE);
    BOOL CompareItem(LPCTSTR lpszOpt1, LPCTSTR lpszOpt2);
	

//@ Liu Kai 2005.7.25
protected:
	PFCBSync m_pSyncCallBack;	
    PASTE_INFO m_PasteInfo;
	unsigned int StrToInt(CString& strContent);   // Joly 8/27,2014
	BOOL HasCustNVItem(CTreeItem* lpItem);
	BOOL HasCustBaseNV(CTreeItem* lpItem);
	BOOL HasChangeItem(CTreeItem* lpItem,BOOL bCustNV = FALSE);
	void AddComments(CTreeItem* lpItem,CStdioFile& file);
	void MarkCustNV( CTreeItem* lpItem,BOOL bCustNV);
	void UpdatedTextColor(CTreeItem* lpItem);
	void MarkAllChangedCustNV( CTreeItem* lpItem,BOOL bCustNV);

	void MarkCustBaseNV(CTreeItem* lpItem,BOOL bCustBase);
	BOOL ParentHasCustBase(CTreeItem* pChild);
	BOOL ChildNotHasCustBase(CTreeItem* lpItem);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)

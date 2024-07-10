#if !defined(AFX_LISTCTRLEX_H__56E16197_DD76_4BC5_8E5A_30CAE9985B8D__INCLUDED_)
#define AFX_LISTCTRLEX_H__56E16197_DD76_4BC5_8E5A_30CAE9985B8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

#include "MySpin.h"
#include "MyCombox.h"
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:

    enum SHOW_TYPE
    {
        NOT_EDIT,
        ONLY_EDIT_TYPE,
        SPIN_EDIT_TYPE,
        COMBOX_TYPE,
    };
    typedef struct 
    {
        SHOW_TYPE m_enShowType;
        long m_lValue;
        CString m_strShowString;
        int m_nSpinLow;
        int m_nSpinHigh;
        BOOL m_bShowTitle;
        std::vector<CString>m_vComboxStrings;
        CInPlaceEdit::EDIT_TYPE m_enumEditType;
        double m_dReserved;
    }COLUMN_DATA, *PCOLUMN_DATA;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// [HitTestEx] - Determine the row index and column index for a point
	// Returns   - the row index or -1 if point is not over a row
	// point     - point to be tested.
	// col       - to hold the column index
	int HitTestEx(CPoint &point, int *pCol);

	//[DeleteItemEx] - Delete the selected item from the list
	void DeleteItemEx( int nIndex );

	//[GetCurSel] - Return the row number of the current selected item
	int GetCurSel(void) const { return m_nCurrentSelectionRow; }
    int GetCurSelCol(void) const { return m_nCurrentSelectionCol; }
	
	//[InsertColumn] - 
	int InsertColumn(int nCol, const LV_COLUMN* pColumn);

    //set spin edit's range
    void SetSpinRange( int _nLower, int _nUpper ) { m_nSpinLow = _nLower; m_nSpinHigh = _nUpper;}

    //sometimes need to show addtion infos in spin edit control
    void SetSpinTitle( const CString& _strTitle ) { m_strSpinTitle = _strTitle;    }
    
    // set display type of this cell . type include spin edit, combox, normal text 
    void SetShowType( SHOW_TYPE _enShowType) { m_enShowType = _enShowType;    }

    void SetInplaceEditType(CInPlaceEdit::EDIT_TYPE _enType ,int iFloatPrecision=1) 
    { 
        m_enumEditType = _enType;   
        m_iFloatPrecision = iFloatPrecision;
        m_editOnly.SetEditType(_enType);   
    }

    // if current cell has a spin edit type, then should set its sub type ,such as decimal number, hex number,etc
    void SetCurrentEditType( CInPlaceEdit::EDIT_TYPE _enumType ) { m_SpinEdit.SetEditType( _enumType );  }

    //if current cell has a commbox type, then should set its dropdown strings
    void SetComBoxStrings( const std::vector<CString>& _vStrings ){ m_vComboxStrings = _vStrings;    }

    void SetEditCheckRange(BOOL bCheckRange = FALSE, double dMaxValue=0.0, double dMinValue=0.0 )
    {
        m_editOnly.SetCheckRange( bCheckRange, dMaxValue,dMinValue );
    }
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	void DropDownComBox( int _nRow, int _nColnum );
	BOOL IsItemEmpty(int nItem);
	CMySpin m_SpinEdit;
    CMyCombox m_ComBox;
    CInPlaceEdit m_editOnly;

    //identify the precision of the value when the cell is float type
	int m_iFloatPrecision;
    // spin range
    int m_nSpinLow; 
    int m_nSpinHigh;

    // the row and column of current selected
	int m_nCurrentSelectionRow;
    int m_nCurrentSelectionCol;
    
    // title of spin edit control
    CString m_strSpinTitle;

    // drop down strings of combox control
    std::vector<CString>m_vComboxStrings;

    // cell's show type ( spin edit, combox, normal text etc
    SHOW_TYPE m_enShowType;
    
    //inplacement edit type
    CInPlaceEdit::EDIT_TYPE m_enumEditType;
    
    // [EditSpinLabel]	- show a edit and spin in the list position( row = nItem, column = nCol )	
    // Returns			- Temporary pointer to the new edit control
	// nItem			- The row index of the item to edit
	// nCol				- The column of the sub item.
	CWnd* EditSpinLabel(int nItem, int nCol);

	// [EditLabel]	- Start edit of a sub item label
	// Returns			- Temporary pointer to the new edit control
	// nItem			- The row index of the item to edit
	// nCol				- The column of the sub item.
 	CEdit* EditLabel( int nItem, int nCol );

	// Is pre item is empty (not exist or just empty)    
	BOOL IsPreItemEmpty(int nItem);   

	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__56E16197_DD76_4BC5_8E5A_30CAE9985B8D__INCLUDED_)

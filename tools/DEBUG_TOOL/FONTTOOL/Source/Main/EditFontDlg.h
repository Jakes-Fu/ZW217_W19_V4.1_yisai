#if !defined(AFX_EDITFONTDLG_H__F1437CE8_AD5B_44D8_8672_9929C9DC27E6__INCLUDED_)
#define AFX_EDITFONTDLG_H__F1437CE8_AD5B_44D8_8672_9929C9DC27E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditFontDlg.h : header file
//
#include "CoolStatic.h"
#include "CoolButton.h"
#define NUM_BAK 5
typedef struct _POINT_FONT
{
	CPoint point;
	int nVal;
}POINT_FONT;

typedef enum
{
	LEFT_KEY,
	RIGHT_KEY,
	UP_KEY,
	DOWN_KEY
}E_DERIECT_KEY;

typedef enum
{
	LT_POINT,
	RT_POINT,
	LB_POINT,
	RB_POINT
}E_DERIECT_POINT;

#define MAX_COLOR 12

/////////////////////////////////////////////////////////////////////////////
// CEditFontDlg dialog

class CEditFontDlg : public CDialog
{
// Construction
public:
	CEditFontDlg(CWnd* pParent = NULL);   // standard constructor

	void SetBtnBmp(CButton *pStc, UINT uID);
	CPoint GetStcPoint(CPoint point);
	CPoint GetRectPoint(CPoint point, DWORD dwFlag);
	void ModifyChar(CPoint point, int nVal);
	void MoveRect(DWORD dwFlag);
	void ReDrawFont(CRect oldRect, CRect newRect);
	void DeleteFont();
	void SaveDotMatrix();
	void GetMoveRect(CRect* rect);
	void AdjustRect(CRect* rect);
//	int IsOnEdge(CPoint point);
	void Undo();
	int GetBrsStcDmin();
	BOOL m_bIsPen;
	BOOL m_bIsErase;
	BOOL m_bIsUndo;
	BOOL m_bIsSelect;
	BOOL m_bIsSelectDown;
	BOOL m_bIsSelectMove;
	BOOL m_bIsSelectUp;
	BOOL m_bDraw;
	BOOL m_bMove;
	CPoint m_StartPoint;
	CPoint m_lastPoint;
	CRect m_FocusRect;
	CRect m_StcRect;
	HINSTANCE m_hinst;
	int m_nIndex;
	BYTE* m_pBakDot;
	BYTE* m_pModifyDot;
	int m_nSize;
	BOOL	m_bIsModifyDown;
	int m_dMin;
	int m_nTotalDX;
	int m_nTotalDY;
	int m_nRange;
	int m_lx;
	int m_ty;
	int m_rx;
	int m_by;
// Dialog Data
	//{{AFX_DATA(CEditFontDlg)
	enum { IDD = IDD_DLG_EDITFONT };
	CCoolStatic    m_csEditFont;
	CCoolStatic    m_csBrowse;
	CButton		   m_csPen;
	CButton		   m_csErase;
	CButton        m_csSelect;
	CButton		   m_btnUndo;
	CSliderCtrl	   m_sliderCtrl;
	int		m_nGrayIdx;
	//}}AFX_DATA

	CCoolButton    m_btnColor[MAX_COLOR];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFontDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditFontDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRdoColor(UINT nID);
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	void OnClickFont(UINT uID);
	DECLARE_MESSAGE_MAP()

private:
	int m_nDeltX;
	int m_nDeltY;
	
	BDF_CHAR_T m_bdfChar;



	void InitBdfChar();

	BOOL AbstractGlyph(PBDF_CHAR_T pChar);
	BOOL FillDotMatrix(PBDF_CHAR_T pChar);
	int  CopyBitsByPositon(LPBYTE lpbDst,int nDstSize,LPBYTE lpbPlus,int nPlusSize, int nPos);
	int  GetPtValue(LPBYTE lpbDotMatrix,int nFullW,int nX,int nY);
	BOOL SetBitByPose(LPBYTE lpbBitmap,int nFullW,int nX,int nY, int nPtValue);

	void InitColorCtrs();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFONTDLG_H__F1437CE8_AD5B_44D8_8672_9929C9DC27E6__INCLUDED_)

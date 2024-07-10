#if !defined(AFX_CHANNELIMPULESTC_H__7AB55814_0552_4A95_ACF3_A338A2E3BAFA__INCLUDED_)
#define AFX_CHANNELIMPULESTC_H__7AB55814_0552_4A95_ACF3_A338A2E3BAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelImpuleStc.h : header file
//
#define CHANNEL_IMPULSE_NUM 256
/////////////////////////////////////////////////////////////////////////////
// CChannelImpuleStc window

class CChannelImpuleStc : public CStatic
{
// Construction
public:
	CChannelImpuleStc();

// Attributes
public:

// Operations
public:
	void AddData(WORD wData, BOOL bClear);
	void Clear();
	
private:
	void DrawAxis(CDC *pDC, CRect rect);
	void DrawChart(CDC *pDC, CRect rect);
	void DrawArrowhead(CDC *pDC,CPoint point);

private:
	WORD m_wData[CHANNEL_IMPULSE_NUM];
	int m_nIndex;
	double m_dIdY;
	double m_dIdX;
	int m_nMaxValue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelImpuleStc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChannelImpuleStc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChannelImpuleStc)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELIMPULESTC_H__7AB55814_0552_4A95_ACF3_A338A2E3BAFA__INCLUDED_)

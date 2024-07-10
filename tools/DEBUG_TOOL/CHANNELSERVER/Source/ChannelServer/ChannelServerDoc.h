// ChannelServerDoc.h : interface of the CChannelServerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELSERVERDOC_H__13C4DCFE_3022_484B_9BB2_49732E452C11__INCLUDED_)
#define AFX_CHANNELSERVERDOC_H__13C4DCFE_3022_484B_9BB2_49732E452C11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChannelServerDoc : public CDocument
{
protected: // create from serialization only
	CChannelServerDoc();
	DECLARE_DYNCREATE(CChannelServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelServerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChannelServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChannelServerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELSERVERDOC_H__13C4DCFE_3022_484B_9BB2_49732E452C11__INCLUDED_)

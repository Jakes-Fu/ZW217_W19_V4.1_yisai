// CODEC_UTDoc.h : interface of the CCODEC_UTDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEC_UTDOC_H__8884A419_8FF1_48C0_99FB_455092B3AD8B__INCLUDED_)
#define AFX_CODEC_UTDOC_H__8884A419_8FF1_48C0_99FB_455092B3AD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCODEC_UTDoc : public CDocument
{
protected: // create from serialization only
	CCODEC_UTDoc();
	DECLARE_DYNCREATE(CCODEC_UTDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCODEC_UTDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCODEC_UTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCODEC_UTDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEC_UTDOC_H__8884A419_8FF1_48C0_99FB_455092B3AD8B__INCLUDED_)

// OwnershipDoc.h : interface of the COwnershipDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERSHIPDOC_H__267D64DF_8FDF_4BE0_B2EC_C59935CC224C__INCLUDED_)
#define AFX_OWNERSHIPDOC_H__267D64DF_8FDF_4BE0_B2EC_C59935CC224C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COwnershipDoc : public CDocument
{
protected: // create from serialization only
	COwnershipDoc();
	DECLARE_DYNCREATE(COwnershipDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnershipDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwnershipDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COwnershipDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERSHIPDOC_H__267D64DF_8FDF_4BE0_B2EC_C59935CC224C__INCLUDED_)

#if !defined(AFX_CITYQUHAOLISTVIEW_H__CF6708CA_F463_495E_8C2F_60F31B9FAABD__INCLUDED_)
#define AFX_CITYQUHAOLISTVIEW_H__CF6708CA_F463_495E_8C2F_60F31B9FAABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CityQuhaoListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCityQuhaoListView view
#include <afxtempl.h>
#include <vector>
#include <algorithm>

#define PROV_LEN 10
#define CITY_LEN 30
#define QUHAO_LEN 5

//#define PROV_FLAG 0
//#define DIRECT_CITY 1

typedef struct _CITY_QUHAO_T
{
	TCHAR szProvince[CITY_LEN];
	TCHAR szCity[CITY_LEN];
	UINT  uQuhao;
	_CITY_QUHAO_T()
	{
		memset(this,0,sizeof(_CITY_QUHAO_T));
	}
	_CITY_QUHAO_T(LPCTSTR lpszProvince,LPCTSTR lpszCity,UINT _uQuhao)
	{
		if(lpszProvince != NULL && _tcslen(lpszProvince)<30)
		{
			_tcscpy(szProvince,lpszProvince);
		}
		if(lpszCity != NULL && _tcslen(lpszCity)<30)
		{
			_tcscpy(szCity,lpszCity);
		}
		this->uQuhao = _uQuhao;
	}

}CITY_QUHAO_T,*CITY_QUHAO_PTR;

typedef struct _PROVINCE_T
{
	TCHAR szProvince[CITY_LEN];
	CArray<CITY_QUHAO_PTR,CITY_QUHAO_PTR> agCityQuhao;
	_PROVINCE_T()
	{
		memset(szProvince,0,sizeof(szProvince));
		agCityQuhao.RemoveAll();
	}

}PROVINCE_T,*PROVINCE_PTR;

typedef CMap<CString, LPCTSTR, PROVINCE_PTR, PROVINCE_PTR> MAP_PCQ;


class CCityQuhaoListView : public CListView
{
protected:
	CCityQuhaoListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCityQuhaoListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCityQuhaoListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCityQuhaoListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCityQuhaoListView)
	afx_msg void OnLoadCityQuhao();
	afx_msg void OnUpdateLoadCityQuhao(CCmdUI* pCmdUI);
	afx_msg void OnGenCityQuhao();
	afx_msg void OnUpdateGenCityQuhao(CCmdUI* pCmdUI);
	afx_msg void OnConvCityQuhaoMidToTxt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL LoadCityQuhaoText(LPCTSTR lpszFileName);
	BOOL GenerateCityQuhao(LPCTSTR lpszMidFileName);
	BOOL GenerateQuhaoCity(LPCTSTR lpszMidFileName);
	BOOL GenerateQuhao(LPCTSTR lpszMidFileName,LPCTSTR lpszCityMidFile);
	BOOL GenerateCity(LPCTSTR lpszMidFileName);
	void ReleaseCityInfo();

	std::vector<CITY_QUHAO_PTR> m_CityInfo;
//	CStringArray m_agCities;
	MAP_PCQ m_mapPcq;
	CStringArray m_agProvinces;


	BOOL ConvertCityQuhaoMid(LPCTSTR lpszMidFileName,LPCTSTR lpszTxtFileName);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CITYQUHAOLISTVIEW_H__CF6708CA_F463_495E_8C2F_60F31B9FAABD__INCLUDED_)

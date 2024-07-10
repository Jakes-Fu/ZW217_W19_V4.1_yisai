#ifndef _GRAPH_PROPS_H_
#define _GRAPH_PROPS_H_

#include "points.h"

class CAxisProps
{
    public:
	CAxisProps();
	~CAxisProps();

	void GetFormattedOutput(double value, int formatLevel, CString* str);
	void GetFullTitle(CString* str);

	void SetNewTitle(const TCHAR* title);
	void SetNewUOM(const TCHAR* newUOM);
	void SetNewPrecision(int newPrecision);

	const TCHAR* GetTitle(){return m_Title;};
	const TCHAR* GetUOM(){return m_UOM;};
	int GetPrecision(){return m_Precision;};

    protected:
	CString m_Title;
	CString m_UOM;
	int m_Precision;
	//format string for different formatLevel
	void CalculateFormatStrings();
	TCHAR* formatStrings[3];
};

class CGraphProps : public CPointsCollection
{
public:
	CGraphProps(COLORREF newColor, const TCHAR* newTitle, BOOL VisibleFlag = TRUE, 
		BOOL _bResources = TRUE, int _index = -1, 
		BOOL b_sort_x = FALSE, BOOL b_keep_same_x = TRUE);
	CGraphProps(CGraphProps* grprop, BOOL bCopyPoints = TRUE);
	virtual ~CGraphProps();

	void SetGraphProps(CGraphProps* grprop);
	
	COLORREF GetGraphColor(){return m_grcolor;};
	void SetGraphColor(COLORREF newColor);

	const TCHAR* GetTitle(){return (LPCTSTR)m_grTitle;};
	void SetTitle(const TCHAR* newTitle){m_grTitle = newTitle;};

	BOOL IsVisible(){return m_bIsVisible;};
	void SetVisible(BOOL bVisibleFlag){m_bIsVisible = bVisibleFlag;};

	int GetIndex(){return m_index;};
	void SetIndex(int _index){m_index = _index;};

    void SetCrossPoint(SSinglePoint& point){ pointCross.x = point.x; pointCross.y = point.y;};
    SSinglePoint *GetCrossPoint(){ return& pointCross;};
    
	BOOL AreResourcesPrivate(){return bPrivateResources;};
	CPen* GetPen()
	{
		CPen* new_pen = NULL;
		if (bPrivateResources)
		{
			new_pen = pen;
		} else
		{
			new_pen = new CPen(PS_SOLID, 1, m_grcolor);
		}
		return new_pen;
	};
    void ReleasePen(CPen* new_pen){if (!bPrivateResources) delete new_pen;};
    
    CBrush* GetBrush()
    {
        CBrush* new_brush = NULL;
        if (bPrivateResources)
        {
            new_brush = brush;
        } else
        {
            new_brush = new CBrush(m_grcolor);
        }
        return new_brush;
    };
    void ReleaseBrush(CBrush* new_brush){if (!bPrivateResources) delete new_brush;};
    
    protected:
        BOOL bPrivateResources;
        CPen* pen;
        CBrush* brush;
        COLORREF m_grcolor;
        CString m_grTitle;
        BOOL m_bIsVisible;
        int m_index;
        SSinglePoint pointCross;
};

typedef CGraphProps* PCGraphProps;

#endif
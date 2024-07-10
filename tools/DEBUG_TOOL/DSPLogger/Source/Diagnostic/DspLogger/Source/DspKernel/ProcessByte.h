// ProcessByte.h: interface for the CProcessByte class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSBYTE_H__3CB05D7A_4E37_4D29_A57B_A881D8C67FFB__INCLUDED_)
#define AFX_PROCESSBYTE_H__3CB05D7A_4E37_4D29_A57B_A881D8C67FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcessByte  
{
public:
	BYTE m_data[4];
    int  m_nData;
    BOOL m_bHeaderFound;
	BYTE m_time[4];
	BOOL m_bFullTime;
	int  m_nTimeCount;
	BOOL m_bLoseData;
	
public:
    CProcessByte(BOOL bNotFilterAddr = FALSE)
    {
		m_bNotFilterAddr = bNotFilterAddr;
        Reset();
    }
	
    BOOL ProcessByte(BYTE);
	BOOL ProcessTime(BYTE pTemp);
	
    void Reset()
    {
        m_nData			= 0;
        m_bHeaderFound	= FALSE;
		m_bFullTime		= TRUE;
		m_bLoseData		= FALSE;
		m_nTimeCount	= 0;
    }
	
    BYTE* GetData()
    {
        return m_data;
    }

	DWORD GetTime()
	{
		return ((DWORD)(m_time[0]) | ((DWORD)(m_time[1]) << 8)
			| ((DWORD)(m_time[2]) << 16) | ((DWORD)(m_time[3]) << 24));
	}
	
	BOOL IsFullTime()
	{
		return m_bFullTime;
	}
	
	BOOL IsLoseData()
	{
		return m_bLoseData;
	}
	
	BOOL IsFullData()
	{
		return  (m_nData == 0) ? TRUE : FALSE;
	}
private:
	BOOL m_bNotFilterAddr;

};

#endif // !defined(AFX_PROCESSBYTE_H__3CB05D7A_4E37_4D29_A57B_A881D8C67FFB__INCLUDED_)

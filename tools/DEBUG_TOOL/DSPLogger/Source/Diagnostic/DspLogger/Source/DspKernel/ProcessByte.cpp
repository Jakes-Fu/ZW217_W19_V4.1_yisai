// ProcessByte.cpp: implementation of the CProcessByte class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProcessByte.h"
#include "DspCommon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CProcessByte::ProcessByte(BYTE pTemp)
{

#ifndef DSPLOGGER_AUDIO
	
	if(!m_bHeaderFound)
	{
		// The first byte of the address must be D
		//teana hu 2009.11.27 for TD: the first byte of the address must be E
		if( !m_bNotFilterAddr && (pTemp & 0xf0) != 0xd0 && (pTemp & 0xf0) != 0xe0 && m_nData == 0 )
		{
			// just discard this byte
			m_data[m_nData++]	= 0xff;
			m_data[m_nData++]	= 0xff;
			m_data[m_nData++]	= 0xff;
			m_data[m_nData++]	= pTemp;
			m_nData				= 0;
			m_bHeaderFound		= FALSE;
			m_bLoseData			= TRUE;
			return TRUE;
		}
		else
		{
			m_bLoseData		= FALSE;
			m_bHeaderFound	= TRUE;
		}
	}
	
#endif	
	
	m_data[m_nData++] = pTemp;
	
	if(m_nData == TP_ITEM_SIZE)
	{
		m_nData			= 0;
		m_bHeaderFound	= FALSE;
		return TRUE;
	}        
	else
	{
		return FALSE;
	}
}

//teana hu 2009.05.18
BOOL CProcessByte::ProcessTime(BYTE pTemp)
{	
	if(m_nTimeCount < 4)
	{
		m_time[m_nTimeCount++]	= pTemp;
		m_bFullTime				= FALSE;
	}
	
	if(m_nTimeCount == 4)
	{
		m_nTimeCount = 0;
		m_bFullTime	 = TRUE;
		return TRUE;
	}        
	else
	{
		return FALSE;
	}
}

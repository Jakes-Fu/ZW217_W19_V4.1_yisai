// RasSpdM.h: interface for the CRasSpdM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RASSPDM_H__A025F0ED_F182_46C5_8F9B_D09B8971075F__INCLUDED_)
#define AFX_RASSPDM_H__A025F0ED_F182_46C5_8F9B_D09B8971075F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WINPERF.H"

#define TOTALBYTES    4096
#define BYTEINCREMENT 1024

class CRasSpdM  
{
public:
	CRasSpdM();
	virtual ~CRasSpdM();

public:
	void InitSpeedMonitor(void);
	void ClearSpeedMonitor(void);
	void GetRASSpeed(DWORD* pTT,DWORD* pRT,DWORD* pSP);

private:
	DWORD GetTotalRecv(void);
	DWORD GetTotalXmit(void);
	DWORD GetTotalSpeed(void);
	void GetRASSpeedOn9X(DWORD* pTT,DWORD* pRT,DWORD* pSP);
	void GetRASSpeedOnNT(DWORD* pTT,DWORD* pRT, DWORD* pSP);
	void GetNameStrings( );
	
	PPERF_COUNTER_DEFINITION NextCounter( PPERF_COUNTER_DEFINITION PerfCntr );
	PPERF_COUNTER_DEFINITION FirstCounter( PPERF_OBJECT_TYPE PerfObj );
	PPERF_INSTANCE_DEFINITION NextInstance( PPERF_INSTANCE_DEFINITION PerfInst );
	PPERF_INSTANCE_DEFINITION FirstInstance( PPERF_OBJECT_TYPE PerfObj );
	PPERF_OBJECT_TYPE NextObject( PPERF_OBJECT_TYPE PerfObj );
	PPERF_OBJECT_TYPE FirstObject( PPERF_DATA_BLOCK PerfData );

private:
	LPSTR m_lpNameStrings;
	LPSTR *m_lpNamesArray;
	BOOL glo_fFromDynReg;

};

#endif // !defined(AFX_RASSPDM_H__A025F0ED_F182_46C5_8F9B_D09B8971075F__INCLUDED_)

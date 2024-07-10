// RasSpdM.cpp: implementation of the CRasSpdM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mobilecnnt.h"
#include "RasSpdM.h"
#include <malloc.h>
#include <pdh.h>
#include <pdhmsg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRasSpdM::CRasSpdM()
{
	m_lpNameStrings=NULL;
	m_lpNamesArray=NULL;
	glo_fFromDynReg=TRUE;

}

CRasSpdM::~CRasSpdM()
{
	m_lpNameStrings=NULL;
	m_lpNamesArray=NULL;
}

DWORD CRasSpdM::GetTotalRecv(void)
{
	HKEY hKey;
	DWORD dwValue=(DWORD)-1;
	if(0 == RegOpenKey(HKEY_DYN_DATA,_T("PerfStats\\StatData"),&hKey))
	{
		DWORD dwType,dwLen=sizeof(DWORD);
		RegQueryValueEx(hKey,_T("Dial-Up Adapter\\TotalBytesRecvd"),NULL,&dwType,(BYTE*)&dwValue,&dwLen);
		RegCloseKey(hKey);
	}
	return dwValue;
}

DWORD CRasSpdM::GetTotalXmit(void)
{
	HKEY hKey;
	DWORD dwValue=(DWORD)-1;
	if(0 == RegOpenKey(HKEY_DYN_DATA,_T("PerfStats\\StatData"),&hKey))
	{
		DWORD dwType,dwLen=sizeof(DWORD);
		RegQueryValueEx(hKey,_T("Dial-Up Adapter\\TotalBytesXmit"),NULL,&dwType,(BYTE*)&dwValue,&dwLen);
		RegCloseKey(hKey);
	}
	return dwValue;
}

DWORD CRasSpdM::GetTotalSpeed(void)
{
	HKEY hKey;
	DWORD dwValue=(DWORD)-1;
	if(0 == RegOpenKey(HKEY_DYN_DATA,_T("PerfStats\\StatData"),&hKey))
	{
		DWORD dwType,dwLen=sizeof(DWORD);
		RegQueryValueEx(hKey,_T("Dial-Up Adapter\\TotalConnectSpeed"),NULL,&dwType,(BYTE*)&dwValue,&dwLen);
		RegCloseKey(hKey);
	}
	return dwValue;
}

void CRasSpdM::GetRASSpeedOn9X(DWORD* pTT,DWORD* pRT, DWORD* pSP)
{// under 9x
	*pTT=GetTotalXmit();
	*pRT=GetTotalRecv();
	*pSP=GetTotalSpeed();
}

/*****************************************************************
在NT下通过per Data得到
 *****************************************************************/

PPERF_OBJECT_TYPE CRasSpdM::FirstObject( PPERF_DATA_BLOCK PerfData )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfData + 
        PerfData->HeaderLength) );
}

PPERF_OBJECT_TYPE CRasSpdM::NextObject( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_OBJECT_TYPE)((PBYTE)PerfObj + 
        PerfObj->TotalByteLength) );
}

PPERF_INSTANCE_DEFINITION CRasSpdM::FirstInstance( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfObj + 
        PerfObj->DefinitionLength) );
}

PPERF_INSTANCE_DEFINITION CRasSpdM::NextInstance( 
    PPERF_INSTANCE_DEFINITION PerfInst )
{
    PPERF_COUNTER_BLOCK PerfCntrBlk;

    PerfCntrBlk = (PPERF_COUNTER_BLOCK)((PBYTE)PerfInst + 
        PerfInst->ByteLength);

    return( (PPERF_INSTANCE_DEFINITION)((PBYTE)PerfCntrBlk + 
        PerfCntrBlk->ByteLength) );
}

PPERF_COUNTER_DEFINITION CRasSpdM::FirstCounter( PPERF_OBJECT_TYPE PerfObj )
{
    return( (PPERF_COUNTER_DEFINITION) ((PBYTE)PerfObj + 
        PerfObj->HeaderLength) );
}

PPERF_COUNTER_DEFINITION CRasSpdM::NextCounter( 
    PPERF_COUNTER_DEFINITION PerfCntr )
{
    return( (PPERF_COUNTER_DEFINITION)((PBYTE)PerfCntr + 
        PerfCntr->ByteLength) );
}

/*****************************************************************
 *                                                               *
 * Load the counter and object names from the registry to the    *
 * global variable m_lpNamesArray.                                 *
 *                                                               *
 *****************************************************************/

void CRasSpdM::GetNameStrings( )
{
	
    HKEY hKeyPerflib;      // handle to registry key
    HKEY hKeyPerflib009;   // handle to registry key
    DWORD dwMaxValueLen;   // maximum size of key values
    DWORD dwBuffer;        // bytes to allocate for buffers
    DWORD dwBufferSize;    // size of dwBuffer
    LPSTR lpCurrentString; // pointer for enumerating data strings
    DWORD dwCounter;       // current counter index

// Get the number of Counter items.

    RegOpenKeyExA( HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib",
        0,
        KEY_READ,
        &hKeyPerflib);

    dwBufferSize = sizeof(dwBuffer);

    RegQueryValueExA( hKeyPerflib,
        "Last Counter",
        NULL,
        NULL,
        (LPBYTE) &dwBuffer,
        &dwBufferSize );

    RegCloseKey( hKeyPerflib );

// Allocate memory for the names array.

    m_lpNamesArray = (LPSTR*)malloc( (dwBuffer+1) * sizeof(LPSTR) );
	memset(m_lpNamesArray, 0, (dwBuffer+1) * sizeof(LPSTR));
// Open key containing counter and object names.

    RegOpenKeyExA( HKEY_LOCAL_MACHINE,
    "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009",
        0,
        KEY_READ,
        &hKeyPerflib009);

// Get the size of the largest value in the key (Counter or Help).

    RegQueryInfoKeyA( hKeyPerflib009,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &dwMaxValueLen,
        NULL,
        NULL);

// Allocate memory for the counter and object names.

    dwBuffer = dwMaxValueLen + 1;

    m_lpNameStrings = (LPSTR)malloc( dwBuffer * sizeof(char) );

// Read Counter value.

    RegQueryValueExA( hKeyPerflib009,
        "Counter",
        NULL,
        NULL,
        (BYTE*)m_lpNameStrings, &dwBuffer );
 //Load names into an array, by index.
	//test
	// Open the key.
	RegOpenKeyExA(HKEY_LOCAL_MACHINE,  
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\009", 0, KEY_READ, &hKeyPerflib009);
	// Get the size of the counter.
	RegQueryValueExA(hKeyPerflib009, "Counter", NULL, NULL, NULL, &dwBuffer);
	// Allocate memory for the buffer.
	if(m_lpNameStrings)
	{
		free(m_lpNameStrings);
	}
	m_lpNameStrings = (LPSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBuffer);
	if (!m_lpNameStrings) {RegCloseKey(hKeyPerflib009); return;}
	// Get the titles and counters.
	RegQueryValueExA(hKeyPerflib009, "Counter", NULL, NULL, (LPBYTE)m_lpNameStrings, &dwBuffer);

	//test
//    int iC=1;
	for( lpCurrentString = m_lpNameStrings; *lpCurrentString; lpCurrentString +=  strlen(lpCurrentString) + 1 )
    {
		//swscanf(lpCurrentString, _T("%d"), &dwCounter);
		dwCounter = atol(lpCurrentString);
        lpCurrentString += strlen(lpCurrentString) + 1;

		//printf("%d length=%d data=%s\n",iC++,dwCounter,lpCurrentString);
        m_lpNamesArray[dwCounter] = (LPSTR) lpCurrentString;
    }

	RegCloseKey(hKeyPerflib009);

}

/*****************************************************************
得到下RAS速度
 *****************************************************************/
void CRasSpdM::GetRASSpeedOnNT(DWORD* pTT,DWORD* pRT,DWORD* pSP)
{// under NT
    PPERF_DATA_BLOCK PerfData = NULL;
    PPERF_OBJECT_TYPE PerfObj;
    PPERF_COUNTER_DEFINITION PerfCntr;
    PPERF_COUNTER_BLOCK PtrToCntr;
    DWORD BufferSize = TOTALBYTES;
    DWORD i, j;

// Get the name strings through the registry.

	//return;
// Allocate the buffer for the performance data.

    PerfData = (PPERF_DATA_BLOCK) malloc( BufferSize );

    while( RegQueryValueEx( HKEY_PERFORMANCE_DATA,
                               _T("906"),//"Global",
                               NULL,
                               NULL,
                               (LPBYTE) PerfData,
                               &BufferSize ) == ERROR_MORE_DATA )
    {
    // Get a buffer that is big enough.

        BufferSize += BYTEINCREMENT;
        PerfData = (PPERF_DATA_BLOCK) realloc( PerfData, BufferSize );
    }

// Get the first object type.

    PerfObj = FirstObject( PerfData );

// Process all objects.

	for( i=0; i < PerfData->NumObjectTypes; i++ )
    {
    // Display the object by index and name.

    // Get the first counter.

        PerfCntr = FirstCounter( PerfObj );

        if( PerfObj->NumInstances > 0 )
        {
			//exit(0);
        }
        else
        {
        // Get the counter block.
            PtrToCntr = (PPERF_COUNTER_BLOCK) ((PBYTE)PerfObj +
                            PerfObj->DefinitionLength );

        // Retrieve all counters.

            for( j=0; j < PerfObj->NumCounters; j++ )
            {
				// Display the counter by index and name.
				if(m_lpNamesArray[PerfCntr->CounterNameTitleIndex] == NULL)
				{
					continue;
				}
				else if(strcmp("Bytes Transmitted",m_lpNamesArray[PerfCntr->CounterNameTitleIndex])==0)
				{
					*pTT=*((DWORD*)((BYTE*)PtrToCntr+PerfCntr->CounterOffset));
				}
                else if(strcmp("Bytes Received",m_lpNamesArray[PerfCntr->CounterNameTitleIndex])==0)
				{
					*pRT=*((DWORD*)((BYTE*)PtrToCntr+PerfCntr->CounterOffset));
				}
				else if(strcmp("Total Connections",m_lpNamesArray[PerfCntr->CounterNameTitleIndex])==0)
				{
					*pSP=*((DWORD*)((BYTE*)PtrToCntr+PerfCntr->CounterOffset));
				}
				// Get the next counter.
				PerfCntr = NextCounter( PerfCntr );
			}
		}

    // Get the next object type.
        PerfObj = NextObject( PerfObj );
    }

	if(PerfData != NULL)
	{
		free(PerfData);
		PerfData = NULL;
	}
}

void CRasSpdM::InitSpeedMonitor(void)
{
	DWORD dwVersion = GetVersion();
	// Get major and minor version numbers of Windows
//	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
//	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	// Get build numbers for Windows NT or Win32s
	if (dwVersion>>31 ==0)                // Windows NT
	{
		glo_fFromDynReg=FALSE;
		GetNameStrings( );
	}
	else         // Windows 95 - 98
		glo_fFromDynReg=TRUE;
}


void CRasSpdM::ClearSpeedMonitor(void)
{
	if(m_lpNameStrings != NULL)
	{
		HeapFree(GetProcessHeap(), 0, m_lpNameStrings);
	}
		
	if(m_lpNamesArray != NULL)
		free(m_lpNamesArray);
}

void CRasSpdM::GetRASSpeed(DWORD* pTT,DWORD* pRT,DWORD* pSP)
{
/*	if(glo_fFromDynReg==FALSE)
	{
		GetRASSpeedOnNT(pTT,pRT,pSP);
	}
	else
	{
		GetRASSpeedOn9X(pTT,pRT,pSP);
	}
*/
	if(glo_fFromDynReg)
	{
		GetRASSpeedOn9X(pTT,pRT,pSP);
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// use PDH to get RAS counter information
	PDH_STATUS         pdhStatus;
	HCOUNTER           phCounter1,phCounter2,phCounter3;
	DWORD              dwUserData = 0;
	HQUERY             hQuery = NULL;
	TCHAR              szCounterPath1[45]= TEXT("\\RAS Total\\Bytes Received");
	TCHAR              szCounterPath2[45]= TEXT("\\RAS Total\\Bytes Transmitted");
	TCHAR              szCounterPath3[45]= TEXT("\\RAS Total\\Total Connections");
	
	// Open a query.
	if( (pdhStatus = PdhOpenQuery( NULL, 0, &hQuery)) == ERROR_SUCCESS )
	{	
		// Add a counter.
		pdhStatus = PdhAddCounter( hQuery, szCounterPath1,dwUserData,&phCounter1);
		pdhStatus = PdhAddCounter( hQuery, szCounterPath2,dwUserData,&phCounter2);
		pdhStatus = PdhAddCounter( hQuery, szCounterPath3,dwUserData,&phCounter3);
		
		if( (pdhStatus = PdhCollectQueryData( hQuery)) == ERROR_SUCCESS)
		{		
			DWORD dwType = PDH_FMT_LONG;
			PDH_FMT_COUNTERVALUE pdhValue = {0};
			pdhStatus = PdhGetFormattedCounterValue( phCounter1, PDH_FMT_LONG, &dwType, &pdhValue);		
		
			if(pdhStatus == ERROR_SUCCESS && pdhValue.longValue!=0)
			{
				*pRT= (DWORD)pdhValue.longValue;
			}		
			memset(&pdhValue,0,sizeof(pdhValue));
			pdhStatus = PdhGetFormattedCounterValue( phCounter2, PDH_FMT_LONG, &dwType, &pdhValue);
			
			if(pdhStatus == ERROR_SUCCESS && pdhValue.longValue!=0)
			{
				*pTT= (DWORD)pdhValue.longValue;
			}
			
			memset(&pdhValue,0,sizeof(pdhValue));
			pdhStatus = PdhGetFormattedCounterValue( phCounter3, PDH_FMT_LONG, &dwType, &pdhValue);
					
			if(pdhStatus == ERROR_SUCCESS && pdhValue.longValue!=0)
			{
				*pSP= (DWORD)pdhValue.longValue;
			}
		}
	}

	if(hQuery)
	{
		PdhCloseQuery(hQuery);
		hQuery = NULL;
	}


	//////////////////////////////////////////////////////////////////////////
}

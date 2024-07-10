/*************************************************
Serial ports enumeration class implementations
*************************************************/
//////////////////////////////////////////////////////////////////////////
// WMI Enum modems and ports
//#define _USE_WMI
//#define _USE_LOG

#ifdef _USE_WMI

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include "StdAfx.h"

#endif  // _USE_WMI
//////////////////////////////////////////////////////////////////////////
#include <objbase.h>
#include <initguid.h>
#include <setupapi.h>
#include "EnumPorts.h"

#ifdef _USE_WMI

#include "windows.h"

typedef unsigned long * ULONG_PTR;
typedef long * LONG_PTR;

#include <comdef.h>
#include <atlbase.h>
#include <WbemCli.h>
#pragma comment(lib, "WbemUuid.Lib")
#endif // _USE_WMI

#ifdef _USE_LOG
#include "ISpLog.h"
#endif

#define PORT_CLASS_NAME		_T("Ports")
#define MODEM_CLASS_NAME	_T("Modem")
#define MODEM_GUID_STRING	_T("{4D36E96D-E325-11CE-BFC1-08002BE10318}")

CEnumPort::CEnumPort()
{
	m_bCheckCanOpen = false;
	m_pLog = NULL;
}

CEnumPort::~CEnumPort()
{
}


DEFINE_GUID(GUID_CLASS_PORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
DEFINE_GUID(GUID_CLASS_MODEM, 0x2c7089aaL, 0x2e0e, 0x11d1, 0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4);

// Enumerate the serial port
unsigned int CEnumPort::EnumPort(
				EnumPortInfo** lppPortInfo,
				unsigned int* lpCount,
				bool          bCheckCanOpen
				)
{
	m_bCheckCanOpen = bCheckCanOpen;
	Log(_T("EnumPort begin."));
	m_EnumPortInfo.clear();
	DWORD dwRet = 0;
	Log(_T("SetupAPI begin."));
	dwRet = EnumGUID((GUID*)&GUID_CLASS_PORT);	

	if(dwRet != 0)
	{
		Log(_T("EnumGUID PORT result [ErrCode:0x%x]."),dwRet);
	}

	dwRet = EnumGUID((GUID*)&GUID_CLASS_MODEM);

	if(dwRet != 0)
	{
		Log(_T("EnumGUID MODEM result [ErrCode:0x%x]."),dwRet);
	}
	Log(_T("SetupAPI end."));


#ifdef _USE_WMI
	Log(_T("WMIEnumDev begin."));
	if(WMIInit())
	{
		WMIEnumDev(_T("Win32_SerialPort"));
		WMIEnumDev(_T("Win32_POTSModem"));
		WMIUninit();
	}
	Log(_T("WMIEnumDev End."));
#endif

	Log(_T("REGEnumModem begin."));
	REGEnumModem();
	Log(_T("REGEnumModem End."));

	Log(_T("REGEnumPort begin."));
	REGEnumPorts();
	Log(_T("REGEnumPort End."));

	if( lppPortInfo )
	{
		*lppPortInfo = &m_EnumPortInfo[0];
	}

	if( lpCount )
	{
		*lpCount = m_EnumPortInfo.size();
	}

	Log(_T("EnumPort end, total num %d."),m_EnumPortInfo.size());
	return ERROR_SUCCESS;
}

unsigned int CEnumPort::EnumGUID(GUID * guid)
{
	GUID *guidDev = guid;
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;

	// Get Port class set
	// Note:We use DIGCF_PRESENT flag,so maybe you can see
	// some ports on the device manager,but they are not 
	// enumerated by SetupDiEnumDeviceInterfaces in the do-while
	// loop,because their driver are disabled,no application 
	// can open and use them.
	hDevInfo = SetupDiGetClassDevs( guidDev,
			NULL,
			NULL,
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
			);

	if(hDevInfo == INVALID_HANDLE_VALUE) 
	{
		return GetLastError();
	}

	SP_DEVICE_INTERFACE_DATA ifcData;
	ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	
	DWORD dwIndex = 0;

	EnumPortInfo epi;

	// Enumerate port and modem class device interfaces
	do
	{
		memset( &epi,0,sizeof(epi) );

		if( SetupDiEnumDeviceInterfaces(hDevInfo,NULL, guidDev, dwIndex, &ifcData) )
		{
			SP_DEVINFO_DATA devData;
			devData.cbSize = sizeof( SP_DEVINFO_DATA );
			
			if( !SetupDiGetDeviceInterfaceDetail( hDevInfo,&ifcData,NULL,0,NULL,&devData ) )
			{
				if( ERROR_INSUFFICIENT_BUFFER != GetLastError() )
				{
					// Can not get detail interface info
					continue;
				}
			}

			// Get Friendly name from registry
			SetupDiGetDeviceRegistryProperty( hDevInfo, &devData, SPDRP_FRIENDLYNAME, NULL,(PBYTE)epi.szFriendlyName, EPI_MAX_LEN*sizeof(TCHAR), NULL);
			// Get port description from registry
			SetupDiGetDeviceRegistryProperty( hDevInfo, &devData, SPDRP_DEVICEDESC, NULL,(PBYTE)epi.szDescription, EPI_MAX_LEN*sizeof(TCHAR), NULL);
			// Get class name from registry
			TCHAR szClass[EPI_MAX_LEN];
			SetupDiGetDeviceRegistryProperty( hDevInfo, &devData, SPDRP_CLASS, NULL,(PBYTE)szClass, EPI_MAX_LEN*sizeof(TCHAR), NULL);
			epi.nType = CheckDeviceClass( szClass );
			
			HKEY hDevKey = SetupDiOpenDevRegKey( hDevInfo,&devData,DICS_FLAG_GLOBAL,0,DIREG_DEV,KEY_READ );
			if( INVALID_HANDLE_VALUE != hDevKey )
			{	
				// Get port name
				DWORD dwCount = EPI_MAX_LEN;
				RegQueryValueEx( hDevKey,_T( "PortName" ),NULL,NULL,(BYTE*)epi.szPortName,&dwCount );
				RegCloseKey( hDevKey );
			}

			// Insert to port info array
			InsertPortInfo( epi );
		}
		else
		{
			if( ERROR_NO_MORE_ITEMS == GetLastError() )
			{
				// No more devices,jump out of the do-while loop
				break;
			}
		}
	}while( ++dwIndex );
	
	return ERROR_SUCCESS;
}

bool CEnumPort::InsertPortInfo( EnumPortInfo& epi )
{
	int nFriendlyName = _tcslen( epi.szFriendlyName ) * sizeof( TCHAR );
	int nPortName = _tcslen( epi.szPortName ) * sizeof( TCHAR );

	Log(_T("InsertPortInfo: [PortName]: %s [FriendlyName]: %s [PortType]: %s"),
		nPortName>0 ? epi.szPortName:_T(" "),
		nFriendlyName>0 ? epi.szFriendlyName:_T(" "),
		(EPIT_Port == epi.nType) ? _T("Port"): _T("Modem") );

	if( nPortName )
	{
		// Get port number from port name
		_stscanf( epi.szPortName,_T("COM%d"),&epi.nPortNum );
		if( 0 == epi.nPortNum )
		{
			// Not a valid port name
			nPortName = 0;
		}
	}
	

	if( !nPortName && nFriendlyName && EPIT_Port == epi.nType )
	{
		// Try to get port number from friendly name
		_stscanf( epi.szFriendlyName,_T("(COM%d)"),&epi.nPortNum );
		if( 0 == epi.nPortNum )
		{
			Log(_T("InsertPortInfo: port num is zero."));
			return false;
		}
	}

	if( 0 == epi.nPortNum )
	{
		// Can not get port number
		Log(_T("InsertPortInfo: port num is zero."));
		return false;
	}

	if( !nPortName )
	{
		// compose a valid port name
		nPortName = _stprintf( epi.szPortName,_T( "COM%d" ),epi.nPortNum ) * sizeof(TCHAR);
	}

	if( !nFriendlyName )
	{
		// compose a void friendly name
		nFriendlyName = _stprintf( epi.szFriendlyName,_T( "COM%d" ),epi.nPortNum ) * sizeof(TCHAR);
	}
	else
	{
		if( EPIT_Modem == epi.nType )
		{
			// Because the friendly name of the modem has no com port information,
			// so we append it to the end of the friendly name,if the name is too
			// long,it will be changed to "part_of_friendly_name...(COMX)"
			bool bCat = true;
			if( nPortName + nFriendlyName + 2 * sizeof( TCHAR ) > EPI_MAX_LEN - sizeof( TCHAR ) )
			{
				nFriendlyName = EPI_MAX_LEN - sizeof( TCHAR ) - nPortName - 5 * sizeof( TCHAR );
				if( nFriendlyName > 0 )
				{
					epi.szFriendlyName[ nFriendlyName ] = _T( '\0' );
					_tcscat( epi.szFriendlyName,_T("...") );
				}
				else
				{
					bCat = false;
				}
			}

			if( bCat )
			{
				_tcscat( epi.szFriendlyName,_T( "(" ) );
				_tcscat( epi.szFriendlyName,epi.szPortName );
				_tcscat( epi.szFriendlyName,_T( ")" ) );
			}
		}
	}
		
	bool bAdded =  InsertPortInfo2(epi,m_bCheckCanOpen);

	if(bAdded)
	{
		Log(_T("InsertPortInfo: [PortName]: %s added"),	epi.szPortName );
	}
	else
	{
		Log(_T("InsertPortInfo: [PortName]: %s not added"),	epi.szPortName );
	}

	return bAdded;
}

EPI_PORT_TYPE CEnumPort::CheckDeviceClass( TCHAR* lpszClass )
{
	if( !_tcscmp( lpszClass,PORT_CLASS_NAME ) )
	{
		return EPIT_Port;
	}
	else if( !_tcscmp( lpszClass,MODEM_CLASS_NAME ) )
	{
		return EPIT_Modem;
	}
	else
	{
		return EPIT_Unkown;
	}
}

// Find a port from the port list			
unsigned int CEnumPort::FindPort(						
					  const TCHAR* lpszPort,
					  bool bWholeName			
					  )
{
	if( NULL == lpszPort )
	{
		return 0;
	}

	for( unsigned int i=0;i<m_EnumPortInfo.size();i++ )
	{
		if( bWholeName )
		{
			// match the whole friendly name
			if( !_tcscmp( lpszPort,m_EnumPortInfo[i].szFriendlyName ) )
			{
				return m_EnumPortInfo[i].nPortNum;
			}
		}
		else
		{
			// search the input string in the friendly name
			if( _tcsstr( m_EnumPortInfo[i].szFriendlyName,lpszPort ) )
			{
				return m_EnumPortInfo[i].nPortNum;
			}
		}
	}

	return 0;
}

// Return the port info
unsigned int CEnumPort::GetPortInfo(
		EnumPortInfo** lppPortInfo	
		)
{
	if( lppPortInfo )
	{
		*lppPortInfo = &m_EnumPortInfo[0];
	}

	return m_EnumPortInfo.size();
}

bool CEnumPort::WMIInit()
{
#ifndef _USE_WMI
	return false;
#else
	//Initialize COM 
	CoInitialize(NULL);
	
	//Initialize COM security 
	HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hr))
	{		
		CoUninitialize();
		Log(_T("CoInitializeSecurity failed, [ErrCode:0x%x]."),hr);
		return false;  
	}
	else
	{
		return true;
	}
#endif
}
void CEnumPort::WMIUninit()
{
#ifdef _USE_WMI
	CoUninitialize();
#endif	
}

// Win32_SerialPort
// Win32_POTSModem
unsigned int CEnumPort::WMIEnumDev(TCHAR *lpszDevName)
{
#ifndef _USE_WMI
	lpszDevName;
	return 0;
#else
	EPI_PORT_TYPE nPortType = EPIT_Unkown;

	if(_tcsicmp(lpszDevName,_T("Win32_SerialPort")) == 0)
	{
		nPortType = EPIT_Port;
	}
	else if(_tcsicmp(lpszDevName,_T("Win32_POTSModem")) == 0 || _tcsicmp(lpszDevName,_T("Win32_POTSModemToSerialPort")) == 0)
	{
		nPortType = EPIT_Modem;
	}
	else
	{
		nPortType = EPIT_Unkown;
	}
	//What will be the return value
	BOOL bSuccess = FALSE;
	
	//Create the WBEM locator
	ATL::CComPtr<IWbemLocator> locator;
	HRESULT hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<void**>(&locator));
	if (SUCCEEDED(hr))
	{
		ATL::CComPtr<IWbemServices> services;
		hr = locator->ConnectServer(_bstr_t("\\\\.\\root\\cimv2"), NULL, NULL, NULL, 0, NULL, NULL, &services);
		if (SUCCEEDED(hr))
		{
			//Execute the query
			ATL::CComPtr<IEnumWbemClassObject> classObject;
			hr = services->CreateInstanceEnum(_bstr_t(lpszDevName), /*WBEM_FLAG_RETURN_IMMEDIATELY*/ WBEM_FLAG_RETURN_WBEM_COMPLETE, NULL, &classObject);
			if (SUCCEEDED(hr))
			{
				bSuccess = TRUE;
				
				//Now enumerate all the ports
				hr = WBEM_S_NO_ERROR;
				
				//Final Next will return WBEM_S_FALSE
				while (hr == WBEM_S_NO_ERROR)
				{
					ULONG uReturned = 0;
					ATL::CComPtr<IWbemClassObject> apObj[10];
					hr = classObject->Next(WBEM_INFINITE, 10, reinterpret_cast<IWbemClassObject**>(apObj), &uReturned);
					if (SUCCEEDED(hr))
					{
						for (ULONG n=0; n<uReturned; n++)
						{
							EnumPortInfo epi = {0};
							epi.nType = nPortType;
							ATL::CComVariant varProperty1;
							HRESULT hrGet = apObj[n]->Get(L"DeviceID", 0, &varProperty1, NULL, NULL);
							if (SUCCEEDED(hrGet) && (varProperty1.vt == VT_BSTR))
							{
								_tcscpy(epi.szPortName,	varProperty1.bstrVal);							
							}

							//Also get the friendly name of the port
							ATL::CComVariant varProperty2;
							if (SUCCEEDED(apObj[n]->Get(L"Name", 0, &varProperty2, NULL, NULL)) && (varProperty2.vt == VT_BSTR))
							{  
								_tcscpy(epi.szFriendlyName,	varProperty2.bstrVal);																		
							}

							//get port name for modem 
							if(nPortType == EPIT_Modem)
							{
								ATL::CComVariant varProperty3;
								if (SUCCEEDED(apObj[n]->Get(L"AttachedTo", 0, &varProperty3, NULL, NULL)) && (varProperty3.vt == VT_BSTR))
								{  
									_tcscpy(epi.szPortName,	varProperty3.bstrVal);																		
								}
							}
							InsertPortInfo(epi);
					
						}// for
					}//if (SUCCEEDED(hr))
				}//while
			}//if (SUCCEEDED(hr))		
		}//if (SUCCEEDED(hr))
	}//if (SUCCEEDED(hr))

	return 0;

#endif //_USE_WMI	
}

unsigned int CEnumPort::REGEnumPorts()
{	
	HKEY hSERIALCOMM;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_QUERY_VALUE, &hSERIALCOMM) == ERROR_SUCCESS)
	{
		//Get the max value name and max value lengths
		DWORD dwMaxValueNameLen;
		DWORD dwMaxValueLen;
		DWORD dwQueryInfo = RegQueryInfoKey(hSERIALCOMM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueNameLen, &dwMaxValueLen, NULL, NULL);
		if (dwQueryInfo == ERROR_SUCCESS)
		{
			DWORD dwMaxValueNameSizeInChars = dwMaxValueNameLen + 1; //Include space for the NULL terminator
			DWORD dwMaxValueNameSizeInBytes = dwMaxValueNameSizeInChars * sizeof(TCHAR);
			DWORD dwMaxValueDataSizeInChars = dwMaxValueLen/sizeof(TCHAR) + 1; //Include space for the NULL terminator
			DWORD dwMaxValueDataSizeInBytes = dwMaxValueDataSizeInChars * sizeof(TCHAR);
			
			//Allocate some space for the value name and value data			
			TCHAR *szValueName = new TCHAR[dwMaxValueNameSizeInChars];
			BYTE * byValue = new BYTE[dwMaxValueDataSizeInBytes];
			
			if (szValueName && byValue)
			{				
				//Enumerate all the values underneath HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM
				DWORD dwIndex = 0;
				DWORD dwType;
				DWORD dwValueNameSize = dwMaxValueNameSizeInChars;
				DWORD dwDataSize = dwMaxValueDataSizeInBytes;
				memset(szValueName, 0, dwMaxValueNameSizeInBytes);
				memset(byValue, 0, dwMaxValueDataSizeInBytes);
				LONG nEnum = RegEnumValue(hSERIALCOMM, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, byValue, &dwDataSize);
				while (nEnum == ERROR_SUCCESS)
				{
					//If the value is of the correct type, then add it to the array
					if (dwType == REG_SZ)
					{
						TCHAR* szPort = reinterpret_cast<TCHAR*>(byValue); 	
						
						if(_tcslen(szPort) > 3)
						{
							EnumPortInfo epi = {0};
							epi.nType = EPIT_Port;
							epi.nPortNum = _ttol(&szPort[3]);
							_stprintf(epi.szPortName,_T("COM%d"),epi.nPortNum);		
							_tcscpy(epi.szFriendlyName,szPort);
							if(epi.nPortNum > 0)
							{
								InsertPortInfo(epi);
							}
						}
					}						
					//Prepare for the next time around
					dwValueNameSize = dwMaxValueNameSizeInChars;
					dwDataSize = dwMaxValueDataSizeInBytes;
					memset(szValueName, 0, dwMaxValueNameSizeInBytes);
					memset(byValue, 0, dwMaxValueDataSizeInBytes);
					++dwIndex;
					nEnum = RegEnumValue(hSERIALCOMM, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, byValue, &dwDataSize);
				}
			}
			else
			{
				return ERROR_OUTOFMEMORY;
			}
			
			if(szValueName != NULL)
				delete [] szValueName;
			if(byValue != NULL)
				delete [] byValue;
		}
		
		//Close the registry key now that we are finished with it    
		RegCloseKey(hSERIALCOMM);	
	}
	
	return 0;
}

unsigned int CEnumPort::REGEnumModem()
{
	int nCount = GetModemCount();
	int i = 0;
	for(i=0;i<nCount;i++)
	{		
		int nPort = GetModemComPortByIndex(i);
		if(nPort != 0)
		{
			//TCHAR * pName = GetModemName(i);
			TCHAR * pFriendlyName = GetModemFriendlyName(i);
			if(pFriendlyName != NULL)
			{						
				EnumPortInfo epi = {0};
				epi.nType = EPIT_Modem;
				epi.nPortNum = nPort;
				_stprintf(epi.szPortName,_T("COM%d"),nPort);		
				_tcscpy(epi.szFriendlyName,pFriendlyName);
				delete [] pFriendlyName;
				InsertPortInfo(epi);	
			}
		}
	}

	return 0;
}

bool CEnumPort::IsPortIdle(int nPort)
{
	HANDLE hUart = INVALID_HANDLE_VALUE;
	
	// Combine the UART port parameter for CreateFile()
	_TCHAR szUart[20]={0};
	_stprintf( szUart, _T("\\\\.\\COM%d"), nPort );
	
	// Open the UART
	hUart = CreateFile(
		szUart,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL
		);

	if( INVALID_HANDLE_VALUE == hUart )  // Failed to open the UART
	{	 
		Log(_T("open port [COM%d] failed, ErrorCode:0x%X"),nPort,GetLastError());
		return false;
    }

	CloseHandle(hUart);
	return true;	
}

bool CEnumPort::InsertPortInfo2( EnumPortInfo& epi,bool bCheckIdle)
{
	if(epi.nPortNum == 0)
		return false;

	int nCount = m_EnumPortInfo.size();
	int i = 0;
	for(i = 0; i<nCount; i++)
	{
		if(m_EnumPortInfo[i].nPortNum == epi.nPortNum)
		{
			Log(_T("Port [COM%d] is already exist."),epi.nPortNum);
			return false;
		}
	}
	
	if(bCheckIdle)
	{
		if(IsPortIdle(epi.nPortNum))
		{
			m_EnumPortInfo.push_back( epi );
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_EnumPortInfo.push_back( epi );
	}

	return true;	
}

TCHAR* CEnumPort::GetProfileString( const TCHAR* lpszSection, const TCHAR* lpszEntry, const TCHAR* lpszDefault, HKEY hKey )
{
	//CString strValue = lpszDefault;

	TCHAR* pValue = NULL;
	pValue = new _TCHAR[_tcslen(lpszDefault)+1];
	if(pValue == NULL)
	{
		return NULL;
	}
	pValue[0] = '\0';
	_tcscpy(pValue,lpszDefault);
	
	// get section key
	HKEY hSectionKey = NULL;
	::RegOpenKeyEx( hKey, lpszSection, 0, KEY_READ, &hSectionKey );
	
	// if section does not exist, return default
	if ( hSectionKey != NULL )
	{
		// get desired entry
		DWORD dwType = 0;
		DWORD dwCount = 0;
		
		if ( ::RegQueryValueEx( hSectionKey, (TCHAR*)lpszEntry, NULL, &dwType, NULL, &dwCount ) == ERROR_SUCCESS )
		{
			if( dwType == REG_SZ )
			{
				delete [] pValue;
				pValue = new TCHAR[(dwCount / sizeof( TCHAR )) + 1];
				if(pValue == NULL)
				{
					return NULL;
				}
				memset(pValue,0, dwCount + sizeof(TCHAR));			
				if ( ::RegQueryValueEx( hSectionKey, (TCHAR*)lpszEntry, NULL, &dwType, (BYTE *)pValue, &dwCount ) != ERROR_SUCCESS )
				{
					delete [] pValue;
					pValue = new _TCHAR[_tcslen(lpszDefault)+1];
					if(pValue == NULL)
						return NULL;
					_tcscpy(pValue,lpszDefault);
				}
			}				
		}
		
		::RegCloseKey( hSectionKey );
	}
	
	return pValue;
}


TCHAR * CEnumPort::GetModemRegistrySection( const int nIndex )
{
	static TCHAR szResult[100] = {0};
	
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &osvi );

	TCHAR szIndex[10] = {0};
	if(nIndex != -1)
	{
		_stprintf(szIndex,_T("\\%04d"), nIndex);	
	}	
	
	if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		_stprintf(szResult,_T("SYSTEM\\CurrentControlSet\\Services\\Class\\Modem%s"),szIndex);
	}
	else
	{
		_stprintf(szResult,_T("SYSTEM\\CurrentControlSet\\Control\\Class\\%s%s"),MODEM_GUID_STRING,szIndex);	
	}
	
	return szResult;
}

int CEnumPort::GetModemCount()
{	
	TCHAR * pszSection = GetModemRegistrySection(-1); // need not release pszSection
	
	DWORD dwSubKeys = 0;
	HKEY hResult = NULL;
	long lRet = ::RegOpenKey( HKEY_LOCAL_MACHINE, pszSection, &hResult );
	if( ERROR_SUCCESS != lRet )
	{
		return dwSubKeys;	
	}
	
	RegQueryInfoKey( hResult, NULL, NULL, NULL
		, &dwSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL );	
	
	::RegCloseKey(hResult);

	return dwSubKeys;
}


int CEnumPort::GetModemComPortByIndex( const int nIndex )
{
	int nResult = 0;
	
	TCHAR *pPort =  GetProfileString( GetModemRegistrySection( nIndex ), _T("AttachedTo"), _T(""), HKEY_LOCAL_MACHINE );
	if ( _tcslen(pPort) !=  0 )
	{
		nResult = _ttol( pPort+3 ); // ie, COM3
	}	

	delete [] pPort;

	return nResult;
}

TCHAR * CEnumPort::GetModemName( const int nIndex )
{
	return GetProfileString( GetModemRegistrySection( nIndex ), _T("Model"), _T(""), HKEY_LOCAL_MACHINE );
}

TCHAR * CEnumPort::GetModemFriendlyName( const int nIndex )
{
	return GetProfileString( GetModemRegistrySection( nIndex ), _T("FriendlyName"), _T(""), HKEY_LOCAL_MACHINE );
}

void CEnumPort::SetLog(void * pLog)
{
	m_pLog = pLog;
}

void  CEnumPort::Log(LPCTSTR lpszFmt, ...)
{
#ifdef _USE_LOG
	if(m_pLog != NULL && lpszFmt != NULL)
	{
		TCHAR  szwBuffer[1024] = {0};
		va_list  args;
		va_start(args, lpszFmt);
		_vsntprintf(szwBuffer, sizeof(szwBuffer), lpszFmt, args);
		va_end(args);

		((ISpLog*)m_pLog)->LogFmtStr(SPLOGLV_INFO,szwBuffer);
	}
#else
	lpszFmt;
#endif
}

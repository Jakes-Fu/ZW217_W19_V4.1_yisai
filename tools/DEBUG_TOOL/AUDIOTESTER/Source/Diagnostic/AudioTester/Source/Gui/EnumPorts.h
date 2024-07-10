/*
Serial ports enumeration class definitions
*/

/***********************************************************
Update history

0.0.1           2009-8-14       liukai 
  The first version
  
  1 Enumerate all the serial ports installed on the 
    PC.
  
  2 Serial ports can be find by port number,friendly name(part
    or full name )
  
  
0.0.2			2009-8-27		liukai

  1 Fix a bug in FindPort function
  
  
0.0.3			2009-9-1		liukai

  1 Can be built in visual studio 2008

************************************************************/
#include <tchar.h>

#pragma warning( push, 3 )
#include <vector>
#pragma warning( pop )

using namespace std;

// Max string length used by the class
#define EPI_MAX_LEN 64

enum EPI_PORT_TYPE
{
	EPIT_Unkown,	// Unknown type
	EPIT_Port,		// Serial port
	EPIT_Modem,		// Modem
	EPIT_MAX
};

// The serial port information struct;
struct EnumPortInfo
{
	unsigned int nPortNum;				// Port number
	TCHAR szPortName[EPI_MAX_LEN];		// Short name for the port,in format like "COM X"
	TCHAR szFriendlyName[EPI_MAX_LEN];	// Friendly name for the port device
	TCHAR szDescription[EPI_MAX_LEN];	// Description of the port
	EPI_PORT_TYPE nType;				// Port type
};

class CEnumPort
{
public:
	CEnumPort();
	virtual ~CEnumPort();

	// Enumerate the serial port
	unsigned int EnumPort(				// 0 means success,otherwise return GetLastError 
		EnumPortInfo** lppPortInfo,		// Point to the port info array,can be NULL
		unsigned int*  lpCount,			// Count of the port info array,can be NULL
		bool  bCheckCanOpen=false       // True to try to open port.
	   );

	// Find a port from the port list			
	unsigned int FindPort(				// 0 means fail,otherwise return the port number
		const TCHAR* lpszPort,			// The friendly name,maybe part of it
		bool bWholeName					// true means exactly match
						 );

	// Return the port info
	unsigned int GetPortInfo(			// size of the port info array
		EnumPortInfo** lppPortInfo		// Pointer point to the port info array,can be NULL
		);

	// set log interface
	void SetLog(void * pLog);           //pLog is a pointer of ISpLog 

protected:
	// Add EnumPortInfo struct to the list
	bool InsertPortInfo( EnumPortInfo& );
	bool InsertPortInfo2( EnumPortInfo& epi,bool bCheckIdle);

	// To determine the port type from the device class name
	EPI_PORT_TYPE CheckDeviceClass( TCHAR* lpszClass );

	unsigned int EnumGUID(GUID * guid);

	//enum port with WMI
	bool WMIInit();
	void WMIUninit();
	unsigned int WMIEnumDev(TCHAR *lpszDevName);

	//enum modem from register
	unsigned int REGEnumModem();
	//enum port from register
	unsigned int REGEnumPorts();

	TCHAR* GetProfileString( const TCHAR*  lpszSection, const TCHAR*  lpszEntry, const TCHAR*  lpszDefault, HKEY hKey );
	TCHAR* GetModemName( const int nIndex );
	TCHAR* GetModemFriendlyName( const int nIndex );
	int    GetModemComPortByIndex( const int nIndex );
	int    GetModemCount();
	TCHAR* GetModemRegistrySection( const int nIndex );
	

	bool IsPortIdle(int nPort);

	void Log(LPCTSTR lpszFmt, ...);

protected:
	vector<EnumPortInfo> m_EnumPortInfo;
	bool                 m_bCheckCanOpen;

	void*                m_pLog;
};
#ifndef _IPORTHOUND_H__
#define _IPORTHOUND_H__

#include <tchar.h>

// Max string length used by the class
#define DEV_NAME_MAX_LEN 256

enum DEV_TYPE
{
	DT_UNKNOWN,	    // Unknown type
	DT_PORT,		// Serial port
	DT_MODEM,		// Modem
	DT_MAX
};

struct DEV_INFO
{
	unsigned int nPortNum;				    // Port number
	TCHAR szPortName[DEV_NAME_MAX_LEN];		// Short name for the port,in format like "COM X"
	TCHAR szFriendlyName[DEV_NAME_MAX_LEN];	// Friendly name for the port device
	TCHAR szDescription[DEV_NAME_MAX_LEN];	// Description of the port
	DEV_TYPE nType;				            // Port type
	unsigned int  bExist;
};

class IDevHound
{
public:
	virtual ~IDevHound() = 0;
    virtual void SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread = TRUE) = 0;
	virtual void Start()= 0;
	virtual void Stop()= 0;
	virtual bool GetPortName(unsigned int nPort,TCHAR szName[DEV_NAME_MAX_LEN])=0;
	virtual int  ScanPort(DEV_INFO **pPortInfo)=0;
	virtual void FreeMemory(void *pBuf)=0;
};

#ifdef  DEVHOUND_EXPORTS
	#define DEVHOUND_API extern "C" __declspec(dllexport)
#else
	#define DEVHOUND_API extern "C" __declspec(dllimport)
#endif

/**
 *  Create DevHound object, export function
 *  @param [out] pDevHound	: the pointer to hold the implement class object of IDevHound
 *  
 *  @return TRUE if create success,  FALSE otherwise
 */
DEVHOUND_API BOOL CreateDevHound( IDevHound ** pDevHound);
/**
 *  Release channel object, export function
 *  @param [out] pDevHound	: the pointer to hold the implement class of IDevHound
 *  
 *  @return void
 */
DEVHOUND_API void ReleaseDevHound( IDevHound *pDevHound);


// c api
DEVHOUND_API void SetReceiver(DWORD dwHandle, DWORD dwMsgID, BOOL bThread);
DEVHOUND_API void Start();
DEVHOUND_API void Stop();
DEVHOUND_API bool GetPortName(unsigned int nPort,TCHAR szName[DEV_NAME_MAX_LEN]);
DEVHOUND_API int  ScanPort(DEV_INFO **pPortInfo);
DEVHOUND_API void FreeMemory(void *pBuf);


#endif //_IPORTHOUND_H__
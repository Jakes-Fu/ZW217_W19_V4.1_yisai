#ifndef BASE_CHAN_H
#define BASE_CHAN_H


#include "ProtoChan.h"
#include "SpLog.h"

#pragma warning(push,3)
#include <list>
#include <vector>
#pragma warning(pop)

using namespace std;

typedef list< PRT_BUFF* > PRT_BUFF_LIST;

struct PRT_COND_T
{
	int ref_count;
	void* lpCond;
	void* lpSync;	// Synchronization object
	PRT_BUFF_LIST list;	
};

typedef vector< PRT_COND_T > COND_LIST;

#define INVALID_OBSERVER_ID 0xFFFFFFFF

typedef vector< IProtocolObserver* > OBSERVER_LIST;

class CBaseChannel : public IProtocolChannel,IProtocolObserver
{
public:
		// ctor and dtor
	CBaseChannel();
	virtual ~CBaseChannel();

#ifdef __SS__
    virtual BOOL    CreateChannel (ICommChannel **ppChannel, CHANNEL_TYPE PortType) = 0;
    virtual void    ReleaseChannel(ICommChannel   *pChannel) = 0;
#endif

public:
	// IProtocolChannel interface functions
	virtual BOOL	InitLog( LPCWSTR pszLogName, UINT uiLogType, 
			                 UINT uiLogLevel=INVALID_VALUE, ISpLog * pLogUtil=NULL,
							 LPCWSTR pszBinLogFileExt = NULL );   

	virtual BOOL	SetReceiver( ULONG  ulMsgId, 
				                 BOOL    bRcvThread,
                          	     LPCVOID pReceiver );   

	virtual void	GetReceiver( ULONG  &ulMsgId, 
			             BOOL &bRcvThread, 
			             LPVOID &pReceiver );     

	virtual BOOL	Open( PCCHANNEL_ATTRIBUTE pOpenArgument ); 

	virtual void	Close();    

	virtual BOOL	Clear();   

	virtual DWORD	Read( LPVOID lpData, DWORD dwDataSize, 
	                      DWORD dwTimeOut, DWORD dwReserved = 0 );    

	virtual DWORD	Write( LPVOID lpData, DWORD dwDataSize,DWORD dwReserved = 0  );  

	virtual void	FreeMem( LPVOID pMemBlock );  

	virtual BOOL	GetProperty( LONG lFlags, DWORD dwPropertyID, 
		                         LPVOID pValue );    

	virtual BOOL 	SetProperty( LONG lFlags, DWORD dwPropertyID, 
                                 LPCVOID pValue );

	virtual void    GetLowerChannel( ICommChannel** lpChannel , bool bProtocolChannel );
	virtual void    SetLowerChannel( ICommChannel* lpChannel, bool bProtocolChannel );

	virtual int     AddObserver( IProtocolObserver * lpObserver );
	virtual bool    RemoveObserver( int nObserverId );

	// IProtocolObserver functions
	virtual int		OnChannelEvent( unsigned int event,void* lpEventData );
	virtual int		OnChannelData( LPVOID lpData, ULONG reserved );

	friend unsigned int __stdcall RecvThread( void *lpArg );

protected:
	bool IsPhyicalChannel(){ return m_bPhysicalChannel; }
	bool IsInternalChannel(){ return m_bInternalChannel; }

	// Add a condition data to the list
	virtual int AddCond( void* lpCond );
	// Remove the given condition data from the list
	// if bForceRemove is true,all the packages attached to 
	// the condition will be released
	virtual bool RemoveCond( int nCondId,bool bForceRemove );
	// Return a package which has been attached to the given condition
	virtual PRT_BUFF* FindPackage( int nCondId );
	// Find a package satisfied the given condition in the package list
	virtual PRT_BUFF* FindPackage( PRT_READ_T* lpRead , bool bTryAgain = false);
	// Remove all packages in the package list that satisfied the given condition
	virtual void RemovePackages( void* lpCond );
	// Return true if the given package satisfied the given condition,otherwise return false
	virtual bool CheckPackage( PRT_BUFF* lpBuff,void* lpCond );
	// Compare two condition data
	virtual bool CompareCond( void* lpCond1,void* lpCond2,bool bRemoval = false );
	// Return the condition data from a given package
	virtual void* GetCondition( void* lpBuffer,unsigned int nSize,bool bRawData = true );
	// Copy a condition data
	virtual void* CopyCondition( void* lpCond );
	virtual void FreeCondition( void* lpCond );
	// To check if all packages satisfied the given condition is received
	virtual bool IsLastPackage( void* lpCond );
	// Load default configurations from int file
	// lpParam is the handle of the dll contain the derived class
	virtual void LoadConfig( void* lpParam );
	
	// Add a condition struct to condition list
	int AddCond( PRT_COND_T* lpCond );

	IProtocolObserver* GetNextObserver( int& nPos );
	int FindObserver( IProtocolObserver* lpObserver );

	unsigned long StartRecvThread();
	void StopRecvThread();

	unsigned long StartProcessThread();
	void StopProcessThread();

	void AddtoPackageList( PRT_BUFF** lpBuff ,int nCount);

	PRT_BUFF* GetPackage();

	void GenerateEndianEvent();

protected:
	void LockLowerChannel( bool bLock );
	void LockCondList( bool bLock );
	void LockPackageList( bool bLock );
	void LockObserverList( bool bLock );

	void WakeupOnPackages();
	void WaitForPackages();

	void WakeupOnResponse( void* lpSync );
	bool WaitForResponse( void* lpSync,unsigned int nTimeout );

public:
	void ProcessData();

	void SetThreadEvent();

protected:
	// If true,m_pLowerChannel is a ICommChannel pointer,otherwise it is a IProtocolChannel
	bool m_bPhysicalChannel;
	// If true means m_pLowerChannel is create by us,otherwise it is passed by SetLowerChannel
	bool m_bInternalChannel;
	int m_nObserverId;	
	COND_LIST m_CondList;
	PRT_BUFF_LIST m_pkgList;
	OBSERVER_LIST m_obsList;

	// The low level channel pointer,it is used to read data and write request
	// Low level channel may be a physical channel or a logical channel
	// If it is a physical channel,it will be a ICommChannel pointer,otherwise,it will be a IProtocalChannel pointer
	ICommChannel* m_pLowerChannel;
	IProtocolPackage* m_lpProtocolPackage;

	unsigned int m_nRecvThId;

	bool m_bProcessThreadRun;

	CSpLog m_Log;
	unsigned int m_uLogLevel;
	unsigned int m_uLogType;
	bool m_bInitLogCalled;
	TCHAR m_szLogName[MAX_PATH];

	PRT_BUFF_LIST::iterator m_itCurPackage;
	unsigned int m_nInternalPackgeCap;

protected:
	// Member variables for synchronization
	CRITICAL_SECTION m_csChannel;
	CRITICAL_SECTION m_csCond;
	CRITICAL_SECTION m_csPackage;
	CRITICAL_SECTION m_csObserver;

	// Event for read and receive functions
	HANDLE m_hProcessEvent;

	HANDLE m_hProcessThread;
	HANDLE m_hRecvThread;

	// Endian
	unsigned char m_nHiLvlEndian;
	unsigned char m_nLoLvlEndian;
	bool m_bEndianEvent;		// True means the channel just decided which endian is used,it will 
								// generate a endian event

	HANDLE m_hEvent; // use to wait a thread to run.
	bool   m_bClearKeepPkg;
};

PRT_BUFF* Alloc_PRT_BUFF( unsigned int size );

#endif 
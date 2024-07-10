#ifndef  PROTOCHAN_H
#define PROTOCHAN_H

#include <windows.h>
#include "ICommChannel.h"

typedef void (*prt_buff_free_f)(void*);

struct PRT_BUFF
{
	int type;					// buffer type
	int ref_count;				// reference count of the buffer
	int size;					// buffer size
	int reserved;				// no use
	prt_buff_free_f free_prt;	// function pointer used to free this buffer
	unsigned char* lpData;		// buffer data
};

struct PRT_READ_T
{
	int nCond;				// Condition id returned by previous Write
	void* lpCond;			// Condition data pointer
	PRT_BUFF** lppBuff;		// pointer to contain returned package
};

enum PRT_WRITE_ACT
{
	PRT_WRITE_normal,		// Write a command,need to call Read to read response
	PRT_WRITE_no_respond,	// Write a command without response
	PRT_WRITE_clear,		// Write a command,need to call Read to read response,
							// clear all packages satisfied the condition before call Read
};

struct PRT_WRITE_T
{
	int action;				// Write action	
	int nCond;				// Condition id returned if a Read will be called after Write
	void* lpProtocolData;	// The data to be write
};

struct PACKAGE_INFO
{
	char nTotalPackages;
	char nValidPackages;
	short bInvalidLen : 1;
	short bNoTail : 1;
	short bCrcError : 1;
	short Reserved : 13;
};

enum
{
	PP_LITTLE_ENDIAN,		// Little endian
    PP_BIG_ENDIAN,			// Big endian
	PP_UNKOWN_ENDIAN,		// The endian will be decided at runtime
};

enum
{
	PP_LOG_NONE,			// No log data
	PP_LOG_ERROR,			// Only log error information
	PP_LOG_WARN,			// Log error and warnings
	PP_LOG_INFO,			// Log runtime information
	PP_LOG_VERBOSE,			// Log all information
};

enum
{
	PP_EVENT_CHANNLE_DISCON,		// Physical channel disconnect
	PP_EVENT_CHANNLE_CONNECT,
	PP_EVENT_DEVICE_REMOVED,		// Communication device is removed,such as usb device
	PP_EVENT_DEVICE_PLUGIN,         // Communication device is plugged in,such as usb device
	PP_EVENT_LITTLE_ENDIAN,			// The channel decide that data from lower channel is little endian
	PP_EVENT_BIG_ENDIAN,			// The channel decide that data from lower channel is big endian
	PP_EVENT_FILE_READ_OVER,		// The channel is file type and all data is sent

	PP_EVENT_USER_BASE = 256,			
};

enum Package_Property_ID
{
	PPI_Endian,		// Input and output endian property
	PPI_GetError,	// Get error code
	PPI_WATCH_DEVICE_CHANGE, // watch the device plug-in or plug-out, such as USB.
	PPI_INTERNAL_PACKAGE_CAPACITY,	// internal package buffer capacity
	PPI_CLEAR_KEEP_PACKAGE,

	PPI_USER_BASE = 256,
};

#define  CONVERT_SHORT(Src,Dst) {(Dst) = MAKEWORD(HIBYTE(Src),LOBYTE(Src));}


#define  CONVERT_INT(Src,Dst)   {\
                                 (Dst)  = MAKELONG(MAKEWORD(HIBYTE(HIWORD(Src)),LOBYTE(HIWORD(Src))),\
                                                   MAKEWORD(HIBYTE(LOWORD(Src)),LOBYTE(LOWORD(Src))));\
                                }


class IProtocolObserver
{
public:
	/**
	*  Called when some channel event happened
	*  @param [in] event		: event id
	*  @param [in] lpEventData	: event data pointer( a PRT_BUFF pointer),different event has different event data,can be null
	*
	*  @return value is not used now
	*/
	virtual int OnChannelEvent( unsigned int event,void* lpEventData ) = 0;

	/**
	*  Called when some data packages received
	*  @param [in] lpData		: data pointer( a PRT_BUFF pointer )
	*  @param [in] ulDataSize	: data size
	*
	*  @return value is not used now
	*/
	virtual int OnChannelData(LPVOID lpData, ULONG reserved ) = 0;
};

class IProtocolChannel : public ICommChannel
{
public:
	/**
	*  Get lower channel pointer
	*  @param [in/out] lpChannel	: pointer to contain lower channel 
	*  @param [in] bProtocolChannel	: true, return lower IProtocolChannel,false,return lower ICommChannel
	*/
	virtual void GetLowerChannel( ICommChannel** lpChannel , bool bProtocolChannel ) = 0;

	/**
	*  Set lower channel pointer
	*  @param [in] lpChannel		: pointer to lower channel 
	*  @param [in] bProtocolChannel	: true, lpChannel is IProtocolChannel,false,lpChannel is ICommChannel
	*/
	virtual void SetLowerChannel( ICommChannel* lpChannel,bool bProtocolChannel ) = 0;

	/**
	*  Add a Observer
	*  @param [in] lpObserver		: pointer to IProtocolObserver
	*
	*  @return internal observer id,used for calling RemoveObserver
	*/
	virtual int AddObserver( IProtocolObserver * lpObserver ) = 0;

	/**
	*  Remove a Observer
	*  @param [in] nObserverId		: Id of the observer to be removed
	*
	*  @return true means success,otherwise false
	*/
	virtual bool RemoveObserver( int nObserverId ) = 0;
};


class IProtocolPackage
{
public:
	/**
	*  Depackage a block of buffer
	*  @param [in] lpBuffer	: input buffer pointer
	*  @param [in] uSize	: input buffer size
	*
	*  @return how many packages found
	*/
	virtual unsigned int Append( void* lpBuffer,unsigned int uSize ) = 0;
	
	/**
	*  Clear internal buffer
	*/
    virtual void Clear() = 0;
  
	/**
	*  Get packages found in Append
	*  @param [out] lpPackage	: pointer to a buffer to contain returned package pointers
	*  @param [in]	size		: size of lpPackage
	*
	*  @return how many packages returned
	*/
    virtual unsigned int GetPackages( void* lpPackage,int size ) = 0;
	
	/**
	*  Free memory allocated in Append and Package
	*  @param [out] lpMemBlock	: pointer to a buffer to be freed
	*/
	virtual void FreeMem( void* lpMemBlock ) = 0;

	/**
	*  Package input data to a protocol package
	*  @param [in]  lpInput		: pointer to a buffer to be packaged
	*  @param [in]	nInputSize	: size of lpInput
	*  @param [out]	lppMemBlock	: Pointer to a buffer contains the package
	*  @param [out]	lpSize		: Pointer to a buffer contains size of the output package
	*
	*  @return 0 means success,otherwise return error code
	*/
	virtual unsigned int Package( void* lpInput,int nInputSize,void** lppMemBlock,int* lpSize ) = 0;
  
	/**
	*  Get property of this program
	*  @param [in] lFlags		: reserved
	*  @param [in] dwPropertyID: property name
	*  @param [out] pValue		: property value pointer
	*  
	*  @return TRUE if get success,  FALSE otherwise
	*/
    virtual bool GetProperty( long lFlags,unsigned int dwPropertyID,void* lpValue ) = 0;

	/**
	*  Set property of this program
	*  @param [in] lFlags		: reserved
	*  @param [in] dwPropertyID: property name
	*  @param [in] pValue		: property value pointer
	*  
	*  @return TRUE if set success,  FALSE otherwise
	*/
    virtual bool SetProperty( long lFlags,unsigned int dwPropertyID,const void* lpValue ) = 0;  
};

#endif
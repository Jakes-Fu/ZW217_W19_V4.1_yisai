#ifndef _BOOTMODEITF_H__
#define _BOOTMODEITF_H__

class IBMOprObserver
{
public:
    /**
    * Invoked when start work at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param dwResult :show  whether the start work is success .
    *
    * @return The return value is ignored.
    */
     virtual HRESULT OnStart( DWORD dwOprCookie,
		                      DWORD dwResult ) = 0;     
    
    /**
    * Invoked when end work at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param dwResult :show  whether the end work is success .
    *
    * @return The return value is ignored.
    */
     virtual HRESULT OnEnd( DWORD dwOprCookie, 
		                    DWORD dwResult ) = 0;      
 
    /**
    * Invoked when start some operation at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param bstrFileID: identifiers of file .
    *
    * @param cbstrFileType : show the current file type,such as FDL, NV.
    *
    * @param cbstrOperationType: show the current operation type,such as Dowmload, ReadFlash.
    * 
    * @param pBMFileInterface: the pointer of BMFile Interface.
    *
    * @return The return value is ignored.
    */
     virtual HRESULT OnOperationStart( DWORD dwOprCookie, 
								       LPCWSTR cbstrFileID,
		                               LPCWSTR cbstrFileType,		
    				                   LPCWSTR cbstrOperationType,
                                       LPVOID pBMFileInterface ) = 0;     
    
    /**
    * Invoked when end some operation at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param bstrFileID: identifiers of file .
    *
    * @param cbstrFileType : show the current file type,such as FDL, NV.
    *
    * @param cbstrOperationType: show the current operation type,such as Dowmload, ReadFlash.
    *
    * @param dwResult :show  whether the operation is success .
    * 
    * @param pBMFileInterface: the pointer of BMFile Interface.
    *
    * @return Returns S_OK if successful,Otherwise returns S_FAIL;
    */    
     virtual HRESULT OnOperationEnd(  DWORD dwOprCookie, 
								      LPCWSTR cbstrFileID,
								      LPCWSTR cbstrFileType, 
    							      LPCWSTR cbstrOperationType, 
								      DWORD dwResult,
								      LPVOID pBMFileInterface ) = 0;  

    /**
    * Invoked when start some file operation at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param bstrFileID: identifiers of file .
    *
    * @param cbstrFileType : show the current file type,such as FDL, NV.
    * 
    * @param pBMFileInterface: the pointer of BMFile Interface.
    *
    * @return Returns S_OK if successful,Otherwise returns S_FAIL;
    */    
    virtual HRESULT OnFileOprStart( DWORD   dwOprCookie,
							        LPCWSTR cbstrFileID,
							        LPCWSTR cbstrFileType, 
                                    LPVOID   pBMFileInterface )= 0;
    
    /**
    * Invoked when end some file operation at boot mode platform.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param bstrFileID: identifiers of file .
    *
    * @param cbstrFileType : show the current file type,such as FDL, NV.
    *
    * @return Returns S_OK if successful,Otherwise returns S_FAIL;
    */    
    virtual HRESULT OnFileOprEnd( DWORD dwOprCookie,
						          LPCWSTR cbstrFileID,
							      LPCWSTR cbstrFileType, 
							      DWORD dwResult )= 0;				  
 
	/**
    * Invoked when prepare some file operation at boot mode application framework.
    *
    * @param dwOprCookie: identifiers of working channel .
    *
    * @param bstrProduct: identifiers of product name .
    *
    * @param bstrFileName: file path to download .
    *
    * @param lpFileInfo : a pointer to FILE_INFO_T struct.
    *
    * @param pBMFileInfoArr: return the pointer of BMFileInfo structs .
    *
    * @param lpBMFileInfoCount: return the number of BMFileInfo structs .
    *
    * @param lpdwFlag: if nothing is done return 0, else return 1.
    *                  
    * @return Returns S_OK if successful,Otherwise returns S_FAIL;
    */    
    virtual HRESULT OnFilePrepare(     DWORD   dwOprCookie,
							           LPCWSTR bstrProduct,
							           LPCWSTR bstrFileName,
							           LPVOID  lpFileInfo,                   
							 /*[out]*/ LPVOID  pBMFileInfoArr,
							 /*[out]*/ LPDWORD lpBMFileInfoCount,
							 /*[out]*/ LPDWORD lpdwFlag )= 0; 	

};

class IBMSettings
{
public:
	virtual ~IBMSettings(){};
	/**
	 * Set retry times of CheckBaud operation.
	 * 
     * @param dwTimes   Retry times,0 means infinite
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */    
    virtual HRESULT SetCheckBaudTimes( DWORD dwTimes ) = 0;

    /**
	 * Set repartition strategy flag
     *
     * @param dwFlag    strategy flag
     * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT  SetRepartitionFlag (  DWORD dwFlag ) = 0;  
    
	/**
	 * Set read flash before repartition flag
     *
     * @param dwFlag    strategy flag
     * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT  SetReadFlashBefRepFlag( DWORD dwFlag ) = 0;
     
    /**
	 * Get packet length of file type
     *
     * @param bstrFileType    file type
     * 
     * @return Returns packet length;
     */
    virtual DWORD GetPacketLength ( LPCWSTR bstrFileType ) = 0;

	/**
     * Get a property value of. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. 
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pvarValue:This parameter cannot be NULL and must point to an uninitialized
     * VARIANT. If no error is returned, the VARIANT is initialized using VariantInit,
     * and then set to contain the property value.  The caller must call VariantClear 
     * on this pointer when the value is no longer required. If an error code is returned, 
     * the VARIANT pointed to by pValue is left unmodified.
     *
     * @return Returns BM_S_OK if successful,Otherwise returns S_FAIL;
     */
     virtual HRESULT GetProperty ( LONG  lFlags, LPCWSTR cbstrName,  LPVOID*  pvarValue ) = 0;

    /**
     * Put a property value. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. 
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pcvarValue: Points to a VARIANT that is treated as read-only. 
     * The value in the VARIANT becomes the named property value.
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
     virtual HRESULT SetProperty ( LONG lFlags,LPCWSTR cbstrName,  LPCVOID pcvarValue ) = 0;
  
};

/**
 * IBootModeHandler interface defines the integrity operations at Boot mode.
 * 
 *
 */
class IBootModeHandler: public IBMSettings
{     
public:
	virtual ~IBootModeHandler(){};
    /**
     * Start the operation at boot mode
     *
     * @param lpDLFileInfo: the array of Download files information; 
     * @param pOpenArgument: the argument of open the channel communication
     * @param lpbstrProgID: the prog id of implement the interface of the channel communication
     * @param bRcvThread : Thread or window
     * @param pReceiver : Set the up-level application' receiver which receives the message
     *                    If bRcvThread is TRUE, pReceiver is a thread ID;
     *                    Otherwise a window handle
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */    
    virtual HRESULT StartBootModeOperation( /*[in]*/ LPVOID  lpDLFileInfo, 
									        /*[in]*/ UINT    uFileCount, 
									        /*[in]*/ LPVOID  pOpenArgument, 
									        /*[in]*/ BOOL    bBigEndian, 
									        /*[in]*/ DWORD   dwOprCookie,
		                                    /*[in]*/ BOOL    bRcvThread,  
		                                    /*[in]*/ LPCVOID pReceiver ,
				                            /*[in]*/ LPCWSTR lpbstrProgID )=0;				  
    /**
     * Stop the operation at boot mode
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT StopBootModeOperation() = 0;
				        
    /**
     * In factory, when the current chip have finished, it'll wait for next chip 
     *
     * @param dwWaitTime: the wait time (in milliseconds).
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */				  
    virtual HRESULT SetWaitTimeForNextChip(/*[in]*/ DWORD dwWaitTime) = 0;

    /**
     * Set the communication channel pointer
     *
     * @param pCommunicateChannel: point to the communication channel.
     *
     * @return The return value is ignored.
     */				  
    virtual HRESULT SetCommunicateChannelPtr( /*[in]*/LPVOID pCommunicateChannel )= 0;

    /**
     * get the communication channel pointer
     *
     * @param ppCommunicateChannel: point to the communication channel pointer.
     *
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */				  
    virtual HRESULT GetCommunicateChannelPtr( /*[out]*/LPVOID* ppCommunicateChannel )= 0;

		/**
	 * Subscribe an observer object to receive notification . 
	 *
	 * @param pSink Points to the sink object which will receive the notification.
	 *
	 * @param uFlags :reserved
	 *
	 * @param lpdwCookie Points to the cookie assigned to the sink, which can be used to unregister the sink.
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */
    virtual HRESULT SubscribeOperationObserver ( IBMOprObserver* pSink,
					                             ULONG uFlags,
					                             LPDWORD lpdwCookie ) = 0;

	/**
	 * Unsubscribe an observer object subscribed previously with SubscribeOperationObserver().
	 * 
	 * @param dwCook The cookie returned by SubscribeOperationObserver().
	 * 
     * @return Returns S_OK if successful,Otherwise returns S_FAIL;
     */    
    virtual HRESULT UnsubscribeOperationObserver ( DWORD dwCookie) = 0;

		/**
	 * Get the read buffer from boot mode platform.
	 * 
     * @return : point to the read buffer.
     */    
    virtual const LPBYTE GetReadBuffer ( ) = 0;

    /**
	 * Get the read buffer from boot mode platform.
     *
     * @return : read buffer size
     */
    virtual DWORD GetReadBufferSize( ) = 0;    

    
};



#ifdef BMP_DLL
#define BMP_EXPORT_CLASS __declspec(dllexport)
#define BMP_EXPORT_API extern "C" __declspec(dllexport)
#else
#define BMP_EXPORT_CLASS __declspec(dllimport)
#define BMP_EXPORT_API extern "C" __declspec(dllimport)
#endif //BMAF_DLL

BMP_EXPORT_API BOOL CreateBMObject( IBootModeHandler ** pBMObj);
BMP_EXPORT_API void ReleaseBMObject( IBootModeHandler * pBMObj);

#endif //_BOOTMODEITF_H__
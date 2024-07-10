
#ifndef _INC_COMMHANDLER_H
#define _INC_COMMHANDLER_H

            
/**
 * ICommHandler interface defines  the basic operations to parser.
 * 
 *
 */

class ICommHandler
{		
public:

    virtual BOOL Initialize( LPCTSTR lpszChannelProgID, LPBYTE lpOpenParam,
        BOOL bBigEndian, DWORD dwCookie ) = 0;
    
    virtual void Uninitialize() = 0;
 
	
		virtual BOOL SendData( LPBYTE lpData, UINT uiDataLen, LPBYTE lpSendParam ) = 0;

    
		virtual BOOL SetReceiver( BOOL bRcvThread, const LPVOID pReceiver ) = 0;
		
		virtual void StartOpr() = 0;
    
		virtual void StopOpr() = 0;

 /**
     * Get a property value of the commhandler. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pvarValue:This parameter cannot be NULL and must point to an uninitialized
     * VARIANT. If no error is returned, the VARIANT is initialized using VariantInit,
     * and then set to contain the property value.  The caller must call VariantClear 
     * on this pointer when the value is no longer required. If an error code is returned, 
     * the VARIANT pointed to by pValue is left unmodified.
     *
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     virtual BOOL GetProperty(  const char* szPropertyName, 
		 WPARAM wParam, VARIANT* pvarValue ) = 0;

    /**
     * Put a property value of the ICommHandler. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. eg, version,
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pcvarValue: Points to a VARIANT that is treated as readonly. 
     * The value in the VARIANT becomes the named property value.
     *
     * @return Returns S_OK if successful,Otherwise returns BM_E_FAILED;
     */
    virtual  BOOL PutProperty ( const char* szPropertyName,
		WPARAM wParam, const VARIANT* pvarValue ) = 0 ;

  
};

#endif

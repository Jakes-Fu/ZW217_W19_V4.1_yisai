#ifndef _INC_CHANNELITF_H
#define _INC_CHANNELITF_H

#define DEFAULT_BAUDRATE        115200
#define CHANGE_BAUD_NOT_AVAILABLE       0
#define NOT_CHANGE_BAUD_MODE            0                

#undef INTERFACE
#define INTERFACE ICommunicationChannel

/**
 * ICommunicationChannel interface defines  the basic operations to the communication channel resource.
 * 
 *
 */
DECLARE_INTERFACE_(ICommunicationChannel, IUnknown )
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;

    /**
     * Set the up-level application' receiver which receives the  data
     * 
     * Discription:
     *      1. After receives the  data, it will forward the data to
     *         this registered receiver via sending the message 'wMsgId'.
     *      2. Must be called before Open()
     *
     * @param dwMsgId : Message ID
     *
     * @param bRcvThread : Thread or window
     *
     * @param pReceiver : If bRcvThread is TRUE, pReceiver is a thread ID;
     *                     Otherwise a window handle
     */
    STDMETHOD( SetReceiver )(
        THIS_ /*[in]*/DWORD  dwMsgId,   
        /*[in]*/BOOL         bRcvThread,
        /*[in]*/const LPVOID pReceiver   
        ) PURE;    


    /**
     * Get the up-level application' receiver which receives the  data
     * 
     * @param dwMsgId : Message ID
     *
     * @param bRcvThread : Thread or window
     *
     * @param pReceiver : If bRcvThread is TRUE, pReceiver is a thread ID;
     *                     Otherwise a window handle
     */
    STDMETHOD( GetReceiver )(
        THIS_ /*[out]*/DWORD  &dwMsgId,   
        /*[out]*/BOOL         &bRcvThread,
        /*[out]*/LPVOID &pReceiver   
        ) PURE;    
    
    /**
     * Open the communication channel
     *
     * @param pOpenArgument: the argument of open the channel communication.such as uart 
     *  channel communication,the pOpenArgument param obtain the uart port and uart baudrate.
     *
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     STDMETHOD( Open )( THIS_ /*[in]*/LPBYTE pOpenArgument ) PURE;

    /**
     * Close the communication channel
     *
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     STDMETHOD( Close )( THIS ) PURE;
     
    /**
     * Clear up the receive buffer
     *
     * @return Returns BM_S_OK ;
     */
     STDMETHOD( Clear )( THIS ) PURE;
     
     /**
     * Read from  the communication channel buffer
     *
     * @param lpData: pointer the buffer that read from channel communication
     * @param ulDataSize: the buffer size 
     *
     * @return Returns the real read size ;
     */
     STDMETHOD_(ULONG, Read )( THIS_ /*[out]*/LPVOID lpData, /*[in]*/ ULONG ulDataSize  ) PURE;

    /**
     * Write to  the communication channel
     *
     * @param lpData: pointer the buffer that write to channel communication
     * @param ulDataSize: the buffer size 
     *
     * @return Returns the real write size ;
     */
     STDMETHOD_(ULONG, Write )( THIS_ /*[in]*/LPVOID lpData, /*[in]*/ULONG ulDataSize ) PURE;

    /**
     * Free the memory block allocated in the  module.
     *
     * @param pMemBlock: memory pointer 
     *
     * @return Returns BM_S_OK 
     */
     STDMETHOD( FreeMem )(THIS_ /*[in]*/LPVOID pMemBlock )PURE;

    /**
     * Get a property value of the communication channel. 
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
     * @return Returns BM_S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     STDMETHOD( GetProperty )( THIS_ LONG  lFlags,  
                const BSTR cbstrName,  VARIANT *  pvarValue ) PURE;

    /**
     * Put a property value of the communication channel. 
     *
     * @param lFlags: reserved
     *
     * @param cbstrName: The name for which the value is to be set. 
     * This must point to a valid BSTR. The pointer is treated as read-only.
     *
     * @param pcvarValue: Points to a VARIANT that is treated as readonly. 
     * The value in the VARIANT becomes the named property value.
     *
     * @return Returns S_OK if successful,Otherwise returns BM_E_FAILED;
     */
     STDMETHOD( PutProperty )( THIS_ LONG lFlags,  
         const BSTR cbstrName,  const VARIANT * pcvarValue ) PURE;

};

#endif

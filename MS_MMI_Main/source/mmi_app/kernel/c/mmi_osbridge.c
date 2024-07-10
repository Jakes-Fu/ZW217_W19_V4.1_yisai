#define MMI_BRIDGE_C

//#include "std_header.h"
//#include "sci_types.h"
#include "mmi_app_kernel_trc.h"
#include "os_api.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h"
#include "sci_api.h"
#include "mmk_msg.h"
#include "tasks_id.h"

/******************************************************************************/
/*! \brief mmi alloc memory
 *  \param[in]		numberOfBytesRequired	byte size to alloc
 *  \param[out]		**buffer				memory pointer allocated
 */
/******************************************************************************/
void MmiAllocMemory(uint16 numberOfBytesRequired,
                                            void **buffer)
{
    *buffer = SCI_ALLOC_APP(numberOfBytesRequired);
}

/******************************************************************************/
/*! \brief mmi free memory
 *  \param[in]		**buffer				memory pointer to be freed
 */
/******************************************************************************/
void MmiFreeMemory(void **buffer)
{
     SCI_FREE(*buffer);
}

/******************************************************************************/
/*! \brief mmi create a signal
 *  \param[in]		signalType		signal type
 *  \param[in]		signalSize		signal size
 *  \param[in]		**signal		signal pointer
 */
/******************************************************************************/
void MmiCreateSignal (uint16 signalType,
                                           int16 signalSize,
                                           MmiSignalS **signal)
{
    /* create the signal */
    //(*signal) = SCI_ALLOC_APP(signalSize + 16);
    //SCI_MEMSET(*signal, 0, (signalSize + 16));
    SCI_ASSERT(signalSize>=16);  /*assert verified*/
    
    (*signal) = ( MmiSignalS *)SCI_ALLOC_APP(signalSize );
    SCI_MEMSET(*signal, 0,(uint32) (signalSize ));
    if((*signal) != NULL)
    {
        (*signal) ->SignalCode = signalType;
        //modify for louis.wei 2003-11-10
    }
    else
    {// can not alloc memory
        //SCI_TRACE_LOW:"MMI : alloc signal error, signal szie = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_OSBRIDGE_50_112_2_18_2_0_11_232,(uint8*)"d",signalSize);
    }
}

/******************************************************************************/
/*! \brief mmi receive a signal from a queue
 *  \param[in]		queueId					queue id
 *  \param[in]		**signal				signal pointer
 */
/******************************************************************************/
void MmiReceiveSignal (uint16 queueId, MmiSignalS **signal)
{
    void * signal_ptr = PNULL;
#ifdef MMI_DUAL_SIM_TEST
    void * new_ptr = PNULL;	
#endif
	
    MMI_RECEIVE_SIGNAL( signal_ptr, queueId);

    if (KPDSVR != ((xSignalHeader)(signal_ptr))->Sender
        && P_APP != ((xSignalHeader)(signal_ptr))->Sender
        && TIMESVR != ((xSignalHeader)(signal_ptr))->Sender
        && CHR_SVR != ((xSignalHeader)(signal_ptr))->Sender)
    {
        SCI_LOG_INPUT_MSG(
                                            ((xSignalHeader)(signal_ptr))->Sender, 
                                            queueId, 
                                            (uint8)((xSignalHeader)(signal_ptr))->SignalCode,//SCI_INVALID_STATE,
                                            ((xSignalHeader)(signal_ptr))->SignalCode,
                                            (uint8*)signal_ptr, 
                                            ((xSignalHeader)(signal_ptr))->SignalSize
                                            );
    }

    *signal = signal_ptr;
	
#ifdef MMI_DUAL_SIM_TEST
    if ((((xSignalHeader)(signal_ptr))->SignalCode >> 8) ==  0xA1)
    {
        new_ptr = SCI_ALLOC_APP(((xSignalHeader)(signal_ptr))->SignalSize + 4);
        SCI_MEMSET(new_ptr, 0, ((xSignalHeader)(signal_ptr))->SignalSize + 4);
        SCI_MEMCPY(new_ptr, signal_ptr, ((xSignalHeader)(signal_ptr))->SignalSize);
        SCI_FREE(signal_ptr);
        *signal = new_ptr;
    }
#endif
}

/******************************************************************************/
/*! \brief mmi destroy a signal
 *  \param[in]		**signal		signal pointer
 */
/******************************************************************************/
void MmiDestroySignal(MmiSignalS **signal)
{
    MMI_FREE_SIGNAL(*signal);
    (*signal) = PNULL;
}

/******************************************************************************/
/*! \brief  mmi send a signal to a task
 *  \param[in]		destTask	task id
 *  \param[in]		*signal		signal pointer
 */
/******************************************************************************/
void MmiSendSignal (uint16 destTask, MmiSignalS *signal)
{
   SCI_SendSignal(   // If send successfully, return SCI_SUCCESS,
                                // else return SCI_ERROR
                               (void*) signal, // Signal pointer to be sended
                                destTask        // Dest thread ID
                                );
}

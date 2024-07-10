#ifndef __BL_OS_H__
#define __BL_OS_H__

// @Xueliang.Wang added(2003-01-10)
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NU_OS
    extern void _tx_thread_context_save(void);
    extern void _tx_thread_context_restore(void);
    
    #define SCI_THREAD_CONTEXT_SAVE         _tx_thread_context_save();
    #define SCI_THREAD_CONTEXT_RESTORE      _tx_thread_context_restore();
#else
    extern void TCT_Interrupt_Context_Save();
    extern void TCT_Interrupt_Context_Restore();
        
    #define SCI_THREAD_CONTEXT_SAVE         TCT_Interrupt_Context_Save();
    #define SCI_THREAD_CONTEXT_RESTORE      TCT_Interrupt_Context_Restore();
#endif  // NU_OS

#ifdef __cplusplus
}
#endif
// End added.

#endif // __BL_OS_H__
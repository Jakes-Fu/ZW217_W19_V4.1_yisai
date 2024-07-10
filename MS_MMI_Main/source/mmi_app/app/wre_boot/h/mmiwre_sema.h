#ifndef _MMIWRE_SEMA_H_
#define _MMIWRE_SEMA_H_

#define  ADAPT_SUCCESS    0


void *Adapt_CreateSemaphore (//if successful, return semaphore
    //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
);



uint32 Adapt_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    void * sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
);


uint32 Adapt_DeleteSemaphore ( //if successful return SCI_SUCCESS,
                                           //else return SCI_ERROR
    void* sem_ptr //semaphore pointer
);

uint32 Adapt_PutSemaphore ( //if successful return SCI_SUCCESS,
                                      //else return SCI_ERROR
    void *sem_ptr //semaphore pointer
);
#endif



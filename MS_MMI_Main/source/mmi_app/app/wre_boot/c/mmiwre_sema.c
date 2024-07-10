#ifdef WRE_SUPPORT

#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmiwre_sema.h"
/*注意事项，注意检测参数定义是否保持一致，如果修改，需要在bootloader适配层进行调整*/


#ifdef DEBUG
//该函数在版本发布阶段需要进行验证关联参数或宏是否有修改
void Adapt_DebugCheck()
{
	SCI_ASSERT(SCI_SUCCESS == ADAPT_SUCCESS);   /*assert verified*/
	
}
#endif

/*****************************************************************************/
//  Description : Adapt_CreateSemaphore
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
PUBLIC void* Adapt_CreateSemaphore (//if successful, return semaphore
    //pointer, else return SCI_NULL
    const char *name_ptr, //name of the semaphore
    uint32 initial_count //initial value of semaphore counter
)
{
	return (void*)SCI_CreateSemaphore(name_ptr,initial_count);
}

/*****************************************************************************/
//  Description : Adapt_GetSemaphore
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    void * sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
)
{
    return SCI_GetSemaphore((SCI_SEMAPHORE_PTR)sem_ptr,wait_option);
}

/*****************************************************************************/
//  Description : Adapt_DeleteSemaphore
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_DeleteSemaphore ( //if successful return SCI_SUCCESS,
                                           //else return SCI_ERROR
    void* sem_ptr //semaphore pointer
)
{
	return SCI_DeleteSemaphore((SCI_SEMAPHORE_PTR)sem_ptr);
}

/*****************************************************************************/
//  Description : Adapt_PutSemaphore
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint32 Adapt_PutSemaphore ( //if successful return SCI_SUCCESS,
                                      //else return SCI_ERROR
    void *sem_ptr //semaphore pointer
)
{
	return SCI_PutSemaphore((SCI_SEMAPHORE_PTR)sem_ptr);
}


#endif

/*
* amuse_thread_device.h
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains thread interface that need to 
* be implemented on different platforms.
*/

#ifndef _AMUSE_THREAD_DEVICE_H_
#define _AMUSE_THREAD_DEVICE_H_

#ifdef __cplusplus
extern "C"{
#endif
enum{
	AMUSE_CREATE_DEFAULT = 0,
	AMUSE_CREATE_SUSPENDED = 1
};

typedef void* (*amuse_thread_func)(void*);
typedef void* amuse_thread_handle;
typedef void* amuse_thread_locker;
typedef void* amuse_event_handle;
typedef void* amuse_semaphore_handle;

#if _WIN32_WCE
int amuse_create_process(char **cmd);
#else
int amuse_create_process(char *cmd);
#endif

/*
* Creates a thread to execute within the virtual address space of the calling process.
* 
* Parameters:
* pThreadFunction: A pointer to the application-defined function to be executed 
*			       by the thread and represents the starting address of the thread.
* pParameters: A pointer to a variable to be passed to the thread.
* prio: priority
*
* Returns: none
*/
amuse_thread_handle amuse_create_thread(amuse_thread_func pThreadFunction, void *pParameters, int prio);

/*
 * This function suspends thread execution.
 *
 * Parameters: 
 * h: handle to thread
 *
 * Returns: -1 if failed, otherwise success.
 */
/*int amuse_suspend_thread(amuse_thread_handle h);*/

/*
 * This function resumes thread execution.
 *
 * Parameters: 
 * h: handle to thread
 *
 * Returns: -1 if failed, otherwise success.
 */
/*int amuse_resume_thread(amuse_thread_handle h);*/

/*
* Suspends the execution of the current thread for at least the specified interval.
*
* Parameters:
* duration: the specified interval
*
* Returns: none
*/
void amuse_sleep(unsigned int duration);

/*
 * This function sets the priority value for the specified thread.
 *
 * Parameters: 
 * h: handle to thread
 * priority: priority value for the thread
 *
 * Returns: zero if fails, nonzero if succeeds
 */
int amuse_set_thread_priority(amuse_thread_handle h, int priority);

/*
 * This function gets a new mutual-exclusion synchronization lock and
 * initializes it, such as a critical section object.
 *
 * Parameters: none
 *
 * Returns: the new initialized lock
 */
amuse_thread_locker amuse_lock_get();

/*
 * This function lock the thread or waits for ownership of the lock.
 * The function returns when the calling thread is granted ownership.
 *
 * Parameters: 
 * plocker: the lock
 *
 * Returns: none
 */
void amuse_lock(amuse_thread_locker plocker);

/*
 * This function releases ownership of the specified lock.
 *
 * Parameters: 
 * plocker: the lock
 *
 * Returns: none
 */
void amuse_unlock(amuse_thread_locker plocker);

/*
 * This function releases all resources used by an unowned lock and free it.
 *
 * Parameters: 
 * plocker: the lock
 *
 * Returns: none
 */
void amuse_lock_release(amuse_thread_locker plocker);

/*
 * This function increments (increases by one) the value of 
 * the specified 32-bit variable, and prevents more than one 
 * thread from using the same variable simultaneously.
 *
 * Parameters: 
 * addend: pointer to the variable to be incremented
 *
 * Returns: the resulting incremented value
 */
long amuse_atom_increment(volatile long* addend);

/*
* This function decrements (decreases by one) the value of 
* the specified 32-bit variable, and prevents more than one 
* thread from using the same variable simultaneously.
*
* Parameters: 
* addend: pointer to the variable to be decremented
*
* Returns: the resulting decremented value
*/
long amuse_atom_decrement(volatile long* addend);

/*获得事件句柄*/
amuse_event_handle amuse_event_handle_get();
/*通知事件，使得等待事件的代码可执行*/
void amuse_signal_event(amuse_event_handle handle);
/*等待事件，直到此事件被通知*/
void amuse_wait_event(amuse_event_handle handle);
/*释放事件句柄*/
void amuse_event_handle_close(amuse_event_handle handle);

/*获得信号量句柄*/
amuse_semaphore_handle amuse_semaphore_handle_get();
/*通知信号量，使得等待事件的代码可执行*/
void amuse_release_semaphore(amuse_semaphore_handle handle);
/*等待信号量，直到此事件被通知*/
void amuse_wait_semaphore(amuse_semaphore_handle handle);
/*释放信号量句柄*/
void amuse_semaphore_handle_close(amuse_semaphore_handle handle);

#ifdef __cplusplus
}
#endif

#endif

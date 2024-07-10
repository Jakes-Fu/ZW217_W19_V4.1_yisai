// port for RTOS by lebao music, 2020-09
#ifdef REAL_WATCH_RTOS

#ifndef ___thread_port_h
#define ___thread_port_h

#define THREAD_IMPLEMENTATION

#define  THREAD_STACK_SIZE_DEFAULT (30 * 1024)

#define INVALID_THREAD_ID 	SCI_INVALID_BLOCK_ID
#define INVALID_MUTEX_ID 	NULL

typedef BLOCK_ID thread_id_t;
thread_id_t p_thread_current_thread_id(void);
void p_thread_yield(void);
void p_thread_set_high_priority(void);
void p_thread_exit(int return_code);

typedef BLOCK_ID thread_ptr_t;
thread_ptr_t p_thread_create(int(*thread_proc)(void*), void* user_data, char const* name, int stack_size);
void p_thread_destroy(thread_ptr_t thread);
int p_thread_join(thread_ptr_t thread);

//typedef SCI_MUTEX_T thread_mutex_t;
#define thread_mutex_t void
void thread_mutex_init(thread_mutex_t** mutex);
void thread_mutex_term(thread_mutex_t* mutex);
void thread_mutex_lock(thread_mutex_t* mutex);
void thread_mutex_unlock(thread_mutex_t* mutex);


//int volatile*
typedef int thread_atomic_int_t;
int thread_atomic_int_load(thread_atomic_int_t* atomic);
void thread_atomic_int_store(thread_atomic_int_t* atomic, int desired);
int thread_atomic_int_inc(thread_atomic_int_t* atomic);
int thread_atomic_int_dec(thread_atomic_int_t* atomic);
int thread_atomic_int_add(thread_atomic_int_t* atomic, int value);
int thread_atomic_int_sub(thread_atomic_int_t* atomic, int value);

#endif /* ___thread_h */

#ifndef thread_impl_RTOS
#define thread_impl_RTOS


#endif /* REAL_WATCH_RTOS */

#else

#include "../src/simulator/thread.h"

#define INVALID_MUTEX_ID 	NULL
#define INVALID_THREAD_ID 	NULL

#endif /* REAL_WATCH_RTOS */
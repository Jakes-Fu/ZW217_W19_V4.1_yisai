// port for vc6 by lebao music, 2020-09
#ifdef SIMULATOR_WATCH

/*
------------------------------------------------------------------------------
          Licensing information can be found at the end of the file.
------------------------------------------------------------------------------

thread.h - v0.3 - Cross platform threading functions for C/C++.

Do this:
    #define THREAD_IMPLEMENTATION
before you include this file in *one* C/C++ file to create the implementation.
*/

#ifndef ___thread_h
#define ___thread_h


#define THREAD_IMPLEMENTATION

#if  !( defined(_WIN32) && (_MSC_VER <= 1200) )
    typedef __int64 THREAD_U64;
#else
	typedef unsigned int uintptr_t;
	typedef unsigned int ULONG_PTR;
#endif

#define THREAD_STACK_SIZE_DEFAULT ( 30 * 1024 )
#define THREAD_SIGNAL_WAIT_INFINITE ( -1 )
#define THREAD_QUEUE_WAIT_INFINITE ( -1 )

typedef void* thread_id_t;
thread_id_t p_thread_current_thread_id( void );
void p_thread_yield( void );
void p_thread_set_high_priority( void );
void p_thread_exit( int return_code );

typedef void* thread_ptr_t;
thread_ptr_t p_thread_create( int (*thread_proc)( void* ), void* user_data, char const* name, int stack_size );
void p_thread_destroy( thread_ptr_t thread );
int p_thread_join( thread_ptr_t thread );

typedef union thread_mutex_t thread_mutex_t;
void thread_mutex_init( thread_mutex_t* mutex );
void thread_mutex_term( thread_mutex_t* mutex );
void thread_mutex_lock( thread_mutex_t* mutex );
void thread_mutex_unlock( thread_mutex_t* mutex );

typedef union thread_signal_t thread_signal_t;
void thread_signal_init( thread_signal_t* signal );
void thread_signal_term( thread_signal_t* signal );
void thread_signal_raise( thread_signal_t* signal );
int thread_signal_wait( thread_signal_t* signal, int timeout_ms );

typedef union thread_atomic_int_t thread_atomic_int_t;
int thread_atomic_int_load( thread_atomic_int_t* atomic );
void thread_atomic_int_store( thread_atomic_int_t* atomic, int desired );
int thread_atomic_int_inc( thread_atomic_int_t* atomic );
int thread_atomic_int_dec( thread_atomic_int_t* atomic );
int thread_atomic_int_add( thread_atomic_int_t* atomic, int value );
int thread_atomic_int_sub( thread_atomic_int_t* atomic, int value );
int thread_atomic_int_swap( thread_atomic_int_t* atomic, int desired );
int thread_atomic_int_compare_and_swap( thread_atomic_int_t* atomic, int expected, int desired );

typedef union thread_atomic_ptr_t thread_atomic_ptr_t;
void* thread_atomic_ptr_load( thread_atomic_ptr_t* atomic );
void thread_atomic_ptr_store( thread_atomic_ptr_t* atomic, void* desired );
void* thread_atomic_ptr_swap( thread_atomic_ptr_t* atomic, void* desired );
void* thread_atomic_ptr_compare_and_swap( thread_atomic_ptr_t* atomic, void* expected, void* desired );

#if  !( defined(_WIN32) && (_MSC_VER <= 1200) ) //VC6
typedef union thread_timer_t thread_timer_t;
void thread_timer_init( thread_timer_t* timer );
void thread_timer_term( thread_timer_t* timer );
void thread_timer_wait( thread_timer_t* timer, THREAD_U64 nanoseconds );
#endif

typedef void* thread_tls_t;
thread_tls_t thread_tls_create( void );
void thread_tls_destroy( thread_tls_t tls );
void thread_tls_set( thread_tls_t tls, void* value );
void* thread_tls_get( thread_tls_t tls );

typedef struct thread_queue_t thread_queue_t;
void thread_queue_init( thread_queue_t* queue, int size, void** values, int count );
void thread_queue_term( thread_queue_t* queue );
int thread_queue_produce( thread_queue_t* queue, void* value, int timeout_ms );
void* thread_queue_consume( thread_queue_t* queue, int timeout_ms );
int thread_queue_count( thread_queue_t* queue );

#endif /* ___thread_h */

#ifndef thread_impl
#define thread_impl

union thread_mutex_t 
    { 
    void* align; 
    char data[ 64 ];
    };

union thread_signal_t 
    { 
    void* align; 
    char data[ 116 ];
    };

union thread_atomic_int_t 
    {
    void* align;
    long i;
    };

union thread_atomic_ptr_t 
    {
    void* ptr;
    };

union thread_timer_t 
    { 
    void* data; 
    char d[ 8 ]; 
    };

struct thread_queue_t
    {
    thread_signal_t data_ready;
    thread_signal_t space_open;
    thread_atomic_int_t count;
    thread_atomic_int_t head;
    thread_atomic_int_t tail;
    void** values;
    int size;
    #ifndef NDEBUG
        thread_atomic_int_t id_produce_is_set;
        thread_id_t id_produce;
        thread_atomic_int_t id_consume_is_set;
        thread_id_t id_consume;
    #endif  
    };

#endif /* thread_impl */

#endif //REAL_WATCH_RTOS
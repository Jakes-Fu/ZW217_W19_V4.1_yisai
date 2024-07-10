#ifndef __PORT_CFG_H
#define __PORT_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef WIN32
	#ifndef _WIN32
	#define _WIN32
	#endif
#endif

#ifdef WIN32
	#define SIMULATOR_WATCH
	#define TRACE_APP_LEBAO  SCI_TraceLow
	#define helper_debug_printf printf
	//WIN32, LINUX, MacOS
	#include <assert.h>

	#ifndef __FUNCTION__
	#define VAL(x) #x
	#define STR(x) VAL(x)
	#define __FUNCTION__  "Line"STR(__LINE__)
	#endif
#else
	#include "os_api.h"

	#define REAL_WATCH_RTOS
    void helper_debug_printf(const char *fmt, ...);
#endif

// for test
//#ifndef MAINLCD_SIZE_128X128
//#define MAINLCD_SIZE_128X128
//#endif

//#ifndef MAINLCD_SIZE_240X240
//#define MAINLCD_SIZE_240X240
//#endif

//#ifndef MAINLCD_SIZE_320X480
//#define MAINLCD_SIZE_320X480
//#endif

//#ifndef MAINLCD_CIRCULAR_WATCH
//#define MAINLCD_CIRCULAR_WATCH
//#endif

//#define LEBAO_HIDE_SOFTKEYBAR

// T117, T107, T107S
#if defined(PLATFORM_UMS9117)
	#define LEBAO_PLATFORM_T117
	#ifndef WIN32
	#define LEBAO_USR_ASYNC_DNS
	#endif
#endif

// 0: nothing, 1 : mbedtls, 2 : openssl, 3 : DAPS/ssl_api.h
#if defined(WIN32)
	#define LEBAO_USE_SSL_TYPE  0
#elif defined(PLATFORM_UMS9117) || defined(PLATFORM_ANTISW3)
	#define LEBAO_USE_SSL_TYPE  3
#else
	#define LEBAO_USE_SSL_TYPE  1
#endif


/*********************
 *      DEFINES
 *********************/

//-----------------------------------------------------
//C99, string handling, maybe platform independent
#ifndef os_memset
#define os_memchr memchr
#define os_memcmp memcmp
#define os_memcpy memcpy
#define os_memmove memmove
#define os_memset memset
#endif

#ifndef os_strcpy
#define os_strlen strlen
#define os_strcpy strcpy
#define os_strncpy strncpy
#define os_strcat strcat
#define os_strncat strncat
#define os_strcmp strcmp
#define os_strncmp strncmp
#define os_strchr strchr
#define os_strrchr strrchr
#define os_strstr strstr
#define os_sprintf sprintf
#endif

//input output handling
#ifndef os_isspace
#define os_isspace	isspace
#define os_sscanf	sscanf
#define os_isprint	isprint
#define os_tolower	tolower
#define os_toupper	toupper
#define os_atoi		atoi
#define os_isalnum	isalnum
#endif

//C89, not C99
#ifndef os_vsnprintf
#if defined(WIN32) && (_WIN32_WINNT < 0x0501)
#define os_vsnprintf _vsnprintf
#else
#define os_vsnprintf vsnprintf
#endif
#endif

#ifndef va_copy
# ifdef __va_copy
# define va_copy(DEST,SRC) __va_copy((DEST),(SRC))
# else
# define va_copy(DEST, SRC) memcpy((&DEST), (&SRC), sizeof(va_list))
# endif
#endif  

//-----------------------------------------------------

#ifdef REAL_WATCH_RTOS // real watch

#ifndef os_malloc
#define os_malloc(size)			( SCI_ALLOC_APP(size) )
#define os_free(ptr)			( SCI_Free(ptr) ) //not SCI_FREE, don't to set ptr = NULL
#define os_realloc(ptr, size)	( os_realloc_impl(ptr, size) )
#define os_calloc(num, size)	( os_calloc_impl(num, size) )
#endif

extern void* os_realloc_impl(void* ptr, size_t newSize);
extern void* os_calloc_impl(size_t num, size_t size);


#ifndef os_assert
#define os_assert( exp ) SCI_ASSERT( exp )
#endif

#else //WIN32, LINUX, MacOS

#ifndef os_assert
#define os_assert assert
#endif

#ifndef os_malloc
#define os_malloc( size )        ( malloc( size ) )
#define os_free( ptr )           ( free( ptr ) )
#define os_realloc( ptr,  size ) ( realloc( ptr, size ) )
#define os_calloc( num,  size )  ( calloc( num, size ) )	
#endif

#endif

#define sdsfree_val(val)  { sdsfree(val); val = NULL; }
#define os_free_val(ptr)  {	os_free(ptr); ptr = NULL; }
typedef void(*OS_DATA_CALLBACK)(void* data);
typedef struct os_data_callback_param_t {
    int eventId;
    int pageId;
    int status;
    OS_DATA_CALLBACK callback;
    void* data;
} os_data_callback_param_t;

//RTOS platform, is not the simulator for PC version
#ifdef REAL_WATCH_RTOS
#include "sfs.h"
#define os_FILE		SFS_HANDLE
#define os_fopen	os_fopen_impl
#define os_fwrite	os_fwrite_impl
#define os_fclose	os_fclose_impl
#define os_rename	os_rename_impl
#define os_remove	os_remove_impl
#define os_fread	os_fread_impl
#define os_fseek	os_fseek_impl

//WIN32, LINUX, MacOS, Simulator
#else
#include <stdio.h>
#define os_FILE		FILE*
#define os_fopen	fopen
#define os_fwrite	fwrite
#define os_fclose	fclose
#define os_rename	rename
#define os_remove	remove
#define os_fread	fread
#define os_fseek	fseek
#endif

#if defined(REAL_WATCH_RTOS) || defined(WIN32)
#define CFG_FILE_SEPERATOR   ("\\")
#else
#define CFG_FILE_SEPERATOR   ("/")
#endif
//-----------------------------------------------------

#ifndef os_thread_sleep
#ifdef REAL_WATCH_RTOS
#define os_thread_sleep SCI_Sleep
#else
#define os_thread_sleep (void)
#endif
#endif


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#define CFG_NAME_MAX_LEN 		(256)
#define CFG_NAME_SHORT_LEN 		(64)
#define CFG_URI_MAX_LEN 		(1024)

// The RTOS system has the function of creating directory, 0 : none, 1 : can create directory
#define  CFG_CREATE_DISK_DIR		(1)

// free disk size, 500 KB
#if (defined(LEBAO_MUSIC_SUPPORT) && defined(LEBAO_FEATURE_PHONE_STYLE))
#define  CFG_SPACE_MIN_SIZE			(60 * 1024)
#else
#define  CFG_SPACE_MIN_SIZE			(120 * 1024)
#endif
// Save picture file to storage (NV / FAT), its need enough storage space, 0 : don't save any picture files, 1: can save
// The size of each picture is limited to CFG_IMAGE_MAX_SIZE
#define  CFG_CACHE_IMAGE_FILE		(0)

// The image size is limited to 30K, otherwise, don't download or open it
#define  CFG_IMAGE_MAX_SIZE			(20 * 1024)

// Maximum limit size of MP3 cached to RAM at once time, 300kb ~ 800 KB, discarding file contents larger than the limit size, the mode of play_buffer 
// default : 300kb
#define	 CFG_MP3_MAX_BUFFER_SIZE	(300 * 1024)

// Maximum limit size of MP3 saved to disk, 300 KB, otherwise, truncate the file
#if (defined(LEBAO_MUSIC_SUPPORT) && defined(LEBAO_FEATURE_PHONE_STYLE))
#define  CFG_MP3_MAX_FILE_SIZE		(60 * 1024)
#else
#define  CFG_MP3_MAX_FILE_SIZE		(300 * 1024)
#endif

// Minimum limit size of MP3 saved to disk, 30 KB, otherwise, delete the file
#define  CFG_MP3_MIN_FILE_SIZE		(30 * 1024)

// Maximum limit size of MP3 cached to RAM, 24kb ~ 800 KB, the mode of play_streaming 
// default : 300kb,  at least CFG_MP3_CHUNK_SIZE * 3
#define	 CFG_MP3_MAX_CACHE_SIZE		(128 * 1024)

// the chunk size of MP3 file when streaming, 8kb ~ 64KB
#define  CFG_MP3_CHUNK_SIZE			(16 * 1024)

// How many files can I save to disk at most
// The file of mp3 has two styles, 260KB-290KB per file, 1.5MB - 5MB of big size per file
// The amount depends on the size of the remaining space
#define  CFG_MP3_MAX_SAVE_COUNT		(1)

// How many files can I add to my favorites
#define  CFG_MP3_MAX_FAVORITES		(20)

// How many files can I add to my favorites
#define  CFG_MP3_MAX_HISTORY		(20)

// Discard all ID3 tags for save memory / cache, 0 : remain, 1 : discard by SRV layer, 2 : discard by CTRL layer
#define  CFG_DISCARD_ID3_TAGS		(1)

// The maximum number of records displayed by the list view control
#define  CFG_LIST_MAX_COUNT		(20)

// the maximum limit size of PCM cached to RAM during recording, 64KB ~ 256KB
#define	 CFG_RECORD_MAX_CACHE_SIZE		(256 * 1024)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__PORT_CFG_H*/

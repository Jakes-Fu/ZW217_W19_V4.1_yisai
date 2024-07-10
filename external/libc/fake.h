#ifndef _FAKE_H_
#define _FAKE_H_

#if defined(__cplusplus)
#define	__BEGIN_DECLS		extern "C" {
#define	__END_DECLS		}
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif

__BEGIN_DECLS

#if 1
#ifdef WIN32
#define SPLIBC_FAKE_LOG		SCI_TRACE_LOW
#else
#define SPLIBC_FAKE_LOG(fmt, args...)		SCI_TRACE_LOW("[fake debug][%s:%d]" fmt , __FUNCTION__, __LINE__, ##args)
#endif
#else
#define SPLIBC_FAKE_LOG
#endif

//extern int errno;

#if defined _WIN32 || defined WIN32 
#define __align(n)
#endif

typedef unsigned int        uint;
typedef unsigned long sigset_t;
#ifndef _CLOCKID_T
#define _CLOCKID_T
typedef int clockid_t;
#endif
typedef	int int32_t;
typedef	unsigned int uint32_t;
typedef	long off_t;
typedef int pid_t;
typedef unsigned int  uintptr_t;
typedef unsigned int __kernel_size_t;

#if defined _WIN32 || defined WIN32 
#define __align(n)
#endif

typedef unsigned short mode_t;

extern int  open(const char*  path, int  mode, ...);
int fcntl(int s, int cmd, int val);

extern int  openat(int fd, const char*  path, int  mode, ...);
extern int  unlinkat(int dirfd, const char *pathname, int flags);
extern int  creat(const char*  path, mode_t  mode);

typedef unsigned char  uint8_t;
typedef long		ssize_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;

#ifndef __user
#define __user
#endif

#define PATH_MAX 			4096

struct sched_param {
	int sched_priority;
};

typedef unsigned int __u32;


#if 1 //defined(__CC_ARM) || defined(__ARMCC__)
struct in_addr {
	__u32 s_addr;
};
#endif
typedef unsigned int size_t;

#ifdef WIN32
typedef __int64  loff_t;
#else
typedef long long loff_t;
#endif
// memory
int  SPLIBC_Init();
void SPLIBC_Finaliz();
void *SPLIBC_malloc( size_t size );
void SPLIBC_free( void *memblock );
void *SPLIBC_calloc( size_t num, size_t size );
void *SPLIBC_realloc( void *memblock, size_t size );
void *SPLIBC_alloc( size_t size );
void* SPLIBC_aligned_malloc(size_t alignment, size_t size);

// file 
//#include "sfs.h"

/*
#define SEEK_SET 		SFS_SEEK_BEGIN  
#define SEEK_CUR 		SFS_SEEK_CUR  
#define SEEK_END 		SFS_SEEK_END 
*/

#include <stdio.h>

FILE * SPLIBC_fopen(const char * path,const char * mode);
int SPLIBC_fclose(FILE * stream);
int SPLIBC_feof(FILE * stream);
size_t SPLIBC_fread(void * ptr,size_t size,size_t nmemb,FILE * stream);
size_t SPLIBC_fwrite(const void * ptr,size_t size,size_t nmemb,FILE * stream);
int SPLIBC_fseek(FILE * stream,long offset,int whence);
long SPLIBC_ftell(FILE * stream);
void SPLIBC_rewind(FILE * stream);

int SPLIBC_fileno(FILE * stream);
char * SPLIBC_mktemp(char * temp);
void SPLIBC_setbuf(FILE * stream,char * buf);
void SPLIBC_setbuffer(FILE * stream,char * buf,size_t size);
int SPLIBC_fflush(FILE* stream);
FILE * SPLIBC_freopen(const char * path, const char * mode,FILE * stream);
FILE * SPLIBC_fdopen(int fildes,const char * mode);

//io
int SPLIBC_printf( const char *format,... );
int SPLIBC__printf( const char *format,... );
int SPLIBC_fprintf( FILE *stream, const char *format,...);
int SPLIBC__fprintf( FILE *stream, const char *format,...);
int SPLIBC_getc(FILE * stream);
int SPLIBC_getchar(void);
int SPLIBC_fputc(int c,FILE * stream);
int SPLIBC_fputs(const char * s,FILE * stream);
int SPLIBC_fgetc(FILE * stream);
char* SPLIBC_fgets(char * s,int size,FILE * stream);
char* SPLIBC_gets(char *s);
int SPLIBC_putc(int c,FILE * stream);

__END_DECLS

#endif

#ifndef __FAKE_API_H__
#define __FAKE_API_H__

//#if defined(_MSC_VER)
// memory
#define malloc(size)		SPLIBC_##malloc(size)
#define free(ptr)			SPLIBC_##free(ptr)

#define calloc(num, size)	SPLIBC_##calloc(num, size)
#define realloc(ptr, size)		SPLIBC_##realloc(ptr, size)
#define _alloca(size)			SPLIBC_##_alloca(size)

// file
#define fopen(path, mode)			SPLIBC_##fopen(path, mode)
#define fclose(ptr) 				SPLIBC_##fclose(ptr)
#define feof(ptr)							SPLIBC_##feof(ptr)
#define fread(ptr,size, nbyte,stream) 	SPLIBC_##fread(ptr,size, nbyte,stream)
#define fwrite(ptr,size, nbyte,stream)	SPLIBC_##fwrite(ptr,size, nbyte,stream)
#define fseek(stream, offset, whence)		SPLIBC_##fseek(stream, offset, whence)
#define ftell(stream)					SPLIBC_##ftell(stream)
#define rewind(stream) 				SPLIBC_##rewind(stream)

#define fileno(stream)				SPLIBC_##fileno(stream)
#define mktemp(ptr)						SPLIBC_##mktemp(ptr)
#define setbuf(stream, buf)			SPLIBC_##setbuf(stream, buf)
#define setbuffe(stream, buf, size)		SPLIBC_##setbuffe(stream, buf, size)
#define fflush(stream)				SPLIBC_##fflush(stream)
#define freopen(path, mode, stream)		SPLIBC_##freopen(path, mode, stream)
#define fdopen(fd, mode)			SPLIBC_##fdopen(fd, mode)

// io
#define printf						SPLIBC_##printf
#define _printf						SPLIBC_##_printf
#define fprintf						SPLIBC_##fprintf
#define _fprintf					SPLIBC_##_fprintf
#define getc(stream)				SPLIBC_##getc(stream)
//#define getchar()					SPLIBC_##getchar()
#define fputc(c, stream)			SPLIBC_##fputc(c, stream)
#define fputs(s, stream)			SPLIBC_##fputs(s, stream)
#define fgetc(stream)				SPLIBC_##fgetc(stream)
#define fgets(s, size, stream)		SPLIBC_##fgets(s, size, stream)
#define gets(s)						SPLIBC_##gets(s)
#define putc(c, stream)				SPLIBC_##putc(c, stream)
//#endif //_MSC_VER

#undef getchar

#include "fake.h"

#endif

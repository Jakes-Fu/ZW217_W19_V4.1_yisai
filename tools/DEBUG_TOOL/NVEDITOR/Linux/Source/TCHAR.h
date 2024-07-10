/***
*tchar.h - definitions for generic international text functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Definitions for generic international functions, mostly defines
*       which map string/formatted-io/ctype functions to char, wchar_t, or
*       MBCS versions.  To be used for compatibility between single-byte,
*       multi-byte and Unicode text models.
*
*       [Public]
*
****/

#ifndef _INC_TCHAR
#define _INC_TCHAR
#include <string.h>
#include "typedef.h"

/* Program */

#define _tmain      main
#define _tWinMain   WinMain
#define _tenviron   environ
#define __targv     __argv


/* Formatted i/o */

#define _tprintf        printf
#define _ftprintf       fprintf
#define _stprintf       sprintf
#define _sntprintf      _snprintf
#define _vtprintf       vprintf
#define _vftprintf      vfprintf
#define _vstprintf      vsprintf
#define _vsntprintf     _vsnprintf

#define _tscanf         scanf
#define _ftscanf        fscanf
#define _stscanf        sscanf
/* Unformatted i/o */

#define _fgettc         fgetc
#define _fgettchar      _fgetchar
#define _fgetts         fgets
#define _fputtc         fputc
#define _fputtchar      _fputchar
#define _fputts         fputs
#define _gettc          getc
#define _gettchar       getchar
#define _getts          gets
#define _puttc          putc
#define _puttchar       putchar
#define _putts          puts
#define _ungettc        ungetc

/* String conversion functions */

#define _tcstod     strtod
#define _tcstol      strtol
#define _tcstoul    strtoul

#define _itot       _itoa
#define _ltot       _ltoa
#define _ultot      _ultoa
#define _ttoi       atoi
#define _ttol       atol

#define _ttoi64     _atoi64
#define _i64tot     _i64toa
#define _ui64tot    _ui64toa

/* String functions */

/* Note that _mbscat, _mbscpy and _mbsdup are functionally equivalent to 
   strcat, strcpy and strdup, respectively. */

#define _tcscat     strcat
#define _tcscpy     strcpy
#define _tcsdup     _strdup
#define _tcslen     strlen
#define _tcsxfrm    strxfrm


/* Execute functions */

#define _texecl     _execl
#define _texecle    _execle
#define _texeclp    _execlp
#define _texeclpe   _execlpe
#define _texecv     _execv
#define _texecve    _execve
#define _texecvp    _execvp
#define _texecvpe   _execvpe

#define _tspawnl    _spawnl
#define _tspawnle   _spawnle
#define _tspawnlp   _spawnlp
#define _tspawnlpe  _spawnlpe
#define _tspawnv    _spawnv
#define _tspawnve   _spawnve
#define _tspawnvp   _spawnvp
#define _tspawnvpe  _spawnvpe

#define _tsystem    system


/* Time functions */

#define _tasctime   asctime
#define _tctime     ctime
#define _tstrdate   _strdate
#define _tstrtime   _strtime
#define _tutime     _utime
#define _tcsftime   strftime

/* Directory functions */

#define _tchdir             _chdir
#define _tgetcwd            _getcwd
#define _tgetdcwd           _getdcwd
#define _tmkdir             _mkdir
#define _trmdir             _rmdir

#define _tfullpath      _fullpath
#define _tgetenv        getenv
#define _tmakepath      _makepath
#define _tputenv        _putenv
#define _tsearchenv     _searchenv
#define _tsplitpath     _splitpath

/* Stdio functions */

#ifdef  _POSIX_
#define _tfdopen    fdopen
#else
#define _tfdopen    _fdopen
#endif
#define _tfsopen    _fsopen
#define _tfopen     fopen
#define _tfreopen   freopen
#define _tperror    perror
#define _tpopen     _popen
#define _ttempnam   _tempnam
#define _ttmpnam    tmpnam


/* Io functions */

#define _tchmod     _chmod
#define _tcreat     _creat
#define _tfindfirst      _findfirst
#define _tfindfirsti64   _findfirsti64
#define _tfindnext       _findnext
#define _tfindnexti64    _findnexti64
#define _tmktemp            _mktemp



#define _tremove    remove
#define _trename    rename
#define _tsopen     _sopen
#define _tunlink    _unlink

#define _tfinddata_t      _finddata_t
#define _tfinddatai64_t   _finddatai64_t

/* ctype functions */
#define _istascii       isascii
#define _istcntrl       iscntrl
#define _istxdigit      isxdigit

/* Stat functions */
#define _tstat      _stat
#define _tstati64   _stati64


/* Setlocale functions */

#define _tsetlocale setlocale



/* MBCS-specific mappings */

#define _tccmp(_cp1,_cp2)   _tcsnccmp(_cp1,_cp2,1)


/* ctype functions */

#define _istalnum       _ismbcalnum
#define _istalpha       _ismbcalpha
#define _istdigit       _ismbcdigit
#define _istgraph       _ismbcgraph
#define _istlegal       _ismbclegal
#define _istlower       _ismbclower
#define _istprint       _ismbcprint
#define _istpunct       _ismbcpunct
#define _istspace       _ismbcspace
#define _istupper       _ismbcupper

#define _totupper       _mbctoupper
#define _totlower       _mbctolower

#define _istlead        _ismbblead
#define _istleadbyte    isleadbyte

#define _tcsspn		    strspn
#define _tcsstr         strstr
#define _tcschr         strchr
#define _tcstok         strtok
#define _tcscmp         strcmp
#define _tcsncmp        strncmp
#define _tcsncpy        strncpy
#define _tcsrchr        strrchr
#define _tcsicmp        strcasecmp
#define _tcsnicmp       strncasecmp
#define _taccess        access

#endif  /* _INC_TCHAR */


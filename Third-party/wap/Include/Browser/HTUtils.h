/*-------------------------------------------------------------------------
---------------------------------------------------------------------------
** HTUtils.c
** 
** Author: Li Yun 
**
** Copyright (c) 1999-2000 Coreland Software 
**              All Rights Reserved.
**
** Unauthorized redistribution of this source code, in whole or part,
** without the express written permission of Coreland Software
** is strictly prohibited.
**
** Notes:
**		MACROS FOR GENERAL USE
**
--------------------------------------------------------------------------
--------------------------------------------------------------------------*/

#ifndef HTUTILS_H
#define HTUTILS_H

#define TOASCII(c) (c)
#define FROMASCII(c) (c)

#ifdef HAVE_CONFIG_H
#else

#include <stdio.h>

#ifndef DELTA_BROWSER

/*function used in Delta Brower which shoule be supplied out of Browser:
	  assert atoi  memset  clearerr  getchar  printf sprintf
	  tcreate  tdelete  send  receivef  mcreate  acreate
	  tdelete  gettime  strtod htos  gethostbyname gethostbyaddr
*/


/*The include files out of Delta Browser*/
/*====================================================================*/

#if defined(__cplusplus) || defined(__CPLUSPLUS) || defined(__cplusplus__)
extern "C"
{
#endif
//wxs#include "task.h"
//wxs#include "output.h"
#include "timer.h"
//wxs#include "mailbox.h"
#include "error.h"
//wxs#include "alarm.h"
/*#include "sys\stat.h"*/
#include "stdlib.h"
//wxs#include "net_config.h"
//wxs#include "usr_config.h"
#if defined(__cplusplus) || defined(__CPLUSPLUS) || defined(__cplusplus__)
}
#endif

/*====================================================================*/


/*The global macros for Delta Browser*/
/*====================================================================*/
#define DELTA_BROWSER
 
#define SHOW_WHEREIS_TARGETS 1
/*#define LY_FIND_LEAKS*/
#define DISABLE_FTP
#define DISABLE_NEWS
#define DISABLE_FINGER
#define DISP_PARTIAL
#define NOSIGHUP
#define DOSPATH
#define NOUSERS
#define NCURSES_VERSION
#define SUPPORT_MULTIBYTE_EDIT
#define CJK_EX
#define WIN_EX
/*====================================================================*/

/*The macro define for the display*/
/*====================================================================*/
#define SetConsoleTitle(p)
#define MAX_CHARS_IN_LINE 20
/*====================================================================*/

#ifdef EOF
#undef EOF
#endif

#define EOF -1  

/*Define the task management function*/
/*====================================================================*/
#define TaskCreate  tcreate
#define TaskDelete  tdelete
#define TaskSuspend suspend
#define TaskResume  resume
/*====================================================================*/

/*Define for the task used to browsing*/
/*====================================================================*/
#define NET_BROWSER_TASK_ID        9
#define NET_BROWSER_TASK_PRIORITY  11
#define NET_BROWSER_TASK_STACKSIZE 0x3C00
/*====================================================================*/

/*Define for the task used to read from network*/
/*====================================================================*/
#define NET_READ_TASK_ID        10
#define NET_READ_TASK_PRIORITY  10
#define NET_READ_TASK_STACKSIZE 0x3C00
/*====================================================================*/

/*Define for the task used for WML browser*/
/*====================================================================*/
#define NET_WML_TASK_ID        11
#define NET_WML_TASK_PRIORITY  11
#define NET_WML_TASK_STACKSIZE 0x3C00
/*====================================================================*/

/*Define for the task used for WML browser iamge handling*/
/*====================================================================*/
#define NET_WML_IMAGE_TASK_ID        12
#define NET_WML_IMAGE_TASK_PRIORITY  11
#define NET_WML_IMAGE_TASK_STACKSIZE 0x3C00
/*====================================================================*/

/*Define for the task used to handling UI*/
/*====================================================================*/
#define NET_UI_TASK_ID        8
#define NET_UI_TASK_PRIORITY  12
#define NET_UI_TASK_STACKSIZE 0x3c00//3c00->6c00
/*====================================================================*/

/*Define the net data message handling function */
/*====================================================================*/
#define NET_DATA_MAIL_ID          10
#define NET_DATA_MAIL_TYPE        FIFO
#define SendNetMessage            send
#define ReceiveNetMessage         receivef
#define CreateNetMessageMailBox() mcreate(NET_DATA_MAIL_ID,NET_DATA_MAIL_TYPE)
#define DeleteNetMessageMailBox() mdelete(NET_DATA_MAIL_ID)
/*====================================================================*/

/*Define the soft clock handling function */
/*====================================================================*/
#define TICK    	5
#define CLOCK_ID    5
#define CLOCK_TYPE  REPETIVE
#define CLOCK_TIME  1000/TICK
#define AlarmCreate acreate
#define AlarmDelete adelete
/*====================================================================*/

/*the Message used by Delta Browser */
#define DB_STOP  0
#define DB_PREV  1 
#define DB_NEXT  2
#define DB_PGDN  3
#define DB_PGUP  4
#define DB_LNDN  5
#define DB_LNUP  6
#define DB_HELP  7


/*Something about socket*/
/*====================================================================*/
typedef struct {
	int fd;
	char *buf;
	int len;
} recv_data_t;

struct in_addr_Browser {
        union {
                struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { unsigned short s_w1,s_w2; } S_un_w;
                unsigned long S_addr;
        } S_un;
#define s_addr  S_un.S_addr
                                /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2
                                /* host on imp */
#define s_net   S_un.S_un_b.s_b1
                                /* network */
#define s_imp   S_un.S_un_w.s_w2
                                /* imp */
#define s_impno S_un.S_un_b.s_b4
                                /* imp # */
#define s_lh    S_un.S_un_b.s_b3
                                /* logical host */
};

struct sockaddr_in_Browser {
   short   sin_family;
   unsigned short   sin_port;
   struct   in_addr_Browser sin_addr;
   char   sin_zero[8];
};

#define SOCKET_CONNECT_TIMEOUT 18000 /*used in HTDoConnect.*/
#define   INADDR_NONE     0xffffffff
#define   IPPROTO_TCP     0
#define GetSocketError(socket) Dnet_errno(socket)
#define SetSocketError Dnet_seterr

#define socket(afam,type,protocol)						Dnet_socket(afam,type,protocol)
#define socket_closesocket(socket)						Dnet_socketclose(socket)
#define socket_recv(socket,buffer,length,flags)			Dnet_recv(socket,buffer,length,flags)
#define socket_send(socket,msg,msglen,flags)			Dnet_send(socket,msg,msglen,flags)
#define socket_connect(socket,addr,addrlen)				Dnet_connect(socket,addr,addrlen)
#define socket_select(ignore,refds,wrfds,exfds,time)	Dnet_select(ignore,refds,wrfds,exfds,time)
#define inet_addr(DotIP)								inet_addr_for_delta(DotIP)
#define socket_setsockopt								Dnet_setsockopt
/*parse_ipad
#define htons(hostshort) htons(hostshort)
#define gethostbyname(name) gethostbyname(name)
#define gethostbyaddr(address,alen,atype) gethostbyaddr(address,alen,atype)
*/
/*====================================================================*/


/*Non-socket function used in Delta Browser*/
/*====================================================================*/
#define time_t unsigned long
#define time(p,a) gettime(CLOCK_GET_TICKS_SINCE_BOOT,a)
#define ctime(p) NULL
#define GetTickCount(a) gettime(CLOCK_GET_TICKS_SINCE_BOOT,a )
/*====================================================================*/

#undef TRUE
#undef FALSE

#define TRUE    (BOOLEAN)1
#define FALSE   (BOOLEAN)0 
typedef char    BOOLEAN; 
#define DWORD unsigned long

#endif

#define gettext(s) s

#define ERANGE 222

#define ANSI_VARARGS 1
#undef HAVE_STDARG_H
#define HAVE_STDARG_H 1

/* Accommodate non-autoconf'd Makefile's (VMS, DJGPP, etc) */

#ifndef NO_ARPA_INET_H
#define HAVE_ARPA_INET_H 1
#endif

#endif /* HAVE_CONFIG_H */

#ifndef LY_MAXPATH
#define LY_MAXPATH 256
#endif

#ifndef	GCC_NORETURN
#define	GCC_NORETURN /* nothing */
#endif

#ifndef	GCC_UNUSED
#define	GCC_UNUSED /* nothing */
#endif

#define BOOLEAN_DEFINED

#ifndef USE_COLOR_STYLE
    /* it's useless for such setup */
#  define NO_EMPTY_HREFLESS_A
#endif

#define TRACE 0

/*

  ERROR TYPE

   This is passed back when streams are aborted. It might be nice to have some structure
   of error messages, numbers, and recursive pointers to reasons.  Curently this is a
   placeholder for something more sophisticated.

 */
typedef void * HTError;                 /* Unused at present -- best definition? */


#define PUBLIC                  /* Accessible outside this module     */
#define PRIVATE static          /* Accessible only within this module */

#if defined(__STDC__) || defined(__BORLANDC__) || defined(_MSC_VER)
#define CONST const             /* "const" only exists in STDC */
#define NOPARAMS (void)
#define PARAMS(parameter_list) parameter_list
#define NOARGS (void)
#define ARGS1(t,a) \
                (t a)
#define ARGS2(t,a,u,b) \
                (t a, u b)
#define ARGS3(t,a,u,b,v,c) \
                (t a, u b, v c)
#define ARGS4(t,a,u,b,v,c,w,d) \
                (t a, u b, v c, w d)
#define ARGS5(t,a,u,b,v,c,w,d,x,e) \
                (t a, u b, v c, w d, x e)
#define ARGS6(t,a,u,b,v,c,w,d,x,e,y,f) \
                (t a, u b, v c, w d, x e, y f)
#define ARGS7(t,a,u,b,v,c,w,d,x,e,y,f,z,g) \
                (t a, u b, v c, w d, x e, y f, z g)
#define ARGS8(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h) \
                (t a, u b, v c, w d, x e, y f, z g, s h)
#define ARGS9(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i) \
                (t a, u b, v c, w d, x e, y f, z g, s h, r i)
#define ARGS10(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i,q,j) \
                (t a, u b, v c, w d, x e, y f, z g, s h, r i, q j)

#else  /* not ANSI */

#define NOPARAMS ()
#define PARAMS(parameter_list) ()
#define NOARGS ()
#define ARGS1(t,a) (a) \
                t a;
#define ARGS2(t,a,u,b) (a,b) \
                t a; u b;
#define ARGS3(t,a,u,b,v,c) (a,b,c) \
                t a; u b; v c;
#define ARGS4(t,a,u,b,v,c,w,d) (a,b,c,d) \
                t a; u b; v c; w d;
#define ARGS5(t,a,u,b,v,c,w,d,x,e) (a,b,c,d,e) \
                t a; u b; v c; w d; x e;
#define ARGS6(t,a,u,b,v,c,w,d,x,e,y,f) (a,b,c,d,e,f) \
                t a; u b; v c; w d; x e; y f;
#define ARGS7(t,a,u,b,v,c,w,d,x,e,y,f,z,g) (a,b,c,d,e,f,g) \
                t a; u b; v c; w d; x e; y f; z g;
#define ARGS8(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h) (a,b,c,d,e,f,g,h) \
                t a; u b; v c; w d; x e; y f; z g; s h;
#define ARGS9(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i) (a,b,c,d,e,f,g,h,i) \
                t a; u b; v c; w d; x e; y f; z g; s h; r i;
#define ARGS10(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i,q,j) (a,b,c,d,e,f,g,h,i,j) \
                t a; u b; v c; w d; x e; y f; z g; s h; r i; q j;


#endif /* __STDC__ (ANSI) */

#ifndef NULL
#define NULL (0)
#endif

#define NONNULL(s) (((s) != 0) ? s : "(null)")

/* array/table size */
#define	TABLESIZE(v)	(sizeof(v)/sizeof(v[0]))

#define	typecalloc(cast)		(cast *)calloc(1,sizeof(cast))
#define	typecallocn(cast,ntypes)	(cast *)calloc(ntypes,sizeof(cast))

/*

OFTEN USED INTEGER MACROS

  Min and Max functions

 */
#ifndef HTMIN
#define HTMIN(a,b) ((a) <= (b) ? (a) : (b))
#define HTMAX(a,b) ((a) >= (b) ? (a) : (b))
#endif
/*

Booleans

 */
/* Note: GOOD and BAD are already defined (differently) on RS6000 aix */
/* #define GOOD(status) ((status)38;1)   VMS style status: test bit 0         */
/* #define BAD(status)  (!GOOD(status))  Bit 0 set if OK, otherwise clear   */

#ifndef BOOL
#define BOOL BOOLEAN
#endif
#ifndef YES
#define YES (BOOLEAN)1
#define NO (BOOLEAN)0
#endif

#define TCP_PORT 80     /* Allocated to http by Jon Postel/ISI 24-Jan-92 */
#define OLD_TCP_PORT 2784       /* Try the old one if no answer on 80 */
#define DNP_OBJ 80      /* This one doesn't look busy, but we must check */
                        /* That one was for decnet */

/*      Inline Function WHITE: Is character c white space? */
/*      For speed, include all control characters */

#define WHITE(c) (((unsigned char)(TOASCII(c))) <= 32)

/*     Inline Function LYIsASCII: Is character c a traditional ASCII
**     character (i.e. <128) after converting from host character set.  */

#define LYIsASCII(c) (TOASCII((unsigned char)(c)) < 128)

/*

Success (>=0) and failure (<0) codes

Some of the values are chosen to be HTTP-like, but status return values
are generally not the response status from any specific protocol.

 */

#define HT_REDIRECTING 399
#define HT_LOADED 200                   /* Instead of a socket */
#define HT_PARTIAL_CONTENT      206     /* Partial Content */
#define HT_INTERRUPTED			-1111
#define NETREAD_INTERRUPTED		-2222
#define HT_NOT_LOADED			-3333
/*#define HT_WEB_TOO_LARGE		-4444 */

#define HT_OK           0               /* Generic success*/

#define HT_ERROR                -1      /* Generic failure */

#define HT_CANNOT_TRANSLATE -4

#define HT_NO_DATA		-204	/* OK but no data was loaded - */
					/* possibly other app started or forked */
#define HT_NO_ACCESS            -401    /* Access not available */
#define HT_FORBIDDEN            -403    /* Access forbidden */
#define HT_NOT_ACCEPTABLE       -406    /* Not Acceptable */

#define HT_PARSER_REOPEN_ELT	 700	/* tells SGML parser to keep tag open */
#define HT_PARSER_OTHER_CONTENT	 701	/* tells SGML to change content model */
#define HT_H_ERRNO_VALID 	-800	/* see h_errno for resolver error */

#define HT_INTERNAL             -900    /* Weird -- should never happen. */
#define HT_BAD_EOF      -12             /* Premature EOF */

#ifndef va_arg
# if HAVE_STDARG_H && ANSI_VARARGS
#  include <stdarg.h>
# else
#  if HAVE_VARARGS_H
#   include <varargs.h>
#  endif
# endif
#endif

#if ANSI_VARARGS
#define LYva_start(ap,format) va_start(ap,format)
#else
#define LYva_start(ap,format) va_start(ap)
#endif

/*
 * GCC can be told that some functions are like printf (and do type-checking on
 * their parameters).
 */
#if	GCC_PRINTF
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif

#include "HTString.h"   /* String utilities */

/*

Out Of Memory checking for malloc() return:

 */
#ifndef __FILE__
#define __FILE__ ""
#define __LINE__ ""
#endif

extern void outofmem(const char *func); /*DragonBall*/

/*

Upper- and Lowercase macros

   The problem here is that toupper(x) is not defined officially unless isupper(x) is.
   These macros are CERTAINLY needed on #if defined(pyr) || define(mips) or BDSI
   platforms.  For safefy, we make them mandatory.

 */
#include <ctype.h>
#include <string.h>

#ifndef TOLOWER
  /* Pyramid and Mips can't uppercase non-alpha */
#define TOLOWER(c) (isupper((unsigned char)c) ? tolower((unsigned char)c) : ((unsigned char)c))
#define TOUPPER(c) (islower((unsigned char)c) ? toupper((unsigned char)c) : ((unsigned char)c))
#endif /* TOLOWER */

#define FREE(x) if (x != 0) {free((char *)x); x = NULL;}

/*

The local equivalents of CR and LF

   We can check for these after net ascii text has been converted to the local
   representation.  Similarly, we include them in strings to be sent as net ascii after
   translation.

 */
#define LF   FROMASCII('\012')  /* ASCII line feed LOCAL EQUIVALENT */
#define CR   FROMASCII('\015')  /* Will be converted to ^M for transmission */

#endif /* HTUTILS_H */

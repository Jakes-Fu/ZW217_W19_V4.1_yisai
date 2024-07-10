#Preprocessor
#MCFLAG_OPT = --gnu
MCFLAG_OPT += -DKAL_ON_RTT

#Directory
MINCPATH = RTOS/export/inc \
	   MS_Ref/export/inc \
	   external/libc/h \
	   external/libc/h/sys \
	   external/libc \
	   DAPS/export/inc \
       DAPS/source/common/inc

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH     += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT
else
MINCPATH     += DAPS/export/inc/tcpip
endif

#Local Dir src
MSRCPATH = external/libc \
           external/libc/c				

SOURCES = fake_pthread.c fake_filesystem.c fake_memory.c fake_io.c POSIX_pthread_cond.c POSIX_time.c POSIX_utils.c POSIX_thread.c socket.c


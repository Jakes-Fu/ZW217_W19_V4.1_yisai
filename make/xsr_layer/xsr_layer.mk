

MCFLAG_OPT =  -DXSR_DP -D_RTOS -DXSR_NW -DOS_SCI
MASMFLAG_OPT = 


ifeq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT += -D_LITTLE_ENDIAN
endif

#---------------XSR kernel path-----------------#
MINCPATH =	MS_Ref/source/xsr_layer/xsr_kernel/include	\
		MS_Ref/source/xsr_layer/xsr_kernel/Core/STL	\
		MS_Ref/source/xsr_layer/xsr_kernel/Core/BML

MSRCPATH =	MS_Ref/source/xsr_layer/xsr_kernel/Core/STL	\
		MS_Ref/source/xsr_layer/xsr_kernel/Core/BML

#---------------XSR port path-------------------#
MINCPATH +=	MS_Ref/export/inc			\
		MS_Ref/source/xsr_layer/xsr_port/dnandl	\
		MS_Ref/source/xsr_layer/xsr_port/dnands	\
		MS_Ref/source/xsr_layer/xsr_port/lld
MINCPATH +=	chip_drv/export/inc/outdated BASE/ps/export/inc

MSRCPATH +=	MS_Ref/source/xsr_layer/xsr_port/dnandl	\
		MS_Ref/source/xsr_layer/xsr_port/dnands	\
		MS_Ref/source/xsr_layer/xsr_port/ecc		\
		MS_Ref/source/xsr_layer/xsr_port/lld		\
		MS_Ref/source/xsr_layer/xsr_port/oam		\
		MS_Ref/source/xsr_layer/xsr_port/pam

#---------------XSR probe path------------------#
MINCPATH +=	MS_Ref/source/xsr_layer
MSRCPATH +=	MS_Ref/source/xsr_layer

#---------------XSR kernel src------------------#
SOURCES	=	BMLInterface.c	\
		BadBlkMgr.c	\
		GarbageQueue.c	\
		OpQueue.c	\
		SamBufMgr.c	\
		SectorMap.c	\
		STLInterface.c	\
		VirtualNand.c
#---------------XSR port src--------------------#
SOURCES +=	os_threadx.c		\
		pam.c			\
		lld.c			\
		ecc.c			\
		pnl.c			\
		pns.c

#---------------XSR probe src-------------------#
SOURCES +=	swap.s			\
		compaction_task.c	\
		xsr_probe.c

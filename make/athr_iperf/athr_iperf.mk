
ifeq ($(strip $(PLATFORM)),	SC6600R)
MCFLAG_OPT   	= -DDISABLE_OPT -D_RECONSTRUCTION_ -DWMFAPI_NO_DRM \
			-DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
			-D_Embedded_x86 -DBITSTREAM_CORRUPTION_ASSERTS -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
			-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
			-DBIG_ENDIAN -DBUILD_WMASTD -D_WMF_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM 

MASMFLAG_OPT   = 
endif

ifeq ($(strip $(PLATFORM)),	SC6800)
MCFLAG_OPT   	= DDISABLE_OPT -D_RECONSTRUCTION_ \
					-DWMFAPI_NO_DRM -DWMA_OPT_STRMDEC_ARM -DWMA_OPT_INVERSQUAN_LOWRATE_ARM -DWMA_OPT_LPCSTD_ARM -DENABLE_LPC \
					-D_Embedded_x86 -DBITSTREAM_CORRUPTION_ASSERTS -DWMA_OPT_SUBFRAMERECON_ARM -DWMA_DEBUG -DENABLE_ALL_ENCOPT \
					-DWMA_OPT_AURECON_ARM -DBUILD_INTEGER -D_RTOS -DWMA_OPT_HUFFDEC_ARM -D_ARM_ -DWMA_OPT_FFT_ARM -D_DEBUG \
					-DBIG_ENDIAN -DBUILD_WMASTD -D_WMF_ -D_REF_SC6800_ -DWMAAPI_NO_DRM_STREAM -DWMA_V9 -D_RAM_ALLOC_ -DWMAAPI_NO_DRM

MASMFLAG_OPT   = 
endif

ATH_IPERF_LIB_SRC = DAPS/source/wifi/tools/iperf/iperf_lib
ATH_IPERF_TEST_SRC = DAPS/source/wifi/tools/iperf/iperf_test
ATH_DRIVER_SRC = DAPS/source/wifi/ar6302

MINCPATH =  $(ATH_IPERF_LIB_SRC)/include \
						$(ATH_DRIVER_SRC)/host/os/sprd_os/include	\
						$(ATH_DRIVER_SRC)/host/os/sprd_os/utility/flag \
						$(ATH_DRIVER_SRC)/host/include	\
						$(ATH_DRIVER_SRC)/include \
						MS_Customize/source/product/driver/wifi/ar6302
						

MSRCPATH =  $(ATH_IPERF_LIB_SRC)/src \
            $(ATH_IPERF_LIB_SRC)/compat \
            $(ATH_IPERF_TEST_SRC)
                
						
SOURCES			= compat.c	\
							error.c	\
							gettimeofday.c	\
							string.c	\
							Thread.c	\
							delay.cpp	\
							my_soc.cpp	\
							gnu_getopt.c	\
							gnu_getopt_long.c	\
							Locale.c	\
							ReportCSV.c	\
							ReportDefault.c	\
							Reporter.c	\
							service.c	\
							SocketAddr.c	\
							stdio.c	\
							globe.c	\
							tcp_window_size.c	\
							sockets.c	\
							Client.cpp	\
							Launch.cpp	\
							List.cpp	\
							main.cpp	\
							PerfSocket.cpp	\
							Settings.cpp	\
							Server.cpp	\
							Listener.cpp \
							iperftest.cpp
				  
					  

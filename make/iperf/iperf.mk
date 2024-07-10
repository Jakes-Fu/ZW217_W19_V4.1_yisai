MINCPATH = DAPS/source/wifi/tools/iperf_1.7.0/lib
MINCPATH += DAPS/source/wifi/tools/iperf_1.7.0/src

MSRCPATH =  DAPS/source/wifi/tools/iperf_1.7.0
MSRCPATH += DAPS/source/wifi/tools/iperf_1.7.0/lib
MSRCPATH += DAPS/source/wifi/tools/iperf_1.7.0/src
MSRCPATH += DAPS/source/wifi/tools/iperf_1.7.0/cfg
ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT += -DPLATFORM_SC6530
endif
ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MCFLAG_OPT += -DPLATFORM_SC6531EFM
endif
MCFLAG_OPT += -DWLAN_HEADERS

RTL_DRIVER_SRC = DAPS/source/wifi/tools/iperf_1.7.0

SOURCES	 =  endian.c error.c gnu_getopt.c gnu_getopt_long.c 
SOURCES += inet_pton.c inet_ntop.c snprintf.c sockets.c
SOURCES += signal.c stdio.c string.c tcp_window_size.c
SOURCES += Queue.cpp Socket.cpp SocketAddr.cpp Thread.cpp delay.cpp list.cpp
SOURCES += Client.cpp Extractor.cpp Listener.cpp PerfSocket.cpp PerfSocket_TCP.cpp
SOURCES += PerfSocket_UDP.cpp Server.cpp Settings.cpp main.cpp Notify.cpp Audience.cpp Speaker.cpp

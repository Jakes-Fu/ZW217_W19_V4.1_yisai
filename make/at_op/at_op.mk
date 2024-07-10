#Preprocessor
#MCFLAG_OPT = --gnu
MCFLAG_OPT = -DKAL_ON_RTT

#Directory
MINCPATH = RTOS/export/inc
MINCPATH += DAPS/export/inc
MINCPATH += MS_Ref/export/inc
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
else
MINCPATH   += DAPS/export/inc/tcpip
endif

#libc
MINCPATH  += external/libc/h
MINCPATH  += external/libc/h/sys
MINCPATH  += external/libc


MINCPATH += BASE/l4/export/inc
MINCPATH += BASE/PS/export/inc
MINCPATH += BASE/atc/export/inc
MINCPATH += BASE/sim/export/inc
MINCPATH += MS_MMI_Main/source/mmi_app/kernel/h
MINCPATH += MS_MMI_Main/source/mmi_app/common/h
MINCPATH += MS_MMI_Main/source/resource
MINCPATH += MS_MMI_Main/source/mmi_app/app/pdp/h
MINCPATH += MS_MMI_Main/source/mmi_app/app/connection/h
MINCPATH += MS_MMI_Main/source/mmi_app/app/phone/h
MINCPATH += MS_MMI_Main/source/mmi_utility/h
MINCPATH += MS_MMI_Main/source/mmi_kernel/include
MINCPATH += CAF/Template/include


#mqtt
MINCPATH +=  Third-party/mqtt/src/unisoc/include \
Third-party/mqtt/src/paho.mqtt.c-master/src \
Third-party/mqtt/src/unisoc/time \
Third-party/mqtt/src/unisoc/thread \
Third-party/mqtt/src/unisoc/tls 


#Local Dir inc
MINCPATH += Third-party/at_op/
MINCPATH += Third-party/atc_op/query_traffic/h

#Local Dir src
MSRCPATH += Third-party/at_op/
MSRCPATH += Third-party/at_op/query_traffic/c


SOURCES += http_test.c speed_test.c socket_test.c ping_test.c atc_daps_api.c query_traffic.c

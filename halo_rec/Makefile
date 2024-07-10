## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

# Name of the module
LOCAL_NAME := platform/base/efuse_rec

P_TJPATH=platform/base
include ./efuse_rec.mk

LOCAL_API_DEPENDS    := \
						platform  \
						platform/chip/hal  \
						platform/chip/hal/$(CT_ASIC)	\
						platform/chip/regs/$(CT_ASIC)   \
						platform/base/rpc \
						platform/base/efuse_rec\
						platform/base/os/sx \
						platform/chip/edrv/aud/sc2720 \
						
						
LOCAL_MODULE_DEPENDS := 

LOCAL_ADD_INCLUDE := ${MINCPATH}
		
#maro
MYCPPFLAGS += ${MCFLAG_OPT}

#c source
C_SRC := ${SOURCES}

# enable file search
VPATH_ENABLE := yes

##search the c file absolute path
VPATH_C_PATH = ${foreach path, ${MSRCPATH}, ${SOFT_WORKDIR}/${path}}

vpath 
vpath %.c ${VPATH_C_PATH}

# This is a top-level module.
IS_TOP_LEVEL := no

## ------------------------------------- ##
##      List all your sources here       ##
## ------------------------------------- ##
#include algorithm/make.srcs
#include business/apcpverify/make.srcs
#include business/control/make.srcs
#include business/factory/make.srcs
#include business/license/make.srcs
#include test/make.srcs


## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

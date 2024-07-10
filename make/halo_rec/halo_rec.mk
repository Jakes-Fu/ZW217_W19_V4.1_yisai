
MINCPATH = halo_rec/algorithm \
		   halo_rec/business/apcpverify \
		   halo_rec/business/control \
		   halo_rec/business/factory \
		   halo_rec/business/license\
		   halo_rec/keys \
		   halo_rec/system_hal \
		   halo_rec/system_hal/hardware \
		   halo_rec/system_hal/os \
		   halo_rec/utility/ipc \
		   halo_rec/test \
           chip_drv/export/inc \
		   chip_drv/export/inc/outdated \
           MS_Ref/export/inc \
           MS_Ref/source/rpc/inc \
		   common/export/inc \

MSRCPATH = halo_rec/algorithm \
		   halo_rec/business/apcpverify \
		   halo_rec/business/control \
		   halo_rec/business/factory \
		   halo_rec/business/license\
		   halo_rec/keys \
		   halo_rec/system_hal \
		   halo_rec/system_hal/hardware \
		   halo_rec/system_hal/os \
		   halo_rec/test \
		   halo_rec/utility/ipc 
		   
ifeq ($(strip $(HALO_SUPPORT)),	TRUE)
SOURCES = halo_ctrl.c \
		  halo_factory.c \
		  halo_license.c \
		  halo_verify.c \
		  sha1_32.c pk1.c rsa.c rsa_sprd.c \
		  halo_ipc.c 
		  
ifeq ($(strip $(HALO_TEST_SUPPORT)), TRUE)
SOURCES +=framework.c  alloc-testing.c test-alloc-testing.c test-diagcmd.c test-algorithm.c test-apcp-process.c test-main.c
endif

	  
CFLAGS := $(CFLAGS) --c99
else
SOURCES = halo_dummy.c
endif


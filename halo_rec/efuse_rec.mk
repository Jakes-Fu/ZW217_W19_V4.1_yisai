INCLUDE_BASE_DIR = $(P_TJPATH)
SOURCE_BASE_DIR = $(P_TJPATH)

MINCPATH = $(P_TJPATH)/efuse_rec/algorithm \
		   $(P_TJPATH)/efuse_rec/business/apcpverify \
		   $(P_TJPATH)/efuse_rec/business/control \
		   $(P_TJPATH)/efuse_rec/business/factory \
		   $(P_TJPATH)/efuse_rec/business/license\
		   $(P_TJPATH)/efuse_rec/keys \
		   $(P_TJPATH)/efuse_rec/system_hal \
		   $(P_TJPATH)/efuse_rec/system_hal/hardware \
		   $(P_TJPATH)/efuse_rec/system_hal/os \
		   $(P_TJPATH)/efuse_rec/utility/ipc \
		   $(P_TJPATH)/efuse_rec/test \
        

MSRCPATH = $(P_TJPATH)/efuse_rec/algorithm \
		   $(P_TJPATH)/efuse_rec/business/apcpverify \
		   $(P_TJPATH)/efuse_rec/business/control \
		   $(P_TJPATH)/efuse_rec/business/factory \
		   $(P_TJPATH)/efuse_rec/business/license\
		   $(P_TJPATH)/efuse_rec/keys \
		   $(P_TJPATH)/efuse_rec/system_hal \
		   $(P_TJPATH)/efuse_rec/system_hal/hardware \
		   $(P_TJPATH)/efuse_rec/system_hal/os \
		   $(P_TJPATH)/efuse_rec/test \
		   $(P_TJPATH)/efuse_rec/utility/ipc 
		   

SOURCES = halo_ctrl.c \
		  halo_factory.c \
		  halo_license.c \
		  halo_verify.c \
		  sha1_32.c pk1.c rsa.c rsa_sprd.c\
		  halo_ipc.c \
		  
#framework.c  alloc-testing.c test-alloc-testing.c test-diagcmd.c test-algorithm.c test-apcp-process.c test-main.c\

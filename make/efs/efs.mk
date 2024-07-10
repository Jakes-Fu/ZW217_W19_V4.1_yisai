

MCFLAG_OPT   	=

ifneq  ($(strip $(COMPILER)), RVCT)
MCFLAG_OPT   += -zo 
endif

ifeq  ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DEFS_LOW_MEMORY 
endif

ifeq ($(strip $(PLATFORM)),UWS6121E)
PLATFORM_TEMP = uws6121e
endif

MINCPATH  = MS_Ref/source/efs/inc MS_Ref/export/inc
MINCPATH += chip_drv/export/inc/outdated BASE/PS/export/inc  \
			MS_Customize/source/product/config/$(strip $(PLATFORM_TEMP))

MSRCPATH  = MS_Ref/source/efs/src

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
MINCPATH += MS_Ref/source/efs/inc/anti
MSRCPATH += MS_Ref/source/efs/src/anti
endif


SOURCES   = efs_dummy.c \
            efs_task.c  \
            efs_direct.c \
            efs_event.c \
            efs_irp.c   \
            efs_item.c  \
            nv_sys.c    \
            update_fixnv.c \
            crc16.c


ifeq ($(strip $(EFS_FILE_SYSTEM_SUPPORT)), TRUE)
SOURCES  += efs_comm.c  \
            efs_find.c   \
            efs_rename.c \
            efs_space.c  \
            efs_create.c \
            efs_rpc.c    \
            efs_ansi.c   \
            efs_delete.c \
            efs_rw.c     \
            efs_close.c  \
            efs_dir.c    \
            efs_fcb.c    \
            efs_length.c \
            efs_seek.c 
endif

ifeq ($(strip $(CONFIG_ANTIREG_LICENSE_SUPPORT)), TRUE)
SOURCES  += antiaes.c  \
            sha256.c   \
            sm3.c \
            sm4.c 
endif
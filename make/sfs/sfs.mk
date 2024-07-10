
ifneq ($(strip $(DRM_SUPPORT)),)  
   ifneq ($(strip $(DRM_SUPPORT)), NONE)    
        MCFLAG_OPT		+=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
   else
        MCFLAG_OPT		+=  -DDRM_SUPPORT_NONE
   endif
endif

ifeq ($(strip $(CARD_LOG)), TRUE)
MCFLAG_OPT      += -DCARD_LOG
endif

ifeq ($(strip $(PLATFORM)),SC8800D)
MCFLAG_OPT   	+= -Ec -D_REF_SC8800D_
else
MCFLAG_OPT   	+= -Ec
endif

ifeq ($(strip $(DRM_ENABLE)), TRUE)
MCFLAG_OPT   += -DDRM_ENABLE
endif

ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 32X64)
ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
MCFLAG_OPT   += -DSFS_SUPPORT_XFIND -DSFS_SUPPORT_XMOVE
endif
endif
endif
endif


MINCPATH =  MS_Ref/source/sfs/inc_cutmem BASE/ps/export/inc

ifeq ($(strip $(CARD_LOG)), TRUE)
MINCPATH      += BASE/logsave/export/inc
endif

MSRCPATH =  MS_Ref/source/sfs/src_cutmem

SOURCES			= fat_api.c fat_area_manager.c fat_databuf_manager.c fat_dev_manager.c fat_diskpal_manager.c fat_fattable_manager.c fat_fcb_manager.c fat_folder.c \
					fat_format.c fat_function.c fat_handle_manager.c fat_mbr_manager.c fat_string.c fat_task.c ffs_api.c \
					sfs_api.c fat_fdlist_manager.c fat_cmd_process.c Fat_cmd_xfind_process.c fat_function_xfind.c Fat_cmd_xmov_process.c fat_function_xmov.c  fat_irp.c


# sme.mk
#
#   2008 Spreatrum, Incoporated. All Rights Reserved.
#
# history
#   date       | author      | remarks
#   2008/04/23 | Pizer.Fan   | created
#   2010/08/16 | Pizer.Fan   | use 'sme' instead of 'sme_target'
#   2011/03/11 | Pizer.Fan   | use 'kal' instead of 'kal_target'
# 

MINCPATH =
MSRCPATH = 
SOURCES = 
MCFLAG_OPT =
INCLUDE_BASE_DIR = $(P_TJPATH)
SOURCE_BASE_DIR = $(P_TJPATH)

# ------------------------------------------------------------- #
#                       B a s e  t a b                          #
# ------------------------------------------------------------- #
MINCPATH = \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/include \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/common/include  \
$(INCLUDE_BASE_DIR)/Source/infra/kal/include  \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/tt/tt_core/inc  \
$(INCLUDE_BASE_DIR)/Source/infra/stacklib/include  \
$(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/production  \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/include  \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/asn/include  \
$(INCLUDE_BASE_DIR)/Source/infra/commlib_target/include  \
$(INCLUDE_BASE_DIR)/Source/infra/init/common/include  \
$(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/debug  \
$(INCLUDE_BASE_DIR)/Source/infra/tools/act-bin/include  \
$(INCLUDE_BASE_DIR)/Source/infra/adaptation/common/include  \
common/include  \
MM_Ref/Include  \
RTOS/Include  \
Transform/Export/inc  \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/src  \
$(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/asn/src  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/dm/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/common/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/3g/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/asn/include  \
$(INCLUDE_BASE_DIR)/Source/infra/adaptation/stack/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs  \
$(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/3g/include  \
$(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsb/include  \
include \
lib \
\
common/export/inc \
RTOS/export/inc \
MS_Ref/export/inc \
chip_drv/export/inc

MSRCPATH += $(SOURCE_BASE_DIR)/Source/infra/test-infra/sme/src
SOURCES += \
    kal_cmd_fp.c \
    sme_buff_mgr.c \
    sme_cmd_fp.c \
    sme_cmd_hdlr.c \
    sme_end.c \
    sme_frame.c \
    sme_ilmesg.c \
    sme_init.c \
    sme_log.c \
    sme_log_var.c \
    sme_main.c \
    sme_print.c \
    sme_reader_end.c \
    sme_reader_init.c \
    sme_reader_main.c \
    sme_reader_reset.c \
    sme_reset.c \
    sme_rw_if.c \
    sme_task_prof.c \
    sme_thinpack_if.c \
    sme_trace_hdlr.c \
    sme_thinpack_if_old.c

MSRCPATH += $(SOURCE_BASE_DIR)/Source/infra/test-infra/sme/asn/src
SOURCES += \
    tt_smestruct_req.c \
    stack_structs_req.c
    

# ------------------------------------------------------------- #
#                      C / C + +  t a b                         #
# ------------------------------------------------------------- #
MCFLAG_OPT = \
    -DDEBUG_KAL -D__SPREADTRUM_SME -DPROJECT_9100 -DENTITY_ID=0 \
    -DKAL_ON_THREADX -DCLOCK_SYNC_NOT_PRESENT \
    -D_RTOS -D_DEBUG -DTARGET_ARM -DTIANLONG_MOCOR

ifeq ($(strip $(GPRSMPDP_SUPPORT)), TRUE)
MCFLAG_OPT += -DGPRSMPDP_ENABLE	
endif
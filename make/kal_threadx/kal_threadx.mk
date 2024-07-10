# kal_threadx.mk
#
#   2009 Spreatrum, Incoporated. All Rights Reserved.
#
# history
#   date       | author      | remarks
#   2009/07/01 | Pizer.Fan   | created
#   2010/08/16 | Pizer.Fan   | use 'kal' instead of 'kal_target'
# 

MINCPATH =
MSRCPATH = 
SOURCES = 
MCFLAG_OPT =
INCLUDE_BASE_DIR = $(P_TJPATH)
SOURCE_BASE_DIR = $(P_TJPATH)/Source/infra/kal

# ------------------------------------------------------------- #
#                       B a s e  t a b                          #
# ------------------------------------------------------------- #
MINCPATH = \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/threadx/include \
    common/include \
    RTOS/Include \
    Transform/Export/inc \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/common/src \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/threadx/src \
    include \
    lib \
    \
    common/export/inc \
    RTOS/export/inc

MSRCPATH += $(SOURCE_BASE_DIR)/common/src
SOURCES += \
    kal_buff_mngr_common.c \
    kal_debug_buff_mngr.c \
    kal_debug_error_handling.c \
    kal_debug_itc.c \
    kal_debug_mem_mgr.c \
    kal_debug_reset.c \
    kal_debug_timer.c \
    kal_error_handling.c \
    kal_mem_mgr_common.c

MSRCPATH += $(SOURCE_BASE_DIR)/threadx/src
SOURCES += \
    kal_debug_threadx_non_exported.c \
    kal_threadx_buff_mgr.c \
    kal_threadx_deinitialize.c \
    kal_threadx_delete_calls.c \
    kal_threadx_error_handling.c \
    kal_threadx_initialize.c \
    kal_threadx_itc.c \
    kal_threadx_mem_mgr.c \
    kal_threadx_tasks.c \
    kal_threadx_timer.c \
    threadx_env.c \
    threadx_libc.c

# ------------------------------------------------------------- #
#                      C / C + +  t a b                         #
# ------------------------------------------------------------- #
MCFLAG_OPT = \
    -DDEBUG_KAL -DKAL_ON_THREADX -DSCI_KAL -D_RTOS -DMEAS_STACK_SZ \
    -D_DEBUG -DCORRUPTION_CHECK -DTARGET_ARM -DMN_AL_SMS_NOT_PRESENT \
    -DTIANLONG_MOCOR

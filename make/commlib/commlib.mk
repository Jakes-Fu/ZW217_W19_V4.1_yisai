# commlib.mk
#
#   2008 Spreatrum, Incoporated. All Rights Reserved.
#
# history
#   date       | author      | remarks
#   2008/04/23 | Pizer.Fan   | created
#   2010/08/16 | Pizer.Fan   | using 'kal' instead of 'kal_target'
# 

MINCPATH =
MSRCPATH = 
SOURCES = 
MCFLAG_OPT =
INCLUDE_BASE_DIR = $(P_TJPATH)
SOURCE_BASE_DIR = $(P_TJPATH)/Source/infra/commlib_target

# ------------------------------------------------------------- #
#                       B a s e  t a b                          #
# ------------------------------------------------------------- #
MINCPATH = \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/armserial/include \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/threadx/include \
    RTOS/Include \
    MM_Ref/Include \
    common/include \
    SIO/Export/inc \
    HAL/Export/inc \
    common \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/armserial/src \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/common/src \
    Transform/Export/inc \
    include \
    lib \
    \
    common/export/inc \
    MS_Ref/export/inc \
    RTOS/export/inc \
    chip_drv/export/inc

MSRCPATH += $(SOURCE_BASE_DIR)/armserial/src
SOURCES += \
    com_serial.c
    
MSRCPATH += $(SOURCE_BASE_DIR)/common/src
SOURCES += \
    com_lch.c \
    com_pch.c \
    com_sys_call.c


# ------------------------------------------------------------- #
#                      C / C + +  t a b                         #
# ------------------------------------------------------------- #
MCFLAG_OPT = \
    -DDEBUG_KAL \
    -DPROJECT_9100 -DKAL_ON_THREADX -DCLOCK_SYNC_NOT_PRESENT -D_RTOS -D_DEBUG \
    -DCOM_LIB_SERIAL -DTARGET_ARM -DCOM_ON_THREADX -DMN_AL_SMS_NOT_PRESENT

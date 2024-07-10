# infra_target.mk
#
#   2008 Spreatrum, Incoporated. All Rights Reserved.
#
# history
#   date       | author      | remarks
#   2009/06/29 | wei.fan     | created
#   2010/08/16 | Pizer.Fan   | using 'kal' instead of 'kal_target'
#              |             | using 'sme' instead of 'sme_target'
# 

MINCPATH =
MSRCPATH = 
SOURCES = 
MCFLAG_OPT =
INCLUDE_BASE_DIR =  $(P_TJPATH)
SOURCE_BASE_DIR =  $(P_TJPATH)/Source

# ------------------------------------------------------------- #
#                       B a s e  t a b                          #
# ------------------------------------------------------------- #
MINCPATH = \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/common/src \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/common/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/common/src \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/dll/src \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/dll/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/debug_support/common/src \
    $(INCLUDE_BASE_DIR)/Source/msd/code/debug_support/common/include \
    $(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/debug \
    $(INCLUDE_BASE_DIR)/Source/msd/build/obj/gen/production \
    $(INCLUDE_BASE_DIR)/Source/infra/init/common/src \
    $(INCLUDE_BASE_DIR)/Source/infra/init/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/asn/src \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/asn/include \
    $(INCLUDE_BASE_DIR)/Source/infra/stacklib/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/sme/include \
    $(INCLUDE_BASE_DIR)/Source/infra/commlib_target/include \
    $(INCLUDE_BASE_DIR)/Source/infra/test-infra/include \
    $(INCLUDE_BASE_DIR)/Source/infra/tools/asn1lib/include \
    $(INCLUDE_BASE_DIR)/Source/infra/stacklib/src \
    $(INCLUDE_BASE_DIR)/Source/msd/code/debug_support/stack/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/stack/src \
    $(INCLUDE_BASE_DIR)/Source/infra/adaptation/stack/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/src \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/include \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/phal/ARM926EJ-S/src \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/phal/include \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/gprs/src \
    $(INCLUDE_BASE_DIR)/Source/infra/pm/gprs/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/osa/src \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/osa/include \
    $(INCLUDE_BASE_DIR)/Source/msd/build \
    \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/common/include \
    $(INCLUDE_BASE_DIR)/Source/infra/kal/threadx/include \
    \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/common/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/common/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/dm/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/3g/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/3g/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsb/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsb/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsc/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/config/stack/gprs/gprsc/asn/include \
    \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/common/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/3g/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/3g/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/dm/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/dm/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/gprsb/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/gprsb/asn/include \
    $(INCLUDE_BASE_DIR)/Source/msd/code/interfaces/gprs/gprsc/asn/include \
    \
    common/export/inc \
    MS_Ref/export/inc \
    RTOS/export/inc \
    chip_drv/export/inc \
    MS_Customize/export/inc \
    ref\include \
    mm_ref/include \
    mm_ref/source/drv_sc8800D/h \
    common/include \
    transform/export/inc \
    rtos/include \
    $(INCLUDE_BASE_DIR)/Source/ref/include

MINCPATH += chip_drv/export/inc/outdated

MSRCPATH += $(SOURCE_BASE_DIR)/infra/adaptation/common/src
SOURCES += \
    infra_adaptation.c \
    system_adaptation.c

MSRCPATH += $(SOURCE_BASE_DIR)/msd/code/config/common/src
SOURCES += \
    ue_infra_channel_reg_funcs.c \
    infra_task_config.c \
    infra_version_info.c \
    ue_infra_channel_config.c \
    infra_profile.c \
    system_version_api.c \
    system_config.c \
    system_config_3g.c

MSRCPATH += $(SOURCE_BASE_DIR)/msd/code/config/osa/src
SOURCES += \
    osa_profile.c \
    osa_version_info.c \
    ue_osa_channel_config.c \
    ue_osa_channel_reg_funcs.c

MSRCPATH += $(SOURCE_BASE_DIR)/infra/test-infra/dll/src
SOURCES += \
    infra_getvar.c

MSRCPATH += $(SOURCE_BASE_DIR)/msd/code/debug_support/common/src
SOURCES += \
    sme_combine.c \
    infra_tp_callb.c \
    infra_dealloc.c

MSRCPATH += $(SOURCE_BASE_DIR)/msd/build/obj/gen/debug
SOURCES += \
    infra_thinpack.c \
    infra_offsize.c

MSRCPATH += $(SOURCE_BASE_DIR)/msd/build/obj/gen/production
SOURCES += \
    infra_version.c

MSRCPATH += $(SOURCE_BASE_DIR)/infra/init/common/src
SOURCES += \
    system_ral_main.c \
    infra_reset.c \
    infra_task_mains.c \
    system_active_identity.c \
    system_common_utils.c \
    infra_init.c

MSRCPATH += $(SOURCE_BASE_DIR)/infra/stacklib/src
SOURCES += \
    bitop_funcs.c \
    bitstream.c \
    event_shed.c \
    gen_utility.c \
    lcd_cqueue.c \
    lcd_dll.c \
    lcd_lqueue.c \
    stack_timer.c \
    stack_utils.c \
    asn_error.c

MSRCPATH += $(SOURCE_BASE_DIR)/infra/pm/src
SOURCES += \
    pm_interface.c

MSRCPATH += $(SOURCE_BASE_DIR)/infra/pm/phal/ARM926EJ-S/src
SOURCES += \
    phal_interface.c


# ------------------------------------------------------------- #
#                      C / C + +  t a b                         #
# ------------------------------------------------------------- #
MCFLAG_OPT = \
    -DDEBUG_KAL -DSMSAL_PDU -DAC_EXT_IF_STUB -DINFRA_SME_CHANNEL_USES_SERIAL \
    -DENTITY_ID=ENTITY_INFRA -DKAL_ON_THREADX -DSTATISTICS_ENABLE \
    -DCLOCK_SYNC_NOT_PRESENT -DLOOPBACK2 -D_RTOS -D_DEBUG \
    -DRPS_DEBUG -DMAC_TVM -DSMG31 -DRRC_DCFE_CONFIG_PARAM_CHECK_ON -DNVM_CHANGES \
    -DTARGET_ARM -DSTACK_BYPASS -DTHIN_SAT -DFLOWCTRL_3G_ON -DMN_AL_SMS_NOT_PRESENT \
    -DTIANLONG_MOCOR -D_X_64



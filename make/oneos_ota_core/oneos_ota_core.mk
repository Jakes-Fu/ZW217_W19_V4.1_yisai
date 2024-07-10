#Preprocessor
MCFLAG_OPT = --C99

#Directory
MINCPATH += external/oneos/inc/kernel \
            external/oneos/inc/fal \
            external/oneos/inc/ \
           Third-party/oneos/ota/cmiot_src/inc \
           Third-party/oneos \
           Third-party/oneos/cms/inc \
           Third-party/oneos/cms/src

        #    MS_Customize/export/inc \
        #    chip_drv/export/inc/outdated \
        #    external/oneos/inc/socket \
        #    MS_Ref/export/inc \
        #    RTOS/export/inc \
        #    chip_drv/export/inc \
        #    common/export/inc \
        #    DAPS/export/inc \
        #    MS_MMI_Main/source/mmi_kernel/include \
        #    CAF/Template/include \
        #    MS_MMI_Main/source/mmi_utility/h \
        #    MS_MMI_Main/source/mmi_app/app/pdp/h \
        #    MS_MMI_Main/source/mmi_app/app/connection/h \
        #    MS_MMI_Main/source/mmi_app/common/h \
        #    BASE/l4/export/inc \
        #    MS_MMI_Main/source/mmi_app/kernel/h \
        #    BASE/sim/export/inc\
        #    BASE/PS/export/inc\
        #    BASE/atc/export/inc\
        #    MS_MMI_Main/source/mmi_app/common/h\
        #    MS_MMI_Main/source/resource \



#Local Dir inc
# MINCPATH += Third-party/oneos/ota/cmiot/inc

#Local Dir src
MSRCPATH = Third-party/oneos/ota/cmiot_src/src/single/cms \
            Third-party/oneos/ota/cmiot_src/src/single/comm \
            Third-party/oneos/ota/cmiot_src/src/comm

SOURCES = cmiot_client.c  cmiot_coap.c  cmiot_http.c  cmiot_config.c  cmiot_hal.c  cmiot_hmd5.c   cmiot_log.c  cmiot_md5.c  cmiot_utils.c

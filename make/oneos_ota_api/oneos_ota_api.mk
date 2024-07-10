#Preprocessor
#MCFLAG_OPT = --gnu

#Directory
MINCPATH = external/oneos/inc/kernel \
           Third-party/oneos
        #    Third-party/oneos/ota/cmiot_src \

        #    MS_MMI_Main/source/mmi_kernel/include\
        #    CAF/Template/include\
        #    MS_MMI_Main/source/mmi_utility/h\
        #    MS_MMI_Main/source/mmi_app/app/pdp/h \
        #    MS_MMI_Main/source/mmi_app/app/connection/h \
        #    BASE/l4/export/inc \
        #    MS_MMI_Main/source/mmi_app/kernel/h \
        #    BASE/sim/export/inc\
        #    BASE/PS/export/inc\
        #    BASE/atc/export/inc\
        #    MS_MMI_Main/source/mmi_app/common/h \
        #    MS_MMI_Main/source/resource \




#Local Dir inc
MINCPATH += Third-party/oneos/ota/cmiot/inc

#Local Dir src
MSRCPATH = Third-party/oneos/ota/cmiot/src


#SOURCES = oneos_socket.c   oneos_fs_posix.c

SOURCES = cmiot_user.c    cmiot_self.c

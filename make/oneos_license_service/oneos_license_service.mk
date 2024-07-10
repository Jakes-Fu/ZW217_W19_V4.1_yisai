
MCFLAG_OPT   	= --c99

MINCPATH += \
    Third-party/oneos/license_service \
    Third-party/oneos/oneos_license \
    Third-party/oneos/cms/inc \
    Third-party/oneos/cms/src \
    Third-party/oneos \
    external/oneos/inc/kernel

MSRCPATH += Third-party/oneos/license_service \
            Third-party/oneos/oneos_license

SOURCES += cms_lic_interface.c cms_lic_net.c oneos_avc_license.c
include Makefile.rules

MCFLAG_OPT   	= 
CREATE_AP_SVN:= $(shell $(PERL) ./make/perl_script/gen_software_ap.pl)
CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime.pl | $(TEE) build/$(PROJECT)_builddir/tmp/buildtime_ver.h)
VERSION_FILE := $(shell $(PERL) ./make/perl_script/get_filename.pl version $(strip $(PLATFORM))_$(PROJECT)_version.c $(strip $(PLATFORM))_$(strip $(PRODUCT_CONFIG))_version.c)

MSRCPATH =  version
MINCPATH +=  build/$(PROJECT)_builddir/tmp
MINCPATH += chip_drv/export/inc/outdated

ifeq ($(strip $(FOTA_SUPPORT)), ADUPS)
MINCPATH += Third-party/adups/hal/include
endif

ifeq ($(strip $(RELEASE_INFO)), TRUE)
MCFLAG_OPT   	+= -D_VERSION_RELEASE_
else
MCFLAG_OPT   	+= -D_VERSION_DEBUG_
endif
#MCFLAG_OPT   	+= -D_SW_VERSION_BYD_

SOURCES			= $(VERSION_FILE)


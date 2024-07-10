MCFLAG_OPT   	= -Otime -zc

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

MINCPATH   		= MS_Code/MS_Ref/export/inc \
			  	  chip_drv/export/inc \
			  	  MS_Customize/source/product/config/$(strip $(PROD_CONFIG))

MSRCPATH		= MS_Ref/source/tp

ifeq ($(strip $(CHIP_VER)),	8800H5)
SOURCES			= tp_srv_sc8800h5.c 
else
ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES			= tp_srv_sc8800h5.c
else
SOURCES			= tp_srv.c
endif
endif



MCFLAG_OPT   	= 

MINCPATH += MS_Ref/source/bsc/inc					\
			 BASE/l4/export/inc  							\
			 MS_Ref/source/base/inc					\
			 chip_drv/export/inc						\
			 chip_drv/export/inc/outdated

MINCPATH += MS_Ref/source/security/inc					             

MSRCPATH =  MS_Ref/source/bsc/src

SOURCES         += bsc.c bsc_device.c security_data_protect.c
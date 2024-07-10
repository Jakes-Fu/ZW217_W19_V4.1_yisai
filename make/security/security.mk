
MCFLAG_OPT   	= 

MINCPATH =	MS_Ref/source/security/inc	\
			 			MS_Ref/source/base/inc	\
			 			fdl_bootloader/common/inc	\
			 			chip_drv/export/inc/outdated	\
			 			chip_drv/chip_plf/export/inc	\
			 			ms_ref/source/bsc/inc

MSRCPATH =  MS_Ref/source/security/src	\
						fdl_bootloader/common/src/algo	\
#						ms_ref/source/base/src \
						fdl_bootloader/chip_drv/src	\
						
SOURCES 	=		secure_drv.c secure_verify.c sha1_32.c
SOURCES		+= 		rsa.c rsa_sub.s rsa_multidw.s 
SOURCES		+= 		aes.c
SOURCES		+= 		secure_write_flash.c
 

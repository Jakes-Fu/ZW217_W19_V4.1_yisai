ifeq ($(SCREEN_TEST_SUPPORT), TRUE)
MCFLAG_OPT		= -DSCREEN_TEST
endif

ifeq ($(strip $(PLATFORM)),	ROCKY)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600R
MASMFLAG_OPT	+= -PD "CHIP_DRV_SC6800D SETL {FALSE}" -PD "CHIP_DRV_SC6600R SETL {TRUE}" -PD "CHIP_DRV_SC6600H SETL {FALSE}" -PD "CHIP_DRV_SC6600M SETL {FALSE}"
endif

MINCPATH      = MS_Ref/source/gzip/inc   
MSRCPATH      = MS_Ref/source/gzip/src 

SOURCES       =      img_gzip_deflate.c   img_gzip_trees.c   img_gzip_util.c \
		     img_gzip_gzip.c      img_gzip_unlzh.c   img_gzip_zip.c     \
		     img_gzip_inflate.c   img_gzip_unlzw.c   img_gzip_unzip.c   img_gzip_lzw.c   \
		     img_gzip_unpack.c 	  img_gzip_bits.c    img_gzip_platform.c  



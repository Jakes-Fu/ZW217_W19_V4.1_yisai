MCFLAG_OPT   	= -Otime -DARM -D__GNUC__

ifeq ($(strip $(CHIP_ENDIAN)),	LITTLE)
MCFLAG_OPT	+= -DREAL_LBR_LITTLE_ENDIAN
endif

MINCPATH =  MS_Ref/source/codec/ra8lbr/inc    MS_Ref/export/inc \
                    MS_Ref/source/aud_exp/eq/inc   MS_Ref/source/audioservice/inc_export   MS_Ref/source/audioservice/inc \
                    

MINCPATH += chip_drv/export/inc/outdated BASE/ps/export/inc
MINCPATH += $(L1_INCPATH)
 	
MSRCPATH =  MS_Ref/source/codec/ra8lbr   MS_Ref/source/codec/ra8lbr/dec_src

SOURCES  =    ra8lbr_adp.c lbr_bitpack.c  lbr_category.c \
                     lbr_couple.c  lbr_envelope.c  LBR_imdct.c LBR_imdct_asm.s lbr_gainctrl.c \
                     lbr_gecko2codec.c   lbr_hufftabs.c \
                     lbr_mlt.c  lbr_sqvh.c  lbr_trigtabs.c 

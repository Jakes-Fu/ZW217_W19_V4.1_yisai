
######################################################################################################
#                                FEATURE OPTIONS
######################################################################################################
ifeq ($(strip $(TMVB_DEC_SUPPORT)), TRUE)
	MEDIA_FILE_DEMUX_RMVB_SUPPORT		= TRUE
endif


######################################################################################################
#                                MACRO DEFINITION
######################################################################################################
ifeq ($(strip $(CHIP_ENDIAN)),	BIG)
	MCFLAG_OPT	= -D_MEDIA_FILE_DEMUX_BIG_ENDIAN_
endif

ifeq ($(strip $(MEDIA_FILE_DEMUX_RMVB_SUPPORT)), TRUE)
	MCFLAG_OPT += -D_MEDIA_FILE_DEMUX_RMVB_SUPPORT_
endif


######################################################################################################
#                                INCLUDE PATH and SOURCE FILE
######################################################################################################

MINCPATH    =   MS_Ref/export/inc
ifeq ($(strip $(MEDIA_FILE_DEMUX_RMVB_SUPPORT)), TRUE)
    MINCPATH    +=  MS_Ref/source/media_file_demux/src/rmvb/lib/rm_inc
endif


ifeq ($(strip $(MEDIA_FILE_DEMUX_RMVB_SUPPORT)), TRUE)
    MSRCPATH    =   MS_Ref/source/media_file_demux/src/rmvb     \
                    MS_Ref/source/media_file_demux/src/rmvb/lib/common \
                    MS_Ref/source/media_file_demux/src/rmvb/lib/ra_depack \
                    MS_Ref/source/media_file_demux/src/rmvb/lib/rv_depack \
                    MS_Ref/source/media_file_demux/src/rmvb/lib/rm_parser
endif


ifeq ($(strip $(MEDIA_FILE_DEMUX_RMVB_SUPPORT)), TRUE)
    SOURCES =  demux_rmvb_adp.c \
                \
                helix_date.c \
                memory_utils.c \
                pack_utils.c \
                rm_error_default.c \
                rm_io_default.c \
                rm_memory_default.c \
                rm_packet.c \
                rm_property.c \
                rm_stream.c \
                stream_hdr_utils.c \
                string_utils.c \
                rasl.c \
                ra_depack.c \
                ra_depack_internal.c \
                rm_parser.c \
                rm_parser_internal.c \
                rv_depack.c \
                rv_depack_internal.c 
endif


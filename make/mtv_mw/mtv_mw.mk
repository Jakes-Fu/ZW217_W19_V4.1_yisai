
ifeq ($(strip $(SCREEN_TEST_SUPPORT)), TRUE)
MCFLAG_OPT      += -DSCREEN_TEST
endif

ifeq ($(strip $(LCD_CONNECTED_TO_V)), TRUE)
MCFLAG_OPT      += -DLCD_CONNECTED_TO_V
endif

ifeq ($(strip $(USING_STATIC_THREAD)), TRUE)
MCFLAG_OPT      += -DUSING_STATIC_THREAD
endif

ifeq ($(strip $(HOST_IS_LITTLEEND)), TRUE)
MCFLAG_OPT      += -DHOST_IS_LITTLEEND
endif

ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)
MCFLAG_OPT      += -DMBBMS_SUPPORT
endif

ifeq ($(strip $(MTV_MIDDLEWARE_ON_BB)), TRUE)

	MINCPATH += CMMB/source/dtl/inc \
				chip_drv/export/inc/outdated \
	            CMMB/source/mbbmsca/h \
	            CMMB/source/mtv/h CMMB/source/misc         \
	            CMMB/export/inc ARMRomCode/Third-party/expat/include \
	            CMMB/source/mmal/inc CMMB/source/cmmbdmx/inc  \
	            CMMB/source/demodal/inc 
	
	MSRCPATH += CMMB/source/mtv/c CMMB/source/misc CMMB/source/cmmbdmx/src
	
	SOURCES += cmmb_operation.c mtv_data_parse.c mtv_demux.c mtv_operation.c  \
	           mtv_operationman.c mtv_stimidata_parse.c mtv_stimiesg_parse.c  \
	           ppbuffer.c tdmb_operation.c csm.c csm_strategy.c mtv_wrapper.c \
	           mtvesg_common.c mtvesg_parser.c      \
	           mtv_dbc.c mtv_dbc_storage.c mtv_crc32.c      \
	           rssi_report.c \
	           cmmbdmx_if.c cmmbdmx.c cmmbdmx_aes.c cmmbdmx_crc.c cmmbdmx_misc.c

	ifeq ($(strip $(MBBMS_SUPPORT)), TRUE)

	else ifeq ($(strip $(MBBMS_SUPPORT)), FALSE)
	
	SOURCES += ca_ccl.c ca_cml.c ca_csl.c ca_dsx.c ca_adapter.c ca_os_sc6600r.c ca_icc.c
	
	endif
endif
ifneq ($(strip $(SPRD_SIPC_ENABLE)), TRUE)
MCFLAG_OPT      += -DSPRD_ENGPC_TEST
endif

#engpc core
MINCPATH      += MS_Ref/source/engpc/sprd_fts_inc
MINCPATH      += MS_Ref/source/engpc/server/inc
MINCPATH      += MS_Ref/source/ulog/inc
MINCPATH      += chip_drv/export/inc/outdated

ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
else
MINCPATH   += DAPS/export/inc/tcpip
endif
MSRCPATH      += MS_Ref/source/engpc/server/src
SOURCES       += chnl_main.c chnl_cp.c chnl_pc.c  modulemgr.c 

#SOURCES  += chnl_wcn.c chnl_sipc.c chnl_wifi.c chnl_sock.c chnl_sock_diag.c chnl_sd.c chnl_wifi_conn.c





#support libgps
MINCPATH      += MS_Ref/source/engpc/modules/libgps/inc \
				 MS_Customize/source/product/driver/gps/green_eye2/gnsspc \
				 MS_Customize/source/product/driver/gps/green_eye2/ge2_libgps_mp/gps_lib/inc




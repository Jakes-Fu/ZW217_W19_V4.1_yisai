MCFLAG_OPT  += -Otime 

MINCPATH =  MS_Ref/export/inc \
						MS_Ref/source/image_proc/inc \
						MS_Ref/source/image_proc/src/img_proc/inc \
						MS_Ref/source/image_proc/src/img_transform/inc \
						MS_Ref/source/image_proc/src/isp/inc \
						MS_Ref/source/image_proc/src/jpeg/inc	\
						chip_drv/export/inc/outdated								
						         
MSRCPATH =  MS_Ref/source/image_proc/src \
						MS_Ref/source/image_proc/src/img_transform/src \
						MS_Ref/source/image_proc/src/isp/src \
            MS_Ref/source/image_proc/src/jpeg/src 
           
ifeq ($(strip $(PLATFORM)), SC6600L)
MINCPATH += MS_Ref/source/vsp/sc6600l/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6600l/inc 
endif  

ifeq ($(strip $(PLATFORM)), SC6800H)
MINCPATH += MS_Ref/source/vsp/sc6800h/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6800h/inc  
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MINCPATH += MS_Ref/source/vsp/sc6530/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/inc
endif  

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += MS_Ref/source/vsp/sc6530/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/inc
endif          

ifeq ($(strip $(PLATFORM)), UWS6121E)
MINCPATH += MS_Ref/source/vsp/sc6530/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/inc
endif  

ifeq ($(strip $(PLATFORM)), SC8800G)
MINCPATH += MS_Ref/source/vsp/sc8800g/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_8800g/inc
endif  
	             
ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES     += isp_drv_6x00.c ivsp_drv_6x00.c
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES     += isp_drv_6x00.c ivsp_drv_6x00.c
endif	

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES     += Ivsp_drv_sc6530.c isp_drv_sc6530.c
endif	

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES     += Ivsp_drv_sc6530.c isp_drv_sc6530.c
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES     += Ivsp_drv_sc6530.c isp_drv_uws6121e.c
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES     += ivsp_drv_8800h5.c isp_drv_sc8800g.c
endif

ifeq ($(strip $(JPG_DEC_SUPPORT)), JPG_HW)
SOURCES			+=  jpeg_task.c 

ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES			+= jpeg_isr_sc6600l.c					  				  						
endif		

#6800h jpeg firmware code
ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES			+= jpeg_isr.c			
endif		

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES			+= jpeg_isr.c			
endif	

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES			+= jpeg_isr.c			
endif

ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES			+= jpeg_isr.c			
endif

#8800G jpeg firmware code
ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES			+= jpeg_isr.c			
endif	
endif

SOURCES			+= yuv2rgb.c \
							 rgb2rgb.c

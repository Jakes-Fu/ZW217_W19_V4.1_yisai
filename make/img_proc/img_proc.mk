MCFLAG_OPT  += -DJPG_MINIATURE_SUPPORT
#MCFLAG_OPT  += -DEXIF_READER_SUPPORT

ifeq ($(strip $(CAMERA_SUPPORT)), TRUE)
MCFLAG_OPT	+= -DEXIF_WRITER_SUPPORT
else
	ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
	MCFLAG_OPT	+= -DEXIF_WRITER_SUPPORT
	endif				
endif

MCFLAG_OPT += --cpu cortex-a5.neon
MCFLAG_OPT +=--fpu VFPv4
MCFLAG_OPT +=-D__ARM_NEON__
MCFLAG_OPT +=-D_NEON_OPT_
MCFLAG_OPT +=-DJPEG_USE_READ_FUNC 
#MCFLAG_OPT += -DIDCT_ASSEMBLY -DMC_ASSEMBLY
MINCPATH =  MS_Ref/source/image_proc/inc \
            MS_Ref/source/image_proc/src/scaling/inc \
            MS_Ref/source/image_proc/src/gif_decode/inc \
            MS_Ref/source/image_proc/src/bmp_decode/inc \
            MS_Ref/source/image_proc/src/exif/inc \
            MS_Ref/source/image_proc/src/sharpen/inc \
            MS_Ref/source/image_proc/src/gif_decode/inc \
            MS_Ref/source/image_proc/src/png_decode/inc \
            MS_Ref/source/image_proc/src/jpeg/inc \
            MS_Ref/source/image_proc/src/img_proc/inc \
            MS_Ref/export/inc \
            MS_Ref/source/image_proc/src/photo_editor/inc \
            MS_Ref/source/image_proc/src/gzip/inc \
            MS_Ref/source/image_proc/src/ui_special_effect/inc \
            MS_Ref/source/image_proc/src/jpeg/image_decoder/inc \
            MS_Ref/source/image_proc/src/common/inc \
            MS_Ref/source/image_proc/src/img_dec/inc \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/image_proc/src/isp/inc \
            MS_Ref/source/display/inc	\
            MS_Ref/source/image_proc/src/img_transform/inc \
            MS_Ref/source/image_proc/src/ico/inc \
            MS_Ref/source/image_proc/src/Chipmunk-6.0.0/include/chipmunk \
            MS_Ref/source/image_proc/src/Chipmunk-6.0.0/include/chipmunk/constraints 
            
MINCPATH += chip_drv/export/inc/outdated
                            
MSRCPATH =  MS_Ref/source/image_proc/src \
            MS_Ref/source/image_proc/src/scaling/src \
            MS_Ref/source/image_proc/src/bmp_decode/src \
            MS_Ref/source/image_proc/src/cascade/src \
            MS_Ref/source/image_proc/src/sharpen/src \
            MS_Ref/source/image_proc/src/exif/src \
            MS_Ref/source/image_proc/src/gif_decode/src \
            MS_Ref/source/image_proc/src/png_decode/src \
            MS_Ref/source/image_proc/src/jpeg/src \
            MS_Ref/source/image_proc/src/img_proc/src \
            MS_Ref/source/image_proc/src/jpeg_codec/src \
            MS_Ref/source/image_proc/src/photo_editor/src \
            MS_Ref/source/image_proc/src/gzip/src \
            MS_Ref/source/image_proc/src/ui_special_effect/src \
            MS_Ref/source/image_proc/src/jpeg/image_decoder/src \
            MS_Ref/source/image_proc/src/common/src\
            MS_Ref/source/image_proc/src/img_dec/src \
            MS_Ref/source/image_proc/src/isp/src \
            MS_Ref/source/image_proc/src/dc/src \
            MS_Ref/source/image_proc/src/img_transform/src \
            MS_Ref/source/image_proc/src/ico/src \
            MS_Ref/source/image_proc/src/Chipmunk-6.0.0/src \
						MS_Ref/source/image_proc/src/Chipmunk-6.0.0/src/constraints \
						MS_Ref/source/image_proc/src/Chipmunk-6.0.0/demo \
						MS_Ref/source/image_proc/src/img_proc/inc
						
ifeq ($(strip $(PLATFORM)), SC6600L)
MINCPATH += MS_Ref/source/vsp/sc6600l/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6600l/inc 
            MS_Ref/source/dc/dc_6x00/inc
endif  

ifeq ($(strip $(PLATFORM)), SC6800H)
MINCPATH += MS_Ref/source/vsp/sc6800h/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6800h/inc  \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/dc/dc_6x00/inc
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

ifeq ($(strip $(PLATFORM)), SC8800G)
MINCPATH += MS_Ref/source/vsp/sc8800g/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_8800g/inc
endif
   
ifeq ($(strip $(PLATFORM)), SC6600L)
MSRCPATH =  MS_Ref/source/image_proc/src \
            MS_Ref/source/image_proc/src/scaling/src \
            MS_Ref/source/image_proc/src/bmp_decode/src \
            MS_Ref/source/image_proc/src/cascade/src \
            MS_Ref/source/image_proc/src/sharpen/src \
            MS_Ref/source/image_proc/src/exif/src \
            MS_Ref/source/image_proc/src/gif_decode/src \
            MS_Ref/source/image_proc/src/png_decode/src \
            MS_Ref/source/image_proc/src/jpeg/src \
            MS_Ref/source/image_proc/src/img_proc/src \
            MS_Ref/source/image_proc/src/jpeg_codec/src \
            MS_Ref/source/image_proc/src/photo_editor/src \
            MS_Ref/source/image_proc/src/gzip/src \
            MS_Ref/source/image_proc/src/ui_special_effect/src \
            MS_Ref/source/image_proc/src/jpeg/image_decoder/src \
            MS_Ref/source/image_proc/src/common/src\
            MS_Ref/source/image_proc/src/img_dec/src \
            MS_Ref/source/image_proc/src/isp/src \
            MS_Ref/source/image_proc/src/dc/src \
            MS_Ref/source/image_proc/src/img_transform/src \
            MS_Ref/source/image_proc/src/Chipmunk-6.0.0/src \
						MS_Ref/source/image_proc/src/Chipmunk-6.0.0/src/constraints \
						MS_Ref/source/image_proc/src/Chipmunk-6.0.0/demo \
						MS_Ref/source/image_proc/src/img_proc/src
endif
									
ifeq ($(strip $(PLATFORM)), SC6600L)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6600l/src
endif  

ifeq ($(strip $(PLATFORM)), SC6800H)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6800h/src 
endif    		

ifeq ($(strip $(PLATFORM)), SC6530)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src  
endif  

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src 
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src 
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_8800g/src 
endif 													

SOURCES     += image_common.c

SOURCES     += img_cascade.c
SOURCES     += img_dec_interface.c img_dec_task.c img_dec_hal.c img_dec_file.c	img_jpeg_decoder.c

ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES     += isp_algorithm.c dc_algorithm.c
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES     += isp_algorithm.c dc_algorithm.c sin_cos.c gen_scale_coef_6x00.c
endif	

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES     += sin_cos.c gen_scale_coef_6530.c
endif	

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES     += sin_cos.c gen_scale_coef_6530.c
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES     += sin_cos.c gen_scale_coef_6530.c
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES     += sin_cos.c gen_scale_coef.c
endif

ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
SOURCES			+= ui_special_effect.c
endif

ifeq ($(strip $(PNG_DEC_SUPPORT)), PNG_SW)
SOURCES			+= adler32.c alpha.c crc32.c inffast.c inflate.c inftrees.c png.c png_app.c png_decode.c pngerror.c \
							 pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c zutil.c \
							 png_app_ctrl.c 
endif

ifeq ($(strip $(BMP_DEC_SUPPORT)), BMP_SW)
SOURCES			+= wbmp_decode.c bmp_decode.c bmp_app.c bmp_task.c
endif 
		
# ifeq ($(strip $(GZIP_DEC_SUPPORT)), GZIP_SW) 		
# SOURCES			+= img_gzip_bits.c img_gzip_deflate.c img_gzip_gzip.c img_gzip_inflate.c img_gzip_lzw.c \
# 			   		   img_gzip_platform.c img_gzip_trees.c img_gzip_unlzh.c img_gzip_unlzw.c img_gzip_unpack.c \
# 			   		   img_gzip_unzip.c img_gzip_util.c img_gzip_zip.c 
# endif

ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
SOURCES			+= pe_process.c
endif

#JPEG common file

ifeq ($(strip $(PLATFORM)), SC6600L)
MINCPATH += MS_Ref/source/vsp/sc6600l/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6600l/inc 
            MS_Ref/source/dc/dc_6x00/inc
endif  

ifeq ($(strip $(PLATFORM)), SC6800H)
MINCPATH += MS_Ref/source/vsp/sc6800h/inc \
            MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6800h/inc  \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/dc/dc_6x00/inc
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
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6600l/src
endif  

ifeq ($(strip $(PLATFORM)), SC6800H)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6800h/src 
endif    		

ifeq ($(strip $(PLATFORM)), SC6530)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src  
endif  

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src  
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_6530/src  
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MSRCPATH += MS_Ref/source/image_proc/src/jpeg/jpeg_fw_8800g/src 
endif 
                
#jpeg comman file 
SOURCES			+= jpeg_interface.c jpeg_hal.c jpeg_head_info.c  \
								jpeg_scaling_init.c jpeg_scaling_adv.c jpeg_coeff_table_adv.c
SOURCES			+= jpeg_stream.c exif_writer.c exif_reader.c								
								
ifeq ($(strip $(JPG_DEC_SUPPORT)), JPG_HW)
SOURCES			+=  jpeg_algorithm.c

ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES			+= jpeg_midware_sc6600l.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegEnc_write.c JpegEnc_init.c JpegEnc_frame.c JpegEnc_vlc.c JpegEnc_quant.c JpegEnc_stream.c \
						   JpegDec_parse.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c JpegDec_stream.c JpegDec_vld.c \
	 						 rearrange.c						  				  						
endif		

#6800h jpeg firmware code
ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES			+= jpeg_midware.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegDec_bitstream.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c \
						   JpegDec_interface.c JpegDec_malloc.c JpegDec_out.c JpegDec_parse.c  \
						   JpegDec_pvld.c JpegDec_vld.c \
						   JpegEnc_bitstream.c JpegEnc_frame.c JpegEnc_header.c JpegEnc_init.c  \
						   JpegEnc_interface.c JpegEnc_malloc.c 			
endif		

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES			+= jpeg_midware.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegDec_bitstream.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c \
						   JpegDec_interface.c JpegDec_malloc.c JpegDec_out.c JpegDec_parse.c  \
						   JpegDec_pvld.c JpegDec_vld.c \
						   JpegEnc_bitstream.c JpegEnc_frame.c JpegEnc_header.c JpegEnc_init.c  \
						   JpegEnc_interface.c JpegEnc_malloc.c			
endif	

ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES			+= jpeg_midware.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegDec_bitstream.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c \
						   JpegDec_interface.c JpegDec_malloc.c JpegDec_out.c JpegDec_parse.c  \
						   JpegDec_pvld.c JpegDec_vld.c \
						   JpegEnc_bitstream.c JpegEnc_frame.c JpegEnc_header.c JpegEnc_init.c  \
						   JpegEnc_interface.c JpegEnc_malloc.c	
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES			+= jpeg_midware.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegDec_bitstream.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c \
						   JpegDec_interface.c JpegDec_malloc.c JpegDec_out.c JpegDec_parse.c  \
						   JpegDec_pvld.c JpegDec_vld.c \
						   JpegEnc_bitstream.c JpegEnc_frame.c JpegEnc_header.c JpegEnc_init.c  \
						   JpegEnc_interface.c JpegEnc_malloc.c	
endif

#8800G jpeg firmware code
ifeq ($(strip $(PLATFORM)), SC8800G)
SOURCES			+= jpeg_midware.c
SOURCES			+= JpegCodec_bufmgr.c JpegCodec_global.c JpegCodec_table.c \
						   JpegDec_bitstream.c JpegDec_dequant.c JpegDec_frame.c JpegDec_init.c \
						   JpegDec_interface.c JpegDec_malloc.c JpegDec_out.c JpegDec_parse.c  \
						   JpegDec_pvld.c JpegDec_vld.c \
						   JpegEnc_bitstream.c JpegEnc_frame.c JpegEnc_header.c JpegEnc_init.c  \
						   JpegEnc_interface.c JpegEnc_malloc.c 			
endif	
endif

ifeq ($(strip $(JPG_DEC_SUPPORT)), JPG_SW)
SOURCES			+= jpeg_software.c
		
SOURCES			+= jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
							 jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c jctrans.c \
							 jdapimin.c jdapistd.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c \
							 jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
							 jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jmemmgr.c jmemnobs.c jquant1.c jquant2.c jutils.c \
						   jump_arm.s japp.c  jsimd.c				  	
endif

ifeq ($(strip $(JPG_ENC_SUPPORT)), JPG_SW)
SOURCES			+= jpeg_software.c
		
SOURCES			+= jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
							 jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c jctrans.c \
							 jdapimin.c jdapistd.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c \
							 jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
							 jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jmemmgr.c jmemnobs.c jquant1.c jquant2.c jutils.c \
						   jump_arm.s japp.c jsimd.c				  	
endif


ifneq ($(strip $(JPG_DEC_SUPPORT)), JPG_NONE)
ifeq ($(strip $(JPG_DEC_TYPE_SUPPORT)), BASELINE_PROGRESSIVE)
#for progressive jpeg decode
SOURCES			+= jpeg_software.c jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
						   jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c jctrans.c \
						   jdapimin.c jdapistd.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c \
						   jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
						   jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jmemmgr.c jmemnobs.c jquant1.c jquant2.c jutils.c \
						   jump_arm.s japp.c jsimd.c	 		 					 						 	 
endif				
endif  		


ifeq ($(strip $(CM_SUPPORT)), TRUE)
SOURCES			+= cpArray.c cpSweep1D.c cpArbiter.c cpBB.c cpBBTree.c cpHashSet.c cpPolyShape.c \
               cpShape.c cpSpaceComponent.c cpSpaceHash.c cpSpatialIndex.c cpVect.c  \
               chipmunk.c cpBody.c cpCollision.c cpSpace.c cpSpaceQuery.c cpSpaceStep.c
                              
SOURCES			+= cpConstraint.c cpGearJoint.c cpGrooveJoint.c cpPinJoint.c cpPivotJoint.c cpRatchetJoint.c \
               cpRotaryLimitJoint.c cpSimpleMotor.c cpSlideJoint.c cpDampedRotarySpring.c cpDampedSpring.c     
               
SOURCES     += Plink.c drawSpace.c ChipmunkDemo.c Tumble.c Balloon.c new_balloon.c
							 #Bench.c Bounce.c ChipmunkDemo.c drawSpace.c Joints.c LogoSmash.c MagnetsElectric.c \
               #OneWay.c Planet.c Player.c	 Pump.c PyramidStack.c PyramidTopple.c Query.c \
               #Sensors.c Simple.c Springies.c Tank.c TheoJansen.c Tumble.c UnsafeOps.c
endif
 
#gif deocder
ifeq ($(strip $(GIF_DEC_SUPPORT)), GIF_SW)
SOURCES			+= gif_decoder.c gif_decode_app.c gif_reentrant_decoder.c gif_app_ctrl.c
endif

#ico decoder
SOURCES			+= ico_decode.c 

#windowed-sinc scaling for bmp/png/gif decoder
SOURCES     += coeff_table_def_cus.c scaling_down_cus.c 

#image scale                		
SOURCES			+= scale_image.c scale_hardware.c imgproc_asm.s 
SOURCES			+= scale_software.c 

#yuv/rgb convert interface
SOURCES  		+= yuv_rgb_convert.c rgb_yuv_convert.c 

#for image editer
ifeq ($(strip $(IMG_EDITER_SUPPORT)), TRUE)
SOURCES			+= img_proc.c img_proc_cfg.c img_alg_common.c img_proc_sqrt.s isp_face_detection_asm.s \
               img_proc_alg_n_in_one.c img_proc_face_dect.c img_proc_alg_hdr.c img_alg_fisheye.c \
               img_proc_lomo.c img_alg_pencilsketch.c img_proc_alg_xray.c img_proc_alg_infrared.c \
               Img_alg_frame.c img_proc_alg_negative.c img_proc_alg_exposal.c
endif

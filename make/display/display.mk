#MCFLAG_OPT		+=  -DSGL_SUPPORT

ifeq ($(strip $(RELEASE_INFO)),FALSE)
MCFLAG_OPT		+=  -DIMG_ASSERT_MODE
endif

#ifeq ($(strip $(PLATFORM)), SC6600L)
#MCFLAG_OPT              += -D_IMG_6600L
#endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MCFLAG_OPT += -DPLATFORM_SC6531EFM
endif

MINCPATH =  MS_Ref/source/display/inc \
            MS_Ref/source/isp_service/inc \
            MS_Ref/source/image_proc/inc \
            MS_Ref/source/display/src/graphics/inc \
            MS_Ref/source/display/src/special_effect/inc
                        
MINCPATH += chip_drv/export/inc/outdated
            
MSRCPATH =  MS_Ref/source/display/src/img MS_Ref/source/display/src/graphics/src MS_Ref/source/display/src/special_effect/src MS_Ref/source/display/src/common

MINCPATH += chip_drv/export/inc

ifeq ($(strip $(PLATFORM)),	SC8800G)
MINCPATH += chip_drv/chip_plf/export/inc
endif

ifeq ($(strip $(SGL_SUPPORT)), TRUE)   
MINCPATH += MS_Ref/source/display/src/sgl/inc
MSRCPATH += MS_Ref/source/display/src/sgl/src
endif

MCFLAG_OPT     += -O2 -Otime 

#use rom code in platform sc6530
ifneq ($(strip $(PLATFORM)), SC6530)
ifneq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES			+= s2d_rotate_kernal.c
endif
endif

SOURCES 		+= s2d_rotate.c s2d_s3d_use.c rotate_image.c

ifeq ($(strip $(SOFTWARE_ROTATION_SUPPORT)),TRUE)
SOURCES 		+= rotate_software.c
endif

ifeq ($(strip $(ROTATE_2D_SUPPORT)),FALSE)
SOURCES			+= s3d_rotate_se.c
endif

SOURCES 		+= s3d_rotation.c
SOURCES 		+= scale_image.c draw_round_rect.c 
SOURCES                 += diffusion_scale_image.c
SOURCES                 += draw_polygon.c


SOURCES 		+= special_effect_main.c 
SOURCES 		+= special_effect_util.c 
SOURCES 		+= blend_algorithm.c

                          
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE) 
MCFLAG_OPT	+=  -DROTATE_2D_SUPPORT
SOURCES 		+= draw_clock.c 
endif

ifeq ($(strip $(SE_FIRE_SUPPORT)), TRUE) 
SOURCES 		+= special_effect_fire.c 
endif
ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE) 
SOURCES 		+= special_effect_ripple.c 
endif
ifeq ($(strip $(SE_VISUAL_SUPPORT)), TRUE)
SOURCES 		+= special_effect_visual.c 
endif

ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
SOURCES			+= page_turn.c 
SOURCES 		+= special_effect_page_turn.c
endif

ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
SOURCES			+= roll.c 
SOURCES 		+= special_effect_roll.c 
endif

ifeq ($(strip $(SE_SOLID_SUPPORT)), TRUE)
SOURCES 		+= solid.c 
SOURCES 		+= special_effect_solid.c 
endif

ifeq ($(strip $(SE_LEAF_SUPPORT)), TRUE)                        
SOURCES			+= special_effect_ripple_ex.c 
SOURCES 		+= special_effect_leaf.c 
SOURCES 		+= special_effect_ripple.c 
endif
ifeq ($(strip $(SE_GRID_SUPPORT)), TRUE)    
SOURCES			+= special_effect_grid.c 
endif
ifeq ($(strip $(CM_SUPPORT)), TRUE)                        
SOURCES			+= special_effect_collison.c
endif
ifeq ($(strip $(SE_CRT_SUPPORT)), TRUE)
SOURCES			+= special_effect_crt.c   
endif     

#ifeq ($(strip $(SE_CURTAIN_SUPPORT)), TRUE)
SOURCES			+= special_effect_curtain.c   
#endif    

ifeq ($(strip $(SGL_SUPPORT)), TRUE)   
SOURCES			+= draw_dot.c draw_line.c \
							 sgl_canvas.c \
							 sgl_scene.c \
							 sgl_shape.c \
							 sgl_shape_line.c \
							 sgl_shape_polygon.c \
							 sgl_shape_texture_plane.c     
endif

/******************************************************************************
 ** File Name:      sci_trace_mod.h                                           *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/
#ifndef _SCI_TRACE_MOD_H
#define _SCI_TRACE_MOD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

typedef enum
{
ATC_3G_TRC=0x1,				    //0x1		\MS_Code\atc_3g						    \MS_Code\common\export\inc\trace_id 
BASE_TRC,				    	//0x2		\MS_Code\Base							\MS_Code\common\export\inc\trace_id                                                             
CAF_TRC,						//0x3 	    \MS_Code\CAF							\MS_Code\common\export\inc\trace_id
CHIP_LIB_TRC,					//0x4 	    \MS_Code\chip_lib					    \MS_Code\common\export\inc\trace_id
CHIP_DRV_TRC,					//0x5		\MS_Code\chip_drv					    \MS_Code\common\export\inc\trace_id                                                        
CMMB_TRC,						//0x6		\MS_Code\CMMB							\MS_Code\common\export\inc\trace_id		                                          
DAPS_TRC,						//0x7		\MS_Code\DAPS							\MS_Code\common\export\inc\trace_id	                                                  
                                                                                                                                          
DM_2G_LAYER1_TRC,			    //0x8		\MS_Code\dm_2g_layer1			        \MS_Code\common\export\inc\trace_id                                          
DM_2G_LAYER1_DSP_TRC,           //0x9		\MS_Code\dm_2g_layer1_dsp	            \MS_Code\common\export\inc\trace_id                                      
DM_2G_PS_TRC,					//0xA		\MS_Code\dm_2g_ps					    \MS_Code\common\export\inc\trace_id                                              
EXTERNAL_TRC,					//0xB		\MS_Code\external					    \MS_Code\common\export\inc\trace_id                                                                                                                                          
MS_CUSTOMIZE_TRC,			    //0xC		\MS_Code\MS_Customize			        \MS_Code\common\export\inc\trace_id                                                  
PARSER_TRC,						//0xD		\MS_Code\PARSER						    \MS_Code\common\export\inc\trace_id
RTOS_TRC,						//0xE		\MS_Code\RTOS							\MS_Code\common\export\inc\trace_id                                              
WRE_TRC,                        //0xF	    \MS_Code\wre							\MS_Code\common\export\inc\trace_id
FDL_BOOTLOADER_TRC,             //0x10      \MS_Code\fdl_bootloader                 \MS_Code\common\export\inc\trace_id
//MS_REF		                                                                                                                  
MS_REF_AUD_CONFIG_TRC = 0x20,	//0x20	\MS_Code\MS_Ref\source\aud_config	        \MS_Code\common\export\inc\trace_id                     
MS_REF_AUD_DEV_TRC,			    //0x21	\MS_Code\MS_Ref\source\aud_dev			    \MS_Code\common\export\inc\trace_id                                
MS_REF_AUD_EXP_TRC,			    //0x22	\MS_Code\MS_Ref\source\aud_exp			    \MS_Code\common\export\inc\trace_id                                
MS_REF_AUDIOSERVICE_TRC,	    //0x23	\MS_Code\MS_Ref\source\audioservice		    \MS_Code\common\export\inc\trace_id 
MS_REF_BASE_TRC,			    //0x24	\MS_Code\MS_Ref\source\base				    \MS_Code\common\export\inc\trace_id                                   
MS_REF_BSD_TRC,				    //0x25	\MS_Code\MS_Ref\source\bsd				    \MS_Code\common\export\inc\trace_id                                    
MS_REF_BT_PAL_CSR_TRC,		    //0x26	\MS_Code\MS_Ref\source\bt_pal_csr		    \MS_Code\common\export\inc\trace_id                     
MS_REF_AUD_ALGORITHM_TRC,		//0x27	\MS_Code\MS_Ref\source\aud_algorithm		\MS_Code\common\export\inc\trace_id 
MS_REF_CDROM_TRC,               //0x28  \MS_Code\MS_Ref\source\cdrom                \MS_Code\common\export\inc\trace_id
MS_REF_CODEC_TRC,			    //0x29	\MS_Code\MS_Ref\source\codec			    \MS_Code\common\export\inc\trace_id                                  
MS_REF_DC_TRC,				    //0x2A	\MS_Code\MS_Ref\source\dc				    \MS_Code\common\export\inc\trace_id                                     
MS_REF_DISK_TRC,                //0x2B  \MS_Code\MS_Ref\source\disk                 \MS_Code\common\export\inc\trace_id
MS_REF_DIPLAY_TRC,              //0x2C  \MS_Code\MS_Ref\source\display               \MS_Code\common\export\inc\trace_id
MS_REF_DV_TRC,				    //0x2D	\MS_Code\MS_Ref\source\dv				    \MS_Code\common\export\inc\trace_id                                     
MS_REF_EFS_TRC,				    //0x2E	\MS_Code\MS_Ref\source\efs				    \MS_Code\common\export\inc\trace_id                                    
MS_REF_FFS_TRC,				    //0x2F	\MS_Code\MS_Ref\source\ffs				    \MS_Code\common\export\inc\trace_id
MS_REF_FOTA_TRC,			    //0x30	\MS_Code\MS_Ref\source\fota				    \MS_Code\common\export\inc\trace_id
MS_REF_FTL_TRC,			        //0x31	\MS_Code\MS_Ref\source\ftl				    \MS_Code\common\export\inc\trace_id                                  
MS_REF_GZIP_TRC,			    //0x32	\MS_Code\MS_Ref\source\gzip				    \MS_Code\common\export\inc\trace_id
MS_REF_H264_CODEC_TRC,		    //0x33	\MS_Code\MS_Ref\source\h264_codec		    \MS_Code\common\export\inc\trace_id
MS_REF_H324_ADP_TRC,		    //0x34	\MS_Code\MS_Ref\source\h324_adp			    \MS_Code\common\export\inc\trace_id
MS_REF_IMAGE_PROC_TRC,		    //0x35	\MS_Code\MS_Ref\source\image_proc		    \MS_Code\common\export\inc\trace_id                
MS_REF_ISP_TRC,				    //0x36	\MS_Code\MS_Ref\source\isp				    \MS_Code\common\export\inc\trace_id                                 
MS_REF_ISP_SERVICE_TRC,		    //0x37	\MS_Code\MS_Ref\source\isp_service		    \MS_Code\common\export\inc\trace_id                  
MS_REF_LCD_TRC,		            //0x38	\MS_Code\MS_Ref\source\lcd      		    \MS_Code\common\export\inc\trace_id
MS_REF_LZMA_TRC,		        //0x39	\MS_Code\MS_Ref\source\lzma      		    \MS_Code\common\export\inc\trace_id                  
MS_REF_MPEG4_CODEC_TRC,		    //0x3A	\MS_Code\MS_Ref\source\mpeg4_codec		    \MS_Code\common\export\inc\trace_id                    
MS_REF_MTP_TRC,     		    //0x3B	\MS_Code\MS_Ref\source\mtp      		    \MS_Code\common\export\inc\trace_id                    
MS_REF_MUX_TRC,     		    //0x3C	\MS_Code\MS_Ref\source\mux      		    \MS_Code\common\export\inc\trace_id                    
MS_REF_NVM_TRC,				    //0x3D	\MS_Code\MS_Ref\source\nvm				    \MS_Code\common\export\inc\trace_id
MS_REF_ROTATION_TRC,		    //0x3E	\MS_Code\MS_Ref\source\rotation			    \MS_Code\common\export\inc\trace_id                                
MS_REF_RV_CODEC_TRC,		    //0x3F	\MS_Code\MS_Ref\source\rv_codec			    \MS_Code\common\export\inc\trace_id                                
MS_REF_SCALING_TRC,			    //0x40	\MS_Code\MS_Ref\source\scaling			    \MS_Code\common\export\inc\trace_id                                
MS_REF_SCI_FTL_LAYER_TRC,	    //0x41	\MS_Code\MS_Ref\source\sci_ftl_layer	    \MS_Code\common\export\inc\trace_id                                
MS_REF_SCM_TRC,				    //0x42	\MS_Code\MS_Ref\source\scm				    \MS_Code\common\export\inc\trace_id                                  
MS_REF_SCP_TRC,				    //0x43	\MS_Code\MS_Ref\source\scp				    \MS_Code\common\export\inc\trace_id                                   
MS_REF_SFS_TRC,				    //0x44	\MS_Code\MS_Ref\source\sfs				    \MS_Code\common\export\inc\trace_id                                    
MS_REF_SPZIP_TRC,			    //0x45	\MS_Code\MS_Ref\source\spzip			    \MS_Code\common\export\inc\trace_id                                    
MS_REF_STREAMING_CODEC_TRC,	    //0x46	\MS_Code\MS_Ref\source\streaming_codec	    \MS_Code\common\export\inc\trace_id                                    
MS_REF_THEME_UNZIP_TRC,		    //0x47	\MS_Code\MS_Ref\source\theme_unzip		    \MS_Code\common\export\inc\trace_id                                    
MS_REF_UBOT_TRC,			    //0x48	\MS_Code\MS_Ref\source\ubot				    \MS_Code\common\export\inc\trace_id                                   
MS_REF_UCOM_TRC,			    //0x49	\MS_Code\MS_Ref\source\ucom				    \MS_Code\common\export\inc\trace_id                                  
MS_REF_UMEM_TRC,			    //0x4A	\MS_Code\MS_Ref\source\umem				    \MS_Code\common\export\inc\trace_id                                  
MS_REF_UMOD_TRC,			    //0x4B	\MS_Code\MS_Ref\source\umod				    \MS_Code\common\export\inc\trace_id                               
MS_REF_UPCC_TRC,			    //0x4C	\MS_Code\MS_Ref\source\upcc				    \MS_Code\common\export\inc\trace_id                                
MS_REF_USBSERVICE_TRC,		    //0x4D	\MS_Code\MS_Ref\source\usbservice		    \MS_Code\common\export\inc\trace_id                                
MS_REF_VIDEO_SECURITY_TRC,	    //0x4E	\MS_Code\MS_Ref\source\video_security	    \MS_Code\common\export\inc\trace_id                                
MS_REF_VIDEO_SECURITY_KMV_TRC,	//0x4F	\MS_Code\MS_Ref\source\video_security_kmv   \MS_Code\common\export\inc\trace_id                                
MS_REF_VSP_TRC,				    //0x50	\MS_Code\MS_Ref\source\vsp				    \MS_Code\common\export\inc\trace_id                                    
MS_REF_XSR_LAYER_TRC,		    //0x51	\MS_Code\MS_Ref\source\xsr_layer		    \MS_Code\common\export\inc\trace_id 
MS_REF_VIDEO_SECURITY_HMV_TRC,  //0x52  \MS_Code\MS_Ref\source\video_security_hmv       \MS_Code\common\export\inc\trace_id                                                                                                                            
//MMI_APP                                                                                                                                 
MMI_APP_COMMON_TRC = 0x90,	    //0x90	\MS_Code\MS_MMI\source\mmi_app\common			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_KERNEL_TRC,			    //0x91	\MS_Code\MS_MMI\source\mmi_app\kernel			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_CUSTOM_TRC,			    //0x92	\MS_Code\MS_MMI\source\mmi_app\custom			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_ACCESSORY_TRC,		    //0x93	\MS_Code\MS_MMI\source\mmi_app\app\accessory	    \MS_Code\common\export\inc\trace_id    
MMI_APP_ASP_TRC,		        //0x94	\MS_Code\MS_MMI\source\mmi_app\app\asp	            \MS_Code\common\export\inc\trace_id    
MMI_APP_ATEST_TRC,		        //0x95	\MS_Code\MS_MMI\source\mmi_app\app\atest	        \MS_Code\common\export\inc\trace_id    
MMI_APP_ATV_TRC,		        //0x96	\MS_Code\MS_MMI\source\mmi_app\app\atv	            \MS_Code\common\export\inc\trace_id    
MMI_APP_AUDIO_TRC,			    //0x97	\MS_Code\MS_MMI\source\mmi_app\app\audio		    \MS_Code\common\export\inc\trace_id     
MMI_APP_AUDIOPLAYER_TRC,	    //0x98	\MS_Code\MS_MMI\source\mmi_app\app\audioplayer		\MS_Code\common\export\inc\trace_id     
MMI_APP_AUT_TRC,		        //0x99	\MS_Code\MS_MMI\source\mmi_app\app\aut	            \MS_Code\common\export\inc\trace_id    
MMI_APP_AZAN_TRC,		        //0x9A	\MS_Code\MS_MMI\source\mmi_app\app\azan	            \MS_Code\common\export\inc\trace_id    
MMI_APP_BROWSER_TRC,		    //0x9B	\MS_Code\MS_MMI\source\mmi_app\app\browser	        \MS_Code\common\export\inc\trace_id    
MMI_APP_BROWSER_MANAGER_TRC,	//0x9C	\MS_Code\MS_MMI\source\mmi_app\app\browser_manager	\MS_Code\common\export\inc\trace_id    
MMI_APP_BT_TRC,				    //0x9D	\MS_Code\MS_MMI\source\mmi_app\app\bt			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_CAMERA_TRC,			    //0x9E	\MS_Code\MS_MMI\source\mmi_app\app\camera		    \MS_Code\common\export\inc\trace_id    
MMI_APP_CC_TRC,				    //0x9F	\MS_Code\MS_MMI\source\mmi_app\app\cc			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_CL_TRC,				    //0xA0	\MS_Code\MS_MMI\source\mmi_app\app\cl			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_CLIPBRD_TRC,		    //0xA1	\MS_Code\MS_MMI\source\mmi_app\app\clipbrd	        \MS_Code\common\export\inc\trace_id    
MMI_APP_CONNECTION_TRC,		    //0xA2	\MS_Code\MS_MMI\source\mmi_app\app\connection	    \MS_Code\common\export\inc\trace_id 
MMI_APP_CS_TRC,		            //0xA3	\MS_Code\MS_MMI\source\mmi_app\app\cs	            \MS_Code\common\export\inc\trace_id    
MMI_APP_CUSTOMER_TRC,		    //0xA4	\MS_Code\MS_MMI\source\mmi_app\app\customer	        \MS_Code\common\export\inc\trace_id    
MMI_APP_DCD_TRC,		        //0xA5	\MS_Code\MS_MMI\source\mmi_app\app\dcd	            \MS_Code\common\export\inc\trace_id    
MMI_APP_DL_TRC,		            //0xA6	\MS_Code\MS_MMI\source\mmi_app\app\dl	            \MS_Code\common\export\inc\trace_id    
MMI_APP_DM_TRC,		            //0xA7	\MS_Code\MS_MMI\source\mmi_app\app\dm	            \MS_Code\common\export\inc\trace_id    
MMI_APP_DRM_TRC,		        //0xA8	\MS_Code\MS_MMI\source\mmi_app\app\drm	            \MS_Code\common\export\inc\trace_id    
MMI_APP_DROPDOWNWIN_TRC,		//0xA9	\MS_Code\MS_MMI\source\mmi_app\app\dropdownwin	    \MS_Code\common\export\inc\trace_id    
MMI_APP_DYNAMIC_TRC,		    //0xAA	\MS_Code\MS_MMI\source\mmi_app\app\dynamic		    \MS_Code\common\export\inc\trace_id      
MMI_APP_EBOOK_TRC,			    //0xAB	\MS_Code\MS_MMI\source\mmi_app\app\ebook		    \MS_Code\common\export\inc\trace_id      
MMI_APP_EMAIL_TRC,		        //0xAC	\MS_Code\MS_MMI\source\mmi_app\app\email	        \MS_Code\common\export\inc\trace_id    
MMI_APP_ENG_TRC,			    //0xAD	\MS_Code\MS_MMI\source\mmi_app\app\eng			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_ENGTD_TRC,			    //0xAE	\MS_Code\MS_MMI\source\mmi_app\app\engtd		    \MS_Code\common\export\inc\trace_id      
MMI_APP_ENVSET_TRC,			    //0xAF	\MS_Code\MS_MMI\source\mmi_app\app\envset		    \MS_Code\common\export\inc\trace_id      
MMI_APP_FDN_TRC,		        //0xB0	\MS_Code\MS_MMI\source\mmi_app\app\fdn	            \MS_Code\common\export\inc\trace_id    
MMI_APP_FLASH_WIN_TRC,		    //0xB1	\MS_Code\MS_MMI\source\mmi_app\app\flash_win	    \MS_Code\common\export\inc\trace_id    
MMI_APP_FM_TRC,				    //0xB2	\MS_Code\MS_MMI\source\mmi_app\app\fm			    \MS_Code\common\export\inc\trace_id                
MMI_APP_FMM_TRC,			    //0xB3	\MS_Code\MS_MMI\source\mmi_app\app\fmm			    \MS_Code\common\export\inc\trace_id                
MMI_APP_GAME_TRC,			    //0xB4	\MS_Code\MS_MMI\source\mmi_app\app\game			    \MS_Code\common\export\inc\trace_id                
MMI_APP_GPS_TRC,			    //0xB5	\MS_Code\MS_MMI\source\mmi_app\app\gps			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_IA_JAVA_TRC,		    //0xB6	\MS_Code\MS_MMI\source\mmi_app\app\ia_java	        \MS_Code\common\export\inc\trace_id    
MMI_APP_IDLE_TRC,			    //0xB7	\MS_Code\MS_MMI\source\mmi_app\app\idle			    \MS_Code\common\export\inc\trace_id                
MMI_APP_IKEYBACKUP_TRC,		    //0xB8	\MS_Code\MS_MMI\source\mmi_app\app\ikeybackup	    \MS_Code\common\export\inc\trace_id    
MMI_APP_IM_TRC,				    //0xB9	\MS_Code\MS_MMI\source\mmi_app\app\im			    \MS_Code\common\export\inc\trace_id                
MMI_APP_JAVA_TRC,			    //0xBA	\MS_Code\MS_MMI\source\mmi_app\app\java			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_KEYLOCK_TRC,		    //0xBB	\MS_Code\MS_MMI\source\mmi_app\app\keylock	        \MS_Code\common\export\inc\trace_id    
MMI_APP_KMOVIE_TRC,		        //0xBC	\MS_Code\MS_MMI\source\mmi_app\app\kmovie	        \MS_Code\common\export\inc\trace_id    
MMI_APP_KURO_TRC,			    //0xBD	\MS_Code\MS_MMI\source\mmi_app\app\kuro			    \MS_Code\common\export\inc\trace_id               
MMI_APP_LIVEWALLPAPER_TRC,		//0xBE	\MS_Code\MS_MMI\source\mmi_app\app\livewallpaper	\MS_Code\common\export\inc\trace_id    
MMI_APP_MEMO_TRC,		        //0xBF	\MS_Code\MS_MMI\source\mmi_app\app\memo	            \MS_Code\common\export\inc\trace_id    
MMI_APP_MENU_TRC,			    //0xC0	\MS_Code\MS_MMI\source\mmi_app\app\menu			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_MET_MEX_TRC,            //0xC1  \MS_Code\MS_MMI\source\mmi_app\app\met_mex          \MS_Code\common\export\inc\trace_id
MMI_APP_MMI3D_TRC,			    //0xC2	\MS_Code\MS_MMI\source\mmi_app\app\mmi3d		    \MS_Code\common\export\inc\trace_id       
MMI_APP_MMISE_TRC,		        //0xC3	\MS_Code\MS_MMI\source\mmi_app\app\mmise	        \MS_Code\common\export\inc\trace_id    
MMI_APP_MMS_TRC,			    //0xC4	\MS_Code\MS_MMI\source\mmi_app\app\mms		        \MS_Code\common\export\inc\trace_id    
MMI_APP_MOBILE_VIDEO_TRC,		//0xC5	\MS_Code\MS_MMI\source\mmi_app\app\mobile_video	    \MS_Code\common\export\inc\trace_id    
MMI_APP_MOBILETV_TRC,		    //0xC6	\MS_Code\MS_MMI\source\mmi_app\app\mobiletv		    \MS_Code\common\export\inc\trace_id
MMI_APP_MSD_TRC,			    //0xC7	\MS_Code\MS_MMI\source\mmi_app\app\msd			    \MS_Code\common\export\inc\trace_id               
MMI_APP_MULTIM_TRC,			    //0xC8	\MS_Code\MS_MMI\source\mmi_app\app\multim		    \MS_Code\common\export\inc\trace_id       
MMI_APP_MYRIAD_JAVA_TRC,		//0xC9	\MS_Code\MS_MMI\source\mmi_app\app\myriad_java	    \MS_Code\common\export\inc\trace_id    
MMI_APP_OCR_TRC,			    //0xCA	\MS_Code\MS_MMI\source\mmi_app\app\ocr			    \MS_Code\common\export\inc\trace_id              
MMI_APP_OPERA_TRC,		        //0xCB	\MS_Code\MS_MMI\source\mmi_app\app\operamini6	    \MS_Code\common\export\inc\trace_id    
MMI_APP_OTAPUSH_TRC,		    //0xCC	\MS_Code\MS_MMI\source\mmi_app\app\otapush	        \MS_Code\common\export\inc\trace_id    
MMI_APP_PARSE_TRC,		        //0xCD	\MS_Code\MS_MMI\source\mmi_app\app\parse	        \MS_Code\common\export\inc\trace_id    
MMI_APP_PB_TRC,				    //0xCE	\MS_Code\MS_MMI\source\mmi_app\app\pb			    \MS_Code\common\export\inc\trace_id                  
MMI_APP_PDP_TRC,		        //0xCF	\MS_Code\MS_MMI\source\mmi_app\app\pdp	            \MS_Code\common\export\inc\trace_id    
MMI_APP_PHONE_TRC,			    //0xD0	\MS_Code\MS_MMI\source\mmi_app\app\phone		    \MS_Code\common\export\inc\trace_id       
MMI_APP_PIC_TRC,			    //0xD1	\MS_Code\MS_MMI\source\mmi_app\app\pic			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_PIC_CROP_TRC,		    //0xD2	\MS_Code\MS_MMI\source\mmi_app\app\pic_crop	        \MS_Code\common\export\inc\trace_id    
MMI_APP_PIC_VIEWER_TRC,		    //0xD3	\MS_Code\MS_MMI\source\mmi_app\app\pic_viewer	    \MS_Code\common\export\inc\trace_id
MMI_APP_PIM_TRC,		        //0xD4	\MS_Code\MS_MMI\source\mmi_app\app\pim	            \MS_Code\common\export\inc\trace_id    
MMI_APP_PRINTSCREEN_TRC,		//0xD5	\MS_Code\MS_MMI\source\mmi_app\app\printscreen	    \MS_Code\common\export\inc\trace_id    
MMI_APP_PUSH_MAIL_TRC,		    //0xD6	\MS_Code\MS_MMI\source\mmi_app\app\push_mail	    \MS_Code\common\export\inc\trace_id    
MMI_APP_QBTHEME_TRC,		    //0xD7	\MS_Code\MS_MMI\source\mmi_app\app\qbtheme	        \MS_Code\common\export\inc\trace_id    
MMI_APP_QQ_TRC,		            //0xD8	\MS_Code\MS_MMI\source\mmi_app\app\qq	            \MS_Code\common\export\inc\trace_id    
MMI_APP_RECORD_TRC,			    //0xD9	\MS_Code\MS_MMI\source\mmi_app\app\record		    \MS_Code\common\export\inc\trace_id       
MMI_APP_SAMPLE_TRC,			    //0xDA	\MS_Code\MS_MMI\source\mmi_app\app\sample		    \MS_Code\common\export\inc\trace_id
MMI_APP_SCREENSAVER_TRC,		//0xDB	\MS_Code\MS_MMI\source\mmi_app\app\screensaver	    \MS_Code\common\export\inc\trace_id    
MMI_APP_SEARCH_TRC,		        //0xDC	\MS_Code\MS_MMI\source\mmi_app\app\search	        \MS_Code\common\export\inc\trace_id    
MMI_APP_SETTING_TRC,		    //0xDD	\MS_Code\MS_MMI\source\mmi_app\app\setting		    \MS_Code\common\export\inc\trace_id     
MMI_APP_SMS_TRC,			    //0xDE	\MS_Code\MS_MMI\source\mmi_app\app\sms			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_SNS_TRC,				//0xDF	 \MS_Code\MS_MMI\source\mmi_app\app\sns				\MS_Code\common\export\inc\trace_id 
MMI_APP_STK_TRC,			    //0xE0	\MS_Code\MS_MMI\source\mmi_app\app\stk			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_TAKESCREENPHOTO,		//0xE1  \MS_Code\MS_MMI\source\mmi_app\app\takescreenphoto  \MS_Code\common\export\inc\trace_id
MMI_APP_TASKMANAGER_TRC,		//0xE2	\MS_Code\MS_MMI\source\mmi_app\app\taskmanager	    \MS_Code\common\export\inc\trace_id    
MMI_APP_TENCENTMCARE_TRC,		//0xE3	\MS_Code\MS_MMI\source\mmi_app\app\TencentMcareV31	    \MS_Code\common\export\inc\trace_id    
MMI_APP_THEME_TRC,			    //0xE4	\MS_Code\MS_MMI\source\mmi_app\app\theme		    \MS_Code\common\export\inc\trace_id
MMI_APP_TP_TRC,				    //0xE5	\MS_Code\MS_MMI\source\mmi_app\app\tp			    \MS_Code\common\export\inc\trace_id                 
MMI_APP_TTS_TRC,		        //0xE6	\MS_Code\MS_MMI\source\mmi_app\app\tts	            \MS_Code\common\export\inc\trace_id    
MMI_APP_UDISK_TRC,			    //0xE7	\MS_Code\MS_MMI\source\mmi_app\app\udisk		    \MS_Code\common\export\inc\trace_id        
MMI_APP_USSD_TRC,		        //0xE8	\MS_Code\MS_MMI\source\mmi_app\app\ussd	            \MS_Code\common\export\inc\trace_id    
MMI_APP_UZONE_TRC,		        //0xE9	\MS_Code\MS_MMI\source\mmi_app\app\uzone	        \MS_Code\common\export\inc\trace_id    
MMI_APP_VCARD_TRC,			    //0xEA	\MS_Code\MS_MMI\source\mmi_app\app\vcard		    \MS_Code\common\export\inc\trace_id        
MMI_APP_VIDEO_PLAYER_TRC,		//0xEB	\MS_Code\MS_MMI\source\mmi_app\app\videoplayer		\MS_Code\common\export\inc\trace_id        
MMI_APP_VT_TRC,				    //0xEC	\MS_Code\MS_MMI\source\mmi_app\app\vt			    \MS_Code\common\export\inc\trace_id                   
MMI_APP_WALLPAPER_TRC,		    //0xED	\MS_Code\MS_MMI\source\mmi_app\app\wallpaper	    \MS_Code\common\export\inc\trace_id    
MMI_APP_WEATHER_TRC,			//0xEE	\MS_Code\MS_MMI\source\mmi_app\app\weather			\MS_Code\common\export\inc\trace_id                   
MMI_APP_WIDGET_TRC,				//0xEF	\MS_Code\MS_MMI\source\mmi_app\app\widget			\MS_Code\common\export\inc\trace_id                   
MMI_APP_WINSE_TRC,				//0xF0	\MS_Code\MS_MMI\source\mmi_app\app\winse			\MS_Code\common\export\inc\trace_id                   
MMI_APP_WLAN_TRC,				//0xF1	\MS_Code\MS_MMI\source\mmi_app\app\wlan			    \MS_Code\common\export\inc\trace_id                   
MMI_APP_WRE_BOOT_TRC,			//0xF2	\MS_Code\MS_MMI\source\mmi_app\app\wre_boot			\MS_Code\common\export\inc\trace_id                   
MMI_APP_WWW_TRC,				//0xF3	\MS_Code\MS_MMI\source\mmi_app\app\www			    \MS_Code\common\export\inc\trace_id
MMI_APP_MRAPP_TRC,              //0xF4  \MS_Code\MS_MMI\source\mmi_app\app\mrapp            \MS_Code\common\export\inc\trace_id
MMI_APP_OPERA4_TRC,		        //0xF5	\MS_Code\MS_MMI\source\mmi_app\app\operamini4	    \MS_Code\common\export\inc\trace_id    
MMI_APP_APPLE_TRC,					//0xF6  \MS_Code\MS_MMI\source\mmi_app\app\apple  			       \MS_Code\common\export\inc\trace_id                                                                                                                                    
MMI_APP_DEVICELOCK_TRC,					//0xF7  \MS_Code\MS_MMI\source\mmi_app\app\devicelock 			       \MS_Code\common\export\inc\trace_id                                                                                                                                    
MMI_APP_CONTROLPANEL_TRC,        //0xF8 \MS_Code\MS_MMI\source\mmi_app\app\cp    \MS_Code\common\export\inc\trace_id
MMI_APP_DATACOUNTER_TRC,

MMI_GUI_TRC = 0x100,			//0x100	\MS_Code\MS_MMI\source\mmi_gui					\MS_Code\common\export\inc\trace_id
MMI_KERNEL_TRC = 0x101,		    //0x101	\MS_Code\MS_MMI\source\mmi_kernel				\MS_Code\common\export\inc\trace_id
MMI_KERNEL_SYS_TRC = 0x102,	    //0x102	\MS_Code\MS_MMI\source\mmi_kernel_sys			\MS_Code\common\export\inc\trace_id
MMI_SERVICE_TRC = 0x103,	    //0x103	\MS_Code\MS_MMI\source\mmi_service			    \MS_Code\common\export\inc\trace_id
MMI_SPML_TRC = 0x104,	        //0x104	\MS_Code\MS_MMI\source\mmi_spml			        \MS_Code\common\export\inc\trace_id
MMI_UTILITY_TRC = 0x105,		//0x105	\MS_Code\MS_MMI\source\mmi_utility				\MS_Code\common\export\inc\trace_id
MMI_PIM_TRC = 0x106,	        //0x106	\MS_Code\MS_MMI\source\pim			            \MS_Code\common\export\inc\trace_id
MMI_PUSH_MAIL = 0x107,	        //0x107	\MS_Code\MS_MMI\source\push_mail			    \MS_Code\common\export\inc\trace_id
MMI_SPIM_TRC = 0x108,		    //0x108	\MS_Code\MS_MMI\source\spim					    \MS_Code\common\export\inc\trace_id
MMI_SUN_JAVA_TRC = 0x109,	    //0x109	\MS_Code\MS_MMI\source\sun_java					\MS_Code\common\export\inc\trace_id
MMI_FLASHUI_TRC = 0x10A,	    //0x10A	\MS_Code\MS_MMI\source\FlashUI					\MS_Code\common\export\inc\trace_id                                                         

TRACE_MODULE_MAX
}trace_module_enum;


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _SCI_TRACE_MOD_H

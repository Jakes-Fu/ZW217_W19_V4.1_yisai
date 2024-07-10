/******************************************************************************
 ** File Name:      oscfg.c                                                   *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           11/08/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines most of system's static tasks.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/08/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

enum
{
    PRI_EFS_OFFSET                          = 30,
    PRI_CHGMNG_SRV_TASK_OFFSET              = 20,
    PRI_DBAT_TASK_OFFSET                		= 21,    
    PRI_AUD_DSP_TASK_OFFSET                 = 15,
    PRI_CARDLOG_TASK_PRIO_OFFSET            = 31,
    PRI_DSP_CODEC_UPLOAD_TASK_OFFSET        = 5, 
    PRI_IMG_DEC_TASK_OFFSET                 = 30,
    PRI_PNGDEC_TASK_OFFSET                  = 31,
    PRI_DCAM_TASK_OFFSET                    = 28,
    PRI_LCD_TASK_OFFSET                     = 27,
    PRI_IMG_SCALE_TASK_OFFSET               = 30,
    PRI_SYSTEM_KERNEL_OFFSET                = 0, 
    PRI_VOICE_UPLINK_TASK_OFFSET            = 5, 
    PRI_VOICE_DOWNLINK_TASK_OFFSET          = 5, 
    PRI_KPDSVR_OFFSET                       = 15,
    PRI_SMCODEC_OFFSET                      = 19,
    PRI_MICRO_USB_OFFSET                    = 20,
    PRI_TIMESVR_OFFSET                      = 20,
    PRI_UCOM_MAIN_OFFSET                    = 20,
    PRI_USB_VBUS_OFFSET                     = 20,
    PRI_USB_SERVICE_OFFSET                  = 20,
    PRI_UBOT_MAIN_OFFSET                    = 30,
    PRI_BT_TASK_OFFSET                      = 20,
    PRI_AUDIO_OFFSET                        = 25,
    PRI_CODEC_DRV_TASK_OFFSET               = PRI_AUDIO_OFFSET+1,
    PRI_AUDIO_HAL_TASK_OFFSET               = 10,
    PRI_TP_OFFSET                           = 25,
    PRI_UCOM_DES_TASK_OFFSET                = 25,
    PRI_BT_SBC_OFFSET                       = 26,
    PRI_SCM_OFFSET                          = 26,
    PRI_IMG_DEC_OFFSET                      = 26,
    PRI_AUDIO_CODEC_HIGH_OFFSET             = 26,
    PRI_SHUTTLE_TASK_OFFSET                 = 26,
    PRI_AGPS_INTERFACE_TASK_OFFSET		 	= 27,
    PRI_GPS_INTERFACE_TASK_OFFSET		 	= 27,
    PRI_GPS_TASK_OFFSET 				 	= 27,
    PRI_GPS_LOG_OFFSET					 	= 27,
    PRI_APP_OFFSET                          = 28,
    PRI_DV_DISPLAY_OFFSET                   = 28,
    PRI_DV_AUDIO_OFFSET                     = 28,
    PRI_STL_COMPACTION_OFFSET               = 28,
    PRI_FMARK_TASK_OFFSET                   = 28,
    PRI_APP_TASK_OFFSET                     = 28,
    #ifdef AUTODEMO_SUPPORT
    PRI_AUTODEMO_TASK_OFFSET                = 30,
    #endif
    PRI_MSENSOR_TASK_OFFSET                 = 28,
    PRI_ISP_SERVICE_TASK_OFFSET             = 29,
    PRI_IMG_PROC_CTRL_TASK_OFFSET           = 28,
    PRI_IMG_PROC_PROCESS_TASK_OFFSET        = 32,
    PRI_DV_DEMUX_OFFSET                     = 30,
    PRI_DCAMERA_TASK_OFFSET                 = 30,
    PRI_VT_MIDDLEWARE_TASK_OFFSET           = 30,
    PRI_VT_LOOP_TASK_OFFSET                 = 30,
    PRI_UCOM_ORG_TASK_OFFSET                = 30,
    PRI_JPEG_TASK_OFFSET                    = 30,
    PRI_JPEG_TASK_HW_DEFAULT_OFFSET         = 30,
    PRIFS_OFFSET                            = 30,
    PRI_DV_VIDEO_OFFSET                     = 29,
    PRI_DV_MUX_OFFSET                       = 29,
    PRI_FFS_OFFSET                          = 31,
    PRI_AUDIO_CODEC_LOW_OFFSET              = 31,
    PRI_BMP_TASK_OFFSET                     = 31,
    PRI_IMG_DEC_CONFIG_OFFSET               = 31,
    PRI_GIF_DECODE_TASK_OFFSET              = 31,
    PRI_UPCC_MAIN_TASK_OFFSET               = 31,
    PRI_UPCC_DRV_TASK_OFFSET                = 31,
    PRI_RECORD_OFFSET                       = 32,
    PRI_BT_FTP_OFFSET                       = 32,
    PRI_BT_TIMEOUT_OFFSET                   = 32,
    PRI_APP_INIT_TASK_OFFSET                = 32,
    PRI_CPU_MONITOR_OFFSET                  = 32,
    PRI_VOICE_RECORD_DEBUG_DATA_TASK_OFFSET = 32,
    PRI_DSP_LOG_TASK_OFFSET                 = 32,
    PRI_BT_LOG_TASK_OFFSET                 = 32,
    PRI_MV_SCHED_TASK_OFFSET                = 25,
    PRI_SFS_DISK_C_TASK_OFFSET              = 31,
    PRI_SFS_DISK_D_TASK_OFFSET              = 31,
    PRI_SFS_DISK_OFFSET_TASK_OFFSET         = 31,
    PRI_SFS_DISK_F_TASK_OFFSET              = 31,
    PRI_NET_PRIORITY_OFFSET				    = 19,
#ifndef WIFI_SUPPORT_UNISOC_RX
    PRI_WIFISUPP_TASK_OFFSET			    = 26,
#else
    PRI_WIFISUPP_TASK_OFFSET			    = 32,
#endif
    PRI_IPERF_TASK_HIGH_OFFSET			    = 19,
    PRI_IPERF_TASK_NORMAL_OFFSET		    = 26,
    PRI_IPERF_TASK_LOW_OFFSET			    = 32,
    PRI_RTP_TASK_OFFSET					    = 26,
    PRI_ODM_TASK_OFFSET					    = 26,
    PRI_ODM_UT_TASK_OFFSET				    = 31,
    PRI_STRM_TASK_OFFSET				    = 27,
    PRI_DUA_TASK_OFFSET					    = 27,
    PRI_NETMGR_TASK_OFFSET				    = 31,
    PRI_NETMGR_TESTAPP_TASK_OFFSET		    = 27,
    PRI_DRM_DH_TASK_OFFSET				    = 32,
    PRI_DRM_IO_TASK_OFFSET				    = 31,
    PRI_WIFI_TEST_TASK_OFFSET			    = 28,
    PRI_WIFITEST_TASK_OFFSET			    = 32,
    PRI_CSR_FW_TASK_OFFSET                  = 31,
    PRI_CSR_WIFI_BH_TASK_OFFSET             = 16,   
    PRI_RTL_HIGH_TASK_OFFSET                = 17,
    PRI_RTL_NORMAL_TASK_OFFSET              = 26,
    PRI_UMEM_HANDLE_OFFSET                  = 30,
    PRI_UMEM_GARBAGECOLLECT_OFFSET          = 32,
    PRI_VOICE_PROCESS_TASK_OFFSET           = 29,
    MAX_SYSTEM_PRIORITY_OFFSET              = MAX_SYSTEM_PRI_NUM
};
                                                                                                             
//System Task Define                                                                                         
SCI_DEFINE_TASK_PRI(PRI_EFS       			          ,SCI_SYSTEM_PRI( PRI_EFS_OFFSET                          ));
SCI_DEFINE_TASK_PRI(PRI_CHGMNG_SRV_TASK               ,SCI_SYSTEM_PRI( PRI_CHGMNG_SRV_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_DBAT_TASK                     ,SCI_SYSTEM_PRI( PRI_DBAT_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_AUD_DSP_TASK       	          ,SCI_SYSTEM_PRI( PRI_AUD_DSP_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_CARDLOG_TASK_PRIO             ,SCI_SYSTEM_PRI( PRI_CARDLOG_TASK_PRIO_OFFSET            ));
SCI_DEFINE_TASK_PRI(PRI_DSP_CODEC_UPLOAD_TASK         ,SCI_SYSTEM_PRI( PRI_DSP_CODEC_UPLOAD_TASK_OFFSET        ));
SCI_DEFINE_TASK_PRI(PRI_IMG_DEC_TASK       	          ,SCI_SYSTEM_PRI( PRI_IMG_DEC_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_PNGDEC_TASK       	          ,SCI_SYSTEM_PRI( PRI_PNGDEC_TASK_OFFSET                  ));
SCI_DEFINE_TASK_PRI(PRI_CODEC_DRV_TASK       		  ,SCI_SYSTEM_PRI( PRI_CODEC_DRV_TASK_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_AUDIO_HAL_TASK       		  ,SCI_SYSTEM_PRI( PRI_AUDIO_HAL_TASK_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_DCAM_TASK       	          ,SCI_SYSTEM_PRI( PRI_DCAM_TASK_OFFSET                    ));
SCI_DEFINE_TASK_PRI(PRI_LCD_TASK       		          ,SCI_SYSTEM_PRI( PRI_LCD_TASK_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_IMG_SCALE_TASK                ,SCI_SYSTEM_PRI( PRI_IMG_SCALE_TASK_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_SYSTEM_KERNEL                 ,SCI_SYSTEM_PRI( PRI_SYSTEM_KERNEL_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_VOICE_UPLINK_TASK             ,SCI_BASE_PRI( PRI_VOICE_UPLINK_TASK_OFFSET            ));
SCI_DEFINE_TASK_PRI(PRI_VOICE_DOWNLINK_TASK           ,SCI_BASE_PRI( PRI_VOICE_DOWNLINK_TASK_OFFSET          ));
SCI_DEFINE_TASK_PRI(PRI_KPDSVR                        ,SCI_BASE_PRI( PRI_KPDSVR_OFFSET                       ));
SCI_DEFINE_TASK_PRI(PRI_SMCODEC                       ,SCI_SYSTEM_PRI( PRI_SMCODEC_OFFSET                      ));
SCI_DEFINE_TASK_PRI(PRI_MICRO_USB                     ,SCI_SYSTEM_PRI( PRI_MICRO_USB_OFFSET                    ));
SCI_DEFINE_TASK_PRI(PRI_TIMESVR                       ,SCI_SYSTEM_PRI( PRI_TIMESVR_OFFSET                      ));
SCI_DEFINE_TASK_PRI(PRI_UCOM_MAIN                     ,SCI_SYSTEM_PRI( PRI_UCOM_MAIN_OFFSET                    ));
SCI_DEFINE_TASK_PRI(PRI_USB_VBUS                      ,SCI_SYSTEM_PRI( PRI_USB_VBUS_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_USB_SERVICE                   ,SCI_SYSTEM_PRI( PRI_USB_SERVICE_OFFSET                  ));
SCI_DEFINE_TASK_PRI(PRI_UBOT_MAIN                     ,SCI_SYSTEM_PRI( PRI_UBOT_MAIN_OFFSET                    ));
SCI_DEFINE_TASK_PRI(PRI_BT_TASK                       ,SCI_SYSTEM_PRI( PRI_BT_TASK_OFFSET                      ));
SCI_DEFINE_TASK_PRI(PRI_AUDIO                         ,SCI_SYSTEM_PRI( PRI_AUDIO_OFFSET                        ));
SCI_DEFINE_TASK_PRI(PRI_TP                            ,SCI_SYSTEM_PRI( PRI_TP_OFFSET                           ));
SCI_DEFINE_TASK_PRI(PRI_UCOM_DES_TASK                 ,SCI_SYSTEM_PRI( PRI_UCOM_DES_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_BT_SBC                        ,SCI_SYSTEM_PRI( PRI_BT_SBC_OFFSET                       ));
SCI_DEFINE_TASK_PRI(PRI_SCM                           ,SCI_SYSTEM_PRI( PRI_SCM_OFFSET                          ));
SCI_DEFINE_TASK_PRI(PRI_IMG_DEC                       ,SCI_SYSTEM_PRI( PRI_IMG_DEC_OFFSET                      ));
SCI_DEFINE_TASK_PRI(PRI_AUDIO_CODEC_HIGH              ,SCI_SYSTEM_PRI( PRI_AUDIO_CODEC_HIGH_OFFSET             ));
SCI_DEFINE_TASK_PRI(PRI_AGPS_INTERFACE_TASK			  ,SCI_SYSTEM_PRI(PRI_AGPS_INTERFACE_TASK_OFFSET			    ));
SCI_DEFINE_TASK_PRI(PRI_GPS_INTERFACE_TASK			  ,SCI_SYSTEM_PRI(PRI_GPS_INTERFACE_TASK_OFFSET			    ));
SCI_DEFINE_TASK_PRI(PRI_GPS_TASK					  ,SCI_SYSTEM_PRI(PRI_GPS_TASK_OFFSET			    ));
SCI_DEFINE_TASK_PRI(PRI_GPS_LOG_TASK				  ,SCI_SYSTEM_PRI(PRI_GPS_LOG_OFFSET			    ));
SCI_DEFINE_TASK_PRI(PRI_APP                           ,SCI_SYSTEM_PRI( PRI_APP_OFFSET                          ));
SCI_DEFINE_TASK_PRI(PRI_DV_DISPLAY                    ,SCI_SYSTEM_PRI( PRI_DV_DISPLAY_OFFSET                   ));
SCI_DEFINE_TASK_PRI(PRI_DV_AUDIO                      ,SCI_SYSTEM_PRI( PRI_DV_AUDIO_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_STL_COMPACTION                ,SCI_SYSTEM_PRI( PRI_STL_COMPACTION_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_FMARK_TASK                    ,SCI_SYSTEM_PRI( PRI_FMARK_TASK_OFFSET                   ));
SCI_DEFINE_TASK_PRI(PRI_APP_TASK                      ,SCI_SYSTEM_PRI( PRI_APP_TASK_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_ISP_SERVICE_TASK              ,SCI_SYSTEM_PRI( PRI_ISP_SERVICE_TASK_OFFSET             ));
SCI_DEFINE_TASK_PRI(PRI_IMG_PROC_CTRL_TASK            ,SCI_SYSTEM_PRI( PRI_IMG_PROC_CTRL_TASK_OFFSET             ));
SCI_DEFINE_TASK_PRI(PRI_IMG_PROC_PROCESS_TASK         ,SCI_SYSTEM_PRI( PRI_IMG_PROC_PROCESS_TASK_OFFSET             ));
SCI_DEFINE_TASK_PRI(PRI_DV_DEMUX                      ,SCI_SYSTEM_PRI( PRI_DV_DEMUX_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_DCAMERA_TASK                  ,SCI_SYSTEM_PRI( PRI_DCAMERA_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_VT_MIDDLEWARE_TASK            ,SCI_SYSTEM_PRI( PRI_VT_MIDDLEWARE_TASK_OFFSET           ));
SCI_DEFINE_TASK_PRI(PRI_VT_LOOP_TASK                  ,SCI_SYSTEM_PRI( PRI_VT_LOOP_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_UCOM_ORG_TASK                 ,SCI_SYSTEM_PRI( PRI_UCOM_ORG_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_JPEG_TASK                     ,SCI_SYSTEM_PRI( PRI_JPEG_TASK_OFFSET                    ));
SCI_DEFINE_TASK_PRI(PRI_JPEG_TASK_HW_DEFAULT          ,SCI_SYSTEM_PRI( PRI_JPEG_TASK_HW_DEFAULT_OFFSET         ));
SCI_DEFINE_TASK_PRI(PRIFS 					          ,SCI_SYSTEM_PRI( PRIFS_OFFSET                            ));
SCI_DEFINE_TASK_PRI(PRI_DV_VIDEO                      ,SCI_SYSTEM_PRI( PRI_DV_VIDEO_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_DV_MUX                        ,SCI_SYSTEM_PRI( PRI_DV_MUX_OFFSET                       ));
SCI_DEFINE_TASK_PRI(PRI_FFS                           ,SCI_SYSTEM_PRI( PRI_FFS_OFFSET                          ));
SCI_DEFINE_TASK_PRI(PRI_AUDIO_CODEC_LOW               ,SCI_SYSTEM_PRI( PRI_AUDIO_CODEC_LOW_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_BMP_TASK                      ,SCI_SYSTEM_PRI( PRI_BMP_TASK_OFFSET                     ));
SCI_DEFINE_TASK_PRI(PRI_IMG_DEC_CONFIG                ,SCI_SYSTEM_PRI( PRI_IMG_DEC_CONFIG_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_GIF_DECODE_TASK               ,SCI_SYSTEM_PRI( PRI_GIF_DECODE_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_UPCC_MAIN_TASK                ,SCI_SYSTEM_PRI( PRI_UPCC_MAIN_TASK_OFFSET               ));
SCI_DEFINE_TASK_PRI(PRI_UPCC_DRV_TASK                 ,SCI_SYSTEM_PRI( PRI_UPCC_DRV_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_RECORD                        ,SCI_SYSTEM_PRI( PRI_RECORD_OFFSET                       ));
SCI_DEFINE_TASK_PRI(PRI_BT_FTP                        ,SCI_SYSTEM_PRI( PRI_BT_FTP_OFFSET                       ));
SCI_DEFINE_TASK_PRI(PRI_BT_TIMEOUT                    ,SCI_SYSTEM_PRI( PRI_BT_TIMEOUT_OFFSET                   ));
SCI_DEFINE_TASK_PRI(PRI_APP_INIT_TASK                 ,SCI_SYSTEM_PRI( PRI_APP_INIT_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_CPU_MONITOR                   ,SCI_SYSTEM_PRI( PRI_CPU_MONITOR_OFFSET                  ));
SCI_DEFINE_TASK_PRI(PRI_VOICE_RECORD_DEBUG_DATA_TASK  ,SCI_SYSTEM_PRI( PRI_VOICE_RECORD_DEBUG_DATA_TASK_OFFSET ));
SCI_DEFINE_TASK_PRI(PRI_DSP_LOG_TASK 		          ,SCI_SYSTEM_PRI( PRI_DSP_LOG_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_BT_LOG_TASK 		          ,SCI_SYSTEM_PRI( PRI_BT_LOG_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_MV_SCHED_TASK 		          ,SCI_SYSTEM_PRI( PRI_MV_SCHED_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_SFS_DISK_C_TASK		          ,SCI_SYSTEM_PRI( PRI_SFS_DISK_C_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_SFS_DISK_D_TASK		          ,SCI_SYSTEM_PRI( PRI_SFS_DISK_D_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_SFS_DISK_OFFSET_TASK		  ,SCI_SYSTEM_PRI( PRI_SFS_DISK_OFFSET_TASK_OFFSET         ));
SCI_DEFINE_TASK_PRI(PRI_SFS_DISK_F_TASK		          ,SCI_SYSTEM_PRI( PRI_SFS_DISK_F_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_MSENSOR_TASK		          ,SCI_SYSTEM_PRI( PRI_MSENSOR_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_SHUTTLE_TASK		          ,SCI_SYSTEM_PRI( PRI_SHUTTLE_TASK_OFFSET                 ));
SCI_DEFINE_TASK_PRI(PRI_NET_PRIORITY				  ,SCI_SYSTEM_PRI( PRI_NET_PRIORITY_OFFSET		           ));
SCI_DEFINE_TASK_PRI(PRI_WIFISUPP_TASK				  ,SCI_SYSTEM_PRI( PRI_WIFISUPP_TASK_OFFSET		           ));
SCI_DEFINE_TASK_PRI(PRI_IPERF_TASK_HIGH				  ,SCI_SYSTEM_PRI( PRI_IPERF_TASK_HIGH_OFFSET		       ));
SCI_DEFINE_TASK_PRI(PRI_IPERF_TASK_NORMAL			  ,SCI_SYSTEM_PRI( PRI_IPERF_TASK_NORMAL_OFFSET	           ));
SCI_DEFINE_TASK_PRI(PRI_IPERF_TASK_LOW				  ,SCI_SYSTEM_PRI( PRI_IPERF_TASK_LOW_OFFSET	           ));
SCI_DEFINE_TASK_PRI(PRI_RTP_TASK					  ,SCI_SYSTEM_PRI( PRI_RTP_TASK_OFFSET			           ));
SCI_DEFINE_TASK_PRI(PRI_ODM_TASK					  ,SCI_SYSTEM_PRI( PRI_ODM_TASK_OFFSET			           ));
SCI_DEFINE_TASK_PRI(PRI_ODM_UT_TASK					  ,SCI_SYSTEM_PRI( PRI_ODM_UT_TASK_OFFSET			       ));
SCI_DEFINE_TASK_PRI(PRI_STRM_TASK					  ,SCI_SYSTEM_PRI( PRI_STRM_TASK_OFFSET			           ));
SCI_DEFINE_TASK_PRI(PRI_DUA_TASK					  ,SCI_SYSTEM_PRI( PRI_DUA_TASK_OFFSET			           ));
SCI_DEFINE_TASK_PRI(PRI_NETMGR_TASK					  ,SCI_SYSTEM_PRI( PRI_NETMGR_TASK_OFFSET			       ));
SCI_DEFINE_TASK_PRI(PRI_NETMGR_TESTAPP_TASK			  ,SCI_SYSTEM_PRI( PRI_NETMGR_TESTAPP_TASK_OFFSET	       ));
SCI_DEFINE_TASK_PRI(PRI_DRM_DH_TASK					  ,SCI_SYSTEM_PRI( PRI_DRM_DH_TASK_OFFSET		           ));
SCI_DEFINE_TASK_PRI(PRI_DRM_IO_TASK					  ,SCI_SYSTEM_PRI( PRI_DRM_IO_TASK_OFFSET		           ));
SCI_DEFINE_TASK_PRI(PRI_WIFI_TEST_TASK				  ,SCI_SYSTEM_PRI( PRI_WIFI_TEST_TASK_OFFSET		       ));
SCI_DEFINE_TASK_PRI(PRI_WIFITEST_TASK				  ,SCI_SYSTEM_PRI( PRI_WIFITEST_TASK_OFFSET		           ));
SCI_DEFINE_TASK_PRI(PRI_CSR_FW_TASK                   ,SCI_SYSTEM_PRI( PRI_CSR_FW_TASK_OFFSET                  ));
SCI_DEFINE_TASK_PRI(PRI_CSR_WIFI_BH_TASK              ,SCI_SYSTEM_PRI( PRI_CSR_WIFI_BH_TASK_OFFSET             ));
SCI_DEFINE_TASK_PRI(PRI_RTL_HIGH_TASK                 ,SCI_SYSTEM_PRI( PRI_RTL_HIGH_TASK_OFFSET                ));
SCI_DEFINE_TASK_PRI(PRI_RTL_NORMAL_TASK               ,SCI_SYSTEM_PRI( PRI_RTL_NORMAL_TASK_OFFSET              ));
SCI_DEFINE_TASK_PRI(PRI_UMEM_HANDLE		              ,SCI_SYSTEM_PRI( PRI_UMEM_HANDLE_OFFSET                  ));
SCI_DEFINE_TASK_PRI(PRI_UMEM_GARBAGECOLLECT           ,SCI_SYSTEM_PRI( PRI_UMEM_GARBAGECOLLECT_OFFSET          ));
SCI_DEFINE_TASK_PRI(PRI_VOICE_PROCESS_TASK            ,SCI_SYSTEM_PRI( PRI_VOICE_PROCESS_TASK_OFFSET           ));
#ifdef AUTODEMO_SUPPORT
SCI_DEFINE_TASK_PRI(PRI_AUTODEMO_TASK                 ,SCI_SYSTEM_PRI( PRI_AUTODEMO_TASK_OFFSET                ));
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

//End of oscfg.c

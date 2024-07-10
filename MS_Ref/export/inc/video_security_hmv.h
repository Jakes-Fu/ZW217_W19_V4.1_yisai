#ifndef VIDEOPLAYER_SECURITY_HMV_H
#define VIDEOPLAYER_SECURITY_HMV_H

#include "video_security.h"


PUBLIC VPLAYER_SECURITY_HANDLE VPlayerSecurity_Open_Hmv(uint32 argc,void **argv);

PUBLIC int VPlayerSecurity_Close_Hmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

PUBLIC uint32 VPlayerSecurity_GetFID_Hmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

PUBLIC int VPlayerSecurity_GetVDecSecPlugInfo_Hmv(
                                VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                VDEC_SECURITY_PLUG_INFO_T **p_info
                                );

PUBLIC int VPlayerSecurity_GetThumbnailInfo_Hmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            VPLAYER_SEC_THUMBNAIL_INFO_T *p_thumbnail_info
                                            );

PUBLIC int VPlayerSecurity_GetThumbnailData_Hmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            uint8 *p_buffer,uint32 len
                                            );




#endif


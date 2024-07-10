
#ifndef VIDEOPLAYER_SECURITY_KMV_H
#define VIDEOPLAYER_SECURITY_KMV_H

#include "video_security.h"


PUBLIC VPLAYER_SECURITY_HANDLE VPlayerSecurity_Open_Kmv(uint32 argc,void **argv);

PUBLIC int VPlayerSecurity_Close_Kmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

PUBLIC uint32 VPlayerSecurity_GetFID_Kmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle);

PUBLIC int VPlayerSecurity_GetVDecSecPlugInfo_Kmv(
                                VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                VDEC_SECURITY_PLUG_INFO_T **p_info
                                );

PUBLIC int VPlayerSecurity_GetThumbnailInfo_Kmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            VPLAYER_SEC_THUMBNAIL_INFO_T *p_thumbnail_info
                                            );

PUBLIC int VPlayerSecurity_GetThumbnailData_Kmv(VPLAYER_SECURITY_HANDLE vplayer_sec_handle,
                                            uint8 *p_buffer,uint32 len
                                            );




#endif

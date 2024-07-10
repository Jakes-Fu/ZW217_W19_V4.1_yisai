/****************************************************************************
** File Name:      mmimtv_save.h                                            *
** Author:         xin.li                                                   *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mtv application.       *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        xin.li           Create
****************************************************************************/
#ifdef CMMB_SUPPORT
#ifndef _MMIMTV_SAVE_H_
#define _MMIMTV_SAVE_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mtv_api.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
//#include "mtvesg_common.h"

//#define MBBMS_SUPPORT
#ifdef MBBMS_SUPPORT
#include "mbbms_sg_common.h"
#include "mbbms_sg_api.h"
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMTV_TRANSPARENT_COLOR                            MMI_BLACK_COLOR
#define MMIMTV_BACKGROUND_COLOR                             MMI_BACKGROUND_COLOR
#define MMIMTV_EB_CONTENT_MAX                               1024        //紧急广播信息的最大长度 
#define MMIMTV_EB_TYPE_MAX                                  20
#define MMIMTV_MAX_NAME_LEN                                 64          //电台名的最大长度
#define MMIMTV_COMMON_MAX_LEN                               255
#define MMIMTV_FILE_NAME_ID_MAX	                            10000000
#define MMIMTV_PROG_HINT_STR_LEN                            255         //节目提示信息的最大长度 
#define MMIMTV_BOOK_RECORD_FILE_NAME_MAX_LEN                MMIFMM_FULL_FILENAME_LEN   //预约录制文件名最大长度 
#define MMIMTV_BOOK_RECORD_FILE_LONG_MAX                    600                        //预约录制最大录制时长
#define MMIMTV_FULL_PATH_MAX_LEN                            MMIFMM_FULL_FILENAME_LEN   //文件名最大长度
#define MMIMTV_COMMON_MAX_LEN                               255

#define MMIMTV_TRANSPARENT_COLOR_0                          0
#define MMIMTV_TRANSPARENT_COLOR_75                         (255*75/100)
#define MMIMTV_TRANSPARENT_COLOR_50                         (255*50/100)
#define MMIMTV_TRANSPARENT_COLOR_25                         (255*25/100)
#define MMIMTV_TRANSPARENT_COLOR_15                         (255*15/100)

#define	MMIMTV_LCD_CONTRSAT_MAX     		                100
#define MMIMTV_SIGNAL_VALUE_MAX                             100     //TV信号最大值
#define MMIMTV_SIGNAL_LEVEL_MAX                             5       //TV信号强度最大值
        
 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef struct MMIMTV_EB_NODE_T_tag
{
    uint32                          eb_grade;
    BOOLEAN                         eb_is_read;
    uint32                          eb_save_time;
    uint32                          eb_service_id;
    wchar                           eb_content[MMIMTV_EB_CONTENT_MAX +1];
    wchar                           eb_type[MMIMTV_EB_TYPE_MAX +1];
    uint16                          eb_content_len;
    struct MMIMTV_EB_NODE_T_tag     *eb_next;
}MMIMTV_EB_NODE_T;

typedef struct MMIMTV_SAVE_SERVICE_INFO_T_tag
{	
    uint32                                  net_id;
    uint32                                  service_id;
    MTV_STIMI_SERVICE_CLASS_E               service_type;
    wchar                                   service_name[MMIMTV_MAX_NAME_LEN + 1];	
    struct MMIMTV_SAVE_SERVICE_INFO_T_tag   *service_next;
}MMIMTV_SAVE_SERVICE_INFO_T;

typedef struct MMIMTV_SAVE_NETEID_INFO_T_tag
{
    uint32                                  net_id;
    uint32                                  center_freqquency;
    uint32                                  service_number;
    uint32                                  shortservice_number;
    wchar                                   net_name[MMIMTV_MAX_NAME_LEN + 1];	
    struct MMIMTV_SAVE_NETEID_INFO_T_tag    *net_next;
    MMIMTV_SAVE_SERVICE_INFO_T              *service_head;
}MMIMTV_SAVE_NETEID_INFO_T;

typedef struct MMIMTV_SAVEFILE_INFO_T_tag
{
    uint32                      version;
    uint32                      net_list_number;
}MMIMTV_SAVEFILE_INFO_T;

typedef enum
{
    MMIMTV_AUDIO_CHN = 0,
    MMIMTV_AUDIO_ENG,
    MMIMTV_AUDIO_LANGUGAE_MAX,
}MMIMTV_AUDIO_LANGUGAE_E;

typedef enum
{
    MMIMTV_AUDIO_MODE_SINGLE_CHANNEL = 0,
    MMIMTV_AUDIO_MODE_STEREO,
    MMIMTV_AUDIO_MODE_MAX,
}MMIMTV_AUDIO_MODE_E;

typedef enum
{
    MMIMTV_ZOOM_16_9 = 0,
    MMIMTV_ZOOM_4_3,
    MMIMTV_ZOOM_FRAME,
    MMIMTV_ZOOM_MODE_MAX,
}MMIMTV_ZOOM_MODE_E;

typedef enum
{
    MMIMTV_VOLUME_LEVEL_0 = 0,
    MMIMTV_VOLUME_LEVEL_1,
    MMIMTV_VOLUME_LEVEL_2,
    MMIMTV_VOLUME_LEVEL_3,
    MMIMTV_VOLUME_LEVEL_4,
    MMIMTV_VOLUME_LEVEL_5,
    MMIMTV_VOLUME_LEVEL_6,
    MMIMTV_VOLUME_LEVEL_7,
    MMIMTV_VOLUME_LEVEL_MAX
}MMIMTV_VOLUME_E;

typedef enum
{
    MMIMTV_BRIGHTNESS_LEVEL_0 = 0,
    MMIMTV_BRIGHTNESS_LEVEL_1,
    MMIMTV_BRIGHTNESS_LEVEL_2,
    MMIMTV_BRIGHTNESS_LEVEL_3,
    MMIMTV_BRIGHTNESS_LEVEL_4,
    MMIMTV_BRIGHTNESS_LEVEL_5,
    MMIMTV_BRIGHTNESS_LEVEL_6,
    MMIMTV_BRIGHTNESS_LEVEL_7,
    MMIMTV_BRIGHTNESS_LEVEL_MAX
}MMIMTV_BRIGHTNESS_E;

typedef enum
{
    MMIMTV_CONTRAST_0 = 0,
    MMIMTV_CONTRAST_1,
    MMIMTV_CONTRAST_2,
    MMIMTV_CONTRAST_3,
    MMIMTV_CONTRAST_4,
    MMIMTV_CONTRAST_5,
    MMIMTV_CONTRAST_6,
    MMIMTV_CONTRAST_7,
    MMIMTV_CONTRAST_MAX
}MMIMTV_CONTRAST_E;

typedef enum
{
    MMIMTV_COLOR_SATURATION_0 = 0,
    MMIMTV_COLOR_SATURATION_1,
    MMIMTV_COLOR_SATURATION_2,
    MMIMTV_COLOR_SATURATION_3,
    MMIMTV_COLOR_SATURATION_4,
    MMIMTV_COLOR_SATURATION_5,
    MMIMTV_COLOR_SATURATION_6,
    MMIMTV_COLOR_SATURATION_7,
    MMIMTV_COLOR_SATURATION_MAX
}MMIMTV_COLOR_SATURATION_E;

typedef enum
{
    MMIMTV_PICTURE_SHARPNESS_0 = 0,
    MMIMTV_PICTURE_SHARPNESS_1,
    MMIMTV_PICTURE_SHARPNESS_2,
    MMIMTV_PICTURE_SHARPNESS_3,
    MMIMTV_PICTURE_SHARPNESS_4,
    MMIMTV_PICTURE_SHARPNESS_5,
    MMIMTV_PICTURE_SHARPNESS_6,
    MMIMTV_PICTURE_SHARPNESS_7,
    MMIMTV_PICTURE_SHARPNESS_MAX
}MMIMTV_PICTURE_SHARPNESS_E;

/*typedef enum
{
    MMIMTV_RECORD_STORE_UDISK = 0,
    MMIMTV_RECORD_STORE_SD,
    MMIMTV_RECORD_STORE_DEV_MAX,
}MMIMTV_RECORD_STORE_DEV_E;*/

/*typedef enum
{
    MMIMTV_EB_STORE_UDISK = 0,
    MMIMTV_EB_STORE_SD,
    MMIMTV_EB_STORE_DEV_MAX,
}MMIMTV_EB_STORE_DEV_E;*/

/*typedef enum
{
    MMIMTV_SNAPSHOT_STORE_UDISK = 0,
    MMIMTV_SNAPSHOT_STORE_SD,
    MMIMTV_SNAPSHOT_STORE_DEV_MAX,
}MMIMTV_SNAPSHOT_STORE_DEV_E;*/

/*typedef enum
{
    MMIMTV_ESG_STORE_UDISK = 0,
    MMIMTV_ESG_STORE_SD,
    MMIMTV_ESG_STORE_DEV_MAX,
}MMIMTV_ESG_STORE_DEV_E;*/

/*********************************************************************
为了实现6800H的存储设备的统一和各个模块兼容多T卡，
以上由charlie.wang删除，请该模块负责人FIX编译的地方，如有问题尽可联系
                                             -charlie.wang 2011.02.28
*********************************************************************/

typedef enum
{
    MMIMTV_OSD_TRANSPARENCY_15 = 0,
    MMIMTV_OSD_TRANSPARENCY_25,
    MMIMTV_OSD_TRANSPARENCY_50,
    MMIMTV_OSD_TRANSPARENCY_75,
    MMIMTV_OSD_TRANSPARENCY_MAX,
}MMIMTV_OSD_TRANSPARENCY_E;

typedef enum
{
    MMIMTV_DISPLAY_TIME_3S = 0,
    MMIMTV_DISPLAY_TIME_5S,
    MMIMTV_DISPLAY_TIME_10S,
    MMIMTV_DISPLAY_TIME_15S,
    MMIMTV_DISPLAY_TIME_MAX,
}MMIMTV_OSD_DISPLAY_TIME_E;

typedef enum
{
    MMIMTV_WATCH_TIME_TIP_10M = 0,
    MMIMTV_WATCH_TIME_TIP_30M,
    MMIMTV_WATCH_TIME_TIP_60M,
    MMIMTV_WATCH_TIME_TIP_MAX,
}MMIMTV_WATCH_TIME_TIP_E;

typedef enum
{
    MMIMTV_BROADCAST_BG_PIC_1 = 0,
    MMIMTV_BROADCAST_BG_PIC_2,
    MMIMTV_BROADCAST_BG_PIC_3,
    MMIMTV_BROADCAST_BG_PIC_MAX,
}MMIMTV_BROADCAST_BG_PIC_E;

typedef struct MMIMTV_SETTING_INFO_T_tag
{
    BOOLEAN                                 mute;
    BOOLEAN                                 display_program_name;
    BOOLEAN                                 subtitle_streams;
    MMIMTV_VOLUME_E                         volume_value;
    MMIMTV_VOLUME_E                         panel_volume_value;
    MMIMTV_AUDIO_LANGUGAE_E                 audio_language;
    MMIMTV_AUDIO_MODE_E                     audio_mode;
    MMIMTV_ZOOM_MODE_E                      zoom_mode;
    MMIMTV_BRIGHTNESS_E                     mtv_brightness;
    MMIMTV_CONTRAST_E                       mtv_contrast;
    MMIMTV_COLOR_SATURATION_E               color_saturation;
    MMIMTV_PICTURE_SHARPNESS_E              picture_sharpness;
    MMIFILE_DEVICE_E                        record_store;
    MMIFILE_DEVICE_E                        eb_store;
    MMIFILE_DEVICE_E                        snapshot_store;
	MMIFILE_DEVICE_E                        esg_path_store;
    MMIMTV_OSD_DISPLAY_TIME_E               osd_display_time;
    MMIMTV_WATCH_TIME_TIP_E                 watch_time_tip;
    MMIMTV_BROADCAST_BG_PIC_E               broadcast_bg;
    MMIMTV_OSD_TRANSPARENCY_E               transparency;
    uint32                                  record_file_id;
    uint32                                  snapshot_file_id;
    uint32                                  book_record_start_time;
    uint32                                  book_record_long;
    uint32                                  book_record_net_id;
    uint32                                  book_record_service_id;
    wchar                                   book_record_file_name[MMIMTV_BOOK_RECORD_FILE_NAME_MAX_LEN + 1];
    uint8                                   signal_level;
}MMIMTV_SETTING_INFO_T;

typedef struct MMIMTV_PROG_HINT_INFO_T_tag
{	
    uint32      service_id;
    wchar       hint_current[MMIMTV_PROG_HINT_STR_LEN +1];
    wchar       hint_next[MMIMTV_PROG_HINT_STR_LEN +1];	
}MMIMTV_PROG_HINT_INFO_T;


//MBBMS start
#ifdef MBBMS_SUPPORT

//MBBMS SG service save node
typedef struct MMIMBBMS_SERVICE_INFO_T_tag
{
    anyURI      globalServiceID;    //频道唯一标识

    struct MMIMBBMS_SERVICE_INFO_T_tag *service_next;
}MMIMBBMS_SAVE_SERVICE_INFO_T;

//MBBMS SG content save node
typedef struct MMIMBBMS_CONTENT_INFO_T_tag
{
    anyURI        globalContentID;    //节目唯一标识
    uint32        event_id;
    SG_DATETIME_T starttime;

    struct MMIMBBMS_CONTENT_INFO_T_tag *content_next;
}MMIMBBMS_SAVE_CONTENT_INFO_T;

//MBBMS SG content save node
typedef struct MMIMBBMS_ALARM_CONTENT_INFO_T_tag
{
    anyURI        globalContentID;    //节目唯一标识
    uint32        event_id;
    SG_DATETIME_T starttime;
    struct MMIMBBMS_ALARM_CONTENT_INFO_T_tag *content_next;
}MMIMBBMS_ALARM_CONTENT_INFO_T;


#endif
//MBBMS end


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddEBNode(MMIMTV_EB_NODE_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteEBNode(MMIMTV_EB_NODE_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EBSortByTime(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EBSortByGrade(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearEBList(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_WriteEBFile(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_ReadEBFile(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetEBListNumber(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_EB_NODE_T* MMIMTV_GetEBNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : is have unread EB msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsHaveTobeReadEBMsg(void);

/*****************************************************************************/
// 	Description : get unread EB msg num
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMTV_GetUnreadEBMsgNum(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
// PUBLIC MMIMTV_EB_NODE_T* MMIMTV_GetEBListNode(uint32 index);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddServiceNode(
                                  MMIMTV_SAVE_NETEID_INFO_T *net_node, 
                                  MMIMTV_SAVE_SERVICE_INFO_T *service_node
                                  );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteServiceNode(
                                     MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                     MMIMTV_SAVE_SERVICE_INFO_T *service_node
                                     );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearServiceList(MMIMTV_SAVE_NETEID_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddNetNode(MMIMTV_SAVE_NETEID_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteNetNode(MMIMTV_SAVE_NETEID_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearNetList(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetServiceNodeViaIndex(
                                                                 const MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                                                 uint32 index
                                                                 );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaFrequency(uint32 frequency);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteServiceNodeViaIndex(
                                             MMIMTV_SAVE_NETEID_INFO_T *net_node, 
                                             uint32 index
                                             );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteNetNodeViaIdex(uint32 index);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteEBNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_WriteNSFile(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_ReadNSFile(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetPlayingNetNode(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetPlayingServiceNode(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPlayingNetNode(MMIMTV_SAVE_NETEID_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPlayingServiceNode(MMIMTV_SAVE_SERVICE_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_ChangeService(BOOLEAN is_next);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_ChangeNet(BOOLEAN is_next);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetNetListNumber(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetServiceListNumber(const MMIMTV_SAVE_NETEID_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetRecordFullFilePath(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIMTV_GetRecordFileName(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_CreateRecordFileName(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_InitSetting(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SaveSetting(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_SetPlayingNetServiceNodeViaID(
                                                    uint32 net_id, 
                                                    uint32 service_id
                                                    );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetPlayingServiceIndexViaNode(const MMIMTV_SAVE_SERVICE_INFO_T *node);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetCurrentProgramHint(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetNextProgramHint(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetServiceNodeViaID(
                                                                    uint32 net_id,
                                                                    uint32 service_id
                                                                    );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_NewDataData(void);

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteDataData(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsMute(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_VOLUME_E MMIMTV_GetVolume(void);

/*****************************************************************************/
// 	Description : get panel volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_VOLUME_E MMIMTV_GetPanelVolume(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_AUDIO_LANGUGAE_E MMIMTV_GetAudioLanguage(void);
     
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_GetSubtitleStreams(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_AUDIO_MODE_E MMIMTV_GetAudioMode(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_ZOOM_MODE_E MMIMTV_GetZoomMode(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_BRIGHTNESS_E MMIMTV_GetBrightness(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_CONTRAST_E MMIMTV_GetContrast(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_COLOR_SATURATION_E MMIMTV_GetColorSaturation(void);
      
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_PICTURE_SHARPNESS_E MMIMTV_GetPictureSharpness(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsDisplayProgramName(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetRecordStore(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetEBStore(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetSnapshotStore(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_DISPLAY_TIME_E MMIMTV_GetOSDDisplayTime(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_WATCH_TIME_TIP_E MMIMTV_GetWatchTimeTip(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_BROADCAST_BG_PIC_E MMIMTV_GetBroadcastBG(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_TRANSPARENCY_E MMIMTV_GetTransparency(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetRecordFileID(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordStartTime(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordLong(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordNetID(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordServiceID(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetBookRecordFileName(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetIsMute(BOOLEAN mute);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetVolume(MMIMTV_VOLUME_E volume);

/*****************************************************************************/
// 	Description : save panel volume value
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPanelVolume(MMIMTV_VOLUME_E volume);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetAudioLanguage(MMIMTV_AUDIO_LANGUGAE_E audio);
     
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetSubtitleStreams(BOOLEAN sub);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetAudioMode(MMIMTV_AUDIO_MODE_E mode);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetZoomMode(MMIMTV_ZOOM_MODE_E zoom);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBrightness(MMIMTV_BRIGHTNESS_E bright);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetContrast(MMIMTV_CONTRAST_E contrast);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetColorSaturation(MMIMTV_COLOR_SATURATION_E color);
      
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPictureSharpness(MMIMTV_PICTURE_SHARPNESS_E sharp);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetIsDisplayProgramName(BOOLEAN display);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetRecordStore(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetEBStore(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetSnapshotStore(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetOSDDisplayTime(MMIMTV_OSD_DISPLAY_TIME_E time);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetWatchTimeTip(MMIMTV_WATCH_TIME_TIP_E tip);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBroadcastBG(MMIMTV_BROADCAST_BG_PIC_E pic);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetTransparency(MMIMTV_OSD_TRANSPARENCY_E trans);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetRecordFileID(uint32 id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordStartTime(uint32 start);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordLong(uint32 record_long);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordNetID(uint32 id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordServiceID(uint32 id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordFileName(const wchar *file_name);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetSnapshotPicturePath(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetSnapshotPictureName(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_CreateSnapshotPictureName(void);

/*****************************************************************************/
// 	Description : set esg path
//	Global resource dependence : none
//  Author: 
//	Note: 设置ESG的工作路径
/*****************************************************************************/
PUBLIC void MMIMTV_SetESGPath(void);

/*****************************************************************************/
// 	Description : whether have enough space to save esg info
//	Global resource dependence : none
//  Author: 
//	Note: 确定有足够的空间保存ESG
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsEnoughESGSpace(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaNetID(uint32 net_id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_Aassert(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: return wstr actural length
/*****************************************************************************/
PUBLIC uint16 MMIMTV_Mtvstring2Unicode(MTV_STRING_T *mtv_string_ptr,  //in   mtv string
                                       wchar *wstr_ptr,             //out    unicode
                                       uint16 wstr_len              //in     unicode max len
                                       );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_RGB565ToYUV422(uint16 *pRgb565Buf, uint8 *pYBuf, uint8* pUVBuf,  uint32 width, uint32 height);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetBMPtoVChip(MMI_IMAGE_ID_T image_id);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMTV_GetSnapshotPictureNameLength(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:查询不到则返回头节点
/*****************************************************************************/
PUBLIC  uint32 MMIMTV_GetPreServiceID(uint32 net_id,
                                       uint32 service_id
                                       );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:查询不到则返回头节点
/*****************************************************************************/
PUBLIC  uint32 MMIMTV_GetNextServiceID(uint32 net_id,
                                       uint32 service_id
                                       );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SettingInfoSetFactory(void);

/*****************************************************************************/
// 	Description : calculate TV signal lenvel via signal value
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_CalcTVSignalLevel(uint32 signal_value);

/*****************************************************************************/
// 	Description : save tv signal level value 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SaveTVSignalLevel(uint8 signal_level);

/*****************************************************************************/
// 	Description : get tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetTVSignalLevel(void);




//MBBMS start
#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : read favorite service info file 
//	Global resource dependence : none
//  Author: 
//	Note: 读取用户收藏频道信息
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ReadFavoriteServiceFile(void);

/*****************************************************************************/
// 	Description : save favorite service info file 
//	Global resource dependence : none
//  Author: 
//	Note: 保存用户收藏频道信息
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_WriteFavoriteServiceFile(void);

/*****************************************************************************/
// 	Description : add favorite service node 
//	Global resource dependence : none
//  Author: 
//	Note: 添加收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddFavoriteServiceNode(MMIMBBMS_SAVE_SERVICE_INFO_T *node_ptr);

/*****************************************************************************/
// 	Description : delete a service node from favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNode(MMIMBBMS_SAVE_SERVICE_INFO_T *node_ptr);

/*****************************************************************************/
// 	Description : delete a service node from favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNodeByServiceID(anyURI *pGlobalServiceID);
/*****************************************************************************/
// 	Description : delete a service node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : get a service node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index获取频道节点
/*****************************************************************************/
PUBLIC MMIMBBMS_SAVE_SERVICE_INFO_T* MMIMBBMS_GetFavoriteServiceNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : clear up favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 清空收藏频道链表
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearFavoriteServiceList(void);

/*****************************************************************************/
// 	Description : get favorite service num
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetFavoriteServiceListNum(void);

/*****************************************************************************/
// 	Description : compare two anyURI index
//	Global resource dependence : none
//  Author: 
//	Note: equal return TRUE, else return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IndexCompare(const anyURI *index1, const anyURI *index2);

/*****************************************************************************/
// 	Description : 频道是否被收藏
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsFavoriteService(const anyURI *global_index);

/*****************************************************************************/
// 	Description : read alarm content info file 
//	Global resource dependence : none
//  Author: 
//	Note: 读取用户收藏节目信息
//  other:文件结构：|-version-|-node_num-|---------------node----------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ReadAlarmContentFile(void);

/*****************************************************************************/
// 	Description : write favorite content info file 
//	Global resource dependence : none
//  Author: 
//	Note: 保存用户收藏节目信息
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_WriteAlarmContentFile(void);

/*****************************************************************************/
// 	Description : add alarm content node 
//	Global resource dependence : none
//  Author: 
//	Note: 添加收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddAlarmContentNode(MMIMBBMS_ALARM_CONTENT_INFO_T *node_ptr);

/*****************************************************************************/
// 	Description : delete a content node from alarm content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNode(MMIMBBMS_ALARM_CONTENT_INFO_T *node_ptr);

/*****************************************************************************/
// 	Description : delete a content node from favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNodeByContentID(anyURI *pGlobalContentID);

/*****************************************************************************/
// 	Description : delete a content node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index删除收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : get a content node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index获取节目节点
/*****************************************************************************/
PUBLIC MMIMBBMS_ALARM_CONTENT_INFO_T* MMIMBBMS_GetAlarmContentNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : delete a content node from favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC MMIMBBMS_ALARM_CONTENT_INFO_T* MMIMBBMS_GetAlarmContentNodeByContentID(anyURI *pGlobalContentID);

/*****************************************************************************/
// 	Description : clear up favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 清空收藏节目链表
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearAlarmContentList(void);

/*****************************************************************************/
// 	Description : get favorite content num
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetAlarmContentListNum(void);

/*****************************************************************************/
// 	Description : 节目是否被收藏
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsAlarmContent(const anyURI *global_index);

/*****************************************************************************/
// 	Description : save key info of cmmb free service 
//	Global resource dependence : none
//  Author: 
//	Note: 保存CMMB清流频道参数信息,主要有network_id,service_id
/*****************************************************************************/
PUBLIC void MMIMBBMS_SaveFreeServiceInfoToFile(uint32 net_id,
                                               uint32 service_id
                                               );

#endif /* MBBMS_SUPPORT */
//MBBMS end

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _MMIMTV_SAVE_H_ */
#endif /* CMMB_SUPPORT */
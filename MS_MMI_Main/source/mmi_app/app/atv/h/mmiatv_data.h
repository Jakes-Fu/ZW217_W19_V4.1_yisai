/******************************************************************************
**  File Name:      mmiatv_data.h                                             *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_DATA_H_
#define _MMIATV_DATA_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmidisplay_data.h"
#include "mmifmm_export.h"
#include "sensor_atv.h"
#include "dal_recorder.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIATV_TRANSPARENT_COLOR        MMI_BLACK_COLOR
#define MMIATV_NAME_MAX_LEN             30		//频道名最大长度
#define MMIATV_COMMON_MAX_LEN           255		//common max len
#define MMIATV_FULL_PATH_LEN            MMIFMM_FULL_FILENAME_LEN
#define MMIATV_FAVORITE_MAX_NUM         99      //收藏频道最大数目

#define MMIATV_RECORD_MIN_SPACE_SIZE        (200*1024) 
#define MMIATV_STRING_BUFFER_SIZE           20
        
#define MMIATV_VIDEO_TYPE_STRING_3GP           L"3gp"
#define MMIATV_VIDEO_TYPE_STRING_3GP_LEN       3
#define MMIATV_VIDEO_TYPE_STRING_AVI           L"avi"
#define MMIATV_VIDEO_TYPE_STRING_AVI_LEN		3   

#define MMIATV_VIDEO_FILE_SUFFIX_3GP           "3gp"  
#define MMIATV_VIDEO_FILE_SUFFIX_AVI           "avi"
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIATV_SNAPSHOT_ERROR_NONE,
    MMIATV_SNAPSHOT_NO_SPACE,
    MMIATV_SNAPSHOT_ENCODE_ERROR,
    MMIATV_SNAPSHOT_ERROR_MAX
} MMIATV_SNAPSHOT_ERROR_E;

typedef enum
{
    MMIATV_VIDEO_TYPE_3GP = 0,
    MMIATV_VIDEO_TYPE_AVI,
    MMIATV_VIDEO_TYPE_MAX
}MMIATV_VIDEO_FILE_TYPE_E;

typedef struct MMIATV_SERVICE_NODE_T_tag
{
    ATV_REGION_E                region;
    uint32                      service_id;
    wchar                       service_name[MMIATV_NAME_MAX_LEN + 1];
    struct MMIATV_SERVICE_NODE_T_tag   *service_next_ptr;
}MMIATV_SERVICE_NODE_T;

typedef struct MMIATV_SETTING_INFO_T_tag 
{
    uint32          brightness;
    uint32          record_file_id;
    MMIFILE_DEVICE_E    record_dev;
    ATV_REGION_E    tv_region;
    MMIATV_VIDEO_FILE_TYPE_E video_type;
} MMIATV_SETTING_INFO_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init atv data info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_InitData(void);

/*****************************************************************************/
// 	Description : release atv data info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DeleteData(void);

/*****************************************************************************/
// 	Description : atv assert
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_Assert(void);

/*****************************************************************************/
// 	Description : init setting info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_InitSetting(void);

/*****************************************************************************/
// 	Description : save setting info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveSetting(void);

/*****************************************************************************/
// 	Description : restore factory setting
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_RestoreFactorySetting(void);

/*****************************************************************************/
// 	Description : clear user data
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearUserData(void);

/*****************************************************************************/
// 	Description : set brightness
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetBrightness(uint32 brightness);

/*****************************************************************************/
// 	Description : get brightness
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetBrightness(void);

/*****************************************************************************/
// 	Description : save tv region
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveRegion(ATV_REGION_E region);

/*****************************************************************************/
// 	Description : get tv region
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC ATV_REGION_E MMIATV_GetRegion(void);

/*****************************************************************************/
// 	Description : get real record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIATV_GetRealRecordDeviceName(void);

/*****************************************************************************/
// 	Description : get record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIATV_GetRecordDevice(void);

/*****************************************************************************/
// 	Description : set record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetRecordDevice(MMIFILE_DEVICE_E dev);

#ifdef MMIATV_RECORD_3GP_SUPPORT //norflashcut_atv
/*****************************************************************************/
// 	Description : save video type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetVideoFileType(MMIATV_VIDEO_FILE_TYPE_E type);
#endif

/*****************************************************************************/
// 	Description : get video type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_VIDEO_FILE_TYPE_E MMIATV_GetVideoFileType(void);

/*****************************************************************************/
// 	Description : get record file id
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetRecordID(void);

/*****************************************************************************/
// 	Description : set record file id
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetRecordID(uint32 file_id);

/*****************************************************************************/
// 	Description : read service list file
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_ReadServiceListFile(void);

/*****************************************************************************/
// 	Description : write/save service list file
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_WriteServiceListFile(void);

/*****************************************************************************/
// 	Description : add service node
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_AddServiceNode(MMIATV_SERVICE_NODE_T *node_ptr);

/*****************************************************************************/
// 	Description : clear service list
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearServiceList(void);

/*****************************************************************************/
// 	Description : get service list node number
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetServiceListNum(void);

/*****************************************************************************/
// 	Description : get service node via index
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_SERVICE_NODE_T* MMIATV_GetServiceNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : read favorite service list file
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_ReadFavoriteServiceListFile(void);

/*****************************************************************************/
// 	Description : write/save favorite service list file
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_WriteFavoriteServiceListFile(void);

/*****************************************************************************/
// 	Description : add favorite service node
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_AddFavoriteServiceNode(MMIATV_SERVICE_NODE_T *node_ptr);

/*****************************************************************************/
// 	Description : delete favorite service node
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DeleteFavoriteServiceNode(MMIATV_SERVICE_NODE_T *node_ptr);

/*****************************************************************************/
// 	Description : clear favorite service list
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearFavoriteServiceList(void);

/*****************************************************************************/
// 	Description : get favorite service list number
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetFavoriteServiceListNum(void);

/*****************************************************************************/
// 	Description : get favorite service node via index
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_SERVICE_NODE_T* MMIATV_GetFavoriteServiceNodeViaIndex(uint32 index);

/*****************************************************************************/
// 	Description : get current video name
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIATV_GetVideoName(void);

/*****************************************************************************/
//  Description : 指定录像文件名
//  Global resource dependence :                                
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_CreateRecordFileName(DRECORDER_START_RECORD_T* recorder_start_t);// [OUT]

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _MMIATV_DATA_H_ */
#endif /* ATV_SUPPORT */



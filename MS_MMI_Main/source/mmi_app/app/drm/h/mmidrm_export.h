/****************************************************************************
** File Name:      mmidrm_export.h                                          *
** Author:         haiwu.chen                                               *
** Date:           03/29/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the interface of drm op    *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/29/2011     haiwu.chen        Create                                 *
****************************************************************************/
#ifndef  _DRM_EXPORT_H_    
#define  _DRM_EXPORT_H_   


#if defined(DRM_SUPPORT)

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "drm_api.h"
#include "mmk_type.h"
#include "ffs.h"
#include "mmifmm_export.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm.h"
#include "mmidrm_image.h"
#include "mmidrm_text.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
// drm file setting the type
typedef enum
{
    DRMFILE_SETTING_NONE,        
    DRMFILE_SETTING_WALLPAPER,
    DRMFILE_SETTING_SCREENSAVER,
    DRMFILE_SETTING_POWERONOFFANIM,
    DRMFILE_SETTING_CALLRING,
    DRMFILE_SETTING_MAXNUM
    
}DRMFILE_SETTING_TYPE;

// drm pre-check status value
typedef enum
{
    DRMFILE_PRE_CHECK_NORMAL,           //һ������
    DRMFILE_PRE_CHECK_INVALID_PARAM,    //��Ч����   
    DRMFILE_PRE_CHECK_INVALID_HANDLE,   //��Ч���
    DRMFILE_PRE_CHECK_NO_SYS_NITZ,      //����ʱ��δͬ��
    DRMFILE_PRE_CHECK_NO_RIGHTS,        //��Ȩ����
    DRMFILE_PRE_CHECK_MAXNUM
    
}DRMFILE_PRE_CHECK_STATUS_E;     

extern const wchar g_mmidrm_tmp_dir[];

#define MMIDRMFILE_SPTMP_DIR            (g_mmidrm_tmp_dir)   //name: "drm_sptmp"
#define MMIDRMFILE_SPTMP_DIR_LEN        9

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*��Э����ص�DRM����*/

//ע�Ȿ�ĵĴ��������ξ���һ��ɫ��ϣ���ڿ�ͷ�ͽ�������ס�
//DRM�漰���ļ�������ϣ���û������ļ��������������������ȥ�򿪣��ر�Ӱ��Ч��
//����Ϊ�˴�ҷ��㣬Ҳ�ṩ��·���Ĳ�����ʽ����������б�����:handle, file_name
//ʵ��ʱ�������ȴ����ļ����������ļ����Ϊnull�����ȥ����file_name����
//��:����д����Ȩ֮��ģ����ȼ����handle��

/*****************************************************************************/
//  Description : drm module init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitModule(void);

/*****************************************************************************/
//  Description : init drm module nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIDRM_RegNv(void);

/*****************************************************************************/
//  Description : drm init
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_Init(void);

/*****************************************************************************/
//  Description : check for drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: �����file_handle���룬�����ȴ���������file_name
//  return: True: is drm file
//          False: is not drm file
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsDRMFile(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : check the rights file is expired
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: permission �û����봫����ȷ��permission,���磬picture: display
//        �����file_handle���룬�����ȴ���������file_name
//  return: True: drm file is expired
//          False: drm file is not expired
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsRightsExpired(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    DRM_PERMISSION_MODE_E permission
);
    
/*****************************************************************************/
//  Description : check whether the file can be forwarded
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ���û��rights_ptr���룬�����ȴ�����֮file_handle,�����file_name
//  return: True: drm file can forward
//          False: drm file can not forward
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsCanForward(
    DRM_RIGHTS_T *rights_ptr,           //[in] handle priority if rights is not null
    MMIDRMFILE_HANDLE file_handle,      //[in] handle secondly if not null
    const uint16 *file_name             //[in] handle last if not null
);
    
/*****************************************************************************/
//  Description : check rights valid by rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsRightsValid(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    BOOLEAN is_care_permission,         //[in] �󲿷�Ӧ�ö�Ӧ�ù��ģ�Ȼ����permission
    DRM_PERMISSION_MODE_E permission    //[in] �������͵��ļ���Ӧ��һ�ִ򿪷�ʽ
);

/*****************************************************************************/
//  Description : get drm rights info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: �����file_handle���룬�����ȴ���������file_name
/*****************************************************************************/
PUBLIC MMIDRMFILE_ERROR_E MMIAPIDRM_GetRightsinfo(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name,            //[in] if file_handle is null, handle path
    DRM_RIGHTS_T *rights_ptr            //[out] get the rights
);

/*****************************************************************************/
//  Description : get drm file permission by mine tye
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ������ļ�ý����������ȡ�ļ���permission
/*****************************************************************************/
PUBLIC DRM_PERMISSION_MODE_E MMIAPIDRM_GetFilePermission(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : consume drm rights
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_ConsumeRights(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get drm rights Constraint info
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_GetRightsConstraint(
    MMIDRMFILE_HANDLE file_handle,      //[in] 
    DRM_CONSTRAINT_T *rights_cons_ptr   //[out] 
);

/*****************************************************************************/
//  Description : get drm rights interval time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_GetDateIntervalTime(
    MMIDRMFILE_HANDLE file_handle,      //[in]
    DRM_RIGHTS_INTERVAL_T *interval_ptr //[out]
);

/*****************************************************************************/
// Description : get time left by interval
// Global resource dependence : 
// Author:haiwu.chen
// Note: ͨ��interval�õ�ʣ��ʱ�������ֵ
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetTimeLeft(
    const DRM_CONSTRAINT_T *right_cons_ptr
);

/*****************************************************************************/
// Description : translator left time to year type
// Global resource dependence : 
// Author:haiwu.chen
// Note: ͨ�����������ʣ�༸��ȣ��͵�ǰʱ���
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_TransLeftTimeToYearType(
    uint32 left_time,
    MMIDRMFILE_LEFT_TM_T *left_time_ptr
);

/*****************************************************************************/
//  Description : get the type of drm file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_METHOD_LEVEL_E MMIAPIDRM_GetDRMFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get the type of drm file mime
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDRM_GetDRMFileMimeType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : get media file type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIDRM_GetMediaFileType(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : download drm rights file
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_DownloadRightsFile(
    MMIDRMFILE_HANDLE file_handle,      //[in] handle priority if not null
    const uint16 *file_name             //[in] if file_handle is null, handle path
);

/*****************************************************************************/
//  Description : set encrypt
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIDRM_SDSetEncrypt(
    MMIDRMFILE_HANDLE file_handle, 
    BOOLEAN is_encrpt_Read
);

/*****************************************************************************/
//  Description : handle the download drm and rights files
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC DRM_ERROR_E MMIAPIDRM_InstallFileReq(
    DRM_INSTALL_FILE_T  *file_handle_ptr
);

/*****************************************************************************/
//  Description : delete agent
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: pair with MMIDRM_InitAgent
/*****************************************************************************/
PUBLIC void MMIAPIDRM_DeInitAgent(void);


/*��Ӧ����صĲ���*/

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ��ʹ��DRM�ļ�֮ǰ��ͳһһ������ļ�״̬��
//  ע���������ֵΪ��DRMFILE_PRE_CHECK_NORMAL����ԭ����ԭ����Ӧ��stop
/*****************************************************************************/
PUBLIC DRMFILE_PRE_CHECK_STATUS_E MMIAPIDRM_PreCheckFileStatus(
    wchar *file_path_ptr,                   //in
    DRM_PERMISSION_MODE_E permmison         //in
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: ��ʹ��DRM�ļ�֮ǰ��ͳһһ������ļ�״̬��
//  SD����ʾ���󲢷���TRUE������ֵ��Ӧ״̬������FALSE��   ������������ʾ��Ӧ��ʹ�ã�SD���ذ�Ȩ���ã�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_PreCheckFileStatusEX(
                                      wchar *file_path_ptr,                   //in
                                      DRM_PERMISSION_MODE_E permmison,         //in
                                      DRMFILE_PRE_CHECK_STATUS_E *drm_status_ptr   //out
                                                               );

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: ��ʹ��DRM�ļ�֮ǰ��ͳһһ������ļ�״̬��
//  ע���������ֵΪ��DRMFILE_PRE_CHECK_NORMAL����ԭ����ԭ����Ӧ��stop
/*****************************************************************************/
PUBLIC DRMFILE_PRE_CHECK_STATUS_E MMIAPIDRM_PreCheckFileStatusExForMMS(
                                      wchar *file_path_ptr,                   //in
                                      DRM_PERMISSION_MODE_E permmison,         //in
                                      BOOLEAN is_alert_win
                                                               );

/*****************************************************************************/
//  Description : Can the file set to all the type,include Wallpaper,ScreenSaver,CallRing
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: ���ļ��Ƿ���Ա����ó��ֻ��ı�ֽ���������������������ػ���������������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_CanDrmFileSetToWSPC(
    uint16 *file_path_ptr, 
    uint16 file_path_len,
    DRMFILE_SETTING_TYPE setting_type
);

/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDRM_ConvertAudioMimeType(
                            MIME_TYPE_E mime_type
                            );

/*****************************************************************************/
//  Description : is the network time syn
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDRM_IsSynNitzTime(void);

/*****************************************************************************/
//  Description : init nv MMINV_SET_NETWORK_TIME_NUM
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_InitNetworkTime(void);

/*****************************************************************************/
//  Description : get the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetNetworkTime(MMIDRM_NITZ_TYPE_T *drm_nitz_ptr);

/*****************************************************************************/
//  Description : set the result of local time minus network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetNetworkTime(MMIDRM_NITZ_TYPE_T drm_nitz);

/*****************************************************************************/
//  Description : get the global nitz_time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_GetDrmNitzTime(MMIDRM_NITZ_TYPE_T *nitz_time_ptr);

/*****************************************************************************/
//  Description : set the global nitz_time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetDrmNitzTime(MMIDRM_NITZ_TYPE_T nitz_time);

/*****************************************************************************/
//  Description : read network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: �����󣬶�ȡNV������ʱ�䣬������ȫ��ֵ
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ReadDrmNitzTime(void);

/*****************************************************************************/
//  Description : update net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ������ʱ�򣬼�鱾�ֻ��������Ƿ�ͬ��������ʱ��һ�Σ����û�У������
//      ����ʱ�䲻������Ҹ��µģ�һ��Ҫ���һ�£��Ƿ��б�Ҫ����Ȼ�����˷���Դ
/*****************************************************************************/
PUBLIC void MMIAPIDRM_UpdateNetworkTime(void);

/*****************************************************************************/
//  Description : clear net work time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ����ʱ�䱻�˸����ˣ�����Ҫ����һ�£������ǹ��ĵ�flag�������ֵ
/*****************************************************************************/
PUBLIC void MMIAPIDRM_ResetNetworkTime(void);

/*****************************************************************************/
//  Description : restore factory network time
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: �ָ��������ã�����Ҫ�ѱ�־λ��0����ʾ�ֻ��������δͬ����
/*****************************************************************************/
PUBLIC void MMIAPIDRM_RestoreFactoryNetworkTime(void);

/*****************************************************************************/
//  Description : alter network time when system time is changed
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ά������ʱ���ֵ����ϵͳʱ�䱻�޸���
/*****************************************************************************/
PUBLIC void MMIAPIDRM_AlterNetworkTime(
    uint32 old_time,
    uint32 new_time
);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif  //#if defined(DRM_SUPPORT)

#endif /* _DRM_EXPORT_H_ */

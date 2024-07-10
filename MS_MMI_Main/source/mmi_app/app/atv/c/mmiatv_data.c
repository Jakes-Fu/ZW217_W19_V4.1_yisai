/*****************************************************************************
** File Name:      mmiatv_data.c                                             *
** Author:                                                                   *
** Date:           2010-05	                                                 *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv data                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_atv_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiatv_data.h"
#include "mmiatv_nv.h"
#include "mmifmm_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_display.h"
#include "os_api.h"
#include "mmidc_save.h"
#include "mmibt_export.h"


/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIATV_SNAPSHOT_MIN_SIZE        (100 * 1024)
#define MMIATV_SAVE_FILE_VERSION        2
#define MMIATV_RECORD_FILE_ID_MAX       100000

LOCAL const wchar       s_atv_default_dir[] = {'a', 't', 'v', 0};	
LOCAL const wchar       s_atv_service_name[] = {'s', 'e', 'r', 'v', 'i', 'c', 'e', '.', 's', 'y', 's', 0};
LOCAL const wchar       s_atv_fs_name[] = {'f', 's', '.', 's', 'y', 's', 0};	

#define ATV_DEFAULT_DIR    (s_atv_default_dir)
#define ATV_DEFAULT_DIR_LEN 3
#define SERVICE_NAME       (s_atv_service_name)
#define SERVICE_NAME_LEN    11
#define FS_NAME            (s_atv_fs_name)
#define FS_NAME_LEN        6
#define SNAPSHOT_PATH       {'D', ':', '\\', 'P', 'h', 'o', 't', 'o', 's', 0}

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

typedef struct MMIATV_DATA_INFO_T_tag
{
    MMIATV_SERVICE_NODE_T       *service_head_ptr;
    uint32                      service_num;
    MMIATV_SERVICE_NODE_T       *favorite_service_head_ptr;
    uint32                      favorite_service_num;
    MMIATV_SERVICE_NODE_T       *playing_service_node;
    uint8                       signal_value;
    wchar                       service_full_path[MMIATV_FULL_PATH_LEN +1];
    wchar                       fs_full_path[MMIATV_FULL_PATH_LEN +1];
    wchar                       full_path_record_name[MMIATV_FULL_PATH_LEN +1];
    wchar                       record_name[MMIATV_FULL_PATH_LEN +1];
    MMIATV_SETTING_INFO_T       atv_settings;
} MMIATV_DATA_INFO_T;

static MMIATV_DATA_INFO_T        *s_data_ptr = PNULL;   //atv应用数据信息
//#define MMIATV_DATA_PTR         (PNULL != s_data_ptr ? s_data_ptr : (MMIATV_DATA_INFO_T*)MMIATV_Assert())

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : clear favourte service list user data
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ClearFavoriteServiceData(void);

/*****************************************************************************/
// 	Description : clear service list user data
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ClearServiceListData(void);

/*****************************************************************************/
//  Description : 是否初始化data
//  Global resource dependence :                                
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInitData(void);

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init atv data info
//	Global resource dependence :
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIATV_InitData(void)
{    
    uint16 full_path_len = MMIATV_FULL_PATH_LEN;

    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_InitData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_91_112_2_18_2_3_6_0,(uint8*)"");
    if (s_data_ptr != PNULL)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_InitData s_data_ptr != PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_94_112_2_18_2_3_6_1,(uint8*)"");
        return;
    }
    
    s_data_ptr = (MMIATV_DATA_INFO_T*)SCI_ALLOC_APP(sizeof(MMIATV_DATA_INFO_T));
    if(PNULL == s_data_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]:s_data_ptr alloc mem error!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_101_112_2_18_2_3_6_2,(uint8*)"");
        return;
    }
    SCI_MEMSET(s_data_ptr, 0, sizeof(MMIATV_DATA_INFO_T));
    
    MMIAPIFMM_GetSysFileFullPathForWrite(
                                       TRUE,
                                       ATV_DEFAULT_DIR,
                                       ATV_DEFAULT_DIR_LEN,
                                       SERVICE_NAME,
                                       SERVICE_NAME_LEN,
                                       0,
                                       s_data_ptr->service_full_path,   
                                       &full_path_len
                                       );

    MMIAPIFMM_GetSysFileFullPathForWrite(
                                       TRUE,
                                       ATV_DEFAULT_DIR,
                                       ATV_DEFAULT_DIR_LEN,
                                       FS_NAME,
                                       FS_NAME_LEN,
                                       0,
                                       s_data_ptr->fs_full_path,   
                                       &full_path_len
                                       );
}

/*****************************************************************************/
// 	Description : release atv data info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DeleteData(void)
{
    if(PNULL != s_data_ptr)
    {
        if(PNULL != s_data_ptr->service_head_ptr)
        {
            MMIATV_ClearServiceList();
        }
        
        if(PNULL != s_data_ptr->favorite_service_head_ptr)
        {
            MMIATV_ClearFavoriteServiceList();
        }
        
        SCI_Free(s_data_ptr);
        s_data_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : atv assert
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_Assert(void)
{
    SCI_ASSERT(0);/* assert to do*/
    return 0;   /*lint !e527*/
}

/*****************************************************************************/
// 	Description : init tv region by current language type
//	Global resource dependence :
//  Author: xin.li
//	Note: 根据当前语言初始化Region
/*****************************************************************************/
LOCAL void InitRegionViaCurLangType(ATV_REGION_E *region_ptr)
{
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;

    if(PNULL == region_ptr)
    {
        return;
    }
    
    MMIAPISET_GetLanguageType(&lang_type);
    switch(lang_type)
    {
    case MMISET_LANGUAGE_ENGLISH:
    case MMISET_LANGUAGE_SIMP_CHINESE:
        *region_ptr = ATV_REGION_CHINA;
        break;
        
    case MMISET_LANGUAGE_TRAD_CHINESE:
        *region_ptr = ATV_REGION_TAIWAN;
        break;
        
    case MMISET_LANGUAGE_ARABIC:
        //一种语言对应多个国家，取其一
        *region_ptr = ATV_REGION_UAE;
        break;
        
    case MMISET_LANGUAGE_FRENCH:
    case MMISET_LANGUAGE_ITALIAN:
    case MMISET_LANGUAGE_GREEK:
        *region_ptr = ATV_REGION_WESTERNEUROPE;
        break;
        
    case MMISET_LANGUAGE_HINDI:
        *region_ptr = ATV_REGION_INDIA;
        break;
        
    case MMISET_LANGUAGE_HUNGARIAN:
        *region_ptr = ATV_REGION_HUNGRY;
        break;
        
    case MMISET_LANGUAGE_INDONESIAN:
        *region_ptr = ATV_REGION_INDONESIA;
        break;
        
    case MMISET_LANGUAGE_MALAY:
        *region_ptr = ATV_REGION_MALAYSIA;
        break;
        
    case MMISET_LANGUAGE_PORTUGUESE:
        *region_ptr = ATV_REGION_PORTUGAL;
        break;
        
    case MMISET_LANGUAGE_RUSSIAN:
        *region_ptr = ATV_REGION_RUSSIA;
        break;
        
    case MMISET_LANGUAGE_SPANISH:
        *region_ptr = ATV_REGION_SPAIN;
        break;
        
    case MMISET_LANGUAGE_TAGALOG:
        *region_ptr = ATV_REGION_PHILIPPINES;
        break;
        
    case MMISET_LANGUAGE_THAI:
        *region_ptr = ATV_REGION_THAILAND;
        break;
        
    case MMISET_LANGUAGE_VIETNAMESE:
        *region_ptr = ATV_REGION_VIETNAM;
        break;
        
    case MMISET_LANGUAGE_URDU:
        *region_ptr = ATV_REGION_PAKISTAN;
        break;
        
    case MMISET_LANGUAGE_PERSIAN:
        *region_ptr = ATV_REGION_IRAN;
        break;
        
    case MMISET_LANGUAGE_TURKISH:
        *region_ptr = ATV_REGION_TURKEY;
        break;
        
    case MMISET_LANGUAGE_GERMAN:
        *region_ptr = ATV_REGION_GERMANY;
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : init setting info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_InitSetting(void)
{
    MN_RETURN_RESULT_E    	return_value = MN_RETURN_FAILURE;
    
	if(IsInitData())
	{
		MMINV_READ(MMINV_ATV_SETTING_INFO, &s_data_ptr->atv_settings, return_value);
		if(MN_RETURN_SUCCESS != return_value)
		{
			SCI_MEMSET(&s_data_ptr->atv_settings, 0, sizeof(MMIATV_SETTING_INFO_T)); /*lint !e666*/
			//bright
			s_data_ptr->atv_settings.brightness = MMISET_CONTRAST_SIX;
			s_data_ptr->atv_settings.record_dev = MMI_DEVICE_SDCARD;
			s_data_ptr->atv_settings.record_file_id = 0;
#ifdef AVI_ENC_SUPPORT			
			s_data_ptr->atv_settings.video_type = MMIATV_VIDEO_TYPE_AVI;
#else
			s_data_ptr->atv_settings.video_type = MMIATV_VIDEO_TYPE_3GP;
#endif
        
			//region
			InitRegionViaCurLangType(&s_data_ptr->atv_settings.tv_region);
        
			MMINV_WRITE(MMINV_ATV_SETTING_INFO, &s_data_ptr->atv_settings);
		}
		if(s_data_ptr->atv_settings.brightness > MMISET_CONTRAST_SEVEN)
        {
			s_data_ptr->atv_settings.brightness = MMISET_CONTRAST_SIX;
			MMINV_WRITE(MMINV_ATV_SETTING_INFO, &s_data_ptr->atv_settings);
		}
	}
}

/*****************************************************************************/
// 	Description : save setting info
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveSetting(void)
{
    if(IsInitData())
    {
        MMINV_WRITE(MMINV_ATV_SETTING_INFO, &s_data_ptr->atv_settings);
    }
}

/*****************************************************************************/
// 	Description : restore factory setting
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_RestoreFactorySetting(void)
{
    MMIATV_SETTING_INFO_T atv_setting = {0};
    
    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_RestoreFactorySetting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_304_112_2_18_2_3_7_3,(uint8*)"");
    SCI_MEMSET(&atv_setting, 0, sizeof(MMIATV_SETTING_INFO_T));
    //bright
    atv_setting.brightness = MMISET_CONTRAST_SIX;
    atv_setting.record_dev = MMI_DEVICE_SDCARD;
    atv_setting.record_file_id = 0;
#ifdef AVI_ENC_SUPPORT
    atv_setting.video_type = MMIATV_VIDEO_TYPE_AVI;
#else
	atv_setting.video_type = MMIATV_VIDEO_TYPE_3GP;
#endif

    //region
    InitRegionViaCurLangType(&atv_setting.tv_region);
    
    MMINV_WRITE(MMINV_ATV_SETTING_INFO, &atv_setting);
}

/*****************************************************************************/
// 	Description : clear favourte service list user data
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ClearFavoriteServiceData(void)
{
	wchar                       abs_default_path[ MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      abs_default_path_length             =  MMIFMM_FILE_FILE_NAME_MAX_LEN;
	MMIFILE_DEVICE_E            dev_type                            = MMI_DEVICE_NUM - 1;
	
	for (dev_type = MMI_DEVICE_NUM - 1; (dev_type >= MMI_DEVICE_UDISK); dev_type--)
	{
		MMIAPIFMM_CombineSysPath(dev_type,
			ATV_DEFAULT_DIR,
			ATV_DEFAULT_DIR_LEN,
            FS_NAME,
            FS_NAME_LEN,
			abs_default_path,
			&abs_default_path_length);

		if(MMIAPIFMM_IsFileExist(abs_default_path, abs_default_path_length))
        {
            MMIAPIFMM_DeleteFileSyn(abs_default_path, abs_default_path_length);
        }
	}
            
}

/*****************************************************************************/
// 	Description : clear service list user data
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void ClearServiceListData(void)
{
	wchar                       abs_default_path[ MMIFMM_FILE_FILE_NAME_MAX_LEN+1] = {0};
    uint16                      abs_default_path_length             =  MMIFMM_FILE_FILE_NAME_MAX_LEN;
	MMIFILE_DEVICE_E            dev_type                            = MMI_DEVICE_NUM - 1;
	
	for (dev_type = MMI_DEVICE_NUM - 1; (dev_type >= MMI_DEVICE_UDISK); dev_type--)
	{
		MMIAPIFMM_CombineSysPath(dev_type,
			ATV_DEFAULT_DIR,
			ATV_DEFAULT_DIR_LEN,
            SERVICE_NAME,
            SERVICE_NAME_LEN,
			abs_default_path,
			&abs_default_path_length);

		if(MMIAPIFMM_IsFileExist(abs_default_path, abs_default_path_length))
        {
            MMIAPIFMM_DeleteFileSyn(abs_default_path, abs_default_path_length);
        }
	}
}

/*****************************************************************************/
// 	Description : clear user data
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearUserData(void)
{
	MMIATV_RestoreFactorySetting();
	ClearFavoriteServiceData();
	ClearServiceListData();
}

/*****************************************************************************/
// 	Description : set brightness
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetBrightness(uint32 brightness)
{
	if (IsInitData())
	{	
		s_data_ptr->atv_settings.brightness = brightness;
	}
}

/*****************************************************************************/
// 	Description : get brightness
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetBrightness(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->atv_settings.brightness;
	}
	return MMISET_CONTRAST_SIX;
}

/*****************************************************************************/
// 	Description : save tv region
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SaveRegion(ATV_REGION_E region)
{
	if (IsInitData())
	{	
		s_data_ptr->atv_settings.tv_region = region;
		MMINV_WRITE(MMINV_ATV_SETTING_INFO, &s_data_ptr->atv_settings);
	}
}

/*****************************************************************************/
// 	Description : get tv region
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC ATV_REGION_E MMIATV_GetRegion(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->atv_settings.tv_region;
	}
	return ATV_REGION_CHINA;
}

/*****************************************************************************/
// 	Description : get real record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIATV_GetRealRecordDeviceName(void)
{
    MMIFILE_DEVICE_E  device_type = MMI_DEVICE_UDISK;
    MMI_TEXT_ID_T       text_id = TXT_NULL;

    if(PNULL != s_data_ptr)
    {
        device_type = MMIAPIFMM_GetDeviceTypeByPath(s_data_ptr->full_path_record_name,MMIFILE_DEVICE_LEN);
        text_id = MMIAPIFMM_GetDeviceName(device_type);
    }
    
    return text_id;
}

/*****************************************************************************/
// 	Description : get record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIATV_GetRecordDevice(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->atv_settings.record_dev;
	}
	return MMI_DEVICE_SDCARD;
}

/*****************************************************************************/
// 	Description : set record device
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetRecordDevice(MMIFILE_DEVICE_E dev)
{
	if (IsInitData())
	{	
		s_data_ptr->atv_settings.record_dev = dev;
	}
}
#ifdef MMIATV_RECORD_3GP_SUPPORT //norflashcut_atv
/*****************************************************************************/
// 	Description : save video type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetVideoFileType(MMIATV_VIDEO_FILE_TYPE_E type)
{
	if (IsInitData())
	{	
		s_data_ptr->atv_settings.video_type = type;
	}
}
#endif

/*****************************************************************************/
// 	Description : get video type
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_VIDEO_FILE_TYPE_E MMIATV_GetVideoFileType(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->atv_settings.video_type;
	}
#ifdef AVI_ENC_SUPPORT
	return MMIATV_VIDEO_TYPE_AVI;
#else
	return MMIATV_VIDEO_TYPE_3GP;
#endif
}

/*****************************************************************************/
// 	Description : get record file id
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetRecordID(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->atv_settings.record_file_id;
	}
	return 0;
}

/*****************************************************************************/
// 	Description : set record file id
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_SetRecordID(uint32 file_id)
{
	if (IsInitData())
	{	
		s_data_ptr->atv_settings.record_file_id = file_id;
	}
}

/*****************************************************************************/
// 	Description : read service list file
//	Global resource dependence :
//  Author: xin.li
//	Note: 文件结构 |-version-|-num-|---------------service_nodes--------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_ReadServiceListFile(void)
{
    uint32           read_size  = 0;
    uint32           list_num   = 0;
    //uint32              write_offset = 0;
    MMIFILE_HANDLE   file_handle = 0;
    uint32           version = 0;
    MMIFILE_DEVICE_E dev     = MMI_DEVICE_NUM -1;
    //BOOLEAN          file_ok = FALSE;
    
    if( !IsInitData()
		|| PNULL != s_data_ptr->service_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadServiceListFile service_head_ptr not null! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_510_112_2_18_2_3_7_4,(uint8*)"");
        return FALSE;
    }
    //遍历所有DEV，查找文件是否存在
    for(dev=MMI_DEVICE_NUM -1; dev>=MMI_DEVICE_UDISK; dev--)
    {
        MMIAPICOM_Wstrncpy(s_data_ptr->service_full_path,MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev));
        if(MMIAPIFMM_IsFileExist(s_data_ptr->service_full_path, (uint16)MMIAPICOM_Wstrlen(s_data_ptr->service_full_path)))
        {
            break;
        }
    }    
    if(dev < MMI_DEVICE_UDISK)
    {
        return FALSE;
    }
    
    file_handle = MMIAPIFMM_CreateFile(s_data_ptr->service_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadServiceListFile create file error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_530_112_2_18_2_3_7_5,(uint8*)"");
        return FALSE;
    }
     
    //读取版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &version, sizeof(uint32), &read_size, NULL))
    {
		//SCI_TRACE_LOW:"[MMIATV]: MMIAPIFMM_ReadFile error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_537_112_2_18_2_3_7_6,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //确认版本信息读取成功
    if(sizeof(uint32) != read_size)
    {
        //失败，文件为空
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //版本信息确认
    if(version != MMIATV_SAVE_FILE_VERSION)
    {
        //版本不匹配
		//SCI_TRACE_LOW:"[MMIATV]: ServiceListFile version error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_552_112_2_18_2_3_7_7,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        MMIAPIFMM_DeleteFile(s_data_ptr->service_full_path, PNULL);
        return FALSE;
    }
    
    //获取节点个数
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, NULL))
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIAPIFMM_ReadFile error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_561_112_2_18_2_3_7_8,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadServiceListFile total list num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_565_112_2_18_2_3_7_9,(uint8*)"d", list_num);
    
    while (list_num --)
    {
        MMIATV_SERVICE_NODE_T node = {0};
        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &node, sizeof(MMIATV_SERVICE_NODE_T), &read_size, NULL))
        {
            MMIATV_AddServiceNode(&node);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIATV]: MMIAPIFMM_ReadFile error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_576_112_2_18_2_3_7_10,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : write/save service list file
//	Global resource dependence :
//  Author: xin.li
//	Note: 文件结构 |-version-|-num-|---------------service_nodes--------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_WriteServiceListFile(void)
{
    uint32                  list_num = 0;
    uint32                  write_len = 0;
    MMIFILE_HANDLE          file_handle = 0;
    MMIATV_SERVICE_NODE_T*  cursor = PNULL;
    uint32                  struct_size = sizeof(MMIATV_SERVICE_NODE_T);
    uint32                  total_size = 0;
    uint32                  version = MMIATV_SAVE_FILE_VERSION;
    MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM -1;
    MMIFILE_DEVICE_E        ret_dev  = MMI_DEVICE_NUM -1;
    
    //file_dev = MMIAPIFMM_GetDeviceTypeByPath(MMIATV_DATA_PTR->service_full_path, MMIFILE_DEVICE_LEN);
    
	if (!IsInitData())
	{
		SCI_TRACE_LOW("[MMIATV] MMIATV_WriteServiceListFile not init data");
		return FALSE;
	}

	list_num = s_data_ptr->service_num;
	cursor = s_data_ptr->service_head_ptr;
	total_size = sizeof(uint32) + sizeof(uint32) + s_data_ptr->service_num * struct_size;

    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(file_dev, total_size, &ret_dev))
    {
        return FALSE;
    }
	

	if(MMIAPIFMM_GetDevicePath(ret_dev) != PNULL)
	{
        MMIAPICOM_Wstrncpy(s_data_ptr->service_full_path, MMIAPIFMM_GetDevicePath(ret_dev), MMIAPIFMM_GetDevicePathLen(ret_dev));
	}
    
    file_handle = MMIAPIFMM_CreateFile(s_data_ptr->service_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIATV]: MMIATV_WriteServiceListFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_615_112_2_18_2_3_8_11,(uint8*)"");
		return FALSE;
	}
	
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &version, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    while(PNULL != cursor)
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_size, &write_len, NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        cursor = cursor->service_next_ptr;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : add service node
//	Global resource dependence :
//  Author: xin.li
//	Note: 添加频道节点
/*****************************************************************************/
PUBLIC void MMIATV_AddServiceNode(MMIATV_SERVICE_NODE_T *node_ptr)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    MMIATV_SERVICE_NODE_T* new_node_ptr = PNULL;
    
    if(!IsInitData()
		|| PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_AddServiceNode node_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_655_112_2_18_2_3_8_12,(uint8*)"");
        return;
    }
    cursor = s_data_ptr->service_head_ptr;
    new_node_ptr = (MMIATV_SERVICE_NODE_T*)SCI_ALLOC_APP(sizeof(MMIATV_SERVICE_NODE_T));
    if(PNULL != new_node_ptr)
    {
        SCI_MEMCPY(new_node_ptr, node_ptr, sizeof(MMIATV_SERVICE_NODE_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_AddServiceNode alloc memory error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_666_112_2_18_2_3_8_13,(uint8*)"");
        return;
    }
    
    if(PNULL == s_data_ptr->service_head_ptr)
    {
        s_data_ptr->service_head_ptr = new_node_ptr;
    }
    else
    {
        while(PNULL != cursor->service_next_ptr)
        {
            cursor = cursor->service_next_ptr;
        }
        cursor->service_next_ptr = new_node_ptr;
    }
    new_node_ptr->service_next_ptr = PNULL;
    s_data_ptr->service_num ++;
}

/*****************************************************************************/
// 	Description : clear service list
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearServiceList(void)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    
    if(IsInitData())
    {
        while(PNULL != s_data_ptr->service_head_ptr)
        {
            cursor = s_data_ptr->service_head_ptr;
            s_data_ptr->service_head_ptr = cursor->service_next_ptr;
            SCI_FREE(cursor);
        }
        s_data_ptr->service_num = 0;
    }
}

/*****************************************************************************/
// 	Description : get service list node number
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetServiceListNum(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->service_num;
	}
	return 0;
}

/*****************************************************************************/
// 	Description : get service node via index
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_SERVICE_NODE_T* MMIATV_GetServiceNodeViaIndex(uint32 index)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetServiceNodeViaIndex, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_722_112_2_18_2_3_8_14,(uint8*)"d", index);
    
    if(!IsInitData()
		|| PNULL == s_data_ptr->service_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetServiceNodeViaIndex PNULL==service_head_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_726_112_2_18_2_3_8_15,(uint8*)"");
        return PNULL;
    }
    cursor = s_data_ptr->service_head_ptr;
    while(index --)
    {
        if(PNULL != cursor)
        {
            cursor = cursor->service_next_ptr;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetServiceNodeViaIndex fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_738_112_2_18_2_3_8_16,(uint8*)"");
            return PNULL;
        }
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : read favorite service list file
//	Global resource dependence :
//  Author: xin.li
//	Note: 文件结构 |-version-|-num-|---------------service_nodes--------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_ReadFavoriteServiceListFile(void)
{
    uint32              read_size  = 0;
    uint32              list_num   = 0;
    //uint32              write_offset = 0;
    MMIFILE_HANDLE      file_handle = 0;
    uint32              version = 0;
    MMIFILE_DEVICE_E dev     = MMI_DEVICE_NUM -1;
    //BOOLEAN          file_ok = FALSE;
    
    if(!IsInitData()
		|| PNULL != s_data_ptr->favorite_service_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadFavoriteServiceListFile service_head_ptr not null! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_761_112_2_18_2_3_8_17,(uint8*)"");
        return FALSE;
    }

    //遍历所有DEV，查找文件是否存在
    for(dev=MMI_DEVICE_NUM -1; dev>=MMI_DEVICE_UDISK; dev--)
    {
        MMIAPICOM_Wstrncpy(s_data_ptr->fs_full_path,MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev));
        if(MMIAPIFMM_IsFileExist(s_data_ptr->fs_full_path, (uint16)MMIAPICOM_Wstrlen(s_data_ptr->fs_full_path)))
        {
            break;
        }
    } 
    if(dev < MMI_DEVICE_UDISK)
    {
        return FALSE;
    }
    
    file_handle = MMIAPIFMM_CreateFile(s_data_ptr->fs_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
	if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadFavoriteServiceListFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_782_112_2_18_2_3_8_18,(uint8*)"");
        return FALSE;
	}

    //读取版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &version, sizeof(uint32), &read_size, NULL))
    {
		//SCI_TRACE_LOW:"[MMIATV]: FavoriteServiceListFile error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_789_112_2_18_2_3_8_19,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //确认版本信息读取成功
    if(sizeof(uint32) != read_size)
    {
        //失败，文件为空
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //版本信息确认
    if(version != MMIATV_SAVE_FILE_VERSION)
    {
        //版本不匹配
		//SCI_TRACE_LOW:"[MMIATV]: FavoriteServiceListFile version error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_804_112_2_18_2_3_8_20,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        MMIAPIFMM_DeleteFile(s_data_ptr->fs_full_path, PNULL);
        return FALSE;
    }
    
    //获取节点个数
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, NULL))
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIAPIFMM_ReadFile error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_813_112_2_18_2_3_8_21,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_ReadFavoriteServiceListFile total list num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_817_112_2_18_2_3_8_22,(uint8*)"d", list_num);
    
    while (list_num --)
    {
        MMIATV_SERVICE_NODE_T node = {0};
        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &node, sizeof(MMIATV_SERVICE_NODE_T), &read_size, NULL))
        {
            MMIATV_AddFavoriteServiceNode(&node);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIATV]: MMIAPIFMM_ReadFile error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_828_112_2_18_2_3_8_23,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : write/save favorite service list file
//	Global resource dependence :
//  Author: xin.li
//	Note: 文件结构 |-version-|-num-|---------------service_nodes--------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_WriteFavoriteServiceListFile(void)
{
    uint32                  list_num = 0;
    uint32                  write_len = 0;
    MMIFILE_HANDLE          file_handle = 0;
    MMIATV_SERVICE_NODE_T*  cursor = PNULL;
    uint32                  struct_size = sizeof(MMIATV_SERVICE_NODE_T);
    uint32                  total_size = 0;
    uint32                  version = MMIATV_SAVE_FILE_VERSION;
    MMIFILE_DEVICE_E        file_dev = MMI_DEVICE_NUM -1;
    MMIFILE_DEVICE_E        ret_dev  = MMI_DEVICE_NUM -1;
    
    //file_dev = MMIAPIFMM_GetDeviceTypeByPath(MMIATV_DATA_PTR->fs_full_path, MMIFILE_DEVICE_LEN);
    if (!IsInitData())
    {
		SCI_TRACE_LOW("[MMIATV] MMIATV_WriteFavoriteServiceListFile not init data");
		return FALSE;
    }
	list_num = s_data_ptr->favorite_service_num;
	cursor = s_data_ptr->favorite_service_head_ptr;
	total_size = sizeof(uint32) + sizeof(uint32) + s_data_ptr->favorite_service_num * struct_size;

    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(file_dev, total_size, &ret_dev))
    {
        return FALSE;
    }
	if(MMIAPIFMM_GetDevicePath(ret_dev) != PNULL)
	{
		MMIAPICOM_Wstrncpy(s_data_ptr->fs_full_path, MMIAPIFMM_GetDevicePath(ret_dev), MMIAPIFMM_GetDevicePathLen(ret_dev));
	}
    
    file_handle = MMIAPIFMM_CreateFile(s_data_ptr->fs_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
		//SCI_TRACE_LOW:"[MMIATV]: MMIATV_WriteFavoriteServiceListFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_867_112_2_18_2_3_8_24,(uint8*)"");
		return FALSE;
	}
	
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &version, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    while(PNULL != cursor)
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_size, &write_len, NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        cursor = cursor->service_next_ptr;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : add favorite service node
//	Global resource dependence :
//  Author: xin.li
//	Note: 添加收藏频道节点
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_AddFavoriteServiceNode(MMIATV_SERVICE_NODE_T *node_ptr)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    MMIATV_SERVICE_NODE_T* new_node_ptr = PNULL;
    BOOLEAN is_repeat = FALSE;
    if(!IsInitData()
		|| PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_AddFavoriteServiceNode node_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_907_112_2_18_2_3_8_25,(uint8*)"");
        return FALSE;
    }
    
	cursor = s_data_ptr->favorite_service_head_ptr;
    new_node_ptr = (MMIATV_SERVICE_NODE_T*)SCI_ALLOC_APP(sizeof(MMIATV_SERVICE_NODE_T));
    if(PNULL != new_node_ptr)
    {
        SCI_MEMCPY(new_node_ptr, node_ptr, sizeof(MMIATV_SERVICE_NODE_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_AddFavoriteServiceNode alloc memory error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_918_112_2_18_2_3_8_26,(uint8*)"");
        return FALSE;
    }
    
	if(PNULL == s_data_ptr->favorite_service_head_ptr)
	{
		s_data_ptr->favorite_service_head_ptr = new_node_ptr;
		new_node_ptr->service_next_ptr = PNULL;
		s_data_ptr->favorite_service_num ++;
		is_repeat = TRUE;
    }
    else
   	{
		while(PNULL != cursor->service_next_ptr)
        {
			if((new_node_ptr->service_id != cursor->service_id) && (MMIAPICOM_Wstrcmp(new_node_ptr->service_name,cursor->service_name) != 0))

			{
				cursor = cursor->service_next_ptr;
			}
			else 
			{
				return FALSE;
			}
			
        }
		if((new_node_ptr->service_id != cursor->service_id) && (MMIAPICOM_Wstrcmp(new_node_ptr->service_name,cursor->service_name) != 0))
		{
			cursor->service_next_ptr = new_node_ptr;
			new_node_ptr->service_next_ptr = PNULL;
			s_data_ptr->favorite_service_num ++;
			is_repeat = TRUE;
		}
		
    }
	return is_repeat;
	
}

/*****************************************************************************/
// 	Description : delete favorite service node
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_DeleteFavoriteServiceNode(MMIATV_SERVICE_NODE_T *node_ptr)
{
    BOOLEAN is_node_find = TRUE;

    if(!IsInitData()
		|| PNULL == node_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_DeleteFavoriteServiceNode node_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_948_112_2_18_2_3_8_27,(uint8*)"");
        return;
    }
    
    if(PNULL == s_data_ptr->favorite_service_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_DeleteFavoriteServiceNode list is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_954_112_2_18_2_3_8_28,(uint8*)"");
        return;
    }
    
    if(node_ptr == s_data_ptr->favorite_service_head_ptr)
    {
        s_data_ptr->favorite_service_head_ptr = node_ptr->service_next_ptr;
        SCI_FREE(node_ptr);
        if(s_data_ptr->favorite_service_num > 0)
        {   
            s_data_ptr->favorite_service_num --;
        }
    }
    else
    {
        MMIATV_SERVICE_NODE_T* temp = s_data_ptr->favorite_service_head_ptr;
        while(node_ptr != temp->service_next_ptr)
        {
            if(PNULL != temp->service_next_ptr)
            {
                temp = temp->service_next_ptr;
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIATV]: MMIATV_DeleteFavoriteServiceNode find node error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_978_112_2_18_2_3_8_29,(uint8*)"");
                is_node_find = FALSE;
                break;
            }
        }
        
        if(is_node_find)
        {
            //find success
            temp->service_next_ptr = node_ptr->service_next_ptr;
            SCI_FREE(node_ptr);
            if(s_data_ptr->favorite_service_num > 0)
            {
                s_data_ptr->favorite_service_num --;
            }
        }
    }
}

/*****************************************************************************/
// 	Description : clear favorite service list
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_ClearFavoriteServiceList(void)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    
    if(IsInitData())
    {
        while(PNULL != s_data_ptr->favorite_service_head_ptr)
        {
            cursor = s_data_ptr->favorite_service_head_ptr;
            s_data_ptr->favorite_service_head_ptr = cursor->service_next_ptr;
            SCI_FREE(cursor);
        }
        s_data_ptr->favorite_service_num = 0;
    }
}

/*****************************************************************************/
// 	Description : get favorite service list number
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIATV_GetFavoriteServiceListNum(void)
{
	if(IsInitData())
    {
		return s_data_ptr->favorite_service_num;
	}
	return 0;
}

/*****************************************************************************/
// 	Description : get favorite service node via index
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIATV_SERVICE_NODE_T* MMIATV_GetFavoriteServiceNodeViaIndex(uint32 index)
{
    MMIATV_SERVICE_NODE_T* cursor = PNULL;
    //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_1033_112_2_18_2_3_8_30,(uint8*)"d", index);
    
    if( !IsInitData()
		|| PNULL == s_data_ptr->favorite_service_head_ptr)
    {
        //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex PNULL==service_head_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_1037_112_2_18_2_3_8_31,(uint8*)"");
        return PNULL;
    }
    
	cursor = s_data_ptr->favorite_service_head_ptr;

    while(index --)
    {
        if(PNULL != cursor)
        {
            cursor = cursor->service_next_ptr;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIATV]: MMIATV_GetFavoriteServiceNodeViaIndex fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIATV_DATA_1049_112_2_18_2_3_8_32,(uint8*)"");
            return PNULL;
        }
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : get current video name
//	Global resource dependence : none
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIATV_GetVideoName(void)
{
	if (IsInitData())
	{	
		return s_data_ptr->record_name;
	}
	return PNULL;
}

/*****************************************************************************/
//  Description : 指定录像文件名
//  Global resource dependence :                                
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIATV_CreateRecordFileName(DRECORDER_START_RECORD_T* recorder_start_t) // [OUT]
{
    BOOLEAN         return_val = TRUE;
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    uint16          full_path_len = MMIATV_FULL_PATH_LEN;
    char            temp_photo_name[MMIATV_FULL_PATH_LEN] = {0};
    uint32                circle = 1;
    register uint32       name_id = 0;
    register uint32       step = 1;
    register uint32       id_max = MMIATV_RECORD_FILE_ID_MAX;
    MMIFILE_DEVICE_E    file_dev = MMIATV_GetRecordDevice();
    MMIFILE_DEVICE_E    record_file_dev = MMI_DEVICE_SDCARD;
    char* suffix_ptr = MMIATV_VIDEO_FILE_SUFFIX_3GP;

    if(!IsInitData()
		|| PNULL == recorder_start_t)
    {
        return FALSE;
    }

    if(MMIATV_VIDEO_TYPE_AVI == MMIATV_GetVideoFileType())
    {
        suffix_ptr = MMIATV_VIDEO_FILE_SUFFIX_AVI;
    }
    else
    {
        suffix_ptr = MMIATV_VIDEO_FILE_SUFFIX_3GP;
    }
    
    if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_PREFER, file_dev, MMIATV_RECORD_MIN_SPACE_SIZE, &record_file_dev))
    {
        file_dev_ptr = MMIAPIFMM_GetDevicePath(record_file_dev);
        file_dev_len = MMIAPIFMM_GetDevicePathLen(record_file_dev);
        
        if((PNULL == file_dev_ptr) || (0 == file_dev_len))
		{
			return FALSE;
		}
            
        name_id = MMIATV_GetRecordID();
        //生成文件名的规则，先取最后一次记录的ID值，再此基础上加1作为新的录制ID。
        //如果该文件名已存在则生成ID所加数目按指数增长，目的是迅速找到一个合适的文件名。
        //如果ID按指数查找方式找到最大值还是重名，则回转加1后，继续按指数方式查找。
        while(1)  /*lint !e716*/
        { 
            name_id += step;
            step = step * 2;
            if(name_id >= id_max)
            {
                circle ++;
                if(circle >= id_max)
                {
                    return FALSE;
                }
                name_id = circle;
                step = 1;
            }
            //s_record_setting_info.record_file_id = name_id;
            MMIAPIDC_GetFileIDString(temp_photo_name, "ATV", name_id, id_max, suffix_ptr);
            MMIAPICOM_StrToWstr(temp_photo_name, s_data_ptr->record_name);/*lint !e64*/
            
            MMIAPIFMM_CombineFullPath(file_dev_ptr, 
                file_dev_len, 
                MMIMULTIM_DIR_MOVIE, 
                MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE),
                s_data_ptr->record_name, 
                (uint16)MMIAPICOM_Wstrlen(s_data_ptr->record_name), 
                s_data_ptr->full_path_record_name, &full_path_len);
            
            // check the exist of the file name.
            if(!MMIAPIFMM_IsFileExist(s_data_ptr->full_path_record_name, full_path_len))
            {
                break;
            }
        }
        recorder_start_t->file_name_ptr = (uint8*)s_data_ptr->full_path_record_name;
        recorder_start_t->file_name_byte_len = full_path_len * sizeof(wchar);
        MMIATV_SetRecordID(name_id);
    }
    else
    {
        return_val = FALSE;
    }

    return return_val;
}


/*****************************************************************************/
//  Description : 是否初始化data
//  Global resource dependence :                                
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsInitData(void)
{
	if (PNULL == s_data_ptr)
	{
		return FALSE;
	}
	return TRUE;
}
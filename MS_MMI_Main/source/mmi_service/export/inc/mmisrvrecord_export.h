/*****************************************************************************
** File Name:      mmirecord_srv.h                                           *
** Author:                                                                   *
** Date:           2012/6/18                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/6/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIRECORD_SRV_H_
#define _MMIRECORD_SRV_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrv.h"
#include "mmi_filemgr.h"
#include "mmisrvaud_api.h"
#include "adpcmrecord_adp.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIRECORD_SRV_MAX_FILE_NAME_LEN          256
#define MMIRECORD_SRV_DEFAULT_SUFFIX_LEN    6    
#define MMIRECORD_SRV_MIN_NEED_SPACE        20000//2000    //Â¼Òô×îÐ¡ÐèÒª¿Õ¼ä
#define MMIRECORD_SRV_STRING_MAX_NUM  60 
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef uint32 RECORD_SRV_HANDLE;

typedef enum
{
	MMIRECORD_SRV_SUPPORT_NONE,
	MMIRECORD_SRV_SUPPORT_RECORD,
	MMIRECORD_SRV_SUPPORT_IQ_RECORD,
	MMIRECORD_SRV_SUPPORT_PLAY,
	MMIRECORD_SRV_SUPPORT_MAX
}MMIRECORD_SRV_SUPPORT_TYPE_E;

typedef enum
{
	MMIRECORD_SRV_RESULT_SUCCESS,
	MMIRECORD_SRV_RESULT_ERROR,
	MMIRECORD_SRV_RESULT_NO_SPACE,
	MMIRECORD_SRV_RESULT_NO_EXIST,
	MMIRECORD_SRV_RESULT_MAX_FILE,
	MMIRECORD_SRV_RESULT_FOLDER_FULL,
	MMIRECORD_SRV_RESULT_PARAM_ERROR,
	MMIRECORD_SRV_RESULT_MAX
}MMIRECORD_SRV_RESULT_E;



typedef struct  
{
	MMISRVAUD_RECORD_FMT_E fmt;
	MMISRVAUD_RECORD_SOURCE_E source;
	BOOLEAN  is_create_file_name;
	MMI_STRING_T *record_name_ptr;
	char* prefix_ptr;
	uint32 * record_file_id_ptr;	
	MMIFILE_DEVICE_E *record_dev_ptr;
    RECORD_PARAM_T *record_param_ptr;
	MMISRVAUD_VOLUME_T volume;
}MMIRECORD_SRV_RECORD_PARAM_T;

typedef void (*RECORD_SRV_NOTIRY_CALLBACK)(RECORD_SRV_HANDLE record_srv_handle, MMIRECORD_SRV_SUPPORT_TYPE_E support_type, MMIRECORD_SRV_RESULT_E result);       

/*****************************************************************************/
// 	Description : get record file name
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORDSRV_GetRecordFileName(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
// 	Description : get record file path
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIRECORDSRV_GetRecordFilePath(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : ÓÃ»§²»±£´æÂ¼Òô£¬É¾³ýÁÙÊ±ÎÄ¼þ
//  Global resource dependence :                                
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_DeleteRecordFile(RECORD_SRV_HANDLE record_srv_handle,
															BOOLEAN  is_clear_info);


/*****************************************************************************/
//  Description : ¿ªÊ¼Â¼Òô
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_StartRecord(RECORD_SRV_HANDLE record_srv_handle,
												MMIRECORD_SRV_RECORD_PARAM_T record_param);


/*****************************************************************************/
//  Description : ·ÖÅäÂ¼ÒôHandle
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC RECORD_SRV_HANDLE MMIRECORDSRV_RequestRecordHandle(RECORD_SRV_NOTIRY_CALLBACK notifycallback);

/*****************************************************************************/
//  Description : ÊÍ·ÅÂ¼ÒôHandle
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORDSRV_FreeRecordHandle(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : play current record
//  Global resource dependence :
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_Play(RECORD_SRV_HANDLE record_srv_handle,
												MMISRVAUD_VOLUME_T volume);

/*****************************************************************************/
//  Description : MMIRECORD_Seek
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_Seek(RECORD_SRV_HANDLE record_srv_handle,
								  uint32  seek_offset);

/*****************************************************************************/
//  Description : Í£Ö¹²¥·ÅÂ¼Òô
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_StopPlay(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : ¼ÌÐø²¥·ÅÂ¼Òô
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_ResumePlay(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : ÔÝÍ£²¥·ÅÂ¼Òô
//  Global resource dependence :                                
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIRECORDSRV_PausePlay(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : ¼ÌÐøÂ¼Òô
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_ResumeRecord(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : ÔÝÍ£Â¼Òô
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_PauseRecord(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description :stop record
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMIRECORDSRV_StopRecord(RECORD_SRV_HANDLE record_srv_handle);


/*****************************************************************************/
//  Description : get playing info
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetPlayingInfo(RECORD_SRV_HANDLE record_srv_handle,  
										MMISRVAUD_PLAY_INFO_T *playing_info_ptr);

/*****************************************************************************/
//  Description : get record info
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetRecordInfo(RECORD_SRV_HANDLE record_srv_handle,
									   MMISRVAUD_RECORD_INFO_T *record_info_ptr);

/*****************************************************************************/
//  Description : get record size
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetRecordSize(RECORD_SRV_HANDLE record_srv_handle,
									   MMISRVAUD_RECORD_SIZE_T *record_info_ptr);

/*****************************************************************************/
//  Description : to whether there is recoding  
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_IsExistSRVRecord(void);

/*****************************************************************************/
//  Description : MMIRECORD_SetVolume
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_SetVolume(RECORD_SRV_HANDLE record_srv_handle, uint32 cur_vol_value);

/*****************************************************************************/
//  Description : MMIRECORD_GetContentInfo
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_GetContentInfo(RECORD_SRV_HANDLE record_srv_handle,
										MMISRVAUD_CONTENT_INFO_T *info_ptr);

/*****************************************************************************/
//  Description : MMIRECORDSRV_GetRecordDB
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIRECORDSRV_GetRecordDB(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : record file is exist
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_IsRecordFileExist(RECORD_SRV_HANDLE record_srv_handle);

/*****************************************************************************/
//  Description : MMIRECORDSRV_QueryEncoderCapability
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIRECORDSRV_QueryEncoderCapability(MMISRVAUD_RECORD_FMT_E fmt);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

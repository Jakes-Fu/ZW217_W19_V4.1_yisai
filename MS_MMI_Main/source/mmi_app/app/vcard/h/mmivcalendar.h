/*****************************************************************************
** File Name:      mmivcalendar.h                                                   *
** Author:                                                                   *
** Date:           02/03/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi vcard               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        renyi hu          Create
******************************************************************************/

#ifndef _MMIVCALENDAR_H_
#define _MMIVCALENDAR_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "vf_iface.h"
#include "mmivc_common.h"
#include "mmiacc_event.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMIVCAL_VERSION_MAX_LEN  20
#define MMIVCAL_MAX_FILE_LEN   (3*1024)
#define MMIVCAL_SUMMARY_MAX_LEN  512
#define MMIVCAL_SEMICOLON   ";"
#define MMIVCAL_COLON       ":"
#define MMIVCAL_LFCR       	"\r\n"
#define MMIVCAL_MIN_NEED_SPACE        2048    //保存VCAL最小需要空间
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum MMIVCAL_PROPERTY_TYPE_E 
{
    MMIVCAL_PROPERTY_INVALID,
    MMIVCAL_PROPERTY_VERSION,
    MMIVCAL_PROPERTY_OBJECT_TYPE,//vevent or vtodo
    MMIVCAL_PROPERTY_CATEGORIES,
    MMIVCAL_PROPERTY_STATUS,
    MMIVCAL_PROPERTY_SUMMARY,
    MMIVCAL_PROPERTY_DESCRIPTION,
    MMIVCAL_PROPERTY_CLASS,
    MMIVCAL_PROPERTY_DTSTART,
    MMIVCAL_PROPERTY_DTEND,
    MMIVCAL_PROPERTY_COMPLETED,
    MMIVCAL_PROPERTY_DUE,
    MMIVCAL_PROPERTY_RRULE,
    MMIVCAL_PROPERTY_AALARM,
    MMIVCAL_PROPERTY_DALARM,
    MMIVCAL_PROPERTY_MAX
}MMIVCAL_PROPERTY_TYPE_E;

 typedef enum MMIVCAL_OBJECT_TYPE_E 
{
	MMIVCAL_OBJECT_TYPE_INVALID,
    MMIVCAL_OBJECT_TYPE_VEVENT,
	MMIVCAL_OBJECT_TYPE_VTODO,
    MMIVCAL_OBJECT_TYPE_MAX
}MMIVCAL_OBJECT_TYPE_E;

typedef enum MMIVCAL_ERROR_TYPE_E 
{
	MMIVCAL_NO_ERROR,
	MMIVCAL_INVALID_DATA_PTR,
	MMIVCAL_NO_SUPPORT,
	MMIVCAL_INVALID_DATETIME,
	MMIVCAL_PARSE_FAIL,
	MMIVCAL_IS_EMPTY,
	MMIVCAL_IS_TOOBIG,
	MMIVCAL_IS_NESTED,
	MMIVCAL_SCH_IS_FULL,
	MMIVCAL_FILE_ERROR,
	MMIVCAL_PARAM_ERROR
}MMIVCAL_ERROR_TYPE_E;

//PRIORITY 
typedef enum  
{
    MMIVCAL_PRIO_HIGH,//0为最高优先级
    MMIVCAL_PRIO_NORM,
    MMIVCAL_PRIO_LOW,
    MMIVCAL_PRIO_MAX
}MMIVCAL_PRIO_TYPE_E;

//CATEGORIES 
typedef enum  
{
    MMIVCAL_CATE_APPOINTMENT,//APPOINTMENT
    MMIVCAL_CATE_BUSINESS,//BUSINESS
    MMIVCAL_CATE_EDUCATION,//EDUCATION
    MMIVCAL_CATE_HOLIDAY,//HOLIDAY
    MMIVCAL_CATE_MEETING,//MEETING
    MMIVCAL_CATE_MISCELLANEOUS,//MISCELLANEOUS
    MMIVCAL_CATE_PERSONAL,//PERSONAL
    MMIVCAL_CATE_PHONE_CALL,//PHONE CALL
    MMIVCAL_CATE_SICK_DAY,//SICK DAY
    MMIVCAL_CATE_SPECIAL_OCCASION,//SPECIAL OCCASION
    MMIVCAL_CATE_TRACEL,//TRACEL
    MMIVCAL_CATE_VACATION,//VACATION
    MMIVCAL_CATE_MAX
}MMIVCAL_CATE_TYPE_E;

//STATUS 
typedef enum  
{
    MMIVCAL_STATUS_ACCEPTED,
    MMIVCAL_STATUS_NEEDS_ACTION,//默认为need action
    MMIVCAL_STATUS_SENT,
    MMIVCAL_STATUS_TENTATIVE,
    MMIVCAL_STATUS_CONFIRMED,
    MMIVCAL_STATUS_DECLINED,
    MMIVCAL_STATUS_COMPLETED,
    MMIVCAL_STATUS_DELEGATED,
    MMIVCAL_STATUS_MAX
}MMIVCAL_STATUS_TYPE_E;

typedef struct MMIVCAL_FIELDINFO_T
{
    MMIVCAL_PROPERTY_TYPE_E    prop_type;
    char                 *prop_name;
}MMIVCAL_FIELDINFO_T;

typedef struct _MMIVCAL_VERSION_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_VERSION_T;

typedef struct _MMIVCAL_CATEGORIES_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_CATEGORIES_T;

typedef struct _MMIVCAL_STATUS_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_STATUS_T;

typedef struct _MMIVCAL_SUMMARY_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_SUMMARY_T;

typedef struct _MMIVCAL_DESCRIPTION_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_DESCRIPTION_T;

typedef struct _MMIVCAL_CLASS_T
{
    uint16   wstr_len;                   
	wchar*   wstr;   
} MMIVCAL_CLASS_T;

typedef struct _MMIVCAL_DTSTART_T
{
    MMIVC_DATE_TIME_T date_time;  
} MMIVCAL_DTSTART_T;

typedef struct _MMIVCAL_DTEND_T
{
    MMIVC_DATE_TIME_T date_time;  
} MMIVCAL_DTEND_T;

typedef struct _MMIVCAL_COMPLETED_T
{
    MMIVC_DATE_TIME_T date_time;  
} MMIVCAL_COMPLETED_T;

typedef struct _MMIVCAL_DUE_T
{
    MMIVC_DATE_TIME_T date_time;  
} MMIVCAL_DUE_T;

typedef struct _MMIVCAL_RRULE_T
{
   	char* rrule;  
} MMIVCAL_RRULE_T;

typedef struct _MMIVCAL_AALARM_T
{
   	MMIVC_DATE_TIME_T run_date_time; 
	uint32 persist_time;
	uint32 repeat_count;
	char* audio_content;
} MMIVCAL_AALARM_T;

typedef struct _MMIVCAL_DALARM_T
{
   	MMIVC_DATE_TIME_T run_date_time; 
	uint32 persist_time;
	uint32 repeat_count;
	char* dis_string;
} MMIVCAL_DALARM_T;

typedef struct _MMIVCALENDAR_INFO_T
{
    MMIVCAL_VERSION_T version;
	MMIVCAL_CATEGORIES_T categories;
	MMIVCAL_OBJECT_TYPE_E object_type;
	MMIVCAL_STATUS_T status;
	MMIVCAL_SUMMARY_T summary;
	MMIVCAL_DESCRIPTION_T description;
	MMIVCAL_CLASS_T classification;
	MMIVCAL_DTSTART_T dtstart;
	MMIVCAL_DTEND_T dtend;
	MMIVCAL_COMPLETED_T completed;
	MMIVCAL_DUE_T due;
	MMIVCAL_RRULE_T repeat_rule;
	MMIVCAL_AALARM_T aalarm;
	MMIVCAL_DALARM_T dalarm;

}MMIVCALENDAR_INFO_T;

#ifdef MMI_VCALENDAR_SUPPORT
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// Description: init vcalendar module
// Global resource dependence:
// Auther: renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCAL_Init(void);

/****************************************************************************/
//  Description: 解析VCALENDAR数据
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_parse_by_data(VF_OBJECT_T **pp_object, uint8 *data_ptr, uint32 data_size);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC void MMIVCAL_ObjectToVcalInfo(VF_OBJECT_T              *pObject,
                                MMIVCALENDAR_INFO_T *vcal_info);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC void MMIVCAL_FreeVcalInfo(MMIVCALENDAR_INFO_T *vcal_info);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_VcalInfoToSchedule(MMIVCALENDAR_INFO_T *vcal_info_ptr,
										MMIACC_SMART_EVENT_T *event_info_ptr);


/****************************************************************************/
//  Description: 解析VCALENDAR数据
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_VcalendarDataToSch(uint8 *data_ptr, uint32 data_size,MMIACC_SMART_EVENT_T *event_info_ptr);

/****************************************************************************/
//  Description: 解析VCALENDAR数据
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_AddSchEvent(uint8 *data_ptr, uint32 data_size,uint16 *index_ptr);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_BuildVcalendarByIndex(uint16 valid_index,
												MMIVCAL_OBJECT_TYPE_E type,
												char* buffer,
												uint32 buffer_size,
												uint32 *len);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_GetNextVcalItem(MMIVCAL_OBJECT_TYPE_E type,
									   int32             lastItemId,
									   int32             *uuid_ptr, 
									   uint8              *item_ptr,
									   uint32             maxItemLen
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_AddVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             *uuid_ptr 
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_ModifyVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             uuid
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_RemoveVcalItem(int32  uuid);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
MMIVCAL_ERROR_TYPE_E MMIVCAL_ReadFile(VF_OBJECT_T **pp_object,
                        uint16       *name_ptr);
/*****************************************************************************/
// Description : 保存vCalendar信息，并写入文件系统
// Global resource dependence : s_vcal_handle_result
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIVCAL_SaveVcalendarFile(MMIFILE_DEVICE_E fs_dev, 
											uint8* vCard_data_ptr,
											wchar* vCard_name_ptr,
											SFS_COMPLETETION_ROUTINE callback,
											wchar* new_file_name_ptr);

/*****************************************************************************/
// Description : 保存vCalendar信息，并写入指定文件夹
// Global resource dependence : g_vcard_handle_result
// Author:Jesse.Yu
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIVCAL_SaveVcalendarFileToAssignFolder(const wchar * folder_name_ptr,
													 	//uint8* vCard_data_ptr,
														//wchar* vCard_name_ptr,
														//SFS_COMPLETETION_ROUTINE callback,
														wchar * new_file_name_ptr);

/*****************************************************************************/
// Description : 同步保存Vcal信息，并写入文件系统
// Global resource dependence : s_vcal_handle_result
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIVCAL_SaveVcalFileSync(MMIFILE_DEVICE_E fs_dev,
												//uint8* vCard_data_ptr,
												//wchar* vCard_name_ptr,
												wchar* new_file_name_ptr);

/****************************************************************************/
//  Description: Read vCal file to global variable
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_GetDataByFile(uint16    *name_ptr);

/*****************************************************************************/
// Description : Init vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_VcalBuildInit(void);

/*****************************************************************************/
// Description : Free vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_VcalBuildExit(void);

/*****************************************************************************/
// Description : Free vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_FreeFileNamePtr(void);

/****************************************************************************/
//  Description:vcalendar是否是空的
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_VcalendarIsEmpty(VF_OBJECT_T    *pObject);

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_SaveVCalendarByData(
                                MMI_WIN_ID_T      win_id,
                                MMI_MESSAGE_ID_E  msg_id,
                                uint8            *data_ptr,
                                uint32            data_len);

/*****************************************************************************/
// Description : 获得vCal文件名称
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC wchar* MMIVCAL_GetVcalendarFileName( MMIACC_SMART_EVENT_T*  event_ptr);

/*****************************************************************************/
//  Description : 保存联系人生成的vCalendar文件的回调函数
//  Global resource dependence : 
//  Author:    Jenny.Sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_CallBackSaveVcalendarFile(SFS_ERROR_E  error, 
                                       uint32       param,
                                       uint32       result,
                                       uint32       param2);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_BuildVcalendarByEvent(MMIACC_SMART_EVENT_T* event_ptr,
												MMIVCAL_OBJECT_TYPE_E type);

/*****************************************************************************/
// Description : Init vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_InitFileNamePtr(void);

/*****************************************************************************/
// Description : Get vcalendar data ptr
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint8 * MMIVCAL_GetBuildData(void);

/*****************************************************************************/
// Description : 处理保存文件的操作
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_HandleWriteCNF(MMI_MESSAGE_ID_E  msg_id,
                                 DPARAM            param);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#else
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_GetNextVcalItem(MMIVCAL_OBJECT_TYPE_E type,
									   int32             lastItemId,
									   int32             *uuid_ptr, 
									   uint8              *item_ptr,
									   uint32             maxItemLen
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_AddVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             *uuid_ptr 
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_ModifyVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             uuid
									   );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_RemoveVcalItem(int32  uuid);
#endif
#ifdef   __cplusplus
    }
#endif

#endif

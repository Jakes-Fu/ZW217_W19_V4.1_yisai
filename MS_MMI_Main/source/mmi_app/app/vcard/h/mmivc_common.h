/*****************************************************************************
** File Name:      mmivcalendar.c                                                   *
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
#ifndef _MMIVC_COMMON_H_
#define _MMIVC_COMMON_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "vf_iface.h"
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
#define MMIVC_TOKEN_YEARS         'Y'
#define MMIVC_TOKEN_MONTHS        'M'
#define MMIVC_TOKEN_WEEKS         'W'
#define MMIVC_TOKEN_DAYS          'D'
#define MMIVC_TOKEN_HOURS         'H'
#define MMIVC_TOKEN_MINUTES       'M'
#define MMIVC_TOKEN_SECONDS       'S'
#define MMIVC_TOKEN_TIME          'T'
#define MMIVC_TOKEN_PERIOD        'P'

#define MMIVC_MAX_LONG_SMS_NUM     30
//#define VCARD_MINUTE_TIME          60
#define MMIVC_MAX_SMS_DATA_LENGTH  (2*1024)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum VF_CHARSET_E 
{
    VF_CHARSET_NONE,
    VF_CHARSET_UTF8,
    VF_CHARSET_GB2312
}VF_CHARSET_E;

typedef struct _MMIVC_DATE_TIME_T_
{
	uint8   sec;        // secondsafter the minute - [0,59] 
	uint8   min;        // minutesafter the hour - [0,59] 
	uint8   hour;       // hours since midnight - [0,23] 
	uint8   mday;       // day of the month - [1,31] 
	uint8   mon;        // months  - [1,12] 
	uint16  year;       // years [2000,2049] 
	uint8   wday;       // days since Sunday - [0,6], (Sunday : 0, Monday : 1)
}MMIVC_DATE_TIME_T;

typedef enum MMIVC_SMS_SEND_TYPE_E 
{
    MMIVC_SMS_SEND_NORMAL,
    MMIVC_SMS_SEND_VCARD,
    MMIVC_SMS_SEND_VCALENDAR,
    MMIVC_SMS_SEND_MAX
}MMIVC_SMS_SEND_TYPE_E;

typedef struct
{
    uint32     recv_time;
    uint16     ref_num;
    uint8      max_num;
    uint8      seq_num;
    uint8      length;
    BOOLEAN is_valid;
    uint8      data_arr[MN_SMS_MAX_USER_VALID_DATA_LENGTH];
}VCARD_SMS_T;

typedef struct
{
    VCARD_SMS_T    vcard_sms[MMIVC_MAX_LONG_SMS_NUM];
}VCARD_LONG_SMS_T;

typedef struct
{
    uint16 length;
    uint8  data[MMIVC_MAX_SMS_DATA_LENGTH];
} VCARD_LONG_SMS_DATA_T;

typedef enum VCARD_FS_RESULT_E 
{
    VCARD_FS_WRITE_SUCCEED,
    VCARD_FS_WRITE_FAIL
}VCARD_FS_RESULT_E;

typedef struct
{
    SFS_HANDLE          handle;
    MMI_WIN_ID_T        win_id;
    MMI_MESSAGE_ID_E    msg_id;
    VCARD_FS_RESULT_E   result;
    uint32              buf_len;
    uint8              *buf_ptr;
    BOOLEAN             isWaiting;
    uint16             *name_ptr;
}VCARD_FILE_CALLBACK_T;
#if 1//BT_DIALER_SUPPORT
//contact name
typedef struct
{
    uint8   alpha_len;                   // alpha数组的BYTE个数
	uint8   alpha[MMIPB_NAME_MAX_LEN  + 1];   // 具体意义请查看GSM 11.11
}MMIVF_INTERNAL_NAME_T;

//定义vcard记录姓名各个组成部分的属性
typedef struct
{
	MMIVF_INTERNAL_NAME_T   family_name;    //姓
    MMIVF_INTERNAL_NAME_T   given_name;     //名
    MMIVF_INTERNAL_NAME_T   middle_name;    //中间名
    MMIVF_INTERNAL_NAME_T   prefix;         //名字前缀
    MMIVF_INTERNAL_NAME_T   suffix;         //名字后缀 
}MMIVF_NAME_T;
//定义vcard记录姓名各个组成部分的属性枚举
typedef enum
{
    VC_NAME_PROPERTY_TYPE_FAMILY = 0,
    VC_NAME_PROPERTY_TYPE_GIVEN,
    VC_NAME_PROPERTY_TYPE_MIDDLE,
    VC_NAME_PROPERTY_TYPE_PREFIX,
    VC_NAME_PROPERTY_TYPE_SUFFIX,
    VC_NAME_PROPERTY_TYPE_MAX
}VC_NAME_PROPERTY_TYPE_E;

typedef enum _VF_TIMESTAMP_OFFSET_E 
{
    VF_TIMESTAMP_YEAR_OFFSET = 0,
    VF_TIMESTAMP_MON_OFFSET  = 4,
    VF_TIMESTAMP_DAY_OFFSET  = 6,
    VF_TIMESTAMP_T_OFFSET    = 8,
    VF_TIMESTAMP_HOUR_OFFSET = 9,
    VF_TIMESTAMP_MIN_OFFSET  = 11,
    VF_TIMESTAMP_SEC_OFFSET  = 13,
    VF_TIMESTAMP_MAX_OFFSET  = 15
}VF_TIMESTAMP_OFFSET_E;

typedef enum _VF_TIMESTAMP_FIELD_TYPE_E 
{
    VF_TIMESTAMP_YEAR_FIELD = 0,
    VF_TIMESTAMP_MON_FIELD,
    VF_TIMESTAMP_DAY_FIELD,
    VF_TIMESTAMP_T_FIELD,
    VF_TIMESTAMP_HOUR_FIELD,
    VF_TIMESTAMP_MIN_FIELD,
    VF_TIMESTAMP_SEC_FIELD,
    VF_TIMESTAMP_MAX_FIELD
}VF_TIMESTAMP_FIELD_TYPE_E;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void *MMIVC_Malloc(uint32_t s);

/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void *MMIVC_Realloc(void *p, uint32_t ns);

/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_Free(void *p);
//#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// Description : 获取Name属性值
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_GetNamePropVaule(
    VF_PROP_T   *p_prop,
    uint32  nValue,
    MMI_STRING_T    *p_value);
//#endif
/*****************************************************************************/
// Description : 获取属性值
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_GetPropValue(VF_PROP_T     *p_prop,
                           uint32         nValue,
                           MMI_STRING_T  *p_value);

/*****************************************************************************/
// Description : 将时间字符串转换为日期时间结构
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVC_StringToDateTime( char* string,MMIVC_DATE_TIME_T *date_time);

/*****************************************************************************/
// Description : free MMI_STRING_T
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_freeData( MMI_STRING_T  *p_data);

/****************************************************************************/
//  Description:MMIVC_GetFirstValidLetterPos
//  Global resource dependence
//  Auther:
//  Note
/****************************************************************************/
PUBLIC uint16 MMIVC_GetFirstValidLetterPos(wchar *name);

/*****************************************************************************/
//  Description : 保存文件的回调函数
//  Global resource dependence : 
//  Author:Bill.Ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVC_WriteFile_CallBack(SFS_ERROR_E  error, 
                                     uint32       param,
                                     uint32       result,
                                     uint32       param2);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



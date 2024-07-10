/*****************************************************************************
** File Name:      mmivcalendar.c                                                   *
** Author:                                                                   *
** Date:           02/03/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi vcalendar           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        renyi hu          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_vcard_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
#include "vf_iface.h"
#include "mmivcalendar.h"
#include "mmisd_export.h"
#include "mmi_common.h"
#include "vf_strings.h"
#include "vf_malloc.h"
#include "gui_ucs2b_converter.h"
#include "vf_malloc_stdlib.h"
#include "mmipub.h"
#include "vf_internals.h"
#include "guiurl.h"
#include "mmivc_common.h"
#include "mmiacc_event.h"
#include "mmicom_time.h"
#include "mmialarm_export.h"
#include "mmivcalendar.h"
#include "mmischedule_export.h"
#include "mmi_solarlunar.h"
#include "mmisms_export.h"
#if defined MMI_VCALENDAR_SUPPORT
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VCAL_DEFAULT_SUBFIX       ".vcs"   //vcalendar文件后缀名
#define VCAL_DEFAULT_SUBFIX_LEN   4        //vcalendar文件后缀名长度
/*---------------------------------------------------------------------------*/
/*                          STATIC DEFINITION                                */
/*---------------------------------------------------------------------------*/
LOCAL const MMIVCAL_FIELDINFO_T s_vcal_fields[] =
{
    {MMIVCAL_PROPERTY_VERSION,     VFP_VERSION},
	{MMIVCAL_PROPERTY_CATEGORIES,     VFP_CATEGORIES},
	{MMIVCAL_PROPERTY_OBJECT_TYPE,VFP_BEGIN},
	{MMIVCAL_PROPERTY_STATUS,VFP_STATUS},
	{MMIVCAL_PROPERTY_SUMMARY,VFP_SUMMARY},
	{MMIVCAL_PROPERTY_DESCRIPTION,VFP_DESCRIPTION},
	{MMIVCAL_PROPERTY_CLASS,VFP_CLASS},
	{MMIVCAL_PROPERTY_DTSTART,VFP_DTSTART},
	{MMIVCAL_PROPERTY_DTEND,VFP_DTEND},
	{MMIVCAL_PROPERTY_COMPLETED,VFP_COMPLETED},
	{MMIVCAL_PROPERTY_DUE,VFP_DUE},
	{MMIVCAL_PROPERTY_RRULE,VFP_RRULE},
	{MMIVCAL_PROPERTY_AALARM,VFP_AALARM},
	{MMIVCAL_PROPERTY_DALARM,VFP_DALARM},
    {MMIVCAL_PROPERTY_INVALID,   ""}
};

//vcalendar 的CATEGORIES 种类，可以多选
LOCAL const char* s_vcal_cate_type[MMIVCAL_CATE_MAX]=
{
        "APPOINTMENT",
        "BUSINESS",
        "EDUCATION",
        "HOLIDAY",
        "MEETING",
        "MISCELLANEOUS",
        "PERSONAL",
        "PHONE CALL",
        "SICK DAY",
        "SPECIAL OCCASION",
        "TRAVEL",
        "VACATION"
        
};

LOCAL const  char* s_vcal_state_type[MMIVCAL_STATUS_MAX]=
{
    "ACCEPTED",
    "NEED ACTION",
    "SENT",
    "TENTATIVE",
    "CONFIRMED",
    "DECLINED",
    "COMPLETED",
    "DELEGATED"
        
};

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          GLOBAL DEFINITION                                */
/*---------------------------------------------------------------------------*/

LOCAL uint8   *s_vCalendar_ptr = PNULL;
LOCAL MMIFILE_HANDLE     s_vcal_handle_result=SFS_INVALID_HANDLE;
wchar             *g_vCal_name_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description:将属性值转换为对应的vcal_info
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL BOOLEAN VcalSetDataToVcalInfo( MMIVCAL_PROPERTY_TYPE_E       proptype,
                           //uint16                   max_len,
                           void                    *p_data,
                           MMIVCALENDAR_INFO_T *vcal_info);

/****************************************************************************/
//  Description:根据属性名获得属性类型
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL MMIVCAL_PROPERTY_TYPE_E VcalGetProptype(const char      *p_name,
                                        MMIVCAL_FIELDINFO_T  *p_field);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL BOOLEAN vcalIsValidDateTime(MMIVC_DATE_TIME_T* date_time);

/*****************************************************************************/
// Description : Init vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
//LOCAL BOOLEAN VcalBuildInit(void);

/*****************************************************************************/
// Description : Free vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
//LOCAL void VcalBuildExit(void);

/****************************************************************************/
//  Description:vcalendar是否是空的
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
//LOCAL BOOLEAN VcalendarIsEmpty(VF_OBJECT_T    *pObject);

/*****************************************************************************/
// Description : 创建vcalendar文件
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL SFS_ERROR_E VcalCreatfile(const uint16 *file_name, 
                                  uint32       mode,
								  SFS_HANDLE* handle);

/*****************************************************************************/
// Description : 创建vCalendar目录
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalInitFolder(const wchar   *device_ptr,
                                uint16   device_len,
                                const wchar   *dir_ptr,
                                uint16   dir_len);

/*****************************************************************************/
// Description : 判断保存文件所需空间是否不足
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalCheckDiskSpaceForSaveVCalendar(MMIFILE_DEVICE_E file_dev, uint32 data_size);

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalWritedFile(
                               MMI_WIN_ID_T      win_id,
                               MMI_MESSAGE_ID_E  msg_id,
                               wchar            *filename,
                               uint8            *buf_ptr,
                               uint32            buflen);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
//  Description:将属性值转换为对应的vcal_info
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL BOOLEAN VcalSetDataToVcalInfo( MMIVCAL_PROPERTY_TYPE_E       proptype,
                           //uint16                   max_len,
                           void                    *p_data,
                           MMIVCALENDAR_INFO_T *vcal_info)
{
    MMI_STRING_T *p_datastr;
    wchar        *p_text;
    
    if(PNULL == vcal_info || PNULL == p_data)
    {
        return FALSE;
    }
	
	p_datastr = (MMI_STRING_T *)p_data;
    //SCI_TRACE_LOW:"VcalSetDataToVcalInfo:p_datastr->wstr_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_199_112_2_18_3_4_31_8,(uint8*)"d",p_datastr->wstr_len);
	if(p_datastr->wstr_len > 0)
	{
		p_text=(wchar*)SCI_ALLOCA((p_datastr->wstr_len+1)*sizeof(wchar));
		if(PNULL==p_text)
		{
		    //SCI_TRACE_LOW:"VcalSetDataToVcalInfo:p_text==PNULL"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_205_112_2_18_3_4_31_9,(uint8*)"");
			return FALSE;
		}
        SCI_MEMSET(p_text,0,(p_datastr->wstr_len+1)*sizeof(wchar));
	}
	else
	{
		return FALSE;
	}
	//SCI_TRACE_LOW:"VcalSetDataToVcalInfo:proptype=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_214_112_2_18_3_4_31_10,(uint8*)"d",proptype);
    switch(proptype)
    {
      case MMIVCAL_PROPERTY_VERSION:
        vcal_info->version.wstr_len=p_datastr->wstr_len;
		if(vcal_info->version.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->version.wstr);
		}
        vcal_info->version.wstr=p_text;
        break;
	case MMIVCAL_PROPERTY_CATEGORIES:
        vcal_info->categories.wstr_len=p_datastr->wstr_len;
		if(vcal_info->categories.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->categories.wstr);
		}
        vcal_info->categories.wstr=p_text;
        break;
	case MMIVCAL_PROPERTY_STATUS:
        vcal_info->status.wstr_len=p_datastr->wstr_len;
		if(vcal_info->status.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->status.wstr);
		}
        vcal_info->status.wstr=p_text;
        break;
	case MMIVCAL_PROPERTY_SUMMARY:
        vcal_info->summary.wstr_len=p_datastr->wstr_len;
		if(vcal_info->summary.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->summary.wstr);
		}
        vcal_info->summary.wstr=p_text;
        break;
	case MMIVCAL_PROPERTY_DESCRIPTION:
        vcal_info->description.wstr_len=p_datastr->wstr_len;
		if(vcal_info->description.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->description.wstr);
		}
        vcal_info->description.wstr=p_text;
        break;
	case MMIVCAL_PROPERTY_CLASS:
        vcal_info->classification.wstr_len=p_datastr->wstr_len;
		if(vcal_info->classification.wstr!=PNULL)
		{
			SCI_FREE(vcal_info->classification.wstr);
		}
        vcal_info->classification.wstr=p_text;
        break;
      default:
        return FALSE;
    }
	
    MMIAPICOM_Wstrncpy(p_text, p_datastr->wstr_ptr, p_datastr->wstr_len);
	
    return TRUE;
}

/****************************************************************************/
//  Description:根据属性名获得属性类型
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL MMIVCAL_PROPERTY_TYPE_E VcalGetProptype(const char      *p_name,
                                        MMIVCAL_FIELDINFO_T  *p_field)
{
    uint32               i = 0;
    char                *p_tmp;
    MMIVCAL_PROPERTY_TYPE_E   ret = MMIVCAL_PROPERTY_INVALID;


    if (NULL == p_name || PNULL == p_field) 
    {
        return ret;
    }
    while(MMIVCAL_PROPERTY_INVALID != p_field[i].prop_type)
    {
        p_tmp = p_field[i].prop_name;
        if ( 0 == p_stricmp(p_name, p_tmp))
        {
            ret = p_field[i].prop_type;
            break;
        }
        i++;
    }
    return ret;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
LOCAL BOOLEAN vcalIsValidDateTime(MMIVC_DATE_TIME_T* date_time)
{
	MMISET_DATE_T date={0};
	MMISET_TIME_T time={0};
	
	if(PNULL==date_time)
	{
		return FALSE;
	}
	
	date.date_day=date_time->mday;
	date.date_month=date_time->mon;
	date.date_year=date_time->year;
	time.time_hour=date_time->hour;
	time.time_minute=date_time->min;

#ifdef CALENDAR_SUPPORT
	if(MMICALENDAR_IsDateValid(date.date_year,date.date_month,date.date_day)
        && MMIAPICOM_CheckTimeIsValid(time))
#else
    if(MMIAPICOM_CheckTimeIsValid(time))
#endif
	{
		return TRUE;
	}
    else//JAVA 对非法时间需要这样处理
    {
        date_time->hour=0;
        date_time->mday=1;
        date_time->min=0;
        date_time->mon=1;
        date_time->sec=0;
        date_time->year=1970;
        return FALSE;
    }
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC void MMIVCAL_FreeVcalInfo(MMIVCALENDAR_INFO_T *vcal_info)
{
    if(PNULL==vcal_info)
    {
        return ;
    }
	if(PNULL!=vcal_info->version.wstr)
	{
		SCI_FREE(vcal_info->version.wstr);
		vcal_info->version.wstr=PNULL;
	}
	if(PNULL!=vcal_info->categories.wstr)
	{
		SCI_FREE(vcal_info->categories.wstr);
		vcal_info->categories.wstr=PNULL;
	}
	if(PNULL!=vcal_info->status.wstr)
	{
		SCI_FREE(vcal_info->status.wstr);
		vcal_info->status.wstr=PNULL;
	}
	if(PNULL!=vcal_info->summary.wstr)
	{
		SCI_FREE(vcal_info->summary.wstr);
		vcal_info->summary.wstr=PNULL;
	}
	if(PNULL!=vcal_info->classification.wstr)
	{
		SCI_FREE(vcal_info->classification.wstr);
		vcal_info->classification.wstr=PNULL;
	}
	if(PNULL!=vcal_info->description.wstr)
	{
		SCI_FREE(vcal_info->description.wstr);
		vcal_info->description.wstr=PNULL;
	}
	if(PNULL!=vcal_info->repeat_rule.rrule)
	{
		SCI_FREE(vcal_info->repeat_rule.rrule);
		vcal_info->repeat_rule.rrule=PNULL;
	}
}

/****************************************************************************/
//  Description: 解析VCALENDAR数据
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_parse_by_data(VF_OBJECT_T **pp_object,
										uint8 *data_ptr, 
										uint32 data_size)
{
    BOOLEAN ret = TRUE;
    VF_PARSER_T      *p_parser;

    if(PNULL == data_ptr || PNULL == pp_object)
    {
       return FALSE; 
    }

    vf_set_mem_functions(MMIVC_Malloc, MMIVC_Realloc, MMIVC_Free);
    if (vf_parse_init(&p_parser, pp_object))
    {
        ret = vf_parse_text(p_parser, (char *)data_ptr, data_size);
        
        if (!vf_parse_end(p_parser))
        {
            ret = FALSE;
        }
    }
    return ret;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC void MMIVCAL_ObjectToVcalInfo(VF_OBJECT_T              *pObject,
                                MMIVCALENDAR_INFO_T *vcal_info)
{
    VF_PROP_T       *pProp;
    const char      *p_PropName = NULL;
    vf_encoding_t    enc;
    void            *pValue;
    uint32_t         nValue;
    MMI_STRING_T     tmp_str = {0};
	uint16           value_len = 0;
	char            *value=PNULL;
	char            *date_time=PNULL;
   
    if(PNULL == pObject || PNULL == vcal_info)
    {
        return ;
    }

    SCI_MEMSET(vcal_info,0,sizeof(MMIVCALENDAR_INFO_T));

    if (vf_get_property(&pProp, pObject, VFGP_FIND, NULL, "*", NULL))
    {
        do
        {
            p_PropName = vf_get_prop_name_string(pProp, 0);
            vf_get_prop_value(pProp, &pValue, &nValue, &enc);
            switch(VcalGetProptype(p_PropName,s_vcal_fields))/*lint !e605*/
            {
            case MMIVCAL_PROPERTY_VERSION://版本信息
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_VERSION,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_OBJECT_TYPE://对象类型，event or vtodo
				value=vf_get_prop_value_string(pProp, 0);
				if(0==p_stricmp(value,VF_NAME_VEVENT))
				{
					vcal_info->object_type=MMIVCAL_OBJECT_TYPE_VEVENT;
				}
				else if(0==p_stricmp(value,VF_NAME_VTODO))
				{
					vcal_info->object_type=MMIVCAL_OBJECT_TYPE_VTODO;
				}
				break;
			case MMIVCAL_PROPERTY_CATEGORIES:
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_CATEGORIES,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_STATUS:
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_STATUS,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_SUMMARY:
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_SUMMARY,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_DESCRIPTION:
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_DESCRIPTION,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_CLASS:
                MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                VcalSetDataToVcalInfo(MMIVCAL_PROPERTY_CLASS,
                               (void *)&tmp_str, vcal_info);
                 MMIVC_freeData(&tmp_str);
                break;
			case MMIVCAL_PROPERTY_DTSTART:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->dtstart.date_time);
				}
				break;
			case MMIVCAL_PROPERTY_DTEND:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->dtend.date_time);
				}
				break;
			case MMIVCAL_PROPERTY_COMPLETED:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->completed.date_time);
				}
				break;
			case MMIVCAL_PROPERTY_DUE:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->due.date_time);
				}
				break;
			case MMIVCAL_PROPERTY_RRULE:
				value=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=value)
				{
					value_len=SCI_STRLEN(value);
					if(PNULL!=vcal_info->repeat_rule.rrule)
					{
						SCI_FREE(vcal_info->repeat_rule.rrule);
						vcal_info->repeat_rule.rrule=PNULL;
					}
					vcal_info->repeat_rule.rrule=(char*)SCI_ALLOCA(value_len+1);
					if(PNULL!=vcal_info->repeat_rule.rrule)
					{
						SCI_MEMSET(vcal_info->repeat_rule.rrule,0,value_len+1);
						SCI_MEMCPY(vcal_info->repeat_rule.rrule,value,value_len);
					}
				}
				break;
			case MMIVCAL_PROPERTY_AALARM:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->aalarm.run_date_time);
				}
				break;
			case MMIVCAL_PROPERTY_DALARM:
				date_time=vf_get_prop_value_string(pProp, 0);
				if(PNULL!=date_time)
				{
					MMIVC_StringToDateTime(date_time,&vcal_info->dalarm.run_date_time);
				}
				break;
            default:
                break;
            }
        }while (vf_get_next_property(&pProp));
    }
    return;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_VcalInfoToSchedule(MMIVCALENDAR_INFO_T *vcal_info_ptr,
										MMIACC_SMART_EVENT_T *event_info_ptr)
{
	uint8   	month =0;
	uint8   	day = 0;
	uint16   	year = 0;
	uint8 		hour = 0;
	uint8 		minute = 0;
	uint16 		content_len = 0;
	//uint16 		desc_len = 0;
	uint8  		end_month = 0;
	uint8  		end_day = 0;
	uint16   	end_year = 0;
	uint8  		ring_month = 0;
	uint8  		ring_day = 0;
	uint16   	ring_year = 0;
	uint8  		ring_hour = 0;
	uint8  		ring_minute = 0;
    
    if(NULL == vcal_info_ptr||NULL == event_info_ptr)
    {
        return MMIVCAL_INVALID_DATA_PTR;
    }

	if(MMIVCAL_OBJECT_TYPE_VEVENT==vcal_info_ptr->object_type)
	{
		if(!vcalIsValidDateTime(&(vcal_info_ptr->dtstart.date_time)))
		{
			return MMIVCAL_INVALID_DATETIME;
		}
		year=vcal_info_ptr->dtstart.date_time.year;
		month=vcal_info_ptr->dtstart.date_time.mon;
		day=vcal_info_ptr->dtstart.date_time.mday;
		hour=vcal_info_ptr->dtstart.date_time.hour;
		minute=vcal_info_ptr->dtstart.date_time.min;

		if(!vcalIsValidDateTime(&(vcal_info_ptr->dtend.date_time)))
		{
			return MMIVCAL_INVALID_DATETIME;
		}

        end_year = vcal_info_ptr->dtend.date_time.year;
        end_month =vcal_info_ptr->dtend.date_time.mon;
        end_day =vcal_info_ptr->dtend.date_time.mday;
	}
	else if(MMIVCAL_OBJECT_TYPE_VTODO==vcal_info_ptr->object_type)
	{
		if(!vcalIsValidDateTime(&(vcal_info_ptr->due.date_time)))
		{
			return MMIVCAL_INVALID_DATETIME;
		}
		year=vcal_info_ptr->due.date_time.year;
		month=vcal_info_ptr->due.date_time.mon;
		day=vcal_info_ptr->due.date_time.mday;
		hour=vcal_info_ptr->due.date_time.hour;
		minute=vcal_info_ptr->due.date_time.min;

		if(!vcalIsValidDateTime(&(vcal_info_ptr->completed.date_time)))
		{
			return MMIVCAL_INVALID_DATETIME;
		}

        end_year = vcal_info_ptr->completed.date_time.year;
        end_month =vcal_info_ptr->completed.date_time.mon;
        end_day =vcal_info_ptr->completed.date_time.mday;
	}
	else
	{
		return MMIVCAL_NO_SUPPORT;
	}
    

    if(vcalIsValidDateTime(&(vcal_info_ptr->aalarm.run_date_time)))
    {

        ring_year = vcal_info_ptr->aalarm.run_date_time.year;
        ring_month =vcal_info_ptr->aalarm.run_date_time.mon;
        ring_day = vcal_info_ptr->aalarm.run_date_time.mday;
        ring_hour = vcal_info_ptr->aalarm.run_date_time.hour;
        ring_minute = vcal_info_ptr->aalarm.run_date_time.min;
    }
	else if(vcalIsValidDateTime(&(vcal_info_ptr->dalarm.run_date_time)))
    {

        ring_year = vcal_info_ptr->dalarm.run_date_time.year;
        ring_month =vcal_info_ptr->dalarm.run_date_time.mon;
        ring_day = vcal_info_ptr->dalarm.run_date_time.mday;
        ring_hour = vcal_info_ptr->dalarm.run_date_time.hour;
        ring_minute = vcal_info_ptr->dalarm.run_date_time.min;
    }
    else
    {
        ring_year = year;
        ring_month = month;
        ring_day = day;
        ring_hour = hour;
        ring_minute = minute;
    }
#ifdef CALENDAR_SUPPORT    
    MMIAPISCH_InitNewSch(year,month,day,event_info_ptr);
#endif	
	event_info_ptr->event_fast_info.start_year= year;
    event_info_ptr->event_fast_info.start_mon= month;
    event_info_ptr->event_fast_info.start_day= day;
	
    event_info_ptr->event_fast_info.year= ring_year;
    event_info_ptr->event_fast_info.mon= ring_month;
    event_info_ptr->event_fast_info.day = ring_day;
    event_info_ptr->event_fast_info.hour = ring_hour;
    event_info_ptr->event_fast_info.minute = ring_minute;
    
    event_info_ptr->event_fast_info.end_year = end_year;
    event_info_ptr->event_fast_info.end_mon = end_month;
    event_info_ptr->event_fast_info.end_day = end_day;

    if(0<vcal_info_ptr->summary.wstr_len)
    {
		content_len = MIN(MMIACC_EVENT_TEXT_MAX_LEN,vcal_info_ptr->summary.wstr_len);
		SCI_MEMSET(event_info_ptr->event_content_info.w_content,
					0,
					sizeof(wchar)*(MMIACC_EVENT_TEXT_MAX_LEN+1));
		MMIAPICOM_Wstrncpy(event_info_ptr->event_content_info.w_content,
							vcal_info_ptr->summary.wstr,
							content_len);
		event_info_ptr->event_content_info.w_length = content_len;
    }
       
    if(NULL!=vcal_info_ptr->repeat_rule.rrule)
    {
        if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"MD1")
            ||p_stristr(vcal_info_ptr->repeat_rule.rrule,"MP1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
        }
        else if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"YM1")
            ||p_stristr(vcal_info_ptr->repeat_rule.rrule,"YD1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
        }
        else if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"D1"))
        {//198053
			if(p_stristr(vcal_info_ptr->repeat_rule.rrule+3,"#1"))
			{
				event_info_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
			}
			else
			{
                event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
			}
        }
        else if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"W1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            event_info_ptr->event_fast_info.fre = 0x00;
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"SA"))
            {
                event_info_ptr->event_fast_info.fre |= 0x40;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"FR"))
            {
                event_info_ptr->event_fast_info.fre |= 0x20;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"TH"))
            {
                event_info_ptr->event_fast_info.fre |= 0x10;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"WE"))
            {
                event_info_ptr->event_fast_info.fre |= 0x08;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"TU"))
            {
                event_info_ptr->event_fast_info.fre |= 0x04;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"MO"))
            {
                event_info_ptr->event_fast_info.fre |= 0x02;
            }
            if(p_stristr(vcal_info_ptr->repeat_rule.rrule,"SU"))
            {
                event_info_ptr->event_fast_info.fre |= 0x01;
            }

            if(0x00==event_info_ptr->event_fast_info.fre)
            {
                event_info_ptr->event_fast_info.fre = 0x3e;//周一到周五
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIVCAL_VcalInfoToSchedule: No support the rrule!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_750_112_2_18_3_4_32_11,(uint8*)"");
            event_info_ptr->event_fast_info.is_on = FALSE;
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_NO;
        }

    }
   /* else if(vcalIsValidDateTime(&(vcal_info_ptr->aalarm.run_date_time)) 
		||vcalIsValidDateTime(&(vcal_info_ptr->dalarm.run_date_time)))
    {
        event_info_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
    }*/
    else
    {
        event_info_ptr->event_fast_info.is_on = FALSE;
        event_info_ptr->event_fast_info.fre_mode = ALM_MODE_NO;
    }
    return MMIVCAL_NO_ERROR;
    
}

/****************************************************************************/
//  Description:vcalendar是否是空的
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_VcalendarIsEmpty(VF_OBJECT_T    *pObject)
{
    BOOLEAN       bRet = TRUE;
    VF_PROP_T    *pProp;

    if(PNULL == pObject)
	{
		return bRet;
	}
    if (vf_get_property(&pProp, pObject, VFGP_FIND, NULL, "*", NULL))
    {
        bRet = FALSE;
    }
    return bRet;
}

/****************************************************************************/
//  Description: vcalendar数据流转换为日程事件
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_VcalendarDataToSch(uint8 *data_ptr, 
														uint32 data_size,
														MMIACC_SMART_EVENT_T *event_info_ptr)
{
    BOOLEAN ret = FALSE;
	MMIVCAL_ERROR_TYPE_E error_type=MMIVCAL_NO_ERROR;
    VF_OBJECT_T *p_object = NULL;
    VF_OBJECT_T *pTmp= NULL;
	MMIVCALENDAR_INFO_T vcal_info_ptr={0};
	
	if(PNULL==data_ptr || PNULL==event_info_ptr)
	{
		return MMIVCAL_INVALID_DATA_PTR;
	}

    SCI_MEMSET(event_info_ptr, 0, sizeof(MMIACC_SMART_EVENT_T));
	SCI_MEMSET(&vcal_info_ptr, 0, sizeof(MMIVCALENDAR_INFO_T));

    ret = MMIVCAL_parse_by_data(&p_object, data_ptr, data_size);
    if (ret && PNULL != p_object)
    {
        pTmp = p_object;
        if (!vf_get_next_object(&pTmp))
        {
            if (!MMIVCAL_VcalendarIsEmpty(p_object))
            {
            	MMIVCAL_ObjectToVcalInfo(p_object,&vcal_info_ptr);
				error_type=MMIVCAL_VcalInfoToSchedule(&vcal_info_ptr,event_info_ptr);
				MMIVCAL_FreeVcalInfo(&vcal_info_ptr);
            }
            else
            {
                error_type=MMIVCAL_IS_EMPTY;
            }
        }
        else
        {
        	error_type=MMIVCAL_IS_NESTED;
        }
        vf_delete_object(p_object, TRUE);
    }
    else
    {
        error_type=MMIVCAL_PARSE_FAIL;
    }

	//SCI_TRACE_LOW:"mmivcal:MMIVCAL_VcalendarDataToSch error_type=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_840_112_2_18_3_4_32_12,(uint8*)"d",error_type);

    return error_type;
}

/****************************************************************************/
//  Description: vcalendar数据流添加为日程事件
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_AddSchEvent(uint8 *data_ptr,
												uint32 data_size,
												uint16 *index_ptr)
{
	MMIACC_SMART_EVENT_T event_info={0};
	MMIVCAL_ERROR_TYPE_E result=MMIVCAL_NO_ERROR;
	*index_ptr=0;

	result=MMIVCAL_VcalendarDataToSch(data_ptr,data_size,&event_info);

	if(MMIVCAL_NO_ERROR==result)
	{
		MMIALM_AddOneEvent(&event_info,index_ptr);
        if(0==(*index_ptr))
    	{
    		result=MMIVCAL_SCH_IS_FULL;
    	}
	}
	
	return result;
}

/*****************************************************************************/
// Description : Init vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_VcalBuildInit(void)
{
	BOOLEAN result=FALSE;
    MMIVCAL_VcalBuildExit();
    s_vCalendar_ptr = (uint8*)SCI_ALLOCA(MMIVCAL_MAX_FILE_LEN*sizeof(uint8));
    if( PNULL != s_vCalendar_ptr )
    {
    	SCI_MEMSET(s_vCalendar_ptr,0,MMIVCAL_MAX_FILE_LEN);
		result=TRUE;
    }

	//SCI_TRACE_LOW:"mmivcal:MMIVCAL_VcalBuildInit result=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_886_112_2_18_3_4_32_13,(uint8*)"d",result);
	
    return result;
}

/*****************************************************************************/
// Description : Init vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_InitFileNamePtr(void)
{
	BOOLEAN result=FALSE;
    MMIVCAL_FreeFileNamePtr();
    g_vCal_name_ptr = (uint8*)SCI_ALLOCA(MMIFILE_FULL_PATH_MAX_LEN*sizeof(wchar));
    if( PNULL != g_vCal_name_ptr )
    {
    	SCI_MEMSET(g_vCal_name_ptr,0,MMIFILE_FULL_PATH_MAX_LEN*sizeof(wchar));
		result=TRUE;
    }

	//SCI_TRACE_LOW:"mmivcal:MMIVCAL_InitFileNamePtr result=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_906_112_2_18_3_4_32_14,(uint8*)"d",result);
	
    return result;
}

/*****************************************************************************/
// Description : Free vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_VcalBuildExit(void)
{
    if ( PNULL != s_vCalendar_ptr )
    {
        SCI_FREE( s_vCalendar_ptr );
        s_vCalendar_ptr = PNULL;
    }
    return;
}

/*****************************************************************************/
// Description : Free vcalendar global var
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_FreeFileNamePtr(void)
{
    if ( PNULL != g_vCal_name_ptr )
    {
        SCI_FREE( g_vCal_name_ptr );
        g_vCal_name_ptr = PNULL;
    }
    return;
}
/*****************************************************************************/
// Description : Get vcalendar data ptr
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC uint8 * MMIVCAL_GetBuildData(void)
{
    return s_vCalendar_ptr;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVCAL_BuildVcalendarByEvent(MMIACC_SMART_EVENT_T* event_ptr,
												MMIVCAL_OBJECT_TYPE_E type)
{
    uint8          *vcal_ptr = s_vCalendar_ptr;
    uint32         vcal_length = 0;
    uint16 temp_len = 0;
    uint8 *temp_buf = PNULL;
    char time_str[20]={0};
    //char qp_ptr[MMIVCAL_SUMMARY_MAX_LEN] = {0};
    uint16 ucs2_len = 0;
    char *version="VERSION:1.0\r\n";
    char *event="BEGIN:VEVENT\r\n";
    char *begin="BEGIN:VCALENDAR\r\n";
    char *rrule="RRULE:D1 #1\r\n";
    char * charset=";CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:";
#if 1
    uint8           i =0;
    BOOLEAN  have_cate = FALSE;
    BOOLEAN vcal_cate[MMIVCAL_CATE_MAX] = {0};
    MMIVCAL_PRIO_TYPE_E vcal_prio = MMIVCAL_PRIO_MAX;
    MMIVCAL_STATUS_TYPE_E vcal_status = MMIVCAL_STATUS_MAX;
    char vcal_class[] = "PRIVATE";//PRIVATE\PUBLIC\CONFIDENTIAL,默认为PUBLIC
    vcal_prio = MMIVCAL_PRIO_NORM;
    vcal_cate[1] = TRUE;
    vcal_status = MMIVCAL_STATUS_NEEDS_ACTION;//默认为needs action
#endif
    temp_buf=(uint8*)SCI_ALLOC_APP(MMIVCAL_SUMMARY_MAX_LEN);

    if(PNULL==vcal_ptr || PNULL==temp_buf )
    {
        return FALSE;
    }

	//SCI_TRACE_LOW:"mmivcal:MMIVCAL_BuildVcalendarByEvent start"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_984_112_2_18_3_4_33_15,(uint8*)"");

    SCI_MEMSET(temp_buf,0,MMIVCAL_SUMMARY_MAX_LEN);
	SCI_MEMSET(vcal_ptr,0,MMIVCAL_MAX_FILE_LEN*sizeof(uint8));

    if(SCI_STRLEN(version)+SCI_STRLEN(begin)>MMIVCAL_MAX_FILE_LEN)/*lint !e774*/
    {
        return FALSE;
    }   
    
    strcat ((char*)vcal_ptr, begin);

    //VERSION
    strcat ((char*)vcal_ptr, version);

    //VEVENT or VTODO
    vcal_length=SCI_STRLEN((char*)vcal_ptr);
    if(vcal_length+SCI_STRLEN(event)>MMIVCAL_MAX_FILE_LEN)
    {
        return FALSE;
    } 
    if(MMIVCAL_OBJECT_TYPE_VEVENT == type)
    {
        strcat ((char*)vcal_ptr, event);
    }
    else
    {
        strcat ((char*)vcal_ptr, "BEGIN:VTODO\r\n");
    }

    
#if 1
    // CATEGORIES 
    for(i=0;i<MMIVCAL_CATE_MAX;i++)
    {
        if(TRUE == vcal_cate[i])
        {
            have_cate = TRUE;
            break;
        }
    }
    if(TRUE == have_cate)
    {
    	//categories 如果有值，则写入，否则不写入
        have_cate = FALSE;
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_CATEGORIES)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        } 
		strcat ((char*)vcal_ptr, VFP_CATEGORIES);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
        for(i=0;i<MMIVCAL_CATE_MAX;i++)
        {
            if(TRUE == vcal_cate[i])
            {
                if(TRUE == have_cate)//第一个categaries属性之前没有";"
                {
                    if(SCI_STRLEN((char*)vcal_ptr)+1>MMIVCAL_MAX_FILE_LEN)
                    {
                        return FALSE;
                    } 
                    strcat((char*)vcal_ptr,";");
                }
                have_cate = TRUE;
                if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(s_vcal_cate_type[i])>MMIVCAL_MAX_FILE_LEN)
                {
                    return FALSE;
                } 
                strcat((char*)vcal_ptr,s_vcal_cate_type[i]);
            }
        }
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(MMIVCAL_LFCR)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        } 
        strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }

    //PRIORITY 
    if(MMIVCAL_PRIO_MAX > vcal_prio)/*lint !e774*/
    {
        char vcal_prio_arr[2]={0};
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_PRIORITY)+SCI_STRLEN(MMIVCAL_COLON)+SCI_STRLEN(MMIVCAL_LFCR)+1>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
		strcat ((char*)vcal_ptr, VFP_PRIORITY);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
        vcal_prio += '0';
        vcal_prio_arr[0]=vcal_prio;
        vcal_prio_arr[1]=0;
        strcat ((char*)vcal_ptr, (char*)vcal_prio_arr);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    
    //CLASS
    if(NULL!=vcal_class)/*lint !e774 !e506*/
    {
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_CLASS)+SCI_STRLEN(MMIVCAL_COLON)+SCI_STRLEN(MMIVCAL_LFCR)+SCI_STRLEN((char*)(vcal_class))>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
		strcat ((char*)vcal_ptr, VFP_CLASS);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
        strcat ((char*)vcal_ptr, (char*)(vcal_class));
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    
    //STATUS 
    if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_STATUS)+SCI_STRLEN(MMIVCAL_COLON)+SCI_STRLEN(MMIVCAL_LFCR)+SCI_STRLEN((char*)(s_vcal_state_type[vcal_status]))>MMIVCAL_MAX_FILE_LEN)
    {
        return FALSE;
    }
    strcat ((char*)vcal_ptr, VFP_STATUS);
	strcat ((char*)vcal_ptr, MMIVCAL_COLON);
    if(MMIVCAL_STATUS_MAX==vcal_status)/*lint !e774*/
    {
        vcal_status = MMIVCAL_STATUS_NEEDS_ACTION;
    }
    strcat ((char*)vcal_ptr,s_vcal_state_type[vcal_status] );
	strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
#endif

	//SUMMARY
	if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_SUMMARY)+SCI_STRLEN(charset)>MMIVCAL_MAX_FILE_LEN)
    {
        return FALSE;
    }
    strcat ((char*)vcal_ptr, VFP_SUMMARY);
    strcat ((char*)vcal_ptr, charset);
    
    temp_len = GUI_WstrToUTF8(temp_buf,
                MMIVCAL_SUMMARY_MAX_LEN,
                event_ptr->event_content_info.w_content,
                MIN(event_ptr->event_content_info.w_length,MMIACC_EVENT_TEXT_MAX_LEN));
	vcal_length=SCI_STRLEN((char*)vcal_ptr);
    if(vcal_length+SCI_STRLEN(MMIVCAL_LFCR)>=MMIVCAL_MAX_FILE_LEN)
    {
        return FALSE;
    }
    ucs2_len = vf_vcard_qprint_encode(temp_buf, temp_len,(char*)(vcal_ptr+vcal_length),MMIVCAL_MAX_FILE_LEN-vcal_length,TRUE);
	strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
	
    if(MMIVCAL_OBJECT_TYPE_VEVENT== type)
    {
        //DTSTART
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_DTSTART)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
		strcat ((char*)vcal_ptr, VFP_DTSTART);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
		SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)+SCI_STRLEN(VFP_DTEND)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
		
		//DTEND
        strcat ((char*)vcal_ptr, VFP_DTEND);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
        SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.end_year, 
                    event_ptr->event_fast_info.end_mon, 
                    event_ptr->event_fast_info.end_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    else//VTODO 对象
    {
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_DUE)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, VFP_DUE);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
		SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)+SCI_STRLEN(VFP_COMPLETED)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
		
        //COMPLETED
        strcat ((char*)vcal_ptr, VFP_COMPLETED);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
        SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.end_year, 
                    event_ptr->event_fast_info.end_mon, 
                    event_ptr->event_fast_info.end_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    
    //RRULE
    if(ALM_MODE_NO == event_ptr->event_fast_info.fre_mode)//不提醒
    {
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_RRULE)+SCI_STRLEN(MMIVCAL_COLON)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
		strcat ((char*)vcal_ptr, VFP_RRULE);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
        strcat((char*)vcal_ptr, "AALARM:;;;\r\n");
    }
    else if(ALM_MODE_ONCE == event_ptr->event_fast_info.fre_mode)//一次
    {
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(rrule)+SCI_STRLEN(VFP_AALARM)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat((char*)vcal_ptr, rrule);
		
        strcat ((char*)vcal_ptr, VFP_AALARM);
		strcat ((char*)vcal_ptr, MMIVCAL_COLON);
		SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.year, 
                    event_ptr->event_fast_info.mon, 
                    event_ptr->event_fast_info.day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)+3>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
        strcat ((char*)vcal_ptr, ";;;");
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    else
    {
    	SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(VFP_RRULE)+SCI_STRLEN(MMIVCAL_COLON)+3+SCI_STRLEN(MMIVCAL_LFCR)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
		
        strcat((char*)vcal_ptr, VFP_RRULE);
        strcat((char*)vcal_ptr, MMIVCAL_COLON);
        if(ALM_MODE_EVERYDAY == event_ptr->event_fast_info.fre_mode)//每天
        {
            strcat((char*)vcal_ptr, "D1 ");
            strcat ((char*)vcal_ptr, time_str);
        }
        else if(ALM_MODE_EVERYMONTH == event_ptr->event_fast_info.fre_mode)//每月
        {
            strcat((char*)vcal_ptr, "MD1 ");
            strcat ((char*)vcal_ptr, time_str);
        }
        else if(ALM_MODE_EVERYYEAR == event_ptr->event_fast_info.fre_mode)//每年
        {
            strcat((char*)vcal_ptr, "YM1 ");
            strcat ((char*)vcal_ptr, time_str);
        }
        else if(ALM_MODE_EVERYWEEK == event_ptr->event_fast_info.fre_mode)//每周
        {
            strcat((char*)vcal_ptr, "W1 ");
            if(event_ptr->event_fast_info.fre&0x02)
            {
                strcat((char*)vcal_ptr, "MO ");//周一
            }
            if(event_ptr->event_fast_info.fre&0x04)
            {
                strcat((char*)vcal_ptr, "TU ");//周二
            }
            if(event_ptr->event_fast_info.fre&0x08)
            {
                strcat((char*)vcal_ptr, "WE ");//周三
            }
            if(event_ptr->event_fast_info.fre&0x10)
            {
                strcat((char*)vcal_ptr, "TH ");//周四
            }
            if(event_ptr->event_fast_info.fre&0x20)
            {
                strcat((char*)vcal_ptr, "FR ");//周五
            }
            if(event_ptr->event_fast_info.fre&0x40)
            {
                strcat((char*)vcal_ptr, "SA ");//周六
            }
            if(event_ptr->event_fast_info.fre&0x01)
            {
                strcat((char*)vcal_ptr, "SU ");//周日
            }
            strcat ((char*)vcal_ptr, time_str);
        }
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
		
        //AALARM
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(VFP_AALARM)+SCI_STRLEN(MMIVCAL_COLON)>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, VFP_AALARM);
        strcat ((char*)vcal_ptr, MMIVCAL_COLON);
		SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN(time_str)+SCI_STRLEN(MMIVCAL_LFCR)+3>MMIVCAL_MAX_FILE_LEN)
        {
            return FALSE;
        }
        strcat ((char*)vcal_ptr, time_str);
        strcat ((char*)vcal_ptr, ";;;");
		strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    }
    
    //end vevent or vtodo
    if(SCI_STRLEN((char*)vcal_ptr)+SCI_STRLEN("END:VEVENT")+SCI_STRLEN(MMIVCAL_LFCR)+SCI_STRLEN("END:VCALENDAR")>MMIVCAL_MAX_FILE_LEN)
    {
        return FALSE;
    }
    if(MMIVCAL_OBJECT_TYPE_VEVENT == type)
    {
        strcat ((char*)vcal_ptr, "END:VEVENT");
    }
    else
    {
        strcat ((char*)vcal_ptr, "END:VTODO");
    }
    strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
    
    //end vcalendar
    strcat ((char*)vcal_ptr, "END:VCALENDAR");
    strcat ((char*)vcal_ptr, MMIVCAL_LFCR);
	
	vcal_length=SCI_STRLEN((char*)vcal_ptr);

	//SCI_TRACE_LOW:"mmivcal: vcal_lendth is=%d,vcal_info is %s "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1361_112_2_18_3_4_33_16,(uint8*)"ds",vcal_length,vcal_ptr);
    return TRUE;
    
}

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
												uint32 *len)
{
	uint16 i=0;
	uint16 valid_count=0;
	uint16 event_index=0;
	MMIACC_SMART_EVENT_T event={0};
	uint32 vcal_len=0;
	const MMIACC_SMART_EVENT_FAST_TABLE_T * event_fast_table=PNULL;

	*len=0;
	
	if(valid_index<1 || valid_index>MMIACC_SCH_NUM)
	{
		return FALSE;
	}
	
	for(i=EVENT_SCH_0; i<=EVENT_SCH_MAX; i++)
    {
    	event_fast_table=MMIALM_GeEvent(i);
        if(event_fast_table->is_valid)
        {
            valid_count++;
			if(valid_index==valid_count)
			{
				event_index=i;
				break;
			}
        }
    }
	
	if(0!=event_index)
	{
		MMIAPIALM_FormInteEventInfo(event_index,&event);
		if(MMIVCAL_VcalBuildInit())
		{
			MMIVCAL_BuildVcalendarByEvent(&event,type);
			vcal_len=SCI_STRLEN((char*)s_vCalendar_ptr);
			if(buffer_size<vcal_len )
			{
				MMIVCAL_VcalBuildExit();
				return FALSE;
			}
			else
			{
				SCI_MEMCPY(buffer,s_vCalendar_ptr,vcal_len);
				*len=vcal_len;
				MMIVCAL_VcalBuildExit();
			}
			
		}
		else
		{
			//SCI_TRACE_LOW:"mmivcal:memory is not enough!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1426_112_2_18_3_4_33_17,(uint8*)"");
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		//SCI_TRACE_LOW:"mmivcal:valid_index is error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1433_112_2_18_3_4_34_18,(uint8*)"");
		return FALSE;
	}
}
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
									   )
{
	uint16 i=0;
	//uint16 valid_count=0;
	//uint16 event_index=0;
	MMIACC_SMART_EVENT_T event={0};
	uint32 vcal_len=0;
	const MMIACC_SMART_EVENT_FAST_TABLE_T * event_fast_table=PNULL;
	int32 ret=-1;
	uint16 last_id=0;
	BOOLEAN is_find=FALSE;

	//SCI_TRACE_LOW:"mmivcal:MMIVCAL_GetNextVcalItem start"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1458_112_2_18_3_4_34_19,(uint8*)"");
	
	if(lastItemId>=MMIACC_SCH_NUM-1)
	{
		return ret;
	}
	
	last_id=lastItemId+1;
	
	for(i=(last_id+EVENT_SCH_0); i<=EVENT_SCH_MAX; i++)
    {
    	event_fast_table=MMIALM_GeEvent(i);
        if(event_fast_table->is_valid)
        {
				*uuid_ptr=i-EVENT_SCH_0;
				is_find=TRUE;
				break;
        }
    }
	
	if(is_find)
	{
		MMIAPIALM_FormInteEventInfo(i,&event);
		if(MMIVCAL_VcalBuildInit())
		{
			MMIVCAL_BuildVcalendarByEvent(&event,type);
			vcal_len=SCI_STRLEN((char*)s_vCalendar_ptr);
			if(maxItemLen<vcal_len )
			{
				MMIVCAL_VcalBuildExit();
			}
			else
			{
				SCI_MEMCPY(item_ptr,s_vCalendar_ptr,vcal_len);
				MMIVCAL_VcalBuildExit();
				ret=0;
			}
		}
		else
		{
			//SCI_TRACE_LOW:"mmivcal:memory is not enough!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1498_112_2_18_3_4_34_20,(uint8*)"");
		}
		return ret;
	}
	else
	{
		//SCI_TRACE_LOW:"mmivcal:not exist valid sch!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1504_112_2_18_3_4_34_21,(uint8*)"");
		return ret;
	}
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_AddVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             *uuid_ptr 
									   )
{
	uint16 index=0;
	int32 ret=-1;
	
	if(MMIVCAL_NO_ERROR==MMIVCAL_AddSchEvent(item_ptr,item_len,&index))
	{
		*uuid_ptr=index-EVENT_SCH_0;
		ret=0;
	}
	else
	{
		ret=-1;
	}
	
	//SCI_TRACE_LOW:"mmivcal:AddtVcalItem uuid=%d,ret=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1531_112_2_18_3_4_34_22,(uint8*)"dd",*uuid_ptr,ret);
	
	return ret;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_ModifyVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             uuid
									   )
{
	MMIACC_SMART_EVENT_T event_info={0};
	//BOOLEAN result=FALSE;
	int32 ret=-1;
	uint16 index=uuid+EVENT_SCH_0;

	if(index<EVENT_SCH_0 || index>EVENT_SCH_MAX)
	{
		//SCI_TRACE_LOW:"mmivcal:ModifyVcalItem uuid is error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1552_112_2_18_3_4_34_23,(uint8*)"");
		return ret;
	}
	
	if(MMIVCAL_NO_ERROR==MMIVCAL_VcalendarDataToSch(item_ptr,item_len,&event_info))
	{
		MMIAPIALM_SaveInteEventInfo(index,&event_info);
		ret=0;
	}
	
	//SCI_TRACE_LOW:"mmivcal:ModifyVcalItem uuid=%d,ret=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1562_112_2_18_3_4_34_24,(uint8*)"dd",uuid,ret);
	
	return ret;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_RemoveVcalItem(int32  uuid)
{
	//BOOLEAN result=FALSE;
	int32 ret=-1;
	uint16 index=uuid+EVENT_SCH_0;

	if(index<EVENT_SCH_0 || index>EVENT_SCH_MAX)
	{
		//SCI_TRACE_LOW:"mmivcal:RemoveVcalItem uuid is error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1579_112_2_18_3_4_34_25,(uint8*)"");
		return ret;
	}
	
	if(MMIALM_DelOneEvent(index))
	{
		ret=0;
	}

	//SCI_TRACE_LOW:"mmivcal:RemoveVcalItem uuid=%d,ret=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1588_112_2_18_3_4_34_26,(uint8*)"dd",uuid,ret);
	
	return ret;
}
/*****************************************************************************/
// Description : 创建vcalendar文件
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL SFS_ERROR_E VcalCreatfile(const uint16 *file_name, 
                                  uint32       mode,
								  SFS_HANDLE* handle)
{
    uint16    path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16    path_len = 0;
	uint16    full_path_len = 0;
	SFS_ERROR_E ret_val = SFS_ERROR_INVALID_PARAM;

    //SCI_TRACE_LOW:"mmivcal:VcalCreatfile()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1605_112_2_18_3_4_34_27,(uint8*)"");
    if (PNULL == file_name || PNULL == handle)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
	full_path_len = MMIAPICOM_Wstrlen(file_name);
	MMIAPICOM_GetFilePath(file_name, full_path_len, path_name, &path_len);
	if(full_path_len > path_len + 2)
	{
		ret_val = SFS_CreateFileAsyn(path_name, &(file_name[path_len + 1]), mode, NULL, NULL, handle, PNULL);
	}
    return ret_val;
}

/*****************************************************************************/
// Description : 创建vCalendar目录
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalInitFolder(const wchar   *device_ptr,
                                uint16   device_len,
                                const wchar   *dir_ptr,
                                uint16   dir_len)
{
    BOOLEAN    result = FALSE;
    wchar     full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    wchar     full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    if((PNULL == device_ptr) || (PNULL == dir_ptr) || (0 == device_len) || (0 == dir_len))
    {
        return FALSE;
    }
    vf_stdlib_init_mem();
    if(MMIAPIFMM_CombineFullPath((wchar *)device_ptr, device_len,
                            (wchar *)dir_ptr, dir_len, 
                            NULL, 0, 
                            full_path_name, &full_path_len))
    {
	    if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
	    {
	        result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
	    }
	    else
	    {
	        result = TRUE;
	    }
    }
    return result;
}

/*****************************************************************************/
// Description: init vcalendar module
// Global resource dependence:
// Auther: renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCAL_Init(void)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
	do
	{
	    if(MMIAPIFMM_GetDeviceTypeStatus(dev))
	    {
		    VcalInitFolder(
		                    MMIAPIFMM_GetDevicePath(dev),
		                    MMIAPIFMM_GetDevicePathLen(dev),
						    MMIMULTIM_DIR_VCALENDAR, 
						    MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VCALENDAR));	    
		}
	}while(++dev < MMI_DEVICE_NUM);
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_ReadFile(VF_OBJECT_T **pp_object,
                        uint16       *name_ptr)
{
    MMIVCAL_ERROR_TYPE_E     ret = MMIVCAL_NO_SUPPORT;
	BOOLEAN           parse_ret = FALSE;
    MMIFILE_HANDLE    file_handle = SFS_INVALID_HANDLE;
    char              buffer[1024];
    uint32            charsread = 0;
    VF_PARSER_T      *p_parser;
    MMIFILE_ERROR_E   ffs_error;
    uint16            file_size = 0;
    
    if((PNULL== pp_object) || (PNULL == name_ptr))
    {
        //SCI_TRACE_LOW:"mmivcal: MMIVCAL_ReadFile pp_object 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1691_112_2_18_3_4_34_28,(uint8*)"d", pp_object);
        return ret;
    }
 
    ffs_error = VcalCreatfile((const uint16 *)name_ptr,
                               SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, &file_handle);/*lint !e655*/
    //SCI_TRACE_LOW:"mmivcal:VcalCreatfile() file_handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1697_112_2_18_3_4_34_29,(uint8*)"d", file_handle);
    if (file_handle)
    {
        //判断文件是否超长
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if ( file_size > MMIVCAL_MAX_FILE_LEN || (0 == file_size))
        {
            MMIAPIFMM_CloseFile(file_handle);
			if(0 == file_size)
			{
				ret = MMIVCAL_IS_EMPTY;
			}
			else
			{
                ret = MMIVCAL_IS_TOOBIG;
			}
            return ret;
        }
        //Set the memory allocation functions
        vf_set_mem_functions(MMIVC_Malloc, MMIVC_Realloc, MMIVC_Free);
        if (vf_parse_init(&p_parser, pp_object))
        {
            do
            {
                //同步读文件
                SCI_MEMSET(buffer, 0, sizeof(buffer));
                ffs_error = MMIAPIFMM_ReadFile(file_handle,buffer,sizeof(buffer),&charsread,PNULL);
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                    parse_ret = vf_parse_text(p_parser, buffer, charsread);
                }
				if(vf_is_end(p_parser))
				{
					break;
				}
            }
            while (parse_ret && (0 < charsread));

            if (vf_parse_end(p_parser))
            {
                ret = MMIVCAL_NO_ERROR;
            }
        }

        MMIAPIFMM_CloseFile(file_handle);
    }
	else
	{
		ret = MMIVCAL_FILE_ERROR;
	}
    return ret;
}

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
											wchar* new_file_name_ptr)
{
    SFS_ERROR_E          result = SFS_ERROR_DEVICE;
    //uint8                *vCard_data_ptr = s_vCard_ptr;
    //wchar               *vCard_name_ptr = g_vCard_name_ptr;
    wchar*                full_path_name = PNULL;
    uint16               full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    uint32                transmitted;
    MMIFILE_OVERLAPPED_T  overLapped={0};

    full_path_name=(wchar*)SCI_ALLOCA((MMIFILE_FULL_PATH_MAX_LEN+2)*sizeof(wchar));
    if(PNULL==full_path_name)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    SCI_MEMSET(full_path_name , 0, ((MMIFILE_FULL_PATH_MAX_LEN+2)*sizeof(wchar)));
	
    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIAPICOM_CombineDefaultFullPathByFilename(fs_dev, vCard_name_ptr, MMIAPICOM_Wstrlen(vCard_name_ptr), 
                                               full_path_name, full_path_len);
	 //创建文件
     MMIAPICOM_GetNoExistFileName(full_path_name, MMIAPICOM_Wstrlen(full_path_name));
	 if(new_file_name_ptr != PNULL)
	 {
		 MMIAPICOM_Wstrncpy(new_file_name_ptr, full_path_name, MMIAPICOM_Wstrlen(full_path_name));
	 }
    result = VcalCreatfile((const uint16 *)full_path_name,
                                         SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &s_vcal_handle_result);/*lint !e655*/
    //SCI_TRACE_LOW:"mmivcal:MMIVCAL_SaveVcalendarFile g_vcard_handle_result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1787_112_2_18_3_4_34_30,(uint8*)"d",s_vcal_handle_result);
    if(SFS_INVALID_HANDLE != s_vcal_handle_result)
    {
        //异步写文件数据
		result = MMIAPIFMM_SetFileSize(s_vcal_handle_result, strlen((char*)vCard_data_ptr));
		if(SFS_ERROR_NONE == result)
		{
			overLapped.complete_route = callback;
			result = MMIAPIFMM_WriteFile(s_vcal_handle_result,
				vCard_data_ptr,
				(uint16)(strlen((char*)vCard_data_ptr)),
				&transmitted, &overLapped);
			//result = TRUE;
			switch (result)
			{
			case SFS_ERROR_IO_PENDING:           
				break;
			case SFS_NO_ERROR:           
				break;
			default:
				//SCI_TRACE_LOW:"mmivcal:MMIVCAL_SaveVcalendarFile: FFS_Write failed. result=%d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1807_112_2_18_3_4_34_31,(uint8*)"d", result);
				MMIAPIFMM_CloseFile(s_vcal_handle_result);
				break;
			}
		}
		else
		{
			MMIAPIFMM_CloseFile(s_vcal_handle_result);
			MMIAPIFMM_DeleteFile(full_path_name, PNULL);
		}
    }
    SCI_FREE(full_path_name);
    return (result);
}

/*****************************************************************************/
// Description : 保存vCalendar信息，并写入指定文件夹
// Global resource dependence : s_vcal_handle_result
// Author:Jesse.Yu
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIVCAL_SaveVcalendarFileToAssignFolder(const wchar * folder_name_ptr,
													 	//uint8* vCard_data_ptr,
														//wchar* vCard_name_ptr,
														//SFS_COMPLETETION_ROUTINE callback,
														wchar * new_file_name_ptr)
{
    MMIFILE_ERROR_E      result = SFS_ERROR_DEVICE;
    wchar*                full_path_name = PNULL;
    uint16               full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
    uint32                transmitted;
    MMIFILE_OVERLAPPED_T  overLapped={0};
    uint8* vCard_data_ptr=s_vCalendar_ptr;
    wchar* vCard_name_ptr=g_vCal_name_ptr;

    if(folder_name_ptr == PNULL || 0 == MMIAPICOM_Wstrlen(folder_name_ptr) || 0 == MMIAPICOM_Wstrlen(vCard_name_ptr))
	{
		return SFS_ERROR_INVALID_PARAM;
	}

    full_path_name=(wchar*)SCI_ALLOCA((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    if(PNULL==full_path_name)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    SCI_MEMSET(full_path_name,0,(MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    
	//MMIFILE_SLASH
	full_path_len = MIN(MMIAPICOM_Wstrlen(folder_name_ptr), MMIFILE_FULL_PATH_MAX_LEN);/*lint !e666*/
	MMIAPICOM_Wstrncpy(full_path_name, folder_name_ptr, MIN(full_path_len, full_path_len));
	if(full_path_len < MMIFILE_FULL_PATH_MAX_LEN)
	{
		full_path_name[full_path_len] = MMIFILE_SLASH;
		full_path_len++;
	}
	if(full_path_len < MMIFILE_FULL_PATH_MAX_LEN)
	{
		MMIAPICOM_Wstrncat(full_path_name, vCard_name_ptr, MIN((MMIFILE_FULL_PATH_MAX_LEN - full_path_len),MMIAPICOM_Wstrlen(vCard_name_ptr)));/*lint !e574*//*lint !e666*/
		 //创建文件
		MMIAPICOM_GetNoExistFileName(full_path_name, MMIAPICOM_Wstrlen(full_path_name));
		if(new_file_name_ptr != PNULL)
		{
			MMIAPICOM_Wstrncpy(new_file_name_ptr, full_path_name, MMIAPICOM_Wstrlen(full_path_name));
		}
		result = VcalCreatfile((const uint16 *)full_path_name,
			SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &s_vcal_handle_result);/*lint !e655*/
 
	    if(SFS_INVALID_HANDLE != s_vcal_handle_result)
	    {
	        //异步写文件数据
			result = MMIAPIFMM_SetFileSize(s_vcal_handle_result, strlen((char*)vCard_data_ptr));
			if(SFS_NO_ERROR == result)
			{
				overLapped.complete_route = MMIVCAL_CallBackSaveVcalendarFile;
				result = MMIAPIFMM_WriteFile(s_vcal_handle_result,
					vCard_data_ptr,
					(uint16)(strlen((char*)vCard_data_ptr)),
					&transmitted, &overLapped);
			}
			else
			{
				MMIAPIFMM_CloseFile(s_vcal_handle_result);
				MMIAPIFMM_DeleteFile(full_path_name, PNULL);
			}
	        //result = TRUE;
            if(SFS_NO_ERROR != result && (SFS_ERROR_IO_PENDING != result))
            {
                MMIAPIFMM_CloseFile(s_vcal_handle_result);;
            }
	    }
	}
    
    if(SFS_NO_ERROR != result && (SFS_ERROR_IO_PENDING != result))
	{
	    MMIVCAL_VcalBuildExit();
		MMIVCAL_FreeFileNamePtr();
	}

    SCI_FREE(full_path_name);
    return (result);
}
/*****************************************************************************/
//  Description : 保存联系人生成的vCalendar文件的回调函数
//  Global resource dependence : 
//  Author:    Jenny.Sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_CallBackSaveVcalendarFile(SFS_ERROR_E  error, 
                                       uint32       param,
                                       uint32       result,
                                       uint32       param2)
{
    MmiSignalS *signal = PNULL;
       
    MMI_CREATE_SIGNAL(signal,(uint16)MSG_VCALENDAR_SAVE_IND,sizeof(MmiSignalS),P_APP);
    MMI_SEND_SIGNAL(signal, P_APP);
    //MMK_PostMsg(VIRTUAL_WIN_ID, MSG_VCALENDAR_SAVE_SCH_CNF, (DPARAM)&error,sizeof(FFS_ERROR_E));
}

/*****************************************************************************/
// Description : 同步保存Vcal信息，并写入文件系统
// Global resource dependence : s_vcal_handle_result
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIVCAL_SaveVcalFileSync(MMIFILE_DEVICE_E fs_dev,
												//uint8* vCard_data_ptr,
												//wchar* vCard_name_ptr,
												wchar* new_file_name_ptr)
{
    SFS_ERROR_E          result = SFS_ERROR_DEVICE;
    wchar                full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16               full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    uint32                transmitted;

   	SCI_MEMSET(full_path_name , 0, ((MMIFILE_FULL_PATH_MAX_LEN+2)*sizeof(wchar)));
    {
        full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIAPICOM_CombineDefaultFullPathByFilename(fs_dev, g_vCal_name_ptr, MMIAPICOM_Wstrlen(g_vCal_name_ptr), 
                                                   full_path_name, full_path_len);
         MMIAPICOM_GetNoExistFileName(full_path_name, MMIAPICOM_Wstrlen(full_path_name));
		 if(new_file_name_ptr != PNULL)
		 {
			 MMIAPICOM_Wstrncpy(new_file_name_ptr, full_path_name, MMIAPICOM_Wstrlen(full_path_name));
		 }
	    result = VcalCreatfile((const uint16 *)full_path_name,
	                                         SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &s_vcal_handle_result);/*lint !e655*/
	    
	    if(SFS_INVALID_HANDLE != s_vcal_handle_result)
	    {
	        result = MMIAPIFMM_WriteFile(s_vcal_handle_result,
	                                      s_vCalendar_ptr,
	                                      (uint16)(strlen((char*)s_vCalendar_ptr)),
	                                      &transmitted, PNULL);
			    
            MMIAPIFMM_CloseFile(s_vcal_handle_result);
	    }
	}
    return (result);
}

/****************************************************************************/
//  Description: Read vCal file to global variable
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
PUBLIC MMIVCAL_ERROR_TYPE_E MMIVCAL_GetDataByFile(uint16    *name_ptr)
{
    MMIVCAL_ERROR_TYPE_E     ret = MMIVCAL_NO_ERROR;
    MMIFILE_HANDLE    file_handle = SFS_INVALID_HANDLE;
    uint32            charsread = 0;
    MMIFILE_ERROR_E   ffs_error = SFS_NO_ERROR;
    uint16            file_size = 0;
    uint8            *p_data = PNULL;
    
    if(PNULL== name_ptr)
    {
        return ret;
    }
	
    ffs_error = VcalCreatfile((const uint16 *)name_ptr,
                               SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, &file_handle);/*lint !e655*/
    //SCI_TRACE_LOW:"mmivcal:MMIVCAL_GetDataByFile() file_handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_1983_112_2_18_3_4_35_32,(uint8*)"d", file_handle);
    if (file_handle)
    {
        //文件长度检查
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if ( 0 == file_size )
        {
            ret = MMIVCAL_IS_EMPTY;
        }
        else if ( file_size > MMIVCAL_MAX_FILE_LEN )
        {
            ret = MMIVCAL_IS_TOOBIG;
        }
        else
        {
            //初始化缓冲区
            MMIVCAL_VcalBuildInit();
            p_data = MMIVCAL_GetBuildData();
            if(p_data != PNULL)
            {
                //同步读文件
                ffs_error = MMIAPIFMM_ReadFile(file_handle,p_data,file_size,&charsread,PNULL);
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                    ret = MMIVCAL_NO_ERROR;
                }                
            }
        }

    }
    if(SFS_INVALID_HANDLE != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
    return ret;
}

/*****************************************************************************/
// Description : 获得vCal文件名称
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC wchar* MMIVCAL_GetVcalendarFileName( MMIACC_SMART_EVENT_T*  event_ptr)
{
    wchar          *vCal_name_ptr = g_vCal_name_ptr;
    uint8            name_str[MMIFILE_FILE_NAME_MAX_LEN] = {0};
    uint16           name_len = 0;
	uint16           max_len = 0;
    uint16           valid_pos = 0;
    const char      *p_ext=VCAL_DEFAULT_SUBFIX;
     
    if(PNULL == event_ptr)
    {
        //SCI_TRACE_LOW:"mmivcal: MMIVCAL_GetVcalendarFileName PNULL == entry_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2035_112_2_18_3_4_35_33,(uint8*)"");
        return PNULL;
    }

    sprintf((char*)name_str, "%d%d%d%d%d%s",
         event_ptr->event_fast_info.year,
         event_ptr->event_fast_info.mon,
         event_ptr->event_fast_info.day,
         event_ptr->event_fast_info.hour,
         event_ptr->event_fast_info.minute,
         VCAL_DEFAULT_SUBFIX);

    if(0<event_ptr->event_content_info.w_length)
    {

        if (MMIAPICOM_IsIncludeInvalidChar(event_ptr->event_content_info.w_content,event_ptr->event_content_info.w_length))
        {
            name_len = MIN(strlen((char*)name_str), MMIFILE_FILE_NAME_MAX_LEN); /*lint !e666*/
            MMI_STRNTOWSTR((wchar *)vCal_name_ptr, MMIFILE_FILE_NAME_MAX_LEN, name_str, name_len, name_len);
        }
        else 
        {
            valid_pos = MMIVC_GetFirstValidLetterPos(event_ptr->event_content_info.w_content);

            max_len = MIN(event_ptr->event_content_info.w_length,(MMIFILE_FILE_NAME_MAX_LEN - VCAL_DEFAULT_SUBFIX_LEN - MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VCALENDAR) -1));/*lint !e666*/
            max_len = MIN(max_len,MMIACC_EVENT_TEXT_MAX_LEN);/*lint !e831*/

            if((valid_pos >= max_len) )/*lint !e831*/
            {
                name_len = MIN(strlen((char*)name_str), MMIFILE_FILE_NAME_MAX_LEN); /*lint !e666*/
                MMI_STRNTOWSTR((wchar *)vCal_name_ptr, MMIFILE_FILE_NAME_MAX_LEN, name_str, name_len, name_len);
            }
            else
            {
                max_len = max_len - valid_pos;
                MMIAPICOM_Wstrncpy(vCal_name_ptr, &event_ptr->event_content_info.w_content[valid_pos], max_len);/*lint !e831*/
                MMI_STRNTOWSTR(&vCal_name_ptr[max_len], VCAL_DEFAULT_SUBFIX_LEN,(uint8*)p_ext, VCAL_DEFAULT_SUBFIX_LEN, VCAL_DEFAULT_SUBFIX_LEN);
                vCal_name_ptr[max_len + VCAL_DEFAULT_SUBFIX_LEN] = 0;
            }
        }
    }
    else
    {
         name_len = MIN(strlen((char*)name_str), MMIFILE_FILE_NAME_MAX_LEN); /*lint !e666*/
         MMI_STRNTOWSTR((wchar *)vCal_name_ptr, MMIFILE_FILE_NAME_MAX_LEN, name_str, name_len, name_len);

    }
    return g_vCal_name_ptr;
}

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
                                uint32            data_len)
{
    uint16          full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint8           vcal_file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {"vcalendar"};
    wchar*          dev_path_ptr = PNULL;
    uint32          dev_len = 0;
    uint16          full_path_len = MMIFILE_FULL_PATH_MAX_LEN+1;
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE;
    uint8           *vcal_buf = PNULL;
    MMIVCAL_ERROR_TYPE_E   result = MMIVCAL_FILE_ERROR;
    MMIFILE_DEVICE_E    fs_dev = MMI_DEVICE_NUM;
    MMIFILE_DEVICE_E    tmp_fs_dev = MMI_DEVICE_NUM;
    SCI_TIME_T      sys_time = {0};
    SCI_DATE_T      sys_data = {0};
    uint16          *name_ptr = PNULL;

 
    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"MMIVCAL_SaveVCalendarByData 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2112_112_2_18_3_4_35_34,(uint8*)"d", data_ptr);
        return result;
    }
    if (0 == data_len)
    {
        //SCI_TRACE_LOW:"MMIVCAL_SaveVCalendarByData data_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2117_112_2_18_3_4_35_35,(uint8*)"d", data_len);
        result = MMIVCAL_IS_EMPTY;
    }
    else
    {
        vcal_buf = (uint8 *)SCI_ALLOCA(data_len*sizeof(uint8));
        if(PNULL != vcal_buf)
        {
            SCI_MEMSET(vcal_buf,0,data_len);
            SCI_MEMCPY(vcal_buf, data_ptr, (data_len*sizeof(uint8)));
            fs_dev = MMIAPIFMM_GetFirstValidDevice();
            //if(VcalCheckDiskSpaceForSaveVCalendar(fs_dev, data_len))
            if(MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_PREFER,fs_dev,data_len,&tmp_fs_dev))
            {
                fs_dev=tmp_fs_dev;               
                //delete unused variable for coverity
                //dev_len = MMIAPIFMM_GetDevicePathLen((fs_dev));
                //dev_path_ptr =  MMIAPIFMM_GetDevicePath((fs_dev));

                TM_GetSysTime(&sys_time);
                TM_GetSysDate(&sys_data);
                
                full_path_len = MIN(strlen((char *)vcal_file_name),MMIFILE_FILE_NAME_MAX_LEN);/*lint !e666*/
                sprintf((char *)&vcal_file_name[full_path_len], "%02d_%02d_%02d %02d%02d.vcs",sys_data.year,sys_data.mon,
                    sys_data.mday, sys_time.hour,sys_time.min);
                full_path_len = MIN(strlen((char *)vcal_file_name), MMIFILE_FILE_NAME_MAX_LEN);/*lint !e666*/
                MMI_STRNTOWSTR(full_path_name, MMIFILE_FILE_NAME_MAX_LEN, vcal_file_name, strlen((char *)vcal_file_name), full_path_len);
                name_ptr = (wchar *)SCI_ALLOC_APP(MMIFILE_FULL_PATH_MAX_LEN*sizeof(wchar));
				SCI_MEMSET(name_ptr, 0x00, MMIFILE_FULL_PATH_MAX_LEN*sizeof(wchar));
                full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
                MMIAPICOM_CombineDefaultFullPathByFilename(fs_dev, full_path_name, MMIAPICOM_Wstrlen(full_path_name), 
                                                   name_ptr, full_path_len);
                {
                    //创建文件

                    //MMIAPIFMM_CloseFile(file_handle);
                    MMIAPICOM_GetNoExistFileName(name_ptr,full_path_len);
                    result = VcalCreatfile((const uint16 *)name_ptr,
                        SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &file_handle);/*lint !e64 !e655*/
                    if(SFS_INVALID_HANDLE != file_handle)
                    {
                        MMIAPIFMM_CloseFile(file_handle);
                    }
                    if(VcalWritedFile(win_id, msg_id,name_ptr, vcal_buf, data_len))
                    {
                        result = MMIVCAL_NO_ERROR;
                    }
                    else
                    {
                        result = MMIVCAL_FILE_ERROR;
                    }
                }
            }//memory enough
            else
            {
                result = MMIVCAL_IS_TOOBIG;
            }//no memory
        }//if(PNULL != vcard_buf)
    }//(0 < data_len)
    //deal with save result and prompt
    if(MMIVCAL_NO_ERROR != result)
    {
        if(PNULL != vcal_buf)
        {
            SCI_FREE(vcal_buf);
#ifdef CALENDAR_SUPPORT
            MMIAPISCH_ReadVCalendarData(data_ptr, data_len);
#endif
        }
    }
    //SCI_TRACE_LOW:"MMIVCAL_SaveVCalendarByData result %d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2184_112_2_18_3_4_35_36,(uint8*)"d", result);
    return result;
}
/*****************************************************************************/
// Description : Free write file callback parameter
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void vcalFreeparam( VCARD_FILE_CALLBACK_T  *p)
{
    if (p != PNULL)
    {
        //free buf
        if (p->buf_ptr != PNULL)
        {
            SCI_FREE(p->buf_ptr);
        }
        if(p->name_ptr != PNULL)
        {
            SCI_FREE(p->name_ptr);
        }
        //free parameter
        SCI_FREE(p);
    }
    return;
}

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalWritedFile(
                               MMI_WIN_ID_T      win_id,
                               MMI_MESSAGE_ID_E  msg_id,
                               wchar            *filename,
                               uint8            *buf_ptr,
                               uint32            buflen)
{
    BOOLEAN                  result = TRUE;
    uint32                   transmitted = 0;
    MMIFILE_ERROR_E          ffs_error = SFS_NO_ERROR;
    MMIFILE_OVERLAPPED_T     overLapped = {0};
    MMIFILE_HANDLE           file_handle = SFS_INVALID_HANDLE;

    VCARD_FILE_CALLBACK_T   *callback_ptr = PNULL;
    //入口参数检查
    if((PNULL == filename) || (PNULL == buf_ptr))
    {
        //SCI_TRACE_LOW:"VcalWritedFile 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2231_112_2_18_3_4_35_37,(uint8*)"d", filename);
    }
    else
    { //创建文件
	    ffs_error = VcalCreatfile((const uint16 *)filename,
	                               SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &file_handle);/*lint !e655*/
	   
	    //异步写文件数据
	    if(SFS_INVALID_HANDLE != file_handle)
	    {
	        //传递参数的赋值
			ffs_error = MMIAPIFMM_SetFileSize(file_handle, buflen);
			if(ffs_error == SFS_NO_ERROR)
			{
				callback_ptr = (VCARD_FILE_CALLBACK_T *)SCI_ALLOC_APP(sizeof(VCARD_FILE_CALLBACK_T));
				callback_ptr->handle    = file_handle;
				callback_ptr->win_id    = win_id;
				callback_ptr->msg_id    = msg_id;
				callback_ptr->result    = VCARD_FS_WRITE_SUCCEED;
				callback_ptr->buf_ptr   = buf_ptr;
				callback_ptr->buf_len   = buflen;
				callback_ptr->name_ptr  = filename;
				overLapped.param = (SFS_PARAM)callback_ptr;
				overLapped.complete_route = MMIVC_WriteFile_CallBack;
				ffs_error = MMIAPIFMM_WriteFile(file_handle,
					callback_ptr->buf_ptr,
					callback_ptr->buf_len,
					&transmitted, &overLapped);
				//SCI_TRACE_LOW:"VcalWritedFile: ffs_error=%d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2259_112_2_18_3_4_35_38,(uint8*)"d", ffs_error);
				if (ffs_error != SFS_ERROR_IO_PENDING &&
					ffs_error != SFS_NO_ERROR)
				{
					result = FALSE;
					//if(SFS_INVALID_HANDLE != file_handle)
					{
						MMIAPIFMM_CloseFile(file_handle);
					}
					//MMIAPIFMM_DeleteFile((const uint16 *)filename, PNULL);
					//free all
					vcalFreeparam(callback_ptr);
				}
			}
			else
			{
				//no space
				//if(SFS_INVALID_HANDLE != file_handle)
				{
					MMIAPIFMM_CloseFile(file_handle);
					MMIAPIFMM_DeleteFile((const uint16 *)filename, PNULL);
				}
				
				result = FALSE;
			}
	    }
	    else
	    {
	        result = FALSE;
	    }
	}
    return result;
}

/*****************************************************************************/
// Description : 判断保存文件所需空间是否不足
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VcalCheckDiskSpaceForSaveVCalendar(MMIFILE_DEVICE_E file_dev, uint32 data_size)
{
    BOOLEAN    bRet = TRUE;
    uint32     space = 0;
    wchar*     dev_path_ptr = PNULL;
    uint16     dev_len = 0;

    if(MMI_DEVICE_NUM ==file_dev)
    {
        return FALSE;
    }
    dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    dev_path_ptr =  MMIAPIFMM_GetDevicePath((file_dev));

    if(!MMIAPIFMM_GetDeviceStatus(dev_path_ptr, dev_len))
    {
        return FALSE;

    }
    
    space = MMI_GetFreeSpace(file_dev);
    if ((data_size >>10) > space)
    {
        bRet = FALSE;
    }
    return bRet;
}

/*****************************************************************************/
// Description : 处理保存文件的操作
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIVCAL_HandleWriteCNF(MMI_MESSAGE_ID_E  msg_id,
                                 DPARAM            param)
{
    VCARD_FILE_CALLBACK_T   *result_ptr = PNULL;


    //获得参数
    if(PNULL != param)
    {
        result_ptr = (VCARD_FILE_CALLBACK_T *)param;
    }
    if (PNULL != result_ptr)
    {
        //SCI_TRACE_LOW:"MMIVCAL_HandleWriteCNF result= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2342_112_2_18_3_4_35_39,(uint8*)"d",result_ptr->result);
    }
    //关闭等待窗口
    /*if (PNULL != result_ptr && result_ptr->isWaiting)
    {
        MMIPUB_CloseWaitWin(MMIPB_VCARD_WRITEFILE_WAITING_WIN_ID);
    }*/
    //处理保存结果
    switch(msg_id)
    {
    case MSG_VCALENDAR_SAVE_SCH_CNF:
        MMIVCAL_VcalBuildExit();
		MMIVCAL_FreeFileNamePtr();
        if(SFS_INVALID_HANDLE != s_vcal_handle_result)
        {
            MMIAPIFMM_CloseFile(s_vcal_handle_result);
        }
        if (PNULL != result_ptr)
		{
			//SCI_TRACE_LOW:"MMIVCAL_HandleWriteCNF result %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2361_112_2_18_3_4_35_40,(uint8*)"d",result_ptr->result);
		}
#ifdef CALENDAR_SUPPORT
		MMIAPISCH_VCalendarSavedNotify(TRUE);
#endif
        break;
    case MSG_VCALENDAR_WRITE_SMS_CNF:
        if (PNULL != result_ptr)
        {
            //SCI_TRACE_LOW:"MMIVCAL_HandleWriteCNF result_ptr->result=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCALENDAR_2368_112_2_18_3_4_35_41,(uint8*)"d",result_ptr->result);
            //保存成功
            if (VCARD_FS_WRITE_SUCCEED == result_ptr->result)
            {
                MMIAPISMS_ShowNewVcardOrVcalendarPrompt(TRUE,MMIVC_SMS_SEND_VCALENDAR);
                if (PNULL != result_ptr->name_ptr)
                {
                    MMK_SendMsg(MMISMS_NEW_VCALENDAR_MSG_WIN_ID,
                                MSG_SMS_VCARD_FILE_NAME,
                                &(result_ptr->name_ptr));
                }
            }
            else
            {
                //free name string
                if (PNULL != result_ptr->name_ptr)
                {
                    SCI_FREE(result_ptr->name_ptr);
                }
            }
            //关闭文件
            if(SFS_INVALID_HANDLE != result_ptr->handle)
            {
                MMIAPIFMM_CloseFile(result_ptr->handle);
            }
        }
        break;
    default:
        break;
    }
    return;
}
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
									   )
{
    return -1;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_AddVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             *uuid_ptr 
									   )
{
    return -1;
}
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_ModifyVcalItem(uint8              *item_ptr,
									   uint32             item_len,
									   int32             uuid
									   )
{
    return -1;
}
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC int32 MMIVCAL_RemoveVcalItem(int32  uuid)
{
    return -1;
}
#endif //#if defined MMI_VCALENDAR_SUPPORT

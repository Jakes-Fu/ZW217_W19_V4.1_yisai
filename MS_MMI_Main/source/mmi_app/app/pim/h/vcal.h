/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : D:\T3901\t3901_0706\MS_MMI\source\mmi_app\app\pim\h\vCal.h
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-7 ,wangxiaolin,create originally.
*
*******************************************************************************/

#ifndef _VCALENDAR_
#define _VCALENDAR_

#include "List.h"
#include "vcard.h"
#include "mmipim.h"

#define BEGIN_VCALENDAR_DELIMITER	"BEGIN:VCALENDAR"
#define END_VCALENDAR_DELIMITER		"END:VCALENDAR"
#define BEGIN_VEVENT_DELIMITER		"BEGIN:VEVENT"
#define END_VEVENT_DELIMITER		"END:VEVENT"
#define BEGIN_VTODO_DELIMITER		"BEGIN:VTODO"
#define END_VTODO_DELIMITER			"END:VTODO"
#define PRODUCT_ID_PROPERTY			"\r\nPRODID"
#define DTSTART_PROPERTY			"\r\nDTSTART"
#define DTEND_PROPERTY				"\r\nDTEND"
#define DESCRIPTION_PROPERTY		"\r\nDESCRIPTION"
#define LOCATION_PROPERTY			"\r\nLOCATION"
#define SUMMARY_PROPERTY			"\r\nSUMMARY"
#define PRIORITY_PROPERTY			"\r\nPRIORITY"
#define UID_PROPERTY				"\r\nUID"
#define SEQUENCE_PROPERTY			"\r\nSEQUENCE"

typedef VCARD_ENCODING_TYPE					IRMC_ENCODING_TYPE;
typedef VCARD_VERSION_PROPERTY_TYPE			IRMC_VERSION_PORPERTY_TYPE;
typedef VCARD_PROPERTY_TYPE					IRMC_PROPERTY_TYPE;
typedef VCARD_VALUE_LOCATION_TYPE			IRMC_VALUE_LOCATION_TYPE;
typedef VCARD_TIME_ZONE_PROPERTY_TYPE		IRMC_TIME_ZONE_PROPERTY_TYPE;
typedef VCARD_GEOGRAPHIC_PROPERTY_TYPE		IRMC_GEOGRAPHIC_PROPERTY_TYPE;

typedef VCARD_PROPERTY_TYPE					VCALENDAR_PROPERTY_TYPE;
typedef VCARD_VERSION_PROPERTY_TYPE			VCALENDAR_VERSION_PROPERTY_TYPE;
typedef VCARD_GEOGRAPHIC_PROPERTY_TYPE		VCALENDAR_GEOGRAPHIC_PROPERTY_TYPE;
typedef VCARD_TIME_ZONE_PROPERTY_TYPE		VCALENDAR_TIME_ZONE_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*attachment;
}VCALENDAR_ATTACHEDMENT_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*prodid;
}VCALENDAR_PRODID_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*dtStart;
}VCALENDAR_DTSTART_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*dtEnd;
}VCALENDAR_DTEND_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*description;
}VCALENDAR_DESCRIPTION_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*location;
}VCALENDAR_LOCATION_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*summary;
}VCALENDAR_SUMMARY_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*priority;
}VCALENDAR_PRIORITY_PROPERTY_TYPE;

typedef struct
{
	VCALENDAR_PROPERTY_TYPE		property;
	char						*uid;
}VCALENDAR_UID_PROPERTY_TYPE;



typedef struct
{
	VCALENDAR_ATTACHEDMENT_PROPERTY_TYPE	vCalAttachment;
	VCALENDAR_DTSTART_PROPERTY_TYPE			vCalDtstart;
	VCALENDAR_DTEND_PROPERTY_TYPE			vCalDtend;
	VCALENDAR_DESCRIPTION_PROPERTY_TYPE		vCalDescription;
	VCALENDAR_LOCATION_PROPERTY_TYPE		vCalLocation;
	VCALENDAR_SUMMARY_PROPERTY_TYPE			vCalSummary;
	VCALENDAR_PRIORITY_PROPERTY_TYPE		vCalPriority;
	VCALENDAR_UID_PROPERTY_TYPE				vCalUid;
}VEVENT_PROPERTY_TYPE;


typedef VEVENT_PROPERTY_TYPE VTODO_PROPERTY_TYPE;



typedef struct
{
	VCALENDAR_VERSION_PROPERTY_TYPE			vCalVer;
	VCALENDAR_GEOGRAPHIC_PROPERTY_TYPE		vCalGeographic;
	VCALENDAR_TIME_ZONE_PROPERTY_TYPE		vCalTimezone;
	VCALENDAR_PRODID_PROPERTY_TYPE			vCalProdid;
	ListType								vCalEvent;
	ListType								vCalTodo;
}VCALENDAR_TYPE;

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.8.4
//status
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *status;
}VCALENDAR_STATUS_PROPERTY_TYPE;

//categories
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *categories[12];
}VCALENDAR_CATE_PROPERTY_TYPE;

//status
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *due;
}VCALENDAR_DUE_PROPERTY_TYPE;

//status
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *completed;
}VCALENDAR_COMPLETED_PROPERTY_TYPE;

//DALARM
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *aalarm;
}VCALENDAR_AALARM_PROPERTY_TYPE;

//RRULE
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *rrule;
}VCALENDAR_RRULE_PROPERTY_TYPE;

//class
typedef struct
{
    VCALENDAR_PROPERTY_TYPE      property;
    char                                         *cclass;
}VCALENDAR_CLASS_PROPERTY_TYPE;

//vcalendar �ṹ
typedef struct
{
    //����
    VCALENDAR_VERSION_PROPERTY_TYPE              vCalVer;
    VCALENDAR_DTSTART_PROPERTY_TYPE              vCalDtstart;//vevent ����
    VCALENDAR_DTEND_PROPERTY_TYPE                 vCalDtend;//vevent ����
    VCALENDAR_DUE_PROPERTY_TYPE                     vCalDue;//vtodo ����
    VCALENDAR_COMPLETED_PROPERTY_TYPE          vCalCompleted;//vtodo ����
    VCALENDAR_DESCRIPTION_PROPERTY_TYPE        vCalDescription;
    VCALENDAR_SUMMARY_PROPERTY_TYPE             vCalSummary;
    VCALENDAR_PRIORITY_PROPERTY_TYPE	             vCalPriority;
    VCALENDAR_STATUS_PROPERTY_TYPE                vCalStatus;
    VCALENDAR_CATE_PROPERTY_TYPE                   vCalCategories;
    //ListType                                vCalEvent;
    //ListType                                vCalTodo;
    
    //��ѡ
    /*vCaldalarm:vCalendar������ָ�ճ̵��������ã���������ʱ��RunTime���ظ�����
    ֮���ʱ����SnoozeTime���ظ����Ѵ���RepeatCount����������DispalyString��*/
    VCALENDAR_AALARM_PROPERTY_TYPE                   vCalaalarm;
    //ָ��vCalendar������ظ�����
    VCALENDAR_RRULE_PROPERTY_TYPE                   vCalRrule;
    VCALENDAR_CLASS_PROPERTY_TYPE                   vCalClass;//�ȼ�
    VCALENDAR_TIME_ZONE_PROPERTY_TYPE           vCalTimezone;//����ʱ��
    VCALENDAR_PRODID_PROPERTY_TYPE                vCalProdid;//����vCalendar����������Ʒ�ı�ʶ
    VCALENDAR_ATTACHEDMENT_PROPERTY_TYPE     vCalAttachment;//����vCalendar����ĸ���λ��
    VCALENDAR_LOCATION_PROPERTY_TYPE             vCalLocation;//vCalendar����Ԥ�ڷ����ĵص�
    VCALENDAR_GEOGRAPHIC_PROPERTY_TYPE         vCalGeographic;//���Ե���λ�ã��ֶ�ֵ�ĸ�ʽΪ��<����>','<γ��>
}VCALENDAR_INFO_T;
#endif /* HS_PIM_VCAL */

#endif

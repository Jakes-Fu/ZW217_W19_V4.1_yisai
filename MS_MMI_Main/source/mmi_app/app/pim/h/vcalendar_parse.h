/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	filename		: Vcalendar_parse.h
 *	author		: wangzhefeng    
 *	description	: 
 *	vesion		:         
 *	date			: 2009-7-29  
 *	history		: 
 * --------    ---------    ----------  ------------------------------------
 *  wangzhefeng 2009-7-29                      ������д���
 *
 *
 *====*====*====*====*====*====*====*====*====*====*====*====*/


#ifndef _VCALENDAR_PARSE_H_
#define _VCALENDAR_PARSE_H_


/*====*====*====*====*====*====*====*====*====*====*====*====*
 *                         Include Files                                    *
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#include "mn_type.h"
//#include "Vf_iface.h"
#include "mmiacc_event.h"
#include "vCal.h"
#include "mmipim.h"
#include "PIMsmldtd.h"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.7.31

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
extern const wchar g_mmivcal_default_dir_wstr[];

#define VCALENDAR_MAX_LEN                  2000
#define VCALENDAR_NAME_MAX_LEN         MMIFILE_FILE_NAME_MAX_LEN       //vCalendar�ļ�������󳤶�
//#define VCALENDAR_REV_MAX_LEN          20       //vCalendar �ļ���REVֵ����󳤶�
#define VCALENDAR_MAX_FILE_LEN         (5*1024) //��֧��vCalendar�ļ�����󳤶�
#define VCALENDAR_DEFAULT_PREFIX       "vcal"   //vCalendar�ļ���ͷ
#define VCALENDAR_DEFAULT_SUBFIX       ".vcs"   //vCalendar�ļ���׺��
#define VCALENDAR_DEFAULT_DIR          (g_mmivcal_default_dir_wstr)//"\x00\x76\x00\x43\x00\x61\x00\x72\x00\x64\x00\x00"
#define VCALENDAR_DEFAULT_DIR_LEN      9

#define VCALENDAR_DESC_MAX_LEN          500//wangzhefeng mod 20090924
#define VCALENDAR_MAX_SMS_DATA_LENGTH  (2*1024)

#define VCALENDAR_LONG_SMS_FILE_NAME   "longsm.dat"
#define VCALENDAR_MIN_NEED_SPACE        2048    //����Vcard��С��Ҫ�ռ�



#define VCAL_BEGIN_VCALENDAR	      "BEGIN:VCALENDAR"
#define VCAL_END_VCALENDAR		"END:VCALENDAR"
#define VCAL_BEGIN_VEVENT		"BEGIN:VEVENT"
#define VCAL_END_VEVENT		"END:VEVENT"
#define VCAL_BEGIN_VTODO		"BEGIN:VTODO"
#define VCAL_END_VTODO			"END:VTODO"
#define VCAL_VERSION			"\r\nVERSION"
#define VCAL_CATEGORIES_PROPERTY			"\r\nCATEGORIES"
//#define VCAL_PRODUCT_ID_PROPERTY			"\r\nPRODID"
#define VCAL_DTSTART_PROPERTY			"\r\nDTSTART"
#define VCAL_DTEND_PROPERTY				"\r\nDTEND"
#define VCAL_DESCRIPTION_PROPERTY		"\r\nDESCRIPTION"
#define VCAL_STATUS_PROPERTY			"\r\nSTATUS"
#define VCAL_LOCATION_PROPERTY			"\r\nLOCATION"
#define VCAL_SUMMARY_PROPERTY			"\r\nSUMMARY"
#define VCAL_PRIORITY_PROPERTY			"\r\nPRIORITY"
#define VCAL_COMPLETED_PROPERTY			"\r\nCOMPLETED"//VTODO����
#define VCAL_DUE_PROPERTY			             "\r\nDUE"//VTODO����

//��ѡ
#define VCAL_CLASS_PROPERTY			"\r\nCLASS"
#define VCAL_RRULE_PROPERTY			"\r\nRRULE"
#define VCAL_AALARM_PROPERTY			"\r\nAALARM"

#define VCAL_ENCODING                          "ENCODING="
#define VCAL_CHARSET                            "CHARSET="
#define VCAL_LANGUAGE                          "LANGUAGE="
#define VCAL_VALUE                                "VALUE="
#define VCAL_TYPE                                  "TYPE="
#define VCAL_BASE_64_ENCODING	          "BASE64"
#define VCAL_QUOTED_PRINTABLE_ENCODING    "QUOTED-PRINTABLE"
#define VCAL_EIGHT_BIT_ENCODING       "8-BIT"
#define VCAL_UTF8_CHARSET                  "UTF-8"





#define VCAL_LFCR						"\r\n"
#define VCAL_DELIMITERS					";"
#define VCAL_SEMICOLON					":"
#define VCAL_LFCRLWSP					"\r\n "


#define MMIPIM_MAX_VCAL_NUM   MMIACC_SCH_NUM

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

//vcalendar���������
typedef enum
{
    VCAL_TYPE_VEVENT,//VEVENT
    VCAL_TYPE_VTODO,//VTODO
    VCAL_TYPE_MAX
}VCAL_OBJ_TYPE_E;

//PRIORITY 
typedef enum  
{
    VCAL_PRIO_HIGH,//0Ϊ������ȼ�
    VCAL_PRIO_NORM,
    VCAL_PRIO_LOW,
    VCAL_PRIO_MAX
}VCAL_PRIO_TYPE_E;

//CATEGORIES 
typedef enum  
{
    VCAL_CATE_APPOINTMENT,//APPOINTMENT
    VCAL_CATE_BUSINESS,//BUSINESS
    VCAL_CATE_EDUCATION,//EDUCATION
    VCAL_CATE_HOLIDAY,//HOLIDAY
    VCAL_CATE_MEETING,//MEETING
    VCAL_CATE_MISCELLANEOUS,//MISCELLANEOUS
    VCAL_CATE_PERSONAL,//PERSONAL
    VCAL_CATE_PHONE_CALL,//PHONE CALL
    VCAL_CATE_SICK_DAY,//SICK DAY
    VCAL_CATE_SPECIAL_OCCASION,//SPECIAL OCCASION
    VCAL_CATE_TRACEL,//TRACEL
    VCAL_CATE_VACATION,//VACATION
    VCAL_CATE_MAX
}VCAL_CATE_TYPE_E;

//STATUS 
typedef enum  
{
    VCAL_STATUS_ACCEPTED,
    VCAL_STATUS_NEEDS_ACTION,//Ĭ��Ϊneed action
    VCAL_STATUS_SENT,
    VCAL_STATUS_TENTATIVE,
    VCAL_STATUS_CONFIRMED,
    VCAL_STATUS_DECLINED,
    VCAL_STATUS_COMPLETED,
    VCAL_STATUS_DELEGATED,
    VCAL_STATUS_MAX
}VCAL_STATUS_TYPE_E;

//AALARM �����ļ�������(��ѡ)
typedef enum  
{
    VCAL_AALARM_PCM,
    VCAL_AALARM_WAVE,
    VCAL_AALARM_AIFF,
    VCAL_AALARM_MAX //û������
}VCAL_AALARM_TYPE_E;

typedef enum  
{
    VCAL_NO_ERROR,
    VCAL_EMPTY_FILE,
    VCAL_TOBIG_FILE,
    VCAL_FILE_ERROR
}VCAL_ERROR_E;


typedef struct
{
    char *summary;            //SUMMARY
    uint16 summaryLen;          // title���ֽڳ���(��u16�ĸ���)
    char *desc;          //DESCRIPTION
    uint16 descLen;        // desc���ֽڳ���(��u16�ĸ���)
    VCAL_PRIO_TYPE_E prio;      //���ȼ�0Ϊ���
    BOOLEAN cate[VCAL_CATE_MAX];     //CATEGORIES
    VCAL_STATUS_TYPE_E status;          //STATUS
    char dtEnd[20];         //eventʹ��
    char dtStart[20];       // ����Eventʹ��
    char dtDue[20];        // ����Todoʹ��
    char complete[20];        // ����Todoʹ��
    char *loc;            // ����Eventʹ��(��ѡ)
    uint8 locLen;          // loc���ֽڳ���(��u16�ĸ���)(��ѡ)	
    //VcalRule rule;				// rule�Ƿ���Чͨ���� type��Ա�ж�
    //char dtAlarm[20];
    //char alarmFile[256]; 	// �����ļ���
    //VCAL_PRIO_TYPE_E  alarmType;    // indicate the type of the alarm audio file
    //cns_u32 alarmDuration; 		// duration time of this alarm
}VCAL_INFO_T;

typedef struct _MMIPIM_VCAL_LOG_INF
{
    BOOLEAN is_valid;
    uint8 * vcal_status_list_ptr;
} MMIPIM_VCAL_LOG_INF;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIAPIVCAL_BuildVcal
 * decr      : ����vcalendar����
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  7/30/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIAPIVCAL_BuildVcal( MMIACC_SMART_EVENT_T* event_ptr ,uint32 index,VCAL_OBJ_TYPE_E type);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIAPIVCAL_ImportVCalOne
 * decr      : ��vcalendar�ļ����룬ת��Ϊ�ճ�
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/2/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIAPIVCAL_ImportVCalOne(wchar *file_name,uint16 name_len);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetPimStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC VCAL_PIM_STATUS_E MMIVCAL_GetPimStatus(   //RETURN: PIM״̬
                                uint16   entry_id   //IN: ��¼��
                                );

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_InitVCALStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC void  MMIPIM_InitVCALStatus(void);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_UpdatePimStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
 PUBLIC BOOLEAN MMIVCAL_UpdatePimStatus(  //RETURN: TRUE,�����ɹ���FALSE,����ʧ��
                                uint16    entry_id,    //IN: ��¼��
                                VCAL_PIM_STATUS_E    pim_status    //IN: PIM״̬
                                );

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_UpdateAllPimStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC void MMIVCAL_UpdateAllPimStatus(
                                    VCAL_PIM_STATUS_E   pim_status  //IN: PIM״̬
                                    );

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetRecentPBItemsCount
 * decr      : ���add\del\mod��״̬����
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetRecentVCALItemsCount(VCAL_PIM_STATUS_E  pim_status);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetAllVCALItemsCount
 * decr      : ����Ѿ��洢���ճ�item��
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetAllVCALItemsCount(void);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetFreeItemsCount
 * decr      : ����ճ̵�ʣ��itme��
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetFreeItemsCount(void);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_GetVcalItem
 * decr      : ���ݴ����id�ɶ����ճ���Ϣ��ת��ΪvCalendar buffer����
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/7/2009 
 ==========================================================*/
PUBLIC int MMIPIM_GetVcalItem(
                                    uint16 entry_id , //IN: ��¼��
                                    char* buffer,      // OUT: �����vCalendar
                                    uint32 buffersize //IN: ��������С
                                    );

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_IsScheItemValid
 * decr      : ��ô˼�¼�ŵ�item�Ƿ�Ϊ��
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIPIM_IsScheItemValid(
                                    uint16 entry_id //IN: ��¼��
                                    );

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_Add
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_Add(short id, void* userData, SmlAddPtr_t pContent);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_replace
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_replace(short id, void* userData, SmlReplacePtr_t pContent);

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_delete
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_delete(short id, void* userData, SmlDeletePtr_t pContent);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif/*HS_PIM_VCAL*/

#endif

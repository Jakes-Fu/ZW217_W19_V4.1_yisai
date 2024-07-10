/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	filename		: Vcalendar_parse.c
 *	author		: wangzhefeng    
 *	description	: 
 *	vesion		:         
 *	date			: 2009-7-29  
 *	history		: 
 * --------    ---------    ----------  ------------------------------------
 *  wangzhefeng 2009-7-29                      初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/


/*====*====*====*====*====*====*====*====*====*====*====*====*
 *                         Important Edit History                            *
 * --------------------------------------------------------------------------*
 * DATE                  NAME             DESCRIPTION                               *
 * 2009.07.29      wangzhefeng          Create
 *====*====*====*====*====*====*====*====*====*====*====*====*/


/*====*====*====*====*====*====*====*====*====*====*====*====*
 *                         Include Files                                    *
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#include "mmi_app_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "Vcalendar_parse.h"
//#include "mmisd_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "Gui_ucs2b_converter.h"
#include "mmk_app.h"
#include "mmiacc_event.h"
#include "vcard_parse.h"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.7.29 //clean pclint warning.
#include "mmischedule_export.h"
#endif
#include "mmipim_nv.h"
#include "mmialarm_export.h"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.7.29 //clean pclint warning.
#include "PIMlibmem.h"//wangzhefeng add 20091105
#endif
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.7.29

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define  MMIVCALENDAR_RENAME_MAX_TIMES			   500
#define  MMIVCALENDAR_MAX_LEN			   1000



/*---------------------------------------------------------------------------*/
/*                          STATIC DEFINITION                                */
/*---------------------------------------------------------------------------*/

const wchar g_mmivcal_default_dir_wstr[]={'v','C','a','l','e','n','d','a','r',0};

#if 0//del by wangzhefeng  2009.12.18
//vcalendar 的CATEGORIES 种类，可以多选
const static  char* s_vcal_cate_type[VCAL_CATE_MAX]=
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

const static  char* s_vcal_state_type[VCAL_STATUS_MAX]=
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
#endif

LOCAL uint8* s_vcalendar_ptr = NULL;

LOCAL uint8 g_vcal_status_arr[MMIPIM_MAX_VCAL_NUM]={0};
LOCAL MMIPIM_VCAL_LOG_INF  g_vcal_log_inf = {FALSE,g_vcal_status_arr};

extern status_cmd_queue_type*  hs_status_cmd_que;

extern SmlNewMapItemListPtr_t  hs_globlemapItemList;


/*==========================================================
 * author    : wangzhefeng      
 * function  : VCAL_StrUpr
 * decr      : 由小写转换为大写
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/7/2009 
 ==========================================================*/
static char *VCAL_StrUpr(char *inPtr)
{
    unsigned int i;

    if (!inPtr)
        return NULL;

    for (i=0; i<strlen(inPtr); i++)
    {
        /* a..z ==> A..Z */
        if ((inPtr[i] >= 'a') && (inPtr[i] <= 'z'))
            inPtr[i] -= 0x20;
    }

    return inPtr;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : VCAL_ExchangeDelimiter
 * decr      : 将字段值中的";"转换成"\"
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/17/2009 
 ==========================================================*/
static wchar *VCAL_ExchangeDelimiter(wchar *inPtr,uint16 content_len)
{
    unsigned int i;

    if (!inPtr)
        return NULL;

    //syncml_cb_message:"MMIPIM @@ VCAL_ExchangeDelimiter enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_138_112_2_18_2_47_17_219,(uint8*)"");
    for (i=0; i<content_len; i++)
    {
        /* ';' ==> '\' */
        if (inPtr[i] == (uint16)0x3b)
        {
            //syncml_cb_message:"MMIPIM @@ VCAL_ExchangeDelimiter delimiter exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_144_112_2_18_2_47_17_220,(uint8*)"");
            inPtr[i] ='\x3b';
        }
    }

    return inPtr;
}



/*==========================================================
 *
 *由schedule 日程转换为vcalendar数据
 *
 ==========================================================*/


/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_BuildFree
 * decr      : vcalendar 建立完成，释放空间
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  7/29/2009 
 ==========================================================*/
static void MMIVCAL_BuildFree(void)
{
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildFree start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_170_112_2_18_2_47_17_221,(uint8*)"");
    if ( PNULL != s_vcalendar_ptr )
    {
        hs_smlLibFree( s_vcalendar_ptr );
        //syncml_cb_message:"MMIPIM @@ s_vcalendar_ptr free succeed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_174_112_2_18_2_47_17_222,(uint8*)"");
        s_vcalendar_ptr = PNULL;
    }
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_BuildInit
 * decr      : vcalendar 初始化
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  7/29/2009 
 ==========================================================*/
static BOOLEAN MMIVCAL_BuildInit(void)
{
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildInit start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_189_112_2_18_2_47_17_223,(uint8*)"");
    MMIVCAL_BuildFree();
    s_vcalendar_ptr = (uint8*)hs_smlLibMalloc(__FILE__, __LINE__,(long)MMIVCALENDAR_MAX_LEN);
    if(PNULL == s_vcalendar_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET(s_vcalendar_ptr,0,MMIVCALENDAR_MAX_LEN);
    return TRUE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_BuildVcal
 * decr      : 形成vcalendar buffer
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  7/31/2009 
 ==========================================================*/
LOCAL BOOLEAN MMIVCAL_BuildVcal(MMIACC_SMART_EVENT_T* event_ptr,uint16 index,VCAL_OBJ_TYPE_E type)
{
    uint8          *vcal_ptr = s_vcalendar_ptr;
    uint32         vcal_length = 0;
    uint16 temp_len = 0;
    uint8 temp_buf[VCALENDAR_DESC_MAX_LEN] = {0};
    char time_str[20]={0};
    char qp_ptr[VCALENDAR_DESC_MAX_LEN] = {0};
    uint16 ucs2_len = 0;
#if 0//del by wangzhefeng  2009.12.17
    uint8           i =0;
    BOOLEAN  have_cate = FALSE;
    BOOLEAN vcal_cate[VCAL_CATE_MAX] = {0};
    VCAL_PRIO_TYPE_E vcal_prio = VCAL_PRIO_MAX;
    VCAL_STATUS_TYPE_E vcal_status = VCAL_STATUS_MAX;
    char vcal_class[] = "PRIVATE";//PRIVATE\PUBLIC\CONFIDENTIAL,默认为PUBLIC
    
    //set priority优先级
    vcal_prio = VCAL_PRIO_NORM;
    //set categaries 
    vcal_cate[1] = TRUE;
    //vcal_cate[0] = TRUE;
    //set status
    vcal_status = VCAL_STATUS_MAX;//默认为needs action
#endif
     //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal begin!"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_234_112_2_18_2_47_17_224,(uint8*)"");
       
    //vcalendar开始
    strcat ((char*)vcal_ptr, VCAL_BEGIN_VCALENDAR);
    //strcat ((char*)vcal_ptr, VCAL_LFCR);
    
    //版本支持vcalendar1.0
    strcat ((char*)vcal_ptr, VCAL_VERSION);
    strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
    strcat ((char*)vcal_ptr, "1.0\r\n");
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal verson add!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_244_112_2_18_2_47_18_225,(uint8*)"");
    //strcat ((char*)vcal_ptr, "VERSION:1.0\r\n");

    //vcalendar对象VEVENT 或者VTODO
    if(VCAL_TYPE_VEVENT == type)
    {
        strcat ((char*)vcal_ptr, VCAL_BEGIN_VEVENT);
    }
    else
    {
        strcat ((char*)vcal_ptr, VCAL_BEGIN_VTODO);
    }
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal vevent add!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_256_112_2_18_2_47_18_226,(uint8*)"");
#if 0//Added by wangzhefeng  2009.9.7
    strcat ((char*)vcal_ptr, VCAL_LFCR);
    
    //厂家扩展字段，必须，扩展为id
    strcat ((char*)vcal_ptr, "X-HSMC-LUID:");
    MMIAPICOM_Int2Str(index,str_buf,VCALENDAR_DESC_MAX_LEN);
    strcat ((char*)vcal_ptr, (char*)str_buf);
    vcal_length = strlen((char*)vcal_ptr);
    vcal_ptr[vcal_length]=0;
#endif//Added by wangzhefeng  2009.9.7
    
#if 0//Added by wangzhefeng  2009.12.10
    // build the property CATEGORIES of the vCal 
    for(i=0;i<VCAL_CATE_MAX;i++)
    {
        if(TRUE == vcal_cate[i])
        {
            have_cate = TRUE;
            break;
        }
    }
    if(TRUE == have_cate)
    {//categories 如果有值，则写入，否则不写入
        have_cate = FALSE;
        strcat ((char*)vcal_ptr, VCAL_CATEGORIES_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        for(i=0;i<VCAL_CATE_MAX;i++)
        {
            if(TRUE == vcal_cate[i])
            {
                if(TRUE == have_cate)//第一个categaries属性之前没有";"
                {
                    strcat((char*)vcal_ptr,VCAL_DELIMITERS);
                }
                have_cate = TRUE;
                strcat((char*)vcal_ptr,s_vcal_cate_type[i]);
            }
        }
        //strcat ((char*)vcal_ptr, VCAL_LFCR);
    }
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal categories add!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_297_112_2_18_2_47_18_227,(uint8*)"");

    // build the property PRIORITY of the vCal 
    if(VCAL_PRIO_MAX > vcal_prio)/*lint !e774*/
    {
        char vcal_prio_arr[2]={0};
        strcat ((char*)vcal_ptr, VCAL_PRIORITY_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        vcal_prio += '0';
        vcal_prio_arr[0]=vcal_prio;
        vcal_prio_arr[1]=0;
        strcat ((char*)vcal_ptr, (char*)vcal_prio_arr);
    }
    
    //class
    if(NULL!=vcal_class)/*lint !e774 !e506*/
    {
        strcat ((char*)vcal_ptr, VCAL_CLASS_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        strcat ((char*)vcal_ptr, (char*)(vcal_class));
    }
    
    // build the property STATUS of the vCal 
    if(VCAL_STATUS_MAX!=vcal_status)/*lint !e774*/
    {
        strcat ((char*)vcal_ptr, VCAL_STATUS_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        if(VCAL_STATUS_MAX==vcal_status)/*lint !e774*/
        {
            vcal_status = VCAL_STATUS_NEEDS_ACTION;
        }
        strcat ((char*)vcal_ptr,s_vcal_state_type[vcal_status] );
    }
#endif//Added by wangzhefeng  2009.12.10
    
    //将英文分号转换为'\'
    VCAL_ExchangeDelimiter(event_ptr->event_content_info.w_content,event_ptr->event_content_info.w_length);
    
    temp_len = GUI_WstrToUTF8(temp_buf,
                VCALENDAR_DESC_MAX_LEN,
                event_ptr->event_content_info.w_content,
                MIN(event_ptr->event_content_info.w_length,MMIACC_EVENT_TEXT_MAX_LEN));
#if 0//Added by wangzhefeng  2009.9.17
                event_ptr->event_content_info.w_length);
#endif//Added by wangzhefeng  2009.9.17
    ucs2_len = EncodeQuoted((unsigned char*)temp_buf, qp_ptr, temp_len);
    
    // build the property SUMMARY of the vCal 
    //if(ucs2_len!=0)//wangzhefeng del 20100105
    {
        //utf8->qp
        strcat ((char*)vcal_ptr, VCAL_SUMMARY_PROPERTY);
        strcat ((char*)vcal_ptr, ";ENCODING=QUOTED-PRINTABLE;CHARSET=UTF-8:");
        strcat ((char*)vcal_ptr, qp_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal summery add!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_351_112_2_18_2_47_18_228,(uint8*)"");
    }

    // build the property description of the vCal 
#if 0//Added by wangzhefeng  2009.9.10
    if(ucs2_len!=0)
    {
        strcat ((char*)vcal_ptr, VCAL_DESCRIPTION_PROPERTY);
        strcat ((char*)vcal_ptr, ";ENCODING=QUOTED-PRINTABLE;CHARSET=UTF-8:");
        strcat ((char*)vcal_ptr, qp_ptr);
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal description add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_362_112_2_18_2_47_18_229,(uint8*)"d",vcal_length);
    }
#endif//Added by wangzhefeng  2009.9.10

    if(VCAL_TYPE_VEVENT == type)
    {
        //dtstart
        strcat ((char*)vcal_ptr, VCAL_DTSTART_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        //dtstart
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        //SCI_MEMCPY(vcal_ptr+vcal_length,time_str,strlen(time_str));
        strcat ((char*)vcal_ptr, time_str);
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal vevent dtstart add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_382_112_2_18_2_47_18_230,(uint8*)"d",vcal_length);
        //dtend
        strcat ((char*)vcal_ptr, VCAL_DTEND_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        //dtend
        SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.end_year, 
                    event_ptr->event_fast_info.end_mon, 
                    event_ptr->event_fast_info.end_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        //SCI_MEMCPY(vcal_info.dtEnd,(char*)time_str,strlen(time_str));
        strcat ((char*)vcal_ptr, time_str);
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal vevent dtend add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_398_112_2_18_2_47_18_231,(uint8*)"d",vcal_length);
    }
    else//VTODO 对象
    {
        //due
        strcat ((char*)vcal_ptr, VCAL_DUE_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        //due
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        strcat ((char*)vcal_ptr, time_str);
        //complete
        strcat ((char*)vcal_ptr, VCAL_COMPLETED_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        //completed
        SCI_MEMSET(time_str,0,sizeof(time_str));
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.end_year, 
                    event_ptr->event_fast_info.end_mon, 
                    event_ptr->event_fast_info.end_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        strcat ((char*)vcal_ptr, time_str);
    }
    
    //rrule
    if(ALM_MODE_NO == event_ptr->event_fast_info.fre_mode)//不提醒
    {
        // 循环规则为不提醒/* wangzhefeng 2009.9.7 */
#if 0//del by wangzhefeng  2010.2.26 for 暂时去掉，实验室服务器解不出来
        //wangzhefeng mod 20100105 
        strcat((char*)vcal_ptr, "\r\nRRULE:");
        strcat((char*)vcal_ptr, "\r\nAALARM:;;;");
#endif
    }
    else if(ALM_MODE_ONCE == event_ptr->event_fast_info.fre_mode)//一次
    {
        //循环规则为一次
#if 0//del by wangzhefeng  2010.2.26 for 暂时去掉，实验室服务器解不出来
        //wangzhefeng mod 20100105
        strcat((char*)vcal_ptr, "\r\nRRULE:");
#endif
        //aalarm
        strcat ((char*)vcal_ptr, VCAL_AALARM_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);

        //wangzhefeng  2009.12.11  避免当日程爆发后由于AALARM字段变化而使慢同步时增加日程条目
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        strcat ((char*)vcal_ptr, time_str);
        strcat ((char*)vcal_ptr, ";;;");
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_461_112_2_18_2_47_18_232,(uint8*)"d",vcal_length);
    }
    else
    {
        strcat((char*)vcal_ptr, VCAL_RRULE_PROPERTY);
        strcat((char*)vcal_ptr, VCAL_SEMICOLON);
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
            //strcat((char*)vcal_ptr, " #0");
        }
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal vevent rrule add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_517_112_2_18_2_47_18_233,(uint8*)"d",vcal_length);

        //aalarm
        strcat ((char*)vcal_ptr, VCAL_AALARM_PROPERTY);
        strcat ((char*)vcal_ptr, VCAL_SEMICOLON);
        //wangzhefeng  2009.12.11  避免当日程爆发后由于AALARM字段变化而使慢同步时增加日程条目
        sprintf( time_str,
                    "%04d%02d%02dT%02d%02d00",
                    event_ptr->event_fast_info.start_year, 
                    event_ptr->event_fast_info.start_mon, 
                    event_ptr->event_fast_info.start_day,
                    event_ptr->event_fast_info.hour,
                    event_ptr->event_fast_info.minute);
        strcat ((char*)vcal_ptr, time_str);
        strcat ((char*)vcal_ptr, ";;;");
        vcal_length = strlen((char*)vcal_ptr);
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal aalarm add!vcal_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_533_112_2_18_2_47_18_234,(uint8*)"d",vcal_length);
    
    }
    
    //end vevent or vtodo
    strcat ((char*)vcal_ptr, VCAL_LFCR);
    if(VCAL_TYPE_VEVENT == type)
    {
        strcat ((char*)vcal_ptr, VCAL_END_VEVENT);
    }
    else
    {
        strcat ((char*)vcal_ptr, VCAL_END_VTODO);
    }
    strcat ((char*)vcal_ptr, VCAL_LFCR);
    
    //end vcalendar
    strcat ((char*)vcal_ptr, VCAL_END_VCALENDAR);
    strcat ((char*)vcal_ptr, VCAL_LFCR);
    vcal_length = strlen((char*)vcal_ptr);
    //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal end!vcal_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_553_112_2_18_2_47_18_235,(uint8*)"d",vcal_length);

    //需要判断是否超出
    if(vcal_length<MMIVCALENDAR_MAX_LEN)
    {
        return TRUE;
    }
    else
    {
        //syncml_cb_message:"MMIPIM @@ MMIVCAL_BuildVcal vcal_lendth is longer than 1000 "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_562_112_2_18_2_47_18_236,(uint8*)"");
        return FALSE;
    }
    
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_GetVcalItem
 * decr      : 根据传入的id由读出日程信息并转换为vCalendar buffer数据
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/7/2009 
 ==========================================================*/
PUBLIC int MMIPIM_GetVcalItem(
                                    uint16 entry_id , //IN: 记录号
                                    char* buffer,      // OUT: 输出的vCalendar
                                    uint32 buffersize //IN: 缓冲区大小
                                    )
{
    MMIACC_SMART_EVENT_T event_info = {0};
    uint32 vcalendar_len = 0;
    //syncml_cb_message:"MMIPIM @@ MMIPIM_GetVcalItem begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_584_112_2_18_2_47_18_237,(uint8*)"");
    //获得完整的日程info
    MMIAPIALM_FormInteEventInfo(entry_id+EVENT_SCH_0-1,&event_info);
    //syncml_cb_message:"MMIPIM @@ MMIPIM_GetVcalItem get event_info finished!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_587_112_2_18_2_47_18_238,(uint8*)"");
    
    //形成vcalendar数据buffer
    if(FALSE==MMIVCAL_BuildInit())
    {
        return 0;
    }
    MMIVCAL_BuildVcal(&event_info,entry_id+EVENT_SCH_0-1,VCAL_TYPE_VEVENT);
    //syncml_cb_message:"MMIPIM @@ MMIPIM_GetVcalItem change vcalendar buffer finished!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_595_112_2_18_2_47_18_239,(uint8*)"");
    
    //buffer赋值
    vcalendar_len = strlen((char*)s_vcalendar_ptr);
    if(buffersize>vcalendar_len+1)
    {
        SCI_MEMCPY(buffer ,s_vcalendar_ptr,vcalendar_len);
        buffer[vcalendar_len] = 0;
        buffer[vcalendar_len+1] = 0;
    }
    else
    {//防止内存覆盖
        SCI_MEMCPY(buffer ,s_vcalendar_ptr,buffersize);
        if(buffersize>=2)
        {
            buffer[buffersize-2] = 0;
        }
        if (buffersize>=1) 
        {
            buffer[buffersize-1] = 0;
        }
    }
    MMIVCAL_BuildFree();
    
    return MIN(vcalendar_len,buffersize);
    
}

/*==========================================================
 *
 *由vcalendar数据解析为schedule 
 *
 ==========================================================*/


/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractClassFrVcal
 * decr      : Class:PUBLIC\PRIVATE\CONFIDENTIAL
 * parameter : 可选字段
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractClassFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractClassFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_650_112_2_18_2_47_18_240,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_CLASS_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_CLASS_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalClass.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalClass.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalClass.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalClass.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalClass.cclass= (char *)hs_smlLibMalloc(__FILE__, __LINE__,(long)(valueLen+1));
        if (!vcal_info_ptr->vCalClass.cclass)
        {
            //syncml_cb_message:"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_681_112_2_18_2_47_19_241,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalClass.cclass, propertyPtr, valueLen);
        vcal_info_ptr->vCalClass.cclass[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractRruleFrVcal
 * decr      : RRULE:循环规则
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractRruleFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractRruleFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_715_112_2_18_2_47_19_242,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_RRULE_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_RRULE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalRrule.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalRrule.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalRrule.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalRrule.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalRrule.rrule= (char *)hs_smlLibMalloc(__FILE__, __LINE__,(long)(valueLen+1));
        if (!vcal_info_ptr->vCalRrule.rrule)
        {
            //syncml_cb_message:"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_746_112_2_18_2_47_19_243,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalRrule.rrule, propertyPtr, valueLen);
        vcal_info_ptr->vCalRrule.rrule[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractClassFrVcal
 * decr      : Class:PUBLIC\PRIVATE\CONFIDENTIAL
 * parameter : 可选字段
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractAalarmFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractAalarmFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_780_112_2_18_2_47_19_244,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_AALARM_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_AALARM_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalaalarm.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalaalarm.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalaalarm.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalaalarm.property.valueLocation);
    
    //dalarm字段仅解析runtime
    lfcr = strstr(headPtr, VCAL_LFCR);
    
    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalaalarm.aalarm= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalaalarm.aalarm)
        {
            //syncml_cb_message:"MMIPIM: ExtractAalarmFrVcal:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_812_112_2_18_2_47_19_245,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalaalarm.aalarm, propertyPtr, valueLen);
        vcal_info_ptr->vCalaalarm.aalarm[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractVersionFrVcal
 * decr      : Version
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractVersionFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    int ret = 0;
    if ((NULL==buffer) || (NULL==vcal_info_ptr))
    {
        //syncml_cb_message:"MMIPIM: ExtractVersionFrVcard buff is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_839_112_2_18_2_47_19_246,(uint8*)"");
        return FALSE;
    }
    
    //syncml_cb_message:"MMIPIM@@ExtractVersionFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_843_112_2_18_2_47_19_247,(uint8*)"");
    
    ret = ExtractVersion(buffer,&vcal_info_ptr->vCalVer);
    if(1==ret)
    {
        return TRUE;
    }
    else
    {
        //syncml_cb_message:"MMIPIM: ExtractVersionFrVcard ret = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_852_112_2_18_2_47_19_248,(uint8*)"d",ret);
        return FALSE;
    }
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractPriorityFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractPriorityFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractPriorityFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_878_112_2_18_2_47_19_249,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_PRIORITY_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_PRIORITY_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalPriority.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalPriority.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalPriority.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalPriority.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalPriority.priority= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalPriority.priority)
        {
            //syncml_cb_message:"MMIPIM: ExtractPriorityFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_909_112_2_18_2_47_19_250,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalPriority.priority, propertyPtr, valueLen);
        vcal_info_ptr->vCalPriority.priority[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractStatusFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractStatusFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractStatusFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_943_112_2_18_2_47_19_251,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_STATUS_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_STATUS_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalStatus.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalStatus.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalStatus.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalStatus.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalStatus.status = (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalStatus.status)
        {
            //syncml_cb_message:"MMIPIM: ExtractStatusFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_974_112_2_18_2_47_19_252,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalStatus.status, propertyPtr, valueLen);
        vcal_info_ptr->vCalStatus.status[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractCategoriesFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractCategoriesFrVcal(const char *buffer,
                                            VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;
    char  eol = 0;
    uint8 i=0;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractCategoriesFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1010_112_2_18_2_47_19_253,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_CATEGORIES_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_CATEGORIES_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalCategories.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalCategories.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalCategories.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalCategories.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/
    if (valueLen > 0)
    {
        for(i=0;i<VCAL_CATE_MAX;i++)
        {
            headPtr = tailPtr+1;
            tailPtr = IdentifyDelimiters(headPtr, &eol);
            valueLen = tailPtr-headPtr;
            if(valueLen>0)
            {
                vcal_info_ptr->vCalCategories.categories[i]= (char *)hs_smlLibMalloc(__FILE__, __LINE__,(long)(valueLen+1));
                if (!vcal_info_ptr->vCalCategories.categories[i])
                {
                    //syncml_cb_message:"MMIPIM: ExtractCategoriesFrVcard:hs_smlLibMalloc FAILED %d\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1047_112_2_18_2_47_19_254,(uint8*)"d", (valueLen+1));
                    return FALSE;
                }

                memcpy(vcal_info_ptr->vCalCategories.categories[i], headPtr, valueLen);
                vcal_info_ptr->vCalCategories.categories[i][valueLen] = '\0';
            }
            else
            {
                break;
            }
            if(1==eol)
            {
                break;
            }
        }

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractDescFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractDescFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractDescFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1091_112_2_18_2_47_20_255,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_DESCRIPTION_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_DESCRIPTION_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalDescription.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalDescription.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalDescription.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalDescription.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/
    
    if (valueLen > 0)
    {
        vcal_info_ptr->vCalDescription.description= (char *)hs_smlLibMalloc(__FILE__, __LINE__,(long)(valueLen+1));
        if (!vcal_info_ptr->vCalDescription.description)
        {
            //syncml_cb_message:"MMIPIM: ExtractDescFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1122_112_2_18_2_47_20_256,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalDescription.description, propertyPtr, valueLen);
        vcal_info_ptr->vCalDescription.description[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractSummaryFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractSummaryFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractSummaryFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1156_112_2_18_2_47_20_257,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_SUMMARY_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_SUMMARY_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalSummary.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalSummary.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalSummary.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalSummary.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalSummary.summary= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalSummary.summary)
        {
            //syncml_cb_message:"MMIPIM: ExtractSummaryFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1187_112_2_18_2_47_20_258,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalSummary.summary, propertyPtr, valueLen);
        vcal_info_ptr->vCalSummary.summary[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractDtstartFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractDtstartFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractDtstartFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1221_112_2_18_2_47_20_259,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_DTSTART_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_DTSTART_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalDtstart.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalDtstart.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalDtstart.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalDtstart.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalDtstart.dtStart= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalDtstart.dtStart)
        {
            //syncml_cb_message:"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1252_112_2_18_2_47_20_260,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalDtstart.dtStart, propertyPtr, valueLen);
        vcal_info_ptr->vCalDtstart.dtStart[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractDtendFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractDtendFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractDtendFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1286_112_2_18_2_47_20_261,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_DTEND_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_DTEND_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalDtend.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalDtend.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalDtend.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalDtend.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalDtend.dtEnd= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalDtend.dtEnd)
        {
            //syncml_cb_message:"MMIPIM: ExtractDtendFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1317_112_2_18_2_47_20_262,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalDtend.dtEnd, propertyPtr, valueLen);
        vcal_info_ptr->vCalDtend.dtEnd[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractDueFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractDueFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractDueFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1351_112_2_18_2_47_20_263,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_DUE_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_DUE_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalDue.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalDue.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalDue.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalDue.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalDue.due= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalDue.due)
        {
            //syncml_cb_message:"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1382_112_2_18_2_47_20_264,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalDue.due, propertyPtr, valueLen);
        vcal_info_ptr->vCalDue.due[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : ExtractCompletedFrVcal
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL BOOLEAN ExtractCompletedFrVcal(const char *buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char *lfcr = NULL;
    char *headPtr = NULL;
    char *tailPtr = NULL;
    signed int valueLen = 0;
    char foundSemicolon = 0;
    char *propertyPtr = NULL;

    if ((!buffer) || (!vcal_info_ptr))
        return FALSE;
    
    //syncml_cb_message:"MMIPIM@@ExtractCompletedFrVcal start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1416_112_2_18_2_47_20_265,(uint8*)"");
    
    headPtr = strstr(buffer, VCAL_COMPLETED_PROPERTY);/*lint !e605*/
    if (!headPtr)
        return FALSE;

    headPtr += strlen(VCAL_COMPLETED_PROPERTY);
    tailPtr = IdentifySemicolon(headPtr, &foundSemicolon);

    if (!( (tailPtr) && (foundSemicolon) ))
        return FALSE;

    // Property Value starts from here
    propertyPtr = tailPtr+1;

    // check if the Encodings exist
    IdentifyEncoding(headPtr, &vcal_info_ptr->vCalCompleted.property.encoding);
    IdentifyCharSet(headPtr, &vcal_info_ptr->vCalCompleted.property.charSet);
    IdentifyLanguage(headPtr, &vcal_info_ptr->vCalCompleted.property.language);
    IdentifyValueLocation(headPtr, &vcal_info_ptr->vCalCompleted.property.valueLocation);

    lfcr = strstr(headPtr, VCAL_LFCR);

    // look for property value length
    valueLen = lfcr - propertyPtr;/*lint !e613*/

    if (valueLen > 0)
    {
        vcal_info_ptr->vCalCompleted.completed= (char *)hs_smlLibMalloc(__FILE__, __LINE__, (long)(valueLen+1));
        if (!vcal_info_ptr->vCalCompleted.completed)
        {
            //syncml_cb_message:"MMIPIM: ExtractDtstartFrVcard:hs_smlLibMalloc FAILED %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1447_112_2_18_2_47_20_266,(uint8*)"d", (valueLen+1));
            return FALSE;
        }

        memcpy(vcal_info_ptr->vCalCompleted.completed, propertyPtr, valueLen);
        vcal_info_ptr->vCalCompleted.completed[valueLen] = '\0';

        return TRUE;
    }

    return FALSE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_BuffToVcalinfo
 * decr      : 将vcalendar数据转换为vcal结构体数据
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/4/2009 
 ==========================================================*/
LOCAL BOOLEAN MMIVCAL_BuffToVcalinfo(char* buffer,
                                        VCALENDAR_INFO_T* vcal_info_ptr)
{
    char* headPtr = NULL;
    if((NULL == buffer)||(NULL == vcal_info_ptr))
    {
        return FALSE;
    }
    
    //syncml_cb_message:"MMIPIM@@MMIVCAL_BuffToVcalinfo start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1477_112_2_18_2_47_21_267,(uint8*)"");
    
    ExtractVersionFrVcal(buffer,vcal_info_ptr);
    ExtractPriorityFrVcal(buffer,vcal_info_ptr);
    ExtractStatusFrVcal(buffer,vcal_info_ptr);
    ExtractCategoriesFrVcal(buffer,vcal_info_ptr);
    ExtractDescFrVcal(buffer,vcal_info_ptr);
    ExtractSummaryFrVcal(buffer,vcal_info_ptr);

    headPtr = strstr(buffer, VCAL_BEGIN_VEVENT);
    if(NULL!=headPtr)
    {
        //syncml_cb_message:"MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1489_112_2_18_2_47_21_268,(uint8*)"");
        ExtractDtstartFrVcal(buffer,vcal_info_ptr);
        ExtractDtendFrVcal(buffer,vcal_info_ptr);
    }
    else if(NULL!=strstr(buffer, VCAL_BEGIN_VTODO))
    {
        //syncml_cb_message:"MMIPIM@@MMIVCAL_BuffToVcalinfo extract VEVENT start!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1495_112_2_18_2_47_21_269,(uint8*)"");
        ExtractDueFrVcal(buffer,vcal_info_ptr);
        ExtractCompletedFrVcal(buffer,vcal_info_ptr);
    }
    
    //解析可选字段
    ExtractClassFrVcal(buffer,vcal_info_ptr);
    ExtractRruleFrVcal(buffer,vcal_info_ptr);
    ExtractAalarmFrVcal(buffer,vcal_info_ptr);
    return TRUE;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_VcalToSche
 * decr      : 将vcal结构体数据转换为日程
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/4/2009 
 ==========================================================*/
LOCAL BOOLEAN MMIVCAL_VcalToSche(VCALENDAR_INFO_T* vcal_info_ptr,
                                            MMIACC_SMART_EVENT_T *event_info_ptr)
{
    uint8   month =0;
    uint8   day = 0;
    uint16   year = 0;
    uint8 hour = 0;
    uint8 minute = 0;
    uint16 content_len = 0;
    uint16 desc_len = 0;
    uint8  end_month = 0;
    uint8  end_day = 0;
    uint16   end_year = 0;
    uint8  ring_month = 0;
    uint8  ring_day = 0;
    uint16   ring_year = 0;
    uint8  ring_hour = 0;
    uint8  ring_minute = 0;
    
    if(NULL == vcal_info_ptr||NULL == event_info_ptr)
    {
        //syncml_cb_message:"MMIPIM: MMIVCAL_VcalToSche vcal_info_ptr or event_info_ptr is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1536_112_2_18_2_47_21_270,(uint8*)"");
        return FALSE;
    }
    
    if(NULL!=vcal_info_ptr->vCalDtstart.dtStart)
    {
        //年
        year = (vcal_info_ptr->vCalDtstart.dtStart[0]-'0')*1000
                +(vcal_info_ptr->vCalDtstart.dtStart[1]-'0')*100
                +(vcal_info_ptr->vCalDtstart.dtStart[2]-'0')*10
                +(vcal_info_ptr->vCalDtstart.dtStart[3]-'0');
        //月
        month = (vcal_info_ptr->vCalDtstart.dtStart[4]-'0')*10
                +(vcal_info_ptr->vCalDtstart.dtStart[5]-'0');
        //日
        day = (vcal_info_ptr->vCalDtstart.dtStart[6]-'0')*10
                +(vcal_info_ptr->vCalDtstart.dtStart[7]-'0');
        //time
        hour = (vcal_info_ptr->vCalDtstart.dtStart[9]-'0')*10
                +(vcal_info_ptr->vCalDtstart.dtStart[10]-'0');
        minute = (vcal_info_ptr->vCalDtstart.dtStart[11]-'0')*10
                +(vcal_info_ptr->vCalDtstart.dtStart[12]-'0');
    }
    else if(NULL!=vcal_info_ptr->vCalDue.due)
    {
        //年
        year = (vcal_info_ptr->vCalDue.due[0]-'0')*1000
                +(vcal_info_ptr->vCalDue.due[1]-'0')*100
                +(vcal_info_ptr->vCalDue.due[2]-'0')*10
                +(vcal_info_ptr->vCalDue.due[3]-'0');
        //月
        month = (vcal_info_ptr->vCalDue.due[4]-'0')*10
                +(vcal_info_ptr->vCalDue.due[5]-'0');
        //日
        day = (vcal_info_ptr->vCalDue.due[6]-'0')*10
                +(vcal_info_ptr->vCalDue.due[7]-'0');
        //time
        hour = (vcal_info_ptr->vCalDue.due[9]-'0')*10
                +(vcal_info_ptr->vCalDue.due[10]-'0');
        minute = (vcal_info_ptr->vCalDue.due[11]-'0')*10
                +(vcal_info_ptr->vCalDue.due[12]-'0');
    }
    if(year>9999||month>12||day>31
        ||year<1900||month==0||day==0)
    {
        //syncml_cb_message:"MMIPIM: MMIVCAL_VcalToSche Starttime is invalid! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1581_112_2_18_2_47_21_271,(uint8*)"");
        return FALSE;
    }
    
    if(NULL!=vcal_info_ptr->vCalDtend.dtEnd)
    {
        //年
        end_year = (vcal_info_ptr->vCalDtend.dtEnd[0]-'0')*1000
                +(vcal_info_ptr->vCalDtend.dtEnd[1]-'0')*100
                +(vcal_info_ptr->vCalDtend.dtEnd[2]-'0')*10
                +(vcal_info_ptr->vCalDtend.dtEnd[3]-'0');
        //月
        end_month = (vcal_info_ptr->vCalDtend.dtEnd[4]-'0')*10
                +(vcal_info_ptr->vCalDtend.dtEnd[5]-'0');
        //日
        end_day = (vcal_info_ptr->vCalDtend.dtEnd[6]-'0')*10
                +(vcal_info_ptr->vCalDtend.dtEnd[7]-'0');

    }
    else if(NULL!=vcal_info_ptr->vCalCompleted.completed)
    {
        //年
        end_year = (vcal_info_ptr->vCalCompleted.completed[0]-'0')*1000
                +(vcal_info_ptr->vCalCompleted.completed[1]-'0')*100
                +(vcal_info_ptr->vCalCompleted.completed[2]-'0')*10
                +(vcal_info_ptr->vCalCompleted.completed[3]-'0');
        //月
        end_month = (vcal_info_ptr->vCalCompleted.completed[4]-'0')*10
                +(vcal_info_ptr->vCalCompleted.completed[5]-'0');
        //日
        end_day = (vcal_info_ptr->vCalCompleted.completed[6]-'0')*10
                +(vcal_info_ptr->vCalCompleted.completed[7]-'0');
    }
#if 0//Added by wangzhefeng  2009.12.11for 避免当日程爆发后由于AALARM字段变化而使慢同步时增加日程条目
    //解析AALARM的爆发时间
    if(NULL!=vcal_info_ptr->vCalaalarm.aalarm)
    {
        //年
        ring_year = (vcal_info_ptr->vCalaalarm.aalarm[0]-'0')*1000
                +(vcal_info_ptr->vCalaalarm.aalarm[1]-'0')*100
                +(vcal_info_ptr->vCalaalarm.aalarm[2]-'0')*10
                +(vcal_info_ptr->vCalaalarm.aalarm[3]-'0');
        //月
        ring_month = (vcal_info_ptr->vCalaalarm.aalarm[4]-'0')*10
                +(vcal_info_ptr->vCalaalarm.aalarm[5]-'0');
        //日
        ring_day = (vcal_info_ptr->vCalaalarm.aalarm[6]-'0')*10
                +(vcal_info_ptr->vCalaalarm.aalarm[7]-'0');
        //time
        ring_hour = (vcal_info_ptr->vCalaalarm.aalarm[9]-'0')*10
                +(vcal_info_ptr->vCalaalarm.aalarm[10]-'0');
        ring_minute = (vcal_info_ptr->vCalaalarm.aalarm[11]-'0')*10
                +(vcal_info_ptr->vCalaalarm.aalarm[12]-'0');
    }
    else
#endif//Added by wangzhefeng  2009.12.11
    {
        //年
        ring_year = year;
        //月
        ring_month = month;
        //日
        ring_day = day;
        //time
        ring_hour = hour;
        ring_minute = minute;
    }
    
    //创建新日程
#ifdef CALENDAR_SUPPORT
    MMIAPISCH_InitNewSch(year,month,day,event_info_ptr);
#endif
    event_info_ptr->event_fast_info.year= ring_year;
    event_info_ptr->event_fast_info.mon= ring_month;
    event_info_ptr->event_fast_info.day = ring_day;
    event_info_ptr->event_fast_info.hour = ring_hour;
    event_info_ptr->event_fast_info.minute = ring_minute;
    
    //如果end_time为正常值就赋值，否则赋值为默认值
    if(!(end_year>9999||end_month>12||end_day>31
        ||end_year<1900||end_month==0||end_day==0))
    {
        event_info_ptr->event_fast_info.end_year = end_year;
        event_info_ptr->event_fast_info.end_mon = end_month;
        event_info_ptr->event_fast_info.end_day = end_day;
    }
    else
    {
        //syncml_cb_message:"MMIPIM: MMIVCAL_VcalToSche the number is invalid! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1667_112_2_18_2_47_21_272,(uint8*)"");
    }

    //content
    if(NULL!=vcal_info_ptr->vCalSummary.summary)
    {
        uint8 utf8_ptr[VCALENDAR_DESC_MAX_LEN] = {0};
        int utf8_len = 0;
        int temp_len = 0;
        wchar temp_info[2*MMIACC_EVENT_TEXT_MAX_LEN+1] = {0};
        
        desc_len = (uint16)strlen(vcal_info_ptr->vCalSummary.summary);

        if(desc_len>0)
        {
            if(strstr(vcal_info_ptr->vCalSummary.property.charSet,"UTF-8")
                &&(vcal_info_ptr->vCalSummary.property.encoding.quotedPrintable == 1))
            {
                //qp->utf-8
                utf8_len = DecodeQuoted(vcal_info_ptr->vCalSummary.summary, utf8_ptr,  desc_len);
                
                //UTF-8解码
                temp_len = GUI_UTF8ToWstr(temp_info,//event_info_ptr->event_content_info.w_content,
                                2*MMIACC_EVENT_TEXT_MAX_LEN+1,
                                utf8_ptr,
                                utf8_len);
                content_len = MIN(MMIACC_EVENT_TEXT_MAX_LEN,temp_len);
                
                SCI_MEMSET(event_info_ptr->event_content_info.w_content,
                                    0,
                                    sizeof(wchar)*(MMIACC_EVENT_TEXT_MAX_LEN+1));
                SCI_MEMCPY(event_info_ptr->event_content_info.w_content,
                                    temp_info,
                                    content_len*(sizeof(wchar)));
            }
            else
            {//正常解码
            
                MMIAPICOM_StrToWstr(vcal_info_ptr->vCalSummary.summary,/*lint !e64*/
                                temp_info);//event_info_ptr->event_content_info.w_content);
                content_len = MIN(MMIACC_EVENT_TEXT_MAX_LEN,desc_len);
                
                SCI_MEMSET(event_info_ptr->event_content_info.w_content,
                                    0,
                                    sizeof(wchar)*(MMIACC_EVENT_TEXT_MAX_LEN+1));
                SCI_MEMCPY(event_info_ptr->event_content_info.w_content,
                                    temp_info,
                                    content_len*(sizeof(wchar)));
            }
        }
        event_info_ptr->event_content_info.w_length = content_len;
    }
       
    //解析循环模式和次数
    if(NULL!=vcal_info_ptr->vCalRrule.rrule)
    {
        VCAL_StrUpr(vcal_info_ptr->vCalRrule.rrule);
#if 0//Added by wangzhefeng  2009.12.8
        if(strstr(vcal_info_ptr->vCalRrule.rrule,"D1 #1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
        }
#endif//Added by wangzhefeng  2009.12.8
        if(strstr(vcal_info_ptr->vCalRrule.rrule,"MD1")
            ||strstr(vcal_info_ptr->vCalRrule.rrule,"MP1"))
        {
            //对于其他终端上传的MP1字段，解析出来会对应有问题，但无法避免
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
        }
        else if(strstr(vcal_info_ptr->vCalRrule.rrule,"YM1")
            ||strstr(vcal_info_ptr->vCalRrule.rrule,"YD1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
        }
        else if(strstr(vcal_info_ptr->vCalRrule.rrule,"D1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
        }
        else if(strstr(vcal_info_ptr->vCalRrule.rrule,"W1"))
        {
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            event_info_ptr->event_fast_info.fre = 0x00;
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"SA"))
            {
                event_info_ptr->event_fast_info.fre |= 0x40;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"FR"))
            {
                event_info_ptr->event_fast_info.fre |= 0x20;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"TH"))
            {
                event_info_ptr->event_fast_info.fre |= 0x10;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"WE"))
            {
                event_info_ptr->event_fast_info.fre |= 0x08;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"TU"))
            {
                event_info_ptr->event_fast_info.fre |= 0x04;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"MO"))
            {
                event_info_ptr->event_fast_info.fre |= 0x02;
            }
            if(strstr(vcal_info_ptr->vCalRrule.rrule,"SU"))
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
            //syncml_cb_message:"MMIPIM: MMIVCAL_VcalToSche the fre_mode is wrong! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_1785_112_2_18_2_47_21_273,(uint8*)"");
#if 0//Added by wangzhefeng  2009.12.8
            event_info_ptr->event_fast_info.is_on = FALSE;
            event_info_ptr->event_fast_info.fre_mode = ALM_MODE_NO;
#endif//Added by wangzhefeng  2009.12.8
        }

    }
    else if(NULL!=vcal_info_ptr->vCalaalarm.aalarm)//一次
    {
        event_info_ptr->event_fast_info.fre_mode = ALM_MODE_ONCE;
    }
    else//不提醒
    {
        event_info_ptr->event_fast_info.is_on = FALSE;
        event_info_ptr->event_fast_info.fre_mode = ALM_MODE_NO;
    }
    return TRUE;
    
}
/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_VcalFree
 * decr      : 释放解码时申请的空间
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  8/5/2009 
 ==========================================================*/
LOCAL void MMIVCAL_VcalFree(VCALENDAR_INFO_T* vcal_info_ptr)
{
    uint8 i = 0;
    //version
    if(NULL!=vcal_info_ptr->vCalVer.verNo)
    {
        hs_smlLibFree(vcal_info_ptr->vCalVer.verNo);
    }
    if(NULL!=vcal_info_ptr->vCalVer.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalVer.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalVer.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalVer.property.language);
    }
    //priority
    if(NULL!=vcal_info_ptr->vCalPriority.priority)
    {
        hs_smlLibFree(vcal_info_ptr->vCalPriority.priority);
    }
    if(NULL!=vcal_info_ptr->vCalPriority.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalPriority.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalPriority.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalPriority.property.language);
    }
    //status
    if(NULL!=vcal_info_ptr->vCalStatus.status)
    {
        hs_smlLibFree(vcal_info_ptr->vCalStatus.status);
    }
    if(NULL!=vcal_info_ptr->vCalStatus.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalStatus.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalStatus.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalStatus.property.language);
    }
    
    //categories
    for(i=0;i<VCAL_CATE_MAX;i++)
    {
        if(NULL!=vcal_info_ptr->vCalCategories.categories[i])
        {
            hs_smlLibFree(vcal_info_ptr->vCalStatus.status);
        }
    }
    if(NULL!=vcal_info_ptr->vCalCategories.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalCategories.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalCategories.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalCategories.property.language);
    }

    //description
    if(NULL!=vcal_info_ptr->vCalDescription.description)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDescription.description);
    }
    if(NULL!=vcal_info_ptr->vCalDescription.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDescription.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalDescription.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDescription.property.language);
    }
    
    //summary
    if(NULL!=vcal_info_ptr->vCalSummary.summary)
    {
        hs_smlLibFree(vcal_info_ptr->vCalSummary.summary);
    }
    if(NULL!=vcal_info_ptr->vCalSummary.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalSummary.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalSummary.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalSummary.property.language);
    }
    //dtstart
    if(NULL!=vcal_info_ptr->vCalDtstart.dtStart)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtstart.dtStart);
    }
    if(NULL!=vcal_info_ptr->vCalDtstart.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtstart.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalDtstart.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtstart.property.language);
    }
    //dtend
    if(NULL!=vcal_info_ptr->vCalDtend.dtEnd)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtend.dtEnd);
    }
    if(NULL!=vcal_info_ptr->vCalDtend.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtend.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalDtend.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDtend.property.language);
    }
    //due
    if(NULL!=vcal_info_ptr->vCalDue.due)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDue.due);
    }
    if(NULL!=vcal_info_ptr->vCalDue.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDue.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalDue.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalDue.property.language);
    }
    //completed
    if(NULL!=vcal_info_ptr->vCalCompleted.completed)
    {
        hs_smlLibFree(vcal_info_ptr->vCalCompleted.completed);
    }
    if(NULL!=vcal_info_ptr->vCalCompleted.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalCompleted.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalCompleted.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalCompleted.property.language);
    }
    
    //class
    if(NULL!=vcal_info_ptr->vCalClass.cclass)
    {
        hs_smlLibFree(vcal_info_ptr->vCalClass.cclass);
    }
    if(NULL!=vcal_info_ptr->vCalClass.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalClass.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalClass.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalClass.property.language);
    }

    //rrule
    if(NULL!=vcal_info_ptr->vCalRrule.rrule)
    {
        hs_smlLibFree(vcal_info_ptr->vCalRrule.rrule);
    }
    if(NULL!=vcal_info_ptr->vCalRrule.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalRrule.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalRrule.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalRrule.property.language);
    }
    
    //dalarm
    if(NULL!=vcal_info_ptr->vCalaalarm.aalarm)
    {
        hs_smlLibFree(vcal_info_ptr->vCalaalarm.aalarm);
    }
    if(NULL!=vcal_info_ptr->vCalaalarm.property.charSet)
    {
        hs_smlLibFree(vcal_info_ptr->vCalaalarm.property.charSet);
    }
    if(NULL!=vcal_info_ptr->vCalaalarm.property.language)
    {
        hs_smlLibFree(vcal_info_ptr->vCalaalarm.property.language);
    }
    
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_BufferToSche
 * decr      : 将传入的buffer数据转换为日程结构体数据
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
static BOOLEAN MMIVCAL_BufferToSche( char *buffer,//in ,传入的buffer数据
                                    MMIACC_SMART_EVENT_T* event_info_ptr //out,日程
                                    )
{
    BOOLEAN ret = FALSE;
    //uint16 index = 0;
    VCALENDAR_INFO_T vcal_info = {0};
    //syncml_cb_message:"MMIPIM@@MMIVCAL_BufferToSche start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2012_112_2_18_2_47_22_274,(uint8*)"");
    //将buffer数据转换为vcal_info
    ret = MMIVCAL_BuffToVcalinfo(buffer,&vcal_info);
    if(FALSE==ret)
    {
        return ret;
    }

    //将vcal_info转换为日程
    ret = MMIVCAL_VcalToSche(&vcal_info,event_info_ptr);
    //释放空间
    MMIVCAL_VcalFree(&vcal_info);
    return ret;
    
}


/*==========================================================
 *
 *提供给PIM同步的接口
 *
 ==========================================================*/
 


/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_PimAdd
 * decr      : 添加一个日程，用到日程的接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/3/2009 
 ==========================================================*/
static BOOLEAN MMIVCAL_PimAdd(
                                MMIACC_SMART_EVENT_T     *event_info_ptr,
                                uint16 *index_ptr //out,存储的位置
                                )
{
    uint16  index = 0;
    BOOLEAN  result = FALSE;
    
#if 0//del by wangxiaolin  2009.10.16
    SCI_ASSERT(PNULL != event_info_ptr);
#else
if(PNULL == event_info_ptr)
{
    return FALSE;
}
#endif
    
    result = MMIALM_AddOneEvent(event_info_ptr,&index);
    //syncml_cb_message:"MMIPIM@@MMIVCAL_PimAdd! id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2062_112_2_18_2_47_22_275,(uint8*)"d",index-EVENT_SCH_0+1);
    
    *index_ptr = index-EVENT_SCH_0+1;
    
    if(TRUE == result)
    {
        MMIVCAL_UpdatePimStatus((index-EVENT_SCH_0+1),VCAL_NO_MODIFY);
    }
    
    return result;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_PimDelete
 * decr      : 删除一个日程，用到日程的接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
static BOOLEAN MMIVCAL_PimDelete(
                                uint16      entry_id   
                                )
{
    BOOLEAN             result = FALSE;
    
#if 0//del by wangxiaolin  2009.10.16
    SCI_ASSERT(0 < entry_id);
#else
  if(0 == entry_id)
  {
       return FALSE;
  }
#endif
    
    //syncml_cb_message:"MMIPIM@@MMIVCAL_PimDelete! id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2097_112_2_18_2_47_22_276,(uint8*)"d",entry_id);

    result = MMIALM_DelOneEvent(entry_id+EVENT_SCH_0-1);
    
    if(TRUE == result)
    {
        MMIVCAL_UpdatePimStatus(entry_id,VCAL_NO_MODIFY);
    }
    
    return (result);
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_PimModify
 * decr      : 修改一个日程，用到日程的接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
static BOOLEAN MMIVCAL_PimModify (
                                uint16 				    event_id, //in
                                MMIACC_SMART_EVENT_T     *event_info_ptr  //in,out
                                )
{
    BOOLEAN    result = FALSE;
        
#if 0//del by wangxiaolin  2009.10.16
    SCI_ASSERT(PNULL != event_info_ptr);
    SCI_ASSERT(0 < event_id);
#else
if((PNULL == event_info_ptr)||(0 == event_id))
{
    return FALSE;
}
#endif
    
    //syncml_cb_message:"MMIPIM@@MMIVCAL_PimModify!id = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2134_112_2_18_2_47_22_277,(uint8*)"d",event_id);
    result = MMIAPIALM_SaveInteEventInfo(event_id+EVENT_SCH_0-1,event_info_ptr);
    
    if(TRUE == result)
    {
        MMIVCAL_UpdatePimStatus(event_id,VCAL_NO_MODIFY);
    }
    
    return (result);
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_Add
 * decr      : 提供给PIMcallback_handler.c的add接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_Add(short id, void* userData, SmlAddPtr_t pContent)
{
    SmlStatusPtr_t  status=NULL;
    SmlSourceRefListPtr_t sourcelist=NULL;
    char* vCalData=NULL;
    char* tempcharptr=NULL;
    SmlNewMapItemListPtr_t maplist=NULL;
    SmlNewMapItemListPtr_t temp=NULL;
	int str_len=0;

    uint16 rec_id = 0;
    MMIACC_SMART_EVENT_T event_info = {0};

    PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin

    //syncml_cb_message:"MMIPIM @@ MMIPIM_VCAL_Add enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2168_112_2_18_2_47_22_278,(uint8*)"");
    status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
    if(NULL==status)
    {
        return hs_smlFreeProtoElement(pContent); 
    }
    sourcelist=(SmlSourceRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSourceRefList_t));
    if(NULL==sourcelist)
    {
        hs_smlLibFree(status);
        return hs_smlFreeProtoElement(pContent); 
    }

    hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
    hs_smlLibMemset(sourcelist, 0, (long)sizeof(SmlSourceRefList_t));

    status->elementType=SML_PE_STATUS;
    status->sourceRefList=sourcelist;  

    status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
    //status->cmdRef=pContent->cmdID;
    status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
    status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Add");
    //status->sourceRefList->sourceRef=pContent->itemList->item->source->locURI;
    status->sourceRefList->sourceRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->source->locURI->content); // chenxiang_memory 20080223
    status->sourceRefList->next=NULL;

    vCalData=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->data);  //malloc room for vCardData here,shall be free in future
    if(NULL==vCalData)
    {
        hs_smlLibFree(status);
        hs_smlLibFree(sourcelist);
        return hs_smlFreeProtoElement(pContent); 
    }

    //syncml_cb_message:"MMIPIM @@MMIPIM_VCAL_Add vCalData_content=%s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2203_112_2_18_2_47_22_279,(uint8*)"s",vCalData);

    pMe->g_sync_result.receive_num++;

   if(0==MMIVCAL_GetFreeItemsCount())
    {
  	 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "420");  //memory full !
  	 //syncml_message:"mmipim @@memory full! in handle add.@@"
  	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2210_112_2_18_2_47_22_280,(uint8*)"");
    }
    else
    {
        if( TRUE == MMIVCAL_BufferToSche(vCalData,&event_info))
        {
            //   添加日程
            if(TRUE ==MMIVCAL_PimAdd(&event_info,&rec_id))
            {   
                //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add id=%d ,time=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2219_112_2_18_2_47_22_281,(uint8*)"dd",rec_id,PIM_GetCurTime());
                //用于显示同步结果
                pMe->g_sync_result.added_num_for_phone++; //add by wangxiaolin 

                status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "201");  //add success // chenxiang 20090206 200-->201
                //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add add record ok!! in handle add.@@"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2224_112_2_18_2_47_22_282,(uint8*)"");
                //  添加map 命令队列=========================

                maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlNewMapItemList_t));
                if(NULL==maplist)
                {
                    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add maplist == NULL!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2230_112_2_18_2_47_22_283,(uint8*)"");
                    hs_smlLibFree(status);
                    hs_smlLibFree(sourcelist);
                    hs_smlLibFree(vCalData);
                    return hs_smlFreeProtoElement(pContent); 
                }
                hs_smlLibMemset(maplist, 0, (long)sizeof(SmlNewMapItemList_t));

                tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);
				str_len=hs_smlLibStrlen(tempcharptr);
                hs_smlLibMemcpy(maplist->target, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 target
                hs_smlLibFree(tempcharptr);
                tempcharptr=NULL;

                tempcharptr=hs_smlunsignedInt2String(__FILE__, __LINE__, rec_id);
				str_len=hs_smlLibStrlen(tempcharptr);
                hs_smlLibMemcpy(maplist->source, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 source
                hs_smlLibFree(tempcharptr);
                tempcharptr=NULL;

                if(NULL==hs_globlemapItemList)
                {
                    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add hs_globlemapItemList ==NULL!! "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2252_112_2_18_2_47_22_284,(uint8*)"");
                    hs_globlemapItemList=maplist;
                }
                else
                {
                    temp=hs_globlemapItemList;
                    while(NULL!=temp->next)
                    {
                        temp=temp->next;
                    }
                    temp->next=maplist;
                }
                //添加结束=================================
            }
            else
            {
                status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //add fail
                //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add add record error!! in handle add.@@"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2269_112_2_18_2_47_22_285,(uint8*)"");
            }
        }
        else
        {
            status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //add fail
            //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add analysis vcard error!! in handle add.@@"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2275_112_2_18_2_47_22_286,(uint8*)"");
        }
    }

    hs_smlLibFree(vCalData); //free vCardData
    hs_myAddstatustoCQ(status);
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_Add add record finish.@@time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2281_112_2_18_2_47_22_287,(uint8*)"d",PIM_GetCurTime());

    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_replace
 * decr      : 提供给PIMcallback_handler.c的replace接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_replace(short id, void* userData, SmlReplacePtr_t pContent)
{
    SmlStatusPtr_t  status=NULL;
    SmlTargetRefListPtr_t targetlist=NULL;
    char* vCalData=NULL;
    uint16 rec_id=0;
    char * rec_id_ptr=PNULL;
    MMIACC_SMART_EVENT_T event_info = {0};
    PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin
    
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace enter! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2304_112_2_18_2_47_22_288,(uint8*)"");
    status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
    if(NULL==status)
    {
        return hs_smlFreeProtoElement(pContent); 
    }
    targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
    if(NULL==targetlist)
    {
        hs_smlLibFree(status);
        return hs_smlFreeProtoElement(pContent); 
    }

    hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
    hs_smlLibMemset(targetlist, 0, (long)sizeof(SmlTargetRefList_t));

    status->elementType=SML_PE_STATUS;
    status->targetRefList=targetlist;  
    status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
    //status->cmdRef=pContent->cmdID;
    status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
    status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Replace");
    //status->targetRefList->targetRef=pContent->itemList->item->target->locURI;
    status->targetRefList->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->target->locURI->content); // chenxiang_memory 20080223

    rec_id_ptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->target->locURI);
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace id_srt=%s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2330_112_2_18_2_47_23_289,(uint8*)"s",rec_id_ptr);

    rec_id=(uint16)hs_smlString2unsignedInt(rec_id_ptr);
    hs_smlLibFree(rec_id_ptr);

    status->targetRefList->next=NULL;

    vCalData=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->data);  //malloc room for vCardData here,shall be free in future
    if(NULL==vCalData)
    {
        hs_smlLibFree(status);
        hs_smlLibFree(targetlist);
        return hs_smlFreeProtoElement(pContent); 
    }
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace id=%d,content=%s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2344_112_2_18_2_47_23_290,(uint8*)"ds",rec_id,vCalData);

    pMe->g_sync_result.receive_num++;
    //   buffer数据转换为日程结构体数据
    if( TRUE == MMIVCAL_BufferToSche(vCalData, &event_info) )
    {
        // 添加日程
        if(TRUE ==MMIVCAL_PimModify(rec_id,&event_info))
        {
            pMe->g_sync_result.replaced_num_for_phone++; //add by wangxiaolin 
            status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
            //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace replace record ok!! in handle replace.@@"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2355_112_2_18_2_47_23_291,(uint8*)"");
        }
        else
        {
            status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
            //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace replace record error!! in handle replace.@@"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2360_112_2_18_2_47_23_292,(uint8*)"");
        }
    }
	else
	{
		status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
		//syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_replace analysis vcal error!! in handle replace.@@"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2366_112_2_18_2_47_23_293,(uint8*)"");
	}

	hs_smlLibFree(vCalData); //free vCardData
	hs_myAddstatustoCQ(status);
	return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_VCAL_delete
 * decr      : 提供给PIMcallback_handler.c的delete接口
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC short MMIPIM_VCAL_delete(short id, void* userData, SmlDeletePtr_t pContent)
{
    SmlStatusPtr_t  status;
    SmlTargetRefListPtr_t targetlist;
    uint16 rec_id=0;
    char * rec_id_ptr=PNULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();
    
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_delete enter!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2390_112_2_18_2_47_23_294,(uint8*)"");
    status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
    if(NULL==status)
    {
        return hs_smlFreeProtoElement(pContent); 
    }
    targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
    if(NULL==targetlist)
    {
        hs_smlLibFree(status);
        return hs_smlFreeProtoElement(pContent); 
    }

    hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
    hs_smlLibMemset(targetlist, 0, (long)sizeof(SmlTargetRefList_t));

    status->elementType=SML_PE_STATUS;
    status->targetRefList=targetlist;  
    status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
    //status->cmdRef=pContent->cmdID;
    status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
    status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Delete");
    //status->targetRefList->targetRef=pContent->itemList->item->target->locURI;
    status->targetRefList->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->target->locURI->content); // chenxiang_memory 20080223

    rec_id_ptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->target->locURI);
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_delete id_srt=%s "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2416_112_2_18_2_47_23_295,(uint8*)"s",rec_id_ptr);
    rec_id=(uint16)hs_smlString2unsignedInt(rec_id_ptr);

    hs_smlLibFree(rec_id_ptr);

    status->targetRefList->next=NULL;
    //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_delete id=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2422_112_2_18_2_47_23_296,(uint8*)"d",rec_id);
    //删除记录
    pMe->g_sync_result.receive_num++;
    if ( TRUE == MMIVCAL_PimDelete( rec_id))
    {
        pMe->g_sync_result.deleted_num_for_phone++; //add by wangxiaolin  // chenxiang 20080702 deleted_num_for_phone

        status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
        //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_delete delete record ok!! in handle delete.@@"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2430_112_2_18_2_47_23_297,(uint8*)"");
    }
    else  
    {
        //慢同步的要求是，如果找不到要删除的记录，忽略
        status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "211");  //delete fail
        //syncml_cb_message:"MMIPIM@@MMIPIM_VCAL_delete delete reocrd error!! in handle delete.@@"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2436_112_2_18_2_47_23_298,(uint8*)"");
    }
    hs_myAddstatustoCQ(status);
    return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}


/*==========================================================
 *
 *提供给PIM同步的日程状态status的接口
 *
 ==========================================================*/
 
/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_InitVCALStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC void  MMIPIM_InitVCALStatus(void)
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    nv_result=MMI_ReadNVItem(MMI_PIM_VCAL_LOG_INFO,g_vcal_log_inf.vcal_status_list_ptr);
    //syncml_cb_message:"MMIPIM:MMIPIM_InitVCALStatus,readnvitem = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2460_112_2_18_2_47_23_299,(uint8*)"d",nv_result);
    if(MN_RETURN_SUCCESS==nv_result)
    {
        g_vcal_log_inf.is_valid=TRUE;
    }
    else
    {
         uint8 vcal_status_arr[MMIPIM_MAX_VCAL_NUM]={0};
         MMI_WriteNVItem(MMI_PIM_VCAL_LOG_INFO,vcal_status_arr);
    }
 }

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetPimStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC VCAL_PIM_STATUS_E MMIVCAL_GetPimStatus(   //RETURN: PIM状态
                                uint16   entry_id   //IN: 记录号
                                )
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;

    if ((entry_id > MMIPIM_MAX_VCAL_NUM) ||(entry_id==0))
    {
        return (VCAL_RECENT_INVALID);
    }

    if(g_vcal_log_inf.is_valid)
    {
       //syncml_cb_message(("MMIPIM: MMIVCAL_GetPimStatus1 entry_id = %d,state=%d", entry_id,g_vcal_log_inf.vcal_status_list_ptr[entry_id -1]));

        return ((VCAL_PIM_STATUS_E)g_vcal_log_inf.vcal_status_list_ptr[entry_id -1]);
    }
    else
    {
        nv_result=MMI_ReadNVItem(MMI_PIM_VCAL_LOG_INFO,g_vcal_log_inf.vcal_status_list_ptr);
        if(MN_RETURN_SUCCESS==nv_result)
        {
           //syncml_cb_message:"MMIPIM: MMIVCAL_GetPimStatus2 ,read sucess! "
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2502_112_2_18_2_47_23_300,(uint8*)"");

            g_vcal_log_inf.is_valid=TRUE;
            return ((VCAL_PIM_STATUS_E)g_vcal_log_inf.vcal_status_list_ptr[entry_id -1]);
        }
        else
        {
            //syncml_cb_message:"MMIPIM: MMIVCAL_GetPimStatus2 ,read fail! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2509_112_2_18_2_47_23_301,(uint8*)"");
            return (VCAL_RECENT_INVALID);
        }
    }
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_UpdatePimStatus
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
 PUBLIC BOOLEAN MMIVCAL_UpdatePimStatus(  //RETURN: TRUE,操作成功；FALSE,操作失败
                                uint16    entry_id,    //IN: 记录号
                                VCAL_PIM_STATUS_E    pim_status    //IN: PIM状态
                                )
{
    VCAL_PIM_STATUS_E old_status = VCAL_RECENT_INVALID; 
    VCAL_PIM_STATUS_E new_status = VCAL_RECENT_INVALID;

    if ((entry_id > MMIPIM_MAX_VCAL_NUM))/*lint !e685*/
    {
        //syncml_cb_message:"MMIPIM: MMIVCAL_UpdatePimStatus ,error!entry_id = %d,pim_status = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2533_112_2_18_2_47_23_302,(uint8*)"dd",entry_id,pim_status);
        return (FALSE);
    }	
    
    old_status = MMIVCAL_GetPimStatus(entry_id);
    //syncml_cb_message:"MMIPIM: MMIVCAL_UpdatePimStatus start!entry_id = %d,old_status = %d,pim_status = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2538_112_2_18_2_47_23_303,(uint8*)"ddd",entry_id,old_status,pim_status);
    
    //判断新状态的值
    switch (old_status)
    {
        case VCAL_NO_MODIFY:
        {
            if (VCAL_NO_MODIFY == pim_status)
            {
                return (TRUE);
            }
            else if (VCAL_RECENT_ADD == pim_status || VCAL_RECENT_REPLACE == pim_status || VCAL_RECENT_DELETE == pim_status)
            {
                new_status = pim_status;
            }
            else
            {
                return (FALSE);
            }
            break;
        }

        case VCAL_RECENT_ADD:
        {
            if (VCAL_NO_MODIFY == pim_status)
            {
                new_status = VCAL_NO_MODIFY;
            }
            else if (VCAL_RECENT_ADD == pim_status)
            {
                return (TRUE); // 这种情况不会发生
            }
            else if (VCAL_RECENT_REPLACE == pim_status)
            {
                return (TRUE);
            }
            else if (VCAL_RECENT_DELETE == pim_status)
            {
                new_status = VCAL_NO_MODIFY;
            }
            else
            {
                return (FALSE);
            }
            break;
        }

        case VCAL_RECENT_REPLACE:
        {
            if (VCAL_NO_MODIFY == pim_status)
            {
                new_status = VCAL_NO_MODIFY;
            }
            else if (VCAL_RECENT_ADD == pim_status)
            {
                return (TRUE); // 这种情况不会发生
            }
            else if (VCAL_RECENT_REPLACE == pim_status)
            {
                return (TRUE);
            }
            else if (VCAL_RECENT_DELETE == pim_status)
            {
                new_status = VCAL_RECENT_DELETE;
            }
            else
            {
                return (FALSE);
            }
            break;
        }
        	
        case VCAL_RECENT_DELETE:
        {
            if (VCAL_NO_MODIFY == pim_status)
            {
                new_status = VCAL_NO_MODIFY;
            }
            else if (VCAL_RECENT_ADD == pim_status)
            {
                new_status = VCAL_RECENT_REPLACE;
            }
            else if (VCAL_RECENT_REPLACE == pim_status)
            {
                new_status = VCAL_RECENT_REPLACE; // 这种情况不会发生
            }
            else if (VCAL_RECENT_DELETE == pim_status)
            {
                return (TRUE); // 这种情况不会发生
            }
            else
            {
                return (FALSE);
            }
            break;
        }
        
        case VCAL_RECENT_INVALID:
            break;

        default:
            return (FALSE);
	}

    //改写全局变量，写NV
    if (entry_id>=1)
    {
        g_vcal_log_inf.vcal_status_list_ptr[entry_id -1]=(uint8)new_status;
    }
    //syncml_cb_message:"MMIPIM: MMIVCAL_UpdatePimStatus end! entry_id = %d,old=%d,new=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2647_112_2_18_2_47_23_304,(uint8*)"ddd", entry_id,old_status,new_status);

    //没有返回值，如何保证写入结果的正确性?
    MMI_WriteNVItem(MMI_PIM_VCAL_LOG_INFO,g_vcal_log_inf.vcal_status_list_ptr);
    
    return (TRUE);

}

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
                                    VCAL_PIM_STATUS_E   pim_status  //IN: PIM状态
                                    )
{
    uint32 i = 0;
    for (i = 0; i < MMIPIM_MAX_VCAL_NUM; i++)
    {
        //if(VCAL_RECENT_DELETE==g_vcal_log_inf.vcal_status_list_ptr[i] )
        {
            g_vcal_log_inf.vcal_status_list_ptr[i] = (uint8)pim_status;
        }
    }
    
    //没有返回值，如何保证写入结果的正确性?
    MMI_WriteNVItem(MMI_PIM_VCAL_LOG_INFO,g_vcal_log_inf.vcal_status_list_ptr);
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetRecentPBItemsCount
 * decr      : 获得add\del\mod各状态条数
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetRecentVCALItemsCount(VCAL_PIM_STATUS_E  pim_status)
{
    uint16 i = 0; 
    uint16 result = 0;
    
    //syncml_cb_message:"MMIPIM: MMIVCAL_GetRecentVCALItemsCount ,pim_status = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2694_112_2_18_2_47_23_305,(uint8*)"d",pim_status);
    for (i = 1; i <= MMIPIM_MAX_VCAL_NUM; i++)
    {
        if (MMIVCAL_GetPimStatus(i) == pim_status)
        result++;
    }
    return (result);
    
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetAllVCALItemsCount
 * decr      : 获得已经存储的日程item数
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetAllVCALItemsCount(void)
{
    uint16  result=0;
    
    result=MMIAPISCH_GetAllSchNumber();
    //syncml_cb_message:"MMIPIM: MMIVCAL_GetAllVCALItemsCount all_vcal_item=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2717_112_2_18_2_47_23_306,(uint8*)"d",result);
    return (result);
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIVCAL_GetFreeItemsCount
 * decr      : 获得日程的剩余itme数
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/4/2009 
 ==========================================================*/
PUBLIC uint16 MMIVCAL_GetFreeItemsCount(void)
{
    uint16  result=0;
    result=MMIPIM_MAX_VCAL_NUM-MMIVCAL_GetAllVCALItemsCount();
    //syncml_cb_message:"MMIPIM: MMIVCAL_GetFreeItemsCount free_num=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2733_112_2_18_2_47_23_307,(uint8*)"d",result);
    return (result);
}



/*==========================================================
 * author    : wangzhefeng      
 * function  : MMIPIM_IsScheItemValid
 * decr      : 获得此记录号的item是否为空
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIPIM_IsScheItemValid(
                                    uint16 entry_id //IN: 记录号
                                    )
{
    //BOOLEAN retval = FALSE;
    MMIACC_SMART_EVENT_FAST_TABLE_T *fast_event_ptr = NULL;
    
    fast_event_ptr = MMIALM_GeEvent(entry_id+EVENT_SCH_0-1);/*lint !e605*/
    //syncml_cb_message:"MMIPIM @@ MMIPIM_IsScheItemValid=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCALENDAR_PARSE_2755_112_2_18_2_47_23_308,(uint8*)"d",fast_event_ptr->is_valid);
    return fast_event_ptr->is_valid;
}


#endif /* HS_PIM_VCAL */


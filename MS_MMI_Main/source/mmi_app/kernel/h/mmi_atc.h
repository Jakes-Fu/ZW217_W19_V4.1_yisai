/****************************************************************************
** File Name:      mmi_atc.h                                               *
** Author:                                                                 *
** Date:           03/11/2004                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 MMMI_SyncTool interface                                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2004        Lin.Lin           Create
** 
****************************************************************************/
#ifndef MMI_ATC_H
#define MMI_ATC_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"//@zhaohui,cr99836
//#include "mmipb_mnapi.h"

#ifdef ATEST_SUPPORT
#include "mmi_osbridge.h"
#endif //ATEST_SUPPORT
#include "mmi_custom_define.h"
#include "mmipb_common.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMI_SYNCTOOL_V3
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//AUTO TEST AT命令的书写方式: at##AUTOTEST#XX#YY#ZZ#String
/**--------------------------------------------------------------------------*
 **                         Auto test Module id                              *
 ** Note: use can add new module id, at tail, when add new module function to test,   *
 **       if this module id already exist in flow, you neednot add it        *
 **       the module id must 
 **--------------------------------------------------------------------------*/
#define MMI_UT_SMS		        1
#define MMI_UT_MMS		        2
#define MMI_UT_WAP		        3
#define MMI_UT_CC		        4
#define MMI_UT_USB		        5  
#define MMI_UT_JAVA		        6 
#define MMI_UT_CONNECTION       7
#define MMI_UT_MAINMENU         8
#define MMI_UT_PUSH_MAIL     9

#define MMI_UT_LCD_BRIGHTNESS        10

#define MMI_UT_AUTOTEST_ALLSETTINGS     11   /* CR247389 */
/**--------------------------------------------------------------------------*
 **                         命令操作                                         *
 ** Note: use can add function at tail                                       *
 **--------------------------------------------------------------------------*/
#define MMI_AUTO_TEST_INPUT_STRING  1
#define MMI_AUTO_TEST_OFF   0//close 
#define MMI_AUTO_TEST_ON  1//open 
#define MMI_AUTO_TEST_ACO_OFF   0//close auto answer call--on 
#define MMI_AUTO_TEST_ACO_ON  1//open auto answer call--on 
#define MMI_AUTO_TEST_HEADSET_MODE_OFF 2//close vt默认为耳机模式 
#define MMI_AUTO_TEST_HEADSET_MODE_ON  3//open vt默认为耳机模式
#define MMI_AUTO_TEST_SET_ICON_MENU 1
#define MMI_AUTO_TEST_SET_PAGE_MENU 2
#define MMI_AUTO_TEST_SET_TITLE_ICON_MENU 3
#define MMI_AUTO_TEST_SET_LIDE_PAGE_MENU 4

#define MMI_AUTO_TEST_BACKLIGHT_FOREVER_OFF 0
#define MMI_AUTO_TEST_BACKLIGHT_FOREVER_ON 1
#define MMI_AUTO_TEST_BACKLIGH_ON 2


#ifndef CHAR
typedef char                    CHAR;
#endif
/**--------------------------------------------------------------------------*
 **                         字符串的编码格式                                 *
 **--------------------------------------------------------------------------*/
//define text format
#define MMI_UT_UTF8                1
#define MMI_UT_UNICODE             2
#define MMI_UT_GB2312              3
#define MMI_UT_STRUCT              4

//MMI AT命令的书写方式: at##MMIxxx
#define MMI_COLON_SYMBOL			":"
#define MMI_COMMA_SYMBOL			","
#define AT_SYNC_INDICATE			"AT+"
#define MMI_SYNC_INDICATE			"MMI"
#define MMI_NEW_LINE_SYMBOL     	"\n"
#define SYNC_TO_MMI_FINISH_SYMBOL	"\\r\\n"
#define MMI_SYNC_UT_INDICATE_UP     "AUTOTEST"
#define MMI_SYNC_UT_INDICATE_LOW     "autotest"
#define MMI_UT_CMD_INTERVAL_FLAG     '#'
#define MMI_SYNC_START              "READY?"
#define MMI_SYNC_VERSION            "ver3."  //version


#define MMI_TO_SYNC_OK_SYMBOL		"\r\nMMI\r\nOK\r\n"
#define MMI_TO_SYNC_ERROR_SYMBOL	"ERROR\r\n"
#define MMI_SIM_VERSION_LEN          40
#define MMI_MAX_INDICATE_LEN         16

//module
#define MMI_SYNC_PB			        '0'
#define MMI_SYNC_SMS		        '1'
#define MMI_SYNC_RING		        '2'
#define MMI_SYNC_PICTURE		    '3'
#define MMI_SYNC_SCHEDULE        	'4'
#define MMI_SYNC_CALLLOG		    '5'
#define MMI_SYNC_PBGROUP  	    	'6'

//operation
#define MMI_SYNC_GETINFO		    '0'
#define MMI_SYNC_ADD		        '1'
#define MMI_SYNC_DELETE		        '2'
#define MMI_SYNC_MODIFY		        '3'
#define MMI_SYNC_GETITEM	        '4'

//sms
#define SMS_SYNC_POS_ME				'0'
#define SMS_SYNC_POS_SIM_1	        '1'
#define SMS_SYNC_POS_SIM_2	        '2'
#define SMS_SYNC_POS_SIM_3	        '3'
#define SMS_SYNC_POS_SIM_4	        '4'

#define SMS_SYNC_STATUS_MT	        '0'
#define SMS_SYNC_STATUS_MO	        '1'

//pb
#define PB_SYNC_POS_ME              '0'
#define PB_SYNC_POS_SIM_1           '1'
#define PB_SYNC_POS_SIM_2           '2'
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
#define PB_SYNC_POS_SIM_3           '3'
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
#define PB_SYNC_POS_SIM_4           '4'
#endif

/*+cr251711*/
#define UEIT_TEST_INDICATE			    "UEIT" 
#define UEIT_TEST_OK_SYMBOL		      "\r\nUEIT\r\nOK\r\n"
/*-cr251711*/
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
/*                                    Begin of Atest                                                      */
/* Description:  Hanlde at cmd for auto test cases                                                                    */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*****************************************************************************/
/*---------------------------------------------------------------------------*/

//add by shuyan.hu
#define    MMI_AT_TEST "atest"
#define   MAX_CMD_LEN            500
#define   CMD_TEM_LEN       10


/*---------------------------------------------------------------------------*/
/*****************************************************************************/
/*                                    End of Atest                                                      */
/* Description:  Hanlde at cmd for auto test cases                                                                    */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*****************************************************************************/
/*---------------------------------------------------------------------------*/

//以下的宏定义用于PC和MMI AT之间传输数据时使用(主要是传输上的一些规定)，不同于手机内部电话本、群组、日程等模块
//中的各自的参数定义, 例如MMI_SYNC_PB_NAME_MAX_LEN表示，用20个字节来传输电话本记录中的姓名一项，姓名长度如果
//不足20个字节，结尾用填充符填充（注：这个长度总是大于电话本中规定的姓名最大长度，否则，传输错误或不完全）
//phonebook
#define MMI_SYNC_PB_MAX_ENTRY_NUM           1024
#ifdef CMCC_UI_STYLE
#undef MMI_SYNC_PB_MAX_ENTRY_NUM
#define MMI_SYNC_PB_MAX_ENTRY_NUM           1536
#endif
#define MMI_SYNC_PB_NAME_MAX_LEN            24 //注意，必须大于等于PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN
#define MMI_SYNC_PB_NUM_MAX_LEN             40
#define MMI_SYNC_PB_MAX_MAIL_LEN 	        40
#define MMI_SYNC_PB_MAX_NV_PHONE_NUM	    MMIPB_MAX_NV_PHONE_NUM

//group
#define MMI_SYNC_GROUP_MAX_NUM              MMIPB_MAX_GROUP_NUM
#define	MMI_SYNC_GROUP_NAME_MAX_LEN		    20
#define ATC_TEMP_BUF_LEN                    50
//schedule
#define	SCHEDULE_MAX_ENTRY_NUM        	      20
#define MMI_SYNC_EVENT_TEXT_MAX_LEN           40

#define MMI_ID_TAG       "ID:"
#define PB_NAME_TAG "FN:"
#define PB_MOBILE_TAG "TEL(mobile):"
#define PB_WORK_TAG "TEL(work):"
#define PB_HOME_TAG "TEL(home):"
#define PB_FAX_TAG "TEL(fax):"
#define PB_EMAIL_TAG "EMAIL:"
#define PB_MEMO_TAG "NOTE:"
#define PB_GROUP_TAG "GROUP:("
#define CMD_LINE_END_TAG "\\r\\n"


#define MMI_TITLE_SYMBOL       "TITLE:"
#define MMI_STARTDATE_SYMBOL  "STARTDATE:"
#define MMI_ENDDATE_SYMBOL  "ENDDATE:"
#define MMI_TIME_SYMBOL  "TIME:"
#define MMI_MODE_SYMBOL  "MODE:"
#define MMI_FRE_SYMBOL  "FRE:("

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/



typedef struct _PB_HEAD_INFO_T
{
    BOOLEAN is_valid;   //TRUE，存储器对应的电话簿有效; 反之，无效
    uint8	max_alpha_len;	 //存储器中支持的电话簿姓名的最大长度
    uint8   max_num_len;    //存储器中支持的电话簿号码的最大长度
    uint8   max_email_len;
    uint8   max_memo_len;
    uint16	max_record_num; //存储器中最多可以保存的记录数
    uint16  used_record_num; //存储器中已经保存的记录数
    uint8   all_entry_id[(MMI_SYNC_PB_MAX_ENTRY_NUM+7)/8+1];
}PB_HEAD_INFO_T;

typedef struct _PB_NAME_T
{
    BOOLEAN is_ucs2;        //字符串的编码类型
    uint16   alpha_len;      //alpha数组的BYTE个数
    uint16   alpha[MMI_SYNC_PB_NAME_MAX_LEN+1];      //alph string
}PB_NAME_T;

typedef enum
{
    PB_INVLID_NUMBER,
    PB_MOBILE_NUMBER,
    PB_WORK_NUMBER,
    PB_HOME_NUMBER,
    PB_FAX_NUMBER,
    PB_NUMBER_MAX
}PB_NUMBER_TYPE_E;


typedef struct PB_NUMBER_T
{
    PB_NUMBER_TYPE_E number_type;
    uint8   number_len;     //number数组的BYTE个数
    uint8   number[MMI_SYNC_PB_NUM_MAX_LEN+1];     //字符串
}PB_NUMBER_T;

// 电话簿中的邮箱
typedef struct _PB_MAIL_T
{
  //  BOOLEAN is_ucs2;                     // 字符串的编码类型
    uint16   mail_len;                    // mail数组的BYTE个数
	uint16   mail[MMI_SYNC_PB_MAX_MAIL_LEN+1];    // 具体的mail地址
} PB_MAIL_T;
 #ifndef PB_SUPPORT_LOW_MEMORY
typedef struct PB_MEMO_T
{
    uint16   note_len;     //number数组的BYTE个数
    uint16   note[MMIPB_MAX_CUSTOM_TEXT_LEN+1];     //字符串
}PB_MEMO_T;
#endif
typedef struct _PB_PHONEBOOK_ENTRY_T
{
    uint16          entry_id;       //记录的id号
    PB_NAME_T		entry_name;     //姓名
    PB_NUMBER_T	    number_t[MMI_SYNC_PB_MAX_NV_PHONE_NUM];    //电话号码，依次为个人、住宅和公司
    PB_MAIL_T	    mail;           //邮箱地址
    #ifndef PB_SUPPORT_LOW_MEMORY
	PB_MEMO_T       memo;
    #endif
	uint32          group;
}PB_PHONEBOOK_ENTRY_T;

//多媒体信息，包括照片，图片，铃声
typedef struct 
{
    uint16 max_number;      //最大数量
    uint16 total_number;    //目前数量
    uint32 max_memory;      //最大内存
    uint32 total_memory;    //目前所用内存,铃声信息中该数据无意义
}MULTIM_INFO_T;


//Group struct
//group infor header
typedef struct _GROUP_HEAD_INFO_T
{
    uint16	    max_name_len;	   		//max group name len
    uint16	    max_group_num;     		//max group num
  //  uint16      used_num;          		//used group num
    uint8      all_group_id[((MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM)+7)/8+1];
}GROUP_HEAD_INFO_T;

//record infor
typedef struct _GROUP_ENTRY_T
{
    uint32      group_id;       			    //id
    BOOLEAN		is_fixed_group;                 //fixed group can't be deleted or modified
  //  BOOLEAN     is_ucs2;
    uint16      name_len;
    uint16       group_name[MMI_SYNC_GROUP_NAME_MAX_LEN+1]; //group name
 //   uint16      member_num;                     //how many members in this group
}GROUP_ENTRY_T;


//schedule struct
//schedule info header
typedef struct _SCHEDULE_HEAD_INFO_T
{
    uint16	 			max_count;	      		//max memo text len
    uint16	 			title_max_len;   		    //max memo record num
    uint8  			all_entry_id[(SCHEDULE_MAX_ENTRY_NUM+7)/8+1];  
}SCHEDULE_HEAD_INFO_T;



typedef struct _SCHEDULE_TIME_T
{
    uint16				year;
    uint8				month;
    uint8				day;
    uint8				hour;
    uint8				minute;
    uint8				sec;
}SCHEDULE_TIME_T;

typedef enum{
    ACC_ALARM_NULL,
    ACC_ALARM_ONCE,
    ACC_ALARM_EVERYDAY,
    ACC_ALARM_EVERYWEEK,
    ACC_ALARM_EVERYMONTH,
    ACC_ALARM_EVERYYEAR,	
    ACC_ALARM_NO_ALARM,
    ACC_ALARM_MAX_TYPE    
}ACC_ALARM_TYPE_E;
//record info
typedef struct _SCHEDULE_ENTRY_T
{
    uint16 title[MMI_SYNC_EVENT_TEXT_MAX_LEN+1];
    uint8   title_len;
    uint8               entry_id;
    uint8               frequece;//bit0,...bit6

    SCHEDULE_TIME_T		start_time;
    SCHEDULE_TIME_T     end_time;
    ACC_ALARM_TYPE_E    alarm_mode;
    
}SCHEDULE_ENTRY_T;

//atest support
#ifdef ATEST_SUPPORT

typedef struct
{
	uint8  language_type;
	char* language_name;
}ATEST_CUR_LANG_T;

typedef enum{
    NONE_ID_TYPE,
    TEXT_TYPE,
    TEXT_ID_TYPE,
    CONTROL_ID_TYPE,
    IMAGE_ID_TYPE,
    INDEX_ID_TYPE,
    IMAGE_INDEX_TYPE,
    TEXT_IMG_TYPE,
    MAX_ID_TYPE    
}ATEST_ID_TYPE_E;

typedef enum{
	ATEST_SIGNAL_NONE,
	ATEST_SIGNAL_KB_DOWN = 0xFFC0,
 	ATEST_SIGNAL_KB_UP,		
 	ATEST_SIGNAL_KB_LEFT,		
 	ATEST_SIGNAL_KB_RIGHT, 		
	ATEST_SIGNAL_IGCT,
	ATEST_SIGNAL_IGCD,
	ATEST_SIGNAL_IFCD,	
	ATEST_SIGNAL_ISCP,
	ATEST_SIGNAL_IGCL,
	ATEST_SIGNAL_IGCR,
	ATEST_SIGNAL_IGLN,
	ATEST_SIGNAL_IRIW,
	ATEST_SIGNAL_ICES,
	ATEST_SIGNAL_IKBD,
	ATEST_SIGNAL_ITPR,
	ATEST_SIGNAL_ITPD,
	ATEST_SIGNAL_ITPM,
	ATEST_SIGNAL_ITPU,
	ATEST_SIGNAL_TPLONG,
	ATEST_SIGNAL_IGCS,
	ATEST_SIGNAL_ISAT,
	ATEST_SIGNAL_IGST,
	ATEST_SIGNAL_IGAT,
	ATEST_SIGNAL_ICCD,
	ATEST_SIGNAL_ICAL,
	ATEST_SIGNAL_MAX
}ATEST_SIGNAL_TYPE_E;

#endif //ATEST_SUPPORT


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function                                                                */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 10/1/2011                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/***************************************************************************
Function: handle pb create message for atest 
//-------------------------------------------------------------------------
// Input Parameter  :none
// Output Parameter : NONE
// Return Value     : none
//--------------------------------------------------------------------------
//  Global resource dependence : g_mmipb_entry_t
//  Author: shuyan.hu
//  Note: 
****************************************************************************/
PUBLIC void Atest_HandlePbCreateSimMessage();

/*****************************************************************************/
// 	Description : response to atc cmd
//	Global resource dependence : none
//  Author:     
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIATC_Response(BOOLEAN result);

//atest support
#ifdef ATEST_SUPPORT

/*****************************************************************************/
// 	Description : ATEST拦截外部消息并处理
//	Global resource dependence : none
//  Author: xiaolei.zheng
//	Note:
/*****************************************************************************/
PUBLIC void ATest_DispatchExtSig(MmiSignalS** signal_ptr);

#endif // ATEST_SUPPORT

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    end of Atest  Function                                                                */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 10/1/2011                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif //#ifdef MMI_SYNCTOOL_V3

#ifdef MMI_SYNCTOOL_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//AUTO TEST AT命令的书写方式: at##AUTOTEST#XX#YY#ZZ#String
/**--------------------------------------------------------------------------*
 **                         Auto test Module id                              *
 ** Note: use can add new module id, at tail, when add new module function to test,   *
 **       if this module id already exist in flow, you neednot add it        *
 **       the module id must 
 **--------------------------------------------------------------------------*/
#define MMI_UT_SMS		        1
#define MMI_UT_MMS		        2
#define MMI_UT_WAP		        3
#define MMI_UT_CC		        4
#define MMI_UT_USB		        5  
#define MMI_UT_JAVA		        6 
#define MMI_UT_CONNECTION       7
#define MMI_UT_MAINMENU         8
#define MMI_UT_PUSH_MAIL     9

#define MMI_UT_LCD_BRIGHTNESS        10

#define MMI_UT_AUTOTEST_ALLSETTINGS     11   /* CR247389 */
/**--------------------------------------------------------------------------*
 **                         命令操作                                         *
 ** Note: use can add function at tail                                       *
 **--------------------------------------------------------------------------*/
#define MMI_AUTO_TEST_INPUT_STRING  1
#define MMI_AUTO_TEST_OFF   0//close 
#define MMI_AUTO_TEST_ON  1//open 
#define MMI_AUTO_TEST_ACO_OFF   0//close auto answer call--on 
#define MMI_AUTO_TEST_ACO_ON  1//open auto answer call--on 
#define MMI_AUTO_TEST_HEADSET_MODE_OFF 2//close vt默认为耳机模式 
#define MMI_AUTO_TEST_HEADSET_MODE_ON  3//open vt默认为耳机模式
#define MMI_AUTO_TEST_SET_ICON_MENU 1
#define MMI_AUTO_TEST_SET_PAGE_MENU 2
#define MMI_AUTO_TEST_SET_TITLE_ICON_MENU 3
#define MMI_AUTO_TEST_SET_LIDE_PAGE_MENU 4

#define MMI_AUTO_TEST_BACKLIGHT_FOREVER_OFF 0
#define MMI_AUTO_TEST_BACKLIGHT_FOREVER_ON 1
#define MMI_AUTO_TEST_BACKLIGH_ON 2


/**--------------------------------------------------------------------------*
 **                         字符串的编码格式                                 *
 **--------------------------------------------------------------------------*/
//define text format
#define MMI_UT_UTF8                1
#define MMI_UT_UNICODE             2
#define MMI_UT_GB2312              3
#define MMI_UT_STRUCT              4

//MMI AT命令的书写方式: at##MMIxxx
#define MMI_SYNC_INDICATE			"MMI"
#define SYNC_TO_MMI_FINISH_SYMBOL	"\r\n"
#define MMI_SYNC_UT_INDICATE_UP     "AUTOTEST"
#define MMI_SYNC_UT_INDICATE_LOW     "autotest"
#define MMI_UT_CMD_INTERVAL_FLAG     '#'
#define MMI_SYNC_START              "READY?"
#define MMI_SYNC_VERSION            "ver2."  //version


#define MMI_TO_SYNC_OK_SYMBOL		"\r\nMMI\r\nOK\r\n"
#define MMI_TO_SYNC_ERROR_SYMBOL	"ERROR\r\n"
#define MMI_SIM_VERSION_LEN          40
#define MMI_MAX_INDICATE_LEN         16

//module
#define MMI_SYNC_PB			        '0'
#define MMI_SYNC_SMS		        '1'
#define MMI_SYNC_RING		        '2'
#define MMI_SYNC_PICTURE		    '3'
#define MMI_SYNC_MEMO	        	'4'
#define MMI_SYNC_CALLLOG		    '5'
#define MMI_SYNC_PBGROUP  	    	'6'

//operation
#define MMI_SYNC_GETINFO		    '0'
#define MMI_SYNC_ADD		        '1'
#define MMI_SYNC_DELETE		        '2'
#define MMI_SYNC_MODIFY		        '3'
#define MMI_SYNC_GETITEM	        '4'

//sms
#define SMS_SYNC_POS_ME				'0'
#define SMS_SYNC_POS_SIM_1	        '1'
#define SMS_SYNC_POS_SIM_2	        '2'
#define SMS_SYNC_POS_SIM_3	        '3'
#define SMS_SYNC_POS_SIM_4	        '4'

#define SMS_SYNC_STATUS_MT	        '0'
#define SMS_SYNC_STATUS_MO	        '1'

//pb
#define PB_SYNC_POS_ME              '0'
#define PB_SYNC_POS_SIM_1           '1'
#define PB_SYNC_POS_SIM_2           '2'
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
#define PB_SYNC_POS_SIM_3           '3'
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
#define PB_SYNC_POS_SIM_4           '4'
#endif

/*+cr251711*/
#define UEIT_TEST_INDICATE			    "UEIT" 
#define UEIT_TEST_OK_SYMBOL		      "\r\nUEIT\r\nOK\r\n"
/*-cr251711*/
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
/*                                    Begin of Atest                                                      */
/* Description:  Hanlde at cmd for auto test cases                                                                    */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*****************************************************************************/
/*---------------------------------------------------------------------------*/

//add by shuyan.hu
#define    MMI_AT_TEST "atest"
#define   MAX_CMD_LEN            500
#define   CMD_TEM_LEN       10


/*---------------------------------------------------------------------------*/
/*****************************************************************************/
/*                                    End of Atest                                                      */
/* Description:  Hanlde at cmd for auto test cases                                                                    */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*****************************************************************************/
/*---------------------------------------------------------------------------*/

//以下的宏定义用于PC和MMI AT之间传输数据时使用(主要是传输上的一些规定)，不同于手机内部电话本、群组、日程等模块
//中的各自的参数定义, 例如MMI_SYNC_PB_NAME_MAX_LEN表示，用20个字节来传输电话本记录中的姓名一项，姓名长度如果
//不足20个字节，结尾用填充符填充（注：这个长度总是大于电话本中规定的姓名最大长度，否则，传输错误或不完全）
//phonebook
#define MMI_SYNC_PB_MAX_ENTRY_NUM           1024
#define MMI_SYNC_PB_NAME_MAX_LEN            241 //注意，必须大于等于PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN
#define MMI_SYNC_PB_NUM_MAX_LEN             40
#define MMI_SYNC_PB_MAX_MAIL_LEN 	        40
#define MMI_SYNC_PB_MAX_NV_PHONE_NUM	    3

//group
#define MMI_SYNC_GROUP_MAX_NUM              20
#define	MMI_SYNC_GROUP_NAME_MAX_LEN		    20

//memo
#define	MEMO_MAX_ENTRY_NUM        	        100
#define	MEMO_TEXT_MAX_LEN			        80


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _PB_HEAD_INFO_T
{
    BOOLEAN is_valid;   //TRUE，存储器对应的电话簿有效; 反之，无效
    uint8	max_alpha_len;	 //存储器中支持的电话簿姓名的最大长度
    uint8   max_num_len;    //存储器中支持的电话簿号码的最大长度
    uint8   max_email_len;
    uint16	max_record_num; //存储器中最多可以保存的记录数
    uint16  used_record_num; //存储器中已经保存的记录数
    uint8   all_entry_id[MMI_SYNC_PB_MAX_ENTRY_NUM/8];
}PB_HEAD_INFO_T;

typedef struct _PB_NAME_T
{
    BOOLEAN is_ucs2;        //字符串的编码类型
    uint8   alpha_len;      //alpha数组的BYTE个数
    uint8   alpha[MMI_SYNC_PB_NAME_MAX_LEN];      //alph string
}PB_NAME_T;


typedef struct PB_NUMBER_T
{
    uint8   number_len;     //number数组的BYTE个数
    uint8   number[MMI_SYNC_PB_NUM_MAX_LEN];     //字符串
}PB_NUMBER_T;

// 电话簿中的邮箱
typedef struct _PB_MAIL_T
{
    BOOLEAN is_ucs2;                     // 字符串的编码类型
    uint8   mail_len;                    // mail数组的BYTE个数
	uint8   mail[MMI_SYNC_PB_MAX_MAIL_LEN];    // 具体的mail地址
} PB_MAIL_T;

typedef struct _PB_PHONEBOOK_ENTRY_T
{
    uint16          entry_id;       //记录的id号
    PB_NAME_T		entry_name;     //姓名
    PB_NUMBER_T	    number_t[MMI_SYNC_PB_MAX_NV_PHONE_NUM];    //电话号码，依次为个人、住宅和公司
    PB_MAIL_T	    mail;           //邮箱地址
    uint8           group_id;
}PB_PHONEBOOK_ENTRY_T;

//多媒体信息，包括照片，图片，铃声
typedef struct 
{
    uint16 max_number;      //最大数量
    uint16 total_number;    //目前数量
    uint32 max_memory;      //最大内存
    uint32 total_memory;    //目前所用内存,铃声信息中该数据无意义
}MULTIM_INFO_T;


//Group struct
//group infor header
typedef struct _GROUP_HEAD_INFO_T
{
    uint16	    max_name_len;	   		//max group name len
    uint16	    max_group_num;     		//max group num
    uint16      used_num;          		//used group num
    uint16      all_entry_id[MMI_SYNC_GROUP_MAX_NUM];
}GROUP_HEAD_INFO_T;

//record infor
typedef struct _GROUP_ENTRY_T
{
    uint16      entry_id;       			    //id
    BOOLEAN		is_fixed_group;                 //fixed group can't be deleted or modified
    BOOLEAN     is_ucs2;
    uint16      name_len;
    uint8       group_name[MMI_SYNC_GROUP_NAME_MAX_LEN]; //group name
    uint16      member_num;                     //how many members in this group
}GROUP_ENTRY_T;


//Memo struct
//memo info header
typedef struct _MEMO_HEAD_INFO_T
{
    uint16	 			max_text_len;	      		//max memo text len
    uint16	 			max_record_num;   		    //max memo record num
    uint16  			used_record_num;   		    //used memo record num
    uint16  			all_entry_id[MEMO_MAX_ENTRY_NUM];  
}MEMO_HEAD_INFO_T;

//memo time struct
typedef enum{
    MEMO_NULL,
    MEMO_MODE_ONCE,
    MEMO_MODE_EVERYDAY,
    MEMO_MODE_EVERYWEEK,
    MEMO_MODE_EVERYMONTH,
    MEMO_MODE_EVERYYEAR,	
    MEMO_MODE_NO_ALARM,
    MEMO_MODE_MAX_TYPE    
}MEMO_MODE_TYPE_E;

typedef struct _MEMO_TIME_T
{
    uint16				year;
    uint8				month;
    uint8				day;
    uint8				hour;
    uint8				minute;
}MEMO_TIME_T;

//memo text struct
typedef struct _MEMO_TEXT_T
{
    uint8               fill_in;
    BOOLEAN 			is_ucs2;        			//is ucs2 code
    uint16    			text_len;         			//text length
    uint8     			text[MEMO_TEXT_MAX_LEN+1];    //text string//coverity 12658
}MEMO_TEXT_T;

//record info
typedef struct _MEMO_ENTRY_T
{
    uint16          	entry_id;       //id
    MEMO_TIME_T		    memo_time;
    uint16              memo_mode;
    MEMO_TEXT_T		    memo_text;
    
    MEMO_TIME_T		    memo_start_time;
    MEMO_TIME_T		    memo_end_time;
    uint8               fre;
    uint8               reserved1;
    uint16              reserved2;
    uint32              service_id;
    
}MEMO_ENTRY_T;

//atest support
#ifdef ATEST_SUPPORT

typedef struct
{
	uint8  language_type;
	char* language_name;
}ATEST_CUR_LANG_T;

typedef enum{
    NONE_ID_TYPE,
    TEXT_TYPE,
    TEXT_ID_TYPE,
    CONTROL_ID_TYPE,
    IMAGE_ID_TYPE,
    INDEX_ID_TYPE,
    IMAGE_INDEX_TYPE,
    TEXT_IMG_TYPE,
    MAX_ID_TYPE    
}ATEST_ID_TYPE_E;

typedef enum{
	ATEST_SIGNAL_NONE,
	ATEST_SIGNAL_KB_DOWN = 0xFFC0,
 	ATEST_SIGNAL_KB_UP,		
 	ATEST_SIGNAL_KB_LEFT,		
 	ATEST_SIGNAL_KB_RIGHT, 		
	ATEST_SIGNAL_IGCT,
	ATEST_SIGNAL_IGCD,
	ATEST_SIGNAL_IFCD,	
	ATEST_SIGNAL_ISCP,
	ATEST_SIGNAL_IGCL,
	ATEST_SIGNAL_IGCR,
	ATEST_SIGNAL_IGLN,
	ATEST_SIGNAL_IRIW,
	ATEST_SIGNAL_ICES,
	ATEST_SIGNAL_IKBD,
	ATEST_SIGNAL_ITPR,
	ATEST_SIGNAL_ITPD,
	ATEST_SIGNAL_ITPM,
	ATEST_SIGNAL_ITPU,
	ATEST_SIGNAL_TPLONG,
	ATEST_SIGNAL_IGCS,
	ATEST_SIGNAL_ISAT,
	ATEST_SIGNAL_IGST,
	ATEST_SIGNAL_IGAT,
	ATEST_SIGNAL_ICCD,
	ATEST_SIGNAL_ICAL,
	ATEST_SIGNAL_MAX
}ATEST_SIGNAL_TYPE_E;

#endif //ATEST_SUPPORT

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : This function call parse function to get result of command response
//	Global resource dependence : none
//  Author:       Jassmine.Meng
//  modify:baokun.yin
//	Note:
/*****************************************************************************/
uint32 ATC_ExtCmd(DPARAM param);

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncHeadInfo( 
							    GROUP_HEAD_INFO_T *  group_head_info_ptr  // OUT
							    );

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncRead( 
                            uint16           entry_id,    //IN:
							GROUP_ENTRY_T *  group_item_info_ptr  // IN & OUT
							);

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncAdd( 
							GROUP_ENTRY_T *group_item_info_ptr  // IN
							);

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncDelete( 
							uint16           entry_id  // IN
							);

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncModify( 
                            uint16           entry_id,    //IN:
							GROUP_ENTRY_T *  group_item_info_ptr  // IN 
							);


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncHeadInfo( 
							MEMO_HEAD_INFO_T *  memo_head_info_ptr  // OUT
							);


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncAdd( 
						MEMO_ENTRY_T *  memo_item_info_ptr,  // IN
                        uint16      *index
						);


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncDelete( 
							uint16           entry_id  // IN
							);

/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncModify( 
							uint16           entry_id,    //IN:
							MEMO_ENTRY_T *   memo_item_info_ptr  // IN 
							);

/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncRead( 
						uint16           entry_id,    //IN:
						MEMO_ENTRY_T *   memo_item_info_ptr  //OUT
						);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function                                                                */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 10/1/2011                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/***************************************************************************
Function: handle pb create message for atest 
//-------------------------------------------------------------------------
// Input Parameter  :none
// Output Parameter : NONE
// Return Value     : none
//--------------------------------------------------------------------------
//  Global resource dependence : g_mmipb_entry_t
//  Author: shuyan.hu
//  Note: 
****************************************************************************/
PUBLIC void Atest_HandlePbCreateSimMessage();

/*****************************************************************************/
// 	Description : response to atc cmd
//	Global resource dependence : none
//  Author:     
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIATC_Response(BOOLEAN result);

//atest support
#ifdef ATEST_SUPPORT

/*****************************************************************************/
// 	Description : ATEST拦截外部消息并处理
//	Global resource dependence : none
//  Author: xiaolei.zheng
//	Note:
/*****************************************************************************/
PUBLIC void ATest_DispatchExtSig(MmiSignalS** signal_ptr);

#endif // ATEST_SUPPORT
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    end of Atest  Function                                                                */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 10/1/2011                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


#endif //#ifdef MMI_SYNCTOOL_SUPPORT
#endif






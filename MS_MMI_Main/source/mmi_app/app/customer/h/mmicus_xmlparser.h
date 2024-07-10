/*****************************************************************************
** File Name:      mmicus_xmlparser.h                                                *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe Customer XML               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
******************************************************************************/
#ifndef _MMICUS_XMLPARSER_H_
#define _MMICUS_XMLPARSER_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmicus_export.h"

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


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**解析 结构定义                                                          *
**--------------------------------------------------------------------------*/ 
typedef struct   xml_attr_tag
{             //描述tag的数据结构
    uint8 * att_name;                      //描述type类型，如<value = "111">，这个att_name表示value
    uint8 * att_value;                     //描述value值是多少，value表示"111",或者<name = "NAME">中的"NAME"
    struct xml_attr_tag *next;             //描述同tag里面的下一个属性，<name = "NAME" value = "111">第一个表示name，那么next表示value
}XML_ATTR_T;                               //命名为XML的别名

typedef struct xml_tag_item_tag
{         //XML使用的xml树结构
    XML_ATTR_T     *tag_property;          //标签属性链表
    uint8		   *tag_add_content;       //标签附件内容    
    uint16	       index;                  //树的层次索引
    uint8	       *tag_name;              //标签类型，如<parm name = "NAME" value = "111">中的parm
    BOOLEAN        get_flag;               //当有多个标签的时候，标记是否取用过
    struct	xml_tag_item_tag   *parent;
    struct  xml_tag_item_tag   *child;
    struct  xml_tag_item_tag   *next;
}XML_TAG_ITEM_T;

//setting
//Security
typedef struct
{
    uint32 reserved;
}MMICUS_CSC_SETTING_MAIN_SECURITY_T;

//Sound
typedef struct
{
    BOOLEAN is_ring_tone_volume;
    BOOLEAN is_msg_tone_volume;
    BOOLEAN is_key_tone_volume;
    uint16 ring_tone_volume;
    uint16 msg_tone_volume;
    uint16 key_tone_volume;
    uint8 profile_set[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 ring_tone_alerttype[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 key_tone_alerttype[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 msg_tone_alerttype[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 touch_tone_alerttype[XML_TAG_CONTENT_MAX_LEN + 1];
}MMICUS_CSC_SETTING_MAIN_SOUND_T;

//Call
typedef struct
{
    BOOLEAN is_connection_tone;
    BOOLEAN is_call_end_tone;
    BOOLEAN is_alert_oncall;
    BOOLEAN is_auto_redial;
    BOOLEAN is_anykey_answer;
    BOOLEAN is_minute_mind;
    BOOLEAN connection_tone;
    BOOLEAN call_end_tone;
    BOOLEAN alert_oncall;
    BOOLEAN auto_redial;
    BOOLEAN anykey_answer;
    uint8 minute_mind;
}MMICUS_CSC_SETTING_MAIN_CALL_T;

//Display
typedef struct
{
    BOOLEAN is_backlight_time;
    BOOLEAN is_keylight_time;
    uint32 backlight_time;
    uint32 keylight_time;
}MMICUS_CSC_SETTING_MAIN_DISPLAY_T;

//Shortcut
typedef struct
{
    uint8 up_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 down_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 left_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 right_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 center_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 leftsoft_key[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 rightsoft_key[XML_TAG_CONTENT_MAX_LEN + 1];
}MMICUS_CSC_SETTING_MAIN_PHONE_SHORTCUT_T;

//Phone
typedef struct
{
    uint8 greet_msg[XML_TAG_MESSAGE_MAX_LEN + 1];
    uint8 default_language[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 default_language_nosim[XML_TAG_CONTENT_MAX_LEN + 1];
    BOOLEAN im_enabling;
    uint8 im_language[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 date_format[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 time_format[XML_TAG_CONTENT_MAX_LEN + 1];
}MMICUS_CSC_SETTING_MAIN_PHONE_T;

typedef struct
{      
    MMICUS_CSC_SETTING_MAIN_SECURITY_T security_setting;
    MMICUS_CSC_SETTING_MAIN_SOUND_T sound_setting;
    MMICUS_CSC_SETTING_MAIN_PHONE_T phone_setting;
    MMICUS_CSC_SETTING_MAIN_CALL_T call_setting;
    MMICUS_CSC_SETTING_MAIN_DISPLAY_T display_setting;
    MMICUS_CSC_SETTING_MAIN_NETWORK_T network_setting;
    MMICUS_CSC_SETTING_MAIN_PHONE_SHORTCUT_T shortcut_setting;
}MMICUS_CSC_SETTING_MAIN_T;

typedef struct
{
    uint8 start_date[XML_TAG_CONTENT_MAX_LEN + 1];
 }MMICUS_CSC_SETTING_CALENDAR_T;

typedef struct
{
    BOOLEAN is_status_report;
    BOOLEAN is_cell_broadcast;
    BOOLEAN is_mms_limit;
    BOOLEAN status_report;
    BOOLEAN cell_broadcast;
    uint16 nb_voicemail;
    uint32 mms_limit;
    uint8 save_position[XML_TAG_CONTENT_MAX_LEN + 1];
    MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T * voice_mail;
}MMICUS_CSC_SETTING_MESSAGE_T;

typedef struct
{
    uint8 user_agent[XML_TAG_UA_MAX_LEN + 1];
    uint8 user_agent_profile[XML_TAG_UA_PROFILE_MAX_LEN + 1];
}MMICUS_CSC_SETTING_CONNECTIONS_T;

typedef struct
{  
    MMICUS_CSC_GENERALINFO_T general_setting;
    MMICUS_CSC_SETTING_MAIN_T main_setting;
    MMICUS_CSC_SETTING_CALENDAR_T calendar_setting;
    MMICUS_CSC_SETTING_MESSAGE_T message_setting;
    MMICUS_CSC_SETTING_CONNECTIONS_T connection_setting;
}MMICUS_CSC_SETTING_T; 

/**--------------------------------------------------------------------------*
**Get Value结构定义(主要定义协议映射结构)                                    *
**--------------------------------------------------------------------------*/ 


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         MMICUS_ParseCSCXML
* Function:     Parse Customer Specific Customization XML
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMICUS_ParseCSCXML(void);

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultXMLCSCEdition
* Function:     Get the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultXMLCSCEdition(void);

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultSalesCode
* Function:     Get the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultSalesCode(void);

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultUAFromCSC(void);

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA Profile from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultUAProfileFromCSC(void);

/*******************************************************************/
//  Interface:		MMICUS_GetMatchLang
//  Description : 	Get Match Lang
//  Global resource dependence : 
//  Author: 
//  Note: 
/*******************************************************************/
PUBLIC uint32 MMICUS_GetMatchLang(const char * lang_name);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


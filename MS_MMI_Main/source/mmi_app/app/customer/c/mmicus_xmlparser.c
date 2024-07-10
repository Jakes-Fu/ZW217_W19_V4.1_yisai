/*****************************************************************************
** File Name:      mmicus_xmlparser.c                                                 *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe Customer XML                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 
******************************************************************************/
#include "mmi_app_customer_trc.h"
#ifdef CSC_XML_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "xml_sap.h"
#include "mmicus_xmlparser.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_export.h"
#include "mmi_resource.h"
#include "mmi_data.h"
#include "mmifmm_export.h"
#include "mmienvset_export.h"
#include "mmiset_export.h"
#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif
#include "mmicus_nv.h"
#include "mmiim.h"
#include "mmiim_im_switching.h"
#include "mmiset_call_export.h"
#if defined(MMS_SUPPORT)
#include "mmimms_export.h"
#endif
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define XML_STACK_OBJECT          unsigned long           //stack objects
#define XML_STACK_DEFAULT_GROW   (20)                     //stack defaut growth
#define XML_STACK_SIZE            10                      //stack size
#define XML_STACK_ALLOW_GROW      1                       //stack allow grow

#define XML_SETTING_CSC_ON_TAG  "on"
#define XML_SETTING_CSC_OFF_TAG  "off"
#define XML_SETTING_CSC_ENABLE_TAG  "enable"
#define XML_SETTING_CSC_DISABLE_TAG  "disable"
#define XML_SETTING_CSC_MELODY_TAG  "melody"
#define XML_SETTING_CSC_VIBRATE_TAG  "vibrate"
#define XML_SETTING_CSC_VIBMELODY_TAG  "vibmelody"
#define XML_SETTING_CSC_VIBBEFOREMELODY_TAG  "vibbeforemelody"
#define XML_SETTING_CSC_STANDARD_TAG  "standard"
#define XML_SETTING_CSC_SLIENT_TAG  "slient"
#define XML_SETTING_CSC_MEETING_TAG  "meeting"
#define XML_SETTING_CSC_INSIDE_TAG  "inside"
#define XML_SETTING_CSC_NOISY_TAG  "noisy"
#define XML_SETTING_CSC_RINGTONE1_TAG  "ringtone1"
#define XML_SETTING_CSC_RINGTONE2_TAG  "ringtone2"

#define XML_SETTING_CSC_MODAY_TAG  "monday"
#define XML_SETTING_CSC_SUNDAY_TAG  "sunday"

#define XML_SETTING_CSC_12H_TAG  "12h"
#define XML_SETTING_CSC_24H_TAG  "24h"
#define XML_SETTING_CSC_YYYYMMDD_TAG  "yyyymmdd"
#define XML_SETTING_CSC_MMDDYYYY_TAG  "mmddyyyy"
#define XML_SETTING_CSC_DDMMYYYY_TAG  "ddmmyyyy"

#define XML_SETTING_CSC_PHONE_TAG  "phone"
#define XML_SETTING_CSC_SIM_TAG  "sim"

extern int stricmp (const char *a, const char *b);

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
LOCAL uint8 s_csc_edition[XML_TAG_CONTENT_MAX_LEN + 1] = {0};
LOCAL uint8 s_salescode_str[XML_TAG_CONTENT_MAX_LEN + 1] = {0};
LOCAL uint8 *s_csc_ua_str = PNULL;
LOCAL uint8 *s_csc_ua_profile_str = PNULL;

typedef struct
{
    uint32 allow_grow;
    uint32 size;
    uint32 len;
    XML_STACK_OBJECT * stack;
}XML_STACK_T;

typedef enum
{        
    TAG_CUSDATA = 0,
        
    TAG_CUSDATA_GENERALINFO,
    TAG_CUSDATA_GENERALINFO_CSCEDITION,
    TAG_CUSDATA_GENERALINFO_COUNTRY,
    TAG_CUSDATA_GENERALINFO_SALESCODE,
    TAG_CUSDATA_GENERALINFO_NBNETWORKINFO,
    TAG_CUSDATA_GENERALINFO_NETWORKINFO,
    TAG_CUSDATA_GENERALINFO_NETWORKINFO_MCCMNC,
    TAG_CUSDATA_GENERALINFO_NETWORKINFO_NETWORKNAME,
    
    TAG_CUSDATA_SETTING,
    TAG_CUSDATA_SETTING_MAIN,
    TAG_CUSDATA_SETTING_MAIN_SECURITY,
    
    TAG_CUSDATA_SETTING_MAIN_SOUND,
    TAG_CUSDATA_SETTING_MAIN_SOUND_PROFILESET,
    TAG_CUSDATA_SETTING_MAIN_SOUND_RINGTONE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_RINGTONEALERTTYPE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_RINGVOLUME,
    TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONEALERTTYPE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONE_KEYVOLUME,
    TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEALERTTYPE,
    TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEVOLUME,
    TAG_CUSDATA_SETTING_MAIN_SOUND_TOUCHTONE,

    TAG_CUSDATA_SETTING_MAIN_CALL,
    TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND,
    TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CONNECTIONTONE,
    TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CALLENDTONE,
    TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_MINUTEMIND,
    TAG_CUSDATA_SETTING_MAIN_CALL_ALERTONCALL,
    TAG_CUSDATA_SETTING_MAIN_CALL_AUTOREDIAL,
    TAG_CUSDATA_SETTING_MAIN_CALL_ANYKEYANSWER,
    
    TAG_CUSDATA_SETTING_MAIN_PHONE,
    TAG_CUSDATA_SETTING_MAIN_PHONE_GREETMSG,
    TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGE,
    TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGENOSIM,
    TAG_CUSDATA_SETTING_MAIN_PHONE_IMENABLING,
    TAG_CUSDATA_SETTING_MAIN_PHONE_IMLANGUAGE,
    TAG_CUSDATA_SETTING_MAIN_PHONE_DATATIMEFORMAT,
    TAG_CUSDATA_SETTING_MAIN_PHONE_DATEFORMAT,
    TAG_CUSDATA_SETTING_MAIN_PHONE_TIMEFORMAT,

    TAG_CUSDATA_SETTING_MAIN_DISPLAY,
    TAG_CUSDATA_SETTING_MAIN_DISPLAY_BACKLIGHTTIME,
    TAG_CUSDATA_SETTING_MAIN_DISPLAY_KEYLIGHTTIME,
    
    TAG_CUSDATA_SETTING_MAIN_NETWORK,
    TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBER,
    TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBER,
    TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBERNOSIM,
    TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBERNOSIM,
    
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_UPKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_DWONKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_CENTERKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTSOFTKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTSOFTKEY,
    TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM,
    
    TAG_CUSDATA_SETTING_MESSAGES,
    TAG_CUSDATA_SETTING_MESSAGES_SMS,
    TAG_CUSDATA_SETTING_MESSAGES_STAUSREPORT,
    TAG_CUSDATA_SETTING_MESSAGES_SAVEPOSITION,
    TAG_CUSDATA_SETTING_MESSAGES_CELLBROADCAST,
    TAG_CUSDATA_SETTING_MESSAGES_MMS,
    TAG_CUSDATA_SETTING_MESSAGES_MMSLIMIT,
    TAG_CUSDATA_SETTING_MESSAGES_NBVOICEMAIL,
    TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL,
    TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_VOICEMAILNAME,
    TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_TELNUM,
    
    TAG_CUSDATA_SETTING_CONNECTIONS,
    TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENT,
    TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENTPROFILE,

    TAG_CUSDATA_SETTING_CALENDAR,
    TAG_CUSDATA_SETTING_CALENDAR_STARTDAY,
    
    TAG_CUSDATA_MAX
}XML_CSC_TAG_NAME_TPYE_E;

typedef struct
{
    char *tag_name;   
    XML_CSC_TAG_NAME_TPYE_E type;
}XML_TAG_NAME_T;

LOCAL const XML_TAG_NAME_T s_xml_tag_name[] =
{
    {"CustomerData",                   TAG_CUSDATA},

    {"GeneralInfo",           TAG_CUSDATA_GENERALINFO},
    {"CSCEdition",            TAG_CUSDATA_GENERALINFO_CSCEDITION},
    {"Country",               TAG_CUSDATA_GENERALINFO_COUNTRY},
    {"SalesCode",             TAG_CUSDATA_GENERALINFO_SALESCODE},
    {"NbNetworkInfo",         TAG_CUSDATA_GENERALINFO_NBNETWORKINFO},
    {"NetworkInfo",           TAG_CUSDATA_GENERALINFO_NETWORKINFO},    
    {"MCCMNC",                TAG_CUSDATA_GENERALINFO_NETWORKINFO_MCCMNC},    
    {"NetworkName",           TAG_CUSDATA_GENERALINFO_NETWORKINFO_NETWORKNAME},
    
    {"Settings",              TAG_CUSDATA_SETTING},
    {"Main",                  TAG_CUSDATA_SETTING_MAIN},
    {"Security",              TAG_CUSDATA_SETTING_MAIN_SECURITY},

    {"Sound",                 TAG_CUSDATA_SETTING_MAIN_SOUND},
    {"Profileset",                 TAG_CUSDATA_SETTING_MAIN_SOUND_PROFILESET},
    {"RingTone",                 TAG_CUSDATA_SETTING_MAIN_SOUND_RINGTONE},
    {"RingToneAlertType",                 TAG_CUSDATA_SETTING_MAIN_SOUND_RINGTONEALERTTYPE},
    {"RingVolume",                 TAG_CUSDATA_SETTING_MAIN_SOUND_RINGVOLUME},
    {"KeyTone",                 TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONE},
    {"KeyToneAlertType",                 TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONEALERTTYPE},
    {"KeyVolume",                 TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONE_KEYVOLUME},
    {"MsgTone",                 TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONE},
    {"MsgToneAlertType",                 TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEALERTTYPE},
    {"MsgToneVolume",                 TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEVOLUME},
    {"TouchTone",                 TAG_CUSDATA_SETTING_MAIN_SOUND_TOUCHTONE},

    {"Call",                 TAG_CUSDATA_SETTING_MAIN_CALL},
    {"ExtraSound",                 TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND},
    {"ConnectTone",                 TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CONNECTIONTONE},
    {"CallEndTone",                 TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CALLENDTONE},
    {"MinuteMind",                 TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_MINUTEMIND},
    {"AlertOnCall",                 TAG_CUSDATA_SETTING_MAIN_CALL_ALERTONCALL},
    {"AutoRedial",                 TAG_CUSDATA_SETTING_MAIN_CALL_AUTOREDIAL},
    {"AnykeyAnswer",                 TAG_CUSDATA_SETTING_MAIN_CALL_ANYKEYANSWER},

    {"Phone",                 TAG_CUSDATA_SETTING_MAIN_PHONE},
    {"GreetMsg",              TAG_CUSDATA_SETTING_MAIN_PHONE_GREETMSG},
    {"DefLanguage",           TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGE},
    {"DefLanguageNoSIM",      TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGENOSIM},	
    {"ImEnabling",            TAG_CUSDATA_SETTING_MAIN_PHONE_IMENABLING},
    {"ImLanguage",            TAG_CUSDATA_SETTING_MAIN_PHONE_IMLANGUAGE},
    {"DateTimeFormat",        TAG_CUSDATA_SETTING_MAIN_PHONE_DATATIMEFORMAT},
    {"DateFormat",            TAG_CUSDATA_SETTING_MAIN_PHONE_DATEFORMAT},
    {"TimeFormat",            TAG_CUSDATA_SETTING_MAIN_PHONE_TIMEFORMAT},

    {"Display",                 TAG_CUSDATA_SETTING_MAIN_DISPLAY},
    {"BackLightTime",                 TAG_CUSDATA_SETTING_MAIN_DISPLAY_BACKLIGHTTIME},
    {"KeyLightTime",                 TAG_CUSDATA_SETTING_MAIN_DISPLAY_KEYLIGHTTIME},
    
    {"Network",               TAG_CUSDATA_SETTING_MAIN_NETWORK},
    {"NbSOSNumber",           TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBER},
    {"SOSNumber",             TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBER},
    {"NbSOSNumberNoSIM",      TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBERNOSIM},
    {"SOSNumberNoSIM",        TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBERNOSIM},
    
    {"Shortcut",              TAG_CUSDATA_SETTING_MAIN_SHORTCUT},
    {"UpKey",                 TAG_CUSDATA_SETTING_MAIN_SHORTCUT_UPKEY},
    {"DownKey",               TAG_CUSDATA_SETTING_MAIN_SHORTCUT_DWONKEY},
    {"LeftKey",               TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTKEY},
    {"RightKey",              TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTKEY},
    {"CenterKey",             TAG_CUSDATA_SETTING_MAIN_SHORTCUT_CENTERKEY},
    {"LeftSoftkey",           TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTSOFTKEY},
    {"RightSoftKey",          TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTSOFTKEY},
    {"ShortcutItem",          TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM},
    
    {"Messages",              TAG_CUSDATA_SETTING_MESSAGES},
    {"SMS",                   TAG_CUSDATA_SETTING_MESSAGES_SMS},
    {"StatusReport",                  TAG_CUSDATA_SETTING_MESSAGES_STAUSREPORT},
    {"SmsSavePosition",                  TAG_CUSDATA_SETTING_MESSAGES_SAVEPOSITION},
    {"CellBroadcast",         TAG_CUSDATA_SETTING_MESSAGES_CELLBROADCAST},
    {"MMS",                   TAG_CUSDATA_SETTING_MESSAGES_MMS},
    {"MMSLimit",                  TAG_CUSDATA_SETTING_MESSAGES_MMSLIMIT},
    {"NbVoicemail",           TAG_CUSDATA_SETTING_MESSAGES_NBVOICEMAIL},
    {"Voicemail",             TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL},
    {"VoicemailName",         TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_VOICEMAILNAME},
    {"TelNum",                TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_TELNUM},
    
    {"Connections",           TAG_CUSDATA_SETTING_CONNECTIONS},
    {"UserAgent",           TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENT},
    {"UserAgentProfile",           TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENTPROFILE},

    {"Calendar",              TAG_CUSDATA_SETTING_CALENDAR},
    {"StartDay",              TAG_CUSDATA_SETTING_CALENDAR_STARTDAY},
    
    {"unknow",                TAG_CUSDATA_MAX}
};

typedef struct
{
    const char *lang_name;
    MMISET_LANGUAGE_TYPE_E lang_type;
} MMIRES_LANG_T;

const MMIRES_LANG_T s_lang_name[] =
{
#ifdef MMI_SIM_LANGUAGE_SUPPORT
    {"Automatic", MMISET_LANGUAGE_AUTO},
#endif
    {"English",MMISET_LANGUAGE_ENGLISH},
    {"SIMpleChinese",MMISET_LANGUAGE_SIMP_CHINESE},
    {"TraditionalChinese",MMISET_LANGUAGE_TRAD_CHINESE},
    {"Arabic",MMISET_LANGUAGE_ARABIC},
    {"French",MMISET_LANGUAGE_FRENCH},
    {"Hindi",MMISET_LANGUAGE_HINDI},
    {"Hungarian",MMISET_LANGUAGE_HUNGARIAN},
    {"Indonesian",MMISET_LANGUAGE_INDONESIAN},
    {"Malay",MMISET_LANGUAGE_MALAY},
    {"Portuguese",MMISET_LANGUAGE_PORTUGUESE},
    {"Russian",MMISET_LANGUAGE_RUSSIAN},
    {"Spanish",MMISET_LANGUAGE_SPANISH},
    {"Tagalog",MMISET_LANGUAGE_TAGALOG},
    {"Thai",MMISET_LANGUAGE_THAI},
    {"Vietnamese",MMISET_LANGUAGE_VIETNAMESE},
    {"Urdu",MMISET_LANGUAGE_URDU},
    {"Italian",MMISET_LANGUAGE_ITALIAN},
    {"Farsi",MMISET_LANGUAGE_PERSIAN},
    {"Turkish",MMISET_LANGUAGE_TURKISH},
    {"German",MMISET_LANGUAGE_GERMAN},
    {"Greece",MMISET_LANGUAGE_GREEK},
    {"Hebrew",MMISET_LANGUAGE_HEBREW},
    {"Bengali",MMISET_LANGUAGE_BENGALI},
    {"Czech",MMISET_LANGUAGE_CZECH},
    {"Slovenian",MMISET_LANGUAGE_SLOVENIAN},
    {"Romanian",MMISET_LANGUAGE_ROMANIAN},
    {"Telugu",MMISET_LANGUAGE_TELUGU},
    {"Marathi",MMISET_LANGUAGE_MARATHI},
    {"Tamil",MMISET_LANGUAGE_TAMIL},
    {"Gujarati",MMISET_LANGUAGE_GUJARATI},
    {"Kannada",MMISET_LANGUAGE_KANNADA},
    {"Malayalam",MMISET_LANGUAGE_MALAYALAM},
    {"Oriya",MMISET_LANGUAGE_ORIYA},
    {"Punjabi",MMISET_LANGUAGE_PUNJABI},
    {"Afrikaans",MMISET_LANGUAGE_AFRIKAANS},
    {"Albanian",MMISET_LANGUAGE_ALBANIAN},
    {"Armenian",MMISET_LANGUAGE_ARMENIAN},
    {"Azerbaijani",MMISET_LANGUAGE_AZERBAIJANI},
    {"Basque",MMISET_LANGUAGE_BASQUE},
    {"Bulgarian",MMISET_LANGUAGE_BULGARIAN},
    {"Catalan",MMISET_LANGUAGE_CATALAN},
    {"Croatian",MMISET_LANGUAGE_CROATIAN},
    {"Danish",MMISET_LANGUAGE_DANISH},
    {"Dutch",MMISET_LANGUAGE_DUTCH},
    {"Estonian",MMISET_LANGUAGE_ESTONIAN},
    {"Filipino",MMISET_LANGUAGE_FILIPINO},
    {"Finnish",MMISET_LANGUAGE_FINNISH},
    {"Galician",MMISET_LANGUAGE_GALICIAN},
    {"Georgian",MMISET_LANGUAGE_GEORGIAN},
    {"Hausa",MMISET_LANGUAGE_HAUSA},
    {"Icelandic",MMISET_LANGUAGE_ICELANDIC},
    {"Igbo",MMISET_LANGUAGE_IGBO},
    {"Irish",MMISET_LANGUAGE_IRISH},
    {"Kazakh",MMISET_LANGUAGE_KAZAKH},
    {"Latvian",MMISET_LANGUAGE_LATVIAN},
    {"Lithuanian",MMISET_LANGUAGE_LITHUANIAN},
    {"Macedonian",MMISET_LANGUAGE_MACEDONIAN},
    {"Moldovan",MMISET_LANGUAGE_MOLDOVAN},
    {"Norwegian",MMISET_LANGUAGE_NORWEGIAN},
    {"Polish",MMISET_LANGUAGE_POLISH},
    {"Serbian",MMISET_LANGUAGE_SERBIAN},
    {"Sesotho",MMISET_LANGUAGE_SESOTHO},
    {"Slovak",MMISET_LANGUAGE_SLOVAK},
    {"Swedish",MMISET_LANGUAGE_SWEDISH},
    {"Ukrainian",MMISET_LANGUAGE_UKRAINIAN},
    {"Yoruba",MMISET_LANGUAGE_YORUBA},
    {"Xhosa",MMISET_LANGUAGE_XHOSA},
    {"Zulu",MMISET_LANGUAGE_ZULU},
    {"Assamese",MMISET_LANGUAGE_ASSAMESE},
    {"Swahili",MMISET_LANGUAGE_SWAHILI},
    {"Myanmar",MMISET_LANGUAGE_MYANMAR},
    {"Amharic",MMISET_LANGUAGE_AMHARIC},
    {"khmer",MMISET_LANGUAGE_KHMER},
    {"Lao",MMISET_LANGUAGE_LAO}
    //{"Hinglish",MMISET_LANGUAGE_HINGLISH},
    //{"Uyghur",MMISET_LANGUAGE_UYGHUR},
};

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL XML_STACK_T* s_xml_stack = PNULL;                       //stack
LOCAL BOOLEAN s_is_parser_parm_start = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         CallocXMLMem
* Function:     XML模块封装SCI_ALLOCA函数和MEMSET函数
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void * CallocXMLMem(uint32 size);

/* ----------------------------------------------------------------------------
* Name:         FreeXMLMem
* Function:     XML封装的内存释放函数
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void FreeXMLMem(void *ptr);

/* ----------------------------------------------------------------------------
* Name:         MallocAndCopyXMLMem
* Function:     Malloc And Copy XML Memory
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/  
LOCAL BOOLEAN MallocAndCopyXMLMem(void **dst, 
                                  const wchar *src
                                  );

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeChild
* Function:     Add child 
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN AddXMLTreeChild(XML_TAG_ITEM_T *parent_ptr, 
                              XML_TAG_ITEM_T *child_ptr
                              );

/* ----------------------------------------------------------------------------
* Name:         AddXMLTreeAtrr
* Function:     Add atrribution
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/
LOCAL BOOLEAN AddXMLTreeAtrr(XML_TAG_ITEM_T * item_ptr, 
                             const wchar *att_name_ptr, 
                             const wchar *att_value_ptr
                             );

/* ----------------------------------------------------------------------------
* Name:         ParserRootDestroy
* Function:     parsered_xml free
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParserRootDestroy(XML_TAG_ITEM_T * item_ptr);

/* ----------------------------------------------------------------------------
* Name:         XMLStackRelloc
* Function:     Alloc for stack
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackRelloc(XML_STACK_T* stack_ptr);

/* ----------------------------------------------------------------------------
* Name:         XMLStackCreate
* Function:     Create stack
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL XML_STACK_T* XMLStackCreate(uint32 size, uint32 allow_grow);

/* ----------------------------------------------------------------------------
* Name:         XMLStackPush
* Function:     Stack Push
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackPush(XML_STACK_T* stack_ptr, 
                           XML_STACK_OBJECT sz_elem
                           );

/* ----------------------------------------------------------------------------
* Name:         XMLStackIsEmpty
* Function:     Stack Is Empty
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 XMLStackIsEmpty(XML_STACK_T* stack_ptr);

/* ----------------------------------------------------------------------------
* Name:         XMLStackPop
* Function:     Stack Pop
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackPop(XML_STACK_T* stack_ptr, 
                          XML_STACK_OBJECT *object_ptr
                          );

/* ----------------------------------------------------------------------------
 * Name:         XMLStackTop
 * Function:     Stack Pop
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackTop(XML_STACK_T* stack_ptr, XML_STACK_OBJECT *object_ptr);

/* ----------------------------------------------------------------------------
* Name:         XMLStackDestroy
* Function:     Stack Destroy
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void XMLStackDestroy(XML_STACK_T**stack_pptr);

/* ----------------------------------------------------------------------------
* Name:         StartParseXMLElement
* Function:     Start parse XML Element
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void StartParseXMLElement(void *user_data_ptr, 
                                const CFLWCHAR *element_ptr, 
                                const CFLWCHAR **atts_pptr
                                );

/* ----------------------------------------------------------------------------
* Name:         EndParseXMLElement
* Function:     End Parse XML Element
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void EndParseXMLElement(void *user_data_ptr, 
                              const CFLWCHAR *element
                              );

/* ----------------------------------------------------------------------------
* Name:         ParseXMLCharacters
* Function:     Parse XML Characters
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void ParseXMLCharacters(void *user_data_ptr, 
                              const CFLWCHAR *data_ptr
                              );

/* ----------------------------------------------------------------------------
* Name:         SettingValueDestroy
* Function:     Setting Value Destroy
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingValueDestroy(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
* Name:         GetTagItemByTagName
* Function:     Get TagItem By TagName
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL XML_TAG_ITEM_T* GetTagItemByTagName(uint8* tag_name_ptr, XML_TAG_ITEM_T *tag_item_ptr);

/* ----------------------------------------------------------------------------
* Name:         GetTagItemNameByTagType
* Function:     Get TagItem Name By TagType
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetTagItemNameByTagType(XML_CSC_TAG_NAME_TPYE_E tag_name_type);

/* ----------------------------------------------------------------------------
* Name:         GetTagItemValueByTagName
* Function:     
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetTagItemValueByTagName(uint8* tag_name_ptr, XML_TAG_ITEM_T *tag_item_ptr);

/* ----------------------------------------------------------------------------
* Name:         XMLParseXMLFile
* Function:     Parse Customer Specific Customization XML File
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLParseXMLFile(MMI_DATA_ID_T data_id, MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetXMLSettings
 * Function:     Parse XML Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetXMLSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetSecuritySettings
 * Function:     Parse Security Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetSecuritySettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetNetworkSettings
 * Function:     Parse Network Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetNetworkSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetPhoneSettings
 * Function:     Parse Phone Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetPhoneSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetSoundSettings
 * Function:     Parse Sound Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetSoundSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetShortcutSettings
 * Function:     Parse ShortCut Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetShortcutSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetCallSettings
 * Function:     Parse Call Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetCallSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetDisplaySettings
 * Function:     Parse Display Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetDisplaySettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetMessageSettings
 * Function:     Parse Message Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetMessageSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
 * Name:         ParseSetConnectionSettings
 * Function:     Parse Connection Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetConnectionSettings(MMICUS_CSC_SETTING_T *setting_value_ptr);

/* ----------------------------------------------------------------------------
* Name:         GetXMLSettingValue
* Function:     Get XML Setting Vaule
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN GetXMLSettingValue(XML_TAG_ITEM_T *parsered_xml_ptr, MMICUS_CSC_SETTING_T * setting_ptr);

/* ----------------------------------------------------------------------------
* Name:         SetDefaultXMLCSCEdition
* Function:     Set the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void SetDefaultXMLCSCEdition(uint8* csc_edition, uint8 len);

/* -------------------------------------------------------------------------
* Name:         SetDefaultSalesCode
* Function:     Set the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void SetDefaultSalesCode(uint8* sales_code_ptr, uint8 len);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         CallocXMLMem
* Function:     
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void * CallocXMLMem(uint32 size)  //malloc memory for XML
{
    void *ptr = PNULL;

    if (0 == size)
    {
        return PNULL;
    }

    ptr = SCI_ALLOCAZ(size);

    return ptr;
}//end CallocXMLMem

/* ----------------------------------------------------------------------------
 * Name:         FreeXMLMem
 * Function:     
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void FreeXMLMem(void *ptr)  //free memory for XML
{
    if (ptr != PNULL)
    {
        SCI_FREE(ptr); 
    }
}//end FreeXMLMem

/* ----------------------------------------------------------------------------
 * Name:         MallocAndCopyXMLMem
* Function:     Malloc And Copy XML Memory
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/  
LOCAL BOOLEAN MallocAndCopyXMLMem(void **dst, const wchar *src) //malloc and copy attr
{
    int32 len = 0;
    uint8 *temp_src_ptr = PNULL;
    
    if (PNULL == dst || PNULL == src)
    {
        return FALSE;    
    }
    
    if (PNULL != *dst)
    {
        FreeXMLMem(*dst);
        *dst = PNULL;
    }

    len = MMIAPICOM_Wstrlen(src);
    temp_src_ptr = CallocXMLMem(len+1);

    if (PNULL == temp_src_ptr)
    {
        return FALSE;
    }

    MMI_WSTRNTOSTR(temp_src_ptr, len, src, len, len);
    
    *dst = CallocXMLMem(len+1);

    if (PNULL != *dst) 
    {
        SCI_STRCPY(*dst, (char *)temp_src_ptr);
        FreeXMLMem(temp_src_ptr);
        return TRUE;
    }

    FreeXMLMem(temp_src_ptr);
    return FALSE;
}//end MallocAndCopyXMLMem

/* ----------------------------------------------------------------------------
 * Name:         AddXMLTreeChild
* Function:     Add child 
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN AddXMLTreeChild(XML_TAG_ITEM_T *parent_ptr, XML_TAG_ITEM_T *child_ptr)
{
    XML_TAG_ITEM_T *last_ptr = PNULL;
    XML_TAG_ITEM_T *temp_ptr = PNULL;   
    int32 counter = 0;
    
    if (parent_ptr == PNULL || child_ptr == PNULL)
    {
        return FALSE;
    }

    temp_ptr = parent_ptr->child;

    if (PNULL != temp_ptr)
    {
        while (PNULL != temp_ptr) 
        {
            last_ptr = temp_ptr;
            temp_ptr = temp_ptr->next;
            ++counter;                         //brother index
        }

        last_ptr->next = child_ptr;
    }
    else
    {
        parent_ptr->child = child_ptr;
    }

    child_ptr->index = counter;
    child_ptr->parent  = parent_ptr;
    
    return  TRUE;
}//end AddXMLTreeChild

/* ----------------------------------------------------------------------------
 * Name:         AddXMLTreeAtrr
 * Function:     Add atrribution
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/
LOCAL BOOLEAN AddXMLTreeAtrr(XML_TAG_ITEM_T * item_ptr, const wchar *att_name_ptr, const wchar *att_value_ptr)
{
    XML_ATTR_T *temp_pro_ptr = PNULL;
    XML_ATTR_T *big_brother_ptr = PNULL; 
    
    if (att_name_ptr == PNULL || att_value_ptr == PNULL)
    {
        return FALSE;
    }
    
    if (PNULL != item_ptr)
    {
        temp_pro_ptr = item_ptr->tag_property;

        if (PNULL == temp_pro_ptr)	
        {
            temp_pro_ptr = (XML_ATTR_T*)CallocXMLMem(sizeof(XML_ATTR_T));

            if (PNULL == temp_pro_ptr)
            {
                return FALSE;
            }

            if (!MallocAndCopyXMLMem(&temp_pro_ptr->att_name,att_name_ptr) || !MallocAndCopyXMLMem(&temp_pro_ptr->att_value,att_value_ptr))
            {
                FreeXMLMem(temp_pro_ptr);
                temp_pro_ptr = PNULL;
            }

            item_ptr->tag_property = temp_pro_ptr;

            return TRUE;
        }//end if (temp_pro_ptr==PNULL)
        else
        {           
            big_brother_ptr = temp_pro_ptr;

            while (PNULL != temp_pro_ptr)
            {
                if ( !MMIAPICOM_Wstrcmp((wchar *)temp_pro_ptr->att_name, att_name_ptr) )
                {
                    if (PNULL != att_value_ptr)	
                    {
                        FreeXMLMem(temp_pro_ptr->att_value);
                        temp_pro_ptr->att_value = PNULL;
                        MallocAndCopyXMLMem(&temp_pro_ptr->att_value, att_value_ptr);
                    }

                    return TRUE;
                }
                big_brother_ptr = temp_pro_ptr;
                temp_pro_ptr = temp_pro_ptr->next;  
            }

            temp_pro_ptr = (XML_ATTR_T *)CallocXMLMem(sizeof(XML_ATTR_T));

            if (PNULL == temp_pro_ptr)
            {
                return FALSE;
            }

            if (!MallocAndCopyXMLMem(&temp_pro_ptr->att_name, att_name_ptr) || !MallocAndCopyXMLMem(&temp_pro_ptr->att_value, att_value_ptr))
            {
                FreeXMLMem(temp_pro_ptr);
                temp_pro_ptr = PNULL;
            }

            if (PNULL != temp_pro_ptr)
            {
                temp_pro_ptr->next  = PNULL;
            }
            big_brother_ptr->next  = temp_pro_ptr;

            return TRUE;            
        }//end else 	
    }//end if (item_ptr)
    
    return FALSE;    
}//end XMLTreeAddAtrr

/* ----------------------------------------------------------------------------
 * Name:         ParserRootDestroy
 * Function:     parsered_xml free
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParserRootDestroy(XML_TAG_ITEM_T * item_ptr)
{
    XML_ATTR_T *attr_ptr = PNULL;

    if (item_ptr == PNULL)
    {
        return TRUE;
    }

    while (PNULL != item_ptr->tag_property)
    {
        attr_ptr = item_ptr->tag_property;

        item_ptr->tag_property = item_ptr->tag_property->next;
        FreeXMLMem(attr_ptr->att_name);
        attr_ptr->att_name = PNULL;
        FreeXMLMem(attr_ptr->att_value);
        attr_ptr->att_value =PNULL;
        FreeXMLMem(attr_ptr);
        attr_ptr = PNULL;        
    }

    if (PNULL != item_ptr->tag_name)
    {
        FreeXMLMem(item_ptr->tag_name);
        item_ptr->tag_name = PNULL;
    }
    
    if (PNULL != item_ptr->tag_add_content)
    {
        FreeXMLMem(item_ptr->tag_add_content);
        item_ptr->tag_add_content = PNULL;
    }   
    
    if (PNULL != item_ptr->child)
    {
        ParserRootDestroy(item_ptr->child);
    }
    
    if (PNULL != item_ptr->next)
    {
        ParserRootDestroy(item_ptr->next);
    }
    
    FreeXMLMem(item_ptr);
    item_ptr = PNULL;
    
    return TRUE;
}//end ParserRootDestroy

/* ----------------------------------------------------------------------------
 * Name:         XMLStackRelloc
 * Function:    
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackRelloc(XML_STACK_T* stack_ptr)
{
    XML_STACK_OBJECT * old_stack_ptr = PNULL;
    uint16  length = 0;
    
    old_stack_ptr = stack_ptr->stack;
    stack_ptr->size += XML_STACK_DEFAULT_GROW;
    stack_ptr->stack = (XML_STACK_OBJECT*)CallocXMLMem(stack_ptr->size*sizeof(XML_STACK_OBJECT));
    
    if (stack_ptr->stack == PNULL) 
    {
        stack_ptr->stack = old_stack_ptr;
        stack_ptr->size -= XML_STACK_DEFAULT_GROW;
        return FALSE;
    }

    length = stack_ptr->size*sizeof(XML_STACK_OBJECT);
    SCI_MEMSET(stack_ptr->stack,0,length);
    SCI_MEMCPY(stack_ptr->stack, old_stack_ptr, sizeof(XML_STACK_OBJECT)*stack_ptr->len);
    
    return TRUE;
}//end XMLStackRelloc

/* ----------------------------------------------------------------------------
 * Name:         XMLStackCreate
 * Function:     
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL XML_STACK_T* XMLStackCreate(uint32 size, uint32 allow_grow)
{
    XML_STACK_T* stack_ptr = (XML_STACK_T*)CallocXMLMem(sizeof(XML_STACK_T));

    if (stack_ptr != PNULL)
    {
        stack_ptr->size = size;
        stack_ptr->allow_grow = allow_grow;
        stack_ptr->stack = (XML_STACK_OBJECT*)CallocXMLMem(sizeof(XML_STACK_OBJECT) * size);
        stack_ptr->len = 0;

        if (stack_ptr->stack == PNULL) 
        {
            FreeXMLMem(&stack_ptr);
            stack_ptr = PNULL;
        }
    }
    
    return stack_ptr;
}//end XMLStackCreate

/* ----------------------------------------------------------------------------
 * Name:         XMLStackPush
* Function:     Stack Push
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackPush(XML_STACK_T* stack_ptr, XML_STACK_OBJECT sz_elem)
{
    if ( PNULL == stack_ptr) 
    {
        return FALSE;
    }

    if (stack_ptr->len >= stack_ptr->size) 
    {
        if (!stack_ptr->allow_grow || !XMLStackRelloc(stack_ptr)) 
        {
            return FALSE;
        }
    }
    
    *(stack_ptr->stack+stack_ptr->len) = sz_elem;
    ++stack_ptr->len;
    
    return TRUE;
}//end  XMLStackPush

/* ----------------------------------------------------------------------------
 * Name:         XMLStackIsEmpty
* Function:     Stack Is Empty
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8 XMLStackIsEmpty(XML_STACK_T* stack_ptr)
{
    if (PNULL == stack_ptr)
    {
        return TRUE;
    }

    return (0 == stack_ptr->len);
}//end XMLStackIsEmpty

/* ----------------------------------------------------------------------------
 * Name:         XMLStackPop
 * Function:     Stack Pop
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackPop(XML_STACK_T* stack_ptr, XML_STACK_OBJECT *object_ptr)
{
    if (!stack_ptr || XMLStackIsEmpty(stack_ptr) || PNULL == object_ptr) 
    {
        return FALSE;
    }

    --stack_ptr->len;
    *object_ptr = *(stack_ptr->stack+stack_ptr->len);
    
    return TRUE;
}//end XMLStackPop

/* ----------------------------------------------------------------------------
 * Name:         XMLStackTop
 * Function:     Stack Pop
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLStackTop(XML_STACK_T* stack_ptr, XML_STACK_OBJECT *object_ptr)
{
    if (!stack_ptr || XMLStackIsEmpty(stack_ptr) || object_ptr == PNULL)
    {
        return FALSE;
    }

    *object_ptr = *(stack_ptr->stack+stack_ptr->len - 1);

    return TRUE;
}//end XMLStackTop

/* ----------------------------------------------------------------------------
 * Name:        XMLStackDestroy
 * Function:    Stack Destroy
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void XMLStackDestroy(XML_STACK_T**stack_pptr)
{
    if (PNULL == stack_pptr || PNULL == (*stack_pptr))
    {
        return;
    }
    
    FreeXMLMem((*stack_pptr)->stack);
    (*stack_pptr)->stack = PNULL;

    FreeXMLMem(*stack_pptr);
    *stack_pptr = PNULL;
}//end XMLtackDestroy

/* ----------------------------------------------------------------------------
 * Name:         StartParseXMLElement
 * Function:     Start parse XML Element
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void StartParseXMLElement(void *user_data_ptr, const CFLWCHAR *element_ptr, const CFLWCHAR **atts_pptr)
{   
    XML_STACK_OBJECT top = 0;
    XML_TAG_ITEM_T *item_ptr = PNULL;
    XML_TAG_ITEM_T *user_data_tem_ptr = user_data_ptr;    
    uint8 attr_index = 0;    
    
    if (PNULL == user_data_ptr)
    {
        return;
    }
    
    item_ptr = (XML_TAG_ITEM_T*)CallocXMLMem(sizeof(XML_TAG_ITEM_T));
    if (item_ptr == PNULL)
    {
        return;
    }

    //set the tag name
    if (!MallocAndCopyXMLMem(&item_ptr->tag_name,(wchar*)element_ptr))
    {
        return;
    }

    if (PNULL != atts_pptr)
    {
        if (PNULL == atts_pptr[attr_index])
        {
            //AddXMLTreeAtrr(item_ptr, (uint8*)element_ptr, PNULL);
            s_is_parser_parm_start = TRUE;
        }

        while (PNULL != atts_pptr[attr_index])
        {
            //add the attr
            AddXMLTreeAtrr(item_ptr, (wchar*)(atts_pptr[attr_index]), (wchar*)(atts_pptr[attr_index + 1]));
            attr_index++;
            attr_index++;
        }        
    }
    //get the top item and add new item to it
    if (XMLStackTop(s_xml_stack,&top))     
    {
        AddXMLTreeChild((XML_TAG_ITEM_T*)top, item_ptr);
    }

    if (PNULL == user_data_tem_ptr->next)
    {
        user_data_tem_ptr->next = item_ptr;
    }

    //push new item into stack
    XMLStackPush(s_xml_stack, (XML_STACK_OBJECT)item_ptr);
}//end StartParseXMLElement

/* ----------------------------------------------------------------------------
 * Name:         EndParseXMLElement
* Function:     End Parse XML Element
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void EndParseXMLElement(void *user_data_ptr, const CFLWCHAR *element)
{
    XML_STACK_OBJECT top = 0;
    
    XMLStackPop(s_xml_stack, &top);    
}//end EndParseXMLElement

/* ----------------------------------------------------------------------------
 * Name:         ParseXMLCharacters
* Function:     Parse XML Characters
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void ParseXMLCharacters(void *user_data_ptr, 
                              const CFLWCHAR *data_ptr
                              )
{
    wchar * value_wstr_ptr = PNULL;
    wchar * name_wstr_ptr = PNULL;
    uint16 length = 0;
    XML_STACK_OBJECT top = 0;
    XML_TAG_ITEM_T *item_ptr = PNULL;
    
    if (!s_is_parser_parm_start)
    {
        return;
    }

    s_is_parser_parm_start = FALSE;

    if (0x20 == *(uint8*)data_ptr || 
        0x0A == *(uint8*)data_ptr || 
        0x0D == *(uint8*)data_ptr)
    {
        return;
    }

    length = MMIAPICOM_Wstrlen(data_ptr);

    if (0 == length)
    {
        return;
    }

    value_wstr_ptr = (wchar *)SCI_ALLOCAZ((length + 1) * sizeof(wchar));

    if (PNULL == value_wstr_ptr)
    {
        return;
    }

    MMI_WSTRNCPY(value_wstr_ptr, length, data_ptr, length, length);

    if (XMLStackTop(s_xml_stack,&top))
    {
        item_ptr = (XML_TAG_ITEM_T*)top;
        length = strlen((char *)item_ptr->tag_name);
        name_wstr_ptr = (wchar *)SCI_ALLOCAZ((length + 1) * sizeof(wchar));

        if (PNULL == name_wstr_ptr)
        {
            FreeXMLMem(value_wstr_ptr);

            return;
        }

        MMI_STRNTOWSTR(name_wstr_ptr, length, item_ptr->tag_name, length, length);
        AddXMLTreeAtrr(item_ptr, name_wstr_ptr, value_wstr_ptr);
    }
    
    FreeXMLMem(value_wstr_ptr); 
    FreeXMLMem(name_wstr_ptr);
}

/* ----------------------------------------------------------------------------
* Name:         SettingValueDestroy
* Function:     Setting Value Destroy
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN SettingValueDestroy(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_networkinfo_ptr = PNULL;
    MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T *temp_voicemail_ptr = PNULL;

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    while (PNULL != setting_value_ptr->general_setting.general_networkinfo)
    {
        temp_networkinfo_ptr = setting_value_ptr->general_setting.general_networkinfo;
        setting_value_ptr->general_setting.general_networkinfo = setting_value_ptr->general_setting.general_networkinfo->next;
        SCI_FREE(temp_networkinfo_ptr);
        temp_networkinfo_ptr = PNULL;
    }

    while (PNULL != setting_value_ptr->message_setting.voice_mail)
    {
        temp_voicemail_ptr = setting_value_ptr->message_setting.voice_mail;
        setting_value_ptr->message_setting.voice_mail = setting_value_ptr->message_setting.voice_mail->next;
        SCI_FREE(temp_voicemail_ptr);
        temp_voicemail_ptr = PNULL;
    }

    return TRUE;
}//end MMISettingDestroy

/* ----------------------------------------------------------------------------
* Name:         XMLParseXMLFile
* Function:     Parse Customer Specific Customization XML File
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN XMLParseXMLFile(MMI_DATA_ID_T data_id, MMICUS_CSC_SETTING_T *setting_value_ptr)
{
#ifdef XML_SUPPORT
    CFLXML_PARSER_T* parser_ptr = PNULL;
    XML_TAG_ITEM_T user_data = {0};
    int32 xml_result = 0;
    BOOLEAN is_xml_res_exist = FALSE;
    MMIRES_DATA_INFO_T xml_file_info = {0};
    XML_TAG_ITEM_T *parsered_xml_ptr = PNULL;        //parsed xml root

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }
    
    is_xml_res_exist = MMI_GetDataInfo(data_id, &xml_file_info);

    if (!is_xml_res_exist || xml_file_info.data_len < 3)
    {
        return FALSE;
    }

    //UTF8 File Format
    if (0xEF != xml_file_info.data_ptr[0]
        || 0xBB != xml_file_info.data_ptr[1]
        || 0xBF != xml_file_info.data_ptr[2])
    {
        return FALSE;
    }

    s_xml_stack = XMLStackCreate(XML_STACK_SIZE, XML_STACK_ALLOW_GROW);

    parser_ptr = CFLXML_CreateParser
        (
        StartParseXMLElement,
        EndParseXMLElement,
        ParseXMLCharacters,
        PNULL,
        &user_data
        );
    
    if (PNULL == parser_ptr)
    {
        return FALSE;
    }

    xml_result = CFLXML_Parse( parser_ptr, (const uint8 *)xml_file_info.data_ptr, xml_file_info.data_len, TRUE );
    
    CFLXML_FreeParser( parser_ptr );
    parser_ptr = PNULL;

    XMLStackDestroy(&s_xml_stack);

    if ( CFLXML_ERR_OK != xml_result )
    {
        return FALSE;
    }

    parsered_xml_ptr = user_data.next;

    GetXMLSettingValue(parsered_xml_ptr, setting_value_ptr);

    ParserRootDestroy(parsered_xml_ptr);

    return TRUE;
#else
    return FALSE;
#endif
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetSecuritySettings
 * Function:     Parse Security Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetSecuritySettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    //to do
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetNetworkSettings
 * Function:     Parse Network Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetNetworkSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetPhoneSettings
 * Function:     Parse Phone Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetPhoneSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    //set time display type
    if (0 != setting_value_ptr->main_setting.phone_setting.time_format[0] && !stricmp((char *)setting_value_ptr->main_setting.phone_setting.time_format, (char *)XML_SETTING_CSC_12H_TAG))
    {
        MMIAPISET_SetTimeDisplayType(MMISET_TIME_12HOURS);
    }
    else if (0 != setting_value_ptr->main_setting.phone_setting.time_format[0] && !stricmp((char *)setting_value_ptr->main_setting.phone_setting.time_format, (char *)XML_SETTING_CSC_24H_TAG))
    {
        MMIAPISET_SetTimeDisplayType(MMISET_TIME_24HOURS);
    }

    //set date display type 
    if (0 != setting_value_ptr->main_setting.phone_setting.date_format[0] && !stricmp((char *)setting_value_ptr->main_setting.phone_setting.date_format, (char *)XML_SETTING_CSC_YYYYMMDD_TAG))
    {
        MMIAPISET_SetDateDisplayType(MMISET_DATE_YMD);
    }
    else if (0 != setting_value_ptr->main_setting.phone_setting.date_format[0] && !stricmp((char *)setting_value_ptr->main_setting.phone_setting.date_format, (char *)XML_SETTING_CSC_MMDDYYYY_TAG))
    {
        MMIAPISET_SetDateDisplayType(MMISET_DATE_MDY);
    }
    else if (0 != setting_value_ptr->main_setting.phone_setting.date_format[0] && !stricmp((char *)setting_value_ptr->main_setting.phone_setting.date_format, (char *)XML_SETTING_CSC_DDMMYYYY_TAG))
    {
        MMIAPISET_SetDateDisplayType(MMISET_DATE_DMY);
    }

    //default language
    if (0 != setting_value_ptr->main_setting.phone_setting.default_language[0])
    {
        MMISET_LANGUAGE_TYPE_E language_type = MMICUS_GetMatchLang((const char *)setting_value_ptr->main_setting.phone_setting.default_language);

        if (language_type < MMISET_MAX_LANGUAGE)
        {
            MMIAPISET_SetLanguageType(language_type);
        }
    }

    //default language no sim
    if (0 != setting_value_ptr->main_setting.phone_setting.default_language_nosim[0])
    {
        //MMISET_LANGUAGE_TYPE_E language_type = MMICUS_GetMatchLang((const char *)setting_value_ptr->main_setting.phone_setting.default_language_nosim);
    }

    //IM enable
    if (setting_value_ptr->main_setting.phone_setting.im_enabling
        && 0 != setting_value_ptr->main_setting.phone_setting.im_language[0])
    {
        MMISET_LANGUAGE_TYPE_E language_type = MMICUS_GetMatchLang((const char *)setting_value_ptr->main_setting.phone_setting.im_language);

        if (language_type < MMISET_MAX_LANGUAGE
#ifdef MMI_SIM_LANGUAGE_SUPPORT
            && MMISET_LANGUAGE_AUTO != language_type
#endif
            )
        {
            //取得GUI语言序
            GUIIM_LANGUAGE_T lang = MMIAPIIM_GetGUILangBySetLangIdx(language_type);

            //把当前的输入语言写NV
            MMIAPIIM_SetLanguage(lang);
        }
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetSoundSettings
 * Function:     Parse Sound Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetSoundSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    uint32 i = 0;
    MMIENVSET_SETTING_T mode_setting = {0};

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    //Profile Set
    if (0 != setting_value_ptr->main_setting.sound_setting.profile_set[0])
    {
        BOOLEAN is_need_set = TRUE;
        MMIENVSET_ENV_MODE_E mode_id = MMIENVSET_STANDARD_MODE;

        if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.profile_set, (char *)XML_SETTING_CSC_STANDARD_TAG))
        {
            mode_id = MMIENVSET_STANDARD_MODE;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.profile_set, (char *)XML_SETTING_CSC_SLIENT_TAG))
        {
            mode_id = MMIENVSET_SILENT_MODE;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.profile_set, (char *)XML_SETTING_CSC_MEETING_TAG))
        {
            mode_id = MMIENVSET_MEETING_MODE;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.profile_set, (char *)XML_SETTING_CSC_INSIDE_TAG))
        {
            mode_id = MMIENVSET_INSIDE_MODE;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.profile_set, (char *)XML_SETTING_CSC_NOISY_TAG))
        {
            mode_id = MMIENVSET_NOISY_MODE;
        }
        else
        {
            is_need_set = FALSE;
        }

        if (is_need_set)
        {
            MMIAPIENVSET_SetActiveModeForIdle(mode_id);
        }
    }

    MMIAPIENVSET_GetModeValue(MMIAPIENVSET_GetActiveModeId(), &mode_setting);

    //Call ring
    if (0 != setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype[0])
    {
        BOOLEAN is_need_set = TRUE;
        uint8 call_ring_type = MMISET_CALL_RING;

        if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype, (char *)XML_SETTING_CSC_MELODY_TAG))
        {
            call_ring_type = (uint8)MMISET_CALL_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype, (char *)XML_SETTING_CSC_VIBRATE_TAG))
        {
            call_ring_type = (uint8)MMISET_CALL_VIBRA;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype, (char *)XML_SETTING_CSC_VIBMELODY_TAG))
        {
            call_ring_type = (uint8)MMISET_CALL_VIBRA_AND_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype, (char *)XML_SETTING_CSC_VIBBEFOREMELODY_TAG))
        {
            call_ring_type = (uint8)MMISET_CALL_VIBRA_BEFORE_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.ring_tone_alerttype, (char *)XML_SETTING_CSC_SLIENT_TAG))
        {
            call_ring_type = (uint8)MMISET_CALL_SILENT;
        }
        else
        {
            is_need_set = FALSE;
        }

        if (is_need_set)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                mode_setting.call_ring_type[i] = call_ring_type;
            }
        }
    }
    
    if (setting_value_ptr->main_setting.sound_setting.is_ring_tone_volume)
    {
        mode_setting.call_ring_vol = setting_value_ptr->main_setting.sound_setting.ring_tone_volume;
    }

    //Msg ring
    if (0 != setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype[0])
    {
        BOOLEAN is_need_set = TRUE;
        uint8 msg_ring_type = MMISET_MSG_RING;

        if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype, (char *)XML_SETTING_CSC_MELODY_TAG))
        {
            msg_ring_type = MMISET_CALL_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype, (char *)XML_SETTING_CSC_VIBRATE_TAG))
        {
            msg_ring_type = MMISET_CALL_VIBRA;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype, (char *)XML_SETTING_CSC_VIBMELODY_TAG))
        {
            msg_ring_type = MMISET_CALL_VIBRA_AND_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype, (char *)XML_SETTING_CSC_VIBBEFOREMELODY_TAG))
        {
            msg_ring_type = MMISET_CALL_VIBRA_BEFORE_RING;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.msg_tone_alerttype, (char *)XML_SETTING_CSC_SLIENT_TAG))
        {
            msg_ring_type = MMISET_CALL_SILENT;
        }
        else
        {
            is_need_set = FALSE;
        }

        if (is_need_set)
        {
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                mode_setting.msg_ring_type[i] = msg_ring_type;
            }
        }
    }

    if (setting_value_ptr->main_setting.sound_setting.is_msg_tone_volume)
    {
        mode_setting.msg_ring_vol = setting_value_ptr->main_setting.sound_setting.msg_tone_volume;
    }

    //key tone
    if (0 != setting_value_ptr->main_setting.sound_setting.key_tone_alerttype[0])
    {
        if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.key_tone_alerttype, (char *)XML_SETTING_CSC_SLIENT_TAG))
        {
            mode_setting.key_ring_type = 0;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.key_tone_alerttype, (char *)XML_SETTING_CSC_RINGTONE1_TAG))
        {
            mode_setting.key_ring_type = 1;
        }
        else if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.key_tone_alerttype, (char *)XML_SETTING_CSC_RINGTONE2_TAG))
        {
            mode_setting.key_ring_type = 2;
        }
    }

    if (setting_value_ptr->main_setting.sound_setting.is_key_tone_volume)
    {
        mode_setting.key_ring_vol = setting_value_ptr->main_setting.sound_setting.key_tone_volume;
    }

    //touch tone
    if (0 != setting_value_ptr->main_setting.sound_setting.touch_tone_alerttype[0])
    {
        if (!stricmp((char *)setting_value_ptr->main_setting.sound_setting.touch_tone_alerttype, (char *)XML_SETTING_CSC_ON_TAG))
        {
            mode_setting.tkey_ring_type = 0;
        }
    }

    MMIAPIENVSET_SetModeValue(MMIAPIENVSET_GetActiveModeId(), &mode_setting);

    return TRUE;
}
/* ----------------------------------------------------------------------------
 * Name:         ParseSetShortcutSettings
 * Function:     Parse ShortCut Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetShortcutSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    //to do
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetCallSettings
 * Function:     Parse Call Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetCallSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    MMISET_ANYKEY_REJECT_SET_T anykey_setting = {0};

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    //minute mind
    if (setting_value_ptr->main_setting.call_setting.is_minute_mind)
    {
        if (0 == setting_value_ptr->main_setting.call_setting.minute_mind)
        {
            MMIAPISET_SetPermitMinhintRingIncall(FALSE, CC_CALL_REMINDER_TIME_OFF);
        }
        else
        {
            MMIAPISET_SetPermitMinhintRingIncall(TRUE, setting_value_ptr->main_setting.call_setting.minute_mind);
        }
    }

    //anykey answer
    if (setting_value_ptr->main_setting.call_setting.is_anykey_answer)
    {
        anykey_setting = MMIAPISET_GetAnykeyRejectMode();

        anykey_setting.is_anykey_answer = setting_value_ptr->main_setting.call_setting.anykey_answer;

        MMIAPISET_SetAnykeyRejectMode(&anykey_setting);
    }

    //Auto redial
    if (setting_value_ptr->main_setting.call_setting.is_auto_redial)
    {
        MMIAPISET_SetIsAutoRedial(setting_value_ptr->main_setting.call_setting.auto_redial);
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetDisplaySettings
 * Function:     Parse Display Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetDisplaySettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    //backlight
    if (setting_value_ptr->main_setting.display_setting.is_backlight_time
        && 0 < setting_value_ptr->main_setting.display_setting.backlight_time)
    {
        MMISET_BACKLIGHT_SET_T backlight_info = {0};
        
        backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
        backlight_info.backlight_time = setting_value_ptr->main_setting.display_setting.backlight_time * 1000;

        switch (backlight_info.backlight_time)
        {
        case 0:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
            break;

#if !defined(MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT)
        case MMISET_KEYLOCK_5_SEC:
            backlight_info.backlight_time = MMISET_TIME_5;
            break;
#endif
        case MMISET_KEYLOCK_15_SEC:
            backlight_info.backlight_time = MMISET_TIME_15;
            break;

        case MMISET_KEYLOCK_30_SEC:
            backlight_info.backlight_time = MMISET_TIME_30;
            break;

        case MMISET_KEYLOCK_1_MIN:
            backlight_info.backlight_time = MMISET_TIME_60;
            break;

        case MMISET_KEYLOCK_5_MIN:
            backlight_info.backlight_time = MMISET_TIME_300;
            break;

        default:
            backlight_info.backlight_time = MMISET_TIME_15;
            break;
        }

        MMIAPISET_SetBackLightInfo(backlight_info);
    }

    //keylight
    //todo

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetMessageSettings
 * Function:     Parse Message Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetMessageSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    uint8 *temp_value_ptr = PNULL;

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    //status report
    if (setting_value_ptr->message_setting.is_status_report)
    {
        MMIAPISMS_SetStatusReportFlag(setting_value_ptr->message_setting.status_report);
    }

#if defined(MMI_SMSCB_SUPPORT)
    //smscb status
    if (setting_value_ptr->message_setting.is_cell_broadcast)
    {
        MMIAPISMSCB_SwitchSmsCBStatus(setting_value_ptr->message_setting.cell_broadcast);
    }
#endif

    //Call ring
    if (0 != setting_value_ptr->message_setting.save_position[0])
    {
        BOOLEAN is_need_set = TRUE;
        MMISMS_SMS_SAVEPOS_TYPE_E save_position = MMISMS_SAVEPOS_PHONE;

        temp_value_ptr = setting_value_ptr->message_setting.save_position;

        if (!stricmp((char *)setting_value_ptr->message_setting.save_position, (char *)XML_SETTING_CSC_PHONE_TAG))
        {
            save_position = MMISMS_SAVEPOS_PHONE;
        }
        else if (!stricmp((char *)setting_value_ptr->message_setting.save_position, (char *)XML_SETTING_CSC_SIM_TAG))
        {
            save_position = MMISMS_SAVEPOS_SIM;
        }
        else
        {
            is_need_set = FALSE;
        }

        if (is_need_set)
        {
            MMIAPISMS_SetSavePos(save_position);
        }
    }

    //Voice mail
    //todo
    
    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetConnectionSettings
 * Function:     Parse Connection Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetConnectionSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    //to do
    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------
 * Name:         ParseSetXMLSettings
 * Function:     Parse XML Settings
 * Author:       
 * Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN ParseSetXMLSettings(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    // Set the security setting related information.
    ParseSetSecuritySettings(setting_value_ptr);

    // Set the Network setting(SOS) related information.
    ParseSetNetworkSettings(setting_value_ptr);
    
    // Set the Phone setting, Phone call and other phone general information.
    ParseSetPhoneSettings(setting_value_ptr);

    //Sound setting
    ParseSetSoundSettings(setting_value_ptr);

    // Set the Shortcut related information.
    ParseSetShortcutSettings(setting_value_ptr);

    // Set the Call related information.
    ParseSetCallSettings(setting_value_ptr);

    // Set the Display related information.
    ParseSetDisplaySettings(setting_value_ptr);

    // Set the Message related information.
    ParseSetMessageSettings(setting_value_ptr);

    return TRUE;
}

/* ----------------------------------------------------------------------------
* Name:         GetTagItemByTagName
* Function:     Get TagItem By TagName
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL XML_TAG_ITEM_T* GetTagItemByTagName(uint8* tag_name_ptr, XML_TAG_ITEM_T *tag_item_ptr)
{
    XML_TAG_ITEM_T *root_item_ptr = tag_item_ptr;
    XML_TAG_ITEM_T *temp_item_ptr = PNULL;
    
    if (PNULL == root_item_ptr || PNULL == tag_name_ptr)
    {
        return PNULL;
    }
    
    while (PNULL != root_item_ptr)
    {
        if (!root_item_ptr->get_flag && !stricmp((char *)tag_name_ptr, (char *)root_item_ptr->tag_name))
        {
            root_item_ptr->get_flag = TRUE;
            return root_item_ptr;
        }
        if (PNULL != root_item_ptr->child)
        {
            temp_item_ptr = GetTagItemByTagName(tag_name_ptr, root_item_ptr->child);
            if (PNULL != temp_item_ptr)
            {
                return temp_item_ptr;
            }
        }
        root_item_ptr = root_item_ptr->next;         
    }

    return temp_item_ptr;
}

/* ----------------------------------------------------------------------------
* Name:         GetTagItemNameByTagType
* Function:     Get TagItem Name By TagType
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetTagItemNameByTagType(XML_CSC_TAG_NAME_TPYE_E tag_name_type)
{
    uint16 item_num = 0;
    uint16 item_index = 0;
    
    item_num = sizeof(s_xml_tag_name)/sizeof(s_xml_tag_name[0]);

    for (item_index = 0; item_index < item_num; item_index++)
    {
        if (tag_name_type == s_xml_tag_name[item_index].type)
        {
            return ((uint8 *)s_xml_tag_name[item_index].tag_name);
        }
    }

    return PNULL;
}

/* ----------------------------------------------------------------------------
* Name:         GetTagItemValueByTagName
* Function:     
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL uint8* GetTagItemValueByTagName(uint8* tag_name_ptr, XML_TAG_ITEM_T *tag_item_ptr)
{
    XML_TAG_ITEM_T *root_item_ptr = tag_item_ptr;
    uint8 *value_ptr = PNULL;
    
    if (PNULL == root_item_ptr || PNULL == tag_name_ptr)
    {
        return PNULL;
    }

    while (PNULL != root_item_ptr)
    {
        if (!root_item_ptr->get_flag && !stricmp((char *)tag_name_ptr, (char *)root_item_ptr->tag_name) && PNULL != root_item_ptr->tag_property)
        {
            if (!stricmp((char *)tag_name_ptr, (char *)root_item_ptr->tag_property->att_name))
            {
                value_ptr = root_item_ptr->tag_property->att_value;
            }

            root_item_ptr->get_flag = TRUE;

            return value_ptr;
        }

        if (PNULL != root_item_ptr->child)
        {
            value_ptr = GetTagItemValueByTagName(tag_name_ptr, root_item_ptr->child);

            if (PNULL != value_ptr)
            {
                return value_ptr;
            }
        }

        root_item_ptr = root_item_ptr->next;
    }
    return PNULL;
}

/* ----------------------------------------------------------------------------
* Name:         GetXMLSettingValue
* Function:     Get XML Setting Vaule
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL BOOLEAN GetXMLSettingValue(XML_TAG_ITEM_T *parsered_xml_ptr, MMICUS_CSC_SETTING_T * setting_ptr)
{
    XML_TAG_ITEM_T *paser_result_ptr = PNULL;
    XML_TAG_ITEM_T *cur_item_ptr = PNULL;
    XML_TAG_ITEM_T *temp_item_ptr = PNULL;
    uint8 *cur_item_name_ptr = PNULL;
    uint8 *item_value_ptr = PNULL;
    uint16 value_len = 0;

    if (PNULL == parsered_xml_ptr)
    {
        return FALSE;
    }

    paser_result_ptr = (XML_TAG_ITEM_T *)parsered_xml_ptr->child;
    
    //general info
    cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO);
    cur_item_ptr = GetTagItemByTagName(cur_item_name_ptr, paser_result_ptr);
    
    if (PNULL != cur_item_ptr && PNULL != cur_item_ptr->child)
    {
        //CSC Edition
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_CSCEDITION);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->general_setting.csc_edition, item_value_ptr, value_len);
        }

        //Country
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_COUNTRY);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->general_setting.country, item_value_ptr, value_len);
        }

        //Sales Code
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_SALESCODE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->general_setting.sales_code, item_value_ptr, value_len);
        }

        //number of Network Info
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_NBNETWORKINFO);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->general_setting.nb_network_info = atoi((char *)item_value_ptr);
        }

        //Network Info
        if (0 < setting_ptr->general_setting.nb_network_info)
        {
            MMICUS_CSC_GENERALINFO_NETWORKINFO_T *networkinfo_ptr = PNULL;
            MMICUS_CSC_GENERALINFO_NETWORKINFO_T *temp_networkinfo_ptr = PNULL;
            uint8 temp_mccmnc[XML_TAG_CONTENT_MAX_LEN] = {0};
            uint16 network_num = setting_ptr->general_setting.nb_network_info;
            
            while (0 < network_num--)
            {
                networkinfo_ptr = (MMICUS_CSC_GENERALINFO_NETWORKINFO_T *)SCI_ALLOCAZ(sizeof(MMICUS_CSC_GENERALINFO_NETWORKINFO_T));

                if (PNULL == networkinfo_ptr)
                {
                    break;
                }

                cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_NETWORKINFO);        
                temp_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);

                if (PNULL == temp_item_ptr)
                {
                    SCI_FREE(networkinfo_ptr);

                    break;
                }
                
                //mcc mnc
                cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_NETWORKINFO_MCCMNC);

                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, temp_item_ptr->child);

                if (PNULL != item_value_ptr)
                {
                    SCI_MEMCPY(temp_mccmnc, item_value_ptr, 3);// 3
                    networkinfo_ptr->mcc = atoi((char *)temp_mccmnc);
                    SCI_MEMSET(temp_mccmnc, 0, XML_TAG_CONTENT_MAX_LEN);
                    SCI_MEMCPY(temp_mccmnc, item_value_ptr + 3, 3);//max 3
                    networkinfo_ptr->mnc = atoi((char *)temp_mccmnc);
                }

                //name
                cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_GENERALINFO_NETWORKINFO_NETWORKNAME);
                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, temp_item_ptr->child);

                if (PNULL != item_value_ptr)
                {
                    value_len = strlen((char*)item_value_ptr);
                    value_len = MIN(value_len, XML_TAG_NAME_MAX_LEN);
                    SCI_MEMCPY(networkinfo_ptr->network_name, item_value_ptr, value_len);
                }
                
                temp_networkinfo_ptr = setting_ptr->general_setting.general_networkinfo;
                if (PNULL == temp_networkinfo_ptr)
                {
                    setting_ptr->general_setting.general_networkinfo = networkinfo_ptr;
                }
                else
                {
                    while (PNULL != temp_networkinfo_ptr->next)
                    {
                        temp_networkinfo_ptr = temp_networkinfo_ptr->next;
                    }
                    temp_networkinfo_ptr->next = networkinfo_ptr;
                }
            }            
        }
    } 
    
    //setting main
    cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN);
    cur_item_ptr = GetTagItemByTagName(cur_item_name_ptr, paser_result_ptr);
    
    if (PNULL != cur_item_ptr)
    {
        XML_TAG_ITEM_T *shortcut_item_ptr = PNULL;
        
        //main setting security
        //.....
        
        //main settting sound
        //Profile Set
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_PROFILESET);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.sound_setting.profile_set, item_value_ptr, value_len);
        }

        //Ringtone
        //Ringtone Alert Type
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_RINGTONEALERTTYPE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.sound_setting.ring_tone_alerttype, item_value_ptr, value_len);
        }

        //Ring Volume
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_RINGVOLUME);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.sound_setting.is_ring_tone_volume = TRUE;
            setting_ptr->main_setting.sound_setting.ring_tone_volume = atoi((char *)item_value_ptr);
        }

        //KeyTone
        //KeyTone Alert Type
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONEALERTTYPE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.sound_setting.key_tone_alerttype, item_value_ptr, value_len);
        }

        //key tone volume
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_KEYTONE_KEYVOLUME);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.sound_setting.is_key_tone_volume = TRUE;
            setting_ptr->main_setting.sound_setting.key_tone_volume = atoi((char *)item_value_ptr);
        }

        //Msg tone
        //message tone alert type 
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEALERTTYPE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.sound_setting.msg_tone_alerttype, item_value_ptr, value_len);
        }

        //message tone volume
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_MSGTONEVOLUME);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.sound_setting.is_msg_tone_volume = TRUE;
            setting_ptr->main_setting.sound_setting.msg_tone_volume = atoi((char *)item_value_ptr);
        }

        //Touchtone
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SOUND_TOUCHTONE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.sound_setting.touch_tone_alerttype, item_value_ptr, value_len);
        }

        //Call
        //Extra Sound
        //connection tone
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CONNECTIONTONE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_connection_tone = TRUE;
                setting_ptr->main_setting.call_setting.connection_tone = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->main_setting.call_setting.is_connection_tone = TRUE;
                setting_ptr->main_setting.call_setting.connection_tone = FALSE;
            }
        }

        //call end tone
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_CALLENDTONE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_call_end_tone = TRUE;
                setting_ptr->main_setting.call_setting.call_end_tone = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->main_setting.call_setting.is_call_end_tone = TRUE;
                setting_ptr->main_setting.call_setting.call_end_tone = FALSE;
            }
        }

        //minute mind tone
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_EXTRASOUND_MINUTEMIND);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.call_setting.is_minute_mind = TRUE;
            setting_ptr->main_setting.call_setting.minute_mind = (uint8)atoi((char *)item_value_ptr);
        }

        //Alert On Call
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_ALERTONCALL);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_alert_oncall = TRUE;
                setting_ptr->main_setting.call_setting.alert_oncall = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->main_setting.call_setting.is_alert_oncall = TRUE;
                setting_ptr->main_setting.call_setting.alert_oncall = FALSE;
            }
        }

        //Auto Redial
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_AUTOREDIAL);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_auto_redial = TRUE;
                setting_ptr->main_setting.call_setting.auto_redial = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->main_setting.call_setting.is_auto_redial = TRUE;
                setting_ptr->main_setting.call_setting.auto_redial = FALSE;
            }
        }

        //Anykey Answer
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_CALL_ANYKEYANSWER);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_anykey_answer = TRUE;
                setting_ptr->main_setting.call_setting.anykey_answer = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->main_setting.call_setting.is_anykey_answer = TRUE;
                setting_ptr->main_setting.call_setting.anykey_answer = FALSE;
            }
        }

        //Phone
        //phone greet message
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_GREETMSG);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.greet_msg, item_value_ptr, value_len);
        }

        //default language
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.default_language, item_value_ptr, value_len);
        }

        //default language no sim
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_DEFLANGUAGENOSIM);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.default_language_nosim, item_value_ptr, value_len);
        }

        //IM enable
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_IMENABLING);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ENABLE_TAG))
            {
                setting_ptr->main_setting.phone_setting.im_enabling = TRUE;
            }
        }

        //IM language 
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_IMLANGUAGE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.im_language, item_value_ptr, value_len);
        }

        //date format
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_DATEFORMAT);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.date_format, item_value_ptr, value_len);
        }

        //time format
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_PHONE_TIMEFORMAT);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->main_setting.phone_setting.time_format, item_value_ptr, value_len);
        }

        //Display
        //back light time
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_DISPLAY_BACKLIGHTTIME);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.display_setting.is_backlight_time = TRUE;
            setting_ptr->main_setting.display_setting.backlight_time = atoi((char *)item_value_ptr);
        }

        //key light time
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_DISPLAY_KEYLIGHTTIME);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.display_setting.is_keylight_time = TRUE;
            setting_ptr->main_setting.display_setting.keylight_time = atoi((char *)item_value_ptr);
        }
        
        //Network
        //number of sos num
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBER);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.network_setting.sos_num.nb_sos_num = atoi((char *)item_value_ptr);
        }

        //sos num
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBER);

        if (0 < setting_ptr->main_setting.network_setting.sos_num.nb_sos_num)
        {
            uint8  temp_sos_number[XML_TAG_MESSAGE_MAX_LEN + 1] = {0};
            char sime_ch = ',';
            uint16 sos_num = setting_ptr->main_setting.network_setting.sos_num.nb_sos_num;
            uint16 number_len = 0;
            
            while (0 < sos_num--)
            {                
                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

                if (PNULL == item_value_ptr)
                {
                    break;
                }
                
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_MESSAGE_MAX_LEN);
                SCI_MEMCPY(temp_sos_number + number_len, item_value_ptr, value_len);
                number_len = number_len + value_len;
                if (0 < sos_num)
                {
                    SCI_MEMCPY(temp_sos_number + number_len, &sime_ch, 1);
                    number_len++;
                }                
            }

            value_len = strlen((char *)temp_sos_number);
            SCI_MEMCPY(setting_ptr->main_setting.network_setting.sos_num.sos_number, temp_sos_number, value_len);
        }

        //number of sos num no sim
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_NETWORK_NBSOSBUMBERNOSIM);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->main_setting.network_setting.sos_num.nb_sos_num_nosim = atoi((char *)item_value_ptr);
        }

        //sos num no sim
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_NETWORK_SOSNUMBERNOSIM);

        if (0 < setting_ptr->main_setting.network_setting.sos_num.nb_sos_num_nosim)
        {
            uint8 temp_sos_number[XML_TAG_MESSAGE_MAX_LEN + 1] = {0};
            char sime_ch = ',';
            uint16 sos_num = setting_ptr->main_setting.network_setting.sos_num.nb_sos_num_nosim;
            uint16 number_len = 0;
            
            while (0 < sos_num--)
            {
                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

                if (PNULL == item_value_ptr)
                {
                    break;
                }
                
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_MESSAGE_MAX_LEN);
                SCI_MEMCPY(temp_sos_number + number_len, item_value_ptr, value_len);
                number_len = number_len + value_len;
                if (0 < sos_num)
                {
                    SCI_MEMCPY(temp_sos_number + number_len, &sime_ch, 1);
                    number_len++;
                }                
            }

            value_len = strlen((char *)temp_sos_number);
            SCI_MEMCPY(setting_ptr->main_setting.network_setting.sos_num.sos_number_nosim, temp_sos_number, value_len);
        }
        
        //shortcut
        //up key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_UPKEY);        
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.up_key, item_value_ptr, value_len);
            }
        }

        //down key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_DWONKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.down_key, item_value_ptr, value_len);
            }
        }

        //left key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.left_key, item_value_ptr, value_len);
            }
        }

        //right key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.right_key, item_value_ptr, value_len);
            }
        }

        //center key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_CENTERKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.center_key, item_value_ptr, value_len);
            }
        }

        //left soft key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_LEFTSOFTKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.leftsoft_key, item_value_ptr, value_len);
            }
        }

        //right soft key
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_RIGHTSOFTKEY);
        shortcut_item_ptr = GetTagItemByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != shortcut_item_ptr && PNULL != shortcut_item_ptr->child)
        {
            cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MAIN_SHORTCUT_SHORTCUTITEM);
            item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, shortcut_item_ptr->child);
            if (PNULL != item_value_ptr)
            {
                value_len = strlen((char*)item_value_ptr);
                value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                SCI_MEMCPY(setting_ptr->main_setting.shortcut_setting.rightsoft_key, item_value_ptr, value_len);
            }
        }
    }

    //Messages
    cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES);
    cur_item_ptr = GetTagItemByTagName(cur_item_name_ptr, paser_result_ptr);

    if (PNULL != cur_item_ptr && PNULL != cur_item_ptr->child)
    {
        //Status report
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_STAUSREPORT);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->message_setting.is_status_report = TRUE;
                setting_ptr->message_setting.status_report = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->message_setting.is_status_report = TRUE;
                setting_ptr->message_setting.status_report = FALSE;
            }
        }

        //Save Position
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_SAVEPOSITION);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->message_setting.save_position, item_value_ptr, value_len);
        }

        //message sms broadcast
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_CELLBROADCAST);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_ON_TAG))
            {
                setting_ptr->message_setting.is_cell_broadcast = TRUE;
                setting_ptr->message_setting.cell_broadcast = TRUE;
            }
            else if (!stricmp((char *)item_value_ptr, (char *)XML_SETTING_CSC_OFF_TAG))
            {
                setting_ptr->message_setting.is_cell_broadcast = TRUE;
                setting_ptr->message_setting.cell_broadcast = FALSE;
            }
        }

        //Voice mail
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_NBVOICEMAIL);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            setting_ptr->message_setting.nb_voicemail = atoi((char *)item_value_ptr);
        }
        
        if (0 < setting_ptr->message_setting.nb_voicemail)
        {
            uint16 voicemail_num = setting_ptr->message_setting.nb_voicemail;
            MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T *new_voicemail_ptr = PNULL;
            MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T *temp_voicemail_ptr = PNULL;
            
            while (0 < voicemail_num--)
            {
                new_voicemail_ptr = (MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T *)SCI_ALLOCAZ(sizeof(MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T));
                if (PNULL == new_voicemail_ptr)
                {
                    break;
                }
                
                //voice mail name
                cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_VOICEMAILNAME);
                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
                if (PNULL != item_value_ptr)
                {
                    value_len = strlen((char*)item_value_ptr);
                    value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                    SCI_MEMCPY(new_voicemail_ptr->voicemail_name, item_value_ptr, value_len);
                }

                //voice mail number
                cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_VOICEMAIL_TELNUM);
                item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
                if (PNULL != item_value_ptr)
                {
                    value_len = strlen((char*)item_value_ptr);
                    value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
                    SCI_MEMCPY(new_voicemail_ptr->voicemail_telnum, item_value_ptr, value_len);
                }

                temp_voicemail_ptr = setting_ptr->message_setting.voice_mail;
                if (PNULL == temp_voicemail_ptr)
                {
                    setting_ptr->message_setting.voice_mail = new_voicemail_ptr;
                }
                else
                {
                    while (PNULL != temp_voicemail_ptr->next)
                    {
                        temp_voicemail_ptr = temp_voicemail_ptr->next;
                    }
                    temp_voicemail_ptr->next = new_voicemail_ptr;
                }
            }        
        }

        //MMS Limit
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_MESSAGES_MMSLIMIT);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);
        if (PNULL != item_value_ptr)
        {
            setting_ptr->message_setting.is_mms_limit = TRUE;
            setting_ptr->message_setting.mms_limit = atoi((char *)item_value_ptr);
        }
    }

    //Conections
    cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_CONNECTIONS);
    cur_item_ptr = GetTagItemByTagName(cur_item_name_ptr, paser_result_ptr);

    if (PNULL != cur_item_ptr && PNULL != cur_item_ptr->child)
    {
        //User Agent
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENT);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_UA_MAX_LEN);
            SCI_MEMCPY(setting_ptr->connection_setting.user_agent, item_value_ptr, value_len);
        }

        //User Agent Profile
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_CONNECTIONS_USERAGENTPROFILE);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_UA_PROFILE_MAX_LEN);
            SCI_MEMCPY(setting_ptr->connection_setting.user_agent_profile, item_value_ptr, value_len);
        }
    }

    //setting calendar
    cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_CALENDAR);
    cur_item_ptr = GetTagItemByTagName(cur_item_name_ptr, paser_result_ptr);

    //start date
    if (PNULL != cur_item_ptr && PNULL != cur_item_ptr->child)
    {
        cur_item_name_ptr = GetTagItemNameByTagType(TAG_CUSDATA_SETTING_CALENDAR_STARTDAY);
        item_value_ptr = GetTagItemValueByTagName(cur_item_name_ptr, cur_item_ptr->child);

        if (PNULL != item_value_ptr)
        {
            value_len = strlen((char*)item_value_ptr);
            value_len = MIN(value_len, XML_TAG_CONTENT_MAX_LEN);
            SCI_MEMCPY(setting_ptr->calendar_setting.start_date, item_value_ptr, value_len);
        }
    }
    
    return TRUE;
}

/* ----------------------------------------------------------------------------
* Name:         SetDefaultXMLCSCEdition
* Function:     Set the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void SetDefaultXMLCSCEdition(uint8* csc_edition, uint8 len)
{
    if (len>0 && len <= XML_TAG_CONTENT_MAX_LEN && PNULL != csc_edition)
    {
        SCI_MEMSET(s_csc_edition, 0x00, sizeof(s_csc_edition));
        SCI_MEMCPY(s_csc_edition, csc_edition, len);
    }
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultXMLCSCEdition
* Function:     Get the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultXMLCSCEdition(void)
{
    return ((uint8 *)s_csc_edition);
}

/* -------------------------------------------------------------------------
* Name:         SetDefaultSalesCode
* Function:     Set the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void SetDefaultSalesCode(uint8* sales_code_ptr, uint8 len)
{
    if (len>0 && len <= XML_TAG_CONTENT_MAX_LEN && PNULL != sales_code_ptr)
    {
        SCI_MEMSET(s_salescode_str, 0x00, sizeof(s_salescode_str));
        SCI_MEMCPY(s_salescode_str, sales_code_ptr, len);
    }
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultSalesCode
* Function:     Get the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultSalesCode(void)
{
    return ((uint8 *)s_salescode_str);
}

/* -------------------------------------------------------------------------
* Name:         SetDefaultUARelated
* Function:     Set the UA Related value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
LOCAL void SetDefaultUARelated(MMICUS_CSC_SETTING_T *setting_value_ptr)
{
    uint16 value_len = 0;

    if (PNULL != s_csc_ua_str)
    {
        SCI_FREE(s_csc_ua_str);
    }

    if (PNULL != s_csc_ua_profile_str)
    {
        SCI_FREE(s_csc_ua_profile_str);
    }

    if (PNULL == setting_value_ptr)
    {
        return;
    }

    if (0 != setting_value_ptr->connection_setting.user_agent[0])
    {
        value_len = strlen((char *)setting_value_ptr->connection_setting.user_agent);

        s_csc_ua_str = SCI_ALLOCAZ(value_len + 1);

        if (PNULL != s_csc_ua_str)
        {
            SCI_MEMCPY(s_csc_ua_str, setting_value_ptr->connection_setting.user_agent, value_len);
        }
    }

    if (0 != setting_value_ptr->connection_setting.user_agent_profile[0])
    {
        value_len = strlen((char *)setting_value_ptr->connection_setting.user_agent_profile);

        s_csc_ua_profile_str = SCI_ALLOCAZ(value_len + 1);

        if (PNULL != s_csc_ua_profile_str)
        {
            SCI_MEMCPY(s_csc_ua_profile_str, setting_value_ptr->connection_setting.user_agent_profile, value_len);
        }
    }
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultUAFromCSC(void)
{
    return s_csc_ua_str;
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA Profile from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMICUS_GetDefaultUAProfileFromCSC(void)
{
    return s_csc_ua_profile_str;
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_ParseCSCXML
* Function:     Parse Customer Specific Customization XML
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMICUS_ParseCSCXML(void)
{
#ifdef XML_SUPPORT
    MMICUS_CSC_SETTING_T *setting_value_ptr = {0};
    BOOLEAN ret_value = FALSE;
    BOOLEAN is_csc_init = FALSE;
    uint16 value_len = 0;

    //SCI_TRACE_LOW:"MMICUS_ParseCSCXML: Begin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICUS_XMLPARSER_2845_112_2_18_2_12_38_0,(uint8*)"");

    setting_value_ptr = SCI_ALLOCAZ(sizeof(MMICUS_CSC_SETTING_T));

    if (PNULL == setting_value_ptr)
    {
        return FALSE;
    }

    ret_value = XMLParseXMLFile((MMI_DATA_ID_T)R_CUSTOMER_SPECIFIC_XML, setting_value_ptr);

    //MCCMNC
    MMIAPIPHONE_SetNetWorkNameByCSC(setting_value_ptr->general_setting.general_networkinfo, setting_value_ptr->general_setting.nb_network_info);

    //SOS
    MMIAPICC_SetSOSByCSC(&setting_value_ptr->main_setting.network_setting.sos_num);

    // Set the CSC editiion value from the XML file
    value_len = strlen((char *)setting_value_ptr->general_setting.csc_edition);
    SetDefaultXMLCSCEdition(setting_value_ptr->general_setting.csc_edition, value_len);

    // Set the SalesCode from the XML file
    value_len = strlen((char *)setting_value_ptr->general_setting.sales_code);
    SetDefaultSalesCode(setting_value_ptr->general_setting.sales_code, value_len);

    // Parse UA/UA Profile
    SetDefaultUARelated(setting_value_ptr);

    //MMS Limit
#if defined(MMS_SUPPORT)
    if (setting_value_ptr->message_setting.is_mms_limit)
    {
        MMIAPIMMS_SetMMSLimitSize(setting_value_ptr->message_setting.mms_limit);
    }
#endif
    
    is_csc_init = MMICUS_GetCscIsInit();

    if (!is_csc_init)
    {
        MMICUS_SetCscIsInit(TRUE); //Update CSC init NV info.

        if (ret_value)
        {
            ParseSetXMLSettings(setting_value_ptr);
        }
    }

    SettingValueDestroy(setting_value_ptr);

    SCI_FREE(setting_value_ptr);
    setting_value_ptr = PNULL;

    //SCI_TRACE_LOW:"MMICUS_ParseCSCXML: End"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICUS_XMLPARSER_2898_112_2_18_2_12_38_1,(uint8*)"");

    return TRUE;
#else
    return FALSE;
#endif
}

/*******************************************************************/
//  Interface:		MMICUS_GetMatchLang
//  Description : 	Get Match Lang
//  Global resource dependence : 
//  Author: 
//  Note: 
/*******************************************************************/
PUBLIC uint32 MMICUS_GetMatchLang(const char * lang_name)
{
    uint32 i = 0;
    uint32 lang_num = ARR_SIZE(s_lang_name);

    if (PNULL == lang_name)
    {
        return MMISET_MAX_LANGUAGE;
    }

    for (i = 0; i < lang_num; i++)
    {
        if (0 == stricmp(s_lang_name[i].lang_name, lang_name))
        {
            return (uint32)s_lang_name[i].lang_type;
        }
    } 

    return MMISET_MAX_LANGUAGE;
}

#endif

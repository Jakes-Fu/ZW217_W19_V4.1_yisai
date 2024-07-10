/*****************************************************************************
** File Name:       wbxml_api.h
** Author:          spreadtrum
** Date:            2009/11/01
** Copyright:       2009 Spreadtrum, Incorporated. All Rights Reserved.         
** Description:     WBXML global header
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION
** 2009/11/01       spreadtrum          Create
******************************************************************************/

#ifndef WBXML_API_H
#define WBXML_API_H

#include "os_api.h"
#include <stdlib.h>
#include <string.h>
#include "cfl_char.h"

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
#define WBXML_WML_SUPPORT
#define WBXML_SI_SUPPORT
#define WBXML_SL_SUPPORT
#define WBXML_CO_SUPPORT
#define WBXML_PROV_SUPPORT
#define WBXML_OTA_SETTINGS_SUPPORT

/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/
typedef enum {
    WBXML_VERSION_UNKNOWN = -1, /* Unknown WBXML Version */
    WBXML_VERSION_10 = 0x00,    /* WBXML 1.0 Token */
    WBXML_VERSION_11 = 0x01,    /* WBXML 1.1 Token */
    WBXML_VERSION_12 = 0x02,    /* WBXML 1.2 Token */
    WBXML_VERSION_13 = 0x03     /* WBXML 1.3 Token */
} WBXML_VERSINON_E;

typedef enum {
    WBXML_LANG_UNKNOWN = 0,     /* Unknown */ 
#if defined( WBXML_WML_SUPPORT )
    WBXML_LANG_WML10,           /* WML 1.0 */
    WBXML_LANG_WML11,           /* WML 1.1 */
    WBXML_LANG_WML12,           /* WML 1.2 */
    WBXML_LANG_WML13,           /* WML 1.3 */
#endif 
#if defined( WBXML_SI_SUPPORT )
    WBXML_LANG_SI10,            /* SI 1.0 */
#endif 
#if defined( WBXML_SL_SUPPORT )
    WBXML_LANG_SL10,            /* SL 1.0 */
#endif 
#if defined( WBXML_CO_SUPPORT )
    WBXML_LANG_CO10,            /* CO 1.0 */
#endif 
#if defined( WBXML_PROV_SUPPORT )
    WBXML_LANG_PROV10,          /* PROV 1.0 */
#endif 
#if defined( WBXML_OTA_SETTINGS_SUPPORT )
    WBXML_LANG_OTA_SETTINGS,    /* OTA Settings */
#endif 
} WBXML_LANG_E;

typedef enum {
    WBXML_CHARSET_UNKNOWN         = 0,       /* Unknown Charset */
    WBXML_CHARSET_US_ASCII        = 3,       /* US-ASCII */
    WBXML_CHARSET_ISO_8859_1      = 4,       /* ISO-8859-1 */
    WBXML_CHARSET_ISO_8859_2      = 5,       /* ISO-8859-2 */
    WBXML_CHARSET_ISO_8859_3      = 6,       /* ISO-8859-3 */
    WBXML_CHARSET_ISO_8859_4      = 7,       /* ISO-8859-4 */
    WBXML_CHARSET_ISO_8859_5      = 8,       /* ISO-8859-5 */
    WBXML_CHARSET_ISO_8859_6      = 9,       /* ISO-8859-6 */
    WBXML_CHARSET_ISO_8859_7      = 10,      /* ISO-8859-7 */
    WBXML_CHARSET_ISO_8859_8      = 11,      /* ISO-8859-8 */
    WBXML_CHARSET_ISO_8859_9      = 12,      /* ISO-8859-9 */
    WBXML_CHARSET_SHIFT_JIS       = 17,      /* Shift_JIS */
    WBXML_CHARSET_UTF_8           = 106,     /* UTF-8 */
    WBXML_CHARSET_ISO_10646_UCS_2 = 1000,    /* ISO-10646-UCS-2 */
    WBXML_CHARSET_UTF_16          = 1015,    /* UTF-16 */
    WBXML_CHARSET_GB2312          = 2025,    /* GB2312. jiqun.li added */
    WBXML_CHARSET_BIG5            = 2026     /* Big5 */
} WBXML_CHARSET_E;

typedef enum {
    WBXML_OK = 0,       /* No Error */
    WBXML_ERROR_ATTR_TABLE_UNDEFINED,
    WBXML_ERROR_BAD_DATETIME,
    WBXML_ERROR_BAD_PARAMETER,    
    WBXML_ERROR_INTERNAL,
    WBXML_ERROR_LANG_TABLE_UNDEFINED,
    WBXML_ERROR_NOT_ENOUGH_MEMORY,
    WBXML_ERROR_TAG_TABLE_UNDEFINED,
    WBXML_ERROR_CHARSET_STR_LEN,
    WBXML_ERROR_ATTR_VALUE_TABLE_UNDEFINED,
    WBXML_ERROR_BAD_OPAQUE_LENGTH,
    WBXML_ERROR_EMPTY_WBXML,
    WBXML_ERROR_END_OF_BUFFER,
    WBXML_ERROR_ENTITY_CODE_OVERFLOW,
    WBXML_ERROR_INVALID_STRTBL_INDEX,
    WBXML_ERROR_NULL_PARSER,
    WBXML_ERROR_NULL_STRING_TABLE,
    WBXML_ERROR_STRING_EXPECTED,
    WBXML_ERROR_STRTBL_LENGTH,   
    WBXML_ERROR_UNKNOWN_ATTR,
    WBXML_ERROR_UNKNOWN_ATTR_VALUE,
    WBXML_ERROR_UNKNOWN_EXTENSION_TOKEN,
    WBXML_ERROR_UNKNOWN_EXTENSION_VALUE,
    WBXML_ERROR_UNKNOWN_PUBLIC_ID,
    WBXML_ERROR_UNKNOWN_TAG,
    WBXML_ERROR_UNVALID_MBUINT32,
} WBXML_ERR_E;

typedef enum 
{
    WBXML_VALUE_TOKEN = 0, /* Token value */
    WBXML_VALUE_LITERAL    /* Literal value */
} WBXML_VALUE_TYPE_E;

typedef struct
{    
    const int8 *xml_name_ptr;   /*XML Tag Name */    
    uint8       wbxml_codepage; /*WBXML Code Page */
    uint8       wbxml_token;    /*WBXML Tag Token */
} WBXML_TAG_INFO_T;

typedef struct
{
    uint8   *data;            /* The data */
    uint32  len;              /* Length of data in buffer */
    uint32  size;             /* size of buffer */
    uint32  block_size;       /* Malloc Block Size */
    BOOLEAN is_static;        /* Is it a static buffer*/
} WBXML_BUFFER_T;

typedef struct
{
    const int8 *xml_name_ptr;   /*XML Attribute Name */
    const int8 *xml_value_ptr;  /*XML Attribute Value*/
    uint8       wbxml_codepage; /*WBXML Code Page */
    uint8       wbxml_token;    /*WBXML Attribute Token */
} WBXML_ATTR_INFO_T;

typedef struct
{
    const int8 *xml_name_ptr;       /*XML Attribute Value */
    uint8       wbxml_codepage;     /* WBXML Code Page */
    uint8       wbxml_token;        /* WBXML Attribute Value Token */
} WBXML_ATTRVALUE_INFO_T;

typedef struct
{
    const int8 *xml_name_ptr;    /* XML Extension Value */
    uint8       wbxml_token;     /* WBXML Extension Value Token */
} WBXML_VALUEEXT_INFO_T;

typedef struct
{
    uint32     wbxml_publicid;      /* WBXML Public ID */
    const int8 *xml_publicid_ptr;   /* XML Public ID */
    const int8 *xml_rootele_ptr;    /* XML Root Element */
    const int8 *xml_dtd_ptr;        /* XML DTD */
} WBXML_PUBLIC_ID_T;

typedef struct
{    
    const int8 *xml_namespace_ptr;  /* XML Name Space */   
    uint8       wbxml_codepage;     /* WBXML Code Page */
} WBXML_NAMESPACE_INFO_T;

typedef struct
{
    WBXML_LANG_E                 langid;              /* Language ID */
    const WBXML_PUBLIC_ID_T      *publicid_ptr;       /* Public ID */
    const WBXML_TAG_INFO_T       *tag_table_ptr;       /* Tags Table */
    const WBXML_NAMESPACE_INFO_T *ns_table_ptr;        /* NameSpaces Table */
    const WBXML_ATTR_INFO_T      *attr_table_ptr;      /* Attributes Table*/
    const WBXML_ATTRVALUE_INFO_T *attr_valuetable_ptr; /* Attributes Values Table */
    const WBXML_VALUEEXT_INFO_T  *ext_valuetable_ptr;  /* Extensions Values Table */
} WBXML_LANG_TABLE_T;

typedef struct 
{
    WBXML_VALUE_TYPE_E type;   /*Token or Literal*/
    union
    {
        const WBXML_TAG_INFO_T *token;   /* Token Tag */
        WBXML_BUFFER_T      *literal; /* Literal Tag */
    }u;
} WBXML_TAG_T;

typedef struct 
{
    WBXML_VALUE_TYPE_E type;   /* Token or Literal */
    union 
    {
        const WBXML_ATTR_INFO_T *token;   /* Token Attribute Name */
        WBXML_BUFFER_T          *literal; /* Literal Attribute Name */
    } u;
} WBXML_ATTRNAME_T;

typedef struct {
    WBXML_ATTRNAME_T *name;  
    WBXML_BUFFER_T   *value;
} WBXML_ATTR_T;

/*****************************************************************************/
// Description : wbxml End Document handler
// Global : none
// Author : spreadtrum
// ctx_ptr: User data
// charset: Charset (The MIBenum from IANA character-sets assignements. See "http://www.iana.org/assignments/character-sets")
// lang_ptr: Language Table for this Document (cf: wbxml_table.[h|c])
/*****************************************************************************/
typedef void (*WBXML_StartDocHandler)(void *ctx_ptr, WBXML_CHARSET_E charset, const WBXML_LANG_TABLE_T *lang_ptr);

/*****************************************************************************/
// Description : wbxml End Document handler
// Global : none
// Author : spreadtrum
// ctx_ptr User data
/*****************************************************************************/
typedef void (*WBXML_EndDocHandler)(void *ctx_ptr);

/*****************************************************************************/
// Description : wbxml Start Element handler
// Global : none
// Author : spreadtrum
// ctx_ptr User data
// localname_ptr: The local tag name
// atts_pptr: The attributes attached to the element
// empty: Set to TRUE if this is an empty element
/*****************************************************************************/
typedef void (*WBXML_StartTagHandler)(void *ctx_ptr, WBXML_TAG_T *localname_ptr, WBXML_ATTR_T **atts_pptr, BOOLEAN empty);

/*****************************************************************************/
// Description : wbxml End Element handler
// Global : none
// Author : spreadtrum
// ctx_ptr User data
// localname_ptr: The local tag name
// empty: Set to TRUE if this is an empty element
/*****************************************************************************/

typedef void (*WBXML_EndTagHandler)(void *ctx_ptr, WBXML_TAG_T *localname_ptr, BOOLEAN empty);

/*****************************************************************************/
// Description : wbxml Characters handler
// Global : none
// Author : spreadtrum
// ctx_ptr User data
// ch: The processing instruction target.
// start: The start position in the array
// length: The number of characters to read from the array
/*****************************************************************************/
typedef void (*WBXML_CharacterHandler)(void *ctx_ptr, uint8 *ch_ptr, uint32 start, uint32 length);

/*****************************************************************************/
// Description : wbxml Processing Instruction Handler
// Global : none
// Author : spreadtrum
// ctx_ptr User data
// target_ptr: The processing instruction target.
// data_ptr: The processing instruction data, or null if  none was supplied. 
/*****************************************************************************/
typedef void (*WBXML_ProcessInstructionHandler)(void *ctx_ptr, const uint8 *target_ptr, uint8 *data_ptr);

typedef struct{
    WBXML_StartDocHandler           start_document_clb;     /* Start Document Handler */
    WBXML_EndDocHandler             end_document_clb;       /* End Document handler */
    WBXML_StartTagHandler           start_element_clb;      /* Start Element handler */
    WBXML_EndTagHandler             end_element_clb;        /* End Element handler */
    WBXML_CharacterHandler          characters_clb;         /* Characters handler */
    WBXML_ProcessInstructionHandler pi_clb;                 /* Processing Instruction Handler */
} WBXML_CONTENT_HANDLER;

typedef struct{
    void*                       user_data;           /* User Data */
    WBXML_CONTENT_HANDLER*      content_hdl_ptr;     /* Content Handlers Callbacks */
    WBXML_BUFFER_T*             wbxml_ptr;           /* The wbxml we are parsing */    
    WBXML_BUFFER_T*             strstbl_ptr;         /* String Table specified in WBXML document */
    const WBXML_LANG_TABLE_T*   lang_table_ptr;      /* Current document Language Table */
    const WBXML_LANG_TABLE_T*   main_table_ptr;      /* Main WBXML Languages Table */
    const WBXML_TAG_INFO_T*     current_tag;         /* Current Tag */
    WBXML_LANG_E                lang_forced;         /* Language forced by User */
    uint32                      public_id;           /* Public ID specified in WBXML document */    
    int32                       public_id_index;     /* If Public ID is a String Table reference, is the index defined in the strtbl */
    WBXML_CHARSET_E             charset;             /* Charset of WBXML document */
    WBXML_CHARSET_E             meta_charset;        /* Meta-info provided by user: only used if Charset is not specified in WBXML document */
    uint32                      pos;                 /* Position of parsing curser in wbxml */    
    WBXML_VERSINON_E            version;             /* WBXML Version field specified in WBXML document */
    uint8                       tagCodePage;         /* Current Tag Code Page */
    uint8                       attrCodePage;        /* Current Attribute Code Page */
}WBXML_PARSER_T;

/*---------------------------------------------------------------------------*
 *                          FUNCTION DECLARATION                             *
 *---------------------------------------------------------------------------*/
 
/*****************************************************************************/
// Description : create a WBXML parser
// Global : none
// Author : spreadtrum
// Notes  : RETURN: newly created WBXML parser, NULL if not enough memory
/*****************************************************************************/
PUBLIC WBXML_PARSER_T* WBXML_CreateParser( void );

/*****************************************************************************/
// Description : destroy a WBXML parser
// Global : none
// Author : spreadtrum
// Notes  : PARAM : parser_ptr (IN) - the WBXML parser to destroy
/*****************************************************************************/
PUBLIC void WBXML_DestroyParser(WBXML_PARSER_T *parser_ptr);

/*****************************************************************************/
// Description : parse a WBXML document using user defined callbacks
// Global : none
// Author : spreadtrum
// Notes  : PARAM1: parser_ptr (IN) - the WBXML parser to use for parsing
//          PARAM2: wbxml_ptr (IN) - the WBXML document to parse
//          PARAM3: wbxml_len (IN) - the WBXML document length
//          RETURN: return WBXML_OK if no error, otherwise an error code
/*****************************************************************************/
PUBLIC WBXML_ERR_E WBXML_ParserParse(WBXML_PARSER_T *parser_ptr, uint8 *wbxml_ptr, uint32 wbxml_len);

/*****************************************************************************/
// Description : set user data for a WBXML parser
// Global : none
// Author : spreadtrum
// Notes  : PARAM1: parser_ptr (IN) - the WBXML parser
//          PARAM2: user_data (IN) - user data which returned as a parameter
//                  in every Content Handler callbacks
/*****************************************************************************/
PUBLIC void WBXML_ParserSetUserData(WBXML_PARSER_T *parser_ptr, void *user_data_ptr);

/*****************************************************************************/
// Description : set content handler for a WBXML parser
// Global : none
// Author : spreadtrum
// Notes  : PARAM1: parser_ptr (IN) - the WBXML parser
//          PARAM2: content_handler_ptr (IN) - the content handler structure
/*****************************************************************************/
PUBLIC void WBXML_ParserSetContentHandler(WBXML_PARSER_T *parser_ptr, WBXML_CONTENT_HANDLER *content_handler_ptr);

/*****************************************************************************/
// Description : force to parse the document of a given language
// Global : none
// Author : spreadtrum
// Notes  : PARAM1: parser_ptr (IN) - the WBXML parser
//          PARAM2: lang (IN) - The language
//          RETURN: TRUE if language is set, FALSE otherwise
//          This permits to force the WBXML parser to parse a WBXML document of
//          a given language. If this function is used, the internal public ID
//          of the WBXML document is ignored. It is sometimes needed for
//          documents that don't have any WBXML public ID.
/*****************************************************************************/
PUBLIC BOOLEAN WBXML_ParserSetLanguage(WBXML_PARSER_T *parser_ptr, WBXML_LANG_E lang);

/*****************************************************************************/
// Description : WBXML Get XML Name of a Tag
// Global : none
// Author : spreadtrum
// tag_ptr: The  tag name
/*****************************************************************************/
PUBLIC const uint8 * WBXML_TagGetXmlName(WBXML_TAG_T *tag_ptr);

/*****************************************************************************/
// Description : WBXML Get XML Attribute Name of a Attribute
// Global : none
// Author : spreadtrum
// attr_ptr: The  Attribute name
/*****************************************************************************/
PUBLIC const uint8 * WBXML_TagAttrGetXmlName(WBXML_ATTR_T *attr_ptr);

/*****************************************************************************/
// Description : WBXML Get XML Attribute value of a Attribute
// Global : none
// Author : spreadtrum
// attr_ptr: The  Attribute name
/*****************************************************************************/
PUBLIC const uint8 * WBXML_TagAttrGetXmlValue(WBXML_ATTR_T *attr_ptr);
#endif /* WBXML_H */

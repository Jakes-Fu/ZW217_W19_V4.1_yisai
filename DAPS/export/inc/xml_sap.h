/*****************************************************************************
** File Name:      xml_sap.h    				                             *
** Author:                                                                   *
** Date:           02/29/2008                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    xml library interface for spreadtrum platform    		 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/29/2008     shulin.chang     Create									 *	
******************************************************************************/

#ifndef _XML_SAP_H_
#define _XML_SAP_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "cfl_wchar.h"

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
// Error code
#define CFLXML_ERR_OK   0           // success
#define CFLXML_ERR_PARSE_FAILED -1  // failed
#define CFLXML_ERR_TRANSCODING  -2  // convert utf8 to utf16 failed



/*---------------------------------------------------------------------------*/
/*                          Enum			                                 */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          typedef				                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Output encoding is UNICODE			             */
/*---------------------------------------------------------------------------*/
/**
 * CFLXMLSAXStartElementFunc:
 * user_data_ptr:   the user data 
 * localname_ptr:   the local name of the element
 *                  If use namespace and namespace exists, localname_ptr will be:
 *                  "namespace" + "namespace_separator" + "name"
 *                  For example http://www.openmobilealliance.org/xmlns/dd$name
 *                  '$' is the namespace separator set by user
 * attributes_pptr: is array of name/value pairs, terminated by 0;
 *                  names and values are 0 terminated.
 * 
 * SAX callback when an element start has been detected by the parser.
 * It provides the namespace information for the element if required
 */
typedef void (*CFLXMLSAXStartElementFunc)(
    void *user_data_ptr,
	const CFLWCHAR *localname_ptr,
	const CFLWCHAR **attributes_pptr);


/**
 * CFLXMLSAXEndElementFunc:
 * user_data_ptr:  the user data 
 * localname_ptr:  the local name of the element
 *                 If use namespace and namespace exists, localname_ptr will be:
 *                  "namespace" + "namespace_separator" + "name"
 *                  For example http://www.openmobilealliance.org/xmlns/dd$name
 *                  '$' is the namespace separator set by user 
 *
 * SAX callback when an element end has been detected by the parser.
 * It provides the namespace information for the element if required
 */

typedef void (*CFLXMLSAXEndElementFunc)(
    void *user_data_ptr,
	const CFLWCHAR *localname_ptr);


/**
 * CFLXMLSAXCharactersFunc:
 * user_data_ptr:  the user data
 * ch_ptr:  a CFLWCHAR string of element content
 *
 * Receiving some chars from the parser.
 * Maybe called more than once for an element.
 */
typedef void (*CFLXMLSAXCharactersFunc)(
    void *user_data_ptr,
	const CFLWCHAR *ch_ptr );



/*---------------------------------------------------------------------------*/
/*                         Output encoding is UTF8  			             */
/*---------------------------------------------------------------------------*/
/**
 * CFLXMLSAXStartElementFuncUTF8:
 * user_data_ptr:   the user data 
 * localname_ptr:   the local name of the element
 *                  If use namespace and namespace exists, localname_ptr will be:
 *                  "namespace" + "namespace_separator" + "name"
 *                  For example http://www.openmobilealliance.org/xmlns/dd$name
 *                  '$' is the namespace separator set by user
 * attributes_pptr: is array of name/value pairs, terminated by 0;
 *                  names and values are 0 terminated.
 * 
 * SAX callback when an element start has been detected by the parser.
 * It provides the namespace information for the element if required
 */
typedef void (*CFLXMLSAXStartElementFuncUTF8)(
    void *user_data_ptr,
	const char *localname_ptr,
	const char **attributes_pptr);


/**
 * CFLXMLSAXEndElementFuncUTF8:
 * user_data_ptr:  the user data 
 * localname_ptr:  the local name of the element
 *                 If use namespace and namespace exists, localname_ptr will be:
 *                  "namespace" + "namespace_separator" + "name"
 *                  For example http://www.openmobilealliance.org/xmlns/dd$name
 *                  '$' is the namespace separator set by user 
 *
 * SAX callback when an element end has been detected by the parser.
 * It provides the namespace information for the element if required
 */

typedef void (*CFLXMLSAXEndElementFuncUTF8)(
    void *user_data_ptr,
	const char *localname_ptr);


/**
 * CFLXMLSAXCharactersFuncUTF8:
 * user_data_ptr:  the user data
 * ch_ptr:  a char string of element content, NULL-Terminated
 * 
 *
 * Receiving some chars from the parser.
 * Maybe called more than once for an element.
 */
typedef void (*CFLXMLSAXCharactersFuncUTF8)(
    void *user_data_ptr,
	const char *ch_ptr );

/*---------------------------------------------------------------------------*/
/*                          Struct				                             */
/*---------------------------------------------------------------------------*/
// Parser Context 
typedef struct {
    int32 dummy;    // may be used in the future
} CFLXML_PARSER_T;


/*---------------------------------------------------------------------------*/
/*                          Message				                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          Variable			                             */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description: create a new parser
//    Global resource dependence : 
//    Author: shulin.chang
//	  Return: 
//		Returns the new parser or NULL
//    Note: 
//      If name_sep is NULL, namespace is not used
//      Output encoding is UNICODE
/*****************************************************************************/
PUBLIC CFLXML_PARSER_T* CFLXML_CreateParser( 
    CFLXMLSAXStartElementFunc StartElementProc,     // StartElement callback
    CFLXMLSAXEndElementFunc EndElementProc,         // EndElement callback
    CFLXMLSAXCharactersFunc CharactersProc,         // Characters Callback
    char* name_sep,                                 // Name space separator 
    void* user_data_ptr );                          // User data


/*****************************************************************************/
//    Description: create a new parser
//    Global resource dependence : 
//    Author: shulin.chang
//	  Return: 
//		Returns the new parser or NULL
//    Note: 
//      If name_sep is NULL, namespace is not used
//      Output encoding is UTF8
/*****************************************************************************/
PUBLIC CFLXML_PARSER_T* CFLXML_CreateParserUTF8( 
    CFLXMLSAXStartElementFuncUTF8 StartElementProcUTF8,     // StartElement callback
    CFLXMLSAXEndElementFuncUTF8 EndElementProcUTF8,         // EndElement callback
    CFLXMLSAXCharactersFuncUTF8 CharactersProcUTF8,         // Characters Callback
    char* name_sep,                                 // Name space separator 
    void* user_data_ptr );     


/*****************************************************************************/
//    Description: parse document
//    Global resource dependence : 
//    Author: shulin.chang
//	  Return: 
//		If success, return CFLXML_ERR_OK 
//      Otherwise error code
//    Note: 
//        Parse some more of the document. The string buff_ptr is a buffer 
//        containing part (or perhaps all) of the document. The number of 
//        bytes of buff_ptr that are part of the document is indicated by len. 
//        This means that buff_ptr doesn't have to be null terminated. 
//        It also means that if len buff_ptr larger than the number of bytes 
//        in the block of memory that buff_ptr points at, then a memory fault is likely. 
//        The isFinal parameter informs the parser that this is the last piece of the document. 
//        Frequently, the last piece is empty (i.e. len is zero.)         
/*****************************************************************************/
int32 CFLXML_Parse(
    CFLXML_PARSER_T* parser_ptr,    // parser
    const uint8 *buff_ptr,          // part of document
    int len,                        // buff's length
    BOOLEAN is_final );             // is the end of document?


/*****************************************************************************/
//    Description: free a parser
//    Global resource dependence : 
//    Author: shulin.chang
//	  Return: 
//		void
//    Note: 
/*****************************************************************************/
void CFLXML_FreeParser( CFLXML_PARSER_T* parser_ptr );


/*****************************************************************************/
//    Description: stop parser
//    Global resource dependence : 
//    Author: shulin.chang
//	  Return: 		
//    Note: must be called in the callback handler, Some call-backs may
//      still follow because they would otherwise get lost. 
/*****************************************************************************/
void CFLXML_StopParser( CFLXML_PARSER_T* parser_ptr );

/*****************************************************************************/
//    Description: Get Current byte index
//    Global resource dependence : 
//    Author: juan.zhang
//	  Return: 
//		Returns the Current byte index
//    Note: 
/*****************************************************************************/
PUBLIC int32 CFLXML_GetCurrentByteIndex(CFLXML_PARSER_T *xml_parser_ptr);
/*****************************************************************************/
//    Description: Get Current line number
//    Global resource dependence : 
//    Author: juan.zhang
//	  Return: 
//		Returns the Current line number
//    Note: 
/*****************************************************************************/
PUBLIC uint32 CFLXML_GetCurrentLineNum(CFLXML_PARSER_T *xml_parser_ptr);
/*****************************************************************************/
//    Description: Get error string according to error code
//    Global resource dependence : 
//    Author: juan.zhang
//	  Return: 
//		Returns the error string, if no error return 0
//    Note: 
/*****************************************************************************/
PUBLIC const char* CFLXML_GetErrorString(CFLXML_PARSER_T *xml_parser_ptr);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif // _XML_SAP_H_

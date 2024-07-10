/* $Id: ps_actionprocess.h V1.0 2001/09/19 */
/**********************************************************************************************
*    This source code has been made available to you by CORETEK on an AS-IS
*    basis.  Anyone receiving this source is licensed under CORETEK
*    copyrights to use it in any way he or she deems fit, including
*    copying it, modifying it, compiling it, and redistributing it either
*    with or without modifications.
*
*    Any person who transfers this source code or any derivative work
*    must include the CORETEK copyright notice, this paragraph, and the
*    preceding two paragraphs in the transferred software.
*
*    COPYRIGHT   CORETEK  CORPORATION 2001
*    LICENSED MATERIAL  -  PROGRAM PROPERTY OF CORETEK
*********************************************************************************************/

/**********************************************************************************************
* FILE: ps_actionprocess.h
* MODULE: Parse Wml
*
*  PURPOSE:  Macro define,structure define,global variants or functions declare .
*
* AUTHOR(S): ZhiLi Wang
* GROUP: Browser Group
* DATE CREATED: 2001/09/20
* REFERENCE DOCUMENT ID: Detail Design of WapBrowser
* MODIFICATIONS:
*    Date        userName     Description
*    2001/07/20  ZhiLi Wang   Create this file
*	   2001/09/19  XueSong Wang Modify this file
*********************************************************************************************/
#ifndef _ERROR_H_
#define _ERROR_H_

#include "wml_config.h"
/********************************************************************************************
*                               macro  declare
********************************************************************************************/
#define WML_CONTENT                   0
#define WBXML_CONTENT                 1
#define IMAGE_CONTENT                 2
#define WMLSCRIPT_CONTENT             3
#define WMLSCRIPTC_CONTENT            4
#define WML_SSLCERT   -1020      
#define INVALID_TAG_NUMBER           -1001
#define INVALID_WBXMLCONTENT_LENGTH  -1002
#define TAGINFOSTACK_NULL            -1003
#define INVALID_WML_CONTENT_LENGTH   -1004
#define WML_CONTENT_ERR              -1005
#define TAGNEME_TOO_LONG             -1006
#define INVALID_TAGNAME_POS          -1007
#define STACK_OVERFLOW               -1008
#define INVALID_TAG_POS              -1009
#define INVALID_WML_CONTENT          -1010 
#define INVALID_HTTP_RESPONSE        -1011
#define UNEXPECT_EXCEPTION           -1012
#define OTHER_CONTENT                -1013 
#define URL_ERROR   				 -1014
#define REDIRECTION_ERROR            -1015
/********************************************************************************************
*                               functions  declare
********************************************************************************************/
#ifdef __cplusplus
extern  "C" {
#endif
void GetLastErrShow(void);
#ifdef __cplusplus
}
#endif
extern char Delta_SetPrompt(char* string);  /*设置提示行*/
#endif  /*_ERROR_H_*/  
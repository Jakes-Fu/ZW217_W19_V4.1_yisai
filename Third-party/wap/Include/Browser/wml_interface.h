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
#ifndef _INTERFACE_H_
#define _INTERFACE_H_
/********************************************************************************************
*                               functions  declare
********************************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif 
  extern char  BeginTimer(Int16 inteval);
  extern char  EndTimer(void);
  extern void  RedisplayPageContent(void);
  extern void  DisplayPageContent(void);
  extern char* URLEncode(char*);
  extern int GetContentByUrl(char* url);
  extern void LoadDeckImage(void);
#ifdef __cplusplus
}
#endif

#endif /*_ACTIONPROCESS_H_*/
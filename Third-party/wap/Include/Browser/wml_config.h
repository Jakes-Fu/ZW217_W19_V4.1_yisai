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
#ifndef _CONFIG_H_
#define _CONFIG_H_

/********************************************************************************************
*                               macro  declare
********************************************************************************************/
#define   CUSTOM_STRNICMP
//#define   WAP_DEBUG
#define   USING_CACHE
//#define   USING_HTML
#define	  ONLINE
#define	  MEM_MALLOC
#define UTF8
//#define   ROS33
//#define   MEIJIN
#define MYMALLOC
#ifdef MEM_MALLOC
#define OPR ->
#else 
#define OPR .
#endif
//#define   DRAGON_BALL
#ifndef MYMALLOC
#ifdef MEIJIN
#define malloc _MemAlloc
#define free _MemFree 
#define calloc MemCalloc
#endif
#else
#define malloc MYMalloc
#define free MYFree
#define calloc MYCalloc
#endif
#endif  /*_CONFIG_H_*/
#ifdef WAP_DEBUG
unsigned long sizeofunusedmem(void);
unsigned int SCI_TraceLow(const char *x_format, ...);
void SCI_TraceBuf(const char    *string_ptr,void    *addr,unsigned int   size);

#endif

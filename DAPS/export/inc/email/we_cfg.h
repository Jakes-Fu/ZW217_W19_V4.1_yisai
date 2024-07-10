/*=====================================================================================

    HEADER NAME : we_cfg.h
    MODULE NAME : WE Framework Config-Macro Define

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file can be included by all WE Programmer.In this file we's config macro
    is defined.
        These Info is  variational and is changed by switching Project.

    TECHSOFT Confidential Proprietary
    (c) Copyright 2006 by TechSoft. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-05-30   HiroWang              create this file

    Self-documenting Code
    Describe/explain low-level design, especially things in header files, of this module and/or group 
    of funtions and/or specific funtion that are hard to understand by reading code and thus requires 
    detail description.
    Free format !

=====================================================================================*/
/*--------------------------START-----------------------------*/


/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_CONFIG_H
#define WE_CONFIG_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/

/*Include Module head file*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
#ifndef WE_DEBUG_INFO

/*Open Debug Info*/
#define WE_DEBUG_INFO

//#define DYNAMIC_MEMORY_DEBUG_FTR //2007/12/14 Samuel ,dynamic memory check 
#endif

#ifndef WE_DEBUG_ERROR

/*Open Debug ERROR*/
#define WE_DEBUG_ERROR
#endif

/*Open Dynamic compile switch*/
#define  WE_BREW_DYNAMIC_COMPILE

#ifndef  WE_BREW_DYNAMIC_COMPILE

#ifndef  WE_BREW_STATIC_COMPILE

/*Open Static compile switch*/
#define  WE_BREW_STATIC_COMPILE

#endif

#endif

/*coresponding to WAP_CONFIG_WTLS in wap_config.h*/
//#define  WE_ALG_SEC

/*open when wap_config.h open WAP_CONFIG_APP_MANAGE_CERTIFICATE*/
#define WE_CONFIG_APP_MANAGE_CERTIFICATE
/*default close*/
//#define WE_CONFIG_SEC_PARSE_CERTIFICATE
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
/*Define Constant Macro end*/


/*Define Macro Function start*/
/*Define Macro Function end*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/
/*Define base type end*/

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/


#endif/*endif WE_CONFIG_H*/



/*--------------------------END-----------------------------*/



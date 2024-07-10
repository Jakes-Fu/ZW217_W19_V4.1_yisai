/*=====================================================================================

    HEADER NAME : we_log.h
    MODULE NAME : WE Framework LOG Interface Define

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file can be included by all WE Programmer.In this file all Log-Interface
    is defined.
        These Info is  aptotic and is not changed by switching Project.

    TECHFAITH Software Confidential Proprietary
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
#ifndef WE_LOG_H
#define WE_LOG_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/
// #include "AEESTDLIB.h"
#include "we_cfg.h"
#include "we_scl.h"


/*Include Program Global head file*/

/*Include Module head file*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
/*Define Constant Macro end*/


/*Define Macro Function start*/

/*=====================================================================================
FUNCTION: 
    We_LogError
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Print log-Error to out window.
ARGUMENTS PASSED:
    pcFormat: printf-style format-control string .
    ...:variable argument array of things that are legal to pass to printf .
RETURN VALUE:
    none.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need print log error.
IMPORTANT NOTES:
    none.      

  
=====================================================================================*/
#define WE_LOGERROR  We_LogError
#define WE_LOGFATAL  We_LogFatal
/*void We_LogError(const WE_CHAR *pcFormat ,...)*/
#ifdef WE_DEBUG_ERROR
#ifndef WIN32_DEBUG
#include "tf3rd_for_HZ.h"
#define We_LogError(x) WE_SCL_PRINTF(x)
#define We_LogFatal(x) WE_SCL_PRINTF(x)
#endif
#else 
#define We_LogError(x) 
#define We_LogFatal(x) 
#endif

/*=====================================================================================
FUNCTION: 
    We_LogInfo
CREATE DATE: 
    2006-05-30
AUTHOR: 
    Hiro Wang
DESCRIPTION:
    Print log-Info to out window.
ARGUMENTS PASSED:
    pcFormat: printf-style format-control string .
    ...:variable argument array of things that are legal to pass to printf .
RETURN VALUE:
    none.
USED GLOBAL VARIABLES:
    none
USED STATIC VARIABLES:
    none
CALL BY:
    All program need print log info.
IMPORTANT NOTES:
    none.      

  
=====================================================================================*/
#define WE_LOGINFO_LOW  We_LogInfo_Low
#define WE_LOGINFO_MED  We_LogInfo_Med
#define WE_LOGINFO_HIGH  We_LogInfo_High
/*void We_LogError(const WE_CHAR *pcFormat ,...)*/
#ifdef WE_DEBUG_INFO
#define We_LogInfo_Low(x)  WE_SCL_PRINTF(x)
#define We_LogInfo_Med(x)  WE_SCL_PRINTF(x)
#define We_LogInfo_High(x) WE_SCL_PRINTF(x)
#else 
#define We_LogInfo_Low(x)  
#define We_LogInfo_Med(x)  
#define We_LogInfo_High(x)  
#endif

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

#endif/*endif WE_LOG_H*/



/*--------------------------END-----------------------------*/



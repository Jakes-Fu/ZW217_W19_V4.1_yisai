/*****************************************************************************
** File Name:      mmicus_export.c                                                 *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe Customer API                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 
******************************************************************************/
#ifdef CSC_SUPPORT

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmicus_func.h"
#ifdef CSC_XML_SUPPORT
#include "mmicus_xmlparser.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_InitModule
* Function:     Init Customer Module
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_InitModule(void)
{
    return MMICUS_InitModule();
}

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_ResetCusSettings
* Function:     Reset Customer settings
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_ResetSettings(void)
{
    return MMICUS_ResetSettings();
}

#ifdef CSC_XML_SUPPORT
/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_ParseCSCXML
* Function:     Parse Customer Specific Customization XML
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_ParseCSCXML(void)
{
    return MMICUS_ParseCSCXML();
}

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultXMLCSCEdition
* Function:     Get the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultXMLCSCEdition(void)
{
    return MMICUS_GetDefaultXMLCSCEdition();
}

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultSalesCode
* Function:     Get the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultSalesCode(void)
{
    return MMICUS_GetDefaultSalesCode();
}

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA from XML file.
* Author:       
* Note:         Maybe return PNULL
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultUAFromCSC(void)
{
    return MMICUS_GetDefaultUAFromCSC();
}

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA Profile from XML file.
* Author:       
* Note:         Maybe return PNULL
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultUAProfileFromCSC(void)
{
    return MMICUS_GetDefaultUAProfileFromCSC();
}

/*******************************************************************/
//  Interface:		MMIAPICUS_GetMatchLang
//  Description : 	Get Match Lang
//  Global resource dependence : 
//  Author: 
//  Note: 
/*******************************************************************/
PUBLIC uint32 MMIAPICUS_GetMatchLang(const char * lang_name)
{
    return MMICUS_GetMatchLang(lang_name);
}
#endif

#endif

/*****************************************************************************
** File Name:      mmicus_export.h                                                *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe Customer API               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
******************************************************************************/
#ifndef _MMICUS_EXPORT_H_
#define _MMICUS_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"

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
#define  XML_TAG_NAME_MAX_LEN     24
#define  XML_TAG_CONTENT_MAX_LEN  24
#define  XML_TAG_MESSAGE_MAX_LEN  128
#define  XML_TAG_UA_MAX_LEN  256
#define  XML_TAG_UA_PROFILE_MAX_LEN  256


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//xml struct
typedef struct generalinfo_networkinfo_tag
{
    uint16 mcc;                                           //mcc
    uint16 mnc;                                           //mnc
    uint8  network_name[XML_TAG_NAME_MAX_LEN + 1];
    struct generalinfo_networkinfo_tag *next; 
}MMICUS_CSC_GENERALINFO_NETWORKINFO_T; 

//General
typedef struct
{
    uint8  csc_edition[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8  country[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8  sales_code[XML_TAG_CONTENT_MAX_LEN + 1];
    uint16 nb_network_info;
    MMICUS_CSC_GENERALINFO_NETWORKINFO_T *general_networkinfo;        //提醒:使用结束这个结构体要释放资源
}MMICUS_CSC_GENERALINFO_T;

//Sos
typedef struct
{
    uint16 nb_sos_num;
    uint16 nb_sos_num_nosim;
    uint8 sos_number[XML_TAG_MESSAGE_MAX_LEN + 1];
    uint8 sos_number_nosim[XML_TAG_MESSAGE_MAX_LEN + 1]; 
}MMICUS_CSC_SETTING_MAIN_SOS_NUM_T;

//Network
typedef struct
{
    MMICUS_CSC_SETTING_MAIN_SOS_NUM_T sos_num;
}MMICUS_CSC_SETTING_MAIN_NETWORK_T;

typedef struct MMICUS_CSC_SETTING_msg_voicemail_tag
{
    uint8 voicemail_name[XML_TAG_CONTENT_MAX_LEN + 1];
    uint8 voicemail_telnum[XML_TAG_CONTENT_MAX_LEN + 1];
    struct MMICUS_CSC_SETTING_msg_voicemail_tag *next;
}MMICUS_CSC_SETTING_MESSAGE_VOICEMAIL_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_InitModule
* Function:     Init Customer Module
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_InitModule(void);

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_ResetSettings
* Function:     Reset Customer settings
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_ResetSettings(void);

#ifdef CSC_XML_SUPPORT
/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_ParseCSCXML
* Function:     Parse Customer Specific Customization XML
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC BOOLEAN MMIAPICUS_ParseCSCXML(void);

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultXMLCSCEdition
* Function:     Get the CSC edition value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultXMLCSCEdition(void);

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultSalesCode
* Function:     Get the SalesCode value from XML file.
* Author:       
* Note:         
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultSalesCode(void);

/* ----------------------------------------------------------------------------
* Name:         MMIAPICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA from XML file.
* Author:       
* Note:         Maybe return PNULL
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultUAFromCSC(void);

/* ----------------------------------------------------------------------------
* Name:         MMICUS_GetDefaultUAFromCSC
* Function:     Get the Defaule UA Profile from XML file.
* Author:       
* Note:         Maybe return PNULL
* ----------------------------------------------------------------------------*/ 
PUBLIC uint8* MMIAPICUS_GetDefaultUAProfileFromCSC(void);

/*******************************************************************/
//  Interface:		MMIAPICUS_GetMatchLang
//  Description : 	Get Match Lang
//  Global resource dependence : 
//  Author: 
//  Note: 
/*******************************************************************/
PUBLIC uint32 MMIAPICUS_GetMatchLang(const char * lang_name);
#endif

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif



/******************************************************************************
** File Name:       config_file.h                                                   
** Author:          Yifeng.Wang                              
** Date:            2007/12/10                                               
** Copyright:       2007 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     This file defines configure file read & write interface
** Note:            Amend from INIFileOP.C v1.0 of Yuewang.Liao      
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2007/12/10       Yifeng.Wang         Create
******************************************************************************/

#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_CFG_BUF                     512 
#define CFG_OK                          0 
#define CFG_SECTION_NOT_FOUND           -1 
#define CFG_KEY_NOT_FOUND               -2 
#define CFG_ERR                         -10 
#define CFG_ERR_FILE                    -10 
#define CFG_ERR_OPEN_FILE               -10 
#define CFG_ERR_CREATE_FILE             -11 
#define CFG_ERR_READ_FILE               -12 
#define CFG_ERR_WRITE_FILE              -13 
#define CFG_ERR_FILE_FORMAT             -14 
#define CFG_ERR_SYSTEM                  -20 
#define CFG_ERR_SYSTEM_CALL             -20 
#define CFG_ERR_INTERNAL                -21 
#define CFG_ERR_EXCEED_BUF_SIZE         -22 
#define COPYF_OK                        0 
#define COPYF_ERR_OPEN_FILE             -10 
#define COPYF_ERR_CREATE_FILE           -11 
#define COPYF_ERR_READ_FILE             -12 
#define COPYF_ERR_WRITE_FILE            -13 
#define TXTF_OK                         0 
#define TXTF_ERR_OPEN_FILE              -1 
#define TXTF_ERR_READ_FILE              -2 
#define TXTF_ERR_WRITE_FILE             -3 
#define TXTF_ERR_DELETE_FILE            -4 
#define TXTF_ERR_NOT_FOUND              -5



/*---------------------------------------------------------------------------*
/*                          FUNCTION DECLARATIONS                            *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: get a key value
// Global resource:
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/
PUBLIC int CONFIGFILE_GetKeyValue( char* file_path, void *section, void *key, void *buf );

/*****************************************************************************/
// Description: set a key value
// Global resource: 
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/ 
PUBLIC int CONFIGFILE_SetKeyValue( char* file_path, void *section, void *key, void *buf ); 

/*****************************************************************************/
// Description: check if section name in file 
// Global resource: none
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/
PUBLIC int CONFIGFILE_IsSectionExist( char* file_path, void *section );


///////////////////////////////////////////////////////////////////////////////
#endif /* _CONFIG_FILE_H */
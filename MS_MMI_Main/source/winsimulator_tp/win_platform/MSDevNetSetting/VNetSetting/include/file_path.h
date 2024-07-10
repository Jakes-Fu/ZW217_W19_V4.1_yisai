/*****************************************************************************
** File Name:       file_path.h
** Author:          Yifeng.Wang
** Date:            2008/02/22
** Copyright:       2008 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     This file defines file and path operation interfaces.
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2008/02/22       Yifeng.Wang         Create
** 2010/08/23       Yifeng.Wang         Modify
******************************************************************************/

#ifndef _FILE_PATH_H_
#define _FILE_PATH_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include <windef.h>
#include "common.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_PATH_LENGTH     MAX_PATH


/**--------------------------------------------------------------------------*
 **                         DECLARATION                                      *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTIONS                                        *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : check if the file or path is existed
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
PUBLIC BOOL FILEPATH_IsExisted( CString strPathName );

/*****************************************************************************/
// Description : format Path (e.g. non-root path end with '\')
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
PUBLIC void FILEPATH_FormatPath( CString& strPathName );

/*****************************************************************************/
// Description : get parent path (e.g. non-root path end with '\')
// Global resource : none
// Author : yifeng.wang
// Notes : 
/*****************************************************************************/
PUBLIC BOOL FILEPATH_ParentPath( CString& strPathName );

/*****************************************************************************/
// Description : get current app path 
// Global resource : none
// Author : yifeng.wang
// Notes :
/*****************************************************************************/
PUBLIC BOOL FILEPATH_GetCurrentAppPath( CString& strCfgPath );


///////////////////////////////////////////////////////////////////////////////
#endif  /* _FILE_PATH_H_ */
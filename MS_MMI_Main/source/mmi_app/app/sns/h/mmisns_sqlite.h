/*****************************************************************************
** File Name:      mmisns_sqlite.h                                         *
** Author:                                                                   *
** Date:             2012/01/05                                             *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012       CBK          Create                                 *
******************************************************************************/
#ifdef SNS_SUPPORT
#ifndef _MMISNS_SQLITE_H_
#define _MMISNS_SQLITE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_time.h"
#include "mmifmm_export.h"


/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#ifdef  SNS_SUPPORT
typedef void*   MMISNSSQL;


typedef enum
{
    MMISNS_ACCOUNT_SINA,
    MMISNS_ACCOUNT_FACEBOOK,
    MMISNS_ACCOUNT_TWITER,
    MMISNS_ACCOUNT_MAX
}MMISNS_ACCOUNT_TYPE_E;

typedef struct  
{
    char*       user_id;  //SNS 帐号ID
    char*       file_path; // 大头贴路径
    char*       name; //好友名
    char*       email; //Email 地址
    char*       birth; //好友生日
    char*       sign; //好友签名
    char*       type; // 类型(SINA,TWITTER, FACEBOOK...)
}MMISNS_SQLITE_DATA_T;
    


typedef struct
{
    MMISNS_SQLITE_DATA_T* search_result;
    uint32 nRow;
    uint32 uiNum;      //查询起止
    uint32 uiOffset;    // 查询数目
    char* SearchName_ptr; //通过姓名查询时使用
}MMISNS_SQLITE_RESULT;

/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Transaction process
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMISNS_Sqlite_Transaction(
                                         MMISNSSQL v_array, 
                                         BOOLEAN begin
                                         );
/*****************************************************************************/
//  Description : Search result
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
#if 0
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByname(
										   MMISNSSQL virtual_array,
										   char *data_ptr
										   );
#endif
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByID(
										   MMISNSSQL virtual_array
										   );
/*****************************************************************************/
//  Description : Add Item into SQLite Database
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_AddItem(
									   MMISNSSQL virtual_array,
									   void *data_ptr
									   );
									   
/*****************************************************************************/
//  Description : Create SQLite handle
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISNS_Sqlite_Create(void);
/*****************************************************************************/
//  Description : Destroy SQLite handle and result pointer
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMISNS_Sqlite_Destroy(void);
/*****************************************************************************/
//  Description : reset all user parameter related to DB
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_ResetUserPara(
                                  MMISNSSQL *virtual_ptr
                                  );

/*****************************************************************************/
//  Description : Search result
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetNumByType(
                                           MMISNSSQL virtual_array,
                                           MMISNSSQL etype
										   
										   );
/*****************************************************************************/
//  Description : Search result by name
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByName(
										   MMISNSSQL virtual_array
										   );
/*****************************************************************************/
//  Description : get  size
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_GetTotalNum(
                                          MMISNSSQL virtual_array
										   );			
/*****************************************************************************/
//  Description : MMISMS_Sqlite_GetItemByType
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByType(
                                          MMISNSSQL virtual_array,
                                          MMISNSSQL etype
										   );
/*****************************************************************************/
//  Description : if database is open
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_isDBOpen(void);
/*****************************************************************************/
//  Description : clean all data in database
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_CleanAll(void);

PUBLIC void MMISNS_SQLite_prepData(void);

PUBLIC void MMISNS_Sqlite_BuildMutex(void);
#endif

#endif
#endif

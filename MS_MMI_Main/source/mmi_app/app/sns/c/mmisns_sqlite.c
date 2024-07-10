/*****************************************************************************
** File Name:      mmisns_sqlite.c                                         *
** Author:                                                                   *
** Date:             05/01/2012                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2012      CBK         Create                                 *
******************************************************************************/

#include "mmi_app_sns_trc.h"
#ifdef  SNS_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmifilearray_export.h"

#include "sfs.h"
#include "os_api.h"
#include "string.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "gui_ucs2b_converter.h"
/*lint -e766*/
#include "fake_api.h" 
/*lint -e766*/
#include <stdio.h>
//extern "C"
//{
#include "sqlite3.h"
//};

#include "mmisns_sqlite.h"
/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define MMISNS_FILENAME_LEN  100
#if 0
#define     MMISNSSQLITE_MAX_SIZE_MEMMORY_MODE      100			// 记录个数 	
#endif
#define     MMISNSSQLITE_MAX_SIZE_FILE_MODE         3000		// 记录个数

#define     MMISNSSQLITE_MIN_DISKSPACE              (100*1024)	// 100k

#define     SQL_MAX_LEN                             255
#define	  DEFAULT_STRING							""

#ifndef MAX
# define MAX(x,y) ((x) < (y) ? (y) : (x))
#endif
#ifndef MIN
# define MIN(x,y) ((x) > (y) ? (y) : (x))
#endif

#define     MMISNSSQLITE_WARNNING                   SCI_TRACE_LOW

#if 1
#define		MMISNSSQLITE_LOG						SCI_TRACE_LOW		
#else
#define		MMISNSSQLITE_LOG
#endif

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

typedef sqlite3 * HSQL;


//typedef enum
//{
   // MMISNS_ACCOUNT_SINA,
    //MMISNS_ACCOUNT_FACEBOOK,
    //MMISNS_ACCOUNT_TWITER,
   // MMISNS_ACCOUNT_MAX
//}MMISNS_ACCOUNT_TYPE_E;

//LOCAL HSQL s_hsql = PNULL;
 typedef struct
{
    HSQL hsql;    // sql handle
    int32 sql_result;  // sql执行结果状态
    char** dbResult; // sql执行结果
    uint32 nColumn;       // sql结果列数
    uint32 nRow;            // sql结果条数
}MMISNS_SQLITE_HANDLE;

LOCAL MMISNS_SQLITE_HANDLE s_handle = {0};
//mutex
LOCAL    SCI_MUTEX_PTR  s_sns_sqlite_mutex = PNULL;
/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
////LOCAL BOOLEAN CoverDateTimeToString(char* str, SCI_DATE_T* date, SCI_TIME_T* time);
////LOCAL void sqlite_memory_status(void);
/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
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
                                         )
{
    char * Trans_ptr = PNULL;    
    
    if (PNULL == s_handle.hsql)
    {
        //MMISNSSQLITE_WARNNING:"MMIVIRTUALARRAY_Transaction, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_110_112_2_18_2_55_42_0,(uint8*)"");
        return FALSE;
    }    
    if (begin)
    {
        Trans_ptr = "begin transaction;";
    }
    else
    {
        Trans_ptr = "commit transaction;";
    }    
    return (SQLITE_OK == sqlite3_exec(s_handle.hsql, Trans_ptr, 0, 0, 0));
}

/*****************************************************************************/
//  Description : Search result
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByID(
										   MMISNSSQL virtual_array
										   )
{
    char **dbResult = PNULL;
    MMISNS_SQLITE_RESULT* search_handle =  (MMISNS_SQLITE_RESULT *)virtual_array;
    uint32 uiResultNum = 0;
    uint32 uiIndex = 0;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    int nRow = 0;
    int nColumn = 0;
    int nresult = 0;
    char *temp_query_ptr = "select user_id, name, email_addr, type, birthday, sign, file_path from SNS_CONTACT limit %d offset %d";

    if( (PNULL == search_handle) || (PNULL == s_handle.hsql) )
    {
        //SCI_TRACE_LOW:"MMISMS_Sqlite_GetItemByID  search_handle->uiNum !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_144_112_2_18_2_55_42_1,(uint8*)"");
        return FALSE;
    }
    
    if(!search_handle->uiNum)
    {
        //SCI_TRACE_LOW:"MMISMS_Sqlite_GetItemByID  search_handle->uiNum = 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_150_112_2_18_2_55_42_2,(uint8*)"");
        return FALSE;
    }        
    
    if(PNULL != search_handle->search_result)
    {
        SCI_FREE(search_handle->search_result);
    }

    _snprintf(sql_sentence, SQL_MAX_LEN, temp_query_ptr,  search_handle->uiNum, search_handle->uiOffset);	
    //MMISNSSQLITE_LOG:"PackQuerySqlSentence: %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_160_112_2_18_2_55_42_3,(uint8*)"s", sql_sentence);

    if (NULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = NULL;
    }
    nresult = sqlite3_get_table(s_handle.hsql, sql_sentence, &(dbResult), &(nRow), &(nColumn), 0);        

    s_handle.nRow = nRow;
    s_handle.nColumn = nColumn;
    s_handle.sql_result = nresult;
    s_handle.dbResult = dbResult;
    if ((SQLITE_OK != s_handle.sql_result) ||(!s_handle.nRow))
    {
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetItemByID  s_handle.sql_result error or result empty!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_175_112_2_18_2_55_42_4,(uint8*)"");
        search_handle->nRow = s_handle.nRow;
        return FALSE;
    }          

    //prepare result 
    search_handle->search_result = (MMISNS_SQLITE_DATA_T *) SCI_ALLOCA(sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);

    if(PNULL == search_handle->search_result)
    {
        return FALSE;
    }

    SCI_MEMSET(search_handle->search_result, 0, sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);
    
    search_handle->nRow = s_handle.nRow;
    uiIndex = s_handle.nColumn;   
    for(uiResultNum =0; uiResultNum < search_handle->nRow; uiResultNum++)
    {
        search_handle->search_result[uiResultNum].user_id = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].name = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].email = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].type = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].birth = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].sign = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].file_path = s_handle.dbResult[uiIndex++];        
    }

    return TRUE;
}
#if 0
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByname(
										   MMISNSSQL virtual_array,
										   char *data_ptr
										   )
{
    BOOLEAN bret = FALSE;
    char **dbResult = PNULL;
    MMISNS_SQLITE_HANDLE *handle = (MMISNS_SQLITE_HANDLE *)virtual_array;    
    char *pErrMsg = PNULL;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    int32 sql_result = 0;
    
    if (PNULL == data_ptr)
    {
        //MMISNSSQLITE_WARNNING:"MMIVIRTUALARRAY_GetItemById, data_ptr = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_212_112_2_18_2_55_43_5,(uint8*)"");
        return FALSE;
    }    

    if (PNULL != sql_sentence && 0 != SQL_MAX_LEN)
    {
        //char *temp_query_ptr = "select name, date_time, size, type from FILE_ARRAY %s limit %d offset %d";	
        char *temp_query_ptr = "select name, email_addr, type, birthday, sign from SNS_CONTACT where name=%s";
        _snprintf(sql_sentence, SQL_MAX_LEN, temp_query_ptr,  data_ptr);	
        //MMISNSSQLITE_LOG:"PackQuerySqlSentence: %s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_221_112_2_18_2_55_43_6,(uint8*)"s", sql_sentence);
        
        if (PNULL != handle->dbResult)
        {
            sqlite3_free_table(handle->dbResult);
            handle->dbResult = PNULL;
        }
        
        handle->sql_result = sqlite3_get_table(s_hsql, sql_sentence, \
            &(handle->dbResult), &(handle->nRow), &(handle->nColumn), 0);
    }
    
    if (SQLITE_OK != sql_result)
    {
#ifdef SQLITE_DEBUG
        char szErr[256] = {0};
        _snprintf(szErr, sizeof(szErr)-1, "err:%s,%s.",sql_sentence, pErrMsg);
        write_mem_log(szErr);
        if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif			
        //MMISNSSQLITE_WARNNING:"MMIVIRTUALARRAY_GetItemById, error:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_241_112_2_18_2_55_43_7,(uint8*)"s", pErrMsg);
        return FALSE;
    }  

    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : Add Item into SQLite Database
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_AddItem(
									   MMISNSSQL virtual_array,
									   void *data_ptr
									   )
{
    MMISNS_SQLITE_DATA_T *filedate_ptr = (MMISNS_SQLITE_DATA_T *)data_ptr;
    char sql_sentence[SQL_MAX_LEN*2 + 1] = {0};
    uint16 totalNum = 0;
    char *temp_add_ptr = "insert into SNS_CONTACT(user_id, name, email_addr, type, birthday, sign, file_path ) values('%s', '%s', '%s', '%s', '%s', '%s', '%s')";
    
    if ((PNULL == filedate_ptr) || (PNULL == s_handle.hsql) )
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_AddItem, data_ptr = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_265_112_2_18_2_55_43_8,(uint8*)"");
        return FALSE;
    }    

    if (PNULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = PNULL;
    }
 
    MMISNS_Sqlite_GetTotalNum(&totalNum);
    
    if(totalNum > MMISNSSQLITE_MAX_SIZE_FILE_MODE)
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_AddItem, num > MAX !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_273_112_2_18_2_55_43_9,(uint8*)"");
        return FALSE;
    }    

    _snprintf(sql_sentence, SQL_MAX_LEN*2, temp_add_ptr, filedate_ptr->user_id, filedate_ptr->name, filedate_ptr->email, filedate_ptr->type, \
        filedate_ptr->birth, filedate_ptr->sign, filedate_ptr->file_path);		
    //MMISNSSQLITE_LOG:"PackAddSqlSentence: %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_286_112_2_18_2_55_43_10,(uint8*)"s", sql_sentence);

    if(PNULL == s_handle.hsql)
    {
        return FALSE;
    }
    
    s_handle.sql_result = sqlite3_exec(s_handle.hsql, sql_sentence, NULL, NULL, NULL);
    if (SQLITE_OK != s_handle.sql_result)
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_AddItem, error:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_291_112_2_18_2_55_43_11,(uint8*)"d", s_handle.sql_result);
        return FALSE;
    }	
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Create SQLite handle
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMISNS_Sqlite_Create(void)
{
    int32     iret = -1;
    char cDB_file_ptr[MMIFILE_FULL_PATH_MAX_LEN+MMISNS_FILENAME_LEN+1] = {0};    
    wchar* file_name = L"E:\\sns\\snsContact.db";
    BOOLEAN isCreated = FALSE;
    //int iDefaultLen = 0;

    if(PNULL != s_handle.hsql)
    {
        return FALSE;
    }
    
    MMIAPICOM_WstrToStr(file_name, (uint8*)cDB_file_ptr);

    if(MMIFILE_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)) )
    {
        isCreated = TRUE;
    }

    //if(PNULL == &s_handle)
    //{
       // return FALSE;
    //}
    
    iret = sqlite3_open(cDB_file_ptr, &s_handle.hsql); 
    if(isCreated)
    {
        if (SQLITE_OK == iret)
        {
            char *sql_journal_mode_ptr = "PRAGMA journal_mode = OFF";
            if(PNULL == s_handle.hsql)
            {
                return FALSE;
            }
            s_handle.sql_result = sqlite3_exec(s_handle.hsql, sql_journal_mode_ptr, 0, 0, 0);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
        
    }
    if (SQLITE_OK == iret)
    {

        char *sql_journal_mode_ptr = "PRAGMA journal_mode = OFF";
        char *sql_create_table_ptr = "create TABLE SNS_CONTACT(ID integer not null primary key, user_id text, name text, email_addr text, type text, birthday text, sign text, file_path text)";
        char *sql_create_index_ptr = "create INDEX id_index on SNS_CONTACT (ID)"; 

        if(PNULL == s_handle.hsql)
        {
            return FALSE;
        }
        s_handle.sql_result = sqlite3_exec(s_handle.hsql, sql_create_table_ptr, 0, 0, 0);
        if (SQLITE_OK != s_handle.sql_result)
        {
            //MMISNSSQLITE_WARNNING:"CreateTable, create talbe fail.%d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_347_112_2_18_2_55_43_12,(uint8*)"dd", s_handle.sql_result, &(s_handle.sql_result));
            MMISNS_Sqlite_Destroy();
            return FALSE;
        }
        else
        {
            if(PNULL == s_handle.hsql)
            {
                return FALSE;
            }
            s_handle.sql_result = sqlite3_exec(s_handle.hsql, sql_journal_mode_ptr, 0, 0, 0);
            s_handle.sql_result = sqlite3_exec(s_handle.hsql, sql_create_index_ptr, 0, 0, 0);
            
            if (SQLITE_OK != s_handle.sql_result)
            {
                //MMISNSSQLITE_WARNNING:"CreateTable, create index fail..%d,%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_358_112_2_18_2_55_43_13,(uint8*)"dd", s_handle.sql_result, &(s_handle.sql_result));
                MMISNS_Sqlite_Destroy();
                return FALSE;
            }
            isCreated = TRUE;
        }        
    }
    else
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_Create, error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_367_112_2_18_2_55_43_14,(uint8*)"");
        return FALSE;
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : Destroy SQLite handle and result pointer
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
  PUBLIC void MMISNS_Sqlite_Destroy(void)
{       
     if (PNULL == s_handle.hsql)
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_Destroy, handle or search_handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_383_112_2_18_2_55_43_15,(uint8*)"");
    }

    if(PNULL == s_sns_sqlite_mutex)
    {
        SCI_TRACE_LOW("MMISNS_Sqlite_Destroy() PNULL == s_sns_sqlite_mutex");
        return ;
    }
    
    SCI_GetMutex(s_sns_sqlite_mutex, SCI_WAIT_FOREVER);
        
    if (PNULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = PNULL;
    }
    
    if (PNULL != s_handle.hsql)
    {
        sqlite3_close(s_handle.hsql);
        s_handle.hsql = PNULL;
    }    
    
    SCI_PutMutex(s_sns_sqlite_mutex);
}

/*****************************************************************************/
//  Description : reset all user parameter related to DB
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_ResetUserPara(
                                  MMISNSSQL *virtual_ptr
                                  )
{
    MMISNS_SQLITE_RESULT* search_handle = PNULL;

    if((PNULL == virtual_ptr))
    {
        return FALSE;
    }
    
    search_handle =  (MMISNS_SQLITE_RESULT *)virtual_ptr;

    if(PNULL != search_handle->search_result)
    {
        SCI_FREE(search_handle->search_result);
    }
    
    search_handle->nRow     = 0;
    search_handle->uiNum    = 0;
    search_handle->uiOffset = 0;  

    return TRUE;
}    

/*****************************************************************************/
//  Description : Search result
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetNumByType(
                                          MMISNSSQL virtual_array,
                                          MMISNSSQL etype
										   )
{
    uint16* returnNum =  (uint16 *)virtual_array;
    MMISNS_ACCOUNT_TYPE_E* type = (MMISNS_ACCOUNT_TYPE_E*)etype;
    char **dbResult = PNULL;
    int nRow = 0;
    int nColumn = 0;
    int nresult = 0;   
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    char *temp_query_ptr = "select ID from SNS_CONTACT where type = %d";

    if ((PNULL == s_handle.hsql) || (PNULL == returnNum))
    {
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetNumByType, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_448_112_2_18_2_55_43_16,(uint8*)"");
        return 0;
    }

    *returnNum = 0;
	
    _snprintf(sql_sentence, SQL_MAX_LEN, temp_query_ptr,  *type);	
    //MMISNSSQLITE_LOG:"MMISMS_Sqlite_GetNumByType PackQuerySqlSentence: %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_455_112_2_18_2_55_43_17,(uint8*)"s", sql_sentence);
    
    if (PNULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = PNULL;
    }        

    nresult = sqlite3_get_table(s_handle.hsql, sql_sentence, &(dbResult), &(nRow), &(nColumn), 0);  

    s_handle.nRow = nRow;
    s_handle.nColumn = nColumn;
    s_handle.sql_result = nresult;
    s_handle.dbResult = dbResult;  
        
    *returnNum = s_handle.nRow;
    
    if (SQLITE_OK != s_handle.sql_result)
    {
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetNumByType, error:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_474_112_2_18_2_55_43_18,(uint8*)"d", s_handle.sql_result);
        return FALSE;
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : Search result by name
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByName(
										   MMISNSSQL virtual_array
										   )
{
    char **dbResult = PNULL;
    MMISNS_SQLITE_RESULT* search_handle =  (MMISNS_SQLITE_RESULT *)virtual_array;
    uint32 uiResultNum = 0;
    uint32 uiIndex = 0;   
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    int nRow = 0;
    int nColumn = 0;
    int nresult = 0;
    char *temp_query_ptr = "select user_id, name, email_addr, type, birthday, sign, file_path from SNS_CONTACT where name = '%s'";
    
    if ((PNULL == search_handle) ||(PNULL == s_handle.hsql))
    {
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetItemByName   !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_502_112_2_18_2_55_43_19,(uint8*)"");
        return FALSE;
    }      

    if(PNULL != search_handle->search_result)
    {
        SCI_FREE(search_handle->search_result);
    }
    
    _snprintf(sql_sentence, SQL_MAX_LEN, temp_query_ptr,  search_handle->SearchName_ptr);	
    //MMISNSSQLITE_LOG:"PackQuerySqlSentence: %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_512_112_2_18_2_55_43_20,(uint8*)"s", sql_sentence);

    if (NULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = NULL;
    }

    nresult = sqlite3_get_table(s_handle.hsql, sql_sentence, &(dbResult), &(nRow), &(nColumn), 0);        

    s_handle.nRow = nRow;
    s_handle.nColumn = nColumn;
    s_handle.sql_result = nresult;
    s_handle.dbResult = dbResult;
    
    if ((SQLITE_OK != s_handle.sql_result) ||(!s_handle.nRow))
    {
        search_handle->nRow = s_handle.nRow;
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetItemByName  s_handle.sql_result error or result empty %d!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_530_112_2_18_2_55_43_21,(uint8*)"d", s_handle.nRow);
        return FALSE;
    }          

    //prepare result
    search_handle->search_result = (MMISNS_SQLITE_DATA_T *) SCI_ALLOCA(sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);

    if(PNULL == search_handle->search_result)
    {
        return FALSE;
    }

    SCI_MEMSET(search_handle->search_result, 0, sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);
    
    search_handle->nRow = s_handle.nRow;
    uiIndex = s_handle.nColumn;
    for(uiResultNum =0; uiResultNum < search_handle->nRow; uiResultNum++)
    {
        search_handle->search_result[uiResultNum].user_id = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].name = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].email = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].type = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].birth = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].sign = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].file_path = s_handle.dbResult[uiIndex++];        
    }  


    return TRUE;
}

/*****************************************************************************/
//  Description : get  size
//  Global resource dependence : 
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_GetTotalNum(
                                          MMISNSSQL virtual_array
										   )
{
    char **dbResult = PNULL;
    int nRow = 0, nColumn = 0;
    uint16* returnNum =  (uint16 *)virtual_array;
    char *sql_count_ptr = "select count(*) from SNS_CONTACT";    

    if ((PNULL == s_handle.hsql) || (PNULL == returnNum))
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_GetTotalNum, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_568_112_2_18_2_55_43_22,(uint8*)"");
        return FALSE;
    }

    *returnNum = 0;
    
    if (SQLITE_OK == sqlite3_get_table(s_handle.hsql, sql_count_ptr, \
        &dbResult, &nRow, &nColumn, 0))
    {
        if (nRow > 0)
        {
            *returnNum = atoi(dbResult[nColumn]);
        }
        sqlite3_free_table(dbResult);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : MMISMS_Sqlite_GetItemByType
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_Sqlite_GetItemByType(
                                          MMISNSSQL virtual_array,
                                          MMISNSSQL etype
										   )
{
    //uint16* returnNum =  (uint16 *)virtual_array;
    MMISNS_SQLITE_RESULT* search_handle =  (MMISNS_SQLITE_RESULT *)virtual_array;
    MMISNS_ACCOUNT_TYPE_E* type = (MMISNS_ACCOUNT_TYPE_E*)etype;
    char **dbResult = PNULL;  
    uint32 uiResultNum = 0;
    uint32 uiIndex = 0;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    int nRow = 0;
    int nColumn = 0;
    int nresult = 0; 
    char *temp_query_ptr = "select user_id, name, email_addr, type, birthday, sign, file_path from SNS_CONTACT where type = %d";

    if ( (PNULL == search_handle) ||(PNULL == s_handle.hsql) )
    {
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetItemByType, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_610_112_2_18_2_55_43_23,(uint8*)"");
        return 0;
    }

    if(PNULL != search_handle->search_result)
    {
        SCI_FREE(search_handle->search_result);
    }
        
    _snprintf(sql_sentence, SQL_MAX_LEN, temp_query_ptr,  *type);	
    //MMISNSSQLITE_LOG:"MMISMS_Sqlite_GetItemByType PackQuerySqlSentence: %s."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_620_112_2_18_2_55_43_24,(uint8*)"s", sql_sentence);
    
    if (PNULL != s_handle.dbResult)
    {
        sqlite3_free_table(s_handle.dbResult);
        s_handle.dbResult = PNULL;
    }        

    nresult = sqlite3_get_table(s_handle.hsql, sql_sentence, &(dbResult), &(nRow), &(nColumn), 0);

    s_handle.nRow = nRow;
    s_handle.nColumn = nColumn;
    s_handle.sql_result = nresult;
    s_handle.dbResult = dbResult;   
    
    if (SQLITE_OK != s_handle.sql_result)
    {
        search_handle->nRow = s_handle.nRow;
        //MMISNSSQLITE_WARNNING:"MMISMS_Sqlite_GetItemByType, error:%d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_638_112_2_18_2_55_43_25,(uint8*)"d", s_handle.sql_result);
        return FALSE;
    }  

    //prepare result 
    search_handle->search_result = (MMISNS_SQLITE_DATA_T *) SCI_ALLOCA(sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);

    if(PNULL == search_handle->search_result)
    {
        return FALSE;
    }

    SCI_MEMSET(search_handle->search_result, 0, sizeof(MMISNS_SQLITE_DATA_T)*s_handle.nRow);


    search_handle->nRow = s_handle.nRow;
    uiIndex = s_handle.nColumn;   
    for(uiResultNum =0; uiResultNum < search_handle->nRow; uiResultNum++)
    {
        search_handle->search_result[uiResultNum].user_id = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].name = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].email = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].type = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].birth = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].sign = s_handle.dbResult[uiIndex++];
        search_handle->search_result[uiResultNum].file_path = s_handle.dbResult[uiIndex++];        
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : if database is open
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_isDBOpen(void)
{
    if (PNULL == s_handle.hsql)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : clean all data in database
//  Global resource dependence : 
//  Author:  CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISNS_Sqlite_CleanAll(void)
{    
    //int32     iret = -1;
    char cDB_file_ptr[MMIFILE_FULL_PATH_MAX_LEN+MMISNS_FILENAME_LEN+1] = {0};
    //const wchar* default_dir = PNULL;
    //uint16 file_name_len = 0;
    wchar* file_name = L"E:\\sns\\snsContact.db";
    //int iDefaultLen = 0;

    if(MMISNS_Sqlite_isDBOpen())
    {
        //MMISNSSQLITE_WARNNING:"MMISNS_Sqlite_CleanAll, DB is open , cannot clean all!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISNS_SQLITE_691_112_2_18_2_55_43_26,(uint8*)"");
        return FALSE;
    }

    MMIAPICOM_WstrToStr(file_name, (uint8*)cDB_file_ptr);

    if(MMIAPIFMM_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)) )
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(file_name, NULL))
        {
            return FALSE;
        }
    }

    MMISNS_Sqlite_Create();    

    MMISNS_Sqlite_Destroy();
    
    return TRUE;
    
}
/*****************************************************************************/
//  Description : clean all data in database
//  Global resource dependence : 
//  Author:  RENWEI
//  Note:
/*****************************************************************************/
PUBLIC void MMISNS_Sqlite_BuildMutex(void)
{
    //mutex
    if(PNULL == s_sns_sqlite_mutex)
    {
        s_sns_sqlite_mutex = SCI_CreateMutex("sns_sqlite_mutex",SCI_NO_INHERIT);
        
        if(PNULL == s_sns_sqlite_mutex)
        {
            SCI_TRACE_LOW("MMISNS_Sqlite_BuildMutex() PNULL == s_sns_sqlite_mutex");
        }
    }
    
    return;
}
#endif  // SNS_SUPPORT

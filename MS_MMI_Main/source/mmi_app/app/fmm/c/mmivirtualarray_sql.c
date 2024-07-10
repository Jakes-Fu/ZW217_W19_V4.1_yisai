/*****************************************************************************
** File Name:      mmivirtualarray.c                                         *
** Author:                                                                   *
** Date:             04/11/2011                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2011       Cheng.Luo          Create                                 *
******************************************************************************/

#include "mmi_app_fmm_trc.h"
#ifdef  SQLITE_FILEARRAY_SUPPORT
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
#include "sqlite3.h"

/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define     VIRTUALARRAY_MAX_SIZE_MEMMORY_MODE      100			// 记录个数 	
#define     VIRTUALARRAY_MAX_SIZE_FILE_MODE         2000		// 记录个数
#define     VIRTUALARRAY_MIN_DISKSPACE              (100*1024)	// 100k

#define     VIRTUALARRAY_MEMORY_DB                  ":memory:"
#define     SQL_MAX_LEN                             255
#define		SQL_DATE_TIME_LEN						31
#define		DEFAULT_STRING							""
#define     SINGLE_QUOT_CHARACTER					'\''
#define		TRANSACTION_MAX_ITEM					50
#define		QUERY_ITEM_COUNT						400

#ifndef MAX
# define MAX(x,y) ((x) < (y) ? (y) : (x))
#endif
#ifndef MIN
# define MIN(x,y) ((x) > (y) ? (y) : (x))
#endif
#define     VIRTUALARRAY_WARNNING                   SCI_TRACE_LOW

#if 0
#define		VIRTUALARRAY_LOG						SCI_TRACE_LOW		
#else
#define		VIRTUALARRAY_LOG
#endif

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef sqlite3 * HSQL;

typedef enum
{
    VIRTUALARRAY_INVALID_MODE,    // 无效
	VIRTUALARRAY_FILE_MODE,       //有关联文件，文件大小有限，数组总数有限制(适用于U盘有一定空间的情况)
	VIRTUALARRAY_MEMORY_MODE      //无关联文件，数组总数有限制，数组完全存放在内存中(适用于U盘空间不足的情况)
}VIRTUALARRAY_MODETYPE_E;

// VIRTUALARRAY_DEFAULT_DIR_U       "\x00\x44\x00\x3A\x00\x5C"
//const wchar g_mmivirtualarray_default_udisk_dir_wstr[]  = { 'D', ':', '\\', 0 };

// VIRTUALARRAY_DEFAULT_DIR_SD      "\x00\x45\x00\x3A\x00\x5C"
//const wchar g_mmivirtualarray_default_sdcard_dir_wstr[] = { 'E', ':', '\\', 0 };

//VIRTUALARRAY的HANDLE结构
typedef struct  
{
    wchar            asso_file_name[MMIFILE_FULL_PATH_MAX_LEN+1];    //关联文件名
    VIRTUALARRAY_MODETYPE_E     mode;								//VIRTUALARRAY的类型
    MMIFILE_ERROR_E  assoc_file_save_ret;							//关联文件保存结果
    MMIFILE_ERROR_E     assoc_file_storage;							//关联文件保存位置
    HSQL				hsql;
    int32			    sql_result;
	FILEARRAY_SORT_E	sort_type;
	BOOLEAN				has_modified;								//if add update delete
	char				**dbResult;									//query结果
	uint32				first_id;									//query first
	uint32				last_id;									//query last
	BOOLEAN             is_need_update;								//是否更新记录数
	uint32				count;										//记录数	
}VIRTUALARRAY_HANDLE_T;

/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN CoverDateTimeToString(char* str, SCI_DATE_T* date, SCI_TIME_T* time);
LOCAL void sqlite_memory_status(void);
/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 根据剩余空间选择存储器
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetFilePathBySapce(
								 VIRTUALARRAY_HANDLE_T *handle
								 )
{
    uint32      high_word = 0;
    uint32      low_word = 0;
	MMIFILE_DEVICE_E device = MMI_DEVICE_SDCARD;
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"GetFilePathBySapce, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_114_112_2_18_2_21_13_639,(uint8*)"");
        return FALSE;
    }
    
	device = MMIAPIFMM_GetMaxFreeDeviceType();
	SFS_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device), &high_word, &low_word);

    if (0 == high_word && VIRTUALARRAY_MIN_DISKSPACE >= low_word)
    {
        VIRTUALARRAY_LOG("GetFilePathBySapce, memory mode.");
        
        handle->assoc_file_save_ret = SFS_ERROR_NO_SPACE;
        handle->mode = VIRTUALARRAY_MEMORY_MODE;
		handle->assoc_file_storage = MMI_DEVICE_NUM;
    }
    else
    {
        VIRTUALARRAY_LOG("GetFilePathBySapce, file mode.");
        handle->assoc_file_storage = device;
        handle->assoc_file_save_ret = SFS_ERROR_NONE;
        handle->mode = VIRTUALARRAY_FILE_MODE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : CreateAssoFile
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateAssoFile(
									 VIRTUALARRAY_HANDLE_T *handle
									 )
{
    MMIFILE_ERROR_E  ret = SFS_ERROR_NONE;
    const wchar      *default_dir = NULL;
    uint16           asso_name_len = 0;
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"CreateAssoFile, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_154_112_2_18_2_21_13_640,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }
    
    if (SFS_ERROR_NO_SPACE == handle->assoc_file_save_ret)
    {
        return SFS_ERROR_NO_ENOUGH_RESOURCE;
    }
	if(handle->assoc_file_storage < MMI_DEVICE_NUM)
	{
		default_dir = MMIAPIFMM_GetDevicePath(handle->assoc_file_storage);
	}
    else
	{
		return SFS_ERROR_NO_ENOUGH_RESOURCE;
	}
	
    ret = MMIAPICOM_CreateRandFileAsyn(default_dir, MMIAPICOM_Wstrlen(default_dir), 
        (uint8 *)VIRTUALARRAY_FILE_PREFIX, NULL, handle->asso_file_name, &asso_name_len);
    if(ret == SFS_ERROR_NONE)
    {
        MMIAPIFMM_SetAttr(handle->asso_file_name, asso_name_len, 
            TRUE, FALSE, FALSE, FALSE);
    }
    
    handle->assoc_file_save_ret = ret;
    
    return ret;   
}

/*****************************************************************************/
//  Description : RemoveAssoFile
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E RemoveAssoFile(
									 VIRTUALARRAY_HANDLE_T *handle
									 )
{
    MMIFILE_ERROR_E  ret = SFS_ERROR_NONE;
    
    if (PNULL != handle)
    {
        ret = SFS_DeleteFile(handle->asso_file_name, NULL);
		VIRTUALARRAY_LOG("RemoveAssoFile, ret = %d",ret);
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : 创建FILE_ARRAY表和索引
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL int32 CreateTable(
						VIRTUALARRAY_HANDLE_T *handle
						)
{
	char *sql_journal_mode_ptr = "PRAGMA journal_mode = OFF";
    char *sql_create_table_ptr = "create TABLE FILE_ARRAY(ID integer not null primary key, name text, date_time text, size integer, type integer)";
    char *sql_create_index_ptr = "create INDEX file_index on FILE_ARRAY (ID)";
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"CreateTable, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_217_112_2_18_2_21_13_641,(uint8*)"");
        return SQLITE_ERROR;
    }
    
    handle->sql_result = sqlite3_exec(handle->hsql, sql_create_table_ptr, 0, 0, 0);
    if (SQLITE_OK != handle->sql_result)
	{
        //VIRTUALARRAY_WARNNING:"CreateTable, create talbe fail."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_224_112_2_18_2_21_13_642,(uint8*)"");
        return SQLITE_ERROR;
	}
	else
	{
		handle->sql_result = sqlite3_exec(handle->hsql, sql_journal_mode_ptr, 0, 0, 0);
		handle->sql_result = sqlite3_exec(handle->hsql, sql_create_index_ptr, 0, 0, 0);
		
		if (SQLITE_OK != handle->sql_result)
		{
			//VIRTUALARRAY_WARNNING:"CreateTable, create index fail."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_234_112_2_18_2_21_13_643,(uint8*)"");
			return SQLITE_ERROR;
		}
		handle->has_modified = FALSE;
		handle->is_need_update = FALSE;
	}
    
    return SQLITE_OK;
}

/*****************************************************************************/
//  Description : get FILE_ARRAY size
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSize(
					 VIRTUALARRAY_HANDLE_T *handle
					 )
{
    char **dbResult = PNULL;
    int nRow = 0, nColumn = 0;
    uint32 array_size = 0;
    char *sql_count_ptr = "select count(*) from FILE_ARRAY";
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"GetSize, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_259_112_2_18_2_21_13_644,(uint8*)"");
        return 0;
    }
    
	if (FALSE == handle->is_need_update)
	{
		return handle->count;
	}
    else if (SQLITE_OK == sqlite3_get_table(handle->hsql, sql_count_ptr, \
        &dbResult, &nRow, &nColumn, 0))
    {
        if (nRow > 0)
        {
            array_size = atoi(dbResult[nColumn]);
        }
        sqlite3_free_table(dbResult);
        handle->count = array_size;
		handle->is_need_update = FALSE;
		
		return array_size;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description : 是否达到FILE_ARRAY上限
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IfReachLimitCount(
								VIRTUALARRAY_HANDLE_T *handle,
								int32 increase_count
								)
{
    BOOLEAN ret = FALSE;
    int32 array_size = 0;
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"IfReachLimitCount, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_298_112_2_18_2_21_13_645,(uint8*)"");
        return TRUE;
    }
    
    array_size = GetSize(handle);
    
    if (VIRTUALARRAY_FILE_MODE == handle->mode)
    {
        ret = (array_size + increase_count > VIRTUALARRAY_MAX_SIZE_FILE_MODE);
    }
    else if (VIRTUALARRAY_MEMORY_MODE == handle->mode)
    {
        ret = (array_size + increase_count > VIRTUALARRAY_MAX_SIZE_MEMMORY_MODE);
    }
    else
    {
        //VIRTUALARRAY_WARNNING:"IfReachLimitCount, invalid mode!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_314_112_2_18_2_21_13_646,(uint8*)"");
        ret = TRUE;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : index是否非法
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValidId(
						VIRTUALARRAY_HANDLE_T *handle,
						uint32 index
						)
{  
	BOOLEAN bret = FALSE;
	
    if (PNULL != handle)
    {
        bret = (GetSize(handle) >= (index + 1));
    }
	
	return bret;
}

/*****************************************************************************/
LOCAL void ResetResult(
					   VIRTUALARRAY_HANDLE_T *handle
					   )
{  
    if (PNULL != handle)
    {
		if (NULL != handle->dbResult)
		{
			sqlite3_free_table(handle->dbResult);
			handle->dbResult = NULL;
		}
		
		handle->has_modified = TRUE;
    }
}

/*******************************************************************/
//  Interface:		CheckNameString
//  Description : 	CheckNameString
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void CheckNameString(
						   char* path_ptr,		/*in&out*/
						   uint32 max_len
						   )
{
	if (NULL != path_ptr)
	{
		uint32 len = strlen(path_ptr);
		char *cur_ptr = (char*)path_ptr + len;
		char *move_tail_ptr = cur_ptr;
		char *move_head_ptr = NULL;
		
		while (cur_ptr-- != path_ptr)
		{
			if (*cur_ptr == SINGLE_QUOT_CHARACTER
				&& ++len != max_len)
			{
				move_tail_ptr++;
				move_head_ptr = move_tail_ptr;
				
				while (--move_head_ptr != cur_ptr)
				{
					*move_head_ptr = *(move_head_ptr - 1);
				}
			}
		}
	}
}
//  Description : 打包插入sql
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void PackAddSqlSentence(
							  char* sql,		/*out*/
							  int32 len, 
							  FILEARRAY_DATA_T *data_ptr
							  )
{
	if (PNULL != sql && 0 != len && PNULL != data_ptr )
	{
		char *temp_add_ptr = "insert into FILE_ARRAY(name, date_time, size, type) values('%s', '%s', %d, %d)";
		uint8 u8_file_name[MMIFMM_FILE_FULL_PATH_MAX_LEN*3 + 1] = {0};
		char u8_date_time[SQL_DATE_TIME_LEN + 1] = {0};
		
		GUI_WstrToUTF8(u8_file_name, (sizeof(u8_file_name) - 1), data_ptr->filename, data_ptr->name_len);
		CheckNameString((char*)u8_file_name, sizeof(u8_file_name) - 1);
		
		CoverDateTimeToString(u8_date_time, &data_ptr->modify_date, &data_ptr->modify_time);
		
		_snprintf(sql, len, temp_add_ptr, u8_file_name, u8_date_time, data_ptr->size, data_ptr->type);		
		VIRTUALARRAY_LOG("PackAddSqlSentence: %s.", sql);
	}
}

/*****************************************************************************/
//  Description : 打包更新sql
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void PackUpdateSqlSentence(
								 char* sql,		/*out*/
								 int32 len,
								 uint32 index,
								 char *sort_ptr,
								 FILEARRAY_DATA_T *data_ptr
								 )
{
	if (PNULL != sql && 0 != len && PNULL != data_ptr )
	{
		char *temp_update_ptr = "update FILE_ARRAY set name = '%s', date_time = '%s', size = %d, type = %d where ID in (select ID from FILE_ARRAY %s limit 1 offset %d)";
		uint8 u8_file_name[MMIFMM_FILE_FULL_PATH_MAX_LEN*3 + 1] = {0};
		char u8_date_time[SQL_DATE_TIME_LEN + 1] = {0};
		char *temp_sort_ptr = sort_ptr;
		if (PNULL == sort_ptr)
		{
			temp_sort_ptr = DEFAULT_STRING;
		}
		
		GUI_WstrToUTF8(u8_file_name, (sizeof(u8_file_name) - 1), data_ptr->filename, data_ptr->name_len);
		CheckNameString((char*)u8_file_name, sizeof(u8_file_name) - 1);
		CoverDateTimeToString(u8_date_time, &data_ptr->modify_date, &data_ptr->modify_time);
		
		_snprintf(sql, len, temp_update_ptr, u8_file_name, u8_date_time, data_ptr->size, data_ptr->type, \
			temp_sort_ptr, index);
		VIRTUALARRAY_LOG("PackUpdateSqlSentence: %s.", sql);
	}
}

/*****************************************************************************/
//  Description : 打包删除sql
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void PackDelSqlSentence(
							  char* sql,		/*out*/
							  int32 len,
							  uint32 index,
							  char* sort_ptr
							  )
{
	if (PNULL != sql && 0 != len)
	{
		char *temp_del_ptr = "delete from FILE_ARRAY where ID in (select ID from FILE_ARRAY %s limit 1 offset %d)";
		char *temp_sort_ptr = sort_ptr;
		if (PNULL == sort_ptr)
		{
			temp_sort_ptr = DEFAULT_STRING;
		}
		
		_snprintf(sql, len, temp_del_ptr, temp_sort_ptr, index);		
		VIRTUALARRAY_LOG("PackDelSqlSentence: %s.", sql);
	}
}

/*****************************************************************************/
//  Description : 打包查询sql
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL void PackQuerySqlSentence(
								char* sql,		/*out*/
								int32 len,
								char* sort_ptr,
								uint32 limit,
								uint32 offset
								)
{
	if (PNULL != sql && 0 != len)
	{
		char *temp_query_ptr = "select name, date_time, size, type from FILE_ARRAY %s limit %d offset %d";
		char *temp_sort_ptr = sort_ptr;
		if (PNULL == sort_ptr)
		{
			temp_sort_ptr = DEFAULT_STRING;
		}
		
		_snprintf(sql, len, temp_query_ptr, temp_sort_ptr, limit, offset);
		VIRTUALARRAY_LOG("PackQuerySqlSentence: %s.", sql);
	}
}

/*****************************************************************************/
//  Description : 打包排序sql
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL char* PackSortSqlSentence(
								FILEARRAY_SORT_E type
								)
{
	char *temp_sort_ptr = PNULL;
	
	switch(type)
	{
	case FILEARRAY_SORT_NAME_ASCEND:
		temp_sort_ptr = "order by name";	
		break;
	case FILEARRAY_SORT_NAME_DESCEND:
		temp_sort_ptr = "order by name desc";
		break;
	case FILEARRAY_SORT_TIME_ASCEND:
		temp_sort_ptr = "order by julianday(date_time)";
		break;
	case FILEARRAY_SORT_TIME_DESCEND:
		temp_sort_ptr = "order by julianday(date_time) desc";
		break;
	case FILEARRAY_SORT_SIZE_ASCEND:
		temp_sort_ptr = "order by size";
		break;
	case FILEARRAY_SORT_SIZE_DESCEND:
		temp_sort_ptr = "order by size desc";
		break;
	case FILEARRAY_SORT_TYPE_ASCEND: //type相反
		temp_sort_ptr = "order by type desc";
		break;
	case FILEARRAY_SORT_TYPE_DESCEND:
		temp_sort_ptr = "order by type";
		break;
	default:
		break;
	}
	
	return temp_sort_ptr;
}

/*****************************************************************************/
//  Description : 字符串转为数据结构
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CoverStringToDateTime(
									char* str,
									SCI_DATE_T* date,
									SCI_TIME_T* time
									)
{
	int32 year, mon, day, hour,min, sec;
	if (PNULL == str || PNULL == date || PNULL == time)
	{
		return FALSE;
	}
	
	year = mon = day = hour = min = sec = 0;
	SCI_MEMSET(date, 0, sizeof(SCI_DATE_T));
	SCI_MEMSET(time, 0, sizeof(SCI_TIME_T));
	
	if (6 == sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d", &year, &mon, &day, &hour, &min, &sec))
	{
		date->year = (uint16)year;
		date->mon =	(uint8)mon;
		date->mday = (uint8)day;
		time->hour = (uint8)hour;
		time->min = (uint8)min;
		time->sec = (uint8)sec;
		
		return TRUE;
	}
	
	return FALSE;
}

/*****************************************************************************/
//  Description : 数据结构转为字符串
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CoverDateTimeToString(
									char* str,
									SCI_DATE_T* date,
									SCI_TIME_T* time
									)
{
	if (PNULL == str || PNULL == date || PNULL == time)
	{
		return FALSE;
	}
	
	sprintf(str, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", date->year, date->mon, date->mday, \
		time->hour, time->min, time->sec);
	
	return TRUE;
}

/*****************************************************************************/
//  Description : 合并两个FILE_ARRAY
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
LOCAL int32 ArrayCombine(
						 HSQL hsql1,
						 HSQL hsql2
						 )
{
	int32 iret = -1;
	char **dbResult = PNULL;
    int nRow = 0, nColumn = 0;
	char *sql_query = "select name, date_time, size, type from FILE_ARRAY";
	
	if (0 == hsql1 || 0 == hsql2)
	{
		return (-1);
	}
	
	iret = sqlite3_get_table(hsql2, sql_query, \
        &dbResult, &nRow, &nColumn, 0);
	
	if (SQLITE_OK == iret)
	{
		int32 i = 0;
		char sql_add[SQL_MAX_LEN+1] = {0};
		char *sql_ptr = "insert into FILE_ARRAY(name, date_time, size, type) values ('%s','%s',%s, %s)";
		
		for (;i < nRow; i++)
		{
			if (0 == (i % TRANSACTION_MAX_ITEM))
			{
				sqlite3_exec(hsql1, "BEGIN;", 0, 0, 0);
			}
			
			_snprintf(sql_add, SQL_MAX_LEN, sql_ptr, dbResult[nColumn*(i + 1)], dbResult[nColumn*(i + 1) + 1],\
				dbResult[nColumn*(i + 1) + 2], dbResult[nColumn*(i + 1) +3]);
			
			iret = sqlite3_exec(hsql1,sql_add , 0, 0, 0);
			if (SQLITE_OK != iret)
			{
				//VIRTUALARRAY_WARNNING:"ArrayCombine, sqlite3_exec insert error!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_639_112_2_18_2_21_14_647,(uint8*)"");
				break;
			}
			
			if ((TRANSACTION_MAX_ITEM - 1) == (i % TRANSACTION_MAX_ITEM)
				|| i == (nRow -1))
			{
				sqlite3_exec(hsql1, "COMMIT;", 0, 0, 0);
			}
		}				
	}
	
    sqlite3_free_table(dbResult);
	
	return iret;
}

//////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
//  Description : VIRTUALARRAY_GetFilePathBySapce
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC VIRTUALARRAY  MMIVIRTUALARRAY_Create()
{
    BOOLEAN bret = FALSE;
    int32     iret = -1;
    VIRTUALARRAY_HANDLE_T *handle = PNULL;
    
    char *db_file_ptr = VIRTUALARRAY_MEMORY_DB;
    char file_path[MMIFILE_FULL_PATH_MAX_LEN * 3 + 1] = {0};
	
    handle = (VIRTUALARRAY_HANDLE_T *)SCI_ALLOCA(sizeof(VIRTUALARRAY_HANDLE_T));
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Create, no enough memory!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_673_112_2_18_2_21_14_648,(uint8*)"");
        return PNULL;
    }
    
    SCI_MEMSET(handle, 0, sizeof(VIRTUALARRAY_HANDLE_T));
    
    bret = GetFilePathBySapce(handle);
    if(!bret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Create, GetFilePathBySapce error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_682_112_2_18_2_21_14_649,(uint8*)"");
        SCI_FREE(handle);
        
        return PNULL;
    }
    
	// <<add for test
	//handle->mode = VIRTUALARRAY_MEMORY_MODE;
	// >>
    if (VIRTUALARRAY_FILE_MODE == handle->mode)
    {
        if (SFS_ERROR_NONE == CreateAssoFile(handle))
        {
            //db_file_ptr = (char*)MMIAPICOM_WstrToStr(handle->asso_file_name, (uint8 *)file_path);
			int32 file_len = MMIAPICOM_Wstrlen(handle->asso_file_name);
			db_file_ptr = (char*)MMI_WSTRNTOSTR((uint8 *)file_path, (sizeof(file_path) - 1), handle->asso_file_name, file_len, file_len);
        }
    }
    
    // create db
    iret = sqlite3_open(db_file_ptr, &(handle->hsql));
	
    if (SQLITE_OK == iret)
    {
		iret = CreateTable(handle);
    }
    
    if (SQLITE_OK != iret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Create, error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_711_112_2_18_2_21_14_650,(uint8*)"");
        
		MMIVIRTUALARRAY_Destroy(&handle);
        
        return PNULL;
    }
    
    return handle;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_Destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIVIRTUALARRAY_Destroy(
									VIRTUALARRAY *virtual_array_ptr
									)
{
    VIRTUALARRAY_HANDLE_T *handle = 0;
	if (PNULL != virtual_array_ptr)
	{
		handle = (VIRTUALARRAY_HANDLE_T *)(*virtual_array_ptr);
	}
    
    if (PNULL != handle)
    {
		if (NULL != handle->dbResult)
		{
			sqlite3_free_table(handle->dbResult);
			handle->dbResult = NULL;
		}
		
        if (NULL != handle->hsql)
        {
            sqlite3_close(handle->hsql);
            handle->hsql = NULL;
        }
        
        if (VIRTUALARRAY_FILE_MODE == handle->mode)
        {
            RemoveAssoFile(handle);
        }
		
		SCI_FREE(handle);
		
		if (PNULL != virtual_array_ptr)
		{
			*virtual_array_ptr = (VIRTUALARRAY)0;
		}
		
		sqlite_memory_status();
    }
}

/*****************************************************************************/
//  Description : get FILE_ARRAY size
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVIRTUALARRAY_GetSize(
									  VIRTUALARRAY virtual_array
									  )
{
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_GetArraySize, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_777_112_2_18_2_21_14_651,(uint8*)"");
        return 0;
    }
    
    return GetSize(handle);
}

/*****************************************************************************/
//  Description : 获取错误类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIVIRTUALARRAY_GetErrorType(
													VIRTUALARRAY virtual_array
													)
{
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
    
    if (PNULL != handle)
    {
        return handle->assoc_file_save_ret;
    }
    
    return SFS_ERROR_NONE;
}

/*****************************************************************************/
//  Description : 获取存储器类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIVIRTUALARRAY_GetFileStorageDev(
													  VIRTUALARRAY virtual_array
													  )
{
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
    
    if (PNULL != handle)
    {
        return handle->assoc_file_storage;
    }
    
    return MMI_DEVICE_UDISK;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_AddItem
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_AddItem(
									   VIRTUALARRAY virtual_array,
									   void *data_ptr
									   )
{
    BOOLEAN bret = FALSE;
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
	FILEARRAY_DATA_T *filedate_ptr = (FILEARRAY_DATA_T *)data_ptr;
    
    char *pErrMsg = NULL;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
	
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_AddItem, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_838_112_2_18_2_21_14_652,(uint8*)"");
        return FALSE;
    }
	
    if (PNULL == filedate_ptr)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_AddItem, data_ptr = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_844_112_2_18_2_21_14_653,(uint8*)"");
        return FALSE;
    }
    
    bret = IfReachLimitCount(handle, 1);
    if(bret)
    {
        VIRTUALARRAY_LOG("MMIVIRTUALARRAY_AddItem, reach limit!");
        return FALSE;
    }
    
    PackAddSqlSentence(sql_sentence, SQL_MAX_LEN, filedate_ptr);
	
    handle->sql_result = sqlite3_exec(handle->hsql, sql_sentence, NULL, NULL, NULL);
    if (SQLITE_OK != handle->sql_result)
    {
#ifdef SQLITE_DEBUG
		char szErr[256] = {0};
		_snprintf(szErr, sizeof(szErr)-1, "err:%s,%s.",pErrMsg,sql_sentence);
		write_mem_log(szErr);
		if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif	
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_AddItem, error:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_866_112_2_18_2_21_14_654,(uint8*)"s", pErrMsg);
        return FALSE;
    }
	
	ResetResult(handle);
	handle->count++;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_RemoveItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_RemoveItemById(
											  VIRTUALARRAY virtual_array,
											  uint32 index
											  )
{
    BOOLEAN bret = FALSE;
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
    
    char *pErrMsg = NULL;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_RemoveItemById, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_893_112_2_18_2_21_14_655,(uint8*)"");
        return FALSE;
    }
    
    bret = IsValidId(handle, index);
    if (!bret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_RemoveItemById, invalid id."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_900_112_2_18_2_21_14_656,(uint8*)"");
        return FALSE;
    }
    
    PackDelSqlSentence(sql_sentence, SQL_MAX_LEN, index, \
		PackSortSqlSentence(handle->sort_type));
    
    handle->sql_result = sqlite3_exec(handle->hsql, sql_sentence, NULL, NULL, NULL);
    if (SQLITE_OK != handle->sql_result)
    {
#ifdef SQLITE_DEBUG        
		char szErr[256] = {0};
		_snprintf(szErr, sizeof(szErr)-1, "err:%s,%s.",sql_sentence, pErrMsg);
		write_mem_log(szErr);
		if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif		
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_RemoveItemById, error:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_916_112_2_18_2_21_14_657,(uint8*)"s", pErrMsg);
        return FALSE;
    }
    
	if (index + 1 != GetSize(virtual_array))
	{
		ResetResult(handle);
	}	
	handle->count--;
	
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_UpdateItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_UpdateItemById(
											  VIRTUALARRAY virtual_array,
											  uint32 index,
											  void *data_ptr
											  )
{
    BOOLEAN bret = FALSE;
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
	FILEARRAY_DATA_T *filedate_ptr = (FILEARRAY_DATA_T *)data_ptr;
    
    char *pErrMsg = NULL;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_UpdateItemById, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_948_112_2_18_2_21_14_658,(uint8*)"");
        return FALSE;
    }
    
    if (PNULL == filedate_ptr)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_UpdateItemById, data_ptr = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_954_112_2_18_2_21_14_659,(uint8*)"");
        return FALSE;
    }
    
    bret = IsValidId(handle, index);
    if (!bret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_UpdateItemById, invalid id."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_961_112_2_18_2_21_14_660,(uint8*)"");
        return FALSE;
    }
    
    PackUpdateSqlSentence(sql_sentence, SQL_MAX_LEN, index, \
		PackSortSqlSentence(handle->sort_type), filedate_ptr);
    
    handle->sql_result = sqlite3_exec(handle->hsql, sql_sentence, NULL, NULL, 0);
    if (SQLITE_OK != handle->sql_result)
    {
#ifdef SQLITE_DEBUG 
		char szErr[256] = {0};
		_snprintf(szErr, sizeof(szErr)-1, "err:%s,%s.",sql_sentence, pErrMsg);
		write_mem_log(szErr);
		if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_UpdateItemById, error:%s."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_977_112_2_18_2_21_14_661,(uint8*)"s", pErrMsg);
        return FALSE;
    }
	
	ResetResult(handle);
    return TRUE;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_GetItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_GetItemById(
										   VIRTUALARRAY virtual_array,
										   uint32 index,
										   void *data_ptr
										   )
{
    BOOLEAN bret = FALSE;
    char **dbResult = PNULL;
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
	FILEARRAY_DATA_T *filedate_ptr = (FILEARRAY_DATA_T *)data_ptr;
    
    char *pErrMsg = NULL;
	static int nColumn = 0;
    int nRow = 0;
    char sql_sentence[SQL_MAX_LEN + 1] = {0};
    
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_GetItemById, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1007_112_2_18_2_21_14_662,(uint8*)"");
        return FALSE;
    }
    
    if (PNULL == filedate_ptr)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_GetItemById, data_ptr = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1013_112_2_18_2_21_14_663,(uint8*)"");
        return FALSE;
    }
    
    bret = IsValidId(handle, index);
    if (!bret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_GetItemById, invalid id."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1020_112_2_18_2_21_14_664,(uint8*)"");
        return FALSE;
    }
    
    if (FALSE == handle->has_modified
		&& NULL != handle->dbResult
		&& index >= handle->first_id
		&& index <= handle->last_id)
    {
    }
	else
	{
		handle->last_id = MIN(index + QUERY_ITEM_COUNT - 1, handle->count - 1);
		handle->first_id = handle->last_id - QUERY_ITEM_COUNT + 1;
		if ((int32)handle->first_id < 0)
		{
			handle->first_id = 0;
		}
		
		PackQuerySqlSentence(sql_sentence, SQL_MAX_LEN, \
			PackSortSqlSentence(handle->sort_type), \
			handle->last_id - handle->first_id + 1,\
			handle->first_id);
		
		if (NULL != handle->dbResult)
		{
			sqlite3_free_table(handle->dbResult);
			handle->dbResult = NULL;
		}
		
		handle->sql_result = sqlite3_get_table(handle->hsql, sql_sentence, \
			&(handle->dbResult), &nRow, &nColumn, 0);
		if (SQLITE_OK != handle->sql_result)
		{
#ifdef SQLITE_DEBUG
			char szErr[256] = {0};
			_snprintf(szErr, sizeof(szErr)-1, "err:%s,%s.",sql_sentence, pErrMsg);
			write_mem_log(szErr);
			if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif			
			//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_GetItemById, error:%s."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1060_112_2_18_2_21_15_665,(uint8*)"s", pErrMsg);
			return FALSE;
		}
		handle->has_modified = FALSE;
	}
	
    //if (nRow > 0)
    {
		uint32 start = (uint32)nColumn * (index - handle->first_id +1);
		
		dbResult = handle->dbResult;
		
        // name
		SCI_MEMSET(filedate_ptr->filename, 0, MMIFILE_FULL_PATH_MAX_LEN);
        GUI_UTF8ToWstr(filedate_ptr->filename, MMIFILE_FULL_PATH_MAX_LEN, (const uint8*)dbResult[start],\
			strlen(dbResult[start]));
        filedate_ptr->name_len = MMIAPICOM_Wstrlen(filedate_ptr->filename);
        
        // data_time
        CoverStringToDateTime(dbResult[start + 1], &filedate_ptr->modify_date, &filedate_ptr->modify_time);
        
        // size
        filedate_ptr->size = atoi(dbResult[start + 2]);
        
        // type
        switch(atoi(dbResult[start + 3]))
        {
        case 1:
            filedate_ptr->type = FILEARRAY_TYPE_FILE;
            break;
        case 2:
            filedate_ptr->type = FILEARRAY_TYPE_FOLDER;
            break;
        default:
            filedate_ptr->type = FILEARRAY_TYPE_NULL;
            break;
        }     
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_SetSort
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_SetSort(
									   VIRTUALARRAY virtual_array,
									   int32	sort_type
									   )
{
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;	
	
    if (PNULL != handle)
    {
		if (sort_type != handle->sort_type)
		{
			handle->sort_type = (FILEARRAY_SORT_E)sort_type;
			ResetResult(handle);
		}
        
        return TRUE;
    }
	
	return FALSE;
}

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_Combine
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_Combine(
									   VIRTUALARRAY array1,
									   VIRTUALARRAY array2
									   )
{
    BOOLEAN bret = FALSE;
    int32     iret = -1;
    uint32  cnt = 0;
	
    VIRTUALARRAY_HANDLE_T *handle1 = (VIRTUALARRAY_HANDLE_T *)array1;
    VIRTUALARRAY_HANDLE_T *handle2 = (VIRTUALARRAY_HANDLE_T *)array2;
	
    if (PNULL == handle1 || PNULL == handle2)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Combine, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1145_112_2_18_2_21_15_666,(uint8*)"");
        return FALSE;
    }
    
    cnt = GetSize(handle2);
	if (0 == cnt)
	{
		return TRUE;
	}
	
    bret = IfReachLimitCount(handle1,cnt);
    if (bret)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Combine, array1 reach limit count!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1158_112_2_18_2_21_15_667,(uint8*)"");
        return FALSE;
    }
    
	iret = ArrayCombine(handle1->hsql, handle2->hsql);
	if (SQLITE_OK == iret)
	{
		handle1->is_need_update = TRUE;
		ResetResult(handle1);
	}
	
    return (SQLITE_OK == iret);    
}

/*****************************************************************************/
//  Description : 另存一个FILE_ARRAY，关机后不会丢失
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIVIRTUALARRAY_SaveToFile(
												  VIRTUALARRAY    virtual_array,
												  uint16         *file_name,
												  uint16         file_name_len
												  )
{
	int32 iret = -1;
	char *pErrMsg = NULL;
	SFS_HANDLE  hfile = NULL;
	uint8 u8_save_file[MMIFMM_FILE_FULL_PATH_MAX_LEN*3 + 1] = {0};
	char sql_attch[SQL_MAX_LEN + 1] = {0};
	char *attch_ptr = "attach '%s' as new_save_db";
	VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;
	
	if (PNULL == handle || PNULL == file_name || 0 == file_name_len)
	{
		//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_SaveToFile, input param error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1192_112_2_18_2_21_15_668,(uint8*)"");
		return SFS_ERROR_INVALID_PARAM;
	}
	
	hfile = SFS_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
	if (NULL == hfile)
	{
		//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_SaveToFile, create file error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1199_112_2_18_2_21_15_669,(uint8*)"");
		return SFS_ERROR_SYSTEM;
	}
	
	SFS_SetFileSize(hfile, 0);
	SFS_CloseFile(hfile);
	
	GUI_WstrToUTF8(u8_save_file, (sizeof(u8_save_file) - 1), file_name, file_name_len);
	CheckNameString((char*)u8_save_file, sizeof(u8_save_file) - 1);
	_snprintf(sql_attch, SQL_MAX_LEN, attch_ptr, u8_save_file);
	iret = sqlite3_exec(handle->hsql, sql_attch, 0, 0, 0);
	
	if (SQLITE_OK == iret)
	{
		iret = sqlite3_exec(handle->hsql, "create table new_save_db.FILE_ARRAY as select * from FILE_ARRAY",\
			0, 0, 0);
	}
    
	if (SQLITE_OK == iret)
	{
		iret = sqlite3_exec(handle->hsql, "detach new_save_db", 0, 0, 0);
	}
	
	if (SQLITE_OK != iret)
	{
#ifdef SQLITE_DEBUG
		char szErr[256] = {0};
		_snprintf(szErr, sizeof(szErr)-1, "err:SaveToFile,%s.", pErrMsg);
		write_mem_log(szErr);
		if( pErrMsg ) sqlite3_free(*pErrMsg);
#endif		
		//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_SaveToFile, attach error:%s!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1230_112_2_18_2_21_15_670,(uint8*)"s", pErrMsg);
		return SFS_ERROR_SYSTEM;
	}
	
	return SFS_NO_ERROR;
}

/*****************************************************************************/
//  Description : 从指定文件中初始化FILE_ARRAY
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_LoadFromFile(
											VIRTUALARRAY    virtual_array,
											uint16         *file_name,
											uint16         file_name_len
											)
{
	int32 iret = -1;
	VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)virtual_array;

   	if (PNULL == handle || PNULL == file_name || 0 == file_name_len)
	{
		//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_LoadFromFile, input param error!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1252_112_2_18_2_21_15_671,(uint8*)"");
		return FALSE;
	}
	
	if (VIRTUALARRAY_FILE_MODE == handle->mode)
	{
		uint8 u8_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		
		sqlite3_close(handle->hsql);
		handle->hsql = NULL;
		RemoveAssoFile(handle);
		
		if (SFS_ERROR_NONE != MMIAPIFMM_CopyFile(file_name, handle->asso_file_name))
		{
			//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_LoadFromFile, copy file error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1266_112_2_18_2_21_15_672,(uint8*)"");
			return FALSE;
		}
		
		MMIAPIFMM_SetAttr(handle->asso_file_name, MMIAPICOM_Wstrlen(handle->asso_file_name), \
            TRUE, FALSE, FALSE, FALSE);
		
		GUI_WstrToUTF8(u8_file_name, MMIFILE_FULL_PATH_MAX_LEN, handle->asso_file_name, \
			MMIAPICOM_Wstrlen(handle->asso_file_name));
		
		iret = sqlite3_open((const char*)u8_file_name, &(handle->hsql));
        if (SQLITE_OK != iret)
		{
			//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_LoadFromFile, FILE_MODE open db error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1279_112_2_18_2_21_15_673,(uint8*)"");
		}
	}
	else if (VIRTUALARRAY_MEMORY_MODE == handle->mode)
	{
		//Combine two table
		HSQL hsql = 0;
		uint8 u8_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
		
		GUI_WstrToUTF8(u8_file_name, MMIFILE_FULL_PATH_MAX_LEN, file_name, file_name_len);
		iret = sqlite3_open((const char*)u8_file_name, &hsql);
		if (SQLITE_OK != iret)
		{
			//VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_LoadFromFile, MEMORY_MODE open db error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1292_112_2_18_2_21_15_674,(uint8*)"");
			return FALSE;
		}
		
		iret = ArrayCombine(handle->hsql, hsql);
		if (SQLITE_OK == iret)
		{
			handle->is_need_update = TRUE;
			ResetResult(handle);
		}
		sqlite3_close(hsql);
	}
	
    return (SQLITE_OK == iret);
}

PUBLIC BOOLEAN MMIVIRTUALARRAY_Transaction(
										   VIRTUALARRAY v_array, 
										   BOOLEAN begin
										   )
{
	char * Trans_ptr = PNULL;
	
    VIRTUALARRAY_HANDLE_T *handle = (VIRTUALARRAY_HANDLE_T *)v_array;
	
    if (PNULL == handle)
    {
        //VIRTUALARRAY_WARNNING:"MMIVIRTUALARRAY_Transaction, handle = 0!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1319_112_2_18_2_21_15_675,(uint8*)"");
        return FALSE;
    }
	
	if (begin)
	{
		Trans_ptr = "BEGIN;";
	}
	else
	{
		Trans_ptr = "COMMIT;";
	}
	
	return (SQLITE_OK == sqlite3_exec(handle->hsql, Trans_ptr, 0, 0, 0));
}

//////////////////////////////////////////////////////////////////////////
// test code
LOCAL void sqlite_memory_status(void)
{
	sqlite3_int64 nCur = 0;
	sqlite3_int64 nHigh = 0;
	char szOut[64] = {0};
	
	nCur = sqlite3_memory_used();
	nHigh = sqlite3_memory_highwater(1);
	
	//SCI_TRACE_LOW:"sqlite_memory_status, nCur = %d,nHigh = %d."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1346_112_2_18_2_21_15_676,(uint8*)"dd",nCur, nHigh);
	
	_snprintf(szOut, sizeof(szOut), "sqlite_memory_status, nCur = %d,nHigh = %d.",(long)nCur, (long)nHigh);
	//write_mem_log(szOut);
	
	if (0 == nCur)
	{
		//SCI_TRACE_LOW:"sqlite free all."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_SQL_1353_112_2_18_2_21_15_677,(uint8*)"");
	}
}

#if 0
FILE* g_mem_file = 0;

#define SQLITE_STAUTS_FILE	"D:\\sql_status.txt"
void write_mem_log(char *str_ptr)
{
	char* pNewline = "\n";
	if (0 == g_mem_file)
	{
		g_mem_file = SPLIBC_fopen(SQLITE_STAUTS_FILE, "a+");;
	}
	if (0 != g_mem_file)
	{
		SPLIBC_fwrite(str_ptr, sizeof(char), strlen(str_ptr), g_mem_file);
		SPLIBC_fwrite(pNewline, sizeof(char), strlen(pNewline), g_mem_file);
	}
	if (0 != g_mem_file)
	{
		SPLIBC_fclose(g_mem_file);
		g_mem_file = 0;
	}
}

void virtual_array_test()
{
	int32 i = 0;
	wchar* file_name[] =
	{
		L"D:\\who.txt",
			L"E:\\ghost.db",
		L"D:\\mygod.bak"
	};
	
	uint cnt = 0; 
	FILEARRAY_DATA_T date_test = {0};
	VIRTUALARRAY varr = MMIVIRTUALARRAY_Create();
	VIRTUALARRAY varr2 = 0;
	MMIFILE_DEVICE_E dev_storage = MMIVIRTUALARRAY_GetFileStorageDev(varr);
	SFS_ERROR_E  err = MMIVIRTUALARRAY_GetErrorType(varr);	
	
	// add
	TM_GetSysDate(&date_test.modify_date);
	TM_GetSysTime(&date_test.modify_time);
	MMIAPICOM_Wstrcpy(date_test.filename, file_name[0]);
	date_test.name_len = MMIAPICOM_Wstrlen(date_test.filename);
	date_test.size = 1211;
	date_test.type = 1;
	
	MMIVIRTUALARRAY_AddItem(varr, &date_test);
	
	SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
	TM_GetSysDate(&date_test.modify_date);
	TM_GetSysTime(&date_test.modify_time);
	MMIAPICOM_Wstrcpy(date_test.filename, file_name[1]);
	date_test.name_len = MMIAPICOM_Wstrlen(date_test.filename);
	date_test.size = 356;
	date_test.type = 1;
	
	MMIVIRTUALARRAY_AddItem(varr, &date_test);
	
	SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
	TM_GetSysDate(&date_test.modify_date);
	TM_GetSysTime(&date_test.modify_time);
	MMIAPICOM_Wstrcpy(date_test.filename, file_name[2]);
	date_test.name_len = MMIAPICOM_Wstrlen(date_test.filename);
	date_test.size = 1600;
	date_test.type = 2;
	
	MMIVIRTUALARRAY_AddItem(varr, &date_test);
	
	// get
	SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
	MMIVIRTUALARRAY_GetItemById(varr, 0, &date_test);
	
	//update
	// 	SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
	// 	TM_GetSysDate(&date_test.modify_date);
	// 	TM_GetSysTime(&date_test.modify_time);
	// 	MMIAPICOM_Wstrcpy(date_test.filename, file_name[0]);
	// 	date_test.name_len = MMIAPICOM_Wstrlen(date_test.filename);
	date_test.size = 1458;
	date_test.type = 1;
	
	MMIVIRTUALARRAY_UpdateItemById(varr, 0, &date_test);
	
	// load
	{
		wchar *pwszFile = L"E:\\VA_TEST";
		
		varr2 = MMIVIRTUALARRAY_Create();
		MMIVIRTUALARRAY_LoadFromFile(varr2, pwszFile, MMIAPICOM_Wstrlen(pwszFile));
		
		cnt = MMIVIRTUALARRAY_GetSize(varr2);
		for (i = 0; i < cnt; i++)
		{
			SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
			MMIVIRTUALARRAY_GetItemById(varr2, i, &date_test);
		}
	}
	
	// combine
	MMIVIRTUALARRAY_Combine(varr, varr2);
	
	// size
	cnt = MMIVIRTUALARRAY_GetSize(varr);
	
	// sort
	MMIVIRTUALARRAY_SetSort(varr, FILEARRAY_SORT_TIME_DESCEND);	
	for (i = 0; i < cnt; i++)
	{
		SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
		MMIVIRTUALARRAY_GetItemById(varr, i, &date_test);
	}
	
	MMIVIRTUALARRAY_SetSort(varr, FILEARRAY_SORT_TIME_ASCEND);	
	for (i = 0; i < cnt; i++)
	{
		SCI_MEMSET(&date_test, 0, sizeof(FILEARRAY_DATA_T));
		MMIVIRTUALARRAY_GetItemById(varr, i, &date_test);
	}
	
	// save
	MMIVIRTUALARRAY_SaveToFile(varr, file_name[1], MMIAPICOM_Wstrlen(file_name[1]));
	virtual_array_get_status();
	
	// remove
	MMIVIRTUALARRAY_RemoveItemById(varr, 0);
	
	// destroy
	MMIVIRTUALARRAY_Destroy(&varr2);
	MMIVIRTUALARRAY_Destroy(&varr);
	virtual_array_get_status();
}
#endif // 0

/*****************************************************************************/
//  Description : get all data size
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVIRTUALARRAY_GetAllDataSize(
                                                     VIRTUALARRAY virtual_array  //in
                                                     )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    uint32 data_size = 0;

    if(NULL == virtual_array)
    {
        return 0;
    }

    //判断virtual_array是否是有效的VIRTUALARRAY_HEADER_T类型指针
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        return 0;
    }

    data_size += v_header_ptr->data_size * v_header_ptr->total_size;
    
    return data_size;

}

#endif  // SQLITE_FILEARRAY_SUPPORT

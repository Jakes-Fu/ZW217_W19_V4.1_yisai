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

#ifdef  SQLITE_FILEARRAY_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifilearray_export.h"
#include "mmivirtualarray.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmifmm_export.h"

/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/



/*****************************************************************************/
//  Description : MMIAPIFILEARRAY_Create
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIFILEARRAY_Create(void)
{
    return (FILEARRAY)MMIVIRTUALARRAY_Create();
}

/*****************************************************************************/
//  Description : MMIAPIFILEARRAY_Destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFILEARRAY_Destroy(
								 FILEARRAY   *file_array_ptr
								 )
{
    if(PNULL != file_array_ptr
		&& PNULL != *file_array_ptr)
    {
		MMIVIRTUALARRAY_Destroy(file_array_ptr);
    }
}

/*****************************************************************************/
//  Description : 获得filearray数组大小
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFILEARRAY_GetArraySize(
										FILEARRAY   file_array      //in, filearray值
										)
{
	if (PNULL == file_array)
	{
		return 0;
	}
	
	return MMIVIRTUALARRAY_GetSize(file_array);
}

/*****************************************************************************/
//  Description : 获取文件错误类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_GetFileErrorType(
											  FILEARRAY   file_array
											  )
{
	if (PNULL == file_array)
	{
		return SFS_ERROR_INVALID_PARAM;
	}
	
	return MMIVIRTUALARRAY_GetErrorType(file_array);
}

/*****************************************************************************/
//  Description : 获取存储器类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIAPIFILEARRAY_GetFileDev( 
									 FILEARRAY   file_array//IN
									 )
{
	return MMIVIRTUALARRAY_GetFileStorageDev(file_array);
}

/*****************************************************************************/
//  Description : 在filearray数组中增加一个数据
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Add(
						 FILEARRAY			file_array,     //in, filearray值
						 FILEARRAY_DATA_T   *data_ptr   //in, 要增加的数据指针
						 )
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_AddItem(file_array, data_ptr);	
}

/*****************************************************************************/
//  Description : 从filearray数组中读取一个数据
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Read(
						  FILEARRAY   file_array,     //in, filearray值
						  uint32      index,   //in, 数据索引号，0到total_size-1
						  FILEARRAY_DATA_T    *data_ptr  //out, 存放读取的数据
						  )
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_GetItemById(file_array, index, data_ptr);				
}

/*****************************************************************************/
//  Description : 在filearray数组中修改一个数据
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Modify( 
							FILEARRAY   file_array,     //in, filearray值
							uint32      index,			//in, 数据索引号，0到total_size-1
							FILEARRAY_DATA_T    *data_ptr  //in, 存放修改的数据
							)
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_UpdateItemById(file_array, index, data_ptr);	
}

/*****************************************************************************/
//  Description : 在filearray数组中删除一个数据
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Delete(
							FILEARRAY   file_array,     //in, filearray值
							uint32      index			//in, 数据索引号，0到total_size-1
							)
{
	if(NULL == file_array)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_RemoveItemById(file_array, index);		
}

/*****************************************************************************/
//  Description : 对filearray数组进行排序
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Sort(
						  FILEARRAY			file_array, //in, filearray值
						  FILEARRAY_SORT_E  sort_type   //in, 排序类型
						  )
{
	if(NULL == file_array
		|| 	MMIFMM_IsSearchingStopped())
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_SetSort(file_array, sort_type);		
}

/*****************************************************************************/
//  Description : 合并两个filearray数组
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Combine( 
							 FILEARRAY   file_array_1,       //in/out, filearray 1
							 FILEARRAY   file_array_2        //in, filearray 2
							 )
{
    if(NULL == file_array_1 || NULL == file_array_2)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_Combine(file_array_1, file_array_2);
}

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SaveArrayToFile(
                                     FILEARRAY  file_array,     //in, filearray值
                                     uint16     *file_name,     //要保存到的文件名, 文件一定要存在
                                     uint16     file_name_len
                                     )
{
    if(NULL == file_array || NULL == file_name || 0 == file_name_len)
    {
        return FALSE;
    }
	
	return (SFS_NO_ERROR == MMIVIRTUALARRAY_SaveToFile(file_array, file_name, file_name_len));
}

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_SaveArrayToFileEx(
											   FILEARRAY  file_array,     //in, filearray值
											   uint16     *file_name,     //要保存到的文件名, 文件一定要存在
											   uint16     file_name_len
											   )
{
    if(NULL == file_array || NULL == file_name || 0 == file_name_len)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
	
	return MMIVIRTUALARRAY_SaveToFile(file_array, file_name, file_name_len);
}

/*****************************************************************************/
//  Description : 从指定文件中初始化filearray数组
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_LoadArrayFromFile(
									   FILEARRAY  file_array,     //in, filearray值
									   uint16     *file_name,     //要读取的文件名
									   uint16     file_name_len
									   )
{
    if(NULL == file_array || NULL == file_name || 0 == file_name_len)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_LoadFromFile(file_array, file_name, file_name_len);
}

/*****************************************************************************/
//  Description : 设置folder array中每个文件夹的子文件在据filearray中的开始位置和结束位置             
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SetFolderStartFileAndEndFile(
                                     //FILEARRAY folder_array,
									 FILEARRAY file_array
                                     )
{
	return TRUE;
}

/*****************************************************************************/
//  Description : 将folder array纪录到base_virtual_array中
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
void MMIAPIFILEARRAY_AddFolderArray(
                                     VIRTUALARRAY    base_virtual_array,
                                     VIRTUALARRAY    folder_virtual_array
                                     )
{
	
}

/*****************************************************************************/
//  Description : 将folder array纪录到base_virtual_array中
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
void MMIVIRTUALARRAY_AddFolderArray(
                                     VIRTUALARRAY    base_virtual_array,
                                     VIRTUALARRAY    folder_virtual_array
                                     )
{
	return;
}


/*****************************************************************************/
//  Description : 读取该文件夹中第一个文件在filearray的index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetFirstFilePos(  //返回文件夹中第一个文件在filearray的位置
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index   //in, 文件夹索引号，0到total_size-1
                        )
{
	return 0;
}

/*****************************************************************************/
//  Description : 读取该文件夹中最后一个文件在filearray的index
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetLastFilePos(  //返回文件夹中最后一个文件在filearray的位置
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index   //in, 文件夹索引号，0到total_size-1                   
                        )
{
    return 0;

}

/*****************************************************************************/
//  Description : 从文件夹数组中读取一个文件夹数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_ReadFolder(  //返回True成功或False失败
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index,   //in, 数据索引号，0到total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //out, 存放读取的数据
                        )
{
    BOOLEAN     result = FALSE;

    return result;
}
/*****************************************************************************/
//  Description : 文件夹的个数
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetTotalFolderCount(   
                         FILEARRAY file_array
                        )
{
   return 0;
}
BOOLEAN MMIAPIFILEARRAY_Transaction(
								 FILEARRAY  file_array, 
								 BOOLEAN begin
								 )
{
    if(NULL == file_array)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_Transaction(file_array, begin);
}

/*****************************************************************************/
//  Description : get file array all data size
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFILEARRAY_GetAllDataSize(
                                                           FILEARRAY    file_array //in
                                                           )
{
    uint32 data_size = 0;


    if (PNULL != file_array)
    {
        data_size = MMIVIRTUALARRAY_GetAllDataSize(file_array);
    }


    return data_size;
}

#endif	// #ifdef  SQLITE_FILEARRAY_SUPPORT

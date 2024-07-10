/*****************************************************************************
** File Name:      mmifilearray_export.h                                            *
** Author:                                                                   *
** Date:             12/18/2007                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2007       liqing.peng          Create                                 *
******************************************************************************/

#ifndef _MMIFILEARRAY_H_
#define _MMIFILEARRAY_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_time.h"
#include "mmi_filemgr.h"
#include "mmivirtualarray.h"

/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define FILEARRAY_FILENAME_MAX_LEN      MMIFILE_FULL_PATH_MAX_LEN   //全路径名最大长度
#define FILEARRAY_ITEM_MIN_NUM          50 //内存缓冲区大小，50个数据
#define FILEARRAY_MAX_BUFFER_NUMBER     100


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
//数组文件类型
typedef void*   FILEARRAY;
typedef VIRTUALARRAY_TYPE_E FILEARRAY_TYPE_E;
//typedef enum
//{
//    FILEARRAY_TYPE_NULL,
//    FILEARRAY_TYPE_FILE,    //文件类型
//    FILEARRAY_TYPE_FOLDER   //文件夹类型
// }FILEARRAY_TYPE_E;
//
////单个数据结构
//typedef struct  
//{
//    wchar       filename[FILEARRAY_FILENAME_MAX_LEN+1]; //文件名或文件夹名
//    uint16      name_len;
//    SCI_DATE_T  modify_date;    //修改日期
//    SCI_TIME_T  modify_time;    //修改时间
//    uint32      size;   //文件大小
//    FILEARRAY_TYPE_E    type;
// }FILEARRAY_DATA_T;

typedef VIRTUALARRAY_DATA_T FILEARRAY_DATA_T;
//数据排序方式
typedef enum
{
    FILEARRAY_SORT_NULL,
    FILEARRAY_SORT_NAME_ASCEND,
    FILEARRAY_SORT_NAME_DESCEND,
    FILEARRAY_SORT_TIME_ASCEND,
    FILEARRAY_SORT_TIME_DESCEND,
    FILEARRAY_SORT_ATTR_ASCEND,//属性 序
    FILEARRAY_SORT_ATTR_DESCEND,
    FILEARRAY_SORT_TIME_CREATE_ASCEND,//创建时间序
    FILEARRAY_SORT_TIME_CREATE_DESCEND,
    FILEARRAY_SORT_TIME_MODIFY_ASCEND,//修改时间序
    FILEARRAY_SORT_TIME_MODIFY_DESCEND,
    FILEARRAY_SORT_TIME_ASK_ASCEND,//访问时间序
    FILEARRAY_SORT_TIME_ASK_DESCEND,
    FILEARRAY_SORT_SIZE_ASCEND,
    FILEARRAY_SORT_SIZE_DESCEND,
    FILEARRAY_SORT_TYPE_ASCEND,
    FILEARRAY_SORT_TYPE_DESCEND,
    FILEARRAY_SORT_TYPE_CUST  //用户自定义排序方式
}FILEARRAY_SORT_E;


#define MMIAPIFILEARRAY_Create()            MMIAPIFA_CreateArray(_D_FILE_NAME, _D_FILE_LINE)
#define MMIAPIFILEARRAY_Destroy(_PTR_)      MMIAPIFA_DestroyArray(_PTR_, _D_FILE_NAME, _D_FILE_LINE)
/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : file array init
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFILEARRAY_Init(void);

/*****************************************************************************/
//  Description : 创建一个filearray数组
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIFA_CreateArray(
                                      const char *file,
                                      uint32   line
                                      ); //返回分配的指针

/*****************************************************************************/
//  Description : 销毁一个filearray数组
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFA_DestroyArray(
                                    FILEARRAY   *file_array_ptr,     //out, 清空指针
                                    const char          *file,
                                    uint32              line
                                     );

/*****************************************************************************/
//  Description : 获得filearray数组大小
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArraySize(   //返回filearray数组大小
                        FILEARRAY   file_array      //in, filearray值
                        );
/*****************************************************************************/
//  Description : 获得filearray数组大小
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy  MMIAPIFILEARRAY_GetArraySize liyan modify for CR153633
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArrayCurSize(   //返回filearray数组大小
									   FILEARRAY   file_array      //in, filearray值
									);
/*****************************************************************************/
//  Description : 判断是否由于磁盘空间不足而提前结束
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_GetFileErrorType(
                            FILEARRAY   file_array
                            );

/*****************************************************************************/
//  Description : 出错的存储器类型
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIAPIFILEARRAY_GetFileDev(    //返回出错的存储器类型
                         FILEARRAY   file_array//IN
						);

/*****************************************************************************/
//  Description : 在filearray数组中增加一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Add(   //返回True成功或False失败
                        FILEARRAY   file_array,     //in, filearray值
                        FILEARRAY_DATA_T    *data_ptr   //in, 要增加的数据指针
                        );

/*****************************************************************************/
//  Description : 从filearray数组中读取一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Read(  //返回True成功或False失败
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index,   //in, 数据索引号，0到total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //out, 存放读取的数据
                        );

/*****************************************************************************/
//  Description : 在filearray数组中修改一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Modify(    //返回True成功或False失败
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index,   //in, 数据索引号，0到total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //in, 存放修改的数据
                        );

/*****************************************************************************/
//  Description : 在filearray数组中删除一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Delete(    //返回True成功或False失败
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index   //in, 数据索引号，0到total_size-1
                        );
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : 对filearray数组进行排序
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Sort(  //返回True成功或False失败
                        FILEARRAY       file_array, //in, filearray值
                        FILEARRAY_SORT_E    sort_type   //in, 排序类型
                        );
#endif
/*****************************************************************************/
//  Description : 组合两个filearray数组, 
//                [file_array_1] + [file_array_2] -> [file_array_1]
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Combine(   //返回True成功或False失败
                            FILEARRAY   file_array_1,       //in/out, filearray 1
                            FILEARRAY   file_array_2        //in, filearray 2
                            );

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SaveArrayToFile(
                                     FILEARRAY  file_array,     //in, filearray值
                                     uint16     *file_name,     //要保存到的文件名, 文件一定要存在
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_SaveArrayToFileEx(
                                     FILEARRAY  file_array,     //in, filearray值
                                     uint16     *file_name,     //要保存到的文件名, 文件一定要存在
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : 从指定文件中初始化filearray数组，和MMIFILEARRAY_SaveArrayToFile
//                函数相对应
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_LoadArrayFromFile(
                                     FILEARRAY  file_array,     //in, filearray值
                                     uint16     *file_name,     //要读取的文件名
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : 设置folder array中每个文件夹的子文件在据filearray中的开始位置和结束位置             
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SetFolderStartFileAndEndFile(
                                     //FILEARRAY folder_array,
									 FILEARRAY file_array
                                     );

/*****************************************************************************/
//  Description : 读取该文件夹中第一个文件在filearray的index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetFirstFilePos(  //返回文件夹中第一个文件在filearray的位置
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index   //in, 文件夹索引号，0到total_size-1
                        );

/*****************************************************************************/
//  Description : 读取该文件夹中最后一个文件在filearray的index
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetLastFilePos(  //返回文件夹中最后一个文件在filearray的位置
                        FILEARRAY   file_array,     //in, filearray值
                        uint32      index   //in, 文件夹索引号，0到total_size-1                   
                        );


/*****************************************************************************/
//  Description : 将folder array纪录到base_virtual_array中
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
void MMIAPIFILEARRAY_AddFolderArray(
                                     VIRTUALARRAY    base_virtual_array,
                                     VIRTUALARRAY    folder_virtual_array
                                     );

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
                        );

/*****************************************************************************/
//  Description : 文件夹的个数
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetTotalFolderCount(   
                         FILEARRAY file_array
                        );

/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SetBufferSize(
                                     FILEARRAY    file_array,
                                     uint32          size           //counted in KB
                                     );

#ifdef  SQLITE_SUPPORT
/*******************************************************************/
//  Interface:		MMIAPIFILEARRAY_Transaction
//  Description : 	MMIAPIFILEARRAY_Transaction
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
BOOLEAN MMIAPIFILEARRAY_Transaction(
								 FILEARRAY  file_array, 
								 BOOLEAN begin
								 );
#endif

/*****************************************************************************/
//  Description : get file array all data size
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFILEARRAY_GetAllDataSize(
                                                           FILEARRAY    file_array //in
                                                           );
                                                             
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIFILEARRAY_Create		 MMIAPIFILEARRAY_Create

#define MMIFILEARRAY_Destroy		 MMIAPIFILEARRAY_Destroy

#define MMIFILEARRAY_GetArraySize		 MMIAPIFILEARRAY_GetArraySize

#define MMIFILEARRAY_GetFileErrorType		 MMIAPIFILEARRAY_GetFileErrorType

#define MMIFILEARRAY_GetFileDev		 MMIAPIFILEARRAY_GetFileDev

#define MMIFILEARRAY_Add		 MMIAPIFILEARRAY_Add

#define MMIFILEARRAY_Read		 MMIAPIFILEARRAY_Read

#define MMIFILEARRAY_Modify		 MMIAPIFILEARRAY_Modify

#define MMIFILEARRAY_Delete		 MMIAPIFILEARRAY_Delete

#define MMIFILEARRAY_Sort		 MMIAPIFILEARRAY_Sort

#define MMIFILEARRAY_Combine		 MMIAPIFILEARRAY_Combine

#define MMIFILEARRAY_SaveArrayToFile		 MMIAPIFILEARRAY_SaveArrayToFile

#define MMIFILEARRAY_SaveArrayToFileEx		 MMIAPIFILEARRAY_SaveArrayToFileEx

#define MMIFILEARRAY_LoadArrayFromFile		 MMIAPIFILEARRAY_LoadArrayFromFile

#define MMIFILEARRAY_SetFolderStartFileAndEndFile		 MMIAPIFILEARRAY_SetFolderStartFileAndEndFile

#define MMIFILEARRAY_GetFirstFilePos		 MMIAPIFILEARRAY_GetFirstFilePos

#define MMIFILEARRAY_GetLastFilePos		 MMIAPIFILEARRAY_GetLastFilePos

#define MMIFILEARRAY_AddFolderArray		 MMIAPIFILEARRAY_AddFolderArray

#define MMIFILEARRAY_ReadFolder		 MMIAPIFILEARRAY_ReadFolder

#define MMIFILEARRAY_GetTotalFolderCount		 MMIAPIFILEARRAY_GetTotalFolderCount

#ifdef  SQLITE_SUPPORT
#define MMIFILEARRAY_Transaction		 MMIAPIFILEARRAY_Transaction
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#endif //_MMIFILEARRAY_H_

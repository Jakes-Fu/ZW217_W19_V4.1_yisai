/*****************************************************************************
** File Name:      mmivirtualarray.h                                         *
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

#ifndef _MMIVIRTUALARRAY_H_
#define _MMIVIRTUALARRAY_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "dal_time.h"
#include "mmi_filemgr.h"


/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
//#define VIRTUALARRAY_DEFAULT_DIR_U      (g_mmivirtualarray_default_udisk_dir_wstr)//"\x00\x44\x00\x3A\x00\x5C"
//#define VIRTUALARRAY_DEFAULT_DIR_SD     (g_mmivirtualarray_default_sdcard_dir_wstr)//"\x00\x45\x00\x3A\x00\x5C"
//#define VIRTUALARRAY_DEFAULT_DIR_SD1     (g_mmivirtualarray_default_sdcard1_dir_wstr)//"\x00\x46\x00\x3A\x00\x5C"

//#define VIRTUALARRAY_DEFAULT_DIR_LEN    3
#define VIRTUALARRAY_FILE_PREFIX        "VIRTUALARRAY_"
#define VIRTUALARRAY_FILE_PREFIX_LEN    13
typedef enum
{
    FILEARRAY_TYPE_NULL,
    FILEARRAY_TYPE_FILE,    //文件类型
    FILEARRAY_TYPE_FOLDER,   //文件夹类型
	FILEARRAY_TYPE_DEFAULT_FOLDER//用户预设路径

}VIRTUALARRAY_TYPE_E;

typedef struct  
{
    wchar       filename[MMIFILE_FULL_PATH_MAX_LEN+1]; //文件名或文件夹名
    uint16      name_len;
#ifdef BAIDU_DRIVE_SUPPORT
    wchar      fs_id[MMIFILE_FS_ID_LEN_MAX+1];    //LEN MAX 64
#endif
    SCI_DATE_T  modify_date;    //修改日期
    SCI_TIME_T  modify_time;    //修改时间
    SCI_DATE_T  create_date;    //创建日期
    SCI_TIME_T  create_time;    //创建时间
    uint32      size;   //文件大小
    VIRTUALARRAY_TYPE_E type;
}VIRTUALARRAY_DATA_T;

extern const wchar g_mmivirtualarray_default_udisk_dir_wstr[];
extern const wchar g_mmivirtualarray_default_sdcard_dir_wstr[];

//虚拟数组类型
typedef void*   VIRTUALARRAY;

#ifndef SQLITE_FILEARRAY_SUPPORT
//排序比较函数, data_1>data_2返回1, data_1=data_2返回0, data_1<data_2返回-1
typedef int (*MMIVIRTUALARRAY_COMPARE)(const void *data_1, const void *data_2);

//extern const wchar g_mmivirtualarray_default_udisk_dir_wstr[];
//extern const wchar g_mmivirtualarray_default_sdcard_dir_wstr[];
//extern const wchar g_mmivirtualarray_default_sdcard1_dir_wstr[];
/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 创建一个虚拟数组
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
VIRTUALARRAY MMIVIRTUALARRAY_Create(    //返回分配的指针
                                uint32      data_size,   //数组中单个数据的大小(字节)
                                const char   *file,
                                uint32       line
                                );

/*****************************************************************************/
//  Description : 销毁一个虚拟数组
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIVIRTUALARRAY_Destroy(
                        VIRTUALARRAY    *virtual_array_ptr      //out, 清空指针
                        );

/*****************************************************************************/
//  Description : 获得虚拟数组大小
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArraySize(    //返回虚拟数组大小
                        VIRTUALARRAY    virtual_array       //in
                        );
/*****************************************************************************/
//  Description : 获得虚拟数组大小
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy MMIVIRTUALARRAY_GetArraySize liyan modify for CR153633
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArrayCurSize(    //返回虚拟数组大小
									   VIRTUALARRAY    virtual_array       //in
									);
/*****************************************************************************/
//  Description : 在虚拟数组中增加一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Add(    //返回True成功或False失败
                        VIRTUALARRAY    virtual_array,      //in
                        void    *data_ptr   //in, 要增加的数据指针
                        );

/*****************************************************************************/
//  Description : 判断是否由于磁盘空间不足而提前结束
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIVIRTUALARRAY_GetFileErrorType(    //返回True由于磁盘空间不足而提前结束
                        VIRTUALARRAY    virtual_array//IN
						);

/*****************************************************************************/
//  Description : 出错的存储器类型
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIVIRTUALARRAY_GetFileDev(    //返回出错的存储器类型
                        VIRTUALARRAY    virtual_array//IN
						);

/*****************************************************************************/
//  Description : 从虚拟数组中读取一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Read(   //返回True成功或False失败
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, 数据索引号，0到total_size-1
                        void        *data_ptr   //out, 存放读取的数据
                        );

/*****************************************************************************/
//  Description : 在虚拟数组中修改一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Modify( //返回True成功或False失败
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, 数据索引号，0到total_size-1
                        void        *data_ptr   //in, 存放修改的数据
                        );

/*****************************************************************************/
//  Description : 在虚拟数组中删除一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Delete( //返回True成功或False失败
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index       //in, 数据索引号，0到total_size-1
                        );

/*****************************************************************************/
//  Description : 对虚拟数组进行排序
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Sort(   //返回True成功或False失败
                        VIRTUALARRAY        virtual_array,  //in
                        MMIVIRTUALARRAY_COMPARE  compare_func   //in, 比较函数
                        );

/*****************************************************************************/
//  Description : 从指定文件中初始化filearray数组，和MMIFILEARRAY_SaveArrayToFile
//                函数相对应
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_LoadArrayFromFile(
                                     VIRTUALARRAY    virtual_array,   
                                     uint16     *file_name,     //要读取的文件名
                                     uint16     file_name_len,
									 uint32     file_offset
                                     );

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SaveArrayToFile(
                                     VIRTUALARRAY    virtual_array,
                                     uint16     *file_name,     //要保存到的文件名, 文件一定要存在
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : 把filearray数组保存到指定文件中，关机后也不会丢失
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIVIRTUALARRAY_SaveArrayToFileEx(
                                     VIRTUALARRAY    virtual_array,
                                     uint16     *file_name,     //要保存到的文件名, 文件一定要存在
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : 设置folder array中每个文件夹的子文件在据filearray中的开始位置和结束位置  
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SetFolderStartFileAndEndFile(
                                     //VIRTUALARRAY folder_array,
									 VIRTUALARRAY file_array
                                     );

/*****************************************************************************/
//  Description : 读取该文件夹中第一个或最后文件在filearray的index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetFilePos(  //返回文件夹中第一个文件在filearray的位置
                        VIRTUALARRAY  virtual_array,     //in, filearray值
                        uint32      index,   //in, 文件夹索引号，0到total_size-1
						BOOLEAN     is_first//IN:TRUE该文件夹中第一个文件在filearray的index
						                    //   FALSE该文件夹中最后一个文件在filearray的index
                        );

/*****************************************************************************/
//  Description : 从虚拟数组中读取一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_ReadFolder(   //返回True成功或False失败
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, 数据索引号，0到total_size-1
                        void        *data_ptr   //out, 存放读取的数据
                        );

/*****************************************************************************/
//  Description : 文件夹的个数
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetTotalFolderCount(   
                        VIRTUALARRAY    virtual_array
                        );

/*****************************************************************************/
//  Description : 将folder array纪录到base_virtual_array中
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
void MMIVIRTUALARRAY_AddFolderArray(
                                     VIRTUALARRAY    base_virtual_array,
                                     VIRTUALARRAY    folder_virtual_array
                                     );

/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SetBufferSize(
                                     VIRTUALARRAY    base_virtual_array,
                                     uint32          size           //counted in KB
                                     );

/*****************************************************************************/
//  Description : get all data size
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVIRTUALARRAY_GetAllDataSize(
                                                     VIRTUALARRAY virtual_array  //in
                                                     );
#else
/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_Create
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC VIRTUALARRAY  MMIVIRTUALARRAY_Create(void);

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_Destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIVIRTUALARRAY_Destroy(
									VIRTUALARRAY* virtual_array_ptr
									);

/*****************************************************************************/
//  Description : get FILE_ARRAY size
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVIRTUALARRAY_GetSize(
									  VIRTUALARRAY virtual_array
									  );

/*****************************************************************************/
//  Description : 获取错误类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIVIRTUALARRAY_GetErrorType(
													VIRTUALARRAY virtual_array
													);

/*****************************************************************************/
//  Description : 获取存储器类型
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIVIRTUALARRAY_GetFileStorageDev(
													  VIRTUALARRAY virtual_array
													  );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_AddItem
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_AddItem(
									   VIRTUALARRAY virtual_array,
									   void *date_ptr
									   );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_RemoveItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_RemoveItemById(
											  VIRTUALARRAY virtual_array,
											  uint32 index
											  );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_UpdateItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_UpdateItemById(
											  VIRTUALARRAY virtual_array,
											  uint32 index,
											  void *date_ptr
											  );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_GetItemById
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_GetItemById(
										   VIRTUALARRAY virtual_array,
										   uint32 index,
										   void *date_ptr
										   );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_SetSort
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_SetSort(
									   VIRTUALARRAY virtual_array,
									   int32 sort_type
									   );

/*****************************************************************************/
//  Description : MMIVIRTUALARRAY_Combine
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIVIRTUALARRAY_Combine(
									   VIRTUALARRAY array1,
									   VIRTUALARRAY array2
									   );

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
												  );

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
											);

PUBLIC BOOLEAN MMIVIRTUALARRAY_Transaction(
										   VIRTUALARRAY v_array, 
										   BOOLEAN begin
										   );


/*****************************************************************************/
//  Description : get all data size
//  Global resource dependence : 
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVIRTUALARRAY_GetAllDataSize(
                                                     VIRTUALARRAY virtual_array  //in
                                                     );
#endif //_MMIVIRTUALARRAY_H_
#endif



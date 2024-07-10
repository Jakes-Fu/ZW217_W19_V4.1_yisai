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
    FILEARRAY_TYPE_FILE,    //�ļ�����
    FILEARRAY_TYPE_FOLDER,   //�ļ�������
	FILEARRAY_TYPE_DEFAULT_FOLDER//�û�Ԥ��·��

}VIRTUALARRAY_TYPE_E;

typedef struct  
{
    wchar       filename[MMIFILE_FULL_PATH_MAX_LEN+1]; //�ļ������ļ�����
    uint16      name_len;
#ifdef BAIDU_DRIVE_SUPPORT
    wchar      fs_id[MMIFILE_FS_ID_LEN_MAX+1];    //LEN MAX 64
#endif
    SCI_DATE_T  modify_date;    //�޸�����
    SCI_TIME_T  modify_time;    //�޸�ʱ��
    SCI_DATE_T  create_date;    //��������
    SCI_TIME_T  create_time;    //����ʱ��
    uint32      size;   //�ļ���С
    VIRTUALARRAY_TYPE_E type;
}VIRTUALARRAY_DATA_T;

extern const wchar g_mmivirtualarray_default_udisk_dir_wstr[];
extern const wchar g_mmivirtualarray_default_sdcard_dir_wstr[];

//������������
typedef void*   VIRTUALARRAY;

#ifndef SQLITE_FILEARRAY_SUPPORT
//����ȽϺ���, data_1>data_2����1, data_1=data_2����0, data_1<data_2����-1
typedef int (*MMIVIRTUALARRAY_COMPARE)(const void *data_1, const void *data_2);

//extern const wchar g_mmivirtualarray_default_udisk_dir_wstr[];
//extern const wchar g_mmivirtualarray_default_sdcard_dir_wstr[];
//extern const wchar g_mmivirtualarray_default_sdcard1_dir_wstr[];
/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ����һ����������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
VIRTUALARRAY MMIVIRTUALARRAY_Create(    //���ط����ָ��
                                uint32      data_size,   //�����е������ݵĴ�С(�ֽ�)
                                const char   *file,
                                uint32       line
                                );

/*****************************************************************************/
//  Description : ����һ����������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIVIRTUALARRAY_Destroy(
                        VIRTUALARRAY    *virtual_array_ptr      //out, ���ָ��
                        );

/*****************************************************************************/
//  Description : ������������С
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArraySize(    //�������������С
                        VIRTUALARRAY    virtual_array       //in
                        );
/*****************************************************************************/
//  Description : ������������С
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy MMIVIRTUALARRAY_GetArraySize liyan modify for CR153633
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArrayCurSize(    //�������������С
									   VIRTUALARRAY    virtual_array       //in
									);
/*****************************************************************************/
//  Description : ����������������һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Add(    //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        void    *data_ptr   //in, Ҫ���ӵ�����ָ��
                        );

/*****************************************************************************/
//  Description : �ж��Ƿ����ڴ��̿ռ䲻�����ǰ����
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIVIRTUALARRAY_GetFileErrorType(    //����True���ڴ��̿ռ䲻�����ǰ����
                        VIRTUALARRAY    virtual_array//IN
						);

/*****************************************************************************/
//  Description : ����Ĵ洢������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIVIRTUALARRAY_GetFileDev(    //���س���Ĵ洢������
                        VIRTUALARRAY    virtual_array//IN
						);

/*****************************************************************************/
//  Description : �����������ж�ȡһ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Read(   //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, ���������ţ�0��total_size-1
                        void        *data_ptr   //out, ��Ŷ�ȡ������
                        );

/*****************************************************************************/
//  Description : �������������޸�һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Modify( //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, ���������ţ�0��total_size-1
                        void        *data_ptr   //in, ����޸ĵ�����
                        );

/*****************************************************************************/
//  Description : ������������ɾ��һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Delete( //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index       //in, ���������ţ�0��total_size-1
                        );

/*****************************************************************************/
//  Description : �����������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Sort(   //����True�ɹ���Falseʧ��
                        VIRTUALARRAY        virtual_array,  //in
                        MMIVIRTUALARRAY_COMPARE  compare_func   //in, �ȽϺ���
                        );

/*****************************************************************************/
//  Description : ��ָ���ļ��г�ʼ��filearray���飬��MMIFILEARRAY_SaveArrayToFile
//                �������Ӧ
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_LoadArrayFromFile(
                                     VIRTUALARRAY    virtual_array,   
                                     uint16     *file_name,     //Ҫ��ȡ���ļ���
                                     uint16     file_name_len,
									 uint32     file_offset
                                     );

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SaveArrayToFile(
                                     VIRTUALARRAY    virtual_array,
                                     uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIVIRTUALARRAY_SaveArrayToFileEx(
                                     VIRTUALARRAY    virtual_array,
                                     uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : ����folder array��ÿ���ļ��е����ļ��ھ�filearray�еĿ�ʼλ�úͽ���λ��  
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SetFolderStartFileAndEndFile(
                                     //VIRTUALARRAY folder_array,
									 VIRTUALARRAY file_array
                                     );

/*****************************************************************************/
//  Description : ��ȡ���ļ����е�һ��������ļ���filearray��index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetFilePos(  //�����ļ����е�һ���ļ���filearray��λ��
                        VIRTUALARRAY  virtual_array,     //in, filearrayֵ
                        uint32      index,   //in, �ļ��������ţ�0��total_size-1
						BOOLEAN     is_first//IN:TRUE���ļ����е�һ���ļ���filearray��index
						                    //   FALSE���ļ��������һ���ļ���filearray��index
                        );

/*****************************************************************************/
//  Description : �����������ж�ȡһ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_ReadFolder(   //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index,      //in, ���������ţ�0��total_size-1
                        void        *data_ptr   //out, ��Ŷ�ȡ������
                        );

/*****************************************************************************/
//  Description : �ļ��еĸ���
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetTotalFolderCount(   
                        VIRTUALARRAY    virtual_array
                        );

/*****************************************************************************/
//  Description : ��folder array��¼��base_virtual_array��
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
//  Description : ��ȡ��������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIVIRTUALARRAY_GetErrorType(
													VIRTUALARRAY virtual_array
													);

/*****************************************************************************/
//  Description : ��ȡ�洢������
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
//  Description : ���һ��FILE_ARRAY���ػ��󲻻ᶪʧ
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
//  Description : ��ָ���ļ��г�ʼ��FILE_ARRAY
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



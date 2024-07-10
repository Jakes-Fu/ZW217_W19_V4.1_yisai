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
#define FILEARRAY_FILENAME_MAX_LEN      MMIFILE_FULL_PATH_MAX_LEN   //ȫ·������󳤶�
#define FILEARRAY_ITEM_MIN_NUM          50 //�ڴ滺������С��50������
#define FILEARRAY_MAX_BUFFER_NUMBER     100


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
//�����ļ�����
typedef void*   FILEARRAY;
typedef VIRTUALARRAY_TYPE_E FILEARRAY_TYPE_E;
//typedef enum
//{
//    FILEARRAY_TYPE_NULL,
//    FILEARRAY_TYPE_FILE,    //�ļ�����
//    FILEARRAY_TYPE_FOLDER   //�ļ�������
// }FILEARRAY_TYPE_E;
//
////�������ݽṹ
//typedef struct  
//{
//    wchar       filename[FILEARRAY_FILENAME_MAX_LEN+1]; //�ļ������ļ�����
//    uint16      name_len;
//    SCI_DATE_T  modify_date;    //�޸�����
//    SCI_TIME_T  modify_time;    //�޸�ʱ��
//    uint32      size;   //�ļ���С
//    FILEARRAY_TYPE_E    type;
// }FILEARRAY_DATA_T;

typedef VIRTUALARRAY_DATA_T FILEARRAY_DATA_T;
//��������ʽ
typedef enum
{
    FILEARRAY_SORT_NULL,
    FILEARRAY_SORT_NAME_ASCEND,
    FILEARRAY_SORT_NAME_DESCEND,
    FILEARRAY_SORT_TIME_ASCEND,
    FILEARRAY_SORT_TIME_DESCEND,
    FILEARRAY_SORT_ATTR_ASCEND,//���� ��
    FILEARRAY_SORT_ATTR_DESCEND,
    FILEARRAY_SORT_TIME_CREATE_ASCEND,//����ʱ����
    FILEARRAY_SORT_TIME_CREATE_DESCEND,
    FILEARRAY_SORT_TIME_MODIFY_ASCEND,//�޸�ʱ����
    FILEARRAY_SORT_TIME_MODIFY_DESCEND,
    FILEARRAY_SORT_TIME_ASK_ASCEND,//����ʱ����
    FILEARRAY_SORT_TIME_ASK_DESCEND,
    FILEARRAY_SORT_SIZE_ASCEND,
    FILEARRAY_SORT_SIZE_DESCEND,
    FILEARRAY_SORT_TYPE_ASCEND,
    FILEARRAY_SORT_TYPE_DESCEND,
    FILEARRAY_SORT_TYPE_CUST  //�û��Զ�������ʽ
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
//  Description : ����һ��filearray����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIFA_CreateArray(
                                      const char *file,
                                      uint32   line
                                      ); //���ط����ָ��

/*****************************************************************************/
//  Description : ����һ��filearray����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFA_DestroyArray(
                                    FILEARRAY   *file_array_ptr,     //out, ���ָ��
                                    const char          *file,
                                    uint32              line
                                     );

/*****************************************************************************/
//  Description : ���filearray�����С
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArraySize(   //����filearray�����С
                        FILEARRAY   file_array      //in, filearrayֵ
                        );
/*****************************************************************************/
//  Description : ���filearray�����С
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy  MMIAPIFILEARRAY_GetArraySize liyan modify for CR153633
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArrayCurSize(   //����filearray�����С
									   FILEARRAY   file_array      //in, filearrayֵ
									);
/*****************************************************************************/
//  Description : �ж��Ƿ����ڴ��̿ռ䲻�����ǰ����
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_GetFileErrorType(
                            FILEARRAY   file_array
                            );

/*****************************************************************************/
//  Description : ����Ĵ洢������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIAPIFILEARRAY_GetFileDev(    //���س���Ĵ洢������
                         FILEARRAY   file_array//IN
						);

/*****************************************************************************/
//  Description : ��filearray����������һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Add(   //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        FILEARRAY_DATA_T    *data_ptr   //in, Ҫ���ӵ�����ָ��
                        );

/*****************************************************************************/
//  Description : ��filearray�����ж�ȡһ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Read(  //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index,   //in, ���������ţ�0��total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //out, ��Ŷ�ȡ������
                        );

/*****************************************************************************/
//  Description : ��filearray�������޸�һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Modify(    //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index,   //in, ���������ţ�0��total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //in, ����޸ĵ�����
                        );

/*****************************************************************************/
//  Description : ��filearray������ɾ��һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Delete(    //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, ���������ţ�0��total_size-1
                        );
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : ��filearray�����������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Sort(  //����True�ɹ���Falseʧ��
                        FILEARRAY       file_array, //in, filearrayֵ
                        FILEARRAY_SORT_E    sort_type   //in, ��������
                        );
#endif
/*****************************************************************************/
//  Description : �������filearray����, 
//                [file_array_1] + [file_array_2] -> [file_array_1]
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Combine(   //����True�ɹ���Falseʧ��
                            FILEARRAY   file_array_1,       //in/out, filearray 1
                            FILEARRAY   file_array_2        //in, filearray 2
                            );

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SaveArrayToFile(
                                     FILEARRAY  file_array,     //in, filearrayֵ
                                     uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_SaveArrayToFileEx(
                                     FILEARRAY  file_array,     //in, filearrayֵ
                                     uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : ��ָ���ļ��г�ʼ��filearray���飬��MMIFILEARRAY_SaveArrayToFile
//                �������Ӧ
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_LoadArrayFromFile(
                                     FILEARRAY  file_array,     //in, filearrayֵ
                                     uint16     *file_name,     //Ҫ��ȡ���ļ���
                                     uint16     file_name_len
                                     );

/*****************************************************************************/
//  Description : ����folder array��ÿ���ļ��е����ļ��ھ�filearray�еĿ�ʼλ�úͽ���λ��             
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SetFolderStartFileAndEndFile(
                                     //FILEARRAY folder_array,
									 FILEARRAY file_array
                                     );

/*****************************************************************************/
//  Description : ��ȡ���ļ����е�һ���ļ���filearray��index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetFirstFilePos(  //�����ļ����е�һ���ļ���filearray��λ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, �ļ��������ţ�0��total_size-1
                        );

/*****************************************************************************/
//  Description : ��ȡ���ļ��������һ���ļ���filearray��index
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetLastFilePos(  //�����ļ��������һ���ļ���filearray��λ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, �ļ��������ţ�0��total_size-1                   
                        );


/*****************************************************************************/
//  Description : ��folder array��¼��base_virtual_array��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
void MMIAPIFILEARRAY_AddFolderArray(
                                     VIRTUALARRAY    base_virtual_array,
                                     VIRTUALARRAY    folder_virtual_array
                                     );

/*****************************************************************************/
//  Description : ���ļ��������ж�ȡһ���ļ�������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_ReadFolder(  //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index,   //in, ���������ţ�0��total_size-1
                        FILEARRAY_DATA_T    *data_ptr  //out, ��Ŷ�ȡ������
                        );

/*****************************************************************************/
//  Description : �ļ��еĸ���
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
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
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

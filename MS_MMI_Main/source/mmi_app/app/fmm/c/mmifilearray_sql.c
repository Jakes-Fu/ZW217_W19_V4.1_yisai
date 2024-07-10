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
//  Description : ���filearray�����С
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFILEARRAY_GetArraySize(
										FILEARRAY   file_array      //in, filearrayֵ
										)
{
	if (PNULL == file_array)
	{
		return 0;
	}
	
	return MMIVIRTUALARRAY_GetSize(file_array);
}

/*****************************************************************************/
//  Description : ��ȡ�ļ���������
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
//  Description : ��ȡ�洢������
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
//  Description : ��filearray����������һ������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Add(
						 FILEARRAY			file_array,     //in, filearrayֵ
						 FILEARRAY_DATA_T   *data_ptr   //in, Ҫ���ӵ�����ָ��
						 )
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_AddItem(file_array, data_ptr);	
}

/*****************************************************************************/
//  Description : ��filearray�����ж�ȡһ������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Read(
						  FILEARRAY   file_array,     //in, filearrayֵ
						  uint32      index,   //in, ���������ţ�0��total_size-1
						  FILEARRAY_DATA_T    *data_ptr  //out, ��Ŷ�ȡ������
						  )
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_GetItemById(file_array, index, data_ptr);				
}

/*****************************************************************************/
//  Description : ��filearray�������޸�һ������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Modify( 
							FILEARRAY   file_array,     //in, filearrayֵ
							uint32      index,			//in, ���������ţ�0��total_size-1
							FILEARRAY_DATA_T    *data_ptr  //in, ����޸ĵ�����
							)
{
	if(NULL == file_array|| NULL == data_ptr)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_UpdateItemById(file_array, index, data_ptr);	
}

/*****************************************************************************/
//  Description : ��filearray������ɾ��һ������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Delete(
							FILEARRAY   file_array,     //in, filearrayֵ
							uint32      index			//in, ���������ţ�0��total_size-1
							)
{
	if(NULL == file_array)
    {
        return FALSE;
    }
	
	return MMIVIRTUALARRAY_RemoveItemById(file_array, index);		
}

/*****************************************************************************/
//  Description : ��filearray�����������
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Sort(
						  FILEARRAY			file_array, //in, filearrayֵ
						  FILEARRAY_SORT_E  sort_type   //in, ��������
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
//  Description : �ϲ�����filearray����
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
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SaveArrayToFile(
                                     FILEARRAY  file_array,     //in, filearrayֵ
                                     uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
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
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_SaveArrayToFileEx(
											   FILEARRAY  file_array,     //in, filearrayֵ
											   uint16     *file_name,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
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
//  Description : ��ָ���ļ��г�ʼ��filearray����
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_LoadArrayFromFile(
									   FILEARRAY  file_array,     //in, filearrayֵ
									   uint16     *file_name,     //Ҫ��ȡ���ļ���
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
//  Description : ����folder array��ÿ���ļ��е����ļ��ھ�filearray�еĿ�ʼλ�úͽ���λ��             
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
//  Description : ��folder array��¼��base_virtual_array��
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
//  Description : ��folder array��¼��base_virtual_array��
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
//  Description : ��ȡ���ļ����е�һ���ļ���filearray��index
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetFirstFilePos(  //�����ļ����е�һ���ļ���filearray��λ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, �ļ��������ţ�0��total_size-1
                        )
{
	return 0;
}

/*****************************************************************************/
//  Description : ��ȡ���ļ��������һ���ļ���filearray��index
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetLastFilePos(  //�����ļ��������һ���ļ���filearray��λ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, �ļ��������ţ�0��total_size-1                   
                        )
{
    return 0;

}

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
                        )
{
    BOOLEAN     result = FALSE;

    return result;
}
/*****************************************************************************/
//  Description : �ļ��еĸ���
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

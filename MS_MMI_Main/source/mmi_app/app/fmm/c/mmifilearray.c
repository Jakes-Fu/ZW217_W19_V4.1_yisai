/*****************************************************************************
** File Name:      mmifilearray_export.c                                            *
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


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifilearray_export.h"
#include "mmivirtualarray.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmifmm_export.h"  //cr219808

/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define FILEARRAY_SAVEFILE_IDENT        "FILEARRAY SAVEFILE 01.00.01"  //�洢�ļ���ʽ�汾��
#define FILEARRAY_SAVEFILE_IDENT_LEN    27


/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : ������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameAsce(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

/*****************************************************************************/
//  Description : ���ֽ�������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareFileType(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

/*****************************************************************************/
//  Description : �ļ�������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeAsce(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

/*****************************************************************************/
//  Description : �ļ����ͽ�������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );
/*****************************************************************************/
//  Description : asecend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
//LOCAL int CompareCTimeAsec(
//                           const void       *data_1_ptr,
//                           const void       *data_2_ptr
//                            );
/*****************************************************************************/
//  Description : descend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
//LOCAL int CompareCTimeDesc(
//                           const void       *data_1_ptr,
//                           const void       *data_2_ptr
//                            );

LOCAL int CompareMTimeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

LOCAL int CompareMTimeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

LOCAL int CompareCTimeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

LOCAL int CompareCTimeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );
LOCAL int CompareSizeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );

LOCAL int CompareSizeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           );
#endif
/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : file array init
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFILEARRAY_Init(void)
{
    wchar            default_dir[MMIFILE_FILE_NAME_MAX_LEN] = {0};
	uint16           device_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    MMIFILE_ERROR_E  result = 0;

    device_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);
    MMIAPICOM_Wstrncpy(default_dir, MMIAPIFMM_GetDevicePath(fmm_dev), MIN(device_len, MMIFILE_FILE_NAME_MAX_LEN));
    if(device_len < MMIFILE_FILE_NAME_MAX_LEN)
    {
        default_dir[device_len] = ':';
    }
    
    result =  MMIAPIFMM_DeleteTreeEx(default_dir,MMIMULTIM_DIR_FILEARRAY,FALSE);
	
    SCI_MEMSET(default_dir,0x00,(sizeof(wchar)*MMIFILE_FILE_NAME_MAX_LEN));
    device_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(fmm_dev), 
        MMIAPIFMM_GetDevicePathLen(fmm_dev),
		MMIMULTIM_DIR_FILEARRAY, 
        MMIAPICOM_Wstrlen(MMIMULTIM_DIR_FILEARRAY),
		PNULL, 0,
 		default_dir, &device_len);
    if (SFS_ERROR_NONE != result )
    {
        MMIAPIFMM_CreateDirectory(default_dir);
        
        MMIAPIFMM_SetAttr(default_dir,device_len, TRUE, FALSE, FALSE, FALSE);
    }
}
/*****************************************************************************/
//  Description : ����һ��filearray����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIFA_CreateArray(
                                      const char *file,
                                      uint32   line
                                      ) //���ط����ָ��
{
    FILEARRAY   file_array = NULL;

    file_array = MMIVIRTUALARRAY_Create(sizeof(FILEARRAY_DATA_T),file,line);

    SCI_TRACE_LOW("MMIAPIFA_CreateArray, file_array = %x,file=%s,line=%d",file_array,file,line);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_112_112_2_18_2_20_3_202,(uint8*)"d", file_array);

    //SCI_ASSERT(NULL != file_array);/* assert to do */

    return file_array;
}

/*****************************************************************************/
//  Description : ����һ��filearray����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFA_DestroyArray(
                                    FILEARRAY   *file_array_ptr,     //out, ���ָ��
                                    const char   *file,
                                    uint32       line
                                  )
{
    if(NULL == file_array_ptr
        || NULL == *file_array_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("MMIAPIFA_DestroyArray, *file_array_ptr = %x,file=%s,line=%d",*file_array_ptr,file,line);
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_133_112_2_18_2_20_3_203,(uint8*)"d", *file_array_ptr);

    MMIVIRTUALARRAY_Destroy(file_array_ptr);
}

/*****************************************************************************/
//  Description : ���filearray�����С
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArraySize(   //����filearray�����С
                        FILEARRAY   file_array      //in, filearrayֵ
                        )
{
    if(NULL == file_array)
    {
        return 0;
    }

    return MMIVIRTUALARRAY_GetArraySize(file_array);
}
/*****************************************************************************/
//  Description : ���filearray�����С
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy  MMIAPIFILEARRAY_GetArraySize for CR153633
/*****************************************************************************/
uint32 MMIAPIFILEARRAY_GetArrayCurSize(   //����filearray�����С
									FILEARRAY   file_array      //in, filearrayֵ
									)
{
    if(NULL == file_array)
    {
        return 0;
    }
	
    return MMIVIRTUALARRAY_GetArrayCurSize(file_array);
}

/*****************************************************************************/
//  Description : ��filearray����������һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Add(   //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        FILEARRAY_DATA_T    *data_ptr   //in, Ҫ���ӵ�����ָ��
                        )
{
    BOOLEAN     result = FALSE;

    if(NULL == file_array
        || NULL == data_ptr)
    {
        return FALSE;
    }
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );

    result = MMIVIRTUALARRAY_Add(file_array, data_ptr);

    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );

    return result;
}

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
                        )
{
    BOOLEAN     result = FALSE;

    if(NULL == file_array
        || NULL == data_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Read(file_array, index, data_ptr);
    if(data_ptr->name_len == 0 || data_ptr->name_len > MMIFILE_FULL_PATH_MAX_LEN)
    {
        //SCI_TRACE_LOW:"[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_197_112_2_18_2_20_3_204,(uint8*)"d", data_ptr->name_len);
        result = FALSE;
    }
    SCI_TRACE_LOW("MMIAPIFILEARRAY_Read result = %d",result);
    return result;
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

    if(NULL == file_array
        || NULL == data_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_ReadFolder(file_array, index, data_ptr);
    if(data_ptr->name_len == 0 || data_ptr->name_len > MMIFILE_FULL_PATH_MAX_LEN)
	{
		//SCI_TRACE_LOW:"[FMM] MMIAPIFILEARRAY_Read data_ptr->name_len %d !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_224_112_2_18_2_20_3_205,(uint8*)"d", data_ptr->name_len);
		result = FALSE;
	}
    return result;
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
	return MMIVIRTUALARRAY_GetFilePos(file_array, index, TRUE);
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
    return MMIVIRTUALARRAY_GetFilePos(file_array, index, FALSE);

}

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
                        )
{
    BOOLEAN     result = FALSE;

    if(NULL == file_array
        || NULL == data_ptr)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Modify(file_array, index, data_ptr);

    return result;
}

/*****************************************************************************/
//  Description : ��filearray������ɾ��һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Delete(    //����True�ɹ���Falseʧ��
                        FILEARRAY   file_array,     //in, filearrayֵ
                        uint32      index   //in, ���������ţ�0��total_size-1
                        )
{
    BOOLEAN     result = FALSE;

    if(NULL == file_array)
    {
        return FALSE;
    }

    result = MMIVIRTUALARRAY_Delete(file_array, index);

    return result;
}

/*****************************************************************************/
//  Description : ��filearray�����������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_Sort(  //����True�ɹ���Falseʧ��
                        FILEARRAY       file_array, //in, filearrayֵ
                        FILEARRAY_SORT_E    sort_type   //in, ��������
                        )
{
    BOOLEAN     result = FALSE;
    MMIVIRTUALARRAY_COMPARE  compare_func = NULL;

    if(NULL == file_array
        || FILEARRAY_SORT_NULL == sort_type)
    {
        return FALSE;
    }
    switch(sort_type)
    {
#ifdef MMI_FMM_SORT_API_EXT_SUPPORT
    case FILEARRAY_SORT_NAME_ASCEND:
        compare_func = CompareNameAsce;
        break;

    case FILEARRAY_SORT_NAME_DESCEND:
        compare_func = CompareNameDesc;
        break;

    case FILEARRAY_SORT_TYPE_ASCEND:
        compare_func = CompareTypeAsce;
        break;

    case FILEARRAY_SORT_TYPE_DESCEND:
        compare_func = CompareTypeDesc;
        break;
    case FILEARRAY_SORT_TIME_ASCEND:
	    compare_func = CompareMTimeAsec ;
	    break;
    case FILEARRAY_SORT_TIME_DESCEND:
		compare_func = CompareMTimeDesc ;
	    break;

    case FILEARRAY_SORT_TIME_CREATE_ASCEND:
	    compare_func = CompareCTimeAsec ;
	    break;

    case FILEARRAY_SORT_TIME_CREATE_DESCEND:
		compare_func = CompareCTimeDesc ;
 	    break;
    case FILEARRAY_SORT_SIZE_ASCEND:
		compare_func = CompareSizeAsec ;
	    break;
	
    case FILEARRAY_SORT_SIZE_DESCEND:
		compare_func = CompareSizeDesc ;
	    break;
#endif
    default:
        break;
    }
#ifdef MMI_FMM_SORT_API_EXT_SUPPORT
	result = MMIVIRTUALARRAY_Sort(file_array, compare_func);
#endif
    return result;

}

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
                            )
{
    BOOLEAN     result = TRUE;
    uint16      file_2_total = 0;
    int16       i = 0;
    FILEARRAY_DATA_T    file_data = {0};

    if(NULL == file_array_1 || NULL == file_array_2)
    {
        return FALSE;
    }

    file_2_total = MMIAPIFILEARRAY_GetArraySize(file_array_2);

    for (i=0; i<file_2_total; i++)
    {
        SCI_MEMSET(&file_data, 0x00, sizeof(file_data));
        if (!MMIAPIFILEARRAY_Read(file_array_2, i, &file_data))
        {
            result = FALSE;
            break;
        }

        if (!MMIAPIFILEARRAY_Add(file_array_1, &file_data))
        {
            result = FALSE;
            break;
        }
    }

    return result;
}

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
                                     )
{
    BOOLEAN     result = FALSE;
    //modified by liyan.zhu for c++ test
	//SFS_ERROR_E  file_ret = SFS_ERROR_NONE;
    SFS_HANDLE  sfs_handle = NULL;
    //uint32      write_size = 0;


    if(NULL == file_array
        || NULL == file_name
        || 0 == file_name_len)
    {
        return FALSE;
    }

    //�����ļ�
    sfs_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        SFS_ERROR_E  file_ret = SFS_ERROR_NONE;
        uint32      write_size = 0;
		//clean this file
		SFS_SetFileSize(sfs_handle, 0);
        //д�ļ���ʶ
		file_ret = SFS_WriteFile(sfs_handle, FILEARRAY_SAVEFILE_IDENT, FILEARRAY_SAVEFILE_IDENT_LEN, &write_size, NULL);
		
        SFS_CloseFile(sfs_handle);
        if(SFS_ERROR_NONE == file_ret)
        {
			result = MMIVIRTUALARRAY_SaveArrayToFile(file_array, file_name, file_name_len);
		}
    }
    return result;
}

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
                                     )
{
    MMIFILE_ERROR_E result = SFS_ERROR_NONE;
	//SFS_ERROR_E  file_ret = SFS_ERROR_NONE;
    SFS_HANDLE  sfs_handle = NULL;
    
    //add for cr219808
    //modified by liyan.zhu for c++ test
    //uint32      write_size = 0;  
    //uint32      file_array_size = 0;
    //uint32      file_array_total_size = 0;
    //MMIFILE_DEVICE_E file_dev   =  MMI_DEVICE_UDISK;

    if(NULL == file_array
        || NULL == file_name
        || 0 == file_name_len)
    {
        return SFS_ERROR_INVALID_PARAM;
    }

    //�����ļ�
    sfs_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        uint32      file_array_size = 0;
        uint32      file_array_total_size = 0;
        file_array_size = MMIAPIFILEARRAY_GetArraySize(file_array);
        
        file_array_total_size = file_array_size * sizeof(FILEARRAY_DATA_T) + FILEARRAY_SAVEFILE_IDENT_LEN;
            
        
        if((file_array_total_size >> 10) >=  MMI_GetFreeSpace(MMIAPIFMM_GetDeviceTypeByPath(file_name, MMIFILE_DEVICE_LEN)))
        {
            MMIAPIFMM_CloseFile(sfs_handle);
            result = SFS_ERROR_NO_SPACE;   //changed by sandy for cr246623 from SFS_ERROR_NO_ENOUGH_RESOURCE to SFS_ERROR_NO_SPACE,Ӧ����ʾ�ռ䲻�㣻
        }
        else
        {
            uint32      write_size = 0;  
            //clean this file
    		SFS_SetFileSize(sfs_handle, 0);

            //д�ļ���ʶ
    		result = SFS_WriteFile(sfs_handle, FILEARRAY_SAVEFILE_IDENT, FILEARRAY_SAVEFILE_IDENT_LEN, &write_size, NULL);
    		
            SFS_CloseFile(sfs_handle);
            if(SFS_ERROR_NONE == result)
            {
    			result = MMIVIRTUALARRAY_SaveArrayToFileEx(file_array, file_name, file_name_len);
    		}
        }

    }
	else
	{
		result =  SFS_ERROR_INVALID_PARAM;
	}
    return result;
}

/*****************************************************************************/
//  Description : 
//                [file_array_1] + [file_array_2] -> [file_array_1]
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIAPIFILEARRAY_GetFileErrorType(
                            FILEARRAY   file_array
                            )
{
     if(NULL == file_array)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
    return MMIVIRTUALARRAY_GetFileErrorType(file_array);
}

/*****************************************************************************/
//  Description : ����Ĵ洢������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIAPIFILEARRAY_GetFileDev(    //���س���Ĵ洢������
                         FILEARRAY   file_array//IN
						)
{
    return MMIVIRTUALARRAY_GetFileDev(file_array);
}
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
                                     )
{
    BOOLEAN         result = FALSE;
    SFS_HANDLE      sfs_handle = 0;
    //modified by liyan.zhu for c++ test
    //uint32          read_size = 0;
//     uint32          to_read_size = 0;
//  uint32          total_num = 0;
//     FILEARRAY_DATA_T    file_data = {0};
    //uint8           file_ident[FILEARRAY_SAVEFILE_IDENT_LEN+2] = {0};
    //SFS_ERROR_E     file_ret = SFS_ERROR_NONE;

    if(NULL == file_array
        || NULL == file_name
        || 0 == file_name_len)
    {
        return FALSE;
    }

    sfs_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        uint32          read_size = 0;
        uint8           file_ident[FILEARRAY_SAVEFILE_IDENT_LEN+2] = {0};
        SFS_ERROR_E     file_ret = SFS_ERROR_NONE;
        //���ļ���ʶ, ������filearray�����ļ��ı�ʶ, �ż���
        file_ret = SFS_ReadFile(sfs_handle, file_ident, FILEARRAY_SAVEFILE_IDENT_LEN, &read_size, NULL);
        SFS_CloseFile(sfs_handle);
        if(SFS_ERROR_NONE == file_ret)
        {
            if(0 == memcmp(file_ident, FILEARRAY_SAVEFILE_IDENT, FILEARRAY_SAVEFILE_IDENT_LEN))
            {
                result = MMIVIRTUALARRAY_LoadArrayFromFile(file_array, file_name, file_name_len, FILEARRAY_SAVEFILE_IDENT_LEN);
            }
        }
    }

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
   return MMIVIRTUALARRAY_GetTotalFolderCount(file_array);
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
	return MMIVIRTUALARRAY_SetFolderStartFileAndEndFile(file_array );
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
	MMIVIRTUALARRAY_AddFolderArray(base_virtual_array, folder_virtual_array);
}

/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence : 
//  Author: ying.xu
//  Note:
/*****************************************************************************/
BOOLEAN MMIAPIFILEARRAY_SetBufferSize(
                                     FILEARRAY    file_array,
                                     uint32          size           //counted in KB
                                     )
{
    return MMIVIRTUALARRAY_SetBufferSize(file_array, size);
}
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : ������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameAsce(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    int     result = 0;
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);
    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareNameAsce param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_628_112_2_18_2_20_4_206,(uint8*)"");
        return -1;
    }
    
    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;
    
    result = MMIAPICOM_CompareTwoWstrExt(file_data_1->filename, file_data_1->name_len,
        file_data_2->filename, file_data_2->name_len, FALSE);

    if(0 < result)
    {
        return 1;
    }
    else if(0 == result)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/*****************************************************************************/
//  Description : ���ֽ�������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareNameDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    int     result = 0;

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);
    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareNameDesc param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_667_112_2_18_2_20_4_207,(uint8*)"");
        return 1;
    }
    
    result = CompareNameAsce(data_1_ptr, data_2_ptr); 
    if(1 == result)
    {
        return -1;
    }
    else if(0 == result)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareFileType(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    int32               result  = 0;
    uint16              data1_suffix_offset = 0;
    uint16              data2_suffix_offset = 0;
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;

    if((PNULL == data_1_ptr) && (PNULL == data_2_ptr))
    {
        return 0;
    }
    if(PNULL == data_1_ptr)
    {
        return -1;
    }
    if(PNULL == data_2_ptr)
    {
        return 1;
    }

    file_data_1 = (FILEARRAY_DATA_T *) data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T *) data_2_ptr; 

    if(0 == file_data_1->name_len)
    {
        return (int)file_data_1->name_len - (int)file_data_2->name_len;
    }    
    for (data1_suffix_offset = (uint16)(file_data_1->name_len -1); data1_suffix_offset > 0; data1_suffix_offset--)
    {
        if (MMIFILE_DOT == file_data_1->filename[data1_suffix_offset])
        {
            break;
        }
    }
    for (data2_suffix_offset = (uint16)(file_data_2->name_len -1);  data2_suffix_offset > 0; data2_suffix_offset--)
    {
        if (MMIFILE_DOT == file_data_2->filename[data2_suffix_offset])
        {
            break;
        }
    }

    if (0 == data1_suffix_offset  &&  data2_suffix_offset > 0)
    {
        // ��һ���ļ�û�к�׺
        result  = -1;
    }
    else if (0 == data2_suffix_offset  &&  data1_suffix_offset > 0)
    {
        // �ڶ����ļ�û�к�׺
        return 1;
    }
    else if (0 == data1_suffix_offset   && 0 == data2_suffix_offset)
    {
        // ��û�к�׺���Ƚ��ļ���
        result = MMIAPICOM_CompareTwoWstrExt( file_data_1->filename, file_data_1->name_len,
            file_data_2->filename, file_data_2->name_len, FALSE);
    }
    else
    {
        // ���к�׺���ȽϺ�׺
        result = MMIAPICOM_CompareTwoWstrExt( &file_data_1->filename[data1_suffix_offset+1], (uint16)(file_data_1->name_len - data1_suffix_offset-1),
            &file_data_2->filename[data2_suffix_offset+1], (uint16)(file_data_2->name_len- data2_suffix_offset-1),FALSE);

        if(0 == result)
        {
            result = MMIAPICOM_CompareTwoWstrExt( file_data_1->filename, file_data_1->name_len,
                file_data_2->filename, file_data_2->name_len, FALSE);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : �ļ�������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeAsce(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);
    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareTypeAsce param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_778_112_2_18_2_20_4_208,(uint8*)"");
        return 0;
    }

    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;


    if(FILEARRAY_TYPE_FILE == file_data_2->type
        && FILEARRAY_TYPE_FOLDER == file_data_1->type)
    {
        return -1;
    }
    else if(FILEARRAY_TYPE_FILE == file_data_1->type
            && FILEARRAY_TYPE_FOLDER == file_data_2->type)
    {
        return 1;
    }
    else
    {
        return CompareFileType(data_1_ptr, data_2_ptr);
    }
}

/*****************************************************************************/
//  Description : �ļ����ͽ�������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL int CompareTypeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    int     result = 0;

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);

    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareTypeDesc param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_818_112_2_18_2_20_4_209,(uint8*)"");
        return 0;
    }


    result = CompareTypeAsce(data_1_ptr, data_2_ptr); 
    if(1 == result)
    {
        return -1;
    }
    else if(0 == result)
    {
        return CompareNameDesc(data_1_ptr, data_2_ptr);
    }
    else
    {
        return 1;
    }
}
/*****************************************************************************/
//  Description : asecend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
//LOCAL int CompareCTimeAsec(
//                           const void       *data_1_ptr,
//                           const void       *data_2_ptr
//                           )
//{	
//    FILEARRAY_DATA_T    *file_data_1 = NULL;
//    FILEARRAY_DATA_T    *file_data_2 = NULL;	
//
//    //SCI_ASSERT(NULL != data_1_ptr);
//    //SCI_ASSERT(NULL != data_2_ptr);
//    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
//    {
//        //SCI_TRACE_LOW:"CompareCTimeAsec param error!"
//        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_854_112_2_18_2_20_4_210,(uint8*)"");
//        return -1;
//    }
//
//    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
//    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;
//
//
//    if(file_data_1->create_date.year != file_data_2->create_date.year)
//    {
//        return file_data_1->modify_date.year - file_data_2->create_date.year ;
//    }
//    else if( file_data_1->create_date.mon !=  file_data_2->create_date.mon)
//    {
//    return file_data_1->create_date.mon -  file_data_2->create_date.mon ;
//    }
//    else if( file_data_1->create_date.mday !=  file_data_2->create_date.mday)
//    {
//        return file_data_1->create_date.mday -  file_data_2->create_date.mday ;
//    }
//    else if( file_data_1->create_time.hour !=  file_data_2->create_time.hour)
//    {
//        return file_data_1->create_time.hour -  file_data_2->create_time.hour ;
//    }
//    else if( file_data_1->create_time.min !=  file_data_2->create_time.min)
//    {
//        return file_data_1->create_time.min -  file_data_2->create_time.min ;
//    }
//    else if( file_data_1->create_time.sec !=  file_data_2->create_time.sec)
//    {
//        return file_data_1->create_time.sec -  file_data_2->create_time.sec ;
//    }
//    else
//    {
//        return -1;
//    }
//}
//
/*****************************************************************************/
//  Description : descend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
//LOCAL int CompareCTimeDesc(
//                           const void       *data_1_ptr,
//                           const void       *data_2_ptr
//                           )
//{
//	return (0 - CompareCTimeAsec(data_1_ptr, data_2_ptr));
//}
/*****************************************************************************/
//  Description : asecend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareMTimeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{	
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;	

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);
    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareTimeAsec param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_854_112_2_18_2_20_4_210,(uint8*)"");
        return -1;
    }

    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;


    if(file_data_1->modify_date.year != file_data_2->modify_date.year)
    {
        return file_data_1->modify_date.year - file_data_2->modify_date.year ;
    }
    else if( file_data_1->modify_date.mon !=  file_data_2->modify_date.mon)
    {
    return file_data_1->modify_date.mon -  file_data_2->modify_date.mon ;
    }
    else if( file_data_1->modify_date.mday !=  file_data_2->modify_date.mday)
    {
        return file_data_1->modify_date.mday -  file_data_2->modify_date.mday ;
    }
    else if( file_data_1->modify_time.hour !=  file_data_2->modify_time.hour)
    {
        return file_data_1->modify_time.hour -  file_data_2->modify_time.hour ;
    }
    else if( file_data_1->modify_time.min !=  file_data_2->modify_time.min)
    {
        return file_data_1->modify_time.min -  file_data_2->modify_time.min ;
    }
    else if( file_data_1->modify_time.sec !=  file_data_2->modify_time.sec)
    {
        return file_data_1->modify_time.sec -  file_data_2->modify_time.sec ;
    }
    else
    {
        return -1;
    }
}

/*****************************************************************************/
//  Description : descend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareMTimeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
	return (0 - CompareMTimeAsec(data_1_ptr, data_2_ptr));
}

/*****************************************************************************/
//  Description : asecend as file create time
//  Global resource dependence : 
//  Author: 
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareCTimeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{	
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;	
    int result = 0;
    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);
    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareTimeAsec param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_854_112_2_18_2_20_4_210,(uint8*)"");
        return -1;
    }

    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;
    
    result = (file_data_1->create_date.year - file_data_2->create_date.year);
    if(!result)
    {
        result = file_data_1->create_date.mon -  file_data_2->create_date.mon;
        if(!result)
        {
            result = file_data_1->create_date.mday -  file_data_2->create_date.mday;
            if(!result)
            {
                result = file_data_1->create_time.hour -  file_data_2->create_time.hour;
                if(!result)
                {
                    result = file_data_1->create_time.min -  file_data_2->create_time.min ;
                    if(!result)result = file_data_1->create_time.sec -  file_data_2->create_time.sec;
                }                
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : descend as file time
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareCTimeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
	return (0 - CompareCTimeAsec(data_1_ptr, data_2_ptr));
}

/*****************************************************************************/
//  Description : asecend as file size
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareSizeAsec(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
    FILEARRAY_DATA_T    *file_data_1 = NULL;
    FILEARRAY_DATA_T    *file_data_2 = NULL;

    //SCI_ASSERT(NULL != data_1_ptr);
    //SCI_ASSERT(NULL != data_2_ptr);

    if((PNULL == data_1_ptr) || (PNULL == data_2_ptr))//�ݴ�
    {
        //SCI_TRACE_LOW:"CompareSizeAsec param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFILEARRAY_921_112_2_18_2_20_4_211,(uint8*)"");
        return 0;
    }

    file_data_1 = (FILEARRAY_DATA_T*)data_1_ptr;
    file_data_2 = (FILEARRAY_DATA_T*)data_2_ptr;
    
    if(file_data_1->size != file_data_2->size)
    {
    	return file_data_1->size - file_data_2->size ;
   }
   else
   {
   	return file_data_1->size - file_data_2->size ;
   }	
}

/*****************************************************************************/
//  Description : descend as file size
//  Global resource dependence : 
//  Author: ying.xu
//  Note:		20100902
/*****************************************************************************/
LOCAL int CompareSizeDesc(
                           const void       *data_1_ptr,
                           const void       *data_2_ptr
                           )
{
	return (0 - CompareSizeAsec(data_1_ptr, data_2_ptr));
}

#endif

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

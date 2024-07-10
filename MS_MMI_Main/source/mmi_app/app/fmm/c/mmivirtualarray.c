/*****************************************************************************
** File Name:      mmivirtualarray.c                                         *
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
#include "mmivirtualarray.h"
#include "mmifmm_internal.h"
#include "sfs.h"
#include "os_api.h"
#include "string.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmifmm_interface.h"
/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define VIRTUALARRAY_IDENTIFIER         0xaaaaaaaa  //ͷ��Ϣ��ʶ

#define VIRTUALARRAY_FILENAME_MAX_LEN   MMIFILE_FULL_PATH_MAX_LEN   //ȫ·������󳤶�
#define VIRTUALARRAY_DATA_MAX_SIZE	    2048 //����data�����ֵ
#ifdef MMI_FMM_MINI_SUPPORT
#define VIRTUALARRAY_BUFFER_SIZE		50	//�ڴ滺������С��50������
#else
#define VIRTUALARRAY_BUFFER_SIZE		100	//�ڴ滺������С��100������
#endif

#define VIRTUALARRAY_MIN_DISKSPACE      61440 //1024x60, 60k
#define VIRTUALARRAY_INVALID            0xffffffff

#define VIRTUALARRAY_DEL_ALLOCSIZE		51200 //50K

#define VIRTUALARRAY_PRE_DEL_MAX_SIZE	1024 //��ɾ�����ǲ���Ҫ���¹����ļ��������ļ�����
//�ⲿ����
#define VIRTUALARRAY_WA_SIZE			(50)		//�û�ѡ������������С
		//�û�ѡ������������С
#define VIRTUALARRAY_MULTIMERGE_WAYS    (5)            //��·�鲢·��
#define VIRTUALARRAY_FOBUF_SIZE         (50)        //��·�鲢ʱ,����ļ�������
#define VIRTUALARRAY_FIBUF_SIZE         (100/5)        //��·�鲢ʱ,�����ļ�������

#define VTMUTEX_NAME_MAXLEN 30
static SCI_MUTEX_PTR mutex = SCI_NULL;
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
//VIRTUALARRAY�����ͣ�һ����3��
typedef enum
{
    //VIRTUALARRAY_MODETYPE_NULL,/*lint !e749*/
    VIRTUALARRAY_MODETYPE_1,        //����1���й����ļ����ļ���С���ޣ���������û������(������U�̿ռ��������)
    VIRTUALARRAY_MODETYPE_2,        //����2���й����ļ����ļ���С���ޣ���������������(������U����һ���ռ�����)
    VIRTUALARRAY_MODETYPE_3     //����3���޹����ļ����������������ƣ�������ȫ������ڴ���(������U�̿ռ䲻������)
}VIRTUALARRAY_MODETYPE_E;

typedef struct
{
    VIRTUALARRAY_MODETYPE_E type;
    BOOLEAN     has_asso_file;  //�Ƿ��й����ļ�
    uint32      num_limit;      //���������Ƿ�������
}VIRTUALARRAY_MODE_T;

//��Ӧ�ļ����������ļ���virtualarray�п�ʼ�ͽ���λ��
typedef struct
{
    uint32    first_index;  //���ļ��е��ļ���virtualarray�п�ʼ��index
    uint32    last_index;   //���ļ��е��ļ���virtualarray�н�����index
}VIRTUALARRAY_FOLDER_FILES_T;
//VIRTUALARRAY����Ϣͷ�ṹ
typedef struct  
{
    uint32      identifier;
    uint32      data_size;          //�������ݴ�С
    void        *buffer_ptr;        //�ڴ滺����ָ��    
    uint32       max_num_in_buffer;        //�ڴ滺�������ܷ��µ�������
	uint32      *del_id_table_ptr;    //��ɾ�����ļ��ڹ����ļ��е�ID
	uint32      num_del_files;        //��ɾ�����ļ���û�и��µ������ļ��е��ļ��ĸ���
	uint32      max_del_id_count;     //
	VIRTUALARRAY_FOLDER_FILES_T *folder_info_ptr;//�ļ����������ļ���virtualarray�п�ʼ�ͽ���λ��
	VIRTUALARRAY *folder_array;//�ļ���
	//uint32      folder_num;//�ļ��еĸ���
    uint32      first_id_inbuf;     //�������е�һ�����ݵ�id��
    uint32      num_inbuf;          //�������д洢�����ݸ���
    wchar       asso_file_name[VIRTUALARRAY_FILENAME_MAX_LEN+1];    //�����ļ���
    uint16      asso_name_len;
    BOOLEAN     need_update;    //�������������ݱ����ӻ��޸�,��Ҫ�ѻ������е����ݸ��µ������ļ���
    uint32      total_size;     //�����С�����ݸ���
    VIRTUALARRAY_MODE_T mode;   //VIRTUALARRAY������
	MMIFILE_ERROR_E  assoc_file_save_ret;//�����ļ�������
	MMIFILE_DEVICE_E  assoc_file_storage;//�����ļ�����λ��
#if !defined RELEASE_INFO
	const char*			file_name_ptr;
	uint32				file_line;
#endif
 //   SCI_MUTEX_PTR mutex;
}VIRTUALARRAY_HEADER_T;


//��������ʹ��
typedef struct merger_file_tag
{
    wchar       file_name[VIRTUALARRAY_FILENAME_MAX_LEN+1];
    uint16      file_name_len;
    uint16      num;

    uint16      data_size;
    uint16      total_file;
    struct merger_file_tag      *next;
}MERGER_FILE_T;

//const wchar g_mmivirtualarray_default_udisk_dir_wstr[]  = { 'D', ':', '\\', 0 };// VIRTUALARRAY_DEFAULT_DIR_U       "\x00\x44\x00\x3A\x00\x5C"
//const wchar g_mmivirtualarray_default_sdcard_dir_wstr[] = { 'E', ':', '\\', 0 };// VIRTUALARRAY_DEFAULT_DIR_SD      "\x00\x45\x00\x3A\x00\x5C"
//const wchar g_mmivirtualarray_default_sdcard1_dir_wstr[]= { 'F', ':', '\\', 0 };// VIRTUALARRAY_DEFAULT_DIR_SD1      "\x00\x46\x00\x3A\x00\x5C"

/**--------------------------------------------------------------------------*
 **                         Function Declare                                 *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ���ڴ��ж�ȡ����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void ReadDataFromBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  read_id,
                            void    *data_ptr
                            );

/*****************************************************************************/
//  Description : ���ڴ���д����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void WriteDataToBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr, 
                            uint32  write_id,
                            void    *data_ptr
                            );

/*****************************************************************************/
//  Description : �ڴ滺�����������ݱ����ӻ��޸�,��Ҫ�ѻ������е����ݸ��µ�
//                �����ļ���
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateBufferToFile(
                              VIRTUALARRAY_HEADER_T *v_header_ptr
                              );

/*****************************************************************************/
//  Description : �����Ѿ�ɾ����û�и��µ������ļ���ƫ�Ʊ�����item_id�ڹ����ļ��е�ƫ����
//  Global resource dependence : 
//  Paramert: v_header_ptr[IN]:virtural array information
//            item_id [IN]:item id
//            id_in_file[OUT]: the id in assoc file
//  Author: 
//  Note:
//  Return : the postion of item_id in delete table
/*****************************************************************************/
LOCAL uint32 GetAssocIDByItemId(const VIRTUALARRAY_HEADER_T *v_header_ptr, 
								   uint32 item_id, 
								   uint32 *id_in_file
								   );
/*****************************************************************************/
//  Description : ���¹����ļ�����û��ʵ��ɾ�����ļ���Ϣ�ӹ����ļ���ɾ��������del_id_table
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateAssoFile(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr
                            );

/*****************************************************************************/
//  Description : �ж� file_index�Ƿ��Ѿ���folder_info_ptr��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FileIsGrouped(VIRTUALARRAY_FOLDER_FILES_T *folder_info_ptr,
					  uint32                      folder_info_num,
					  uint32                      *file_index//IN:OUT
					  );
/*****************************************************************************/
//  Description : ���ļ���ɾ���������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteBufFromFile(SFS_HANDLE  sfs_handle, 
								uint32 file_size, 
								uint32 begin_offset, 
								uint32 end_offset
								);

/*****************************************************************************/
//  Description : ���ļ��ж�ȡ����,id��first_id��first_id+VIRTUALARRAY_BUFFER_SIZE-1
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadFileToBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  first_id
                            );

/*****************************************************************************/
//  Description : ɾ�������ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void DeleteAssoFile(
                          VIRTUALARRAY_HEADER_T *v_header_ptr
                          );

/*****************************************************************************/
//  Description : ���������ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateAssoFile(
                              VIRTUALARRAY_HEADER_T *v_header_ptr
                              );

/*****************************************************************************/
//  Description : ���ļ���ɾ��һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneDataInFile(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  del_id
                            );

/*****************************************************************************/
//  Description : ����������ֵ��ļ��ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateRandFile(
                              uint16    *file_name_ptr,
                              uint16    *file_name_len_ptr
                              );
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : ���㷨�����ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ExternalSortFile(
                           VIRTUALARRAY_HEADER_T    *v_header_ptr,
                           MMIVIRTUALARRAY_COMPARE  compare_func,   //�ȽϺ���
                           uint16       *file_name,         //out
                           uint16       *file_name_len      //out
                           );

/*****************************************************************************/
//  Description : ��ʼ���鲢��
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* InitMergeFiles(
                                   VIRTUALARRAY_HEADER_T    *v_header_ptr,
                                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                                   );

/*****************************************************************************/
//  Description : ��·�鲢
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* MultiWayMerge(
                                   MERGER_FILE_T    *m_fi_ptr,
                                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                                   );

/*****************************************************************************/
//  Description : ���ٹ鲢��
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyMergeFiles(
                               MERGER_FILE_T    *m_file_ptr
                               );

/*****************************************************************************/
//  Description : �û�ѡ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL uint16 ReplaceSelSort(
                           SFS_HANDLE   fo,

                           void         *wa,
                           uint16       wa_size,
                           uint16       *wa_num,
                           uint16       data_size,

                           VIRTUALARRAY_HEADER_T    *fi,
                           uint16       fi_total,
                           uint16       *fi_index,

                           MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                           );
#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : ֱ��ѡ�����򣬱Ƚϴ���O(n*n),copy����O(n)
// ���ڱ����ļ��������ݽṹ��ͦ���ڴ濽��ʱ����ƿ��
//  Global resource dependence : 
//  Author: ying.xu
//  Note:�Ϳ⺯��qsort����compare_func���ֵ��������
/*****************************************************************************/
LOCAL BOOLEAN DirectSelectSort(void *p_array, uint32 array_num, uint32 item_size,
                           MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                           );
#endif
 #ifndef MMI_FMM_SORT_API_EXT_SUPPORT
 #else
/*****************************************************************************/
//  Description : �����һ��鲢�ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* GetNextMergeGroup(
                                     MERGER_FILE_T  *m_fi_ptr,
                                     uint16         ways
                                     );

/*****************************************************************************/
//  Description : �Ѽ����ļ��ϲ���һ���ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL uint16 Merge(
                   SFS_HANDLE       fo,
                   MERGER_FILE_T    *m_fi_ptr,
                   uint16           ways,
                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                   );
#endif
/**--------------------------------------------------------------------------*
 **                         Function Implement                               *
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
                                )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_SDCARD;
    MMIFILE_ERROR_E file_error = SFS_NO_ERROR;
    
    if(0 == data_size)
    {
        return NULL;
    }

    //����һ�δ��ͷ��Ϣ���ڴ�
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)SCI_MallocApp(sizeof(VIRTUALARRAY_HEADER_T),file,line);
    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr)
    {
        //SCI_TRACE_LOW:"MMIVIRTUALARRAY_Create:MMIVIRTUALARRAY_Create PNULL == v_header_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_317_112_2_18_2_21_8_599,(uint8*)"");
        return PNULL;
    }
    SCI_MEMSET(v_header_ptr, 0, sizeof(VIRTUALARRAY_HEADER_T));

    file_error = MMIAPIFMM_GetSuitableFileDev(MMI_DEVICE_SDCARD, VIRTUALARRAY_MIN_DISKSPACE, &file_dev);
    v_header_ptr->assoc_file_storage = file_dev;
    
    //��ʼ����Ա����
    v_header_ptr->identifier = VIRTUALARRAY_IDENTIFIER;
    v_header_ptr->data_size = data_size;
    v_header_ptr->total_size = 0;
    v_header_ptr->need_update = FALSE;
    v_header_ptr->num_del_files = 0;
	v_header_ptr->max_del_id_count = 0;
	v_header_ptr->del_id_table_ptr = PNULL;
#if !defined RELEASE_INFO
    v_header_ptr->file_name_ptr = file;
    v_header_ptr->file_line  = line;
#endif
    //�����ڴ滺����
    v_header_ptr->max_num_in_buffer = VIRTUALARRAY_BUFFER_SIZE;
    v_header_ptr->buffer_ptr = (void *)SCI_MallocApp(v_header_ptr->max_num_in_buffer*data_size,file,line);
    if (NULL == v_header_ptr->buffer_ptr)
    {
        SCI_FREE(v_header_ptr);
        return 0;
    }
    
    if(SCI_NULL == mutex)
    {
        char mutex_name[VTMUTEX_NAME_MAXLEN] = {0};

        sprintf(mutex_name,"VTMUTEX0x%lx",v_header_ptr);
        mutex = SCI_CreateMutex(mutex_name, SCI_INHERIT);
    }
    
    if(SCI_NULL == mutex)
    {
        SCI_FREE(v_header_ptr->buffer_ptr);
        SCI_FREE(v_header_ptr);
        return 0;
    }
    
	v_header_ptr->assoc_file_save_ret = SFS_ERROR_NONE;
    v_header_ptr->first_id_inbuf = 0;
    v_header_ptr->num_inbuf = 0;

    //���U��ʣ��ռ�
    if(SFS_ERROR_NO_SPACE == file_error)
    {
        //U�̿ռ䲻��
        //SCI_TRACE_LOW:"MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_348_112_2_18_2_21_8_600,(uint8*)"");
        v_header_ptr->assoc_file_save_ret = SFS_ERROR_NO_SPACE;

        v_header_ptr->mode.type = VIRTUALARRAY_MODETYPE_3;
        v_header_ptr->mode.has_asso_file = FALSE;
        v_header_ptr->mode.num_limit = v_header_ptr->max_num_in_buffer;
    }
    else
    {

        //���������ļ�
        v_header_ptr->assoc_file_save_ret = CreateAssoFile(v_header_ptr);
        if (SFS_ERROR_NONE == v_header_ptr->assoc_file_save_ret
           ||SFS_ERROR_HAS_EXIST == v_header_ptr->assoc_file_save_ret//&&MMIAPIFMM_IsFileExist(v_header_ptr->asso_file_name, v_header_ptr->asso_name_len)
           )
        {
            v_header_ptr->mode.type = VIRTUALARRAY_MODETYPE_1;
            v_header_ptr->mode.has_asso_file = TRUE;
            v_header_ptr->mode.num_limit = VIRTUALARRAY_INVALID;            
            //SCI_TRACE_LOW:"MMIVIRTUALARRAY_Create, create virtualarray by file!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_367_112_2_18_2_21_8_601,(uint8*)"");
        }
        else
        {
            //U�̿ռ䲻��
            //SCI_TRACE_LOW:"MMIVIRTUALARRAY_Create, create virtualarray by no udisk space!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_372_112_2_18_2_21_8_602,(uint8*)"");
            v_header_ptr->mode.type = VIRTUALARRAY_MODETYPE_3;
            v_header_ptr->mode.has_asso_file = FALSE;
            v_header_ptr->mode.num_limit = v_header_ptr->max_num_in_buffer;
        }
    }

    //����ͷ��Ϣ�ڴ��ַ

    return v_header_ptr;
}

/*****************************************************************************/
//  Description : ����һ����������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
void MMIVIRTUALARRAY_Destroy(
                        VIRTUALARRAY    *virtual_array_ptr      //out, ���ָ��
                        )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
	VIRTUALARRAY_HEADER_T   *folder_header_ptr = NULL;
//	SCI_MUTEX_PTR mutex = SCI_NULL;

    if(NULL == virtual_array_ptr
        || NULL == *virtual_array_ptr)
    {
        return;
    }

    //�ж�virtual_array_ptr�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(*virtual_array_ptr);
	SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return;
    }
    
    //mutex = v_header_ptr->mutex;
    

    folder_header_ptr = (VIRTUALARRAY_HEADER_T *)v_header_ptr->folder_array;
    
    //�ͷ��ڴ滺����
    if(PNULL != v_header_ptr->buffer_ptr)
    {
        SCI_FREE(v_header_ptr->buffer_ptr);
    }
    if(v_header_ptr->del_id_table_ptr != PNULL)
    {
        SCI_FREE(v_header_ptr->del_id_table_ptr);
        v_header_ptr->del_id_table_ptr = PNULL;
    }
    if(v_header_ptr->folder_info_ptr != PNULL)
    {
        SCI_FREE(v_header_ptr->folder_info_ptr);
        v_header_ptr->folder_info_ptr = PNULL;
    }
    if(v_header_ptr->mode.has_asso_file)
    {
        //ɾ�������ļ�
        DeleteAssoFile(v_header_ptr);
    }
    
    if(folder_header_ptr != PNULL && (VIRTUALARRAY_IDENTIFIER == folder_header_ptr->identifier))
    {
        //�ͷ��ڴ滺����
        SCI_FREE(folder_header_ptr->buffer_ptr);
        if(folder_header_ptr->del_id_table_ptr != PNULL)
        {
            SCI_FREE(folder_header_ptr->del_id_table_ptr);
            folder_header_ptr->del_id_table_ptr = PNULL;
        }
        if(folder_header_ptr->folder_info_ptr != PNULL)
        {
            SCI_FREE(folder_header_ptr->folder_info_ptr);
            folder_header_ptr->folder_info_ptr = PNULL;
        }
        //  if(SCI_NULL != folder_header_ptr->mutex)
        //   {
        //      SCI_DeleteMutex(folder_header_ptr->mutex);
        //   }
        if(folder_header_ptr->mode.has_asso_file)
        {
            //ɾ�������ļ�
            DeleteAssoFile(folder_header_ptr);
        }
        SCI_FREE(folder_header_ptr);
        v_header_ptr->folder_array = PNULL;
    }
    //�ͷ�ͷ��Ϣ
    SCI_FREE(v_header_ptr);

    //���virtual_array_ptr
    *virtual_array_ptr = NULL;
    
    SCI_PutMutex(mutex);
  //  SCI_DeleteMutex(mutex);

    return;
}
/*****************************************************************************/
//  Description : ������������С
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note:copy MMIVIRTUALARRAY_GetArraySize liyan modify for CR153633
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArrayCurSize(    //�������������С
									VIRTUALARRAY    virtual_array       //in
									)
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
	MMIFILE_HANDLE            file_handle = NULL;
	MMIFILE_ERROR_E              file_ret = SFS_ERROR_NONE;
    uint32 total_size = 0;
	
    if(NULL == virtual_array)
    {
        return 0;
    }
	
    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        return 0;
    }

    file_ret = MMIAPIFMM_CreateFileAsyn(v_header_ptr->asso_file_name, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, /*lint !e655*/
        NULL, 
        NULL,
        0,
        0,
        &file_handle);

    if(SFS_INVALID_HANDLE != file_handle)
    {
    uint32  file_size = 0;
    SFS_GetFileSize(file_handle, &file_size);
    SFS_CloseFile(file_handle);
    if (0 == file_size)//filearray��item����δд���ļ�ʱ
    {
            total_size = v_header_ptr->total_size;
        }
        else
        {
            total_size = file_size/v_header_ptr->data_size;
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIVIRTUALARRAY.C][MMIVIRTUALARRAY_GetArraySize] NULL == sfs_handle, file_ret = %d",file_ret);
        total_size =  v_header_ptr->total_size;
    }

    return total_size;
}
/*****************************************************************************/
//  Description : ������������С
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetArraySize(    //�������������С
                        VIRTUALARRAY    virtual_array       //in
                        )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;

    if(NULL == virtual_array)
    {
        return 0;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        return 0;
    }

    return v_header_ptr->total_size;
}

/*****************************************************************************/
//  Description : �ж��Ƿ����ڴ��̿ռ䲻�����ǰ����
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_ERROR_E MMIVIRTUALARRAY_GetFileErrorType(    //����True���ڴ��̿ռ䲻�����ǰ����
                        VIRTUALARRAY    virtual_array//IN
						)
{
    MMIFILE_ERROR_E         result = SFS_ERROR_NONE;
    //modified by liyan.zhu for c++ test
	//VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
	if(PNULL != virtual_array)
	{
        VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
		v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);

		return v_header_ptr->assoc_file_save_ret;
	}
	return result;
}

/*****************************************************************************/
//  Description : ����Ĵ洢������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFILE_DEVICE_E MMIVIRTUALARRAY_GetFileDev(    //���س���Ĵ洢������
                        VIRTUALARRAY    virtual_array//IN
						)
{
    //modified by liyan.zhu for c++ test
	//VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
	if(PNULL != virtual_array)
	{
      VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
		v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);

		return v_header_ptr->assoc_file_storage;
	}
	return MMI_DEVICE_UDISK;
}
/*****************************************************************************/
//  Description : ����������������һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Add(    //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        void    *data_ptr   //in, Ҫ���ӵ�����ָ��
                        )
{
    BOOLEAN     result = FALSE;
	// BOOLEAN     update_result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    uint32      add_id = 0;
	
	  

    if(NULL == virtual_array
        || NULL == data_ptr)
    {
        return FALSE;
    }
    
    
    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);

    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    if(v_header_ptr->num_del_files > 0)
    {
        UpdateAssoFile(v_header_ptr);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        ReadFileToBuffer(v_header_ptr, 0);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    }
    //�ж��Ƿ�virtual_array�����Ѵﵽ����
    if(v_header_ptr->total_size + v_header_ptr->num_del_files >= v_header_ptr->mode.num_limit)
    {
        //v_header_ptr->assoc_file_save_ret = SFS_ERROR_NO_SPACE;
        SCI_PutMutex(mutex);
        return FALSE;
    }

    //���������Ƿ�δ�������������һ�����ݣ���ֱ���򻺳�������������
    if(v_header_ptr->max_num_in_buffer > v_header_ptr->num_inbuf
        && v_header_ptr->first_id_inbuf + v_header_ptr->num_inbuf <= v_header_ptr->total_size + v_header_ptr->num_del_files
		&& (v_header_ptr->total_size + v_header_ptr->num_del_files - v_header_ptr->first_id_inbuf < v_header_ptr->max_num_in_buffer)
		)
    {
        add_id = v_header_ptr->total_size + v_header_ptr->num_del_files;
        WriteDataToBuffer(v_header_ptr, add_id, data_ptr);

        v_header_ptr->num_inbuf++;
        v_header_ptr->total_size++;
        v_header_ptr->need_update = TRUE;

        result = TRUE;
    }
    else
    {
        //�������������ж��Ƿ���Ҫ�Ȱѻ��������µ��ļ�
//		if(v_header_ptr->need_update)
//		{
//			//update buf to file first
//			result = UpdateBufferToFile(v_header_ptr);
//		}
//		else
//		{
//			result = TRUE;
// 		}
        if(!v_header_ptr->need_update || UpdateBufferToFile(v_header_ptr))
        {
            //��ջ�����
            v_header_ptr->first_id_inbuf = 0;
            v_header_ptr->num_inbuf = 0;

            //�򻺳��������һ������
            add_id = v_header_ptr->total_size + v_header_ptr->num_del_files;
            v_header_ptr->first_id_inbuf = add_id;
            v_header_ptr->num_inbuf = 1;
            WriteDataToBuffer(v_header_ptr, add_id, data_ptr);

            v_header_ptr->total_size++;
            v_header_ptr->need_update = TRUE;
            result = TRUE;
        }

    }
    SCI_PutMutex(mutex);
    return result;
}

/*****************************************************************************/
//  Description : �ļ��еĸ���
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
uint32 MMIVIRTUALARRAY_GetTotalFolderCount(   
                        VIRTUALARRAY    virtual_array
                        )
{
	VIRTUALARRAY_HEADER_T   *v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    //modified by liyan.zhu for c++ test
    //VIRTUALARRAY_HEADER_T   *folder_header_ptr =PNULL;
    uint32 total_size = 0;
	if(v_header_ptr != PNULL)
	{
       VIRTUALARRAY_HEADER_T   *folder_header_ptr =PNULL;
       folder_header_ptr = (VIRTUALARRAY_HEADER_T *)v_header_ptr->folder_array;
	   if(folder_header_ptr != PNULL)
	   {
		   total_size = folder_header_ptr->total_size;
	   }
	}
	return total_size;
}

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
                        )
{
	VIRTUALARRAY_HEADER_T   *v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);;

	if(v_header_ptr != PNULL)
	{
       return MMIVIRTUALARRAY_Read(v_header_ptr->folder_array, index, data_ptr);
	}
	return FALSE;
}


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
                        )
{
	VIRTUALARRAY_HEADER_T   *v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    //modified by liyan.zhu for c++ test
    //VIRTUALARRAY_HEADER_T   *folder_header_ptr = PNULL;
	uint32 pos = MMIFMM_INVALID_INDEX;
	
	if(v_header_ptr != PNULL && v_header_ptr->folder_info_ptr != PNULL)	
	{
        VIRTUALARRAY_HEADER_T   *folder_header_ptr = PNULL;
		folder_header_ptr = (VIRTUALARRAY_HEADER_T *)v_header_ptr->folder_array;
		if(index < folder_header_ptr->total_size)
		{
			if(is_first)
			{
				pos =  v_header_ptr->folder_info_ptr[index].first_index;
			}
			else
			{
				pos =  v_header_ptr->folder_info_ptr[index].last_index;
			}
		}
	}
	return pos;
}
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
                        )
{
    BOOLEAN     result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    uint32      read_id = 0;
    //modified by liyan.zhu for c++ test
    //uint32      first_id = 0;

    if(NULL == virtual_array
        || NULL == data_ptr)
    {
        return FALSE;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    SCI_TRACE_LOW("MMIVIRTUALARRAY_Read check mem 1");
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    //�ж�index�Ƿ�Ϸ�
    if(index >= v_header_ptr->total_size)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    //read_id = index;
    GetAssocIDByItemId(v_header_ptr, index, &read_id);

    if(0 < v_header_ptr->num_inbuf
        &&read_id >= v_header_ptr->first_id_inbuf
        && read_id <= (v_header_ptr->first_id_inbuf + v_header_ptr->num_inbuf - 1))
    {
        //Ҫ���������������ڻ�������
        ReadDataFromBuffer(v_header_ptr, read_id, data_ptr);

        result = TRUE;
    }
    else
    {
        uint32      first_id = 0;
        //Ҫ���������Ų��ڻ������ڣ��ж��Ƿ���Ҫ�Ȱѻ��������µ��ļ�
		//first_id = index;
        if(v_header_ptr->need_update)
        {
            UpdateBufferToFile(v_header_ptr);
        }

        //Ҫ���������Ų��ڻ������ڣ����ļ��а����ݶ�����������
        if(v_header_ptr->max_num_in_buffer <= v_header_ptr->total_size + v_header_ptr->num_del_files - read_id)
        {
            first_id = read_id;
        }
        else if(v_header_ptr->max_num_in_buffer <= v_header_ptr->total_size  + v_header_ptr->num_del_files)
        {
            first_id = (v_header_ptr->total_size  + v_header_ptr->num_del_files - v_header_ptr->max_num_in_buffer);
        }
        else
        {
            first_id = 0;
        }
        SCI_TRACE_LOW("MMIVIRTUALARRAY_Read check mem 2");
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        if(ReadFileToBuffer(v_header_ptr, first_id))
        {
            SCI_TRACE_LOW("MMIVIRTUALARRAY_Read check mem 3");
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            v_header_ptr->need_update = FALSE;

            //��ʱ�����������Ѿ������������������
            ReadDataFromBuffer(v_header_ptr, read_id, data_ptr);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            result = TRUE;
        }
    }
    SCI_TRACE_LOW("MMIVIRTUALARRAY_Read check mem end");
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    SCI_PutMutex(mutex);
    return result;
}

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
                        )
{
    BOOLEAN     result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    uint32      write_id = 0;
    //modified by liyan.zhu for c++ test
    //uint32      first_id = 0;

    if(NULL == virtual_array
        || NULL == data_ptr)
    {
        return FALSE;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
    
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    GetAssocIDByItemId(v_header_ptr, index, &write_id);
    //�ж�index�Ƿ�Ϸ�
    if(write_id >= v_header_ptr->total_size + v_header_ptr->num_del_files)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    //write_id = modify_id;

    if(0 < v_header_ptr->num_inbuf
        &&write_id >= v_header_ptr->first_id_inbuf
        && write_id <= (v_header_ptr->first_id_inbuf + v_header_ptr->num_inbuf - 1))
    {
        //Ҫд�������������ڻ�������
        WriteDataToBuffer(v_header_ptr, write_id, data_ptr);
        //v_header_ptr->need_update = TRUE;
        UpdateBufferToFile(v_header_ptr);
        result = TRUE;
    }
    else
    {
        uint32      first_id = 0;
        //Ҫд�������Ų��ڻ������ڣ��ж��Ƿ���Ҫ�Ȱѻ��������µ��ļ�
        if(v_header_ptr->need_update)
        {
            UpdateBufferToFile(v_header_ptr);
        }

        //Ҫ���������Ų��ڻ������ڣ����ļ��а����ݶ�����������
        if(((v_header_ptr->max_num_in_buffer/2)-1) <= write_id)
        {
            first_id = write_id - ((v_header_ptr->max_num_in_buffer/2)-1);
        }
        else
        {
            first_id = 0;
        }

        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        if(ReadFileToBuffer(v_header_ptr, first_id))
        {
            //��ʱ�����������Ѿ������������������
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            WriteDataToBuffer(v_header_ptr, write_id, data_ptr);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            v_header_ptr->need_update = TRUE;

            result = TRUE;
        }
    }
    SCI_PutMutex(mutex);
    return result;
}

/*****************************************************************************/
//  Description : ������������ɾ��һ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Delete( //����True�ɹ���Falseʧ��
                        VIRTUALARRAY    virtual_array,      //in
                        uint32      index       //in, ���������ţ�0��total_size-1
                        )
{
    BOOLEAN     result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    //modified by liyan.zhu for c++ test
	//VIRTUALARRAY_HEADER_T   *folder_header_ptr = NULL;
    uint32      first_id = 0;
    //uint32      del_id = 0;
    uint32       i = 0;
	//uint32       del_folder_id = 0;
	//uint32       folder_size = 0;
    uint32      data_size = 0;

    if(NULL == virtual_array)
    {
        return FALSE;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);

    
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    
    //�ж�index�Ƿ�Ϸ�
    if(index >= v_header_ptr->total_size || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    if(v_header_ptr->total_size == 0)
    {
        SCI_PutMutex(mutex);
        return TRUE;
    }
    //�����ļ��б����ļ�����
    if(v_header_ptr->folder_info_ptr != PNULL && v_header_ptr->folder_array != PNULL)
    {
        VIRTUALARRAY_HEADER_T   *folder_header_ptr = NULL;
        uint32       del_folder_id = 0;
        uint32       folder_size = 0;
        folder_header_ptr = (VIRTUALARRAY_HEADER_T *)(v_header_ptr->folder_array);
        //��folder_info_ptr���ҵ�Ҫɾ���ļ��ķ�Χ��������ļ�����Ҫ��һ
        del_folder_id = folder_header_ptr->total_size;
        folder_size = folder_header_ptr->total_size;
        for(i =0; i < folder_size; i++)
        {
            if(v_header_ptr->folder_info_ptr[i].first_index < VIRTUALARRAY_INVALID)
            {
                if(index== v_header_ptr->folder_info_ptr[i].first_index && v_header_ptr->folder_info_ptr[i].first_index == v_header_ptr->folder_info_ptr[i].last_index)
                {
                    //Ҫɾ�����ļ����ļ�����Ψһ���ļ���Ҫͬʱ�Ѹ��ļ���Ҳɾ��
                    MMIVIRTUALARRAY_Delete(v_header_ptr->folder_array, i);
                    del_folder_id = i;
                }
                else if(index < v_header_ptr->folder_info_ptr[i].first_index && v_header_ptr->folder_info_ptr[i].first_index > 0)
                {
                    //Ҫɾ�����ļ��ڸ��ļ���֮ǰ
                    v_header_ptr->folder_info_ptr[i].first_index--;
                    v_header_ptr->folder_info_ptr[i].last_index--;
                }
                else if(index <= v_header_ptr->folder_info_ptr[i].last_index && v_header_ptr->folder_info_ptr[i].last_index > 0)
                {
                    //Ҫɾ�����ļ��ڸ��ļ����м�
                    v_header_ptr->folder_info_ptr[i].last_index--;
                }
            }
        }
        if(del_folder_id + 1 < folder_size)
        {
            //Ҫɾ�����ļ��ǵ�(del_folder_id+1)�ļ�����Ψһ���ļ�,���ļ������Ѿ�û���ļ����Ѹ��ļ��дӱ���ɾ��
            SCI_MEMCPY(&v_header_ptr->folder_info_ptr[del_folder_id], &v_header_ptr->folder_info_ptr[del_folder_id +1], sizeof(VIRTUALARRAY_FOLDER_FILES_T)*(folder_size - del_folder_id -1));
        }
    }
    //del_id = index;
    GetAssocIDByItemId(v_header_ptr, index, &first_id);
    if(VIRTUALARRAY_MODETYPE_3 == v_header_ptr->mode.type)
    {
        //ֻ��Ҫɾ���ڴ��еļ�¼
        data_size = v_header_ptr->data_size;
        if(PNULL != v_header_ptr->buffer_ptr)
        {
            for(i=first_id; i<v_header_ptr->total_size-1; i++)
            {
                MMI_MEMCPY((uint8*)v_header_ptr->buffer_ptr+data_size*i, data_size,
                           (uint8*)v_header_ptr->buffer_ptr+data_size*(i+1), data_size, data_size);
            }
            v_header_ptr->total_size--;
            v_header_ptr->num_inbuf--;

            result = TRUE;
        }
    }
    else
    {
        //�Ȱѻ��������µ��ļ� 
        if(v_header_ptr->mode.has_asso_file && v_header_ptr->need_update)
        {
            UpdateBufferToFile(v_header_ptr);
        }
        
        //�ӹ����ļ���ɾ��del_id��¼
        if(DeleteOneDataInFile(v_header_ptr, index))
        {
            //���»�����
//            if(0 < v_header_ptr->num_inbuf)
//            {
//                if(first_id > v_header_ptr->first_id_inbuf+v_header_ptr->num_inbuf-1)
//                {
//                    //ɾ����id�Ŵ��ڻ����������һ����¼��id��
//                    //do nothing
//                }
//                else if(first_id < v_header_ptr->first_id_inbuf)
//                {
//                    //ɾ����id��С�ڻ������е�һ����¼��id��
//                    v_header_ptr->first_id_inbuf--;
//                }
//                else
//                {
//                    //ɾ����id���ڻ�������
//                    v_header_ptr->first_id_inbuf = 0;
//                    v_header_ptr->num_inbuf = 0;
//                    v_header_ptr->need_update = FALSE;
//                }
//             }

            result = TRUE;
        }
    }
    SCI_PutMutex(mutex);
    return result;
}
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : �����������������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_Sort(   //����True�ɹ���Falseʧ��
                        VIRTUALARRAY        virtual_array,  //in
                        MMIVIRTUALARRAY_COMPARE  compare_func   //in, �ȽϺ���
                        )
{
    BOOLEAN     result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
//  uint32      data_size = 0;

//    uint16      sort_file[VIRTUALARRAY_FILENAME_MAX_LEN+1] = {0};
    uint16      *sort_file_ptr = PNULL;
    uint16      sort_file_len = 0;
    BOOLEAN     sort_result = FALSE;

    if(NULL == virtual_array
        || NULL == compare_func
		|| MMIFMM_IsSearchingStopped())
    {
        return FALSE;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    if (0 == v_header_ptr->total_size)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    if(PNULL == v_header_ptr->buffer_ptr)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    
    //���û�й����ļ�����ֻ��Ҫ�ѻ������е����ݽ�������
    if(!v_header_ptr->mode.has_asso_file)
    {
        //qsort���ʺϱ�case���Ƚϴ�����0(N*LogN),�����ڴ濽��Ҳ��(N*LogN)���ټ��ϵݹ�
        //�����ϻ���ֱ��ѡ������ȽϺ�,������VC��qsort�ȽϿ죬�ݲ��޸�
        qsort(v_header_ptr->buffer_ptr, v_header_ptr->num_inbuf, 
              v_header_ptr->data_size, compare_func);
        //DirectSelectSort(v_header_ptr->buffer_ptr, v_header_ptr->num_inbuf,
        //    v_header_ptr->data_size, compare_func);
        SCI_PutMutex(mutex);
        return TRUE;
    }

    //Ҫ���������Ų��ڻ������ڣ��ж��Ƿ���Ҫ�Ȱѻ��������µ��ļ�
    if(v_header_ptr->mode.has_asso_file && v_header_ptr->need_update)
    {
		UpdateBufferToFile(v_header_ptr);
    }

    //buffer�зŵ��£�ֱ����buffer������
    if(v_header_ptr->total_size <= v_header_ptr->max_num_in_buffer)
    {        
		UpdateAssoFile(v_header_ptr);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
		ReadFileToBuffer(v_header_ptr, 0 );
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        //DirectSelectSort(v_header_ptr->buffer_ptr, v_header_ptr->num_inbuf,
        //    v_header_ptr->data_size, compare_func);
        qsort(v_header_ptr->buffer_ptr, v_header_ptr->num_inbuf,
            v_header_ptr->data_size, compare_func);
		UpdateBufferToFile(v_header_ptr);
		SCI_PutMutex(mutex);
        return TRUE;
    }

    sort_file_ptr = (uint16 *)SCI_ALLOCA(sizeof(uint16)*(VIRTUALARRAY_FILENAME_MAX_LEN+1));
    //SCI_ASSERT(PNULL!=sort_file_ptr);
    if(PNULL == sort_file_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:MMIVIRTUALARRAY_Sort PNULL == sort_file_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1034_112_2_18_2_21_9_603,(uint8*)"");
        SCI_PutMutex(mutex);
        return FALSE;
    }

    SCI_MEMSET(sort_file_ptr,0x00,(sizeof(uint16)*(VIRTUALARRAY_FILENAME_MAX_LEN+1)));
    
    sort_result = ExternalSortFile(v_header_ptr, compare_func,
                                   sort_file_ptr, &sort_file_len);

    if (sort_result)
    {
        //����ɹ�, ɾ��ԭ���Ĺ����ļ�, �����������ļ����óɹ����ļ�
        SFS_DeleteFile(v_header_ptr->asso_file_name, NULL);
        SCI_MEMSET(v_header_ptr->asso_file_name, 0, sizeof(v_header_ptr->asso_file_name) );
        MMI_WSTRNCPY(v_header_ptr->asso_file_name, VIRTUALARRAY_FILENAME_MAX_LEN,
                   sort_file_ptr, sort_file_len, sort_file_len);
        v_header_ptr->asso_name_len = sort_file_len;

        v_header_ptr->first_id_inbuf = 0;
        v_header_ptr->num_inbuf = 0;
        v_header_ptr->need_update = FALSE;
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        ReadFileToBuffer(v_header_ptr, v_header_ptr->first_id_inbuf);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        result = TRUE;
    }
    else
    {
        //����ʧ��, ɾ����ʱ�ļ�
        v_header_ptr->need_update = FALSE;
        result = FALSE;
    }
    
    //if(PNULL!=sort_file_ptr)
    {
        SCI_FREE(sort_file_ptr);
        sort_file_ptr = PNULL;
    }
    SCI_PutMutex(mutex);
    return result;
}
#endif
/*****************************************************************************/
//  Description : �ڴ滺�����������ݱ����ӻ��޸�,��Ҫ�ѻ������е����ݸ��µ�
//                �����ļ���
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateBufferToFile(
                              VIRTUALARRAY_HEADER_T *v_header_ptr
                              )
{
    BOOLEAN     result = FALSE;
	SFS_ERROR_E file_ret = SFS_ERROR_NONE;
    SFS_HANDLE  sfs_handle = NULL;
    int32   offset = 0;
    uint32  to_write_size = 0;
    //modified by liyan.zhu for c++ test
    //uint32  write_size = 0;
    uint32  file_size = 0;

    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr 
    || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE 
    || (v_header_ptr->data_size == 0)
    || PNULL == v_header_ptr->buffer_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[UpdateBufferToFile] PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1096_112_2_18_2_21_9_604,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"UpdateBufferToFile, first id = %d, num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1100_112_2_18_2_21_9_605,(uint8*)"dd", v_header_ptr->first_id_inbuf, v_header_ptr->num_inbuf);
    if(!v_header_ptr->mode.has_asso_file)
	{
		//no asso file, need not to update
		return FALSE;
	}
    offset = v_header_ptr->first_id_inbuf*v_header_ptr->data_size;
    to_write_size = v_header_ptr->num_inbuf*v_header_ptr->data_size;

    file_ret = MMIAPIFMM_CreateFileAsyn(v_header_ptr->asso_file_name, 
		                    SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, /*lint !e655*/
							NULL, 
							NULL,
							0,
							0,
							&sfs_handle);
    if(NULL != sfs_handle)
    {
		file_ret = SFS_SetFilePointer(sfs_handle, offset, SFS_SEEK_BEGIN);/*lint !e747*/
        if(SFS_ERROR_NONE == file_ret)
        {
            uint32  write_size = 0;
            file_ret = SFS_WriteFile(sfs_handle, v_header_ptr->buffer_ptr, to_write_size, &write_size, NULL); 
            if(SFS_ERROR_NONE == file_ret)
            {
                result = TRUE;
            }
        }

        if(!result)
        {
			SFS_GetFileSize(sfs_handle, &file_size);
        }

        SFS_CloseFile(sfs_handle);
    }

    if(result)
    {
        v_header_ptr->need_update = FALSE;
    }
    else
    {
		v_header_ptr->assoc_file_save_ret = file_ret;
        
        if(0 < file_size)
        {
			//v_header_ptr->is_disk_no_space = TRUE;
            //virtual array���ʹ� VIRTUALARRAY_MODETYPE_1 �л��� VIRTUALARRAY_MODETYPE_2
            v_header_ptr->mode.type = VIRTUALARRAY_MODETYPE_2;
            if (v_header_ptr->data_size!=0)
            {
                v_header_ptr->mode.num_limit = file_size/v_header_ptr->data_size;
            }
            else
            {
                v_header_ptr->mode.num_limit = v_header_ptr->max_num_in_buffer;
            }
            v_header_ptr->first_id_inbuf = 0;
            v_header_ptr->num_inbuf = 0;
            v_header_ptr->need_update = FALSE;
            if(v_header_ptr->total_size > v_header_ptr->mode.num_limit)
            {
                v_header_ptr->total_size = v_header_ptr->mode.num_limit;
            }
        }
		else
		{

            //only in buffer
			//v_header_ptr->is_disk_no_space = TRUE;
            //virtual array���ʹ� VIRTUALARRAY_MODETYPE_1 �л��� VIRTUALARRAY_MODETYPE_2
            v_header_ptr->mode.type = VIRTUALARRAY_MODETYPE_2;
            v_header_ptr->mode.num_limit = v_header_ptr->max_num_in_buffer;
            //v_header_ptr->first_id_inbuf = 0;
            //v_header_ptr->num_inbuf = 0;
            v_header_ptr->need_update = FALSE;
            //v_header_ptr->total_size = v_header_ptr->mode.num_limit;		
		}
    }

    return result;
}

/*****************************************************************************/
//  Description : ���ļ��ж�ȡ����,id��first_id��first_id+VIRTUALARRAY_BUFFER_SIZE-1
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadFileToBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  first_id
                            )
{
    BOOLEAN     result = FALSE;
    SFS_HANDLE  sfs_handle = NULL;
    int32   offset = 0;
    uint32  to_read_size = 0;
    //modified by liyan.zhu for c++ test
    //uint32  read_size = 0;
    //uint32  id_in_file = 0;

    //SCI_ASSERT(NULL != v_header_ptr);  
    if(PNULL == v_header_ptr 
    || v_header_ptr->data_size == 0 
    || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE
    || PNULL == v_header_ptr->buffer_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[ReadFileToBuffer] PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1205_112_2_18_2_21_9_606,(uint8*)"");
        return FALSE;
    }

	//GetAssocIDByItemId(v_header_ptr, first_id, &id_in_file);
    offset = first_id*v_header_ptr->data_size;
    if(v_header_ptr->total_size + v_header_ptr->num_del_files - first_id > v_header_ptr->max_num_in_buffer)
    {
        to_read_size = v_header_ptr->max_num_in_buffer*v_header_ptr->data_size;
    }
    else
    {
        to_read_size = (v_header_ptr->total_size + v_header_ptr->num_del_files - first_id)*v_header_ptr->data_size;
    }

    sfs_handle = SFS_CreateFile(v_header_ptr->asso_file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(NULL != sfs_handle)
    {
        if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, offset, SFS_SEEK_BEGIN))/*lint !e747*/
        {
            uint32  read_size = 0;
            if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, v_header_ptr->buffer_ptr, to_read_size, &read_size, NULL))
            {
                result = TRUE;
            }
        }

        SFS_CloseFile(sfs_handle);
    }

    if(result)
    {
        v_header_ptr->first_id_inbuf = first_id;
        
        if(0 == v_header_ptr->data_size)
        {
            v_header_ptr->num_inbuf = 0;
        }
        else
        {
            v_header_ptr->num_inbuf = to_read_size/v_header_ptr->data_size;
        }

        //SCI_TRACE_LOW:"ReadFileToBuffer, first_id = %d, num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1247_112_2_18_2_21_9_607,(uint8*)"dd", first_id, v_header_ptr->num_inbuf);
    }
    

    return result;
}

/*****************************************************************************/
//  Description : �Ӳ��ұ�ɾ�����ļ�
//  Global resource dependence : 
//  Author: 
//  Note: if the delete file number < VIRTUALARRAY_PRE_DEL_MAX_SIZE, only remember this delete id,
//        not delete it from assoc file, only when the deleted file number >= VIRTUALARRAY_PRE_DEL_MAX_SIZE,
//        delete it from assoc file.
/*****************************************************************************/
LOCAL BOOLEAN DeleteOneDataInFile(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  del_id
                            )
{
    //BOOLEAN     result = FALSE;
    //modified by liyan.zhu for c++ test
    //uint32      del_max_num = 0;
    uint32      item_real_id = 0;
    uint32      insert_pos = 0;
    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DeleteOneDataInFile] PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1272_112_2_18_2_21_9_608,(uint8*)"");
        return FALSE;
    }

    if(v_header_ptr->max_del_id_count > 0 && (v_header_ptr->num_del_files >= v_header_ptr->max_del_id_count))
    {
        //need update assoc file, delete all delete files in table from assoc file
        UpdateAssoFile(v_header_ptr);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        ReadFileToBuffer(v_header_ptr, 0);
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    }
    if(v_header_ptr->num_del_files == 0)
    {
        uint32      del_max_num = 0;
        //need alloc offset table
        del_max_num = MIN(VIRTUALARRAY_PRE_DEL_MAX_SIZE, v_header_ptr->total_size + v_header_ptr->num_del_files);
        if(v_header_ptr->max_del_id_count < del_max_num)
        {
            if(PNULL != v_header_ptr->del_id_table_ptr)
            {
                SCI_FREE(v_header_ptr->del_id_table_ptr);
            }
            v_header_ptr->del_id_table_ptr = (uint32 *)SCI_ALLOCA(del_max_num*sizeof(uint32));
            v_header_ptr->max_del_id_count = del_max_num;
            //SCI_ASSERT(NULL != v_header_ptr->del_id_table_ptr);
            if(PNULL == v_header_ptr->del_id_table_ptr)
            {
                //SCI_TRACE_LOW:"MMIFMM:[DeleteOneDataInFile] 1 PNULL == v_header_ptr->del_id_table_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1297_112_2_18_2_21_9_609,(uint8*)"");
                return FALSE;
            }
            //SCI_MEMSET(v_header_ptr->del_id_table_ptr, 0x00, (del_max_num*sizeof(uint32)));
        }
        SCI_MEMSET(v_header_ptr->del_id_table_ptr, 0x00, (v_header_ptr->max_del_id_count*sizeof(uint32)));
    }
    insert_pos = GetAssocIDByItemId(v_header_ptr, del_id, &item_real_id);
    if(insert_pos >= v_header_ptr->num_del_files)
    {
        //add table tail
        if(PNULL != v_header_ptr->del_id_table_ptr)
        {
            v_header_ptr->del_id_table_ptr[v_header_ptr->num_del_files] = item_real_id;
        }
    }
    else
    {
        if (PNULL==v_header_ptr->del_id_table_ptr)
        {
            //SCI_TRACE_LOW:"MMIFMM:[DeleteOneDataInFile] 2 PNULL == v_header_ptr->del_id_table_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1314_112_2_18_2_21_9_610,(uint8*)"");
            return FALSE;
        }
        memmove(&v_header_ptr->del_id_table_ptr[insert_pos +1],
                &v_header_ptr->del_id_table_ptr[insert_pos],
                sizeof(uint32)*(v_header_ptr->num_del_files -insert_pos));
        v_header_ptr->del_id_table_ptr[insert_pos] = item_real_id;
    }
    v_header_ptr->num_del_files ++;
    v_header_ptr->total_size--;

    return  TRUE;
}

/*****************************************************************************/
//  Description : ɾ�������ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void DeleteAssoFile(
                          VIRTUALARRAY_HEADER_T *v_header_ptr
                          )
{
    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1339_112_2_18_2_21_10_611,(uint8*)"");
        return;
    }

    if(SFS_ERROR_NONE != SFS_DeleteFile(v_header_ptr->asso_file_name, NULL))
    {
        //SCI_TRACE_LOW:"DeleteAssoFile, fail!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1345_112_2_18_2_21_10_612,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : ���������ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateAssoFile(
                              VIRTUALARRAY_HEADER_T *v_header_ptr
                              )
{
    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DeleteAssoFile] PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1360_112_2_18_2_21_10_613,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }

    return CreateRandFile(v_header_ptr->asso_file_name, &v_header_ptr->asso_name_len);
}

/*****************************************************************************/
//  Description : ����������ֵ��ļ��ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_ERROR_E CreateRandFile(
                              uint16    *file_name_ptr,
                              uint16    *file_name_len_ptr
                              )
{
    MMIFILE_ERROR_E  result = SFS_ERROR_DEVICE;
    //modified by liyan.zhu for c++ test
    //wchar            default_dir[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
	//uint16           device_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIFILE_DEVICE_E fmm_dev = MMI_DEVICE_SDCARD;
    
    //SCI_ASSERT(NULL != file_name_ptr);
    //SCI_ASSERT(NULL != file_name_len_ptr);
    if(PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[CreateRandFile] PNULL == file_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1385_112_2_18_2_21_10_614,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }

    if(PNULL == file_name_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[CreateRandFile] PNULL == file_name_len_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1391_112_2_18_2_21_10_615,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }

    //ȷ�ϴ洢�ռ�, ѡ���нϴ�ռ�Ĵ����豸
    fmm_dev = (MMIFILE_DEVICE_E)MMIAPIFMM_GetMaxFreeDeviceType();
	if(fmm_dev < MMI_DEVICE_NUM)
	{
        wchar            default_dir[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
        uint16           device_len = MMIFILE_FILE_NAME_MAX_LEN;
		MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(fmm_dev), 
            MMIAPIFMM_GetDevicePathLen(fmm_dev),
			MMIMULTIM_DIR_FILEARRAY, 
            MMIAPICOM_Wstrlen(MMIMULTIM_DIR_FILEARRAY),
			PNULL, 0,
			default_dir, &device_len);

        result = MMIAPIFMM_CreateDirectory(default_dir);

        if (SFS_ERROR_NONE == result || SFS_ERROR_HAS_EXIST == result)
        {
            result = MMIAPICOM_CreateRandFileAsyn(default_dir, device_len,
			VIRTUALARRAY_FILE_PREFIX, 0,/*lint !e64*/
			file_name_ptr, file_name_len_ptr);
		    if(result == SFS_ERROR_NONE)
		    {
			    MMIAPIFMM_SetAttr(file_name_ptr, *file_name_len_ptr, TRUE, FALSE, FALSE, FALSE);
		    }
        }
	}
 
    return result;
}

/*****************************************************************************/
//  Description : ���ڴ��ж�ȡ����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void ReadDataFromBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr,
                            uint32  read_id,
                            void    *data_ptr
                            )
{
    //SCI_ASSERT(NULL != v_header_ptr);
    //SCI_ASSERT(NULL != data_ptr);
    if((PNULL == v_header_ptr) 
    || (PNULL == data_ptr) 
    || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE 
    || v_header_ptr->data_size == 0
    || PNULL == v_header_ptr->buffer_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[ReadDataFromBuffer] param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1437_112_2_18_2_21_10_616,(uint8*)"");
        return;
    }
    SCI_TRACE_LOW("ReadDataFromBuffer MEM CHECK 1");
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    MMI_MEMCPY(data_ptr, v_header_ptr->data_size,
               (uint8*)v_header_ptr->buffer_ptr + (read_id - v_header_ptr->first_id_inbuf)*v_header_ptr->data_size, 
               v_header_ptr->data_size,
               v_header_ptr->data_size);
    SCI_TRACE_LOW("ReadDataFromBuffer MEM CHECK END");
    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
}

/*****************************************************************************/
//  Description : ���ڴ���д����
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL void WriteDataToBuffer(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr, 
                            uint32  write_id,
                            void    *data_ptr
                            )
{
    //SCI_ASSERT(NULL != v_header_ptr);
    //SCI_ASSERT(NULL != data_ptr);
    if((PNULL == v_header_ptr) 
    || (PNULL == data_ptr) 
    || v_header_ptr->data_size == 0 
    || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE
    || PNULL == v_header_ptr->buffer_ptr)    
    {
        //SCI_TRACE_LOW:"MMIFMM:[WriteDataToBuffer] param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1465_112_2_18_2_21_10_617,(uint8*)"");
        return;
    }

    MMI_MEMCPY((uint8*)v_header_ptr->buffer_ptr + (write_id - v_header_ptr->first_id_inbuf)*v_header_ptr->data_size, 
                v_header_ptr->data_size,
                data_ptr, v_header_ptr->data_size,
                v_header_ptr->data_size);
}
#ifndef MMI_FMM_SORT_API_EXT_SUPPORT
#else
/*****************************************************************************/
//  Description : ���������㷨�����ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ExternalSortFile(
                           VIRTUALARRAY_HEADER_T    *v_header_ptr,
                           MMIVIRTUALARRAY_COMPARE  compare_func,   //�ȽϺ���
                           uint16       *file_name,         //out
                           uint16       *file_name_len      //out
                           )
{
    BOOLEAN     result = FALSE;
    MERGER_FILE_T   *m_file_ptr = NULL;
    MERGER_FILE_T   *m_file_out_ptr = NULL;

    //SCI_ASSERT(NULL != v_header_ptr);
    //SCI_ASSERT(NULL != compare_func);
    //SCI_ASSERT(NULL != file_name);
    //SCI_ASSERT(NULL != file_name_len);
    if((PNULL == v_header_ptr) ||(PNULL == file_name) || (PNULL == file_name_len))
    {
        //SCI_TRACE_LOW:"MMIFMM:ExternalSortFile param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1496_112_2_18_2_21_10_618,(uint8*)"");
        return FALSE;
    }
    if(PNULL == compare_func)
    {
        //SCI_TRACE_LOW:"MMIFMM:ExternalSortFile PNULL == compare_func"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1501_112_2_18_2_21_10_619,(uint8*)"");
        return FALSE;
    }

    {
        MMIFILE_DEVICE_E    file_dev  = MMI_DEVICE_SDCARD;
        MMIFILE_ERROR_E file_ret  = SFS_ERROR_NONE;
        
        file_ret = MMIAPIFMM_GetSuitableFileDev(MMI_DEVICE_SDCARD, VIRTUALARRAY_MIN_DISKSPACE, &file_dev);
        
        if (SFS_ERROR_NONE != file_ret)
        {
            //�ռ䲻��
            //SCI_TRACE_LOW:"ExternalSortFile, not enough memory!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1514_112_2_18_2_21_10_620,(uint8*)"");
            return FALSE;
        }
    }
    
    //SCI_TRACE_LOW:"ExternalSortFile enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1519_112_2_18_2_21_10_621,(uint8*)"");

    //��ʼ���鲢��
    m_file_ptr = InitMergeFiles(v_header_ptr, compare_func);
    if (NULL == m_file_ptr)
    {
        //SCI_TRACE_LOW:"ExternalSortFile, InitMergeFiles fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1525_112_2_18_2_21_10_622,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"ExternalSortFile, InitMergeFiles success!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1528_112_2_18_2_21_10_623,(uint8*)"");

    //��·�鲢
    while(NULL != m_file_ptr && 1 != m_file_ptr->total_file && (!MMIFMM_IsSearchingStopped())) 
    {
        m_file_out_ptr = MultiWayMerge(m_file_ptr, compare_func);

        if (NULL == m_file_out_ptr)
        {
            break;
        }
        else
        {
            //destroy m_file_ptr
            DestroyMergeFiles(m_file_ptr);

            m_file_ptr = m_file_out_ptr;
        }
    }

    if ((1 == m_file_ptr->total_file) && (!MMIFMM_IsSearchingStopped()) )
    {
        //SCI_TRACE_LOW:"ExternalSortFile, MultiWayMerge success!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1550_112_2_18_2_21_10_624,(uint8*)"");
        result = TRUE;

        MMI_WSTRNCPY(file_name, VIRTUALARRAY_FILENAME_MAX_LEN,
                   m_file_ptr->file_name, 
                   m_file_ptr->file_name_len, 
                   m_file_ptr->file_name_len);
        *file_name_len = m_file_ptr->file_name_len;

        SCI_FREE(m_file_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"ExternalSortFile, MultiWayMerge fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1563_112_2_18_2_21_10_625,(uint8*)"");

        //destroy m_file_ptr
		if((!MMIFMM_IsSearchingStopped()))DestroyMergeFiles(m_file_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : ��ʼ���鲢��
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* InitMergeFiles(
                                   VIRTUALARRAY_HEADER_T    *v_header_ptr,
                                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                                   )
{
    MERGER_FILE_T   *m_file_ptr = NULL;
    MERGER_FILE_T   *m_prev_ptr = NULL;
    MERGER_FILE_T   *m_first_ptr = NULL;
    SFS_HANDLE      m_handle = NULL;

    uint8           *wa_buf = NULL;
    uint16          wa_num = 0;
    uint16          wa_first_num = 0;

    uint16          file_total = 0;
    uint16          init_num = 0;
    uint16          read_index = 0;

    uint32          i = 0;
    

	if(v_header_ptr == PNULL || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE || v_header_ptr->data_size == 0)
	{
		//SCI_TRACE_LOW:"InitMergeFiles v_header_ptr 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1599_112_2_18_2_21_10_626,(uint8*)"d", v_header_ptr);
		return PNULL;
	}
    //��������ʼ��wa��
    wa_buf = (uint8 *)SCI_ALLOCA(VIRTUALARRAY_WA_SIZE*v_header_ptr->data_size);
    //SCI_ASSERT(NULL != wa_buf);
    if(PNULL == wa_buf)
    {
        //SCI_TRACE_LOW:"MMIFMM:InitMergeFiles ERROR! PNULL == wa_buf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1607_112_2_18_2_21_10_627,(uint8*)"");
        return PNULL;
    }
    
    wa_num = 0;
    wa_first_num = MIN(VIRTUALARRAY_WA_SIZE, v_header_ptr->total_size);
    for (i=0; i<wa_first_num && (!MMIFMM_IsSearchingStopped()); i++)
    {
        MMIVIRTUALARRAY_Read(v_header_ptr, i, ((uint8*)wa_buf+v_header_ptr->data_size*i));
    }
    wa_num = wa_first_num;
    read_index = wa_first_num;


    while (init_num < v_header_ptr->total_size &&((!MMIFMM_IsSearchingStopped())))
    {
        //����һ���鲢�ļ�
        m_file_ptr = (MERGER_FILE_T*)SCI_ALLOCA(sizeof(MERGER_FILE_T));
        if(m_file_ptr == PNULL)
        {
            break;
        }
        SCI_MEMSET(m_file_ptr, 0, sizeof(MERGER_FILE_T));

        //create random file
		v_header_ptr->assoc_file_save_ret = CreateRandFile(m_file_ptr->file_name, &m_file_ptr->file_name_len);

        if (SFS_ERROR_NONE != v_header_ptr->assoc_file_save_ret)
        {
            //destroy m_file_ptr
            if (NULL != m_first_ptr)
            {
                m_first_ptr->total_file = file_total;
                DestroyMergeFiles(m_first_ptr);
            }
            
            SCI_FREE(m_file_ptr);
            SCI_FREE(wa_buf);

            return NULL;
        }

        //�򿪹鲢�ļ�
        m_handle = SFS_CreateFile(m_file_ptr->file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        //SCI_ASSERT(NULL != m_handle);
        if (NULL!= m_handle)
        {
            //�û�ѡ������
            m_file_ptr->num = ReplaceSelSort(m_handle, 
                                           wa_buf, VIRTUALARRAY_WA_SIZE, &wa_num,
                                           v_header_ptr->data_size,
                                           v_header_ptr, v_header_ptr->total_size, &read_index,
                                           compare_func);

            SFS_CloseFile(m_handle);
        }
        else
        {
            SCI_FREE(m_file_ptr);
            SCI_FREE(wa_buf);

            return NULL;
        }

        init_num += m_file_ptr->num;
        file_total++;
        if (NULL != m_prev_ptr)
        {
            m_prev_ptr->next = m_file_ptr;
        }
        else
        {
            m_first_ptr = m_file_ptr;
        }
        m_prev_ptr = m_file_ptr;
    }
    if (PNULL!=m_first_ptr)
    {
        m_first_ptr->total_file = file_total;
        m_first_ptr->data_size = v_header_ptr->data_size;
    }
    SCI_FREE(wa_buf);

    return m_first_ptr;
}

/*****************************************************************************/
//  Description : ��·�鲢
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* MultiWayMerge(
                                   MERGER_FILE_T    *m_fi_ptr,
                                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                                   )
{
    MERGER_FILE_T   *m_file_ptr = NULL;
    MERGER_FILE_T   *m_prev_ptr = NULL;
    MERGER_FILE_T   *m_first_ptr = NULL;
    SFS_HANDLE      m_handle = NULL;

    uint16          ways = VIRTUALARRAY_MULTIMERGE_WAYS;
    uint16          file_total = 0;
    uint16          data_size = 0;

    //SCI_ASSERT(NULL != m_fi_ptr);
    //SCI_ASSERT(NULL != compare_func);
    if(PNULL == m_fi_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MultiWayMerge] PNULL == m_fi_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1715_112_2_18_2_21_10_628,(uint8*)"");
        return PNULL;
    }
    
    if(PNULL == compare_func)
    {
        //SCI_TRACE_LOW:"MMIFMM:[MultiWayMerge] PNULL == compare_func"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1721_112_2_18_2_21_10_629,(uint8*)"");
        return PNULL;
    }
    data_size = m_fi_ptr->data_size;

    while (NULL != m_fi_ptr && (!MMIFMM_IsSearchingStopped()))
    {
        //����һ���鲢�ļ�
        m_file_ptr = (MERGER_FILE_T *)SCI_ALLOCA(sizeof(MERGER_FILE_T));
        if(m_file_ptr == PNULL)
        {
            break;
        }
        SCI_MEMSET(m_file_ptr, 0, sizeof(MERGER_FILE_T));

        //create random file
        if (SFS_ERROR_NONE != CreateRandFile(m_file_ptr->file_name, &m_file_ptr->file_name_len))
        {
            //destroy m_file_ptr
            if (NULL != m_first_ptr)
            {
                m_first_ptr->total_file = file_total;
                DestroyMergeFiles(m_first_ptr);
            }

            SCI_FREE(m_file_ptr);

            return NULL;
        }

        //�򿪹鲢�ļ�
        m_handle = SFS_CreateFile(m_file_ptr->file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        
        if ( NULL!=m_handle)
        {
            //�ϲ�һ��, �Ѽ����ļ��ϲ���һ���ļ�
            m_fi_ptr->data_size = data_size;
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            m_file_ptr->num = Merge(
                                    m_handle,
                                    m_fi_ptr,
                                    ways,
                                    compare_func
                                    );
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            SFS_CloseFile(m_handle);
        }
        else
        {
            SCI_FREE(m_file_ptr);

            return NULL;
        }
        file_total++;
        if (NULL != m_prev_ptr)
        {
            m_prev_ptr->next = m_file_ptr;
        }
        else
        {
            m_first_ptr = m_file_ptr;
        }
        m_prev_ptr = m_file_ptr;

        m_fi_ptr = GetNextMergeGroup(m_fi_ptr, ways);
    }
    if (PNULL!=m_first_ptr)
    {
        m_first_ptr->total_file = file_total;
        m_first_ptr->data_size = data_size;
    }

    return m_first_ptr;
}

/*****************************************************************************/
//  Description : ���ٹ鲢��
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyMergeFiles(
                               MERGER_FILE_T    *m_file_ptr
                               )
{
    MERGER_FILE_T   *m_next_ptr = NULL;
    uint16          total = 0;
    uint16          count = 0;
    SFS_ERROR_E     f_error = SFS_NO_ERROR;

    //SCI_ASSERT(NULL != m_file_ptr);
    if(PNULL == m_file_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:[DestroyMergeFiles] PNULL == m_file_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1811_112_2_18_2_21_10_630,(uint8*)"");
        return FALSE;
    }
    
    total = m_file_ptr->total_file;

    while(NULL != m_file_ptr && count < total && (!MMIFMM_IsSearchingStopped()))
    {
        m_next_ptr = m_file_ptr->next;

        //delete file
        f_error = SFS_DeleteFile(m_file_ptr->file_name, NULL);

        SCI_FREE(m_file_ptr);

        count++;

        m_file_ptr = m_next_ptr;
    }

//    if (!(NULL == m_file_ptr && count == total))
//    {
//        SCI_ASSERT(0);
//     }

    return TRUE;
}

/*****************************************************************************/
//  Description : �û�ѡ������
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL uint16 ReplaceSelSort(
                           SFS_HANDLE   fo,

                           void         *wa,
                           uint16       wa_size,
                           uint16       *wa_num,
                           uint16       data_size,

                           VIRTUALARRAY_HEADER_T    *fi,
                           uint16       fi_total,
                           uint16       *fi_index,

                           MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                           )
{
    uint16      sort_num = 0;
    int16       i = 0;
    void        *switch_data = NULL;

    uint32          bytes_written = 0;
    SFS_ERROR_E     sfs_error = SFS_NO_ERROR;

    //SCI_ASSERT(NULL != fo);
    //SCI_ASSERT(NULL != wa);
    //SCI_ASSERT(NULL != wa_num);
    //SCI_ASSERT(NULL != fi);
    //SCI_ASSERT(NULL != fi_index);
    //SCI_ASSERT(*wa_num <= wa_size);
    if(PNULL == wa_num)
    {
        //SCI_TRACE_LOW:"MMIFMM:ReplaceSelSort PNULL == wa_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1874_112_2_18_2_21_11_631,(uint8*)"");
        return sort_num;
    }
        
    if((PNULL == fo) || (PNULL == wa) || (PNULL == fi) || (PNULL == fi_index) || (*wa_num > wa_size))
    {
        //SCI_TRACE_LOW:"MMIFMM:ReplaceSelSort param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1880_112_2_18_2_21_11_632,(uint8*)"");
        return sort_num;
    }

    switch_data = (void *)SCI_ALLOCA(data_size);
    //SCI_ASSERT(NULL != switch_data);
    if(PNULL == switch_data)
    {
        //SCI_TRACE_LOW:"MMIFMM:ReplaceSelSort error:PNULL == switch_data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1888_112_2_18_2_21_11_633,(uint8*)"");
        return 0;
    }

    //��wa����������,Ȼ��д��fo��,��DirectSelectSort����
    /*
    for(j=0; j<*wa_num-1; j++)
    {
        for(i=0; i<*wa_num-1-j; i++)
        {
            if (0 < compare_func((uint8*)wa+data_size*i, (uint8*)wa+data_size*(i+1)))
            {
                SCI_MEMCPY(switch_data, (uint8*)wa+data_size*i, data_size);
                SCI_MEMCPY((uint8*)wa+data_size*i, (uint8*)wa+data_size*(i+1), data_size);
                SCI_MEMCPY((uint8*)wa+data_size*(i+1), switch_data, data_size);
            }
        }
    }
*/
    
    qsort(wa, *wa_num,  data_size, compare_func);
    //DirectSelectSort(wa, *wa_num, data_size, compare_func);
    sfs_error = SFS_WriteFile(fo, wa, (data_size*(*wa_num)), &bytes_written, NULL);
    sort_num = *wa_num;
    *wa_num = 0;

    if (*fi_index < fi_total)
    {
        //��fi�ж�ȡwa_size�����ݵ�wa��
        *wa_num = 0;
        for (i=0; i<wa_size; i++)
        {
            if (*fi_index < fi_total)
            {
                MMIVIRTUALARRAY_Read(fi, *fi_index, (uint8*)wa+data_size*i);
                (*wa_num)++;
                (*fi_index)++;
            }
            else
            {
                break;
            }
        }
    }

    SCI_FREE(switch_data);

    return sort_num;
}
#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : ֱ��ѡ�����򣬱Ƚϴ���O(n*n),copy����O(n),VC�п⺯�����Ż���û���ڴ濽���Ķ���
// ���ڱ����ļ��������ݽṹ��ͦ���ڴ濽��ʱ����ƿ��
//  Global resource dependence : 
//  Author: ying.xu
//  Note:�Ϳ⺯��qsort����compare_func���ֵ��������
/*****************************************************************************/
LOCAL BOOLEAN DirectSelectSort(void *p_array, uint32 array_num, uint32 item_size,
                           MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                           )
{
    uint8* p_temp_array = (uint8*)p_array;
    void* p_temp_item = 0;
    void* p_compare_src = 0;    
    void* p_compare_dst = 0;
    uint32 minimum = 0;
    uint32 i = 0;
    uint32 j = 0;
    
    p_temp_item = SCI_ALLOCA(item_size);
    if(0 == p_temp_item)
    {
        return FALSE;
    }

    while(i < array_num)
    {
        j = i ;
        minimum = i;
        while(j < array_num)
        {        
            if(0 < compare_func( p_temp_array + item_size*minimum, p_temp_array + item_size*j))
            {
                minimum = j;
            }
            j++;
        }        
        p_compare_src = p_temp_array + item_size*minimum;
        p_compare_dst = p_temp_array + item_size*i;
        //swap
        SCI_MEMCPY(p_temp_item, p_compare_src, item_size);
        SCI_MEMCPY(p_compare_src, p_compare_dst, item_size);
        SCI_MEMCPY(p_compare_dst,p_temp_item, item_size);
        i++;
    }
    SCI_FREE(p_temp_item);
    return TRUE;
}
#endif
 #ifndef MMI_FMM_SORT_API_EXT_SUPPORT
 #else
/*****************************************************************************/
//  Description : �����һ��鲢�ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL MERGER_FILE_T* GetNextMergeGroup(
                                     MERGER_FILE_T  *m_fi_ptr,
                                     uint16         ways
                                     )
{
    int16   i = 0;

    //SCI_ASSERT(1 < ways);
    if(1 >= ways)
    {
        //SCI_TRACE_LOW:"MMIFMM:GetNextMergeGroup ways = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1948_112_2_18_2_21_11_634,(uint8*)"d",1000*ways);
        return PNULL;
    }

    while(NULL != m_fi_ptr && i < ways)
    {
        m_fi_ptr = m_fi_ptr->next;
        i++;
    }

    return m_fi_ptr;
}

/*****************************************************************************/
//  Description : �Ѽ����ļ��ϲ���һ���ļ�
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
LOCAL uint16 Merge(
                   SFS_HANDLE       fo,
                   MERGER_FILE_T    *m_fi_ptr,
                   uint16           ways,
                   MMIVIRTUALARRAY_COMPARE  compare_func    //�ȽϺ���
                   )
{
    uint32          i = 0;
    MERGER_FILE_T   *m_ptr = NULL;
    uint32          fi_buf_size = 0;
    uint16          data_size = 0;

    uint16          total = 0;
    uint16          count = 0;

    SFS_HANDLE      *fi_ptr = NULL;
    uint8           *fi_buf_ptr = NULL;
    int16           *fi_buf_pos_ptr = NULL; //������״̬
    uint16          min = 0;

    SFS_ERROR_E     sfs_error = SFS_NO_ERROR;
    uint32          read_size = 0;
    uint32          writen_size = 0;

    uint8           *fo_buf_ptr = NULL;
    int16           fo_buf_pos = 0;

    uint8           *src_ptr = NULL;
    uint8           *dest_ptr = NULL;

    //SCI_ASSERT(NULL != fo);
    //SCI_ASSERT(NULL != m_fi_ptr);
    //SCI_ASSERT(1 < ways);
    if((PNULL == fo) || (PNULL == m_fi_ptr) || (1 >= ways))
    {
        //SCI_TRACE_LOW:"MMIFMM:Merge param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_1998_112_2_18_2_21_11_635,(uint8*)"");
        return 0;
    }

    #define FI_DATA(X)  (fi_buf_ptr + data_size*VIRTUALARRAY_FIBUF_SIZE*X + data_size*fi_buf_pos_ptr[X])

    data_size = m_fi_ptr->data_size;
    //SCI_ASSERT(0 != data_size);
    if(0 == data_size)
    {
        //SCI_TRACE_LOW:"MMIFMM:Merge 0 == data_size"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_2008_112_2_18_2_21_11_636,(uint8*)"");
        return 0;
    }

    //����ʵ�ʵ���Ҫ�ϲ����ļ�����
    i = 0;
    m_ptr = m_fi_ptr;
    while(NULL != m_ptr && i < ways && (!MMIFMM_IsSearchingStopped()))
    {
        m_ptr = m_ptr->next;
        i++;
    }
    ways = i;
   	
    fo_buf_ptr = (uint8 *)SCI_ALLOCA(VIRTUALARRAY_FOBUF_SIZE*data_size);
    if(fo_buf_ptr == PNULL)
    {
        return 0;
    }
    SCI_MEMSET(fo_buf_ptr, 0x00, VIRTUALARRAY_FOBUF_SIZE*data_size);
    //SCI_ASSERT(NULL != fo_buf_ptr);/* assert verified */
    m_ptr = m_fi_ptr;
    if(m_ptr->num < VIRTUALARRAY_FIBUF_SIZE)
    {
        fi_buf_size = data_size*(VIRTUALARRAY_FOBUF_SIZE+VIRTUALARRAY_FIBUF_SIZE - m_ptr->num)*ways;
    }
    else
    {
        fi_buf_size = data_size*(VIRTUALARRAY_FIBUF_SIZE)*ways;
    }
    fi_buf_ptr = (uint8 *)SCI_ALLOCA(fi_buf_size);
    if(fi_buf_ptr == PNULL)
    {        
        SCI_FREE(fo_buf_ptr);
        return 0;
    }   
    SCI_MEMSET(fi_buf_ptr, 0x00, fi_buf_size);
    fo_buf_pos = 0;

    fi_ptr = (SFS_HANDLE *)SCI_ALLOC_APPZ(sizeof(SFS_HANDLE)*ways);
    //SCI_ASSERT(NULL != fi_ptr);/* assert verified */


    //SCI_ASSERT(NULL != fi_buf_ptr);/* assert verified */

    fi_buf_pos_ptr = (int16 *)SCI_ALLOC_APPZ(sizeof(int16)*ways);
    //SCI_ASSERT(NULL != fi_buf_pos_ptr);/* assert verified */

    //������Ҫ�ϲ����ļ�, �����ļ��ж����ݵ�������
   
    for(i=0; i<ways && (!MMIFMM_IsSearchingStopped()); i++)   
    {
        fi_ptr[i] = SFS_CreateFile(m_ptr->file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

        //SCI_ASSERT(NULL != fi_ptr[i]);
        if (NULL!=fi_ptr[i])
        {
			if(m_ptr->num < VIRTUALARRAY_FIBUF_SIZE)
			{
				fi_buf_pos_ptr[i] = VIRTUALARRAY_FIBUF_SIZE - m_ptr->num;
			}
			else
			{
				fi_buf_pos_ptr[i] = 0;
			}
            sfs_error = SFS_ReadFile(fi_ptr[i], 
				fi_buf_ptr+data_size*VIRTUALARRAY_FIBUF_SIZE*i + fi_buf_pos_ptr[i]*data_size, 
				data_size*VIRTUALARRAY_FIBUF_SIZE - (fi_buf_pos_ptr[i]*data_size), 
				&read_size, NULL);

        //fi_buf_pos_ptr[i] = 0;

        total += m_ptr->num;

        m_ptr = m_ptr->next;
        }
        else
        {
            break;
        }
    }

    //����ѭ��
    count = 0;
    while(count < total && (!MMIFMM_IsSearchingStopped()))
    {
        //�ӻ������ҵ���С������
        min = 0xffff;
        for(i=0; i<ways; i++)
        {
            if (fi_buf_pos_ptr[i] < VIRTUALARRAY_FIBUF_SIZE)
            {
                //��i·���뻺����������Ч
                if (min >= ways)
                {
                    min = i;
                }
                else if(0 < compare_func(FI_DATA(min), 
                                         FI_DATA(i)))
                {
                    min = i;
                }
            }
        }

        if(min >= ways)
		{
			break;
		}

        //����С����д�������������,������������д���ļ���
        if (VIRTUALARRAY_FOBUF_SIZE <= fo_buf_pos)
        {
            sfs_error = SFS_WriteFile(fo, fo_buf_ptr, data_size*VIRTUALARRAY_FOBUF_SIZE, &writen_size, NULL);
            fo_buf_pos = 0;
        }
        SCI_MEMCPY(fo_buf_ptr + data_size*fo_buf_pos, 
                   FI_DATA(min), 
                   data_size);
        fo_buf_pos++;

        //�������뻺����ָ��,����������ʱ,�ӹ鲢�����ļ��ж�ȡ����
        fi_buf_pos_ptr[min]++;
        if (VIRTUALARRAY_FIBUF_SIZE <= fi_buf_pos_ptr[min])
        {
            sfs_error = SFS_ReadFile(fi_ptr[min], 
                                     fi_buf_ptr + data_size*VIRTUALARRAY_FIBUF_SIZE*min, 
                                     data_size*VIRTUALARRAY_FIBUF_SIZE, &read_size, NULL);

            if (data_size*VIRTUALARRAY_FIBUF_SIZE == read_size)
            {
                //���뻺����ȫ������
                fi_buf_pos_ptr[min] = 0;
            }
            else if(0 != read_size)
            {
                //���뻺����δ����,������Ҫ����ƶ�
                fi_buf_pos_ptr[min] = VIRTUALARRAY_FIBUF_SIZE - read_size/data_size;

                src_ptr = fi_buf_ptr + data_size*VIRTUALARRAY_FIBUF_SIZE*min + read_size - 1;
                dest_ptr = FI_DATA(min) + read_size - 1;
                for (i=0; i<read_size; i++)
                {
                    *dest_ptr = *src_ptr;
                    dest_ptr--;
                    src_ptr--;
                }
            }
        }

        count++;
    }

    if (0 < fo_buf_pos)
    {
        sfs_error = SFS_WriteFile(fo, fo_buf_ptr, data_size*fo_buf_pos, &writen_size, NULL);
        fo_buf_pos = 0;
    }

    //�ر��ļ�
    m_ptr = m_fi_ptr;
    for(i=0; i<ways; i++)   
    {
        SFS_CloseFile(fi_ptr[i]);
        m_ptr = m_ptr->next;
    }

    SCI_FREE(fi_ptr);
    SCI_FREE(fi_buf_ptr);
    SCI_FREE(fi_buf_pos_ptr);
    SCI_FREE(fo_buf_ptr);

    return total;
}
#endif
/*****************************************************************************/
//  Description : �����Ѿ�ɾ����û�и��µ������ļ���ƫ�Ʊ�����item_id�ڹ����ļ��е�ƫ����
//  Global resource dependence : 
//  Paramert: v_header_ptr[IN]:virtural array information
//            item_id [IN]:item id
//            id_in_file[OUT]: the id in assoc file
//  Author: 
//  Note:
//  Return : the postion of item_id in delete table
/*****************************************************************************/
LOCAL uint32 GetAssocIDByItemId(const VIRTUALARRAY_HEADER_T *v_header_ptr, uint32 item_id, uint32 *id_in_file)
{
	uint32 search_pos = 0;     //postion of the id_in_file in del_id_table_ptr
	//modified by liyan.zhu for c++ test
	//uint32 i = 0;
	if(v_header_ptr)
	{
		if(v_header_ptr->del_id_table_ptr && (v_header_ptr->num_del_files > 0))
		{
			//exist no-update delete files
			if(v_header_ptr->del_id_table_ptr[v_header_ptr->num_del_files - 1] < item_id)
			{
				//the item_id is in back of all deleted id
				*id_in_file = v_header_ptr->num_del_files + item_id;
				search_pos = v_header_ptr->num_del_files;
			}
			else if(v_header_ptr->del_id_table_ptr[0] > item_id)
			{
                //before item_id, no delete id
                *id_in_file = item_id;
                search_pos = 0;
			}
			else
			{
                uint32 i = 0;
				//the item id is in middle of deleted ids
				for(i = 0; i < v_header_ptr->num_del_files; i++)
				{
					//from begin to find not be deleted id num
					//only not be deleted id num == item_id + 1, it means this id is its id in assoc file
					if((v_header_ptr->del_id_table_ptr[i] - i) >= item_id + 1)
					{
						break;
					}
					*id_in_file =item_id + i + 1;//i+ 1 is deleted file num
				}
				search_pos = i;
			}
		}
		else
		{
			//no-update delete files not exist
			*id_in_file = item_id;
			search_pos = 0;
		}
	}
	return search_pos;
}

/*****************************************************************************/
//  Description : ���¹����ļ�����û��ʵ��ɾ�����ļ���Ϣ�ӹ����ļ���ɾ��������del_id_table
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateAssoFile(
                            VIRTUALARRAY_HEADER_T   *v_header_ptr
                            )
{
    BOOLEAN     result = FALSE;
    SFS_HANDLE  sfs_handle = NULL;
    //modified by liyan.zhu for c++ test
    //uint32      file_size = 0;
    //uint32  i = 0;
	//uint32  begin_id = 0;
	//uint32  end_id = 0;
	//uint32  deleted_id_num = 0;
	//uint32  left_file_num = 0;
	//SFS_HANDLE sfs_handle = NULL;

    //SCI_ASSERT(NULL != v_header_ptr);
    if(PNULL == v_header_ptr || v_header_ptr->data_size == 0 || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE)
    {
        //SCI_TRACE_LOW:"MMIFMM:UpdateAssoFile PNULL == v_header_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_2250_112_2_18_2_21_11_637,(uint8*)"");
        return FALSE;
    }
    if(!v_header_ptr->mode.has_asso_file)
    {
        return FALSE;
    }
    if(v_header_ptr->total_size == 0)
    {
        //all files be deleted, need clean asso file
        sfs_handle = SFS_CreateFile(v_header_ptr->asso_file_name, SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        //sfs_handle = SFS_CreateFile(v_header_ptr->asso_file_name, )
        if(PNULL != sfs_handle)
        {
            SFS_SetFileSize(sfs_handle, 0);
        }
        SFS_CloseFile(sfs_handle);
        //DeleteAssoFile(v_header_ptr);
        v_header_ptr->total_size = 0;
        v_header_ptr->first_id_inbuf = 0;
        v_header_ptr->num_inbuf = 0;
        result = TRUE;
    }
    else
    {
        //delete file info from asso file
        if(v_header_ptr->num_del_files > 0)
        {
            sfs_handle = SFS_CreateFile(v_header_ptr->asso_file_name, SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
            if(NULL != sfs_handle)
            {
                uint32      file_size = 0;
                if(SFS_ERROR_NONE == SFS_GetFileSize(sfs_handle, &file_size))
                {
                    uint32  i = 0;
                    uint32  begin_id = 0;
                    uint32  end_id = 0;
                    uint32  deleted_id_num = 0;
                    uint32  left_file_num = 0;

                    begin_id = v_header_ptr->del_id_table_ptr[0];
                    end_id = begin_id;
                    left_file_num = begin_id;
                    for(i = 0; i < v_header_ptr->num_del_files; i++)
                    {
                        if(v_header_ptr->del_id_table_ptr[i] != i + left_file_num)
                        {
                            //the delete buffer not adjacent
                            //delet previos  adjacent buffer from asso file
                            result = DeleteBufFromFile(sfs_handle, file_size, (begin_id - deleted_id_num)*v_header_ptr->data_size, (end_id + 1 - deleted_id_num)*v_header_ptr->data_size);
                            deleted_id_num += (end_id - begin_id + 1);
                            file_size = file_size - ((end_id - begin_id + 1)*v_header_ptr->data_size);

                            left_file_num = v_header_ptr->del_id_table_ptr[i] - i;
                            begin_id =  v_header_ptr->del_id_table_ptr[i];
                            end_id = begin_id;
                        }
                        else
                        {
                            end_id =  v_header_ptr->del_id_table_ptr[i];
                        }
                    }//for(i = 0; i < v_header_ptr->num_del_files; i++)
                    if(end_id >= begin_id)
                    {
                    result = DeleteBufFromFile(sfs_handle, file_size, (begin_id - deleted_id_num)*v_header_ptr->data_size, (end_id + 1 - deleted_id_num)*v_header_ptr->data_size);
                    }
                }//get file size success
                SFS_CloseFile(sfs_handle);
            }//open file success
        }
        else
        {
            result = TRUE;
        }
    }//delete file info from asso file
    v_header_ptr->num_del_files = 0;

    return result;
}

/*****************************************************************************/
//  Description : ���ļ���ɾ���������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteBufFromFile(SFS_HANDLE  sfs_handle, 
								uint32 file_size, 
								uint32 begin_offset, 
								uint32 end_offset
								)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     read_result = FALSE;
    BOOLEAN     write_result = FALSE;
    int32   offset_read = 0;
    uint32  to_read_size = 0;
    uint32  read_size = 0;
    int32   offset_write = 0;
    uint32  to_write_size = 0;
    uint32  write_size = 0;
	
	uint8* buffer_ptr = NULL;

	offset_read = end_offset;
    offset_write = begin_offset;
	
	//�����ļ���дbuffer
	buffer_ptr = (uint8 *)SCI_ALLOCA(VIRTUALARRAY_DEL_ALLOCSIZE);
	//SCI_ASSERT(NULL != buffer_ptr);
	if(PNULL == buffer_ptr)
    {
        //SCI_TRACE_LOW:"MMIFMM:DeleteBufFromFile PNULL == buffer_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVIRTUALARRAY_2349_112_2_18_2_21_12_638,(uint8*)"");
        return FALSE;
    }
	
	//��ʼɾ�������Ѻ����������ǰ����
	while(offset_read < (int32)file_size)
	{
		to_read_size = VIRTUALARRAY_DEL_ALLOCSIZE;
		if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, offset_read, SFS_SEEK_BEGIN))/*lint !e747*/
		{
			if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, buffer_ptr, to_read_size, &read_size, NULL))
			{
				read_result = TRUE;
			}
		}
		
		if(read_result)
		{
			to_write_size = read_size;
			if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, offset_write, SFS_SEEK_BEGIN))/*lint !e747*/
			{
				if(SFS_ERROR_NONE == SFS_WriteFile(sfs_handle, buffer_ptr, to_write_size, &write_size, NULL))
				{
					write_result = TRUE;
				}
			}
		}
		
		if(read_result && write_result)
		{
			offset_read += (int32)write_size;
			offset_write += (int32)write_size;
		}
		else
		{
			result = FALSE;
			break;
		}
		
		read_result = FALSE;
		write_result = FALSE;
	}
	
	if(offset_read >= (int32)file_size)
	{
		file_size -= (end_offset - begin_offset);
		SFS_SetFileSize(sfs_handle, file_size);
		result = TRUE;
	}
	
	//�ͷ�buffer
	SCI_FREE(buffer_ptr);
	return result;
}

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
                                     )
{
    BOOLEAN result = FALSE;
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    //modified by liyan.zhu for c++ test
    //SFS_HANDLE sfs_handle = NULL;
    //uint32     to_read_size = 0;
    uint32     total_size = 0;

    if(PNULL == virtual_array || PNULL == file_name || 0 == file_name_len)
    {
        return result;
    }
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);

    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);


    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    if(0 == v_header_ptr->data_size
        || v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE
        || PNULL == v_header_ptr->buffer_ptr
        )
    {
        SCI_PutMutex(mutex);
        return result;
    }

    if(v_header_ptr->mode.has_asso_file)
    {
        //write to asso file
        MMIAPIFMM_AppendFile(v_header_ptr->asso_file_name, file_name, file_offset, FALSE, &total_size);
        v_header_ptr->total_size = total_size/v_header_ptr->data_size;
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        result = ReadFileToBuffer(v_header_ptr, 0); 
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
    }
    else
    {
        SFS_HANDLE sfs_handle = NULL;
        //read file to buffer, no asso file to write
        sfs_handle = SFS_CreateFile(file_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
        if(NULL != sfs_handle)
        {
            uint32     to_read_size = 0;
            if(SFS_ERROR_NONE == SFS_GetFileSize(sfs_handle, &to_read_size))
            {
                if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle, file_offset, SFS_SEEK_BEGIN))/*lint !e747*/
                {
                    if(to_read_size > file_offset)
                    {
                        to_read_size = to_read_size - file_offset;
                    }
                    to_read_size = MIN((v_header_ptr->data_size*v_header_ptr->mode.num_limit), to_read_size);
                    if(SFS_ERROR_NONE == SFS_ReadFile(sfs_handle, v_header_ptr->buffer_ptr, to_read_size, &total_size, NULL))
                    {
                        result = TRUE;
                        v_header_ptr->first_id_inbuf = 0;
                        v_header_ptr->total_size = total_size/v_header_ptr->data_size;
                        v_header_ptr->num_inbuf = v_header_ptr->total_size;
                    }
                }
            }
            SFS_CloseFile(sfs_handle);
        }
    }
    if(!result)
    {
        v_header_ptr->total_size = 0;
    }
    SCI_PutMutex(mutex);
    return result;
}

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
                                     )
{
	BOOLEAN result = FALSE;
	VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    SFS_HANDLE sfs_handle = NULL;
	uint32     total_size = 0;

	if(PNULL == virtual_array || PNULL == file_name || 0 == file_name_len)
	{
		return result;
	}
	v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
	
	SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    
	if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    
	if(PNULL == v_header_ptr->buffer_ptr
		||v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE
		|| PNULL ==v_header_ptr->buffer_ptr
		)
    {
        SCI_PutMutex(mutex);
        return result;
    }	

  
	if(v_header_ptr->mode.has_asso_file)
	{
		//append asso file to list file 
		if(v_header_ptr->num_del_files > 0)
		{
			UpdateAssoFile(v_header_ptr);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
			ReadFileToBuffer(v_header_ptr, 0);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
		}
		if(v_header_ptr->need_update)
		{
			UpdateBufferToFile(v_header_ptr);
		}
		MMIAPIFMM_AppendFile(file_name, v_header_ptr->asso_file_name,0, FALSE, PNULL);
		result = TRUE;
	}
	else
	{
		//write buffer to list file
		sfs_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);/*lint !e655*/
		if(NULL != sfs_handle)
		{
			total_size = v_header_ptr->data_size * v_header_ptr->total_size;
  
			if(SFS_ERROR_NONE == SFS_WriteFile(sfs_handle, v_header_ptr->buffer_ptr, total_size, &total_size, NULL))
			{
         
				result = TRUE;
			}
			MMIAPIFMM_CloseFile(sfs_handle);
		}
	}
	SCI_PutMutex(mutex);
    return result;
}

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
                                     )
{
	MMIFILE_ERROR_E result = SFS_ERROR_NONE;
	VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    //modified by liyan.zhu for c++ test
    //SFS_HANDLE sfs_handle = NULL;
	//uint32     total_size = 0;

	if(PNULL == virtual_array || PNULL == file_name || 0 == file_name_len)
	{
		return SFS_ERROR_INVALID_PARAM;
	}
	v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
   
   if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    if(PNULL == v_header_ptr->buffer_ptr
        ||v_header_ptr->data_size > VIRTUALARRAY_DATA_MAX_SIZE
        )
    {
        SCI_PutMutex(mutex);
        return SFS_ERROR_INVALID_PARAM;
    }


    if(v_header_ptr->mode.has_asso_file)
    {
        //append asso file to list file

        if(v_header_ptr->num_del_files > 0)
        {
            UpdateAssoFile(v_header_ptr);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
            ReadFileToBuffer(v_header_ptr, 0);
            MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        }

        if(v_header_ptr->need_update)
        {
            UpdateBufferToFile(v_header_ptr);
        }
        result = MMIAPIFMM_AppendFile(file_name, v_header_ptr->asso_file_name,0, FALSE, PNULL);
    }
    else
    {
        SFS_HANDLE sfs_handle = NULL;
        //write buffer to list file
        sfs_handle = SFS_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_APPEND, NULL, NULL);/*lint !e655*/
        if(NULL != sfs_handle)
        {
            uint32     total_size = 0;
            total_size = v_header_ptr->data_size * v_header_ptr->total_size;
            result = SFS_WriteFile(sfs_handle, v_header_ptr->buffer_ptr, total_size, &total_size, NULL);
            MMIAPIFMM_CloseFile(sfs_handle);
        }
        else
        {
            result = SFS_ERROR_INVALID_PARAM;
        }
    }
    SCI_PutMutex(mutex);
    return result;
}

/*****************************************************************************/
//  Description : �ж� file_index�Ƿ��Ѿ���folder_info_ptr��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FileIsGrouped(VIRTUALARRAY_FOLDER_FILES_T *folder_info_ptr,
                        uint32                      folder_info_num,
                        uint32                      *file_index//IN:OUT
                        )
{
    BOOLEAN ret = FALSE;
    //modified by liyan.zhu for c++ test
    //uint32  i = 0;
    if(file_index != PNULL && folder_info_ptr != PNULL && folder_info_num > 0)
    {
        uint32  i = 0;
        for(i = 0; i < folder_info_num; i++)
        {
            if(*file_index >= folder_info_ptr[i].first_index && *file_index <= folder_info_ptr[i].last_index)
            {
                *file_index = folder_info_ptr[i].last_index + 1;
                ret = TRUE;
                break;
            }
        }
    }
    return ret;
}
/*****************************************************************************/
//  Description : ����folder array��ÿ���ļ��е����ļ��ھ�filearray�еĿ�ʼλ�úͽ���λ��  
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SetFolderStartFileAndEndFile(
                                     //VIRTUALARRAY folder_array,
									 VIRTUALARRAY file_array
                                     )
{
    //MMIFILE_ERROR_E result = SFS_ERROR_NONE;
    VIRTUALARRAY_HEADER_T   *file_header_ptr = NULL;
    VIRTUALARRAY_HEADER_T   *folder_header_ptr = NULL;
    VIRTUALARRAY_DATA_T     *folder_data_ptr = PNULL;
    VIRTUALARRAY_DATA_T     *file_data_ptr = PNULL;
    //SFS_HANDLE sfs_handle = NULL;
    //modified by liyan.zhu for c++ test
    //uint32     j = 0;
    //uint32     i = 0;
    //BOOLEAN    is_first_matched = FALSE;

    file_header_ptr = (VIRTUALARRAY_HEADER_T *)file_array;
    if (PNULL == file_header_ptr)
    {
        return FALSE;
    }
    SCI_GetMutex(mutex, SCI_WAIT_FOREVER);

     if(VIRTUALARRAY_IDENTIFIER != file_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    folder_header_ptr = (VIRTUALARRAY_HEADER_T *)file_header_ptr->folder_array;

    if (PNULL == folder_header_ptr)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    //file_header_ptr->folder_array = folder_header_ptr;
    folder_data_ptr = (VIRTUALARRAY_DATA_T *)SCI_ALLOCA(sizeof(VIRTUALARRAY_DATA_T));
    if(folder_data_ptr == PNULL)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }
    file_data_ptr = (VIRTUALARRAY_DATA_T *)SCI_ALLOCA(sizeof(VIRTUALARRAY_DATA_T));
    if(file_data_ptr == PNULL)
    {
        SCI_FREE(folder_data_ptr);
        SCI_PutMutex(mutex);
        return FALSE;
    }
    if(folder_header_ptr->total_size > 0)
    {
        uint32     i = 0;
        BOOLEAN    is_first_matched = FALSE;
        file_header_ptr->folder_info_ptr = SCI_ALLOCA(sizeof(VIRTUALARRAY_FOLDER_FILES_T)*folder_header_ptr->total_size);
        if(file_header_ptr->folder_info_ptr == PNULL)
        {
            SCI_FREE(file_data_ptr);
            SCI_FREE(folder_data_ptr);
            SCI_PutMutex(mutex);
            return FALSE;
        }
        SCI_MEMSET(file_header_ptr->folder_info_ptr, 0xFF, (sizeof(VIRTUALARRAY_FOLDER_FILES_T)*folder_header_ptr->total_size));
        //for(i = 0; i < folder_header_ptr->total_size && (!MMIFMM_IsSearchingStopped()); i++)
        for(i = 0; i < folder_header_ptr->total_size; i++)
        {
            uint32     j = 0;
            SCI_MEMSET(folder_data_ptr, 0x00, sizeof(VIRTUALARRAY_DATA_T));
            SCI_MEMSET(file_data_ptr, 0x00, sizeof(VIRTUALARRAY_DATA_T));
            MMIVIRTUALARRAY_Read(file_header_ptr->folder_array, i, folder_data_ptr);
            is_first_matched = FALSE;
            //for(j = 0; j < file_header_ptr->total_size && (!MMIFMM_IsSearchingStopped()); )
            //modified by liyan.zhu for c++ test
            //for(j = 0; j < file_header_ptr->total_size; )
            while(j < file_header_ptr->total_size)
            {
                if(FILEARRAY_TYPE_DEFAULT_FOLDER == folder_data_ptr->type)
                {
                    //�û�Ԥ��·�����Ѿ���������ֱ��ȡ��һ��
                    file_header_ptr->folder_info_ptr[i] = file_header_ptr->folder_info_ptr[0];
                    break;
                }
                else if(!FileIsGrouped(file_header_ptr->folder_info_ptr, i, &j))//jû�б�����Ŀ¼��ǹ�
                {
                    MMIVIRTUALARRAY_Read(file_array, j, file_data_ptr);
                    //���ļ��ǵ�ǰ�ļ��е����ļ�
                    if(MMIFILE_FileIsInThisFolder(folder_data_ptr->filename,
                        folder_data_ptr->name_len,
                        file_data_ptr->filename,
                        file_data_ptr->name_len
                        ))
                    {
                        if(!is_first_matched)
                        {
                            file_header_ptr->folder_info_ptr[i].first_index = j;
                        }

                        file_header_ptr->folder_info_ptr[i].last_index = j;
                        is_first_matched = TRUE;
                    }
                    else //if(MMIAPICOM_Wstrcmp(file_data_ptr->filename, folder_data_ptr->filename) > 0)
                    {
                        //����û����
                        break;
                    }
                    j++;
                }

            }

        }
    }
    SCI_FREE(file_data_ptr);
    SCI_FREE(folder_data_ptr);
    SCI_PutMutex(mutex);
    return TRUE;

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
    //modified by liyan.zhu for c++ test
	//VIRTUALARRAY_HEADER_T   *base_header_ptr = NULL;

	if(base_virtual_array != PNULL && folder_virtual_array != PNULL)
	{
        VIRTUALARRAY_HEADER_T   *base_header_ptr = NULL;
		base_header_ptr = (VIRTUALARRAY_HEADER_T *)base_virtual_array;
        SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
		if(VIRTUALARRAY_IDENTIFIER != base_header_ptr->identifier)
        {
    	     SCI_PutMutex(mutex);
             return ;
         }
        if (PNULL != base_header_ptr->folder_array)
        {
            MMIVIRTUALARRAY_Destroy(&base_header_ptr->folder_array);
        }
		base_header_ptr->folder_array = folder_virtual_array;
	    SCI_PutMutex(mutex);
	}
}

/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence : 
//  Author: ying.xu
//  Note:set large size when loading files folders or sorting,such as 512KB
//       set small size when used as play list, for example mp3 play list ,such as 2-10KB
/*****************************************************************************/
LOCAL BOOLEAN SetBufferSize(
                                     VIRTUALARRAY    virtual_array,
                                     uint32          size_in_KB           //counted in KB
                                     )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    void*       p_temp = 0;
    uint32      max_num_temp = 0;
    
    if(NULL == virtual_array)
    {
        return FALSE;
    }

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        return FALSE;
    }

    //�ж�size �Ƿ�Ϸ� minimum size is 2KB
    if(size_in_KB < 2)
    {
        return FALSE;
    }
    
    //update file buffer error, nothing can be done
    if(!v_header_ptr->mode.has_asso_file)
	{
		return FALSE;
	}

    if(!UpdateAssoFile(v_header_ptr))
    {
        return FALSE;
    }

    if(v_header_ptr->need_update)
    {
        if(!UpdateBufferToFile(v_header_ptr))
        {
            return FALSE;
        }
    }
	
    p_temp = v_header_ptr->buffer_ptr;
    max_num_temp =  v_header_ptr->max_num_in_buffer;
    
    v_header_ptr->max_num_in_buffer = size_in_KB * 1024 / v_header_ptr->data_size;    
    v_header_ptr->buffer_ptr = (void *)SCI_ALLOCA(v_header_ptr->max_num_in_buffer*v_header_ptr->data_size);
    if(0 == v_header_ptr->buffer_ptr)
    {
        SCI_TRACE_LOW("MMIVIRTUALARRAY_SetBufferSize ALLOC failed!");
        v_header_ptr->buffer_ptr = p_temp;
        v_header_ptr->max_num_in_buffer = max_num_temp;
        return FALSE;
    }

    SCI_MEMSET(v_header_ptr->buffer_ptr, 0, v_header_ptr->max_num_in_buffer*v_header_ptr->data_size);

    MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );

    if(!ReadFileToBuffer(v_header_ptr, v_header_ptr->first_id_inbuf))
    {    
        MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE );
        SCI_TRACE_LOW("MMIVIRTUALARRAY_SetBufferSize ReadFileToBuffer failed");
        SCI_FREE(v_header_ptr->buffer_ptr);
        v_header_ptr->buffer_ptr = p_temp;  
        v_header_ptr->max_num_in_buffer = max_num_temp;
        return FALSE;
    }
    
    SCI_FREE(p_temp);

    SetBufferSize(v_header_ptr->folder_array,size_in_KB);
    
    return TRUE;
}                                     
/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence : 
//  Author: ying.xu
//  modify:yao.chen (cannot set the buffer size when GetMutex return fail)
//  Note:set large size when loading files folders or sorting,such as 512KB
//       set small size when used as play list, for example mp3 play list ,such as 2-10KB
/*****************************************************************************/
BOOLEAN MMIVIRTUALARRAY_SetBufferSize(
                                     VIRTUALARRAY    virtual_array,
                                     uint32          size_in_KB           //counted in KB
                                     )
{
    VIRTUALARRAY_HEADER_T   *v_header_ptr = NULL;
    uint32 ret = SCI_SUCCESS;
    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);

    ret = SCI_GetMutex(mutex, SCI_NO_WAIT);
    
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        SCI_PutMutex(mutex);
        return FALSE;
    }

    if(SCI_SUCCESS != ret)
    {
        SCI_TRACE_LOW("MMIVIRTUALARRAY_SetBufferSize GetMutex FAIL! ret = %d",ret);
        return FALSE;
    }

    SetBufferSize(virtual_array, size_in_KB);
    
    SCI_PutMutex(mutex);
	return TRUE;
}                                     

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

    //�ж�virtual_array�Ƿ�����Ч��VIRTUALARRAY_HEADER_T����ָ��
    v_header_ptr = (VIRTUALARRAY_HEADER_T *)(virtual_array);
    if(VIRTUALARRAY_IDENTIFIER != v_header_ptr->identifier)
    {
        return 0;
    }

    data_size += v_header_ptr->data_size * v_header_ptr->total_size;
    
    return data_size;

}


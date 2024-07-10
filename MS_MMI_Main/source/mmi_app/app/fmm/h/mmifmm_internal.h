/*****************************************************************************
** File Name:      mmifmm_internal.h                                                  *
** Author:                                                                   *
** Date:           06/04/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:     file manager modue                                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Robert           Create
******************************************************************************/

#ifndef _MMIFMM_INTERNAL_H_
#define _MMIFMM_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mmi_filetask.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mime_type.h"
#if defined(DRM_SUPPORT)
#include "drm_api.h"
#endif
#include "mmifmm_pic.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL VALUE DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 //fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : delete file syn
//  Global resource dependence : 
//  Author: liqing.peng
//  Modify: xinrui.wang
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFMM_DeleteFileSyn(
                             const wchar *full_path_ptr, //in
                             uint16      full_path_len   //in
                              );

/*****************************************************************************/
//  Description : get file information
//  Global resource dependence : 
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFMM_GetFileInfo(
                           const wchar        *full_path_ptr,     //in
                           uint16       full_path_len,      //in
                           uint32       *create_time_ptr,   //out
                           uint32       *file_size_ptr      //out
                           );
#endif

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_AlertNoSpaceInDisk(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:liqing.peng
//  Modify: xinrui.wang
//  Note: 
/*****************************************************************************/
uint16 MMIFMM_GetPathDepth(
                           const wchar  *path_ptr,
                           uint16   path_len
                           );
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : get all device info and check if have enough space 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsEnoughSpace(uint32 size);
#endif
/*****************************************************************************/
//  Description : register fmm module nv len and max item
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RegFMMNv(void);

/*****************************************************************************/
//  Description : set display file type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_SetDisplayFileType(uint32 file_type);

/*****************************************************************************/
//  Description : get display file type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIFMM_GetDisplayFileType(void);

/*****************************************************************************/
// Description : �Ƿ�ѡ�е���ͬһ���ļ������ļ�
// Global resource dependence : 
// Author: renyi.hu
// Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_IsSelectedSameFileType(MMIFMM_DATA_INFO_T    *list_data_ptr,
//                                                MMIFMM_FILE_TYPE_E     type
//                                            );


                                
/*****************************************************************************/
//  Description : set tab title
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_SetWinTitle(BOOLEAN is_normal_type,
//                                MMI_WIN_ID_T win_id,                                 
//                                MMI_CTRL_ID_T tab_ctrl_id, 
//                                wchar * current_path,
//                                 uint16  current_path_len);

/*****************************************************************************/
//  Description : ����б�����
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
//PUBLIC void MMIFMM_RemoveListData( MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : MMIFMM_IsListMultiSel
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsListMultiSel(void);


/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
BOOLEAN MMIFMM_IsFilesInFolder(
                            const wchar *full_path_ptr,     //in
                            uint16       full_path_len ,      //in, ˫�ֽ�Ϊ��λ
							const wchar *filter//IN:��ѯ�ַ���
                            );

/*****************************************************************************/
//  Description : check wether file is this folder's sub file 
//  Global resource dependence : 
//  Author:
//  Note: 
//  Return: TRUE:file_name_ptr is in path_name_ptr; FALSE:file_name_ptr isnot in path_name_ptr; 
/*****************************************************************************/
BOOLEAN MMIFILE_FileIsInThisFolder(    
                              const wchar      *path_name_ptr,     //in
                              uint16           path_name_len,      //in, ˫�ֽ�Ϊ��λ
                              const wchar      *file_name_ptr,     //IN
                              uint16           file_name_len  //in, ˫�ֽ�Ϊ��λ
                              );

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : find files in directory by key word
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
uint16 MMIFMM_FindAndGetFiles(         //�����ҵ����ļ���
                                const wchar             *full_path_ptr,     //in, ���ҵ��ļ���(����ͨ���*), eg: D:\Music\*.mp3
                                uint16                  full_path_len,      //in, unicode����
                                MMIFILE_FILE_INFO_T     file_list[],        //out,���ڴ洢�ҵ����ļ���Ϣ
                                uint16                  file_list_size      //in, file_list ����Ĵ�С
                                );

/*****************************************************************************/
//  Description : read file data syn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_ReadFilesDataSynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size        //in
);

/*****************************************************************************/
//  Description : read file data syn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFMM_ReadFileDataByOffsetSynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    uint32                  offset              //in
);

/*****************************************************************************/
//  Description : write file data by offset syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_WriteFileDataByOffsetSyn(
                                        const wchar            *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode����
                                        const uint8             *buf_ptr,           //in
                                        uint32                  to_write_size,      //in
                                        uint32                  offset              //in
                                        );

/*****************************************************************************/
//  Description : read file data by offset asyn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
BOOLEAN MMIFILE_ReadFileDataByOffsetAsyn(
                                        const wchar             *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode����
                                        uint8                   *buf_ptr,           //out
                                        uint32                  to_read_size,       //in
                                        uint32                  offset,             //in
                                        MMI_WIN_ID_T            win_id,             //in
                                        MMI_MESSAGE_ID_E        msg_id              //in
                                        );
#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : combine drm suffix
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ͨ����DRM�ĺ�׺�����������drm�ĺ�׺
/*****************************************************************************/
//PUBLIC uint32 MMIFMM_CombineDrmSuffixInfo(uint32 suffix_type);


/*****************************************************************************/
//  Description : drm support type
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_IsDrmSupportType(uint32 suffix_type);

/*****************************************************************************/
//  Description : convert suffix type
//  Global resource dependence : 
//  Author: haiwu.chen`
/*****************************************************************************/
PUBLIC uint32 MMIFMM_ConvertMimeTypeToSuffix(
    MIME_TYPE_E mime_type
);

/*****************************************************************************/
//  Description : get drm icon id by full path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetDrmIconId(uint16 *file_path_ptr);
#endif

///*****************************************************************************/
////  Description : ����ǰĿ¼�е��ļ����ݣ�
////  Global resource dependence :
////  Author: robert.lu
////  Note: �ļ����棬û��ʵ��
///*****************************************************************************/
//PUBLIC uint16 MMIFMM_GetCurrentPathFile(// �����ļ���Ŀ¼������
//                                           const wchar          *full_path_ptr,     // ȫ·��,��������
//                                           //MMIFMM_READ_TYPE_E   type,               // ����
//                                           MMIFMM_DATA_INFO_T   *data_ptr         // �������
//                                           //uint16               *output_filename_ptr// ������ݵ��ļ���
//                                            );
/*****************************************************************************/
//  Description : Ԥ���򣬽��ļ�����ǰ���ļ��ź�
//  Global resource dependence :
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIFMM_FilePreSort(MMIFMM_DATA_INFO_T* data_ptr);


/*****************************************************************************/
//  Description :MMIFMM_GetPicIndexData
//  Global resource dependence : 
//  Author: juan.wu
//  Note:   
/*****************************************************************************/
PUBLIC MMIFMM_PICTURE_INDEX_DATA_T  ** MMIFMM_GetPicIndexData(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

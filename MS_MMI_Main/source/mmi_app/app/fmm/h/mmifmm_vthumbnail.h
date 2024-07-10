/*****************************************************************************
** File Name:      mmifmm_vthumbnail.h                                       *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIFMM_VTHUMBNAIL_H_
#define _MMIFMM_VTHUMBNAIL_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guilistbox.h"
#include "mmifmm_export.h"

/*****************************************************************************/
//  Description : VIDEO�ļ���֧������ͼ���ܵ�״̬��ʼ�����б����ʾ���
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnail_Init (MMIFMM_PATH_INFO_T* current_path_ptr);

/*****************************************************************************/
//  Description :VIDEO�ļ���֧������ͼ������ ˢ��δ��ʾ��ICONLIST
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  FmmVideoThumbnailUpdateIcon (
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
);

/*****************************************************************************/
//  Description : VIDEO�ļ���֧������ͼ����ʵʱˢ�� ICONLIST�б�
//  Global resource dependence :
//  Author:liyan.zhu
//  Note:ForCR139456 �ع������ĺ���֢
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailUpdateIcon();

/*****************************************************************************/
//  Description : VIDEO�ļ���֧������ͼ�����йر�����ͼ��ȡ��������״̬
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailClose (void);

/*****************************************************************************/
//  Description : VIDEO�ļ���֧������ͼ�����������Ƶ����Ƶ�б�
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumbnailAdd (wchar  *video_fullpath_name);

/*****************************************************************************/
//  Description :VIDEO�ļ���֧������ͼ������ ͨ����Ƶȫ·����ȡ��ӦBMP ȫ·��
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIFMM_FmmVideoThumbnailGetPathname (wchar* thumbnail, uint16 *length, wchar* videopath);
/*****************************************************************************/
//  Description : change state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_VideoThumnailChangeState (
    GUILIST_SLIDE_STATE_E state,
    MMIFMM_DATA_INFO_T* list_data_ptr,
    MMIFMM_PATH_INFO_T* current_path_ptr
);
/*****************************************************************************/
//  Description : whether file is in default picture folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsDefaultVideoFolder (
											const wchar      *file_name,
											uint16      name_len
											);
/*****************************************************************************/
//  Description :VIDEO�ļ���֧������ͼ������ ɾ����ص��ļ�
//  Global resource dependence : 
//  Author: 
//  Note: cr130686
/*****************************************************************************/
PUBLIC void  MMIFMM_VideoThumbnailDeleteRelatedFile(wchar *file_name_ptr);

/*****************************************************************************/
//  Description :VIDEO�ļ���֧������ͼ������ ���ص�ǰҳ��δ��������Ƶ�б�
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIFMM_FmmVideoThumbnailLoadingMovieList (MMIFMM_DATA_INFO_T* list_data_ptr,
        MMIFMM_PATH_INFO_T* current_path_ptr
                                                       );

#endif
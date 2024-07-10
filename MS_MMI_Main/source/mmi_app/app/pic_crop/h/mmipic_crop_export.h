/*****************************************************************************
** File Name:      mmipic_crop_export.h                                                  *
** Author:           chunyou                                                     *
** Date:             12/25/2011                                                *
** Copyright:      2011 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_crop.c  mmipic_crop_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       chunyou           Create
******************************************************************************/
#ifndef _MMIPIC_CROP_EXPORT_H_
#define _MMIPIC_CROP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "sysTypes.h"
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
typedef enum
{
    MMIPIC_CROP_WALLPAPER = 0,      //ǽֽ�ü�
    MMIPIC_CROP_DEFAULT_SAVE,        //������Ĭ��·��
    MMIPIC_CROP_CUSTOMIZE_SAVE,        //�������Զ���·��
    MMIPIC_CROP_TYPE_MAX
}MMIPICCROP_TYPE_E;

typedef enum
{
    MMI_PICCROP_SUCCESS = 0,          //�ɹ�
    MMI_PICCROP_SUCCESS_NO_CROP,          //�ɹ�,û�вü���ͼƬ�ߴ�̫С
    MMI_PICCROP_ERR_NO_SPACE,       // �ռ䲻��
    MMI_PICCROP_ERR_NOT_SUPPORT,       // ��ʽ��֧��
    MMI_PICCROP_ERR_NO_MEMORY,       // �ڴ治��
    MMI_PICCROP_ERR_CANCEL,       // ȡ���ü�
    MMI_PICCROP_ERR_EXIT,            // ������Ҫ�˳�ͼƬ�ü��Ĵ���
}MMI_PICCROP_ERROR_TYPE_E;

typedef struct 
{
    MMIPICCROP_TYPE_E type;     //�ü����ͣ��������ֵһ��
    MMI_PICCROP_ERROR_TYPE_E err_type;  //��������
    wchar* full_path_ptr;       //�ļ�·��  
    uint16 full_path_len;       //�ļ�·������
    BOOLEAN      is_pre_wallpaper;      //�Ƿ��ǻָ�ǰһ��ǽֽ���������ֵһ��
} MMIPICCROP_CB_T;

typedef void (*MMIPICCROP_CALLBACK)(MMIPICCROP_CB_T* callback);
typedef struct 
{
    uint16      min_width;                /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint16      min_height;               /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint8       *src_buf_ptr;       /*!< ͼ��buffer��ַ @��Ϊpnullʱ��Ч */
    uint32      src_data_size;      /*!< ͼ��buffer��С @src_buf_ptr��Ϊpnull src_data_size��Ϊ0ʱ��Ч */
    wchar      *full_path_ptr;     /*!< ͼ���ļ�·�� @��Ϊpnullʱ��Ч */
    uint16      full_path_len;      /*!< ͼ���ļ�·������ @full_path_ptr��Ϊpnull full_path_len��Ϊ0ʱ��Ч */
    MMIPICCROP_TYPE_E crop_type;
    MMIPICCROP_CALLBACK callback;     /*�ü�ͼƬ�����full_path_len=0��ʾʧ��*/    
    BOOLEAN      is_pre_wallpaper;      //�Ƿ��ǻָ�ǰһ��ǽֽ
} MMIPICCROP_SRC_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_ExitFromPicCrop(void);

/*****************************************************************************/
//  Description : init pic crop module
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_InitModule(void);

/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICCROP_OpenPicCrop(
                                              MMIPICCROP_SRC_T* crop_src    //in:
                                              );
#endif


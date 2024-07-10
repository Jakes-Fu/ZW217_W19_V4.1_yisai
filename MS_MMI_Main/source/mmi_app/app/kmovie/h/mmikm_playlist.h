/*****************************************************************************
** File Name:      mmikm_playlist.h                                          *
** Author:                                                                   *
** Date:           14/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** ------------------------------------------------------------------------  *
** DATE             NAME              DESCRIPTION                            *
** 07/2011         tonny.chen          Create                                *
**																		     *
*****************************************************************************/

#ifndef _MMIKM_PLAYLIST_H_
#define _MMIKM_PLAYLIST_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define KM_FILE_SUFFIX_MAX_LEN                 7  //�ļ���׺����󳤶�

#define MEMORY_OFFSET     0x4d
#define THUMBNAIL_OFFSET  0x3d36
#define ITEM_OFFSET       0x18

typedef enum
{
    KM_PLAYLIST_DEFAULT, //ϵͳĿ¼�б�
    KM_PLAYLIST_MAX
}KM_PLAYLIST_TYPE_E;  //���ز����б�����

//km��Ƶ֧�ֵ�����
typedef enum
{
    MMIKM_VIDEO_TYPE_KMV,
    MMIKM_VIDEO_TYPE_SMV,  //smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    MMIKM_VIDEO_TYPE_HMV,  //hmv_support
#endif
    MMIKM_VIDEO_TYPE_MAX,
}MMIKM_VIDEO_TYPE_E;

//km������Ƶ�ļ���֧������Լ������Ϣ
typedef struct 
{
    uint8   suffix[KM_FILE_SUFFIX_MAX_LEN]; //��Ƶ�ļ���׺
    uint32  type;                           //��Ƶ�ļ�����
    BOOLEAN is_support;                     //�Ƿ�֧��
}KM_SUFFIX_T;

typedef struct
{
	uint16	item_size;
	uint16	item_is_true;
    uint8*	item_buffer_ptr; 
}CONTENT_ITEM_T;


typedef struct
{
    CONTENT_ITEM_T	item_thumbnail;
	CONTENT_ITEM_T	item_year;
	CONTENT_ITEM_T	item_time;
	CONTENT_ITEM_T	item_name;
	CONTENT_ITEM_T	item_actor;
	CONTENT_ITEM_T	item_synop;
	CONTENT_ITEM_T	item_type;
    uint8          *data_buf_ptr;
}KMV_INFORMATION_T;

/*****************************************************************************/
//  Description : ��ȡ���ز����б���б������Ŀ��
//  Global resource dependence :  none
//  Author: tonny.chen
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIKM_GetPlaylistNum(KM_PLAYLIST_TYPE_E  list_type );//andy.he_cr122288 

#ifdef __cplusplus
}
#endif

#endif //_MMIKM_PLAYLIST_H_


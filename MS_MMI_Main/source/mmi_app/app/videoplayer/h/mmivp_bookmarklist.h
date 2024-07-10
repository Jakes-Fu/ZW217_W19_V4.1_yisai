/**************************************************************************
** File Name:      mmivp_bookmarklist.h                                                   *
** Author:                                                                   *
** Date:           2010/3/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/18      apple.zhang       Create
******************************************************************************/

#ifndef _MMIVP_BOOKMARKLIST_H_
#define _MMIVP_BOOKMARKLIST_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmivp_export.h"
#include "mmivp_internal.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_BOOKMARKLIST_FILENAME         L"movie_bookmarklist.dat"  //�����ǩ��Ĭ���ļ���
#define VP_BOOKMARKLIST_FILENAME_LEN     22                         //�����ǩ��Ĭ���ļ�������
#define VP_BOOKMARKLIST_VER             "VPBOOKMARK.10.00.10"       //��ǩĬ���ļ��İ汾��
#define VP_BOOKMARKLIST_VER_LEN          19                         //��ǩĬ���ļ��汾�ų���

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/  

//��ǩ�ļ���ͷ��Ϣ
typedef struct
{
    uint8                      version[VP_BOOKMARKLIST_VER_LEN+1]; //��ǩ�ļ��汾��    
}VP_BOOKMARKLIST_HEAD_T;

//�ղؼ���Ϣ
typedef struct
{
    uint32                 item_count;                       //��ǩ����Ŀ
    VP_NET_MOVIE_INFO_T    item_info[VP_BOOKMARK_MAX_ITEM];                       //��ǩ��Ϣ
} VP_BOOKMARK_CONTEXT_T;
/*****************************************************************************/
//  Description : ��ʼ����ʷ��¼�б�������ʷ��¼��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_Load( void );

/*****************************************************************************/
//  Description : �ͷ���ǩ�б���Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_BookmarkList_Release( void );

/*****************************************************************************/
//  Description : ��ȡ��ǩ����
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_BookmarkList_GetNum( void );

/*****************************************************************************/
//  Description :��ȡĳһ��ǩ����Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_NET_MOVIE_INFO_T * VP_BookmarkList_GetInfo(uint32  bookmark_list_index);

/*****************************************************************************/
//  Description :ɾ��ĳһ��ǩ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelItem( uint32 index );

/*****************************************************************************/
//  Description :ɾ��ȫ����ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_BookmarkList_DelAllItem(void );

/*****************************************************************************/
//  Description :�����ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_AddItem(VP_NET_MOVIE_INFO_T     bookmark_data);

/*****************************************************************************/
//  Description :�޸���ǩ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_LIST_ERROR_E VP_BookmarkList_ModifyItem( 
                                                uint16               bookmark_list_index,
                                                VP_NET_MOVIE_INFO_T     bookmark_data
                                                );
/*****************************************************************************/
//  Description :��ȡԤ��CMCC��ý���Ż���ַ��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NET_MOVIE_INFO_T VP_GetCMCCAddressInfo(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif




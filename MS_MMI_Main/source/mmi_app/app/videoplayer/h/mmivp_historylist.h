/*****************************************************************************
** File Name:      mmivp_historylist.h                                                   *
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

#ifndef _MMIVP_HISTORYLIST_H_
#define _MMIVP_HISTORYLIST_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#ifdef MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#include "mmifmm_export.h"
#include "mmivp_export.h"
#include "mmivp_internal.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VP_MAX_HISTORY                  30                         //�����ʷ��¼����
#define VP_HISTORYLIST_FILENAME         L"movie_historylist.dat"   //������ʷ��¼��Ĭ���ļ���
#define VP_HISTORYLIST_FILENAME_LEN     21                         //������ʷ��¼��Ĭ���ļ�������
#define VP_HISTORYLIST_VER             "VPHISTORY.10.00.01"        //Ĭ���ļ��汾��
#define VP_HISTORYLIST_VER_LEN          18                         //Ĭ���ļ��İ汾�ų���

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/   

//��ʷ��¼�б��ļ���ͷ��Ϣ
typedef struct
{
    uint8                      version[VP_HISTORYLIST_VER_LEN+1]; //��ʷ��¼�ļ��汾��    
}VP_HISTORYLIST_HEAD_T;

//��ʷ��¼�б���Ϣ
typedef struct
{
    uint32                  item_count;         //��ʷ��¼����Ŀ
    VP_HISTORY_ITEM_T        *item_info_ptr;    //��ʷ��¼����Ϣ
}VP_HISTORY_CONTEXT_T;
/*****************************************************************************/
//  Description : ��ʼ����ʷ��¼�б�������ʷ��¼��Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_Load( void );

/*****************************************************************************/
//  Description : �ͷ���ʷ��¼��Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_HistoryList_Release( void );

/*****************************************************************************/
//  Description : �ͷ���ʷ��¼��Դ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 VP_HistoryList_GetNum( void );

/*****************************************************************************/
//  Description : ��ȡĳһ��ʷ��¼����Ϣ
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC const VP_HISTORY_ITEM_T*     VP_HistoryList_GetInfo(uint32    historylist_index);

/*****************************************************************************/
//  Description : ɾ��ĳһ��ʷ���ż�¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelItem( uint32 index );

/*****************************************************************************/
//  Description : ɾ��ȫ����ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_DelAllItem(void );

/*****************************************************************************/
//  Description : �����ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_AddItem(VP_HISTORY_ITEM_T  history_list_item);

/*****************************************************************************/
//  Description : Ϊ���ڲ��ŵ���ʷ��¼�������һ��ֹͣ���ŵ�λ��
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_SetLastPlayPoint(uint32        last_play_point );

/*****************************************************************************/
//  Description : �޸���ʷ��¼
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_HistoryList_ModifyItem(                                         
                                         uint32 last_play_point,
                                         uint32 total_time,
                                         uint32 video_height,
                                         uint32 video_width,
                                         uint32 media_length,
                                         uint16 title_len,
                                         wchar  *title_ptr);


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif //MMI_VIDEOPLAYER_HISTORYLIST_SUPPORT
#endif



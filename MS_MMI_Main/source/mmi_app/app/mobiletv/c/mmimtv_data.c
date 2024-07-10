/*****************************************************************************
** File Name:      mmimtv_save.c                                             *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv window table            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef CMMB_SUPPORT
#include "mtv_api.h"
#include "mmimtv_data.h"
#include "mmimtv_main.h"
#include "mmimtv_nv.h"
#include "mmifmm_export.h"
#include "os_api.h"
#include "mmidc_export.h"
#include "gui_ucs2b_converter.h"
#include "mmimtv_image.h"
#include "mmimtv_position.h"
#include "block_mem.h"
#include "guilcd.h"
#include "guires.h"
#include "lcd_backlight.h"
#include "mmimbbms_main.h"

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIMTV_SORT_BY_TIME                 8
#define MMIMTV_SORT_BY_READ                 4
#define MMIMTV_SORT_BY_GRADE                0
#define MMIMTV_SNAPSHOT_SPACE_AT_LEAST      (100*1024)
#define MMIMTV_RECORD_SPACE_AT_LEAST        (300*1024)
#define MMIMTV_ESG_SPACE_AT_LEAST			(200*1024)
#define MMIMTV_ENG_MODE_PROFILE_MAX_LEN     512

#define MMIMTV_SAVE_FILE_VERSION            0
#define MMIMTV_NS_FILE_VERSION              1

#define RGB565_R(prgb565) ((*(prgb565) >> 8) & 0xf8)
//return g
#define RGB565_G(prgb565) ((*(prgb565) >> 3) & 0xfc)
//return b 
#define RGB565_B(prgb565) ((*(prgb565) << 3)  & 0xf8)

#define EB_FULL_PATH    {'E', ':', '\\', 'm', 't', 'v', '\\', 'e', 'b', '.', 's', 'y', 's', 0}
#define NS_FULL_PATH    {'D', ':', '\\', 'm', 't', 'v', '\\', 'n', 's', '.', 's', 'y', 's', 0}
#define ESG_PATH        {'D', ':', '\\', 'm', 't', 'v', 0}
#define RECORD_PATH     {'D', ':', '\\', 'm', 't', 'v', '\\', 'v', 'i', 'd', 'e', 'o',0}
#define SNAPSHOT_PATH   {'D', ':', '\\', 'P', 'h', 'o', 't', 'o', 's', 0}
#ifdef MBBMS_SUPPORT
#define FAVORITE_SERVICE_FULL_PATH  {'D', ':', '\\', 'm', 't', 'v', '\\', 'f', 's', '.', 's', 'y', 's', 0}
#define FAVORITE_CONTENT_FULL_PATH  {'D', ':', '\\', 'm', 't', 'v', '\\', 'f', 'c', '.', 's', 'y', 's', 0}
#define ALARM_CONTENT_FULL_PATH     {'D', ':', '\\', 'm', 't', 'v', '\\', 'a', 'c', '.', 's', 'y', 's', 0}
#endif

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
typedef struct MMIMTV_DATA_DATA_tag
{    
    uint16                               snapshot_picture_name_len;
    
    MMIMTV_EB_NODE_T                     *eb_head_node_ptr;
    uint32                               eb_list_number;
    MMIMTV_SAVE_NETEID_INFO_T            *net_head_node_ptr;
    uint32                               net_list_number;
#ifdef MBBMS_SUPPORT
    MMIMBBMS_SAVE_SERVICE_INFO_T         *favorite_service_head_ptr;     //收藏频道链表头
    uint32                               favorite_service_list_num;     //收藏频道计数
    MMIMBBMS_SAVE_CONTENT_INFO_T         *favorite_content_head_ptr;     //收藏节目链表头
    uint32                               favorite_content_list_num;     //收藏节目计数
    MMIMBBMS_ALARM_CONTENT_INFO_T        *alarm_content_head_ptr;     // 提醒节目链表头
    uint32                               alarm_content_list_num;     //提醒节目计数
    wchar                                favorite_service_full_path[MMIMTV_FULL_PATH_MAX_LEN +1]; //收藏频道文件路径
    wchar                                favorite_content_full_path[MMIMTV_FULL_PATH_MAX_LEN +1]; //收藏节目文件路径
    wchar                                alarm_content_full_path[MMIMTV_FULL_PATH_MAX_LEN +1]; //提醒节目文件路径
#endif
    
    MMIMTV_SAVE_SERVICE_INFO_T           *service_playing_node_ptr;
    MMIMTV_SAVE_NETEID_INFO_T            *net_playing_node_ptr;
    
    wchar                                eb_full_path[MMIMTV_FULL_PATH_MAX_LEN +1];
    wchar                                ns_full_path[MMIMTV_FULL_PATH_MAX_LEN +1];
	uint8								 esg_path[MMIMTV_FULL_PATH_MAX_LEN +1];
    wchar                                full_path_record_file_name[MMIMTV_FULL_PATH_MAX_LEN + 1];
    wchar                                record_file_name[MMIMTV_FULL_PATH_MAX_LEN + 1];
    wchar                                path_snapshot_picture_name[MMIMTV_FULL_PATH_MAX_LEN + 1];
    wchar                                snapshot_picture_name[MMIMTV_FULL_PATH_MAX_LEN + 1];
    MMIMTV_SETTING_INFO_T                mtv_settings;
}MMIMTV_DATA_DATA;

static MMIMTV_DATA_DATA                  *s_data_data_ptr = PNULL;
#define MMIMTV_DATA_PTR                  (PNULL != s_data_data_ptr ? s_data_data_ptr : (MMIMTV_DATA_DATA*)MMIMTV_Aassert())
    

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
//TV信号强度映射表
//signal value: 0 - 100
//signal level: 0 - 5
LOCAL CONST uint8 s_signal_level_table[MMIMTV_SIGNAL_VALUE_MAX + 1] ={
        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
        5,5,5,5,5
};




/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: 紧急广播：eb.sys
//        网络/频道列表：ns.sys
//        收藏频道：fs.sys
//        收藏节目：fc.sys
/*****************************************************************************/
PUBLIC void MMIMTV_NewDataData(void)
{
    wchar eb_full_path[] = EB_FULL_PATH;
    wchar ns_full_path[] = NS_FULL_PATH;
	uint8 esg_path[] = ESG_PATH;
#ifdef MBBMS_SUPPORT
    wchar favorite_service_full_path[] = FAVORITE_SERVICE_FULL_PATH;
    wchar favorite_content_full_path[] = FAVORITE_CONTENT_FULL_PATH;
    wchar alarm_content_full_path[] = ALARM_CONTENT_FULL_PATH;
#endif

    if (s_data_data_ptr != PNULL)
    {
        return;
    }
    
    s_data_data_ptr = (MMIMTV_DATA_DATA*)SCI_ALLOC_APP(sizeof(MMIMTV_DATA_DATA));
    if(PNULL == s_data_data_ptr)
    {
        //SCI_TRACE_LOW:"MMIMTV_NewDataData s_data_data_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_149_112_2_18_2_40_12_539,(uint8*)"");
        return;
    }

    SCI_MEMSET(s_data_data_ptr, 0, sizeof(MMIMTV_DATA_DATA));
    SCI_MEMCPY(s_data_data_ptr->eb_full_path, eb_full_path, sizeof(eb_full_path));
    SCI_MEMCPY(s_data_data_ptr->ns_full_path, ns_full_path, sizeof(ns_full_path));
	SCI_MEMCPY(s_data_data_ptr->esg_path, esg_path, sizeof(esg_path));
#ifdef MBBMS_SUPPORT
    SCI_MEMCPY(s_data_data_ptr->favorite_service_full_path, favorite_service_full_path, sizeof(favorite_service_full_path));
    SCI_MEMCPY(s_data_data_ptr->favorite_content_full_path, favorite_content_full_path, sizeof(favorite_content_full_path));
    SCI_MEMCPY(s_data_data_ptr->alarm_content_full_path, alarm_content_full_path, sizeof(alarm_content_full_path));
#endif
}

/*****************************************************************************/
// 	Description : fill rect in mtv
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteDataData(void)
{
    if(PNULL != s_data_data_ptr)
    {
        SCI_Free(s_data_data_ptr);
        s_data_data_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIMTV_Aassert(void)    
{
    SCI_ASSERT(0); /*assert verified*/
    //return 0;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddServiceNode(
                                  MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                  MMIMTV_SAVE_SERVICE_INFO_T *service_node
                                  )
{
    MMIMTV_SAVE_SERVICE_INFO_T* cursor = PNULL;
    MMIMTV_SAVE_SERVICE_INFO_T* node = (MMIMTV_SAVE_SERVICE_INFO_T*)SCI_ALLOC_APP(sizeof(MMIMTV_SAVE_SERVICE_INFO_T));
    MMIMTV_SAVE_NETEID_INFO_T* src_node = PNULL;
    
    if(PNULL == net_node
        || PNULL == service_node)
    {
        return;
    }
    
    src_node = MMIMTV_GetNetNodeViaNetID(net_node->net_id);
    if(PNULL == src_node)
    {
        return;
    }
    
    SCI_MEMCPY(node, service_node, sizeof(MMIMTV_SAVE_SERVICE_INFO_T));
    cursor = src_node->service_head;
    if(PNULL == src_node->service_head)
    {
        src_node->service_head = node;
        if(src_node == MMIMTV_DATA_PTR->net_playing_node_ptr)
        {
            MMIMTV_DATA_PTR->service_playing_node_ptr = src_node->service_head;
        }
    }
    else
    {
        while(PNULL != cursor->service_next)
        {
            cursor = cursor->service_next;
        }
        cursor->service_next = node;
    }
    node->service_next = PNULL;
    src_node->service_number ++;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteServiceNode(
                                     MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                     MMIMTV_SAVE_SERVICE_INFO_T *service_node
                                     )
{
    if(PNULL == net_node
        || PNULL == service_node)
    {
        return;
    }
    
    if(PNULL == net_node->service_head)
    {
        return;
    }
    
    if(service_node == net_node->service_head)
    {
        net_node->service_head = net_node->service_head->service_next;
        SCI_FREE(service_node);
    }
    else
    {
        MMIMTV_SAVE_SERVICE_INFO_T* temp = net_node->service_head;
        while(service_node != temp->service_next)
        {
            if(PNULL != temp->service_next)
            {
                temp = temp->service_next;
            }
            else
            {
      		  //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_DeleteServiceNode find service node error!"
      		  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_269_112_2_18_2_40_13_540,(uint8*)"");
                return;
            }
        }
        if(service_node == MMIMTV_DATA_PTR->service_playing_node_ptr)
        {
            MMIMTV_DATA_PTR->service_playing_node_ptr = PNULL;
        }
        temp->service_next = service_node->service_next;
        SCI_FREE(service_node);
    }
    net_node->service_number --;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearServiceList(MMIMTV_SAVE_NETEID_INFO_T *node)
{
    MMIMTV_SAVE_SERVICE_INFO_T* cursor = PNULL;
    
    while(PNULL != node->service_head)
    {
        cursor = node->service_head;
        node->service_head = cursor->service_next;
        SCI_FREE(cursor);
        node->service_number --;
    }
    if(node == MMIMTV_DATA_PTR->net_playing_node_ptr)
    {
        MMIMTV_DATA_PTR->service_playing_node_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddNetNode(MMIMTV_SAVE_NETEID_INFO_T *node)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    MMIMTV_SAVE_NETEID_INFO_T* net = SCI_ALLOC_APP(sizeof(MMIMTV_SAVE_NETEID_INFO_T));
    
    if(PNULL == node)
    {
        return;
    }
    
    SCI_MEMCPY(net, node, sizeof(MMIMTV_SAVE_NETEID_INFO_T));
    if(PNULL == MMIMTV_DATA_PTR->net_head_node_ptr)
    {
        MMIMTV_DATA_PTR->net_head_node_ptr = net;
        MMIMTV_DATA_PTR->net_playing_node_ptr = MMIMTV_DATA_PTR->net_head_node_ptr;
    }
    else
    {
        while(PNULL != cursor->net_next)
        {
            cursor = cursor->net_next;
        }
        cursor->net_next = net;
    }
    net->net_next = PNULL;
    net->service_head = PNULL;
    MMIMTV_DATA_PTR->net_list_number ++;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteNetNode(MMIMTV_SAVE_NETEID_INFO_T *node)
{
    if(PNULL == node)
    {
        return;
    }
    
    if(node == MMIMTV_DATA_PTR->net_head_node_ptr)
    {
        MMIMTV_DATA_PTR->net_head_node_ptr = node->net_next;

        if(node == MMIMTV_DATA_PTR->net_playing_node_ptr)
        {
            //current node is playing net node
            MMIMTV_DATA_PTR->net_playing_node_ptr = MMIMTV_DATA_PTR->net_head_node_ptr;
            MMIMTV_DATA_PTR->service_playing_node_ptr = PNULL;
        }

        MMIMTV_ClearServiceList(node);
        SCI_FREE(node);
    }
    else
    {
        MMIMTV_SAVE_NETEID_INFO_T* temp = MMIMTV_DATA_PTR->net_head_node_ptr;
        while(node != temp->net_next)
        {
            if(PNULL != temp->net_next)
            {
                temp = temp->net_next;
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_DeleteNetNode, find net node error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_373_112_2_18_2_40_13_541,(uint8*)"");
                return;
            }
        }
        temp->net_next = node->net_next;
        
        if(node == MMIMTV_DATA_PTR->net_playing_node_ptr)
        {
            //current node is playing net node
            MMIMTV_DATA_PTR->net_playing_node_ptr = MMIMTV_DATA_PTR->net_head_node_ptr;
            MMIMTV_DATA_PTR->service_playing_node_ptr = PNULL;
        }

        MMIMTV_ClearServiceList(node);
        SCI_FREE(node);
    }
    MMIMTV_DATA_PTR->net_list_number --;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetNetListNumber(void)
{
    return MMIMTV_DATA_PTR->net_list_number;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetServiceListNumber(const MMIMTV_SAVE_NETEID_INFO_T* node)
{
    if(PNULL == node)
    {
        return 0;
    }
    
    return node->service_number;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearNetList(void)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = PNULL;
    
    if (PNULL != s_data_data_ptr)
    {
        while(PNULL != MMIMTV_DATA_PTR->net_head_node_ptr)
        {
            cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
            MMIMTV_DATA_PTR->net_head_node_ptr = cursor->net_next;
            if(PNULL != cursor->service_head)
            {
                MMIMTV_ClearServiceList(cursor);
            }
            SCI_FREE(cursor);
            MMIMTV_DATA_PTR->net_list_number --;
        }
        MMIMTV_DATA_PTR->net_playing_node_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetServiceNodeViaIndex(
                                                                 const MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                                                 uint32 index
                                                                 )
{
    MMIMTV_SAVE_SERVICE_INFO_T* cursor = PNULL;
    
    if(PNULL == net_node)
    {
        return PNULL;
    }
    
    if(PNULL == net_node->service_head 
        || index > (net_node->service_number - 1))
    {
        return PNULL;
    }

    cursor = net_node->service_head;
    while(index --)
    {
        cursor = cursor->service_next;
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetServiceNodeViaID(
                                                                    uint32 net_id,
                                                                    uint32 service_id
                                                                    )
{
    MMIMTV_SAVE_SERVICE_INFO_T* cursor1 = PNULL;
    MMIMTV_SAVE_NETEID_INFO_T* cursor2 = MMIMTV_DATA_PTR->net_head_node_ptr;

    while(PNULL != cursor2)
    {
        if(cursor2->net_id == net_id)
        {
            cursor1 = cursor2->service_head;
            while(PNULL != cursor1)
            {
                if(cursor1->service_id == service_id)
                {
                    return cursor1;
                }
                cursor1 = cursor1->service_next;
            }
        }
        cursor2 = cursor2->net_next;
    }
    return PNULL;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaIndex(uint32 index)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_GetNetNodeViaIndex, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_508_112_2_18_2_40_13_542,(uint8*)"d", index);
    
    if(index > MMIMTV_DATA_PTR->net_list_number)
    {
        return PNULL;
    }
    
    if(PNULL == MMIMTV_DATA_PTR->net_head_node_ptr)
    {
        return PNULL;
    }
    
    while(index --)
    {
        if(PNULL == cursor)
        {
            return PNULL;
        }
        
        cursor = cursor->net_next;
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaFrequency(uint32 frequency)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    while(PNULL != cursor)
    {
        if(frequency == cursor->center_freqquency)
        {
            return cursor;
        }
        cursor = cursor->net_next;
    }
    return PNULL;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetNetNodeViaNetID(uint32 net_id)
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    while(PNULL != cursor)
    {
        if(net_id == cursor->net_id)
        {
            return cursor;
        }
        cursor = cursor->net_next;
    }
    return PNULL;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetPlayingServiceIndexViaNode(const MMIMTV_SAVE_SERVICE_INFO_T *node)
{
    MMIMTV_SAVE_SERVICE_INFO_T* cursor = PNULL;
    uint32 index = 0;
    
    if(PNULL == MMIMTV_DATA_PTR->net_playing_node_ptr || PNULL == node || PNULL ==MMIMTV_DATA_PTR->net_playing_node_ptr->service_head)
    {
        return 0;
    }
    cursor = MMIMTV_DATA_PTR->net_playing_node_ptr->service_head;
    while(PNULL != cursor)
    {
        if(node == cursor)
        {
            return index;
        }
        
        index ++;
        cursor = cursor->service_next;
    }
    return 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteServiceNodeViaIndex(
                                             MMIMTV_SAVE_NETEID_INFO_T *net_node,
                                             uint32 index
                                             )
{
    MMIMTV_DeleteServiceNode(net_node, MMIMTV_GetServiceNodeViaIndex(net_node, index));
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteNetNodeViaIdex(uint32 index)
{
    MMIMTV_DeleteNetNode(MMIMTV_GetNetNodeViaIndex(index));
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_GetPlayingNetNode(void)
{
    return MMIMTV_DATA_PTR->net_playing_node_ptr;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_GetPlayingServiceNode(void)
{
    return MMIMTV_DATA_PTR->service_playing_node_ptr;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPlayingNetNode(MMIMTV_SAVE_NETEID_INFO_T *node)
{
    if(PNULL == node)
    {
        return;
    }
    
    MMIMTV_DATA_PTR->net_playing_node_ptr = node;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPlayingServiceNode(MMIMTV_SAVE_SERVICE_INFO_T *node)
{
    MMIMTV_DATA_PTR->service_playing_node_ptr = node;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_SetPlayingNetServiceNodeViaID(
                                                    uint32 net_id,
                                                    uint32 service_id
                                                    )
{
    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    MMIMTV_SAVE_SERVICE_INFO_T* node = PNULL;

    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_SetPlayingNetServiceNodeViaID, net_id=%d, service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_669_112_2_18_2_40_14_543,(uint8*)"dd", net_id, service_id);
    while(PNULL != cursor)
    {
        if(net_id == cursor->net_id)
        {
            MMIMTV_DATA_PTR->net_playing_node_ptr = cursor;
            node = cursor->service_head;
            while(PNULL != node)
            {
                if(service_id == node->service_id)
                {
                    MMIMTV_DATA_PTR->service_playing_node_ptr = node;
                    return TRUE;
                }
                node = node->service_next;
            }
        }
        cursor = cursor->net_next;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:查询不到则返回头节点
/*****************************************************************************/
PUBLIC  uint32 MMIMTV_GetNextServiceID(uint32 net_id,
                                       uint32 service_id
                                       )
{

    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    MMIMTV_SAVE_SERVICE_INFO_T* node = PNULL;
    while(PNULL != cursor)
    {
        if(net_id == cursor->net_id)
        {
            node = cursor->service_head;
            while(PNULL != node)
            {
                if(service_id == node->service_id)
                {
                    if(PNULL != node->service_next)
                    {
                        node = node->service_next;
                    }
                    else
                    {
                        node = cursor->service_head;
                    }
                    return node->service_id;
                }
                node = node->service_next;
            }
            return cursor->service_head->service_id;
        }
        cursor = cursor->net_next;
    }
    return 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:查询不到则返回头节点
/*****************************************************************************/
PUBLIC  uint32 MMIMTV_GetPreServiceID(uint32 net_id,
                                       uint32 service_id
                                       )
{

    MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    MMIMTV_SAVE_SERVICE_INFO_T* node = PNULL;
    while(PNULL != cursor)
    {
        if(net_id == cursor->net_id)
        {
            node = cursor->service_head;
            if(service_id == node->service_id)
            {
                while(PNULL != node->service_next)
                {
                    node = node->service_next;
                }
            }
            else
            {
                while(PNULL != node->service_next && service_id != node->service_next->service_id)
                {
                    node = node->service_next;
                }
            }
            return node->service_id;
        }
        cursor = cursor->net_next;
    }
    return 0;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_SAVE_SERVICE_INFO_T* MMIMTV_ChangeService(BOOLEAN is_next)
{
    if(PNULL == MMIMTV_DATA_PTR->service_playing_node_ptr || PNULL == MMIMTV_DATA_PTR->net_playing_node_ptr)
    {
        return PNULL;
    }

    if(1 == MMIMTV_DATA_PTR->net_playing_node_ptr->service_number)
    {
        return PNULL;
    }
    if(is_next)
    {
        if(PNULL != MMIMTV_DATA_PTR->service_playing_node_ptr->service_next)
        {
            MMIMTV_DATA_PTR->service_playing_node_ptr = MMIMTV_DATA_PTR->service_playing_node_ptr->service_next;
        }
        else
        {
            MMIMTV_DATA_PTR->service_playing_node_ptr = MMIMTV_DATA_PTR->net_playing_node_ptr->service_head;
        }
    }
    else
    {
        MMIMTV_SAVE_SERVICE_INFO_T* cursor = MMIMTV_DATA_PTR->net_playing_node_ptr->service_head;
        if(MMIMTV_DATA_PTR->net_playing_node_ptr->service_head == MMIMTV_DATA_PTR->service_playing_node_ptr)
        {
            while(PNULL != cursor->service_next)
            {
                cursor = cursor->service_next;
            }
        }
        else
        {
            while(PNULL != cursor->service_next && MMIMTV_DATA_PTR->service_playing_node_ptr != cursor->service_next)
            {
                cursor = cursor->service_next;
            }
        }
        MMIMTV_DATA_PTR->service_playing_node_ptr = cursor;
    }
    return MMIMTV_DATA_PTR->service_playing_node_ptr;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const MMIMTV_SAVE_NETEID_INFO_T* MMIMTV_ChangeNet(BOOLEAN is_next)
{
    if(PNULL == MMIMTV_DATA_PTR->net_playing_node_ptr)
    {
        return PNULL;
    }
    
    if(1 == MMIMTV_DATA_PTR->net_list_number)
    {
        return PNULL;
    }
    if(is_next)
    {
        if(PNULL != MMIMTV_DATA_PTR->net_playing_node_ptr->net_next)
        {
            MMIMTV_DATA_PTR->net_playing_node_ptr = MMIMTV_DATA_PTR->net_playing_node_ptr->net_next;
        }
        else
        {
            MMIMTV_DATA_PTR->net_playing_node_ptr = MMIMTV_DATA_PTR->net_head_node_ptr;
        }
    }
    else
    {
        MMIMTV_SAVE_NETEID_INFO_T* cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
        if(MMIMTV_DATA_PTR->net_head_node_ptr == MMIMTV_DATA_PTR->net_playing_node_ptr)
        {
            while(PNULL != cursor->net_next)
            {
                cursor = cursor->net_next;
            }
        }
        else
        {
            while(PNULL != cursor->net_next && MMIMTV_DATA_PTR->net_playing_node_ptr != cursor->net_next)
            {
                cursor = cursor->net_next;
            }
        }
        MMIMTV_DATA_PTR->net_playing_node_ptr = cursor;
    }
    MMIMTV_DATA_PTR->service_playing_node_ptr = MMIMTV_DATA_PTR->net_playing_node_ptr->service_head;
    return MMIMTV_DATA_PTR->net_playing_node_ptr;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_AddEBNode(MMIMTV_EB_NODE_T *node)
{
    MMIMTV_EB_NODE_T* eb = PNULL;
    
    if(PNULL == node)
    {
        return;
    }
    
    if(MMIMTV_DATA_PTR->eb_list_number > MMIMTV_EB_MAX_NUMBER)
    {
        return;
    }
    
    eb = (MMIMTV_EB_NODE_T*)SCI_ALLOC_APP(sizeof(MMIMTV_EB_NODE_T));
    if(PNULL != eb)
    {
        SCI_MEMCPY(eb, node, sizeof(MMIMTV_EB_NODE_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_AddEBNode alloc memory error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_889_112_2_18_2_40_14_544,(uint8*)"");
        return;
    }
    
    eb->eb_next = MMIMTV_DATA_PTR->eb_head_node_ptr;
    MMIMTV_DATA_PTR->eb_head_node_ptr = eb;
    MMIMTV_DATA_PTR->eb_list_number ++;
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_AddEBNode eb_list_number = %d", MMIMTV_DATA_PTR->eb_list_number);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteEBNode(MMIMTV_EB_NODE_T *node)
{
    if(PNULL == MMIMTV_DATA_PTR->eb_head_node_ptr)
    {
        return;
    }
    
    if(PNULL == node)
    {
        return;
    }

    if(node == MMIMTV_DATA_PTR->eb_head_node_ptr)
    {
        MMIMTV_DATA_PTR->eb_head_node_ptr = node->eb_next;
        SCI_FREE(node);
    }
    else
    {
        MMIMTV_EB_NODE_T* temp = MMIMTV_DATA_PTR->eb_head_node_ptr;
        while(node != temp->eb_next)
        {
            if(PNULL != temp->eb_next)
            {
                temp = temp->eb_next;
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_DeleteEBNode find eb node error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_931_112_2_18_2_40_14_545,(uint8*)"");
            }
        }
        temp->eb_next = node->eb_next;
        SCI_FREE(node);
    }
    MMIMTV_DATA_PTR->eb_list_number --;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_EB_NODE_T* MMIMTV_GetEBNodeViaIndex(uint32 index)
{
    MMIMTV_EB_NODE_T* cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_GetEBNodeViaIndex, index = %d", index);
    
    if(PNULL == MMIMTV_DATA_PTR->eb_head_node_ptr
        || (index > MMIMTV_DATA_PTR->eb_list_number))
    {
        return PNULL;
    }
    
    while(index --)
    {
        if(PNULL == cursor)
        {
            return PNULL;
        }
        
        cursor = cursor->eb_next;
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : is have unread EB msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsHaveTobeReadEBMsg(void)
{
    MMIMTV_EB_NODE_T* cursor = PNULL;
    
    if(PNULL != s_data_data_ptr)
    {
        cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
        while(PNULL != cursor)
        {
            //有未读EB消息
            if(!cursor->eb_is_read)
            {
                return TRUE;
            }
            else
            {
                cursor = cursor->eb_next;
            }
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : get unread EB msg num
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMTV_GetUnreadEBMsgNum(void)
{
    MMIMTV_EB_NODE_T* cursor = PNULL;
    uint16 num = 0;
    
    if(PNULL != s_data_data_ptr)
    {
        cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
        while(PNULL != cursor)
        {
            //有未读EB消息
            if(!cursor->eb_is_read)
            {
                num++;
            }
            cursor = cursor->eb_next;
        }
    }
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_GetUnreadEBMsgNum, num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1018_112_2_18_2_40_14_546,(uint8*)"d", num);
    return num;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_DeleteEBNodeViaIndex(uint32 index)
{
    MMIMTV_DeleteEBNode(MMIMTV_GetEBNodeViaIndex(index));
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ClearEBList(void)
{
    MMIMTV_EB_NODE_T* cursor = PNULL;
    
    if(PNULL != s_data_data_ptr)
    {
        while(PNULL != MMIMTV_DATA_PTR->eb_head_node_ptr)
        {
            cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
            MMIMTV_DATA_PTR->eb_head_node_ptr = cursor->eb_next;
            SCI_FREE(cursor);
        }
        MMIMTV_DATA_PTR->eb_list_number = 0;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void EBSort(
                  uint8 var_offset, 
                  BOOLEAN big_to_small
                  )
{
    MMIMTV_EB_NODE_T* head = MMIMTV_DATA_PTR->eb_head_node_ptr;
    MMIMTV_EB_NODE_T* cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
    MMIMTV_EB_NODE_T* object = MMIMTV_DATA_PTR->eb_head_node_ptr;
    MMIMTV_EB_NODE_T* front_object = MMIMTV_DATA_PTR->eb_head_node_ptr;
    MMIMTV_EB_NODE_T* temp_head = head;
    uint32 value = 0;
    uint32 temp_value = 0;
    BOOLEAN res = 0;
    
    if(PNULL == MMIMTV_DATA_PTR->eb_head_node_ptr)
    {
        return;
    }
    
    MMIMTV_DATA_PTR->eb_head_node_ptr = PNULL;
    while (PNULL != head)
    {
        cursor = head;
        front_object = head;
        object = head;
        value = *((uint32*)((char*)cursor + var_offset));
        while (cursor->eb_next != PNULL)
        {
            temp_value = *((uint32*)((char*)cursor->eb_next + var_offset));
            res = value < temp_value;
            if(!big_to_small)
            {
                res = !res;
            }
            if(res && value != temp_value)
            {
                front_object = cursor;
                object = cursor->eb_next;
                value = temp_value;
            }
            cursor = cursor->eb_next;
        }
        if(head == object)
        {
            head = object->eb_next;
        }
        else
        {
            front_object->eb_next = object->eb_next;
        }        
        if(PNULL == MMIMTV_DATA_PTR->eb_head_node_ptr)
        {
            MMIMTV_DATA_PTR->eb_head_node_ptr = object;
        }
        else
        {
            temp_head->eb_next = object;
        }
        temp_head = object;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EBSortByTime(void)
{
    EBSort(MMIMTV_SORT_BY_TIME, TRUE);
    //EBSort(MMIMTV_SORT_BY_READ, FALSE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EBSortByGrade(void)
{
    //EBSort(MMIMTV_SORT_BY_TIME, TRUE);
    EBSort(MMIMTV_SORT_BY_GRADE, FALSE);
    //EBSort(MMIMTV_SORT_BY_READ, FALSE);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetEBListNumber(void)
{
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_GetEBListNumber eb_list_number = %d", MMIMTV_DATA_PTR->eb_list_number);
    return MMIMTV_DATA_PTR->eb_list_number;
}

/*****************************************************************************/
// 	Description : 读取EB文件信息，生成紧急广播信息列表
//	Global resource dependence : none
//  Author: 
//	Note: EB文件结构 |-version-|-eb_num-|---------------eb_nodes--------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_ReadEBFile(void)
{
    uint32              read_size  = 0;
    uint32              list_num   = 0;
    uint32              write_offset = 0;
    MMIFILE_HANDLE      file_handle = 0;  
    uint32              struct_len     = sizeof(MMIMTV_EB_NODE_T);
    uint32              version = 0;
    MMIFILE_DEVICE_E    dev     = MMI_DEVICE_SDCARD;
    BOOLEAN             file_ok = FALSE;
    
    if(PNULL != MMIMTV_DATA_PTR->eb_head_node_ptr)
    {
        return FALSE;
    }
   
    if(PNULL == MMIMTV_DATA_PTR->eb_full_path)
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_ReadEBFile service_head_ptr not null! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1170_112_2_18_2_40_15_547,(uint8*)"");
        return FALSE;
    }

    //遍历所有DEV，查找文件是否存在
    for(dev = MMI_DEVICE_NUM -1; dev >= MMI_DEVICE_UDISK; dev++)
    {
        MMIMTV_DATA_PTR->eb_full_path[0] = *MMIAPIFMM_GetDevicePath((dev));
        if(MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->eb_full_path, (uint16)MMIAPICOM_Wstrlen(MMIMTV_DATA_PTR->eb_full_path)))
        {
            break;
        }
    }
    if(dev < MMI_DEVICE_UDISK)
    {
        return FALSE;//不存在
    }
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->eb_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
	if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_ReadEBFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1191_112_2_18_2_40_15_548,(uint8*)"");
	}

    //读取版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &version, sizeof(uint32), &read_size, NULL))
    {
		//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1197_112_2_18_2_40_15_549,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //确认版本信息读取成功
    if(0 == read_size)
    {
        //失败，文件为空
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //版本信息确认
    if(version != MMIMTV_SAVE_FILE_VERSION)
    {
        //版本不匹配
		//SCI_TRACE_LOW:"[MMIMTV]: EB file version error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1212_112_2_18_2_40_15_550,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //获取EB节点个数
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, NULL))
        {
			//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1223_112_2_18_2_40_15_551,(uint8*)"");
			MMIAPIFMM_CloseFile(file_handle);
			return FALSE;
        }
		//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_ReadEBFile total list num = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1227_112_2_18_2_40_15_552,(uint8*)"d", list_num);
    }
    else
    {
		//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_SetFilePointer error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1231_112_2_18_2_40_15_553,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //逐个添加EB节点
    //write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
    while (list_num)
    {
		write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
		//SCI_TRACE_LOW("[MMIMTV]: MMIMTV_ReadEBFile list num = %d  offset:%d", list_num, write_offset);
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
        {
            MMIMTV_EB_NODE_T node = {0};
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &node, struct_len, &read_size, NULL))
            {
                MMIMTV_AddEBNode(&node);
            }
            else
            {
				//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1251_112_2_18_2_40_15_554,(uint8*)"");
				MMIAPIFMM_CloseFile(file_handle);
				return FALSE;
            }
        }
        else
        {
			//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_SetFilePointer error! "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1258_112_2_18_2_40_15_555,(uint8*)"");
			MMIAPIFMM_CloseFile(file_handle);
			return FALSE;
        }
        //write_offset = 2 * sizeof(uint32) + list_num * struct_len;

		list_num--;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_WriteEBFile(void)
{
    uint32                  list_num = MMIMTV_DATA_PTR->eb_list_number;
    uint32                  write_offset = 0;
    uint32                  write_len = 0;
    MMIFILE_HANDLE          file_handle = 0;
    MMIMTV_EB_NODE_T*       cursor = MMIMTV_DATA_PTR->eb_head_node_ptr;
    uint32                  struct_size = sizeof(MMIMTV_EB_NODE_T);
    uint32                  total_size = MMIMTV_DATA_PTR->eb_list_number * struct_size + 2 * sizeof(uint32);
    uint32                  version = MMIMTV_SAVE_FILE_VERSION;
    MMIFILE_DEVICE_E        dev = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E        file_type = MMI_DEVICE_UDISK;
    
    dev = MMIAPIFMM_GetDeviceTypeByPath(MMIMTV_DATA_PTR->eb_full_path, MMIFILE_DEVICE_LEN);
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((dev), total_size, &file_type))
    {
        return FALSE;
    }
    if(file_type < MMI_DEVICE_NUM) MMIMTV_DATA_PTR->eb_full_path[0] = *MMIAPIFMM_GetDevicePath(file_type);
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->eb_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
	if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_WriteEBFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1297_112_2_18_2_40_15_556,(uint8*)"");
		return FALSE;
	}
	
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &version, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //write_offset += sizeof(uint32);
    //list_num = 0;
    while(PNULL != cursor)
    {
        //list_num ++;
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_size, &write_len, NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //write_offset += struct_size;
        cursor = cursor->eb_next;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_ReadNSFile(void)
{
    uint32                          read_size  = 0;
    MMIFILE_HANDLE                  file_handle = 0;
    MMIMTV_SAVEFILE_INFO_T          fileinfo = {0};
    uint32                          struct_net_size = sizeof(MMIMTV_SAVE_NETEID_INFO_T);
    uint32                          struct_service_size = sizeof(MMIMTV_SAVE_SERVICE_INFO_T);
    uint32                          struct_fileinfo_size = sizeof(MMIMTV_SAVEFILE_INFO_T);
    MMIFILE_DEVICE_E                dev     = MMI_DEVICE_UDISK;
    BOOLEAN                         file_ok = FALSE;
    if(PNULL != MMIMTV_DATA_PTR->net_head_node_ptr)
    {
        return FALSE;
    }
    
    //遍历所有DEV，查找文件是否存在
    for(dev=MMI_DEVICE_UDISK;dev<MMI_DEVICE_NUM;dev++)
    {
        MMIMTV_DATA_PTR->ns_full_path[0] = *MMIAPIFMM_GetDevicePath((dev));
        if(MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->ns_full_path, (uint16)MMIAPICOM_Wstrlen(MMIMTV_DATA_PTR->ns_full_path)))
        {
            break;
        }
    }
    if(dev >= MMI_DEVICE_NUM)
    {
        return FALSE;//不存在
    }
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->ns_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
	if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_ReadNSFile open file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1365_112_2_18_2_40_15_557,(uint8*)"");
		return FALSE;
	}
    
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &fileinfo, struct_fileinfo_size, &read_size, NULL))
    {
		//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1371_112_2_18_2_40_15_558,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    if(fileinfo.version != MMIMTV_NS_FILE_VERSION)
    {
		//SCI_TRACE_LOW:"[MMIMTV]: NS file version error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1377_112_2_18_2_40_15_559,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        MMIAPIFMM_DeleteFile(MMIMTV_DATA_PTR->ns_full_path, PNULL);
        return FALSE;
    }
    while (fileinfo.net_list_number --)
    {
        MMIMTV_SAVE_NETEID_INFO_T net = {0};
        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &net, struct_net_size, &read_size, NULL))
        {
            uint32 number = net.service_number;

            net.service_number = 0;     
            MMIMTV_AddNetNode(&net);            
            while(number --)
            {
                MMIMTV_SAVE_SERVICE_INFO_T ser = {0};
				
                if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &ser, struct_service_size, &read_size, NULL))
                {
                    MMIMTV_AddServiceNode(&net, &ser);
                }
				else
				{
					//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1401_112_2_18_2_40_15_560,(uint8*)"");
					MMIAPIFMM_CloseFile(file_handle);
					return FALSE;
				}
            }
        }
        else
        {
			//SCI_TRACE_LOW:"[MMIMTV]: MMIAPIFMM_ReadFile error! "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1409_112_2_18_2_40_15_561,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_WriteNSFile(void)
{
    uint32                          write_len = 0;
    MMIFILE_HANDLE                  file_handle = 0;
    MMIMTV_SAVE_NETEID_INFO_T*      cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    MMIMTV_SAVE_SERVICE_INFO_T*     service = PNULL;
    MMIMTV_SAVEFILE_INFO_T          fileinfo = {0};
    uint32                          struct_net_size = sizeof(MMIMTV_SAVE_NETEID_INFO_T);
    uint32                          struct_service_size = sizeof(MMIMTV_SAVE_SERVICE_INFO_T);
    uint32                          total_size = 0;
    MMIFILE_DEVICE_E                dev       = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E                file_type = MMI_DEVICE_UDISK;
    
    while(cursor != PNULL)
    {
        total_size += cursor->service_number * struct_service_size;
        cursor = cursor->net_next;
    }
    total_size = total_size + MMIMTV_DATA_PTR->net_list_number * struct_net_size + sizeof(uint32) + sizeof(MMIMTV_SAVEFILE_INFO_T);

    dev = MMIAPIFMM_GetDeviceTypeByPath(MMIMTV_DATA_PTR->ns_full_path, MMIFILE_DEVICE_LEN);
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((dev), total_size, &file_type))
    {
        return FALSE;
    }
    MMIMTV_DATA_PTR->ns_full_path[0] = *MMIAPIFMM_GetDevicePath(file_type);
    
    fileinfo.version = MMIMTV_NS_FILE_VERSION;
    fileinfo.net_list_number = MMIMTV_DATA_PTR->net_list_number;
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->ns_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
	if(SFS_INVALID_HANDLE == file_handle)
	{
		//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_WriteNSFile create file error! "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1454_112_2_18_2_40_15_562,(uint8*)"");
		return FALSE;
	}
	
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &fileinfo, sizeof(MMIMTV_SAVEFILE_INFO_T), &write_len, NULL))
    {
		MMIAPIFMM_CloseFile(file_handle);
		return FALSE;
    }
    cursor = MMIMTV_DATA_PTR->net_head_node_ptr;
    while(cursor != PNULL)
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_net_size, &write_len, NULL))
        {
			MMIAPIFMM_CloseFile(file_handle);
			return FALSE;
        }
        service = cursor->service_head;
        cursor = cursor->net_next;
        while(service != PNULL)
        {
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, service, struct_service_size, &write_len, NULL))
            {
				MMIAPIFMM_CloseFile(file_handle);
				return FALSE;
            }
            service = service->service_next;
        }
    } 
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetRecordFullFilePath(void)
{
    return MMIMTV_DATA_PTR->full_path_record_file_name;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIMTV_GetRecordFileName(void)
{
    return MMIMTV_DATA_PTR->record_file_name;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_CreateRecordFileName(void)
{
    char temp_name[MMIMTV_FULL_PATH_MAX_LEN] = {0};
    wchar path[] = RECORD_PATH;
    uint16 full_path_len = 0;
    uint32 step = 1;
    uint32 circle = 1;
    uint32 name_id = 0;
    uint32 id_max = MMIMTV_FILE_NAME_ID_MAX;
    wchar  *full_name_ptr = PNULL;
    uint16 full_name_count = 0;
    wchar  *name_ptr = PNULL;
    uint16 name_count = 0;
    MMIFILE_DEVICE_E  file_type = MMI_DEVICE_UDISK;
    
    full_name_ptr   = MMIMTV_DATA_PTR->full_path_record_file_name;
    full_name_count = MMIMTV_FULL_PATH_MAX_LEN * sizeof(wchar);
    SCI_MEMSET(full_name_ptr, 0, full_name_count);
    
    name_ptr    = MMIMTV_DATA_PTR->record_file_name;
    name_count  = MMIMTV_FULL_PATH_MAX_LEN * sizeof(wchar);
    SCI_MEMSET(name_ptr, 0, name_count);

    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((MMIMTV_DATA_PTR->mtv_settings.record_store), MMIMTV_RECORD_SPACE_AT_LEAST, &file_type))
    {
        return FALSE;
    }
    path[0] = *MMIAPIFMM_GetDevicePath(file_type);

    name_id = MMIMTV_DATA_PTR->mtv_settings.record_file_id;
    while(1)  /*lint !e716 */
    {
        name_id += step;
        step = step * 2;
        if(name_id >= id_max)
        {
            circle ++;
            if(circle >= id_max)
            {
                circle = 1;
                id_max = id_max * 10;
            }
            name_id = circle;
            step = 1;
        }
        MMIAPIDC_GetFileIDString(temp_name, "MTV", name_id, id_max, "mtv");
        
        MMIAPICOM_StrToWstr((uint8*)temp_name, MMIMTV_DATA_PTR->record_file_name);
        
        full_path_len = MMIAPIDC_CombinePathName(MMIMTV_DATA_PTR->full_path_record_file_name, MMIMTV_FULL_PATH_MAX_LEN, 
            path, MMIMTV_DATA_PTR->record_file_name);
   
        if(!MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->full_path_record_file_name, full_path_len))
        {
            break;
        }
    }
    MMIMTV_DATA_PTR->mtv_settings.record_file_id = name_id;
    return TRUE;    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetSnapshotPicturePath(void)
{
    return MMIMTV_DATA_PTR->path_snapshot_picture_name;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMTV_GetSnapshotPictureNameLength(void)
{
    return MMIMTV_DATA_PTR->snapshot_picture_name_len;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetSnapshotPictureName(void)
{
    return MMIMTV_DATA_PTR->snapshot_picture_name;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_CreateSnapshotPictureName(void)
{
    char temp_name[MMIMTV_FULL_PATH_MAX_LEN] = {0};
    wchar path[] = SNAPSHOT_PATH;
    uint16 full_path_len = 0;
    uint32 step = 1;
    uint32 circle = 1;
    uint32 name_id = 0;
    uint32 id_max = MMIMTV_FILE_NAME_ID_MAX;
    wchar  *full_name_ptr = PNULL;
    uint16 full_name_count = 0;
    wchar  *name_ptr = PNULL;
    uint16 name_count = 0;
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_UDISK;
    
    full_name_ptr   = MMIMTV_DATA_PTR->path_snapshot_picture_name;
    full_name_count = MMIMTV_FULL_PATH_MAX_LEN * sizeof(wchar);
    SCI_MEMSET(full_name_ptr, 0, full_name_count);
    
    name_ptr    = MMIMTV_DATA_PTR->snapshot_picture_name;
    name_count  = MMIMTV_FULL_PATH_MAX_LEN * sizeof(wchar);
    SCI_MEMSET(name_ptr, 0, name_count);

    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((MMIMTV_DATA_PTR->mtv_settings.snapshot_store), MMIMTV_SNAPSHOT_SPACE_AT_LEAST, &file_type))
    {
        return FALSE;
    }
    path[0] = *MMIAPIFMM_GetDevicePath(file_type);
   
    name_id = MMIMTV_DATA_PTR->mtv_settings.snapshot_file_id;
    while(1)  /*lint !e716 */
    {
        name_id += step;
        step = step * 2;
        if(name_id >= id_max)
        {
            circle ++;
            if(circle >= id_max)
            {
                circle = 1;
                id_max = id_max * 10;
            }
            name_id = circle;
            step = 1;
        }
        MMIMTV_DATA_PTR->snapshot_picture_name_len = MMIAPIDC_GetFileIDString(temp_name, "MTV", name_id, id_max, "jpg");
        
        MMIAPICOM_StrToWstr((uint8*)temp_name, MMIMTV_DATA_PTR->snapshot_picture_name);
        
        full_path_len = MMIAPIDC_CombinePathName(MMIMTV_DATA_PTR->path_snapshot_picture_name, MMIMTV_FULL_PATH_MAX_LEN, 
            path, MMIMTV_DATA_PTR->snapshot_picture_name);
        
        if(!MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->path_snapshot_picture_name, full_path_len))
        {
            break;
        }
    }
    MMIMTV_DATA_PTR->mtv_settings.snapshot_file_id = name_id;
    return TRUE;    
}

/*****************************************************************************/
// 	Description : set esg path
//	Global resource dependence : none
//  Author: 
//	Note: 设置ESG的工作路径
/*****************************************************************************/
PUBLIC void MMIMTV_SetESGPath(void)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_NUM;

    file_dev = MMIMTV_DATA_PTR->mtv_settings.esg_path_store;
    if(MMI_DEVICE_NUM == file_dev)
    {
        return;
    }
    
    MMIMTV_DATA_PTR->esg_path[0] = file_dev;

	//set esg path
	CMMB_SetEsgDataPath((KDuint16)(strlen((char*)MMIMTV_DATA_PTR->esg_path)), (KDuint8*)(MMIMTV_DATA_PTR->esg_path));
	//SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_SetESGPath path: %s"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_1680_112_2_18_2_40_16_563,(uint8*)"s", MMIMTV_DATA_PTR->esg_path);
}

/*****************************************************************************/
// 	Description : whether have enough space to save esg info
//	Global resource dependence : none
//  Author: 
//	Note: 确定有足够的空间保存ESG
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsEnoughESGSpace(void)
{
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_UDISK;
    
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((MMIMTV_DATA_PTR->mtv_settings.esg_path_store), MMIMTV_ESG_SPACE_AT_LEAST, &file_type))
    {
        return FALSE;
    }
	return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_InitSetting(void)
{
    MN_RETURN_RESULT_E    	return_value = MN_RETURN_FAILURE;
    MMIMTV_SETTING_INFO_T   *mtv_settings_ptr = PNULL;
 
    MMINV_READ(MMINV_MTV_SETTING_INFO, &MMIMTV_DATA_PTR->mtv_settings, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        mtv_settings_ptr = &MMIMTV_DATA_PTR->mtv_settings;
        
        SCI_MEMSET(mtv_settings_ptr, 0, sizeof(MMIMTV_SETTING_INFO_T));
        MMIMTV_DATA_PTR->mtv_settings.display_program_name = TRUE;
        MMIMTV_DATA_PTR->mtv_settings.volume_value = MMIMTV_VOLUME_LEVEL_4;
        MMIMTV_DATA_PTR->mtv_settings.panel_volume_value = MMIMTV_VOLUME_LEVEL_4;
        MMIMTV_DATA_PTR->mtv_settings.audio_language = MMIMTV_AUDIO_CHN;
        MMIMTV_DATA_PTR->mtv_settings.subtitle_streams = TRUE;
        MMIMTV_DATA_PTR->mtv_settings.mute = FALSE;
        MMIMTV_DATA_PTR->mtv_settings.audio_mode = MMIMTV_AUDIO_MODE_SINGLE_CHANNEL;
        MMIMTV_DATA_PTR->mtv_settings.zoom_mode = MMIMTV_ZOOM_FRAME;
        MMIMTV_DATA_PTR->mtv_settings.mtv_brightness = MMIMTV_BRIGHTNESS_LEVEL_4;
        MMIMTV_DATA_PTR->mtv_settings.record_store = MMI_DEVICE_SDCARD;
        MMIMTV_DATA_PTR->mtv_settings.snapshot_store = MMI_DEVICE_SDCARD;
		MMIMTV_DATA_PTR->mtv_settings.esg_path_store = MMI_DEVICE_SDCARD;
        MMIMTV_DATA_PTR->mtv_settings.transparency = MMIMTV_OSD_TRANSPARENCY_75;
        MMIMTV_DATA_PTR->mtv_settings.osd_display_time = MMIMTV_DISPLAY_TIME_5S;
        MMIMTV_DATA_PTR->mtv_settings.watch_time_tip = MMIMTV_WATCH_TIME_TIP_10M;
        MMIMTV_DATA_PTR->mtv_settings.broadcast_bg = MMIMTV_BROADCAST_BG_PIC_1;
        MMIMTV_DATA_PTR->mtv_settings.snapshot_file_id = 0;
        MMIMTV_DATA_PTR->mtv_settings.record_file_id = 0;
        
        MMINV_WRITE(MMINV_MTV_SETTING_INFO, &MMIMTV_DATA_PTR->mtv_settings); 
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetIsMute(BOOLEAN mute)
{
    MMIMTV_DATA_PTR->mtv_settings.mute = mute;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetVolume(MMIMTV_VOLUME_E volume)
{
    MMIMTV_DATA_PTR->mtv_settings.volume_value = volume;
}

/*****************************************************************************/
// 	Description : save panel volume value
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPanelVolume(MMIMTV_VOLUME_E volume)
{
    MMIMTV_DATA_PTR->mtv_settings.panel_volume_value = volume;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetAudioLanguage(MMIMTV_AUDIO_LANGUGAE_E audio)
{
    MMIMTV_DATA_PTR->mtv_settings.audio_language = audio;
}   
     
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetSubtitleStreams(BOOLEAN sub)
{
    MMIMTV_DATA_PTR->mtv_settings.subtitle_streams = sub;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetAudioMode(MMIMTV_AUDIO_MODE_E mode)
{
    MMIMTV_DATA_PTR->mtv_settings.audio_mode = mode;
}                         

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetZoomMode(MMIMTV_ZOOM_MODE_E zoom)
{
    if(MMIMTV_DATA_PTR->mtv_settings.zoom_mode != zoom)
    {
        MMIMTV_DATA_PTR->mtv_settings.zoom_mode = zoom;
        MMIMTV_SetVideoSpec(zoom);
        MMIMTV_ResetBG();
    } 
}           

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBrightness(MMIMTV_BRIGHTNESS_E bright)
{
    MMIMTV_DATA_PTR->mtv_settings.mtv_brightness = bright;
    LCD_SetBackLightBrightness(MMIMTV_LCD_CONTRSAT_MAX*(bright+1)/MMIMTV_BRIGHTNESS_LEVEL_MAX);
}    

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetContrast(MMIMTV_CONTRAST_E contrast)
{
    MMIMTV_DATA_PTR->mtv_settings.mtv_contrast = contrast;
}   

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetColorSaturation(MMIMTV_COLOR_SATURATION_E color)
{
    MMIMTV_DATA_PTR->mtv_settings.color_saturation = color;
}  
      
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetPictureSharpness(MMIMTV_PICTURE_SHARPNESS_E sharp)
{
    MMIMTV_DATA_PTR->mtv_settings.picture_sharpness = sharp;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetIsDisplayProgramName(BOOLEAN display)
{
    MMIMTV_DATA_PTR->mtv_settings.display_program_name = display;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetRecordStore(MMIFILE_DEVICE_E dev)
{
    MMIMTV_DATA_PTR->mtv_settings.record_store = dev;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetEBStore(MMIFILE_DEVICE_E dev)
{
    MMIMTV_DATA_PTR->mtv_settings.eb_store = dev;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetSnapshotStore(MMIFILE_DEVICE_E dev)
{
    MMIMTV_DATA_PTR->mtv_settings.snapshot_store = dev;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetOSDDisplayTime(MMIMTV_OSD_DISPLAY_TIME_E time)
{
    MMIMTV_DATA_PTR->mtv_settings.osd_display_time = time;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetWatchTimeTip(MMIMTV_WATCH_TIME_TIP_E tip)
{
    MMIMTV_DATA_PTR->mtv_settings.watch_time_tip = tip;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBroadcastBG(MMIMTV_BROADCAST_BG_PIC_E pic)
{
    MMIMTV_DATA_PTR->mtv_settings.broadcast_bg = pic;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetTransparency(MMIMTV_OSD_TRANSPARENCY_E trans)
{
    MMIMTV_DATA_PTR->mtv_settings.transparency = trans;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetRecordFileID(uint32 id)
{
    MMIMTV_DATA_PTR->mtv_settings.record_file_id = id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordStartTime(uint32 start)
{
    MMIMTV_DATA_PTR->mtv_settings.book_record_start_time = start;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordLong(uint32 record_long)
{
    MMIMTV_DATA_PTR->mtv_settings.book_record_long = record_long;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordNetID(uint32 id)
{
    MMIMTV_DATA_PTR->mtv_settings.book_record_net_id = id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordServiceID(uint32 id)
{
    MMIMTV_DATA_PTR->mtv_settings.book_record_service_id = id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetBookRecordFileName(const wchar* file_name)
{
    if(PNULL == file_name)
    {
        return;
    }
    
    MMIAPICOM_Wstrcpy(MMIMTV_DATA_PTR->mtv_settings.book_record_file_name, file_name);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsMute(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.mute;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_VOLUME_E MMIMTV_GetVolume(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.volume_value;
}

/*****************************************************************************/
// 	Description : get panel volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_VOLUME_E MMIMTV_GetPanelVolume(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.panel_volume_value;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_AUDIO_LANGUGAE_E MMIMTV_GetAudioLanguage(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.audio_language;
}   
     
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_GetSubtitleStreams(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.subtitle_streams;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_AUDIO_MODE_E MMIMTV_GetAudioMode(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.audio_mode;
}                         

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_ZOOM_MODE_E MMIMTV_GetZoomMode(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.zoom_mode;
}           

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_BRIGHTNESS_E MMIMTV_GetBrightness(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.mtv_brightness;
}    

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_CONTRAST_E MMIMTV_GetContrast(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.mtv_contrast;
}   

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_COLOR_SATURATION_E MMIMTV_GetColorSaturation(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.color_saturation;
}  
      
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_PICTURE_SHARPNESS_E MMIMTV_GetPictureSharpness(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.picture_sharpness;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsDisplayProgramName(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.display_program_name;
}  

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetRecordStore(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.record_store;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetEBStore(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.eb_store;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMTV_GetSnapshotStore(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.snapshot_store;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_DISPLAY_TIME_E MMIMTV_GetOSDDisplayTime(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.osd_display_time;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_WATCH_TIME_TIP_E MMIMTV_GetWatchTimeTip(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.watch_time_tip;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_BROADCAST_BG_PIC_E MMIMTV_GetBroadcastBG(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.broadcast_bg;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_TRANSPARENCY_E MMIMTV_GetTransparency(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.transparency;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetRecordFileID(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.record_file_id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordStartTime(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.book_record_start_time;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordLong(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.book_record_long;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordNetID(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.book_record_net_id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetBookRecordServiceID(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.book_record_service_id;
} 

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetBookRecordFileName(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.book_record_file_name;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SettingInfoSetFactory(void)
{
    MMIMTV_SETTING_INFO_T mtv_setting = {0};
    
    SCI_MEMSET(&mtv_setting, 0, sizeof(MMIMTV_SETTING_INFO_T));
    mtv_setting.display_program_name = TRUE;
    mtv_setting.volume_value = MMIMTV_VOLUME_LEVEL_4;
    mtv_setting.panel_volume_value = MMIMTV_VOLUME_LEVEL_4;
    mtv_setting.audio_language = MMIMTV_AUDIO_CHN;
    mtv_setting.subtitle_streams = TRUE;
    mtv_setting.mute = FALSE;
    mtv_setting.audio_mode = MMIMTV_AUDIO_MODE_SINGLE_CHANNEL;
    mtv_setting.zoom_mode = MMIMTV_ZOOM_FRAME;
    mtv_setting.mtv_brightness = MMIMTV_BRIGHTNESS_LEVEL_3;
    mtv_setting.record_store = MMI_DEVICE_SDCARD;
    mtv_setting.snapshot_store = MMI_DEVICE_SDCARD;
    mtv_setting.transparency = MMIMTV_OSD_TRANSPARENCY_75;
    mtv_setting.osd_display_time = MMIMTV_DISPLAY_TIME_5S;
    mtv_setting.watch_time_tip = MMIMTV_WATCH_TIME_TIP_10M;
    mtv_setting.broadcast_bg = MMIMTV_BROADCAST_BG_PIC_1;
    mtv_setting.snapshot_file_id = 0;
    mtv_setting.record_file_id = 0;
    
    MMINV_WRITE(MMINV_MTV_SETTING_INFO, &mtv_setting); 
  #ifdef MBBMS_SUPPORT
   MMIMBBMS_RestoreFactorySetting();
  #endif
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SaveSetting(void)
{
    if(PNULL != s_data_data_ptr)
    {
        //TV信号不要保存
        MMIMTV_DATA_PTR->mtv_settings.signal_level = 0;

        MMINV_WRITE(MMINV_MTV_SETTING_INFO, &MMIMTV_DATA_PTR->mtv_settings);
    }
    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetCurrentProgramHint(void)
{
    const MMIMTV_PROG_HINT_INFO_T* program_hint = MMIMTV_GetProgramHintInfo();
    if(PNULL == program_hint)
    {
        return PNULL;
    }
    
    return program_hint->hint_current;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC const wchar* MMIMTV_GetNextProgramHint(void)
{
    const MMIMTV_PROG_HINT_INFO_T* program_hint = MMIMTV_GetProgramHintInfo();
    if(PNULL == program_hint)
    {
        return PNULL;
    }
    
    return program_hint->hint_next;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: return wstr actural length
/*****************************************************************************/
PUBLIC uint16 MMIMTV_Mtvstring2Unicode(MTV_STRING_T *mtv_string_ptr,  //in   mtv string
                                       wchar *wstr_ptr,             //out    unicode
                                       uint16 wstr_len              //in     unicode max len
                                       )
{
    uint16 len = 0;
    GUI_CHAR_CODE_TYPE_E code_type = GUI_CHAR_CODE_ANSI;
    
    if(PNULL == mtv_string_ptr || PNULL == wstr_ptr)
    {
        return 0;
    }
    
    if(PNULL == mtv_string_ptr->str_ptr || 0 == mtv_string_ptr->length)
    {
        //SCI_TRACE_LOW:"MMIMTV_Mtvstring2Unicode length==0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2295_112_2_18_2_40_17_564,(uint8*)"");
        return 0;
    }
    
    //SCI_TRACE_LOW("MMIMTV_Mtvstring2Unicode encode = %d, len = %d", mtv_string_ptr->encode, mtv_string_ptr->length);
    switch(mtv_string_ptr->encode)
    {
    case MTV_UTF8:
        code_type = GUI_CHAR_CODE_UTF8;
        break;
        
    case MTV_UCS2:
        code_type = GUI_CHAR_CODE_UCS2_BE;
        break;
        
    default:
        code_type = GUI_CHAR_CODE_ANSI;
        break;
    }
    len = GUI_OtherCodeToWstr(
            wstr_ptr, 
            wstr_len, 
            code_type, 
            mtv_string_ptr->str_ptr, 
            mtv_string_ptr->length
            );
    //SCI_TRACE_LOW("MMIMTV_Mtvstring2Unicode GUI_OtherCodeToWstr convert len = %d", len);
    
    return len;
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_RGB565ToYUV422(uint16 *pRgb565Buf, uint8 *pYBuf, uint8* pUVBuf,  uint32 width, uint32 height)
{    
    uint32 r = 0, g = 0, b = 0;
    uint32 i = 0, j = 0;
    
    if(PNULL == pRgb565Buf
        || PNULL == pYBuf)
    {
        return;
    }
    
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            r = RGB565_R(pRgb565Buf);
            g = RGB565_G(pRgb565Buf);
            b = RGB565_B(pRgb565Buf);
            pRgb565Buf++;
            
            //Y = 0.299 * r + 0.587 * g + 0.114 * b;
            *pYBuf++ = ((77 * r + 150 * g + 29 * b) >> 8) ;
            
            if (j % 2 == 0)            //even column
            {   
                //U = 128 - 0.1687 * r - 0.3313 * g + 0.5 * b 
                *pUVBuf++ =  ((128 * b - 43 * r - 85 * g) >> 8)+128;     
                
            }
            else
            {
                //V = 128 + 0.5 * r - 0.4187 * g - 0.0813 * b 
                *pUVBuf++ = ((128 * r - 107 * g - 21 * b ) >> 8)+128;    
            }
        }
    }  
}

/*****************************************************************************/
// 	Description : set image to v chip
//	Global resource dependence : none
//  Author: 
//	Note: change IMG to YUV422, then set yuv422 to vchip
//  注意: 目前由于Vchip的限制，设置给Vchip的尺寸不能大于352*288
/*****************************************************************************/
PUBLIC void MMIMTV_SetBMPtoVChip(MMI_IMAGE_ID_T image_id)
{
    GUIRES_IMG_SRC_T res_img= {0};
    uint8           *pYBuf = PNULL;
    uint8           *pUVBuf = PNULL;
    uint8           *rgbbuf = PNULL;
    //GUI_LCD_ID_E        lcd_id = GUI_LCD_0; 
    GUIRES_IMG_INFO_T image_info={0};
    MMI_WIN_ID_T    win_id = VIRTUAL_WIN_ID;
    GUI_RECT_T      display_rect = {0};
    
    //SCI_TRACE_LOW:"MMIMTV_BMPToYUV422 ENTER"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2385_112_2_18_2_40_17_565,(uint8*)"");
    res_img.data_ptr = (uint8 *)MMI_GetLabelImage(image_id, win_id, &res_img.data_size); 
    if (GUIRES_GetImgInfo(&res_img,&image_info))
    {
        //SCI_TRACE_LOW:"MMIMTV_BMPToYUV422 height=%d, width=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2389_112_2_18_2_40_17_566,(uint8*)"dd",image_info.height,image_info.width);
        image_info.height = MIN(image_info.height,MMIMTV_LCD_HEIGHT);
        image_info.width = MIN(image_info.width,MMIMTV_LCD_WIDTH);
    }
    else// get image info error
    {
        //SCI_TRACE_LOW:"MMIMTV_BMPToYUV422: error, please check image type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2395_112_2_18_2_40_17_567,(uint8*)"");
        return;
    }
    
    display_rect.left   = 0;
    display_rect.top    = 0;
    display_rect.right  = image_info.width - 1;
    display_rect.bottom = image_info.height - 1;
    rgbbuf = (uint8 *)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_RGB); 
    GUIRES_CopyImgBuf(res_img.data_ptr,image_info.width,image_info.height,
        image_info.width*image_info.height,res_img.data_size,PNULL,&display_rect,(GUI_COLOR_T*)rgbbuf);
    pYBuf = (uint8 *)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_Y);
    pUVBuf = (uint8 *)BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_UV);
    MMIMTV_RGB565ToYUV422((uint16*)rgbbuf,pYBuf,pUVBuf,image_info.width,image_info.height);
    CMMB_SetBackGround(pYBuf,pUVBuf,image_info.width, image_info.height);
    BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_RGB);
    BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_Y);
    BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_UV);
}

/*****************************************************************************/
// 	Description : calculate TV signal lenvel via signal value
//	Global resource dependence : none
//  Author: xin.li
//	Note: signal level 0-5
/*****************************************************************************/
PUBLIC uint8 MMIMTV_CalcTVSignalLevel(uint32 signal_value)
{
    if(signal_value > MMIMTV_SIGNAL_VALUE_MAX)
    {
        signal_value = MMIMTV_SIGNAL_VALUE_MAX;
    }

    return s_signal_level_table[signal_value];
}

/*****************************************************************************/
// 	Description : save tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SaveTVSignalLevel(uint8 signal_level)
{
    if(signal_level > MMIMTV_SIGNAL_LEVEL_MAX)
    {
        signal_level = MMIMTV_SIGNAL_LEVEL_MAX;
    }
    MMIMTV_DATA_PTR->mtv_settings.signal_level = signal_level;
}

/*****************************************************************************/
// 	Description : get tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetTVSignalLevel(void)
{
    return MMIMTV_DATA_PTR->mtv_settings.signal_level;
}



//MBBMS start
#ifdef MBBMS_SUPPORT

/*****************************************************************************/
// 	Description : read favorite service info file 
//	Global resource dependence : none
//  Author: 
//	Note: 读取用户收藏频道信息
//  other:文件结构：|-version-|-node_num-|---------------node----------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ReadFavoriteServiceFile(void)
{
    MMIFILE_HANDLE      file_handle = 0;  
    uint32              read_size   = 0;
    uint32              list_num    = 0;
    uint32              write_offset = 0;
    uint32              struct_len  = sizeof(MMIMBBMS_SAVE_SERVICE_INFO_T);
    uint32              version     = 0;
    MMIFILE_DEVICE_E    dev     = MMI_DEVICE_UDISK;
    BOOLEAN             file_ok = FALSE;
    
    /* jhpeng modified begin for CR155220 */
    if (PNULL == s_data_data_ptr)
    {
        return FALSE;
    }
    if (MMIMTV_DATA_PTR->favorite_service_head_ptr != PNULL)
    {
        return TRUE;
    }
    
    //遍历所有DEV，查找文件是否存在
    for(dev=MMI_DEVICE_UDISK;dev<MMI_DEVICE_NUM;dev++)
    {
        MMIMTV_DATA_PTR->favorite_service_full_path[0] = *MMIAPIFMM_GetDevicePath((dev));
        if(MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->favorite_service_full_path, (uint16)MMIAPICOM_Wstrlen(MMIMTV_DATA_PTR->favorite_service_full_path)))
        {
            break;
        }
    }
    if(dev >= MMI_DEVICE_NUM)
    {
        return FALSE;//不存在
    }
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->favorite_service_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile create file error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2499_112_2_18_2_40_18_568,(uint8*)"");
        return FALSE;
    }

    //读取版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &version, sizeof(uint32), &read_size, NULL))
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2506_112_2_18_2_40_18_569,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //确认版本信息读取成功
    if(0 == read_size)
    {
        //失败，文件为空
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    //版本信息确认
    if(version != MMIMTV_SAVE_FILE_VERSION)
    {
        //版本不匹配
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: Favorite Service file version error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2522_112_2_18_2_40_18_570,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //获取频道节点个数
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, NULL))
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2533_112_2_18_2_40_18_571,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteServiceFile total list num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2537_112_2_18_2_40_18_572,(uint8*)"d", list_num);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2541_112_2_18_2_40_18_573,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //添加频道节点, 由后向前逐个添加
    //write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
    while (list_num)
    {
        /* jhpeng modified for CR155220 */
        write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
        {
            MMIMBBMS_SAVE_SERVICE_INFO_T node = {0};
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &node, struct_len, &read_size, NULL))
            {
                MMIMBBMS_AddFavoriteServiceNode(&node);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2561_112_2_18_2_40_18_574,(uint8*)"");
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2568_112_2_18_2_40_18_575,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //write_offset = 2 * sizeof(uint32) + list_num * struct_len;
        list_num--;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : save favorite service info file 
//	Global resource dependence : none
//  Author: 
//	Note: 保存用户收藏频道信息
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_WriteFavoriteServiceFile(void)
{
    MMIFILE_HANDLE          file_handle = 0;
    uint32                  list_num = 0;
    uint32                  write_offset = 0;
    uint32                  write_len = 0;
    MMIMBBMS_SAVE_SERVICE_INFO_T* cursor = 0;
    uint32                  struct_size = sizeof(MMIMBBMS_SAVE_SERVICE_INFO_T);
    uint32                  total_size = list_num * struct_size + 2 * sizeof(uint32);
    uint32                  version = MMIMTV_SAVE_FILE_VERSION;
    MMIFILE_DEVICE_E        file_type = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E        dev       = MMI_DEVICE_UDISK;
    
    //保护处理
    if(PNULL == s_data_data_ptr)
    {
        return FALSE;
    }
    list_num = MMIMTV_DATA_PTR->favorite_service_list_num;
    cursor = MMIMTV_DATA_PTR->favorite_service_head_ptr;
    
    dev = MMIAPIFMM_GetDeviceTypeByPath(MMIMTV_DATA_PTR->favorite_service_full_path, MMIFILE_DEVICE_LEN);
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((dev), total_size, &file_type))
    {
        return FALSE;
    }
    MMIMTV_DATA_PTR->favorite_service_full_path[0] = *MMIAPIFMM_GetDevicePath(file_type);
   
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->favorite_service_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteServiceFile create file error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2614_112_2_18_2_40_18_576,(uint8*)"");
        return FALSE;
    }

    //写入版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &version, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //写入节点数目
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //逐个写入节点
    //write_offset += sizeof(uint32);
    //list_num = 0;
    while(PNULL != cursor)
    {
        //list_num ++;
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_size, &write_len, NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //write_offset += struct_size;
        cursor = cursor->service_next;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : add favorite service node 
//	Global resource dependence : none
//  Author: 
//	Note: 添加收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddFavoriteServiceNode(MMIMBBMS_SAVE_SERVICE_INFO_T *node_ptr)
{
    MMIMBBMS_SAVE_SERVICE_INFO_T* new_node_ptr = PNULL;
    
    if(PNULL == node_ptr)
    {
        return;
    }
    
    new_node_ptr = (MMIMBBMS_SAVE_SERVICE_INFO_T*)SCI_ALLOC_APP(sizeof(MMIMBBMS_SAVE_SERVICE_INFO_T));
    if(PNULL != new_node_ptr)
    {
        SCI_MEMCPY(new_node_ptr, node_ptr, sizeof(MMIMBBMS_SAVE_SERVICE_INFO_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_AddFavoriteServiceNode alloc memory error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2671_112_2_18_2_40_18_577,(uint8*)"");
        return;
    }
    
    //添加至头
    new_node_ptr->service_next = MMIMTV_DATA_PTR->favorite_service_head_ptr;
    MMIMTV_DATA_PTR->favorite_service_head_ptr = new_node_ptr;
    MMIMTV_DATA_PTR->favorite_service_list_num ++;
}

/*****************************************************************************/
// 	Description : delete a service node from favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNode(MMIMBBMS_SAVE_SERVICE_INFO_T *node_ptr)
{
    if(PNULL == node_ptr
        || PNULL == MMIMTV_DATA_PTR->favorite_service_head_ptr)
    {
        return;
    }
    
    if(node_ptr == MMIMTV_DATA_PTR->favorite_service_head_ptr)
    {
        MMIMTV_DATA_PTR->favorite_service_head_ptr = node_ptr->service_next;
        SCI_FREE(node_ptr);
    }
    else
    {
        MMIMBBMS_SAVE_SERVICE_INFO_T* temp = MMIMTV_DATA_PTR->favorite_service_head_ptr;
        while(node_ptr != temp->service_next)
        {
            if(PNULL != temp->service_next)
            {
                temp = temp->service_next;
            }
            else
            {
                return;
            }
        }
        temp->service_next = node_ptr->service_next;
        SCI_FREE(node_ptr);
    }
    MMIMTV_DATA_PTR->favorite_service_list_num --;
}

/*****************************************************************************/
// 	Description : delete a service node from favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNodeByServiceID(anyURI *pGlobalServiceID)
{
    BOOLEAN   bResult = FALSE;
    MMIMBBMS_SAVE_SERVICE_INFO_T* temp = PNULL;
    MMIMBBMS_SAVE_SERVICE_INFO_T* pre_node = PNULL;

    temp = MMIMTV_DATA_PTR->favorite_service_head_ptr;
    pre_node = temp;

    while(temp != PNULL)
    {
        bResult = MMIMBBMS_IndexCompare(pGlobalServiceID, &(temp->globalServiceID));

        if (bResult)
        {
            if (MMIMTV_DATA_PTR->favorite_service_head_ptr == temp)
            {
                MMIMTV_DATA_PTR->favorite_service_head_ptr = temp->service_next;
            }
            pre_node->service_next = temp->service_next;
            SCI_FREE(temp);
            temp = PNULL;
            MMIMTV_DATA_PTR->favorite_service_list_num --;
      
            if (0 == MMIMTV_DATA_PTR->favorite_service_list_num)
            {
                MMIMTV_DATA_PTR->favorite_service_head_ptr = PNULL;
            }

            break;
        }

        pre_node = temp;
        temp = temp->service_next;
    }

    return;
}


/*****************************************************************************/
// 	Description : delete a service node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelFavoriteServiceNodeViaIndex(uint32 index)
{
    MMIMBBMS_DelFavoriteServiceNode(MMIMBBMS_GetFavoriteServiceNodeViaIndex(index));
}

/*****************************************************************************/
// 	Description : get a service node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index获取频道节点
/*****************************************************************************/
PUBLIC MMIMBBMS_SAVE_SERVICE_INFO_T* MMIMBBMS_GetFavoriteServiceNodeViaIndex(uint32 index)
{
    MMIMBBMS_SAVE_SERVICE_INFO_T* cursor = MMIMTV_DATA_PTR->favorite_service_head_ptr;
    
    if(index > MMIMTV_DATA_PTR->favorite_service_list_num)
    {
        return PNULL;
    }
    
    if (cursor == PNULL)
    {
        return cursor;
    }
    while(index --)
    {
        if(PNULL == cursor)
        {
            return PNULL;
        }
        
        cursor = cursor->service_next;
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : clear up favorite service list
//	Global resource dependence : none
//  Author: 
//	Note: 清空收藏频道链表
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearFavoriteServiceList(void)
{
    MMIMBBMS_SAVE_SERVICE_INFO_T* cursor = PNULL;
    
    if(PNULL != s_data_data_ptr)
    {
        while(PNULL != MMIMTV_DATA_PTR->favorite_service_head_ptr)
        {
            cursor = MMIMTV_DATA_PTR->favorite_service_head_ptr;
            MMIMTV_DATA_PTR->favorite_service_head_ptr = cursor->service_next;
            SCI_FREE(cursor);
        }
        MMIMTV_DATA_PTR->favorite_service_list_num = 0;
    }
}

/*****************************************************************************/
// 	Description : get favorite service num
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetFavoriteServiceListNum(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_GetFavoriteServiceListNum  num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2826_112_2_18_2_40_18_578,(uint8*)"d", MMIMTV_DATA_PTR->favorite_service_list_num);
    return MMIMTV_DATA_PTR->favorite_service_list_num;
}

/*****************************************************************************/
// 	Description : compare two anyURI index
//	Global resource dependence : none
//  Author: 
//	Note: equal return TRUE, else return FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IndexCompare(const anyURI *index1, const anyURI *index2)
{
    if(PNULL == index1
        || PNULL == index2)
    {
        return FALSE;
    }
    
    if (index1->string_len != index2->string_len)
    {
        return FALSE;
    }
    
    if (index1->string_len > SG_MAX_URI_LEN)
    {
        if(0 == strncmp((char*)index1->anyURI_ptr, (char*)index2->anyURI_ptr, index2->string_len))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if(0 == strncmp((char*)index1->anyURI_arr, (char*)index2->anyURI_arr, index2->string_len))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/*****************************************************************************/
// 	Description : 频道是否被收藏
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsFavoriteService(const anyURI *global_index)
{
    MMIMBBMS_SAVE_SERVICE_INFO_T* cursor = MMIMTV_DATA_PTR->favorite_service_head_ptr;
    
    if(PNULL == global_index)
    {
        return FALSE;
    }
    
    while(cursor)
    {
        if(MMIMBBMS_IndexCompare(global_index, &cursor->globalServiceID))
        {
            return TRUE;
        }
        
        cursor = cursor->service_next;
    }
    
    return FALSE;
}

/*******************************************************************************/



/*****************************************************************************/
// 	Description : read alarm content info file 
//	Global resource dependence : none
//  Author: 
//	Note: 读取用户收藏节目信息
//  other:文件结构：|-version-|-node_num-|---------------node----------------|
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ReadAlarmContentFile(void)
{
    MMIFILE_HANDLE      file_handle = 0;  
    uint32              read_size   = 0;
    uint32              list_num    = 0;
    uint32              write_offset = 0;
    uint32              struct_len  = sizeof(MMIMBBMS_ALARM_CONTENT_INFO_T);
    uint32              version     = 0;
    MMIFILE_DEVICE_E    dev     = MMI_DEVICE_UDISK;
    BOOLEAN             file_ok = FALSE;

    /* jhpeng modified begin for CR155220 */
    if (MMIMTV_DATA_PTR->alarm_content_head_ptr != PNULL)
    {
        return TRUE;
    }
    /* jhpeng modified end for CR155220 */
    
    //遍历所有DEV，查找文件是否存在
    for(dev=MMI_DEVICE_UDISK;dev<MMI_DEVICE_NUM;dev++)
    {
        MMIMTV_DATA_PTR->alarm_content_full_path[0] = *MMIAPIFMM_GetDevicePath((dev));
        if(MMIAPIFMM_IsFileExist(MMIMTV_DATA_PTR->alarm_content_full_path, (uint16)MMIAPICOM_Wstrlen(MMIMTV_DATA_PTR->alarm_content_full_path)))
        {
            break;
        }
    }
    if(dev >= MMI_DEVICE_NUM)
    {
        return FALSE;//不存在
    }
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->alarm_content_full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile create file error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2940_112_2_18_2_40_18_579,(uint8*)"");
        return FALSE;
    }


    //读取版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &version, sizeof(uint32), &read_size, NULL))
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2948_112_2_18_2_40_18_580,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //确认版本信息读取成功
    if(0 == read_size)
    {
        //失败，文件为空
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //版本信息确认
    if(version != MMIMTV_SAVE_FILE_VERSION)
    {
        //版本不匹配
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: Favorite Content file version error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2965_112_2_18_2_40_19_581,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //获取节目节点个数
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
    {
        if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &list_num, sizeof(uint32), &read_size, NULL))
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2976_112_2_18_2_40_19_582,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_ReadFavoriteContentFile total list num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2980_112_2_18_2_40_19_583,(uint8*)"d", list_num);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_2984_112_2_18_2_40_19_584,(uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //添加节目节点, 由后向前逐个添加
    //write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
    while (list_num)
    {
        /* jhpeng modified for CR155220 */
        write_offset = 2 * sizeof(uint32) + (list_num - 1) * struct_len;
        //write_offset = 2 * sizeof(uint32) +  (list_num - 1) * struct_len;
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, write_offset, SFS_SEEK_BEGIN))
        {
            MMIMBBMS_ALARM_CONTENT_INFO_T node = {0};
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &node, struct_len, &read_size, NULL))
            {
                MMIMBBMS_AddAlarmContentNode(&node);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_ReadFile error! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3005_112_2_18_2_40_19_585,(uint8*)"");
                MMIAPIFMM_CloseFile(file_handle);
                return FALSE;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIAPIFMM_SetFilePointer error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3012_112_2_18_2_40_19_586,(uint8*)"");
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //write_offset = 2 * sizeof(uint32) + list_num * struct_len;
        list_num--;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : write favorite content info file 
//	Global resource dependence : none
//  Author: 
//	Note: 保存用户收藏节目信息
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_WriteAlarmContentFile(void)
{
    MMIFILE_HANDLE          file_handle = 0;
    uint32                  list_num = 0;
    uint32                  write_offset = 0;
    uint32                  write_len = 0;
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = PNULL;
    uint32                  struct_size = sizeof(MMIMBBMS_ALARM_CONTENT_INFO_T);
    uint32                  total_size = list_num * struct_size + 2 * sizeof(uint32);
    uint32                  version = MMIMTV_SAVE_FILE_VERSION;
    MMIFILE_DEVICE_E        file_type = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E        dev       = MMI_DEVICE_UDISK;

    //保护处理
    if(PNULL == s_data_data_ptr)
    {
        return FALSE;
    }
    list_num = MMIMTV_DATA_PTR->alarm_content_list_num;
    cursor = MMIMTV_DATA_PTR->alarm_content_head_ptr;

    dev = MMIAPIFMM_GetDeviceTypeByPath(MMIMTV_DATA_PTR->alarm_content_full_path, MMIFILE_DEVICE_LEN);
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev((dev), total_size, &file_type))
    {
        return FALSE;
    }
    MMIMTV_DATA_PTR->alarm_content_full_path[0] = *MMIAPIFMM_GetDevicePath(file_type);
    
    file_handle = MMIAPIFMM_CreateFile(MMIMTV_DATA_PTR->alarm_content_full_path, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_WriteFavoriteContentFile create file error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3058_112_2_18_2_40_19_587,(uint8*)"");
        return FALSE;
    }
    
    //写入版本信息
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &version, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //写入节点数目
    write_offset = sizeof(uint32);
    if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &list_num, sizeof(uint32), &write_len, NULL))
    {
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }
    
    //逐个写入节点
    //write_offset += sizeof(uint32);
    //list_num = 0;
    while(PNULL != cursor)
    {
        //list_num ++;
        if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, cursor, struct_size, &write_len, NULL))
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        //write_offset += struct_size;
        cursor = cursor->content_next;
    }
    MMIAPIFMM_CloseFile(file_handle);
    return TRUE;
}

/*****************************************************************************/
// 	Description : add alarm content node 
//	Global resource dependence : none
//  Author: 
//	Note: 添加收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_AddAlarmContentNode(MMIMBBMS_ALARM_CONTENT_INFO_T *node_ptr)
{
    MMIMBBMS_ALARM_CONTENT_INFO_T* new_node_ptr = PNULL;
    
    if(PNULL == node_ptr)
    {
        return;
    }
    
    new_node_ptr = (MMIMBBMS_ALARM_CONTENT_INFO_T*)SCI_ALLOC_APP(sizeof(MMIMBBMS_ALARM_CONTENT_INFO_T));
    if(PNULL != new_node_ptr)
    {
    SCI_MEMCPY(new_node_ptr, node_ptr, sizeof(MMIMBBMS_ALARM_CONTENT_INFO_T));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_AddAlarmContentNode alloc memory error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3115_112_2_18_2_40_19_588,(uint8*)"");
        return;
    }
    
    //添加至头
    new_node_ptr->content_next = MMIMTV_DATA_PTR->alarm_content_head_ptr;
    MMIMTV_DATA_PTR->alarm_content_head_ptr = new_node_ptr;
    MMIMTV_DATA_PTR->alarm_content_list_num ++;
}

/*****************************************************************************/
// 	Description : delete a content node from alarm content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNode(MMIMBBMS_ALARM_CONTENT_INFO_T *node_ptr)
{
    if (PNULL == node_ptr)
    {
        return;
    }

    if(node_ptr == MMIMTV_DATA_PTR->alarm_content_head_ptr)
    {
        MMIMTV_DATA_PTR->alarm_content_head_ptr = node_ptr->content_next;
        SCI_FREE(node_ptr);
    }
    else
    {
        MMIMBBMS_ALARM_CONTENT_INFO_T* temp = MMIMTV_DATA_PTR->alarm_content_head_ptr;
        while(node_ptr != temp->content_next)
        {
            if(PNULL != temp->content_next)
            {
                temp = temp->content_next;
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_DelAlarmContentNode find node error! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3152_112_2_18_2_40_19_589,(uint8*)"");
                return;
            }
        }
        temp->content_next = node_ptr->content_next;
        SCI_FREE(node_ptr);
    }
    MMIMTV_DATA_PTR->alarm_content_list_num --;
}

/*****************************************************************************/
// 	Description : delete a content node from favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNodeByContentID(anyURI *pGlobalContentID)
{
    BOOLEAN   bResult = FALSE;
    MMIMBBMS_ALARM_CONTENT_INFO_T* temp = PNULL;
    MMIMBBMS_ALARM_CONTENT_INFO_T* pre_node = PNULL;

    temp = MMIMTV_DATA_PTR->alarm_content_head_ptr;
    pre_node = temp;

    while(temp != PNULL)
    {
        bResult = MMIMBBMS_IndexCompare(pGlobalContentID, &(temp->globalContentID));

        if (bResult)
        {
            if (MMIMTV_DATA_PTR->alarm_content_head_ptr == temp)
            {
                MMIMTV_DATA_PTR->alarm_content_head_ptr = temp->content_next;
            }
            pre_node->content_next = temp->content_next;
            SCI_FREE(temp);
            temp = PNULL;
            MMIMTV_DATA_PTR->alarm_content_list_num --;

            if (0 == MMIMTV_DATA_PTR->alarm_content_list_num)
            {
                MMIMTV_DATA_PTR->alarm_content_head_ptr = PNULL;
            }
      
            break;
        }

        pre_node = temp;
        temp = temp->content_next;
    }

    return;
}


/*****************************************************************************/
// 	Description : delete a content node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index删除收藏节目
/*****************************************************************************/
PUBLIC void MMIMBBMS_DelAlarmContentNodeViaIndex(uint32 index)
{
    MMIMBBMS_DelAlarmContentNode(MMIMBBMS_GetAlarmContentNodeViaIndex(index));
}

/*****************************************************************************/
// 	Description : get a content node via index
//	Global resource dependence : none
//  Author: 
//	Note: 依据index获取节目节点
/*****************************************************************************/
PUBLIC MMIMBBMS_ALARM_CONTENT_INFO_T* MMIMBBMS_GetAlarmContentNodeViaIndex(uint32 index)
{
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = PNULL;

	if (s_data_data_ptr == PNULL)
	{
	    return PNULL;
	}
	
	cursor = MMIMTV_DATA_PTR->alarm_content_head_ptr;
    
    if (cursor == PNULL)
    {
        return cursor;
    }
    while(index --)
    {
        if(PNULL == cursor)
        {
            return PNULL;
        }
        
        cursor = cursor->content_next;
    }
    return cursor;
}

/*****************************************************************************/
// 	Description : delete a content node from favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 删除收藏频道
/*****************************************************************************/
PUBLIC MMIMBBMS_ALARM_CONTENT_INFO_T* MMIMBBMS_GetAlarmContentNodeByContentID(anyURI *pGlobalContentID)
{
    BOOLEAN   bResult = FALSE;
    MMIMBBMS_ALARM_CONTENT_INFO_T* temp = PNULL;
    MMIMBBMS_ALARM_CONTENT_INFO_T* pre_node = PNULL;

    temp = MMIMTV_DATA_PTR->alarm_content_head_ptr;
    pre_node = temp;

    while(temp != PNULL)
    {
        bResult = MMIMBBMS_IndexCompare(pGlobalContentID, &(temp->globalContentID));

        if (bResult)
        {
            return temp;      
        }

        pre_node = temp;
        temp = temp->content_next;
    }

    return PNULL;
}


/*****************************************************************************/
// 	Description : clear up favorite content list
//	Global resource dependence : none
//  Author: 
//	Note: 清空收藏节目链表
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearAlarmContentList(void)
{
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = PNULL;
    
    if(PNULL != s_data_data_ptr)
    {
        while(PNULL != MMIMTV_DATA_PTR->alarm_content_head_ptr)
        {
            cursor = MMIMTV_DATA_PTR->alarm_content_head_ptr;
            MMIMTV_DATA_PTR->alarm_content_head_ptr = cursor->content_next;
            SCI_FREE(cursor);
        }
        MMIMTV_DATA_PTR->alarm_content_list_num = 0;
    }
}

/*****************************************************************************/
// 	Description : get favorite content num
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMBBMS_GetAlarmContentListNum(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_GetAlarmContentListNum  num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3303_112_2_18_2_40_19_590,(uint8*)"d",MMIMTV_DATA_PTR->alarm_content_list_num);
    return MMIMTV_DATA_PTR->alarm_content_list_num;
}

/*****************************************************************************/
// 	Description : 节目是否被收藏
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsAlarmContent(const anyURI *global_index)
{
    MMIMBBMS_ALARM_CONTENT_INFO_T* cursor = MMIMTV_DATA_PTR->alarm_content_head_ptr;
    
    if(PNULL == global_index)
    {
        return FALSE;
    }
    
    while(PNULL != cursor)
    {
        if(MMIMBBMS_IndexCompare(global_index, &(cursor->globalContentID)))
        {
            return TRUE;
        }
        
        cursor = cursor->content_next;
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : save key info of cmmb free service 
//	Global resource dependence : none
//  Author: 
//	Note: 保存CMMB清流频道参数信息,主要有network_id,service_id
/*****************************************************************************/
PUBLIC void MMIMBBMS_SaveFreeServiceInfoToFile(uint32 net_id,
                                               uint32 service_id
                                               )
{
    wchar  save_file_fullpath[MMIMTV_FULL_PATH_MAX_LEN+1] = {'E',':','\\','C','M','M','B','\\','8','8','0','0','g','c','m','m','b','t','e','s','t','.','i','n','i',0};/*lint !e785*/
    uint32 write_len = 0;
    char   str_info[MMIMTV_ENG_MODE_PROFILE_MAX_LEN+1] = {0};
    uint32 str_len = 0;
    uint32 err_ret = SFS_ERROR_NONE;
    MMIFILE_HANDLE file_handle = 0;
    
    uint16	bler_err_blk = 0;
    uint16	bler_num_blk = 0;
    int16	rssi_min = 0;
    int16	rssi_max = 0;
    uint32  rf_test_freq = 0;
    uint32  offset = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile net_id = %d, service_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3355_112_2_18_2_40_19_591,(uint8*)"dd", net_id, service_id);
    
    //assign default param
    rf_test_freq = 730000;
    bler_err_blk = 1;
    bler_num_blk = 100;
    rssi_min     = -100;
    rssi_max     = 0;
    
    //head_1
    sprintf(str_info, "%s\r\n", "[signal] ");
    
    //network_id and service_id
    offset = strlen(str_info);
    sprintf(str_info + offset, "%s%d\r\n%s%d\r\n", "network_id = ", net_id, "service_id = ", service_id);
    
    //channel param
    offset = strlen(str_info);
    sprintf(str_info + offset, "%s%d\r\n%s%d\r\n%s%d\r\n%s%d\r\n", "bler_err_blk = ", bler_err_blk, \
        "bler_num_blk = ", bler_num_blk, "rssi_min = ", rssi_min, "rssi_max = ", rssi_max);
    
    //head_2
    offset = strlen(str_info);
    sprintf(str_info + offset, "%s\r\n", "[no signal] ");
    
    //rf_search_freq
    offset = strlen(str_info);
    sprintf(str_info + offset, "%s%d\r\n", "rf_search_freq = ", rf_test_freq);
    
    //total len
    str_len = strlen(str_info);
    
    //E disk
    file_handle = MMIAPIFMM_CreateFile(save_file_fullpath, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {   
        err_ret = MMIAPIFMM_WriteFile(file_handle, str_info, str_len, &write_len, NULL);
        if(SFS_ERROR_NONE != err_ret)
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in E disk error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3394_112_2_18_2_40_19_592,(uint8*)"");
        }
        MMIAPIFMM_CloseFile(file_handle);
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in E disk ok! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3397_112_2_18_2_40_19_593,(uint8*)"");
    }
    
    
    //D disk
    save_file_fullpath[0] = MMIFILE_D_LETTER;
    file_handle = MMIAPIFMM_CreateFile(save_file_fullpath, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, NULL, NULL); /*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {
        err_ret = MMIAPIFMM_WriteFile(file_handle, str_info, str_len, &write_len, NULL);
        if(SFS_ERROR_NONE != err_ret)
        {
            //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile write file in D disk error! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3409_112_2_18_2_40_19_594,(uint8*)"");
        }
        MMIAPIFMM_CloseFile(file_handle);
        //SCI_TRACE_LOW:"[MMIMTV MMIMBBMS]: MMIMBBMS_SaveFreeServiceInfoToFile create file in D disk error! "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_DATA_3412_112_2_18_2_40_19_595,(uint8*)"");
    }
    
    return;
}

#endif
//MBBMS end


#endif /* CMMB_SUPPORT */

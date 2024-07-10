 /****************************************************************************
** File Name:      mmisrvaud_util.c                                                              *
** Author:          Yintang.ren                                                               *
** Date:             22/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to do some util functions.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 22/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#include "mmi_service_trc.h"
#include "mmisrvaud_api.h"
#include "mmisrvaud_util.h"
#include "mmi_link.h"
#include "mmk_timer.h"
#include "mmibt_export.h"
#include "gpio_prod_api.h"
//#include "mmi_appmsg.h"
#include "bt_abs.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmisrvvp.h"
//#include "mmivp_export.h"
#endif
#include "mmibt_a2dp.h"

#define STACK_UNIT_NUM     50
#define AUD_LINK_NODE_T   MMI_LINK_NODE_T

#define AUD_LINK_CREATE              MMILINK_CreateHead
#define AUD_LINK_CREATE_NODE    MMILINK_CreateNode
#define AUD_LINK_APPEND             MMILINK_AddNodeAfterBaseNode
#define AUD_LINK_PUSH                 MMILINK_AddNodeBeforeBaseNode
//#define AUD_LINK_GET                    MMILINK_DestroyNode 
#define AUD_LINK_DELETE              MMILINK_DestroyNode
#define AUD_LINK_SEARCH             MMILINK_SearchNode
#define AUD_LINK_GET_NUM           MMILINK_GetListNodesNum
#define AUD_LINK_DESTROY            MMILINK_DestroyLink

typedef struct
{
    uint32 stack[STACK_UNIT_NUM];
    uint16 num;
}AUD_STACK_T;


LOCAL AUD_LINK_NODE_T *s_aud_link_active = PNULL;
LOCAL AUD_LINK_NODE_T *s_aud_link_ready = PNULL;
LOCAL AUD_LINK_NODE_T *s_aud_link_standing = PNULL;
LOCAL AUD_LINK_NODE_T *s_aud_link_suspend = PNULL;

LOCAL AUD_LINK_NODE_T *s_aud_link_node_cursor = PNULL; /* Be used as common */

LOCAL AUD_STACK_T s_aud_stack = {0};
LOCAL MMISRVAUD_TYPE_E s_cur_audio_type = 0;

/*****************************************************************************/
//  Description:  Get link head by link type.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL AUD_LINK_NODE_T *GetLinkHeadByType(SRVAUD_LINK_E link_type)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetLinkHeadByType() entry, link_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_59_112_2_18_3_23_59_345,(uint8*)"d", link_type));

    switch(link_type)
    {
    case SRVAUD_LINK_ACTIVE:
        return s_aud_link_active;
    case SRVAUD_LINK_READY:
        return s_aud_link_ready;
    case SRVAUD_LINK_STANDING:
        return s_aud_link_standing;
    case SRVAUD_LINK_SUSPEND:
        return s_aud_link_suspend;
    default:
        return PNULL;
    }
}

/*****************************************************************************/
//  Description:  Link search compare function.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
BOOLEAN LinkSearch(const AUD_LINK_NODE_T *link_node, uint32 data1, uint32 data2)
{
    if(PNULL == link_node)
    {
        return FALSE;
    }
    
    return (link_node->data == data1);
}

/*****************************************************************************/
//  Description:  Init audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkCreate(void)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkCreate() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_96_112_2_18_3_23_59_346,(uint8*)""));

    /* if some link create failed, we stop create the other links, and do not destroy the links that have been created */
     s_aud_link_active = AUD_LINK_CREATE(0);
     if(PNULL == s_aud_link_active)
     {
        return FALSE;
     }
     
     s_aud_link_ready = AUD_LINK_CREATE(0);
     if(PNULL == s_aud_link_ready)
     {
        return FALSE;
     }
     
     s_aud_link_standing = AUD_LINK_CREATE(0);
     if(PNULL == s_aud_link_standing)
     {
        return FALSE;
     }

     s_aud_link_suspend = AUD_LINK_CREATE(0);
     if(PNULL == s_aud_link_suspend)
     {
        return FALSE;
     }

     _DBG(     //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkCreate() exit!!!"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_123_112_2_18_3_23_59_347,(uint8*)""));

     return TRUE;
}

/*****************************************************************************/
//  Description:  Push service entity to an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkPush(SRVAUD_LINK_E link_type, uint32 data)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkPush() entry, link_type=%d,data=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_139_112_2_18_3_23_59_348,(uint8*)"ddd", link_type, data, link_head));
    
    if(PNULL == link_head)
    {
        return FALSE;
    }

    node = AUD_LINK_CREATE_NODE(data);
    if(node != PNULL)
    {
        AUD_LINK_PUSH(link_head, node);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description:  Pop a service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkPop(SRVAUD_LINK_E link_type)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    uint32 data = 0;
    
    link_head = GetLinkHeadByType(link_type);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkPop() entry, link_type=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_171_112_2_18_3_23_59_349,(uint8*)"dd", link_type, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }
    
    if(link_head->prev_ptr != PNULL)
    {
        node = link_head->prev_ptr;
        data = node->data;
        AUD_LINK_DELETE(node, link_head, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkPop() exit, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_185_112_2_18_3_23_59_350,(uint8*)"d",data));

    return data;
}

/*****************************************************************************/
//  Description:  Append a service entity to an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkAppend(SRVAUD_LINK_E link_type, uint32 data)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkAppend() entry, link_type=%d,data=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_201_112_2_18_3_23_59_351,(uint8*)"ddd", link_type, data, link_head));
    
    if(PNULL == link_head)
    {
        return FALSE;
    }

    node = AUD_LINK_CREATE_NODE(data);
    if(node != PNULL)
    {
        AUD_LINK_APPEND(link_head, node);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkDeAppend(SRVAUD_LINK_E link_type)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    uint32 data = 0;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkDeAppend() entry, link_type=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_233_112_2_18_3_23_59_352,(uint8*)"dd", link_type, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }
    
    if(link_head->next_ptr != PNULL)
    {
        node = link_head->next_ptr;
        data = node->data;
        AUD_LINK_DELETE(node, link_head, PNULL);
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkDeAppend() exit, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_247_112_2_18_3_23_59_353,(uint8*)"d",data));
    
    return data;
}

/*****************************************************************************/
//  Description:  Get the top node of one link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_E link_type)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    uint32 data = 0;
    
    link_head = GetLinkHeadByType(link_type);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() entry, link_type=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_264_112_2_18_3_23_59_354,(uint8*)"dd", link_type, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }
    
    if(link_head->prev_ptr != PNULL)
    {
        node = link_head->prev_ptr;
        data = node->data;
    }

    s_aud_link_node_cursor = node;  /* save the cursor */
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetHeadNode() exit, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_279_112_2_18_3_24_0_355,(uint8*)"d",data));

    return data;
}

/*****************************************************************************/
//  Description:  Get the tail node of one link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetTailNode(SRVAUD_LINK_E link_type)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *node = PNULL;
    uint32 data = 0;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() entry, link_type=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_296_112_2_18_3_24_0_356,(uint8*)"dd", link_type, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }
    
    if(link_head->next_ptr != PNULL)
    {
        node = link_head->next_ptr;
        data = node->data;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetTailNode() exit, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_309_112_2_18_3_24_0_357,(uint8*)"d",data));
    
    return data;
}

/*****************************************************************************/
//  Description:  Get the next node of one link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: Pay attention!!! Please call MMISRVAUD_LinkGetHeadNode() first, 
//          otherwise it will occur unexpected error!!!
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNextNode(void)
{
    if(s_aud_link_node_cursor != PNULL)
    {
        s_aud_link_node_cursor = s_aud_link_node_cursor->prev_ptr;
        return s_aud_link_node_cursor->data;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNum(SRVAUD_LINK_E link_type)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    uint32 num = 0;
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, link_type=%d,link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_342_112_2_18_3_24_0_358,(uint8*)"dd", link_type, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }

    num = AUD_LINK_GET_NUM(link_head);

    /* Sub 1, because the link header is empty */
    if(num > 0)
    {
        num--;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetNum() entry, num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_357_112_2_18_3_24_0_359,(uint8*)"d", num));

    return num;
}

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link and delete it.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_E link_type, uint32 data)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *link_node = PNULL;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() entry, link_type=%d,data=0x%x, link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_373_112_2_18_3_24_0_360,(uint8*)"ddd", link_type, data, link_head));
    
    if(PNULL == link_head)
    {
        return FALSE;
    }

    link_node = AUD_LINK_SEARCH(link_head, TRUE, LinkSearch, data, 0);

    if(PNULL != link_node)
    {
        AUD_LINK_DELETE(link_node, link_head, PNULL);
       _DBG(       //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetAndDelete() exit, found! link_node=0x%x"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_385_112_2_18_3_24_0_361,(uint8*)"d",link_node));
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/*****************************************************************************/
//  Description:  Get the last service entity from an appointed audio service link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkGetNode(SRVAUD_LINK_E link_type, uint32 data)
{
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *link_node = PNULL;
    
    link_head = GetLinkHeadByType(link_type);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkGetNode() entry, link_type=%d,data=0x%x, link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_407_112_2_18_3_24_0_362,(uint8*)"ddd", link_type, data, link_head));
    
    if(PNULL == link_head)
    {
        return 0;
    }

    link_node = AUD_LINK_SEARCH(link_head, TRUE, LinkSearch, data, 0);

    if(PNULL != link_node)
    {
        return data;
    }
    else
    {
        return 0;
    }

}

/*****************************************************************************/
//  Description:  suspend the active link.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_LinkSuspend(void)
{
    AUD_LINK_NODE_T *link_head = GetLinkHeadByType(SRVAUD_LINK_ACTIVE);

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkSuspend() entry, active link_head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_435_112_2_18_3_24_0_363,(uint8*)"d", link_head));

    if(PNULL == link_head)
    {
        return 0;
    }

    s_aud_link_active = AUD_LINK_CREATE(0);
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkSuspend(), new active link head=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_443_112_2_18_3_24_0_364,(uint8*)"d", s_aud_link_active));

    return (uint32)link_head;
}

/*****************************************************************************/
//  Description:  resume the appointed suspended link 
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkResume(uint32 data)
{
    AUD_LINK_NODE_T *link_head = GetLinkHeadByType(SRVAUD_LINK_ACTIVE);
    uint32 num = 0;
    if(link_head != PNULL)
    {
        num = AUD_LINK_GET_NUM(link_head);
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkResume() entry, active link_head=0x%x,num=%d,data=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_459_112_2_18_3_24_0_365,(uint8*)"ddd", link_head, num, data));

        /* If there is active service still or the data to be resumed is zero, do nothing. */
        if(num > 1 || 0 == data )
        {
            return FALSE;
        }

        /* num is 1, which means it is the link head node, we should destroy the old link. */
        if(1 == num)
        {
            /* free the old head */
            AUD_LINK_DESTROY(link_head, PNULL);
        }
    }
    /* the data is the suspended link's head node */
    s_aud_link_active = (AUD_LINK_NODE_T *)data;

    return TRUE;
}

/*****************************************************************************/
//  Description:  Destroy all the links.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_LinkDestroy(void)
{
    if(s_aud_link_active != PNULL)
    {
        AUD_LINK_DESTROY(s_aud_link_active, PNULL);
    }
    if(s_aud_link_ready != PNULL)
    {
        AUD_LINK_DESTROY(s_aud_link_ready, PNULL);
    }
    if(s_aud_link_standing != PNULL)
    {
        AUD_LINK_DESTROY(s_aud_link_standing, PNULL);
    }
    if(s_aud_link_suspend != PNULL)
    {
        AUD_LINK_DESTROY(s_aud_link_suspend, PNULL);
    }
}

/*****************************************************************************/
//  Description:  Is Node Exist in traval link
//  Global resource dependence:
//  Author: changli
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsExistInLink(AUD_LINK_NODE_T *link_node,AUD_LINK_NODE_T *link_head)
{

    AUD_LINK_NODE_T *tmp_node = PNULL;
    BOOLEAN result = FALSE;

    if(PNULL == link_node || PNULL == link_head)
    {
        SCI_TRACE_LOW("[MMISRV]:IsExistInLink link_node FALSE : 0x%x link_head 0x%x",link_node, link_head);
        return FALSE;
    }

    if(link_node == link_head)
    {
       SCI_TRACE_LOW("[MMISRV]:IsExistInLink link_node:0x%x is equal to link_head 0x%x\n",link_node,link_head);
       return TRUE;
    }
    tmp_node = link_head->prev_ptr;
    while(tmp_node != link_head)
    {
        if(tmp_node == link_node)
        {
            result = TRUE;
            break;
        }
        tmp_node = tmp_node->prev_ptr;
    }
    if(!result)
    {
        SCI_TRACE_LOW("[MMISRV]:IsExistInLink link_node FALSE : 0x%x link_head 0x%x",link_node, link_head);
    }
    return result;
}

/*****************************************************************************/
//  Description:  Travel link and do somethine one by one.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkTravel(SRVAUD_LINK_E link_type, MMISRVAUD_LINK_TRAVEL_FUNC func, uint32 param)
{
    AUD_LINK_NODE_T *link_head = GetLinkHeadByType(link_type); 
    AUD_LINK_NODE_T *next_del_node = PNULL;
    AUD_LINK_NODE_T *del_node = PNULL;
    uint32 data = 0;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_513_112_2_18_3_24_0_366,(uint8*)"ddd", link_type, link_head, func));

    if(PNULL == link_head || PNULL == func)
    {
        return FALSE;
    }
    del_node = link_head->prev_ptr;
    next_del_node    = PNULL;
    MMISRV_TRACE_LOW("MMISRVAUD_LinkTravel,line:%d,del_node:0x%x\n",__LINE__,del_node);

    while(del_node != link_head)
    {
        /*through next_del_node,we can find the next node that should be dealed with func*/
        next_del_node = del_node->next_ptr;
        /* save the data temperally, because it may be freed after calling func */
        data = del_node->data;
        MMISRV_TRACE_LOW("MMISRVAUD_LinkTravel,line:%d,del_node->data:0x%x,next_del_node:0x%x,del_node->prev_ptr:0x%x\n",__LINE__,data,next_del_node,del_node->prev_ptr);
        //this func may delete two active service once or not delete any service
        func(data, param);

        // head may change when  execute func
        link_head = GetLinkHeadByType(link_type);

        if(TRUE == IsExistInLink(del_node,link_head))
        {
            del_node = del_node->prev_ptr;
            MMISRV_TRACE_LOW("MMISRVAUD_LinkTravel,line:%d,del_node:0x%x\n",__LINE__,del_node);
        }
        else
        {
            if(TRUE == IsExistInLink(next_del_node,link_head))
            {
                del_node = next_del_node->prev_ptr;
                MMISRV_TRACE_LOW("MMISRVAUD_LinkTravel,line:%d,del_node:0x%x\n",__LINE__,del_node);
            }
            else
            {
                MMISRV_TRACE_LOW("MMISRVAUD_LinkTravel:ERROR!!!,next_del_node is Not Exist!!!\n");
                return FALSE;
            }
        }
    }
    return TRUE;

}
/*****************************************************************************/
//  Description:  Travel link and do somethine one by one.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_LinkTravelEx(SRVAUD_LINK_E link_type, MMISRVAUD_LINK_TRAVEL_EX_FUNC func, uint32 param, uint32 *res)
{
    AUD_LINK_NODE_T *link_head = GetLinkHeadByType(link_type);
    AUD_LINK_NODE_T *tmp_node = PNULL;
    uint32 data = 0;
    BOOLEAN result = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_LinkTravel() entry, link_type=%d, link_head=0x%x, func=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_541_112_2_18_3_24_0_367,(uint8*)"ddd", link_type, link_head, func));

    if(PNULL == link_head || PNULL == func)
    {
        return FALSE;
    }

    tmp_node = link_head->prev_ptr;
    while(tmp_node != link_head)
    {
        /* save the data temperally, because it may be freed after calling func */
        data = tmp_node->data;
        tmp_node = tmp_node->prev_ptr;
        /* if return TRUE once, we think the all process is TRUE */
        if(!result)
        {
            result = func(data, param, res);
        }
        else
        {
            func(data, param, res);
        }
        
    }
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio MMISRVAUD_LinkTravel() exit, result=%d", result));
    
    return result;
}

/*****************************************************************************/
//  Description:  push the current active link to stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StackPush(void)
{
    uint32 data = 0;

    data = (uint32)GetLinkHeadByType(SRVAUD_LINK_ACTIVE);
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPush entry, data=0x%x, stack_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_577_112_2_18_3_24_0_368,(uint8*)"dd", data, s_aud_stack.num));
    
    if(MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE) == 0 && 0 == data)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPush, active link num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_581_112_2_18_3_24_0_369,(uint8*)"d", MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE)));
        return;
    }
    if(s_aud_stack.num < STACK_UNIT_NUM)
    {
        s_aud_stack.stack[s_aud_stack.num] = data;
        s_aud_stack.num++;
        s_aud_link_active = AUD_LINK_CREATE(0);
    }
    else
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio, stack is full!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_592_112_2_18_3_24_0_370,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:  pop a link from stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_StackPop(void)
{
    uint32 data = 0;
    uint32 ret_data = 0;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPop entry, s_aud_stack.num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_605_112_2_18_3_24_0_371,(uint8*)"d", s_aud_stack.num));
    
    while(s_aud_stack.num > 0)
    {
        s_aud_stack.num--;
        data = s_aud_stack.stack[s_aud_stack.num];
        s_aud_stack.stack[s_aud_stack.num] = 0;
        
        /* if this link num is 0 or 1, it means this link is empty.
            we should ignore this link and destroy it, and then pop next link */
        if(AUD_LINK_GET_NUM((AUD_LINK_NODE_T *)data) > 1)
        {
            /* link is not empty, pop suceess and jump out loop */
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPop, success!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_618_112_2_18_3_24_0_372,(uint8*)""));
            ret_data = data;
            break;
        }
        else
        {
            /* Empty link, destroy it and go on next */
            AUD_LINK_DESTROY((AUD_LINK_NODE_T *)data, PNULL);
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPop, this link is empty!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_626_112_2_18_3_24_0_373,(uint8*)""));
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackPop exit, data=0x%x, s_aud_stack.num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_629_112_2_18_3_24_0_374,(uint8*)"dd", data, s_aud_stack.num));
    
    return ret_data;
}

/*****************************************************************************/
//  Description:  Get the link from stack by data.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_StackGetLink(uint32 data)
{
    uint16 i = 0;
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *link_node = PNULL;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetLink entry, data=0x%x, s_aud_stack.num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_644_112_2_18_3_24_0_375,(uint8*)"dd", data, s_aud_stack.num));

    if(0 == data)
    {
        return 0;
    }

    
    /* first, we find the position of the data in the stack */
    while(i < s_aud_stack.num)
    {
        if(s_aud_stack.stack[i] != 0)
        {
            link_head = (AUD_LINK_NODE_T *)s_aud_stack.stack[i];
            link_node = AUD_LINK_SEARCH(link_head, TRUE, LinkSearch, data, 0);
            if(link_node != 0)
            {
                break;
            }
        }
        i++;
    }

    /* if found, make the stack decline from the 'data' position */
    if(i < s_aud_stack.num)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetLink,Found! i=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_669_112_2_18_3_24_0_376,(uint8*)"d", i));
    
        while(i < s_aud_stack.num - 1)
        {
            s_aud_stack.stack[i] = s_aud_stack.stack[i + 1];
            i++;
        }
        /* set the topest one to zero */
        s_aud_stack.stack[i] = 0;
        s_aud_stack.num--;
    }    
    /* if not found, return immediately */
    else
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetLink, Not Found!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_683_112_2_18_3_24_0_377,(uint8*)""));
        link_head = PNULL;
    }
    return (uint32)link_head;
}

/*****************************************************************************/
//  Description:  Get the data from stack and delete it from stack.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_StackGetAndDelete(uint32 data)
{
    uint16 i = 0;
    AUD_LINK_NODE_T *link_head = PNULL;
    AUD_LINK_NODE_T *link_node = PNULL;
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete entry, data=0x%x, s_aud_stack.num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_700_112_2_18_3_24_0_378,(uint8*)"dd", data, s_aud_stack.num));
    
    if(0 == data)
    {
        return 0;
    }

    
    /* First, we find the position of the data in the stack */
    while(i < s_aud_stack.num)
    {
        if(s_aud_stack.stack[i] != 0)
        {
            link_head = (AUD_LINK_NODE_T *)s_aud_stack.stack[i];
            link_node = AUD_LINK_SEARCH(link_head, TRUE, LinkSearch, data, 0);
            
            /* find one node, delete it form link */
            if(link_node != 0)
            {
                /* delete it from the link */
                AUD_LINK_DELETE(link_node, link_head, PNULL);
                res = TRUE;

                /* if the link has no other node, destroy it */
                if(AUD_LINK_GET_NUM(link_head) <= 1)
                {
                    _DBG(                    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current link is empty!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_725_112_2_18_3_24_0_379,(uint8*)""));
                    
                    /* Empty link, destroy it */
                    AUD_LINK_DESTROY(link_head, PNULL);

                    /* make the stack decline, and set the stack's top to zero */
                    if(i < s_aud_stack.num)
                    {
                        _DBG(                        //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete, current is not top! i=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_733_112_2_18_3_24_0_380,(uint8*)"d", i));
                    
                        while(i < s_aud_stack.num - 1)
                        {
                            s_aud_stack.stack[i] = s_aud_stack.stack[i + 1];
                            i++;
                        }
                    }
                    /* set the topest one to zero */
                    s_aud_stack.stack[i] = 0;
                    s_aud_stack.num--;                    
                }

                break;
            }
        }
        i++;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StackGetAndDelete exit, i=%, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_751_112_2_18_3_24_0_381,(uint8*)"d", i, res));
    
    return res;
}

/*****************************************************************************/
//  Description:  Get the stack num.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMISRVAUD_StackGetNum(void)
{
    return s_aud_stack.num;
}

/*****************************************************************************/
//  Description:  Make a judgement if headset is plug in.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsHeadSetPlugIn(void)
{
    BOOLEAN res = FALSE;
    
    #ifndef WIN32
    if (GPIO_CheckHeadsetStatus()) 
    {
        res = TRUE;
    }
    #endif

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_IsHeadSetPlugIn =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_780_112_2_18_3_24_1_382,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description:  Make a judgement if BT headset is active.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsBTHeadSetActive(void)
{
#ifdef WIN32
    return FALSE;
#else

#ifdef BLUETOOTH_SUPPORT
    BOOLEAN res = MMIAPIBT_GetActiveBTHeadset();
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_IsBTHeadSetActive =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_797_112_2_18_3_24_1_383,(uint8*)"d", res));

    return res;
#else
    return FALSE;
#endif

#endif
}


/*****************************************************************************/
//  Description:  Create and start a timer with callback function.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMISRVAUD_CreateTimer(
                    uint32 time_out,
                    MMISRVAUD_TIMER_CB_FUNC func,
                    uint32 param,
                    BOOLEAN is_period
                    )
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_CreateTimer entry, time_out=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_819_112_2_18_3_24_1_384,(uint8*)"d", time_out));

    return MMK_CreateTimerCallback(time_out, (MMI_TIMER_FUNC)func, param, is_period);/*lint !e611*/
}

/*****************************************************************************/
//  Description:  Close timer.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_StopTimer(
                    uint8 timer_id
                    )
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_StopTimer entry, timer_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_832_112_2_18_3_24_1_385,(uint8*)"d", timer_id));

    if(timer_id != 0)
    {
        MMK_StopTimer (timer_id);
    }
}

/*****************************************************************************/
//  Description:  If the BT headset is a2dp type.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtIsA2dp(void)
{
#ifdef BLUETOOTH_SUPPORT
    BOOLEAN is_a2dp = MMIBT_IsExitedActiveA2dp();
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio, MMISRVAUD_BtIsA2dp entry, is_a2dp=%d", is_a2dp));
    return is_a2dp;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description:  Open BT device.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtOpen(MMISRVAUD_TYPE_U *type_ptr, uint32 sample_rate)
{
#ifdef BLUETOOTH_SUPPORT
    BOOLEAN result = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_BtOpen entry, type_ptr=0x%x,sample_rate=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_849_112_2_18_3_24_1_386,(uint8*)"dd", type_ptr, sample_rate));

    if(type_ptr == PNULL)
    {
        return FALSE;
    }
    switch(type_ptr->type)
    {
    case MMISRVAUD_TYPE_ATV:
        MMIAPIBT_SpecifyHeadset(MMIBT_HFG_HEADSET);
        result = MMIAPIBT_A2dpOpenDevice_1(sample_rate);
        break;
    case MMISRVAUD_TYPE_VOICE:
        //HFG_AudioReqSend(TRUE);
        BT_TransferAudioConn(BT_AUDIO_CONN_AG_TO_HF);
        result = TRUE;
        break;
#ifdef VT_SUPPORT
	case MMISRVAUD_TYPE_VIRTUAL:
		if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
		{
		        BT_TransferAudioConn(BT_AUDIO_CONN_AG_TO_HF);
		        result = TRUE;		
		}
		break;
#endif
    default:
        MMIAPIBT_SpecifyHeadset(MMIBT_DEFAULT_HEADSET);
        result = MMIAPIBT_A2dpOpenDevice_1(sample_rate);
        break;
    }

    s_cur_audio_type = type_ptr->type;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_BtOpen exit, result=%d,type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_874_112_2_18_3_24_1_387,(uint8*)"dd", result, type_ptr->type));

    return result;
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description:  Close BT device.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_BtClose(void)
{
#ifdef BLUETOOTH_SUPPORT
    BOOLEAN result = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_BtClose entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_891_112_2_18_3_24_1_388,(uint8*)""));

    //if(PNULL == type_ptr)
    //{
    //    return FALSE;
    //}

    switch(s_cur_audio_type)
    {
    case MMISRVAUD_TYPE_VOICE:
        //HFG_AudioReqSend(FALSE);
        BT_TransferAudioConn(BT_AUDIO_CONN_HF_TO_AG);
        break;
#ifdef VT_SUPPORT
	case MMISRVAUD_TYPE_VIRTUAL:
		if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
		{
     		 BT_TransferAudioConn(BT_AUDIO_CONN_HF_TO_AG);
		}
		break;
#endif
    default:
        result = MMIAPIBT_A2dpStopDevice_1();
        break;
    }

    s_cur_audio_type = 0;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio, MMISRVAUD_BtClose exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_UTIL_911_112_2_18_3_24_1_389,(uint8*)"d", result));

    return result;
#else
    return FALSE;
#endif
}


/*****************************************************************************/
//  Description : Set video's sound is play or stop.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_SetVideoSound(BOOLEAN is_sound_on)
{
#ifdef VIDEO_PLAYER_SUPPORT
	MMIAPIVP_SetBgVideoAudioSupport(is_sound_on);
#endif 
}

/*****************************************************************************/
//  Description : Create a mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_MUTEX_PTR MMISRVAUD_CreateMutex(void)
{
    MMISRVAUD_MUTEX_PTR mutex_ptr = PNULL;
    mutex_ptr = SCI_CreateMutex(STR_SRV_AUD_NAME, SCI_INHERIT);
    return mutex_ptr;
}

/*****************************************************************************/
//  Description : Get the appointed mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_GetMutext(MMISRVAUD_MUTEX_PTR mutex_ptr)
{
    SCI_GetMutex(mutex_ptr, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
}

/*****************************************************************************/
//  Description : Free the appointed mutex.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_PutMutext(MMISRVAUD_MUTEX_PTR mutex_ptr)
{
    SCI_PutMutex(mutex_ptr);
}

/*****************************************************************************/
//  Description : If in calling state
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_IsInCall(void)
{
    return MMICC_IsAudioHandleEnable();
}

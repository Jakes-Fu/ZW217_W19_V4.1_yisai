/*****************************************************************
 **  File Name: 	brw_history.c				     			 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of history class                   *
 **  feature list:                                               *
 **  1)��ҳ������ʷ��¼:��ҳ�����ʱ��,����"ǰ��","����",�����浽*
 **    �̶��洢����                                              *
 **  2)ҳ�������¼:ά���û��������ҳ���б�,���浽�̶��洢����, *
 **    �ػ��ؿ���Ȼ���Կ���                                      *
 **  3)������¼�ֿ��洢,�����ά��BRW_MAX_HIS_ITEM_NUM��         *
 **  4)��ҳ�������¼�ﵽ�������Ƶ�ʱ��,��̭�����ϵ��Ǹ�         *
 **  5)��ҳ�������¼�б����Ѿ���һ����url�����,������,ֻ�Ǹ��� *
 **    ���ʴ���,������ʱ���λ��                               *
 **  6)ҳ�������¼����ɾ��,ɾ������                             *
 **  7)��ʱ��֧������(��д�ļ�����),���Ҫ֧������,��Ҫ�޸��ļ���*
 **    ��д,����ȫ�ֱ����޸�Ϊ������                             *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/

#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "brw_history.h"

#define BRW_HISTORY_PATH	"Browser\\History"

//��ҳ������ʷ��¼
typedef struct tagBRW_NAV_HIS_LIST_T
{
    uint8   url_arr[BRW_MAX_URL_LEN];
    WAP_REQUEST_T   *wap_request_cache_ptr;
    struct tagBRW_NAV_HIS_LIST_T    *prev_ptr;
    struct tagBRW_NAV_HIS_LIST_T    *next_ptr;
}BRW_NAV_HIS_LIST_T;

LOCAL BRW_NAV_HIS_LIST_T  *s_brw_nav_his_list_ptr = PNULL;//������ʷ��¼�б�
LOCAL BRW_NAV_HIS_LIST_T  *s_brw_nav_cur_his_ptr = PNULL;//������ʷ��¼��ǰ��¼
LOCAL uint16  s_brw_nav_his_item_num = 0;
LOCAL uint16  s_brw_nav_cache_req_num = 0;

LOCAL BRW_VISIT_HIS_QUE_T *s_brw_visit_his_que_ptr = PNULL;//ҳ�������ʷ��¼
/*
 *	desc: free navigate history list
 */
LOCAL void FreeNavHisList(BRW_NAV_HIS_LIST_T** brw_nav_his_list_pptr);
LOCAL void NavHisFreeCurCacheReq(BRW_NAV_HIS_LIST_T *nav_ptr);

PUBLIC void BrwNavHisAddCache(WAP_REQUEST_T *to_be_cached_req_ptr)
{
    if (PNULL == to_be_cached_req_ptr)
    {
        return;
    }
    
    if ((PNULL == to_be_cached_req_ptr->absolute_url) ||
        (PNULL == to_be_cached_req_ptr->http_req_ptr) ||
        (PNULL == to_be_cached_req_ptr->layout_handle)
        )
    {
        //BRWDebug_Printf:"BrwNavHisAddCache: cathed request error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_62_112_2_18_2_7_14_646,(uint8*)"");
        return;
    }
    
    if ((PNULL == s_brw_nav_cur_his_ptr) || (PNULL == s_brw_nav_his_list_ptr))
    {
        //BRWDebug_Printf:"BrwNavHisAddCache: error 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_68_112_2_18_2_7_14_647,(uint8*)"");
        return;
    }
    //������cache�����ֵ,ɾ���ɵĸ��µ���ǰ
    if (s_brw_nav_cache_req_num >= BRW_NAV_MAX_CACHE_REQ_NUM)     
    {
        if (PNULL == s_brw_nav_cur_his_ptr->wap_request_cache_ptr)//�ͷ�����ǰ���cache req
        {
            BRW_NAV_HIS_LIST_T *temp_nav_ptr = s_brw_nav_his_list_ptr;

            while(temp_nav_ptr != PNULL)
            {
                if (temp_nav_ptr->wap_request_cache_ptr != PNULL)
                {
                    break;
                }
                else
                {
                    temp_nav_ptr = temp_nav_ptr->next_ptr;
                }
            }
            
            if(temp_nav_ptr != PNULL)
            {
                //BRWDebug_Printf:"brw BrwNavHisAddCache, free cathe"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_92_112_2_18_2_7_14_648,(uint8*)"");
                NavHisFreeCurCacheReq(temp_nav_ptr);
            }
        }
        {
            /*�滻��ǰs_brw_nav_cur_his_ptr,
            Ŀǰ�������ֻ������BrwNavHisAddItem(ֻ�����������url��ͬ�ļ�¼),����������ͬurl����ʱ*/
        }
    }

    NavHisFreeCurCacheReq(s_brw_nav_cur_his_ptr);//����s_brw_nav_cur_his_ptr->wap_request_cache_ptr���ڴ�й©
    /*ע:�˴�ֱ�Ӷ�s_brw_nav_cur_his_ptr->wap_request_cache_ptr��ֵ����,����Ӧ��BrwNavHisAddItem�뱾�������*/
    s_brw_nav_cur_his_ptr->wap_request_cache_ptr = to_be_cached_req_ptr;
    s_brw_nav_cache_req_num ++;
}
PUBLIC void BrwNavHisFreeCurCacheReq(void)
{
    if (s_brw_nav_cur_his_ptr != PNULL)
    {
        NavHisFreeCurCacheReq(s_brw_nav_cur_his_ptr);
    }
}

PUBLIC BOOLEAN BrwNavClearCache(void)
{
    BRW_NAV_HIS_LIST_T  *cur_his_ptr = s_brw_nav_his_list_ptr;
    BOOLEAN has_cathe_to_clear = FALSE;

    while (cur_his_ptr != PNULL)
    {
        if(!BrwIsCacheUsing(cur_his_ptr->wap_request_cache_ptr))
        {
            //BRWDebug_Printf:"brw BrwNavClearCache free cathe"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_124_112_2_18_2_7_14_649,(uint8*)"");
            NavHisFreeCurCacheReq(cur_his_ptr);
            has_cathe_to_clear = TRUE;
        }
        cur_his_ptr = cur_his_ptr->next_ptr;
    }
    return has_cathe_to_clear;
}

LOCAL void NavHisFreeCurCacheReq(BRW_NAV_HIS_LIST_T *nav_ptr)
{
    BRW_NAV_HIS_LIST_T  *cur_his_ptr = s_brw_nav_his_list_ptr;
    BOOLEAN is_after_ptr = FALSE;
    BOOLEAN is_need_clean = FALSE;

    if ((nav_ptr != PNULL) && (nav_ptr->wap_request_cache_ptr != PNULL))
    {
        //to check if the cache to be cleared is using in other records.
        while (cur_his_ptr != PNULL)
        {
            if (cur_his_ptr != nav_ptr) 
            {
                if(cur_his_ptr->wap_request_cache_ptr == nav_ptr->wap_request_cache_ptr)
                {
                    if (!is_after_ptr) 
                    {
                        return;
                    }
                    else
                    {
                        is_need_clean = TRUE;
                    }
                }
            }
            else
            {
                is_after_ptr = TRUE;
            }
            cur_his_ptr = cur_his_ptr->next_ptr;
        }

        if (is_need_clean)/*���nav list��wap_request_cache_ptr��ͬ��,����ʹ���Ѿ��ͷŵ�ָ��*/
        {
            cur_his_ptr = nav_ptr->next_ptr;
            while (cur_his_ptr != PNULL)
            {
                if(cur_his_ptr->wap_request_cache_ptr == nav_ptr->wap_request_cache_ptr)
                {
                    cur_his_ptr->wap_request_cache_ptr = PNULL;
                }
                cur_his_ptr = cur_his_ptr->next_ptr;
            }
        }
    	
        BrwFreeHttpRequest(nav_ptr->wap_request_cache_ptr->http_req_ptr);
        nav_ptr->wap_request_cache_ptr->http_req_ptr = PNULL;
        BRWMem_Free(nav_ptr->wap_request_cache_ptr->absolute_url);
        nav_ptr->wap_request_cache_ptr->absolute_url = PNULL;
        BrwDestroyLayoutCtrl(nav_ptr->wap_request_cache_ptr->layout_handle);
        //BRWDebug_Printf:"[BRW]NavHisFreeCurCacheReq,BrwDestroyLayoutCtrl: 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_183_112_2_18_2_7_14_650,(uint8*)"d", nav_ptr->wap_request_cache_ptr->layout_handle);
        nav_ptr->wap_request_cache_ptr->layout_handle = BRW_INVALID_LAYOUT_HANDLE;
        BRWMem_Free(nav_ptr->wap_request_cache_ptr);
        //BRWDebug_Printf:"brw free wap request cathe:%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_186_112_2_18_2_7_14_651,(uint8*)"d", nav_ptr->wap_request_cache_ptr);
        nav_ptr->wap_request_cache_ptr = PNULL;
        s_brw_nav_cache_req_num--;
    }
}

PUBLIC WAP_REQUEST_T* BrwNavHisGetBackwardCacheReq(void)
{
    if ((PNULL == s_brw_nav_cur_his_ptr) || (PNULL == s_brw_nav_his_list_ptr))
    {
        return PNULL;
    }

    if (PNULL == s_brw_nav_cur_his_ptr->prev_ptr)
    {
        return PNULL;
    }
    return s_brw_nav_cur_his_ptr->prev_ptr->wap_request_cache_ptr;
}
/*
 *	desc:add an visit history item
 *  param:url_ptr  url of item
 *        name_ptr  name of item,in unicode format
 *        name_len  length of name
 *  return: TRUE if sucess
 */
PUBLIC BOOLEAN BrwVisitHisAddItem(uint8 *url_ptr,uint16 *name_ptr,uint16 name_len)
{    
    BOOLEAN result = TRUE;
    uint16  i=0;
    uint16  oldest_index=0;
    uint32  oldest_time=0;
    uint16  url_len=0;
    BRW_VISIT_HIS_QUE_T *visit_his_que_ptr = s_brw_visit_his_que_ptr;

    url_len = (uint16)BRW_Strlen(url_ptr);
    
    if ((0 == url_len) || (url_len > BRW_MAX_URL_LEN))
    {
        //BRWDebug_Printf:"BrwVisitHisAddItem:url len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_224_112_2_18_2_7_15_652,(uint8*)"d",url_len);
        return FALSE;
    }
    
    if (PNULL == visit_his_que_ptr)
    {
        //BRWDebug_Printf:"[BRW]BrwVisitHisAddItem:s_brw_visit_his_que_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_230_112_2_18_2_7_15_653,(uint8*)"");
        return FALSE;
    }
    
    for( ;i < BRW_MAX_VISIT_HIS_ITEM_NUM; i++)
    {
        if(!visit_his_que_ptr->his_item_que[i].is_used)
        {
            break;
        }
        else if(BRW_Stricmp(url_ptr,visit_his_que_ptr->his_item_que[i].visit_his_info.url_ptr)==0)
        {
            //just update last visit time and visit count
            BRW_Memset(visit_his_que_ptr->his_item_que[i].visit_his_info.name_ptr, 0, sizeof(visit_his_que_ptr->his_item_que[i].visit_his_info.name_ptr));
            name_len = MIN(name_len, BRW_MAX_HIS_NAME_LEN);
            BRW_Memcpy(visit_his_que_ptr->his_item_que[i].visit_his_info.name_ptr, name_ptr, name_len*sizeof(uint16));
            visit_his_que_ptr->his_item_que[i].visit_his_info.name_len=name_len;
            visit_his_que_ptr->his_item_que[i].visit_his_info.last_visit_time=BRWPL_GetCurTime();
            visit_his_que_ptr->his_item_que[i].visit_his_info.visit_count++;
            return TRUE;
        }
        else if((0 == oldest_time) || (visit_his_que_ptr->his_item_que[i].visit_his_info.last_visit_time < oldest_time))
        {
            oldest_time = visit_his_que_ptr->his_item_que[i].visit_his_info.last_visit_time;
            oldest_index= i;
        }
    }

    if (BRW_MAX_VISIT_HIS_ITEM_NUM == i)
    {
        //��̭���ϵ��Ǹ�
        BRW_Memset(&visit_his_que_ptr->his_item_que[oldest_index].visit_his_info, 0, sizeof(BRW_VISIT_HIS_INFO_T));    
        i = oldest_index;
    }
    
    BRW_Memcpy(visit_his_que_ptr->his_item_que[i].visit_his_info.url_ptr, url_ptr, url_len);/* parasoft-suppress BD-RES-INVFREE "���жϼ��" */
    name_len = MIN(name_len, BRW_MAX_HIS_NAME_LEN);
    if(name_len != 0)
    {
        BRW_Memcpy(visit_his_que_ptr->his_item_que[i].visit_his_info.name_ptr, name_ptr, name_len*sizeof(uint16));
    }
    visit_his_que_ptr->his_item_que[i].is_used = TRUE;
    visit_his_que_ptr->his_item_que[i].visit_his_info.name_len = name_len;
    visit_his_que_ptr->his_item_que[i].visit_his_info.last_visit_time = BRWPL_GetCurTime();
    visit_his_que_ptr->his_item_que[i].visit_his_info.visit_count = 1;
    
    return result;
}
/*
 *	desc:delete an visit history item
 *  param:url_ptr  url of item
 *  return: 
 */
PUBLIC BOOLEAN BRW_VisitHisDelItem(uint16 index)
{
    //BRWDebug_Printf:"[BRW]BRW_VisitHisDelItem:index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_284_112_2_18_2_7_15_654,(uint8*)"d", index);

    if ((0  == index) || (index > BRW_MAX_VISIT_HIS_ITEM_NUM) || (PNULL == s_brw_visit_his_que_ptr))
    {
        //BRWDebug_Printf:"[BRW]BRW_VisitHisDelItem ERROR:index =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_288_112_2_18_2_7_15_655,(uint8*)"d", index);
        return FALSE;
    }
    
    BRW_Memset(&s_brw_visit_his_que_ptr->his_item_que[index-1], 0x00, sizeof(BRW_VISIT_HIS_ITEM_T)); 

    return TRUE;
}

/*
 *	desc:delete an visit history
 *  param:url_ptr  url of item
 *  return: 
 */
PUBLIC void BRW_VisitHisDel(uint8* url_ptr)
{
    uint16  i=0;
    BRW_VISIT_HIS_QUE_T *visit_his_que_ptr = s_brw_visit_his_que_ptr;
    
    if (PNULL == url_ptr)
    {
        BRWDebug_PrintWarning("BRW_VisitHisDel:url null");
        return;
    }
    
    if (PNULL == visit_his_que_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRW_VisitHisDel:s_brw_visit_his_que_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_314_112_2_18_2_7_15_656,(uint8*)"");
        return;
    }
    
    for(;i<BRW_MAX_VISIT_HIS_ITEM_NUM;i++)
    {
        if(visit_his_que_ptr->his_item_que[i].is_used)
        {
            if(0==BRW_Stricmp(visit_his_que_ptr->his_item_que[i].visit_his_info.url_ptr,url_ptr))
            {
                BRW_Memset(&visit_his_que_ptr->his_item_que[i].visit_his_info,0,sizeof(BRW_VISIT_HIS_INFO_T)); 
                visit_his_que_ptr->his_item_que[i].is_used=FALSE;
                return;
            }
        }
    }
}

/*
 *	desc:clean visit history list
 */
PUBLIC void Brw_VisitHisClean(void)
{
    if(PNULL != s_brw_visit_his_que_ptr)
    {
        BRW_Memset(s_brw_visit_his_que_ptr,0,sizeof(BRW_VISIT_HIS_QUE_T));
    }
}
/*
 *	desc:init navigate history
 */
PUBLIC BOOLEAN BRW_VisitHisInit(void)
{
    uint32  read_len=0;
    
    s_brw_visit_his_que_ptr = (BRW_VISIT_HIS_QUE_T*)BRWMem_Alloc(sizeof(BRW_VISIT_HIS_QUE_T));
    
    if (PNULL == s_brw_visit_his_que_ptr)
    {
        BRWDebug_PrintWarning("BRW_VisitHisInit:malloc s_brw_visit_his_que_ptr failed");
        return FALSE;
    }
    
    BRW_Memset(s_brw_visit_his_que_ptr, 0, sizeof(BRW_VISIT_HIS_QUE_T));
    BRWUtil_ReadFileSyn(
            (const uint8*)BRW_HISTORY_FILE_NAME,
            (const uint8*)BRW_HISTORY_PATH,
            (uint8*)s_brw_visit_his_que_ptr,
            sizeof(BRW_VISIT_HIS_QUE_T),
            &read_len
            );    
            
    //BRWDebug_Printf:"BRW_VisitHisInit:read_len =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_364_112_2_18_2_7_15_657,(uint8*)"d", read_len);

    return TRUE;
}
/*
 *	desc:exist history,store data to persistent storage
 */
PUBLIC void BRW_VisitHisExit(void)
{
    if (s_brw_visit_his_que_ptr != PNULL)
    {
        BRWUtil_WriteFileSyn(
                (const uint8*)BRW_HISTORY_FILE_NAME,
                (const uint8*)BRW_HISTORY_PATH,
                (uint8*)s_brw_visit_his_que_ptr,
                sizeof(BRW_VISIT_HIS_QUE_T)
                );
        BRWMem_Free(s_brw_visit_his_que_ptr);
        s_brw_visit_his_que_ptr = PNULL;
    }    
}
/*
 *	desc:���ҳ�������¼����������
 *  param:index_arr_ptr ��������ָ��
 *  return:ҳ�������¼�ĸ���
 */
PUBLIC uint16 BRW_VisitHisGenIndexArr(BRW_VISIT_HIS_INDEX_QUE_T* index_arr_ptr)
{
    uint16  i=0,j=0,k=0;
    uint16  item_num=0;
    uint16  index=0;
    BRW_VISIT_HIS_QUE_T *visit_his_que_ptr = s_brw_visit_his_que_ptr;

    if ((PNULL == index_arr_ptr) || (PNULL == visit_his_que_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BRW_VisitHisGenIndexArr:index_arr_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_397_112_2_18_2_7_15_658,(uint8*)"d",index_arr_ptr);
        return 0;
    }
    
    //����last_visit_time��������,�Ӹߵ���
    BRW_Memset(index_arr_ptr, 0, sizeof(BRW_VISIT_HIS_INDEX_QUE_T));
    
    for(i = 0;i < BRW_MAX_VISIT_HIS_ITEM_NUM; i++)
    {
        if (visit_his_que_ptr->his_item_que[i].is_used)
        {
            if (0 == item_num)
            {
                index_arr_ptr->index_arr[0]=i+1;
            }//if(item_num==0)
            else
            {
                //��������
                for(j=0;j<item_num;j++)
                {
                    if(index_arr_ptr->index_arr[j] != 0)
                    {
                        index=index_arr_ptr->index_arr[j]-1;
                        if(visit_his_que_ptr->his_item_que[i].visit_his_info.last_visit_time 
                            > visit_his_que_ptr->his_item_que[index].visit_his_info.last_visit_time)
                        {
                            for(k=item_num;k>j;k--)
                            {
                                index_arr_ptr->index_arr[k]=index_arr_ptr->index_arr[k-1];
                            }
                            index_arr_ptr->index_arr[j]=i+1;
                            break;
                        }
                        else if(j==item_num-1)
                        {
                            index_arr_ptr->index_arr[item_num]=i+1;
                        }                        
                    }
                }//for(;j<item_num;j++)
            }//if(item_num==0) else
            item_num++;
        }//if(visit_his_que_ptr->his_item_que[i].is_used)
    }//for(i=0;i<BRW_MAX_HIS_ITEM_NUM;i++)
    return item_num;
}
/*
 *	desc:���ҳ�������¼������
 *  param:
 *  return:ҳ�������¼�ĸ���
 */
PUBLIC uint16 BRW_VisitHisGetTotal(void)
{
    uint16  i=0;
    uint16  item_num=0;
    BRW_VISIT_HIS_QUE_T *visit_his_que_ptr = s_brw_visit_his_que_ptr;

    if (PNULL == visit_his_que_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BRW_VisitHisGetTotal:error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_454_112_2_18_2_7_15_659,(uint8*)"");
        return 0;
    }
        
    for(i = 0;i < BRW_MAX_VISIT_HIS_ITEM_NUM; i++)
    {
        if (visit_his_que_ptr->his_item_que[i].is_used)
        {
            item_num++;
        }
    }
    return item_num;
}

/*
 *	desc:����ĳ��������Ӧ��ҳ�������ʷ��¼��Ŀ��Ϣ
 *  param: index  ����
 *  return:�ɹ����ض�Ӧ��ҳ�������ʷ��¼��Ŀָ��,ʧ�ܷ���NULL
 */
PUBLIC  BRW_VISIT_HIS_INFO_T* BRW_VisitHisGetHisInfoByIndex(uint16 index)
{
    if ((0 == index) || (PNULL == s_brw_visit_his_que_ptr))
    {
        //BRWDebug_Printf:"BRW]W BRW_VisitHisGetHisInfoByIndex:index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_476_112_2_18_2_7_15_660,(uint8*)"d",index);
        return FALSE;
    }
    
    return &s_brw_visit_his_que_ptr->his_item_que[index-1].visit_his_info;
}
/*
 *	desc: free navigate history list
 */
LOCAL void FreeNavHisList(BRW_NAV_HIS_LIST_T** brw_nav_his_list_pptr)
{
    BRW_NAV_HIS_LIST_T  *node_ptr = PNULL;
    BRW_NAV_HIS_LIST_T  *next_node_ptr = PNULL;
    BRW_NAV_HIS_LIST_T  **head_pptr = brw_nav_his_list_pptr;
    
    if ((head_pptr != PNULL)&& (*head_pptr != PNULL))
    {
        node_ptr = *head_pptr;
        while (node_ptr != PNULL)
        {
            next_node_ptr = node_ptr->next_ptr;
            if (node_ptr->wap_request_cache_ptr != PNULL)
            {
                //BRWDebug_Printf:"brw FreeNavHisList free cathe"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_497_112_2_18_2_7_15_661,(uint8*)"");
                NavHisFreeCurCacheReq(node_ptr);
            }
            BRWMem_Free(node_ptr);
			*head_pptr = next_node_ptr;
            node_ptr = next_node_ptr;
            if(s_brw_nav_his_item_num > 0)
            {
                s_brw_nav_his_item_num--;
            }
        }
    }
}
/*
 *	desc:init navigate history
 */
PUBLIC void BrwNavHisInit(void)
{
    FreeNavHisList(&s_brw_nav_his_list_ptr);
    //s_brw_nav_his_list_ptr=PNULL;
    s_brw_nav_cur_his_ptr=PNULL;
    s_brw_nav_his_item_num=0;
    s_brw_nav_cache_req_num = 0;
}
/*
 *	desc:exit navigate history
 */
PUBLIC void BrwNavHisExit(void)
{
    BrwNavHisClean();
}
/*
 *	desc:clean navigate history
 */
PUBLIC void BrwNavHisClean(void)
{
    FreeNavHisList(&s_brw_nav_his_list_ptr);
    //s_brw_nav_his_list_ptr=PNULL;
    s_brw_nav_cur_his_ptr=PNULL;
    s_brw_nav_his_item_num=0;
    s_brw_nav_cache_req_num = 0;
}
/*
 *	desc:add navigate history item
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwNavHisAddItem(const uint8* url_ptr)
{
    BRW_NAV_HIS_LIST_T  *cur_nav_his_list_ptr = s_brw_nav_cur_his_ptr;
    BRW_NAV_HIS_LIST_T  *temp_nav_his_list_ptr = PNULL;
    uint16 url_len=0;
    
#ifndef WIN32 
    if (url_ptr != PNULL)
    {
        //BRWDebug_Printf:"BrwNavHisAddItem,%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_548_112_2_18_2_7_15_662,(uint8*)"s",url_ptr);
    }
#endif

    url_len=(uint16)BRW_Strlen(url_ptr);
    if ((0 == url_len) || (url_len >= BRW_MAX_URL_LEN))
    {
        //BRWDebug_Printf:"[BRW]BrwNavHisAddItem:url_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_555_112_2_18_2_7_15_663,(uint8*)"d",url_len);
        return FALSE;
    }
    
    if (PNULL == cur_nav_his_list_ptr)//first NAV history
    {
        if(PNULL != s_brw_nav_his_list_ptr)
        {
            BRWDebug_PrintWarning("BrwNavHisAddItem:wrong data!");
            return FALSE;
        }
        temp_nav_his_list_ptr = (BRW_NAV_HIS_LIST_T*)BRWMem_Alloc(sizeof(BRW_NAV_HIS_LIST_T));
        if (PNULL == temp_nav_his_list_ptr)
        {
            //BRWDebug_Printf:"[BRW]BrwNavHisAddItem:malloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_569_112_2_18_2_7_15_664,(uint8*)"");
            return FALSE;
        }
        BRW_Memset(temp_nav_his_list_ptr,0,sizeof(BRW_NAV_HIS_LIST_T));
        BRW_Memcpy(temp_nav_his_list_ptr->url_arr, url_ptr, url_len);
        s_brw_nav_his_list_ptr = temp_nav_his_list_ptr;
        s_brw_nav_cur_his_ptr = temp_nav_his_list_ptr;
        s_brw_nav_his_item_num++;
    }
    else
    {
        if (BRW_Stricmp(url_ptr, cur_nav_his_list_ptr->url_arr) == 0)
        {
            if(cur_nav_his_list_ptr->prev_ptr != PNULL)
            {
                if (BRW_Stricmp(url_ptr,cur_nav_his_list_ptr->prev_ptr->url_arr)==0)
                {
                    //��������������һ����
                    return TRUE;
                }
            }
        }
        
        temp_nav_his_list_ptr = (BRW_NAV_HIS_LIST_T*)BRWMem_Alloc(sizeof(BRW_NAV_HIS_LIST_T));
        if (PNULL == temp_nav_his_list_ptr)
        {
            //BRWDebug_Printf:"[BRW]BrwNavHisAddItem:malloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_595_112_2_18_2_7_16_665,(uint8*)"");
            return FALSE;
        }
        BRW_Memset(temp_nav_his_list_ptr, 0, sizeof(BRW_NAV_HIS_LIST_T));
        BRW_Memcpy(temp_nav_his_list_ptr->url_arr,url_ptr,url_len); 
        FreeNavHisList(&(cur_nav_his_list_ptr->next_ptr));
        //cur_nav_his_list_ptr->next_ptr = PNULL;
        if (s_brw_nav_his_item_num >= BRW_MAX_NAV_HIS_ITEM_NUM)
        {
            //��̭ͷ�ڵ�
            BRW_NAV_HIS_LIST_T  *head_his_list_ptr = s_brw_nav_his_list_ptr;
            s_brw_nav_his_list_ptr = head_his_list_ptr->next_ptr;
            head_his_list_ptr->next_ptr = PNULL;
            if (s_brw_nav_his_list_ptr != PNULL)
            {
                s_brw_nav_his_list_ptr->prev_ptr = PNULL;
            }
            else
            {
                cur_nav_his_list_ptr = PNULL;
            }
            FreeNavHisList(&head_his_list_ptr);
        }
        
        if (PNULL != cur_nav_his_list_ptr)
        {
            FreeNavHisList(&(cur_nav_his_list_ptr->next_ptr));
            cur_nav_his_list_ptr->next_ptr = temp_nav_his_list_ptr; 
            temp_nav_his_list_ptr->prev_ptr = cur_nav_his_list_ptr;
        }
        else
        {
            s_brw_nav_his_list_ptr = temp_nav_his_list_ptr;
        }              
        s_brw_nav_cur_his_ptr = temp_nav_his_list_ptr;
        s_brw_nav_his_item_num++;
        //copy the cathe ptr if cur url is a fragment url of the prevs'
        if(s_brw_nav_cur_his_ptr->prev_ptr != PNULL)
        {
            if(BrwIsUrlIsEquExceptFragment(s_brw_nav_cur_his_ptr->prev_ptr->url_arr, s_brw_nav_cur_his_ptr->url_arr))
            {
                if(s_brw_nav_cur_his_ptr->prev_ptr->wap_request_cache_ptr != PNULL)
                {
                	s_brw_nav_cur_his_ptr->wap_request_cache_ptr = s_brw_nav_cur_his_ptr->prev_ptr->wap_request_cache_ptr;
                }
            }
        }
    }
    return TRUE;
}
/*
 *	desc:get forward history item's url
 */
PUBLIC uint8* BrwGetForwardUrl(void)
{
    if (PNULL == s_brw_nav_cur_his_ptr)
    {
        return PNULL;
    }
    
    if(PNULL != s_brw_nav_cur_his_ptr->next_ptr)
    {
        return s_brw_nav_cur_his_ptr->next_ptr->url_arr;
    }
    
    return PNULL;
}
/*
 *	desc:forward operation
 *  return:the url of previous navigate history item
 */
PUBLIC uint8* BrwGetBackwardUrl(void)
{
    if (PNULL == s_brw_nav_cur_his_ptr)
    {
        return PNULL;
    }	
    
    if(PNULL != s_brw_nav_cur_his_ptr->prev_ptr)
    {
        return s_brw_nav_cur_his_ptr->prev_ptr->url_arr;
    }
    return PNULL;
}
/*
 *	desc:forward operation
 *  return:the url of next navigate history item
 */
PUBLIC uint8*  BrwNavHisForward(void)
{
    if ((PNULL != s_brw_nav_cur_his_ptr) && (PNULL != s_brw_nav_cur_his_ptr->next_ptr))
    {
        //BRWDebug_Printf:"BrwNavHisForward,total num:%d,cur:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_685_112_2_18_2_7_16_666,(uint8*)"ds",s_brw_nav_his_item_num,s_brw_nav_cur_his_ptr->url_arr);
        s_brw_nav_cur_his_ptr = s_brw_nav_cur_his_ptr->next_ptr;
        return s_brw_nav_cur_his_ptr->url_arr;
    }
    return PNULL;
}
/*
 *	desc:backward operation
 *  return:the url of previous navigate history item
 */
PUBLIC uint8*  BrwNavHisBackward(void)
{
    if ((PNULL != s_brw_nav_cur_his_ptr) && (PNULL != s_brw_nav_cur_his_ptr->prev_ptr))
    {
        //BRWDebug_Printf:"BrwNavHisBackward,total num:%d,cur:%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_699_112_2_18_2_7_16_667,(uint8*)"ds",s_brw_nav_his_item_num,s_brw_nav_cur_his_ptr->url_arr);
        NavHisFreeCurCacheReq(s_brw_nav_cur_his_ptr);
        s_brw_nav_cur_his_ptr=s_brw_nav_cur_his_ptr->prev_ptr;
        return s_brw_nav_cur_his_ptr->url_arr;
    }
    return PNULL;
}
/*
 *	desc:get current history item's url
 */
PUBLIC uint8* BrwNavHisGetCurUrl(void)
{
    if(PNULL != s_brw_nav_cur_his_ptr)
    {
        return s_brw_nav_cur_his_ptr->url_arr;
    }
    return PNULL;
}

PUBLIC uint8* BRW_NavHisGetCurUrl(void)
{
	return BrwNavHisGetCurUrl();
}
/*
 *	desc:check if backward enable
 *  return:return TRUE if enable,else FALSE
 */
PUBLIC BOOLEAN BRW_NavHisIsBackwardEnable(void)
{
    if ((PNULL != s_brw_nav_cur_his_ptr) && (PNULL != s_brw_nav_cur_his_ptr->prev_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*
 *	desc:check if forward enable
 *  return:return TRUE if enable,else FALSE
 */
PUBLIC BOOLEAN BRW_NavHisIsForwardEnable(void)
{
    if ((PNULL != s_brw_nav_cur_his_ptr) && (PNULL != s_brw_nav_cur_his_ptr->next_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 * author:fen.xie
 * desc:ѭ���õ�his��ÿ���ǿյ�wap_request_cache,֪��his list�������
 *          ��screen opt���profile�仯ʱ����Ҫ��cache�е�wap������layoutһ��
 *  notice: for MS00195411
 * return:PNULL��ʾloop����;��PNULL:his list�е�һ��wap_request_cache
 */
PUBLIC  WAP_REQUEST_T* BrwNavHisGetPrevLoop(void)
{
    LOCAL BRW_NAV_HIS_LIST_T *s_temp_his_ptr = PNULL;
    WAP_REQUEST_T *wap_request_cache_ptr = PNULL;

    if ((PNULL == s_brw_nav_cur_his_ptr) || (PNULL == s_brw_nav_his_list_ptr))
    {
        s_temp_his_ptr = PNULL;
        return PNULL;
    }

    if (PNULL == s_temp_his_ptr)//first loop,set to s_brw_nav_cur_his_ptr
    {
        s_temp_his_ptr = s_brw_nav_cur_his_ptr;
        wap_request_cache_ptr = s_temp_his_ptr->wap_request_cache_ptr;
    }

    //��Ҫ��his���汣�������wap_request_cathe������һ��,����NULL��
    while ((PNULL == wap_request_cache_ptr) && (s_temp_his_ptr->prev_ptr != PNULL))
    {
        s_temp_his_ptr = s_temp_his_ptr->prev_ptr;
        wap_request_cache_ptr = s_temp_his_ptr->wap_request_cache_ptr;
    }

    if ((PNULL == wap_request_cache_ptr) && (PNULL == s_temp_his_ptr->prev_ptr))
    {
        s_temp_his_ptr = PNULL;//last one,reset for end and stop loop
    }    

    return wap_request_cache_ptr;
}

PUBLIC BOOLEAN BRW_ClearVisitHis(void)
{
    wchar full_path[BRW_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len=0;//length in wchar
    wchar   *device_name_ptr=PNULL;
    uint16 device_name_len=0;
    BOOLEAN result=TRUE;

    device_name_ptr = BRWPL_GetBrwSysDevName(&device_name_len);
    if(PNULL == device_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRWUtil_ReadFileSyn:device null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_799_112_2_18_2_7_16_668,(uint8*)"");
        return FALSE;
    }
    full_path_len = device_name_len + BRW_Strlen((const uint8*)FILE_DEVICE_SLASH)
                + BRW_Strlen((const uint8*)BRW_HISTORY_PATH) + BRW_Strlen((const uint8*)BRW_FILE_SLASH)
                + BRW_Strlen((const uint8*)BRW_HISTORY_FILE_NAME);
                
    if(full_path_len > BRW_FULL_PATH_MAX_LEN)
    {
        BRWDebug_PrintWarning("BRWUtil_ReadFileSyn:full_path_len too long");
        return FALSE;		
    }
    BRW_WstrCat(full_path,device_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, FILE_DEVICE_SLASH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_HISTORY_PATH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_FILE_SLASH, BRW_FULL_PATH_MAX_LEN + 1);
    BRWPL_WstrStrnCatWithAsciiStr(full_path, BRW_HISTORY_FILE_NAME, BRW_FULL_PATH_MAX_LEN + 1);

    result = BRWPL_DelFileSyn(full_path, full_path_len);
    
    //BRWDebug_Printf:"[BRW]BRW_ClearVisitHis result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_HISTORY_819_112_2_18_2_7_16_669,(uint8*)"d", result);

    return TRUE;
}

PUBLIC BOOLEAN BrwIsJumpByBackward(WAP_REQUEST_T *wap_request_ptr)
{
    LAYOUT_HANDLE cur_layout_handle = BRW_INVALID_LAYOUT_HANDLE;
    LAYOUT_HANDLE his_pre_layout_handle = BRW_INVALID_LAYOUT_HANDLE;

    if (PNULL != wap_request_ptr)
    {
        cur_layout_handle = wap_request_ptr->layout_handle;
    }
    
    if ((s_brw_nav_cur_his_ptr != PNULL) && (s_brw_nav_cur_his_ptr->prev_ptr != PNULL)
        &&(s_brw_nav_cur_his_ptr->prev_ptr->wap_request_cache_ptr != PNULL))
    {
        his_pre_layout_handle = s_brw_nav_cur_his_ptr->prev_ptr->wap_request_cache_ptr->layout_handle;
    }

    if ((cur_layout_handle != BRW_INVALID_LAYOUT_HANDLE) && (cur_layout_handle == his_pre_layout_handle))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


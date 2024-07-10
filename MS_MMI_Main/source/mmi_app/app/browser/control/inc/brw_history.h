/*****************************************************************
 **  File Name: 	brw_history.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:header file of history module                   *
 **  note:����޸�BRW_VISIT_HIS_ITEM_T����BRW_VISIT_HIS_INFO_T�� *
 **  ��,�޸�BRW_HISTORY_FILE_NAME�еİ汾��ʶ                  *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_HISTORY_H
#define BRW_HISTORY_H

#include "brw_control_api.h"
#include "brw_control.h"
//001��ʶ�汾��,��BRW_VISIT_HIS_INFO_T��BRW_VISIT_HIS_ITEM_T�޸ĵ�ʱ��,���İ汾��ʶ,�����ļ����ݵ�����
#define BRW_HISTORY_FILE_NAME   "history001.dat"

#if defined(LOW_MEMORY_SUPPORT)  || !defined(NANDBOOT_SUPPORT)
#define BRW_MAX_NAV_HIS_ITEM_NUM  10//������ʷ��¼�������
#else
#define BRW_MAX_NAV_HIS_ITEM_NUM  20//������ʷ��¼�������
#endif

#ifdef CMCC_UI_STYLE
#ifdef BRW_MAX_NAV_HIS_ITEM_NUM
#undef BRW_MAX_NAV_HIS_ITEM_NUM
#endif
#define BRW_MAX_NAV_HIS_ITEM_NUM    20
#endif

typedef struct tagBRW_VISIT_HIS_ITEM_T
{
    BRW_VISIT_HIS_INFO_T  visit_his_info;
    BOOLEAN is_used;
}BRW_VISIT_HIS_ITEM_T;
typedef struct tagBRW_VISIT_HIS_QUE_T
{
    BRW_VISIT_HIS_ITEM_T his_item_que[BRW_MAX_VISIT_HIS_ITEM_NUM];
}BRW_VISIT_HIS_QUE_T;
/*
 *	desc:add an visit history item
 *  param:url_ptr  url of item
 *        name_ptr  name of item,in unicode format
 *        name_len  length of name
 *  return: TRUE if sucess
 */
PUBLIC BOOLEAN BrwVisitHisAddItem(uint8* url_ptr,uint16* name_ptr,uint16 name_len);
/*
 *	desc:init navigate history
 */
PUBLIC void BrwNavHisInit(void);
/*
 *	desc:exit navigate history
 */
PUBLIC void BrwNavHisExit(void);
/*
 *	desc:clean navigate history
 */
PUBLIC void BrwNavHisClean(void);
/*
 *	desc:add navigate history item
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwNavHisAddItem(const uint8* url_ptr);
/*
 *	desc:get forward history item's url
 */
PUBLIC uint8* BrwGetForwardUrl(void);
/*
 *	desc:get backward's history item's url
 */
PUBLIC uint8* BrwGetBackwardUrl(void);
/*
 *	desc:forward operation
 *  return:the url of next navigate history item
 */
PUBLIC uint8*  BrwNavHisForward(void);
/*
 *	desc:backward operation
 *  return:the url of previous navigate history item
 */
PUBLIC uint8*  BrwNavHisBackward(void);
/*
 *	desc:get current history item's url
 */
PUBLIC uint8* BrwNavHisGetCurUrl(void);

PUBLIC void BrwNavHisAddCache(WAP_REQUEST_T *to_be_cached_req_ptr);
PUBLIC WAP_REQUEST_T* BrwNavHisGetBackwardCacheReq(void);
PUBLIC void BrwNavHisFreeCurCacheReq(void);
PUBLIC BOOLEAN BrwNavClearCache(void);

/*
 * author:fen.xie
 * desc:ѭ���õ�his��ÿ���ǿյ�wap_request_cache,֪��his list�������
 *          ��screen opt���profile�仯ʱ����Ҫ��cache�е�wap������layoutһ��
 *  notice: for MS00195411
 * return:PNULL��ʾloop����;��PNULL:his list�е�һ��wap_request_cache
 */
PUBLIC  WAP_REQUEST_T* BrwNavHisGetPrevLoop(void);

PUBLIC BOOLEAN BrwIsJumpByBackward(WAP_REQUEST_T *wap_request_ptr);


#endif

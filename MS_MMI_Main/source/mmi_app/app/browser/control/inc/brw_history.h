/*****************************************************************
 **  File Name: 	brw_history.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:header file of history module                   *
 **  note:如果修改BRW_VISIT_HIS_ITEM_T或者BRW_VISIT_HIS_INFO_T结 *
 **  构,修改BRW_HISTORY_FILE_NAME中的版本标识                  *
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
//001标识版本号,当BRW_VISIT_HIS_INFO_T或BRW_VISIT_HIS_ITEM_T修改的时候,更改版本标识,避免文件兼容的问题
#define BRW_HISTORY_FILE_NAME   "history001.dat"

#if defined(LOW_MEMORY_SUPPORT)  || !defined(NANDBOOT_SUPPORT)
#define BRW_MAX_NAV_HIS_ITEM_NUM  10//导航历史记录最大条数
#else
#define BRW_MAX_NAV_HIS_ITEM_NUM  20//导航历史记录最大条数
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
 * desc:循环得到his的每个非空的wap_request_cache,知道his list遍历完毕
 *          供screen opt这个profile变化时，需要将cache中的wap都重新layout一遍
 *  notice: for MS00195411
 * return:PNULL表示loop结束;非PNULL:his list中的一个wap_request_cache
 */
PUBLIC  WAP_REQUEST_T* BrwNavHisGetPrevLoop(void);

PUBLIC BOOLEAN BrwIsJumpByBackward(WAP_REQUEST_T *wap_request_ptr);


#endif

/*****************************************************************************
** File Name:      mmischedule_atc.c                                                
** Author:         liqing.peng   
** Date:           2006/07/11
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to implement schedule function 
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/07/11     liqing.peng      Create                                    *
******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         include files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MMI_SYNCTOOL_V3
#include "mmischedule_atc.h"
#include "window_parse.h"
//#include "sci_types.h"
//#include "ffs.h"
#include "mmialarm_export.h"
#include "mmischedule_export.h"
#include "mmialarm_image.h"



/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CopyWStrToInternalStr(
                                 uint16       *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint16      out_max_len,        //IN:
                                 BOOLEAN     is_ucs2,            //IN:
                                 uint16       in_len,             //IN:
                                 wchar       *in_alpha_ptr       //IN:
                                 );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:  xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncHeadInfo( 
							SCHEDULE_HEAD_INFO_T *schedule_head_info_ptr  // OUT
							)
{
    uint16                      i = 0;
    uint16                      total = 0;
    uint16   remainder  = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr = NULL;

    //SCI_ASSERT(NULL != memo_head_info_ptr);/*assert verified*/
    if(PNULL == schedule_head_info_ptr)
    {
        return FALSE;
    }


    schedule_head_info_ptr->max_count= MMIACC_SCH_NUM;
    schedule_head_info_ptr->title_max_len = MMIACC_EVENT_TEXT_MAX_LEN;
    
    for(i=0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0+i));
        if(event_ptr->is_valid)
        {//ID flag,id is 1 is valid,is 0 not exist
            remainder = i%8;
            schedule_head_info_ptr->all_entry_id[total/8] |= (1<<(7-remainder));
            total++;
        }
    }


    return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncAdd( 
						SCHEDULE_ENTRY_T    *schedule_info_ptr, // IN
						uint16 *index_ptr
						)
{
    MMIACC_SMART_EVENT_T    event = {0};
    //SCI_DATE_T              cur_date = {0};
    //SCI_TIME_T              cur_time = {0};
    //SCI_DATE_T              second_date = {0};
    //SCI_TIME_T              second_time = {0};
    BOOLEAN                 result = FALSE;

    if(PNULL == schedule_info_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    //MMIAPIALM_FormInteEventInfo(*index_ptr,&event);
	//MMIAPIALM_GetRingInfo(*index_ptr,&event);

    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SCH;

    if(ACC_ALARM_NULL== schedule_info_ptr->alarm_mode)
    {
        event.event_fast_info.is_on = FALSE;
    }
    else
    {
        event.event_fast_info.is_on = TRUE;
        switch(schedule_info_ptr->alarm_mode)
        {
        case ACC_ALARM_ONCE:
           
            event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;

        case ACC_ALARM_EVERYDAY:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;

        case ACC_ALARM_EVERYWEEK:
            //TM_GetSysDate(&cur_date);
            //event.event_fast_info.fre |= s_week_mask[cur_date.wday];
            event.event_fast_info.fre=schedule_info_ptr->frequece;
            event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            break;

        case ACC_ALARM_EVERYMONTH:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
            break;

        case ACC_ALARM_EVERYYEAR:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMISCHEDULE_SyncAdd: schedule_info_ptr->alarm_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_160_112_2_18_2_2_17_98,(uint8*)"d",schedule_info_ptr->alarm_mode);
            break;
        }
    }
    event.event_fast_info.year= schedule_info_ptr->start_time.year;
    event.event_fast_info.mon= schedule_info_ptr->start_time.month;
    event.event_fast_info.day= schedule_info_ptr->start_time.day;
    event.event_fast_info.hour= schedule_info_ptr->start_time.hour;
    event.event_fast_info.minute= schedule_info_ptr->start_time.minute;

    event.event_fast_info.start_year= schedule_info_ptr->start_time.year;
    event.event_fast_info.start_mon= schedule_info_ptr->start_time.month;
    event.event_fast_info.start_day= schedule_info_ptr->start_time.day;

    event.event_fast_info.end_year= schedule_info_ptr->end_time.year;
    event.event_fast_info.end_mon= schedule_info_ptr->end_time.month;
    event.event_fast_info.end_day= schedule_info_ptr->end_time.day;

    event.event_fast_info.fre = schedule_info_ptr->frequece;

    MMIAPICOM_Wstrcpy(event.event_content_info.w_content,schedule_info_ptr->title);
    event.event_content_info.w_length = schedule_info_ptr->title_len;
    
    event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    event.event_content_info.ring_type = ALM_FIX_RING;
    event.event_content_info.ring = 1;

    if (MMIALM_AddOneEvent(&event,index_ptr))
    {
        (*index_ptr) = (*index_ptr)-EVENT_SCH_0;
        result = TRUE;
    }
    else
    {
         result = FALSE;
    }

    return result;
}


/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:     xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncDelete( 
							uint16           entry_id  // IN
							)
{
    const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = NULL;

    if(MMIACC_SCH_NUM <= entry_id)
    {
        return FALSE;
    }
    
    event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0+entry_id));
    if(!event_ptr->is_valid)
    {
        return FALSE;
    }

    MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + entry_id));

    return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:  xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncModify( 
							uint16           entry_id,    //IN:
							SCHEDULE_ENTRY_T *  schedule_info_ptr  // IN 
							)
{
    MMIACC_SMART_EVENT_T    event = {0};
    //SCI_DATE_T              cur_date = {0};
    //SCI_TIME_T              cur_time = {0};
    //SCI_DATE_T              second_date = {0};
    //SCI_TIME_T              second_time = {0};
    BOOLEAN                 result = FALSE;
    //SCI_ASSERT(NULL != memo_item_info_ptr);/*assert verified*/
    if(MMIACC_SCH_NUM <= entry_id || PNULL == schedule_info_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    MMIAPIALM_FormInteEventInfo(entry_id+EVENT_SCH_0,&event);
	MMIAPIALM_GetRingInfo(entry_id+EVENT_SCH_0,&event);

    if(!(MMIALM_GeEvent((uint16)(EVENT_SCH_0 + entry_id)))->is_valid)
    {
        return FALSE;
    }
    
    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SCH;

	SCI_MEMSET(event.event_content_info.w_content,0,sizeof(event.event_content_info.w_content));
    MMIAPICOM_Wstrcpy(event.event_content_info.w_content,schedule_info_ptr->title);
    event.event_content_info.w_length = schedule_info_ptr->title_len;
        
    if(ACC_ALARM_NULL== schedule_info_ptr->alarm_mode)
    {
        event.event_fast_info.is_on = FALSE;
    }
    else
    {
        event.event_fast_info.is_on = TRUE;
        switch(schedule_info_ptr->alarm_mode)
        {
        case ACC_ALARM_ONCE:
           
            event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;

        case ACC_ALARM_EVERYDAY:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;

        case ACC_ALARM_EVERYWEEK:
            //TM_GetSysDate(&cur_date);
            //event.event_fast_info.fre |= s_week_mask[cur_date.wday];
            event.event_fast_info.fre=schedule_info_ptr->frequece;
            event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            break;

        case ACC_ALARM_EVERYMONTH:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
            break;

        case ACC_ALARM_EVERYYEAR:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMISCHEDULE_SyncAdd: schedule_info_ptr->alarm_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_160_112_2_18_2_2_17_98,(uint8*)"d",schedule_info_ptr->alarm_mode);
            break;
        }
    }
    
    event.event_fast_info.start_year= schedule_info_ptr->start_time.year;
    event.event_fast_info.start_mon= schedule_info_ptr->start_time.month;
    event.event_fast_info.start_day= schedule_info_ptr->start_time.day;
    event.event_fast_info.hour= schedule_info_ptr->start_time.hour;
    event.event_fast_info.minute= schedule_info_ptr->start_time.minute;

    event.event_fast_info.end_year= schedule_info_ptr->end_time.year;
    event.event_fast_info.end_mon= schedule_info_ptr->end_time.month;
    event.event_fast_info.end_day= schedule_info_ptr->end_time.day;

    event.event_fast_info.fre = schedule_info_ptr->frequece;
    
    event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    event.event_content_info.ring_type = ALM_FIX_RING;
    event.event_content_info.ring = 1;

    if(MMIALM_ModifyEvent((uint16)(EVENT_SCH_0 + entry_id), (MMIACC_SMART_EVENT_FAST_TABLE_T*)&event.event_fast_info)&&
		MMIALM_ModifyEventName((uint16)(EVENT_SCH_0 + entry_id),(MMIACC_SMART_EVENT_CONTENT_T*)&event.event_content_info))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
// 	Description : This function is provided by schedule, used by MMI AT
//	Global resource dependence : none
//  Author:   xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMISCHEDULE_SyncRead( 
						uint16           entry_id,    //IN:
						SCHEDULE_ENTRY_T *   schedule_info_ptr  //OUT
						)
{
   BOOLEAN result = FALSE;
    MMIACC_SMART_EVENT_T    event = {0};

    //SCI_PASSERT(MMIACC_SCH_NUM > entry_id, ("MMISCHEDULE_SyncRead: entry_id is %d",entry_id));	/*assert verified*/
    //SCI_ASSERT(NULL != memo_item_info_ptr);/*assert verified*/
    if(MMIACC_SCH_NUM <= entry_id || PNULL == schedule_info_ptr)
    {
        return FALSE;
    }
   
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    MMIAPIALM_FormInteEventInfo(entry_id+EVENT_SCH_0,&event);
	MMIAPIALM_GetRingInfo(entry_id+EVENT_SCH_0,&event);

    if(!event.event_fast_info.is_valid)
    {
        return result;
    }

    schedule_info_ptr->entry_id = entry_id;
    MMIAPICOM_Wstrcpy(schedule_info_ptr->title,event.event_content_info.w_content);
    schedule_info_ptr->title_len = event.event_content_info.w_length;
        
    schedule_info_ptr->start_time.year = event.event_fast_info.start_year;
    schedule_info_ptr->start_time.month = event.event_fast_info.start_mon;
    schedule_info_ptr->start_time.day = event.event_fast_info.start_day;
    schedule_info_ptr->start_time.hour = event.event_fast_info.hour;
    schedule_info_ptr->start_time.minute = event.event_fast_info.minute;

    schedule_info_ptr->end_time.year = event.event_fast_info.end_year;
    schedule_info_ptr->end_time.month = event.event_fast_info.end_mon;
    schedule_info_ptr->end_time.day = event.event_fast_info.end_day;
    schedule_info_ptr->end_time.hour = event.event_fast_info.hour;
    schedule_info_ptr->end_time.minute = event.event_fast_info.minute;
    
    schedule_info_ptr->frequece = event.event_fast_info.fre;
    result = TRUE;
    if(event.event_fast_info.is_on)
    {
        switch(event.event_fast_info.fre_mode)
        {
        case ALM_MODE_ONCE:
            schedule_info_ptr->alarm_mode = ACC_ALARM_ONCE;
            break;

        case ALM_MODE_EVERYDAY:
            schedule_info_ptr->alarm_mode = ACC_ALARM_EVERYDAY;
            break;

        case ALM_MODE_EVERYWEEK:
            schedule_info_ptr->alarm_mode = ACC_ALARM_EVERYWEEK;
            break;

        case ALM_MODE_EVERYMONTH:
            schedule_info_ptr->alarm_mode = ACC_ALARM_EVERYMONTH;
            break;

        case ALM_MODE_EVERYYEAR:
            schedule_info_ptr->alarm_mode = ACC_ALARM_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMIMEMO_SyncRead: event_ptr->fre_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_447_112_2_18_2_2_18_100,(uint8*)"d",event.event_fast_info.fre_mode);
            break;
        }
    }
    else
    {
        schedule_info_ptr->alarm_mode = ACC_ALARM_NULL;
    }
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CopyWStrToInternalStr(
                                 uint16       *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint16      out_max_len,        //IN:
                                 BOOLEAN     is_ucs2,            //IN:
                                 uint16       in_len,             //IN:
                                 wchar       *in_alpha_ptr       //IN:
                                 )
{
    
    if(PNULL == out_len_ptr || PNULL == out_alpha_ptr)
    {
        return;
    }
    
    SCI_MEMSET( out_alpha_ptr,
        0X00,
        out_max_len
        );
    if (0 < in_len && (PNULL != in_alpha_ptr))
    {
        if (is_ucs2)
        {
            *out_len_ptr = MIN((in_len*sizeof(wchar)), out_max_len);
            SCI_MEMCPY(out_alpha_ptr, (uint8 *)in_alpha_ptr, *out_len_ptr);
            
            
        }
        else
        {
            *out_len_ptr = MIN(in_len, out_max_len);
            MMI_WSTRNTOSTR(out_alpha_ptr, out_max_len, in_alpha_ptr, out_max_len, *out_len_ptr);
        }
    }
    else
    {
        *out_len_ptr = 0;
    }
}
#endif //#ifdef MMI_SYNCTOOL_SUPPORT


#ifdef MMI_SYNCTOOL_SUPPORT
#include "mmi_atc.h"
#include "window_parse.h"
//#include "sci_types.h"
//#include "ffs.h"
#include "mmialarm_export.h"
#include "mmischedule_export.h"
#include "mmialarm_image.h"



/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CopyWStrToInternalStr(
                                 uint16       *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint16      out_max_len,        //IN:
                                 BOOLEAN     is_ucs2,            //IN:
                                 uint16       in_len,             //IN:
                                 wchar       *in_alpha_ptr       //IN:
                                 );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncHeadInfo( 
							MEMO_HEAD_INFO_T    *memo_head_info_ptr  // OUT
							)
{
    uint16                      i = 0;
    uint16                      total = 0;
    const MMIACC_SMART_EVENT_FAST_TABLE_T  *event_ptr = NULL;

    //SCI_ASSERT(NULL != memo_head_info_ptr);/*assert verified*/
    if(PNULL == memo_head_info_ptr)
    {
        return FALSE;
    }

    memo_head_info_ptr->max_text_len = MMIACC_EVENT_TEXT_MAX_LEN;
    memo_head_info_ptr->max_record_num = MMIACC_SCH_NUM;

    for(i=0; i<MMIACC_SCH_NUM; i++)
    {
        event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0+i));
        if(event_ptr->is_valid)
        {
            memo_head_info_ptr->all_entry_id[total] = i;
            total++;
        }
    }

    memo_head_info_ptr->used_record_num = total;

    return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncAdd( 
						MEMO_ENTRY_T    *memo_item_info_ptr,  // IN
                        uint16          *index_ptr
						)
{
    uint16                  text_len = 0;
    MMIACC_SMART_EVENT_T    event = {0};
    //SCI_DATE_T              cur_date = {0};
    //SCI_TIME_T              cur_time = {0};
    //SCI_DATE_T              second_date = {0};
    //SCI_TIME_T              second_time = {0};
    BOOLEAN                 result = FALSE;
    //SCI_ASSERT(NULL != memo_item_info_ptr);/*assert verified*/
    //SCI_ASSERT(NULL != index_ptr);/*assert verified*/
    if(PNULL == memo_item_info_ptr || PNULL == index_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    //MMIAPIALM_FormInteEventInfo(*index_ptr,&event);
	//MMIAPIALM_GetRingInfo(*index_ptr,&event);

    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SCH;
    event.event_fast_info.service_id=memo_item_info_ptr->service_id;

    if(MEMO_MODE_NO_ALARM == memo_item_info_ptr->memo_mode)
    {
        event.event_fast_info.is_on = FALSE;
    }
    else
    {
        event.event_fast_info.is_on = TRUE;
        switch(memo_item_info_ptr->memo_mode)
        {
        case MEMO_MODE_ONCE:
            /*TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            second_date.year = memo_item_info_ptr->memo_time.year;
            second_date.mon  = memo_item_info_ptr->memo_time.month;
            second_date.mday = memo_item_info_ptr->memo_time.day;
            second_time.hour = memo_item_info_ptr->memo_time.hour;
            second_time.min = memo_item_info_ptr->memo_time.minute;
            second_time.sec = 0;
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) != CMP_FIRST_SMALL)
            {
                return FALSE;
            }*/
            event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;

        case MEMO_MODE_EVERYDAY:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;

        case MEMO_MODE_EVERYWEEK:
            //TM_GetSysDate(&cur_date);
            //event.event_fast_info.fre |= s_week_mask[cur_date.wday];
            event.event_fast_info.fre=memo_item_info_ptr->fre;
            event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            break;

        case MEMO_MODE_EVERYMONTH:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
            break;

        case MEMO_MODE_EVERYYEAR:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMIMEMO_SyncAdd: memo_item_info_ptr->memo_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_160_112_2_18_2_2_17_98,(uint8*)"d",memo_item_info_ptr->memo_mode);
            break;
        }
    }

    event.event_fast_info.year = memo_item_info_ptr->memo_time.year;
    event.event_fast_info.mon = memo_item_info_ptr->memo_time.month;
    event.event_fast_info.day = memo_item_info_ptr->memo_time.day;
    event.event_fast_info.hour = memo_item_info_ptr->memo_time.hour;
    event.event_fast_info.minute = memo_item_info_ptr->memo_time.minute;

    event.event_fast_info.start_year= memo_item_info_ptr->memo_start_time.year;
    event.event_fast_info.start_mon= memo_item_info_ptr->memo_start_time.month;
    event.event_fast_info.start_day= memo_item_info_ptr->memo_start_time.day;

    event.event_fast_info.end_year= memo_item_info_ptr->memo_end_time.year;
    event.event_fast_info.end_mon= memo_item_info_ptr->memo_end_time.month;
    event.event_fast_info.end_day= memo_item_info_ptr->memo_end_time.day;
 

    if(memo_item_info_ptr->memo_text.is_ucs2)
    {
        text_len=MIN(MMIACC_EVENT_TEXT_MAX_LEN*sizeof(wchar),memo_item_info_ptr->memo_text.text_len);
        SCI_MEMCPY((uint8*)event.event_content_info.w_content,memo_item_info_ptr->memo_text.text,text_len);
        event.event_content_info.w_length = text_len/2;
    }
    else
    {
        text_len = memo_item_info_ptr->memo_text.text_len;
        event.event_content_info.w_length = text_len;
        MMI_STRNTOWSTR(event.event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN, 
                   memo_item_info_ptr->memo_text.text, text_len, text_len);
   }

    event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    event.event_content_info.ring_type = ALM_FIX_RING;
    event.event_content_info.ring = 1;

    if (MMIALM_AddOneEvent(&event,index_ptr))
    {
        (*index_ptr) = (*index_ptr)-EVENT_SCH_0;
        result = TRUE;
    }
    else
    {
         result = FALSE;
    }

    return result;
}


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncDelete( 
							uint16           entry_id  // IN
							)
{
    const MMIACC_SMART_EVENT_FAST_TABLE_T *event_ptr = NULL;

    //SCI_PASSERT(MMIACC_SCH_NUM > entry_id, ("MMIMEMO_SyncDelete: entry_id is %d",entry_id));/*assert verified*/		
    if(MMIACC_SCH_NUM <= entry_id)
    {
        return FALSE;
    }
    
    event_ptr = MMIALM_GeEvent((uint16)(EVENT_SCH_0+entry_id));
    if(!event_ptr->is_valid)
    {
        return FALSE;
    }

    return MMIALM_DelOneEvent((uint16)(EVENT_SCH_0 + entry_id)); //coverity 10016

    //return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncModify( 
							uint16           entry_id,           //IN
							MEMO_ENTRY_T *   memo_item_info_ptr  //IN 
							)
{
    uint16                  text_len = 0;
    MMIACC_SMART_EVENT_T    event = {0};
    //SCI_DATE_T              cur_date = {0};
    //SCI_TIME_T              cur_time = {0};
    //SCI_DATE_T              second_date = {0};
    //SCI_TIME_T              second_time = {0};
    BOOLEAN                 result = FALSE;
    //SCI_PASSERT(MMIACC_SCH_NUM > entry_id, ("MMIMEMO_SyncModify: entry_id is %d",entry_id));/*assert verified*/	
    //SCI_ASSERT(NULL != memo_item_info_ptr);/*assert verified*/
    if(MMIACC_SCH_NUM <= entry_id || PNULL == memo_item_info_ptr)
    {
        return FALSE;
    }
    
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    MMIAPIALM_FormInteEventInfo(entry_id+EVENT_SCH_0,&event);
	MMIAPIALM_GetRingInfo(entry_id+EVENT_SCH_0,&event);

    if(!(MMIALM_GeEvent((uint16)(EVENT_SCH_0 + entry_id)))->is_valid)
    {
        return FALSE;
    }

    event.event_fast_info.is_valid = TRUE;
    event.event_fast_info.type = SMART_EVENT_SCH;
    event.event_fast_info.service_id=memo_item_info_ptr->service_id;

    if(MEMO_MODE_NO_ALARM == memo_item_info_ptr->memo_mode)
    {
        event.event_fast_info.is_on = FALSE;
    }
    else
    {
        event.event_fast_info.is_on = TRUE;
        switch(memo_item_info_ptr->memo_mode)
        {
        case MEMO_MODE_ONCE:
            /*TM_GetSysDate(&cur_date);
            TM_GetSysTime(&cur_time);
            second_date.year = memo_item_info_ptr->memo_time.year;
            second_date.mon  = memo_item_info_ptr->memo_time.month;
            second_date.mday = memo_item_info_ptr->memo_time.day;
            second_time.hour = memo_item_info_ptr->memo_time.hour;
            second_time.min = memo_item_info_ptr->memo_time.minute;
            second_time.sec = 0;
            if (MMIAPIALM_CompareDateAndTime(cur_date, cur_time, second_date, second_time) != CMP_FIRST_SMALL)
            {
                return FALSE;
            }*/
            event.event_fast_info.fre_mode = ALM_MODE_ONCE;
            break;

        case MEMO_MODE_EVERYDAY:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYDAY;
            break;

        case MEMO_MODE_EVERYWEEK:
            //TM_GetSysDate(&cur_date);
            //event.event_fast_info.fre |= s_week_mask[cur_date.wday];
            event.event_fast_info.fre=memo_item_info_ptr->fre;
            event.event_fast_info.fre_mode = ALM_MODE_EVERYWEEK;
            break;

        case MEMO_MODE_EVERYMONTH:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYMONTH;
            break;

        case MEMO_MODE_EVERYYEAR:
            event.event_fast_info.fre_mode = ALM_MODE_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMIMEMO_SyncModify: memo_item_info_ptr->memo_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_321_112_2_18_2_2_18_99,(uint8*)"d",memo_item_info_ptr->memo_mode);
            break;
        }
    }

    event.event_fast_info.year = memo_item_info_ptr->memo_time.year;
    event.event_fast_info.mon = memo_item_info_ptr->memo_time.month;
    event.event_fast_info.day = memo_item_info_ptr->memo_time.day;
    event.event_fast_info.hour = memo_item_info_ptr->memo_time.hour;
    event.event_fast_info.minute = memo_item_info_ptr->memo_time.minute;

    event.event_fast_info.start_year= memo_item_info_ptr->memo_start_time.year;
    event.event_fast_info.start_mon= memo_item_info_ptr->memo_start_time.month;
    event.event_fast_info.start_day= memo_item_info_ptr->memo_start_time.day;

    event.event_fast_info.end_year= memo_item_info_ptr->memo_end_time.year;
    event.event_fast_info.end_mon= memo_item_info_ptr->memo_end_time.month;
    event.event_fast_info.end_day= memo_item_info_ptr->memo_end_time.day;

    if(memo_item_info_ptr->memo_text.is_ucs2)
    {
        text_len=MIN(MMIACC_EVENT_TEXT_MAX_LEN*sizeof(wchar),memo_item_info_ptr->memo_text.text_len);
        SCI_MEMCPY((uint8*)event.event_content_info.w_content,memo_item_info_ptr->memo_text.text,text_len);
        event.event_content_info.w_length = text_len/2;
    }
    else
    {
        text_len = memo_item_info_ptr->memo_text.text_len;
        event.event_content_info.w_length = text_len;
        MMI_STRNTOWSTR(event.event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN, 
                   memo_item_info_ptr->memo_text.text, text_len, text_len);
   }   

    event.event_content_info.image_id = IMAGE_SCHEDULE_ARRANGE_01;
    event.event_content_info.ring_type = ALM_FIX_RING;
    event.event_content_info.ring = 1;

    if(MMIALM_ModifyEvent((uint16)(EVENT_SCH_0 + entry_id), (MMIACC_SMART_EVENT_FAST_TABLE_T*)&event))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}


/*****************************************************************************/
// 	Description : This function is provided by memo, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIMEMO_SyncRead( 
						uint16           entry_id,           //IN
						MEMO_ENTRY_T    *memo_item_info_ptr  //OUT
						)
{
    //uint16                      text_len = 0;
    MMIACC_SMART_EVENT_T    event = {0};

    //SCI_PASSERT(MMIACC_SCH_NUM > entry_id, ("MMIMEMO_SyncRead: entry_id is %d",entry_id));	/*assert verified*/
    //SCI_ASSERT(NULL != memo_item_info_ptr);/*assert verified*/
    if(MMIACC_SCH_NUM <= entry_id || PNULL == memo_item_info_ptr)
    {
        return FALSE;
    }
   
    SCI_MEMSET(&event, 0, sizeof(MMIACC_SMART_EVENT_T));
    MMIAPIALM_FormInteEventInfo(entry_id+EVENT_SCH_0,&event);
	MMIAPIALM_GetRingInfo(entry_id+EVENT_SCH_0,&event);

    if(!event.event_fast_info.is_valid)
    {
        return FALSE;
    }

    memo_item_info_ptr->entry_id = entry_id;
    memo_item_info_ptr->service_id=event.event_fast_info.service_id;

    memo_item_info_ptr->memo_time.year = event.event_fast_info.year;
    memo_item_info_ptr->memo_time.month = event.event_fast_info.mon;
    memo_item_info_ptr->memo_time.day = event.event_fast_info.day;
    memo_item_info_ptr->memo_time.hour = event.event_fast_info.hour;
    memo_item_info_ptr->memo_time.minute = event.event_fast_info.minute;

    memo_item_info_ptr->memo_start_time.year = event.event_fast_info.start_year;
    memo_item_info_ptr->memo_start_time.month = event.event_fast_info.start_mon;
    memo_item_info_ptr->memo_start_time.day = event.event_fast_info.start_day;
    memo_item_info_ptr->memo_start_time.hour = event.event_fast_info.hour;
    memo_item_info_ptr->memo_start_time.minute = event.event_fast_info.minute;

    memo_item_info_ptr->memo_end_time.year = event.event_fast_info.end_year;
    memo_item_info_ptr->memo_end_time.month = event.event_fast_info.end_mon;
    memo_item_info_ptr->memo_end_time.day = event.event_fast_info.end_day;
    memo_item_info_ptr->memo_end_time.hour = event.event_fast_info.hour;
    memo_item_info_ptr->memo_end_time.minute = event.event_fast_info.minute;
    
    memo_item_info_ptr->fre=event.event_fast_info.fre;
    
    if(event.event_fast_info.is_on)
    {
        switch(event.event_fast_info.fre_mode)
        {
        case ALM_MODE_ONCE:
            memo_item_info_ptr->memo_mode = MEMO_MODE_ONCE;
            break;

        case ALM_MODE_EVERYDAY:
            memo_item_info_ptr->memo_mode = MEMO_MODE_EVERYDAY;
            break;

        case ALM_MODE_EVERYWEEK:
            memo_item_info_ptr->memo_mode = MEMO_MODE_EVERYWEEK;
            break;

        case ALM_MODE_EVERYMONTH:
            memo_item_info_ptr->memo_mode = MEMO_MODE_EVERYMONTH;
            break;

        case ALM_MODE_EVERYYEAR:
            memo_item_info_ptr->memo_mode = MEMO_MODE_EVERYYEAR;
            break;

        default:
            //SCI_TRACE_LOW:"MMIMEMO_SyncRead: event_ptr->fre_mode is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISCHEDULE_ATC_447_112_2_18_2_2_18_100,(uint8*)"d",event.event_fast_info.fre_mode);
            break;
        }
    }
    else
    {
        memo_item_info_ptr->memo_mode = MEMO_MODE_NO_ALARM;
    }

    memo_item_info_ptr->memo_text.is_ucs2= !MMIAPICOM_IsASCIIString(event.event_content_info.w_content, event.event_content_info.w_length);
    /*text_len = event.event_content_info.w_length;
    memo_item_info_ptr->memo_text.text_len = text_len;
    MMI_WSTRNTOSTR(memo_item_info_ptr->memo_text.text, MEMO_TEXT_MAX_LEN,
                event.event_content_info.w_content, MMIACC_EVENT_TEXT_MAX_LEN, text_len);*/
    CopyWStrToInternalStr(&memo_item_info_ptr->memo_text.text_len,
                             memo_item_info_ptr->memo_text.text,
                             MEMO_TEXT_MAX_LEN,
                             memo_item_info_ptr->memo_text.is_ucs2,
                             event.event_content_info.w_length,
                             event.event_content_info.w_content);

    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CopyWStrToInternalStr(
                                 uint16       *out_len_ptr,       //OUT:
                                 uint8       *out_alpha_ptr,     //OUT:
                                 uint16      out_max_len,        //IN:
                                 BOOLEAN     is_ucs2,            //IN:
                                 uint16       in_len,             //IN:
                                 wchar       *in_alpha_ptr       //IN:
                                 )
{
    
    if(PNULL == out_len_ptr || PNULL == out_alpha_ptr)
    {
        return;
    }
    
    SCI_MEMSET( out_alpha_ptr,
        0X00,
        out_max_len
        );
    if (0 < in_len && (PNULL != in_alpha_ptr))
    {
        if (is_ucs2)
        {
            *out_len_ptr = MIN((in_len*sizeof(wchar)), out_max_len);
            SCI_MEMCPY(out_alpha_ptr, (uint8 *)in_alpha_ptr, *out_len_ptr);
            
            
        }
        else
        {
            *out_len_ptr = MIN(in_len, out_max_len);
            MMI_WSTRNTOSTR(out_alpha_ptr, out_max_len, in_alpha_ptr, out_max_len, *out_len_ptr);
        }
    }
    else
    {
        *out_len_ptr = 0;
    }
}
#endif //#ifdef MMI_SYNCTOOL_SUPPORT





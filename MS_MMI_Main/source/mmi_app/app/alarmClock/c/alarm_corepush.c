

#include "sci_types.h"
#include "mmiacc_event.h"


void alarm_set_for_corepush(char *alarm_str, unsigned char count)
{
    /* Bit 0: Sunday, bit 1: Monday ... bit 6: Saturday */
    unsigned char repeat = 0;
    /* Bit 0: Monday... bit 6: Sunday */
    unsigned char corepush_repeat = 0;
    SCI_DATE_T  dateValue = { 0 };
    uint8 i = 0;
    BOOLEAN save_result = FALSE;

    MMIACC_SMART_EVENT_T*   pEventTable = PNULL;
    pEventTable = (MMIACC_SMART_EVENT_T*)SCI_ALLOC_APPZ( sizeof(MMIACC_SMART_EVENT_T) );
    Trace_Readboy_Log("corepush: %s\n", __FUNCTION__);  
    if( PNULL == pEventTable )
    {
        return ;
    }
     // date && time
    TM_GetSysDate( &dateValue );

    pEventTable->event_fast_info.start_year  = dateValue.year;
    pEventTable->event_fast_info.start_mon   = dateValue.mon;
    pEventTable->event_fast_info.start_day   = dateValue.mday;

    pEventTable->event_fast_info.year        = dateValue.year;
    pEventTable->event_fast_info.mon         = dateValue.mon;
    pEventTable->event_fast_info.day         = dateValue.mday;
    
    for( i= 0; i < count; i++) 
    {
        pEventTable->event_fast_info.is_valid    = TRUE;
        alarm_str++;
        pEventTable->event_fast_info.is_on = *alarm_str;
        alarm_str++;
        pEventTable->event_fast_info.hour = *alarm_str;
        alarm_str++;
        pEventTable->event_fast_info.minute = *alarm_str;
        alarm_str++;
        corepush_repeat = *alarm_str;
        if(corepush_repeat > 0)
        {
            pEventTable->event_fast_info.fre_mode   = ALM_MODE_EVERYWEEK;
            pEventTable->event_fast_info.fre = corepush_repeat;
        }
        else
        {
            pEventTable->event_fast_info.fre_mode   = ALM_MODE_ONCE;
            pEventTable->event_fast_info.fre        = 0;
        }
        alarm_str++;
	 //pEventTable->event_content_info.is_add = TRUE;
	 pEventTable->event_content_info.ring_type = ALM_FIX_RING;
	 pEventTable->event_content_info.ring = 1;

        save_result = ALARMCLOCK_SaveInteEventInfo(i, pEventTable);
	 if(save_result){
		MMIALM_RemoveIdlePushMsg(i, SMART_EVENT_ALARM);
	 }else{
		Trace_Readboy_Log("corepush: alarm set alarm_set_for_corepush save alarm[%d] error!!", i);
	 }
        Trace_Readboy_Log("corepush: alarm set alarm_set_for_corepush valid=%d on_off=%d hour=%d repeat_bitmap=%d\n", pEventTable->event_fast_info.is_valid,  pEventTable->event_fast_info.is_on,  pEventTable->event_fast_info.hour,  pEventTable->event_fast_info.fre);  
    }

    for( i= count; i < ALM_ALARM_NUM; i++) 
    {
        pEventTable->event_fast_info.is_valid    = FALSE;
        pEventTable->event_fast_info.is_on = 0;
        pEventTable->event_fast_info.hour = 0;
        pEventTable->event_fast_info.minute = 0;
        pEventTable->event_fast_info.fre_mode   = ALM_MODE_ONCE;
        pEventTable->event_fast_info.fre        = 0;
	 //pEventTable->event_content_info.is_add = FALSE;
	 pEventTable->event_content_info.ring_type = ALM_FIX_RING;
	 pEventTable->event_content_info.ring = 1;

        save_result = ALARMCLOCK_SaveInteEventInfo(i, pEventTable);
	 if(save_result){
		MMIALM_RemoveIdlePushMsg(i, SMART_EVENT_ALARM);
	 }else{
		Trace_Readboy_Log("corepush: alarm set alarm_set_for_corepush set alarm[%d] null error!!", i);
	 }
    }

    SCI_FREE( pEventTable );
    
}


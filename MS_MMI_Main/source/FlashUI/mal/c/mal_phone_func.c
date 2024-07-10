/*
 * FileName: mal_alarm_func.c
 * Copyright (c) 2005-2011 Microrapid Inc.
 * Author:  Platform Group
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

 /* =======================================================================
 * Include headers
 * ======================================================================= */  
#include "mmi_flashui_trc.h"
#include "amuse_ani.h" 
#include "amuse_as_value_api.h"
#include "flashtask_msg.h"
#include "mal_phone_func.h"
#include "mal_phone_interface.h"
#include "guistatusbar_data.h"
#include "mmi_id.h"
#include "mmiflash.h"

#include "Gui_ucs2b_converter.h"
/* =======================================================================
 *  Macro defines
 * ======================================================================= */
/* =======================================================================
 *  External variables
 * ======================================================================= */

/* =======================================================================
 *  Global variables
 * ======================================================================= */

/* =======================================================================
 *  Global functions
 * ======================================================================= */

int Mal_Phone_Entry_Screen(int screen_id)
{	
	if(screen_id < FLASH_SCREEN_ID_MAX && screen_id> FLASH_SCREEN_ID_INVALID)
    {   
        MMK_PostMsg(MAIN_IDLE_WIN_ID, MMIFLASH_MSG_ENTRY_SCREEN,	&screen_id, sizeof(screen_id));
    }
    else
    {
        //SCI_TRACE_LOW:"Mal_Phone_Entry_Screen id = %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAL_PHONE_FUNC_41_112_2_18_3_27_30_3,(uint8*)"d", screen_id);
    }
	return 1;
}

BOOLEAN Mal_Phone_STB_Update(void)
{	
	FlashPSMsg ps = {0};

	MMIFlash_UpdateIdleItem();
	ps.phone.m_type = MAL_PHONE_EVENT_STATUSBAR_CHANGE;
	FlashTask_PushPSEvent(FLASH_MSG_PS_PHONE, &ps);
	return TRUE;
}

int Phone_Event_StatusBar_Change()
{
	avalue	res;
	avalue	iconList;
	avalue	iconObj;
	avalue *argv[1];
	FLASH_IDLE_ITEM_T *pflashIdleItem = NULL;

	unsigned int  idx;
    BOOLEAN item_is_display = FALSE;
    char string[FLASH_IDLE_ITEM_STRING_LEN_MAX*3+1] = {0};

	pflashIdleItem = MMIFlash_GetFlashIdleItem();

	as_value_set_array(&iconList);
	for(idx = FLASH_IDLE_ITEM_NET1; idx < FLASH_IDLE_ITEM_MAX; idx++)
	{
	    item_is_display = pflashIdleItem[idx].is_display;
	    SCI_MEMSET(string, 0, FLASH_IDLE_ITEM_STRING_LEN_MAX*3+1);
	    if(pflashIdleItem[idx].wstr_len > 0)
	    {
    	    GUI_WstrToUTF8((uint8 *)string, (uint32)FLASH_IDLE_ITEM_STRING_LEN_MAX*3, pflashIdleItem[idx].wstr_ptr, pflashIdleItem[idx].wstr_len);
	    }
		as_value_set_object(&iconObj);
		as_value_push_member_object(&iconObj, AS_VALUE_TYPE_NUMBER_UNSIGNED_INT, "itemID", &idx);
		as_value_push_member_object(&iconObj, AS_VALUE_TYPE_BOOLEAN, "itemDisplay", &item_is_display);
		as_value_push_member_object(&iconObj, AS_VALUE_TYPE_STRING, "itemString", string);
		// TODO:text
		as_value_push_member_array(&iconList, AS_VALUE_TYPE_OBJECT, &iconObj);
	}
	
	argv[0] = &iconList;
	ani_callASFunc(MAL_PHONE_CALLBACK_STATUSBAR_CHANGE, 1, argv, &res);
	return 0;
}




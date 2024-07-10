/*
 * FileName: mal_phone_interface.c
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.11.3
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

/* =======================================================================
 * Include headers
 * ======================================================================= */  

#include "amuse_ani.h" 
#include "amuse_as_value_api.h"
#include "flashtask_msg.h"
//#include "flashtask_main.h"
#include "mal_phone_context.h"
#include "mal_phone_func.h"
#include "mal_phone_interface.h"
#include "amuse_file_device.h"



/* =======================================================================
 *  Macro defines
 * ======================================================================= */

// FOR AS API 
typedef int (*PHONE_API)(avalue *param, avalue *res);

/*lint -e773 */
#define REGISTER_PHONE_API_HANDLER(cmd, func)	gPhoneAPI[cmd] = func

/*lint -e773 */
#define CALL_PHONE_API(cmd)	if ((cmd < MAL_PHONE_CMD_MAX) && gPhoneAPI[cmd]){gPhoneAPI[cmd](argv[1], res);}


// FOR EVENT HANDLER
typedef int (*PHONE_HANDLER)(void);
#define REGISTER_PHONE_EVENT_HANDLER(evt, func)	gPhoneHandler[evt] = func

/*lint -e773 */
#define CALL_PHONE_EVENT_HANDLER(evt)	if(gPhoneHandler[evt]) {gPhoneHandler[evt]();}


/* =======================================================================
 *  External variables
 * ======================================================================= */

/* =======================================================================
 *  Global variables
 * ======================================================================= */
static PHONE_API	gPhoneAPI[MAL_PHONE_CMD_MAX] = {0};
static PHONE_HANDLER	gPhoneHandler[MAL_PHONE_EVENT_MAX] = {0};

/* =======================================================================
 *  Functions implement
 * ======================================================================= */ 

//=======================================================================
//  AS API
//=======================================================================

static int Phone_API_ENTRY_SCREEN(avalue *param, avalue *res)
{
	int screen_id = 0;
	as_value_get_value(param, &screen_id, AS_VALUE_TYPE_NUMBER_INT);

	Mal_Phone_Entry_Screen(screen_id);		
	as_value_set_boolean(res, 1);
	return 0;
}

static int Phone_API_GET_STB(avalue *param, avalue *res)
{
	unsigned char ret = Mal_Phone_STB_Update();
	as_value_set_boolean(res, ret);
	return 0;
}

static void
MAL_Phone_API_Init(void)
{
	memset(gPhoneAPI, 0, sizeof(gPhoneAPI));
	REGISTER_PHONE_API_HANDLER(MAL_PHONE_CMD_ENTRY_SCREEN, Phone_API_ENTRY_SCREEN);
	REGISTER_PHONE_API_HANDLER(MAL_PHONE_CMD_GET_STB, Phone_API_GET_STB);
}

static void 
MAL_Phone_API(aint argc, avalue **argv, avalue *res)
{
	unsigned int cmd;

	ANI_ASSERT_ARGC(2);
	ANI_ASSERT_ARGV(0, ANI_NUMBER);
	as_value_get_value(argv[0], &cmd, AS_VALUE_TYPE_NUMBER_UNSIGNED_INT);
	CALL_PHONE_API(cmd);
}
//=======================================================================



//=======================================================================
//  PS Events handler
//=======================================================================


static void
MAL_Phone_EventHandler_Init(void)
{
	memset(gPhoneHandler, 0, sizeof(gPhoneHandler));
	REGISTER_PHONE_EVENT_HANDLER(MAL_PHONE_EVENT_STATUSBAR_CHANGE, Phone_Event_StatusBar_Change);
}

static int 
MAL_Phone_CallBack(Flash_Signal *signal)
{
	unsigned int evt = signal->msg.ps.phone.m_type;
	amuse_printf("Phone Event callback, id = %d\n", evt);

	CALL_PHONE_EVENT_HANDLER(evt);
	return 0;
}
//=======================================================================






//=======================================================================
//  Exported functions
//=======================================================================
int MAL_Phone_Init(void)
{
	MAL_Phone_Context_Init();
	MAL_Phone_API_Init();
	MAL_Phone_EventHandler_Init();
	//MAL_Phone_EntryScreen_Init();
	return 0;
}


int MAL_Phone_Destroy(void)
{
	MAL_Phone_Context_Destroy();
	//Mal_Phone_STB_Destroy();
	return 0;
}


int MAL_Phone_RegisterEvtHandler(void)
{
	FlashTask_RegisterEventHandler(FLASH_MSG_PS_PHONE, MAL_Phone_CallBack);
	return 0;
}

int MAL_Phone_RegisterASAPI(void)
{
	ani_registerNative(MAL_PHONE_API_NAME, MAL_Phone_API);
	return 0;
}


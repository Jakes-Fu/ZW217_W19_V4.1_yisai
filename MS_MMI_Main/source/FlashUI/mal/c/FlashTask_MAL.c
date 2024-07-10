/*
 * FileName: FlashTask_MAL.c
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.31
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

/* =======================================================================
 * Include headers
 * ======================================================================= */  
#include "flashtask_mal.h"
#include "mal_phone_interface.h"
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
 *  Functions implement
 * ======================================================================= */ 
int FlashTask_MAL_Init(void)
{
	MAL_Phone_Init();
	return 0;
}

int FlashTask_MAL_Destroy(void)
{
	MAL_Phone_Destroy();
	return 0;
}

int FlashTask_MAL_RegisterEvtHandler(void)
{
	MAL_Phone_RegisterEvtHandler();
	return 0;
}

int FlashTask_MAL_RegisterASAPI(void)
{
	MAL_Phone_RegisterASAPI();
	return 0;
}


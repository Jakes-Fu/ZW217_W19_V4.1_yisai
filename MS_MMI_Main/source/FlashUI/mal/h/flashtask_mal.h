/*
 * FileName: FlashTask_MAL.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.31
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */
#ifndef FLASHTASK_MAL_H
#define FLASHTASK_MAL_H



/****************************************************************
 * Function:       FlashTask_MAL_Init
 * Description:   Initialize the Mobile Abstract Layer
 * Params:	None
 * Return:	
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_MAL_Init(void);

int FlashTask_MAL_Destroy(void);

int FlashTask_MAL_RegisterEvtHandler(void);

int FlashTask_MAL_RegisterASAPI(void);


#endif // FLASHTASK_MAL_H

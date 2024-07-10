/*
 * FileName: mal_phone_interface.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.11.3
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#ifndef MAL_PHONE_INTERFACE_H
#define MAL_PHONE_INTERFACE_H


#define MAL_PHONE_API_NAME		"PHONE_API"


typedef enum
{
	MAL_PHONE_CMD_INVALID=0,
	MAL_PHONE_CMD_ENTRY_SCREEN,
	MAL_PHONE_CMD_GET_STB,
	MAL_PHONE_CMD_MAX
}MAL_PHONE_API_CMD;



#define MAL_PHONE_CALLBACK_STATUSBAR_CHANGE "StatusBarChange"

typedef enum
{
	MAL_PHONE_EVENT_INVALID = 0,
	MAL_PHONE_EVENT_STATUSBAR_CHANGE,
	MAL_PHONE_EVENT_MAX
}MAL_PHONE_PS_EVENT;



int MAL_Phone_Init(void);


int MAL_Phone_Destroy(void);


int MAL_Phone_RegisterEvtHandler(void);


int MAL_Phone_RegisterASAPI(void);

#endif // MAL_PHONE_INTERFACE_H


/*
* amuse_fscommand_device.c
* Copyright (C) 2011 Microrapid Inc
* Author: gentoo <gentoo@Microrapid.com>
* 
* This file implementes flash fscommand interface(dummy) 
*   for mtk platform.
*/
#include "amuse_time_device.h"
#include <stdio.h>
#include <string.h>


void
amuse_fscmd2_Quit()
{

}

void 
amuse_fscmd_Launch(char* path)
{
	
}

int 
amuse_fscmd2_FullScreen(int size)
{
	return -1;
}

int
amuse_fscmd2_GetBatteryLevel()
{
	return 10; //fake implement
}

int
amuse_fscmd2_GetMaxBatteryLevel()
{
	return 10; //fake implement
}

int
amuse_fscmd2_GetDateDay()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0))
		return -1;

	return amuse_localtime(&current_time)->tm_mday;
}

int
amuse_fscmd2_GetDateMonth()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0))
		return -1;

	return amuse_localtime(&current_time)->tm_mon + 1;
}

int
amuse_fscmd2_GetDateWeekday()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0))
		return -1;

	return amuse_localtime(&current_time)->tm_wday;
}

int
amuse_fscmd2_GetDateYear()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0))
		return -1;

	return amuse_localtime(&current_time)->tm_year + 1900;
}

int 
amuse_fscmd2_GetDevice(char* buf, int len)
{
	const char* device_string_win32 = "AMUSE_WIN32_DEVICE";
	if((int)strlen(device_string_win32) < len){
		strcpy(buf, device_string_win32);
		return 0;
	}else{
		return -1;
	}
}

int 
amuse_fscmd2_GetDeviceID(char* buf, int len)
{
	const char* device_id_win32 = "AMUSE_WIN32_DEVICEID";
	if((int)strlen(device_id_win32) < len){
		strcpy(buf, device_id_win32);
		return 0;
	}else{
		return -1;
	}
}

int 
amuse_fscmd2_GetLanguage(char* buf, int len)
{
	const char* language_zhcn = "zh-CN";
	if((int)strlen(language_zhcn) < len){
		strcpy(buf, language_zhcn);
		return 0;
	}else{
		return -1;
	}
}

int 
amuse_fscmd2_GetLocaleLongDate(char* buf, int len)
{
	const char* Months[12] =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"January",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	amuse_time_t current_time = amuse_time(0);
	struct amuse_tm* date;
	char longdate[64];
	int length;

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	date = amuse_localtime(&current_time);
	length = sprintf(longdate, "%s %d, %d", Months[date->tm_mon], date->tm_mday, date->tm_year + 1900);
	if(length < len){
		strcpy(buf, longdate);
		return 0;
	}else{
		return -1;
	}
}

int 
amuse_fscmd2_GetLocaleShortDate(char* buf, int len)
{
	amuse_time_t current_time = amuse_time(0);
	struct amuse_tm* date;
	char shortdate[32];
	int length;

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	date = amuse_localtime(&current_time);
	length = sprintf(shortdate, "%d/%d/%d", date->tm_mon + 1, date->tm_mday, date->tm_year + 1900);
	if(length < len){
		strcpy(buf, shortdate);
		return 0;
	}else{
		return -1;
	}
}

int 
amuse_fscmd2_GetLocaleTime(char* buf, int len)
{
	amuse_time_t current_time = amuse_time(0);
	struct amuse_tm* date;
	char localetime[32];
	int length;

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	date = amuse_localtime(&current_time);
	length = sprintf(localetime, "%d:%d:%d", date->tm_hour, date->tm_min, date->tm_sec);
	if(length < len){
		strcpy(buf, localetime);
		return 0;
	}else{
		return -1;
	}
}

int
amuse_fscmd2_GetSignalLevel()
{
	return 10; // fake implement
}

int
amuse_fscmd2_GetMaxSignalLevel()
{
	return 10; // fake implement
}

int
amuse_fscmd2_GetVolumeLevel()
{
	return 100; // fake implement
}

int
amuse_fscmd2_GetMaxVolumeLevel()
{
	return 100; // fake implement
}

int
amuse_fscmd2_GetNetworkConnectStatus()
{
	return 0; // fake implement
}

int 
amuse_fscmd2_GetNetworkName(char* buf, int len)
{
	const char* networkname_win32 = "AMUSE_WIN32_NETWORK";
	if((int)strlen(networkname_win32) < len){
		strcpy(buf, networkname_win32);
		return 2;
	}else{
		return -1;
	}
}

int
amuse_fscmd2_GetNetworkRequestStatus()
{
	return -1;
}

int
amuse_fscmd2_GetNetworkStatus()
{
	return -1;
}

int 
amuse_fscmd2_GetPlatform(char* buf, int len)
{
	const char* platform_amuse_win32 = "AMUSE_WIN32_PLATFORM";
	if((int)strlen(platform_amuse_win32) < len){
		strcpy(buf, platform_amuse_win32);
		return 0;
	}else{
		return -1;
	}
}

int
amuse_fscmd2_GetPowerSource()
{
	return 1; // fake implement
}

int
amuse_fscmd2_GetTimeHours()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	return amuse_localtime(&current_time)->tm_hour;
}

int
amuse_fscmd2_GetTimeMinutes()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	return amuse_localtime(&current_time)->tm_min;
}

int
amuse_fscmd2_GetTimeSeconds()
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}

	return amuse_localtime(&current_time)->tm_sec;
}

int 
amuse_fscmd2_GetTimeZoneOffset(int* offset)
{
	amuse_time_t current_time = amuse_time(0);

	if(current_time == (amuse_time_t)(~0)){
		return -1;
	}else{
		*offset = (int)(amuse_difftime(amuse_mktime(amuse_localtime(&current_time)), amuse_mktime(amuse_gmtime(&current_time))) / 60);
		return 0;
	}
}

int 
amuse_fscmd2_StartVibrate(int duration, int interval, int times)
{
	return -1;
}

int
amuse_fscmd2_StopVibrate()
{
	return -1;
}


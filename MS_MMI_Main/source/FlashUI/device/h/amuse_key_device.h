/*
* amuse_key_device.h
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains key interface that need to 
* be implemented on different platforms.
*/

#ifndef _AMUSE_KEY_DEVICE_H_
#define _AMUSE_KEY_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "amuse_key.h"

#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C	__declspec(dllimport)
#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
#endif

/** 
* This function maps custom key code to amuse key code.
*
* Parameters: none
* Returns: none
*/
EXPORT_C key_map* amuse_key_createMap(int keynum);

/** 
* This function maps custom key code to amuse key code.
*
* Parameters: none
* Returns: none
*/
EXPORT_C void amuse_key_destroyMap(key_map* keymap);

/** 
* call back function to check if have key repeat
*
* Parameters: none
* Returns: none
*/
void checkKeyRepeat_cb(void);

/** 
* check the key
*
* Parameters: hard key
* Returns: 1:ok ,0:fail
*/
EXPORT_C int is_valid_key(int hard_key);

EXPORT_C unsigned int get_key_index_from_hard_code(unsigned int hard);

EXPORT_C char* key_device_get_item(int index, int* default_key, int* softkey);


#ifdef __cplusplus
}
#endif

#endif

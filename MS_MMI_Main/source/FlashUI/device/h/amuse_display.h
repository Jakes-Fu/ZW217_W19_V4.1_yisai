/*
* amuse_display.h
* Copyright (C) 2011 Microrapid Inc
* Author: Black <black@Microrapid.com>
* 
* This file contains display interface that need to 
* be implemented on different platforms.
*/

#ifndef _AMUSE_DISPLAY_H_
#define _AMUSE_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C	__declspec(dllimport)
#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
#endif

#include "mmk_type.h"

#ifndef AMUSE_CANVAS_T
typedef unsigned char* amuse_canvas;
#define AMUSE_CANVAS_T
#endif

typedef struct  
{
	unsigned char *canvas;
	int x;
	int y;
	int width;
	int height;
}amuse_display_rect_info;

/*
 * amuse_get_canvas: get canvas,display buffer
 * 
 * return value : a buffer for amuse output , 0 for error
 */
EXPORT_C amuse_canvas amuse_get_canvas();

/*
 * amuse_display_init: init display
 * width : screen width
 * height : screen height
 * color_depth: 16 or 32
 * return value : a buffer for amuse output , 0 for error
 */
EXPORT_C amuse_canvas amuse_display_init(int width, int height,int color_depth);

/*
 *
 */
EXPORT_C amuse_canvas open_screen();

/*
 * amuse_display_destory: destory display
 * return value: 0 for ok, -1 for error
 */
EXPORT_C int amuse_display_destory(void);

/*
 * amuse_display_lock: lock screen before write display buffer
 * if the platform need not lock
 * only return 0
 * return value : 0 for ok, -1 for error
 */
EXPORT_C int amuse_display_lock();

/*
 *  amuse_display_unlock: unlock screen after write display buffer
 *  if the platform need not lock
 *  just return 0
 *  return value: 0 for ok, -1 for error
 */
EXPORT_C int amuse_display_unlock();

/*
 * amuse_display_update: update screen
 * return value: 0 for ok, -1 for error
 */
EXPORT_C int amuse_display_update(int width,int height);

/*
* amuse_display_update_rect: update screen area (with clip)
* return value: 0 for ok, -1 for error
*/
EXPORT_C int amuse_display_update_rect(int x,int y, int width,int height);

/*
* GetClipRectLeft: get the refresh rect xmin
*
*/
EXPORT_C int GetClipRectLeft();

/*
* GetClipRectRight: get the refresh rect xmax
* 
*/
EXPORT_C int GetClipRectRight();

/*
* GetClipRectTop: get the refresh rect ymin
* 
*/
EXPORT_C int GetClipRectTop();

/*
* GetClipRectBottom: get the refresh rect ymax
* 
*/
EXPORT_C int GetClipRectBottom();

EXPORT_C int amuse_display_update_rect(int x, int y, int width, int height);

EXPORT_C void amuse_get_flash_update_rect(GUI_RECT_T *pRect);
EXPORT_C void amuse_set_screen_size(short w, short h);
#ifdef __cplusplus
}
#endif

#endif

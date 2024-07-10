#ifndef __LEBAO_WIN_MENU_H
#define __LEBAO_WIN_MENU_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_menu_open(void* data);
void lebao_menu_close(void* data);
int  lebao_menu_is_opened(void);

void lebao_about_open(void* data);
void lebao_about_close(void* data);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_MENU_H*/
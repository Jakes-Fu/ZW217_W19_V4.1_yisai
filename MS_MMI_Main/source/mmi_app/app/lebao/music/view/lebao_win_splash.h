#ifndef __LEBAO_WIN_SPLASH_H
#define __LEBAO_WIN_SPLASH_H

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
void lebao_splash_open(void* data);
void lebao_splash_close(void* data);
int  lebao_splash_is_opened(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_SPLASH_H*/

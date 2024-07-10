#ifndef __LEBAO_WIN_VOLUME_H
#define __LEBAO_WIN_VOLUME_H

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
void lebao_volume_open(void* data);
void lebao_volume_close(void* data);
void lebao_volume_set_default(const int per);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_VOLUME_H*/
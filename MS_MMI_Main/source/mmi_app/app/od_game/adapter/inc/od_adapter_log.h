
#ifndef _OD_ADAPTER_LOG_H
#define _OD_ADAPTER_LOG_H
#include "od_type_def.h"
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
extern void   od_game_printf( const char *format, ... );

//#define __OD_DEBUG__

#ifdef __OD_DEBUG__
    #define OD_LOG  od_game_printf	
#else
    #define OD_LOG
#endif

#ifdef __cplusplus
}
#endif

#endif



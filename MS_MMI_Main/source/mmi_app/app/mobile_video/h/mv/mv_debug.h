#ifndef  _MV_DEBUG_H
#define _MV_DEBUG_H

#include "_Types.h"

#define  DEBUG_OUTPUT       
#define  MV_NET_DEBUG     
#define  PLAYER_DEBUG    
#define  DEBUG_FS

#if 0
#define  SOCK_DEBUG  
#define MV_MM_DEBUG
#define  DATE_DEBUG
#define  DEBUG_SMS
#endif
#define MV_TRACE_LOW SCI_TRACE_LOW
extern _VOID WriteLogsL(_UINT uLevel, _CONST _CHAR* aFmt, ...);
#endif

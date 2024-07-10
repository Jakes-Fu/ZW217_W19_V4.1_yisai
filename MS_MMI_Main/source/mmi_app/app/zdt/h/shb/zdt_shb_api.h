#ifndef __ZDT_SHB_API_H__
#define __ZDT_SHB_API_H__

#include "zdt_common.h"

typedef struct SHB_APP_T         SHB_APP_T;

extern int SHB_API_BatStatus_Post(uint8 level,uint8 percent);
extern int SHB_API_LBS_Change_Post(void);
#endif

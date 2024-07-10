#ifndef MMIWATCHIDLE_H
#define MMIWATCHIDLE_H

#include "mmiwatchidle_effect0.h"
#include "mmiwatchidle_effect1.h"

#ifdef ANALOG_CLK_SUPPORT  //yangyu add

PUBLIC void MMIWatchIdle_StartClockTimer(int usec);
PUBLIC void MMIWatchIdle_StopClockTimer(void);

PUBLIC void createBackground(void* param);
PUBLIC void createClock(void* param);
PUBLIC void updateBackground(void* param);
PUBLIC void updateClock(void* param);
PUBLIC void destoryBackground(void* param);
PUBLIC void destoryClock(void* param);

#endif
#endif
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "string.h"
//#include "osi_api.h"
//#include "osi_log.h"
#include "paho_config.h"

#if !defined(__PAHO_UNITIME_H__)
#define __PAHO_UNITIME_H__

int process_compilation_warning(int rc);

//Portint Time
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0x00
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0x01
#endif
int usleep(useconds_t usec);
int clock_gettime(clockid_t _clk_id, struct timespec *_tp);
long long GetTickCount();

#endif

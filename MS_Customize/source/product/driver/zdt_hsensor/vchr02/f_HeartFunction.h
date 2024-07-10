/*
 * f_getHeartParasInit.h
 *
 *  Created on: 2017-1-10
 *      Author: runningHorse
 */

#ifndef GETHEARTPARAS
#define GETHEARTPARAS

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void f_HeartFunctionInit(void);
void f_HeartFunction(int32_t heartRate,int32_t reliability,int32_t accX,int32_t stdTime,int16_t sbpRef,int16_t dbpRef,int16_t *sdnn,int16_t *sbp,int16_t *dbp,int16_t *oxygen,int16_t *errType);
#endif /* GETHEARTPARAS */
#ifndef __pah_800x_bsp_h__
#define __pah_800x_bsp_h__

// c
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// pah
#include "pah_unit.h"
#include "pah_800x_config.h"


//============================================================================
// Public function
//============================================================================
#ifdef __cplusplus
extern  "C" {
#endif 

void pah_delay_ms(uint32_t ms);
uint64_t pah_get_tick_count(void);
pah_ret pah_comm_i2c_write(uint8_t addr, const uint8_t *data, size_t write_size);
pah_ret pah_comm_i2c_read(uint8_t addr, uint8_t *data, size_t read_size);

/*
void pah_print(
    int32_t                         fileline,
    const char                      *funcname,
    const char                      *filename,
    const char                      *format, ...);
*/

void ReadGSensor(float *x, float *y, float *z);

#ifdef __cplusplus
}
#endif

#endif  // header guard

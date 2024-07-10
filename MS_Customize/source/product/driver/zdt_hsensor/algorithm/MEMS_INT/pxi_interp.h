//#pragma once

#include <stdint.h>
#include <stddef.h>

void pxi_interp_ex1(int16_t *src_data, size_t src_data_count, int16_t *dst_data, size_t dst_data_count, size_t offset);
void  PXI_MEMS_interp(int16_t *src_data, size_t src_data_count, int16_t *dst_data, size_t dst_data_count);

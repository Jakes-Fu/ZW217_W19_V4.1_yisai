#include "pxi_interp.h"


void  PXI_MEMS_interp(int16_t *src_data, size_t src_data_count, int16_t *dst_data, size_t dst_data_count)
{
	  pxi_interp_ex1(src_data,  src_data_count ,dst_data,dst_data_count ,3);
		pxi_interp_ex1(&src_data[1],  src_data_count ,&dst_data[1],dst_data_count ,3);
		pxi_interp_ex1(&src_data[2],  src_data_count ,&dst_data[2],dst_data_count ,3);
}




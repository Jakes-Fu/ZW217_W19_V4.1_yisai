#include "pxi_interp.h"


/*****
* @brief Example of Platform. In this function we initialize powerkey and register callback.
* @param None.
* @return None.
*
*****/

void pxi_interp_ex1(int16_t *src_data, size_t src_data_count, int16_t *dst_data, size_t dst_data_count, size_t offset)
{
    float pre_time_acc = 0.0f;
    float time_acc = 0.0f;
    float d_time = (float)dst_data_count / src_data_count;
    size_t idx_ptr = 0;

    for (size_t i = 0; i < dst_data_count; ++i)
    {
        while (time_acc < (float)i)
        {
            pre_time_acc = time_acc;
            time_acc = time_acc + d_time;
            idx_ptr++;
        }

        if (idx_ptr == 0)
        {
            dst_data[i * offset] = src_data[idx_ptr * offset];
        }
        else if (idx_ptr == dst_data_count)
        {
            dst_data[i * offset] = src_data[(idx_ptr - 1) * offset];
        }
        else
        {
            float time_ratio = ((float)i - pre_time_acc) / (time_acc - pre_time_acc);
            dst_data[i * offset] = (int16_t)((float)src_data[(idx_ptr - 1) * offset] * (1.0f - time_ratio) + (float)src_data[idx_ptr * offset] * time_ratio + 0.5f);
        }
    }
}


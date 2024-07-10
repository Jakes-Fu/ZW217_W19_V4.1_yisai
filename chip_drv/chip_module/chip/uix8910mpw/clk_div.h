#ifndef __RDA_CLK_DIV_H__
#define __RDA_CLK_DIV_H__

#include "sci_types.h"

uint16 clk_get_div_by_regval(unsigned reg, unsigned bit_field);
uint16 clk_get_regval_by_freq(uint32 basefreq, uint32 reqfreq);
uint16 clk_get_regval_by_div(uint16 div);

#endif /*__RDA_CLK_DIV_H__*/


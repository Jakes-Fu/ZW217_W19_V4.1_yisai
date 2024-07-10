
#include "sci_types.h"
#include "global_macros.h"
#include "sys_ctrl.h"

/* clock divider table size */
static const uint16 clock_div_tab[][2] = {
	{10, 0x0f},			/* div: 1 */
	{20, 0x0d},			/* div: 2 */
	{25, 0x0c},			/* div: 2.5 */
	{30, 0x0b},			/* div: 3 */
	{35, 0x0a},			/* div: 3.5 */
	{40, 0x09},			/* div: 4 */
	{45, 0x08},			/* div: 4.5 */
	{50, 0x07},			/* div: 5 */
	{55, 0x06},			/* div: 5.5 */
	{60, 0x05},			/* div: 6 */
	{70, 0x04},			/* div: 7 */
	{80, 0x03},			/* div: 8 */
	{120, 0x02},			/* div: 12 */
	{160, 0x01},			/* div: 16 */
	{240, 0x0e},			/* div: 24 */
	{240, 0x00}			/* div: 24 */
};

/*
 * given a base frequecy, and the required frequecy,
 * calculate the divider, and then return the real
 * value used by the clock register
 */
uint16 clk_get_regval_by_div(uint16 div)
{
	uint32 i;

	for (i = 0; i < (ARRAY_SIZE(clock_div_tab) - 1); i++) {
		if (div == clock_div_tab[i][0])
			break;
		if ((div > clock_div_tab[i][0])
		    && (div < clock_div_tab[i + 1][0]))
			break;
		if (i == (ARRAY_SIZE(clock_div_tab) - 2))
			break;
	}
	//rprintf("i=%d,div=%x\n", i, clock_div_tab[i][1]);
	return clock_div_tab[i][1];
}

uint16 clk_get_regval_by_freq(uint32 basefreq, uint32 reqfreq)
{
	uint16 adiv;

	//rprintf("clk_get_regval_by_div\n");
	if (reqfreq == 0)
		adiv = 10;
	else
		adiv = (basefreq * 10) / reqfreq;

	return clk_get_regval_by_div(adiv);
}

/*
 * given a register, and the diveder's mask bit, return
 * the real divider;
 * this is the opposite way of the function
 * clk_get_regval_by_div()
 *
 */
uint16 clk_get_div_by_regval(unsigned reg, unsigned bit_field)
{
	unsigned long div;
	int i;

	div = reg & bit_field;
	for (i = 0; i < ARRAY_SIZE(clock_div_tab); i++)
       {   
            if (div == clock_div_tab[i][1])
            {
                return clock_div_tab[i][0];
            }
       }
       return 0;
}


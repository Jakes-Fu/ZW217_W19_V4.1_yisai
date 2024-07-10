#include <stdio.h>
//#include "pxialg_r.h"


#define debug_fpritnf printf

void log_printf(char *s, float f)
{
			debug_fpritnf("%s = %f\n", s, f);
}

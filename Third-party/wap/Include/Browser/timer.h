/* timer.h
 *
 * This file contains API about time.
 *
 */

#ifndef TIME_FUNCTION
#define TIME_FUNCTION

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"

/*==============================
 * EXTERNAL DECLARATIONS
 *==============================*/
extern signed8 tick(void);
extern signed8 settime(TOD_Control *time_buffer);
//!!extern signed8 gettime(clock_get_options  option, void  *time_buffer);

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */

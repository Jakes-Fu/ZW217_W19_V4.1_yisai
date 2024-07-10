#ifndef _MMIDROPDOWN_POSITION_H_ /*perl robot add*/
#define _MMIDROPDOWN_POSITION_H_ /*perl robot add*/
//****************************************Shortcut***********************************//
#include "mmi_position.h"
#include "mmidropdownwin_export.h"
#include "mmi_signal_ext.h"

#ifdef MAINLCD_SIZE_320X480
#include "mmidropdown_position_320x480.h"

#elif defined MAINLCD_SIZE_240X400
#include "mmidropdown_position_240x400.h"

#elif defined MAINLCD_SIZE_240X320
#include "mmidropdown_position_240x320.h"

#elif defined MAINLCD_SIZE_176X220
#include "mmidropdown_position_176x220.h"
#elif defined MAINLCD_SIZE_128X160
#include "mmidropdown_position_128X160.h"
#else
#error
#endif

#endif  /*perl robot add*/


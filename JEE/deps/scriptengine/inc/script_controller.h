#ifndef __JERRY_SCRIPT_CONTROLLER_H__
#define __JERRY_SCRIPT_CONTROLLER_H__

#include "jerryscript.h"
#include "jerry_util.h"

#ifdef __cplusplus
extern "C" {
#endif

	int execute_script(const char* file, const jerry_char_t* script, jerry_length_t script_size);
	bool script_controller_init();
	void script_controller_exit();

#ifdef __cplusplus
}
#endif

#endif // __JERRY_SCRIPT_CONTROLLER_H__
/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 //#ifdef WINER
 //#include <io.h>
 //#include <process.h>
#include <stdarg.h>
//#esle
//#include <unistd.h>
//#endif
#include "os_api.h"
#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerry_util.h"
#include "jerry_port.h"

#ifdef WIN32
#ifdef USE_MINGW
#define snprintf(...) _snprintf(__VA_ARGS__)
#define vsnprintf(...) _vsnprintf(__VA_ARGS__)
#endif
#endif

extern void SCI_Trace(const char* x_format, ...);
extern uint32 SCI_GetCurrentTime(void);
extern void SCI_Sleep(uint32 time_in_ms);

#define MAX_LOG_BUFFER_SIZE (256)
static char js_log_buf[MAX_LOG_BUFFER_SIZE];


/**
 * JerryScript log level
 */
static jerry_log_level_t jerry_log_level = JERRY_LOG_LEVEL_DEBUG;



/**
 * Sets log level.
 */
void set_log_level(jerry_log_level_t level)
{
    jerry_log_level = level;
} /* set_log_level */
void jerry_port_fatalEx(char* errcode,jerry_fatal_code_t code)
{
    char buff[128] = { 0 };
#ifdef WIN32
        abort();//for debug
#endif
#ifdef JERRY_DEBUGGER
    jerry_debugger_get_memstats(buff, sizeof(buff));
    SCI_TRACE_LOW(" JSE Error : jerry_port_fatal code is %d", code);
#else
    SCI_TRACE_LOW("javascript exception! but not open debugger");
#endif
    SCI_PASSERT(1<0,(errcode));
}

/**
 * Aborts the program.
 */
void jerry_port_fatal(jerry_fatal_code_t code)
{
    char buff[128] = { 0 };
    #ifdef WIN32
        abort();//for debug
    #endif
#ifdef JERRY_DEBUGGER
    jerry_debugger_get_memstats(buff, sizeof(buff));
    SCI_TRACE_LOW(" JSE Error : jerry_port_fatal code is %d", code);
#else
    SCI_TRACE_LOW("javascript exception! but not open debugger");
#endif

    SCI_ASSERT(0);
} /* jerry_port_fatal */

/**
 * Provide log message implementation for the engine.
 */
extern void pal_printlog(char* fmt, char* buff);
void
jerry_port_log(jerry_log_level_t level, /**< log level */
	const char* format, /**< format string */
	...)  /**< parameters */
{
	if (level <= jerry_log_level) {
		va_list args;
		size_t length = 0;

		va_start(args, format);

		/* the return value of vsnprintf is the number of bytes that would be
		 * written to buffer had if the size of the buffer been sufficiently
		 * large excluding the terminating null byte. If the output string
		 * would be larger than the js_log_buf, we have to adjust the output
		 * length. */
		length = vsnprintf(js_log_buf, MAX_LOG_BUFFER_SIZE - 1, format, args);
		if (length > MAX_LOG_BUFFER_SIZE - 1) {
			js_log_buf[MAX_LOG_BUFFER_SIZE - 1] = '\0';
		}
		else {
			js_log_buf[length] = '\0';
		}

		//SCI_TraceLow("%s", js_log_buf);

		switch (level) {

		case JERRY_LOG_LEVEL_ERROR:
			SCI_TRACE_LOW(" JSE Error :  %s", js_log_buf);
			break;
		case JERRY_LOG_LEVEL_WARNING:
			SCI_TRACE_LOW(" JSE Warn  :  %s", js_log_buf);
			break;
		case JERRY_LOG_LEVEL_DEBUG:
			SCI_TRACE_LOW(" JSE Debug  :  %s", js_log_buf);
			break;
		default:
			SCI_TRACE_LOW(" JSE Info  :  %s", js_log_buf);
			break;
		}



		va_end(args);
	}
} /* jerry_port_log */

/**
 * Determines the size of the given file.
 * @return size of the file
 */
static size_t
jerry_port_get_file_size(FILE* file_p) /**< opened file */
{
	fseek(file_p, 0, SEEK_END);
	long size = ftell(file_p);
	fseek(file_p, 0, SEEK_SET);

	return (size_t)size;
} /* jerry_port_get_file_size */

/**
 * Opens file with the given path and reads its source.
 * @return the source of the file
 */
uint8_t*
jerry_port_read_source(const char* file_name_p, /**< file name */
	size_t* out_size_p) /**< [out] read bytes */
{
	char* file_content = NULL;
	char* full_path = NULL;
	char* full_dir = NULL;
	char* dirname = NULL;
	int dirname_len = 0;

	jerry_value_t dirname_value = 0;
	jerry_value_t global_obj = 0;

	//Get Current Work Path
	global_obj = jerry_get_global_object();
	dirname_value = jerry_jval_get_property(global_obj, "__dirname");

	jerry_string_t jdir_name = jerry_jval_as_string(dirname_value);
	dirname = jerry_string_data(&jdir_name);
	dirname_len = jerry_string_size(&jdir_name);

	//Retry path
	full_path = module_retry_path(dirname, file_name_p);
	if (full_path)
	{
		file_content = js_read_file(full_path, out_size_p);
	}

	if (NULL != dirname && dirname_len != 0)
	{
		jerry_string_destroy(&jdir_name);
	}

	if (full_dir)
	{
		art_free(full_dir);
	}
	if (full_path)
	{
		art_free(full_path);
	}

	jerry_release_value(global_obj);
	jerry_release_value(dirname_value);

	return file_content;
} /* jerry_port_read_source */

/**
 * Release the previously opened file's content.
 */
void
jerry_port_release_source(uint8_t* buffer_p) /**< buffer to free */
{
	art_free(buffer_p);
} /* jerry_port_release_source */

/**
 * Normalize a file path
 *
 * @return length of the path written to the output buffer
 */
size_t
jerry_port_normalize_path(const char* in_path_p,   /**< input file path */
	char* out_buf_p,         /**< output buffer */
	size_t out_buf_size,     /**< size of output buffer */
	char* base_file_p) /**< base file path */
{
	(void)base_file_p;
	size_t len = 0;
	if (!in_path_p || !out_buf_p)
	{
		return 0;
	}
	len = strlen(in_path_p);
	if (len + 1 > out_buf_size)
	{
		return 0;
	}

	/* Return the original string. */
	strcpy(out_buf_p, in_path_p);
	return len;
} /* jerry_port_normalize_path */

/**
 * Get the module object of a native module.
 *
 * @return undefined
 */
jerry_value_t
jerry_port_get_native_module(jerry_value_t name) /**< module specifier */
{
	(void)name;
	return jerry_create_undefined();
} /* jerry_port_get_native_module */

/**
 * Dummy function to get the time zone adjustment.
 *
 * @return 0
 */
double
jerry_port_get_local_time_zone_adjustment(double unix_ms, bool is_utc)
{
	/* We live in UTC. */
	return 0;
} /* jerry_port_get_local_time_zone_adjustment */

/**
 * Dummy function to get the current time.
 *
 * @return 0
 */
double
jerry_port_get_current_time(void)
{
	uint32 ms_num = SCI_GetCurrentTime();
	return (double)ms_num;
} /* jerry_port_get_current_time */

/**
 * Provide the implementation of jerry_port_print_char.
 * Uses 'JS_DEBUG_LOG' to print a single character to standard output.
 */
void
jerry_port_print_char(char c) /**< the character to print */
{
	abort();
	JS_DEBUG_LOG("%c", c);
} /* jerry_port_print_char */

/**
 * Provide implementation of jerry_port_sleep.
 */
void jerry_port_sleep(uint32_t sleep_time) /**< milliseconds to sleep */
{
	SCI_Sleep(sleep_time);
} /* jerry_port_sleep */

/**
 * Pointer to the current context.
 */
static jerry_context_t* current_context_p = NULL;

/**
 * Set the current_context_p as the passed pointer.
 */
void
jerry_port_default_set_current_context(jerry_context_t* context_p) /**< points to the created context */
{
	current_context_p = context_p;
} /* jerry_port_default_set_current_context */

/**
 * Get the current context.
 *
 * @return the pointer to the current context
 */
jerry_context_t*
jerry_port_get_current_context(void)
{
	return current_context_p;
} /* jerry_port_get_current_context */

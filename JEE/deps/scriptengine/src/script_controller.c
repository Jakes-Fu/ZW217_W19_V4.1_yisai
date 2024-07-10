#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "jerryscript.h"
#include "jerryscript-ext/debugger.h"
#include "jerryscript-ext/handler.h"
#include "jerryscript-port.h"

#include "script_controller.h"
#include "urjs_art.h"
LOCAL jerry_context_t* g_tls_context = NULL;

static jerry_value_t backtrace_handler();

int execute_script(const char* file, const jerry_char_t* script, jerry_length_t script_size)
{
    bool run_ok = false;
    int tick = 0;
    int flen = 0;
    if (file != NULL) flen = strlen(file);

    tick = SCI_GetTickCount();
    /* Setup Global scope code */
    jerry_value_t parsed_code = jerry_parse(file, flen, script, script_size, JERRY_PARSE_NO_OPTS);
    alogd("parse %s : %dms", file ? file : "null", SCI_GetTickCount() - tick);

    /* Check if there is any JS code parse error */
    if (!jerry_value_is_error(parsed_code))
    {
        tick = SCI_GetTickCount();
        /* Execute the parsed source code in the Global scope */
        jerry_value_t ret_value = jerry_run(parsed_code);
        alogd("run %s : %dms", file ? file : "null", SCI_GetTickCount() - tick);

        /* Check the execution return value if there is any error */
        run_ok = !jerry_value_is_error(ret_value);
        if (jerry_value_is_error(ret_value)) {
            char buff[128] = { 0 };
            aloge("run source error! :%s ", file);
#ifdef JERRY_DEBUGGER			
            jerry_debugger_get_exception_info(buff, sizeof(buff));
            aloge("javascript exception!   %s ", buff);
#else

			aloge("javascript exception! but not open debugger");
#endif

            backtrace_handler();
#ifdef JERRY_DEBUGGER
            memset(buff, 0, sizeof(buff));
            jerry_debugger_get_memstats(buff, sizeof(buff));
            alogi("memory: %s", buff);
#else
            aloge("javascript exception! but not open debugger");
#endif

        }

        /* Returned value must be freed */
        jerry_release_value(ret_value);
    }
    else
    {
#ifdef JERRY_DEBUGGER    
        char buff[128] = { 0 };
        aloge("parse source error! ");
        jerry_debugger_get_exception_info(buff, sizeof(buff));
        aloge("javascript exception!  %s", buff);
#else
        aloge("javascript exception! but not open debugger");
#endif		
    }

    /* Parsed source code must be freed */
    jerry_release_value(parsed_code);
    if (jerry_value_is_error(parsed_code)) {
        js_art_parse_error_notify();
    }
    return (run_ok ? 1 : 0);
}

bool script_controller_init_modules() {
    js_module_init();
}

static void* script_controller_context_alloc(size_t size, void* cb_data_p)
{
    return art_malloc(size);

}


static jerry_value_t backtrace_handler()
{
    if (!jerry_is_feature_enabled(JERRY_FEATURE_LINE_INFO))
    {
        printf("Line info disabled, no backtrace will be printed\n");
        return jerry_create_undefined();
    }

    /* If the line info feature is disabled an empty array will be returned. */
    jerry_value_t backtrace_array = jerry_get_backtrace(5);
    uint32_t array_length = jerry_get_array_length(backtrace_array);
    alogi("backtrace:");
    for (uint32_t idx = 0; idx < array_length; idx++)
    {
        jerry_value_t property = jerry_get_property_by_index(backtrace_array, idx);

        jerry_char_t string_buffer[64];
        jerry_size_t copied_bytes = jerry_substring_to_char_buffer(property,
            0,
            63,
            string_buffer,
            63);
        string_buffer[copied_bytes] = '\0';
        alogi(" %d: %s", idx, string_buffer);

        jerry_release_value(property);
    }

    jerry_release_value(backtrace_array);

    return jerry_create_undefined();
} /* backtrace_handler */

bool script_controller_init()
{
    /* JERRY_ENABLE_EXTERNAL_CONTEXT */
	//512 * 1024,
    g_tls_context = jerry_create_context(256 * 1024, script_controller_context_alloc, NULL);
    jerry_port_default_set_current_context(g_tls_context);
    jerry_init(JERRY_INIT_EMPTY);

    jerry_value_t global = jerry_get_global_object();
    {
        jerry_value_t func = jerry_create_external_function(backtrace_handler);
        jerry_value_t name = jerry_create_string((const jerry_char_t*) "js_art_backtrace");
        jerry_value_t result = jerry_set_property(global, name, func);
        jerry_release_value(result);
        jerry_release_value(name);
        jerry_release_value(func);
    }
    jerry_release_value(global);

    script_controller_init_modules();
    return true;
}

void script_controller_exit()
{
#ifdef WIN32//force exit js engine,,,temp fix,bug2114788
        /* Cleanup engine */
        jerry_cleanup();
#endif


    art_free(g_tls_context);

}


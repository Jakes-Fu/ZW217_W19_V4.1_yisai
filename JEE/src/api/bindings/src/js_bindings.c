#include <stdlib.h>
#include <stdio.h>

#include "jerryscript.h"
#include "jerry_bindings.h"
#include "jerry_module.h"

#include "js_audio.h"

#include "js_device.h"

#include "jerry_gui.h"
#include "jerry_kv.h"
#include "js_fef.h"
#include "jerry_packagemanager.h"
// add for resourceload
#include "jerry_resourceload.h"
#include "jerry_permission.h"

bool register_native_function(const char *name,
                              jerry_external_handler_t handler)
{
    jerry_value_t global_obj_val = jerry_get_global_object();
    jerry_value_t reg_func_val = jerry_create_external_function(handler);
    bool bok = true;

    if (!(jerry_value_is_function(reg_func_val)
          && jerry_value_is_constructor(reg_func_val)))
    {
        JS_ERROR_LOG("!!! create_external_function failed !!!\r\n");
        jerry_release_value(reg_func_val);
        jerry_release_value(global_obj_val);
        return false;
    }

    jerry_value_t prop_name_val =
        jerry_create_string((const jerry_char_t *)name);
    jerry_value_t res =
        jerry_set_property(global_obj_val, prop_name_val, reg_func_val);

    jerry_release_value(reg_func_val);
    jerry_release_value(global_obj_val);
    jerry_release_value(prop_name_val);

    if (jerry_value_is_error(res))
    {
        JS_DEBUG_LOG("!!! register_native_function failed: [%s]\r\n", name);
        jerry_release_value(res);
        return false;
    }

    jerry_release_value(res);

    return true;
} /* register_native_function */

// TODO register all UAL API
bool js_bindings_init(void)
{
    jerry_init_buffer();
    js_register_audio_functions();
    js_register_device_functions();
    js_register_file_functions();

    js_register_gui_functions();
    js_register_kv_functions();
    js_register_system_functions();
    js_register_console_functions();
    js_register_watchface_day_functions();
    js_register_watchface_week_functions();
    js_register_watchface_multinum_functions();
    js_register_watchface_digitaltime_functions();
    js_register_watchface_month_functions();
    js_register_art_functions();
    js_register_packagemanager_functions();
    js_register_editbox_functions();
    js_register_i18n_functions();
    // add for resourceload
    js_register_resourceload_functions();
    // add for permission
    js_register_permission_functions();
    //js_register_http_functions();
    return true;
}

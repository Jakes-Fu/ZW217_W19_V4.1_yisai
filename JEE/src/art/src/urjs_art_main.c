/*
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 * Copyright 2021-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 * Created on: 2021/09/18
 * Author: jie.yang@unisoc.com
 */

#include "jerryscript-core.h"
#include "jee.h"
#include "jee_def.h"
#include "urjs_art.h"
#include "urjs_art_memory.h"
#include "packagemanager.h"
#include "permission_check.h"
//#include "pm_list.h"
#include "urjs_art_module.h"
#ifdef JERRY_DEBUGGER
#if JERRY_DEBUGGER
//#if defined (JERRY_DEBUGGER) && (JERRY_DEBUGGER == 1)
#include "debugger.h"
#include "jerryscript-debugger.h"
#include "ual_kv.h"

#define DEV_MODE_KEY "developer_mode"
#define JERRY_DEBUGGER_PORT 5001
#endif
#endif

#ifdef WIN32
#ifdef USE_MINGW
#define snprintf(...) _snprintf(__VA_ARGS__)
#define vsnprintf(...) _vsnprintf(__VA_ARGS__)
#endif
#endif

enum JS_APP_STATUS_E
{
    JS_APP_STATUS_NULL,
    JS_APP_STATUS_RUNNING,
    JS_APP_STATUS_EXITING,
    JS_APP_STATUS_EXITED,
    JS_APP_STATUS_RESTART,
};

#define MAIN_ENTRY_JS "app.js"

#define MAIN_PAGE_JS "/pages/index/index"
#define JEE_VERSION "1.0"
#define JEE_SYSTEMUI_APP "com.unisoc.jeeui"

LOCAL char g_app_guid[APP_ID_LENGTH] = {0};
LOCAL char g_app_para[APP_ID_LENGTH] = {0};
LOCAL char g_app_path[MAX_PATH] = {0};
LOCAL int g_app_status = JS_APP_STATUS_NULL;
LOCAL char g_restart_app_guid[APP_ID_LENGTH] = {0};

// LOCAL char g_approot[256] = { 0 };
LOCAL int g_art_init = 0;
LOCAL char *g_buildin_module_file[] = {"util\\helper.js", "util\\stack.js",
                                       "art\\art.js",     "timer\\timer.js",
                                       "mvvm\\mvvm.js",   "i18n\\i18n.js"};

LOCAL int js_art_run_root_page(char *path)
{
    if (NULL == path)
    {
        js_call_global_obj_function_string("router", "navigateTo",
                                           MAIN_PAGE_JS);
    }
    else
    {
        js_call_global_obj_function_string("router", "navigateTo", path);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////

LOCAL void update_current_work_path(char *path)
{
    jerry_value_t global_obj;
    char *full_dir = NULL;

    /* add __filename, __dirname */
    if (path)
    {
        full_dir = js_module_dirname(path);
        global_obj = jerry_get_global_object();
        if (path)
        {
            // alogi("workpath: __filename is %s ", path);
            jerry_jval_set_property_string_raw(global_obj, "__filename", path);
        }
        if (full_dir)
        {
            // alogi("workpath: __dirname is %s ", full_dir);
            jerry_jval_set_property_string_raw(global_obj, "__dirname",
                                               full_dir);
            art_free(full_dir);
        }
        jerry_release_value(global_obj);
    }
}

LOCAL void load_and_evalue_js_file(char *path)
{
    char *src = NULL;
    size_t bytes_read = 0;

    int fd = art_open_js_file(path,
                              (uint32)(SFS_MODE_READ | SFS_MODE_OPEN_EXISTING));
    if (-1 == fd)
    {
        aloge("art open  file %s  faild", path);
        return;
    }
    int file_size = art_get_js_file_size(fd);
    src = art_malloc(file_size + 1);
    memset(src, 0, file_size + 1);
    bytes_read = art_read_js_file(fd, src, file_size);
    art_close_js_file(fd);

    art_engine_runcode(path, src, bytes_read);
    art_free(src);
}

LOCAL void load_buildin_module()
{
    int count =
        sizeof(g_buildin_module_file) / sizeof(g_buildin_module_file[0]);
    int len = 0;

    for (int i = 0; i < count; i++)
    {
        char fullPath[MAX_PATH] = {0};
        snprintf(fullPath, MAX_PATH - 1, "%s%s", JEE_FWK_DIR,
                 g_buildin_module_file[i]);
        load_and_evalue_js_file(fullPath);
    }
}

LOCAL int js_art_uninit()
{
    if (1 == g_art_init)
    {
        art_uninit_binding();
        art_uninit_engine();
        art_uninit_memorypool();
        g_art_init = 0;
    }

    return 0;
}

LOCAL int js_art_run_app_lunch(char *parameter)
{
    char *src = "if(getApp()['onLaunch']!=null){getApp()['onLaunch']({"
                "parameter:'%s'});}else{console.log('call onLaunch failed!')}";
    char run[256] = {0};
    sprintf(run, src, parameter);
    return art_engine_call_js_func("onLaunch", run);
}

LOCAL int js_art_run_app_exit()
{
    art_engine_call_js_func("onExit", "if((getApp()!=null)&&(getApp()['onExit']"
                                      "!=null)){getApp()['onExit']();}");
    alogi("jsart_runExit onExit done!");

    return 0;
}

LOCAL int js_art_run_app_error()
{
    art_engine_call_js_func(
        "onError", "if(getApp()['onError']!=null){getApp()['onError']();}");
    alogi("art  js_art_run_app_error done!");
    return 0;
}

LOCAL int js_art_run_app(char *path, char *parameter)
{
    int ret = 0;
    do
    {
        int fd = 0;
        int file_size = 0;
        char *src = NULL;
        char app_js_path[256] = {0};

        ret = art_init_memorypool();
        if (0 == ret)
        {
            aloge("art art_init_memorypool error!");
            break;
        }

        strcat(app_js_path, path);
        strcat(app_js_path, MAIN_ENTRY_JS);
        fd = art_open_js_file(app_js_path,
                              (uint32)(SFS_MODE_READ | SFS_MODE_OPEN_EXISTING));
        if (0 == fd)
        {
            art_uninit_memorypool();
            aloge("art open file error! path = %s", app_js_path);
            break;
        }
        file_size = art_get_js_file_size(fd);
        alogd("art app.js file size =%d", file_size);
        src = art_malloc(file_size + 1);
        memset(src, 0, file_size + 1);
        ret = art_read_js_file(fd, src, file_size);
        art_close_js_file(fd);

        if (-1 == ret)
        {
            art_free(src);
            art_uninit_memorypool();
            aloge("art os_readfile error!");
            break;
        }

        g_app_status = JS_APP_STATUS_RUNNING;

        // init engine
        art_init_engine((JSART_MALLOC)art_malloc, (JSART_FREE)art_free);
        // START - DEBUUGER
#ifdef WIN32
        #if defined (JERRY_DEBUGGER) && (JERRY_DEBUGGER == 1)
        {
                // initial the debugger switch
                boolean startDebugger = false;
                uint8 buffer_len  = 128;
                // MALLOC FREE SPACE
                char* buffer =(char*)(art_calloc(buffer_len, sizeof(char)));
                // ual_kv_set_value("developer_mode", "TRUE");
                // BUFFER IS NOT NULL
                if(NULL != buffer)
                {
                        // READ THE KV
                        ual_kv_result_e ret = ual_kv_get_value(DEV_MODE_KEY, buffer, &buffer_len);
                        // READ THE KV SUCESS
                        if (UAL_KV_RESULT_SUCCESS == ret)
                        {
                                if(strcmp((char *)buffer, "TRUE") == 0)
                                {
                                        aloge("read debugger kv -is- true!");
                                        startDebugger = true;
                                }
                                art_free(buffer);
                        }
                        else if(UAL_KV_RESULT_KEY_NOT_EXIST == ret)
                        {
                                art_free(buffer);
                                aloge("read debugger kv - not exist!");
                        }
                        else
                        {
                                art_free(buffer);
                                aloge("read debugger kv - unknowerror!");
                        }
                }
                // start debugger
                if( startDebugger)
                {
                        jerryx_debugger_after_connect(
                                jerryx_debugger_tcp_create(JERRY_DEBUGGER_PORT)
                                && jerryx_debugger_ws_create());
                        // add for program flow
                        jerry_debugger_stop_at_breakpoint(true);
                }
        }
        #endif
#endif
        // init binding
        art_init_binding((JSART_MALLOC)art_malloc, (JSART_FREE)art_free);
        g_art_init = 1;
        // load core js
        load_buildin_module();

        // app: init context
        update_current_work_path(app_js_path);

        // app: init pages
        js_art_module_init_pages(g_app_guid);

        if (JS_APP_FEF == js_art_module_get_app_type())
        {
            // app: module reg to main.js
            js_art_module_reg();

            // app: run main.js
            js_art_run_main(src, file_size);

            // app: module unreg from main.js
            js_art_module_unreg();
        }
        else
        {
            art_engine_runcode("app.js", src, file_size);
        }

        // app: run onLunch
        js_art_run_app_lunch(parameter);

        if (JS_APP_FEF == js_art_module_get_app_type())
        {
            // app: run root page
            js_art_run_root_page(js_art_module_get_app_root_page());
        }
#ifdef WIN32
        #if defined (JERRY_DEBUGGER) && (JERRY_DEBUGGER == 1)
                jerry_debugger_stop_at_breakpoint(false);
        #endif
 #endif
        // app: uninit pages
        js_art_module_uninit_pages();

    } while (0);

    if (0 == ret)
    {
        js_art_uninit();
    }

    return 0;
}

LOCAL int perm_init_done_notify(int code)
{
    js_art_run_app(g_app_path, g_app_para);
    return 0;
}

PUBLIC int jee_get_app_guid(char *buff, int len)
{
    strncpy(buff, g_app_guid, len);
    return 0;
}

PUBLIC int js_art_get_app_path(char *buff, int len)
{
    strncpy(buff, g_app_path, len);
    return 0;
}

LOCAL void jeeMsg(char *msg)
{
    js_art_restart_app("com.unisoc.jeeui", msg);
}

PUBLIC int jee_start_app(char *guid, char *para)
{
    if (JS_APP_STATUS_RUNNING == g_app_status)
    {
        aloge("art app running.....");
        return JS_ART_RUNTIME_ERROR;
    }

    if (JS_APP_STATUS_EXITING == g_app_status)
    {
        aloge("art app exiting.....");
        return JS_ART_RUNTIME_ERROR;
    }

    if (PNULL != guid)
    {
        strcpy(g_app_guid, guid);
    }

    alogi("art try start  app :%s(%s)", g_app_guid, g_app_para);

    js_set_fwk_dir(JEE_FWK_DIR);
    pm_package_info_t pi = {0};
    int ret = js_get_app_info(guid, &pi);

    if (0 != ret)
    {
        char errmsg[64] = {0};
        sprintf(errmsg, "art  get packageinfo(%s) error:%d", g_app_guid, ret);
        aloge("%s", errmsg);

        jeeMsg(errmsg);
        return JS_ART_APPINFO_ERROR;
    }

    if (atoi(pi.api_version_compatible) > atoi(JEE_VERSION))
    {
        char errmsg[64] = {0};
        sprintf(errmsg, "app not compatible with version: %s", JEE_VERSION);
        aloge("%s", errmsg);

        jeeMsg(errmsg);
        return JS_ART_COMPATIBLE_ERROR;
    }

    strcpy(g_app_path, pi.app_install_path);
    strcat(g_app_path, "\\");

    if (NULL != para)
    {
        strcpy(g_app_para, para);
    }
    else
    {
        strcpy(g_app_para, pi.parameter);
    }

    alogi("art  start  app :%s(%s)", g_app_guid, g_app_para);
    if (strcmp(g_app_guid, JEE_SYSTEMUI_APP) == 0)
    {
        js_art_run_app(g_app_path, g_app_para);
    }
    else
    {
        //perm_grant_app_permissions(guid, perm_init_done_notify);
		perm_init_done_notify(0);
    }

    // release permission list
    listhead_clean(pi.p_permisson_list_head, pm_permisson_info_t,
                   permisson_info_node);

    // release pages list
    listhead_clean(pi.p_pages_list_head, pm_pages_info_t, page_info_node);

    return 0;
}

PUBLIC int js_art_restart_app(char *guid, char *para)
{
    if (JS_APP_STATUS_RUNNING == g_app_status)
    {
        js_art_run_app_exit();
    }
    g_app_status = JS_APP_STATUS_RESTART;
    strcpy(g_restart_app_guid, guid);
    if (NULL != para)
    {
        strcpy(g_app_para, para);
    }
    return 0;
}

PUBLIC int js_art_req_exit()
{
    if(g_app_status != JS_APP_STATUS_EXITING)
    {
        js_art_run_app_exit();
        g_app_status = JS_APP_STATUS_EXITING;
    }
    return 0;
}

PUBLIC int js_art_parse_error_notify()
{

    g_app_status = JS_APP_STATUS_EXITING;
    return 0;
}


//temp fix, bug2114788
LOCAL BOOLEAN g_force_exit_flag = FALSE;/*TRUE means js system error occurs,need exit js engine*/
PUBLIC BOOLEAN js_art_get_force_exit_flag(void)
{
    alogi("get g_force_exit_flag is %d", g_force_exit_flag);
    return g_force_exit_flag;
}

PUBLIC void js_art_set_force_exit_flag(BOOLEAN error_flag)
{
     g_force_exit_flag = error_flag;
     alogi("set g_force_exit_flag is %d", error_flag);
}

PUBLIC int jee_exit_app()
{
    if (JS_APP_STATUS_EXITING == g_app_status)
    {
        js_art_uninit();
        g_app_status = JS_APP_STATUS_EXITED;
        js_art_set_force_exit_flag(FALSE);
    }
    if (JS_APP_STATUS_RESTART == g_app_status)
    {
        js_art_uninit();
        g_app_status = JS_APP_STATUS_EXITED;
        jee_start_app(g_restart_app_guid, g_app_para);
    }
    return 0;
}


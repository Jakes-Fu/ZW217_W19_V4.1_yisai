/**
 * Copyright (c) 2015-2022 by UNISOC Technology(Shanghai),
 * Inc. ALL RIGHTS RESERVED.
 *
 * copyrighted works and confidential proprietary information of
 * UNISOC Technology(Shanghai), Inc. They may not be modified, copied,
 *reproduced, distributed, or disclosed to third parties in any manner, medium,
 *or form, in whole or in part, without the prior written consent of UNISOC
 *Technology(Shanghai), Inc.
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jerry_util.h"
#include "jerry_module.h"
#include "jerry_module_in.h"
#include "urjs_art.h"
#include <jerryscript-ext/module.h>
#include "ual_type.h"
#include "sfs.h"
#include "urjs_art_memory.h"

// #include <ecma-globals.h>

#ifdef WIN32
#ifdef USE_MINGW
#define snprintf(...) _snprintf(__VA_ARGS__)
#define vsnprintf(...) _vsnprintf(__VA_ARGS__)
#endif
#endif

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#define UR_FRAMEWORK_DIR_PATH_DEVICE "device"
#define UR_FRAMEWORK_DIR_PATH_CONSOLE "console"
#define UR_FRAMEWORK_DIR_PATH_ASSERT "assert"
#define UR_FRAMEWORK_DIR_PATH_ART "art"
#define UR_FRAMEWORK_DIR_PATH_GUI "gui"
#define UR_FRAMEWORK_DIR_PATH_TIMER "timer"
#define UR_FRAMEWORK_DIR_PATH_WATCHFACE "watchface"
#define UR_FRAMEWORK_DIR_PATH_PACKAGEMANAGER "packagemanager"
#define UR_FRAMEWORK_DIR_PATH_RESOURCELOAD "resourceload"
#define UR_FRAMEWORK_DIR_PATH_TESTFW "testfw"
#define UR_FRAMEWORK_DIR_PATH_STORAGE "storage"

#define UR_FRAMEWORK_DIR_PATH_AUDIO "audio"
#define UR_FRAMEWORK_DIR_PATH_KV "kv"
#define UR_FRAMEWORK_DIR_PATH_FEF "fef"
#define UR_FRAMEWORK_DIR_PATH_EXPORT "export"
#define UR_FRAMEWORK_DIR_PATH_FS "fs"
#define UR_FRAMEWORK_DIR_PATH_APP "app"
#define UR_FRAMEWORK_DIR_PATH_HTTP "http"

#define JS_EXT ".js"

LOCAL char *g_ur_framework_dir[] = {
    UR_FRAMEWORK_DIR_PATH_DEVICE,       UR_FRAMEWORK_DIR_PATH_CONSOLE,
    UR_FRAMEWORK_DIR_PATH_ASSERT,       UR_FRAMEWORK_DIR_PATH_ART,
    UR_FRAMEWORK_DIR_PATH_GUI,          UR_FRAMEWORK_DIR_PATH_TIMER,
    UR_FRAMEWORK_DIR_PATH_WATCHFACE,    UR_FRAMEWORK_DIR_PATH_AUDIO,
    UR_FRAMEWORK_DIR_PATH_KV,           UR_FRAMEWORK_DIR_PATH_FEF,
    UR_FRAMEWORK_DIR_PATH_EXPORT,       UR_FRAMEWORK_DIR_PATH_PACKAGEMANAGER,
    UR_FRAMEWORK_DIR_PATH_RESOURCELOAD, UR_FRAMEWORK_DIR_PATH_FS,
    UR_FRAMEWORK_DIR_PATH_APP,          UR_FRAMEWORK_DIR_PATH_RESOURCELOAD,
    UR_FRAMEWORK_DIR_PATH_TESTFW,       UR_FRAMEWORK_DIR_PATH_STORAGE,
    UR_FRAMEWORK_DIR_PATH_HTTP};

LOCAL char g_system_root_dir[PATH_MAX] = {0};

typedef jerry_value_t (*module_init_func_t)(void);

void js_set_fwk_dir(char *fwkdir)
{
    strcpy(g_system_root_dir, fwkdir);
}
char *js_module_dirname(char *path)
{
    size_t i;
    char *s = NULL;
    if (!path || !*path)
        return NULL;

    s = unisoc_strdup(path);
    if (!s)
        return NULL;

    i = strlen(s) - 1;
    for (; s[i] == '\\'; i--)
    {
        if (!i)
        {
            s[0] = '\\';
            goto __exit;
        }
    }
    for (; s[i] != '\\'; i--)
    {
        if (!i)
        {
            s[0] = '.';
            goto __exit;
        }
    }
    for (; s[i] == '\\'; i--)
    {
        if (!i)
        {
            s[0] = '\\';
            goto __exit;
        }
    }
__exit:
    s[i + 1] = 0;
    return s;
}

char *js_module_normalize_path(const char *directory, const char *filename)
{
    char *fullpath = NULL;
    char *dst0 = NULL;
    char *dst = NULL;
    char *src = NULL;
    char *temp = NULL;
    int i;
    int fullpath_len = 0;

    /* check parameters */
    if (filename == NULL || directory == NULL)
    {
        return NULL;
    }

    // Change Formate to windows
    temp = filename;
    for (i = 0; i < strlen(filename); i++)
    {
        if (temp[i] == '/')
        {
            temp[i] = '\\';
        }
    }
    fullpath_len = strlen(directory) + strlen(filename) + 2;
    fullpath = art_malloc(fullpath_len);
    if (fullpath == NULL)
    {
        return NULL;
    }
    memset(fullpath, 0, fullpath_len);

    /* join path and file name */
    snprintf(fullpath, fullpath_len, "%s\\%s", directory, filename);

    src = fullpath;
    dst = fullpath;
    dst0 = dst;
    while (1)
    {
        char c = *src;

        if (c == '.')
        {
            if (!src[1])
                src++; /* '.' and ends */
            else if (src[1] == '\\')
            {
                /* './' case */
                src += 2;

                while ((*src == '\\') && (*src != '\0'))
                    src++;
                continue;
            }
            else if (src[1] == '.')
            {
                if (!src[2])
                {
                    /* '..' and ends case */
                    src += 2;
                    goto up_one;
                }
                else if (src[2] == '\\')
                {
                    /* '../' case */
                    src += 3;

                    while ((*src == '\\') && (*src != '\0'))
                        src++;
                    goto up_one;
                }
            }
        }

        /* copy up the next '/' and erase all '/' */
        while ((c = *src++) != '\0' && c != '\\')
            *dst++ = c;

        if (c == '\\')
        {
            *dst++ = '\\';
            while (c == '\\')
                c = *src++;

            src--;
        }
        else if (!c)
            break;

        continue;

    up_one:
        dst--;
        if (dst < dst0)
        {
            art_free(fullpath);
            return NULL;
        }
        while (dst0 < dst && dst[-1] != '\\')
            dst--;
    }

    *dst = '\0';

    /* remove '/' in the end of path if exist */
    dst--;
    if ((dst != fullpath) && (*dst == '\\'))
        *dst = '\0';

    /* final check fullpath is not empty, for the special path of lwext "/.." */
    if ('\0' == fullpath[0])
    {
        fullpath[0] = '\\';
        fullpath[1] = '\0';
    }
    return fullpath;
}

static bool file_check(char *path)
{
    SFS_FIND_DATA_T  find_data = {0};
    SFS_HANDLE file_handle = NULL;
    wchar file_name_wstr[256] = {0};

    MMIAPICOM_StrToWstr((uint8*)path, file_name_wstr);
    file_handle = SFS_FindFirstFile(file_name_wstr, &find_data);
    if (0 != file_handle)
    {
        if (!(SFS_ATTR_DIR & find_data.attr))
        {
            SFS_FindClose (file_handle);
            return TRUE;
        }
    }
    SFS_FindClose (file_handle);
    return FALSE;
}

char *module_retry_path(char *workdir, char *module)
{
    int i = 0;
    int count = 0;
    int module_ext_len = 0;
    char *full_path = NULL;
    char *module_ext = NULL;
    int tick = SCI_GetTickCount();
    if (module == NULL)
    {
        return NULL;
    }
    // Maybe module is Full Path
    if (TRUE == file_check(module))
    {
        full_path = art_malloc(strlen(module) + 1);
        if (full_path)
        {
            memset(full_path, 0, strlen(module) + 1);
            memcpy(full_path, module, strlen(module));
            // alogi("full_path is %s",full_path);
        }
    }
    else
    {
        // Check Current work Path with module name
        full_path = js_module_normalize_path(workdir, module);
        // alogi("[full_path is %s, workdir is %s, module is %s]%s
        // %d\n",full_path, workdir, module, __FUNCTION__,__LINE__);
        if (FALSE == file_check(full_path))
        {
            module_ext_len = strlen(module) + strlen(JS_EXT) + 1;
            count = sizeof(g_ur_framework_dir) / sizeof(g_ur_framework_dir[0]);
            module_ext = NULL;
            if (full_path)
            {
                art_free(full_path);
                full_path = NULL;
            }
            module_ext = art_malloc(module_ext_len);
            if (module_ext)
            {
                memset(module_ext, 0, module_ext_len);
                memcpy(module_ext, module, strlen(module));
                memcpy(module_ext + strlen(module), JS_EXT, strlen(JS_EXT));
                // Check Current work Path with JS_EXT
                full_path = js_module_normalize_path(workdir, module_ext);
                if (FALSE == file_check(full_path))
                {
                    // Check Buildin Path
                    for (i = 0; i < count; i++)
                    {
                        if (full_path)
                        {
                            art_free(full_path);
                            full_path = NULL;
                        }
                        // Check Buildin Path with module name
                        int index = 0;
                        if (strstr(module, "@") == module)
                        {
                            index = 1;
                        }
                        char module_dir[256] = {0};
                        sprintf(module_dir, "%s%s", g_system_root_dir,
                                g_ur_framework_dir[i]);
                        full_path = js_module_normalize_path(module_dir,
                                                             module + index);
                        // alogi("full_path is %s, g_ur_framework_dir[i] is %s,
                        // module is
                        // %s",full_path,g_ur_framework_dir[i],module);
                        if (TRUE == file_check(full_path))
                        {
                            break;
                        }
                        else
                        {
                            if (full_path)
                            {
                                art_free(full_path);
                                full_path = NULL;
                            }
                            // Check Buildin Path with JS_EXT

                            full_path = js_module_normalize_path(
                                module_dir, module_ext + index);
                            // alogi("full_path is %s, g_ur_framework_dir[i] is
                            // %s, module_ext is
                            // %s",full_path,g_ur_framework_dir[i],module_ext);
                            if (TRUE == file_check(full_path))
                            {
                                break;
                            }
                            else
                            {
                                // aloge("module file load failed! path : %s,
                                // module_ext : %s", full_path,  module_ext);
                            }
                        }
                    }
                }
                art_free(module_ext);
                module_ext = NULL;
            }
        }
    }

    alogd("module_retry_path(%s): %dms", module, SCI_GetTickCount() - tick);
    return full_path;
}
/* load module from file system */
static bool load_module_from_filesystem(const jerry_value_t module_name,
                                        jerry_value_t *result)
{
    bool ret = false;
    char *str = NULL;
    char *module = js_value_to_string(module_name);

    char *dirname = NULL;
    int dirname_len = 0;

    jerry_value_t dirname_value = 0;
    jerry_value_t filename_value = 0;
    jerry_value_t global_obj = 0;

    char *full_path = NULL;
    char *full_dir = NULL;
    alogi("load_module_from_filesystem:%s", module);
    global_obj = jerry_get_global_object();
    dirname_value = jerry_jval_get_property(global_obj, "__dirname");

    jerry_string_t jdir_name = jerry_jval_as_string(dirname_value);
    dirname = jerry_string_data(&jdir_name);
    dirname_len = jerry_string_size(&jdir_name);

    alogi("current workdir : %s,required module:%s\n", dirname, module);
    full_path = module_retry_path(dirname, module);
    if (NULL != dirname && dirname_len != 0)
    {
        jerry_string_destroy(&jdir_name);
    }

    if (!full_path)
    {
        goto __exit;
    }

    jerry_string_t jfile_str = jerry_file_read_by_path(full_path);
    int len = jerry_string_size(&jfile_str);
    if (len == 0)
    {
        goto __exit;
    }
    str = jerry_string_data(&jfile_str);

    filename_value = jerry_jval_get_property(global_obj, "__filename");

    /* set new __filename and __dirname */
    full_dir = js_module_dirname(full_path);

    jerry_jval_set_property_string_raw(global_obj, "__dirname", full_dir);
    jerry_jval_set_property_string_raw(global_obj, "__filename", full_path);

    (*result) = jerry_eval((jerry_char_t *)str, len, false);
    if (jerry_value_is_error(*result))
    {
        // aloge("failed to evaluate JS\n");
        alogi("faild  load module '%s' from  %s", module, full_path);
    }
    else
    {
        ret = true;
    }

    /* restore __filename and __dirname */
    jerry_jval_set_property_jval(global_obj, "__dirname", dirname_value);
    jerry_jval_set_property_jval(global_obj, "__filename", filename_value);

__exit:
    if (full_dir)
        art_free(full_dir);
    if (full_path)
        art_free(full_path);

    jerry_release_value(global_obj);
    jerry_release_value(dirname_value);
    jerry_release_value(filename_value);

    art_free(module);
    jerry_string_destroy(&jfile_str);

    return ret;
}

/* load builtin module */
static bool load_module_from_builtin(const jerry_value_t module_name,
                                     jerry_value_t *result)
{
    bool ret = false;
    module_init_func_t module_init;

    jerry_string_t jmodule_name = jerry_jval_as_string(module_name);
    char *module = jerry_string_data(&jmodule_name);

    if (strcmp(module, "buffer") == 0)
    {
        module_init = jerry_init_buffer;
        *result = module_init();
        ret = true;
    }

    jerry_string_destroy(&jmodule_name);

    return ret;
}

static jerryx_module_resolver_t load_filesystem_resolver = {
    NULL, load_module_from_filesystem};

static jerryx_module_resolver_t load_builtin_resolver = {
    NULL, load_module_from_builtin};

static const jerryx_module_resolver_t *resolvers[] = {
    &load_builtin_resolver, &load_filesystem_resolver};

DECLARE_HANDLER(require)
{
    jerry_value_t result;
    char *module = NULL;

    JERRY_CHECK_ARGS(1, string);

    jerry_string_t module_name = JERRY_GET_ARG(0, string);

    module = jerry_string_data(&module_name);

    {
        /* make new module.exports */
        jerry_value_t global_obj = jerry_get_global_object();
        jerry_value_t modules_obj =
            jerry_jval_get_property(global_obj, "module");
        jerry_value_t exports_obj =
            jerry_jval_get_property(modules_obj, "exports");

        jerry_value_t module_exports_obj = jerry_create_object();
        jerry_jval_set_property_jval(modules_obj, "exports",
                                     module_exports_obj);
        jerry_release_value(module_exports_obj);

        result = jerryx_module_resolve(
            args_p[0], resolvers, sizeof(resolvers) / sizeof(resolvers[0]));
        if (jerry_value_is_error(result))
        {
            aloge(" failed require module '%s' ", module);

            jerry_release_value(result);

            /* create result with error */
            result = jerry_create_error(
                JERRY_ERROR_TYPE, (const jerry_char_t *)"Module not found");
        }

        /* restore the parent module.exports */
        jerry_jval_set_property_jval(modules_obj, "exports", exports_obj);

        jerry_release_value(global_obj);
        jerry_release_value(modules_obj);
        jerry_release_value(exports_obj);
    }

    jerry_string_destroy(&module_name);

    return result;
}

int js_module_init(void)
{
    jerry_value_t global_obj = jerry_get_global_object();
    jerry_value_t modules_obj = jerry_create_object();
    jerry_value_t exports_obj = jerry_create_object();

    jerry_jval_set_property_jval(modules_obj, "exports", exports_obj);
    jerry_jval_set_property_jval(global_obj, "module", modules_obj);

    REGISTER_GLOBAL_HANDLER(require);

    jerry_release_value(global_obj);
    jerry_release_value(modules_obj);
    jerry_release_value(exports_obj);

    return 0;
}

/**
 * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
 * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
 *
 * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
 * Licensed under the Unisoc General Software License V1;
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * Software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 * See the Unisoc General Software License v1 for more details.
 *
 **/

/**
 *****************************************************************************
 * @file     urjs_art_module.c
 * @brief    -
 * @author   qingjun.yu@unisoc.com
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 unisoc </center></h2>
 **/

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/

#include "urjs_art_module.h"
#include "packagemanager.h"
#include "urjs_art.h"
#include "js_list.h"
#include "sfs.h"

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

#define IMPORT_SECTON_NUM (2)
#define VIEW_OBJECT_SUFFIX "_ui_gen"

/**---------------------------------------------------------------------------*
**                         Data Structures                                   *
**---------------------------------------------------------------------------*/

typedef struct
{
    char **p_page;
    int size;
} js_art_pages_t;

typedef struct
{
    char guid[APP_ID_LENGTH];
    char root_path[MAX_PATH];
    js_art_pages_t pages;
} js_art_app_t;

typedef struct
{
    struct list_head node;
    char *import;
    char *script;
} js_art_module_t;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/

LOCAL js_art_app_t g_js_art_app = {0};

LOCAL LIST_HEAD(g_js_art_module_head);

LOCAL const char *g_js_art_page_import_temple = "\
import %s from \"%s\";\r\n\
";

LOCAL const char *g_js_art_page_obj_temple = "\
\"%s\":{ \r\n\
        \"logic\":%s, \r\n\
        \"view\":%s \r\n\
}";

LOCAL const char *g_js_art_page_script_temple = "\r\n\r\n\
(function(pl){ \r\n\
PageManager.Init(pl); \r\n\
})({\r\n\
%s \r\n\
}); \r\n\
";

LOCAL const char *g_js_art_router_import_temple = "\
\r\nimport{Router} from \"framework.js\";\r\n\
";

LOCAL const char *g_js_art_router_script_temple = "\
\r\nvar router = new Router();\r\n\
\r\n$global.router = router;\r\n\
";

LOCAL const char *g_js_art_pm_script_temple = "\
\r\nimport {PageManager} from \"framework.js\";\r\n\
";

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    get app type
//  Note:
//  Parameter:
//  Returns:        JS_APP_SRV/JS_APP_GUI/JS_APP_FEF
//****************************************************************************/

PUBLIC int js_art_module_get_app_type()
{
    if (g_js_art_app.pages.p_page != NULL)
    {
        return JS_APP_FEF;
    }
    else
    {
        return JS_APP_GUI;
    }
}

/*****************************************************************************/
//  Description:    get app root page
//  Note:
//  Parameter:
//  Returns:        the page string, like: /pages/index/index
//****************************************************************************/

PUBLIC char *js_art_module_get_app_root_page()
{
    if (g_js_art_app.pages.p_page != NULL)
    {
        return g_js_art_app.pages.p_page[0];
    }
    else
    {
        return NULL;
    }
}

/*****************************************************************************/
//  Description:    init app page list.
//  Note:
//  Parameter:      guid: app guid
//  Returns:        void
//****************************************************************************/

PUBLIC void js_art_module_init_pages(const char *guid)
{
    pm_package_info_t pi = {0};
    list_node *find_node = NULL;
    pm_pages_info_t *page_info = NULL;
    int len = 0;
    int ret = js_get_app_info(guid, &pi);
    if (-1 == ret)
    {
        return;
    }
    strcpy(g_js_art_app.guid, guid);
    strcpy(g_js_art_app.root_path, pi.app_install_path);

    if (pi.p_pages_list_head)
    {
        listnode_for_each(find_node, pi.p_pages_list_head)
        {
            len++;
        }
    }

    if (len > 0)
    {
        char **pl = art_malloc(len * sizeof(char *));
        if (pl != NULL)
        {
            char **head = pl;
            listnode_for_each(find_node, pi.p_pages_list_head)
            {
                page_info =
                    listnode_entry(find_node, pm_pages_info_t, page_info_node);
                int len = strlen(page_info->page_name) + 1;
                *(head) = art_malloc(len);
                memset(*(head), '\0', len);
                strncpy(*(head++), page_info->page_name,
                        strlen(page_info->page_name));
            }

            g_js_art_app.pages.p_page = pl;
            g_js_art_app.pages.size = len;
        }

        // release permission list
        listhead_clean(pi.p_permisson_list_head, pm_permisson_info_t,
                       permisson_info_node);

        // release pages list
        listhead_clean(pi.p_pages_list_head, pm_pages_info_t, page_info_node);
    }
    else
    {
        // this app have no pages
        alogi("this app (%s) have no pages", guid);
    }
}

/*****************************************************************************/
//  Description:    uninit app page list.
//  Note:
//  Parameter:
//  Returns:        void
//****************************************************************************/

PUBLIC void js_art_module_uninit_pages()
{
    int i = 0;

    if (g_js_art_app.pages.p_page != NULL)
    {
        for (i = 0; i < g_js_art_app.pages.size; i++)
        {
            if (g_js_art_app.pages.p_page[i] != NULL)
            {
                // free memory.
                art_free(g_js_art_app.pages.p_page[i]);
                g_js_art_app.pages.p_page[i] = NULL;
            }
        }

        art_free(g_js_art_app.pages.p_page);
        g_js_art_app.pages.size = 0;
        g_js_art_app.pages.p_page = NULL;
    }
}

/*****************************************************************************/
//  Description:    register module
//  Note:
//  Parameter:      import: import module string
//                  script: javascript code
//  Returns:        void
//****************************************************************************/

LOCAL void js_art_module_reg_low(char *import, char *script)
{
    js_art_module_t *module = art_calloc(1, sizeof(js_art_module_t));
    if (NULL == module)
    {
        alogi("art malloc fail!");
        return;
    }

    if (import != NULL)
    {
        module->import = art_calloc(1, strlen(import) + 1);
        strncpy(module->import, import, strlen(import));
    }

    if (script != NULL)
    {
        module->script = art_calloc(1, strlen(script) + 1);
        strncpy(module->script, script, strlen(script));
    }

    list_add_tail(&module->node, &g_js_art_module_head);
}

/*****************************************************************************/
//  Description:    register router
//  Note:
//  Parameter:
//
//  Returns:        void
//****************************************************************************/

LOCAL void js_art_module_reg_router()
{
    js_art_module_reg_low(g_js_art_router_import_temple,
                          g_js_art_router_script_temple);
}

/*****************************************************************************/
//  Description:    register packagemanger
//  Note:
//  Parameter:
//
//  Returns:        void
//****************************************************************************/

LOCAL void js_art_module_reg_pm()
{
    js_art_module_reg_low(g_js_art_pm_script_temple, NULL);
}

/*****************************************************************************/
//  Description:    check file exist
//  Note:
//  Parameter:      file_path
//
//  Returns:        TRUE: exist, FALSE: not exist
//****************************************************************************/

LOCAL BOOLEAN js_art_module_is_file_exist(char *file_path)
{
    int len = 0;
    char *file = NULL;
    int fd = -1;
    int i = 0;
    if (file_path == NULL)
    {
        return FALSE;
    }

    len += strlen(g_js_art_app.root_path);
    len += strlen(file_path);
    file = art_malloc(len + 1);
    if(PNULL == file)
    {
        alogi("art malloc fail!");
        return FALSE;
    }
    memset(file, 0, len + 1);
    strcat(file, g_js_art_app.root_path);
    // warning: file_path start with "./"
    strcat(file, file_path + 1);

    for (; i < len; i++)
    {
        if (file[i] == '/')
        {
            file[i] = '\\';
        }
    }

    fd = art_open_js_file(file,
                          (uint32)(SFS_MODE_READ | SFS_MODE_OPEN_EXISTING));
    if (-1 == fd)
    {
        art_free(file);
        return FALSE;
    }

    art_close_js_file(fd);
    art_free(file);
    return TRUE;
}

/*****************************************************************************/
//  Description:    register page module
//  Note:
//  Parameter:      pagelist: page list
//                  size: len of page list
//  Returns:
//****************************************************************************/

LOCAL void js_art_module_reg_page(char **pagelist, int size)
{
    char *page_obj = NULL;
    char *page_script = NULL;
    int len = 0;

    if (0 >= size)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        char *page = pagelist[i];
        if (page == NULL)
        {
            continue;
        }

        char *logic_obj = NULL;
        char *logic_path = NULL;
        char *view_obj = NULL;
        char *view_path = NULL;
        char *import = NULL;
        char *obj = NULL;
        char *page_obj_temp = NULL;
        int j = 0;
        BOOLEAN logic_exist = FALSE;
        BOOLEAN view_exist = FALSE;

        // logic object
        logic_obj = art_malloc(strlen(page) + 1);
        if(PNULL == logic_obj)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(logic_obj, 0, strlen(page) + 1);
        strncpy(logic_obj, page, strlen(page));
        for (j = 0; j < strlen(logic_obj); j++)
        {
            if (logic_obj[j] == '/')
                logic_obj[j] = '_';
        }
        // logic path: 5 = strlen(".")+strlen(".js")+1
        logic_path = art_malloc(strlen(page) + 5);
        if(PNULL == logic_path)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(logic_path, 0, strlen(page) + 5);
        strcat(logic_path, ".");
        strcat(logic_path, page);
        strcat(logic_path, ".js");
        if (js_art_module_is_file_exist(logic_path))
        {
            logic_exist = TRUE;
        }
        else
        {
            strcpy(logic_obj, "null");
        }

        // view obj
        view_obj = art_malloc(strlen(page) + 16);
        if(PNULL == view_obj)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(view_obj, 0, strlen(page) + 16);
        strncpy(view_obj, page, strlen(page));
        for (j = 0; j < strlen(view_obj); j++)
        {
            if (view_obj[j] == '/')
                view_obj[j] = '_';
        }
        strcat(view_obj, VIEW_OBJECT_SUFFIX);
        // view path
        view_path = art_malloc(strlen(page) + 8);
        if(PNULL == view_path)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(view_path, 0, strlen(page) + 8);
        strcat(view_path, ".");
        strncpy(view_path + 1, page, strrchr(page, '/') - page + 1);
        strcat(view_path, VIEW_OBJECT_SUFFIX);
        strcat(view_path, ".js");
        if (js_art_module_is_file_exist(view_path))
        {
            view_exist = TRUE;
        }
        else
        {
            strcpy(view_obj, "null");
        }

        // import
        len = strlen(g_js_art_page_import_temple) * IMPORT_SECTON_NUM
              + strlen(logic_obj) + strlen(logic_path) + strlen(view_obj)
              + strlen(view_path);
        import = art_malloc(len + 1);
        if(PNULL == import)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(import, 0, len + 1);
        if (logic_exist)
        {
            int len_logic = strlen(g_js_art_page_import_temple)
                            + strlen(logic_obj) + strlen(logic_path);
            char *logic_import = art_malloc(len_logic + 1);
            if(PNULL == logic_import)
            {
                alogi("art malloc fail!");
                return;
            }
            memset(logic_import, 0, len_logic + 1);
            sprintf(logic_import, g_js_art_page_import_temple, logic_obj,
                    logic_path);

            strcat(import, logic_import);
            art_free(logic_import);
        }
        if (view_exist)
        {
            int len_view = strlen(g_js_art_page_import_temple)
                           + strlen(view_obj) + strlen(view_path);
            char *view_import = art_malloc(len_view + 1);
            if(PNULL == view_import)
            {
                alogi("art malloc fail!");
                return;
            }
            memset(view_import, 0, len_view + 1);
            sprintf(view_import, g_js_art_page_import_temple, view_obj,
                    view_path);

            strcat(import, view_import);
            art_free(view_import);
        }

        js_art_module_reg_low(import, NULL);

        art_free(import);

        len = strlen(g_js_art_page_obj_temple) + strlen(page)
              + strlen(logic_obj) + strlen(view_obj);
        obj = art_malloc(len + 1);
        if(PNULL == obj)
        {
            alogi("art malloc fail!");
            return;
        }
        memset(obj, 0, len + 1);
        sprintf(obj, g_js_art_page_obj_temple, page, logic_obj, view_obj);
        if (i < size - 1)
        {
            strcat(obj, ",\r\n");
        }
        else
        {
            strcat(obj, "\r\n");
        }

        if (page_obj != NULL)
        {
            page_obj_temp = page_obj;
            len += strlen(page_obj);
            page_obj = art_malloc(len + 1);
            if(PNULL == page_obj)
            {
                alogi("art malloc fail!");
                return ;
            }
            memset(page_obj, 0, len + 1);
            strcat(page_obj, page_obj_temp);
            art_free(page_obj_temp);
        }
        else
        {
            page_obj = art_malloc(len + 1);
            if(PNULL == page_obj)
            {
                alogi("art malloc fail!");
                return;
            }
            memset(page_obj, 0, len + 1);
        }
        strcat(page_obj, obj);

        art_free(logic_obj);
        art_free(logic_path);
        art_free(view_obj);
        art_free(view_path);
        art_free(obj);
    }

    if(PNULL == page_obj)
    {
        alogi("art malloc fail!");
        return;
    }
    len = strlen(g_js_art_page_script_temple) + strlen(page_obj);
    page_script = art_malloc(len + 1);
    if(PNULL == page_script)
    {
        alogi("art malloc fail!");
        return;
    }
    memset(page_script, 0, len + 1);
    sprintf(page_script, g_js_art_page_script_temple, page_obj);
    js_art_module_reg_low(NULL, page_script);

    art_free(page_obj);
    page_obj = NULL;
    art_free(page_script);
    page_script = NULL;
};

/*****************************************************************************/
//  Description:    get module size
//  Note:
//  Parameter:
//
//  Returns:        size of modules
//****************************************************************************/

LOCAL int js_art_module_get_size()
{
    struct list_head *head = &g_js_art_module_head;
    js_art_module_t *md;
    int size = 0;
    while ((head = head->next) != &g_js_art_module_head)
    {
        md = (js_art_module_t *)(head);
        if (md->import != NULL)
        {
            size += strlen(md->import);
        }
        if (md->script != NULL)
        {
            size += strlen(md->script);
        }
    }

    return size;
}

/*****************************************************************************/
//  Description:    register module
//  Note:
//  Parameter:
//
//  Returns:
//****************************************************************************/

PUBLIC void js_art_module_reg()
{
    js_art_module_reg_router();
    js_art_module_reg_pm();
    if (g_js_art_app.pages.p_page != NULL)
    {
        js_art_module_reg_page(g_js_art_app.pages.p_page,
                               g_js_art_app.pages.size);
    }
}

/*****************************************************************************/
//  Description:    unregister module
//  Note:
//  Parameter:
//
//  Returns:
//****************************************************************************/

PUBLIC void js_art_module_unreg()
{
    struct list_head *head = &g_js_art_module_head;
    js_art_module_t *md;
    while (head->next != head)
    {
        md = (js_art_module_t *)(head->next);
        if (md->import != NULL)
        {
            art_free(md->import);
            md->import = NULL;
        }
        if (md->script != NULL)
        {
            art_free(md->script);
            md->script = NULL;
        }

        list_remove(md);
        art_free(md);
        md = NULL;
    }
}

/*****************************************************************************/
//  Description:    run main.js
//  Note:
//  Parameter:      src: javascript code
//                  len: length of javascript code
//  Returns:
//****************************************************************************/

PUBLIC void js_art_run_main(char *src, int len)
{
    struct list_head *head = &g_js_art_module_head;
    js_art_module_t *md = NULL;
    int size = js_art_module_get_size();
    char *main_js = art_calloc(1, size + len + 1);
    if(PNULL == main_js)
    {
        alogi("art malloc fail!");
        return;
    }
    // add import *... to main.js
    while ((head = head->next) != &g_js_art_module_head)
    {
        md = (js_art_module_t *)head;
        if (md->import != NULL)
        {
            strcat(main_js, md->import);
        }
    }

    // add app.js to main.js
    strcat(main_js, src);

    // add global script to main.js
    head = &g_js_art_module_head;
    while ((head = head->next) != &g_js_art_module_head)
    {
        md = (js_art_module_t *)head;
        if (md->script != NULL)
        {
            strcat(main_js, md->script);
        }
    }

    // run main.js
    art_engine_runcode("main.js", main_js, size + len);

    art_free(main_js);
}

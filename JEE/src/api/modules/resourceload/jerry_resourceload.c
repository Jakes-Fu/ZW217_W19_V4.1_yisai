/*
 * Copyright (c) 2015-2020 by UNISOC Technology(Shanghai), Inc.  ALL RIGHTS
 * RESERVED.
 *
 * copyrighted works and confidential proprietary information of
 * UNISOC Technology(Shanghai), Inc. They may not be modified, copied,
 * reproduced, distributed, or disclosed to third parties in any manner, medium,
 * or form, in whole or in part, without the prior written consent of UNISOC
 * Technology(Shanghai), Inc.
 */

/**
 *****************************************************************************
 * @file     jerry_resourceload.c
 * @brief    -
 * @author   ningbiao.huang
 * @version  V1.0.0
 * @date
 * @history
 * @note
 * detailed description:
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 unisoc </center></h2>
 **/

#include "jerry_resourceload.h"
#include "jerry_bindings.h"

#include "os_api.h"
#include "packagemanager.h"
//#include "pm_list.h"

#include "resourceload.h"

/*lint -e64 */

/**---------------------------------------------------------------------------*
 **                         Mocor Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         LOCAL Functions                                *
 **---------------------------------------------------------------------------*/

static uint8 *wstr_to_str(const wchar *src, uint8 *dst) {
  uint8 *return_str = dst;

  if (src == PNULL || PNULL == dst) {
    return return_str;
  }

  while (!(*src & 0xFF00) && '\0' != (*dst++ = *src++)) {
    ;
  }

  return return_str;
}

/**
 * list_size - get size of list
 * @head: the head of list.
 */
static int listnode_size(list_node *head) {
  int size = 0;
  list_node *find_node = NULL;

  if (PNULL == head) {
    return 0;
  }
  listnode_for_each(find_node, head) { size++; }
  return size;
}

static jerry_value_t create_property_value_string(jerry_char_t *_prop_name,
                                                  jerry_char_t *_prop_value) {
  if (NULL != _prop_value && strlen(_prop_value) > 0) {
    jerry_value_t object = jerry_create_object();

    jerry_value_t prop_name = jerry_create_string(_prop_name);
    jerry_value_t prop_value = jerry_create_string(_prop_value);
    jerry_release_value(jerry_set_property(object, prop_name, prop_value));
    jerry_release_value(prop_value);
    jerry_release_value(prop_name);

    return object;
  }
  return jerry_create_null();
}

/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/

DECLARE_HANDLER(js_get_all_resource_dir_info) {
  rl_found_files_t *files_info = NULL;
  list_node *files_list_head = NULL;
  list_node *find_node = NULL;
  jerry_value_t out_array = 0;
  int index = 0;
  int list_size = 0;

  SCI_TRACE_LOW(" js_get_all_resource_dir_info! \n");

  files_list_head = get_all_resource_dir_info();

  list_size = listnode_size(files_list_head);
  if (list_size <= 0) {
    SCI_TRACE_LOW(" return null ! \n");
    return jerry_create_null();
  }

  out_array = jerry_create_array(list_size);

  listnode_for_each(find_node, files_list_head) {
    jerry_value_t find_object;
    uint8 dir_name[256] = {0};
    files_info = listnode_entry(find_node, rl_found_files_t, node);

    wstr_to_str(files_info->found_file, dir_name);
    // build app info object
    find_object = create_property_value_string((jerry_char_t *)"dirName",
                                               (jerry_char_t *)dir_name);

    // add to array
    jerry_release_value(
        jerry_set_property_by_index(out_array, index, find_object));
    jerry_release_value(find_object);
    index++;

    // release
    listnode_del(find_node);
    SCI_FREE(files_info);
    files_info = NULL;
  }

  if (NULL != files_list_head) {
    SCI_FREE(files_list_head);
    files_list_head = NULL;
  }

  SCI_TRACE_LOW(" js_get_all_resource_dir_info  end! \n");
  return out_array;
}

void js_register_resourceload_functions() {
  /*lint -e516 */
  REGISTER_HANDLER(js_get_all_resource_dir_info);
}
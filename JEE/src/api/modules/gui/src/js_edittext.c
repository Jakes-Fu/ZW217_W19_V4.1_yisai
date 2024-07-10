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
  * Created on: 2022/03/01
  * Author: jie.yang@unisoc.com
  */

#include "jerry_bindings.h"

#include "ctrlwf_export.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "mmk_type.h"
#include "caf.h"
#include "ctrlbutton_export.h"
#include "guianim.h"
#include "ctrlbase.h"
#include "guiedit.h"



DECLARE_HANDLER(js_edittext_ctl_create) {
	if (JERRY_UNLIKELY(args_cnt < 6)) {
		return jerry_create_number(0);
	}
	jerry_value_t obj = JERRY_GET_ARG(0, object);
	MMI_WIN_ID_T win_id = jerry_get_number_value(args_p[1]);

	double x = jerry_get_number_value(args_p[2]);
	double y = jerry_get_number_value(args_p[3]);
	double w = jerry_get_number_value(args_p[4]);
	double h = jerry_get_number_value(args_p[5]);

	MMI_CONTROL_CREATE_T  create = { 0 };
	MMI_HANDLE_T handle = NULL;
	MMI_CTRL_ID_T ctrl_id = js_native_gui_lookupCtrlId();
	GUIEDIT_INIT_DATA_T  data = { 0 };
	CTRLBASE_OBJ_T* ctrl_ptr = NULL;
	data.both_rect.v_rect.left = x;
	data.both_rect.v_rect.top = y;
	data.both_rect.v_rect.right = x + w;
	data.both_rect.v_rect.bottom = y + h;
	data.type = GUIEDIT_TYPE_TEXT;
	data.str_max_len = 36;
	create.ctrl_id = ctrl_id;
	create.guid = SPRD_GUI_TEXTEDIT_ID;
	create.parent_win_handle = win_id;
	create.parent_ctrl_handle = NULL;
	create.init_data_ptr = &data;

	ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_CreateControl(&create);
	if (PNULL != ctrl_ptr)
	{
		handle = ctrl_ptr->handle;
	}

	CTRLBASE_SetUserData(handle, jerry_acquire_value(obj));



	return jerry_create_number(handle);
}



DECLARE_HANDLER(js_edittext_ctl_destroy) {
	if (JERRY_UNLIKELY(args_cnt < 1)) {
		return jerry_create_number(0);
	}
	int handle = jerry_get_number_value(args_p[0]);
	js_native_gui_releaseCtrlId(MMK_GetCtrlId(handle));
	jerry_release_value(CTRLBASE_GetUserData(handle));
	MMK_DestroyControl(handle);



	return jerry_create_number(0);
}


DECLARE_HANDLER(js_native_gui_editbox_set_text) {
	if (JERRY_UNLIKELY(args_cnt < 2)) {
		return jerry_create_number(0);
	}
	int handle = jerry_get_number_value(args_p[0]);
	jerry_string_t value = JERRY_GET_ARG(1, string);
	char* text = jerry_string_data(&value);
	int len = strlen(text);
	wchar* dst = NULL;
	wchar* buff = SCI_ALLOC_APP((len + 1) * sizeof(wchar));
	if (buff == NULL) {
		return jerry_create_number(-1);
	}
	dst = MMIAPICOM_StrToWstr(text, buff);
	GUIEDIT_SetString(handle, buff, len);
	MMK_UpdateScreen();
	SCI_FREE(buff);
	return jerry_create_number(0);

}
DECLARE_HANDLER(js_native_gui_editbox_get_text) {
	uint8               nameChar[64 + 1] = { 0 };
	MMI_STRING_T        mmiNameStr = { 0 };

	int handle = jerry_get_number_value(args_p[0]);
	GUIEDIT_GetString(handle, &mmiNameStr);
	if (0 < mmiNameStr.wstr_len)
	{
		MMI_WSTRNTOSTR(nameChar, 64, mmiNameStr.wstr_ptr, mmiNameStr.wstr_len, mmiNameStr.wstr_len);
	}
	return jerry_create_string(nameChar);
}






DECLARE_PUBLIC_HANDLER(js_native_gui_editbox_set_text_color) {
	JERRY_CHECK_ARGS(2, number, number);
	int handle = jerry_get_number_value(args_p[0]);
	int color = jerry_get_number_value(args_p[1]);

	GUIEDIT_SetFontColor(handle, color);

	return jerry_create_null();
}

void js_register_editbox_functions() {

	REGISTER_HANDLER(js_edittext_ctl_create);
	REGISTER_HANDLER(js_native_gui_editbox_get_text);
	REGISTER_HANDLER(js_native_gui_editbox_set_text);
	REGISTER_HANDLER(js_native_gui_editbox_set_text_color);

	REGISTER_HANDLER(js_edittext_ctl_destroy);

};

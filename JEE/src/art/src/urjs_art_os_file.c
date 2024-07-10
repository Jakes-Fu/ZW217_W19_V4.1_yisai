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


#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "urjs_art.h"
#include "ual_type.h"


int art_open_js_file(char* full_path_ptr, uint32 mode) {
#ifdef LOAD_JS_FROM_WIN32_FS
    FILE* fp = fopen(full_path_ptr, "rb");
    return fp;
#else
	int sfs_handle = 0;
	wchar file_name_wstr[256] = { 0 };

	MMIAPICOM_StrToWstr((uint8*)full_path_ptr, file_name_wstr);
	sfs_handle = SFS_CreateFile(file_name_wstr, mode, PNULL, PNULL);
	if (PNULL == sfs_handle) {
		//ART_LOG("SFS_CreateFile [%s]  ERR", full_path_ptr);
		return -1;
	}
	return sfs_handle;
#endif
}

int art_read_js_file(int fd, char* data_buf_ptr, int rsize) {
#ifdef LOAD_JS_FROM_WIN32_FS
	size_t bytes_read = fread(data_buf_ptr, 1u, rsize, fd);
	return bytes_read;
#else
	int rc = 0;
	if (SFS_ERROR_NONE != SFS_ReadFile(fd, data_buf_ptr, rsize, &rc, PNULL)) {
		//aloge("SFS_ReadFile   ERR");

		return -1;
	}
	return rc;
#endif
}

int art_get_js_file_size(int fd) {
	int file_size = 0;
#ifdef LOAD_JS_FROM_WIN32_FS
	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
#else
	SFS_GetFileSize(fd, &file_size);
#endif

	return file_size;
}

int art_close_js_file(int fd) {
#ifdef LOAD_JS_FROM_WIN32_FS
	fclose(fd);
#else
	SFS_CloseFile(fd);
#endif

	return 0;
}


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
#include "cfl_dlmalloc.h"
#include "urjs_art.h"

#define ART_MEMPOOL
//1024*1024
#define MEM_POOL_SIZE 456*1024

LOCAL int g_memPoolPtr = NULL;
LOCAL CFL_MSPACE_HANDLE g_memPoolHandle = NULL;

int art_init_memorypool() {
#ifdef ART_MEMPOOL
    SCI_ASSERT(NULL == g_memPoolPtr);
    g_memPoolPtr = SCI_ALLOC_APP(MEM_POOL_SIZE);
    SCI_ASSERT(NULL != g_memPoolPtr);
    SCI_ASSERT(NULL == g_memPoolHandle);
    g_memPoolHandle = CFL_CreateMspace(g_memPoolPtr, MEM_POOL_SIZE);
    SCI_ASSERT(NULL != g_memPoolHandle);
    return  g_memPoolHandle;
#else 
    return 1;
#endif
}


void* art_malloc(int size) {
#ifdef ART_MEMPOOL
    SCI_ASSERT(NULL != g_memPoolHandle);
    void* ret = CFL_MspaceMalloc(g_memPoolHandle, size);
    SCI_ASSERT(ret != NULL);
    return ret;
#else
    return  SCI_ALLOC_APP(size);
#endif
}

void* art_calloc(int num, int size) {
#ifdef ART_MEMPOOL
    SCI_ASSERT(NULL != g_memPoolHandle);
    void* ret = CFL_MspaceMalloc(g_memPoolHandle, num * size);
    SCI_ASSERT(ret != NULL);
    memset(ret, 0, num * size);
    return ret;
#else
    //return  SCI_CALLOC(num, size);
	return  SCI_ALLOC(num*size);
#endif
}

void art_free(void* ptr) {
#ifdef ART_MEMPOOL
	SCI_ASSERT(NULL != g_memPoolHandle);
	CFL_MspaceFree(g_memPoolHandle, ptr);  //return 
#else
	SCI_FREE(ptr);
    return;
#endif
}
char* art_strdup(char* str) {
	char* ret = art_malloc((strlen(str) + 1) * sizeof(char));
	memset(ret, 0, strlen(str) + 1);
	strcpy(ret, str);
	return ret;
}
int art_uninit_memorypool() {
#ifdef ART_MEMPOOL
	alogi("uninit_memorypool begin");
	SCI_ASSERT(NULL != g_memPoolHandle);
	alogi("try CFL_DestroyMspace ");
	CFL_DestroyMspace(g_memPoolHandle);
	alogi("try SCI_FREE ");
	SCI_FREE(g_memPoolPtr);
	alogi("SCI_FREE OK");
	g_memPoolPtr = NULL;

	g_memPoolHandle = NULL;
	alogi("uninit_memorypool done");
#endif
	return 0;
}

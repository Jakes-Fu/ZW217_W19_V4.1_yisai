#ifndef __DISK_API_H__
#define __DISK_API_H__
#include "sci_types.h"

typedef  void*   DISK_HANDLE_PTR;

typedef struct DISK_CONFIG_Tag
{
    const char* name;
    uint32  type;
    uint32  attribute;
}DISK_CONFIG_T;

PUBLIC DISK_HANDLE_PTR DISK_Create(uint8* start_mem_addr, uint32 mem_len, DISK_CONFIG_T* config); 

PUBLIC BOOLEAN DISK_Delete(DISK_HANDLE_PTR  disk_hanle_ptr); 

#endif
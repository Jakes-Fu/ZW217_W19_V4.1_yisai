

#ifndef RM_IO_DEFAULT_H
#define RM_IO_DEFAULT_H

#include "helix_types.h"
#include "helix_result.h"

UINT32 rm_io_default_read(void*  pUserRead, BYTE* pBuf, UINT32 ulBytesToRead);
void rm_io_default_seek(void* pUserRead, UINT32 ulOffset, UINT32 ulOrigin);

#endif /* #ifndef RM_IO_DEFAULT_H */

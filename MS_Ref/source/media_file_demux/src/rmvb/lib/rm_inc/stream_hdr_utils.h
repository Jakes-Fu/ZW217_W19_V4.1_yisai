

#ifndef STREAM_HDR_UTILS_H
#define STREAM_HDR_UTILS_H

#include "helix_types.h"
#include "helix_result.h"
#include "rm_memory.h"
#include "stream_hdr_structs.h"

#ifdef __cplusplus
extern "C" {
#endif

HX_RESULT rm_unpack_rule_map(BYTE**             ppBuf,
                             UINT32*            pulLen,
                             rm_malloc_func_ptr fpMalloc,
                             rm_free_func_ptr   fpFree,
                             void*              pUserMem,
                             rm_rule_map*       pMap);

void rm_cleanup_rule_map(rm_free_func_ptr fpFree,
                         void*            pUserMem,
                         rm_rule_map*     pMap);

HX_RESULT rm_unpack_multistream_hdr(BYTE**              ppBuf,
                                    UINT32*             pulLen,
                                    rm_malloc_func_ptr  fpMalloc,
                                    rm_free_func_ptr    fpFree,
                                    void*               pUserMem,
                                    rm_multistream_hdr* hdr);

void rm_cleanup_multistream_hdr(rm_free_func_ptr    fpFree,
                                void*               pUserMem,
                                rm_multistream_hdr* hdr);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef STREAM_HDR_UTILS_H */

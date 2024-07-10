

#ifndef RV30BACKEND_H__
#define RV30BACKEND_H__

#include "helix_types.h"
#include "helix_result.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

HX_RESULT _RV40toYUV420CustomMessage(UINT32 *msg_id, void *global);

HX_RESULT _RV40toYUV420HiveMessage(UINT32 *msg_id, void *global);

HX_RESULT _RV40toYUV420Transform    (
    UCHAR     *pRV20Packets,
    UCHAR     *pDecodedFrameBuffer,
    void      *pInputParams,
    void      *pOutputParams,
    void      *global
    );

HX_RESULT _RV40toYUV420Free(void *global);

HX_RESULT _RV40toYUV420Init(void *prv10Init, void **decoderState);

#ifdef __cplusplus
}
#endif

#endif /* RV30BACKEND_H__ */



#ifndef RV20BACKEND_H__
#define RV20BACKEND_H__

#include "helix_types.h"
#include "helix_result.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

  //HX_RESULT _RV20toYUV420CustomMessage(UINT32 *msg_id, void *global);
HX_RESULT _RV20toYUV420CustomMessage(void *msg_id, void *global);

HX_RESULT _RV20toYUV420HiveMessage(UINT32 *msg_id, void *global);

HX_RESULT _RV20toYUV420Transform    (
    UCHAR     *pRV10Packets,
    UCHAR     *pDecodedFrameBuffer,
    void      *pInputParams,
    void      *pOutputParams,
    void      *global
    );

HX_RESULT _RV20toYUV420Free(void *global);

HX_RESULT _RV20toYUV420Init(void *prv10Init, void **decoderState);

#ifdef __cplusplus
}
#endif

#endif /* RV20BACKEND_H__ */

#ifndef __QR_DECODE_I_H__
#define __QR_DECODE_I_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void *hero_qrdecode_open(int imgw, int imgh);

void hero_qrdecode_close(void *pHandle);

int hero_qrdecode_push_luma_img(void *pHandle, char *pImg, int imgW, int imgH);

int hero_qrdecode_decode(void *pHandle, char **payload, int *payloadsize);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif

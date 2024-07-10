/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_RINGBUF_H__
#define __AVC_RINGBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void* ringbuf_handle_t;

ringbuf_handle_t avc_ringbuf_create(int size);
int              avc_ringbuf_destroy(ringbuf_handle_t handle);
int              avc_ringbuf_peek(char *dst, int readlen, ringbuf_handle_t handle);
int              avc_ringbuf_clear(ringbuf_handle_t handle);
int              avc_ringbuf_getfreesize(ringbuf_handle_t handle);
int              avc_ringbuf_getdatasize(ringbuf_handle_t handle);
int              avc_ringbuf_write(ringbuf_handle_t handle, char *src, int writelen);
int              avc_ringbuf_read(char *dst, int readlen, ringbuf_handle_t handle);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_RINGBUF_H__ */

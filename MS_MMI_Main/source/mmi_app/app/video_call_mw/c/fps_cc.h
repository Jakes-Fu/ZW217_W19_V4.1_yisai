/*************************************************************
 *
 * This is a part of the Agora Media Framework Library.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __FPS_CC_H__
#define __FPS_CC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef void* fps_cc_handle_t;

/**
 * @brief create fps cc handle
 * 
 * @return fps_cc_handle_t null for fail, otherwise success
 */
fps_cc_handle_t fps_cc_create(void);

/**
 * @brief fps cc process
 * 
 * @param handle fps cc handle
 * @param current_jpeg_size current jpeg size in bytes
 * @param target_bps target bps
 * @return true for skip this jpeg frame, cannot send to network
 * @return false for send this jpeg to network
 */
bool fps_cc_avoid_trigger(fps_cc_handle_t handle, uint32_t current_jpeg_size, uint32_t target_bps);

/**
 * @brief fps cc destroy
 * 
 * @param handle 
 */
void fps_cc_destroy(fps_cc_handle_t handle);

#ifdef __cplusplus
}
#endif
#endif /* __FPS_CC_H__ */
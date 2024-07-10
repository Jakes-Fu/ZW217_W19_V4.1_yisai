/*************************************************************
 *
 * This is a part of the Agora Media Framework Library.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include <os_api.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sci_types.h>
#include "fps_cc.h"
#include "avc_platform.h"

#define AVG_JPEG_IN_FPS_CALC_INTERVAL_MSEC         (6000)
#define AVG_JPEG_IN_BPS_CALC_INTERVAL_MSEC         (2000)
#define AVG_JPEG_IN_DEFAULT_FPS                    (7)
#define AVG_JPEG_IN_DEFAULT_BPS                    (300 * 1000)

#define TAG                                        "[fps-cc]"
#define DIV_ROUND(divident, divider)               (((divident) + ((divider) >> 1)) / (divider))

typedef struct {
    bool      inited;

    uint32_t  avg_jpeg_in_fps;
    uint32_t  avg_jpeg_in_fps_calc_start_ts;
    uint32_t  jpeg_in_total_frames_in_calc_interval;
    uint32_t  fps;

    uint32_t  avg_jpeg_in_bps;
    uint32_t  avg_jpeg_in_bps_calc_start_ts;
    uint32_t  jpeg_in_total_bytes_in_calc_interval;
} fps_cc_t;

fps_cc_handle_t fps_cc_create(void)
{
    fps_cc_t *fps_cc = (fps_cc_t *)malloc(sizeof(fps_cc_t));
    assert(fps_cc);
    memset(fps_cc, 0, sizeof(fps_cc_t));
    return (fps_cc_handle_t)fps_cc;
}

static void __jpeg_in_fps_calc(fps_cc_t *fps_cc, uint32_t now)
{
    uint32_t elapsed_msec;

    fps_cc->jpeg_in_total_frames_in_calc_interval++;

    /* initialize */
    if (!fps_cc->inited) {
        fps_cc->avg_jpeg_in_fps_calc_start_ts = now;
        fps_cc->avg_jpeg_in_fps               = AVG_JPEG_IN_DEFAULT_FPS;
        fps_cc->fps                           = AVG_JPEG_IN_DEFAULT_FPS;
        return;
    }

    /* timestamp wrapped */
    if (fps_cc->avg_jpeg_in_fps_calc_start_ts > now) {
        fps_cc->avg_jpeg_in_fps_calc_start_ts         = now;
        fps_cc->jpeg_in_total_frames_in_calc_interval = 0;
        return;
    }

    /* calc fps */
    if (fps_cc->avg_jpeg_in_fps_calc_start_ts + AVG_JPEG_IN_FPS_CALC_INTERVAL_MSEC <= now) {
        elapsed_msec = now - fps_cc->avg_jpeg_in_fps_calc_start_ts;
        if (elapsed_msec >= AVG_JPEG_IN_FPS_CALC_INTERVAL_MSEC) {
            fps_cc->avg_jpeg_in_fps = DIV_ROUND((fps_cc->jpeg_in_total_frames_in_calc_interval * 1000), elapsed_msec);
            fps_cc->avg_jpeg_in_fps = MAX(fps_cc->avg_jpeg_in_fps, 1);
        }

        fps_cc->jpeg_in_total_frames_in_calc_interval = 0;
        fps_cc->avg_jpeg_in_fps_calc_start_ts         = now;
    }
}

static void __jpeg_in_bps_calc(fps_cc_t *fps_cc, uint32_t now, uint32_t current_jpeg_size)
{
    /* initialize */
    uint32_t elapsed_msec;

    fps_cc->jpeg_in_total_bytes_in_calc_interval += current_jpeg_size;

    if (!fps_cc->inited) {
        fps_cc->avg_jpeg_in_bps_calc_start_ts = now;
        fps_cc->avg_jpeg_in_bps               = AVG_JPEG_IN_DEFAULT_BPS;
        return;
    }

    /* timestamp wrapped */
    if (fps_cc->avg_jpeg_in_bps_calc_start_ts > now) {
        fps_cc->avg_jpeg_in_bps_calc_start_ts        = now;
        fps_cc->jpeg_in_total_bytes_in_calc_interval = 0;
        return;
    }

    /* calc bps */
    if (fps_cc->avg_jpeg_in_bps_calc_start_ts + AVG_JPEG_IN_BPS_CALC_INTERVAL_MSEC <= now) {
        elapsed_msec = now - fps_cc->avg_jpeg_in_bps_calc_start_ts;
        if (elapsed_msec >= AVG_JPEG_IN_BPS_CALC_INTERVAL_MSEC) {
            fps_cc->avg_jpeg_in_bps = DIV_ROUND((fps_cc->jpeg_in_total_bytes_in_calc_interval * 8000ULL), elapsed_msec);
            fps_cc->avg_jpeg_in_bps = MAX(fps_cc->avg_jpeg_in_bps, 1000 * 100);
        }

        fps_cc->jpeg_in_total_bytes_in_calc_interval = 0;
        fps_cc->avg_jpeg_in_bps_calc_start_ts        = now;
    }
}

static void __fps_cc_inited(fps_cc_t *fps_cc)
{
    fps_cc->inited = true;
}

static bool __fps_cc(fps_cc_t *fps_cc, uint32_t target_bps)
{
    uint32_t target_fps;
    bool skip = true;

    if (target_bps >= fps_cc->avg_jpeg_in_bps) {
        return false;
    }

    target_fps = DIV_ROUND((fps_cc->avg_jpeg_in_fps * target_bps), fps_cc->avg_jpeg_in_bps);
    target_fps = MAX(target_fps, 1);

    if (fps_cc->fps >= fps_cc->avg_jpeg_in_fps) {
        skip = false;
        fps_cc->fps -= fps_cc->avg_jpeg_in_fps;
    }

    fps_cc->fps += target_fps;
    if (skip) {
        LOGT(TAG, "fps cc avoid. target-bps=%u, target-fps=%d, actual-bps=%u, actual-fps=%d",
             target_bps, target_fps, fps_cc->avg_jpeg_in_bps, fps_cc->avg_jpeg_in_fps);
    }

    return skip;
}

bool fps_cc_avoid_trigger(fps_cc_handle_t handle, uint32_t current_jpeg_size, uint32_t target_bps)
{
    uint32_t now = SCI_GetTickCount();
    fps_cc_t *fps_cc = (fps_cc_t *)handle;
    if (NULL == fps_cc) {
        return true;
    }

    /* step1. calculate jpeg in avg fps in AVG_JPEG_IN_FPS_CALC_INTERVAL_MSEC */
    __jpeg_in_fps_calc(fps_cc, now);

    /* step2. calculate jpeg in avg bps in AVG_JPEG_IN_BPS_CALC_INTERVAL_MSEC*/
    __jpeg_in_bps_calc(fps_cc, now, current_jpeg_size);

    /* step3. set initilze flag*/
    __fps_cc_inited(fps_cc);

    /* step4. fps cc process */
    return __fps_cc(fps_cc, target_bps);
}

void fps_cc_destroy(fps_cc_handle_t handle)
{
    if (handle) {
        free(handle);
    }
}
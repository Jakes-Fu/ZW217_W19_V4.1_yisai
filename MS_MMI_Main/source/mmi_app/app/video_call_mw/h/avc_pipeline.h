/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#ifndef __AVC_PIPELINE_H__
#define __AVC_PIPELINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "avc_list_head.h"
#include "avc_platform.h"

typedef enum {
    AVC_PIPELINE_STOP = 0, /* must be zero, cannot change it. default status */
    AVC_PIPELINE_START,
} avc_pipeline_event_type_e;

typedef struct {
    avc_pipeline_event_type_e type;
    void                      *content;
} avc_pipeline_event_t;

struct avc_pipeline_node_t;
typedef int (*avc_pipeline_accept_data_cb)(struct avc_pipeline_node_t *pipeline,
                                           uint8_t *buffer, size_t len, avc_media_info_t *info);
typedef int (*avc_pipeline_accept_ctrl_cb)(struct avc_pipeline_node_t *pipeline,
                                           avc_pipeline_event_t *event);

typedef struct avc_pipeline_node_t {
    struct avc_pipeline_node_t  *self;
    avc_pipeline_accept_data_cb data;
    avc_pipeline_accept_ctrl_cb cmd;
    avc_list_head_t             link;
    avc_list_head_t             rear_list;
    char                        name[16];
} avc_pipeline_node_t;

int32_t avc_pipeline_node_init(avc_pipeline_node_t *node, avc_pipeline_accept_ctrl_cb cmd,
                               avc_pipeline_accept_data_cb data, const char *name);
int32_t avc_pipeline_connect(avc_pipeline_node_t *pre, avc_pipeline_node_t *rear);
int32_t avc_pipeline_disconnect(avc_pipeline_node_t *pre, avc_pipeline_node_t *rear);
int32_t avc_pipeline_clear(avc_pipeline_node_t *node);
int32_t avc_pipeline_push_data(avc_pipeline_node_t *node, uint8_t *buffer, size_t len, avc_media_info_t *info);
int32_t avc_pipeline_push_cmd(avc_pipeline_node_t *node, avc_pipeline_event_t *event);

#ifdef __cplusplus
}
#endif
#endif /* __AVC_PIPELINE_H__ */

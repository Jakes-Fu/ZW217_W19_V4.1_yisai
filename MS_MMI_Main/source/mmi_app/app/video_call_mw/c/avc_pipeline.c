/*************************************************************
 *
 * This is a part of the Agora Video Call Service SDK.
 * Copyright (C) 2021 Agora IO
 * All rights reserved.
 *
 *************************************************************/
#include "avc_pipeline.h"

#define TAG "[pipeline]"

int32_t avc_pipeline_node_init(avc_pipeline_node_t *node, avc_pipeline_accept_ctrl_cb cb_cmd,
                               avc_pipeline_accept_data_cb cb_data, const char *name)
{
    if (NULL == node) {
        LOGE(TAG, "param invalid. node=%p", node);
        return -1;
    }

    node->self = node;
    node->data = cb_data;
    node->cmd  = cb_cmd;
    snprintf(node->name, sizeof(node->name), "%s", name);
    avc_list_init(&node->link);
    avc_list_init(&node->rear_list);
    return 0;
}

int32_t avc_pipeline_connect(avc_pipeline_node_t *pre, avc_pipeline_node_t *rear)
{
    if (NULL == pre || NULL == rear) {
        LOGE(TAG, "param invalid. pre=%p, rear=%p", pre, rear);
        return -1;
    }

    avc_list_add(&rear->link, &pre->rear_list);
    LOGD(TAG, "linking %s --> %s", pre->name, rear->name);
    return 0;
}

int32_t avc_pipeline_disconnect(avc_pipeline_node_t *pre, avc_pipeline_node_t *rear)
{
    avc_pipeline_node_t *p;
    if (NULL == pre || NULL == rear) {
        LOGE(TAG, "param invalid. pre=%p, rear=%p", pre, rear);
        return -1;
    }

    avc_list_for_each_entry(p, &pre->rear_list, avc_pipeline_node_t, link) {
        if (p == rear) {
            avc_list_del(&p->link);
            break;
        }
    }

    return 0;
}

int32_t avc_pipeline_clear(avc_pipeline_node_t *pipeline)
{
    avc_pipeline_node_t *p, *n;
    if (NULL == pipeline) {
        LOGE(TAG, "param invalid. node=%p", pipeline);
    }

    avc_list_for_each_entry_safe(p, n, &pipeline->rear_list, avc_pipeline_node_t, link) {
        avc_list_del(&p->link);
    }

    return 0;
}

int32_t avc_pipeline_push_data(avc_pipeline_node_t *node, uint8_t *buffer, size_t len, avc_media_info_t* info)
{
    avc_pipeline_node_t *p;
    if (NULL == node || NULL == buffer) {
        LOGE(TAG, "param invalid. node=%s, buffer=%p, len=%d", node == NULL ? "NULL" : node->name, buffer, (int)len);
        return -1;
    }

    avc_list_for_each_entry(p, &node->rear_list, avc_pipeline_node_t, link) {
        LOGD(TAG, "push data. from %s --> %s", node->name, p->name);
        p->data(p, buffer, len, info);
    }

    return 0;
}

int32_t avc_pipeline_push_cmd(avc_pipeline_node_t *node, avc_pipeline_event_t *event)
{
    avc_pipeline_node_t *p;
    if (NULL == node || NULL == event) {
        LOGE(TAG, "param invalid. node=%p, event=%p", node, event);
        return -1;
    }

    avc_list_for_each_entry(p, &node->rear_list, avc_pipeline_node_t, link) {
        LOGD(TAG, "push cmd[%d]. from %s --> %s", event->type, node->name, p->name);
        p->cmd(p, event);
    }

    return 0;
}

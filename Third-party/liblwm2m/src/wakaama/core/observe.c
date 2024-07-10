/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Toby Jaffey - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/

/*
 Copyright (c) 2013, 2014 Intel Corporation

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

 David Navarro <david.navarro@intel.com>

*/

#include "internals.h"
#include <stdio.h>
//#include "osi_log.h"  //modify by xiameng.yang


#include "liblwm2m.h"



#ifdef LWM2M_CLIENT_MODE
static lwm2m_observed_t * prv_findObserved(lwm2m_context_t * contextP,
                                           lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;

    targetP = contextP->observedList;
    while (targetP != NULL
        && (targetP->uri.objectId != uriP->objectId
         || targetP->uri.flag != uriP->flag
         || (LWM2M_URI_IS_SET_INSTANCE(uriP) && targetP->uri.instanceId != uriP->instanceId)
         || (LWM2M_URI_IS_SET_RESOURCE(uriP) && targetP->uri.resourceId != uriP->resourceId)))
    {
        targetP = targetP->next;
    }

    return targetP;
}

static void prv_unlinkObserved(lwm2m_context_t * contextP,
                               lwm2m_observed_t * observedP)
{
    if (contextP->observedList == observedP)
    {
        contextP->observedList = contextP->observedList->next;
    }
    else
    {
        lwm2m_observed_t * parentP;

        parentP = contextP->observedList;
        while (parentP->next != NULL
            && parentP->next != observedP)
        {
            parentP = parentP->next;
        }
        if (parentP->next != NULL)
        {
            parentP->next = parentP->next->next;
        }
    }
}

static lwm2m_watcher_t * prv_findWatcher(lwm2m_observed_t * observedP,
                                         lwm2m_server_t * serverP)
{
    lwm2m_watcher_t * targetP;

    targetP = observedP->watcherList;
    while (targetP != NULL
        && targetP->server != serverP)
    {
        targetP = targetP->next;
    }

    return targetP;
}

static lwm2m_watcher_t * prv_getWatcher(lwm2m_context_t * contextP,
                                        lwm2m_uri_t * uriP,
                                        lwm2m_server_t * serverP)
{
    lwm2m_observed_t * observedP;
    bool allocatedObserver;
    lwm2m_watcher_t * watcherP;

    allocatedObserver = false;

    observedP = prv_findObserved(contextP, uriP);
    if (observedP == NULL)
    {
        observedP = (lwm2m_observed_t *)lwm2m_malloc(sizeof(lwm2m_observed_t));
        if (observedP == NULL) return NULL;
        allocatedObserver = true;
        memset(observedP, 0, sizeof(lwm2m_observed_t));
        memcpy(&(observedP->uri), uriP, sizeof(lwm2m_uri_t));
        observedP->next = contextP->observedList;
        contextP->observedList = observedP;
        if (uriP->objectId == LWM2M_FOTA_UPDATE_OBJECT_ID && uriP->instanceId == 0)
        {
            contextP->fota_upgrade_observed = observedP;
        }
    }

    watcherP = prv_findWatcher(observedP, serverP);
    if (watcherP == NULL)
    {
        watcherP = (lwm2m_watcher_t *)lwm2m_malloc(sizeof(lwm2m_watcher_t));
        if (watcherP == NULL)
        {
            if (allocatedObserver == true)
            {
                lwm2m_free(observedP);
            }
            return NULL;
        }
        memset(watcherP, 0, sizeof(lwm2m_watcher_t));
        watcherP->active = false;
        watcherP->server = serverP;
        watcherP->next = observedP->watcherList;
        observedP->watcherList = watcherP;
    }

    return watcherP;
}

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
void observe_cancel_onenet(lwm2m_context_t * contextP, lwm2m_uri_t* uriP, void * fromSessionH)
{
    lwm2m_observed_t * observedP = NULL;
    lwm2m_watcher_t * watcherP = NULL;

    LOG_URI(uriP);

    observedP = prv_findObserved(contextP, uriP);
    if(observedP != NULL)
    {
        if (lwm2m_session_is_equal(observedP->watcherList->server->sessionH, fromSessionH, contextP->userData))
        {
            watcherP = observedP->watcherList;
            observedP->watcherList = observedP->watcherList->next;
        }
        else
        {
            lwm2m_watcher_t * parentP = observedP->watcherList;

            while (parentP->next != NULL && !lwm2m_session_is_equal(parentP->next->server->sessionH, fromSessionH, contextP->userData))
                parentP = parentP->next;

            if (parentP->next != NULL)
            {
                watcherP = parentP->next;
                parentP->next = parentP->next->next;
            }
        }

        if (watcherP != NULL)
        {
            if (watcherP->parameters != NULL)
            {
                lwm2m_free(watcherP->parameters);
                watcherP->parameters = NULL;
            }

            if(watcherP->reportData != NULL)
            {
                lwm2m_data_free(watcherP->reportDataCount, watcherP->reportData);
                watcherP->reportData = NULL;
                watcherP->reportDataCount = 0;
            }

            lwm2m_free(watcherP);

            if (observedP->watcherList == NULL)
            {
                prv_unlinkObserved(contextP, observedP);
                lwm2m_free(observedP);
            }
        }
    }

    return;
}

coap_status_t observe_asynAckNodata(lwm2m_context_t * context, st_request_t* request, uint8_t result)
{
    lwm2m_observed_t *observedP = NULL;
    lwm2m_watcher_t  *watcherP = NULL;
    lwm2m_uri_t* uriP = &request->uri;
    coap_packet_t packet[1];
    SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
    if (request->tokenLen == 0)
    {
        SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
        return COAP_500_INTERNAL_SERVER_ERROR;
    }
    SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
    coap_init_message(packet, COAP_TYPE_ACK, result, (uint16_t)request->mid);
    coap_set_header_token(packet, request->token, request->tokenLen);

    if(request->type == CALLBACK_TYPE_OBSERVE)
    {
        if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) 
        {
            SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
            return COAP_400_BAD_REQUEST;
        }

        if(result == COAP_205_CONTENT)
        {
            watcherP = prv_getWatcher(context, uriP, request->serverP);
            if (watcherP == NULL)
                {
                SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
                return COAP_500_INTERNAL_SERVER_ERROR;
                }

            watcherP->active = true;
            watcherP->format = LWM2M_CONTENT_TEXT;
            watcherP->watcherMid = request->mid;
            watcherP->tokenLen = request->tokenLen;
            memcpy(watcherP->token, request->token, request->tokenLen);
            watcherP->lastTime = lwm2m_gettime();
            coap_set_header_observe(packet, watcherP->counter++);
        }
    }
    else if(request->type == CALLBACK_TYPE_OBSERVE_CANCEL)
    {
        observedP = prv_findObserved(context, uriP);
        if (observedP == NULL)
            {
            SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
            return COAP_500_INTERNAL_SERVER_ERROR;
            }

        watcherP = prv_findWatcher(observedP, request->serverP);
        if (watcherP == NULL)
            {
            SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
            return COAP_500_INTERNAL_SERVER_ERROR;
            }

        observe_cancel_onenet(context, uriP, request->serverP->sessionH);
    }
    else if(request->type == CALLBACK_TYPE_OBSERVE_PARAMS)
    {
        SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
        return COAP_400_BAD_REQUEST;
    }
    else
    {
        SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
        return COAP_400_BAD_REQUEST;
    }
    SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
    message_send(context, packet, request->serverP->sessionH);

    return COAP_NO_ERROR;
}

static void prv_handleNotifySendReply_onenet(lwm2m_transaction_t * transacP,
                                              void * message)
{
    coap_packet_t * packet = (coap_packet_t *)message;
    lwm2m_context_t * targetP = (lwm2m_context_t *)(transacP->userData);


    if (packet != NULL && packet->type == COAP_TYPE_ACK)
    {
        lwm2mPostEvent(targetP->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_SENT_SUCCESS, packet->mid, 0);

        LOG("NotifySend successful");
    }
    else
    {
        if (packet == NULL)
            lwm2mPostEvent(targetP->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_SENT_TIMEOUT, 0, 0);
        else
            lwm2mPostEvent(targetP->ref, DATA_SEND_IND, LWM2M_EVENT_NOTIFY_FAILED, packet->mid, packet->type);

        LOG("NotifySend failed");
    }
}

coap_status_t observe_asynReport(lwm2m_context_t * context, lwm2m_observed_t *observeP, uint16_t msgid, uint8_t result)
{
    lwm2m_watcher_t* watcherP = NULL;
    lwm2m_media_type_t formatP;
    uint8_t* bufferP;
    int32_t length;
    lwm2m_data_t* data ;
    uint16_t count ;

    // get the watcher
    for (watcherP = observeP->watcherList; watcherP != NULL ; watcherP = watcherP->next)
    {
        if(watcherP->watcherMid == msgid)
            break;
    }
    if(watcherP == NULL)
    {
        return COAP_404_NOT_FOUND;
    }

    data = watcherP->reportData;
    count = watcherP->reportDataCount;

    if (data != NULL)
    {
        formatP = watcherP->format;
        length = lwm2m_data_serialize_onenet(&observeP->uri, count, data, &formatP, &bufferP);
        if (length <= -1) 
        {
            LOG("ERROR:observe report data serialize failed");
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }
    else
    {
        LOG("ERROR:observe report data is null");
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    if(context->notifyMode == NOTIFY_NON)
    {
        coap_packet_t packet[1];
        coap_init_message(packet, COAP_TYPE_ACK, COAP_205_CONTENT, 0);
        coap_set_header_content_type(packet, formatP);
        coap_set_payload(packet, bufferP, length);
        watcherP->lastTime = lwm2m_gettime();
        watcherP->lastMid = context->nextMID++;
        packet->mid = watcherP->lastMid;
        coap_set_header_token(packet, watcherP->token, watcherP->tokenLen);
        coap_set_header_observe(packet, watcherP->counter++);
        message_send(context, packet, watcherP->server->sessionH);
    }
    else
    {
        lwm2m_transaction_t *transaction;
        watcherP->lastTime = lwm2m_gettime();
        watcherP->lastMid = context->nextMID++;
        transaction = transaction_new(watcherP->server->sessionH, COAP_205_CONTENT, NULL, NULL, watcherP->lastMid, watcherP->tokenLen, watcherP->token);
        if (transaction == NULL)
        {
            lwm2m_free(bufferP);
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
        coap_set_header_content_type(transaction->message, formatP);
        coap_set_payload(transaction->message, bufferP, length);
        coap_set_header_observe(transaction->message, watcherP->counter++);
        transaction->callback = prv_handleNotifySendReply_onenet;
        transaction->userData = context;
        context->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(context->transactionList, transaction);
        if (transaction_send(context, transaction) != 0)
        {
            lwm2m_free(bufferP);
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
    }


    lwm2m_free(bufferP);

    return COAP_NO_ERROR;
}

coap_status_t observe_handleRequest_onenet(lwm2m_context_t * contextP,
                              lwm2m_uri_t * uriP,
                              lwm2m_server_t * serverP,
                              coap_packet_t * message,
                              coap_packet_t * response)
{
    lwm2m_observed_t *observedP = NULL;
    lwm2m_watcher_t  *watcherP = NULL;
    uint32_t count;
    uint16_t instanceId;
    uint16_t resourceId;
    bool isObserveRepeat;

    LOG_ARG("Code: %02X, server status: %s", message->code, STR_STATUS(serverP->status));
    LOG_URI(uriP);

    coap_get_header_observe(message, &count);
    LOG_ARG("count: %d, uriP->flag: 0x%x", count, uriP->flag);

    //uint16_t instanceId;
    //uint16_t resourceId;
    switch (count)
    {
        case 0:
        {
            if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) 
                return COAP_400_BAD_REQUEST;
            if (message->token_len == 0) 
                return COAP_400_BAD_REQUEST;

            instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
            resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;

            if (object_isStdObject(uriP->objectId))
                return COAP_405_METHOD_NOT_ALLOWED;
            
            isObserveRepeat = packet_asynFindObserveRequest(contextP, message->mid);
            if (isObserveRepeat)
            {
                LOG_ARG("repeat observe request %d", message->mid);
            }
            else
            {
                packet_asynPushRequest(contextP, serverP, message, CALLBACK_TYPE_OBSERVE);
                LOG_ARG("new observe request %d", message->mid);
            }

            lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_IND, coap_get_mid(),
                    (uint32_t)uriP->objectId<<16 | instanceId, (uint32_t)resourceId<<16 | !count);
            
            return COAP_IGNORE;
        }
        case 1:
        {
            // cancellation
            instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
            resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;

            observedP = prv_findObserved(contextP, uriP);
            if (observedP == NULL)
                return COAP_404_NOT_FOUND;

            watcherP = prv_findWatcher(observedP, serverP);
            if (watcherP == NULL)
                return COAP_404_NOT_FOUND;

            packet_asynPushRequest(contextP, serverP, message, CALLBACK_TYPE_OBSERVE_CANCEL);

            lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_IND, coap_get_mid(),
                        (uint32_t)uriP->objectId<<16 | instanceId, (uint32_t)resourceId<<16 | !count);
            return COAP_IGNORE;
        }
        default:
            return COAP_400_BAD_REQUEST;
    }
}

coap_status_t observe_setParameters_onenet(lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_server_t *serverP, lwm2m_attributes_t * attrP, coap_packet_t* message)
{
#define _PRV_BUFFER_SIZE 32

    lwm2m_watcher_t * watcherP = NULL;
    uint16_t instanceId ;
    uint16_t resourceId;
    setParam_t *setparam;

	LOG_ARG("toSet: %08X, toClear: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
		attrP->toSet, attrP->toClear, attrP->minPeriod, attrP->maxPeriod, attrP->greaterThan, attrP->lessThan, attrP->step);
    LOG_URI(uriP);

	if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) 
	{
		return COAP_400_BAD_REQUEST;
	}
    
    watcherP = prv_getWatcher(contextP, uriP, serverP);
	if (watcherP == NULL)
	{
		return COAP_500_INTERNAL_SERVER_ERROR;
	}
    
	// Check rule “lt” value + 2*”stp” values < “gt” value
	if ((((attrP->toSet | (watcherP->parameters?watcherP->parameters->toSet:0)) & ~attrP->toClear) & ATTR_FLAG_NUMERIC) == ATTR_FLAG_NUMERIC)
	{
		float gt;
		float lt;
		float stp;

		if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN))
		{
			gt = (float)attrP->greaterThan;
		}
		else
		{
			gt = (float)(watcherP->parameters?watcherP->parameters->greaterThan:0);
		}
		if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN))
		{
			lt = (float)attrP->lessThan;
		}
		else
		{
			lt = (float)(watcherP->parameters?watcherP->parameters->lessThan:0);
		}
		if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_STEP))
		{
			stp = (float)attrP->step;
		}
		else
		{
			stp = (float)(watcherP->parameters?watcherP->parameters->step:0);
		}

		if (lt + (2 * stp) >= gt) 
		{
			return COAP_400_BAD_REQUEST;
		}
	}

	if (watcherP->parameters == NULL)
	{
		if (attrP->toSet != 0)
		{
			watcherP->parameters = (lwm2m_attributes_t *)lwm2m_malloc(sizeof(lwm2m_attributes_t));
			if (watcherP->parameters == NULL) 
			{
				return COAP_500_INTERNAL_SERVER_ERROR;
			}
			memcpy(watcherP->parameters, attrP, sizeof(lwm2m_attributes_t));
		}
	}
	else
	{
		watcherP->parameters->toSet &= ~attrP->toClear;
		if (attrP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
		{
			watcherP->parameters->minPeriod = attrP->minPeriod;
		}
		if (attrP->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD)
		{
			watcherP->parameters->maxPeriod = attrP->maxPeriod;
		}
		if (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN)
		{
			watcherP->parameters->greaterThan = attrP->greaterThan;
		}
		if (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN)
		{
			watcherP->parameters->lessThan = attrP->lessThan;
		}
		if (attrP->toSet & LWM2M_ATTR_FLAG_STEP)
		{
			watcherP->parameters->step = attrP->step;
		}
	}

    if(watcherP->parameters){
    	LOG_ARG("Final toSet: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
    		watcherP->parameters->toSet, watcherP->parameters->minPeriod, watcherP->parameters->maxPeriod, watcherP->parameters->greaterThan, watcherP->parameters->lessThan, watcherP->parameters->step);
    }

    instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
    resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;

    setparam = lwm2m_malloc(sizeof(setParam_t));
    if(setparam == NULL)
        return COAP_500_INTERNAL_SERVER_ERROR;
    setparam->objid = uriP->objectId;
    setparam->insid = instanceId;
    setparam->resid = resourceId;
    setparam->param = lwm2m_malloc(256);
    if(setparam->param == NULL)
    {
        free(setparam);
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    sprintf((char *)setparam->param,"pmin=%lu; pmax=%lu; gt=%f; lt=%f; stp=%f",
                            watcherP->parameters->minPeriod,
                            watcherP->parameters->maxPeriod,
                            watcherP->parameters->greaterThan,
                            watcherP->parameters->lessThan,
                            watcherP->parameters->step);
    lwm2mPostEvent(contextP->ref, OBJ_SETPARAM_IND, coap_get_mid(),(uint32_t)setparam, 0);
    packet_asynPushRequest(contextP, serverP, message, CALLBACK_TYPE_OBSERVE_PARAMS);

	return COAP_IGNORE;
}

#endif

uint8_t observe_handleRequest(lwm2m_context_t * contextP,
                              lwm2m_uri_t * uriP,
                              lwm2m_server_t * serverP,
                              coap_packet_t * message,
                              coap_packet_t * response)
{
    lwm2m_observed_t * observedP;
    lwm2m_watcher_t * watcherP;
    uint32_t count;
    uint16_t instanceId;
    uint16_t resourceId;

    LOG_ARG("Code: %02X, server status: %s", message->code, STR_STATUS(serverP->status));
    LOG_URI(uriP);

    coap_get_header_observe(message, &count);
    LOG_ARG("count: %d, uriP->flag: 0x%x", count, uriP->flag);

    
    switch (count)
    {
        case 0:
        {
            if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) 
                return COAP_400_BAD_REQUEST;
            if (message->token_len == 0) 
                return COAP_400_BAD_REQUEST;
    
            instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
            resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;
    
            watcherP = prv_getWatcher(contextP, uriP, serverP);
            if (watcherP == NULL)
                return COAP_500_INTERNAL_SERVER_ERROR;
    
            if (uriP->objectId == LWM2M_FOTA_UPDATE_OBJECT_ID && uriP->instanceId == 0)
            {
                contextP->fota_upgrade_observed->watcherList = watcherP;
            }
    
            watcherP->tokenLen = message->token_len;
            memcpy(watcherP->token, message->token, message->token_len);
            watcherP->active = true;
            watcherP->lastTime = lwm2m_gettime();
            if (IS_OPTION(message, COAP_OPTION_ACCEPT))
            {
                watcherP->format = utils_convertMediaType(message->accept[0]);
            }
            else if (IS_OPTION(message, COAP_OPTION_CONTENT_TYPE))
            {
                watcherP->format = utils_convertMediaType(message->content_type);
            }
            else
            {
                watcherP->format = LWM2M_CONTENT_TLV;
            }
            if (uriP->objectId == LWM2M_FOTA_UPDATE_OBJECT_ID && uriP->instanceId == 0 && uriP->resourceId == 0)
            {
                watcherP->format = LWM2M_CONTENT_OPAQUE;
            }
    
            coap_set_header_observe(response, watcherP->counter++);
            
            lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_IND, coap_get_mid(),
                        (uint32_t)uriP->objectId<<16 | instanceId, (uint32_t)resourceId<<16 | !count);
    
            return COAP_205_CONTENT;
        }
        case 1:
        {
            // cancellation
            instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
            resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;

            observedP = prv_findObserved(contextP, uriP);
            if(observedP)
            {
                watcherP = prv_findWatcher(observedP, serverP);
                if(watcherP)
                {
                    observe_cancel(contextP, watcherP->lastMid, serverP->sessionH);
                }
            }
            lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_IND, coap_get_mid(),
                        (uint32_t)uriP->objectId<<16 | instanceId, (uint32_t)resourceId<<16 | !count);
            
            return COAP_205_CONTENT;
        }
        default:
            return COAP_400_BAD_REQUEST;
    }
}

void observe_cancel(lwm2m_context_t * contextP,
                    uint16_t mid,
                    void * fromSessionH)
{
    lwm2m_observed_t * observedP;

    LOG_ARG("mid: %d", mid);

    for (observedP = contextP->observedList;
         observedP != NULL;
         observedP = observedP->next)
    {
        lwm2m_watcher_t * targetP = NULL;

        if ((LWM2M_MAX_ID == mid || observedP->watcherList->lastMid == mid)
         && lwm2m_session_is_equal(observedP->watcherList->server->sessionH, fromSessionH, contextP->userData))
        {
            targetP = observedP->watcherList;
            observedP->watcherList = observedP->watcherList->next;
        }
        else
        {
            lwm2m_watcher_t * parentP;

            parentP = observedP->watcherList;
            while (parentP->next != NULL
                && (parentP->next->lastMid != mid
                 || lwm2m_session_is_equal(parentP->next->server->sessionH, fromSessionH, contextP->userData)))
            {
                parentP = parentP->next;
            }

            if (parentP->next != NULL)
            {
                targetP = parentP->next;
                parentP->next = parentP->next->next;
            }
        }
        if (targetP != NULL)
        {
            if (targetP->parameters != NULL)
                lwm2m_free(targetP->parameters);
            lwm2m_free(targetP);

            if (observedP->watcherList == NULL)
            {
                prv_unlinkObserved(contextP, observedP);
                lwm2m_free(observedP);
            }
            return;
        }
    }
}

void observe_clear(lwm2m_context_t * contextP,
                   lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * observedP;

    LOG_URI(uriP);

    observedP = contextP->observedList;
    while(observedP != NULL)
    {
        if (observedP->uri.objectId == uriP->objectId
            && (LWM2M_URI_IS_SET_INSTANCE(uriP) == false
                || observedP->uri.instanceId == uriP->instanceId))
        {
            lwm2m_observed_t * nextP;
            lwm2m_watcher_t * watcherP;

            nextP = observedP->next;

            for (watcherP = observedP->watcherList; watcherP != NULL; watcherP = watcherP->next)
            {
                if (watcherP->parameters != NULL) lwm2m_free(watcherP->parameters);
            }
            LWM2M_LIST_FREE(observedP->watcherList);

            prv_unlinkObserved(contextP, observedP);
            lwm2m_free(observedP);

            observedP = nextP;
        }
        else
        {
            observedP = observedP->next;
        }
    }
}

uint8_t observe_setParameters(lwm2m_context_t * contextP,
                              lwm2m_uri_t * uriP,
                              lwm2m_server_t * serverP,
                              lwm2m_attributes_t * attrP)
{
#define _PRV_BUFFER_SIZE 32

    lwm2m_watcher_t * watcherP;
    uint16_t instanceId;
    uint16_t resourceId;

    uint8_t *greaterThan;
    uint8_t *lessThan;
    uint8_t *step;
    setParam_t *setparam;

    LOG_URI(uriP);
    LOG_ARG("toSet: %08X, toClear: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
            attrP->toSet, attrP->toClear, attrP->minPeriod, attrP->maxPeriod, attrP->greaterThan, attrP->lessThan, attrP->step);

    if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) return COAP_400_BAD_REQUEST;

    watcherP = prv_getWatcher(contextP, uriP, serverP);
    if (watcherP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;

    // Check rule “lt” value + 2*”stp” values < “gt” value
    if ((((attrP->toSet | (watcherP->parameters?watcherP->parameters->toSet:0)) & ~attrP->toClear) & ATTR_FLAG_NUMERIC) == ATTR_FLAG_NUMERIC)
    {
        float gt;
        float lt;
        float stp;

        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN) ||watcherP->parameters == NULL)
        {
            gt = attrP->greaterThan;
        }
        else
        {
            gt = watcherP->parameters->greaterThan;
        }
        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN) ||watcherP->parameters == NULL)
        {
            lt = attrP->lessThan;
        }
        else
        {
            lt = watcherP->parameters->lessThan;
        }
        if (0 != (attrP->toSet & LWM2M_ATTR_FLAG_STEP)||watcherP->parameters == NULL)
        {
            stp = attrP->step;
        }
        else
        {
            stp = watcherP->parameters->step;
        }

        if (lt + (2 * stp) >= gt) return COAP_400_BAD_REQUEST;
    }

    if (watcherP->parameters == NULL)
    {
        if (attrP->toSet != 0)
        {
            watcherP->parameters = (lwm2m_attributes_t *)lwm2m_malloc(sizeof(lwm2m_attributes_t));
            if (watcherP->parameters == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
            memcpy(watcherP->parameters, attrP, sizeof(lwm2m_attributes_t));
        }
    }
    else
    {
        watcherP->parameters->toSet &= ~attrP->toClear;
        if (attrP->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD)
        {
            watcherP->parameters->minPeriod = attrP->minPeriod;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD)
        {
            watcherP->parameters->maxPeriod = attrP->maxPeriod;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_GREATER_THAN)
        {
            watcherP->parameters->greaterThan = attrP->greaterThan;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_LESS_THAN)
        {
            watcherP->parameters->lessThan = attrP->lessThan;
        }
        if (attrP->toSet & LWM2M_ATTR_FLAG_STEP)
        {
            watcherP->parameters->step = attrP->step;
        }
    }

    if(watcherP->parameters != NULL)
        LOG_ARG("Final toSet: %08X, minPeriod: %d, maxPeriod: %d, greaterThan: %f, lessThan: %f, step: %f",
                 watcherP->parameters->toSet, watcherP->parameters->minPeriod, watcherP->parameters->maxPeriod,
                 watcherP->parameters->greaterThan, watcherP->parameters->lessThan, watcherP->parameters->step);
    else
        return COAP_500_INTERNAL_SERVER_ERROR;

    instanceId = LWM2M_URI_IS_SET_INSTANCE(uriP)?uriP->instanceId:0xffff;
    resourceId = LWM2M_URI_IS_SET_RESOURCE(uriP)?uriP->resourceId:0xffff;

    setparam = lwm2m_malloc(sizeof(setParam_t));
    setparam->objid = uriP->objectId;
    setparam->insid = instanceId;
    setparam->resid = resourceId;
    setparam->param = lwm2m_malloc(200);

    greaterThan = lwm2m_malloc(_PRV_BUFFER_SIZE);
    lessThan = lwm2m_malloc(_PRV_BUFFER_SIZE);
    step = lwm2m_malloc(_PRV_BUFFER_SIZE);

    utils_floatToText(watcherP->parameters->greaterThan, greaterThan, _PRV_BUFFER_SIZE);
    utils_floatToText(watcherP->parameters->lessThan, lessThan, _PRV_BUFFER_SIZE);
    utils_floatToText(watcherP->parameters->step, step, _PRV_BUFFER_SIZE);

    sprintf((char *)setparam->param,"pmin=%lu; pmax=%lu; gt=%s; lt=%s; stp=%s",watcherP->parameters->minPeriod,
        watcherP->parameters->maxPeriod, greaterThan, lessThan, step);
    lwm2mPostEvent(contextP->ref,OBJ_SETPARAM_IND,coap_get_mid(),(uint32_t)setparam,0);

    lwm2m_free(greaterThan);
    lwm2m_free(lessThan);
    lwm2m_free(step);
    lwm2m_free(setparam->param);
    lwm2m_free(setparam);
    return COAP_204_CHANGED;
}

lwm2m_observed_t * observe_findByUri(lwm2m_context_t * contextP,
                                     lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;

    LOG_URI(uriP);
    targetP = contextP->observedList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId)
        {
            if ((!LWM2M_URI_IS_SET_INSTANCE(uriP) && !LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)))
             || (LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_INSTANCE(&(targetP->uri)) && (uriP->instanceId == targetP->uri.instanceId)))
             {
                 if ((!LWM2M_URI_IS_SET_RESOURCE(uriP) && !LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)))
                     || (LWM2M_URI_IS_SET_RESOURCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(&(targetP->uri)) && (uriP->resourceId == targetP->uri.resourceId)))
                 {
                     LOG_ARG("Found one with%s observers.", targetP->watcherList ? "" : " no");
                     return targetP;
                 }
             }
        }
        targetP = targetP->next;
    }

    LOG("Found nothing");
    return NULL;
}

void lwm2m_resource_value_changed(lwm2m_context_t * contextP,
                                  lwm2m_uri_t * uriP)
{
    lwm2m_observed_t * targetP;

    LOG("uriP info:");
    LOG_URI(uriP);
    targetP = contextP->observedList;
    while (targetP != NULL)
    {
        LOG("targetP->uri:");
        LOG_URI(&(targetP->uri));
        
        if (targetP->uri.objectId == uriP->objectId)
        {
            if (!LWM2M_URI_IS_SET_INSTANCE(uriP)
             || (targetP->uri.flag & LWM2M_URI_FLAG_INSTANCE_ID) == 0
             || uriP->instanceId == targetP->uri.instanceId)
            {
                LOG("Found an instanece");
                if (!LWM2M_URI_IS_SET_RESOURCE(uriP)
                 || (targetP->uri.flag & LWM2M_URI_FLAG_RESOURCE_ID) == 0
                 || uriP->resourceId == targetP->uri.resourceId)
                {
                    lwm2m_watcher_t * watcherP;

                    LOG("Found an observation");
                    LOG_URI(&(targetP->uri));

                    for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
                    {
                        if (watcherP->active == true)
                        {
                            LOG("Tagging a watcher");
                            watcherP->update = true;
                        }
                    }
                }
            }
        }
        targetP = targetP->next;
    }
}

static void prv_handleNotifySendReply(lwm2m_transaction_t * transacP,
                                              void * message)
{
    coap_packet_t * packet = (coap_packet_t *)message;
    lwm2m_context_t * targetP = (lwm2m_context_t *)(transacP->userData);
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
    lwm2m_uri_t *uri = &transacP->uri_path;   //uri of notify send 

    uint32_t eventParam = ((transacP->seqnum & 0xFF) ==0? NOTIFY_CON << 8 : transacP->seqnum & 0xFFFF);
#endif
    if (packet != NULL && packet->type == COAP_TYPE_ACK)
    {
        lwm2mPostEvent(targetP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_SENT_SUCCESS,packet->mid,0);
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
        lwm2mPostEvent(targetP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SENT_SUCCESS,
            uri->objectId,  eventParam);
#endif
        lwm2mPostEvent(targetP->ref,OBJ_OPERATE_RSP_IND,0,-1,0);
        LOG("NotifySend successful");
    }
    else
    {
        if (packet == NULL)
        {
            lwm2mPostEvent(targetP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_SENT_TIMEOUT, 0, 0);
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
            lwm2mPostEvent(targetP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SENT_TIMEOUT,
                uri->objectId,  eventParam);
#endif
        }
        else
        {
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
            if(packet->type == COAP_TYPE_RST)
                lwm2mPostEvent(targetP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SENT_RESET,
                    uri->objectId , eventParam);
            else
                lwm2mPostEvent(targetP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SENT_TIMEOUT,
                    uri->objectId , eventParam);
#endif
            lwm2mPostEvent(targetP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_FAILED,packet->mid,packet->type);
        }
        lwm2mPostEvent(targetP->ref,OBJ_OPERATE_RSP_IND,-1,-1,0);
        LOG("NotifySend failed");
    }
}

void observe_step(lwm2m_context_t * contextP,
                  time_t currentTime,
                  time_t * timeoutP)
{
    lwm2m_observed_t * targetP;

    LOG("Entering");
    for (targetP = contextP->observedList ; targetP != NULL ; targetP = targetP->next)
    {
        lwm2m_watcher_t * watcherP;
        uint8_t * buffer = NULL;
        size_t length = 0;
        lwm2m_data_t * dataP = NULL;
        int size = 0;
        double floatValue = 0;
        int64_t integerValue = 0;
        bool storeValue = false;
        coap_packet_t message[1];
        time_t interval;

        LOG_URI(&(targetP->uri));
        for (watcherP = targetP->watcherList ; watcherP != NULL ; watcherP = watcherP->next)
        {
            lwm2m_transaction_t * transaction = NULL;
            if (watcherP->active == true)
            {
                bool notify = false;

                if (watcherP->update == true)
                {
                    // value changed, should we notify the server ?

                    if (watcherP->parameters == NULL || watcherP->parameters->toSet == 0)
                    {
                        // no conditions
                        notify = true;
                        LOG("Notify with no conditions");
                        LOG_URI(&(targetP->uri));
                    }

                    if (notify == false
                     && watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & ATTR_FLAG_NUMERIC) != 0)
                    {
                        if (LWM2M_URI_IS_SET_RESOURCE(&targetP->uri))
                        {
                            int ret = 0;
                            if (COAP_205_CONTENT != object_readData(contextP, &targetP->uri, &size, &dataP)) continue;
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            OSI_LOGI(0x10009495, "object read value rai %d con %d seq", dataP->rai_mode, dataP->notify_type, dataP->seq_num);
                            #endif
                            switch (dataP->type)
                            {
                            case LWM2M_TYPE_INTEGER:
                                if (1 != lwm2m_data_decode_int(dataP, &integerValue))
                                {
                                    lwm2m_data_free(size, dataP);
                                    ret = -1;
                                }
                                storeValue = true;
                                break;
                            case LWM2M_TYPE_FLOAT:
                                if (1 != lwm2m_data_decode_float(dataP, &floatValue))
                                {
                                    lwm2m_data_free(size, dataP);
                                    ret = -1;
                                }
                                storeValue = true;
                                break;
                            default:
                                break;
                            }
                            if (ret == -1)
                                break;
                        }
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_LESS_THAN) != 0)
                        {
                            LOG("Checking lower threshold");
                            // Did we cross the lower threshold ?
                            if(dataP != NULL)
                            {
                                switch (dataP->type)
                                {
                                case LWM2M_TYPE_INTEGER:
                                    if ((integerValue <= watcherP->parameters->lessThan
                                      && watcherP->lastValue.asInteger > watcherP->parameters->lessThan)
                                     || (integerValue >= watcherP->parameters->lessThan
                                      && watcherP->lastValue.asInteger < watcherP->parameters->lessThan))
                                    {
                                        LOG("Notify on lower threshold crossing");
                                        notify = true;
                                    }
                                    break;
                                case LWM2M_TYPE_FLOAT:
                                    if ((floatValue <= watcherP->parameters->lessThan
                                      && watcherP->lastValue.asFloat > watcherP->parameters->lessThan)
                                     || (floatValue >= watcherP->parameters->lessThan
                                      && watcherP->lastValue.asFloat < watcherP->parameters->lessThan))
                                    {
                                        LOG("Notify on lower threshold crossing");
                                        notify = true;
                                    }
                                    break;
                                default:
                                    break;
                                }
                            }
                            else
                            {
                                LOG("dataP is Null");
                            }
                        }
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_GREATER_THAN) != 0)
                        {
                            LOG("Checking upper threshold");
                            // Did we cross the upper threshold ?
                            if(dataP != NULL)
                            {
                                switch (dataP->type)
                                {
                                case LWM2M_TYPE_INTEGER:
                                    if ((integerValue <= watcherP->parameters->greaterThan
                                      && watcherP->lastValue.asInteger > watcherP->parameters->greaterThan)
                                     || (integerValue >= watcherP->parameters->greaterThan
                                      && watcherP->lastValue.asInteger < watcherP->parameters->greaterThan))
                                    {
                                        LOG("Notify on lower upper crossing");
                                        notify = true;
                                    }
                                    break;
                                case LWM2M_TYPE_FLOAT:
                                    if ((floatValue <= watcherP->parameters->greaterThan
                                      && watcherP->lastValue.asFloat > watcherP->parameters->greaterThan)
                                     || (floatValue >= watcherP->parameters->greaterThan
                                      && watcherP->lastValue.asFloat < watcherP->parameters->greaterThan))
                                    {
                                        LOG("Notify on lower upper crossing");
                                        notify = true;
                                    }
                                    break;
                                default:
                                    break;
                                }
                            }
                            else
                            {
                                LOG("dataP is Null");
                            }
                        }
                        if ((watcherP->parameters->toSet & LWM2M_ATTR_FLAG_STEP) != 0)
                        {
                            LOG("Checking step");
                            if(dataP != NULL)
                            {
                                switch (dataP->type)
                                {
                                case LWM2M_TYPE_INTEGER:
                                {
                                    int64_t diff;

                                    diff = integerValue - watcherP->lastValue.asInteger;
                                    if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                     || (diff >= 0 && diff >= watcherP->parameters->step))
                                    {
                                        LOG("Notify on step condition");
                                        notify = true;
                                    }
                                }
                                    break;
                                case LWM2M_TYPE_FLOAT:
                                {
                                    double diff;
    
                                    diff = floatValue - watcherP->lastValue.asFloat;
                                    if ((diff < 0 && (0 - diff) >= watcherP->parameters->step)
                                     || (diff >= 0 && diff >= watcherP->parameters->step))
                                    {
                                        LOG("Notify on step condition");
                                        notify = true;
                                    }
                                }
                                    break;
                                default:
                                    break;
                                }
                            }
                            else
                            {
                                LOG("dataP is Null");
                            }
	                    }
                    }

                    if (watcherP->parameters != NULL
                     && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MIN_PERIOD) != 0)
                    {
                        LOG_ARG("Checking minimal period (%d s)", watcherP->parameters->minPeriod);

                        if (watcherP->lastTime + watcherP->parameters->minPeriod > currentTime)
                        {
                            // Minimum Period did not elapse yet
                            interval = watcherP->lastTime + watcherP->parameters->minPeriod - currentTime;
                            if (*timeoutP > interval) *timeoutP = interval;
                            notify = false;
                        }
                        else
                        {
                            LOG("Notify on minimal period");
                            notify = true;
                        }
                    }
                }

                // Is the Maximum Period reached ?
                if (notify == false
                 && watcherP->parameters != NULL
                 && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    LOG_ARG("Checking maximal period (%d s)", watcherP->parameters->maxPeriod);

                    if (watcherP->lastTime + watcherP->parameters->maxPeriod <= currentTime)
                    {
                        LOG("Notify on maximal period");
                        notify = true;
                    }
                }

                if (notify == true)
                {
                    if (buffer == NULL)
                    {
                        if (dataP != NULL)
                        {
                            int res;

                            res = lwm2m_data_serialize(&targetP->uri, size, dataP, &(watcherP->format), &buffer);
                            if (res < 0)
                            {
                                break;
                            }
                            else
                            {
                                length = (size_t)res;
                            }
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            contextP->notifyMode = (dataP->notify_type  == 0? NOTIFY_CON: NOTIFY_NON);
                            contextP->sendflag |= dataP->rai_mode;
                            if(dataP->seq_num == 0)
                                contextP->seqnum = 0;
                            else
                                contextP->seqnum = targetP->uri.objectId << 16 |contextP->notifyMode <<8 | dataP->seq_num;
                            #endif
                        }
                        else
                        {
                            lwm2m_data_t objData;
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            contextP->notifyMode = NOTIFY_CON;
                            contextP->seqnum = 0;
                            #endif
                            //lwm2m_data_t objData;
                            dataP = &objData;
                            if (COAP_205_CONTENT != object_readex(contextP, &targetP->uri, &(watcherP->format), &buffer, &length, dataP))
                            {
                                buffer = NULL;
                                break;
                            }
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            else
                            {
                                contextP->notifyMode = (dataP->notify_type == 0? NOTIFY_CON : NOTIFY_NON);
                                contextP->sendflag |= dataP->rai_mode;
                                if(dataP->seq_num == 0)
                                    contextP->seqnum = 0;
                                else
                                    contextP->seqnum = targetP->uri.objectId << 16 |contextP->notifyMode <<8 | dataP->seq_num;
                            }
                            #endif
                        }

                        if(contextP->notifyMode == NOTIFY_NON)
                        {
                            coap_init_message(message, COAP_TYPE_NON, COAP_205_CONTENT, 0);
                            coap_set_header_content_type(message, watcherP->format);
                            coap_set_payload(message, buffer, length);
                        }
                    }
                    watcherP->lastTime = currentTime;
                    watcherP->lastMid = contextP->nextMID++;

                    if(contextP->notifyMode == NOTIFY_NON)
                    {
                        int sentret;
                        message->mid = watcherP->lastMid;
                        coap_set_header_token(message, watcherP->token, watcherP->tokenLen);
                        coap_set_header_observe(message, watcherP->counter++);
                        sentret = -1;
                        if(0 != (sentret = message_send(contextP, message, watcherP->server->sessionH)))
                        {
                            LOG_ARG("obeserve_step message_send failed return:%d", sentret);
                            if (contextP->serverList != NULL)
                            {
                                contextP->serverList->status = STATE_REG_FAILED;
                            }
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_FAILED,message->mid, 0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_FAILED,
                                targetP->uri.objectId,  contextP->seqnum & 0xFFFF);
                            #endif
                        }
                        else
                        {
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_SEND_SUCCESS,message->mid, 0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SEND_SUCCESS,
                                    targetP->uri.objectId,  contextP->seqnum & 0xFFFF);
                            #endif
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_SENT_SUCCESS,message->mid, 0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            if((contextP->seqnum & 0xFF) == 0)
                                lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SENT_SUCCESS,
                                targetP->uri.objectId,  contextP->seqnum & 0xFFFF);
                            #endif
                        }
                    }
                    else
                    {
                        transaction = transaction_new(watcherP->server->sessionH, COAP_205_CONTENT, NULL, NULL, watcherP->lastMid, watcherP->tokenLen, watcherP->token);
                        if (transaction == NULL)
                        {
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_FAILED,contextP->nextMID, 0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_FAILED,
                                targetP->uri.objectId,  NOTIFY_CON << 8 | (contextP->seqnum & 0xFFFF));
                            #endif
                            break;
                        }
                        coap_set_header_content_type(transaction->message, watcherP->format);
                        coap_set_payload(transaction->message, buffer, length);
                        coap_set_header_observe(transaction->message, watcherP->counter++);
                        transaction->callback = prv_handleNotifySendReply;
                        transaction->userData = contextP;
                        #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                        memcpy(&transaction->uri_path, &targetP->uri, sizeof(lwm2m_uri_t));
                        transaction->seqnum = contextP->seqnum;
                        #endif
                        contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transaction);
                        if (transaction_send(contextP, transaction) == 0)
                        {
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_SEND_SUCCESS,watcherP->lastMid, 0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_SEND_SUCCESS,
                                targetP->uri.objectId,  NOTIFY_CON << 8 |(contextP->seqnum & 0xFFFF));
                            #endif
                        }else
                        {
                            lwm2mPostEvent(contextP->ref,DATA_SEND_IND,LWM2M_EVENT_NOTIFY_FAILED,watcherP->lastMid,0);
                            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
                            lwm2mPostEvent(contextP->ref,OBJ_OBSERVE_NOTIFY_IND,LWM2M_EVENT_NOTIFY_FAILED,
                                targetP->uri.objectId,  NOTIFY_CON << 8 |(contextP->seqnum & 0xFFFF));
                            #endif
                        }
                    }
                    watcherP->update = false;
                }

                // Store this value
                if (notify == true && storeValue == true)
                {
                    switch (dataP->type)
                    {
                    case LWM2M_TYPE_INTEGER:
                        watcherP->lastValue.asInteger = integerValue;
                        break;
                    case LWM2M_TYPE_FLOAT:
                        watcherP->lastValue.asFloat = floatValue;
                        break;
                    default:
                        break;
                    }
                }

                if (watcherP->parameters != NULL && (watcherP->parameters->toSet & LWM2M_ATTR_FLAG_MAX_PERIOD) != 0)
                {
                    // update timers
                    interval = watcherP->lastTime + watcherP->parameters->maxPeriod - currentTime;
                    if (*timeoutP > interval) *timeoutP = interval;
                }
            }
            #ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
            contextP->notifyMode = NOTIFY_CON;
            contextP->sendflag &= 0xFC; 
            contextP->seqnum = 0;
            #endif
        }

        if (dataP != NULL) lwm2m_data_free(size, dataP);
        if (buffer != NULL) lwm2m_free(buffer);
    }
}

#endif

#ifdef LWM2M_SERVER_MODE

typedef struct
{
    lwm2m_observation_t * observationP;
    lwm2m_result_callback_t callbackP;
    void * userDataP;
} cancellation_data_t;

static lwm2m_observation_t * prv_findObservationByURI(lwm2m_client_t * clientP,
                                                      lwm2m_uri_t * uriP)
{
    lwm2m_observation_t * targetP;

    targetP = clientP->observationList;
    while (targetP != NULL)
    {
        if (targetP->uri.objectId == uriP->objectId
         && targetP->uri.flag == uriP->flag
         && targetP->uri.instanceId == uriP->instanceId
         && targetP->uri.resourceId == uriP->resourceId)
        {
            return targetP;
        }

        targetP = targetP->next;
    }

    return targetP;
}

void observe_remove(lwm2m_observation_t * observationP)
{
    LOG("Entering");
    observationP->clientP->observationList = (lwm2m_observation_t *) LWM2M_LIST_RM(observationP->clientP->observationList, observationP->id, NULL);
    lwm2m_free(observationP);
}

static void prv_obsRequestCallback(lwm2m_transaction_t * transacP,
                                   void * message)
{
    lwm2m_observation_t * observationP = (lwm2m_observation_t *)transacP->userData;
    coap_packet_t * packet = (coap_packet_t *)message;
    uint8_t code;

    switch (observationP->status)
    {
    case STATE_DEREG_PENDING:
        // Observation was canceled by the user.
        observe_remove(observationP);
        return;

    case STATE_REG_PENDING:
        observationP->status = STATE_REGISTERED;
        break;

    default:
        break;
    }

    if (message == NULL)
    {
        code = COAP_503_SERVICE_UNAVAILABLE;
    }
    else if (packet->code == COAP_205_CONTENT
         && !IS_OPTION(packet, COAP_OPTION_OBSERVE))
    {
        code = COAP_405_METHOD_NOT_ALLOWED;
    }
    else
    {
        code = packet->code;
    }

    if (code != COAP_205_CONTENT)
    {
        observationP->callback(observationP->clientP->internalID,
                               &observationP->uri,
                               code,
                               LWM2M_CONTENT_TEXT, NULL, 0,
                               observationP->userData);
        observe_remove(observationP);
    }
    else
    {
        observationP->callback(observationP->clientP->internalID,
                               &observationP->uri,
                               0,
                               packet->content_type, packet->payload, packet->payload_len,
                               observationP->userData);
    }
}


static void prv_obsCancelRequestCallback(lwm2m_transaction_t * transacP,
                                         void * message)
{
    cancellation_data_t * cancelP = (cancellation_data_t *)transacP->userData;
    coap_packet_t * packet = (coap_packet_t *)message;
    uint8_t code;

    if (message == NULL)
    {
        code = COAP_503_SERVICE_UNAVAILABLE;
    }
    else
    {
        code = packet->code;
    }

    if (code != COAP_205_CONTENT)
    {
        cancelP->callbackP(cancelP->observationP->clientP->internalID,
                           &cancelP->observationP->uri,
                           code,
                           LWM2M_CONTENT_TEXT, NULL, 0,
                           cancelP->userDataP);
    }
    else
    {
        cancelP->callbackP(cancelP->observationP->clientP->internalID,
                           &cancelP->observationP->uri,
                           0,
                           packet->content_type, packet->payload, packet->payload_len,
                           cancelP->userDataP);
    }

    observe_remove(cancelP->observationP);

    lwm2m_free(cancelP);
}


int lwm2m_observe(lwm2m_context_t * contextP,
                  uint16_t clientID,
                  lwm2m_uri_t * uriP,
                  lwm2m_result_callback_t callback,
                  void * userData)
{
    lwm2m_client_t * clientP;
    lwm2m_transaction_t * transactionP;
    lwm2m_observation_t * observationP;
    uint8_t token[4];

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    if (!LWM2M_URI_IS_SET_INSTANCE(uriP) && LWM2M_URI_IS_SET_RESOURCE(uriP)) return COAP_400_BAD_REQUEST;

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return COAP_404_NOT_FOUND;

    for (observationP = clientP->observationList; observationP != NULL; observationP = observationP->next)
    {
        if (uriP->objectId == observationP->uri.objectId
            && (LWM2M_URI_IS_SET_INSTANCE(uriP) == false
                || observationP->uri.instanceId == uriP->instanceId)
            && (LWM2M_URI_IS_SET_RESOURCE(uriP) == false
                || observationP->uri.resourceId == uriP->resourceId))
        {
            break;
        }
    }
    if (observationP == NULL)
    {
        observationP = (lwm2m_observation_t *)lwm2m_malloc(sizeof(lwm2m_observation_t));
        if (observationP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        memset(observationP, 0, sizeof(lwm2m_observation_t));

        observationP->id = lwm2m_list_newId((lwm2m_list_t *)clientP->observationList);
        memcpy(&observationP->uri, uriP, sizeof(lwm2m_uri_t));
        observationP->clientP = clientP;

        observationP->clientP->observationList = (lwm2m_observation_t *)LWM2M_LIST_ADD(observationP->clientP->observationList, observationP);
    }
    observationP->status = STATE_REG_PENDING;
    observationP->callback = callback;
    observationP->userData = userData;

    token[0] = clientP->internalID >> 8;
    token[1] = clientP->internalID & 0xFF;
    token[2] = observationP->id >> 8;
    token[3] = observationP->id & 0xFF;

    transactionP = transaction_new(clientP->sessionH, COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, token);
    if (transactionP == NULL)
    {
        observationP->clientP->observationList = (lwm2m_observation_t *)LWM2M_LIST_RM(observationP->clientP->observationList, observationP->id, NULL);
        lwm2m_free(observationP);
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    coap_set_header_observe(transactionP->message, 0);
    if (clientP->supportJSON == true)
    {
        coap_set_header_accept(transactionP->message, LWM2M_CONTENT_JSON);
    }
    else
    {
        coap_set_header_accept(transactionP->message, LWM2M_CONTENT_TLV);
    }

    transactionP->callback = prv_obsRequestCallback;
    transactionP->userData = (void *)observationP;

    contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transactionP);

    return transaction_send(contextP, transactionP);
}

int lwm2m_observe_cancel(lwm2m_context_t * contextP,
                         uint16_t clientID,
                         lwm2m_uri_t * uriP,
                         lwm2m_result_callback_t callback,
                         void * userData)
{
    lwm2m_client_t * clientP;
    lwm2m_observation_t * observationP;

    LOG_ARG("clientID: %d", clientID);
    LOG_URI(uriP);

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return COAP_404_NOT_FOUND;

    observationP = prv_findObservationByURI(clientP, uriP);
    if (observationP == NULL) return COAP_404_NOT_FOUND;

    switch (observationP->status)
    {
    case STATE_REGISTERED:
    {
        lwm2m_transaction_t * transactionP;
        cancellation_data_t * cancelP;
        uint8_t token[4];

        token[0] = clientP->internalID >> 8;
        token[1] = clientP->internalID & 0xFF;
        token[2] = observationP->id >> 8;
        token[3] = observationP->id & 0xFF;

        transactionP = transaction_new(clientP->sessionH, COAP_GET, clientP->altPath, uriP, contextP->nextMID++, 4, token);
        if (transactionP == NULL)
        {
            return COAP_500_INTERNAL_SERVER_ERROR;
        }
        cancelP = (cancellation_data_t *)lwm2m_malloc(sizeof(cancellation_data_t));
        if (cancelP == NULL)
        {
            lwm2m_free(transactionP);
            return COAP_500_INTERNAL_SERVER_ERROR;
        }

        coap_set_header_observe(transactionP->message, 1);

        cancelP->observationP = observationP;
        cancelP->callbackP = callback;
        cancelP->userDataP = userData;

        transactionP->callback = prv_obsCancelRequestCallback;
        transactionP->userData = (void *)cancelP;

        contextP->transactionList = (lwm2m_transaction_t *)LWM2M_LIST_ADD(contextP->transactionList, transactionP);

        return transaction_send(contextP, transactionP);
    }

    case STATE_REG_PENDING:
        observationP->status = STATE_DEREG_PENDING;
        break;

    default:
        // Should not happen
        break;
    }

    return COAP_NO_ERROR;
}

bool observe_handleNotify(lwm2m_context_t * contextP,
                           void * fromSessionH,
                           coap_packet_t * message,
        				   coap_packet_t * response)
{
    uint8_t * tokenP;
    int token_len;
    uint16_t clientID;
    uint16_t obsID;
    lwm2m_client_t * clientP;
    lwm2m_observation_t * observationP;
    uint32_t count;

    LOG("Entering");
    token_len = coap_get_header_token(message, (const uint8_t **)&tokenP);
    if (token_len != sizeof(uint32_t)) return false;

    if (1 != coap_get_header_observe(message, &count)) return false;

    clientID = (tokenP[0] << 8) | tokenP[1];
    obsID = (tokenP[2] << 8) | tokenP[3];

    clientP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)contextP->clientList, clientID);
    if (clientP == NULL) return false;

    observationP = (lwm2m_observation_t *)lwm2m_list_find((lwm2m_list_t *)clientP->observationList, obsID);
    if (observationP == NULL)
    {
        coap_init_message(response, COAP_TYPE_RST, 0, message->mid);
        message_send(contextP, response, fromSessionH);
    }
    else
    {
        if (message->type == COAP_TYPE_CON ) {
            coap_init_message(response, COAP_TYPE_ACK, 0, message->mid);
            message_send(contextP, response, fromSessionH);
        }
        observationP->callback(clientID,
                               &observationP->uri,
                               (int)count,
                               message->content_type, message->payload, message->payload_len,
                               observationP->userData);
    }
    return true;
}
#endif

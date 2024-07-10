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
 *    domedambrosio - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Fabien Fleutot - Please refer to git log
 *    Simon Bernard - Please refer to git log
 *    Toby Jaffey - Please refer to git log
 *    Pascal Rieux - Please refer to git log
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

/*
Contains code snippets which are:

 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

*/


#include "internals.h"
#include "liblwm2m.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef CONFIG_LWM2M_FOTA_SUPPORT     //modify by xiameng.yang
#include "fupdate.h"
#include "vfs.h"
#endif

//#include "osi_log.h"   //modify by xiameng.yang

#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif

#ifdef CONFIG_LWM2M_ONENET_SUPPORT

#define ONENET_CONFIG_CALLBACK_TIMEOUT (30)

uint8_t prv_notifyDiscoverReport(lwm2m_context_t * context, st_request_t* request, uint8_t result, uint8_t *valuestring)
{
    int i = 0;
    int instCount = 0;
    int instIndex = 0;
    lwm2m_object_t * object;
    lwm2m_list_t * instanceP;
    ipso_obj_t *ipso_obj;
    int nbRes ;
    ipso_res_t *ipso_res_array ;

    if (result == COAP_205_CONTENT)
    {
        object = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, request->uri.objectId);
        if(object == NULL)
        {
            LOG("ERROR: notify discover response failed as not found object");
            return COAP_404_NOT_FOUND;
        }

        if(lwm2m_registNewRes(object->objID, valuestring, context->ref) != 1)
        {
            LOG("ERROR: lwm2m register new resource failed");
            return COAP_406_NOT_ACCEPTABLE;
        }
        
        ipso_obj = lwm2m_get_ipso_obj(object->objID, -1, NULL, context->ref);
        if (ipso_obj == NULL || ipso_obj->resCount == 0)
        {
            LOG("ERROR: notify discover response failed as not found ipso obj");
            return COAP_404_NOT_FOUND;
        }
        nbRes = ipso_obj->resCount;
        ipso_res_array = ipso_obj->resArray;

        for (instanceP = object->instanceList; instanceP != NULL; instanceP = instanceP->next)
            instCount++;

        //get instance valid count for malloc instance memory
        request->respData = lwm2m_data_new(instCount);
        request->respDataCount = instCount;
        if(request->respData == NULL)
        {
            LOG("ERROR: notify discover response failed as malloc data error");
            return COAP_500_INTERNAL_SERVER_ERROR;
        }

        /*Get instance array index of reportData by bitmap index*/
        for (instanceP = object->instanceList; instanceP != NULL; instanceP = instanceP->next)
        {
            request->respData[instIndex].type = LWM2M_TYPE_OBJECT_INSTANCE;
            request->respData[instIndex].id = instanceP->id;
            request->respData[instIndex].value.asChildren.array = lwm2m_data_new(nbRes);
            request->respData[instIndex].value.asChildren.count = nbRes;

            for (i = 0; i < nbRes; i++)
            {
                request->respData[instIndex].value.asChildren.array[i].id = ipso_res_array[i].resId;
                request->respData[instIndex].value.asChildren.array[i].type = LWM2M_TYPE_OBJECT_LINK;
                request->respData[instIndex].value.asChildren.array[i].value.asObjLink.objectId = object->objID;
                request->respData[instIndex].value.asChildren.array[i].value.asObjLink.objectInstanceId = instanceP->id;
            }

            skel_discover_update_resource(instanceP->id, object);
            
            instIndex++;
        }
    }    

    LOG_ARG("DiscoverResponse Done id:0x%x, Result:(0x%x)",request->mid, result);
    object_asynAckReadData(context, request, result);
    lwm2m_data_free(request->respDataCount, request->respData);
    request->respData = NULL;
    request->respDataCount = 0;

    return COAP_NO_ERROR;
}

uint8_t prv_notifyObserveReport(lwm2m_context_t * context, lwm2m_observed_t *observeP, uint16_t msgid, lwm2m_uri_t * uriP, uint8_t result, uint8_t *value, uint32_t valuelen, uint8_t valuetype)
{
    int i = 0;
    int instCount = 0;
    int instIndex = 0;
    int attrCount = 0;
    int attrIndex = 0;

    lwm2m_watcher_t * watcherP = NULL;
    lwm2m_object_t *object = NULL;
    lwm2m_list_t * instanceP = NULL;
    ipso_obj_t *ipso_obj;
    bool tlv_flag;
    coap_status_t ret;

    if((!LWM2M_URI_IS_SET_OBJECT(uriP) || value == NULL) && 
        result != COAP_231_CONTINUE)
    {
        goto TAG_DONE;
    }

    if(!LWM2M_URI_IS_SET_INSTANCE(uriP) ||
        !LWM2M_URI_IS_SET_RESOURCE(uriP) ||
        !LWM2M_URI_IS_SET_OBJECT(uriP))
    {
        LOG("Notify failed, invalid uri");
        return COAP_400_BAD_REQUEST;
    }

    object = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, uriP->objectId);
    if(object == NULL)
    {
        LOG("Notify failed, not found object");
        return COAP_404_NOT_FOUND;
    }

    // get ipso object table
    ipso_obj = lwm2m_get_ipso_obj(object->objID, -1, NULL, context->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
    {
        LOG("Notify failed, not found ipso obj");
        return COAP_404_NOT_FOUND;
    }

    // get the instance number
    for (instanceP = object->instanceList; instanceP != NULL; instanceP = instanceP->next)
    {
        if(instanceP->id == uriP->instanceId)
            break;
    }
    if(instanceP == NULL)
    {
        LOG("Notify failed, not exist instance");
        return COAP_404_NOT_FOUND;
    }

    // get the watcher
    for (watcherP = observeP->watcherList; watcherP != NULL ; watcherP = watcherP->next)
    {
        if(watcherP->watcherMid == msgid)
            break;
    }
    if(watcherP == NULL)
    {
        LOG("Notify failed, not exist watcherP");
        return COAP_404_NOT_FOUND;
    }

    //malloc data memory in first call of observe
    if(watcherP->reportData == NULL)
    {
        if((!LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && !LWM2M_URI_IS_SET_RESOURCE(&observeP->uri)) || 
            ((LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && !LWM2M_URI_IS_SET_RESOURCE(&observeP->uri))))
        {
            //object/instance
            //for single instance.
            instCount = 1;
            watcherP->reportData = lwm2m_data_new(instCount);
            if(watcherP->reportData == NULL)
            {
                return COAP_500_INTERNAL_SERVER_ERROR;
            }

            watcherP->reportDataCount = instCount;
            watcherP->reportData->id = uriP->instanceId;

        }
        else if(LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && LWM2M_URI_IS_SET_RESOURCE(&observeP->uri))
        {
            //object/instance/resource
            instCount = 1;
            watcherP->reportData = lwm2m_data_new(instCount);
            if(watcherP->reportData == NULL)
            {
                return COAP_500_INTERNAL_SERVER_ERROR;
            }
            watcherP->reportDataCount = instCount;
            watcherP->reportData->id = uriP->instanceId;
        }
    }

    if(watcherP->reportData == NULL)
    {
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    if(watcherP->reportData->id != uriP->instanceId)
    {
        LOG("Notify failed, last request not finish");
        return COAP_400_BAD_REQUEST;
    }

    if((!LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && !LWM2M_URI_IS_SET_RESOURCE(&observeP->uri)) || 
        ((LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && !LWM2M_URI_IS_SET_RESOURCE(&observeP->uri))))
    {
        //object/instance
        //for single instance.
        instIndex = 0;
        instCount = 1;

        //set all resources count;
        attrCount =  ipso_obj->resCount;
    }
    else if(LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && LWM2M_URI_IS_SET_RESOURCE(&observeP->uri))
    {
        //object/instance/resource
        //for single instance.
        instIndex = 0;
        instCount = 1;
        //for single resource;
        attrCount = 1;
    }


    tlv_flag = false;
	if(instCount == 1 && attrCount == 1)
    {
        if(!LWM2M_URI_IS_SET_INSTANCE(&observeP->uri) && !LWM2M_URI_IS_SET_RESOURCE(&observeP->uri))
            tlv_flag=true;
    }

    if(instCount == 1 && attrCount == 1 && tlv_flag==false)
    {
        watcherP->reportData->type = (lwm2m_data_type_t)(valuetype + 3);
        watcherP->reportData->id = uriP->resourceId;

        if(valuetype == lwm2m_opaque || valuetype == lwm2m_string)
        {
            watcherP->reportData->value.asBuffer.length = valuelen;
            watcherP->reportData->value.asBuffer.buffer = (uint8_t*)lwm2m_malloc(valuelen);
            if(watcherP->reportData->value.asBuffer.buffer == NULL)
                return COAP_500_INTERNAL_SERVER_ERROR;
            memcpy(watcherP->reportData->value.asBuffer.buffer, value, valuelen);
        }
        else
        {
            memcpy(&watcherP->reportData->value.asInteger, value, valuelen);
        }
    }
    else
    {

        if(watcherP->reportData[instIndex].value.asChildren.array == NULL)
        {
            watcherP->reportData[instIndex].type = LWM2M_TYPE_OBJECT_INSTANCE;
            watcherP->reportData[instIndex].id = uriP->instanceId;
            watcherP->reportData[instIndex].value.asChildren.array = lwm2m_data_new(ipso_obj->resCount);
            if(watcherP->reportData[instIndex].value.asChildren.array == NULL)
                return COAP_500_INTERNAL_SERVER_ERROR;
            watcherP->reportData[instIndex].value.asChildren.count = ipso_obj->resCount;
        }

        //found out index of resource for unused array memory
        for (i=0; i<ipso_obj->resCount; i++)
        {
            if(watcherP->reportData[instIndex].value.asChildren.array[i].type == LWM2M_TYPE_UNDEFINED)
                break;
            attrIndex++;
        }

        //it's all occupied of resource array.
        if(attrIndex >= ipso_obj->resCount)
        {
            LOG("Notify failed, attr index out of bounds");
            return COAP_400_BAD_REQUEST;
        }

        watcherP->reportData[instIndex].value.asChildren.array[attrIndex].id = uriP->resourceId;
        watcherP->reportData[instIndex].value.asChildren.array[attrIndex].type = (lwm2m_data_type_t)(valuetype + 3);
        if(valuetype == lwm2m_opaque || valuetype == lwm2m_string)
        {
            watcherP->reportData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.length = valuelen;
            watcherP->reportData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer = (uint8_t*)lwm2m_malloc(valuelen);
            if(watcherP->reportData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer == NULL)
                return COAP_500_INTERNAL_SERVER_ERROR;
            memcpy(watcherP->reportData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer, value, valuelen);
        }
        else
        {
            memcpy(&watcherP->reportData[instIndex].value.asChildren.array[attrIndex].value.asInteger, value, valuelen);
        }
    }

    if(result == COAP_231_CONTINUE)
    {
        LOG_ARG("Notify 231_Continue id: 0x%x", msgid);
        return COAP_231_CONTINUE;
    }

TAG_DONE:
    
    LOG_ARG("Notify done id:0x%x", msgid);

    //send observe report message with data packet
    ret = observe_asynReport(context, observeP, msgid, result);

    if(watcherP != NULL)
    {
        lwm2m_data_free(watcherP->reportDataCount, watcherP->reportData);
        watcherP->reportData = NULL;
        watcherP->reportDataCount = 0;
    }

    return ret;
}

uint8_t prv_notifyReadReport(lwm2m_context_t * context, st_request_t* request, lwm2m_uri_t * uriP, uint8_t result, uint8_t *value, uint32_t valuelen, uint8_t valuetype)
{
    int i = 0;
    int instCount = 0;
    int instIndex = 0;
    int attrCount = 0;
    int attrIndex = 0;

    lwm2m_list_t * instanceP = NULL;
    lwm2m_object_t *object;
    ipso_obj_t *ipso_obj;
    int nbRes ;
    bool tlv_flag;

    if(result != COAP_231_CONTINUE && 
        result != COAP_205_CONTENT)
    {
        goto TAG_DONE;
    }

    LOG_URI(uriP);

    if(!object_isStdObject(uriP->objectId) && skel_read_update_resource(context, uriP, value, valuelen, valuetype) != 0)
    {
        LOG("ReadResponse failed, update resource failed");
        return COAP_400_BAD_REQUEST;
    }

    object = (lwm2m_object_t *)LWM2M_LIST_FIND(context->objectList, uriP->objectId);
    if(object == NULL)
    {
        LOG("ReadResponse failed, not found object");
        return COAP_404_NOT_FOUND;
    }

    // get the instance number
    for (instanceP = object->instanceList; instanceP != NULL; instanceP = instanceP->next)
        i++;

    // get ipso object table
    ipso_obj = lwm2m_get_ipso_obj(object->objID, -1, NULL, context->ref);
    if (ipso_obj == NULL || ipso_obj->resCount == 0)
    {
        LOG("ReadResponse failed, not found ipso obj");
        return COAP_404_NOT_FOUND;
    }
    nbRes = ipso_obj->resCount;

    //malloc data memory in first call of request
    if(request->respData == NULL)
    {
        if(!LWM2M_URI_IS_SET_INSTANCE(&request->uri) &&
            !LWM2M_URI_IS_SET_RESOURCE(&request->uri))
        {
            //for mutiple instance.
            instCount = i;
            request->respData = lwm2m_data_new(instCount);
            request->respDataCount = instCount;
        }
        else if((LWM2M_URI_IS_SET_INSTANCE(&request->uri) && !LWM2M_URI_IS_SET_RESOURCE(&request->uri)))
        {
            //for single instance.
            instCount = 1;
            request->respData = lwm2m_data_new(instCount);
            request->respDataCount = instCount;

        }
        else if(LWM2M_URI_IS_SET_INSTANCE(&request->uri) && LWM2M_URI_IS_SET_RESOURCE(&request->uri))
        {
            //for single instance.
            instCount = 1;
            request->respData = lwm2m_data_new(instCount);
            request->respDataCount = instCount;
            //for single resource;
            attrCount = 1;
        }
    }
    
    if(request->respData == NULL)
    {
        LOG("ReadResponse failed, malloc data error");
        return COAP_500_INTERNAL_SERVER_ERROR;
    }

    if(!LWM2M_URI_IS_SET_INSTANCE(&request->uri) &&
        !LWM2M_URI_IS_SET_RESOURCE(&request->uri))
    {
        instIndex = 0;
        instCount = i;

        for (instanceP = object->instanceList; instanceP != NULL; instanceP = instanceP->next)
        {
            if(instanceP->id == uriP->instanceId)
                break;

            instIndex++;
        }
        //set all resources count;
        attrCount =  nbRes;

    }else if((LWM2M_URI_IS_SET_INSTANCE(&request->uri) && !LWM2M_URI_IS_SET_RESOURCE(&request->uri)))
    {
        //for single instance.
        instIndex = 0;
        instCount = 1;

        //set all resources count;
        attrCount =  nbRes;

    }else if(LWM2M_URI_IS_SET_INSTANCE(&request->uri) && LWM2M_URI_IS_SET_RESOURCE(&request->uri))
    {
        //for single instance.
        instIndex = 0;
        instCount = 1;
        //for single resource;
        attrCount = 1;
    }

    
	tlv_flag = false;
	if (instCount == 1 && attrCount == 1) {
		if (!LWM2M_URI_IS_SET_INSTANCE(&request->uri) && !LWM2M_URI_IS_SET_RESOURCE(&request->uri))
			tlv_flag = true;
	}

    if (instCount == 1 && attrCount == 1 && tlv_flag == false)
    {
        request->respData->type = (lwm2m_data_type_t)(valuetype + 3);
        request->respData->id = uriP->resourceId;

        if(valuetype == lwm2m_opaque || valuetype == lwm2m_string)
        {
            request->respData->value.asBuffer.length = valuelen;
            request->respData->value.asBuffer.buffer = (uint8_t*)lwm2m_malloc(valuelen);
            if(request->respData->value.asBuffer.buffer == NULL)
            {
                return COAP_500_INTERNAL_SERVER_ERROR;
            }
            memcpy(request->respData->value.asBuffer.buffer,value,valuelen);
        }else
        {
            memcpy(&request->respData->value.asInteger,value,valuelen);
        }
    }
    else
    {
        //used asChildren
        if(request->respData[instIndex].value.asChildren.array == NULL)
        {
            request->respData[instIndex].type = LWM2M_TYPE_OBJECT_INSTANCE;
            request->respData[instIndex].id = uriP->instanceId;
            request->respData[instIndex].value.asChildren.array = lwm2m_data_new(attrCount);
            request->respData[instIndex].value.asChildren.count = attrCount;
        }

        //found out index of resource for unused array memory
        for (i = 0;i < nbRes;i++)
        {
            if(request->respData[instIndex].value.asChildren.array[i].type == LWM2M_TYPE_UNDEFINED)
                break;
            attrIndex++;
        }

        //it's all occupied of resource array.
        if(attrIndex >= nbRes)
        {
            LOG("ReadResponse failed, attr index out of bounds");
            return COAP_400_BAD_REQUEST;
        }


        request->respData[instIndex].value.asChildren.array[attrIndex].id = uriP->resourceId;
        request->respData[instIndex].value.asChildren.array[attrIndex].type = (lwm2m_data_type_t)(valuetype + 3);
        if(valuetype == lwm2m_opaque || valuetype == lwm2m_string)
        {
            request->respData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.length = valuelen;
            request->respData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer = (uint8_t*)lwm2m_malloc(valuelen);
            if(request->respData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer == NULL)
            {
                return COAP_500_INTERNAL_SERVER_ERROR;
            }
            memcpy(request->respData[instIndex].value.asChildren.array[attrIndex].value.asBuffer.buffer,value,valuelen);
        }
        else
        {
            memcpy(&request->respData[instIndex].value.asChildren.array[attrIndex].value.asInteger,value,valuelen);
        }
    }
    
    if(result == COAP_231_CONTINUE)
    {
        LOG_ARG("ReadResponse 231_Continue id:0x%x", request->mid);
        return COAP_231_CONTINUE;
    }

TAG_DONE:

    LOG_ARG("ReadResponse Done id:0x%x,Result:(0x%x)",request->mid, result);
    
    //send response message with data packet
    object_asynAckReadData(context,request,result);
    
    lwm2m_data_free(request->respDataCount,request->respData);
    request->respData = NULL;
    request->respDataCount = 0;
        
    return COAP_NO_ERROR;
}

bool packet_asynFindObserveRequest(lwm2m_context_t * context, uint16_t mid)
{
    st_request_t* nodeP;
    
    lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    nodeP = (st_request_t* )LWM2M_LIST_FIND(context->requestList, mid);
    lwm2m_unlock(context->lockRequest);

    if(nodeP != NULL)
        return true;
    else
        return false;
}

bool packet_asynPushRequest(lwm2m_context_t * context, lwm2m_server_t * serverP, const coap_packet_t* packet, lwm2m_callback_type_t type)
{
    st_request_t* nodeP;
    lwm2m_uri_t* uriP;

    LOG_ARG("asyn push message id:%d, type:%d", packet->mid, type);

    nodeP = (st_request_t* )LWM2M_LIST_FIND(context->requestList, packet->mid);
    if(nodeP != NULL)
    {
        if(context->rsp_timeout != 0)
        {
            nodeP->time = lwm2m_gettime();
        }

        return false;
    }

    nodeP = (st_request_t*)lwm2m_malloc(sizeof(st_request_t));
    if(nodeP == NULL) return false;
    memset(nodeP, 0, sizeof(st_request_t));

    uriP = uri_decode(context->altPath, packet->uri_path);
    if (uriP == NULL) 
    {
        lwm2m_free(nodeP);
        return false;
    }
	nodeP->format = utils_convertMediaType(packet->content_type);
    memcpy(nodeP->token, packet->token, packet->token_len);
	nodeP->tokenLen = packet->token_len;
	nodeP->type = type;
    nodeP->uri = *uriP;
    nodeP->mid = packet->mid;
    nodeP->serverP = serverP;
    nodeP->time = lwm2m_gettime();
    nodeP->respData = NULL;
    nodeP->respDataCount = 0;

    lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    context->requestList = (st_request_t *)LWM2M_LIST_ADD(context->requestList, nodeP);
    lwm2m_unlock(context->lockRequest);

    lwm2m_free(uriP);
    return true;
}

void packet_asynRemoveRequest(lwm2m_context_t * context, uint16_t mid, st_request_t** node)
{
    LOG_ARG("asyn remove message id:0x%x, type:%d", mid, (*node)->type);
    SCI_TRACE_LOW( "  LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);

    lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    context->requestList = (st_request_t * )LWM2M_LIST_RM((lwm2m_list_t *)context->requestList, mid, node);

    if((*node)->respData != NULL)
    {
        lwm2m_data_free((*node)->respDataCount,(*node)->respData);
    }

    lwm2m_free(*node);
    *node = NULL;
    lwm2m_unlock(context->lockRequest);
}

void packet_step(lwm2m_context_t *context, time_t currentTime, time_t * timeoutP)
{
    time_t interval;
    st_request_t *targetRequest = NULL;
    st_request_t *delRequest = NULL;
    uint8_t timeout = (context->rsp_timeout != 0 ? context->rsp_timeout : ONENET_CONFIG_CALLBACK_TIMEOUT);

    /*check request node list*/
    lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
    targetRequest = context->requestList;
    lwm2m_unlock(context->lockRequest);
	while(targetRequest != NULL)
	{
		interval = currentTime - targetRequest->time;
		if(interval < timeout)
		{
            lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
            targetRequest = targetRequest->next;
            lwm2m_unlock(context->lockRequest);
            continue;
		}
        else{
            delRequest = targetRequest;
            
            lwm2m_lock(context->lockRequest, LWM2M_CONFIG_LOCK_INFINITY);
            targetRequest = targetRequest->next;     
            lwm2m_unlock(context->lockRequest);

            packet_asynRemoveRequest(context, delRequest->mid, &delRequest);
        }
	}
}

#endif

static void handle_reset(lwm2m_context_t * contextP,
                         void * fromSessionH,
                         coap_packet_t * message)
{
#ifdef LWM2M_CLIENT_MODE
    //OSI_LOGI(0x100076b2, "handle_reset Entering");
    lwm2m_observed_t * observedP;
    OSI_LOGI(0x100076b2, "handle_reset Entering");
    observedP = contextP->observedList;
    while(observedP != NULL)
    {
        lwm2m_watcher_t * watcherP;
        watcherP = observedP->watcherList;
        while(watcherP != NULL)
        {
            if((watcherP->lastMid == message->mid) && lwm2m_session_is_equal(watcherP->server->sessionH, fromSessionH, contextP->userData))
            {
                lwm2mPostEvent(contextP->ref, OBJ_OBSERVE_IND, coap_get_mid(),
                        (uint32_t)observedP->uri.objectId<<16 | observedP->uri.instanceId, (uint32_t)observedP->uri.resourceId<<16 | 0);
                break;
            }
            watcherP = watcherP->next;
        }
        observedP = observedP->next;
    }
    observe_cancel(contextP, message->mid, fromSessionH);
#endif
}

static uint8_t handle_request(lwm2m_context_t * contextP,
                              void * fromSessionH,
                              coap_packet_t * message,
                              coap_packet_t * response)
{
    lwm2m_uri_t * uriP;
    uint8_t result = COAP_IGNORE;

    OSI_LOGI(0x100076b3, "handle_request Entering");
	
#ifdef LWM2M_CLIENT_MODE
    uriP = uri_decode(contextP->altPath, message->uri_path);
#else
    uriP = uri_decode(NULL, message->uri_path);
#endif

    if (uriP == NULL) return COAP_400_BAD_REQUEST;

    switch(uriP->flag & LWM2M_URI_MASK_TYPE)
    {
#ifdef LWM2M_CLIENT_MODE
    case LWM2M_URI_FLAG_DM:
    {
        lwm2m_server_t * serverP;

        serverP = utils_findServer(contextP, fromSessionH);
        if (serverP != NULL)
        {
            result = dm_handleRequest(contextP, uriP, serverP, message, response);
        }
#ifdef LWM2M_BOOTSTRAP
        else
        {
            serverP = utils_findBootstrapServer(contextP, fromSessionH);
            if (serverP != NULL)
            {
                result = bootstrap_handleCommand(contextP, uriP, serverP, message, response);
            }
        }
#endif
    }
    break;

#ifdef LWM2M_BOOTSTRAP
    case LWM2M_URI_FLAG_DELETE_ALL:
        if (COAP_DELETE != message->code)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            result = bootstrap_handleDeleteAll(contextP, fromSessionH);
        }
        break;

    case LWM2M_URI_FLAG_BOOTSTRAP:
        if (message->code == COAP_POST)
        {
            result = bootstrap_handleFinish(contextP, fromSessionH);
        }
        break;
#endif
#endif

#ifdef LWM2M_SERVER_MODE
    case LWM2M_URI_FLAG_REGISTRATION:
        result = registration_handleRequest(contextP, uriP, fromSessionH, message, response);
        break;
#endif
#ifdef LWM2M_BOOTSTRAP_SERVER_MODE
    case LWM2M_URI_FLAG_BOOTSTRAP:
        result = bootstrap_handleRequest(contextP, uriP, fromSessionH, message, response);
        break;
#endif
    default:
        result = COAP_IGNORE;
        break;
    }

    coap_set_status_code(response, result);

    if (COAP_IGNORE < result && result < COAP_400_BAD_REQUEST)
    {
        result = NO_ERROR;
    }

    lwm2m_free(uriP);
    return result;
}

#ifdef CONFIG_LWM2M_ONENET_UPDATE
extern int32_t reset_fotaIDIL(lwm2m_context_t* context, uint32_t msgid);
uint32_t   cissys_writeFwBytes(lwm2m_context_t * lwm2mH, firmware_data_t* data,uint32_t size, uint8_t* buffer);

uint32_t writeFotaUpgradeData(uint32_t block1_num, uint8_t  block1_more, uint8_t * data, uint16_t datalen)
{
    static int fd = -1;
    /*ssize_t write_len = 0;*/ //2 alvin: avoid waraning: set but not used.

    OSI_LOGI(0x10009496, "writeFotaUpgradeData savebyte = %d", cissys_getFwSavedBytes());
    if (0==block1_num && 0==cissys_getFwSavedBytes())
    {
        //fupdateInvalidate(true);   //MODIFY BY xiameng.yang
        //vfs_mkdir(CONFIG_FS_FOTA_DATA_DIR, 0);  //MODIFY BY xiameng.yang
        //fd  = vfs_open(FUPDATE_PACK_FILE_NAME, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0); //MODIFY BY xiameng.yang
        if (fd < 0){
            OSI_LOGI(0x10009497, "writeFotaUpgradeData fd=%d", fd);
            return COAP_IGNORE;}
    }

    if (fd < 0)
        return COAP_IGNORE;
    #if 0    ////MODIFY BY xiameng.yang
    OSI_LOGI(0x10009498, "writeFotaUpgradeData filesize = %d", vfs_file_size(FUPDATE_PACK_FILE_NAME));
    
    if (vfs_file_size(FUPDATE_PACK_FILE_NAME) == cissys_getFwSavedBytes())
    {
        /*write_len = */vfs_write(fd, data, datalen);
    }
    else if (vfs_file_size(FUPDATE_PACK_FILE_NAME) > cissys_getFwSavedBytes())
    {
        vfs_lseek(fd, cissys_getFwSavedBytes(), SEEK_SET);
        /*write_len = */vfs_write(fd, data, datalen);
    }
    else
    {
        fupdateInvalidate(true);
        OSI_LOGI(0x10009499, "vfs_file_size(FUPDATE_PACK_FILE_NAME) < cissys_getFwSavedBytes() return 1");
        return COAP_IGNORE;
    }

    if(!block1_more)
    {
        vfs_close(fd);
    }
    #endif
    return COAP_NO_ERROR;
}

coap_status_t prv_packet_handle_fota_block(lwm2m_context_t * contextP,
    firmware_data_t * data,
	lwm2m_uri_t *uriP,
	coap_packet_t *response,
	coap_packet_t * message,
	lwm2m_server_t* serverP)
{
    coap_status_t coap_error_code = COAP_NO_ERROR;
    // st_request_t * request = NULL;
    // parse block1 header
    uint32_t block1_num;
    uint8_t  block1_more;
    uint16_t block1_size;
    coap_get_header_block1(message, &block1_num, &block1_more, &block1_size, NULL);
    OSI_LOGI(0x1000949a, "Blockwise: block1 request NUM %u (SZX %u/ SZX Max%u) MORE %u", block1_num, block1_size, REST_MAX_CHUNK_SIZE, block1_more);

    //if not in downloading mode, just return 400
    if (data == NULL)
    {
        st_request_t* request;
        OSI_LOGI(0x1000949b, "No firmware instance");
        //object_asynAckBlockWrite(contextP,response,COAP_400_BAD_REQUEST,contextP->fw_request.ack_type);
        //st_request_t* request;
        request = (st_request_t *)lwm2m_malloc(sizeof(st_request_t));
        if (request == NULL)
        {
            lwm2m_free(contextP);
            return -1;
        }
        memset(request,0,sizeof(struct st_request));
        request->mid = message->mid;

        memcpy(request->token,message->token,message->token_len);
        request->tokenLen = message->token_len;
        request->uri = *uriP;
        request->serverP = serverP;
        object_asynAckNodata(contextP,request,COAP_400_BAD_REQUEST);
        lwm2m_free(request);
        request=NULL;

        coap_error_code = COAP_400_BAD_REQUEST;
        return coap_error_code;
    }

    if (data->state != FOTA_STATE_DOWNLOADING)
    {
        OSI_LOGI(0x1000949c, "not downloading mode");
        object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,(uint8_t)COAP_400_BAD_REQUEST,contextP->fw_request.ack_type);
        //object_asynAckNodata(contextP,response,COAP_400_BAD_REQUEST);
        coap_error_code = COAP_400_BAD_REQUEST;
        return coap_error_code;
    }
    if(message->mid<contextP->fw_request.block1_mid)
    return COAP_IGNORE;

    if (contextP->fw_request.block1_mid == message->mid) //repeat message
    {
        OSI_LOGI(0x1000949d, "Block packet repeat,msgid:%d,contextP->fw_request.write_state=%d",contextP->fw_request.write_state,message->mid);
        switch(contextP->fw_request.write_state)
        {
            case PACKAGE_WRITE_FAIL:
            case PACKAGE_WRITE_SUCCESS:
            {
                OSI_LOGI(0x1000949e, "ack lost,need resend, msgid:%d",message->mid);
                contextP->fw_request.block1_num = block1_num;
                object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,contextP->fw_request.code,contextP->fw_request.ack_type);
            }
            break;
            case PACKAGE_WRITE_IDIL:
            if (contextP->fw_request.code == COAP_231_CONTINUE || contextP->fw_request.code == COAP_204_CHANGED)
            {
                contextP->fw_request.block1_num = block1_num;
                object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,contextP->fw_request.code,contextP->fw_request.ack_type);
            }
            case PACKAGE_WRITING:
            break;
            default:
            break;
        }
        return coap_error_code;
    }

    contextP->fw_request.block1_more = block1_more;
    contextP->fw_request.ack_type = response->type; //ACK or NON
    contextP->fw_request.code = response->code;     //205
    contextP->fw_request.block1_mid=message->mid;
    //	if (contextP->fw_request.mid == 0) //first write message
    OSI_LOGI(0x10009850, "block1_num=%d, write_state=%d",block1_num,contextP->fw_request.write_state);

    if(0==block1_num)
    {
        if (contextP->fw_request.write_state == PACKAGE_WRITE_IDIL)
        {
            //normal write process
            memcpy(contextP->fw_request.response_ack->token,message->token,message->token_len);
            contextP->fw_request.response_ack->tokenLen = message->token_len;
            contextP->fw_request.response_ack->type = CALLBACK_BLOCK_WRITE;
            contextP->fw_request.response_ack->uri = *uriP;
            contextP->fw_request.block1_num = block1_num;
            contextP->fw_request.response_ack->mid =  message->mid;
        }
        else
        {
            object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,COAP_400_BAD_REQUEST,contextP->fw_request.ack_type);
            return coap_error_code;
        }
    }
    else
    {
        if (contextP->fw_request.block1_num+1 == block1_num) //expect block
        {
            OSI_LOGI(0x100094a0, "contextP->fw_request.block1_num=%d,block1_num:%d",contextP->fw_request.block1_num,block1_num);
            contextP->fw_request.block1_num = block1_num;
            contextP->fw_request.response_ack->mid =  message->mid;
            if (contextP->fw_request.write_state == PACKAGE_WRITE_SUCCESS)
            {
                contextP->fw_request.write_state = PACKAGE_WRITE_IDIL;
            }else if (contextP->fw_request.write_state == PACKAGE_WRITE_FAIL)
            {
                //retry or just notify the result
                object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,COAP_408_REQ_ENTITY_INCOMPLETE,contextP->fw_request.ack_type);
                coap_error_code = COAP_408_REQ_ENTITY_INCOMPLETE;
                return coap_error_code;
            }
            contextP->fw_request.write_state = PACKAGE_WRITE_IDIL;
        }
        else
        {
            //not expect block, report 408 error to platform
            OSI_LOGI(0x100094a1, "Un expected Block packet,block1_num:%d",block1_num);
            contextP->fw_request.block1_num = block1_num;
            object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,COAP_408_REQ_ENTITY_INCOMPLETE,contextP->fw_request.ack_type);
            reset_fotaIDIL(contextP,COAP_408_REQ_ENTITY_INCOMPLETE);
            coap_error_code = COAP_408_REQ_ENTITY_INCOMPLETE;
            return coap_error_code;
        }
    }

    if(block1_more)
    {
        coap_error_code = COAP_231_CONTINUE;
        block1_size = MIN(block1_size, REST_MAX_CHUNK_SIZE);
        coap_set_header_block1(response,block1_num, block1_more,block1_size);
    }
    else   //last block
    {
        coap_error_code = COAP_NO_ERROR;
    }

    contextP->fw_request.block1_size = block1_size;
    contextP->fw_request.write_state = PACKAGE_WRITING;
    contextP->fw_request.last_packet_size = message->payload_len;

#ifdef CONFIG_LWM2M_ONENET_SOTA
    if(contextP->isupdate_mcu == false) // FOTA
#endif
    {
        uint32_t result = writeFotaUpgradeData(block1_num, block1_more, message->payload, message->payload_len);
        LOG_ARG("writeFotaUpgradeData return :%d",result);
        if (COAP_NO_ERROR != result)
        {
            object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,COAP_408_REQ_ENTITY_INCOMPLETE,contextP->fw_request.ack_type);
            reset_fotaIDIL(contextP,COAP_408_REQ_ENTITY_INCOMPLETE);
            return coap_error_code;
        }
    }

#ifdef CONFIG_LWM2M_ONENET_SOTA
    if (contextP->isupdate_mcu == false)
    {
        if (0 != cissys_writeFwBytes(contextP,data, message->payload_len, message->payload))
        {
            object_asynAckBlockWrite(contextP, contextP->fw_request.response_ack, COAP_408_REQ_ENTITY_INCOMPLETE, contextP->fw_request.ack_type);
            reset_fotaIDIL(contextP, COAP_408_REQ_ENTITY_INCOMPLETE);
            return coap_error_code;
        }
    }
    else
    {
        lwm2m_data_t * dataP = NULL;
        write_param_t *writehead = NULL;
        write_param_t *writelist = NULL;
        uint32_t paramCount = 0;        
        uint16_t resIndex;
        uint16_t resCount = 0;

        resCount = (uint16_t)lwm2m_data_parse(uriP, message->payload, message->payload_len, LWM2M_CONTENT_OPAQUE, &dataP);

        if (resCount == 0) 
        {
            return COAP_406_NOT_ACCEPTABLE;
        } 

        OSI_LOGI(0x10009851, "resCount is %d",resCount);
        for (resIndex = 0;resIndex < resCount;resIndex++)
        {
            write_param_t *writeparam = (write_param_t *)lwm2m_malloc(sizeof(write_param_t));
            writeparam->objid = uriP->objectId;
            writeparam->insid = uriP->instanceId;
            writeparam->resid = uriP->resourceId;
            writeparam->type = (lwm2m_tpye_t)(dataP[resIndex].type -3);
            writeparam->data = utils_bufferdup(dataP[resIndex].value.asBuffer.buffer,dataP[resIndex].value.asBuffer.length);
            writeparam->len = dataP[resIndex].value.asBuffer.length;
            writeparam->next = NULL;
            if (writelist != NULL)
            {
                writelist->next = writeparam;
            }
            writelist = writeparam;
            if (writehead == NULL)
            {
                writehead = writelist;
            }
            paramCount++;
        }

        lwm2m_data_free(resCount, dataP);
        if (writehead != NULL)
        {
            contextP->fw_request.need_async_ack = true;    
            lwm2mPostEvent(contextP->ref, OBJ_WRITE_IND, coap_get_mid(), (uint32_t)writehead, paramCount);
        }
    }
#else
    if (0 != cissys_writeFwBytes(contextP,data,message->payload_len,message->payload))
    {
        object_asynAckBlockWrite(contextP,contextP->fw_request.response_ack,COAP_408_REQ_ENTITY_INCOMPLETE,contextP->fw_request.ack_type);
        reset_fotaIDIL(contextP,COAP_408_REQ_ENTITY_INCOMPLETE);
        return coap_error_code;
    }
#endif
    return coap_error_code;
}
#endif

/* This function is an adaptation of function coap_receive() from Erbium's er-coap-13-engine.c.
 * Erbium is Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 */
void lwm2m_handle_packet(lwm2m_context_t * contextP,
                         uint8_t * buffer,
                         int length,
                         void * fromSessionH)
{
    uint8_t coap_error_code = NO_ERROR;
    static coap_packet_t message[1];
    static coap_packet_t response[1];

    OSI_LOGI(0x100076b4, " lwm2m_handle_packet Entering------->");
    coap_error_code = coap_parse_message(message, buffer, (uint16_t)length);
    if (coap_error_code == NO_ERROR)
    {
        LOG_ARG("Parsed: ver %u, type %u, tkl %u, code %u.%.2u, mid %u, Content type: %d, payload_len:%d",
                message->version, message->type, message->token_len, message->code >> 5, message->code & 0x1F, message->mid, message->content_type,message->payload_len);
        //LOG_ARG("Payload:%d, %.*s", message->payload_len, message->payload);
        /* 8811 NB-IoT stack lost log due to too much printing.
        for(int i=0; i<message->payload_len; i++)
        {
            LOG_ARG("payload[%d]:0x%x", i, message->payload[i]);
        }
        */
        if (message->code >= COAP_GET && message->code <= COAP_DELETE)
        {
            uint32_t block_num = 0;
            uint16_t block_size = REST_MAX_CHUNK_SIZE;
            uint32_t block_offset = 0;
            int64_t new_offset = 0;

            /* prepare response */
            if (message->type == COAP_TYPE_CON)
            {
                /* Reliable CON requests are answered with an ACK. */
                coap_init_message(response, COAP_TYPE_ACK, COAP_205_CONTENT, message->mid);
            }
            else
            {
                /* Unreliable NON requests are answered with a NON as well. */
                coap_init_message(response, COAP_TYPE_NON, COAP_205_CONTENT, contextP->nextMID++);
            }

            /* mirror token */
            if (message->token_len)
            {
                coap_set_header_token(response, message->token, message->token_len);
            }

            /* get offset for blockwise transfers */
            if (coap_get_header_block2(message, &block_num, NULL, &block_size, &block_offset))
            {
                LOG_ARG("Blockwise: block request %u (%u/%u) @ %u bytes", block_num, block_size, REST_MAX_CHUNK_SIZE, block_offset);
                block_size = MIN(block_size, REST_MAX_CHUNK_SIZE);
                new_offset = block_offset;
            }

            /* handle block1 option */
            if (IS_OPTION(message, COAP_OPTION_BLOCK1))
            {
#ifdef LWM2M_CLIENT_MODE
                // get server
                lwm2m_server_t * serverP;
                serverP = utils_findServer(contextP, fromSessionH);
#ifdef LWM2M_BOOTSTRAP
                if (serverP == NULL)
                {
                    serverP = utils_findBootstrapServer(contextP, fromSessionH);
                }
#endif
                if (serverP == NULL)
                {
                    coap_error_code = COAP_500_INTERNAL_SERVER_ERROR;
                }
                else
                {
                    uint32_t block1_num;
                    uint8_t  block1_more;
                    uint16_t block1_size;
                    uint8_t * complete_buffer = NULL;
                    size_t complete_buffer_size;

                    // parse block1 header
                    coap_get_header_block1(message, &block1_num, &block1_more, &block1_size, NULL);
                    LOG_ARG("Blockwise: block1 request NUM %u (SZX %u/ SZX Max%u) MORE %u", block1_num, block1_size, REST_MAX_CHUNK_SIZE, block1_more);
#ifdef CONFIG_LWM2M_ONENET_UPDATE
                    if(contextP->serverType == LWM2M_SERVER_ONENET){
                        //LOG("contextP->serverType == LWM2M_SERVER_ONENET");
                        
                        lwm2m_uri_t *uriP=NULL;
                        LOG("contextP->serverType == LWM2M_SERVER_ONENET");
                        uriP = uri_decode(contextP->altPath, message->uri_path);
                        if(uriP==NULL)
                            return;
                        if(/*uriP->flag == 7 && */uriP->objectId == LWM2M_FIRMWARE_UPDATE_OBJECT_ID && uriP->instanceId == 0/* && uriP->resourceId == RES_M_PACKAGE*/)
                        {
                            lwm2m_object_t* objectP;
                            firmware_data_t * data ;
                            contextP->fw_request.response_ack->serverP = serverP;
                             objectP = (lwm2m_object_t*)LWM2M_LIST_FIND(contextP->objectList,LWM2M_FIRMWARE_UPDATE_OBJECT_ID);
                            data = (firmware_data_t*)(objectP->userData);
                            prv_packet_handle_fota_block(contextP,data,uriP,response,message,serverP);
                            lwm2m_free(uriP);
                            coap_free_header(message);
                            return;
                        }
                        lwm2m_free(uriP);
                    } else
#endif
                    {
                        // handle block 1
                        coap_error_code = coap_block1_handler(&serverP->block1Data, message->mid, message->payload, message->payload_len, block1_size, block1_num, block1_more, &complete_buffer, &complete_buffer_size);

                        // if payload is complete, replace it in the coap message.
                        if (coap_error_code == NO_ERROR)
                        {
                            message->payload = complete_buffer;
                            message->payload_len = complete_buffer_size;
                        }
                        else if (coap_error_code == COAP_231_CONTINUE)
                        {
                            block1_size = MIN(block1_size, REST_MAX_CHUNK_SIZE);
                            coap_set_header_block1(response,block1_num, block1_more,block1_size);
                        }
                    }
                }
#else
                coap_error_code = COAP_501_NOT_IMPLEMENTED;
#endif
            }
            if (coap_error_code == NO_ERROR)
            {
                coap_error_code = handle_request(contextP, fromSessionH, message, response);
            }
            if (coap_error_code == NO_ERROR)
            {
                if (IS_OPTION(message, COAP_OPTION_BLOCK2))
                {
                    /* unchanged new_offset indicates that resource is unaware of blockwise transfer */
                    if (new_offset==block_offset)
                    {
                        LOG_ARG("Blockwise: unaware resource with payload length %u/%u", response->payload_len, block_size);
                        if (block_offset >= response->payload_len)
                        {
                            OSI_LOGI(0x100076b5, "handle_incoming_data(): block_offset >= response->payload_len");

                            response->code = COAP_402_BAD_OPTION;
                            coap_set_payload(response, "BlockOutOfScope", 15); /* a const char str[] and sizeof(str) produces larger code size */
                        }
                        else
                        {
                            coap_set_header_block2(response, block_num, response->payload_len - block_offset > block_size, block_size);
                            coap_set_payload(response, response->payload+block_offset, MIN(response->payload_len - block_offset, block_size));
                        } /* if (valid offset) */
                    }
                    else
                    {
                        /* resource provides chunk-wise data */
                        LOG_ARG("Blockwise: blockwise resource, new offset %d", (int) new_offset);
                        coap_set_header_block2(response, block_num, new_offset!=-1 || response->payload_len > block_size, block_size);
                        if (response->payload_len > block_size) coap_set_payload(response, response->payload, block_size);
                    } /* if (resource aware of blockwise) */
                }
                else if (new_offset!=0)
                {
                    LOG_ARG("Blockwise: no block option for blockwise resource, using block size %u", REST_MAX_CHUNK_SIZE);

                    coap_set_header_block2(response, 0, new_offset!=-1, REST_MAX_CHUNK_SIZE);
                    coap_set_payload(response, response->payload, MIN(response->payload_len, REST_MAX_CHUNK_SIZE));
                } /* if (blockwise request) */

                coap_error_code = message_send(contextP, response, fromSessionH);

                lwm2m_free(response->payload);
                response->payload = NULL;
                response->payload_len = 0;
            }
            else if (coap_error_code != COAP_IGNORE)
            {
                if (1 == coap_set_status_code(response, coap_error_code))
                {
                    coap_error_code = message_send(contextP, response, fromSessionH);
                }
            }
        }
        else if(message->code == CONTENT_2_05)
        {
            if(IS_OPTION(message,COAP_OPTION_BLOCK2))
            {
                //lwm2m_server_t * serverP;
                //serverP = utils_findServer(contextP, fromSessionH);
                //if(serverP != NULL)
                {
                    uint32_t block2_num;
                    uint8_t block2_more;
                    uint16_t block2_size;
                    coap_get_header_block2(message, &block2_num, &block2_more,&block2_size,NULL);
                    coap_error_code = lwm2m_fota_block2_handler(contextP,  message->mid, message->payload, message->payload_len, block2_size, block2_num, block2_more, message);
                    LOG_ARG("lwm2m_fota_block2_handler coap_error_code %d\r\n",coap_error_code);
                }
            }
        }
        else
        {
            /* Responses */
            switch (message->type)
            {
            case COAP_TYPE_NON:
            case COAP_TYPE_CON:
                {
                    bool done = transaction_handleResponse(contextP, fromSessionH, message, response);

#ifdef LWM2M_SERVER_MODE
                    if (!done && IS_OPTION(message, COAP_OPTION_OBSERVE) &&
                        ((message->code == COAP_204_CHANGED) || (message->code == COAP_205_CONTENT)))
                    {
                        done = observe_handleNotify(contextP, fromSessionH, message, response);
                    }
#endif
                    if (!done && message->type == COAP_TYPE_CON )
                    {
                        coap_init_message(response, COAP_TYPE_ACK, 0, message->mid);
                        coap_error_code = message_send(contextP, response, fromSessionH);
                    }
                }
                break;

            case COAP_TYPE_RST:
                /* Cancel possible subscriptions. */
                handle_reset(contextP, fromSessionH, message);
                transaction_handleResponse(contextP, fromSessionH, message, NULL);
                break;

            case COAP_TYPE_ACK:
                transaction_handleResponse(contextP, fromSessionH, message, NULL);
                break;

            default:
                break;
            }
        } /* Request or Response */
        coap_free_header(message);
    } /* if (parsed correctly) */
    else
    {
        LOG_ARG("Message parsing failed %u.%2u", coap_error_code >> 5, coap_error_code & 0x1F);
    }

    if (coap_error_code != NO_ERROR && coap_error_code != COAP_IGNORE)
    {
        LOG_ARG("ERROR %u: %s", coap_error_code, coap_error_message);

        /* Set to sendable error code. */
        if (coap_error_code >= 192)
        {
            coap_error_code = COAP_500_INTERNAL_SERVER_ERROR;
        }
        /* Reuse input buffer for error message. */
        coap_init_message(message, COAP_TYPE_ACK, coap_error_code, message->mid);
        coap_set_payload(message, coap_error_message, strlen(coap_error_message));
        message_send(contextP, message, fromSessionH);
    }
}


uint8_t message_send(lwm2m_context_t * contextP,
                     coap_packet_t * message,
                     void * sessionH)
{
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    uint8_t * pktBuffer;
    size_t pktBufferLen = 0;
    size_t allocLen;

    OSI_LOGI(0x100076b6, "message_send Entering");
    allocLen = coap_serialize_get_size(message);
    LOG_ARG("Size to allocate: %d", allocLen);
    if (allocLen == 0) return COAP_500_INTERNAL_SERVER_ERROR;

    pktBuffer = (uint8_t *)lwm2m_malloc(allocLen);
    if (pktBuffer != NULL)
    {
        pktBufferLen = coap_serialize_message(message, pktBuffer);
        LOG_ARG("coap_serialize_message() returned %d", pktBufferLen);
        /* 8811 NB-IoT stack lost log due to too much printing.
        for (int i=0;i<message->payload_len;i++)
        {
            OSI_LOGI(0x100076b7, "message_send: payload[%d]=0x%x",i,message->payload[i]);
        }
        */
        if (0 != pktBufferLen)
        {
            result = lwm2m_buffer_send(sessionH, pktBuffer, pktBufferLen, contextP->userData);
        }
        lwm2m_free(pktBuffer);
    }

    return result;
}

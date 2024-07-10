/******************************************************************************
 ** File Name:   acm.h                                                       *
 ** Date:        11/02/2005                                                   *
 ** Copyright:   2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description: This file supports datatypes and prototype of functions in   *
                    audio collision manager.                                  *
*******************************************************************************/
#ifndef _ACM_H_
#define _ACM_H_

/******************************************************************************
 **                          Dependencies                                     *
*******************************************************************************/
#include "sci_types.h"
#include "audio_api.h"

/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
enum AUDIO_Collision_SubHandleType
{
    INVALID_COLLISIONSUBHANDLETYPE = 0, 
    COLLISION_CLASS_HANDLE,
    COLLISION_OBJECT_HANDLE
};

/******************************************************************************
 **                          Data Structures                                  *
*******************************************************************************/
typedef struct AUDIO_Collision_Object_struct AUDIO_COLLISION_OBJECT_T;

typedef struct AUDIO_CM_Instance_struct
{
    HAUDIO hAudio;
    AUDIO_COLLISION_OBJECT_T *ptAudioCollisionObject;
    int32 InstanceNeedStop;

    struct AUDIO_CM_Instance_struct *ptNextInstance;
    struct AUDIO_CM_Instance_struct *ptPrevInstance;
}AUDIO_CM_INSTANCE_T;

typedef AUDIO_RESULT_E (*AUDIO_CM_POLICY)(
AUDIO_COLLISION_OBJECT_T *ptCollisionObject,
AUDIO_CM_INSTANCE_T      *ptAcmInstanceLListHeader,
HCOLLISIONCLASS          hClass);

typedef struct AUDIO_CM_Class_struct
{
    uint16 szClassName[16];
    HCOLLISIONCLASS hClass;
    uint32 uiPrivateBufferSize;     //byte number
    
    AUDIO_CM_POLICY pfClassPolicy;
    AUDIO_CM_INSTANCE_T *ptAcmInstanceLListHeader;
    
    struct AUDIO_CM_Class_struct *ptNextClass;
    struct AUDIO_CM_Class_struct *ptPrevClass;
}AUDIO_CM_CLASS_T;

typedef struct AUDIO_CM_ClassObject_struct
{
    AUDIO_CM_CLASS_T *ptAcmClass;
    void *pvPrivateBuffer;

    struct AUDIO_CM_ClassObject_struct *ptNextClassObject;
    struct AUDIO_CM_ClassObject_struct *ptPrevClassObject;
}AUDIO_CM_CLASSOBJECT_T;

struct AUDIO_Collision_Object_struct
{
    uint32 uiPriority;
    HCOLLISION hCollision;                  //collision ID
    AUDIO_CM_CLASSOBJECT_T *ptClassObjectLListHeader;

    struct AUDIO_Collision_Object_struct *ptNextCollisionObject;
    struct AUDIO_Collision_Object_struct *ptPrevCollisionObject;
};

/******************************************************************************
 **                          Function prototype                               *
*******************************************************************************/
/******************************************************************************
 ** Description: audio collision manager initialization                       *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_Init(void);

/******************************************************************************
 ** Description: create an audio collision class                              *
 ** Input:                                                                    *
 **     pszName: the name of the class to be created                          *
 **     pfAcmPolicy: class policy(callback function)                          *
 **     uiPrivateBufferSize: the byte number needed for class private info    *
 ** Return: return class ID. if return 0xffffffff, error encountered          *
*******************************************************************************/
PUBLIC uint32 AUDIO_CM_CreateCollisionClass(
uint16 *pszName,
AUDIO_CM_POLICY pfAcmPolicy,
uint32 uiPrivateBufferSize
);

/******************************************************************************
 ** Description: delete an audio collision class                              *
 ** Input:                                                                    *
 **     uiClassId: the ID of the class to be deleted                          *
 ** Return: the result of the delete operation                                *
 ** Notes: if any collision object uses the class, it can't be deleted        *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_DeleteCollisionClass(HCOLLISIONCLASS hClass);

/******************************************************************************
 ** Description: create an audio collision object                             *
 ** Input:                                                                    *
 **     uiPriority: priority of the collision object to be created            *
 ** Return: return handle of collision object created                         *
 ** Notes:  the collision object ID is created by calling function in ASM     *
*******************************************************************************/
PUBLIC HCOLLISION AUDIO_CM_CreateCollisionObject(uint32 uiPriority);

/******************************************************************************
 ** Description: configure the priority of the collision object               *
 ** Input:                                                                    *
 **     uiPriority                                                            *
 **     hCollision: the ID of the audio collision object                      *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_SetCollisionObjectPriority(
uint32 uiPriority,
HCOLLISION hCollision);

/******************************************************************************
 ** Description: delete an audio collision object                             *
 ** Input:                                                                    *
 **     hCollision: the ID of the object to be deleted                        *
 ** Notes:                                                                    *
 **     The collision object can't be deleted if any ACM instance uses it     *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_DeleteCollisionObject(HCOLLISION hCollision);

/******************************************************************************
 ** Description: register a class to a collison object                        *
 ** Input:                                                                    *
 **     uiClassId: ID of the class to be registered to the audio object       *
 **     hCollision: the ID of the audio collision object                      *
 **     ptClassEntity: pointer to the buffer stored private information       *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_RegCollisionClass(
HCOLLISIONCLASS hClass,
HCOLLISION hCollision,
void *ptClassEntity);

/******************************************************************************
 ** Description: unregister a class from a collison object                    *
 ** Input:                                                                    *
 **     uiClassId: ID of the class to be unregistered.                        *
 **     hCollision: the ID of the audio collision object                      *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_UnregCollisionClass(
HCOLLISIONCLASS hClass,
HCOLLISION hCollision);

/******************************************************************************
 ** Description: judge the collision                                          *
 ** Input:                                                                    *
 **     hAudio: the audio instance ID to be checked                           *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_EnterDispose(const HAUDIO hAudio);

/******************************************************************************
 ** Description: delete the audio instance from ACM                           *
 ** Input:                                                                    *
 **     hAudio: the audio instance ID to be operated                          *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_ExitDispose(HAUDIO hAudio);

/******************************************************************************
 ** Description: set need stop flag                           *
 ** Input:                                                                    *
 **     hAudio: the audio instance ID to be operated                          *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_CM_SetNeedStopFlag(HAUDIO hAudio);

/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif //_ACM_H_

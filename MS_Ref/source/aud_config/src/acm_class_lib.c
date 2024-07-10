/******************************************************************************
 ** File Name:   acm_class_lib.c                                              *
 ** Date:        12/02/2005                                                   *
 ** Copyright:   2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description: This file defines audio collision policy.                    *
*******************************************************************************/
/******************************************************************************
 **                          Dependencies                                     *
*******************************************************************************/
#include "ms_ref_aud_config_trc.h"
#include "acm.h"
#include "aud_gen.h"
#include "asm.h"
#include "asm_hc.h"
#include "acm_class_lib.h"
/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************************************
 **                          Function Definitions                             *
*******************************************************************************/
/******************************************************************************
 ** audio collision class policy:                                             *
 **     if level in private information is the same                           *
 **        they can be played in the meantime                                 *
 **     else                                                                  *
 **        if input instance priority is higher than or equal to the playing  *
 **            the input instance can't be played and the playing instances   *
 **            go on playing.                                                 *
 **        else                                                               *
 **            the input instance can be played and the playing instances     *
 **            stop playing.                                                  *
 ** notes:                                                                    *
 **     this policy uses private informations--level.                         *
 **     in a class which uses this policy                                     *
 **         codecs used DSP VB is a level, including AMR, voice,              *
 **                                      custom tone, dtmf tone, generic tone.*
 **         codecs used ARM VB is another level, including MIDI, MP3,         *
 **                                                       ADPCM, AAC, PCM.    *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E ACM_Policy_DifferentLevelCollide(
AUDIO_COLLISION_OBJECT_T *ptCollisionObject,        //the collision object in the audio instance to be played
AUDIO_CM_INSTANCE_T      *ptAcmInstanceLListHeader, //instance linklist header in a class
HCOLLISIONCLASS          hClass                     //class handle
)
{
    AUDIO_CM_INSTANCE_T *ptCurAcmInstanceInClass = NULL;
    AUDIO_CM_CLASSOBJECT_T *ptCurAcmClassObject = NULL;
    int32 level_of_input_collision = 0;
    int32 level_of_cur_instance = 0;
 
    AUDIO_RESULT_E result = AUDIO_ERROR;

    //check input parameters
    if (NULL == ptCollisionObject)
    {
        return AUDIO_PARAM_ERROR;
    }
    // Check the validity of the class handle
    result = AUDIO_SMHC_VerifyHandle(hClass, COLLISION_HANDLE, COLLISION_CLASS_HANDLE);
    if(result != AUDIO_NO_ERROR)        
    {
        return result;
    }    
    result = AUDIO_CM_CAN_PLAY;

    //find the input private information level
    ptCurAcmClassObject = ptCollisionObject->ptClassObjectLListHeader;
    while (NULL != ptCurAcmClassObject)
    {
        if (ptCurAcmClassObject->ptAcmClass->hClass == hClass)
        {
            level_of_input_collision = ((CLASS1PRIVATEINFO_T *)(ptCurAcmClassObject->pvPrivateBuffer))->level;
            break;
        }
        ptCurAcmClassObject = ptCurAcmClassObject->ptNextClassObject;
    }
    if (NULL == ptCurAcmClassObject)
    {
         //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_78_112_2_18_1_1_59_0,(uint8*)"");
        ASM_ASSERT_WARNING(FALSE);
        
      return AUDIO_ERROR;
    }

    //judge the collision between input and each instance in the class
    ptCurAcmInstanceInClass = ptAcmInstanceLListHeader;
    while (NULL != ptCurAcmInstanceInClass)
    {
        //find the level for each instance.
        ptCurAcmClassObject = ptCurAcmInstanceInClass->ptAudioCollisionObject->ptClassObjectLListHeader;
        while (NULL != ptCurAcmClassObject)
        {
            if (ptCurAcmClassObject->ptAcmClass->hClass == hClass)
            {
                level_of_cur_instance = ((CLASS1PRIVATEINFO_T *)(ptCurAcmClassObject->pvPrivateBuffer))->level;
                break;
            }
            ptCurAcmClassObject =ptCurAcmClassObject->ptNextClassObject;
        }
        if (NULL == ptCurAcmClassObject)
        {
            //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:NULL == ptCurAcmClassObject"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_101_112_2_18_1_1_59_1,(uint8*)"");
               ASM_ASSERT_WARNING(FALSE);
        
          return AUDIO_ERROR;
        }

        //judge
        if (level_of_input_collision != level_of_cur_instance)
        {
            if (ptCollisionObject->uiPriority < ptCurAcmInstanceInClass->ptAudioCollisionObject->uiPriority)
            {
                result = AUDIO_CM_CANNOT_PLAY;
                break;
            }
            else
            {
                //stop the playing instance
                if (AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag(ptCurAcmInstanceInClass->hAudio))
                {
                    //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_DifferentLevelCollide:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_120_112_2_18_1_1_59_2,(uint8*)"");
                       ASM_ASSERT_WARNING(FALSE);
        
                 return AUDIO_ERROR;
                }
            }
        }
        
        ptCurAcmInstanceInClass = ptCurAcmInstanceInClass->ptNextInstance;
    }    
    return result;
}


/******************************************************************************
 ** audio collision class policy:                                             *
 **     if priority of the input instance is higher than or equal to the      *
 **     playing instance                                                      *
 **        stop playing isntance and the input instance can be played.        *
 **     else                                                                  *
 **        the input instance can't be played and the playing instance go on. *
 ** notes:                                                                    *
 **     this policy uses no private informations.                             *
 **     following codec is exclusive: MIDI, MP3, AAC.                         *
 **         so they must use the same class which adopts this policy.         *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E ACM_Policy_Exclusive(
AUDIO_COLLISION_OBJECT_T *ptCollisionObject,        //the collision object in the audio instance to be played
AUDIO_CM_INSTANCE_T      *ptAcmInstanceLListHeader, //instance linklist header in a class
HCOLLISIONCLASS          hClass                     //class handle
)
{
    AUDIO_CM_INSTANCE_T *ptCurAcmInstanceInClass = NULL;
    AUDIO_RESULT_E result = AUDIO_ERROR;

    //check input parameters
    if (NULL == ptCollisionObject)
    {
        return AUDIO_PARAM_ERROR;
    }
    // Check the validity of the class handle
    result = AUDIO_SMHC_VerifyHandle(hClass, COLLISION_HANDLE, COLLISION_CLASS_HANDLE);
    if(result != AUDIO_NO_ERROR)        
    {
        return result;
    }
    
    result = AUDIO_CM_CAN_PLAY;
    
    ptCurAcmInstanceInClass = ptAcmInstanceLListHeader;
    while (NULL != ptCurAcmInstanceInClass)
    {
        if (ptCollisionObject->uiPriority > ptCurAcmInstanceInClass->ptAudioCollisionObject->uiPriority)
        {
            result = AUDIO_CM_CANNOT_PLAY;
            break;
        }
        else
        {
            //stop playing instance.
            if (AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag(ptCurAcmInstanceInClass->hAudio))
            {
                    //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_181_112_2_18_1_1_59_3,(uint8*)"");
                       ASM_ASSERT_WARNING(FALSE);
        
                 return AUDIO_ERROR;
            }
        }
        
        ptCurAcmInstanceInClass = ptCurAcmInstanceInClass->ptNextInstance;
    }    
    
    return result;
}

/******************************************************************************
 ** audio collision class policy:                                             *
 **     if there is any instance in which private information the             *
 **     isSinglePlay is true.                                                 *
 **        if priority of the input instance is higher than or equal to       *
 **        the playing instance                                               *
 **            the input instance can be played and the playing instances     *
 **            should be stop.                                                *
 **        if priority of the input instance is lower than the playing        *
 **        instance                                                           *
 **            the input instance can't be played and the playing instances   *
 **            go on playing.                                                 *
 **     else                                                                  *
 **        both the input instance and the playing instance can be played.    *
 ** notes:                                                                    *
 **     this policy uses private informations--isSinglePlay.                  *
 **     in a class which uses this policy                                     *
 **         codec with isSinglePlay equal to 1 includes MIDI, MP3, AAC, vioce,*
 **                                      custom tone, dtmf tone, generic tone *
 **         codec with isSinglePlay equal to 0 includes ADPCM, PCM.           *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E ACM_Policy_SinglePlay(
AUDIO_COLLISION_OBJECT_T *ptCollisionObject,        //the collision object in the audio instance to be played
AUDIO_CM_INSTANCE_T      *ptAcmInstanceLListHeader, //instance linklist header in a class
HCOLLISIONCLASS          hClass                     //class handle
)
{
    AUDIO_CM_INSTANCE_T *ptCurAcmInstanceInClass = NULL;
    AUDIO_CM_CLASSOBJECT_T *ptCurAcmClassObject = NULL;
    int32 is_single_play_of_input_collision = 0;
    int32 is_single_play_of_cur_instance = 0;

    AUDIO_RESULT_E result = AUDIO_ERROR;

    //check input parameters
    if (NULL == ptCollisionObject)
    {
        return AUDIO_PARAM_ERROR;
    }
    // Check the validity of the class handle
    result = AUDIO_SMHC_VerifyHandle(hClass, COLLISION_HANDLE, COLLISION_CLASS_HANDLE);
    if(result != AUDIO_NO_ERROR)        
    {
        return result;
    }
    
    result = AUDIO_CM_CAN_PLAY;

    //find the input private information level
    ptCurAcmClassObject = ptCollisionObject->ptClassObjectLListHeader;
    while (NULL != ptCurAcmClassObject)
    {
        if (ptCurAcmClassObject->ptAcmClass->hClass == hClass)
        {
            is_single_play_of_input_collision = ((CLASS3PRIVATEINFO_T *)(ptCurAcmClassObject->pvPrivateBuffer))->isSinglePlay;
            break;
        }
        ptCurAcmClassObject = ptCurAcmClassObject->ptNextClassObject;
    }
    if (NULL == ptCurAcmClassObject)
    {
#line 251    
         //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_SinglePlay:error:NULL == ptCurAcmClassObject"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_255_112_2_18_1_1_59_4,(uint8*)"");
        ASM_ASSERT_WARNING(FALSE);
        
      return AUDIO_ERROR;
    }

    //judge the collision between input and each instance in the class
    ptCurAcmInstanceInClass = ptAcmInstanceLListHeader;
    while (NULL != ptCurAcmInstanceInClass)
    {
        //find the level for each instance.
        ptCurAcmClassObject = ptCurAcmInstanceInClass->ptAudioCollisionObject->ptClassObjectLListHeader;
        while (NULL != ptCurAcmClassObject)
        {
            if (ptCurAcmClassObject->ptAcmClass->hClass == hClass)
            {
                is_single_play_of_cur_instance = ((CLASS3PRIVATEINFO_T *)(ptCurAcmClassObject->pvPrivateBuffer))->isSinglePlay;
                break;
            }
            ptCurAcmClassObject =ptCurAcmClassObject->ptNextClassObject;
        }
        if (NULL == ptCurAcmClassObject)
        {
            //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_Exclusive:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_278_112_2_18_1_1_59_5,(uint8*)"");
               ASM_ASSERT_WARNING(FALSE);
         return AUDIO_ERROR;
        }

        //judge
        if (is_single_play_of_input_collision || is_single_play_of_cur_instance)
        {
            if (ptCollisionObject->uiPriority < ptCurAcmInstanceInClass->ptAudioCollisionObject->uiPriority)
            {
                result = AUDIO_CM_CANNOT_PLAY;
                break;
            }
            else
            {
                //stop the playing instance
                if (AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag(ptCurAcmInstanceInClass->hAudio))
                {
                      //SCI_TRACE_LOW:"acm_class_lib.c:ACM_Policy_SinglePlay:error:AUDIO_NO_ERROR != AUDIO_CM_SetNeedStopFlag"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,ACM_CLASS_LIB_296_112_2_18_1_1_59_6,(uint8*)"");
                     ASM_ASSERT_WARNING(FALSE);
            return AUDIO_ERROR;
                }
            }
        }

        ptCurAcmInstanceInClass = ptCurAcmInstanceInClass->ptNextInstance;
    }
    
    return result;
}

/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
}
#endif

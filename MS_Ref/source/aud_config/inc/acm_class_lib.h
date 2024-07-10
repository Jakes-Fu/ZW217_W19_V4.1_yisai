/******************************************************************************
 ** File Name:   acm_class_lib.h                                              *
 ** Date:        12/02/2005                                                   *
 ** Copyright:   2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description: This file includes the prototypes of audio collision policy, *
 **               struct definition  for class private information            *
*******************************************************************************/
#ifndef _ACM_CLASS_LIB_H_
#define _ACM_CLASS_LIB_H_

/******************************************************************************
 **                          Dependencies                                     *
*******************************************************************************/
#include "acm.h"

/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 **                          Data Structures                                  *
*******************************************************************************/
/*
  the private information struct can be redefined.
*/
//CLASS 1 private information
typedef struct Class1PrivateInfo_struct
{
    int32 level;
}CLASS1PRIVATEINFO_T;

//CLASS 2 has no private information

//CLASS 3 private information
typedef struct Class3PrivateInfo_struct
{
    int32 isSinglePlay;
}CLASS3PRIVATEINFO_T;

/******************************************************************************
 **                          Macro Definition                                 *
*******************************************************************************/
/******************************************************************************
 ** priority for all codecs:                                                  *
 **     priority level 1: custom tone, dtmf tone, generic tone.               *
 **     priority level 2: ring, including MIDI, MP3, ADPCM, PCM, AAC.         *
 **     priority level 3: voice.                                              *
 **     priority level 4: recording.                                          *
*******************************************************************************/
#define PRIORITY_LEVEL_1 2
#define PRIORITY_LEVEL_2 4
#define PRIORITY_LEVEL_3 6
#define PRIORITY_LEVEL_4 8

/******************************************************************************
 **                          Function prototype                               *
*******************************************************************************/
/******************************************************************************
 ** audio collision class policy:                                             *
 **     if level in private information is the same                           *
 **        they can be played in the meantime                                 *
 **     else                                                                  *
 **        if input instance priority is higher than or equal to the playing  *
 **            the input instance can't be played and the playing instances   *
 **            gon on playing.                                                *
 **        else                                                               *
 **            the input instance can be played and the playing instances     *
 **            stop playing.                                                  *
 ** notes:                                                                    *
 **     this policy uses private informations--level.                         *
 **     in a class which uses this policy                                     *
 **         codec used DSP VB is a level, including AMR, voice,               *
 **                                      custom tone, dtmf tone, generic tone *
 **         codec used ARM VB is another level, including MIDI, MP3,          *
 **                                                       ADPCM, AAC, PCM     *
*******************************************************************************/
PUBLIC AUDIO_RESULT_E ACM_Policy_DifferentLevelCollide(
AUDIO_COLLISION_OBJECT_T *ptCollisionObject,        //the collision object in the audio instance to be played
AUDIO_CM_INSTANCE_T      *ptAcmInstanceLListHeader, //instance linklist header in a class
HCOLLISIONCLASS          hClass                     //class handle
);

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
);

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
);

/******************************************************************************
 **                          Compiler Flag                                    *
*******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif //_ACM_CLASS_LIB_H_

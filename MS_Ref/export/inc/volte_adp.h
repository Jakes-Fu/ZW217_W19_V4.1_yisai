/******************************************************************************
** File Name:      voice_adp.h                                              
** Author:         shujing.dong                                              
** DATE:           12/13/2005                                                
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of voice_adp                     
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 12/13/2005     shujing.dong     Create.        
** 04/30/2008      Binggo.Zhou     Modify For Audio Service 2.0                                 *
******************************************************************************/
#ifndef _VOICE_ADP_H
#define _VOICE_ADP_H
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
#include "asm.h"
/**---------------------------------------------------------------------------**
**                        Debugging Flag                                     **
**---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{
    STATE_VOLTE_IDLE,
    STATE_VOLTE_START,
    STATE_VOLTE_CVS,
    STATE_VOLTE_AMR,
    STATE_VOLTE_STOPPED
}AUD_VOLTE_STATE;

struct AUDIO_VOLTE_TYPE
{
    SCI_EVENT_GROUP_PTR p_event;
    AUDIO_OBJECT_T* audio_obj;
    SCI_MUTEX_PTR mutex;
    BLOCK_ID  thread_id;
    AUD_VOLTE_STATE state;
    BLOCK_ID thread_id;
    AUDIO_DEVICE_HANDLE_T* s_ptDevice;
    HAUDIO uiHandleID;  /*!< Audio Handle.*/
};

typedef struct AUDIO_VOLTE_TYPE  AUDIO_VOLTE_T ;
	
/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/
	
/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Shujing.Dong
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC VOICE_RegCodecPlugger( //return codec handle
    HCOLLISION hCollision,               //collision handle
    AUDIO_CODEC_EXT_CFGINFO_T* ptCodecExtCfgInfo);          	
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _VOICE_ADP_H

//end of file

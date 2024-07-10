/******************************************************************************
 ** File Name:      audio_mtv.h                                            *
 ** Author:         Cherry.Liu                                                 *
 ** DATE:           18/9/2008                                                 *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    an adapter layer of mtv device for Audio Service.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 18/9/2008      Cherry.Liu    	Create.                                   *
 ******************************************************************************/


#ifndef _AUDIO_MTV_H
#define _AUDIO_MTV_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This Function is used to initialize the audio server. 
//  Author:         Cherry.Liu
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC MTV_RegCodecPlugger(
	HCOLLISION hCollision, 
	uint32 uiPriority
	);
/*****************************************************************************/
/*                                                                   		 */
/*       MTV_SoundInit() : mtv Sound Initialize				 		 */
/*                                                                   		 */
/*****************************************************************************/
PUBLIC void MTV_SoundInit(
	int samples, 
	int sample_rate 
	);
/*****************************************************************************/
/*                                                                   		 */
/*       MTV_SoundExit() : mtv Sound exit				 		 */
/*                                                                   		 */
/*****************************************************************************/
PUBLIC void MTV_SoundExit( 
	void 
	);

/*****************************************************************************/
/*                                                                   		 */
/*        MTV_SoundOpen() : MTV Sound Open                      	 */
/*                                                                   		 */
/*        samples: PCM number per calling Soundoutput				 		 */
/*        sample_rate: PCM samplerate								 		 */
/*****************************************************************************/
PUBLIC int MTV_SoundOpen( 
	int samples, 
	int sample_rate 
	);
/*****************************************************************************/
/*                                                                   		 */
/*        MTV_SoundClose() : MTV Sound Close                    		 */
/*                                                                   		 */
/*****************************************************************************/
PUBLIC void MTV_SoundClose(
	void
	);
/*****************************************************************************/
/*                                                                   		 */
/*       MTV_SoundOutput() :   MTV Sound Output samples in word  		 */
/*       system need to write the PCM in ringbuffer                 	     */
/*                                                                  		 */
/*        samples: PCM number per calling Soundoutput						 */
/*        buff_ptr: PCM buffer pointer										 */
/*****************************************************************************/
PUBLIC void MTV_SoundOutput( 
	int samples,
	int16 *buff_ptr
	);
/*****************************************************************************/
/*                                                                   		 */
/*       MTV_SoundOutputStereo() :  MTV Sound Output samples in word  		 */
/*       system need to write the PCM in ringbuffer                 	     */
/*                                                                  		 */
/*        samples: PCM number per calling Soundoutput						 */
/*        lbuff_ptr: left channel PCM buffer pointer										 */
/*        rbuff_ptr: right channel PCM buffer pointer										 */
/*****************************************************************************/
PUBLIC void MTV_SoundOutputStereo( 
	int samples, 
	int16 *lbuff_ptr, 
	int16 *rbuff_ptr
	);
/*****************************************************************************/
/*                                                                  		 */
/*       MTV_SoundGetPCMNum() :  MTV Sound Output samples in word 		     */
/*       system need to write the PCM in ringbuffer                 		 */
/*                                                                  		 */
/*       return PCM in halfword num								 			 */
/*****************************************************************************/
PUBLIC int MTV_SoundGetPCMNum(
	void
	);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AUDIO_MTV_H

// End of audio_mtv.h

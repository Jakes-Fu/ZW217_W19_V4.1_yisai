/******************************************************************************
 ** File Name:    audio_extpcm.h                                              *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  the api function for application                            *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ******************************************************************************/
#ifndef _AUDIO_EXTPCM_H_
#define _AUDIO_EXTPCM_H_

void ExtPCM_SoundExit( void );
/*****************************************************************************/
/*                                                                   		 */
/*       ExtPCM_SoundInit() : Extended PCM Sound Initialize				 	 */
/*                                                                   		 */
/*****************************************************************************/

void ExtPCM_SoundInit( int samples, int sample_rate );

/*****************************************************************************/
/*                                                                   		 */
/*        ExtPCM_SoundOpen() : Extend PCM Sound Open                      	 */
/*                                                                   		 */
/*        samples: PCM number per calling Soundoutput				 		 */
/*        sample_rate: PCM samplerate								 		 */
/*****************************************************************************/
int ExtPCM_SoundOpen( int samples, int sample_rate );

/*****************************************************************************/
/*                                                                   		 */
/*        ExtPCM_SoundClose() : Extended PCM Sound Close                     */
/*                                                                   		 */
/*****************************************************************************/
void ExtPCM_SoundClose( void ) ;

/*****************************************************************************/
/*                                                                   		 */
/*       ExtPCM_SoundOutput() :  Extended Sound Output samples in word  	 */
/*       system need to write the PCM in ringbuffer                 	     */
/*                                                                  		 */
/*        samples: PCM number per calling Soundoutput						 */
/*        buff_ptr: PCM buffer pointer										 */
/*****************************************************************************/
void ExtPCM_SoundOutput( int samples, short *buff_ptr);

/*****************************************************************************/
/*                                                                  		 */
/*       ExtPCM_SoundGetPCMNum() :  Extended PCM Sound Output samples in word*/
/*       system need to write the PCM in ringbuffer                 		 */
/*                                                                  		 */
/*       return PCM in halfword num								 			 */
/*****************************************************************************/
int ExtPCM_SoundGetPCMNum(void);

#endif /*_AUDIO_EXTPCM_H*/

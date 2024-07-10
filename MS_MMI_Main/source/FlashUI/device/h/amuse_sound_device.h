/*
* amuse_sound_device.h
* Copyright (C) 2011 Microrapid Inc
* Author: Black <black@Microrapid.com>
* 
* This file contains sound interface that need to 
* be implemented on different platforms.
*/
#ifndef _AMUSE_SOUND_DEVICE_H_
#define _AMUSE_SOUND_DEVICE_H_

#ifdef __cplusplus
extern "C"{
#endif

enum 
{
	AMUSE_SOUND_IDLE = 0,
	AMUSE_SOUND_READY,	
	AMUSE_SOUND_PLAY,
	AMUSE_SOUND_PAUSE,
	
};
	
/*
 * Init sound resource
 *
 * Parameters: none
 *
 * Returns: 0 for success, other for fail
 */
int amuse_sound_init(void);

/*
 * Open sound device.
 *
 * Parameters: none
 *
 * Returns: 0 for success, other for fail
 */
int amuse_sound_open(void);

/*
 * Close sound device.
 *
 * Parameters: none
 *
 * Returns: 0 for success, other for fail
 */
int amuse_sound_close(void);

/*
 * Lock the sound device.
 * For most platform, you must lock the device when you fill the sound data.
 *
 * Parameters: none
 *
 * Returns: 0 for success, other for fail
 */
int amuse_sound_lock(void);

/*
 * Unlock the device.
 *
 * Parameters: none
 *
 * Returns: 0 for success, other for fail
 */
int amuse_sound_unlock(void);

/*
* Release sound resource.
*
* Parameters: none
*
* Returns: 0 for success, other for fail
*/
void amuse_sound_release(void);

/*
* pause sound .
*
* Parameters: none
*
* Returns: 0 for success, other for fail
*/
int amuse_sound_pause(void);

/*
* resume sound. 
*
* Parameters: none
*
* Returns: 0 for success, other for fail
*/
int amuse_sound_resume(void);

int amuse_sound_open(void);

int amuse_sound_play(void);
		
int amuse_sound_close(void);

int amuse_sound_lock(void);

int amuse_sound_unlock(void);

int is_audio_playing(void);

void reset_audio_playing(void);

int amuse_sound_pause(void);

int amuse_sound_resume(void);

int amuse_sound_init(void);

void amuse_sound_release(void);

#ifdef __cplusplus
}
#endif

#endif 

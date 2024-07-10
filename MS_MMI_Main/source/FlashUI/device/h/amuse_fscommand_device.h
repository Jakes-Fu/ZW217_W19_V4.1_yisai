/*
* amuse_fscommand_device.h
* Author: Tim <tim@Microrapid.com>
* 
* This file contains common fscommand interfaces, which have
* different implementations in different devices.
*/

#ifndef _AMUSE_FSCOMMAND_DEVICE_H_
#define _AMUSE_FSCOMMAND_DEVICE_H_

/*
 * This function sets size of the display area.
 *
 * Parameters: 
 * size: 1 full-screen, others not full-screen
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_FullScreen(int size);

/*
 * This function retrieves battery level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, battery level number
 */
int amuse_fscmd2_GetBatteryLevel(void);

/*
 * This function gets max battery level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, max battery level number
 */
int amuse_fscmd2_GetMaxBatteryLevel(void);

/*
 * This function gets day value of a date.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 1 - 31, current day number of a month
 */
int amuse_fscmd2_GetDateDay(void);

/*
 * This function gets month value of a date.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 1 - 12, current month number
 */
int amuse_fscmd2_GetDateMonth(void);

/*
 * This function gets a weekday value of a date.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 6, Sunday - Saturday
 */
int amuse_fscmd2_GetDateWeekday(void);

/*
 * This function gets a year value of a date.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 9999, current year number
 */
int amuse_fscmd2_GetDateYear(void);

/*
 * This function gets device symbol.
 *
 * Parameters: 
 * buf: buffer to hold device name string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetDevice(char* buf, int len);

/*
 * This function gets device id.
 *
 * Parameters: 
 * buf: buffer to hold device id string
 * len: buffer length
 *
 * Returns: -1 unsupported; 0 success
 */
int amuse_fscmd2_GetDeviceID(char* buf, int len);

/*
 * This function get language type code.
 *
 * Parameters: 
 * buf: buffer to hold language string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetLanguage(char* buf, int len);

/*
 * This function gets locale long date.
 *
 * Parameters: 
 * buf: buffer to hold locale long date string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetLocaleLongDate(char* buf, int len);

/*
 * This function gets locale short date.
 *
 * Parameters: 
 * buf: buffer to hold locale short date string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetLocaleShortDate(char* buf, int len);

/*
 * This function gets locale time.
 *
 * Parameters: 
 * buf: buffer to hold locale time string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetLocaleTime(char* buf, int len);

/*
 * This function gets max signal level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, max signal level number
 */
int amuse_fscmd2_GetMaxSignalLevel(void);

/*
 * This function gets max volume level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, max volume level number
 */
int amuse_fscmd2_GetMaxVolumeLevel(void);

/*
 * This function gets network connect status.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 4, network connect status number
 */
int amuse_fscmd2_GetNetworkConnectStatus(void);

/*
 * This function gets network name.
 *
 * Parameters: 
 * buf: buffer to hold network name string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0 - 2, network status
 */
int amuse_fscmd2_GetNetworkName(char* buf, int len);

/*
 * This function gets network request status.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 10, network request status number
 */
int amuse_fscmd2_GetNetworkRequestStatus(void);

/*
 * This function gets network work status.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 3, network status number
 */
int amuse_fscmd2_GetNetworkStatus(void);

/*
 * This function get platform type string.
 *
 * Parameters: 
 * buf: buffer to hold platform string
 * len: buffer length
 *
 * Returns: -1, unsupported; 0, success
 */
int amuse_fscmd2_GetPlatform(char* buf, int len);

/*
 * This function gets power source.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 1, power source flag
 */
int amuse_fscmd2_GetPowerSource(void);

/*
 * This function gets signal level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, current signal level number
 */
int amuse_fscmd2_GetSignalLevel(void);

/*
 * This function gets hours value of current time.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 23, current hour number
 */
int amuse_fscmd2_GetTimeHours(void);

/*
 * This function gets minutes value of current time.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 59, current minute number
 */
int amuse_fscmd2_GetTimeMinutes(void);

/*
 * This function gets seconds value of current time.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0 - 59, current second number
 */
int amuse_fscmd2_GetTimeSeconds(void);

/*
 * This function gets time zone offset value.
 *
 * Parameters: 
 * offset: current time zone offset
 *
 * Returns: -1, unsupported; 0, success 
 */
int amuse_fscmd2_GetTimeZoneOffset(int* offset);

/*
 * This function gets volume level.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, current volume level number
 */
int amuse_fscmd2_GetVolumeLevel(void);

/*
 * This function starts vibration.
 *
 * Parameters: 
 * duration: vibration duration time
 * interval: interval time
 * times: times to vibrate
 *
 * Returns: -1, unsupported; 0, started; 1, fail to start
 */
int amuse_fscmd2_StartVibrate(int duration, int interval, int times);

/*
 * This function stops vibration.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; 0, stopped
 */
int amuse_fscmd2_StopVibrate(void);

/*
 * This function quits the player.
 *
 * Parameters: none
 *
 * Returns: -1, unsupported; others, success
 */
int amuse_fscmd2_Quit(void);

/*
 * This function launches another application.
 *
 * Parameters: 
 * path: application path and arguments
 *
 * Returns: none
 */
void amuse_fscmd_Launch(char* path);

#endif
